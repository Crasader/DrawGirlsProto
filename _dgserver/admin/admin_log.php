<?php
include "header.php";

if(!$_GET['where'])$_GET['where']="";
if(!$_GET['limit'])$_GET['limit']=100;
if(!$_GET['sort'])$_GET['sort']="no desc";


/*
$memberlist = '[{"profile":{"memberNo":88899626759591057,"nickname":"최상민","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106182014,"reserved":"{\"pho\":\"http:\/\/th-p32.talk.kakao.co.kr\/th\/talkp\/wkemNKHY1x\/5ZOutnt0W7AB51buR1PzMk\/u16jyf_110x110_c.jpg\",\"thm\":\"http:\/\/th-p32.talk.kakao.co.kr\/th\/talkp\/wkemNKHY1x\/5ZOutnt0W7AB51buR1PzMk\/u16jyf_110x110_c.jpg\"}\n"},"rankingGrade":20,"rankingScore":13200,"rankChanged":268435454},{"profile":{"memberNo":88899626759589916,"nickname":"남희","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131105230740,"reserved":"{\"pho\":\"http:\/\/th-p3.talk.kakao.co.kr\/th\/talkp\/wkavUz2xvS\/QtDh0DkEIP2lDhpXCz9PMK\/qu1lae_110x110_c.jpg\",\"thm\":\"http:\/\/th-p3.talk.kakao.co.kr\/th\/talkp\/wkavUz2xvS\/QtDh0DkEIP2lDhpXCz9PMK\/qu1lae_110x110_c.jpg\"}\n"},"rankingGrade":0,"rankingScore":0,"rankChanged":0},{"profile":{"memberNo":88899626759591050,"nickname":"최재영","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131107042611,"reserved":"{\"pho\":\"http:\/\/th-p24.talk.kakao.co.kr\/th\/talkp\/wkemFe10Ls\/BG8ojObPMEo3RGvSRae8f0\/832stf_110x110_c.jpg\",\"thm\":\"http:\/\/th-p24.talk.kakao.co.kr\/th\/talkp\/wkemFe10Ls\/BG8ojObPMEo3RGvSRae8f0\/832stf_110x110_c.jpg\"}\n"},"rankingGrade":13,"rankingScore":90722,"rankChanged":268435454},{"profile":{"memberNo":88899626759591058,"nickname":"최홍근","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131107010400,"reserved":"{\"pho\":\"\",\"thm\":\"\"}\n"},"rankingGrade":2,"rankingScore":421442,"rankChanged":268435454},{"profile":{"memberNo":88899626759591032,"nickname":"진승호","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106173153,"reserved":"{\"pho\":\"http:\/\/th-p28.talk.kakao.co.kr\/th\/talkp\/wkcL3ocRpB\/lOOXUVCz25ZmogTY0gOkkK\/bhuv10_110x110_c.jpg\",\"thm\":\"http:\/\/th-p28.talk.kakao.co.kr\/th\/talkp\/wkcL3ocRpB\/lOOXUVCz25ZmogTY0gOkkK\/bhuv10_110x110_c.jpg\"}\n"},"rankingGrade":17,"rankingScore":67471,"rankChanged":268435454},{"profile":{"memberNo":88899626759591031,"nickname":"노군~","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106174704,"reserved":"{\"pho\":\"http:\/\/th-p3.talk.kakao.co.kr\/th\/talkp\/wkebW81a0T\/CiL4F8PvgCMcMl9sZPvnd1\/eftyoe_110x110_c.jpg\",\"thm\":\"http:\/\/th-p3.talk.kakao.co.kr\/th\/talkp\/wkebW81a0T\/CiL4F8PvgCMcMl9sZPvnd1\/eftyoe_110x110_c.jpg\"}\n"},"rankingGrade":6,"rankingScore":131328,"rankChanged":268435454},{"profile":{"memberNo":88899626759591047,"nickname":"安閑良","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106201144,"reserved":"{\"pho\":\"http:\/\/th-p11.talk.kakao.co.kr\/th\/talkp\/wkemJVKByX\/xp9MjiRpX940EJBkt8C35k\/3c01aa_110x110_c.jpg\",\"thm\":\"http:\/\/th-p11.talk.kakao.co.kr\/th\/talkp\/wkemJVKByX\/xp9MjiRpX940EJBkt8C35k\/3c01aa_110x110_c.jpg\"}\n"},"rankingGrade":18,"rankingScore":47228,"rankChanged":268435454},{"profile":{"memberNo":88899626759591051,"nickname":"장동효","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106175740,"reserved":"{\"pho\":\"http:\/\/th-p1.talk.kakao.co.kr\/th\/talkp\/wkaQVMstHt\/xJmnmMPefGRi62GdiUHBJ1\/qna8tv_110x110_c.jpg\",\"thm\":\"http:\/\/th-p1.talk.kakao.co.kr\/th\/talkp\/wkaQVMstHt\/xJmnmMPefGRi62GdiUHBJ1\/qna8tv_110x110_c.jpg\"}\n"},"rankingGrade":19,"rankingScore":39810,"rankChanged":268435454},{"profile":{"memberNo":88899626759591036,"nickname":"우승우","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106194027,"reserved":"{\"pho\":\"http:\/\/th-p42.talk.kakao.co.kr\/th\/talkp\/wkd9OiI9xO\/nwr3vQWDeFeVES6vnIBsJ0\/n9vbgz_110x110_c.jpg\",\"thm\":\"http:\/\/th-p42.talk.kakao.co.kr\/th\/talkp\/wkd9OiI9xO\/nwr3vQWDeFeVES6vnIBsJ0\/n9vbgz_110x110_c.jpg\"}\n"},"rankingGrade":0,"rankingScore":0,"rankChanged":0},{"profile":{"memberNo":88899626759589867,"nickname":"쑤니","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131105182616,"reserved":"{\"pho\":\"\",\"thm\":\"\"}\n"},"rankingGrade":0,"rankingScore":0,"rankChanged":0},{"profile":{"memberNo":88899626759591046,"nickname":"허주영","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106204242,"reserved":"{\"pho\":\"http:\/\/th-p41.talk.kakao.co.kr\/th\/talkp\/wkdj4UqYSs\/UMnCZOHhVCiGDTrOF5GE41\/ikhdty_110x110_c.jpg\",\"thm\":\"http:\/\/th-p41.talk.kakao.co.kr\/th\/talkp\/wkdj4UqYSs\/UMnCZOHhVCiGDTrOF5GE41\/ikhdty_110x110_c.jpg\"}\n"},"rankingGrade":7,"rankingScore":124908,"rankChanged":268435454},{"profile":{"memberNo":88899626759591045,"nickname":"박세홍","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106190925,"reserved":"{\"pho\":\"http:\/\/th-p30.talk.kakao.co.kr\/th\/talkp\/wkcozOc3cA\/MSO1ajX8ObJEGVoqurkmA0\/oxifwf_110x110_c.jpg\",\"thm\":\"http:\/\/th-p30.talk.kakao.co.kr\/th\/talkp\/wkcozOc3cA\/MSO1ajX8ObJEGVoqurkmA0\/oxifwf_110x110_c.jpg\"}\n"},"rankingGrade":0,"rankingScore":0,"rankChanged":0},{"profile":{"memberNo":88899626759591033,"nickname":"최준화","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106180209,"reserved":"{\"pho\":\"http:\/\/th-p15.talk.kakao.co.kr\/th\/talkp\/wkd6R06bJa\/eTFleNIH2HTuqE3YMklNm0\/ufe3ed_110x110_c.jpg\",\"thm\":\"http:\/\/th-p15.talk.kakao.co.kr\/th\/talkp\/wkd6R06bJa\/eTFleNIH2HTuqE3YMklNm0\/ufe3ed_110x110_c.jpg\"}\n"},"rankingGrade":0,"rankingScore":0,"rankChanged":0},{"profile":{"memberNo":88899626759591079,"nickname":"박신애","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131107000707,"reserved":"{\"pho\":\"http:\/\/th-p19.talk.kakao.co.kr\/th\/talkp\/wkemMSEwst\/LVWsyvwEXD9dUOAy2dFHAK\/f4txdy_110x110_c.jpg\",\"thm\":\"http:\/\/th-p19.talk.kakao.co.kr\/th\/talkp\/wkemMSEwst\/LVWsyvwEXD9dUOAy2dFHAK\/f4txdy_110x110_c.jpg\"}\n"},"rankingGrade":15,"rankingScore":76252,"rankChanged":268435454},{"profile":{"memberNo":88899626759591044,"nickname":"이현준","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106200703,"reserved":"{\"pho\":\"http:\/\/th-p8.talk.kakao.co.kr\/th\/talkp\/wkeb0XLiuC\/al4DKgmBc1s79hh1FAYhEk\/6f45ct_110x110_c.jpg\",\"thm\":\"http:\/\/th-p8.talk.kakao.co.kr\/th\/talkp\/wkeb0XLiuC\/al4DKgmBc1s79hh1FAYhEk\/6f45ct_110x110_c.jpg\"}\n"},"rankingGrade":12,"rankingScore":100492,"rankChanged":268435454},{"profile":{"memberNo":88899626759589551,"nickname":"조현식","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131030135955,"reserved":"{\"pho\":\"http:\/\/th-p0.talk.kakao.co.kr\/th\/talkp\/wkcdt8LHLM\/0gyUonCm43zvfBmpAC6o60\/y4kv79_110x110_c.jpg\",\"thm\":\"http:\/\/th-p0.talk.kakao.co.kr\/th\/talkp\/wkcdt8LHLM\/0gyUonCm43zvfBmpAC6o60\/y4kv79_110x110_c.jpg\"}\n"},"rankingGrade":0,"rankingScore":0,"rankChanged":0},{"profile":{"memberNo":88899626759591034,"nickname":"유동석","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106181906,"reserved":"{\"pho\":\"http:\/\/th-p6.talk.kakao.co.kr\/th\/talkp\/wkeiMyBCp1\/ibXxGqqCX0mS0emDYFLpx1\/vd7h8b_110x110_c.jpg\",\"thm\":\"http:\/\/th-p6.talk.kakao.co.kr\/th\/talkp\/wkeiMyBCp1\/ibXxGqqCX0mS0emDYFLpx1\/vd7h8b_110x110_c.jpg\"}\n"},"rankingGrade":8,"rankingScore":116297,"rankChanged":268435454},{"profile":{"memberNo":88899626759591049,"nickname":"민진Kang","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106223653,"reserved":"{\"pho\":\"http:\/\/th-p8.talk.kakao.co.kr\/th\/talkp\/wkekYSYPbD\/KkDoPccVKQCWMDvAMjEke0\/smdjnw_110x110_c.jpg\",\"thm\":\"http:\/\/th-p8.talk.kakao.co.kr\/th\/talkp\/wkekYSYPbD\/KkDoPccVKQCWMDvAMjEke0\/smdjnw_110x110_c.jpg\"}\n"},"rankingGrade":10,"rankingScore":114008,"rankChanged":268435454},{"profile":{"memberNo":88899626759591064,"nickname":"김재욱","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106201232,"reserved":"{\"pho\":\"http:\/\/th-p18.talk.kakao.co.kr\/th\/talkp\/wkemOJwbXQ\/5Ax5wowHmYfKq34hkFVRuk\/u0wh76_110x110_c.jpg\",\"thm\":\"http:\/\/th-p18.talk.kakao.co.kr\/th\/talkp\/wkemOJwbXQ\/5Ax5wowHmYfKq34hkFVRuk\/u0wh76_110x110_c.jpg\"}\n"},"rankingGrade":3,"rankingScore":306779,"rankChanged":268435454},{"profile":{"memberNo":88899626759589034,"nickname":"김민수","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106213803,"reserved":"{\"pho\":\"\",\"thm\":\"\"}\n"},"rankingGrade":11,"rankingScore":104424,"rankChanged":268435454},{"profile":{"memberNo":88899626759589914,"nickname":"영호","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131106125652,"reserved":"{\"pho\":\"\",\"thm\":\"\"}\n"},"rankingGrade":14,"rankingScore":90259,"rankChanged":268435454},{"profile":{"memberNo":88899626759589415,"nickname":"com.litqoo.한경수","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131104211029,"reserved":"{\"pho\":\"http:\/\/th-p36.talk.kakao.co.kr\/th\/talkp\/wkdfBY15Mx\/spuGTCihLD15qkTGHPQGi0\/jhgwmy_110x110_c.jpg\",\"thm\":\"http:\/\/th-p36.talk.kakao.co.kr\/th\/talkp\/wkdfBY15Mx\/spuGTCihLD15qkTGHPQGi0\/jhgwmy_110x110_c.jpg\"}\n"},"rankingGrade":0,"rankingScore":0,"rankChanged":0},{"profile":{"memberNo":88899626759591039,"nickname":"이상욱","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131107132715,"reserved":"{\"pho\":\"http:\/\/th-p.talk.kakao.co.kr\/th\/talkp\/wkeiK1RE2J\/w1v3EXKutu58daUHZO0lHk\/6oiu7_110x110_c.jpg\",\"thm\":\"http:\/\/th-p.talk.kakao.co.kr\/th\/talkp\/wkeiK1RE2J\/w1v3EXKutu58daUHZO0lHk\/6oiu7_110x110_c.jpg\"}\n"},"rankingGrade":4,"rankingScore":304150,"rankChanged":268435454},{"profile":{"memberNo":88899626759588789,"nickname":"김현수","validCode":"Y","recentlyPlayedGameNo":10226,"exposeOnline":true,"lastLoginTime":20131107150631,"reserved":"{\"pho\":\"\",\"thm\":\"\"}\n"},"rankingGrade":1,"rankingScore":783639,"rankChanged":268435454}]';


$memberlist = json_decode($memberlist,true);
$newmemberlist = array();
for($i=0;$i<count($memberlist);$i++){
	$memberid = $memberlist[$i]["profile"]["memberNo"];
	$newmemberlist[$memberid]=$memberlist[$i]["profile"]["nickname"];
}

echo json_encode($newmemberlist,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
*/
?>

