/*

1.basic-editor  "datatype":"int","default":"defaultvalue"
a.text
{"type":"text"}

b.textarea
{"type":"textarea"}

c.select
{"type":"select","element":["gold","ruby","social","coin"]}

d.bool
{"type":"bool"}


2.multi-editor
f.array
{"type":"array","element":ANOTHEREDITOR}              , ANOTHEREDITOR ex {"type":"text","datatype":"int"}

g.dictionary
{"type":"dictionary","element":[ANOTHEREDITOR,ANOTHEREDITOR... ]}   , ANOTHEREDITOR ex {"field":"max","type":"text","datatype":"int"}, add field

e.table
{"type":"table","element":[ANOTHEREDITOR,ANOTHEREDITOR... ]} , ANOTHEREDITOR ex {"field":"shape","title":"모양","type":"select","list":["circle","snake"]}


3.viewer   option["cut"]





*/








// 테이블 만들때 기억해야할 상수
/*
	VARIABLEKEY - dictionary에서 key명은 유저가 정하면서 형식이 정해진 필드를 추가할경우, dictionary 옵션의 field에 사용
*/


var timeToDate = function (timestamp, fmt) {
	fmt = fmt || "%Y/%M/%d %H:%m:%s"

	var date = new Date(Number(timestamp)*1000)

    function pad(value) {
        return (value.toString().length < 2) ? '0' + value : value;
    }

    return fmt.replace(/%([a-zA-Z])/g, function (_, fmtCode) {
        switch (fmtCode) {
        case 'Y':
            return date.getUTCFullYear();
        case 'M':
            return pad(date.getUTCMonth() + 1);
        case 'd':
            return pad(date.getUTCDate());
        case 'H':
            return pad(date.getUTCHours());
        case 'm':
            return pad(date.getUTCMinutes());
        case 's':
            return pad(date.getUTCSeconds());
        default:
            throw new Error('Unsupported format code: ' + fmtCode);
        }
    });
}


var datetimeFormat = function (datetime, fmt) {
	if(typeof(datetime)=="undefined")return "";
	if(!datetime)return "";
	if(datetime=="0" || datetime==0)return "";
	if(datetime<99999999)return datetime;
	if(typeof(datetime)=="number")datetime=datetime.toString();

	var y = datetime.substr(2,2);
	var Y = datetime.substr(0,4);
	var m = datetime.substr(4,2);
	var d = datetime.substr(6,2);
	var h = datetime.substr(8,2);
	var i = datetime.substr(10,2);
	var s = datetime.substr(12,2);

	fmt=fmt.replace("Y",Y);
	fmt=fmt.replace("y",y);
	fmt=fmt.replace("m",m);
	fmt=fmt.replace("d",d);
	fmt=fmt.replace("h",h);
	fmt=fmt.replace("i",i);
	fmt=fmt.replace("s",s);

	//log("datetimeFormat-"+datetime+"-"+fmt);
	return fmt;
}

var log = function(data){
	console.log(j2s(data));
};

var printLog=function(serverResult){
	var rlist = s2j(serverResult);
	rlist = rlist["log"]
	log("[================================================== log start ==============================================]");
	if(rlist && typeof(rlist) == "object"){
		for(var i=0;i<rlist.length;i++){
			log(rlist[i]);
		}
	}
	log("[================================================== log end ==============================================]");
}

var delegator = [];
var delekeyCnt = 1;

var addDelegate = function(obj,func){
	var deleSet = {"object":obj,"func":func};
	delegator[delekeyCnt]=deleSet;
	delekeyCnt++;
	return delekeyCnt-1;
}


window.LQSelectedData = function(delekey,value) {
	var deleSet = delegator[delekey];
	var func = deleSet["func"];
	func(deleSet["object"],value);
}

var openDataSelector = function(obj,func,mode){
	func = eval(func);
	delkey = addDelegate(obj,func);
	window.open('./dataSelector.php?gid='+gid+'&delkey='+delkey+'&mode='+mode,'LQDataselector'+delkey,'width=500 height=500 menubar=no status=no');
}

var getFieldInfo = function(obj){
	var result = {};
	var depth=0;
	var selectedObj;


	selectedObj = obj;
	depth=0;
	while(1){
		if(selectedObj.hasClass("LQDataTable") || selectedObj.hasClass("LQDataForm")){
			result["table"]=selectedObj;
			result["row"]=selectedObj;
			break;
		}else{
			selectedObj = selectedObj.parent();
			if(depth>10){
				//log("dont find table");
				return null;
			}
		}
		
		depth++;
	}

	selectedObj = obj;
	depth=0;
	while(1){
		//log("find row");
		if(selectedObj.hasClass("LQDataRow") || selectedObj.hasClass("LQDataAddRow")){
			result["row"]=selectedObj;
			break;
		}else{
			selectedObj = selectedObj.parent();
			
			if(depth>10){
				//log("dont find row");
				break;
			}
		}
		
		depth++;
	}

	selectedObj = obj;
	depth=0;
	while(1){
		if(selectedObj.attr("field") || selectedObj.hasClass("LQDataCell") || selectedObj.hasClass("LQDataAddCell") || selectedObj.hasClass("LQManageCell")){
			result["cell"]=selectedObj;
			break;
		}else{
			selectedObj = selectedObj.parent();
			if(depth>10)break;
		}
		
		depth++;
	}			
	
	if(result["table"]){
		result["dbSource"]=result["table"].attr("dbSource");
		result["dbTable"] = result["table"].attr("dbTable");
		result["tableName"] = result["table"].attr("name");
		result["dbSort"] = result["table"].attr("dbSort");
		result["dbWhere"] = result["table"].attr("dbWhere");
		result["defaultData"]=s2j(result["table"].attr("defaultData"));
		if(!result["dbWhere"])result["dbWhere"]={};
		result["dbLimit"] = result["table"].attr("dbLimit");
		if(!result["dbLimit"])result["dbLimit"]=30;
		result["dbLoadParam"] =j2s({"name":result["tableName"],"where":s2j(result["dbWhere"]),"limit":s2j(result["dbLimit"]),"sort":s2j(result["dbSort"])});
		dbFunc = s2j(result["table"].attr("dbFunc"));

		if(dbFunc){
			result["selectFunc"] = dbFunc["select"];
			result["writeFunc"] = dbFunc["write"];
			result["updateFunc"] = dbFunc["update"];
			result["deleteFunc"] = dbFunc["delete"];
			result["loadFunc"] = dbFunc["load"];
		}

		result["dbClass"] = result["table"].attr("dbClass");
		result["commenter"]=result["table"].attr("commenter");
		result["alwaysList"] = [];
		result["table"].find("[always]").each(function(){
			result["alwaysList"].push($(this).attr("field"));
		});
		result["primaryKey"] = result["table"].find("[primary]").attr("field");
		//if(typeof(result["primaryKey"])=="undefined")result["primaryKey"]=result["table"].find("td[primary]").attr("field");

		result["manageHeader"]=result["table"].find("th[manage]:first");
		if(result["manageHeader"].attr("manage")==undefined){
			result["manageHeader"]= result["table"].find("td[manage]:first");
		}
		result["manage"]= result["manageHeader"].attr("manage");
		
	}
	if(result["row"]){
		result["rowData"]=s2j(result["row"].attr("data"));
		result["shardIndex"]=result["row"].attr("shardIndex");
		if(typeof(result["rowData"])=="undefined")result["rowData"]=s2j(result["table"].attr("data"));

		try{
			result["primaryValue"]=result["rowData"][result["primaryKey"]];
		}catch(e){
		}
	}
	
	if(result["cell"]){
		result["field"]=result["cell"].attr("field");
		result["editorObj"] = result["cell"].find(".LQEditor:first");
		result["fieldHeader"]=result["table"].find("th[field="+result["field"]+"]:first");
		if(result["fieldHeader"])result["title"]=result["fieldHeader"].html();
		try{
			result["isPrimary"]=(typeof(result["fieldHeader"].attr("primary"))=="undefined")?false:true;
		}catch(e){
			result["isPrimary"]=false;
		}
		try{
			result["isVirtual"]=(typeof(result["fieldHeader"].attr("virtual"))=="undefined")?false:true;
		}catch(e){
			result["isVirtual"]=false;
		}

		if(result["isVirtual"]==false){
			try{
				result["isVirtual"]=(typeof(result["cell"].attr("virtual"))=="undefined")?false:true;
			}catch(e){
				result["isVirtual"]=false;
			}
		}
		// if(result["fieldHeader"]==undefined || !result["fieldHeader"]){
		// 	result["fieldHeader"]=result["cell"];
		// }
		//result["class"]=result["cell"].attr("class");
		result["editor"]=result["fieldHeader"].attr("editor");
		if(typeof(result["editor"])=="undefined")result["editor"]=result["cell"].attr("editor");
		result["viewer"]=result["fieldHeader"].attr("viewer");
		if(typeof(result["viewer"])=="undefined")result["viewer"]=result["cell"].attr("viewer");
		// log("getFieldInfo, editorValue "+result["editorObj"].attr("editor"));
		// log("getFieldInfo, viewer "+result["viewer"]);
		// log("getFieldInfo, editor "+result["editor"]);
		// log("getFieldInfo, fieldheader "+result["fieldHeader"].attr("class"));

		result["editorValue"]=getObjValue(result["editorObj"]);


		try{
			result["data"]=result["rowData"][result["field"]];				
		}catch(e){
			result["data"]=undefined;		
		}
	}
	
	
	return result;
	
	
}

var gf = getFieldInfo;

var contextSelectedCell=null;

var nl2br = function(str, is_xhtml) {
		var breakTag = (is_xhtml || typeof is_xhtml === 'undefined') ? '<br ' + '/>' : '<br>'; // Adjust comment to avoid issue on phpjs.org display

		return (str + '').replace(/([^>\r\n]?)(\r\n|\n\r|\r|\n)/g, '$1' + breakTag + '$2');
}	

var s2j = function(text){
	
	var start = "";
	try{
		text2 = j2s(text);
		start = text2.substr(0,1);
	}catch(err){
		start = "";
	}

	var r = text;
	try
	{	
	
		if(start=="{" || start=="["){
			r = JSON.parse(text);
		}

	}catch(err){
		//log("----json parse error : "+text);
		r = text;
	}


	return r;
}

var j2s = function(obj){
if(typeof(obj)=="string")return obj;
if(typeof(obj)=="undefined")return undefined;
	try
	{
		var r = JSON.stringify(obj);
	}catch(err){
		//log("----json parse error : "+obj);
		r = obj;
	}
	return r;
}


