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
echo TimeManager::getCurrentDateTime();
 ?>
</form>
</center>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::getST("message")?>" dbSort="<?=$_GET["sort"]?>" dbLimit='<?=$_GET["limit"]?>' dbWhere='<?=$_GET["where"]?>' border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
        	<th field="nick" viewer='{"type":"text"}'>nick</th>
			<th field="memberID" viewer='{"type":"text"}' editor='{"type":"text"}'>memberID</th>
         	<th field="friendNick" viewer='{"type":"text"}'>friendNick</th>
         	<th field="friendID" viewer='{"type":"text"}' editor='{"type":"text"}'>friendID</th>
			<th field="content" viewer='{"type":"text","cut":100}' editor='{"type":"textarea"}'>content</th>
			<th field="type" viewer='{"type":"text"}' editor='{"type":"text"}'>type</th>
        	<th field="isSendMsg" viewer='{"type":"text"}' editor='{"type":"text"}'>isSendMsg</th>
			<th field="regDate" viewer='{"type":"text"}'>regDate</th>
         	<th field="regTime" viewer='{"type":"text"}'>regTime</th>
         	<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>