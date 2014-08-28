<?php
include "manage_header.php";
include "../libs/dbchecker.php";


?>
<input name="gid" value="<?=$gid?>" type="hidden">

<br><br>

<table width=100%><tr><td><h2 id="tables-contextual-classes">|DB서버구성도</h2></td><td align=right><a href=manage_dbcheck_test.php>config_test.php TEST</a></td></table>
<br><br>
<?php




dbchecker("main");
dbchecker("user");
dbchecker("log");
?>


<?php
include "manage_footer.php";
?>