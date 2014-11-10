<?php
include "../lib.php";
//"ip.php" example- display user IP address on any page
Header("content-type: application/x-javascript");
echo "var LQ_EXHANGE_CATEGORY = ".json_encode(Exchange::getCategory());
?>

