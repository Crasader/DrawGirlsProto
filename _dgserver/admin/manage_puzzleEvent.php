<?php
include "manage_header.php";
?>

<br><br>
<h2 id="tables-contextual-classes">|이벤트퍼즐</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="PuzzleEvent" dbWhere='{}' name="datatable" editRowOnly="true" editType="form" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='우선순위' field='order' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >우선순위</th>
		<th title='타이틀' field='title' viewer='{"type":"text"}' editor='{"type":"text"}' >타이틀</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='국가' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelector"}' >국가</th>
		<th title='시작일시' field='startDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >시작일시</th>
		<th title='종료일시' field='endDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >종료일시</th>
		<th title='시작시간' field='startTime' viewer='{"type":"time","format":"h:i:s"}' editor='{"type":"time"}' >시작시간</th>
		<th title='종료시간' field='endTime' viewer='{"type":"time","format":"h:i:s"}' editor='{"type":"time"}' >종료시간</th>
		<th title='요일' field='weekDay' viewer='{"type":"weekDayViewer"}' editor='{"type":"weekDaySelector"}' >요일</th>
		<th title='설명' field='comment' viewer='{"type":"text"}' editor='{"type":"text"}' >설명</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>