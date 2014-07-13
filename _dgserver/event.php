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

	$nowDate = TimeManager::get()->getCurrentDateTime();

	while($obj = Notice::getObjectByQuery("where startDate<$nowDate and endDate>$nowDate and os IN ('all','".CurrentUserInfo::$os."') and `cc` IN ('all','".CurrentUserInfo::$country."') and isList=1 order by `no` desc")){

		$imgInfo = json_decode($obj->banner,true); 
		echo"<tr><td align=center><a href=event.php?no=".$obj->no."><img src='".$imgInfo["img"]."' border=0></a></td></tr>";
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