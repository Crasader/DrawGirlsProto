<?php 
include_once("DBManagerLib.php");
include_once("DBManagerLib2.php");

class CommitManager{
	

	static public $m_dbMaster=null;
	static public $m_userIndex=null;
	static public $m_isSuccess=null;
	static public $m_releaseCount=null;
	static public $m_passFunc=false;

	static public function construct($memberID=null){
		self::$m_userIndex=array();
		self::$m_dbMaster=array();
		self::$m_isSuccess=array();
		self::$m_releaseCount=array();
	}


	static public function begin($memberID){
		if(!self::$m_releaseCount[$memberID]){
			self::$m_releaseCount[$memberID]=1;
			
			//user db
			if($memberID!="main"){
				self::$m_userIndex[$memberID] = UserIndex::create($memberID);

				self::$m_dbMaster[$memberID] = self::$m_userIndex[$memberID]->getDBMaster();
			//main db
			}else{
				self::$m_dbMaster[$memberID] = DBGroup::create("main")->getMaster(1);
			}

			self::$m_isSuccess[$memberID]=true;
			mysql_query("START TRANSACTION",self::$m_dbMaster[$memberID]->getConnection());
			
		}else{
			self::$m_releaseCount[$memberID]++;
			
		}
	}

	static public function commit($memberID){
		if(!self::$m_releaseCount[$memberID]) return false;

		self::$m_releaseCount[$memberID]--;

		if(!self::isSuccess($memberID)){
			if(self::$m_releaseCount[$memberID]==0){
				$result = mysql_query("ROLLBACK", self::$m_dbMaster[$memberID]->getConnection());
			}
			return false;
		}

		if(self::$m_releaseCount[$memberID]==0){
			$result = mysql_query("COMMIT", self::$m_dbMaster[$memberID]->getConnection());
		}else{
			$result = true;
		}

		
		return $result;
	}

	static public function rollback($memberID){
		if(!self::$m_releaseCount[$memberID]) return false;

		self::$m_releaseCount[$memberID]--;

		if(self::$m_releaseCount[$memberID]==0){
			$result = mysql_query("ROLLBACK", self::$m_dbMaster[$memberID]->getConnection());
		}else{
			self::setSuccess($memberID,false);
			$result=true;
		}
		return $result;
	}

	static public function setSuccess($memberID,$success){
		if(!$success)LogManager::addLog("fuck the success fail ".mysql_error());
		
		if(self::$m_isSuccess[$memberID]==false)return;

		self::$m_isSuccess[$memberID]=$success;
	}

	static public function isSuccess($memberID){
		return self::$m_isSuccess[$memberID];
	}
}

CommitManager::construct();

class UserIndex extends DBTable2{


	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aUserShardIndex");


		self::setLQTableSelectCustomFunction(function($data){
			$userdata = UserData::create($data["memberID"]);
			$data = array_merge($data,$userdata->getArrayData(false));
			return $data;
		});
	}

	public static $sharedIndexes=array();

	public static function create($memberID=null,$userindex=null,$socialID=null,$nick=null){
		self::initial();

		if($memberID && self::$sharedIndexes[$memberID]){
			return self::$sharedIndexes[$memberID];
		}


		$newIndex =new UserIndex($memberID,$userindex,$socialID,$nick);

		if($newIndex->isLoaded()){
			self::$sharedIndexes[$newIndex->memberID]=$newIndex;
		}
		return $newIndex;
	}

	public function __construct($memberID=null,$userindex=null,$socialID=null,$nick=null){

		parent::__construct();
		
		
		if($userindex){
			parent::load("no=".$userindex);

		}else if($memberID){
		 	if(parent::load("memberID='".$memberID."'")){
		 		
		 	}else{
				$this->memberID = $memberID;
		 		$this->userShardOrder = UserData::newShardkeyFunc($memberID);
		 		$this->logShardOrder = UserLog::newShardkeyFunc($memberID);
		 		
		 	}
	 	}else if($socialID){
			parent::load("socialID=".$socialID);
	 	}else if($nick){
			parent::load("nick='".$nick."'");
	 	}
	}


	public function getUserIndex(){
		return $this->no;
	}


	public function remove(){
		return parent::remove();
	}


	public function save($isIncludePrimaryKey = false){
		return parent::save($isIncludePrimaryKey);
	}

	public function getDBMaster(){
		return self::getDBGroup()->getMaster(1);
	}

	public static function getNickName($param){
		$r = array();
		for($i=0;$i<count($param);$i++){
			$p = $param[$i];
			
			if($p["type"]=="sno")$user = UserIndex::create($p["find"]);
			else $user = UserIndex::create(null,null,null,$p["find"]);

			if(!$user->isLoaded()){
				return ResultState::makeReturn(ResultState::GDDONTFIND,$p["find"]."를 찾을수없습니다.");
			}


			$result["nick"]=$user->nick;
			$result["memberID"]=$user->memberID;
		
			$r["list"][]=$result;
		}
		$r["result"]=ResultState::toArray(ResultState::GDSUCCESS);
		return $r;

	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UserLog extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("log");
		self::setDBTable("UserLog_".TimeManager::getCurrentDate());
		self::setShardKey("memberID");
	}

	public function __construct($memberID=null,$no=null){
		parent::__construct();
		
		$this->memberID = $memberID;
		$this->setDBShardKeyValue($memberID);
		
		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["findDate"]){
				self::setDBTable("UserLog_".$param["where"]["findDate"]);
			}
			if($param["where"]["category"]){
				return "where category = '".$param["where"]["category"]."'";
			}
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});



		if($memberID && $no){
			parent::load("no=".$no." and memberID='".$memberID."'");
		}
	}

	public function save($isIncludePrimaryKey=false){
		//LogManager::addLog("userLog save!!");
		$save = parent::save($isIncludePrimaryKey);
		if(!$save){
			//LogManager::addLog("userLog save fail and createnew");

			$createStr = "
			CREATE TABLE `".self::getDBTable()."` (
			  `no` bigint(20) NOT NULL AUTO_INCREMENT,
			  `memberID` bigint(20) NOT NULL,
			  `ip` varchar(30) NOT NULL,
			  `header` text NOT NULL,
			  `category` varchar(100) NOT NULL,
			  `input` text NOT NULL,
			  `output` text NOT NULL,
			  `regDate` bigint(20) NOT NULL,
			  `execTime` float NOT NULL,
			  `regTime` bigint(20) NOT NULL,
			  PRIMARY KEY (`no`),
			  KEY `memberID` (`memberID`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
			";
			
			$state = self::getQueryResultWithShardKey($createStr,$this->memberID,false);
			
			if($state){
				$save = parent::save($isIncludePrimaryKey);
			}
		}

		return $save;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SendItem extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("UserLogTable");
		self::setShardKey("memberID");
	}

	public function __construct($memberID=null,$socialID=null,$nick=null){
		parent::__construct();
	}


	public static function selectWithLQForm($param){
		$user=null;

		if($param["where"]["type"]=="sno"){
			$user = UserData::create($param["where"]["id"]);
		}else if($param["where"]["type"]=="nick"){
			$user = UserData::create(null,null,$param["where"]["id"]);
		}else{
			$user = UserData::create(null,$param["where"]["id"]);
		}

		if(!$user->isLoaded()){
			$r = ResultState::makeReturn(ResultState::GDDONTFINDUSER,"유저를 찾지 못했습니다");
			return $r;
		}
		
		$r=$user->getArrayData(true);
		$r["memberList"]=array($user->memberID=>$user->nick);
		$r["sendType"]="";
		$r["exchangeID"]="";
		$result["data"]=$r;
		$result["result"]=ResultState::successToArray();
		return $result;
	}
	public static function sendAllUser($param){
		$data = $param["data"];

		if(!$data["country"]){
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"대상국가를 입력해주세요.");
		}

		$exchange = new Exchange($data["exchangeID"]);

		if(!$exchange->isLoaded())return ResultState::makeReturn(ResultState::GDDONTSAVE,"교환정보 로드실패");

		$result["exchangeID"]=$exchange->id;
		$result["reward"]=$exchange->list;
		$rList = array();


		$memberCnt=0;
		while($userindex = UserIndex::getRowByQuery()){

			$mID = $userindex["memberID"];
			$nick = $userindex["nick"];
			
			if($data["country"]!="all"){
				$userData = new UserData($mID);
				if(!$userData->isLoaded())continue;
				if($userData->country!=$data["country"])continue;
			}

			CommitManager::begin($mID);

			if($data["sendType"]["type"]=="giftbox"){
				$param["memberID"]=$mID;
				$param["sender"]="GM";
				$param["content"]=$data["sendType"]["message"];
				$param["exchangeID"]=$exchange->id;
				$param["reward"]=$exchange->list;
				$sR = commandClass::sendgiftboxhistory($param);
				CommitManager::setSuccess($mID,ResultState::successCheck($sR["result"]));
			
			//바로지급
			}else{

				$param["memberID"]=$mID;
				$param["exchangeID"]=$exchange->id;
				$sR = commandClass::exchange($param);
				CommitManager::setSuccess($mID,ResultState::successCheck($sR["result"]));
			}


			if(CommitManager::commit($mID)){
				$rList[$mID]=array("result"=>"success","nick"=>$nick);
				$memberCnt++;
			}else{
				$rList[$mID]=array("result"=>"fail","nick"=>$nick);
			}

		}


		$result["list"]=$rList;
		$result["result"]=ResultState::successToArray($memberCnt."명에게 전송되었습니다.");
		return $result;
	}

	public static function updateWithLQForm($param){
		$data = $param["data"];

		//LogManager::addLog(json_encode($data));



		$exchange = new Exchange($data["exchangeID"]);

		if(!$exchange->isLoaded() && $data["exchangeID"])return ResultState::makeReturn(ResultState::GDDONTSAVE,"교환정보 로드실패");

		$result["exchangeID"]=$exchange->id;
		$result["reward"]=$exchange->list;
		$rList = array();
		foreach ($data["memberList"] as $mID => $nick) {
			CommitManager::begin($mID);
			
			$userInfo = UserData::create($mID);

			if(!$userInfo->isLoaded()){
				$rList[$mID]=array("result"=>"fail","nick"=>$nick);
				continue;
			}

			//선물상자로 지급
			if($data["sendType"]["type"]=="giftbox"){
				$param["memberID"]=$mID;
				$param["sender"]="GM";
				$param["content"]=$data["sendType"]["message"];
				$param["data"]=$data["sendType"]["data"];
				$param["exchangeID"]=$exchange->id;
				$param["reward"]=$exchange->list;
				$sR = commandClass::sendgiftboxhistory($param);
				CommitManager::setSuccess($mID,ResultState::successCheck($sR["result"]));
			
			//바로지급
			}else{

				$param["memberID"]=$mID;
				$param["exchangeID"]=$exchange->id;
				$sR = commandClass::exchange($param);
				CommitManager::setSuccess($mID,ResultState::successCheck($sR["result"]));
			}

			if(CommitManager::commit($mID)){
				$rList[$mID]=array("result"=>"success","nick"=>$nick);
			}else{
				$rList[$mID]=array("result"=>"fail","nick"=>$nick);
			}
		}
		$result["list"]=$rList;
		$result["result"]=ResultState::successToArray("전송되었습니다.");
		return $result;
	}

}

