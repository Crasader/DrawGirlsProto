<?php
$menuoff=true;
include "header.php";
?>

<script>

	$(document).ready(function() {
		$("body").on("click",".selectObj", function(e){ //user click on remove text
			var fInfo = gf($(this));
			var result = {"index":fInfo["primaryValue"],"data":fInfo["rowData"]};
			opener.LQSelectedData("<?=$_GET[delkey]?>",result);
	        window.close();
			return false;
		}) 
	});


	var nameViewer = function(value,option){
		return "<input type=button value='"+value+"' class=selectObj>";
	}
	
	var imageViewer = function(value,option){
		return "<img src='"+value+"' width=100>";
	}

</script>
<?php
if($_GET[mode]=="pattern"){
?>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("pattern")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="type" viewer='{"type":"text"}' primary>no</th>
			<th field="category" viewer='{"type":"text"}'>name</th>
			<th field="name" viewer='{"type":"custom","func":"nameViewer"}'>name</th>
			<th field="image" viewer='{"type":"custom","func":"imageViewer"}'>image</th>
			<th field="comment" viewer='{"type":"textarea"}'>comment</th>
		</tr>
	</thead>
	<tbody datazone>
	</tbody>
</table>

<?php }else if($_GET[mode]=="monster"){?>

<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("monster")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}'>no</th>
			<th field="name" viewer='{"type":"custom","func":"nameViewer"}' primary>name</th>
			<th field="image" viewer='{"type":"custom","func":"imageViewer"}'>image</th>
			<th field="comment" viewer='{"type":"textarea"}'>comment</th>
		</tr>
	</thead>
	<tbody datazone>
	</tbody>
</table>

<?php
}
?>

<?php
include "footer.php";
?>