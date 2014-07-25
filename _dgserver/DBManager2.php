
class DBGroup{
	static public $m_groupList;

	public $m_name;
	public $m_masterList;
	public $m_newShardKeyFunc;
	public $m_getShardKeyFunc;

	public static function create($name){
		self::$m_groupList[$name] = new DBGroup($name);
		return self::$m_groupList[$name];
	}
	public function __construct($name){
		$this->m_name=$name;
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
		$this->m_server->selecteDB($this->m_name);
		return $conn;
	}
	
	public function closeConnection(){
		if($this->m_server)
			$this->m_server->closeConnection();
	}

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DBTable2{
	public $m__data;
	public $m__isLoaded=false;

	static public $m__DBTable=NULL;
	static public $m__primarykey=NULL;
	static public $m__autoIncreaseKey=NULL;
	static public $m__DBGroup;
	static public $m__qResult = -1;
	static public $m__qCnt = -1;
	static public $m__LQTableSelectCustomFunction=null;
	static public $m__LQTableInsertCustomFunction=null;
	static public $m__LQTableUpdateCustomFunction=null;
	static public $m__LQTableDeleteCustomFunction=null;
	static public $m__LQTableSelectQueryCustomFunction=null;


	public function __construct(){
		static::$m__DBTable=&$newvalue;
		static::$m__DBTable = DBManager::getST(get_called_class());

		if(!static::$m__DBTable){
			static::$m__DBTable = DBManager::getMT(get_called_class());
		}
		LogManager::addLog("db table auto select by ".get_called_class()." ==>".static::$m__DBTable);
	}
	
	static public function isMainDBClass(){
		if(DBManager::getST(get_called_class()))return false;

		return true;
	}

	public static function getPrimaryKey(){
		return static::$m__primarykey;
	}
	
	public function getPrimaryValue(){
		$pkey = static::$m__primarykey;
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
		
		if(static::$m__primarykey && !$isIncludePrimaryKey){
			unset($arraydata[static::$m__primarykey]);
		}
		
		return $arraydata;
	}
	
	public function updateQuery($where=false){
		if(!$where)$where = "where ".static::$m__primarykey."=".$this->getPrimaryValue();
		return DBManager::updateQuery(static::$m__DBTable,$this->getArrayData(),$where);
	}
	
	public function insertQuery($isIncludePrimaryKey=false){
		return DBManager::insertQuery(static::$m__DBTable,$this->getArrayData($isIncludePrimaryKey));
	}
	
	public function save($isIncludePrimaryKey=false){
		LogManager::addLog("DBRow save function");
		if(!static::$m__DBInfo)return false;

		if(static::$m__DBInfo->getConnection()){
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
				if($isInsert && static::$m__autoIncreaseKey){
					$this->{static::$m__autoIncreaseKey}=mysql_insert_id($this->getDBConnection());
					LogManager::addLog("find autoIncreaseKey ".static::$m__autoIncreaseKey." value is ".$this->m__data[static::$m__autoIncreaseKey]);
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
		if(!static::$m__DBTable)return false;
		if(!static::$m__DBInfo)return false;
		if(!static::$m__DBInfo->getConnection())return false;
		if(!$where && !$this->getPrimaryValue())return false;
		
		$query = "select * from ".static::$m__DBTable." where `".static::$m__primarykey."`='".$this->getPrimaryValue()."'";
		
		if($where){
			$query = "select * from ".static::$m__DBTable." where ".$where;
		}
		
		LogManager::addLog("load query ".$query);

		$this->m__isLoaded=false;

		$result=mysql_query($query,$this->getDBConnection());
		$tempdata = null;
		if($result)$tempdata = mysql_fetch_array($result,MYSQL_ASSOC);
		
		if($tempdata && $tempdata[static::$m__primarykey]){
			$this->m__data = $tempdata;
			LogManager::addLog("load ok, primarykey is".$tempdata[static::$m__primarykey]);
			$this->m__isLoaded=true;
			return true;
		}
		
		LogManager::addLog("load fail".mysql_error());
		return false;
	}
	
	public function remove(){
		if($this->isLoaded()){
			$query = "DELETE FROM ".static::$m__DBTable." WHERE `".static::$m__primarykey."`='".$this->getPrimaryValue()."'";
			$result = mysql_query($query,static::$m__DBInfo->getConnection());
			if($result)return true;
		}
		return false;
	}
	// public function setDBInfo($dbInfo){
	// 	static::$m__DBInfo = $dbInfo;
	// }

	// public function getDBInfo(){
	// 	return static::$m__DBInfo;
	// }

	public static function getDBInfo(){
		return static::$m__DBInfo;
	}

	public static function setDBInfo($dbInfo){
		static::$m__DBInfo =& $newvalue;
		static::$m__DBInfo = $dbInfo;
	}

	public function getDBConnection(){
		return static::$m__DBInfo->getConnection();
	}
	
	public static function setPrimarykey($pkey,$isAutoincrease=false){
		static::$m__primarykey =& $newvalue;
		static::$m__primarykey=$pkey;
		if($isAutoincrease)self::setAutoIncreaseKey($pkey);
	}
	
	public static function setAutoIncreaseKey($pkey){
		static::$m__autoIncreaseKey =& $newvalue;
		static::$m__autoIncreaseKey=$pkey;
	}
	
	public function setDBTable($dbTable){
		static::$m__DBTable = $dbTable;
	}
	public function getDBTable(){
		return static::$m__DBTable;
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

	static public function setLQTableSelectQueryCustomFunction($func){
		$newValue="";
		static::$m__LQTableSelectQueryCustomFunction=&$newValue;
		static::$m__LQTableSelectQueryCustomFunction=$func;
	}
	static public function setLQTableSelectCustomFunction($func){
		LogManager::addLog("set custom func");
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


		$admin = new AdminUser($_SESSION["admin_no"]);
		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY);
		}


		$klass = get_called_class();
		$obj = new $klass();

		if(self::isMainDBClass()){
			$q = mysql_query('DESCRIBE '.static::$m__DBTable,DBManager::getMainConnection());
		}else{
			$q = mysql_query('DESCRIBE '.static::$m__DBTable,DBManager::getConnectionByShardIndex(1));
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

	public static function selectWithLQTable($param){
		$admin = new AdminUser($_SESSION["admin_no"]);
		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY);
		}

		$klass = get_called_class();
		$obj = new $klass();

		$dataList = array();


		//쿼리 만들기
		$query = "";
		if(static::$m__LQTableSelectQueryCustomFunction){
			$func = static::$m__LQTableSelectQueryCustomFunction;
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
			
			LogManager::addLog("call custom function1 ".get_called_class());
			$func = static::$m__LQTableSelectCustomFunction;
			if($func){
				LogManager::addLog("call custom function2");
				$dataList[]=$func($rData);
			}
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

	public static function insertWithLQTable($param){
		$admin = new AdminUser($_SESSION["admin_no"]);
		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY);
		}

		$klass = get_called_class();
		$obj = new $klass();

		$shardIndex=$param["shardIndex"];
		if(!self::getDBInfo()){
			if(self::isMainDBClass()){
				self::setDBInfo(DBManager::getMainDBInfo());
			}else if($shardIndex){
				self::setDBInfo(DBManager::getDBInfoByShardIndex($shardIndex));
			}else{
				$userIndex = UserIndex::create($param["data"]["memberID"]);
				self::setDBInfo($userIndex->getShardDBInfo());
			}
		}

		$loadQuery=-1;
		if(static::$m__LQTableInsertCustomFunction){
			$func = static::$m__LQTableInsertCustomFunction;
			$loadQuery=$func($param);
		}else{
			$pkey = self::getPrimarykey();
			if($param["data"][$pkey]){
				$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
			}
		}

		LogManager::addLog("loadQuery:".$loadQuery);
		LogManager::addLog("param check ".json_encode($param["data"],true));
		
		if($loadQuery!=-1){
			$obj->load($loadQuery);
		}

		if($obj->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"로드실패");
			return $r;
		}

		foreach ($param["data"] as $key => $value) {
			$obj->$key = $value;
			LogManager::addLog("foreach $key -".json_encode($value,true));
		}

		if($obj->save()){
			$r["data"] = $obj->getArrayData(true);
			$r["shardIndex"]=$shardIndex;
			$r["result"]=ResultState::successToArray();
			return $r;
		}

		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE,"저장할수없습니다.");
		return $r;
	}


