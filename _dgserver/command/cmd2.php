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

function help_httpgateway(){

	$r["description"] = "httpgateway를 이용해 hsp서버에 접속합니다.";
	
	//$r["param"]
	
	$r["param"][] = array("name"=>"api","type"=>"string","description"=>"사용할 api 이름");
	$r["param"][] = array("name"=>"version","type"=>"float","description"=>"hsp버전(없으면 기본값설정됨)".DBManager::$m_httpgateway["version"].")");
	$r["param"][] = array("name"=>"headerGameNo","type"=>"int","description"=>"게임번호(없으면 기본값설정됨) : ".DBManager::$m_gameNo.")");
	$r["param"][] = array("name"=>"gameNo","type"=>"int","description"=>"게임번호(없으면 기본값설정됨): ".DBManager::$m_gameNo.")");
	$r["param"][] = array("name"=>"url","type"=>"string","description"=>"httpgateway주소(없으면 기본값)".DBManager::$m_httpgateway["URL"].")");
	$r["param"][] = array("name"=>"help","type"=>"bool","description"=>"true면 api정보보기(기본값 false)");

	//$r["return"]
	
	$r["result"][]=ResultState::toArray(3001,"httpgateway 접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}

function httpgateway($data) {

	if(!$data[version])$data[version]=DBManager::$m_httpgateway["version"];
	if(!$data[headerGameNo])$data[headerGameNo]=DBManager::$m_gameNo;
	

	$url = DBManager::$m_httpgateway["URL"];
	if($data["url"]){
		$url = $data["url"];
		unset($data["url"]);
	}

	if($data["api"]=="apilist"){
		unset($data["api"]);
		$url =DBManager::$m_httpgateway["helpURL"];
	}

	if($data["help"]){
		$url = DBManager::$m_httpgateway["helpURL"];
	}
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
	curl_setopt($curlsession, CURLOPT_URL, $url."?".$fields_string); //hsp/httpgw/help.json?api=$api&version=1.3
	curl_setopt($curlsession, CURLOPT_PORT, DBManager::$m_httpgateway["port"]);
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
		$r["result"]=ResultState::toArray(3001,"httpgateway error");
		$r["msg"]="gateway server error";
		$r["send"]=$url."?".$fields_string;
		return $r;
	}else{
		$r["data"]=json_decode($buffer,true);
		$r["result"]=ResultState::successToArray();
		$r["state"]="ok";
		$r["send"]=$url."?".$fields_string;
		return $r;
	}
}
/////////////////////////////////////////
/////////////////////////////////////////


function help_requestitemdelivery(){

	$r["description"] = "구매내역을 확인하여 아이템을 배송합니다.";
	
	//$r["param"]
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(3001,"httpgateway 접속실패");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2016,"처리할내용이 없음");
	$r["result"][]=ResultState::toArray(1,"success");

	//$r["example"]
	
	return $r;
}


function requestitemdelivery($p){
	$memberID = $p["memberID"];

	if(!$memberID) return ResultState::makeReturn(2002,"memberID");

	$pp["headerGameNo"] = DBManager::$m_gameNo;
	$pp["api"]="RequestItemDelivery2";
	$pp["deliveryMaxCount"]=10;
	$pp["deliveryHeader"]="memberNo-".$p["memberID"].",gameNo-".DBManager::$m_gameNo;
	$result = $this->httpgateway($pp);
	LogManager::get()->addLog("RequestItemDelivery2-->".json_encode($result));
	if(!ResultState::successCheck($result["result"]))return $result;
	
	$r = array();

	$ppp=array();
	$ppp["memberID"]=$p["memberID"];
	$ppp["list"]=array();
	$itemSeq=array();
	for($i=0;$i<count($result["data"]["deliveryResponse"]["itemIds"]);$i++){
		LogManager::get()->addLog("item id is ".$result["data"]["deliveryResponse"]["itemIds"][$i]);
		$item = new Shop($result["data"]["deliveryResponse"]["itemIds"][$i]);
	
		if(!$item->isLoaded())continue;
		
		$ppp["list"][] = array("type"=>$item->type,"count"=>$item->count,"statsID"=>"purchase","statsValue"=>"","content"=>"루비구매","isPurchase"=>true);
		$r["list"][]=$item->getArrayData();
		$itemSeq[]=$result["data"]["deliveryResponse"]["itemDeliverySequences"][$i];
	}
	
	if(count($result["data"]["deliveryResponse"]["itemIds"])>0){
		$result2 = $this->changeuserproperties($ppp);
		
		if(ResultState::successCheck($result2["result"])){
			$pppp=array();
			$pppp["headerGameNo"] = DBManager::$m_gameNo;
			$pppp["api"]="FinishItemDelivery2";
			$pppp["itemDeliverySequences"]=implode(",",$itemSeq);
			$pppp["deliveryHeader"]="memberNo-".$p["memberID"].",gameNo-".DBManager::$m_gameNo;
			$result3 = $this->httpgateway($pppp);
			LogManager::get()->addLog("-->".json_encode($result3));
			if(ResultState::successCheck($result3["result"])){
				return $result2;
			}
			return $result3;
		}
		return $result2;

	}

	return ResultState::makeReturn(2016,"nothingwork");
	
}

////////////////////////////////////////
function help_getcommonsetting(){

	$r["description"] = "게임 기본 설정을 가져옵니다.";
	
	//$r["param"]
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}

