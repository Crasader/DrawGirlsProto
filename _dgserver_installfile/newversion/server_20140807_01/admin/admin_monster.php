<?php
include "header.php";
?>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Monster" autoSetting="true" dbWhere='' name="datatable" border=1 align=center>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";

//LQDataTable 초기설정에 updateAllways=true 속성 추가하여 업데이트 요청시 항상 같이 날려주도록 하기. --> version 관리를 위해..
?>

