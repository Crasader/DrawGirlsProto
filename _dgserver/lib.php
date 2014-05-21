<?php


error_reporting( E_ALL ^ E_NOTICE );
ini_set( 'display_errors',  E_ALL ^ E_NOTICE );

srand((float)microtime() * 10000000);

include_once("DBManager.php");
include_once("DBManagerForManage.php");

$GAMEID = $_POST["gid"];
if(!$GAMEID)$GAMEID = $_GET["gid"];

$gid = $GAMEID;

include_once("config.php");

$AES_KEY = "qrqhyrlgprghedvh";

//DBManager::get()->setDataBase($GAMEID);

if($_SERVER["SERVER_NAME"]=="182.162.201.147"){
	$LQset["db_host"]="10.99.197.209:13306";
	$LQset["db_id"]="drawgirlsdb";
	$LQset["db_name"]="drawgirls";
	$LQset["db_pass"]="litqoo!@#234";
}else{
	$LQset["db_host"]="localhost";
	$LQset["db_id"]="hsdooki";
	$LQset["db_name"]="hsdooki";
	$LQset["db_pass"]="8706";
}



$TABLE_COMMONSETTING = "aCommonSetting";
$TABLE_NOTICE = "aNoticeTable";
$TABLE_CARD = "aCardTable";
$TABLE_ITEM = "aItemTable";
$TABLE_MISSION = "aMissionTable";
$TABLE_MONSTER = "aMonsterTable";
$TABLE_PATTERN = "aPatternTable";
$TABLE_THEME = "aThemeTable";
$TABLE_LANGUAGE = "aLanguageTable";
$TABLE_STAGE = "aStageTable";
$TABLE_PIECE = "aPieceTable";
$TABLE_IMAGE = "aImageTable";
$TABLE_CHECKPOINT = "aCheckpointTable";
$TABLE_MONSTER_MOVEMENT = "aMonsterMovementTable";
$TABLE_EVENTSTAGE = "aEventStageTable";
$TABLE_PUZZLE = "aPuzzleTable";
$TABLE_BOOK = "aBookTable";
$TABLE_CHARACTER = "aCharacterTable";
$TABLE_KEYINTVALUE = "aKeyIntValue";
$TABLE_KEYSTRINGVALUE = "aKeyStringValue";
$TABLE_SHOP = "aShopTable";
$TABLE_WORKLIST = "aWorkList";
$TABLE_FORMSETTER = "aFormSetter";
$TABLE_USERDATA = "UserDataTable";
$TABLE_LOG = "logTempTable";
$TABLE_MESSAGE = "messageTable";

$HSP_VERSION="1.3";
$HSP_GAMENO="10226";
$HSP_PORT="18080";
$HSP_GATEWAY_URL="http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";
$HSP_GATEWAY_HELP_URL="http://alpha-httpgw.hangame.com/hsp/httpgw/help.json";

$SECRETKEY = "12345678";

$ERRORCODE["puzzle_dontfind"]=array("code"=>15000,"msg"=>"dont find puzzle");





//db connect!
/*
if(!$connect){
$connect=@mysql_connect($LQset[db_host],$LQset[db_id],$LQset[db_pass]);
@mysql_select_db($LQset[db_name], $connect);
}
*/


function decryptByAESWithBase64($text,$key=null){

	$st = mcrypt_decrypt(
		MCRYPT_RIJNDAEL_128, 
		'123456789abcdef', 
		$tmp_st, 
		MCRYPT_MODE_ECB, 
		mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_ECB), MCRYPT_RAND)
		);


	global $AES_KEY;
	if(!$key)$key=$AES_KEY;
    $text = base64_decode(str_replace(" ","+",$text));
    // $iv_size = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_ECB);
    // $iv_dec = substr($text, 0, $iv_size);
    # retrieves the cipher text (everything except the $iv_size in the front)
  	 // $text = substr($text, $iv_size);
    # may remove 00h valued characters from end of plain text
    $plaintext_dec = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $text, MCRYPT_MODE_ECB, mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_ECB), MCRYPT_RAND));
    return  $plaintext_dec;
}

function encryptByAESWithBase64($text,$key=null){
	global $AES_KEY;
	if(!$key)$key=$AES_KEY;

    //$key_size =  strlen($key);
    //$iv_size = mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_ECB);
    $iv = mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_ECB), MCRYPT_RAND); //mcrypt_create_iv($iv_size, MCRYPT_RAND);
    $ciphertext = mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key,
                                 $text, MCRYPT_MODE_ECB, $iv);
    //$ciphertext = $iv . $ciphertext;
    return base64_encode($ciphertext);
}

