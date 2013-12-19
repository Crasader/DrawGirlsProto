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
		echo'{"result":"ok","query":"'.addslashes($query).'","data":'.json_encode($data).'}'; mysql_close(); exit; 
	}
	
	echo'{"result":"fail","query":"'.addslashes($query).'"}'; mysql_close(); exit; 
}else if($mode=="delete"){
	if(!$primaryKey || !$primaryValue || !$table){ echo'{"result":"fail"}'; mysql_close(); exit; }
	$query = "delete from $table where $primaryKey = $primaryValue";
	if(mysql_query($query,DBManager::get()->getMainConnection())){
		echo'{"result":"ok","query":"'.addslashes($query).'"}'; mysql_close(); exit; 
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
	if(!$table){ echo'{"result":"fail"}'; mysql_close();exit; }
	$qstr="";
	if($where)$qstr .= "where ".$where." ";
	if($sort)$qstr .= "order by ".$sort." ";
	if($limit)$qstr .= "limit ".$limit;
	$query = @mysql_query("select * from `$table` ".$qstr,DBManager::get()->getMainConnection());
	echo"[";
	$isFirst = true;
	while($data = @mysql_fetch_array($query)){
		
		if(!$isFirst){
			echo ",";
		}
		$isFirst=false;
		for($i=0;$i<count($data);$i++){
			unset($data[$i]);
		}
		echo json_encode($data);	
	}
	echo"]";
}


DBManager::get()->closeDB();

?>

