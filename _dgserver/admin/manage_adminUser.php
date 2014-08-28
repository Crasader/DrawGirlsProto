<?php
include "manage_header.php";

$permissionList = array_keys(AdminUser::$m__permissinoGroup);

$pListString=array();
$pListName = array();
foreach (AdminUser::$m__permissinoGroup as $key => $value) {
	$pListString[]=$key;
	$pListName[]=AdminUser::$m__permissinoGroup[$key][0];
}
?>
<input name="gid" value="<?=$gid?>" type="hidden">
<script>

var adminPermissionView = function(value,opotion){
	value = s2j(value);
	var perList = <?=json_encode($pListString)?>;
	var nameList = <?=json_encode($pListName)?>;
	
	var table = $("<table>").addClass("table").addClass("table-bordered");
	var titleRow = $("<tr>");
	var valueRow = $("<tr>");
	for(var i=0;i<perList.length;i++){
		var cbox =  viewerFunc_select(value[perList[i]],'{"type":"select","element":["없음","읽기","읽기/쓰기"],"value":["none","read","write"]}');

		$("<td>").html(nameList[i]).appendTo(titleRow).attr("align","center");
		$("<td>").html(cbox).appendTo(valueRow).attr("align","center");
		//var cbox = $("<input>").attr("type","checkbox").attr("name",perList[i]).appendTo(td);
		//if(value[perList[i]]=="true")cbox.attr("checked",true);
	}

	table.append(titleRow).append(valueRow);
	return table;

}


var adminPermissionEditor = function(value,option){
	value = s2j(value);
	var perList = <?=json_encode($pListString)?>;
	var nameList = <?=json_encode($pListName)?>;

	var table = $("<table>").addClass("table").addClass("table-bordered").addClass("LQEditor").attr("editor",j2s(option));
	var titleRow = $("<tr>");
	var valueRow = $("<tr>");
	for(var i=0;i<perList.length;i++){
		$("<td>").html(nameList[i]).appendTo(titleRow).attr("align","center");
		var td = $("<td>").appendTo(valueRow).attr("align","center").attr("name",perList[i]);
		var cbox =  editorFunc_select(value[perList[i]],'{"type":"select","element":["없음","읽기","읽기/쓰기"],"value":["none","read","write"]}').appendTo(td);
	}

	table.append(titleRow).append(valueRow);
	return table;

}

var adminPermissionEditor_value = function(obj){
	var r = {};
	obj.find(".LQEditor").each(function(l){	
		var n = $(this).parent().attr("name");
		r[n]=getObjValue($(this));
	});
	return r;
}

</script>
<br><br>
<h2 id="tables-contextual-classes">|관리자목록</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="AdminUser" dbWhere='{}' dbLimit="30" name="datatable" editRowOnly="true" editType="form" border=1  commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='ID' field='id' viewer='{"type":"text"}' editor='{"type":"text"}' >ID</th>
		<th title='PW' field='passwd' viewer='{"type":"text"}' editor='{"type":"password"}' >PW</th>
		<th title='권한' field='permission' viewer='{"type":"adminPermissionView"}' editor='{"type":"adminPermissionEditor"}' >권한</th>
		<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>