<?php
include "manage_header.php";
?>
<script>
var viewer_image = function(value,option){
	value = s2j(value);
	var neditor = $("<img>").attr("src",value["img"]).attr("width",100);
	return neditor;
}
</script>
<input name="gid" value="<?=$gid?>" type="hidden">

<br><br>
<h2 id="tables-contextual-classes">|게임 공지</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Notice" dbWhere='{}' name="datatable" editRowOnly="true" editType="form" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='우선순위' field='order' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >우선순위</th>
		<th virtual title='진행상태' field='state' viewer='{"type":"text"}' >진행상태</th>
		<th title='제목' field='title' viewer='{"type":"text"}' editor='null' >제목</th>
		<th title='시작일시' field='startDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >시작일시</th>
		<th title='종료일시' field='endDate' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}' editor='{"type":"datetime"}' >종료일시</th>
		<th title='운영체제' field='os' viewer='{"type":"osViewer"}' editor='{"type":"osSelector"}' >운영체제</th>
		<th title='국가' field='cc' viewer='{"type":"countryViewer"}' editor='{"type":"countrySelector"}' >국가</th>
		<th title='팝업표시여부' field='isPopup' viewer='{"type":"bool"}' editor='{"type":"bool"}' >팝업표시여부</th>
		<th title='이미지' field='imgInfo' viewer='{"type":"viewer_image"}' editor='{"type":"dictionary","element":[{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >이미지</th>
		<th title='연결URL' field='linkURL' viewer='{"type":"text"}' editor='null' >연결URL</th>
		<th title='클라이언트내용' field='content' viewer='{"type":"text"}' editor='{"type":"textarea"}' >클라이언트내용</th>
		<th title='목록표시여부' field='isList' viewer='{"type":"bool"}' editor='{"type":"bool"}' >목록표시여부</th>
		<th title='목록표시내용' field='webContent' viewer='{"type":"text"}' editor='{"type":"textarea"}' >목록표시내용</th>
		<th title='목록배너' field='banner' viewer='{"type":"viewer_image"}' editor='{"type":"dictionary","element":[{"field":"img","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >목록배너</th>
		<th title='앱정지여부' field='isBreak' viewer='{"type":"bool"}' editor='{"type":"bool"}' >앱정지여부</th>
		<th title='보조앱용공지' field='isForDiary' viewer='{"type":"bool"}' editor='{"type":"bool"}' >보조앱용공지</th>		
		<th title='최하빌드넘버' field='minBuildNo' viewer='{"type":"text"}' editor='{"type":"text"}' >최하빌드넘버</th>
		<th title='최고빌드넘버' field='maxBuildNo' viewer='{"type":"text"}' editor='{"type":"text"}' >최고빌드넘버</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>