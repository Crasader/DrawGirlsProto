<!DOCTYPE html>

<html>
<head>
<title>admin</title>
		<script src="http://code.jquery.com/jquery-2.0.3.min.js"></script>
		<script src="../lib.js"></script>
		<script src="../LQDataTable.js"></script>
		<!-- Latest compiled and minified CSS -->

<!-- Bootstrap styles -->
<link rel="stylesheet" href="//netdna.bootstrapcdn.com/bootstrap/3.0.0/css/bootstrap.min.css">
<!-- Generic page styles -->
<link rel="stylesheet" href="../jqueryupload/css/style.css">
<!-- CSS to style the file input field as button and adjust the Bootstrap progress bars -->
<link rel="stylesheet" href="../jqueryupload/css/jquery.fileupload.css">

<meta http-equiv="Content-Type" content="text/html; charset=utf-8"> 

    <script>

	$(document).ready(function() {
		$("body").on("click","#okbtn", function(e){ //user click on remove text
			var value = $("#output").val();
			var pathDict = JSON.parse(value);
			var pathArray = [];
			var j=0;
			for(var i=0;i<pathDict.length;i++){
				pathArray[j++]=pathDict[i]["x"];
				pathArray[j++]=pathDict[i]["y"];
			}
			
			var result = {"path":pathArray};
			log("path"+j2s(pathArray));
			$("#output2").val(j2s(pathArray));
			opener.LQSelectedData("<?=$_GET[delkey]?>",result);
	        window.close();
			return false;
		}) 
	});


	var nameViewer = function(value,option){
		return "<input type=button value='"+value+"' class=selectObj>";
	}
	
	var imageViewer = function(value,option){
		return "<img src='../images/"+value+"' width=100>";
	}

</script>

</head>
<body style="text-align: center;background: #f2f6f8;">
<div style="display:inline-block;width:auto; margin: 0 auto; background: black; position:relative; border:5px solid black; border-radius: 10px; box-shadow: 0 5px 50px #333">
    <canvas id="gameCanvas" width="960" height="640"></canvas>
</div>

<!-- <canvas id="gameCanvas" width="480" height="320"></canvas> -->
<script src="cocos2d.js"></script>

<br />
<br />
잘쓰렴~
<input type="text" id="output" size="250"> <br />
<input type="button" id="btnBack" value="Undo"> <br />
<label><input type="checkbox" id="curve" checked>curve</label> <br /> <br />
<input type="button" id="okbtn" value="save&close"> <br />
임시로~~ <input type="text" id="output2" size="250">
</body>
</html>