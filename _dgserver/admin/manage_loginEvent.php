<?php
include "manage_header.php";
?>
<script>
var viewer_image = function(value,option){
	value = s2j(value);
	var neditor = $("<img>").attr("src","../images/"+value["img"]).attr("width",100);
	return neditor;
 }
</script>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|로그인이벤트</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="LoginEvent" dbWhere='{}' name="datatable" editRowOnly="true" editType="form" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th virtual title='진행상태' field='state' viewer='{"type":"text"}' >진행상태</th>
		<th title='이벤트명' field='title' viewer='{"type":"text"}' editor='{"type":"text"}' >이벤트명</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='스토어' field='store' viewer='{"type":"storeViewer"}' editor='{"type":"storeSelector"}' >스토어</th>
		<th title='국가' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelector"}' >국가</th>
		<th title='시작일시' field='startDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >시작일시</th>
		<th title='종료일시' field='endDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >종료일시</th>
		<th title='시작시간' field='startTime' viewer='{"type":"time","format":"h:i:s"}' editor='{"type":"time"}' >시작시간</th>
		<th title='종료시간' field='endTime' viewer='{"type":"time","format":"h:i:s"}' editor='{"type":"time"}' >종료시간</th>
		<th title='반복' field='repeat' viewer='{"type":"bool"}' editor='{"type":"bool"}' >반복</th>
		<th title='교환ID' field='exchangeID' viewer='{"type":"exchangeviewer"}' editor='{"type":"exchangemaker","content":"로그인이벤트","statsID":"loginEvent","statsValueField":"no"}' >교환ID</th>
		<th title='스페셜데이' field='isSpecialDay' viewer='{"type":"bool"}' editor='{"type":"bool"}' >스페셜데이</th>
		<th manage='insert delete update' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>