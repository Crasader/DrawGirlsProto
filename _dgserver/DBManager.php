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

			LogManager::get()->addLog("start tranjaction".mysql_error());
		}else{
			LogManager::get()->addLog("start tranjaction but ++");
			$this->m_releaseCount[$memberID]++;
		}
	}

	public function commit($memberID){
		if(!$this->m_releaseCount[$memberID]) return false;


		LogManager::get()->addLog("commit tranjaction count=".$this->m_releaseCount[$memberID]."-1");

		$this->m_releaseCount[$memberID]--;

		if(!$this->isSuccess($memberID) && $this->m_releaseCount[$memberID]==0){
			$result = mysql_query("ROLLBACK", $this->m_dbInfo[$memberID]->getConnection());
			LogManager::get()->addLog("commit query but rollback : ".mysql_error());
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


		LogManager::get()->addLog("rollback tranjaction count=".$this->m_releaseCount[$memberID]."-1");

		$this->m_releaseCount[$memberID]--;

		if($this->m_releaseCount[$memberID]==0){
			$result = mysql_query("ROLLBACK", $this->m_dbInfo[$memberID]->getConnection());
		}else{
			$this->setSuccess($memberID,false);
			$result=true;
		}
		LogManager::get()->addLog("rollback tranjaction");
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
		 		LogManager::get()->addLog("load fail userindex shardIndex is ".$this->shardIndex." m_shardDBCount is ".DBManager::get()->m_shardDBCount);
		 		//$this->save(true);
		 	}
		}
	}


	public function getUserIndex(){
		return $this->no;
	}
	public function getShardIndexByNumberKey($numberKey){
		return abs($numberKey%DBManager::get()->m_shardDBCount);
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

	public function getHistory($param){
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

				}
			}
		}
		

		//가입시간
		if(!$this->joinDate)$this->joinDate=TimeManager::get()->getCurrentDateString();
	}



	public function save($isIncludePrimaryKey=false){
		//마지막접속시간
		$this->lastDate = TimeManager::get()->getCurrentDateString();
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
	/*
public function __construct($memberID=0,$DBInfo=null){
		if($DBInfo==null){
			$DBInfo = DBManager::get()->getDBInfoByShardKey($memberID);
		}
		$this->m__DBInfo = $DBInfo;
		$this->m_memberID = $memberID;
		
		if($this->m_memberID){
			$query = "select * from ".DBManager::getST("userdata")." where memberID=".$this->m_memberID;
			$this->m__result = mysql_fetch_array(mysql_query($query,$this->m__DBInfo->getConnection()),MYSQL_ASSOC);
			if($this->m__result){
				$this->m_no = $this->m__result["no"];
				$this->m_data = $this->m__result["data"];
				$this->m_nick = $this->m__result["nick"];
				$this->m_lastDate = $this->m__result["lastDate"];
				$this->m_joinDate = $this->m__result["joinDate"];
			}
		}
	}
*/
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


}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Message extends DBRow{
	public $m_no=null;
	public $m_memberID=null;
	public $m_content=null;
	public $m_regDate=null;
	public $m_confirmDate=null;
	public $m_regTime=null;
	public $m_friendID=null;
	public $m_type=null;
	public $m_isSendMsg=null;
	public $m__userIndex=null;

	static public $m__queryResult = null;
	static public $m__queryCnt = 0;
	public function __construct($memberID=null,$messageNo=null){
		parent::__construct();
		$this->setPrimarykey("no");
		//$this->setDBTable(DBManager::getST("message"));
		
		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
		}
		
		if($memberID && $messageNo){
			if(parent::load("no = $messageNo")){
				$this->autoMatching($this->m__result);
				// $this->m_memberID = $this->m__result["memberID"];
				// $this->m_no = $this->m__result["no"];
				// $this->m_content = $this->m__result["conetnt"];
				// $this->m_regDate = $this->m__result["regDate"];
				// $this->m_friendID = $this->m__result["friendID"];
				// $this->m_type = $this->m__result["type"];
				// $this->m_isSendMsg = $this->m__result["isSendMsg"];
				// $this->m_data = $this->m__result["data"];
			}
		}
	}
	

	public function send(){
		if(!$this->m_memberID)return "error";
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


	public function getList($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."' and isSendMsg=0")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
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
		

		if($cardNo)$this->m_no=$cardNo;

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

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
	    	$cardInfo = new Card($rData["cardNo"]);
	    	$pieceInfo = new Piece($cardInfo->stage);
	    	$puzzleInfo = new Puzzle($pieceInfo->puzzle);

	    	$rData["cardName"]=$cardInfo->name;
	    	$rData["cardImg"]=json_decode($cardInfo->imgInfo,true);
	    	$rData["puzzleInfo"]=$puzzleInfo->no."-".$puzzleInfo->title;

			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}


}


