<?php
Class CommandHelpClass{



public static function help_httpgateway(){

	$r["description"] = "httpgateway를 이용해 hsp서버에 접속합니다.";
	
	//$r["param"]
	
	$r["param"][] = array("name"=>"api","type"=>"string","description"=>"사용할 api 이름");
	$r["param"][] = array("name"=>"version","type"=>"float","description"=>"hsp버전(없으면 기본값설정됨)".Infomation::$m_httpgateway["version"].")");
	$r["param"][] = array("name"=>"headerGameNo","type"=>"int","description"=>"게임번호(없으면 기본값설정됨) : ".Infomation::$m_gameNo.")");
	$r["param"][] = array("name"=>"gameNo","type"=>"int","description"=>"게임번호(없으면 기본값설정됨): ".Infomation::$m_gameNo.")");
	$r["param"][] = array("name"=>"url","type"=>"string","description"=>"httpgateway주소(없으면 기본값)".Infomation::$m_httpgateway["URL"].")");
	$r["param"][] = array("name"=>"help","type"=>"bool","description"=>"true면 api정보보기(기본값 false)");

	//$r["return"]
	
	$r["result"][]=ResultState::toArray(3001,"httpgateway 접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}



public static function help_requestitemdelivery(){

	$r["description"] = "구매내역을 확인하여 아이템을 배송합니다.";
	
	//$r["param"]
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID");
	$r["param"][] = array("name"=>"deliveryTxId","type"=>"string or int","description"=>"배송 요청에 대한 transactionId.");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(3001,"httpgateway 접속실패");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2016,"처리할내용이 없음");
	$r["result"][]=ResultState::toArray(1,"success");

	//$r["example"]
	
	return $r;
}

public static function help_getcommonsetting(){

	$r["description"] = "게임 기본 설정을 가져옵니다.";
	
	//$r["param"]
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}




public static function help_getnoticelist(){

	$r["description"] = "공지사항목록을 가져옵니다.";
	//$r["param"]
	
	$r["param"][] = array("name"=>"buildNo","type"=>"int","description"=>"빌드번호");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}


public static function help_getrealtimemessage(){

	$r["description"] = "실시간 메세지를 가져옵니다.";
	//$r["param"]
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}

public static function help_getnoticelistfordiary(){

	$r["description"] = "다이어리용 공지사항목록을 가져옵니다.";
	//$r["param"]
	
	$r["param"][] = array("name"=>"buildNo","type"=>"int","description"=>"빌드번호");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}



public static function help_getpuzzleevent(){

	$r["description"] = "이벤트퍼즐을 가져옵니다.";
	//$r["param"]
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1001,"db쿼리실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	//$r["example"]
	
	return $r;
}


public static function help_getcharacterlist(){

	$r["description"] = "케릭터 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


public static function help_getshoplist(){

	$r["description"] = "샵 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

public static function help_getmonsterlist(){

	$r["description"] = "몬스터 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

public static function help_getpieceinfo(){

	$r["description"] = "피스 정보를 받아옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"피스 번호");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1003,"디비fetch실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


public static function help_getcardlist(){

	$r["description"] = "카드정보 목록을 받아옵니다.";
	
	$r["param"][] = array("name"=>"noList","type"=>"array(int)","description"=>"카드번호목록");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"fail to get mainConnection");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}



public static function help_getallcardlist(){

	$r["description"] = "버전보다 높은 모든 카드정보 목록을 받아옵니다.";
	
	$r["param"][] = array("name"=>"noList","type"=>"array(int)","description"=>"카드번호목록");
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"fail to get mainConnection");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}



public static function help_getcardlistbylist(){

	$r["description"] = "파라메터로 넣은 카드목록중 업데이트된 카드목록만 가져옵니다.";
	
	$r["param"][] = array("name"=>"noList","type"=>"array(int)","description"=>"카드번호목록");
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"fail to get mainConnection");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


public static function help_getadultcardlist(){

	$r["description"] = "성인용 카드 시리얼번호 리스트를 불러옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"fail to get mainConnection");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


public static function help_getpuzzlelist(){

	$r["description"] = "퍼즐 정보 목록을 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"퍼즐정보목록버전");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"얻어올 시작 퍼즐번호");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작번호부터 순서대로 갯수");
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

public static function help_writelog(){

	$r["description"] = "로그를 작성합니다.";
	
	$r["param"][] = array("name"=>"memberID","type"=>"int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"input (exname is content)","type"=>"string","description"=>"내용");
	$r["param"][] = array("name"=>"output","type"=>"string","description"=>"결과");
	$r["param"][] = array("name"=>"category","type"=>"string","description"=>"카테고리");
	
	$r["result"][]=ResultState::toArray(1001,"로그작성실패");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}
public static function help_dropoutuser(){

	$r["description"] = "회원탈퇴";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"탈퇴할 회원 아이디");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"데이터베이스로인한 삭제오류");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2003,"찾을수없음");
	return $r;
}


public static function help_login(){

	$r["description"] = "로그인";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	$r["param"][] = array("name"=>"diaryCode","type"=>"string","description"=>"다이어리앱일경우 인증코드");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2007,"가입필요");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(ResultState::GDBLOCKEDUSER,"blocked");
	return $r;
}


public static function help_saveintroducer(){

	$r["description"] = "추천인저장 및 보상받기";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"추천인 닉네임");
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"선물메세지 내용");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFIND,"추천인을 찾지 못함");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFINDUSER,"유저를 찾지 못함");
	$r["result"][]=ResultState::toArray(ResultState::GDALREADY,"이미 저장된 추천인이 있음");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTSAVE,"저장실패");
	return $r;
}


public static function help_completeintroducer(){

	$r["description"] = "추천인에게 보상주기";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDPARAMETER,"추천인이 저장되어 있지 않음");
	$r["result"][]=ResultState::toArray(ResultState::GDALREADY,"이미 추천인 보상을 주었음");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFIND,"추천인을 찾을수 없음");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTSAVE,"저장실패");
	return $r;
}

