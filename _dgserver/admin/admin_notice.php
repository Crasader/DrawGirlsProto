<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("notice")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="isOn" viewer='{"type":"text"}' editor='{"type":"bool"}'>isOn</th>
			<th field="startDate" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>startDate</th>
			<th field="endDate" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>endDate</th>
			<th field="title" viewer='{"type":"text"}' editor='{"type":"text"}'>title</th>			
			<th field="imgInfo" viewer='{"type":"json"}' editor='{"type":"dictionary","element":[{"field":"img","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}'>imgInfo</td>
			<th field="command" viewer='{"type":"text"}' editor='{"type":"textarea"}'>command</th>	
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>