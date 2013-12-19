<?php


error_reporting( E_ALL ^ E_NOTICE );
ini_set( 'display_errors',  E_ALL ^ E_NOTICE );

srand((float)microtime() * 10000000);

include "DBManager.php";

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
$TABLE_IMAGE = "aImageTable";
$TABLE_CHECKPOINT = "aCheckpointTable";
$TABLE_MONSTER_MOVEMENT = "aMonsterMovementTable";
$TABLE_EVENTSTAGE = "aEventStageTable";
$TABLE_PUZZLE = "aPuzzleTable";
$TABLE_CHARACTER = "aCharacterTable";
$TABLE_KEYINTVALUE = "aKeyIntValue";
$TABLE_KEYSTRINGVALUE = "aKeyStringValue";

$TABLE_LOG = "logTempTable";


$HSP_VERSION="1.3";
$HSP_GAMENO="10226";
$HSP_PORT="18080";
$HSP_GATEWAY_URL="http://alpha-httpgw.hangame.com/hsp/httpgw/nomad.json";

$SECRETKEY = "12345678";

$ERRORCODE["puzzle_dontfind"]=array("code"=>15000,"msg"=>"dont find puzzle");





//db connect!
/*
if(!$connect){
$connect=@mysql_connect($LQset[db_host],$LQset[db_id],$LQset[db_pass]);
@mysql_select_db($LQset[db_name], $connect);
}
*/


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
		global $TABLE_KEYSTRINGVALUE;
		$query = mysql_query("select * from $TABLE_KEYSTRINGVALUE where `key`='$key'",DBManager::get()->getMainConnection());
		if($query)$r = mysql_fetch_array($query);
		if($r)return $r["value"];
		else return false;	
	}
	
	public static function getInt($key){
		global $TABLE_KEYINTVALUE;
		
		$query = mysql_query("select * from $TABLE_KEYINTVALUE where `key`='$key'",DBManager::get()->getMainConnection());
		
		
		if($query)$r = mysql_fetch_array($query);
		if($r)return $r["value"];
		else return false;
	}
	
	public static function increase($key){
	
		global $TABLE_KEYINTVALUE;
		
		mysql_query("UPDATE $TABLE_KEYINTVALUE SET `value`=value+1 WHERE `key`='$key'",DBManager::get()->getMainConnection());
		$arows = mysql_affected_rows();
		
		if($arows<=0){
			if(mysql_query("INSERT INTO $TABLE_KEYINTVALUE (`key`, `value`) VALUES ('$key', '1')",DBManager::get()->getMainConnection())){
				return true;
			}else{
				return false;
			}
		}
		
		return true;
	}

	public static function set($key,$value){
		global $TABLE_KEYINTVALUE;
		global $TABLE_KEYSTRINGVALUE;
		//mysql_affected_rows()
		
		$table = $TABLE_KEYINTVALUE;
		if(is_string($value))$table = $TABLE_KEYSTRINGVALUE;
		
		mysql_query("UPDATE $table SET `value`='$value' WHERE `key`='$key'",DBManager::get()->getMainConnection());
		if(mysql_affected_rows()<=0){
			mysql_query("INSERT INTO $table (`key`, `value`) VALUES ('$key', '$value')",DBManager::get()->getMainConnection());
		}
	}
}
	?>