<?php
include "header.php";


// while($piece = Piece::getObjectbyQuery()){
// 	$boss =& $piece->getRef("boss");
// 	if($piece->no<=100){
// 		$boss[0]["agi"]=$piece->no;	
		
// 		$jr =& $piece->getRef("junior");
// 		for($i=0;$i<count($jr);$i++){
// 			$jr[$i]["agi"]=$piece->no;
// 		}
// 	}else{
// 		$boss[0]["agi"]=100;	
// 		$jr =& $piece->getRef("junior");
// 		for($i=0;$i<count($jr);$i++){
// 			$jr[$i]["agi"]=100;
// 		}
// 	}


// 	echo json_encode($piece->boss)."<br><br>";
// 	echo json_encode($piece->junior)."<br><br>";
// 	$piece->save();
// }
// while($userindex = UserIndex::getRowByQuery()){
// 	$mID = $userindex["memberID"];
// 	$userData = new UserData($mID);
// 	$character = new CharacterHistory($mID,1);
//	if($userData->level==0){
// 	$charLevel = $character->level;

// 	$userData->level = $charLevel;
//	$character->level = 1;
// 	$character->save();
//	}
// }
// $enc = decryptByAESWithBase64("es1hmm2BeF8wfgnRfKN7D3zgX78TbuzEj4GjeKaQG6tqqxmeHmCbagw6OfUjLnROF4XhcTl8gertaZppPtb8FoNFk5dBCot7D81iEnWfiEd1OsX2mqM8aRsHsbR+DMl8YRkz+LgWpFS+rfyMSet2ntVF7Mu2UOb2DIm6wvJoF5fsjf4y3y3EOlRkyUziccR/6P1jAaLij8bYyFWu45tpDjSGZa+iQBw5cQqfIWUdwt4zhbMVFKWfrcE4dNGUNSQy6ZrWhiCOTUSSS57KLwCVONqskiJ23Dk2SqndpUFBD7RNKDq9MbZKH7MRQthfxmiG");

// echo $enc;


// exit;
// while($obj = Monster::getObjectByQuery("")){


// 	$script =& $obj->getRef("script");
// 	if($script["default"])$script["en"]=$script["default"];
// 	else if($script["ko"])$script["default"]=$script["ko"];
// 	else if($script["kr"])$script["default"]=$script["kr"];
// 	$script["en"]=$script["default"];
// 	$script["kr"]=$script["default"];



// 	$obj->save();

// }


// return;
// while($obj = Card::getObjectByQuery("")){


// 	$name =& $obj->getRef("name");
// 	if($name["default"])$name["en"]=$name["default"];
// 	else if($name["ko"])$name["default"]=$name["ko"];
// 	else if($name["kr"])$name["default"]=$name["kr"];
// 	$name["en"]=$name["default"];
// 	$name["kr"]=$name["default"];


// 	$script =& $obj->getRef("script");
// 	if($script["default"])$script["en"]=$script["default"];
// 	else if($script["ko"])$script["default"]=$script["ko"];
// 	else if($script["kr"])$script["default"]=$script["kr"];
// 	$script["en"]=$script["default"];
// 	$script["kr"]=$script["default"];


// 	$profile =& $obj->getRef("profile");
// 	if($profile["default"])$profile["en"]=$profile["default"];
// 	else if($profile["ko"])$profile["default"]=$profile["ko"];
// 	else if($profile["kr"])$profile["default"]=$profile["kr"];
// 	$profile["en"]=$profile["default"];
// 	$profile["kr"]=$profile["default"];


// 	$obj->save();

// }


// return;


// while($obj = Puzzle::getObjectByQuery("")){
// 	$title =& $obj->getRef("title");
	
// 	if($title["default"])$title["en"]=$title["default"];
// 	else if($title["en"])$title["default"]=$title["en"];

// 	$title["ko"]=$title["default"];
// 	$title["kr"]=$title["default"];

// 	$obj->save();

// }


// return;

// while($obj = Archivement::getObjectByQuery("")){
// 	$title =& $obj->getRef("title");
// 	$content =& $obj->getRef("content");
	