var getObjValue = function(obj){
	//obj = s2j(obj);

	// log("getObjValue --------------------------------------- start ---------------");
	// log("getObjValue, obj is "+j2s(obj));
	var editor = s2j(obj.attr("editor"));
	// log("getObjValue, obj.attr(editor) is "+j2s(editor));
	var editorName = undefined;
	if(editor){
		editorName = editor["type"];
		// log("getObjValue, ok set editorName "+editorName);
	}

	var vFunc = undefined;
	var r=undefined;
	
	if(editorName != undefined){
		eval("if(typeof editorFunc_"+editorName+"_value != 'undefined') vFunc=editorFunc_"+editorName+"_value;");
		if(vFunc==undefined){
			eval("if(typeof "+editorName+"_value != 'undefined') vFunc="+editorName+"_value;");
		}
	}
	
	// log("getObjValue, editorName is "+editorName);
	if(typeof(vFunc)=="function"){
		r=vFunc(obj);
	}
	else{
		r=j2s(obj.val());
	}
	var datatype = "";
	if(typeof(editor)!="undefined" && typeof(editor["datatype"])!="undefined")datatype = editor["datatype"];
	
	
	if(datatype=="int" || datatype=="bool"){
		//log("s to number!!");
		r=Number(r);
	}
	// log("getObjValue return is "+j2s(r)+" and datatype is"+datatype);

	// log("getObjValue --------------------------------------- end ---------------");
	return r;
	
	
/*
r = obj.val();
	if(r == undefined){
		r = obj.attr("value");
	}
	
	if(r!=undefined && r!=""){
		if(obj.attr("datatype")=="json"){
			if(typeof(r) == "string") r = s2j(r);	
		}	
	}
	
	log("getobjvalue return "+ j2s(r));
	return r;
*/

}


/// viewer ////////////////////////////////////////////////////////////////////////////
var viewerFunc_format = function(value,option){
	option=s2j(option);
	value = s2j(value);
	var result = option["format"];
	result=result.replace("[value]",j2s(value));
	
	if(option["rowData"]){
		for(var key in option["rowData"]){
			result=result.replace("["+key+"]",option["rowData"][key]);
		}
	}

	for(var key in value){
		result=result.replace("["+key+"]",value[key]);
	}
	// log("format option "+j2s(option));
	// log("format reuslt "+result);
	return result;
}


var viewerFunc_bool =function(value,option){
	var sList = [1,0];				
	var sListName = ["true","false"];	
	for(var i in sList){
		if(value==sList[i])return sListName[i];
	}
	return "false";
	
}

var viewerFunc_url = function(value,option){
	return "<a href='"+value+"' "+option["option"]+">"+value+"</a>";
}

var viewerFunc_image = function(value,option){
	if(!value)return"nothing";
	return "<img src='"+value+"' "+option["option"]+">"+value+"</a>";
}

var viewerFunc_json = function(value,option){
	if(typeof(value)=="string") return value;
	
	return j2s(value);
}
var viewerFunc_json = function(value,option){
	if(typeof(value)=="string") return value;
	
	return j2s(value);
}

var viewerFunc_datetime = function(value,option){
	if(typeof(value)=="object")value = j2s(value);
	if(typeof(option)=="string")option=s2j(option["option"]);
	if(!option["format"])option["format"]="Y/m/d h:i:s";
	return datetimeFormat(value,option["format"]);
}

var viewerFunc_timestamp = function(value,option){
	var timestamp = value;
  	var date = new Date(timestamp * 1000);
  	return date.getFullYear()+"/"+(date.getMonth()+1)+"/"+date.getDate()+" "+(date.getHours()<10?"0"+date.getHours():date.getHours())+":"+(date.getMinutes()<10?"0"+date.getMinutes():date.getMinutes())+":"+(date.getSeconds()<10?"0"+date.getSeconds():date.getSeconds());
}

function zeroFill( number, width )
{
  width -= number.toString().length;
  if ( width > 0 )
  {
    return new Array( width + (/\./.test( number ) ? 2 : 1) ).join( '0' ) + number;
  }
  return number + ""; // always return a string
}

var viewerFunc_time = function(value,option){
	if(typeof(value)=="object")value = j2s(value);
	if(typeof(value)=="undefined")value=0;
	if(typeof(option)=="string")option=s2j(option["option"]);

	value = "19870620"+zeroFill(Number(value),6);
	return datetimeFormat(value,option["format"]);
}

var viewerFunc_text = function(value,option){
	if(typeof(value)=="object")value = j2s(value);
	if(typeof(value)=="undefined")value="";
	if(typeof(option)=="string")option=s2j(option["option"]);
	if(typeof(option)=="object" && option["cut"]>0) return value.substring(0,option["cut"]);
	else return nl2br(value);
}
var viewerFunc_select = function(value,option){
	var _option = option;
	if(typeof(_option)=="string" && _option!=""){
			_option = s2j(_option);
	} 
		
	var sList = [];
	var vist = [];

	if(_option["value"])vList=_option["value"];
	else vList=_option["element"];

	if(typeof(_option) == "object" && typeof(_option["element"]) == "object")sList=_option["element"];		

	for(var i in vList){
		if(value==vList[i])return sList[i];
	}
	return "";
}
/// editor ////////////////////////////////////////////////////////////////////////////

var editorFunc_dataSelector = function(value,option){
	//var r =  $("<div>");

	var r =$("<input>").attr("type","text").attr("value",value).attr("datatype",option['datatype']).addClass("LQEditor LQDataSelectorButton form-control form-control-inline").attr("editor",j2s(option)).appendTo(r);
	//$("<input>").attr("type","button").attr("value","select").attr("func",option["func"]).attr("mode",option["mode"]).addClass("LQDataSelectorButton btn btn-info").appendTo(r);
	r="<input type=button value='select' func='"+option["func"]+"' mode='"+option["mode"]+"' class='LQDataSelectorButton btn btn-info'>";
	return r;
}


var editorFunc_text = function(value,option){
	var _option = s2j(option); 
	
	var datatype = "";
	var placeholder="String";

	if(typeof(_option)=="object"){
		if("datatype" in _option)datatype=_option["datatype"];
	}

	if(typeof(value)=="number"){
		datatype="int";
		placeholder="Number";
	}
	if(typeof(value)=="object"){
		value = j2s(value);
	}
	if(datatype=="number" || datatype=="int"){
		placeholder="Number";
	}
	if(!option["placeholder"])option["placeholder"]=placeholder;
	var neditor = $("<input>").attr("type","text").attr("value",value).addClass("LQEditor form-control form-control-inline").attr("editor",j2s(option)).attr("placeholder",option["placeholder"]);
	return neditor;
}



var editorFunc_password = function(value,option){

	var neditor = $("<input>").attr("type","password").attr("value",value).addClass("LQEditor form-control form-control-inline").attr("editor",j2s(option));
	return neditor;
	//return "<input type='text' value='"+value+"' class='LQEditor form-control form-control-inline' editor='"+j2s(option)+"'>";
}

var editorFunc_date = function(value,option){
	var neditor = $("<input>").attr("type","text").attr("value",value).addClass("LQEditor form-control form-control-inline LQDateEditor").attr("editor",j2s(option));
	neditor.datepicker({ dateFormat: "yymmdd",changeMonth: true, changeYear: true });
	return neditor;
}

var editorFunc_time = function(value,option){
	var neditor = $("<input>").attr("type","text").attr("value",value).addClass("LQEditor form-control form-control-inline LQDateEditor").attr("editor",j2s(option)).css("width",100);
	neditor.timepicker({controlType: 'select',separator:'',timeFormat: 'HHmmss',stepHour: 1,
			stepMinute: 1,
			stepSecond: 1});
	return neditor;
}

var editorFunc_datetime = function(value,option){
	var neditor = $("<input>");
	neditor.attr("type","text").attr("value",value).addClass("LQEditor form-control form-control-inline").attr("editor",j2s(option)).css("width",180);
	neditor.datetimepicker({
			controlType: 'select',
			dateFormat: 'yymmdd',
			timeFormat: 'HHmmss',
			separator:'', 
			stepHour: 1,
			stepMinute: 1,
			stepSecond: 1
		});
	//return "<input type='text' value='"+value+"' class='LQEditor form-control form-control-inline LQDateTimeEditor' editor='"+j2s(option)+"'>";
	return neditor;
}

var editorFunc_textarea = function(value,option){
	var r = $("<textarea>").addClass("LQEditor form-control form-control-inline").attr("editor",j2s(option)).attr("cols",30).attr("rows",2).text(j2s(value));
	//return "<textarea class='LQEditor form-control form-control-inline' editor='"+j2s(option)+"' cols=30 rows=2>"+j2s(value)+"</textarea>";
	return r;
}
var editorFunc_select = function(value,option){
	var _option = option;
	if(typeof(_option)=="string" && _option!=""){
			_option = s2j(_option);
	} 
	var datatype = "";
	if(typeof(_option)=="object"){
		if("datatype" in _option)datatype=_option["datatype"];
	}
	if(typeof(value)=="number")
		datatype="int";
	
	var sList = [];
	var vist = [];

	if(typeof(_option) == "object" && typeof(_option["element"]) == "object")sList=_option["element"];				
	
	var neditor = $("<select>").addClass("LQEditor form-control form-control-inline").attr("editor",j2s(option));
	var et = "<select class='LQEditor form-control form-control-inline' editor='"+j2s(option)+"'>";
	var op="";
	
	if(_option["value"])vList=_option["value"];
	else vList=_option["element"];

	for(var i in sList){

		var no = $("<option>").attr("value",vList[i]).text(sList[i]);
		if(value==vList[i])no.attr("selected",true);
		neditor.append(no);
		et+="<option value='"+vList[i]+"' "+op+">"+sList[i]+"</option>";
	}
	et += "</select>";
	//return et;
	return neditor;
}
var editorFunc_bool =function(value,option){
	var sList = [1,0];				
	var sListName = ["true","false"];	
	option["datatype"]="bool";			
	
	var neditor = $("<select>").addClass("LQEditor form-control form-control-inline").attr("editor",j2s(option));
	
	for(var i in sList){
		var no = $("<option>").attr("value",sList[i]).text(sListName[i]);
		if(value==sList[i])no.attr("selected","selected");
		
		neditor.append(no);
	}

	return neditor;
	return et;
	
}
var editorFunc_array = function(value,option){
	var _option = s2j(option);
	var _value = s2j(value);


	if(!_value)_value=[];
	
	if(typeof(_value)!="object")_value=[_value];
	
	// log("editorFunc_array value is "+ j2s(_value));
	// log("editorFunc_array option is "+j2s(_option));
	
	var eTable = $("<table>").attr("value",j2s(_value)).attr("editor",j2s(_option)).addClass("LQEditor table table-bordered");
	var eTbody = $("<tbody>").appendTo(eTable);


	//  var _result = "<table border=1 value='"+j2s(_value)+"' editor='"+j2s(_option)+"' class='LQEditor table table-bordered' editor='array'><tbody>";
	
	// //타이틀
	// _result+="<tr>";
	// _result+="<td colspan=2>";
	// _result+="<input type='button' value='add' class='LQJSONArrayEditorAdd btn btn-primary'></td>";
	// _result+="</tr>";
	
	//log("-----------start-------------");
	for(var j in _value){
		var nTR = $("<tr>").attr("datarow","").appendTo(eTbody);
		var cTD = $("<td>").attr("bgcolor","cccccc").attr("width",1).css("min-width",1).appendTo(nTR);
		var nTD = $("<td>").attr("datafield","").appendTo(nTR);
		nTD.append(editorSelector(_option["element"],_value[j]));
		var nTD2 = $("<td>").appendTo(nTR);
		nTD2.append("<input type='button' value='-' class='LQJSONArrayEditorDelete  btn btn-danger'>");
		// _result+="<tr datarow>";
		// _result+="<td datafield>";	
		// log("editroFunc_array in for "+j2s(_value[j])+" in "+j+" element "+j2s(_option["element"])+" fullsource "+j2s(_value));
		// _result+= editorSelector(_option["element"],_value[j]);
		// _result+="</td>";
		// _result+="<td><input type='button' value='delete' class='LQJSONArrayEditorDelete  btn btn-danger'></td>";
		// _result+="</tr>";
	}

	var eTR = $("<tr>").addClass("addrow").appendTo(eTbody);
	eTR.append("<td bgcolor=cccccc></td><td colspan=2 align=center><input type='button' value='Add to Array' class='LQJSONArrayEditorAdd btn btn-primary'></td></tr>");

	//log("-----------end-------------");
	//_result+="</tbody></table>";
	
	return eTable;
	//return _result;

	
}

