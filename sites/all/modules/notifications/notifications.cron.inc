<?php
// $Id: notifications.cron.inc,v 1.6.2.14 2008/11/07 23:58:38 jareyero Exp $
/**
 * Subscriptions module. Queue processing.
 * 
 * Callbacks for queue processing. They may be implemented by other modules
 *    _load_user()
 *    _user_allowed()
 *    _process_send()
 * 
 * @ TO DO: Support different languages for message localization
 */

// Number of users to process for each step
define('NOTIFICATIONS_STEP_ROWS', 1000);
define('NOTIFICATIONS_STEP_USERS', 1000);

// Minimum amount of seconds the process will need for clean-up tasks
// Just to make sure that after exhausting cron assigned time we'll have a few spare seconds for some cleanup
define('NOTIFICATIONS_TIME_MARGIN', 5);

/**
 * Function to be called on cron by the main notifications_cron
 * 
 * It will call each subscription_process for each interval a number of times
 * 
 * This should send out messages starting with immediate delivery. We send first immediate delivery
 * because the other ones can be added up for each period. 
 * Assumption: The bigger the interval, the longer delay it may admit (?) I.e. sending hourly email 
 * after 1 hour 15 mins may be ok if the system is overloaded.
 */
function notifications_process_run($cron = TRUE) {  
  notifications_log('Starting queue process');
  notifications_process('start');
  // There may be special time adjustments for cron
  if ($cron) {
    notifications_process('cron');
  }
  $stop = FALSE;
  $send_intervals = _notifications_send_intervals();
  unset($send_intervals[-1]);
  $max_sqid = notifications_process_prepare();
  foreach ($send_intervals as $interval => $name) {
    notifications_log('Processing send interval '. $name);
    while (notifications_process_queue($interval, $max_sqid)) {
      $stop = !notifications_process('check');
    }
    if ($stop) {
      notifications_log('Process stopped');
      break;
    }
  }
}

/**
 * Support of pull messaging
 * 
 * @see messaging_pull_pending()
 * 
 * @return
 *  Array of pending messages
 */
function notifications_process_pull($method, $users, $limit = 0, $delete = FALSE) {
  $messages = array();
  $maxsqid = 0;
  // Just fetches row in creation order de-duping same events on the step
  $sql = "SELECT uid, eid, module, MIN(sid) AS sid, MAX(sqid) AS sqid FROM {notifications_queue} ";
  $sql .= "WHERE send_method = '%s' AND uid IN (%s) GROUP BY uid, eid, module ORDER BY sqid";
  $str_uids = implode(',', $users);
  if ($limit) {
    $result = db_query_range($sql, $method, $str_uids, 0, $limit);
  }
  else {
    $result = db_query($sql, $method, $str_uids);
  }
  
  // Fetch and prepare messages
  while ($queue = db_fetch_object($result)) {
    $maxsqid = $queue->squid;
    $account = notifications_callback($queue->module, 'load_user', $queue->uid);
    $event = notifications_load_event($queue->eid);
    // Access control for event objects
    if (notifications_callback($queue->module, 'user_allowed', 'event', $account, $event)) {
      $subscriptions = array($queue->sid);
      $message = notifications_callback($queue->module, 'process_message', $account, $event, $subscriptions, $method);
      $message['uid'] = $queue->uid;
      $message['from'] = $event->uid;
      $messages[] = $message;
    }
  }
  // Delete returned rows
  if ($messages && $delete) {
    db_query("DELETE FROM {notifications_queue} WHERE sqid < %d AND send_method = '%s' AND uid IN (%s)", $maxsqid, $method, $str_uids);
  }

  // Return collected messages;
  return $messages;
}

/**
 * Prepare subscriptions queue
 * 
 * This is intended to avoid race conditions where new rows are added while the process is running
 * 
 * @return
 *   Max $sqid that will be processed this cron
 */
