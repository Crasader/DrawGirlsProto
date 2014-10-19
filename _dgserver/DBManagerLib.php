<?php
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CurrentUserInfo{
	static public $os="android";
	static public $language="ko";
	static public $memberID;
	static public $socialID;
	static public $country="kr";
	static public $timezone="Asia/Seoul";

	static public function getLocalizedValueInData($data){
		return $data[CurrentUserInfo::$country]?$data[CurrentUserInfo::$country]:$data["default"];	
	}

	static public $ccList = array("kr","th","tw","hk","vn","ph","sg","mo","my","id","jp","cn","us");
	static public $osList = array("ios","android");
	
	static public function getCountryBit($cc){
		$idx = array_search($cc,self::$ccList);
		if($idx!==false)return 1<<$idx;
		return 0;
	}
	
	static public function getOsBit($os){
		$idx = array_search($os,self::$osList);
		if($idx!==false)return 1<<$idx;
		return 0;
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
	static public $m_manageTimezone = "Asia/Seoul";
	//싱글턴 얻어오기
	// public static function get()
	// {
	//     if ( is_null( self::$m_instance ) )
	//     {
	//       self::$m_instance = new self();

	//     }
	//     return self::$m_instance;
	// }
	
	static public $weekDayList = array("sun","mon","tue","wed","thu","fri","sat");
	
	static public function getWeekDayBit($day){
		$idx = array_search($day,self::$weekDayList);
		if($idx!==false)return 1<<$idx;
		return 0;
	}

	static public function construct(){
		if(!CurrentUserInfo::$timezone)CurrentUserInfo::$timezone=TimeManager::$m_manageTimezone;
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
		$wr = new CommonSetting("rankResetTime");
		$rInfo =& $wr->getRef("value");
		$sunDay = strtotime("next ".$rInfo["day"],TimeManager::$m_nowDate->getTimestamp());
		$nextday = new DateTime(date("Y-m-d h:i:s",$sunDay),TimeManager::$m_nowDate->getTimezone());
		$nextday->setTime($rInfo["hour"],0,0);
		
		$subDay = $nextday->diff(TimeManager::$m_nowDate);

		return ($subDay->d*24*60*60+$subDay->h*60*60+$subDay->i*60+$subDay->s);
	}

	static public function getRemainTimeForTodayMission(){

		$nextday = new DateTime("tomorrow",TimeManager::$m_nowDate->getTimezone());
		$nextday->setTime(0,0,0);
		$subDay = $nextday->diff(TimeManager::$m_nowDate);

		
		return ($subDay->h*60*60+$subDay->i*60+$subDay->s);
	}

	// public function getWeekNo($timestamp){
	// 	return date("W",$timestamp);
	// }
	
	static public function getDateTime($timestamp){
		return date("YmdHis",$timestamp);
	}
	
	
	static public function getCurrentDateTimeForKorea(){
		$_date = new DateTime("now",new DateTimeZone(TimeManager::$m_manageTimezone));
		return $_date->format("YmdHis");
	}

	static public function getCurrentDateTime(){
		return self::$m_nowDate->format("YmdHis");
	}

	static public function getCurrentTime(){
		return self::$m_nowDate->format("His");
  		//return date("His",TimeManager::getTime());
	}
	static public function getWeekDayNo($timestamp){
		return self::$m_nowDate->format("w");
		//return date("w",$timestamp);
	}

	static public function getCurrentWeekDayNo(){
		return TimeManager::getWeekDayNo(TimeManager::getTime());
	}
  	
  	static public function getCurrentHour(){

		return self::$m_nowDate->format("H");
  		//return date("H",TimeManager::getTime());
  	}

  	static public function getCurrentDate(){
		return self::$m_nowDate->format("Ymd");
  		//return date("Ymd",TimeManager::getTime());
  	}

  	static public function getYesterDate(){
  		$nextday = new DateTime("yesterday",TimeManager::$m_nowDate->getTimezone());
		return $nextday->format("Ymd");
  		//return date("Ymd",TimeManager::getTime());
  	}


  	static public function getModifyDateFromToday($add){

		$today = new DateTime("now",new DateTimeZone(CurrentUserInfo::$timezone));
		$today->modify($add);
		return $today->format("Ymd");
  		//return date("Ymd",TimeManager::getTime());
  	}


  	static public function getYesterDayDate(){
  		return date("Ymd",strtotime(TimeManager::getCurrentDate())-24*60*60);
  	}

  	static public function dateStringToDate($date){
		$dateInfo = date_create_from_format('Ymd', $date);
		
  		return $dateInfo->getTimestamp();
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

	public static function nothing1($abc){
		echo "gkgkgkgkgkgkgkgk"; 
		return true;
	}
}

new ResultState();

?>