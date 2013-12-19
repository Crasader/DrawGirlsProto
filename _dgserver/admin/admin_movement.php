<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_MONSTER_MOVEMENT?>" border=1 align=center>
	<thead>
		<tr>
			<th field="type" viewer="text" primary editor="text">type</th>
			<th field="name" viewer="text" editor="text">name</th>
			<th field="image"  viewer="custom" viewerOption="imageViewer" editor="custom" editorOption="imageSelector">image</th>
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