class UserData extends DBTable2{
	public static $sharedUserDatas=array();



	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("UserDataTable");
		self::setShardKey("memberID");
	}


	public static function create($memberID=null,$socialID=null,$nick=null){
		
		if($memberID && self::$sharedUserDatas[$memberID]){
			return self::$sharedUserDatas[$memberID];
		}


		$newUserData =new UserData($memberID,$socialID,$nick);

		if($newUserData->isLoaded()){
			self::$sharedUserDatas[$newUserData->memberID]=$newUserData;
		}	
		return $newUserData;
	}



	public function __construct($memberID=null,$socialID=null,$nick=null){
		parent::__construct();
		
		$this->memberID = $memberID;
		$this->setDBShardKeyValue($memberID);
		self::setLQTableSelectCustomFunction(function ($data){
			$data["memberID"] = strval($data["memberID"]);
			return $data;
		});

		if($memberID || $socialID || $nick){
			$userIndex= UserIndex::create($memberID,null,$socialID,$nick);
			
			if($userIndex && $userIndex->isLoaded()){
				$this->memberID=$userIndex->memberID;
				if(parent::load("memberID='".$userIndex->memberID."'")){
					$this->nick=$userIndex->nick;
					// $this->jsonToObj("archiveData");
					// $this->jsonToObj("eventCheckData");
					// $this->jsonToObj("TMInfo");
					// $this->jsonToObj("TMLevel");
					// $this->jsonToObj("endlessData");
				}
			}
		}
	}

	public function save($isIncludePrimaryKey=false){
		//마지막접속시간
		$this->lastDate = TimeManager::getCurrentDateTime();
		$this->lastTime = TimeManager::getTime();
		return parent::save($isIncludePrimaryKey);
	}


	public function getUserIndexNo(){
		$userindex = UserIndex::create($this->memberID);
		return $userindex->getUserIndex();
	}

	public function getUserIndex(){
		$userindex = UserIndex::create($this->memberID);
		return $userindex;
	}

	public function getLevel(){
		return intval($this->exp/10+1);
	}
	
	public function getArrayDataForPublic(){
		$data=array();
		$data["memberID"]=$this->memberID;
		$data["nick"]=$this->nick;
		$data["lastDate"]=$this->lastDate;
		$data["lastTime"]=$this->lastTime;
		$data["flag"]=$this->flag;
		$data["highPiece"]=$this->highPiece;
		$data["highScore"]=$this->highScore;
		$data["introduceCnt"]=$this->introduceCnt;
		return $data;
	}

	public function getArrayData($isIncludePrimaryKey=false,$keyList=null){
		
		$data = parent::getArrayData($isIncludePrimaryKey);

		if($data["userdata"]){
			$userdata =  json_decode($data["userdata"],true);

			foreach($userdata as $key=>$value){
				if($keyList && !in_array($name,$keyList)){
					unset($userdata[$key]);
				}
			}

			$data["userdata"]=json_encode($userdata,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		}	

		return $data;
	}
	
	public function updateData($data){
		//merge
		$resultData = "{}";
		if($this->data){
			$check = json_decode($this->data,true);
			if(is_array($check)){
				$oldJson = json_decode($this->data,true);
				$newJson = json_decode($data,true);
				$mergeJson =array_merge($oldJson,$newJson);
					
				$resultData = json_encode($mergeJson,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK); 
			}else{
				$resultData = $data;
			}
		}else{
			$resultData = $data;
		}	
		
		$this->data = $resultData;
		return $this->save();
	}
	
	public function isInDB(){
		if($this->no && $this->memberID)return true;
		
		return false;
	}
	
	public function getData(){
		return $this->data;
	}
	
	public function addFriend($friendID){
		
		$friendList=array();
		if($this->friendList)$friendList = json_decode($this->friendList,true);
		
		$friendList[]=$friendID;
		
		$friendList = array_unique($friendList);		
		$this->friendList = json_encode($friendList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
	}

	public function setArchiveData($key,$value){
		$archiveData =& $this->getRef("archiveData");

		if(!is_array($archiveData))$archiveData = array();

		$archiveData = array_merge($archiveData,array($key=>$value));

	}


	public static function selectWithLQForm($param){
		$user=null;

		if($param["where"]["type"]=="sno"){
			$user = UserData::create($param["where"]["id"]);
		}else if($param["where"]["type"]=="nick"){
			$user = UserData::create(null,null,$param["where"]["id"]);
		}else{
			$user = UserData::create(null,$param["where"]["id"]);
		}

		if(!$user->isLoaded()){
			$r = ResultState::makeReturn(ResultState::GDDONTFINDUSER,"유저를 찾지 못했습니다");
			return $r;
		}
		
		$p["api"]="GetProfileDetail2";
		$p["memberNo"]=$user->memberID;
		
		$hgr = commandClass::httpgateway($p);
		
		$r=$user->getArrayData(true);
		$r = array_merge($hgr,$r);
		
		$tm =& $user->getRef("TMInfo");
		$tmLevel =& $user->getRef("TMLevel");
		
		$cntResult=PieceHistory::getQueryResultWithShardKey("select sum(tryCount) from ".PieceHistory::getDBTable()." where memberID=".$user->memberID,$user->memberID);
		LogManager::addLog("select sum(tryCount) from ".PieceHistory::getDBTable()." where memberID=".$user->memberID);
		if($cntResult)$playCnt = mysql_fetch_array($cntResult);
		else $playCnt[0]=0;

		$r["playCnt"]=$playCnt[0];

		$r["tm_type"]=$tm["type"];
		$r["tm_goal"]=$tm["goal"];
		$r["tm_count"]=$tm["count"];
		$r["tm_date"]=$tm["date"];
		$r["tm_reward"]=$tm["rewardInfo"]["reward"][0]["type"];
		$r["tm_success"]=$tm["isSuccess"];
		$r["tm_todayCnt"]=$tm["todayCnt"];

		$r["userShardOrder"] = $user->getUserIndex()->userShardOrder;
		$r["logShardOrder"] = $user->getUserIndex()->logShardOrder;

		$character = new CharacterHistory($user->memberID,$user->selectedCharNO);
		//LogManager::addLog("character".$user->memberID."-".$user->selectedCharNO);
		$r["characterLevel"]=$character->level;

		$storage = new UserStorage($user->memberID);
		$r["r"]='{"fr":'.$storage->fr.',"pr":'.$storage->pr.'}';
		$r["isConnecting"]=((TimeManager::getTime()-$user->lastTime)<300)?"Y":"N";
		$r = array_merge($r,$storage->getArrayData());
		$r["shardIndex"]=$user->getUserIndex()->userShardOrder;
		$result["data"]=$r;
		$result["result"]=ResultState::successToArray();
	    	
		return $result;
	}

	public static function updateWithLQForm($param){
		//값저장.
		//멤버아이디 안넘어왔음 넘겨달라 에러 띄울것
		//변경내역저장
		if(array_key_exists("eventAtdCount",$param["data"])){
			self::updateWithLQTable($param);

		}

		if(array_key_exists("eventCheckDate",$param["data"])){
			self::updateWithLQTable($param);
		}

		if(array_key_exists("eventAtdNo",$param["data"])){
			self::updateWithLQTable($param);

		}

		if(array_key_exists("eventCheckWeek",$param["data"])){
			self::updateWithLQTable($param);

		}

		if(array_key_exists("diaryJoinDate",$param["data"])){
			self::updateWithLQTable($param);

		}


		if(array_key_exists("missionEvent",$param["data"])){
			self::updateWithLQTable($param);
		}

		if(array_key_exists("highScore",$param["data"])){
			self::updateWithLQTable($param);

		}

		if(array_key_exists("highPiece",$param["data"])){
			self::updateWithLQTable($param);

		}

		if(array_key_exists("TMLevel",$param["data"])){
			self::updateWithLQTable($param);

		}

		if(array_key_exists("introducerID",$param["data"])){
			self::updateWithLQTable($param);

		}


		if(array_key_exists("introduceCnt",$param["data"])){
			self::updateWithLQTable($param);
		}


		if(array_key_exists("tm_date",$param["data"])){
			$user = UserData::create($param["primaryValue"]);
			$tm =& $user->getRef("TMInfo");
			$tm["date"]=$param["data"]["tm_date"];
			$user->save();
		}

		if(array_key_exists("tm_type",$param["data"])){
			$user = UserData::create($param["primaryValue"]);
			$tm =& $user->getRef("TMInfo");
			$tm["type"]=$param["data"]["tm_type"];
			$user->save();
		}

		if(array_key_exists("tm_goal",$param["data"])){
			$user = UserData::create($param["primaryValue"]);
			$tm =& $user->getRef("TMInfo");
			$tm["goal"]=$param["data"]["tm_goal"];
			$user->save();
		}

		if(array_key_exists("tm_count",$param["data"])){
			$user = UserData::create($param["primaryValue"]);
			$tm =& $user->getRef("TMInfo");
			$tm["count"]=$param["data"]["tm_count"];
			$user->save();
		}

		if(array_key_exists("tm_todayCnt",$param["data"])){
			$user = UserData::create($param["primaryValue"]);
			$tm =& $user->getRef("TMInfo");
			$tm["todayCnt"]=$param["data"]["tm_todayCnt"];
			$user->save();
		}

		if(array_key_exists("tm_success",$param["data"])){
			$user = UserData::create($param["primaryValue"]);
			$tm =& $user->getRef("TMInfo");
			$tm["isSuccess"]=$param["data"]["tm_success"];
			$user->save();
		}

		$param["where"]["type"]="sno";
		$param["where"]["id"]=$param["primaryValue"];
		return self::selectWithLQForm($param);

	}






}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
//	퍼즐정보
////////////////////////////////////////////////////////////////////////////////////////
class Puzzle extends DBTable2{


	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aPuzzleTable");
	}

	public function __construct($puzzleNo=null,$order=null){
		
		parent::__construct();

		if($puzzleNo)$this->no=$puzzleNo;
		if($order)$this->order=$order;

		if($puzzleNo || $order){
			$query = "`order`=".$order;
			if($puzzleNo)$query = "no=".$puzzleNo;

			if(parent::load($query)){

				// $this->jsonToObj("center");
				// $this->jsonToObj("face");
				// $this->jsonToObj("original");
				// $this->jsonToObj("thumbnail");
				// //$this->jsonToObj("map");
				// $this->jsonToObj("pathInfo");
				// //$this->jsonToObj("coordinateInfo");
				// //$this->jsonToObj("startPosition");
				// //$this->jsonToObj("endPosition");
				// $this->jsonToObj("color");
				// $this->jsonToObj("title");
				
			}
		}
	}

	public function getArrayDataForClient(){
		$this->jsonToObj("clearReward");
		$objInfo = $this->getArrayData(true);
		$objInfo[title]=$this->getLocalizedValue("title");

		if($objInfo["clearReward"]["normal"]){
			$ncard = new Card($objInfo["clearReward"]["normal"]);
			$objInfo["clearReward"]["normal"]=$ncard->getArrayDataForClient();
		}
		
		if($objInfo["clearReward"]["perfect"]){
			$ncard = new Card($objInfo["clearReward"]["perfect"]);
			$objInfo["clearReward"]["perfect"]=$ncard->getArrayDataForClient();
		}

		$piecelist=array();
		$startStageNo=0;

		while($piece = Piece::getObjectByQuery("where puzzle=".$objInfo[no]." order by no asc")){
			if($startStageNo==0)$startStageNo=$piece->no;
			$pieceInfo = $piece->getArrayDataForClient();
			$pieceInfo[puzzleOrder] = $puzzleInfo[order];
			$piecelist[]=$pieceInfo;
		}
		
		$objInfo["list"]=$piecelist;
		$objInfo["startStage"]=$startStageNo;
		
		return $objInfo;
	}


	public static function updateWithLQTable($p){
		reloadPuzzleInfo();
		kvManager::increase("puzzleListVer");
		$p["data"]["version"]=kvManager::get("puzzleListVer",1);
		$r = parent::updateWithLQTable($p);

		return $r;
	}

	public static function insertWithLQTable($p){
		reloadPuzzleInfo();
		kvManager::increase("puzzleListVer");
		$p["data"]["version"]=kvManager::get("puzzleListVer",1);
		$r = parent::insertWithLQTable($p);
		return $r;
	}

	public static function deleteWithLQTable($p){
		kvManager::increase("puzzleListVer");
		return parent::deleteWithLQTable($p);

	}


	static public function getPieceCount($puzzleNo){

		$alluser=0;
		$cnt=0;

		$result = mysql_query("select count(*) from ".Piece::getDBTable()." where puzzle=".$puzzleNo,DBGroup::create("main")->getConnectionForReadByRand());
		
		$data = mysql_fetch_array($result);

		return $data[0];
	}

	static public function getPieceStart($puzzleNo){

		$alluser=0;
		$cnt=0;

		$result = mysql_query("select no from ".Piece::getDBTable()." where puzzle=".$puzzleNo." order by no asc limit 1",DBGroup::create("main")->getConnectionForReadByRand());
		$data = mysql_fetch_array($result);

		return $data["no"];
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////////////////



class PuzzleEvent extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aPuzzleEventTable");
	}


	public function __construct($no=null){

		parent::__construct();


		$this->no = $no;
		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}

}