function getcommonsetting($p){
	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$result = mysql_query("select * from ".DBManager::get()->getMT("commonsetting"),$mainConn);  
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

/////////////////////////////////////////
function help_getformsetter(){

	$r["description"] = "오브젝트 위치를 잡습니다.";
	
	//$r["param"]
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}

function getformsetter($p){
	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$result = mysql_query("select * from ".DBManager::get()->getMT("formsetter"),$mainConn);  
	$r = array();
	
	if(!$result)return ResultState::makeReturn(1001,mysql_error());
	
	while($setting = mysql_fetch_array($result,MYSQL_ASSOC)){
		$key = $setting["name"];
		$value = json_decode($setting["data"],true);
		
		$r[$key]=$value;
	}
	
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	
	return $r;
}

////////////////////////////////////////////////
function help_getnoticelist(){

	$r["description"] = "공지사항목록을 가져옵니다.";
	//$r["param"]
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}

function getnoticelist($p){

	global $nowurl;
	
	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$nowDate = TimeManager::get()->getCurrentDateString();
	

	$r = array();

	while($rData = Notice::getRowByQuery("where startDate<$nowDate and endDate>$nowDate order by `order` asc")){
		$rData[imgInfo]=json_decode($rData[imgInfo],true);
		$rData[imgInfo][img]=$nowurl."/images/".$rData[imgInfo][img];		
		$r["list"][]=$rData;
	}

	
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	
	return $r;
}

/////////////////////////////////////////

function help_getcharacterlist(){

	$r["description"] = "케릭터 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getcharacterlist($p){
	global $nowurl;

	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$listVer = kvManager::get("charListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		
		return $r;
	}
	
	$result =  mysql_query("select * from ".DBManager::get()->getMT("character"),$mainConn);
	
	if(!$result)return ResultState::makeReturn(1001,"getcharacterlist error");
	
	$list = array();
	while($charInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
			$charInfo[purchaseInfo]=json_decode($charInfo[purchaseInfo],true);
			$charInfo[statInfo]=json_decode($charInfo[statInfo],true);
			$charInfo[resourceInfo]=json_decode($charInfo[resourceInfo],true);
			$charInfo[resourceInfo][ccbi]=$nowurl."/".$charInfo[resourceInfo][ccbi];
			$charInfo[resourceInfo][plist]=$nowurl."/".$charInfo[resourceInfo][plist];
			$charInfo[resourceInfo][pvrccz]=$nowurl."/".$charInfo[resourceInfo][pvrccz];
	
			$list[]=$charInfo;
	}
	
	$r["list"]=$list;
	$r["version"]=$listVer;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

/////////////////////////////////////////

function help_getshoplist(){

	$r["description"] = "샵 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getshoplist($p){
	global $nowurl;

	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$listVer = kvManager::get("shopListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		
		return $r;
	}
	
	$result =  mysql_query("select * from ".DBManager::get()->getMT("shop")." order by type asc, count asc",$mainConn);
	
	if(!$result)return ResultState::makeReturn(1001,"getshoplist error");
	
	$list = array();
	$oldlistname = "";
	while($charInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
			if($charInfo[type]!=$oldlistname)$listname=$charInfo[type];

			$charInfo[data]=json_decode($charInfo[data],true);
			$charInfo[image]=$nowurl."/".$charInfo[image];
			$list[$listname][]=$charInfo;
			$oldlistname = $listname;
	}
	
	$r["list"]=$list;
	$r["version"]=$listVer;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}


/////////////////////////////////////////
function help_getmonsterlist(){

	$r["description"] = "몬스터 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getmonsterlist($p){
	global $nowurl;

	$mainConn = DBManager::get()->getMainConnection();
	
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$listVer = kvManager::get("monsterListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		
		return $r;
	}
	
	$result =  mysql_query("select * from ".DBManager::get()->getMT("monster"),$mainConn);
	
	if(!$result)return ResultState::makeReturn(1001,"getmonsterlist error");
	
	$list = array();
	while($mobInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
			$mobInfo[resourceInfo]=json_decode($mobInfo[resourceInfo],true);
			$mobInfo[resourceInfo][ccbi]=$nowurl."/".$mobInfo[resourceInfo][ccbi];
			$mobInfo[resourceInfo][plist]=$nowurl."/".$mobInfo[resourceInfo][plist];
			$mobInfo[resourceInfo][pvrccz]=$nowurl."/".$mobInfo[resourceInfo][pvrccz];
	
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

	$r["description"] = "스테이지 정보를 받아옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"스테이지 번호");
	
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
	if(!$pTable)$pTable=DBManager::get()->getMT("stage");
	
	$stageVer=0;
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");

	$siresult = mysql_query("select * from $pTable where no=$p[no]",$mainConn);
	if(!$siresult) return ResultState::makeReturn(1001,"stageinfo");
	
	$stageInfo = mysql_fetch_array($siresult,MYSQL_ASSOC);
	if(!$stageInfo) return ResultState::makeReturn(1003,"stageinfo");
	

	$stageVer = $stageInfo[version];

	if($stageVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		$r["result"]=ResultState::toArray(2001);
		return $r;
	}
	//$puzzleInfo = mysql_fetch_assoc(mysql_query("select * from ".DBManager::get()->getMT("puzzle")." where no = ".$stageInfo[puzzle],$mainConn));

	$stageInfo[shopItems]=json_decode($stageInfo[shopItems],true);
	$stageInfo[defItems]=json_decode($stageInfo[defItems],true);
	$stageInfo[cards]=json_decode($stageInfo[cards],true);
	$stageInfo[mission]=json_decode($stageInfo[mission],true);
	//$stageInfo[puzzleOrder] = $puzzleInfo[order];	
	
	$card = array();
	for($i=0;$i<count($stageInfo[cards]);$i++){
		$cardInfo = mysql_fetch_array(mysql_query("select * from ".DBManager::get()->getMT("card")." where no=".$stageInfo[cards][$i],$mainConn),MYSQL_ASSOC);
		if($cardInfo){
			$cardInfo[stage]=$stageInfo[no];
			$cardInfo[grade]=$i+1;
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
	return $this->getstageinfo($p,DBManager::get()->getMT("eventstage"));
}

///////////////////////////////////////////



/////////////////////////////////////////

function help_getpieceinfo(){

	$r["description"] = "피스 정보를 받아옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"피스 번호");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1003,"디비fetch실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getpieceinfo($p,$pTable=""){
	return $this->getstageinfo($p,DBManager::get()->getMT("piece"));
}

///////////////////////////////////////////


function help_getcardlist(){

	$r["description"] = "카드정보 목록을 받아옵니다.";
	
	$r["param"][] = array("name"=>"noList","type"=>"array(int)","description"=>"카드번호목록");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"fail to get mainConnection");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getcardlist($p){
	global $nowurl;
	$cardlist="";
	
	if(is_array($p["noList"]))
		$cardlist = join(',',$p["noList"]);
	else 
		$cardlist = $p["noList"];
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$result = mysql_query("select * from ".DBManager::get()->getMT("card")." where no IN ($cardlist)",$mainConn);  
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

function help_getcardinfobyrand(){

	$r["description"] = "카드를 랜덤으로 받아옵니다.";
	
	$r["param"][] = array("name"=>"type","type"=>"string","description"=>"gold or ruby or social");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"fail to get mainConnection");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getcardinfobyrand($p){
	global $nowurl;
	

	$cType = $p["type"];
	
	if($cType=="gold"){
		$cLevel = 1;
	}else if($cType=="ruby"){
		$cLevel = 2;
	}else if($cType=="social"){
		$cLevel = 3;
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
		return $r;
	}



	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$result = mysql_query("select * from ".DBManager::get()->getMT("card")." where stage<>0 order by rand() limit 1",$mainConn);  
	if(!$result)return ResultState::makeReturn(1001);
	
	$cardInfo = mysql_fetch_array($result,MYSQL_ASSOC);
	$cardInfo[ability]=json_decode($cardInfo[ability],true);
	$cardInfo[missile]=json_decode($cardInfo[missile],true);
	$cardInfo[aniInfo]=json_decode($cardInfo[aniInfo],true);
	$cardInfo[imgInfo]=json_decode($cardInfo[imgInfo],true);
	$cardInfo[silImgInfo]=json_decode($cardInfo[silImgInfo],true);		
	$cardInfo[imgInfo][img]=$nowurl."/images/".$cardInfo[imgInfo][img];
	$cardInfo[aniInfo][detail][img]=$nowurl."/images/".$cardInfo[aniInfo][detail][img];
	$cardInfo[silImgInfo][img]=$nowurl."/images/".$cardInfo[silImgInfo][img];
	$r["cardInfo"]=$cardInfo;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
	// $list = array();
	// while($cardInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
	// 	if($cardInfo){
	// 		$cardInfo[ability]=json_decode($cardInfo[ability],true);
	// 		$cardInfo[missile]=json_decode($cardInfo[missile],true);
	// 		$cardInfo[aniInfo]=json_decode($cardInfo[aniInfo],true);
	// 		$cardInfo[imgInfo]=json_decode($cardInfo[imgInfo],true);
	// 		$cardInfo[silImgInfo]=json_decode($cardInfo[silImgInfo],true);		
	// 		$cardInfo[imgInfo][img]=$nowurl."/images/".$cardInfo[imgInfo][img];
	// 		$cardInfo[aniInfo][detail][img]=$nowurl."/images/".$cardInfo[aniInfo][detail][img];
	// 		$cardInfo[silImgInfo][img]=$nowurl."/images/".$cardInfo[silImgInfo][img];
	// 		$list[]=$cardInfo;
	// 	}
	// }
	
	// $r["list"]= $list; //json_decode(json_encode($list,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK),true);
	// $r["state"]="ok";
	// $r["result"]=ResultState::successToArray();
	// return $r;

}



//////////////////////////////////////////
function help_getpuzzleinfo(){

	$r["description"] = "퍼즐 정보를 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"퍼즐정보버전");
	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"퍼즐 번호");
	$r["param"][] = array("name"=>"order","type"=>"int","description"=>"퍼즐 순서 (퍼즐번호 없을시 입력)");
	
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
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	
	// $puzzleVer = kvManager::get("puzzleVer_".$p[no],1);



	$piresult = null;
	if($p[no])$piresult = mysql_query("select * from ".DBManager::get()->getMT("puzzle")." where no=$p[no]",$mainConn);
	else if($p[order])$piresult = mysql_query("select * from ".DBManager::get()->getMT("puzzle")." where `order`=$p[order]",$mainConn);

	if(!$piresult)return ResultState::makeReturn(1001,"puzzle info");
	
	$puzzleInfo =  mysql_fetch_array($piresult,MYSQL_ASSOC);
	if(!$puzzleInfo)return ResultState::makeReturn(1003,"puzzle info");
	

	$puzzleVer = $puzzleInfo[version];
	if($puzzleVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		$r["result"]=ResultState::toArray(2001);
		return $r;
	}


	//$puzzleInfo[version]=$puzzleVer;
	$puzzleInfo[center] = json_decode($puzzleInfo[center],true);
	$puzzleInfo[center][image]=$nowurl."/images/".$puzzleInfo[center][image];
	$puzzleInfo[face] = json_decode($puzzleInfo[face],true);
	$puzzleInfo[face][image]=$nowurl."/images/".$puzzleInfo[face][image];
	$puzzleInfo[original] = json_decode($puzzleInfo[original],true);
	$puzzleInfo[original][image]=$nowurl."/images/".$puzzleInfo[original][image];
	$puzzleInfo[thumbnail] = json_decode($puzzleInfo[thumbnail],true);
	$puzzleInfo[thumbnail][image]=$nowurl."/images/".$puzzleInfo[thumbnail][image];
	$puzzleInfo[map] = json_decode($puzzleInfo[map],true);
	$puzzleInfo[map][image]=$nowurl."/images/".$puzzleInfo[map][image];
	$puzzleInfo[pathInfo] = json_decode($puzzleInfo[pathInfo],true);
	$puzzleInfo[coordinateInfo] = json_decode($puzzleInfo[coordinateInfo],true);
	$puzzleInfo[startPosition]=json_decode($puzzleInfo[startPosition],true);
	$puzzleInfo[endPosition]=json_decode($puzzleInfo[endPosition],true);
	
	
	$query = mysql_query("select * from ".DBManager::get()->getMT("piece")." where puzzle=".$puzzleInfo[no]." order by no asc",$mainConn);
	if(!$query)return ResultState::makeReturn(1001,"piece list");
		
	$stagelist=array();
	$cnt=0;
	$startStageNo=0;
	while($stageInfo = mysql_fetch_array($query,MYSQL_ASSOC)){
		if($startStageNo==0)$startStageNo=$stageInfo[no];
		$stageInfo[puzzleOrder] = $puzzleInfo[order];
		$stageInfo["condition"] = json_decode($stageInfo["condition"],true);
		$stageInfo[shopItems]=json_decode($stageInfo[shopItems],true);
		$stageInfo[defItems]=json_decode($stageInfo[defItems],true);
		$stageInfo[cards]=json_decode($stageInfo[cards],true);
		$stageInfo[mission]=json_decode($stageInfo[mission],true);
		$card = array();
		for($i=0;$i<count($stageInfo[cards]);$i++){
			$cardInfo = mysql_fetch_array(mysql_query("select * from ".DBManager::get()->getMT("card")." where no=".$stageInfo[cards][$i],$mainConn),MYSQL_ASSOC);
			if($cardInfo){

				$cardInfo[stage]=$stageInfo[no];
				$cardInfo[grade]=$i+1;
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
	
	//$puzzleInfo[no]=$puzzleInfo[puzzleNo];

	$puzzleInfo["list"]=$stagelist;
	$puzzleInfo["startStage"]=$startStageNo;
	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
}

/////////////////////////////////////////////
function help_geteventpuzzlelist(){

	$r["description"] = "이벤트 퍼즐 정보 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"퍼즐정보목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"얻어올 시작 퍼즐번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function geteventpuzzlelist($p){
	global $nowurl;
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$puzzleListVer = kvManager::get("epuzzleListVer",1);

	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
	}else{
		// 여기서 eventstagelistversion 도 비교
		if(!$p[start])$p[start]=0;
		if(!$p[limit])$p[limit]=100;
		
		$query = mysql_query("select no,`order`,thumbnail,title,point,ticket from ".DBManager::get()->getMT("puzzle")." where isEvent=1 order by `order` asc limit $p[start],$p[limit]",$mainConn);
		
		$list=array();
		$cnt=0;
		while($pData = mysql_fetch_array($query,MYSQL_ASSOC)){
			//퍼즐 시작stage 알아오기
			$r1 = mysql_query("select no from ".DBManager::get()->getMT("piece")." where puzzle=".$pData[no]." order by no asc limit 1",$mainConn);
			if($r1){
				$r1d = mysql_fetch_array($r1,MYSQL_ASSOC);
				$pData["startStage"]=$r1d[no];
			}
			
			//퍼즐 안에 스테이지 갯수 알아오기
			$r2 = mysql_query("select count(no) from ".DBManager::get()->getMT("piece")." where puzzle=".$pData[no],$mainConn);
			if($r2){
				$r2d = mysql_fetch_array($r2);
				$pData["stageCount"]=$r2d[0];
			}
			
			$pData[thumbnail]=json_decode($pData[thumbnail],true);
			//$pData[pathInfo]=json_decode($pData[pathInfo],true);
			$pData[thumbnail][image]=$nowurl."/images/".$pData[thumbnail][image];

			// $pData[center] = json_decode($pData[center],true);
			// $pData[center][image]=$nowurl."/images/".$pData[center][image];
			// $pData[original] = json_decode($pData[original],true);
			// $pData[original][image]=$nowurl."/images/".$pData[original][image];
			// $pData[face] = json_decode($pData[face],true);
			// $pData[face][image]=$nowurl."/images/".$pData[face][image];
			$list[]=$pData;
			$cnt++;
		}
		
		$puzzleInfo["list"]=$list;
	}
	

	$puzzleInfo["version"]=$puzzleListVer;

	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
	
}

/////////////////////////////////////////////
function help_getpuzzlelist(){

	$r["description"] = "퍼즐 정보 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"퍼즐정보목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"얻어올 시작 퍼즐번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getpuzzlelist($p){
	global $nowurl;
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$puzzleListVer = kvManager::get("puzzleListVer",1);

	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
	}else{
		// 여기서 eventstagelistversion 도 비교
		if(!$p[start])$p[start]=0;
		if(!$p[limit])$p[limit]=100;
		
		$query = mysql_query("select no,`order`,title,point,ticket,thumbnail,pathInfo,cardInfo,rewardInfo,levelInfo,conditionInfo,coordinateInfo,startPosition,endPosition,map,center,original,face,`condition` from ".DBManager::get()->getMT("puzzle")." where isEvent=0 order by `order` asc limit $p[start],$p[limit]",$mainConn);
		
		$list=array();
		$cnt=0;
		while($pData = mysql_fetch_array($query,MYSQL_ASSOC)){
			//퍼즐 시작stage 알아오기
			$r1 = mysql_query("select no from ".DBManager::get()->getMT("piece")." where puzzle=".$pData[no]." order by no asc limit 1",$mainConn);
			if($r1){
				$r1d = mysql_fetch_array($r1,MYSQL_ASSOC);
				$pData["startStage"]=$r1d[no];
			}
			
			//퍼즐 안에 스테이지 갯수 알아오기
			$r2 = mysql_query("select count(no) from ".DBManager::get()->getMT("piece")." where puzzle=".$pData[no],$mainConn);
			if($r2){
				$r2d = mysql_fetch_array($r2);
				$pData["stageCount"]=$r2d[0];
			}
			
			//$pData[version]=kvManager::get("puzzleVer_".$pData[no],1);
			$pData[thumbnail]=json_decode($pData[thumbnail],true);
			$pData[thumbnail][image]=$nowurl."/images/".$pData[thumbnail][image];
			$pData[pathInfo]=json_decode($pData[pathInfo],true);
			$pData[cardInfo]=json_decode($pData[cardInfo],true);
			$pData[rewardInfo]=json_decode($pData[rewardInfo],true);
			$pData[levelInfo]=json_decode($pData[levelInfo],true);
			$pData[conditionInfo]=json_decode($pData[conditionInfo],true);
			$pData[coordinateInfo]=json_decode($pData[coordinateInfo],true);
			$pData[startPosition]=json_decode($pData[startPosition],true);
			$pData[endPosition]=json_decode($pData[endPosition],true);

			$pData[center] = json_decode($pData[center],true);
			$pData[center][image]=$nowurl."/images/".$pData[center][image];
			$pData[original] = json_decode($pData[original],true);
			$pData[original][image]=$nowurl."/images/".$pData[original][image];
			$pData[face] = json_decode($pData[face],true);
			$pData[face][image]=$nowurl."/images/".$pData[face][image];
			$pData[map] = json_decode($pData[map],true);
			$pData[map][image]=$nowurl."/images/".$pData[map][image];
			$list[]=$pData;
			$cnt++;
		}
		
		$puzzleInfo["list"]=$list;
		//$puzzleInfo["puzzlelist"]=$list;
	}
	

	$puzzleInfo["version"]=$puzzleListVer;
	$puzzleInfo["puzzlelistversion"]=$puzzleListVer;
	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
	
}

/////////////////////////////////////////////


//book



//////////////////////////////////////////
function help_getbookinfo(){

	$r["description"] = "책 정보를 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"책정보버전");
	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"책 번호");
	$r["param"][] = array("name"=>"order","type"=>"int","description"=>"책 순서 (책번호 없을시 입력)");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1003,"디비fetch실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getbookinfo($p){
	global $nowurl;
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	
	// $puzzleVer = kvManager::get("puzzleVer_".$p[no],1);



	$piresult = null;
	if($p[no])$piresult = mysql_query("select * from ".DBManager::get()->getMT("book")." where no=$p[no]",$mainConn);
	else if($p[order])$piresult = mysql_query("select * from ".DBManager::get()->getMT("book")." where `order`=$p[order]",$mainConn);

	if(!$piresult)return ResultState::makeReturn(1001,"book info");
	
	$puzzleInfo =  mysql_fetch_array($piresult,MYSQL_ASSOC);
	if(!$puzzleInfo)return ResultState::makeReturn(1003,"book info");
	

	$puzzleVer = $puzzleInfo[version];
	if($puzzleVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		$r["result"]=ResultState::toArray(2001);
		return $r;
	}


	//$puzzleInfo[version]=$puzzleVer;
	$puzzleInfo[center] = json_decode($puzzleInfo[center],true);
	$puzzleInfo[center][image]=$nowurl."/images/".$puzzleInfo[center][image];
	$puzzleInfo[face] = json_decode($puzzleInfo[face],true);
	$puzzleInfo[face][image]=$nowurl."/images/".$puzzleInfo[face][image];
	$puzzleInfo[original] = json_decode($puzzleInfo[original],true);
	$puzzleInfo[original][image]=$nowurl."/images/".$puzzleInfo[original][image];
	$puzzleInfo[thumbnail] = json_decode($puzzleInfo[thumbnail],true);
	$puzzleInfo[thumbnail][image]=$nowurl."/images/".$puzzleInfo[thumbnail][image];
	$puzzleInfo[map] = json_decode($puzzleInfo[map],true);
	$puzzleInfo[map][image]=$nowurl."/images/".$puzzleInfo[map][image];
	$puzzleInfo[pathInfo] = json_decode($puzzleInfo[pathInfo],true);
	$puzzleInfo[coordinateInfo] = json_decode($puzzleInfo[coordinateInfo],true);
	$puzzleInfo[startPosition]=json_decode($puzzleInfo[startPosition],true);
	$puzzleInfo[endPosition]=json_decode($puzzleInfo[endPosition],true);
	
	
	$query = mysql_query("select * from ".DBManager::get()->getMT("stage")." where book=".$puzzleInfo[no]." order by no asc",$mainConn);
	if(!$query)return ResultState::makeReturn(1001,"stage list");
		
	$stagelist=array();
	$cnt=0;
	$startStageNo=0;
	while($stageInfo = mysql_fetch_array($query,MYSQL_ASSOC)){
		if($startStageNo==0)$startStageNo=$stageInfo[no];
		$stageInfo[puzzleOrder] = $puzzleInfo[order];
		$stageInfo["condition"] = json_decode($stageInfo["condition"],true);
		$stageInfo[shopItems]=json_decode($stageInfo[shopItems],true);
		$stageInfo[defItems]=json_decode($stageInfo[defItems],true);
		$stageInfo[cards]=json_decode($stageInfo[cards],true);
		$stageInfo[mission]=json_decode($stageInfo[mission],true);
		$card = array();
		for($i=0;$i<count($stageInfo[cards]);$i++){
			$cardInfo = mysql_fetch_array(mysql_query("select * from ".DBManager::get()->getMT("card")." where no=".$stageInfo[cards][$i],$mainConn),MYSQL_ASSOC);
			if($cardInfo){

				$cardInfo[stage]=$stageInfo[no];
				$cardInfo[grade]=$i+1;
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
	
	//$puzzleInfo[no]=$puzzleInfo[puzzleNo];

	$puzzleInfo["list"]=$stagelist;
	$puzzleInfo["startStage"]=$startStageNo;
	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
}

/////////////////////////////////////////////
function help_geteventbooklist(){

	$r["description"] = "이벤트 책 정보 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"책정보목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"얻어올 시작 책번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function geteventbooklist($p){
		global $nowurl;
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$puzzleListVer = kvManager::get("ebookListVer",1);

	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
	}else{
		// 여기서 eventstagelistversion 도 비교
		if(!$p[start])$p[start]=0;
		if(!$p[limit])$p[limit]=100;
		
		$query = mysql_query("select no,`order`,thumbnail,title,point,ticket from ".DBManager::get()->getMT("book")." where isEvent=1 order by `order` asc limit $p[start],$p[limit]",$mainConn);
		
		$list=array();
		$cnt=0;
		while($pData = mysql_fetch_array($query,MYSQL_ASSOC)){
			//퍼즐 시작stage 알아오기
			$r1 = mysql_query("select no from ".DBManager::get()->getMT("stage")." where book=".$pData[no]." order by no asc limit 1",$mainConn);
			if($r1){
				$r1d = mysql_fetch_array($r1,MYSQL_ASSOC);
				$pData["startStage"]=$r1d[no];
			}
			
			//퍼즐 안에 스테이지 갯수 알아오기
			$r2 = mysql_query("select count(no) from ".DBManager::get()->getMT("stage")." where book=".$pData[no],$mainConn);
			if($r2){
				$r2d = mysql_fetch_array($r2);
				$pData["stageCount"]=$r2d[0];
			}
			
			$pData[thumbnail]=json_decode($pData[thumbnail],true);
			//$pData[pathInfo]=json_decode($pData[pathInfo],true);
			$pData[thumbnail][image]=$nowurl."/images/".$pData[thumbnail][image];

			// $pData[center] = json_decode($pData[center],true);
			// $pData[center][image]=$nowurl."/images/".$pData[center][image];
			// $pData[original] = json_decode($pData[original],true);
			// $pData[original][image]=$nowurl."/images/".$pData[original][image];
			// $pData[face] = json_decode($pData[face],true);
			// $pData[face][image]=$nowurl."/images/".$pData[face][image];
			$list[]=$pData;
			$cnt++;
		}
		
		$puzzleInfo["list"]=$list;
	}
	

	$puzzleInfo["version"]=$puzzleListVer;

	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
	
}

/////////////////////////////////////////////
function help_getbooklist(){

	$r["description"] = "퍼즐 정보 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"책정보목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"얻어올 시작 책번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getbooklist($p){
	global $nowurl;
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$puzzleListVer = kvManager::get("bookListVer",1);

	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
	}else{
		// 여기서 eventstagelistversion 도 비교
		if(!$p[start])$p[start]=0;
		if(!$p[limit])$p[limit]=100;
		
		$query = mysql_query("select no,`order`,title,point,ticket,thumbnail,pathInfo,cardInfo,rewardInfo,levelInfo,conditionInfo,coordinateInfo,startPosition,endPosition,map,center,original,face from ".DBManager::get()->getMT("book")." where isEvent=0 order by `order` asc limit $p[start],$p[limit]",$mainConn);
		
		$list=array();
		$cnt=0;
		while($pData = mysql_fetch_array($query,MYSQL_ASSOC)){
			//퍼즐 시작stage 알아오기
			$r1 = mysql_query("select no from ".DBManager::get()->getMT("stage")." where book=".$pData[no]." order by no asc limit 1",$mainConn);
			if($r1){
				$r1d = mysql_fetch_array($r1,MYSQL_ASSOC);
				$pData["startStage"]=$r1d[no];
			}
			
			//퍼즐 안에 스테이지 갯수 알아오기
			$r2 = mysql_query("select count(no) from ".DBManager::get()->getMT("stage")." where book=".$pData[no],$mainConn);
			if($r2){
				$r2d = mysql_fetch_array($r2);
				$pData["stageCount"]=$r2d[0];
			}
			
			//$pData[version]=kvManager::get("puzzleVer_".$pData[no],1);
			$pData[thumbnail]=json_decode($pData[thumbnail],true);
			$pData[thumbnail][image]=$nowurl."/images/".$pData[thumbnail][image];
			$pData[pathInfo]=json_decode($pData[pathInfo],true);
			$pData[cardInfo]=json_decode($pData[cardInfo],true);
			$pData[rewardInfo]=json_decode($pData[rewardInfo],true);
			$pData[levelInfo]=json_decode($pData[levelInfo],true);
			$pData[conditionInfo]=json_decode($pData[conditionInfo],true);
			$pData[coordinateInfo]=json_decode($pData[coordinateInfo],true);
			$pData[startPosition]=json_decode($pData[startPosition],true);
			$pData[endPosition]=json_decode($pData[endPosition],true);

			$pData[center] = json_decode($pData[center],true);
			$pData[center][image]=$nowurl."/images/".$pData[center][image];
			$pData[original] = json_decode($pData[original],true);
			$pData[original][image]=$nowurl."/images/".$pData[original][image];
			$pData[face] = json_decode($pData[face],true);
			$pData[face][image]=$nowurl."/images/".$pData[face][image];
			$pData[map] = json_decode($pData[map],true);
			$pData[map][image]=$nowurl."/images/".$pData[map][image];
			$list[]=$pData;
			$cnt++;
		}
		
		$puzzleInfo["list"]=$list;
		//$puzzleInfo["puzzlelist"]=$list;
	}
	

	$puzzleInfo["version"]=$puzzleListVer;
	$puzzleInfo["booklistversion"]=$puzzleListVer;
	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
	
}

/////////////////////////////////////////////






//////////////////////////////////////////////
function help_getpathinfo(){

	$r["description"] = "퍼즐 path 정보 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"puzzlelistversion","type"=>"int","description"=>"퍼즐정보목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"얻어올 시작 퍼즐번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getpathinfo($p){
	global $nowurl;
	global $ERRORCODE;
	
	
	$mainConn = DBManager::get()->getMainConnection();
	if(!$mainConn) return ResultState::makeReturn(1002,"fail to get mainConnection");
	
	$puzzleVer = kvManager::get("puzzleListVer",1);

	if($puzzleVer==$p[puzzlelistversion]){
		$r[puzzlelistversion]=$p[puzzlelistversion];
	}else{
		// 여기서 eventstagelistversion 도 비교
		if(!$p[start])$p[start]=0;
		if(!$p[limit])$p[limit]=100;
		
		$query = mysql_query("select no,`order` from ".DBManager::get()->getMT("puzzle")." where isEvent=0 order by `order` asc limit $p[start],$p[limit]",$mainConn);
		
		$list=array();
		$nData=array();
		// $cnt=0;
		while($pData = mysql_fetch_array($query,MYSQL_ASSOC)){
			//퍼즐 시작stage 알아오기
			$pathList = array();
			$startStage = 0;
			$r1 = mysql_query("select no,pieceNo from ".DBManager::get()->getMT("stage")." where puzzle=".$pData[no]." order by no asc",$mainConn);
			while($sData=mysql_fetch_array($r1)){
				if($startStage==0)$startStage=$sData["no"];
				$pathList[] = $sData["pieceNo"];
			}
			$puzzleData["path"]=$pathList;
			$puzzleData["puzzleOrder"]=$pData["order"];
			$puzzleData["puzzleNo"]=$pData["no"];
			$puzzleData["stageStart"]=$startStage;

			
			$nData[]=$puzzleData;
			
			// $pData[version]=kvManager::get("puzzleVer_".$pData[no],1);
			// $pData[thumbnail]=json_decode($pData[thumbnail],true);
			// $pData[thumbnail][image]=$nowurl."/images/".$pData[thumbnail][image];
			// $pData[no]=$pData[puzzleNo];
			// $list[]=$pData;
			// $cnt++;
		}
		
		$puzzleInfo["puzzlelist"]=$nData;
	}

	$puzzleInfo["pathlistversion"]=$puzzleVer;

	$puzzleInfo["state"]="ok";
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
	
}


/////////////////////////////////////////////
function help_geteventstagelist(){

	$r["description"] = "이벤트스테이지 목록을 받아옵니다.";
	
	$r["param"][] = array("name"=>"puzzlelistversion","type"=>"int","description"=>"퍼즐정보목록버전");
	$r["param"][] = array("name"=>"eventstagelistversion","type"=>"int","description"=>"이벤트스테이지목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"얻어올 시작 퍼즐번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같으면 정보를 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function geteventstagelist($p){
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
	
	$query = mysql_query("select no,thumbnail from ".DBManager::get()->getMT("eventstage"),$mainConn);
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



//////////////////////////////////////////

function help_writelog(){

	$r["description"] = "로그를 작성합니다.";
	
	$r["param"][] = array("name"=>"memberID","type"=>"int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"input (exname is content)","type"=>"string","description"=>"내용");
	$r["param"][] = array("name"=>"output","type"=>"string","description"=>"결과");
	$r["param"][] = array("name"=>"category","type"=>"string","description"=>"카테고리");
	
	$r["result"][]=ResultState::toArray(1001,"로그작성실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


function writelog($p){
	
	if(!$p["input"])$p["input"]=$p["content"];
	if(!$p["memberID"])$p["memberID"]=1;
	$userlog = new UserLog($p["memberID"]);
	$userlog->memberID = $p["memberID"];
	$userlog->output = $p["output"];
	$userlog->input = $p["input"];
	$userlog->regDate = TimeManager::get()->getCurrentDateString();
	$userlog->regTime = TimeManager::get()->getTime();
	$userlog->category = $p["category"];
	$userlog->ip = get_client_ip();
	$userlog->header = json_encode(getallheaders());
	$userlog->execTime = $p["execTime"];

	if(!$userlog->save()){
		return ResultState::makeReturn(1001,"fail to write");
	}

	$result["state"]="ok";
	$result["result"]=ResultState::successToArray();

	return $result;


	
	// return $result;
}
/////////////////////////////////////////

function help_increasestats(){

	$r["description"] = "숫자통계 증가";
	
	$r["param"][] = array("name"=>"key","type"=>"string","description"=>"항목 키 값");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"증가실패");
	
	return $r;
}

function increasestats($p){
	$r=array();
	if(kvManager::increase($p["key"])){
		$r["result"]=ResultState::successToArray();	
	}else{
		$r["result"]=ResultState::toArray("1001");
	}
	
	$r["state"]="ok";
	return $r;
}

///////////////////////////////////////////
function help_dropoutuser(){

	$r["description"] = "회원탈퇴";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"탈퇴할 회원 아이디");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"데이터베이스로인한 삭제오류");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2003,"찾을수없음");
	return $r;
}

function dropoutuser($p){
	$memberid = $p["memberID"];
	$keylist = $p["keyList"];
	if($memberid){
		$user = new UserData($memberid);

		
		if($user->isLoaded()){
			mysql_query("delete from ".DBManager::getMT("userindex")." where memberID=".$user->m_memberID,DBManager::get()->getMainConnection());
			mysql_query("delete from ".DBManager::getST("message")." where memberID=".$user->m_memberID." and isSendMsg=0",$user->getDBConnection());
			StageScore::removeRowByQuery("where memberID=".$user->m_memberID);
			WeeklyScore::removeRowByQuery("where memberID=".$user->m_memberID);
			PieceHistory::removeRowByQuery("where memberID=".$user->m_memberID);
			CardHistory::removeRowByQuery("where memberID=".$user->m_memberID);
			PuzzleHistory::removeRowByQuery("where memberID=".$user->m_memberID);
			ArchivementHistory::removeRowByQuery("where memberID=".$user->m_memberID);
			CharacterHistory::removeRowByQuery("where memberID=".$user->m_memberID);
			UserProperty::removeRowByQuery("where memberID=".$user->m_memberID);
			if($user->remove()){
				$r["result"]=ResultState::successToArray();
			}else{
				$r["result"]=ResultState::toArray(1001,"query fail");
			}
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


function help_login(){

	$r["description"] = "로그인";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2007,"가입필요");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	return $r;
}

function login($p){
	$memberid = $p["memberID"];
	if($memberid){
		$user = new UserData($memberid);
		
		if(!$user->isLoaded())return ResultState::makeReturn(2007);

		$user->lastDate = TimeManager::get()->getCurrentDateString();
		$user->lastTime = TimeManager::get()->getTime();
		
		$newID=rand(1,100000);
		while($user->deviceID==$newID){
			$newID = rand(1,100000);
		}
		$user->deviceID=$newID;
		$user->lastCmdNo=0;
		$user->save();

		$r["data"] = $user->getArrayData(true);
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
	}
	
	return $r;
}


function help_join(){

	$r["description"] = "가입";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2008,"닉네임중복");
	$r["result"][]=ResultState::toArray(2009,"불량닉네임");
	$r["result"][]=ResultState::toArray(2010,"이미회원임");
	$r["result"][]=ResultState::toArray(2011,"너무긴닉네임");
	$r["result"][]=ResultState::toArray(2012,"너무짧은닉네임");
	$r["result"][]=ResultState::toArray(2002,"memberID or nick");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	return $r;
}

function join($p){
	$memberID = $p["memberID"];
	$nick = $p["nick"];

	if(!$memberID)return ResultState::makeReturn(2002);
	if(!$nick)return ResultState::makeReturn(2002);

	$user = new UserData($memberID);
	$user->memberID = $memberID;

    LogManager::get()->addLog("join1 userindex is ".json_encode($user->m__userIndex->getArrayData(true)));
	if(mb_strlen($nick,'UTF-8')>8)return ResultState::makeReturn(2011); 
	if(mb_strlen($nick,'UTF-8')<3)return ResultState::makeReturn(2012); 
	
	//이미 가입한유저
	if($user->isLoaded())return ResultState::makeReturn(2010);


	//닉네임중복검사
	while($rData = UserIndex::getRowByQuery("where nick='".addslashes($nick)."'")){
		if($rData){
			return ResultState::makeReturn(2008);
		}
	}

	//불량닉네임검사
	while($rData = FaultyNick::getRowByQuery("where (nick like '%".addslashes($nick)."%' and isInclusionRule=1) or (nick = '".addslashes($nick)."' and isInclusionRule=0)")){
		if($rData){
			return ResultState::makeReturn(2009);
		}
	}

	//deviceID발급
	$newID=rand(1,100000);
	while($user->deviceID==$newID){
		$newID = rand(1,100000);
	}
	$user->deviceID=$newID;
	$user->lastCmdNo=0;

    LogManager::get()->addLog("join user DeviceID s1 ".$user->deviceID);

	$user->m__userIndex->nick = addslashes($p["nick"]);

    LogManager::get()->addLog("join2 userindex is ".json_encode($user->m__userIndex->getArrayData(true)));

	if($user->m__userIndex->save()){
		if($user->save()){
			$r["data"] = $user->getArrayData(true);
			$r["result"]=ResultState::successToArray();

			CommitManager::get()->begin($memberID);
			

            LogManager::get()->addLog("join user DeviceID s2 ".$user->deviceID);
			
			$itemType = array("fr","g","m","pr","h","i9","i6","i8","p1","p2","p3","p4");
			$itemValue = array(10,5000,0,0,5,1,1,1,1,1,1,1);

			for($i=0;$i<count($itemType);$i++){
				$userProp = new UserProperty($memberID,$itemType[$i]);
				$userProp->memberID = $memberID;
				$userProp->type = $itemType[$i];
				$userProp->count = $itemValue[$i];
				CommitManager::get()->setSuccess($memberID,$userProp->save());
			}

			$character = new CharacterHistory($memberID,1);
			$character->memberID=$memberID;
			$character->characterNo = 1;
			CommitManager::get()->setSuccess($memberID,$character->save());

			if(CommitManager::get()->commit($memberID)){
				return $r;
			}else{
				return ResultState::makeReturn(2014);
			}
		}
	}



	return ResultState::makeReturn(1001);
}


function help_setuserdata(){

	$r["description"] = "유저데이터 저장";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"저장할데이터 json string");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	return $r;
}

function setuserdata($p){
	$memberid = $p["memberID"];
	if($memberid){
		$user = new UserData($memberid);
		
		if($p["nick"]){
			$user->m_nick = $p["nick"];
		}
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

	$r["description"] = "유저데이터 로드";
	
	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"멤머ID");
	$r["param"][] = array("name"=>"userIndex","type"=>"int or string","description"=>"유저인덱스");
	$r["param"][] = array("name"=>"keyList","type"=>"array(string)","description"=>"받아올키목록, 없으면 모두 받아옴");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2003,"정보로드실패");
	
	return $r;
}

function getuserdata($p){
	$memberid = $p["memberID"];
	$userindex = $p["userIndex"];
	$keylist = $p["keyList"];
	if($memberid){
		$user = new UserData($memberid);
		if($user->isLoaded()){
			$r = $user->getArrayData(true,$keylist);
			$r["state"]="ok";
			$r["userIndex"]=$user->getUserIndex();
			$r["result"]=ResultState::successToArray();
			$r["result"]["nick"]=$user->m__userIndex->nick;
		}else{
			$r["state"]="error";
			$r["result"]=ResultState::toArray(2003,"fail to load userdata");
		}
	}else if($userindex){
		$uIndex = UserIndex::create(0,$userindex);
		if($uIndex->isLoaded()){
			$user = new UserData($uIndex->m_memberID);
			if($user->isLoaded()){
				$r = $user->getArrayData(true,$keylist);
				$r["state"]="ok";
				$r["userIndex"]=$user->getUserIndex();
				$r["result"]=ResultState::successToArray();
			}else{
				$r["state"]="error";
				$r["result"]=ResultState::toArray(2003,"fail to load userdata");
			}
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

	$r["description"] = "유저데이터목록 로드";
	
	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string)","description"=>"멤머ID목록");
	$r["param"][] = array("name"=>"keyList","type"=>"array(string)","description"=>"받아올키목록, 없으면 모두 받아옴");
	
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

	$r["description"] = "유저데이터 업데이트";

	$r["param"][] = array("name"=>"memberID","type"=>"string","description"=>"멤버ID");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"저장할데이터 json string");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID 안넣음");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	
	return $r;
}


function updateuserdata($p){
	$memberid = $p["memberID"];
	$r=array();
	if($memberid){
		$user = new UserData($memberid);
		LogManager::get()->addLog("updateuserdata for ".$memberid);
		if($user->isLoaded()){
			LogManager::get()->addLog("updateuserdata load ok ".json_encode($user->getArrayData()));
		}
		if($p["nick"]){
			$user->m_nick = $p["nick"];
		}
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

	$r["description"] = "유저데이터 키 더하기";

	$r["param"][] = array("name"=>"memberID","type"=>"string","description"=>"멤버ID");
	$r["param"][] = array("name"=>"key","type"=>"string","description"=>"올릴키");
	$r["param"][] = array("name"=>"value","type"=>"int","description"=>"값");
	
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


function help_updatenick(){

	$r["description"] = "유저닉네임 업데이트";

	$r["param"][] = array("name"=>"memberID","type"=>"string","description"=>"멤버ID");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	
	return $r;
}


function updatenick($p){
	$memberid = $p["memberID"];
	$nick = $p["nick"];

	$user = new UserData($memberid);

	if(!$memberid){
		$r["state"]="error";
		$r["errorCode"]=2002;
		$r["result"]=ResultState::toArray(2002,"membeID");
		return $r;
	}
	if(!$nick){
		$r["state"]="error";
		$r["errorCode"]=2002;
		$r["result"]=ResultState::toArray(2002,"nick");
		return $r;
	}

	//$user->m__userIndex->m_nick = $nick;
	
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();

	return $r;
}


function help_addfriend(){

	$r["description"] = "친구추가";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","description"=>"추가할 친구 ID");
	$r["param"][] = array("name"=>"friendMax","type"=>"int","description"=>"추가가능한 친구 인원(미입력시 500,최대 500)");
	
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

	$r["description"] = "서로친구추가";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","description"=>"추가할 친구 ID");
	$r["param"][] = array("name"=>"friendMax","type"=>"int","description"=>"추가가능한 친구 인원");
	
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

	$r["description"] = "서로친구삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","description"=>"삭제할 친구 ID");
	
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

	$r["description"] = "친구삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","description"=>"삭제할 친구 ID");
	
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

	$r["description"] = "친구목록받아오기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID");
	
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

	$r["description"] = "추천친구목록";

	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"갯수, (기본 10, 최대 50)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"fail to load list");
	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
	return $r;
}


function getuserlistbyrandom($p){
	
	$limit = 10;
	if(is_numeric($p["limit"]))$limit = $p["limit"];
	
	if($limit>50)$limit=50;
	
	$shardConn = UserIndex::getShardConnectionByRandom();
	
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

	$r["description"] = "메세지보내기";

	$r["param"][] = array("name"=>"receiverMemberID","type"=>"string or int","description"=>"받는사람아이디");
	$r["param"][] = array("name"=>"senderMemberID","type"=>"string or int","description"=>"보내는사람아이디");
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"내용");
	$r["param"][] = array("name"=>"type","type"=>"int","description"=>"메세지타입");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


function sendmessage($p){	
	
	//보내기
	$message = new Message();
	$message->m_memberID=$p["receiverMemberID"];
	$message->m_content=$p["content"];
	$message->m_regDate=TimeManager::get()->getCurrentDateString();
	$message->m_regTime=TimeManager::get()->getTime();
	$message->m_friendID=$p["senderMemberID"];
	$message->m_type=$p["type"];
	$message->m_isSendMsg=0;
	$r["send"]=$message->send();
	
	
	//보낸편지로 등록
	// $message2 = new Message();
	// $message2->m_memberID=$p["senderMemberID"];
	// $message2->m_content=$p["content"];
	// $message2->m_regDate=TimeManager::get()->getCurrentDateString();
	// $message2->m_friendID=$p["receiverMemberID"];
	// $message2->m_type=$p["type"];
	// $message2->m_isSendMsg=1;
	// $r["receive"]=$message2->send();

	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_sendmessagebylist(){

	$r["description"] = "메세지 여러사람에게 보내기";

	$r["param"][] = array("name"=>"receiverMemberIDList","type"=>"array(string or int)","description"=>"받는사람아이디목록");
	$r["param"][] = array("name"=>"senderMemberID","type"=>"string or int","description"=>"보내는사람아이디");
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"내용");
	$r["param"][] = array("name"=>"type","type"=>"int","description"=>"메세지타입");
	
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
		$message->m_regTime=TimeManager::get()->getTime();
		$message->m_friendID=$p["senderMemberID"];
		$message->m_type=$p["type"];
		$message->m_isSendMsg=0;
		$r["send"][]=$message->send();
		
		//보낸편지로 등록
		// $message2 = new Message();
		// $message2->m_memberID=$p["senderMemberID"];
		// $message2->m_content=$p["content"];
		// $message2->m_regDate=TimeManager::get()->getCurrentDateString();
		// $message2->m_friendID=$rmemberid;
		// $message2->m_type=$p["type"];
		// $message2->m_isSendMsg=1;
		// $r["receive"][]=$message2->send();
	}

	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	
	return $r;
}

function help_getmessagelist(){

	$r["description"] = "메세지목록 받기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"type","type"=>"int","description"=>"메세지타입, (0혹은 미입력시 모두받아옴)");
	$r["param"][] = array("name"=>"keyList","type"=>"array","description"=>"data필드내에 받아올 필드목록(미입력시 전체)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
	return $r;
}


function getmessagelist($p){
	$whereType = "";
	$keyList=NULL;

	if($p["keyList"])$keyList=$p["keyList"];
	if(!$p["memberID"]){
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
		return $r;
	}

	$userIndex = UserIndex::create($p["memberID"]);
	$shardConn = $userIndex->getShardConnection();	

	if(!$shardConn)return ResultState::makeReturn(1002,"fail to get shardConnection");
	
	if($p["type"])$whereType=" and type='".$p["type"]."'";
	$result = mysql_query("select * from MessageTable where memberID=".$p["memberID"].$whereType." and isSendMsg=0 order by no desc",$shardConn);
	
	$mlist=array();
	while($message = mysql_fetch_array($result,MYSQL_ASSOC)){

		if($message["data"] && $keyList){
			$userdata =  json_decode($message["data"],true);

			foreach($userdata as $key=>$value){
				if($keyList && !in_array($name,$keyList)){
					unset($message[$key]);
				}
			}

			$message["data"]=json_encode($userdata,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		}	

		$mlist[]=$message;
	}
	$r["list"]=$mlist;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_removemessage(){

	$r["description"] = "메세지삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"메세지번호");
	
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
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}
	return $r;
}

function help_removemessagebylist(){

	$r["description"] = "메세지 여러개 삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"noList","type"=>"array(int)","description"=>"메세지번호목록");
	
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
	
	$userIndex = UserIndex::create($p["memberID"]);
	$shardConn = $userIndex->getShardConnection();	
	if(!$shardConn)return ResultState::makeReturn(1002,"fail to get shardConnection");
	
	$messageNos = implode(",", $p["noList"]);
	$whereNo = " and no IN (".$messageNos.")";
	$result = mysql_query("delete from MessageTable where memberID=".$p["memberID"].$whereNo." and isSendMsg=0",$shardConn);
	
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_removeallmessage(){

	$r["description"] = "메세지 전부삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"type","type"=>"int","description"=>"메세지타입(0혹은 입력안할시 모두삭제)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
	return $r;
}

function removeallmessage($p){
	if($p["type"] && $p["type"]>0)$whereType=" and type='".$p["type"]."'";
	
	$userIndex = UserIndex::create($p["memberID"]);
	$shardConn = $userIndex->getShardConnection();	
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

	$r["description"] = "주간점수등록";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"점수");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function setweeklyscore($p){

	$ws =new WeeklyScore($p["memberID"],TimeManager::get()->getCurrentWeekNo());
	$ws->m_memberID = $p["memberID"];
	$ws->m_data = $p["data"];
	$ws->m_regDate = TimeManager::get()->getCurrentDateString();
	$ws->m_regTime = TimeManager::get()->getTime();
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


function help_addweeklyscore(){

	$r["description"] = "주간점수누적하기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"점수");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function addweeklyscore($p){

	$ws =new WeeklyScore($p["memberID"],TimeManager::get()->getCurrentWeekNo());
	$ws->memberID = $p["memberID"];
	$ws->data = $p["data"];
	$ws->regDate = TimeManager::get()->getCurrentDateString();
	$ws->regTime = TimeManager::get()->getTime();
	$ws->regWeek = TimeManager::get()->getCurrentWeekNo();
	$ws->score = $ws->m_score + $p["score"];
	$ws->count = $ws->m_count+1;
	$r["update"]=false;
	$r["uresult"]=$ws->save();
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_getweeklyscorelist(){

	$r["description"] = "주간점수목록";

	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string or int)","description"=>"멤버아이디목록");
	$r["param"][] = array("name"=>"weekNo","type"=>"int","description"=>"주간번호 없을경우 이번주");
	
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
			$data = $ws->getArrayData(true);
			$list[]=$data;
		}
	}
	$r["remainTime"]=strtotime("next Sunday")-time();
	$r["list"]=$list;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_getweeklyrankbyalluser(){

	$r["description"] = "주간점수목록(같은서버유저전체)";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"내아이디");
	$r["param"][] = array("name"=>"weekNo","type"=>"int","description"=>"주간번호, 없을경우 이번주");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"시작등수(기본값 1)");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작등수로 부터 아래로 몇까지?(기본 10, 최대 50)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function getweeklyrankbyalluser($p){

	////////////////////////////////////////////////////
	$memberID = $p["memberID"];
	$start = $p["start"];
	$limit = $p["limit"];
	$weekNo = TimeManager::get()->getCurrentWeekNo();
	if($p["weekNo"])$weekNo=$p["weekNo"];
	if(!$start)$start=0;
	if(!$limit)$limit=10;


	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$myRank = new WeeklyScore($memberID,$weekNo);
	$myRank->memberID=$memberID;
	$myRank->regWeek=$weekNo;

	$r["list"]=$myRank->getTopRank($start,$limit);
	$r["myscore"]=$myRank->score;
	$r["alluser"]=$myRank->getAllUser();
	$r["myrank"]=$myRank->getMyRank();
	$r["remainTime"]=TimeManager::get()->getRemainTimeForWeeklyRank();
	if($r["myrank"]<=0){
		$r["alluser"]+=1;
		$r["myrank"]=$r["alluser"];
	}
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();


	// $userIndex = UserIndex::create($memberID);

	// $mresult = mysql_query("select * from ".DBManager::getST("weeklyscore")." where regWeek=$weekNo and memberID='$memberID'",$userIndex->getShardConnection());
	// $myInfo=array();
	// if($mresult){
	// 	$myInfo = mysql_fetch_array($mresult,MYSQL_ASSOC);
	// 	$myInfo["nick"]=$myInfo["memberID"];
	// }else{
	// 	$myInfo = array("nick"=>"none","score"=>0);
	// }
	// $myscore = $myInfo["score"];
	// if(!$myscore)$myscore=0;

	// $mresult = mysql_query("select count(*) from ".DBManager::getST("weeklyscore")." where regWeek=$weekNo",$userIndex->getShardConnection());

	// if($mresult)$allusercnt = mysql_fetch_array($mresult);
	// else $allusercnt[0]=0;
	// $alluser=$allusercnt[0];
	// $r["alluser"]=$alluser;

	// if($alluser==1){
	// 	$r["list"]=$list;
	// 	$r["state"]="ok";
	// 	$r["result"]=ResultState::successToArray();
	// 	return $r;
	// }

	// if($myscore){
	// 	$mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("weeklyscore")." where regWeek=$weekNo and score>$myscore order by score desc",$userIndex->getShardConnection()));
	// 	$myrank=$mresult[0]+1;
	// 	$myInfo["rank"]=$myrank;
	// 	$r["myrank"]=$myrank;
	// }else{
	// 	$myInfo["myrank"]= $myrank=$alluser+1;
	// }
	
	// $list=array();
	// $bronzeScore=0;
	// $_r=1;
	// $qresult = mysql_query("select * from ".DBManager::getST("weeklyscore")." where regWeek=$weekNo and score>$myscore order by score desc limit 3",$userIndex->getShardConnection());
	

	// while($rankInfo = mysql_fetch_array($qresult,MYSQL_ASSOC)){
	// 	$rankInfo["nick"]=$rankInfo["memberID"];
	// 	$rankInfo["rank"]=$_r++;
	// 	$list[]=$rankInfo;
	// 	$bronzeScore = $rankInfo["score"];
	// }	




	// if($myrank>3){
	// 	$_r=1;
	// 	$list_front=array();
	// 	$limit = 3;
	// 	if($myrank<6)$limit = $myrank-3;
	// 	$qresult = mysql_query("select * from ".DBManager::getST("weeklyscore")." where regWeek=$weekNo and score<$bronzeScore and score>$myscore order by score asc limit $limit",$userIndex->getShardConnection());
	// 	while($rankInfo = mysql_fetch_array($qresult,MYSQL_ASSOC)){
	// 		$rankInfo["nick"]=$rankInfo["memberID"];
	// 		$rankInfo["rank"]=$myrank-$_r++;
	// 		$list_front[]=$rankInfo;
	// 	}	
		
	// 	if($list_front){
	// 		$list_front=array_reverse($list_front);
	// 		$list = array_merge($list,$list_front);
	// 	}
	// }
	// //여기에 내점수 넣기
	// $list[]=$myInfo;

	// $_r=1;
	// $limit = 3;
	// if($myrank<10)$limit = 10 - $myrank;
	// $list_back=array();
	// $qresult = mysql_query("select * from ".DBManager::getST("weeklyscore")." where regWeek=$weekNo and score<$myscore order by score desc limit $limit",$userIndex->getShardConnection());
	// while($rankInfo = mysql_fetch_array($qresult,MYSQL_ASSOC)){
	// 	$rankInfo["nick"]=$rankInfo["memberID"];
	// 	$rankInfo["rank"]=$myrank+$_r++;
	// 	$list_back[]=$rankInfo;
	// }	
	
	// if($list_back)$list = array_merge($list,$list_back);

	return $r;
}
////////////////////////////////////////////////////////////////////////////////////

// function help_getweeklyrankbyalluser(){

// 	$r["description"] = "주간점수목록(같은서버유저전체)";

// 	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"내아이디");
// 	$r["param"][] = array("name"=>"weekNo","type"=>"int","description"=>"주간번호, 없을경우 이번주");
// 	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"시작등수(기본값 1)");
// 	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작등수로 부터 아래로 몇까지?(기본 10, 최대 50)");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
	
// 	return $r;
// }

// function getweeklyrankbyalluser($p){

// 	////////////////////////////////////////////////////
// 	$memberID = $p["memberID"];
// 	$userIndex = UserIndex::create($memberID);
// 	$weekNo=$p["weekNo"];
// 	if($p["weekNo"])$weekNo = TimeManager::get()->getCurrentWeekNo();
	


// 	$r["list"]=$list;
// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
// 	return $r;
// }
////////////////////////////////////////////////////////////////////////////////////
function help_setstagescore(){

	$r["description"] = "스테이지점수등록";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","description"=>"스테이지번호");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"점수");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

function setstagescore($p){

	$stageNo = $p["stageNo"];
	$score=0;
	if(is_numeric($p["score"]))$score = $p["score"];
	$ss = new StageScore($stageNo,$p["memberID"]);
	$ss->m_memberID = $p["memberID"];
	$ss->m_stageNo = $stageNo;
	$ss->m_data = $p["data"];
	$ss->m_regDate = TimeManager::get()->getCurrentDateString();
	$ss->m_regTime = TimeManager::get()->getTime();

	$r["update"]=false;
	if($ss->m_score<=$score){
		$r["update"]=true;
		$ss->m_score=$score;
		$ss->save();
	}
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

function help_addstagescore(){

	$r["description"] = "스테이지점수추가";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","description"=>"스테이지번호");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"점수");
	
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
	$ss->m_regTime = TimeManager::get()->getTime();
	$ss->m_score+=$p["score"];	
	$ss->save();
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}


function help_getstagescorelist(){

	$r["description"] = "스테이지점수목록";

	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string or int)","description"=>"멤버아이디목록");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","description"=>"스테이지번호");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"파라메터없음");
	
	return $r;
}

function getstagescorelist($p){
	$memberlist = $p["memberIDList"];
	$stageNo = $p["stageNo"];
	
	if(!$memberlist)return ResultState::makeReturn(2002,"memberlist");
	if(!$stageNo)return ResultState::makeReturn(2002,"stageNo");

	$memberIDListString = "(".implode(",",$memberlist).")";
	$list=array();

	$qresult = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=$stageNo and memberID IN $memberIDListString",DBManager::get()->getConnectionByShardKey($stageNo));
	while($rankInfo = mysql_fetch_array($qresult,MYSQL_ASSOC)){
		$list[]=$rankInfo;
	}	
	// foreach($memberlist as $key=>$value){
	// 	$ws=new StageScore($stageNo,$value);
	// 	if($ws->isLoaded()){
	// 		$list[]=$ws->getArrayData(true);
	// 	}
	// }
	
	$r["list"]=$list;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}


function help_getstageranklist(){

	$r["description"] = "스테이지별 랭킹목록";

	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string or int)","description"=>"멤버아이디목록");
	$r["param"][] = array("name"=>"stageNoList","type"=>"array(int)","description"=>"스테이지번호목록");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"몇등까지뽑아올것인가, 기본값 1, 최대값 10");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"파라메터없음");
	
	return $r;
}

function getstageranklist($p){
	$limit = $p["limit"];
	
	if(!$limit)$limit = 1;
	if($limit>10)$limit=10;

	$memberIDList = $p["memberIDList"];
	$stageNoList = $p["stageNoList"];

	if(!$memberIDList)return ResultState::makeReturn(2002,"memberIDList");
	if(!$stageNoList)return ResultState::makeReturn(2002,"stageNoList");
	if(!is_array($memberIDList))return ResultState::makeReturn(2002,"memberIDList");
	if(count($memberIDList)==1 && $memberIDList[0]=="") ResultState::makeReturn(2002,"memberIDList");

	$memberIDListString = "(".implode(",",$memberIDList).")";
	$list = array();
	foreach($stageNoList as $key=>$stageNo){
		$qresult = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=$stageNo and memberID IN $memberIDListString order by score desc limit $limit",DBManager::get()->getConnectionByShardKey($stageNo));
		
		$rlist = array();
		LogManager::get()->addLog("select * from ".DBManager::getST("stagescore")." where stageNo=$stageNo and memberID IN $memberIDListString order by score desc limit $limit");
		
		if($qresult){
			while($user = mysql_fetch_array($qresult,MYSQL_ASSOC)){
				$rlist[]=$user;
			}
			$list[]=$rlist;
		}
	}
	$r["list"]=$list;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();

	return $r;
}


//////////////////////////////////////////



function help_getstagerankbyalluser(){

	$r["description"] = "스테이지랭킹";

	$r["param"][] = array("name"=>"memberID","type"=>"int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","description"=>"스테이지번호");
	$r["param"][] = array("name"=>"myScore","type"=>"int","description"=>"0을 입력하면 기존 내점수, 최고점수보다 높으면 기록갱신");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"파라메터없음");
	
	return $r;
}

function getstagerankbyalluser($p){

	$stageNo = $p["stageNo"];
	$myscore = $p["myScore"];
	$memberID = $p["memberID"];
	
	if(!$stageNo)return ResultState::makeReturn(2002,"stageNo");


	$myRank = new StageScore($memberID,$stageNo);
	$myRank->memberID = $memberID;
	$myRank->stageNo = $stageNo;
	if($myscore){
		if($myRank->score<$myscore){
			$myRank->score = $myscore;
			$myRank->save();
		}

		$myRank->score = $myscore;
	}


	$r["list"] = $myRank->getTop4();
	$r["alluser"] = $myRank->getAllUser();

	$r["myscore"] = $myRank->score;
	$r["myrank"] = $myRank->getMyRank();
	$r["stageNo"] = $stageNo;
	
	if($r["myrank"]<=0 || $r["myrank"]>$r["alluser"])$r["myrank"]=$r["alluser"];

	$r["result"]=ResultState::successToArray();

	return $r;
	

	// StageScore::q

	// $mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("stagescore")." where stageNo=$stageNo",DBManager::get()->getConnectionByShardKey($stageNo)));
	// $alluser=$mresult[0]+1;
	// $r["alluser"]=$alluser+1;

		
	// $list=array();

	// $topquery = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=$stageNo order by score desc limit 4");

	
	// while($data = StageScore::getRowByQuery("where stageNo=$stageNo order by score desc limit 4")){
		
	// }


	// if($myscore){
	// 	$mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("stagescore")." where stageNo=$stageNo and score>$myscore order by score desc",DBManager::get()->getConnectionByShardKey($stageNo)));
	// 	$myrank=$mresult[0]+1;
	// 	$r["myrank"]=$myrank;
	// }else{
	// 	$r["myrank"]= $myrank=$alluser+1;
	// }
	


// 	$list=array();
// 	$bronzeScore=0;
// 	$_r=1;
// 	$qresult = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=$stageNo order by score desc limit 4",DBManager::get()->getConnectionByShardKey($stageNo));
// 	while($rankInfo = mysql_fetch_array($qresult,MYSQL_ASSOC)){
// 		$rankInfo["nick"]=$rankInfo["memberID"];
// 		$rankInfo["rank"]=$_r++;
// 		$list[]=$rankInfo;
// 		$bronzeScore = $rankInfo["score"];
// 	}	






// 	$r["list"]=$list;
// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
// 	return $r;



// ////////////
// 	$stageNo = $p["stageNo"];
// 	$myscore = $p["myScore"];
	
// 	if(!$stageNo)return ResultState::makeReturn(2002,"stageNo");

// 	$mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("stagescore")." where stageNo=$stageNo",DBManager::get()->getConnectionByShardKey($stageNo)));
// 	$alluser=$mresult[0]+1;
// 	$r["alluser"]=$alluser+1;

// 	if($myscore){
// 		$mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("stagescore")." where stageNo=$stageNo and score>$myscore order by score desc",DBManager::get()->getConnectionByShardKey($stageNo)));
// 		$myrank=$mresult[0]+1;
// 		$r["myrank"]=$myrank;
// 	}else{
// 		$r["myrank"]= $myrank=$alluser+1;
// 	}
	
// 	$list=array();
// 	$bronzeScore=0;
// 	$_r=1;
// 	$qresult = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=$stageNo and score>$myscore order by score desc limit 3",DBManager::get()->getConnectionByShardKey($stageNo));
// 	while($rankInfo = mysql_fetch_array($qresult,MYSQL_ASSOC)){
// 		$rankInfo["nick"]=$rankInfo["memberID"];
// 		$rankInfo["rank"]=$_r++;
// 		$list[]=$rankInfo;
// 		$bronzeScore = $rankInfo["score"];
// 	}	




// 	if($myrank>3){
// 		$_r=1;
// 		$list_front=array();
// 		$limit = 3;
// 		if($myrank<6)$limit = $myrank-3;
// 		$qresult = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=$stageNo and score<$bronzeScore and score>$myscore order by score asc limit $limit",DBManager::get()->getConnectionByShardKey($stageNo));
// 		while($rankInfo = mysql_fetch_array($qresult,MYSQL_ASSOC)){
// 			$rankInfo["nick"]=$rankInfo["memberID"];
// 			$rankInfo["rank"]=$myrank-$_r++;
// 			$list_front[]=$rankInfo;
// 		}	
		
// 		if($list_front){
// 			$list_front=array_reverse($list_front);
// 			$list = array_merge($list,$list_front);
// 		}
// 	}
// 	//여기에 내점수 넣기

// 	$_r=1;
// 	$limit = 3;
// 	if($myrank<10)$limit = 10 - $myrank;
// 	$list_back=array();
// 	$qresult = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=$stageNo and score<$myscore order by score desc limit $limit",DBManager::get()->getConnectionByShardKey($stageNo));
// 	while($rankInfo = mysql_fetch_array($qresult,MYSQL_ASSOC)){
// 		$rankInfo["nick"]=$rankInfo["memberID"];
// 		$rankInfo["rank"]=$myrank+$_r++;
// 		$list_back[]=$rankInfo;
// 	}	
	
// 	if($list_back)$list = array_merge($list,$list_back);

// 	$r["list"]=$list;
// 	$r["result"]=ResultState::successToArray();
// 	return $r;
}

/////////////////////////////////////////

function help_updatepuzzlehistory(){

	$r["description"] = "퍼즐히스토리를 남깁니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"puzzleNo","type"=>"int","description"=>"퍼즐번호(필수)");
	$r["param"][] = array("name"=>"updateOpenDate","type"=>"bool","description"=>"true 일 경우 오픈시간을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"updateClearDate","type"=>"bool","description"=>"true 일 경우 클리어시각을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"updatePerfectDate","type"=>"bool","description"=>"true 일 경우 퍼펙트클리어시각을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"openType","type"=>"string","description"=>"어떻게 오픈했는가(ex 무료, 이벤트, 구입-100루비)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or puzzleNo");
	
	return $r;
}


function updatepuzzlehistory($p){	
	
	$memberID=$p["memberID"];
	$puzzleNo=$p["puzzleNo"];
	
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$puzzleNo)return ResultState::makeReturn(2002,"puzzleNo");
	
	//보내기
	$obj = new PuzzleHistory($memberID,$puzzleNo);
	if($p["updateOpenDate"] && !$obj->openDate)$obj->openDate=TimeManager::get()->getCurrentDateString();
	if($p["updateClearDate"] && !$obj->clearDate)$obj->clearDate=TimeManager::get()->getCurrentDateString();
	if($p["updatePerfectDate"] && !$obj->perfectDate)$obj->perfectDate=TimeManager::get()->getCurrentDateString();
	if($p["openType"])$obj->openType=$p["openType"];

	if($obj->save()){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	return $r;
}


function help_getpuzzlehistory(){

	$r["description"] = "퍼즐히스토리를 가져옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	
	return $r;
}


function getpuzzlehistory($p){	
	
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
    while($rData = PuzzleHistory::getRowByQuery("where memberID='".$memberID."'")){
    	$dataList[]=$rData;
    }


	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;
}
/////////////////////////////////////////

function help_updatepiecehistory(){

	$r["description"] = "피스히스토리를 남깁니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"pieceNo","type"=>"int","description"=>"피스번호(필수)");
	$r["param"][] = array("name"=>"openDate","type"=>"bool or int","description"=>"0초과 혹은 true일경우 현재시각으로 오픈일자변경");
	$r["param"][] = array("name"=>"firstClearDate","type"=>"bool","description"=>"0초과 혹은 true일경우 현재시각으로 클리어일자변경");
	$r["param"][] = array("name"=>"tryCount","type"=>"int","description"=>"총 시도횟수변경");
	$r["param"][] = array("name"=>"clearCount","type"=>"int","description"=>"성공했을때까지의 시도횟수변경");
	$r["param"][] = array("name"=>"clearDateList","type"=>"array(1등급bool,2등급bool,3등급bool,4등급bool)","description"=>"등급클리어정보업데이트");
	$r["param"][] = array("name"=>"openType","type"=>"string","description"=>"어떻게 오픈했는가(ex 이전피스클리어 or 5루비구입");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or pieceNo");
	
	return $r;
}


function updatepiecehistory($p){	
	
	$memberID=$p["memberID"];
	$pieceNo=$p["pieceNo"];
	$clearRank = $p["updateClearDate"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$pieceNo)return ResultState::makeReturn(2002,"pieceNo");
	
	//보내기
	$obj = new PieceHistory($memberID,$pieceNo);
	
	$obj->memberID = $memberID;
	$obj->pieceNo = $pieceNo;
	if($p["openDate"] && !$obj->openDate)$obj->openDate = TimeManager::get()->getCurrentDateString();
	if($p["firstClearDate"] && !$obj->firstClearDate)$obj->firstClearDate = TimeManager::get()->getCurrentDateString();
	if($p["tryCount"])$obj->tryCount= $p["tryCount"];
	if($p["clearCount"] && !$obj->clearCount)$obj->clearCount = $p["clearCount"];
	if(is_array($p["clearDateList"])){
		
		$newlistdata = array(0,0,0,0);
		if(!$obj->clearDateList)$obj->clearDateList=array(0,0,0,0);
		else if(!is_array($obj->clearDateList))$obj->clearDateList = json_decode($obj->clearDateList,true);
		foreach ($obj->clearDateList as $key => $value) {
			if($value<=0 && ($p["clearDateList"][$key]>0)){
				$newlistdata[$key]=TimeManager::get()->getCurrentDateString();
			}else if($value){
				$newlistdata[$key]=$value;
			}
		}
	}
	$obj->clearDateList=$newlistdata;

	if($p["openType"])$obj->openType = $p["openType"];


	// if(!$p["updateOpenDate"])$obj->tryCount=$obj->m_tryCount+1;
	// else $obj->openDate=TimeManager::get()->getCurrentDateString();
	// if($clearRank>0){
	// 	$clearDateList = json_decode($obj->clearDateList,true);
	// 	if(!$clearDateList)$clearDateList=array(0,0,0,0);
	// 	$clearDateList[$clearRank-1] = TimeManager::get()->getCurrentDateString();
	// 	$obj->clearDateList=json_encode($clearDateList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	// }
	// if(!$obj->clearDateList)$obj->clearDateList=array(0,0,0,0);
	// if($p["updateFirstClearDate"] || ($clearRank>0 && !$obj->firstClearDate)){
	// 	$obj->firstClearDate=TimeManager::get()->getCurrentDateString();
	// 	$obj->clearCount=$obj->tryCount;
	// }
	// if($p["openType"])$obj->openType=$p["openType"];


	LogManager::get()->addLog("lastCheck".json_encode($obj->clearDateList));
	if($obj->save()){
		$r["data"]=$obj->getArrayData(true);
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014);
	}
	
	return $r;
}


function help_getpiecehistory(){

	$r["description"] = "피스히스토리를 가져옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	
	return $r;
}


function getpiecehistory($p){	
	
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
	//echo "fuck this??";
    while($rData = PieceHistory::getRowByQuery("where memberID='".$memberID."'")){

		//echo "fuck this2??";
    	$rData["clearDateList"]=json_decode($rData["clearDateList"],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		$dataList[]=$rData;
    }


	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;
}
////////////////////////////////////////


function help_updatecardhistory(){

	$r["description"] = "카드히스토리를 남깁니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"cardNo","type"=>"int","description"=>"카드번호(필수)");
	$r["param"][] = array("name"=>"comment","type"=>"string","description"=>"코멘트");
	$r["param"][] = array("name"=>"updateTakeDate","type"=>"bool","description"=>"true 일 경우 획득시각을 현재시각으로 설정합니다.");
	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or cardNo");
	$r["result"][]=ResultState::toArray(2014,"Dont save");
	
	return $r;
}


function updatecardhistory($p){	
	
	$memberID=$p["memberID"];
	$cardNo=$p["cardNo"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$cardNo)return ResultState::makeReturn(2002,"cardNo");
	
	//보내기
	$obj = new CardHistory($memberID,$cardNo);
	
	if($p["updateTakeDate"] || !$obj->isLoaded())$obj->takeDate=TimeManager::get()->getCurrentDateString();
	if($p["comment"])$obj->comment=addslashes($p["comment"]);

	LogManager::get()->addLog($obj->comment);
	LogManager::get()->addLog($obj->getArrayData(true));

	if(!$obj->save())return ResultState::makeReturn(2014,"dont save");


	$r["write"]="success";	
	$r["result"]=ResultState::successToArray();
	return $r;
}



function help_getcardhistory(){

	$r["description"] = "카드히스토리를 가져옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	
	return $r;
}


function getcardhistory($p){	
	
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
    while($rData = CardHistory::getRowByQuery("where memberID='".$memberID."'")){
    	$rData["comment"]=stripslashes($rData["comment"]);
		$dataList[]=$rData;
    }


	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;
}


/////////////////////////////////////////////////////////////////////////////////////////////


function help_addproperty(){

	$r["description"] = "보관함에 아이템/재화를 추가합니다. (서버관리자용입니다.)";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"count","type"=>"int","description"=>"변경량");
	$r["param"][] = array("name"=>"type","type"=>"string","description"=>"property type");	
	$r["param"][] = array("name"=>"eventID","type"=>"string","description"=>"eventID");	
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"변경내용");	
	$r["param"][] = array("name"=>"sender","type"=>"string","description"=>"user or 사번");	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}


function addproperty($p){		
	$memberID=$p["memberID"];
	$count=$p["count"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$count)return ResultState::makeReturn(2002,"count");
	if(!$p["type"])return ResultState::makeReturn(2002,"type");

	CommitManager::get()->begin($memberID);

	$userProp = new UserProperty($memberID,$p["type"]);
	
	if(!$userProp->m__userIndex->isLoaded())return ResultState::makeReturn(2005,"dont find user");

	$userHistory = new UserPropertyHistory($memberID);
	$userHistory->memberID = $memberID;
	$userHistory->count = $p["count"];
	$userHistory->type = $p["type"];
	$userHistory->count = $p["count"];
	$userHistory->statsID = $p["statsID"];
	$userHistory->statsValue = $p["statsValue"];
	$userHistory->total = $userProp->count+$p["count"];
	$userHistory->content = $p["content"];
	$userHistory->sender = $p["sender"];
	$userHistory->regDate = TimeManager::get()->getCurrentDateString();

	$userProp->count = $userProp->count + $count;
	$userProp->memberID=$memberID;
	$userProp->type=$p["type"];


	$r["data"]=$userProp->getArrayData(true);	

	if($userProp->count<0){
		$r["result"] = ResultState::toArray(2015);
		$r["minusType"]=$userProp->type;
		$r["minusCount"]=$userProp->count-$count;
		return $r;
	}

	if(!$userProp->save() or !$userHistory->save()){
		CommitManager::get()->rollback($memberID);
		$r["result"] = ResultState::toArray(2014,"Dont Save gold");
		return $r;
	}

	CommitManager::get()->commit($memberID);
	
	$r["result"]=ResultState::successToArray();
	return $r;
}


////////////////////////////////////////


function help_changeuserproperties(){

	$r["description"] = "보관함에 아이템/재화를 변경합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"list","type"=>"array({type,count,statsID,statsValue,content,isPurchase})","description"=>"변경할 소유물 목록");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	$r["result"][]=ResultState::toArray(2015,"property is minus");
	
	$r["comment"] = '
*type
m(실제돈)
g (골드)
r (루비)
h (하트)
i6 (스피드업)
i8 (아이템두배)
i9 (타임)
p1 (이어하기 이용권)
p2 (맵가챠 이용권)
p3 (업그레이드 이용권)
p4 (아이템뽑기 이용권)
p5 (99프로 이용권)

*statsID / statsValue
stage / 스테이지번호
event / 이벤트번호
archivement / 업적번호
shop / none

	';

	return $r;
}

function changeuserproperties($p){

	$list = $p["list"];
	$memberID=$p["memberID"];
	if(!$list)return ResultState::makeReturn(2002,"list");
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$r=array();
	$rs=array();

	$minusProperty = "";
	$minusPropertyValue = 0;
	CommitManager::get()->begin($memberID);

	foreach ($list as $key => $value) {
		$param=array();

		//루비인경우 특수처리
		if($value["type"]=="r"){
			//take
			if($value["count"]>=0){
				if($value["isPurchase"]){
					$value["type"]="pr";
				}else{
					$value["type"]="fr";
				}
				$param["memberID"]=$memberID;
				$param["type"]=$value["type"];
				$param["count"]=$value["count"];
				$param["statsID"]=$value["statsID"];
				$param["statsValue"]=$value["statsValue"];
				$param["content"]=$value["content"];
				$param["sender"]="user";

				$result = $this->addProperty($param);	
				if(!ResultState::successCheck($result["result"])){
					if($result["result"]["code"]==2015){
						$r["result"]=ResultState::toArray(2015);
					}
					CommitManager::get()->setSuccess($memberID,false);
				}

				$fRuby = new UserProperty($memberID,"fr");
				$pRuby = new UserProperty($memberID,"pr");
				$rs[]=array("type"=>"r","count"=>($fRuby->count+$pRuby->count));

			//use
			}else if($value["count"]<0){
				//fRuby와 pRuby읽어옴
				$fRuby = new UserProperty($memberID,"fr");
				$pRuby = new UserProperty($memberID,"pr");
				//fRuby먼저 깍고 fRuby모자를 경우 pRuby깍음. 근데 둘중하나라도 마이너스면 에러
				if($fRuby->count+$pRuby->count+$value["count"]<0){
					$r["result"]=ResultState::toArray(2015);
					$rs[]=array("type"=>"r","count"=>($fRuby->count+$pRuby->count));
					$minusProperty="r";
					$minusPropertyValue = $fRuby->count+$pRuby->count;
					CommitManager::get()->setSuccess($memberID,false);
				}else{
					//무료루비가 충분하다면 무료루비만 사용
					if($value["count"]+$fRuby->count>=0){
						$fRuby->count = $fRuby->count+$value["count"]; 

						$userHistory = new UserPropertyHistory($memberID);
						$userHistory->memberID = $memberID;
						$userHistory->count = $value["count"];
						$userHistory->type = "fr";
						$userHistory->statsID = $value["statsID"];
						$userHistory->statsValue = $value["statsValue"];
						$userHistory->total = $fRuby->count;
						$userHistory->content = $value["content"];
						$userHistory->sender = "user";
						$userHistory->regDate = TimeManager::get()->getCurrentDateString();
						CommitManager::get()->setSuccess($memberID,($userHistory->save() && $fRuby->save()));

					//무료루비가 부족하면 뮤료루비=0, 남은건 유료루비에서 차감
					}else{
						{
							$userHistory1 = new UserPropertyHistory($memberID);
							$userHistory1->memberID = $memberID;
							$userHistory1->count = $fRuby->count;
							$userHistory1->type = "fr";
							$userHistory1->statsID = $value["statsID"];
							$userHistory1->statsValue = $value["statsValue"];
							$userHistory1->total = 0;
							$userHistory1->content = $value["content"];
							$userHistory1->sender = "user";
							$userHistory1->regDate = TimeManager::get()->getCurrentDateString();
							CommitManager::get()->setSuccess($memberID,$userHistory1->save());
						}

						$pRuby->count=$pRuby->count+$fRuby->count+$value["count"];
						$fRuby->count=0;

						{
							$userHistory2 = new UserPropertyHistory($memberID);
							$userHistory2->memberID = $memberID;
							$userHistory2->count = $fRuby->count+$value["count"];
							$userHistory2->type = "pr";
							$userHistory2->statsID = $value["statsID"];
							$userHistory2->statsValue = $value["statsValue"];
							$userHistory2->total = $pRuby->count;
							$userHistory2->content = $value["content"];
							$userHistory2->sender = "user";
							$userHistory2->regDate = TimeManager::get()->getCurrentDateString();
							CommitManager::get()->setSuccess($memberID,$userHistory2->save());
						}

						CommitManager::get()->setSuccess($memberID,$fRuby->save() && $pRuby->save());
					}

					$rs[]=array("type"=>"r","count"=>($fRuby->count+$pRuby->count));
				}

				//아니면 적용

			}

		//다른재화
		}else{
			$param["memberID"]=$memberID;
			$param["type"]=$value["type"];
			$param["count"]=$value["count"];
			$param["statsID"]=$value["statsID"];
			$param["statsValue"]=$value["statsValue"];
			$param["content"]=$value["content"];
			$param["sender"]="user";

			$result = $this->addProperty($param);	
			if(!ResultState::successCheck($result["result"])){
				if($result["result"]["code"]==2015){
					$r["result"]=ResultState::toArray(2015);
					$minusProperty=$result["minusType"];
					$minusPropertyValue = $result["minusCount"];

				}
				CommitManager::get()->setSuccess($memberID,false);
				$rs[]=array("type"=>$result["data"]["type"],"count"=>$result["data"]["count"]);
			}else{
				$rs[]=array("type"=>$result["data"]["type"],"count"=>$result["data"]["count"]);
			}
		}
	}

	$r["list"]=$rs;

	if($minusProperty){$r["minusType"]=$minusProperty; $r["minusCount"]=$minusPropertyValue;}
	if(!CommitManager::get()->isSuccess($memberID)){
		CommitManager::get()->rollback($memberID);
		$r["result"]=ResultState::toArray(2013,"dont success");
		
	}else{
		if(CommitManager::get()->commit($memberID))$r["result"]=ResultState::successToArray();
		else ResultState::toArray(2013,"dont success");
	}

	if($minusProperty)
	{
		$r["result"]=ResultState::toArray(2015,"dont success");
		unset($r["list"]);
	}
	return $r;

	
}


////////////////////////////////////////////////////////////////////////////////////////////

function help_getuserproperties(){
	$r["description"] = "유저 보유 재화/아이템 목록을 불러옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}

function getuserproperties($p){

	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
	$checkRuby = false;
	$fRuby=array();
	$pRuby=array();
    while($rData = UserProperty::getRowByQuery("where memberID='".$memberID."'")){
    	unset($rData["memberID"]);
    	unset($rData["no"]);
    	if($rData["type"]=="fr"){
    		$fRuby = $rData;
    		continue;
    	}
    	if($rData["type"]=="pr"){
    		$pRuby = $rData;
    		continue;
    	}
		$dataList[]=$rData;
    }
    $pRuby["type"]="r";
    $pRuby["count"]=$pRuby["count"]+$fRuby["count"];
    $dataList[]=$pRuby;
	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;

}


/////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////
////////////////////////////////////////

function help_starttranjaction(){

	$r["description"] = "함께보낸 명령들에 대해 트랜잭션을 시작하고 결과를 리턴합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2013,"트랜잭션실패");
	
	return $r;
}


function starttranjaction($p){
	$memberID = $p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$p["actions"])return ResultState::makeReturn(2002,"actions");

	return true;
}

function help_tranjaction(){

	$r["description"] = "트랜잭션을 추가합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"actions","type"=>"array(dict(action(string),param(dict)))","description"=>'트랜잭션내 실행할 action // ex) [{"action":"updateGoldHistory","param":{"memberID":12,"changeCount":1000}},{"action":"addGold","param":{"memberID":12,"count":1000}}]');
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2013,"트랜잭션실패");
	
	return $r;
}


function tranjaction($p){
	$memberID = $p["memberID"];
	$cResult=array();
	$results = array();

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$p["actions"])return ResultState::makeReturn(2002,"actions");

	CommitManager::get()->begin($memberID);
	
	foreach ($p["actions"] as $key => $value) {
		$a = $value["action"];
		$p = $value["param"];
		$p["memberID"]=$memberID;
		if(method_exists($this,$a)){
			$r = $this->$a($p);
			$results[]=$r;
			if(!ResultState::successCheck($r["result"])){
				CommitManager::get()->setSuccess($memberID,false);
				break;
			}
		}
	}


	if(CommitManager::get()->isSuccess($memberID)){
		CommitManager::get()->commit($memberID);
		$cResult["list"]=$results;
		$cResult["result"]=ResultState::successToArray();
	}else{
		CommitManager::get()->rollback($memberID);
		return ResultState::makeReturn(2013,"Fail Tranjection");
	}

	return $cResult;
}

