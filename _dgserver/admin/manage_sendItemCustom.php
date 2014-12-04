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
	$("<button>").attr("id","sendType").attr("value","giftbox").append("선물함지급").appendTo(select).addClass("btn btn-default active");
	$("<textarea>").attr("id","message").appendTo(select).addClass("form-control").attr("placeholder","선물메세지");
	$("<textarea>").attr("id","data").appendTo(select).addClass("form-control").attr("placeholder","data");

	return neditor;
}

var editor_sendType_value = function(obj){
	var t = obj.find('.active[id=sendType]').val();
	var v = obj.find('[id=message]').val();
	var d = obj.find('[id=data]').val();
	return {"type":t,"message":v,"data":d};
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

	var removetr = function(event){
		var obj = event.data.obj; 
		var neditor = event.data.obj.parent().parent().parent();
		var v = s2j(neditor.attr("v")); 
		delete v[obj.attr("memberID")]; 
		neditor.attr("v",j2s(v));
		obj.remove();
	}

	var neditor = $("<div>").addClass("LQEditor").attr("editor",j2s(option));
	var table = $("<table>").addClass("table table-bordered").appendTo(neditor);
	var tr1 = $("<tr>").appendTo(table);
	$("<td>").appendTo(tr1).html("회원번호");
	$("<td>").appendTo(tr1).html("닉네임");
	$("<td>").appendTo(tr1).html("갯수");
	$("<td>").appendTo(tr1).html("삭제");

	neditor.attr("v",j2s(value));
	for(k in value){	
		var tr2 = $("<tr>").appendTo(table).addClass("member").attr("memberID",k);
		$("<td>").appendTo(tr2).html(k);
		$("<td>").appendTo(tr2).html(value[k]["nick"]);
		$("<td>").appendTo(tr2).html(value[k]["count"]);
		var btn = $("<button>").addClass("btn btn-danger").html("-").on("click",{"obj":tr2},removetr);
		$("<td>").appendTo(tr2).append(btn);

	}

	return neditor;
}

var viewer_memberList_value = function(obj){
	var t = s2j(obj.attr("v"));
	return t;
}



var getNickNameAjaxFunc = function(data){
	printLog(data);
	log(data);
	if(data["result"]["code"]==1){
		var v = s2j($("[field=memberList]").find(".LQEditor").attr("v"));
		var op = $("[field=memberList]").find(".LQEditor").attr("editor");
		if(!v)v={};
		
		for(var i=0;i<data["list"].length;i++){
			v[data["list"][i]["memberID"]]={"nick":data["list"][i]["nick"],"count":data["list"][i]["count"]};	
		}
		
		$("[field=memberList]").html("").append(viewer_memberList(v,op));
    	$('#findNo').val("");
    	$('#findNo').focus();
	}else{
		alert("error",data["result"]["message"]);
	}
}
		    

var readFileData = function(value,option){
	var newobj = new FileUploader(value,option);
	newobj.callbackFunc = function(resultTxt){
		log(resultTxt);
		var result = s2j(resultTxt);	

		result = result["list"];
		var memberlist = [];

		for(var i=0;i<result.length;i++){
			var m = {"type":"sno","find":result[i][0],"count":result[i][1]};
			memberlist.push(m);
		}
		//[{"type":$('.active[id=findType]').val(),"find":$("#findNo").val()}]
		var dbparam = {"gid":gid,"dbMode":"custom","dbFunc":"getNickName2","dbClass":"UserIndex","param":j2s(memberlist)};
		log(dbparam);
			$.ajax({
			    url : "dataManager2.php", 
			    data : dbparam,
			    dataType : "json", 
			    type : "post",
			    success : getNickNameAjaxFunc,
			    error : function(e){
			    	alert("error","아이디를 로드할수 없습니다.");
			    }
			});

		this.editor.attr("v",j2s(result));
	}
	var neditor = newobj.createForm();
	neditor.attr("editor",j2s(option));
	return neditor;
}

var readFileData_value = function(obj){
	return s2j(obj.attr("v"));
}


