<?php
	include "manage_header.php";

	$puzzleViewer=array("type"=>"select");
	while($pData = Puzzle::getRowByQuery("",null,"title,no")){
		$lang = json_decode($pData["title"],true);
		$puzzleViewer["element"][] = $pData["no"]."-".$lang["ko"];
		$puzzleViewer["value"][]=$pData["no"];
	}
?>
<script>


	var loadFunc = function(table,data,filename){
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

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|보유퍼즐</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="PuzzleHistory" autoLoad="false" dbWhere='{}' dbLimit="50" dbSort='{"no":"desc"}' name="datatable" border=1  commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
			<th primary title='고유번호' field='no' viewer='{"type":"text"}'>고유번호</th>
			<th title='회원번호' field='memberID' viewer='{"type":"text"}' editor='{"type":"text"}' >회원번호</th>
			<th title='퍼즐정보' field='puzzleNo' viewer='<?=json_encode($puzzleViewer)?>' editor='<?=json_encode($puzzleViewer)?>' >퍼즐정보</th>
			<th title='오픈유형' field='openType' viewer='{"type":"text"}' editor='{"type":"text"}' >오픈유형</th>
			<th title='최초오픈일시' field='openDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >최초오픈일시</th>
			<th title='최초완성일시' field='clearDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >최초완성일시</th>
			<th title='퍼펙트완성일시' field='perfectDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >퍼펙트완성일시</th>
			<th title='보상' field='reward' viewer='{"type":"text"}' >보상</th>
			<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>
	</tbody>
</table>


<?php
include "manage_footer.php";
?>