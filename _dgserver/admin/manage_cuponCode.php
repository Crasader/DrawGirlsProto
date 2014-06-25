<?php
include "manage_header.php";

if(!$_GET["cuponNo"])$_GET["cuponNo"]=0;

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

function CuponMaker(value,option){
	this.list = s2j(value);
	this.option = s2j(option);
	this.editor = $("<div>").addClass("LQEditor");
	this.editTable = $("<table>").addClass("table table-boarded");

	this.createForm = function(){
		var obj = this;
		var editor = this.editor;
		var editTable = this.editTable;
		editor.html("");
		editTable.html("");

		var nBtn1 = $("<button>").addClass("btn btn-primary").append("자동생성").on("click",{"obj":obj},function(event){event.data.obj.autoMake()});
		$("<tr>").append($("<td>").append(nBtn1)).appendTo(this.editTable);
		
		var nBtn2 = $("<button>").addClass("btn btn-info").append("수동생성").on("click",{"obj":obj},function(event){event.data.obj.manualMake()});		
		$("<tr>").append($("<td>").append(nBtn2)).appendTo(this.editTable);

		var nBtn3 = $("<button>").addClass("btn btn-info").append("파일생성").on("click",{"obj":obj},function(event){event.data.obj.fileMake()});		
		$("<tr>").append($("<td>").append(nBtn3)).appendTo(this.editTable);

		editor.append(editTable);
		//추가모드
		return this.editor;
	}

	this.autoMake = function(){
		this.editor.attr("mode","auto");
		this.editTable.html("");
		$("<tr>").append($("<td>").append("자동생성갯수")).appendTo(this.editTable);
		$("<tr>").append($("<td>").append($("<input>").attr("id","autoMakeValue").val(1).addClass("LQEditor form-control form-control form-control-inline"))).appendTo(this.editTable);
	}

	this.manualMake = function(){
		this.editor.attr("mode","manual");
		this.editor.html("");
		this.editTable = editorFunc_array("",{"type":"array","element":{"type":"text"}});
		this.editor.append(this.editTable);

	}

	this.fileMake = function(){
		value="";
		option = {"type":"excelMemberList","uploadFile":"./processupload/processupload.php","uploadDir":"../images/","excelReturn":true, "removeFile":true};
		var newobj = new FileUploader(value,option);
		newobj.callbackFunc = this.getDataByFile;
		var neditor = newobj.createForm();
		neditor.attr("editor",j2s(option));

		this.editor.html("").append(neditor);
	}

	this.getDataByFile = function(result){
		log(j2s(result));
		var list = s2j(result);
		list = list["excelData"][0];


		this.editor.attr("mode","manual");
		this.editor.html("");

		this.editTable = editorFunc_array(list,{"type":"array","element":{"type":"text"}});
		this.editor.append(this.editTable);
	}

}

var cuponMaker = function(value,option){
	var newobj = new CuponMaker(value,option);
	var neditor = newobj.createForm();
	neditor.attr("editor",j2s(option));
	return neditor;
}

var cuponMaker_value = function(obj){
	var value = 0;
	if(obj.attr("mode")=="auto"){
		value = obj.find("#autoMakeValue").val();
	}else{
		value = editorFunc_array_value(obj.find("table"));
	}
	return value;
}

</script>
<input name="gid" value="<?=$gid?>" type="hidden">
<br><br>
<div class="table-responsive">
<table align=center><tr><td>
<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">
			  <button id="findType" type="button" value="cuponNo" class="btn btn-default active" data-toggle="button">쿠폰번호</button>
			  <button id="findType" type="button" value="cuponCode" class="btn btn-default" data-toggle="button">쿠폰코드</button>
			</div>
		</td><td>
			<div class="input-group">
	      		<input type="text" class="form-control" id="findNo" value="<?=$_GET["cuponNo"]?>">
	      		<span class="input-group-btn">
	        		<button class="btn btn-default" type="button" id="findUserInfo">조회</button>
	      		</span>
      		</div>
		</td></tr></table>
</div>
<br><br>
<h2 id="tables-contextual-classes">|쿠폰코드</h2>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="cuponCode" autoSetting="true" dbWhere='{"type":"cuponNo","id":<?=$_GET["cuponNo"]?>}' dbLimit="10" dbSort='{"no":"desc"}' editRowOnly="ture" editType="form" name="datatable" border=1>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>