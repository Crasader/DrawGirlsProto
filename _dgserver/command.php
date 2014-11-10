<?php
ob_start();


include "lib.php";
#iconv_set_encoding("internal_encoding", "UTF-8");
#iconv_set_encoding("output_encoding", "UTF-8");

//echo "2dhqLCia6H379CQaBAPFAFU7YhDpsAay7JQM6jiDSY0=#";
//sleep(1);
$nowurl = $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"];
$_dirs = explode("/",$_SERVER["REQUEST_URI"]);
unset($_dirs[count($_dirs)-1]);
$_dirs = implode($_dirs,"/");
$nowurl = $nowurl.$_dirs;


$mode = $_GET["mode"];
$allResult=array();
$allResult["cmdNoError"]=false;
$allResult["checkDeviceError"]=false;
$allResult["longTimeError"]=false;
$allResult["restart"]=false;

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
    
    $paramoriginal = decryptByAESWithBase64($_POST["command"]); //@mcrypt_decrypt(MCRYPT_DES, "12345678", $param, MCRYPT_MODE_ECB);
    //LogManager::addLog("original:".$_POST["command"]);
    $param = json_decode(trim($paramoriginal),true);
    CurrentUserInfo::$os = $param["os"];
    CurrentUserInfo::$language = "ko"; //$param["lang"];
    CurrentUserInfo::$memberID = $param["memberID"];
    CurrentUserInfo::$socialID = $param["socialID"];
    CurrentUserInfo::$country = "kr";//$param["country"];
    if($param["timezone"])CurrentUserInfo::$timezone = $param["timezone"];
    if($param["log"])LogManager::$m_isLocked=false;
    // if(!is_array($param)){
    //     $param = json_decode(trim(preg_replace("/'/","\"", $paramoriginal)),true);
    //     //LogManager::addLog("ok --> ".$paramoriginal);
    //     //LogManager::addLog("ok --> ".preg_replace("/'/","\"", $paramoriginal));
    // }
    $version = $_POST["version"];
    
}


if($version)include "command/cmd".$version.".php";
else include "command/cmd2.php";

$checklorj=false;
$error="";
//보안체크용
if(CurrentUserInfo::$memberID){
    //LogManager::addLog("start checking user");
    $userData = UserData::create(CurrentUserInfo::$memberID);

    if($userData->isLoaded()){
        //ERROR
        $error = false;
        if($userData->deviceID!=$param["deviceID"]){
            $error = "checkDeviceError";
            LogManager::addLog("server deviceID is ".$userData->deviceID." , client deviceID is".$param["deviceID"]);
        }else if($userData->lastCmdNo>=$param["cmdNo"]){
            $error = "cmdNoError";
            LogManager::addLog("server CmdNo is ".$userData->lastCmdNo." , client cmdNo is".$param["cmdNo"]);
        }else if((TimeManager::getTime()-$userData->lastTime)>60*60*24){
            $error = "longTimeError";
        }else{
            //save deviceID, save checkDeviceError, save longtimeerror
            $userData->lastCmdNo=$param["cmdNo"];
            $cnt=0;
            while(!$userData->save()){
                $cnt++; 
                if($cnt>5){
                    $error = "longTimeError";
                    break;
                }
            }
        }


    }else{
        //LogManager::addLog("userLogFailed : dont find.uu ".CurrentUserInfo::$memberID);
        $error="restart";
         LogManager::addLog("filed to save user data");
    }
}else{
    //LogManager::addLog("userLogFailed : id is none uu");
    $error="restart";
    LogManager::addLog("memberID dont find");
}

if($param["dontcheck"]){
    $error="";
    //LogManager::addLog("dontcheck!!");
}

if($error){
   $checklorj = false;
   for($c=0;$c<count($param);$c++){
        $cmd = (string)$c;
        if(!$param[$cmd]){
            $cmd = (string)str_pad($cmd, 3,"0",STR_PAD_LEFT);
            if(!$param[$cmd])continue;
        }
        $p = $param[$cmd]["p"];
        $a = strtolower($param[$cmd]["a"]);
        if($a=="login" || $a=="join"){
            $error = "";
            break;
        }
    }
}

