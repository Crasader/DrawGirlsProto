<?php
include "manage_header.php";
?>

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|몬스터패턴</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Pattern" dbWhere='{}' dbSort='{"isUse":"desc","category":"desc"}' dbLimit="50" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='category' viewer='{"type":"text"}'>category</th>
		<th field='isUse' viewer='{"type":"select","element":["사용","미사용"],"value":[1,0]}'>isUse</th>
		<th field='type' viewer='{"type":"text"}' editor='{"type":"text"}' >type</th>
		<th field='name' viewer='{"type":"text"}' editor='{"type":"text"}' >name</th>
		<th field='image' viewer='{"type":"imageViewer"}'>image</th>
		<th field='comment' viewer='{"type":"text"}' editor='{"type":"textarea"}' >comment</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>

