<?php
include "manage_header.php";
?>
<script>

$(document).ready(function(){

	$('body').on('click','#storeID',function(){
		$('button[id=storeID]').each(function(index){
			$(this).removeClass("active");
		});
		$(this).addClass("active");
		
		var dataview = getDataTable("datatable");
		var selectType = $('.active[id=storeID]').val();
		dataview.attr("dbWhere",'{"storeID":"'+selectType+'"}');
		loadDataTable(dataview);
	});
});
</script>

<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|상점</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">	
	<button id="storeID" type="button" value="*" class="btn btn-default active" data-toggle="button">모두보기</button>
	<button id="storeID" type="button" value="google" class="btn btn-default" data-toggle="button">Google</button>
	<button id="storeID" type="button" value="apple" class="btn btn-default" data-toggle="button">Apple</button>
	<button id="storeID" type="button" value="tstore" class="btn btn-default" data-toggle="button">Tstore</button>
	<button id="storeID" type="button" value="nstore" class="btn btn-default" data-toggle="button">Nstore</button>
</div>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Shop" dbWhere='{}' name="datatable" border=1 align=center dbSort='{"no":"asc"}' dbLimit="30" editRowOnly="true" editType="form"  commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}'>고유번호</th>
		<th title='국가코드' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelectorOnce"}' >국가코드</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='스토어' field='store' viewer='{"type":"storeViewer"}' editor='{"type":"storeSelector"}' >스토어</th>
		<th title='설명' field='comment' viewer='{"type":"text"}' editor='{"type":"textarea"}' >설명</th>
		<th title='상품ID' field='id' viewer='{"type":"text"}' editor='{"type":"text"}' >상품ID</th>
		<th title='표시갯수' field='countName' viewer='{"type":"text"}' editor='{"type":"textarea"}' >표시갯수</th>
		<th title='표시가격' field='priceName' viewer='{"type":"text"}' editor='{"type":"textarea"}' >표시가격</th>
		<th title='할인표시' field='sale' viewer='{"type":"text"}' editor='{"type":"text"}' >할인표시</th>
		<th title='교환ID' field='exchangeID' viewer='{"type":"exchangeviewer"}' editor='{"type":"exchangemaker","content":"상점","statsID":"shop","statsValueField":"no"}' >교환ID</th>
		<th title='스토어결제ID' field='pID' viewer='{"type":"text"}' editor='{"type":"text"}' >스토어결제ID</th>
		<th title='데이터' field='data' viewer='{"type":"autoViewer"}' editor='{"type":"dictionary"}' >데이터</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table> 

<?php
include "manage_footer.php";
?>

