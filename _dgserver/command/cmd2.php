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
	if(!ResultState::successCheck($result["result"]))return $result;
	
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
					$user->save();
				}
				return $result2;
			}
			return $result3;
		}
		return $result2;

	}

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


////////////////////////////////////////////////

public static function getnoticelist($p){

	$nowDate = TimeManager::getCurrentDateTime();
	

	$r = array();

	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	while($rData = Notice::getRowByQuery("where startDate<$nowDate and endDate>$nowDate and os&".$osBit.">0 and cc&".$ccBit.">0 and isPopup=1 order by `order` asc")){
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
		
		return $r;
	}
	
	
	$list = array();
	while($charInfo = Character::getRowByQuery("")){
			$charInfo[purchaseInfo]=json_decode($charInfo[purchaseInfo],true);
			$charInfo[statInfo]=json_decode($charInfo[statInfo],true);
			$charInfo[resourceInfo]=json_decode($charInfo[resourceInfo],true);


			$script=json_decode($charInfo[name],true);
			$charInfo["name"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];

			$script=json_decode($charInfo[comment],true);
			$charInfo["comment"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
	
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


	$listVer = kvManager::get("shopListVer",1);
	
	if($listVer==$p[version]){
		$r[version]=$p[version];
		$r[state]="ok";
		
		$r["result"]=ResultState::toArray(2001);
		
		return $r;
	}
	
	$r=array();
	$list = array();
	$oldlistname = "";
	$shopLoad=false;
	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);

	$result = Shop::getQueryResult("select ".Shop::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".Shop::getDBTable()." left join ".Exchange::getDBTable()." on ".Shop::getDBTable().".exchangeID=".Exchange::getDBTable().".id where cc='".CurrentUserInfo::$country."' and os&".$osBit.">0");
	LogManager::addLog("loginevent1->".mysql_error());
	while($shopInfo = mysql_fetch_assoc($result)){
		LogManager::addLog("data->".json_encode($shopInfo));

		//while($shopInfo = Shop::getRowByQuery("where cc='".CurrentUserInfo::$country."' and os&".$osBit.">0")){
		$shopInfo["data"]=json_decode($shopInfo["data"],true);
		$id = $shopInfo["id"];
		$r[$id]=$shopInfo;
		$shopLoad=true;
	}
	if(!$shopLoad){
		$result2 = Shop::getQueryResult("select ".Shop::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".Shop::getDBTable()." left join ".Exchange::getDBTable()." on ".Shop::getDBTable().".exchangeID=".Exchange::getDBTable().".id where cc='default' and os&".$osBit.">0");
		while($shopInfo = mysql_fetch_assoc($result)){
			$shopInfo["data"]=json_decode($shopInfo["data"],true);
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

	$result2 = ShopEvent::getQueryResult("select ".ShopEvent::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".ShopEvent::getDBTable()." left join ".Exchange::getDBTable()." on ".ShopEvent::getDBTable().".exchangeID=".Exchange::getDBTable().".id where ".ShopEvent::getDBTable().".startDate<=".$today." and ".ShopEvent::getDBTable().".endDate>=".$today." and ".ShopEvent::getDBTable().".startTime<=".$nowtime." and ".ShopEvent::getDBTable().".endTime>=".$nowtime." and ".ShopEvent::getDBTable().".os&".$osBit.">0 and ".ShopEvent::getDBTable().".cc&".$ccBit.">0");
	while($shopInfo = mysql_fetch_assoc($result)){
	//while($shopInfo = ShopEvent::getRowByQuery("where startDate<=".$today." and endDate>=".$today." and startTime<=".$nowtime." and endTime>=".$nowtime." and os&".$osBit.">0 and cc&".$ccBit.">0")){
			$shopInfo["data"]=json_decode($shopInfo["data"],true);
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
		
		return $r;
	}
	
	
	$list = array();
	while($mobInfo = Monster::getRowByQuery()){
			$script=json_decode($mobInfo[script],true);
			$mobInfo["script"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
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

	if($puzzleListVer==$p[version])return ResultState::makeReturn(2001);


	$cardlist="";
	
	if(is_array($p["noList"]))
		$cardlist = join(',',$p["noList"]);
	else 
		$cardlist = $p["noList"];
	
	
	$list = array();
	while($cardInfo = Card::getRowByQuery("where no IN (".$cardlist.")")){
		if($cardInfo){

			$script=json_decode($cardInfo["profile"],true);
			$cardInfo["profile"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
			
			$script=json_decode($cardInfo[script],true);
			$cardInfo["script"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
			
			$script=json_decode($cardInfo["name"],true);
			$cardInfo["name"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];

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

	if($puzzleListVer==$p[version])return ResultState::makeReturn(2001);

	
	$list = array();
	while($cardInfo = Card::getRowByQuery("where version>".$p[version])){
		if($cardInfo){

			$script=json_decode($cardInfo["profile"],true);
			$cardInfo["profile"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
			
			$script=json_decode($cardInfo[script],true);
			$cardInfo["script"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
			
			$script=json_decode($cardInfo["name"],true);
			$cardInfo["name"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];

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
	$noList = $param["noList"];
	if($puzzleListVer==$p[version])return ResultState::makeReturn(2001);

	
	$list = array();


	$cardlist="";
	
	if(is_array($noList))
		$cardlist = join(',',$noList);
	else 
		$cardlist = $p["noList"];
	
	
	$list = array();

	while($cardInfo = Card::getRowByQuery("where  no IN (".$cardlist.") AND version>".$p[version])){
		if($cardInfo){

			$script=json_decode($cardInfo["profile"],true);
			$cardInfo["profile"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
			
			$script=json_decode($cardInfo[script],true);
			$cardInfo["script"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
			
			$script=json_decode($cardInfo["name"],true);
			$cardInfo["name"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];

			$list[]=$cardInfo;
		}
	}
	
	$r["list"]= $list; //json_decode(json_encode($list,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK),true);
	$r["state"]="ok";
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
		while($pData =  Puzzle::getRowByQuery("where isEvent>=0 order by `order` asc limit ".$p[start].",".$p[limit])){
			
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
			$pData["title"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
			
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
		return ResultState::makeReturn(ResultState::GDSAMEVERSION);
	}else{
		// 여기서 eventstagelistversion 도 비교
		
		$list=array();
		$cnt=0;
		while($pData =  HellMode::getRowByQuery("")){
			
			$pieceInfo = new Piece($pData["pieceNo"]);
			$pData["pieceInfo"]=$pieceInfo->getArrayDataForClient();

			$script=json_decode($pData[title],true);
			$pData["title"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];

			$script=json_decode($pData[content],true);
			$pData["content"] = $script[CurrentUserInfo::$language]?$script[CurrentUserInfo::$language]:$script["en"];
			
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
	$user->diaryCode = CuponCode::getRandomString(5);

	if($user->save()){
		$r["diaryCode"]=$user->diaryCode;
		$r["result"]=ResultState::successToArray(ResultState::GDSUCCESS);
		return $r;
	}else{
		return ResultState::makeReturn(ResultState::GDDONTSAVE,mysql_error());
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
		
		if($endlessData["ing_state"]==1 || $endlessData["ing_week"]!=TimeManager::getCurrentWeekNo()){
			$endlessData["ing_win"]=0;
			if($endlessData["ing_state"])$endlessData["lose"]+=1;
			$endlessData["ing_score"]=0;
			$endlessData["ing_week"]=TimeManager::getCurrentWeekNo();
		}

		$user->lastDate = TimeManager::getCurrentDateTime();
		$user->lastTime = TimeManager::getTime();
		
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
	if(mb_strlen($nick,'UTF-8')<3)return ResultState::makeReturn(2012); 




	$user = UserData::create($memberID);
	$userIndex = $user->getUserIndex();
	$user->memberID = $memberID;
	$user->flag = $p["flag"];
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
	if($memberid){
		$user = UserData::create($memberid); //UserData::create($memberid);
		if($user->isLoaded()){
			if($isPublic){
				$r = $user->getArrayDataForPublic();
			}else{
				$r = $user->getArrayData(true,$keylist);
			}
			$r["state"]="ok";
			$r["userIndex"]=$user->getUserIndexNo();
			$r["result"]=ResultState::successToArray();
		}else{
			$r["state"]="error";
			$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"fail to load userdata1");
		}
	}else if($userindex && $userindex>0){
		$uIndex = UserIndex::create(0,$userindex);
		if($uIndex->isLoaded()){
			$user = UserData::create($uIndex->memberID);
			if($user->isLoaded()){
				if($isPublic){
					$r = $user->getArrayDataForPublic();
				}else{
					$r = $user->getArrayData(true,$keylist);
				}
				$r["state"]="ok";
				$r["userIndex"]=$user->getUserIndexNo();
				$r["result"]=ResultState::successToArray();
			}else{
				$r["state"]="error";
				$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"fail to load userdata2");
			}
		}else{
			$r["state"]="error";
			$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"fail to load userdata3");
		}

	}else if($nick){
		$user = UserData::create(null,null,$nick); //UserData::create($memberid);
		if($user->isLoaded()){
			if($isPublic){
				$r = $user->getArrayDataForPublic();
			}else{
				$r = $user->getArrayData(true,$keylist);
			}
			$r["state"]="ok";
			$r["userIndex"]=$user->getUserIndexNo();
			$r["result"]=ResultState::successToArray();
		}else{
			$r["state"]="error";
			$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"fail to load userdata1");
		}
	}else{
		$r["state"]="error";
		$r["errorCode"]=10010;
		$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"memberID");
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
		if($p["archiveData"]){
			$archiveData =& $user->getRef("archiveData");
			$archiveData = array_merge($archiveData,$p["archiveData"]);
		}
		if($p["highPiece"])$user->highPiece = $p["highPiece"];

		if($p["data"]){
			//LogManager::addLog("updateuserdata updateData");
			//if(!$user->updateData($p["data"]))return ResultState::makeReturn(2006);
		}else{
			//LogManager::addLog("updateuserdata save");
			if(!$user->save())return ResultState::makeReturn(2006);
		} 

		$r=$user->getArrayData();
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
	while($other = UserData::getRowByQueryWithRandom("order by rand() limit $limit","memberID,nick,lastDate,flag,highPiece,highScore")){
		if(!in_array($other["memberID"],$friendlist)){
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



public static function saveIntroducer($p){
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

	$exchage2 = new Exchange("introduce2");
	$param["memberID"]=$user->memberID;
	$param["sender"]=$introducer->nick;
	$param["content"]=$content;
	$param["exchangeID"]=$exchage2->id;
	$param["reward"]=$exchage2->list;
	$sR2 = self::sendgiftboxhistory($param);

	CommitManager::setSuccess($memberID,ResultState::successCheck($sR2["result"]));

	if(CommitManager::commit($memberID)){
		return $sR2;
	}else{
		return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
	}

	$r =  ResultState::makeReturn(ResultState::GDSUCCESS);
	$r["introducerID"]=$user->introducerID;
	return $r;
}


public static function completeIntroducer($p){
	$memberID = $p["memberID"];
	$content = $p["content"];
	
	$user = UserData::create($memberID);

	if(!$user->introducerID)return ResultState::makeReturn(ResultState::GDPARAMETER);

	if($user->introducerID==-1) ResultState::makeReturn(ResultState::GDALREADY);
	
	$introducer = UserData::create($user->introducerID);
	if(!$introducer->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND);
	
	if($memberID==$introducer->memberID)return ResultState::makeReturn(ResultState::GDDONTFIND);

	if($user->save()){	
		
		CommitManager::begin($introducer->memberID);
		$exchage1 = new Exchange("introduce1");


		$introducer->introduceCnt=$introducer->introduceCnt+1;
		CommitManager::setSuccess($introducer->memberID,$introducer->save());

		$param["memberID"]=$introducer->memberID;
		$param["sender"]=$user->nick;
		$param["content"]=$content;
		$param["exchangeID"]=$exchage1->id;
		$param["reward"]=$exchage1->list;
		$sR1 = self::sendgiftboxhistory($param);

		CommitManager::setSuccess($memberID,ResultState::successCheck($sR1["result"]));
		
		if(CommitManager::commit($memberID)){
			return $sR1;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
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

	if($userInfo->save())return ResultState::makeReturn(ResultState::GDSUCCESS);

	$friendInfo = UserData::create($friendID);
	$ffriendlist =& $friendInfo->getRef("friendList");

	foreach ($ffriendlist as $key => $value) {
		if($value==$memberID){
			unset($ffriendlist[$key]);
		}
	}

	$friendInfo->save();

	return ResultState::makeReturn(ResultState::GDDONTSAVE);
}
public static function sendmessage($p){
	//친구제안메세지보내기

	if($p["exchangeID"]){
		$exchange = new Exchange($p["exchangeID"]);
		if(!$exchange->isLoaded())ResultState::makeReturn(ResultState::GDDONTFIND);
		$param["exchangeID"]=$exchange->id;
		$param["reward"]=$exchange->list;
		
	}

	if($p["template"]){
		$msgTemp = new CommonSetting($p["template"]);

		if($msgTemp->isLoaded()){
			$msgData =& $msgTemp->getRef("value");
			$p=array_merge($p,$msgData);
		}

	}

	$param["memberID"]=$p["friendID"];
	$param["sender"]=$p["nick"];
	$param["content"]=$p["content"];
	$param["data"]=$p["data"];
	
	$sR = self::sendgiftboxhistory($param);
	
	return $sR;

}

public static function addweeklyscore($p){

	$ws =new WeeklyScore($p["memberID"],TimeManager::getCurrentWeekNo());
	$ws->memberID = $p["memberID"];
	$ws->data = $p["data"];
	$ws->regDate = TimeManager::getCurrentDateTime();
	$ws->regTime = TimeManager::getTime();
	$ws->regWeek = TimeManager::getCurrentWeekNo();
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
	$weekNo = TimeManager::getCurrentWeekNo();
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
	
	
	}else if($userInfo->eventCheckWeek!=TimeManager::getCurrentWeekNo()){
	
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

	if($userInfo->eventCheckWeek!=TimeManager::getCurrentWeekNo()){
		$userInfo->eventCheckWeek = TimeManager::getCurrentWeekNo();
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




public static function getstagerankbyalluser($p){

	$stageNo = $p["stageNo"];
	$myscore = $p["myScore"];
	$memberID = $p["memberID"];
	
	if(!$stageNo)return ResultState::makeReturn(2002,"stageNo");


	$myRank = new StageScore($memberID,$stageNo);
	$myRank->memberID = $memberID;
	$myRank->stageNo = $stageNo;
	
	if($p["data"])$myRank->data = $p["data"];

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

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$cardNo)return ResultState::makeReturn(2002,"cardNo");
	
	$cardInfo = new Card($cardNo);
	if(!$cardInfo->isLoaded()){
		return ResultState::makeReturn(ResultState::GDDONTFIND);
	}

	//보내기
	$obj = new CardHistory($memberID,$cardNo);
	$obj->memberID = $memberID;
	$obj->cardNo = $cardNo;
	
	if(!$obj->isLoaded()){
		$obj->level = $cardInfo->level;
	}

	if($addCount){
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

	if(!$obj->save())return ResultState::makeReturn(2014,"dont save");

	$r["data"]=$obj->getArrayData(true);
	$r["write"]="success";	
	$r["result"]=ResultState::successToArray();
	return $r;
}



public static function getcardhistory($p){	
	
	$memberID=$p["memberID"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$dataList = array();
    while($rData = CardHistory::getRowByQueryWithShardKey("where memberID='".$memberID."'",$memberID)){
    	$rData["comment"]=stripslashes($rData["comment"]);
		$dataList[]=$rData;
    }

    $q = Card::getQueryResultWithShardKey("select max(`serial`) from ".Card::getDBTable(),1);
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

	foreach ($list as $key => $value) {
		$param=array();

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
							$userHistory->exchangeID=$p["exchangeID"];
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
							$userHistory->exchangeID=$p["exchangeID"];
							$userHistory2->regDate = TimeManager::getCurrentDateTime();
							CommitManager::setSuccess($memberID,$userHistory2->save());
						}

						//CommitManager::setSuccess($memberID,$fRuby->save() && $pRuby->save());
					}

					$rs[]=array("type"=>"r","count"=>($userStorage->fr+$userStorage->pr));
				}

				//아니면 적용

			}

		//카드획득
		}else if($value["type"]=="cd"){
			$ucp["memberID"]=$memberID;
			$ucp["cardNo"]=$value["count"];
			$ucp["updateTakeDate"]=true;
			$ucp["addCount"]=1;
			$result = self::updatecardhistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);
		//피스획득
		}else if($value["type"]=="cdm"){
			$ucp["memberID"]=$memberID;
			$ucp["cardNo"]=$value["count"];
			$ucp["updateTakeDate"]=true;
			$ucp["addCount"]=-1;
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
		//캐릭터업글
		}else if($value["type"]=="cu"){
			$char = new CharacterHistory($memberID,$value["count"]);
			$ucp["memberID"]=$memberID;
			$ucp["levelup"]=true;
			$ucp["characterNo"]=$value["count"];
			$result = self::updatecharacterhistory($ucp);
			$rs[]=array("type"=>$value["type"],"result"=>$result);

		//캐릭터획득
		}else if($value["type"]=="cp"){
			$ucp["memberID"]=$memberID;
			$ucp["characterNo"]=$value["count"];
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


		if($value["type"]!="r" && $value["type"]!="fr" && $value["type"]!="pr"){
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
	
	$rs[]=array("type"=>"r","count"=>($userStorage->fr+$userStorage->pr));
	
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

	//////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	$r=array();
	$rs=array();

	$minusProperty = "";
	$minusPropertyValue = 0;

	//if(count($list)==1 && !$list[0]["valu"])return ResultState::makeReturn(1);

	CommitManager::begin($memberID);

	foreach ($list as $key => $value) {
		$param=array();

		//젬인경우 특수처리
		if(!$value["type"] || $value["count"]==0){
			continue;
		}else if($value["type"]=="r"){
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

				$result = self::addProperty($param);	
				if(!ResultState::successCheck($result["result"])){
					if($result["result"]["code"]==2015){
						$r["result"]=ResultState::toArray(2015);
					}
					CommitManager::setSuccess($memberID,false);
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
					CommitManager::setSuccess($memberID,false);
				}else{
					//무료젬가 충분하다면 무료젬만 사용
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
						$userHistory->regDate = TimeManager::getCurrentDateTime();
						CommitManager::setSuccess($memberID,($userHistory->save() && $fRuby->save()));

					//무료젬가 부족하면 뮤료젬=0, 남은건 유료젬에서 차감
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
							$userHistory1->regDate = TimeManager::getCurrentDateTime();
							CommitManager::setSuccess($memberID,$userHistory1->save());
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
							$userHistory2->regDate = TimeManager::getCurrentDateTime();
							CommitManager::setSuccess($memberID,$userHistory2->save());
						}

						CommitManager::setSuccess($memberID,$fRuby->save() && $pRuby->save());
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

			$result = self::addProperty($param);	
			if(!ResultState::successCheck($result["result"])){
				if($result["result"]["code"]==2015){
					$r["result"]=ResultState::toArray(2015);
					$minusProperty=$result["minusType"];
					$minusPropertyValue = $result["minusCount"];

				}
				CommitManager::setSuccess($memberID,false);
				$rs[]=array("type"=>$result["data"]["type"],"count"=>$result["data"]["count"]);
			}else{
				$rs[]=array("type"=>$result["data"]["type"],"count"=>$result["data"]["count"]);
			}
		}
	}

	$r["list"]=$rs;

	if($minusProperty){$r["minusType"]=$minusProperty; $r["minusCount"]=$minusPropertyValue;}
	if(!CommitManager::isSuccess($memberID)){
		CommitManager::rollback($memberID);
		$r["result"]=ResultState::toArray(2013,"dont success");
		
	}else{
		if(CommitManager::commit($memberID))$r["result"]=ResultState::successToArray();
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
	
	return true;
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
	$gb = new GiftBoxHistory($memberID);
	$gb->memberID = $memberID;
	$gb->sender = $p["sender"];
	$gb->content = $p["content"];
	$gb->reward = $p["reward"];
	$gb->data = $p["data"];

	if($p["exchangeID"]){
		$exchange = new Exchange($p["exchagneID"]);
		$gb->exchangeID = $p["exchangeID"];
		$gb->exchangeList = $exchange->list;
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
		$param["list"]=$obj->exchangeList;
		$cResult = self::exchange($param);
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
	LogManager::addLog("->".mysql_error());
	$param["exchangeIDList"]=GiftBoxHistory::getAllExchangeID($memberID);
	if(count($param["exchangeIDList"])<=0)return ResultState::makeReturn(ResultState::GDSUCCESS);
	LogManager::addLog("->".mysql_error()."exchangeIDList->".json_encode($param["exchangeIDList"]));
	//LogManager::addLog("exchangeIDlist is ".json_encode($param["exchangeIDList"]));
	$cResult = self::exchangebylist($param);
	LogManager::addLog("->".mysql_error()."exchangeIDList->".json_encode($cResult));

	CommitManager::setSuccess($memberID,GiftBoxHistory::confirmAll($memberID));

	LogManager::addLog("->".mysql_error());
	$error = "";
	$error .= mysql_error();
	LogManager::addLog("->".mysql_error());
	if(ResultState::successCheck($cResult["result"])){
	LogManager::addLog("->".mysql_error());	//LogManager::addLog("cresult is true");
		CommitManager::setSuccess($memberID,true);
	}else{
		//LogManager::addLog("cresult is false ".json_encode($cResult));
	LogManager::addLog("->".mysql_error());
		CommitManager::setSuccess($memberID,false);
	}
	
	LogManager::addLog("->".mysql_error());
	$error .= mysql_error();
	if(CommitManager::commit($memberID)){
	LogManager::addLog("->".mysql_error());
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

	$where = "where memberID=".$memberID;
	$where = $where." and confirmDate='' limit 1";

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
    	$character = new Character($rData["characterNo"]);
		$powerInfo = $character->getPowerInfo($rData["level"]);
		$rData=array_merge($rData,$powerInfo);

    	$dataList[]=$rData;
    }

	$r["list"]=$dataList;
	$r["result"]=ResultState::successToArray();
	return $r;


}

public static function updatecharacterhistory($p){	
	
	$memberID=$p["memberID"];
	$characterNo=$p["characterNo"];
	LogManager::addLog("param - ".json_encode($p));
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	if(!$characterNo)return ResultState::makeReturn(2002,"characterNo");
	
	//보내기
	$obj = new CharacterHistory($memberID,$characterNo);
	$obj->memberID = $memberID;
	$obj->characterNo = $characterNo;
	if(!$obj->isLoaded()){
		$obj->regDate = TimeManager::getCurrentDateTime();
	}

	if($p["level"])$obj->level = $p["level"];
	if($p["levelup"])$obj->level +=1;
	if($obj->level==0)$obj->level=1;

	$r=array();
	
	if($obj->save()){
		$r = $obj->getArrayData(true);
		$character = new Character($obj->characterNo);
		$powerInfo = $character->getPowerInfo($obj->level);
		$r=array_merge($r,$powerInfo);
		$r["result"]=ResultState::successToArray();
	}else{
		$r["result"]=ResultState::toArray(2014,"dont save");
	}
	
	return $r;
}

public static function gettimeinfo($p){
	//$r["test"]=$p["test"];
	//if($p["offset"])TimeManager::setTimeOffset($p["offset"]);
	$r["timestamp"]=TimeManager::getTime();
	$r["weekNo"]=TimeManager::getCurrentWeekNo();
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
		}else if($tMission["type"]){
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

		if($rewardRnd>95)$tMission["rewardInfo"]=$tmReward[0];		//이어하기권
		else if($rewardRnd>85)$tMission["rewardInfo"]=$tmReward[1]; //업그레이드
		else if($rewardRnd>70)$tMission["rewardInfo"]=$tmReward[2]; //99가챠
		else if($rewardRnd>40)$tMission["rewardInfo"]=$tmReward[3]; //뽑기
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
		$tMission["count"]=0;
		$tLevel[$tMission["type"]-1]+=1;
		// if(!ResultState::successCheck($result["result"])){
		// 	CommitManager::setSuccess($memberID,false);
		// }


		$param["memberID"]=$memberID;
		$param["sender"]="GM";
		$param["content"]="오늘의미션보상";
		$param["exchangeID"]=$tMission["rewardInfo"]["exchangeID"];
		$param["reward"]=$tMission["rewardInfo"]["reward"];
		$sR = self::sendgiftboxhistory($param);
		if(!ResultState::successCheck($sR["result"])){
			CommitManager::setSuccess($memberID,false);
		}
	}

	//$user->TMInfo = $tMission;

	if(!$user->save())CommitManager::setSuccess($memberID,false);

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
	$result["changes"]=$exchange->list;
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
	}

	$result = self::changeuserproperties($pByExchange);

	return $result;

}
public static function checkloginevent($p){

	$memberID  = $p["memberID"];
	if(!$memberID)return ResultState::makeReturn(2002,"memberID");
	
	CommitManager::begin($memberID);
	
	$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
	$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
	$userData = UserData::create($memberID);
	$todayDateTime = TimeManager::getCurrentDateTime();
	$todayDate = TimeManager::getCurrentDate();
	$nowTime = TimeManager::getCurrentTime();
	$eventCheckData =& $userData->getRef("eventCheckData");

	$result = LoginEvent::getQueryResult("select ".LoginEvent::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".LoginEvent::getDBTable()." left join ".Exchange::getDBTable()." on ".LoginEvent::getDBTable().".exchangeID=".Exchange::getDBTable().".id  where startDate<$todayDateTime and endDate>$todayDateTime and os&".$osBit.">0 and cc&".$ccBit.">0");
	LogManager::addLog("loginevent1->".mysql_error());
	while($rData = mysql_fetch_assoc($result)){

		LogManager::addLog("data->".json_encode($rData));
		$idx=-1;
		for($i=0;$i<count($eventCheckData);$i++){
			if($eventCheckData[$i]["no"]==$rData["no"]){
				$idx = $i;
				break;
			}
		}

		if($idx!=-1 && (($rData["repeat"] && $eventCheckData[$idx]["date"]!=$todayDate) || ($todayDate!=$eventCheckData[$idx]["date"]))){
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
	LogManager::addLog("loginevent2->".mysql_error());
	CommitManager::setSuccess($memberID,$userData->save());
	LogManager::addLog("loginevent3->".mysql_error());
	if(CommitManager::commit($memberID)){
		return ResultState::makeReturn(1);
	}

	LogManager::addLog("loginevent5->".mysql_error());
	return ResultState::makeReturn(1001);

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////

public static function getendlessrank($p){

	$memberID = $p["memberID"];
	$start = $p["start"];
	$limit = $p["limit"];
	$weekNo = TimeManager::getCurrentWeekNo();
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
	$r["level"]=$myRank->level<=0?0:$myRank->level;
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
	$weekNo = TimeManager::getCurrentWeekNo();
	if($p["weekNo"])$weekNo=$p["weekNo"];

	if(!$memberID)return ResultState::makeReturn(2002,"memberID");

	$userData = UserData::create($memberID);

	$myRank = new EndlessRank($memberID,$weekNo);
	$myRank->memberID=$memberID;
	$myRank->regWeek=$weekNo;

	$r["score"]=$myRank->score;
	$r["victory"]=$myRank->victory;
	$r["level"]=$myRank->level<=0?0:$myRank->level;
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

	$endlessRank = new EndlessRank($memberID,TimeManager::getCurrentWeekNo());

	$endlessRank->memberID=$memberID;
	$endlessRank->nick = $p["nick"];
	$endlessRank->level = $p["level"];
	$endlessRank->flag = $p["flag"];
	
	
	if($endlessRank->score<$p["score"]){
		$endlessRank->score = $p["score"];
		$endlessRank->regDate = TimeManager::getCurrentDateTime();
	}

	if($endlessRank->victory<$p["victory"]){
		$endlessRank->victory = $p["victory"];
		$endlessRank->regDate = TimeManager::getCurrentDateTime();
	}
	
	$endlessRank->regWeek = TimeManager::getCurrentWeekNo();
	
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

	$endlessRank = new EndlessRank($memberID,TimeManager::getCurrentWeekNo());
	$userData = UserData::create($memberID);
	$endlessData =& $userData->getRef("endlessData");

	//위크번호다르면 초기화~
	if($endlessData["ing_week"]!=TimeManager::getCurrentWeekNo()){
		//LogManager::addLog("reset ing_win , bcus new week");
		$endlessData["ing_score"]=0;
		$endlessData["ing_win"]=0;
		$endlessData["ing_week"]=TimeManager::getCurrentWeekNo();
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
	$endlessRank->level = $userData->getLevel();
	$endlessRank->flag = $p["flag"];
	
	//주간최고기록갱신
	if($endlessData["ing_score"]>$endlessRank->score)$endlessRank->score=$endlessData["ing_score"];
	if($endlessData["ing_win"]>$endlessRank->victory)$endlessRank->victory=$endlessData["ing_win"];

	$endlessRank->regDate = TimeManager::getCurrentDateTime();
	$endlessRank->regWeek = TimeManager::getCurrentWeekNo();
	
	$r["sendGift"]=false;
	//졌으면 끄읏
	if(!$p["victory"]){
		$endlessData["ing_win"]=0;
		$endlessData["ing_score"]=0;
		$endlessData["ing_week"]=TimeManager::getCurrentWeekNo();
		$endlessData["lose"]+=1;
		$endlessData["ing_level"]=0;
	}else{
		$cs = new CommonSetting("pvpReward");
		$ew =& $cs->getRef("value");
		if($endlessData["ing_win"]%$ew["win"]==0){
			$r["sendGift"]=true;
			$r["reward"]=$ew["reward"];
			$param["memberID"]=$memberID;
			$param["sender"]="GM";
			$param["content"]=$ew["title"];
			$param["exchangeID"]=$ew["exchangeID"];
			$param["reward"]=$ew["reward"];
			$sR = self::sendgiftboxhistory($param);
			if(!ResultState::successCheck($sR["result"])){
				//LogManager::addLog("sendgift save fail");
				CommitManager::setSuccess($memberID,false);
			}
		}
	}
	
	CommitManager::begin($memberID);

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

	if($endlessData["ing_state"]==1 || $endlessData["ing_week"]!=TimeManager::getCurrentWeekNo()){
		$endlessData["ing_win"]=0;
		$endlessData["lose"]+=1;
		$endlessData["ing_score"]=0;
		$endlessData["ing_week"]=TimeManager::getCurrentWeekNo();
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
	
	$r["rival"] = $rival2[0];
	unset($rival2[0]["playData"]);
	$r["dummy"] = $rival2;
	
	$sp["no"]=$rival["pieceNo"];

	//if(!$sp["no"])
	
	srand((double)microtime()*1000000);
	$sp["no"]=rand(1,$maxStage);

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


	//LogManager::addLog("== check checkattendenceevent ==========================");

	//아래코드를 초기화하면 출첵이 계속 뜬다
	if($userData->eventCheckDate==TimeManager::getCurrentDate()){
		$r["result"]=ResultState::successToArray();
		$r["dayList"]=LoginEvent::getRewardDays();
		return $r;
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

	$result = AttendenceEventDay::getQueryResult("select ".AttendenceEventDay::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".AttendenceEventDay::getDBTable()." left join ".Exchange::getDBTable()." on ".AttendenceEventDay::getDBTable().".exchangeID=".Exchange::getDBTable().".id where ".AttendenceEventDay::getDBTable().".eventNo='".$userData->eventAtdNo."' order by day asc");
	LogManager::addLog("AttendenceEventDay->".mysql_error());
	while($shopInfo = mysql_fetch_assoc($result)){
	//while($dData = AttendenceEventDay::getRowByQuery("where eventNo='".$userData->eventAtdNo."' order by day asc")){
		$dNo = $dData["day"]-1;
		$rewards = json_decode($dData["reward"]);
		if($dNo==$userData->eventAtdCount){
			$rewardInfo=array("reward"=>$rewards,"exchangeID"=>$dData["exchangeID"],"title"=>$dData["title"]);
		}
		$rewardList[$dNo]=$rewards[0];
	}

	//보상지급
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
	}
	$r["dayCount"]=$userData->eventAtdCount+1;
	$r["rewardList"]=$rewardList;
	$r["dayList"]=LoginEvent::getRewardDays();
	$userData->eventAtdCount++;
	$userData->eventCheckDate=TimeManager::getCurrentDate();
	if(!$userData->save()){
		CommitManager::setSuccess($memberID,false);
	}

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

	LogManager::addLog("sendcardbyuser : ".json_encode($csInfo));
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
	LogManager::addLog("mysqlError1:".mysql_error());
	$cardInfo->count-=1;

	CommitManager::setSuccess($memberID,$cardInfo->save());
	LogManager::addLog("mysqlError1:".mysql_error());
	if(!CommitManager::commit($memberID)){
		return ResultState::makeReturn(ResultState::GDDONTSAVE,"mysql_eror :".mysql_error());
	};

	$exchangeInfo = new Exchange($csInfo["exchangeID"]);
	if(!$exchangeInfo->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND);

	$param["memberID"]=$toMemberID;
	$param["sender"]=$userData->nick;
	$param["content"]=$csInfo["content"];
	$param["exchangeID"]=$exchangeInfo->id;
	$param["reward"]=$exchangeInfo->list;
	$param["exchangeList"]=array(array("type"=>"cd","count"=>$cardNo,"level"=>$cardInfo->level));

	
	$sR = self::sendgiftboxhistory($param);
	//CommitManager::setSuccess($mID,ResultState::successCheck($sR["result"]));
	//$r["result"]=ResultState::toArray(ResultState::GDSUCCESS);
	return $sR;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
}

?>