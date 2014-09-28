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
$allData = cardHistoryStats("isMorphing");

?>
<script src="./flot/jquery.js" charset="UTF-8"></script>
<script src="./flot/jquery.flot.js" charset="UTF-8"></script>
		

<h2 id="tables-contextual-classes">|카드별 모핑 통계 (실제적용)</h2>
<br>노란색 카량 / 파란색 모핑적용량
<br>
<?php
	$r=printStatsList($allData);
	$gData=$r[0];
	$gData2=$r2[0];
	$printList = $r[1];
?>
<table width="95%" align=center><tr><td height=500 id="placeholder">


<script src="./flot/jquery.flot.categories.js" charset="UTF-8"></script>
<script type="text/javascript">

	$(function() {


		$.plot("#placeholder", [ <?=json_encode($gData["cnt"])?>,<?=json_encode($gData["sum"])?>], {
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