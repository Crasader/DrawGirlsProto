<?php
include "manage_header.php";
// include "phpexcel/PHPExcel.php";
// include "phpexcel/PHPExcel/IOFactory.php";

// $filename = "../images/3120544836.xls";

 

?>
<script>

$(document).ready(function(){
	

	$('body').on('click','#findUserInfo',function(){
		
		var dataview = getDataForm("userinfo");
		var id = $("#findNo").val();
		var selectType = $('.active[id=findType]').val();
		dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'"}');
		loadDataForm(dataview);
	});



});

var editor_sendType = function(value,option){
	var neditor = $("<div>").addClass("LQEditor").attr("editor",j2s(option));
	var select = $("<div>").addClass("btn-group radio-btn LQEditor").appendTo(neditor);
	$("<button>").attr("id","sendType").attr("value","direct").append("직접지급").appendTo(select).addClass("btn btn-default");
	$("<button>").attr("id","sendType").attr("value","giftbox").append("선물함지급").appendTo(select).addClass("btn btn-default active");;
	$("<input>").attr("id","message").appendTo(select).addClass("form-control").attr("placeholder","변경사유 입력");

	return neditor;
}

var editor_sendType_value = function(obj){
	var t = obj.find('.active[id=sendType]').val();
	var v = obj.find('[id=message]').val();
	return {"type":t,"message":v};
}

var editor_propData = function(value,option){
	var neditor = $("<div>").addClass("LQEditor").attr("editor",j2s(option));
	var select = $("<div>").addClass("btn-group radio-btn LQEditor").appendTo(neditor);
	$("<button>").attr("id","propSign").attr("value","p").append("지급").appendTo(select).addClass("btn btn-default active");
	$("<button>").attr("id","propSign").attr("value","m").append("회수").appendTo(select).addClass("btn btn-default");;
	$("<input>").attr("id","propCount").appendTo(select).addClass("form-control").attr("placeholder","갯수입력");

	return neditor;
}


var editor_propData_value = function(obj){
	var t = obj.find('.active[id=propSign]').val();
	var v = obj.find('[id=propCount]').val();
	return {"sign":t,"count":v};
}


var viewer_memberList = function(value,option){
	value = s2j(value);
	var neditor = $("<textarea>").addClass("LQEditor form-control").attr("editor",j2s(option));
	neditor.attr("v",j2s(value));
	for(k in value){
		neditor.append(value[k]+"("+k+")\n");
	}

	return neditor;
}

var viewer_memberList_value = function(obj){
	var t = s2j(obj.attr("v"));
	return t;
}



var excelMemberList = function(value,option){
	var newobj = new FileUploader(value,option);
	newobj.callbackFunc = function(resultTxt){
		log(resultTxt);
		var result = s2j(resultTxt);	
		result = result["excelData"][0];
		var memberlist={};
		for(var i=0;i<result.length;i++){
			memberlist[""+result[i]]="nick";
		}
		var op = $("[field=memberList]").find(".LQEditor").attr("editor");
		$("[field=memberList]").html("").append(viewer_memberList(memberlist,op));

		this.editor.attr("v",j2s(resultTxt));
	}
	var neditor = newobj.createForm();
	neditor.attr("editor",j2s(option));
	return neditor;
}

var excelMemberList_value = function(obj){
	return s2j(obj.attr("v"));
}

var addByID = function(obj){
	var neditor = $("<div>").addClass("Editor");
	var input = $("<input>").addClass("form-control form-control-inline").attr("id","addID").appendTo(neditor);
	var btn = $("<button>").addClass("btn btn-primary").append("Add").attr("id","addIDBtn").on("click",function(){
		
			var v = s2j($("[field=memberList]").find(".LQEditor").attr("v"));
			var op = $("[field=memberList]").find(".LQEditor").attr("editor");
			if(!v)v={};
			v[""+$("#addID").val()]="nick";
			$("[field=memberList]").html("").append(viewer_memberList(v,op));
	}).appendTo(neditor);;
	return neditor;
}
</script>
<input name="gid" value="<?=$gid?>" type="hidden">
<br><br>
<!-- <div class="table-responsive">
	<table align=center>
		<tr>
			<td>
				<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">
				  <button id="findType" type="button" value="nick" class="btn btn-default" data-toggle="button">닉네임</button>
				  <button id="findType" type="button" value="fb" class="btn btn-default" data-toggle="button">페이스북</button>
				  <button id="findType" type="button" value="pc" class="btn btn-default" data-toggle="button">Payco</button>
				  <button id="findType" type="button" value="sno" class="btn btn-default active" data-toggle="button">회원번호</button>
				</div>
			</td>
			<td>
				<div class="input-group">
		      <input type="text" class="form-control" id="findNo">
		      <span class="input-group-btn">
		        <button class="btn btn-default" type="button" id="findUserInfo">조회</button>
		      </span>
	      </div>
			</td>
		</tr>
		<tr>
		<td colspan=2>

		</td>
		</tr>
	</table>
</div> -->
<br><br>
<div class="table-responsive">
<table class="LQDataForm" autoLoad="false" startMode="update" dbSource="dataManager2.php" dbClass="SendItem" dbWhere='' name="userinfo" width="1000" align=center commenter='{"type":"custom","func":"commenter"}'>
	<tr><td manage="update insert delete">
	<h2 id="tables-contextual-classes">|재화/아이템 변경</h2>
	<table class="table table-bordered">
		<tr>
			<td>엑셀파일로로드</td>
			<td field="excelMemberList" editor='{"type":"excelMemberList","uploadFile":"./processupload/processupload.php","uploadDir":"../images/","excelReturn":true, "removeFile":true}'></td>
		</tr>
		<tr>
			<td>아이디추가</td>
			<td field="addByID" editor='{"type":"addByID"}'></td>
		</tr>
		<tr>
			<td>대상회원목록</td>
			<td field="memberList" editor='{"type":"viewer_memberList"}'></td>
		</tr>
		<tr>
			<td>교환ID</td>
			<td field="exchangeID" editor='{"type":"exchangemaker"}'></td>
		</tr>
		<tr>
			<td>변경방식</td>
			<td field="sendType" editor='{"type":"editor_sendType"}'></td>
		</tr>
		<tr>
			<td colspan=2 align=center><button class="LQSendButton btn btn-primary" func="updateWithLQForm">보내기</button></td>
		</tr>
	</table>


</td></tr>
</table>
</div>


<?php
include "manage_footer.php";
?>