var editorFunc_array_value = function(obj){
	
	 var fieldname = [];
	 var dataTable = obj;
	 var eavresult=[];
	
	dataTable.children('tbody').children("tr[datarow]").each(function(index,item){
		 var dtfield = $(this).find('td[datafield]:first');
		 var editor = dtfield.find(".LQEditor:first");
		 var vvalue = getObjValue(editor);
		
		eavresult.push(vvalue);
	});

	//log("result is."+j2s(eavresult));
	return eavresult;

}

var editorFunc_table = function(value,option){
	var _option = s2j(option["element"]);
	var _value = s2j(value);
	if(!_value)_value=[];
	//var _result = "<table border=1 value='"+j2s(_value)+"' editor='"+j2s(option)+"' class='LQEditor table table-bordered'><tbody>";
	var eTable = $("<table>").attr("value",j2s(_value)).attr("editor",j2s(option)).addClass("LQEditor table table-bordered");
	var eTbody = $("<tbody>").appendTo(eTable);
	var eTR = $("<tr>").appendTo(eTbody);	
	//타이틀
	//_result+="<tr>";
	for(var i in _option){
		$("<td>").attr("field",_option[i]["field"]).append(_option[i]["title"]).appendTo(eTR);
		// _result+="<td field='"+_option[i]["field"]+"'>";
		// _result+=_option[i]["title"];
		// _result+="</td>";
	}
	var eTD = $("<td>").appendTo(eTR);
	$("<input>").attr("type","button").attr("value","add").addClass("LQJSONTableEditorAdd btn btn-primary").appendTo(eTD);

	// _result+="<td><input type='button' value='add' class='LQJSONTableEditorAdd btn btn-primary'></td>";
	// _result+="</tr>";
	for(var j in _value){
		var nTR = $("<tr>").attr("datarow","").appendTo(eTbody);
//		_result+="<tr datarow>";
		
		if(_option.length>1){
			for(var i in _option){
				fieldname = _option[i]["field"];
			 	$("<td>").attr("field",fieldname).attr("datafield","").appendTo(nTR).append(editorSelector(_option[i],_value[j][fieldname]));
			 	// _result+="<td field='"+fieldname+"' datafield>";	
			 	// log("editorFunc_table in for, editor is "+j2s(_option[i])+" value["+j+"]["+fieldname+"] is "+j2s(_value));
			 	// _result+= editorSelector(_option[i],_value[j][fieldname]);
			 	// _result+="</td>";

			}
		}else{
			$("<td>").attr("field","LQTABLESINGLE").attr("datafield").append(editorSelector(_option[0],_value[j])).appendTo(nTR);
			// _result+="<td field='LQTABLESINGLE' datafield>";	
			// _result+= editorSelector(_option[0],_value[j]);
			// _result+="</td>";
		}
		 var nTD = $("<td>").appendTo(nTR);
		 $("<input>").attr("btn","button").attr("value","-").addClass("LQJSONTableEditorDelete btn btn-danger").appendTo(nTD);

		//  _result+="<td><input type='button' value='delete' class='LQJSONTableEditorDelete  btn btn-danger'></td>";
		// _result+="</tr>";
	}
//	_result+="</tbody></table>";
	return eTable;	
//	return $(_result);
}		

var editorFunc_table_value = function(obj){
	
	var fieldname = [];
	var dataTable = obj;
	var result=[];
	
	dataTable.children('tbody').children("tr[datarow]").each(function(index,item){
		var vdict = {};
		$(this).children('td[datafield]').each(function(index,item){
			var editor = $(this).children(".LQEditor:first");
			var editorName = editor.attr("editor");
			var fieldname = $(this).attr("field");
			var vvalue = getObjValue(editor);
			if(typeof(fieldname)!="undefined" && fieldname!="" && fieldname!="undefined" && fieldname!="LQTABLESINGLE")
				vdict[fieldname]=vvalue;
			else
				vdict=vvalue;
		});
		result.push(vdict);
	});
	return result;

}



var editorFunc_dictionary = function(value,option){
	//log("dict option is"+j2s(option));
	var _option = s2j(option["element"]);
	var _value = s2j(value);
	if(!_value)_value={};
	var keyEditor=undefined;
	var keyEditorOption=undefined;
	
	var isInVKey = false;
	//option 이랑 value랑 비교하여 value 에 없는 option  집어넣기

	var check=false;
	for(var key in _option){
		check=true;
		if(!keyEditor){
			keyEditor = _option[key]["keyEditor"];
			keyEditorOption = s2j(_option[key]["keyEditorOption"]);
			if(keyEditor){
				_option.splice(key, 1);
				continue;
			}
		}
		
		if(_option[key]["field"]=="VARIABLEKEY")isInVKey=true;
		
		for(key2 in _value){
			//log("check"+key2+"==="+_option[key]["field"]);
			if(_option[key]["field"] == key2){
				check=false;
				break;
			}
		}
		
		if(check==true){
			if(_option[key]["field"]!="VARIABLEKEY"){
				_value[_option[key]["field"]]="";
			
			}else{
				if(!value){
				_value[_option[key]["title"]]="";
				}
			}
		}
		
	}
	// log(_value);
	// log("value is"+j2s(_value));
	


	//var _result = "<table border=1 value='"+j2s(_value)+"' editor='"+j2s(option)+"' class='LQEditor table table-bordered' keyEditor='"+j2s(keyEditor)+"' keyEditorOption='"+j2s(keyEditorOption)+"'><tbody>";

	var neditor = $("<table>").attr("value",j2s(_value)).attr("editor",j2s(option)).addClass('LQEditor table table-bordered').attr("keyEditor",j2s(keyEditor)).attr("keyEditorOption",j2s(keyEditorOption));
	var eTbody = $('<tbody>').appendTo(neditor);
	// var eTR = $('<tr>').appendTo(eTbody);
	// var eTD = $('<td>').appendTo(eTR).attr("colspan",3);
	// //$('<input>').attr("type","text").addClass("LQDictFieldName form-control form-control-inline'").appendTo(eTD);
	// var eSelect = $('<select>').addClass("LQDictEditorSelect form-control form-control-inline").appendTo(eTD);
	// if(isInVKey) eSelect.append("<option value='VARIABLEKEY'>VARIABLE FIELD</option>");
	// eSelect.append("<option value='text(string)'>text(string)</option><option value='text(number)'>text(number)</option><option value='textarea'>textarea</option><option value='dictionary'>dictionary</option><option value='array(number)'>array(number)</option><option value='array(string)'>array(string)</option><option value='array(dict)'>array(dict)</option></select>");
	
	
	//$("<tr>").append("<td>key</td><td>value</td><td></td>").appendTo(eTbody);

	// _result+="<tr><td colspan=3><input type='text' class='LQDictFieldNameText form-control form-control-inline'>"
	
	// _result+="<select class='LQDictEditorSelect form-control form-control-inline'>";
	
	// //옵션이랑 같은 필드 추가
	// if(isInVKey)_result+="<option value='VARIABLEKEY'>VARIABLE FIELD</option>";
	
	// _result+="<option value='text(string)'>text(string)</option><option value='text(number)'>text(number)</option><option value='textarea'>textarea</option><option value='dictionary'>dictionary</option><option value='array(number)'>array(number)</option><option value='array(string)'>array(string)</option><option value='array(dict)'>array(dict)</option></select>";
	
	// _result+="<input type='button' value='add' class='LQJSONDictionaryEditorAdd btn btn-primary'></td></tr>";
	
	
	// _result+="<tr><td>key</td><td>value</td><td></td></tr>";
	
	for(var j in _value){
		var nTR = $("<tr>").attr("datarow","").appendTo(eTbody);
		var cTD = $("<td>").attr("bgcolor","cccccc").attr("width",1).css("min-width",1).appendTo(nTR);
		var nTD = $("<td>").appendTo(nTR);
		$("<input>").attr("type","text").attr("value",j).addClass("LQDictFieldName form-control form-control-inline").appendTo(nTD);

		// _result+="<tr datarow><td>";
		
		// _result+="<input type='text' value='"+j+"' class='LQDictFieldName form-control form-control-inline'>";
		
		if(keyEditor){
			//log("keyEditor"+keyEditor);
			$("<input>").attr("type","button").attr("value","select").attr("func",keyEditorOption["func"]).attr("mode",keyEditorOption["mode"]).addClass("LQDataSelectorButton btn btn-info").appendTo(nTD);

			//_result+="<input type=button value='select' func='"+keyEditorOption["func"]+"' mode='"+keyEditorOption["mode"]+"' class='LQDataSelectorButton btn btn-info'>";
		}
		
		var nTD2 = $("<td>").appendTo(nTR).attr("field",j).attr("datafield","");

		//_result+="</td><td field='"+j+"' datafield>";
		
		_op2="";
		for(k in _option){
			if(_option[k]["field"]==j)_op2=_option[k];
		}
				
		if(_op2=="")
			if(typeof(_option) != "undefined" && typeof(_option[0])!="undefined" && _option[0]["field"]=="VARIABLEKEY")_op2 = _option[0];
			
			
		_op1="";
		for(k in _option)
			if(_option[k]["field"]==j)_op1=_option[k];

	
		if(_op1=="")
		if(typeof(_option) != "undefined" && typeof(_option[0])!="undefined" && _option[0]["field"]=="VARIABLEKEY")_op1 = _option[0];
			
		//if(_op1=="")_op1="text";

		
		//log("editorFunc_dictionary _op1 is " + j2s(_op1));
		
		nTD2.append(editorSelector(_op1,_value[j]));
//		_result+= editorSelector(_op1,_value[j]);
		
		var nTD3 = $("<td>").appendTo(nTR);
		$("<input>").attr("type","button").attr("value","-").addClass("LQJSONTableEditorDelete btn btn-danger").appendTo(nTD3);
		
		// _result+="</td><td><input type='button' value='delete' class='LQJSONTableEditorDelete btn btn-danger'></td>";
		//_result+="</tr>";
	}
	//_result+="</tbody></table>";

	var nTR = $("<tr>").addClass("addrow").appendTo(eTbody);
	var nTD0 = $("<td>").appendTo(nTR).attr("bgcolor","cccccc").css("min-width",1);
	var nTD = $("<td>").appendTo(nTR).attr("colspan",3).attr("align","center");
	var addBtn = $("<input>").attr("type","button").attr("value","Add to Dict").addClass("LQJSONDictionaryEditorAdd btn btn-primary").appendTo(nTD);
	
	return neditor;
//	return $(_result);

}

