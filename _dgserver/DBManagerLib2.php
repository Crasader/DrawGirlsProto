<?php
class Infomation{
	public static $m_serverInfo=array();
	public static $m_shardDBInfoList=array();
	public static $m_mainDBInfo=array();
	public static $m_gameNo=null;
	public static $m_httpgateway=null;
	public static $m_gameID=null;
	public static $m_mainTables=array();
	public static $m_shardTables=array();
	public static $m_secretKey;

	static public function setStaticInfo($p){
		self::$m_gameID = $p["gameID"];
		self::$m_gameNo = $p["gameNo"];//10289;
		self::$m_httpgateway["URL"]=$p["HTTPGATEWAY_URL"]; //"http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";
		self::$m_httpgateway["helpURL"]=$p["HTTPGATEWAY_HELP_URL"]; //"http://alpha-httpgw.hangame.com/hsp/httpgw/help.json";
		self::$m_httpgateway["version"]=$p["HTTPGATEWAY_VERSION"];
		self::$m_httpgateway["port"]=$p["HTTPGATEWAY_PORT"];//"18080";
		self::$m_secretKey = $p["secretKey"];//"12345678";
	}
}

class DBServer{
	static public $m_serverList=array(0);
	public $m_host=null;
	public $m_id=null;
	public $m_pw=null;
	public $m_connection=null;
	public $m_selectedDBName=null;
	public function __construct($host,$id,$pw){
		////LogManager::addLog("__construct server".$host.$id.$pw);
		$this->m_host=$host;
		$this->m_id=$id;
		$this->m_pw=$pw;
	}

	static public function create($host,$id,$pw){
		////LogManager::addLog("create server".$host.$id.$pw);
		// for($i=1;$i<count(self::$m_serverList);$i++){
		// 	$server = $m_serverList[$i];
		// 	if($host==$server->m_host && $id == $server->m_id){
		// 		//LogManager::addLog("selected server");
		// 		return $server;}
		// }

		$server = new DBServer($host,$id,$pw);
		self::$m_serverList[]=$server;

		return $server;
	}

	static public function getServer($order){
		return $m_serverList[$order];
	}


	public function getConnection(){
		
		if(!$this->m_connection){
			$this->m_connection = mysql_connect($this->m_host,$this->m_id,$this->m_pw);
			if($this->m_connection){
				////LogManager::addLog("success mysql_connect");
			}
		}
		return $this->m_connection;
	}
	
	public function closeConnection(){
		if($this->m_connection){
			@mysql_close($this->m_connection);
			$this->m_connection=null;
		}	
	}

	public function selectDB($dbName){

		if(mysql_select_db($dbName, $this->getConnection())){
			$this->m_selectedDBName=$dbName;
			return true;
		}else{
			return false;
		}
	}

	static public function closeAllConnection(){
		for($i=1;$i<count(self::$m_serverList);$i++){
			$server = self::$m_serverList[$i];
			if($server){
				$server->closeConnection();
			}
		}
	}
}
class DBGroup{
	static public $m_groupList;

	public $m_name;
	public $m_masterList=array(0);
	public $m_newShardKeyFunc=array();
	public $m_getShardKeyFunc=array();
	public static function create($name){
		if(self::$m_groupList[$name])return self::$m_groupList[$name];
		self::$m_groupList[$name] = new DBGroup($name);
		return self::$m_groupList[$name];
	}
	public function __construct($name){
		$this->m_name=$name;
	}

	public function addMaster($db){
		$this->m_masterList[]=$db;

		////LogManager::addLog("masterSize is ".count($this->m_masterList));
	}

	public function getMaster($order){
		return $this->m_masterList[$order];
	}

	public function getMasterList(){
		return $this->m_masterList;
	}

	public function getMasterCount(){
		return count($this->m_masterList)-1;
	}

	public function getSlaveList(){
		$r = array(0);
		for($i=1;$i<count($this->m_masterList);$i++){
			$r[]=$this->m_masterList[$i]->getSlaveByRand();
		}
		return $r;
	}


	public function setNewShardKeyFunc($func,$cls="default"){
		$this->m_newShardKeyFunc[$cls] = $func;
	}

	public function getNewShardOrderByKey($key,$cls="default"){
		if(!$this->m_newShardKeyFunc[$cls])$cls="default";
		$func = $this->m_newShardKeyFunc[$cls];
		$shardOrder =  $func($key);
		return $shardOrder;
	}

	public function setGetShardKeyFunc($func,$cls="default"){
		$this->m_getShardKeyFunc[$cls] = $func;
	}

	public function getShardKeyFunc($skey,$cls="default"){
		if(!$this->m_getShardKeyFunc[$cls])$cls="default";
		$func = $this->m_getShardKeyFunc[$cls];
		return $func($skey);
	}

	public function newShardKeyFunc($skey,$cls="default"){
		if(!$this->m_newShardKeyFunc[$cls])$cls="default";
		$func = $this->m_newShardKeyFunc[$cls];
		return $func($skey);
	}