////////////////////////////////////////////////////////////////////////////////////////
//	피스정보
////////////////////////////////////////////////////////////////////////////////////////
class Piece extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",false);
		self::setDBGroup("main");
		self::setDBTable("aPieceTable");
	}

	public function __construct($no=null,$order=null){
		
		parent::__construct();
		
		
		if($no)$this->no=$no;

		if($no || $order){
			$query="";
			if($no)$query = "no=".$no;
			if($order)$query = "`order`=".$order;
			if(parent::load($query)){
				// $this->jsonToObj("condition");
				// $this->jsonToObj("shopItems");
				// $this->jsonToObj("defItems");
				// $this->jsonToObj("cards");
				// $this->jsonToObj("mission");
			};
		}
	}
	
	public function getArrayDataForClient(){
		$stageInfo = $this->getArrayData(true);
		$stageInfo["condition"] = json_decode($stageInfo["condition"],true);
		$stageInfo[shopItems]=json_decode($stageInfo[shopItems],true);
		$stageInfo[defItems]=json_decode($stageInfo[defItems],true);
		$stageInfo[cards]=json_decode($stageInfo[cards],true);
		$stageInfo[mission]=json_decode($stageInfo[mission],true);
		$card = array();

		while($myCard = Card::getObjectByQuery("where piece='".$stageInfo[no]."' order by rank asc",0)){
		//for($i=0;$i<count($stageInfo[cards]);$i++){
			//$myCard = new Card($stageInfo[cards][$i]);
			//if($myCard->isLoaded()){
				$card[]= $myCard->getArrayDataForClient();
		//	}
		}
		$stageInfo[cards]=$card;
		return $stageInfo;
	}

	public static function updateWithLQTable($p){
		CommitManager::begin("main");
		if($p["data"]["puzzle"]){
			$puzzle = new Puzzle($p["data"]["puzzle"]);
			if($puzzle->isLoaded()){
				kvManager::increase("puzzleListVer");
				$puzzle->version=kvManager::get("puzzleListVer",1);
				CommitManager::setSuccess("main",$puzzle->save());
			}else{
				kvManager::increase("hellModeListVer");
			}
		}

		kvManager::increase("pieceListVer");
		$p["data"]["version"]=kvManager::get("pieceListVer",1);
		$r = parent::updateWithLQTable($p);
		CommitManager::setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!".mysql_error());
		}
	}

	public static function insertWithLQTable($p){
		CommitManager::begin("main");
		if($p["data"]["puzzle"]){
			$puzzle = new Puzzle($p["data"]["puzzle"]);
			if($puzzle->isLoaded()){
				kvManager::increase("puzzleListVer");
				$puzzle->version=kvManager::get("puzzleListVer",1);
				CommitManager::setSuccess("main",$puzzle->save());
			}else{
				kvManager::increase("hellModeListVer");
			}
		}

		kvManager::increase("pieceListVer");
		$p["data"]["version"]=kvManager::get("pieceListVer",1);
 
		$r = parent::insertWithLQTable($p);
		CommitManager::setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!".mysql_error());
		}
	}

	public static function deleteWithLQTable($p){
		kvManager::increase("pieceListVer");
		return parent::deleteWithLQTable($p);
	}
}


class HellMode extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",false);
		self::setDBGroup("main");
		self::setDBTable("aHellModeTable");
	}

	public function __construct($no=null){
		
		parent::__construct();
		
		
		if($no)$this->no=$no;

		if($no){
			$query = "no=".$no;
			if(parent::load($query)){
			
			};
		}
	}
	
	public function getArrayDataForClient(){

		$r = $this->getArrayData(true);
		$pieceInfo = new Piece($this->openPieceNo);
		$r["pieceInfo"]=$pieceInfo->getArrayDataForClient();
		return $r;
	}

	public static function updateWithLQTable($p){
		reloadPuzzleInfo();
		kvManager::increase("hellModeListVer");
		$r = parent::updateWithLQTable($p);
		return $r;
	}
	public static function insertWithLQTable($p){
		reloadPuzzleInfo();
		kvManager::increase("hellModeListVer");
		$r = parent::insertWithLQTable($p);
		return $r;
	}

	public static function deleteWithLQTable($p){
		reloadPuzzleInfo();
		kvManager::increase("hellModeListVer");
		return parent::deleteWithLQTable($p);
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	카드정보
////////////////////////////////////////////////////////////////////////////////////////
class Card extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",false);
		self::setDBGroup("main");
		self::setDBTable("aCardTable");
	}


	public function __construct($cardNo=null){
		
		parent::__construct();

		if($cardNo)$this->no=$cardNo;


		self::setLQTableSelectQueryCustomFunction(function ($param){
			if(!$param["where"]["cardNo"] || $param["where"]["cardNo"]=="*")return "";
			
			return "where no='".$param["where"]["cardNo"]."'";
		});


		if($cardNo){
			$query = "no=".$cardNo;
			if(parent::load($query)){
				// $this->jsonToObj("imgInfo");
				// $this->jsonToObj("aniInfo");
				// $this->jsonToObj("thumbnailInfo");
				// $this->jsonToObj("silImgInfo");
				// $this->jsonToObj("mPrice");
				// $this->jsonToObj("name");
				// $this->jsonToObj("script");
				// $this->jsonToObj("sound");
			}
		}
	}

	public function getArrayDataForClient(){
		$cardInfo = $this->getArrayData(true);
		$cardInfo[stage]=$stageInfo[no];
		
		$cardInfo["name"] = $this->getLocalizedValue("name");
		$cardInfo["script"] = $this->getLocalizedValue("script");
		$cardInfo["profile"] = $this->getLocalizedValue("profile");
		return $cardInfo;
	}

	public static function updateWithLQTable($p){
		CommitManager::begin("main");
		
		if($p["data"]["piece"]){
			$piece = new Piece($p["data"]["piece"]);

			kvManager::increase("pieceListVer");
			$piece->version=kvManager::get("pieceListVer",1);
			CommitManager::setSuccess("main",$piece->save());

			if($piece->puzzle){
				$puzzle = new Puzzle($piece->puzzle);
				if($puzzle->isLoaded()){
					kvManager::increase("puzzleListVer");
					$puzzle->version=kvManager::get("puzzleListVer",1);
					CommitManager::setSuccess("main",$puzzle->save());
				}else{

					kvManager::increase("hellModeListVer");

				}
			}
		}
		
		kvManager::increase("cardListVer");
		$p["data"]["version"]=kvManager::get("cardListVer",1);

		$r = parent::updateWithLQTable($p);
		CommitManager::setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
		}

	}

	public static function insertWithLQTable($p){
		CommitManager::begin("main");
		
		if($p["data"]["piece"]){
			$piece = new Piece($p["data"]["piece"]);

			kvManager::increase("pieceListVer");
			$piece->version=kvManager::get("pieceListVer",1);
			CommitManager::setSuccess("main",$piece->save());

			if($piece->puzzle){
				$puzzle = new Puzzle($piece->puzzle);
				if($puzzle->isLoaded()){
					kvManager::increase("puzzleListVer");
					$puzzle->version=kvManager::get("puzzleListVer",1);
					CommitManager::setSuccess("main",$puzzle->save());
				}else{
					kvManager::increase("hellModeListVer");
				}
			}
		}

		kvManager::increase("cardListVer");
		$p["data"]["version"]=kvManager::get("cardListVer",1);

		$r = parent::insertWithLQTable($p);
		CommitManager::setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
		}
	}

	public static function deleteWithLQTable($p){
		kvManager::increase("cardListVer");
		return parent::deleteWithLQTable($p);
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//	카드히스토리
////////////////////////////////////////////////////////////////////////////////////////

class CardHistory extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("CardHistory");
		self::setShardKey("memberID");
	}


	public function __construct($memberID=null,$cardNo=null){
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		self::setLQTableSelectCustomFunction(function ($rData){
	    	$cardInfo = new Card($rData["cardNo"]);
	    	$pieceInfo = new Piece($cardInfo->piece);
	    	$puzzleInfo = new Puzzle($pieceInfo->puzzle);

	    	$rData["puzzleNo"]=$puzzleInfo->no;
			return $rData;
		});

		$this->memberID = $memberID;

		if($cardNo)$this->cardNo=$cardNo;

		if($memberID && $cardNo){
			parent::load("memberID=".$memberID." and cardNo=".$cardNo);
		}
	}
	
	public static function updateWithLQTable($p){
		unset($p["data"]["puzzleNo"]);
		$r = parent::updateWithLQTable($p);
		return $r;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//	퍼즐히스토리
////////////////////////////////////////////////////////////////////////////////////////

class PuzzleHistory extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("PuzzleHistory");
		self::setShardKey("memberID");
	}
	public function __construct($memberID=null,$puzzleNo=null){
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		$this->memberID = $memberID;

		if($puzzleNo)$this->puzzleNo=$puzzleNo;

		if($memberID && $puzzleNo){
			parent::load("memberID=".$memberID." and puzzleNo=".$puzzleNo);
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//	스테이지히스토리
////////////////////////////////////////////////////////////////////////////////////////

class PieceHistory extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("PieceHistory");
		self::setShardKey("memberID");
	}

	public function __construct($memberID=null,$pieceNo=null){
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);
		

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});


		$this->memberID = $memberID;

		if($pieceNo)$this->pieceNo=$pieceNo;

		if($memberID && $pieceNo){
			parent::load("memberID=".$memberID." and pieceNo=".$pieceNo);

			if($this->clearDateList)$this->clearDateList = json_decode($this->clearDateList,true); 
		}
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//		불량닉네임
////////////////////////////////////////////////////////////////////////////////////////
class FaultyNick extends DBTable2{
	
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aFaultyNickTable");
	}

	public function __construct($no=null){
		
		parent::__construct();

		if($no)$this->m_no=$no;

		if($no){
			$query = "no=".$no;
			if(parent::load($query)){

			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	업적목록
////////////////////////////////////////////////////////////////////////////////////////
class Archivement extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aArchivementTable");
	}

	public function __construct($id=null){
		
		parent::__construct();

		if($id)$this->id=$id;

		if($id){
			$query = "`id`=".$id;
			if(parent::load($query)){
				//$this->jsonToObj("reward");
			}
		}
	}

	public static function updateWithLQTable($p){
		$r = parent::updateWithLQTable($p);

		kvManager::increase("arcListVer");
		
		return $r;
	}
	public static function insertWithLQTable($p){

		$r = parent::insertWithLQTable($p);

		kvManager::increase("arcListVer");

		return $r;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	업적히스토리
////////////////////////////////////////////////////////////////////////////////////////

class ArchivementHistory extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("ArchivementHistory");
		self::setShardKey("memberID");
	}

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		self::setLQTableSelectCustomFunction(function($rData){
			$archive = new Archivement($rData["archiveID"]);
			$exchange = new Exchange($archive->exchangeID);
			$rData["goal"]=$rData["archiveID"];
			if($rData["rewardDate"]>0){
				$rData["reward"]=$exchange->list;
			}
			return $rData;
		});

		$this->memberID = $memberID;
		if($fNo)$this->archiveID=$fNo;

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and archiveID='".$fNo."'");
		}
	}

}



