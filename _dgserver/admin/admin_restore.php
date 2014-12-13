<?php
include "header.php";

$memberID = $_GET["memberID"];
$doApply = $_GET["doApply"];

?>

캐릭터/카드 복구기 - 헬모드클리어기준, 피스획득여부를 검사하여 클리어로 보상받아야했지만 못받은 카드/캐릭터를 복구합니다. <br><br>

step1. 회원번호 입력<br>
<form action=admin_restore.php>
회원번호 : <input type="text" name="memberID" <?=$isLock?> value=<?=$memberID?> ><br>
<?php if($isLock=="readonly"){?><a href=admin_timeMachine.php><font color=red>[다른유저처리하기]</font></a><?php }else{ ?>

<input type=submit value="정보확인">
<?php } ?>
</form>
<br>

<?php

if($memberID){
echo $memberID."번 회원<br><br>";
?>
step2. 정보확인<br>
<?php		

	$hellModeList = array();
	while($pData =  HellMode::getRowByQuery("")){
		$hellModeList[$pData["pieceNo"]]=$pData["characterNo"];
	}


	while($rData = PieceHistory::getRowByQueryWithShardKey("where memberID='".$memberID."'",$memberID)){
		$stage = new Piece($rData["pieceNo"]);
		$clearList = json_decode($rData["clearDateList"],true);
		$cardList =& $stage->getRef("cards");
		if($stage->type == "normal"){
			for($i=0;$i<count($clearList);$i++) {
				$cardNo = $cardList[$i];
				$isClear = $clearList[$i];
				if($isClear>0){
					$myCard = new CardHistory($memberID,$cardNo);
					if(!$myCard->isLoaded() || $myCard->count<=0){

						echo "[카드]".$stage->no."스테이지 ".$cardNo."번 카드는 클리어했지만 카드가 없는 상태";

						if($doApply=="true"){
							$myCard->count=1;
							$myCard->takeDate=TimeManager::getCurrentDateTime();
							$myCard->level=0;
							if($myCard->save()){
								echo "<font color=blue>->복구완료</font>";
							}
						}
						echo"<br>";
					}
				}
			}
		}else if($stage->type=="hell"){
			$charNo = $hellModeList[$stage->no];
			$myChar = new CharacterHistory($memberID,$charNo);
			if(!$myChar->isLoaded()){
				echo "[캐릭터]".$stage->no."헬모드 클리어했으나 ".$charNo."번 캐릭터없음";

				if($doApply=="true"){
					$myChar->characterNo=$charNo;
					$myChar->regDate=TimeManager::getCurrentDateTime();
					$myChar->level=1;
					if($myChar->save()){
						echo "<font color=blue>->복구완료</font>";
					}
				}

				echo "<br>";
			}
		}

	}

?>
</b>
<br><br>
<form action=admin_restore.php>
<input type="hidden" name="memberID" value=<?=$memberID?>><br>
<input type="hidden" name="doApply" value="true"><br>
<input type=submit value="적용하기"><br>
</form>

<?php
	}
?>
<?php
include "footer.php";
?>