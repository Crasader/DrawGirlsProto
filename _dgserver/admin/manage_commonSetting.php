<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<h2 id="tables-contextual-classes">|기본설정</h2>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="CommonSetting" dbWhere='{}' dbLimit="100" name="datatable" border=1 align=center commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='key' viewer='{"type":"text"}' editor='{"type":"text"}' >key</th>
		<th field='value' viewer='{"type":"autoViewer"}' editor='{"type":"autoEditor"}' >value</th>
		<th field='comment' viewer='{"type":"text"}' editor='{"type":"textarea"}' >comment</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>
	</tbody>
</table>

<?php
include "manage_footer.php";
?>