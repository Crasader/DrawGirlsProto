<?php
include "header.php";
?>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Monster" dbWhere='{}' dbLimit="10" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='type' viewer='{"type":"text"}' editor='{"type":"select","element":["circle","snake","jr"]}' >type</th>
		<th field='name' viewer='{"type":"text"}' editor='{"type":"text"}' >name</th>
		<th field='image' viewer='{"type":"imageViewer"}' editor='{"type":"imageSelector"}' >image</th>
		<th field='resourceInfo' viewer='{"type":"json"}' editor='{"type":"dictionary","element":[{"field":"ccbiID","type":"text"},{"field":"ccbi","type":"resourceSelector","viewer":"text"},{"field":"imageID","type":"text"},{"field":"plist","type":"resourceSelector","viewer":"text"},{"field":"pvrccz","type":"resourceSelector","viewer":"text"},{"field":"size","type":"text","datatype":"int"}]}' >resourceInfo</th>
		<th field='script' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"field":"ko","type":"dictionary","element":[{"type":"text","field":"start"},{"type":"text","field":"clear"},{"type":"text","field":"fail"}]},{"field":"en","type":"dictionary","element":[{"type":"text","field":"start"},{"type":"text","field":"clear"},{"type":"text","field":"fail"}]}]}' >script</th>
		<th field='comment' viewer='{"type":"text"}' editor='{"type":"textarea"}' >comment</th>
		<th always field='version' viewer='null' editor='null' >version</th>
		<th manage='[delete,update,insert]' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";

//LQDataTable 초기설정에 updateAllways=true 속성 추가하여 업데이트 요청시 항상 같이 날려주도록 하기. --> version 관리를 위해..
?>

