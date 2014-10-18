<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">

<h2 id="tables-contextual-classes">|결제랭킹</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserStorage" dbWhere='{"id":"*"}' dbLimit="50" dbSort='{"m":"desc"}' name="goldRank" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}'>no</th>
		<th field='memberID' viewer='{"type":"text"}' editor='{"type":"text"}' >memberID</th>
		<th field='m' viewer='{"type":"text"}' editor='{"type":"text"}' >m</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>