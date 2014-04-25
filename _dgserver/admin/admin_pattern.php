
<?php
include "header.php";
?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="aPatternTable" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="category" viewer='{"type":"text"}' editor='{"type":"text"}'>category</th>
			<th field="type" viewer='{"type":"text"}' editor='{"type":"text"}'>type</th>
			<th field="name" viewer='{"type":"text"}' editor='{"type":"text"}'>name</th>
			<th field="image" viewer='{"type":"custom","func":"imageViewer"}' editor='{"type":"custom","func":"imageSelector"}'>image</th>
			<th field="template" viewer='{"type":"text"}' editor='{"type":"dictionary"}'>template</th>
			<th field="comment" viewer='{"type":"text"}' editor='{"type":"textarea"}'>comment</th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";
?>