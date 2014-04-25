<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("keyintvalue")?>" dbSort="`key` asc" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="key" viewer='{"type":"text"}' editor='{"type":"text"}'>key</th>
			<th field="value" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>value</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>