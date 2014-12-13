<?php
include "header.php";
/*
리소스만들기
./cdn 들어가서 새로운버전파일 압축
tar cfvz ./tarfiles/20140110_1.tar.gz ./drawgirls_tstore/2/
실행
만들어진 파일 다운로드ㅣ
*/
if($_POST["dec"]){
	$enc = decryptByAESWithBase64($_POST["dec"]);
}else{
	$enc = $_POST["enc"];
}

if($_POST["enc"]){

	$dec = encryptByAESWithBase64($_POST["enc"]);
}else{
	$dec = $_POST["dec"];
}


function checkPatch($url){
	$file  = explode("/",$url);
	if($file[3]=="images" || $file[3]=="resource")return true;
	return false;
}

function filecopy($srcfile,$dstfile){

	@mkdir(dirname($dstfile), 0777, true);
	if(!@copy($srcfile, $dstfile)){
		if(!@copy(urlencode($srcfile),$dstfile)){
			return @copy(urldecode($srcfile),$dstfile);
		}
	}
	return true;
}

function patchResource($dir,$refKey,$imgKey,$cls,$func =null){
	#$dir = "c_o";
	#$refKey = "imgInfo";
	#$imgKey = "img";
	#$cls = "Card";
	global $_GET;

	if($func==null){
		$func = function($refKey,$imgKey,$dir,$obj){
			global $_GET;
			global $gid;
			


			$imgInfo =& $obj->getRef($refKey);
			if(!$imgInfo) return false;
			$pointArray = explode(".",$imgInfo[$imgKey]);

			$lcnt = count($pointArray)-1;
			$lename = $pointArray[$lcnt];
			$lastPoint = $pointArray[$lcnt];
			$exfile = explode("/",$imgInfo[$imgKey]);
			for($i=3;$i<count($exfile);$i++){
				$ofile = $ofile."/".$exfile[$i];	
			}
			$ofile = "..".$ofile;
			
			$newName = CuponCode::getRandomString(20);
			if($lename=="ccz"){
				$lexe = "pvr.ccz";
			}else if($lename=="plist"){
				$lexe = "plist";
			}else{
				$lexe = $lename;
			}
			$nfile = "cdn/".$gid."/".$_GET[version]."/".$dir."/".$newName.".".$lexe;
			
			while(is_file("../".$nfile)){
				$newName = CuponCode::getRandomString(20);
				$nfile = "cdn/".$gid."/".$_GET[version]."/".$dir."/".$newName.".".$lexe;
			}

			$imgInfo[$imgKey]="http://182.162.201.147:10010/".$nfile;

			$files["ofile"]=$ofile;
			$files["nfile"]=$nfile;
			$files["newurl"]=$imgInfo[$imgKey];

			
			return $files;
		};
	}

	echo "--- $dir --- <br>";
	while($obj = $cls::getObjectbyQuery()){
		$imgInfo =& $obj->getRef($refKey);
		echo "obj[".$imgKey."]->".$imgInfo[$imgKey];
		if($_GET["mode"]=="patch" && $_GET["version"]){
			if(checkPatch($imgInfo[$imgKey])){
				 $fInfo = $func($refKey,$imgKey,$dir,$obj);
				 if(!$fInfo)continue;

				 $ofile = $fInfo["ofile"];
				 $nfile = $fInfo["nfile"];
				 $newurl = $fInfo["newurl"];
				 echo " --> ";
				 //var_dump($fInfo);
				if(!is_array($ofile)){
					$imgInfo[$imgKey] = $newurl;
					echo $imgInfo[$imgKey];
					if(filecopy($ofile,"../".$nfile)){ 	
					 	echo "<font color=green>success</font>";
					 	if(!$obj->save()){
					 		echo " <font color=red>info save fail</font>";
					 	}
					 }else{
					 	echo "<font color=red>fail (".$ofile.",".$nfile.")</font>";
					 }
				}else{
					$imgInfo[$imgKey] = $newurl[0];
					echo $imgInfo[$imgKey];	 	
					for($i=0;$i<count($ofile);$i++){
						if(filecopy($ofile[$i],"../".$nfile[$i])){ 	
						 	echo "<font color=green>success</font>";
						}else{
						 	echo "<font color=red>fail</font>";
						}
					}
				 	if(!$obj->save()){
						 echo " <font color=red>info save fail</font>";
					}
				}
			}
		}else if($_GET["mode"]=="changeurl" && $_GET["newurl"] && $_GET["oldurl"]){
				$new=str_replace($_GET["oldurl"],$_GET["newurl"],$imgInfo[$imgKey]);
				if($new!=$imgInfo[$imgKey]){
					echo "-->".$new;
					$imgInfo[$imgKey]=$new;
					if(!$obj->save()){
						echo "<font color=red>fail</font>";
					}else{
						echo "<font color=green>success</font>";
					}
				}
		}else{
			if(checkPatch($imgInfo[$imgKey])){
				echo" -> <font color=red>need patch</font>";
			}
		}
		echo"<br>";
	}

}

