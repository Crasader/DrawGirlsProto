<?php

include "../lib.php";

if(!$_GET[mode])exit;
?>


<table width=1024><tr><td>
<?php
//아이템 세팅하기

$itemlistStr = '[{"type":10,"currency":"gold","price":1000,"option":{"sec":30}},{"type":4,"currency":"gold","price":1000,"option":{"sec":3}},{"type":5,"currency":"gold","price":1000,"option":{}},{"type":6,"currency":"gold","price":1000,"option":{}},{"type":7,"currency":"gold","price":1000,"option":{"percent":1}},{"type":8,"currency":"gold","price":1000,"option":{"sec":20}},{"type":9,"currency":"gold","price":1000,"option":{}}]';
$gitemlistStr = '[{"type":11,"currency":"ruby","price":1,"option":{"percent":30}},{"type":12,"currency":"ruby","price":1,"option":{"percent":20}},{"type":13,"currency":"ruby","price":1,"option":{"percent":5}},{"type":14,"currency":"ruby","price":1,"option":{"percent":2}},{"type":15,"currency":"ruby","price":1,"option":{}}]';
$sitemStr = '{"type":16,"currency":"social","price":10,"option":{}}';
$itemList = json_decode($itemlistStr,true);
$gitemList = json_decode($gitemlistStr,true);
$sitem = json_decode($sitemStr,true);

$shopItems[] = json_decode('{"type":16,"currency":"gold","price":100,"option":{}}',true);