var addByID = function(obj){
	var neditor = $("<div>").addClass("Editor");
	// var input = $("<input>").addClass("form-control form-control-inline").attr("id","addID").appendTo(neditor);
	// var btn = $("<button>").addClass("btn btn-primary").append("Add").attr("id","addIDBtn").on("click",function(){		


	// $("<button>").addClass("btn").addClass("btn-default").attr("id","findUserInfo").html("조회").appendTo(i_span);

	// 	//ajax로 생성
	// 	$.ajax({
	//     url : "dataManager2.php", 
	//     data : {"dbMode":"custom","dbFunc":"getNickName","dbClass":"UserIndex","param":j2s({"memberID":$("#addID").val()})},
	//     dataType : "json", 
	//     type : "post",
	//     success : function(data){
	//     	printLog(data);
	//     	log(data);
	//     	if(data["result"]["code"]==1){
	//     		var v = s2j($("[field=memberList]").find(".LQEditor").attr("v"));
	// 			var op = $("[field=memberList]").find(".LQEditor").attr("editor");
	// 			if(!v)v={};
	// 			v[data["memberID"]]=data["nick"];
	// 			$("[field=memberList]").html("").append(viewer_memberList(v,op));

	//     	}else{
	//     		alert("error","아이디를 로드할수 없습니다."+j2s(data));
	//     	}
	//     },
	//     error : function(e){
	//     	alert("error","아이디를 로드할수 없습니다."+j2s(e));
	//     }
	// 	});
	// }).appendTo(neditor);;

	var i_table = $("<table>").appendTo(neditor);
	var i_tr1 = $("<tr>").appendTo(i_table);
	var i_td1 = $("<td>").appendTo(i_tr1);
	var i_div1 = $("<div>").attr("data-toggle-name","radius_options").attr("data-toggle","buttons-radio").addClass("btn-group").addClass("radio-btn").appendTo(i_td1);
	$("<button>").attr("id","findType").addClass("btn").addClass("btn-default").attr("data-toggle","button").appendTo(i_div1).attr("value","nick").html("닉네임");
	$("<button>").attr("id","findType").addClass("btn").addClass("btn-default").attr("data-toggle","button").appendTo(i_div1).attr("value","fb").html("페북아이디");
	$("<button>").attr("id","findType").addClass("btn").addClass("btn-default").attr("data-toggle","button").appendTo(i_div1).attr("value","pc").html("Payco");
	$("<button>").attr("id","findType").addClass("btn").addClass("btn-default").addClass("active").attr("data-toggle","button").appendTo(i_div1).attr("value","sno").html("회원번호");
	var i_td2 = $("<td>").appendTo(i_tr1);
	var i_div2 = $("<div>").addClass("input-group").appendTo(i_td2);
	var i_input = $("<input>").attr("type","text").addClass("form-control").attr("id","findNo").appendTo(i_div2);
	var i_span = $("<span>").addClass("input-group-btn").appendTo(i_div2);
	var findbtn = $("<button>").attr("id","findBtn").addClass("btn").addClass("btn-default").appendTo(i_span).html("추가").on("click",function(){	

	var dbparam = {"gid":gid,"dbMode":"custom","dbFunc":"getNickName","dbClass":"UserIndex","param":j2s([{"type":$('.active[id=findType]').val(),"find":$("#findNo").val()}])};
	log(dbparam);
		$.ajax({
		    url : "dataManager2.php", 
		    data : dbparam,
		    dataType : "json", 
		    type : "post",
		    success : getNickNameAjaxFunc,
		    error : function(e){
		    	alert("error","아이디를 로드할수 없습니다.");
		    }
		});

	});
	
	i_input.keyup(function(event){
	    if(event.keyCode == 13){
	       $("#findBtn").trigger("click");
	       $("#findNo").blur();
	    }
	});

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
	
	<table width=100%><tr><td><h2 id="tables-contextual-classes">|특정회원 재화/아이템 변경</h2></td><td align=right valign=bottom>[<a href=manage_sendItemAlluser.php?gid=<?=$gid?>>전체회원에게 보내기</a>]</td></tr></table>
	<table class="table table-bordered">
		<tr>
			<td>아이디추가<br><font color=cccccc>text파일에<br> 회원번호 보상\r\n</font></td>
			<td field="fileData" editor='{"type":"readFileData","uploadFile":"./processupload/csvupload.php","uploadDir":"../images/", "removeFile":true}'></td>
		</tr>
		<!--tr>
			<td>아이디추가</td>
			<td field="addByID" editor='{"type":"addByID"}'></td>
		</tr-->
		<tr>
			<td>대상회원목록</td>
			<td field="memberList" editor='{"type":"viewer_memberList"}'></td>
		</tr>
		<tr>
			<td>교환ID<br>한가지보상항목<br> 선택후 값은 0입력</td>
			<td field="exchangeID" editor='{"type":"exchangemaker"}'></td>
		</tr>
		<tr>
			<td>변경방식</td>
			<td field="sendType" editor='{"type":"editor_sendType"}'></td>
		</tr>
		<tr>
			<td colspan=2 align=center><button class="LQSendButton btn btn-primary" func="updateWithLQForm2">보내기</button></td>
		</tr>
	</table>


</td></tr>
</table>
</div>


<?php
include "manage_footer.php";
?>

