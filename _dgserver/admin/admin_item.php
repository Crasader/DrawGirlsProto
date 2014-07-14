<?php
include "header.php";
?>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::getMT("item")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="type" viewer='{"type":"text"}' primary editor='{"type":"text"}'>type</th>
			<th field="name" viewer='{"type":"text"}' editor='{"type":"text"}'>name</th>
			<th field="currency" viewer='{"type":"text"}' editor='{"type":"select","element":["gold","ruby","social"]}'>currency</th>
			<th field="price" viewer='{"type":"text"}' editor='{"type":"text"}'>price</th>
			<th field="option" viewer='{"type":"json"}' editor='{"type":"dictionary"}'>option</th>
			<th field="image" viewer='{"type":"custom","func":"imageViewer"}' editor='{"type":"custom","func":"imageSelector"}'>image</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "footer.php";
?>