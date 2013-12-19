<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_MONSTER?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
			<th field="name" viewer="text" editor="text">name</th>
			<th field="image"  viewer="custom" viewerOption="imageViewer" editor="custom" editorOption="imageSelector">image</th>
			<th field="resourceInfo" viewer="json" editor="dictionary" editorOption='
			[	
				{"field":"ccbiID","editor":"text"},		
				{"field":"ccbi","editor":"custom","editorOption":"resourceSelector","viewer":"text"},
				{"field":"imageID","editor":"text"},	
				{"field":"plist","editor":"custom","editorOption":"resourceSelector","viewer":"text"},
				{"field":"pvrccz","editor":"custom","editorOption":"resourceSelector","viewer":"text"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}					
			]
			'>resourceInfo</th>
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