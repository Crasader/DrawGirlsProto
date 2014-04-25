<?php
include "manage_header.php";
?>
<script>

$(document).ready(function(){
	$('body').on('click','.findUser',function(){
		
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
		<td><input type="submit" value="조회" class="findUser"></td>
	</tr>
</table>

<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="PieceHistory" dbFunc='{"select":"getHistory"}' dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="pieceNo" viewer='{"type":"text"}'>피스번호</th>
			<th field="openDate" viewer='{"type":"text"}'>최초오픈일시</th>
			<th field="firstClearDate" viewer='{"type":"text"}'>최초클리어일시</th>
			<th field="tryCount" viewer='{"type":"text"}'>시도횟수</th>
			<th field="state" viewer='{"type":"text"}'>진행상태</th>
			<th field="reward" viewer='{"type":"text"}'>보상</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>