<center>
<form action=<?=$_SERVER["PHP_SELF"]?>>
	<input name="gid" value="<?=$gid?>" type="hidden">
where : <input name='where' size="50" value='<?=$_GET['where']?>'><br>
sort : <input name='sort' size="50" value='<?=$_GET['sort']?>'><br>
limit : <input name='limit' size="50" value='<?=$_GET['limit']?>'><br>
 <input type=submit value="확인"><br>
 
 현재시각 : <?php 
echo TimeManager::get()->getCurrentDateTime();
 ?>
</form>
</center>
<table class="LQDataTable" dbSource="dataManager.php" dbTable="<?=DBManager::getST("userlog")?>" dbSort="<?=$_GET["sort"]?>" dbLimit='<?=$_GET["limit"]?>' dbWhere='<?=$_GET["where"]?>' border=1 align=center>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="ip" viewer='{"type":"text"}'>ip</th>
         	<th field="nick" viewer='{"type":"text"}'>nick</th>
			<th field="memberID" viewer='{"type":"text"}'>memberID</th>
			<th field="header" viewer='{"type":"text","cut":100}' editor='{"type":"textarea"}'>header</th>
			<th field="category" viewer='{"type":"text"}'>category</th>
			<th field="input" viewer='{"type":"text","cut":100}' editor='{"type":"textarea"}'>input</th>
			<th field="output" viewer='{"type":"text","cut":100}' editor='{"type":"textarea"}'>output</th>
        	<th field="execTime" viewer='{"type":"text"}'>execTime</th>
			<th field="regDate" viewer='{"type":"text"}'>regDate</th>
			<th field="regTime" viewer='{"type":"text"}'>regTime</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>
</table>



<?php
include "footer.php";
?>