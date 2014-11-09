<?php
include "header.php";
?>

<script>
	function filesize (url) {
    var req = window.ActiveXObject ? new ActiveXObject("Microsoft.XMLHTTP") : new XMLHttpRequest();
    if (!req) throw new Error('XMLHttpRequest not supported');
    
    req.open ('HEAD', url, false);
    req.send (null);
    
    if (!req.getResponseHeader) {
        try {
            throw new Error('No getResponseHeader!');
        } catch(e){
            return false;
        }
    } else if (!req.getResponseHeader('Content-Length')) {
        try {
            throw new Error('No Content-Length!');
        } catch(e){
            return false;
        }
    } else {
        return req.getResponseHeader('Content-Length'); 
    }
    }
    
		$(document).ready(function() {
			$("body").on("click",".selectImage", function(e){ //user click on remove text
				imgurl = $(this).attr('imgname')
				fsize = filesize(imgurl)/1024;
				
				imgurl = imgurl.replace("../images/", "http://<?php echo $_SERVER["SERVER_NAME"].":".$_SERVER["SERVER_PORT"]; ?>/images/");
				
				log("select"+imgurl+" size:"+fsize);
				
				opener.changeJsonFormInput(<?php echo $_GET[delkey]; ?>,imgurl,fsize);

		        //$(opener.document).find("#{{ input }}").val($(this).prop('src')).change();
		        window.close();
				return false;
			}) 

			$('#imageupload').on("change", function(){ 
				var file = $(this)[0].files[0];
				$("#imagename").val(file.name);

			 });
			

		});


	var imageViewFunc = function(value,option){
		return "<img src='../images/"+value+"' class='selectImage' imgname='"+value+"' width=100>";
	}

</script>





<table border="1" align="center" width=800>
	<tr><td>

<?php
$categorys = array("card","cbt","obt","monster","sil","item","mission","pattern","theme","thumbnail","puzzle","piece","ui","etc","frame");


$uploaderForm = '<select name="category" id="imageCategory">';

for($i=0;$i<count($categorys);$i++){
	$uploaderForm.="<option value=".$categorys[$i].">".$categorys[$i]."</option>";
}

$uploaderForm.="</select>";

$uploaderForm.='writeMode :
<select name="writeMode">
<option value="random">random : 랜덤파일명</option>
<option value="update">update : 숫자늘리기</option>
<option value="overwrite">overwrite : 덮어쓰기</option>
</select>';

?>


<?php 

include "jqueryupload/uploader.html";
 
?>

	</td></tr>
</table>
<br>

<table border=1 align=center>
<tr>
<?php

echo"<td><a href=".$_SERVER["PHP_SELF"]."?gid=$gid>all</a></td>";
	
for($i=0;$i<count($categorys);$i++){
	echo"<td><a href=".$_SERVER["PHP_SELF"]."?gid=$gid&category=".$categorys[$i]."&delkey=".$_GET[delkey].">".$categorys[$i]."</a></td>";
}

?>

<td>
</table>
<table align=center border=1><tr>
<?php
$realDir="../images";
if($_GET["category"])$realDir.="/".$_GET["category"];
$i=0;
foreach (filesInDir ($realDir) as $key => $a_file){ 
	$i++;
	echo "<td><a href=#a class='selectImage' imgname='".$a_file."'>";
	if($i<100)echo "<img src='".$a_file ."' width='200'>";
	echo "<br><font size=1>$a_file</font></a></td>" ; 
	
	if($i%5==0)echo"</tr><tr>";
} 
?>
</tr></table>



<?php
include "footer.php";
?>