////////////////////////////////////////////////////////////////////////////////////////
//	선물상자

////////////////////////////////////////////////////////////////////////////////////////
class GiftBoxHistory extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("GiftBoxHistory");
		self::setShardKey("memberID");
	}

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		self::setLQTableSelectCustomFunction(function ($rData){
			if($rData["reward"])$rData["reward"] = json_decode($rData["reward"],true);
			return $rData;
		});

		$this->memberID=$memberID;

		if($memberID && $fNo){
			if(parent::load("memberID=".$memberID." and no=".$fNo)){
				//$this->jsonToObj("reward");
				//$this->jsonToObj("exchangeList");
			}
		}
	}

	public static function getAllExchangeID($memberID){
		self::initial();

		$elist=array();
		while($rData = self::getRowByQueryWithShardKey("where memberID='".$memberID."' and confirmDate=0",$memberID)){
	LogManager::addLog("->".mysql_error());
			if($rData["exchangeID"])$elist[]=$rData["exchangeID"];
		}
	LogManager::addLog("->".mysql_error());

		return $elist;
	}

	public static function confirmAll($memberID){
		self::initial();

		//LogManager::addLog("comfirmAll start ".$memberID);
		$lastDay = TimeManager::getDateTime(TimeManager::getTime()-60*60*24*30);
	LogManager::addLog("->".mysql_error());
		$result = self::getQueryResultWithShardKey("update ".self::getDBTable()." set confirmDate='".TimeManager::getCurrentDateTime()."' where memberID='".$memberID."' and confirmDate=0 and regDate>$lastDay and exchangeID<>''",$memberID,false);
		
	LogManager::addLog("->".mysql_error()."confirmall is".json_encode($result));
		return $result;
	}

	public static function removeGiftBox(){
		$deldate = TimeManager::getModifyDateFromToday("-30 day");
		$deldate = $deldate."240000";
		$result = GiftBoxHistory::removeRowByQueryWithShardKey("where regDate<=".$deldate." memberID=".$user->memberID,$memberID);
		return $result;
	}

}


////////////////////////////////////////////////////////////////////////////////////////
//	유저보관함
////////////////////////////////////////////////////////////////////////////////////////

class UserStorage extends DBTable2{
	
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("UserStorage");
		self::setShardKey("memberID");
	}
	public function __construct($memberID=null){
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);

		self::setLQTableSelectQueryCustomFunction(function ($param){

			//LogManager::addLog("userstorage start custom func!".json_encode($param));
			//LogManager::addLog("userstorage start custom func!".$param["where"]["id"]."=".$param["where"]["type"]);
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);


			//LogManager::addLog("userstorage start custom func2");

			if(!$user->isLoaded()){
				
				//LogManager::addLog("userstorage start custom func! fail");
				return "where memberID='-1'";
			}

			////LogManager::addLog("userstorage start custom func ok".var_export($user,true));
			return "where memberID='".$user->memberID."'";
		});
		

		$this->memberID=$memberID;
		if($memberID){
			parent::load("memberID=".$memberID);
		}
	}

	public static function propCodeToStr($p){
		switch($p){
			case "m":return "현금";
			case "r":return "젬";
			case "pr":return "유료젬";
			case "fr":return "무료젬";
			case "g":return "골드";
			case "h":return "하트";
			case "i6":return "아이템두배아이템";
			case "i8":return "시간추가아이템";
			case "i9":return "신발아이템";
			case "i11":return "자석아이템";
			case "p1":return "이어하기권";
			case "p2":return "맵가챠권";
			case "p3":return "캐릭업글권";
			case "p4":return "아이템뽑기권";
			case "p5":return "99프로뽑기권";
			case "p6":return "생명의 돌";
			case "cd":return "카드";
			case "pc":return "피스";
			case "pz":return "퍼즐";
			case "i":return "아이템";
			case "p":return "이용권";
		}
		return "unkown";
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	유저보관함히스토리
////////////////////////////////////////////////////////////////////////////////////////



class UserPropertyHistory extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("log");
		self::setDBTable("UserPropertyHistory_".TimeManager::getCurrentDate());
		self::setShardKey("memberID");
	}

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["date"])self::setDBTable("UserPropertyHistory_".$param["where"]["date"]);

			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			$query = "where memberID='".$user->memberID."'";

			if($param["where"]["propType"]=="*" || !$param["where"]["propType"])return $query;

			if($param["where"]["propType"]=="i")$query.=" and `type` like 'i%'";
			if($param["where"]["propType"]=="p")$query.=" and `type` like 'p%' and `type`<>'pr'";
			else			
			$query.=" and `type`='".$param["where"]["propType"]."'";

			return $query;
		});

		$this->memberID=$memberID;

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and `no`=".$fNo);
		}
	}

	public function save($isIncludePrimaryKey=false){
		$save = parent::save($isIncludePrimaryKey);

		if(!$save){
			//LogManager::addLog("userLog save fail and createnew");


			$createStr = "
			CREATE TABLE `".self::getDBTable()."` (
			  `no` int(11) NOT NULL AUTO_INCREMENT,
			  `memberID` bigint(20) NOT NULL,
			  `type` varchar(45) NOT NULL,
			  `count` int(11) NOT NULL,
			  `total` int(11) NOT NULL,
			  `statsID` varchar(45) NOT NULL,
			  `statsValue` int(11) NOT NULL,
			  `content` varchar(255) NOT NULL,
			  `sender` varchar(45) NOT NULL,
			  `regDate` bigint(20) NOT NULL,
			  `exchangeID` varchar(45) NOT NULL,
			  PRIMARY KEY (`no`),
			  KEY `memberID` (`memberID`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
			";
			
			$state = self::getQueryResultWithShardKey($createStr,$this->memberID,false);
			
			if($state){
				$save = parent::save($isIncludePrimaryKey);
			}
		}

		return $save;
	}

}




////////////////////////////////////////////////////////////////////////////////////////
//	로그인이벤트
////////////////////////////////////////////////////////////////////////////////////////
class LoginEvent extends DBTable2{
	
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aLoginEventTable");
	}

	public function __construct($fNo=null){
		
		parent::__construct();
		

		self::setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($rData["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}


			
			return $rData;
		});


		if($fNo){
			parent::load("no=".$fNo);
		}
	}
	static public function getRewardDays(){
		$data=array();
		$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
		$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
		$result = LoginEvent::getQueryResult("select ".LoginEvent::getDBTable().".*,".Exchange::getDBTable().".list as reward from ".LoginEvent::getDBTable()." left join ".Exchange::getDBTable()." on ".LoginEvent::getDBTable().".exchangeID=".Exchange::getDBTable().".id  where ".LoginEvent::getDBTable().".endDate>".TimeManager::getCurrentDateTime()." and ".LoginEvent::getDBTable().".endTime=235959 and ".LoginEvent::getDBTable().".startTime=0  and ".LoginEvent::getDBTable().".os&".$osBit.">0 and ".LoginEvent::getDBTable().".cc&".$ccBit.">0 limit 3");
		LogManager::addLog("error?".mysql_error());
		// = LoginEvent::getRowByQuery("where endDate>".TimeManager::getCurrentDateTime()." and endTime=235959 and endDate-startDate=235959 limit 3")
		if(!$result)return $data;
		while($rData = mysql_fetch_assoc($result)){
			$data[]=$rData;
		}
		return $data;
	}

	
}

////////////////////////////////////////////////////////////////////////////////////////
//	출석이벤트
////////////////////////////////////////////////////////////////////////////////////////
class AttendenceEvent extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aAttendenceEventTable");
	}
	public function __construct($fNo=null){
		
		parent::__construct();
		
		self::setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($rData["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}
			
			return $rData;
		});

		if($fNo){
			$query = "no=".$fNo;
		}else{
			$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
			$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
			$today = TimeManager::getCurrentDateTime();
			$query = "startDate<=".$today." and endDate>=".$today." and os&".$osBit.">0 and cc&".$ccBit.">0 order by `order` asc limit 1";
		}
		if(parent::load($query)){
			//$this->jsonToObj("rewardList");
			//$this->jsonToObj("exchangeIDList");
		}
		
	}


}

////////////////////////////////////////////////////////////////////////////////////////////////////

class AttendenceEventDay extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aAttendenceEventDayTable");
	}
	public function __construct($fNo=null){
		
		parent::__construct();
		
		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"] && $param["where"]["id"] && $param["where"]["type"]=="eventNo"){
				return "where eventNo=".$param["where"]["id"];
			}

			return "where eventNo=-1";

			// return "`cuponCode`='".$param["data"]["cuponCode"]."'";
		});

		if($fNo){
			$query = "no=".$fNo;
		}

		if($query && parent::load($query)){
			//$this->jsonToObj("exchangeIDList");
		}
		
	}



}
////////////////////////////////////////////////////////////////////////////////////////
//	미션이벤트
////////////////////////////////////////////////////////////////////////////////////////
// class MissionEvent extends DBTable2{
// 	static public function construct(){
// 		self::setPrimarykey("no",true);
// 		self::setDBGroup("main");
// 		self::setDBTable("aMissionEventTable");
// 	}
// 	public function __construct($memberID=null,$fNo=null){
		
// 		parent::__construct();
		

// 		if($memberID && $fNo){
// 			parent::load("memberID=".$memberID." and no=".$fNo);
// 		}
// 	}

// }

