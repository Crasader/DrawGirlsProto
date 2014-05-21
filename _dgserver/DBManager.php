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
			$this->m_userIndex[$memberID] = UserIndex::create($memberID);
			$this->m_dbInfo[$memberID] = $this->m_userIndex[$memberID]->getShardDBInfo();
			$this->m_isSuccess[$memberID]=true;
			mysql_query("SET AUTOCOMMIT=0",$this->m_dbInfo[$memberID]->getConnection());
			mysql_query("BEGIN",$this->m_dbInfo[$memberID]->getConnection());

			LogManager::get()->addLog("start transaction".mysql_error());
		}else{
			LogManager::get()->addLog("start transaction but ++");
			$this->m_releaseCount[$memberID]++;
		}
	}

	public function commit($memberID){
		if(!$this->m_releaseCount[$memberID]) return false;


		LogManager::get()->addLog("commit transaction count=".$this->m_releaseCount[$memberID]."-1");

		$this->m_releaseCount[$memberID]--;

		if(!$this->isSuccess($memberID)){
			if($this->m_releaseCount[$memberID]==0){
				$result = mysql_query("ROLLBACK", $this->m_dbInfo[$memberID]->getConnection());
				LogManager::get()->addLog("commit query but rollback : ".mysql_error());
			}
			return false;
		}

		if($this->m_releaseCount[$memberID]==0){
			$result = mysql_query("COMMIT", $this->m_dbInfo[$memberID]->getConnection());
			LogManager::get()->addLog("commit query : ".mysql_error());
		}else{
			$result = true;
		}

		
		return $result;
	}

	public function rollback($memberID){
		if(!$this->m_releaseCount[$memberID]) return false;


		LogManager::get()->addLog("rollback transaction count=".$this->m_releaseCount[$memberID]."-1");

		$this->m_releaseCount[$memberID]--;

		if($this->m_releaseCount[$memberID]==0){
			$result = mysql_query("ROLLBACK", $this->m_dbInfo[$memberID]->getConnection());
		}else{
			$this->setSuccess($memberID,false);
			$result=true;
		}
		LogManager::get()->addLog("rollback transaction");
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

	public static function create($memberID=null,$userindex=null){
		
		LogManager::get()->addLog("create userIndex");
		if($memberID && self::$sharedIndexes[$memberID]){
			LogManager::get()->addLog("finded userIndex in sharedIndexes ".$memberID);
			return self::$sharedIndexes[$memberID];
		}


		LogManager::get()->addLog("new userIndex, memberID is ".$memberID." and userIndex is ".$userindex);
		$newIndex =new UserIndex($memberID,$userindex);
		if($newIndex->isLoaded()){
			LogManager::get()->addLog("useindex load success no is".$newIndex->no);
			self::$sharedIndexes[$newIndex->memberID]=$newIndex;
		}else{
			LogManager::get()->addLog("userindex load fail it's new obj is ".json_encode($newIndex->getArrayData(true))." and shardIndex is ".$newIndex->shardIndex);
			//$test = get_class_vars(get_class($newIndex));
			//LogManager::get()->addLog("userindex load fuck!!".json_encode($test));
		}	
		return $newIndex;
	}

	public function __construct($memberID=null,$userindex=null){

		parent::__construct();
		
		LogManager::get()->addLog("construct userIndex for ".$memberID);

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

		 	if(parent::load("memberID=".$memberID)){
		 		//$this->autoMatching($this->m__result);
		 		LogManager::get()->addLog("load success userindex shardIndex is".$this->shardIndex);
		 	}else{
				$this->memberID = $memberID;
		 		$this->shardIndex = $this->getShardIndexByNumberKey($memberID);
		 		LogManager::get()->addLog("load fail userindex shardIndex is ".$this->shardIndex." m_shardDBCount is ".DBManager::get()->getShardDBCount());
		 		//$this->save(true);
		 	}
		}
	}


	public function getUserIndex(){
		return $this->no;
	}
	public function getShardIndexByNumberKey($numberKey){
		return abs($numberKey%DBManager::get()->getShardDBCount());
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

		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
		}

		if($memberID && $no){
			parent::load("no=".$no." and memberID=".$memberID);
			// if(parent::load("no=".$no." and memberID=".$memberID)){
			// 	$this->autoMatching($this->m__result);
			// }
		}
	}

	public function selectWithLQTable($param){
		$dataList = array();
		$where="";

		if($param["category"]){
			$where = "where category='".$param["category"]."'";
		}

		$limit = $param["limit"];
		if(!$limit)$limit=10;

		$rl=0;

		if($param["nextInfo"] && $param["nextInfo"]["nextTime"]){
			if(!$where)$where="where";
			else $where.=" and ";
			$where=$where." regTime<".$param["nextInfo"]["nextTime"];
		}	


		$query = $where." order by no desc limit ".$limit;
	    while($data = self ::getRowByQuery($query)){
			$rl++;
			$l=0;

			if($dataList[$limit-1]["regTime"]>$data["regTime"]){
				self::$m__qResult=null;
				self::$m__qCnt++;
			}
			for($i=0;$i<count($dataList);$i++){
				if($dataList[$i]["regTime"]<$data["regTime"])break;
				$l++;
			}
			array_splice($dataList, $l, 0, array($data));
			array_splice($dataList, $limit, 1);
	    }
	    $result["nextInfo"]=array("nextTime"=>$dataList[$limit-1]["regTime"]);
	    $result["param"]=$param;
	    $result["query"]=$query;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}


}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	// static public $m__queryResult = null;
	// static public $m__queryCnt = 0;
	public function __construct($memberID=null){
		parent::__construct();
		
		$this->setPrimarykey("no");
		//$this->setDBTable(DBManager::getST("userdata"));
		$this->memberID = $memberID;
		
		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			LogManager::get()->addLog("create userindex for ".$memberID." result is ".json_encode($this->m__userIndex->getArrayData(true)));
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			if($this->m__userIndex->isLoaded()){
				if(parent::load("memberID=".$memberID)){
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
		if(!$this->joinDate)$this->joinDate=TimeManager::get()->getCurrentDateTime();
	}



	public function save($isIncludePrimaryKey=false){
		//마지막접속시간
		$this->lastDate = TimeManager::get()->getCurrentDateTime();
		$this->lastTime = TimeManager::get()->getTime();
		return parent::save($isIncludePrimaryKey);
	}

	public function remove(){
		$r = $this->m__userIndex->remove();
		if($r)parent::remove();
		
		LogManager::get()->addLog("dropout User query error is".mysql_error()." and ".json_encode($r));

		return $r;
	}

	public function getUserIndex(){
		return $this->m__userIndex->getUserIndex();
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
		LogManager::get()->addLog("addfriend ".$friendID);

		$friendList=array();
		if($this->friendList)$friendList = json_decode($this->friendList,true);
		
		$friendList[]=$friendID;
		
		$friendList = array_unique($friendList);		
		$this->friendList = json_encode($friendList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		LogManager::get()->addLog("addfriendResult is ".$this->friendList);
	}

	public function setArchiveData($key,$value){
		if(!is_array($this->archiveData))$this->archiveData = array();

		$this->archiveData = array_merge($this->archiveData,array($key=>$value));

	}

}



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
			

// 			LogManager::get()->addLog("stage score construct ".$q_where);
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

			parent::load($query);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//	피스정보
////////////////////////////////////////////////////////////////////////////////////////
class Piece extends DBTable{


	public function __construct($pieceNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		

		if($pieceNo)$this->no=$pieceNo;

		if($pieceNo){
			$query = "no=".$pieceNo;
			parent::load($query);
		}
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
			parent::load($query);
		}
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
			return "where memberID='".$param["id"]."'";
		});

		$this->setLQTableSelectCustomFunction(function ($rData){
	    	$cardInfo = new Card($rData["cardNo"]);
	    	$pieceInfo = new Piece($cardInfo->stage);
	    	$puzzleInfo = new Puzzle($pieceInfo->puzzle);

	    	$rData["cardName"]=$cardInfo->name;
	    	$rData["cardImg"]=json_decode($cardInfo->imgInfo,true);
	    	$rData["puzzleInfo"]=$puzzleInfo->no."-".$puzzleInfo->title;
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
			return "where memberID='".$param["id"]."'";
		});

		$this->setLQTableSelectCustomFunction(function ($data){
			$puzzle = new Puzzle($data["puzzleNo"]);
			$data["title"]=$puzzle->title;
			return $data;
		});

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
			return "where memberID='".$param["id"]."'";
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



	public function __construct($no=null){
		
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

		if($no)$this->no=$no;

		if($no){
			$query = "no=".$no;
			parent::load($query);
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
			return "where memberID='".$param["id"]."'";
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
			return "where memberID='".$param["id"]."'";
		});

		$this->setLQTableSelectCustomFunction(function ($rData){
			if($rData["reward"])$rData["reward"] = json_decode($rData["reward"],true);
			if($rData["exchangeID"]){
				$exchange = new Exchange($rData["exchangeID"]);
				$rData["exchangeList"]=$exchange->list;				
			}
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


	public function updateWithLQTable($p){
		$exchange=null;
		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
		}
		
		unset($p["data"]["exchangeList"]);
		
		$r = parent::updateWithLQTable($p);
		
		if($exchange)$r["data"]["exchangeList"]=$exchange->list;

		return $r;
	}

	public function insertWithLQTable($p){
		$exchange=null;
		if($p["data"]["exchangeID"]){
			$exchange = new Exchange($p["data"]["exchangeID"]);
		}
		
		unset($p["data"]["exchangeList"]);

		$r = parent::insertWithLQTable($p);

		if($exchange)$r["data"]["exchangeList"]=$exchange->list;

		return $r;
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
			return "where memberID='".$param["id"]."'";
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
		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"editor"=>$textEditor,"primary");
		$data["head"][]=array("field"=>"memberID","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"pr","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"fr","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"h","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"m","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"g","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i1","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i2","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i3","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i4","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i5","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i6","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i7","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i8","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"i9","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"p1","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"p2","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"p3","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"p4","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"p5","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"p6","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"p7","viewer"=>$textViewer,"editor"=>$textEditor);
		//$data["head"][]=array("manage"=>"update delete insert");
		
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
			return "where memberID='".$param["id"]."'";
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


}




////////////////////////////////////////////////////////////////////////////////////////
//	로그인이벤트
////////////////////////////////////////////////////////////////////////////////////////
class LoginEvent extends DBTable{
	
	public function __construct($fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectCustomFunction(function ($data){
			if($data["exchangeID"]){
				$exchange = new Exchange($data["exchangeID"]);
				$data["exchangeList"]=$exchange->list;				
			}
			return $data;
		});

		$this->setDBInfo(DBManager::get()->getMainDBInfo());

		if($fNo){
			parent::load("no=".$fNo);
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

		return $r;
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

		$this->setLQTableSelectCustomFunction(function ($data){
			if($data["exchangeID"]){
				$exchange = new Exchange($data["exchangeID"]);
				$data["exchangeList"]=$exchange->list;				
			}
			return $data;
		});

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
		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("field"=>"title","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"os","viewer"=>$textViewer,"editor"=>$osEditor);
		$data["head"][]=array("field"=>"startDate","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"endDate","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"reward","viewer"=>$textViewer,"editor"=>$rewardEditor);
		$data["head"][]=array("field"=>"isCommon","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"writer","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"regDate","viewer"=>$textViewer);
		$data["head"][]=array("field"=>"exchangeID","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"exchangeList","viewer"=>$textViewer,"editor"=>$exchangeEditor);

		$data["head"][]=array("manage"=>"update delete insert");
		
		return $data;
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
		$p["data"]["regDate"]=TimeManager::get()->getCurrentDateTime();
		$r = parent::insertWithLQTable($p);

		if($exchange){
			$r["data"]["exchangeList"]=$exchange->list;
		}

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
		
		$this->setLQTableInsertCustomFunction(function ($param){
			return "`cuponCode`='".$param["data"]["cuponCode"]."'";
		});
		if($fNo){
			parent::load("cuponCode='".$fNo."'");
		}
	}


	public function loadWithDataTable($p){
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");
		$codeEditor = array("type"=>"array","element"=>array("type"=>"text"));
		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("field"=>"cuponNo","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"cuponCode","viewer"=>$textViewer,"editor"=>$codeEditor);
		$data["head"][]=array("field"=>"shardIndex","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("manage"=>"delete insert");
		
		return $data;
	}

	public function updateWithLQTable($p){
		//$r = parent::updateWithLQTable($p);
		$r["result"]="fail";
		return $r;
	}

	public function insertWithLQTable($p){
		$codeList = json_decode($p["data"]["cuponCode"],true);
		//unset($p["data"]["code"]);
		LogManager::get()->addLog("test count is ".count($codeList));
		for($i=0;$i<count($codeList);$i++){

			LogManager::get()->addLog("test1 ".$i);
			
			$p2=unserialize(serialize($p));
			$p2["data"]["cuponCode"]=$codeList[$i];
			$this->setLoaded(false);
			$ri = parent::insertWithLQTable($p2);
			if($ri["result"]=="ok")$r["data"][]=$ri["data"];
		}
		$r["result"]="ok";
		return $r;
	}

	public function getRandomString($length = 10) {
	    $validCharacters = "abcdefghijklmnopqrstuxyvwz123456789";
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
			return "where memberID='".$param["id"]."'";
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
		
		$data["head"][]=array("field"=>"no","viewer"=>$textViewer,"primary");
		$data["head"][]=array("field"=>"memberID","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"cuponNo","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"cuponCode","viewer"=>$textViewer,"editor"=>$textEditor);
		$data["head"][]=array("field"=>"userDate","viewer"=>$textViewer,"editor"=>$textEditor);
		
		$data["head"][]=array("manage"=>"update delete insert");
		
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
			$this->setDbInfo(DBManager::get()->getDBInfoByShardIndex($shardIndex));
			
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
		LogManager::get()->addLog("weeklyScore is ".DBManager::getST("weeklyScore"));


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
		}
		
		if($memberID && $weekNo){
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
	


	public function __construct($memberID=null,$stageNo=null,$where=null){
		
		if(!$stageNo)return false;
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setDBInfo(DBManager::get()->getDBInfoByShardKey($stageNo));
		$this->stageNo=$stageNo;
		$this->memberID=$memberID;
		
		if($memberID || $where){
			if($where)$q_where = $where;
			else $q_where = "memberID=".$memberID." and stageNo=".$stageNo;
			

			LogManager::get()->addLog("stage score construct ".$q_where);
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
		$topquery = mysql_query("select * from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo." order by score desc limit 4",$this->getDBConnection());
		
		LogManager::get()->addLog("select * from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo." order by score desc limit 4");
		$rank=1;
		$rdata = array(); 
		while($data = mysql_fetch_assoc($topquery)){
			$data["rank"]=$rank++;
			$rdata[]=$data;
		}

		return $rdata;
	}

	public function getAllUser(){
		$mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo,$this->getDBConnection()));
		$alluser=$mresult[0];
		return $alluser;
	}

	public function getMyRank(){
		if($this->score<=0)return -1;
		$mresult = mysql_fetch_array(mysql_query("select count(*) from ".DBManager::getST("stagescore")." where stageNo=".$this->stageNo." and score>".$this->score,$this->getDBConnection()));
		$myrank=$mresult[0]+1;
		return $myrank;
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
}
////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터히스토리
////////////////////////////////////////////////////////////////////////////////////////
class CharacterHistory extends DBTable{
	public $m__userIndex = null;
	public function __construct($memberID=null,$characterNo=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setLQTableSelectCustomFunction(function ($data) {
			LogManager::get()->addLog("test");
			$character = new Character($data["characterNo"]);
			$data["name"]=$character->name;
			return $data;
		});

		$this->setLQTableSelectQueryCustomFunction(function ($param){
			return "where memberID='".$param["id"]."'";
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
}


////////////////////////////////////////////////////////////////////////////////////////
//	공지사항
////////////////////////////////////////////////////////////////////////////////////////

class Notice extends DBTable{
	
	public function __construct($no=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);


		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		
		if($no){
			$q_where = "no=".$no;
			parent::load($q_where);
		}
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

		if($pID){
			$q_where = "pID='".$pID."'";
			parent::load($q_where);
		}else if($exchangeID){
			$q_where = "exchangeID='".$exchangeID."'";
			parent::load($q_where);
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

		return $r;
	}
	// public function selectWithLQTable($p){
	// 	$p["data"]["exchangeList"]="test";
	// 	return parent::selectWithLQTable($p);
	// }
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

		$orderField = "victory";
		while($data = EndlessRank::getRowByQuery($query)){
			$rl++;
			$l=0;
			if(count($rdata)==0){
				$rdata[]=$data;
				continue;
			}

			if($rdata[$count-1][$orderField]>$data[$orderField]){
				EndlessRank::$m__qResult=null;
				EndlessRank::$m__qCnt++;
			}
			for($i=0;$i<count($rdata);$i++){
				if($rdata[$i][$orderField]<$data[$orderField])break;
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
	
	public function __construct($memberID=null,$weekNo=null){

		parent::__construct();

		$this->setPrimarykey("no",true);
		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

		if($memberID && $weekNo){
			$q_where = "`memberID`='".$memberID."'";
			if($weekNo)$q_where.=" and weekNo=".$weekNo;
			if(parent::load($q_where)){
			}
		}
	}

	public function getPlayDataByRandom($lvl){

		$query = mysql_query("select * from `".$this->getDBTable()."` where autoLevel<=".($lvl+5)." and autoLevel>=".($lvl-5)." ORDER BY RAND() limit 1",UserIndex::getShardConnectionByRandom());
		LogManager::get()->addLog("select * from `".$this->getDBTable()."` where autoLevel<=".($lvl+5)." and autoLevel>=".($lvl-5)." ORDER BY RAND() limit 1");
		if($query)$result = mysql_fetch_assoc($query);
		if(!$result){
			$query = mysql_query("select * from `".$this->getDBTable()."` ORDER BY RAND() limit 1",UserIndex::getShardConnectionByRandom());
			if($query)$result = mysql_fetch_assoc($query);
		}

		LogManager::get()->addLog(mysql_error());
		
		return $result;
	}

}


?>