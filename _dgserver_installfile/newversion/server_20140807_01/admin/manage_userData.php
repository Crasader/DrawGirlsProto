<?php
include "manage_header.php";
?>

<br><br>
<h2 id="tables-contextual-classes">|유저목록</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserData" dbWhere='' dbLimit=10 autoSetting="true" name="datatable" editRowOnly="true" editType="form" border=1>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>
