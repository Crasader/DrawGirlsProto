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

			var log = function(data){
				console.log(j2s(data));
			};
			

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
				window.open('./dataSelector.php?delkey='+delkey+'&mode='+mode,'LQDataselector'+delkey,'width=500 height=500 menubar=no status=no');
			}
			
			var getFieldInfo = function(obj){
				var result = {};
				var depth=0;
				var selectedObj = obj;
				
				while(1){
					if(selectedObj.attr("class")=="LQDataTable"){
						result["table"]=selectedObj;
						break;
					}else if(selectedObj.attr("class")=="LQDataRow" || selectedObj.attr("class")=="LQDataAddRow"){
						result["row"]=selectedObj;
						result["table"]=result["row"].parent().parent();
						break;
					}else if(selectedObj.attr("class")=="LQDataCell" || selectedObj.attr("class")=="LQDataAddCell" || selectedObj.attr("class")=="LQManageCell"){
						result["cell"]=selectedObj;
						result["row"]=result["cell"].parent();
						result["table"]=result["row"].parent().parent();
						break;
					}else{
						selectedObj = selectedObj.parent();
						if(depth>10)return null;
					}
					
					depth++;
				}
								
				result["thisClass"]=obj.attr("class");
				
				if(result["table"]){
					result["dbSource"]=result["table"].attr("dbSource");
					result["dbTable"] = result["table"].attr("dbTable");
					result["dbSort"] = result["table"].attr("dbSort");
					result["dbWhere"] = result["table"].attr("dbWhere");
					result["dbLimit"] = result["table"].attr("dbLimit");
					result["primaryKey"] = result["table"].find("th[primary]").attr("field");
					result["manageHeader"]= result["table"].find("th[manage]:first");
					result["manage"]= result["manageHeader"].attr("manage");
				}
				if(result["row"]){
					result["rowData"]=s2j(result["row"].attr("data"));
					try{
					result["primaryValue"]=result["rowData"][result["primaryKey"]];
					}catch(e){
					}
				}
				
				if(result["cell"]){
					result["field"]=result["cell"].attr("field");
					result["editor"] = result["cell"].children(".LQEditor:first");
					result["fieldHeader"]=result["table"].find("th[field="+result["field"]+"]:first");
					try{
						result["isPrimary"]=(type(result["fieldHeader"].attr("primary"))=="undefined")?false:true;
					}catch(e){
						result["isPrimary"]=false;
					}
					result["class"]=result["cell"].attr("class");
					result["editorName"]=result["fieldHeader"].attr("editor");
					result["editorOption"]=s2j(result["fieldHeader"].attr("editorOption"));
					result["viewerName"]=result["fieldHeader"].attr("viewer");
					result["viewerOption"]=s2j(result["fieldHeader"].attr("viewerOption"));
					result["editorValue"]=getObjValue(result["editor"]);
					try{
						result["value"]=result["rowData"][result["field"]];				
					}catch(e){
						result["value"]=undefined;		
					}
				}
				
				
				return result;
				
				
			}
			
			var gf = getFieldInfo;
			
			var nl2br = function(str, is_xhtml) {
					var breakTag = (is_xhtml || typeof is_xhtml === 'undefined') ? '<br ' + '/>' : '<br>'; // Adjust comment to avoid issue on phpjs.org display

					return (str + '').replace(/([^>\r\n]?)(\r\n|\n\r|\r|\n)/g, '$1' + breakTag + '$2');
			}	

			var s2j = function(text){
				try
				{
					var r = JSON.parse(text);
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
				var editorName = obj.attr("editor");
				var vFunc = undefined;
				var r;
				
				if(editorName != undefined){
					eval("if(typeof editorFunc_"+editorName+"_value != 'undefined') vFunc=editorFunc_"+editorName+"_value;");
				}
				
				if(typeof(vFunc)=="function"){
					r=vFunc(obj);
				}
				else{
					r=j2s(obj.val());
				}
				
				var datatype = obj.attr("datatype");
				
				if(datatype=="int"){
					//log("s to number!!");
					r=Number(r);
				}
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
			
			var viewerFunc_url = function(value,option){
				return "<a href='"+value+"' "+option+">"+value+"</a>";
			}
			
			var viewerFunc_image = function(value,option){
				if(!value)return"nothing";
				return "<img src='"+value+"' "+option+">"+value+"</a>";
			}
			
			var viewerFunc_json = function(value,option){
				if(typeof(value)=="string") return value;
				
				return j2s(value);
			}
			var viewerFunc_json = function(value,option){
				if(typeof(value)=="string") return value;
				
				return j2s(value);
			}
			var viewerFunc_text = function(value,option){
				if(typeof(value)=="object")value = j2s(value);
				if(typeof(value)=="undefined")value="";
				if(typeof(option)=="string")option=s2j(option);
				if(typeof(option)=="object" && option["cut"]>0) return value.substring(0,option["cut"]);
				else return nl2br(value);
			}
			
			/// editor ////////////////////////////////////////////////////////////////////////////
			
			var editorFunc_dataSelector = function(value,option){
				var r =  "<input type='text' value='"+value+"' datatype='"+option['datatype']+"' class='LQEditor' editor='dataSelector'>";
				r+="<input type=button value=선택 func='"+option["func"]+"' mode='"+option["mode"]+"' class='LQDataSelectorButton'>";
				return r;
			}
			
			var editorFunc_text = function(value,option){
				var _option = option; 
				
				var datatype = "";
				
				if(typeof(_option)=="object"){
					if("datatype" in _option)datatype=_option["datatype"];
				}
				
				if(typeof(value)=="number")
					datatype="int";
				
				return "<input type='text' value='"+value+"' datatype='"+datatype+"' class='LQEditor' editor='text'>";
			}
			

			var editorFunc_textarea = function(value,option){
				return "<textarea class='LQEditor' editor='textarea' cols=50 rows=8>"+j2s(value)+"</textarea>";
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
				if(typeof(_option) == "object")sList=_option;				
				var et = "<select class='LQEditor' datatype='"+datatype+"' editor='select'>";
				var op;
				for(var i in sList){
					if(value==sList[i])op = "selected";
					else op="";
					et+="<option value='"+sList[i]+"' "+op+">"+sList[i]+"</option>";
				}
				et += "</select>";
				return et;
			}
			var editorFunc_bool =function(value,option){
				var sList = [1,0];				
				var sListName = ["true","false"];				
				var et = "<select class='LQEditor' editor='bool'>";
				for(var i in sList){
					if(value==sList[i])op = "selected";
					else op="";
					et+="<option value='"+sList[i]+"' "+op+">"+sListName[i]+"</option>";
				}
				et += "</select>";
				return et;
				
			}
			var editorFunc_array = function(value,option){
				var _option = s2j(option);
				var _value = s2j(value);
				if(!_value)_value=[];
				
				if(typeof(_value)!="object")_value=[_value];
				
				log(_value);
				log(_option);
				
				var _result = "<table border=1 value='"+j2s(_value)+"' editorOption='"+j2s(_option)+"' class='LQEditor' editor='array'><tbody>";
				
				//타이틀
				_result+="<tr>";
				_result+="<td colspan=2>";
				_result+="<input type='button' value='add' class='LQJSONArrayEditorAdd'></td>";
				_result+="</tr>";
				
				for(var j in _value){
					_result+="<tr datarow>";
					_result+="<td datafield>";	
					_result+= editorSelector(_option["editor"],_value[j],_option["editorOption"]);
					_result+="</td>";
					_result+="<td><input type='button' value='delete' class='LQJSONArrayEditorDelete'></td>";
					_result+="</tr>";
				}
				_result+="</tbody></table>";
				
				return _result;

				
			}
			
			var editorFunc_array_value = function(obj){
				
				var fieldname = [];
				var dataTable = obj;
				var result=[];
				
				dataTable.children('tbody').children("tr[datarow]").each(function(index,item){
					var dtfield = $(this).children('td[datafield]');
					var editor = dtfield.children(".LQEditor:first");
					var vvalue = getObjValue(editor);
					
					result.push(vvalue);
				});
				return result;

			}
			
			var editorFunc_table = function(value,option){
				var _option = s2j(option);
				var _value = s2j(value);
				if(!_value)_value=[];
				var _result = "<table border=1 value='"+j2s(_value)+"' editorOption='"+j2s(_option)+"' class='LQEditor' editor='table'><tbody>";
					
				//타이틀
				_result+="<tr>";
				for(var i in _option){
					_result+="<td field='"+_option[i]["field"]+"'>";
					_result+=_option[i]["title"];
					_result+="</td>";
				}
				_result+="<td><input type='button' value='add' class='LQJSONTableEditorAdd'></td>";
				_result+="</tr>";
				for(var j in _value){
					_result+="<tr datarow>";
					
					if(_option.length>1){
						for(var i in _option){
							var fieldname = _option[i]["field"];
						 	_result+="<td field='"+fieldname+"' datafield>";	
						 	_result+= editorSelector(_option[i]["editor"],_value[j][fieldname],_option[i]["editorOption"]);
						 	_result+="</td>";
	
						}
					}else{
						_result+="<td field='LQTABLESINGLE' datafield>";	
						_result+= editorSelector(_option[0]["editor"],_value[j],_option[0]["editorOption"]);
						_result+="</td>";
					}
					 	_result+="<td><input type='button' value='delete' class='LQJSONTableEditorDelete'></td>";
					_result+="</tr>";
				}
				_result+="</tbody></table>";
				
				return _result;
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
				log("dict option is"+j2s(option));
				var _option = s2j(option);
				var _value = s2j(value);
				if(!_value)_value={};
				var keyEditor;
				var keyEditorOption;
				
				var isInVKey = false;
				//option 이랑 value랑 비교하여 value 에 없는 option  집어넣기
	
				
				for(var key in _option){
					var check=true;
					if(!keyEditor){
						keyEditor = _option[key]["keyEditor"];
						keyEditorOption = s2j(_option[key]["keyEditorOption"]);
						if(keyEditor){
							_option.splice(key, 1);
							continue;
						}
					}
					
					if(_option[key]["field"]=="VARIABLEKEY")isInVKey=true;
					
					for(var key2 in _value){
						log("check"+key2+"==="+_option[key]["field"]);
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
				log(_value);
				log("value is"+j2s(_value));
				
				var _result = "<table border=1 value='"+j2s(_value)+"' editorOption='"+j2s(_option)+"' class='LQEditor' editor='dictionary' keyEditor='"+j2s(keyEditor)+"' keyEditorOption='"+j2s(keyEditorOption)+"'><tbody>";


				
				_result+="<tr><td colspan=3><input type='text' class='LQDictFieldNameText'>"
				
				_result+="<select class='LQDictEditorSelect'>";
				
				//옵션이랑 같은 필드 추가
				if(isInVKey)_result+="<option value='VARIABLEKEY'>VARIABLE FIELD</option>";
				
				_result+="<option value='text(number)'>text(number)</option><option value='text(string)'>text(string)</option><option value='textarea'>textarea</option><option value='dictionary'>dictionary</option><option value='array(number)'>array(number)</option><option value='array(string)'>array(string)</option></select>";
				
				_result+="<input type='button' value='add' class='LQJSONDictionaryEditorAdd'></td></tr>";
				
				
				_result+="<tr><td>key</td><td>value</td><td></td></tr>";
				
				for(var j in _value){
					_result+="<tr datarow><td>";
					
					_result+="<input type='text' value='"+j+"' class='LQDictFieldName'>";
					
					if(keyEditor){
						log(keyEditor);
						
						_result+="<input type=button value=선택 func='"+keyEditorOption["func"]+"' mode='"+keyEditorOption["mode"]+"' class='LQDataSelectorButton'>";
					}
					
					_result+="</td><td field='"+j+"' datafield>";
					
					var _op2="";
					for(var k in _option){
						if(_option[k]["field"]==j)_op2=_option[k]["editorOption"];
					}
							
					if(_op2=="")
						if(typeof(_option[0])!="undefined" && _option[0]["field"]=="VARIABLEKEY")_op2 = _option[0]["editorOption"];
						
						
					var _op1="";
					for(var k in _option)
						if(_option[k]["field"]==j)_op1=_option[k]["editor"];

				
					if(_op1=="")
					if(typeof(_option[0])!="undefined" && _option[0]["field"]=="VARIABLEKEY")_op1 = _option[0]["editor"];
						
					//if(_op1=="")_op1="text";

					
					
					_result+= editorSelector(_op1,_value[j],_op2);

					 _result+="</td><td><input type='button' value='delete' class='LQJSONTableEditorDelete'></td>";
					_result+="</tr>";
				}
				_result+="</tbody></table>";
				
				return _result;
			
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
				return vdictresult;
			
			}			
			///// selector  //////////////////////////////////////////////////////////////////////////////////////
			
			var viewerSelector = function(viewer,value,option){
				var viewValue="";
				switch(viewer){
					case "url": viewValue = viewerFunc_url(value,option); break;
					case "json": viewValue = viewerFunc_json(value,option); break;
					case "image": viewValue = viewerFunc_image(value,option); break;
					case "text": viewValue = viewerFunc_text(value,option); break;
					case "custom": viewValue = eval(option+"(value,option);");break;
					default: viewValue = value; break; 
				}
				
				return viewValue;
			}
			
			var editorSelector = function(editor,value,option){
				value = s2j(value);
				log("editor is "+editor);
				if(!editor || typeof(editor)=="undefined"){
					if(typeof(value)=="number"){ editor="text"; option=[]; option["datatype"]="int";}
					if(typeof(value)=="string" && value.length<100) editor="text";
					if(typeof(value)=="string" && value.length>100) editor="textarea";
					if(typeof(value)=="object" && value[0]!=undefined) editor="array";
					else if(typeof(value)=="object") editor="dictionary";
					
					
					log("sleectedfajsdflkasdlfk "+typeof(value)+"editor is "+editor);
				}
				var pushEditor="";
				switch(editor){
					case "custom": pushEditor = eval(option+"(value,option);");break;
					case "textarea": pushEditor = editorFunc_textarea(value,option);break;
					case "dataSelector":pushEditor=editorFunc_dataSelector(value,option);break;
					case "text":pushEditor=editorFunc_text(value,option);break;
					case "select":pushEditor=editorFunc_select(value,option);break;
					case "bool":pushEditor=editorFunc_bool(value,option);break;
					case "table":pushEditor=editorFunc_table(value,option);break;
					case "array":pushEditor=editorFunc_array(value,option);break;
					case "dictionary":pushEditor=editorFunc_dictionary(value,option);break;
					case "NONE":pushEditor=j2s(value);break;
					default:
					
					pushEditor=editorFunc_text(value,option);
					log("selected default editor");
					break;

				}
				return pushEditor;
			}
			
			var makeManageButton = function(manage,isEditorMode){
				var pushData="";
				manage = j2s(manage);
				if(isEditorMode){
					pushData = "<input type=button value='Apply' class='LQModifyApply'><input type=button value='Cancel' class='LQModifyCancel'>";
				}else{
					if(typeof(manage)=="string"){
						if(manage.indexOf("delete")!=-1)pushData+="<input type=button value='Delete' class='LQDelete'>";
						if(manage.indexOf("update")!=-1)pushData+="<input type=button value='Modify' class='LQModify'>";
					}
				}
				return pushData;
			}
			
			$(document).ready(function(){
				$('.LQDataTable').each(function(index,item){
					tInfo = gf($(this));
/*
					var LQDataTable = $(this);
					var dbSource = LQDataTable.attr('dbSource');
					var dbTable = LQDataTable.attr('dbTable');
					var dbSort = LQDataTable.attr('dbSort'); 
*/					
					var dbParam = {"table":tInfo["dbTable"],"sort":tInfo["dbSort"],"limit":tInfo["dbLimit"],"where":tInfo["dbWhere"]};
					
					log(j2s(dbParam));
				
					//추가폼 만들기
					var addForm="<tr class='LQDataAddRow'>";
					tInfo["table"].find("thead > tr > th").each(function(index,item){
								var field = $(this).attr('field');
								var manageOption = $(this).attr('manage');
								var editor = $(this).attr('editor');
								var editorOption = $(this).attr('editorOption');
								if($(this).attr('manage')==undefined){
									//viewer 적용
									var editorView = editorSelector(editor,"",editorOption);
									
									//primary key 속성 넣어주기
									addForm+="<td class='LQDataAddCell' field='"+field+"'>"+editorView+"</td>";
								
								//메니저필드
								}else{
									addForm+="<td class='LQAddManageCell' manage='"+manageOption+"'>";
									addForm+="<input type='button' value='Add' class='LQAdd'>";
									addForm+="</td>";
								}
						
					});
					addForm+="</tr>";
					if(tInfo["table"].find("th[manage]").attr("manage")!=undefined)
						if(tInfo["table"].find("th[manage]").attr("manage").indexOf("insert")!=-1)
							tInfo["table"].find('tbody[datazone]:last').append(addForm);

					
		   		$.ajax({
				    url : tInfo["dbSource"], 
				    data : dbParam,
				    dataType : "json", 
				    type : "post",
				    success : function(data){
						//받아온 data와 표시할정보 비교해서 뿌릴데이터모으기
						var pushData="";
						for(var i in data){
							var rowData = data[i];
							pushData+="<tr data='"+j2s(rowData)+"' class='LQDataRow'>";
	
							tInfo["table"].find("thead > tr > th").each(function(index,item){
								var field = $(this).attr('field');
								var viewer = $(this).attr('viewer');
								var viewerOption = $(this).attr('viewerOption');
								var manage = $(this).attr('manage');
								var editor = $(this).attr('editor');
								var value=rowData[field];
								//데이터필드
								if(manage==undefined){
									//viewer 적용
									
									var viewValue = viewerSelector(viewer,value,viewerOption);
									
									//primary key 속성 넣어주기
									pushData+="<td class='LQDataCell' field='"+field+"'>"+viewValue+"</td>";
								
								//메니저필드
								}else{
									// delete='"+$(this).attr("delete")+"' modify='"+$(this).attr("modify")+"'
									pushData+="<td class='LQManageCell'>";
									pushData+=makeManageButton(manage,false);
									pushData+="</td>";
								}
								
							});
							
							pushData+="</tr>";
						}
						
						//데이터 테이블에 집어넣기
						tInfo["table"].find('tbody[datazone]:last').append(pushData);
					
					}
				,
				error: function(e) {
				    	alert("error : " + j2s(e));
				    }
				});
			
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
/*
				option = dataTable.attr("option");
				if(typeof(option)=="string")
					option = s2j(option);
*/
					
				
			//	for(i in option){
				//	log(option[i]);
				
					var fieldname = $(this).parent().children(".LQDictFieldNameText").val();
					var fieldeditor = $(this).parent().children(".LQDictEditorSelect").val();
					
					var option = dataTable.attr("editorOption");
					log("editor option is"+option);
					
					if(option != undefined)option=s2j(option);
					
					
					var keyEditor=dataTable.attr("keyEditor");
					var keyEditorOption=s2j(dataTable.attr("keyEditorOption"));
					
					
//					result+="<td><input type='text' value='"+fieldname+"' class='LQDictFieldName'></td><td datafield>";
					
					
					
					result+="<td>";
					
					result+="<input type='text' class='LQDictFieldName'>";
					
					if(keyEditor){
						result+="<input type=button value=선택 func='"+keyEditorOption["func"]+"' mode='"+keyEditorOption["mode"]+"' class='LQDataSelectorButton'>";
					}
					
					result+="</td><td datafield>";
					
					
					
					
					if(fieldeditor=="array(number)"){
						result+= editorSelector("array","",{"editor":"text","editorOption":{"datatype":"int"}});	
					}else if(fieldeditor=="array(string)"){
						result+= editorSelector("array","","");	
					}else if(fieldeditor=="text(number)"){
						result+= editorSelector("text","",{"datatype":"int"});	
					}else if(fieldeditor=="text(string)"){
						result+= editorSelector("text","","");
					}else if(fieldeditor=="VARIABLEKEY"){
						var vk;
						for(k in option){
							if(option[k]["field"]=="VARIABLEKEY")vk=k;
						}
						result+= editorSelector("dictionary","",option[vk]["editorOption"]);
					}else if(fieldeditor=="table"){
						result+= editorSelector(fieldeditor,"",[{"title":"카드번호","field":"type","editor":"text","editorOption":{"datatype":"int"}}]);	
					}else{
						result+= editorSelector(fieldeditor,"","");	
					}
					result+="</td><td><input type='button' value='delete' class='LQJSONTableEditorDelete'></td>";
			//	}
				result+="</tr>";
				
				dataTable.children('tbody').append(result);
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
 				var result="<tr datarow>";
				var option = dataTable.attr("editorOption");
				if(option)
					if(typeof(option)=="string")
						option = s2j(option);
					if(option.length>1){
						for(var i in option){
							var fieldname = option[i]["field"];
						 	result+="<td field='"+fieldname+"' datafield>";	
						 	result+= editorSelector(option[i]["editor"],"",option[i]["editorOption"]);
						 	result+="</td>";
						}
					}else{
						result+="<td field='LQTableSINGLE' datafield>";	
						result+= editorSelector(option[0]["editor"],"",option[0]["editorOption"]);
						result+="</td>";
					}
				
				result+="<td><input type='button' value='delete' class='LQJSONTableEditorDelete'></td></tr>";
				
				dataTable.children('tbody').append(result);
			});
			
			$('body').on('click','.LQJSONArrayEditorAdd',function(){
				//var tInfo = gf($(this));
 				var dataTable = $(this).parent().parent().parent().parent();
				var fieldeditor = $(this).parent().children(".LQArrayEditorSelect").val();
 				var result="<tr datarow>";
				var option = dataTable.attr("editorOption");
				if(option)
					if(typeof(option)=="string")
						option = s2j(option);
						

				var fieldname = option["field"];
			 	result+="<td field='"+fieldname+"' datafield>";	
			 	
			 	log("editor is"+fieldeditor);
			 	
			 	result+= editorSelector(option["editor"],"",option["editorOption"]);	
			 	result+="</td>";

				
				result+="<td><input type='button' value='delete' class='LQJSONArrayEditorDelete'></td></tr>";
				
				dataTable.children('tbody').append(result);
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
				var tInfo = gf($(this));
			/*
	var selectedRow = $(this).parent().parent();
				var dataTable = selectedRow.parent().parent();
				var rowData = s2j(selectedRow.attr("data"));
			
*/		
				tInfo["row"].find("td").each(function(index,item){
/*
					var field = $(this).attr("field");
					var fieldHeader = dataTable.find("th[field="+field+"]");
					var editorOption = fieldHeader.attr("editorOption");
					var clsType = $(this).attr("class");
					var editor = fieldHeader.attr("editor");
*/
					var fInfo=gf($(this));
					
						
					if(fInfo["class"]=="LQDataCell" && fInfo["editorName"]!=undefined){
						var viewValue = editorSelector(fInfo["editorName"],fInfo["rowData"][fInfo["field"]],fInfo["editorOption"]);//editorSelector(fInfo["viewerName"],fInfo["rowData"][fInfo["field"]],fInfo["viewerOption"]);
						$(this).html(viewValue);
					}else if(fInfo["class"]=="LQManageCell"){
						var viewValue = makeManageButton(fInfo["manage"],true);	
						$(this).html(viewValue);
					}
				});
			});
			
			
			$('body').on('dblclick','.LQDataCell',function(){
				if($(this).children(".LQModifyApply").length>0)return;
				
				var fInfo = gf($(this));
				
/*
				var selectedRow = $(this).parent();
				var dataTable = selectedRow.parent().parent();
				var rowData = s2j(selectedRow.attr("data"));
					var field = $(this).attr("field");
					var fieldHeader = dataTable.find("th[field="+field+"]");
					var editorOption = fieldHeader.attr("editorOption");
					var clsType = $(this).attr("class");
					var editor = fieldHeader.attr("editor");
					
					
*/
					if(fInfo["editorName"]==undefined)return;
					var pushEditor = editorSelector(fInfo["editorName"],fInfo["rowData"][fInfo["field"]],fInfo["editorOption"]);
					pushEditor+=makeManageButton(fInfo["manage"],true);	
					$(this).html(pushEditor);
		   });
			//수정취소/////////////////////////////////////////////////////////////////////////////////////////////
			$('body').on('click','.LQModifyCancel',function(){
/*
				var selectedRow = $(this).parent().parent();
				var rowData = s2j(selectedRow.attr("data"));
				var dataTable = selectedRow.parent().parent();
*/

				var tInfo = gf($(this));
				
				tInfo["row"].find("td").each(function(index,item){
					
					var fInfo = gf($(this));
					/*

					
					var field = $(this).attr("field");
					var fieldHeader = dataTable.find("th[field="+field+"]");
					var editor = fieldHeader.attr("editor");
					var clsType = $(this).attr("class");
					var viewerOption = fieldHeader.attr("viewerOption");
					var viewer = fieldHeader.attr('viewer');
					
*/
					if(fInfo){
						if(fInfo["class"]=="LQDataCell"){
							//viewer 적용
							var viewValue = viewerSelector(fInfo["viewerName"],fInfo["rowData"][fInfo["field"]],fInfo["viewerOption"]);
							$(this).html(viewValue);
						}else if(fInfo["class"]=="LQManageCell"){
							var viewValue = makeManageButton(fInfo["manage"],false);
							$(this).html(viewValue);
						}
					}
				});
				
			});
			
			//수정완료/////////////////////////////////////////////////////////////////////////////////////////////
			$('body').on('click','.LQModifyApply',function(){
			
				var tInfo = gf($(this));
				
/*
				var selectedRow = $(this).parent().parent();
				var rowData = s2j(selectedRow.attr("data"));
				var _dataTable = selectedRow.parent().parent();
*/
				
				//셀들찾기
				tInfo["row"].children("td").each(function(index,item){
				
					// ##################  셋벨류 하기~~~
					
					var fInfo = gf($(this));
/*
					var field = $(this).attr("field");
					var fieldHeader = _dataTable.find("th[field="+field+"]");
					var editor = fieldHeader.attr("editor");
					var clsType = $(this).attr("class");
					var viewerOption = fieldHeader.attr("viewerOption");
					var viewer = fieldHeader.attr('viewer');
*/				
					//값찾기
					if(fInfo["editorName"]!=undefined){
						var newValue=fInfo["editorValue"];
						//getObjValue($(this).find(".LQEditor:first"),0);
						if(newValue != undefined && newValue !=fInfo["value"]){
							tInfo["rowData"][fInfo["field"]]=j2s(newValue);
						}
					}
				});
				
				//수정사항 디비로보내기
				/*
var dbSource=_dataTable.attr("dbSource");
				var dbTable = _dataTable.attr("dbTable");
				var primaryKey = _dataTable.find("th[primary]").attr("field");
				log("primarykey is "+primaryKey);
				var primaryValue = rowData[primaryKey]; 
*/
				if(tInfo["primaryValue"] == undefined)log("don't find primaryvalue");
				
				param={"mode":"update","table":tInfo["dbTable"],"primaryKey":tInfo["primaryKey"],"primaryValue":tInfo["primaryValue"],"data":j2s(tInfo["rowData"])};
				
				
				$.ajax({
				    url : tInfo["dbSource"], 
				    data : param,
				    dataType : "json", 
				    type : "post",
				    success : function(data){ 
		   			if(data['result']=="ok"){
			   			//값재설정
						tInfo["rowData"] = data["data"];
						tInfo["row"].attr("data",j2s(tInfo["rowData"]));
						log("modify success");
						log(j2s(data));
						
										//셀들찾기
					tInfo["row"].children("td").each(function(index,item){
						var fInfo=gf($(this));
						// ##################  셋벨류 하기~~~
/*
					var field = $(this).attr("field");
					var fieldHeader = _dataTable.find("th[field="+field+"]");
					var editor = fieldHeader.attr("editor");
					var clsType = $(this).attr("class");
					var viewerOption = fieldHeader.attr("viewerOption");
					var viewer = fieldHeader.attr('viewer');
*/
									
					//view모드로 복구					
					if(fInfo["class"]=="LQDataCell" && fInfo["isPrimary"]==false){
						//viewer 적용
						var viewValue = viewerSelector(fInfo["viewerName"],fInfo["rowData"][fInfo["field"]],fInfo["viewerOption"]);
						$(this).html(viewValue);
					}else if(fInfo["class"]=="LQManageCell"){
						//버튼만들기
						var viewValue = makeManageButton(fInfo["manage"],false);
						$(this).html(viewValue);
					}
				});
				
		   			}else{
			   			alert("error:"+j2s(data));
		   			}
		   		}
		   		,error: function(e) {
				    	alert("error : " + j2s(e));
				}
		   		});
		   		

				
			});
			
			//삭제/////////////////////////////////////////////////////////////////////////////////////////////
			$('body').on('click','.LQDelete',function(){
				//수정사항 디비로보내기
				if(!confirm("really?"))return;
				
				var tInfo = gf($(this));
/*				
				var selectedRow = $(this).parent().parent();
				var dataTable = selectedRow.parent().parent();
				var rowData = s2j(selectedRow.attr("data"));
				var dbSource=dataTable.attr("dbSource");
				var dbTable = dataTable.attr("dbTable");
				var primaryKey = dataTable.find("th[primary]").attr("field");
				var primaryValue = rowData[primaryKey]; 
*/
				var param={"mode":"delete","table":tInfo["dbTable"],"primaryKey":tInfo["primaryKey"],"primaryValue":tInfo["primaryValue"]};
				
				log(param);
				
				
				$.ajax({
				    url : tInfo["dbSource"], 
				    data : param,
				    dataType : "json", 
				    type : "post",
				    success : function(data){ 
		   			if(data['result']=="ok"){
			   			//값재설정
			   			tInfo["row"].remove();
						log("delete success");
		   			}else{
			   			alert("error:"+data['result']);
		   			}
		   		},
		   		error: function(e) {
				    	alert("error : " + j2s(e));
				}
		   		});
			
			});
			//추가/////////////////////////////////////////////////////////////////////////////////////////////
			$('body').on('click','.LQAdd',function(){
				//수정사항 디비로보내기
				//if(!confirm("really?"))return;
				
				var tInfo = gf($(this));
				
/*
				var selectedRow = $(this).parent().parent();
				var dataTable = selectedRow.parent().parent();
				var dbSource=dataTable.attr("dbSource");
				var dbTable = dataTable.attr("dbTable");
				var primaryKey = dataTable.find("th[primary]").attr("field");
				var clsType = $(this).attr("class");
*/
				//primaryValue = rowData[primaryKey]; 
				
				var pushData ="";
				var rowData = {};
				
				//필드데이터모으기
				tInfo["row"].children("td").each(function(index,item){
					
					var fInfo = gf($(this));
/*
					var field = $(this).attr("field");
					var fieldHeader = dataTable.find("th[field="+field+"]");
					var editor = fieldHeader.attr("editor");
					var clsType = $(this).attr("class");
					var viewerOption = fieldHeader.attr("viewerOption");
					var viewer = fieldHeader.attr('viewer');
*/
					log("classsssss"+fInfo["class"]+"000"+fInfo["thisClass"]);
					
					//추가할row 만들기
					if(fInfo["thisClass"]=="LQDataAddCell"){
						//값찾기
						var newValue=fInfo["editorValue"];
						if(newValue != undefined && newValue !=rowData[fInfo["field"]]){
							rowData[fInfo["field"]]=j2s(newValue);
						}else{
							rowData[fInfo["field"]]="";
						}
						//viewer 적용
						var viewValue = viewerSelector(fInfo["viewer"],j2s(newValue),fInfo["viewerOption"]);
						//primary key 속성 넣어주기
						pushData+="<td class='LQDataCell' field='"+fInfo["field"]+"'>"+viewValue+"</td>";
					
					//메니저필드
					}else if(fInfo["thisClass"]=="LQAddManageCell"){
						// delete='"+$(this).attr("delete")+"' modify='"+$(this).attr("modify")+"'
						log("manage option is"+tInfo["manage"]+"-finfo--"+fInfo["manage"]);
						pushData+="<td class='LQManageCell'>";
						//manageHeader=dataTable.find("th[manage]");
						pushData += makeManageButton(fInfo["manage"],false);
						pushData += "</td>";
					}
				});
				
				pushData+="</tr>";
				
				pushData="<tr data='"+j2s(rowData)+"' class='LQDataRow'>"+pushData;
				
				//데이터 테이블에 집어넣기
				tInfo["table"].find('tbody[datazone]:last').append(pushData);
				
				var param={"mode":"insert","table":tInfo["dbTable"],"data":j2s(rowData)};
				//서버로보내기
				log(j2s(param));
				$.ajax({
				    url : tInfo["dbSource"], 
				    data : param,
				    dataType : "json", 
				    type : "post",
				    success :function(data){ 
				    
				    log(data);
		   			if(data['result']=="ok"){
			   			//값재설정
						log("add success");
						
		   			}else{
			   			alert("error :"+data['result']);
			   			tInfo["table"].find('tr:last').remove();
		   			}
		   			return true;
		   			},
		   		error: function(e) {
		   			log(e);
				    	alert("error : " + j2s(e));
				}
		   		});
			
			});
			////////////////////////////////// init ////////////////////////

				
			});
		