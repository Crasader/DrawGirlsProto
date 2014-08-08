<?php
include "header.php";
?>
<script>

</script>
<table align=center>
	<tr><td>
		-condition type<br>
		퍼즐 p(value),별 s(value),젬 r(value,exchangeID),요일 w(weekday,s,e),일 d(s,e)

	</td></tr></table><br><br>
	
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Puzzle" autoSetting="true" dbWhere='' name="datatable" dbSort='{"order":"asc"}' dbLimit="100" border=1 align=center>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>