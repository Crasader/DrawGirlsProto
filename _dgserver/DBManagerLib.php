<?php
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class TimeManager{
	public $m_timeOffset=32400;
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

	static public function getMicroTime() 
	{ 
	  list($usec, $sec) = explode(" ", microtime()); 
	  return ((float)$usec + (float)$sec + $m_timeOffset); 
	} 
	
	public function getCurrentWeekNo(){
		$y = date("Y",$this->getTime());
		$w = date("W",$this->getTime());
		if($w<10)$w="0".$w;
		
		return $y.$w;
	}
	
	public function getRemainTimeForWeeklyRank(){
		return strtotime("next Sunday")-time();
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
		if(!$this->isLocked()){
			$dInfo = debug_backtrace();
			$dInfoSize = count($dInfo);
		
			$dInfoSize = $dInfoSize-2;
			if($dInfoSize<0)$dInfoSize=0;
			$php_self = explode("/", $dInfo[$dInfoSize]["file"]);
			$url_cnt = count($php_self) - 1;
			$this_page = $php_self[$url_cnt];
			
			$this->m_logList[]=$this_page."(".$dInfo[$dInfoSize]["line"].")@".$dInfo[$dInfoSize]["class"]."::".$dInfo[$dInfoSize]["function"]." : ".$log;

		}
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
			ResultState::addResultCode(2007,"GDNEEDJOIN");
			ResultState::addResultCode(2008,"GDDUPLICATEDNICK");
			ResultState::addResultCode(2009,"GDFAULTYNICK");
			ResultState::addResultCode(2010,"GDALREADYMEMBER");
			ResultState::addResultCode(2011,"GDLONGNAME");
			ResultState::addResultCode(2012,"GDSHORTNAME");
			ResultState::addResultCode(2013,"GDFAILTRANJACTION");
			ResultState::addResultCode(2014,"GDDONTSAVE");
			ResultState::addResultCode(2015,"GDPROPERTYISMINUS");


			//3000 httpgateway
			ResultState::addResultCode(3001,"GDHTTPGATEWAYERROR");

			//4000 보안관련
			ResultState::addResultCode(4001,"GDCMDNOERROR");
			ResultState::addResultCode(4002,"GDDUPLICATEDDEVICE");
			ResultState::addResultCode(4003,"GDLONGTIME");
						
						
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

	public static function successCheck($result){
		if($result["code"]==ResultState::getResultCode("GDSUCCESS"))return true;

		return false;
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
	public  $m_shardDBInfoList=array();
	public  $m_shardDBCount=0;
	public  $m_mainDBInfo=array();
	public static $m_gameNo=null;
	public static $m_httpgateway=null;
	public static $m_gameID=null;
	public static $m_mainTables=array();
	public static $m_shardTables=array();
	public static $m_secretKey;
	private static $m_instance;
	
	//초기화
	public function __construct(){



		//테이블 설정		
		self::$m_shardTables["userdata"]="UserDataTable";
		self::$m_shardTables["message"]="MessageTable";
		self::$m_shardTables["weeklyscore"]="WeeklyScoreTable";
		self::$m_shardTables["stagescore"]="StageScoreTable";
		self::$m_shardTables["userlog"]="UserLogTable";
		self::$m_shardTables["puzzlehistory"]="PuzzleHistory";
		self::$m_shardTables["piecehistory"]="PieceHistory";
		self::$m_shardTables["cardhistory"]="CardHistory";

		self::$m_shardTables["archivementhistory"]="ArchivementHistory";
		self::$m_shardTables["giftboxhistory"]="GiftBoxHistory";
		self::$m_shardTables["cuponhistory"]="CuponHistory";
		self::$m_shardTables["hearthistory"]="HeartHistory";
		self::$m_shardTables["rubyhistory"]="RubyHistory";
		self::$m_shardTables["goldhistory"]="GoldHistory";
		self::$m_shardTables["itemhistory"]="ItemHistory";
		self::$m_shardTables["userproperty"]="UserProperty";
		self::$m_shardTables["userpropertyhistory"]="UserPropertyHistory";
		self::$m_shardTables["characterhistory"]="CharacterHistory";

		self::$m_mainTables["userindex"]="aUserShardIndex";
		self::$m_mainTables["commonsetting"]="aCommonSetting";
		self::$m_mainTables["notice"]="aNoticeTable";
		self::$m_mainTables["card"]="aCardTable";
		self::$m_mainTables["item"]="aItemTable";
		self::$m_mainTables["mission"]="aMissionTable";
		self::$m_mainTables["monster"]="aMonsterTable";
		self::$m_mainTables["pattern"]="aPatternTable";
		self::$m_mainTables["theme"]="aThemeTable";
		self::$m_mainTables["language"]="aLanguageTable";
		self::$m_mainTables["image"]="aImageTable";
		self::$m_mainTables["checkpoint"]="aCheckpointTable";
		self::$m_mainTables["monstermovement"]="aMonsterMovementTable";
		self::$m_mainTables["eventstage"]="aEventStageTable";
		self::$m_mainTables["puzzle"]="aPuzzleTable";
		self::$m_mainTables["book"]="aBookTable";
		self::$m_mainTables["piece"]="aPieceTable";
		self::$m_mainTables["stage"]="aStageTable";
		self::$m_mainTables["character"]="aCharacterTable";
		self::$m_mainTables["keyintvalue"]="aKeyIntValue";
		self::$m_mainTables["keystringvalue"]="aKeyStringValue";
		self::$m_mainTables["shop"]="aShopTable";
		self::$m_mainTables["worklist"]="aWorkList";
		self::$m_mainTables["formsetter"]="aFormSetter";
		self::$m_mainTables["userdata"]="UserDataTable";
		self::$m_mainTables["logtemp"]="logTempTable";
		self::$m_mainTables["message"]="messageTable";
		self::$m_mainTables["faultynick"]="aFaultyNickTable";


		self::$m_mainTables["archivement"]="aArchivementTable";
		self::$m_mainTables["cuponcode"]="aCuponCodeTable";
		self::$m_mainTables["cuponmanager"]="aCuponManageTable";
		self::$m_mainTables["missionevent"]="aMissionEventTable";
		self::$m_mainTables["attendenceevent"]="aAttendenceEventTable";
		self::$m_mainTables["loginevent"]="aLoginEventTable";
	}
	
	public function setDataBase($gameid=null){
		self::$m_gameID = $gameid;
		self::$m_gameNo = 10226;
		self::$m_httpgateway["URL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";
		self::$m_httpgateway["helpURL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/help.json";
		self::$m_httpgateway["version"]="1.3";
		self::$m_httpgateway["port"]="18080";
		self::$m_secretKey = "12345678";

		if(!$gameid and $gameid=="drawgirls"){


			//서버설정
			$serverInfo = new ServerInfo("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");
			$server0Index = $this->addServerInfo($serverInfo);
			
			//메인db설정
			$this->m_mainDBInfo = new DBInfo("drawgirls",$this->getServerInfo($server0Index));
			
			//샤드db설정
			$this->m_shardDBInfoList[]=new DBInfo("dg001",$this->getServerInfo($server0Index));
			$this->m_shardDBInfoList[]=new DBInfo("dg002",$this->getServerInfo($server0Index));
			
			$this->m_shardDBCount = count($this->m_shardDBInfoList);

			LogManager::get()->addLog("dbmanager use drawgirls db");
		}else if($gameid == "drawgirls_tstore"){
			//서버설정
			$serverInfo = new ServerInfo("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");
			$server0Index = $this->addServerInfo($serverInfo);
			
			//메인db설정
			$this->m_mainDBInfo = new DBInfo("drawgirls",$this->getServerInfo($server0Index));
			
			//샤드db설정
			$this->m_shardDBInfoList[]=new DBInfo("dg001",$this->getServerInfo($server0Index));
			$this->m_shardDBInfoList[]=new DBInfo("dg002",$this->getServerInfo($server0Index));
			
			$this->m_shardDBCount = count($this->m_shardDBInfoList);

			LogManager::get()->addLog("dbmanager use drawgirls_tstore db");
		}else{
			
			$serverInfo = new ServerInfo("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");
			$server0Index = $this->addServerInfo($serverInfo);
			
			//메인db설정
			$this->m_mainDBInfo = new DBInfo("drawgirls",$this->getServerInfo($server0Index));
			
			//샤드db설정
			$this->m_shardDBInfoList[]=new DBInfo("dg001",$this->getServerInfo($server0Index));
			$this->m_shardDBInfoList[]=new DBInfo("dg002",$this->getServerInfo($server0Index));
			
			$this->m_shardDBCount = count($this->m_shardDBInfoList);

			LogManager::get()->addLog("dbmanager use nothing or $gameID db");
		}
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

	public function getMainDBInfo(){
		return $this->m_mainDBInfo;
	}
	public function getDBInfoByShardKey($shardKey){
	
		return $this->getDBInfoByShardIndex($this->getDBIndexByShardKey($shardKey));
	}
	
	public function getDBInfoByShardIndex($index){
		return $this->m_shardDBInfoList[$index];
	}

	public function getConnectionByShardKey($shardKey){
		
		
		$conn = $this->getConnectionByShardIndex($this->getDBIndexByShardKey($shardKey));

		return $conn;
	}

	public function getConnectionByShardIndex($index){
		$conn = $this->m_shardDBInfoList[$index]->getConnection();
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
		foreach($this->m_shardDBInfoList as &$dbInfo){
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

	public function getShardDBInfoList(){
		return $this->m_shardDBInfoList;
	}
	//static
	

	public static function getMT($id){
		if(!is_scalar($id)){
			$id = get_class($id);
		}

		$id = strtolower($id);
		return self::$m_mainTables[$id];
	}
	
	public static function getST($id){
		if(!is_scalar($id)){
			$id = get_class($id);
		}

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
////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DBRow{
	public $m__DBTable=NULL;
	public $m__primarykey=NULL;
	public $m__result;
	public $m__DBInfo;
	public $m__isLoaded=false;
	public $m__isMainClass=true;
	static public $m__qResult = null;
	static public $m__qCnt = 0;

	
	static public function isMainDBClass(){
		if(DBManager::getST(get_called_class()))return false;

		return true;
	}

	public function __construct(){
		$this->m__DBTable = DBManager::getST(get_called_class());
		$this->m__isMainClass = false;

		if(!$this->m__DBTable){
			$this->m__DBTable = DBManager::getMT(get_called_class());
			$this->m__isMainClass=true;
		}
		LogManager::get()->addLog("db table auto select by ".get_called_class()." ==>".$this->m__DBTable);
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

	public function setField($field,$value){
		$field = "m_".$field;
		$this->$field = $value;
	}

	public function getField($field){
		$field = "m_".$field;
		return $this->$field;
	}

	public function __get($name) {
	    if ( property_exists($this, $name) ) {
	        return $this->{$name};
	    }
	    $m_name = "m_{$name}";
	    if ( property_exists($this, $m_name) ) {
	        return $this->{$m_name};
	    }
	 
	    trigger_error("Undefined property $name or method $method_name");
	}
	 
	public function __set($name, $value) {
	    if ( property_exists($this, $name) ) {
	        $this->{$name} = $value;
	        return;
	    }
	    $m_name = "m_{$name}";
	    if ( property_exists($this, $m_name) ) {
	        $this->{$m_name}=$value;
	        return;
	    }
	 
		$dInfo = debug_backtrace();
	    trigger_error($dInfo[0]['file']."(".$dInfo[0]['line'].")"."Undefined property $name or method $method_name");
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
	
	public function insertQuery($isIncludePrimaryKey=false){
		return DBManager::insertQuery($this->m__DBTable,$this->getArrayData($isIncludePrimaryKey));
	}
	
	public function save($isIncludePrimaryKey=false){
		LogManager::get()->addLog("save function start");
		if(!$this->m__DBInfo){

			LogManager::get()->addLog("save function fail, undefined m__DBInfo");
			return false;
		}

		if($this->m__DBInfo->getConnection()){
			$query="";
			if($this->isLoaded()){
				//update
				$query = $this->updateQuery();
				LogManager::get()->addLog("save make update query ".$query);
			}else{
				//insert
				$query = $this->insertQuery($isIncludePrimaryKey);
				LogManager::get()->addLog("save make insert query ".$query);
			}
			
			if(mysql_query($query,$this->m__DBInfo->getConnection())){
				LogManager::get()->addLog("save query ok".mysql_error());
				return true;
			}else{
				LogManager::get()->addLog("save query fail".mysql_error());
				return false;
			}
		}
		LogManager::get()->addLog("save fail, m__DBInfo->getConnection() is false");
		return false;
	}
	
	public function load($where){
		LogManager::get()->addLog("load start");
		if(!$this->m__DBTable)return false;
		if(!$this->m__DBInfo)return false;
		if(!$this->m__DBInfo->getConnection())return false;
		if(!$where && !$this->getPrimaryValue())return false;
		
		$query = "select * from ".$this->m__DBTable." where `".$this->m__primarykey."`='".$this->getPrimaryValue()."'";
		
		if($where){
			$query = "select * from ".$this->m__DBTable." where ".$where;
		}
		
		LogManager::get()->addLog("load query ->".$query);

		$result=mysql_query($query,$this->m__DBInfo->getConnection());

		if(mysql_error() || mysql_affected_rows()<1){
			
			LogManager::get()->addLog("load fail ->".mysql_error()." affected_rows is ".mysql_affected_rows());
			return false;
		}

		if($result)$this->m__result = mysql_fetch_array($result,MYSQL_ASSOC);
		
		if($this->m__result && $this->m__result[$this->m__primarykey]){
			LogManager::get()->addLog("load ok, primarykey is ".$this->m__primarykey." and value is ".$this->m__result[$this->m__primarykey]);
			$this->m__isLoaded=true;
			return true;
		}
		
		LogManager::get()->addLog("load fail");
		return false;
	}
	
	public function remove(){
		LogManager::get()->addLog("remove function start");
		if($this->isLoaded()){
			$query = "DELETE FROM ".$this->m__DBTable." WHERE `".$this->m__primarykey."`='".$this->getPrimaryValue()."'";
			$result = mysql_query($query,$this->m__DBInfo->getConnection());
			if(!mysql_error()){
				LogManager::get()->addLog("remove success with query -> ".$query);
				return true;
			}
		}

		LogManager::get()->addLog("remove fail");
		return false;
	}
	public function setDBInfo($dbInfo){
		$this->m__DBInfo = $dbInfo;
	}

	public function getDBConnection(){
		return $this->m__DBInfo->getConnection();
	}
	
	public function setPrimarykey($pkey){
		$this->m__primarykey=$pkey;
	}
	
	public function setDBTable($dbTable){
		$this->m__DBTable = $dbTable;
	}

	public function autoMatching($data){
		foreach($data as $key => $value){
			$this->{"m_".$key} = $value;
		}
	}

	static public function getShardDBInfoList(){
		return UserIndex::getShardDBInfoList();
	}

	static public function getRowByQuery($where="",$dbcon=NULL){
		if(self::isMainDBClass()){
			if(!self::$m__qResult)self::$m__qResult = mysql_query("select * from ".DBManager::getMT(get_called_class())." ".$where,DBManager::get()->getMainConnection());
			$result = mysql_fetch_array(self::$m__qResult,MYSQL_ASSOC);
			if(!$result)self::$m__qResult=null;
			return $result;
		}

		$dbInfoList = self::getShardDBInfoList();
		if(self::$m__qCnt>=count($dbInfoList)){
			self::$m__qCnt=0;
			return false;
		}
		
		if(!self::$m__qResult){
			if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
			else $dbconn = $dbcon;

			self::$m__qResult = mysql_query("select * from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
		}

		$result = mysql_fetch_array(self::$m__qResult,MYSQL_ASSOC);

		if($result || $dbcon!=NULL)return $result;
		
		self::$m__qResult=null;
		self::$m__qCnt++;

		return self::getRowByQuery($where);
	}

	static public function removeRowByQuery($where="",$dbcon=NULL){
		if(!$where)return;

		if(self::isMainDBClass()){
			$result=mysql_query("delete from ".DBManager::getMT(get_called_class())." ".$where,DBManager::get()->getMainConnection());
			return;
		}

		$dbInfoList = self::getShardDBInfoList();
		for($i=0;$i<count($dbInfoList);$i++){
			$dbconn = $dbInfoList[$i]->getConnection();
			$result=mysql_query("delete from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
		}

		return;

		// $dbInfoList = self::getShardDBInfoList();
		// if(self::$m__qCnt>=count($dbInfoList)){
		// 	self::$m__qCnt=0;
		// 	return false;
		// }
		
		// if(!self::$m__qResult){
		// 	if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
		// 	else $dbconn = $dbcon;

		// 	self::$m__qResult = mysql_query("delete from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
		// 	LogManager::get()->addLog("removeRowByQuery : ".self::$m__qResult);
		// }

		// if($dbcon!=NULL)return $result;
		
		// self::$m__qResult=null;
		// self::$m__qCnt++;

		// return self::getRowByQuery($where);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DBTable{
	public $m__DBTable=NULL;
	public $m__primarykey=NULL;
	public $m__autoIncreaseKey=NULL;
	public $m__data;
	public $m__DBInfo;
	public $m__isLoaded=false;
	public $m__isMainClass=true;
	static public $m__qResult = null;
	static public $m__qCnt = 0;

	
	static public function isMainDBClass(){
		if(DBManager::getST(get_called_class()))return false;

		return true;
	}

	public function __construct(){
		$this->m__DBTable = DBManager::getST(get_called_class());
		$this->m__isMainClass = false;

		if(!$this->m__DBTable){
			$this->m__DBTable = DBManager::getMT(get_called_class());
			$this->m__isMainClass=true;
		}
		LogManager::get()->addLog("db table auto select by ".get_called_class()." ==>".$this->m__DBTable);
	}
	
	public function getPrimaryKey(){
		return $this->m__primarykey;
	}
	
	public function getPrimaryValue(){
		$pkey = $this->m__primarykey;
		if($pkey){
			return $this->m__data[$pkey];
		}
		
		return null;
	}
	
	public function isLoaded(){		
		return $this->m__isLoaded;
	}

	public function getArrayData($isIncludePrimaryKey=false,$keyList=null){


		$arraydata=array();

		foreach($this->m__data as $name=>$value){
			if(!$keyList || in_array($name,$keyList)){
					$arraydata[$name]=$this->m__data[$name];
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
	
	public function insertQuery($isIncludePrimaryKey=false){
		return DBManager::insertQuery($this->m__DBTable,$this->getArrayData($isIncludePrimaryKey));
	}
	
	public function save($isIncludePrimaryKey=false){



		LogManager::get()->addLog("DBRow save function");
		if(!$this->m__DBInfo)return false;

		if($this->m__DBInfo->getConnection()){
			$query="";
			$isInsert = false;
			if($this->isLoaded()){
				//update
				$query = $this->updateQuery();
				LogManager::get()->addLog("make update query ".$query);
			}else{
				//insert
				$query = $this->insertQuery($isIncludePrimaryKey);
				LogManager::get()->addLog("make insert query ".$query);
				$isInsert=true;
			}
			
			if(mysql_query($query,$this->m__DBInfo->getConnection())){
				LogManager::get()->addLog("query ok ");
				if($isInsert && $this->m__autoIncreaseKey){
					$this->m__data[$this->m__autoIncreaseKey]=mysql_insert_id($this->m__DBInfo->getConnection());
				}
				return true;
			}else{
				LogManager::get()->addLog("query fail ".mysql_error());
				return false;
			}
		}
		LogManager::get()->addLog("dginfo fail");
		return false;
	}
	
	public function load($where){
		if(!$this->m__DBTable)return false;
		if(!$this->m__DBInfo)return false;
		if(!$this->m__DBInfo->getConnection())return false;
		if(!$where && !$this->getPrimaryValue())return false;
		
		$query = "select * from ".$this->m__DBTable." where `".$this->m__primarykey."`='".$this->getPrimaryValue()."'";
		
		if($where){
			$query = "select * from ".$this->m__DBTable." where ".$where;
		}
		
		LogManager::get()->addLog("load query ".$query);

		$result=mysql_query($query,$this->getDBConnection());
		if($result)$this->m__data = mysql_fetch_array($result,MYSQL_ASSOC);
		

		if($this->m__data){
			LogManager::get()->addLog("load ok");
			$this->m__isLoaded=true;
			return true;
		}
		
		LogManager::get()->addLog("load fail".mysql_error());
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

	public function getDBInfo(){
		return $this->m__DBInfo;
	}

	public function getDBConnection(){
		return $this->m__DBInfo->getConnection();
	}
	
	public function setPrimarykey($pkey,$isAutoincrease=false){
		$this->m__primarykey=$pkey;
		if($isAutoincrease)$this->setAutoIncreaseKey($pkey);
	}
	
	public function setAutoIncreaseKey($pkey){
		$this->m__autoIncreaseKey=$pkey;
	}
	
	public function setDBTable($dbTable){
		$this->m__DBTable = $dbTable;
	}


	public function getField($key){
		return $this->m__data[$key];
	}

	public function setField($key,$value){
		$this->m__data[$key]=$value;
	}

	public function __get($name) {
	    if ( property_exists($this, $name) ) {
	        return $this->{$name};
	    }

	   	return $this->m__data[$name];
	}
	 
	public function __set($name, $value) {
	    if ( property_exists($this, $name) ) {
	        $this->{$name} = $value;
	        return;
	    }
	    $this->m__data[$name]=$value;
		return;	 
	    trigger_error("Undefined property $name or method $method_name");
	}

	static public function getShardDBInfoList(){
		return UserIndex::getShardDBInfoList();
	}

	static public function getQueryResult($query,$dbcon=NULL){
		if(self::isMainDBClass()){
		LogManager::get()->addLog("getqueryresult2->".$query);
			$result = mysql_query($query,DBManager::get()->getMainConnection());
			return $result;
		}

		$dbInfoList = self::getShardDBInfoList();


		if(self::$m__qCnt>=count($dbInfoList)){
			self::$m__qCnt=0;
			return false;
		}
		
		if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
		else $dbconn = $dbcon;


		LogManager::get()->addLog("getqueryresult2->".$query);
		$result = mysql_query($query,$dbconn);


		self::$m__qCnt++;
		self::$m__qResult=null;
		
		return $result;
	}

	static public function getRowByQuery($where="",$dbcon=NULL){
		if(self::isMainDBClass()){
			if(!self::$m__qResult)self::$m__qResult = mysql_query("select * from ".DBManager::getMT(get_called_class())." ".$where,DBManager::get()->getMainConnection());
			$result = mysql_fetch_array(self::$m__qResult,MYSQL_ASSOC);
			if(!$result)self::$m__qResult=null;
			return $result;
		}

		$dbInfoList = self::getShardDBInfoList();
		if(self::$m__qCnt>=count($dbInfoList)){
			self::$m__qCnt=0;
			return false;
		}
		
		if(!self::$m__qResult){
			if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
			else $dbconn = $dbcon;

			self::$m__qResult = mysql_query("select * from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
			LogManager::get()->addLog("ok go select * from ".DBManager::getST(get_called_class())." ".$where);
		}

		$result = mysql_fetch_array(self::$m__qResult,MYSQL_ASSOC);

		if($result || $dbcon!=NULL)return $result;
		
		self::$m__qResult=null;
		self::$m__qCnt++;

		return self::getRowByQuery($where);
	}

	static public function removeRowByQuery($where="",$dbcon=NULL){
		if(!$where)return;

		if(self::isMainDBClass()){
			$result=mysql_query("delete from ".DBManager::getMT(get_called_class())." ".$where,DBManager::get()->getMainConnection());
			return;
		}

		$dbInfoList = self::getShardDBInfoList();
		for($i=0;$i<count($dbInfoList);$i++){
			$dbconn = $dbInfoList[$i]->getConnection();
			$result=mysql_query("delete from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
		}

		return;

		// $dbInfoList = self::getShardDBInfoList();
		// if(self::$m__qCnt>=count($dbInfoList)){
		// 	self::$m__qCnt=0;
		// 	return false;
		// }
		
		// if(!self::$m__qResult){
		// 	if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
		// 	else $dbconn = $dbcon;

		// 	self::$m__qResult = mysql_query("delete from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
		// 	LogManager::get()->addLog("removeRowByQuery : ".self::$m__qResult);
		// }

		// if($dbcon!=NULL)return $result;
		
		// self::$m__qResult=null;
		// self::$m__qCnt++;

		// return self::getRowByQuery($where);
	}
}
?>