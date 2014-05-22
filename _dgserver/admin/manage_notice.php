<?php
include "manage_header.php";
?>

<center>
<input name="gid" value="<?=$gid?>" type="hidden">


<br><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="Notice" dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="title" viewer='{"type":"text"}' editor='{"type":"text"}'>title</th>
			<th field="startDate" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>startDate</th>
			<th field="endDate" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>endDate</th>
			<th field="os" viewer='{"type":"text"}' editor='{"type":"select","element":["all","android","ios"]}'>os</th>
			<th field="language" viewer='{"type":"text"}' editor='{"type":"text"}'>language</th>	
			<th field="type" viewer='{"type":"text"}' editor='{"type":"select","element":["image","text","긴급"]}'>type</th>
			<th field="imgInfo" viewer='{"type":"text"}' editor='{"type":"dictionary","element":[{"field":"img","type":"custom","func":"imageSelector"},{"field":"size","type":"text","datatype":"int"}]}'>imgInfo</th>
			<th field="linkURL" viewer='{"type":"text"}' editor='{"type":"text"}'>linkURL</th>
			<th field="content" viewer='{"type":"text"}' editor='{"type":"textarea"}'>content</th>
			<th field="order" viewer='{"type":"text"}' editor='{"type":"text","datatype":"int"}'>order</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "manage_footer.php";
?>