// 	if($title["default"])$title["en"]=$title["default"];
// 	else if($title["en"])$title["default"]=$title["en"];

// 	$title["ko"]=$title["default"];
// 	$title["kr"]=$title["default"];

// 	if($content["default"])$content["en"]=$content["default"];
// 	else if($content["en"])$content["default"]=$content["en"];

// 	$content["ko"]=$content["default"];
// 	$content["kr"]=$content["default"];

// 	$obj->save();
// }



/*
// exit;
$targetCardDir = "obt_card2";
$targetPuzzleDir = "obt_puzzle";
$targetPuzzleCardDir = "obt_puzzlecard";
$targetCardAniDir = "obt_ani2";

$realDir="../images/".$targetCardDir;

$i=0;
$cards=array();
foreach (filesInDir ($realDir) as $key => $a_file){ 
	$i++;
	$path = explode("/",$a_file);
	$fileIndex = count($path)-1;
	$fileName = $path[$fileIndex];
	$cardInfo = explode("_",$fileName);
	$puzzle = $cardInfo[0];
	$piece = $cardInfo[1];
	$gradeInfo = $cardInfo[2];
	$isAdult = false;
	if($cardInfo[3]=="a")$isAdult=true;

	$grade = substr($gradeInfo,0,1);
	$isSil = (strpos($gradeInfo,'s') !== false);
	echo $fileName."->";
	echo $puzzle."-";
	echo $piece."-";
	echo $grade."-";
	echo $isSil."-";

	if($isSil){
		$cards[$puzzle]["stage"][$piece][$grade]["sil"]["url"]="http://182.162.201.147:10010/images/".$targetCardDir."/".$fileName;
		$cards[$puzzle]["stage"][$piece][$grade]["sil"]["filename"]=$fileName;
	}else if($isAdult){
		$cards[$puzzle]["stage"][$piece][$grade]["adult"]["url"]="http://182.162.201.147:10010/images/".$targetCardDir."/".$fileName;
		$cards[$puzzle]["stage"][$piece][$grade]["adult"]["filename"]=$fileName;
	}else{
		$cards[$puzzle]["stage"][$piece][$grade]["img"]["url"]="http://182.162.201.147:10010/images/".$targetCardDir."/".$fileName;	
		$cards[$puzzle]["stage"][$piece][$grade]["img"]["filename"]=$fileName;
	} 
	echo "<br>";
} 

echo "--------------------------------";
$realDir="../images/".$targetPuzzleDir;
foreach (filesInDir ($realDir) as $key => $a_file){ 
	$i++;
	$path = explode("/",$a_file);
	$fileIndex = count($path)-1;
	$fileName = $path[$fileIndex];
	$cardInfo = explode("_",$fileName);
	$puzzle = substr($cardInfo[0],6);
	$ptype = explode(".",$cardInfo[1]);
	$ptype = $ptype[0];
	echo $fileName."->";
	echo $puzzle."-";
	$cards[$puzzle][$ptype]["url"]="http://182.162.201.147:10010/images/".$targetPuzzleDir."/".$fileName;
	$cards[$puzzle][$ptype]["filename"]=$fileName;
	//if($isSil)$cards[$puzzle]["stage"][$piece][$grade]["sil"]=$fileName;
	//else $cards[$puzzle][$piece]["stage"][$grade]["img"]=$fileName;
	echo "<br>";
} 
echo "--------------------------------";
$realDir="../images/".$targetPuzzleCardDir;
foreach (filesInDir ($realDir) as $key => $a_file){ 
	$i++;
	$path = explode("/",$a_file);
	$fileIndex = count($path)-1;
	$fileName = $path[$fileIndex];
	$cardInfo = explode("_",$fileName);
	$puzzle = substr($cardInfo[0],6);
	$ptype = explode(".",$cardInfo[1]);
	$ptype = $ptype[0];
	$grade = substr($ptype,4,1);
	$isSil = (strpos($ptype,'s') !== false);
	
	if($grade==2)$grade="perfect";
	else $grade="normal";

	if($isSil)$ftype="sil";
	else $ftype="img";

	echo $fileName."->";
	echo $puzzle."-";
	echo $grade."-";
	echo $ftype."-";
	
	$cards[$puzzle]["clearReward"][$grade][$ftype]["url"]="http://182.162.201.147:10010/images/".$targetPuzzleCardDir."/".$fileName;
	$cards[$puzzle]["clearReward"][$grade][$ftype]["filename"]=$fileName;
	//if($isSil)$cards[$puzzle]["stage"][$piece][$grade]["sil"]=$fileName;
	//else $cards[$puzzle][$piece]["stage"][$grade]["img"]=$fileName;
	echo "<br>";
} 

echo "--------------------------------";


$realDir="../images/".$targetCardAniDir;

$i=0;
$anis=array();
foreach (filesInDir ($realDir) as $key => $a_file){ 
	$i++;
	$path = explode("/",$a_file);
	$fileIndex = count($path)-1;
	$fileName = $path[$fileIndex];
	
	$fileex = explode(".",$fileName);
	if($fileex[1]=="plist")$filetype="plist";
	else if($fileex[1]=="pvr")$filetype="pvr";
	else if($fileex[1]=="ccbi")$filetype="ccbi";
	else $filetype=0;

	$cardNo = substr($fileex[0],4);

	if($filetype){
		$anis[$cardNo][$filetype]["url"]="http://182.162.201.147:10010/images/".$targetCardAniDir."/".$fileName;
		$anis[$cardNo][$filetype]["filename"]=$fileName;
		$anis[$cardNo][$filetype]["id"]=$fileex[0];
	}
	
	echo $fileName."->";
	echo $filetype;

	echo "<br>";
} 

echo "--------------------------------";

for($puzzle=1;$puzzle<=20;$puzzle++){
	$info = new Puzzle($puzzle);
	$info->thumbnail = '{"image":"'.$cards[$puzzle]["frame"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleDir."/".$cards[$puzzle]["frame"]["filename"])/1000).'}';
	$info->original = '{"image":"'.$cards[$puzzle]["front"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleDir."/".$cards[$puzzle]["front"]["filename"])/1000).'}';
	$info->center = '{"image":"'.$cards[$puzzle]["back"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleDir."/".$cards[$puzzle]["back"]["filename"])/1000).'}';
	$info->face = '{"image":"'.$cards[$puzzle]["sum"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleDir."/".$cards[$puzzle]["sum"]["filename"])/1000).'}';
	$info->order=$puzzle;
	$info->clearReward = '{"normal":'.(($puzzle)*22-1).',"perfect":'.(($puzzle)*22).'}';
	//$info->title='{"ko":"에피소드'.$puzzle.'","en":"episode'.$puzzle.'"}';
	//$info->condition='[[{"type":"p","value":'.($puzzle-1).'}]]]';
	//$info->pathInfo='[1,2,3,4,5]';
	//$info->isEvent=0;
	//$info->color='{"r":55,"g":170,"b":255}';
	$cn=1;
	$cardset=array();

	$cardInfo = new Card((($puzzle)*22-1));
	$cardInfo->no=(($puzzle)*22-1);
	$cardInfo->grade = 3;
	$cardInfo->rank = 3;
	$cardInfo->category="nPuzzle";
	$cardInfo->imgInfo = '{"img":"'.$cards[$puzzle]["clearReward"]["normal"]["img"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleCardDir."/".$cards[$puzzle]["clearReward"]["normal"]["img"]["filename"])/1000).'}';
	$cardInfo->silImgInfo = '{"isSil":1,"img":"'.$cards[$puzzle]["clearReward"]["normal"]["sil"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleCardDir."/".$cards[$puzzle]["clearReward"]["normal"]["sil"]["filename"])/1000).'}';
	//$cardInfo->adultImgInfo = '{"img":"'.$cards[$puzzle]["clearReward"]["normal"]["img"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleCardDir."/".$cards[$puzzle]["clearReward"]["normal"]["img"]["filename"])/1000).'}';
	//$cardInfo->adultSilImgInfo = '{"isSil":1,"img":"'.$cards[$puzzle]["clearReward"]["normal"]["sil"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleCardDir."/".$cards[$puzzle]["clearReward"]["normal"]["sil"]["filename"])/1000).'}';
	$cardInfo->haveAdult=0;
	if($anis[$cardInfo->no]){
		$cardInfo->faceInfo = '{"ccbiID":"'.$anis[$cardInfo->no]["ccbi"]["id"].'","ccbi":"'.$anis[$cardInfo->no]["ccbi"]["url"].'","imageID":"'.$anis[$cardInfo->no]["plist"]["id"].'","plist":"'.$anis[$cardInfo->no]["plist"]["url"].'","pvrccz":"'.$anis[$cardInfo->no]["pvr"]["url"].'","size":'.(int)(filesize("../images/".$targetCardAniDir."/".$anis[$cardInfo->no]["pvr"]["filename"])/1000).'}';
		$cardInfo->adultFaceInfo = $cardInfo->faceInfo;
	}else {
		$cardInfo->faceInfo="";
		$cardInfo->adultFaceInfo="";
	}
	if(!$cardInfo->save(true))echo "card_normal".mysql_error().json_encode($cardInfo->getArrayData(true));

	$cardInfo = new Card((($puzzle)*22));
	$cardInfo->no=(($puzzle)*22);
	$cardInfo->grade = 4;
	$cardInfo->rank = 4;
	$cardInfo->category="nPuzzle";
	$cardInfo->imgInfo = '{"img":"'.$cards[$puzzle]["clearReward"]["perfect"]["img"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleCardDir."/".$cards[$puzzle]["clearReward"]["perfect"]["img"]["filename"])/1000).'}';
	$cardInfo->silImgInfo = '{"isSil":1,"img":"'.$cards[$puzzle]["clearReward"]["perfect"]["sil"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleCardDir."/".$cards[$puzzle]["clearReward"]["perfect"]["sil"]["filename"])/1000).'}';
	//$cardInfo->adultImgInfo = '{"img":"'.$cards[$puzzle]["clearReward"]["perfect"]["img"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleCardDir."/".$cards[$puzzle]["clearReward"]["perfect"]["img"]["filename"])/1000).'}';
	//$cardInfo->adultSilImgInfo = '{"isSil":1,"img":"'.$cards[$puzzle]["clearReward"]["perfect"]["sil"]["url"].'","size":'.(int)(filesize("../images/".$targetPuzzleCardDir."/".$cards[$puzzle]["clearReward"]["perfect"]["sil"]["filename"])/1000).'}';
	$cardInfo->haveAdult=0;
	if($anis[$cardInfo->no]){
		$cardInfo->faceInfo = '{"ccbiID":"'.$anis[$cardInfo->no]["ccbi"]["id"].'","ccbi":"'.$anis[$cardInfo->no]["ccbi"]["url"].'","imageID":"'.$anis[$cardInfo->no]["plist"]["id"].'","plist":"'.$anis[$cardInfo->no]["plist"]["url"].'","pvrccz":"'.$anis[$cardInfo->no]["pvr"]["url"].'","size":'.(int)(filesize("../images/".$targetCardAniDir."/".$anis[$cardInfo->no]["pvr"]["filename"])/1000).'}';
		$cardInfo->adultFaceInfo = $cardInfo->faceInfo;
	}else {
		$cardInfo->faceInfo="";
		$cardInfo->adultFaceInfo="";
	}
	if(!$cardInfo->save(true))echo "card_perfect".mysql_error().json_encode($cardInfo->getArrayData(true));
	


	for($i=0;$i<=4;$i++){
		$pieceNo = ($puzzle-1)*5+$i+1;
		$pieceInfo = new Piece($pieceNo);
		$pieceInfo->puzzle=$puzzle;
		$pieceInfo->pieceNo=$i+1;
		$pieceInfo->condition = '{"no":'.($pieceNo-1).'}';

		if(!$pieceInfo->save())echo "piece".mysql_error();
		for($j=0;$j<=3;$j++){
			
			$cardNo = ($puzzle-1)*22+$cn;
			$cardInfo = new Card($cardNo);
			$cardInfo->piece = $pieceNo;
			$cardInfo->no=$cardNo;
			$cardInfo->grade = $j+1;
			$cardInfo->rank = $j+1;
			$cardInfo->category="normal";
			$cardInfo->imgInfo = '{"img":"'.$cards[$puzzle]["stage"][($i+1)][($j+1)]["img"]["url"].'","size":'.(int)(filesize("../images/".$targetCardDir."/".$cards[$puzzle]["stage"][($i+1)][($j+1)]["img"]["filename"])/1000).'}';
			$cardInfo->silImgInfo = '{"isSil":1,"img":"'.$cards[$puzzle]["stage"][($i+1)][($j+1)]["sil"]["url"].'","size":'.(int)(filesize("../images/".$targetCardDir."/".$cards[$puzzle]["stage"][($i+1)][($j+1)]["sil"]["filename"])/1000).'}';
			
			if($cards[$puzzle]["stage"][($i+1)][($j+1)]["adult"]["url"]){
				$cardInfo->adultImgInfo = '{"img":"'.$cards[$puzzle]["stage"][($i+1)][($j+1)]["adult"]["url"].'","size":'.(int)(filesize("../images/".$targetCardDir."/".$cards[$puzzle]["stage"][($i+1)][($j+1)]["adult"]["filename"])/1000).'}';
				$cardInfo->adultSilImgInfo = '{"isSil":1,"img":"'.$cards[$puzzle]["stage"][($i+1)][($j+1)]["sil"]["url"].'","size":'.(int)(filesize("../images/".$targetCardDir."/".$cards[$puzzle]["stage"][($i+1)][($j+1)]["sil"]["filename"])/1000).'}';
				$cardInfo->haveAdult=1;
			}else{
				$cardInfo->adultImgInfo="";
				$cardInfo->adultSilImgInfo=""; 
				$cardInfo->haveAdult=0;
			}
			if($anis[$cardInfo->no]){
				$cardInfo->faceInfo = '{"ccbiID":"'.$anis[$cardInfo->no]["ccbi"]["id"].'","ccbi":"'.$anis[$cardInfo->no]["ccbi"]["url"].'","imageID":"'.$anis[$cardInfo->no]["plist"]["id"].'","plist":"'.$anis[$cardInfo->no]["plist"]["url"].'","pvrccz":"'.$anis[$cardInfo->no]["pvr"]["url"].'","size":'.(int)(filesize("../images/".$targetCardAniDir."/".$anis[$cardInfo->no]["pvr"]["filename"])/1000).'}';
				if($cards[$puzzle]["stage"][($i+1)][($j+1)]["adult"]["url"])$cardInfo->adultFaceInfo = $cardInfo->faceInfo;
			}else {
				$cardInfo->faceInfo="";
				$cardInfo->adultFaceInfo="";
			}
			if(!$cardInfo->save(true))echo "card".mysql_error().json_encode($cardInfo->getArrayData(true));
			echo "<br><br>";

			$cardset[$i][$j]=$cardNo;
			$cn++;
		}
	}
	$info->cardInfo = $cardset;
	if(!$info->save(true))echo "puzzle".mysql_error();
	//echo json_encode($info->getArrayData(true));
	echo "<br><br>";
	
}

echo json_encode($cards);

class abcdefg{
	static function doCommand($funcName){
		include "command/functinos/$functions.php";
		return $functinos();
	}
}
*/

if($_POST["dec"]){
	$text = bin2hex(base64_decode(str_replace(" ","+",$_POST["dec"])));

	$enc = $text."\n\n".decryptByAESWithBase64($_POST["dec"]);
	$dec = $_POST["dec"];
}else{
	$enc = $_POST["enc"];
}

if($_POST["enc"]){

	//$dec = encryptByAESWithBase64($_POST["enc"]);
}else{
	//$dec = $_POST["dec"];
}



?>


<form action="admin_convertor.php" method="post">
<input type=hidden name="gid" value=<?=$gid?>>

	origin<br>
	<textarea cols=200 rows=20 name=enc><?=$enc?></textarea> 
	<br>

	base64(aes(origin))<br>
	<textarea cols=200 rows=20 name=dec><?=$dec?></textarea>
	<input type=submit value="go!">
</from>


<?php
include "footer.php";
?>