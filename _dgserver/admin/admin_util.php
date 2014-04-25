<?php
include "header.php";
?>

<script>


$(document).ready(function() {
	$('body').on('click','#convertButton',function(){
		var convert_data = $("#ozString").val();
		is_zero = true;
		error_cnt = 0;
		cmp_cnt = 0;
		result_data = "";
		for(i=0;i<convert_data.length;i++){
			if(is_zero)
			{
				if(convert_data[i] == '0')
				{
					cmp_cnt++;
				}
				else if(convert_data[i] == '1')
				{
					if(cmp_cnt >= 10000)		result_data+="D";
					else if(cmp_cnt >= 1000)	result_data+="C";
					else if(cmp_cnt >= 100)		result_data+="B";
					else if(cmp_cnt >= 10)		result_data+="A";
					
					result_data+=cmp_cnt;
					cmp_cnt = 1;
					is_zero = false;
				}
				else
				{
	//				CCLog("invalid data %s", convert_data.substr(i,1).c_str());
					if(error_cnt == 0)
						log("error data : "+convert_data[i]);
					error_cnt++;
				}
			}
			else
			{
				if(convert_data[i] == '1')
				{
					cmp_cnt++;
				}
				else if(convert_data[i] == '0')
				{
					if(cmp_cnt >= 10000)		result_data+="D";
					else if(cmp_cnt >= 1000)	result_data+="C";
					else if(cmp_cnt >= 100)		result_data+="B";
					else if(cmp_cnt >= 10)		result_data+="A";
					
					result_data+=cmp_cnt;
					cmp_cnt = 1;
					is_zero = true;
				}
				else
				{

					if(error_cnt == 0)
						log("error data : "+convert_data[i]);
					
					error_cnt++;

				}
			}
		}
		
		
		
	if(cmp_cnt >= 10000)		result_data+="D";
	else if(cmp_cnt >= 1000)	result_data+="C";
	else if(cmp_cnt >= 100)		result_data+="B";
	else if(cmp_cnt >= 10)		result_data+="A";
	
	result_data+=cmp_cnt;
	
	log("error count is "+error_cnt);
	log("original size is "+convert_data.length+" convert size is "+result_data.length);
	$("#resultString").text(result_data);


	});

});


</script>
<center>

<b><font color=red>실루엣데이터변환기</font></b><br>
<textarea id="ozString" rows=10 cols=50></textarea>
<input type=button id="convertButton" value=">>>">
<textarea id="resultString" rows=10 cols=50></textarea>
<br><br>

<form action=admin_util.php>
	<input name="gid" value="<?=$gid?>" type="hidden">
<input type=hidden name=mode value=addStage>
<b><font color=red>퍼즐 생성기 - 1번 퍼즐의 스테이지들을 복사합니다.</font></b><br>
복사할 퍼즐번호 <input type=text name=originalPuzzleNo> 생성할 퍼즐번호 <input type=text name=puzzleNo> 비번(asdf) <input type=text name=pass size=4> <input type=submit value=생성>
</form>

<?php
if($_GET[mode]=="addStage" && $_GET[puzzleNo] && $_GET[pass]=="asdf"){
	echo "<textarea cols=10 rows=10>"; 
	$i=1;
	$puzzleNo = $_GET[puzzleNo];
	$originalPuzzleNo = $_GET[originalPuzzleNo]; 
	echo "select * from ".DBManager::get()->getMT("stage")." where `puzzle`=$originalPuzzleNo \n";
	$query = mysql_query("select * from ".DBManager::get()->getMT("stage")." where `puzzle`=$originalPuzzleNo",DBManager::get()->getMainConnection());
	while($data = mysql_fetch_array($query)){
		for($j=0;$j<count($data);$j++){
				unset($data[$j]);
		}
		
		$stageNo = $i+24*($puzzleNo-1);
		$themeNo = (int)(($i-1)/6)+($puzzleNo-1)*4+1;
		$data[no]=$stageNo;
		$data[theme]=$themeNo;
		$data[puzzle]=$puzzleNo;
		$sCard = $stageNo*10;
		$data[cards]="[".($sCard).",".($sCard+1).",".($sCard+2)."]";
		
		$query2 = lq_query_insert($data,DBManager::get()->getMT("stage"));
		if(mysql_query($query2,DBManager::get()->getMainConnection())){
			echo $data[no]."스테이지 추가 완료 $query2 \n";
			$cQuery = mysql_query("select * from ".DBManager::get()->getMT("card")." where `stage`=$i",DBManager::get()->getMainConnection());
			while($cData = mysql_fetch_array($cQuery)){
				for($j=0;$j<count($cData);$j++){
						unset($cData[$j]);
				}
				
				$cData[stage]=$stageNo;
				$cData[no]=$stageNo*10+$cData[rank]-1;
				$query3 = lq_query_insert($cData,DBManager::get()->getMT("card"));
				if(mysql_query($query3,DBManager::get()->getMainConnection())){
					echo $cData[no]."카드 추가 완료 $query3 \n";
					
				}else{
					echo $cData[no]."스테이지 추가 실패 $query3 \n";
					
				}
				
			}

		
		}else{
			echo $data[no]."스테이지 추가 실패 $query2 \n";
		}
		
		
		
		
		$i++;
	}
	echo "</textarea>";
}

