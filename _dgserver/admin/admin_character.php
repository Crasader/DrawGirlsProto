<?php
include "header.php";

?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_CHARACTER?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" primary>no</th>
			<th field="name" viewer="text" editor="text">name</th>
			<th field="purchaseInfo" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"type","editor":"text"},			
				{"field":"value","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>purchaseInfo</th>
			<th field="statInfo" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"gold","editor":"text","editorOption":{"datatype":"int"}},
				{"field":"percent","editor":"text","editorOption":{"datatype":"int"}},
				{"field":"feverTime","editor":"text","editorOption":{"datatype":"int"}},
				{"field":"speed","editor":"text","editorOption":{"datatype":"int"}},
				{"field":"life","editor":"text","editorOption":{"datatype":"int"}}			
							
			]
			'>statInfo</th>
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
			<th field="comment" viewer="text" editor="textarea" editorOption='{"datatype":"int"}'>comment</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";
?>