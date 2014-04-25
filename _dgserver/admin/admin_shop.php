<?php
include "header.php";

?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("shop")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" primary>no</th>
			<th field="type" viewer='{"type":"text"}' editor='{"type":"select","element":["gold","ruby","social","coin"]}'>type</th>
			<th field="count" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>count</th>
			<th field="countName" viewer='{"type":"text"}' editor='{"type":"text"}'>countName</th>
			<th field="price" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>price</th>
			<th field="priceType" viewer='{"type":"text"}' editor='{"type":"select","element":["gold","ruby","social","coin","money"]}'>priceType</th>
			<th field="sale" viewer='{"type":"text"}' editor='{"type":"text"}'>sale</th>

			<th field="image" viewer='{"type":"text"}' editor='{"type":"text"}'>image</th>
			<th field="data" viewer='{"type":"json"}' editor='{"type":"dictionary"}'>data</th>
			<th field="pID" viewer='{"type":"text"}' editor='{"type":"text"}'>pID</th>

			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";
?>