<?php
include "header.php";
?>
<script>
var showPuzzleImg = function(value,option){
	data = s2j(value);
	if(!data)return "";
	if(!data["image"]) return "";
	return '<img src='+data["image"]+' width=300>';
}
</script>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("book")?>" dbSort="`order` asc" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="order" viewer='{"type":"text"}' editor='{"type":"text"}'>order</th>
			<th field="isEvent" viewer='{"type":"text"}' editor='{"type":"select","element":["일반","이벤트","미노출"],"value":[0,1,2]}'>isEvent</th>
			<th field="title" viewer='{"type":"text"}' editor='{"type":"text"}'>title</th>
			<th field="thumbnail" viewer='{"type":"custom","func":"showPuzzleImg"}' editor='{"type":"dictionary","element":[
				{"field":"image","type":"custom","func":"imageSelector","viewer":{"type":"custom","func":"imageViewer"}},
				{"field":"size","type":"text","datatype":"int"}			
			]}'>thumbnail</th>
			
			<th field="center" viewer='{"type":"json"}' editor='{"type":"dictionary","element":
			[
				{"field":"image","type":"custom","func":"imageSelector","viewer":{"type":"custom","func":"imageViewer"}},
				{"field":"size","type":"text","datatype":"int"}			
			]}
			'>center</th>
			<th field="original" viewer='{"type":"json"}' editor='{"type":"dictionary","element":
			[
				{"field":"image","type":"custom","func":"imageSelector","viewer":{"type":"custom","func":"imageViewer"}},
				{"field":"size","type":"text","datatype":"int"}			
			]}
			'>original</th>
			<th field="face" viewer='{"type":"json"}'  editor='{"type":"dictionary","element":
			[
				{"field":"image","type":"custom","func":"imageSelector","viewer":{"type":"custom","func":"imageViewer"}},
				{"field":"size","type":"text","datatype":"int"}			
			]}
			'>face</th>
			<th field="map" viewer='{"type":"custom","func":"showPuzzleImg"}' editor='{"type":"dictionary","element":
			[
				{"field":"image","type":"custom","func":"imageSelector","viewer":{"type":"custom","func":"imageViewer"}},
				{"field":"size","type":"text","datatype":"int"}			
			]}
			'>map</th>
			<th field="coordinateInfo" viewer='{"type":"json"}' editor='{"type":"table","element":[
			{"title":"x","field":"x","type":"text","datatype":"int"},
			{"title":"y","field":"y","type":"text","datatype":"int"}					
			]}'>coordinateInfo</th>

			<th field="startPosition" viewer='{"type":"json"}' editor='{"type":"dictionary","element":[
				{"field":"x","type":"text","datatype":"int"},
				{"field":"y","type":"text","datatype":"int"}			
			]}
			'>startPosition</th>

			<th field="endPosition" viewer='{"type":"json"}' editor='{"type":"dictionary","element":[
				{"field":"x","type":"text","datatype":"int"},
				{"field":"y","type":"text","datatype":"int"}		
			]}
			'>endPosition</th>
			<th field="point" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>point</th>
			<th field="pathInfo" viewer='{"type":"json"}' editor='{"type":"array","element":{"type":"text","datatype":"int"}}'>pathInfo</th>
			<th field="cardInfo" viewer='{"type":"json"}' editor='{"type":"array","element":{"type":"array","element":{"type":"text","datatype":"int"}}}'>cardInfo</th>
			<th field="rewardInfo" viewer='{"type":"json"}'>rewardInfo (자동갱신)</th>
			<th field="levelInfo" viewer='{"type":"json"}'>levelInfo (자동갱신)</th>
			<th field="conditionInfo" viewer='{"type":"json"}'>conditionInfo (자동갱신)</th>
			<th field="ticket" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>ticket</th>
			<th field="point" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>point</th>
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