function notifications_process_prepare($module = 'notifications') {
  // Clean up event table. As events are created sequentially, we use this fact to speed up the query
  // This expiretime will prevent some race condition that occurs when the event is saved but the subs queue not yet populated  
  $expiretime = time() - 60;  
  db_query("DELETE FROM {notifications_event} WHERE created < %d AND eid < (SELECT MIN(eid) FROM {notifications_queue})", $expiretime);
  
  // This will get the latest notification in queue so we don't mess with new ones being created during cron run
  // It will also prevent clashes with the inmediate sending feature
  return db_result(db_query("SELECT max(sqid) FROM {notifications_queue}"));
}

/**
 * Controls and checks limits for queue processing
 * It can be used by other modules to add their own limits here, like number of sms sent, etc...
 * @param $op
 *   'start' => Start the counters
 *   'cron' => Special time adjustment for cron operations
 *   'init' => Start a new counter with $value limit
 *   'option' => Sets /gets options
 *      - debug
 *      - output Enables output for admin page
 * @return
 *   TRUE if we are yet under the processing limits
 */
function notifications_process($op = 'check', $name = NULL, $value = NULL) {
  static $limit = array(), $options = array();
  static $current = array('message' => 0, 'step' => 0);

  switch ($op) {
    case 'start':
      $defaults = variable_get('notifications_process_limit', array('time' => 0, 'message' => 0, 'row' => 0, 'percent' => 0));
      foreach ($defaults as $name => $value) {
        if ($value && !isset($limit[$name])) {
          $limit[$name] = $value;
        }
      }
      break;
    case 'cron':
      // Calculate time limit. We get the smaller of all these times in seconds
      // There's an issue with poormanscron not setting the cron semaphore so it will default to current time
      $timelimit = array();
      $cronstart = variable_get('cron_semaphore', time());
      
      // Max execution time may be zero meaning no limit, then no limits based on this
      if ($maxtime = ini_get('max_execution_time')) {
        $timelimit[] =  $cronstart + $maxtime - NOTIFICATIONS_TIME_MARGIN;
        if ($limit['percent']) {
          $timelimit[] = time() + $maxtime * $limit['percent'] / 100;
          unset($limit['percent']);
        }
      }
      // This is an absolute limit, applies always if set
      if ($limit['time']) {
        $timelimit[] = time() + $limit['time'];
      }
      if ($timelimit) {
        $limit['time'] = min($timelimit);
      }
      break;
    case 'init':
      $current[$name] = 0;
      $limit[$name] = $value;
      break;
    case 'count':
      $value =  $value ? $value : 1;
      isset($current[$name]) ? ($current[$name] += $value) : $current[$name] = $value;
      break;
    case 'option':
      if (isset($value)) {
        $options[$name] = $value;
      }
      return $options[$name];
  }
  
  $current['time'] = time();
  
  // Check all limits till we find a false one
  foreach ($limit as $name => $value) {
    if ($value && !empty($current[$name]) && $current[$name] >= $value) {
      watchdog('notifications', t('Reached processing limit on queue processing: %name = %value', array('%name' => $name, '%value' => $value)));
      return FALSE;
    }
  }
  return TRUE;
}

/**
 * Process rows given query conditions
 * 
 * This is used by the immediate sending feature
 * @see notifications_queue_query()
 * 
 * @param $conditions
 *   Array of query conditions
 */