////////////////////////////////////////////////////////////////////////////////////////
//	쿠폰메니저
////////////////////////////////////////////////////////////////////////////////////////
class CuponManager extends DBTable2{
	

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aCuponManageTable");
	}

	public function __construct($fNo=null,$cuponCode=null){
		
		parent::__construct();
		
		// self::setPrimarykey("no",true);

		// self::setDBInfo(DBManager::getMainDBInfo());

		self::setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($rData["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}
			$rData["cuponCount"] = CuponCode::getCuponCount($rData[no]);
			return $rData;
		});

		if($fNo || $cuponCode){
			$q = "no=".$fNo;
			if($cuponCode)$q = "cuponCode=`".$cuponCode."`";
			if(parent::load($q)){
				//$this->jsonToObj("reward");
			}
		}
	}

	public function checkCountry($mycc){
		$ccList = explode(",",$this->cc);
		
		if(array_search($mycc,$ccList)!==false)return true;

		return false;
	}


	public function checkOs($myos){
		$osList = explode(",",$this->os);
		
		if(array_search($myos,$osList)!==false)return true;

		return false;
	}

	

}

////////////////////////////////////////////////////////////////////////////////////////
//	쿠폰코드
////////////////////////////////////////////////////////////////////////////////////////
class CuponCode extends DBTable2{
	
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aCuponCodeTable");
	}

	public function __construct($fNo=null){
		
		parent::__construct();
		
		//LogManager::addLog("create cuponcode for".$fNo);
		
		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"] && $param["where"]["type"]=="cuponNo"){
				return "where cuponNo=".$param["where"]["id"];
			}else if ($param["where"]["id"] && $param["where"]["type"]=="cuponCode"){
				return "where cuponCode='".$param["where"]["id"]."'";
			}

			return "";

			// return "`cuponCode`='".$param["data"]["cuponCode"]."'";
		});

		self::setLQTableSelectCustomFunction(function($rData){
			$cuponInfo = new CuponManager($rData["cuponNo"]);
				
			if($cuponInfo->isLoaded()){
				if($cuponInfo->isCommon==1){
					$rData["isUsed"]="중복가능";
				}else{
					$cuponUsedInfo = new CuponUsedInfo($rData["cuponCode"]);
					

					if($cuponUsedInfo->isLoaded()){
						$rData["isUsed"]="사용";
						$rData["usedDate"]=$cuponUsedInfo->useDate;
						$rData["memberID"]=$cuponUsedInfo->memberID;
					}else{
						$rData["isUsed"]="미사용";
					}
				}
			}
			return $rData;
		});

		if($fNo){
			//LogManager::addLog("load cuponcode");
			parent::load("cuponCode='".$fNo."'");
		}
	}

	static public function getCuponCount($cuponNo){

		$alluser=0;
		$cnt=0;
		while($result = CuponCode::getQueryResult("select count(*) from ".self::getDBTable()." where cuponNo=".$cuponNo)){
			$cnt++;
			if($cnt>100){
				//LogManager::addLog("superBreak getAllUser");
				break;
			}
			$data = mysql_fetch_array($result);
			$alluser+=$data[0];
		}

		return $alluser;

	}

	public static function getCuponList($param){
		$data = array();

		while($rData = CuponCode::getRowByQuery("where cuponNo=".$param["cuponNo"])){
			$data[]=$rData;
		}

		$r["list"]=$data;
		$r["result"]=ResultState::successToArray();
		return $r;
	}


	public static function updateWithLQTable($p){
		//$r = parent::updateWithLQTable($p);
		$r["result"]=ResultState::toArray(ResultState::GDUNKNOWNRESULT,"업데이트할수없습니다.");
		return $r;
	}

	public static function insertWithLQTable($p){
		//$codeList = json_decode($p["data"]["cuponCode"],true);
		if(is_array($p["data"]["cuponCode"])){
			$codeList=$p["data"]["cuponCode"];
			$cnt = count($codeList);
			$mode="manual";
		}else{
			$cnt = $p["data"]["cuponCode"];
			$mode="auto";
		}
		//unset($p["data"]["code"]);
		//LogManager::addLog("test count is ".count($codeList));
		CommitManager::begin("main");
		for($i=0;$i<$cnt;$i++){

			//LogManager::addLog("test1 ".$i);
			
			$p2=unserialize(serialize($p));
			if($mode=="manual")	$p2["data"]["cuponCode"]=$codeList[$i];
			else $p2["data"]["cuponCode"] = CuponCode::getRandomString(12);

			$p2["data"]["serverNo"]=CuponUsedInfo::newShardKeyFunc($p2["data"]["cuponCode"]);
			
			//$this->setLoaded(false);
			$ri = parent::insertWithLQTable($p2);
			if(ResultState::successCheck($ri["result"])){
				$r["data"][]=$ri["data"];	
			}else{
				CommitManager::rollback("main");
				return ResultState::makeReturn(ResultState::GDDONTSAVE,"코드가중복되었습니다.(".$p2["data"]["cuponCode"].")");	
			}
					
		}

		if(CommitManager::commit("main")){
			$r["result"]=ResultState::successToArray();
		}else{
			$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"저장하지 못했습니다");
		}

		return $r;
	}

	static public function getRandomString($length = 10) {
	    $validCharacters = "abcdefghijklmnopqrstuxyvwz";
	    $validCharNumber = strlen($validCharacters);
	 
	    $result = "";
	 
	    for ($i = 0; $i < $length; $i++) {
	        $index = mt_rand(0, $validCharNumber - 1);
	        $result .= $validCharacters[$index];
	    }
	 
	    return $result;
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//	쿠폰히스토리
////////////////////////////////////////////////////////////////////////////////////////
class CuponHistory extends DBTable2{
	
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("CuponHistory");
		self::setShardKey("memberID");
	}

	public function __construct($code=null,$memberID=null){
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";

			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		self::setLQTableSelectCustomFunction(function($rData){
			$cuponInfo = new CuponManager($rData["cuponNo"]);
			$rData["cuponTitle"]=$cuponInfo->title;
			return $rData;
		});

		$this->memberID=$memberID;
		
		if($code && $memberID){
			parent::load("memberID='".$memberID."' and cuponCode='".$code."'");
		}
	}


}

////////////////////////////////////////////////////////////////////////////////////////
//	쿠폰사용정보
////////////////////////////////////////////////////////////////////////////////////////
class CuponUsedInfo extends DBTable2{



	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("CuponUsedInfo");
		self::setShardKey("cuponCode");

		//LogManager::addLog("CuponUsedInfo setNewShardKeyFunc call");
		//$func = CuponUsedInfo::createShardOrder;
		self::setNewShardKeyFunc(function($code){
			//LogManager::addLog("st new shard key func !!!!!!! for cuponUsedInfo");
			//서버대수만큼 나눠서 관리
			srand((double)microtime()*1000000);
			$cNumber = rand(1,self::getDBGroup()->getMasterCount());
			return $cNumber;
		});