	public function getMasterConnection($key,$cls="default"){
		if(!$this->m_getShardKeyFunc[$cls])$cls="default";
		$func = $this->m_getShardKeyFunc[$cls];
		$serverOrder = $func($key);
		$server = $this->getMaster($serverOrder);
		return $server->getConnection();
	}

	public function getConnectionForWrite($shardKeyValue,$shardOrder=null,$cls="default"){
		

		if(!$shardOrder){
			if(!$this->m_getShardKeyFunc[$cls])$cls="default";
			$func = $this->m_getShardKeyFunc[$cls];
			$shardOrder = $func($shardKeyValue);
		}
		
		$server = $this->getMaster($shardOrder);
		if(!$server)return null;
		return $server->getConnection();
	}

	public function getConnectionForRead($shardKeyValue,$shardOrder=null,$cls="default"){
		if(!$shardOrder){
			if(!$this->m_getShardKeyFunc[$cls])$cls="default";
			$func = $this->m_getShardKeyFunc[$cls];
			$shardOrder = $func($shardKeyValue);
		}
		
		$master = $this->getMaster($shardOrder);
		if(!$master)return null;
		$slave = $master->getSlaveByRand();
		if(!$slave)return null;
		return $slave->getConnection();
	}

	public function getConnectionForReadByRand(){
		return $this->getMaster(1)->getSlaveByRand()->getConnection();
	}



	public function getDBOrderByShardKey($shardKey){
		return abs(($shardKey%($this->getMasterCount()))+1);
	}
	
	public function getDBOrderByShardString($shardString){
		return $this->getDBOrderByShardKey(crc32($shardString));
	}
}

class DBMaster{
	public $m_name = null;
	public $m_server = null;
	public $m_slaveList = array(0);

	public function __construct($name,$server){
		$this->m_name=$name;
		$this->m_server=$server;
	}
	
	public function getConnection(){
		if(!$this->m_server)return null;
		
		$conn=$this->m_server->getConnection();
		$this->m_server->selectDB($this->m_name);
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
		return $this->m_slaveList[$order];
	}

	public function getSlaveList(){
		return $this->m_slaveList;
	}

	public function getSlaveCount(){
		return count($this->m_slaveList)-1;
	}

	public function getSlaveByRand(){
		srand((double)microtime()*1000000);
		$r  =rand(1,$this->getSlaveCount());
		////LogManager::addLog("slaveList is".var_export($this->m_slaveList,true));
		$slave = $this->m_slaveList[$r];
		////LogManager::addLog("slave is ".var_export($slave,true));
		return $slave;
	}

}

class DBSlave{
	public $m_name = null;
	public $m_server = null;
	public function __construct($name,$server){
		$this->m_name=$name;
		$this->m_server=$server;
	}
	
	public function getConnection(){
		if(!$this->m_server)return null;
		
		$conn=$this->m_server->getConnection();
		$this->m_server->selectDB($this->m_name);
		return $conn;
	}
	
