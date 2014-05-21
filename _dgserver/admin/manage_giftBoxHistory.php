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
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="GiftBoxHistory" dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="memberID" viewer='{"type":"text"}' editor='{"type":"text"}'>memberID</th>
			<th field="regDate" viewer='{"type":"text"}' editor='{"type":"text"}'>regDate</th>
			<th field="confirmDate" viewer='{"type":"text"}' editor='{"type":"text"}'>confirmDate</th>
			<th field="sender" viewer='{"type":"text"}' editor='{"type":"text"}'>sender</th>
			<th field="content" viewer='{"type":"text"}' editor='{"type":"text"}'>content</th>
			<th field="reward" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"}]}}'>reward</th>
			<th field="data" viewer='{"type":"text"}' editor='{"type":"textarea"}'>data</th>
			<th field="exchangeID" viewer='{"type":"text"}' editor='{"type":"textarea"}'>exchangeID</th>
			<th field="exchangeList" viewer='{"type":"text"}'>exchangeList</th>
			<th manage="insert delete update">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>