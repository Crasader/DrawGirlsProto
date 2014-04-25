<?php
include "header.php";

?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("character")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="name" viewer='{"type":"text"}' editor='{"type":"text"}'>name</th>
			<th field="sale" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>sale</th>
			<th field="purchaseInfo" viewer='{"type":"json"}' editor='{"type":"dictionary","element":
			[
				{"field":"type","type":"text"},			
				{"field":"value","type":"text","datatype":"int"}			
			]
			}'>purchaseInfo</th>
			<th field="statInfo" viewer='{"type":"json"}' editor='{"type":"dictionary","element":
			[
				{"field":"gold","type":"text","datatype":"int"},
				{"field":"percent","type":"text","datatype":"int"},
				{"field":"feverTime","type":"text","datatype":"int"},
				{"field":"speed","type":"text","datatype":"int"},
				{"field":"life","type":"text","datatype":"int"}	,
				{"field":"color","type":"select","element":[0,1,2,3,4,5,6]}				
							
			]
			}'>statInfo</th>
			<th field="resourceInfo" viewer='{"type":"json"}' editor='{"type":"dictionary","element":
			[	
				{"field":"ccbiID","type":"text"},		
				{"field":"ccbi","type":"custom","func":"resourceSelector"},
				{"field":"imageID","type":"text"},	
				{"field":"plist","type":"custom","func":"resourceSelector"},
				{"field":"pvrccz","type":"custom","func":"resourceSelector"},
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