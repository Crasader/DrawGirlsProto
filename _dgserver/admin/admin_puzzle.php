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
	
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Puzzle" dbWhere='' name="datatable" dbSort='{"order":"asc"}' dbLimit="100" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='order' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >order</th>
		<th field='isEvent' viewer='{"type":"select","element":["비공개","일반","이벤트"],"value":[-1,0,1]}' editor='{"type":"select","element":["비공개","일반","이벤트"],"value":[-1,0,1]}' >isEvent</th>
		<th field='title' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}' >title</th>
		<th field='thumbnail' viewer='{"type":"showPuzzleImg"}' editor='{"type":"dictionary","element":[{"field":"image","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >thumbnail</th>
		<th field='center' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"field":"image","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >center</th>
		<th field='original' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"field":"image","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >original</th>
		<th field='face' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"field":"image","type":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}' >face</th>
		<th field='color' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"field":"r","type":"text","datatype":"int"},{"field":"g","type":"text","datatype":"int"},{"field":"b","type":"text","datatype":"int"}]}' >color</th>
		<th field='conditionInfo' title='conditionInfo (자동갱신)' viewer='{"type":"text"}' >conditionInfo (자동갱신)</th>
		<th field='pathInfo' title='pathInfo' viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"text","datatype":"int"}}' >pathInfo</th>
		<th field='cardInfo' title='cardInfo' viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"array","element":{"type":"text","datatype":"int"}}}' >cardInfo</th>
		<th field='condition' viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"value","datatype":"int"}]}}}' >condition</th>
		<th field='clearReward' viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"type":"text","field":"normal"},{"type":"text","field":"perfect"}]}' >clearReward</th>
		<th always field='version' viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}' >version</th>
		<th manage='update delete insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>