<?php
include "header.php";

if(!$_GET['where'])$_GET['where']="regweek=".TimeManager::getCurrentWeekNoForRank();
if(!$_GET['limit'])$_GET['limit']=100;
if(!$_GET['sort'])$_GET['sort']="no desc";


?>

<center>
<form action=<?=$_SERVER["PHP_SELF"]?>><input name="gid" value="<?=$gid?>" type="hidden">
where : <input name='where' size="50" value='<?=$_GET['where']?>'><br>
sort : <input name='sort' size="50" value='<?=$_GET['sort']?>'><br>
limit : <input name='limit' size="50" value='<?=$_GET['limit']?>'><br>
 <input type=submit value="확인"><br>
 
 현재시각 : <?php 
echo TimeManager::getCurrentDateTime();
 ?>
<br>
 이번주번호 :<?php 
echo TimeManager::getCurrentWeekNo();
 ?>
</form>
</center>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::getST("weeklyscore")?>" dbSort="<?=$_GET["sort"]?>" dbLimit='<?=$_GET["limit"]?>' dbWhere='<?=$_GET["where"]?>' border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
         	<th field="nick" viewer='{"type":"text"}'>nick</th>
			<th field="memberID" viewer='{"type":"text"}'>memberID</th>
			<th field="score" viewer='{"type":"text"}' editor='{"type":"text"}'>score</th>
			<th field="count" viewer='{"type":"text"}'>count</th>
			<th field="regDate" viewer='{"type":"text"}'>regDate</th>
			<th field="regTime" viewer='{"type":"text"}'>regTime</th>
			<th field="regWeek" viewer='{"type":"text"}'>regWeek</th>
         	<th field="data" viewer='{"type":"text","cut":100}' editor='{"type":"textarea"}'>data</th>
         	<th manage="[delete,update]">manage</th>         
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>