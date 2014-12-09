<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|추천인랭킹</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserData" dbWhere='{"id":"*"}' dbLimit="50" dbSort='{"introduceCnt":"desc"}' name="introduceRank" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th virtual rank>rank</th>
		<th primary field='no' viewer='{"type":"text"}'>no</th>
		<th field='nick' viewer='{"type":"text"}' editor='{"type":"text"}' >nick</th>
		<th field='memberID' viewer='{"type":"text"}' editor='{"type":"text"}' >memberID</th>
		<th field='introduceCnt' viewer='{"type":"text"}' editor='{"type":"text"}' >introduceCnt</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>