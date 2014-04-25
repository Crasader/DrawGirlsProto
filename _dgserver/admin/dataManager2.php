<?php
include_once("../lib.php");
include_once("../command/cmd2.php");



$obj = new $_POST["dbClass"]();

$result =  $obj->$_POST["dbFunc"](json_decode($_POST["param"],true));
$result["log"]=LogManager::get()->getLogAndClear();
echo json_encode($result,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);




?>