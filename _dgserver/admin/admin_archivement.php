<?php
include "header.php";
?>


<center>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Archivement" autoSetting="true" dbWhere='{}' name="datatable" border=1>
	<thead>
		<tr>
		
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>

