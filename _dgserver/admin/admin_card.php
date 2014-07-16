<?php
include "header.php";
?>


<?
/*
	viewer - [text], time, price, json, image, url, custom(option:func), bool
	editor - [lock],text, number, time, json, image, select, custom:(option:func)
	                                            option: ["select1","select2"]
	
	
	
*/
?>
<script>
var showCardImg = function(value,option){
	data = s2j(value);
	return '<img src='+data["img"]+' width=100>';
}
</script>
<center>
<form action=admin_card.php>
<input name="gid" value="<?=$gid?>" type="hidden">
조건 : <input name='where' size="50" value='<?=$_GET['where']?>'> <input type=submit value="확인">
</form>

</center>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="UserLog" autoSetting="true" dbWhere='' name="datatable" border=1 align=center>
	<thead>
	</thead>
	<tbody datazone>

	</tbody>
</table>

<?php
include "footer.php";
?>