public static function help_sendmessage(){

	$r["description"] = "메세지보내기";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	$r["param"][] = array("name"=>"friendID","type"=>"string or int","description"=>"friendID");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"nick");
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"내용");
	$r["param"][] = array("name"=>"data","type"=>"string or dict","description"=>"data");
	$r["param"][] = array("name"=>"exchangeID","type"=>"string or dict","description"=>"exchangeID");
	$r["param"][] = array("name"=>"template","type"=>"string","description"=>"템플릿으로 사용할 commonsetting key");
	$r["param"][] = array("name"=>"custom","type"=>"array(string)","description"=>"템플릿 커스텀");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	return $r;
}

public static function help_checkdiarycode(){

	$r["description"] = "다이어리코드를 체크하고 로그인용코드를 생성하여 돌려줍니다.";
	
	$r["param"][] = array("name"=>"diaryCode","type"=>"string or int","description"=>"diaryCode");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDEXPIRE,"잘못된 코드");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTSAVE,"저장실패/인증오류");
	return $r;
}


public static function help_makediarycode(){

	$r["description"] = "디이어리코드를 생성합니다.";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	return $r;
}


public static function help_join(){

	$r["description"] = "가입";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	$r["param"][] = array("name"=>"flag","type"=>"string","description"=>"국기");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"닉네임중복");
	$r["result"][]=ResultState::toArray(2008,"닉네임중복");
	$r["result"][]=ResultState::toArray(2009,"불량닉네임");
	$r["result"][]=ResultState::toArray(2010,"이미회원임");
	$r["result"][]=ResultState::toArray(2011,"너무긴닉네임");
	$r["result"][]=ResultState::toArray(2012,"너무짧은닉네임");
	$r["result"][]=ResultState::toArray(2002,"memberID or nick");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	return $r;
}

public static function help_setuserdata(){

	$r["description"] = "유저데이터 저장";
	
	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"memberID");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"저장할데이터 json string");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	return $r;
}

public static function help_getuserdata(){

	$r["description"] = "유저데이터 로드";
	
	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"멤머ID로 찾기");
	$r["param"][] = array("name"=>"userIndex","type"=>"int or string","description"=>"유저인덱스로 찾기");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임 으로 찾기");
	$r["param"][] = array("name"=>"keyList","type"=>"array(string)","description"=>"받아올키목록, 없으면 모두 받아옴");
	$r["param"][] = array("name"=>"isPublic","type"=>"bool","description"=>"다른유저의 정보받아올때 true, 기본 false");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID를 안넣음");
	$r["result"][]=ResultState::toArray(2003,"정보로드실패");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFINDUSER,"정보얻기실패");
	
	return $r;
}

public static function help_getuserdatalist(){

	$r["description"] = "유저데이터목록 로드";
	
	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string)","description"=>"멤머ID목록");
	$r["param"][] = array("name"=>"keyList","type"=>"array(string)","description"=>"받아올키목록, 없으면 모두 받아옴");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberIDList를 안넣음");
	
	return $r;
}

public static function help_updateuserdata(){

	$r["description"] = "유저데이터 업데이트";

	$r["param"][] = array("name"=>"memberID","type"=>"string","description"=>"멤버ID");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	$r["param"][] = array("name"=>"isVIP","type"=>"int","description"=>"0이면 무료유저, 1이면 인앱구매유저");
	$r["param"][] = array("name"=>"onlyOneBuyPack","type"=>"int","description"=>"구매유도플로우저장용");
	$r["param"][] = array("name"=>"isFirstBuy","type"=>"int","description"=>"이벤트팩구입저장용");
	$r["param"][] = array("name"=>"totalPlayCount","type"=>"int","description"=>"총플레이횟수");
	$r["param"][] = array("name"=>"failCount","type"=>"int","description"=>"연속실패카운트");
	$r["param"][] = array("name"=>"autoLevel","type"=>"int","description"=>"오토벨런싱용레벨");
	$r["param"][] = array("name"=>"selectedCharNO","type"=>"int","description"=>"선택된캐릭터번호");
	$r["param"][] = array("name"=>"highScore","type"=>"int","description"=>"최고점수");
	$r["param"][] = array("name"=>"highPiece","type"=>"int","description"=>"최고피스");
	$r["param"][] = array("name"=>"aMapGacha","type"=>"int","description"=>"업적수치(맵가챠카운트)");
	$r["param"][] = array("name"=>"aNoFail","type"=>"int","description"=>"업적수치(연속클리어횟수)");
	$r["param"][] = array("name"=>"aHunter","type"=>"int","description"=>"업적수치(잡은몬스터마리수)");
	$r["param"][] = array("name"=>"aChange","type"=>"int","description"=>"업적수치(체인지횟수)");
	$r["param"][] = array("name"=>"aFail","type"=>"int","description"=>"업적수치(실패횟수)");
	$r["param"][] = array("name"=>"aPerfect","type"=>"int","description"=>"업적수치(클리어횟수)");
	$r["param"][] = array("name"=>"aSeqAtd","type"=>"int","description"=>"업적수치(연속출석횟수)");
	$r["param"][] = array("name"=>"aItByCnt","type"=>"int","description"=>"아이템구매횟수");
	$r["param"][] = array("name"=>"archiveData","type"=>"dict","description"=>"업적데이터배열");
	$r["param"][] = array("name"=>"loginType","type"=>"dict","description"=>"로그인타입");
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID 안넣음");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	
	return $r;
}


public static function help_getuserlistbyrandom(){

	$r["description"] = "추천친구목록";

	$r["param"][] = array("name"=>"memberID","type"=>"string","description"=>"멤버ID");
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	
	return $r;
}


public static function help_getuserlist(){

	$r["description"] = "유저데이터목록";

	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string)","description"=>"멤버ID array");
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFINDUSER,"유저목록이 없거나 찾지못함");
	
	return $r;
}


