<?php
include "header.php";
?>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_ITEM?>" border=1 align=center>
	<thead>
		<tr>
			<th field="type" viewer="text" primary editor="text">type</th>
			<th field="name" viewer="text" editor="text">name</th>
			<th field="currency" viewer="text" editor="select" editorOption='["gold","ruby","social"]'>currency</th>
			<th field="price" viewer="text" editor="text">price</th>
			<th field="option" viewer="json" editor="dictionary" editorOption=''>option</th>
			<th field="image" viewer="custom" viewerOption="imageViewer" editor="custom" editorOption="imageSelector">image</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "footer.php";
?>