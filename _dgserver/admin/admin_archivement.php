<?php
include "header.php";
?>


<center>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Archivement" dbWhere='{}' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="id" viewer='{"type":"text"}' editor='{"type":"text"}'>id</th>
			<th field="category" viewer='{"type":"text"}' editor='{"type":"text"}'>category</th>
			<th field="title" viewer='{"type":"text"}' editor='{"type":"text"}'>title</th>
			<th field="content" viewer='{"type":"text"}' editor='{"type":"text"}'>content</th>
			<th field="reward" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"count","type":"text","datatype":"int"}]}}'>reward</th>
			<th field="goal" viewer='{"type":"text"}'  editor='{"type":"text"}'>goal</th>
			<th field="exchangeID" viewer='{"type":"text"}' editor='{"type":"text"}'>exchangeID</th>
			<th field="exchangeList" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}'>exchangeList</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>