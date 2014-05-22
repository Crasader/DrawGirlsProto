<?php
include_once("../lib.php");
include_once("../command/cmd2.php");



$obj = new $_POST["dbClass"]();

if($_POST["dbFunc"]){
$result =  $obj->$_POST["dbFunc"](json_decode($_POST["param"],true));
}else{
	$param = json_decode($_POST["param"],true);
	if($_POST["dbMode"]=="insert")$result = $obj->insertWithLQTable($param);
	else if($_POST["dbMode"]=="update")$result = $obj->updateWithLQTable($param);
	else if($_POST["dbMode"]=="select")$result = $obj->selectWithLQTable($param);
	else if($_POST["dbMode"]=="delete")$result = $obj->deleteWithLQTable($param);
	else if($_POST["dbMode"]=="load")$result = $obj->loadWithDataTable($param);
}

$result["log"]=LogManager::get()->getLogAndClear();
echo json_encode($result,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
?>