<?php
include "manage_header.php";
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
		var id = $("#findTxt").val();
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
			  <button id="findType" type="button" value="id" class="btn btn-default active" data-toggle="button">교환ID</button>
			  <button id="findType" type="button" value="content" class="btn btn-default" data-toggle="button">설명</button>
			</div>
		</td><td>
			<div class="input-group">
	      		<input type="text" class="form-control" id="findTxt">
	      		<span class="input-group-btn">
	        		<button class="btn btn-default" type="button" id="findUserInfo">조회</button>
	      		</span>
      		</div>
		</td></tr></table>
</div>
<br><br>

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|교환ID(event)</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>


<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="ExchangeForEvent" dbWhere='{}' dbLimit="100" dbSort='{"no":"asc"}' name="datatable" border=1  commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='아이디' field='id' viewer='{"type":"text"}' editor='{"type":"text"}' >아이디</th>
		<th title='교환내용' field='list' viewer='{"type":"rewardViewer"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"propSelect","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}' >교환내용</th>
		<th title='설명' field='content' viewer='{"type":"text"}' editor='{"type":"text"}' >설명</th>
		<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>