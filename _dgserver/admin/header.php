<?php
include_once("../lib.php");

$admin = new AdminUser($_SESSION["admin_no"]);
if(!$admin->isLogined()){
	?>

	<script>
	location.href = 'index.php';
	</script>

	<?php
	exit;
}
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
		<script src="http://code.jquery.com/ui/1.10.4/jquery-ui.min.js"></script>
		<link rel="stylesheet" href="http://code.jquery.com/ui/1.10.4/themes/black-tie/jquery-ui.css">
		<script src="jquery-ui-timepicker-addon.js"></script>
		<script src="lib.js"></script>
		<script src="LQDataTable.js"></script>
		<script src="LQEditor.js"></script>
		<link rel="stylesheet" href="LQDataTable.css">
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
<td><a href=admin_help.php?gid=<?=$gid?>>API<br><font color=red>에이피아이</font></a></td>
<td><a href=admin_apiTest.php?gid=<?=$gid?>>APITest<br><font color=red>에이피아이테스트</font></a></td>
<td><a href=admin_util.php?gid=<?=$gid?>>util<br><font color=red>유틸</font></a></td>
<td><a href=admin_convertor.php?gid=<?=$gid?>>convertor<br><font color=red>convertor</font></a></td>
<td><a href=admin_pushMessage.php?gid=<?=$gid?>>pushMessage<br><font color=red>푸쉬메세지</font></a></td>
<td><a href=admin_usingfilelist.php?gid=<?=$gid?>>usingfilelist<br><font color=red>사용파일목록</font></a></td>
<td><a href=admin_autoLeveling.php?gid=<?=$gid?>>autoLeveling<br><font color=red>레벨링</font></a></td>
<td><a href=admin_updateQueryMaker.php?gid=<?=$gid?>>updateQueryMaker<br><font color=red>업데이트쿼리생성</font></a></td>
</tr>
</table>

<table border=1>
<tr>
<td bgcolor=cccccc><b>MainServer</b></td>
<td><a href=admin_commonSetting.php?gid=<?=$gid?>>commonSetting<br><font color=red>커먼세팅</font></a></td>
<td><a href=admin_keyintvalue.php?gid=<?=$gid?>>key-intvalue<br><font color=red>키인트벨류</font></a></td>
<td><a href=admin_notice.php?gid=<?=$gid?>>notice<br><font color=red>공지</font></a></td>
<td><a href=admin_puzzle.php?gid=<?=$gid?>>puzzle<br><font color=red>퍼즐</font></a></td>
<td><a href=admin_piece.php?gid=<?=$gid?>>piece<br><font color=red>피스</font></a></td>
<td><a href=admin_card.php?gid=<?=$gid?>>card<br><font color=red>카드</font></a></td>
<td><a href=admin_hellMode.php?gid=<?=$gid?>>hellMode<br><font color=red>헬모드</font></a></td>
<td><a href=admin_character.php?gid=<?=$gid?>>character<br><font color=red>캐릭터</font></a></td>
<td><a href=admin_item.php?gid=<?=$gid?>>item<br><font color=red>아이템</font></a></td>
<td><a href=admin_shop.php?gid=<?=$gid?>>shop<br><font color=red>상점</font></a></td>
<td><a href=admin_exchange.php?gid=<?=$gid?>>exchange<br><font color=red>교환매니저</font></a></td>
<td><a href=admin_mission.php?gid=<?=$gid?>>mission<br><font color=red>미션</font></a></td>
<td><a href=admin_archivement.php?gid=<?=$gid?>>archivement<br><font color=red>업적</font></a></td>
<td><a href=admin_monster.php?gid=<?=$gid?>>monster<br><font color=red>몬스터</font></a></td>
<td><a href=admin_movement.php?gid=<?=$gid?>>movement<br><font color=red>움직임</font></a></td>
<td><a href=admin_pattern.php?gid=<?=$gid?>>pattern<br><font color=red>패턴</font></a></td>
<td><a href=admin_images.php?gid=<?=$gid?>>images<br><font color=red>이미지</font></a></td>
<td><a href=admin_resources.php?gid=<?=$gid?>>resources<br><font color=red>리소스</font></a></td>
</tr>
</table>

<table border=1>
<tr>
<td bgcolor=cccccc><b>ShardServer</b></td>
<td><a href=admin_user.php?gid=<?=$gid?>>user<br><font color=red>유저</font></a></td>
<td><a href=admin_weeklyScore.php?gid=<?=$gid?>>weeklyscore<br><font color=red>주간점수</font></a></td>
<td><a href=admin_stageScore.php?gid=<?=$gid?>>stagescore<br><font color=red>스테이지점수</font></a></td>
</tr>
</table>
</td></tr></table>
<br>
<?php } ?>