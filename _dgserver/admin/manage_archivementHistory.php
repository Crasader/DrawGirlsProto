<?php
include "manage_header.php";

?>
<script>

$(document).ready(function(){
	$('body').on('click','.findUserInfo',function(){
		
		var datatable = getDataTable("datatable");
		var idType = $("input[name=type]").val();
		var id = $("input[name=findNo]").val();
		 
		datatable.attr("dbWhere",'{"type":"'+idType+'","id":"'+id+'"}');
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
		<td><input type="submit" value="조회" class="findUserInfo"></td>
	</tr>
</table>

<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="ArchivementHistory" dbFunc='{"select":"getHistory"}' dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="archiveID" viewer='{"type":"text"}'>archiveID</th>
			<th field="clearDate" viewer='{"type":"text"}'>clearDate</th>
			<th field="rewardDate" viewer='{"type":"text"}'>rewardDate</th>
			<th field="count" viewer='{"type":"text"}'>count</th>
			<th field="goal" viewer='{"type":"text"}'>goal</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>