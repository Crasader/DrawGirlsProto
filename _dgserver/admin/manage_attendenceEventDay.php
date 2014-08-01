<?php
include "manage_header.php";

$autoLoad = "false";
if($_GET["eventNo"]){
	$autoLoad = "true";
}
?>

<script>
$(document).ready(function(){

	$('body').on('click','#find',function(){
		
		var dataview = getDataTable("datatable");
		var id = $("#findNo").val();
		var selectType = $('.active[id=findType]').val();
		dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'"}');
		loadDataTable(dataview);
	});
});
</script>

<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<div class="table-responsive">
<table align=center><tr><td>
<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">
			  <button id="findType" type="button" value="eventNo" class="btn btn-default active" data-toggle="button">출석이벤트번호</button>
			</div>
		</td><td>
			<div class="input-group">
	      		<input type="text" class="form-control" id="findNo" value="<?=$_GET["eventNo"]?>">
	      		<span class="input-group-btn">
	        		<button class="btn btn-default" type="button" id="find">조회</button>
	      		</span>
      		</div>
		</td></tr></table>
</div>
<br><br>


<h2 id="tables-contextual-classes">|출석이벤트보상</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="AttendenceEventDay" dbWhere='{"type":"eventNo","id":<?=$_GET["eventNo"]?>}' autoSetting="true" editRowOnly="true" editType="form" name="datatable" border=1 autoLoad="<?=$autoLoad?>">
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>