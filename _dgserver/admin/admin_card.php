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
	return '<img src='+data["img"]+' width=100>';
}
</script>
<center>
<form action=admin_card.php>
<input name="gid" value="<?=$gid?>" type="hidden">
조건 : <input name='where' size="50" value='<?=$_GET['where']?>'> <input type=submit value="확인">
</form>

</center>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Card" dbWhere='{}' dbLimit="200" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' editor='{"type":"text"}' >no</th>
		<th field='serial' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >serial</th>
		<th field='rank' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >rank</th>
		<th field='grade' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >grade</th>
		<th field='name' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}' >name</th>
		<th field='cc' viewer='{"type":"text"}' editor='{"type":"select","element":["kr","jp","en","cn"]}' >cc</th>
		<th always field='piece' viewer='{"type":"text"}'>piece</th>
		<th primary field='no' viewer='{"type":"text"}'>no</th>
		<th field='imgInfo' viewer='{"type":"showCardImg"}' editor='{"type":"dictionary","element":[{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >imgInfo</th>
		<th field='silImgInfo' viewer='{"type":"text","cut":50}' editor='{"type":"dictionary","element":[{"field":"isSil","type":"bool"},{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >silImgInfo</th>
		<th field='faceInfo' viewer='{"type":"text","cut":50}' editor='{"type":"dictionary","element":[{"field":"ccbiID","type":"text"},{"field":"ccbi","type":"resourceSelector","viewer":"text"},{"field":"imageID","type":"text"},{"field":"plist","type":"resourceSelector","viewer":"text"},{"field":"pvrccz","type":"resourceSelector","viewer":"text"},{"field":"size","type":"text","datatype":"int"}]}' >faceInfo</th>
		<th field='haveAdult' viewer='{"type":"text"}' editor='{"type":"bool"}' >haveAdult</th>
		<th field='adultImgInfo' viewer='{"type":"showCardImg"}' editor='{"type":"dictionary","element":[{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >adultImgInfo</th>
		<th field='adultSilImgInfo' viewer='{"type":"text","cut":50}' editor='{"type":"dictionary","element":[{"field":"isSil","type":"bool"},{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >adultSilImgInfo</th>
		<th field='adultFaceInfo' viewer='{"type":"text","cut":50}' editor='{"type":"dictionary","element":[{"field":"ccbiID","type":"text"},{"field":"ccbi","type":"resourceSelector","viewer":"text"},{"field":"imageID","type":"text"},{"field":"plist","type":"resourceSelector","viewer":"text"},{"field":"pvrccz","type":"resourceSelector","viewer":"text"},{"field":"size","type":"text","datatype":"int"}]}' >adultFaceInfo</th>
		<th field='script' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}' >script</th>
		<th field='profile' viewer='{"type":"text"}' editor='{"type":"textarea"}' >profile</th>
		<th field='mPrice' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"field":"r","type":"text","datatype":"int"},{"field":"p6","type":"text","datatype":"int"}]}' >mPrice</th>
		<th field='category' viewer='{"type":"text"}' editor='{"type":"select","element":["normal","nPuzzle","special","event","ePuzzle","gift","leader"]}' >category</th>
		<th field='type' viewer='{"type":"text"}' editor='{"type":"select","element":["음","양","섬"],"value":[1,2,3]}' >type</th>
		<th field='level' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >level</th>
		<th field='characterNo' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >characterNo</th>
		<th field='sound' viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"text"}}' >sound</th>
		<th always field='version' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >version</th>
		<th field='aniInfo' viewer='{"type":"json"}' editor='{"type":"dictionary","element":[{"field":"isAni","type":"bool"},{"field":"detail","type":"dictionary","element":[{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"},{"field":"loopLength","type":"text","datatype":"int"},{"field":"loopSeq","type":"array","element":{"type":"text","datatype":"int"}},{"field":"cutWidth","type":"text","datatype":"int"},{"field":"cutHeight","type":"text","datatype":"int"},{"field":"cutLength","type":"text","datatype":"int"},{"field":"positionX","type":"text","datatype":"int"},{"field":"positionY","type":"text","datatype":"int"}]}]}' >aniInfo</th>
		
		<th manage='[delete,update,insert]' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";
?>