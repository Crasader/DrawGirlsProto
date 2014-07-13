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

var selectedMonster = function(obj,value){
	var data = s2j(value["data"]);
	//var row = obj.parent().parent();
	//var editor = row.find(".LQEditor");
	//var template = s2j(data["template"]);
	obj.parent().children(".LQEditor").val(data["type"]);
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
<form action=admin_stage.php>
	<input name="gid" value="<?=$gid?>" type="hidden">
조건 : <input name='where' size="50" value='<?=$_GET['where']?>'> <input type=submit value="확인">
</form>

</center>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::getMT("stage")?>" dbWhere="<?=$_GET['where']?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary editor='{"type":"text"}'>no</th>
			<th field="puzzle" viewer='{"type":"text"}'>puzzle (자동갱신)</th>
			<th field="book" viewer='{"type":"text"}'>book (자동갱신)</th>
			<th field="theme" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>theme</th>
			<th field="level" viewer='{"type":"text"}' editor='{"type":"text",{"datatype":"int"}}'>level</th>
			<th field="rewardInfo" viewer='{"type":"json"}' editor='{"type":"array","element":{"type":"text","datatype":"int"}}'>rewardInfo</th>
			<th field="scale" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>scale</th>
			<th field="autoBalanceTry" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>autoBalanceTry</th>
			<th field="boss" viewer='{"type":"text"}' editor='{"type":"table","element":[
			{"field":"shape","title":"모양","type":"select","element":["circle","snake"]},
			{"field":"type","title":"몬스터번호","type":"dataSelector","func":"selectedMonster","mode":"monster"},
			{
				"title":"패턴",
				"field":"pattern",
				"type":"array",
				"element":{
								"type":"dictionary",
								"element":[
									{"field":"pattern","type":"dataSelector","func":"selectedPattern","mode":"pattern"},
									{"field":"target","type":"select","element":["no","yes"]},
									{"field":"atype","type":"select","element":["normal","special","crash"]},
									{"field":"percent","type":"text","datatype":"int"}
					
				]}			
			},			
					
			{   
			    "title":"속도",
				"field":"speed","type":"dictionary",
				"element":[
									{"field":"max","type":"text","datatype":"int"},
									{"field":"start","type":"text","datatype":"int"},
									{"field":"min","type":"text","datatype":"int"}
								]
			},
			{
			    "title":"스케일",
				"field":"scale","type":"dictionary",
				"element":[
									{"field":"max","type":"text","datatype":"int"},
									{"field":"start","type":"text","datatype":"int"},
									{"field":"min","type":"text","datatype":"int"}
								]
			},
			{
			    "title":"움직임",
				"field":"movement","type":"dictionary",
				"element":[
									{"field":"normal","type":"text","datatype":"int"},
									{"field":"draw","type":"text","datatype":"int"}
								]
			},
			{"title":"공격주기","field":"attackpercent","type":"text","datatype":"int"},
			{"title":"에너지","field":"hp","type":"text","datatype":"int"}	,
			{"title":"민첩","field":"agi","type":"text","datatype":"int"}	,
			{"title":"AI","field":"ai","type":"text","datatype":"int"}		,
			{"title":"isflipx","field":"isflipx","type":"bool"}	
						
			]}'>boss</th>
			<th field="junior"  viewer='{"type":"json"}' editor='{"type":"table","element":[
			
			{"field":"type","title":"몬스터번호","type":"dataSelector","func":"selectedMonster","mode":"monster"},			
			{   
			    "title":"속도",
				"field":"speed","type":"dictionary",
				"element":[
									{"field":"max","type":"text","datatype":"int"},
									{"field":"start","type":"text","datatype":"int"},
									{"field":"min","type":"text","datatype":"int"}
								]
			},
			{
			    "title":"스케일",
				"field":"scale","type":"dictionary",
				"element":[
									{"field":"max","type":"text","datatype":"int"},
									{"field":"start","type":"text","datatype":"int"},
									{"field":"min","type":"text","datatype":"int"}
								]
			},
			{
			    "title":"움직임",
				"field":"movement","type":"dictionary",
				"element":[
									{"field":"normal","type":"text","datatype":"int"},
									{"field":"draw","type":"text","datatype":"int"}
								]
			},
			{"title":"에너지","field":"hp","type":"text","datatype":"int"}	,
			{"title":"민첩","field":"agi","type":"text","datatype":"int"}	,
			{"title":"AI","field":"ai","type":"text","datatype":"int"}	
						
			]}'>junior</td>
			<th field="mission" viewer='{"type":"json"}' editor='{"type":"dictionary","element":
			[
				{"field":"type","type":"text","datatype":"int"},
				{
				 "field":"option",
				 "type":"dictionary",
				 "element":[]
				}
			]}
			'>mission</th>
			<th field="point" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>point</th>
			<th field="scoreRate" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>scoreRate</th>
			<th field="playtime" viewer='{"type":"text"}' editor='{"type":"text"}'>playtime</th>
			<th field="shopItems" viewer='{"type":"json"}' editor='{"type":"table","element":
			[
				{
					"title":"아이템번호",
					"field":"type",
					"type":"text",
					"datatype":"int"
				},
				{
					"title":"통화",
					"field":"currency",
					"type":"select",
					"element":["gold","ruby","social"]
				},
				{
					"title":"가격",
					"field":"price",
					"type":"text",
					"datatype":"int"
				},
				{
					"title":"옵션",
					"field":"option",
					"type":"dictionary",
					"element":[]
				}

			]}'>shopItems</th>
			<th field="defItems"  viewer='{"type":"json"}' editor='{"type":"table","element":[
				{
					"title":"아이템번호",
					"field":"type",
					"type":"text",
					"datatype":"int"
				},
				{
					"title":"옵션",
					"field":"option",
					"type":"dictionary",
					"element":[]
				}

			]}'>defItems</th>
			<th field="cards"  viewer='{"type":"json"}' editor='{"type":"array","element":{"type":"text","datatype":"int"}}'>cards (자동갱신)</th>
			
			<th field="no" viewer='{"type":"text"}' primary>no</th> 
			<th field="pieceNo" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>pieceNo (자동갱신)</th>
			
			<th field="pieceType" viewer='{"type":"text"}' editor='{"type":"select","element":["h","w"]}'>pieceType</th>
		
			<th field="condition" viewer='{"type":"json"}' editor='{"type":"dictionary","element":[
				{"field":"gold","type":"text","datatype":"int"},
				{"field":"pieceNo","type":"text","datatype":"int"}			
			]}'>condition(해제조건)</th>

			<th field="minigame" viewer='{"type":"text"}' editor='{"type":"bool"}'>minigame</th>
			<th field="version" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>version</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>