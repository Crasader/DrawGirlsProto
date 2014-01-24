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
조건 : <input name='where' size="50" value='<?=$_GET['where']?>'> <input type=submit value="확인">
</form>

</center>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_STAGE?>" dbWhere="<?=$_GET['where']?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary editor="text">no</th>
			<th field="puzzle" viewer="text" editor="text" editorOption='{"datatype":"int"}'>puzzle</th>
			<th field="theme" viewer="text" editor="text" editorOption='{"datatype":"int"}'>theme</th>
			<th field="level" viewer="text" editor="text" editorOption='{"datatype":"int"}'>level</th>
			<th field="scale" viewer="text" editor="text" editorOption='{"datatype":"int"}'>scale</th>
			<th field="autoBalanceTry" viewer="text" editor="text" editorOption='{"datatype":"int"}'>autoBalanceTry</th>
			<th field="boss" viewer="json" editor="table" editorOption='[
			{"field":"shape","title":"모양","editor":"select","editorOption":["circle","snake"]},
			{"field":"type","title":"몬스터번호","editor":"dataSelector","editorOption":{"func":"selectedMonster","mode":"monster"}},
			{
				"title":"패턴",
				"field":"pattern",
				"editor":"array",
				"editorOption":{
								"editor":"dictionary",
								"editorOption":[
									{"field":"pattern","editor":"dataSelector","editorOption":{"func":"selectedPattern","mode":"pattern"}},
									{"field":"target","editor":"select","editorOption":["no","yes"]},
									{"field":"atype","editor":"select","editorOption":["normal","special","crash"]},
									{"field":"percent","editor":"text","editorOption":{"datatype":"int"}}
					
				]}			
			},			
					
			{   
			    "title":"속도",
				"field":"speed","editor":"dictionary",
				"editorOption":[
									{"field":"max","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"start","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"min","editor":"text","editorOption":{"datatype":"int"}}
								]
			},
			{
			    "title":"스케일",
				"field":"scale","editor":"dictionary",
				"editorOption":[
									{"field":"max","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"start","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"min","editor":"text","editorOption":{"datatype":"int"}}
								]
			},
			{
			    "title":"움직임",
				"field":"movement","editor":"dictionary",
				"editorOption":[
									{"field":"normal","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"draw","editor":"text","editorOption":{"datatype":"int"}}
								]
			},
			{"title":"공격주기","field":"attackpercent","editor":"text","editorOption":{"datatype":"int"}},
			{"title":"에너지","field":"hp","editor":"text","editorOption":{"datatype":"int"}}	,
			{"title":"민첩","field":"agi","editor":"text","editorOption":{"datatype":"int"}}	,
			{"title":"AI","field":"ai","editor":"text","editorOption":{"datatype":"int"}}	
						
			]'>boss</th>
			<th field="junior"  viewer="json" editor="table" editorOption='[
			
			{"field":"type","title":"몬스터번호","editor":"dataSelector","editorOption":{"func":"selectedMonster","mode":"monster"}},			
			{   
			    "title":"속도",
				"field":"speed","editor":"dictionary",
				"editorOption":[
									{"field":"max","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"start","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"min","editor":"text","editorOption":{"datatype":"int"}}
								]
			},
			{
			    "title":"스케일",
				"field":"scale","editor":"dictionary",
				"editorOption":[
									{"field":"max","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"start","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"min","editor":"text","editorOption":{"datatype":"int"}}
								]
			},
			{
			    "title":"움직임",
				"field":"movement","editor":"dictionary",
				"editorOption":[
									{"field":"normal","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"draw","editor":"text","editorOption":{"datatype":"int"}}
								]
			},
			{"title":"에너지","field":"hp","editor":"text","editorOption":{"datatype":"int"}}	,
			{"title":"민첩","field":"agi","editor":"text","editorOption":{"datatype":"int"}}	,
			{"title":"AI","field":"ai","editor":"text","editorOption":{"datatype":"int"}}	
						
			]'>junior</td>
			<th field="mission" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"type","editor":"text","editorOption":{"datatype":"int"}},
				{
				 "field":"option",
				 "editor":"dictionary",
				 "editorOption":[]
				}
			]
			'>mission</th>
			<th field="point" viewer="text" editor="text" editorOption='{"datatype":"int"}'>point</th>
			<th field="scoreRate" viewer="text" editor="text" editorOption='{"datatype":"int"}'>scoreRate</th>
			<th field="playtime" viewer="text" editor="text">playtime</th>
			<th field="shopItems" viewer="json" editor="table" editorOption='
			[
				{
					"title":"아이템번호",
					"field":"type",
					"editor":"text",
					"editorOption":{"datatype":"int"}
				},
				{
					"title":"통화",
					"field":"currency",
					"editor":"select",
					"editorOption":["gold","ruby","social"]
				},
				{
					"title":"가격",
					"field":"price",
					"editor":"text",
					"editorOption":{"datatype":"int"}
				},
				{
					"title":"옵션",
					"field":"option",
					"editor":"dictionary",
					"editorOption":[]
				}

			]'>shopItems</th>
			<th field="defItems"  viewer="json" editor="table" editorOption='[
				{
					"title":"아이템번호",
					"field":"type",
					"editor":"text",
					"editorOption":{"datatype":"int"}
				},
				{
					"title":"옵션",
					"field":"option",
					"editor":"dictionary",
					"editorOption":[]
				}

			]'>defItems</th>
			<th field="cards"  viewer="json" editor="array" editorOption='{"title":"카드번호","field":"type","editor":"text","editorOption":{"datatype":"int"}}'>cards</th>
			
			<!--th field="thumbnail" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}},
				{
				 "field":"x",
				 "editor":"text",
				 "editorOption":{"datatype":"int"}
				},
				{
				 "field":"y",
				 "editor":"text",
				 "editorOption":{"datatype":"int"}
				}
				,
				{
				 "field":"path",
				 "editor":"table",
				 "editorOption":[
				 				{"title":"방향","field":"d", "editor":"text"},
				 				{"title":"위치","field":"c", "editor":"text", "editorOption":{"datatype":"int"}}
				 ]
				}
			]
			'>thumbnail</th-->
			<th field="no" viewer="text" primary editor="text">no</th>
			<th field="pieceNo" viewer="text" editor="text" editorOption='{"datatype":"int"}'>pieceNo</th>
			<!--th field="piece" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"image","editor":"custom","editorOption":"imageSelector","viewer":"custom","viewerOption":"imageViewer"},
				{"field":"size","editor":"text","editorOption":{"datatype":"int"}}			
			]
			'>piece</th-->
			<th field="pieceType" viewer="text" editor="select" editorOption='["h","w"]'>pieceType</th>
			<!--th field="x" viewer="text" editor="text" editorOption='{"datatype":"int"}'>x</th>
			<th field="y" viewer="text" editor="text" editorOption='{"datatype":"int"}'>y</th-->

			<th field="condition" viewer="json" editor="dictionary" editorOption='[
				{"field":"gold","editor":"text","editorOption":{"datatype":"int"}},
				{"field":"pieceNo","editor":"text","editorOption":{"datatype":"int"}}			
			]'>condition(해제조건)</th>

			<th field="minigame" viewer="text" editor="bool">minigame</th>
			<th field="version" viewer="text" editor="text" editorOption='{"datatype":"int"}'>version</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>