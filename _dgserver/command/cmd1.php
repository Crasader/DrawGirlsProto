<?php



for($c=0;$c<count($param);$c++){
	$cmd = (string)$c;
	$p = $param[$cmd]["p"];
	$a = strtolower($param[$cmd]["a"]);
	
	//////////////////////////////////////////////////////////////////////////////////////////
	// getStageInfo
	//////////////////////////////////////////////////////////////////////////////////////////
	
	if($a == "getstageinfo"){
		$stageInfo =  mysql_fetch_array(mysql_query("select * from ".DBManager::get()->getMT("stage")." where no=$p[no]"));
		for($i=0;$i<count($stageInfo);$i++){
					unset($stageInfo[$i]);
		}
		
		
		if($p[version]<$stageInfo[version] || $p[version]==-1){
		
			$stageInfo[no]=(int)$stageInfo[no];
			$stageInfo[version]=(int)$stageInfo[version];
			$stageInfo[theme]=(int)$stageInfo[theme];
			$stageInfo[playtime]=(int)$stageInfo[playtime];
			$stageInfo[shopItems]=json_decode($stageInfo[shopItems],true);
			$stageInfo[defItems]=json_decode($stageInfo[defItems],true);
			$stageInfo[cards]=json_decode($stageInfo[cards],true);
			$stageInfo[boss]=$stageInfo[boss];
			$stageInfo[junior]=$stageInfo[junior];
			$stageInfo[mission]=json_decode($stageInfo[mission],true);
			$stageInfo[thumbnail]=json_decode($stageInfo[thumbnail],true);
		
			$card = array();
			for($i=0;$i<count($stageInfo[cards]);$i++){
				$cardInfo = mysql_fetch_array(mysql_query("select * from ".DBManager::get()->getMT("card")." where no=".$stageInfo[cards][$i]));
				
				for($j=0;$j<count($cardInfo);$j++){
						unset($cardInfo[$j]);
				}
				$cardInfo[ability]=json_decode($cardInfo[ability],true);
				$cardInfo[aniInfo]=json_decode($cardInfo[aniInfo],true);
				$cardInfo[imgInfo]=json_decode($cardInfo[imgInfo],true);
				$cardInfo[thumbnailInfo]=json_decode($cardInfo[thumbnailInfo],true);
				$cardInfo[silImgInfo]=json_decode($cardInfo[silImgInfo],true);
				
				
				$cardInfo[no]=(int)$cardInfo[no];
				$cardInfo[stage]=(int)$cardInfo[stage];
				$cardInfo[rank]=(int)$cardInfo[rank];
				$cardInfo[aniInfo][isAni]=(bool)$cardInfo[aniInfo][isAni];
				$cardInfo[aniInfo][detail][size]=(int)$cardInfo[aniInfo][detail][size];
				$cardInfo[silImgInfo][isSil]=(bool)$cardInfo[silImgInfo][isSil];
				$cardInfo[thumbnailInfo][img]=$nowurl."/images/".$cardInfo[thumbnailInfo][img];
				$cardInfo[imgInfo][img]=$nowurl."/images/".$cardInfo[imgInfo][img];
				$cardInfo[aniInfo][detail][img]=$nowurl."/images/".$cardInfo[aniInfo][detail][img];
				$cardInfo[silImgInfo][img]=$nowurl."/images/".$cardInfo[silImgInfo][img];
				$cardInfo[durability]=(int)$cardInfo[durability];
				$card[]=$cardInfo;
				
			}
			
			
			$stageInfo[cards]=$card;
			$stageInfo[state]="ok";
			$allResult[$cmd]=$stageInfo;
		
		}else{
			$rData[version]=$p[version];
			$rData[state]="ok";
			$allResult[$cmd]=$rData;
		}
		
		
		
	//////////////////////////////////////////////////////////////////////////////////////////
	// getStageInfo
	//////////////////////////////////////////////////////////////////////////////////////////
	
	}else if($a=="getstagelist"){
		//$theme=$p["theme"];	//받아올 시작번호
		/*
			{
"allStage":100,
"stageList":[
			{"no":1,"thumbnail":"url","x":11,"y":33}
]
		*/
		//혹은 받아올 테마
		
		//where theme=$theme
		$query = mysql_query("select no,thumbnail from ".DBManager::get()->getMT("stage"));
		
		$list="";
		$cnt=0;
		while($stageList = mysql_fetch_array($query)){
			for($j=0;$j<count($stageList);$j++){
					unset($stageList[$j]);
			}
			
			$stageList[no]=(int)$stageList[no];
			$stageList[thumbnail]=json_decode($stageList[thumbnail],true);
			$stageList[thumbnail][image]=$nowurl."/images/".$stageList[thumbnail][image];	
			$list[]=$stageList;
			$cnt++;
		}
		
		$result[theme]=$theme;
		$result[count]=$cnt;
		$result["list"]=$list;
		$result["version"]=10;
		$result["state"]="ok";
		$allResult[$cmd]=$result;
		
	}
	
}
$allResult[state]="ok";
$allResult[timestamp]=time();

$allResult = json_encode($allResult,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

if($mode=="nodes"){
echo $allResult;
}else{
echo base64_encode(@mcrypt_encrypt(MCRYPT_DES, $SECRETKEY,   $allResult, MCRYPT_MODE_ECB))."#";	
	
}



?>