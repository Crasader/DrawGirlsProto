<?php

class commandClass
{
	public $m_hspMemberNo;
	public $m_kakaoMemberID;
	
	public function __construct(){
		//$this->m_hspMemberNo = $param["hspMemberNo"] || $param["memberNo"];;
		//$this->m_kakaoMemberID = $param["kakaoMemberID"];
	}
	
function getinfo($data){
	$data["appname"]="drawgirls";
	return $data;
}

function httpgateway($data) {
	global $HSP_VERSION;
	global $HSP_GAMENO;
	global $HSP_PORT;
	global $HSP_GATEWAY_URL;

	if(!$data[version])$data[version]=$HSP_VERSION;
	if(!$data[headerGameNo])$data[headerGameNo]=$HSP_GAMENO;
	
	//$data[gameNo]=(int)$HSP_GAMENO;
	
	//$data_string = json_encode($data); 
	
	foreach($data as $key=>$value) { 
		if(is_scalar($value))
			$fields_string .= $key.'='.urlencode($value).'&';
		else if(is_array($value)){
			$nvalue ="";
			foreach($value as $key2=>$value2){
				$nvalue.=$value2.",";
			}
			$nvalue=rtrim($nvalue, ',');

			$fields_string .= $key.'='.urlencode($nvalue).'&';
		} 
		
	}
	$fields_string=rtrim($fields_string, '&');
	
	
	$agent = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)';
	$curlsession = curl_init();
	curl_setopt($curlsession, CURLOPT_URL, $HSP_GATEWAY_URL."?".$fields_string); //hsp/httpgw/help.json?api=$api&version=1.3
	curl_setopt($curlsession, CURLOPT_PORT, $HSP_PORT);
	curl_setopt($curlsession, CURLOPT_FOLLOWLOCATION, true);  //리다이렉트를 자동으로 잡아줘서 302가 아니라 200이 리턴됨
	curl_setopt($curlsession, CURLOPT_MAXREDIRS, 10); //최대 리다이렉트 횟수
	curl_setopt($curlsession, CURLOPT_USERAGENT, $agent);
	curl_setopt($curlsession, CURLOPT_REFERER, "");
	curl_setopt($curlsession, CURLOPT_TIMEOUT, 10);  //타임아웃 시간
	curl_setopt($curlsession, CURLOPT_CUSTOMREQUEST, "GET");                                                                     
	curl_setopt($curlsession, CURLOPT_POSTFIELDS, $fields_string);                                                                  
	curl_setopt($curlsession, CURLOPT_RETURNTRANSFER, true);                                                                                                        

	$buffer = curl_exec($curlsession);
	$cinfo = curl_getinfo($curlsession);
	curl_close($curlsession);
	if ($cinfo['http_code'] != 200)
	{
		$r["state"]="error";
		$r["msg"]="gateway server error";
		$r["send"]=$HSP_GATEWAY_URL."?".$fields_string;
		return $r;
	}else{
		$r=json_decode($buffer,true);
		$r["state"]="ok";
		$r["send"]=$HSP_GATEWAY_URL."?".$fields_string;
		return $r;
	}
}
/////////////////////////////////////////
function help_getcommonsetting(){

	$r["discript"] = "게임 기본 설정을 가져옵니다.";
	
	//$r["param"]
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}

function getcommonsetting($p){
	global $TABLE_COMMONSETTING;
	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$result = mysql_query("select * from $TABLE_COMMONSETTING",$mainConn);  
	$r = array();
	
	if(!$result)return ResultState::makeReturn(1001,"commonsetting error");
	
	while($setting = mysql_fetch_array($result,MYSQL_ASSOC)){
		$key = $setting["key"];
		$value = $setting["value"];
		
		$r[$key]=$value;
	}
	
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	
	return $r;
}

function help_getnoticelist(){

	$r["discript"] = "공지사항목록을 가져옵니다.";
	//$r["param"]
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}

function getnoticelist($p){
	global $TABLE_NOTICE;
	global $nowurl;
	
	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	$nowDate = TimeManager::get()->getCurrentDateString();
	$result = mysql_query("select * from $TABLE_NOTICE where isOn='true' and startDate<$nowDate and endDate>$nowDate",$mainConn);
	
	LogManager::get()->addLog("select * from $TABLE_NOTICE where isOn='true' and startDate<$nowDate and endDate>$nowDate");
	
	$r = array();
	
	if(!$result)return ResultState::makeReturn(1001,"commonsetting error");
	
	while($noticeInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
		
		$noticeInfo[imgInfo]=json_decode($noticeInfo[imgInfo],true);
		$noticeInfo[imgInfo][img]=$nowurl."/images/".$noticeInfo[imgInfo][img];		
		$r["list"][]=$noticeInfo;
	}
	
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	
	return $r;
}

/////////////////////////////////////////

