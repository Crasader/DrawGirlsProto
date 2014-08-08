<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<h2 id="tables-contextual-classes">|업적관리</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Archivement" autoSetting="true" dbFunc='{"load":"loadWithLQTableForCardInfo"}' dbWhere='{}' name="datatable" border=1>
	<thead>
		<tr>
		
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>