function showAutoBalanceUpdateMode($name,$count=3){
	$name=$name."_check";
	global $_GET;
	$value =  $_GET[$name];
	$nonestate = "";
	$modifystate ="";
	$addstate = "";
	if($value=="modify")$modifystate="checked";
	else if($value=="add")$addstate="checked";
	else $nonestate="checked";
	
	echo '<center>미적용<input type=radio name="'.$name.'" value="" '.$nonestate.'> 수정<input type=radio name="'.$name.'" value="modify" '.$modifystate.'>';
	if($count==3)echo ' 추가<input type=radio name="'.$name.'" value="add" '.$addstate.'>';
	echo "</center>";
}
?>


<br>

<form action=admin_util.php>
	<input name="gid" value="<?=$gid?>" type="hidden">
<input type=hidden name=mode value=autoBalance>
<b><font color=red>슈퍼오토벨런싱</font></b>
<br>
적용조건 --> <input type=text name=puzzle size=3 value=<?=$_GET['puzzle']?>>번째 퍼즐 and 레벨 : <input type=text name=level size=3 value=<?=$_GET['level']?>>, 랜덤시드<input type=text name=rand_seed size=3 value=<?=$_GET['rand_seed']?>><br>

<br>
<table border=1>
<tr bgcolor=cccccc><td><?php echo showAutoBalanceUpdateMode("boss");?></td><td colspan=2>[보스]</td></tr>
<tr>
<td><?php echo showAutoBalanceUpdateMode("boss_type",2);?></td>
<td>보스</td><td><input type=text size=20 name=boss_type value=<?=$_GET[boss_type]?>><br> 중 선택 ( 따옴표로 구분 ), 모양<input type=text size=20 name=boss_shape value=<?=$_GET[boss_shape]?>>(circle or snake)</td>
</tr>
<tr>
<td rowspan=3><?php echo showAutoBalanceUpdateMode("boss_pattern");?></td><td>미사일패턴</td><td><textarea cols=200 rows=5 name="boss_pattern1"><?=$_GET[boss_pattern1]?></textarea><br><input type=text size=3 name=boss_pattern1_count value=<?=$_GET[boss_pattern1_count]?>>개</td>
</tr>
<tr>
<td>특수패턴</td><td><textarea cols=200 rows=5 name="boss_pattern2"><?=$_GET[boss_pattern2]?></textarea><br><input type=text size=3 name=boss_pattern2_count value=<?=$_GET[boss_pattern2_count]?>>개</td>
</tr>
<tr>
<td>부수기패턴</td><td><textarea cols=200 rows=5 name="boss_pattern3"><?=$_GET[boss_pattern3]?></textarea><br><input type=text size=3 name=boss_pattern3_count value=<?=$_GET[boss_pattern3_count]?>>개</td>
</tr>
<tr>
<td><?php echo showAutoBalanceUpdateMode("boss_scale",2);?></td>
<td>스케일</td><td>최대<input type=text  size=3 name=boss_scale_max value=<?=$_GET[boss_scale_max]?>>~최소<input type=text size=3 name=boss_scale_min value=<?=$_GET[boss_scale_min]?>></td>
</tr>
<tr>
<td><?php echo showAutoBalanceUpdateMode("boss_movement",2);?></td>
<td>움직임</td><td><input type=text size=3 name=boss_movement value=<?=$_GET[boss_movement]?>> 중 선택 ( 따옴표로 구분 )</td>
</tr>
<?php
if($_GET["autoConfig"]=="true")$autoConfigChecked="checked";
?>
<tr><td colspan=3> 자동설정 <input type=checkbox name="autoConfig" value="true" <?=$autoConfigChecked?>> (아래속성들을 레벨에 따라 자동으로 설정합니다.)</td></tr>
<tr>
<td><?php echo showAutoBalanceUpdateMode("boss_hp",2);?></td>
<td>HP</td><td>최대<input type=text  size=3 name=boss_hp_max value=<?=$_GET[boss_hp_max]?>>~최소<input type=text size=3 name=boss_hp_min value=<?=$_GET[boss_hp_min]?>></td>
</tr>
<tr>
<td rowspan=2><?php echo showAutoBalanceUpdateMode("boss_speed",2);?></td>
<td>최대속도</td><td>최대<input type=text size=3 name=boss_maxspeed_max value=<?=$_GET[boss_maxspeed_max]?>>~최소<input type=text size=3 name=boss_maxspeed_min value=<?=$_GET[boss_maxspeed_min]?>></td>
</tr>
<tr>
<td>최저속도</td><td>최대속도 - <input type=text size=3 name=boss_minspeed value=<?=$_GET[boss_minspeed]?>></td>
</tr>
<tr>
<td><?php echo showAutoBalanceUpdateMode("boss_ai",2);?></td>
<td>AI</td><td>최대<input type=text  size=3 name=boss_ai_max value=<?=$_GET[boss_ai_max]?>>~최소<input type=text size=3 name=boss_ai_min value=<?=$_GET[boss_ai_min]?>></td>
</tr>
<tr>
<td><?php echo showAutoBalanceUpdateMode("boss_agi",2);?></td>
<td>민첩</td><td>최대<input type=text size=3 name=boss_agi_max value=<?=$_GET[boss_agi_max]?>>~최소<input type=text size=3 name=boss_agi_min value=<?=$_GET[boss_agi_min]?>></td>
</tr>
<tr>
<td><?php echo showAutoBalanceUpdateMode("boss_attackpercent",2);?></td>
<td>공격주기</td><td>최대<input type=text size=3 name=boss_attackpercent_max value=<?=$_GET[boss_attackpercent_max]?>>~최소<input type=text size=3 name=boss_attackpercent_min value=<?=$_GET[boss_attackpercent_min]?>></td>
</tr>