function notifications_process_rows($conditions) {
  $account = NULL;
  $subscriptions = $events = $processed = array();
  $send_method = $send_interval = NULL;

  // Build query and fetch rows from queue
  $query = notifications_queue_query($conditions);
  $sql = "SELECT * FROM {notifications_queue} ";
  $sql .= " WHERE ". implode(' AND ', $query['where']);
  $sql .= " ORDER BY uid, send_method, send_interval";
  $result = db_query($sql, $query['args']);

  // Group rows by user, send_method, send_interval before sending
  // This loop has to run a final time after all rows have been fetched
  while (($queue = db_fetch_object($result)) || $processed) {
    if (!$account || !$queue || ($queue->uid != $account->uid) || $queue->send_method != $send_method || $queue->send_interval != $send_interval) {
      // New user or sending method, send if not the first row and reset
      if ($account && $events && $subscriptions) {
        notifications_process_send($account, $events, $subscriptions, $send_method, $send_interval);
        notifications_update_sent($account->uid, $send_method, $send_interval, time());
      }
      if ($processed) {
        notifications_queue_delete(array('sqids' => $processed));        
      }
      $subscriptions = $events = $processed = array();
      if ($queue) {
        $account = notifications_load_user($queue->uid);
        $send_method = $queue->send_method;
        $send_interval = $queue->send_interval;
      }
    }
    if ($queue) {
      $event = notifications_load_event($queue->eid);
      if (notifications_user_allowed('event', $account, $event)) {
        $events[$queue->eid] = $event;
        $subscriptions[$queue->eid][] = $queue->sid;
      }
      $processed[] = $queue->sqid;
    }
  }
}

/**
 * Process subscriptions queue
 * 
 * The subscriptions queue has the following fields
 * sqid, uid, eid, sid, digest
 * 
 * This function should be able of splitting the whole processing in several steps.
 * It will be called multiple times for each send interval
 * 
 * Messages will be processed for each send interval, send_method, user
 * 
 * @param $send_interval
 *   Send interval to process
 * @param $max_sqid
 *   Max queue id to process
 * @param $module
 *   Module's rows to process. There may be other types of rows sitting on the table
 * @return Number of rows processed
 * 
 * @ TODO Review time conditions
 */
function notifications_process_queue($send_interval, $max_sqid, $module = 'notifications') {
  $count = 0;
  // This is the time from which stored rows will be sent
  $timelimit = time() - $send_interval;

  // Get users to process messages for, with this time interval and ordered by squid
  // Order by last sent for this send interval
  // Note: If we get the users with more messages pending first this may save some time
  $sql = "SELECT q.uid, q.send_method, count(*) AS count FROM {notifications_queue} q ";
  $sql .= " LEFT JOIN {notifications_sent} su ON q.uid = su.uid AND q.send_interval = su.send_interval AND q.send_method = su.send_method ";
  $sql .= " WHERE q.cron = 1 AND q.send_interval = '%d' AND q.sqid <= %d AND q.module = '%s'";
  $sql .= " AND (su.uid IS NULL OR su.sent < %d) ";
   // Note: the group by su.sent seems to be needed by pgsql
  $sql .= " GROUP BY q.uid, q.send_method, su.sent ORDER BY su.sent";
  $result = db_query_range($sql, $send_interval, $max_sqid, $module, $timelimit, 0, NOTIFICATIONS_STEP_USERS);

  $sqid = 0;
  // @ TODO Add time conditions
  while (($user = db_fetch_object($result)) && notifications_process('check')) {
    notifications_log("Processing user $user->uid, rows $user->count, send_method $user->send_method");
    $events = $subscriptions = $processed = array();    
    $send_method = $user->send_method;
    // Users may be handled by a different module
    $account = notifications_callback($module, 'load_user', $user->uid);

    // Process all rows for this user. With some hard limit to prevent process lock ups.
    $result_subs = db_query_range("SELECT * FROM {notifications_queue} WHERE cron = 1 AND send_interval = '%d' AND uid = %d AND sqid <= %d ORDER BY send_method, sqid", $send_interval, $account->uid, $max_sqid, 0, NOTIFICATIONS_STEP_ROWS);
    while (($queue = db_fetch_object($result_subs)) && notifications_process('count', 'row')) {
      $count++;
      $processed[] = $sqid = $queue->sqid;      
      // Load event and check access
      $event = notifications_load_event($queue->eid, TRUE);
      if (notifications_callback($module, 'user_allowed', 'event', $account, $event)) {
        // This will take care of duplicated events
        $events[$queue->eid] = $event;
        // We keep track also of subscriptions originating this event
        $subscriptions[$queue->eid][] = $queue->sid;
        notifications_log("Processing queued sid=$queue->sid event=$queue->eid ($event->type, $event->action) send_method=$send_method");
      } else {
        notifications_log("Access denied for queued event sid=$queue->sid event=$queue->eid ($event->type, $event->action)");
      }
    }
    if ($events) {
      notifications_callback($module, 'process_send', $account, $events, $subscriptions, $send_method, $send_interval);
      notifications_callback($module, 'update_sent', $user->uid, $send_method, $send_interval, time());  
    }
    if ($processed && !notifications_process('option', 'debug')) {
       notifications_queue_delete(array('uid' => $user->uid, 'send_interval' => $send_interval, 'send_method' => $send_method, 'max_sqid' => $sqid));      
    }
  }
  
  return $count;
}

