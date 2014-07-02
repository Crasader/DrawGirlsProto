<?php
include_once("../lib.php");
include_once("../command/cmd2.php");



$obj = new $_POST["dbClass"]();

if($_POST["dbFunc"]){
$result =  $obj->$_POST["dbFunc"](json_decode($_POST["param"],true));
}else{
	$param = json_decode(($_POST["param"]),true);
	
	if($_POST["type"]=="form"){
		if($_POST["dbMode"]=="insert")$result = $obj->insertWithLQForm($param);
		else if($_POST["dbMode"]=="update")$result = $obj->updateWithLQForm($param);
		else if($_POST["dbMode"]=="select")$result = $obj->selectWithLQForm($param);
		else if($_POST["dbMode"]=="delete")$result = $obj->deleteWithLQForm($param);
		else if($_POST["dbMode"]=="load")$result = $obj->loadWithDataForm($param);
		else if($_POST["dbMode"]=="custom")$result = $obj->$_POST["dbFunc"]($param);
	}else{
		if($_POST["dbMode"]=="insert")$result = $obj->insertWithLQTable($param);
		else if($_POST["dbMode"]=="update")$result = $obj->updateWithLQTable($param);
		else if($_POST["dbMode"]=="select")$result = $obj->selectWithLQTable($param);
		else if($_POST["dbMode"]=="delete")$result = $obj->deleteWithLQTable($param);
		else if($_POST["dbMode"]=="load")$result = $obj->loadWithDataTable($param);
		else if($_POST["dbMode"]=="custom")$result = $obj->$_POST["dbFunc"]($param);
	}
}

$result["log"]=LogManager::get()->getLogAndClear();
echo json_encode($result,JSON_UNESCAPED_UNICODE);
?>