<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_PUZZLE?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary>no</th>
			<th field="title" viewer="text" editor="text">title</th>
			<th field="thumbnail" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>thumbnail</th>
			<!--th field="top" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>top</th>
			<th field="bottom" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>bottom</th>
			<th field="left" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>left</th>
			
			<th field="right" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>right</th-->
			<th field="center" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>center</th>
			<th field="original" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>original</th>
			<th field="face" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>face</th>
			<th field="ticket" viewer="text" editor="text" editorOption='{"datatype":"int"}'>ticket</th>
			<th field="point" viewer="text" editor="text" editorOption='{"datatype":"int"}'>point</th>
			<th field="version" viewer="text" editor="text" editorOption='{"datatype":"int"}'>version</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>