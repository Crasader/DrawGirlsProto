<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_NOTICE?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
			<th field="isOn" viewer="text" editor="select" editorOption='["true","false"]'>isOn</th>
			<th field="startDate" viewer="text" editor="text" editorOption='{"datatype":"int"}'>startDate</th>
			<th field="endDate" viewer="text" editor="text" editorOption='{"datatype":"int"}'>endDate</th>
			<th field="title" viewer="text" editor="text">title</th>			
			<th field="imgInfo" viewer="json" editor="dictionary" editorOption='[{"field":"img","editor":"custom","editorOption":"imageSelector"},{"field":"size","editor":"text","editorOption":{"datatype":"int"}}]'>imgInfo</td>
			<th field="command" viewer="text" editor="textarea">command</th>	
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>