<?php

include "lib.php";
iconv_set_encoding("internal_encoding", "UTF-8");
iconv_set_encoding("output_encoding", "UTF-8");




$nowurl = $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"];
$_dirs = explode("/",$_SERVER["REQUEST_URI"]);
unset($_dirs[count($_dirs)-1]);
$_dirs = implode($_dirs,"/");
$nowurl = $nowurl.$_dirs;

CurrentUserInfo::$language = "ko";

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

include "command/cmd2.php";

// if($version)include "command/cmd".$version.".php";
// else include "command/cmd1.php";


if(!$stopCommand){
	//$command = new commandClass();
	
	for($c=0;$c<count($param);$c++){
		$cmd = (string)$c;
		if(!$param[$cmd])continue;
		$p = $param[$cmd]["p"];
		$a = strtolower($param[$cmd]["a"]);
		
		
		if(method_exists("commandClass",$a)){
			$startTime = TimeManager::getMicroTime();
			$r = commandClass::$a($p);
			$endTime = TimeManager::getMicroTime();

			$r[log] = LogManager::getLogAndClear();
		 	$allResult[$cmd]= $r;
		 	
		 	$p2 = array();
		 	$p2["memberID"]= $param["memberID"];
			if(!$p2["memberID"])$p2["memberID"]= $param["hspMemberNo"];
			if(!$p2["memberID"])$p2["memberID"]= $param["memberNo"];
	
			$p2["category"]=$a;
			$p2["content"]=json_encode($p,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$p2["output"]=$allResult[$cmd];
			$p2["execTime"]=$endTime-$startTime;
			if($a!="writelog")commandClass::writelog($p2);
			
		}else if($a=="help"){
			$class_methods = get_class_methods('commandClass');
			foreach ($class_methods as $method_name) {
			    echo "$method_name <br>";
			}
			mysql_close();
			exit;
		}else{
			$p["api"]=$param[$cmd]["a"];
			$allResult[$cmd]= commandClass::httpgateway($p);
			
			
			$p2=array();
		 	$p2["memberID"]= $param["memberID"];  
			if(!$p2["memberID"])$p2["memberID"]= $param["hspMemberNo"];
			if(!$p2["memberID"])$p2["memberID"]= $param["memberNo"];
					
			$p2["category"]=$p["api"];
			$p2["content"]=json_encode($p,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK | JSON_BIGINT_AS_STRING);
			$p2["output"]=$allResult[$cmd];
			commandClass::writelog($p2);
			//$allResult[$cmd]=array("state"=>"error","msg"=>"don't find command");
		}
		
		
	}
	$allResult[state]="ok";
	$allResult[timestamp]=TimeManager::getTime();
	
	$allResult = json_encode($allResult,JSON_UNESCAPED_UNICODE);
	
	 if($mode=="nodes"){
	 	echo $allResult;
	 }else{
		echo base64_encode(@mcrypt_encrypt(MCRYPT_DES, $SECRETKEY,$allResult, MCRYPT_MODE_ECB))."#";		
	}
	
	
	DBServer::closeAllConnection();
	@mysql_close();

}

?>