	public function closeConnection(){
		if($this->m_server)
			$this->m_server->closeConnection();
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DBTable2{
	public $m__data;
	public $m__modifyData;
	public $m__isLoaded=false;
	public $m__shardKeyValue=null;
	public $m__shardOrder=null;
	
	static public $m__DBTable=NULL;
	static public $m__primaryKey=NULL;
	static public $m__autoIncreaseKey=NULL;
	static public $m__shardKey=NULL;
	static public $m__DBGroup;
	static public $m__qResult = -1;
	static public $m__qCnt = -1;
	static public $m__LQTableSelectCustomFunction=null;
	static public $m__LQTableInsertCustomFunction=null;
	static public $m__LQTableUpdateCustomFunction=null;
	static public $m__LQTableDeleteCustomFunction=null;
	static public $m__LQTableSelectQueryCustomFunction=null;
	static public $m__dbList=null;
	static public $m__isInit=false;
	
	static public function newShardKeyFunc($shardkey){
		self::initial();
		return self::getDBGroup()->newShardKeyFunc($shardkey,self::getDBTable());
	}

	static public function getShardKeyFunc($shardkey){
		self::initial();
		return self::getDBGroup()->getShardKeyFunc($shardkey,self::getDBTable());
	}

	static public function setNewShardKeyFunc($func){
		self::initial();
		self::getDBGroup()->setNewShardKeyFunc($func,self::getDBTable());
	}
	
	static public function setGetShardKeyFunc($func){
		self::initial();
		self::getDBGroup()->setGetShardKeyFunc($func,self::getDBTable());
	}
	
	static public function initial(){
		if(static::$m__isInit==false){
			static::$m__isInit=&$newvalue;
			static::$m__isInit=true;

			// static::$m__DBTable=&$newvalue;
			// static::$m__DBTable = DBManager::getST(get_called_class());

			// if(!static::$m__DBTable){
			// 	static::$m__DBTable = DBManager::getMT(get_called_class());
			// }


			

			static::construct();
		}
	}

	public function __construct(){
			static::construct();

	}
	
	
	public function getPrimaryValue(){
		$pkey = static::$m__primaryKey;
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
		
		if(static::$m__primaryKey && !$isIncludePrimaryKey){
			unset($arraydata[static::$m__primaryKey]);
		}
		
		return $arraydata;
	}

	public function getArrayModifyData($isIncludePrimaryKey=false){
		$arraydata=array();
		
		if(!$this->m__modifyData || !is_array($this->m__modifyData))return $arraydata;

		foreach($this->m__modifyData as $name=>$value){
			if(!$keyList || in_array($name,$keyList)){
					$arraydata[$name]=$this->m__data[$name];
			}
		}
		
		if(static::$m__primaryKey && !$isIncludePrimaryKey){
			unset($arraydata[static::$m__primaryKey]);
		}
		
		return $arraydata;
	}
	
	public function updateQuery($where=false,$isIncludePrimaryKey=false){
		if(!$where)$where = "where ".static::$m__primaryKey."=".$this->getPrimaryValue();
		$data = $this->getArrayModifyData($isIncludePrimaryKey); 
		$table = static::$m__DBTable;
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
	
	public function insertQuery($isIncludePrimaryKey=false){
		$data = $this->getArrayData($isIncludePrimaryKey);
		$table = self::getDBTable();
		$key=array_keys($data);  
		$query="insert into `$table` ("; 
			for($i=0;$i<count($key);$i++){ 
				$query.="`".$key[$i]."`"; 
				if($i!=count($key)-1) $query.=","; 
			} 
	
		$query.=") values ("; 
		for($i=0;$i<count($key);$i++){ 
			if(is_array($data[$key[$i]])) $data[$key[$i]]=json_encode($data[$key[$i]],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$query.="'".@addslashes($data[$key[$i]])."'"; 
			if($i!=count($key)-1) $query.=","; 
		} 
		$query.=")"; 
		return $query; 

	}
	
	public function save($isIncludePrimaryKey=false){
		if(!static::$m__DBGroup){
			return false;
		}


		if(!$this->m__shardKeyValue){
			$key = static::$m__shardKey;
			$this->m__shardKeyValue = $this->{$key};
		}

		$writeConn = static::$m__DBGroup->getConnectionForWrite($this->m__shardKeyValue,$this->m__shardOrder,self::getDBTable());
		


		if($writeConn){
			$query="";
			$isInsert = false;
			if($this->isLoaded()){
				//update
				$query = $this->updateQuery(false,$isIncludePrimaryKey);
				//LogManager::addLog("make update query ".$query);
		}else{
				//insert
				$query = $this->insertQuery($isIncludePrimaryKey);
				//LogManager::addLog("make insert query ".$query);
				$isInsert=true;
			}
			
			LogManager::addLog("make query ".$query);
				
			if(mysql_query($query,$writeConn)){
				if($isInsert && static::$m__autoIncreaseKey){
					$this->{static::$m__autoIncreaseKey}=mysql_insert_id($writeConn);
				}
				return true;
			}else{
				//LogManager::addLog("query fail ".mysql_error());
				return false;
			}
		}
		return false;
	}
	
	public function jsonToObj($field){
		if(is_string($this->m__data[$field]))$this->m__data[$field]=json_decode($this->m__data[$field],true);
	}

	public function load($where){
		self::initial();
		//LogManager::addLog("start load1 $where and ".$this->m__shardKeyValue);
		if(!static::$m__DBTable){
			//LogManager::addLog("need check m__DBTable");
			return false;
		}
		if(!static::$m__DBGroup){
			//LogManager::addLog("need check m__DBGroup");
			return false;
		}
		if(!$where && !$this->getPrimaryValue()){
			//LogManager::addLog("need check where or this->getPrimaryValue");
			return false;
		}

		if(!$this->m__shardKeyValue){

			$key = static::$m__shardKey;
			$this->m__shardKeyValue = $this->{$key};
		}

		$readConn = static::$m__DBGroup->getConnectionForRead($this->m__shardKeyValue,$this->m__shardOrder,self::getDBTable());
		if(!$readConn){
			//LogManager::addLog("need connection ",$this->m__shardKeyValue,$this->m__shardOrder);
			return false;
		}
		


		$query = "select * from ".static::$m__DBTable." where `".static::$m__primaryKey."`='".$this->getPrimaryValue()."'";
		
		if($where){
			$query = "select * from ".static::$m__DBTable." where ".$where;
		}
		
		//LogManager::addLog("load query ".$query);

		$this->m__isLoaded=false;

		$result=mysql_query($query,$readConn);
		$tempdata = null;
		if($result)$tempdata = mysql_fetch_array($result,MYSQL_ASSOC);
		
		if($tempdata && $tempdata[static::$m__primaryKey]){
			$this->m__data = $tempdata;
			//LogManager::addLog("load ok, primarykey is".$tempdata[static::$m__primaryKey]);
			$this->m__isLoaded=true;
			return true;
		}
		
		//LogManager::addLog("load fail".mysql_error());
		return false;
	}
	
	public function remove(){
		$writeConn = static::$m__DBGroup->getConnectionForWrite($this->m__shardKeyValue,$this->m__shardOrder,self::getDBTable());

		if($writeConn && $this->isLoaded()){
			$query = "DELETE FROM ".static::$m__DBTable." WHERE `".static::$m__primaryKey."`='".$this->getPrimaryValue()."'";
			$result = mysql_query($query,$writeConn);
			if($result)return true;
		}
		return false;
	}

	public function setDBShardKeyValue($shardkeyValue){
		$this->m__shardKeyValue = $shardkeyValue;
	}

	public function setDBShardOrder($shardOrder){
		$this->m__shardOrder = $shardOrder;
	}

	// public function getDBConnection(){
	// 	return static::$m__DBInfo->getConnection();
	// }
	

	public function &getRef($key,$isRead=false){
		if(is_string($this->m__data[$key]))$this->jsonToObj($key);
		if($isRead==false)$this->m__modifyData[$key]=true;
		return $this->m__data[$key];
	}
	public function setRef($field,$key,$value){
		$this->m__modifyData[$field]=true;
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
		return $data[CurrentUserInfo::$country]?$data[CurrentUserInfo::$country]:$data["default"];	
	}

	public function __get($name) {
	    if ( property_exists($this, $name) ) {
	        return $this->{$name};
	    }else if(method_exists($this,$name)){
	    	return $this->$name();
	    }

	   	return $this->m__data[$name];
	}
	 
	public function __set($name, $value) {
	    if ( property_exists($this, $name) ) {
	        $this->{$name} = $value;
	        return;
	    }else if(method_exists($this,$name)){
	    	return $this->$name($value);
	    }

	    $this->m__data[$name]=$value;
	    $this->m__modifyData[$name]=true;

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
        unset($this->m__modifyData[$key]);
    }
	


    // lqdatatable 관련

	public static function setShardKey($shardkey){
		self::initial();
		static::$m__shardKey =& $newvalue;
		static::$m__shardKey = $shardkey;
		//LogManager::addLog(get_called_class()."set m__shardKey = ".static::$m__shardKey);
	}

	public static function getShardKey(){
		self::initial();
		return $this->m__shardKey;
	}

	public static function getPrimaryKey(){
		self::initial();
		return static::$m__primaryKey;
	}
	

	public static function getAutoIncreaseKey(){
		self::initial();
		return static::$m__autoIncreaseKey;
	}
	

	public static function getDBGroup(){
		self::initial();
		return static::$m__DBGroup;
	}

	public static function setDBGroup($name){
		self::initial();
		static::$m__DBGroup =& $newvalue;
		static::$m__DBGroup = DBGroup::create($name);
	}

	public static function setPrimarykey($pkey,$isAutoincrease=false){
		self::initial();
		static::$m__primaryKey =& $newvalue;
		static::$m__primaryKey=$pkey;
		if($isAutoincrease)self::setAutoIncreaseKey($pkey);
	}
	
	public static function setAutoIncreaseKey($pkey){
		self::initial();
		static::$m__autoIncreaseKey =& $newvalue;
		static::$m__autoIncreaseKey=$pkey;
	}
	
	public static function setDBTable($dbTable){
		self::initial();
		static::$m__DBTable =& $newvalue;
		static::$m__DBTable = $dbTable;
	}
	public static function getDBTable(){
		self::initial();
		return static::$m__DBTable;
	}

	static public function setLQTableSelectQueryCustomFunction($func){
		$newValue="";
		static::$m__LQTableSelectQueryCustomFunction=&$newValue;
		static::$m__LQTableSelectQueryCustomFunction=$func;
	}
	static public function setLQTableSelectCustomFunction($func){
		//LogManager::addLog("set select custom func for ".self::getDBTable());
		$newValue="";
		static::$m__LQTableSelectCustomFunction=&$newValue;
		static::$m__LQTableSelectCustomFunction = $func;
	}
	static public function setLQTableInsertCustomFunction($func){
		$newValue="";
		static::$m__LQTableInsertCustomFunction=&$newValue;
		static::$m__LQTableInsertCustomFunction = $func;
	}
	static public function setLQTableUpdateCustomFunction($func){
		$newValue="";
		static::$m__LQTableUpdateCustomFunction=&$newValue;
		static::$m__LQTableUpdateCustomFunction = $func;
	}
	static public function setLQTableDeleteCustomFunction($func){
		$newValue="";
		static::$m__LQTableDeleteCustomFunction=&$newValue;
		static::$m__LQTableDeleteCustomFunction = $func;
	}

	public static function loadWithLQTable($p){
		self::initial();
		$admin = new AdminUser($_SESSION["admin_no"]);

		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");
		}
		if(!$admin->checkPermissionGroup(get_called_class(),"read")){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");	
		}

		$klass = get_called_class();
		$obj = new $klass();
		
		$q = mysql_query('DESCRIBE '.static::$m__DBTable,static::$m__DBGroup->getConnectionForReadByRand());

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

	public static function selectWithLQTable($param){
		self::initial();
		$admin = new AdminUser($_SESSION["admin_no"]);

		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");
		}
		if(!$admin->checkPermissionGroup(get_called_class(),"read")){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");	
		}

		$klass = get_called_class();
		$obj = new $klass();

		$dataList = array();


		//쿼리 만들기
		$query = "";
		if(static::$m__LQTableSelectQueryCustomFunction){
			//LogManager::addLog("start custom select query param is".json_encode($param));
			$func = static::$m__LQTableSelectQueryCustomFunction;
			$query = $func($param);
			//LogManager::addLog("custom select query : ".$query);
		}


		//정렬쿼리 붙이기
		$orderStr="";
		$orders = array();
		if($param["sort"]){
			$orderInfo = $param["sort"];
			LogManager::addLog("sort info".json_encode($orderInfo));
			if($orderInfo){
				$orderStr=" order by";
				$cc=1;
				$orderCnt=count($orderInfo);
				if($orderCnt>0){
					foreach ($orderInfo as $key => $value) {
						if($value=="desc")$orderType="desc";
						else $orderType="asc";
						$orderStr.=" `".$key."` ".$orderType;

						$orders[]=array("field"=>$key,"dir"=>$orderType);

						if($orderCnt!=$cc)$orderStr.=",";
						$cc++;


					}
				}
			}
		}
		LogManager::addLog("sort info".$orderStr);
		LogManager::addLog("sort info".json_encode($orders));
		$query.=$orderStr;

		//limit 쿼리붙이기
		$limitQuery="";
		if($param["limit"]){
			// if(!$param["where"] || !$param["where"]["startList"]){
			// 	$param["where"]["start"]=1;
			// }
			// $result["nextInfo"]=array("start"=>$param["where"]["start"]+$param["limit"]);

			$limitOfServer = $param["where"]["startList"];
			if(count($limitOfServer)>0){
				$limitQuery =" limit ".($limitOfServer[1]).",".$param["limit"];
			}else{
				$limitOfServer = array(0,0);
				$limitQuery = " limit 0,".$param["limit"];	
			}
		}else{
			$param["limit"]=1000;
			$limitQuery = " limit 0,".$param["limit"];	
		}

	    //결과 뽑기
	    $cnt=0; 
	    $rl=0;
	    $rdata = array();
	    if(count($orders)>=1){
	    	$orderField1 = $orders[0]["field"];	
			$orderDir1 = $orders[0]["dir"];
	    }
	    if(count($orders)>=2){
	    	$orderField2 = $orders[1]["field"];
			$orderDir2 = $orders[1]["dir"];
	    }
		
		function isBig($value1,$value2,$dir){
			if($dir=="desc"){
				if($value1>=$value2){
					return true;
				}
			}else{
				if($value1<=$value2){
					return true;
				}
			}
			return false;
		}
		$count = $param["limit"];


	    $serveri=0;
	    $oShard=-1;
	    
	    $whereFunc = function($cnt) use ($query,$limitOfServer,$param){
	    	if(!$limitOfServer[$cnt])$limitOfServer[$cnt]=0;
	    	$limitQuery = " limit ".($limitOfServer[$cnt]).",".$param["limit"];
	    	return $query.$limitQuery;
	    };
	    
	    while($data = self::getRowByQuery($query.$limitQuery,null,"*",$whereFunc)){


	    	$serveri++;
	    	$nShard = self::getShardIndexNow();
			
			$data["shardIndex"]=$nShard;
			
	    	
	    	if($oShard!=$nShard)$serveri=1;
	    	if($param["limit"]>0 && $serveri==$param["limit"] && $oShard==$nShard){
	    		static::$m__qResult=null;
				static::$m__qCnt++;
	    		$serveri=0;
	    		$nShard = self::getShardIndexNow();
				if(!$limitOfServer[$nShard+1])$limitOfServer[$nShard+1]=0;
				$limitQuery =" limit ".($limitOfServer[$nShard+1]).",".$param["limit"];
				
	    	}
	    	$oShard = $nShard;

	    	$rl++;
			$l=0;

			$func = static::$m__LQTableSelectCustomFunction;
			if($func){
				$data=$func($data);
			}

			

			if(!$orderField1){
				$rdata[]=$data;
				continue;
			}

			if(count($rdata)==0){
				$rdata[]=$data;
				continue;
			}

			//새로비교하는놈이 기준에 안맞으면 이놈을 끝으로 다음서버로 넘어간다.
			if($orderField1 && $rdata[$count-1][$orderField1] && isBig($rdata[$count-1][$orderField1],$data[$orderField1],$orderDir1)){
				static::$m__qResult=null;
				static::$m__qCnt++;
				$serveri=0;
				//if(count($limitOfServer)>0){
				$nShard = self::getShardIndexNow();
				if(!$limitOfServer[$nShard])$limitOfServer[$nShard]=0;
				$limitQuery =" limit ".($limitOfServer[$nShard]).",".$param["limit"];
				
			//}
			}


			if($orderField1){
				for($i=0;$i<count($rdata);$i++){
					if(!isBig($rdata[$i][$orderField1],$data[$orderField1],$orderDir1))break;
					if($rdata[$i][$orderField1]==$data[$orderField1] && !isBig($rdata[$i][$orderField2],$data[$orderField2],$orderDir2))break;
					$l++;
				}
			}


			array_splice($rdata, $l, 0, array($data));
			array_splice($rdata, $count, 1);
			$cnt++;
	    }

	    //LogManager::addLog("test startlimit");
	    $limitOfServer2 = array(0,0,0,0,0,0,0,0,0,0,0,0,0);


		for($i=0;$i<count($rdata);$i++){
			$si = $rdata[$i]["shardIndex"];
			$limitOfServer[$si] +=1;
		}


		for($i=0;$i<count($limitOfServer2);$i++){
	    	//LogManager::addLog("test startlimit->".$i.",".$limitOfServer[$i]);
			if($limitOfServer[$i])$limitOfServer2[$i]=$limitOfServer[$i];
		}

		$result["nextInfo"]["startList"]=$limitOfServer2;


	    $result["param"]=$param;
	    $result["data"]=$rdata;
	    if(count($rdata)<=0 && $param["where"]["start"]>1){
	    	$result["result"]=ResultState::toArray(ResultState::GDDONTFIND,"데이터를 찾을 수 없습니다.");
	    	return $result;
	    }
		$result["result"]=ResultState::successToArray();
	    return $result;
	}

	public static function insertWithLQTable($param){
		self::initial();
		$admin = new AdminUser($_SESSION["admin_no"]);

		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");
		}
		if(!$admin->checkPermissionGroup(get_called_class(),"write")){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");	
		}

		if($param["enableComment"]==true){
			$mh = new ModifyHistory($param["data"]["memberID"]);
			$mh->writer = $admin->id;
			$mh->oldData="insert";
			$mh->newData=$param["data"];
			$mh->reason=$param["reason"];
			$mh->category=get_called_class();
			$mh->save();
		}

		$klass = get_called_class();
		$shardIndex=$param["shardIndex"];
		$pkey = self::getPrimaryKey();
		$autokey = self::getAutoIncreaseKey();
		$obj = new $klass();
		$obj->setDBShardOrder($shardIndex);

		// if(!static::$m__DBGroup){
		// 	if(self::isMainDBClass()){
		// 		self::setDBGroup("main");
		// 	}else if($shardIndex){
		// 		self::setDBGroup("user",null,$shardIndex);
		// 	}else{
		// 		$userIndex = UserIndex::create($param["data"]["memberID"]);
		// 		$obj->setDBShardOrder($userIndex->userShardOrder);
		// 		//self::setDBInfo($userIndex->getShardDBInfo());
		// 	}
		// }

		$loadQuery=-1;
		if(static::$m__LQTableInsertCustomFunction){
			$func = static::$m__LQTableInsertCustomFunction;
			$loadQuery=$func($param);
		}else{
			
			if($param["data"][$pkey]){
				$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
			}
		}

		//LogManager::addLog("loadQuery:".$loadQuery);
		//LogManager::addLog("param check ".json_encode($param["data"],true));
		
		if($loadQuery!=-1){
			$obj->load($loadQuery);
		}

		if($obj->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"로드실패");
			return $r;
		}

		foreach ($param["data"] as $key => $value) {
			$obj->$key = $value;
			//LogManager::addLog("foreach $key -".json_encode($value,true));
		}

		$pkeyInclude = $autokey?false:true;
		//LogManager::addLog("check pkey include pk:".$pkey." and ak:".$autokey." result:".$pkeyInclude);
		if($obj->save($pkeyInclude)){
			$r["data"] = $obj->getArrayData(true);
			$r["shardIndex"]=$shardIndex;
			$r["result"]=ResultState::successToArray();
			return $r;
		}

		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"저장할수없습니다.");
		return $r;
	}