	public static function updateWithLQTable($param){
		$admin = new AdminUser($_SESSION["admin_no"]);
		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY);
		}

		$klass = get_called_class();
		$obj = new $klass();

		if(!self::getDBInfo()){
			if(self::isMainDBClass()){
				self::setDBInfo(DBManager::getMainDBInfo());
			}else if($param["shardIndex"]){
				self::setDBInfo(DBManager::getDBInfoByShardIndex($param["shardIndex"]));
			}else{
				$userIndex = UserIndex::create($param["data"]["memberID"]);
				self::setDBInfo($userIndex->getShardDBInfo());
			}
		}

		$pkey = self::getPrimarykey();

		if(static::$m__LQTableUpdateCustomFunction){
			$func=static::$m__LQTableUpdateCustomFunction;
			$loadQuery=$func($param);
		}else{
			
			$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
		}


		$obj->load($loadQuery);


		unset($param["data"][$pkey]);
		
		if(!$obj->isLoaded()){
			$r["result"]=ResultState::toArray(ResultState::GDMYSQLQUERY,"로드실패");
			return $r;
		}

		foreach ($param["data"] as $key => $value) {
			$obj->$key = $value;
		}

		if($obj->save()){
			$r["data"] = $obj->getArrayData(true);
			$func = static::$m__LQTableSelectCustomFunction;
			if($func)$r["data"]=$func($r["data"]);
			$r["shardIndex"]=$param["shardIndex"];
			$r["result"]=ResultState::successToArray();
			return $r;
		}

		$r["result"]=ResultState::toArray(ResultState::GDDONTSAVE);
		return $r;
	}

	public static function deleteWithLQTable($param){
		$admin = new AdminUser($_SESSION["admin_no"]);
		if(!$admin->isLogined()){
			return ResultState::makeReturn(ResultState::GDSECURITY);
		}

		$klass = get_called_class();
		$obj = new $klass();

		$shardIndex=$param["shardIndex"];
		if(!self::getDBInfo()){
			if(!self::isMainDBClass() && $shardIndex){
				self::setDBInfo(DBManager::getDBInfoByShardIndex($shardIndex));
			}else{
				self::setDBInfo(DBManager::getMainDBInfo());
			}
		}

		$pkey = self::getPrimarykey();

		if(static::$m__LQTableDeleteCustomFunction){
			$func=static::$m__LQTableDeleteCustomFunction;
			$loadQuery=$func($param);
		}else{
			$loadQuery = "`".$pkey."`=".$param["data"][$pkey];
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

	static public function getShardDBInfoList(){
		return UserIndex::getShardDBInfoList();
	}

	static public function getQueryResult($query,$dbcon=NULL){
		if(static::$m__qCnt==-1){
			$newvalue1=1;
			static::$m__qCnt =& $newvalue1;
			static::$m__qCnt=1;
		}
		if(static::$m__qResult==-1){
			$newvalue2="";
			static::$m__qResult =& $newvalue2;
			static::$m__qResult="";
		}

		if(self::isMainDBClass()){
			LogManager::addLog("getqueryresult2->".$query);
			$result = mysql_query($query,DBManager::getMainConnection());
			return $result;
		}

		$dbInfoList = self::getShardDBInfoList();


		if(static::$m__qCnt>=count($dbInfoList)){
			static::$m__qCnt=1;
			return false;
		}
		
		if($dbcon==NULL){

			$dbconn = $dbInfoList[static::$m__qCnt]->getConnection();
			
		}else $dbconn = $dbcon;


		LogManager::addLog("getqueryresult2->".$query);
		$result = mysql_query($query,$dbconn);


		static::$m__qCnt++;
		static::$m__qResult=null;
		
		return $result;
	}
	static public function getShardIndexNow(){
		return static::$m__qCnt;
	}
	static public function getRowByQuery($where="",$dbcon=NULL,$fields="*"){
		if(static::$m__qCnt==-1){
			$newvalue1=1;
			static::$m__qCnt =& $newvalue1;
		}

		if(static::$m__qResult==-1){
			$newvalue2="";
			static::$m__qResult =& $newvalue2;
		}

		if(self::isMainDBClass()){
			if(!static::$m__qResult){
				static::$m__qResult = mysql_query("select ".$fields." from ".DBManager::getMT(get_called_class())." ".$where,DBManager::getMainConnection());
			}
			LogManager::addLog("select ".$fields." from ".DBManager::getMT(get_called_class())." ".$where);
			if(static::$m__qResult)$result = mysql_fetch_array(static::$m__qResult,MYSQL_ASSOC);
			if(!$result)static::$m__qResult=null;
			return $result;
		}

		$dbInfoList = self::getShardDBInfoList();
		if(static::$m__qCnt>=count($dbInfoList)){
			static::$m__qCnt=1;
			return false;
		}
		
		if(!static::$m__qResult){
			if($dbcon==NULL)$dbconn = $dbInfoList[static::$m__qCnt]->getConnection();
			else $dbconn = $dbcon;
			static::$m__qResult =& $newvalue4;
			static::$m__qResult = mysql_query("select ".$fields." from ".DBManager::getST(get_called_class())." ".$where,$dbconn);
			LogManager::addLog("ok go select ".$fields." from ".DBManager::getST(get_called_class())." ".$where);
		}

		$result = mysql_fetch_array(static::$m__qResult,MYSQL_ASSOC);

		if($result || $dbcon!=NULL)return $result;
		
		static::$m__qResult="";
		static::$m__qCnt++;

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
			$result=mysql_query("delete from ".DBManager::getMT(get_called_class())." ".$where,DBManager::getMainConnection());
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

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