var editorFunc_dictionary_value = function(obj){
	var fieldname = [];
	var dataTable = obj;
	var vdictresult={};
	
	dataTable.children('tbody').children("tr[datarow]").each(function(index,item){
		
		var fieldname = $(this).children("td").children(".LQDictFieldName").val();
		var _valueEditor = $(this).children("td[datafield]").children(".LQEditor:first");
		
		var vvalue = getObjValue(_valueEditor);
		vdictresult[fieldname] = vvalue;

	});
	//log("editorFunc_dictionary_value value is "+j2s(vdictresult));
	return vdictresult;

}			
///// selector  //////////////////////////////////////////////////////////////////////////////////////

var viewerSelector = function(viewer,value){
	var viewValue="";
	viewer=s2j(viewer);
	if(typeof(viewer)=="undefined" || typeof(viewer["type"])=="undefined")viewer={"type":"text"};

	switch(viewer["type"]){
		case "url": viewValue = viewerFunc_url(value,viewer); break;
		case "json": viewValue = viewerFunc_json(value,viewer); break;
		case "image": viewValue = viewerFunc_image(value,viewer); break;
		case "text": viewValue = viewerFunc_text(value,viewer); break;
		case "datetime": viewValue = viewerFunc_datetime(value,viewer); break;
		case "timestamp": viewValue = viewerFunc_timestamp(value,viewer); break;
		case "time": viewValue = viewerFunc_time(value,viewer); break;
		case "format": viewValue = viewerFunc_format(value,viewer); break;
		case "select": viewValue = viewerFunc_select(value,viewer); break;
		case "bool": viewValue = viewerFunc_bool(value,viewer); break;
		case "custom": viewValue = eval(viewer["func"]+"(value,viewer);");break;
		default:
		var vFunc = false;
		eval("if(typeof "+viewer["type"]+" != 'undefined') vFunc="+viewer["type"]+";");
		if(vFunc){
			viewValue = vFunc(value,viewer);
		}else{
			viewValue = value;
		}
		 break; 
	}
	return viewValue;
}

var editorSelector = function(editor,value){
	if(typeof(value)=='undefined'){
		value="";	
	}else{
		value = s2j(value);
	}
	editor = s2j(editor);
	//log("editor is "+j2s(editor));
	if(!editor || typeof(editor)=="undefined" || editor==""){
		editor = {};
		editor["type"]="autoEditor";
		// if(typeof(value)=="number"){ editor["type"]="text"; editor["datatype"]="int";}
		// if(typeof(value)=="string" && value.length<100) editor["type"]="text";
		// if(typeof(value)=="string" && value.length>100) editor["type"]="textarea";
		// if(typeof(value)=="object" && value[0]!=undefined) editor["type"]="array";
		// else if(typeof(value)=="object") editor["type"]="dictionary";
		
		
		//log("editorSelector, valuse is "+typeof(value)+" editor is "+editor);
	}


	var pushEditor="";
	var editortype = editor["type"];
	//log("editortype is " + editortype);
	switch(editortype){
		case "custom": pushEditor = eval(editor["func"]+"(value,editor);");break;
		case "textarea": pushEditor = editorFunc_textarea(value,editor);break;
		case "dataSelector":pushEditor=editorFunc_dataSelector(value,editor);break;
		case "text":
			if(typeof(value)=="string" && value.indexOf("\n")> -1){
				pushEditor=editorFunc_textarea(value,editor);
			}else{
				pushEditor=editorFunc_text(value,editor);
			}
		break;
		case "select":pushEditor=editorFunc_select(value,editor);break;
		case "bool":pushEditor=editorFunc_bool(value,editor);break;
		case "table":pushEditor=editorFunc_table(value,editor);break;
		case "array":pushEditor=editorFunc_array(value,editor);break;
		case "dictionary":pushEditor=editorFunc_dictionary(value,editor);break;
		case "date":pushEditor=editorFunc_date(value,editor);break;
		case "datetime":pushEditor=editorFunc_datetime(value,editor);break;
		case "time":pushEditor=editorFunc_time(value,editor);break;
		case "password":pushEditor=editorFunc_password(value,editor);break;
		case "NONE":pushEditor=j2s(value);break;
		default:
		var vFunc = editorFunc_text;
		eval("if(typeof "+editortype+" != 'undefined') vFunc="+editortype+";");
		pushEditor=vFunc(value,editor);
		//log("selected default editor");
		break;

	}
	return pushEditor;
}

var makeManageButton = function(manage,isEditorMode){
	var pushData="";
	manage = j2s(manage);
	if(isEditorMode){
		pushData = "<input type=button value='Apply' class='LQModifyApply btn btn-primary'><input type=button value='Cancel' class='LQModifyCancel btn btn-danger'>";
	}else{
		if(typeof(manage)=="string"){
			if(manage.indexOf("update")!=-1)pushData+="<button  class='LQModify btn btn-primary glyphicon glyphicon-edit'></button>";
			if(manage.indexOf("delete")!=-1)pushData+="<button  class='LQDelete btn btn-danger glyphicon glyphicon-trash'></button>";
		}
	}
	return pushData;
}


var startUpdateMode = function(obj){
	var tInfo = gf(obj);

	if(tInfo["table"].attr("editType")=="form"){
		
		contextSelectedCell = tInfo["cell"];
		var newDiv = $("#LQEditForm").html("");
		var newTable = $("<table>").appendTo(newDiv).addClass("table table-bordered LQDataForm LQEditForm").attr("data",j2s(tInfo["rowData"])).attr("startMode","update").attr("func","updateWithLQTable");
		
		var attrs = tInfo["table"][0].attributes;
		for(var i=0;i<attrs.length;i++) {
			if(attrs[i].nodeName!="class"){
				newTable.attr(attrs[i].nodeName,attrs[i].value);
				//log(attrs[i].nodeName + " => " + attrs[i].nodeValue);
			}
		}


		tInfo["table"].find("th").each(function(index,item){
			var fInfo=gf($(this));
			if($(this).attr("field")){
				var nTR = $("<tr>").appendTo(newTable);
				var nTD = $("<td>").appendTo(nTR).append(fInfo["title"]);
				var nTD2 = $("<td>").appendTo(nTR).addClass("LQDataCell").attr("field",fInfo["field"]);
				var attrs = $(this)[0].attributes;
				for(var i=0;i<attrs.length;i++) {
					nTD2.attr(attrs[i].nodeName,attrs[i].value);
				}
				var v="";
				if(tInfo["rowData"]){
					v = tInfo["rowData"][fInfo["field"]]; 
				}

				
				if(fInfo["editor"]!=undefined && fInfo["isVirtual"]!=true){
					fInfo["editor"]["rowData"]=fInfo["rowData"];
					viewValue = editorSelector(fInfo["editor"],v);
				}
				else{
					fInfo["viewer"]["rowData"]=fInfo["rowData"];
					viewValue = viewerSelector(fInfo["viewer"],v);
				}
				nTD2.append(viewValue);
			}
		});
		
		
		newDiv.attr("title","Edit");
		newDiv.dialog({
		  beforeClose: function(){},
	      modal: true,
	      width:800,
	      height:800,
	      buttons:{"Save":function(){
	      	requestUpdate(newTable);
	      	$(this).dialog( "close" );
	      }}
	    });


	}else{
		tInfo["row"].find("td").each(function(index,item){
			var fInfo=gf($(this));
			var viewValue;
			if($(this).attr("field") && fInfo["editor"]!=undefined){

				var v="";
				if(fInfo["rowData"]){
					v = fInfo["rowData"][fInfo["field"]]; 
				}
				viewValue = editorSelector(fInfo["editor"],v);//editorSelector(fInfo["viewerName"],fInfo["rowData"][fInfo["field"]],fInfo["viewerOption"]);
				// log("aklsjdflasjdklfjalskdf");
				// //$(this).html(viewValue);
				// $(this).html("").append(viewValue);
				$(this).html("").append(viewValue);
			}else if($(this).hasClass("LQManageCell")){
				viewValue = makeManageButton(fInfo["manage"],true);	
				//$(this).html(viewValue);
				$(this).html("").append(viewValue);
			}
			
		});
	}
}


var getLQEditorValue = function(name){
	var obj = $("td[field='"+name+"']").find(".LQEditor:first");
	return getObjValue(obj);
}
var getLQDataObject = function(name){
	return $("table[name='"+name+"']");
}
var getDataForm = function(name){
	return $("table[name='"+name+"']");
}


var getDataTable = function(name){
	return $("table[name='"+name+"']");
}

