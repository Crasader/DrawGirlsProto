<?php
	include "lib.php";

	CurrentUserInfo::$os = $_GET["os"];
    CurrentUserInfo::$language = $_GET["lang"];
    CurrentUserInfo::$country = $_GET["country"];
    CurrentUserInfo::$timezone = $_GET["timezone"];


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

	$nowDate = TimeManager::
	getCurrentDateTime();

	while($obj = Notice::getObjectByQuery("where startDate<$nowDate and endDate>$nowDate and os IN ('all','".CurrentUserInfo::$os."') and `cc` IN ('all','".CurrentUserInfo::$country."') and isList=1 order by `order` asc")){

		$imgInfo = json_decode($obj->banner,true); 
		$url = "event.php?no=".$obj->no;
		if($obj->linkURL)$url = $obj->linkURL;
		echo"<tr><td align=center><a href='".$url."'><img src='".$imgInfo["img"]."' border=0></a></td></tr>";
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