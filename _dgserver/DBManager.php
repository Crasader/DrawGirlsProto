<?php 
include_once("DBManagerLib.php");
include_once("DBManagerLib2.php");

class CommitManager{
	

	static public $m_dbMaster=null;
	static public $m_userIndex=null;
	static public $m_isSuccess=null;
	static public $m_releaseCount=null;
	//private static $m_instance=NULL;

	static public function construct($memberID=null){
		self::$m_userIndex=array();
		self::$m_dbMaster=array();
		self::$m_isSuccess=array();
		self::$m_releaseCount=array();
	}

	// //싱글턴 얻어오기
	// public static function get()
	// {
	//     if ( is_null( self::$m_instance ) )
	//     {
	//       self::$m_instance = new self();88899626759640837 88899626759640700
	//     }
	//     return self::$m_instance;
	// }

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
			mysql_query("SET AUTOCOMMIT=0",self::$m_dbMaster[$memberID]->getConnection());
			mysql_query("BEGIN",self::$m_dbMaster[$memberID]->getConnection());

			LogManager::addLog("start transaction".mysql_error());
		}else{
			self::$m_releaseCount[$memberID]++;
			LogManager::addLog("start transaction but ++ =>".self::$m_releaseCount[$memberID]);
			
		}
	}

	static public function commit($memberID){
		if(!self::$m_releaseCount[$memberID]) return false;


		LogManager::addLog("commit transaction count=".self::$m_releaseCount[$memberID]."-1");

		self::$m_releaseCount[$memberID]--;

		if(!self::isSuccess($memberID)){
			if(self::$m_releaseCount[$memberID]==0){
				$result = mysql_query("ROLLBACK", self::$m_dbMaster[$memberID]->getConnection());
				LogManager::addLog("commit query but rollback : ".mysql_error());
				mysql_query("SET AUTOCOMMIT=1",self::$m_dbMaster[$memberID]->getConnection());
			}
			return false;
		}

		if(self::$m_releaseCount[$memberID]==0){
			$result = mysql_query("COMMIT", self::$m_dbMaster[$memberID]->getConnection());
			LogManager::addLog("commit query ok? : ".mysql_error());
			mysql_query("SET AUTOCOMMIT=1",self::$m_dbMaster[$memberID]->getConnection());
		}else{
			$result = true;
		}

		
		return $result;
	}

	static public function rollback($memberID){
		if(!self::$m_releaseCount[$memberID]) return false;


		LogManager::addLog("rollback transaction count=".self::$m_releaseCount[$memberID]."-1");
		//LogManager::addLog("why? ->".var_export(debug_backtrace(),true));

		self::$m_releaseCount[$memberID]--;

		if(self::$m_releaseCount[$memberID]==0){
			$result = mysql_query("ROLLBACK", self::$m_dbMaster[$memberID]->getConnection());
		}else{
			self::setSuccess($memberID,false);
			$result=true;
		}
		LogManager::addLog("rollback transaction");
		return $result;
	}

	static public function setSuccess($memberID,$success){
		if(!$success){
			LogManager::addLog("oh my god commit fail 1 ".mysql_error());
			//LogManager::addLog("why? ->".var_export(debug_backtrace(),true));
		}
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
	}

	public static $sharedIndexes=array();

	public static function create($memberID=null,$userindex=null,$socialID=null,$nick=null){
		self::initial();

		LogManager::addLog("create userIndex");
		if($memberID && self::$sharedIndexes[$memberID]){
			LogManager::addLog("finded userIndex in sharedIndexes ".$memberID);
			return self::$sharedIndexes[$memberID];
		}


		LogManager::addLog("new userIndex, memberID is ".$memberID." and userIndex is ".$userindex);
		$newIndex =new UserIndex($memberID,$userindex,$socialID,$nick);

		if($newIndex->isLoaded()){
			LogManager::addLog("useindex load success no is".$newIndex->no);
			self::$sharedIndexes[$newIndex->memberID]=$newIndex;
		}else{
			LogManager::addLog("userindex load fail it's new obj is ".json_encode($newIndex->getArrayData(true))." and shardIndex is ".$newIndex->userShardOrder);
			
			// if(!$memberID){
			// 	return null;
			// }
			//$test = get_class_vars(get_class($newIndex));
			//LogManager::addLog("userindex load fuck!!".json_encode($test));
		}	
		return $newIndex;
	}

	public function __construct($memberID=null,$userindex=null,$socialID=null,$nick=null){

		parent::__construct();
		
		LogManager::addLog("construct userIndex for ".$memberID);

		if($userindex){
			parent::load("no=".$userindex);

		 	// if(parent::load("no=".$userindex))
		 	// {
		 	// 	$this->autoMatching($this->m__result);
		 	// }
		}else if($memberID){
			//parent::load("memberID=".$memberID);

		 	if(parent::load("memberID='".$memberID."'")){
		 		//$this->autoMatching($this->m__result);
		 		LogManager::addLog("load success userindex shardIndex is".$this->userShardOrder);
		 	}else{
				$this->memberID = $memberID;
		 		$this->userShardOrder = UserData::newShardkeyFunc($memberID);
		 		$this->logShardOrder = UserLog::newShardkeyFunc($memberID);
		 		LogManager::addLog("load fail userindex shardIndex is ".$this->userShardOrder." m_shardDBCount is ".count(self::getDBGroup()->getMasterList()));
		 		//$this->save(true);d
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
		LogManager::addLog("remove userindex!!");
		return parent::remove();
	}


	public function save($isIncludePrimaryKey = false){
		LogManager::addLog("save userindex!!");
		return parent::save($isIncludePrimaryKey);
	}

	public function getDBMaster(){
		return self::getDBGroup()->getMaster(1);
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
		
		//$this->setDBTable(DBManager::getST("userlog"));
		$this->memberID = $memberID;
		$this->setDBShardKeyValue($memberID);
		// self::setLQTableSelectQueryCustomFunction(function ($param){
		
		// 	if(!$param["where"] || !$param["where"]["category"] || $param["where"]["category"]=="")return "";
		// 	return "where category='".$param["where"]["category"]."'";
		// });
		
		self::setLQTableSelectQueryCustomFunction(function ($param){
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
			// if(parent::load("no=".$no." and memberID=".$memberID)){
			// 	$this->autoMatching($this->m__result);
			// }
		}
	}

	public function save($isIncludePrimaryKey=false){
		LogManager::addLog("userLog save!!");
		$save = parent::save($isIncludePrimaryKey);
		if(!$save){
			LogManager::addLog("userLog save fail and createnew");

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
			
			LogManager::addLog("createquery result : ".$state.mysql_error());
			
			if($state){
				$save = parent::save($isIncludePrimaryKey);
			}
		}

		return $save;
	}
	// public function selectWithLQTable($param){
	// 	$dataList = array();
	// 	$where="";

	// 	if($param["category"]){
	// 		$where = "where category='".$param["category"]."'";
	// 	}

	// 	$limit = $param["limit"];
	// 	if(!$limit)$limit=10;

	// 	$rl=0;

	// 	if($param["nextInfo"] && $param["nextInfo"]["nextTime"]){
	// 		if(!$where)$where="where";
	// 		else $where.=" and ";
	// 		$where=$where." regTime<".$param["nextInfo"]["nextTime"];
	// 	}	


	// 	$query = $where." order by no desc limit ".$limit;
	//     while($data = self ::getRowByQuery($query)){
	// 		$rl++;
	// 		$l=0;

	// 		if($dataList[$limit-1]["regTime"]>$data["regTime"]){
	// 			static::$m__qResult=null;
	// 			static::$m__qCnt++;
	// 		}
	// 		for($i=0;$i<count($dataList);$i++){
	// 			if($dataList[$i]["regTime"]<$data["regTime"])break;
	// 			$l++;
	// 		}
	// 		array_splice($dataList, $l, 0, array($data));
	// 		array_splice($dataList, $limit, 1);
	//     }
	//     $result["nextInfo"]=array("nextTime"=>$dataList[$limit-1]["regTime"]);
	//     $result["param"]=$param;
	//     $result["query"]=$query;
	//     $result["data"]=$dataList;
	//     $result["result"]=ResultState::successToArray();
	//     return $result;
	// }
	public static function loadWithLQTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"카테고리","field"=>"category","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"IP","field"=>"ip","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"입력값","field"=>"input","viewer"=>json_decode('{"type":"textareaViewer"}',true));
		$data["head"][]=array("title"=>"출력값","field"=>"output","viewer"=>json_decode('{"type":"textareaViewer"}',true));
		$data["head"][]=array("title"=>"변경일시","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"타임스탬프","field"=>"regTime","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"실행시간","field"=>"execTime","viewer"=>json_decode('{"type":"text"}',true));
		//$data["head"][]=array("manage"=>"update delete insert");
	return $data;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SendItem extends DBTable2{
	// static public $m__queryResult = null;
	// static public $m__queryCnt = 0;

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("UserLogTable");
		self::setShardKey("memberID");
	}


	public function __construct($memberID=null,$socialID=null,$nick=null){
		parent::__construct();
		

		//가입시간
		//if(!$this->joinDate)$this->joinDate=TimeManager::getCurrentDateTime();
	}


	public static function selectWithLQForm($param){
		$command = new commandClass();
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

	public static function updateWithLQForm($param){
		//여러멤버아이디
		//멤버국가표시
		//변경내용 바로 지급
		//여러선물 동시에 보내기
		//

		$data = $param["data"];

		LogManager::addLog(json_encode($data));

		// $propCount = $data["propData"]["count"];
		// if($data["propData"]["count"]=="m"){
		// 	$propCount*=-1;
		// }

		// //exchange 생성
		// $newExID = CuponCode::getRandomString(10);
		// $exchange = new Exchange($newExID);
		// if($data["propType"]!="msg"){
		// 	while($exchange->isLoaded()){
		// 		$newExID = CuponCode::getRandomString(10);
		// 		$exchange = new Exchange($newExID);
		// 	}
		// 	$exchange->list=json_decode('[{"type":"'.$data["propType"].'","count":'.$propCount.',"statsID":"sendGift","content":"'.$data["sendType"]["message"].'"}]');		
		// 	if(!$exchange->save()){
		// 		return ResultState::makeReturn(ResultState::GDDONTSAVE,"exchange생성 오류");
		// 	}
		// }else{
		// 	$exchange->id="";
		// 	$exchange->list="";
		// }

		$exchange = new Exchange($data["exchangeID"]);

		if(!$exchange->isLoaded())return ResultState::makeReturn(ResultState::GDDONTSAVE,"교환정보 로드실패");

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
				$param["exchangeID"]=$exchange->id;
				$param["reward"]=$exchange->list;
				$cmd = new commandClass();
				$sR = $cmd->sendgiftboxhistory($param);
				CommitManager::setSuccess($mID,ResultState::successCheck($sR["result"]));
			
			//바로지급
			}else{

				$param["memberID"]=$mID;
				$param["exchangeID"]=$exchange->id;
				$cmd = new commandClass();
				$sR = $cmd->exchange($param);
				CommitManager::setSuccess($mID,ResultState::successCheck($sR["result"]));
			}

			if($param["comment"]){
				$mh = new ModifyHistory($mID);
				unset($p["data"][self::getPrimarykey()]);
				$mh->oldData="";
				$mh->newData=$exchange->list;
				$mh->comment=$param["comment"];
				$mh->category=get_called_class();
				CommitManager::setSuccess($mID,$mh->save());
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

	//public $m__userIndex=null;
	public static $sharedUserDatas=array();



	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("UserDataTable");
		self::setShardKey("memberID");
	}


	public static function create($memberID=null,$socialID=null,$nick=null){
		
		LogManager::addLog("create userData");
		if($memberID && self::$sharedUserDatas[$memberID]){
			LogManager::addLog("finded userData in sharedUserDatas ".$memberID);
			return self::$sharedUserDatas[$memberID];
		}


		LogManager::addLog("UserData::create, memberID is ".$memberID." and userIndex is ");
		$newUserData =new UserData($memberID,$socialID,$nick);

		if($newUserData->isLoaded()){
			LogManager::addLog("userData load success no is".$newUserData->no);
			self::$sharedUserDatas[$newUserData->memberID]=$newUserData;
		}else{
			LogManager::addLog("userData load fail it's new obj is ".json_encode($newUserData->getArrayData(true)));
			
			// if(!$memberID){
			// 	return null;
			// }
			//$test = get_class_vars(get_class($newIndex));
			//LogManager::addLog("userindex load fuck!!".json_encode($test));
		}	
		return $newUserData;
	}



	public function __construct($memberID=null,$socialID=null,$nick=null){
		parent::__construct();
		
		//self::setPrimarykey("no");
		//$this->setDBTable(DBManager::getST("userdata"));
		$this->memberID = $memberID;
		$this->setDBShardKeyValue($memberID);
		self::setLQTableSelectCustomFunction(function ($data){
			$data["memberID"] = strval($data["memberID"]);
			return $data;
		});

		if($memberID || $socialID || $nick){
			$userIndex= UserIndex::create($memberID,null,$socialID,$nick);
			//LogManager::addLog("create userindex for ".$this->m__userIndex->memberID." result is ".json_encode($this->m__userIndex->getArrayData(true)));
			
			if($userIndex && $userIndex->isLoaded()){
				$this->memberID=$userIndex->memberID;
				if(parent::load("memberID='".$userIndex->memberID."'")){
					//$this->autoMatching($this->m__result);
					$this->jsonToObj("archiveData");
					$this->jsonToObj("eventCheckData");
					$this->jsonToObj("TMInfo");
					$this->jsonToObj("TMLevel");
					$this->jsonToObj("endlessData");
				}
			}
		}
		

		//가입시간
		//if(!$this->joinDate)$this->joinDate=TimeManager::getCurrentDateTime();
	}


	public static function loadWithLQTable($p){
		$r=parent::loadWithLQTable($p);
		$r["head"][]=array("manage"=>"update delete insert");
		return $r;
	}

	public function save($isIncludePrimaryKey=false){
		//마지막접속시간
		LogManager::addLog("userdata save!!!!!!");
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

		// $arraydata=array();
		// $class_vars = get_class_vars(get_class($this));
		// foreach($class_vars as $name=>$value){
		// 	if(!strpos($name,"__")){
		// 		$fieldname = str_replace("m_","",$name);
		// 		if(!$keyList|| in_array($fieldname,$keyList)){
		// 			$arraydata[$fieldname]=$this->$name;
		// 		}
		// 	}
		// }
		
		// if(static::$m__primarykey && !$isIncludePrimaryKey){
		// 	unset($arraydata[static::$m__primarykey]);
		// }
		
		// return $arraydata;
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
		LogManager::addLog("addfriend ".$friendID);

		$friendList=array();
		if($this->friendList)$friendList = json_decode($this->friendList,true);
		
		$friendList[]=$friendID;
		
		$friendList = array_unique($friendList);		
		$this->friendList = json_encode($friendList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		LogManager::addLog("addfriendResult is ".$this->friendList);
	}

	public function setArchiveData($key,$value){
		if(!is_array($this->archiveData))$this->archiveData = array();

		$this->archiveData = array_merge($this->archiveData,array($key=>$value));

	}


	public static function selectWithLQForm($param){
		$command = new commandClass();
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
		
		$hgr = $command->httpgateway($p);
		
		$r=$user->getArrayData(true);
		$r = array_merge($hgr,$r);
		
		$tm =& $user->getRef("TMInfo");
		$r["tm_type"]=$tm["type"];
		$r["tm_goal"]=$tm["goal"];
		$r["tm_count"]=$tm["count"];
		$r["tm_date"]=$tm["date"];

		$character = new CharacterHistory($user->memberID,$user->selectedCharNO);
		LogManager::addLog("character".$user->memberID."-".$user->selectedCharNO);
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
			$param["eventAtdCount"]=$param["data"]["eventAtdCount"];
			self::updateWithLQTable($param);

		}

		if(array_key_exists("eventCheckDate",$param["data"])){
			$param["eventCheckDate"]=$param["data"]["eventCheckDate"];
			self::updateWithLQTable($param);

		}

		if(array_key_exists("eventCheckWeek",$param["data"])){
			$param["eventCheckWeek"]=$param["data"]["eventCheckWeek"];
			self::updateWithLQTable($param);

		}

		if(array_key_exists("tm_date",$param["data"])){
			$user = UserData::create($param["primaryValue"]);
			$tm =& $user->getRef("TMInfo");
			$tm["date"]=$param["data"]["tm_date"];
			$user->save();
		}

		$param["where"]["type"]="sno";
		$param["where"]["id"]=$param["primaryValue"];
		return self::selectWithLQForm($param);

	}


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// class StageScore extends DBRow{
// 	public $m_no;
// 	public $m_memberID;
// 	public $m_stageNo;
// 	public $m_score;
// 	public $m_regDate;
// 	public $m_regTime;
// 	public $m_data;
	
// 	public function __construct($stageNo=null,$memberID=null,$where=null){
		
// 		if(!$stageNo)return false;
		
// 		parent::__construct();
		
		
// 		self::setPrimarykey("no");
// 		//$this->setDBTable(DBManager::getST("stageScore"));
// 		self::setDBInfo(DBManager::getDBInfoByShardKey($stageNo));
		
// 		if($memberID || $where){
// 			if($where)$q_where = $where;
// 			else $q_where = "memberID=".$memberID." and stageNo=".$stageNo;
			

// 			LogManager::addLog("stage score construct ".$q_where);
// 			if(parent::load($q_where)){
// 				$this->autoMatching($this->m__result);
// 				// $this->m_no = $this->m__result["no"];
// 				// $this->m_stageNo = $this->m__result["stageNo"];
// 				// $this->m_memberID = $this->m__result["memberID"];
// 				// $this->m_score = $this->m__result["score"];
// 				// $this->m_regDate = $this->m__result["regDate"];
// 				// $this->m_data = $this->m__result["data"];
// 			}
// 		}
// 	}
// }


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
		
		// self::setPrimarykey("no");
		// self::setDBInfo(DBManager::getMainDBInfo());
		

		if($puzzleNo)$this->no=$puzzleNo;
		if($order)$this->order=$order;

		if($puzzleNo || $order){
			$query = "`order`=".$order;
			if($puzzleNo)$query = "no=".$puzzleNo;

			if(parent::load($query)){

				$this->jsonToObj("center");
				$this->jsonToObj("face");
				$this->jsonToObj("original");
				$this->jsonToObj("thumbnail");
				$this->jsonToObj("map");
				$this->jsonToObj("pathInfo");
				$this->jsonToObj("coordinateInfo");
				$this->jsonToObj("startPosition");
				$this->jsonToObj("endPosition");
				$this->jsonToObj("color");
				$this->jsonToObj("clearReward");
				$this->jsonToObj("title");
			}
		}
	}

	public function getArrayDataForClient(){
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

	public static function loadWithLQTable($p){
		$intEditor = array("type"=>"text","datatype"=>"int");
		$textViewer = array("type"=>"text");
		$textareaEditor = array("type"=>"textarea");
		$dictEditor = array("type"=>"dictionary");
		$osEditor = json_decode('{"type":"select","element":["all","android","ios"]}',true);
		$imgEditor = json_decode('{"type":"dictionary","element":[{"field":"image","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}',true);
		$eventEditor = json_decode('{"type":"select","element":["일반","이벤트"],"value":[0,1]}',true);
		$langEditor = json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true);
		$coordinateEditor = json_decode('{"type":"table","element":[
			{"title":"x","field":"x","type":"text","datatype":"int"},
			{"title":"y","field":"y","type":"text","datatype":"int"}					
			]}',true);
		$positionEditor = json_decode('{"type":"dictionary","element":[
				{"field":"x","type":"text","datatype":"int"},
				{"field":"y","type":"text","datatype":"int"}			
			]}',true);
		$colorEditor = json_decode('{"type":"dictionary","element":[{"field":"r","type":"text","datatype":"int"},{"field":"g","type":"text","datatype":"int"},{"field":"b","type":"text","datatype":"int"}]}',true);
		$pathEditor = json_decode('{"type":"array","element":{"type":"text","datatype":"int"}}',true);
		$cardEditor = json_decode('{"type":"array","element":{"type":"array","element":{"type":"text","datatype":"int"}}}',true);
		$conditionEditor = json_decode('{"type":"array","element":{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"value","datatype":"int"}]}}}',true);
		$imgViewer = json_decode('{"type":"showPuzzleImg"}',true);

		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("field"=>"order","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("field"=>"isEvent","viewer"=>$textViewer,"editor"=>$eventEditor);
		$data["head"][]=array("field"=>"title","viewer"=>$textViewer,"editor"=>$langEditor);
		$data["head"][]=array("field"=>"thumbnail","viewer"=>$imgViewer,"editor"=>$imgEditor);
		$data["head"][]=array("field"=>"center","viewer"=>$textViewer,"editor"=>$imgEditor);
		$data["head"][]=array("field"=>"original","viewer"=>$textViewer,"editor"=>$imgEditor);
		$data["head"][]=array("field"=>"face","viewer"=>$textViewer,"editor"=>$imgEditor);
		$data["head"][]=array("field"=>"map","viewer"=>$textViewer,"editor"=>$imgEditor);
		$data["head"][]=array("field"=>"coordinateInfo","viewer"=>$textViewer,"editor"=>$coordinateEditor);
		$data["head"][]=array("field"=>"startPosition","viewer"=>$textViewer,"editor"=>$positionEditor);
		$data["head"][]=array("field"=>"endPosition","viewer"=>$textViewer,"editor"=>$positionEditor);
		$data["head"][]=array("field"=>"color","viewer"=>$textViewer,"editor"=>$colorEditor);
		$data["head"][]=array("field"=>"conditionInfo","title"=>"conditionInfo (자동갱신)","viewer"=>$textViewer);
		$data["head"][]=array("field"=>"rewardInfo","title"=>"rewardInfo (자동갱신)","viewer"=>$textViewer);
		$data["head"][]=array("field"=>"levelInfo","title"=>"levelInfo (자동갱신)","viewer"=>$textViewer);
		$data["head"][]=array("field"=>"pathInfo","title"=>"pathInfo","viewer"=>$textViewer,"editor"=>$pathEditor);
		$data["head"][]=array("field"=>"cardInfo","title"=>"cardInfo","viewer"=>$textViewer,"editor"=>$cardEditor);
		$data["head"][]=array("field"=>"ticket","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("field"=>"point","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("field"=>"condition","viewer"=>$textViewer,"editor"=>$conditionEditor);
		$data["head"][]=array("field"=>"clearReward","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"type":"text","field":"normal"},{"type":"text","field":"perfect"}]}',true));
		
		$data["head"][]=array("field"=>"version","viewer"=>$textViewer,"editor"=>$intEditor,"always");

		$data["head"][]=array("manage"=>"update delete insert");
		 
		return $data;
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
		
		// self::setPrimarykey("no");
		// self::setDBInfo(DBManager::getMainDBInfo());
		
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

		while($myCard = Card::getObjectByQuery("where piece='".$stageInfo[no]."'",0)){
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
			kvManager::increase("puzzleListVer");
			$puzzle->version=kvManager::get("puzzleListVer",1);
			CommitManager::setSuccess("main",$puzzle->save());
		}

		kvManager::increase("pieceListVer");
		$p["data"]["version"]=kvManager::get("pieceListVer",1);
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
		if($p["data"]["puzzle"]){
			$puzzle = new Puzzle($p["data"]["puzzle"]);
			kvManager::increase("puzzleListVer");
			$puzzle->version=kvManager::get("puzzleListVer",1);
			CommitManager::setSuccess("main",$puzzle->save());
		}

		kvManager::increase("pieceListVer");
		$p["data"]["version"]=kvManager::get("pieceListVer",1);
 
		$r = parent::updateWithLQTable($p);
		CommitManager::setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
		}
	}

	public static function deleteWithLQTable($p){
		kvManager::increase("pieceListVer");
		return parent::deleteWithLQTable($p);
	}

	public static function loadWithLQTable($p){

		$listViewer=array("type"=>"select","field"=>"type","title"=>"몬스터명");
		while($pData = Monster::getRowByQuery("",null,"name,no")){
			$listViewer["element"][] = $pData["no"]."-".$pData["name"];
			$listViewer["value"][]=$pData["name"];
		}

		$listViewer2=array("type"=>"custom","func"=>"patternSelector","field"=>"pattern","title"=>"패턴");
		while($pData = Pattern::getRowByQuery("",null,"type,name,template")){
			$listViewer2["element"][] = $pData["type"]."-".$pData["name"];
			$listViewer2["value"][]=$pData["type"];
			$listViewer2["template"][$pData["type"]]=$pData["template"];
		}

		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"puzzle","viewer"=>json_decode('{"type":"text"}',true),"always");
		$data["head"][]=array("field"=>"book","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"theme","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"level","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text",{"datatype":"int"}}',true));
		$data["head"][]=array("field"=>"rewardInfo","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"text","datatype":"int"}}',true));
		$data["head"][]=array("field"=>"scale","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"autoBalanceTry","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"boss","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"table","element":[
					{"field":"shape","title":"모양","type":"select","element":["circle","snake"]},
					'.json_encode($listViewer).',
					{
						"title":"패턴",
						"field":"pattern",
						"type":"array",
						"element":{
										"type":"dictionary",
										"element":[
											'.json_encode($listViewer2).',
											{"field":"target","type":"select","element":["no","yes"]},
											{"field":"atype","type":"select","element":["normal","special","crash"]},
											{"field":"percent","type":"text","datatype":"int"}
							
						]}			
					},			
							
					{   
					    "title":"속도",
						"field":"speed","type":"dictionary",
						"element":[
											{"field":"max","type":"text","datatype":"int"},
											{"field":"start","type":"text","datatype":"int"},
											{"field":"min","type":"text","datatype":"int"}
										]
					},
					{
					    "title":"스케일",
						"field":"scale","type":"dictionary",
						"element":[
											{"field":"max","type":"text","datatype":"int"},
											{"field":"start","type":"text","datatype":"int"},
											{"field":"min","type":"text","datatype":"int"}
										]
					},
					{
					    "title":"움직임",
						"field":"movement","type":"dictionary",
						"element":[
											{"field":"normal","type":"text","datatype":"int"},
											{"field":"draw","type":"text","datatype":"int"}
										]
					},
					{"title":"공격주기","field":"attackpercent","type":"text","datatype":"int"},
					{"title":"에너지","field":"hp","type":"text","datatype":"int"}	,
					{"title":"민첩","field":"agi","type":"text","datatype":"int"}	,
					{"title":"AI","field":"ai","type":"text","datatype":"int"}	,
					{"title":"isflipx","field":"isflipx","type":"bool"}	
					]}',true));
		$data["head"][]=array("field"=>"junior","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"table","element":[
					
					'.json_encode($listViewer).',			
					{   
					    "title":"속도",
						"field":"speed","type":"dictionary",
						"element":[
											{"field":"max","type":"text","datatype":"int"},
											{"field":"start","type":"text","datatype":"int"},
											{"field":"min","type":"text","datatype":"int"}
										]
					},
					{
					    "title":"스케일",
						"field":"scale","type":"dictionary",
						"element":[
											{"field":"max","type":"text","datatype":"int"},
											{"field":"start","type":"text","datatype":"int"},
											{"field":"min","type":"text","datatype":"int"}
										]
					},
					{
					    "title":"움직임",
						"field":"movement","type":"dictionary",
						"element":[
											{"field":"normal","type":"text","datatype":"int"},
											{"field":"draw","type":"text","datatype":"int"}
										]
					},
					{"title":"에너지","field":"hp","type":"text","datatype":"int"}	,
					{"title":"민첩","field":"agi","type":"text","datatype":"int"}	,
					{"title":"AI","field":"ai","type":"text","datatype":"int"}		,
					{"title":"isflipx","field":"isflipx","type":"bool"}	
								
					]}',true));
		$data["head"][]=array("field"=>"mission","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[
						{"field":"type","type":"text","datatype":"int"},
						{
						 "field":"option",
						 "type":"dictionary",
						 "element":[]
						}
					]}
					',true));
		$data["head"][]=array("field"=>"point","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"scoreRate","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"playtime","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"shopItems","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"table","element":
					[
						{
							"title":"아이템번호",
							"field":"type",
							"type":"text",
							"datatype":"int"
						},
						{
							"title":"통화",
							"field":"currency",
							"type":"select",
							"element":["gold","ruby","social"]
						},
						{
							"title":"가격",
							"field":"price",
							"type":"text",
							"datatype":"int"
						},
						{
							"title":"옵션",
							"field":"option",
							"type":"dictionary",
							"element":[]
						}

					]}',true));
		$data["head"][]=array("field"=>"defItems","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"table","element":[
						{
							"title":"아이템번호",
							"field":"type",
							"type":"text",
							"datatype":"int"
						},
						{
							"title":"옵션",
							"field":"option",
							"type":"dictionary",
							"element":[]
						}

					]}',true));
		$data["head"][]=array("field"=>"cards","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"text","datatype":"int"}}',true));
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"pieceNo","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"pieceType","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["h","w"]}',true));
		$data["head"][]=array("field"=>"condition","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":[
						{"field":"gold","type":"text","datatype":"int"},
						{"field":"pieceNo","type":"text","datatype":"int"}			
					]}',true));
		$data["head"][]=array("field"=>"minigame","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"bool"}',true));
		$data["head"][]=array("field"=>"version","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true),"always");
		$data["head"][]=array("field"=>"type","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["normal","special"]}',true));
		$data["head"][]=array("title"=>"오토레벨적용여부","field"=>"autoLevel","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"bool"}',true));
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
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
				$this->jsonToObj("ability");
				$this->jsonToObj("missile");
				$this->jsonToObj("imgInfo");
				$this->jsonToObj("aniInfo");
				$this->jsonToObj("thumbnailInfo");
				$this->jsonToObj("silImgInfo");
				$this->jsonToObj("mPrice");
				$this->jsonToObj("name");
				$this->jsonToObj("script");
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
				kvManager::increase("puzzleListVer");
				$puzzle->version=kvManager::get("puzzleListVer",1);
				CommitManager::setSuccess("main",$puzzle->save());
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
				kvManager::increase("puzzleListVer");
				$puzzle->version=kvManager::get("puzzleListVer",1);
				CommitManager::setSuccess("main",$puzzle->save());
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

	public static function loadWithLQTable($p){




		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"rank","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"grade","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"name","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true));
		// $data["head"][]=array("field"=>"reward","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		// $data["head"][]=array("field"=>"durability","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		// $data["head"][]=array("field"=>"ability","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"table","element":[{"title":"아이템번호","field":"type","type":"text","datatype":"int"},{"title":"옵션","field":"option","type":"dictionary"}]}',true));
		// $data["head"][]=array("field"=>"passive","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":[
		// 			{"title":"operator","field":"operator","type":"select","element":["-","*(1-x)"]},	
		// 			{
		// 				"title":"패턴",
		// 				"field":"pattern",
		// 				"type":"dictionary",
		// 				"element":[
		// 					{"field":"castframe","type":"text","datatype":"int"},
		// 					{"field":"totalframe","type":"text","datatype":"int"},
		// 					{"field":"speed","type":"text","datatype":"int"}
		// 				]			
		// 			},			
							
		// 			{   
		// 			    "title":"속도",
		// 				"field":"speed","type":"text","datatype":"int"
		// 			},
		// 			{   
		// 			    "title":"스케일",
		// 				"field":"scale","type":"text","datatype":"int"
		// 			},
		// 			{"title":"공격주기","field":"attackpercent","type":"text","datatype":"int"},
		// 			{"title":"에너지","field":"hp","type":"text","datatype":"int"},
		// 			{"title":"민첩","field":"agi","type":"text","datatype":"int"}	,
		// 			{"title":"AI","field":"ai","type":"text","datatype":"int"}				
		// 			]}',true));
		//$data["head"][]=array("field"=>"missile","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"speed","type":"text","datatype":"int"},{"field":"power","type":"text","datatype":"int"},{"field":"dex","type":"text","datatype":"int"}]}',true));
		$data["head"][]=array("field"=>"cc","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["kr","jp","en","cn"]}',true));
		$data["head"][]=array("field"=>"stage","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"piece","viewer"=>json_decode('{"type":"text"}',true),'always');
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"imgInfo","viewer"=>json_decode('{"type":"showCardImg"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}',true));
		$data["head"][]=array("field"=>"aniInfo","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[
						{"field":"isAni","type":"bool"},
						{"field":"detail",
						 "type":"dictionary",
						 "element":[
										{"field":"img","type":"imageSelector"},
										{"field":"size","type":"text","datatype":"int"},
										{"field":"loopLength","type":"text","datatype":"int"},
										{"field":"loopSeq","type":"array","element":{"type":"text","datatype":"int"}},
										{"field":"cutWidth","type":"text","datatype":"int"},
										{"field":"cutHeight","type":"text","datatype":"int"},
										{"field":"cutLength","type":"text","datatype":"int"},
										{"field":"positionX","type":"text","datatype":"int"},
										{"field":"positionY","type":"text","datatype":"int"}
							]
								}
					]}',true));
		$data["head"][]=array("field"=>"script","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true));
		$data["head"][]=array("field"=>"profile","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("field"=>"silImgInfo","viewer"=>json_decode('{"type":"text","cut":50}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"field":"isSil","type":"bool"},{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}',true));
		$data["head"][]=array("field"=>"mPrice","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"field":"r","type":"text","datatype":"int"},{"field":"p6","type":"text","datatype":"int"}]}',true));
		$data["head"][]=array("field"=>"category","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["normal","nPuzzle","special","event","ePuzzle","gift","leader"]}',true));
		$data["head"][]=array("field"=>"type","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["음","양","섬"],"value":[1,2,3]}',true));
		$data["head"][]=array("field"=>"level","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"characterNo","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"sound","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"text"}}',true));
		$data["head"][]=array("field"=>"version","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true),"always");
		$data["head"][]=array("manage"=>"[delete,update,insert]");

		return $data;
	}



	public static function loadWithLQTableForCardInfo($p){

		$data["head"][]=array("title"=>"카드번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"배치스테이지","field"=>"piece","viewer"=>json_decode('{"type":"text"}',true),'always');
		$data["head"][]=array("title"=>"이미지","field"=>"imgInfo","viewer"=>json_decode('{"type":"imageInfoViewer"}',true));
		$data["head"][]=array("title"=>"이름","field"=>"name","viewer"=>json_decode('{"type":"languageViewer"}',true),"editor"=>json_decode('{"type":"languageEditor"}',true));
		$data["head"][]=array("title"=>"스크립트","field"=>"script","viewer"=>json_decode('{"type":"languageViewer"}',true),"editor"=>json_decode('{"type":"languageEditor"}',true));
		$data["head"][]=array("title"=>"프로필","field"=>"profile","viewer"=>json_decode('{"type":"languageViewer"}',true),"editor"=>json_decode('{"type":"languageEditor","element":{"type":"textarea"}}',true));
		$data["head"][]=array("manage"=>"[update]");

		return $data;
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
	public static function loadWithLQTable($p){

		$listViewer=array("type"=>"select");
		while($pData = Card::getRowByQuery("",null,"name,no")){
			$lang = json_decode($pData["name"],true);
			$listViewer["element"][] = $pData["no"]."-".$lang["ko"];
			$listViewer["value"][]=$pData["no"];
		}

		$listViewer2=array("type"=>"select");
		while($pData = Puzzle::getRowByQuery("",null,"title,no")){
			$lang=json_decode($pData["title"],true);
			$listViewer2["element"][] = $pData["no"]."-".$lang["ko"];
			$listViewer2["value"][]=$pData["no"];
		}

		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$intEditor = json_decode('{"type":"text","datatype":"int"}',true);
		$osEditor = json_decode('{"type":"select","element":["all","android","ios"]}',true);
		$rewardEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"type","field":"type"},{"type":"text","field":"count"}]}}',true);
		$exchangeEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"카드","field"=>"cardNo","viewer"=>$listViewer,"editor"=>$intEditor);
		$data["head"][]=array("title"=>"갯수","field"=>"count","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"퍼즐","field"=>"puzzleNo","viewer"=>$listViewer2);
		$data["head"][]=array("title"=>"코멘트","field"=>"comment","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"획득일시","field"=>"takeDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"모핑여부","field"=>"isMorphing","viewer"=>json_decode('{"type":"bool"}',true),"editor"=>json_decode('{"type":"bool"}',true));
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}
	public static function updateWithLQTable($p){
		unset($p["data"]["puzzleNo"]);
		$r = parent::updateWithLQTable($p);

		if($p["comment"]){
			$mh = new ModifyHistory($r["data"]["memberID"]);
			unset($p["data"][self::getPrimarykey()]);
			$mh->oldData=$p["oldData"];
			$mh->newData=$p["data"];
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
	}

	public static function deleteWithLQTable($p){
		$r = parent::deleteWithLQTable($p);
		if($p["comment"]){
			$mh = new ModifyHistory($p["data"]["memberID"]);
			unset($p["data"][self::getPrimarykey()]);
			$mh->oldData=$p["data"];
			$mh->newData="삭제";
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
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

	public static function loadWithLQTable($p){

		$puzzleViewer=array("type"=>"select");
		while($pData = Puzzle::getRowByQuery("",null,"title,no")){
			$lang = json_decode($pData["title"],true);
			$puzzleViewer["element"][] = $pData["no"]."-".$lang["ko"];
			$puzzleViewer["value"][]=$pData["no"];
		}

		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"퍼즐정보","field"=>"puzzleNo","viewer"=>$puzzleViewer,"editor"=>$puzzleViewer);
		$data["head"][]=array("title"=>"오픈유형","field"=>"openType","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"최초오픈일시","field"=>"openDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"최초완성일시","field"=>"clearDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"퍼펙트완성일시","field"=>"perfectDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"보상","field"=>"reward","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("manage"=>"update delete insert");		
		return $data;
	}

	public static function updateWithLQTable($p){

		unset($p["data"]["reward"]);
		$r = parent::updateWithLQTable($p);

		if($p["comment"]){
			$mh = new ModifyHistory($r["data"]["memberID"]);
			unset($p["data"][self::getPrimarykey()]);
			$mh->oldData=$p["oldData"];
			$mh->newData=$p["data"];
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
	}

	public static function deleteWithLQTable($p){
		$r = parent::deleteWithLQTable($p);
		if($p["comment"]){
			$mh = new ModifyHistory($p["data"]["memberID"]);
			unset($p["data"][self::getPrimarykey()]);
			$mh->oldData=$p["data"];
			$mh->newData="삭제";
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
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

	public static function loadWithLQTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"스테이지번호","field"=>"pieceNo","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"최초진입일시","field"=>"openDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"최초클리어일시","field"=>"firstClearDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"플레이횟수","field"=>"tryCount","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"진행상태","field"=>"clearDateList","viewer"=>json_decode('{"type":"clearList"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"datetime"}}',true));
		$data["head"][]=array("manage"=>"update delete insert");
		return $data;
	}

	public static function updateWithLQTable($p){
		
		unset($p["data"]["reward"]);
		$r = parent::updateWithLQTable($p);

		if($p["comment"]){
			$mh = new ModifyHistory($r["data"]["memberID"]);
			unset($p["data"][self::getPrimarykey()]);
			$mh->oldData=$p["oldData"];
			$mh->newData=$p["data"];
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
	}

	public static function deleteWithLQTable($p){
		$r = parent::deleteWithLQTable($p);
		if($p["comment"]){
			$mh = new ModifyHistory($p["data"]["memberID"]);
			unset($p["data"][self::getPrimarykey()]);
			$mh->oldData=$p["data"];
			$mh->newData="삭제";
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
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
		
		// self::setPrimarykey("no",true);
		// self::setDBInfo(DBManager::getMainDBInfo());
		
		// self::setLQTableSelectCustomFunction(function ($data){

		// 	LogManager::addLog("called custom function");

		// 	if($data["exchangeID"]){
		// 		$exchange = new Exchange($data["exchangeID"]);
		// 		$data["exchangeList"]=$exchange->list;				
		// 	}
		// 	return $data;
		// });

		if($id)$this->id=$id;

		if($id){
			$query = "`id`=".$id;
			if(parent::load($query)){
				$this->jsonToObj("reward");
			}
		}
	}

	// public static function updateWithLQTable($p){
		

	// 	$exchange=null;
	// 	if($p["data"]["exchangeID"]){
	// 			$exchange = new Exchange($p["data"]["exchangeID"]);
	// 			if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
	// 			if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
	// 			$exchange->save();		
	// 	}
	// 	unset($p["data"]["exchangeList"]);
	// 	$r = parent::updateWithLQTable($p);

	// 	if($exchange){
	// 		$r["data"]["exchangeList"]=$exchange->list;
	// 	}

	// 	kvManager::increase("arcListVer");

	// 	return $r;
	// }

	public static function updateWithLQTable($p){
		
		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}

		$r = parent::updateWithLQTable($p);

		kvManager::increase("arcListVer");
		
		return $r;
	}

	// public static function insertWithLQTable($p){
	// 	$exchange=null;
	// 	if($p["data"]["exchangeID"]){
	// 			$exchange = new Exchange($p["data"]["exchangeID"]);
	// 			if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
	// 			if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
	// 			$exchange->save();		
	// 	}
	// 	unset($p["data"]["exchangeList"]);
	// 	$r = parent::insertWithLQTable($p);

	// 	if($exchange){
	// 		$r["data"]["exchangeList"]=$exchange->list;
	// 	}
	// 	kvManager::increase("arcListVer");

	// 	return $r;
	// }

	public static function insertWithLQTable($p){
		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}
		$r = parent::insertWithLQTable($p);

		kvManager::increase("arcListVer");

		return $r;
	}


	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$dictEditor = array("type"=>"dictionary");
		$intEditor = json_decode('{"type":"text","datatype":"int"}',true);
		$typeEditor = json_decode('{"type":"select","element":["gold","ruby","heart"],"value":["g","r","h"]}',true);
		$countNameEditor = json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true);
		$exchangeListEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		$rewardEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"count","type":"text","datatype":"int"}]}}',true);
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"editor"=>$textEditor,"primary");
		$data["head"][]=array("title"=>"그룹번호","field"=>"groupNo","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("title"=>"업적ID","field"=>"id","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"공개여부","field"=>"category","viewer"=>json_decode('{"type":"select","element":["공개","숨김"],"value":["d","h"]}',true),"editor"=>json_decode('{"type":"select","element":["공개","숨김"],"value":["d","h"]}',true));
		$data["head"][]=array("title"=>"제목","field"=>"title","viewer"=>json_decode('{"type":"languageViewer"}',true),"editor"=>json_decode('{"type":"languageEditor"}',true));
		$data["head"][]=array("title"=>"내용","field"=>"content","viewer"=>json_decode('{"type":"languageViewer"}',true),"editor"=>$dictEditor);
		$data["head"][]=array("title"=>"목표","field"=>"goal","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"exchangeviewer"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"업적","statsID":"archive","statsValueField":"no"}',true));
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
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
			if($rData["rewardDate"]>0){
				$archive = new Archivement($rData["archiveID"]);
				$rData["reward"]=$archive->reward;
			}
			return $rData;
		});

		$this->memberID = $memberID;
		if($fNo)$this->archiveID=$fNo;

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and archiveID='".$fNo."'");
		}
	}

	public static function loadWithLQTable($p){

		$listViewer=array("type"=>"select");
		while($pData = Archivement::getRowByQuery("",null,"title,id")){
			$lang = json_decode($pData["title"],true);
			$listViewer["element"][] = $pData["id"]."-".$lang["ko"];
			$listViewer["value"][]=$pData["id"];
		}

		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"업적","field"=>"archiveID","viewer"=>$listViewer);
		$data["head"][]=array("title"=>"값","field"=>"count","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"목표","field"=>"goal","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"완료일시","field"=>"clearDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"보상일시","field"=>"rewardDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"보상","field"=>"reward","viewer"=>json_decode('{"type":"rewardViewer"}',true),"virtual");
		$data["head"][]=array("manage"=>"update delete");
		return $data;
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
			// if($rData["exchangeID"]){
			// 	$exchange = new Exchange($rData["exchangeID"]);
			// 	$rData["exchangeList"]=$exchange->list;				
			// }
			return $rData;
		});

		$this->memberID=$memberID;

		if($memberID && $fNo){
			if(parent::load("memberID=".$memberID." and no=".$fNo)){
				$this->jsonToObj("reward");
				$this->jsonToObj("exchangeList");
			}
		}
	}

	public static function getAllExchangeID($memberID){
		self::initial();

		$elist=array();
		while($rData = self::getRowByQueryWithShardKey("where memberID='".$memberID."' and confirmDate=0",$memberID)){
			if($rData["exchangeID"])$elist[]=$rData["exchangeID"];
		}

		return $elist;
	}

	public static function confirmAll($memberID){
		self::initial();

		LogManager::addLog("comfirmAll start ".$memberID);
		$lastDay = TimeManager::getDateTime(TimeManager::getTime()-60*60*24*30);
		$result = self::getQueryResultWithShardKey("update ".self::getDBTable()." set confirmDate='".TimeManager::getCurrentDateTime()."' where memberID='".$memberID."' and confirmDate=0 and regDate>$lastDay",$memberID,false);
		return $result;
	}

	// public static function updateWithLQTable($p){
	// 	$exchange=null;
	// 	// if($p["data"]["exchangeID"]){
	// 	// 	$exchange = new Exchange($p["data"]["exchangeID"]);
	// 	// }
		
	// 	//unset($p["data"]["exchangeList"]);
		
	// 	$r = self::updateWithLQTable($p);
		
	// 	//if($exchange)$r["data"]["exchangeList"]=$exchange->list;

	// 	return $r;
	// }

	// public static function insertWithLQTable($p){
	// 	$exchange=null;
	// 	// if($p["data"]["exchangeID"]){
	// 	// 	$exchange = new Exchange($p["data"]["exchangeID"]);
	// 	// }
		
	// 	//unset($p["data"]["exchangeList"]);

	// 	$r = self::insertWithLQTable($p);

	// 	//if($exchange)$r["data"]["exchangeList"]=$exchange->list;

	// 	return $r;
	// }

	public static function loadWithLQTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"받은일시","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"확인일시","field"=>"confirmDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"보낸사람","field"=>"sender","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"내용","field"=>"content","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"보상","field"=>"reward","viewer"=>json_decode('{"type":"rewardViewer"}',true),"virtual");
		// $data["head"][]=array("field"=>"data","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"text"}',true));
		// $data["head"][]=array("field"=>"exchangeList","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("manage"=>"delete update");
		return $data;
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

			LogManager::addLog("userstorage start custom func!".json_encode($param));
			LogManager::addLog("userstorage start custom func!".$param["where"]["id"]."=".$param["where"]["type"]);
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);


			LogManager::addLog("userstorage start custom func2");

			if(!$user->isLoaded()){
				
				LogManager::addLog("userstorage start custom func! fail");
				return "where memberID='-1'";
			}

			//LogManager::addLog("userstorage start custom func ok".var_export($user,true));
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

	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"유료젬","field"=>"pr","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"무료젬","field"=>"fr","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"하트","field"=>"h","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"현금","field"=>"m","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"골드","field"=>"g","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i1","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i2","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i3","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i4","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i5","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"아이템두배","field"=>"i6","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i7","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i8","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"신발","field"=>"i9","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i10","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"자석","field"=>"i11","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"이어하기","field"=>"p1","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"맵가챠","field"=>"p2","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"캐릭업글","field"=>"p3","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"아이템뽑기","field"=>"p4","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"99프로뽑기","field"=>"p5","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"생명의돌","field"=>"p6","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"p7","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("manage"=>"update");
		
		return $data;
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//	유저보관함히스토리
////////////////////////////////////////////////////////////////////////////////////////



