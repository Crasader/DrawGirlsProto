<?php
include "../lib.php";


?>

<html>
<head>
<title>
<?php
if($title)echo $title;
else {
$php_self = explode("/", $_SERVER[PHP_SELF]);
$url_cnt = count($php_self) - 1;
$this_page = $php_self[$url_cnt];
$this_page_name = explode(".",$this_page);
$this_page_name = $this_page_name[count($this_page_name)-2];
echo $this_page_name;
}
?>

</title>
		<script src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
		<script src="lib.js"></script>
		<script src="LQDataTable.js"></script>
		<!-- Latest compiled and minified CSS -->

<!-- Bootstrap styles -->
<link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css">
<!-- Generic page styles -->
<link rel="stylesheet" href="jqueryupload/css/style.css">
<!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
<link rel="stylesheet" href="jqueryupload/css/jquery.fileupload.css">

<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
</head>
<body>






<?php if(!$menuoff){ ?>

<table border=1 align=center>
<tr>
<td><a href=admin_help.php>api</a></td>
<td><a href=admin_commonSetting.php>commonSetting</a></td>
<td><a href=admin_notice.php>notice</a></td>
<td><a href=admin_puzzle.php>puzzle</a></td>
<td><a href=admin_theme.php>theme</a></td>
<td><a href=admin_stage.php>stage</a></td>
<td><a href=admin_eventstage.php>eventstage</a></td>
<td><a href=admin_card.php>card</a></td>
<td><a href=admin_character.php>character</a></td>
<td><a href=admin_item.php>item</a></td>
<td><a href=admin_language.php>language</a></td>
<td><a href=admin_mission.php>mission</a></td>
<td><a href=admin_monster.php>monster</a></td>
<td><a href=admin_movement.php>movement</a></td>
<td><a href=admin_pattern.php>pattern</a></td>
<td><a href=admin_images.php>images</a></td>
<td><a href=admin_resources.php>resources</a></td>
<td><a href=admin_keyintvalue.php>key-intvalue</a></td>
<td><a href=admin_log.php>log</a></td>
<td><a href=admin_message.php>message</a></td>
<td><a href=admin_statsClear.php>stats-clear</a></td>
<td><a href=admin_util.php>util</a></td>
</tr>
</table>
<br>


<?php } ?>