<?php
// $Id: mail.inc,v 1.1.2.1 2008/02/16 20:54:55 chx Exp $

/**
 * To generate this file from Drupal 6 mail.inc, delete drupal_mail and
 * drupal_mail_send functions.
 */

/**
 * Perform format=flowed soft wrapping for mail (RFC 3676).
 *
 * We use delsp=yes wrapping, but only break non-spaced languages when
 * absolutely necessary to avoid compatibility issues.
 *
 * We deliberately use LF rather than CRLF, see drupal_mail().
 *
 * @param $text
 *   The plain text to process.
 * @param $indent (optional)
 *   A string to indent the text with. Only '>' characters are repeated on
 *   subsequent wrapped lines. Others are replaced by spaces.
 */
function drupal_wrap_mail($text, $indent = '') {
  // Convert CRLF into LF.
  $text = str_replace("\r", '', $text);
  // See if soft-wrapping is allowed.
  $clean_indent = _drupal_html_to_text_clean($indent);
  $soft = strpos($clean_indent, ' ') === FALSE;
  // Check if the string has line breaks.
  if (strpos($text, "\n") !== FALSE) {
    // Remove trailing spaces to make existing breaks hard.
    $text = preg_replace('/ +\n/m', "\n", $text);
    // Wrap each line at the needed width.
    $lines = explode("\n", $text);
    array_walk($lines, '_drupal_wrap_mail_line', array('soft' => $soft, 'length' => strlen($indent)));
    $text = implode("\n", $lines);
  }
  else {
    // Wrap this line.
    _drupal_wrap_mail_line($text, 0, array('soft' => $soft, 'length' => strlen($indent)));
  }
  // Empty lines with nothing but spaces.
  $text = preg_replace('/^ +\n/m', "\n", $text);
  // Space-stuff special lines.
  $text = preg_replace('/^(>| |From)/m', ' $1', $text);
  // Apply indentation. We only include non-'>' indentation on the first line.
  $text = $indent . substr(preg_replace('/^/m', $clean_indent, $text), strlen($indent));

  return $text;
}

/**
 * Transform an HTML string into plain text, preserving the structure of the
 * markup. Useful for preparing the body of a node to be sent by e-mail.
 *
 * The output will be suitable for use as 'format=flowed; delsp=yes' text
 * (RFC 3676) and can be passed directly to drupal_mail() for sending.
 *
 * We deliberately use LF rather than CRLF, see drupal_mail().
 *
 * This function provides suitable alternatives for the following tags:
 * <a> <em> <i> <strong> <b> <br> <p> <blockquote> <ul> <ol> <li> <dl> <dt>
 * <dd> <h1> <h2> <h3> <h4> <h5> <h6> <hr>
 *
 * @param $string
 *   The string to be transformed.
 * @param $allowed_tags (optional)
 *   If supplied, a list of tags that will be transformed. If omitted, all
 *   all supported tags are transformed.
 * @return
 *   The transformed string.
 */
