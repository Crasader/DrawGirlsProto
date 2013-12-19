<?php

iconv_set_encoding("internal_encoding", "UTF-8");
iconv_set_encoding("output_encoding", "UTF-8");

include "lib.php";


$nowurl = $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"];
$_dirs = explode("/",$_SERVER["REQUEST_URI"]);
unset($_dirs[count($_dirs)-1]);
$_dirs = implode($_dirs,"/");
$nowurl = $nowurl.$_dirs;


$mode = $_GET["mode"];
$allResult="";
$version=1;
function webLog($log){
	global $mode;
	if($mode){
		echo $log;
	}
}

if($mode){

	$param["0"]=array();
	$param["0"]["p"]=json_decode(stripcslashes($_GET[p]),true);
	$param["0"]["a"]=$_GET[a];
	$version = $_GET["version"];

}else{
	$param = base64_decode(str_replace(" ","+",$_POST["command"]));
	
	$param = @mcrypt_decrypt(MCRYPT_DES, "12345678", $param, MCRYPT_MODE_ECB);
	
	
	$param = json_decode(trim($param),true);
	$version = $_POST["version"];
	
}


if($version)include "command/cmd".$version.".php";
else include "command/cmd1.php";

?>