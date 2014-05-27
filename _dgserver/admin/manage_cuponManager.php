<?php
include "manage_header.php";
?>
<center>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="CuponManager" dbWhere='' name="datatable" border=1 autoSetting="true">
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>