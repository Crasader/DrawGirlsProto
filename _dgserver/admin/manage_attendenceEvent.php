<?php
include "manage_header.php";
?>
<center>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="AttendenceEvent" dbWhere='' autoSetting="true" editRowOnly="true" editType="form" name="datatable" border=1>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>