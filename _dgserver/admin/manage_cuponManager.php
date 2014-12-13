<?php
include "manage_header.php";
?>

<script>

function CuponManager(value,option){
	this.list = s2j(value);
	this.option = s2j(option);
	this.editor = $("<div>").addClass("LQEditor");
	this.editTable = $("<table>").addClass("table table-boarded");
	
	this.addNewRow = function(data){

		var editTableTR2 = $("<tr>").appendTo(this.editTable);
		$("<td>").appendTo(editTableTR2).append(data["no"]);
		$("<td>").appendTo(editTableTR2).append(data["cuponNo"]);
		$("<td>").appendTo(editTableTR2).append(data["cuponCode"]);


		// var editTableTR2 = $("<tr>").attr("datarow","");
		// var editTableTD2 = $("<td>").appendTo(editTableTR2).append(editorFunc_text(lang,{"type":"text","placeholder":"lang code"}));
		// var editTableTD3 = $("<td>").appendTo(editTableTR2).append(editorFunc_text(content,{"type":"text","placeholder":"content"}));
		// var editTableTD4 = $("<td>").appendTo(editTableTR2);
		// $("<button>").append("-").addClass("btn btn-danger").appendTo(editTableTD4).on("click",{"obj":editTableTR2},function(event){event.data.obj.remove();});
		this.editTable.find("tr:last").before(editTableTR2);
	}

	this.createForm = function(){
		var editor = this.editor;
		var editTable = this.editTable;
		editor.html("");
		editTable.html("");


		// var editTableTR2 = $("<tr>").appendTo(editTable);
		// $("<td>").appendTo(editTableTR2).append("번호");
		// $("<td>").appendTo(editTableTR2).append("쿠폰ID");
		// $("<td>").appendTo(editTableTR2).append("쿠폰코드");
		var editTableTR3 = $("<tr>").appendTo(editTable);
		var editCell = $("<td>").appendTo(editTableTR3).attr("colspan",3);
		$("<button>").addClass("btn btn-primary").append("코드추가").appendTo(editCell).on("click",{"obj":this},function(event){

			$(location).attr('href',"manage_cuponCode.php?gid="+gid+"&cuponNo="+event.data.obj.option["rowData"]["no"]);
		});

		var editTableTR2 = $("<tr>").appendTo(editTable);
		$("<td>").appendTo(editTableTR2).append("코드번호");
		$("<td>").appendTo(editTableTR2).append("쿠폰번호");
		$("<td>").appendTo(editTableTR2).append("쿠폰코드");


		editor.append(editTable);

		this.ajaxCuponCodeList();

		return this.editor;
	}

	this.ajaxCuponCodeList = function(){
		var obj=this;
		//ajax로 생성
		$.ajax({
	    url : "dataManager2.php", 
	    data : {"gid":gid,"dbMode":"custom","dbFunc":"getCuponList","dbClass":"CuponCode","param":j2s({"cuponNo":this.option["rowData"]["no"]})},
	    dataType : "json", 
	    type : "post",
	    success : function(data){
	    	log(data);
	    	if(data["result"]["code"]==1){
	    		for(var i in data["list"]){
	    			obj.addNewRow(data["list"][i]);	
	    		}
	    	}else{
	    		alert("error",data["result"]["msg"],function(){obj.createForm();$(this).dialog("close");});
	    	}
	    },
	    error : function(e){
	    	alert("error",j2s(e),function(){obj.createForm();$(this).dialog("close");});
	    }
		});
	}
}

var cuponCount = function(value,option){
	if(!value) return "";

	var newobj = new CuponManager(value,option);

	var neditor=$("<div>").addClass(".LQEditor").append(value+"개 <br>").attr("align","center");
	$("<button>").addClass("btn btn-primary").append("코드관리").appendTo(neditor).on("click",function(){
		alert("코드목록",newobj.createForm(),null,{"확인":function(){$(this).dialog( "close" );}});
	});
	return neditor;
}
</script>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|쿠폰관리</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="CuponManager" dbLimit="30" dbWhere='{}' name="datatable" editRowOnly="true" editType="form" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th virtual title='진행상태' field='state' viewer='{"type":"text"}' >진행상태</th>
		<th title='쿠폰명' field='title' viewer='{"type":"text"}' editor='{"type":"text"}' >쿠폰명</th>
		<th virtual title='갯수' field='cuponCount' viewer='{"type":"cuponCount"}' >갯수</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='국가' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelector"}' >국가</th>
		<th title='시작일시' field='startDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >시작일시</th>
		<th title='종료일시' field='endDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >종료일시</th>
		<th title='중복사용' field='isCommon' viewer='{"type":"bool"}' editor='{"type":"bool"}' >중복사용</th>
		<th title='교환ID' field='exchangeID' viewer='{"type":"exchangeviewer"}' editor='{"type":"exchangemaker","content":"쿠폰","statsID":"cupon","statsValueField":"no"}' >교환ID</th>
		<th title='생성일' field='regDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' >생성일</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>