public static function help_getintroducereward(){

	$r["description"] = "추천인보상정보불러오기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID ");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFIND,"오류");
	
	return $r;
}


public static function help_gethellmodelist(){

	$r["description"] = "헬모드목록불러오기";

	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"버전");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDSAMEVERSION,"버전이 같음");
	
	return $r;
}
public static function help_getfriendlist(){

	$r["description"] = "친구목록불러오기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버ID");
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDPARAMETER,"memberID없음");
	
	return $r;
}

public static function help_addfriend(){

	$r["description"] = "친구추가";

	$r["param"][] = array("name"=>"memberID","type"=>"string","description"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string","description"=>"친구추가할 멤버ID");
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFINDUSER,"유저를 찾지 못함");
	$r["result"][]=ResultState::toArray(ResultState::GDFRIENDMAX,"내친구 제한 넘었을때는 [result][message] 필드에 'me', 상대방이 넘었을땐 'you' 들어옴");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTSAVE,"저장실패");
	
	return $r;
}

public static function help_removefriend(){

	$r["description"] = "친구삭제";

	$r["param"][] = array("name"=>"memberID","type"=>"string","description"=>"멤버ID");
	$r["param"][] = array("name"=>"friendID","type"=>"string","description"=>"삭제할 친구 멤버ID");
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFINDUSER,"유저를 찾지 못함");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTSAVE,"삭제오류");
	
	return $r;
}

public static function help_addweeklyscore(){

	$r["description"] = "주간점수누적하기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"점수");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	$r["param"][] = array("name"=>"flag","type"=>"string","description"=>"국기");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}
public static function help_addweeklyandstagescore(){

	$r["description"] = "주간점수누적, 스테이지스코어등록 후 랭킹받기";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"stage","type"=>"int","description"=>"스테이지번호");
	$r["param"][] = array("name"=>"myscore","type"=>"int","description"=>"점수");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"닉네임");
	$r["param"][] = array("name"=>"flag","type"=>"string","description"=>"국기");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}


public static function help_getweeklyrankbyalluser(){

	$r["description"] = "주간점수목록(같은서버유저전체)";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"내아이디");
	$r["param"][] = array("name"=>"weekNo","type"=>"int","description"=>"주간번호, 없을경우 이번주");
	$r["param"][] = array("name"=>"start","type"=>"int","description"=>"시작등수(기본값 1)");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"시작등수로 부터 아래로 몇까지?(기본 10, 최대 50)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

public static function help_checkweeklyreward(){

	$r["description"] = "주간점수목록(같은서버유저전체)";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"내아이디");

	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}
// public static function help_adduserdata(){

// 	$r["description"] = "유저데이터 키 더하기";

// 	$r["param"][] = array("name"=>"memberID","type"=>"string","description"=>"멤버ID");
// 	$r["param"][] = array("name"=>"key","type"=>"string","description"=>"올릴키");
// 	$r["param"][] = array("name"=>"value","type"=>"int","description"=>"값");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
// 	$r["result"][]=ResultState::toArray(2002,"memberID");
// 	$r["result"][]=ResultState::toArray(2006,"정보저장실패");
	
// 	return $r;
// }


// public static function adduserdata($p){
// 	$memberid = $p["memberID"];
// 	$key = $p["key"];
// 	$value = $p["value"];
// 	$safekey = $p["safekey"];
// 	if($memberid){
// 		$user = UserData::create($memberid);
		
// 		$udata = json_decode($user->data,true);
// 		if(is_numeric($udata[$key]))$udata[$key]+=$value;
// 		else if(!$udata[$key])$udata[$key]=$value;
		
// 		$user->data = json_encode($udata,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);

		
// 		if($user->save())return ResultState::makeReturn(2006);
		
// 		$r["value"]=$udata[$key];
// 		$r["state"]="ok";
// 		$r["result"]=ResultState::successToArray();
// 	}else{
// 		$r["state"]="error";
// 		$r["result"]=ResultState::toArray(2002,"memberID");
		
// 	}
	
// 	return $r;
// }



// public static function help_sendmessage(){

// 	$r["description"] = "메세지보내기";

// 	$r["param"][] = array("name"=>"receiverMemberID","type"=>"string or int","description"=>"받는사람아이디");
// 	$r["param"][] = array("name"=>"senderMemberID","type"=>"string or int","description"=>"보내는사람아이디");
// 	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"내용");
// 	$r["param"][] = array("name"=>"type","type"=>"int","description"=>"메세지타입");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
	
// 	return $r;
// }


// public static function sendmessage($p){	
	
// 	//보내기
// 	$message = new Message();
// 	$message->m_memberID=$p["receiverMemberID"];
// 	$message->m_content=$p["content"];
// 	$message->m_regDate=TimeManager::getCurrentDateTime();
// 	$message->m_regTime=TimeManager::getTime();
// 	$message->m_friendID=$p["senderMemberID"];
// 	$message->m_type=$p["type"];
// 	$message->m_isSendMsg=0;
// 	$r["send"]=$message->send();
	
	
// 	//보낸편지로 등록
// 	// $message2 = new Message();
// 	// $message2->m_memberID=$p["senderMemberID"];
// 	// $message2->m_content=$p["content"];
// 	// $message2->m_regDate=TimeManager::getCurrentDateTime();
// 	// $message2->m_friendID=$p["receiverMemberID"];
// 	// $message2->m_type=$p["type"];
// 	// $message2->m_isSendMsg=1;
// 	// $r["receive"]=$message2->send();

// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
// 	return $r;
// }

// public static function help_sendmessagebylist(){

// 	$r["description"] = "메세지 여러사람에게 보내기";

// 	$r["param"][] = array("name"=>"receiverMemberIDList","type"=>"array(string or int)","description"=>"받는사람아이디목록");
// 	$r["param"][] = array("name"=>"senderMemberID","type"=>"string or int","description"=>"보내는사람아이디");
// 	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"내용");
// 	$r["param"][] = array("name"=>"type","type"=>"int","description"=>"메세지타입");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
// 	$r["result"][]=ResultState::toArray(2002,"receiverMemberIDList");
	
