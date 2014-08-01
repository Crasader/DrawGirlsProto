<?php
include "manage_header.php";
?>
<script>
var AtdrewardInfo=function(value,option){
	var vvv = s2j(option);
	var neditor=$("<div>").addClass(".LQEditor").attr("align","center");
	$("<button>").addClass("btn btn-primary").append("보상관리").appendTo(neditor).on("click",function(){
		$(location).attr('href',"manage_attendenceEventDay.php?gid="+gid+"&eventNo="+vvv["rowData"]["no"]);
	});
	return neditor;
}
</script>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<h2 id="tables-contextual-classes">|출석이벤트</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="AttendenceEvent" dbWhere='' autoSetting="true" editRowOnly="true" editType="form" name="datatable" border=1>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>