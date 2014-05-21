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
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="PuzzleHistory" dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="puzzleNo" viewer='{"type":"text"}'>퍼즐번호</th>
			<th field="title" viewer='{"type":"text"}'>퍼즐이름</th>
			<th field="openDate" viewer='{"type":"text"}'>최초오픈일시</th>
			<th field="openType" viewer='{"type":"text"}'>오픈유형</th>
			<th field="state" viewer='{"type":"text"}'>진행상태</th>
			<th field="clearDate" viewer='{"type":"text"}'>클리어시각</th>
			<th field="perfectDate" viewer='{"type":"text"}'>퍼펙트클리어시각</th>
			<th field="reward" viewer='{"type":"text"}'>보상</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>