function get_client_ip() { 
global $_SERVER;
if (!empty($_SERVER['HTTP_CLIENT_IP'])) { 
// check ip from share internet 
$ip = $_SERVER['HTTP_CLIENT_IP']; 
} elseif (!empty($_SERVER['HTTP_X_FORWARDED_FOR'])) { 
// to check ip is pass from proxy 
$ip = $_SERVER['HTTP_X_FORWARDED_FOR']; 
} else { 
$ip = $_SERVER['REMOTE_ADDR']; 
} 
return $ip; 
}

function LQgo($url){
echo "<meta http-equiv=\"refresh\" content=\"0;url=$url\">";

}

function hmac ($key, $data)
{
    // RFC 2104 HMAC implementation for php.
    // Creates an md5 HMAC.
    // Eliminates the need to install mhash to compute a HMAC
    // Hacked by Lance Rushing

    $b = 64; // byte length for md5
    if (strlen($key) > $b) {
        $key = pack("H*",md5($key));
    }
    $key  = str_pad($key, $b, chr(0x00));
    $ipad = str_pad('', $b, chr(0x36));
    $opad = str_pad('', $b, chr(0x5c));
    $k_ipad = $key ^ $ipad ;
    $k_opad = $key ^ $opad;

    return md5($k_opad  . pack("H*",md5($k_ipad . $data)));
}

function img_upload($files,$no,$updir){
		global $_FILES;
		$file1 = $_FILES[$files][tmp_name];
		$file1_name = $_FILES[$files][name];
		$file1_size = $_FILES[$files][size];
		$file1_type = $_FILES[$files][type];

			if(!is_dir($updir)) { 
				mkdir($updir,0777);
				chmod($updir,0777);
			}
		if($file1){
			$date=date("his",time());
			$newname=trim($file1_name);
			$nnn = explode(" ",$newname);
			$hjj = explode(".",$newname);
			$hjj = $hjj[count($hjj)-1];
			
			for($i=0;$i<=count($nnn);$i++){
				$newname2.=$nnn[$i];
			}
			$newname=trim($newname2);
			$newname=hmac('ripemd160', "lq".$date.$newname).".".$hjj;

			if (move_uploaded_file($file1,$updir."/".$newname))
				return $newname;
			else
				return false;
		}
		return false;
			
	}
	
	
	
	function img_multi_upload($files,$updir){
		global $_FILES;
		$filenames = array();
		for($i=0;$i<count($_FILES[$files][name]);$i++){
			
					
			$file1 = $_FILES[$files][tmp_name][$i];
			$file1_name = $_FILES[$files][name][$i];
			$file1_size = $_FILES[$files][size][$i];
			$file1_type = $_FILES[$files][type][$i];
	
				if(!is_dir($updir)) { 
					mkdir($updir,0777);
					chmod($updir,0777);
				}
				
			if($file1){
				$date=date("his",time());
				$newname=trim($file1_name);
				$nnn = explode(" ",$newname);
				$hjj = explode(".",$newname);
				$hjj = $hjj[count($hjj)-1];
				
				for($i=0;$i<=count($nnn);$i++){
					$newname2.=$nnn[$i];
				}
				$newname=trim($newname2);
				$newname=hmac('ripemd160', "lq".$date.$newname).".".$hjj;
	
				if (move_uploaded_file($file1,$updir."/".$newname))
					$filenames[]=$newname;
				else
					return false;
			}
			
		
		}
		return false;
			
	}
	
	


if(!function_exists("json_encode")){ 
    function json_encode($a=false,$isTemp=false){ 
        if(is_null($a)) return 'null'; 
        if($a === false) return 'false'; 
        if($a === true) return 'true'; 
        if(is_scalar($a)){ 
            if(is_float($a)) return floatval(str_replace(",", ".", strval($a))); 
            if(is_int($a)) return $a;
            if(is_string($a)){ 
                static $jsonReplaces = array(array("\\", "/", "\n", "\t", "\r", "\b", "\f", '"'), array('\\\\', '\\/', '\\n', '\\t', '\\r', '\\b', '\\f', '\"')); 
                return '"' . str_replace($jsonReplaces[0], $jsonReplaces[1], $a) . '"'; 
            } else return $a; 
        } 
        $isList = true; 
        for($i=0, reset($a); $i<count($a); $i++, next($a)){ 
            if(key($a) !== $i){ 
                $isList = false; 
                break; 
            } 
        } 
        $result = array(); 
        if($isList){ 
            foreach($a as $v) $result[] = json_encode($v); 
            return '[' . join(',', $result) . ']'; 
        } else{ 
            foreach($a as $k => $v){
              if(is_int($k))$k='"'.$k.'"';
              else $k = json_encode($k);
	          $result[] = $k.':'.json_encode($v);  
            }  
            return '{' . join(',', $result) . '}'; 
        } 
    } 
}


