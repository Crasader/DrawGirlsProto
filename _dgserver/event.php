<?php

    
	include "lib.php";

	CurrentUserInfo::$os = $_GET["os"];
    CurrentUserInfo::$language = $_GET["lang"];
    CurrentUserInfo::$country = $_GET["country"];
    CurrentUserInfo::$timezone = $_GET["timezone"];
    CurrentUserInfo::setStoreIDByStoreCode($_GET["store"]);
    if(!CurrentUserInfo::$store)CurrentUserInfo::$store="tstore";
	if(!CurrentUserInfo::$os)CurrentUserInfo::$os = "android";
    if(!CurrentUserInfo::$language) CurrentUserInfo::$language = "ko";
    if(!CurrentUserInfo::$country) CurrentUserInfo::$country = "kr";
    if(!CurrentUserInfo::$timezone) CurrentUserInfo::$timezone = "asia/seoul";

    
?>
<html>
<head>
<title>Event</title>
	<meta name="viewport" content="user-scalable=no,initial-scale=1.0">
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
</head>
<body topmargin=0 leftmargin=0 bgcolor="black">
<font color=white>
<?php
	if(!$_GET["no"]){

?>
<table border=0 width=100%>
<?php

	$nowDate = TimeManager::getCurrentDateTime();
	$storeBit = CurrentUserInfo::getStoreBit(CurrentUserInfo::$store);
	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);

	$noticeCnt=0;
	while($obj = Notice::getObjectByQuery("where startDate<$nowDate and endDate>$nowDate and os&".$osBit.">0 and cc&".$ccBit.">0 and store&".$storeBit.">0 and isList>0 order by `order` asc")){
		$noticeCnt++;
		$imgInfo = json_decode($obj->banner,true); 
		$url = "event.php?gid=".$gid."&no=".$obj->no;
		if($obj->linkURL)$url = $obj->linkURL;
		echo"<tr><td align=center><a href='".$url."'><img src='".$imgInfo["img"]."' border=0></a></td></tr>";
	}

	if(!$noticeCnt){
		echo "<br><br><center><font color=white>진행중인 이벤트가 없습니다.</font></center>";
	}
?>
</table>
<?php
	}else{
		echo"<font color=white>";
		$noticeInfo = new Notice($_GET["no"]);
		if($noticeInfo->isLoaded()){
			if($noticeInfo->isList){
				echo $noticeInfo->webContent;
			}
		}
		echo "</font>";

	}
?>
</font>
</body>
</html>