	public static function updateWithLQTable($param){
		self::initial();
		$admin = new AdminUser($_SESSION["admin_no"]);

		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");
		}
		if(!$admin->checkPermissionGroup(get_called_class(),"write")){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");	
		}
		
		if($param["enableComment"]==true){
			$mh = new ModifyHistory($param["data"]["memberID"]);
			$mh->writer = $admin->id;
			$mh->oldData=$param["oldData"];
			$mh->newData=$param["data"];
			$mh->reason=$param["reason"];
			$mh->category=get_called_class();
			$mh->save();
		}
		
		$klass = get_called_class();
		$shardIndex=$param["shardIndex"];

		$obj = new $klass();
		$obj->setDBShardOrder($shardIndex);

		// if(!static::$m__DBGroup){
		// 	if(self::isMainDBClass()){
		// 		self::setDBGroup("main");
		// 	}else if($shardIndex){
		// 		self::setDBGroup("user",null,$shardIndex);
		// 	}else{
		// 		$userIndex = UserIndex::create($param["data"]["memberID"]);
		// 		$obj->setDBShardOrder($userIndex->userShardOrder);
		// 	}
		// }

		$pkey = self::getPrimaryKey();
		$autokey = self::getAutoIncreaseKey();
		
		if(static::$m__LQTableUpdateCustomFunction){
			$func=static::$m__LQTableUpdateCustomFunction;
			$loadQuery=$func($param);
		}else{
			if($param["primaryKey"] && $param["primaryValue"]){
				$loadQuery = "`".$param["primaryKey"]."`=".$param["primaryValue"];
			}else{
				$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
			}

		}