// 	return $r;
// }

// public static function sendmessagebylist($p){	
// 	$memberidlist = $p["receiverMemberIDList"];
// 	if(!is_array($memberidlist)){
// 		$r["state"]="error";
// 		$r["result"]=ResultState::toArray(2002,"receiverMemberIDList");
// 		return $r;
// 	}
	
// 	foreach($memberidlist as $rmemberid){
// 		//보내기
// 		$message = new Message();
// 		$message->m_memberID=$rmemberid;
// 		$message->m_content=$p["content"];
// 		$message->m_regDate=TimeManager::getCurrentDateTime();
// 		$message->m_regTime=TimeManager::getTime();
// 		$message->m_friendID=$p["senderMemberID"];
// 		$message->m_type=$p["type"];
// 		$message->m_isSendMsg=0;
// 		$r["send"][]=$message->send();
		
// 		//보낸편지로 등록
// 		// $message2 = new Message();
// 		// $message2->m_memberID=$p["senderMemberID"];
// 		// $message2->m_content=$p["content"];
// 		// $message2->m_regDate=TimeManager::getCurrentDateTime();
// 		// $message2->m_friendID=$rmemberid;
// 		// $message2->m_type=$p["type"];
// 		// $message2->m_isSendMsg=1;
// 		// $r["receive"][]=$message2->send();
// 	}

// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
	
// 	return $r;
// }

// public static function help_getmessagelist(){

// 	$r["description"] = "메세지목록 받기";

// 	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
// 	$r["param"][] = array("name"=>"type","type"=>"int","description"=>"메세지타입, (0혹은 미입력시 모두받아옴)");
// 	$r["param"][] = array("name"=>"keyList","type"=>"array","description"=>"data필드내에 받아올 필드목록(미입력시 전체)");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
// 	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
// 	return $r;
// }


// public static function getmessagelist($p){
// 	$whereType = "";
// 	$keyList=NULL;

// 	if($p["keyList"])$keyList=$p["keyList"];
// 	if(!$p["memberID"]){
// 		$r["state"]="error";
// 		$r["result"]=ResultState::toArray(2002,"memberID");
// 		return $r;
// 	}

// 	$userIndex = UserIndex::create($p["memberID"]);
// 	$shardConn = $userIndex->getShardConnection();	

// 	if(!$shardConn)return ResultState::makeReturn(1002,"fail to get shardConnection");
	
// 	if($p["type"])$whereType=" and type='".$p["type"]."'";
// 	$result = mysql_query("select * from MessageTable where memberID=".$p["memberID"].$whereType." and isSendMsg=0 order by no desc",$shardConn);
	
// 	$mlist=array();
// 	while($message = mysql_fetch_array($result,MYSQL_ASSOC)){

// 		if($message["data"] && $keyList){
// 			$userdata =  json_decode($message["data"],true);

// 			foreach($userdata as $key=>$value){
// 				if($keyList && !in_array($name,$keyList)){
// 					unset($message[$key]);
// 				}
// 			}

// 			$message["data"]=json_encode($userdata,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
// 		}	

// 		$mlist[]=$message;
// 	}
// 	$r["list"]=$mlist;
// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
// 	return $r;
// }

// public static function help_removemessage(){

// 	$r["description"] = "메세지삭제";

// 	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
// 	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"메세지번호");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
// 	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
// 	return $r;
// }


// public static function removemessage($p){
// 	//메세지삭제	
	
// 	$message = new Message($p["memberID"],$p["no"]);
// 	if($message->isLoaded()){
// 		$message->remove();
// 		$r["state"]="ok";
// 		$r["result"]=ResultState::successToArray();
// 	}else{
// 		$r["state"]="ok";
// 		$r["result"]=ResultState::successToArray();
// 	}
// 	return $r;
// }

// public static function help_removemessagebylist(){

// 	$r["description"] = "메세지 여러개 삭제";

// 	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
// 	$r["param"][] = array("name"=>"noList","type"=>"array(int)","description"=>"메세지번호목록");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
// 	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
// 	return $r;
// }


// public static function removemessagebylist($p){
// 	//메세지삭제	
// 	if(!is_array($p["noList"])){
// 		$r["state"]="error";
// 		$r["result"]=ResultState::toArray(2002,"noList");
// 		return $r;
// 	}
	
// 	$userIndex = UserIndex::create($p["memberID"]);
// 	$shardConn = $userIndex->getShardConnection();	
// 	if(!$shardConn)return ResultState::makeReturn(1002,"fail to get shardConnection");
	
// 	$messageNos = implode(",", $p["noList"]);
// 	$whereNo = " and no IN (".$messageNos.")";
// 	$result = mysql_query("delete from MessageTable where memberID=".$p["memberID"].$whereNo." and isSendMsg=0",$shardConn);
	
// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
// 	return $r;
// }

// public static function help_removeallmessage(){

// 	$r["description"] = "메세지 전부삭제";

// 	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
// 	$r["param"][] = array("name"=>"type","type"=>"int","description"=>"메세지타입(0혹은 입력안할시 모두삭제)");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
// 	$r["result"][]=ResultState::toArray(1002,"fail to get shardConnection");
	
// 	return $r;
// }

// public static function removeallmessage($p){
// 	if($p["type"] && $p["type"]>0)$whereType=" and type='".$p["type"]."'";
	
// 	$userIndex = UserIndex::create($p["memberID"]);
// 	$shardConn = $userIndex->getShardConnection();	
// 	if(!$shardConn)return ResultState::makeReturn(1002,"fail to get shardConnection");
	
// 	$result = mysql_query("delete from MessageTable where memberID=".$p["memberID"].$whereType." and isSendMsg=0",$shardConn);
	
// 	if($result){
// 		$r["state"]="ok";
// 		$r["result"]=ResultState::successToArray();
// 	}else{
// 		$r["error"]="error";
// 		$r["result"]=ResultState::toArray(1001);
// 	} 
		
