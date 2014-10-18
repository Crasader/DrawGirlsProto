<?php
include "manage_header.php";


$result = Piece::getQueryResultWithShardKey("select * from ".Piece::getDBTable()." where no<=100 order by no asc",1);
?>
<center>
<table border=1>
	<tr align=center>
		<td colspan=4>스테이지</td>
		<td colspan=12>보스</td>
		<td colspan=6>주니어</td>
	</tr>
	<tr>
		<td>번호</td>
		<td>레벨</td>
		<td>미션</td>
		<td>오토벨런스<br>횟수</td>
		<td>Name</td>
		<td>AI</td>
		<td>공격주기</td>
		<td>5초안에<br>공격할확률</td>
		<td>체력</td>
		<td>회피<br>(사용안함)</td>
		<td>속도<br>(px/frame)</td>
		<td>좌우끝<br>이동시간(초)</td>
		<td>미사일</td>
		<td>순간이동</td>
		<td>특수</td>
		<td>파괴</td>
		<td>Name</td>
		<td>갯수</td>
		<td>속도<br>(px/frame)</td>
		<td>체력</td>
		<td>회피<br>(사용안함)</td>
		<td>AI</td>
	</tr>
<?php

while($data = mysql_fetch_assoc($result)){
	
	$boss=json_decode($data["boss"],true);
	$jr = json_decode($data["junior"],ture);
	$mission = json_decode($data["mission"],true);
	$ps="";
	$pc="";
	$pm="";
	foreach ($boss[0]["pattern"] as $key => $value) {
		$pattern = new Pattern($boss[0]["pattern"][$key]["pattern"]);
		if($boss[0]["pattern"][$key]["atype"]=="special")$ps.=$pattern->name.",";
		if($boss[0]["pattern"][$key]["atype"]=="crash")$pc.=$pattern->name.",";
		if($boss[0]["pattern"][$key]["atype"]=="normal")$pm="O";
	}
	$bgcolor="ffffff";
	if($data[no]%2)$bgcolor="eeeeee";
	if($data[no]%5==0)$bgcolor="eeffff";
	

	$missionName = "";
	if($mission[type]){
		$missionInfo = new Mission($mission[type]);
		if($missionInfo->isLoaded())$missionName=$missionInfo->name;
	}
	$ap =$boss[0][attackpercent];
	$sPer =(1-pow((1 - $ap/100),(60 * 5)))*100;
	echo"
	<tr bgcolor=$bgcolor>
		<td>".$data[no]."</td>
		<td>".$data[level]."</td>
		<td>".$missionName."(".json_encode($mission[option]).")</td>
		<td>".$data[autoBalanceTry]."</td>
		<td>".$boss[0][type]."</td>
		<td>".round($boss[0][ai])."</td>
		<td>".round($ap,2)."</td>
		<td>".round($sPer,1)."%</td>
		<td>".round($boss[0][hp])."</td>
		<td>".round($boss[0][agi])."</td>
		<td>".round($boss[0][speed][start],2)."</td>
		<td>".round(320/($boss[0][speed][start]*60),1)."</td>
		<td>".$pm."</td>
		<td>O</td>
		<td>".$ps."</td>
		<td>".$pc."</td>
		<td>".$jr[0][type]."</td>
		<td>".count($jr)."</td>
		<td>".round($jr[0][speed][start],2)."</td>
		<td>".round($jr[0][hp])."</td>
		<td>".round($jr[0][agi])."</td>
		<td>".round($jr[0][ai])."</td>
	</tr>
	";
}

echo"</table></center>";
include "manage_footer.php";
?>
