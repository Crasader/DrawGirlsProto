<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|무료젬랭킹</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserStorage" dbWhere='{"id":"*"}' dbLimit="50" dbSort='{"fr":"desc"}' name="goldRank" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th virtual rank>rank</th>
		<th primary field='no' viewer='{"type":"text"}'>no</th>
		<th virtual field='nick' viewer='{"type":"text"}'>nick</th>
		<th field='memberID' viewer='{"type":"text"}'>memberID</th>
		<th field='fr' viewer='{"type":"text"}'>fr</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>