// 	return $r;
// }
// /////////////////////////////////////////

// public static function help_setweeklyscore(){

// 	$r["description"] = "주간점수등록";

// 	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
// 	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"점수");
// 	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
	
// 	return $r;
// }

// public static function setweeklyscore($p){

// 	$ws =new WeeklyScore($p["memberID"],TimeManager::getCurrentWeekNo());
// 	$ws->memberID = $p["memberID"];
// 	$ws->data = $p["data"];
// 	$ws->regDate = TimeManager::getCurrentDateTime();
// 	$ws->regTime = TimeManager::getTime();
// 	$ws->regWeek = TimeManager::getCurrentWeekNo();
	
// 	$r["update"]=false;
// 	if($ws->score<$p["score"]){
// 		$r["update"]=true;
// 		$ws->score=$p["score"];
// 		$r["uresult"]=$ws->save();
// 	}else{
// 		$r["uresult"]=$ws->save();
// 	}
// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
// 	return $r;
// }

// public static function help_getweeklyscorelist(){

// 	$r["description"] = "주간점수목록";

// 	$r["param"][] = array("name"=>"memberIDList","type"=>"array(string or int)","description"=>"멤버아이디목록");
// 	$r["param"][] = array("name"=>"weekNo","type"=>"int","description"=>"주간번호 없을경우 이번주");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
	
// 	return $r;
// }

// public static function getweeklyscorelist($p){
// 	$memberlist = $p["memberIDList"];
// 	$weekNo = TimeManager::getCurrentWeekNo();
// 	if($p["weekNo"])$weekNo=$p["weekNo"];
	
// 	$list=array();
// 	foreach($memberlist as $key=>$value){
// 		$ws=new WeeklyScore($value,$weekNo);
// 		if($ws->isLoaded()){
// 			$data = $ws->getArrayData(true);
// 			$list[]=$data;
// 		}
// 	}
// 	$r["remainTime"]=strtotime("next Sunday")-time();
// 	$r["list"]=$list;
// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
// 	return $r;
// }
public static function help_setstagescore(){

	$r["description"] = "스테이지점수등록";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","description"=>"스테이지번호");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"nick");
	$r["param"][] = array("name"=>"flag","type"=>"string","description"=>"flag");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"점수");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

// public static function help_addstagescore(){

// 	$r["description"] = "스테이지점수추가";

// 	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"멤버아이디");
// 	$r["param"][] = array("name"=>"stageNo","type"=>"int","description"=>"스테이지번호");
// 	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
// 	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"점수");
	
// 	$r["result"][]=ResultState::toArray(1,"success");
	
// 	return $r;
// }

// public static function addstagescore($p){
// 	$stageNo = $p["stageNo"];
// 	$ss = new StageScore($stageNo,$p["memberID"]);
// 	$ss->memberID = $p["memberID"];
// 	$ss->stageNo = $stageNo;
// 	$ss->data = $p["data"];
// 	$ss->regDate = TimeManager::getCurrentDateTime();
// 	$ss->regTime = TimeManager::getTime();
// 	$ss->score+=$p["score"];	
// 	$ss->save();
// 	$r["state"]="ok";
// 	$r["result"]=ResultState::successToArray();
// 	return $r;
// }


public static function help_getstagerankbyalluser(){

	$r["description"] = "스테이지랭킹";

	$r["param"][] = array("name"=>"memberID","type"=>"int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"stageNo","type"=>"int","description"=>"스테이지번호");
	$r["param"][] = array("name"=>"myScore","type"=>"int","description"=>"0을 입력하면 기존 내점수, 최고점수보다 높으면 기록갱신");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"파라메터없음");
	
	return $r;
}
public static function help_getpuzzleinfo(){

	$r["description"] = "퍼즐 정보를 가져옵니다.";
	
	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"퍼즐정보버전");
	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"퍼즐 번호");
	$r["param"][] = array("name"=>"order","type"=>"int","description"=>"퍼즐 순서 (퍼즐번호 없을시 입력)");
	
	//$r["return"]
	
	$r["result"][]=ResultState::toArray(1001,"디비query실패");
	$r["result"][]=ResultState::toArray(1002,"메인디비접속실패");
	$r["result"][]=ResultState::toArray(1003,"디비fetch실패");
	$r["result"][]=ResultState::toArray(2001,"버전이 같을경우 아무런 값도 리턴하지 않습니다.");
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}

