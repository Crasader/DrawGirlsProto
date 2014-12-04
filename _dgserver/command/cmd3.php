<?php

class commandClass
{
	public $m_hspMemberNo;
	public $m_kakaoMemberID;
	
	public function __construct(){
		//self::m_hspMemberNo = $param["hspMemberNo"] || $param["memberNo"];;
		//self::m_kakaoMemberID = $param["kakaoMemberID"];
	}
	
public static function getinfo($data){
	$data["appname"]="drawgirls";
	return $data;
}

public static function httpgateway($data) {

	if(!$data[version])$data[version]=Infomation::$m_httpgateway["version"];
	if(!$data[headerGameNo])$data[headerGameNo]=Infomation::$m_gameNo;
	

	$url = Infomation::$m_httpgateway["URL"];
	if($data["url"]){
		$url = $data["url"];
		unset($data["url"]);
	}

	if($data["api"]=="apilist"){
		unset($data["api"]);
		$url =Infomation::$m_httpgateway["helpURL"];
	}

	if($data["help"]){
		$url = Infomation::$m_httpgateway["helpURL"];
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
	curl_setopt($curlsession, CURLOPT_PORT, Infomation::$m_httpgateway["port"]);
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




public static function requestitemdelivery($p){
	$memberID = $p["memberID"];

	if(!$memberID) return ResultState::makeReturn(2002,"memberID");

	$pp["headerGameNo"] = Infomation::$m_gameNo;
	$pp["api"]="RequestItemDelivery2";
	$pp["deliveryMaxCount"]=10;
	$pp["deliveryHeader"]="memberNo-".$p["memberID"].",gameNo-".Infomation::$m_gameNo;
	if($p["deliveryTxId"])$pp["deliveryHeader"].",deliveryTxId-".$p["deliveryTxId"];
	LogManager::addLog("RequestItemDelivery param-->".json_encode($pp));
	$result = self::httpgateway($pp);
	LogManager::addLog("RequestItemDelivery httpgateway result-->".json_encode($result));

	if(!ResultState::successCheck($result["result"])){
		CommitManager::rollback($memberID);
		return $result;
	}
	$r = array();

	$ppp=array();
	$ppp["memberID"]=$p["memberID"];
	$ppp["exchangeIDList"]=array();
	$itemSeq=array();
	for($i=0;$i<count($result["data"]["deliveryResponse"]["itemIds"]);$i++){
		LogManager::addLog("item id is ".$result["data"]["deliveryResponse"]["itemIds"][$i]);
		$item = new Shop($result["data"]["deliveryResponse"]["itemIds"][$i]);
	
		if(!$item->isLoaded()){
			$item = new ShopEvent($result["data"]["deliveryResponse"]["itemIds"][$i]);
			if(!$item->isLoaded())continue;
		}
		
		$ppp["exchangeIDList"][] = $item->exchangeID; //array("type"=>$item->type,"count"=>$item->count,"statsID"=>"purchase","statsValue"=>"","content"=>"젬구매","isPurchase"=>true);
		$r["list"][]=$item->getArrayData();
		$itemSeq[]=$result["data"]["deliveryResponse"]["itemDeliverySequences"][$i];
	}
	

	CommitManager::begin($memberID);

	if(count($result["data"]["deliveryResponse"]["itemIds"])>0){
		$result2 = self::exchangebylist($ppp);
		
		if(ResultState::successCheck($result2["result"])){
			$pppp=array();
			$pppp["headerGameNo"] = Infomation::$m_gameNo;
			$pppp["api"]="FinishItemDelivery2";
			$pppp["itemDeliverySequences"]=implode(",",$itemSeq);
			$pppp["deliveryHeader"]="memberNo-".$p["memberID"].",gameNo-".Infomation::$m_gameNo;
			$result3 = self::httpgateway($pppp);
			LogManager::addLog("-->".json_encode($result3));
			if(ResultState::successCheck($result3["result"])){
				$user = UserData::create($memberID);
				if($user->isLoaded()){
					$user->isVIP = 1;
				}
				CommitManager::commit($memberID);
				return $result2;
			}
			CommitManager::setSuccess($memberID,false);
			CommitManager::rollback($memberID);
			return $result3;
		}
		CommitManager::setSuccess($memberID,false);
		CommitManager::rollback($memberID);
		return $result2;

	}

	CommitManager::commit($memberID);
	return ResultState::makeReturn(2016,"nothingworka");
	
}

////////////////////////////////////////


public static function getcommonsetting($p){
	
	$r=array();
	while($setting = CommonSetting::getRowByQuery("where forClient=1")){
		$key = $setting["key"];
		if($setting["isLocal"]==1){
			$setting["value"]=CurrentUserInfo::getLocalizedValueInData(json_decode($setting["value"],true));
		}
		$r[$key]=$setting["value"];
	}
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	
	return $r;
	
}

/////////////////////////////////////////

public static function checkmissionevent($p){
	$memberID = $p["memberID"];
	$type = $p["type"];
	$value = $p["value"];
	$value2 = $p["value2"];

	$missionInfo = new MissionEvent(null,$type,$value,$value2);

	if(!$missionInfo->isLoaded()){
		LogManager::addLog("loadFailed");
		return ResultState::makeReturn(ResultState::GDSUCCESS);
	}

	CommitManager::begin($memberID);

	$userInfo = UserData::create($memberID);

	$missionData =& $userInfo->getRef("missionEvent");

	$today = TimeManager::getCurrentDate();

	if($missionData["date"]!=$today){
		LogManager::addLog("new start today");
		$missionData["date"]=$today;
		$missionData["list"]=array();
	}

	if(!$missionData["list"])$missionData["list"]=array();

	if(in_array($missionInfo->no,$missionData["list"])){
		LogManager::addLog("failed already today");
		CommitManager::commit($memberID);
		return ResultState::makeReturn(ResultState::GDSUCCESS);	
	}


	$param["memberID"]=$memberID;
	$param["sender"]="GM";
	$param["exchangeID"]=$missionInfo->exchangeID;
	$param["content"]=$missionInfo->message;
	$sR = self::sendgiftboxhistory($param);
	
	if(!ResultState::successCheck($sR["result"])){

		LogManager::addLog("failed send message ".json_encode($sR));
		CommitManager::setSuccess($memberID,false);
	}

	$missionData["list"][]=$missionInfo->no; 

	CommitManager::setSuccess($memberID,$userInfo->save());

	if(CommitManager::commit($memberID))return ResultState::makeReturn(ResultState::GDSUCCESS);
	else return ResultState::makeReturn(ResultState::GDDONTSAVE);
}

////////////////////////////////////////////////

public static function getnoticelist($p){
	$buildNo=$p["buildNo"];
	$nowDate = TimeManager::getCurrentDateTime();
	

	$r = array();

	$storeBit = CurrentUserInfo::getStoreBit(CurrentUserInfo::$store);
	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	$addQuery = "";
	//if($buildNo)$addQuery = "and maxBuildNo>=".$buildNo." and minBuildNo<=".$buildNo;
	
	while($rData = Notice::getRowByQuery("where startDate<$nowDate and endDate>$nowDate and os&".$osBit.">0 and cc&".$ccBit.">0 and store&".$storeBit.">0 and isPopup=1 and isForDiary=0 order by `order` asc")){ //store&".$storeBit.">0 and 
		$rData[imgInfo]=json_decode($rData[imgInfo],true);	
		$r["list"][]=$rData;
	}

	$r["date"]=(string)TimeManager::getCurrentDateTime();
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	
	return $r;
}


public static function getrealtimemessage($p){
	$no=$p["version"];
	if(!$no)$no=0;

	$storeBit = CurrentUserInfo::getStoreBit(CurrentUserInfo::$store);
	$nowDate = TimeManager::getCurrentDateTime();
	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	$timecheck = TimeManager::getTimestampByUTC()-300;
	$todayDateTime = TimeManager::getCurrentDateTimeForKorea();

	$r = RealtimeMsg::getRowByQuery("where startDate<$todayDateTime and endDate>$todayDateTime and os&".$osBit.">0 and store&".$storeBit.">0 and cc&".$ccBit.">0  order by no desc limit 1"); //and store&".$storeBit.">0
	if($r){
		$r["version"]=$r["no"];
		$r["result"]=ResultState::successToArray();
	
	}else{
		$r["version"]=$no;
		$r["result"]=ResultState::toArray(ResultState::GDSAMEVERSION);
		CommitManager::$m_passFunc=true;
	}
	
	return $r;
}


public static function getnoticelistfordiary($p){
	$buildNo=$p["buildNo"];

	$nowDate = TimeManager::getCurrentDateTime();
	

	$r = array();

	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	$addQuery = "";
	if(!$buildNo)$buildNo=0;

	$addQuery = "and maxBuildNo>=".$buildNo." and minBuildNo<=".$buildNo;
	while($rData = Notice::getRowByQuery("where startDate<$nowDate and endDate>$nowDate and os&".$osBit.">0 and cc&".$ccBit.">0 and isPopup=1 and isForDiary<>0 ".$addQuery." order by `order` asc")){
		$rData[imgInfo]=json_decode($rData[imgInfo],true);	
		$r["list"][]=$rData;
	}

	$r["date"]=(string)TimeManager::getCurrentDateTime();
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	
	return $r;
}
/////////////////////////////////////////




public static function getpuzzleevent($p){

	$nowDate = TimeManager::getCurrentDateTime();
	

	$r = array();
	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	$weekDayBit = 1<<TimeManager::getCurrentWeekDayNo();
	//LogManager::addLog("where startDate<$nowDate and endDate>$nowDate and os&".$osBit.">0 and cc&".$ccBit.">0 and weekDay&".$weekDayBit.">0 order by `order` asc limit 1");
	while($rData = PuzzleEvent::getRowByQuery("where startDate<$nowDate and endDate>$nowDate and os&".$osBit.">0 and cc&".$ccBit.">0 and weekDay&".$weekDayBit.">0 order by `order` asc limit 1")){
		$r["data"]=$rData;
	}

	$r["date"]=(string)TimeManager::getCurrentDateTime();
	$r["result"]=ResultState::successToArray();
	$r["state"]="ok";
	
	return $r;
}

/////////////////////////////////////////

public static function getcharacterlist($p){

	
	
	$listVer = kvManager::get("charListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		
		return $r;
	}
	
	
	$list = array();
	while($charInfo = Character::getRowByQuery("where serial<1000 order by `serial` asc")){
			$charInfo[purchaseInfo]=json_decode($charInfo[purchaseInfo],true);
			$charInfo[statInfo]=json_decode($charInfo[statInfo],true);
			$charInfo[resourceInfo]=json_decode($charInfo[resourceInfo],true);


			$script=json_decode($charInfo[name],true);
			$charInfo["name"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];

			$script=json_decode($charInfo[comment],true);
			$charInfo["comment"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			
			$script=json_decode($charInfo[scriptInfo],true);
			$charInfo["scriptInfo"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
	
			$list[]=$charInfo;
	}
	
	$r["list"]=$list;
	$r["version"]=$listVer;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

/////////////////////////////////////////


public static function getshoplist($p){

	$storeID = CurrentUserInfo::$store;

	$listVer = kvManager::get("shopListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		
		return $r;
	}
	
	$r=array();
	$list = array();
	$oldlistname = "";
	$shopLoad=false;
	$storeQuery = "";

	if($storeID){
		$storeBit = CurrentUserInfo::getStoreBit($storeID);
		$storeQuery = "and store&".$storeBit.">0";
	}else{
		$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
		$storeQuery = "and os&".$osBit.">0";
	}

	$result = Shop::getQueryResult("select ".Shop::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".Shop::getDBTable()." left join ".Exchange::getDBTable()." on ".Shop::getDBTable().".exchangeID=".Exchange::getDBTable().".id where cc='".CurrentUserInfo::$country."' ".$storeQuery);
	while($shopInfo = mysql_fetch_assoc($result)){
	
		//while($shopInfo = Shop::getRowByQuery("where cc='".CurrentUserInfo::$country."' and os&".$osBit.">0")){
		$shopInfo["data"]=json_decode($shopInfo["data"],true);
		$id = $shopInfo["id"];
		$shopInfo["pID"]=$shopInfo["pID"]." ";
		$r[$id]=$shopInfo;
		$shopLoad=true;
	}
	if(!$shopLoad){
		$result2 = Shop::getQueryResult("select ".Shop::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".Shop::getDBTable()." left join ".Exchange::getDBTable()." on ".Shop::getDBTable().".exchangeID=".Exchange::getDBTable().".id where cc='default' ".$storeQuery);
		while($shopInfo = mysql_fetch_assoc($result)){
			$shopInfo["data"]=json_decode($shopInfo["data"],true);
			$shopInfo["pID"]=$shopInfo["pID"]." ";
			$id = $shopInfo["id"];
			$r[$id]=$shopInfo;
			$shopLoad=true;
		}
	}

	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	$today = TimeManager::getCurrentDateTime();
	$nowtime = TimeManager::getCurrentTime();
	$ingEvent = false;

	$result2 = ShopEvent::getQueryResult("select ".ShopEvent::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".ShopEvent::getDBTable()." left join ".Exchange::getDBTable()." on ".ShopEvent::getDBTable().".exchangeID=".Exchange::getDBTable().".id where ".ShopEvent::getDBTable().".startDate<=".$today." and ".ShopEvent::getDBTable().".endDate>=".$today." and ".ShopEvent::getDBTable().".startTime<=".$nowtime." and ".ShopEvent::getDBTable().".endTime>=".$nowtime." and ".ShopEvent::getDBTable().".cc&".$ccBit.">0 ".$storeQuery);
	while($shopInfo = mysql_fetch_assoc($result)){
	//while($shopInfo = ShopEvent::getRowByQuery("where startDate<=".$today." and endDate>=".$today." and startTime<=".$nowtime." and endTime>=".$nowtime." and os&".$osBit.">0 and cc&".$ccBit.">0")){
			$shopInfo["data"]=json_decode($shopInfo["data"],true);
			$shopInfo["pID"]=$shopInfo["pID"]." ";
			$id = $shopInfo["id"];
			$r[$id]=$shopInfo;
			$ingEvent = true;
			//LogManager::addLog("find event".json_encode($shopInfo));
	}

	$r["event"]=$ingEvent;
	$r["list"]=$list;
	$r["version"]=$listVer;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}


/////////////////////////////////////////

public static function getmonsterlist($p){

	
	$listVer = kvManager::get("monsterListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		
		return $r;
	}
	
	
	$list = array();
	while($mobInfo = Monster::getRowByQuery()){
			$script=json_decode($mobInfo[script],true);
			$mobInfo["script"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			$mobInfo[resourceInfo]=json_decode($mobInfo[resourceInfo],true);
	
			$list[]=$mobInfo;
	}
	
	$r["list"]=$list;
	$r["version"]=$listVer;
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}

/////////////////////////////////////////

/////////////////////////////////////////

public static function getpieceinfo($p){
	$piece = new Piece($p["no"]);

	if($piece->version==$p[version]){
		$r[version]=$p[version];
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		return $r;
	}

	if(!$piece->isLoaded()){
		return ResultState::makeReturn(ResultState::GDDONTFIND);
	}

	$r = $piece->getArrayDataForClient();	
	$r["result"]=ResultState::successToArray();
	
	return $r;
}

///////////////////////////////////////////

public static function getcardlist($p){

	$puzzleListVer = kvManager::get("cardListVer",1);

	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		return $r;
	}


	$cardlist="";
	
	if(is_array($p["noList"]))
		$cardlist = join(',',$p["noList"]);
	else 
		$cardlist = $p["noList"];
	
	
	$list = array();
	while($cardInfo = Card::getRowByQuery("where no IN (".$cardlist.")")){
		if($cardInfo){

			$script=json_decode($cardInfo["profile"],true);
			$cardInfo["profile"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			
			$script=json_decode($cardInfo[script],true);
			$cardInfo["script"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			
			$script=json_decode($cardInfo["name"],true);
			$cardInfo["name"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];

			$list[]=$cardInfo;
		}
	}
	
	$r["list"]= $list; //json_decode(json_encode($list,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK),true);
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;

}



public static function getallcardlist($p){

	if(!$p[version])$p[version]=0;
	$puzzleListVer = kvManager::get("cardListVer",1);

	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		return $r;
	}

	
	$list = array();
	while($cardInfo = Card::getRowByQuery("where version>".$p[version])){
		if($cardInfo){

			$script=json_decode($cardInfo["profile"],true);
			$cardInfo["profile"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["defa"];
			
			$script=json_decode($cardInfo[script],true);
			$cardInfo["script"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			
			$script=json_decode($cardInfo["name"],true);
			$cardInfo["name"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];

			$list[]=$cardInfo;
		}
	}
	
	$r["list"]= $list; //json_decode(json_encode($list,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK),true);
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;

}
///////////////////////////////////////////////

public static function getcardlistbylist($p){

	if(!$p[version])$p[version]=0;
	$puzzleListVer = kvManager::get("cardListVer",1);
	$noList = $p["noList"];
	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		return $r;
	}

	
	$list = array();


	$cardlist="";
	if(is_array($noList)){
		$cardlist = join(',',$noList);
	}else 
		$cardlist = $p["noList"];
	
	
	$list = array();
	while($cardInfo = Card::getRowByQuery("where  no IN (".$cardlist.") AND version>".$p[version])){
		if($cardInfo){

			$script=json_decode($cardInfo["profile"],true);
			$cardInfo["profile"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			
			$script=json_decode($cardInfo[script],true);
			$cardInfo["script"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			
			$script=json_decode($cardInfo["name"],true);
			$cardInfo["name"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];

			$list[]=$cardInfo;
		}
	}
	
	$r["list"]= $list; //json_decode(json_encode($list,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK),true);
	$r["state"]="ok";
	$r["version"]=$puzzleListVer;
	$r["result"]=ResultState::successToArray();
	return $r;

}



public static function getadultcardlist($p){

	if(!$p[version])$p[version]=0;
	
	$puzzleListVer = kvManager::get("cardListVer",1);
	
	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		return $r;
	}

	$cardlist=array();
	LogManager::addLog("start getadultcardlist");
	while($cardInfo = Card::getRowByQuery("where haveAdult<>0")){
		LogManager::addLog(json_encode($cardInfo));
		LogManager::addLog(mysql_error());
		$cardlist[]=$cardInfo["serial"];
	}
		LogManager::addLog(mysql_error());
	
	$r["list"]= $cardlist;
	$r["version"]=$puzzleListVer;
	$r["result"]=ResultState::successToArray();
	return $r;

}
/////////////////////////////////////////////


public static function getpuzzlelist($p){
	$puzzleListVer = kvManager::get("puzzleListVer",1);

	if($puzzleListVer==$p[version]){
		$r[version]=$p[version];
	}else{
		// 여기서 eventstagelistversion 도 비교
		if(!$p[start])$p[start]=0;
		if(!$p[limit])$p[limit]=100;
		
		$list=array();
		$cnt=0;
		while($pData =  Puzzle::getRowByQuery("where no>0 and isEvent>=0 order by `order` asc limit ".$p[start].",".$p[limit])){
			
			//퍼즐 시작stage 알아오기
			$pData["startStage"]=Puzzle::getPieceStart($pData["no"]);
			
			//퍼즐 안에 스테이지 갯수 알아오기
			$pData["stageCount"]=Puzzle::getPieceCount($pData["no"]);
			
			if(!$pData["startStage"] || !$pData["stageCount"])continue;

			//$pData[version]=kvManager::get("puzzleVer_".$pData[no],1);
			$pData[thumbnail]=json_decode($pData[thumbnail],true);
			$pData[pathInfo]=json_decode($pData[pathInfo],true);
			$pData[cardInfo]=json_decode($pData[cardInfo],true);
			//$pData[rewardInfo]=json_decode($pData[rewardInfo],true);
			//$pData[levelInfo]=json_decode($pData[levelInfo],true);
			$pData[conditionInfo]=json_decode($pData[conditionInfo],true);
			//$pData[coordinateInfo]=json_decode($pData[coordinateInfo],true);
			//$pData[startPosition]=json_decode($pData[startPosition],true);
			//$pData[endPosition]=json_decode($pData[endPosition],true);
			$pData[color]=json_decode($pData[color],true);
			
			$script=json_decode($pData[title],true);
			$pData["title"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			
			$pData[clearReward]=json_decode($pData[clearReward],true);
			$pData[center] = json_decode($pData[center],true);
			$pData[original] = json_decode($pData[original],true);
			$pData[face] = json_decode($pData[face],true);
			$pData[map] = json_decode($pData[map],true);
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


//////////////////////////////////////////



public static function gethellmodelist($p){
	global $ERRORCODE;
	
	$hellModeListVer = kvManager::get("hellModeListVer",1);

	if($hellModeListVer==$p[version]){
		$r[version]=$p[version];
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		return $r;
	}else{
		// 여기서 eventstagelistversion 도 비교
		
		$list=array();
		$cnt=0;
		while($pData =  HellMode::getRowByQuery("")){
			
			$pieceInfo = new Piece($pData["pieceNo"]);
			$pData["pieceInfo"]=$pieceInfo->getArrayDataForClient();

			$script=json_decode($pData[title],true);
			$pData["title"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];

			$script=json_decode($pData[content],true);
			$pData["content"] = $script[CurrentUserInfo::$country]?$script[CurrentUserInfo::$country]:$script["default"];
			
			$list[]=$pData;
		}
		
		$puzzleInfo["list"]=$list;
		//$puzzleInfo["puzzlelist"]=$list;
		$puzzleInfo["result"]=ResultState::successToArray();
		$puzzleInfo["version"]=$hellModeListVer;
		return $puzzleInfo;
	
	}
	

	
}


//////////////////////////////////////////


public static function writelog($p){
	//LogManager::addLog("----start write log---");
	if(Logmanager::isLocked()){
		$result["result"]=ResultState::successToArray();
		return $result;
	}
	if(!$p["input"])$p["input"]=$p["content"];
	if(!$p["memberID"])$p["memberID"]=1;
	$userlog = new UserLog($p["memberID"]);
	$userlog->memberID = $p["memberID"];
	$userlog->output = $p["output"];
	$userlog->input = $p["input"];
	$userlog->regDate = TimeManager::getCurrentDateTime();
	$userlog->regTime = TimeManager::getTime();
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

///////////////////////////////////////////

public static function dropoutuser($p){
	$memberID = $p["memberID"];
	$keylist = $p["keyList"];
	if($memberID){
		CommitManager::begin($memberID);
		$user = UserData::create($memberID);

		
		if($user->isLoaded()){
			CommitManager::setSuccess($memberID,UserData::removeRowByQueryWithShardKey("where memberID=".$user->memberID,$memberID));
			//CommitManager::setSuccess($memberID,StageScore::removeRowByQueryWithShardKey("where memberID=".$user->memberID,$memberID));
			CommitManager::setSuccess($memberID,WeeklyScore::removeRowByQueryWithShardKey("where memberID=".$user->memberID,$memberID));
			CommitManager::setSuccess($memberID,UserPropertyHistory::removeRowByQueryWithShardKey("where memberID=".$user->memberID,$memberID));
			CommitManager::setSuccess($memberID,EndlessRank::removeRowByQueryWithShardKey("where memberID=".$user->memberID,$memberID));
			
			CommitManager::setSuccess($memberID,$user->remove());

			if(CommitManager::commit($memberID)){
				$user->getUserIndex()->remove();
				$r["result"]=ResultState::successToArray();
			}else{
				$r["result"]=ResultState::toArray(1001,"query fail");
			}
		}else{
			CommitManager::rollback($memberID);
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

public static function makediarycode($p){
	$memberID=$p["memberID"];
	if(!$memberID)return ResultState::makeReturn(ResultState::GDPARAMETER,"memberID");

	$user = UserData::create($memberID);
	if(!$user->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);
	
	$randcode = CuponCode::getRandomString(4);
	$code = $randcode.dechex($user->getUserIndexNo());

	LogManager::addLog("randcode:".$randcode."/userindex:".$user->getUserIndexNo()."/enNumber:".dechex($user->getUserIndexNo()));
	$user->diaryCode = $code;

	if($user->save()){
		$r["diaryCode"]=$user->diaryCode;
		$r["result"]=ResultState::successToArray();
		return $r;
	}else{
		return ResultState::makeReturn(ResultState::GDDONTSAVE,mysql_error());
	}
}

public static function checkdiarycode($p){
	$diaryCode = $p["diaryCode"];
	$code = substr($diaryCode, 0,4);
	$uindex = hexdec(substr($diaryCode, 4));
	LogManager::addLog("code:".$code." / number:".substr($diaryCode, 4)." deNumber:".$uindex);
	$userIndex = UserIndex::create(null,$uindex);

	if(!$userIndex->isLoaded()){
		LogManager::addLog("dont find user");
		return ResultState::makeReturn(ResultState::GDEXPIRE);
	}

	$userData = UserData::create($userIndex->memberID);

	if($userData->diaryCode==$diaryCode){
		$r["result"] = ResultState::toArray(ResultState::GDSUCCESS);
		$r["diaryCode"]=$diaryCode; //CuponCode::getRandomString(5);
		$r["memberID"]=$userData->memberID;
		$userData->diaryCode=$r["diaryCode"];
		if($userData->save())return $r;
		else return ResultState::makeReturn(ResultState::GDDONTSAVE);
	}else{
		return ResultState::makeReturn(ResultState::GDEXPIRE);
	}
}

public static function login($p){
	$memberID = $p["memberID"];
	if($memberID){
		$user = UserData::create($memberID);
		
		if(!$user->isLoaded())return ResultState::makeReturn(ResultState::GDNEEDJOIN);
		
		//닉네임없으면 다시 가입하라고~
		if(!$user->getUserIndex()->nick)return ResultState::makeReturn(ResultState::GDNEEDJOIN);

		if($p["diaryCode"]){
			if($p["diaryCode"]==$user->diaryCode){
				if(!$user->diaryJoinDate){
					$user->diaryJoinDate = TimeManager::getCurrentDateTime();
					$user->save();
				}
				return ResultState::makeReturn(ResultState::GDSUCCESS);
			}else{
				return ResultState::makeReturn(ResultState::GDDONTFIND);
			}
		}

		if($user->blockDate>TimeManager::getCurrentDateTime()){
			$r["result"] = ResultState::toArray(ResultState::GDBLOCKEDUSER,$user->blockReason);
			$r["blockDate"]=$user->blockDate;
			$r["blockReason"]=$user->blockReason;
			return $r;
		}

		$endlessData =& $user->getRef("endlessData");
		
		if($endlessData["ing_state"]==1 || $endlessData["ing_week"]!=TimeManager::getCurrentWeekNoForRank()){
			$endlessData["ing_win"]=0;
			if($endlessData["ing_state"])$endlessData["lose"]+=1;
			$endlessData["ing_score"]=0;
			$endlessData["ing_week"]=TimeManager::getCurrentWeekNoForRank();
		}

		$user->lastDate = TimeManager::getCurrentDateTime();
		$user->lastTime = TimeManager::getTime();
		$user->loginType = $p["loginType"];
		
		srand((double)microtime()*1000000);
		$newID=rand(1,100000);
		while($user->deviceID==$newID){
			$newID = rand(1,100000);
		}
		$user->deviceID=$newID;
		$user->lastCmdNo=0;
		$user->save();

		$param["memberID"]=$memberID;
		self::checkloginevent($param);

		$r["data"] = $user->getArrayData(true);
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
	}
	
	return $r;
}


public static function join($p){
	$memberID = $p["memberID"];
	$nick = $p["nick"];
	$nick = iconv("EUC-KR", "UTF-8", iconv("UTF-8", "EUC-KR//TRANSLIT", $nick));
	$nick = preg_replace("/\s+/", "", $nick);


	if(preg_match("/[^a-z0-9A-Z一-龥ぁ-んァ-ヶ가-힣]+/iu",$nick)){
		return ResultState::makeReturn(2009);
	}

	if(!$memberID)return ResultState::makeReturn(2002);
	if(!$nick)return ResultState::makeReturn(2002);
	if(!$p["flag"])$p["flag"]="kr";

	$nick = trim(addslashes($nick));

	if(mb_strlen($nick,'UTF-8')>8)return ResultState::makeReturn(2011); 
	if(mb_strlen($nick,'UTF-8')<2)return ResultState::makeReturn(2012); 




	$user = UserData::create($memberID);
	$userIndex = $user->getUserIndex();
	$user->memberID = $memberID;
	$user->flag = $p["flag"];
	$user->level = 1;
	$user->country = CurrentUserInfo::$country;

    //LogManager::addLog("join1 userindex is ".json_encode($userIndex->getArrayData(true)));

	
	//이미 가입한유저
	if($user->isLoaded() && $user->getUserIndex()->nick)return ResultState::makeReturn(2010);

	//닉네임중복검사
	while($rData = UserIndex::getRowByQuery("where nick='".$nick."'")){
		if($rData){
			//LogManager::addLog("dupl data is".json_encode($rData));
			return ResultState::makeReturn(2008);
		}
	}

	CommitManager::begin($memberID);

	//SELECT * FROM drawgirls.aFaultyNickTable where 'ㄱ' regexp `pattern`; 
	while($rData = FaultyNick::getRowByQuery("where '".addslashes($nick)."' regexp `nick` and isInclusionRule=1 limit 1")){
		//if($rData){
			return ResultState::makeReturn(2009);
		//}
	}

	
	//deviceID발급
	srand((double)microtime()*1000000);
	$newID=rand(1,100000);
	while($user->deviceID==$newID){
		$newID = rand(1,100000);
	}
	$user->deviceID=$newID;
	$user->lastCmdNo=0;
	$user->selectedCharNO=1;

    //LogManager::addLog("join user DeviceID s1 ".$user->deviceID);

	$userIndex->nick = $nick;
	$user->nick = $nick;
	$user->joinDate=TimeManager::getCurrentDateTime();
	
    //LogManager::addLog("join2 userindex is ".json_encode($userIndex->getArrayData(true)));

	if($userIndex->save()){
		if($user->save()){
			$r["data"] = $user->getArrayData(true);
			$r["result"]=ResultState::successToArray();
	
			$userStorage = new UserStorage($memberID);
			


			$freeset = new CommonSetting("freeset");
			$freesetDict =& $freeset->getRef("value");

			foreach ($freesetDict as $key => $value) {
				$userStorage->{$key}=$value;
			}

			CommitManager::setSuccess($memberID,$userStorage->save());

			$character = new CharacterHistory($memberID,1);
			$character->memberID=$memberID;
			$character->characterNo = 1;
			$character->level=1;
			$character->regDate = TimeManager::getCurrentDateTime();
			CommitManager::setSuccess($memberID,$character->save());

			if(CommitManager::commit($memberID)){
				return $r;
			}else{
				return ResultState::makeReturn(2014);
			}
		}
	}else{
		CommitManager::commit($memberID);
	}



	return ResultState::makeReturn(1001);
}


public static function setuserdata($p){
	$memberid = $p["memberID"];
	if($memberid){
		$user = UserData::create($memberid);
		
		if($p["nick"]){
			$user->nick = $p["nick"];
		}
		if($p["data"])$user->data = $p["data"];
		
		
		if(!$user->save())return ResultState::makeReturn(2006);
		
		$r["state"]="ok";
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(2002,"memberID");
	}
	
	return $r;
}

public static function getuserdata($p){
	$memberid = $p["memberID"];
	$userindex = $p["userIndex"];
	$nick = $p["nick"];
	$keylist = $p["keyList"];
	$isPublic  = $p["isPublic"];
	$user = null;
	if($memberid){
		$user = UserData::create($memberid);
	}else if($userindex && $userindex>0){
		$uIndex = UserIndex::create(0,$userindex);
		if(!$uIndex->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"fail to load userdata2");
			return $r;
		}
		$user = UserData::create($uIndex->memberID);
	}else if($nick){
		$user = UserData::create(null,null,$nick);
	}else{
		$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"memberID");
		return $r;
	}

	if($user->isLoaded()){
		if($isPublic){
			$r = $user->getArrayDataForPublic();
		}else{
			$r = $user->getArrayData(true,$keylist);
		}
		$r["state"]="ok";
		$r["userIndex"]=$user->getUserIndexNo();
		$r["missileInfo"]=Character::getPowerInfo($user->level);
		$r["result"]=ResultState::successToArray();
	}else{
		$r["state"]="error";
		$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"fail to load userdata1");
	}
	
	return $r;

}



public static function updateuserdata($p){
	$memberid = $p["memberID"];
	$r=array();
	if($memberid){
		$user = UserData::create($memberid);
		//LogManager::addLog("updateuserdata for ".$memberid);
		if(!$user->isLoaded()){
			//LogManager::addLog("updateuserdata load ok ".json_encode($user->getArrayData()));
			return ResultState::makeReturn(ResultState::GDDONTFINDUSER);
		}
		//if($p["nick"])$user->nick = $p["nick"];
		if(array_key_exists("isVIP",$p))$user->isVIP = $p["isVIP"];
		if(array_key_exists("isFirstBuy",$p))$user->isFirstBuy = $p["isFirstBuy"];
		if(array_key_exists("onlyOneBuyPack",$p))$user->onlyOneBuyPack = $p["onlyOneBuyPack"];
		if(array_key_exists("totalPlayCount",$p))$user->totalPlayCount = $p["totalPlayCount"];
		if(array_key_exists("failCount",$p))$user->failCount = $p["failCount"];
		if(array_key_exists("autoLevel",$p))$user->autoLevel = $p["autoLevel"];
		if(array_key_exists("pGuide",$p))$user->pGuide = $p["pGuide"];
		if(array_key_exists("selectedCharNO",$p))$user->selectedCharNO=$p["selectedCharNO"];
		if(array_key_exists("highScore",$p) && $user->highScore<$p["highScore"])$user->highScore=$p["highScore"];
		if(array_key_exists("aMapGacha",$p))$user->setArchiveData("aMapGacha",$p["aMapGacha"]);
		if(array_key_exists("aNoFail",$p))$user->setArchiveData("aNoFail",$p["aNoFail"]);
		if(array_key_exists("aHunter",$p))$user->setArchiveData("aHunter",$p["aHunter"]);
		if(array_key_exists("aChange",$p))$user->setArchiveData("aChange",$p["aChange"]);
		if(array_key_exists("aFail",$p))$user->setArchiveData("aFail",$p["aFail"]);
		if(array_key_exists("aPerfect",$p))$user->setArchiveData("aPerfect",$p["aPerfect"]);
		if(array_key_exists("aSeqAtd",$p))$user->setArchiveData("aSeqAtd",$p["aSeqAtd"]);
		if(array_key_exists("loginType",$p))$user->loginType=$p["loginType"];
		if($p["archiveData"]){
			$archiveData =& $user->getRef("archiveData");
			$archiveData = array_merge($archiveData,$p["archiveData"]);
		}
		if($p["highPiece"])$user->highPiece = $p["highPiece"];

		if(!$user->save())return ResultState::makeReturn(2006);
		
		$r=$user->getArrayData();
		$r["userIndex"]=$user->getUserIndexNo();
		$r["missileInfo"]=Character::getPowerInfo($user->level);

		$r["result"]=ResultState::successToArray();
		$r["state"]="ok";
	}else{
		return ResultState::makeReturn(ResultState::GDDONTFINDUSER);
	}
	
	return $r;
}


public static function getuserlistbyrandom($p){
	$memberID = $p["memberID"];
	$limit = $p["limit"];
	if(!$limit)$limit=10;
	if(!$memberID)return ResultState::makeReturn(ResultState::GDDONTFINDUSER);

	$user = UserData::create($memberID);
	if(!$user->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);
	$friendlist = $user->getRef("friendList");
	if(!is_array($friendlist))$friendlist=array();
	$friendlist[]=$memberID;
	$userList=array();
	while($other = UserData::getRowByQueryWithRandom("where nick <> '' order by rand() limit $limit","memberID,nick,lastDate,flag,highPiece,highScore,selectedCharNO")){
		if(!in_array($other["memberID"],$friendlist)){
			$other["characterNo"]=$other["selectedCharNO"];
			$userList[]=$other;
		}
	}
	$r["result"]=ResultState::successToArray();
	$r["list"]=$userList;
	return $r;
}

public static function getuserlist($p){
	$memberIDList = $p["memberIDList"];
	
	if(!$memberIDList)return ResultState::makeReturn(ResultState::GDDONTFINDUSER);
	
	$userlist = array();
	foreach ($memberIDList as $key => $value) {
		$userInfo = UserData::create($value);
		$userlist[]=$userInfo->getArrayDataForPublic();
	}
	$r["result"]=ResultState::successToArray();
	$r["list"]=$userlist;
	return $r;
}



public static function saveintroducer($p){
	$memberID = $p["memberID"];
	$nick = $p["nick"];
	$content = $p["content"];
	
	$introducer = UserData::create(null,null,$nick);
	if(!$introducer->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND);
	
	$user = UserData::create($memberID);
	if(!$user->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);
	if($user->introducerID)return ResultState::makeReturn(ResultState::GDALREADY);
	
	CommitManager::begin($memberID);
	
	
	$user->introducerID = $introducer->memberID;


	CommitManager::setSuccess($memberID,$user->save());

	$param["memberID"]=$user->memberID;
	$param["sender"]=$introducer->nick;
	$param["exchangeID"]="introduceSave";
	$param["exchangeList"]=array(array("type"=>"r","content"=>$introducer->memberID));
	$param["template"]="msgSaveIntroduce";
	$sR2 = self::sendgiftboxhistory($param);

	CommitManager::setSuccess($memberID,ResultState::successCheck($sR2["result"]));

	if(CommitManager::commit($memberID)){
		$sR2["introducerID"]=(string)$introducer->memberID;
		return $sR2;
	}else{
		return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
	}

	$r =  ResultState::makeReturn(ResultState::GDSUCCESS);
	$r["introducerID"]=(string)$introducer->memberID;
	return $r;
}


public static function completeintroducer($p){
	CommitManager::$m_passFunc=true;

	$memberID = $p["memberID"];
	
	$user = UserData::create($memberID);

	if(!$user->introducerID)return ResultState::makeReturn(ResultState::GDPARAMETER);

	LogManager::addLog($user->introducerID);

	LogManager::addLog("gogo1?");

	if($user->introducerID<0) return ResultState::makeReturn(ResultState::GDALREADY);
	
	LogManager::addLog("gogo2?");
	

	$introducer = UserData::create($user->introducerID);
	if(!$introducer->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND);
	
	if($memberID==$introducer->memberID)return ResultState::makeReturn(ResultState::GDDONTFIND);


	$user->introducerID = $user->introducerID*-1;

	if($user->save()){	
		
		CommitManager::begin($introducer->memberID);


		$introducer->introduceCnt=$introducer->introduceCnt+1;
		CommitManager::setSuccess($introducer->memberID,$introducer->save());

		//여기에 추천인수에 따른 보상지급하기
		$cs = new CommonSetting("introduceReward");
		$rewardInfo =& $cs->getRef("value");
		$checkIndex = -1;
		LogManager::addLog(json_encode($rewardInfo));

		for($i=0;$i<count($rewardInfo["count"]);$i++){
			if($rewardInfo["count"][$i]==$introducer->introduceCnt){
				$checkIndex=$i;
				break;
			}
		}

		if($checkIndex!=-1){
			LogManager::addLog("msg is ".$rewardInfo["msg"][$checkIndex]." and exchangeID is ".$rewardInfo["exchangeID"][$checkIndex]);
			$param["memberID"]=$introducer->memberID;
			$param["sender"]=$user->nick;
			$param["content"]=$rewardInfo["msg"][$checkIndex];
			$param["exchangeID"]=$rewardInfo["exchangeID"][$checkIndex];
			$sR1 = self::sendgiftboxhistory($param);
			CommitManager::setSuccess($introducer->memberID,ResultState::successCheck($sR1["result"]));
		}


		LogManager::addLog("introduce cnt is".$introducer->introduceCnt);

		LogManager::addLog("checkIndex is".$checkIndex);

		if(CommitManager::commit($introducer->memberID)){
			return ResultState::makeReturn(ResultState::GDSUCCESS);
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!".mysql_error().json_encode($sR1)."int->".$introducer->introduceCnt);
		}
	}else{
		return ResultState::makeReturn(ResultState::GDDONTSAVE);
	}
}

static public function getintroducereward($p){
	$memberID = $p["memberID"];
	if(!$memberID)return ResultState::makeReturn(ResultState::GDPARAMETER);
	
	$userdata = new UserData($memberID);
	if(!$userdata->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);

	$r["introduceCnt"]=$userdata->introduceCnt;

	$rewardInfo = new CommonSetting("introduceReward");
	if(!$rewardInfo->value)return ResultState::makeReturn(ResultState::GDDONTFIND);
	$r["reward"] = $rewardInfo->value;
	$r["result"]=ResultState::toArray(GDSUCCESS);
	return $r;

}

static public function getfriendlist($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(ResultState::GDPARAMETER);

	$myInfo = UserData::create($memberID);

	$userlist=array();
	$friendList =& $myInfo->getRef("friendList");
	if(!is_array($friendList))$friendList=array();
	foreach ($friendList as $key => $value) {
		$userInfo = UserData::create($value);
		if($userInfo->isLoaded())$userlist[]=$userInfo->getArrayDataForPublic();
	}

	//$userlist[]=$myInfo->getArrayDataForPublic();
	$r["result"]=ResultState::successToArray();

	usort($userlist, function($a, $b){
		return $a['highPiece']<$b['highPiece'];
	});

	$r["list"]=$userlist;
	return $r;

}


public static function addfriend($p){
	$memberID = $p["memberID"];
	$friendID = $p["friendID"];
	$limit = $p["limit"];
	if(!$limit)$limit=10;
	if(!$memberID)return ResultState::makeReturn(ResultState::GDPARAMETER,"memberID");
	if(!$friendID)return ResultState::makeReturn(ResultState::GDPARAMETER,"friendID");
	$memberInfo = UserData::create($memberID);
	if(!$memberInfo->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);
	$friendInfo = UserData::create($friendID);
	if(!$friendInfo->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);

	$fListIn=false;
	$friendList =& $memberInfo->getRef("friendList");
	$friendList2 =& $friendInfo->getRef("friendList");

	//친구제한 넘었는지 검사
	$cs = new CommonSetting("gameFriendMax");

	//서로추가하고 중복제거
	$friendList[]=$friendID;
	$friendList = array_unique($friendList);

	if(count($friendList)>$cs->value){
		return ResultState::makeReturn(ResultState::GDFRIENDMAX,"me");
	}

	$friendList2[]=$memberID;
	$friendList2 = array_unique($friendList2);

	if(count($friendList2)>$cs->value){
		return ResultState::makeReturn(ResultState::GDFRIENDMAX,"you");
	}

	if($fListIn==false){
		if($memberInfo->save()){
			if($friendInfo->save())return ResultState::makeReturn(ResultState::GDSUCCESS);
			return ResultState::makeReturn(ResultState::GDDONTSAVE);
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE);
		}

		
	}else{
		return ResultState::makeReturn(ResultState::GDSUCCESS);
	}
}

public static function removefriend($p){
	$memberID = $p["memberID"];
	$friendID = $p["friendID"];

	$userInfo = UserData::create($memberID);

	if(!$userInfo->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);

	$friendList =& $userInfo->getRef("friendList");

	foreach ($friendList as $key => $value) {
		if($value==$friendID){
			unset($friendList[$key]);
		}
	}


	$friendInfo = UserData::create($friendID);
	if($friendInfo->isLoaded()){
		$ffriendlist =& $friendInfo->getRef("friendList");

		foreach ($ffriendlist as $key => $value) {
			if($value==$memberID){
				unset($ffriendlist[$key]);
			}
		}
		$friendInfo->save();
	}
	
	$userInfo->save();

	return ResultState::makeReturn(ResultState::GDSUCCESS);
}
public static function sendmessage($p){
	//친구제안메세지보내기

	$param["memberID"]=$p["friendID"];
	$param["sender"]=$p["nick"];
	$param["content"]=$p["content"];
	$param["data"]=$p["data"];
	$param["template"]=$p["template"];
	$param["custom"]=$p["custom"];

	if($p["sendMemberID"])$param["sendMemberID"]=$p["sendMemberID"];
	if($p["memberID"])$param["sendMemberID"]=$p["memberID"];
	$sR = self::sendgiftboxhistory($param);
	
	return $sR;

}

public static function addweeklyscore($p){

	$ws =new WeeklyScore($p["memberID"],TimeManager::getCurrentWeekNoForRank());
	$ws->memberID = $p["memberID"];
	$ws->data = $p["data"];
	$ws->nick = $p["nick"];
	$ws->flag = $p["flag"];
	$ws->regDate = TimeManager::getCurrentDateTime();
	$ws->regTime = TimeManager::getTime();
	$ws->regWeek = TimeManager::getCurrentWeekNoForRank();
	$ws->score = $ws->score + $p["score"];
	$ws->count = $ws->count+1;
	$r["update"]=false;
	$r["uresult"]=$ws->save();
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}


public static function getweeklyrankbyalluser($p){

	////////////////////////////////////////////////////
	$memberID = $p["memberID"];
	$start = $p["start"];
	$limit = $p["limit"];
	$weekNo = TimeManager::getCurrentWeekNoForRank();
	if($p["weekNo"])$weekNo=$p["weekNo"];
	if(!$start)$start=1;
	if(!$limit)$limit=10;


	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$myRank = new WeeklyScore($memberID,$weekNo);
	$myRank->memberID=$memberID;
	$myRank->regWeek=$weekNo;

	$rewardInfo = new CommonSetting("weeklyReward");

	$nrinfo=array();
	foreach ($rewardInfo->value as $key => $value) {
		$nrinfo[]=$value;
	}
	$r["rewardInfo"]=$nrinfo;
	$r["list"]=$myRank->getTopRank($start,$limit);
	$r["myscore"]=$myRank->score;
	$r["alluser"]=$myRank->getAllUser();
	$r["myrank"]=$myRank->getMyRank();
	$r["remainTime"]=TimeManager::getRemainTimeForWeeklyRank();
	if($r["myrank"]<=0){
		$r["alluser"]+=1;
		$r["myrank"]=$r["alluser"];
	}
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();

	return $r;
}
////////////////////////////////////////////////////////////////////////////////////


public static function checkweeklyreward($p){



	$memberID = $p["memberID"];
	
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	

	$rewardInfo = new CommonSetting("weeklyReward");
	$nrinfo=array();

	if(!is_array($rewardInfo->value) || empty($rewardInfo->value)){
		////LogManager::addLog("checkweeklyreward error ".var_export($rewardInfo,true));
		return ResultState::makeReturn(2002,"memberID");
	}	
	foreach ($rewardInfo->value as $key => $value) {
		$nrinfo[]=$value;
	}
	$r["rewardInfo"]=$nrinfo;
	$rewards=$rewardInfo->value;
	CommitManager::begin($memberID);
	$userInfo = UserData::create($memberID);
	
	if(!$userInfo->eventCheckWeek){
	
	
	}else if($userInfo->eventCheckWeek!=TimeManager::getCurrentWeekNoForRank()){
	
	//위의 if제거하고 아래 if주석풀면 계속 해서 확인함
	//if(1){
		$r["sendGift"]=false;
		$r["lastWeek"]=$userInfo->eventCheckWeek;
		//// 1 주간 스테이지 랭킹 확인
		$stagerank = new WeeklyScore($memberID,$userInfo->eventCheckWeek);

		//if($stagerank->isLoaded()){}... 
		$sResult["myrank"] = $stagerank->getMyRank();
		$sResult["alluser"] = $stagerank->getAllUser();
		$sResult["score"] = $stagerank->score;
		$sResult["count"] = $stagerank->count;
		if($sResult["myrank"]==-1){
			$sResult["alluser"]++;
			$sResult["myrank"]=$sResult["alluser"];
		}

		$rewardType = "";


		$pc = $sResult["myrank"]/$sResult["alluser"]*100;

		//LogManager::addLog("weeklyscore rank is".$sResult["myrank"]);
		//LogManager::addLog("weeklyscore pc is".$pc);
		if($sResult["myrank"]==1)$rewardType="t1";
		else if($sResult["myrank"]==2)$rewardType="t2";
		else if($sResult["myrank"]==3)$rewardType="t3";
		else if($sResult["myrank"]<=10)$rewardType="t10";
		else if($sResult["myrank"]<=50)$rewardType="t50";
		else if($pc<2)$rewardType="p1";
		else if($pc<6)$rewardType="p5";
		else if($pc<11)$rewardType="p10";
		else if($pc<21)$rewardType="p20";
		else if($pc<31)$rewardType="p30";
		else if($pc<41)$rewardType="p40";
		else if($pc<51)$rewardType="p50";
		else $rewardType="p100";

		
		$sResult["reward"]=null;
		$sResult["rewardList"]=$r["reward"]; //############## 임시 추후 삭제

		if($rewardType){
			$sResult["reward"]=$rewards[$rewardType]["reward"];
			$param["memberID"]=$memberID;
			$param["sender"]="GM";
			$param["content"]=$rewards[$rewardType]["content"];
			$param["exchangeID"]=$rewards[$rewardType]["exchangeID"];
			$param["reward"]=$rewards[$rewardType]["reward"];
			$sR = self::sendgiftboxhistory($param);
			
			if(!ResultState::successCheck($sR["result"])){
				CommitManager::setSuccess($memberID,false);
			}
			
			$r["sendGift"]=true;
		}

		$r["stage"]=$sResult;

		//// 1 주간 엔드리스모드 랭킹
		$endlessrank = new EndlessRank($memberID,$userInfo->eventCheckWeek);
		$eResult["myrank"] = $endlessrank->getMyRank();
		$eResult["alluser"] = $endlessrank->getAllUser();
		$eResult["score"] = $endlessrank->score;
		$eResult["victory"] = $endlessrank->victory;
		if($eResult["myrank"]==-1){
			$eResult["alluser"]++;
			$eResult["myrank"]=$eResult["alluser"];
		}

		
		$rewardType = "";
		$pc = $eResult["myrank"]/$eResult["alluser"]*100;
		//LogManager::addLog("EndlessRank rank is".$eResult["myrank"]);
		//LogManager::addLog("EndlessRank pc is".$pc);
		if($eResult["myrank"]==1)$rewardType="t1";
		else if($eResult["myrank"]==2)$rewardType="t2";
		else if($eResult["myrank"]==3)$rewardType="t3";
		else if($eResult["myrank"]<=10)$rewardType="t10";
		else if($eResult["myrank"]<=50)$rewardType="t50";
		else if($pc<2)$rewardType="p1";
		else if($pc<6)$rewardType="p5";
		else if($pc<11)$rewardType="p10";
		else if($pc<21)$rewardType="p20";
		else if($pc<31)$rewardType="p30";
		else if($pc<41)$rewardType="p40";
		else if($pc<51)$rewardType="p50";
		else $rewardType="p100";

		$eResult["reward"]=null;
		$eResult["rewardList"]=$r["reward"]; //##############  임시 추후 삭제

		if($rewardType){
			$eResult["reward"]=$rewards[$rewardType]["reward"];
			$param["memberID"]=$memberID;
			$param["sender"]="GM";
			$param["content"]=$rewards[$rewardType]["content"];
			$param["exchangeID"]=$rewards[$rewardType]["exchangeID"];
			$param["reward"]=$rewards[$rewardType]["reward"];
			$sR = self::sendgiftboxhistory($param);
			
			if(!ResultState::successCheck($sR["result"])){
				CommitManager::setSuccess($memberID,false);
			}
			
			$r["sendGift"]=true;
		}

		$r["endless"]=$eResult;


	}

	if($userInfo->eventCheckWeek!=TimeManager::getCurrentWeekNoForRank()){
		$userInfo->eventCheckWeek = TimeManager::getCurrentWeekNoForRank();
		CommitManager::setSuccess($memberID,$userInfo->save());
	}


	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	return $r;
}
////////////////////////////////////////////////////////////////////////////////////


public static function setstagescore($p){

	$stageNo = $p["stageNo"];
	$score=0;
	if(is_numeric($p["score"]))$score = $p["score"];
	$ss = new StageScore($stageNo,$p["memberID"]);
	$ss->memberID = $p["memberID"];
	$ss->stageNo = $stageNo;
	$ss->data = $p["data"];
	$ss->regDate = TimeManager::getCurrentDateTime();
	$ss->regTime = TimeManager::getTime();

	$r["update"]=false;
	if($ss->score<=$score){
		$r["update"]=true;
		$ss->score=$score;
		$ss->save();
	}
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;
}


public static function addweeklyandstagescore($p){
	$stageNo = $p["stageNo"];
	$myscore = $p["myScore"];
	$memberID = $p["memberID"];
	
	if(!$stageNo)return ResultState::makeReturn(2002,"stageNo");


	$myRank = new StageScore($memberID,$stageNo);
	$myRank->memberID = $memberID;
	$myRank->stageNo = $stageNo;
	$myRank->nick = $p["nick"];
	$myRank->flag = $p["flag"];
	$myRank->data = $p["data"];
	
	if($p["data"])$myRank->data = $p["data"];

	$r["isMax"]=false;

	if($p["getWeeklyRank"]){
		//주간랭킹등록

		$ws =new WeeklyScore($memberID,TimeManager::getCurrentWeekNoForRank());
		$ws->memberID = $memberID;
		$ws->data = $p["data"];
		$ws->nick = $p["nick"];
		$ws->flag = $p["flag"];
		$ws->regDate = TimeManager::getCurrentDateTime();
		$ws->regTime = TimeManager::getTime();
		$ws->regWeek = TimeManager::getCurrentWeekNoForRank();
		$ws->count = $ws->count+1;

		if($myscore){
			$ws->score = $ws->score + $myscore;
			$r["saveWeekly"]=$ws->save();
		}
	}

	if($myscore){
		
		//피스클리어
		$cmP["type"]="piece";
		$cmP["value"]=$stageNo;
		$cmP["memberID"]=$memberID;
		self::checkmissionevent($cmP);

		//미션이벤트체크
		$cmP["type"]="pieceScore";
		$cmP["value"]=$myscore;
		$cmP["value2"]=$stageNo;
		$cmP["memberID"]=$memberID;
		self::checkmissionevent($cmP);

		if($myRank->score<$myscore){
			LogManager::addLog("is max! prev:".$myRank->score."- now:".$myscore);
			$r["isMax"]=true;
			$myRank->score = $myscore;
			$myRank->save();
		}else{

		}

		//$myRank->score = $myscore;
	}else{
		$r["isMax"]==true;
	}

	// 스테이지랭킹 
	if($r["isMax"]==true){
		$r["list"] = $myRank->getTop4();
		$r["alluser"] = $myRank->getAllUser();
		$r["myrank"] = $myRank->getMyRank();
		$r["myscore"] = $myRank->score;
	}else{
		LogManager::addLog("is not max! weekly score gogo");
		$r["list"] = $ws->getTopRank(1,4);
		$r["alluser"] = $ws->getAllUser();
		$r["myrank"] = $ws->getMyRank();
		$r["myscore"] = $ws->score;
	}

	$r["stageNo"] = $stageNo;
	
	if($r["myrank"]<=0 || $r["myrank"]>$r["alluser"])$r["myrank"]=$r["alluser"];

	$r["result"]=ResultState::successToArray();

	return $r;

}

public static function getstagerankbyalluser($p){

	$stageNo = $p["stageNo"];
	$myscore = $p["myScore"];
	$memberID = $p["memberID"];
	
	if(!$stageNo)return ResultState::makeReturn(2002,"stageNo");


	$myRank = new StageScore($memberID,$stageNo);
	$myRank->memberID = $memberID;
	$myRank->stageNo = $stageNo;
	$myRank->nick = $p["nick"];
	$myRank->flag = $p["flag"];
	$myRank->data = $p["data"];
	
	if($p["data"])$myRank->data = $p["data"];

	$r["isMax"]=false;
	
	if($myscore){
		//피스클리어
		$cmP["type"]="piece";
		$cmP["value"]=$stageNo;
		$cmP["memberID"]=$memberID;
		self::checkmissionevent($cmP);

		//미션이벤트체크
		$cmP["type"]="pieceScore";
		$cmP["value"]=$myscore;
		$cmP["value2"]=$stageNo;
		$cmP["memberID"]=$memberID;
		self::checkmissionevent($cmP);

		if($myRank->score<$myscore){
			LogManager::addLog("is max! prev:".$myRank->score."- now:".$myscore);
			$r["isMax"]=true;
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
}
//////////////////////////////////////////

public static function getpuzzleinfo($p){
	
	$puzzle = new Puzzle($p[no],$p[order]);

	if(!$puzzle->isLoaded()){
		return ResultState::makeReturn(ResultState::GDDONTFIND);
	}
	
	if($puzzle->version==$p[version]){
		$r[version]=$p[version];
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		return $r;
	}

	$puzzleInfo = $puzzle->getArrayDataForClient();


	
	$puzzleInfo["result"]=ResultState::successToArray();
	return $puzzleInfo;
}

/////////////////////////////////////////////////////

public static function updatepuzzlehistory($p){	
	
	$memberID=$p["memberID"];
	$puzzleNo=$p["puzzleNo"];
	
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$puzzleNo)return ResultState::makeReturn(2002,"puzzleNo");
	

	//보내기

	$r["sendGift"]=false;
	CommitManager::begin($memberID);
	$obj = new PuzzleHistory($memberID,$puzzleNo);
	$obj->memberID=$memberID;
	$obj->puzzleNo = $puzzleNo;
	if($p["updateOpenDate"] && !$obj->openDate)$obj->openDate=TimeManager::getCurrentDateTime();
	if($p["updateClearDate"] && !$obj->clearDate){
		$obj->clearDate=TimeManager::getCurrentDateTime();
		//카드선물하기
		$puzzle = new Puzzle(null,$p["puzzleNo"]);
		$clearReward =& $puzzle->getRef("clearReward");

		if($clearReward["normal"]){
			$cp["memberID"]=$memberID;
			$cp["cardNo"]=$clearReward["normal"];
			$cp["isMorphing"]=true;
			$cr = self::updatecardhistory($cp);

			$r["sendGift"]=true;
			$r["giftCardNo"]=$clearReward["normal"];
			if(!ResultState::successCheck($cr["result"])){
				CommitManager::setSuccess($memberID,false);
			}

			//미션이벤트체크
			$cmP["type"]="puzzleClear";
			$cmP["value"]=$puzzle->order;
			$cmP["memberID"]=$memberID;
			self::checkmissionevent($cmP);

			$r["giftData"]=$cr["data"];
		}

	}

	if($p["updatePerfectDate"] && !$obj->perfectDate){
		$obj->perfectDate=TimeManager::getCurrentDateTime();
		//카드선물하기
		$puzzle = new Puzzle($p["puzzleNo"]);
		$clearReward =& $puzzle->getRef("clearReward");
		
		if($clearReward["perfect"]){
			$cp["memberID"]=$memberID;
			$cp["cardNo"]=$clearReward["perfect"];
			$cp["isMorphing"]=true;
			$cr = self::updatecardhistory($cp);

			$r["sendGift"]=true;
			$r["giftCardNo"]=$clearReward["perfect"];
			if(!ResultState::successCheck($cr["result"])){
				CommitManager::setSuccess($memberID,false);
			}

			//미션이벤트체크
			$cmP["type"]="puzzlePerfectClear";
			$cmP["value"]=$puzzle->order;
			$cmP["memberID"]=$memberID;
			self::checkmissionevent($cmP);

			$r["giftData"]=$cr["data"];
		}
	}

	if($p["openType"])$obj->openType=$p["openType"];

	if(!$obj->save()){
		CommitManager::setSuccess($memberID,false);
	}

	if(CommitManager::commit($memberID)){
		$r["data"]=$obj->getArrayData(true);
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	return $r;
}



public static function getpuzzlehistory($p){	
	
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
    while($rData = PuzzleHistory::getRowByQueryWithShardKey("where memberID='".$memberID."'",$memberID)){
    	$dataList[]=$rData;
    }


	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;
}
/////////////////////////////////////////


public static function updatepiecehistory($p){	
	
	$memberID=$p["memberID"];
	$pieceNo=$p["pieceNo"];
	$clearRank = $p["updateClearDate"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$pieceNo)return ResultState::makeReturn(2002,"pieceNo");
	
	//보내기
	$obj = new PieceHistory($memberID,$pieceNo);
	
	if(!$obj->isLoaded()){
		$obj->tryCount=0;
		$obj->clearCount=0;
	}

	$obj->memberID = $memberID;
	$obj->pieceNo = $pieceNo;
	if($p["openDate"] && !$obj->openDate)$obj->openDate = TimeManager::getCurrentDateTime();
	if(($p["firstClearDate"] && !$obj->firstClearDate) || ($p["clearCount"]>0 && !$obj->firstClearDate))$obj->firstClearDate = TimeManager::getCurrentDateTime();
	if($p["tryCount"])$obj->tryCount= $p["tryCount"];
	if($p["clearCount"] && !$obj->clearCount)$obj->clearCount = $p["clearCount"];
	if(is_array($p["clearDateList"])){
		
		$newlistdata = array(0,0,0,0);
		if(!$obj->clearDateList)$obj->clearDateList=array(0,0,0,0);
		else if(!is_array($obj->clearDateList))$obj->clearDateList = json_decode($obj->clearDateList,true);
		foreach ($obj->clearDateList as $key => $value) {
			if($value<=0 && ($p["clearDateList"][$key]>0)){
				$newlistdata[$key]=TimeManager::getCurrentDateTime();
			}else if($value){
				$newlistdata[$key]=$value;
			}
		}
	}
	$obj->clearDateList=$newlistdata;

	if($p["openType"])$obj->openType = $p["openType"];

	if($obj->save()){
		$r["data"]=$obj->getArrayData(true);
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014);
	}
	
	return $r;
}




public static function getpiecehistory($p){	
	
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
	//echo "fuck this??";
    while($rData = PieceHistory::getRowByQueryWithShardKey("where memberID='".$memberID."'",$memberID)){

		//echo "fuck this2??";
    	$rData["clearDateList"]=json_decode($rData["clearDateList"],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		$dataList[]=$rData;
    }


	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;
}
////////////////////////////////////////

public static function updatecardhistory($p){	
	
	$memberID=$p["memberID"];
	$cardNo=$p["cardNo"];
	$addCount = $p["addCount"];
	$includeCardInfo = $p["includeCardInfo"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$cardNo)return ResultState::makeReturn(2002,"cardNo");
	

	$cardInfo = new Card($cardNo);
	if(!$cardInfo->isLoaded()){
		return ResultState::makeReturn(ResultState::GDDONTFIND);
	}

	if($addCount>=0){
		//미션이벤트체크
		$cmP["type"]="card";
		$cmP["value"]=$cardInfo->serial;
		$cmP["memberID"]=$memberID;
		self::checkmissionevent($cmP);
	}

	//보내기
	$obj = new CardHistory($memberID,$cardNo);
	$obj->memberID = $memberID;
	$obj->cardNo = $cardNo;
	
	$total=0;
	if(!$obj->isLoaded()){
		$obj->level = $cardInfo->level;
	}else{
		$total = $obj->count;
	}
	$cnt=0;
	if($addCount){
		$cnt=$addCount;
		if(!$obj->isLoaded())$obj->count=$addCount;
		else $obj->count+=$addCount;

		if($obj->count<=0){
			return ResultState::makeReturn(ResultState::GDPROPERTYISMINUS);
		}
	}



	if($p["updateTakeDate"] || !$obj->isLoaded())$obj->takeDate=TimeManager::getCurrentDateTime();
	if($p["comment"])$obj->comment=addslashes($p["comment"]);
	if($p["isMorphing"])$obj->isMorphing=$p["isMorphing"];
	
	if($p["level"] && $cardInfo->category!='leader')$obj->level = $p["level"];

	//LogManager::addLog($obj->comment);
	//LogManager::addLog($obj->getArrayData(true));

	CommitManager::begin($memberID);

	CommitManager::setSuccess($memberID,$obj->save());
	//if(!$obj->save())//return ResultState::makeReturn(2014,"dont save");


	if($cnt){
		$param["memberID"]=$memberID;
		$param["type"]="cd";
		$param["count"]=$cnt;
		$param["total"]=$total+$cnt;
		$param["statsID"]="cardNo";
		$param["statsValue"]=$cardNo;
		$param["content"]=$p["content"];
		$param["sender"]="user";
		$param["exchangeID"]=$p["exchangeID"];

		$result = self::updateuserpropertyhistory($param);	
		if(!ResultState::successCheck($result["result"])){
			//LogManager::addLog("save fail another prop");
			CommitManager::setSuccess($memberID,false);
		}
	}

	$r["data"]=$obj->getArrayData(true);
	if($includeCardInfo)$r["cardInfo"]=$cardInfo->getArrayDataForClient();
	$r["write"]="success";	

	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
		return $r;
	}else{
		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
		return $r;
	}
}



public static function getcardhistory($p){	
	
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
    while($rData = CardHistory::getRowByQueryWithShardKey("where memberID='".$memberID."'",$memberID)){
    	$rData["comment"]=stripslashes($rData["comment"]);
		$dataList[]=$rData;
    }

    $q = Card::getQueryResultWithShardKey("select max(`serial`) from ".Card::getDBTable()." where `category`='normal' or `category`='nPuzzle'",1);
    $d = mysql_fetch_array($q);
    $r["lastCardNo"]=$d[0];


	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;
}


/////////////////////////////////////////////////////////////////////////////////////////////

public static function addproperty($p){		
	$memberID=$p["memberID"];
	$count=$p["count"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$count)return ResultState::makeReturn(2002,"count");
	if(!$p["type"])return ResultState::makeReturn(2002,"type");

	CommitManager::begin($memberID);

	$userProp = new UserStorage($memberID);
	
	if(!$userProp->m__userIndex->isLoaded())return ResultState::makeReturn(2005,"dont find user");

	$userHistory = new UserPropertyHistory($memberID);
	$userHistory->memberID = $memberID;
	$userHistory->count = $p["count"];
	$userHistory->type = $p["type"];
	$userHistory->count = $p["count"];
	$userHistory->statsID = $p["statsID"];
	$userHistory->statsValue = $p["statsValue"];
	$userHistory->total = $userProp->{$p["type"]}+$p["count"];
	$userHistory->content = $p["content"];
	$userHistory->sender = $p["sender"];
	$userHistory->regDate = TimeManager::getCurrentDateTime();

	$userProp->{$p["type"]} += $count;

	$r["data"]=array("type"=>$p["type"],"count"=>$userProp->{$p["type"]});	

	if($userProp->{$p["type"]}<0){
		$r["result"] = ResultState::toArray(2015);
		$r["minusType"]=$userProp->type;
		$r["minusCount"]=$userProp->count-$count;
		return $r;
	}

	if(!$userProp->save() or !$userHistory->save()){
		CommitManager::rollback($memberID);
		$r["result"] = ResultState::toArray(2014,"Dont Save gold");
		return $r;
	}

	CommitManager::commit($memberID);
	
	$r["result"]=ResultState::successToArray();
	return $r;
}
/////////////////////////////////////////////////////////////////////////////////////////////

public static function updateuserpropertyhistory($p){		
	//LogManager::addLog("== start == updateuserpropertyhistory =================================".json_encode($p));
	
	$memberID=$p["memberID"];
	$count=$p["count"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$count)return ResultState::makeReturn(2002,"count");
	if(!$p["type"])return ResultState::makeReturn(2002,"type");



	$userHistory = new UserPropertyHistory($memberID);
	$userHistory->memberID = $memberID;
	$userHistory->count = $p["count"];
	$userHistory->type = $p["type"];
	$userHistory->count = $p["count"];
	$userHistory->statsID = $p["statsID"];
	$userHistory->statsValue = $p["statsValue"];
	$userHistory->total = $p["total"];
	$userHistory->content = $p["content"];
	$userHistory->sender = $p["sender"];
	$userHistory->exchangeID = $p["exchangeID"];
	$userHistory->regDate = TimeManager::getCurrentDateTime();

	//LogManager::addLog("== save start == updateuserpropertyhistory =================================");
	
	if(!$userHistory->save()){
		$r["result"] = ResultState::toArray(2014,"Dont Save gold");
		return $r;
	}

	
	$r["result"]=ResultState::successToArray();
	return $r;
}


////////////////////////////////////////

public static function changeuserproperties($p){

	$list = $p["list"];
	$memberID=$p["memberID"];
	if(!$list)return ResultState::makeReturn(2002,"list");
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	//LogManager::addLog("changeuserproperties ".json_encode($list));

	CommitManager::begin($memberID);

	$userStorage = new UserStorage($memberID);
	$isRUp = false;
	foreach ($list as $key => $value) {
		$param=array();
		if($value["type"]=="fr"){
			$value["type"]="r";
			$value["isPurchase"]=false;
		}else if($value["type"]=="pr"){
			$value["type"]="r";
			$value["isPurchase"]=true;
		}
		
		//젬인경우 특수처리
		if(!$value["type"] || $value["count"]==0){
			continue;
		}else if($value["type"]=="r"){
			//take
			if($value["count"]>=0){
				if($value["isPurchase"]){
					$value["type"]="pr";
					$userStorage->pr += $value["count"];
					$value["total"]=$userStorage->pr;
				}else{
					$value["type"]="fr";
					$userStorage->fr += $value["count"];
					$value["total"]=$userStorage->fr;
				}

				$param["memberID"]=$memberID;
				$param["type"]=$value["type"];
				$param["count"]=$value["count"];
				$param["total"]=$value["total"];
				$param["statsID"]=$value["statsID"];
				$param["statsValue"]=$value["statsValue"];
				$param["content"]=$value["content"];
				$param["exchangeID"]=$p["exchangeID"];
				$param["sender"]="user";

				$result = self::updateuserpropertyhistory($param);	
				
				if(!ResultState::successCheck($result["result"])){
					CommitManager::setSuccess($memberID,false);
				}

				$rs[]=array("type"=>"r","count"=>($userStorage->fr+$userStorage->pr));
				$isRUp=true;
			//use
			}else if($value["count"]<0){

				//fRuby와 pRuby읽어옴
				//fRuby먼저 깍고 fRuby모자를 경우 pRuby깍음. 근데 둘중하나라도 마이너스면 에러
				if($userStorage->fr+$userStorage->pr+$value["count"]<0){
					
					$r["result"]=ResultState::toArray(2015);
					$rs[]=array("type"=>"r","count"=>($userStorage->fr+$userStorage->pr));
					$minusProperty="r";
					$minusPropertyValue = $userStorage->fr+$userStorage->pr;
					CommitManager::setSuccess($memberID,false);
				}else{

					//무료젬가 충분하다면 무료젬만 사용
					if($value["count"]+$userStorage->fr>=0){
						$userStorage->fr = $userStorage->fr+$value["count"]; 

						$userHistory = new UserPropertyHistory($memberID);
						$userHistory->memberID = $memberID;
						$userHistory->count = $value["count"];
						$userHistory->type = "fr";
						$userHistory->statsID = $value["statsID"];
						$userHistory->statsValue = $value["statsValue"];
						$userHistory->total = $userStorage->fr;
						$userHistory->content = $value["content"];
						$userHistory->sender = "user";
						$userHistory->exchangeID=$p["exchangeID"];
						$userHistory->regDate = TimeManager::getCurrentDateTime();
						CommitManager::setSuccess($memberID,$userHistory->save());

					//무료젬가 부족하면 뮤료젬=0, 남은건 유료젬에서 차감
					}else{
						if($userStorage->fr>0){
							$userHistory1 = new UserPropertyHistory($memberID);
							$userHistory1->memberID = $memberID;
							$userHistory1->count = $userStorage->fr*-1;
							$userHistory1->type = "fr";
							$userHistory1->statsID = $value["statsID"];
							$userHistory1->statsValue = $value["statsValue"];
							$userHistory1->total = 0;
							$userHistory1->content = $value["content"];
							$userHistory1->sender = "user";
							$userHistory1->exchangeID=$p["exchangeID"];
							$userHistory1->regDate = TimeManager::getCurrentDateTime();
							CommitManager::setSuccess($memberID,$userHistory1->save());
						}

						$userStorage->pr=$userStorage->pr+$userStorage->fr+$value["count"];
						$userStorage->fr=0;

						{
							$userHistory2 = new UserPropertyHistory($memberID);
							$userHistory2->memberID = $memberID;
							$userHistory2->count = $userStorage->fr+$value["count"];
							$userHistory2->type = "pr";
							$userHistory2->statsID = $value["statsID"];
							$userHistory2->statsValue = $value["statsValue"];
							$userHistory2->total = $userStorage->pr;
							$userHistory2->content = $value["content"];
							$userHistory2->sender = "user";
							$userHistory2->exchangeID=$p["exchangeID"];
							$userHistory2->regDate = TimeManager::getCurrentDateTime();
							CommitManager::setSuccess($memberID,$userHistory2->save());
						}

						//CommitManager::setSuccess($memberID,$fRuby->save() && $pRuby->save());
					}

					$rs[]=array("type"=>"r","count"=>($userStorage->fr+$userStorage->pr));
					$isRUp = true;
				}

				//아니면 적용

			}

		//카드획득
		}else if($value["type"]=="cd"){
			$ucp["memberID"]=$memberID;
			$ucp["cardNo"]=$value["count"];
			$ucp["updateTakeDate"]=true;
			$ucp["addCount"]=1;
			$ucp["statsID"]=$value["statsID"];
			$ucp["exchangeID"]=$p["exchangeID"];
			$ucp["content"]=$value["content"];;
			$ucp["includeCardInfo"]=true;
			$result = self::updatecardhistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);
		//카드획득
		}else if($value["type"]=="dhcd"){
			$grade = $value["count"];
			if($grade==0 || $grade>4)$grade=rand(1,4);

			$cs = new CommonSetting("gachaCardNoAdd");
			$maxcard = CardHistory::getRowByQueryWithShardKey("where `memberID`='".$memberID."' and cardNo<10000",$memberID,"max(cardNo) as maxcardno");
			$mymaxcardNo = $maxcard["maxcardno"];
			$maxcardNo = $mymaxcardNo+$cs->value;
			
			while($obj = Card::getObjectbyQuery("where `grade`='$grade' and `category`='normal' and `no`>'$mymaxcardNo' and no<'$maxcardNo' order by rand() limit 1")){
				$ucp["cardNo"]=$obj->no;
			}

			//카드가 없으면 전체카드랜덤으로
			if(!$ucp["cardNo"]){
				while($obj = Card::getObjectbyQuery("where `grade`='$grade' and `category`='normal' order by no asc")){
					$mycard = new CardHistory($memberID,$obj->no);
					if(!$mycard->isLoaded()){
						$ucp["cardNo"]=$obj->no;
						break;
					}
				}
			}

			//카드가 없으면 전체카드랜덤으로
			if(!$ucp["cardNo"]){
				while($obj = Card::getObjectbyQuery("where `grade`='$grade' and `category`='normal' order by rand() limit 1")){
					$ucp["cardNo"]=$obj->no;
				}
			}
			$ucp["memberID"]=$memberID;
			$ucp["updateTakeDate"]=true;
			$ucp["addCount"]=1;
			$ucp["statsID"]=$value["statsID"];
			$ucp["exchangeID"]=$p["exchangeID"];
			$ucp["content"]=$value["content"];;
			$ucp["includeCardInfo"]=true;
			$result = self::updatecardhistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);
		}else if($value["type"]=="gncd"){
			$grade = $value["count"];
			if($grade==0 || $grade>4)$grade=rand(1,4);

			$maxcard = CardHistory::getRowByQueryWithShardKey("where `memberID`='".$memberID."' and cardNo<10000",$memberID,"max(cardNo) as maxcardno");
			$mymaxcardNo = $maxcard["maxcardno"];

			$cs = new CommonSetting("gachaCardNoAdd");
			$maxcardNo = $mymaxcardNo+$cs->value;

			while($obj = Card::getObjectbyQuery("where `grade`='$grade' and `category`='normal' and no<$maxcardNo order by rand() limit 1")){
				$ucp["cardNo"]=$obj->no;
			}

			//카드가 없으면 전체카드랜덤으로
			if(!$ucp["cardNo"]){
				while($obj = Card::getObjectbyQuery("where `grade`='$grade' and `category`='normal' order by rand() limit 1")){
					$ucp["cardNo"]=$obj->no;
				}
			}
			$ucp["memberID"]=$memberID;
			$ucp["updateTakeDate"]=true;
			$ucp["addCount"]=1;
			$ucp["statsID"]=$value["statsID"];
			$ucp["exchangeID"]=$p["exchangeID"];
			$ucp["content"]=$value["content"];;
			$ucp["includeCardInfo"]=true;
			$result = self::updatecardhistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);
		//카드마이너스
		}else if($value["type"]=="cdm"){
			$ucp["memberID"]=$memberID;
			$ucp["cardNo"]=$value["count"];
			$ucp["updateTakeDate"]=true;
			$ucp["addCount"]=-1;
			$ucp["statsID"]=$value["statsID"];
			$ucp["exchangeID"]=$p["exchangeID"];
			$ucp["content"]=$value["content"];;
			$result = self::updatecardhistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);
		//피스획득
		}else if($value["type"]=="pc"){
			$ucp["memberID"]=$memberID;
			$ucp["pieceNo"]=$value["count"];
			$ucp["openType"]=$value["content"];
			$ucp["openDate"]=true;
			$result = self::updatepiecehistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);
		//퍼즐획득
		}else if($value["type"]=="pz"){
			$ucp["memberID"]=$memberID;
			$ucp["puzzleNo"]=$value["count"];
			$ucp["openType"]=$value["content"];
			$ucp["updateOpenDate"]=true;
			$result = self::updatepuzzlehistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);
		//미사일업글
		}else if($value["type"]=="cu"){
			$userInfo = UserData::create($memberID);
			$userInfo->level=$userInfo->level+1;
			LogManager::addLog("aaaaa ".$userInfo->level);
			if($userInfo->save()){
				$result = array("result"=>ResultState::toArray(ResultState::GDSUCCESS));
			}else{
				$result = array("result"=>ResultState::toArray(ResultState::GDMYSQLQUERY));
			}

			$rs[]=array("type"=>$value["type"],"result"=>$result);

			// $char = new CharacterHistory($memberID,$value["count"]);
			// $ucp["memberID"]=$memberID;
			// $ucp["levelup"]=true;
			// $ucp["characterNo"]=$value["count"];
			// $result = self::updatecharacterhistory($ucp);
			// $rs[]=array("type"=>$value["type"],"result"=>$result);

		//캐릭터획득
		}else if($value["type"]=="cp"){
			$ucp["memberID"]=$memberID;
			$ucp["characterNo"]=$value["count"];
			$ucp["useExchange"]=true;
			$result = self::updatecharacterhistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);
		//다른재화
		}else{
			$pType = $value["type"];

			$userStorage->{$pType} += $value["count"]; 

			if($userStorage->{$pType}<0){
				$r["result"]=ResultState::toArray(2015);
				$minusProperty=$pType;
				$minusPropertyValue = $userStorage->{$pType}-$value["count"];
				CommitManager::setSuccess($memberID,false);
			}else{
				//LogManager::addLog("another propertys =========================");
				// $param["memberID"]=$memberID;
				// $param["type"]=$value["type"];
				// $param["count"]=$value["count"];
				// $param["total"]=$userStorage->{$pType};
				// $param["statsID"]=$value["statsID"];
				// $param["statsValue"]=$value["statsValue"];
				// $param["content"]=$value["content"];
				// $param["sender"]="user";
				// $param["exchangeID"]=$p["exchangeID"];

				// $result = self::updateuserpropertyhistory($param);	
				// if(!ResultState::successCheck($result["result"])){
				// 	//LogManager::addLog("save fail another prop");
				// 	CommitManager::setSuccess($memberID,false);
				// }

				$rs[]=array("type"=>$pType,"count"=>$userStorage->{$pType});
			}
			
		}

	
		if($value["type"]!="r" && $value["type"]!="fr" && $value["type"]!="pr"  && $value["type"]!="cd"  && $value["type"]!="cdm"  && $value["type"]!="dhcd" && $value["type"]!="gncd"){
			$param["memberID"]=$memberID;
			$param["type"]=$value["type"];
			$param["count"]=$value["count"];
			$param["total"]=$userStorage->{$pType};
			$param["statsID"]=$value["statsID"];
			$param["statsValue"]=$value["statsValue"];
			$param["content"]=$value["content"];
			$param["sender"]="user";
			$param["exchangeID"]=$p["exchangeID"];

			$result = self::updateuserpropertyhistory($param);	
			if(!ResultState::successCheck($result["result"])){
				//LogManager::addLog("save fail another prop");
				CommitManager::setSuccess($memberID,false);
			}
		}


	}
	
	if(!$isRUp)$rs[]=array("type"=>"r","count"=>($userStorage->fr+$userStorage->pr));
	
	if(!$userStorage->save()){
		//LogManager::addLog("faild save userStorage ".mysql_error());
		CommitManager::setSuccess($memberID,false);
	}



	if(CommitManager::commit($memberID))$r["result"]=ResultState::successToArray();
	else{	
		if(!$r["result"])$r["result"]=ResultState::toArray(2013,"dont success");
	}

	
	$r["list"]=$rs;

	if($minusProperty)
	{
		$r["result"]=ResultState::toArray(2015,"dont success");
		$r["minusType"]=$minusProperty; $r["minusCount"]=$minusPropertyValue;
		unset($r["list"]);
	}

	return $r;

	
}


////////////////////////////////////////////////////////////////////////////////////////////

public static function getuserproperties($p){

	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$userStorage = new UserStorage($memberID);

	$data = $userStorage->getArrayData();

	$list=array();
	foreach ($data as $key => $value) {
		if($key=="no" || $key=="memberID" || $key=="pr" || $key=="fr")continue;
		$list[] = array("type"=>$key,"count"=>$value);
	}

	$list[]=array("type"=>"r","count"=>$userStorage->fr+$userStorage->pr);

	$r["list"]=$list;
	$r["result"]=ResultState::successToArray();

	return $r;
	////////////////////////////////////////////////////////////////////////////////////
	$dataList = array();
	$checkRuby = false;
	$fRuby=array();
	$pRuby=array();
    while($rData = UserProperty::getRowByQueryWithShardKey("where memberID='".$memberID."'",$memberID)){
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

public static function starttransaction($p){
	$memberID = $p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	
	return ResultState::makeReturn(ResultState::GDSUCCESS);
}

public static function transaction($p){
	$memberID = $p["memberID"];
	$cResult=array();
	$results = array();

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$p["actions"])return ResultState::makeReturn(2002,"actions");

	CommitManager::begin($memberID);
	
	foreach ($p["actions"] as $key => $value) {
		$a = $value["action"];
		$p = $value["param"];
		$p["memberID"]=$memberID;
		if(method_exists($this,$a)){
			$r = self::$a($p);
			$results[]=$r;
			if(!ResultState::successCheck($r["result"])){
				CommitManager::setSuccess($memberID,false);
				break;
			}
		}
	}


	if(CommitManager::isSuccess($memberID)){
		CommitManager::commit($memberID);
		$cResult["list"]=$results;
		$cResult["result"]=ResultState::successToArray();
	}else{
		CommitManager::rollback($memberID);
		return ResultState::makeReturn(2013,"Fail Tranjection");
	}

	return $cResult;
}

////////////////////////////////////////////////////////////////////////////////////

public static function getarchivementlist($p){
	$listVer = kvManager::get("arcListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		CommitManager::$m_passFunc=true;
		
		return $r;
	}

	$dataList = array();
	$dataList[0]=0;
	$aCnt=0;
	$lNo=-1;
	$aNo=0;

	$result = Archivement::getQueryResult("select ".Archivement::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".Archivement::getDBTable()." left join ".Exchange::getDBTable()." on ".Archivement::getDBTable().".exchangeID=".Exchange::getDBTable().".id order by groupNo");
	LogManager::addLog("select ".Archivement::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".Archivement::getDBTable()." left join ".Exchange::getDBTable()." on ".Archivement::getDBTable().".exchangeID=".Exchange::getDBTable().".id order by groupNo");
	
	while($rData = mysql_fetch_assoc($result)){

   // while($rData = Archivement::getRowByQuery("order by groupNo")){
    	$aNo = $rData["groupNo"];
    	if($aNo!=$lNo){$aCnt++;}
    	$rData["reward"]=json_decode($rData["reward"],true);
    	$rData["content"]= CurrentUserInfo::getLocalizedValueInData(json_decode($rData["content"],true));
		$rData["title"]= CurrentUserInfo::getLocalizedValueInData(json_decode($rData["title"],true));
		$dataList[$aCnt][]=$rData;

		$lNo=$rData["groupNo"];
    }

	$r["list"]=$dataList;
	$r["version"]=$listVer;
	$r["result"]=ResultState::successToArray();
	return $r;

}



public static function getarchivementhistory($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
    while($rData = ArchivementHistory::getRowByQueryWithShardKey("where memberID=".$memberID,$memberID)){
		$dataList[]=$rData;
    }

	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;


}

public static function updatearchivementhistory($p){	
	
	$memberID=$p["memberID"];
	$archiveID=$p["archiveID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$archiveID)return ResultState::makeReturn(2002,"archiveID");
	
	//보내기
	$obj = new ArchivementHistory($memberID,$archiveID);
	$obj->memberID=$memberID;
	$obj->archiveID=$archiveID;
	if($p["updateClearDate"] && !$obj->clearDate)$obj->clearDate=TimeManager::getCurrentDateTime();
	if($p["updateRewardDate"] && !$obj->rewardDate)$obj->rewardDate=TimeManager::getCurrentDateTime();
	if($p["count"])$obj->count=$p["count"];

	if($obj->save()){
		$r = $obj->getArrayData(true);
		$r["result"]=ResultState::successToArray();

	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	
	return $r;
}

/////////////////////////////////////////////////////////////////////////////////////////

public static function getgiftboxhistory($p){
	$memberID=$p["memberID"];
	$includeConfirm = $p["includeConfirm"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");


	$lastDay = TimeManager::getDateTime(TimeManager::getTime()-60*60*24*30);

	$where = "where memberID=".$memberID;

	//30일 지난데이터는 삭제
    GiftBoxHistory::removeRowByQueryWithShardKey("where memberID=".$user->memberID." and regDate<$lastDay",$memberID);

	if($includeConfirm)$where = $where." and regDate>$lastDay order by regDate desc";
	else $where = $where." and confirmDate='' and regDate>$lastDay order by regDate desc";
	$dataList = array();
    while($rData = GiftBoxHistory::getRowByQueryWithShardKey($where,$memberID)){
    	if($rData["reward"]){
    		$rData["reward"] = json_decode($rData["reward"],true);
			
			if($rData["exchangeList"]){
				$rData["exchangeList"] = json_decode($rData["exchangeList"],true);
				$rData["reward"] = Exchange::mergeCustom($rData["reward"],$rData["exchangeList"]);
				unset($rData["exchangeList"]);
			}
    	
    	}
		$dataList[]=$rData;
    }



	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;


}

public static function updategiftboxhistory($p){	
	
	$memberID=$p["memberID"];
	$giftBoxNo=$p["giftBoxNo"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$giftBoxNo)return ResultState::makeReturn(2002,"giftBoxNo");
	
	//보내기
	$obj = new GiftBoxHistory($memberID,$giftBoxNo);

	if(!$obj->isLoaded())return ResultState::makeReturn(2002,"dont find message");

	if($p["updateConfirmDate"])$obj->confirmDate=TimeManager::getCurrentDateTime();

	if($obj->save()){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	
	return $r;
}

public static function sendgiftboxhistory($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$p["sender"])$p["sender"]="user";
	
	if($p["template"]){
		$msgTemp = new CommonSetting($p["template"]);

		if($msgTemp->isLoaded()){
			$msgData =& $msgTemp->getRef("value");

			$p=array_merge($p,$msgData);
			if($p["custom"]){
				for($i=0;$i<count($p["custom"]);$i++){
					$cnt=$i+1;
					$p["content"]=str_replace("[p".$cnt."]",$p["custom"][$i],$p["content"]);
				}
			}
		}

	}

	$gb = new GiftBoxHistory($memberID);
	$gb->memberID = $memberID;
	$gb->sendMemberID = $p["sendMemberID"];
	$gb->sender = $p["sender"];
	$gb->content = $p["content"];
	$gb->data = $p["data"];

	if($p["exchangeID"]){
		$exchange = new Exchange($p["exchangeID"]);
		$gb->exchangeID = $p["exchangeID"];
		$gb->exchangeList = $p["exchangeList"];
		$gb->reward = $exchange->list;
		$r["reward"]=$gb->reward;
	}


	$gb->regDate = TimeManager::getCurrentDateTime();

	if($gb->save()){
		$r["result"]=ResultState::successToArray();
	
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	return $r;
}

public static function confirmgiftboxhistory($p){

	$memberID=$p["memberID"];
	$giftBoxNo=$p["giftBoxNo"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$giftBoxNo)return ResultState::makeReturn(2002,"giftBoxNo");
	
	$obj = new GiftBoxHistory($memberID,$giftBoxNo);

	if(!$obj->isLoaded())return ResultState::makeReturn(2003,"dont find message");

	if($obj->confirmDate>0)return ResultState::makeReturn(2003,"dont find message");

	CommitManager::begin($memberID);

	$cResult = array();
	// if(!$obj->exchangeID){
	// 	$_reward = &$obj->getRef("reward");
	// 	for($i=0;$i<count($_reward);$i++){
	// 		$_reward[$i]["content"]="giftbox";
	// 		$_reward[$i]["statsID"]="giftbox";
	// 	//LogManager::addLog("reward is 1 ".json_encode($_reward[$i]));
	// 	}
	// 	//LogManager::addLog("reward is ".json_encode($obj->reward));

	// 	$param["list"]=$obj->reward;
	// 	$param["memberID"]=$memberID;
	// 	$cResult = self::changeuserproperties($param);
	// }else{


	if($obj->exchangeID){
		$param["memberID"]=$memberID;
		$param["exchangeID"]=$obj->exchangeID;
		$param["list"]=$obj->getRef("exchangeList");
		LogManager::addLog("cardexchangeparam - ".json_encode($param));
		
		$cResult = self::exchange($param);
		LogManager::addLog("cardexchange - ".json_encode($cResult));
		//}


		if(ResultState::successCheck($cResult["result"])){
			CommitManager::setSuccess($memberID,true);
		}else{
			CommitManager::setSuccess($memberID,false);
		}
	}

	$obj->confirmDate=TimeManager::getCurrentDateTime();

	if($obj->save()){
		CommitManager::setSuccess($memberID,true);
		$r["result"]=ResultState::successToArray();
		$r["list"]=$cResult["list"];
	}else{
		CommitManager::setSuccess($memberID,false);
		$r["result"]=ResultState::toArray(2014,"dont save");
	}

	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
		$r["list"]=$cResult["list"];
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	
	return $r;
}

public static function confirmallgiftboxhistory($p){

	$memberID=$p["memberID"];
	$giftBoxNoList=$p["giftBoxNoList"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	
	
	CommitManager::begin($memberID);

	$cResult = array();

	$param["memberID"]=$memberID;
	$param["exchangeIDList"]=GiftBoxHistory::getAllExchangeID($memberID);
	if(count($param["exchangeIDList"])<=0)return ResultState::makeReturn(ResultState::GDSUCCESS);
	$cResult = self::exchangebylist($param);
	
	CommitManager::setSuccess($memberID,GiftBoxHistory::confirmAll($memberID));

	$error = "";
	$error .= mysql_error();
	if(ResultState::successCheck($cResult["result"])){
		CommitManager::setSuccess($memberID,true);
	}else{
		//LogManager::addLog("cresult is false ".json_encode($cResult));
		CommitManager::setSuccess($memberID,false);
	}
	
	$error .= mysql_error();
	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
		$r["list"]=$cResult["list"];
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save".mysql_error().$error);
	}
	
	return $r;
}

public static function checkgiftboxhistory($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");


	$lastDay = TimeManager::getDateTime(TimeManager::getTime()-60*60*24*30);

	$where = "where memberID=".$memberID." and regDate>$lastDay and confirmDate='' limit 1";
	//$userInfo = UserIndex::create($memberID);
	$q = GiftBoxHistory::getQueryResultWithShardKey("select count(*) from ".GiftBoxHistory::getDBTable()." ".$where,$memberID);
	//LogManager::addLog("select count(*) from ".GiftBoxHistory::getDBTable()." ".$where);
	if(!$q)return ResultState::makeReturn(2002,"memberID");
	$cnt = mysql_fetch_array($q);
    if($cnt[0]>0){
    	$r["haveNewGift"] = true;
    	$r["haveNewGiftCnt"]=$cnt[0];
    }else{
    	$r["haveNewGift"] = false;
    	$r["haveNewGiftCnt"]=0;
    }

	$r["result"]=ResultState::successToArray();
	return $r;
}

public static function getcharacterhistory($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$where = "where memberID=".$memberID;
	$dataList = array();
    while($rData = CharacterHistory::getRowByQueryWithShardKey($where,$memberID)){
    	$nextInfo = Character::getExpInfoByExp($rData["exp"]);
		$rData["levelInfo"]=$nextInfo;
    	$dataList[]=$rData;
    }

	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;


}


public static function updatecharacterhistory($p){	
	
	$memberID=$p["memberID"];
	$characterNo=$p["characterNo"];
	$exp = $p["exp"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$characterNo)return ResultState::makeReturn(2002,"characterNo");
	
	//보내기
	$obj = new CharacterHistory($memberID,$characterNo);
	$obj->memberID = $memberID;
	$obj->characterNo = $characterNo;
	if(!$obj->isLoaded()){
		$obj->regDate = TimeManager::getCurrentDateTime();
	}else{
		if($p["useExchange"]==true){
			$cs = new CommonSetting("gachaCharExp");
			$exp=$cs->value;
		}
	}

	$r=array();
	if(!$obj->level)$obj->level=1;
	if($p["level"])$obj->level = $p["level"];
	if($p["levelup"])$obj->level +=1;
	if($obj->level==0)$obj->level=1;
	$nextInfo=array();
	if($exp){
		$nextExp = $obj->exp+$exp;
		$nextInfo = Character::getExpInfoByExp($nextExp);
		$obj->exp = $nextInfo["exp"];
		$r["levelInfo"]=$nextInfo;
		if($obj->level!=$r["levelInfo"]["level"]){
			$obj->level=$r["levelInfo"]["level"];
			$r["isLevelUp"]=true;
		}
	}else{
		$nextInfo = Character::getExpInfoByExp($obj->exp);
	}

	if($obj->save()){
		$r = $obj->getArrayData(true);
		$r["levelInfo"]=$nextInfo;
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	
	return $r;
}

public static function levelup($p){
	$usePass = $p["usePass"];
	$memberID=$p["memberID"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	CommitManager::begin($memberID);
	$userInfo = UserData::create($memberID);
	$powerInfo = Character::getPowerInfo($userInfo->level+1);
	$lev = $powerInfo["level"];

	if($userInfo->level>=$powerInfo["maxLevel"]){
		return ResultState::makeReturn(ResultState::GDALREADY);
	}

	CommitManager::setSuccess($memberID,$userInfo->save()+1);
	
	$param["memberID"]=$memberID;
	
	if(!$usePass){
		$param["exchangeID"]="cu_g";
		$param["list"][0]=array("type"=>"g","count"=>($powerInfo["prevPrice"]*-1));
	}else{
		$param["exchangeID"]="cu_p";
	}

	$result = self::exchange($param);
	if(!ResultState::successCheck($result["result"])){
		CommitManager::setSuccess($memberID,false);

	}

	if(!CommitManager::commit($memberID)){
		$result["result"]=ResultState::toArray(ResultState::GDMYSQLQUERY);
	}

	$result=array_merge($result,$powerInfo);
	
	return $result;
}

public static function gettimeinfo($p){
	//$r["test"]=$p["test"];
	//if($p["offset"])TimeManager::setTimeOffset($p["offset"]);
	$r["timestamp"]=TimeManager::getTime();
	$r["weekNo"]=TimeManager::getCurrentWeekNoForRank();
	$r["weekday"]=TimeManager::getCurrentWeekDayNo();
	$r["date"]=TimeManager::getCurrentDateTime();
	$r["hour"]=TimeManager::getCurrentHour();
	$r["result"]=ResultState::successToArray();
	return $r;
}

public static function gettodaymission($p){

	$memberID  = $p["memberID"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$user = UserData::create($p["memberID"]);

	$tMission =& $user->getRef("TMInfo");//$user->TMInfo;
	$tLevel =& $user->getRef("TMLevel");
	$todayDate = TimeManager::getCurrentDate();

	if(!$tLevel)$tLevel=array(0,0,0,0,0,0,0,0,0,0);
	if(!$tMission)$tMission=array();
	
	$myComon = new CommonSetting("todayMissionReward");
	$tmReward =& $myComon->getRef("value");
	$isFirstCheck = false;
	
	//오늘인데 성공했으면 날짜 어제로 돌려놓고 다시 시작함.
	if($tMission["date"]!=$todayDate){
		$tMission["todayCnt"]=0;
	}

	$restart=false;
	if($tMission["date"]==$todayDate && $tMission["isSuccess"]==true){
		if($tMission["todayCnt"]<2){
			$tMission["isSuccess"]=false;
			$tMission["todayCnt"]+=1;
			$restart=true;
			$isFirstCheck=true;
		}
	}

	//데이트비교해서 이전 미션이 남아있으면 1.완료하지 못한미션일경우 해당 타입 오토벨런싱 -1 시킴. 그리고 데이터 리셋, 투데이미션 재발급
	if($tMission["date"]!=$todayDate || $restart){
		$isFirstCheck = true;
		if($tMission["isSuccess"]==true && $tMission["type"]){
			//$tLevel[$tMission["type"]-1]+=1;
		}else if($tMission["type"] && $restart==false){
			$tLevel[$tMission["type"]-1]-=1;
			if($tLevel[$tMission["type"]-1]<=0)$tLevel[$tMission["type"]-1]=0;
		}
		//미션발급
		srand((double)microtime()*1000000);
		$tMission["date"]=$todayDate;
		$tMission["type"]= rand(1,4);
		$tMission["count"]=0;
		$tMission["isSuccess"]=false;

		$rewardRnd = rand(1,100);

		if($rewardRnd>90)$tMission["rewardInfo"]=$tmReward[0];		//이어하기권
		else if($rewardRnd>80)$tMission["rewardInfo"]=$tmReward[1]; //업그레이드
		else if($rewardRnd>60)$tMission["rewardInfo"]=$tmReward[2]; //99가챠
		else if($rewardRnd>30)$tMission["rewardInfo"]=$tmReward[3]; //뽑기
		else $tMission["rewardInfo"]=$tmReward[4];					//맵가챠
		
		$t = $tMission["type"]-1;
		if($t<0)$t=0;
		if($tMission["type"]==1){ // 영역모으기
			$tMission["goal"] = 500 + (int)($tLevel[$t]*100);
		}else if($tMission["type"]==2){ // 점수모으기
			$tMission["goal"] = 300000 + (int)($tLevel[$t]*100000);
		}else if($tMission["type"]==3){ // 골드
			$tMission["goal"] = 5000 + (int)($tLevel[$t]*1000);
		}else if($tMission["type"]==4){ //부하몹
			$tMission["goal"] = 10 + (int)($tLevel[$t]*2);
		}

		// else if($tMission["type"]==5){
		// 	$tMission["goal"] = 50 + (int)($tLevel[$tMission["type"]]*10);
		// }

		// else if($tMission["type"]==5){
		// 	$tMission["goal"] = 20 + $tLevel[$tMission["type"]]*5;
		// }else if($tMission["type"]==6){
		// 	$tMission["goal"] = 3 + $tLevel[$tMission["type"]]*2;
		// }

		//$user->TMInfo = $tMission;
		//$user->TMLevel = $tLevel;
		if(!$user->save()){
			//LogManager::addLog("todaymission error ".mysql_error());
			return ResultState::makeReturn(1001);
		}
	}

	$r = $tMission;
	$r["remainTime"]= TimeManager::getRemainTimeForTodayMission(); //$r["resetTimestamp"] - TimeManager::getTime();
	$r["resetTimestamp"]=TimeManager::getTime()+$r["remainTime"];
	$r["isFirstCheck"]=$isFirstCheck;
	$r["result"] = ResultState::successToArray();
	return $r;
}


public static function updatetodaymission($p){

	$memberID  = $p["memberID"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$todayDate = TimeManager::getCurrentDate();
	if($todayDate != $p["date"]){
		return ResultState::makeReturn(ResultState::GDSUCCESS,"date not same");
	}
	CommitManager::begin($memberID);
	$user = UserData::create($memberID);
	$tMission =& $user->getRef("TMInfo");
	$tLevel =& $user->getRef("TMLevel");
	$r = array();
	
	if($tMission["todayCnt"]>2){
		return ResultState::makeReturn(ResultState::GDSUCCESS,"date not same");
	}

	$tMission["count"]+=$p["count"];
	$checkFirst = false;
	$checkCount = $tMission["count"];
	$mCount = $tMission["count"];
	if($tMission["count"]<$tMission["goal"])$tMission["isSuccess"]=false;
	if($tMission["count"]>=$tMission["goal"] && $tMission["isSuccess"]!=true){
		$tMission["isSuccess"]=true;
		// $p["memberID"]=$memberID;
		// $p["count"]=$tMission["reward"]["count"];
		// $p["type"]=$tMission["reward"]["type"];
		// $p["statsID"]="tm";
		// $p["statsValue"]=$tMission["type"];
		// $p["content"]="오늘미션보상";
		// $result = self::addproperty($p);
		$checkFirst=true;
		//$tMission["count"]=0;
		$missiontype = $tMission["type"]-1;
		$tLevel[$missiontype]+=1;
		// if(!ResultState::successCheck($result["result"])){
		// 	CommitManager::setSuccess($memberID,false);
		// }


		$param["memberID"]=$memberID;
		$param["template"]="msgTodayMission";
		$param["exchangeID"]=$tMission["rewardInfo"]["exchangeID"];
		$sR = self::sendgiftboxhistory($param);

		if(!ResultState::successCheck($sR["result"])){
			CommitManager::setSuccess($memberID,false);
		}
	}

	//$user->TMInfo = $tMission;
	$a = $user->save();
	CommitManager::setSuccess($memberID,$a);
	
	if(CommitManager::commit($memberID)){
		$r = $tMission;
		$r["count"]=$mCount;
		$r["resetTimestamp"]=strtotime("24:00", TimeManager::getTime()+60*60*5);
		$r["remainTime"]= $r["resetTimestamp"] - TimeManager::getTime();
		if($checkFirst){
			$r["isFirstCheck"]=true;
			$r["rewardCount"]=$checkCount;
		}else{
			$r["isFirstCheck"]=false;
		}
		$r["result"] = ResultState::successToArray();
		return $r;
	}else{
		return ResultState::makeReturn(1001);
	}
}
public static function exchange($p){

	$memberID  = $p["memberID"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$p["exchangeID"])return ResultState::makeReturn(2002,"exchangeID");

	$exchange = new Exchange($p["exchangeID"]);
	
	if(!$exchange->isLoaded())return ResultState::makeReturn(2003,"dont find exchangeID");

	$pByExchange["memberID"]=$memberID;
	$pByExchange["exchangeID"]=$p["exchangeID"];
	

	if(is_array($p["list"])){
		$eList =& $exchange->getRef("list");
		
		for ($j=0; $j<count($p["list"]);$j++) {
			for($i=0;$i<count($eList);$i++){
				if($eList[$i]["type"]==$p["list"][$j]["type"]){
					$eList[$i]=array_merge($eList[$i],$p["list"][$j]);
					break;
				}
				if($i==count($eList)-1){
					$eList[]=$p["list"][$j];
				}
			}
		}
	}


	$pByExchange["list"]=$exchange->list;

	$result = self::changeuserproperties($pByExchange);
	if(ResultState::successCheck($result["result"]))$result["changes"]=$exchange->list;
	return $result;

}
public static function exchangebylist($p){

	$memberID  = $p["memberID"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!is_array($p["exchangeIDList"]))return ResultState::makeReturn(2002,"exchangeIDList");


	$pByExchange = array("memberID"=>$memberID, "list"=>array());

	for($i=0;$i<count($p["exchangeIDList"]);$i++){
		$exchange = new Exchange($p["exchangeIDList"][$i]);
		
		if(!$exchange->isLoaded())return ResultState::makeReturn(2003,"dont find exchangeID ".$p["exchangeIDList"][$i]);

		$pByExchange["list"]=array_merge($pByExchange["list"],$exchange->list);
		$pByExchange["exchangeID"]=$p["exchangeIDList"][$i];
	}

	$result = self::changeuserproperties($pByExchange);

	return $result;

}
public static function checkloginevent($p){

	$memberID  = $p["memberID"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	
	CommitManager::begin($memberID);
	
	$storeBit = CurrentUserInfo::getStoreBit(CurrentUserInfo::$store);
	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	$userData = UserData::create($memberID);
	$todayDateTime = TimeManager::getCurrentDateTime();
	$todayDate = TimeManager::getCurrentDate();
	$nowTime = TimeManager::getCurrentTime();
	$eventCheckData =& $userData->getRef("eventCheckData");

	$result = LoginEvent::getQueryResult("select ".LoginEvent::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".LoginEvent::getDBTable()." left join ".Exchange::getDBTable()." on ".LoginEvent::getDBTable().".exchangeID=".Exchange::getDBTable().".id  where ".LoginEvent::getDBTable().".startDate<$todayDateTime and ".LoginEvent::getDBTable().".endDate>$todayDateTime and ".LoginEvent::getDBTable().".os&".$osBit.">0 and ".LoginEvent::getDBTable().".cc&".$ccBit.">0 and ".LoginEvent::getDBTable().".store&".$storeBit.">0");
	while($rData = mysql_fetch_assoc($result)){

		$idx=-1;
		for($i=0;$i<count($eventCheckData);$i++){
			if($eventCheckData[$i]["no"]==$rData["no"]){
				$idx = $i;
				break;
			}
		}

		if($idx!=-1 && $rData["repeat"] && $eventCheckData[$idx]["date"]!=$todayDate){
			unset($eventCheckData[$idx]);
			$eventCheckData = array_values($eventCheckData);
			$idx=-1;
		}

		if($rData["startTime"]<$nowTime && $rData["endTime"]>$nowTime){	
			//무조건 지급
			if($idx==-1){
				$eventCheckData[]=array("no"=>$rData["no"],"date"=>$todayDate);
				$param["memberID"]=$memberID;
				$param["sender"]="GM";
				$param["content"]=$rData["title"];
				$param["exchangeID"]=$rData["exchangeID"];
				$param["reward"]=$rData["reward"];
				$sR = self::sendgiftboxhistory($param);
				if(!ResultState::successCheck($sR["result"])){
					CommitManager::setSuccess($memberID,false);
				}
			}
		}
	}
	CommitManager::setSuccess($memberID,$userData->save());
	if(CommitManager::commit($memberID)){
		return ResultState::makeReturn(1);
	}

	return ResultState::makeReturn(1001);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

public static function getendlessrank($p){

	$memberID = $p["memberID"];
	$start = $p["start"];
	$limit = $p["limit"];
	$weekNo = TimeManager::getCurrentWeekNoForRank();
	if($p["weekNo"])$weekNo=$p["weekNo"];
	if(!$start)$start=1;
	if(!$limit)$limit=50;

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$userData = UserData::create($memberID);

	$myRank = new EndlessRank($memberID,$weekNo);
	$myRank->memberID=$memberID;
	$myRank->regWeek=$weekNo;

	$r["list"]=$myRank->getTopRank($start,$limit);
	$r["myscore"]=$myRank->score;
	$r["victory"]=$myRank->victory;
	$r["alluser"]=$myRank->getAllUser();
	$r["myrank"]=$myRank->getMyRank();
	$r["remainTime"]=TimeManager::getRemainTimeForWeeklyRank();
	$r["myInfo"]=$userData->endlessData;
	if($r["myrank"]<=0){
		$r["alluser"]+=1;
		$r["myrank"]=$r["alluser"];
	}
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();

	$rewardInfo = new CommonSetting("weeklyReward");
	$nrinfo=array();
	foreach ($rewardInfo->value as $key => $value) {
		$nrinfo[]=$value;
	}
	$r["rewardInfo"]=$nrinfo;

	return $r;

}
public static function getendlessrankinfo($p){

	$memberID = $p["memberID"];
	$weekNo = TimeManager::getCurrentWeekNoForRank();
	if($p["weekNo"])$weekNo=$p["weekNo"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$userData = UserData::create($memberID);

	$myRank = new EndlessRank($memberID,$weekNo);
	$myRank->memberID=$memberID;
	$myRank->regWeek=$weekNo;

	$r["score"]=$myRank->score;
	$r["victory"]=$myRank->victory;
	$r["alluser"]=$myRank->getAllUser();
	$r["rank"]=$myRank->getMyRank();
	$r["endlessData"]=$userData->endlessData;
	$r["nick"]=$userData->nick;
	$r["flag"]=$userData->flag;
	if($r["rank"]<=0){
		$r["alluser"]+=1;
		$r["rank"]=$r["alluser"];
	}
	$r["state"]="ok";
	$r["result"]=ResultState::successToArray();
	return $r;

}
public static function setendlessrank($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$endlessRank = new EndlessRank($memberID,TimeManager::getCurrentWeekNoForRank());

	$endlessRank->memberID=$memberID;
	$endlessRank->nick = $p["nick"];
	$endlessRank->flag = $p["flag"];
	
	
	if($endlessRank->score<$p["score"]){
		$endlessRank->score = $p["score"];
		$endlessRank->regDate = TimeManager::getCurrentDateTime();
	}

	if($endlessRank->victory<$p["victory"]){
		$endlessRank->victory = $p["victory"];
		$endlessRank->regDate = TimeManager::getCurrentDateTime();
	}
	
	$endlessRank->regWeek = TimeManager::getCurrentWeekNoForRank();
	
	$userData = UserData::create($memberID);
	
	$endlessData =& $userData->getRef("endlessData");
	$endlessData["win"]+=$p["victory"];
	$endlessData["lose"]+=1;
	if($p["score"]>$endlessData["score"])$endlessData["score"]=$p["score"];
	if($p["victory"]>$endlessData["victory"])$endlessData["victory"]=$p["victory"];

	CommitManager::begin($memberID);

	if(!$userData->save()){
		CommitManager::setSuccess($memberID,false);
	}
	if(!$endlessRank->save()){
		CommitManager::setSuccess($memberID,false);
	}
	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
	}
	return $r;

}

public static function finishendlessplay($p){
	$memberID=$p["memberID"];
	//LogManager::addLog("start finishendlessplay");
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");


	CommitManager::begin($memberID);
	$endlessRank = new EndlessRank($memberID,TimeManager::getCurrentWeekNoForRank());
	$userData = UserData::create($memberID);
	$endlessData =& $userData->getRef("endlessData");

	//위크번호다르면 초기화~
	if($endlessData["ing_week"]!=TimeManager::getCurrentWeekNoForRank()){
		//LogManager::addLog("reset ing_win , bcus new week");
		$endlessData["ing_score"]=0;
		$endlessData["ing_win"]=0;
		$endlessData["ing_week"]=TimeManager::getCurrentWeekNoForRank();
		$endlessData["ing_level"]=0;
	}


	$endlessData["ing_score"]+=$p["score"];
	$endlessData["ing_state"]=0;

	//승리시 임시저장
	if($p["victory"]){
		$endlessData["win"]+=1;
		$endlessData["ing_win"]+=1;
		$endlessData["ing_level"]+=1;
		$userData->exp += 1;
	}

	//최고기록갱신
	if($endlessData["ing_score"]>$endlessData["score"])$endlessData["score"]=$endlessData["ing_score"];
	if($endlessData["ing_win"]>$endlessData["victory"])$endlessData["victory"]=$endlessData["ing_win"];

	

	$endlessRank->memberID=$memberID;
	$endlessRank->nick = $p["nick"];
	$endlessRank->flag = $p["flag"];
	$endlessRank->data = $p["data"];
	
	//주간최고기록갱신
	if($endlessData["ing_score"]>$endlessRank->score)$endlessRank->score=$endlessData["ing_score"];
	if($endlessData["ing_win"]>$endlessRank->victory)$endlessRank->victory=$endlessData["ing_win"];

	$endlessRank->regDate = TimeManager::getCurrentDateTime();
	$endlessRank->regWeek = TimeManager::getCurrentWeekNoForRank();
	
	$r["sendGift"]=false;
	//졌으면 끄읏
	if(!$p["victory"]){
		$endlessData["ing_win"]=0;
		$endlessData["ing_score"]=0;
		$endlessData["ing_week"]=TimeManager::getCurrentWeekNoForRank();
		$endlessData["lose"]+=1;
		$endlessData["ing_level"]=0;
	}else{
		$cs = new CommonSetting("pvpReward");
		$ew =& $cs->getRef("value");
		if($endlessData["ing_win"]%$ew["win"]==0){
			$r["sendGift"]=true;
			$param["memberID"]=$memberID;
			$param["sender"]="GM";
			$param["content"]=$ew["title"];
			$param["exchangeID"]=$ew["exchangeIDs"][rand(0,count($ew["exchangeIDs"])-1)];
			$sR = self::sendgiftboxhistory($param);
			$r["reward"]=$sR["reward"];
			if(!ResultState::successCheck($sR["result"])){
				//LogManager::addLog("sendgift save fail");
				CommitManager::setSuccess($memberID,false);
			}
		}

		
		//미션이벤트체크
		$cmP["type"]="pvpWin";
		$cmP["value"]=$endlessData["ing_win"];
		$cmP["memberID"]=$memberID;
		self::checkmissionevent($cmP);

		//미션이벤트체크
		$cmP["type"]="pvpScore";
		$cmP["value"]=$endlessData["ing_score"];
		$cmP["memberID"]=$memberID;
		self::checkmissionevent($cmP);
		
	}
	

	$r["endlessData"]=$endlessData;

	if(!$userData->save()){
		//LogManager::addLog("userdata save fail");
		CommitManager::setSuccess($memberID,false);
	}
	if(!$endlessRank->save()){
		//LogManager::addLog("endlessrank save fail");
		CommitManager::setSuccess($memberID,false);
	}
	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
	}
	return $r;

}


public static function getendlessplaydata($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$endlessPlayList = new EndlessPlayList($memberID);

	$r=array();
	if($p["no"]){
		$r["rival"] = $endlessPlayList->getPlayDataByNo($p["no"]);
	}else{
		$r["rival"] = $endlessPlayList->getPlayDataByRandom($memberID,$p["win"]);
	}
	
	srand((double)microtime()*1000000);
	$sp["no"]=rand(1,25);
	$r["stageInfo"]=self::getpieceinfo($sp);
	$r["stageInfo"]["no"]=99999;
	$r["stageInfo"]["order"]=99999;
	
	for($i=0;$i<count($r["stageInfo"][cards]);$i++){
		$r["stageInfo"]["cards"][$i]["no"]=1000000+$i;
		$r["stageInfo"]["cards"][$i]["piece"]=99999;
		$r["stageInfo"]["cards"][$i]["stage"]=99999;
	}
	
	$r["result"]=ResultState::successToArray();
	return $r;

}

public static function startendlessplay($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	
	$userInfo = UserData::create($memberID);
	$endlessData =& $userInfo->getRef("endlessData");
	
	if($endlessData["ing_win"]<=0){
		$endlessData["ing_level"]=$p["autoLevel"];		
	}

	if($endlessData["ing_state"]==1 || $endlessData["ing_week"]!=TimeManager::getCurrentWeekNoForRank()){
		$endlessData["ing_win"]=0;
		$endlessData["lose"]+=1;
		$endlessData["ing_score"]=0;
		$endlessData["ing_week"]=TimeManager::getCurrentWeekNoForRank();
	}
	
	$endlessData["ing_state"]=1;


	$r["endlessData"]=$endlessData;
	if($userInfo->save()){
		$r["result"]=ResultState::successToArray();		
	}else{
		return ResultState::makeReturn(ResultState::GDDONTSAVE);
	}

	return $r;

}
public static function getendlessplayriver($p){
	$memberID=$p["memberID"];
	$ingLevel = $p["win"];
	$maxStage = $p["highPiece"];
	if(!$maxStage)$maxStage=25;
	$endlessPlayList = new EndlessPlayList();

	if(!$ingLevel)$ingLevel=1;

	$rival2 = $endlessPlayList->getPlayDataByRandom($memberID,$ingLevel,3);
	
	for($i=0;$i<count($rival2);$i++){
		$uChar = UserData::create($rival2[$i]["memberID"]);
		$rival2[$i]["characterNo"]=$uChar->selectedCharNO;
	}

	$r["rival"] = $rival2[0];

	if($rival2[1]["nick"]==$rival2[2]["nick"]){
		$rival2[1]["nick"]="LitQoo";
		$rival2[2]["nick"]="NHNEnt";
	}

	if($rival2[0]["nick"]==$rival2[1]["nick"]){
		$rival2[1]["nick"]="LitQoo";
		$rival2[2]["nick"]="NHNEnt";
	}

	if($rival2[0]["nick"]==$rival2[2]["nick"]){
		$rival2[1]["nick"]="LitQoo";
		$rival2[2]["nick"]="NHNEnt";
	}


	unset($rival2[0]["playData"]);
	unset($rival2[1]["playData"]);
	unset($rival2[2]["playData"]);
	$r["dummy"] = $rival2;
	
	$sp["no"]=$rival["pieceNo"];

	//if(!$sp["no"])
	
	$userInfo = UserData::create($memberID);

	srand((double)microtime()*1000000);


	$max = 20+$ingLevel*20;
	if($max>100)$max=100;
	$sp["no"]=rand(1,$max);

	$r["stageInfo"]=self::getpieceinfo($sp);
	$r["stageInfo"]["realNo"]=$r["stageInfo"]["no"];
	$r["stageInfo"]["no"]=99999;
	$r["stageInfo"]["order"]=99999;
	
	for($i=0;$i<count($r["stageInfo"]["cards"]);$i++){
		$r["stageInfo"]["cards"][$i]["realPiece"]=$r["stageInfo"]["cards"][$i]["piece"];
		$r["stageInfo"]["cards"][$i]["realStage"]=$r["stageInfo"]["cards"][$i]["stage"];
		$r["stageInfo"]["cards"][$i]["piece"]=99999;
		$r["stageInfo"]["cards"][$i]["stage"]=99999;
		$r["stageInfo"]["cards"][$i]["grade"]=$i+1;
	}
		
	$r["result"]=ResultState::successToArray();		
	return $r;
}
public static function saveendlessplaydata($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$endlessPlayList = new EndlessPlayList($memberID);
	$endlessPlayList->memberID=$memberID;
	$endlessPlayList->score = $p["score"];
	$endlessPlayList->nick = $p["nick"];
	$endlessPlayList->level = $p["level"];
	$endlessPlayList->autoLevel = $p["autoLevel"];
	$endlessPlayList->flag = $p["flag"];
	$endlessPlayList->victory = $p["victory"];
	$endlessPlayList->playData = $p["playData"];
	$endlessPlayList->pieceNo = $p["pieceNo"];
	$endlessPlayList->regDate = TimeManager::getCurrentDateTime();

	if($endlessPlayList->save()){
		//LogManager::addLog("saveendlessplaydata ok");
		$r["no"]=$endlessPlayList->no;
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(1001);
	}
	return $r;

}



public static function checkattendenceevent($p){
	$memberID=$p["memberID"];
	//LogManager::addLog("== start checkattendenceevent ==========================");
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	$userData = UserData::create($memberID);
	
	$r["sendGift"]=false;
	$needSendGift=true;

	//LogManager::addLog("== check checkattendenceevent ==========================");

	//아래코드를 초기화하면 출첵이 계속 뜬다
	if($userData->eventCheckDate==TimeManager::getCurrentDate()){
		$needSendGift=false;
		//$r["result"]=ResultState::successToArray();
		//return $r;
	}


	//LogManager::addLog("== info check checkattendenceevent ==========================");

	//1. 어제출석정보가 있는지. 없으면 출첵정보초기화
	// if($userData->eventCheckDate!=TimeManager::getYesterDayDate()){
	// 	//LogManager::addLog("reset eventAtdCount1");
	// 	$userData->eventCheckDate=0;
	// 	$userData->eventAtdNo=0;
	// 	$userData->eventAtdCount=0;
	// }
	

	//28일지났어도 초기화
	if($userData->eventAtdCount>=28){
		//LogManager::addLog("reset eventAtdCount2");
		$userData->eventCheckDate=0;
		$userData->eventAtdNo=0;
		$userData->eventAtdCount=0;
	}

	//2. 진행중인 출첵이 있다면 해당 출첵정보 로드, 없으면 현재진행중인거 로드
	$adtInfo = new AttendenceEvent($userData->eventAtdNo);


	if(!$adtInfo->isLoaded()){
		$r["result"]=ResultState::successToArray();

		return $r;
	}
	
	CommitManager::begin($memberID);
	//4. 출석정보업데이트하고, 하루더 카운팅, 보상지급
	$userData->eventAtdNo=$adtInfo->no;

	//보상정보로드

	$rewardInfo=array();
	$rewardList=array();

	$result = AttendenceEventDay::getQueryResult("select ".AttendenceEventDay::getDBTable().".*,".ExchangeForEvent::getDBTable().".list as reward from ".AttendenceEventDay::getDBTable()." left join ".ExchangeForEvent::getDBTable()." on ".AttendenceEventDay::getDBTable().".exchangeID=".ExchangeForEvent::getDBTable().".id where ".AttendenceEventDay::getDBTable().".eventNo='".$userData->eventAtdNo."' order by day asc");
	while($dData = mysql_fetch_assoc($result)){
	//while($dData = AttendenceEventDay::getRowByQuery("where eventNo='".$userData->eventAtdNo."' order by day asc")){
		$dNo = $dData["day"]-1;
		$rewards = json_decode($dData["reward"]);
		if($dNo==$userData->eventAtdCount){
			$rewardInfo=array("reward"=>$rewards,"exchangeID"=>$dData["exchangeID"],"title"=>$dData["title"]);
		}
		$rewardList[$dNo]=$rewards[0];
	}

	//보상지급
	if($needSendGift){
		if($rewardInfo){
			$param["memberID"]=$memberID;
			$param["sender"]="GM";
			$param["content"]=$rewardInfo["title"];
			$param["exchangeID"]=$rewardInfo["exchangeID"];
			$param["reward"]=$rewardInfo["reward"];
			$sR = self::sendgiftboxhistory($param);
			if(!ResultState::successCheck($sR["result"])){
				CommitManager::setSuccess($memberID,false);
			}
			$r["sendGift"]=true;
		}else{
			$userData->eventCheckDate=0;
			$userData->eventAtdNo=0;
			$userData->eventAtdCount=-1;
		}
	}
	if($needSendGift)$r["dayCount"]=$userData->eventAtdCount+1;
	else $r["dayCount"]=$userData->eventAtdCount;

	$r["rewardList"]=$rewardList;
	$r["dayList"]=LoginEvent::getRewardDays();

	if($needSendGift){
		$userData->eventAtdCount++;
		$userData->eventCheckDate=TimeManager::getCurrentDate();
		if(!$userData->save()){
			CommitManager::setSuccess($memberID,false);
		}
	}

	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
	}

	return $r;

}




public static function recheckattendenceevent($p){
	$memberID=$p["memberID"];
	//LogManager::addLog("== start checkattendenceevent ==========================");
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	$userData = UserData::create($memberID);
	
	$r["sendGift"]=false;


	//LogManager::addLog("== check checkattendenceevent ==========================");

	//아래코드를 초기화하면 출첵이 계속 뜬다
	// if($userData->eventCheckDate==TimeManager::getCurrentDate()){
	// 	$r["result"]=ResultState::successToArray();
	// 	return $r;
	// }


	//LogManager::addLog("== info check checkattendenceevent ==========================");

	//1. 어제출석정보가 있는지. 없으면 출첵정보초기화
	// if($userData->eventCheckDate!=TimeManager::getYesterDayDate()){
	// 	//LogManager::addLog("reset eventAtdCount1");
	// 	$userData->eventCheckDate=0;
	// 	$userData->eventAtdNo=0;
	// 	$userData->eventAtdCount=0;
	// }
	

	//28일지났어도 초기화
	if($userData->eventAtdCount>=28){
		//LogManager::addLog("reset eventAtdCount2");
		$userData->eventCheckDate=0;
		$userData->eventAtdNo=0;
		$userData->eventAtdCount=0;
	}

	//2. 진행중인 출첵이 있다면 해당 출첵정보 로드, 없으면 현재진행중인거 로드
	$adtInfo = new AttendenceEvent($userData->eventAtdNo);


	if(!$adtInfo->isLoaded()){
		$r["result"]=ResultState::successToArray();

		return $r;
	}
	
	CommitManager::begin($memberID);
	//4. 출석정보업데이트하고, 하루더 카운팅, 보상지급
	$userData->eventAtdNo=$adtInfo->no;

	//보상정보로드

	$rewardInfo=array();
	$rewardList=array();

	$result = AttendenceEventDay::getQueryResult("select ".AttendenceEventDay::getDBTable().".*,".ExchangeForEvent::getDBTable().".list as reward from ".AttendenceEventDay::getDBTable()." left join ".ExchangeForEvent::getDBTable()." on ".AttendenceEventDay::getDBTable().".exchangeID=".ExchangeForEvent::getDBTable().".id where ".AttendenceEventDay::getDBTable().".eventNo='".$userData->eventAtdNo."' order by day asc");
	while($dData = mysql_fetch_assoc($result)){
	//while($dData = AttendenceEventDay::getRowByQuery("where eventNo='".$userData->eventAtdNo."' order by day asc")){
		$dNo = $dData["day"]-1;
		$rewards = json_decode($dData["reward"]);
		if($dNo==$userData->eventAtdCount){
			$rewardInfo=array("reward"=>$rewards,"exchangeID"=>$dData["exchangeID"],"title"=>$dData["title"]);
		}
		$rewardList[$dNo]=$rewards[0];
	}

	
	$r["dayCount"]=$userData->eventAtdCount+1;
	$r["rewardList"]=$rewardList;
	$r["dayList"]=LoginEvent::getRewardDays();

	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
	}

	return $r;

}

public static function usecupon($p){
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	// 1. 쿠폰코드검색. 검색안되면 튕구기
	$cuponCode = new CuponCode($p["cuponCode"]);
	if(!$cuponCode->isLoaded()){
		return ResultState::makeReturn(ResultState::GDDONTFIND);
	}
	
	//LogManager::addLog("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ start CuponManager"); 

	// 2. 쿠폰정보로드
	$cuponInfo = new CuponManager($cuponCode->cuponNo);
	
	//유효기간검사
	$today = TimeManager::getCurrentDateTime();
	if($cuponInfo->startDate>$today || $cuponInfo->endDate<$today){
		return ResultState::makeReturn(ResultState::GDEXPIRE);	
	}

	//국가코드검사
	if(!$cuponInfo->checkCountry(CurrentUserInfo::$country)){
		return ResultState::makeReturn(ResultState::GDDONTFIND);
	}
	//os검사
	if(!$cuponInfo->checkOs(CurrentUserInfo::$os)){
		return ResultState::makeReturn(ResultState::GDDONTFIND);
	}

	CommitManager::begin($memberID);


	//LogManager::addLog("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ start CuponHistory"); 

	// 3. 사용내역로드. 사용했으면 튕구기
	$cuponHistory = new CuponHistory($cuponCode->cuponCode,$memberID);
	
	if($cuponHistory->isLoaded()){
		return ResultState::makeReturn(ResultState::GDALREADY);	
	}


	//LogManager::addLog("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ start CuponUsedInfo"); 
	$cuponUsedInfo = new CuponUsedInfo($cuponCode->cuponCode);
	if($cuponUsedInfo->useDate>0 && !$cuponInfo->isCommon){
		return ResultState::makeReturn(ResultState::GDALREADY);
	}

	// 4. 보상지급 히스토리등록 폐기처리
	
	//LogManager::addLog("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ start save usedInfo");
	//사용등록
	if(!$cuponInfo->isCommon){
		$cuponUsedInfo->useDate = $today;
		$cuponUsedInfo->memberID = $memberID;
		if(!$cuponUsedInfo->save()){
			CommitManager::setSuccess($memberID,false);
		}
	}


	//LogManager::addLog("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ start sendgift");
	//보상지급
	$param["memberID"]=$memberID;
	$param["sender"]="GM";
	$param["content"]=$cuponInfo->title;
	$param["exchangeID"]=$cuponInfo->exchangeID;
	//$param["reward"]=$cuponInfo->reward;
	$sR = self::sendgiftboxhistory($param);
	
	if(!ResultState::successCheck($sR["result"])){
		CommitManager::setSuccess($memberID,false);
	}

	//히스토리등록
	$cuponHistory->useDate = $today;
	$cuponHistory->cuponNo = $cuponInfo->no;
	$cuponHistory->cuponCode = $cuponCode->cuponCode;
	
	if(!$cuponHistory->save()){
		CommitManager::setSuccess($memberID,false);
	}

	//끗
	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
	}

	return $r;

}
////////////////////////////////////////////////


public static function gettimeevent($p){
	$nowDate = TimeManager::getCurrentDateTime();
	$nowtime = TimeManager::getCurrentTime();
	$eList = array();
	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	while($rData = TimeEvent::getRowByQuery("where startDate<".$nowDate." and endDate>".$nowDate." and startTime<=".$nowtime." and endTime>=".$nowtime." and os&".$osBit.">0 and cc&".$ccBit.">0")){
		$eList[]=$rData;
	}

	$r["list"]=$eList;
	$r["result"]=ResultState::successToArray();
	return $r;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

public static function getheart($p){

	$memberID=$p["memberID"];
	$use = $p["use"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$now = TimeManager::getTimestampByUTC();
	

	$cooltime = new CommonSetting("heartCoolTime");
	$heartMax = new CommonSetting("heartMax");
	if(!$cooltime->isLoaded() || !$heartMax->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND);

	CommitManager::begin($memberID);
	$userInfo = UserData::create($memberID);
	$userStorage = new UserStorage($memberID);
	$r["heart"] = $userStorage->h; 
	if(!$userInfo->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND);

	//하트가 맥스일때
	if($userStorage->h>=$heartMax->value){
		//LogManager::addLog("heart is max");
		$userInfo->lastHeartTime=$now;

		CommitManager::setSuccess($memberID,$userInfo->save());
		$r["leftTime"]=$cooltime->value;	
		$r["heart"] = $userStorage->h;
	//하트가 맥스 이하일때
	}else{

		//LogManager::addLog("heart is not max");
		//일딴 새로 채울 하트가 있는가~
		$m = $now-$userInfo->lastHeartTime;

		$nHeart = floor($m/$cooltime->value);
		$m = $m%$cooltime->value;
		
		//LogManager::addLog("plus heart is $nHeart");
		//LogManager::addLog("next heart time is $m");		
		
		if($nHeart>0){
			//시간업데이트하고
			$userInfo->lastHeartTime = $now-$m;

			//하트맥스보다 방금채운 하트값이 더 많다면 하트는 하트맥스로 재조절
			if($userStorage->h+$nHeart>$heartMax->value)$nHeart=$heartMax->value-$userStorage->h;
			
			//LogManager::addLog("if1--> $nHeart");


			//LogManager::addLog("if2--> $nHeart");

			//채울값이 없다면 그냥 리턴. 한개차고 바로 한개 깍는경우이지~
			// if($nHeart==0){

			// 	//LogManager::addLog("nHeart is 0 gogo");
			// 	//하트값이 0개라면 그냥ㄱㄱ
			// 	$r["leftTime"]=$cooltime->value;
			// 	$r["max"]=$heartMax->value;
			// 	$r["heart"]=0;
			// 	$r["result"]=ResultState::toArray(ResultState::GDSUCCESS);
			// 	return $r;
			// }

			//LogManager::addLog("do heart exchange");

			$ep["memberID"]=$memberID;
			$ep["exchangeID"]="getHeart";
			$ep["statsValue"]=$userInfo->highPiece;
			$ep["list"][]=array("type"=>"h","count"=>$nHeart);
			$er=self::exchange($ep);
			CommitManager::setSuccess($memberID,ResultState::successCheck($er["result"]));
			$r["heart"] = $er["list"][0]["count"];
			

			CommitManager::setSuccess($memberID,$userInfo->save());
			//CommitManager::setSuccess($memberID,$userStorage->save());
			$r["leftTime"]=$cooltime->value-$m;
		}else{
			$r["leftTime"]=$cooltime->value-$m;			
		}



	}


	///////////////////////////////////////////////////////여기서 exchange getHeart
	if($use){
		$ep["memberID"]=$memberID;
		$ep["exchangeID"]="useHeart";
		$ep["statsValue"]=$userInfo->highPiece;
		unset($ep["list"]);
		$er=self::exchange($ep);
		CommitManager::setSuccess($memberID,ResultState::successCheck($er["result"]));

		if($er["result"]["code"]==ResultState::GDPROPERTYISMINUS){
			CommitManager::rollback($memberID);
			$r = ResultState::makeReturn(ResultState::GDEXPIRE);
			$r["leftTime"]=$cooltime->value-$m;
			$r["max"]=$heartMax->value;
			$r["heart"]=0;
			$r["isMinus"]=true;
			$r["exchangeResult"]=$er;
			return $r;
		}

		$r["exchangeResult"]=$er;
		$r["heart"] = $er["list"][0]["count"];
	}

	$r["max"]=$heartMax->value;
	
	if(CommitManager::commit($memberID)){
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
	}

	return $r;



}

///////// 카드 선물하기

public static function sendcard($p){
	$memberID=$p["memberID"];
	$toMemberID=$p["toMemberID"];
	$cardNo=$p["cardNo"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$toMemberID)return ResultState::makeReturn(2002,"toMemberID");
	if(!$cardNo)return ResultState::makeReturn(2002,"cardNo");

	$cardInfo = new CardHistory($memberID,$cardNo);
	if(!$cardInfo->isLoaded() || $cardInfo->count<=1)return ResultState::makeReturn(ResultState::GDEXPIRE);
	
	$toUserData = new UserData($toMemberID);
	if(!$toUserData->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);

	$myUserData = new UserData($memberID);
	if(!$myUserData->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFINDUSER);

	$cs = new CommonSetting("sendcardbyuser");
	$csInfo=&$cs->getRef("value");

	$nowTime = TimeManager::getTime();

	if($nowTime-$myUserData->sendCardTime < $csInfo["hourLimit"]*60*60){
		$r = ResultState::makeReturn(ResultState::GDALREADY);
		$r["hourLimit"]=$csInfo["hourLimit"];
		$r["leftTime"]= $csInfo["hourLimit"]*60*60-($nowTime-$myUserData->sendCardTime);
		return $r;
	}


	CommitManager::begin($memberID);


	$myUserData->sendCardTime=$nowTime;
	CommitManager::setSuccess($memberID,$myUserData->save());
	$cardInfo->count-=1;

	CommitManager::setSuccess($memberID,$cardInfo->save());
	if(!CommitManager::commit($memberID)){
		return ResultState::makeReturn(ResultState::GDDONTSAVE,"mysql_eror :".mysql_error());
	};

	$cardInfo = new Card($cardNo);

	$param["memberID"]=$toMemberID;
	$param["sendMemberID"]=$myUserData->memberID;
	$param["sender"]=$myUserData->nick;
	$param["template"]=$csInfo["template"];
	$param["custom"]=array($myUserData->nick,$cardInfo->serial);
	$param["exchangeID"]=$csInfo["exchangeID"];
	$param["exchangeList"]=array(array("type"=>"cd","count"=>$cardNo,"level"=>$cardInfo->level));

	
	$sR = self::sendgiftboxhistory($param);
	//CommitManager::setSuccess($mID,ResultState::successCheck($sR["result"]));
	//$r["result"]=ResultState::toArray(ResultState::GDSUCCESS);
	return $sR;

}

//// 가챠상품목록 불러오기

public static function getgachalist($p){
	
	$list = array();
	$levelList = array();

	$levelList = array(1,1,1,1,1,1,1,1,1,1,1,1);
	$cs = new CommonSetting("gachaForm");
	$gachaForm =& $cs->getRef("value");

	$gachaKey = "normal";
	$isPremium=false;
	if($p["isPremium"]){
		$gachaKey = "premium";
		$isPremium=true;
	}

	$k=0;
	for($i=0;$i<count($gachaForm[$gachaKey]);$i++){
		//$i+1 = 등급, $gachaForm[$gachaKey][$i] 등급갯수 		
		$pre="";
		$level = $i+1;
		if(!$isPremium)$pre = " and premiumOnly=0 ";
		else $pre = " and premiumOnly=1 ";
		$q_where = "level=".$level." ".$pre." and showProb>0 order by rand()*100+showProb desc limit ".$gachaForm[$gachaKey][$i];
			
		while($rData = Gacha::getRowByQuery("where ".$q_where)){
			$exchange = new Exchange($rData["exchangeID"]);
			$list[]=array("exchangeID"=>$rData["exchangeID"],"reward"=>$exchange->list,"level"=>$level,"percent"=>$rData["gainProb"]);
	
		}

	}

	shuffle($list);
	$r["list"]=$list;
	$r["result"]=ResultState::toArray(ResultState::GDSUCCESS);
	return $r;
/*
	$list = array();
	$levelList = array();

	$levelList = array(1,1,1,1,1,1,1,1,1,1,1,1);
	$cs = new CommonSetting("gachaForm");
	$gachaForm =& $cs->getRef("value");

	$gachaKey = "normal";
	
	if($p["isPremium"]){
		$gachaKey = "premium";
	}

	$k=0;
	for($i=0;$i<count($gachaForm[$gachaKey]);$i++){
		for($j=0;$j<$gachaForm[$gachaKey][$i];$j++){
			$levelList[$k]=$i+1;
			$k++;
		}
	}

	foreach ($levelList as $key => $value) {
		$reward = new Gacha(null,$value,$p["isPremium"]);
		$exchange = new Exchange($reward->exchangeID);
		$list[]=array("exchangeID"=>$reward->exchangeID,"reward"=>$exchange->list,"level"=>$value,"percent"=>$reward->gainProb);
	}
	shuffle($list);
	$r["list"]=$list;
	$r["result"]=ResultState::toArray(ResultState::GDSUCCESS);
*/
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
}

?>