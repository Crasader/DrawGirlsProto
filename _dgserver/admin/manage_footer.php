<br><br><br><br>
<textarea cols=200 rows=10>
	
	<?=json_encode(LogManager::get()->getLogAndClear(),JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK)?>

</textarea>

</body>
</html>


<?php 

DBManager::get()->closeDB();

if($connect){
	@mysql_close();
} 
?>