public static function help_updatepuzzlehistory(){

	$r["description"] = "퍼즐히스토리를 남깁니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"puzzleNo","type"=>"int","description"=>"퍼즐번호(필수)");
	$r["param"][] = array("name"=>"updateOpenDate","type"=>"bool","description"=>"true 일 경우 오픈시간을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"updateClearDate","type"=>"bool","description"=>"true 일 경우 클리어시각을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"updatePerfectDate","type"=>"bool","description"=>"true 일 경우 퍼펙트클리어시각을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"openType","type"=>"string","description"=>"어떻게 오픈했는가(ex 무료, 이벤트, 구입-100젬)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or puzzleNo");
	
	return $r;
}

public static function help_getpuzzlehistory(){

	$r["description"] = "퍼즐히스토리를 가져옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	
	return $r;
}

public static function help_updatepiecehistory(){

	$r["description"] = "피스히스토리를 남깁니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"pieceNo","type"=>"int","description"=>"피스번호(필수)");
	$r["param"][] = array("name"=>"openDate","type"=>"bool or int","description"=>"0초과 혹은 true일경우 현재시각으로 오픈일자변경");
	$r["param"][] = array("name"=>"firstClearDate","type"=>"bool","description"=>"0초과 혹은 true일경우 현재시각으로 클리어일자변경");
	$r["param"][] = array("name"=>"tryCount","type"=>"int","description"=>"총 시도횟수변경");
	$r["param"][] = array("name"=>"clearCount","type"=>"int","description"=>"성공했을때까지의 시도횟수변경");
	$r["param"][] = array("name"=>"clearDateList","type"=>"array(1등급bool,2등급bool,3등급bool,4등급bool)","description"=>"등급클리어정보업데이트");
	$r["param"][] = array("name"=>"openType","type"=>"string","description"=>"어떻게 오픈했는가(ex 이전피스클리어 or 5젬구입");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or pieceNo");
	
	return $r;
}

public static function help_getpiecehistory(){

	$r["description"] = "피스히스토리를 가져옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	
	return $r;
}


public static function help_updatecardhistory(){

	$r["description"] = "카드히스토리를 남깁니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"cardNo","type"=>"int","description"=>"카드번호(필수)");
	$r["param"][] = array("name"=>"comment","type"=>"string","description"=>"코멘트");
	$r["param"][] = array("name"=>"updateTakeDate","type"=>"bool","description"=>"true 일 경우 획득시각을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"isMorphing","type"=>"bool","description"=>"모핑가능함?");
	$r["param"][] = array("name"=>"addCount","type"=>"bool","description"=>"더할갯수");
	$r["param"][] = array("name"=>"level","type"=>"bool","description"=>"레벨");
	$r["param"][] = array("name"=>"includeCardInfo","type"=>"bool","description"=>"true 일 경우 해당카드의 정보를 돌려줍니다.");
	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or cardNo");
	$r["result"][]=ResultState::toArray(2014,"Dont save");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFIND,"Dont find");
	
	return $r;
}

public static function help_getcardhistory(){

	$r["description"] = "카드히스토리를 가져옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	
	return $r;
}


public static function help_addproperty(){

	$r["description"] = "보관함에 아이템/재화를 추가합니다. (서버관리자용입니다.)";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"count","type"=>"int","description"=>"변경량");
	$r["param"][] = array("name"=>"type","type"=>"string","description"=>"property type");	
	$r["param"][] = array("name"=>"statsID","type"=>"string","description"=>"statsID");	
	$r["param"][] = array("name"=>"statsValue","type"=>"string","description"=>"statsValue");	
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"변경내용");	
	$r["param"][] = array("name"=>"sender","type"=>"string","description"=>"user or 사번");	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}


public static function help_updateuserpropertyhistory(){

	$r["description"] = "보관함에 아이템/재화를 추가합니다. (서버관리자용입니다.)";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"count","type"=>"int","description"=>"변경량");
	$r["param"][] = array("name"=>"type","type"=>"string","description"=>"property type");	
	$r["param"][] = array("name"=>"statsID","type"=>"string","description"=>"statsID");	
	$r["param"][] = array("name"=>"statsValue","type"=>"string","description"=>"statsValue");	
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"변경내용");	
	$r["param"][] = array("name"=>"sender","type"=>"string","description"=>"user or 사번");	
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}


public static function help_changeuserproperties(){

	$r["description"] = "보관함에 아이템/재화를 변경합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"list","type"=>"array({type,count,statsID,statsValue,content,isPurchase})","description"=>"변경할 소유물 목록");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	$r["result"][]=ResultState::toArray(2015,"property is minus");
	
	$r["comment"] = '
*type
m(실제돈)
g (골드)
r (젬)
h (하트)
i6 (스피드업)
i8 (아이템두배)
i9 (타임)
p1 (이어하기 이용권)
p2 (맵가챠 이용권)
p3 (업그레이드 이용권)
p4 (아이템뽑기 이용권)
p5 (99프로 이용권)
p6 생명의돌
cd 카드획득
cdm 카드소멸
pc 피스획득
pz 퍼즐획득
cu 캐릭업글
cp 캐릭터획득

*statsID / statsValue
stage / 스테이지번호
event / 이벤트번호
archivement / 업적번호
shop / none

	';

	return $r;
}

public static function help_getuserproperties(){
	$r["description"] = "유저 보유 재화/아이템 목록을 불러옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}

public static function help_starttransaction(){

	$r["description"] = "함께보낸 명령들에 대해 트랜잭션을 시작하고 결과를 리턴합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2013,"트랜잭션실패");
	
	return $r;
}

public static function help_transaction(){

	$r["description"] = "트랜잭션을 추가합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"actions","type"=>"array(dict(action(string),param(dict)))","description"=>'트랜잭션내 실행할 action // ex) [{"action":"updateGoldHistory","param":{"memberID":12,"changeCount":1000}},{"action":"addGold","param":{"memberID":12,"count":1000}}]');
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2013,"트랜잭션실패");
	
	return $r;
}

public static function help_getarchivementlist(){
	$r["description"] = "전체 업적 목록을 불러옵니다.";

	$r["param"][] = array("name"=>"version","type"=>"int","description"=>"version");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2001,"version");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}

public static function help_getarchivementhistory(){
	$r["description"] = "유저가 진행중인 업적목록을 불러옵니다. ";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}



