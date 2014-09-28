<?php
include "../lib.php";
include "./phpexcel/PHPExcel.php";
include "./phpexcel/PHPExcel/IOFactory.php";

$admin = new AdminUser($_SESSION["admin_no"]);
if(!$admin->isLogined()){
?>
	<script>
	location.href = 'index.php';
	</script>

<?php
exit;
}

if(!$admin->checkPermission("event","read")){
	echo "권한없음";
	exit;
}

$query = "where 1=0";

if($_GET["id"] && $_GET["type"]=="cuponNo"){
	$query =  "where cuponNo=".$_GET["id"];
}else if ($_GET["id"] && $_GET["type"]=="cuponCode"){
	$query = "where cuponCode='".$_GET["id"]."'";
}

// Create new PHPExcel object 
$objPHPExcel = new PHPExcel(); 

// Set properties 
// Excel 문서 속성을 지정해주는 부분이다. 적당히 수정하면 된다. 
$objPHPExcel->getProperties()->setCreator("CuponCode") 
                            ->setLastModifiedBy("CuponCode") 
                            ->setTitle("CuponCode") 
                            ->setSubject("CuponCode") 
                            ->setDescription("CuponCode") 
                            ->setCategory("CuponCode"); 

// Add some data 
// Excel 파일의 각 셀의 타이틀을 정해준다. 
$objPHPExcel->setActiveSheetIndex(0) 
            ->setCellValue("A1", "no") 
            ->setCellValue("B1", "cuponNo") 
            ->setCellValue("C1", "cuponCode") 
            ->setCellValue("D1", "serverNo");

$i=2;
while($cupon = CuponCode::getRowByQuery($query)){
	$objPHPExcel->setActiveSheetIndex(0)->setCellValue("A$i", $cupon["no"])->setCellValue("B$i", $cupon["cuponNo"])->setCellValue("C$i", $cupon["cuponCode"])->setCellValue("D$i", $cupon["serverNo"]); 
	$i++;
}
// for 문을 이용해 DB에서 가져온 데이터를 순차적으로 입력한다. 
// 변수 i의 값은 2부터 시작하도록 해야한다. 
// for ($i=2; $row=db_fetch_array($result); $i++) 
// {    
//     // Add some data 
//     $objPHPExcel->setActiveSheetIndex(0) 
//                 ->setCellValue("A$i", "$row[uid]") 
//                 ->setCellValue("B$i", "$row[name]") 
//                 ->setCellValue("C$i", "$row[slate_no]") 
//                 ->setCellValue("D$i", "$row[biz_name]") 
//                 ->setCellValue("E$i", "$row[tel]") 
//                 ->setCellValue("F$i", "$row[hp]") 
//                 ->setCellValue("G$i", "$row[address]") 
//                 ->setCellValue("H$i", "$row[site]") 
//                 ->setCellValue("I$i", "$row[invoice]") 
//                 ->setCellValue("J$i", "$row[status]") 
//                 ->setCellValue("K$i", "$row[license]") 
//                 ->setCellValue("L$i", "$row[license_date]"); 
// } 

// Rename sheet 
$objPHPExcel->getActiveSheet()->setTitle("cuponCode"); 

// Set active sheet index to the first sheet, so Excel opens this as the first sheet 
$objPHPExcel->setActiveSheetIndex(0); 

// 파일의 저장형식이 utf-8일 경우 한글파일 이름은 깨지므로 euc-kr로 변환해준다. 
$filename = "cuponcode_".date("YmdHis"); 

// Redirect output to a client's web browser (Excel5) 
header('Content-Type: application/vnd.ms-excel'); 
header('Content-Disposition: attachment;filename="' . $filename . '.xls"'); 
header('Cache-Control: max-age=0'); 

$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel5'); 
$objWriter->save('php://output'); 
exit; 
?>

