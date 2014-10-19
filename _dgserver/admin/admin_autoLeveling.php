<?php
include "header.php";


exit;
?>


<form action="admin_autoLeveling.php" method="get">
<input type=hidden name="gid" value=<?=$gid?>>
	passwd <input type=password name="passwd">
	<input type=submit value="start Leveling">
</from>

<br>
<?php

if($_GET["passwd"]=="1234"){
	srand(1);
	function makePattern($patternInfo,$vlevel,$type,$patternNo=null){
		static $patternLevel=array();
		while(1){
			$pattern = new Pattern($patternNo,$type);
			$check=false;
			foreach ($patternInfo as $key => $value) {
				if($value["pattern"]==$pattern->no)$check=true;
			}
			if($check==false)break;
		}

		$id = "pattern".$pattern->type;
		$level = $patternLevel[$id];
		if(!array_key_exists($id,$patternLevel))$level=0;
		else $level++;
		$patternLevel[$id]=$level;
		

		$pInfo =& $pattern->getRef("template");
		$patternProp =& $pattern->getRef("propInfo");
		
		if($patternProp){
			foreach ($patternProp as $key2 => $value2) {
				
				if($pattern->category=="missile")$lev = $vlevel;
				else $lev = $level;

				$v=$value2["min"]+$value2["variation"]*$lev;
					
				if($value2["variation"]>0){
					if($value2["max"]<$v)$v = $value2["max"];
				}else{
					if($value2["max"]>$v)$v = $value2["max"];
				}
				$pInfo[$value2["propName"]]=$v;	
			}
		}
		//$pInfo["level"]=$level;
		if($pattern->category=="missile")$pInfo["percent"]=3;
		if($pattern->category=="special")$pInfo["percent"]=3;
		if($pattern->category=="critical")$pInfo["percent"]=3;
		if($pattern->category=="crash")$pInfo["percent"]=2;

		$pInfo["category"]=$pattern->category;
		$pInfo["level"]=$level;
		$pInfo["pattern"]=$pattern->type;
		return $pInfo;
	}
	
	function getBoss($monster,$level){

		$patternCate= array("missile"=>2,"special"=>1,"critical"=>1,"crash"=>1);
		
		if($level==2){
			$patternCate["missile"]=1;
			$patternCate["special"]=0;
			$patternCate["critical"]=0;
			$patternCate["crash"]=0;
		}else if($level<=2){
			$patternCate["missile"]=0;
			$patternCate["special"]=0;
			$patternCate["critical"]=0;
			$patternCate["crash"]=0;
		}else if($level<=3){
			$patternCate["missile"]=1;
			$patternCate["special"]=0;
			$patternCate["critical"]=0;
			$patternCate["crash"]=0;
		}else if($level<=4){
			$patternCate["missile"]=1;
			$patternCate["special"]=1;
			$patternCate["critical"]=0;
			$patternCate["crash"]=0;
		}



		if($level<5){
			$patternCate["critical"]=0;
			$patternCate["crash"]=0;
		}

		if($level>10)$patternCate["crash"]=2;

		$monsterInfo = array();
		
		$monsterProp =& $monster->getRef("propInfo");

		$monsterInfo["shape"]=$monster->type;
		$monsterInfo["type"]=$monster->name;
		$monsterInfo["movement"]=array("normal"=>$monsterProp[movement],"draw"=>$monsterProp[movement]);
		$monsterInfo["isflipx"]=$monsterProp[isflipx];

		$monsterInfo["hp"]=$monsterProp[hp]*ceil(1000+100*$level);
		$monsterInfo["ai"]=$monsterProp[ai]*5*$level;
		if($monsterInfo["ai"]>70)$monsterInfo["ai"]=70;
		$monsterInfo["agi"]=10*$level;

		$monsterInfo["attackpercent"]=0.005*$level+0.18;
		if($monsterInfo["attackpercent"]>0.3)$monsterInfo["attackpercent"]=0.3;

		$maxSpeed = (1.3-0.3)/(float)20.0*$level+0.4;
		if($maxSpeed>1)$maxSpeed=1;
		$monsterInfo["speed"]=array("min"=>$maxSpeed/(float)2,"start"=>$maxSpeed,"max"=>$maxSpeed);  
		$monsterInfo["scale"]=array("min"=>0.4,"start"=>0.4,"max"=>0.4);

		$monsterInfo["pattern"]=array();

		$patternInfo =& $monster->getRef("patternInfo");		
		
		//지정패턴입력
		foreach ($patternInfo as $key => $value) {
			echo "<font color=green>$value</font>";
			$pInfo = makePattern($monsterInfo["pattern"],$level,null,$value);
			if($pInfo){
				$monsterInfo["pattern"][]=$pInfo;
				$patternCate[$pInfo["category"]]--;
			}
			
		}

		//if($level>10)$patternCate["crash"]++;

		//미사일채우기
		if($patternCate["missile"]>0){
			for($i=1;$i<=$patternCate["missile"];$i++){
				$monsterInfo["pattern"][]=makePattern($monsterInfo["pattern"],$level,"missile");
			}
		}

		//스페셜채우기
		if($patternCate["special"]>0){
			for($i=1;$i<=$patternCate["special"];$i++){
				$monsterInfo["pattern"][]=makePattern($monsterInfo["pattern"],$level,"special");
			}
		}

		//크리티컬채우기
		if($patternCate["critical"]>0){
			for($i=1;$i<=$patternCate["critical"];$i++){
				$monsterInfo["pattern"][]=makePattern($monsterInfo["pattern"],$level,"critical");
			}
		}


		//부수기채우기
		if($patternCate["crash"]>0){
			for($i=1;$i<=$patternCate["crash"];$i++){
				$monsterInfo["pattern"][]=makePattern($monsterInfo["pattern"],$level,"crash");
			}
		}



		return $monsterInfo;

	};

	function getJunior($bossInfo,$junior,$puzzleNo){
		$jrInfo=array();
		$jrInfo["type"]=$junior->name;


		$jrInfo["movement"]=$bossInfo[movement];
		$jrInfo["isflipx"]=$bossInfo[isflipx];

		$jrInfo["hp"]=$bossInfo[hp]*0.25;
		$jrInfo["ai"]=$bossInfo[ai]*0.5;
		$jrInfo["agi"]=$bossInfo[agi]*0.5;

		$jrInfo["speed"]=array("min"=>$bossInfo[speed][min]*0.75,"start"=>$bossInfo[speed][start]*0.75,"max"=>$bossInfo[speed][max]*0.75);  
		$jrInfo["scale"]=array("min"=>0.4,"start"=>0.4,"max"=>0.4);

		return $jrInfo;
	}

	function getMission($level,$pieceNo){

		$level--;

		if($level<3)return array("type"=>0);
		if($pieceNo%5==1)return array("type"=>0);
		else if($level<10){
			if(rand(1,4)==1)return array("type"=>0);
		}

		$level-=3;
		if($level<0)$level=0;

		$mission = new Mission();
		
		$missionInfo = array();

		$missionInfo["type"]=$mission->type;

		$levelInfo =& $mission->getRef("levelInfo");
		
		$value = $levelInfo["min"] + $levelInfo["variation"]*$level;
			
		if($levelInfo["variation"]>0){
			if($levelInfo["max"]<$value)$value=$levelInfo["max"];
		}else{
			if($levelInfo["max"]>$value)$value=$levelInfo["max"];
		}

		$missionInfo["option"]=array($levelInfo["propName"]=>$value);
		
		return $missionInfo;
	}

	$level=1;
	while($puzzle = Puzzle::getObjectByQuery("where isEvent=0 order by no asc")){
		
		$monster = new Monster(null,$puzzle->no,1);
		$junior = new Monster(null,$puzzle->no,0);
		echo "<font color=red><b> START ".$puzzle->no." PUZZLE</b><br>";
		echo json_encode($monster->getArrayData(true))."</font><br>";
		$pLevel=0;
		while($piece = Piece::getObjectByQuery("where puzzle=".$puzzle->order." order by no asc")){
			if(!$piece->autoLevel){
				echo "<font color=red>DONT AUTO LEVEL ".$piece->no." PIECE</font><br><br><br>";
				continue;
			}
			
			$apLevel = (int)($level+$pLevel);
			echo"<font color=blue> START AUTO LEVEL ".$piece->no." PIECE ".$apLevel." Level </font><br>";
			$boss =& $piece->getRef("boss");
			$boss=array();
			$boss[0]=getBoss($monster,(int)($apLevel));
			$pLevel+=0.6;
			$piece->mission = getMission($apLevel,$piece->no);
			$piece->level = $apLevel;
			$jr=&$piece->getRef("junior");
			$jr=array();
			if($apLevel>2){
				$jrCnt = (int)($apLevel/(float)1.7);
				if($jrCnt>12)$jrCnt=12;
				if($apLevel>=3 && $jrCnt<4)$jrCnt=4;
				$startCnt = (int)($jrCnt/2);
				if($startCnt<1)$startCnt=1;
				for($i=0;$i<$startCnt;$i++){
					$jr[$i]=getJunior($boss[0],$junior,$puzzle->no);
				}
			}
			$piece->junior = $jr;

			
			if($apLevel>2){	
				//부하몹리젠
				$defaultPattern1 = json_decode('{"atype":"special","childs":2,"maxchilds":6,"percent":1,"pattern":1020}',true);
				$defaultPattern1["maxchilds"]=(int)$jrCnt;
				$boss[0]["pattern"][]=$defaultPattern1;
			}
			//순간이동채우기
			$defaultPattern2 = json_decode('{"atype":"special","target":"no","percent":1,"pattern":1007}',true);
			$boss[0]["pattern"][]=$defaultPattern2;

			echo json_encode($piece->getArrayData(true));
			echo "<br>";
			if($piece->save()){
				echo "<font color=orange>save SUCCESS</font>";
			}else{
				echo "<font color=orange>save FAIL</font>";
			}
			echo "<br><br><br>";

			unset($boss);
			unset($jr);
			
		}
		$level++;

		unset($junior);
		unset($monster);
	}




}


include "footer.php";
?>