$result = mysql_query("select * from $TABLE_STAGE where level>=5",DBManager::get()->getMainConnection());
srand(1);
while($sData = mysql_fetch_assoc($result)){
	
	shuffle($itemList);
	shuffle($gitemList);
	
	$newitem = array_slice($itemList, 0,6);
	$mission="0";
	if(rand(0,7)==6 && $sData[level]>=7){
		$gi = $gitemList[0];
		$newitem[]=$gi;
		if($gi["type"]==10)$mission=7;
		if($gi["type"]==11)$mission=1;
		if($gi["type"]==12)$mission=2;
		if($gi["type"]==13)$mission=3;
		if($gi["type"]==14)$mission=5;
		if($gi["type"]==15)$mission=6;

		echo "<font color=red>mission</font>";
	}else{
		$newitem[] = $itemList[6];
	}
	$newitem[]=$sitem;

	$shopItemStr = json_encode($newitem,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

	$missionStr = '{"type":'.$mission.',"option":[]}';
	echo ("update $TABLE_STAGE set shopItems='".$shopItemStr."',mission='".$missionStr."' where no = ".$sData[no]); //,DBManager::get()->getMainConnection()
	mysql_query("update $TABLE_STAGE set shopItems='".$shopItemStr."',mission='".$missionStr."' where no = ".$sData[no],DBManager::get()->getMainConnection()); 
	echo mysql_error();
	echo "<br><br>";


	kvManager::increase("stageVer_".$sData[no]);
	kvManager::increase("puzzleVer_".$sData[puzzle]);
}

//2레벨 아이템3개 , 3레벨 아이템 4개 , 4레벨 아이템 5개 배치 랜덤으로 ,itemlistStr 만 이용하여 배치하기

$result = mysql_query("select * from $TABLE_STAGE where level>1 and level<5",DBManager::get()->getMainConnection());

while($sData = mysql_fetch_assoc($result)){
	shuffle($itemList);
	$newitem = array_slice($itemList, 0,$sData[level]+1);
	$shopItemStr = json_encode($newitem,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

	$missionStr = '{"type":0,"option":[]}';
	echo ("update $TABLE_STAGE set shopItems='".$shopItemStr."',mission='".$missionStr."' where no = ".$sData[no]); //,DBManager::get()->getMainConnection()
	mysql_query("update $TABLE_STAGE set shopItems='".$shopItemStr."',mission='".$missionStr."' where no = ".$sData[no],DBManager::get()->getMainConnection()); 
	echo mysql_error();
	echo "<br><br>";
	kvManager::increase("stageVer_".$sData[no]);
	kvManager::increase("puzzleVer_".$sData[puzzle]);
}



//퍼즐별 카드번호 자동세팅하기 
/*
$puzzleNo = 6;
$cardHeader = 11;

$result = mysql_query("select * from $TABLE_STAGE where puzzle=$puzzleNo",DBManager::get()->getMainConnection());
echo mysql_error();
$i=0;
while($stageInfo = mysql_fetch_array($result,MYSQL_ASSOC)){
	
	echo "abc";
	$zero = "0";
	if($i>10)$zero="";
	$mycard = $cardHeader*1000+$i*10;
	$cardtext = "[".($mycard+1).",".($mycard+2).",".($mycard+3)."]";
	//echo("update $TABLE_STAGE set cards = '".$cardtext."' where no=".$stageInfo[no]);
	mysql_query("update $TABLE_STAGE set cards = '".$cardtext."' where no=".$stageInfo[no],DBManager::get()->getMainConnection());

	echo "<br>";
	$i++;
}
*/
//카드파일명으로 카드자동생성하기

// $startCardNo = 11000;

// $oldCardID=0;
// $realDir = "../images/cards";
// $i=0;
// $t=($startCardNo/1000)-1;
// $files = filesInDir ($realDir);

// $fileText = "/Volumes/PublicHDD/Galsprototype/c_cf_01_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_01_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_01_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_01_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_01_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_02_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_03_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_04_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_05_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_06_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_07_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_08_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_09_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_10_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_11_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_12_3.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_1.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_1s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_2.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_2s.png
// /Volumes/PublicHDD/Galsprototype/c_cf_13_3.png";

// $files = explode("\n",$fileText);

// sort($files);
// foreach ( $files as $key => $a_file){ 

// 	$filepath = explode("/",$a_file);
// 	$filename = $filepath[count($filepath)-1];
// 	$filename = explode(".",$filename);
// 	$filename = $filename[0];

// 	$fileinfo = explode("_",$filename);
// 	$type = $fileinfo[1];
// 	$typeNo = $fileinfo[2];
// 	$rank = $fileinfo[3];
// 	$isSil = true;
// 	if(is_numeric($rank))$isSil=false;

// 	$newCardID = $type.$typeNo;
// 	$newTheme = $type;


// 	if($i!=0 && $oldCardID==$newCardID){
// 		$oldCardID = $newCardID;
// 		continue;
// 	}


// 	if($newTheme!=$oldTheme)$t++;
// 	$i++;

// 	$filepath_head = "../images/";
// 	$filename_head = "cards/"."c_".$type."_".$typeNo."_";

// 	$card1 = $filename_head."1.png";
// 	$card1s = $filename_head."1s.png";
// 	$card2 = $filename_head."2.png";
// 	$card2s = $filename_head."2s.png";
// 	$card3 = $filename_head."3.png";
// 	$cardNo = ($typeNo-1)*10+1000*$t;
	
// 	$data1=array();
// 	$data1[no]=$cardNo+1;
// 	$data1[rank]=1;
// 	$data1[reward]=500;
// 	$data1[durability]=5;
// 	$data1[language]="kr";
// 	$data1[imgInfo]='{"img":"'.$card1.'","size":'.(int)(filesize($filepath_head.$card1)/1000).'}';
// 	$data1[silImgInfo]='{"isSil":"1","img":"'.$card1s.'","size":'.(int)(filesize($filepath_head.$card1s)/1000).'}';

// 	$data2=array();
// 	$data2[no]=$cardNo+2;
// 	$data2[rank]=2;
// 	$data2[reward]=1000;
// 	$data2[durability]=5;
// 	$data2[language]="kr";
// 	$data2[imgInfo]='{"img":"'.$card2.'","size":'.(int)(filesize($filepath_head.$card2)/1000).'}';
// 	$data2[silImgInfo]='{"isSil":"1","img":"'.$card2s.'","size":'.(int)(filesize($filepath_head.$card2s)/1000).'}';

// 	$data3=array();
// 	$data3[no]=$cardNo+3;
// 	$data3[rank]=3;
// 	$data3[reward]=2000;
// 	$data3[durability]=5;
// 	$data3[language]="kr";
// 	$data3[imgInfo]='{"img":"'.$card3.'","size":'.(int)(filesize($filepath_head.$card3)/1000).'}';
// 	$data3[silImgInfo]='{"isSil":"0"}';
	
// 	$query1 = lq_query_insert($data1,$TABLE_CARD);
// 	$query2 = lq_query_insert($data2,$TABLE_CARD);
// 	$query3 = lq_query_insert($data3,$TABLE_CARD);

// 	mysql_query($query1,DBManager::get()->getMainConnection());
// 	mysql_query($query2,DBManager::get()->getMainConnection());
// 	mysql_query($query3,DBManager::get()->getMainConnection());

// 	echo $query1;
// 	echo $query2;
// 	echo $query3;
	
// 	echo $t."->".$i."->".$cardNo."->".$filename."->".$newCardID."<br>"; 

// 	$oldCardID = $newCardID;
// 	$oldTheme=$type;
// } 


/*

//몬스터 세팅~




$res["boss_metal1"]["circle"][]="circle_01";
$res["boss_metal1"]["circle"][]="circle_02";
$res["boss_metal1"]["circle"][]="circle_03";
$res["boss_metal1"]["circle"][]="circle_04";
$res["boss_metal1"]["circle"][]="circle_05";
$res["boss_metal1"]["circle"][]="circle_06";
$res["boss_metal1"]["circle"][]="circle_07";
$res["boss_metal1"]["circle"][]="circle_08";
$res["boss_metal1"]["circle"][]="circle_09";
$res["boss_metal1"]["circle"][]="circle_10";
$res["boss_metal1"]["circle"][]="circle_11";
$res["boss_metal1"]["circle"][]="circle_12";
$res["boss_metal1"]["circle"][]="circle_13";




$res["boss_metal2"]["circle"][]="circle_14";
$res["boss_metal2"]["circle"][]="circle_15";
$res["boss_metal2"]["circle"][]="circle_16";
$res["boss_metal2"]["circle"][]="circle_17";
$res["boss_metal2"]["circle"][]="circle_18";
$res["boss_metal2"]["circle"][]="circle_19";
$res["boss_metal2"]["circle"][]="circle_20";
$res["boss_metal2"]["circle"][]="circle_21";
$res["boss_metal2"]["circle"][]="circle_22";
$res["boss_metal2"]["circle"][]="circle_23";
$res["boss_metal2"]["circle"][]="circle_24";

$res["boss_metal1"]["jr"][]="jr_01";
$res["boss_metal1"]["jr"][]="jr_02";
$res["boss_metal1"]["jr"][]="jr_03";
$res["boss_metal1"]["jr"][]="jr_04";
$res["boss_metal1"]["jr"][]="jr_05";
$res["boss_metal1"]["jr"][]="jr_06";
$res["boss_metal1"]["jr"][]="jr_07";
$res["boss_metal1"]["jr"][]="jr_08";



$res["boss_metal2"]["jr"][]="jr_09";
$res["boss_metal2"]["jr"][]="jr_10";

$res["boss_metal1"]["snake"][]="snake_01";
$res["boss_metal1"]["snake"][]="snake_02";
$res["boss_metal1"]["snake"][]="snake_03";
$res["boss_metal1"]["snake"][]="snake_04";
$res["boss_metal1"]["snake"][]="snake_05";


$res["boss_metal2"]["snake"][]="snake_06";
$res["boss_metal2"]["snake"][]="snake_07";
$res["boss_metal2"]["snake"][]="snake_08";
$res["boss_metal2"]["snake"][]="snake_09";
$res["boss_metal2"]["snake"][]="snake_10";

foreach ($res as $plistName => $value){
	foreach ($value as $mobType => $filelist) {
		foreach ($filelist as $no => $filename){
			echo $plistName."->".$mobType."->".$filename."<br>";
			$mobinfo["ccbiID"]=$filename;
			$mobinfo["ccbi"]="resource/".$filename;
			$mobinfo["imageID"]=$plistName;
			$mobinfo["plist"]="resource/".$plistName.".plist";
			$mobinfo["pvrccz"]="resource/".$plistName.".pvr.ccz";
			$mobinfo["size"]=(int)(filesize("../".$mobinfo["pvrccz"])/1000);
			$mobinfo["type"]=$mobType;
			$rInfo =  json_encode($mobinfo,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK); 
			echo("insert into $TABLE_MONSTER ('name','resourceInfo','type') values ('".$filename."','".$rInfo."','".$mobType."') <br>");
			mysql_query("insert into $TABLE_MONSTER (`name`,`resourceInfo`,`type`) values ('".$filename."','".$rInfo."','".$mobType."')",DBManager::get()->getMainConnection());
			echo mysql_error();
			echo "ok<br><br>";
		}
	}
}
*/
DBManager::get()->closeDB();

?>

</td></tr></table>