/**
 * Update user last time sent
 */
function notifications_update_sent($uid, $method, $interval, $time) {
  db_query("UPDATE {notifications_sent} SET sent = %d WHERE uid = %d AND send_interval = '%d' AND send_method = '%s'", $time, $uid, $interval, $method);
  if (!db_affected_rows()) {
    db_query("INSERT INTO {notifications_sent}(uid, send_interval, send_method, sent) VALUES(%d, '%d', '%s', %d)", $uid, $interval, $method, $time);
  }
}
/**
 * Message delivery.
 * 
 * Processes everything, included digestion and sends message/s.
 * 
 * Adds some more information into $message['notifications'] that may be used by other modules
 * 
 * @param $account
 *   User account to send the notification to
 * @param $events
 *   Array of loaded event objects to be processed
 * @param $subscriptions
 *   Array of arrays of subscription ids (sids) for each event(eid)
 */
function notifications_process_send($account, $events, $subscriptions, $send_method, $send_interval) {
  notifications_log("Sending out, method=$send_method, interval=$send_interval, events=". count($events));
  // Digest if send_interval > 0 (not immediate sending)
  if ($digest = notifications_digest_method($send_interval)) {
     $function = $digest['digest callback'];
     // It can be digested in more than one message by some other digest plug-in
     $messages = $function($account, $events, $subscriptions, $send_interval, $send_method);
  }
  else {
    $sender_option = variable_get('notifications_sender', 0);
    foreach ($events as $event) {
      $message = notifications_process_message($account, $event, $subscriptions[$event->eid], $send_method);
      // We pass on the full information so it can be used by modules implementing some of the hooks
      $message['notifications'] = array('events' => array($event), 'subscriptions' => $subscriptions);
      // Optional sender, if chosen will be the user account who produced the event
      // It will be up to the sending method modules what to do with this information.
      if ($sender_option) {
        $sender = notifications_load_user($event->uid);
        $message['sender_name'] = $sender->name;
        if ($sender_option == 2) {
          $message['sender_account'] = $sender;
        }
      }
      $messages[] = $message;
    }
  }
  // Now send messages
  foreach ($messages as $message) {
    notifications_process('count', 'send');
    // notifications_log($message, 'message');
    notifications_message_send($account, $message, $send_method);
  }
  
  return $messages;
}

/**
 * Digest multiple events in a single message, short format.
 * 
 * @return array with messages ready to be sent
 */
