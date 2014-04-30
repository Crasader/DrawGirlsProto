<?php
include "header.php";

?>
<center>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Exchange" dbFunc='{"select":"getList","write":"writeData","update":"updateData","delete":"deleteData"}' dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="id" viewer='{"type":"text"}' editor='{"type":"text"}'>id</th>
			<th field="list" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}'>list</th>
			<th field="content" viewer='{"type":"text"}' editor='{"type":"text"}'>content</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>
</center>



<?php
include "footer.php";
?>