<?php

$sInfo["gameID"]="SKDDMK";
$sInfo["gameNo"]="10316";
$sInfo["HTTPGATEWAY_URL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";
$sInfo["HTTPGATEWAY_HELP_URL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/help.json";
$sInfo["HTTPGATEWAY_VERSION"]="1.3";
$sInfo["HTTPGATEWAY_PORT"]="18080";
$sInfo["secretKey"]="qrqhyrlgprghedvh";
DBManager::setStaticInfo($sInfo);

if($_SERVER["SERVER_NAME"]=="hsdooki.cafe24.com"){
	// 테스트용 서버
	//서버설정											ip                    id            pw
	$server0Index = DBManager::addDBServer("localhost","hsdooki","q1w2e3!@");

	//메인db설정					dbname       server index
	DBManager::setMainDB("hsdooki",$server0Index);

	//유저db설정                   dbname      server index
	DBManager::addShardDB("hsdooki",$server0Index);
}else{



/***************************************************************************************

서버설정

지금 설정상태 = db서버한대안에 drawgirls(maindb), dg001(userdb), dg002(userdb) 3개의 데이터베이스가 생성되어있는 상태

아래의 코드를 고쳐주세요.
****************************************************************************************/

	//서버추가											ip                    id            pw
	$server0Index = DBManager::addDBServer("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");

	//메인db설정, 위 에서 추가한 서버의 drawgirls라는 dbname을 사용하겠다.
	DBManager::setMainDB("drawgirls",$server0Index);

	//유저db설정, 위 에서 추가한 서버의 dg001, dg002 라는 dbname을 사용하겠다.
	DBManager::addShardDB("dg001",$server0Index);
	DBManager::addShardDB("dg002",$server0Index);
}

?>