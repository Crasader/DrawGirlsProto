<?php
include "manage_header.php";
?>
<center>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="AttendenceEvent" dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="title" viewer='{"type":"text"}' editor='{"type":"text"}'>title</th>
			<th field="os" viewer='{"type":"text"}' editor='{"type":"select","element":["all","android","ios"]}'>os</th>
			<th field="language" viewer='{"type":"text"}' editor='{"type":"text"}'>language</th>	
			<th field="startDate" viewer='{"type":"text"}' editor='{"type":"text"}'>startDate</th>
			<th field="endDate" viewer='{"type":"text"}' editor='{"type":"text"}'>endDate</th>
			<th field="rewardList" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"type","field":"type"},{"type":"text","field":"display"}]}}'>rewardList</th>
			<th field="exchangeIDList" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"text"}}'>exchangeIDList</th>
			<th manage="insert delete update">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>