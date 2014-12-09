<?php
include "manage_header.php";
?>

<br><br>
<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|유저목록</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserIndex" dbWhere='{}' dbLimit="30" dbSort='{"no":"desc"}' name="datatable" editRowOnly="true" editType="form" border=1>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' editor='{"type":"text"}' >no</th>
		<th field='memberID' viewer='{"type":"text"}' editor='{"type":"text"}' >memberID</th>
		<th field='nick' viewer='{"type":"text"}' editor='{"type":"text"}' >nick</th>
		<th field='country' viewer='{"type":"text"}' editor='{"type":"text"}' >country</th>
		<th field='flag' viewer='{"type":"text"}' editor='{"type":"text"}' >flag</th>
		<th field='lastDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"text"}' >lastDate</th>
		<th field='joinDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"text"}' >joinDate</th>
		<th field='lastHeartTime' viewer='{"type":"text"}' editor='{"type":"text"}' >lastHeartTime</th>
		<th field='lastCmdNo' viewer='{"type":"text"}' editor='{"type":"text"}' >lastCmdNo</th>
		<th field='deviceID' viewer='{"type":"text"}' editor='{"type":"text"}' >deviceID</th>
		<th field='userShardOrder' viewer='{"type":"text"}' editor='{"type":"text"}' >userShardOrder</th>
		<th field='logShardOrder' viewer='{"type":"text"}' editor='{"type":"text"}' >logShardOrder</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>

