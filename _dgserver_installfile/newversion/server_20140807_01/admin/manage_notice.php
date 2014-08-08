<?php
include "manage_header.php";
?>
<script>
var viewer_image = function(value,option){
	value = s2j(value);
	var neditor = $("<img>").attr("src",value["img"]).attr("width",100);
	return neditor;
}
</script>
<input name="gid" value="<?=$gid?>" type="hidden">

<br><br>
<h2 id="tables-contextual-classes">|게임 공지</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Notice" dbWhere='' autoSetting="true" name="datatable" editRowOnly="true" editType="form" border=1>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>