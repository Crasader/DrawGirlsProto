
function ExchangeEditor(value,option){
	this.exchangeID = value;
	this.option = s2j(option);
	this.editor = $("<div>").addClass("LQEditor");
	this.editTable = $("<table>").addClass("table table-boarded");
	this.editor.on("saveCallBack",{"obj":this},function(event){
		event.data.obj.saveReward();
		return false;
	})
	this.addNewReward = function(value,count){
		var editTableTR2 = $("<tr>").attr("datarow","");
		var editTableTD2 = $("<td>").appendTo(editTableTR2).append(propSelect(value));
		var editTableTD3 = $("<td>").appendTo(editTableTR2);
		$("<input>").addClass("form-control form-control-inline").attr("placeholder","value").attr("value",count).appendTo(editTableTD3);
		var editTableTD4 = $("<td>").appendTo(editTableTR2);
		$("<button>").append("-").addClass("btn btn-danger").appendTo(editTableTD4).on("click",{"obj":editTableTR2},function(event){event.data.obj.remove();});
		this.editTable.find("tr:last").before(editTableTR2);
	}
	this.saveReward = function(){
		var obj = this;
		var dataRow = [];
		//var statsValue = obj.option["rowData"][obj.option["statsValueField"]
		this.editTable.find("tr[datarow]").each(function(){
			dataRow.push({"type":$(this).find("select").val(),"count":$(this).find("input").val(),"statsID":obj.option["statsID"],"content":obj.option["content"]});
		});

		log(j2s(dataRow));

		//ajax로 생성
		$.ajax({
	    url : "dataManager2.php", 
	    data : {"dbMode":"custom","dbFunc":"saveExchangeID","dbClass":"Exchange","param":j2s({"id":this.exchangeID,"list":dataRow})},
	    dataType : "json", 
	    type : "post",
	    success : function(data){
	    	printLog(data);
	    	log(data);
	    	if(data["result"]["code"]==1){

	    		obj.exchangeID = data["exchangeID"];
	    		obj.editor.attr("value",obj.exchangeID);
	    		obj.createRewardEditor(data["list"]);
	    	}else{
	    		alert("error","교환정보가 제대로 저장되지 않았습니다. 다시 시도해주세요"+j2s(data));
	    	}
	    },
	    error : function(e){
	    	alert("error","교환정보가 제대로 저장되지 않았습니다. 다시 시도해주세요"+j2s(e));
	    }
		});

	}
	this.createRewardEditor = function(list){
		var editor = this.editor;
		var editTable = this.editTable;
		editor.html("");
		editTable.html("");

		var editTableTR = $("<tr>").appendTo(editTable);
		var editTableTD = $("<td>").attr("colspan",2).appendTo(editTableTR);
		$("<label>").append("교환ID : "+this.exchangeID).appendTo(editTableTD);
		var editTableTD2 = $("<td>").appendTo(editTableTR).attr("align","right");
		$("<button>").append("다른ID").addClass("btn btn-info").appendTo(editTableTD2).on("click",{"obj":this},function(event){ event.data.obj.exchangeID=null;event.data.obj.createForm()});


		var editTableTR2 = $("<tr>").appendTo(editTable);
		var editTableTD3 = $("<td>").attr("colspan",3).appendTo(editTableTR2).attr("align","center");
		$("<button>").append("교환 항목 추가").addClass("btn btn-primary").appendTo(editTableTD3).on("click",{"obj":this},function(event){event.data.obj.addNewReward("",0)});

		if(list){
			for(var i =0 ; i<list.length; i++){
				this.addNewReward(list[i]["type"],list[i]["count"]);
			}
		}else{
			this.addNewReward("g",0);
		}
		editor.append(editTable);


	}

	this.createExchangeID =  function(){
		var obj = this;
		var editor = this.editor;
		if(!this.exchangeID){
			this.exchangeID = this.editor.find("#myID").val();
			if(!this.exchangeID){
				alert("신규교환생성","랜덤으로 신규 교환ID를 생성합니다.",null,{"확인":function(){obj.ajaxMakeExchangeID();$(this).dialog("close");}});
			}else{
				alert("교환정보로드","입력한 교환ID로 기존교환정보를 로드합니다.<br> 이때 교환정보를 수정할경우 데이터 무결성을 보장하지 않습니다.<br> 해당정보가 없을경우 해당ID로 새로운 교환을 생성합니다.",null,{"확인":function(){obj.ajaxMakeExchangeID();$(this).dialog("close");}});
			}
		}else{
			this.ajaxMakeExchangeID();
		}
	}

	this.ajaxMakeExchangeID = function(){
		var obj=this;
		//ajax로 생성
		$.ajax({
	    url : "dataManager2.php", 
	    data : {"dbMode":"custom","dbFunc":"makeExchangeIDByRandom","dbClass":"Exchange","param":j2s({"id":this.exchangeID})},
	    dataType : "json", 
	    type : "post",
	    success : function(data){
	    	printLog(data);
	    	log(data);
	    	if(data["result"]["code"]==1){

	    		obj.exchangeID = data["exchangeID"];
	    		obj.editor.attr("value",obj.exchangeID);
	    		obj.createRewardEditor(data["list"]);

	    	}else{
	    		alert("error",data["result"]["msg"],function(){obj.exchangeID=null;obj.createForm();$(this).dialog("close");});
	    	}
	    },
	    error : function(e){
	    	alert("error",j2s(e),function(){obj.exchangeID=null;obj.createForm();$(this).dialog("close");});
	    }
		});
	}

	this.createForm = function(){

		var obj = this;
		var editor = this.editor;
		var editTable = this.editTable;
		editor.html("");
		editTable.html("");

		if(!this.exchangeID){
			//추가모드
			$("<input>").attr("id","myID").addClass("form-control").attr("placeholder","교환ID").appendTo(this.editor);
			$("<button>").addClass("btn btn-primary").append("교환정보로드").appendTo(this.editor).on("click",{"obj":obj},function(event){event.data.obj.createExchangeID()});
			$("<button>").addClass("btn btn-info").append("교환자동생성").appendTo(this.editor).on("click",{"obj":obj},function(event){event.data.obj.createExchangeID()});
			$("<br>").appendTo(this.editor);
			$("<br>").appendTo(this.editor);			
		}else{
			//바로수정모드
			this.createExchangeID();
		}
		return this.editor;
	}


}

