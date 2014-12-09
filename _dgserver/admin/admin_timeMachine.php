<?php
include "header.php";

$memberID = $_GET["memberID"];

if($memberID)$isLock="readonly";

$doApply = $_GET["doApply"];
$sReset = $_GET["sReset"];
$initDateList = $_GET["initDateList"];
if(!$initDateList) $initDateList = "20141120,20141121,20141122,20141123,20141124";
else $isLock="readonly";

if($memberID && $doApply=="true"){
	if($_SESSION['check_$memberID']=="true"){
		echo"$memberID 는 이미 실행한적 있는 유저입니다. 계속하시겠습니까? 계속하려면 아래버튼을 누르고 다시 처음부터 시작해주세요.<br>";
		echo "<a href=admin_timeMachine.php?memberID=$memberID&sReset=true>해당유저아이디로 다시 시도</a><br>";
		echo "<a href=admin_timeMachine.php>실행안함</a>";
		exit;
	}else{
		$_SESSION['check_$memberID']="true";
	}
}

if($memberID && $sReset=="true"){
	unset($_SESSION['check_$memberID']);
}
?>
뽑기어뷰징유저 처리기 <br><br>
step1. 회원번호 입력<br>
<form action=admin_timeMachine.php>
회원번호 : <input type="text" name="memberID" <?=$isLock?> value=<?=$memberID?> ><br>
되돌아갈일자목록 : <input type="text" name="initDateList" size="200" <?=$isLock?> value=<?=$initDateList?>><br>
yyyymmdd 형식으로 콤마로 구분, 마지막엔 콤마붙일필요없음, 처음 적은 날짜로 되돌아감, 날짜순서대로 적을것. ( ex: 20141120,20141121,20141122,20141123,20141124 -> 20141120 기준으로 되돌아감 )<br><br>
<?php if($isLock=="readonly"){?><a href=admin_timeMachine.php><font color=red>[다른유저처리하기]</font></a><?php }else{ ?>

<input type=submit value="정보확인">
<?php } ?>
</form>
<br>

<?php

