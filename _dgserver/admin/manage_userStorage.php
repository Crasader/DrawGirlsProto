<?php
include "manage_header.php";
?>
<script>

$(document).ready(function(){

	$('body').on('click','#propType',function(){
		$('button[id=propType]').each(function(index){
			$(this).removeClass("active");
		});
		$(this).addClass("active");
	});
	$('body').on('click','#findUserInfo',function(){
		{
			var dataview = getDataTable("datatable");
			var id = $("#findNo").val();
			var selectType = $('.active[id=findType]').val();
			dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'"}');
			loadDataTable(dataview);
		}
		{
			var dataview = getDataTable("itemhistory");
			var id = $("#findNo").val();
			var selectType = $('.active[id=findType]').val();
			dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'"}');
			loadDataTable(dataview);
		}
	});

	$('body').on('click','#propType',function(){
		{
			var dataview = getDataTable("datatable");
			var id = $("#findNo").val();
			var selectType = $('.active[id=findType]').val();
			dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'"}');
			loadDataTable(dataview);
		}
		{
			var dataview = getDataTable("itemhistory");
			var id = $("#findNo").val();
			var selectType = $('.active[id=findType]').val();
			dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'","propType":"'+$(this).attr("value")+'"}');
			loadDataTable(dataview);
		}
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

<h2 id="tables-contextual-classes">|아이템 정보조회</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserStorage" autoLoad="false" autoSetting="true" dbWhere='{}' dbLimit="10" dbSort='{"no":"desc"}' name="datatable" border=1 commenterx='{"type":"custom","func":"commenter"}'>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<h2 id="tables-contextual-classes">|아이템 히스토리
	<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">	
		<button id="propType" type="button" value="*" class="btn btn-default active" data-toggle="button">모두보기</button>
		<button id="propType" type="button" value="pr" class="btn btn-default" data-toggle="button">유료루비</button>
		<button id="propType" type="button" value="fr" class="btn btn-default" data-toggle="button">무료루비</button>
		<button id="propType" type="button" value="h" class="btn btn-default" data-toggle="button">하트</button>
		<button id="propType" type="button" value="g" class="btn btn-default" data-toggle="button">골드</button>
		<button id="propType" type="button" value="i" class="btn btn-default" data-toggle="button">아이템</button>
		<button id="propType" type="button" value="p" class="btn btn-default" data-toggle="button">이용권</button>
	</div>
</h2>


<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserPropertyHistory" autoLoad="false" autoSetting="true" dbWhere='{}' dbLimit="10" dbSort='{"no":"desc"}' name="itemhistory" border=1 commenterx='{"type":"custom","func":"commenter"}'>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>