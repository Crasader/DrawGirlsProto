<?php
include "manage_header.php";
?>

<br><br>
<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|타임이벤트</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="TimeEvent" dbWhere='{}' name="datatable" editRowOnly="true" editType="form" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th virtual title='진행상태' field='state' viewer='{"type":"text"}' >진행상태</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='국가' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelector"}' >국가</th>
		<th title='시작일시' field='startDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >시작일시</th>
		<th title='종료일시' field='endDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >종료일시</th>
		<th title='시작시간' field='startTime' viewer='{"type":"time","format":"h:i:s"}' editor='{"type":"time"}' >시작시간</th>
		<th title='종료시간' field='endTime' viewer='{"type":"time","format":"h:i:s"}' editor='{"type":"time"}' >종료시간</th>
		<th title='이벤트타입' field='type' viewer='{"type":"text"}' editor='{"type":"select","element":["gold","heart","card","clear"]}' >이벤트타입</th>
		<th title='이벤트값' field='value' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >이벤트값</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>