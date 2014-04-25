<?php

class userInfo
{
	function getUserInfo($param){

	}

	function writeUserInfo($param){

	}

	function updateUserInfo($param){

	}
}


$obj = new $dbClass();

$result =  $obj->$func($param);


echo'{"no":1,"nick":"litqoo","sex":"1,"birth":19870620,"phone":01045492643,"lastLogin":20110412,"regDate":20100110}';
exit;
echo json_encode($result,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);




?>