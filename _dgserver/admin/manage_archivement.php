<?php
	include "manage_header.php";
	
?>

<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<h2 id="tables-contextual-classes">|업적관리</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Archivement" dbLimit="20" dbWhere='{}' name="datatable" border=1 commenter='{"type":"custom","func":"commenter"}'>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' editor='{"type":"text"}' >고유번호</th>
		<th title='그룹번호' field='groupNo' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >그룹번호</th>
		<th title='업적ID' field='id' viewer='{"type":"text"}' editor='{"type":"text"}' >업적ID</th>
		<th title='공개여부' field='category' viewer='{"type":"select","element":["공개","숨김"],"value":["d","h"]}' editor='{"type":"select","element":["공개","숨김"],"value":["d","h"]}' >공개여부</th>
		<th title='제목' field='title' viewer='{"type":"languageViewer"}' editor='{"type":"languageEditor"}' >제목</th>
		<th title='내용' field='content' viewer='{"type":"languageViewer"}' editor='{"type":"dictionary"}' >내용</th>
		<th title='목표' field='goal' viewer='{"type":"text"}' editor='{"type":"text"}' >목표</th>
		<th title='교환ID' field='exchangeID' viewer='{"type":"exchangeviewer"}' editor='{"type":"exchangemaker","content":"업적","statsID":"archive","statsValueField":"no"}' >교환ID</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>