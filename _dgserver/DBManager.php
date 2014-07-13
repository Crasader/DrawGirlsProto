<?php
 
 include_once("DBManagerLib.php");

class CommitManager{
	

	public $m_dbInfo=null;
	public $m_userIndex=null;
	public $m_isSuccess=null;
	public $m_releaseCount=null;
	private static $m_instance=NULL;

	public function __construct($memberID=null){
		$this->m_userIndex=array();
		$this->m_dbInfo=array();
		$this->m_isSuccess=array();
		$this->m_releaseCount=array();
	}

	//싱글턴 얻어오기
	public static function get()
	{
	    if ( is_null( self::$m_instance ) )
	    {
	      self::$m_instance = new self();
	    }
	    return self::$m_instance;
	}

	public function begin($memberID){
		if(!$this->m_releaseCount[$memberID]){
			$this->m_releaseCount[$memberID]=1;
			
			//user db
			if($memberID!="main"){
				$this->m_userIndex[$memberID] = UserIndex::create($memberID);
				$this->m_dbInfo[$memberID] = $this->m_userIndex[$memberID]->getShardDBInfo();
			//main db
			}else{
				$this->m_dbInfo[$memberID] = DBManager::get()->getMainDBInfo();
			}

			$this->m_isSuccess[$memberID]=true;
			mysql_query("SET AUTOCOMMIT=0",$this->m_dbInfo[$memberID]->getConnection());
			mysql_query("BEGIN",$this->m_dbInfo[$memberID]->getConnection());

			LogManager::addLog("start transaction".mysql_error());
		}else{
			LogManager::addLog("start transaction but ++");
			$this->m_releaseCount[$memberID]++;
		}
	}

	public function commit($memberID){
		if(!$this->m_releaseCount[$memberID]) return false;


		LogManager::addLog("commit transaction count=".$this->m_releaseCount[$memberID]."-1");

		$this->m_releaseCount[$memberID]--;

		if(!$this->isSuccess($memberID)){
			if($this->m_releaseCount[$memberID]==0){
				$result = mysql_query("ROLLBACK", $this->m_dbInfo[$memberID]->getConnection());
				LogManager::addLog("commit query but rollback : ".mysql_error());
				mysql_query("SET AUTOCOMMIT=1",$this->m_dbInfo[$memberID]->getConnection());
			}
			return false;
		}

		if($this->m_releaseCount[$memberID]==0){
			$result = mysql_query("COMMIT", $this->m_dbInfo[$memberID]->getConnection());
			LogManager::addLog("commit query ok? : ".mysql_error());
			mysql_query("SET AUTOCOMMIT=1",$this->m_dbInfo[$memberID]->getConnection());
		}else{
			$result = true;
		}

		
		return $result;
	}

	public function rollback($memberID){
		if(!$this->m_releaseCount[$memberID]) return false;


		LogManager::addLog("rollback transaction count=".$this->m_releaseCount[$memberID]."-1");

		$this->m_releaseCount[$memberID]--;

		if($this->m_releaseCount[$memberID]==0){
			$result = mysql_query("ROLLBACK", $this->m_dbInfo[$memberID]->getConnection());
		}else{
			$this->setSuccess($memberID,false);
			$result=true;
		}
		LogManager::addLog("rollback transaction");
		return $result;
	}

	public function setSuccess($memberID,$success){
		if($this->m_isSuccess[$memberID]==false)return;

		$this->m_isSuccess[$memberID]=$success;
	}

	public function isSuccess($memberID){
		return $this->m_isSuccess[$memberID];
	}
}

class UserIndex extends DBTable{
	// public $m_memberID = null;
	// public $m_shardIndex = null;
	// public $m_socialID = 0;
	// public $m_nick = null;
	// public $m_no = null;

	public static $sharedIndexes=array();

