</body>
</html>

<?php 

DBManager::get()->closeDB();

if($connect){
	@mysql_close();
} 
?>