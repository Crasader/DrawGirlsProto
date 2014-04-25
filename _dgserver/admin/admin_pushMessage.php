<?php
include "header.php";
?>



<form action=<?=$_SERVER["PHP_SELF"]?>>
<input type="hidden" name="mode" value="send">
<input name="gid" value="<?=$gid?>" type="hidden">
memberNo : <input name='memberNo' type="text" value=<?=$_GET["memberNo"]?>> 0 : 전체<br>
gameNo : <input name='gameNo' type="text" value=<?=$_GET["gameNo"]?>> tstore : 10289<br>
osNo : <input name='osNo' type="text" value=<?=$_GET["osNo"]?>> 0:전체 , 1: ios, 2: android<br>
message : <input name='message' type="text" value=<?=$_GET["message"]?>> <br>
param : <input name='param' type="text" value="<?=$_GET["param"]?>">  key-value <br>
<input type="submit" value="send">
</form>

<?php
if($_GET["mode"]=="send"){
	echo "-------------------- result --------------------- <br>";
	$data = array();
	$data["memberNo"]=$_GET["memberNo"];
	$data["gameNo"]=$_GET["gameNo"];
	$data["osNo"]=$_GET["osNo"];
	$data["message"]=$_GET["message"];
	$data["api"]="SendPushMsg";
	if(!$data[version])$data[version]=$HSP_VERSION;
	if(!$data[headerGameNo])$data[headerGameNo]=$_GET["gameNo"];
	
	//$data[gameNo]=(int)$HSP_GAMENO;
	
	//$data_string = json_encode($data); 
	
	foreach($data as $key=>$value) { 
		if(is_scalar($value))
			$fields_string .= $key.'='.urlencode($value).'&';
		else if(is_array($value)){
			$nvalue ="";
			foreach($value as $key2=>$value2){
				$nvalue.=$value2.",";
			}
			$nvalue=rtrim($nvalue, ',');

			$fields_string .= $key.'='.urlencode($nvalue).'&';
		} 
		
	}
	$fields_string=rtrim($fields_string, '&');
	
	
	$agent = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)';
	$curlsession = curl_init();
	curl_setopt($curlsession, CURLOPT_URL, $HSP_GATEWAY_URL."?".$fields_string); //hsp/httpgw/help.json?api=$api&version=1.3
	curl_setopt($curlsession, CURLOPT_PORT, $HSP_PORT);
	curl_setopt($curlsession, CURLOPT_FOLLOWLOCATION, true);  //리다이렉트를 자동으로 잡아줘서 302가 아니라 200이 리턴됨
	curl_setopt($curlsession, CURLOPT_MAXREDIRS, 10); //최대 리다이렉트 횟수
	curl_setopt($curlsession, CURLOPT_USERAGENT, $agent);
	curl_setopt($curlsession, CURLOPT_REFERER, "");
	curl_setopt($curlsession, CURLOPT_TIMEOUT, 10);  //타임아웃 시간
	curl_setopt($curlsession, CURLOPT_CUSTOMREQUEST, "GET");                                                                     
	curl_setopt($curlsession, CURLOPT_POSTFIELDS, $fields_string);                                                                  
	curl_setopt($curlsession, CURLOPT_RETURNTRANSFER, true);                                                                                                        

	$buffer = curl_exec($curlsession);
	$cinfo = curl_getinfo($curlsession);
	curl_close($curlsession);
	if ($cinfo['http_code'] != 200)
	{
		$r["state"]="error";
		$r["msg"]="gateway server error";
		$r["send"]=$HSP_GATEWAY_URL."?".$fields_string;
	}else{
		$r=json_decode($buffer,true);
		$r["state"]="ok";
		$r["send"]=$HSP_GATEWAY_URL."?".$fields_string;
	}
	echo json_encode($r,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
}

?>