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
	if($table==DBManager::get()->getMT("stage")){
		$data["version"]=$data["version"]+1;
		kvManager::increase("stageVersion_".$data[no]);
		kvManager::increase("puzzleVersion_".$data[puzzle]);
	}
	if($table==DBManager::get()->getMT("puzzle")){
		$data["version"]=$data["version"]+1;
		kvManager::increase("puzzleVersion_".$data[no]);
	}
	
*/

	if($data)$data = json_decode($data,true);	
	
	if($table==DBManager::get()->getMT("stage") && $data){
		$data["version"]=$data["version"]+1;

		$pResult = mysql_query("select * from ".DBManager::get()->getMT("book")." where no=$data[book]",DBManager::get()->getMainConnection());
		if($pResult){
			$pData = mysql_fetch_assoc($pResult);
			kvManager::increase("bookVer_".$pData[no]);
			kvManager::increase("bookListVer");
			mysql_query("update ".DBManager::get()->getMT("book")." set version=version+1 where no=".$data[book],DBManager::get()->getMainConnection());
		}
		

		$cardlist = json_decode($data["cards"],true);

		for($i=0;$i<count($cardlist);$i++){
			mysql_query("update ".DBManager::get()->getMT("card")." set `stage`=".$data[no].", `grade`=".($i+1)." where no=".$cardlist[$i],DBManager::get()->getMainConnection());
		}
	}

	if($table==DBManager::get()->getMT("piece") && $data){
		$data["version"]=$data["version"]+1;
		$pResult = mysql_query("select * from ".DBManager::get()->getMT("puzzle")." where no=$data[puzzle]",DBManager::get()->getMainConnection());
		
		if($pResult){
			$pData = mysql_fetch_assoc($pResult);
			kvManager::increase("puzzleVer_".$pData[no]);
			kvManager::increase("puzzleListVer");
			mysql_query("update ".DBManager::get()->getMT("puzzle")." set version=version+1 where no=".$data[puzzle],DBManager::get()->getMainConnection());
		}
		

		$cardlist = json_decode($data["cards"],true);

		for($i=0;$i<count($cardlist);$i++){
			mysql_query("update ".DBManager::get()->getMT("card")." set `stage`=".$data[no].", `grade`=".($i+1)." where no=".$cardlist[$i],DBManager::get()->getMainConnection());
		}
	}





	if($table==DBManager::get()->getMT("eventstage") && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("eStageVer_".$data[no]);
		kvManager::increase("eStageListVer");
	}
	
	//puzzle
	if($table==DBManager::get()->getMT("puzzle") && $data["isEvent"]==1){
		$data["version"]=$data["version"]+1;
		kvManager::increase("puzzleVer_".$data[no]);
		kvManager::increase("ePuzzleListVer");
		mysql_query("update ".DBManager::get()->getMT("puzzle")." set version=version+1 where no=".$data[no],DBManager::get()->getMainConnection());

		reloadPuzzleInfo();
	}else if($table==DBManager::get()->getMT("puzzle") && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("puzzleVer_".$data[no]);
		kvManager::increase("puzzleListVer");

		reloadPuzzleInfo();
	}else if($table==DBManager::get()->getMT("puzzle")){
		
		reloadPuzzleInfo();
	}

	//book
	if($table==DBManager::get()->getMT("book") && $data["isEvent"]==1){
		$data["version"]=$data["version"]+1;
		kvManager::increase("bookVer_".$data[no]);
		kvManager::increase("eBookListVer");
		mysql_query("update ".DBManager::get()->getMT("book")." set version=version+1 where no=".$data[no],DBManager::get()->getMainConnection());

	}else if($table==DBManager::get()->getMT("book") && $data){
		$data["version"]=$data["version"]+1;
		kvManager::increase("bookVer_".$data[no]);
		kvManager::increase("bookListVer");
	}

	if($table==DBManager::get()->getMT("card") && $data){
		kvManager::increase("cardVer_".$data[no]);
		kvManager::increase("stageVer_".$data[stage]);
	}
	if($table==DBManager::get()->getMT("character") && $data){
		kvManager::increase("charListVer");
	}
	if($table==DBManager::get()->getMT("monster") && $data){
		kvManager::increase("monsterListVer");
	}

	if($table==DBManager::get()->getMT("shop") && $data){
		kvManager::increase("shopListVer");
	}
	