	public static function create($memberID=null,$userindex=null,$socialID=null,$nick=null){
		
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
			LogManager::addLog("userindex load fail it's new obj is ".json_encode($newIndex->getArrayData(true))." and shardIndex is ".$newIndex->shardIndex);
			
			if(!$memberID){
				return null;
			}
			//$test = get_class_vars(get_class($newIndex));
			//LogManager::addLog("userindex load fuck!!".json_encode($test));
		}	
		return $newIndex;
	}

	public function __construct($memberID=null,$userindex=null,$socialID=null,$nick=null){

		parent::__construct();
		
		LogManager::addLog("construct userIndex for ".$memberID);

		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		
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
		 		LogManager::addLog("load success userindex shardIndex is".$this->shardIndex);
		 	}else{
				$this->memberID = $memberID;
		 		$this->shardIndex = $this->getShardIndexByNumberKey($memberID);
		 		LogManager::addLog("load fail userindex shardIndex is ".$this->shardIndex." m_shardDBCount is ".DBManager::get()->getShardDBCount());
		 		//$this->save(true);
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
	public function getShardIndexByNumberKey($numberKey){
		return DBManager::get()->getDBIndexByShardKey($numberKey);
	}

	public function getShardConnection(){
		return DBManager::get()->getConnectionByShardIndex($this->shardIndex);
	}

	public function getShardDBInfo(){
		return DBManager::get()->getDBInfoByShardIndex($this->shardIndex);
	}

	static public function getShardConnectionByRandom(){
		return DBManager::get()->getConnectionByShardKey(rand(0,10));
	}

	static public function getShardDBInfoList(){
		return DBManager::get()->getShardDBInfoList();
	}

	public function remove(){
		LogManager::addLog("remove userindex!!");
		return parent::remove();
	}


	public function save($isIncludePrimaryKey = false){
		LogManager::addLog("save userindex!!");
		return parent::save($isIncludePrimaryKey);
	}

	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class UserLog extends DBTable{
	// public $m_no = null;
	// public $m_memberID = null;
	// public $m_ip = null;
	// public $m_header = null;
	// public $m_category = null;
	// public $m_input = null;
	// public $m_output = null;
	// public $m_regDate = null;
	// public $m_regTime = null;
	// public $m_execTime = null;
	public $m__userIndex = null;
	// static public $m__queryResult = null;
	// static public $m__queryCnt = 0;
	public function __construct($memberID=null,$no=null){
		parent::__construct();
		
		$this->setPrimarykey("no");
		//$this->setDBTable(DBManager::getST("userlog"));
		$this->memberID = $memberID;
		
		// $this->setLQTableSelectQueryCustomFunction(function ($param){
		
		// 	if(!$param["where"] || !$param["where"]["category"] || $param["where"]["category"]=="")return "";
		// 	return "where category='".$param["where"]["category"]."'";
		// });
		
		$this->setLQTableSelectQueryCustomFunction(function ($param){
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

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
		}

		if($memberID && $no){
			parent::load("no=".$no." and memberID='".$memberID."'");
			// if(parent::load("no=".$no." and memberID=".$memberID)){
			// 	$this->autoMatching($this->m__result);
			// }
		}
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
	// 			self::$m__qResult=null;
	// 			self::$m__qCnt++;
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
	public function loadWithDataTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"카테고리","field"=>"category","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"IP","field"=>"ip","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"입력값","field"=>"input","viewer"=>json_decode('{"type":"custom","func":"textareaViewer"}',true));
		$data["head"][]=array("title"=>"출력값","field"=>"output","viewer"=>json_decode('{"type":"custom","func":"textareaViewer"}',true));
		$data["head"][]=array("title"=>"변경일시","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"타임스탬프","field"=>"regTime","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"실행시간","field"=>"execTime","viewer"=>json_decode('{"type":"text"}',true));
		//$data["head"][]=array("manage"=>"update delete insert");
	return $data;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SendItem extends DBTable{
	public $m__userIndex=null;
	// static public $m__queryResult = null;
	// static public $m__queryCnt = 0;
	public function __construct($memberID=null,$socialID=null,$nick=null){
		parent::__construct();
		
		$this->setPrimarykey("no");
		//$this->setDBTable(DBManager::getST("userdata"));
		$this->memberID = $memberID;
		
		$this->setLQTableSelectCustomFunction(function ($data){
			$data["memberID"] = strval($data["memberID"]);
			return $data;
		});

		if($memberID || $socialID || $nick){
			$this->m__userIndex = UserIndex::create($memberID,null,$socialID,$nick);
			//LogManager::addLog("create userindex for ".$this->m__userIndex->memberID." result is ".json_encode($this->m__userIndex->getArrayData(true)));
			if($this->m__userIndex)$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			if($this->m__userIndex && $this->m__userIndex->isLoaded()){
				if(parent::load("memberID=".$this->m__userIndex->memberID)){
					//$this->autoMatching($this->m__result);
					$this->archiveData = json_decode($this->archiveData,true);
					$this->eventCheckData = json_decode($this->eventCheckData,true);
					$this->TMInfo = json_decode($this->TMInfo,true);
					$this->TMLevel = json_decode($this->TMLevel,true);
					$this->endlessData = json_decode($this->endlessData,true);
				}
			}
		}
		

		//가입시간
		//if(!$this->joinDate)$this->joinDate=TimeManager::get()->getCurrentDateTime();
	}


	public function selectWithLQForm($param){
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

	public function updateWithLQForm($param){
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
			CommitManager::get()->begin($mID);
			
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
				CommitManager::get()->setSuccess($mID,ResultState::successCheck($sR["result"]));
			
			//바로지급
			}else{

				$param["memberID"]=$mID;
				$param["exchangeID"]=$exchange->id;
				$cmd = new commandClass();
				$sR = $cmd->exchange($param);
				CommitManager::get()->setSuccess($mID,ResultState::successCheck($sR["result"]));
			}

			if($param["comment"]){
				$mh = new ModifyHistory($mID);
				unset($p["data"][$this->getPrimaryKey()]);
				$mh->oldData="";
				$mh->newData=$exchange->list;
				$mh->comment=$param["comment"];
				$mh->category=get_called_class();
				CommitManager::get()->setSuccess($mID,$mh->save());
			}

			if(CommitManager::get()->commit($mID)){
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
class UserData extends DBTable{
	// public $m_no=null;
	// public $m_memberID=null;
	// public $m_data=null;
	// public $m_nick=null;
	// public $m_lastDate=null;
	// public $m_lastTime=null;
	// public $m_joinDate=null;
	// public $m_lastCmdNo=null;
	// public $m_deviceID=null;
	// public $m_friendList=null;
	public $m__userIndex=null;
	public static $sharedUserDatas=array();
	// static public $m__queryResult = null;
	// static public $m__queryCnt = 0;

	public static function create($memberID=null,$socialID=null,$nick=null){
		
		LogManager::addLog("create userData");
		if($memberID && self::$sharedUserDatas[$memberID]){
			LogManager::addLog("finded userData in sharedUserDatas ".$memberID);
			return self::$sharedUserDatas[$memberID];
		}


		LogManager::addLog("UserData::create, memberID is ".$memberID." and userIndex is ");
		$newUserData =new UserData($memberID,$socialID,$nick);

		if($newUserData->isLoaded()){
			LogManager::addLog("useindex load success no is".$newUserData->no);
			self::$sharedUserDatas[$newUserData->memberID]=$newUserData;
		}else{
			LogManager::addLog("userindex load fail it's new obj is ".json_encode($newUserData->getArrayData(true)));
			
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
		
		$this->setPrimarykey("no");
		//$this->setDBTable(DBManager::getST("userdata"));
		$this->memberID = $memberID;
		
		$this->setLQTableSelectCustomFunction(function ($data){
			$data["memberID"] = strval($data["memberID"]);
			return $data;
		});

		if($memberID || $socialID || $nick){
			$this->m__userIndex = UserIndex::create($memberID,null,$socialID,$nick);
			//LogManager::addLog("create userindex for ".$this->m__userIndex->memberID." result is ".json_encode($this->m__userIndex->getArrayData(true)));
			//if($this->m__userIndex->isLoaded())$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			if($this->m__userIndex && $this->m__userIndex->isLoaded()){
				$this->setDBInfo($this->m__userIndex->getShardDBInfo());
				if(parent::load("memberID='".$this->m__userIndex->memberID."'")){
					//$this->autoMatching($this->m__result);
					$this->archiveData = json_decode($this->archiveData,true);
					$this->eventCheckData = json_decode($this->eventCheckData,true);
					$this->TMInfo = json_decode($this->TMInfo,true);
					$this->TMLevel = json_decode($this->TMLevel,true);
					$this->endlessData = json_decode($this->endlessData,true);
				}
			}
		}
		

		//가입시간
		//if(!$this->joinDate)$this->joinDate=TimeManager::get()->getCurrentDateTime();
	}


	public function loadWithDataTable($p){
		$r=parent::loadWithDataTable($p);
		$r["head"][]=array("manage"=>"update delete insert");
		return $r;
	}

	public function save($isIncludePrimaryKey=false){
		//마지막접속시간
		LogManager::addLog("userdata save!!!!!!");
		$this->lastDate = TimeManager::get()->getCurrentDateTime();
		$this->lastTime = TimeManager::get()->getTime();
		return parent::save($isIncludePrimaryKey);
	}

	public function remove(){
		$r = $this->m__userIndex->remove();
		if($r)parent::remove();
		
		LogManager::addLog("dropout User query error is".mysql_error()." and ".json_encode($r));

		return $r;
	}

	public function getUserIndexNo(){
		return $this->m__userIndex->getUserIndex();
	}

	public function getUserIndex(){
		return $this->m__userIndex;
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
		
		// if($this->m__primarykey && !$isIncludePrimaryKey){
		// 	unset($arraydata[$this->m__primarykey]);
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


	public function selectWithLQForm($param){
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

		$character = new CharacterHistory($user->memberID,$user->selectedCharNO);
		LogManager::addLog("character".$user->memberID."-".$user->selectedCharNO);
		$r["characterLevel"]=$character->level;

		$storage = new UserStorage($user->memberID);
		$r["r"]='{"fr":'.$storage->fr.',"pr":'.$storage->pr.'}';
		$r["isConnecting"]=((TimeManager::get()->getTime()-$user->lastTime)<300)?"Y":"N";
		$r = array_merge($r,$storage->getArrayData());

		$result["data"]=$r;
		$result["result"]=ResultState::successToArray();
		return $result;
	}

	public function updateWithLQForm($param){
		//값저장.
		//멤버아이디 안넘어왔음 넘겨달라 에러 띄울것
		//변경내역저장
		$result["result"]=ResultState::successToArray();
		return $result;
	}


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Message extends DBTable{

	public $m__userIndex=null;
	public function __construct($memberID=null,$messageNo=null){
		parent::__construct();
		$this->setPrimarykey("no");
		//$this->setDBTable(DBManager::getST("message"));
		$this->setLQTableSelectQueryCustomFunction(function ($param){
			return "where memberID='".$param["id"]."' and isSendMsg=0";
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
		}
		
		if($memberID && $messageNo){
			if(parent::load("no = $messageNo")){

			}
		}
	}
	

	public function send(){
		if(!$this->memberID)return "error";
		if(!$this->m__DBInfo){
			$userIndex = UserIndex::create($this->m_memberID);
			$this->setDBInfo($userIndex->getShardDBInfo());
		}
		
		$result=mysql_query("insert into ".DBManager::getST("message")." (memberID,content,regDate,regTime,friendID,type,isSendMsg) values ('".$this->m_memberID."','".$this->m_content."','".$this->m_regDate."','".$this->m_regTime."','".$this->m_friendID."','".$this->m_type."','".$this->m_isSendMsg."')",$this->m__DBInfo->getConnection());
		
		if(!$result)return "error";
		
		return 0;
	}


	public function getArrayData($isIncludePrimaryKey=false,$keyList=null){
		
		$data = parent::getArrayData($isIncludePrimaryKey);

		if($data["data"]){
			$userdata =  json_decode($data["data"],true);

			foreach($userdata as $key=>$value){
				if($keyList && !in_array($name,$keyList)){
					unset($userdata[$key]);
				}
			}

			$data["data"]=json_encode($userdata,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		}	
		return $data;

	}


}
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
		
		
// 		$this->setPrimarykey("no");
// 		//$this->setDBTable(DBManager::getST("stageScore"));
// 		$this->setDBInfo(DBManager::get()->getDBInfoByShardKey($stageNo));
		
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
class Puzzle extends DBTable{

	public function __construct($puzzleNo=null,$order=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		

		if($puzzleNo)$this->no=$puzzleNo;
		if($order)$this->order=$order;

		if($puzzleNo || $order){
			$query = "`order`=".$order;
			if($puzzleNo)$query = "no=".$puzzleNo;

			if(parent::load($query)){
				$this->center = json_decode($this->center,true);
				$this->face = json_decode($this->face,true);
				$this->original = json_decode($this->original,true);
				$this->thumbnail = json_decode($this->thumbnail,true);
				$this->map = json_decode($this->map,true);
				$this->pathInfo = json_decode($this->pathInfo,true);
				$this->coordinateInfo = json_decode($this->coordinateInfo,true);
				$this->startPosition = json_decode($this->startPosition,true);
				$this->endPosition = json_decode($this->endPosition,true);
				$this->color = json_decode($this->color,true);
				$this->clearReward = json_decode($this->clearReward,true);
				$this->title = json_decode($this->title,true);
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

	public function loadWithDataTable($p){
		$intEditor = array("type"=>"text","datatype"=>"int");
		$textViewer = array("type"=>"text");
		$textareaEditor = array("type"=>"textarea");
		$dictEditor = array("type"=>"dictionary");
		$osEditor = json_decode('{"type":"select","element":["all","android","ios"]}',true);
		$imgEditor = json_decode('{"type":"dictionary","element":[{"field":"image","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}',true);
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
		$imgViewer = json_decode('{"type":"custom","func":"showPuzzleImg"}',true);

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


	public function updateWithLQTable($p){
		reloadPuzzleInfo();
		$p["data"]["version"]+=1;
		$r = parent::updateWithLQTable($p);
		kvManager::increase("puzzleListVer");

		return $r;
	}

	public function insertWithLQTable($p){
		reloadPuzzleInfo();
		$r = parent::insertWithLQTable($p);
		kvManager::increase("puzzleListVer");
		return $r;
	}


}


////////////////////////////////////////////////////////////////////////////////////////
//	피스정보
////////////////////////////////////////////////////////////////////////////////////////
class Piece extends DBTable{

	public function __construct($no=null,$order=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		
		if($no)$this->no=$no;

		if($no || $order){
			$query="";
			if($no)$query = "no=".$no;
			if($order)$query = "`order`=".$order;
			parent::load($query);
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
		for($i=0;$i<count($stageInfo[cards]);$i++){
			$myCard = new Card($stageInfo[cards][$i]);
			if($myCard->isLoaded()){
				$card[]= $myCard->getArrayDataForClient();
			}
		}
		$stageInfo[cards]=$card;
		return $stageInfo;
	}

	public function updateWithLQTable($p){
		CommitManager::get()->begin("main");
		if($p["data"]["puzzle"]){
			$puzzle = new Puzzle($p["data"]["puzzle"]);
			$puzzle->version+=1;
			kvManager::increase("puzzleListVer");
			CommitManager::get()->setSuccess("main",$puzzle->save());
		}

		$p["data"]["version"]+=1;
		$r = parent::updateWithLQTable($p);
		CommitManager::get()->setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::get()->commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
		}
	}

	public function insertWithLQTable($p){
		CommitManager::get()->begin("main");
		if($p["data"]["puzzle"]){
			$puzzle = new Puzzle($p["data"]["puzzle"]);
			$puzzle->version+=1;
			kvManager::increase("puzzleListVer");
			CommitManager::get()->setSuccess("main",$puzzle->save());
		}

		//$p["data"]["version"]+=1;
		$r = parent::updateWithLQTable($p);
		CommitManager::get()->setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::get()->commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
		}
	}

	public function loadWithDataTable($p){

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
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//	카드정보
////////////////////////////////////////////////////////////////////////////////////////
class Card extends DBTable{


	public function __construct($cardNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		

		if($cardNo)$this->no=$cardNo;

		if($cardNo){
			$query = "no=".$cardNo;
			if(parent::load($query)){
				$this->ability = json_decode($this->ability,true);
				$this->missile = json_decode($this->missile,true);
				$this->imgInfo = json_decode($this->imgInfo,true);
				$this->aniInfo = json_decode($this->aniInfo,true);
				$this->thumbnailInfo = json_decode($this->thumbnailInfo,true);
				$this->silImgInfo = json_decode($this->silImgInfo,true);
				$this->mPrice = json_decode($this->mPrice,true);
				$this->name = json_decode($this->name,true);
				$this->script = json_decode($this->script,true);
			}
		}
	}

	public function getArrayDataForClient(){
		$cardInfo = $this->getArrayData(true);
		$cardInfo[stage]=$stageInfo[no];
		

		
		$cardInfo["name"] = $this->getLocalizedValue("name");
		$cardInfo["script"] = $this->getLocalizedValue("script");
		return $cardInfo;
	}

	public function updateWithLQTable($p){
		CommitManager::get()->begin("main");
		
		if($p["data"]["piece"]){
			$piece = new Piece($p["data"]["piece"]);
			$piece->version+=1;
			CommitManager::get()->setSuccess("main",$piece->save());

			if($piece->puzzle){
				$puzzle = new Puzzle($piece->puzzle);
				$puzzle->version+=1;
				kvManager::increase("puzzleListVer");
				CommitManager::get()->setSuccess("main",$puzzle->save());
			}
		}

		$r = parent::updateWithLQTable($p);
		CommitManager::get()->setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::get()->commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
		}

	}

	public function insertWithLQTable($p){
		CommitManager::get()->begin("main");
		
		if($p["data"]["piece"]){
			$piece = new Piece($p["data"]["piece"]);
			$piece->version+=1;
			CommitManager::get()->setSuccess("main",$piece->save());

			if($piece->puzzle){
				$puzzle = new Puzzle($piece->puzzle);
				$puzzle->version+=1;
				kvManager::increase("puzzleListVer");
				CommitManager::get()->setSuccess("main",$puzzle->save());
			}
		}

		$r = parent::insertWithLQTable($p);
		CommitManager::get()->setSuccess("main",ResultState::successCheck($r["result"]));

		if(CommitManager::get()->commit("main")){
			return $r;
		}else{
			return ResultState::makeReturn(ResultState::GDDONTSAVE,"저장에러!!");
		}
	}

	public function loadWithDataTable($p){




		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"rank","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"grade","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"name","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true));
		$data["head"][]=array("field"=>"reward","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"durability","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"ability","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"table","element":[{"title":"아이템번호","field":"type","type":"text","datatype":"int"},{"title":"옵션","field":"option","type":"dictionary"}]}',true));
		$data["head"][]=array("field"=>"passive","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":[
					{"title":"operator","field":"operator","type":"select","element":["-","*(1-x)"]},	
					{
						"title":"패턴",
						"field":"pattern",
						"type":"dictionary",
						"element":[
							{"field":"castframe","type":"text","datatype":"int"},
							{"field":"totalframe","type":"text","datatype":"int"},
							{"field":"speed","type":"text","datatype":"int"}
						]			
					},			
							
					{   
					    "title":"속도",
						"field":"speed","type":"text","datatype":"int"
					},
					{   
					    "title":"스케일",
						"field":"scale","type":"text","datatype":"int"
					},
					{"title":"공격주기","field":"attackpercent","type":"text","datatype":"int"},
					{"title":"에너지","field":"hp","type":"text","datatype":"int"},
					{"title":"민첩","field":"agi","type":"text","datatype":"int"}	,
					{"title":"AI","field":"ai","type":"text","datatype":"int"}				
					]}',true));
		$data["head"][]=array("field"=>"missile","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"speed","type":"text","datatype":"int"},{"field":"power","type":"text","datatype":"int"},{"field":"dex","type":"text","datatype":"int"}]}',true));
		$data["head"][]=array("field"=>"cc","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["kr","jp","en","cn"]}',true));
		$data["head"][]=array("field"=>"stage","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"piece","viewer"=>json_decode('{"type":"text"}',true),'always');
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"imgInfo","viewer"=>json_decode('{"type":"custom","func":"showCardImg"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"field":"img","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}',true));
		$data["head"][]=array("field"=>"aniInfo","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[
						{"field":"isAni","type":"bool"},
						{"field":"detail",
						 "type":"dictionary",
						 "element":[
										{"field":"img","type":"custom","func":"imageSelector"},
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
		$data["head"][]=array("field"=>"silImgInfo","viewer"=>json_decode('{"type":"text","cut":50}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"field":"isSil","type":"bool"},{"field":"img","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}',true));
		$data["head"][]=array("field"=>"mPrice","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary","element":[{"field":"r","type":"text","datatype":"int"},{"field":"p6","type":"text","datatype":"int"}]}',true));
		$data["head"][]=array("field"=>"category","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["normal","nPuzzle","special","event","ePuzzle","gift","leader"]}',true));
		$data["head"][]=array("field"=>"type","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["음","양","섬"],"value":[1,2,3]}',true));
		$data["head"][]=array("field"=>"level","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"characterNo","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text","datatype":"int"}',true));
		$data["head"][]=array("field"=>"sound","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"text"}}',true));
		$data["head"][]=array("manage"=>"[delete,update,insert]");

		return $data;
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//	카드히스토리
////////////////////////////////////////////////////////////////////////////////////////

class CardHistory extends DBTable{
	public $m__userIndex;
	public function __construct($memberID=null,$cardNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		$this->setLQTableSelectCustomFunction(function ($rData){
	    	$cardInfo = new Card($rData["cardNo"]);
	    	$pieceInfo = new Piece($cardInfo->piece);
	    	$puzzleInfo = new Puzzle($pieceInfo->puzzle);

	    	$rData["puzzleNo"]=$puzzleInfo->no;
			return $rData;
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID = $memberID;
		}


		if($cardNo)$this->cardNo=$cardNo;

		if($memberID && $cardNo){
			parent::load("memberID=".$memberID." and cardNo=".$cardNo);
		}
	}
	public function loadWithDataTable($p){

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
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"카드","field"=>"cardNo","viewer"=>$listViewer);
		$data["head"][]=array("title"=>"퍼즐","field"=>"puzzleNo","viewer"=>$listViewer2);
		$data["head"][]=array("title"=>"코멘트","field"=>"comment","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"획득일시","field"=>"takeDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"모핑여부","field"=>"isMorphing","viewer"=>json_decode('{"type":"bool"}',true),"editor"=>json_decode('{"type":"bool"}',true));
		$data["head"][]=array("manage"=>"update delete");
		
		return $data;
	}
	public function updateWithLQTable($p){
		unset($p["data"]["puzzleNo"]);
		$r = parent::updateWithLQTable($p);

		if($p["comment"]){
			$mh = new ModifyHistory($r["data"]["memberID"]);
			unset($p["data"][$this->getPrimaryKey()]);
			$mh->oldData=$p["oldData"];
			$mh->newData=$p["data"];
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
	}

	public function deleteWithLQTable($p){
		$r = parent::deleteWithLQTable($p);
		if($p["comment"]){
			$mh = new ModifyHistory($p["data"]["memberID"]);
			unset($p["data"][$this->getPrimaryKey()]);
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

class PuzzleHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$puzzleNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		// $this->setLQTableSelectCustomFunction(function ($data){
		// 	return $data;
		// });


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID = $memberID;
		}

		if($puzzleNo)$this->puzzleNo=$puzzleNo;

		if($memberID && $puzzleNo){
			parent::load("memberID=".$memberID." and puzzleNo=".$puzzleNo);
		}
	}

	public function loadWithDataTable($p){

		$puzzleViewer=array("type"=>"select");
		while($pData = Puzzle::getRowByQuery("",null,"title,no")){
			$lang = json_decode($pData["title"],true);
			$puzzleViewer["element"][] = $pData["no"]."-".$lang["ko"];
			$puzzleViewer["value"][]=$pData["no"];
		}

		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"퍼즐정보","field"=>"puzzleNo","viewer"=>$puzzleViewer,"editor"=>$puzzleViewer);
		$data["head"][]=array("title"=>"오픈유형","field"=>"openType","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"최초오픈일시","field"=>"openDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"최초완성일시","field"=>"clearDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"퍼펙트완성일시","field"=>"perfectDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"보상","field"=>"reward","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("manage"=>"update delete");		
		return $data;
	}

	public function updateWithLQTable($p){
		
		unset($p["data"]["reward"]);
		$r = parent::updateWithLQTable($p);

		if($p["comment"]){
			$mh = new ModifyHistory($r["data"]["memberID"]);
			unset($p["data"][$this->getPrimaryKey()]);
			$mh->oldData=$p["oldData"];
			$mh->newData=$p["data"];
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
	}

	public function deleteWithLQTable($p){
		$r = parent::deleteWithLQTable($p);
		if($p["comment"]){
			$mh = new ModifyHistory($p["data"]["memberID"]);
			unset($p["data"][$this->getPrimaryKey()]);
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

class PieceHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$pieceNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID = $memberID;
		}

		if($pieceNo)$this->pieceNo=$pieceNo;

		if($memberID && $pieceNo){
			parent::load("memberID=".$memberID." and pieceNo=".$pieceNo);

			if($this->clearDateList)$this->clearDateList = json_decode($this->clearDateList,true); 
		}
	}

	public function loadWithDataTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"스테이지번호","field"=>"pieceNo","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"최초진입일시","field"=>"openDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"최초클리어일시","field"=>"firstClearDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"플레이횟수","field"=>"tryCount","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"진행상태","field"=>"clearDateList","viewer"=>json_decode('{"type":"custom","func":"clearList"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"datetime"}}',true));
		$data["head"][]=array("title"=>"보상","field"=>"reward","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("manage"=>"update delete");
		return $data;
	}

	public function updateWithLQTable($p){
		
		unset($p["data"]["reward"]);
		$r = parent::updateWithLQTable($p);

		if($p["comment"]){
			$mh = new ModifyHistory($r["data"]["memberID"]);
			unset($p["data"][$this->getPrimaryKey()]);
			$mh->oldData=$p["oldData"];
			$mh->newData=$p["data"];
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
	}

	public function deleteWithLQTable($p){
		$r = parent::deleteWithLQTable($p);
		if($p["comment"]){
			$mh = new ModifyHistory($p["data"]["memberID"]);
			unset($p["data"][$this->getPrimaryKey()]);
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
class FaultyNick extends DBTable{

	public function __construct($no=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		

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
class Archivement extends DBTable{



	public function __construct($id=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		
		$this->setLQTableSelectCustomFunction(function ($data){
			if($data["exchangeID"]){
				$exchange = new Exchange($data["exchangeID"]);
				$data["exchangeList"]=$exchange->list;				
			}
			return $data;
		});

		if($id)$this->id=$id;

		if($id){
			$query = "`id`=".$id;
			if(parent::load($query)){
				$this->reward = json_decode($this->reward,true);
			}
		}
	}

	public function updateWithLQTable($p){
		

		$exchange=null;
		if($p["data"]["exchangeID"]){
				$exchange = new Exchange($p["data"]["exchangeID"]);
				if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
				if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
				$exchange->save();		
		}
		unset($p["data"]["exchangeList"]);
		$r = parent::updateWithLQTable($p);

		if($exchange){
			$r["data"]["exchangeList"]=$exchange->list;
		}

		kvManager::increase("arcListVer");

		return $r;
	}

	public function insertWithLQTable($p){
		$exchange=null;
		if($p["data"]["exchangeID"]){
				$exchange = new Exchange($p["data"]["exchangeID"]);
				if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
				if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
				$exchange->save();		
		}
		unset($p["data"]["exchangeList"]);
		$r = parent::insertWithLQTable($p);

		if($exchange){
			$r["data"]["exchangeList"]=$exchange->list;
		}
		kvManager::increase("arcListVer");

		return $r;
	}


	public function loadWithDataTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$dictEditor = array("type"=>"dictionary");
		$intEditor = json_decode('{"type":"text","datatype":"int"}',true);
		$typeEditor = json_decode('{"type":"select","element":["gold","ruby","heart"],"value":["g","r","h"]}',true);
		$countNameEditor = json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true);
		$exchangeListEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		$rewardEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"count","type":"text","datatype":"int"}]}}',true);
		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"editor"=>$textEditor,"primary");
		$data["head"][]=array("field"=>"groupNo","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("field"=>"id","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"category","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"title","viewer"=>$textViewer,"editor"=>$dictEditor);
		$data["head"][]=array("field"=>"content","viewer"=>$textViewer,"editor"=>$dictEditor);
		$data["head"][]=array("field"=>"reward","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"goal","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"exchangeID","viewer"=>$textViewer,"editor"=>$textEditor,"always");
		$data["head"][]=array("field"=>"exchangeListEditor","viewer"=>$textViewer,"editor"=>$textEditor);
		


		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//	업적히스토리
////////////////////////////////////////////////////////////////////////////////////////

class ArchivementHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		$this->setLQTableSelectCustomFunction(function($rData){
			if($rData["rewardDate"]>0){
				$archive = new Archivement($rData["archiveID"]);
				$rData["reward"]=$archive->reward;
			}
			return $rData;
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID = $memberID;
		}

		if($fNo)$this->archiveID=$fNo;

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and archiveID='".$fNo."'");
		}
	}

	public function loadWithDataTable($p){

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
		$data["head"][]=array("title"=>"보상","field"=>"reward","viewer"=>json_decode('{"type":"custom","func":"rewardViewer"}',true),"virtual");
		$data["head"][]=array("manage"=>"update delete");
		return $data;
	}
}



////////////////////////////////////////////////////////////////////////////////////////
//	선물상자

////////////////////////////////////////////////////////////////////////////////////////
class GiftBoxHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		$this->setLQTableSelectCustomFunction(function ($rData){
			if($rData["reward"])$rData["reward"] = json_decode($rData["reward"],true);
			// if($rData["exchangeID"]){
			// 	$exchange = new Exchange($rData["exchangeID"]);
			// 	$rData["exchangeList"]=$exchange->list;				
			// }
			return $rData;
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->setField("memberID",$memberID);
		}

		if($memberID && $fNo){
			if(parent::load("memberID=".$memberID." and no=".$fNo)){
				$this->reward = json_decode($this->reward,true);
			}
		}
	}

	public function getAllExchangeID(){
		$elist=array();
		while($rData = $this->getRowByQuery("where memberID='".$this->memberID."' and confirmDate=0",$this->getDBConnection())){
			if($rData["exchangeID"])$elist[]=$rData["exchangeID"];
		}

		return $elist;
	}

	public function confirmAll(){
		$lastDay = TimeManager::get()->getDateTime(TimeManager::get()->getTime()-60*60*24*30);
		$result = mysql_query("update ".$this->getDBTable()." set confirmDate='".TimeManager::get()->getCurrentDateTime()."' where memberID='".$this->memberID."' and confirmDate=0 and regDate>$lastDay",$this->getDBConnection());
		return $result;
	}

	// public function updateWithLQTable($p){
	// 	$exchange=null;
	// 	// if($p["data"]["exchangeID"]){
	// 	// 	$exchange = new Exchange($p["data"]["exchangeID"]);
	// 	// }
		
	// 	//unset($p["data"]["exchangeList"]);
		
	// 	$r = parent::updateWithLQTable($p);
		
	// 	//if($exchange)$r["data"]["exchangeList"]=$exchange->list;

	// 	return $r;
	// }

	// public function insertWithLQTable($p){
	// 	$exchange=null;
	// 	// if($p["data"]["exchangeID"]){
	// 	// 	$exchange = new Exchange($p["data"]["exchangeID"]);
	// 	// }
		
	// 	//unset($p["data"]["exchangeList"]);

	// 	$r = parent::insertWithLQTable($p);

	// 	//if($exchange)$r["data"]["exchangeList"]=$exchange->list;

	// 	return $r;
	// }

	public function loadWithDataTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"회원번호","field"=>"memberID","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"받은일시","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"확인일시","field"=>"confirmDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		$data["head"][]=array("title"=>"보낸사람","field"=>"sender","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"내용","field"=>"content","viewer"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"보상","field"=>"reward","viewer"=>json_decode('{"type":"custom","func":"rewardViewer"}',true),"virtual");
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

class UserProperty extends DBTable{
	const TypeGold = "g";
	const TypeRuby = "r";
	const TypeFreeRuby = "fr";
	const TypePurchaseRuby = "pr";
	const TypeHeart = "h";
	const TypeItem6 = "i6";
	const TypeItem9 = "i9";
	const TypeItem8 = "i8";

	public $m__userIndex;

	public function __construct($memberID=null,$type=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			return "where memberID='".$param["id"]."'";
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

		if($memberID && $type){
			parent::load("memberID=".$memberID." and `type`='".$type."'");
		}
	}

}


////////////////////////////////////////////////////////////////////////////////////////
//	유저보관함
////////////////////////////////////////////////////////////////////////////////////////

class UserStorage extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

		if($memberID){
			parent::load("memberID=".$memberID);
		}
	}

	public function loadWithDataTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"editor"=>$textEditor,"primary");
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
class UserPropertyHistory extends DBTable{
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

	public $m__userIndex;

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectQueryCustomFunction(function ($param){
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

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and `no`=".$fNo);
		}
	}

	public function loadWithDataTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"종류","field"=>"type","viewer"=>json_decode('{"type":"custom","func":"propChange"}',true));
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
class LoginEvent extends DBTable{
	
	public function __construct($fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::get()->getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($r["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}


			
			return $rData;
		});

		$this->setDBInfo(DBManager::get()->getMainDBInfo());

		if($fNo){
			parent::load("no=".$fNo);
		}
	}
	static public function getRewardDays(){
		$data=array();
		while($rData = LoginEvent::getRowByQuery("where endDate>".TimeManager::get()->getCurrentDateTime()." and startTime=0 and endTime=235959 limit 4")){
			$rData["reward"]=json_decode($rData["reward"],true);
			$data[]=$rData;
		}
		return $data;
	}
	public function updateWithLQTable($p){
		
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

	public function insertWithLQTable($p){
		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
			if($exchange->isLoaded())$p["data"]["reward"]=$exchange->list;
		}
		$r = parent::insertWithLQTable($p);
		return $r;
	}

	public function loadWithDataTable($p){
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
		$data["head"][]=array("title"=>"보상ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"로그인이벤트","statsID":"loginEvent","statsValueField":"no"}',true));
		$data["head"][]=array("title"=>"보상내용","field"=>"reward","viewer"=>json_decode('{"type":"rewardViewer"}',true));
		$data["head"][]=array("manage"=>"insert delete update");
		return $data;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	출석이벤트
////////////////////////////////////////////////////////////////////////////////////////
class AttendenceEvent extends DBTable{
	
	public function __construct($fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		$this->setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::get()->getCurrentDateTime();
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
			$today = TimeManager::get()->getCurrentDateTime();
			$query = "startDate<=".$today." and endDate>=".$today;
		}
		if(parent::load($query)){
			$this->rewardList = json_decode($this->rewardList,true);
			$this->exchangeIDList = json_decode($this->exchangeIDList,true);
		}
		
	}

	public function loadWithDataTable($p){
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("title"=>"진행상태","field"=>"state","viewer"=>json_decode('{"type":"text"}',true),"virtual");
		$data["head"][]=array("title"=>"제목","field"=>"title","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"운영체제","field"=>"os","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["all","android","ios"]}',true));
		$data["head"][]=array("title"=>"국가","field"=>"cc","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("title"=>"시작일시","field"=>"startDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"종료일시","field"=>"endDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"보상목록","field"=>"rewardList","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"type","field":"type"}]}}',true));
		$data["head"][]=array("title"=>"보상ID목록","field"=>"exchangeIDList","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"array","element":{"type":"text"}}',true));
		$data["head"][]=array("manage"=>"insert delete update");
		return $data;
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//	미션이벤트
////////////////////////////////////////////////////////////////////////////////////////
class MissionEvent extends DBTable{
	
	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setDBInfo(DBManager::get()->getMainDBInfo());

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and no=".$fNo);
		}
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//	쿠폰메니저
////////////////////////////////////////////////////////////////////////////////////////
class CuponManager extends DBTable{
	
	public function __construct($fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setDBInfo(DBManager::get()->getMainDBInfo());

		$this->setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::get()->getCurrentDateTime();
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
		// $this->setLQTableSelectCustomFunction(function ($data){
		// 	if($data["exchangeID"]){
		// 		$exchange = new Exchange($data["exchangeID"]);
		// 		$data["exchangeList"]=$exchange->list;				
		// 	}
		// 	return $data;
		// });

		if($fNo){
			if(parent::load("no=".$fNo)){
				$this->reward=json_decode($this->reward,true);
			}
		}
	}


	public function loadWithDataTable($p){
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
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"로그인이벤트","statsID":"loginEvent","statsValueField":"no"}',true));
		$data["head"][]=array("title"=>"교환내용","field"=>"reward","viewer"=>json_decode('{"type":"rewardViewer"}',true));
		$data["head"][]=array("title"=>"작성자","field"=>"writer","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"생성일","field"=>"regDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true));
		//$data["head"][]=array("title"=>"중복사용","field"=>"exchangeList","viewer"=>$textViewer,"editor"=>$exchangeEditor);

		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}

	public function updateWithLQTable($p){
		
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

	public function insertWithLQTable($p){
		// $exchange=null;
		// if($p["data"]["exchangeID"]){
		// 		$exchange = new Exchange($p["data"]["exchangeID"]);
		// 		if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
		// 		if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
		// 		$exchange->save();		
		// }
		// unset($p["data"]["exchangeList"]);
		// $p["data"]["regDate"]=TimeManager::get()->getCurrentDateTime();

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
class CuponCode extends DBTable{
	
	public function __construct($fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		
		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"] && $param["where"]["type"]=="cuponNo"){
				return "where cuponNo=".$param["where"]["id"];
			}else if ($param["where"]["id"] && $param["where"]["type"]=="cuponCode"){
				return "where cuponCode='".$param["where"]["id"]."'";
			}

			return "";

			// return "`cuponCode`='".$param["data"]["cuponCode"]."'";
		});

		$this->setLQTableSelectCustomFunction(function($rData){
			$cuponInfo = new CuponManager($rData["cuponNo"]);
				
			if($cuponInfo->isLoaded()){
				if($cuponInfo->isCommon){
					$rData["isUsed"]="중복가능";
				}else{
					$cuponUsedInfo = new CuponUsedInfo($rData["cuponCode"],$rData["serverNo"]);
					

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
			parent::load("cuponCode='".$fNo."'");
		}
	}

	static public function getCuponCount($cuponNo){
		$query = mysql_query("select count(*) from ".DBManager::getMT(get_called_class())." where cuponNo=".$cuponNo);
		$row = mysql_fetch_array($query);
		return $row[0];
	}

	public function getCuponList($param){
		$data = array();

		while($rData = CuponCode::getRowByQuery("where cuponNo=".$param["cuponNo"])){
			$data[]=$rData;
		}

		$r["list"]=$data;
		$r["result"]=ResultState::successToArray();
		return $r;
	}


	public function loadWithDataTable($p){
		$listViewer=array("type"=>"select");
		while($pData = CuponManager::getRowByQuery("",null,"title,no")){
			$listViewer["element"][] = $pData["no"]."-".$pData["title"];
			$listViewer["value"][]=$pData["no"];
		}

		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$codeEditor = array("type"=>"array","element"=>array("type"=>"text"));
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"쿠폰번호","field"=>"cuponNo","viewer"=>$textViewer,"editor"=>$listViewer);
		$data["head"][]=array("title"=>"쿠폰코드","field"=>"cuponCode","viewer"=>json_decode('{"type":"cuponCodeViewer"}',true),"editor"=>json_decode('{"type":"cuponMaker"}',true));
		$data["head"][]=array("title"=>"관리서버","field"=>"serverNo","viewer"=>$textViewer);
		$data["head"][]=array("title"=>"사용여부","field"=>"isUsed","viewer"=>$textViewer,"virtual");
		$data["head"][]=array("title"=>"사용일시","field"=>"usedDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"virtual");
		$data["head"][]=array("title"=>"사용유저","field"=>"memberID","viewer"=>$textViewer,"virtual");
		$data["head"][]=array("manage"=>"delete insert");
		
		return $data;
	}

	public function updateWithLQTable($p){
		//$r = parent::updateWithLQTable($p);
		$r["result"]=ResultState::toArray(ResultState::GDUNKNOWNRESULT,"업데이트할수없습니다.");
		return $r;
	}

	public function insertWithLQTable($p){
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
		CommitManager::get()->begin("main");
		for($i=0;$i<$cnt;$i++){

			LogManager::addLog("test1 ".$i);
			
			$p2=unserialize(serialize($p));
			if($mode=="manual")	$p2["data"]["cuponCode"]=$codeList[$i];
			else $p2["data"]["cuponCode"] = CuponCode::getRandomString(12);

			$p2["data"]["serverNo"]=DBManager::get()->getDBIndexByShardString($p2["data"]["cuponCode"]);
			
			$this->setLoaded(false);
			$ri = parent::insertWithLQTable($p2);
			if(ResultState::successCheck($ri["result"])){
				$r["data"][]=$ri["data"];	
			}else{
				CommitManager::get()->rollback("main");
				return ResultState::makeReturn(ResultState::GDDONTSAVE,"코드가중복되었습니다.(".$p2["data"]["cuponCode"].")");	
			}
					
		}

		if(CommitManager::get()->commit("main")){
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
class CuponHistory extends DBTable{
	public $m__userIndex;

	public function __construct($code=null,$memberID=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";

			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		$this->setLQTableSelectCustomFunction(function($rData){
			$cuponInfo = new CuponManager($rData["cuponNo"]);
			$rData["cuponTitle"]=$cuponInfo->title;
			return $rData;
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}
		if($code && $memberID){
			parent::load("memberID='".$memberID."' and cuponCode='".$code."'");
		}
	}



	public function loadWithDataTable($p){
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
class CuponUsedInfo extends DBTable{
	public function __construct($code=null,$shardIndex=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		if($shardIndex){
			$this->setDBInfo(DBManager::get()->getDBInfoByShardIndex($shardIndex));
		}
		$this->cuponCode=$code;
		if($code){
			parent::load("cuponCode='".$code."'");
		}
	}

}

///////////////////////////////////////////////////////////////////////////////////////


class WeeklyScore extends DBTable{

	public $m__userIndex;

	public function __construct($memberID=null,$weekNo=null){
		parent::__construct();
		
		$this->setPrimarykey("no");
		//$this->setDBTable(DBManager::getST("weeklyScore"));
		LogManager::addLog("weeklyScore is ".DBManager::getST("weeklyScore"));


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
		}
		
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
		while($result = WeeklyScore::getQueryResult("select count(*) from ".DBManager::getST("weeklyscore")." where regWeek=".$this->regWeek)){
			$data = mysql_fetch_array($result);
			$alluser+=$data[0];
		}

		return $alluser;
	}

	public function getMyRank(){
		if($this->score<=0)return -1;
		
		$alluser=0;
		while($result = WeeklyScore::getQueryResult("select count(*) from ".DBManager::getST("weeklyscore")." where regWeek=".$this->regWeek." and score>".$this->score)){			
			$data =mysql_fetch_array($result);
			$alluser+=$data[0];
		}
		
		return $alluser+1;
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//	스테이지랭킹
////////////////////////////////////////////////////////////////////////////////////////
class StageScore extends DBTable{
	

	public $m__userIndex;

	public function __construct($memberID=null,$stageNo=null,$where=null){
		
		if(!$stageNo)return false;
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);
		$this->stageNo=$stageNo;

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}
		
		if($memberID || $where){
			if($where)$q_where = $where;
			else $q_where = "memberID=".$memberID." and stageNo=".$stageNo;
			

			LogManager::addLog("stage score construct ".$q_where);
			parent::load($q_where);
		}
	}
	public function save($p=null){
		if(!$this->regDate){
			$this->regDate=TimeManager::get()->getCurrentDateTime();
			$this->regDate=TimeManager::get()->getTime();
		}
		return parent::save($p);
	}

	public function getTop4(){
		// $topquery = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo." order by score desc limit 4",$this->getDBConnection());
		
		// LogManager::addLog("select * from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo." order by score desc limit 4");
		// $rank=1;
		// $rdata = array(); 
		// while($data = mysql_fetch_assoc($topquery)){
		// 	$data["rank"]=$rank++;
		// 	$rdata[]=$data;
		// }

		// return $rdata;
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
		// $mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo,$this->getDBConnection()));
		// $alluser=$mresult[0];
		// return $alluser;

		$alluser=0;
		while($result = StageScore::getQueryResult("select count(*) from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo)){
			$data = mysql_fetch_array($result);
			$alluser+=$data[0];
		}

		return $alluser;
	}

	public function getMyRank(){
		// if($this->score<=0)return -1;
		// $mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo." and score>".$this->score,$this->getDBConnection()));
		// $myrank=$mresult[0]+1;
		// return $myrank;

		if($this->score<=0)return -1;
		
		$alluser=0;
		while($result = StageScore::getQueryResult("select count(*) from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo." and score>".$this->score)){			
			$data =mysql_fetch_array($result);
			$alluser+=$data[0];
		}
		
		return $alluser+1;
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터
////////////////////////////////////////////////////////////////////////////////////////
class Character extends DBTable{
	
	public function __construct($characterNo=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);


		$this->setDBInfo(DBManager::get()->getMainDBInfo());

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
	public function updateWithLQTable($p){
		$r = parent::updateWithLQTable($p);
		kvManager::increase("charListVer");
		return $r;
	}

	public function insertWithLQTable($p){
		$r = parent::insertWithLQTable($p);
		kvManager::increase("charListVer");
		return $r;
	}

	public function loadWithDataTable($p){
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
						{"field":"ccbi","type":"custom","func":"resourceSelector"},
						{"field":"imageID","type":"text"},	
						{"field":"plist","type":"custom","func":"resourceSelector"},
						{"field":"pvrccz","type":"custom","func":"resourceSelector"},
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
class CharacterHistory extends DBTable{
	public $m__userIndex = null;
	public function __construct($memberID=null,$characterNo=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);


		$this->setLQTableSelectQueryCustomFunction(function ($param){
			LogManager::addLog("--->".json_encode($param));
			if($param["where"]["id"]=="*")return "";
			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}
		
		if($memberID && $characterNo){
			$q_where = "memberID=".$memberID." and characterNo=".$characterNo;
			if(parent::load($q_where)){
				
			}
		}
	}

	public function loadWithDataTable($p){
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

	public function updateWithLQTable($p){
		
		unset($p["data"]["name"]);
		$r = parent::updateWithLQTable($p);

		if($p["comment"]){
			$mh = new ModifyHistory($r["data"]["memberID"]);
			unset($p["data"][$this->getPrimaryKey()]);
			$mh->oldData=$p["oldData"];
			$mh->newData=$p["data"];
			$mh->comment=$p["comment"];
			$mh->category=get_called_class();
			$mh->save();
		}
		return $r;
	}

	public function deleteWithLQTable($p){
		$r = parent::deleteWithLQTable($p);
		if($p["comment"]){
			$mh = new ModifyHistory($p["data"]["memberID"]);
			unset($p["data"][$this->getPrimaryKey()]);
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

class Notice extends DBTable{
	
	public function __construct($no=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);


		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		$this->setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::get()->getCurrentDateTime();
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
				$this->banner = json_decode($this->banner,true);
				$this->imgInfo = json_decode($this->imgInfo,true);
			}
		}
	}


	public function loadWithDataTable($p){
		$intEditor = array("type"=>"text","datatype"=>"int");
		$textViewer = array("type"=>"text");
		$textareaEditor = array("type"=>"textarea");
		$dictEditor = array("type"=>"dictionary");
		$osEditor = json_decode('{"type":"select","element":["all","android","ios"]}',true);
		$imgEditor = json_decode('{"type":"dictionary","element":[{"field":"img","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}',true);
		

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


class Shop extends DBTable{
	public function __construct($pID=null,$exchangeID=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		
		$this->setLQTableSelectCustomFunction(function ($data){
			if($data["exchangeID"]){
				$exchange = new Exchange($data["exchangeID"]);
				$data["exchangeList"]=$exchange->list;				
			}
			return $data;
		});

		if($pID || $exchangeID){
			if($pID){
				$q_where = "pID='".$pID."'";
			}else if($exchangeID){
				$q_where = "exchangeID='".$exchangeID."'";
			}	
			
			if(parent::load($q_where)){
				$this->priceName = json_decode($this->priceName,ture);
				$this->countName = json_decode($this->countName,ture);
				$this->data = json_decode($this->data,ture);
			}
			
		}
	}

	public function updateWithLQTable($p){
		
		$exchange=null;
		if($p["data"]["exchangeID"]){
				$exchange = new Exchange($p["data"]["exchangeID"]);
				if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
				if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
				$exchange->save();		
		}
		unset($p["data"]["exchangeList"]);
		$r = parent::updateWithLQTable($p);

		if($exchange){
			$r["data"]["exchangeList"]=$exchange->list;
		}

		kvManager::increase("shopListVer");

		return $r;
	}

	public function insertWithLQTable($p){
		$exchange=null;
		if($p["data"]["exchangeID"]){
				$exchange = new Exchange($p["data"]["exchangeID"]);
				if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
				if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
				$exchange->save();		
		}
		unset($p["data"]["exchangeList"]);
		$r = parent::insertWithLQTable($p);

		if($exchange){
			$r["data"]["exchangeList"]=$exchange->list;
		}


		kvManager::increase("shopListVer");

		return $r;
	}


	public function loadWithDataTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$dictEditor = array("type"=>"dictionary");
		$intEditor = json_decode('{"type":"text","datatype":"int"}',true);
		$typeEditor = json_decode('{"type":"select","element":["gold","ruby","heart"],"value":["g","r","h"]}',true);
		$countNameEditor = json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true);
		$exchangeListEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		
		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"editor"=>$textEditor,"primary");
		$data["head"][]=array("field"=>"id","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"type","viewer"=>$textViewer,"editor"=>$typeEditor);
		$data["head"][]=array("field"=>"count","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("field"=>"countName","viewer"=>$textViewer,"editor"=>$countNameEditor);
		$data["head"][]=array("field"=>"priceType","viewer"=>$textViewer,"editor"=>$typeEditor);
		$data["head"][]=array("field"=>"price","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("field"=>"priceName","viewer"=>$textViewer,"editor"=>$countNameEditor);
		$data["head"][]=array("field"=>"sale","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"image","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"data","viewer"=>$textViewer,"editor"=>$dictEditor);
		$data["head"][]=array("field"=>"pID","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"exchangeID","viewer"=>$textViewer,"editor"=>$textEditor,"always");
		$data["head"][]=array("field"=>"exchangeList","viewer"=>$textViewer,"editor"=>$exchangeListEditor);
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}
}


class ShopEvent extends DBTable{
	public function __construct($pID=null,$exchangeID=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		
		$this->setLQTableSelectCustomFunction(function ($data){
			// if($data["exchangeID"]){
			// 	$exchange = new Exchange($data["exchangeID"]);
			// 	$data["exchangeList"]=$exchange->list;				
			// }
			return $data;
		});

		$this->setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::get()->getCurrentDateTime();
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
				$this->priceName = json_decode($this->priceName,ture);
				$this->countName = json_decode($this->countName,ture);
				$this->data = json_decode($this->data,ture);
			}
			
		}
	}

	public function updateWithLQTable($p){
		
		// $exchange=null;
		// if($p["data"]["exchangeID"]){
		// 		$exchange = new Exchange($p["data"]["exchangeID"]);
		// 		if($p["data"]["exchangeList"] && is_string($p["data"]["exchangeList"]))$p["data"]["exchangeList"]=json_decode($p["data"]["exchangeList"],true);
		// 		if(is_array($p["data"]["exchangeList"]) && count($p["data"]["exchangeList"])>0)$exchange->list=$p["data"]["exchangeList"];		
		// 		$exchange->save();		
		// }
		// unset($p["data"]["exchangeList"]);
		// $r = parent::updateWithLQTable($p);

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

	public function insertWithLQTable($p){
		// if($p["data"]["exchangeID"]){
		// 	$exchange = new Exchange($p["data"]["exchangeID"]);
		// 	if($exchange->isLoaded())$p["data"]["exchangeList"]=$exchange->list;
		// }
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
		// $r = parent::insertWithLQTable($p);

		// if($exchange){
		// 	$r["data"]["exchangeList"]=$exchange->list;
		// }

		// kvManager::increase("shopListVer");
		// return $r;
	}

	public function loadWithDataTable($p){

		$listViewer=array("type"=>"select","field"=>"type");
		while($pData = Shop::getRowByQuery("",null,"id,type,count")){
			$listViewer["element"][] = $pData["id"]." : ".$pData["type"]." ".$pData["count"]."개";
			$listViewer["value"][]=$pData["id"];
		}

		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$dictEditor = array("type"=>"dictionary");
		$intEditor = json_decode('{"type":"text","datatype":"int"}',true);
		$typeEditor = json_decode('{"type":"select","element":["gold","ruby","heart"],"value":["g","r","h"]}',true);
		$countNameEditor = json_decode('{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}',true);
		$exchangeListEditor = json_decode('{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}',true);
		
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"진행상태","field"=>"state","viewer"=>json_decode('{"type":"text"}',true),"virtual");
		$data["head"][]=array("title"=>"대상상점ID","field"=>"id","viewer"=>$listViewer,"editor"=>$listViewer);
		$data["head"][]=array("title"=>"시작일시","field"=>"startDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"종료일시","field"=>"endDate","viewer"=>json_decode('{"type":"datetime","format":"Y/m/d h:i:s"}',true),"editor"=>json_decode('{"type":"datetime"}',true));
		$data["head"][]=array("title"=>"시작시간","field"=>"startTime","viewer"=>json_decode('{"type":"time","format":"h:i:s"}',true),"editor"=>json_decode('{"type":"time"}',true));
		$data["head"][]=array("title"=>"종료시간","field"=>"endTime","viewer"=>json_decode('{"type":"time","format":"h:i:s"}',true),"editor"=>json_decode('{"type":"time"}',true));
		$data["head"][]=array("title"=>"판매재화","field"=>"type","viewer"=>json_decode('{"type":"propChange"}',true),"editor"=>json_decode('{"type":"propSelect"}',true));
		$data["head"][]=array("title"=>"판매갯수","field"=>"count","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("title"=>"판매명","field"=>"countName","viewer"=>json_decode('{"type":"languageViewer"}',true),"editor"=>json_decode('{"type":"languageEditor"}',true));
		$data["head"][]=array("title"=>"구입재화","field"=>"priceType","viewer"=>json_decode('{"type":"propChange"}',true),"editor"=>json_decode('{"type":"propSelect"}',true));
		$data["head"][]=array("title"=>"구입갯수","field"=>"price","viewer"=>$textViewer,"editor"=>$intEditor);
		$data["head"][]=array("title"=>"구입명","field"=>"priceName","viewer"=>json_decode('{"type":"languageViewer"}',true),"editor"=>json_decode('{"type":"languageEditor"}',true));
		$data["head"][]=array("title"=>"세일표시","field"=>"sale","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"결제ID","field"=>"pID","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("title"=>"교환ID","field"=>"exchangeID","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"exchangemaker","content":"상점이벤트","statsID":"shopEvent","statsValueField":"no"}',true));
		$data["head"][]=array("title"=>"데이터","field"=>"data","viewer"=>$textViewer,"editor"=>$dictEditor);
		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//	몬스터
////////////////////////////////////////////////////////////////////////////////////////
class Monster extends DBTable{


	public function __construct($cardNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		

		if($cardNo)$this->no=$cardNo;

		if($cardNo){
			$query = "no=".$cardNo;
			parent::load($query);
		}
	}

	public function updateWithLQTable($p){
		$r = parent::updateWithLQTable($p);
		kvManager::increase("monsterListVer");
		return $r;
	}

	public function insertWithLQTable($p){
		$r = parent::insertWithLQTable($p);
		kvManager::increase("monsterListVer");
		return $r;
	}

	public function loadWithDataTable($p){
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"type","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"select","element":["circle","snake","jr"]}',true));
		$data["head"][]=array("field"=>"name","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"image","viewer"=>json_decode('{"type":"custom","func":"imageViewer"}',true),"editor"=>json_decode('{"type":"custom","func":"imageSelector"}',true));
		$data["head"][]=array("field"=>"resourceInfo","viewer"=>json_decode('{"type":"json"}',true),"editor"=>json_decode('{"type":"dictionary","element":
					[	
						{"field":"ccbiID","type":"text"},		
						{"field":"ccbi","type":"custom","func":"resourceSelector","viewer":"text"},
						{"field":"imageID","type":"text"},	
						{"field":"plist","type":"custom","func":"resourceSelector","viewer":"text"},
						{"field":"pvrccz","type":"custom","func":"resourceSelector","viewer":"text"},
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
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
	}

}



////////////////////////////////////////////////////////////////////////////////////////
//	몬스터
////////////////////////////////////////////////////////////////////////////////////////
class Pattern extends DBTable{


	public function __construct($cardNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		

		if($cardNo)$this->no=$cardNo;

		if($cardNo){
			$query = "no=".$cardNo;
			parent::load($query);
		}
	}

	public function loadWithDataTable($p){
		$data["head"][]=array("field"=>"no","viewer"=>json_decode('{"type":"text"}',true),"primary");
		$data["head"][]=array("field"=>"category","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"type","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"name","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"text"}',true));
		$data["head"][]=array("field"=>"image","viewer"=>json_decode('{"type":"custom","func":"imageViewer"}',true),"editor"=>json_decode('{"type":"custom","func":"imageSelector"}',true));
		$data["head"][]=array("field"=>"template","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"dictionary"}',true));
		$data["head"][]=array("field"=>"comment","viewer"=>json_decode('{"type":"text"}',true),"editor"=>json_decode('{"type":"textarea"}',true));
		$data["head"][]=array("manage"=>"delete update insert");
		return $data;
	}

}
////////////////////////////////////////////////////////////////////////////////////////
//	샵
////////////////////////////////////////////////////////////////////////////////////////


class Exchange extends DBTable{
public function __construct($id=null){

		parent::__construct();
		
		$this->setLQTableInsertCustomFunction(function ($param){
			return "`id`=".$param["data"]["id"];
		});

		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		$this->id=$id;
		if($id){
			$q_where = "`id`='".$id."'";
			if(parent::load($q_where)){
				$this->list = json_decode($this->list,true);
			}
		}
	}

	public function makeExchangeIDByRandom($param){
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

	public function saveExchangeID($param){
		
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
}



class FormSetter extends DBTable{
public function __construct($no=null){

		parent::__construct();

		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		$this->no = $no;
		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}

}


class EndlessRank extends DBTable{
	public $m__userIndex=null;

	public function __construct($memberID=null,$weekNo=null){

		parent::__construct();

		$this->setPrimarykey("no",true);


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

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

}



class EndlessPlayList extends DBTable{
	public $m__userIndex=null;
	
	public function __construct($memberID=null,$pNo=null){

		parent::__construct();
		$this->setPrimarykey("no",true);

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

		if($pNo){
			if(parent::load("no=".$pNo)){
			}
		}
	}

	public function getPlayDataByRandom($memberID,$lvl,$limit=1,$fieldlist="*"){
		$result = array();
		$query = mysql_query("select ".$fieldlist." from `".$this->getDBTable()."` where memberID in (SELECT DISTINCT memberID FROM `".$this->getDBTable()."`) and victory<=".($lvl+1)." and victory>=".($lvl-1)." and memberID <> ".$memberID." ORDER BY RAND() limit ".$limit,UserIndex::getShardConnectionByRandom());
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
			$query = mysql_query("select ".$fieldlist." from `".$this->getDBTable()."` where memberID in (SELECT DISTINCT memberID FROM `".$this->getDBTable()."`) and memberID<>'".$memberID."' ORDER BY RAND() limit ".$limit,UserIndex::getShardConnectionByRandom());
			while($rData = mysql_fetch_assoc($query)){
				if($check)unset($rData["playData"]);
				$result[]=$rData;
			}
			$i++;
			if($i>50)break;
		}

		LogManager::addLog(mysql_error());
		
		return $result;
	}

	public function getPlayDataByNo($no){

		$query = mysql_query("select * from `".$this->getDBTable()."` where no=".$no,$this->getDBConnection());
		if($query)$result = mysql_fetch_assoc($query);
		if(!$result){
			$query = mysql_query("select * from `".$this->getDBTable()."` ORDER BY RAND() limit 1",$this->getDBConnection());
			if($query)$result = mysql_fetch_assoc($query);
		}

		LogManager::addLog(mysql_error());
		
		return $result;
	}

	public function loadWithDataTable($p){

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


class CommonSetting extends DBTable{
	public function __construct($key=null){

		parent::__construct();

		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		if($key){
			$this->key = $key;
			$q_where = "`key`='".$key."'";
			if(parent::load($q_where)){
				$conv = json_decode($this->value,true);
				if($conv){
					$this->value = $conv;
				}
			}
		}
	}

	public function loadWithDataTable($p){
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


class KeyIntValue extends DBTable{
	public function __construct($no=null,$key=null){

		parent::__construct();

		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		$this->no = $no;
		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}

	public function loadWithDataTable($p){
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


class TimeEvent extends DBTable{
public function __construct($no=null){

		parent::__construct();
		
		$this->setLQTableSelectCustomFunction(function($rData){
			$now = TimeManager::get()->getCurrentDateTime();
			if($rData["startDate"]<=$now && $rData["endDate"]>=$now){
				$rData["state"]="진행중";	
			}else if($r["startDate"]>=$now){
				$rData["state"]="대기";
			}else{
				$rData["state"]="종료";
			}


			
			return $rData;
		});

		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		$this->no = $no;
		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}


	public function loadWithDataTable($p){
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


class ModifyHistory extends DBTable{
	public $m__userIndex = null;
	public function __construct($memberID=null,$no=null){

		parent::__construct();
		
		$this->setLQTableSelectQueryCustomFunction(function ($param){
			if($param["where"]["id"]=="*")return "";

			if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
			else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
			else $user = UserData::create(null,$param["where"]["id"]);

			if(!$user->isLoaded())return "where memberID='-1'";

			return "where memberID='".$user->memberID."'";
		});

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

		if($no){
			$q_where = "`no`='".$no."'";
			if(parent::load($q_where)){
			}
		}
	}

	public function save($isIncludePrimaryKey = false){
		if(!$this->regDate)$this->regDate=TimeManager::get()->getCurrentDateTime();
		return parent::save($isIncludePrimaryKey);
	}

	public function loadWithDataTable($p){
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


class AdminUser extends DBTable{
	public $m__isLogin = false;
	public function __construct($no=null,$id=null,$pw=null){

		parent::__construct();
		


		// $this->setLQTableSelectQueryCustomFunction(function ($param){
		// 	if($param["where"]["id"]=="*")return "";

		// 	if($param["where"]["type"]=="sno")$user = UserData::create($param["where"]["id"]);
		// 	else if($param["where"]["type"]=="nick")$user = UserData::create(null,null,$param["where"]["id"]);
		// 	else $user = UserData::create(null,$param["where"]["id"]);

		// 	if(!$user->isLoaded())return "where memberID='-1'";

		// 	return "where memberID='".$user->memberID."'";
		// });

		// if($memberID){
		// 	$this->m__userIndex = UserIndex::create($memberID);
		// 	$this->setDBInfo($this->m__userIndex->getShardDBInfo());
		// 	$this->memberID=$memberID;
		// }

		$this->setPrimarykey("no",true);
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
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

		if($q_where){
			if(parent::load($q_where)){
				$this->permission = json_decode($this->permission,true);
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
		return $p[$where];
	}

	public function login($p){
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

	public function insertWithLQTable($p){
		if($p["data"]["passwd"]){
			$q = mysql_query("select password('".$p["data"]["passwd"]."')",DBManager::get()->getMainConnection());
			$pass = mysql_fetch_array($q);
			$p["data"]["passwd"]=$pass[0];
		}
		$r = parent::insertWithLQTable($p);
		return $r;
	}

	public function updateWithLQTable($p){
		if($p["data"]["passwd"])unset($p["data"]["passwd"]);
		$r = parent::updateWithLQTable($p);
		return $r;
	}

	public function loadWithDataTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		
		$data["head"][]=array("title"=>"고유번호","field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("title"=>"ID","field"=>"id","viewer"=>$textViewer,"editor"=>$textViewer);
		$data["head"][]=array("title"=>"PW","field"=>"passwd","viewer"=>$textViewer,"editor"=>json_decode('{"type":"password"}',true));
		$data["head"][]=array("title"=>"권한","field"=>"permission","viewer"=>$textViewer,"editor"=>json_decode('{"type":"dictionary"}',true));

		$data["head"][]=array("manage"=>"delete update insert");
		
		return $data;
	}
}

?>