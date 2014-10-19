<?php
include "header.php";

$listViewer=array("type"=>"select","field"=>"type","title"=>"몬스터명");
while($pData = Monster::getRowByQuery("",null,"name,no")){
	$listViewer["element"][] = $pData["no"]."-".$pData["name"];
	$listViewer["value"][]=$pData["name"];
}

$listViewer2=array("type"=>"custom","func"=>"patternSelector","field"=>"pattern","title"=>"패턴");
while($pData = Pattern::getRowByQuery("",null,"type,name,template")){
	$listViewer2["element"][] = $pData["type"]."-".$pData["name"];
	$listViewer2["value"][]=$pData["type"];
	$listViewer2["template"][$pData["type"]]=$pData["template"];
}

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
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Piece" dbWhere='{}' dbLimit="100" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' editor='{"type":"text"}' >no</th>
		<th always field='puzzle' viewer='{"type":"text"}' >puzzle</th>
		<th field='level' viewer='{"type":"text"}' editor='null' >level</th>
		<th field='scale' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >scale</th>
		<th field='autoBalanceTry' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >autoBalanceTry</th>
		<th field='boss' viewer='{"type":"autoViewer"}' editor='{"type":"table","element":[{"field":"shape","title":"모양","type":"select","element":["circle","snake"]},<?=json_encode($listViewer)?>,{"title":"패턴","field":"pattern","type":"array","element":{"type":"dictionary","element":[<?=json_encode($listViewer2)?>,{"field":"target","type":"select","element":["no","yes"]},{"field":"atype","type":"select","element":["normal","special","crash"]},{"field":"percent","type":"text","datatype":"int"}]}},{"title":"속도","field":"speed","type":"dictionary","element":[{"field":"max","type":"text","datatype":"int"},{"field":"start","type":"text","datatype":"int"},{"field":"min","type":"text","datatype":"int"}]},{"title":"스케일","field":"scale","type":"dictionary","element":[{"field":"max","type":"text","datatype":"int"},{"field":"start","type":"text","datatype":"int"},{"field":"min","type":"text","datatype":"int"}]},{"title":"움직임","field":"movement","type":"dictionary","element":[{"field":"normal","type":"text","datatype":"int"},{"field":"draw","type":"text","datatype":"int"}]},{"title":"공격주기","field":"attackpercent","type":"text","datatype":"int"},{"title":"에너지","field":"hp","type":"text","datatype":"int"},{"title":"민첩","field":"agi","type":"text","datatype":"int"},{"title":"AI","field":"ai","type":"text","datatype":"int"},{"title":"isflipx","field":"isflipx","type":"bool"}]}' >boss</th>
		<th field='junior' viewer='{"type":"json"}' editor='{"type":"table","element":[{"type":"select","field":"type","title":"몬스터명","element":["113-boss_yellow","114-boss_yellow_jr","115-boss_tree","116-boss_tree_jr","117-boss_stone","118-boss_stone_jr","119-boss_plane","120-boss_plane_jr","121-boss_bee","122-boss_bee_jr","123-boss_octopus","124-boss_octopus_jr","125-boss_succubus","126-boss_succubus_jr","127-boss_snake","128-boss_snake_jr","129-boss_monk","130-boss_monk_jr","131-boss_yagyu","132-boss_yagyu_jr","133-boss_asuka","134-boss_asuka_jr","135-boss_bunny","136-boss_bunny_jr","137-boss_fox","138-boss_fox_jr","139-boss_snow","140-boss_snow_jr","141-boss_maid","142-boss_maid_jr","143-boss_bat","144-boss_bat_jr","145-boss_cyborg","146-boss_cyborg_jr","147-boss_cow","148-boss_cow_jr","149-boss_cat","150-boss_cat_jr","151-boss_mummy","152-boss_mummy_jr","153-boss_honeybee","154-boss_honeybee_jr","155-boss_alice","156-boss_alice_jr","157-boss_butterfly","158-boss_butterfly_jr","159-boss_penguin","160-boss_penguin_jr","161-boss_pumpkin","162-boss_pumpkin_jr","163-boss_nurse","164-boss_nurse_jr","165-boss_rain","166-boss_rain_jr","167-snake_08"],"value":["boss_yellow","boss_yellow_jr","boss_tree","boss_tree_jr","boss_stone","boss_stone_jr","boss_plane","boss_plane_jr","boss_bee","boss_bee_jr","boss_octopus","boss_octopus_jr","boss_succubus","boss_succubus_jr","boss_snake","boss_snake_jr","boss_monk","boss_monk_jr","boss_yagyu","boss_yagyu_jr","boss_asuka","boss_asuka_jr","boss_bunny","boss_bunny_jr","boss_fox","boss_fox_jr","boss_snow","boss_snow_jr","boss_maid","boss_maid_jr","boss_bat","boss_bat_jr","boss_cyborg","boss_cyborg_jr","boss_cow","boss_cow_jr","boss_cat","boss_cat_jr","boss_mummy","boss_mummy_jr","boss_honeybee","boss_honeybee_jr","boss_alice","boss_alice_jr","boss_butterfly","boss_butterfly_jr","boss_penguin","boss_penguin_jr","boss_pumpkin","boss_pumpkin_jr","boss_nurse","boss_nurse_jr","boss_rain","boss_rain_jr","snake_08"]},{"title":"속도","field":"speed","type":"dictionary","element":[{"field":"max","type":"text","datatype":"int"},{"field":"start","type":"text","datatype":"int"},{"field":"min","type":"text","datatype":"int"}]},{"title":"스케일","field":"scale","type":"dictionary","element":[{"field":"max","type":"text","datatype":"int"},{"field":"start","type":"text","datatype":"int"},{"field":"min","type":"text","datatype":"int"}]},{"title":"움직임","field":"movement","type":"dictionary","element":[{"field":"normal","type":"text","datatype":"int"},{"field":"draw","type":"text","datatype":"int"}]},{"title":"에너지","field":"hp","type":"text","datatype":"int"},{"title":"민첩","field":"agi","type":"text","datatype":"int"},{"title":"AI","field":"ai","type":"text","datatype":"int"},{"title":"isflipx","field":"isflipx","type":"bool"}]}' >junior</th>
		<th field='mission' viewer='{"type":"json"}' editor='{"type":"dictionary","element":[{"field":"type","type":"text","datatype":"int"},{"field":"option","type":"dictionary","element":[]}]}' >mission</th>
		<th field='scoreRate' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >scoreRate</th>
		<th field='playtime' viewer='{"type":"text"}' editor='{"type":"text"}' >playtime</th>
		<th field='shopItems' viewer='{"type":"json"}' editor='{"type":"table","element":[{"title":"아이템번호","field":"type","type":"text","datatype":"int"},{"title":"통화","field":"currency","type":"select","element":["gold","ruby","social"]},{"title":"가격","field":"price","type":"text","datatype":"int"},{"title":"옵션","field":"option","type":"dictionary","element":[]}]}' >shopItems</th>
		<th field='defItems' viewer='{"type":"json"}' editor='{"type":"table","element":[{"title":"아이템번호","field":"type","type":"text","datatype":"int"},{"title":"옵션","field":"option","type":"dictionary","element":[]}]}' >defItems</th>
		<th field='cards' viewer='{"type":"json"}' editor='{"type":"array","element":{"type":"text","datatype":"int"}}' >cards</th>
		<th virtual field='pieceNo' viewer='{"type":"text"}' editor='{"type":"text"}' >pieceNo</th>
		<th field='condition' viewer='{"type":"json"}' editor='{"type":"dictionary","element":[{"field":"gold","type":"text","datatype":"int"},{"field":"pieceNo","type":"text","datatype":"int"}]}' >condition</th>
		<th always field='version' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >version</th>
		<th field='type' viewer='{"type":"text"}' editor='{"type":"select","element":["normal","special","event","hell"]}' >type</th>
		<th field='scale' viewer='{"type":"text"}' editor='{"type":"text"}' >scale</th>
		<th title='오토레벨적용여부' field='autoLevel' viewer='{"type":"text"}' editor='{"type":"bool"}' >오토레벨적용여부</th>
		<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>




<?php
include "footer.php";
?>