function drupal_html_to_text($string, $allowed_tags = NULL) {
  // Cache list of supported tags.
  static $supported_tags;
  if (empty($supported_tags)) {
    $supported_tags = array('a', 'em', 'i', 'strong', 'b', 'br', 'p', 'blockquote', 'ul', 'ol', 'li', 'dl', 'dt', 'dd', 'h1', 'h2', 'h3', 'h4', 'h5', 'h6', 'hr');
  }

  // Make sure only supported tags are kept.
  $allowed_tags = isset($allowed_tags) ? array_intersect($supported_tags, $allowed_tags) : $supported_tags;

  // Make sure tags, entities and attributes are well-formed and properly nested.
  $string = _filter_htmlcorrector(filter_xss($string, $allowed_tags));

  // Apply inline styles.
  $string = preg_replace('!</?(em|i)>!i', '/', $string);
  $string = preg_replace('!</?(strong|b)>!i', '*', $string);

  // Replace inline <a> tags with the text of link and a footnote.
  // 'See <a href="http://drupal.org">the Drupal site</a>' becomes
  // 'See the Drupal site [1]' with the URL included as a footnote.
  _drupal_html_to_mail_urls(NULL, TRUE);
  $pattern = '@(<a[^>]+?href="([^"]*)"[^>]*?>(.+?)</a>)@i';
  $string = preg_replace_callback($pattern, '_drupal_html_to_mail_urls', $string);
  $urls = _drupal_html_to_mail_urls();
  $footnotes = '';
  if (count($urls)) {
    $footnotes .= "\n";
    for ($i = 0, $max = count($urls); $i < $max; $i++) {
      $footnotes .= '['. ($i + 1) .'] '. $urls[$i] ."\n";
    }
  }

  // Split tags from text.
  $split = preg_split('/<([^>]+?)>/', $string, -1, PREG_SPLIT_DELIM_CAPTURE);
  // Note: PHP ensures the array consists of alternating delimiters and literals
  // and begins and ends with a literal (inserting $null as required).

  $tag = FALSE; // Odd/even counter (tag or no tag)
  $casing = NULL; // Case conversion function
  $output = '';
  $indent = array(); // All current indentation string chunks
  $lists = array(); // Array of counters for opened lists
  foreach ($split as $value) {
    $chunk = NULL; // Holds a string ready to be formatted and output.

    // Process HTML tags (but don't output any literally).
    if ($tag) {
      list($tagname) = explode(' ', strtolower($value), 2);
      switch ($tagname) {
        // List counters
        case 'ul':
          array_unshift($lists, '*');
          break;
        case 'ol':
          array_unshift($lists, 1);
          break;
        case '/ul':
        case '/ol':
          array_shift($lists);
          $chunk = ''; // Ensure blank new-line.
          break;

        // Quotation/list markers, non-fancy headers
        case 'blockquote':
          // Format=flowed indentation cannot be mixed with lists.
          $indent[] = count($lists) ? ' "' : '>';
          break;
        case 'li':
          $indent[] = is_numeric($lists[0]) ? ' '. $lists[0]++ .') ' : ' * ';
          break;
        case 'dd':
          $indent[] = '    ';
          break;
        case 'h3':
          $indent[] = '.... ';
          break;
        case 'h4':
          $indent[] = '.. ';
          break;
        case '/blockquote':
          if (count($lists)) {
            // Append closing quote for inline quotes (immediately).
            $output = rtrim($output, "> \n") ."\"\n";
            $chunk = ''; // Ensure blank new-line.
          }
          // Fall-through
        case '/li':
        case '/dd':
          array_pop($indent);
          break;
        case '/h3':
        case '/h4':
          array_pop($indent);
        case '/h5':
        case '/h6':
          $chunk = ''; // Ensure blank new-line.
          break;

        // Fancy headers
        case 'h1':
          $indent[] = '======== ';
          $casing = 'drupal_strtoupper';
          break;
        case 'h2':
          $indent[] = '-------- ';
          $casing = 'drupal_strtoupper';
          break;
        case '/h1':
        case '/h2':
          $casing = NULL;
          // Pad the line with dashes.
          $output = _drupal_html_to_text_pad($output, ($tagname == '/h1') ? '=' : '-', ' ');
          array_pop($indent);
          $chunk = ''; // Ensure blank new-line.
          break;

        // Horizontal rulers
        case 'hr':
          // Insert immediately.
          $output .= drupal_wrap_mail('', implode('', $indent)) ."\n";
          $output = _drupal_html_to_text_pad($output, '-');
          break;

        // Paragraphs and definition lists
        case '/p':
        case '/dl':
          $chunk = ''; // Ensure blank new-line.
          break;
      }
    }
    // Process blocks of text.
    else {
      // Convert inline HTML text to plain text.
      $value = trim(preg_replace('/\s+/', ' ', decode_entities($value)));
      if (strlen($value)) {
        $chunk = $value;
      }
    }

    // See if there is something waiting to be output.
    if (isset($chunk)) {
      // Apply any necessary case conversion.
      if (isset($casing)) {
        $chunk = $casing($chunk);
      }
      // Format it and apply the current indentation.
      $output .= drupal_wrap_mail($chunk, implode('', $indent)) ."\n";
      // Remove non-quotation markers from indentation.
      $indent = array_map('_drupal_html_to_text_clean', $indent);
    }

    $tag = !$tag;
  }

  return $output . $footnotes;
}

/**
 * Helper function for array_walk in drupal_wrap_mail().
 *
 * Wraps words on a single line.
 */
function _drupal_wrap_mail_line(&$line, $key, $values) {
  // Use soft-breaks only for purely quoted or unindented text.
  $line = wordwrap($line, 77 - $values['length'], $values['soft'] ? "  \n" : "\n");
  // Break really long words at the maximum width allowed.
  $line = wordwrap($line, 996 - $values['length'], $values['soft'] ? " \n" : "\n");
}

/**
 * Helper function for drupal_html_to_text().
 *
 * Keeps track of URLs and replaces them with placeholder tokens.
 */
function _drupal_html_to_mail_urls($match = NULL, $reset = FALSE) {
  global $base_url, $base_path;
  static $urls = array(), $regexp;
  
  if ($reset) {
    // Reset internal URL list.
    $urls = array();
  }
  else {
    if (empty($regexp)) {
      $regexp = '@^'. preg_quote($base_path, '@') .'@';
    }
    if ($match) {
      list(, , $url, $label) = $match;
      // Ensure all URLs are absolute.
      $urls[] = strpos($url, '://') ? $url : preg_replace($regexp, $base_url .'/', $url);
      return $label .' ['. count($urls) .']';
    }
  }
  return $urls;
}

/**
 * Helper function for drupal_wrap_mail() and drupal_html_to_text().
 *
 * Replace all non-quotation markers from a given piece of indentation with spaces.
 */
function _drupal_html_to_text_clean($indent) {
  return preg_replace('/[^>]/', ' ', $indent);
}

/**
 * Helper function for drupal_html_to_text().
 *
 * Pad the last line with the given character.
 */
function _drupal_html_to_text_pad($text, $pad, $prefix = '') {
  // Remove last line break.
  $text = substr($text, 0, -1);
  // Calculate needed padding space and add it.
  if (($p = strrpos($text, "\n")) === FALSE) {
    $p = -1;
  }
  $n = max(0, 79 - (strlen($text) - $p));
  // Add prefix and padding, and restore linebreak.
  return $text . $prefix . str_repeat($pad, $n - strlen($prefix)) ."\n";
}
