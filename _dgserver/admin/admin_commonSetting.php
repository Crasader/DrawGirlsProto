<?php
include "header.php";
?>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("commonsetting")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="key" viewer='{"type":"text"}' editor='{"type":"text"}'>key</th>
			<th field="value" viewer='{"type":"text"}' editor='{"type":"dictionary"}'>value</th>
			<th field="comment" viewer='{"type":"text"}' editor='{"type":"textarea"}'>comment</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";
?>