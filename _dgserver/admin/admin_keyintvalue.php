<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_KEYINTVALUE?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
			<th field="key" viewer="text">key</th>
			<th field="value" viewer="text" editor="text" editorOption='{"datatype":"int"}'>value</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>