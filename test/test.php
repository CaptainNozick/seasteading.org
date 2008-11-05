<?php
	include('parse_ics.php');
	$ical = parse_ical('http://movement.meetup.com/72/calendar/ical/Bay+Area+Seasteading+Socials');
?>
<pre>
<?php print_r($ical); ?>
</pre>
<hr />
<?php foreach ($ical as $event) : ?>
<?=$event['summary']?> : <?=date('d-m-Y @ g:i a', ($event['start_unix'] - 25200) ) ?><br />
<?php endforeach ?>