if ( !function_exists('json_decode') ){ 
function json_decode($json,$isTemp=false) 
{  
	$json=stripslashes($json);
    // Author: walidator.info 2009 
    $comment = false; 
    $out = '$x='; 
    
    for ($i=0; $i<strlen($json); $i++) 
    { 
        if (!$comment) 
        { 
            if ($json[$i] == '{')        $out .= ' array('; 
            else if ($json[$i] == '}')    $out .= ')'; 
            else if ($json[$i] == '[')    $out .= 'array('; 
            else if ($json[$i] == ']')    $out .= ')'; 
            else if ($json[$i] == ':')    $out .= '=>'; 
            else                         $out .= $json[$i];            
        } 
        else $out .= $json[$i]; 
        if ($json[$i] == '"')    $comment = !$comment; 
    } 
    @eval($out . ';'); 
    return $x; 
}  
} 



function lq_query_insert($data,$table){ 

	$key=array_keys ($data);  
	$query="insert into `$table` ("; 
		for($i=0;$i<count($key);$i++){ 
			$query.="`".$key[$i]."`"; 
			if($i!=count($key)-1) $query.=","; 
		} 

	$query.=") values ("; 
	for($i=0;$i<count($key);$i++){ 
		if(is_array($data[$key[$i]]))$data[$key[$i]] = json_encode($data[$key[$i]]);
		$query.="'".addslashes($data[$key[$i]])."'"; 
		if($i!=count($key)-1) $query.=","; 
	} 
	$query.=")"; 
	return $query; 
} 

function lq_query_update($data,$table,$where){
$key=array_keys($data); 
$query="update $table set ";
for($i=0;$i<count($key);$i++){
if(is_array($data[$key[$i]]))$data[$key[$i]] = json_encode($data[$key[$i]]);
$query.="`$key[$i]`=\"".addslashes($data[$key[$i]])."\"";
if($i!=count($key)-1) $query.=",";
}
$query.=" ".$where;

return $query;
}


function get_full_url() {
    $https = !empty($_SERVER['HTTPS']) && strcasecmp($_SERVER['HTTPS'], 'on') === 0;
    return
        ($https ? 'https://' : 'http://').
        (!empty($_SERVER['REMOTE_USER']) ? $_SERVER['REMOTE_USER'].'@' : '').
        (isset($_SERVER['HTTP_HOST']) ? $_SERVER['HTTP_HOST'] : ($_SERVER['SERVER_NAME'].
        ($https && $_SERVER['SERVER_PORT'] === 443 ||
        $_SERVER['SERVER_PORT'] === 80 ? '' : ':'.$_SERVER['SERVER_PORT']))).
        substr($_SERVER['SCRIPT_NAME'],0, strrpos($_SERVER['SCRIPT_NAME'], '/'));
}

function get_server_var($id) {
        return isset($_SERVER[$id]) ? $_SERVER[$id] : '';
    }    


function filesInDir ($tdir) 
{ 

	
		
/*
		exec("ls $realDir -t",$out);
		
		foreach($out as $s){
			echo $s."<br>";
		}
*/

	if(@opendir($tdir)) { 
		
		$files = Array(); 
		$in_files = Array(); 
	
		@exec("ls $tdir -t",$out);
		
		if(is_array($out) && count($out)>0){
		foreach($out as $a_file) { 
			if($a_file[0] != '.') { 
				if(@is_dir ($tdir . "/" . $a_file)) { 
					$in_files = filesInDir ($tdir . "/" . $a_file); 
					if(is_array ($in_files)) $files = array_merge ($files , $in_files); 
				} else { 
					array_push ($files , $tdir . "/" . $a_file); 
				} 
			} 
		} 
		
		//closedir ($dh); 
		return $files ; 
		}
	} 
} 

