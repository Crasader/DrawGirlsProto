<?php

	include "../lib.php";
	$admin = new AdminUser($_SESSION["admin_no"]);
	if(!$admin->isLogined()){
		echo "";
		exit;
	}

	echo"<br><br><br><br><center>";
	echo"<a href=manage_userInfo.php?gid=drawgirls>for kakao at google</a><br><br>";
	echo"<a href=manage_userInfo.php?gid=drawgirls_tstore>for stand alone at tstore</a><br><br>";
	echo"</center>";

?>