		self::setGetShardKeyFunc(function($code){
			//서버대수만큼 나눠서 생성
			//LogManager::addLog("st get shard key func !!!!!!! for cuponUsedInfo with ".$code);
			$cManager = new CuponCode($code);
			return $cManager->serverNo;
		});
	}



	public function __construct($code=null){
		
		parent::__construct();
		$this->setDBShardKeyValue($code);


		$this->cuponCode=$code;
		if($code){
			parent::load("cuponCode='".$code."'");
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////


class WeeklyScore extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("log");
		self::setDBTable("WeeklyScore_".TimeManager::getCurrentWeekNo());
		self::setShardKey("memberID");


		self::setLQTableSelectQueryCustomFunction(function ($param){

			$where = "where 1=1";
			
			self::setDBTable("WeeklyScore_".$param["where"]["weekNo"]);


			if($param["where"]["id"]=="*")$where.="";
			else if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if($param["where"]["id"]!="*"){
				if(!$user->isLoaded())$where .=  " and memberID='-1'";
				else $where .=" and memberID='".$user->memberID."'";
			}
			return $where;
		});

		self::setLQTableSelectCustomFunction(function($data){
			//$data["data"] = json_decode($data["data"],true);
			//$data["nick"] = $data["data"]["nick"];
			return $data;
		});
	}

	public function load($q){
			//LogManager::addLog("test!!1".$this->regWeek);
		if($this->regWeek){
			//LogManager::addLog("test!!2".$this->regWeek);
			self::setDBTable("WeeklyScore_".$this->regWeek);
		}

		return parent::load($q);
	}

	public function __construct($memberID=null,$weekNo=null){
		parent::__construct();
		$this->setDBShardKeyValue($memberID);

		$this->memberID=$memberID;
		if($weekNo){
			self::setDBTable("WeeklyScore_".$weekNo);
		}
		if($memberID){
			self::load("memberID=".$memberID); //." and regWeek=".$weekNo
		}
	}

	public function save($isIncludePrimaryKey=false){
		self::setDBTable("WeeklyScore_".$this->regWeek);
		
		$save = parent::save($isIncludePrimaryKey);

		if(!$save){
			//LogManager::addLog("userLog save fail and createnew");


			$createStr = "
			CREATE TABLE `".self::getDBTable()."` (
			  `no` int(11) NOT NULL AUTO_INCREMENT,
			  `memberID` bigint(20) NOT NULL,
			  `score` int(11) NOT NULL,
			  `regDate` bigint(20) NOT NULL,
			  `regWeek` int(11) NOT NULL,
			  `nick` varchar(100) NOT NULL,
			  `flag` varchar(45) NOT NULL,
			  `data` text NOT NULL,
			  `count` int(11) NOT NULL DEFAULT '0',
			  `regTime` bigint(20) NOT NULL,
			  PRIMARY KEY (`no`),
			  KEY `memberID` (`memberID`,`regWeek`),
			  KEY `rank` (`regWeek`,`score`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
			";
			
			$state = self::getQueryResultWithShardKey($createStr,$this->memberID,false);
			
			if($state){
				$save = parent::save($isIncludePrimaryKey);
			}
		}

		return $save;
	}

	public function getTopRank($start=1,$count=50){
		$rdata = array(); 
		$start-=1;
		$query = "order by score desc limit $start,$count"; //"where regWeek=".$this->regWeek.
		$rl=0;
		while($data = WeeklyScore::getRowByQuery($query)){
			if($rl>50){
				//LogManager::addLog("superBreak getTopRank");
				break;
			}
			$rl++;
			$l=0;
			if(count($rdata)==0){
				$rdata[]=$data;
				continue;
			}

			if($rdata[$count-1]["score"]>$data["score"]){
				WeeklyScore::$m__qResult=null;
				WeeklyScore::$m__qCnt++;
			}
			for($i=0;$i<count($rdata);$i++){
				if($rdata[$i]["score"]<$data["score"])break;
				$l++;
			}

			array_splice($rdata, $l, 0, array($data));
			array_splice($rdata, $count, 1);
		}
		return $rdata;
	}

	public function getAllUser(){

		$alluser=0;
		$cnt=0;
		while($result = WeeklyScore::getQueryResult("select count(*) from ".self::getDBTable())){//." where regWeek=".$this->regWeek
			$cnt++;
			if($cnt>50){
				//LogManager::addLog("superBreak getAllUser");
				break;
			}
			$data = mysql_fetch_array($result);
			$alluser+=$data[0];
		}

		return $alluser;
	}

	public function getMyRank(){
		if($this->score<=0)return -1;
		
		$alluser=0;
		$cnt=0;
		while($result = WeeklyScore::getQueryResult("select count(*) from ".self::getDBTable()." where score>".$this->score)){		// regWeek=".$this->regWeek." and	
			$cnt++;
			if($cnt>50){
				//LogManager::addLog("superBreak getAllUser");
				break;
			}
			$data =mysql_fetch_array($result);
			$alluser+=$data[0];
		}
		
		return $alluser+1;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//	스테이지랭킹
////////////////////////////////////////////////////////////////////////////////////////
class StageScore extends DBTable2{
	

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("StageScoreTable");
		self::setShardKey("memberID");

		self::setLQTableSelectQueryCustomFunction(function ($param){

			$where = "where 1=1";

			if($param["where"]["stage"]!="*"){
				$where = "where stageNo=".$param["where"]["stage"];
			}


			if($param["where"]["id"]=="*")$where.="";
			else if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if($param["where"]["id"]!="*"){
				if(!$user->isLoaded())$where .=  " and memberID='-1'";
				else $where .=" and memberID='".$user->memberID."'";
			}
			return $where;
		});
	}


	public function __construct($memberID=null,$stageNo=null,$where=null){
		
		if(!$stageNo)return false;
		
		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);

		$this->stageNo=$stageNo;

		$this->memberID=$memberID;
		


		if($memberID || $where){
			if($where)$q_where = $where;
			else $q_where = "memberID=".$memberID." and stageNo=".$stageNo;
			

			//LogManager::addLog("stage score construct ".$q_where);
			parent::load($q_where);
		}
	}
	public function save($p=null){
		if(!$this->regDate){
			$this->regDate=TimeManager::getCurrentDateTime();
			$this->regTime=TimeManager::getTime();
		}
		return parent::save($p);
	}

	public function getTop4(){
		return $this->getTopRank(1,4);
	}

	public function getTopRank($start=1,$count=50){
		$rdata = array(); 
		$start-=1;
		$query = "where stageNo=".$this->stageNo." order by score desc limit $start,$count";
		$rl=0;
		while($data = StageScore::getRowByQuery($query)){
			$rl++;
			$l=0;
			if(count($rdata)==0){
				$rdata[]=$data;
				continue;
			}

			if($rdata[$count-1]["score"]>$data["score"]){
				StageScore::$m__qResult=null;
				StageScore::$m__qCnt++;
			}
			for($i=0;$i<count($rdata);$i++){
				if($rdata[$i]["score"]<$data["score"])break;
				$l++;
			}

			array_splice($rdata, $l, 0, array($data));
			array_splice($rdata, $count, 1);
		}
		return $rdata;
	}

	public function getAllUser(){

		$alluser=0;
		while($result = StageScore::getQueryResult("select count(*) from ".self::getDBTable()." where stageNo=".$this->stageNo)){
			$data = mysql_fetch_array($result);
			$alluser+=$data[0];
		}

		return $alluser;
	}

	public function getMyRank(){


		if($this->score<=0)return -1;
		
		$alluser=0;
		while($result = StageScore::getQueryResult("select count(*) from ".self::getDBTable()." where stageNo=".$this->stageNo." and score>".$this->score)){			
			$data =mysql_fetch_array($result);
			$alluser+=$data[0];
		}
		
		return $alluser+1;
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터
////////////////////////////////////////////////////////////////////////////////////////
class Character extends DBTable2{
	

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aCharacterTable");
	}

	public function __construct($characterNo=null){

		parent::__construct();
		

		if($characterNo){
			$q_where = "no=".$characterNo;
			if(parent::load($q_where)){

			}
		}
	}

	public function getPowerInfo($level){
		$mInfo =& $this->getRef("missileInfo");
		$nextLevInfo = $mInfo[$level];
		$isMax=false;

		if($level-1>=0)$currentLevInfo = $mInfo[$level-1];
		else $currentLevInfo=array(0,0);
		
		if($level-2>=0)$prevLevInfo = $mInfo[$level-2];
		else $prevLevInfo=array(0,0);

		if($level>=count($mInfo)){
			$isMax=true;
			$nextLevInfo=array(0,0);
		}

		$r["nextPrice"]=$nextLevInfo[1];
    	$r["power"]=$currentLevInfo[0];
    	$r["nextPower"]=$nextLevInfo[0];
    	$r["prevPower"]= $prevLevInfo[0];
    	$r["isMaxLevel"]=$isMax;
    	
    	return $r;
	}

	public static function updateWithLQTable($p){
		$r = parent::updateWithLQTable($p);
		kvManager::increase("charListVer");
		return $r;
	}

	public static function insertWithLQTable($p){
		$r = parent::insertWithLQTable($p);
		kvManager::increase("charListVer");
		return $r;
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터히스토리
////////////////////////////////////////////////////////////////////////////////////////
class CharacterHistory extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("CharacterHistory");
		self::setShardKey("memberID");
	}


	public $m__userIndex = null;
	public function __construct($memberID=null,$characterNo=null){

		parent::__construct();
		
		$this->setDBShardKeyValue($memberID);
		
		$this->memberID=$memberID;


		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		
		if($memberID && $characterNo){
			$q_where = "memberID=".$memberID." and characterNo=".$characterNo;
			if(parent::load($q_where)){
				
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//	공지사항
////////////////////////////////////////////////////////////////////////////////////////

class Notice extends DBTable2{
	
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aNoticeTable");
	}

	public function __construct($no=null){

		parent::__construct();
		

		self::setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($rData["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}
			
			return $rData;
		});
		if($no){
			$q_where = "no=".$no;
			if(parent::load($q_where)){
				$this->jsonToObj("banner");
				$this->jsonToObj("imgInfo");
			}
		}
	}

}


////////////// 실시간 공지 ///////////////////////


class RealtimeMsg extends DBTable2{
	
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aRealtimeMsgTable");
	}

	public function __construct($no=null){

		parent::__construct();
		
		if($no){
			$q_where = "no=".$no;
			if(parent::load($q_where)){
	
			}
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//	샵
////////////////////////////////////////////////////////////////////////////////////////

class Shop extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aShopTable");
	}

	public function __construct($pID=null,$exchangeID=null){

		parent::__construct();

		if($pID || $exchangeID){
			if($pID){
				$q_where = "pID='".$pID."'";
			}else if($exchangeID){
				$q_where = "exchangeID='".$exchangeID."'";
			}	
			
			if(parent::load($q_where)){
				$this->jsonToObj("data");
			}
			
		}
	}

	public static function updateWithLQTable($p){
		
		
		$r = parent::updateWithLQTable($p);


		kvManager::increase("shopListVer");

		return $r;
	}

	public static function insertWithLQTable($p){
		$exchange=null;
		
		$r = parent::insertWithLQTable($p);

		kvManager::increase("shopListVer");

		return $r;
	}

}


class ShopEvent extends DBTable2{


	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aShopEventTable");
	}

	public function __construct($pID=null,$exchangeID=null){

		parent::__construct();


		self::setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($rData["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}
			return $rData;
		});

		if($pID || $exchangeID){
			if($pID){
				$q_where = "pID='".$pID."'";
			}else if($exchangeID){
				$q_where = "exchangeID='".$exchangeID."'";
			}	
			
			if(parent::load($q_where)){
				$this->jsonToObj("data");
			}
			
		}
	}

	// public static function updateWithLQTable($p){
		
	// 	$r = parent::updateWithLQTable($p);
	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($r["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData=$p["oldData"];
	// 		$mh->newData=$p["data"];
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	kvManager::increase("shopListVer");
	// 	return $r;
	// }

	// public static function insertWithLQTable($p){


	// 	kvManager::increase("shopListVer");
	// 	$r = parent::insertWithLQTable($p);
	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($p["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData="추가";
	// 		$mh->newData=$p["data"];
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	return $r;
	// }

	// public static function deleteWithLQTable($p){
	// 	$r = parent::deleteWithLQTable($p);
	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($p["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData=$p["data"];
	// 		$mh->newData="삭제";
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	return $r;
	// }

}


class Item extends DBTable2{
	

	static public function construct(){
		self::setPrimarykey("no",no);
		self::setDBGroup("main");
		self::setDBTable("aItemTable");
	}

	public function __construct($characterNo=null){

		parent::__construct();

		if($characterNo){
			$q_where = "no=".$characterNo;
			if(parent::load($q_where)){

			}
		}
	}


}

////////////////////////////////////////////////////////////////////////////////////////
//	몬스터
////////////////////////////////////////////////////////////////////////////////////////

class MonsterMovement extends DBTable2{
	

	static public function construct(){
		self::setPrimarykey("type",false);
		self::setDBGroup("main");
		self::setDBTable("aMonsterMovementTable");
	}

	public function __construct($characterNo=null){

		parent::__construct();

		if($characterNo){
			$q_where = "`type`=".$characterNo;
			if(parent::load($q_where)){

			}
		}
	}


}


class Monster extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no");
		self::setDBGroup("main");
		self::setDBTable("aMonsterTable");
	}


	public function __construct($mobNo=null,$puzzleNo=null,$isBoss=null){
		
		parent::__construct();
		


		if($mobNo)$this->no=$mobNo;

		if($mobNo){
			$query = "no=".$mobNo;
			parent::load($query);
		}else if($puzzleNo){
			parent::load("puzzleNo=".$puzzleNo." and isBoss=".$isBoss);
		}
	}

	public static function updateWithLQTable($p){
		kvManager::increase("monsterListVer");
		$p["data"]["version"]=kvManager::get("monsterListVer",1);
		$r = parent::updateWithLQTable($p);
		return $r;
	}

	public static function insertWithLQTable($p){
		kvManager::increase("monsterListVer");
		$p["data"]["version"]=kvManager::get("monsterListVer",1);
		$r = parent::insertWithLQTable($p);
		kvManager::increase("monsterListVer");
		return $r;
	}

	public static function deleteWithLQTable($p){
		kvManager::increase("monsterListVer");
		return parent::deleteWithLQTable($p);
	}


}



////////////////////////////////////////////////////////////////////////////////////////
//	몬스터
////////////////////////////////////////////////////////////////////////////////////////
class Pattern extends DBTable2{


	static public function construct(){
		self::setPrimarykey("no");
		self::setDBGroup("main");
		self::setDBTable("aPatternTable");
	}

	public function __construct($patternNo=null,$type=null){
		
		parent::__construct();
		

		if($patternNo)$this->no=$patternNo;

		if($patternNo){
			$query = "type=".$patternNo;
			parent::load($query);
		}else if($type){
			parent::load("category='".$type."' and isUse=1 order by rand() limit 1");
		}
	}


}
////////////////////////////////////////////////////////////////////////////////////////
//	샵
////////////////////////////////////////////////////////////////////////////////////////


class Exchange extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aExchangeManager");
	}


	public function __construct($id=null){

		parent::__construct();
		
		if($id){
			if(substr($id,0,1)=="@"){
				self::setDBTable("aExchangeManagerForEvent");
			}
		}

		self::setLQTableInsertCustomFunction(function ($param){
			return "`id`=".$param["data"]["id"];
		});

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="id")return "where `id`='".$param["where"]["id"]."'";
			if($param["where"]["type"]=="content")return "where `content` like '%".$param["where"]["id"]."%'";
		});


		$this->id=$id;
		if($id){
			$q_where = "`id`='".$id."'";
			if(parent::load($q_where)){
				$this->jsonToObj("list");
			}
		}
	}

	public static function exchangeViewer($param){
		//LogManager::addLog("exchangeID is ".$param["id"]);
		$exchange = new Exchange($param["id"]);
		if(!$exchange->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND);
		//LogManager::addLog("exchangelist is ".json_encode($exchange->list));
		$r["exchangeID"]=$exchange->id;
		$r["list"]=$exchange->list;
		$r["result"]=ResultState::successToArray();
		return $r;
	}

	public static function saveExchangeID($param){
		
		$exchange = new Exchange($param["id"]);

		//if(!$exchange->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND,"보상아이디를 찾지 못했습니다.");

		$exchange->list = $param["list"];

		
		if($exchange->save()){
			$r["exchangeID"]=$exchange->id;
			$r["list"]=$exchange->list;
			$r["result"]=ResultState::successToArray();
		}else{
			$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"exchangeID생성실패");
		}

		return $r;
	}

	public static function mergeCustom($eList,$custom){
		if(is_array($custom) && $eList){
			for ($j=0; $j<count($custom);$j++) {
				for($i=0;$i<count($eList);$i++){
					if($eList[$i]["type"]==$custom[$j]["type"]){
						$eList[$i]=array_merge($eList[$i],$custom[$j]);
						break;
					}
					if($i==count($eList)-1){
						$eList[]=$custom[$j];
					}
				}
			}
		}

		return $eList;
	}


	public static function makeExchangeIDByRandom($param){
		$r["param"]=$param;
		if($param["id"]){
			//if(substr($param["id"],0,1)!="@")$param["id"]="@".$param["id"];
			//LogManager::addLog("exchangeID is ".$param["id"]);
			$exchange = new Exchange($param["id"]);
			//LogManager::addLog("exchangelist is ".json_encode($exchange->list));
			$r["exchangeID"]=$exchange->id;
			$r["list"]=$exchange->list;
			$r["result"]=ResultState::successToArray();
			return $r;
			
		}

		$exchange = new Exchange("@".CuponCode::getRandomString(10));
		$cnt = 0;
		while($exchange->isLoaded()){
			$newExID = "@".CuponCode::getRandomString(10);
			$exchange = new Exchange($newExID);
			$cnt++;
			if($cnt>100)return ResultState::makeReturn(GDUNKNOWNRESULT,"생성실패");
		}

		
		if($exchange->save()){
			$r["exchangeID"]=$exchange->id;
			$r["list"]=$exchange->list;
			$r["result"]=ResultState::successToArray();
		}else{
			$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"exchangeID생성실패");
		}
		return $r;
	}
	// public static function updateWithLQTable($p){
		
	// 	$r = parent::updateWithLQTable($p);

	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($r["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData=$p["oldData"];
	// 		$mh->newData=$p["data"];
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	return $r;
	// }

	// public static function deleteWithLQTable($p){
	// 	$r = parent::deleteWithLQTable($p);
	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($p["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData=$p["data"];
	// 		$mh->newData="삭제";
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	return $r;
	// }

	// public static function insertWithLQTable($p){
	// 	$r = parent::insertWithLQTable($p);

	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($p["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData="추가";
	// 		$mh->newData=$p["data"];
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	return $r;
	// }
}


class ExchangeForEvent extends Exchange{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aExchangeManagerForEvent");
	}
	public function __construct($id=null){

		parent::__construct();
		
		if($id){
			if(substr($id,0,1)!="@"){
				$id="@".$id;
			}
		}

		self::setLQTableInsertCustomFunction(function ($param){
			return "`id`=".$param["data"]["id"];
		});

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="id")return "where `id`='".$param["where"]["id"]."'";
			if($param["where"]["type"]=="content")return "where `content` like '%".$param["where"]["id"]."%'";
		});


		$this->id=$id;
		if($id){
			$q_where = "`id`='".$id."'";
			if(parent::load($q_where)){
				$this->jsonToObj("list");
			}
		}
	}




}

class FormSetter extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aFormSetter");
	}


	public function __construct($no=null){

		parent::__construct();

		$this->no = $no;
		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}

}


class EndlessRank extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("log");
		self::setDBTable("EndlessRank_".TimeManager::getCurrentWeekNo());
		self::setShardKey("memberID");

		self::setLQTableSelectQueryCustomFunction(function ($param){

			$where = "where 1=1";

			self::setDBTable("EndlessRank_".$param["where"]["weekNo"]);

			if($param["where"]["id"]=="*")$where.="";
			else if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if($param["where"]["id"]!="*"){
				if(!$user->isLoaded())$where .=  " and memberID='-1'";
				else $where .=" and memberID='".$user->memberID."'";
			}
			return $where;
		});
	}

	public function __construct($memberID=null,$weekNo=null){

		parent::__construct();

		$this->setDBShardKeyValue($memberID);
		$this->memberID=$memberID;
		if($weekNo){
			self::setDBTable("EndlessRank_".$weekNo);
		}
		if($memberID){
			$this->regWeek = $weekNo;
			$q_where = "`memberID`='".$memberID."'";
			//if($weekNo)$q_where.=" and regWeek=".$weekNo;
			if(parent::load($q_where)){
			}
		}
	}

	public function save($isIncludePrimaryKey=false){

		self::setDBTable("EndlessRank_".$this->regWeek);
		$save = parent::save($isIncludePrimaryKey);

		if(!$save){
			//LogManager::addLog("userLog save fail and createnew");


			$createStr = "
			CREATE TABLE `".self::getDBTable()."` (
			  `no` int(11) NOT NULL AUTO_INCREMENT,
			  `memberID` bigint(20) NOT NULL,
			  `regWeek` int(11) NOT NULL,
			  `score` int(11) NOT NULL,
			  `nick` varchar(100) NOT NULL,
			  `flag` varchar(45) NOT NULL,
			  `victory` int(11) NOT NULL,
			  `regDate` bigint(20) NOT NULL,
			  `data` text NOT NULL,
			  PRIMARY KEY (`no`),
			  KEY `memberID` (`memberID`),
			  KEY `rank` (`regWeek`,`victory`,`score`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
			";
			
			$state = self::getQueryResultWithShardKey($createStr,$this->memberID,false);
			
			if($state){
				$save = parent::save($isIncludePrimaryKey);
			}
		}

		return $save;
	}

	public function getTopRank($start=1,$count=50){
		$rdata = array(); 
		$start-=1;
		$query = "order by victory desc, score desc limit $start,$count"; //"where regWeek=".$this->regWeek." order by victory desc, score desc limit $start,$count";
		$rl=0;

		$orderField1 = "victory";
		$orderField2 = "score";
		while($data = EndlessRank::getRowByQuery($query)){
			$rl++;
			$l=0;
			if(count($rdata)==0){
				$rdata[]=$data;
				continue;
			}

			if($rdata[$count-1][$orderField1]>$data[$orderField1]){
				EndlessRank::$m__qResult=null;
				EndlessRank::$m__qCnt++;
			}

			for($i=0;$i<count($rdata);$i++){
				if($rdata[$i][$orderField1]<$data[$orderField1])break;
				if($orderField2 && $rdata[$i][$orderField1]==$data[$orderField1] && $rdata[$i][$orderField2]<$data[$orderField2])break;
				$l++;
			}

			array_splice($rdata, $l, 0, array($data));
			array_splice($rdata, $count, 1);
		}
		return $rdata;
	}

	public function getAllUser(){

		$alluser=0;
		while($result = EndlessRank::getQueryResult("select count(*) from ".$this->getDBTable())){ //." where regWeek=".$this->regWeek
			$data = mysql_fetch_array($result);
			$alluser+=$data[0];
		}

		return $alluser;
	}

	public function getMyRank(){
		if($this->score<=0)return -1;
		
		$alluser=0;
		while($result = EndlessRank::getQueryResult("select count(*) from ".$this->getDBTable()." where (victory>".$this->victory." or (victory=".$this->victory." and score>".$this->score."))")){ //regWeek=".$this->regWeek." and	
			$data =mysql_fetch_array($result);
			$alluser+=$data[0];
		}
		
		return $alluser+1;
	}

// 	public static function updateWithLQTable($p){
		
// 		$r = parent::updateWithLQTable($p);

// 		if(array_key_exists("reason",$p)){
// 			$mh = new ModifyHistory($r["data"]["memberID"]);
// 			unset($p["data"][self::getPrimarykey()]);
// 			$mh->oldData=$p["oldData"];
// 			$mh->newData=$p["data"];
// 			$mh->reason=$p["reason"];
// 			$mh->category=get_called_class();
// 			$mh->save();
// 		}
// 		return $r;
// 	}

// 	public static function deleteWithLQTable($p){
// 		$r = parent::deleteWithLQTable($p);
// 		if(array_key_exists("reason",$p)){
// 			$mh = new ModifyHistory($p["data"]["memberID"]);
// 			unset($p["data"][self::getPrimarykey()]);
// 			$mh->oldData=$p["data"];
// 			$mh->newData="삭제";
// 			$mh->reason=$p["reason"];
// 			$mh->category=get_called_class();
// 			$mh->save();
// 		}
// 		return $r;
// 	}

// 	public static function insertWithLQTable($p){
// 		$r = parent::insertWithLQTable($p);

// 		if(array_key_exists("reason",$p)){
// 			$mh = new ModifyHistory($p["data"]["memberID"]);
// 			unset($p["data"][self::getPrimarykey()]);
// 			$mh->oldData="추가";
// 			$mh->newData=$p["data"];
// 			$mh->reason=$p["reason"];
// 			$mh->category=get_called_class();
// 			$mh->save();
// 		}
// 		return $r;
// 	}
 }



class EndlessPlayList extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("log");
		self::setDBTable("EndlessPlayList_".TimeManager::getYesterDate());
		self::setShardKey("memberID");
	}
	
	public function __construct($memberID=null,$pNo=null){

		parent::__construct();
		$this->memberID=$memberID;
		$this->setDBShardKeyValue($memberID);
		if($pNo){
			if(parent::load("no=".$pNo)){
			}
		}
	}

	public function save($isIncludePrimaryKey=false){
		//LogManager::addLog("userLog save!!");
		self::setDBTable("EndlessPlayList_".TimeManager::getCurrentDate());
		$save = parent::save($isIncludePrimaryKey);
		if(!$save){
			//LogManager::addLog("userLog save fail and createnew");

			$createStr = "
			CREATE TABLE `".self::getDBTable()."` (
			  `no` int(11) NOT NULL AUTO_INCREMENT,
			  `memberID` bigint(20) NOT NULL,
			  `nick` varchar(100) NOT NULL,
			  `flag` varchar(45) NOT NULL,
			  `victory` int(11) NOT NULL,
			  `autoLevel` int(11) NOT NULL,
			  `level` int(11) NOT NULL,
			  `score` int(11) NOT NULL,
			  `playData` text NOT NULL,
			  `regDate` bigint(20) NOT NULL,
			  `pieceNo` int(11) NOT NULL,
			  PRIMARY KEY (`no`),
			  KEY `memberID` (`memberID`),
			  KEY `autoLevel` (`memberID`,`victory`)
			) ENGINE=InnoDB DEFAULT CHARSET=utf8;
			";
			
			$state = self::getQueryResultWithShardKey($createStr,$this->memberID,false);
			
			if($state){
				$save = parent::save($isIncludePrimaryKey);
			}
		}

		self::setDBTable("EndlessPlayList_".TimeManager::getYesterDate());
		return $save;
	}



	public function getPlayDataByRandom($memberID,$lvl=1,$limit=1,$fieldlist="*"){
		$result = array();
		$query = EndlessPlayList::getQueryResult("select ".$fieldlist." from `".EndlessPlayList::getDBTable()."` where memberID in (SELECT DISTINCT memberID FROM `".EndlessPlayList::getDBTable()."`) and victory<=".($lvl+1)." and victory>=".($lvl-1)." and memberID <> ".$memberID." ORDER BY RAND() limit ".$limit);

		
		//LogManager::addLog("select ".$fieldlist." from `".EndlessPlayList::getDBTable()."` where memberID in (SELECT DISTINCT memberID FROM `".EndlessPlayList::getDBTable()."`) and victory<=".($lvl+1)." and victory>=".($lvl-1)." and memberID <> ".$memberID." ORDER BY RAND() limit ".$limit);
		


		if($query){
		$check=false;
			while($rData = mysql_fetch_assoc($query)){
				if($check)unset($rData["playData"]);
				$result[]=$rData;
				$check=true;
			}
		}
		//LogManager::addLog("test size is".count($result));
		
		self::setDBTable("EndlessPlayList");
		$i=0;
		while(count($result)<$limit){
			$lvl--;

			$query = EndlessPlayList::getQueryResult("select ".$fieldlist." from `".EndlessPlayList::getDBTable()."` where memberID in (SELECT DISTINCT memberID FROM `".EndlessPlayList::getDBTable()."`) and memberID <> ".$memberID." ORDER BY RAND() limit ".$limit);
			if($query){
				while($rData = mysql_fetch_assoc($query)){
					if($check)unset($rData["playData"]);
					$result[]=$rData;
					$check=true;
				}
			}
			$i++;
			if($i>50){
				LogManager::addLog("fuck dont find user play");
				break;
			}
		}

		self::setDBTable("EndlessPlayList_".TimeManager::getYesterDate());
		//LogManager::addLog(mysql_error());
		
		return $result;
	}

	public function getPlayDataByNo($no){

		return $result;
	}


}

