<?php
include "header.php";
?>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_COMMONSETTING?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
			<th field="key" viewer="text" editor="text">key</th>
			<th field="value" viewer="text" editor="textarea">value</th>
			<th field="comment" viewer="text" editor="textarea">comment</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "footer.php";
?>