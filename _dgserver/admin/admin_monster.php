<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("monster")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="type" viewer='{"type":"text"}' editor='{"type":"select","element":["circle","snake","jr"]}'>type</th>
			<th field="name" viewer='{"type":"text"}' editor='{"type":"text"}'>name</th>
			<th field="image"  viewer='{"type":"custom","func":"imageViewer"}' editor='{"type":"custom","func":"imageSelector"}'>image</th>
			<th field="resourceInfo" viewer='{"type":"json"}' editor='{"type":"dictionary","element":
			[	
				{"field":"ccbiID","type":"text"},		
				{"field":"ccbi","type":"custom","func":"resourceSelector","viewer":"text"},
				{"field":"imageID","type":"text"},	
				{"field":"plist","type":"custom","func":"resourceSelector","viewer":"text"},
				{"field":"pvrccz","type":"custom","func":"resourceSelector","viewer":"text"},
				{"field":"size","type":"text","datatype":"int"}					
			]
			}'>resourceInfo</th>
			<th field="comment" viewer='{"type":"text"}' editor='{"type":"textarea"}'>comment</th>
			
			<th manage="delete update insert">manage</th>
			
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>