var exchangemaker = function(value,option){
	var newobj = new ExchangeEditor(value,option);
	var neditor = newobj.createForm();
	neditor.attr("editor",j2s(option));
	return neditor;
}
var exchangemaker_value = function(obj){
	return obj.attr("value");
}

var propertyViewer = function(value,option){
	return propChange(value);
}

var arrayViewer = function(value,option){

	value = s2j(value);
	var table = $("<table>").addClass("table").addClass("table-bordered");	

		
	for(var i in value){
		var tr1 = $("<tr>").appendTo(table);
		$("<td>").html(value[i]).appendTo(tr1);
	}

	return table;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////

function ExchangeViewer(value,option){
	this.exchangeID = value;
	this.option = s2j(option);
	this.editor = $("<div>");
	this.editTable = $("<table>").addClass("table table-boarded");
	
	this.addNewReward = function(value,count){
		var editTableTR2 = $("<tr>").attr("datarow","");
		var editTableTD2 = $("<td>").appendTo(editTableTR2).append(propChange(value));
		var editTableTD3 = $("<td>").appendTo(editTableTR2).append(count);
		this.editTable.find("tr:last").before(editTableTR2);
	}

	this.createRewardEditor = function(list){
		var editor = this.editor;
		var editTable = this.editTable;
		editor.html("");
		editTable.html("");

		var editTableTR = $("<tr>").appendTo(editTable);
		var editTableTD = $("<td>").attr("colspan",2).appendTo(editTableTR);
		$("<label>").append("교환ID : "+this.exchangeID).appendTo(editTableTD);
		var editTableTR2 = $("<tr>").appendTo(editTable);
		var editTableTD3 = $("<td>").attr("colspan",2).appendTo(editTableTR2).attr("align","center");
		
		if(list){
			for(var i =0 ; i<list.length; i++){
				this.addNewReward(list[i]["type"],list[i]["count"]);
			}
		}else{
			this.addNewReward("g",0);
		}
		editor.append(editTable);


	}

	
	this.ajaxExchangeIDViewer = function(){
		var obj=this;
		//ajax로 생성
		$.ajax({
	    url : "dataManager2.php", 
	    data : {"dbMode":"custom","dbFunc":"exchangeViewer","dbClass":"Exchange","param":j2s({"id":this.exchangeID})},
	    dataType : "json", 
	    type : "post",
	    success : function(data){
	    	printLog(data);
	    	log(data);
	    	if(data["result"]["code"]==1){

	    		obj.exchangeID = data["exchangeID"];
	    		obj.editor.attr("value",obj.exchangeID);
	    		obj.createRewardEditor(data["list"]);

	    	}else{
	    		alert("error",data["result"]["msg"],function(){obj.exchangeID=null;obj.createForm();$(this).dialog("close");});
	    	}
	    },
	    error : function(e){
	    	alert("error",j2s(e),function(){obj.exchangeID=null;obj.createForm();$(this).dialog("close");});
	    }
		});
	}

	this.createForm = function(){

		var obj = this;
		var editor = this.editor;
		var editTable = this.editTable;
		editor.html("");
		editTable.html("");

		if(!this.exchangeID){
						
		}else{
			//바로수정모드
			this.ajaxExchangeIDViewer();
		}
		return this.editor;
	}


}

var exchangeviewer = function(value,option){
	var newobj = new ExchangeViewer(value,option);
	var neditor = newobj.createForm();
	neditor.attr("editor",j2s(option));
	return neditor;
}



////////////////////////////////////////////////////////////////////
// LanguageEditor
////////////////////////////////////////////////////////////////////

function LanguageEditor(value,option){
	this.exchangeID = value;
	this.list = s2j(value);
	this.option = s2j(option);
	this.editor = $("<div>").addClass("LQEditor");
	this.editTable = $("<table>").addClass("table table-boarded");
	
	this.addNewReward = function(lang,content){
		var editTableTR2 = $("<tr>").attr("datarow","");
		var editTableTD2 = $("<td>").appendTo(editTableTR2).append(editorSelector({"type":"text","placeholder":"lang code"},lang));
		
		if(!this.option["element"])this.option["element"]={"type":"text","placeholder":"content"};

		var editTableTD3 =$("<td>").appendTo(editTableTR2).append(editorSelector(this.option["element"],content));
		
		var editTableTD4 = $("<td>").appendTo(editTableTR2);
		$("<button>").append("-").addClass("btn btn-danger").appendTo(editTableTD4).on("click",{"obj":editTableTR2},function(event){event.data.obj.remove();});
		this.editTable.find("tr:last").before(editTableTR2);
	}

	this.createForm = function(){
		var editor = this.editor;
		var editTable = this.editTable;
		editor.html("");
		editTable.html("");


		var editTableTR2 = $("<tr>").appendTo(editTable);
		var editTableTD3 = $("<td>").attr("colspan",3).appendTo(editTableTR2).attr("align","center");
		$("<button>").append("언어추가").addClass("btn btn-primary").appendTo(editTableTD3).on("click",{"obj":this},function(event){event.data.obj.addNewReward("","")});

		if(this.list){
			for(var lang in this.list){
					this.addNewReward(lang,this.list[lang]);	
			}
		}else{
			this.addNewReward("en","");
		}
		editor.append(editTable);

		return this.editor;
	}
}

var languageEditor = function(value,option){
	var newobj = new LanguageEditor(value,option);
	var neditor = newobj.createForm();
	neditor.attr("editor",j2s(option));
	return neditor;
}
var languageEditor_value = function(obj){
	var dataRow = {};
	obj.find("tr[datarow]").each(function(){
		var veditor = $(this).find(".LQEditor:last");
		var vv = getObjValue(veditor);
		dataRow[$(this).find(".LQEditor:first").val()]=vv;
	});

	return dataRow;
}


var languageViewer = function(value,option){
	value = s2j(value);
	// var pushData = '<table class="table table-boarded">';
	// for(i in value){
	// 		pushData+='<tr><td>'+i+'</td>';
	// 		pushData+='<td>'+nl2br(value[i])+'</td></tr>';
	// }
	// pushData+="</table>";

	return nl2br(value["ko"]);
}

var cuponCodeViewer = function(value,option){
	return value.substr(0,4)+"-"+value.substr(4,4)+"-"+value.substr(8,4);
}

var textareaViewer = function(obj,value){
	return "<textarea rows=3 cols=50>"+j2s(obj)+"</textarea>";
}

var imageInfoViewer = function(value,option){
	data = s2j(value);
	return '<img src='+data["img"]+' width=100>';
}
/////////////////////////////////////////////////////


var adminPermissionView = function(value,opotion){
	value = s2j(value);
	var perList = ["readAdmin","readUser","readEvent","readGame"];
	var nameList = ["접속자관리","유저관리","이벤트관리","게임설정"];
	
	var table = $("<table>").addClass("table").addClass("table-bordered");
	var titleRow = $("<tr>");
	var valueRow = $("<tr>");
	for(var i=0;i<perList.length;i++){
		$("<td>").html(nameList[i]).appendTo(titleRow).attr("align","center");
		var v = "X";
		
		if(value[perList[i]]=="true")v="O";
		
		
		$("<td>").html(v).appendTo(valueRow).attr("align","center");
		//var cbox = $("<input>").attr("type","checkbox").attr("name",perList[i]).appendTo(td);
		//if(value[perList[i]]=="true")cbox.attr("checked",true);
	}

	table.append(titleRow).append(valueRow);
	return table;

}


var adminPermissionEditor = function(value,option){
	value = s2j(value);
	var perList = ["readAdmin","readUser","readEvent","readGame"];
	var nameList = ["접속자관리","유저관리","이벤트관리","게임설정"];

	var table = $("<table>").addClass("table").addClass("table-bordered").addClass("LQEditor").attr("editor",j2s(option));
	var titleRow = $("<tr>");
	var valueRow = $("<tr>");
	for(var i=0;i<perList.length;i++){
		$("<td>").html(nameList[i]).appendTo(titleRow).attr("align","center");
		var td = $("<td>").appendTo(valueRow).attr("align","center");
		var cbox = $("<input>").attr("type","checkbox").attr("name",perList[i]).appendTo(td).addClass("apCheck");
		if(value[perList[i]]=="true")cbox.attr("checked",true);
	}

	table.append(titleRow).append(valueRow);
	return table;

}

var adminPermissionEditor_value = function(obj){
	var r = {};
	obj.find(".apCheck").each(function(l){	
		var n = $(this).attr("name");
		if($(this).is(":checked"))r[n]="true";
		else r[n]="false";
	});
	return r;
}

var showPuzzleImg = function(value,option){
	data = s2j(value);
	if(!data)return "";
	if(!data["image"]) return "";
	return '<img src='+data["image"]+' height=100>';
}

var puzzleOpenConditionViewer = function(value,option){
	value = s2j(value);
	
	var table = $("<table>").addClass("table").addClass("table-bordered");	

	if(typeof(value)!="object" || value.length==0) table.html("<tr><td>없음</td></tr>");
	for(var i in value){
		var row = $("<tr>").appendTo(table);
		var cell = $("<td>").appendTo(row);
		var conStr = "";
		for(var j in value[i]){
			if(value[i][j]["type"]=="s")conStr+="별 "+value[i][j]["value"]+"개 이상 보유";
			if(value[i][j]["type"]=="p")conStr+=value[i][j]["value"]+"번 퍼즐 클리어";
			if(value[i][j]["type"]=="r")conStr+=value[i][j]["value"]+"젬 으로 구매";
			if(value[i][j]["type"]=="w")conStr+=value[i][j]["weekday"]+"번째 요일 "+value[i][j]["s"]+"~"+value[i][j]["e"];
			if(value[i][j]["type"]=="d")conStr+=value[i][j]["s"]+"~"+value[i][j]["e"];
			if(j<value[i].length-1)conStr+=" & ";
		}
		cell.html(conStr);
	}

	return table;


}

var puzzleCardRewradViewer = function(value,option){
	value = s2j(value);
	
	var table = $("<table>").addClass("table").addClass("table-bordered");	
	var nRow = $("<tr>").html("<td>최초완성</td><td>"+value["normal"]+"</td>").appendTo(table);
	var pRow = $("<tr>").html("<td>퍼펙트완성</td><td>"+value["perfect"]+"</td>").appendTo(table);

	return table;
}

var keyArrayViewer = function(value,option){

	value = s2j(value);
	option = s2j(option);
	var table = $("<table>").addClass("table").addClass("table-bordered");	

	var tr1 = $("<tr>").appendTo(table);
	var tr2 = $("<tr>").appendTo(table);
		
	for(var i in option["title"]){
		$("<td>").html(option["title"][i]).appendTo(tr1);
		$("<td>").html(value[i]).appendTo(tr2);
	}

	return table;
}

var keyArrayEditor = function(value,option){

	value = s2j(value);
	option = s2j(option);
	var table = $("<table>").addClass("table").addClass("table-bordered").addClass("LQEditor").attr("editor",j2s(option));	

	var tr1 = $("<tr>").appendTo(table);
	var tr2 = $("<tr>").appendTo(table);
		
	for(var i in option["title"]){
		
		var editor = editorSelector('{"type":"text"}', value[i]);
		$("<td>").html(option["title"][i]).appendTo(tr1);
		$("<td>").append(editor).appendTo(tr2);

	}

	return table;
}

var keyArrayEditor_value = function(obj){
	var dataRow = {};
	var value=[];
	var i=0;
	obj.find(".LQEditor").each(function(){
		var veditor = $(this);
		var vv = getObjValue(veditor);
		value[i]=vv;
		i++;
	});

	return value;
}
///////////////////////
///////////////////////  file upload
///////////////////////

function FileUploader(value,option){
	this.exchangeID = value;
	this.list = s2j(value);
	this.option = s2j(option);
	this.editor = $("<div>").addClass("LQEditor");
	this.editTable = $("<table>").addClass("table table-boarded");
	this.callbackFunc = null;
	this.createForm = function(){
		var editor = this.editor;
		var editTable = this.editTable;
		editor.html("");
		var obj=this;

		var form = $("<form>").attr("action",this.option["uploadFile"]).attr("method","post").attr("enctype","multipart/form-data").attr("id","MyUploadForm").appendTo(editor);
		
		//form.on("submit",{"obj":obj},function(event){return event.data.obj.submitFunc();});
		form.submit({"obj":obj},this.submit);
		$("<input>").attr("name","FileInput").attr("id","FileInput").attr("type","file").addClass("form-control").appendTo(form);
		$("<input>").attr("value","Upload").attr("id","submit-btn").attr("type","submit").addClass("btn btn-primary").appendTo(form);
		$("<img>").attr("id","loading-img").css("display","none").appendTo(form);

		$("<div>").attr("id","progressbox").append($("<div>").attr("id","progressbar")).append($("<div>").attr("id","statustxt")).appendTo(editor);
		
		return this.editor;
	}

	this.submit = function(event){
			var obj = event.data.obj;
		    var options = { 
		    data : obj.option,
		    dataType : "json",
		    type:"POST",
		    beforeSubmit:  function(){obj.beforeSubmit();},  // pre-submit callback 
		    success:       function(responseText, statusText, xhr, $form){obj.afterSuccess(responseText, statusText, xhr, $form);},  // post-submit callback 
		    uploadProgress: function(event, position, total, percentComplete){obj.OnProgress(event, position, total, percentComplete);}, //upload progress callback 
		    resetForm: true        // reset the form after successful submit 
		    }; 
			
			$(this).ajaxSubmit(options);        

			return false;   
	}

	this.afterSuccess = function(responseText, statusText, xhr, $form)
	{
		this.editor.find('#submit-btn').show(); //hide submit button
		this.editor.find('#loading-img').hide(); //hide submit button
		this.editor.find('#progressbox').delay( 1000 ).fadeOut(); //hide progress bar
		if(this.callbackFunc)this.callbackFunc(responseText);
	}

	this.beforeSubmit = function(){
	    //check whether browser fully supports all File API
	   // var obj = this;
	   	if (window.File && window.FileReader && window.FileList && window.Blob)
		{

		    if( !this.editor.find('#FileInput').val()) //check empty input filed
		    {
		       	alert("Error","파일을 선택해주세요");
		        return false
		    }

		    var fsize = this.editor.find('#FileInput')[0].files[0].size; //get file size
		    var ftype = this.editor.find('#FileInput')[0].files[0].type; // get file type


		    //allow file types 
		    // switch(ftype)
		    // {
		    // 		case 'image/png':
		    // 		case 'image/jpg':
		    // 		case 'image/gif':
		    //     case 'application/xls':
		    //         break;
		    //     default:
		    //        	alert("error","<b>"+ftype+"</b> Unsupported file type!");
		    //         return false
		    // }

		    //Allowed file size is less than 5 MB (1048576)
		    if(fsize>52428800) 
		    {
		    	alert("error","<b>"+this.bytesToSize(fsize) +"</b> Too big file! <br />File is too big, it should be less than 50 MB.")
		        return false;
		    }

		    this.editor.find('#submit-btn').hide(); //hide submit button
		    this.editor.find('#loading-img').show(); //hide submit button
		}
		else
		{
		    //Output error to older unsupported browsers that doesn't support HTML5 File API
		   	alert("error","Please upgrade your browser, because your current browser lacks some new features we need!");
		    return false;
		}
	}


	this.OnProgress = function(event, position, total, percentComplete)
	{
	   //Progress bar
		this.editor.find('#progressbox').show();
		this.editor.find('#progressbar').width(percentComplete + '%') //update progressbar percent complete
		this.editor.find('#statustxt').html(percentComplete + '%'); //update status text
		if(percentComplete>50)
	    {
	        this.editor.find('#statustxt').css('color','#000'); //change status text to white after 50%
	    }
	}

	//function to format bites bit.ly/19yoIPO
	this.bytesToSize = function(bytes) {
		var sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
		if (bytes == 0) return '0 Bytes';
		var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
		return Math.round(bytes / Math.pow(1024, i), 2) + ' ' + sizes[i];
	} 

}




	var delegator2 = [];
	var delekeyCnt2 = 1;
	var addDelegate2 = function(obj){
		delegator2[delekeyCnt2]=obj;
		delekeyCnt2++;
		return delekeyCnt2-1;
	}
	

		
	var imageSelector = function(value,option){
		var img;
		
		if(value)img = "src='"+value+"'";
		else img=""
		
		if(value)_val = value;
		else _val="";

		var newEditor = $("<div>").addClass("LQEditor").attr("editor",j2s(option)).attr("value",_val);
		$("<input>").attr("type","text").addClass("imageSelector").appendTo(newEditor).val(_val).on("input",function(){$(this).parent().find(".imageView").attr("src",$(this).val());});
		$("<input>").attr("type","button").attr("value","선택하기").addClass("imageSelectorBtn").attr("width",100).appendTo(newEditor);
		$("<img>").addClass("imageView").attr("width","100").appendTo(newEditor).attr("src",_val);		
		return newEditor;
	}

	// var imageSelector_value = function(obj){
	// 	log("testtesttest-> "+obj.find(".imageSelector").val());
	// 	return obj.find(".imageSelector").val();
	// }
	var imageSelector_value = function(obj){
		return obj.attr("value");
	}	
	var resourceSelector = function(value,option){	

		var img;
		
		if(value)img = "src='"+value+"'";
		else img=""
		
		if(value)_val = value;
		else _val="";

		var newEditor = $("<div>").addClass("LQEditor").attr("editor",j2s(option)).attr("value",_val);
		$("<input>").attr("type","text").addClass("resourceSelector").appendTo(newEditor).val(_val).on("input",function(){$(this).parent().find(".imageView").attr("src",$(this).val());});
		$("<input>").attr("type","button").attr("value","선택하기").addClass("resourceSelectorBtn").attr("width",100).appendTo(newEditor);
		$("<img>").addClass("imageView").attr("width","100").appendTo(newEditor).attr("src",_val);		
		return newEditor;
	}
	
	var resourceSelector_value = function(obj){
		return obj.attr("value");
	}

	var imageViewer = function(value,option){
		var img;
		
		if(value)img = "<img src='"+value+"' width=100>";
		else img=""
		
		return img;
	}
	
	window.changeJsonFormInput = function(input,value,size) {
		log("changeJsonFormInput:"+input+",value:"+value+",size:"+size);
		var obj = delegator2[input];
		
		
		obj.find(".imageView").attr("src",value);
		//obj.find(".imageSelector").attr("value",value);
		obj.find(".imageSelector").val(value);
		obj.attr("value",value);
		//obj.val(value);
		
		obj.parent().parent().parent().find("td[field=size]").children("input").val(parseInt(size))
		
		
		
		//$(input).val(value).change();
	}

	window.changeJsonFormInputByResource = function(input,value,size) {
		log("changeJsonFormInput:"+input+",value:"+value+",size:"+size);
		var obj = delegator2[input];
		
		
		obj.find(".imageView").attr("src",value);
		//obj.find(".imageSelector").attr("value",value);
		obj.find(".resourceSelector").val(value);
		obj.attr("value",value);

		
		if(obj.attr("getSize")=="true"){
			obj.parent().parent().parent().find("td[field=size]").children("input").val(parseInt(size))
		}
		
		
		//$(input).val(value).change();
	}

	$(document).ready(function(){

		$('body').on('click','.imageSelectorBtn',function(){

			var delkey = addDelegate2($(this).parent());
			window.open('./admin_images.php?gid=nothing&delkey='+delkey,'imageselector','width=1000 height=800 menubar=no status=no');
		});
		
		$('body').on('click','.resourceSelectorBtn',function(){

			var delkey = addDelegate2($(this).parent());
			window.open('./admin_resources.php?gid=nothing&delkey='+delkey,'resourceselector','width=1000 height=800 menubar=no status=no');
		});
	});