////////////////////////////////////////////////////////////////////////////////////

function help_getarchivementlist(){
	$r["description"] = "전체 업적 목록을 불러옵니다.";

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}

function getarchivementlist($p){
	$dataList = array();
    while($rData = Archivement::getRowByQuery()){
		$dataList[]=$rData;
    }

	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;

}



function help_getarchivementhistory(){
	$r["description"] = "유저가 진행중인 업적목록을 불러옵니다. ";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}

function getarchivementhistory($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
    while($rData = ArchivementHistory::getRowByQuery("where memberID=".$memberID)){
		$dataList[]=$rData;
    }

	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;


}



function help_updatearchivementhistory(){

	$r["description"] = "업적을 업데이트합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"archiveID","type"=>"char","description"=>"업적ID(필수)");
	$r["param"][] = array("name"=>"updateClearDate","type"=>"bool","description"=>"true 일 경우 클리어시간을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"updateRewardDate","type"=>"bool","description"=>"true 일 경우 보상시각을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"count","type"=>"int","description"=>"현재수치");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or puzzleNo");
	
	return $r;
}


function updatearchivementhistory($p){	
	
	$memberID=$p["memberID"];
	$archiveID=$p["archiveID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$archiveID)return ResultState::makeReturn(2002,"archiveID");
	
	//보내기
	$obj = new ArchivementHistory($memberID,$archiveID);
	$obj->memberID=$memberID;
	$obj->archiveID=$archiveID;
	if($p["updateClearDate"])$obj->clearDate=TimeManager::get()->getCurrentDateString();
	if($p["updateRewardDate"])$obj->rewardDate=TimeManager::get()->getCurrentDateString();
	if($p["count"])$obj->count=$p["count"];

	if($obj->save()){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	
	return $r;
}

/////////////////////////////////////////////////////////////////////////////////////////


function help_getgiftboxhistory(){
	$r["description"] = "선물함 목록을 가져옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"includeConfirm","type"=>"bool","description"=>"true일경우 확인메세지까지 불러옵니다.");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	
	return $r;
}

function getgiftboxhistory($p){
	$memberID=$p["memberID"];
	$includeConfirm = $p["includeConfirm"];


	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$where = "where memberID=".$memberID;
	if($includeConfirm)$where = $where." and confirmDate<>''";
	else $where = $where." and confirmDate=''";
	$dataList = array();
    while($rData = GiftBoxHistory::getRowByQuery($where)){
    	if($rData["reward"])$rData["reward"] = json_decode($rData["reward"],true);
		$dataList[]=$rData;
    }

	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;


}



function help_updategiftboxhistory(){

	$r["description"] = "선물함 메세지정보를 업데이트합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"giftBoxNo","type"=>"int","description"=>"기프트박스메세지번호");
	$r["param"][] = array("name"=>"updateConfirmDate","type"=>"bool","description"=>"true일경우 확인시각을 현재시각으로 업데이트합니다.");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or giftBoxNo");
	$r["result"][]=ResultState::toArray(2003,"dont find message");
	
	return $r;
}


function updategiftboxhistory($p){	
	
	$memberID=$p["memberID"];
	$giftBoxNo=$p["giftBoxNo"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$giftBoxNo)return ResultState::makeReturn(2002,"giftBoxNo");
	
	//보내기
	$obj = new GiftBoxHistory($memberID,$giftBoxNo);

	if(!$obj->isLoaded())return ResultState::makeReturn(2002,"dont find message");

	if($p["updateConfirmDate"])$obj->confirmDate=TimeManager::get()->getCurrentDateString();

	if($obj->save()){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	
	return $r;
}




function help_sendgiftboxhistory(){
	$r["description"] = "선물메세지를 보냅니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"sender","type"=>"string or int","description"=>"보내는사람(없으면 user)");
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"내용");
	$r["param"][] = array("name"=>"reward","type"=>"array(dict(key,value))","description"=>"보상");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}

function sendgiftboxhistory($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$p["sender"])$p["sender"]="user";
	$gb = new GiftBoxHistory($memberID);
	$gb->memberID = $memberID;
	$gb->sender = $p["sender"];
	$gb->content = $p["content"];
	$gb->reward = json_decode($p["reward"],true);
	$gb->data = $p["data"];

	if($gb->save()){
		$r["result"]=ResultState::successToArray();
	
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	return $r;


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

function help_getcharacterhistory(){
	$r["description"] = "내캐릭터목록을 들고옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	
	return $r;
}

function getcharacterhistory($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$where = "where memberID=".$memberID;
	$dataList = array();
    while($rData = CharacterHistory::getRowByQuery($where)){
    	$dataList[]=$rData;
    }

	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;


}



function help_updatecharacterhistory(){

	$r["description"] = "내캐릭터정보를 업데이트합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"characterNo","type"=>"int","description"=>"캐릭터번호");
	$r["param"][] = array("name"=>"level","type"=>"int","description"=>"레벨");
	$r["param"][] = array("name"=>"isSelected","type"=>"bool","description"=>"사용중여부");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or characterNo");
	$r["result"][]=ResultState::toArray(2003,"dont find message");
	
	return $r;
}


function updatecharacterhistory($p){	
	
	$memberID=$p["memberID"];
	$characterNo=$p["characterNo"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$characterNo)return ResultState::makeReturn(2002,"characterNo");
	
	//보내기
	$obj = new CharacterHistory($memberID,$characterNo);
	$obj->memberID = $memberID;
	$obj->characterNo = $characterNo;
	if(!$obj->isLoaded()){
		$obj->regDate = TimeManager::get()->getCurrentDateString();
	}

	if($p["level"])$obj->level = $p["level"];
	if($p["isSelected"])$obj->isSelected = $p["isSelected"];

	if($obj->save()){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	
	return $r;
}






//////////////////////////////////////////////////////////////////////////////////////////////////////////
}

?>