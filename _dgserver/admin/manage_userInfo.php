<?php
include "manage_header.php";
?>
<script>

$(document).ready(function(){
	

	$('body').on('click','#findUserInfo',function(){
		
		var dataview = getDataForm("userinfo");
		var id = $("#findNo").val();
		var selectType = $('.active[id=findType]').val();
		dataview.attr("dbWhere",'{"type":"'+selectType+'","id":"'+id+'"}');
		loadDataForm(dataview);
	});



});


</script>
<input name="gid" value="<?=$gid?>" type="hidden">
<br><br>
<div class="table-responsive">
	<table align=center><tr><td>
<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">
			  <button id="findType" type="button" value="nick" class="btn btn-default" data-toggle="button">닉네임</button>
			  <button id="findType" type="button" value="fb" class="btn btn-default" data-toggle="button">페이스북</button>
			  <button id="findType" type="button" value="pc" class="btn btn-default" data-toggle="button">Payco</button>
			  <button id="findType" type="button" value="sno" class="btn btn-default active" data-toggle="button">회원번호</button>
			</div>
		</td><td>
			<div class="input-group">
	      		<input type="text" class="form-control" id="findNo">
	      		<span class="input-group-btn">
	        		<button class="btn btn-default" type="button" id="findUserInfo">조회</button>
	      		</span>
      		</div>
		</td></tr></table>
</div>
<br><br>
<div class="table-responsive">
<table class="LQDataForm" autoLoad="false" dbSource="dataManager2.php" dbClass="UserData" dbWhere='' name="userinfo" width="1000" align=center commenter='{"type":"custom","func":"commenter"}'>
	<tr><td manage="update insert delete">
	<h2 id="tables-contextual-classes">|회원정보</h2>
	<table class="table table-bordered">
		<thead>
		<tr>
			<th>S-No</th>
			<th>닉네임</th>
			<th>성별</th>
			<th>생년월일</th>
			<th>전화번호</th>
			<th>최근로그인</th>
			<th>회원가입일시</th>
		</tr>
		</thead>
		<tr>
			<td field="memberID" primary></td>
			<td field="nick" editorx='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
			<td field="gender" editorx='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
			<td field="age" editorx='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
			<td field="phoneNo" editorx='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
			<td field="lastDate" editorx='{"type":"datetime"}' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}'></td>
			<td field="joinDate" editorx='{"type":"datetime"}' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}'></td>
		</tr>
	</table>
<br><br>
	<h2 id="tables-contextual-classes">|게임 기록 정보</h2>
	<table class="table table-bordered">	  
		<tr>
			<td>닉네임</td><td field="nick" editorx='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
			<td>이용국가</td><td field="country" editorx='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
			<td></td><td></td>
		</tr>
		<tr>
			<td>케릭터레벨</td><td field="characterLevel" viewer='{"type":"text"}'></td>
			<td>출석상태</td><td field="eventAtdCount" viewer='{"type":"text"}'></td>
			<td></td><td></td>
		</tr>
		<tr>
			<td>게임시작일시</td><td field="joinDate" editorx='{"type":"datetime"}' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}'></td>
			<td>최근접속일시</td><td field="lastDate" editorx='{"type":"datetime"}' viewer='{"type":"datetime","format":"Y/m/d h:i:s"}'></td>
			<td>접속여부</td><td field="isConnecting" viewer='{"type":"text"}'></td>
		</tr>
		<tr>
			<td>보유골드</td><td field="g" viewer='{"type":"text"}'></td>
			<td>보유젬<br>(무료/유료)</td><td field="r" viewer='{"type":"format","format":"[fr] / [pr]"}'></td>
			<td>보유하트</td><td field="h" viewer='{"type":"text"}'></td>
		</tr>
		<tr>
			<td>최고스테이지</td><td field="highPiece" viewer='{"type":"text"}'></td>
			<td>최고기록</td><td field="highScore" viewer='{"type":"text"}'></td>
			<td></td><td></td>
		</tr>
	</table>

	<br><br>
	<h2 id="tables-contextual-classes">|보유 아이템</h2>
	<table class="table table-bordered">
		<tr>
			<td>i1(미사용)</td>
			<td>i2(미사용)</td>
			<td>i3(미사용)</td>
			<td>i4(미사용)</td>
			<td>i5(미사용)</td>
			<td>아이템두배</td>
			<td>i7(미사용)</td>
			<td>i8(미사용)</td>
			<td>신발</td>
			<td>i10(미사용)</td>
			<td>자석</td>
		</tr>
		<tr>
			<td field="i1" viewer='{"type":"text"}'></td>
			<td field="i2" viewer='{"type":"text"}'></td>
			<td field="i3" viewer='{"type":"text"}'></td>
			<td field="i4" viewer='{"type":"text"}'></td>
			<td field="i5" viewer='{"type":"text"}'></td>
			<td field="i6" viewer='{"type":"text"}'></td>
			<td field="i7" viewer='{"type":"text"}'></td>
			<td field="i8" viewer='{"type":"text"}'></td>
			<td field="i9" viewer='{"type":"text"}'></td>
			<td field="i10" viewer='{"type":"text"}'></td>
			<td field="i11" viewer='{"type":"text"}'></td>
		</tr>
		<tr>
			<td>이어하기</td>
			<td>맵가챠</td>
			<td>캐릭업글</td>
			<td>아이템뽑기</td>
			<td>99프로뽑기</td>
			<td>생명의돌</td>
			<td>p7(미사용)</td>
			<td>p8(미사용)</td>
			<td>p9(미사용)</td>
			<td>p10(미사용)</td>
			<td>p11(미사용)</td>
		</tr>
		<tr>
			<td field="p1" viewer='{"type":"text"}'></td>
			<td field="p2" viewer='{"type":"text"}'></td>
			<td field="p3" viewer='{"type":"text"}'></td>
			<td field="p4" viewer='{"type":"text"}'></td>
			<td field="p5" viewer='{"type":"text"}'></td>
			<td field="p6" viewer='{"type":"text"}'></td>
			<td field="p7" viewer='{"type":"text"}'></td>
			<td field="p8" viewer='{"type":"text"}'></td>
			<td field="p9" viewer='{"type":"text"}'></td>
			<td field="p10" viewer='{"type":"text"}'></td>
			<td field="p11" viewer='{"type":"text"}'></td>
		</tr>
	</table>
</td></tr>
</table>
</div>


<?php
include "manage_footer.php";
?>