class kvManager{
	public static function get($key,$default){
		if(is_string($default)){
			$r = kvManager::getString($key);
			if($r===false)return $default;
			return $r;
		}else{
			$r = kvManager::getInt($key);
			if($r===false)return $default;
			return $r;
		}
	}

	public static function getString($key){
		$query = mysql_query("select * from ".DBManager::get()->getMT("keystringvalue")." where `key`='$key'",DBManager::get()->getMainConnection());
		if($query)$r = mysql_fetch_array($query);
		if($r)return $r["value"];
		else return false;	
	}
	
	public static function getInt($key){
		
		$query = mysql_query("select * from ".DBManager::get()->getMT("keyintvalue")." where `key`='$key'",DBManager::get()->getMainConnection());
		
		
		if($query)$r = mysql_fetch_array($query);
		if($r)return $r["value"];
		else return false;
	}
	
	public static function increase($key){
	
		
		mysql_query("UPDATE ".DBManager::get()->getMT("keyintvalue")." SET `value`=value+1 WHERE `key`='$key'",DBManager::get()->getMainConnection());
		$arows = mysql_affected_rows();
		
		if($arows<=0){
			if(mysql_query("INSERT INTO ".DBManager::get()->getMT("keyintvalue")." (`key`, `value`) VALUES ('$key', '1')",DBManager::get()->getMainConnection())){
				return true;
			}else{
				return false;
			}
		}
		
		return true;
	}

	public static function set($key,$value){
		//mysql_affected_rows()
		
		$table = DBManager::get()->getMT("keyintvalue");
		if(is_string($value))$table = DBManager::get()->getMT("keystringvalue");
		
		mysql_query("UPDATE $table SET `value`='$value' WHERE `key`='$key'",DBManager::get()->getMainConnection());
		if(mysql_affected_rows()<=0){
			mysql_query("INSERT INTO $table (`key`, `value`) VALUES ('$key', '$value')",DBManager::get()->getMainConnection());
		}
	}
}





function getCardMissile($boss,$stageLevel){
	$alpabat = array("A","B","C","D","E","F","G","H","I","J","K","L","m","n","o");
	//뒷자리가 EFG면 레이저
	$arand = $alpabat[(int)rand(0,8)].$alpabat[(int)rand(0,3)];
	$prand = $boss[hp]/(float)20;
	$drand = $boss[agi];

	$speedmax = $stageLevel*3;
	if($speedmax>70)$speedmax=70;
	$srand=rand(20,$speedmax+20)/(float)10.0;

	$cardStat=array();
	for($grade=1;$grade<=3;$grade++){
				//카드 단계별 속성능력 up
		$prg=0.5;
		$drg=0.5;
		if($grade==3){
			$prg = 1.0;
			$drg = 1.0;
		}else if($grade==2){
			if(rand(0,1)){
				$prg = 0.8;
			}else{
				$drg = 0.8;
			}
		}

		
		$ms["type"]=$arand;
		$ms["power"]=(int)($prg*$prand);		
		$ms["dex"]=(int)($drg*$drand);		
		$ms["speed"]=$srand*0.5;
		
		if($ms["power"]<1)$ms["power"]=1;
		if($ms["dex"]<1)$ms["dex"]=1;
		if($ms["speed"]<0.5)$ms["speed"]=0.5;
		
		$cardStat[$grade-1]=$ms;
	}

	return $cardStat;
}



