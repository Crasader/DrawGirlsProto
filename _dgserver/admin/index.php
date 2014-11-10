
<?php


	function filesInDir ($tdir) 
	{ 


		if(@opendir($tdir)) { 
			
			$files = Array(); 
			$in_files = Array(); 
		
			@exec("ls $tdir -t",$out);
			
			if(is_array($out) && count($out)>0){
			foreach($out as $a_file) { 
				if($a_file[0] != '.') { 
					if(@is_dir ($tdir . "/" . $a_file)) { 
						$in_files = filesInDir ($tdir . "/" . $a_file); 
						if(is_array ($in_files)) $files = array_merge ($files , $in_files); 
					} else { 
						array_push ($files , $tdir . "/" . $a_file); 
					} 
				} 
			} 
			
			//closedir ($dh); 
			return $files ; 
			}
		} 
	} 

	echo"<br><br><br><br><center>";
	echo"select game<br><br>";

	$realDir="../config";
	$i=0;
	
	foreach (filesInDir($realDir) as $key => $a_file){ 
		$d = explode("/",$a_file);
		$rc = count($d)-1;
		$filename = $d[$rc];
		$id = explode(".",$filename);
		echo"<a href=admin.php?gid=".$id[0].">".$id[0]."</a><br><br>";
	} 



	echo"</center>";

	?>




