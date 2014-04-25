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
<input name="gid" value="<?=$gid?>" type="hidden">
조건 : <input name='where' size="50" value='<?=$_GET['where']?>'> <input type=submit value="확인">
</form>

</center>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("card")?>" dbWhere="<?=$_GET['where']?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' editor='{"type":"text"}' primary>no</th>
			<th field="rank" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>rank(카드등급)</th>
			<th field="grade" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>grade(스테이지단계)</th>
			<th field="name" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>name(이름)</th>
			<th field="reward" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>reward(골드보상)</th>
			<th field="durability" viewer='{"type":"text"}' editor='{"type":"text"}'>durability</th>
			<th field="ability" viewer='{"type":"json"}' editor='{"type":"table","element":[{"title":"아이템번호","field":"type","type":"text","datatype":"int"},{"title":"옵션","field":"option","type":"dictionary"}]}'>ability</th>
			<th field="passive" viewer='{"type":"json"}' editor='{"type":"dictionary","element":[
			{"title":"operator","field":"operator","type":"select","element":["-","*(1-x)"]},	
			{
				"title":"패턴",
				"field":"pattern",
				"type":"dictionary",
				"element":[
					{"field":"castframe","type":"text","datatype":"int"},
					{"field":"totalframe","type":"text","datatype":"int"},
					{"field":"speed","type":"text","datatype":"int"}
				]			
			},			
					
			{   
			    "title":"속도",
				"field":"speed","type":"text","datatype":"int"
			},
			{   
			    "title":"스케일",
				"field":"scale","type":"text","datatype":"int"
			},
			{"title":"공격주기","field":"attackpercent","type":"text","datatype":"int"},
			{"title":"에너지","field":"hp","type":"text","datatype":"int"},
			{"title":"민첩","field":"agi","type":"text","datatype":"int"}	,
			{"title":"AI","field":"ai","type":"text","datatype":"int"}				
			]}'>passive(몬스터저주)</th>
			<th field="missile" viewer='{"type":"json"}' editor='{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"speed","type":"text","datatype":"int"},{"field":"power","type":"text","datatype":"int"},{"field":"dex","type":"text","datatype":"int"}]}'>missile</td>
			<th field="language" viewer='{"type":"text"}' editor='{"type":"select","element":["kr","jp","en","cn"]}'>language</th>
			<th field="stage" viewer='{"type":"text"}'>stage (자동갱신)</th>
			<th field="piece" viewer='{"type":"text"}'>piece (자동갱신)</th>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="imgInfo" viewer='{"type":"custom","func":"showCardImg"}' editor='{"type":"dictionary","element":[{"field":"img","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}'>imgInfo</td>
			
			<!--th field="thumbnailInfo" viewer='{"type":"json"}' editor="dictionary" editorOption='[{"field":"img","editor":"custom","editorOption":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]'>thumbnailInfo</td-->
			<th field="aniInfo" viewer='{"type":"json"}' editor='{"type":"dictionary","element":
			[
				{"field":"isAni","type":"bool"},
				{"field":"detail",
				 "type":"dictionary",
				 "element":[
								{"field":"img","type":"custom","func":"imageSelector"},
								{"field":"size","type":"text","datatype":"int"},
								{"field":"loopLength","type":"text","datatype":"int"},
								{"field":"loopSeq","type":"array","element":{"type":"text","datatype":"int"}},
								{"field":"cutWidth","type":"text","datatype":"int"},
								{"field":"cutHeight","type":"text","datatype":"int"},
								{"field":"cutLength","type":"text","datatype":"int"},
								{"field":"positionX","type":"text","datatype":"int"},
								{"field":"positionY","type":"text","datatype":"int"}
					]
						}
			]}'>aniInfo</th>
			<th field="script" viewer='{"type":"text"}' editor='{"type":"text"}'>script</th>
			<th field="silImgInfo" viewer='{"type":"text","cut":50}' editor='{"type":"dictionary","element":[{"field":"isSil","type":"bool"},{"field":"img","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}'>silImgInfo</td>
			<th manage="[delete,update,insert]">manage</th>
		</tr>
	</thead>
	<tbody datazone>
	</tbody>
</table>

<?php
include "footer.php";
?>