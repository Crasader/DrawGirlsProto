<?php

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ResultState{
	public $m_code;
	public $m_name;
	public $m_message;
	
	static public $m_resutlCodeList=null;
	
	public function __construct($code=1,$message=""){
		
		if(ResultState::$m_resutlCodeList == null){
			ResultState::addResultCode(0,"GDUNKNOWNRESULT");
			ResultState::addResultCode(1,"GDSUCCESS");
			ResultState::addResultCode(2,"GDDONTMAKETHREAD");
			ResultState::addResultCode(3,"GDSECURITY");
			ResultState::addResultCode(4,"GDCHECKNETWORK");
			
			//1000번대 mysql 관련 오류
			ResultState::addResultCode(1001,"GDMYSQLQUERY");
			ResultState::addResultCode(1002,"GDMYSQLCONNECTION");
			ResultState::addResultCode(1003,"GDMYSQLFETCH");
			
			//2000번대
			ResultState::addResultCode(2001,"GDSAMEVERSION");
			ResultState::addResultCode(2002,"GDPARAMETER");
			ResultState::addResultCode(2003,"GDDONTFIND");
			ResultState::addResultCode(2004,"GDFRIENDMAX");
			ResultState::addResultCode(2005,"GDDONTFINDUSER");
			ResultState::addResultCode(2006,"GDFAILTOSAVEUSERDATA");
						
						
			//return ResultState::makeReturn(1002,"fail to get shardConnection");
			// $r["result"]=ResultState::successToArray();
			// $r["result"]=ResultState::toArray(2002,"memberID");

		}
		
		$this->m_code=$code;
		$this->m_name=ResultState::getResultName($code);
		$this->m_message=$message;
	}
	
	public function isSuccess(){
		if($this->m_code==1)return true;
		return false;
	}
	
	public function getJson(){
		$a = $this->getArray();
		return json_encode($a,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	}
	
	public function getArray(){
		$a["code"]=$this->code;
		$a["name"]=$this->name;
		$a["message"]=$this->message;
		return $a;
	}
	
	public static function addResultCode($code,$name){
		ResultState::$m_resutlCodeList[$code]=$name;
	}
	
	public static function getResultName($code){
		return ResultState::$m_resutlCodeList[$code];
	}
	
	public static function toJson($code,$message=""){
		$a = ResultState::toArray($code,$message);
		return json_encode($a,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);		
	}
	
	public static function getResultCode($name){
		foreach(ResultState::$m_resutlCodeList as $key=>$value){
			if($name==$value)return $key;
		}
	}
	public static function toArray($id,$message=""){
	
		if(is_numeric($id)){
			$a["code"]=$id;
			$a["name"]=ResultState::getResultName($id);
			if($message)$a["message"]=$message;
			return $a;
		}else{
			$a["name"]=$id;
			$a["code"]=ResultState::getResultCode($id);
			if($message)$a["message"]=$message;
			return $a;
		}
	}
	
	public static function successToArray(){
		$a["code"]=1;
		return $a;
	}
	
	public static function makeReturn($code,$message=""){
		$a = ResultState::toArray($code,$message);
		$r["result"] = $a;
		return $r;
	}
}

new ResultState();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ServerInfo{
	public $m_host=null;
	public $m_id=null;
	public $m_password=null;
	public $m_connection=null;
	
	public function __construct($host,$id,$password){
		$this->setServerInfo($host,$id,$password);
	}
	
	public function setServerInfo($host,$id,$password){
		$this->m_host=$host;
		$this->m_id=$id;
		$this->m_password=$password;
	}
	
	public function getConnection(){
		if(!$this->m_connection)$this->m_connection = mysql_connect($this->m_host,$this->m_id,$this->m_password);
		
		return $this->m_connection;
	}
	
	public function closeConnection(){
		if($this->m_connection){
			mysql_close($this->m_connection);
			$this->m_connection=null;
		}	
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class DBInfo{
	public $m_name = null;
	public $m_server = null;
	
	public function __construct($name,$server){
		$this->m_name=$name;
		$this->m_server=$server;
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

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DBManager{
	public  $m_serverInfo=array();
	public  $m_shardDBInfo=array();
	public  $m_shardDBCount=0;
	public  $m_mainDBInfo=array();
	public static $m_mainTables=array();
	public static $m_shardTables=array();
	private static $m_instance;
	
	//초기화
	public function __construct(){

		//서버설정
		$serverInfo = new ServerInfo("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");
		$server0Index = $this->addServerInfo($serverInfo);
		
		//메인db설정
		$this->m_mainDBInfo = new DBInfo("drawgirls",$this->getServerInfo($server0Index));
		
		//샤드db설정
		$this->m_shardDBInfo[]=new DBInfo("dg001",$this->getServerInfo($server0Index));
		$this->m_shardDBInfo[]=new DBInfo("dg002",$this->getServerInfo($server0Index));
		
		$this->m_shardDBCount = count($this->m_shardDBInfo);

		//테이블 설정		
		self::$m_shardTables["userdata"]="UserDataTable";
		self::$m_shardTables["message"]="MessageTable";
		self::$m_shardTables["weeklyscore"]="WeeklyScoreTable";
		self::$m_shardTables["stagescore"]="StageScoreTable";
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
  
	//샤드서버추가
	public function addServerInfo($server){
		$this->m_serverInfo[]=$server;
		return count($this->m_serverInfo)-1;
	}
	
	public function getServerInfo($index){
		return $this->m_serverInfo[$index];
	}

	public function getDBInfoByShardKey($shardKey){
	
		return $this->m_shardDBInfo[$this->getDBIndexByShardKey($shardKey)];
	}
	
	public function getConnectionByShardKey($shardKey){
		
		LogManager::get()->addLog("getConnectionByShardKey, $shardKey -> dbindex:".$this->getDBIndexByShardKey($shardKey)." / db count is".$this->m_shardDBCount);
		$conn = $this->m_shardDBInfo[$this->getDBIndexByShardKey($shardKey)]->getConnection();

		return $conn;
	}
		
	public function getMainConnection(){ 
		return $this->m_mainDBInfo->getConnection();
	}

	
	public function getDBIndexByShardKey($shardKey){
		return $shardKey%$this->m_shardDBCount;
	}
	
	public function closeShardDB(){
		//for문 돌면서 close
		foreach($this->m_shardDBInfo as &$dbInfo){
			$dbInfo->closeConnection();
		}
	}
	
	public function closeMainDB(){
		//close
		$this->m_mainDBInfo->closeConnection();
	}
	
	public function closeDB(){
		$this->closeMainDB();
		$this->closeShardDB();
	}

	//static
	

	public static function getMT($id){
		$id = strtolower($id);
		return self::$m_mainTables[$id];
	}
	
	public static function getST($id){
		$id = strtolower($id);
		return self::$m_shardTables[$id];
	}
	
	public static function insertQuery($table,$data){ 
		$key=array_keys ($data);  
		$query="insert into `$table` ("; 
			for($i=0;$i<count($key);$i++){ 
				$query.="`".$key[$i]."`"; 
				if($i!=count($key)-1) $query.=","; 
			} 
	
		$query.=") values ("; 
		for($i=0;$i<count($key);$i++){ 
			if(is_array($data[$key[$i]]))$data[$key[$i]] = json_encode($data[$key[$i]],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$query.="'".addslashes($data[$key[$i]])."'"; 
			if($i!=count($key)-1) $query.=","; 
		} 
		$query.=")"; 
		return $query; 
	} 

	public static function updateQuery($table,$data,$where){
		$key=array_keys($data); 
		$query="update $table set ";
		for($i=0;$i<count($key);$i++){
			if(is_array($data[$key[$i]]))$data[$key[$i]] = json_encode($data[$key[$i]],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			
			$query.="`$key[$i]`=\"".addslashes($data[$key[$i]])."\"";
			if($i!=count($key)-1) $query.=",";
		}
		$query.=" ".$where;
	
		return $query;
	}
}

DBManager::get();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class DBRow{
	public $m__DBTable=NULL;
	public $m__primarykey=NULL;
	public $m__result;
	public $m__DBInfo;
	public $m__isLoaded=false;
	
	public function __construct(){

	}
	
	public function getPrimaryKey(){
		return $this->m__primarykey;
	}
	
	public function getPrimaryValue(){
		$pkey = $this->m__primarykey;
		if($pkey){
			return $this->{"m_".$pkey};	
		}
		
		return null;
	}
	
	public function isLoaded(){		
		return $this->m__isLoaded;
	}

	public function getArrayData($isIncludePrimaryKey=false,$keyList=null){
		$arraydata=array();
		$class_vars = get_class_vars(get_class($this));
		foreach($class_vars as $name=>$value){
			if(!strpos($name,"__")){
				$fieldname = str_replace("m_","",$name);
				if(!$keyList|| in_array($fieldname,$keyList)){
					$arraydata[$fieldname]=$this->$name;
				}
			}
		}
		
		if($this->m__primarykey && !$isIncludePrimaryKey){
			unset($arraydata[$this->m__primarykey]);
		}
		
		return $arraydata;
	}
	
	public function updateQuery($where=false){
		if(!$where)$where = "where ".$this->m__primarykey."=".$this->getPrimaryValue();
		return DBManager::updateQuery($this->m__DBTable,$this->getArrayData(),$where);
	}
	
	public function insertQuery(){
		return DBManager::insertQuery($this->m__DBTable,$this->getArrayData());
	}
	
	public function save(){
		LogManager::get()->addLog("DBRow save function");
		if($this->m__DBInfo->getConnection()){
			$query="";
			if($this->isLoaded()){
				//update
				$query = $this->updateQuery();
			}else{
				//insert
				$query = $this->insertQuery();
			}
			
			if(mysql_query($query,$this->m__DBInfo->getConnection())){
				LogManager::get()->addLog("query ok $query");
				return true;
			}else{
				LogManager::get()->addLog("query fail $query");
				return false;
			}
		}
		LogManager::get()->addLog("dginfo fail");
		return false;
	}
	
	public function load($where){
		if(!$this->m__DBTable)return false;
		if(!$this->m__DBInfo->getConnection())return false;
		if(!$where && !$this->getPrimaryValue())return false;
		
		$query = "select * from ".$this->m__DBTable." where `".$this->m__primarykey."`='".$this->getPrimaryValue()."'";
		
		if($where){
			$query = "select * from ".$this->m__DBTable." where ".$where;
		}
		
		$result=mysql_query($query,$this->m__DBInfo->getConnection());
		if($result)$this->m__result = mysql_fetch_array($result,MYSQL_ASSOC);
		
		if($this->m__result){
			$this->m__isLoaded=true;
			return true;
		}
		
		return false;
	}
	
	public function remove(){
		if($this->isLoaded()){
			$query = "DELETE FROM ".$this->m__DBTable." WHERE `".$this->m__primarykey."`='".$this->getPrimaryValue()."'";
			$result = mysql_query($query,$this->m__DBInfo->getConnection());
			if($result)return true;
		}
		return false;
	}
	public function setDBInfo($dbInfo){
		$this->m__DBInfo = $dbInfo;
	}
	
	public function setPrimarykey($pkey){
		$this->m__primarykey=$pkey;
	}
	
	public function setDBTable($dbTable){
		$this->m__DBTable = $dbTable;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class UserData extends DBRow{
	public $m_no=null;
	public $m_memberID=null;
	public $m_data=null;
	public $m_nick=null;
	public $m_lastDate=null;
	public $m_joinDate=null;
	public $m_friendList=null;
	public function __construct($memberID=null,$weekNo=null){
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBTable(DBManager::getST("userdata"));
		$this->m_memberID = $memberID;
		
		if($memberID)$this->setDBInfo(DBManager::get()->getDBInfoByShardKey($memberID));
		
		if($memberID){
			if(parent::load("memberID=".$memberID)){
				$this->m_no = $this->m__result["no"];
				$this->m_memberID = $this->m__result["memberID"];
				$this->m_data = $this->m__result["data"];
				$this->m_nick = $this->m__result["nick"];
				$this->m_lastDate = $this->m__result["lastDate"];
				$this->m_joinDate = $this->m__result["joinDate"];
				$this->m_friendList = $this->m__result["friendList"];
			}
		}
		//가입시간
		if(!$this->m_joinDate)$this->m_joinDate=TimeManager::get()->getCurrentDateString();
	}

	public function save(){
		//마지막접속시간
		$this->m_lastDate = TimeManager::get()->getCurrentDateString();
		return parent::save();
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
	
	
	public function updateData($data){
		//merge
		$resultData = "{}";
		if($this->m_data){
			$check = json_decode($this->m_data,true);
			if(is_array($check)){
				$oldJson = json_decode($this->m_data,true);
				$newJson = json_decode($data,true);
				$mergeJson =array_merge($oldJson,$newJson);
					
				$resultData = json_encode($mergeJson,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK); 
			}else{
				$resultData = $data;
			}
		}else{
			$resultData = $data;
		}	
		
		$this->m_data = $resultData;
		return $this->save();
	}
	
	public function isInDB(){
		if($this->m_no && $this->m_memberID)return true;
		
		return false;
	}
	
	public function getData(){
		return $this->m_data;
	}
	
	public function addFriend($friendID){
		LogManager::get()->addLog("addfriend ".$friendID);

		$friendList=array();
		if($this->m_friendList)$friendList = json_decode($this->m_friendList,true);
		
		$friendList[]=$friendID;
		
		$friendList = array_unique($friendList);		
		$this->m_friendList = json_encode($friendList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		LogManager::get()->addLog("addfriendResult is ".$this->m_friendList);
	}
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class Message extends DBRow{
	public $m_no=null;
	public $m_memberID=null;
	public $m_content=null;
	public $m_regDate=null;
	public $m_friendID=null;
	public $m_type=null;
	public $m_isSendMsg=null;
	
	public function __construct($memberID=null,$messageNo=null){
		parent::__construct();
		$this->setPrimarykey("no");
		$this->setDBTable(DBManager::getST("message"));
		
		if($memberID)$this->setDBInfo(DBManager::get()->getDBInfoByShardKey($memberID));
		
		if($memberID && $messageNo){
			if(parent::load("no = $messageNo")){
				$this->m_memberID = $this->m__result["memberID"];
				$this->m_no = $this->m__result["no"];
				$this->m_content = $this->m__result["conetnt"];
				$this->m_regDate = $this->m__result["regDate"];
				$this->m_friendID = $this->m__result["friendID"];
				$this->m_type = $this->m__result["type"];
				$this->m_isSendMsg = $this->m__result["isSendMsg"];
				$this->m_data = $this->m__result["data"];
			}
		}
	}
	

	public function send(){
		if(!$this->m_memberID)return "error";
		if(!$this->m__DBInfo){
			$this->m__DBInfo = DBManager::get()->getDBInfoByShardKey($this->m_memberID);
		}
		
		$result=mysql_query("insert into ".DBManager::getST("message")." (memberID,content,regDate,friendID,type,isSendMsg) values ('".$this->m_memberID."','".$this->m_content."','".$this->m_regDate."','".$this->m_friendID."','".$this->m_type."','".$this->m_isSendMsg."')",$this->m__DBInfo->getConnection());
		
		if(!$result)return "error";
		
		return 0;
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class WeeklyScore extends DBRow{
	public $m_no;
	public $m_memberID;
	public $m_score;
	public $m_regDate;
	public $m_regWeek; //yyyyw - (w:몇번째주) ex) 
	public $m_data;

	public function __construct($memberID=null,$weekNo=null){
		parent::__construct();
		
		$this->setPrimarykey("no");
		$this->setDBTable(DBManager::getST("weeklyScore"));
		LogManager::get()->addLog("weeklyScore is ".DBManager::getST("weeklyScore"));
		$this->setDBInfo(DBManager::get()->getDBInfoByShardKey($memberID));
		
		if($memberID && $weekNo){
			if(parent::load("memberID=".$memberID." and regWeek=".$weekNo)){
				$this->m_no = $this->m__result["no"];
				$this->m_memberID = $this->m__result["memberID"];
				$this->m_score = $this->m__result["score"];
				$this->m_regDate = $this->m__result["regDate"];
				$this->m_regWeek = $this->m__result["regWeek"];
				$this->m_data = $this->m__result["data"];
			}
		}
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class StageScore extends DBRow{
	public $m_no;
	public $m_memberID;
	public $m_stageNo;
	public $m_score;
	public $m_regDate;
	public $m_data;
	
	public function __construct($stageNo=null,$memberID=null,$where=null){
		
		if(!$stageNo)return false;
		
		parent::__construct();
		
		
		$this->setPrimarykey("no");
		$this->setDBTable(DBManager::getST("stageScore"));
		$this->setDBInfo(DBManager::get()->getDBInfoByShardKey($stageNo));
		
		if($memberID || $where){
			if($where)$q_where = $where;
			else $q_where = "memberID=".$memberID." and stageNo=".$stageNo;
			
			LogManager::get()->addLog($q_where);
			
			if(parent::load($q_where)){
				$this->m_no = $this->m__result["no"];
				$this->m_stageNo = $this->m__result["stageNo"];
				$this->m_memberID = $this->m__result["memberID"];
				$this->m_score = $this->m__result["score"];
				$this->m_regDate = $this->m__result["regDate"];
				$this->m_data = $this->m__result["data"];
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TimeManager{
	public $m_timeOffset=0;
	private static $m_instance=NULL;
	//싱글턴 얻어오기
	public static function get()
	{
	    if ( is_null( self::$m_instance ) )
	    {
	      self::$m_instance = new self();
	    }
	    return self::$m_instance;
	}
	
	public function setTimeOffset($offset){
		$this->m_timeOffset = $offset;
	}
	
	public function getTime(){
		return time()+$this->m_timeOffset;
	}
	
	public function getCurrentWeekNo(){
		$y = date("Y",$this->getTime());
		$w = date("W",$this->getTime());
		if($w<10)$w="0".$w;
		
		return $y.$w;
	}
	
	public function getWeekNo($timestamp){
		return date("W",$timestamp);
	}
	
	public function getDateString($timestamp){
		return date("YmdHis",$timestamp);
	}
	
	public function getCurrentDateString(){
		return $this->getDateString($this->getTime());
	}
	
  
}

class LogManager{
	public $m_logList=array();
	private static $m_instance=NULL;
	public $m_isLocked=false;
	//싱글턴 얻어오기
	public static function get()
	{
	    if ( is_null( self::$m_instance ) )
	    {
	      self::$m_instance = new self();
	    }
	    return self::$m_instance;
	}
	
	public function addLog($log){
		if(!$this->isLocked())$this->m_logList[]=$log;
	}
	
	public function getLog(){
		return $this->m_logList;
	}
	
	public function getLogAndClear(){
		$loglist = unserialize(serialize($this->m_logList));
		$this->m_logList=array();
		return $loglist;
	}
	
	public function setLock($locked){
		$this->m_isLocked=$locked;
	}
	
	public function isLocked(){
		return $m_isLocked;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
class dbRow{
	public function toArray(){
		// dbField 클래스 멤버검색후 어레이형식으로 만들기
	}
	
	public put()
}

class dbField{

	private $m_value=null;
	private $m_type="int";
	private $m_size=null;

	public function __construct($type){
		$this->m_type = $type;
	}
	
	public function get(){
		return $this->m_value;
	}
	
	public function set($value){
		$this->m_value = $value;
	}
	
}

class intProperty extends dbField{
	public function __construct($value=0,$size=11){
		super::__construct("int");
		$this->m_value = $value;
		$this->m_size = $size;
	}
}

modifytest2
*/

?>