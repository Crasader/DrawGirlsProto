<?php
include "header.php";

$stopCommand = true;

include "../command/cmd2.php";


?>
 <script>
var categoryName = "";
$(document).ready(function(){
	$('body').on('change','select[class=LQEditor]',function(){
		var divName = $(this).val();
		log("on div"+divName);

		categoryName = divName;

		$("div").css("display","none");
		$("div[name="+divName+"]").css("display","block");

		var datatable = getDataTable("datatable");
		datatable.attr("dbWhere",'{"category":"'+divName+'","limit":20}');
		loadDataTable(datatable,"reload");

	});

	$('body').on('click','.doAction',function(){
		var api = getLQEditorValue("a");
		var pdata = getLQEditorValue("p");
		var param = {"mode":"nodes","version":"2"};
		var apiurl = "http://182.162.201.147:10010/data.php";

		param["a"]=j2s(api);
		param["p"]=j2s(pdata);



		$.ajax({
				    url : apiurl, 
				    data : param,
				    dataType : "json", 
				    type : "get",
				    success : function(data){

				    	var resultDisplay = $("td[name=resultDisplay]");
				    	result = "요청:"+apiurl+"<br>파마메터:"+j2s(param)+"<br><br>결과:<br><textarea cols=130 rows=5>"+j2s(data["0"])+"</textarea>";
						resultDisplay.html(result);


						var datatable = getDataTable("datatable");
						datatable.attr("dbWhere",'{"category":"'+categoryName+'","limit":10}');
						loadDataTable(datatable,"reload");
					}
					,
					error: function(e) {
					    	alert("error : " + j2s(e));
					    }
					});



	});

});

</script>


<center>
 <table class="LQDataForm" border=1 width=50%>
 	<tr>
 		<td colspan=2>
 			<?php

$command = new commandClass();



$class_methods = get_class_methods($command);
$apiList=array();

$apiList[]="-----select api-------";
foreach ($class_methods as $method_name) {
    if(strpos($method_name,"help_")  !== false){
	    $realName = str_replace("help_","",$method_name);
	    $mInfo = $command->$method_name();
	    $apiList[]=$realName;
	    echo "<div name='".$realName."'' style='display:none'>";
	    echo "<font size=6 color=blue>".$realName."</font><br>";
	    echo "<b>".$mInfo["description"]."</b><br><br>";
	    echo "* parameter <br>";
	    echo "<table border=1><tr><td>name</td><td>type</td><td>description</td></tr>";
	    for($i=0;$i<count($mInfo["param"]);$i++){
		    echo "<tr><td>".$mInfo["param"][$i]["name"]."</td><td>".$mInfo["param"][$i]["type"]."</td><td>".$mInfo["param"][$i]["description"]."</td></tr>";
	    }
	    echo "</table><br>";
	    
	    echo"* result<br>";
	    echo "<table border=1><tr><td>code</td><td>name</td><td>message</td></tr>";
	    for($i=0;$i<count($mInfo["result"]);$i++){
		    echo "<tr><td>".$mInfo["result"][$i]["code"]."</td><td>".$mInfo["result"][$i]["name"]."</td><td>".$mInfo["result"][$i]["message"]."</td></tr>";
	    }
	    echo "</table><br>";


	    if($mInfo["comment"]){
		    echo"* comment<br>";
		    echo "<table border=1><tr><td>".nl2br($mInfo["comment"])."</td></tr>";

		    echo "</table><br><br>";
		}

	    echo"</div>";
    }
}

$apiListStr = json_encode($apiList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

 			?>

 		</td>

 	</tr>
 	<tr>
		<td>api</td>	
		<td field="a" editor='{"type":"select","element":<?=$apiListStr?>}'></td>
	</tr>
	<tr>
		<td>param</td>
		<td field="p" editor='{"type":"dictionary"}'></td>
	</tr>
	<tr><td colspan=2><input type="submit" value="실행" class="doAction"></td></tr>
	<tr><td colspan=2 name="resultDisplay">
		

	</td></tr>
</table>

<script>
	var textareaViewer = function(obj,value){
		return "<textarea rows=3 cols=50>"+j2s(obj)+"</textarea>";
	}
</script>
<br><br>
<b>--최근로그--</b><br>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserLog" dbFunc='{"select":"getHistory"}' dbWhere='' name="datatable" border=1>
	<thead>
		<tr>
			<th field="no" viewer='{"type":"text"}' primary>no</th>
			<th field="category" viewer='{"type":"text"}'>category</th>
			<th field="ip" viewer='{"type":"text"}'>ip</th>
			<th field="input" viewer='{"type":"custom","func":"textareaViewer"}'>input</th>
			<th field="output" viewer='{"type":"custom","func":"textareaViewer"}'>output</th>
			<th field="regDate" viewer='{"type":"text"}'>regDate</th>
			<th field="regTime" viewer='{"type":"text"}'>regtime</th>
			<th field="execTime" viewer='{"type":"text"}'>exectime</th>
		</tr>
	</thead>
	<tbody datazone>

	</tbody>

	<tfoot>
		<tr>
			<td colspan="8" align="center">
				<a href="#next" class="LQLoadNext">Next</a>
			</td>
		</tr>
	</tfoot>
</table>

</center>
<?php
include "footer.php";
?>