<?php
include "manage_header.php";
?>
<script>

$(document).ready(function(){
	$('body').on('click','.findUserInfo',function(){
		
		var dataview = getDataView("userinfo");
		var idType = $("input[name=type]").val();
		var id = $("input[name=findNo]").val();
		 
		dataview.attr("dbWhere",'{"type":"'+idType+'","id":"'+id+'"}');
		loadDataView(dataview);
	});
});

</script>
<center>
<input name="gid" value="<?=$gid?>" type="hidden">
<table border=1>
	<tr>
		<td>조회대상</td>
		<td><input type="radio" name="type" value="fb">페이스북ID <input type="radio" name="type" value="pc">Payco ID <input type="radio" name="type" value="sno" checked>회원번호 </td>
		<td><input type="text" name="findNo"><td>
		<td><input type="submit" value="조회" class="findUserInfo"></td>
	</tr>
</table>
<br><br>
<table class="LQDataView" dbSource="dataManager2.php" dbClass="userInfo" dbFunc='{"select":"getUserInfo","write":"wrtieUserInfo","update":"updateUserInfo"}' dbWhere='' name="userinfo" border=1>
	<tr>
		<td>S-No</td>
		<td>닉네임</td>
		<td>성별</td>
		<td>생년월일</td>
		<td>전화번호</td>
		<td>최근로그인</td>
		<td>회원가입일시</td>
		<td>구매한적있는가</td>
		<td>구매유도플로우자료</td>
		<td>연속실패</td>
		<td>오토벨런싱레벨</td>
		<td>총플레이횟수</td>
	</tr>
	<tr>
		<td field="no" primary></td>
		<td field="nick" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="sex" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="birth" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="phone" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="lastDate" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="joinDate" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="isVIP" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="isFirstBuy" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="failCount" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="autoLevel" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
		<td field="totalPlayCount" editor='{"type":"text","datatype":"int"}' viewer='{"type":"text"}'></td>
	</tr>
</table>



<?php
include "manage_footer.php";
?>