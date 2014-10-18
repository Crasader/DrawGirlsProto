<?php
include_once("../lib.php");

$admin = new AdminUser($_SESSION["admin_no"]);
if(!$admin->isLogined()){
	?>

	<script>
	location.href = 'index.php';
	</script>

	<?php
	exit;
}

$left_menu = array();
$left_menu[]=array("title"=>"","url"=>"");

?>
<!DOCTYPE html>
<html>
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<title>
			<?php
				if($title)echo $title;
				else {
					$php_self = explode("/", $_SERVER[PHP_SELF]);
					$url_cnt = count($php_self) - 1;
					$this_page = $php_self[$url_cnt];
					$this_page_name = explode(".",$this_page);
					$this_page_name = $this_page_name[count($this_page_name)-2];
					$lname = explode("_",$this_page_name);
					echo $lname[1];
				}
			?>
		</title>
		<script>
			var gid = '<?=$gid?>';
		</script>
		<script src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
		<script src="http://code.jquery.com/ui/1.10.4/jquery-ui.min.js"></script>
		<link rel="stylesheet" href="http://code.jquery.com/ui/1.10.4/themes/black-tie/jquery-ui.css">
		<script src="jquery-ui-timepicker-addon.js"></script>
		<script src="jquery.form.js"></script>
		<script src="lib.js" charset="UTF-8"></script>
		<script src="LQDataTable.js" charset="UTF-8"></script>
		<script src="LQEditor.js" charset="UTF-8"></script>
		<link rel="stylesheet" href="LQDataTable.css">
		<!-- Latest compiled and minified CSS -->

		<!-- Bootstrap styles -->
		<link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css">
		<!-- Generic page styles -->
		<link rel="stylesheet" href="jqueryupload/css/style.css">
		<!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
		<link rel="stylesheet" href="jqueryupload/css/jquery.fileupload.css">

	</head>
