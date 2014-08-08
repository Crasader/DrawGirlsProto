<?php
include_once("../lib.php");
include_once("../command/cmd2.php");



$klass = $_POST["dbClass"];
if($_POST["dbFunc"]){
$result =  $klass::$_POST["dbFunc"](json_decode($_POST["param"],true));
}else{
	$param = json_decode(($_POST["param"]),true);
	
	if($_POST["type"]=="form"){
		if($_POST["dbMode"]=="insert")$result = $klass::insertWithLQForm($param);
		else if($_POST["dbMode"]=="update")$result = $klass::updateWithLQForm($param);
		else if($_POST["dbMode"]=="select")$result = $klass::selectWithLQForm($param);
		else if($_POST["dbMode"]=="delete")$result = $klass::deleteWithLQForm($param);
		else if($_POST["dbMode"]=="load")$result = $klass::loadWithDataForm($param);
		else if($_POST["dbMode"]=="custom")$result = $klass::$_POST["dbFunc"]($param);
	}else{
		if($_POST["dbMode"]=="insert")$result = $klass::insertWithLQTable($param);
		else if($_POST["dbMode"]=="update")$result = $klass::updateWithLQTable($param);
		else if($_POST["dbMode"]=="select")$result = $klass::selectWithLQTable($param);
		else if($_POST["dbMode"]=="delete")$result = $klass::deleteWithLQTable($param);
		else if($_POST["dbMode"]=="load")$result = $klass::loadWithLQTable($param);
		else if($_POST["dbMode"]=="custom")$result = $klass::$_POST["dbFunc"]($param);
	}
}

$result["log"]=LogManager::getLogAndClear();
echo json_encode($result,JSON_UNESCAPED_UNICODE);
?>