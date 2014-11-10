<?php
	include "manage_header.php";

	$listViewer=array("type"=>"select");
	while($pData = Character::getRowByQuery("",null,"name,no")){
		$lang = json_decode($pData["name"],true);
		$listViewer["element"][] = $pData["no"]."-".$lang["kr"];
		$listViewer["value"][]=$pData["no"];
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

<h2 id="tables-contextual-classes">|캐릭터정보</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="CharacterHistory" autoLoad="false" dbWhere='{}' dbLimit="10" dbSort='{"no":"desc"}' name="datatable" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
			<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
			<th title='회원번호' field='memberID' viewer='{"type":"text"}' editor='{"type":"text"}' >회원번호</th>
			<th title='캐릭터' field='characterNo' viewer='<?=json_encode($listViewer)?>' editor='<?=json_encode($listViewer)?>' >캐릭터</th>
			<th title='레벨' field='level' viewer='{"type":"text"}' editor='{"type":"text"}' >레벨</th>
			<th title='경험치' field='exp' viewer='{"type":"text"}' editor='{"type":"text"}' >경험치</th>
			<th title='구입일시' field='regDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >구입일시</th>
			<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>