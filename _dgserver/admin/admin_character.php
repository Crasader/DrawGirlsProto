<?php
include "header.php";

?>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Character" dbWhere='{}' dbLimit="30" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='name' viewer='{"type":"koreanViewer"}' editor='{"type":"languageEditor"}' >name</th>
		<th field='sale' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >sale</th>
		<th field='purchaseInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"value","type":"text","datatype":"int"}]}' >purchaseInfo</th>
		<th field='statInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"gold","type":"text","datatype":"int"},{"field":"percent","type":"text","datatype":"int"},{"field":"feverTime","type":"text","datatype":"int"},{"field":"speed","type":"text","datatype":"int"},{"field":"life","type":"text","datatype":"int"},{"field":"color","type":"select","element":[0,1,2,3,4,5,6]}]}' >statInfo</th>
		<th field='resourceInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"ccbiID","type":"text"},{"field":"ccbi","type":"resourceSelector"},{"field":"imageID","type":"text"},{"field":"plist","type":"resourceSelector"},{"field":"pvrccz","type":"resourceSelector"},{"field":"size","type":"text","datatype":"int"}]}' >resourceInfo</th>
		<th field='comment' viewer='{"type":"koreanViewer"}' editor='{"type":"languageEditor"}' >comment</th>
		<th field='missileInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary"}' >missileInfo</th>
		<th field='patternInfo' viewer='{"type":"autoViewer"}' editor='{"type":"array"}' >patternInfo</th>
		<th field='missionInfo' viewer='{"type":"autoViewer"}' editor='{"type":"array"}' >missionInfo</th>
		<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>
<?php
include "footer.php";
?>