function notifications_process_digest_short($account, $events, $subscriptions, $send_interval, $send_method) {
  // Compile list of events for each object
  $list = array();

  // Build up the digested list with text replacement
  // We need text replacement for each line because it depends on different objects
  foreach ($events as $event) {
    notifications_log($event, 'digesting');
    $sid = is_array($subscriptions[$event->eid]) ? array_shift($subscriptions[$event->eid]) : 0;
    $subscription = $sid ? notifications_load_subscription($sid) : NULL;
    $objects = $event->objects + array('user' => $account, 'subscription' => $subscription);
    // $info = notifications_event_types($event->type, $event->action);
    $digest = nofitications_digest_event_info($event);
    $digest_type = $digest['type'];
    $digest_value = $digest['value'];

    if (!isset($list[$digest_type][$digest_value]['group'])) {
      $group = array(      
        'title' => notifications_digest_group($digest, 'title', $send_method),
        'footer' => notifications_digest_group($digest, 'footer', $send_method),
      );
      // The objects passed here for tokens will be the ones from the first event only
      $list[$digest_type][$digest_value]['group'] = notifications_text_replace($group, $objects);
      notifications_log("Digesting object (type=$digest_type value=$digest_value)");
    }
    // Check duplicate notifications for the same event so we do some deduping
    if (!isset($list[$digest_type][$digest_value]['line'][$event->eid])) {
      $line = notifications_digest_line($event, $send_method, $objects);
      $list[$digest_type][$digest_value]['line'][$event->eid] = notifications_text_replace($line, $event->objects);
    }
  }
  // Create message. Do all this in one replacement, then strip out the subject
  $text['subject'] = notifications_message_part('digest', 'subject', $send_method);
  $text['header'] = notifications_message_part('digest', 'header', $send_method);
  $text['footer'] = notifications_message_part('digest', 'footer', $send_method);

  // We dont pass a subscription object here, won't be too much use anyway
  $text = notifications_text_replace($text, array('user' => $account, 'subscription' => NULL));
  
  // Compose body. All these lines have been text replaced
  $body = theme('notifications_digest_short_body', $text, $list);
    
  // Build the final digested message, and return in an array
  $message = array(
    'subject' => $text['subject'], 
    'body' => $body,
    'events' => $events,
    'subscriptions' => $subscriptions,
    'digest' => 'short',
  );
  return array($message);
}

/**
 * Digest multiple events in a single message, long format.
 * 
 * @return array with messages ready to be sent
 */
function notifications_process_digest_long($account, $events, $subscriptions, $send_interval, $send_method) {
  // Build the message body as an array of event notifications
  $body = array();

  // Build up the digested list with text replacement, body as big array
  // We need text replacement for each line because it depends on different objects
  foreach ($events as $event) {
    notifications_log($event, 'digesting');
    // We use the regular template for the events
    $part = notifications_message_part('event', 'main', $send_method, $event);
    // Pass only the first subscription here
    $sid = is_array($subscriptions[$event->eid]) ? array_shift($subscriptions[$event->eid]) : 0;
    $subscription = $sid ? notifications_load_subscription($sid) : NULL;
    $objects = $event->objects + array('user' => $account, 'subscription' => $subscription);
    $body[] = notifications_text_replace($part, $objects);
  }
  
  // Create message. Do all this in one replacement, then strip out the subject
  $text['subject'] = notifications_message_part('digest', 'subject', $send_method);
  $text['header'] = notifications_message_part('digest', 'header', $send_method);
  $text['footer'] = notifications_message_part('digest', 'footer', $send_method);

  // We dont pass a subscription object here, won't be too much use anyway
  $text = notifications_text_replace($text, array('user' => $account, 'subscription' => NULL));
  
  // Compose body. All these lines have been text replaced
  $body = theme('notifications_digest_long_body', $text['header'], $body, $text['footer']);
  
  // Build the final digested message, and return in an array
  $message = array(
    'subject' => $text['subject'], 
    'body' => $body,
    'events' => $events,
    'subscriptions' => $subscriptions,
    'digest' => 'long',
  );
  return array($message);
}

/**
 * Get digest information for an event.
 */
function nofitications_digest_event_info($event) {
  $info = notifications_event_types($event->type, $event->action);
  $type = $info['digest'][0];
  $field = $info['digest'][1];
  // Check object and values
  $object = !empty($event->objects[$type]) ? $event->objects[$type] : NULL;
  $value = $object && isset($object->$field) ? $object->$field : 0;
  return array('type' => $type, 'field' => $field, 'value' => $value, 'object' => $object);  
}

/**
 * Creates a single message for a single event
 * 
 * @param $account
 *   Destination user account
 * @param $event
 *   Event object which caused this notification
 * @param $subscriptions
 *   Array of subscription ids
 * 
 * @return
 *   Message array 
 */
