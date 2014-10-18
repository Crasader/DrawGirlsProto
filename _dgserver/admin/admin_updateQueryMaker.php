<?php
include "header.php";
if(!$pKey)$pKey = $_GET['pKey'];
if(!$table)$table = $_GET['table'];
if(!$ver)$ver = $_GET['ver'];

if(!$pKey)$pKey = "no";
?>

<form action=admin_updateQueryMaker.php>
<input name="gid" value="<?=$gid?>" type="hidden">
테이블 : <input name='table' size="50" value='<?=$_GET['table']?>'> 
version : <input name='ver' size="50" value='<?=$_GET['ver']?>'> 
primaryKey : <input name='pKey' size="50" value='<?=$_GET['pKey']?>'> 
<input type=submit value="확인">
</form>

<?php

if(!$table)exit;
$result = mysql_query("select * from `".$table."` where version>=".$ver,DBGroup::create("main")->getConnectionForReadByRand());

$mQuery = "";
while($data = mysql_fetch_assoc($result)){
	echo $data[$pKey].",";


	$mQuery.="UPDATE `".$table."` SET ";
	$isFirst = true;
	foreach ($data as $key => $value) {
		if($key!=$pKey){
			if(!$isFirst)$mQuery.=",";
			$mQuery .= "`$key`=\"".addslashes($data[$key])."\"";
			$isFirst = false;
		}
		# code...
	}

	$mQuery.=" where `".$pKey."`='".addslashes($data[$pKey])."';\n\n";
	//echo json_encode($data)."<br>";
}



?>
<br><br>
Result:
<textarea><?=$mQuery?></textarea>

<?php
include "footer.php";
?>