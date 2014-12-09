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
	<h2 id="tables-contextual-classes">|PVP플레이데이터</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="EndlessPlayList" autoLoad="false" dbWhere='{}' dbLimit="10" dbSort='{"no":"desc"}' name="datatable" border=1>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='닉네임' field='nick' viewer='{"type":"text"}' >닉네임</th>
		<th title='연승' field='victory' viewer='{"type":"text"}' >연승</th>
		<th title='레벨' field='level' viewer='{"type":"text"}' >레벨</th>
		<th title='오토레벨' field='autoLevel' viewer='{"type":"text"}' >오토레벨</th>
		<th title='점수' field='score' viewer='{"type":"text"}' >점수</th>
		<th title='데이터' field='playData' viewer='{"type":"text","cut":100}' editor='{"type":"textarea"}' >데이터</th>
		<th title='등록일자' field='regDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' >등록일자</th>
		<th title='피스번호' field='pieceNo' viewer='{"type":"text"}' >피스번호</th>
		<th manage='delete update' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>