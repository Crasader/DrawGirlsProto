<?php
include "header.php";
?>
<br><br>
<center>
<table><tr><td>
간단사용법<br>
 1.웹어드민에 id를 등록하고 대충 x,y,scale값을 일단 대충 적습니다.(anchorX,ancorY값도 입력가능)<br>
 2.조절할 오브젝트를 추가해줍니다.<br>
	FormSetter::get()->addObject("id",object); //웹어드민에 등록한 id , 조절할 오브젝트(CCNode*)<br>
 3.오브젝트가 제거될땐(씬전환시) FormSetter::get()->removeObject("id"); 로 제거해줍니다.<br>
 4.이제 실행해서 해당페이지 펼쳐놓고 웹어드민 설정하면서 최적화된 값을 찾습니다.<br>
 5.값을 찾았으면 2번의 소스를 지우고 setPosition이용해 코딩합니다.<br>
 <br>
 FormSetter.h 속에 더 다양한 기능 사용법이~~
</td></tr></table></center>
<br><br>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::get()->getMT("formsetter")?>" border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="name" viewer='{"type":"text"}' editor='{"type":"text"}'>name</th>
			<th field="comment" viewer='{"type":"text"}' editor='{"type":"textarea"}'>comment</th>
			<th field="data" viewer='{"type":"json"}' editor='{"type":"dictionary"}'>data <br> <font color=red>[예약옵션 x,y,scale,anchorX,anchorY]</font></th>
			<th manage="delete update insert">manage</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>


<?php
include "footer.php";
?>