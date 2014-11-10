<?php
include "manage_header.php";
?>
<script>
	var powerInfoEditor = function(value,option){
		value = s2j(value);
		if(typeof(value)!="object")value=[];
		var table = $("<table>").addClass("table").addClass("table-bordered").addClass("LQEditor").attr("editor",j2s(option));
		var title_tr = $("<tr>").appendTo(table).append("<td>level</td><td>power</td><td>price</td>");

		for(var i=0;i<30;i++){
			if(typeof(value[i])!="object")value[i]=[0,0];
			$("<tr>").addClass("powerRow").append(
				$("<td>").append(i+1),
				$("<td>").append(editorSelector({"type":"text","datatype":"int"}, value[i][0])),
				$("<td>").append(editorSelector({"type":"text","datatype":"int"}, value[i][1]))
			).appendTo(table);
		}

		return table;
	}

	var powerInfoEditor_value = function(obj){
		var r = [];
		var n = 0;
		obj.find(".powerRow").each(function(l){	
			var pow = getObjValue($(this).find(".LQEditor:first"));
			var price = getObjValue($(this).find(".LQEditor:last"));
			r.push([pow,price]);
		});
		return r;
	}

</script>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Character" dbWhere='{}' dbSort='{"serial":"asc"}' dbLimit="30" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='serial' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >serial</th>
		<th field='name' viewer='{"type":"koreanViewer"}' editor='{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}' >name</th>
		<th field='sale' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >sale</th>
		<th field='purchaseInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"value","type":"text","datatype":"int"}]}' >purchaseInfo</th>
		<th field='statInfo' viewer='{"type":"autoViewer"}' editor='{"type":"array","element":{"type":"dictionary"}}' >statInfo</th>
		<th field='resourceInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"ccbiID","type":"text"},{"field":"ccbi","type":"resourceSelector"},{"field":"imageID","type":"text"},{"field":"plist","type":"resourceSelector"},{"field":"pvrccz","type":"resourceSelector"},{"field":"size","type":"text","datatype":"int"}]}' >resourceInfo</th>
		<th field='missileInfo' viewer='{"type":"autoViewer"}' editor='{"type":"array","element":{"type":"dictionary"}}' >missileInfo</th>
		<th field='patternInfo' viewer='{"type":"autoViewer"}' editor='{"type":"array","element":{"type":"dictionary"}}' >patternInfo</th>
		<th field='missionInfo' viewer='{"type":"autoViewer"}' editor='{"type":"array","element":{"type":"dictionary"}}' >missionInfo</th>
		<th field='scriptInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary"}' >scriptInfo</th>
		<th field='comment' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":{"field":"kr","type":"array","element":{"type":"text"}}}' >comment</th>
		<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

</tbody>
</table>



<?php
include "manage_footer.php";
?>