///////////////////////////////
///////////////////////////////


class CommonSetting extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aCommonSetting");
	}


	public function __construct($key=null){

		parent::__construct();


		if($key){
			$this->key = $key;
			$q_where = "`key`='".$key."'";
			if(parent::load($q_where)){
				if($this->isLocal==1){
					$this->value=CurrentUserInfo::getLocalizedValueInData(json_decode($this->value,true));
				}else{
					$this->jsonToObj("value");
				}
			}
		}
	}

	// public static function updateWithLQTable($p){
		
	// 	$r = parent::updateWithLQTable($p);

	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($r["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData=$p["oldData"];
	// 		$mh->newData=$p["data"];
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	return $r;
	// }

	// public static function deleteWithLQTable($p){
	// 	$r = parent::deleteWithLQTable($p);
	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($p["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData=$p["data"];
	// 		$mh->newData="삭제";
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	return $r;
	// }

	// public static function insertWithLQTable($p){
	// 	$r = parent::insertWithLQTable($p);

	// 	if(array_key_exists("reason",$p)){
	// 		$mh = new ModifyHistory($p["data"]["memberID"]);
	// 		unset($p["data"][self::getPrimarykey()]);
	// 		$mh->oldData="추가";
	// 		$mh->newData=$p["data"];
	// 		$mh->reason=$p["reason"];
	// 		$mh->category=get_called_class();
	// 		$mh->save();
	// 	}
	// 	return $r;
	// }
}

////////////////////////////////////
////////////////////////////////////


class KeyIntValue extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aKeyIntValue");
	}


	public function __construct($no=null,$key=null){

		parent::__construct();


		$this->no = $no;
		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}


}

/////////////////////////////////////
/////////////////////////////////////


class TimeEvent extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aTimeEventTable");
	}


	public function __construct($no=null){

		parent::__construct();
		
		self::setLQTableSelectCustomFunction(function($rData){

			//LogManager::addLog("in custom function");

			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($rData["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}


			
			return $rData;
		});


		$this->no = $no;
		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}

}

/////////////////////////////////////


class MissionEvent extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aMissionEventTable");
	}


	public function __construct($no=null,$type=null,$value=null,$value2=null){

		parent::__construct();
		
		self::setLQTableSelectCustomFunction(function($rData){

			//LogManager::addLog("in custom function");

			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($rData["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}


			
			return $rData;
		});


		$this->no = $no;
		if($no){
			$q_where = "`no`='".$no."'";
			parent::load($q_where);
		}else if($type || $value){
			$osBit = CurrentUserInfo::getOsBit(CurrentUserInfo::$os);
			$ccBit = CurrentUserInfo::getCountryBit(CurrentUserInfo::$country);
			$today = TimeManager::getCurrentDateTime();
			$sign = "=";
			if($type=="pvpScore")$sign="<";
			if($type=="pieceScore")$sign="<";
			$value2Sql = "";
			if($value2)$value2Sql = " and value2 = ".$value2;
			$q_where = "startDate<=".$today." and endDate>=".$today." and os&".$osBit.">0 and cc&".$ccBit.">0 and type='".$type."' and value ".$sign.$value.$value2Sql." order by `order` asc limit 1";
			LogManager::addLog("missionEvent query -> ".$q_where);
			parent::load($q_where);
		}
	}

}



/////////////////////////////////////
/////////////////////////////////////


class ModifyHistory extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("ModifyHistory");
		self::setShardKey("memberID");
	}

	public function __construct($memberID=null,$no=null){

		parent::__construct();

		$this->setDBShardKeyValue($memberID);

		self::setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";

			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		$this->memberID=$memberID;
		
		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}

	public function save($isIncludePrimaryKey = false){
		// $admin = new AdminUser($_SESSION["admin_no"]);
		// if(!$admin->isLogined()){
		// 	return ResultState::makeReturn(ResultState::GDSECURITY);
		// }

		// $this->writer = $admin->id;

		if(!$this->regDate)$this->regDate=TimeManager::getCurrentDateTime();
		return parent::save($isIncludePrimaryKey);
	}


}



