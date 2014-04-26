<?php
include "manage_header.php";

?>
<script>

$(document).ready(function(){
	$('body').on('click','.findUser',function(){
		
		var datatable = getDataTable("datatable");
		var idType = $("input[name=type]").val();
		var id = $("input[name=findNo]").val();
		var startDate = $("input[name=startDate]").val();
		var endDate = $("input[name=startDate]").val();
		datatable.attr("dbWhere",'{"type":"'+idType+'","id":"'+id+'","startDate":"'+startDate+'","endDate":"'+endDate+'"}');
		loadDataTable(datatable);
	});
});

</script>
<center>
<input name="gid" value="<?=$gid?>" type="hidden">
<table border=1>
	<tr>
		<td>조회대상</td>
		<td><input type="radio" name="type" value="fb">페이스북ID <input type="radio" name="type" value="pc">Payco ID <input type="radio" name="type" value="sno" checked>회원번호 </td>
		<td><input type="text" name="findNo"><td>
		<td rowspan=2><input type="submit" value="조회" class="findUser"></td>
	</tr>
	<tr>
		<td>기간</td>
		<td colspan=2><input type="text" name="startDate" value="<?=TimeManager::get()->getDateString(TimeManager::get()->getTime()-60*60*24*30)?>"> ~ <input type="text" name="endDate" value="<?=TimeManager::get()->getCurrentDateString()?>"></td>
	</tr>
</table>

<br><br>


<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserPropertyHistory" dbFunc='{"select":"getHistory"}' dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="memberID" viewer='{"type":"text"}'>memberID</th>
			<th field="type" viewer='{"type":"text"}'>type</th>
			<th field="count" viewer='{"type":"text"}'>count</th>
			<th field="total" viewer='{"type":"text"}'>total</th>
			<th field="statsID" viewer='{"type":"text"}'>statsID</th>
			<th field="statsValue" viewer='{"type":"text"}'>statsValue</th>
			<th field="content" viewer='{"type":"text"}'>content</th>
			<th field="sender" viewer='{"type":"text"}'>sender</th>
			<th field="regDate" viewer='{"type":"text"}'>regDate</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>