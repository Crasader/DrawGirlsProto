<?php
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CurrentUserInfo{
	static public $os;
	static public $language="ko";
	static public $memberID;
	static public $socialID;
	static public $country="kr";
	static public $timezone="Asia/Seoul";

	static public function getLocalizedValueInData($data){
		return $data[CurrentUserInfo::$language]?$data[CurrentUserInfo::$language]:$data["en"];	
	}
}

class DataTableUtil{
	static function editor_text(){
		return json_decode('{"type":"text"}',true);
	}

	static function viewer_text(){
		return json_decode('{"type":"text"}',true);
	}

	static function viewer_listViewer($obj,$keyField,$valueField){
		$listViewer=array("type"=>"select");
		while($pData = Monster::getRowByQuery("",null,$keyField.",".$valueField)){
			$lang = json_decode($pData["valueField "],true);
			$listViewer["element"][] = $pData[$keyField]."-".$lang["ko"];
			$listViewer["value"][]=$pData["no"];
		}

	}
}

class TimeManager{
	static public $m_timeOffset=32400;
	// private static $m_instance=NULL;
	static public $m_nowDate = NULL;
	//싱글턴 얻어오기
	// public static function get()
	// {
	//     if ( is_null( self::$m_instance ) )
	//     {
	//       self::$m_instance = new self();

	//     }
	//     return self::$m_instance;
	// }
	
	static public function construct(){
		if(!CurrentUserInfo::$timezone)CurrentUserInfo::$timezone="Asia/Seoul";
		TimeManager::$m_nowDate = new DateTime("now",new DateTimeZone(CurrentUserInfo::$timezone));
	}

	// public function setTimeOffsetByLanguage($lang){
	// 	if($lnag=="kr")$this->m_timeOffset = 32400;
	// 	else $this->m_timeOffset = 0;
	// }

	// public function setTimeOffset($offset){
	// 	$this->m_timeOffset = $offset;
	// }
	
	// public function getTimeOffset(){
	// 	return $this->m_timeOffset;
	// }
	static public function getTime(){
		return TimeManager::$m_nowDate->getTimestamp();
		//return time()+$this->m_timeOffset;
	}

	static public function getTimestampByUTC(){
		return time();
	}
	static public function getMicroTime() 
	{ 
	  list($usec, $sec) = explode(" ", microtime()); 
	  return ((float)$usec + (float)$sec + $m_timeOffset); 
	} 
	
	static public function getCurrentWeekNo(){
		$y  =TimeManager::$m_nowDate->format("Y");
		$w  = TimeManager::$m_nowDate->format("W");
		if($w<10)$w="0".$w;

		return $y.$w;

		//return $this->getWeekNo($this->getTime());
	}
	
	// public function getWeekNo($time){
	// 	$y = date("Y",$time);
	// 	$w = date("W",$time);
	// 	if($w<10)$w="0".$w;
		
	// 	return $y.$w;
	// }

	static public function getRemainTimeForWeeklyRank(){
		//return strtotime("next Sunday")-time();
		$sunDay = strtotime("next Sunday",TimeManager::$m_nowDate->getTimestamp());
		$nextday = new DateTime(date("Y-m-d h:i:s",$sunDay),TimeManager::$m_nowDate->getTimezone());
		$nextday->setTime(5,0,0);
		
		$subDay = $nextday->diff(TimeManager::$m_nowDate);

		return ($subDay->d*24*60*60+$subDay->h*60*60+$subDay->i*60+$subDay->s);
	}

	static public function getRemainTimeForTodayMission(){

		$nextday = new DateTime("tomorrow",TimeManager::$m_nowDate->getTimezone());
		$nextday->setTime(5,0,0);
		$subDay = $nextday->diff(TimeManager::$m_nowDate);

		
		return ($subDay->h*60*60+$subDay->i*60+$subDay->s);
	}

	// public function getWeekNo($timestamp){
	// 	return date("W",$timestamp);
	// }
	
	static public function getDateTime($timestamp){
		return date("YmdHis",$timestamp);
	}
	
	static public function getCurrentDateTime(){
		return TimeManager::getDateTime(TimeManager::getTime());
	}

	static public function getCurrentTime(){
  		return date("His",TimeManager::getTime());
	}
	static public function getWeekDayNo($timestamp){
		return date("w",$timestamp);
	}

	static public function getCurrentWeekDayNo(){
		return TimeManager::getWeekDayNo(TimeManager::getTime());
	}
  	
  	static public function getCurrentHour(){
  		return date("H",TimeManager::getTime());
  	}

  	static public function getCurrentDate(){
  		return date("Ymd",TimeManager::getTime());
  	}

  	static public function getYesterDayDate(){
  		return date("Ymd",strtotime(TimeManager::getCurrentDate())-24*60*60);
  	}
}

TimeManager::construct();

class LogManager{
	public static $m_logList=array();
	//private static $m_instance=NULL;
	public static $m_isLocked=false;
	//싱글턴 얻어오기
	// public static function get()
	// {
	//     if ( is_null( self::$m_instance ) )
	//     {
	//       self::$m_instance = new self();
	//     }
	//     return self::$m_instance;
	// }
	
