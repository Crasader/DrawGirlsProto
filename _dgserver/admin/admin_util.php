<?php
include "header.php";
?>


<form action=admin_util.php>
pass:	<input type="text" name="pass">
<input type=submit value="go">
</form>

<?php

if($_GET["pass"]=="12345"){
	while($userindex = UserIndex::getRowByQuery()){
		$mID = $userindex["memberID"];
		$userData = new UserData($mID);
		$character = new CharacterHistory($mID,1);
		if($userData->level==0){
			$charLevel = $character->level;
			if($charLevel==0)$charLevel=1;
			$userData->level = $charLevel;
			$character->level = 1;
			$character->save();
			$userData->save();

			echo $userData->nick." save <br>";
		}
	}
}
?>

<?php
include "footer.php";
?>