<tr bgcolor=cccccc>
<td colspan=3>[부하몹]</td>
</tr>
<tr>
<td rowspan=9><?php echo showAutoBalanceUpdateMode("jr_count",2);?></td>
<td>갯수</td><td><input type=text size=3 name=jr_count value=<?=$_GET[jr_count]?>> 한종류만씀<input type=checkbox value=copy name=jr_copy <?php if($_GET["jr_copy"]=="copy")echo"checked"; ?>></td>
</tr>
<tr>
<td>부하몹</td><td><input type=text size=20 name=jr_type value=<?=$_GET[jr_type]?>><br> 중 선택 ( 따옴표로 구분 )</td>
</tr>
<tr>
<td>스케일</td><td>최대<input type=text  size=3 name=jr_scale_max value=<?=$_GET[jr_scale_max]?>>~최소<input type=text size=3 name=jr_scale_min value=<?=$_GET[jr_scale_min]?>></td>
</tr>
<tr>
<td>움직임</td><td><input type=text size=3 name=jr_movement value=<?=$_GET[jr_movement]?>> 중 선택 ( 따옴표로 구분 )</td>
</tr>
<tr>
<td>hp</td><td>최대<input type=text size=3 name=jr_hp_max value=<?=$_GET[jr_hp_max]?>>~최소<input type=text size=3 name=jr_hp_min value=<?=$_GET[jr_hp_min]?>></td>
</tr>
<tr>
<td>최대속도</td>
<td>최대<input type=text size=3 name=jr_maxspeed_max value=<?=$_GET[jr_maxspeed_max]?>>~최소<input type=text size=3 name=jr_maxspeed_min value=<?=$_GET[jr_maxspeed_min]?>></td>
</tr>
<tr>
<td>최저속도</td><td>최대속도 - <input type=text size=3 name=jr_minspeed value=<?=$_GET[jr_minspeed]?>></td>
</tr>
<tr>
<td>AI</td><td>최대<input type=text  size=3 name=jr_ai_max value=<?=$_GET[jr_ai_max]?>>~최소<input type=text size=3 name=jr_ai_min value=<?=$_GET[jr_ai_min]?>></td>
</tr>
<tr>
<td>민첩</td><td>최대<input type=text size=3 name=jr_agi_max value=<?=$_GET[jr_agi_max]?>>~최소<input type=text size=3 name=jr_agi_min value=<?=$_GET[jr_agi_min]?>></td>
</tr>
<tr><td colspan=3 align=center>비번(asdf) <input type=text name=pass size=4> <input type=submit value=확인></td></tr>
</table>
<br>
</form>


