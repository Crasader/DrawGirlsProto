<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("mission")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="type" viewer='{"type":"text"}' primary editor='{"type":"text"}'>type</th>
			<th field="name" viewer='{"type":"text"}' editor='{"type":"text"}'>name</th>
			<th field="image" viewer='{"type":"custom","func":"imageViewer"}' editor='{"type":"custom","func":"imageSelector"}'>image</th>
			<th field="template" viewer='{"type":"json"}' editor='{"type":"textarea"}'>template</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>