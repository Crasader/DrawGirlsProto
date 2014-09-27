<?php

include "../lib.php";

if($gid){
	$_SESSION['game_id']=$gid;
}


$admin = new AdminUser($_SESSION["admin_no"]);
if($admin->isLogined() && !$_GET["mode"]){
	?>

	<script>
	location.href = 'manage_userInfo.php?gid=<?=$gid?>';
	</script>

	<?php
	exit;
}

?>
<html>
<head>
<title>Login</title>
	<script>
		var gid = '<?=$gid?>';



	</script>
	<script src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
	<script src="http://code.jquery.com/ui/1.10.4/jquery-ui.min.js"></script>
	<link rel="stylesheet" href="http://code.jquery.com/ui/1.10.4/themes/black-tie/jquery-ui.css">
	<script src="jquery-ui-timepicker-addon.js"></script>
	<script src="jquery.form.js"></script>
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
	<script>
		var loginResult = function(result){
			if(result["isLogined"]){
				$(location).attr('href',"admin.php?gid=<?=$gid?>");
				return false;
			}else{
				alert("error","로그인실패");
			}
		}

	</script>
</head>
<body>
<br><br><br><br>

<table class="LQDataForm" autoLoad="false" dbSource="dataManager2.php" dbClass="AdminUser" dbWhere='' name="loginTable" align=center startMode="update">
<tr>
	<td>ID</td><td field="id" editor='{"type":"text"}'></td>
</tr>
<tr>
	<td>PASSWORD</td><td field="passwd" editor='{"type":"password"}'></td>
</tr>
<tr>
	<td colspan=2>
		<button class="LQSendButton btn btn-primary" func="login" callbackFunc="loginResult">로그인</button>
	</td>
</tr>
</table>
</body>
</html>
	