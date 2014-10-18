<?php
include "manage_header.php";

$sStage = 1;
$eStage = 25;
$sDate = TimeManager::getYesterDate();
$eDate = TimeManager::getCurrentDate();
if($_GET["sStage"])$sStage = $_GET["sStage"];
if($_GET["eStage"])$eStage = $_GET["eStage"];
if($_GET["sDate"])$sDate = $_GET["sDate"];
if($_GET["eDate"])$eDate = $_GET["eDate"];
// useHeart,gacha_startmap,clearHeartUp,stage,cupon,archive,loginEvent,tm,cu,new_item_bonus,use_item,gacha_99,shop,allclear,gacha_item,morphing,getHeart,endless,card_rankup,puzzleHeart,puzzle_perfect

$allData = userStats($sDate,$eDate);
?>
<script src="./flot/jquery.js" charset="UTF-8"></script>
<script src="./flot/jquery.flot.js" charset="UTF-8"></script>
		
<h2 id="tables-contextual-classes">총 추천인 입력자</h2>
<?php
$alluser=0;
	while($result = UserData::getQueryResult("select count(*) from ".UserData::getDBTable()." where introducerID>0")){
		$data = mysql_fetch_array($result);
		$alluser+=$data[0];
	}

	echo $alluser."명";
?>	

<h2 id="tables-contextual-classes">총 다이어리 연동자</h2>
<?php
$alluser=0;
	while($result = UserData::getQueryResult("select count(*) from ".UserData::getDBTable()." where diaryJoinDate>0")){
		$data = mysql_fetch_array($result);
		$alluser+=$data[0];
	}

	echo $alluser."명";
?>	

<h2 id="tables-contextual-classes">평균 플레이판수</h2>
<?php
$alluser=0;
	while($result = UserData::getQueryResult("select count(*) from ".UserData::getDBTable())){
		$data = mysql_fetch_array($result);
		$alluser+=$data[0];
	}

$allPlayCnt=0;
	while($result = PieceHistory::getQueryResult("select sum(tryCount) from ".PieceHistory::getDBTable())){
		$data = mysql_fetch_array($result);
		$allPlayCnt+=$data[0];
	}

	echo $alluser."명 / 총".$allPlayCnt."판/ 평균 ".($allPlayCnt/$alluser)."판";
?>	



<h2 id="tables-contextual-classes">|NRU/DAU</h2>
<br>
<form action="<?=$_SERVER["PHP_SELF"]?>">
<input type="hidden" value="<?=$gid?>" name="gid">
Date : <input type="text" name="sDate" value="<?=$sDate?>">- <input type="text" name="eDate" value="<?=$eDate?>"> <input type="submit" value="검색">
</form>
<br>
<?php

?>
<table width="95%" align=center><tr><td height=500 id="placeholder">
</td></tr></table>
<table class="table table-bordered">
	<tr bgcolor=cccccc>
		<td>DATE</td><td><font color=blue>NRU</font></td><td><font color=yellow>DAU</font></td>
	<tr>

<?php
$gData = array();
	foreach ($allData as $key => $value) {
		$gData["nru"][]=array($key,$value["nru"]);
		$gData["dau"][]=array($key,$value["dau"]);
		# code...

		echo "	<tr>
			<td>".$key."</td><td>".$value["nru"]."</td><td>".$value["dau"]."</td>
		<tr>";
	}
	echo "</table>";
?>

<script src="./flot/jquery.flot.categories.js" charset="UTF-8"></script>
<script type="text/javascript">

	$(function() {


		$.plot("#placeholder", [ <?=json_encode($gData["dau"])?>,<?=json_encode($gData["nru"])?>], {
			series: {
				lines: {
					show: true,
					align: "center",
					barWidth: 0.6
				}
			},
			xaxis: {
				mode: "categories",
				align: "center",
				tickLength: 0
			}
		});

	});

	</script>

<?php

include "manage_footer.php";
?>