function notifications_process_message($account, $event, $subscriptions, $send_method) {
  $info = notifications_event_text($event);

  // Create message. Do all this in one replacemente
  $text = array(
    'subject' => notifications_message_part('event', 'subject', $send_method, $event),
    'header' => notifications_message_part('event' , 'header', $send_method, $event),
    'event'  => notifications_message_part('event', 'main', $send_method, $event),
    'footer' => notifications_message_part('event', 'footer', $send_method, $event),
  );

  // We pass only the first subscription, which is at least something
  // @ TODO Handle nicely the case where there are more than one subscription
  if ($sid = array_shift($subscriptions)) {
    $subscription = notifications_load_subscription($sid);
  }
  else {
    $subscription = NULL;
  }
  $objects = array('user' => $account, 'event' => $event, 'subscription' => $subscription);
  $objects = array_merge($objects, $event->objects);
  $text = notifications_text_replace($text, $objects);

  // Get subject out of text and build the message array
  $subject = $text['subject'];
  unset($text['subject']);
  return array('subject' => $subject, 'body' => $text);
}

/**** Retrieving and replacing text parts, interfacing with tokens and messaging module ****/

/**
 * Get message part
 * 
 * It searches for optional message group keys for options defaulting to $type
 * 1. $type-[$event->type]-[$event->action]
 * 2. $type-[$event->type]
 * 3. $type
 * 
 * @param $type
 *   Message type to send, either 'event' or 'digest'
 * @param $key
 *   Id of message part, ie 'header'
 * @param $method
 *   Method by which message will be sent. Normally 'mail'
 * @param $param
 *   Event data
 *
 * @return
 *   Part of the message.
 *
 */
function notifications_message_part($type, $key, $method, $param = NULL) {
  // If event passed check for predefined text or get optional keys from event
  if ($type == 'event' && is_object($param)) {
    if (isset($param->text[$key])) {
      return $param->text[$key];  
    } else {
      $options = array($param->type, $param->action);
    }
  } elseif ($method == 'test') {
    // Little trick for this to be testable
    return "$type $key [type-name] [title] [site-name]";
  } else {
    $options = is_array($param) ? $param : array();
  }
  $keyparts = array_merge(array('notifications-'. $type), $options);
  // Output some debugging info in case we dont find a suitable message part
  $output = "[UNDEFINED type = $type, method = $method, key = ". implode('-', $keyparts) .']';
  while ($keyparts) {
    $groupkey = implode('-', $keyparts);
     if ($text = messaging_message_part($groupkey, $key, $method)) {
      $output = $text == MESSAGING_EMPTY ? '' : $text;
      break;
    }
    // If no text trim out latest part of the key and retry
    array_pop($keyparts);
  }
  return $output;
}

/**
 * Text replacement with tokens
 */
function notifications_text_replace($text, $objects) {
  // Add some token types
  $objects['global'] = NULL;
  return token_replace_multiple($text, $objects);
}

/**
 * Message sending
 */
function notifications_message_send($account, $message, $send_method) {
  $message['type'] = 'notifications';
  notifications_process('count', 'message');
  messaging_message_send_user($account, $message, $send_method);
}

/**
 * Get texts for event
 * 
 * @ TODO Support for configurable texts
 */
function notifications_event_text($event) {
  $info = notifications_event_types($event->type, $event->action);
  return $info;
}

/**
 * Get users with static caching
 */
function notifications_load_user($uid) {
  return messaging_load_user($uid);
}

/**
 * Get events with static caching
 */
function notifications_load_event($id) {
  static $cache = array();
  if (!array_key_exists($id, $cache)) {
    $event = db_fetch_object(db_query("SELECT * FROM {notifications_event} WHERE eid = %d", $id));
    $event->params = unserialize($event->params);
    // Load aditional objects for the event
    $event->objects = array();
    notifications_module_invoke('event load', $event);
    $cache[$id] = $event;
  }
  return $cache[$id];
}

/**
 * Delete rows from subscriptions queue
 * 
 * @see notifications_queue_query()
 * 
 * Note: Handle with care if no params may delete all rows
 */
