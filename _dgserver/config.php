<?php

$sInfo["gameID"]="drawgirls";
$sInfo["gameNo"]="10289";
$sInfo["HTTPGATEWAY_URL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";
$sInfo["HTTPGATEWAY_HELP_URL"]="http://alpha-httpgw.hangame.com/hsp/httpgw/help.json";
$sInfo["HTTPGATEWAY_VERSION"]="1.3";
$sInfo["HTTPGATEWAY_PORT"]="18080";
$sInfo["secretKey"]="qrqhyrlgprghedvh";
DBManager::setStaticInfo($sInfo);


//서버설정											ip                    id            pw
$server0Index = DBManager::get()->addDBServer("10.99.197.209:13306","drawgirlsdb","litqoo!@#234");

//메인db설정					dbname       server index
DBManager::get()->setMainDB("drawgirls",$server0Index);

//유저db설정                   dbname      server index
DBManager::get()->addShardDB("dg001",$server0Index);
DBManager::get()->addShardDB("dg002",$server0Index);


?>