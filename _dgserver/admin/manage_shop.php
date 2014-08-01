<?php
include "manage_header.php";
?>


<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<h2 id="tables-contextual-classes">|상점</h2>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Shop" dbWhere='' name="datatable" border=1 align=center autoSetting="true" editRowOnly="true" editType="form">
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table> 

<?php
include "manage_footer.php";
?>