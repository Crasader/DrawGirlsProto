<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">

<br><br>
<h2 id="tables-contextual-classes">|실시간 메세지</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="RealtimeMsg" dbWhere='{}' name="datatable" editRowOnly="true" editType="form" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='메세지' field='message' viewer='{"type":"text"}' editor='{"type":"textarea"}' >메세지</th>
		<th title='스토어' field='store' viewer='{"type":"storeViewer"}' editor='{"type":"storeSelector"}' >스토어</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='국가' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelector"}' >국가</th>
		<th field='startDate' editor='{"type":"datetime"}' viewer='{"type":"datetime"}'>시작일시</th>
		<th field='endDate' editor='{"type":"datetime"}' viewer='{"type":"datetime"}'>종료일시</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>