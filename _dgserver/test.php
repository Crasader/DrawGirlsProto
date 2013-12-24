<?php
include "lib.php";
echo"ok1";
//http://httpgw.kakao.hangame.com:11080/hsp/httpgw/nomad.json?api=help&version=1.0.0&headerGameNo=10226
//18080 -> 같은네트웍내에서
//11080 ->외부에서 사용가능
$api = $_GET['api'];
function get_content($url) {
	$agent = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)';
	$curlsession = curl_init();
	curl_setopt ($curlsession, CURLOPT_URL, $url);
	curl_setopt ($curlsession, CURLOPT_HEADER, 0);
	curl_setopt ($curlsession, CURLOPT_RETURNTRANSFER, 1);
	curl_setopt ($curlsession, CURLOPT_FOLLOWLOCATION, true);  //리다이렉트를 자동으로 잡아줘서 302가 아니라 200이 리턴됨
	curl_setopt ($curlsession, CURLOPT_MAXREDIRS, 10); //최대 리다이렉트 횟수
	curl_setopt ($curlsession, CURLOPT_POST, 0);
	curl_setopt ($curlsession, CURLOPT_USERAGENT, $agent);
	curl_setopt ($curlsession, CURLOPT_REFERER, "");
	curl_setopt ($curlsession, CURLOPT_TIMEOUT, 10);  //타임아웃 시간
	$buffer = curl_exec($curlsession);
	$cinfo = curl_getinfo($curlsession);
	curl_close($curlsession);
	
	echo"<pre>";
	var_dump($cinfo);
	
	echo"</pre><br><br>";
	if ($cinfo['http_code'] != 200)
	{
		return "Error";
	}else{
		return $buffer;
	}
}
//http://httpgw.kakao.hangame.com:11080/hsp/httpgw/help.json?api=$api&version=1.2
echo get_content("http://alpha-httpgw.hangame.com:18080/hsp/httpgw/help.json?api=$api&version=1.3");

?>