<?php
include "header.php";
?>
<script>
var selectedPattern = function(obj,value){
	
	var data = s2j(value["data"]);
	var row = obj.parent().parent();
	var editor = row.parent().parent();
	
	
	var template = s2j(data["template"]); 
	
	if(!template || typeof(template) != "object")template={};
	if(!template["percent"])template["percent"]=1;
	if(!template["target"])template["target"]="no";
	if(!template["atype"])template["atype"]="normal";
	
	template["pattern"]=value["index"];
	
	var result = editorSelector(editor.attr("editor"),template,editor.attr("editorOption"));
	
	editor.parent().html(result);
	
	//LQEditorSetValue(editor,value);
	
}
var patternSelector = function(value,option){
	var editorObj = editorFunc_select(value,option);
	editorObj.addClass("patternSelector");
	$('body').on("change",".patternSelector",function(){
		var editorOption = s2j($(this).attr("editor"));
		var temp = editorOption["template"][$(this).val()];
		var md = {"data":{"template":temp},"index":$(this).val()};
		selectedPattern($(this),md);
	});
	return editorObj;
}
var selectedMonster = function(obj,value){
	var data = s2j(value["data"]);
	//var row = obj.parent().parent();
	//var editor = row.find(".LQEditor");
	//var template = s2j(data["template"]);
	obj.parent().children(".LQEditor").val(data["name"]);
	//LQEditorSetValue(editor,value);
	
}


var selectedPath = function(obj,value){
	var data = s2j(value["data"]);
	//var row = obj.parent().parent();
	//var editor = row.find(".LQEditor");
	//var template = s2j(data["template"]);
	obj.parent().children(".LQEditor").val(data["type"]);
	//LQEditorSetValue(editor,value);
	
}

			
$('body').on('click','.PathSelectorButton',function(){			
	openDataSelector($(this),$(this).attr("func"),$(this).attr("mode"));

});
var pathSelector = function(value,option){
	var r =  "<input type='text' value='"+value+"' datatype='"+option['datatype']+"' class='LQEditor' editor='dataSelector'>";
	r+="<input type=button value=선택 func='"+option["func"]+"' mode='"+option["mode"]+"' class='LQDataSelectorButton'>";
	return r;
}

</script>

<center>
<form action=admin_piece.php>
	<input name="gid" value="<?=$gid?>" type="hidden">
조건 : <input name='where' size="50" value='<?=$_GET['where']?>'> <input type=submit value="확인">
</form>

</center>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Piece" autoSetting="true" dbWhere='' name="datatable" border=1 align=center>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>




<?php
include "footer.php";
?>