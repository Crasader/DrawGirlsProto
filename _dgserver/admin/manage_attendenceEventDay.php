<?php
include "manage_header.php";

$autoLoad = "false";
if($_GET["eventNo"]){
	$autoLoad = "true";
}

$listViewer=array("type"=>"select","field"=>"type");
while($pData = AttendenceEvent::getRowByQuery("",null,"no,title")){
	$listViewer["element"][] = $pData["title"]."(".$pData["no"].")";
	$listViewer["value"][]=$pData["no"];
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
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="AttendenceEventDay" dbWhere='{"type":"eventNo","id":<?=$_GET["eventNo"]?>}' editRowOnly="true" dbLimit="30" dbSort='{"no":"asc"}' editType="form" name="datatable" border=1 autoLoad="<?=$autoLoad?>" commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='이벤트번호' field='eventNo' viewer='<?=json_encode($listViewer)?>' editor='<?=json_encode($listViewer)?>' >이벤트번호</th>
		<th title='선물함내용' field='title' viewer='{"type":"text"}' editor='{"type":"text"}' >선물함내용</th>
		<th title='몇번째날?' field='day' viewer='{"type":"text"}' editor='{"type":"text"}' >몇번째날?</th>
		<th title='교환ID' field='exchangeID' viewer='{"type":"exchangeviewer"}' editor='{"type":"exchangemaker","content":"출석이벤트","statsID":"attendence","statsValueField":"no"}' >교환ID</th>
		<th manage='insert delete update' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>