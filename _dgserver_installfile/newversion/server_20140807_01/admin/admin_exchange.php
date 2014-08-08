<?php
include "header.php";

?>
<center>

<table border=1>
	<tr><td>
		<b>* type</b> <br>
		r 젬 , g 골드 , h 하트 , m 실제돈 , cd 카드 , pc 피스 , pz 퍼즐 , cu 캐릭업그레이드 , cp 캐릭터 <br>
		패스권 p1,p2,p3,p4,p5 ...<br>
		아이템 i1,i2,i3,i4,i5 ...<br>
	</td></tr>
</table>
<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Exchange" dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="id" viewer='{"type":"text"}' editor='{"type":"text"}'>id</th>
			<th field="list" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}'>list</th>
			<th field="content" viewer='{"type":"text"}' editor='{"type":"text"}'>content</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>
</center>



<?php
include "footer.php";
?>