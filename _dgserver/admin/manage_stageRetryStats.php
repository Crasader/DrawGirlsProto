<?php
include "manage_header.php";

$sStage = 1;
$eStage = 25;
$sDate = TimeManager::getCurrentDate();
$eDate = TimeManager::getCurrentDate();
if($_GET["sStage"])$sStage = $_GET["sStage"];
if($_GET["eStage"])$eStage = $_GET["eStage"];
if($_GET["sDate"])$sDate = $_GET["sDate"];
if($_GET["eDate"])$eDate = $_GET["eDate"];
// useHeart,gacha_startmap,clearHeartUp,stage,cupon,archive,loginEvent,tm,cu,new_item_bonus,use_item,gacha_99,shop,allclear,gacha_item,morphing,getHeart,endless,card_rankup,puzzleHeart,puzzle_perfect
// tm, type
// 
$allData = pieceHistoryStats("clearCount");
$allData2 = pieceHistoryStats("tryCount");

?>
<script src="./flot/jquery.js" charset="UTF-8"></script>
<script src="./flot/jquery.flot.js" charset="UTF-8"></script>
		

<h2 id="tables-contextual-classes">|스테이지별 재시도 평균</h2>
<br>
파란색 총시도횟수, 노란색 최초클리어까지 걸린횟수
<br>
<?php
	$r=printStatsList($allData); 
	$r2=printStatsList($allData2);
	$gData=$r[0]; // clearCount
	$gData2=$r2[0]; // tryCount
	$printList = $r[1]; // clearCount
?>
<table width="95%" align=center><tr><td height=500 id="placeholder">


<script src="./flot/jquery.flot.categories.js" charset="UTF-8"></script>
<script type="text/javascript">

	$(function() {


		$.plot("#placeholder", [ <?=json_encode($gData["avg"])?>,<?=json_encode($gData2["avg"])?>], {
			series: {
				bars: {
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

echo $printList;

include "manage_footer.php";
?>