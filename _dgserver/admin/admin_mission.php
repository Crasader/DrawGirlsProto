<?php
include "header.php";

?>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Mission" autoSetting="false" dbWhere='{}' dbLimit="50" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='type' viewer='{"type":"text"}' editor='{"type":"text"}' >type</th>
		<th field='isUse' viewer='{"type":"text"}' editor='{"type":"bool"}' >isUse</th>
		<th field='name' viewer='{"type":"text"}' editor='{"type":"text"}' >name</th>
		<th field='image' viewer='{"type":"text"}' editor='{"type":"text"}' >image</th>
		<th field='template' viewer='{"type":"text"}' editor='{"type":"textarea"}' >template</th>
		<th field='levelInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"type":"text","field":"propName"},{"type":"text","field":"max"},{"type":"text","field":"min"},{"type":"text","field":"variation"}]}' >levelInfo</th>
		<th manage='update delete insert' ></th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>
<?php
include "footer.php";
?>