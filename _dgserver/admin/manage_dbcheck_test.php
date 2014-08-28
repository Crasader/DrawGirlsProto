<?php
$checkdb="test";
include "../lib.php";
include "../libs/dbchecker.php";
?>
<input name="gid" value="<?=$gid?>" type="hidden">
<!-- Bootstrap styles -->
<link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css">
<!-- Generic page styles -->
<link rel="stylesheet" href="jqueryupload/css/style.css">
<!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
<link rel="stylesheet" href="jqueryupload/css/jquery.fileupload.css">
<br><br>
<h2 id="tables-contextual-classes">|TEST DB서버구성도</h2><br><br>

<?php


dbchecker("main");
dbchecker("user");
dbchecker("log");

DBServer::closeAllConnection();
?>

