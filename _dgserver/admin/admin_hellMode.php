<?php
include "header.php";
?>


<table class="LQDataTable" dbSource="dataManager2.php" dbClass="HellMode" dbWhere='{}' dbLimit="50" dbSort='{}' name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='pieceNo' viewer='{"type":"text"}' editor='{"type":"text"}'>pieceNo</th>
		<th field='cardNo' viewer='{"type":"text"}' editor='{"type":"text"}'>cardNo</th>
		<th field='characterNo' viewer='{"type":"text"}' editor='{"type":"text"}'>characterNo</th>
		<th field='openPieceNo' viewer='{"type":"text"}' editor='{"type":"text"}'>openPieceNo</th>
		<th field='balance' viewer='{"type":"autoViewer"}' editor='{"type":"array"}' >balance</th>
		<th field='cellImgInfo' viewer='{"type":"imageViewer"}' editor='{"type":"imageSelector"}' >cellImgInfo</th>
		<th field='title' viewer='{"type":"koreanViewer"}' editor='{"type":"languageEditor"}' >title</th>
		<th field='content' viewer='{"type":"koreanViewer"}' editor='{"type":"languageEditor"}' >content</th>
		<th manage='[delete,update,insert]' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";

//LQDataTable 초기설정에 updateAllways=true 속성 추가하여 업데이트 요청시 항상 같이 날려주도록 하기. --> version 관리를 위해..
?>

 