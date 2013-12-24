<?php
include "header.php";

$stopCommand = true;

include "../command/cmd2.php";

echo time()."<br>";
echo (microtime(true)*100)."<br>";
?>
 
<table width=80% align=center><tr><td>
<?php


$command = new commandClass();


echo "<hr border=2>";
echo "<br><br><font size=6 color=error>에러코드목록</font><br>";
/*
echo "<table border=1><tr><td>code</td><td>name</td></tr>";
foreach(ResultState::$m_resutlCodeList as $key=>$value){
	echo "<tr><td>$key</td><td>$value</td></tr>";
}
echo "</table>";
*/
echo "<pre>";

echo "enum GDRESULT{\n";
foreach(ResultState::$m_resutlCodeList as $key=>$value){
	echo "	$value = $key, \n";
}
echo "};";
echo"</pre><br><br>";

$class_methods = get_class_methods($command);

foreach ($class_methods as $method_name) {
    if(strpos($method_name,"help_")  !== false){
	    $realName = str_replace("help_","",$method_name);
	    $mInfo = $command->$method_name();
	    echo "<hr border=2>";
	    echo "<font size=6 color=blue>".$realName."</font><br>";
	    echo "<b>".$mInfo["discript"]."</b><br><br>";
	    echo "* parameter <br>";
	    echo "<table border=1><tr><td>name</td><td>type</td><td>discript</td></tr>";
	    for($i=0;$i<count($mInfo["param"]);$i++){
		    echo "<tr><td>".$mInfo["param"][$i]["name"]."</td><td>".$mInfo["param"][$i]["type"]."</td><td>".$mInfo["param"][$i]["discript"]."</td></tr>";
	    }
	    echo "</table><br>";
	    
	    echo"* result<br>";
	    echo "<table border=1><tr><td>code</td><td>name</td><td>message</td></tr>";
	    for($i=0;$i<count($mInfo["result"]);$i++){
		    echo "<tr><td>".$mInfo["result"][$i]["code"]."</td><td>".$mInfo["result"][$i]["name"]."</td><td>".$mInfo["result"][$i]["message"]."</td></tr>";
	    }
	    echo "</table><br>";
    }
}

?>

</td></tr></table>

<br>
<br>
<br>

<?php
include "footer.php";
?>