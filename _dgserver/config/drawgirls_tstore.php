<?php

$sInfo["gameID"]="SKDDMK";
$sInfo["gameNo"]="10331";
$sInfo["HTTPGATEWAY_URL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";
$sInfo["HTTPGATEWAY_HELP_URL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/help.json";
$sInfo["HTTPGATEWAY_VERSION"]="1.3";
$sInfo["HTTPGATEWAY_PORT"]="18080";
$sInfo["secretKey"]="qrqhyrlgprghedvh";
Infomation::setStaticInfo($sInfo);

LogManager::setLock(false);

/***************************************************************************************

서버설정

지금 설정상태 = db서버한대안에 drawgirls(maindb), dg001(userdb), dg002(userdb), dgLog001(logdb) 4개의 데이터베이스가 생성되어있는 상태

아래의 코드를 고쳐주세요.
****************************************************************************************/


	//새로운 설정법

	//서버생성
	$server1 = DBServer::create("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");

	





	//DB그룹 생성 - mainDB : 대부분 읽기전용 자료들. 운영자만 추가/수정/삭제 가능한 자료들. 퍼즐/피스/카드/몬스터/캐릭터목록/벨런스데이터 등 /////////////////////////////////////
	$mainGroup = DBGroup::create("main");
	
	//새로 작성할때 $memberID에 따라 접속할 서버번호 지정해주기 - mainDB는 한대이므로 1로 설정
	$mainGroup->setNewShardKeyFunc(function($memberID){		
		return 1;
	});

	//있는 자료 읽어올때 $memberID에 따라 접속할 서버번호 지정해주기 - mainDB는 한대이므로 1로설정
	$mainGroup->setGetShardKeyFunc(function($key){
		return 1;
	});







	//마스터DB(쓰기용) 지정          db명        서버
	$mainMaster01 = new DBMaster("drawgirls",$server1);




	//슬레이브DB(읽기용) 지정        db명         서버        * 현재는 따로 슬레이브서버가 없으므로 그냥 마스터db와 똑같은db를 지정. = 읽기/쓰기 모두 server1-drawgirls db 사용
	$mainSlave01 = new DBSlave("drawgirls",$server1);

	//마스터디비0에 슬레이브0 추가  (마스터0-슬레이브0 리플리케이션 설정되어있어야함. 혹은 마스터0=슬레이브0)
	$mainMaster01->addSlave($mainSlave01);





	//main그룹에 master0 추가
	$mainGroup->addMaster($mainMaster01);






	



	//DB그룹 생성 끝 - mainDB ///////////////////////////////////////////////////////////////////////////////////////
	



	//DB그룹 생성 - userDB : 각 유저들의 데이터. 게임플레이에 따라 자유로운 업데이트/추가/삭제가 이루어짐. 보유재화/보유카드/ ///////////////////////////////////////////////////////////////////////////////////////

	$userGroup = DBGroup::create("user");


	//새로 가입할때 $memberID에 따라 저장할 서버번호 지정하기 - 멤버ID를 2로나누고 +1한 값으로 서버번호지정.(1 or 2) 
	$userGroup->setNewShardKeyFunc(function($memberID){		
		//LogManager::addLog("setNewShardKeyFunc");
		return 1;
		//return $memberID%3+1;;
	}); // 샤드키발급

	//기존의 회원이 접속할 서버번호를 지정하기. 유저정보에 저장되어있는 서버번호 읽어와 돌려주기.
	$userGroup->setGetShardKeyFunc(function($memberID){
		$userIndex = UserIndex::create($memberID);
		//LogManager::addLog("user server setGetShardKeyFunc = ".$userIndex->userShardOrder);
		return $userIndex->userShardOrder;
	}); // 샤드키찾기






	//마스터DB1(쓰기용) 생성
	$userMaster01 = new DBMaster("dg001",$server1);

	//user 그룹에 마스터DB1추가
	$userGroup->addMaster($userMaster01);




	//슬레이브DB1(읽기용) 생성  -  따로 리플리케이션된 슬레이브가 없으니 마스터db와같은 디비로 생성.
	$userSlave01 = new DBSlave("dg001",$server1);

	//마스터DB1에 슬레이브DB1추가
	$userMaster01->addSlave($userSlave01);




	





	//마스터DB2(쓰기용) 생성
	$userMaster02 = new DBMaster("dg002",$server1);
	
	$userGroup->addMaster($userMaster02);





	//슬레이브DB2(읽기용) 생성  - 따로 리플리케이션된 슬레이브가 없으니 마스터db와 같은 디비로 생성.
	$userSlave02 = new DBSlave("dg002",$server1);
	
	//각각 그룹에 마스터, 마스터에 슬레이브 추가
	$userMaster02->addSlave($userSlave02);









	//마스터DB2(쓰기용) 생성
	$userMaster03 = new DBMaster("dg003",$server1);
	
	$userGroup->addMaster($userMaster03);




	//슬레이브DB2(읽기용) 생성  - 따로 리플리케이션된 슬레이브가 없으니 마스터db와 같은 디비로 생성.
	$userSlave03 = new DBSlave("dg003",$server1);
	
	//각각 그룹에 마스터, 마스터에 슬레이브 추가
	$userMaster03->addSlave($userSlave03);
	

	
	//DB그룹 생성 끝 - userDB  ///////////////////////////////////////////////////////////////////////////////////////
	



	//DB그룹 생성 - logDB : 각 유저들의 로그데이터 ////////////////////////////////////////////////////////////////////////////////////////

	$logGroup = DBGroup::create("log");

	//새로 가입할때 $memberID에 따라 저장할 서버번호 지정하기 - 서버가 한대밖에 없으니 무조건 1번 서버접속
	$logGroup->setNewShardKeyFunc(function($memberID){		
		return 1;
	});

	//기존의 회원이 접속할 서버번호를 지정하기. - 서버가 한대밖에 없으니 무조건 1번 서버접속
	$logGroup->setGetShardKeyFunc(function($memberID){
		return 1;
	});





	//마스터1(쓰기용) 생성
	$logMaster01 = new DBMaster("dgLog001",$server1);

	//log 그룹에 마스터DB1추가
	$logGroup->addMaster($logMaster01);





	
	//슬레이브1(읽기용) 생성  -  따로 리플리케이션된 슬레이브가 없으니 마스터db와같은 디비로 생성.
	$logSlave01 = new DBSlave("dgLog001",$server1);

	//마스터1에 슬레이브1추가
	$logMaster01->addSlave($logSlave01);




?>