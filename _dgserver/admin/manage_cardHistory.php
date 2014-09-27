<?php
include "manage_header.php";

	$listViewer=array("type"=>"select");
	while($pData = Card::getRowByQuery("",null,"name,no")){
		$lang = json_decode($pData["name"],true);
		$listViewer["element"][] = $pData["no"]."-".$lang["ko"];
		$listViewer["value"][]=$pData["no"];
	}

	$listViewer2=array("type"=>"select");
	while($pData = Puzzle::getRowByQuery("",null,"title,no")){
		$lang=json_decode($pData["title"],true);
		$listViewer2["element"][] = $pData["no"]."-".$lang["ko"];
		$listViewer2["value"][]=$pData["no"];
	}
?>
<script>

	var loadFunc = function(table,data){
		var defaultData = s2j(table.attr("defaultData"));
		if(typeof(defaultData)=="undefined")defaultData={};

		if(typeof(data["data"])=="object" && data["data"].length>0 && typeof(data["data"][0]["memberID"])!="undefined"){
			defaultData["memberID"]=data["data"][0]["memberID"];
		} 

		table.attr("defaultData",j2s(defaultData));
	}

$(document).ready(function(){

	setLoadDataCallBack("datatable",loadFunc);

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

<h2 id="tables-contextual-classes">|카드정보</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="CardHistory" autoLoad="false" dbWhere='{}' dbLimit="30" dbSort='{"no":"desc"}' name="datatable" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
			<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
			<th title='회원번호' field='memberID' viewer='{"type":"text"}' editor='{"type":"text"}' >회원번호</th>
			<th title='카드' field='cardNo' viewer='<?=json_encode($listViewer)?>' editor='{"type":"text","datatype":"int"}' >카드</th>
			<th title='갯수' field='count' viewer='{"type":"text"}' editor='{"type":"text"}' >갯수</th>
			<th title='퍼즐' field='puzzleNo' viewer='<?=json_encode($listViewer2)?>' >퍼즐</th>
			<th title='코멘트' field='comment' viewer='{"type":"text"}' editor='{"type":"text"}' >코멘트</th>
			<th title='획득일시' field='takeDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >획득일시</th>
			<th title='모핑여부' field='isMorphing' viewer='{"type":"bool"}' editor='{"type":"bool"}' >모핑여부</th>
			<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>