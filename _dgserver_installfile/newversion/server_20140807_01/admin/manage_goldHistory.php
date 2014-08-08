<?php
include "manage_header.php";

?>
<script>

$(document).ready(function(){
	$('body').on('click','.findUser',function(){
		
		var datatable = getDataTable("datatable");
		var idType = $("input[name=type]").val();
		var id = $("#findNo").val();
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
		<td><input type="text" id="findNo"><td>
		<td rowspan=2><input type="submit" value="조회" class="findUser"></td>
	</tr>
	<tr>
		<td>기간</td>
		<td colspan=2><input type="text" name="startDate" value="<?=TimeManager::getDateTime(TimeManager::getTime()-60*60*24*30)?>"> ~ <input type="text" name="endDate" value="<?=TimeManager::getCurrentDateTime()?>"></td>
	</tr>
</table>

<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="GoldHistory" dbFunc='{"select":"getHistory"}' dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="memberID" viewer='{"type":"text"}'>변경일시</th>
			<th field="content" viewer='{"type":"text"}'>변경내용</th>
			<th field="changeCount" viewer='{"type":"text"}'>변경량</th>
			<th field="count" viewer='{"type":"text"}'>최종보유량</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>