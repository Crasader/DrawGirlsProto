
<?php
function dbchecker($name){
$mainDB = DBGroup::create($name);

echo "<b>".$name." DB</b><br> <table class='table table-bordered'>";

for($i=0;$i<$mainDB->getMasterCount();$i++){
	$master = $mainDB->getMaster($i+1);
	$colspan = $master->getSlaveCount()+1;
	echo "<tr><td><font color=red>MASTER".($i+1)."</font><br>";
	echo $master->m_server->m_host."<br>";
	echo $master->m_name."<br>";
	if($master->m_server->selectDB($master->m_name)){
		echo "<font color=green>OK</font> <br>";
		
	}else{
		echo "<font color=red>ERROR</font> <br>"; 
	}
	echo "</td><td>SLAVE<br>of<br>Master".($i+1)." >></td>";
	for($j=0;$j<$master->getSlaveCount();$j++){
		$slave = $master->getSlave($j+1);
		echo "<td><font color=blue>SLAVE".($j+1)."</font><br>";
		echo $slave->m_server->m_host."<br>";
		echo $slave->m_name."<br>";
		if($slave->m_server->selectDB($slave->m_name)){
			$r = UserData::getQueryResult("select count(*) as cnt from ".UserData::getDBTable(),$slave->getConnection());
			if($r){
				$cnt = mysql_fetch_array($r);
				echo "<font color=orange>".$cnt["cnt"]." Users</font><br>";
			}
			echo "<font color=green>OK</font> <br>";
			
		}else{
			echo "<font color=red>ERROR</font> <br>"; 
		}
		echo "</td>";
	}
	echo "</tr>";
}

echo "</table><br><br>";
}
?>