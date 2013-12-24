<?php
include "header.php";



if(!$_GET[where])$where = "puzzle=1 order by no asc";
else $where = $_GET[where];

$result = mysql_query("select * from $TABLE_STAGE where $where",DBManager::get()->getMainConnection());

?>
<center>
<form action=admin_statsClear.php>
퍼즐번호 : <input name='where' size="50" value='<?=$where?>'> <input type=submit value="확인">
</form>

</center><?php
echo"<table border=1 align=center><tr><td>스테이지</td><td>레벨</td><td>시도</td><td>성공</td><td>실패</td><td>???</td><td>성공율</td></tr>";

$lData = array();
while($stageInfo=mysql_fetch_array($result)){
	$stage = $stageInfo[no];
	$level = $stageInfo[level];
	$start = kvManager::get("stage_start_".$stageInfo[no],1)-1;
	$clear = kvManager::get("stage_clear_".$stageInfo[no],1)-1;
	$fail = kvManager::get("stage_over_".$stageInfo[no],1)-1;
	if($start<0)$start=0;
	if($clear<0)$clear=0;
	if($fail<0)$fail=0;
	
	
	$ting = $start-$clear-$fail;
	$level = $stageInfo[level];
	if($fail+$clear==0)$percent=0;
	else $percent = $clear/(float)($fail+$clear)*100;
	
	$lData[$level][level] = $level;
	$lData[$level][stage] += 1;
	$lData[$level][start] += $start;
	$lData[$level][clear] += $clear;
	$lData[$level][fail] += $fail;
	$lData[$level][ting] += $ting;
	echo"<tr><td>$stage</td><td>$level</td><td>$start</td><td>$clear</td><td>$fail</td><td>$ting</td><td>$percent %</td></tr>";	
	
}

foreach($lData as $key=>$data){

	 $level =$data[level];
	 $start = $data[start];
	 $clear =$data[clear];
	 $fail =$data[fail];
	 $ting =$data[ting];
	 $stage =$data[stage];
	 if($fail+$clear==0)$percent=0;
	else $percent = $clear/(float)($fail+$clear)*100;
	
	echo"<tr bgcolor=cccccc><td>종합($stage 개)</td><td>$level</td><td>$start</td><td>$clear</td><td>$fail</td><td>$ting</td><td>$percent %</td></tr>";	

}

echo"</table>";




include "footer.php";
?>