if($_GET["mode"]=="patch" && !$_GET["version"] ){
	echo "error";
	exit;
}
patchResource("c_o","imgInfo","img","Card");

patchResource("c_s","silImgInfo","img","Card");

patchResource("c_f","faceInfo","ccbi","Card");

patchResource("c_f","faceInfo","plist","Card");

patchResource("c_f","faceInfo","pvrccz","Card");


patchResource("ac_o","adultImgInfo","img","Card");

patchResource("ac_s","audltSilImgInfo","img","Card");


patchResource("p_c","center","image","Puzzle");

patchResource("p_t","thumbnail","image","Puzzle");


patchResource("p_o","original","image","Puzzle");


patchResource("p_f","face","image","Puzzle");

patchResource("p_f","face","image","Puzzle");



patchResource("char","resourceInfo","ccbi","Character");

patchResource("char","resourceInfo","plist","Character");

patchResource("char","resourceInfo","pvrccz","Character");

patchResource("mon","resourceInfo","pvrccz","Monster");

patchResource("mon","resourceInfo","plist","Monster");

patchResource("mon","resourceInfo","ccbi","Monster",function($refKey,$imgKey,$dir,$obj){
			global $_GET;
			global $gid;
			$imgInfo =& $obj->getRef($refKey);
			$exfile = explode("/",$imgInfo[$imgKey]);
			$ofile="";
			$nfile="";
			for($i=3;$i<count($exfile);$i++){
				$ofile = $ofile."/".$exfile[$i];	
			}
			
			$ofile = "..".$ofile;
			$nfile = "cdn/".$gid."/".$_GET[version]."/".$dir."/".$exfile[count($exfile)-1];
			
			$imgInfo[$imgKey]="http://182.162.201.147:10010/".$nfile;

			if($obj->type!="snake"){
				$files["ofile"]=$ofile;
				$files["nfile"]=$nfile;
				$files["newurl"]=$imgInfo[$imgKey];
				return $files;	
			}


			$oHead = explode(".ccbi",$ofile);
			$oHead = $oHead[0];
			$nfile = "cdn/".$gid."/".$_GET[version]."/".$dir."/".$exfile[count($exfile)-1];
			$nHead = explode(".ccbi",$nfile);
			$nHead = $nHead[0];

			
			$files["ofile"][]=$oHead."_head.ccbi";
			$files["nfile"][]=$nHead."_head.ccbi";
			$files["newurl"][]=$imgInfo[$imgKey];
			
			$files["ofile"][]=$oHead."_body.ccbi";
			$files["nfile"][]=$nHead."_body.ccbi";
			$files["newurl"][]=$imgInfo[$imgKey];
			
			$files["ofile"][]=$oHead."_tail.ccbi";
			$files["nfile"][]=$nHead."_tail.ccbi";
			$files["newurl"][]=$imgInfo[$imgKey];
			
			return $files;
});


// echo "--- monster resource --- <br>";
// $dir = "mon";
// while($obj = Monster::getObjectbyQuery()){
// 	$imgInfo =& $obj->getRef("resourceInfo");
// 	if($obj->type == "snake"){
// 		$file = explode(".ccbi",$imgInfo["ccbi"]);
// 		echo $file[0]."_head.ccbi<br>";
// 		echo $file[0]."_tail.ccbi<br>";
// 		echo $file[0]."_body.ccbi<br>";
// 	}else{
// 		echo $imgInfo["ccbi"]."<br>";
// 	}
// 	echo $imgInfo["plist"]."<br>";
// 	echo $imgInfo["pvrccz"]."<br>";

// }

?>
<br><br>

--patch--
<form action="admin_usingfilelist.php">
	<input type="hidden" name="mode" value="patch">
	<input type="hidden" name="gid" value="<?=$gid?>">
	version : <input type="text" name="version"><input type="submit" value="patch">
</form>

--changeUrl--
<form action="admin_usingfilelist.php">
	<input type="hidden" name="mode" value="changeurl">
	<input type="hidden" name="gid" value="<?=$gid?>">
	<input type="text" name="oldurl">-><input type="text" name="newurl"><input type="submit" value="patch">
</form>

<?php
include "footer.php";
?>