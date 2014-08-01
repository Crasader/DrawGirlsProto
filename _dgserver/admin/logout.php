<?php
include "../lib.php";


$admin = new AdminUser($_SESSION["admin_no"]);

$admin->logout();

?>

<script>
location.href = 'index.php';
</script>
