<br><br><br><br>
<center>
<textarea cols=200 rows=10>
	[LOG]
	<?=json_encode(LogManager::getLogAndClear(),JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK)?>

</textarea>
</center
</body>
</html>


<?php 

DBServer::closeAllConnection();

if($connect){
	@mysql_close();
} 
?>r