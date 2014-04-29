<?php
include_once("DBManager.php");


class userInfo
{
	function getUserInfo($param){

		$command = new commandClass();
		$user = new UserData($param["id"]);
		$p["api"]="GetProfileDetail2";
		$p["memberNo"]=$param["id"];
		
		$hgr = $command->httpgateway($p);
		
		$test=$r["profile"];
		$test["paramreturn"]=$param;
		
		$r=$user->getArrayData(true);
		$r["no"] = "_".$user->memberID;
		$r["sex"] = $hgr["gender"];
		$r["birth"] = $hgr["age"];
		$r["phone"] = $hgr["phoneNo"];

		$result["data"]=$r;
		$result["result"]="ok";
		return $result;
	}

	function writeUserInfo($param){
		$test =  '{"value":"writeUserInfo","result":"ok"}';

		return json_decode($test,true);
	}

	function updateUserInfo($param){
		$test =  '{"value":"updateUserInfo","result":"ok"}';
		$test = json_decode($test,true);
		$test["data"]=$param["data"];
		return $test;
	}

	function deleteUserInfo($param){
		$test =  '{"value":"deleteUserInfo","result":"ok"}';
		$test = json_decode($test,true);
		$test["data"]=$param;
		return $test;
	}
}




?>