public static function help_updatearchivementhistory(){

	$r["description"] = "업적을 업데이트합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"archiveID","type"=>"char","description"=>"업적ID(필수)");
	$r["param"][] = array("name"=>"updateClearDate","type"=>"bool","description"=>"true 일 경우 클리어시간을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"updateRewardDate","type"=>"bool","description"=>"true 일 경우 보상시각을 현재시각으로 설정합니다.");
	$r["param"][] = array("name"=>"count","type"=>"int","description"=>"현재수치");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or puzzleNo");
	
	return $r;
}


public static function help_getgiftboxhistory(){
	$r["description"] = "선물함 목록을 가져옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"includeConfirm","type"=>"bool","description"=>"true일경우 확인메세지까지 불러옵니다.");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	
	return $r;
}



public static function help_updategiftboxhistory(){

	$r["description"] = "선물함 메세지정보를 업데이트합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"giftBoxNo","type"=>"int","description"=>"기프트박스메세지번호");
	$r["param"][] = array("name"=>"updateConfirmDate","type"=>"bool","description"=>"true일경우 확인시각을 현재시각으로 업데이트합니다.");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or giftBoxNo");
	$r["result"][]=ResultState::toArray(2003,"dont find message");
	
	return $r;
}




public static function help_sendgiftboxhistory(){
	$r["description"] = "선물메세지를 보냅니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"sender","type"=>"string or int","description"=>"보내는사람(없으면 user)");
	$r["param"][] = array("name"=>"content","type"=>"string","description"=>"내용");
	$r["param"][] = array("name"=>"reward","type"=>"array(dict(key,value))","description"=>"보상");
	$r["param"][] = array("name"=>"data","type"=>"string","description"=>"데이터");
	$r["param"][] = array("name"=>"exchangeID","type"=>"string","description"=>"exchangeID");
	$r["param"][] = array("name"=>"exchangeList","type"=>"string","description"=>"exchangeList (exchnage custom)");
	$r["param"][] = array("name"=>"template","type"=>"string","description"=>"템플릿으로 사용할 commonsetting key");
	$r["param"][] = array("name"=>"custom","type"=>"array(string)","description"=>"템플릿 커스텀");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}


public static function help_confirmgiftboxhistory(){
	$r["description"] = "선물을 받습니다. 자동으로 선물이 지급됩니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");
	$r["param"][] = array("name"=>"giftBoxNo","type"=>"int","description"=>"선물메세지번호");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID,giftBoxNo");
	$r["result"][]=ResultState::toArray(2003,"dont find message");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}


public static function help_confirmallgiftboxhistory(){
	$r["description"] = "모든 선물을 받습니다. 자동으로 선물이 지급됩니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID,giftBoxNo");
	$r["result"][]=ResultState::toArray(2003,"dont find message");
	$r["result"][]=ResultState::toArray(2014,"dont save");
	
	return $r;
}


public static function help_checkgiftboxhistory(){
	$r["description"] = "선물이 있는지 체크합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	
	return $r;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

public static function help_getcharacterhistory(){
	$r["description"] = "내캐릭터목록을 들고옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or count");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	
	return $r;
}



public static function help_updatecharacterhistory(){

	$r["description"] = "내캐릭터정보를 업데이트합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"characterNo","type"=>"int","description"=>"캐릭터번호");
	//$r["param"][] = array("name"=>"level","type"=>"int","description"=>"레벨");
	//$r["param"][] = array("name"=>"levelup","type"=>"bool","description"=>"true 일경우 level을 1 올림");
	$r["param"][] = array("name"=>"exp","type"=>"int","description"=>"경험치를 더하고, 레벨업이 됬을경우 레벨업을 시킴");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(2002,"memberID or characterNo");
	$r["result"][]=ResultState::toArray(2003,"dont find message");
	
	return $r;
}


public static function help_levelup(){

	$r["description"] = "레벨업을 합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"string or int","description"=>"유저아이디(필수)");
	$r["param"][] = array("name"=>"usePass","type"=>"bool","description"=>"true일경우 이용권소모 false일경우 골드소모");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDALREADY,"더이상 레벨업할수없음(maxlevel)");
	$r["result"][]=ResultState::toArray(2002,"memberID or characterNo");
	
	return $r;
}

public static function help_gettimeinfo(){

	$r["description"] = "시간정보를 받습니다.";

	$r["param"][] = array("name"=>"offset","type"=>"int","description"=>"이 값(초단위)만큼 현시간에서 더하여 계산합니다.");
	
	$r["result"][]=ResultState::toArray(1,"success");
	
	return $r;
}




public static function help_gettodaymission(){

	$r["description"] = "오늘의 미션 정보를 받습니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");

	
	return $r;
}

public static function help_updatetodaymission(){

	$r["description"] = "오늘의 미션을 업데이트하고 미션정보를 반환합니다. 미션을 수행완료하였을경우 보상을지급하고 isSuccess=true 를 반환합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"date","type"=>"int","description"=>"미션수행날짜");
	$r["param"][] = array("name"=>"count","type"=>"int","description"=>"수행수치에 더할 수치");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(2003,"날짜가 지남");

	
	return $r;
}



public static function help_exchange(){

	$r["description"] = "교환매니저를 통해 재화를 조절합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"exchangeID","type"=>"string","description"=>"exchagneID");
	$r["param"][] = array("name"=>"list","type"=>"array({type,count,statsID,statsValue,content,isPurchase})","description"=>"수정/추가 할 목록 (필수아님)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID,exchangeID");
	$r["result"][]=ResultState::toArray(2003,"dont find");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2015,"property is minus");

	
	return $r;
}



public static function help_exchangebylist(){

	$r["description"] = "교환매니저를 통해 재화를 조절합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"exchangeIDList","type"=>"array(string)","description"=>"exchagneID");
	$r["param"][] = array("name"=>"list","type"=>"array({type,count,statsID,statsValue,content,isPurchase})","description"=>"수정/추가 할 목록 (필수아님)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID,exchangeID");
	$r["result"][]=ResultState::toArray(2003,"dont find");
	$r["result"][]=ResultState::toArray(2005,"dont find user");
	$r["result"][]=ResultState::toArray(2015,"property is minus");

	
	return $r;
}



public static function help_checkloginevent(){

	$r["description"] = "로그인이벤트를 체크합니다. 보상이 있을경우 메세지를 날립니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");

	
	return $r;
}


public static function help_getendlessrank(){

	$r["description"] = "무한모드랭킹을 불러온다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"weekNo","type"=>"int","description"=>"불러올 주간번호(없으면 현재주)");
	$r["param"][] = array("name"=>"start","type"=>"string","description"=>"몇등부터?");
	$r["param"][] = array("name"=>"limit","type"=>"int","description"=>"몇명까지?");
	
	$r["result"][]=ResultState::toArray(1,"success");

	
	return $r;
}



public static function help_getendlessrankinfo(){

	$r["description"] = "한 유저의 무한모드랭킹정보를 불러온다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"weekNo","type"=>"int","description"=>"불러올 주간번호(없으면 현재주)");
	
	$r["result"][]=ResultState::toArray(1,"success");

	
	return $r;
}


