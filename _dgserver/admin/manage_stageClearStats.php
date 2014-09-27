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
$allData = pieceHistoryStats("clearCount","where clearCount>0");

?>
<script src="./flot/jquery.js" charset="UTF-8"></script>
<script src="./flot/jquery.flot.js" charset="UTF-8"></script>
		

<h2 id="tables-contextual-classes">|스테이지별 클리어 통계</h2>
<br>
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


		$.plot("#placeholder", [ <?=json_encode($gData["cnt"])?>], {
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