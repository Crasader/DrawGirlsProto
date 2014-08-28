<?php
include "manage_header.php";
?>

<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<h2 id="tables-contextual-classes">|퍼즐오픈조건</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Puzzle" dbSort='{"order":"asc"}' dbWhere='{}' name="openCondition" border=1>
	<thead>
		<tr>
		<th primary title='고유번호' field='no' viewer='{"type":"text"}' >고유번호</th>
		<th title='순서' field='order' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >순서</th>
		<th title='공개여부' field='isEvent' viewer='{"type":"select","element":["비공개","일반","이벤트"],"value":[-1,0,1]}' editor='{"type":"select","element":["비공개","일반","이벤트"],"value":[-1,0,1]}' >공개여부</th>
		<th title='타이틀' field='title' viewer='{"type":"languageViewer"}' editor='{"type":"languageEditor"}' >타이틀</th>
		<th title='이미지' field='original' viewer='{"type":"showPuzzleImg"}' editor='{"type":"dictionary","element":[{"field":"image","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >이미지</th>
		<th title='오픈조건' field='condition' viewer='{"type":"puzzleOpenConditionViewer"}' editor='{"type":"array","element":{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"value","datatype":"int"}]}}}' >오픈조건</th>
		<th title='보상카드' field='clearReward' viewer='{"type":"puzzleCardRewradViewer"}' editor='{"type":"dictionary","element":[{"type":"text","field":"normal"},{"type":"text","field":"perfect"}]}' >보상카드</th>
		<th manage='update delete' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "manage_footer.php";
?>