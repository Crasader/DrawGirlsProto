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

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Character" dbWhere='{}' dbLimit="30" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='name' viewer='{"type":"koreanViewer"}' editor='{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}' >name</th>
		<th field='sale' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >sale</th>
		<th field='purchaseInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"type","type":"text"},{"field":"value","type":"text","datatype":"int"}]}' >purchaseInfo</th>
		<th field='statInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"gold","type":"text","datatype":"int"},{"field":"percent","type":"text","datatype":"int"},{"field":"feverTime","type":"text","datatype":"int"},{"field":"speed","type":"text","datatype":"int"},{"field":"life","type":"text","datatype":"int"},{"field":"color","type":"select","element":[0,1,2,3,4,5,6]}]}' >statInfo</th>
		<th field='resourceInfo' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary","element":[{"field":"ccbiID","type":"text"},{"field":"ccbi","type":"resourceSelector"},{"field":"imageID","type":"text"},{"field":"plist","type":"resourceSelector"},{"field":"pvrccz","type":"resourceSelector"},{"field":"size","type":"text","datatype":"int"}]}' >resourceInfo</th>
		<th field='missileInfo' viewer='{"type":"autoViewer"}' editor='{"type":"powerInfoEditor"}' >missileInfo</th>
		<th field='comment' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}' >comment</th>
		<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>