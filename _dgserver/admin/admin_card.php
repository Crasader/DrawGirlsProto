<?php
include "header.php";
?>


<?
/*
	viewer - [text], time, price, json, image, url, custom(option:func), bool
	editor - [lock],text, number, time, json, image, select, custom:(option:func)
	                                            option: ["select1","select2"]
	
	
	
*/
?>
<script>
var showCardImg = function(value,option){
	data = s2j(value);
	return '<img src=../images/'+data["img"]+' width=100>';
}
</script>
<center>
<form action=admin_card.php>
조건 : <input name='where' size="50" value='<?=$_GET['where']?>'> <input type=submit value="확인">
</form>

</center>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=$TABLE_CARD?>" dbWhere="<?=$_GET['where']?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer="text" primary editor="text">no</th>
			<th field="rank" viewer="text" editor="text" editorOption='{"datatype":"int"}'>rank(카드등급)</th>
			<th field="grade" viewer="text" editor="text" editorOption='{"datatype":"int"}'>grade(스테이지단계)</th>
			<th field="reward" viewer="text" editor="text" editorOption='{"datatype":"int"}'>reward(골드보상)</th>
			<th field="durability" viewer="text" editor="text">durability</th>
			<th field="ability" viewer="json" editor="table" editorOption='[{"title":"아이템번호","field":"type","editor":"text","editorOption":{"datatype":"int"}},{"title":"옵션","field":"option","editor":"dictionary"}]'>ability</th>
			<th field="passive" viewer="json" editor="dictionary" editorOption='[
			{"title":"operator","field":"operator","editor":"select","editorOption":["-","*(1-x)"]},	
			{
				"title":"패턴",
				"field":"pattern",

								"editor":"dictionary",
								"editorOption":[
									{"field":"castframe","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"totalframe","editor":"text","editorOption":{"datatype":"int"}},
									{"field":"speed","editor":"text","editorOption":{"datatype":"int"}}
					
				]			
			},			
					
			{   
			    "title":"속도",
				"field":"speed","editor":"text",
				"editorOption":{"datatype":"int"}
			},
			{   
			    "title":"스케일",
				"field":"scale","editor":"text",
				"editorOption":{"datatype":"int"}
			},
			{"title":"공격주기","field":"attackpercent","editor":"text","editorOption":{"datatype":"int"}},
			{"title":"에너지","field":"hp","editor":"text","editorOption":{"datatype":"int"}}	,
			{"title":"민첩","field":"agi","editor":"text","editorOption":{"datatype":"int"}}	,
			{"title":"AI","field":"ai","editor":"text","editorOption":{"datatype":"int"}}				
			]'>passive(몬스터저주)</th>
			<th field="missile" viewer="json" editor="dictionary" editorOption='[{"field":"type","editor":"text"},{"field":"speed","editor":"text","editorOption":{"datatype":"int"}},{"field":"power","editor":"text","editorOption":{"datatype":"int"}},{"field":"dex","editor":"text","editorOption":{"datatype":"int"}}]'>missile</td>
			<th field="language" viewer="text" editor="select" editorOption='["kr","jp","en","cn"]'>language</th>
			<th field="stage" viewer="text" editor="text">stage</th>
			<th field="no" viewer="text" primary editor="text">no</th>
			<th field="imgInfo" viewer="custom" viewerOption="showCardImg" editor="dictionary" editorOption='[{"field":"img","editor":"custom","editorOption":"imageSelector"},{"field":"size","editor":"text","editorOption":{"datatype":"int"}}]'>imgInfo</td>
			
			<!--th field="thumbnailInfo" viewer="json" editor="dictionary" editorOption='[{"field":"img","editor":"custom","editorOption":"imageSelector"},{"field":"size","editor":"text","editorOption":{"datatype":"int"}}]'>thumbnailInfo</td-->
			<th field="aniInfo" viewer="json" editor="dictionary" editorOption='
			[
				{"field":"isAni","editor":"bool"},
				{"field":"detail",
				 "editor":"dictionary",
				 "editorOption":[
								{"field":"img","editor":"custom","editorOption":"imageSelector"},
								{"field":"size","editor":"text","editorOption":{"datatype":"int"}},
								{"field":"loopLength","editor":"text","editorOption":{"datatype":"int"}},
								{"field":"loopSeq","editor":"array","editorOption":{"field":"SINGLE","editor":"text","title":"loopSeq","editorOption":{"datatype":"int"}}},
								{"field":"cutWidth","editor":"text","editorOption":{"datatype":"int"}},
								{"field":"cutHeight","editor":"text","editorOption":{"datatype":"int"}},
								{"field":"cutLength","editor":"text","editorOption":{"datatype":"int"}},
								{"field":"positionX","editor":"text","editorOption":{"datatype":"int"}},
								{"field":"positionY","editor":"text","editorOption":{"datatype":"int"}}
					]
						}
			]'>aniInfo</th>
			<th field="script" viewer="text" editor="text">script</th>
			<th field="silImgInfo" viewer="text" viewerOption='{"cut":50}' editor="dictionary" editorOption='[{"field":"isSil","editor":"bool"},{"field":"img","editor":"custom","editorOption":"imageSelector"},{"field":"size","editor":"text","editorOption":{"datatype":"int"}}]'>silImgInfo</td>
			<th manage="[delete,update,insert]">manage</th>
		</tr>
	</thead>
	<tbody datazone>
	</tbody>
</table>

<?php
include "footer.php";
?>