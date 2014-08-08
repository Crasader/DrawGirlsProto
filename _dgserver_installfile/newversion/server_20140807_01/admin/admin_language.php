<?php
include "header.php";
?>


<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::getMT("language")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary editor='{"type":"text"}'>no</th>
			<th field="language" viewer='{"type":"text"}' editor='{"type":"text"}'>language</th>
			<th field="data" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"text","datatype":"int"}}'>data</th>
			<th field="isDefault" viewer='{"type":"text"}' editor='{"type":"bool"}'>isDefault</th>
			<th field="parent" viewer='{"type":"text"}' editor='{"type":"text"}'>parent</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>