var loadDataEditor = function(obj){
	obj.find("td[field]").each(function(index){
		var pushEditor = editorSelector($(this).attr("editor"),"");	
		$(this).append(pushEditor);
	});
}
var loadDataForm = function(obj){
	if(typeof(obj)=="string")obj = $("table[name="+obj+"]");

		var tInfo = gf(obj);
		//var dbParam = {"gid":gid,"dbFunc":tInfo["selectFunc"],"dbClass":tInfo["dbClass"],"param":tInfo["dbWhere"],"dbMode":"select"};

		var dbParam = {"gid":gid,"dbClass":tInfo["dbClass"],"param":tInfo["dbLoadParam"],"dbMode":"select","type":"form"};
		log("loadDataForm param is"+j2s(dbParam));

		obj.find("[field]").each(function(index,item){
			$(this).addClass("LQDataCell");
		});

		obj.find(".LQDataCell").each(function(index,item){
			$(this).html("");
		});

		$.ajax({
	    url : tInfo["dbSource"], 
	    data : dbParam,
	    dataType : "json", 
	    type : "post",
	    success : function(data){
	    	printLog(data);
	    	if(data["result"]["code"]==1){
		    	log("LQDataForm "+j2s(data));
				//받아온 data와 표시할정보 비교해서 뿌릴데이터모으기
			
				var rowdata = data["data"];
				var shi =rowdata["shardIndex"];
				delete rowdata["shardIndex"];
				tInfo["table"].attr("data",j2s(data["data"])).attr("shardIndex",shi);
				for(var i in rowdata){
					var viewer = tInfo["table"].find("td[field="+i+"]").attr('viewer');
					var viewValue = viewerSelector(viewer,rowdata[i]);
					log("field:"+i+"view:"+viewer);
					tInfo["table"].find("td[field="+i+"]").html("").append(viewValue);
					tInfo["table"].find("td[field="+i+"]").addClass("LQDataCell");
				}

				if(obj.attr("startMode")=="update"){
					startUpdateMode(obj);
				}
			}else{
				alert("Error",data["result"]["message"],null,null);
  				//alert("Error\n"+data["result"]["message"]);
			}
		}
		,
		error: function(e) {
		    	alert("error",j2s(e));
		    }
		});



}
var alert = function(mtitle,msg,closeFunc,btns){
	log("ALERT:"+mtitle+"-"+msg);
	if(typeof(msg)=="undefined" || !msg)msg=mtitle;
	$("#LQDialog").html(msg);
	$("#LQDialog").dialog({
	  title:mtitle,
	  beforeClose: closeFunc,
      modal: true,
      buttons: btns
    });
}

var loadDataTableInfo = function(obj,callFunc){
	if(typeof(obj)=="string")obj = $("table[name="+obj+"]");

	var tInfo = gf(obj);

	var dbParam = {"gid":gid,"dbClass":tInfo["dbClass"],"param":tInfo["dbLoadParam"],"dbFunc":tInfo["loadFunc"],"dbMode":"load"};
	log(j2s(dbParam));
	$.ajax({
	    url : tInfo["dbSource"], 
	    data : dbParam,
	    dataType : "json", 
	    type : "post",
	    success : function(data){
	    	var headTR = $('<tr>');
	    	printLog(data);
	    	
	    	if(typeof(data["head"]) == "string") data["head"] = s2j(data["head"]);

	    	if(!data["head"]){
	    		callFunc(obj);
	    		return;
	    	}

	    	var headString = "<tr>\n";

	 		for(var i in data["head"]){
	 			var headData=data["head"][i];
	 			var headTitle = "";
	 			var headTH = $("<th>");
	 			headString+="<th ";	
	 			if(typeof(headData)=="string")headData=s2j(headData);

	 			for(var j in headData){
	 				if(!isNaN(parseInt(j))){
		 				headTH.attr(headData[j],"");
	 					headString+=j2s(headData[j])+" ";
	 				}else{
		 				headTH.attr(j,j2s(headData[j]));
	 					headString+=j2s(j)+"='"+j2s(headData[j])+"' ";
	 				}
	 			}
	 			
	 			if(headData["field"])headTitle=headData["field"];
	 			if(headData["title"])headTitle=headData["title"];

	 			headString+=">"+headTitle+"</th>\n";

	 			headTH.html(headTitle);

	 			headTH.appendTo(headTR);
	 		}
	 		headString+="</tr>";
	 		log(headString);
	 		
	 		headTR.appendTo(obj.find("thead"));
	 		log(headTR.html());
	    	if(tInfo["dbLimit"]>0){
	    		var foot = $('<tfoot>').appendTo(tInfo["table"]);
	    		var footTR = $("<tr>").appendTo(foot);
	    		var footTD = $("<td>").attr("align","center")
							    	  .addClass("LQLoadNext")
							    	  .attr("colspan",data["head"].length)
							    	  .html("Next")
							    	  .appendTo(footTR);
	    	}
	    	callFunc(obj);
		}
		,
		error: function(e) {
			alert("error",j2s(e));
			callFunc(obj);	
		    }
		});
}

var setLoadDataCallBack = function(dataviewname,func){
	var dataview = $("[name="+dataviewname+"]");
	dataview.data("loadDataCallBack",func);
}

var loadDataTable = function(obj,addMode){
		var tInfo = gf(obj);
/*
		var LQDataTable = $(this);
		var dbSource = LQDataTable.attr('dbSource');
		var dbTable = LQDataTable.attr('dbTable');
		var dbSort = LQDataTable.attr('dbSort'); 
*/					
		var dbParam = "";
		
		if(addMode=="reload" || !addMode){
			tInfo["table"].find("tbody[datazone]").html("");
		}

		if(typeof(tInfo["dbClass"])!="undefined"){
			// var param = s2j(tInfo["dbWhere"]);
			// param["limit"]=tInfo["dbLimit"];
			// param["sort"]=tInfo["dbSort"];
			// tInfo["dbWhere"] = j2s(param);
			dbParam = {"gid":gid,"dbFunc":tInfo["selectFunc"],"dbClass":tInfo["dbClass"],"param":tInfo["dbLoadParam"],"dbMode":"select"};
		}else{
			dbParam = {"gid":gid,"table":tInfo["dbTable"],"sort":tInfo["dbSort"],"limit":tInfo["dbLimit"],"where":tInfo["dbWhere"],"dbMode":"select"};
		}

		log("loadDataTable"+j2s(dbParam));
	
		//추가폼 만들기
		if(tInfo["manage"] && tInfo["manage"].toLowerCase().indexOf("insert")>=0){
			$("th[manage]").html("");
			$("<input>").attr("type","button").attr("value","+").addClass("LQAddStart btn btn-primary").appendTo($("th[manage]"));
		}
		
		//addForm+="</tr>";

		
		$.ajax({
	    url : tInfo["dbSource"], 
	    data : dbParam,
	    dataType : "json", 
	    type : "post",
	    success : function(resultData){

	    	printLog(resultData);
		    log("getData "+j2s(resultData));
			var a = tInfo;
	    	var callbackfunc = tInfo["table"].data("loadDataCallBack")
	    	if(typeof(callbackfunc)=="function")callbackfunc(tInfo["table"],resultData);

	    	if(resultData["result"]["code"]==1){

		    	var data = resultData["data"];
		    	log("getData "+j2s(resultData));
				//받아온 data와 표시할정보 비교해서 뿌릴데이터모으기
				for(var i in data){
					var rowData = data[i];
					//rColor = '#'+(0x1000000+(Math.random())*0xffffff).toString(16).substr(1,6);
					if(i%2==0)bgcolor="eeeeee";
					else bgcolor="ffffff";

					tInfo["table"].find('tbody[datazone]:last').append(makeDataRow(rowData,obj,bgcolor));
				}
				
				
				var dbWhere = s2j(tInfo["table"].attr("dbWhere"));
				
				if("nextInfo" in resultData){
					dbWhere["start"]=resultData["nextInfo"]["start"];
					dbWhere["startList"]=resultData["nextInfo"]["startList"];
				}
				tInfo["table"].attr("dbWhere",j2s(dbWhere));

				log("set nextInfo is"+j2s(resultData["nextInfo"]));

			}else{
				alert("error",resultData["result"]["message"]);
			}
		}
		,
		error: function(e) {
	    	alert("error",j2s(e));
	    }
	});
}



var makeDataRow = function(rowData,tableObj,bgcolor){
	var tInfo = gf(tableObj);
	if(!bgcolor)bgcolor="ffffff";
	var shi =rowData["shardIndex"];
	delete rowData["shardIndex"];
	//var pushData="<tr data='"+j2s(rowData)+"' class='LQDataRow' bgcolor='"+bgcolor+"' shardIndex='"+shi+"'>";
	var dataTR = $("<tr>").attr("data",j2s(rowData)).addClass("LQDataRow").attr("bgcolor",bgcolor).attr("shardIndex",shi);
	tInfo["table"].find("thead > tr > th").each(function(index,item){
		var isVirtual = $(this).attr('virtual');
		var field = $(this).attr('field');
		var viewer = $(this).attr('viewer');
		var viewerOption = $(this).attr('viewerOption');
		var manage = $(this).attr('manage');
		var editor = $(this).attr('editor');
		var value=rowData[field];
		//데이터필드
		if(manage==undefined){
			//viewer 적용
			if(isVirtual=="" && rowData){
				viewer = s2j(viewer);
				viewer["rowData"]=rowData;
				viewer = j2s(viewer);
			}
			var viewValue = viewerSelector(viewer,value);
			
			//primary key 속성 넣어주기
			$("<td>").addClass("LQDataCell").attr("field",field).append(viewValue).appendTo(dataTR);
			//pushData+="<td class='LQDataCell' field='"+field+"'>"+viewValue+"</td>";
		
		//메니저필드
		}else{
			// delete='"+$(this).attr("delete")+"' modify='"+$(this).attr("modify")+"'
			$("<td>").addClass("LQManageCell").append(makeManageButton(manage,false)).appendTo(dataTR);
			// pushData+="<td class='LQManageCell'>";
			// pushData+=makeManageButton(manage,false);
			// pushData+="</td>";
		}
		
	});
	
	//pushData+="</tr>";

	return dataTR;
}