<body>

	<?php
		if(!$gid){
			echo "게임아이디를 찾을수없습니다. <a href=index.php>select gid</a>";
			exit;
		}

	?>

	<nav class="navbar navbar-default navbar-fixed-top" role="navigation">
		 <!-- Brand and toggle get grouped for better mobile display -->
	    <div class="navbar-header">
	      <a class="navbar-brand" href="#">GraphDog</a>
	    </div>

	    <!-- Collect the nav links, forms, and other content for toggling -->
	    <div class="collapse navbar-collapse">
	      <ul class="nav navbar-nav navbar-right">
	        <li><a href="#"><?=$gid?></a></li>
	        </li>
	      </ul>
	    </div><!-- /.navbar-collapse -->
	</nav>
	<div class="container-fluid">
		<div class="row">
			<div class="col-lg-2"> <!--navbar-fixed-left-->
				<div class="bs-sidebar hidden-print" role="complementary">
					<center>
							
		        	<br>
						Hello <?=$admin->id?>
		            	<!--[<a href=manage_userInfo.php?gid=<?=$gid?>>운영툴</a>] [<a href=admin_help.php?gid=<?=$gid?>>관리툴</a>]-->
		        	</center>
		            <ul class="nav bs-sidenav">
		            	<!-- li><a href="#glyphicons">Glyphicons</a>
							<ul class="nav">
								<li><a href="#glyphicons-glyphs">사용가능한 기호</a></li>
								<li><a href="#glyphicons-how-to-use">사용법</a></li>
								<li><a href="#glyphicons-examples">예제</a></li>
							</ul>
		            	</li -->
						<?php
							if($admin->checkPermission("user","read")){
						?>
		            	<li>유저관리
							<ul class="nav">
								<li><a href=manage_userInfo.php?gid=<?=$gid?>>기본정보조회</a></li>
								<li><a href=manage_modifyHistory.php?gid=<?=$gid?>>변경내역조회</a></li>
								<li><a href=manage_puzzleHistory.php?gid=<?=$gid?>>퍼즐정보</a></li>
								<li><a href=manage_pieceHistory.php?gid=<?=$gid?>>보유피스</a></li>
								<li><a href=manage_characterHistory.php?gid=<?=$gid?>>보유캐릭터</a></li>
								<li><a href=manage_cardHistory.php?gid=<?=$gid?>>보유카드</a></li>
								<li><a href=manage_archivementHistory.php?gid=<?=$gid?>>업적진행상황</a></li>
								<li><a href=manage_giftBoxHistory.php?gid=<?=$gid?>>우편함</a></li>
								<li><a href=manage_userStorage.php?gid=<?=$gid?>>재화보관함</a></li>
								<li><a href=manage_cuponHistory.php?gid=<?=$gid?>>쿠폰사용내역</a></li>
								<li><a href=manage_userLog.php?gid=<?=$gid?>>유저로그</a></li>
								<li><a href=manage_userData.php?gid=<?=$gid?>>유저목록</a></li>
							</ul>
		            	</li>
						<?php
							}

							if($admin->checkPermission("rank","read")){
						?>
		            	<li>랭킹
							<ul class="nav">
								<li><a href=manage_stageRank.php?gid=<?=$gid?>>스테이지</a></li>
								<li><a href=manage_pvpRank.php?gid=<?=$gid?>>PVP</a></li>
								<li><a href=manage_weeklyRank.php?gid=<?=$gid?>>주간누적랭킹</a></li>
								<li><a href=manage_inappRank.php?gid=<?=$gid?>>결제랭킹</a></li>
								<li><a href=manage_goldRank.php?gid=<?=$gid?>>골드랭킹</a></li>
								<li><a href=manage_freeRubyRank.php?gid=<?=$gid?>>무료루비랭킹</a></li>
								<li><a href=manage_purchaseRubyRank.php?gid=<?=$gid?>>유료루비랭킹</a></li>
								<li><a href=manage_introduceRank.php?gid=<?=$gid?>>추천인랭킹</a></li>
							</ul>
		            	</li>
						
						<?php
							}

							if($admin->checkPermission("stats","read")){
						?>
		            	<li>통계
							<ul class="nav">
								<li><a href=manage_dbcheck.php?gid=<?=$gid?>>DB서버상태</a></li>
								<li><a href=manage_userStats.php?gid=<?=$gid?>>NRU/DAU</a></li>
								<li><a href=manage_goldStats.php?gid=<?=$gid?>>스테이지별 획득 골드</a></li>
								<li><a href=manage_itemPurchasStats.php?gid=<?=$gid?>>아이템 사용 통계</a></li>
								<li><a href=manage_morphingStats.php?gid=<?=$gid?>>카드별 생명의돌 사용량1</a></li>
								<li><a href=manage_morphingStats2.php?gid=<?=$gid?>>카드별 생명의돌 사용량2</a></li>
								<li><a href=manage_archiveStats.php?gid=<?=$gid?>>업적별 달성수</a></li>
								<li><a href=manage_gababoResultStats.php?gid=<?=$gid?>>가위바위보 게임결과</a></li>
								<li><a href=manage_todayMissionRewardStats.php?gid=<?=$gid?>>오늘의미션 보상 통계</a></li>
								<li><a href=manage_goldTakeUseStats.php?gid=<?=$gid?>>골드 사용/획득 출처</a></li>
								<li><a href=manage_continueRateStats.php?gid=<?=$gid?>>이어하기 종류</a></li>
								<li><a href=manage_stageClearStats.php?gid=<?=$gid?>>스테이지별 클리어 통계</a></li>
								<li><a href=manage_stageRetryStats.php?gid=<?=$gid?>>스테이지별 재시도 평균</a></li>
								<li><a href=manage_charLevelStats.php?gid=<?=$gid?>>레벨 통계</a></li>


							</ul>
		            	</li>


						<?php
							}

							if($admin->checkPermission("event","read")){
						?>
		            	<li>이벤트
							<ul class="nav">
								<li><a href=manage_sendItem.php?gid=<?=$gid?>>재화/아이템 선물하기</a></li>
								<li><a href=manage_notice.php?gid=<?=$gid?>>공지사항</a></li>
								<li><a href=manage_realTimeMsg.php?gid=<?=$gid?>>실시간메세지</a></li>
								<li><a href=manage_loginEvent.php?gid=<?=$gid?>>로그인이벤트</a></li>
								<li><a href=manage_puzzleEvent.php?gid=<?=$gid?>>퍼즐이벤트</a></li>
								<li><a href=manage_shopEvent.php?gid=<?=$gid?>>할인이벤트</a></li>
								<li><a href=manage_timeEvent.php?gid=<?=$gid?>>타임이벤트</a></li>
								<li><a href=manage_missionEvent.php?gid=<?=$gid?>>미션이벤트</a></li>
								<li><a href=manage_attendenceEvent.php?gid=<?=$gid?>>출석이벤트</a></li>
								<li><a href=manage_attendenceEventDay.php?gid=<?=$gid?>>출석이벤트보상</a></li>
								<li><a href=manage_cuponManager.php?gid=<?=$gid?>>쿠폰관리</a></li>
								<li><a href=manage_cuponCode.php?gid=<?=$gid?>>쿠폰코드</a></li>
							</ul>
						</li>
						<?php
							}

							if($admin->checkPermission("game","read")){
						?>
		            	<li>게임관리
							<ul class="nav">
								<li><a href=manage_commonSetting.php?gid=<?=$gid?>>기본설정</a></li>
								<li><a href=manage_exchange.php?gid=<?=$gid?>>교환ID(시스템)</a></li>
								<li><a href=manage_exchangeForEvent.php?gid=<?=$gid?>>교환ID(이벤트)</a></li>
								<li><a href=manage_archivement.php?gid=<?=$gid?>>업적</a></li>
								<li><a href=manage_cardText.php?gid=<?=$gid?>>카드 스크립트/프로필</a></li>
								<li><a href=manage_shop.php?gid=<?=$gid?>>상점</a></li>
								<li><a href=manage_character.php?gid=<?=$gid?>>캐릭터</a></li>
								<li><a href=manage_puzzleOpen.php?gid=<?=$gid?>>퍼즐오픈조건</a></li>
								<li><a href=manage_endlessPlayList.php?gid=<?=$gid?>>PVP플레이데이터</a></li>
								<li><a href=manage_pattern.php?gid=<?=$gid?>>패턴</a></li>
								<li><a href=manage_balance.php?gid=<?=$gid?>>벨런스</a></li>
							</ul>
						</li>
						<li><a href=admin_help.php?gid=<?=$gid?>>관리툴</a></li>
						<?php
							}
						?>

						<?php
							if($admin->checkPermission("admin","read")){
						?>
						<li><a href=manage_adminUser.php?gid=<?=$gid?>>운영자관리</a></li>
						<?php
							}
						?>
				  		<li><a href="logout.php?gid=<?=$gid?>">로그아웃</a></li>
				  	</ul>

				</div>
			</div>
			<div class="col-lg-10">

