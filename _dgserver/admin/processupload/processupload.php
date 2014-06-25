<?php

include "../phpexcel/PHPExcel.php";
include "../phpexcel/PHPExcel/IOFactory.php";

if(isset($_FILES["FileInput"]) && $_FILES["FileInput"]["error"]== UPLOAD_ERR_OK)
{
	############ Edit settings ##############
	$UploadDirectory	= "../".$_POST["uploadDir"]; //'../../images/'; //specify upload directory ends with / (slash)
	##########################################
	
	/*
	Note : You will run into errors or blank page if "memory_limit" or "upload_max_filesize" is set to low in "php.ini". 
	Open "php.ini" file, and search for "memory_limit" or "upload_max_filesize" limit 
	and set them adequately, also check "post_max_size".
	*/
	
	$r = array();

	//check if this is an ajax request
	if (!isset($_SERVER['HTTP_X_REQUESTED_WITH'])){
		die();
	}
	
	
	//Is file size is less than allowed size.
	if ($_FILES["FileInput"]["size"] > 52428800) {
		die("File size is too big!");
	}
	
	//allowed file type Server side check
	switch(strtolower($_FILES['FileInput']['type']))
		{
			//allowed file types
            case 'image/png': 
			case 'image/gif': 
			case 'image/jpeg':
			case 'application/xls':
			case 'application/octet-stream':
				break;
			default:
				die('{"result":{"code":1002},"filetype":"'.strtolower($_FILES['FileInput']['type']).'"}'); //output error
	}
	
	$File_Name          = strtolower($_FILES['FileInput']['name']);
	$File_Ext           = substr($File_Name, strrpos($File_Name, '.')); //get file extention
	$Random_Number      = rand(0, 9999999999); //Random number to be added to name.
	$NewFileName 		= $Random_Number.$File_Ext; //new file name
	
	if(move_uploaded_file($_FILES['FileInput']['tmp_name'], $UploadDirectory.$NewFileName ))
	   {
	   	$r = json_decode('{"result":{"code":1},"filename":"'.$UploadDirectory.$NewFileName.'"}',true);
	   	$r["param"]=$_POST;

	   	if($_POST["excelReturn"]){

	   		$filename = $UploadDirectory.$NewFileName;
			try {
			    // 업로드 된 엑셀 형식에 맞는 Reader객체를 만든다.
			 $objReader = PHPExcel_IOFactory::createReaderForFile($filename);
			 // 읽기전용으로 설정
			 $objReader->setReadDataOnly(true);
			 // 엑셀파일을 읽는다
			 $objExcel = $objReader->load($filename);
			 // 첫번째 시트를 선택
			 $objExcel->setActiveSheetIndex(0);
			 $objWorksheet = $objExcel->getActiveSheet();
			 
			 //$r["table"]+="<table>";
			 $rowIterator = $objWorksheet->getRowIterator();
			 $x=0; $c=0;
			 foreach ($rowIterator as $row) { // 모든 행에 대해서
			  //$r["table"]+="<tr>";
			  $cellIterator = $row->getCellIterator();
			  $cellIterator->setIterateOnlyExistingCells(false); 
			  foreach ($cellIterator as $cell) { // 해당 열의 모든 셀에 대해서
			 	 $r["excelData"][$x][$c]=$cell->getValue();
			   $c++;
			  //$r["table"]+="<td>".$cell->getValue()."</td>";
			  }
			  //$r["table"]+="<tr>";
			 }
			 //$r["table"]+="</table>";
			 	$x++;
			} 
			catch (exception $e) {
			    $r = json_decode('{"result":{"code":1001},"msg":"엑셀파일을 읽는도중 오류가 발생하였습니다.",filename":"'.$UploadDirectory.$NewFileName.'"}',true);
				die(json_encode($r));
			}

			if($_POST["removeFile"]){
				$file_server_path = realpath(__FILE__);
				// PHP 파일 이름이 들어간 절대 서버 경로
				$php_filename = basename(__FILE__);
				// PHP 파일 이름
				$server_path = str_replace(basename(__FILE__), "", $file_server_path);
				// PHP 파일 이름을 뺀 절대 서버 경로
				
				$path = $server_path.$UploadDirectory.$NewFileName;
				$r["path"]=$path;
				if(unlink($path)){
					$r["removeFile"]=true;
				}else{
					$r["removeFile"]=false;
				}

			}
	   	}

		die(json_encode($r));
	}else{
		$r = json_decode('{"result":{"code":1001},"filename":"'.$UploadDirectory.$NewFileName.'"}',true);
	   	$r["param"]=$_POST;
		die(json_encode($r));
	}
	
}
else
{
	die('{"result":{"code":1001}}');
}