<?php
	function commaRand($val,$count=1){
		if($count<=0)return null;
		
		$list = explode(",",$val);
		
		if(count($list)<=0 && $count==1)return 0;
		if(count($list)<=0)return array();
		if(count($list)<$count)$count = count($list);
		$k = array_rand($list,$count);
		
		if(is_array($k)){
			for($i=0;$i<count($k);$i++){
				$r[]=$list[$k[$i]];	
			}
		}else{
			$r[]=$list[$k];
		}
		
		if($count==1)return $r[0];
		
		return $r;
	}
	function jsonRand($jsonstr,$count=1){
		if($count<=0)return array();
		
		$list = json_decode($jsonstr,true);
		
		
		if(count($list)<=0 && $count==1)return 0;
		if(count($list)<=0)return array();
		if(count($list)<$count)$count = count($list);
		
		$k = array_rand($list,$count);
		
		if(is_array($k)){
			for($i=0;$i<count($k);$i++){
				$r[]=$list[$k[$i]];	
			}			
		}else{
			$r[]=$list[$k];
		}
		
		if($count==1)return $r;
		
		return $r;
	}
	function plusArray($array1,$array2){
		if(count($array2)<=0 && count($array1)<=0)return array();
		if(count($array1)<=0)return $array2;
		if(count($array2)<=0)return $array1;
		
		for($i=0;$i<count($array2);$i++){
			$array1[] = $array2[$i];
		}
		return array_filter($array1);
	}
	function minmaxRand($min,$max){
		if($min==$max)return $min;
		if($max<$min){
			$t = $min;
			$min=$max;
			$max=$t;
		}
		$t = ($max-$min)/10.0;
		return ($min+rand(0,10)*$t);
	}
	
	if($_GET['mode']=="autoBalance"){

		srand($_GET['rand_seed']);
		echo "<textarea cols=200 rows=5>";
		
		$w_puzzle=null;
		if(is_numeric($_GET['puzzle'])){
			$r = mysql_fetch_assoc(mysql_query("select * from ".DBManager::get()->getMT("puzzle")." where `order`=".$_GET['puzzle'],DBManager::get()->getMainConnection()));
			$w_puzzle = $r[no];
		}

		$w_level = $_GET['level'];
		if($w_puzzle && $w_level){
			$w_level_where = " and level=$w_level";
			if($w_level=="*")$w_level_where = "";
			if(!is_numeric($w_puzzle))$w_puzzle_where = "1=1";
			else $w_puzzle_where = "puzzle=".$w_puzzle;
			$result = mysql_query("select * from ".DBManager::get()->getMT("stage")." where ".$w_puzzle_where.$w_level_where,DBManager::get()->getMainConnection());
			
			while($stageInfo = mysql_fetch_array($result)){
				unset($uData);
				unset($boss);
				unset($jr);
				$stageLevel = $stageInfo[level];
				$boss_info = json_decode($stageInfo["boss"],true);
				/////////////////////////////////////////////////////////////////////////////////////////////////
				
				if($_GET[boss_type_check]=="modify"){
					$boss[0][type]=commaRand($_GET[boss_type],1);
					$boss[0][shape]=$_GET[boss_shape];
				}
				
				$boss_pattern1 = jsonRand($_GET[boss_pattern1],$_GET[boss_pattern1_count]);
				$boss_pattern2 = jsonRand($_GET[boss_pattern2],$_GET[boss_pattern2_count]);
				$boss_pattern3 = jsonRand($_GET[boss_pattern3],$_GET[boss_pattern3_count]);
				$newPattern = plusArray($boss_pattern1,$boss_pattern2);
				$newPattern = plusArray($newPattern,$boss_pattern3);				
				
				//패턴
				if($_GET[boss_pattern_check]=="modify"){
					$boss[0][pattern]=$newPattern;
				}else if($_GET[boss_pattern_check]=="add"){
					$boss[0][pattern]=plusArray($newPattern,$boss_info[0][pattern]);					
				}
				

				
				//스케일
				if($_GET[boss_scale_check]=="modify"){
					$boss[0][scale][max] = $_GET[boss_scale_max];
					$boss[0][scale][min] = $_GET[boss_scale_min];
					$boss[0][scale][start] = ($boss[0][scale][max]+$boss[0][scale][min])/2.0; 
				}
				
				//움직임
				if($_GET[boss_movement_check]=="modify"){
					$boss[0][movement][normal] = commaRand($_GET[boss_movement],1);
					$boss[0][movement][draw] = $boss[0][movement][normal];
				}
				
				
				
				
				
				if($_GET[autoConfig]!="true"){
				
					//공격주기
					if($_GET[boss_attackpercent_check]=="modify"){
						$boss[0][attackpercent] = minmaxRand($_GET[boss_attackpercent_max],$_GET[boss_attackpercent_min]);
					}
								
					//속도
					if($_GET[boss_speed_check]=="modify"){
						$boss[0][speed][max] = minmaxRand($_GET[boss_maxspeed_max],$_GET[boss_maxspeed_min]);
						$boss[0][speed][min] = $boss[0][speed][max] - $_GET[boss_minspeed];
						$boss[0][speed][start] = ($boss[0][speed][max]+$boss[0][speed][min])/2.0; 				
					}
					
					//hp
					if($_GET[boss_hp_check]=="modify"){
						$boss[0][hp] = minmaxRand($_GET[boss_hp_max],$_GET[boss_hp_min]);
					}
					
					//ai
					if($_GET[boss_ai_check]=="modify"){
						$boss[0][ai] = minmaxRand($_GET[boss_ai_max],$_GET[boss_ai_min]);
					}
					
					//민첩
					if($_GET[boss_agi_check]=="modify"){
						$boss[0][agi] = minmaxRand($_GET[boss_agi_max],$_GET[boss_agi_min]);
					}
				}else{
					//쥬니어갯수만큼 일단 뺌
					$stageLevel -= (int)($_GET[jr_count]*$stageLevel/5);
					

					$level_speed_max = 7; 
					$level_attack_max = 7;

					$vv1=1;$vv2=1;$vv3=1;
					
					$vv1 = (int)rand(1,$stageLevel);
					if($stageLevel-$vv1>1)$vv2 = (int)rand(1,$stageLevel-$vv1);
					$vv3 = (int)($stageLevel-$vv2-$vv1);
					if($vv3<1)$vv3=1;
					$vkey = array("attack","hp","agi");
					shuffle($vkey);
					
					${"level_".$vkey[0]}=$vv1;
					${"level_".$vkey[1]}=$vv2;
					${"level_".$vkey[2]}=$vv3;
					
					$level_speed = $level_hp;


					if($level_speed>$level_speed_max)$level_speed=$level_speed_max;
				
					if($level_attack>$level_attack_max){
						$level_attack=$level_attack_max;
						if(rand(0,1))$level_dex += $level_attack-$level_attack_max;
						else $level_hp += $level_attack-$level_attack_max;
					}
					
					//공격주기 범위는 0.2~0.4 , 10 단계로 나뉨, 1단계에 0.1+0.03*level
					/*$boss_attackpercent_max = 0.2+0.02*$level_attack;
					$boss_attackpercent_min = $boss_attackpercent_max * 0.5;*/

					$boss_attackpercent_max = 0.2+0.03*$level_attack;
					$boss_attackpercent_min = $boss_attackpercent_max * 0.5;
					
					if($boss_attackpercent_min<0.2)$boss_attackpercent_min=0.2;
					$boss[0][attackpercent] = minmaxRand($boss_attackpercent_max,$boss_attackpercent_min);
					
					
					//속도 범위는 1~2.5  10단계로 나뉨 0.5+0.15*level; 
					$boss_maxspeed_max = 0.9+0.15*$level_speed;
					if($boss_maxspeed_max>2)$boss_maxspeed_max=2;
					$boss_maxspeed_min = $boss_maxspeed_max * 0.5;
					if($boss_maxspeed_min<0.8)$boss_maxspeed_min=0.8;
					$boss[0][speed][max] = minmaxRand($boss_maxspeed_max,$boss_maxspeed_min);
					$boss[0][speed][min] = $boss[0][speed][max]*0.5;
					$boss[0][speed][start] = ($boss[0][speed][max]+$boss[0][speed][min])/2.0; 	
					
					//hp는 계속 늘어남 기본 1000 + 500*level
					$boss_hp_max = 1000+500*$level_hp;
					$boss_hp_min = $boss_hp_max*0.5;
					$boss[0][hp] = (int)minmaxRand($boss_hp_max,$boss_hp_min);
					
					//민첩은 계속 늘어남
					$boss_agi_max = 10+10*$level_agi;
					$boss_agi_min = $boss_agi_max*0.5;
					$boss[0][agi] = (int)minmaxRand($boss_agi_max,$boss_agi_min);
					
					$boss[0][ai] = (int)minmaxRand($stageLevel*9,$stageLevel*9*1/$level_speed);
					if($boss[0][ai]<$stageLevel*9/2)$boss[0][ai]=$stageLevel*9/2;
					if($boss[0][ai]>90)$boss[0][ai]=90;
					
				}
				
				
				
				$jr=array();
				
				if($_GET[jr_count]>0){
					for($ji = 0; $ji<$_GET[jr_count];$ji++){
						if($_GET["jr_copy"]!="copy" || $ji==0){
							$jr[$ji][type] = commaRand($_GET[jr_type],1);
							
							$jr[$ji][scale][max] = $_GET[jr_scale_max];
							$jr[$ji][scale][min] = $_GET[jr_scale_min];
							$jr[$ji][scale][start]= ($jr[$ji][scale][max]+$jr[$ji][scale][min])/2.0; 
							
							$jr[$ji][movement][normal] = commaRand($_GET[jr_movement],1);
							$jr[$ji][movement][draw] = $jr[$ji][movement][normal];
								
							if($_GET[autoConfig]!="true"){
								$jr[$ji][speed][max]= minmaxRand($_GET[jr_maxspeed_max],$_GET[jr_maxspeed_min]);
								$jr[$ji][speed][min] = $jr[$ji][speed][max] - $_GET[jr_minspeed];
								$jr[$ji][speed][start]= ($jr[$ji][speed][max]+$jr[$ji][speed][min])/2.0; 
								
								$jr[$ji][hp]=minmaxRand($_GET[jr_hp_max],$_GET[jr_hp_min]);
								$jr[$ji][ai] = minmaxRand($_GET[jr_ai_max],$_GET[jr_ai_min]);
								$jr[$ji][agi] = minmaxRand($_GET[jr_agi_max],$_GET[jr_agi_min]);
							}else{
								$jr[$ji][speed][max]= $boss[0][speed][max]*minmaxRand(1.1,0.5);
								if($jr[$ji][speed][max]<0.5)$jr[$ji][speed][max]=0.5;
								$jr[$ji][speed][min] = $jr[$ji][speed][max]*0.5; 
								$jr[$ji][speed][start]= ($jr[$ji][speed][max]+$jr[$ji][speed][min])/2.0; 
								
								$jr[$ji][hp]= (int)($boss[0][hp]*minmaxRand(0.3,0.1));
								$jr[$ji][ai] =(int)($boss[0][ai]*minmaxRand(1.0,0.5));
								$jr[$ji][agi] = (int)($boss[0][agi]*minmaxRand(1.0,0.7));
							}
						}else{
							$jr[$ji][type] = $jr[0][type];
							$jr[$ji][speed][max]= $jr[0][speed][max];
							$jr[$ji][speed][min] = $jr[0][speed][min];
							$jr[$ji][speed][start]= $jr[0][speed][start];
							$jr[$ji][scale][max] = $jr[0][scale][max];
							$jr[$ji][scale][min] = $jr[0][scale][min];
							$jr[$ji][scale][start]= $jr[0][scale][start];
							$jr[$ji][movement][normal] = $jr[0][movement][normal];
							$jr[$ji][movement][draw] = $jr[0][movement][draw];
							$jr[$ji][hp]=$jr[0][hp];
							$jr[$ji][ai] = $jr[0][ai];
							$jr[$ji][agi] = $jr[0][agi];
						}
					}
				}
				
				
				/////////////////////////////////////////////////////////////////////////////////////////////////
				if($_GET[boss_check]=="modify"){
					$bosss[0] = array_merge($boss_info[0],$boss[0]);
					$uData["boss"]=  json_encode($bosss,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);					
				}else if($_GET[boss_check]=="add"){
					$uData["boss"]=  json_encode($boss,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
				}
				
				if($_GET[jr_count]>0){
					$uData["junior"]=json_encode($jr,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
				}else{
					$uData["junior"]="";
				}
				$ns = $stageInfo[no];
				
				$query = lq_query_update($uData,DBManager::get()->getMT("stage"),", version=version+1 where no = $ns");
				
				if($_GET['pass'] == "asdf"){
					kvManager::increase("stageVer_".$ns);
					mysql_query($query,DBManager::get()->getMainConnection());
					echo"적용됨\n";
				}else{
					echo"적용은 안됨\n";
				}
				
				if($_GET[autoConfig]=="true")echo"autoConfig at: $level_attack hp: $level_hp agi: $level_agi \n";
				echo $query."\n\n";
				
			}
			
			if($_GET['pass'] == "asdf"){
				kvManager::increase("puzzleVer_".$w_puzzle);
			}
		}else{
			echo"set the puzzle and level";
		}
		
		
		echo "</textarea><br>";
	}
	
	

	/*
if($_GET["mode"]=="imagechange"){
	
	echo "<textarea>";
		$result = mysql_query("select * from ".DBManager::get()->getMT("stage"));
		
		while($cdata = mysql_fetch_array($result)){
			

			
			unset($aa);
			unset($sd);
			$aa = json_decode($cdata["thumbnail"],true);
			if($aa[image]){
				$aa[image] = str_replace("piece", "thumbnail", $aa[image]);
				echo $aa[image];
				$sd[thumbnail]=json_encode($aa,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
				$sd[thumbnail] = str_replace("\\","",$sd[thumbnail]);
				$nq = lq_query_update($sd,DBManager::get()->getMT("stage"),"where no = ".$cdata[no]);
				echo $nq;
				mysql_query($nq);
			}	
					
		}
	echo "</textarea>";
	}
*/
	
?>



<br><br>

<form action=admin_util.php><input name="gid" value="<?=$gid?>" type="hidden">
<input type=hidden name=mode value=autoMissile>
<b><font color=red>카드미사일속성 세팅기</font></b>

비번(asdf) <input type=text name=pass size=4> <input type=submit value=확인>
</form>
<?php
if($_GET['mode']=="autoMissile"){
	srand(1);
	echo "<textarea cols=200 rows=5>";
	$result = mysql_query("select * from ".DBManager::get()->getMT("card")." where no<100000",DBManager::get()->getMainConnection());
	$alpabat = array("A","B","C","D","E","F","G","H","I","J","K","L","m","n","o");
	$oldstage=0;
	while($cardInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
		
		
		//카드 속성정하기~~
		if($oldstage!=$cardInfo[stage]){
			$stageInfo = mysql_fetch_array(mysql_query("select * from ".DBManager::get()->getMT("stage")." where no = $cardInfo[stage]",DBManager::get()->getMainConnection()),MYSQL_ASSOC);
			$stageLevel = $stageInfo[level];
			
		
			
			//몬스터가 뱀형이면 
			$arand = $alpabat[(int)rand(0,8)].$alpabat[(int)rand(0,6)];
		
			$boss=json_decode($stageInfo["boss"],true);
			$boss = $boss[0];
			
						
			//$prand=rand($stageLevel,$stageLevel*10);
			//$drand=rand($stageLevel,$stageLevel+$stageLevel*10-$prand);
			
			$prand = $boss[hp]/20;
			$drand = $boss[agi];
			
			$speedmax = $stageLevel*3;
			if($speedmax>70)$speedmax=70;
			$srand=rand(20,$speedmax+20)/10.0;
			
			echo "speed : $stageLevel \n";
			
			$oldstage = $cardInfo[stage];
		}
		
		
		//카드 단계별 속성능력 up
		$prg=0.5;
		$drg=0.5;
		if($cardInfo[grade]==3){
			$prg = 1;
			$drg = 1;
		}else if($cardInfo[grade]==2){
			if(rand(0,1)){
				$prg = 0.8;
			}else{
				$drg = 0.8;
			}
		}
			
		
		$ms["type"]=$arand;
		
		
		$ms["power"]=(int)($prg*$prand);		
		
		
		$ms["dex"]=(int)($drg*$drand);		
		

		$ms["speed"]=$srand*0.5;
		
		if($ms["power"]<1)$ms["power"]=1;
		if($ms["dex"]<1)$ms["dex"]=1;
		if($ms["speed"]<1)$ms["speed"]=1;
		
		$uData["missile"]=json_encode($ms,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
		
		$query = lq_query_update($uData,DBManager::get()->getMT("card"),"where no = $cardInfo[no]");

			echo $query."\n\n";
		mysql_query($query,DBManager::get()->getMainConnection());
		
	}

	echo "</textarea>";

}
?>

</center>


<!--

	CCLog("%s", (filename+"_sildata.txt").c_str());
	string path = CCFileUtils::sharedFileUtils()->fullPathForFilename((filename+".txt").c_str());
	
	unsigned long buff = 0;
	unsigned char* pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "rt", &buff);
	
	string convert_data((char*)pBuffer);
	
	remove(convert_data.begin(), convert_data.end(), '\t');
	remove(convert_data.begin(), convert_data.end(), '\n');
	
	convert_data = convert_data.substr(0,convert_data.find(":::")); // ,buff);
	
	bool is_zero = true;
	int error_cnt = 0;
	int cmp_cnt = 0;
	string result_data = "";
	for(int i=0;i<convert_data.size();i++)
	{
		if(is_zero)
		{
			if(convert_data[i] == '0')
			{
				cmp_cnt++;
			}
			else if(convert_data[i] == '1')
			{
				if(cmp_cnt >= 10000)		result_data.append("D");
				else if(cmp_cnt >= 1000)	result_data.append("C");
				else if(cmp_cnt >= 100)		result_data.append("B");
				else if(cmp_cnt >= 10)		result_data.append("A");
				
				ostringstream oss;
				oss<<cmp_cnt;
				
				result_data.append(oss.str().c_str());
				cmp_cnt = 1;
				is_zero = false;
			}
			else
			{
//				CCLog("invalid data %s", convert_data.substr(i,1).c_str());
				if(error_cnt == 0)
					CCLog("error data : %d", convert_data[i]);
				error_cnt++;
			}
		}
		else
		{
			if(convert_data[i] == '1')
			{
				cmp_cnt++;
			}
			else if(convert_data[i] == '0')
			{
				if(cmp_cnt >= 10000)		result_data.append("D");
				else if(cmp_cnt >= 1000)	result_data.append("C");
				else if(cmp_cnt >= 100)		result_data.append("B");
				else if(cmp_cnt >= 10)		result_data.append("A");
				
				ostringstream oss;
				oss<<cmp_cnt;
				
				result_data.append(oss.str().c_str());
				cmp_cnt = 1;
				is_zero = true;
			}
			else
			{
//				CCLog("invalid data %s", convert_data.substr(i,1).c_str());
				if(error_cnt == 0)
					CCLog("error data : %d", convert_data[i]);
				error_cnt++;
			}
		}
	}
	
	CCLog("error cnt : %d", error_cnt);
	
	if(cmp_cnt >= 10000)		result_data.append("D");
	else if(cmp_cnt >= 1000)	result_data.append("C");
	else if(cmp_cnt >= 100)		result_data.append("B");
	else if(cmp_cnt >= 10)		result_data.append("A");
	
	ostringstream oss;
	oss<<cmp_cnt;
	
	result_data.append(oss.str().c_str());
	
	CCLog("result size : %d", (int)result_data.size());
	CCLog("%s", result_data.c_str());
	CCLog("!!!");

-->

<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<?php
include "footer.php";
?>