if($error!=""){
    $allResult[$error]=true;
    $allResult["log"]=LogManager::getLogAndClear();

}else{
//if(!$stopCommand){
    //$command = new commandClass();
    $istransaction = false;    
    $commitMemberID=0;
    $commitCmdName="";
    $checkUserdata = false;
    $transactionErrorList=array();
    for($c=0;$c<count($param);$c++){
        $cmd = (string)$c;
        if(!$param[$cmd]){
            $cmd = (string)str_pad($cmd, 3,"0",STR_PAD_LEFT);
            if(!$param[$cmd])continue;
        }
        $p = $param[$cmd]["p"];
        $a = strtolower($param[$cmd]["a"]);
        
        if($a=="starttransaction"){
            $commitMemberID=$p["memberID"];
            CommitManager::begin($commitMemberID);
            $commitCmdName = $cmd;
        }else if(method_exists("commandClass",$a)){
            if(!CurrentUserInfo::$memberID && $p["memberID"]){
                CurrentUserInfo::$memberID = $p["memberID"];
            }


            $startTime = TimeManager::getMicroTime();
            CommitManager::$m_passFunc=false;
            $r = CommandClass::$a($p);
            $endTime = TimeManager::getMicroTime();
            
            if($a=="login" || $a=="join"){
                //LogManager::addLog("ok out deviceID ".$r["result"]["code"]);
                if(ResultState::successCheck($r["result"])){
                    $checkUserdata=true;
                    $allResult["lastCmdNo"]=0;
                    $allResult["deviceID"]=$r["data"]["deviceID"];
                    //LogManager::addLog("out deviceID".$r["data"]["deviceID"]);
                }else{
                    $allResult["deviceID"]=0;
                }
            }

            if(!CommitManager::$m_passFunc && $commitMemberID && !ResultState::successCheck($r["result"])){
                 CommitManager::setSuccess($commitMemberID,false);
                 $transactionErrorList["result"]=$r["result"];
                 $transactionErrorList["name"]=$a;
            }

            
            $p2 = array();
            $p2["memberID"]= CurrentUserInfo::$memberID;
        

            $logs = LogManager::getLog();

            $p2["category"]=$a;
            $p2["content"]=json_encode($p,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
            $p2["output"]=$r;
            $p2["output"]["log"]=$logs;
            $p2["execTime"]=$endTime-$startTime;
            if($a!="writelog" && !ResultState::successCheck($r["result"]) && !CommitManager::$m_passFunc)CommandClass::writelog($p2);
            
            
            $r["log"]=LogManager::getLogAndClear();
            
            $allResult[$cmd]= $r;
            
            CommitManager::$m_passFunc=false;
        }else if($a=="help"){
            $class_methods = get_class_methods('commandClass');
            foreach ($class_methods as $method_name) {
                echo "$method_name <br>";
            }
            mysql_close();
            exit;
        }else{
            $p["api"]=$param[$cmd]["a"];
            $allResult[$cmd]= CommandClass::httpgateway($p);
            
            
            $p2=array();
            $p2["memberID"]= CurrentUserInfo::$memberID;
                    
            $p2["category"]=$p["api"];
            $p2["content"]=json_encode($p,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
            $p2["output"]=$allResult[$cmd];
            CommandClass::writelog($p2);
            //$allResult[$cmd]=array("state"=>"error","msg"=>"don't find command");
        }
        
        
    }

    if($commitMemberID>0){
        $commitsuccess=CommitManager::commit($commitMemberID);
        if($commitsuccess){
            $cr["result"] = ResultState::successToArray();
            $cr["list"] = $allResult;
            $allResult[$commitCmdName]=$cr;
        }else{
            $cr["result"] = ResultState::toArray(ResultState::GDFAILTRANSACTION,"transaction fail");
            $cr["command"]=$transactionErrorList;
            $allResult[$commitCmdName]=$cr;
        }

        for($c=0;$c<count($param);$c++){
            $cmd = (string)$c;
            $cmd = (string)str_pad($cmd, 3,"0",STR_PAD_LEFT);
            if($allResult[$cmd]){
                $allResult[$cmd]["transaction"]=$commitsuccess;
                if(!$commitsuccess && ResultState::successCheck($allResult[$cmd]["result"]))$allResult[$cmd]["result"]=ResultState::toArray(ResultState::GDFAILTRANSACTION);
            }
        }

        $p2["category"]="starttransaction";
        $p2["content"]='{"memberID":"'.CurrentUserInfo::$memberID.'"}';
        $p2["output"]=$allResult[$commitCmdName];
        CommandClass::writelog($p2);
    }

}




$allResult["state"]="ok";
$allResult["timestamp"]=TimeManager::getTime();
$allResult["date"]=TimeManager::getCurrentDateTime();
$allResult["weekNo"]=TimeManager::getCurrentWeekNo();
$allResult["cmdNo"]=$param["cmdNo"];
$allResult = json_encode($allResult,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK | JSON_BIGINT_AS_STRING);

if($mode=="nodes"){
    echo $allResult;
}else{
    //echo $allResult;
     echo encryptByAESWithBase64($allResult)."#";
   // echo base64_encode(@mcrypt_encrypt(MCRYPT_DES, $SECRETKEY,$allResult, MCRYPT_MODE_ECB))."#";        
}


//DBServer::closeAllConnection();
DBServer::closeAllConnection();
@mysql_close();

ob_end_flush();

?>