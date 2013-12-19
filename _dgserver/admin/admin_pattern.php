<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_PATTERN?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
			<th field="category" viewer="text" editor="text">category</th>
			<th field="type" viewer="text" editor="text">type</th>
			<th field="name" viewer="text" editor="text">name</th>
			<th field="image" viewer="custom" viewerOption="imageViewer" editor="custom" editorOption="imageSelector">image</th>
			<th field="template" viewer="text" editor="textarea">template</th>
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