function reloadPuzzleInfo(){


	mysql_query("update ".DBManager::get()->getMT("piece")." set cards='',puzzle=0,pieceNo=0",DBManager::get()->getMainConnection());
	mysql_query("update ".DBManager::get()->getMT("card")." set piece=0",DBManager::get()->getMainConnection());


	//모두세팅!!!!! 일반

	//mysql_query("update aCardTable set ability='[]'",DBManager::get()->getMainConnection());


	$query = mysql_query("select * from ".DBManager::get()->getMT("piece")." where no<10000 order by no asc",DBManager::get()->getMainConnection());
	LogManager::get()->addLog(mysql_error());
	$puzzleOrder=1;
	$puzzleCount=0;
	$puzzleData = null;
	$puzzlePathInfo = null;
	$puzzleConditionInfo = array();
	$puzzleRewardInfo = array();
	while($stageData = mysql_fetch_array($query)){
		
		if($puzzleCount==0){
			$qq = "select * from ".DBManager::get()->getMT("puzzle")." where ".DBManager::get()->getMT("puzzle").".order=$puzzleOrder";
			$puzzleData = mysql_fetch_assoc(mysql_query($qq,DBManager::get()->getMainConnection()));
			$puzzlePathInfo = json_decode($puzzleData[pathInfo],true);
			$puzzleCardInfo = json_decode($puzzleData[cardInfo],true);
			LogManager::get()->addLog("Q--->".$qq);
			LogManager::get()->addLog("R--->".mysql_error());
			// echo "--->".$qq."<br>";
			// echo"puzzle...<br>";
			// var_dump($puzzleData);
			// echo"puzzle...<br>";

			// var_dump($puzzlePathInfo);
			// echo"puzzle...<br>";

			// var_dump($puzzleCardInfo);
			// echo"puzzle...<br>";
			// echo "<b><font color=green>".mysql_error()."</font></b>";
			//퍼즐정보업뎃
			//mysql_query("update aPuzzleTable set stageStart=".$stageData[no]." where no=".$puzzleData[no],DBManager::get()->getMainConnection());

			if(!$puzzleData){
				LogManager::get()->addLog("stop~~~~!!!!");
				//echo "stop~~!!!!!!!!!!!!!!";
				break;
			}
		}

		$stageReward = json_decode($stageData[rewardInfo],true);
		$cards = $puzzleCardInfo[$puzzleCount];
		$pieceNo = $puzzlePathInfo[$puzzleCount];
		$puzzle = $puzzleData["no"];

		$puzzleRewardInfo[] = $stageReward;
		$cTxt = json_encode($cards,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		//스테이지 카드세팅

		$cmodify='{"no":0}';
		if($puzzleCount!=0)$cmodify = '{"no":'.($stageData[no]-1).'}';
		
		$originCondition = json_decode($stageData["condition"],true);
		if($originCondition["gold"])$cmodify = $stageData["condition"];
		
		$puzzleConditionInfo[] = json_decode($cmodify,true);
		
		$q = "update ".DBManager::get()->getMT("piece")." set cards='$cTxt', pieceNo=$pieceNo, puzzle=$puzzle,`condition`='".$cmodify."',`version`=`version`+1 where no=".$stageData[no];
		//echo $q ."<br>";
		mysql_query($q ,DBManager::get()->getMainConnection());

			LogManager::get()->addLog("Q--->".$q );
			LogManager::get()->addLog("--->".mysql_error());
			//echo "<b><font color=green>".mysql_error()."</font></b>";

		//카드능력치 셋팅 by 스테이지몬스터정보
		$boss = json_decode($stageData[boss],true);
		$cardStat = getCardMissile($boss[0],$stageData[level]);
		for($ci=0;$ci<count($cards);$ci++){
			$reward = $stageReward[$ci];
			$sString = json_encode($cardStat[$ci],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$qs1 = "update ".DBManager::get()->getMT("card")." set missile='".$sString."',reward=$reward,piece=".$stageData[no]." where no=".$cards[$ci]; //
			mysql_query($qs1,DBManager::get()->getMainConnection());
			LogManager::get()->addLog("Q--->".$qs1);
			LogManager::get()->addLog("--->".mysql_error());
			//echo "<br><font color=blue>$qs1</font><br>";
			//echo "<b><font color=green>".mysql_error()."</font></b>";
			//echo json_encode($cardStat,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

		}

		$puzzleCount++;

		if($puzzleCount >= count($puzzlePathInfo)){
			$cString = json_encode($puzzleConditionInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$rString = json_encode($puzzleRewardInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$qs = "update ".DBManager::get()->getMT("puzzle")." set  `version`=`version`+1,conditionInfo='".$cString."',rewardInfo='".$rString."' where no=".$puzzleData[no];
			mysql_query($qs,DBManager::get()->getMainConnection());
			LogManager::get()->addLog("Q--->".$qs);
			LogManager::get()->addLog("--->".mysql_error());

			//echo "<br><font color=red>$qs</font><br>";
			//echo "<b><font color=green>".mysql_error()."</font></b>";

			$puzzleConditionInfo=array();
			$puzzleRewardInfo=array();

			$puzzleCount=0;
			$puzzleOrder++;
		}

		//스테이지 쭉~~ 불러와서 카드정보, 퍼즐번호 업뎃.
		//퍼즐의 스타트스테이지업뎃

		//
	}


	//모두세팅 이벤트 

	//echo "<br>startEventsetting <br>";

	// $query = mysql_query("select * from aStageTable where no>10000 order by no asc",DBManager::get()->getMainConnection());

	// $puzzleOrder=10001;
	// $puzzleCount=0;
	// $puzzleData = null;
	// $puzzlePathInfo = null;
	// $puzzleConditionInfo = array();
	// $puzzleRewardInfo = array();
	// while($stageData = mysql_fetch_array($query)){
		
	// 	if($puzzleCount==0){
	// 		$qq = "select * from aPuzzleTable where aPuzzleTable.order=$puzzleOrder";
	// 		$puzzleData = mysql_fetch_assoc(mysql_query($qq,DBManager::get()->getMainConnection()));
	// 		$puzzlePathInfo = json_decode($puzzleData[pathInfo],true);
	// 		$puzzleCardInfo = json_decode($puzzleData[cardInfo],true);
	// 		//echo "--->".mysql_error();
	// 		//echo "--->".$qq."<br>";
	// 		//echo"puzzle...<br>";
	// 		var_dump($puzzleData);
	// 		//echo"puzzle...<br>";

	// 		var_dump($puzzlePathInfo);
	// 		//echo"puzzle...<br>";

	// 		var_dump($puzzleCardInfo);
	// 		//echo"puzzle...<br>";
	// 		//echo "<b><font color=green>".mysql_error()."</font></b>";

	// 		//퍼즐정보업뎃
	// 		//mysql_query("update aPuzzleTable set stageStart=".$stageData[no]." where no=".$puzzleData[no],DBManager::get()->getMainConnection());

	// 		if(!$puzzleData){
	// 			echo "stop~~!!!!!!!!!!!!!!";
	// 			break;
	// 		}
	// 	}

	// 	$stageReward = json_decode($stageData[rewardInfo],true);
	// 	$cards = $puzzleCardInfo[$puzzleCount];
	// 	$pieceNo = $puzzlePathInfo[$puzzleCount];
	// 	$puzzle = $puzzleData["no"];

	// 	$puzzleRewardInfo[] = $stageReward;
	// 	$cTxt = json_encode($cards,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
	// 	//스테이지 카드세팅

	// 	$cmodify='{"no":0}';
	// 	if($puzzleCount!=0)$cmodify = '{"no":'.($stageData[no]-1).'}';
		
	// 	$puzzleConditionInfo[] = json_decode($cmodify,true);
		
	// 	$q = "update aStageTable set cards='$cTxt', pieceNo=$pieceNo, puzzle=$puzzle,`condition`='".$cmodify."',`version`=`version`+1 where no=".$stageData[no];
	// 	//echo $q ."<br>";
	// 	mysql_query($q ,DBManager::get()->getMainConnection());

	// 		//echo "<b><font color=green>".mysql_error()."</font></b>";
	// 	//카드능력치 셋팅 by 스테이지몬스터정보
	// 	$boss = json_decode($stageData[boss],true);
	// 	$cardStat = getCardMissile($boss[0],$stageData[level]);
	// 	for($ci=0;$ci<count($cards);$ci++){
	// 		$reward = $stageReward[$ci];
	// 		$sString = json_encode($cardStat[$ci],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	// 		$qs1 = "update aCardTable set missile='".$sString."',reward=$reward,stage=".$stageData[no]." where no=".$cards[$ci];
	// 		mysql_query($qs1,DBManager::get()->getMainConnection());
	// 		//echo "<br><font color=blue>$qs1</font><br>";
	// 		//echo "<b><font color=green>".mysql_error()."</font></b>";
	// 		//echo json_encode($cardStat,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

	// 	}

	// 	$puzzleCount++;

	// 	if($puzzleCount >= count($puzzlePathInfo)){
	// 		$cString = json_encode($puzzleConditionInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	// 		$rString = json_encode($puzzleRewardInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	// 		$qs = "update aPuzzleTable set  `version`=`version`+1,conditionInfo='".$cString."',rewardInfo='".$rString."' where no=".$puzzleData[no];
	// 		mysql_query($qs,DBManager::get()->getMainConnection());

	// 		//echo "<br><font color=red>$qs</font><br>";
	// 		//echo "<b><font color=green>".mysql_error()."</font></b>";

	// 		$puzzleConditionInfo=array();
	// 		$puzzleRewardInfo=array();

	// 		$puzzleCount=0;
	// 		$puzzleOrder++;
	// 	}

	// 	//스테이지 쭉~~ 불러와서 카드정보, 퍼즐번호 업뎃.
	// 	//퍼즐의 스타트스테이지업뎃

	// 	//
	// }




	kvManager::increase("eventPuzzleListVer");
	kvManager::increase("puzzleListVer");
}



function reloadBookInfo(){


	mysql_query("update aStageTable set cards='',puzzle=0,pieceNo=0",DBManager::get()->getMainConnection());
	mysql_query("update aCardTable set stage=0",DBManager::get()->getMainConnection());


	//모두세팅!!!!! 일반

	//mysql_query("update aCardTable set ability='[]'",DBManager::get()->getMainConnection());


	$query = mysql_query("select * from aStageTable where no<10000 order by no asc",DBManager::get()->getMainConnection());
	$puzzleOrder=1;
	$puzzleCount=0;
	$puzzleData = null;
	$puzzlePathInfo = null;
	$puzzleConditionInfo = array();
	$puzzleRewardInfo = array();
	while($stageData = mysql_fetch_array($query)){
		
		if($puzzleCount==0){
			$qq = "select * from aPuzzleTable where aPuzzleTable.order=$puzzleOrder";
			$puzzleData = mysql_fetch_assoc(mysql_query($qq,DBManager::get()->getMainConnection()));
			$puzzlePathInfo = json_decode($puzzleData[pathInfo],true);
			$puzzleCardInfo = json_decode($puzzleData[cardInfo],true);
			// echo "--->".mysql_error();
			// echo "--->".$qq."<br>";
			// echo"puzzle...<br>";
			// var_dump($puzzleData);
			// echo"puzzle...<br>";

			// var_dump($puzzlePathInfo);
			// echo"puzzle...<br>";

			// var_dump($puzzleCardInfo);
			// echo"puzzle...<br>";
			// echo "<b><font color=green>".mysql_error()."</font></b>";
			//퍼즐정보업뎃
			//mysql_query("update aPuzzleTable set stageStart=".$stageData[no]." where no=".$puzzleData[no],DBManager::get()->getMainConnection());

			if(!$puzzleData){
				//echo "stop~~!!!!!!!!!!!!!!";
				break;
			}
		}

		$stageReward = json_decode($stageData[rewardInfo],true);
		$cards = $puzzleCardInfo[$puzzleCount];
		$pieceNo = $puzzlePathInfo[$puzzleCount];
		$puzzle = $puzzleData["no"];

		$puzzleRewardInfo[] = $stageReward;
		$cTxt = json_encode($cards,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		//스테이지 카드세팅

		$cmodify='{"no":0}';
		if($puzzleCount!=0)$cmodify = '{"no":'.($stageData[no]-1).'}';
		
		$puzzleConditionInfo[] = json_decode($cmodify,true);
		
		$q = "update aStageTable set cards='$cTxt', pieceNo=$pieceNo, puzzle=$puzzle,`condition`='".$cmodify."',`version`=`version`+1 where no=".$stageData[no];
		//echo $q ."<br>";
		mysql_query($q ,DBManager::get()->getMainConnection());

			//echo "<b><font color=green>".mysql_error()."</font></b>";

		//카드능력치 셋팅 by 스테이지몬스터정보
		$boss = json_decode($stageData[boss],true);
		$cardStat = getCardMissile($boss[0],$stageData[level]);
		for($ci=0;$ci<count($cards);$ci++){
			$reward = $stageReward[$ci];
			$sString = json_encode($cardStat[$ci],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$qs1 = "update aCardTable set missile='".$sString."',reward=$reward,stage=".$stageData[no]." where no=".$cards[$ci];
			mysql_query($qs1,DBManager::get()->getMainConnection());
			//echo "<br><font color=blue>$qs1</font><br>";
			//echo "<b><font color=green>".mysql_error()."</font></b>";
			//echo json_encode($cardStat,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

		}

		$puzzleCount++;

		if($puzzleCount >= count($puzzlePathInfo)){
			$cString = json_encode($puzzleConditionInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$rString = json_encode($puzzleRewardInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$qs = "update aPuzzleTable set  `version`=`version`+1,conditionInfo='".$cString."',rewardInfo='".$rString."' where no=".$puzzleData[no];
			mysql_query($qs,DBManager::get()->getMainConnection());

			//echo "<br><font color=red>$qs</font><br>";
			//echo "<b><font color=green>".mysql_error()."</font></b>";

			$puzzleConditionInfo=array();
			$puzzleRewardInfo=array();

			$puzzleCount=0;
			$puzzleOrder++;
		}

		//스테이지 쭉~~ 불러와서 카드정보, 퍼즐번호 업뎃.
		//퍼즐의 스타트스테이지업뎃

		//
	}


	//모두세팅 이벤트 

	//echo "<br>startEventsetting <br>";

	$query = mysql_query("select * from aStageTable where no>10000 order by no asc",DBManager::get()->getMainConnection());

	$puzzleOrder=10001;
	$puzzleCount=0;
	$puzzleData = null;
	$puzzlePathInfo = null;
	$puzzleConditionInfo = array();
	$puzzleRewardInfo = array();
	while($stageData = mysql_fetch_array($query)){
		
		if($puzzleCount==0){
			$qq = "select * from aPuzzleTable where aPuzzleTable.order=$puzzleOrder";
			$puzzleData = mysql_fetch_assoc(mysql_query($qq,DBManager::get()->getMainConnection()));
			$puzzlePathInfo = json_decode($puzzleData[pathInfo],true);
			$puzzleCardInfo = json_decode($puzzleData[cardInfo],true);
			//echo "--->".mysql_error();
			//echo "--->".$qq."<br>";
			//echo"puzzle...<br>";
			var_dump($puzzleData);
			//echo"puzzle...<br>";

			var_dump($puzzlePathInfo);
			//echo"puzzle...<br>";

			var_dump($puzzleCardInfo);
			//echo"puzzle...<br>";
			//echo "<b><font color=green>".mysql_error()."</font></b>";

			//퍼즐정보업뎃
			//mysql_query("update aPuzzleTable set stageStart=".$stageData[no]." where no=".$puzzleData[no],DBManager::get()->getMainConnection());

			if(!$puzzleData){
				echo "stop~~!!!!!!!!!!!!!!";
				break;
			}
		}

		$stageReward = json_decode($stageData[rewardInfo],true);
		$cards = $puzzleCardInfo[$puzzleCount];
		$pieceNo = $puzzlePathInfo[$puzzleCount];
		$puzzle = $puzzleData["no"];

		$puzzleRewardInfo[] = $stageReward;
		$cTxt = json_encode($cards,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		//스테이지 카드세팅

		$cmodify='{"no":0}';
		if($puzzleCount!=0)$cmodify = '{"no":'.($stageData[no]-1).'}';
		
		$puzzleConditionInfo[] = json_decode($cmodify,true);
		
		$q = "update aStageTable set cards='$cTxt', pieceNo=$pieceNo, puzzle=$puzzle,`condition`='".$cmodify."',`version`=`version`+1 where no=".$stageData[no];
		//echo $q ."<br>";
		mysql_query($q ,DBManager::get()->getMainConnection());

			//echo "<b><font color=green>".mysql_error()."</font></b>";
		//카드능력치 셋팅 by 스테이지몬스터정보
		$boss = json_decode($stageData[boss],true);
		$cardStat = getCardMissile($boss[0],$stageData[level]);
		for($ci=0;$ci<count($cards);$ci++){
			$reward = $stageReward[$ci];
			$sString = json_encode($cardStat[$ci],JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$qs1 = "update aCardTable set missile='".$sString."',reward=$reward,stage=".$stageData[no]." where no=".$cards[$ci];
			mysql_query($qs1,DBManager::get()->getMainConnection());
			//echo "<br><font color=blue>$qs1</font><br>";
			//echo "<b><font color=green>".mysql_error()."</font></b>";
			//echo json_encode($cardStat,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

		}

		$puzzleCount++;

		if($puzzleCount >= count($puzzlePathInfo)){
			$cString = json_encode($puzzleConditionInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$rString = json_encode($puzzleRewardInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$qs = "update aPuzzleTable set  `version`=`version`+1,conditionInfo='".$cString."',rewardInfo='".$rString."' where no=".$puzzleData[no];
			mysql_query($qs,DBManager::get()->getMainConnection());

			//echo "<br><font color=red>$qs</font><br>";
			//echo "<b><font color=green>".mysql_error()."</font></b>";

			$puzzleConditionInfo=array();
			$puzzleRewardInfo=array();

			$puzzleCount=0;
			$puzzleOrder++;
		}

		//스테이지 쭉~~ 불러와서 카드정보, 퍼즐번호 업뎃.
		//퍼즐의 스타트스테이지업뎃

		//
	}




	kvManager::increase("eventPuzzleListVer");
	kvManager::increase("puzzleListVer");
}








	?>