function notifications_queue_delete($params) {
  $query = notifications_queue_query($params);
  db_query("DELETE FROM {notifications_queue} WHERE ". implode(' AND ', $query['where']), $query['args']);
}

/**
 * Build query conditions for queue queries
 * 
 * @param $params
 *   Array of parameters, field => value form
 *   Special parameters
 *     'max_squid' => max squid to delete
 *     'rows' => array of squid values to delte
 * @return
 *   Array with 'where' and 'args' elements. Each of them is an array
 */
function notifications_queue_query($params) {
  $where = $args = array();
  
  foreach ($params as $field => $value) {
    switch ($field) {
      case 'max_sqid':
        $where[] = "sqid <= %d";
        $args[] = $value;
        break;
      case 'sqids':
        $where[] = "sqid IN (%s)";
        $args[] = implode(',', array_map('db_escape_string', $value));
        break;      
      default:
        $where[] = "$field = '%s'";
        $args[] = $value;
        break;
    }
  }  
  return array('where' => $where, 'args' => $args);
}

/**
 * Log for debugging
 */
function notifications_log($info = NULL, $type = 'info') {
  static $logs;
  if ($info) {
    $message = $type .': ';
    $message .= is_string($info) ? $info : print_r($info, TRUE);
    $logs[] = $message;
    if ($type == 'watchdog') {
      watchdog('notifications', $info);
    }
  }
  else {
    return $logs;
  }
}

/** Themeable functions **/

/**
 * Theme notifications digest
 * 
 * @param $text
 *   Array with message parts, currently only 'header' and 'footer'
 * @param $list
 *   Structured array with list of digested items. For each object type
 *   'type' => (  // Type may be node, user, etc...
 *      'oid' => ( // One for each object, may be nid, uid...
 *        'group' => Group title and footer 
 *        'line' => Array of lines, one for each related event
 *       )
 *   )   
 * @return
 *   Structured array with 'header', 'footer', and multiple text lines
 */
function theme_notifications_digest_short_body($text, $list) {
  $body['header'] = $text['header'];
  foreach ($list as $type => $objects) {
    foreach ($objects as $oid => $data) {      
      $body['content'][] = $data['group']['title'];
      foreach ($data['line'] as $line) {
        $body['content'][] = theme('notifications_digest_short_line', $line, $data['group']);
      }
      $body['content'][] = $data['group']['footer'];
    }
  }
  $body['footer'] = $text['footer'];
  return $body;
}

/**
 * Single line of text
 */
function theme_notifications_digest_short_line($line, $group) {
  return '- ' . $line;
}

/**
 * Build the message body for long digests. 
 * 
 * Actually we do nothing here, but it will be themeable.
 */
function theme_notifications_digest_long_body($header, $content, $footer) {
  return array('header' => $header, 'content' => $content, 'footer' => $footer);
}

/**
 * Get text parts for digests.
 * 
 * Useful to get the group title and footer given some kind of digesting
 */
function notifications_digest_group($digest, $part, $method) {
  static $texts = array();

  $type = $digest['type'];
  $value = $digest['value'];
  
  if (!isset($texts[$type][$value][$part][$method])) {
    if ($line = notifications_message_part('digest', $part, $method, array($type, $digest['field']))) {
      $output = $line;
    } else {
      $output = '';
    }
    $texts[$type][$value][$part][$method] = $output;
  }
  
  return $texts[$type][$value][$part][$method];
}

/**
 * Digest each line, with some caching for performance
 */
function notifications_digest_line($event, $method) {
  static $digest = array();
  
  if (!isset($digest[$event->eid][$method])) {
    // The event may have an specific digest line, otherwise use template if present or even information
    if (!empty($event->text['digest'])) {
      $line = $event->text['digest'];
    } elseif ($part = notifications_message_part('event', 'digest', $method, $event)) {
      $line = $part;
    } else { // Get it from event information
      $info = notifications_event_types($event->type, $event->action);
      $line = $info['line'];
    }    
    $digest[$event->eid][$method] = $line;
  }
  
  return $digest[$event->eid][$method];
}
