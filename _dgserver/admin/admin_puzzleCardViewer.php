<?php
include "header.php";

$memberID = $_GET["memberID"];
$doApply = $_GET["doApply"];

?>

스테이지/퍼즐 클리어상황 뷰어<br><br>

step1. 회원번호 입력<br>
<form action=admin_puzzleCardViewer.php>
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
	$oldPuzzle = 0;
	$nowPuzzle = 0;
	$isNormalClaer=true;
	$isPerfectClear=true;
	echo "<table border=1>";
	while($rData = Piece::getRowByQuery("where `type`='normal' order by `no` asc")){
		$nowPuzzle = $rData["puzzle"];
		if($nowPuzzle!=$oldPuzzle){
			if($oldPuzzle){
				echo "<tr><td colspan=5>";
				$puzzleInfo = new Puzzle($oldPuzzle);
				$clearReward =& $puzzleInfo->getRef("clearReward");
				if($isNormalClaer){
					echo "노말클리어성공 => ";
					echo $clearReward["normal"]."번카드 ";
					$cardH = new CardHistory($memberID,$clearReward["normal"]);
					if($cardH->isLoaded() && $cardH->count>0){
						echo $cardH->count."장 보유<br>";	
					}else{
						echo "<font color=red>미보유</font><br>";
					}

				}else{
					echo "노말클리어실패";
				}

				echo "</td></tr><tr><td colspan=5>";
				if($isPerfectClear){
					echo "퍼펙클리어성공 => ";
					echo $clearReward["perfect"]."번카드 ";
					$cardH = new CardHistory($memberID,$clearReward["perfect"]);
					if($cardH->isLoaded() && $cardH->count>0){
						echo $cardH->count."장 보유<br>";	
					}else{
					echo "<font color=red>미보유</font><br>";
					}
				}else{
					echo " 퍼펙클리어실패";
				}
				echo "</td></tr>";
			}
			$isNormalClaer=true;
			$isPerfectClear=true;
			echo "<tr><td colspan=5 bgcolor=cccccc><br><br>".$nowPuzzle."번 퍼즐</td></tr>";
			echo "<tr><td>스테이지번호</td><td>1단계</td><td>2단계</td><td>3단계</td><td>4단계</td></tr>";
		}
		
		echo "<tr><td>".$rData["no"]."번 스테이지</td>";

		$pieceHistory = new PieceHistory($memberID,$rData["no"]);

		if(!$pieceHistory->isLoaded()){
			echo "<td colspan=4>오픈못함</td></tr>"; 
			$isNormalClaer=false;
			$isPerfectClear=false;
			$oldPuzzle = $nowPuzzle;
			continue;
		}

		$clearList =& $pieceHistory->getRef("clearDateList");

		$cardList = json_decode($rData["cards"],true);

		$isClear=false;
		foreach ($clearList as $index => $clearDate) {
			//echo ($index+1)."단계->";
			if($clearDate>0){
				$cardHistory = new CardHistory($memberID,$cardList[$index]);
				echo"<td>Clear<br>(".$clearDate.")<br>";

				echo $cardList[$index]."번 카드:";
				if($cardHistory->isLoaded() && $cardHistory->count>0){
					echo $cardHistory->count."장 보유<br>";	
				}else{
					echo "<font color=red>미보유</font><br>";
				}

				echo "</td>";
				$isClear=true;
			}
			else 
			{
				$isPerfectClear=false;
				echo"<td>X</td>";
			}

		}

		if(!$isClear){
			$isNormalClaer=false;
		}

		echo"</tr>";

		$oldPuzzle = $nowPuzzle;


	}
	echo "</table>";

?>
</b>
<br><br>


<?php
	}
?>
<?php
include "footer.php";
?>