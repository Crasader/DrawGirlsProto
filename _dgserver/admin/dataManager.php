<?php
include "../lib.php";		


$mode = $_POST['mode'];
$primaryKey = $_POST['primaryKey'];
$primaryValue = $_POST['primaryValue'];
$table = $_POST['table'];
$data = $_POST['data'];
$sort = $_POST['sort'];
$where = $_POST['where'];
$limit = $_POST['limit'];

if(!$table)$table=$_GET['table'];
/*
		$data = json_decode(stripslashes($data));	
	if($table==$TABLE_STAGE){
		$data["version"]=$data["version"]+1;
		kvManager::increase("stageVersion_".$data[no]);
		kvManager::increase("puzzleVersion_".$data[puzzle]);
	}
	if($table==$TABLE_PUZZLE){
		$data["version"]=$data["version"]+1;
		kvManager::increase("puzzleVersion_".$data[no]);
	}
	
*/
	if($data)$data = json_decode($data,true);	
	
	if($table==$TABLE_STAGE && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("stageVer_".$data[no]);
		kvManager::increase("puzzleVer_".$data[puzzle]);

		$cardlist = json_decode($data["cards"],true);

		for($i=0;$i<count($cardlist);$i++){
			mysql_query("update $TABLE_CARD set `stage`=".$data[no].", `grade`=".($i+1)." where no=".$cardlist[$i]);
		}
	}
	if($table==$TABLE_EVENTSTAGE && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("eStageVer_".$data[no]);
		kvManager::increase("eStageListVer");
	}
	if($table==$TABLE_PUZZLE && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("puzzleVer_".$data[no]);
		kvManager::increase("puzzleListVer");
	}
	if($table==$TABLE_CARD && $data){
		kvManager::increase("cardVer_".$data[no]);
		kvManager::increase("stageVer_".$data[stage]);
	}
	if($table==$TABLE_CHARACTER && $data){
		kvManager::increase("charListVer");
	}
	if($table==$TABLE_MONSTER && $data){
		kvManager::increase("monsterListVer");
	}
	
if($mode=="update"){
	if(!$primaryKey || !$primaryValue || !$table){ echo'{"result":"fail"}'; mysql_close(); exit; }
	

	
	$query = lq_query_update($data,$table,"where $primaryKey = $primaryValue");
	

	if(mysql_query($query,DBManager::get()->getMainConnection())){
		//"query":"'.addslashes($query).'"
		echo'{"result":"ok","data":'.json_encode($data).'}'; mysql_close(); exit; 
	}
	
	echo'{"result":"fail","query":"'.addslashes($query).'"}'; mysql_close(); exit; 
}else if($mode=="delete"){
	if(!$primaryKey || !$primaryValue || !$table){ echo'{"result":"fail"}'; mysql_close(); exit; }
	$query = "delete from $table where $primaryKey = $primaryValue";
	if(mysql_query($query,DBManager::get()->getMainConnection())){
		//,"query":"'.addslashes($query).'"
		echo'{"result":"ok"}'; mysql_close(); exit; 
	}
	echo'{"result":"fail","query":"'.addslashes($query).'"}';mysql_close(); exit; 

}else if($mode=="insert"){
	//$data = json_decode($data,true);
	
	if(!$table){ echo'{"result":"fail"}'; mysql_close();exit; }
	
	$query = lq_query_insert($data,$table);

	if(mysql_query($query,DBManager::get()->getMainConnection())){
		echo'{"result":"ok"}'; mysql_close();exit; 
	}
	
	echo'{"result":"fail","query":"'.addslashes($query).'","data":'.json_encode($data).'}'; mysql_close();exit; 


	
}else{
	if(!$table){ echo'{"result":"fail"}'; @mysql_close();exit; }

	if($table==$TABLE_MESSAGE){
		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    while($data = Message::getMessageByQuery($qstr)){
			$memberID = $data["memberID"];
			$friendID = $data["friendID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($data["memberID"]);
			}
			if(!$userList[$friendID]){
				$userList[$friendID] = new UserData($data["friendID"]);
			}

			$userInfo = $userList[$memberID];
			$friendInfo = $userList[$friendID];
			$data["nick"]=$userInfo->m_nick;
			$data["friendNick"]=$friendInfo->m_nick;

			$dataList[]=$data;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		usort($dataList, build_sorter('regDate'));

		rsort($dataList); 

	    echo json_encode($dataList);
	}else if($table==$TABLE_LOG){

		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$logList = array();
	    $userList = array();
	    while($userlog = UserLog::getUserLogByQuery($qstr)){
			$memberID = $userlog["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userlog["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userlog["nick"]=$userInfo->m_nick;

			$logList[]=$userlog;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		usort($logList, build_sorter('regDate'));

		rsort($logList); 

	    echo json_encode($logList);

	}else{
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$query = @mysql_query("select * from `$table` ".$qstr,DBManager::get()->getMainConnection());
		echo"[";
		$isFirst = true;
		while($data = @mysql_fetch_array($query,MYSQL_ASSOC)){
			
			if(!$isFirst){
				echo ",";
			}
			$isFirst=false;
			echo json_encode($data);	
		}
	echo"]";
	}
}


DBManager::get()->closeDB();

?>