$(document).ready(function(){

	$('.LQDataEditor').each(function(index,item){
		loadDataEditor($(this));
	});

	$('.LQDataForm').each(function(index,item){
		if($(this).attr("autoLoad")!="false")loadDataForm($(this));
		else{
			if($(this).attr("startMode")=="update"){
				startUpdateMode($(this));
			}
		}
	});

	$('.LQDataTable').each(function(index,item){
		$(this).addClass("table table-bordered");
		var autoLoadSetting = $(this).attr("autoLoad");
		if($(this).attr("autoSetting")=="true"){
			$(this).find("thead").css("background-color","cccccc");
			

			loadDataTableInfo($(this),function(obj){
				if(autoLoadSetting!="false")loadDataTable(obj,"reload");
			});
		}else{
			if(autoLoadSetting!="false")loadDataTable($(this),"reload");

			var tInfo = gf($(this));
			var titleLength = tInfo["table"].find("th").length;
    		var foot = $('<tfoot>').appendTo(tInfo["table"]);
    		var footTR = $("<tr>").appendTo(foot);
    		var footTD = $("<td>").attr("align","center")
						    	  .addClass("LQLoadNext")
						    	  .attr("colspan",titleLength)
						    	  .html("Next")
						    	  .appendTo(footTR);
    	
		}
	});
	
	
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// event /////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	$('body').on('click','.LQDataSelectorButton',function(){			
		openDataSelector($(this),$(this).attr("func"),$(this).attr("mode"));
	});

	//LQJSONDictionaryEditorAdd
	$('body').on('click','.LQJSONDictionaryEditorAdd',function(){
		var dataTable = $(this).parent().parent().parent().parent();
		var result="<tr datarow>";

		var eTR = $("<tr>").attr("datarow","");
		var cTD = $("<td>").attr("bgcolor","cccccc").attr("width",1).css("min-width",1).appendTo(eTR);
/*
	option = dataTable.attr("option");
	if(typeof(option)=="string")
		option = s2j(option);
*/
		
	
//	for(i in option){
	//	log(option[i]);
	
		var fieldname = $(this).parent().children(".LQDictFieldName").val();
		//var fieldeditor = $(this).parent().children(".LQDictEditorSelect").val();
		
		var option = dataTable.attr("editor");
		log("editor option is"+option);
		
		if(option != undefined)option=s2j(option);
		
		
		var keyEditor=dataTable.attr("keyEditor");
		var keyEditorOption=s2j(dataTable.attr("keyEditorOption"));
		
		
//					result+="<td><input type='text' value='"+fieldname+"' class='LQDictFieldName'></td><td datafield>";
		
		
		var eTD = $("<td>").appendTo(eTR);
		$("<input>").attr("type","text").addClass("LQDictFieldName form-control form-control-inline").appendTo(eTD);

		// result+="<td>";
		
		// result+="<input type='text' class='LQDictFieldName form-control form-control-inline'>";
		
		// // if(keyEditor){
		// // 	result+="<input type=button value=선택 func='"+keyEditorOption["func"]+"' mode='"+keyEditorOption["mode"]+"' class='LQDataSelectorButton'>";
		// // }
		
		// result+="</td><td datafield>";
		
		var eTD2 = $("<td>").attr("datafield","").appendTo(eTR);

		
		
		var addEditor;
		// if(fieldeditor=="array(number)"){
		// 	addEditor= editorSelector({"type":"array","element":{"type":"text","datatype":"int"}},"");	
		// }else if(fieldeditor=="array(string)"){
		// 	addEditor= editorSelector({"type":"array"},"");	
		// }else if(fieldeditor=="array(dict)"){
		// 	addEditor= editorSelector({"type":"array","element":{"type":"dictionary"}},"");	

		// }else if(fieldeditor=="text(number)"){
		// 	addEditor= editorSelector({"type":"text","datatype":"int"},"");	
		// }else if(fieldeditor=="text(string)"){
		// 	addEditor= editorSelector({"type":"text"},"");
		// }else if(fieldeditor=="VARIABLEKEY"){
		// 	var vk;
		// 	for(k in option){
		// 		if(option[k]["field"]=="VARIABLEKEY")vk=k;
		// 	}
		// 	addEditor= editorSelector(option[vk]["editor"],"");
		// }else{
		// 	addEditor= editorSelector({"type":"autoViewer"},"");	
		// }


		addEditor= editorSelector({"type":"autoEditor"},"");	
		eTD2.append(addEditor);

		var eTD3 = $("<td>").appendTo(eTR);
		$("<input>").attr("type","button").attr("value","-").addClass("LQJSONTableEditorDelete btn btn-danger").appendTo(eTD3);
		//result+="</td><td><input type='button' value='delete' class='LQJSONTableEditorDelete btn btn-danger'></td>";
//	}
	//result+="</tr>";
	
	dataTable.children('tbody').children('.addrow').before(eTR);
});


$('body').on('click','.LQJSONTableEditorDelete',function(){
	log("delete");
	$(this).parent().parent().remove();
});

$('body').on('click','.LQJSONArrayEditorDelete',function(){
	log("delete");
	$(this).parent().parent().remove();
});

$('body').on('click','.LQJSONTableEditorAdd',function(){
	//var tInfo = gf($(this));
	var dataTable = $(this).parent().parent().parent().parent();
	//var result="<tr datarow>";
	var option = s2j(dataTable.attr("editor"));
	var element = option["element"];
	
	var eTR = $("<tr>");
	if(element)
		if(typeof(element)=="string")
			element = s2j(element);
		if(element.length>1){
			for(var i in element){
				var fieldname = element[i]["field"];
			 	$("<td>").attr("datafield","").append(editorSelector(element[i],"")).appendTo(eTR);

			 	// result+="<td field='"+fieldname+"' datafield>";	
			 	// result+= editorSelector(element[i],"");
			 	// result+="</td>";
			}
		}else{
			$("<td>").attr("datafield","").attr("field","LQTableSINGLE").append(editorSelector(option["element"],"")).appendTo(eTR);
			//result+="<td field='LQTableSINGLE' datafield>";	
			//result+= editorSelector(option["element"],"");
			//result+="</td>";
		}
	
	var nTD = $("<td>").appendTo(eTR);
	$("<input>").attr("type","button").attr("value","-").addClass("LQJSONTableEditorDelete btn btn-danger").appendTo(nTD);
	//result+="<td><input type='button' value='delete' class='LQJSONTableEditorDelete btn btn-danger'></td></tr>";
	
	dataTable.children('tbody').append(eTR);
});

$('body').on('click','.LQJSONArrayEditorAdd',function(){
	//var tInfo = gf($(this));
	var dataTable = $(this).parent().parent().parent().parent();
	var fieldeditor = $(this).parent().children(".LQArrayEditorSelect").val();
	//var result="<tr datarow>";
	var option = dataTable.attr("editor");
	var eTR = $("<tr>").attr("datarow","");
	var cTD = $("<td>").attr("bgcolor","cccccc").attr("width",1).css("min-width",1).appendTo(eTR);
	if(option)
		if(typeof(option)=="string")
			option = s2j(option);
	
	$("<td>").attr("datafield","").append(editorSelector(option["element"],"")).appendTo(eTR);
 	// result+="<td datafield>";	
 	
 	// //log("editor is"+fieldeditor);
 	
 	// result+= editorSelector(option["element"],"");	
 	// result+="</td>";

	
	var nTD = $("<td>").appendTo(eTR);
	$("<input>").attr("type","button").attr("value","-").addClass("LQJSONArrayEditorDelete btn btn-danger").appendTo(nTD);
//	result+="<td><input type='button' value='delete' class='LQJSONArrayEditorDelete btn btn-danger'></td></tr>";
	
	dataTable.children("tbody").children(".addrow").before(eTR);
});

/*
//LQJSONArrayEditorApply
$('body').on('click','.LQJSONArrayEditorApply',function(){
	dataTable = $(this).parent().parent().parent().parent();
	result=[];
	dataTable.children('tbody').children("tr[datarow]").each(function(index,item){
		dict={};
		log('datarow');
		$(this).children('td[datafield]').each(function(index,item){
		log('datafield');
			
			editor = $(this).children(".LQEditor:first");
			fieldname = $(this).attr("field");
			dict[fieldname]=getObjValue(editor,0);
			
			//lqdeidotr 에디터 알아내고  에디터_setvalue($this.find(".LQEditor")) 함수 호출하기 ##############
		});
		result.push(dict);
	});
	log(j2s(result));
	dataTable.attr("value",j2s(result));
	dataTable.val(j2s(result));
});
*/

//수정눌렀을때 수정모드로~~!/////////////////////////////////////////////////////////////////////////////////////////////
$('body').on('click','.LQModify',function(){
	startUpdateMode($(this));
});

$('body').append('<div id="LQDataToolTip" style="position:relative;display:none"></div>');
$('body').append('<div id="LQDialog" style="position:relative;display:none"></div>');
$('body').append('<div id="LQEditForm" style="position:relative;display:none"></div>');

$('body').on('contextmenu','.LQDataCell',function(event){

	contextSelectedCell=$(this);
	if(contextSelectedCell.find(".LQEditor").length)return;

	$("#LQDataToolTip").css("position","absolute");
	$("#LQDataToolTip").css("display","block");
	$("#LQDataToolTip").css("left",event.pageX);
	$("#LQDataToolTip").css("top",event.pageY);


	log("start find mange !!!!!!!!!!!!!!!!!");
	var fInfo = gf(contextSelectedCell);
	var pushEditor="";
	pushEditor+='<div class="btn-group-vertical">';
	pushEditor+= '<button class="btn btn-default"><b><font color=orange>' + $(this).attr("field") + '</b></font></button>';
	if(fInfo["editor"]!=undefined && fInfo["manage"].toLowerCase().indexOf("update")>=0){
		pushEditor += '<button id=LQContextEditByEditorButton class="btn btn-default">Edit</button>';
		pushEditor +='<button id=LQContextEditByTextButton class="btn btn-default">TextEdit</button>';
	}
	pushEditor +='<button id=LQContextCloseButton class="btn btn-default">close</button>';
	pushEditor +='</div>';
	$("#LQDataToolTip").html(pushEditor);
	$(".LQContextButton").css("width",100);
	

	 $(this).blur();
 	return false;
});
$('body').on('click','*',function(event){
	$("#LQDataToolTip").css("display","none");
});
$('body').on('click','#LQContextEditByEditorButton',function(event){
	
	$("#LQDataToolTip").css("display","none");
	var fInfo = gf(contextSelectedCell);
	
	if(fInfo["table"].attr("editRowOnly")!="true"){
		fInfo["editor"]["rowData"]=fInfo["rowData"];
		var pushEditor = editorSelector(fInfo["editor"],fInfo["rowData"][fInfo["field"]]);
		contextSelectedCell.html("");
		contextSelectedCell.append(pushEditor);
		contextSelectedCell.append(makeManageButton(fInfo["manage"],true));	
		contextSelectedCell.blur();
	}else{
		startUpdateMode(contextSelectedCell);
	}

});



$('body').on('click','#LQContextEditByTextButton',function(event){
	$("#LQDataToolTip").css("display","none");
	var fInfo = gf(contextSelectedCell);
	// var pushEditor = editorSelector({"type":"textarea"},fInfo["rowData"][fInfo["field"]]);
	// pushEditor+=makeManageButton(fInfo["manage"],true);	
	contextSelectedCell.html("");
	contextSelectedCell.append(editorSelector({"type":"textarea"},fInfo["rowData"][fInfo["field"]]));
	contextSelectedCell.append(makeManageButton(fInfo["manage"],true));
	contextSelectedCell.blur();

});

