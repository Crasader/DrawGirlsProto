<?php

iconv_set_encoding("internal_encoding", "UTF-8");
iconv_set_encoding("output_encoding", "UTF-8");


include "lib.php";


$nowurl = $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"];
$_dirs = explode("/",$_SERVER["REQUEST_URI"]);
unset($_dirs[count($_dirs)-1]);
$_dirs = implode($_dirs,"/");
$nowurl = $nowurl.$_dirs;


$mode = $_GET["mode"];
$allResult="";
$version=1;

function webLog($log){
    global $mode;
    if($mode){
        echo $log;
    }
}

if($mode){

    $param["0"]=array();
    $param["0"]["p"]=json_decode(stripcslashes($_GET[p]),true);
    $param["0"]["a"]=$_GET[a];
    $version = $_GET["version"];

}else{
   // $param = base64_decode(str_replace(" ","+",$_POST["command"]));
    
    $param = decryptByAESWithBase64($_POST["command"]); //@mcrypt_decrypt(MCRYPT_DES, "12345678", $param, MCRYPT_MODE_ECB);
    
    $param = json_decode(trim($param),true);
    $version = $_POST["version"];
    
}


if($version)include "command/cmd".$version.".php";
else include "command/cmd2.php";


if(!$stopCommand){
    $command = new commandClass();
    $isTranjaction = false;    
    $commitMemberID=0;
    $commitCmdName="";
    $checkUserdata = false;
    for($c=0;$c<count($param);$c++){
        $cmd = (string)$c;
        if(!$param[$cmd])continue;
        $p = $param[$cmd]["p"];
        $a = strtolower($param[$cmd]["a"]);
        
        if($a=="starttranjaction"){
            $commitMemberID=$p["memberID"];
            CommitManager::get()->begin($commitMemberID);
            $commitCmdName = $cmd;
        }else if(method_exists($command,$a)){
            if(!($a=="login" || $a=="join") && $p["memberID"] && $checkUserdata==false){
                $userdata = new UserData($p["memberID"]);
                LogManager::get()->addLog("action is ".$a." deviceID ".$userdata->deviceID." and cmdNo".$userdata->lastCmdNo." userdata is".json_encode($userdata->getArrayData(true)));
                
                if($userdata->isLoaded()){
                    if($userdata->deviceID!=$param["deviceID"]){
                            $checkUserdata=true;
                            $allResult["checkDeviceError"]=true;
                            $r["result"]=ResultState::toArray(4002);
                            $allResult[$cmd]= $r;
                            //continue;
                    }else if($userdata->lastCmdNo>=$param["cmdNo"]){
                        $allResult["cmdNoError"]=true;
                        $checkUserdata=true;
                        $r["result"]=ResultState::toArray(4001);
                        $allResult[$cmd]= $r;
                        //continue;

                    }else if((TimeManager::get()->getTime()-$userdata->lastTime)>60*30){
                        $checkUserdata=true;
                        $allResult["longTimeError"]=true;
                        $r["result"]=ResultState::toArray(4003);
                        $allResult[$cmd]= $r;
                        //continue;
                    }else{
                        $userdata->lastCmdNo=$param["cmdNo"];
                        if($userdata->save()){
                            $checkUserdata=true;
                        }else{
                            $userdata->save();
                            $checkUserdata=true;
                        }
                    }
                }



            }

            $startTime = TimeManager::get()->getMicroTime();
            $r = $command->$a($p);
            $endTime = TimeManager::get()->getMicroTime();
            
            if($a=="login" || $a=="join"){
                LogManager::get()->addLog("ok out deviceID ".$r["result"]["code"]);
                if(ResultState::successCheck($r["result"])){
                    $checkUserdata=true;
                    $allResult["lastCmdNo"]=0;
                    $allResult["deviceID"]=$r["data"]["deviceID"];
                    LogManager::get()->addLog("out deviceID".$r["data"]["deviceID"]);
                }else{
                    $allResult["deviceID"]=0;
                }
            }

            if($commitMemberID && !ResultState::successCheck($r["result"])){
                CommitManager::get()->setSuccess($commitMemberID,false);
            }

            
            $p2 = array();
            $p2["memberID"]= $param["memberID"];
            if(!$p2["memberID"])$p2["memberID"]= $param["hspMemberNo"];
            if(!$p2["memberID"])$p2["memberID"]= $param["memberNo"];
        

            $r["log"] = LogManager::get()->getLog();

            $p2["category"]=$a;
            $p2["content"]=json_encode($p,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
            $p2["output"]=$r;
            $p2["execTime"]=$endTime-$startTime;
            if($a!="writelog")$command->writelog($p2);
            LogManager::get()->getLogAndClear();
            $allResult[$cmd]= $r;
        }else if($a=="help"){
            $class_methods = get_class_methods('commandClass');
            foreach ($class_methods as $method_name) {
                echo "$method_name <br>";
            }
            mysql_close();
            exit;
        }else{
            $p["api"]=$param[$cmd]["a"];
            $allResult[$cmd]= $command->httpgateway($p);
            
            
            $p2=array();
            $p2["memberID"]= $param["memberID"];  
            if(!$p2["memberID"])$p2["memberID"]= $param["hspMemberNo"];
            if(!$p2["memberID"])$p2["memberID"]= $param["memberNo"];
                    
            $p2["category"]=$p["api"];
            $p2["content"]=json_encode($p,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
            $p2["output"]=$allResult[$cmd];
            $command->writelog($p2);
            //$allResult[$cmd]=array("state"=>"error","msg"=>"don't find command");
        }
        
        
    }

    if($commitMemberID>0){
        $commitsuccess=CommitManager::get()->commit($commitMemberID);
        if($commitsuccess){
            $cr["result"] = ResultState::successToArray();
            $cr["list"] = $allResult;
            $allResult[$commitCmdName]=$cr;
        }else{
            $cr["result"] = ResultState::toArray(3001,"tranjaction fail");
            $allResult[$commitCmdName]=$cr;
        }

         for($c=0;$c<count($param);$c++){
            $cmd = (string)$c;
            $allResult[$cmd]["tranjaction"]=$commitsuccess;
        }
    }

    $allResult["state"]="ok";
    $allResult["timestamp"]=TimeManager::get()->getTime();
    $allResult["date"]=TimeManager::get()->getCurrentDateString();
    $allResult["weekNo"]=TimeManager::get()->getCurrentWeekNo();
    
    $allResult = json_encode($allResult,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
    
    if($mode=="nodes"){
        echo $allResult;
    }else{
        //echo $allResult;
         echo encryptByAESWithBase64($allResult)."#";
       // echo base64_encode(@mcrypt_encrypt(MCRYPT_DES, $SECRETKEY,$allResult, MCRYPT_MODE_ECB))."#";        
    }
    
    
    DBManager::get()->closeDB();
    @mysql_close();

}

?>