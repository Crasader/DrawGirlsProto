<?php
session_cache_expire(60*60*24);
session_start();

//error_reporting( E_ALL ^ E_NOTICE );
//ini_set( 'display_errors',  E_ALL ^ E_NOTICE );

srand((float)microtime() * 10000000);

include_once("DBManager.php");
//include_once("DBManagerForManage.php");

$GAMEID = $_POST["gid"];
if(!$GAMEID)$GAMEID = $_GET["gid"];
if(!$GAMEID)$GAMEID = $_SESSION['game_id'];
$gid = $GAMEID;

if($checkdb=="test"){
	include_once("config_test.php");
}else{
	LogManager::addLog("select db ->".$gid);
	if(!@include_once("config/".$gid.".php")){
		include_once("config.php");
	}			
}

$AES_KEY = "qrqhyrlgprghedvh";
  

function decryptByAESWithBase64($text,$key=null){

	// $st = mcrypt_decrypt(
	// 	MCRYPT_RIJNDAEL_128, 
	// 	'123456789abcdef', 
	// 	$tmp_st, 
	// 	MCRYPT_MODE_ECB, 
	// 	mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_ECB), MCRYPT_RAND)
	// 	);


	global $AES_KEY;
	if(!$key)$key=$AES_KEY;
    $text = base64_decode(str_replace(" ","+",$text));

    $plaintext_dec = mcrypt_decrypt(MCRYPT_RIJNDAEL_128, $key, $text, MCRYPT_MODE_ECB, mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_ECB), MCRYPT_RAND));
    return  $plaintext_dec;
}

function pkcs5_pad ($text) { 
	 $block = mcrypt_get_block_size('rijndael_128', 'ecb');
    $pad = $block - (strlen($str) % $block);
    $text .= str_repeat(chr($pad), $pad); 
    return $text;
} 

function encryptByAESWithBase64($text,$key=null){
	global $AES_KEY;
	if(!$key)$key=$AES_KEY;

    $iv = mcrypt_create_iv(mcrypt_get_iv_size(MCRYPT_RIJNDAEL_128, MCRYPT_MODE_ECB), MCRYPT_RAND); //mcrypt_create_iv($iv_size, MCRYPT_RAND);
    $ciphertext = mcrypt_encrypt(MCRYPT_RIJNDAEL_128, $key,
                                 $text, MCRYPT_MODE_ECB, $iv);
    $ciphertext = pkcs5_pad($ciphertext);
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
		$query = mysql_query("select * from `aKeyStringValue` where `key`='$key'",DBGroup::create("main")->getConnectionForReadByRand());
		if($query)$r = mysql_fetch_array($query);
		if($r)return $r["value"];
		else return false;	
	}
	
	public static function getInt($key){
		
		$query = mysql_query("select * from `aKeyIntValue` where `key`='$key'",DBGroup::create("main")->getConnectionForReadByRand());
		
		
		if($query)$r = mysql_fetch_array($query);
		if($r)return $r["value"];
		else return false;
	}
	
	public static function increase($key){
	
		
		mysql_query("UPDATE `aKeyIntValue` SET `value`=value+1 WHERE `key`='$key'",DBGroup::create("main")->getConnectionForReadByRand());
		$arows = mysql_affected_rows();
		
		if($arows<=0){
			if(mysql_query("INSERT INTO `aKeyIntValue` (`key`, `value`) VALUES ('$key', '1')",DBGroup::create("main")->getConnectionForReadByRand())){
				return true;
			}else{
				return false;
			}
		}
		
		return true;
	}

	public static function set($key,$value){
		//mysql_affected_rows()
		
		$table = "aKeyIntValue";
		if(is_string($value))$table = "aKeyStringValue";
		
		mysql_query("UPDATE $table SET `value`='$value' WHERE `key`='$key'",DBGroup::create("main")->getConnectionForReadByRand());
		if(mysql_affected_rows()<=0){
			mysql_query("INSERT INTO $table (`key`, `value`) VALUES ('$key', '$value')",DBGroup::create("main")->getConnectionForReadByRand());
		}
	}
}


