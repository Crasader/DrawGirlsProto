<?php
include_once("../lib.php");

$left_menu = array();
$left_menu[]=array("title"=>"","url"=>"");

?>
<!DOCTYPE html>
<html lang="ko">
	<head>
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
		<script src="lib.js"></script>
		<script src="LQDataTable.js"></script>

		<link rel="stylesheet" href="LQDataTable.css">
		<!-- Latest compiled and minified CSS -->

		<!-- Bootstrap styles -->
		<link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css">
		<!-- Generic page styles -->
		<link rel="stylesheet" href="jqueryupload/css/style.css">
		<!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
		<link rel="stylesheet" href="jqueryupload/css/jquery.fileupload.css">

		<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 
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
		            	[<a href=manage_userInfo.php?gid=<?=$gid?>>운영툴</a>] [<a href=admin_help.php?gid=<?=$gid?>>관리툴</a>]
		        	</center>
		        	<br>
		            <ul class="nav bs-sidenav">
		            	<!-- li><a href="#glyphicons">Glyphicons</a>
							<ul class="nav">
								<li><a href="#glyphicons-glyphs">사용가능한 기호</a></li>
								<li><a href="#glyphicons-how-to-use">사용법</a></li>
								<li><a href="#glyphicons-examples">예제</a></li>
							</ul>
		            	</li -->

		            	<li>유저관리
							<ul class="nav">
								<li><a href=manage_userInfo.php?gid=<?=$gid?>>userInfo<br>기본정보조회</a></li>
								<li><a href=manage_modifyHistory.php?gid=<?=$gid?>>modifyHistory<br>변경내역조회</a></li>
								<li><a href=manage_puzzleHistory.php?gid=<?=$gid?>>puzzleHistory<br>퍼즐정보</a></li>
								<li><a href=manage_pieceHistory.php?gid=<?=$gid?>>pieceHistory<br>피스정보</a></li>
								<li><a href=manage_characterHistory.php?gid=<?=$gid?>>characterHistory<br>캐릭터히스토리</a></li>
								<li><a href=manage_cardHistory.php?gid=<?=$gid?>>cardHistory<br>카드정보</a></li>
								<li><a href=manage_archivementHistory.php?gid=<?=$gid?>>archivementHistory<br>업적히스토리</a></li>
								<li><a href=manage_giftBoxHistory.php?gid=<?=$gid?>>giftBoxHistory<br>선물함히스토리</a></li>
								<li><a href=manage_userStorage.php?gid=<?=$gid?>>userStorage<br>보관함관리</a></li>
								<li><a href=manage_cuponHistory.php?gid=<?=$gid?>>cuponHistory<br>쿠폰사용내역</a></li>
								<li><a href=manage_userLog.php?gid=<?=$gid?>>userLog<br>유저로그</a></li>
								<li><a href=manage_userData.php?gid=<?=$gid?>>userdata<br>유저목록</a></li>
							</ul>
		            	</li>

		            	<li>이벤트
							<ul class="nav">
								<li><a href=manage_sendItem.php?gid=<?=$gid?>>sendItem<br>재화/아이템 정보변경</a></li>
								<li><a href=manage_notice.php?gid=<?=$gid?>>notice<br>공지</a></li>
								<li><a href=manage_loginEvent.php?gid=<?=$gid?>>loginEvent<br>로그인이벤트</a></li>
								<li><a href=manage_shopEvent.php?gid=<?=$gid?>>shopEvent<br>할인이벤트</a></li>
								<li><a href=manage_timeEvent.php?gid=<?=$gid?>>timeEvent<br>타임이벤트</a></li>
								<li><a href=manage_cuponManager.php?gid=<?=$gid?>>cuponManager<br>쿠폰관리</a></li>
								<li><a href=manage_cuponCode.php?gid=<?=$gid?>>cuponCode<br>쿠폰코드</a></li>
							</ul>
						</li>

		            	<li>미완성
							<ul class="nav">
								<li><a href=manage_attendenceEvent.php?gid=<?=$gid?>>attendenceEvent<br>출석이벤트</a></li>
								<li><a href=manage_balance.php?gid=<?=$gid?>>balance<br>벨런스</a></li>
								<li><a href=manage_userPropertyHistory.php?gid=<?=$gid?>>userPropertyHistory<br>보관함히스토리</a></li>
								<li><a href=manage_endlessPlayList.php?gid=<?=$gid?>>PVPPlayData<br>PVP플레이데이터</a></li>
							</ul>
						</li>
				  	</ul>
				</div>
			</div>
			<div class="col-lg-10">

