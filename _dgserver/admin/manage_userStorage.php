<?php
include "manage_header.php";

$today = TimeManager::getCurrentDate();
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
			dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'","date":"'+$("#date").val()+'"}');
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
			dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'","propType":"'+$(this).attr("value")+'","date":"'+$("#date").val()+'"}');
			loadDataTable(dataview);
		}
	});


	$('body').on('click','#findDate',function(){
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
			dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'","propType":"'+$(".active[id=propType]").attr("value")+'","date":"'+$("#date").val()+'"}');
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

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|보유재화</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserStorage" autoLoad="false" dbWhere='{}' dbLimit="10" dbSort='{"no":"desc"}' name="datatable" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='회원번호' field='memberID' viewer='{"type":"text"}' >회원번호</th>
		<th title='유료젬' field='pr' viewer='{"type":"text"}' editor='{"type":"text"}' >유료젬</th>
		<th title='무료젬' field='fr' viewer='{"type":"text"}' editor='{"type":"text"}' >무료젬</th>
		<th title='하트' field='h' viewer='{"type":"text"}' editor='{"type":"text"}' >하트</th>
		<th title='현금' field='m' viewer='{"type":"text"}' editor='{"type":"text"}' >현금</th>
		<th title='골드' field='g' viewer='{"type":"text"}' editor='{"type":"text"}' >골드</th>
		<th field='i1' viewer='{"type":"text"}' editor='{"type":"text"}' >i1</th>
		<th field='i2' viewer='{"type":"text"}' editor='{"type":"text"}' >i2</th>
		<th field='i3' viewer='{"type":"text"}' editor='{"type":"text"}' >i3</th>
		<th field='i4' viewer='{"type":"text"}' editor='{"type":"text"}' >i4</th>
		<th field='i5' viewer='{"type":"text"}' editor='{"type":"text"}' >i5</th>
		<th title='아이템두배' field='i6' viewer='{"type":"text"}' editor='{"type":"text"}' >아이템두배</th>
		<th field='i7' viewer='{"type":"text"}' editor='{"type":"text"}' >i7</th>
		<th field='i8' viewer='{"type":"text"}' editor='{"type":"text"}' >i8</th>
		<th title='신발' field='i9' viewer='{"type":"text"}' editor='{"type":"text"}' >신발</th>
		<th field='i10' viewer='{"type":"text"}' editor='{"type":"text"}' >i10</th>
		<th title='자석' field='i11' viewer='{"type":"text"}' editor='{"type":"text"}' >자석</th>
		<th title='이어하기' field='p1' viewer='{"type":"text"}' editor='{"type":"text"}' >이어하기</th>
		<th title='맵가챠' field='p2' viewer='{"type":"text"}' editor='{"type":"text"}' >맵가챠</th>
		<th title='캐릭업글' field='p3' viewer='{"type":"text"}' editor='{"type":"text"}' >캐릭업글</th>
		<th title='아이템뽑기' field='p4' viewer='{"type":"text"}' editor='{"type":"text"}' >아이템뽑기</th>
		<th title='99프로뽑기' field='p5' viewer='{"type":"text"}' editor='{"type":"text"}' >99프로뽑기</th>
		<th title='생명의돌' field='p6' viewer='{"type":"text"}' editor='{"type":"text"}' >생명의돌</th>
		<th title='일반뽑기권' field='p7' viewer='{"type":"text"}' editor='{"type":"text"}' >일반뽑기권</th>
		<th title='고급뽑기권' field='p8' viewer='{"type":"text"}' editor='{"type":"text"}' >고급뽑기권</th>
		<th manage='update' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|아이템 히스토리</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='itemhistory' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>
	<div>
		<table><tr><td>
		<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">	
			<button id="propType" type="button" value="*" class="btn btn-default active" data-toggle="button">모두보기</button>
			<button id="propType" type="button" value="cd" class="btn btn-default" data-toggle="button">카드</button>
			<button id="propType" type="button" value="pr" class="btn btn-default" data-toggle="button">유료젬</button>
			<button id="propType" type="button" value="fr" class="btn btn-default" data-toggle="button">무료젬</button>
			<button id="propType" type="button" value="h" class="btn btn-default" data-toggle="button">하트</button>
			<button id="propType" type="button" value="g" class="btn btn-default" data-toggle="button">골드</button>
			<button id="propType" type="button" value="i" class="btn btn-default" data-toggle="button">아이템</button>
			<button id="propType" type="button" value="p" class="btn btn-default" data-toggle="button">이용권</button>
		</div>
		</td><td>
		<div class="input-group">
	  		<input type="text" class="form-control" id="date" value="<?=$today?>" placeholder="조회날짜(yyyymmdd)">
	  		<span class="input-group-btn">
	    		<button class="btn btn-default" type="button" id="findDate">조회</button>
	  		</span>
		</div>
	</td></tr></table>
	</div>
</h2>


<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserPropertyHistory" autoLoad="false" dbWhere='{}' dbLimit="10" dbSort='{"no":"desc"}' name="itemhistory" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='종류' field='type' viewer='{"type":"propChange"}' >종류</th>
		<th title='교환ID' field='exchangeID' viewer='{"type":"text"}' >교환ID</th>
		<th title='변화량' field='count' viewer='{"type":"text"}' >변화량</th>
		<th title='총개수' field='total' viewer='{"type":"text"}' >총개수</th>
		<th title='statsID' field='statsID' viewer='{"type":"text"}' >statsID</th>
		<th title='statsValue' field='statsValue' viewer='{"type":"text"}' >statsValue</th>
		<th title='내용' field='content' viewer='{"type":"text"}' >내용</th>
		<th title='획득일시' field='regDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' >획득일시</th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>