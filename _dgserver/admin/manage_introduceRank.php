<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">

<h2 id="tables-contextual-classes">|추천인랭킹</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserData" dbWhere='{"id":"*"}' dbLimit="50" dbSort='{"introduceCnt":"desc"}' name="introduceRank" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
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