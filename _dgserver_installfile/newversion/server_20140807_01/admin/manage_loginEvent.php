<?php
include "manage_header.php";
?>
<script>
var viewer_image = function(value,option){
	value = s2j(value);
	var neditor = $("<img>").attr("src","../images/"+value["img"]).attr("width",100);
	return neditor;
 }
</script>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<h2 id="tables-contextual-classes">|로그인 이벤트</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="LoginEvent" dbWhere='' autoSetting="true" name="datatable" editRowOnly="true" editType="form" border=1>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>