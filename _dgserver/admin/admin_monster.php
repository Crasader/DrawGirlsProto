<?php
include "header.php";

$listViewer2=array("type"=>"custom","func"=>"patternSelector","field"=>"pattern","title"=>"패턴");
while($pData = Pattern::getRowByQuery("order by category",null,"type,name,template")){
	$listViewer2["element"][] = $pData["type"]."-".$pData["name"];
	$listViewer2["value"][]=$pData["type"];
	$listViewer2["template"][$pData["type"]]=$pData["template"];
}

?>
<script>

var patternSelector = function(value,option){
	var editorObj = editorFunc_select(value,option);
	editorObj.addClass("patternSelector");

	return editorObj;
}

$('body').on("change",".patternSelector",function(){
	var editorOption = s2j($(this).attr("editor"));
	var temp = editorOption["template"][$(this).val()];
	var md = {"data":{"template":temp},"index":$(this).val()};
	selectedPattern($(this),md);
});
</script>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Monster" dbWhere='{}' dbLimit="50" dbSort='{"puzzleNo":"asc"}' name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='puzzleNo' viewer='{"type":"text"}' editor='{"type":"text"}'>puzzleNo</th>
		<th field='type' viewer='{"type":"text"}' editor='{"type":"select","element":["circle","snake","jr"]}' >type</th>
		<th field='isBoss' viewer='{"type":"text"}' editor='{"type":"bool"}' >isBoss</th>
		<th field='name' viewer='{"type":"text"}' editor='{"type":"text"}' >name</th>
		<th field='image' viewer='{"type":"imageViewer"}' editor='{"type":"imageSelector"}' >image</th>
		<th field='resourceInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"ccbiID","type":"text"},{"field":"ccbi","type":"resourceSelector","viewer":"text"},{"field":"imageID","type":"text"},{"field":"plist","type":"resourceSelector","viewer":"text"},{"field":"pvrccz","type":"resourceSelector","viewer":"text"},{"field":"size","type":"text","datatype":"int"}]}' >resourceInfo</th>
		<th field='script' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"ko","type":"dictionary","element":[{"type":"text","field":"start"},{"type":"text","field":"clear"},{"type":"text","field":"fail"}]},{"field":"en","type":"dictionary","element":[{"type":"text","field":"start"},{"type":"text","field":"clear"},{"type":"text","field":"fail"}]}]}' >script</th>
		<th field='propInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"ai","type":"text","datatype":"int"},{"field":"speed","type":"text","datatype":"int"},{"field":"hp","type":"text","datatype":"int"},{"field":"scale","type":"text","datatype":"int"},{"field":"movement","type":"text","datatype":"int"},{"field":"isflipx","type":"bool"}]}' >propInfo</th>
		<th field='patternInfo' viewer='{"type":"autoViewer"}' editor='{"type":"array","element":<?=json_encode($listViewer2)?>}' >patternInfo</th>

		<th field='comment' viewer='{"type":"text"}' editor='{"type":"textarea"}' >comment</th>
		
		<th always field='version'>version</th>
		<th manage='[delete,update,insert]' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";

//LQDataTable 초기설정에 updateAllways=true 속성 추가하여 업데이트 요청시 항상 같이 날려주도록 하기. --> version 관리를 위해..
?>

