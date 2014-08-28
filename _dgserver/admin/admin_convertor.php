<?php
include "header.php";
phpinfo();
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



?>


<form action="admin_convertor.php" method="post">
<input type=hidden name="gid" value=<?=$gid?>>

	origin<br>
	<textarea cols=200 rows=40 name=enc><?=$enc?></textarea> 
	<br>

	base64(aes(origin))<br>
	<textarea cols=200 rows=40 name=dec><?=$dec?></textarea>
	<input type=submit value="go!">
</from>


<?php
include "footer.php";
?>