	static public function addLog($log){
		if(!self::isLocked()){
			$dInfo = debug_backtrace();
			$dInfoSize = count($dInfo);
		
			$dInfoSize = $dInfoSize-2;
			if($dInfoSize<0)$dInfoSize=0;
			$php_self = explode("/", $dInfo[$dInfoSize]["file"]);
			$url_cnt = count($php_self) - 1;
			$this_page = $php_self[$url_cnt];
			
			self::$m_logList[]=$this_page."(".$dInfo[$dInfoSize]["line"].")@".$dInfo[$dInfoSize]["class"]."::".$dInfo[$dInfoSize]["function"]." : ".$log;

		}
	}
	
	static public function getLog(){
		return unserialize(serialize(self::$m_logList));
	}
	
	static public function getLogAndClear(){
		$loglist = unserialize(serialize(self::$m_logList));
		self::$m_logList=array();
		return $loglist;
	}
	
	static public function setLock($locked){
		self::$m_isLocked=$locked;
	}
	
	static public function isLocked(){
		return self::$m_isLocked;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ResultState{
	public $m_code;
	public $m_name;
	public $m_message;
	static public $m_resutlCodeList=null;

	const GDUNKNOWNRESULT 		= 0;
	const GDSUCCESS 			= 1;
	const GDDONTMAKETHREAD 		= 2;
	const GDSECURITY 			= 3;
	const GDCHECKNETWORK 		= 4;
	
	const GDMYSQLQUERY 			= 1001;
	const GDMYSQLCONNECTION 	= 1002;
	const GDMYSQLFETCH 			= 1003;

	const GDSAMEVERSION			= 2001;
	const GDPARAMETER 			= 2002;
	const GDDONTFIND 			= 2003;
	const GDFRIENDMAX 			= 2004;
	const GDDONTFINDUSER 		= 2005;
	const GDFAILTOSAVEUSERDATA 	= 2006;
	const GDNEEDJOIN 			= 2007;
	const GDDUPLICATEDNICK 		= 2008;
	const GDFAULTYNICK 			= 2009;
	const GDALREADYMEMBER 		= 2010;
	const GDLONGNAME 			= 2011;
	const GDSHORTNAME 			= 2012;
	const GDFAILTRANSACTION 	= 2013;
	const GDDONTSAVE 			= 2014;
	const GDPROPERTYISMINUS 	= 2015;
	const GDNOTINGWORK 			= 2016;
	const GDALREADY 			= 2017;
	const GDEXPIRE 				= 2018;
	const GDOSERROR				= 2019;
	const GDBLOCKEDUSER				= 2020;

	const GDHTTPGATEWAYERROR	= 3001;

	const GDCMDNOERROR			= 4001;
	const GDDUPLICATEDDEVICE	= 4002;
	const GDLONGTIME			= 4003;

	public function __construct($code=1,$message=""){
		
		if(ResultState::$m_resutlCodeList == null){

			$refl = new ReflectionClass('ResultState');
			foreach ($refl->getConstants() as $key => $value) {
				ResultState::addResultCode($value,$key);			
			}

			// ResultState::addResultCode(0,"GDUNKNOWNRESULT");
			// ResultState::addResultCode(1,"GDSUCCESS");
			// ResultState::addResultCode(2,"GDDONTMAKETHREAD");
			// ResultState::addResultCode(3,"GDSECURITY");
			// ResultState::addResultCode(4,"GDCHECKNETWORK");
			
			// //1000번대 mysql 관련 오류
			// ResultState::addResultCode(1001,"GDMYSQLQUERY");
			// ResultState::addResultCode(1002,"GDMYSQLCONNECTION");
			// ResultState::addResultCode(1003,"GDMYSQLFETCH");
			
			// //2000번대
			// ResultState::addResultCode(2001,"GDSAMEVERSION");
			// ResultState::addResultCode(2002,"GDPARAMETER");
			// ResultState::addResultCode(2003,"GDDONTFIND");
			// ResultState::addResultCode(2004,"GDFRIENDMAX");
			// ResultState::addResultCode(2005,"GDDONTFINDUSER");
			// ResultState::addResultCode(2006,"GDFAILTOSAVEUSERDATA");
			// ResultState::addResultCode(2007,"GDNEEDJOIN");
			// ResultState::addResultCode(2008,"GDDUPLICATEDNICK");
			// ResultState::addResultCode(2009,"GDFAULTYNICK");
			// ResultState::addResultCode(2010,"GDALREADYMEMBER");
			// ResultState::addResultCode(2011,"GDLONGNAME");
			// ResultState::addResultCode(2012,"GDSHORTNAME");
			// ResultState::addResultCode(2013,"GDFAILTRANSACTION");
			// ResultState::addResultCode(2014,"GDDONTSAVE");
			// ResultState::addResultCode(2015,"GDPROPERTYISMINUS");
			// ResultState::addResultCode(2016,"GDNOTINGWORK");


			// //3000 httpgateway
			// ResultState::addResultCode(3001,"GDHTTPGATEWAYERROR");

			// //4000 보안관련
			// ResultState::addResultCode(4001,"GDCMDNOERROR");
			// ResultState::addResultCode(4002,"GDDUPLICATEDDEVICE");
			// ResultState::addResultCode(4003,"GDLONGTIME");
						
						
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
	
	public static function successToArray($message=null){
		$a["code"]=1;
		if($message)$a["message"]=$message;
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
		$this->m_shardDBInfoList[]=null;


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
		self::$m_shardTables["cuponusedinfo"]="CuponUsedInfo";
		self::$m_shardTables["hearthistory"]="HeartHistory";
		self::$m_shardTables["rubyhistory"]="RubyHistory";
		self::$m_shardTables["goldhistory"]="GoldHistory";
		self::$m_shardTables["itemhistory"]="ItemHistory";
		self::$m_shardTables["userproperty"]="UserProperty";
		self::$m_shardTables["userstorage"]="UserStorage";
		self::$m_shardTables["userpropertyhistory"]="UserPropertyHistory";
		self::$m_shardTables["characterhistory"]="CharacterHistory";
		self::$m_shardTables["endlessrank"]="EndlessRank";
		self::$m_shardTables["endlessplaylist"]="EndlessPlayList";
		self::$m_shardTables["modifyhistory"]="ModifyHistory";

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
		self::$m_mainTables["shopevent"]="aShopEventTable";
		self::$m_mainTables["timeevent"]="aTimeEventTable";
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
		self::$m_mainTables["exchange"]="aExchangeManager";
		self::$m_mainTables["adminuser"]="aAdminUser";
	}
	static public function setStaticInfo($p){
		self::$m_gameID = $p["gameID"];
		self::$m_gameNo = $p["gameNo"];//10289;
		self::$m_httpgateway["URL"]=$p["HTTPGATEWAY_URL"]; //"http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";
		self::$m_httpgateway["helpURL"]=$p["HTTPGATEWAY_HELP_URL"]; //"http://alpha-httpgw.hangame.com/hsp/httpgw/help.json";
		self::$m_httpgateway["version"]=$p["HTTPGATEWAY_VERSION"];
		self::$m_httpgateway["port"]=$p["HTTPGATEWAY_PORT"];//"18080";
		self::$m_secretKey = $p["secretKey"];//"12345678";
	}
	// public function setDataBase($gameid=null){
	// 	self::$m_gameID = $gameid;
	// 	self::$m_gameNo = 10289;
	// 	self::$m_httpgateway["URL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";
	// 	self::$m_httpgateway["helpURL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/help.json";
	// 	self::$m_httpgateway["version"]="1.3";
	// 	self::$m_httpgateway["port"]="18080";
	// 	self::$m_secretKey = "12345678";

	// 	if(!$gameid and $gameid=="drawgirls"){


	// 		//서버설정
	// 		$serverInfo = new ServerInfo("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");
	// 		$server0Index = $this->addServerInfo($serverInfo);
			
	// 		//메인db설정
	// 		$this->m_mainDBInfo = new DBInfo("drawgirls",$this->getServerInfo($server0Index));
			
	// 		//샤드db설정
	// 		$this->m_shardDBInfoList[]=new DBInfo("dg001",$this->getServerInfo($server0Index));
	// 		$this->m_shardDBInfoList[]=new DBInfo("dg002",$this->getServerInfo($server0Index));
			

	// 		LogManager::addLog("dbmanager use drawgirls db");
	// 	}else if($gameid == "drawgirls_tstore"){
	// 		//서버설정
	// 		$serverInfo = new ServerInfo("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");
	// 		$server0Index = $this->addServerInfo($serverInfo);
			
	// 		//메인db설정
	// 		$this->m_mainDBInfo = new DBInfo("drawgirls",$this->getServerInfo($server0Index));
			
	// 		//샤드db설정
	// 		$this->m_shardDBInfoList[]=new DBInfo("dg001",$this->getServerInfo($server0Index));
	// 		$this->m_shardDBInfoList[]=new DBInfo("dg002",$this->getServerInfo($server0Index));
			

	// 		LogManager::addLog("dbmanager use drawgirls_tstore db");
	// 	}else{
			
	// 		$serverInfo = new ServerInfo("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");
	// 		$server0Index = $this->addServerInfo($serverInfo);
			
	// 		//메인db설정
	// 		$this->m_mainDBInfo = new DBInfo("drawgirls",$this->getServerInfo($server0Index));
			
	// 		//샤드db설정
	// 		$this->m_shardDBInfoList[]=new DBInfo("dg001",$this->getServerInfo($server0Index));
	// 		$this->m_shardDBInfoList[]=new DBInfo("dg002",$this->getServerInfo($server0Index));
			

	// 		LogManager::addLog("dbmanager use nothing or $gameID db");
	// 	}
	// }

	static public function getSecretKey(){
		return self::$m_secretKey;
	}
	public function setMainDB($dbname,$serverIndex){
		$this->m_mainDBInfo = new DBInfo($dbname,$this->getServerInfo($serverIndex));
	}

	public function addShardDB($dbname,$serverIndex){
		$this->m_shardDBInfoList[]=new DBInfo($dbname,$this->getServerInfo($serverIndex));
	}

	public function addDBServer($ip,$id,$pass){
		$serverInfo = new ServerInfo($ip,$id,$pass);
		return $this->addServerInfo($serverInfo);
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

	public function getShardDBCount(){
		return count($this->m_shardDBInfoList);
	}
	public function getDBIndexByShardKey($shardKey){
		return abs(($shardKey%($this->getShardDBCount()-1))+1);
	}
	public function getDBIndexByShardString($shardString){
		return $this->getDBIndexByShardKey(crc32($shardString));
	}
	
	public function closeShardDB(){
		//for문 돌면서 close
		foreach($this->m_shardDBInfoList as &$dbInfo){
			if($dbInfo)$dbInfo->closeConnection();
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
			$query.="'".@addslashes($data[$key[$i]])."'"; 
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
			
			$query.="`$key[$i]`=\"".@addslashes($data[$key[$i]])."\"";
			if($i!=count($key)-1) $query.=",";
		}
		$query.=" ".$where;
	
		return $query;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class DBRow{
// 	public $m__DBTable=NULL;
// 	public $m__primarykey=NULL;
// 	public $m__result;
// 	public $m__DBInfo;
// 	public $m__isLoaded=false;
// 	public $m__isMainClass=true;
// 	static public $m__qResult = null;
// 	static public $m__qCnt = 0;

	
// 	static public function isMainDBClass(){
// 		if(DBManager::getST(get_called_class()))return false;

// 		return true;
// 	}

// 	public function __construct(){
// 		$this->m__DBTable = DBManager::getST(get_called_class());
// 		$this->m__isMainClass = false;

// 		if(!$this->m__DBTable){
// 			$this->m__DBTable = DBManager::getMT(get_called_class());
// 			$this->m__isMainClass=true;
// 		}
// 		LogManager::addLog("db table auto select by ".get_called_class()." ==>".$this->m__DBTable);
// 	}
	
// 	public function getPrimaryKey(){
// 		return $this->m__primarykey;
// 	}
	
// 	public function getPrimaryValue(){
// 		$pkey = $this->m__primarykey;
// 		if($pkey){
// 			return $this->{"m_".$pkey};	
// 		}
		
// 		return null;
// 	}
	
// 	public function isLoaded(){		
// 		return $this->m__isLoaded;
// 	}

// 	public function setLoaded($isLoaded){
// 		$this->m__isLoaded = $isLoaded;
// 	}

// 	public function setField($field,$value){
// 		$field = "m_".$field;
// 		$this->$field = $value;
// 	}

// 	public function getField($field){
// 		$field = "m_".$field;
// 		return $this->$field;
// 	}

// 	public function __get($name) {
// 	    if ( property_exists($this, $name) ) {
// 	        return $this->{$name};
// 	    }
// 	    $m_name = "m_{$name}";
// 	    if ( property_exists($this, $m_name) ) {
// 	        return $this->{$m_name};
// 	    }
	 
// 	    trigger_error("Undefined property $name or method $method_name");
// 	}
	 
// 	public function __set($name, $value) {
// 	    if ( property_exists($this, $name) ) {
// 	        $this->{$name} = $value;
// 	        return;
// 	    }
// 	    $m_name = "m_{$name}";
// 	    if ( property_exists($this, $m_name) ) {
// 	        $this->{$m_name}=$value;
// 	        return;
// 	    }
	 
// 		$dInfo = debug_backtrace();
// 	    trigger_error($dInfo[0]['file']."(".$dInfo[0]['line'].")"."Undefined property $name or method $method_name");
// 	}

// 	public function getArrayData($isIncludePrimaryKey=false,$keyList=null){


// 		$arraydata=array();
// 		$class_vars = get_class_vars(get_class($this));
// 		foreach($class_vars as $name=>$value){
// 			if(!strpos($name,"__")){
// 				$fieldname = str_replace("m_","",$name);
// 				if(!$keyList|| in_array($fieldname,$keyList)){
// 					$arraydata[$fieldname]=$this->$name;
// 				}
// 			}
// 		}
// 		if($this->m__primarykey && !$isIncludePrimaryKey){
// 			unset($arraydata[$this->m__primarykey]);
// 		}
		
// 		return $arraydata;
// 	}
	
// 	public function updateQuery($where=false){
// 		if(!$where)$where = "where ".$this->m__primarykey."=".$this->getPrimaryValue();
// 		return DBManager::updateQuery($this->m__DBTable,$this->getArrayData(),$where);
// 	}
	
// 	public function insertQuery($isIncludePrimaryKey=false){
// 		return DBManager::insertQuery($this->m__DBTable,$this->getArrayData($isIncludePrimaryKey));
// 	}
	
// 	public function save($isIncludePrimaryKey=false){
// 		LogManager::addLog("save function start");
// 		if(!$this->m__DBInfo){

// 			LogManager::addLog("save function fail, undefined m__DBInfo");
// 			return false;
// 		}

// 		if($this->m__DBInfo->getConnection()){
// 			$query="";
// 			if($this->isLoaded()){
// 				//update
// 				$query = $this->updateQuery();
// 				LogManager::addLog("save make update query ".$query);
// 			}else{
// 				//insert
// 				$query = $this->insertQuery($isIncludePrimaryKey);
// 				LogManager::addLog("save make insert query ".$query);
// 			}
			
// 			if(mysql_query($query,$this->m__DBInfo->getConnection())){
// 				LogManager::addLog("save query ok".mysql_error());
// 				return true;
// 			}else{
// 				LogManager::addLog("save query fail".mysql_error());
// 				return false;
// 			}
// 		}
// 		LogManager::addLog("save fail, m__DBInfo->getConnection() is false");
// 		return false;
// 	}
	
// 	public function load($where){
// 		LogManager::addLog("load start");
// 		if(!$this->m__DBTable)return false;
// 		if(!$this->m__DBInfo)return false;
// 		if(!$this->m__DBInfo->getConnection())return false;
// 		if(!$where && !$this->getPrimaryValue())return false;
		
// 		$this->m__isLoaded=false;

// 		$query = "select * from ".$this->m__DBTable." where `".$this->m__primarykey."`='".$this->getPrimaryValue()."'";
		
// 		if($where){
// 			$query = "select * from ".$this->m__DBTable." where ".$where;
// 		}
		
// 		LogManager::addLog("load query ->".$query);

// 		$result=mysql_query($query,$this->m__DBInfo->getConnection());

// 		if(mysql_error() || mysql_affected_rows()<1){
			
// 			LogManager::addLog("load fail ->".mysql_error()." affected_rows is ".mysql_affected_rows());
// 			return false;
// 		}

// 		if($result)$this->m__result = mysql_fetch_array($result,MYSQL_ASSOC);
		
// 		if($this->m__result && $this->m__result[$this->m__primarykey]){
// 			LogManager::addLog("load ok, primarykey is ".$this->m__primarykey." and value is ".$this->m__result[$this->m__primarykey]);
// 			$this->m__isLoaded=true;
// 			return true;
// 		}
		
// 		LogManager::addLog("load fail");
// 		return false;
// 	}
	
// 	public function remove(){
// 		LogManager::addLog("remove function start");
// 		if($this->isLoaded()){
// 			$query = "DELETE FROM ".$this->m__DBTable." WHERE `".$this->m__primarykey."`='".$this->getPrimaryValue()."'";
// 			$result = mysql_query($query,$this->m__DBInfo->getConnection());
// 			if(!mysql_error()){
// 				LogManager::addLog("remove success with query -> ".$query);
// 				return true;
// 			}
// 		}

// 		LogManager::addLog("remove fail");
// 		return false;
// 	}
// 	public function setDBInfo($dbInfo){
// 		$this->m__DBInfo = $dbInfo;
// 	}

// 	public function getDBConnection(){
// 		return $this->m__DBInfo->getConnection();
// 	}
	
// 	public function setPrimarykey($pkey){
// 		$this->m__primarykey=$pkey;
// 	}
	
// 	public function setDBTable($dbTable){
// 		$this->m__DBTable = $dbTable;
// 	}

// 	public function getDBTable(){
// 		return $this->m__DBTable;
// 	}

// 	public function autoMatching($data){
// 		foreach($data as $key => $value){
// 			$this->{"m_".$key} = $value;
// 		}
// 	}

// 	static public function getShardDBInfoList(){
// 		return UserIndex::getShardDBInfoList();
// 	}

// 	static public function getRowByQuery($where="",$dbcon=NULL){
// 		if(self::isMainDBClass()){
// 			if(!self::$m__qResult)self::$m__qResult = mysql_query("select * from ".DBManager::getMT(get_called_class())." ".$where,DBManager::get()->getMainConnection());
// 			$result = mysql_fetch_array(self::$m__qResult,MYSQL_ASSOC);
// 			if(!$result)self::$m__qResult=null;
// 			return $result;
// 		}

// 		$dbInfoList = self::getShardDBInfoList();
// 		if(self::$m__qCnt>=count($dbInfoList)){
// 			self::$m__qCnt=0;
// 			return false;
// 		}
		
// 		if(!self::$m__qResult){
// 			if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
// 			else $dbconn = $dbcon;

// 			self::$m__qResult = mysql_query("select * from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
// 		}

// 		$result = mysql_fetch_array(self::$m__qResult,MYSQL_ASSOC);

// 		if($result || $dbcon!=NULL)return $result;
		
// 		self::$m__qResult=null;
// 		self::$m__qCnt++;

// 		return self::getRowByQuery($where);
// 	}

// 	static public function removeRowByQuery($where="",$dbcon=NULL){
// 		if(!$where)return;

// 		if(self::isMainDBClass()){
// 			$result=mysql_query("delete from ".DBManager::getMT(get_called_class())." ".$where,DBManager::get()->getMainConnection());
// 			return;
// 		}

// 		$dbInfoList = self::getShardDBInfoList();
// 		for($i=0;$i<count($dbInfoList);$i++){
// 			$dbconn = $dbInfoList[$i]->getConnection();
// 			$result=mysql_query("delete from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
// 		}

// 		return;

// 		// $dbInfoList = self::getShardDBInfoList();
// 		// if(self::$m__qCnt>=count($dbInfoList)){
// 		// 	self::$m__qCnt=0;
// 		// 	return false;
// 		// }
		
// 		// if(!self::$m__qResult){
// 		// 	if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
// 		// 	else $dbconn = $dbcon;

// 		// 	self::$m__qResult = mysql_query("delete from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
// 		// 	LogManager::addLog("removeRowByQuery : ".self::$m__qResult);
// 		// }

// 		// if($dbcon!=NULL)return $result;
		
// 		// self::$m__qResult=null;
// 		// self::$m__qCnt++;

// 		// return self::getRowByQuery($where);
// 	}
// }

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
	static public $m__qCnt = 1;
	private $m__LQTableSelectCustomFunction=null;
	private $m__LQTableInsertCustomFunction=null;
	private $m__LQTableUpdateCustomFunction=null;
	private $m__LQTableDeleteCustomFunction=null;
	private $m__LQTableSelectQueryCustomFunction=null;


	public function __construct(){
		$this->m__DBTable = DBManager::getST(get_called_class());
		$this->m__isMainClass = false;

		if(!$this->m__DBTable){
			$this->m__DBTable = DBManager::getMT(get_called_class());
			$this->m__isMainClass=true;
		}
		LogManager::addLog("db table auto select by ".get_called_class()." ==>".$this->m__DBTable);
	}
	
	static public function isMainDBClass(){
		if(DBManager::getST(get_called_class()))return false;

		return true;
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

	public function setLoaded($isLoaded){
		$this->m__isLoaded = $isLoaded;
	}
	
	public function getArrayData($isIncludePrimaryKey=false,$keyList=null){
		$arraydata=array();
		
		if(!$this->m__data || !is_array($this->m__data))return $arraydata;

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
		LogManager::addLog("DBRow save function");
		if(!$this->m__DBInfo)return false;

		if($this->m__DBInfo->getConnection()){
			$query="";
			$isInsert = false;
			if($this->isLoaded()){
				//update
				$query = $this->updateQuery();
				LogManager::addLog("make update query ".$query);
			}else{
				//insert
				$query = $this->insertQuery($isIncludePrimaryKey);
				LogManager::addLog("make insert query ".$query);
				$isInsert=true;
			}
			
			if(mysql_query($query,$this->getDBConnection())){
				LogManager::addLog("what?->".mysql_error());
				LogManager::addLog("query ok ");
				if($isInsert && $this->m__autoIncreaseKey){
					$this->{$this->m__autoIncreaseKey}=mysql_insert_id($this->getDBConnection());
					LogManager::addLog("find autoIncreaseKey ".$this->m__autoIncreaseKey." value is ".$this->m__data[$this->m__autoIncreaseKey]);
				}
				return true;
			}else{
				LogManager::addLog("query fail ".mysql_error());
				return false;
			}
		}
		LogManager::addLog("dginfo fail");
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
		
		LogManager::addLog("load query ".$query);

		$this->m__isLoaded=false;

		$result=mysql_query($query,$this->getDBConnection());
		$tempdata = null;
		if($result)$tempdata = mysql_fetch_array($result,MYSQL_ASSOC);
		
		if($tempdata && $tempdata[$this->m__primarykey]){
			$this->m__data = $tempdata;
			LogManager::addLog("load ok, primarykey is".$tempdata[$this->m__primarykey]);
			$this->m__isLoaded=true;
			return true;
		}
		
		LogManager::addLog("load fail".mysql_error());
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
	public function getDBTable(){
		return $this->m__DBTable;
	}
	public function &getRef($key){
		return $this->m__data[$key];
	}
	public function setRef($field,$key,$value){
		return $this->m__data[$field][$key]=$value;	
	}
	public function getField($key){
		return $this->m__data[$key];
	}

	public function setField($key,$value){
		$this->m__data[$key]=$value;
	}

	public function getlocalizedValue($key){
		$data = $this->$key;
		if(!is_array($data))$data = json_decode($data,true);
		return $this->getLocalizedValueInData($data);
	}

	public function getLocalizedValueInData($data){
		return $data[CurrentUserInfo::$language]?$data[CurrentUserInfo::$language]:$data["en"];	
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

   	public function __isset ($key) {
	    if ( property_exists($this, $name) ) {
	        isset($this->{$name});
	        return;
	    }

        return isset($this->m__data[$key]);
    }

    public function __unset($key) {

	    if ( property_exists($this, $name) ) {
	        unset($this->{$name});
	        return;
	    }

        unset($this->m__data[$key]);
    }
	


    // lqdatatable 관련

	public function setLQTableSelectQueryCustomFunction($func){
		$this->m__LQTableSelectQueryCustomFunction=$func;
	}
	public function setLQTableSelectCustomFunction($func){
		$this->m__LQTableSelectCustomFunction = $func;
	}
	public function setLQTableInsertCustomFunction($func){
		$this->m__LQTableInsertCustomFunction = $func;
	}
	public function setLQTableUpdateCustomFunction($func){
		$this->m__LQTableUpdateCustomFunction = $func;
	}
	public function setLQTableDeleteCustomFunction($func){
		$this->m__LQTableDeleteCustomFunction = $func;
	}

	public function loadWithDataTable($p){
		if($this->isMainDBClass()){
			$q = mysql_query('DESCRIBE '.$this->m__DBTable,DBManager::get()->getMainConnection());
		}else{
			$q = mysql_query('DESCRIBE '.$this->m__DBTable,DBManager::get()->getConnectionByShardIndex(1));
		}
		$textEditor = array("type"=>"text");
		$textViewer = array("type"=>"text");

		while($row = mysql_fetch_array($q)) {
			$fInfo=array("field"=>$row['Field'],"viewer"=>$textViewer,"editor"=>$textEditor);
			if($row["Key"]=="PRI"){
				$fInfo[]="primary";
			}
			$data["head"][]=$fInfo;
		}

		$data["result"]=ResultState::successToArray();

		return $data;
	}

	public function selectWithLQTable($param){
		$dataList = array();


		//쿼리 만들기
		$query = "";
		if($this->m__LQTableSelectQueryCustomFunction){
			$func = $this->m__LQTableSelectQueryCustomFunction;
			$query = $func($param);
		}


		//정렬쿼리 붙이기
		$orderStr="";
		if($param["sort"]){
			$orderInfo = $param["sort"];
			LogManager::addLog("sort info".$param["sort"]);
			if($orderInfo){
				$orderStr=" order by";
				$cc=1;
				$orderCnt=count($orderInfo);
				if($orderCnt>0){
					foreach ($orderInfo as $key => $value) {
						if($value=="desc")$orderType="desc";
						else $orderType="asc";
						$orderStr.=" `".$key."` ".$orderType;

						if($orderCnt!=$cc)$orderStr.=",";
						$cc++;
					}
				}
			}
		}
		$query.=$orderStr;

		//limit 쿼리붙이기
		if($param["limit"]){
			if(!$param["where"] || !$param["where"]["start"])$param["where"]["start"]=0;
			$query .= " limit ".$param["where"]["start"].",".$param["limit"];
			$result["nextInfo"]=array("start"=>$param["where"]["start"]+$param["limit"]);
		}	

	    //결과 뽑기
	    $cnt=0; 
	    while($rData = self::getRowByQuery($query)){
	    	$rData["shardIndex"]=self::getShardIndexNow();
	    	$func = $this->m__LQTableSelectCustomFunction;
			if($func)$dataList[]=$func($rData);
			else $dataList[]=$rData;
			$cnt++;
	    }

	    $result["param"]=$param;
	    $result["data"]=$dataList;
	    if(count($dataList)<=0){
	    	$result["result"]=ResultState::toArray(ResultState::GDDONTFIND,"데이터를 찾을 수 없습니다.");
	    	return $result;
	    }
		$result["result"]=ResultState::successToArray();
	    return $result;
	}

	public function insertWithLQTable($param){
		$shardIndex=$param["shardIndex"];
		if(!$this->getDBInfo()){
			if($this->isMainDBClass()){
				$this->setDBInfo(DBManager::get()->getMainDBInfo());
			}else if($shardIndex){
				$this->setDBInfo(DBManager::get()->getDBInfoByShardIndex($shardIndex));
			}else{
				$userIndex = UserIndex::create($param["data"]["memberID"]);
				$this->setDBInfo($userIndex->getShardDBInfo());
			}
		}

		$loadQuery=-1;
		if($this->m__LQTableInsertCustomFunction){
			$func = $this->m__LQTableInsertCustomFunction;
			$loadQuery=$func($param);
		}else{
			$pkey = $this->getPrimaryKey();
			if($param["data"][$pkey]){
				$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
			}
		}

		LogManager::addLog("loadQuery:".$loadQuery);
		LogManager::addLog("param check ".json_encode($param["data"],true));
		if($loadQuery!=-1)$this->load($loadQuery);

		if($this->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"로드실패");
			return $r;
		}

		foreach ($param["data"] as $key => $value) {
			$this->$key = $value;
			LogManager::addLog("foreach $key -".json_encode($value,true));
		}

		if($this->save()){
			$r["data"] = $this->getArrayData(true);
			$r["shardIndex"]=$shardIndex;
			$r["result"]=ResultState::successToArray();
			return $r;
		}

		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"저장할수없습니다.");
		return $r;
	}


	public function updateWithLQTable($param){
		if(!$this->getDBInfo()){
			if($this->isMainDBClass()){
				$this->setDBInfo(DBManager::get()->getMainDBInfo());
			}else if($param["shardIndex"]){
				$this->setDBInfo(DBManager::get()->getDBInfoByShardIndex($param["shardIndex"]));
			}else{
				$userIndex = UserIndex::create($param["data"]["memberID"]);
				$this->setDBInfo($userIndex->getShardDBInfo());
			}
		}

		$pkey = $this->getPrimaryKey();

		if($this->m__LQTableUpdateCustomFunction){
			$func=$this->m__LQTableUpdateCustomFunction;
			$loadQuery=$func($param);
		}else{
			
			$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
		}

		$this->load($loadQuery);

		unset($param["data"][$pkey]);
		
		if(!$this->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDMYSQLQUERY,"로드실패");
			return $r;
		}

		foreach ($param["data"] as $key => $value) {
			$this->$key = $value;
		}

		if($this->save()){
			$r["data"] = $this->getArrayData(true);
			$func = $this->m__LQTableSelectCustomFunction;
			if($func)$r["data"]=$func($r["data"]);
			$r["shardIndex"]=$param["shardIndex"];
			$r["result"]=ResultState::successToArray();
			return $r;
		}

		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
		return $r;
	}

	public function deleteWithLQTable($param){
		$shardIndex=$param["shardIndex"];
		if(!$this->getDBInfo()){
			if(!$this->isMainDBClass() && $shardIndex){
				$this->setDBInfo(DBManager::get()->getDBInfoByShardIndex($shardIndex));
			}else{
				$this->setDBInfo(DBManager::get()->getMainDBInfo());
			}
		}

		$pkey = $this->getPrimaryKey();

		if($this->m__LQTableDeleteCustomFunction){
			$func=$this->m__LQTableDeleteCustomFunction;
			$loadQuery=$func($param);
		}else{
			$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
		}

		$this->load($loadQuery);

		if(!$this->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDMYSQLQUERY,"로드실패");
			return $r;
		}

		if($this->remove()){
			$r["result"]=ResultState::successToArray();
			return $r;
		}

		$r["result"]=ResultState::toArray(ResultState::GDMYSQLQUERY,"삭제실패");
		return $r;


	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//static functions
	///////////////////////////////////////////////////////////////////////////////////////////////////

	static public function getShardDBInfoList(){
		return UserIndex::getShardDBInfoList();
	}

	static public function getQueryResult($query,$dbcon=NULL){
		if(self::isMainDBClass()){
		LogManager::addLog("getqueryresult2->".$query);
			$result = mysql_query($query,DBManager::get()->getMainConnection());
			return $result;
		}

		$dbInfoList = self::getShardDBInfoList();


		if(self::$m__qCnt>=count($dbInfoList)){
			self::$m__qCnt=1;
			return false;
		}
		
		if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
		else $dbconn = $dbcon;


		LogManager::addLog("getqueryresult2->".$query);
		$result = mysql_query($query,$dbconn);


		self::$m__qCnt++;
		self::$m__qResult=null;
		
		return $result;
	}
	static public function getShardIndexNow(){
		return self::$m__qCnt;
	}
	static public function getRowByQuery($where="",$dbcon=NULL,$fields="*"){
		if(self::isMainDBClass()){
			if(!self::$m__qResult)self::$m__qResult = mysql_query("select ".$fields." from ".DBManager::getMT(get_called_class())." ".$where,DBManager::get()->getMainConnection());
			LogManager::addLog("select ".$fields." from ".DBManager::getMT(get_called_class())." ".$where);
			if(self::$m__qResult)$result = mysql_fetch_array(self::$m__qResult,MYSQL_ASSOC);
			if(!$result)self::$m__qResult=null;
			return $result;
		}

		$dbInfoList = self::getShardDBInfoList();
		if(self::$m__qCnt>=count($dbInfoList)){
			self::$m__qCnt=1;
			return false;
		}
		
		if(!self::$m__qResult){
			if($dbcon==NULL)$dbconn = $dbInfoList[self::$m__qCnt]->getConnection();
			else $dbconn = $dbcon;

			self::$m__qResult = mysql_query("select ".$fields." from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
			LogManager::addLog("ok go select ".$fields." from ".DBManager::getST(get_called_class())." ".$where);
		}

		$result = mysql_fetch_array(self::$m__qResult,MYSQL_ASSOC);

		if($result || $dbcon!=NULL)return $result;
		
		self::$m__qResult=null;
		self::$m__qCnt++;

		return self::getRowByQuery($where);
	}

	static public function getObjectByQuery($where="",$dbcon=NULL,$fields="*"){
		$rowData = self::getRowByQuery($where,$dbcon,$fields);
		$obj=null;
		if($rowData){
			$cls = get_called_class();
			$obj = new $cls();
			$obj->m__data = $rowData;
			$obj->setLoaded(true);
		}
		return $obj;
	}

	static public function removeRowByQuery($where="",$dbcon=NULL){
		if(!$where)return false;

		if(self::isMainDBClass()){
			$result=mysql_query("delete from ".DBManager::getMT(get_called_class())." ".$where,DBManager::get()->getMainConnection());
			return $result;
 		}

		if($dbcon){
			$table = DBManager::getMT(get_called_class());
			if(!$table)$table=DBManager::getST(get_called_class());
			$result=mysql_query("delete from ".$table." ".$where,$dbcon);
			LogManager::addLog("delete gogo is ".$result);
			return $result;
		}

		$dbInfoList = self::getShardDBInfoList();
		for($i=1;$i<count($dbInfoList);$i++){
			$dbconn = $dbInfoList[$i]->getConnection();
			$result=mysql_query("delete from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
		}

		return true;
	}


}
?>