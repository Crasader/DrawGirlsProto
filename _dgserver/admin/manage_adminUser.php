<?php
include "manage_header.php";
?>
<input name="gid" value="<?=$gid?>" type="hidden">

<br><br>
<h2 id="tables-contextual-classes">|관리자목록</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="AdminUser" dbWhere='' autoSetting="true" name="datatable" editRowOnly="true" editType="form" border=1>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>