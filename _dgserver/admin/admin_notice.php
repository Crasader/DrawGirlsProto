<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("notice")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="title" viewer='{"type":"text"}' editor='{"type":"text"}'>title</th>			
			<th field="startDate" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>startDate</th>
			<th field="endDate" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>endDate</th>
			<th field="os" viewer='{"type":"text"}' editor='{"type":"select","element":["all","ios","android"]}'>os</th>	
			<th field="language" viewer='{"type":"text"}' editor='{"type":"text"}'>language</th>	
			<th field="imgInfo" viewer='{"type":"json"}' editor='{"type":"dictionary","element":[{"field":"img","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}'>imgInfo</td>
			<th field="linkURL" viewer='{"type":"text"}' editor='{"type":"text"}'>linkURL</th>
			<th field="content" viewer='{"type":"textarea"}' editor='{"type":"textarea"}'>content</th>
			<th field="order" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>order</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>