if($mode=="update"){
	if(!$primaryKey || !$primaryValue || !$table){ echo'{"result":"fail","msg":"기본키, 기본키값, 테이블 미정의"}'; mysql_close(); exit; }
	if($table==DBManager::getST("weeklyscore")){
		$dataObj = new WeeklyScore($data["memberID"],$data["weekNo"]);
		$dataObj->memberID=$data["memberID"];
		$dataObj->regWeek=$data["regWeek"];
		$dataObj->score=$data["score"];
		$dataObj->regDate=$data["regDate"];
		$dataObj->data=$data["data"];
		$result=$dataObj->save();
		$data[log] = LogManager::get()->getLogAndClear();
		if($result)echo'{"result":"ok","data":'.json_encode($data).'}';
		else echo'{"result":"fail","msg":"세이브쿼리실패"}';

	}else if($table==DBManager::getST("stagescore")){
		$dataObj = new StageScore($data["stageNo"],$data["memberID"]);
		LogManager::get()->addLog("modify stagescore");
		LogManager::get()->addLog("modify stagescore is load? ".$dataObj->isLoaded());


		$dataObj->memberID=$data["memberID"];
		$dataObj->stageNo=$data["stageNo"];
		$dataObj->score=$data["score"];
		$dataObj->regDate=$data["regDate"];
		$dataObj->data=$data["data"];
		$result=$dataObj->save();
		$data[log] = LogManager::get()->getLogAndClear();
		if($result)echo'{"result":"ok","data":'.json_encode($data).'}';
		else echo'{"result":"fail","msg":"세이브쿼리실패"}';

	}else if($table==DBManager::getST("message")){
		//보내기
		$message = new Message($data["memberID"],$data["no"]);
		$message->memberID=$data["memberID"];
		$message->content=$data["content"];
		$message->regDate=$data["regDate"];
		$message->friendID=$data["friendID"];
		$message->type=$data["type"];
		$message->isSendMsg=$data["isSendMsg"];
		$result=$message->save();
		if($result)echo'{"result":"ok","data":'.json_encode($data).'}';
		else echo'{"result":"fail","msg":"세이브쿼리실패"}';

	}else if($table==DBManager::getST("userdata")){
		//보내기
		$userdata = new UserData($data["memberID"]);
		$userdata->data = $data["data"];
		$userdata->friendList = $data["friendList"];

		$result=$userdata->save();
		if($result)echo'{"result":"ok","data":'.json_encode($data).'}';
		else echo'{"result":"fail","msg":"세이브쿼리실패"}';

	}else{
	
		$query = lq_query_update($data,$table,"where $primaryKey = $primaryValue");
		

		if(mysql_query($query,DBManager::get()->getMainConnection())){
			//"query":"'.addslashes($query).'"
			echo'{"result":"ok","data":'.json_encode($data).'}'; mysql_close(); exit; 
		}
		
		echo'{"result":"fail","msg":"세이브쿼리실패 '.addslashes($query).'"}'; mysql_close(); exit; 

	}
}else if($mode=="delete"){
	if(!$primaryKey || !$primaryValue || !$table){ echo'{"result":"fail","msg":"기본키,기본키값,테이블 미정의"}'; mysql_close(); exit; }

	if($table==DBManager::getST("message") || $table==DBManager::getST("userdata")){
		//보내기
		// $message = new Message($data["memberID"],$data["no"]);
		// $result = $message->remove();
		// if($result)echo'{"result":"ok"}';
		// else 

		echo'{"result":"fail","msg":"메세지와 유저데이터는 수정불가"}';
	}else{
		$query = "delete from $table where $primaryKey = $primaryValue";
		if(mysql_query($query,DBManager::get()->getMainConnection())){
			//,"query":"'.addslashes($query).'"
			echo'{"result":"ok"}'; mysql_close(); exit; 
		}
		echo'{"result":"fail","msg":"세이브쿼리실패 '.addslashes($query).'"}'; mysql_close(); exit; 
	}
}else if($mode=="insert"){
	//$data = json_decode($data,true);
	


	if(!$table){ echo'{"result":"fail","msg":"테이블 미정의"}'; mysql_close();exit; }
	

	if($table==DBManager::getST("message")){
		//보내기
		$message = new Message();
		$message->memberID=$data["memberID"];
		$message->content=$data["content"];
		$message->regDate=TimeManager::get()->getCurrentDateString();
		$message->friendID=$data["friendID"];
		$message->type=$data["type"];
		$message->isSendMsg=$data["isSendMsg"];
		$r["send"]=$message->send();
		echo'{"result":"ok"}';
	}else{

		$query = lq_query_insert($data,$table);

		if(mysql_query($query,DBManager::get()->getMainConnection())){
			$no = mysql_insert_id(DBManager::get()->getMainConnection());
			$primaryString="";
			if($no)$primaryString=',"'.$primaryKey.'":'.$no;
			echo'{"result":"ok" '.$primaryString.'}'; mysql_close();exit; 
		}


		echo'{"result":"fail","msg":"쿼리실패 '.addslashes($query).'","data":'.json_encode($data).'}'; mysql_close();exit; 
	}

	
}else{
	if(!$table){ echo'{"result":"fail","msg":"테이블미정의"}'; @mysql_close();exit; }

	if($table==DBManager::getST("stagescore")){
		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    //testtest
	    while($userdata = StageScore::getRowByQuery($qstr)){
			$memberID = $userdata["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userdata["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userdata["nick"]=$userInfo->nick;

			$dataList[]=$userdata;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		$resort = "regDate";
		if($sort){
			$resort = explode(" ",$sort);
			$resort = $resort[0];
		}
		usort($dataList, build_sorter($resort));

		rsort($dataList); 

		$resultData["result"]="ok";
		$resultData["data"]=$dataList;
	    echo json_encode($resultData);
	}elseif($table==DBManager::getST("weeklyscore")){
		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    //testtest
	    while($userdata = WeeklyScore::getRowByQuery($qstr)){
			$memberID = $userdata["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userdata["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userdata["nick"]=$userInfo->nick;

			$dataList[]=$userdata;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		$resort = "regDate";
		if($sort){
			$resort = explode(" ",$sort);
			$resort = $resort[0];
		}
		usort($dataList, build_sorter($resort));

		rsort($dataList); 

		$resultData["result"]="ok";
		$resultData["data"]=$dataList;
	    echo json_encode($resultData);
	}else if($table==DBManager::getST("message")){
		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    while($data = Message::getRowByQuery($qstr)){
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
			$data["nick"]=$userInfo->nick;
			$data["friendNick"]=$friendInfo->nick;

			$dataList[]=$data;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		usort($dataList, build_sorter('regDate'));

		rsort($dataList); 

		$resultData["result"]="ok";
		$resultData["data"]=$dataList;
	    echo json_encode($resultData);
	}else if($table==DBManager::getST("userlog")){

		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$logList = array();
	    $userList = array();
	    while($userlog = UserLog::getRowByQuery($qstr)){
			$memberID = $userlog["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userlog["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userlog["nick"]=$userInfo->nick;

			$logList[]=$userlog;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		usort($logList, build_sorter('regDate'));

		rsort($logList); 

		$resultData["result"]="ok";
		$resultData["data"]=$logList;
	    echo json_encode($resultData);

	}else if($table == DBManager::getST("userdata")){

		$isFirst = true;
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$dataList = array();
	    $userList = array();
	    //testtest
	    while($userdata = UserData::getRowByQuery($qstr)){
			$memberID = $userdata["memberID"];
			if(!$userList[$memberID]){
				$userList[$memberID] = new UserData($userdata["memberID"]);
			}

			$userInfo = $userList[$memberID];
			
			$userdata["nick"]=$userInfo->nick;

			$dataList[]=$userdata;
	    }

	    function build_sorter($key) {
    		return function ($a, $b) use ($key) {
        		return strnatcmp($a[$key], $b[$key]);
    		};
		}

		usort($dataList, build_sorter('regDate'));

		rsort($dataList); 

		$resultData["result"]="ok";
		$resultData["data"]=$dataList;
	    echo json_encode($resultData);

	}else{
		$qstr="";
		if($where)$qstr .= "where ".$where." ";
		if($sort)$qstr .= "order by ".$sort." ";
		if($limit)$qstr .= "limit ".$limit;
		$query = @mysql_query("select * from `$table` ".$qstr,DBManager::get()->getMainConnection());
		echo'{"result":"ok","data":[';
		$isFirst = true;
		while($data = @mysql_fetch_array($query,MYSQL_ASSOC)){
			
			if(!$isFirst){
				echo ",";
			}
			$isFirst=false;
			echo json_encode($data);	
		}
	echo"]}";
	}
}


DBManager::get()->closeDB();

?>

