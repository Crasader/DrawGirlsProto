<?php
include_once("../lib.php");


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
$lname = explode("_",$this_page_name);
echo $lname[1];
}
?>

</title>
<script>
	var gid = '<?=$gid?>';
</script>
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

<?php
if(!$gid){
	echo "게임아이디를 찾을수없습니다. <a href=index.php>select gid</a>";
	exit;
}
?>



<?php if(!$menuoff){ ?>

<center><font size=5><b><?=$gid?></b></font><br>

[<a href=manage_userInfo.php?gid=<?=$gid?>>운영툴</a>] [<a href=admin_help.php?gid=<?=$gid?>>관리툴</a>]
<br><br>
</center>
<table border=1 align=center>
<tr>
<td>

<table border=1>
<tr>
<td bgcolor=cccccc><b>System</b></td>
<td><a href=manage_userInfo.php?gid=<?=$gid?>>userInfo<br><font color=red>기본정보조회</font></a></td>
<td><a href=manage_characterHistory.php?gid=<?=$gid?>>characterHistory<br><font color=red>캐릭터히스토리</font></a></td>
<td><a href=manage_puzzleHistory.php?gid=<?=$gid?>>puzzleHistory<br><font color=red>퍼즐정보</font></a></td>
<td><a href=manage_pieceHistory.php?gid=<?=$gid?>>pieceHistory<br><font color=red>피스정보</font></a></td>
<td><a href=manage_cardHistory.php?gid=<?=$gid?>>cardHistory<br><font color=red>카드정보</font></a></td>
<td><a href=manage_message.php?gid=<?=$gid?>>Message<br><font color=red>수신함관리</font></a></td>
<td><a href=manage_userProperty.php?gid=<?=$gid?>>userProperty<br><font color=red>보관함관리</font></a></td>
<td><a href=manage_userPropertyHistory.php?gid=<?=$gid?>>userPropertyHistory<br><font color=red>보관함히스토리</font></a></td>
<td><a href=manage_archivementHistory.php?gid=<?=$gid?>>archivementHistory<br><font color=red>업적히스토리</font></a></td>
<td><a href=manage_giftBoxHistory.php?gid=<?=$gid?>>giftBoxHistory<br><font color=red>선물함히스토리</font></a></td>
<td><a href=manage_balance.php?gid=<?=$gid?>>balance<br><font color=red>벨런스</font></a></td>
<td><a href=manage_notice.php?gid=<?=$gid?>>notice<br><font color=red>공지</font></a></td>
</tr>
</table>

</td></tr></table>
<br>
<?php } ?>