/////////////////////////////////////
/////////////////////////////////////


class AdminUser extends DBTable2{
	public $m__isLogin = false;

	static public $m__permissinoGroup = array();

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aAdminUser");

		//유저관리
		self::$m__permissinoGroup["user"]=array("유저관리","UserData","ModifyHistory","PuzzleHistory","PieceHistory","CharacterHistory","CardHistory","ArchivementHistory","GiftBoxHistory","UserStorage","UserPropertyHistory","UserLog","UserIndex","CuponHistory");
		
		//통계
		self::$m__permissinoGroup["stats"]=array("통계","UserData","ModifyHistory","PuzzleHistory","PieceHistory","CharacterHistory","CardHistory","ArchivementHistory","GiftBoxHistory","UserStorage","UserPropertyHistory","UserLog","UserIndex","CuponHistory");

		//랭킹
		self::$m__permissinoGroup["rank"]=array("랭킹","StageScore","EndlessRank","WeeklyScore");
		//이벤트관리
		self::$m__permissinoGroup["event"]=array("이벤트","Notice","LoginEvent","PuzzleEvent","ShopEvent","TimeEvent","AttendenceEvent","AttendenceEventDay","CuponManager","CuponCode","CuponUsedInfo");
		//게임관리
		self::$m__permissinoGroup["game"]=array("게임관리","CommonSetting","Exchange","Archivement","Card","Puzzle","Piece","Monster","Pattern","Mission","Shop","EndlessPlayList");
		//운영자관리
		self::$m__permissinoGroup["admin"]=array("운영자관리","AdminUser");
	}


	public function __construct($no=null,$id=null,$pw=null){

		parent::__construct();
		
		self::setLQTableSelectCustomFunction(function($data){
			$data["passwd"]="********";
			return $data;
		});

		$this->m__isLogin = false;
		$q_where="";
		if($id){
			$q_where = "`id`='".$id."'";
			if($pw){
				$q_where .= " and `passwd`=password('".$pw."')";
				$this->m__isLogin = true;
			}
		}else if($no){
			$q_where = "`no`='".$no."'";
			$this->m__isLogin = true;
		}

		//LogManager::addLog("login query is ".$q_where);
		if($q_where){
			if(parent::load($q_where)){			
				$this->jsonToObj("permission");
			}else{
				$this->m__isLogin = false;
			}
		}
	}


	public function isLogined(){
		return $this->m__isLogin;
	}

	public function logout(){
		unset($_SESSION['admin_id']);
		unset($_SESSION['admin_no']);
		unset($_SESSION['game_id']);
	}
	
	public function checkPermission($where,$permission){

		$p =& $this->getRef("permission");

		if($permission==$p[$where]){
			return true;	
		}

		if($permission=="read"){
			if($p[$where]=="read" || $p[$where]=="write") return true;	
		}

		return false;
	}

	public function checkPermissionGroup($cls,$permission){

		$where="";

		foreach (self::$m__permissinoGroup as $key => $value) {
			if(in_array($cls,self::$m__permissinoGroup[$key])){$where=$key; break;}
		}

		if(!$where)return true;
		//LogManager::addLog("check per group is ".$where);
		return $this->checkPermission($where,$permission);
	}

	public static function login($p){
		$r["param"]=$p;
		$admin = new AdminUser(null,$p["data"]["id"],$p["data"]["passwd"]);
		$r["isLogined"]=false;
		if($admin->isLogined()){
			$_SESSION['admin_id']=$admin->id;
			$_SESSION['admin_no']= $admin->no;
			$r["isLogined"]=true;
			$r["result"]=ResultState::successToArray();
		}else{
			$r["isLogined"]=false;
			$r["result"]=ResultState::toArray(ResultState::GDDONTFINDUSER,"로그인실패");
		}

		
		return $r;
	}

	public static function insertWithLQTable($p){
		if($p["data"]["passwd"]){
			$q =  mysql_query("select password('".$p["data"]["passwd"]."')",self::getDBGroup()->getConnectionForReadByRand());
			
			$pass = mysql_fetch_array($q);
			$p["data"]["passwd"]=$pass[0];
		}
		$r = parent::insertWithLQTable($p);
		$r["data"]["passwd"]="********";
		return $r;
	}

	public static function updateWithLQTable($p){
		if($p["data"]["passwd"]=="********")unset($p["data"]["passwd"]);
		else{
			$q =  mysql_query("select password('".$p["data"]["passwd"]."')",self::getDBGroup()->getConnectionForReadByRand());
			$pass = mysql_fetch_array($q);
			$p["data"]["passwd"]=$pass[0];
		}
		$r = parent::updateWithLQTable($p);
		return $r;
	}

}


class Mission extends DBTable2{
	

	static public function construct(){
		self::setPrimarykey("type",false);
		self::setDBGroup("main");
		self::setDBTable("aMissionTable");
	}

	public function __construct($no=null){

		parent::__construct();

		if($no){
			parent::load("`type`=".$no);
		}else{
			parent::load("isUse=1 order by rand() limit 1");
		}
	}



}

?>