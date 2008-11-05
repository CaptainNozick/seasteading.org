<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language ?>" lang="<?php print $language ?>">
	<head>
		<title><?php print $head_title ?></title>
		<?php print $head ?>
		<?php print $styles ?>
		<?php print $scripts ?>
		<style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/style.css";</style>
		<style type="text/css" media="print">@import "<?php print base_path() . path_to_theme() ?>/print.css";</style>
		<!--[if lt IE 7]>
			<style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/fix-ie.css";</style>
		<![endif]-->
		<!--[if IE 7]>
			<style type="text/css" media="all">@import "<?php print base_path() . path_to_theme() ?>/fix-ie_7.css";</style>
		<![endif]-->
		<script type="text/javascript" src="<?=base_path() . path_to_theme() ?>/scripts/application.js"></script>
	</head>