class UserPropertyHistory extends DBTable2{
	 //   {ruby,gold,heart}   {add,use}  at Scene (for {ruby,gold,heart}) 

	const prefixAddGoldByGamePlay = "so_";
	const prefixAddGoldByPurchase = "pc_";
	const prefixAddRubyByPurchase = "pc_";
	const prefixAddHeartByPurchase = "pc_";
	const prefixAddHeartByTime = "fr_";
	const prefixAddItemByPurchase = "pc_";

	const prefixUseItem = "us_";
	const prefixUseHeart = "us_";
	const prefixUseGoldForItem = "us_";
	const prefixUseGoldForUpgrade = "up_";
	const prefixUseRubyForGold = "pg_";
	const prefixUseRubyForHeart = "pc_";

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("UserPropertyHistory");
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

	public static function loadWithLQTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"종류","field"=>"type","viewer"=>json_decode('{"type":"propChange"}',true));
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"변화량","field"=>"count","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"총개수","field"=>"total","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"내용","field"=>"content","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"획득일시","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		//$data["head"][]=array("manage"=>"update delete");
		return $data;
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
			}else if($r["startDate"]>=$now){
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
		while($rData = LoginEvent::getRowByQuery("where endDate>".TimeManager::getCurrentDateTime()." and endTime=235959 and endDate-startDate=235959 limit 3")){
			$rData["reward"]=json_decode($rData["reward"],true);
			$data[]=$rData;
		}
		return $data;
	}
	public static function updateWithLQTable($p){
		
		// $exchange=null;
		// if($p["data"]["exchangeID"]){
		// 		$exchange = new Exchange($p["data"]["exchangeID"]);
		// 		if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
		// 		if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
		// 		$exchange->save();		
		// }
		// unset($p["data"]["exchangeList"]);

		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}

		$r = parent::updateWithLQTable($p);

		// if($exchange){
		// 	$r["data"]["exchangeList"]=$exchange->list;
		// }

		return $r;
	}

	public static function insertWithLQTable($p){
		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}
		$r = parent::insertWithLQTable($p);
		return $r;
	}

	public static function loadWithLQTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"진행상태","field"=>"state","viewer"=>json_decode('{"type":"text"}',true),"virtual");
		$data["head"][]=array("title"=>"이벤트명","field"=>"title","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"운영체제","field"=>"os","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["all","android","ios"]}',true));
		$data["head"][]=array("title"=>"국가","field"=>"cc","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"시작일시","field"=>"startDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"종료일시","field"=>"endDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"시작시간","field"=>"startTime","viewer"=>json_decode('{"type":"time","format":"h:i:s"}',true),"editor"=>json_decode('{"type":"time"}',true));
		$data["head"][]=array("title"=>"종료시간","field"=>"endTime","viewer"=>json_decode('{"type":"time","format":"h:i:s"}',true),"editor"=>json_decode('{"type":"time"}',true));
		$data["head"][]=array("title"=>"반복","field"=>"repeat","viewer"=>json_decode('{"type":"bool"}',true),"editor"=>json_decode('{"type":"bool"}',true));
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"exchangeviewer"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"로그인이벤트","statsID":"loginEvent","statsValueField":"no"}',true));
		//$data["head"][]=array("title"=>"보상내용","field"=>"reward","viewer"=>json_decode('{"type":"rewardViewer"}',true));
		$data["head"][]=array("manage"=>"insert delete update");
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
			}else if($r["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}
			
			return $rData;
		});

		if($fNo){
			$query = "no=".$fNo;
		}else{
			$today = TimeManager::getCurrentDateTime();
			$query = "startDate<=".$today." and endDate>=".$today." order by `order` asc limit 1";
		}
		if(parent::load($query)){
			$this->jsonToObj("rewardList");
			$this->jsonToObj("exchangeIDList");
		}
		
	}

	public static function loadWithLQTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"진행상태","field"=>"state","viewer"=>json_decode('{"type":"text"}',true),"virtual");
		$data["head"][]=array("title"=>"우선순위","field"=>"order","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"제목","field"=>"title","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"운영체제","field"=>"os","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["all","android","ios"]}',true));
		$data["head"][]=array("title"=>"국가","field"=>"cc","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"시작일시","field"=>"startDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"종료일시","field"=>"endDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"보상정보","field"=>"rewardInfo","viewer"=>json_decode('{"type":"AtdrewardInfo"}',true),"virtual");
		// $data["head"][]=array("title"=>"보상목록","field"=>"rewardList","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"type","field":"type"}]}}',true));
		// $data["head"][]=array("title"=>"보상ID목록","field"=>"exchangeIDList","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"text"}}',true));
		$data["head"][]=array("manage"=>"insert delete update");
		return $data;
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
			$this->jsonToObj("rewardList");
			$this->jsonToObj("exchangeIDList");
		}
		
	}
	public static function updateWithLQTable($p){
		
		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}

		$r = parent::updateWithLQTable($p);

		return $r;
	}

	public static function insertWithLQTable($p){
		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}
		$r = parent::insertWithLQTable($p);
		return $r;
	}
	public static function loadWithLQTable($p){

		$listViewer=array("type"=>"select","field"=>"type");
		while($pData = AttendenceEvent::getRowByQuery("",null,"no,title")){
			$listViewer["element"][] = $pData["title"]."(".$pData["no"].")";
			$listViewer["value"][]=$pData["no"];
		}

		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"이벤트번호","field"=>"eventNo","viewer"=>$listViewer,"editor"=>$listViewer);
		$data["head"][]=array("title"=>"선물함내용","field"=>"title","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"몇번째날?","field"=>"day","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"exchangeviewer"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"로그인이벤트","statsID":"loginEvent","statsValueField":"no"}',true));
		//$data["head"][]=array("title"=>"보상내용","field"=>"reward","viewer"=>json_decode('{"type":"rewardViewer"}',true));
		
		$data["head"][]=array("manage"=>"insert delete update");
		return $data;
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//	미션이벤트
////////////////////////////////////////////////////////////////////////////////////////
class MissionEvent extends DBTable2{
	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aMissionEventTable");
	}
	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and no=".$fNo);
		}
	}

}

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
			}else if($r["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}
			$rData["cuponCount"] = CuponCode::getCuponCount($rData[no]);
			return $rData;
		});
		// self::setLQTableSelectCustomFunction(function ($data){
		// 	if($data["exchangeID"]){
		// 		$exchange = new Exchange($data["exchangeID"]);
		// 		$data["exchangeList"]=$exchange->list;				
		// 	}
		// 	return $data;
		// });

		if($fNo || $cuponCode){
			$q = "no=".$fNo;
			if($cuponCode)$q = "cuponCode=`".$cuponCode."`";
			if(parent::load($q)){
				$this->jsonToObj("reward");
			}
		}
	}


	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$osEditor = json_decode('{"type":"select","element":["all","android","ios"]}',true);
		$rewardEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"type","field":"type"},{"type":"text","field":"count"}]}}',true);
		$exchangeEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"진행상태","field"=>"state","viewer"=>json_decode('{"type":"text"}',true),"virtual");
		$data["head"][]=array("title"=>"쿠폰명","field"=>"title","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"갯수","field"=>"cuponCount","viewer"=>json_decode('{"type":"cuponCode"}',true),"virtual");
		$data["head"][]=array("title"=>"운영체제","field"=>"os","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["all","android","ios"]}',true));
		$data["head"][]=array("title"=>"시작일시","field"=>"startDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"종료일시","field"=>"endDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"중복사용","field"=>"isCommon","viewer"=>json_decode('{"type":"bool"}',true),"editor"=>json_decode('{"type":"bool"}',true));
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"쿠폰","statsID":"cupon","statsValueField":"no"}',true));
		$data["head"][]=array("title"=>"교환내용","field"=>"reward","viewer"=>json_decode('{"type":"rewardViewer"}',true));
		$data["head"][]=array("title"=>"작성자","field"=>"writer","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"생성일","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		//$data["head"][]=array("title"=>"중복사용","field"=>"exchangeList","viewer"=>$textViewer,"editor"=>$exchangeEditor);

		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}

	public static function updateWithLQTable($p){
		
		// $exchange=null;
		// if($p["data"]["exchangeID"]){
		// 		$exchange = new Exchange($p["data"]["exchangeID"]);
		// 		if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
		// 		if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
		// 		$exchange->save();		
		// }
		// unset($p["data"]["exchangeList"]);

		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}
		$r = parent::updateWithLQTable($p);

		// if($exchange){
		// 	$r["data"]["exchangeList"]=$exchange->list;
		// }

		return $r;
	}

	public static function insertWithLQTable($p){
		// $exchange=null;
		// if($p["data"]["exchangeID"]){
		// 		$exchange = new Exchange($p["data"]["exchangeID"]);
		// 		if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
		// 		if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
		// 		$exchange->save();		
		// }
		// unset($p["data"]["exchangeList"]);
		// $p["data"]["regDate"]=TimeManager::getCurrentDateTime();

		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}

		$r = parent::insertWithLQTable($p);

		// if($exchange){
		// 	$r["data"]["exchangeList"]=$exchange->list;
		// }

		return $r;
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
		
		LogManager::addLog("create cuponcode for".$fNo);
		
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
			LogManager::addLog("load cuponcode");
			parent::load("cuponCode='".$fNo."'");
		}
	}

	static public function getCuponCount($cuponNo){

		$alluser=0;
		$cnt=0;
		while($result = WeeklyScore::getQueryResult("select count(*) from ".self::getDBTable()." where cuponNo=".$cuponNo)){
			$cnt++;
			if($cnt>100){
				LogManager::addLog("superBreak getAllUser");
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


	public static function loadWithLQTable($p){
		$listViewer=array("type"=>"select");
		while($pData = CuponManager::getRowByQuery("",null,"title,no")){
			$listViewer["element"][] = $pData["no"]."-".$pData["title"];
			$listViewer["value"][]=$pData["no"];
		}

		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$codeEditor = array("type"=>"array","element"=>array("type"=>"text"));
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"쿠폰번호","field"=>"cuponNo","viewer"=>$listViewer,"editor"=>$listViewer);
		$data["head"][]=array("title"=>"쿠폰코드","field"=>"cuponCode","viewer"=>json_decode('{"type":"cuponCodeViewer"}',true),"editor"=>json_decode('{"type":"cuponMaker"}',true));
		$data["head"][]=array("title"=>"관리서버","field"=>"serverNo","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"사용여부","field"=>"isUsed","viewer"=>$textViewer,"virtual");
		$data["head"][]=array("title"=>"사용일시","field"=>"usedDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"virtual");
		$data["head"][]=array("title"=>"사용유저","field"=>"memberID","viewer"=>$textViewer,"virtual");
		$data["head"][]=array("manage"=>"delete insert");
		
		return $data;
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
		LogManager::addLog("test count is ".count($codeList));
		CommitManager::begin("main");
		for($i=0;$i<$cnt;$i++){

			LogManager::addLog("test1 ".$i);
			
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



	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"쿠폰명","field"=>"cuponTitle","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"쿠폰번호","field"=>"cuponNo","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"코드코드","field"=>"cuponCode","viewer"=>json_decode('{"type":"cuponCodeViewer"}',true));
		$data["head"][]=array("title"=>"사용일시","field"=>"useDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		
		//$data["head"][]=array("manage"=>"");
		
		return $data;
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

		LogManager::addLog("CuponUsedInfo setNewShardKeyFunc call");
		//$func = CuponUsedInfo::createShardOrder;
		self::setNewShardKeyFunc(function($code){
			LogManager::addLog("st new shard key func !!!!!!! for cuponUsedInfo");
			//서버대수만큼 나눠서 관리
			srand((double)microtime()*1000000);
			$cNumber = rand(1,self::getDBGroup()->getMasterCount());
			return $cNumber;
		});

		self::setGetShardKeyFunc(function($code){
			//서버대수만큼 나눠서 생성
			LogManager::addLog("st get shard key func !!!!!!! for cuponUsedInfo with ".$code);
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
		self::setDBGroup("user");
		self::setDBTable("WeeklyScoreTable");
		self::setShardKey("memberID");
	}


	public function __construct($memberID=null,$weekNo=null){
		parent::__construct();
		$this->setDBShardKeyValue($memberID);

		$this->memberID=$memberID;
		
		self::setLQTableSelectQueryCustomFunction(function ($param){

			$where = "where 1=1";

			if($param["where"]["weekNo"]!="*"){
				$where = "where regWeek=".$param["where"]["weekNo"];
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

		self::setLQTableSelectCustomFunction(function($data){
			$data["data"] = json_decode($data["data"],true);
			$data["nick"] = $data["data"]["nick"];
			return $data;
		});
		if($memberID && $weekNo){
			$this->regWeek = $weekNo;
			parent::load("memberID=".$memberID." and regWeek=".$weekNo);
		}
	}

	public function getTopRank($start=1,$count=50){
		$rdata = array(); 
		$start-=1;
		$query = "where regWeek=".$this->regWeek." order by score desc limit $start,$count";
		$rl=0;
		while($data = WeeklyScore::getRowByQuery($query)){
			if($rl>50){
				LogManager::addLog("superBreak getTopRank");
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
		while($result = WeeklyScore::getQueryResult("select count(*) from ".self::getDBTable()." where regWeek=".$this->regWeek)){
			$cnt++;
			if($cnt>50){
				LogManager::addLog("superBreak getAllUser");
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
		while($result = WeeklyScore::getQueryResult("select count(*) from ".self::getDBTable()." where regWeek=".$this->regWeek." and score>".$this->score)){			
			$cnt++;
			if($cnt>50){
				LogManager::addLog("superBreak getAllUser");
				break;
			}
			$data =mysql_fetch_array($result);
			$alluser+=$data[0];
		}
		
		return $alluser+1;
	}

	public static function loadWithLQTable($p){
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"regWeek","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"nick","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true),"virtual");
		$data["head"][]=array("field"=>"score","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"regDate","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
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
			

			LogManager::addLog("stage score construct ".$q_where);
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

	public static function loadWithLQTable($p){
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"stageNo","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"score","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"regDate","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("field"=>"data","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("field"=>"regTime","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
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

	static public function getPowerInfo($level){
		$r["nextPrice"]=$level*1000;
    	$r["power"]=$level*10;
    	$r["nextPower"]=($level+1)*10;
    	$r["prevPower"]=($level-1)*10;
    	$r["isMaxLevel"]=false;
    	if($level>=30)$r["isMaxLevel"]=true;

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

	public static function loadWithLQTable($p){
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"name","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true));
		$data["head"][]=array("field"=>"sale","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"purchaseInfo","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[
						{"field":"type","type":"text"},			
						{"field":"value","type":"text","datatype":"int"}			
					]
					}',true));
		$data["head"][]=array("field"=>"statInfo","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[
						{"field":"gold","type":"text","datatype":"int"},
						{"field":"percent","type":"text","datatype":"int"},
						{"field":"feverTime","type":"text","datatype":"int"},
						{"field":"speed","type":"text","datatype":"int"},
						{"field":"life","type":"text","datatype":"int"}	,
						{"field":"color","type":"select","element":[0,1,2,3,4,5,6]}				
									
					]
					}',true));
		$data["head"][]=array("field"=>"resourceInfo","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[	
						{"field":"ccbiID","type":"text"},		
						{"field":"ccbi","type":"resourceSelector"},
						{"field":"imageID","type":"text"},	
						{"field":"plist","type":"resourceSelector"},
						{"field":"pvrccz","type":"resourceSelector"},
						{"field":"size","type":"text","datatype":"int"}	
					]
					}',true));
		$data["head"][]=array("field"=>"comment","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true));
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
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

	public static function loadWithLQTable($p){
		$listViewer=array("type"=>"select");
		while($pData = Character::getRowByQuery("",null,"name,no")){
			$lang = json_decode($pData["name"],true);
			$listViewer["element"][] = $pData["no"]."-".$lang["ko"];
			$listViewer["value"][]=$pData["no"];
		}

		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"캐릭터","field"=>"characterNo","viewer"=>$listViewer,"editor"=>$listViewer);
		$data["head"][]=array("title"=>"레벨","field"=>"level","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"구입일시","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
	}

	public static function updateWithLQTable($p){
		
		unset($p["data"]["name"]);
		$r = parent::updateWithLQTable($p);

		if($p["comment"]){
			$mh = new ModifyHistory($r["data"]["memberID"]);
			unset($p["data"][self::getPrimarykey()]);
			$mh->oldData=$p["oldData"];
			$mh->newData=$p["data"];
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
	}

	public static function deleteWithLQTable($p){
		$r = parent::deleteWithLQTable($p);
		if($p["comment"]){
			$mh = new ModifyHistory($p["data"]["memberID"]);
			unset($p["data"][self::getPrimarykey()]);
			$mh->oldData=$p["data"];
			$mh->newData="삭제";
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
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
			}else if($r["startDate"]>=$now){
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


	public static function loadWithLQTable($p){
		$intEditor = array("type"=>"text","datatype"=>"int");
		$textViewer = array("type"=>"text");
		$textareaEditor = array("type"=>"textarea");
		$dictEditor = array("type"=>"dictionary");
		$osEditor = json_decode('{"type":"select","element":["all","android","ios"]}',true);
		$imgEditor = json_decode('{"type":"dictionary","element":[{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}',true);
		

		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"우선순위","field"=>"order","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("title"=>"진행상태","field"=>"state","viewer"=>$textViewer,"virtual");
		$data["head"][]=array("title"=>"제목","field"=>"title","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"시작일시","field"=>"startDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"종료일시","field"=>"endDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"운영체제","field"=>"os","viewer"=>$textViewer,"editor"=>$osEditor);
		$data["head"][]=array("title"=>"국가","field"=>"cc","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"팝업표시여부","field"=>"isPopup","viewer"=>$textViewer,"editor"=>json_decode('{"type":"bool"}'));
		$data["head"][]=array("title"=>"이미지","field"=>"imgInfo","viewer"=>json_decode('{"type":"viewer_image"}'),"editor"=>$imgEditor);
		$data["head"][]=array("title"=>"연결URL","field"=>"linkURL","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"클라이언트내용","field"=>"content","viewer"=>$textViewer,"editor"=>$textareaEditor);
		$data["head"][]=array("title"=>"목록표시여부","field"=>"isList","viewer"=>$textViewer,"editor"=>json_decode('{"type":"bool"}'));
		$data["head"][]=array("title"=>"목록표시내용","field"=>"webContent","viewer"=>$textViewer,"editor"=>$textareaEditor);
		$data["head"][]=array("title"=>"목록배너","field"=>"banner","viewer"=>json_decode('{"type":"viewer_image"}'),"editor"=>$imgEditor);
		
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
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


	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$dictEditor = array("type"=>"dictionary");
		$intEditor = json_decode('{"type":"text","datatype":"int"}',true);
		$typeEditor = json_decode('{"type":"select","element":["gold","ruby","heart"],"value":["g","r","h"]}',true);
		$countNameEditor = json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true);
		$exchangeListEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"editor"=>$textEditor,"primary");
		$data["head"][]=array("title"=>"국가코드","field"=>"cc","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"상품ID","field"=>"id","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"표시갯수","field"=>"countName","viewer"=>$textViewer,"editor"=> json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("title"=>"표시가격","field"=>"priceName","viewer"=>$textViewer,"editor"=> json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("title"=>"할인표시","field"=>"sale","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"exchangeviewer"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"상점","statsID":"shop","statsValueField":"no"}',true));
		$data["head"][]=array("title"=>"스토어결제ID","field"=>"pID","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"설명","field"=>"comment","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("title"=>"데이터","field"=>"data","viewer"=>$textViewer,"editor"=>$dictEditor);
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
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

		
		// self::setLQTableSelectCustomFunction(function ($data){
		// 	// if($data["exchangeID"]){
		// 	// 	$exchange = new Exchange($data["exchangeID"]);
		// 	// 	$data["exchangeList"]=$exchange->list;				
		// 	// }
		// 	return $data;
		// });

		self::setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($r["startDate"]>=$now){
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

	public static function updateWithLQTable($p){
		
		// $exchange=null;
		// if($p["data"]["exchangeID"]){
		// 		$exchange = new Exchange($p["data"]["exchangeID"]);
		// 		if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
		// 		if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
		// 		$exchange->save();		
		// }
		// unset($p["data"]["exchangeList"]);
		// $r = self::updateWithLQTable($p);

		// if($exchange){
		// 	$r["data"]["exchangeList"]=$exchange->list;
		// }
		// if($p["data"]["exchangeID"]){
		// 	$exchange = new Exchange($p["data"]["exchangeID"]);
		// 	if($exchange->isLoaded())$p["data"]["exchangeList"]=$exchange->list;
		// }

		$r = parent::updateWithLQTable($p);

		kvManager::increase("shopListVer");
		return $r;
	}

	public static function insertWithLQTable($p){
		// if($p["data"]["exchangeID"]){
		// 	$exchange = new Exchange($p["data"]["exchangeID"]);
		// 	if($exchange->isLoaded())$p["data"]["exchangeList"]=$exchange->list;
		// }

		kvManager::increase("shopListVer");
		$r = parent::insertWithLQTable($p);
		return $r;
		// $exchange=null;
		// if($p["data"]["exchangeID"]){
		// 		$exchange = new Exchange($p["data"]["exchangeID"]);
		// 		if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
		// 		if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
		// 		$exchange->save();		
		// }
		// unset($p["data"]["exchangeList"]);
		// $r = self::insertWithLQTable($p);

		// if($exchange){
		// 	$r["data"]["exchangeList"]=$exchange->list;
		// }

		// kvManager::increase("shopListVer");
		// return $r;
	}

	public static function loadWithLQTable($p){

		$listViewer=array("type"=>"select","field"=>"type");
		while($pData = Shop::getRowByQuery("",null,"id,priceName,countName,comment")){
			$listViewer["element"][] = $pData["comment"];
			$listViewer["value"][]=$pData["id"];
		}

		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$dictEditor = array("type"=>"dictionary");
		$intEditor = json_decode('{"type":"text","datatype":"int"}',true);
		$typeEditor = json_decode('{"type":"select","element":["gold","ruby","heart"],"value":["g","r","h"]}',true);
		$countNameEditor = json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true);
		$exchangeListEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		

		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"editor"=>$textEditor,"primary");
		$data["head"][]=array("title"=>"국가코드","field"=>"cc","viewer"=>$textViewer,"editor"=>$textEditor);
	
		$data["head"][]=array("title"=>"진행상태","field"=>"state","viewer"=>json_decode('{"type":"text"}',true),"virtual");
		
		$data["head"][]=array("title"=>"할인대상","field"=>"id","viewer"=>$listViewer,"editor"=>$listViewer);
		$data["head"][]=array("title"=>"시작일시","field"=>"startDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"종료일시","field"=>"endDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"시작시간","field"=>"startTime","viewer"=>json_decode('{"type":"time","format":"h:i:s"}',true),"editor"=>json_decode('{"type":"time"}',true));
		$data["head"][]=array("title"=>"종료시간","field"=>"endTime","viewer"=>json_decode('{"type":"time","format":"h:i:s"}',true),"editor"=>json_decode('{"type":"time"}',true));
	

		$data["head"][]=array("title"=>"표시갯수","field"=>"countName","viewer"=>$textViewer,"editor"=> json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("title"=>"표시가격","field"=>"priceName","viewer"=>$textViewer,"editor"=> json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("title"=>"할인표시","field"=>"sale","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"exchangeviewer"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"상점할인","statsID":"saleshop","statsValueField":"no"}',true));
		$data["head"][]=array("title"=>"스토어결제ID","field"=>"pID","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"설명","field"=>"comment","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("title"=>"데이터","field"=>"data","viewer"=>$textViewer,"editor"=>$dictEditor);
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	몬스터
////////////////////////////////////////////////////////////////////////////////////////
class Monster extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no");
		self::setDBGroup("main");
		self::setDBTable("aMonsterTable");
	}


	public function __construct($cardNo=null){
		
		parent::__construct();
		


		if($cardNo)$this->no=$cardNo;

		if($cardNo){
			$query = "no=".$cardNo;
			parent::load($query);
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

	public static function loadWithLQTable($p){
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"type","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["circle","snake","jr"]}',true));
		$data["head"][]=array("field"=>"name","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"image","viewer"=>json_decode('{"type":"imageViewer"}',true),"editor"=>json_decode('{"type":"imageSelector"}',true));
		$data["head"][]=array("field"=>"resourceInfo","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[	
						{"field":"ccbiID","type":"text"},		
						{"field":"ccbi","type":"resourceSelector","viewer":"text"},
						{"field":"imageID","type":"text"},	
						{"field":"plist","type":"resourceSelector","viewer":"text"},
						{"field":"pvrccz","type":"resourceSelector","viewer":"text"},
						{"field":"size","type":"text","datatype":"int"}					
					]
					}',true));
		$data["head"][]=array("field"=>"script","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[
						{"field":"ko","type":"dictionary","element":[
																	{"type":"text","field":"start"},
																	{"type":"text","field":"clear"},
																	{"type":"text","field":"fail"}
																	]
						},
						{"field":"en","type":"dictionary","element":[
																	{"type":"text","field":"start"},
																	{"type":"text","field":"clear"},
																	{"type":"text","field":"fail"}
																	]
						}
					]
					}',true));
		$data["head"][]=array("field"=>"comment","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("field"=>"version","viewer"=>$textViewer,"editor"=>$intEditor,"always");
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
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

	public function __construct($cardNo=null){
		
		parent::__construct();
		

		if($cardNo)$this->no=$cardNo;

		if($cardNo){
			$query = "no=".$cardNo;
			parent::load($query);
		}
	}

	public static function loadWithLQTable($p){
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"category","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"type","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"name","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"image","viewer"=>json_decode('{"type":"imageViewer"}',true),"editor"=>json_decode('{"type":"imageSelector"}',true));
		$data["head"][]=array("field"=>"template","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary"}',true));
		$data["head"][]=array("field"=>"comment","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
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
		
		self::setLQTableInsertCustomFunction(function ($param){
			return "`id`=".$param["data"]["id"];
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
		LogManager::addLog("exchangeID is ".$param["id"]);
		$exchange = new Exchange($param["id"]);
		if(!$exchange->isLoaded())return ResultState::makeReturn(ResultState::GDDONTFIND);
		LogManager::addLog("exchangelist is ".json_encode($exchange->list));
		$r["exchangeID"]=$exchange->id;
		$r["list"]=$exchange->list;
		$r["result"]=ResultState::successToArray();
		return $r;
	}

	public static function makeExchangeIDByRandom($param){
		$r["param"]=$param;
		if($param["id"]){
			LogManager::addLog("exchangeID is ".$param["id"]);
			$exchange = new Exchange($param["id"]);
			LogManager::addLog("exchangelist is ".json_encode($exchange->list));
			$r["exchangeID"]=$exchange->id;
			$r["list"]=$exchange->list;
			$r["result"]=ResultState::successToArray();
			return $r;
			
		}

		$exchange = new Exchange(CuponCode::getRandomString(10));
		
		$cnt = 0;
		while($exchange->isLoaded()){
			$newExID = CuponCode::getRandomString(10);
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
		self::setDBGroup("user");
		self::setDBTable("EndlessRank");
		self::setShardKey("memberID");

		self::setLQTableSelectQueryCustomFunction(function ($param){

			$where = "where 1=1";

			if($param["where"]["weekNo"]!="*"){
				$where = "where regWeek=".$param["where"]["weekNo"];
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

	public function __construct($memberID=null,$weekNo=null){

		parent::__construct();

		$this->setDBShardKeyValue($memberID);

		$this->memberID=$memberID;

		if($memberID && $weekNo){
			$this->regWeek = $weekNo;
			$q_where = "`memberID`='".$memberID."'";
			if($weekNo)$q_where.=" and regWeek=".$weekNo;
			if(parent::load($q_where)){
			}
		}
	}

public function getTopRank($start=1,$count=50){
		$rdata = array(); 
		$start-=1;
		$query = "where regWeek=".$this->regWeek." order by victory desc, score desc limit $start,$count";
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
		while($result = EndlessRank::getQueryResult("select count(*) from ".$this->getDBTable()." where regWeek=".$this->regWeek)){
			$data = mysql_fetch_array($result);
			$alluser+=$data[0];
		}

		return $alluser;
	}

	public function getMyRank(){
		if($this->score<=0)return -1;
		
		$alluser=0;
		while($result = EndlessRank::getQueryResult("select count(*) from ".$this->getDBTable()." where regWeek=".$this->regWeek." and (victory>".$this->victory." or (victory=".$this->victory." and score>".$this->score."))")){			
			$data =mysql_fetch_array($result);
			$alluser+=$data[0];
		}
		
		return $alluser+1;
	}

	public static function loadWithLQTable($p){
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"regWeek","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"nick","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"victory","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"score","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"regDate","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
	}
}



class EndlessPlayList extends DBTable2{

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("user");
		self::setDBTable("EndlessPlayList");
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

	public function getPlayDataByRandom($memberID,$lvl=1,$limit=1,$fieldlist="*"){
		$result = array();
		$query = self::getQueryResultWithShardKey("select ".$fieldlist." from `".$this->getDBTable()."` where memberID in (SELECT DISTINCT memberID FROM `".$this->getDBTable()."`) and victory<=".($lvl+1)." and victory>=".($lvl-1)." and memberID <> ".$memberID." ORDER BY RAND() limit ".$limit,$memberID);

		
		LogManager::addLog("select ".$fieldlist." from `".$this->getDBTable()."` where memberID in (SELECT DISTINCT memberID FROM `".$this->getDBTable()."`) and victory<=".($lvl+1)." and victory>=".($lvl-1)." and memberID <> ".$memberID." ORDER BY RAND() limit ".$limit);
		
		$check=false;
		while($rData = mysql_fetch_assoc($query)){
			if($check)unset($rData["playData"]);
			$result[]=$rData;
			$check=true;
		}
		LogManager::addLog("test size is".count($result));
		$i=0;
		while(count($result)<$limit){
			$lvl--;
			$query = self::getQueryResultWithShardKey("select ".$fieldlist." from `".$this->getDBTable()."` where memberID in (SELECT DISTINCT memberID FROM `".$this->getDBTable()."`) and victory<=".($lvl+1)." and victory>=".($lvl-1)." and memberID <> ".$memberID." ORDER BY RAND() limit ".$limit,$memberID);
			if($query){
				while($rData = mysql_fetch_assoc($query)){
					if($check)unset($rData["playData"]);
					$result[]=$rData;
				}
			}
			$i++;
			if($i>50)break;
		}

		LogManager::addLog(mysql_error());
		
		return $result;
	}

	public function getPlayDataByNo($no){

		// $query = self::getQueryResult("select * from `".$this->getDBTable()."` where no=".$no,1);
		// if($query)$result = mysql_fetch_assoc($query);
		// if(!$result){
		// 	$query = mysql_query("select * from `".$this->getDBTable()."` ORDER BY RAND() limit 1",self::getDBConnection());
		// 	if($query)$result = mysql_fetch_assoc($query);
		// }

		// LogManager::addLog(mysql_error());
		
		return $result;
	}

	public static function loadWithLQTable($p){

		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"닉네임","field"=>"nick","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"연승","field"=>"victory","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"레벨","field"=>"level","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"오토레벨","field"=>"autoLevel","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"점수","field"=>"score","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"데이터","field"=>"playData","viewer"=>json_decode('{"type":"text","cut":100}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("title"=>"등록일자","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"피스번호","field"=>"pieceNo","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("manage"=>"delete update");		
		return $data;
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
				$this->jsonToObj("value");
				if($conv){
					$this->value = $conv;
				}
			}
		}
	}

	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$textareaEditor = array("type"=>"textarea");
		$dictEditor = array("type"=>"dictionary");

		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("field"=>"key","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"value","viewer"=>$textViewer,"editor"=>$dictEditor);
		$data["head"][]=array("field"=>"comment","viewer"=>$textViewer,"editor"=>$textareaEditor);
		
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}

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

	public static function loadWithLQTable($p){
		$intEditor = array("type"=>"text","datatype"=>"int");
		$textViewer = array("type"=>"text");
		$textareaEditor = array("type"=>"textarea");
		$dictEditor = array("type"=>"dictionary");

		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("field"=>"key","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"value","viewer"=>$textViewer,"editor"=>$intEditor);
		
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
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

			LogManager::addLog("in custom function");

			$now = TimeManager::getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($r["startDate"]>=$now){
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


	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$dictEditor = array("type"=>"dictionary");
		$osEditor = json_decode('{"type":"select","element":["all","android","ios"]}',true);
		$intEditor = json_decode('{"type":"text","datatype":"int"}',true);
		$typeEditor = json_decode('{"type":"select","element":["gold","ruby","heart"],"value":["g","r","h"]}',true);
		$countNameEditor = json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true);
		$exchangeListEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"진행상태","field"=>"state","viewer"=>json_decode('{"type":"text"}',true),"virtual");
		$data["head"][]=array("title"=>"운영체제","field"=>"os","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["all","android","ios"]}',true));
		$data["head"][]=array("title"=>"국가","field"=>"cc","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"시작일시","field"=>"startDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"종료일시","field"=>"endDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"시작시간","field"=>"startTime","viewer"=>json_decode('{"type":"time","format":"h:i:s"}',true),"editor"=>json_decode('{"type":"time"}',true));
		$data["head"][]=array("title"=>"종료시간","field"=>"endTime","viewer"=>json_decode('{"type":"time","format":"h:i:s"}',true),"editor"=>json_decode('{"type":"time"}',true));
		$data["head"][]=array("title"=>"이벤트타입","field"=>"type","viewer"=>$textViewer,"editor"=>json_decode('{"type":"select","element":["gold","heart","card","clear"]}',true));
		$data["head"][]=array("title"=>"이벤트값","field"=>"value","viewer"=>$textViewer,"editor"=>$intEditor);


		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
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
		$admin = new AdminUser($_SESSION["admin_no"]);
		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY);
		}

		$this->writer = $admin->id;

		if(!$this->regDate)$this->regDate=TimeManager::getCurrentDateTime();
		return parent::save($isIncludePrimaryKey);
	}

	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"카테고리","field"=>"category","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"수정전데이터","field"=>"oldData","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"수정후데이터","field"=>"newData","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"수정사유","field"=>"comment","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"작성자","field"=>"writer","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"수정일시","field"=>"regDate","viewer"=>$textViewer);

		//$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}

	// public function selectWithLQTable($param){
	// 	if($param["where"]["type"]=="sno"){
	// 		$user = UserData::create($param["where"]["id"]);
	// 	}else if($param["where"]["type"]=="nick"){
	// 		$user = UserData::create(null,null,$param["where"]["id"]);
	// 	}else{
	// 		$user = UserData::create(null,$param["where"]["id"]);
	// 	}

	// 	if(!$user->isLoaded()){
	// 		$r = ResultState::makeReturn(ResultState::GDDONTFINDUSER,"유저를 찾지 못했습니다");
	// 		return $r;
	// 	}


	// }

}



/////////////////////////////////////
/////////////////////////////////////


class AdminUser extends DBTable2{
	public $m__isLogin = false;


	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aAdminUser");
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

		LogManager::addLog("login query is ".$q_where);
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
	}
	
	public function checkPermission($where){
		$p =& $this->getRef("permission");
		if($p[$where]=="true")return true;

		return false;
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

	public static function loadWithLQTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"ID","field"=>"id","viewer"=>$textViewer,"editor"=>$textViewer);
		$data["head"][]=array("title"=>"PW","field"=>"passwd","viewer"=>$textViewer,"editor"=>json_decode('{"type":"password"}',true));
		$data["head"][]=array("title"=>"권한","field"=>"permission","viewer"=>json_decode('{"type":"adminPermissionView"}',true),"editor"=>json_decode('{"type":"adminPermissionEditor"}',true));

		$data["head"][]=array("manage"=>"delete update insert");
		
		return $data;
	}
}


class Mission extends DBTable2{
	

	static public function construct(){
		self::setPrimarykey("no",true);
		self::setDBGroup("main");
		self::setDBTable("aMissionTable");
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

/*

class DBGroup{
	public $m_groupList;
	public $m_masterList;
	public $m_newShardKeyFunc;
	public $m_getShardKeyFunc;

	public static function create($name){
		self::$m_groupList[$name] = new DBGroup();
		return self::$m_groupList[$name];
	}

	public function addMaster($db){
		$this->m_masterList[]=$db;
	}

	public function getMaster($order){
		return $this->m_masterList[$order-1];
	}

	public function getMasterList(){
		return $this->m_masterList;
	}

	public function setNewShardKeyFunc($func){
		$this->m_newShardKeyFunc = $func;
	}

	public function setGetShardKeyFunc($func){
		$this->m_getShardKeyFunc = $func;
	}

	public function getMasterConnection($key){
		$func = $this->m_getShardKeyFunc;
		$serverOrder = $func($key);
		$server = $this->getMaster($serverOrder);
		return $server->getConnection();
	}
}

class DBMaster{
	public $m_name = null;
	public $m_server = null;
	public $m_slaveList = null;

	public function __construct($name,$server){
		$this->m_name=$name;
		$this->m_server=$server;
		$this->m_slaveList=array();
	}
	

	public function getConnection(){
		if(!$this->m_server)return null;
	
		$conn=$this->m_server->getConnection();
		
		mysql_select_db($this->m_name, $conn);
		
		return $conn;
	}
	
	public function closeConnection(){
		if($this->m_server)
			$this->m_server->closeConnection();
	}

	public function addSlave($db){
		$this->m_slaveList[]=$db;
	}

	public function getSlave($order){
		return $this->m_slaveList[$order-1];
	}

	public function getSlaveList(){
		return $this->m_slaveList;
	}

	public function getSlaveByRand(){
		srand((double)microtime()*1000000);
		$r  =rand(1,1000)%count($this->slaveList);
		return $htis->m_slaveList[$r];
	}

}


	$userGroup = new DBGroup("user");
	
	$userGroup->setNewShardKeyFunc(function($key){
		$userIndex = UserIndex::create($key);
		return $userIndex->userShardOrder;
	}); // 샤드키발급
	
	$userGroup->setGetShardKeyFunc(function($key){return 1;}); // 샤드키찾기

	$masterDB1 = new DBMaster("dg001",$server0Index);
	$salveDB1 = new DBSlave("dg001",$server0Index);
	$masterDB1->addSlave($salveDB1);
	$userGroup->addMaster($masterDB1);


	$masterDB2 = new DBMaster("dg002",$server0Index);
	$salveDB2 = new DBSlave("dg002",$server0Index);
	$masterDB2->addSlave($salveDB2);
	$userGroup->addMaster($masterDB2);

	class noticestable extends DBTable{
		public function __construct($memberID=null){
			parent::__construct();
			
			self::setDBGroup("user");

			
		}
	}
*/
?>