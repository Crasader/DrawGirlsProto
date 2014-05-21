<?php
include "header.php";

?>

<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Shop" dbWhere='' name="datatable" border=1 align=center>
	<thead>
		<tr>
			<th field="no" primary>no</th>
			<th field="category" viewer='{"type":"text"}' editor='{"type":"text"}'>category</th>

			<th field="id" viewer='{"type":"text"}' editor='{"type":"text"}'>id</th>
			<th field="type" viewer='{"type":"text"}' editor='{"type":"select","element":["gold","ruby","heart"],"value":["g","r","h"]}'>type</th>
			<th field="count" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>count</th>
			<th field="countName" viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}'>countName</th>
			<th field="priceType" viewer='{"type":"text"}' editor='{"type":"select","element":["gold","ruby","heart","money"],"value":["g","r","h","m"]}'>priceType</th>
			<th field="price" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>price</th>
			<th field="priceName" viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"type":"text","field":"ko"},{"type":"text","field":"en"}]}'>priceName</th>
			<th field="sale" viewer='{"type":"text"}' editor='{"type":"text"}'>sale</th>
			
			<th field="image" viewer='{"type":"text"}' editor='{"type":"text"}'>image</th>
			<th field="data" viewer='{"type":"json"}' editor='{"type":"dictionary"}'>data</th>
			<th field="pID" viewer='{"type":"text"}' editor='{"type":"text"}'>pID</th>
			<th field="exchangeID" viewer='{"type":"text"}' editor='{"type":"text"}'>exchangeID</th>
			<th field="exchangeList" viewer='{"type":"text"}' editor='{"type":"array","element":{"type":"dictionary","element":[{"type":"text","field":"type"},{"type":"text","field":"count","datatype":"int"},{"type":"text","field":"statsID"},{"type":"text","field":"statsValue","datatype":"int"},{"type":"text","field":"content"}]}}'>exchangeList</th>

			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table> 

<?php
include "footer.php";
?>