////////////////////////////////////////////////////////////////////////////////////////
//	퍼즐히스토리
////////////////////////////////////////////////////////////////////////////////////////

class PuzzleHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$puzzleNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");


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

	public function getHistory($param){
		$dataList = array();

	    while($rData = self ::getRowByQuery("where memberID='".$param["id"]."'")){
			
	    	$puzzle = new Puzzle($rData["puzzleNo"]);
			$rData["title"]=$puzzle->title;
			$dataList[]=$rData;

	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}


}

////////////////////////////////////////////////////////////////////////////////////////
//	스테이지히스토리
////////////////////////////////////////////////////////////////////////////////////////

class PieceHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$pieceNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");


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

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}


}
////////////////////////////////////////////////////////////////////////////////////////
//		불량닉네임
////////////////////////////////////////////////////////////////////////////////////////
class FaultyNick extends DBRow{

	public $m_no;
	public $m_nick;
	public $m_isIncluionRule;

	public function __construct($no=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		

		if($no)$this->m_no=$no;

		if($no){
			$query = "no=".$no;
			if(parent::load($query)){
				$this->autoMatching($this->m__result);
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
		
		$this->setPrimarykey("no");
		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		

		if($no)$this->no=$no;

		if($no){
			$query = "no=".$no;
			parent::load($query);
		}
	}


	public function getList($param){
		$dataList = array();

	    while($rData = self::getRowByQuery()){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}

	public function writeData($param){
		$this->id = $param["data"]["id"];
		$this->category = $param["data"]["category"];
		$this->title = $param["data"]["title"];
		$this->reward = $param["data"]["reward"];
		$this->goal = $param["data"]["goal"];
		if($this->save()){
			$r["data"] = $this->getArrayData(true);
			$r["result"]="ok";
			return $r;
		}

		$r["result"]="fail";
		return $r;
	}


	public function updateData($param){

		parent::load("no=".$param["data"]["no"]);

		if(!$this->isLoaded()){
			$r["result"]="fail";
			return $r;
		}
		$this->id = $param["data"]["id"];
		$this->category = $param["data"]["category"];
		$this->title = $param["data"]["title"];
		$this->reward = $param["data"]["reward"];
		$this->goal = $param["data"]["goal"];
		if($this->save()){
			$r["data"] = $this->getArrayData(true);
			$r["result"]="ok";
			return $r;
		}

		$r["result"]="fail";
		return $r;
	}

	public function deleteData($param){
		parent::load("no=".$param["data"]["no"]);

		if(!$this->isLoaded()){
			$r["result"]="fail";
			return $r;
		}

		if($this->remove()){
			$r["result"]="ok";
			return $r;

		}

		$r["result"]="fail";
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
		
		$this->setPrimarykey("no");


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

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
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


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->setField("memberID",$memberID);
		}

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and no=".$fNo);
		}
	}

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
    	if($rData["reward"])$rData["reward"] = json_decode($rData["reward"],true);
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}


	public function writeData($param){

		$this->m__userIndex = UserIndex::create($param["data"]["memberID"]);
		$this->setDBInfo($this->m__userIndex->getShardDBInfo());

		parent::load("no=".$param["data"]["no"]);

		foreach ($param["data"] as $key => $value) {
			$this->$key = $value;
		}


		if($this->save()){
			$r["data"] = $this->getArrayData(true);
			$r["result"]="ok";
			return $r;
		}

		$r["result"]="fail";
		return $r;
	}


	public function updateData($param){

		$this->m__userIndex = UserIndex::create($param["data"]["memberID"]);
		$this->setDBInfo($this->m__userIndex->getShardDBInfo());

		parent::load("no=".$param["data"]["no"]);

		unset($param["data"]["no"]);

		if(!$this->isLoaded()){
			$r["result"]="fail";
			$r["message"]="로드실패";
			return $r;
		}

		foreach ($param["data"] as $key => $value) {
			$this->$key = $value;
		}

		if($this->save()){
			$r["data"] = $this->getArrayData(true);
			$r["result"]="ok";
			return $r;
		}

		$r["result"]="fail";
		return $r;
	}


	public function deleteData($param){
		$this->m__userIndex = UserIndex::create($param["data"]["memberID"]);
		$this->setDBInfo($this->m__userIndex->getShardDBInfo());

		parent::load("no=".$param["data"]["no"]);

		if(!$this->isLoaded()){
			$r["result"]="fail";
			return $r;
		}

		if($this->remove()){
			$r["result"]="ok";
			return $r;

		}

		$r["result"]="fail";
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


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

		if($memberID && $type){
			parent::load("memberID=".$memberID." and `type`='".$type."'");
		}
	}

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
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


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and `no`=".$fNo);
		}
	}

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}
}