function reloadPuzzleInfo(){


	//CommitManager::begin("main");
	mysql_query("update ".Piece::getDBTable()." set cards='',puzzle=0,pieceNo=0",DBGroup::create("main")->getConnectionForWrite(1));
	mysql_query("update ".Card::getDBTable()." set piece=0,grade=0",DBGroup::create("main")->getConnectionForWrite(1));


	//모두세팅!!!!! 일반

	//mysql_query("update aCardTable set ability='[]'",DBManager::getMainConnection());

	//퍼즐보상카드

	while($obj=HellMode::GetObjectByQuery("")){
		$piece = new Piece($obj->pieceNo);
		
		if($piece->isLoaded()){
			$piece->cards = array($obj->cardNo);
			$piece->puzzle = -1;
			$piece->save();
		}

		$card = new Card($obj->cardNo);
		if($card->isLoaded()){
			$card->piece = $obj->pieceNo;
			$card->grade = 1;
			$card->save();
		}

	}

	$query = mysql_query("select * from ".Puzzle::getDBTable(),DBGroup::create("main")->getConnectionForReadByRand());
	while($pData = mysql_fetch_assoc($query)){
		$clearReward = json_decode($pData["clearReward"],true);
		if($clearReward["normal"]){
			mysql_query("update ".Card::getDBTable()." set category='nPuzzle' where no=".$clearReward["normal"],DBGroup::create("main")->getConnectionForWrite(1));
		}

		if($clearReward["perfect"]){
			mysql_query("update ".Card::getDBTable()." set category='nPuzzle' where no=".$clearReward["perfect"],DBGroup::create("main")->getConnectionForWrite(1));
		}
	}

	$query = mysql_query("select * from ".Piece::getDBTable()." order by no asc",DBGroup::create("main")->getConnectionForReadByRand());
	//LogManager::addLog(mysql_error());
	$puzzleOrder=1;
	$puzzleCount=0;
	$puzzleData = null;
	$puzzlePathInfo = null;
	$puzzleConditionInfo = array();
	while($stageData = mysql_fetch_array($query)){
		
		if($puzzleCount==0){
			$qq = "select * from ".Puzzle::getDBTable()." where `order`=$puzzleOrder";
			$puzzleData = mysql_fetch_assoc(mysql_query($qq,DBGroup::create("main")->getConnectionForReadByRand()));
			$puzzlePathInfo = json_decode($puzzleData[pathInfo],true);
			$puzzleCardInfo = json_decode($puzzleData[cardInfo],true);
			
			if(!$puzzleData){
				//LogManager::addLog("stop~~~~!!!!");
				//echo "stop~~!!!!!!!!!!!!!!";
				break;
			}
		}

		$cards = $puzzleCardInfo[$puzzleCount];
		$pieceNo = $puzzlePathInfo[$puzzleCount];
		$puzzle = $puzzleData["no"];

		$cTxt = json_encode($cards,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		//스테이지 카드세팅

		$cmodify='{"no":0}';
		if($puzzleCount!=0)$cmodify = '{"no":'.($stageData[no]-1).'}';
		
		$originCondition = json_decode($stageData["condition"],true);
		if($originCondition["gold"])$cmodify = $stageData["condition"];
		
		$puzzleConditionInfo[] = json_decode($cmodify,true);
		
		$q = "update ".Piece::getDBTable()." set cards='$cTxt', pieceNo=$pieceNo, puzzle=$puzzle,`condition`='".$cmodify."',`version`=`version`+1 where no=".$stageData[no];
		//echo $q ."<br>";
		mysql_query($q ,DBGroup::create("main")->getConnectionForWrite(1));

		//카드능력치 셋팅 by 스테이지몬스터정보
		$boss = json_decode($stageData[boss],true);
		for($ci=0;$ci<count($cards);$ci++){
			$reward = $stageReward[$ci];
			$grade = $ci+1;
			$qs1 = "update ".Card::getDBTable()." set piece=".$stageData[no].",grade=$grade,category='normal' where no=".$cards[$ci]; //
			mysql_query($qs1,DBGroup::create("main")->getConnectionForWrite(1));

		}

		$puzzleCount++;

		if($puzzleCount >= count($puzzlePathInfo)){
			$cString = json_encode($puzzleConditionInfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
			$qs = "update ".Puzzle::getDBTable()." set  `version`=`version`+1,conditionInfo='".$cString."' where no=".$puzzleData[no];
			mysql_query($qs,DBGroup::create("main")->getConnectionForWrite(1));
			
			$puzzleConditionInfo=array();
			$puzzleRewardInfo=array();
 
			$puzzleCount=0;
			$puzzleOrder++;
		}

		
	}


	kvManager::increase("puzzleListVer");
}

function userStats($sDate,$eDate){
	$allData = array();
	$cDate = $sDate;

	$where = $type."='".$statsID."'";
	
	while(1){
		$cnt=0;		
		while($result = UserData::getQueryResult("select count(no) from ".UserData::getDBTable()." where `joinDate` like '".$cDate."%'")){
			$data = mysql_fetch_array($result);
			$cnt+=$data[0];
		}
		$allData[$cDate]["nru"]=$cnt;
		$cnt=0;
		while($result = UserPropertyHistory::getQueryResult("select count(DISTINCT memberID) from "."UserPropertyHistory_".$cDate)){
			$data = mysql_fetch_array($result);
			$cnt+=$data[0];
		}

		$allData[$cDate]["dau"]=$cnt;

		if($cDate==$eDate)break;
		$cTimestamp = TimeManager::dateStringToDate($cDate);
		$cDate = date("Ymd",$cTimestamp+24*60*60);

	}
	ksort($allData);
	return $allData;
}


function userPropStats($statsID,$sDate,$eDate,$groupField,$type="statsID"){
	$allData = array();
	$cDate = $sDate;

	$where = $type."='".$statsID."'";
	
	while(1){
		UserPropertyHistory::setDBTable("UserPropertyHistory_".$cDate);
		// statsID, statsValue 가 카테고리 나누는용도
		
		// while($q = UserPropertyHistory::getQueryResult("select DISTINCT statsID from `".UserPropertyHistory::getDBTable())){
		// 	while($data=mysql_fetch_assoc($q)){
		// 		echo $data["statsID"].",";
		// 	}
		// }
		while($q = UserPropertyHistory::getQueryResult("select `".$groupField."`,statsID,count(*) as `cnt`,sum(`count`) as `sum`,max(`count`) as `max`,min(`count`) as `min`,avg(`count`) as `avg` from `".UserPropertyHistory::getDBTable()."` where ".$where." group by `".$groupField."`")){
			
			while($data=mysql_fetch_assoc($q)){
				if(!$allData[$data[$groupField]]["max"] || $allData[$data[$groupField]]["max"]<$data["max"])$allData[$data[$groupField]]["max"]=$data["max"];
				if(!$allData[$data[$groupField]]["min"] || $allData[$data[$groupField]]["min"]>$data["min"])$allData[$data[$groupField]]["min"]=$data["min"];
				$allData[$data[$groupField]]["sum"]+=$data["sum"];
				$allData[$data[$groupField]]["cnt"]+=$data["cnt"];
				$allData[$data[$groupField]]["avg"]=$allData[$data[$groupField]]["sum"]/$allData[$data[$groupField]]["cnt"];
			};
			
		}

		if($cDate==$eDate)break;
		$cTimestamp = TimeManager::dateStringToDate($cDate);
		$cDate = date("Ymd",$cTimestamp+24*60*60);

	}

	ksort($allData);
	return $allData;
}

function pieceHistoryStats($statsField,$where=""){
	$allData = array();
	$groupField="pieceNo";
	//$statsField="retryCount";

	while($q = PieceHistory::getQueryResult("select `".$groupField."`,count(*) as `cnt`,sum(`".$statsField."`) as `sum`,max(`".$statsField."`) as `max`,min(`".$statsField."`) as `min`,avg(`".$statsField."`) as `avg` from `".PieceHistory::getDBTable()."` ".$where." group by `".$groupField."`")){			
		while($data=mysql_fetch_assoc($q)){
			if(!$allData[$data[$groupField]]["max"] || $allData[$data[$groupField]]["max"]<$data["max"])$allData[$data[$groupField]]["max"]=$data["max"];
			if(!$allData[$data[$groupField]]["min"] || $allData[$data[$groupField]]["min"]>$data["min"])$allData[$data[$groupField]]["min"]=$data["min"];
			$allData[$data[$groupField]]["sum"]+=$data["sum"];
			$allData[$data[$groupField]]["cnt"]+=$data["cnt"];
			$allData[$data[$groupField]]["avg"]=$allData[$data[$groupField]]["sum"]/$allData[$data[$groupField]]["cnt"];
		};
			
	}

	ksort($allData);
	return $allData;
}

function cardHistoryStats($statsField,$where=""){
	$allData = array();
	$groupField="cardNo";
	//$statsField="retryCount";

	while($q = CardHistory::getQueryResult("select `".$groupField."`,count(*) as `cnt`,sum(`".$statsField."`) as `sum`,max(`".$statsField."`) as `max`,min(`".$statsField."`) as `min`,avg(`".$statsField."`) as `avg` from `".CardHistory::getDBTable()."` ".$where." group by `".$groupField."`")){			
		while($data=mysql_fetch_assoc($q)){
			if(!$allData[$data[$groupField]]["max"] || $allData[$data[$groupField]]["max"]<$data["max"])$allData[$data[$groupField]]["max"]=$data["max"];
			if(!$allData[$data[$groupField]]["min"] || $allData[$data[$groupField]]["min"]>$data["min"])$allData[$data[$groupField]]["min"]=$data["min"];
			$allData[$data[$groupField]]["sum"]+=$data["sum"];
			$allData[$data[$groupField]]["cnt"]+=$data["cnt"];
			$allData[$data[$groupField]]["avg"]=$allData[$data[$groupField]]["sum"]/$allData[$data[$groupField]]["cnt"];
		};
			
	}

	ksort($allData);
	return $allData;
}


function charHistoryStats($statsField,$where=""){
	$allData = array();
	$groupField="level";
	//$statsField="retryCount";

	while($q = CharacterHistory::getQueryResult("select `".$groupField."`,count(*) as `cnt`,sum(`".$statsField."`) as `sum`,max(`".$statsField."`) as `max`,min(`".$statsField."`) as `min`,avg(`".$statsField."`) as `avg` from `".CharacterHistory::getDBTable()."` ".$where." group by `".$groupField."`")){			
		while($data=mysql_fetch_assoc($q)){
			if(!$allData[$data[$groupField]]["max"] || $allData[$data[$groupField]]["max"]<$data["max"])$allData[$data[$groupField]]["max"]=$data["max"];
			if(!$allData[$data[$groupField]]["min"] || $allData[$data[$groupField]]["min"]>$data["min"])$allData[$data[$groupField]]["min"]=$data["min"];
			$allData[$data[$groupField]]["sum"]+=$data["sum"];
			$allData[$data[$groupField]]["cnt"]+=$data["cnt"];
			$allData[$data[$groupField]]["avg"]=$allData[$data[$groupField]]["sum"]/$allData[$data[$groupField]]["cnt"];
		};
			
	}

	ksort($allData);
	return $allData;
}

function printStatsList($allData){

	$print = '</td></tr></table>
	<table class="table table-bordered">
	<tr>
		<td>category</td><td>count</td><td>sum</td><td>avg</td><td>min</td><td>max</td>
	<tr>';

	$gData = array();

	foreach ($allData as $key => $value) {
		$gData["avg"][]=array($key,$value["avg"]);
		$gData["max"][]=array($key,$value["max"]);
		$gData["min"][]=array($key,$value["min"]);
		$gData["sum"][]=array($key,$value["sum"]);
		$gData["cnt"][]=array($key,$value["cnt"]);
		# code...

		$print.= "	<tr>
			<td>".$key."</td><td>".$value["cnt"]."</td><td>".$value["sum"]."</td><td>".$value["avg"]."</td><td>".$value["min"]."</td><td>".$value["max"]."</td>
		<tr>";
	}

	$print.= "</table>";

	return array($gData,$print);
}

	?>