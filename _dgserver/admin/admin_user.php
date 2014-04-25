<?php
include "header.php";

if(!$_GET['where'])$_GET['where']="";
if(!$_GET['limit'])$_GET['limit']=100;
if(!$_GET['sort'])$_GET['sort']="no desc";


?>

<center>
<form action=<?=$_SERVER["PHP_SELF"]?>>
   <input name="gid" value="<?=$gid?>" type="hidden">
where : <input name='where' size="50" value='<?=$_GET['where']?>'><br>
sort : <input name='sort' size="50" value='<?=$_GET['sort']?>'><br>
limit : <input name='limit' size="50" value='<?=$_GET['limit']?>'><br>
 <input type=submit value="확인"><br>
 
 현재시각 : <?php 
echo TimeManager::get()->getCurrentDateString();
 ?>
</form>
</center>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::getST("userdata")?>" dbSort="<?=$_GET["sort"]?>" dbLimit='<?=$_GET["limit"]?>' dbWhere='<?=$_GET["where"]?>' border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
	        <th field="nick" viewer='{"type":"text"}'>nick</th>
			<th field="memberID" viewer='{"type":"text"}'>memberID</th>
	        <th field="data" viewer='{"type":"text","cut":100}' editor='{"type":"textarea"}'>data</th>
	        <th field="friendList" viewer='{"type":"text","cut":100}' editor='{"type":"textarea"}'>friendList</th>
	        <th field="lastDate" viewer='{"type":"text"}'>lastDate</th>
	        <th field="lastTime" viewer='{"type":"text"}'>lastTime</th>
			<th field="joinDate" viewer='{"type":"text"}'>joinDate</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>