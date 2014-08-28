<?php
include "header.php";
?>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Pattern" dbWhere='{}' dbLimit="30" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='category' viewer='{"type":"text"}' editor='{"type":"text"}' >category</th>
		<th field='type' viewer='{"type":"text"}' editor='{"type":"text"}' >type</th>
		<th field='name' viewer='{"type":"text"}' editor='{"type":"text"}' >name</th>
		<th field='image' viewer='{"type":"imageViewer"}' editor='{"type":"imageSelector"}' >image</th>
		<th field='template' viewer='{"type":"text"}' editor='{"type":"dictionary"}' >template</th>
		<th field='comment' viewer='{"type":"text"}' editor='{"type":"textarea"}' >comment</th>
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

