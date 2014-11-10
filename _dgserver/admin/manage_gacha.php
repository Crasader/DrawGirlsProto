<?php
include "manage_header.php";
?>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Gacha" dbWhere='{}' dbLimit="30" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='exchangeID' viewer='{"type":"exchangeviewer"}' editor='{"type":"exchangemaker","content":"뽑기","statsID":"gacha","statsValueField":"no"}' >교환ID</th>
		<th field='level' viewer='{"type":"text"}' editor='{"type":"select","element":[1,2,3,4]}' >level</th>
		<th field='premiumOnly' viewer='{"type":"select","element":["고급뽑기","일반뽑기"],"value":[1,0]}' editor='{"type":"select","element":["고급뽑기","일반뽑기"],"value":[1,0]}' >고급전용</th>
		<th field='showProb' viewer='{"type":"text"}' editor='{"type":"text"}' >노출확률<br>(0~100)</th>
		<th field='gainProb' viewer='{"type":"text"}' editor='{"type":"text"}' >획득확률<br>(1~100)</th>
		<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

</tbody>
</table>



<?php
include "manage_footer.php";
?>