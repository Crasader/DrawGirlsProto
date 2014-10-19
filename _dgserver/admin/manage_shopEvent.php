<?php
include "manage_header.php";

$listViewer=array("type"=>"select","field"=>"type");
while($pData = Shop::getRowByQuery("",null,"id,priceName,countName,comment")){
	$listViewer["element"][] = $pData["comment"];
	$listViewer["value"][]=$pData["id"];
}

?>


<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<h2 id="tables-contextual-classes">|상점 이벤트</h2>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="ShopEvent" dbWhere='{}' name="datatable" border=1 align=center editRowOnly="true" editType="form" commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}'>고유번호</th>
		<th virtual title='진행상태' field='state' viewer='{"type":"text"}' >진행상태</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='국가' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelector"}' >국가</th>
		<th title='설명' field='comment' viewer='{"type":"text"}' editor='{"type":"textarea"}' >설명</th>
		<th title='할인대상' field='id' viewer='<?=json_encode($listViewer)?>' editor='<?=json_encode($listViewer)?>'>할인대상</th>
		<th title='시작일시' field='startDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >시작일시</th>
		<th title='종료일시' field='endDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >종료일시</th>
		<th title='시작시간' field='startTime' viewer='{"type":"time","format":"h:i:s"}' editor='{"type":"time"}' >시작시간</th>
		<th title='종료시간' field='endTime' viewer='{"type":"time","format":"h:i:s"}' editor='{"type":"time"}' >종료시간</th>
		<th title='표시갯수' field='countName' viewer='{"type":"text"}' editor='{"type":"textarea"}' >표시갯수</th>
		<th title='표시가격' field='priceName' viewer='{"type":"text"}' editor='{"type":"textarea"}' >표시가격</th>
		<th title='할인표시' field='sale' viewer='{"type":"text"}' editor='{"type":"text"}' >할인표시</th>
		<th title='교환ID' field='exchangeID' viewer='{"type":"exchangeviewer"}' editor='{"type":"exchangemaker","content":"상점할인","statsID":"saleshop","statsValueField":"no"}' >교환ID</th>
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