<?php
include "manage_header.php";
?>
<script>

$(document).ready(function(){

	$('body').on('click','#findUserInfo',function(){
		
		var dataview = getDataTable("datatable");
		var id = $("#findNo").val();
		var selectType = $('.active[id=findType]').val();
		var findDate = $("#findDate").val();
		dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'","findDate":"'+findDate+'"}');
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
			</td>
			<td>
				<input type="text" class="form-control" id="findNo">
			</td>
			<td>&nbsp;&nbsp;조회일자&nbsp;&nbsp;</td>
			<td>
				<div class="input-group">
		      		
		      		<input type="text" class="form-control" id="findDate" value="<?=TimeManager::getCurrentDate()?>">
		      		<span class="input-group-btn">
		        		<button class="btn btn-default" type="button" id="findUserInfo">조회</button>
		      		</span>
	      		</div>
		</td></tr></table>
</div>
<br><br>
<h2 id="tables-contextual-classes">|유저로그</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserLog" autoLoad="false" dbWhere='{}' dbLimit="30" dbSort='{"no":"desc"}' name="datatable" border=1  commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='회원번호' field='memberID' viewer='{"type":"text"}' >회원번호</th>
		<th title='카테고리' field='category' viewer='{"type":"text"}' >카테고리</th>
		<th title='IP' field='ip' viewer='{"type":"text"}' >IP</th>
		<th title='입력값' field='input' viewer='{"type":"textareaViewer"}' >입력값</th>
		<th title='출력값' field='output' viewer='{"type":"textareaViewer"}' >출력값</th>
		<th title='변경일시' field='regDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' >변경일시</th>
		<th title='타임스탬프' field='regTime' viewer='{"type":"text"}' >타임스탬프</th>
		<th title='실행시간' field='execTime' viewer='{"type":"text"}' >실행시간</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>