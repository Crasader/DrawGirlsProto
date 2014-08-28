<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="KeyIntValue"  dbWhere='{}' dbLimit="30" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='key' viewer='{"type":"text"}' >key</th>
		<th field='value' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >value</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>