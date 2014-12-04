<?php
include "manage_header.php";
?>
<script>
var AtdrewardInfo=function(value,option){
	var vvv = s2j(option);
	var neditor=$("<div>").addClass(".LQEditor").attr("align","center");
	$("<button>").addClass("btn btn-primary").append("보상관리").appendTo(neditor).on("click",function(){
		$(location).attr('href',"manage_attendenceEventDay.php?gid="+gid+"&eventNo="+vvv["rowData"]["no"]);
	});
	return neditor;
}
</script>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|출석이벤트</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>

<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="AttendenceEvent" dbWhere='{}' editRowOnly="true" dbLimit="20" editType="form" name="datatable" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th virtual title='진행상태' field='state' viewer='{"type":"text"}' >진행상태</th>
		<th title='우선순위' field='order' viewer='{"type":"text"}' editor='{"type":"text"}' >우선순위</th>
		<th title='제목' field='title' viewer='{"type":"text"}' editor='{"type":"text"}' >제목</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='국가' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelector"}' >국가</th>
		<th title='시작일시' field='startDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >시작일시</th>
		<th title='종료일시' field='endDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >종료일시</th>
		<th virtual title='보상정보' field='rewardInfo' viewer='{"type":"AtdrewardInfo"}' >보상정보</th>
		<th manage='insert delete update' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "manage_footer.php";
?>