$('body').on('click','#LQContextCloseButton',function(event){
	$("#LQDataToolTip").css("display","none");
});
$('body').on('mouseout','.LQDataCell',function(event){
	//$(".LQDataToolTip").css("display","none");
});

//수정취소/////////////////////////////////////////////////////////////////////////////////////////////
$('body').on('click','.LQModifyCancel',function(){
/*
	var selectedRow = $(this).parent().parent();
	var rowData = s2j(selectedRow.attr("data"));
	var dataTable = selectedRow.parent().parent();
*/

	var tInfo = gf($(this));
	var rowpos = tInfo["row"].position();
		//tInfo["row"].attr("bgcolor","cccccc"); 
	var findobj = tInfo["row"];

	if(tInfo["table"].hasClass("LQDataForm"))findobj = tInfo["table"];

	findobj.find("td").each(function(index,item){
		var fInfo = gf($(this));
		if(fInfo){
			if($(this).hasClass("LQDataCell")){
				//viewer 적용
				var viewValue = viewerSelector(fInfo["viewer"],fInfo["rowData"][fInfo["field"]]);
				$(this).html(viewValue);
			}else if($(this).hasClass("LQManageCell")){
				var viewValue = makeManageButton(fInfo["manage"],false);
				$(this).html(viewValue);
			}
		}
	});


	$('html, body').animate({scrollTop:rowpos.top-100}, 'fast');
	
});

//수정완료/////////////////////////////////////////////////////////////////////////////////////////////
$('body').on('click','.LQModifyApply',function(){
	requestUpdate($(this));
});

$('body').on('click','.LQSendButton',function(){

	requestUpdate($(this));
	

});

//삭제/////////////////////////////////////////////////////////////////////////////////////////////
$('body').on('click','.LQDelete',function(){
	//수정사항 디비로보내기
	
	var tInfo = gf($(this));

	//if(!confirm("really?"))return;


	var param={"gid":gid,"mode":"delete","table":tInfo["dbTable"],"primaryKey":tInfo["primaryKey"],"primaryValue":tInfo["primaryValue"],"dbMode":"delete"};
	

	if(typeof(tInfo["dbClass"]) != "undefined")
	{	
		var lastData = s2j(tInfo["dbLoadParam"]);
		lastData["shardIndex"]=tInfo["shardIndex"];
		lastData["data"]=tInfo["rowData"];
		param={"gid":gid,"dbClass":tInfo["dbClass"],"dbFunc":tInfo["deleteFunc"],"param":j2s(lastData),"dbMode":"delete"};
	}

	
	
	if(tInfo["commenter"]){
		var pushData = editorSelector(tInfo["commenter"],"");
		alert("Comment", "삭제하시겠습니까?<br>"+pushData,function(){

		} ,{"Delete":function(){
			var cmt = getObjValue($(this).find(".LQEditor:first"));
			var nP = s2j(param["param"]);
			nP["reason"]=j2s(cmt);
			nP["enableComment"]=true;
			param["param"]=j2s(nP);
			deleteApplyAjax(param,tInfo);

			$(this).dialog( "close" );		
		}});
	}else{
		var pushData = editorSelector(tInfo["commenter"],"");
		alert("Comment", "삭제하시겠습니까?",function(){} ,{"Delete":function(){
			deleteApplyAjax(param,tInfo);
			$(this).dialog( "close" );		
		}});	
	}
});

var  deleteApplyAjax = function(param,tInfo){
	log(param);
	$.ajax({
	    url : tInfo["dbSource"], 
	    data : param,
	    dataType : "json", 
	    type : "post",
	    success : function(data){ 

	    printLog(data);
	    log(j2s(data));
			if(data['result']['code']==1){
	   			//값재설정
	   			tInfo["row"].remove();
				log("delete success");
			}else{
   				alert("error",data['result']['message']);
			}
		},
		error: function(e) {
	    	alert("network error",j2s(e));
		}
	});
}
//추가/////////////////////////////////////////////////////////////////////////////////////////////
$('body').on('click','.LQAddCancel',function(){
	var tInfo = gf($(this));
	tInfo["row"].remove();
});



var requestInsert = function(obj,modal){
	//수정사항 디비로보내기
	//if(!confirm("really?"))return;
	
	var tInfo = gf(obj);
	var rowDatas = {};
	
	//필드데이터모으기
	tInfo["row"].find("td").each(function(index,item){
		var fInfo = gf($(this));
		//추가할row 만들기
		if($(this).hasClass("LQDataAddCell")){
			//값찾기

			if(fInfo["editorObj"])fInfo["editorObj"].triggerHandler("saveCallBack");
			var newValue=fInfo["editorValue"];
			if(newValue != undefined && newValue !=rowDatas[fInfo["field"]] && fInfo["isVirtual"]==false){
				rowDatas[fInfo["field"]]=newValue;
			}
		}
	});
	
	var param={"gid":gid,"mode":"insert","table":tInfo["dbTable"],"data":j2s(rowDatas),"primaryKey":tInfo["primaryKey"],"primaryValue":tInfo["primaryValue"],"dbMode":"insert"};
	//서버로보내기

	if(typeof(tInfo["dbClass"]) != "undefined")
	{
		var dbMode = "insert";
		var dbFunc = tInfo["insertFunc"];
		if(tInfo["table"].attr("startMode")=="update" || tInfo["table"].hasClass("LQEditForm")){
			dbFunc=obj.attr("func");
			dbMode = "custom";
		}


		var loadparam = s2j(tInfo["dbLoadParam"]);
		loadparam["data"]=rowDatas;		
		param={"gid":gid,"dbClass":tInfo["dbClass"],"dbFunc":tInfo["writeFunc"],"param":j2s(loadparam),"dbMode":"insert"};
		param["shardIndex"]=tInfo["shardIndex"];
	}

	
	if(tInfo["commenter"]){
		var pushData = editorSelector(tInfo["commenter"],"");
		alert("Comment", pushData,function(){

		} ,{"Modify":function(){
			var cmt = getObjValue($(this).find(".LQEditor:first"));
			var nP = s2j(param["param"]);
			nP["enableComment"]=true;
			nP["reason"]=j2s(cmt);
			param["param"]=j2s(nP);
			insertApplyAjax(param,tInfo,modal);
			$(this).dialog( "close" );		
		}});
	}else{
		insertApplyAjax(param,tInfo,modal);	
	}
}

$('body').on('click','.LQAddStart',function(){
	var tInfo = gf($(this));
	
	if(tInfo["table"].attr("editType")!="form"){
		var addForm=$("<tr>").addClass("LQDataAddRow"); //"<tr class='LQDataAddRow'>"; 

		tInfo["table"].find("th").each(function(index,item){
					var field = $(this).attr('field');
					var manageOption = $(this).attr('manage');
					var editor = $(this).attr('editor');
					var viewer = $(this).attr('viewer');
					var defValue ="";
					if(typeof(tInfo["defaultData"])=="object" && typeof(tInfo["defaultData"][field])!="undefined")defValue=tInfo["defaultData"][field];
					if($(this).attr('manage')==undefined){
						//primary key 속성 넣어주기
						if(editor){
							//viewer 적용
							var editorView = editorSelector(editor,defValue);
							$("<td>").addClass("LQDataAddCell").attr("field",field).append(editorView).appendTo(addForm);
							//addForm+="<td class='LQDataAddCell' field='"+field+"'>"+editorView+"</td>";
						}else{
							//viewer 적용
							var editorView = viewerSelector(viewer,defValue);

							$("<td>").addClass("LQDataAddCell").attr("field",field).append(editorView).appendTo(addForm);
							//addForm+="<td class='LQDataAddCell' field='"+field+"'></td>";	
						}
					//메니저필드
					}else{

						var adFormTD = $("<td>").addClass("LQAddManageCell").attr("manage",manageOption).appendTo(addForm);
						$("<input>").attr("type","button").attr("value","Add").addClass("LQAdd btn btn-primary").appendTo(adFormTD);
						$("<input>").attr("type","button").attr("value","Cancel").addClass("LQAddCancel btn btn-danger").appendTo(adFormTD);
						//addForm+="<td class='LQAddManageCell' manage='"+manageOption+"'>";
						//addForm+="<input type='button' value='Add' class='LQAdd btn btn-primary'>";
						//addForm+="</td>";
					}
			
		});

		if(tInfo["table"].find("th[manage]").attr("manage")!=undefined){
			if(tInfo["table"].find("th[manage]").attr("manage").indexOf("insert")!=-1){
				tInfo["table"].find('tbody[datazone]:first').prepend(addForm);
			}
		}
	}else{
		
		contextSelectedCell = tInfo["row"];
		var newDiv = $("#LQEditForm").html("");
		var newTable = $("<table>").appendTo(newDiv).addClass("table table-bordered LQDataForm LQEditForm").attr("data",j2s(tInfo["rowData"])).attr("startMode","update").attr("func","insertWithLQTable");
		
		var attrs = tInfo["table"][0].attributes;
		for(var i=0;i<attrs.length;i++) {
			if(attrs[i].nodeName!="class"){
				newTable.attr(attrs[i].nodeName,attrs[i].value);
				log(attrs[i].nodeName + " => " + attrs[i].value);
			}
		}


		tInfo["table"].find("th").each(function(index,item){
			var fInfo=gf($(this));
			if($(this).attr("field")){
				var nTR = $("<tr>").appendTo(newTable);
				var nTD = $("<td>").appendTo(nTR).append(fInfo["title"]);
				var nTD2 = $("<td>").appendTo(nTR).addClass("LQDataAddCell").attr("field",fInfo["field"]);
				var attrs = $(this)[0].attributes;
				for(var i=0;i<attrs.length;i++) {
					nTD2.attr(attrs[i].nodeName,attrs[i].value);
				}
				
				if(fInfo["editor"]!=undefined)viewValue = editorSelector(fInfo["editor"],"");
				else viewValue = viewerSelector(fInfo["viewer"],"");
				nTD2.append(viewValue);
			}
		});
		
		
		newDiv.attr("title","Insert");
		newDiv.dialog({
		  beforeClose: function(){},
	      modal: true,
	      width:800,
	      height:800,
	      buttons:{"Save":function(){
	      	requestInsert(newTable,$(this));
	      }}
	    });
	}

});

$('body').on('click','.LQAdd',function(){



	requestInsert($(this));
});


$('body').on('click','.LQLoadNext',function(){		
	var tInfo = gf($(this));
	loadDataTable($(this),"after");
});



