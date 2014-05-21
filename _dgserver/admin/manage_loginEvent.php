<?php
include "manage_header.php";
?>
<center>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="LoginEvent" dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="title" viewer='{"type":"text"}' editor='{"type":"text"}'>title</th>
			<th field="os" viewer='{"type":"text"}' editor='{"type":"select","element":["all","android","ios"]}'>os</th>
			<th field="language" viewer='{"type":"text"}' editor='{"type":"text"}'>language</th>	
			<th field="startDate" viewer='{"type":"text"}' editor='{"type":"text"}'>startDate</th>
			<th field="endDate" viewer='{"type":"text"}' editor='{"type":"text"}'>endDate</th>
			<th field="startTime" viewer='{"type":"text"}' editor='{"type":"text"}'>startTime</th>
			<th field="endTime" viewer='{"type":"text"}' editor='{"type":"text"}'>endTime</th>
			<th field="reward" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"}]}}'>reward</th>
			<th field="repeat" viewer='{"type":"text"}' editor='{"type":"text"}'>repeat</th>
			<th field="exchangeID" viewer='{"type":"text"}' editor='{"type":"text"}'>exchangeID</th>
			<th field="exchangeList" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}'>exchangeList</th>
			
			<th manage="insert delete update">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>