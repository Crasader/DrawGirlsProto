<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">

<h2 id="tables-contextual-classes">|골드랭킹</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserStorage" dbWhere='{"id":"*"}' dbLimit="50" dbSort='{"g":"desc"}' name="goldRank" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th virtual rank>rank</th>
		<th primary field='no' viewer='{"type":"text"}'>no</th>
		<th virtual field='nick' viewer='{"type":"text"}'>nick</th>
		<th field='memberID' viewer='{"type":"text"}'>memberID</th>
		<th field='g' viewer='{"type":"text"}'>g</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>