function help_getcharacterlist(){

	$r["discript"] = "케릭터 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","discript"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getcharacterlist($p){
	global $nowurl;
	global $TABLE_CHARACTER;

	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$listVer = kvManager::get("charListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		
		return $r;
	}
	
	$result =  mysql_query("select * from $TABLE_CHARACTER",$mainConn);
	
	if(!$result)return ResultState::makeReturn(1001,"getcharacterlist error");
	
	$list = array();
	while($charInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
			$charInfo[purchaseInfo]=json_decode($charInfo[purchaseInfo],true);
			$charInfo[statInfo]=json_decode($charInfo[statInfo],true);
			$charInfo[resourceInfo]=json_decode($charInfo[resourceInfo],true);
			$charInfo[resourceInfo][ccbi]=$nowurl."/images/".$charInfo[resourceInfo][ccbi];
			$charInfo[resourceInfo][plist]=$nowurl."/images/".$charInfo[resourceInfo][plist];
			$charInfo[resourceInfo][pvrccz]=$nowurl."/images/".$charInfo[resourceInfo][pvrccz];
	
			$list[]=$charInfo;
	}
	
	$r["list"]=$list;
	$r["version"]=$listVer;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}
/////////////////////////////////////////

function help_getmonsterlist(){

	$r["discript"] = "몬스터 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","discript"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getmonsterlist($p){
	global $nowurl;
	global $TABLE_MONSTER;

	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$listVer = kvManager::get("monsterListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		
		return $r;
	}
	
	$result =  mysql_query("select * from $TABLE_MONSTER",$mainConn);
	
	if(!$result)return ResultState::makeReturn(1001,"getmonsterlist error");
	
	$list = array();
	while($mobInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
			$mobInfo[resourceInfo]=json_decode($mobInfo[resourceInfo],true);
			$mobInfo[resourceInfo][ccbi]=$nowurl."/images/".$mobInfo[resourceInfo][ccbi];
			$mobInfo[resourceInfo][plist]=$nowurl."/images/".$mobInfo[resourceInfo][plist];
			$mobInfo[resourceInfo][pvrccz]=$nowurl."/images/".$mobInfo[resourceInfo][pvrccz];
	
			$list[]=$mobInfo;
	}
	
	$r["list"]=$list;
	$r["version"]=$listVer;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

/////////////////////////////////////////

function help_getstageinfo(){

	$r["discript"] = "스테이지 정보를 받아옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","discript"=>"버전");
	$r["param"][] = array("name"=>"no","type"=>"int","discript"=>"스테이지 번호");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1003,"디비fetch실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getstageinfo($p,$pTable=""){
	global $nowurl;
	global $TABLE_STAGE;
	global $TABLE_CARD;
	if(!$pTable)$pTable=$TABLE_STAGE;
	
	$stageVer=0;
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	
	if($p[no]<10000){
		$stageVer = kvManager::get("stageVer_".$p[no],1);		
	}else{
		$stageVer = kvManager::get("eStageVer_".$p[no],1);
	}

	if($stageVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		$r["result"]=ResultState::toArray(2001);
		return $r;
	}
	
	$siresult = mysql_query("select * from $pTable where no=$p[no]",$mainConn);
	if(!$siresult) return ResultState::makeReturn(1001,"stageinfo");
	
	$stageInfo = mysql_fetch_array($siresult,MYSQL_ASSOC);
	if(!$stageInfo) return ResultState::makeReturn(1003,"stageinfo");
	
	$stageInfo[version]=$stageVer;
	$stageInfo[shopItems]=json_decode($stageInfo[shopItems],true);
	$stageInfo[defItems]=json_decode($stageInfo[defItems],true);
	$stageInfo[cards]=json_decode($stageInfo[cards],true);
	$stageInfo[mission]=json_decode($stageInfo[mission],true);
	
	$card = array();
	for($i=0;$i<count($stageInfo[cards]);$i++){
		$cardInfo = mysql_fetch_array(mysql_query("select * from $TABLE_CARD where no=".$stageInfo[cards][$i],$mainConn),MYSQL_ASSOC);
		if($cardInfo){
			$cardInfo[ability]=json_decode($cardInfo[ability],true);
			$cardInfo[missile]=json_decode($cardInfo[missile],true);
			$cardInfo[aniInfo]=json_decode($cardInfo[aniInfo],true);
			$cardInfo[imgInfo]=json_decode($cardInfo[imgInfo],true);
			$cardInfo[thumbnailInfo]=json_decode($cardInfo[thumbnailInfo],true);
			$cardInfo[silImgInfo]=json_decode($cardInfo[silImgInfo],true);
			
			
			$cardInfo[thumbnailInfo][img]=$nowurl."/images/".$cardInfo[thumbnailInfo][img];
			$cardInfo[imgInfo][img]=$nowurl."/images/".$cardInfo[imgInfo][img];
			$cardInfo[aniInfo][detail][img]=$nowurl."/images/".$cardInfo[aniInfo][detail][img];
			$cardInfo[silImgInfo][img]=$nowurl."/images/".$cardInfo[silImgInfo][img];
			$card[]=$cardInfo;
		}
	}
	
	
	$stageInfo[cards]=$card;
	$stageInfo[state]="ok";
	$stageInfo["result"]=ResultState::successToArray();
	return $stageInfo;
}

function geteventstageinfo($p)
{
	global $TABLE_EVENTSTAGE;
	return $this->getstageinfo($p,$TABLE_EVENTSTAGE);
}

///////////////////////////////////////////

function help_getcardlist(){

	$r["discript"] = "카드정보 목록을 받아옵니다.";
	
	$r["param"][] = array("name"=>"noList","type"=>"array(int)","discript"=>"카드번호목록");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"fail to get mainConnection");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getcardlist($p){
	global $TABLE_CARD,$nowurl;
	$cardlist="";
	
	if(is_array($p["noList"]))
		$cardlist = join(',',$p["noList"]);
	else 
		$cardlist = $p["noList"];
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$result = mysql_query("select * from $TABLE_CARD where no IN ($cardlist)",$mainConn);  
	if(!$result)return ResultState::makeReturn(1001);
	
	$list = array();
	while($cardInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
		if($cardInfo){
			$cardInfo[ability]=json_decode($cardInfo[ability],true);
			$cardInfo[missile]=json_decode($cardInfo[missile],true);
			$cardInfo[aniInfo]=json_decode($cardInfo[aniInfo],true);
			$cardInfo[imgInfo]=json_decode($cardInfo[imgInfo],true);
			$cardInfo[silImgInfo]=json_decode($cardInfo[silImgInfo],true);		
			$cardInfo[imgInfo][img]=$nowurl."/images/".$cardInfo[imgInfo][img];
			$cardInfo[aniInfo][detail][img]=$nowurl."/images/".$cardInfo[aniInfo][detail][img];
			$cardInfo[silImgInfo][img]=$nowurl."/images/".$cardInfo[silImgInfo][img];
			$list[]=$cardInfo;
		}
	}
	
	$r["list"]= $list; //json_decode(json_encode($list,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK),true);
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;

}
//////////////////////////////////////////
function help_getpuzzleinfo(){

	$r["discript"] = "퍼즐 정보를 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","discript"=>"퍼즐정보버전");
	$r["param"][] = array("name"=>"no","type"=>"int","discript"=>"퍼즐 번호");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1003,"디비fetch실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getpuzzleinfo($p){
	global $nowurl;
	global $TABLE_STAGE;
	global $TABLE_PUZZLE;
	global $TABLE_CARD;
	global $TABLE_EVENTSTAGE;
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	
	$puzzleVer = kvManager::get("puzzleVer_".$p[no],1);

	if($puzzleVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		$r["result"]=ResultState::toArray(2001);
		return $r;
	}
	
	$piresult = mysql_query("select * from $TABLE_PUZZLE where no=$p[no]",$mainConn);
	if(!$piresult)return ResultState::makeReturn(1001,"puzzle info");
	
	$puzzleInfo =  mysql_fetch_array($piresult,MYSQL_ASSOC);
	if(!$puzzleInfo)return ResultState::makeReturn(1003,"puzzle info");
	

	$puzzleInfo[version]=$puzzleVer;
	$puzzleInfo[center] = json_decode($puzzleInfo[center],true);
	$puzzleInfo[center][image]=$nowurl."/images/".$puzzleInfo[center][image];
	$puzzleInfo[face] = json_decode($puzzleInfo[face],true);
	$puzzleInfo[face][image]=$nowurl."/images/".$puzzleInfo[face][image];
	$puzzleInfo[original] = json_decode($puzzleInfo[original],true);
	$puzzleInfo[original][image]=$nowurl."/images/".$puzzleInfo[original][image];
	$puzzleInfo[thumbnail] = json_decode($puzzleInfo[thumbnail],true);
	$puzzleInfo[thumbnail][image]=$nowurl."/images/".$puzzleInfo[thumbnail][image];
	
	
	
	$query = mysql_query("select * from $TABLE_STAGE where puzzle=$p[no] order by no asc",$mainConn);
	if(!$query)return ResultState::makeReturn(1001,"stage list");
		
	$stagelist=array();
	$cnt=0;
	$startStageNo=0;
	while($stageInfo = mysql_fetch_array($query,MYSQL_ASSOC)){
		if($startStageNo==0)$startStageNo=$stageInfo[no];

		$stageInfo["condition"] = json_decode($stageInfo["condition"],true);
		$stageInfo[shopItems]=json_decode($stageInfo[shopItems],true);
		$stageInfo[defItems]=json_decode($stageInfo[defItems],true);
		$stageInfo[cards]=json_decode($stageInfo[cards],true);
		$stageInfo[mission]=json_decode($stageInfo[mission],true);
		$card = array();
		for($i=0;$i<count($stageInfo[cards]);$i++){
			$cardInfo = mysql_fetch_array(mysql_query("select * from $TABLE_CARD where no=".$stageInfo[cards][$i],$mainConn),MYSQL_ASSOC);
			if($cardInfo){
				$cardInfo[ability]=json_decode($cardInfo[ability],true);
				$cardInfo[missile]=json_decode($cardInfo[missile],true);
				$cardInfo[aniInfo]=json_decode($cardInfo[aniInfo],true);
				$cardInfo[imgInfo]=json_decode($cardInfo[imgInfo],true);
				$cardInfo[thumbnailInfo]=json_decode($cardInfo[thumbnailInfo],true);
				$cardInfo[silImgInfo]=json_decode($cardInfo[silImgInfo],true);
				
				
				$cardInfo[thumbnailInfo][img]=$nowurl."/images/".$cardInfo[thumbnailInfo][img];
				$cardInfo[imgInfo][img]=$nowurl."/images/".$cardInfo[imgInfo][img];
				$cardInfo[aniInfo][detail][img]=$nowurl."/images/".$cardInfo[aniInfo][detail][img];
				$cardInfo[silImgInfo][img]=$nowurl."/images/".$cardInfo[silImgInfo][img];
				$card[]=$cardInfo;
			}
			
		}
		
		
		$stageInfo[cards]=$card;
		$stagelist[]=$stageInfo;
		$cnt++;
	}
	
	$puzzleInfo["list"]=$stagelist;
	$puzzleInfo["startStage"]=$startStageNo;
	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
}

/////////////////////////////////////////////
function help_getpuzzlelist(){

	$r["discript"] = "퍼즐 정보 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"puzzlelistversion","type"=>"int","discript"=>"퍼즐정보목록버전");
	$r["param"][] = array("name"=>"eventstagelistversion","type"=>"int","discript"=>"이벤트스테이지목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","discript"=>"얻어올 시작 퍼즐번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","discript"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getpuzzlelist($p){
	global $nowurl;
	global $TABLE_STAGE;
	global $TABLE_PUZZLE;
	global $TABLE_EVENTSTAGE;
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$puzzleVer = kvManager::get("puzzleListVer",1);

	if($puzzleVer==$p[puzzlelistversion]){
		$r[puzzlelistversion]=$p[puzzlelistversion];
	}else{
		// 여기서 eventstagelistversion 도 비교
		if(!$p[start])$p[start]=0;
		if(!$p[limit])$p[limit]=6;
		
		$query = mysql_query("select no,thumbnail,title,point,ticket from $TABLE_PUZZLE order by no asc limit $p[start],$p[limit]",$mainConn);
		
		$list=array();
		$cnt=0;
		while($pData = mysql_fetch_array($query,MYSQL_ASSOC)){
			//퍼즐 시작stage 알아오기
			$r1 = mysql_query("select no from $TABLE_STAGE where puzzle=".$pData[no]." order by no asc limit 1",$mainConn);
			if($r1){
				$r1d = mysql_fetch_array($r1,MYSQL_ASSOC);
				$pData["startStage"]=$r1d[no];
			}
			
			//퍼즐 안에 스테이지 갯수 알아오기
			$r2 = mysql_query("select count(no) from $TABLE_STAGE where puzzle=".$pData[no],$mainConn);
			if($r2){
				$r2d = mysql_fetch_array($r2);
				$pData["stageCount"]=$r2d[0];
			}
			
			$pData[version]=kvManager::get("puzzleVer_".$pData[no],1);
			$pData[thumbnail]=json_decode($pData[thumbnail],true);
			$pData[thumbnail][image]=$nowurl."/images/".$pData[thumbnail][image];
			
			$list[]=$pData;
			$cnt++;
		}
		
		$puzzleInfo["puzzlelist"]=$list;
	}
	
	$eListVer = kvManager::get("eStageListVer",1);
	if($eListVer == $p[eventstagelistversion]){
		$r[eventstagelistversion]=$p[eventstagelistversion];
	}else{
		$eList=array();
		$ecnt=0;
		$query = mysql_query("select no,thumbnail from $TABLE_EVENTSTAGE",$mainConn);
		while($eventList = mysql_fetch_array($query,MYSQL_ASSOC)){
			$eventList[thumbnail]=json_decode($eventList[thumbnail],true);
			$eventList[thumbnail][image]=$nowurl."/images/".$eventList[thumbnail][image];
			$eventList[no]=(int)$eventList[no];	
			$eList[]=$eventList;
			$ecnt++;
		}
		
		$puzzleInfo["eventstagelist"]=$eList;
	
	}

	$puzzleInfo["puzzlelistversion"]=$puzzleVer;
	$puzzleInfo["eventstagelistversion"]=$eListVer;

	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
	
}
/////////////////////////////////////////////
function help_geteventstagelist(){

	$r["discript"] = "이벤트스테이지 목록을 받아옵니다.";
	
	$r["param"][] = array("name"=>"puzzlelistversion","type"=>"int","discript"=>"퍼즐정보목록버전");
	$r["param"][] = array("name"=>"eventstagelistversion","type"=>"int","discript"=>"이벤트스테이지목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","discript"=>"얻어올 시작 퍼즐번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","discript"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같으면 정보를 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function geteventstagelist($p){
	global $TABLE_EVENTSTAGE;
	global $TABLE_CHECKPOINT;
	global $TABLE_CARD;
	global $nowurl;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$eventVer = kvManager::get("eStageListVer",1);

	if($eventVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		$r["result"]=ResultState::toArray(2001);
		return $r;
	}
	
	$query = mysql_query("select no,thumbnail from $TABLE_EVENTSTAGE",$mainConn);
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$list=array();
	$cnt=0;
	while($stageList = mysql_fetch_array($query,MYSQL_ASSOC)){
		$stageList[thumbnail]=json_decode($stageList[thumbnail],true);
		$stageList[thumbnail][image]=$nowurl."/images/".$stageList[thumbnail][image];
		$list[]=$stageList;
		$cnt++;
	}
	
	$result["count"]=$cnt;
	$result["list"]=$list;
	$result["version"]=$eventVer;
	$result["state"]="ok";
	$result["result"]=ResultState::successToArray();
	return $result;
}
/*

function help_getstagelist(){

	$r["discript"] = "스테이지 목록을 받아옵니다.";
	
	$r["param"][] = array("name"=>"start","type"=>"int","discript"=>"얻어올 시작 퍼즐번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","discript"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"fail to get mainConnection");
	$r["result"][]=ResultState::toArray(2001,"버전이 같으면 정보를 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getstagelist($p){
	
	global $TABLE_STAGE;
	global $TABLE_CHECKPOINT;
	global $TABLE_EVENTSTAGE;
	global $TABLE_CARD;
	global $nowurl;
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$query = mysql_query("select no from $TABLE_STAGE",$mainConn);
		
	$list=array();
	$cnt=0;
	while($stageList = mysql_fetch_array($query,MYSQL_ASSOC)){
		$stageList[thumbnail]=json_decode($stageList[thumbnail],true);
		$stageList[thumbnail][image]=$nowurl."/images/".$stageList[thumbnail][image];
		$list[]=$stageList;
		$cnt++;
	}
	
	$checkpoint=array();

	$query = mysql_query("select * from $TABLE_CHECKPOINT",$mainConn);
	while($checkpointList = mysql_fetch_array($query,MYSQL_ASSOC)){
		$checkpointList[thumbnail]=json_decode($checkpointList[thumbnail],true);
		$checkpointList[thumbnail][image]=$nowurl."/images/".$checkpointList[thumbnail][image];
		unset($checkpointList[no]);
		$checkpoint[]=$checkpointList;
		
	}
	
	$eList=array();
	$ecnt=0;
	$query = mysql_query("select no,thumbnail from $TABLE_EVENTSTAGE",$mainConn);
	while($eventList = mysql_fetch_array($query,MYSQL_ASSOC)){
		$eventList[thumbnail]=json_decode($eventList[thumbnail],true);
		$eventList[thumbnail][image]=$nowurl."/images/".$eventList[thumbnail][image];
		$eList[]=$eventList;
		$ecnt++;
	}
	
	$result[theme]=$theme;
	$result[count]=$cnt;
	$result["list"]=$list;
	$result["eventCount"]=$ecnt;
	$result["eventList"]=$eList;
	$result["cpList"]=$checkpoint;
	$result["version"]=10;
	$result["state"]="ok";
	return $result;

}
*/


//////////////////////////////////////////

function help_writelog(){

	$r["discript"] = "로그를 작성합니다.";
	
	$r["param"][] = array("name"=>"memberID","type"=>"int","discript"=>"멤버아이디");
	$r["param"][] = array("name"=>"content","type"=>"string","discript"=>"내용");
	$r["param"][] = array("name"=>"output","type"=>"string","discript"=>"결과");
	$r["param"][] = array("name"=>"category","type"=>"string","discript"=>"카테고리");
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"로그작성실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


function writelog($p){
	global $TABLE_LOG;
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$data["memberID"]=$p["memberID"];
	$data["content"]=$p["content"];
	$data["output"]=$p["output"];
	$data["time"]=time();
	$data["category"]=$p["category"];
	$data["ip"]=get_client_ip();
	$data["header"]=json_encode(getallheaders());
	$query = lq_query_insert($data,$TABLE_LOG);
	$r = mysql_query($query,$mainConn);
	
	if(!$r)return ResultState::makeReturn(1001,"fail to write");
	
	$result["state"]="ok";
	$result["result"]=ResultState::successToArray();
	
	
	return $result;
}
/////////////////////////////////////////

function help_increasestats(){

	$r["discript"] = "숫자통계 증가";
	
	$r["param"][] = array("name"=>"key","type"=>"string","discript"=>"항목 키 값");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"증가실패");
	
	return $r;
}

function increasestats($p){
	$r=array();
	if(kvManager::increase($p["key"])){
		$r["result"]=ResultState::successToArray();	
	}else{
		$r["result"]=ResultState::toArray(1001);
	}
	
	$r["state"]="ok";
	return $r;
}

///////////////////////////////////////////

function help_setuserdata(){

	$r["discript"] = "유저데이터 저장";
	
	$r["param"][] = array("name"=>"data","type"=>"string","discript"=>"저장할데이터 json string");
	$r["param"][] = array("name"=>"nick","type"=>"string","discript"=>"닉네임");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	return $r;
}

function setuserdata($p){
	$memberid = $p["memberID"];
	if($memberid){
		$user = new UserData($memberid);
		
		if($p["nick"])$user->m_nick = $p["nick"];
		if($p["data"])$user->m_data = $p["data"];
		
		
		if(!$user->save())return ResultState::makeReturn(2006);
		
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
	}
	
	return $r;
}



function help_getuserdata(){

	$r["discript"] = "유저데이터 로드";
	
	$r["param"][] = array("name"=>"memberID","type"=>"int or string","discript"=>"멤머ID");
	$r["param"][] = array("name"=>"keyList","type"=>"array(string)","discript"=>"받아올키목록, 없으면 모두 받아옴");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2003,"정보로드실패");
	
	return $r;
}

function getuserdata($p){
	$memberid = $p["memberID"];
	$keylist = $p["keyList"];
	if($memberid){
		$user = new UserData($memberid);
		if($user->isLoaded()){
			$r = $user->getArrayData(true,$keylist);
			$r["state"]="ok";
			$r["result"]=ResultState::successToArray();
		}else{
			$r["state"]="error";
			$r["result"]=ResultState::toArray(2003,"fail to load userdata");
		}
	}else{
		$r["state"]="error";
		$r["errorCode"]=10010;
		$r["result"]=ResultState::toArray(2002,"memberID");
	}
	
	return $r;

}

function help_getuserdatalist(){

	$r["discript"] = "유저데이터목록 로드";
	
	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string)","discript"=>"멤머ID목록");
	$r["param"][] = array("name"=>"keyList","type"=>"array(string)","discript"=>"받아올키목록, 없으면 모두 받아옴");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberIDList를 안넣음");
	
	return $r;
}

function getuserdatalist($p){
	$memberlist = $p["memberIDList"];
	$keylist = $p["keyList"];
	if(!is_array($memberlist)){
		$r["state"]="error";
		$r["errorCode"]=10010;
		$r["result"]=ResultState::toArray(2002,"memberIDList");
		return $r;
	}
	$list = array();
	for($i=0;$i<count($memberlist);$i++){
		$memberid = $memberlist[$i];
		if($memberid){
			$user = new UserData($memberid);
			if($user->isLoaded()){
				$_r = $user->getArrayData(true,$keylist);
				$list[]=$_r;	
			}
		}
	}
	
	$r["list"]=$list;
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	return $r;
}

function help_updateuserdata(){

	$r["discript"] = "유저데이터 업데이트";

	$r["param"][] = array("name"=>"memberID","type"=>"string","discript"=>"멤버ID");
	$r["param"][] = array("name"=>"data","type"=>"string","discript"=>"저장할데이터 json string");
	$r["param"][] = array("name"=>"nick","type"=>"string","discript"=>"닉네임");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberIDList를 안넣음");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	
	return $r;
}


function updateuserdata($p){
	$memberid = $p["memberID"];
	$r=array();
	if($memberid){
		$user = new UserData($memberid);
		
		if($p["nick"])$user->m_nick = $p["nick"];
		
		if($p["data"]){
			LogManager::get()->addLog("updateuserdata updateData");
			if(!$user->updateData($p["data"]))return ResultState::makeReturn(2006);
			$r["data"]=$user->getData();
		}else{
			LogManager::get()->addLog("updateuserdata save");
			if(!$user->save())return ResultState::makeReturn(2006);
		} 
		$r["result"]=ResultState::successToArray();
		$r["state"]="ok";
	}else{
		$r["state"]="error";
		$r["errorCode"]=10010;
		$r["result"]=ResultState::toArray(2002,"memberID");
		
	}
	
	return $r;
}


function help_adduserdata(){

	$r["discript"] = "유저데이터 키 더하기";

	$r["param"][] = array("name"=>"memberID","type"=>"string","discript"=>"멤버ID");
	$r["param"][] = array("name"=>"key","type"=>"string","discript"=>"올릴키");
	$r["param"][] = array("name"=>"value","type"=>"int","discript"=>"값");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	
	return $r;
}


function adduserdata($p){
	$memberid = $p["memberID"];
	$key = $p["key"];
	$value = $p["value"];
	$safekey = $p["safekey"];
	if($memberid){
		$user = new UserData($memberid);
		
		$udata = json_decode($user->m_data,true);
		if(is_numeric($udata[$key]))$udata[$key]+=$value;
		else if(!$udata[$key])$udata[$key]=$value;
		
		$user->m_data = json_encode($udata,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
/*		safeway
		if($safekey){
			$now = TimeManager::get()->getCurrentDateString();
			if(!($user->m_safekey==$safekey)){
				$r["state"]="error";
				$r["error"]="not safe";
				return $r;
			}	
			$user->m_safekey=$safekey;
		}
*/
		
		if($user->save())return ResultState::makeReturn(2006);
		
		$r["value"]=$udata[$key];
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
		
	}
	
	return $r;
}

function help_addfriend(){

	$r["discript"] = "친구추가";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","discript"=>"추가할 친구 ID");
	$r["param"][] = array("name"=>"friendMax","type"=>"int","discript"=>"추가가능한 친구 인원(미입력시 500,최대 500)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2004,"친구한도초과");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	
	return $r;
}

function addfriend($p){
	$memberid = $p["memberID"];
	$friendid = $p["friendID"];
	$friendMax = $p["friendMax"];
	if(!$friendMax)$friendMax=500;
	if($friendMax>500)$friendMax=500;
	
	if($memberid){
		$user = new UserData($memberid);
		
		if(!$user->isLoaded())return ResultState::makeReturn(2005);
		
		$friendList = json_decode($user->m_friendList,true);
		
		if($friendMax>0 && count($friendList)>$friendMax){
			$r["state"]="error";
			$r["errorCode"]=10030;
			$r["result"]=ResultState::toArray(2004,"friend max");
			return $r;
		}
		
		$user->addFriend($friendid);
		if($user->save())return ResultState::makeReturn(2006);
		
		$r["friendList"]=json_decode($user->m_friendList,true);
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
	}
	
	return $r;
}

function help_addfriendeach(){

	$r["discript"] = "서로친구추가";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","discript"=>"추가할 친구 ID");
	$r["param"][] = array("name"=>"friendMax","type"=>"int","discript"=>"추가가능한 친구 인원");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2004,"친구한도초과");
	$r["result"][]=ResultState::toArray(2005,"유저정보찾을수없음");
	$r["result"][]=ResultState::toArray(2006,"유저정보저장 실패");
	
	return $r;
}

function addfriendeach($p){
	LogManager::get()->addLog("addfriendeach");
	$memberid = $p["memberID"];
	$friendid = $p["friendID"];
	$friendMax = $p["friendMax"];
	if(!$friendMax)$friendMax=500;
	if(!$memberid || !$friendid){
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID or friendID");
		return $r;
	}
	
	$user = new UserData($memberid);
	$userfriendList = json_decode($user->m_friendList,true);
	
	$friend = new UserData($friendid);
	$friendfriendList = json_decode($friend->m_friendList,true);
	
	if(!$user->isLoaded() || !$friend->isLoaded()){
		$r["state"]="error";
		$r["errorCode"]=10020;
		$r["result"]=ResultState::toArray(2005);
		return $r;
	}
	
	if($friendMax>0 && count($userfriendList)>$friendMax){
		$r["state"]="error";
		$r["errorCode"]=10030;
		$r["result"]=ResultState::toArray(2004,"나의 친구 한도초과");
		return $r;
	}
	
	if($friendMax>0 && count($friendfriendList)>$friendMax){
		$r["state"]="error";
		$r["errorCode"]=10031;
		$r["result"]=ResultState::toArray(2004,"상대편 친구 한도초과");
		return $r;
	}
		
	$user->addFriend($friendid);
	if(!$user->save()){
		return ResultState::makeReturn(2006);
	}
	
	$friend->addFriend($memberid);
	if(!$friend->save()){
		return ResultState::makeReturn(2006);
	}
	
	$r["friendInfo"]=$friend->getArrayData(true);
	$r["friendList"]=json_decode($user->m_friendList,true);
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_removefriendeach(){

	$r["discript"] = "서로친구삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","discript"=>"삭제할 친구 ID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function removefriendeach($p){
	
	$memberid = $p["memberID"];
	$friendid = $p["friendID"];
	
	$r1 = $this->removefriend($p);
	
	if($r1["result"]["code"]==1){
		$p2["memberID"]=$p["friendID"];
		$p2["friendID"]=$p["memberID"];
		
		$r = $this->removefriend($p2);
		
		return $r;
	}else{
		return $r1;
	}
}


function help_removefriend(){

	$r["discript"] = "친구삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","discript"=>"삭제할 친구 ID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2006,"유저정보저장 실패");
	
	return $r;
}


function removefriend($p){
	$memberid = $p["memberID"];
	$friendid = $p["friendID"];
	if($memberid){
		$user = new UserData($memberid);
		$friendList = json_decode($user->m_friendList,true);
		
		$index = array_search($friendid, $friendList);
		array_splice($friendList, $index, 1);
		$friendList = array_unique($friendList);				
		$user->m_friendList = json_encode($friendList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		if(!$user->save())return ResultState::makeReturn(2006);
		
		$r["list"]=$user->m_friendList;
		$r["result"]=ResultState::successToArray();
		$r["state"]="ok";
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
	}
	
	return $r;
}


function help_getfriendlist(){

	$r["discript"] = "친구목록받아오기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버ID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	
	return $r;
}


function getfriendlist($p){
	$memberid = $p["memberID"];
	if($memberid){
		$user = new UserData($memberid);	
		$list=json_decode($user->m_friendList,true); //json_encode($user->m_friendList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		for($i=0;$i<count($list);$i++){
			$friend = new UserData($list[$i]);
			if($friend->isLoaded())$fList[]=$friend->getArrayData(true);	
		}
		
		$r["list"]=$fList;
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
	}
	
	return $r;
}

function help_getuserlistbyrandom(){

	$r["discript"] = "추천친구목록";

	$r["param"][] = array("name"=>"limit","type"=>"int","discript"=>"갯수, (기본 10, 최대 50)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"fail to load list");
	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
	return $r;
}


function getuserlistbyrandom($p){
	
	$limit = 10;
	if(is_numeric($p["limit"]))$limit = $p["limit"];
	
	if($limit>50)$limit=50;
	
	
	$shardConn = DBManager::get()->getConnectionByShardKey(rand(0,10));
	if(!$shardConn) return ResultState::makeReturn(1002,"fail to get shardConnection");
	
	$result = mysql_query("select memberID,nick,lastDate from ".DBManager::getST("userdata")." where nick<>'' limit $limit",$shardConn);
	
	if(!$result)return ResultState::makeReturn(1001,"fail to load list");
	
	$list=array();
	
	while($userdata = mysql_fetch_array($result,MYSQL_ASSOC)){
		$list[]=$userdata;
	}
	
	$r["list"]=$list;
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	return $r;
}

/////////////////////////////////////////

function help_sendmessage(){

	$r["discript"] = "메세지보내기";

	$r["param"][] = array("name"=>"receiverMemberID","type"=>"string or int","discript"=>"받는사람아이디");
	$r["param"][] = array("name"=>"senderMemberID","type"=>"string or int","discript"=>"보내는사람아이디");
	$r["param"][] = array("name"=>"content","type"=>"string","discript"=>"내용");
	$r["param"][] = array("name"=>"type","type"=>"int","discript"=>"메세지타입");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


function sendmessage($p){	
	
	//보내기
	$message = new Message();
	$message->m_memberID=$p["receiverMemberID"];
	$message->m_content=$p["content"];
	$message->m_regDate=TimeManager::get()->getCurrentDateString();
	$message->m_friendID=$p["senderMemberID"];
	$message->m_type=$p["type"];
	$message->m_isSendMsg=0;
	$r["send"]=$message->send();
	
	
	//보낸편지로 등록
	$message2 = new Message();
	$message2->m_memberID=$p["senderMemberID"];
	$message2->m_content=$p["content"];
	$message2->m_regDate=TimeManager::get()->getCurrentDateString();
	$message2->m_friendID=$p["receiverMemberID"];
	$message2->m_type=$p["type"];
	$message2->m_isSendMsg=1;
	$r["receive"]=$message2->send();

	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_sendmessagebylist(){

	$r["discript"] = "메세지 여러사람에게 보내기";

	$r["param"][] = array("name"=>"receiverMemberIDList","type"=>"array(string or int)","discript"=>"받는사람아이디목록");
	$r["param"][] = array("name"=>"senderMemberID","type"=>"string or int","discript"=>"보내는사람아이디");
	$r["param"][] = array("name"=>"content","type"=>"string","discript"=>"내용");
	$r["param"][] = array("name"=>"type","type"=>"int","discript"=>"메세지타입");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"receiverMemberIDList");
	
	return $r;
}

function sendmessagebylist($p){	
	$memberidlist = $p["receiverMemberIDList"];
	if(!is_array($memberidlist)){
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"receiverMemberIDList");
		return $r;
	}
	
	foreach($memberidlist as $rmemberid){
		//보내기
		$message = new Message();
		$message->m_memberID=$rmemberid;
		$message->m_content=$p["content"];
		$message->m_regDate=TimeManager::get()->getCurrentDateString();
		$message->m_friendID=$p["senderMemberID"];
		$message->m_type=$p["type"];
		$message->m_isSendMsg=0;
		$r["send"][]=$message->send();
		
		//보낸편지로 등록
		$message2 = new Message();
		$message2->m_memberID=$p["senderMemberID"];
		$message2->m_content=$p["content"];
		$message2->m_regDate=TimeManager::get()->getCurrentDateString();
		$message2->m_friendID=$rmemberid;
		$message2->m_type=$p["type"];
		$message2->m_isSendMsg=1;
		$r["receive"][]=$message2->send();
	}

	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	
	return $r;
}

function help_getmessagelist(){

	$r["discript"] = "메세지목록 받기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버아이디");
	$r["param"][] = array("name"=>"type","type"=>"int","discript"=>"메세지타입, (0혹은 미입력시 모두받아옴)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
	return $r;
}


function getmessagelist($p){
	$whereType = "";
	$shardConn = DBManager::get()->getConnectionByShardKey($p["memberID"]);
	if(!$shardConn)return ResultState::makeReturn(1002,"fail to get shardConnection");
	
	if($p["type"])$whereType=" and type='".$p["type"]."'";
	$result = mysql_query("select * from MessageTable where memberID=".$p["memberID"].$whereType." and isSendMsg=0",$shardConn);
	
	$mlist=array();
	while($message = mysql_fetch_array($result,MYSQL_ASSOC)){
		$mlist[]=$message;
	}
	$r["list"]=$mlist;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_removemessage(){

	$r["discript"] = "메세지삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버아이디");
	$r["param"][] = array("name"=>"no","type"=>"int","discript"=>"메세지번호");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
	return $r;
}


function removemessage($p){
	//메세지삭제	
	
	$message = new Message($p["memberID"],$p["no"]);
	if($message->isLoaded()){
		$message->remove();
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2003);
	}
	return $r;
}

function help_removemessagebylist(){

	$r["discript"] = "메세지 여러개 삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버아이디");
	$r["param"][] = array("name"=>"noList","type"=>"array(int)","discript"=>"메세지번호목록");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
	return $r;
}


function removemessagebylist($p){
	//메세지삭제	
	if(!is_array($p["noList"])){
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"noList");
		return $r;
	}
	
	
	$shardConn = DBManager::get()->getConnectionByShardKey($p["memberID"]);
	if(!$shardConn)return ResultState::makeReturn(1002,"fail to get shardConnection");
	
	$messageNos = implode(",", $p["noList"]);
	$whereNo = " and no IN (".$messageNos.")";
	$result = mysql_query("delete from MessageTable where memberID=".$p["memberID"].$whereNo." and isSendMsg=0",$shardConn);
	
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_removeallmessage(){

	$r["discript"] = "메세지 전부삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버아이디");
	$r["param"][] = array("name"=>"type","type"=>"int","discript"=>"메세지타입(0혹은 입력안할시 모두삭제)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
	return $r;
}

function removeallmessage($p){
	if($p["type"] && $p["type"]>0)$whereType=" and type='".$p["type"]."'";
	
	$shardConn = DBManager::get()->getConnectionByShardKey($p["memberID"]);
	if(!$shardConn)return ResultState::makeReturn(1002,"fail to get shardConnection");
	
	$result = mysql_query("delete from MessageTable where memberID=".$p["memberID"].$whereType." and isSendMsg=0",$shardConn);
	
	if($result){
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["error"]="error";
		$r["result"]=ResultState::toArray(1001);
	} 
		
	return $r;
}
/////////////////////////////////////////

function help_setweeklyscore(){

	$r["discript"] = "주간점수등록";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버아이디");
	$r["param"][] = array("name"=>"score","type"=>"int","discript"=>"점수");
	$r["param"][] = array("name"=>"data","type"=>"string","discript"=>"데이터");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function setweeklyscore($p){

	$ws =new WeeklyScore($p["memberID"],TimeManager::get()->getCurrentWeekNo());
	$ws->m_memberID = $p["memberID"];
	$ws->m_data = $p["data"];
	$ws->m_regDate = TimeManager::get()->getCurrentDateString();
	$ws->m_regWeek = TimeManager::get()->getCurrentWeekNo();
	
	$r["update"]=false;
	if($ws->m_score<$p["score"]){
		$r["update"]=true;
		$ws->m_score=$p["score"];
		$r["uresult"]=$ws->save();
	}else{
		$r["uresult"]=$ws->save();
	}
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_getweeklyscorelist(){

	$r["discript"] = "주간점수목록";

	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string or int)","discript"=>"멤버아이디목록");
	$r["param"][] = array("name"=>"weekNo","type"=>"int","discript"=>"주간번호 없을경우 이번주");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getweeklyscorelist($p){
	$memberlist = $p["memberIDList"];
	$weekNo = TimeManager::get()->getCurrentWeekNo();
	if($p["weekNo"])$weekNo=$p["weekNo"];
	
	$list=array();
	foreach($memberlist as $key=>$value){
		$ws=new WeeklyScore($value,$weekNo);
		if($ws->isLoaded()){
			$list[]=$ws->getArrayData(true);
		}
	}
	
	$r["list"]=$list;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}
////////////////////////////////////////////////////////////////////////////////////

function help_setstagescore(){

	$r["discript"] = "스테이지점수등록";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버아이디");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","discript"=>"스테이지번호");
	$r["param"][] = array("name"=>"data","type"=>"string","discript"=>"데이터");
	$r["param"][] = array("name"=>"score","type"=>"int","discript"=>"점수");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function setstagescore($p){
	$stageNo = $p["stageNo"];
	$ss = new StageScore($stageNo,$p["memberID"]);
	$ws->m_memberID = $p["memberID"];
	$ws->m_stageNo = $stageNo;
	$ws->m_data = $p["data"];
	$ws->m_regDate = TimeManager::get()->getCurrentDateString();
	
	$r["update"]=false;
	if($ws->m_score<$p["score"]){
		$r["update"]=true;
		$ws->m_score=$p["score"];
		$ws->save();
	}
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_addstagescore(){

	$r["discript"] = "스테이지점수추가";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","discript"=>"멤버아이디");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","discript"=>"스테이지번호");
	$r["param"][] = array("name"=>"data","type"=>"string","discript"=>"데이터");
	$r["param"][] = array("name"=>"score","type"=>"int","discript"=>"점수");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function addstagescore($p){
	$stageNo = $p["stageNo"];
	$ss = new StageScore($stageNo,$p["memberID"]);
	$ss->m_memberID = $p["memberID"];
	$ss->m_stageNo = $stageNo;
	$ss->m_data = $p["data"];
	$ss->m_regDate = TimeManager::get()->getCurrentDateString();
	$ss->m_score+=$p["score"];	
	$ss->save();
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}


function help_getstagescorelist(){

	$r["discript"] = "스테이지점수목록";

	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string or int)","discript"=>"멤버아이디목록");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","discript"=>"스테이지번호");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getstagescorelist($p){
	$memberlist = $p["memberIDList"];
	$stageNo = $p["stageNo"];
	
	$list=array();
	foreach($memberlist as $key=>$value){
		$ws=new StageScore($stageNo,$value);
		if($ws->isLoaded()){
			$list[]=$ws->getArrayData(true);
		}
	}
	
	$r["list"]=$list;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}


function gettopstagescorelist($p){

}


//////////////////////////////////////////

}


if(!$stopCommand){
	$command = new commandClass();
	
	for($c=0;$c<count($param);$c++){
		$cmd = (string)$c;
		if(!$param[$cmd])continue;
		$p = $param[$cmd]["p"];
		$a = strtolower($param[$cmd]["a"]);
		
		
		if(method_exists($command,$a)){
			$r = $command->$a($p);
			$r[log] = LogManager::get()->getLogAndClear();
		 	$allResult[$cmd]= $r;
		 	
		 	$p2 = array();
		 	$p2["memberID"]= $param["memberID"];
			if(!$p2["memberID"])$p2["memberID"]= $param["hspMemberNo"];
			if(!$p2["memberID"])$p2["memberID"]= $param["memberNo"];
	
			$p2["category"]=$a;
			$p2["content"]=json_encode($p,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$p2["output"]=$allResult[$cmd];
			
			if($a!="writelog")$command->writelog($p2);
			
		}else if($a=="help"){
			$class_methods = get_class_methods('commandClass');
			foreach ($class_methods as $method_name) {
			    echo "$method_name <br>";
			}
			mysql_close();
			exit;
		}else{
			$p["api"]=$param[$cmd]["a"];
			$allResult[$cmd]= $command->httpgateway($p);
			
			
			$p2=array();
		 	$p2["memberID"]= $param["memberID"];
			if(!$p2["memberID"])$p2["memberID"]= $param["hspMemberNo"];
			if(!$p2["memberID"])$p2["memberID"]= $param["memberNo"];
					
			$p2["category"]=$p["api"];
			$p2["content"]=json_encode($p,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$p2["output"]=$allResult[$cmd];
			$command->writelog($p2);
			//$allResult[$cmd]=array("state"=>"error","msg"=>"don't find command");
		}
		
		
	}
	$allResult[state]="ok";
	$allResult[timestamp]=time();
	
	$allResult = json_encode($allResult,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	
	if($mode=="nodes"){
		echo $allResult;
	}else{
		echo base64_encode(@mcrypt_encrypt(MCRYPT_DES, $SECRETKEY,$allResult, MCRYPT_MODE_ECB))."#";		
	}
	
	
	DBManager::get()->closeDB();
	@mysql_close();

}
?>