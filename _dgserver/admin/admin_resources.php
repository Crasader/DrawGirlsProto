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
			$("body").on("click",".selectResource", function(e){ //user click on remove text
				imgurl = $(this).attr('imgname')
				fsize = filesize(imgurl)/1024;
				
				imgurl = imgurl.replace("../images/", "");
				
				log("select"+imgurl+" size:"+fsize);
				
				opener.changeJsonFormInputByResource(<?php echo $_GET[delkey]; ?>,imgurl,fsize);

		        //$(opener.document).find("#{{ input }}").val($(this).prop('src')).change();
		        window.close();
				return false;
			}) 

			$('#imageupload').on("change", function(){ 
				var file = $(this)[0].files[0];
				$("#imagename").val(file.name);

			 });
			

		});




</script>





<table border="1" align="center" width=800>
	<tr><td>



<?php 
$uploaderForm = '<input type="hidden" name="category" value="resource"><input type="hidden" name="randfilename" value="false"> overwrite<input type="checkbox" name="overwrite" value="true">';

include "jqueryupload/uploader.html";
 
?>

	</td></tr>
</table>
<br>


<table align=center border=1><tr><td>
<?php
$realDir="../images/resource";
$i=0;
foreach (filesInDir($realDir) as $key => $a_file){ 
	$i++;
	echo "<a href=#a src='".$a_file ."' width='200' class='selectResource' imgname='".$a_file."'>$a_file<a><br>" ; 
} 
?>
</td></tr></table>



<?php
include "footer.php";
?>