		foreach ($param["data"] as $key => $value) {
			$obj->$key = $value;
			//LogManager::addLog("foreach $key -".json_encode($value,true));
		}
		
		LogManager::addLog("updatewithinsert loadquery is ".$loadQuery);
		$obj->load($loadQuery);


		unset($param["data"][$pkey]);
		
		if(!$obj->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDMYSQLQUERY,"로드실패");
			return $r;
		}

		foreach ($param["data"] as $key => $value) {
			$obj->$key = $value;
			//LogManager::addLog("foreach $key -".json_encode($value,true));
		}
		
		$pkeyInclude = $autokey?false:true;
		//LogManager::addLog("check pkey include pk:".$pkey." and ak:".$autokey." result:".$pkeyInclude);
		
		if($obj->save($pkeyInclude)){
			$r["data"] = $obj->getArrayData(true);
			$func = static::$m__LQTableSelectCustomFunction;
			if($func)$r["data"]=$func($r["data"]);
			$r["shardIndex"]=$param["shardIndex"];
			
			$r["result"]=ResultState::successToArray();
			return $r;
		}

		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"저장오류");
		return $r;
	}

	public static function deleteWithLQTable($param){
		self::initial();
		$admin = new AdminUser($_SESSION["admin_no"]);
		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");
		}
		if(!$admin->checkPermissionGroup(get_called_class(),"write")){
			return ResultState::makeReturn(ResultState::GDSECURITY,"권한이 없습니다.");	
		}

		if($param["enableComment"]==true){
			$mh = new ModifyHistory($param["data"]["memberID"]);
			$mh->writer = $admin->id;
			$mh->oldData=$param["data"];
			$mh->newData="delete";
			$mh->reason=$param["reason"];
			$mh->category=get_called_class();
			$mh->save();
		}
		
		$klass = get_called_class();
		$shardIndex=$param["shardIndex"];

		$obj = new $klass();
		$obj->setDBShardOrder($shardIndex);

		// if(!self::$m__DBGroup){
		// 	if(self::isMainDBClass()){
		// 		self::setDBGroup("main");
		// 	}else if($shardIndex){
		// 		self::setDBGroup("user",null,$shardIndex);
		// 	}else{
		// 		$userIndex = UserIndex::create($param["data"]["memberID"]);
		// 		$obj->setShardOrder($userIndex->userShardOrder);
		// 	}
		// }

		$pkey = self::getPrimaryKey();

		if(static::$m__LQTableDeleteCustomFunction){
			$func=static::$m__LQTableDeleteCustomFunction;
			$loadQuery=$func($param);
		}else{
			if($param["primaryKey"] && $param["primaryValue"]){
				$loadQuery = "`".$param["primaryKey"]."`=".$param["primaryValue"];
			}else{
				$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
			}
		}

		

		$obj->load($loadQuery);

		if(!$obj->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDMYSQLQUERY,"로드실패");
			return $r;
		}

		if($obj->remove()){
			$r["result"]=ResultState::successToArray();
			return $r;
		}

		$r["result"]=ResultState::toArray(ResultState::GDMYSQLQUERY,"삭제실패");
		return $r;


	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//static functions
	///////////////////////////////////////////////////////////////////////////////////////////////////

	// static public function getShardDBInfoList(){
	// 	return UserIndex::getShardDBInfoList();
	// }


	static public function getQueryResultWithShardOrder($query,$shardOrder,$isForRead=true){
		self::initial();

		$con = static::$m__DBGroup->getConnectionForRead(null,$shardOrder,self::getDBTable()); 
		//LogManager::addLog("getQueryResult query0->".$query);
		return self::getQueryResult($query,$con,$isForRead);
	}

	static public function getQueryResultWithShardKey($query,$shardKey,$isForRead=true){
		self::initial();

		$con = static::$m__DBGroup->getConnectionForRead($shardKey,null,self::getDBTable()); 
		//LogManager::addLog("getQueryResult query0->".$query);
		return self::getQueryResult($query,$con,$isForRead);
	}

	static public function getQueryResult($query,$dbcon=NULL,$isForRead=true){
		self::initial();

		if(static::$m__qCnt==-1){
			$newvalue1=1;
			static::$m__qCnt =& $newvalue1;
			$newvalue2=array();
			static::$m__dbList =& $newValue2;
			if($isForRead){
				static::$m__dbList = static::$m__DBGroup->getSlaveList();
			}else{
				static::$m__dbList = static::$m__DBGroup->getMasterList();
			}
		}

		if(static::$m__qResult==-1){
			$newvalue2="";
			static::$m__qResult =& $newvalue2;
		}else if(!static::$m__qResult){
			static::$m__qCnt=1;
		}
		
		if(static::$m__qCnt>=count(static::$m__dbList)){
			static::$m__qCnt=1;
			return false;
		}
		
		//if(!static::$m__qResult){
		if($dbcon==NULL) $dbconn = static::$m__dbList[static::$m__qCnt]->getConnection();
		else $dbconn = $dbcon;
		if(!$dbconn)return null;
		static::$m__qResult = mysql_query($query,$dbconn);
		//}
		
		if(!static::$m__qResult){
			return NULL;
		}

		//$result = mysql_fetch_array(static::$m__qResult,MYSQL_ASSOC);

		static::$m__qCnt++;
		//LogManager::addLog("pp m__qCnt to".static::$m__qCnt);
		if(static::$m__qResult || $dbcon!=NULL)return static::$m__qResult;
		
		//static::$m__qResult="";
		//return self::getRowByQuery($where);
	}

	static public function getShardIndexNow(){
		return static::$m__qCnt;
	}

	static public function getRowByQueryWithRandom($where="",$fields="*"){
		self::initial();

		$con = static::$m__DBGroup->getConnectionForReadByRand(); 
		return self::getRowByQuery($where,$con,$fields);
	}

	static public function getRowByQueryWithShardOrder($where="",$shardOrder,$fields="*"){
		self::initial();

		$con = static::$m__DBGroup->getConnectionForRead(null,$shardOrder,self::getDBTable()); 
		return self::getRowByQuery($where,$con,$fields);
	}

	static public function getRowByQueryWithShardKey($where="",$shardKey,$fields="*"){
		self::initial();

		$con = static::$m__DBGroup->getConnectionForRead($shardKey,null,self::getDBTable()); 
		return self::getRowByQuery($where,$con,$fields);
	}

	static public function getRowByQuery($where="",$dbcon=NULL,$fields="*",$whereFunc=null){

		self::initial();
		if(static::$m__qCnt==-1){
			$newvalue1=1;
			static::$m__qCnt =& $newvalue1;
			static::$m__qCnt=1;
			$newvalue2=array();
			static::$m__dbList =& $newValue2;
			static::$m__dbList = static::$m__DBGroup->getMasterList();
		}

		if(static::$m__qResult==-1){
			$newvalue2="";
			static::$m__qResult =& $newvalue2;
		}

		// if(self::isMainDBClass()){
		// 	if(!static::$m__qResult){
		// 		static::$m__qResult = mysql_query("select ".$fields." from ".DBManager::getMT(get_called_class())." ".$where,DBManager::getMainConnection());
		// 	}
		// 	//LogManager::addLog("select ".$fields." from ".DBManager::getMT(get_called_class())." ".$where);
		// 	if(static::$m__qResult)$result = mysql_fetch_array(static::$m__qResult,MYSQL_ASSOC);
		// 	if(!$result)static::$m__qResult=null;
		// 	return $result;
		// }

		// $dbInfoList = self::getShardDBInfoList();

		if(static::$m__qCnt>=count(static::$m__dbList)){
			static::$m__qCnt=1;
			return false;
		}
		
		//첫시작
		if(!static::$m__qResult){
			if($dbcon==NULL) $dbconn = static::$m__dbList[static::$m__qCnt]->getSlaveByRand()->getConnection();
			else $dbconn = $dbcon;
			static::$m__qResult =& $newvalue4;
			static::$m__qResult = mysql_query("select ".$fields." from ".static::$m__DBTable." ".$where,$dbconn);
		}
		
		//구문에 오류가 있다면 리턴
		if(!static::$m__qResult)return NULL;

		//LogManager::addLog($where." with shardOrder is ".static::$m__qCnt);
		$result = mysql_fetch_array(static::$m__qResult,MYSQL_ASSOC);

		//결과가 있으면 리턴
		if($result || $dbcon!=NULL)return $result;
		
		
		//결과가 없으면 다시 한번 시도
		static::$m__qResult="";
		static::$m__qCnt++;

		if($whereFunc)$where = $whereFunc(static::$m__qCnt);
		return self::getRowByQuery($where,$dbcon,$fields,$whereFunc);
	}

	static public function getObjectByQueryWithShardKey($where="",$shardKey){
		self::initial();

		$con = static::$m__DBGroup->getConnectionForRead($shardKey,null,self::getDBTable()); 
		return self::getObjectByQuery($where,$con);
	}

	static public function getObjectByQuery($where="",$dbcon=NULL,$fields="*"){
		self::initial();
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

	static public function removeRowByQueryWithShardKey($where="",$shardKey){
		self::initial();

		$con = static::$m__DBGroup->getConnectionForWrite($shardKey,null,self::getDBTable()); 
		return self::removeRowByQuery($where,$con);
	}

	static public function removeRowByQuery($where="",$dbcon=NULL){
		self::initial();
		if(!$where)return false;

		// if(self::isMainDBClass()){
		// 	$result=mysql_query("delete from ".DBManager::getMT(get_called_class())." ".$where,DBManager::getMainConnection());
		// 	return $result;
 	// 	}

		if($dbcon){
			$result=mysql_query("delete from ".static::$m__DBTable." ".$where,$dbcon);
			//LogManager::addLog("delete gogo is ".$result);
			return $result;
		}

		$dbInfoList = static::$m__DBGroup->getMasterList();
		for($i=1;$i<count($dbInfoList);$i++){
			$dbconn = $dbInfoList[$i]->getConnection();
			$result=mysql_query("delete from ".static::$m__DBTable." ".$where,$dbconn);
		}

		return true;
	}


}

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
?>
