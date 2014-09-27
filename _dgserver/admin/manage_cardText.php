<?php
include "manage_header.php";
?>
<script>
$(document).ready(function(){

	$('body').on('click','#find',function(){
		
		var dataview = getDataTable("datatable");
		var id = $("#findNo").val();
		var selectType = $('.active[id=findType]').val();
		dataview.attr("dbWhere",'{"type":"'+selectType+'","cardNo":"'+id+'"}');
		loadDataTable(dataview);
	});
});
</script>

<input name="gid" value="<?=$gid?>" type="hidden">
<br><br>
<div class="table-responsive">
<table align=center><tr><td>
<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">
			  <button id="findType" type="button" value="eventNo" class="btn btn-default active" data-toggle="button">카드번호</button>
			</div>
		</td><td>
			<div class="input-group">
	      		<input type="text" class="form-control" id="findNo">
	      		<span class="input-group-btn">
	        		<button class="btn btn-default" type="button" id="find">조회</button>
	      		</span>
      		</div>
		</td></tr></table>
</div>
<br><br>

<h2 id="tables-contextual-classes">|카드관리</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Card" dbWhere='{}' dbLimit='50' name="datatable" border=1>
	<thead>
		<tr>
		<th primary title='카드번호' field='no' viewer='{"type":"text"}' >카드번호</th>
		<th always title='스테이지' field='piece' viewer='{"type":"text"}' >스테이지</th>
		<th title='이미지' field='imgInfo' viewer='{"type":"imageInfoViewer"}' >이미지</th>
		<th title='이름' field='name' viewer='{"type":"koreanViewer"}' editor='{"type":"languageEditor"}' >이름</th>
		<th title='스크립트' field='script' viewer='{"type":"koreanViewer"}' editor='{"type":"languageEditor"}' >스크립트</th>
		<th title='프로필' field='profile' viewer='{"type":"koreanViewer"}' editor='{"type":"languageEditor","element":{"type":"textarea"}}' >프로필</th>
		<th title='사운드' field='sound' viewer='{"type":"arrayViewer"}' editor='{"type":"array","element":{"type":"text"}}' >사운드</th>
		<th manage='[update]' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>