public static function help_setendlessrank(){

	$r["description"] = "무한모드랭킹을 저장합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"score");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"nick");
	$r["param"][] = array("name"=>"level","type"=>"int","description"=>"level");
	$r["param"][] = array("name"=>"flag","type"=>"string","description"=>"flag");
	$r["param"][] = array("name"=>"victory","type"=>"int","description"=>"victory");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");

	
	return $r;
}


public static function help_finishendlessplay(){

	$r["description"] = "무한모드랭킹 저장 및 연승을 세이브합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"방금 한판의 score");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"nick");
	$r["param"][] = array("name"=>"flag","type"=>"string","description"=>"flag");
	$r["param"][] = array("name"=>"victory","type"=>"bool","description"=>"승리시 true, 패배시 false");
	$r["param"][] = array("name"=>"data","type"=>"array or string","description"=>"data");

	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");

	
	return $r;
}

public static function help_getendlessplaydata(){

	$r["description"] = "무한모드상대와 스테이지정보를 불러옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"win","type"=>"int","description"=>"현재연승수");
	$r["param"][] = array("name"=>"no","type"=>"int","description"=>"지정번호(필수아님)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID,exchangeID");

	return $r;
}


public static function help_startendlessplay(){

	$r["description"] = "무한모드 시작상태로 내상태를 변경합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"autoLevel","type"=>"int","description"=>"autoLevel(userdata의 autoLevel)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTSAVE,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");

	return $r;
}

public static function help_getendlessplayriver(){

	$r["description"] = "무한모드상대와 스테이지정보를 불러옵니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"win","type"=>"int","description"=>"win(userdata의 ing_win)");
	$r["param"][] = array("name"=>"highPiece","type"=>"int","description"=>"최고진입스테이지(userdata의 highPiece)");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTSAVE,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");

	return $r;
}


public static function help_saveendlessplaydata(){

	$r["description"] = "무한모드데이터를 등록합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"score","type"=>"int","description"=>"score");
	$r["param"][] = array("name"=>"nick","type"=>"string","description"=>"nick");
	$r["param"][] = array("name"=>"level","type"=>"int","description"=>"level");
	$r["param"][] = array("name"=>"autoLevel","type"=>"int","description"=>"autoLevel");
	$r["param"][] = array("name"=>"flag","type"=>"string","description"=>"flag");
	$r["param"][] = array("name"=>"victory","type"=>"int","description"=>"victory");
	$r["param"][] = array("name"=>"playData","type"=>"text","description"=>"playData");
	$r["param"][] = array("name"=>"pieceNo","type"=>"text","description"=>"pieceNo");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID,exchangeID");

	return $r;
}

public static function help_checkattendenceevent(){

	$r["description"] = "출석체크를 합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");

	return $r;
}

//쿠폰관리


public static function help_usecupon(){

	$r["description"] = "무한모드데이터를 등록합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"cuponCode","type"=>"string","description"=>"cuponCode");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(ResultState::GDALREADY,"이미사용함");
	$r["result"][]=ResultState::toArray(ResultState::GDEXPIRE,"기간지남");
	$r["result"][]=ResultState::toArray(ResultState::GDOSERROR,"os안맞음");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFIND,"찾을수없음");

	return $r;
}

public static function help_gettimeevent(){

	$r["description"] = "시간이벤트를 불러옵니다.";

	//$r["param"][] = array("name"=>"language","type"=>"text","description"=>"language");
	//$r["param"][] = array("name"=>"os","type"=>"text","description"=>"os");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID,exchangeID");

	return $r;
}


public static function help_checkmissionevent(){

	$r["description"] = "미션이벤트를 체크합니다.";

	//$r["param"][] = array("name"=>"language","type"=>"text","description"=>"language");
	//$r["param"][] = array("name"=>"os","type"=>"text","description"=>"os");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTSAVE,"저장오류");

	return $r;
}

public static function help_getheart(){

	$r["description"] = "현재생명갯수 정보를 가져옵니다";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"memberID");
	$r["param"][] = array("name"=>"use","type"=>"bool","description"=>"한개 깍을건지 여부");
	//$r["param"][] = array("name"=>"os","type"=>"text","description"=>"os");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFIND,"fail to load commonsetting");
	$r["result"][]=ResultState::toArray(ResultState::GDEXPIRE,"하트모자람");

	return $r;
}

public static function help_sendcard(){

	$r["description"] = "카드를 선물합니다.";

	$r["param"][] = array("name"=>"memberID","type"=>"int or string","description"=>"my memberID");
	$r["param"][] = array("name"=>"toMemberID","type"=>"int or string","description"=>"상대방 memberID");
	$r["param"][] = array("name"=>"cardNo","type"=>"int or string","description"=>"선물할 카드번호");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFIND,"정보를 찾을수없음");
	$r["result"][]=ResultState::toArray(ResultState::GDEXPIRE,"2장이상 있어야 카드 선물가능");
	$r["result"][]=ResultState::toArray(ResultState::GDALREADY,"시간제한에 걸림 hourLimit(한번선물가능시간(시간단위)),leftTime(남은시간(초단위)) 같이 리턴됨.");

	return $r;
}

public static function help_getgachalist(){

	$r["description"] = "가차목록을 불러옵니다.";

	$r["param"][] = array("name"=>"isPremium","type"=>"bool","description"=>"true일경우 비싼목록");
	
	$r["result"][]=ResultState::toArray(1,"success");
	$r["result"][]=ResultState::toArray(1001,"저장오류");
	$r["result"][]=ResultState::toArray(2002,"memberID");
	$r["result"][]=ResultState::toArray(ResultState::GDDONTFIND,"정보를 찾을수없음");
	$r["result"][]=ResultState::toArray(ResultState::GDEXPIRE,"2장이상 있어야 카드 선물가능");
	
	return $r;
}
























}
?>