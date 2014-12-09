<?php
include "manage_header.php";
?>


<h2><b>일반뽑기</b></h2>
<br>

<table border=1>
<tr>
	<td>exchagneID</td>
	<td>level</td>
	<td>노출설정값</td>
	<td>당첨설정값</td>
	<td>노출확률</td>
	<td>당첨확률</td>
	<td>최종확률</td>
	<td>노출후<br>기대가격</td>
	<td>기대가격</td>
</tr>

<?php
$cs = new CommonSetting("gachaForm");
$productGainProb =& $cs->getRef("value");
$productGainProb = $productGainProb["normal"];

//일반 nmlGc , 고급 prmGc
$cs2 = new CommonSetting("nmlGc");
$price =& $cs2->getRef("value");

echo "상품구성:".json_encode($productGainProb)."<br>";
echo "가격".json_encode($price)."<br>";

$sumGain = array_sum($productGainProb);

$data=array();
$allGainCnt = array();
$allShowCnt = array();
while($obj = Gacha::getObjectByQuery("where premiumOnly=0 order by level asc")){

	$data[]=$obj->getArrayData();
	$allShowCnt[$obj->level] +=$obj->showProb;
	
	$allGainCnt[$obj->level] =$obj->gainProb;

}

$allGainCnt[1]=$allGainCnt[1]*$productGainProb[0];
$allGainCnt[2]=$allGainCnt[2]*$productGainProb[1];
$allGainCnt[3]=$allGainCnt[3]*$productGainProb[2];
$allGainCnt[4]=$allGainCnt[4]*$productGainProb[3];

$allGainCntSum = array_sum($allGainCnt);


$resultPSum=0;
$levelNeedPrice
foreach ($data as $key => $value) {
	# code...
	$level = $value["level"];
	$showP = $value[showProb]/$allShowCnt[$level];
	$gainP = $value[gainProb]/$allGainCntSum;
	$resultP = $showP*$gainP*$productGainProb[$level-1];
	$startIndex = count($price)-1;
	$pp = 0.5/$resultP*$price[$startIndex];
	$pp2 = 0.5/$gainP*$price[$startIndex];

	$ex = new Exchange($value[exchangeID]);
	$cate = Exchange::getCategory();
	$resultPSum += $resultP*100;
	$exList =& $ex->getRef("list");
	
	echo "<tr><td>";
	echo $cate[$exList[0]["type"]]["title"]."-".$exList[0]["count"];
	echo "</td><td>";
	echo $value[level];
	echo "</td><td>";
	echo $value[showProb];
	echo "</td><td>";
	echo $value[gainProb];
	echo "</td><td>";
	echo round($showP*100,1)."%";
	echo "</td><td>";
	echo round($gainP*100,1)."%";
	echo "</td><td>";
	echo round($resultP*100,1)."%";
	echo "</td><td>";
	echo floor($pp2);
	echo "</td><td>";
	echo floor($pp);
	echo "</td></tr>";
}
?>


</table>

최종확률합계 : <?=$resultPSum?> <br>
당첨설정값합계 : <?=$allGainCntSum?> <br><br><br>




<h2><b>고급뽑기</b></h2>
<br>

<table border=1>
<tr>
	<td>exchagneID</td>
	<td>level</td>
	<td>노출설정값</td>
	<td>당첨설정값</td>
	<td>노출확률</td>
	<td>당첨확률</td>
	<td>최종확률</td>
	<td>노출후<br>기대가격</td>
	<td>기대가격</td>
</tr>

<?php
$cs = new CommonSetting("gachaForm");
$productGainProb =& $cs->getRef("value");
$productGainProb = $productGainProb["premium"];

//일반 nmlGc , 고급 prmGc
$cs2 = new CommonSetting("prmGc");
$price =& $cs2->getRef("value");

echo "상품구성:".json_encode($productGainProb)."<br>";
echo "가격".json_encode($price)."<br>";

$sumGain = array_sum($productGainProb);

$data=array();
$allGainCnt = array();
$allShowCnt = array();
while($obj = Gacha::getObjectByQuery("where premiumOnly=1 order by level asc")){

	$data[]=$obj->getArrayData();
	$allShowCnt[$obj->level] +=$obj->showProb;
	
	$allGainCnt[$obj->level] =$obj->gainProb;

}

$allGainCnt[1]=$allGainCnt[1]*$productGainProb[0];
$allGainCnt[2]=$allGainCnt[2]*$productGainProb[1];
$allGainCnt[3]=$allGainCnt[3]*$productGainProb[2];
$allGainCnt[4]=$allGainCnt[4]*$productGainProb[3];

$allGainCntSum = array_sum($allGainCnt);


$resultPSum=0;
foreach ($data as $key => $value) {
	# code...
	$level = $value["level"];
	$showP = $value[showProb]/$allShowCnt[$level];
	$gainP = $value[gainProb]/$allGainCntSum;
	$resultP = $showP*$gainP*$productGainProb[$level-1];
	$startIndex = count($price)-1;
	$pp = 0.5/$resultP*$price[$startIndex];
	$pp2 = 0.5/$gainP*$price[$startIndex];

	$ex = new Exchange($value[exchangeID]);
	$cate = Exchange::getCategory();
	$resultPSum += $resultP*100;
	$exList =& $ex->getRef("list");
	
	echo "<tr><td>";
	echo $cate[$exList[0]["type"]]["title"]."-".$exList[0]["count"];
	echo "</td><td>";
	echo $value[level];
	echo "</td><td>";
	echo $value[showProb];
	echo "</td><td>";
	echo $value[gainProb];
	echo "</td><td>";
	echo round($showP*100,1)."%";
	echo "</td><td>";
	echo round($gainP*100,1)."%";
	echo "</td><td>";
	echo round($resultP*100,1)."%";
	echo "</td><td>";
	echo floor($pp2);
	echo "</td><td>";
	echo floor($pp);
	echo "</td></tr>";
}
?>


</table>

최종확률합계 : <?=$resultPSum?> <br>
당첨설정값합계 : <?=$allGainCntSum?> 

<?php
include "manage_footer.php";
?>