///////////////////////////////////////////////////////////////////////////////////////
//	골드히스토리
////////////////////////////////////////////////////////////////////////////////////////
class GoldHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->setField("memberID",$memberID);
		}

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and no=".$fNo);
		}
	}

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}

}


////////////////////////////////////////////////////////////////////////////////////////
//	루비히스토리
////////////////////////////////////////////////////////////////////////////////////////
class RubyHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->setField("memberID",$memberID);
		}

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and no=".$fNo);
		}
	}

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}

}

////////////////////////////////////////////////////////////////////////////////////////
//	하트히스토리
////////////////////////////////////////////////////////////////////////////////////////
class HeartHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->setField("memberID",$memberID);
		}

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and no=".$fNo);
		}
	}

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}

}



////////////////////////////////////////////////////////////////////////////////////////
//	쿠폰히스토리
////////////////////////////////////////////////////////////////////////////////////////
class CuponHistory extends DBTable{
	public $m__userIndex;

	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->setField("memberID",$memberID);
		}

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and no=".$fNo);
		}
	}

	public function getHistory($param){
		$dataList = array();

	    while($rData = self::getRowByQuery("where memberID='".$param["id"]."'")){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}

}




////////////////////////////////////////////////////////////////////////////////////////
//	로그인이벤트
////////////////////////////////////////////////////////////////////////////////////////
class LoginEvent extends DBTable{
	
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
//	출석이벤트
////////////////////////////////////////////////////////////////////////////////////////
class AttendenceEvent extends DBTable{
	
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
//	쿠폰코드
////////////////////////////////////////////////////////////////////////////////////////
class CuponCode extends DBTable{
	
	public function __construct($memberID=null,$fNo=null){
		
		parent::__construct();
		
		$this->setPrimarykey("no",true);

		$this->setDBInfo(DBManager::get()->getMainDBInfo());

		if($memberID && $fNo){
			parent::load("memberID=".$memberID." and no=".$fNo);
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

	public function getTopRank($start=0,$count=50){
		$rdata = array(); 
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
			$this->regDate=TimeManager::get()->getCurrentDateString();
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
		$alluser=$mresult[0]+1;
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
			parent::load($q_where);
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////
//	캐릭터히스토리
////////////////////////////////////////////////////////////////////////////////////////
class CharacterHistory extends DBTable{
	
	public function __construct($memberID=null,$characterNo=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);


		if($memberID){
			$this->m__userIndex = UserIndex::create($memberID);
			$this->setDBInfo($this->m__userIndex->getShardDBInfo());
			$this->memberID=$memberID;
		}
		
		if($memberID && $characterNo){
			$q_where = "memberID=".$memberID." and characterNo=".$characterNo;
			parent::load($q_where);
		}
	}


	public function getHistory($param){
		$dataList = array();

	    while($rData = self ::getRowByQuery("where memberID='".$param["id"]."'")){
			$character = new Character($rData["characterNo"]);
			$rData["name"]=$character->name;
			$dataList[]=$rData;

	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
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


	public function getList($param){
		$dataList = array();

	    while($rData = self::getRowByQuery()){
			$dataList[]=$rData;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    $result["result"]="ok";
	    return $result;
	}


	public function writeData($param){

		parent::load("no=".$param["data"]["no"]);

		foreach ($param["data"] as $key => $value) {
			$this->$key = $value;
		}

		if($this->save()){
			$r["data"] = $this->getArrayData(true);
			$r["result"]="ok";
			return $r;
		}

		$r["result"]="fail";
		return $r;
	}


	public function updateData($param){

		parent::load("no=".$param["data"]["no"]);

		unset($param["data"]["no"]);

		if(!$this->isLoaded()){
			$r["result"]="fail";
			$r["message"]="로드실패";
			return $r;
		}

		foreach ($param["data"] as $key => $value) {
			$this->$key = $value;
		}

		if($this->save()){
			$r["data"] = $this->getArrayData(true);
			$r["result"]="ok";
			return $r;
		}

		$r["result"]="fail";
		return $r;
	}


	public function deleteData($param){

		parent::load("no=".$param["data"]["no"]);

		if(!$this->isLoaded()){
			$r["result"]="fail";
			return $r;
		}

		if($this->remove()){
			$r["result"]="ok";
			return $r;

		}

		$r["result"]="fail";
		return $r;


	}
}



////////////////////////////////////////////////////////////////////////////////////////
//	샵
////////////////////////////////////////////////////////////////////////////////////////


class Shop extends DBTable{
public function __construct($pID=null){

		parent::__construct();
		
		$this->setPrimarykey("no",true);


		$this->setDBInfo(DBManager::get()->getMainDBInfo());
		
		if($pID){
			$q_where = "pID='".$pID."'";
			parent::load($q_where);
		}
	}
}
?>