if($memberID){

?>
step2. 정보확인<br>
- A.현재재화량
<?php

$backPropList = array("h","fr","g","i11","i6","i9","p1","p2","p3","p4","p5","p6","p7","p8");

	$storage = new UserStorage($memberID);

	echo json_encode($storage->getArrayData());
?>
<br><br>
- B.초기재화계산<br>
<?php 
	$_dbc = UserPropertyHistory::getDBGroup()->getConnectionForRead($memberID,null,UserPropertyHistory::getDBTable());

	$takeStorageFind=array();
	$iDateList = explode(",",$initDateList);

	$tableList = array();

	foreach ($iDateList as $key => $value) {
		$tableList[]="UserPropertyHistory_".$value;
	}

	$initialStorage = array();
	foreach ($tableList as $key => $value) {
		$query = mysql_query("SELECT `memberID`,`type`,(total-count) as 'value' FROM ".$value." where memberID='".$memberID."' group by `type` order by `no` asc",$_dbc);
		
		if($query)
		while($history = mysql_fetch_assoc($query)){
			if(in_array($history["type"],$backPropList) && !$initialStorage[$history["type"]]){
				$initialStorage[$history["type"]]=$history["value"];
				$takeStorageFind[$history["type"]]="ok";
			}
		}
	}

	echo json_encode($initialStorage);
?>

<br><br>
- C.스테이지/로그인이벤트/업적 으로 획득한 재화들<br> 
<?php 

	$_dbc = UserPropertyHistory::getDBGroup()->getConnectionForRead($memberID,null,UserPropertyHistory::getDBTable());
	$takeStorage=array();
	foreach ($tableList as $key => $value) {

		$query = mysql_query("SELECT `memberID`,`type`,sum(count) as `value` FROM ".$value." where memberID='".$memberID."' and statsID='archive' group by `type`",$_dbc);
		
			echo mysql_error();
		if($query)
		while($history = mysql_fetch_assoc($query)){
			if($history["type"]){
				$takeStorage[$history["type"]]+=$history["value"];
				$takeStorageFind[$history["type"]]="ok";
			}
			echo mysql_error();
			//echo json_encode($history)."<br>";
		}
	}


?>
<?php 

	foreach ($tableList as $key => $value) {

		$query = mysql_query("SELECT `memberID`,`type`,sum(count) as `value` FROM ".$value." where memberID='".$memberID."' and statsID='loginEvent' group by `type`",$_dbc);
		
			echo mysql_error();
		if($query)
		while($history = mysql_fetch_assoc($query)){
			if($history["type"]){
				$takeStorage[$history["type"]]+=$history["value"];
				$takeStorageFind[$history["type"]]="ok";
			}
			echo mysql_error();
			//echo json_encode($history)."<br>";
		}
	}
?>
<?php 

	foreach ($tableList as $key => $value) {

		$query = mysql_query("SELECT `memberID`,`type`,sum(count) as `value` FROM ".$value." where memberID='".$memberID."' and statsID='attendence' group by `type`",$_dbc);
		
			echo mysql_error();
		if($query)
		while($history = mysql_fetch_assoc($query)){
			if($history["type"]){
				$takeStorage[$history["type"]]+=$history["value"];
				$takeStorageFind[$history["type"]]="ok";
			}
			echo mysql_error();
			//echo json_encode($history)."<br>";
		}
	}
?>
<?php 

	foreach ($tableList as $key => $value) {

		$query = mysql_query("SELECT `memberID`,`type`,sum(count) as `value` FROM ".$value." where memberID='".$memberID."' and statsID='allclear' group by `type`",$_dbc);
		
			echo mysql_error();
		if($query)
		while($history = mysql_fetch_assoc($query)){
			if($history["type"]){
				$takeStorage[$history["type"]]+=$history["value"];
				$takeStorageFind[$history["type"]]="ok";
			}
			echo mysql_error();
			//echo json_encode($history)."<br>";
		}
	}
?>
<?php 

	foreach ($tableList as $key => $value) {
		$query = mysql_query("SELECT memberID,sum(count) FROM ".$value." where memberID='".$memberID."' and statsID='stage'",$_dbc);
	

			echo mysql_error();
		if($query)
		while($history = mysql_fetch_assoc($query)){
			if($history["type"]){
				$takeStorage[$history["type"]]+=$history["value"];
				$takeStorageFind[$history["type"]]="ok";
			}
			
			echo mysql_error();
			//echo json_encode($history)."<br>";
		}
	}
	echo json_encode($takeStorage);
?>

<br><br>
<b>
- D.재화처리필요사항 (B와C를 더하여 적용, 더한값이 A보다 많다면 A로 유지)<br>
<?php
foreach ($backPropList as $key => $value) {
	$initialStorage[$value] += $takeStorage[$value];

	if($takeStorageFind[$value]!="ok"){
		echo $value."=>".$storage->$value."(초기재화량 찾을수없음)<br>";	
	}else{
		if($storage->$value>=$initialStorage[$value]){
			$storage->$value = $initialStorage[$value];
			echo $value."=>".$storage->$value."<br>";	
		}else{
			echo "<font color=red>";
			echo $value."=>".$storage->$value."</font><br>";	

		}
	}

}

if($doApply=="true"){
	echo "<br><br><font color=blue>적용시도";
	echo json_encode($storage->getArrayData())."<br>";
	

	if($storage->save())echo "<font color=blue>적용성공</font>";
echo"</font>";
}

?></b>
<br>*빨간색은 재설정시 값이 오히려 많아지기 때문에 현재값을 유지하는 부분
<br><br>
- E.뽑기로 뽑은 캐릭터<br>
<?php 

	$_dbc = UserPropertyHistory::getDBGroup()->getConnectionForRead($memberID,null,UserPropertyHistory::getDBTable());
	$characterList = array();
	foreach ($tableList as $key => $value) {
		$query = mysql_query("SELECT memberID,count as 'characterNo',count(count) as 'count' FROM ".$value." where `statsID`='gacha' and `type`='cp' and memberID='".$memberID."' group by `count`",$_dbc);

		if($query)
		while($history = mysql_fetch_assoc($query)){
			$characterList[$history["characterNo"]]=$history["count"]*-1;
			echo mysql_error();
			echo json_encode($history)."<br>";
		}
	}
?>



<br><br>
- F.헬모드 스테이지로 획득한 캐릭터<br>
<?php
	$query = CharacterHistory::getQueryResultWithShardKey("SELECT memberID,pieceNo FROM PieceHistory where memberID='".$memberID."' and `pieceNo`>100000 and clearCount>0",$memberID);

	if($query)
	while($history = mysql_fetch_assoc($query)){

		$hellModeInfo = HellMode::getRowByQuery("where pieceNo='".$history["pieceNo"]."'");
		//echo json_encode($hellModeInfo);
		$history["characterNo"]=$hellModeInfo["characterNo"];

		echo json_encode($history);
		if($characterList[$history["characterNo"]])$characterList[$history["characterNo"]]=$characterList[$history["characterNo"]]*-1;
		else $characterList[$history["characterNo"]]=0;

	}

?>

<br><br><b>
- G.캐릭터처리필요사항(F목록에 없는 E목록은 삭제, 있는 것은 count*300 만큼 경험치 제거)<br>
<?php
	foreach ($characterList as $key => $value) {
		
		

		if($value>0){
			echo $key."번 캐릭터 ";
			echo "<font color=red>경험치".($value*300)." 제거</font>,";
			if($doApply=="true"){
				$userCharacter = new CharacterHistory($memberID,$key);
				if(!$userCharacter->isLoaded()){
					echo "로드실패";
				}
				$userCharacter->exp -= $value*300;

				if($userCharacter->exp<0){
					$userCharacter->exp=0;
				}

				if($userCharacter->save())echo"<font color=blue>->적용완료</font>";
			}
		}else if($value<0){
			echo $key."번 캐릭터 ";
			echo "삭제,";
			if($doApply=="true"){
				$userCharacter = new CharacterHistory($memberID,$key);
				if(!$userCharacter->isLoaded()){
					echo "로드실패";
				}

				if($userCharacter->remove())echo"<font color=blue>->삭제완료</font>";
			}
		}
	}
?>
</b>
<br><br>
- H.뽑기로 뽑은 카드<br>
<?php 

	$_dbc = UserPropertyHistory::getDBGroup()->getConnectionForRead($memberID,null,UserPropertyHistory::getDBTable());
	$cardList = array();
	foreach ($tableList as $key => $value) {
		$query = mysql_query("SELECT memberID,statsValue as 'cardNo',count(statsValue) as 'count' FROM ".$value." where `content`='뽑기' and `type`='cd' and `memberID`='".$memberID."' group by `statsValue`",$_dbc);
		echo mysql_error();
		if($query)
		while($history = mysql_fetch_assoc($query)){
			if($cardList[$history["cardNo"]])$cardList[$history["cardNo"]]+=$history["count"];
			else $cardList[$history["cardNo"]]=$history["count"];

			echo mysql_error();
			echo json_encode($history)."<br>";
		}
	}
?>
<br><br><b>
- I.카드처리필요사항 (H목록 유저보유카드에서 한장씩제거, 보유수량0장이면 해당row제거)
<br>
<?php
	foreach ($cardList as $key => $value) {
		$cardHistory = new CardHistory($memberID,$key);
		$cardHistory->count -= $value;

		if($cardHistory->count>0){
			echo $key."번 카드 ".$value."장 제거, ".$cardHistory->count."장 남음";
			$cardHistory->count -= $value;
			if($doApply=="true"){
				if($cardHistory->save())echo"<font color=blue>->적용</font>";
			}
		}
		else {
			echo $key."번 카드 삭제";
			if($doApply=="true"){
				if($cardHistory->remove())echo "<font color=blue>->삭제</font>";
			}
		}


		echo "<br>";
	}

?>
</b>
<br><br>
<form action=admin_timeMachine.php>
<input type="hidden" name="memberID" value=<?=$memberID?>><br>
<input type="hidden" name="doApply" value="true"><br>
<input type="hidden" name="initDateList" value=<?=$initDateList?>><br>
<input type=submit value="적용하기"><br>
</form>

<?php
	}
?>
<?php
include "footer.php";
?>