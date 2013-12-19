<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_LANGUAGE?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary editor="text">no</th>
			<th field="language" viewer="text" editor="text">language</th>
			<th field="data" viewer="text" editor="textarea">data</th>
			<th field="isDefault" viewer="text" editor="bool">isDefault</th>
			<th field="parent" viewer="text" editor="text">parent</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>