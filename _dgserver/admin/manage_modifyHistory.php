<?php
include "manage_header.php";
?>
<script>

$(document).ready(function(){

	$('body').on('click','#findUserInfo',function(){
		
		var dataview = getDataTable("datatable");
		var id = $("#findNo").val();
		var selectType = $('.active[id=findType]').val();
		dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'"}');
		loadDataTable(dataview);
	});
});

</script>
<input name="gid" value="<?=$gid?>" type="hidden">
<br><br>
<div class="table-responsive">
	<table align=center><tr><td>
<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">
			  <button id="findType" type="button" value="nick" class="btn btn-default" data-toggle="button">닉네임</button>
			  <button id="findType" type="button" value="fb" class="btn btn-default" data-toggle="button">페이스북</button>
			  <button id="findType" type="button" value="pc" class="btn btn-default" data-toggle="button">Payco</button>
			  <button id="findType" type="button" value="sno" class="btn btn-default active" data-toggle="button">회원번호</button>
			</div>
		</td><td>
			<div class="input-group">
	      		<input type="text" class="form-control" id="findNo">
	      		<span class="input-group-btn">
	        		<button class="btn btn-default" type="button" id="findUserInfo">조회</button>
	      		</span>
      		</div>
		</td></tr></table>
</div>
<br><br>
<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|변경내역조회</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="ModifyHistory" autoLoad="false" dbWhere='{}' dbLimit="10" dbSort='{"no":"desc"}' name="datatable" border=1>
	<thead>
		<tr>
			<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
			<th title='회원번호' field='memberID' viewer='{"type":"text"}' >회원번호</th>
			<th title='카테고리' field='category' viewer='{"type":"text"}' >카테고리</th>
			<th title='수정전데이터' field='oldData' viewer='{"type":"autoViewer"}' >수정전데이터</th>
			<th title='수정후데이터' field='newData' viewer='{"type":"autoViewer"}' >수정후데이터</th>
			<th title='수정사유' field='reason' viewer='{"type":"text"}' >수정사유</th>
			<th title='작성자' field='writer' viewer='{"type":"text"}' >작성자</th>
			<th title='수정일시' field='regDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' >수정일시</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>