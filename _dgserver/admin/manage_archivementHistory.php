<?php
include "manage_header.php";

	$listViewer=array("type"=>"select");
	$listViewer2=array("type"=>"select");
	while($pData = Archivement::getRowByQuery("",null,"title,id,goal")){
		$lang = json_decode($pData["title"],true);
		$listViewer["element"][] = $pData["id"]."-".$lang["ko"];
		$listViewer["value"][]=$pData["id"];

		$listViewer2["element"][] = $pData["goal"];
		$listViewer2["value"][]=$pData["id"];
	}

?>
<script>

$(document).ready(function(){

	$('body').on('click','#findUserInfo',function(){
		
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
			  <button id="findType" type="button" value="nick" class="btn btn-default" data-toggle="button">닉네임</button>
			  <button id="findType" type="button" value="fb" class="btn btn-default" data-toggle="button">페이스북</button>
			  <button id="findType" type="button" value="pc" class="btn btn-default" data-toggle="button">Payco</button>
			  <button id="findType" type="button" value="sno" class="btn btn-default active" data-toggle="button">회원번호</button>
			</div>
		</td><td>
			<div class="input-group">
	      		<input type="text" class="form-control" id="findNo">
	      		<span class="input-group-btn">
	        		<button class="btn btn-default" type="button" id="findUserInfo">조회</button>
	      		</span>
      		</div>
		</td></tr></table>
</div>
<br><br>

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|업적진행현황</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>


<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="ArchivementHistory" autoLoad="false" dbWhere='{}' dbLimit="50" dbSort='{"no":"desc"}' name="datatable" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='회원번호' field='memberID' viewer='{"type":"text"}' >회원번호</th>
		<th title='업적' field='archiveID' viewer='<?=json_encode($listViewer)?>' >업적</th>
		<th title='값' field='count' viewer='{"type":"text"}' editor='{"type":"text"}' >값</th>
		<th title='목표' field='goal' viewer='<?=json_encode($listViewer2)?>' virtual>목표</th>
		<th title='완료일시' field='clearDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' >완료일시</th>
		<th title='보상일시' field='rewardDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' >보상일시</th>
		<th virtual title='보상' field='reward' viewer='{"type":"rewardViewer"}' >보상</th>
		<th manage='update delete' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>