////////////////////////////////// init ////////////////////////

$('body').on('click','.radio-btn>button',function(){
	var radioID = $(this).attr("id");
	$('button[id='+radioID+']').each(function(index){
		$(this).removeClass("active");
	});
	$(this).addClass("active");
});

$("#findNo").keyup(function(event){
    if(event.keyCode == 13){
       $("#findUserInfo").trigger("click");
       $("#findNo").blur();
    }
});

$(".LQAdd .LQDelete .LQModify .LQModifyApply .LQModifyCancel").blur();

	var makeHeadInfo = function(){
	log("make head info");
	var a = "";
	a+='public static function loadWithLQTable($p){\n';
	$(".LQDataTable").find("th").each(function(index,item){
		//log("test");
		item = $(this);
		//order","viewer"=>$textViewer,"editor"=>$intEditor);
		a += '$data["head"][]=array(';
		if(item.attr("manage"))a+= '"manage"=>"'+item.attr("manage")+'"';
		if(item.attr("field"))a+= '"field"=>"'+item.attr("field")+'"';
		if(item.attr("viewer"))a+= ',"viewer"=>json_decode(\''+item.attr("viewer")+'\',true)';
		if(item.attr("editor"))a+= ',"editor"=>json_decode(\''+item.attr("editor")+'\',true)';
		if(item.attr("title"))a+= ',"title"=>"'+item.html()+'"';
		if(item.attr("primary")=="")a+= ',"primary"';
		a+=');\n';
		
	});
	a+='$data["head"][]=array("manage"=>"update delete insert");\nreturn $data;\n}\n';
	log(a);
	}

	makeHeadInfo();

	
});

var requestUpdate = function(obj,modal){
	var tInfo = gf(obj);
	var changeData={};
	var oldData={};
	var callbackFunc = obj.attr("callbackFunc");
	if(!tInfo["rowData"])tInfo["rowData"]={};


	//always 속성 가진놈들 처리하기.
	tInfo["table"].find("[always]").each(function(){
		changeData[$(this).attr("field")] = tInfo["rowData"][$(this).attr("field")];
	});

	//전체수정
	if(obj.parent().hasClass("LQManageCell") || tInfo["table"].attr("startMode")=="update" || tInfo["table"].attr("editRowOnly")=="true"){
		//셀들찾기
		tInfo["row"].find("td").each(function(index,item){
			if(!$(this).hasClass("LQDataCell") && !$(this).attr("field"))return;
			// ##################  셋벨류 하기~~~
			var fInfo = gf($(this));
			//값찾기
			log("what da fuck cell is "+$(this).attr("field"));
			
			if(fInfo && typeof(fInfo["editor"])!="undefined"){
				if(fInfo["editorObj"])fInfo["editorObj"].triggerHandler("saveCallBack");
				var newValue=fInfo["editorValue"];
				//getObjValue($(this).find(".LQEditor:first"),0);

				log("click LQModifyApply newvalue is "+j2s(newValue)+" in Finfo is"+fInfo["editorValue"]);
				
				if(newValue != undefined && newValue !=fInfo["value"] && fInfo["isVirtual"]!=true){
					oldData[fInfo["field"]]=tInfo["rowData"][fInfo["field"]];
					changeData[fInfo["field"]]=newValue;
					//tInfo["rowData"][fInfo["field"]]=j2s(newValue);
				}
			}
		});
		//changeData = tInfo["rowData"];
	//부분수정
	}else{
		if(tInfo["editorObj"])tInfo["editorObj"].triggerHandler("saveCallBack");
		//log("test bbb"+abc);
		changeData[tInfo["field"]]=tInfo["editorValue"];
		oldData[tInfo["field"]]=tInfo["rowData"][tInfo["field"]];
	}
	
	//changeData[tInfo["primaryKey"]]=tInfo["rowData"][tInfo["primaryKey"]];

	var param;

	if(typeof(tInfo["dbClass"]) == "undefined")
	{
		param={"gid":gid,"mode":"update","table":tInfo["dbTable"],"primaryValue":tInfo["primaryValue"],"data":j2s(changeData),"oldData":j2s(oldData),"dbMode":"update"};
	}else{

		// var lastData = {"data":changeData,"shardIndex":tInfo["shardIndex"],"oldData":oldData};
		// lastData["primaryKey"]=tInfo["primaryKey"];
		// lastData["primaryValue"]=tInfo["primaryValue"];


		var lastData = s2j(tInfo["dbLoadParam"]); 
		lastData["data"]=changeData;
		lastData["shardIndex"]=tInfo["shardIndex"];
		lastData["oldData"]=oldData;
		lastData["primaryKey"]=tInfo["primaryKey"];
		lastData["primaryValue"]=tInfo["primaryValue"];

		var dbMode = "update";
		var dbFunc = tInfo["updateFunc"];
		if(tInfo["table"].attr("startMode")=="update"){
			dbFunc=obj.attr("func");
			dbMode = "custom";
		}
		param={"gid":gid,"dbClass":tInfo["dbClass"],"dbFunc":dbFunc,"param":j2s(lastData),"dbMode":dbMode};
	}
	param["callbackFunc"]=callbackFunc;

	log("shardIndex is "+tInfo["shardIndex"]);
	if(tInfo["table"].hasClass("LQDataForm"))param["type"]="form";
	else param["type"]="table";


	if(tInfo["commenter"]){
		var pushData = editorSelector(tInfo["commenter"],"");
		alert("Comment", pushData,function(){

		} ,{"Modify":function(){
			var cmt = getObjValue($(this).find(".LQEditor:first"));
			var nP = s2j(param["param"]);
			nP["enableComment"]=true;
			nP["reason"]=j2s(cmt);
			param["param"]=j2s(nP);
			modfiyApplyAjax(param,tInfo,modal);

			$(this).dialog( "close" );		
		}});
	}else{
		modfiyApplyAjax(param,tInfo,modal);	
	}

}


var modfiyApplyAjax = function(param,tInfo,modal){
	if(tInfo["table"].hasClass("LQEditForm")){
		tInfo = gf(contextSelectedCell);
	}
	log("param is "+j2s(param));

	$.ajax({
	    url : tInfo["dbSource"], 
	    data : param,
	    dataType : "json", 
	    type : "post",
	    success : function(data){ 

	    	printLog(data);
	    	var openSuccessAlert=true;
	    	if(param['callbackFunc'] != undefined){
	    		
	    		eval("if(typeof "+param['callbackFunc']+" != 'undefined') vFunc="+param['callbackFunc']+";");
	    		openSuccessAlert=vFunc(data);
	    	}

			if(data['result']['code']==1){
				if(modal)modal.dialog( "close" );
				log("result"+j2s(data));
	   			//값재설정
				tInfo["rowData"] = data["data"];

				tInfo["row"].attr("data",j2s(tInfo["rowData"]));
				
				//셀들찾기

				if(tInfo["table"].attr("startMode")!="update"){
					tInfo["row"].find("td").each(function(index,item){
						var fInfo=gf($(this));
						// ##################  셋벨류 하기~~~						
						//view모드로 복구					
						if($(this).hasClass("LQDataCell") && $(this).find(".LQEditor")){
							//viewer 적용
							
							var viewer = fInfo["viewer"];
							if(fInfo["isVirtual"]){
								viewer = s2j(viewer);
								viewer["rowData"]=tInfo["rowData"];
								viewer = j2s(viewer);
							}

							var viewValue = viewerSelector(viewer,fInfo["rowData"][fInfo["field"]]);
							$(this).html(viewValue);

						}else if($(this).hasClass("LQManageCell")){
							//버튼만들기
							var viewValue = makeManageButton(fInfo["manage"],false);
							$(this).html(viewValue);
						}
					});
				}else{
					if(openSuccessAlert)alert("success",j2s(data["result"]["message"])); // ############## 모두 초기화해야하나?
				}	
		

				var rowpos = tInfo["row"].position();
				$('html, body').animate({scrollTop:rowpos.top-100}, 'fast');
				tInfo["row"].attr("bgcolor","cccccc"); 

			}else{
   				alert("error",j2s(data["result"]["message"]));
			}
		}
		,error: function(e) {
	    	alert("netwark error",j2s(e));
	}
		});
}



var insertApplyAjax = function(param,tInfo,modal){
	if(tInfo["table"].hasClass("LQEditForm")){
		tInfo = gf(contextSelectedCell);
	}
	log("param is "+j2s(param));

	$.ajax({
	    url : tInfo["dbSource"], 
	    data : param,
	    dataType : "json", 
	    type : "post",
	    success :function(resultData){ 
	    	
	    	printLog(resultData);
	    	if(tInfo["table"].hasClass("LQEditForm")){
				tInfo = gf(contextSelectedCell);
			}

	    	log("add result data :"+j2s(resultData));

	   		 var data = resultData["data"];

			if(resultData['result']['code']==1){

				if(modal)modal.dialog( "close" );
	   			//값재설정
				log("add success");

				//if(resultData["no"] && tInfo["primaryKey"])rowData[tInfo["primaryKey"]]=resultData["no"];

				var pushData="";

				if(typeof(data[0])=="undefined"){
					if(tInfo["table"].find('tbody[datazone]:last > tr:first').length > 0)
						tInfo["table"].find('tbody[datazone]:last > tr:first').before(makeDataRow(data,tInfo["table"],"yellow"));
					else
						tInfo["table"].find('tbody[datazone]').append(makeDataRow(data,tInfo["table"],"yellow"));
				}else{					
					for(var i in data){
						var rowData = data[i];
						if(tInfo["table"].find('tbody[datazone]:last > tr:first').length > 0)
							tInfo["table"].find('tbody[datazone]:last > tr:first').before(makeDataRow(rowData,tInfo["table"],"yellow"));
						else
							tInfo["table"].find('tbody[datazone]').append(makeDataRow(data,tInfo["table"],"yellow"));
					}
				}

				
				
				//데이터 테이블에 집어넣기
				//tInfo["table"].find('tbody[datazone]:last > tr:first').after(pushData);
				

				if(tInfo["table"].attr("editType")!="form"){
					tInfo["row"].remove();
				}
				// rowData[tInfo["primaryKey"]] = data["data"]


				// mydata = s2j(tInfo["table"].find('tbody[datazone]:last > tr:first').after().attr("data"));
				// tInfo["rowData"] = data["data"];
				// tInfo["row"].attr("data",j2s(tInfo["rowData"]));

			}else{
   			alert("error",resultData['result']['message']);
   			//tInfo["table"].find('tr:last').remove();
			}
			return true;
			},
		error: function(e) {
			log(e);
	    	alert("network error",j2s(e));
		}
	});
}


