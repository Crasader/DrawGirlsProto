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
		var id = $("#findNo").val();
		var selectType = $('.active[id=findType]').val();
		dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'"}');
		loadDataTable(dataview);
	});
});

var commenter = function(value,option){
	return "변경이유 : <input type='text' value='"+value+"' class='LQEditor form-control form-control-inline' editor='"+j2s(option)+"'>";
}
var commenter_value = function(obj){
	return obj.val();
} 

var clearList = function(value,option){
	value = s2j(value);
	var pushData = '<table class="table table-boarded">';
	for(i in value){
		var n = i*1+1;
			pushData+='<tr><td>'+n+'등급</td>';

			if(value[i]>0)pushData+='<td> 클리어 ('+datetimeFormat(value[i],'Y/m/d h:i:s')+')</td></tr>';
			else pushData+='<td>-</td></tr>';
	}
	pushData+="</table>";

	return pushData;
}
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

<h2 id="tables-contextual-classes">|피스정보조회</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="PieceHistory" autoLoad="false" autoSetting="true" dbWhere='{}' dbLimit="10" dbSort='{"no":"desc"}' name="datatable" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>