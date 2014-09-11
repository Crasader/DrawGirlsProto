#include "MyLocalization.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <Foundation/Foundation.h>
#endif


void MyLocal::initLocal()
{	
	ko();
	en();
	ja();
}

CCString* MyLocal::getLocalCode()
{
	string tempCode;
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
	NSArray *languages = [defaults objectForKey:@"AppleLanguages"];
	NSString *currentLanguage = [languages objectAtIndex:0];
	
	// get the current language code.(such as English is "en", Chinese is "zh" and so on)
	NSDictionary* temp = [NSLocale componentsFromLocaleIdentifier:currentLanguage];
	NSString * languageCode = [temp objectForKey:NSLocaleLanguageCode];
	
	tempCode = [languageCode cStringUsingEncoding:NSASCIIStringEncoding];
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	tempCode = getCurrentLanguageJNI();
#endif

	return CCString::create("ko");
}

const char* MyLocal::getSupportLocalCode()
{
	languageType = getLocalCode();
	CCArray* allkeys_ = this->allKeys();
	bool supportable = false;
	for(int i=0;i<allkeys_->count();i++)
	{
		CCString* key = (CCString*)allkeys_->objectAtIndex(i);
		if(languageType->isEqual(key))
		{
			supportable = true;
			break;
		}
	}
	if(!supportable)
	{
		languageType = CCString::create("en");
	}
	
	return languageType->getCString();
}

const char* MyLocal::getLocalForKey( MyLocalKey key )
{
	languageType = getLocalCode();
	CCArray* allkeys_ = this->allKeys();
	bool supportable = false;
	for(int i=0;i<allkeys_->count();i++)
	{
		CCString* key = (CCString*)allkeys_->objectAtIndex(i);
		if(languageType->isEqual(key))
		{
			supportable = true;
			break;
		}
	}
	if(!supportable)
	{
		languageType = CCString::create("en");
	}
	CCDictionary* localDic = (CCDictionary*)this->objectForKey(languageType->getCString());
	CCString* returnLocal = (CCString*)localDic->objectForKey(key);

	if(returnLocal == NULL)
		returnLocal = (CCString*)((CCDictionary*)objectForKey("en"))->objectForKey(key);
	if(returnLocal == NULL)
		returnLocal = CCString::create("");
	
	return returnLocal->getCString();
}

MyLocal* MyLocal::sharedInstance()
{
	static MyLocal* myLocal = NULL;
	if(myLocal == NULL)
	{
		myLocal = new MyLocal();
	}
	return myLocal;
}

void MyLocal::setLocal()
{
	languageType = getLocalCode();
}
void MyLocal::ko()
{
	
	CCDictionary* ko = CCDictionary::create();
	ko->setObject(CCString::create("서버에 접속 중입니다..."), kMyLocalKey_connectingServer);
//	ko->setObject(CCString::create("닉네임 입력"), kMyLocalKey_inputNick);
	ko->setObject(CCString::create(" 닉네임을 입력해주세요."), kMyLocalKey_inputPlease);
	ko->setObject(CCString::create("확인"), kMyLocalKey_ok);
	ko->setObject(CCString::create("이미지 정보를 받아옵니다."), kMyLocalKey_downImgInfo);
	ko->setObject(CCString::create("정보 불러오기 실패"), kMyLocalKey_failLoadInfo);
	ko->setObject(CCString::create("재시도"), kMyLocalKey_replay);
//	ko->setObject(CCString::create("다운로드중 %.0f%% : %d/%d"), kMyLocalKey_downloadingProgress);
	ko->setObject(CCString::create("이미지 받아오기 실패"), kMyLocalKey_downImgFail);
//	ko->setObject(CCString::create("로그인 성공"), kMyLocalKey_successLogin);
	ko->setObject(CCString::create("사용할 수 없는 닉네임 입니다."), kMyLocalKey_sameNick);
	ko->setObject(CCString::create("사용할 수 없는 닉네임 입니다."), kMyLocalKey_invalidNick);
	ko->setObject(CCString::create("닉네임이 너무 깁니다."), kMyLocalKey_longNick);
	ko->setObject(CCString::create("닉네임이 너무 짧습니다."), kMyLocalKey_shortNick);
//	ko->setObject(CCString::create("특수문자는 사용 할 수 없습니다."), kMyLocalKey_specialCharacterError);
	ko->setObject(CCString::create("지금 열기"), kMyLocalKey_openRightNow);
	ko->setObject(CCString::create("RUBY %d 로 오픈"), kMyLocalKey_openRubyValue);
//	ko->setObject(CCString::create("GOLD %d 로 오픈"), kMyLocalKey_openGoldValue);
	ko->setObject(CCString::create("CGP 일반 보상"), kMyLocalKey_cgpNormalTitle);
	ko->setObject(CCString::create("보상받기를 눌러주세요."), kMyLocalKey_cgpNormalContent);
	ko->setObject(CCString::create("보상받기"), kMyLocalKey_getReward);
	ko->setObject(CCString::create("받기"), kMyLocalKey_take);
	ko->setObject(CCString::create("CGP 전체 팝업 보상"), kMyLocalKey_cgpAllPopupTitle);
	ko->setObject(CCString::create("상점"), kMyLocalKey_shop);
	ko->setObject(CCString::create("랭킹"), kMyLocalKey_ranking);
	ko->setObject(CCString::create("내카드"), kMyLocalKey_mycard);
	ko->setObject(CCString::create("오늘의 미션"), kMyLocalKey_todaymission);
	ko->setObject(CCString::create("이벤트"), kMyLocalKey_event);
	ko->setObject(CCString::create("스페셜 스테이지"), kMyLocalKey_openStage);
	ko->setObject(CCString::create("스페셜 스테이지를 오픈하시겠습니까?"), kMyLocalKey_openStageContent);
	ko->setObject(CCString::create("오픈하기"), kMyLocalKey_doOpen);
//	ko->setObject(CCString::create("스테이지 오픈"), kMyLocalKey_stageOpenTitle);
//	ko->setObject(CCString::create("새로운 스테이지가\n오픈 되었습니다."), kMyLocalKey_stageOpenContent);
//	ko->setObject(CCString::create("퍼즐 오픈"), kMyLocalKey_puzzleOpenTitle);
//	ko->setObject(CCString::create("새로운 퍼즐이\n오픈 되었습니다."), kMyLocalKey_puzzleOpenContent);
	ko->setObject(CCString::create("보기"), kMyLocalKey_view);
	ko->setObject(CCString::create("영역 85%"), kMyLocalKey_condition1);
	ko->setObject(CCString::create("영역 100%"), kMyLocalKey_condition2);
	ko->setObject(CCString::create("체인지 + 영역 85%"), kMyLocalKey_condition3);
	ko->setObject(CCString::create("체인지 + 영역 100%"), kMyLocalKey_condition4);
//	ko->setObject(CCString::create("85%"), kMyLocalKey_conditionTwoLine1);
//	ko->setObject(CCString::create("85%\n+ 체인지"), kMyLocalKey_conditionTwoLine2);
//	ko->setObject(CCString::create("100%"), kMyLocalKey_conditionTwoLine3);
//	ko->setObject(CCString::create("100%\n+ 체인지"), kMyLocalKey_conditionTwoLine4);
	ko->setObject(CCString::create("%d 스테이지"), kMyLocalKey_stageValue);
//	ko->setObject(CCString::create("나의 위치 %d"), kMyLocalKey_myrankValue);
	ko->setObject(CCString::create("랭킹 정보 확인 실패"), kMyLocalKey_failCheckRanking);
	ko->setObject(CCString::create("스테이지"), kMyLocalKey_stage);
	ko->setObject(CCString::create("게임에 사용할 아이템을 선택하세요."), kMyLocalKey_selectUseItem);
	ko->setObject(CCString::create("레벨 %d"), kMyLocalKey_levelValue);
	ko->setObject(CCString::create("파워 %s"), kMyLocalKey_powerValue);
	ko->setObject(CCString::create("레벨 %d 업그레이드"), kMyLocalKey_upgradeLevelValue);
	ko->setObject(CCString::create("레벨 %d\n업그레이드"), kMyLocalKey_upgradeLevelValue2);
	ko->setObject(CCString::create("아이템은 최대 3개까지\n선택이 가능합니다."), kMyLocalKey_totalItemSelectCount3);
	ko->setObject(CCString::create("골드가 부족합니다."), kMyLocalKey_goldNotEnought);
	ko->setObject(CCString::create("젬이 부족합니다."), kMyLocalKey_rubyNotEnought);
	ko->setObject(CCString::create("하트가 부족합니다."), kMyLocalKey_heartNotEnought);
//	ko->setObject(CCString::create("구매하러 가시겠습니까?"), kMyLocalKey_goToShop);
	ko->setObject(CCString::create("기본속도향상"), kMyLocalKey_baseSpeedUpTitle);
	ko->setObject(CCString::create("유저 속도가 빠른상태로 시작합니다."), kMyLocalKey_baseSpeedUpContent);
	ko->setObject(CCString::create("더블아이템"), kMyLocalKey_doubleItemTitle);
	ko->setObject(CCString::create("아이템을 빠르게 생성합니다."), kMyLocalKey_doubleItemContent);
	ko->setObject(CCString::create("시간추가"), kMyLocalKey_longTimeTitle);
	ko->setObject(CCString::create("게임시간을 증가시킵니다."), kMyLocalKey_longTimeContent);
	ko->setObject(CCString::create("자석"), kMyLocalKey_magnetTitle);
	ko->setObject(CCString::create("골드와 아이템을 빨아들입니다."), kMyLocalKey_magnetContent);
	ko->setObject(CCString::create("아이템 뽑기"), kMyLocalKey_itemGachaTitle);
	ko->setObject(CCString::create("어떤 아이템이 나올까요?"), kMyLocalKey_itemGachaDefaultContent1);
	ko->setObject(CCString::create("뽑기에만 출몰하는 다양한 아이템!!"), kMyLocalKey_itemGachaDefaultContent2);
	ko->setObject(CCString::create("부활 아이템에 도전하세요!"), kMyLocalKey_itemGachaContent);
	ko->setObject(CCString::create("젬상점"), kMyLocalKey_rubyShop);
	ko->setObject(CCString::create("골드상점"), kMyLocalKey_goldShop);
	ko->setObject(CCString::create("하트상점"), kMyLocalKey_heartShop);
//	ko->setObject(CCString::create("구매확인"), kMyLocalKey_checkBuy);
	ko->setObject(CCString::create("구매하시겠습니까?"), kMyLocalKey_realBuy);
	ko->setObject(CCString::create("설정"), kMyLocalKey_option);
	ko->setObject(CCString::create("쿠폰등록"), kMyLocalKey_regCoupon);
	ko->setObject(CCString::create("커뮤니티"), kMyLocalKey_community);
//	ko->setObject(CCString::create("게임tip"), kMyLocalKey_gametip);
//	ko->setObject(CCString::create("튜토리얼"), kMyLocalKey_tutorial);
	ko->setObject(CCString::create("시스템"), kMyLocalKey_system);
	ko->setObject(CCString::create("배경음"), kMyLocalKey_bgm);
	ko->setObject(CCString::create("효과음"), kMyLocalKey_effect);
	ko->setObject(CCString::create("조이스틱\n위치"), kMyLocalKey_joystickPosition);
	ko->setObject(CCString::create("조이스틱\n조작"), kMyLocalKey_joystickControl);
	ko->setObject(CCString::create("대중교통\n모드"), kMyLocalKey_safetyMode);
	ko->setObject(CCString::create("알림"), kMyLocalKey_noti);
	ko->setObject(CCString::create("푸쉬알림"), kMyLocalKey_pushNoti);
	ko->setObject(CCString::create("메세지\n알림"), kMyLocalKey_messageNoti);
	ko->setObject(CCString::create("서버연결에 실패하였습니다."), kMyLocalKey_failedConnect);
	ko->setObject(CCString::create("메세지확인"), kMyLocalKey_confirmGift);
//	ko->setObject(CCString::create("선물을 받았습니다."), kMyLocalKey_getgift);
	ko->setObject(CCString::create("받은 메세지가 없습니다."), kMyLocalKey_nogift);
	ko->setObject(CCString::create("*받은 메세지는 30일 이후에 삭제됩니다."), kMyLocalKey_giftboxAlert);
	ko->setObject(CCString::create("재접속"), kMyLocalKey_reConnect);
	ko->setObject(CCString::create("다른 기기로 연결되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert1);
	ko->setObject(CCString::create("서버와의 접속에 오류가 발생하였습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert2);
	ko->setObject(CCString::create("세션이 종료되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert3);
	ko->setObject(CCString::create("서버연결에 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_reConnectAlert4);
	ko->setObject(CCString::create("BLOCKED"), kMyLocalKey_blocked);
	ko->setObject(CCString::create("계정이 아래의 사유로 정지 되었습니다."), kMyLocalKey_blockedMsg);
	ko->setObject(CCString::create("회원ID : "), kMyLocalKey_memberID);
	ko->setObject(CCString::create("게임버젼 : "), kMyLocalKey_gameversion);
	ko->setObject(CCString::create("게임탈퇴"), kMyLocalKey_withdraw);
	ko->setObject(CCString::create("고객센터"), kMyLocalKey_callCenter);
	ko->setObject(CCString::create("로그아웃 하시겠습니까?"), kMyLocalKey_checkLogout);
	ko->setObject(CCString::create("취소"), kMyLocalKey_cancel);
	ko->setObject(CCString::create("게임 탈퇴"), kMyLocalKey_withdrawTitle);
	ko->setObject(CCString::create("탈퇴시 게임을 이용하실 수 없으며"), kMyLocalKey_withdrawContent1);
	ko->setObject(CCString::create("모든 데이터가 영구적으로 삭제됩니다."), kMyLocalKey_withdrawContent2);
	ko->setObject(CCString::create("정말 탈퇴하시겠습니까?"), kMyLocalKey_withdrawContent3);
	ko->setObject(CCString::create("탈퇴하기"), kMyLocalKey_doWithdraw);
	ko->setObject(CCString::create("꺼짐"), kMyLocalKey_lightOff);
	ko->setObject(CCString::create("켜짐"), kMyLocalKey_lightOn);
	ko->setObject(CCString::create("오른쪽"), kMyLocalKey_right);
	ko->setObject(CCString::create("왼쪽"), kMyLocalKey_left);
	ko->setObject(CCString::create("고정"), kMyLocalKey_fix);
	ko->setObject(CCString::create("이동"), kMyLocalKey_move);
	ko->setObject(CCString::create("주간랭킹"), kMyLocalKey_weeklyranking);
	ko->setObject(CCString::create("%d일후 리셋"), kMyLocalKey_dayAfterReset);
	ko->setObject(CCString::create("%d시간후 리셋"), kMyLocalKey_hourAfterReset);
	ko->setObject(CCString::create("%d분후 리셋"), kMyLocalKey_secondAfterReset);
	ko->setObject(CCString::create("선물함"), kMyLocalKey_giftbox);
	ko->setObject(CCString::create("<font color=#FFAA14 size=15>%s</font><br><font color=#FFFFFF>- %s -</font><br><font color=#CCCCCC size=10s>%s</font>"), kMyLocalKey_giftboxContent);
	ko->setObject(CCString::create("모든선물 받기"), kMyLocalKey_allAccept);
	ko->setObject(CCString::create("티켓함"), kMyLocalKey_ticketBox);
	ko->setObject(CCString::create("도움함"), kMyLocalKey_helpBox);
	ko->setObject(CCString::create("도전함"), kMyLocalKey_challengeBox);
	ko->setObject(CCString::create("하트함"), kMyLocalKey_heartBox);
	ko->setObject(CCString::create("전체보기"), kMyLocalKey_allView);
//	ko->setObject(CCString::create("하트가 도착했어요."), kMyLocalKey_arriveHeart);
//	ko->setObject(CCString::create("도전장이 도착했어요."), kMyLocalKey_arriveChallenge);
//	ko->setObject(CCString::create("도전을 받아들이시겠습니까?"), kMyLocalKey_checkAcceptChallenge);
//	ko->setObject(CCString::create("도망가기"), kMyLocalKey_ttt);
//	ko->setObject(CCString::create("도전에 임하는 자세를 가집시다!!\n도망으로 간주, 패하셨습니다."), kMyLocalKey_tttContent);
//	ko->setObject(CCString::create("도전결과!!"), kMyLocalKey_resultChallenge);
//	ko->setObject(CCString::create("졌습니다. 브레이크 포인트 필요함. 위에 contentObj"), kMyLocalKey_loseContent);
//	ko->setObject(CCString::create("도움요청이 도착했어요."), kMyLocalKey_arriveHelp);
//	ko->setObject(CCString::create("상대방을 도와줍니다."), kMyLocalKey_acceptHelp);
//	ko->setObject(CCString::create("거절"), kMyLocalKey_deny);
//	ko->setObject(CCString::create("수락"), kMyLocalKey_accept);
//	ko->setObject(CCString::create("상대방의 도움이 왔어요!"), kMyLocalKey_arriveHelped);
//	ko->setObject(CCString::create("카드 정보 로딩"), kMyLocalKey_cardInfoLoading);
//	ko->setObject(CCString::create("도움을 받았습니다!!"), kMyLocalKey_iHelped);
//	ko->setObject(CCString::create("티켓요청이 도착했어요."), kMyLocalKey_arriveNeedTicket);
//	ko->setObject(CCString::create("티켓요청이 도착. 티켓은 퍼즐을 열 때 필요합니다. 친구를 도와주세요!!"), kMyLocalKey_arriveNeedTicketContent);
//	ko->setObject(CCString::create("보내기"), kMyLocalKey_send);
//	ko->setObject(CCString::create("티켓이 왔네요 어서 받으세요."), kMyLocalKey_arriveTicket);
//	ko->setObject(CCString::create("티켓이 도착했습니다. 티켓을 받으세요."), kMyLocalKey_arriveTicketContent);
//	ko->setObject(CCString::create("티켓받기"), kMyLocalKey_takeTicket);
//	ko->setObject(CCString::create("친구추가 요청이 왔습니다."), kMyLocalKey_arriveAddFriend);
//	ko->setObject(CCString::create("오류가 발생했습니다."), kMyLocalKey_whatError);
//	ko->setObject(CCString::create("알 수 없는 요청이 도착했어요."), kMyLocalKey_arriveWhatError);
	ko->setObject(CCString::create("기본정렬"), kMyLocalKey_defaultSort);
	ko->setObject(CCString::create("획득순"), kMyLocalKey_takeOrder);
	ko->setObject(CCString::create("등급순"), kMyLocalKey_gradeOrder);
//	ko->setObject(CCString::create("회전"), kMyLocalKey_rotation);
	ko->setObject(CCString::create("레벨을 올리면 보스를 좀 더 쉽게 물리칠 수 있어요!"), kMyLocalKey_upgradeSubMent);
	ko->setObject(CCString::create("버리고 다시 뽑기"), kMyLocalKey_itemRegacha);
	ko->setObject(CCString::create("%.0f%% 할인"), kMyLocalKey_itemRegachaDiscountValue);
	ko->setObject(CCString::create("이 아이템 사용"), kMyLocalKey_thisItemUse);
//	ko->setObject(CCString::create("타임"), kMyLocalKey_time);
//	ko->setObject(CCString::create("골드"), kMyLocalKey_gold);
//	ko->setObject(CCString::create("점수"), kMyLocalKey_score);
	ko->setObject(CCString::create("총점"), kMyLocalKey_totalScore);
	ko->setObject(CCString::create("다음스테이지"), kMyLocalKey_nextStage);
	ko->setObject(CCString::create("다시하기"), kMyLocalKey_regame);
	ko->setObject(CCString::create("메인으로"), kMyLocalKey_toMain);
	ko->setObject(CCString::create("이전 스테이지를\n클리어 해주세요!"), kMyLocalKey_beforeNotClearPuzzle);
	ko->setObject(CCString::create("이전액자를\n모두클리어하면\n다음액자가나타납니다."), kMyLocalKey_waitForUpdate);
//	ko->setObject(CCString::create("기다릴게요!\n제 사진\n모아주세요!"), kMyLocalKey_diaryNoImg);
	ko->setObject(CCString::create("보상"), kMyLocalKey_reward);
	ko->setObject(CCString::create("스피드"), kMyLocalKey_speed);
	ko->setObject(CCString::create("준비하기"), kMyLocalKey_ready);
	ko->setObject(CCString::create("상대찾기"), kMyLocalKey_endlessReady);
//	ko->setObject(CCString::create("지존"), kMyLocalKey_rankA);
//	ko->setObject(CCString::create("영웅"), kMyLocalKey_rankB);
//	ko->setObject(CCString::create("평민"), kMyLocalKey_rankC);
//	ko->setObject(CCString::create("하수"), kMyLocalKey_rankD);
//	ko->setObject(CCString::create("구입"), kMyLocalKey_buy);
	ko->setObject(CCString::create("꾸욱"), kMyLocalKey_controlClick);
	ko->setObject(CCString::create("잠김"), kMyLocalKey_locked);
//	ko->setObject(CCString::create("상자를 선택해주세요."), kMyLocalKey_selectClearBox);
	
	ko->setObject(CCString::create(""), kMyLocalKey_titleLoadingBegin);
	ko->setObject(CCString::create("각 스테이지 별로 각기 다른 4장의 이미지를 획득해 보세요."), kMyLocalKey_titleLoading1);
	ko->setObject(CCString::create("공공장소나 대중교통 이용시에는 '대중교통 모드'로 Play 하세요. 획득한 이미지는 내카드에서 확인 할 수 있습니다."), kMyLocalKey_titleLoading2);
	ko->setObject(CCString::create("파란색 실루엣 영역을 획득해야 % 가 증가해요."), kMyLocalKey_titleLoading3);
	ko->setObject(CCString::create("'영역 다시 뽑기' 를 해서 넓은 영역으로 play 를 시작하면 좀 더 쉽게 높은 등급의 카드를 획득하기가 쉬워져요"), kMyLocalKey_titleLoading4);
	ko->setObject(CCString::create("'대중교통 모드' 는 play 중에 일시정지 버튼을 탭해서 설정할 수 있습니다."), kMyLocalKey_titleLoading5);
	ko->setObject(CCString::create("Clear 등급에 따라 획득되는 별의 개수가 다릅니다. 별이 모이면 다음 액자를 열 수 있어요."), kMyLocalKey_titleLoading7);
	ko->setObject(CCString::create("'업적' 창에서 달성한 업적을 보상받으세요. 보석을 드립니다."), kMyLocalKey_titleLoading8);
	ko->setObject(CCString::create("일일미션에 도전해 보세요. 일일 미션을 달성하시면 다양한 선물을 드립니다."), kMyLocalKey_titleLoading9);
	ko->setObject(CCString::create("주간랭킹에 도전해 보세요. 매주 새로운 선물을 푸짐하게 드립니다."), kMyLocalKey_titleLoading10);
	ko->setObject(CCString::create("change 글자를 모아서 특별한 이미지를 확인하세요."), kMyLocalKey_titleLoading11);
	ko->setObject(CCString::create("미사일을 업그레이드 하면 강력한 공격력으로 높은 등급 카드 획득이 가능합니다."), kMyLocalKey_titleLoading12);
	ko->setObject(CCString::create("아이템뽑기에서는 '부활' 아이템을 뽑을 수 있어요."), kMyLocalKey_titleLoading13);
	ko->setObject(CCString::create("스테이지마다 showtime 에 도전해 보세요."), kMyLocalKey_titleLoading14);
	ko->setObject(CCString::create("매주 새로운 이벤트가 열립니다. 공식 카페를 확인해 주세요."), kMyLocalKey_titleLoading15);
	ko->setObject(CCString::create("게임 내에서는 이름 대신 닉네임을 사용합니다."), kMyLocalKey_titleLoading16);
	ko->setObject(CCString::create("99% 라 너무 아쉽다면, 보너스 뽑기에 도전 해 보세요."), kMyLocalKey_titleLoading17);
	ko->setObject(CCString::create("이벤트 스테이지에서는 특별한 이미지와 선물이 기다립니다."), kMyLocalKey_titleLoading18);
	ko->setObject(CCString::create("스테이지별 미션 clear 가 어렵다면 아이템 뽑기를 통해 미션 전용 아이템을 사용해 보세요."), kMyLocalKey_titleLoading19);
	ko->setObject(CCString::create("한 번에 많은 영역을 획득할 수록 여러 개의 미사일이 발사되며, 공격을 성공한 만큼 콤보가 적용됩니다."), kMyLocalKey_titleLoading6);
	ko->setObject(CCString::create("이어하기는 두 번 까지만 사용할 수 있습니다."), kMyLocalKey_titleLoading20);
	ko->setObject(CCString::create("내카드 목록에서 카드 이미지를 터치하면 카드 상세 내용을 보실 수 있습니다."), kMyLocalKey_titleLoading21);
	ko->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading22);
	ko->setObject(CCString::create("85% 로 클리어 하면 별 1개를 획득합니다."), kMyLocalKey_titleLoading23);
	ko->setObject(CCString::create("change를 모두 모은 후 클리어하면 별 2개를 획득합니다."), kMyLocalKey_titleLoading24);
	ko->setObject(CCString::create("100%로 클리어 하면 별 3개를 획득합니다."), kMyLocalKey_titleLoading25);
	ko->setObject(CCString::create("change를 모두 모은 후 100%로 클리어하면 별 4개를 모을 수 있습니다."), kMyLocalKey_titleLoading26);
	ko->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading27);
	ko->setObject(CCString::create("플레이 중 획득한 영역에 따라 골드를 드립니다."), kMyLocalKey_titleLoading28);
	ko->setObject(CCString::create("30%나 할인되는 종합 아이템 구매 찬스를 놓치지 마세요!"), kMyLocalKey_titleLoading29);
	ko->setObject(CCString::create("미션스테이지에서는 미션을 먼저 완료한 후 클리어가 가능합니다."), kMyLocalKey_titleLoading30);
	ko->setObject(CCString::create("미사일 공격 시 그리는 선에 미사일이 닿으면 유도 공격을 당해요."), kMyLocalKey_titleLoading31);
	ko->setObject(CCString::create("게임 중 타임 보너스 아이템을 먹으면 남은 시간에서 10초가 추가됩니다."), kMyLocalKey_titleLoading32);
	ko->setObject(CCString::create("선을 그리다가 꾸욱 버튼에서 손을 떼면 캐릭터가 안전 영역으로 되돌아 갑니다."), kMyLocalKey_titleLoading33);
	ko->setObject(CCString::create("보스에게 연속 공격을 성공시키면 콤보가 적용되며, 게임 결과에서 콤보 보너스를 받을 수 있습니다."), kMyLocalKey_titleLoading34);
	ko->setObject(CCString::create("내가 모은 별의 개수는 메인화면의 왼쪽 상단에 있는 별 아이콘을 통해 확인하실 수 있습니다."), kMyLocalKey_titleLoading35);
	ko->setObject(CCString::create("운석이 떨어지면 캐릭터에 맞지 않도록 주의하세요."), kMyLocalKey_titleLoading36);
	ko->setObject(CCString::create("조이스틱과 꾸욱 버튼의 위치를 바꾸고 싶다면 설정메뉴에서 조이스틱위치를 반대로 변경해 보세요."), kMyLocalKey_titleLoading37);
	ko->setObject(CCString::create("조이스틱의 위치를 고정하고 싶다면 설정메뉴에서 조이스틱조작을 고정으로 변경해 보세요"), kMyLocalKey_titleLoading38);
	ko->setObject(CCString::create("꾸욱 버튼을 누른채 한쪽 방향으로 그린 다음 되돌아 오면 한줄 그리기가 가능합니다."), kMyLocalKey_titleLoading39);
	ko->setObject(CCString::create("그리는 도중 줄이 꼬여도 당황하지 마세요. 줄이 꼬인 상태에서도 영역을 획득할 수 있습니다."), kMyLocalKey_titleLoading40);
	ko->setObject(CCString::create("작은 몬스터에는 HP가 존재하며, 공격이 성공할 때마다 HP가 줄어듭니다."), kMyLocalKey_titleLoading41);
	ko->setObject(CCString::create("액자 하나를 완성할 때마다 특별한 카드와 하트를 선물로 드립니다."), kMyLocalKey_titleLoading42);
	ko->setObject(CCString::create("클리어 점수는 영역획득점수와 콤보점수, 남은 타임 보너스가 합산된 점수입니다."), kMyLocalKey_titleLoading43);
	ko->setObject(CCString::create("고득점을 노린다면 연속 콤보를 성공시켜 보세요."), kMyLocalKey_titleLoading44);
	ko->setObject(CCString::create(""), kMyLocalKey_titleLoadingEnd);
	
	ko->setObject(CCString::create("게임시작"), kMyLocalKey_gamestart);
	ko->setObject(CCString::create("대전 시작"), kMyLocalKey_battleStart);
	ko->setObject(CCString::create("내 미사일"), kMyLocalKey_mymissile);
	ko->setObject(CCString::create("파워"), kMyLocalKey_power);
	ko->setObject(CCString::create("뽑기"), kMyLocalKey_gacha);
	ko->setObject(CCString::create("일일미션을 완료해보세요!\n선물함으로 선물을 드려요! ^^"), kMyLocalKey_stageListDown);
	ko->setObject(CCString::create("액자오픈조건"), kMyLocalKey_frameOpenConditionTitle);
	ko->setObject(CCString::create("이전액자클리어\n+별%d개이상"), kMyLocalKey_frameOpenConditionContent);
	ko->setObject(CCString::create("젬으로오픈"), kMyLocalKey_frameOpenConditionContentRuby);
	ko->setObject(CCString::create("%s\n%d:00-%d:00"), kMyLocalKey_frameOpenConditionContentTimeWeek);
	ko->setObject(CCString::create("%s월%s일%s:%s오픈"), kMyLocalKey_frameOpenConditionContentTimeDate);
	ko->setObject(CCString::create("자세히보기"), kMyLocalKey_detailView);
	ko->setObject(CCString::create("별 획득 방법"), kMyLocalKey_detailConditionPopupTitle);
	ko->setObject(CCString::create("<font newline=14 strokecolor=000>스테이지를 클리어 할 때마다<font newline=30>클리어 단계에 따라 별을 획득합니다.<font color=961>한 스테이지<font newline=14>에서 획득할 수 있는<font color=961>별 갯수는 총 10개<font newline=14>입니다."), kMyLocalKey_detailConditionPopupContent);
//	ko->setObject(CCString::create("<font color=999 size=12>스테이지에서 <font color=990 size=12>4가지 등급<font color=999 size=12 newline=13>을 모두 Clear하면<font color=990 size=12>별 10개<font color=999 size=12>를 모을 수 있습니다."), kMyLocalKey_detailConditionPopupContent2);
//	ko->setObject(CCString::create("에서 획득할 수 있는"), kMyLocalKey_detailConditionPopupContent3);
//	ko->setObject(CCString::create("별 갯수는 총 10개"), kMyLocalKey_detailConditionPopupContent4);
//	ko->setObject(CCString::create("입니다."), kMyLocalKey_detailConditionPopupContent5);
	ko->setObject(CCString::create("결제 실패"), kMyLocalKey_failPurchase);
	ko->setObject(CCString::create("구매 완료"), kMyLocalKey_successPurchase);
	ko->setObject(CCString::create("지금 상점으로 이동하시겠습니까?"), kMyLocalKey_enoughtGoodsContent);
//	ko->setObject(CCString::create("포기하기"), kMyLocalKey_giveup);
	ko->setObject(CCString::create("상점 바로가기"), kMyLocalKey_goShopButton);
	
	ko->setObject(CCString::create("액자를 플레이할 수 있는 시간이 지났습니다.\n홈으로 돌아갑니다."), kMyLocalKey_timeOutFrame);
	ko->setObject(CCString::create("바로입장"), kMyLocalKey_directEnter);
	ko->setObject(CCString::create("카드 승급 기회!!"), kMyLocalKey_rankUpTitle);
	ko->setObject(CCString::create("승급 성공시 한 단계 높은 카드를 획득할 수 있어요!"), kMyLocalKey_rankUpSubTitle);
	ko->setObject(CCString::create("현재 획득카드"), kMyLocalKey_recentTakeCard);
	ko->setObject(CCString::create("승급확률"), kMyLocalKey_rankUpRate);
	ko->setObject(CCString::create("승급하기"), kMyLocalKey_rankup);
	
	ko->setObject(CCString::create("구매하기"), kMyLocalKey_purchase);
	ko->setObject(CCString::create("첫구매시 할인"), kMyLocalKey_firstPurchaseSale);
	ko->setObject(CCString::create("%s할인은 첫 구매 회원님께만"), kMyLocalKey_firstPurchaseMent1);
	ko->setObject(CCString::create(" 적용됩니다."), kMyLocalKey_firstPurchaseMent2);
	ko->setObject(CCString::create("이 창을 닫으면 %s 할인 기회는 사라집니다."), kMyLocalKey_firstPurchaseMent3);
	ko->setObject(CCString::create("지금 묶음 아이템을 구매하시면"), kMyLocalKey_emptyItemSaleMent1);
	ko->setObject(CCString::create("%s 할인해 드립니다."), kMyLocalKey_emptyItemSaleMent2);
	ko->setObject(CCString::create("플레이를 도와주는 기능성 아이템을"), kMyLocalKey_stupidNpuHelpMent1);
	ko->setObject(CCString::create("%s할인된 가격으로 사용해보세요."), kMyLocalKey_stupidNpuHelpMent2);
	ko->setObject(CCString::create("묶음아이템 할인"), kMyLocalKey_packageItemSale);
	ko->setObject(CCString::create("5번째 구매시"), kMyLocalKey_mileageMent1);
	ko->setObject(CCString::create("아주 특별한 할인혜택의 기회가 있으니"), kMyLocalKey_mileageMent2);
	ko->setObject(CCString::create("혜택을 꼭! 확인해주세요."), kMyLocalKey_mileageMent3);
	ko->setObject(CCString::create("단 한번의 구매 찬스"), kMyLocalKey_eventShopTitle);
	ko->setObject(CCString::create("당신만을 위한 특별한 기회!"), kMyLocalKey_eventShopMent1);
	ko->setObject(CCString::create("%s 보너스 혜택을 드려요."), kMyLocalKey_eventShopMent2);
	ko->setObject(CCString::create("이 창을 닫으면 %s 보너스 기회는 사라집니다."), kMyLocalKey_eventShopMent3);
	ko->setObject(CCString::create("<font>미사일 레벨을 올리면 </font><font color=961>부하몬스터를 쉽게 죽이고,</font>"), kMyLocalKey_levelupGuideMent1);
	ko->setObject(CCString::create("<font color=961>패턴공격을 빨리 저지시킬 수 있으며,</font>"), kMyLocalKey_levelupGuideMent2);
	ko->setObject(CCString::create("<font color=961>고득점 기록에 도움이 됩니다.</font>"), kMyLocalKey_levelupGuideMent3);
	ko->setObject(CCString::create("<font>미사일 업그레이드를 하러 갈까요?</font>"), kMyLocalKey_levelupGuideMent4);
//	ko->setObject(CCString::create("다음에"), kMyLocalKey_next);
	ko->setObject(CCString::create("업그레이드"), kMyLocalKey_levelupGo);
	ko->setObject(CCString::create("업적"), kMyLocalKey_achievement);
	ko->setObject(CCString::create("모든보상 받기"), kMyLocalKey_allRewardGet);
	ko->setObject(CCString::create("종합 아이템 구매 찬스!"), kMyLocalKey_itemPackageChance);
	ko->setObject(CCString::create("아이템이 하나도 없네요."), kMyLocalKey_emptyItemSaleTitle);
	ko->setObject(CCString::create("첫구매 대폭할인!!"), kMyLocalKey_firstPurchaseSaleTitle);
	ko->setObject(CCString::create("사랑합니다. 고객님!"), kMyLocalKey_mileageTitle);
	ko->setObject(CCString::create("미사일 업그레이드"), kMyLocalKey_missileUpgrade);
	ko->setObject(CCString::create("<font>누적영역</font><font color=961>%d%%</font><font strokesize=0.5 strokeopacity=150>를 획득하라</font>"), kMyLocalKey_todaymissionTotalPercent1);
	ko->setObject(CCString::create("%s%%"), kMyLocalKey_todaymissionTotalPercent2);
	ko->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalPercent3);
	ko->setObject(CCString::create("현재 획득영역 "), kMyLocalKey_todaymissionTotalPercent4);
	ko->setObject(CCString::create("<font>누적점수</font><font color=961>%s점</font><font strokesize=0.5 strokeopacity=150>을 획득하라</font>"), kMyLocalKey_todaymissionTotalScore1);
	ko->setObject(CCString::create("%s점"), kMyLocalKey_todaymissionTotalScore2);
	ko->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalScore3);
	ko->setObject(CCString::create("현재 획득점수 "), kMyLocalKey_todaymissionTotalScore4);
	ko->setObject(CCString::create("<font color=961>%s골드</font><font strokesize=0.5 strokeopacity=150>를 획득하라</font>"), kMyLocalKey_todaymissionTotalTakeGold1);
	ko->setObject(CCString::create("%s골드"), kMyLocalKey_todaymissionTotalTakeGold2);
	ko->setObject(CCString::create("현재 획득골드 "), kMyLocalKey_todaymissionTotalTakeGold3);
	ko->setObject(CCString::create("<font>부하몹</font><font color=961>%d마리</font><font strokesize=0.5 strokeopacity=150>를 획득하라</font>"), kMyLocalKey_todaymissionTotalCatch1);
	ko->setObject(CCString::create("%s마리"), kMyLocalKey_todaymissionTotalCatch2);
	ko->setObject(CCString::create(" 사냥하라!!"), kMyLocalKey_todaymissionTotalCatch3);
	ko->setObject(CCString::create("현재 잡은 부하몹 "), kMyLocalKey_todaymissionTotalCatch4);
//	ko->setObject(CCString::create("오늘의 미션을 완료하였습니다."), kMyLocalKey_todaymissionSuccess);
	ko->setObject(CCString::create("완료"), kMyLocalKey_complete);
	ko->setObject(CCString::create("업적달성"), kMyLocalKey_achieveSuccess);
	ko->setObject(CCString::create("업적미완성"), kMyLocalKey_achieveNotSuccess);
	ko->setObject(CCString::create("업적보상"), kMyLocalKey_achieveReward);
	ko->setObject(CCString::create("달성한 업적이 없습니다."), kMyLocalKey_nothingSuccessAchieve);
	ko->setObject(CCString::create("보상받을 업적이 없습니다."), kMyLocalKey_nothingRewardAchieve);
	
	
	ko->setObject(CCString::create("영역획득"), kMyLocalKey_missionTitle0);
	ko->setObject(CCString::create("용감한 기사"), kMyLocalKey_missionTitle1);
	ko->setObject(CCString::create("부하몹 사냥"), kMyLocalKey_missionTitle2);
	ko->setObject(CCString::create("욕심쟁이"), kMyLocalKey_missionTitle3);
	ko->setObject(CCString::create("수집가"), kMyLocalKey_missionTitle4);
	ko->setObject(CCString::create("완벽주의자"), kMyLocalKey_missionTitle5);
	ko->setObject(CCString::create("결벽주의자"), kMyLocalKey_missionTitle6);
	ko->setObject(CCString::create("비지니스맨"), kMyLocalKey_missionTitle7);
	
	ko->setObject(CCString::create("<font color=961 size=13>%.0f%%이상<font color=999 size=13> 획득하라!"), kMyLocalKey_missionDiscription0);
	ko->setObject(CCString::create("<font color=999 size=13>보스의 에너지를 모두 소진시켜라!"), kMyLocalKey_missionDiscription1);
	ko->setObject(CCString::create("<font color=999 size=13>부하 몬스터를 가두어 잡으세요!"), kMyLocalKey_missionDiscription2);
	ko->setObject(CCString::create("<font color=999 size=13>정해진 횟수만큼 한번에 많이 먹으세요!"), kMyLocalKey_missionDiscription3);
	ko->setObject(CCString::create("<font color=999 size=13>정해진 숫자만큼 아이템을 모으세요!"), kMyLocalKey_missionDiscription4);
	ko->setObject(CCString::create("<font color=999 size=13>정해진 목표로 정확하게 영역을 획득하세요!"), kMyLocalKey_missionDiscription5);
	ko->setObject(CCString::create("<font color=999 size=13>CHANGE를 순서대로 획득하세요!"), kMyLocalKey_missionDiscription6);
	ko->setObject(CCString::create("<font color=999 size=13>제한시간 내에 클리어하세요!"), kMyLocalKey_missionDiscription7);
	
	ko->setObject(CCString::create(""), kMyLocalKey_missionCondition0);
	ko->setObject(CCString::create(""), kMyLocalKey_missionCondition1);
	ko->setObject(CCString::create("<font color=961 size=18>%d마리"), kMyLocalKey_missionCondition2);
	ko->setObject(CCString::create("<font color=961 size=18>%d%%이상 %d번"), kMyLocalKey_missionCondition3);
	ko->setObject(CCString::create("<font>아이템을 </font><font color=961 size=18>%d개</font><font> 모으세요!</font>"), kMyLocalKey_missionCondition4);
	ko->setObject(CCString::create(""), kMyLocalKey_missionCondition5);
	ko->setObject(CCString::create(""), kMyLocalKey_missionCondition6);
	ko->setObject(CCString::create("<font color=961 size=18>%d초"), kMyLocalKey_missionCondition7);
	
	
	ko->setObject(CCString::create("대쉬"), kMyLocalKey_item4title);
	ko->setObject(CCString::create("잠깐동안 빠른속도로 이동 가능합니다."), kMyLocalKey_item4ment);
	ko->setObject(CCString::create("부하몹제거"), kMyLocalKey_item5title);
	ko->setObject(CCString::create("부하 몬스터 한마리를 제거합니다."), kMyLocalKey_item5ment);
	ko->setObject(CCString::create("침묵"), kMyLocalKey_item7title);
	ko->setObject(CCString::create("잠깐동안 몬스터가 공격을 못하도록 합니다."), kMyLocalKey_item7ment);
	ko->setObject(CCString::create("부활"), kMyLocalKey_item10title);
	ko->setObject(CCString::create("마지막 순간에 부활합니다."), kMyLocalKey_item10ment);
	ko->setObject(CCString::create("시간추가"), kMyLocalKey_item8title);
	ko->setObject(CCString::create("제한시간을 늘려줍니다."), kMyLocalKey_item8ment);
	ko->setObject(CCString::create("일시정지"), kMyLocalKey_pause);
	ko->setObject(CCString::create("계속하기"), kMyLocalKey_continue);
	ko->setObject(CCString::create("나가기"), kMyLocalKey_ingameOut);
	ko->setObject(CCString::create("재시작"), kMyLocalKey_ingameReplay);
	ko->setObject(CCString::create("대중교통\n모드"), kMyLocalKey_publicPlaceMode);
//	ko->setObject(CCString::create("이어하기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass1);
//	ko->setObject(CCString::create("맵다시뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass2);
//	ko->setObject(CCString::create("업그레이드 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass3);
//	ko->setObject(CCString::create("아이템뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass4);
//	ko->setObject(CCString::create("1프로뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass5);
	ko->setObject(CCString::create("<font newline=16>오늘의 미션을 완료하였습니다.<font color=961 newline=16>보상은 선물함으로 지급됩니다."), kMyLocalKey_todaymissionSuccessCommon);
	ko->setObject(CCString::create("무료"), kMyLocalKey_free);
	ko->setObject(CCString::create("영역 다시뽑기"), kMyLocalKey_areaReGacha);
//	ko->setObject(CCString::create("버튼을 눌러주세요."), kMyLocalKey_pleaseClickButton);
//	ko->setObject(CCString::create("멈춰!"), kMyLocalKey_stop);
	ko->setObject(CCString::create("도전하기"), kMyLocalKey_go100percent);
	ko->setObject(CCString::create("계정연결"), kMyLocalKey_accountLink);
	ko->setObject(CCString::create("최고 레벨 %d"), kMyLocalKey_maxLevel);
	ko->setObject(CCString::create("업그레이드 완료"), kMyLocalKey_endUpgrade);
	ko->setObject(CCString::create("%d단계 카드 획득방법"), kMyLocalKey_cardLockedTitle);
	ko->setObject(CCString::create("<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent1);
	ko->setObject(CCString::create("<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent2);
	ko->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent3);
	ko->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent4);
	ko->setObject(CCString::create("<font color=999 size=13 strokesize=0.5 strokeopacity=150>남은시간"), kMyLocalKey_todaymissionRemainTime);
	ko->setObject(CCString::create("<font color=999 size=13 strokesize=0.5 strokeopacity=150>남은시간"), kMyLocalKey_todaymissionRemainTimeMinute);
	ko->setObject(CCString::create("<font color=961 size=15>%dHour"), kMyLocalKey_todaymissionRemainTime2);
	ko->setObject(CCString::create("<font color=961 size=15>%dMinute"), kMyLocalKey_todaymissionRemainTimeMinute2);
	ko->setObject(CCString::create("<font color=961 size=15>모든 피스 획득!"), kMyLocalKey_puzzleSuccessTitle);
	ko->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 하트를 드릴께요!"), kMyLocalKey_puzzleSuccessMent);
	ko->setObject(CCString::create("<font color=961 size=15>놀라워요! <font color=961 size=15>퍼펙트 클리어!!"), kMyLocalKey_puzzlePerfectTitle);
	ko->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 젬를 드릴께요!"), kMyLocalKey_puzzlePerfectMent);
	ko->setObject(CCString::create("이어하시겠습니까?"), kMyLocalKey_doYouWantToContinue);
	ko->setObject(CCString::create("이어하기"), kMyLocalKey_onContinue);
	ko->setObject(CCString::create("그만하기"), kMyLocalKey_offContinue);
	ko->setObject(CCString::create("이번 스테이지는 아이템을 사용할 수 없습니다."), kMyLocalKey_notUseItem);
	ko->setObject(CCString::create("새로운 아이템 등장!"), kMyLocalKey_newItemTitle);
	ko->setObject(CCString::create("새로운 아이템을 선물로 드릴께요!"), kMyLocalKey_newItemMent);
	ko->setObject(CCString::create("보스가 쏜 미사일이\n선에 닿으면 불씨가 생겨 닿으면 죽어요."), kMyLocalKey_dieTutorial1);
	ko->setObject(CCString::create("선을 그을때 몬스터가\n선에 닿으면 파동이 생겨 죽어요!"), kMyLocalKey_dieTutorial2);
	ko->setObject(CCString::create(""), kMyLocalKey_dieTutorial3);
	ko->setObject(CCString::create("남은시간 "), kMyLocalKey_restTime);
	ko->setObject(CCString::create("%d Day"), kMyLocalKey_restTimeDay);
	ko->setObject(CCString::create("%d Hour"), kMyLocalKey_restTimeHour);
	ko->setObject(CCString::create("%d Min"), kMyLocalKey_restTimeMinute);
	ko->setObject(CCString::create("%d Sec"), kMyLocalKey_restTimeSecond);
//	ko->setObject(CCString::create(""), kMyLocalKey_);
//	ko->setObject(CCString::create(""), kMyLocalKey_);
//	ko->setObject(CCString::create(""), kMyLocalKey_);
//	ko->setObject(CCString::create(""), kMyLocalKey_);
	
	
	ko->setObject(CCString::create("게임을 종료하시겠습니까?"), kMyLocalKey_exit);
	
	ko->setObject(CCString::create("카운트 폭탄"), kMyLocalKey_warning9);
	ko->setObject(CCString::create("주황 톱니바퀴"), kMyLocalKey_warning105);
	ko->setObject(CCString::create("파란 톱니바퀴"), kMyLocalKey_warning106);
	ko->setObject(CCString::create("파괴 레이저"), kMyLocalKey_warning107);
	ko->setObject(CCString::create("미사일"), kMyLocalKey_warning108);
	ko->setObject(CCString::create("돌진"), kMyLocalKey_warning109);
	ko->setObject(CCString::create("불꽃놀이"), kMyLocalKey_warning110);
	ko->setObject(CCString::create("해바라기"), kMyLocalKey_warning111);
	ko->setObject(CCString::create("얼음폭탄"), kMyLocalKey_warning112);
	ko->setObject(CCString::create("유도폭탄"), kMyLocalKey_warning113);
	ko->setObject(CCString::create("토네이도"), kMyLocalKey_warning1001);
	ko->setObject(CCString::create("태양권"), kMyLocalKey_warning1002);
	ko->setObject(CCString::create("슬로우존"), kMyLocalKey_warning1003);
	ko->setObject(CCString::create("가시감옥"), kMyLocalKey_warning1004);
	ko->setObject(CCString::create("빙결"), kMyLocalKey_warning1005);
	ko->setObject(CCString::create("혼란"), kMyLocalKey_warning1006);
	ko->setObject(CCString::create("순간이동"), kMyLocalKey_warning1007);
	ko->setObject(CCString::create("투명망토"), kMyLocalKey_warning1008);
	ko->setObject(CCString::create("화염 방사"), kMyLocalKey_warning1009);
	ko->setObject(CCString::create("벼락"), kMyLocalKey_warning1010);
	ko->setObject(CCString::create("스피드레이저"), kMyLocalKey_warning1011);
	ko->setObject(CCString::create("방사능"), kMyLocalKey_warning1012);
	ko->setObject(CCString::create("메테오"), kMyLocalKey_warning1013);
	ko->setObject(CCString::create("운석"), kMyLocalKey_warning1014);
	ko->setObject(CCString::create("뾰족풍차"), kMyLocalKey_warning1015);
	ko->setObject(CCString::create("다이너마이트 지뢰"), kMyLocalKey_warning1016);
	ko->setObject(CCString::create("불꽃스토커"), kMyLocalKey_warning1017);
	ko->setObject(CCString::create("문득구름"), kMyLocalKey_warning1018);
	ko->setObject(CCString::create("부하몬스터 소환"), kMyLocalKey_warning1020);
	ko->setObject(CCString::create("보스공격저지"), kMyLocalKey_warningBossSuccess);
	ko->setObject(CCString::create("마지막 목숨"), kMyLocalKey_warningLastLife);
	
	ko->setObject(CCString::create("새로운 보스 공격!"), kMyLocalKey_newBossPattern);
	
	ko->setObject(CCString::create("맵에 부딪힐때마다 숫자가 줄고, 0이 되면 폭발해요."), kMyLocalKey_patternContent9);
	ko->setObject(CCString::create("하나의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent105);
	ko->setObject(CCString::create("여러개의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent106);
	ko->setObject(CCString::create("레이저가 획득영역을 부숴요."), kMyLocalKey_patternContent107);
	ko->setObject(CCString::create("그리는도중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent108);
	ko->setObject(CCString::create("보스가 획득영역을 파괴하며 이동해요."), kMyLocalKey_patternContent109);
	ko->setObject(CCString::create("획득영역에서 폭발후 미사일을 퍼트려요."), kMyLocalKey_patternContent110);
	ko->setObject(CCString::create("사방으로 미사일을 발사해요."), kMyLocalKey_patternContent111);
	ko->setObject(CCString::create("연속으로 발사되어 획득영역을 부숴요."), kMyLocalKey_patternContent112);
	ko->setObject(CCString::create("따라와서 폭발해요."), kMyLocalKey_patternContent113);
	ko->setObject(CCString::create("맵의 일부를 일정시간 가립니다."), kMyLocalKey_patternContent1001);
	ko->setObject(CCString::create("일정시간 화면 전체를 뒤덮어 시야를 가려요."), kMyLocalKey_patternContent1002);
	ko->setObject(CCString::create("슬로우존 안에서는 이동속도가 느려져요."), kMyLocalKey_patternContent1003);
	ko->setObject(CCString::create("붉은 원의 테두리에 닿으면 생명 하나를 잃어요."), kMyLocalKey_patternContent1004);
	ko->setObject(CCString::create("일정시간 움직이지 못해요."), kMyLocalKey_patternContent1005);
	ko->setObject(CCString::create("조작 방향의 반대로 움직여요."), kMyLocalKey_patternContent1006);
	ko->setObject(CCString::create("보스가 다른위치로 순간이동해요."), kMyLocalKey_patternContent1007);
	ko->setObject(CCString::create("일정시간 보스가 보이지 않아요."), kMyLocalKey_patternContent1008);
	ko->setObject(CCString::create("획득영역에 불을 피워요."), kMyLocalKey_patternContent1009);
	ko->setObject(CCString::create("캐릭터를 따라오며 맵을 부숴요."), kMyLocalKey_patternContent1010);
	ko->setObject(CCString::create("그리는중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent1011);
	ko->setObject(CCString::create("방사능이 퍼진곳을 조심하세요."), kMyLocalKey_patternContent1012);
	ko->setObject(CCString::create("대각선으로 떨어져 맵을 부숴요."), kMyLocalKey_patternContent1013);
	ko->setObject(CCString::create("위에서 아래로 운석이 떨어져요."), kMyLocalKey_patternContent1014);
	ko->setObject(CCString::create("닿지않도록 주의하세요."), kMyLocalKey_patternContent1015);
	ko->setObject(CCString::create("카운트가 끝나면 폭발해요."), kMyLocalKey_patternContent1016);
	ko->setObject(CCString::create("일정시간 맵을 따라 움직여요."), kMyLocalKey_patternContent1017);
	ko->setObject(CCString::create("언제 폭발할지 몰라요."), kMyLocalKey_patternContent1018);
	ko->setObject(CCString::create("부하몬스터를 새로 소환합니다."), kMyLocalKey_patternContent1020);
	
//	ko->setObject(CCString::create("서버에 연결할 수 없습니다."), kMyLocalKey_canNotConnectedServer);
//	ko->setObject(CCString::create("화면을 터치 해주세요."), kMyLocalKey_touchPlease);
	
	ko->setObject(CCString::create("가운데 빨간 보석이 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다."), kMyLocalKey_tutorial1);
	ko->setObject(CCString::create("먼저 영역 위를 이동하는 방법을 알려드리겠습니다.\n오른쪽 아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있습니다.\n조이스틱으로 캐릭터를 위로 이동시켜보세요."), kMyLocalKey_tutorial2);
	ko->setObject(CCString::create("캐릭터를 위로 이동시키기"), kMyLocalKey_tutorial3);
	ko->setObject(CCString::create("다음에는 영역을 획득하는 방법을 알아보도록 해요.\n왼쪽 아래의 꾸욱 버튼을 누르고 있으면\n영역 바깥으로 나갈 수 있습니다.\n보이는 것처럼 영역을 획득해보세요."), kMyLocalKey_tutorial4);
	ko->setObject(CCString::create("영역획득하기"), kMyLocalKey_tutorial5);
	ko->setObject(CCString::create("파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다."), kMyLocalKey_tutorial6);
	ko->setObject(CCString::create("<font strokesize=0>제한시간 내에 달성도 <font color=961 newline=13 strokesize=0>85%를 넘기면 클리어!!"), kMyLocalKey_tutorial7);
	ko->setObject(CCString::create("기본 튜토리얼을 모두 진행하셨습니다.\n본 게임으로 들아갑니다."), kMyLocalKey_tutorial8);
	
	ko->setObject(CCString::create("겹친선도 획득!"), kMyLocalKey_controlTip_lineTangle_title);
	ko->setObject(CCString::create("꼬인선! 걱정마세요! 그대로 획득할 수 있어요!"), kMyLocalKey_controlTip_lineTangle_content);
	ko->setObject(CCString::create("한줄 먹기!"), kMyLocalKey_controlTip_slimLine_title);
	ko->setObject(CCString::create("캐릭터를 왼쪽으로 이동 후 오른쪽으로 조작하면 OK!"), kMyLocalKey_controlTip_slimLine_content);
	
//	ko->setObject(CCString::create("미션"), kMyLocalKey_mission);
	ko->setObject(CCString::create("PvP"), kMyLocalKey_endlessMode);
	ko->setObject(CCString::create("주간연승랭킹"), kMyLocalKey_endlessWeeklyranking);
//	ko->setObject(CCString::create("정보"), kMyLocalKey_endlessMyInfo);
	ko->setObject(CCString::create("전적"), kMyLocalKey_endlessInfoScore);
	ko->setObject(CCString::create("<font color=999 strokecolor=000 strokesize=0>%d승 %d패 </font><font color=950 strokecolor=000 strokesize=0>(승률%d%%)</font>"), kMyLocalKey_endlessInfoScoreValue2);
//	ko->setObject(CCString::create("%d승 %d패 승률%d%%"), kMyLocalKey_endlessInfoScoreValue);
	ko->setObject(CCString::create("최고점수"), kMyLocalKey_endlessHighScore);
	ko->setObject(CCString::create("최고연승"), kMyLocalKey_endlessHighStraight);
	ko->setObject(CCString::create("<font size=16 color=#FFFFFF>%d</font><font size=10 color=#FFFFFF>연승</font>"), kMyLocalKey_endlessHighStraightValue1);
	ko->setObject(CCString::create("%d연승"), kMyLocalKey_endlessHighStraightValue2);
	ko->setObject(CCString::create("뭔가 문제가 발생하였습니다.\n다시 시도해주세요."), kMyLocalKey_endlessServerError);
	ko->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_endlessDownloadFail);
//	ko->setObject(CCString::create("포기하시면 1패가 추가됩니다.\n계속 하시겠습니까?"), kMyLocalKey_endlessCheckGiveup);
	
	ko->setObject(CCString::create("스페셜데이"), kMyLocalKey_attendanceSpecialTitle);
	ko->setObject(CCString::create("매일 매일 연속 출석!!"), kMyLocalKey_attendanceSeqTitle);
	ko->setObject(CCString::create("연속출석 일수에 따라 더욱 푸짐한 선물을 드립니다."), kMyLocalKey_attendanceSpecialContent);
	
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>골드"), kMyLocalKey_attendanceSpecialGoodsTypeGold);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>젬"), kMyLocalKey_attendanceSpecialGoodsTypeRuby);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem9);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem6);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem11);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass1);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass2);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass3);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass4);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass5);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass6);
	ko->setObject(CCString::create("<font size=13.5>%d<font size=10>종류"), kMyLocalKey_attendanceSpecialGoodsTypeMany);
	
	ko->setObject(CCString::create("<font size=12>%d<font size=8>골드"), kMyLocalKey_attendanceGoodsTypeGold);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>젬"), kMyLocalKey_attendanceGoodsTypeRuby);
	ko->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem9);
	ko->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem6);
	ko->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem11);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass1);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass2);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass3);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass4);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass5);
	ko->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass6);
	ko->setObject(CCString::create("선물상자"), kMyLocalKey_attendanceGoodsTypeMany);
	
	ko->setObject(CCString::create("출석체크"), kMyLocalKey_attendanceTitle);
	ko->setObject(CCString::create("지급된 보상은 수신함에서 확인 가능합니다."), kMyLocalKey_attendanceContent);
	
	ko->setObject(CCString::create("일차"), kMyLocalKey_attendanceDay);
	
	ko->setObject(CCString::create("생명의 돌"), kMyLocalKey_buyMorphingTitle);
	ko->setObject(CCString::create("<font color=961 newline=18>생명의 돌을 사용하면 카드가 살아납니다.<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_buyMorphingContent);
	
	ko->setObject(CCString::create("보유갯수 :"), kMyLocalKey_buyMorphingTake);
	ko->setObject(CCString::create("개"), kMyLocalKey_buyMorphingTakeValue);
	
	ko->setObject(CCString::create("사용하기"), kMyLocalKey_buyMorphingBuy);
	
	ko->setObject(CCString::create("생명의 돌이 부족합니다."), kMyLocalKey_notEnoughtMorphingTitle);
	ko->setObject(CCString::create("<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_notEnoughtMorphingContent);
	
	ko->setObject(CCString::create("쿠폰등록"), kMyLocalKey_couponTitle);
	ko->setObject(CCString::create("입력"), kMyLocalKey_couponContent);
	ko->setObject(CCString::create("쿠폰등록성공"), kMyLocalKey_couponSuccess);
	ko->setObject(CCString::create("선물함을 확인하세요."), kMyLocalKey_couponGiftbox);
	ko->setObject(CCString::create("쿠폰등록실패"), kMyLocalKey_couponFail);
	ko->setObject(CCString::create("이미 사용한 쿠폰입니다."), kMyLocalKey_couponAlready);
	ko->setObject(CCString::create("유효기간이 만료 쿠폰입니다."), kMyLocalKey_couponExpire);
	ko->setObject(CCString::create("잘못된 사용입니다."), kMyLocalKey_couponOserror);
	ko->setObject(CCString::create("쿠폰을 찾을 수 없습니다."), kMyLocalKey_couponDontfind);
	ko->setObject(CCString::create("통신이 원활하지 않습니다.\n잠시후 다시 시도해주세요."), kMyLocalKey_couponOtherError);
	
	ko->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_notOpenPuzzleNotEnoughtStarTitle);
	ko->setObject(CCString::create("<font newline=16>별이 부족해요.<font newline=16>더 높은 등급으로 클리어해서<font color=961 newline=16>별 %d개 모아주세요."), kMyLocalKey_notOpenPuzzleNotEnoughtStarContent);
	
	ko->setObject(CCString::create("대전 준비"), kMyLocalKey_endlessStartSettingTitle);
	
	ko->setObject(CCString::create("영역점수"), kMyLocalKey_endlessCalcTitleAreaScore);
	ko->setObject(CCString::create("공격점수"), kMyLocalKey_endlessCalcTitleDamageScore);
	ko->setObject(CCString::create("콤보점수"), kMyLocalKey_endlessCalcTitleComboScore);
	ko->setObject(CCString::create("생명점수"), kMyLocalKey_endlessCalcTitleLifeScore);
	ko->setObject(CCString::create("시간점수"), kMyLocalKey_endlessCalcTitleTimeScore);
	ko->setObject(CCString::create("등급점수"), kMyLocalKey_endlessCalcTitleGradeScore);
	ko->setObject(CCString::create("획득골드"), kMyLocalKey_endlessCalcTitleTakeGold);
	ko->setObject(CCString::create("획득영역"), kMyLocalKey_endlessCalcTitleTakeArea);
	ko->setObject(CCString::create("피격점수"), kMyLocalKey_endlessCalcTitleDamagedScore);
	
	ko->setObject(CCString::create("승"), kMyLocalKey_win);
	ko->setObject(CCString::create("게임결과"), kMyLocalKey_gameResult);
	ko->setObject(CCString::create("실패"), kMyLocalKey_fail);
	ko->setObject(CCString::create("달성!!"), kMyLocalKey_achieve);
	ko->setObject(CCString::create("닫기"), kMyLocalKey_close);
	
	ko->setObject(CCString::create("%d연승중"), kMyLocalKey_endlessIngWin);
	
	ko->setObject(CCString::create("PVP 오픈조건"), kMyLocalKey_endlessOpenConditionTitle);
	ko->setObject(CCString::create("<font color=961 newline=16>PVP는 %d스테이지 플레이 후<font newline=16>자동으로 오픈됩니다."), kMyLocalKey_endlessOpenConditionContent);
	
	ko->setObject(CCString::create("대전패배"), kMyLocalKey_endlessKeepWin);
	
	ko->setObject(CCString::create("연승저장"), kMyLocalKey_endlessKeepWinTitle);
	ko->setObject(CCString::create("연승 정보는 유지되며, 다시 PvP 도전 시\n저장된 연승에 이어서 플레이할 수 있습니다."), kMyLocalKey_endlessKeepWinContent);
	 
	ko->setObject(CCString::create("%d연승 도전"), kMyLocalKey_endlessReadyIngWin);
	ko->setObject(CCString::create("닉네임 오류"), kMyLocalKey_nicknameError);
	ko->setObject(CCString::create("<font color=#FFFFFF strokesize=1 >남은 판수 </font>"
																								"<font color=#FFAA14 size=12 strokesize=1>%d회 / </font>"
																								"<font color=#FFFFFF strokesize=1>성공시 </font>"
																								"<font color=#FFAA14 size=12 strokesize=1>%.1f%% 획득</font>"), kMyLocalKey_onePercentDesc2);
	
	ko->setObject(CCString::create("대전상대를 찾는 중입니다."), kMyLocalKey_rivalSearch);
	
	ko->setObject(CCString::create("%d레벨"), kMyLocalKey_rivalLevel);
	ko->setObject(CCString::create("%d 연승 중"), kMyLocalKey_rivalIngWin);
	ko->setObject(CCString::create("대전상대를 선택해주세요."), kMyLocalKey_rivalSelect);
	
	ko->setObject(CCString::create("업적완료"), kMyLocalKey_achieveSuccess2);
	
	ko->setObject(CCString::create("소녀들의 위치를 파악하고 있습니다."), kMyLocalKey_titleTempScript1);
	ko->setObject(CCString::create("소녀들의 의상을 점검하고 있습니다."), kMyLocalKey_titleTempScript2);
	ko->setObject(CCString::create("소녀들을 불러모으고 있습니다."), kMyLocalKey_titleTempScript3);
	ko->setObject(CCString::create("하트 공짜"), kMyLocalKey_heartFree);
	
	ko->setObject(CCString::create("지난주 랭킹 결과"), kMyLocalKey_rankRewardTitle);
	ko->setObject(CCString::create("보상선물들은 선물함에서 확인하세요."), kMyLocalKey_rankRewardContent);
	ko->setObject(CCString::create("주간 누적 랭킹"), kMyLocalKey_rankRewardStageTitle);
	ko->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardStageScore);
	ko->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardStageRank);
	ko->setObject(CCString::create("지난주 플레이횟수"), kMyLocalKey_rankRewardStagePlayCount);
	ko->setObject(CCString::create("PvP 랭킹"), kMyLocalKey_rankRewardEndlessTitle);
	ko->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardEndlessScore);
	ko->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardEndlessRank);
	ko->setObject(CCString::create("지난주 연승기록"), kMyLocalKey_rankRewardEndlessVictory);
	ko->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardStageReward);
	ko->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardEndlessReward);
	
	ko->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardStageRankValue);
	ko->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardEndlessRankValue);
	
	ko->setObject(CCString::create("%s연승"), kMyLocalKey_rankRewardEndlessVictoryScript);
	ko->setObject(CCString::create("%d위"), kMyLocalKey_rankRewardRankValue);
	
	ko->setObject(CCString::create("%s골드"), kMyLocalKey_rankRewardGoodsTypeGold);
	ko->setObject(CCString::create("%s젬"), kMyLocalKey_rankRewardGoodsTypeRuby);
	ko->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem9);
	ko->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem6);
	ko->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem11);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass1);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass2);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass3);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass4);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass5);
	ko->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass6);
	ko->setObject(CCString::create("선물상자"), kMyLocalKey_rankRewardGoodsTypeMany);
	
	ko->setObject(CCString::create("카드 승급 이벤트!"), kMyLocalKey_rankUpEventTitle);
	ko->setObject(CCString::create("승급 비용 공짜! 승급 확률 100%!!"), kMyLocalKey_rankUpEventContent);
	
	ko->setObject(CCString::create("아니오"), kMyLocalKey_no);
	ko->setObject(CCString::create("네"), kMyLocalKey_yes);
	ko->setObject(CCString::create("<font>기본 클리어조건 <font color=961 newline=17>%.0f%%를<font color=961>%.0f%%로 하향<font newline=17> 시켜드립니다.<font newline=17>적용하시겠습니까?"), kMyLocalKey_clearTimeEventMent);
	ko->setObject(CCString::create("이벤트"), kMyLocalKey_clearTimeEventTitle);
	
	ko->setObject(CCString::create("숨겨진 업적"), kMyLocalKey_achieveHiddenTitle);
	ko->setObject(CCString::create("저를 찾아주세요! 곳곳에 숨겨져 있어요!"), kMyLocalKey_achieveHiddenContent);
	
	ko->setObject(CCString::create("<font newline=20>액자를 선택하면<font color=961 newline=20>퍼즐화면으로 이동합니다."), kMyLocalKey_mainflowDimmed1);
	ko->setObject(CCString::create("<font newline=16>획득한<font color=999>별갯수"), kMyLocalKey_mainflowDimmed2);
	ko->setObject(CCString::create("<font newline=20>선물함"), kMyLocalKey_mainflowDimmed3);
	ko->setObject(CCString::create("<font newline=20>업적"), kMyLocalKey_mainflowDimmed4);
	ko->setObject(CCString::create("<font newline=20>설정"), kMyLocalKey_mainflowDimmed5);
	ko->setObject(CCString::create("<font newline=20>이벤트"), kMyLocalKey_mainflowDimmed6);
	ko->setObject(CCString::create("<font newline=20>랭킹"), kMyLocalKey_mainflowDimmed7);
	ko->setObject(CCString::create("<font newline=20>상점"), kMyLocalKey_mainflowDimmed8);
	ko->setObject(CCString::create("<font newline=20>카드"), kMyLocalKey_mainflowDimmed9);
	ko->setObject(CCString::create("<font newline=16>오늘의<font newline=16>미션"), kMyLocalKey_mainflowDimmed10);
	
	ko->setObject(CCString::create("모든 피스 획득!"), kMyLocalKey_puzzleClearCardTitle);
	ko->setObject(CCString::create("<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzleClearCardContent);
	ko->setObject(CCString::create("모든 카드 획득!"), kMyLocalKey_puzzlePerfectCardTitle);
	ko->setObject(CCString::create("<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzlePerfectCardContent);
	
	ko->setObject(CCString::create("%d연승 돌파!"), kMyLocalKey_endlessSeqWinRewardTitle);
	ko->setObject(CCString::create("%d연승을 축하드립니다!"), kMyLocalKey_endlessSeqWinRewardContent);
	ko->setObject(CCString::create("<font newline=15>%d골드를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeGold);
	ko->setObject(CCString::create("<font newline=15>%d젬을 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby);
	ko->setObject(CCString::create("<font newline=15>기본속도업아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9);
	ko->setObject(CCString::create("<font newline=15>더블아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6);
	ko->setObject(CCString::create("<font newline=15>자석아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11);
	ko->setObject(CCString::create("<font newline=15>이어하기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass1);
	ko->setObject(CCString::create("<font newline=15>맵가챠 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass2);
	ko->setObject(CCString::create("<font newline=15>업그레이드 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass3);
	ko->setObject(CCString::create("<font newline=15>아이템뽑기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass4);
	ko->setObject(CCString::create("<font newline=15>1프로찬스 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass5);
	ko->setObject(CCString::create("<font newline=15>생명의 돌을 %d개를 드립니다.<font newline=15>생명의 돌은 내카드 다이어리에서 사용하세요."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass6);
	ko->setObject(CCString::create("<font newline=15>선물상자를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeMany);
	
	ko->setObject(CCString::create("퍼즐에서 획득한 카드입니다."), kMyLocalKey_cardSettingMyCardContent);
	ko->setObject(CCString::create("스페셜 카드 보기"), kMyLocalKey_cardSettingMyCardToSpecial);
	
	ko->setObject(CCString::create("모든 피스\n획득시!"), kMyLocalKey_cardSettingClearCardMent);
	ko->setObject(CCString::create("퍼펙트\n클리어시!"), kMyLocalKey_cardSettingPerfectCardMent);
	
	ko->setObject(CCString::create("스페셜 카드"), kMyLocalKey_cardSettingSpecialCardTitle);
	ko->setObject(CCString::create("이벤트를 통해 얻을 수 있는 카드입니다."), kMyLocalKey_cardSettingSpecialCardContent);
	ko->setObject(CCString::create("내 카드 보기"), kMyLocalKey_cardSettingSpecialCardToMyCard);
	
	ko->setObject(CCString::create("<font newline=16>PVP모드가<font newline=16>오픈되었어요!"), kMyLocalKey_endlessTutorialMent1);
	ko->setObject(CCString::create("<font newline=16>내 정보 입니다."), kMyLocalKey_endlessTutorialMent2);
	ko->setObject(CCString::create("<font>목록에서 <font color=961>다른 친구를 선택<font newline=16>해보세요.<font newline=16>다른 친구의 정보를 볼 수 있어요."), kMyLocalKey_endlessTutorialMent3);
	ko->setObject(CCString::create("<font color=961>대전준비를 눌러보세요."), kMyLocalKey_endlessTutorialMent4);
	ko->setObject(CCString::create("<font color=961>대전시작을 눌러보세요."), kMyLocalKey_endlessTutorialMent5);
	ko->setObject(CCString::create("<font color=961>내 미니맵"), kMyLocalKey_endlessTutorialMent6);
	ko->setObject(CCString::create("<font color=961>내 점수"), kMyLocalKey_endlessTutorialMent7);
	ko->setObject(CCString::create("<font color=961>내 장착아이템"), kMyLocalKey_endlessTutorialMent8);
	ko->setObject(CCString::create("<font color=961>상대 정보"), kMyLocalKey_endlessTutorialMent9);
	ko->setObject(CCString::create("<font color=961 newline=16>5콤보 달성시 마다<font color=961 newline=16>미사일을 발사하여<font color=961 newline=16>상대의 점수를 깎습니다."), kMyLocalKey_endlessTutorialMent10);
	ko->setObject(CCString::create("화면을 터치하면 게임 진행됩니다."), kMyLocalKey_endlessTutorialMent11);
	ko->setObject(CCString::create("<font color=961>나의 결과"), kMyLocalKey_endlessTutorialMent12);
	ko->setObject(CCString::create("<font color=961>상대방 결과"), kMyLocalKey_endlessTutorialMent13);
	ko->setObject(CCString::create("<font>스크롤하면 점수 내역을 볼 수 있어요."), kMyLocalKey_endlessTutorialMent14);
	ko->setObject(CCString::create("<font color=961>2연승에 도전할 수 있어요!"), kMyLocalKey_endlessTutorialMent15);
	ko->setObject(CCString::create("잠깐!"), kMyLocalKey_endlessTutorialMent16);
	ko->setObject(CCString::create("3연승을 할 때마다 생명의 돌을 1개\n받을 수 있습니다. 생명의 돌은 내가 가진\n카드에 생명을 불어넣을 수 있는\n특별한 아이템이니 소중히 사용해주세요.\n튜토리얼을 마치겠습니다. 그럼 전 이만!"), kMyLocalKey_endlessTutorialMent17);
	ko->setObject(CCString::create("<font>대전 상대를 선택해주세요."), kMyLocalKey_endlessTutorialMent18);
	
	ko->setObject(CCString::create("에피소드 %d"), kMyLocalKey_puzzleValue);
	
	ko->setObject(CCString::create("보너스 영역 뽑기"), kMyLocalKey_onePercentTutorial1);
	ko->setObject(CCString::create("<font color=#FFFFFF size=10 newline=12>안녕하세요!</font>"
								   "<font color=#FFFFFF size=10 newline=12>난 카츠라기라고 해요.</font>"
								   "<font color=#FFFFFF size=10 newline=12>아슬아슬하게 영역을</font>"
								   "<font color=#FFFFFF size=10 newline=24>100% 획득 못했군요.</font>"
								   "<font color=#FFFFFF size=10 newline=12>100% 영역획득 할 수 있게</font>"
									 "<font color=#FFFFFF size=10>내가 도와줄게요.</font>"), kMyLocalKey_onePercentTutorial2);
	ko->setObject(CCString::create("다  음"), kMyLocalKey_onePercentTutorial3);
	ko->setObject(CCString::create("<font color=#FFFFFF size=10 newline=14>아래 게이지바에서 </font>"
								   "<font color=#FFFFFF size=10 newline=14>노란색 영역을 맞히면 퍼센트를</font>"
								   "<font color=#FFFFFF size=10 newline=28>추가로 획득할 수 있어요.</font>"
								   "<font color=#FFFFFF size=10 newline=14>기회는 총 3번 이에요!</font>"
								   "<font color=#FFFFFF size=10 newline=14>100% 에 도전해 볼까요?</font>"), kMyLocalKey_onePercentTutorial4);
	
	ko->setObject(CCString::create("<font color=#FFAA14 size=11 newline=16>뽑기 기회는 단 3번!!</font><font color=#FFFFFF size=11>부족한 영역을 채워 100%에 도전하세요.</font>"), kMyLocalKey_onePercentGame1);
	ko->setObject(CCString::create("<font color=#FFAA14 size=11 newline=16>100%에 성공시</font><font color=#FFAA14 size=11 tag=1>한 단계 높은 카드</font><font color=#FFFFFF size=11>를 획득할 수 있어요.</font>"), kMyLocalKey_onePercentGame2);
	ko->setObject(CCString::create("<font color=#FFFFFF size=13>현재 획득 영역</font><font color=#FFFF00 size=35 strokecolor=000 strokesize=2 tag=1>99.9%</font>"), kMyLocalKey_onePercentGame3);
	ko->setObject(CCString::create("<font size=12 color=#FFFFFF newline=16>포인터가 노란색 영역에</font>"
								   "<font size=12 color=#FFFFFF newline=16>들어왔을 때</font><font size=12 color=#FFFFFF>멈춰 버튼을 눌러주세요!</font>"), kMyLocalKey_onePercentGame4);
	ko->setObject(CCString::create("멈춰!"), kMyLocalKey_onePercentGame5);
	ko->setObject(CCString::create("100% 실패"), kMyLocalKey_onePercentGame6);
	ko->setObject(CCString::create("<font color=#FF0000 size=12>최종 획득 영역     </font>"
								   "<font color=#FF0000 size=30>%.1f%%</font>"), kMyLocalKey_onePercentGame7);
	ko->setObject(CCString::create("닫기"), kMyLocalKey_onePercentGame8);
	ko->setObject(CCString::create("100% 달성!!"), kMyLocalKey_onePercentGame9);
	ko->setObject(CCString::create("카드 받기"), kMyLocalKey_onePercentGame10);
	
	ko->setObject(CCString::create("보상내용"), kMyLocalKey_endlessRewardInfo);
	
	ko->setObject(CCString::create("<font color=#FFFFFF size=12>가위 바위 보 중</font>"
								   "<font color=#FFAA14 size=12>하나를 선택 해 주세요.</font>"), kMyLocalKey_gababoContent1);
	ko->setObject(CCString::create("<font size=12>연승보상</font>"), kMyLocalKey_gababoContent2);
	ko->setObject(CCString::create("보상"), kMyLocalKey_gababoContent3);
	ko->setObject(CCString::create("1승"), kMyLocalKey_gababoContent4);
	ko->setObject(CCString::create("2승"), kMyLocalKey_gababoContent5);
	ko->setObject(CCString::create("3승"), kMyLocalKey_gababoContent6);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent7);
	ko->setObject(CCString::create("확 인"), kMyLocalKey_gababoContent8);
	ko->setObject(CCString::create("나"), kMyLocalKey_gababoContent9);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런, 무승부네요!</font>"
								   "<font color=#FFAA14 size=12>한번 더!</font>"), kMyLocalKey_gababoContent10);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent11);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 4판 모두 이기다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>최고의 보상을 드릴게요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>보너스 게임은 퍼펙트 클리어시마다</font>"
								   "<font color=#FFAA14 size=12 newline=14>할 수 있어요!</font><font color=#FFFFFF size=12>그럼 다음에 또 만나요!</font>"), kMyLocalKey_gababoContent12);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런이런... 지고 말았네요.</font>"
																 "<font color=#FFFFFF size=12>너무 상심마세요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>보너스게임은</font>"
																 "<font color=#FFAA14 size=12 newline=14>퍼펙트 클리어시마다 할 수 있어요!</font>"
																 "<font color=#FFFFFF size=12>다음엔 꼭 저를 이겨보세요!</font>"), kMyLocalKey_gababoContent13);
	ko->setObject(CCString::create("루우비~!"), kMyLocalKey_gababoContent14);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>보너스 게임에</font>"
																 "<font color=#FFFFFF size=12 newline=14>오신 것을 환영합니다.</font>"
																 "<font color=#FFFFFF size=12 newline=14>전 히바리라고 해요.</font>"
																 "<font color=#FFFFFF size=12 newline=14>벌써 퍼즐을 퍼펙트 클리어</font>"
																 "<font color=#FFFFFF size=12 newline=28>하시다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>게임 설명은 최초 1회만</font>"
																 "<font color=#FFAA14 size=12> 해드리니 잘 봐주세요!</font>"
																 ), kMyLocalKey_gababoContent15);
	ko->setObject(CCString::create("다 음"), kMyLocalKey_gababoContent16);
	ko->setObject(CCString::create("<font color=#FFFFFF size=12 newline=43>게임은 가위바위보 입니다!</font>"
								   "<font color=#FFAA14 size=12 newline=14>버튼 3개중 하나를 선택하여</font>"
																 "<font color=#FFAA14 size=12 newline=14>저에게 이기시면 됩니다.</font>"
																 "<font color=#FFAA14 size=12 newline=14>연승 횟수에 따라</font>"
								   "<font color=#FFAA14 size=12 newline=14>더 큰 보상이 있어요!</font>"
								   "<font color=#FFFFFF size=12 newline=14>그럼 행운을 빌어요!</font>"), kMyLocalKey_gababoContent17);
	
	ko->setObject(CCString::create("+%d"), kMyLocalKey_gababoReward);
	
	ko->setObject(CCString::create("PVP 튜토리얼 중에는 나가실 수 없습니다."), kMyLocalKey_notGohomeEndlessTutorialMent);
	ko->setObject(CCString::create("PVP 중에는 재시도할 수 없습니다."), kMyLocalKey_notReplayEndlessMent);
	
	ko->setObject(CCString::create("대중교통 모드"), kMyLocalKey_optionSecretTitle);
	ko->setObject(CCString::create("<font newline=16>대중교통모드가 적용중입니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 해제하실 수 있습니다."), kMyLocalKey_optionSecretOn);
	ko->setObject(CCString::create("<font newline=16>대중교통모드가 풀렸습니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 적용하실 수 있습니다."), kMyLocalKey_optionSecretOff);
	
	ko->setObject(CCString::create("카드 선물하기"), kMyLocalKey_cardGiftTitle);
	ko->setObject(CCString::create("<font color=999>내 아이디는 <font color=961>%s<font color=999> 입니다."), kMyLocalKey_cardGiftMyID);
	ko->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_cardGiftInputText);
	ko->setObject(CCString::create("검색"), kMyLocalKey_cardGiftSearch);
	ko->setObject(CCString::create("선물하기"), kMyLocalKey_cardGiftSend);
	ko->setObject(CCString::create("검색결과가 없습니다."), kMyLocalKey_cardGiftNotFound);
	
	ko->setObject(CCString::create("<font color=999>실패시 다음번 승급 성공확률이 <font color=961>%.0f%%<font color=999> 올라갑니다."), kMyLocalKey_rankUpFailRateUp);
	ko->setObject(CCString::create("<font color=999>이벤트를 놓치지 마세요!!"), kMyLocalKey_rankUpEventSubMent);
	
	ko->setObject(CCString::create("타임 오버"), kMyLocalKey_failTitleTimeover);
	ko->setObject(CCString::create("미션 실패"), kMyLocalKey_failTitleMissionfail);
	ko->setObject(CCString::create("게임 오버"), kMyLocalKey_failTitleGameover);
	ko->setObject(CCString::create("스테이지 클리어"), kMyLocalKey_clearTitle);
	
	ko->setObject(CCString::create("모든 퍼즐 클리어"), kMyLocalKey_allPuzzleClearTitle);
	ko->setObject(CCString::create("<font newline=15>마지막 퍼즐을 클리어하셨습니다.<font newline=15>다음 퍼즐은 곧 업데이트 됩니다.<font newline=15>아직 획득하지 못한 카드들을<font>모두 모아보세요."), kMyLocalKey_allPuzzleClearMent);
	
	ko->setObject(CCString::create("주의!"), kMyLocalKey_warningDie);
	ko->setObject(CCString::create("쿠폰 번호를 입력해주세요."), kMyLocalKey_couponMent);
	ko->setObject(CCString::create("내 정보"), kMyLocalKey_nickTitle);
	ko->setObject(CCString::create("추가지급"), kMyLocalKey_plusGive);
	
	ko->setObject(CCString::create("정보"), kMyLocalKey_pvpInfo);
	ko->setObject(CCString::create("보상내용"), kMyLocalKey_pvpReward);
	
	ko->setObject(CCString::create("선택"), kMyLocalKey_choice);
	ko->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_stageImgLoadFail);
	
	ko->setObject(CCString::create("카드선물"), kMyLocalKey_cardGiftSuccessTitle);
	ko->setObject(CCString::create("카드를 선물하였습니다."), kMyLocalKey_cardGiftSuccessContent);
	ko->setObject(CCString::create("프로필"), kMyLocalKey_profile);
	ko->setObject(CCString::create("카드 속성"), kMyLocalKey_cardElemental);
	
	ko->setObject(CCString::create("터치"), kMyLocalKey_touch);
	ko->setObject(CCString::create("사운드"), kMyLocalKey_sound);
	ko->setObject(CCString::create("속성(%s)"), kMyLocalKey_elementalValue);
	
	ko->setObject(CCString::create("내카드수 "), kMyLocalKey_myCardCount);
	ko->setObject(CCString::create("다이어리 보기"), kMyLocalKey_diaryView);
	
	ko->setObject(CCString::create("보유장수 %d"), kMyLocalKey_cardTakeCnt);
	ko->setObject(CCString::create("<font newline=20>땅을 획득하려면<font color=961 newline=20>꾸욱 버튼을 누른상태에서<font newline=20>조이스틱을 움직여주세요!"), kMyLocalKey_buttonTutorialMent);
	
	ko->setObject(CCString::create("\"한 번 입력한 닉네임은 변경할 수 없으니 신중히 선택해 주세요!\""), kMyLocalKey_nickBottomMent);
	ko->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_pleaseInputID);
	ko->setObject(CCString::create("유효하지 않은 아이디 입니다."), kMyLocalKey_invalidID);
	ko->setObject(CCString::create("다음"), kMyLocalKey_tutorialNextButton);
	
	ko->setObject(CCString::create("<font newline=16>선택된</font><font>스테이지</font>"), kMyLocalKey_puzzleDimmed1);
	ko->setObject(CCString::create("<font>스테이지</font>"), kMyLocalKey_puzzleDimmed2);
	ko->setObject(CCString::create("<font newline=16>스테이지</font><font>클리어 정보</font>"), kMyLocalKey_puzzleDimmed3);
	ko->setObject(CCString::create("<font>게임준비</font>"), kMyLocalKey_puzzleDimmed4);
	
	ko->setObject(CCString::create("<font>클리어 조건</font>"), kMyLocalKey_stageSettingDimmed1);
	ko->setObject(CCString::create("<font>시작버튼</font>"), kMyLocalKey_stageSettingDimmed2);
	
	ko->setObject(CCString::create("다시보지않기"), kMyLocalKey_noReview);
	ko->setObject(CCString::create("젬 100개"), kMyLocalKey_gem100);
	
	ko->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>PvP랭킹은 연승이 중요합니다.</font>"), kMyLocalKey_endlessOpeningMarquee1);
	ko->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>연승수가 같은 경우 누적점수로 랭킹이 결정됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee2);
	ko->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>PvP점수는 주간점수 랭킹에 추가되지 않습니다.</font>"), kMyLocalKey_endlessOpeningMarquee3);
	ko->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>한 주간의 기록 중 가장 높은 연승기록으로 랭킹이 결정됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee4);
	ko->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>랭킹은 매주 월요일 5시에 초기화 됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee5);
	ko->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>이번주 플레이 누적 점수입니다.</font>"), kMyLocalKey_rankNewMarquee1);
	ko->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>PvP점수는 합산되지 않습니다.</font>"), kMyLocalKey_rankNewMarquee2);
	ko->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>주간랭킹은 매주 월요일 5시에 초기화 됩니다.</font>"), kMyLocalKey_rankNewMarquee3);
	
	ko->setObject(CCString::create("드디어 오늘부터 닌자훈련을 시작하는구나!!\n할아버지의 명성에 걸 맞는\n훌륭한 닌자가 될꺼야!"), kMyLocalKey_scenarioMent1);
	ko->setObject(CCString::create("여기가 훈련장인가?\n인기척도 없고 전력으로 훈련할 수 있겠는데?!"), kMyLocalKey_scenarioMent2);
	ko->setObject(CCString::create("안녕하세요.\n저는 한조국립대 학생 리더인 이카루가입니다."), kMyLocalKey_scenarioMent3);
	ko->setObject(CCString::create("앗 안녕하세요.\n저는 아스카 라고합니다."), kMyLocalKey_scenarioMent4);
	ko->setObject(CCString::create("네. 전설의 닌자 한조님의 손녀라고\n들었습니다. 잘 부탁 드립니다.\n훈련 전에 선생님께서 간단한 시험을\n하신다는군요."), kMyLocalKey_scenarioMent5);
	ko->setObject(CCString::create("으윽, 시험은 그다지 좋아하지 않지만\n알겠습니다."), kMyLocalKey_scenarioMent6);
	ko->setObject(CCString::create("어떤 시험인가요?"), kMyLocalKey_scenarioMent7);
	ko->setObject(CCString::create("시노비 결계 속에서 대전하는 법을\n익히시면 됩니다."), kMyLocalKey_scenarioMent8);
	ko->setObject(CCString::create("네. 그러면 잘 부탁 드리겠습니다."), kMyLocalKey_scenarioMent9);
	ko->setObject(CCString::create("그전에 간단히 메뉴들을 소개해 드릴께요."), kMyLocalKey_scenarioMent10);
	ko->setObject(CCString::create("숙지하겠습니다."), kMyLocalKey_scenarioMent11);
	ko->setObject(CCString::create("그럼 시험장으로 이동 하죠.\n1번액자로 입장합니다."), kMyLocalKey_scenarioMent12);
	ko->setObject(CCString::create("이곳은 스테이지를 선택할 수 있는\n퍼즐화면입니다."), kMyLocalKey_scenarioMent13);
	ko->setObject(CCString::create("먼저 1스테이지로 이동하시죠."), kMyLocalKey_scenarioMent14);
	ko->setObject(CCString::create("이곳은 게임시작 전 아이템이나\n무기를 업그레이드 시킬 수 있는\n게임시작화면입니다."), kMyLocalKey_scenarioMent15);
	ko->setObject(CCString::create("게임을 시작하려면 하트가 필요합니다."), kMyLocalKey_scenarioMent16);
	ko->setObject(CCString::create("지금은 하트가 충분하니\n바로 시작해볼까요?"), kMyLocalKey_scenarioMent17);
	ko->setObject(CCString::create("이런곳은 처음인데 여긴 어디지?"), kMyLocalKey_scenarioMent18);
	ko->setObject(CCString::create("시노비 결계 속 입니다.\n일반인의 안전을 위해 닌자들은\n이곳에서 훈련하죠."), kMyLocalKey_scenarioMent19);
	ko->setObject(CCString::create("이곳은 처음이실테니\n컨트롤 방법부터 설명 드리겠습니다."), kMyLocalKey_scenarioMent20);
	ko->setObject(CCString::create("제한시간 내에 달성도 85%를 넘기면\n시험 합격입니다.\n그럼 시작해볼까요?"), kMyLocalKey_scenarioMent21);
	ko->setObject(CCString::create("잠깐!\n깜빡하고 지나갈 뻔했네요."), kMyLocalKey_scenarioMent22);
	ko->setObject(CCString::create("시험을 잘 볼 수 있게 도와드릴게요.\n \n왼쪽에 보이는 것이 지금 플레이하고 있는\n스테이지의 미니맵이예요.\n여기서 보스와 나의 위치를 파악하면 되겠죠?"), kMyLocalKey_scenarioMent23);
	ko->setObject(CCString::create("게임을 잠시 멈추거나 게임을 나가고 싶다면\n일시정지 버튼을 눌러주세요.\n일시정지에는 유용한 기능들이 있으니\n한번 확인해보세요."), kMyLocalKey_scenarioMent24);
	ko->setObject(CCString::create("여기에 점수와 획득한 골드, 그리고 콤보가\n표시됩니다. 자 이제 시작해 볼까요?"), kMyLocalKey_scenarioMent25);
	ko->setObject(CCString::create("축하합니다. 첫 번째 시험을 통과하셨군요.\n게임이 끝나면 이곳으로 오게 됩니다."), kMyLocalKey_scenarioMent26);
	ko->setObject(CCString::create("스테이지 랭킹"), kMyLocalKey_scenarioMent27);
	ko->setObject(CCString::create("점수 정보"), kMyLocalKey_scenarioMent28);
	ko->setObject(CCString::create("클리어 등급"), kMyLocalKey_scenarioMent29);
	ko->setObject(CCString::create("그럼 계속 해볼까요?\n5스테이지 클리어 후 메인화면에서\n뵙기로 하죠!"), kMyLocalKey_scenarioMent30);
	ko->setObject(CCString::create("네! 그럼 최선을 다 해보겠습니다!"), kMyLocalKey_scenarioMent31);
	ko->setObject(CCString::create("이런.. 다음엔 더 잘 할 수 있겠죠?\n여긴 게임이 끝나면 오게 됩니다."), kMyLocalKey_scenarioMent32);
	ko->setObject(CCString::create("휴.. 이걸로 시험끝!!\n드디어 훈련시작이네~!"), kMyLocalKey_scenarioMent33);
	ko->setObject(CCString::create("어랏. 아스카도 한조국립대에서 훈련하는거야?\n같이 최고의 닌자를 꿈꿀 수 있겠구나!!"), kMyLocalKey_scenarioMent34);
	ko->setObject(CCString::create("쉿. 일반 학생들이 다니는 교정 안에서는\n닌자에 관한 이야기는 하지 않는게 좋아.\n이곳의 닌자 양성 클래스는\n일반 학생들에겐 비밀이니까!"), kMyLocalKey_scenarioMent35);
	ko->setObject(CCString::create("흐응... 비밀이라니...\n조금 긴장하게 되.."), kMyLocalKey_scenarioMent36);
	ko->setObject(CCString::create("히바리, 긴장할 필요 까진 없어.\n언제나처럼 시노비결계 안에서 훈련하기\n때문에 보통 사람들은 볼 수 없으니까."), kMyLocalKey_scenarioMent37);
	ko->setObject(CCString::create("응, 알고는 있는데... 아스카짱 고마워\n앞으로나 완전 열심히 할꺼야!"), kMyLocalKey_scenarioMent38);
	ko->setObject(CCString::create("히바리 위험해!!!\n긴장을 늦추지 마. 적은 남아 있다구!!"), kMyLocalKey_scenarioMent39);
	ko->setObject(CCString::create("야규! 히바리를 도와주었구나!"), kMyLocalKey_scenarioMent40);
	ko->setObject(CCString::create("어... 어떻게 라는 건 잘 모르겠고.\n교내에도 아직 적이 남아 있으니\n주변의 적을 해치우자."), kMyLocalKey_scenarioMent41);
	ko->setObject(CCString::create("야규, 아까는 도와줘서 고마워.\n히바리가 거치적거리니까\n이대로면 모두한테 폐를 끼치게 돼."), kMyLocalKey_scenarioMent42);
	ko->setObject(CCString::create("히바리는 거치적 거리는 존재 따위가 아니야.\n우리들은 모두 강하다고 인정받았으니까.\n히바리의 강한 점은 내가 제일 잘 알고있고,\n또 모두들 알고 있어.\n그러니까 더 자신에게 자신감을 가져."), kMyLocalKey_scenarioMent43);
	ko->setObject(CCString::create("야규.."), kMyLocalKey_scenarioMent44);
	ko->setObject(CCString::create("내.. 내가 말하고 싶은 건 그것뿐이야..\n히바리, 내일은 히바리가 좋아하는\n우사네 찻집에 가자."), kMyLocalKey_scenarioMent45);
	ko->setObject(CCString::create("응!"), kMyLocalKey_scenarioMent46);
	ko->setObject(CCString::create("이카루가.. 그리고\n야규, 아스카, 히바리도 마침 딱 있었네!"), kMyLocalKey_scenarioMent47);
	ko->setObject(CCString::create("카츠라기님. 오늘 훈련은 끝났는데\n집에 안가세요?"), kMyLocalKey_scenarioMent48);
	ko->setObject(CCString::create("어, 응..음... 뭐라고 할까\n소화불량이라고나 할까...\n어쨌든 좀 더 몸을 움직이려고... 응?"), kMyLocalKey_scenarioMent49);
	ko->setObject(CCString::create("아앗! 저건 뭐지?"), kMyLocalKey_scenarioMent50);
	ko->setObject(CCString::create("소매치기예요!! 꼬마의 물건을 훔쳐가네요!!"), kMyLocalKey_scenarioMent51);
	ko->setObject(CCString::create("아이의 즐거움을 뺏는\n배짱 좋은 녀석이군.\n저런 썩은 자식은 우리들이 혼내줄테다!!"), kMyLocalKey_scenarioMent52);
	ko->setObject(CCString::create("우와! 실력이 일취월장인데요?\n벌써 실력이 많이 늘었네요.\n이젠 다른사람과 대전을 해볼까요?\n \n따라오세요."), kMyLocalKey_scenarioMent53);
	ko->setObject(CCString::create("이건 내 정보예요.\n보상내용도 나중에 확인해보세요."), kMyLocalKey_scenarioMent54);
	ko->setObject(CCString::create("다른 친구를 누르면 해당친구의 정보를\n볼 수 있어요.\n \nPVP를 시작해보죠."), kMyLocalKey_scenarioMent55);
	ko->setObject(CCString::create("제가 당신과 붙을 만한 상대 세분을\n골라봤어요.\n \n셋중 한명을 선택해주세요."), kMyLocalKey_scenarioMent56);
	ko->setObject(CCString::create("아아아!! 죄송해요!!\n평소 대전과 달라서 설명을 드려야하는걸\n제가 까먹었네요."), kMyLocalKey_scenarioMent57);
	ko->setObject(CCString::create("폭탄이예요! 5콤보시마다 미사일을\n발사해서 상대방의 점수를 깎을 수 있어요!\n조심하세요! 상대방이 절 공격 못하게!\n영역을 획득할 때 발사되는 미사일이\n보스를 타격하게 되면 콤보가 쌓이게 되고,\n콤보점수가 추가됩니다."), kMyLocalKey_scenarioMent58);
	ko->setObject(CCString::create("행운을 빌어요."), kMyLocalKey_scenarioMent59);
	ko->setObject(CCString::create("첫 대전이라 떨리시죠?\n \n이기면 보상이 있으니 행운을 빌어요.\n그럼 전 이만.."), kMyLocalKey_scenarioMent60);
	ko->setObject(CCString::create("PVP가 끝나면 이곳으로 와요."), kMyLocalKey_scenarioMent61);
	ko->setObject(CCString::create("모두 게임오버 되었을 때는\n피격점수로 승패를 가릅니다."), kMyLocalKey_scenarioMent62);
	
	ko->setObject(CCString::create("업적 메뉴에서는 게임 중 달성한 업적을\n확인하고 보상을 받을 수 있습니다.\n앞으로 해야 할 업적들이 정말 많군요!"), kMyLocalKey_kindTutorial1);
	ko->setObject(CCString::create("모든보상받기 버튼을 누르면 달성한 업적\n보상을 모두 한번에 받을 수 있어요!"), kMyLocalKey_kindTutorial2);
	ko->setObject(CCString::create("하하! 또 만나네요.\n내카드에서는 스테이지에서 획득한 카드를\n확인할 수 있어요."), kMyLocalKey_kindTutorial3);
	ko->setObject(CCString::create("내가 획득한 카드나 다이어리 보기 버튼을\n터치하면 카드의 상세 내용을 확인할 수\n있는 다이어리로 이동합니다."), kMyLocalKey_kindTutorial4);
	ko->setObject(CCString::create("상단의 정렬버튼을 눌러 여러가지 옵션으로\n정렬기능을 이용할 수도 있습니다."), kMyLocalKey_kindTutorial5);
	ko->setObject(CCString::create("어머! 여기서 또 만나네요?\n이번엔 랭킹에 대해 설명해 드릴께요.\n \n주간랭킹은 일주일 동안 획득한 스테이지\n점수를 합산하여 랭킹이 산정됩니다."), kMyLocalKey_kindTutorial6);
	ko->setObject(CCString::create("순위마다 보상 내용이 다르니\n잘 확인해 주세요."), kMyLocalKey_kindTutorial7);
	ko->setObject(CCString::create("드디어 뽑기상자가 등장했네요."), kMyLocalKey_kindTutorial8);
	ko->setObject(CCString::create("이건 뭐죠? 아이템인가요?"), kMyLocalKey_kindTutorial9);
	ko->setObject(CCString::create("특별한 아이템을 뽑을 수 있는 상자입니다.\n운이 좋으면 목숨 하나가 추가되는\n부활아이템을 얻을 수 있죠."), kMyLocalKey_kindTutorial10);
	ko->setObject(CCString::create("껄끄러운 게임미션이로군.\n히바리. 이번 게임에선 부하몬스터를\n표시된 개수만큼 가둬 잡아야 클리어할 수 있어.\n너라면 충분히 해낼 수 있을꺼야."), kMyLocalKey_kindTutorial11);
	ko->setObject(CCString::create("응! 야규짱!"), kMyLocalKey_kindTutorial12);
	ko->setObject(CCString::create("이번엔 게임미션이 조금 다른 것 같아요."), kMyLocalKey_kindTutorial13);
	ko->setObject(CCString::create("네. 주어진 개수만큼 생성되는 아이템을\n먹어야 클리어 됩니다.\n클리어하기 더 어려워지겠는데요?"), kMyLocalKey_kindTutorial14);
	ko->setObject(CCString::create("게임시간이 엄청나게 줄어들었어요.\n조금 서둘러야겠는걸요."), kMyLocalKey_kindTutorial15);
	ko->setObject(CCString::create("알고있어!! 게임 중에 생기는 시간아이템을\n적절히 먹으면서 플레이하면 문제없을꺼야!!"), kMyLocalKey_kindTutorial16);
	ko->setObject(CCString::create("앗! 이건 뭐죠?"), kMyLocalKey_kindTutorial17);
	ko->setObject(CCString::create("이 신발을 착용하면 게임을\n더 빠른 스피드로 시작할 수 있어요.\n무료로 몇 개 지급되니 사용해보세요."), kMyLocalKey_kindTutorial18);
	ko->setObject(CCString::create("새로운 아이템이 등장했네요."), kMyLocalKey_kindTutorial19);
	ko->setObject(CCString::create("게임 중에 아이템이 두 배로 더 많이\n나오도록 해주는 인법서군요.\n많은 도움이 될겁니다."), kMyLocalKey_kindTutorial20);
	ko->setObject(CCString::create("이건 자석인가?"), kMyLocalKey_kindTutorial21);
	ko->setObject(CCString::create("네. 게임 중에 생기는 골드나 아이템을\n더 쉽게 먹을 수 있도록 도와주는\n아이템입니다."), kMyLocalKey_kindTutorial22);
	
	ko->setObject(CCString::create("최고스피드!"), kMyLocalKey_maxSpeed);
	ko->setObject(CCString::create("쇼타임!"), kMyLocalKey_showtime);
	ko->setObject(CCString::create("미스"), kMyLocalKey_miss);
	ko->setObject(CCString::create("콤보"), kMyLocalKey_combo);
	ko->setObject(CCString::create("잠금이 해제되었습니다!"), kMyLocalKey_unlockedAreaScroll);
	ko->setObject(CCString::create("미션성공!"), kMyLocalKey_missionComplete);
	ko->setObject(CCString::create("체인지를 모으세요!"), kMyLocalKey_collectChange);
	ko->setObject(CCString::create("정식오픈 후 사용할 수 있습니다."), kMyLocalKey_afterOpenCBT);
	ko->setObject(CCString::create("카드 체인지!"), kMyLocalKey_cardChange);
	ko->setObject(CCString::create("100% 쇼타임에서는 생명의 돌을 체험할 수 있어요."), kMyLocalKey_showtimeMorphingTouch);
	ko->setObject(CCString::create("카드강화"), kMyLocalKey_cardStrength);
	ko->setObject(CCString::create("업데이트 예정"), kMyLocalKey_updateTitle);
	ko->setObject(CCString::create("추후 업데이트 됩니다."), kMyLocalKey_updateContent);
	ko->setObject(CCString::create("준비중"), kMyLocalKey_communityNotOpenTitle);
	ko->setObject(CCString::create("준비입니다. 공식카페를 이용해 주세요."), kMyLocalKey_communityNotOpenContent);
	
	ko->setObject(CCString::create("조이스틱\n크기"), kMyLocalKey_joystickSize);
	ko->setObject(CCString::create("대각선\n사용"), kMyLocalKey_useSideDirection);
	ko->setObject(CCString::create("19+ 다이어리"), kMyLocalKey_toDiary19);
	
	ko->setObject(CCString::create("19+ 다이어리"), kMyLocalKey_toDiary19Title);
	ko->setObject(CCString::create("<font size=12 newline=15>보조앱(19+ 다이어리)이 없네요.</font><font size=12 newline=15>더욱 화끈하고! 쎈! 그녀들이 기다립니다.</font><font size=12>해당앱으로 연결하시겠습니까?</font>"), kMyLocalKey_toDiary19Content);
	ko->setObject(CCString::create("19+ 다이어리 다운 받기"), kMyLocalKey_toDiary19Button);
	ko->setObject(CCString::create("조이스틱\n진동"), kMyLocalKey_joystickVib);
	
	ko->setObject(CCString::create("조이스틱 크기조절"), kMyLocalKey_joystickSizeQuestionTitle);
	ko->setObject(CCString::create("<font newline=17>게임에서 사용되는 조이스틱의 크기를</font><font newline=17>9단계의 크기로 조절하고 있습니다.</font><font color=961>평균 5단계를 사용합니다.</font>"), kMyLocalKey_joystickSizeQuestionContent);
	
	ko->setObject(CCString::create("헬모드"), kMyLocalKey_missionTitle8);
	ko->setObject(CCString::create("목표 달성률"), kMyLocalKey_missionTitle9);
	ko->setObject(CCString::create("목표 점수"), kMyLocalKey_missionTitle10);
	ko->setObject(CCString::create("목표 콤보"), kMyLocalKey_missionTitle11);
	ko->setObject(CCString::create("목표 골드"), kMyLocalKey_missionTitle12);
	ko->setObject(CCString::create("목표 턴수"), kMyLocalKey_missionTitle13);
	
	ko->setObject(CCString::create("<font>오래 버티세요!"), kMyLocalKey_missionDiscription8);
	ko->setObject(CCString::create("<font color=961 size=13>%d%%이상<font color=999 size=13> 획득하라!"), kMyLocalKey_missionDiscription9);
	ko->setObject(CCString::create("<font>높은 점수를 달성하세요!"), kMyLocalKey_missionDiscription10);
	ko->setObject(CCString::create("<font>높은 콤보를 달성하세요!"), kMyLocalKey_missionDiscription11);
	ko->setObject(CCString::create("<font>많은 골드를 획득하세요!"), kMyLocalKey_missionDiscription12);
	ko->setObject(CCString::create("<font>작은 턴으로 게임을 클리어 하세요!"), kMyLocalKey_missionDiscription13);
	
	ko->setObject(CCString::create("<font>오래 버티세요!"), kMyLocalKey_missionCondition8);
	ko->setObject(CCString::create("<font color=961 size=13>%d%%이상<font color=999 size=13> 획득하라!"), kMyLocalKey_missionCondition9);
	ko->setObject(CCString::create("<font color=961>%d점<font color=999>이상으로 클리어하세요!"), kMyLocalKey_missionCondition10);
	ko->setObject(CCString::create("<font color=961>%d콤보<font color=999>를 달성하세요!"), kMyLocalKey_missionCondition11);
	ko->setObject(CCString::create("<font color=961>%d골드<font color=999>를 획득하세요!"), kMyLocalKey_missionCondition12);
	ko->setObject(CCString::create("<font color=961>%d턴<font color=999>안에 게임을 클리어 하세요!"), kMyLocalKey_missionCondition13);
	
	ko->setObject(CCString::create("%d점"), kMyLocalKey_mission10Label);
	ko->setObject(CCString::create("%d콤보"), kMyLocalKey_mission11Label);
	ko->setObject(CCString::create("%d골드"), kMyLocalKey_mission12Label);
	ko->setObject(CCString::create("/%d턴"), kMyLocalKey_mission13Label);
	
	setObject(ko, "ko");
}
void MyLocal::en()
{
	
	CCDictionary* en = CCDictionary::create();
	
	en->setObject(CCString::create("Server Online"), kMyLocalKey_connectingServer);
//	en->setObject(CCString::create("Nickname enter"), kMyLocalKey_inputNick);
	en->setObject(CCString::create("Please enter."), kMyLocalKey_inputPlease);
	en->setObject(CCString::create("OK"), kMyLocalKey_ok);
	en->setObject(CCString::create("Loading images."), kMyLocalKey_downImgInfo);
	en->setObject(CCString::create("Information Retrieval Failure"), kMyLocalKey_failLoadInfo);
	en->setObject(CCString::create("RE-TRY"), kMyLocalKey_replay);
//	en->setObject(CCString::create("Downloading %.0f%% : %d/%d"), kMyLocalKey_downloadingProgress);
	en->setObject(CCString::create("Receive images fail"), kMyLocalKey_downImgFail);
//	en->setObject(CCString::create("Login successful"), kMyLocalKey_successLogin);
	en->setObject(CCString::create("This nickname is already in use."), kMyLocalKey_sameNick);
	en->setObject(CCString::create("Nickname error"), kMyLocalKey_invalidNick);
	en->setObject(CCString::create("Nickname is too long."), kMyLocalKey_longNick);
	en->setObject(CCString::create("Nickname is too short."), kMyLocalKey_shortNick);
//	en->setObject(CCString::create("특수문자는 사용 할 수 없습니다."), kMyLocalKey_specialCharacterError);
	en->setObject(CCString::create("Now Open"), kMyLocalKey_openRightNow);
	en->setObject(CCString::create("RUBY %d open"), kMyLocalKey_openRubyValue);
//	en->setObject(CCString::create("GOLD %d open"), kMyLocalKey_openGoldValue);
	en->setObject(CCString::create("CGP 일반 보상"), kMyLocalKey_cgpNormalTitle);
	en->setObject(CCString::create("Press to receive reward."), kMyLocalKey_cgpNormalContent);
	en->setObject(CCString::create("Receive reward"), kMyLocalKey_getReward);
	en->setObject(CCString::create("take"), kMyLocalKey_take);
	en->setObject(CCString::create("CGP 전체 팝업 보상"), kMyLocalKey_cgpAllPopupTitle);
	en->setObject(CCString::create("Shop"), kMyLocalKey_shop);
	en->setObject(CCString::create("Ranking"), kMyLocalKey_ranking);
	en->setObject(CCString::create("My card"), kMyLocalKey_mycard);
	en->setObject(CCString::create("Today mission"), kMyLocalKey_todaymission);
	en->setObject(CCString::create("Event"), kMyLocalKey_event);
	en->setObject(CCString::create("스페셜 스테이지"), kMyLocalKey_openStage);
	en->setObject(CCString::create("스페셜 스테이지를 오픈하시겠습니까?"), kMyLocalKey_openStageContent);
	en->setObject(CCString::create("오픈하기"), kMyLocalKey_doOpen);
//	en->setObject(CCString::create("Open Stage"), kMyLocalKey_stageOpenTitle);
//	en->setObject(CCString::create("A new stage\nhas been unlocked"), kMyLocalKey_stageOpenContent);
//	en->setObject(CCString::create("Puzzle open"), kMyLocalKey_puzzleOpenTitle);
//	en->setObject(CCString::create("A new puzzle\nhas been unlocked."), kMyLocalKey_puzzleOpenContent);
	en->setObject(CCString::create("View"), kMyLocalKey_view);
	en->setObject(CCString::create("Area 85%"), kMyLocalKey_condition1);
	en->setObject(CCString::create("Area 100%"), kMyLocalKey_condition2);
	en->setObject(CCString::create("Change + Area 85%"), kMyLocalKey_condition3);
	en->setObject(CCString::create("Change + Area 100%"), kMyLocalKey_condition4);
//	en->setObject(CCString::create("85%"), kMyLocalKey_conditionTwoLine1);
//	en->setObject(CCString::create("85%\n+ Change"), kMyLocalKey_conditionTwoLine2);
//	en->setObject(CCString::create("100%"), kMyLocalKey_conditionTwoLine3);
//	en->setObject(CCString::create("100%\n+ Change"), kMyLocalKey_conditionTwoLine4);
	en->setObject(CCString::create("%d Stage"), kMyLocalKey_stageValue);
//	en->setObject(CCString::create("My location %d"), kMyLocalKey_myrankValue);
	en->setObject(CCString::create("Failure Ranking Information"), kMyLocalKey_failCheckRanking);
	en->setObject(CCString::create("Stage"), kMyLocalKey_stage);
	en->setObject(CCString::create("Choose the items that you will use during the game."), kMyLocalKey_selectUseItem);
	en->setObject(CCString::create("Level %d"), kMyLocalKey_levelValue);
	en->setObject(CCString::create("Power %s"), kMyLocalKey_powerValue);
	en->setObject(CCString::create("Level %d Upgrade"), kMyLocalKey_upgradeLevelValue);
	en->setObject(CCString::create("Level %d\nUpgrade"), kMyLocalKey_upgradeLevelValue2);
	en->setObject(CCString::create("Up to three items\ncan be selected."), kMyLocalKey_totalItemSelectCount3);
	en->setObject(CCString::create("You don’t have enough gold"), kMyLocalKey_goldNotEnought);
	en->setObject(CCString::create("You don’t have enough ruby."), kMyLocalKey_rubyNotEnought);
	en->setObject(CCString::create("You don’t have enough heart."), kMyLocalKey_heartNotEnought);
//	en->setObject(CCString::create("Would you like to go buy?"), kMyLocalKey_goToShop);
	en->setObject(CCString::create("The default speed"), kMyLocalKey_baseSpeedUpTitle);
	en->setObject(CCString::create("The user starts with a fast state."), kMyLocalKey_baseSpeedUpContent);
	en->setObject(CCString::create("Double items"), kMyLocalKey_doubleItemTitle);
	en->setObject(CCString::create("Quickly create items."), kMyLocalKey_doubleItemContent);
	en->setObject(CCString::create("자석"), kMyLocalKey_magnetTitle);
	en->setObject(CCString::create("골드와 아이템을 빨아들입니다."), kMyLocalKey_magnetContent);
	en->setObject(CCString::create("Additional time"), kMyLocalKey_longTimeTitle);
	en->setObject(CCString::create("Game time increases."), kMyLocalKey_longTimeContent);
	en->setObject(CCString::create("Items Draw"), kMyLocalKey_itemGachaTitle);
	en->setObject(CCString::create("어떤 아이템이 나올까요?"), kMyLocalKey_itemGachaDefaultContent1);
	en->setObject(CCString::create("뽑기에만 출몰하는 다양한 아이템!!"), kMyLocalKey_itemGachaDefaultContent2);
	en->setObject(CCString::create("Resurrection challenge your items!"), kMyLocalKey_itemGachaContent);
	en->setObject(CCString::create("RUBY SHOP"), kMyLocalKey_rubyShop);
	en->setObject(CCString::create("GOLD SHOP"), kMyLocalKey_goldShop);
	en->setObject(CCString::create("HEART SHOP"), kMyLocalKey_heartShop);
//	en->setObject(CCString::create("구매확인"), kMyLocalKey_checkBuy);
	en->setObject(CCString::create("Do you want to buy?"), kMyLocalKey_realBuy);
	en->setObject(CCString::create("Option"), kMyLocalKey_option);
	en->setObject(CCString::create("Coupon"), kMyLocalKey_regCoupon);
	en->setObject(CCString::create("Community"), kMyLocalKey_community);
//	en->setObject(CCString::create("Gametip"), kMyLocalKey_gametip);
//	en->setObject(CCString::create("Tutorial"), kMyLocalKey_tutorial);
	en->setObject(CCString::create("System"), kMyLocalKey_system);
	en->setObject(CCString::create("Background sound"), kMyLocalKey_bgm);
	en->setObject(CCString::create("Sound Effects"), kMyLocalKey_effect);
	en->setObject(CCString::create("Joystick\nposition"), kMyLocalKey_joystickPosition);
	en->setObject(CCString::create("Operation\nJoystick"), kMyLocalKey_joystickControl);
	en->setObject(CCString::create("Safe Mode"), kMyLocalKey_safetyMode);
	en->setObject(CCString::create("Notify"), kMyLocalKey_noti);
	en->setObject(CCString::create("Push"), kMyLocalKey_pushNoti);
	en->setObject(CCString::create("서버연결에 실패하였습니다."), kMyLocalKey_failedConnect);
	en->setObject(CCString::create("메세지확인"), kMyLocalKey_confirmGift);
	en->setObject(CCString::create("Message\nAlarm"), kMyLocalKey_messageNoti);
//	en->setObject(CCString::create("선물을 받았습니다."), kMyLocalKey_getgift);
	en->setObject(CCString::create("받은 메세지가 없습니다."), kMyLocalKey_nogift);
	en->setObject(CCString::create("*받은 메세지는 30일 이후에 삭제됩니다."), kMyLocalKey_giftboxAlert);
	en->setObject(CCString::create("재접속"), kMyLocalKey_reConnect);
	en->setObject(CCString::create("다른 기기로 연결되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert1);
	en->setObject(CCString::create("서버와의 접속에 오류가 발생하였습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert2);
	en->setObject(CCString::create("세션이 종료되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert3);
	en->setObject(CCString::create("서버연결에 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_reConnectAlert4);
	en->setObject(CCString::create("BLOCKED"), kMyLocalKey_blocked);
	en->setObject(CCString::create("계정이 아래의 사유로 정지 되었습니다."), kMyLocalKey_blockedMsg);
	en->setObject(CCString::create("Member ID : "), kMyLocalKey_memberID);
	en->setObject(CCString::create("Version : "), kMyLocalKey_gameversion);
	en->setObject(CCString::create("Secession"), kMyLocalKey_withdraw);
	en->setObject(CCString::create("Customer Service"), kMyLocalKey_callCenter);
	en->setObject(CCString::create("Are you sure you want to logout?"), kMyLocalKey_checkLogout);
	en->setObject(CCString::create("Cancel"), kMyLocalKey_cancel);
	en->setObject(CCString::create("게임 탈퇴"), kMyLocalKey_withdrawTitle);
	en->setObject(CCString::create("Can not use the room when you leave the game,"), kMyLocalKey_withdrawContent1);
	en->setObject(CCString::create("all data will be deleted permanently."), kMyLocalKey_withdrawContent2);
	en->setObject(CCString::create("Are you sure you want to really leave?"), kMyLocalKey_withdrawContent3);
	en->setObject(CCString::create("탈퇴하기"), kMyLocalKey_doWithdraw);
	en->setObject(CCString::create("Off"), kMyLocalKey_lightOff);
	en->setObject(CCString::create("On"), kMyLocalKey_lightOn);
	en->setObject(CCString::create("Right"), kMyLocalKey_right);
	en->setObject(CCString::create("Left"), kMyLocalKey_left);
	en->setObject(CCString::create("Fixing"), kMyLocalKey_fix);
	en->setObject(CCString::create("Movement"), kMyLocalKey_move);
	en->setObject(CCString::create("Weekly cumulative ranking"), kMyLocalKey_weeklyranking);
	en->setObject(CCString::create("%ddays after the reset"), kMyLocalKey_dayAfterReset);
	en->setObject(CCString::create("%dtimes after the reset"), kMyLocalKey_hourAfterReset);
	en->setObject(CCString::create("%dMinutes after the reset"), kMyLocalKey_secondAfterReset);
	en->setObject(CCString::create("Gift Box"), kMyLocalKey_giftbox);
	en->setObject(CCString::create("<font color=#FFAA14 size=15>%s</font><br><font color=#FFFFFF>- %s -</font><br><font color=#CCCCCC size=10s>%s</font>"), kMyLocalKey_giftboxContent);
	en->setObject(CCString::create("Accept all"), kMyLocalKey_allAccept);
	en->setObject(CCString::create("티켓함"), kMyLocalKey_ticketBox);
	en->setObject(CCString::create("도움함"), kMyLocalKey_helpBox);
	en->setObject(CCString::create("도전함"), kMyLocalKey_challengeBox);
	en->setObject(CCString::create("하트함"), kMyLocalKey_heartBox);
	en->setObject(CCString::create("전체보기"), kMyLocalKey_allView);
//	en->setObject(CCString::create("하트가 도착했어요."), kMyLocalKey_arriveHeart);
//	en->setObject(CCString::create("도전장이 도착했어요."), kMyLocalKey_arriveChallenge);
//	en->setObject(CCString::create("도전을 받아들이시겠습니까?"), kMyLocalKey_checkAcceptChallenge);
//	en->setObject(CCString::create("도망가기"), kMyLocalKey_ttt);
//	en->setObject(CCString::create("도전에 임하는 자세를 가집시다!!\n도망으로 간주, 패하셨습니다."), kMyLocalKey_tttContent);
//	en->setObject(CCString::create("도전결과!!"), kMyLocalKey_resultChallenge);
//	en->setObject(CCString::create("졌습니다. 브레이크 포인트 필요함. 위에 contentObj"), kMyLocalKey_loseContent);
//	en->setObject(CCString::create("도움요청이 도착했어요."), kMyLocalKey_arriveHelp);
//	en->setObject(CCString::create("상대방을 도와줍니다."), kMyLocalKey_acceptHelp);
//	en->setObject(CCString::create("거절"), kMyLocalKey_deny);
//	en->setObject(CCString::create("수락"), kMyLocalKey_accept);
//	en->setObject(CCString::create("상대방의 도움이 왔어요!"), kMyLocalKey_arriveHelped);
//	en->setObject(CCString::create("카드 정보 로딩"), kMyLocalKey_cardInfoLoading);
//	en->setObject(CCString::create("도움을 받았습니다!!"), kMyLocalKey_iHelped);
//	en->setObject(CCString::create("티켓요청이 도착했어요."), kMyLocalKey_arriveNeedTicket);
//	en->setObject(CCString::create("티켓요청이 도착. 티켓은 퍼즐을 열 때 필요합니다. 친구를 도와주세요!!"), kMyLocalKey_arriveNeedTicketContent);
//	en->setObject(CCString::create("보내기"), kMyLocalKey_send);
//	en->setObject(CCString::create("티켓이 왔네요 어서 받으세요."), kMyLocalKey_arriveTicket);
//	en->setObject(CCString::create("티켓이 도착했습니다. 티켓을 받으세요."), kMyLocalKey_arriveTicketContent);
//	en->setObject(CCString::create("티켓받기"), kMyLocalKey_takeTicket);
//	en->setObject(CCString::create("친구추가 요청이 왔습니다."), kMyLocalKey_arriveAddFriend);
//	en->setObject(CCString::create("An error has occurred."), kMyLocalKey_whatError);
//	en->setObject(CCString::create("I can not find the request arrived."), kMyLocalKey_arriveWhatError);
	en->setObject(CCString::create("Default"), kMyLocalKey_defaultSort);
	en->setObject(CCString::create("Take"), kMyLocalKey_takeOrder);
	en->setObject(CCString::create("Rank"), kMyLocalKey_gradeOrder);
//	en->setObject(CCString::create("Rotation"), kMyLocalKey_rotation);
	en->setObject(CCString::create("Raising levels to defeat the boss can do it more easily!"), kMyLocalKey_upgradeSubMent);
	en->setObject(CCString::create("Draw again"), kMyLocalKey_itemRegacha);
	en->setObject(CCString::create("%.0f%% Discount"), kMyLocalKey_itemRegachaDiscountValue);
	en->setObject(CCString::create("Use of this item"), kMyLocalKey_thisItemUse);
//	en->setObject(CCString::create("Time"), kMyLocalKey_time);
//	en->setObject(CCString::create("Gold"), kMyLocalKey_gold);
//	en->setObject(CCString::create("Score"), kMyLocalKey_score);
	en->setObject(CCString::create("총점"), kMyLocalKey_totalScore);
	en->setObject(CCString::create("The next stage"), kMyLocalKey_nextStage);
	en->setObject(CCString::create("AGAIN TO"), kMyLocalKey_regame);
	en->setObject(CCString::create("MAIN"), kMyLocalKey_toMain);
	en->setObject(CCString::create("Please clear\nthe previous stage."), kMyLocalKey_beforeNotClearPuzzle);
	en->setObject(CCString::create("이전액자를\n모두클리어하면\n다음액자가나타납니다."), kMyLocalKey_waitForUpdate);
//	en->setObject(CCString::create("I'll wait!\n My collection\nof pictures of it!"), kMyLocalKey_diaryNoImg);
	en->setObject(CCString::create("Reward"), kMyLocalKey_reward);
	en->setObject(CCString::create("SPEED"), kMyLocalKey_speed);
	en->setObject(CCString::create("READY"), kMyLocalKey_ready);
	en->setObject(CCString::create("상대찾기"), kMyLocalKey_endlessReady);
//	en->setObject(CCString::create("God"), kMyLocalKey_rankA);
//	en->setObject(CCString::create("Hero"), kMyLocalKey_rankB);
//	en->setObject(CCString::create("Normal"), kMyLocalKey_rankC);
//	en->setObject(CCString::create("Monkey"), kMyLocalKey_rankD);
//	en->setObject(CCString::create("Buy"), kMyLocalKey_buy);
	en->setObject(CCString::create("Push"), kMyLocalKey_controlClick);
	en->setObject(CCString::create("Locked"), kMyLocalKey_locked);
//	en->setObject(CCString::create("Please select the box."), kMyLocalKey_selectClearBox);
	
	en->setObject(CCString::create(""), kMyLocalKey_titleLoadingBegin);
	en->setObject(CCString::create("각 스테이지 별로 각기 다른 4장의 이미지를 획득해 보세요."), kMyLocalKey_titleLoading1);
	en->setObject(CCString::create("공공장소나 대중교통 이용시에는 '대중교통 모드'로 Play 하세요. 획득한 이미지는 내카드에서 확인 할 수 있습니다."), kMyLocalKey_titleLoading2);
	en->setObject(CCString::create("파란색 실루엣 영역을 획득해야 % 가 증가해요."), kMyLocalKey_titleLoading3);
	en->setObject(CCString::create("'영역 다시 뽑기' 를 해서 넓은 영역으로 play 를 시작하면 좀 더 쉽게 높은 등급의 카드를 획득하기가 쉬워져요"), kMyLocalKey_titleLoading4);
	en->setObject(CCString::create("'대중교통 모드' 는 play 중에 일시정지 버튼을 탭해서 설정할 수 있습니다."), kMyLocalKey_titleLoading5);
	en->setObject(CCString::create("Clear 등급에 따라 획득되는 별의 개수가 다릅니다. 별이 모이면 다음 액자를 열 수 있어요."), kMyLocalKey_titleLoading7);
	en->setObject(CCString::create("'업적' 창에서 달성한 업적을 보상받으세요. 보석을 드립니다."), kMyLocalKey_titleLoading8);
	en->setObject(CCString::create("일일미션에 도전해 보세요. 일일 미션을 달성하시면 다양한 선물을 드립니다."), kMyLocalKey_titleLoading9);
	en->setObject(CCString::create("주간랭킹에 도전해 보세요. 매주 새로운 선물을 푸짐하게 드립니다."), kMyLocalKey_titleLoading10);
	en->setObject(CCString::create("change 글자를 모아서 특별한 이미지를 확인하세요."), kMyLocalKey_titleLoading11);
	en->setObject(CCString::create("미사일을 업그레이드 하면 강력한 공격력으로 높은 등급 카드 획득이 가능합니다."), kMyLocalKey_titleLoading12);
	en->setObject(CCString::create("아이템뽑기에서는 '부활' 아이템을 뽑을 수 있어요."), kMyLocalKey_titleLoading13);
	en->setObject(CCString::create("스테이지마다 showtime 에 도전해 보세요."), kMyLocalKey_titleLoading14);
	en->setObject(CCString::create("매주 새로운 이벤트가 열립니다. 공식 카페를 확인해 주세요."), kMyLocalKey_titleLoading15);
	en->setObject(CCString::create("게임 내에서는 이름 대신 닉네임을 사용합니다."), kMyLocalKey_titleLoading16);
	en->setObject(CCString::create("99% 라 너무 아쉽다면, 보너스 뽑기에 도전 해 보세요."), kMyLocalKey_titleLoading17);
	en->setObject(CCString::create("이벤트 스테이지에서는 특별한 이미지와 선물이 기다립니다."), kMyLocalKey_titleLoading18);
	en->setObject(CCString::create("스테이지별 미션 clear 가 어렵다면 아이템 뽑기를 통해 미션 전용 아이템을 사용해 보세요."), kMyLocalKey_titleLoading19);
	en->setObject(CCString::create("한 번에 많은 영역을 획득할 수록 여러 개의 미사일이 발사되며, 공격을 성공한 만큼 콤보가 적용됩니다."), kMyLocalKey_titleLoading6);
	en->setObject(CCString::create("이어하기는 두 번 까지만 사용할 수 있습니다."), kMyLocalKey_titleLoading20);
	en->setObject(CCString::create("내카드 목록에서 카드 이미지를 터치하면 카드 상세 내용을 보실 수 있습니다."), kMyLocalKey_titleLoading21);
	en->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading22);
	en->setObject(CCString::create("85% 로 클리어 하면 별 1개를 획득합니다."), kMyLocalKey_titleLoading23);
	en->setObject(CCString::create("change를 모두 모은 후 클리어하면 별 2개를 획득합니다."), kMyLocalKey_titleLoading24);
	en->setObject(CCString::create("100%로 클리어 하면 별 3개를 획득합니다."), kMyLocalKey_titleLoading25);
	en->setObject(CCString::create("change를 모두 모은 후 100%로 클리어하면 별 4개를 모을 수 있습니다."), kMyLocalKey_titleLoading26);
	en->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading27);
	en->setObject(CCString::create("플레이 중 획득한 영역에 따라 골드를 드립니다."), kMyLocalKey_titleLoading28);
	en->setObject(CCString::create("30%나 할인되는 종합 아이템 구매 찬스를 놓치지 마세요!"), kMyLocalKey_titleLoading29);
	en->setObject(CCString::create("미션스테이지에서는 미션을 먼저 완료한 후 클리어가 가능합니다."), kMyLocalKey_titleLoading30);
	en->setObject(CCString::create("미사일 공격 시 그리는 선에 미사일이 닿으면 유도 공격을 당해요."), kMyLocalKey_titleLoading31);
	en->setObject(CCString::create("게임 중 타임 보너스 아이템을 먹으면 남은 시간에서 10초가 추가됩니다."), kMyLocalKey_titleLoading32);
	en->setObject(CCString::create("선을 그리다가 꾸욱 버튼에서 손을 떼면 캐릭터가 안전 영역으로 되돌아 갑니다."), kMyLocalKey_titleLoading33);
	en->setObject(CCString::create("보스에게 연속 공격을 성공시키면 콤보가 적용되며, 게임 결과에서 콤보 보너스를 받을 수 있습니다."), kMyLocalKey_titleLoading34);
	en->setObject(CCString::create("내가 모은 별의 개수는 메인화면의 왼쪽 상단에 있는 별 아이콘을 통해 확인하실 수 있습니다."), kMyLocalKey_titleLoading35);
	en->setObject(CCString::create("운석이 떨어지면 캐릭터에 맞지 않도록 주의하세요."), kMyLocalKey_titleLoading36);
	en->setObject(CCString::create("조이스틱과 꾸욱 버튼의 위치를 바꾸고 싶다면 설정메뉴에서 조이스틱위치를 반대로 변경해 보세요."), kMyLocalKey_titleLoading37);
	en->setObject(CCString::create("조이스틱의 위치를 고정하고 싶다면 설정메뉴에서 조이스틱조작을 고정으로 변경해 보세요"), kMyLocalKey_titleLoading38);
	en->setObject(CCString::create("꾸욱 버튼을 누른채 한쪽 방향으로 그린 다음 되돌아 오면 한줄 그리기가 가능합니다."), kMyLocalKey_titleLoading39);
	en->setObject(CCString::create("그리는 도중 줄이 꼬여도 당황하지 마세요. 줄이 꼬인 상태에서도 영역을 획득할 수 있습니다."), kMyLocalKey_titleLoading40);
	en->setObject(CCString::create("작은 몬스터에는 HP가 존재하며, 공격이 성공할 때마다 HP가 줄어듭니다."), kMyLocalKey_titleLoading41);
	en->setObject(CCString::create("액자 하나를 완성할 때마다 특별한 카드와 하트를 선물로 드립니다."), kMyLocalKey_titleLoading42);
	en->setObject(CCString::create("클리어 점수는 영역획득점수와 콤보점수, 남은 타임 보너스가 합산된 점수입니다."), kMyLocalKey_titleLoading43);
	en->setObject(CCString::create("고득점을 노린다면 연속 콤보를 성공시켜 보세요."), kMyLocalKey_titleLoading44);
	en->setObject(CCString::create(""), kMyLocalKey_titleLoadingEnd);
				
	en->setObject(CCString::create("START"), kMyLocalKey_gamestart);
	en->setObject(CCString::create("대전 시작"), kMyLocalKey_battleStart);
	en->setObject(CCString::create("Missile"), kMyLocalKey_mymissile);
	en->setObject(CCString::create("Damage"), kMyLocalKey_power);
	en->setObject(CCString::create("Gacha"), kMyLocalKey_gacha);
	en->setObject(CCString::create("Try one day to complete the mission! Gifts for you!"), kMyLocalKey_stageListDown);
	en->setObject(CCString::create("액자오픈조건"), kMyLocalKey_frameOpenConditionTitle);
	en->setObject(CCString::create("이전액자클리어\n+별%d개이상"), kMyLocalKey_frameOpenConditionContent);
	en->setObject(CCString::create("젬으로오픈"), kMyLocalKey_frameOpenConditionContentRuby);
	en->setObject(CCString::create("%s\n%d:00-%d:00"), kMyLocalKey_frameOpenConditionContentTimeWeek);
	en->setObject(CCString::create("%s월%s일%s:%s오픈"), kMyLocalKey_frameOpenConditionContentTimeDate);
	en->setObject(CCString::create("자세히보기"), kMyLocalKey_detailView);
	en->setObject(CCString::create("별 획득 방법"), kMyLocalKey_detailConditionPopupTitle);
	en->setObject(CCString::create("<font newline=14>스테이지를 클리어 할 때마다<font newline=30>클리어 단계에 따라 별을 획득합니다.<font color=961>한 스테이지<font newline=14>에서 획득할 수 있는<font color=961>별 갯수는 총 10개<font newline=14>입니다."), kMyLocalKey_detailConditionPopupContent);
//	en->setObject(CCString::create("<font color=999 size=12>스테이지에서 <font color=990 size=12>4가지 등급<font color=999 size=12 newline=12>을 모두 Clear하면<font color=990 size=12>별 10개<font color=999 size=12>를 모을 수 있습니다."), kMyLocalKey_detailConditionPopupContent2);
//	en->setObject(CCString::create("에서 획득할 수 있는"), kMyLocalKey_detailConditionPopupContent3);
//	en->setObject(CCString::create("별 갯수는 총 10개"), kMyLocalKey_detailConditionPopupContent4);
//	en->setObject(CCString::create("입니다."), kMyLocalKey_detailConditionPopupContent5);
	en->setObject(CCString::create("결제 실패"), kMyLocalKey_failPurchase);
	en->setObject(CCString::create("구매 완료"), kMyLocalKey_successPurchase);
	en->setObject(CCString::create("지금 상점으로 이동하시겠습니까?"), kMyLocalKey_enoughtGoodsContent);
//	en->setObject(CCString::create("포기하기"), kMyLocalKey_giveup);
	en->setObject(CCString::create("상점 바로가기"), kMyLocalKey_goShopButton);
	
	en->setObject(CCString::create("액자를 플레이할 수 있는 시간이 지났습니다.\n홈으로 돌아갑니다."), kMyLocalKey_timeOutFrame);
	en->setObject(CCString::create("바로입장"), kMyLocalKey_directEnter);
	en->setObject(CCString::create("카드 승급 기회!!"), kMyLocalKey_rankUpTitle);
	en->setObject(CCString::create("승급 성공시 한 단계 높은 카드를 획득할 수 있어요!"), kMyLocalKey_rankUpSubTitle);
	en->setObject(CCString::create("현재 획득카드"), kMyLocalKey_recentTakeCard);
	en->setObject(CCString::create("승급확률"), kMyLocalKey_rankUpRate);
	en->setObject(CCString::create("승급하기"), kMyLocalKey_rankup);
	
	en->setObject(CCString::create("구매하기"), kMyLocalKey_purchase);
	en->setObject(CCString::create("첫구매시 할인"), kMyLocalKey_firstPurchaseSale);
	en->setObject(CCString::create("%s할인은 첫 구매 회원님께만"), kMyLocalKey_firstPurchaseMent1);
	en->setObject(CCString::create(" 적용됩니다."), kMyLocalKey_firstPurchaseMent2);
	en->setObject(CCString::create("이 창을 닫으면 %s 할인 기회는 사라집니다."), kMyLocalKey_firstPurchaseMent3);
	en->setObject(CCString::create("지금 묶음 아이템을 구매하시면"), kMyLocalKey_emptyItemSaleMent1);
	en->setObject(CCString::create("%s 할인해 드립니다."), kMyLocalKey_emptyItemSaleMent2);
	en->setObject(CCString::create("플레이를 도와주는 기능성 아이템을"), kMyLocalKey_stupidNpuHelpMent1);
	en->setObject(CCString::create("%s할인된 가격으로 사용해보세요."), kMyLocalKey_stupidNpuHelpMent2);
	en->setObject(CCString::create("묶음아이템 할인"), kMyLocalKey_packageItemSale);
	en->setObject(CCString::create("5번째 구매시"), kMyLocalKey_mileageMent1);
	en->setObject(CCString::create("아주 특별한 할인혜택의 기회가 있으니"), kMyLocalKey_mileageMent2);
	en->setObject(CCString::create("혜택을 꼭! 확인해주세요."), kMyLocalKey_mileageMent3);
	en->setObject(CCString::create("단 한번의 구매 찬스"), kMyLocalKey_eventShopTitle);
	en->setObject(CCString::create("당신만을 위한 특별한 기회!"), kMyLocalKey_eventShopMent1);
	en->setObject(CCString::create("%s 보너스 혜택을 드려요."), kMyLocalKey_eventShopMent2);
	en->setObject(CCString::create("이 창을 닫으면 %s 보너스 기회는 사라집니다."), kMyLocalKey_eventShopMent3);
	en->setObject(CCString::create("<font>미사일 레벨을 올리면 </font><font color=961>부하몬스터를 쉽게 죽이고,</font>"), kMyLocalKey_levelupGuideMent1);
	en->setObject(CCString::create("<font color=961>패턴공격을 빨리 저지시킬 수 있으며,</font>"), kMyLocalKey_levelupGuideMent2);
	en->setObject(CCString::create("<font color=961>고득점 기록에 도움이 됩니다.</font>"), kMyLocalKey_levelupGuideMent3);
	en->setObject(CCString::create("<font>미사일 업그레이드를 하러 갈까요?</font>"), kMyLocalKey_levelupGuideMent4);
//	en->setObject(CCString::create("다음에"), kMyLocalKey_next);
	en->setObject(CCString::create("업그레이드"), kMyLocalKey_levelupGo);
	en->setObject(CCString::create("업적"), kMyLocalKey_achievement);
	en->setObject(CCString::create("모든보상 받기"), kMyLocalKey_allRewardGet);
	en->setObject(CCString::create("종합 아이템 구매 찬스!"), kMyLocalKey_itemPackageChance);
	en->setObject(CCString::create("아이템이 하나도 없네요."), kMyLocalKey_emptyItemSaleTitle);
	en->setObject(CCString::create("첫구매 대폭할인!!"), kMyLocalKey_firstPurchaseSaleTitle);
	en->setObject(CCString::create("사랑합니다. 고객님!"), kMyLocalKey_mileageTitle);
	en->setObject(CCString::create("미사일 업그레이드"), kMyLocalKey_missileUpgrade);
	en->setObject(CCString::create("<font>누적영역</font><font color=961> %d%%</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalPercent1);
	en->setObject(CCString::create("%s%%"), kMyLocalKey_todaymissionTotalPercent2);
	en->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalPercent3);
	en->setObject(CCString::create("현재 획득영역"), kMyLocalKey_todaymissionTotalPercent4);
	en->setObject(CCString::create("<font>누적점수</font><font color=961> %s점</font><font>을 획득하라</font>"), kMyLocalKey_todaymissionTotalScore1);
	en->setObject(CCString::create("%s점"), kMyLocalKey_todaymissionTotalScore2);
	en->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalScore3);
	en->setObject(CCString::create("현재 획득점수"), kMyLocalKey_todaymissionTotalScore4);
	en->setObject(CCString::create("<font color=961>%s골드</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalTakeGold1);
	en->setObject(CCString::create("%s골드"), kMyLocalKey_todaymissionTotalTakeGold2);
	en->setObject(CCString::create("현재 획득골드"), kMyLocalKey_todaymissionTotalTakeGold3);
	en->setObject(CCString::create("<font>부하몹</font><font color=961> %d마리</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalCatch1);
	en->setObject(CCString::create("%s마리"), kMyLocalKey_todaymissionTotalCatch2);
	en->setObject(CCString::create(" 사냥하라!!"), kMyLocalKey_todaymissionTotalCatch3);
	en->setObject(CCString::create("현재 잡은 부하몹"), kMyLocalKey_todaymissionTotalCatch4);
//	en->setObject(CCString::create("오늘의 미션을 완료하였습니다."), kMyLocalKey_todaymissionSuccess);
	en->setObject(CCString::create("완료"), kMyLocalKey_complete);
	en->setObject(CCString::create("업적달성"), kMyLocalKey_achieveSuccess);
	en->setObject(CCString::create("업적미완성"), kMyLocalKey_achieveNotSuccess);
	en->setObject(CCString::create("업적보상"), kMyLocalKey_achieveReward);
	en->setObject(CCString::create("달성한 업적이 없습니다."), kMyLocalKey_nothingSuccessAchieve);
	en->setObject(CCString::create("보상받을 업적이 없습니다."), kMyLocalKey_nothingRewardAchieve);
	
	en->setObject(CCString::create("영역획득"), kMyLocalKey_missionTitle0);
	en->setObject(CCString::create("용감한 기사"), kMyLocalKey_missionTitle1);
	en->setObject(CCString::create("부하몹 사냥"), kMyLocalKey_missionTitle2);
	en->setObject(CCString::create("욕심쟁이"), kMyLocalKey_missionTitle3);
	en->setObject(CCString::create("수집가"), kMyLocalKey_missionTitle4);
	en->setObject(CCString::create("완벽주의자"), kMyLocalKey_missionTitle5);
	en->setObject(CCString::create("결벽주의자"), kMyLocalKey_missionTitle6);
	en->setObject(CCString::create("비지니스맨"), kMyLocalKey_missionTitle7);
	
	
	en->setObject(CCString::create("<font color=961 size=13>%.0f%%이상<font color=999 size=13> 획득하라!"), kMyLocalKey_missionDiscription0);
	en->setObject(CCString::create("<font color=999 size=13>보스의 에너지를 모두 소진시켜라!"), kMyLocalKey_missionDiscription1);
	en->setObject(CCString::create("<font color=999 size=13>부하 몬스터를 가두어 잡으세요!"), kMyLocalKey_missionDiscription2);
	en->setObject(CCString::create("<font color=999 size=13>정해진 횟수만큼 한번에 많이 먹으세요!"), kMyLocalKey_missionDiscription3);
	en->setObject(CCString::create("<font color=999 size=13>정해진 숫자만큼 아이템을 모으세요!"), kMyLocalKey_missionDiscription4);
	en->setObject(CCString::create("<font color=999 size=13>정해진 목표로 정확하게 영역을 획득하세요!"), kMyLocalKey_missionDiscription5);
	en->setObject(CCString::create("<font color=999 size=13>CHANGE를 순서대로 획득하세요!"), kMyLocalKey_missionDiscription6);
	en->setObject(CCString::create("<font color=999 size=13>제한시간 내에 클리어하세요!"), kMyLocalKey_missionDiscription7);
	
	en->setObject(CCString::create(""), kMyLocalKey_missionCondition0);
	en->setObject(CCString::create(""), kMyLocalKey_missionCondition1);
	en->setObject(CCString::create("<font color=961 size=18>%d마리"), kMyLocalKey_missionCondition2);
	en->setObject(CCString::create("<font color=961 size=18>%d%%이상 %d번"), kMyLocalKey_missionCondition3);
	en->setObject(CCString::create("<font>아이템을 </font><font color=961 size=18>%d개</font><font> 모으세요!</font>"), kMyLocalKey_missionCondition4);
	en->setObject(CCString::create(""), kMyLocalKey_missionCondition5);
	en->setObject(CCString::create(""), kMyLocalKey_missionCondition6);
	en->setObject(CCString::create("<font color=961 size=18>%d초"), kMyLocalKey_missionCondition7);
	
	en->setObject(CCString::create("대쉬"), kMyLocalKey_item4title);
	en->setObject(CCString::create("잠깐동안 빠른속도로 이동 가능합니다."), kMyLocalKey_item4ment);
	en->setObject(CCString::create("부하몹제거"), kMyLocalKey_item5title);
	en->setObject(CCString::create("부하 몬스터 한마리를 제거합니다."), kMyLocalKey_item5ment);
	en->setObject(CCString::create("침묵"), kMyLocalKey_item7title);
	en->setObject(CCString::create("잠깐동안 몬스터가 공격을 못하도록 합니다."), kMyLocalKey_item7ment);
	en->setObject(CCString::create("부활"), kMyLocalKey_item10title);
	en->setObject(CCString::create("마지막 순간에 부활합니다."), kMyLocalKey_item10ment);
	en->setObject(CCString::create("시간추가"), kMyLocalKey_item8title);
	en->setObject(CCString::create("제한시간을 늘려줍니다."), kMyLocalKey_item8ment);
	en->setObject(CCString::create("일시정지"), kMyLocalKey_pause);
	en->setObject(CCString::create("계속하기"), kMyLocalKey_continue);
	en->setObject(CCString::create("나가기"), kMyLocalKey_ingameOut);
	en->setObject(CCString::create("재시작"), kMyLocalKey_ingameReplay);
	en->setObject(CCString::create("대중교통\n모드"), kMyLocalKey_publicPlaceMode);
//	en->setObject(CCString::create("이어하기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass1);
//	en->setObject(CCString::create("맵다시뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass2);
//	en->setObject(CCString::create("업그레이드 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass3);
//	en->setObject(CCString::create("아이템뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass4);
//	en->setObject(CCString::create("1프로뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass5);
	en->setObject(CCString::create("<font newline=16>오늘의 미션을 완료하였습니다.<font color=961 newline=16>보상은 선물함으로 지급됩니다."), kMyLocalKey_todaymissionSuccessCommon);
	en->setObject(CCString::create("무료"), kMyLocalKey_free);
	en->setObject(CCString::create("영역 다시뽑기"), kMyLocalKey_areaReGacha);
//	en->setObject(CCString::create("버튼을 눌러주세요."), kMyLocalKey_pleaseClickButton);
//	en->setObject(CCString::create("STOP"), kMyLocalKey_stop);
	en->setObject(CCString::create("도전하기"), kMyLocalKey_go100percent);
	en->setObject(CCString::create("계정연결"), kMyLocalKey_accountLink);
	en->setObject(CCString::create("최고 레벨 %d"), kMyLocalKey_maxLevel);
	en->setObject(CCString::create("COMPLETE"), kMyLocalKey_endUpgrade);
	en->setObject(CCString::create("%d단계 카드 획득방법"), kMyLocalKey_cardLockedTitle);
	en->setObject(CCString::create("<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent1);
	en->setObject(CCString::create("<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent2);
	en->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent3);
	en->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent4);
	en->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTime);
	en->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTimeMinute);
	en->setObject(CCString::create("<font color=961 size=15>%dHour"), kMyLocalKey_todaymissionRemainTime2);
	en->setObject(CCString::create("<font color=961 size=15>%dMinute"), kMyLocalKey_todaymissionRemainTimeMinute2);
	en->setObject(CCString::create("<font color=961 size=15>모든 피스 획득!"), kMyLocalKey_puzzleSuccessTitle);
	en->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 하트를 드릴께요!"), kMyLocalKey_puzzleSuccessMent);
	en->setObject(CCString::create("<font color=961 size=15>놀라워요! <font color=961 size=15>퍼펙트 클리어!!"), kMyLocalKey_puzzlePerfectTitle);
	en->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 젬를 드릴께요!"), kMyLocalKey_puzzlePerfectMent);
	en->setObject(CCString::create("이어하시겠습니까?"), kMyLocalKey_doYouWantToContinue);
	en->setObject(CCString::create("이어하기"), kMyLocalKey_onContinue);
	en->setObject(CCString::create("그만하기"), kMyLocalKey_offContinue);
	en->setObject(CCString::create("이번 스테이지는 아이템을 사용할 수 없습니다."), kMyLocalKey_notUseItem);
	en->setObject(CCString::create("새로운 아이템 등장!"), kMyLocalKey_newItemTitle);
	en->setObject(CCString::create("새로운 아이템을 선물로 드릴께요!"), kMyLocalKey_newItemMent);
	en->setObject(CCString::create("보스가 쏜 미사일이\n선에 닿으면 불씨가 생겨 닿으면 죽어요."), kMyLocalKey_dieTutorial1);
	en->setObject(CCString::create("선을 그을때 몬스터가\n선에 닿으면 파동이 생겨 죽어요!"), kMyLocalKey_dieTutorial2);
	en->setObject(CCString::create(""), kMyLocalKey_dieTutorial3);
	en->setObject(CCString::create("남은시간 "), kMyLocalKey_restTime);
	en->setObject(CCString::create("%d Day"), kMyLocalKey_restTimeDay);
	en->setObject(CCString::create("%d Hour"), kMyLocalKey_restTimeHour);
	en->setObject(CCString::create("%d Min"), kMyLocalKey_restTimeMinute);
	en->setObject(CCString::create("%d Sec"), kMyLocalKey_restTimeSecond);
	
	en->setObject(CCString::create("Do you want to exit the game?"), kMyLocalKey_exit);
	
	en->setObject(CCString::create("Count Bomb"), kMyLocalKey_warning9);
	en->setObject(CCString::create("Orange Saw"), kMyLocalKey_warning105);
	en->setObject(CCString::create("Blue Saw"), kMyLocalKey_warning106);
	en->setObject(CCString::create("Crash Laser"), kMyLocalKey_warning107);
	en->setObject(CCString::create("Missile"), kMyLocalKey_warning108);
	en->setObject(CCString::create("Rush"), kMyLocalKey_warning109);
	en->setObject(CCString::create("Fire Work"), kMyLocalKey_warning110);
	en->setObject(CCString::create("Sunflower"), kMyLocalKey_warning111);
	en->setObject(CCString::create("Ice Bomb"), kMyLocalKey_warning112);
	en->setObject(CCString::create("Guided Bomb"), kMyLocalKey_warning113);
	en->setObject(CCString::create("Tornado"), kMyLocalKey_warning1001);
	en->setObject(CCString::create("Flashing"), kMyLocalKey_warning1002);
	en->setObject(CCString::create("Slow Zone"), kMyLocalKey_warning1003);
	en->setObject(CCString::create("Thorn Prison"), kMyLocalKey_warning1004);
	en->setObject(CCString::create("Freezing"), kMyLocalKey_warning1005);
	en->setObject(CCString::create("Chaos"), kMyLocalKey_warning1006);
	en->setObject(CCString::create("Teleport"), kMyLocalKey_warning1007);
	en->setObject(CCString::create("Invisibility"), kMyLocalKey_warning1008);
	en->setObject(CCString::create("Flame Spraying"), kMyLocalKey_warning1009);
	en->setObject(CCString::create("ThunderBolt"), kMyLocalKey_warning1010);
	en->setObject(CCString::create("Speed Laser"), kMyLocalKey_warning1011);
	en->setObject(CCString::create("Radioactivity"), kMyLocalKey_warning1012);
	en->setObject(CCString::create("Meteor"), kMyLocalKey_warning1013);
	en->setObject(CCString::create("Falling Stone"), kMyLocalKey_warning1014);
	en->setObject(CCString::create("Do not come"), kMyLocalKey_warning1015);
	en->setObject(CCString::create("Dynamite Mine"), kMyLocalKey_warning1016);
	en->setObject(CCString::create("Flame Stoker"), kMyLocalKey_warning1017);
	en->setObject(CCString::create("Sudden Cloude"), kMyLocalKey_warning1018);
	en->setObject(CCString::create("Boss Attacks Blocked"), kMyLocalKey_warningBossSuccess);
	en->setObject(CCString::create("Last Life"), kMyLocalKey_warningLastLife);
	
	en->setObject(CCString::create("새로운 보스 공격!"), kMyLocalKey_newBossPattern);
	
	en->setObject(CCString::create("맵에 부딪힐때마다 숫자가 줄고, 0이 되면 폭발해요."), kMyLocalKey_patternContent9);
	en->setObject(CCString::create("하나의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent105);
	en->setObject(CCString::create("여러개의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent106);
	en->setObject(CCString::create("레이저가 획득영역을 부숴요."), kMyLocalKey_patternContent107);
	en->setObject(CCString::create("그리는도중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent108);
	en->setObject(CCString::create("보스가 획득영역을 파괴하며 이동해요."), kMyLocalKey_patternContent109);
	en->setObject(CCString::create("획득영역에서 폭발후 미사일을 퍼트려요."), kMyLocalKey_patternContent110);
	en->setObject(CCString::create("사방으로 미사일을 발사해요."), kMyLocalKey_patternContent111);
	en->setObject(CCString::create("연속으로 발사되어 획득영역을 부숴요."), kMyLocalKey_patternContent112);
	en->setObject(CCString::create("따라와서 폭발해요."), kMyLocalKey_patternContent113);
	en->setObject(CCString::create("맵의 일부를 일정시간 가립니다."), kMyLocalKey_patternContent1001);
	en->setObject(CCString::create("일정시간 화면 전체를 뒤덮어 시야를 가려요."), kMyLocalKey_patternContent1002);
	en->setObject(CCString::create("슬로우존 안에서는 이동속도가 느려져요."), kMyLocalKey_patternContent1003);
	en->setObject(CCString::create("붉은 원의 테두리에 닿으면 생명 하나를 잃어요."), kMyLocalKey_patternContent1004);
	en->setObject(CCString::create("일정시간 움직이지 못해요."), kMyLocalKey_patternContent1005);
	en->setObject(CCString::create("조작 방향의 반대로 움직여요."), kMyLocalKey_patternContent1006);
	en->setObject(CCString::create("보스가 다른위치로 순간이동해요."), kMyLocalKey_patternContent1007);
	en->setObject(CCString::create("일정시간 보스가 보이지 않아요."), kMyLocalKey_patternContent1008);
	en->setObject(CCString::create("획득영역에 불을 피워요."), kMyLocalKey_patternContent1009);
	en->setObject(CCString::create("캐릭터를 따라오며 맵을 부숴요."), kMyLocalKey_patternContent1010);
	en->setObject(CCString::create("그리는중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent1011);
	en->setObject(CCString::create("방사능이 퍼진곳을 조심하세요."), kMyLocalKey_patternContent1012);
	en->setObject(CCString::create("대각선으로 떨어져 맵을 부숴요."), kMyLocalKey_patternContent1013);
	en->setObject(CCString::create("위에서 아래로 운석이 떨어져요."), kMyLocalKey_patternContent1014);
	en->setObject(CCString::create("닿지않도록 주의하세요."), kMyLocalKey_patternContent1015);
	en->setObject(CCString::create("카운트가 끝나면 폭발해요."), kMyLocalKey_patternContent1016);
	en->setObject(CCString::create("일정시간 맵을 따라 움직여요."), kMyLocalKey_patternContent1017);
	en->setObject(CCString::create("언제 폭발할지 몰라요."), kMyLocalKey_patternContent1018);
	
	en->setObject(CCString::create("가운데 빨간 보석이 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다."), kMyLocalKey_tutorial1);
	en->setObject(CCString::create("먼저 영역 위를 이동하는 방법을 알려드리겠습니다.\n오른쪽 아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있습니다.\n조이스틱으로 캐릭터를 위로 이동시켜보세요."), kMyLocalKey_tutorial2);
	en->setObject(CCString::create("캐릭터를 위로 이동시키기"), kMyLocalKey_tutorial3);
	en->setObject(CCString::create("다음에는 영역을 획득하는 방법을 알아보도록 해요.\n왼쪽 아래의 꾸욱 버튼을 누르고 있으면\n영역 바깥으로 나갈 수 있습니다.\n보이는 것처럼 영역을 획득해보세요."), kMyLocalKey_tutorial4);
	en->setObject(CCString::create("영역획득하기"), kMyLocalKey_tutorial5);
	en->setObject(CCString::create("파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다."), kMyLocalKey_tutorial6);
	en->setObject(CCString::create("<font strokesize=0>제한시간 내에 달성도 <font color=961 newline=13 strokesize=0>85%를 넘기면 클리어!!"), kMyLocalKey_tutorial7);
	en->setObject(CCString::create("기본 튜토리얼을 모두 진행하셨습니다.\n본 게임으로 들아갑니다."), kMyLocalKey_tutorial8);
	
	en->setObject(CCString::create("겹친선도 획득!"), kMyLocalKey_controlTip_lineTangle_title);
	en->setObject(CCString::create("꼬인선! 걱정마세요! 그대로 획득할 수 있어요!"), kMyLocalKey_controlTip_lineTangle_content);
	en->setObject(CCString::create("한줄 먹기!"), kMyLocalKey_controlTip_slimLine_title);
	en->setObject(CCString::create("캐릭터를 왼쪽으로 이동 후 오른쪽으로 조작하면 OK!"), kMyLocalKey_controlTip_slimLine_content);
	
//	en->setObject(CCString::create("미션"), kMyLocalKey_mission);
	en->setObject(CCString::create("PvP"), kMyLocalKey_endlessMode);
	en->setObject(CCString::create("주간연승랭킹"), kMyLocalKey_endlessWeeklyranking);
//	en->setObject(CCString::create("정보"), kMyLocalKey_endlessMyInfo);
	en->setObject(CCString::create("전적"), kMyLocalKey_endlessInfoScore);
	en->setObject(CCString::create("<font color=999 strokecolor=000 strokesize=0>%d승 %d패 </font><font color=950 strokecolor=000 strokesize=0>(승률%d%%)</font>"), kMyLocalKey_endlessInfoScoreValue2);
//	en->setObject(CCString::create("%d승 %d패 승률%d%%"), kMyLocalKey_endlessInfoScoreValue);
//	en->setObject(CCString::create("<font color=999 strokecolor=0 strokesize=1>%d승 %d패 </font><font color=950 strokecolor=0 strokesize=1>승률%d%%</font>"), kMyLocalKey_endlessInfoScoreValue);
	en->setObject(CCString::create("최고점수"), kMyLocalKey_endlessHighScore);
	en->setObject(CCString::create("최고연승"), kMyLocalKey_endlessHighStraight);
	en->setObject(CCString::create("<font size=16 color=#FFFFFF>%d</font><font size=10 color=#FFFFFF>연승</font>"), kMyLocalKey_endlessHighStraightValue1);
	en->setObject(CCString::create("%d연승"), kMyLocalKey_endlessHighStraightValue2);
	en->setObject(CCString::create("뭔가 문제가 발생하였습니다.\n다시 시도해주세요."), kMyLocalKey_endlessServerError);
	en->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_endlessDownloadFail);
//	en->setObject(CCString::create("포기하시면 1패가 추가됩니다.\n계속 하시겠습니까?"), kMyLocalKey_endlessCheckGiveup);
	
	en->setObject(CCString::create("스페셜데이"), kMyLocalKey_attendanceSpecialTitle);
	en->setObject(CCString::create("매일 매일 연속 출석!!"), kMyLocalKey_attendanceSeqTitle);
	en->setObject(CCString::create("연속출석 일수에 따라 더욱 푸짐한 선물을 드립니다."), kMyLocalKey_attendanceSpecialContent);
	
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>골드"), kMyLocalKey_attendanceSpecialGoodsTypeGold);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>젬"), kMyLocalKey_attendanceSpecialGoodsTypeRuby);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem9);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem6);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem11);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass1);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass2);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass3);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass4);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass5);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass6);
	en->setObject(CCString::create("<font size=13.5>%d<font size=10>종류"), kMyLocalKey_attendanceSpecialGoodsTypeMany);
	
	en->setObject(CCString::create("<font size=12>%d<font size=8>골드"), kMyLocalKey_attendanceGoodsTypeGold);
	en->setObject(CCString::create("<font size=12>%d<font size=8>젬"), kMyLocalKey_attendanceGoodsTypeRuby);
	en->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem9);
	en->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem6);
	en->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem11);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass1);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass2);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass3);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass4);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass5);
	en->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass6);
	en->setObject(CCString::create("선물상자"), kMyLocalKey_attendanceGoodsTypeMany);
	
	en->setObject(CCString::create("출석체크"), kMyLocalKey_attendanceTitle);
	en->setObject(CCString::create("지급된 보상은 수신함에서 확인 가능합니다."), kMyLocalKey_attendanceContent);
	en->setObject(CCString::create("DAY"), kMyLocalKey_attendanceDay);
	
	en->setObject(CCString::create("생명의 돌"), kMyLocalKey_buyMorphingTitle);
	en->setObject(CCString::create("<font color=961 newline=18>생명의 돌을 사용하면 카드가 살아납니다.<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_buyMorphingContent);
	
	en->setObject(CCString::create("보유갯수 :"), kMyLocalKey_buyMorphingTake);
	en->setObject(CCString::create("개"), kMyLocalKey_buyMorphingTakeValue);
	
	en->setObject(CCString::create("사용하기"), kMyLocalKey_buyMorphingBuy);
	
	en->setObject(CCString::create("생명의 돌이 부족합니다."), kMyLocalKey_notEnoughtMorphingTitle);
	en->setObject(CCString::create("<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_notEnoughtMorphingContent);
	
	en->setObject(CCString::create("쿠폰등록"), kMyLocalKey_couponTitle);
	en->setObject(CCString::create("입력"), kMyLocalKey_couponContent);
	en->setObject(CCString::create("쿠폰등록성공"), kMyLocalKey_couponSuccess);
	en->setObject(CCString::create("선물함을 확인하세요."), kMyLocalKey_couponGiftbox);
	en->setObject(CCString::create("쿠폰등록실패"), kMyLocalKey_couponFail);
	en->setObject(CCString::create("이미 사용한 쿠폰입니다."), kMyLocalKey_couponAlready);
	en->setObject(CCString::create("유효기간이 만료 쿠폰입니다."), kMyLocalKey_couponExpire);
	en->setObject(CCString::create("잘못된 사용입니다."), kMyLocalKey_couponOserror);
	en->setObject(CCString::create("쿠폰을 찾을 수 없습니다."), kMyLocalKey_couponDontfind);
	en->setObject(CCString::create("통신이 원활하지 않습니다. 잠시후 다시 시도해주세요."), kMyLocalKey_couponOtherError);
	
	en->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_notOpenPuzzleNotEnoughtStarTitle);
	en->setObject(CCString::create("<font newline=16>별이 부족해요.<font newline=16>더 높은 등급으로 클리어해서<font color=961 newline=16>별 %d개 모아주세요."), kMyLocalKey_notOpenPuzzleNotEnoughtStarContent);
	
	en->setObject(CCString::create("대전 준비"), kMyLocalKey_endlessStartSettingTitle);
	
	en->setObject(CCString::create("영역점수"), kMyLocalKey_endlessCalcTitleAreaScore);
	en->setObject(CCString::create("공격점수"), kMyLocalKey_endlessCalcTitleDamageScore);
	en->setObject(CCString::create("콤보점수"), kMyLocalKey_endlessCalcTitleComboScore);
	en->setObject(CCString::create("생명점수"), kMyLocalKey_endlessCalcTitleLifeScore);
	en->setObject(CCString::create("시간점수"), kMyLocalKey_endlessCalcTitleTimeScore);
	en->setObject(CCString::create("등급점수"), kMyLocalKey_endlessCalcTitleGradeScore);
	en->setObject(CCString::create("획득골드"), kMyLocalKey_endlessCalcTitleTakeGold);
	en->setObject(CCString::create("획득영역"), kMyLocalKey_endlessCalcTitleTakeArea);
	en->setObject(CCString::create("피격점수"), kMyLocalKey_endlessCalcTitleDamagedScore);
	
	en->setObject(CCString::create("승"), kMyLocalKey_win);
	en->setObject(CCString::create("게임결과"), kMyLocalKey_gameResult);
	en->setObject(CCString::create("실패"), kMyLocalKey_fail);
	en->setObject(CCString::create("달성!!"), kMyLocalKey_achieve);
	en->setObject(CCString::create("닫기"), kMyLocalKey_close);
	en->setObject(CCString::create("%d연승중"), kMyLocalKey_endlessIngWin);
	
	en->setObject(CCString::create("PVP 오픈조건"), kMyLocalKey_endlessOpenConditionTitle);
	en->setObject(CCString::create("<font color=961 newline=16>PVP는 %d스테이지 플레이 후<font newline=16>자동으로 오픈됩니다."), kMyLocalKey_endlessOpenConditionContent);
	
	en->setObject(CCString::create("대전패배"), kMyLocalKey_endlessKeepWin);
	
	en->setObject(CCString::create("연승저장"), kMyLocalKey_endlessKeepWinTitle);
	en->setObject(CCString::create("연승 정보는 유지되며, 다시 PvP 도전 시\n저장된 연승에 이어서 플레이할 수 있습니다."), kMyLocalKey_endlessKeepWinContent);
	
	en->setObject(CCString::create("%d연승 도전"), kMyLocalKey_endlessReadyIngWin);
	
	en->setObject(CCString::create("닉네임 오류"), kMyLocalKey_nicknameError);
	en->setObject(CCString::create("<font color=#FFFFFF strokesize=1 >남은 판수 </font>"
																 "<font color=#FFAA14 size=12 strokesize=1>%d회 / </font>"
																 "<font color=#FFFFFF strokesize=1>성공시 </font>"
																 "<font color=#FFAA14 size=12 strokesize=1>%.1f%% 획득</font>"), kMyLocalKey_onePercentDesc2);

	
	en->setObject(CCString::create("대전상대를 찾는 중입니다."), kMyLocalKey_rivalSearch);
	
	en->setObject(CCString::create("%d레벨"), kMyLocalKey_rivalLevel);
	en->setObject(CCString::create("%d 연승 중"), kMyLocalKey_rivalIngWin);
	en->setObject(CCString::create("대전상대를 선택해주세요."), kMyLocalKey_rivalSelect);
	
	en->setObject(CCString::create("업적완료"), kMyLocalKey_achieveSuccess2);
	
	en->setObject(CCString::create("소녀들의 위치를 파악하고 있습니다."), kMyLocalKey_titleTempScript1);
	en->setObject(CCString::create("소녀들의 의상을 점검하고 있습니다."), kMyLocalKey_titleTempScript2);
	en->setObject(CCString::create("소녀들을 불러모으고 있습니다."), kMyLocalKey_titleTempScript3);
	

	en->setObject(CCString::create("하트 공짜"), kMyLocalKey_heartFree);

	en->setObject(CCString::create("지난주 랭킹 결과"), kMyLocalKey_rankRewardTitle);
	en->setObject(CCString::create("보상선물들은 선물함에서 확인하세요."), kMyLocalKey_rankRewardContent);
	en->setObject(CCString::create("주간 누적 랭킹"), kMyLocalKey_rankRewardStageTitle);
	en->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardStageScore);
	en->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardStageRank);
	en->setObject(CCString::create("지난주 플레이횟수"), kMyLocalKey_rankRewardStagePlayCount);
	en->setObject(CCString::create("PvP 랭킹"), kMyLocalKey_rankRewardEndlessTitle);
	en->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardEndlessScore);
	en->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardEndlessRank);
	en->setObject(CCString::create("지난주 연승기록"), kMyLocalKey_rankRewardEndlessVictory);
	en->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardStageReward);
	en->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardEndlessReward);
	
	en->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardStageRankValue);
	en->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardEndlessRankValue);
	
	en->setObject(CCString::create("%s연승"), kMyLocalKey_rankRewardEndlessVictoryScript);
	
	en->setObject(CCString::create("%d위"), kMyLocalKey_rankRewardRankValue);
	
	en->setObject(CCString::create("%s골드"), kMyLocalKey_rankRewardGoodsTypeGold);
	en->setObject(CCString::create("%s젬"), kMyLocalKey_rankRewardGoodsTypeRuby);
	en->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem9);
	en->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem6);
	en->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem11);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass1);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass2);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass3);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass4);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass5);
	en->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass6);
	en->setObject(CCString::create("선물상자"), kMyLocalKey_rankRewardGoodsTypeMany);
	
	en->setObject(CCString::create("카드 승급 이벤트!"), kMyLocalKey_rankUpEventTitle);
	en->setObject(CCString::create("승급 비용 공짜! 승급 확률 100%!!"), kMyLocalKey_rankUpEventContent);
	
	en->setObject(CCString::create("아니오"), kMyLocalKey_no);
	en->setObject(CCString::create("네"), kMyLocalKey_yes);
	en->setObject(CCString::create("<font>기본 클리어조건 <font color=961 newline=17>%.0f%%를<font color=961>%.0f%%로 하향<font newline=17> 시켜드립니다.<font newline=17>적용하시겠습니까?"), kMyLocalKey_clearTimeEventMent);
	en->setObject(CCString::create("이벤트"), kMyLocalKey_clearTimeEventTitle);
	
	en->setObject(CCString::create("숨겨진 업적"), kMyLocalKey_achieveHiddenTitle);
	en->setObject(CCString::create("저를 찾아주세요! 곳곳에 숨겨져 있어요!"), kMyLocalKey_achieveHiddenContent);
	
	en->setObject(CCString::create("<font newline=20>액자를 선택하면<font color=961 newline=20>퍼즐화면으로 이동합니다."), kMyLocalKey_mainflowDimmed1);
	en->setObject(CCString::create("<font newline=16>획득한<font color=999>별갯수"), kMyLocalKey_mainflowDimmed2);
	en->setObject(CCString::create("<font newline=20>선물함"), kMyLocalKey_mainflowDimmed3);
	en->setObject(CCString::create("<font newline=20>업적"), kMyLocalKey_mainflowDimmed4);
	en->setObject(CCString::create("<font newline=20>설정"), kMyLocalKey_mainflowDimmed5);
	en->setObject(CCString::create("<font newline=20>이벤트"), kMyLocalKey_mainflowDimmed6);
	en->setObject(CCString::create("<font newline=20>랭킹"), kMyLocalKey_mainflowDimmed7);
	en->setObject(CCString::create("<font newline=20>상점"), kMyLocalKey_mainflowDimmed8);
	en->setObject(CCString::create("<font newline=20>카드"), kMyLocalKey_mainflowDimmed9);
	en->setObject(CCString::create("<font newline=16>오늘의<font newline=16>미션"), kMyLocalKey_mainflowDimmed10);
	
	en->setObject(CCString::create("모든 피스 획득!"), kMyLocalKey_puzzleClearCardTitle);
	en->setObject(CCString::create("<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzleClearCardContent);
	en->setObject(CCString::create("모든 카드 획득!"), kMyLocalKey_puzzlePerfectCardTitle);
	en->setObject(CCString::create("<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzlePerfectCardContent);
	
	en->setObject(CCString::create("%d연승 돌파!"), kMyLocalKey_endlessSeqWinRewardTitle);
	en->setObject(CCString::create("%d연승을 축하드립니다!"), kMyLocalKey_endlessSeqWinRewardContent);
	en->setObject(CCString::create("<font newline=18>%d골드를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeGold);
	en->setObject(CCString::create("<font newline=18>%d젬을 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby);
	en->setObject(CCString::create("<font newline=18>기본속도업아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9);
	en->setObject(CCString::create("<font newline=18>더블아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6);
	en->setObject(CCString::create("<font newline=18>자석아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11);
	en->setObject(CCString::create("<font newline=18>이어하기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass1);
	en->setObject(CCString::create("<font newline=18>맵가챠 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass2);
	en->setObject(CCString::create("<font newline=18>업그레이드 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass3);
	en->setObject(CCString::create("<font newline=18>아이템뽑기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass4);
	en->setObject(CCString::create("<font newline=18>1프로찬스 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass5);
	en->setObject(CCString::create("<font newline=18>생명의 돌을 %d개를 드립니다.<font newline=18>생명의 돌은 내카드 다이어리에서 사용하세요."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass6);
	en->setObject(CCString::create("<font newline=18>선물상자를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeMany);
	
	en->setObject(CCString::create("퍼즐에서 획득한 카드입니다."), kMyLocalKey_cardSettingMyCardContent);
	en->setObject(CCString::create("스페셜 카드 보기"), kMyLocalKey_cardSettingMyCardToSpecial);

	en->setObject(CCString::create("모든 피스\n획득시!"), kMyLocalKey_cardSettingClearCardMent);
	en->setObject(CCString::create("퍼펙트\n클리어시!"), kMyLocalKey_cardSettingPerfectCardMent);
	
	en->setObject(CCString::create("스페셜 카드"), kMyLocalKey_cardSettingSpecialCardTitle);
	en->setObject(CCString::create("이벤트를 통해 얻을 수 있는 카드입니다."), kMyLocalKey_cardSettingSpecialCardContent);
	en->setObject(CCString::create("내 카드 보기"), kMyLocalKey_cardSettingSpecialCardToMyCard);
	
	en->setObject(CCString::create("<font newline=16>PVP모드가<font newline=16>오픈되었어요!"), kMyLocalKey_endlessTutorialMent1);
	en->setObject(CCString::create("<font newline=16>내 정보 입니다."), kMyLocalKey_endlessTutorialMent2);
	en->setObject(CCString::create("<font>목록에서 <font color=961>다른 친구를 선택<font newline=16>해보세요.<font newline=16>다른 친구의 정보를 볼 수 있어요."), kMyLocalKey_endlessTutorialMent3);
	en->setObject(CCString::create("<font color=961>대전준비를 눌러보세요."), kMyLocalKey_endlessTutorialMent4);
	en->setObject(CCString::create("<font color=961>대전시작을 눌러보세요."), kMyLocalKey_endlessTutorialMent5);
	en->setObject(CCString::create("<font color=961>내 미니맵"), kMyLocalKey_endlessTutorialMent6);
	en->setObject(CCString::create("<font color=961>내 점수"), kMyLocalKey_endlessTutorialMent7);
	en->setObject(CCString::create("<font color=961>내 장착아이템"), kMyLocalKey_endlessTutorialMent8);
	en->setObject(CCString::create("<font color=961>상대 정보"), kMyLocalKey_endlessTutorialMent9);
	en->setObject(CCString::create("<font color=961 newline=16>5콤보 달성시 마다<font color=961 newline=16>미사일을 발사하여<font color=961 newline=16>상대의 점수를 깎습니다."), kMyLocalKey_endlessTutorialMent10);
	en->setObject(CCString::create("화면을 터치하면 게임 진행됩니다."), kMyLocalKey_endlessTutorialMent11);
	en->setObject(CCString::create("<font color=961>나의 결과"), kMyLocalKey_endlessTutorialMent12);
	en->setObject(CCString::create("<font color=961>상대방 결과"), kMyLocalKey_endlessTutorialMent13);
	en->setObject(CCString::create("<font>스크롤하면 점수 내역을 볼 수 있어요."), kMyLocalKey_endlessTutorialMent14);
	en->setObject(CCString::create("<font color=961>2연승에 도전할 수 있어요!"), kMyLocalKey_endlessTutorialMent15);
	en->setObject(CCString::create("잠깐!"), kMyLocalKey_endlessTutorialMent16);
	en->setObject(CCString::create("3연승을 할 때마다 생명의 돌을 1개\n받을 수 있습니다. 생명의 돌은 내가 가진\n카드에 생명을 불어넣을 수 있는\n특별한 아이템이니 소중히 사용해주세요.\n튜토리얼을 마치겠습니다. 그럼 전 이만!"), kMyLocalKey_endlessTutorialMent17);
	en->setObject(CCString::create("<font>대전 상대를 선택해주세요."), kMyLocalKey_endlessTutorialMent18);
	
	en->setObject(CCString::create("에피소드 %d"), kMyLocalKey_puzzleValue);
	
	en->setObject(CCString::create("보너스 영역 뽑기"), kMyLocalKey_onePercentTutorial1);
	en->setObject(CCString::create("<font color=#FFFFFF size=10 newline=12>안녕하세요!</font>"
																 "<font color=#FFFFFF size=10 newline=12>난 카츠라기라고 해요.</font>"
																 "<font color=#FFFFFF size=10 newline=12>아슬아슬하게 영역을</font>"
																 "<font color=#FFFFFF size=10 newline=24>100% 획득 못했군요.</font>"
																 "<font color=#FFFFFF size=10 newline=12>100% 영역획득 할 수 있게</font>"
																 "<font color=#FFFFFF size=10>내가 도와줄게요.</font>"), kMyLocalKey_onePercentTutorial2);
	en->setObject(CCString::create("다  음"), kMyLocalKey_onePercentTutorial3);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=16>아래 게이지바에서 노란색 영역을 맞히면</font>"
								   "<font color=#FFAA14 size=12 newline=16>0.3%를 추가로 획득할 수 있습니다.</font>"
								   "<font color=#FFFFFF size=12 newline=30>기회는 총 3번 주어집니다.</font>"
								   "<font color=#FFAA14 size=14 newline=15>자 그럼 100% 에 도전해 볼까요?</font>"), kMyLocalKey_onePercentTutorial4);
	
	en->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>뽑기 기회는 단 3번!!</font><font color=#FFFFFF size=12>부족한 영역을 채워 100%에 도전하세요.</font>"), kMyLocalKey_onePercentGame1);
	en->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>100%에 성공시</font><font color=#FFAA14 size=12 tag=1>한 단계 높은 카드</font><font color=#FFFFFF size=12>를 획득할 수 있어요.</font>"), kMyLocalKey_onePercentGame2);
	en->setObject(CCString::create("<font color=#FFFFFF size=12>현재 획득 영역       </font><font color=#FFFF00 size=30 strokecolor=000 strokesize=2 tag=1>99.9%</font>"), kMyLocalKey_onePercentGame3);
	en->setObject(CCString::create("<font size=12 color=#FFFFFF newline=16>노란색 영역에 맞게</font>"
								   "<font size=12 color=#FFAA14>멈춰 버튼 터치</font><font size=12 color=#FFFFFF>하세요!</font>"), kMyLocalKey_onePercentGame4);
	en->setObject(CCString::create("멈춰!"), kMyLocalKey_onePercentGame5);
	en->setObject(CCString::create("100% 실패"), kMyLocalKey_onePercentGame6);
	en->setObject(CCString::create("<font color=#FF0000 size=12>최종 획득 영역     </font>"
								   "<font color=#FF0000 size=30>%.1f%%</font>"), kMyLocalKey_onePercentGame7);
	en->setObject(CCString::create("닫기"), kMyLocalKey_onePercentGame8);
	en->setObject(CCString::create("100% 달성!!"), kMyLocalKey_onePercentGame9);
	en->setObject(CCString::create("카드 받기"), kMyLocalKey_onePercentGame10);
	
	en->setObject(CCString::create("보상내용"), kMyLocalKey_endlessRewardInfo);
	
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>가위 바위 보 중</font>"
								   "<font color=#FFAA14 size=12>하나를 선택 해 주세요.</font>"), kMyLocalKey_gababoContent1);
	en->setObject(CCString::create("<font size=12 newline=13>연</font>"
								   "<font size=12 newline=13>승</font>"
								   "<font size=12 newline=13>보</font>"
								   "<font size=12 newline=13>상</font>"), kMyLocalKey_gababoContent2);
	en->setObject(CCString::create("보상"), kMyLocalKey_gababoContent3);
	en->setObject(CCString::create("1승"), kMyLocalKey_gababoContent4);
	en->setObject(CCString::create("2승"), kMyLocalKey_gababoContent5);
	en->setObject(CCString::create("3승"), kMyLocalKey_gababoContent6);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent7);
	en->setObject(CCString::create("확 인"), kMyLocalKey_gababoContent8);
	en->setObject(CCString::create("나"), kMyLocalKey_gababoContent9);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런, 무승부네요!</font>"
								   "<font color=#FFAA14 size=12>한번 더!</font>"), kMyLocalKey_gababoContent10);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent11);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 4판 모두 이기다니 정말 대단해요!</font>"
																 "<font color=#FFAA14 size=12 newline=14>최고의 보상을 드릴게요!</font>"
																 "<font color=#FFAA14 size=12 newline=14>보너스 게임은 퍼펙트 클리어시마다</font>"
																 "<font color=#FFAA14 size=12 newline=14>할 수 있어요!</font><font color=#FFFFFF size=12>그럼 다음에 또 만나요!</font>"), kMyLocalKey_gababoContent12);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런이런... 지고 말았네요.</font>"
																 "<font color=#FFFFFF size=12>너무 상심마세요!</font>"
																 "<font color=#FFAA14 size=12 newline=14>보너스게임은</font>"
																 "<font color=#FFAA14 size=12 newline=14>퍼펙트 클리어시마다 할 수 있어요!</font>"
																 "<font color=#FFFFFF size=12>다음엔 꼭 저를 이겨보세요!</font>"), kMyLocalKey_gababoContent13);
	en->setObject(CCString::create("루우비~!"), kMyLocalKey_gababoContent14);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>보너스 게임에 오신 것을 환영합니다!</font>"
								   "<font color=#FFFFFF size=12 newline=14>벌써 퍼즐을 퍼펙트 클리어 하시다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12>게임 설명은 최초 1회만 해드리니 잘 봐주세요!</font>"), kMyLocalKey_gababoContent15);
	en->setObject(CCString::create("다 음"), kMyLocalKey_gababoContent16);
	en->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>게임은 가위바위보 입니다!</font>"
								   "<font color=#FFAA14 size=12 newline=14>가위바위보 중 하나를 선택하여 저를 이기시면 됩니다.</font>"
								   "<font color=#FFAA14 size=12 newline=14>연승 횟수에 따라 더 큰 보상이 있어요!</font>"
								   "<font color=#FFFFFF size=12 newline=14>그럼 행운을 빌어요!</font>"), kMyLocalKey_gababoContent17);
	
	en->setObject(CCString::create("+%d"), kMyLocalKey_gababoReward);
	
	en->setObject(CCString::create("PVP 튜토리얼 중에는 나가실 수 없습니다."), kMyLocalKey_notGohomeEndlessTutorialMent);
	en->setObject(CCString::create("PVP 중에는 재시도할 수 없습니다."), kMyLocalKey_notReplayEndlessMent);
	
	en->setObject(CCString::create("대중교통 모드"), kMyLocalKey_optionSecretTitle);
	en->setObject(CCString::create("<font newline=16>대중교통모드가 적용중입니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 해제하실 수 있습니다."), kMyLocalKey_optionSecretOn);
	en->setObject(CCString::create("<font newline=16>대중교통모드가 풀렸습니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 적용하실 수 있습니다."), kMyLocalKey_optionSecretOff);
	
	en->setObject(CCString::create("카드 선물하기"), kMyLocalKey_cardGiftTitle);
	en->setObject(CCString::create("<font color=999>내 아이디는 <font color=961>%s<font color=999> 입니다."), kMyLocalKey_cardGiftMyID);
	en->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_cardGiftInputText);
	en->setObject(CCString::create("검색"), kMyLocalKey_cardGiftSearch);
	en->setObject(CCString::create("선물하기"), kMyLocalKey_cardGiftSend);
	en->setObject(CCString::create("검색결과가 없습니다."), kMyLocalKey_cardGiftNotFound);
	
	en->setObject(CCString::create("<font color=999>실패시 다음번 승급 성공확률이 <font color=961>%.0f%%<font color=999> 올라갑니다."), kMyLocalKey_rankUpFailRateUp);
	en->setObject(CCString::create("<font color=999>이벤트를 놓치지 마세요!!"), kMyLocalKey_rankUpEventSubMent);
	
	en->setObject(CCString::create("TIME OVER"), kMyLocalKey_failTitleTimeover);
	en->setObject(CCString::create("MISSION FAIL"), kMyLocalKey_failTitleMissionfail);
	en->setObject(CCString::create("GAME OVER"), kMyLocalKey_failTitleGameover);
	
	en->setObject(CCString::create("STAGE CLEAR"), kMyLocalKey_clearTitle);
	en->setObject(CCString::create("Warning!"), kMyLocalKey_warningDie);
	en->setObject(CCString::create("쿠폰 번호를 입력해주세요."), kMyLocalKey_couponMent);
	
	en->setObject(CCString::create("내 정보"), kMyLocalKey_nickTitle);
	en->setObject(CCString::create("추가지급"), kMyLocalKey_plusGive);
	
	en->setObject(CCString::create("정보"), kMyLocalKey_pvpInfo);
	en->setObject(CCString::create("보상내용"), kMyLocalKey_pvpReward);
	
	en->setObject(CCString::create("선택"), kMyLocalKey_choice);
	en->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_stageImgLoadFail);
	
	en->setObject(CCString::create("카드선물"), kMyLocalKey_cardGiftSuccessTitle);
	en->setObject(CCString::create("카드를 선물하였습니다."), kMyLocalKey_cardGiftSuccessContent);
	en->setObject(CCString::create("프로필"), kMyLocalKey_profile);
	en->setObject(CCString::create("카드 속성"), kMyLocalKey_cardElemental);
	
	en->setObject(CCString::create("터치"), kMyLocalKey_touch);
	en->setObject(CCString::create("사운드"), kMyLocalKey_sound);
	en->setObject(CCString::create("속성(%s)"), kMyLocalKey_elementalValue);
	
	en->setObject(CCString::create("내카드수 "), kMyLocalKey_myCardCount);
	en->setObject(CCString::create("다이어리 보기"), kMyLocalKey_diaryView);
	
	en->setObject(CCString::create("보유장수 %d"), kMyLocalKey_cardTakeCnt);
	en->setObject(CCString::create("<font newline=20>땅을 획득하려면<font color=961 newline=20>꾸욱 버튼을 누른상태에서<font newline=20>조이스틱을 움직여주세요!"), kMyLocalKey_buttonTutorialMent);
	
	en->setObject(CCString::create("\"한 번 입력한 닉네임은 변경할 수 없으니 신중히 선택해 주세요!\""), kMyLocalKey_nickBottomMent);
	en->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_pleaseInputID);
	en->setObject(CCString::create("유효하지 않은 아이디 입니다."), kMyLocalKey_invalidID);
	en->setObject(CCString::create("다음"), kMyLocalKey_tutorialNextButton);
	
	en->setObject(CCString::create("<font newline=16>선택된</font><font>스테이지</font>"), kMyLocalKey_puzzleDimmed1);
	en->setObject(CCString::create("<font>스테이지</font>"), kMyLocalKey_puzzleDimmed2);
	en->setObject(CCString::create("<font newline=16>스테이지</font><font>클리어 정보</font>"), kMyLocalKey_puzzleDimmed3);
	en->setObject(CCString::create("<font>게임준비</font>"), kMyLocalKey_puzzleDimmed4);
	
	en->setObject(CCString::create("다시보지않기"), kMyLocalKey_noReview);
	en->setObject(CCString::create("젬 100개"), kMyLocalKey_gem100);
	
	en->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>PvP랭킹은 연승이 중요합니다.</font>"), kMyLocalKey_endlessOpeningMarquee1);
	en->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>연승수가 같은 경우 누적점수로 랭킹이 결정됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee2);
	en->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>PvP점수는 주간점수 랭킹에 추가되지 않습니다.</font>"), kMyLocalKey_endlessOpeningMarquee3);
	en->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>한 주간의 기록 중 가장 높은 연승기록으로 랭킹이 결정됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee4);
	en->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>랭킹은 매주 월요일 5시에 초기화 됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee5);
	en->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>이번주 플레이 누적 점수입니다.</font>"), kMyLocalKey_rankNewMarquee1);
	en->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>PvP점수는 합산되지 않습니다.</font>"), kMyLocalKey_rankNewMarquee2);
	en->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>주간랭킹은 매주 월요일 5시에 초기화 됩니다.</font>"), kMyLocalKey_rankNewMarquee3);
	
	en->setObject(CCString::create("드디어 오늘부터 닌자훈련을 시작하는구나!!\n할아버지의 명성에 걸 맞는\n훌륭한 닌자가 될꺼야!"), kMyLocalKey_scenarioMent1);
	en->setObject(CCString::create("여기가 훈련장인가?\n인기척도 없고 전력으로 훈련할 수 있겠는데?!"), kMyLocalKey_scenarioMent2);
	en->setObject(CCString::create("안녕하세요.\n저는 한조국립대 학생 리더인 이카루가입니다."), kMyLocalKey_scenarioMent3);
	en->setObject(CCString::create("앗 안녕하세요.\n저는 아스카 라고합니다."), kMyLocalKey_scenarioMent4);
	en->setObject(CCString::create("네. 전설의 닌자 한조님의 손녀라고\n들었습니다. 잘 부탁 드립니다.\n훈련 전에 선생님께서 간단한 시험을\n하신다는군요."), kMyLocalKey_scenarioMent5);
	en->setObject(CCString::create("으윽, 시험은 그다지 좋아하지 않지만\n알겠습니다."), kMyLocalKey_scenarioMent6);
	en->setObject(CCString::create("어떤 시험인가요?"), kMyLocalKey_scenarioMent7);
	en->setObject(CCString::create("시노비 결계 속에서 대전하는 법을\n익히시면 됩니다."), kMyLocalKey_scenarioMent8);
	en->setObject(CCString::create("네. 그러면 잘 부탁 드리겠습니다."), kMyLocalKey_scenarioMent9);
	en->setObject(CCString::create("그전에 간단히 메뉴들을 소개해 드릴께요."), kMyLocalKey_scenarioMent10);
	en->setObject(CCString::create("숙지하겠습니다."), kMyLocalKey_scenarioMent11);
	en->setObject(CCString::create("그럼 시험장으로 이동 하죠.\n1번액자로 입장합니다."), kMyLocalKey_scenarioMent12);
	en->setObject(CCString::create("이곳은 스테이지를 선택할 수 있는\n퍼즐화면입니다."), kMyLocalKey_scenarioMent13);
	en->setObject(CCString::create("먼저 1스테이지로 이동하시죠."), kMyLocalKey_scenarioMent14);
	en->setObject(CCString::create("이곳은 게임시작 전 아이템이나\n무기를 업그레이드 시킬 수 있는\n게임시작화면입니다."), kMyLocalKey_scenarioMent15);
	en->setObject(CCString::create("게임을 시작하려면 하트가 필요합니다."), kMyLocalKey_scenarioMent16);
	en->setObject(CCString::create("지금은 하트가 충분하니\n바로 시작해볼까요?"), kMyLocalKey_scenarioMent17);
	en->setObject(CCString::create("이런곳은 처음인데 여긴 어디지?"), kMyLocalKey_scenarioMent18);
	en->setObject(CCString::create("시노비 결계 속 입니다.\n일반인의 안전을 위해 닌자들은\n이곳에서 훈련하죠."), kMyLocalKey_scenarioMent19);
	en->setObject(CCString::create("이곳은 처음이실테니\n컨트롤 방법부터 설명 드리겠습니다."), kMyLocalKey_scenarioMent20);
	en->setObject(CCString::create("제한시간 내에 달성도 85%를 넘기면\n시험 합격입니다.\n그럼 시작해볼까요?"), kMyLocalKey_scenarioMent21);
	en->setObject(CCString::create("잠깐!\n깜빡하고 지나갈 뻔했네요."), kMyLocalKey_scenarioMent22);
	en->setObject(CCString::create("시험을 잘 볼 수 있게 도와드릴게요.\n \n왼쪽에 보이는 것이 지금 플레이하고 있는\n스테이지의 미니맵이예요.\n여기서 보스와 나의 위치를 파악하면 되겠죠?"), kMyLocalKey_scenarioMent23);
	en->setObject(CCString::create("게임을 잠시 멈추거나 게임을 나가고 싶다면\n일시정지 버튼을 눌러주세요.\n일시정지에는 유용한 기능들이 있으니\n한번 확인해보세요."), kMyLocalKey_scenarioMent24);
	en->setObject(CCString::create("여기에 점수와 획득한 골드, 그리고 콤보가\n표시됩니다. 자 이제 시작해 볼까요?"), kMyLocalKey_scenarioMent25);
	en->setObject(CCString::create("축하합니다. 첫 번째 시험을 통과하셨군요.\n게임이 끝나면 이곳으로 오게 됩니다."), kMyLocalKey_scenarioMent26);
	en->setObject(CCString::create("스테이지 랭킹"), kMyLocalKey_scenarioMent27);
	en->setObject(CCString::create("점수 정보"), kMyLocalKey_scenarioMent28);
	en->setObject(CCString::create("클리어 등급"), kMyLocalKey_scenarioMent29);
	en->setObject(CCString::create("그럼 계속 해볼까요?\n5스테이지 클리어 후 메인화면에서\n뵙기로 하죠!"), kMyLocalKey_scenarioMent30);
	en->setObject(CCString::create("네! 그럼 최선을 다 해보겠습니다!"), kMyLocalKey_scenarioMent31);
	en->setObject(CCString::create("이런.. 다음엔 더 잘 할 수 있겠죠?\n여긴 게임이 끝나면 오게 됩니다."), kMyLocalKey_scenarioMent32);
	en->setObject(CCString::create("휴.. 이걸로 시험끝!!\n드디어 훈련시작이네~!"), kMyLocalKey_scenarioMent33);
	en->setObject(CCString::create("어랏. 아스카도 한조국립대에서 훈련하는거야?\n같이 최고의 닌자를 꿈꿀 수 있겠구나!!"), kMyLocalKey_scenarioMent34);
	en->setObject(CCString::create("쉿. 일반 학생들이 다니는 교정 안에서는\n닌자에 관한 이야기는 하지 않는게 좋아.\n이곳의 닌자 양성 클래스는\n일반 학생들에겐 비밀이니까!"), kMyLocalKey_scenarioMent35);
	en->setObject(CCString::create("흐응... 비밀이라니...\n조금 긴장하게 되.."), kMyLocalKey_scenarioMent36);
	en->setObject(CCString::create("히바리, 긴장할 필요 까진 없어.\n언제나처럼 시노비결계 안에서 훈련하기\n때문에 보통 사람들은 볼 수 없으니까."), kMyLocalKey_scenarioMent37);
	en->setObject(CCString::create("응, 알고는 있는데... 아스카짱 고마워\n앞으로나 완전 열심히 할꺼야!"), kMyLocalKey_scenarioMent38);
	en->setObject(CCString::create("히바리 위험해!!!\n긴장을 늦추지 마. 적은 남아 있다구!!"), kMyLocalKey_scenarioMent39);
	en->setObject(CCString::create("야규! 히바리를 도와주었구나!"), kMyLocalKey_scenarioMent40);
	en->setObject(CCString::create("어... 어떻게 라는 건 잘 모르겠고.\n교내에도 아직 적이 남아 있으니\n주변의 적을 해치우자."), kMyLocalKey_scenarioMent41);
	en->setObject(CCString::create("야규, 아까는 도와줘서 고마워.\n히바리가 거치적거리니까\n이대로면 모두한테 폐를 끼치게 돼."), kMyLocalKey_scenarioMent42);
	en->setObject(CCString::create("히바리는 거치적 거리는 존재 따위가 아니야.\n우리들은 모두 강하다고 인정받았으니까.\n히바리의 강한 점은 내가 제일 잘 알고있고,\n또 모두들 알고 있어.\n그러니까 더 자신에게 자신감을 가져."), kMyLocalKey_scenarioMent43);
	en->setObject(CCString::create("야규.."), kMyLocalKey_scenarioMent44);
	en->setObject(CCString::create("내.. 내가 말하고 싶은 건 그것뿐이야..\n히바리, 내일은 히바리가 좋아하는\n우사네 찻집에 가자."), kMyLocalKey_scenarioMent45);
	en->setObject(CCString::create("응!"), kMyLocalKey_scenarioMent46);
	en->setObject(CCString::create("이카루가.. 그리고\n야규, 아스카, 히바리도 마침 딱 있었네!"), kMyLocalKey_scenarioMent47);
	en->setObject(CCString::create("카츠라기님. 오늘 훈련은 끝났는데\n집에 안가세요?"), kMyLocalKey_scenarioMent48);
	en->setObject(CCString::create("어, 응..음... 뭐라고 할까\n소화불량이라고나 할까...\n어쨌든 좀 더 몸을 움직이려고... 응?"), kMyLocalKey_scenarioMent49);
	en->setObject(CCString::create("아앗! 저건 뭐지?"), kMyLocalKey_scenarioMent50);
	en->setObject(CCString::create("소매치기예요!! 꼬마의 물건을 훔쳐가네요!!"), kMyLocalKey_scenarioMent51);
	en->setObject(CCString::create("아이의 즐거움을 뺏는\n배짱 좋은 녀석이군.\n저런 썩은 자식은 우리들이 혼내줄테다!!"), kMyLocalKey_scenarioMent52);
	en->setObject(CCString::create("우와! 실력이 일취월장인데요?\n벌써 실력이 많이 늘었네요.\n이젠 다른사람과 대전을 해볼까요?\n \n따라오세요."), kMyLocalKey_scenarioMent53);
	en->setObject(CCString::create("이건 내 정보예요.\n보상내용도 나중에 확인해보세요."), kMyLocalKey_scenarioMent54);
	en->setObject(CCString::create("다른 친구를 누르면 해당친구의 정보를\n볼 수 있어요.\n \nPVP를 시작해보죠."), kMyLocalKey_scenarioMent55);
	en->setObject(CCString::create("제가 당신과 붙을 만한 상대 세분을\n골라봤어요.\n \n셋중 한명을 선택해주세요."), kMyLocalKey_scenarioMent56);
	en->setObject(CCString::create("아아아!! 죄송해요!!\n평소 대전과 달라서 설명을 드려야하는걸\n제가 까먹었네요."), kMyLocalKey_scenarioMent57);
	en->setObject(CCString::create("폭탄이예요! 5콤보시마다 미사일을\n발사해서 상대방의 점수를 깎을 수 있어요!\n조심하세요! 상대방이 절 공격 못하게!\n영역을 획득할 때 발사되는 미사일이\n보스를 타격하게 되면 콤보가 쌓이게 되고,\n콤보점수가 추가됩니다."), kMyLocalKey_scenarioMent58);
	en->setObject(CCString::create("행운을 빌어요."), kMyLocalKey_scenarioMent59);
	en->setObject(CCString::create("첫 대전이라 떨리시죠?\n \n이기면 보상이 있으니 행운을 빌어요.\n그럼 전 이만.."), kMyLocalKey_scenarioMent60);
	en->setObject(CCString::create("PVP가 끝나면 이곳으로 와요."), kMyLocalKey_scenarioMent61);
	en->setObject(CCString::create("모두 게임오버 되었을 때는\n피격점수로 승패를 가릅니다."), kMyLocalKey_scenarioMent62);
	
	en->setObject(CCString::create("업적 메뉴에서는 게임 중 달성한 업적을\n확인하고 보상을 받을 수 있습니다.\n앞으로 해야 할 업적들이 정말 많군요!"), kMyLocalKey_kindTutorial1);
	en->setObject(CCString::create("모든보상받기 버튼을 누르면 달성한 업적\n보상을 모두 한번에 받을 수 있어요!"), kMyLocalKey_kindTutorial2);
	en->setObject(CCString::create("하하! 또 만나네요.\n내카드에서는 스테이지에서 획득한 카드를\n확인할 수 있어요."), kMyLocalKey_kindTutorial3);
	en->setObject(CCString::create("내가 획득한 카드나 다이어리 보기 버튼을\n터치하면 카드의 상세 내용을 확인할 수\n있는 다이어리로 이동합니다."), kMyLocalKey_kindTutorial4);
	en->setObject(CCString::create("상단의 정렬버튼을 눌러 여러가지 옵션으로\n정렬기능을 이용할 수도 있습니다."), kMyLocalKey_kindTutorial5);
	en->setObject(CCString::create("어머! 여기서 또 만나네요?\n이번엔 랭킹에 대해 설명해 드릴께요.\n \n주간랭킹은 일주일 동안 획득한 스테이지\n점수를 합산하여 랭킹이 산정됩니다."), kMyLocalKey_kindTutorial6);
	en->setObject(CCString::create("순위마다 보상 내용이 다르니\n잘 확인해 주세요."), kMyLocalKey_kindTutorial7);
	en->setObject(CCString::create("드디어 뽑기상자가 등장했네요."), kMyLocalKey_kindTutorial8);
	en->setObject(CCString::create("이건 뭐죠? 아이템인가요?"), kMyLocalKey_kindTutorial9);
	en->setObject(CCString::create("특별한 아이템을 뽑을 수 있는 상자입니다.\n운이 좋으면 목숨 하나가 추가되는\n부활아이템을 얻을 수 있죠."), kMyLocalKey_kindTutorial10);
	en->setObject(CCString::create("껄끄러운 게임미션이로군.\n히바리. 이번 게임에선 부하몬스터를\n표시된 개수만큼 가둬 잡아야 클리어할 수 있어.\n너라면 충분히 해낼 수 있을꺼야."), kMyLocalKey_kindTutorial11);
	en->setObject(CCString::create("응! 야규짱!"), kMyLocalKey_kindTutorial12);
	en->setObject(CCString::create("이번엔 게임미션이 조금 다른 것 같아요."), kMyLocalKey_kindTutorial13);
	en->setObject(CCString::create("네. 주어진 개수만큼 생성되는 아이템을\n먹어야 클리어 됩니다.\n클리어하기 더 어려워지겠는데요?"), kMyLocalKey_kindTutorial14);
	en->setObject(CCString::create("게임시간이 엄청나게 줄어들었어요.\n조금 서둘러야겠는걸요."), kMyLocalKey_kindTutorial15);
	en->setObject(CCString::create("알고있어!! 게임 중에 생기는 시간아이템을\n적절히 먹으면서 플레이하면 문제없을꺼야!!"), kMyLocalKey_kindTutorial16);
	en->setObject(CCString::create("앗! 이건 뭐죠?"), kMyLocalKey_kindTutorial17);
	en->setObject(CCString::create("이 신발을 착용하면 게임을\n더 빠른 스피드로 시작할 수 있어요.\n무료로 몇 개 지급되니 사용해보세요."), kMyLocalKey_kindTutorial18);
	en->setObject(CCString::create("새로운 아이템이 등장했네요."), kMyLocalKey_kindTutorial19);
	en->setObject(CCString::create("게임 중에 아이템이 두 배로 더 많이\n나오도록 해주는 인법서군요.\n많은 도움이 될겁니다."), kMyLocalKey_kindTutorial20);
	en->setObject(CCString::create("이건 자석인가?"), kMyLocalKey_kindTutorial21);
	en->setObject(CCString::create("네. 게임 중에 생기는 골드나 아이템을\n더 쉽게 먹을 수 있도록 도와주는\n아이템입니다."), kMyLocalKey_kindTutorial22);
	
	en->setObject(CCString::create("MAX"), kMyLocalKey_maxSpeed);
	en->setObject(CCString::create("SHOW TIME"), kMyLocalKey_showtime);
	en->setObject(CCString::create("MISS"), kMyLocalKey_miss);
	en->setObject(CCString::create("COMBO"), kMyLocalKey_combo);
	en->setObject(CCString::create("Scrolling is possible!"), kMyLocalKey_unlockedAreaScroll);
	en->setObject(CCString::create("MISSION COMPLETE"), kMyLocalKey_missionComplete);
	en->setObject(CCString::create("COLLECT CHANGE!!"), kMyLocalKey_collectChange);
	en->setObject(CCString::create("정식오픈 후 사용할 수 있습니다."), kMyLocalKey_afterOpenCBT);
	en->setObject(CCString::create("CARD CHANGE!"), kMyLocalKey_cardChange);
	en->setObject(CCString::create("100% 쇼타임에서는 생명의 돌을 체험할 수 있어요."), kMyLocalKey_showtimeMorphingTouch);
	en->setObject(CCString::create("카드강화"), kMyLocalKey_cardStrength);
	en->setObject(CCString::create("업데이트 예정"), kMyLocalKey_updateTitle);
	en->setObject(CCString::create("추후 업데이트 됩니다."), kMyLocalKey_updateContent);
	en->setObject(CCString::create("준비중"), kMyLocalKey_communityNotOpenTitle);
	en->setObject(CCString::create("준비입니다. 공식카페를 이용해 주세요."), kMyLocalKey_communityNotOpenContent);
	
	en->setObject(CCString::create("조이스틱\n크기"), kMyLocalKey_joystickSize);
	en->setObject(CCString::create("대각선\n사용"), kMyLocalKey_useSideDirection);
	en->setObject(CCString::create("19+ 다이어리"), kMyLocalKey_toDiary19);
	
	en->setObject(CCString::create("19+ 다이어리"), kMyLocalKey_toDiary19Title);
	en->setObject(CCString::create("<font size=12 newline=15>보조앱(19+ 다이어리)이 없네요.</font><font size=12 newline=15>더욱 화끈하고! 쎈! 그녀들이 기다립니다.</font><font size=12>해당앱으로 연결하시겠습니까?</font>"), kMyLocalKey_toDiary19Content);
	en->setObject(CCString::create("19+ 다이어리 다운 받기"), kMyLocalKey_toDiary19Button);
	en->setObject(CCString::create("조이스틱\n진동"), kMyLocalKey_joystickVib);
	
	en->setObject(CCString::create("조이스틱 크기조절"), kMyLocalKey_joystickSizeQuestionTitle);
	en->setObject(CCString::create("<font newline=17>게임에서 사용되는 조이스틱의 크기를</font><font newline=17>9단계의 크기로 조절하고 있습니다.</font><font color=961>평균 5단계를 사용합니다.</font>"), kMyLocalKey_joystickSizeQuestionContent);
	
	setObject(en, "en");
}
void MyLocal::ja()
{
	
	CCDictionary* ja = CCDictionary::create();
	
	ja->setObject(CCString::create("Server Online"), kMyLocalKey_connectingServer);
//	ja->setObject(CCString::create("Nickname enter"), kMyLocalKey_inputNick);
	ja->setObject(CCString::create("Please enter."), kMyLocalKey_inputPlease);
	ja->setObject(CCString::create("OK"), kMyLocalKey_ok);
	ja->setObject(CCString::create("Loading images."), kMyLocalKey_downImgInfo);
	ja->setObject(CCString::create("Information Retrieval Failure"), kMyLocalKey_failLoadInfo);
	ja->setObject(CCString::create("Retry"), kMyLocalKey_replay);
//	ja->setObject(CCString::create("Downloading %.0f%% : %d/%d"), kMyLocalKey_downloadingProgress);
	ja->setObject(CCString::create("Receive images fail"), kMyLocalKey_downImgFail);
//	ja->setObject(CCString::create("Login successful"), kMyLocalKey_successLogin);
	ja->setObject(CCString::create("This nickname is already in use."), kMyLocalKey_sameNick);
	ja->setObject(CCString::create("Nickname error"), kMyLocalKey_invalidNick);
	ja->setObject(CCString::create("Nickname is too long."), kMyLocalKey_longNick);
	ja->setObject(CCString::create("Nickname is too short."), kMyLocalKey_shortNick);
//	ja->setObject(CCString::create("특수문자는 사용 할 수 없습니다."), kMyLocalKey_specialCharacterError);
	ja->setObject(CCString::create("Now Open"), kMyLocalKey_openRightNow);
	ja->setObject(CCString::create("RUBY %d open"), kMyLocalKey_openRubyValue);
//	ja->setObject(CCString::create("GOLD %d open"), kMyLocalKey_openGoldValue);
	ja->setObject(CCString::create("CGP 일반 보상"), kMyLocalKey_cgpNormalTitle);
	ja->setObject(CCString::create("Press to receive reward."), kMyLocalKey_cgpNormalContent);
	ja->setObject(CCString::create("Receive reward"), kMyLocalKey_getReward);
	ja->setObject(CCString::create("take"), kMyLocalKey_take);
	ja->setObject(CCString::create("CGP 전체 팝업 보상"), kMyLocalKey_cgpAllPopupTitle);
	ja->setObject(CCString::create("Shop"), kMyLocalKey_shop);
	ja->setObject(CCString::create("Ranking"), kMyLocalKey_ranking);
	ja->setObject(CCString::create("My card"), kMyLocalKey_mycard);
	ja->setObject(CCString::create("Today mission"), kMyLocalKey_todaymission);
	ja->setObject(CCString::create("Event"), kMyLocalKey_event);
	ja->setObject(CCString::create("스페셜 스테이지"), kMyLocalKey_openStage);
	ja->setObject(CCString::create("스페셜 스테이지를 오픈하시겠습니까?"), kMyLocalKey_openStageContent);
	ja->setObject(CCString::create("오픈하기"), kMyLocalKey_doOpen);
//	ja->setObject(CCString::create("Open Stage"), kMyLocalKey_stageOpenTitle);
//	ja->setObject(CCString::create("A new stage\nhas been unlocked"), kMyLocalKey_stageOpenContent);
//	ja->setObject(CCString::create("Puzzle open"), kMyLocalKey_puzzleOpenTitle);
//	ja->setObject(CCString::create("A new puzzle\nhas been unlocked."), kMyLocalKey_puzzleOpenContent);
	ja->setObject(CCString::create("View"), kMyLocalKey_view);
	ja->setObject(CCString::create("Area 85%"), kMyLocalKey_condition1);
	ja->setObject(CCString::create("Area 100%"), kMyLocalKey_condition2);
	ja->setObject(CCString::create("Change + Area 85%"), kMyLocalKey_condition3);
	ja->setObject(CCString::create("Change + Area 100%"), kMyLocalKey_condition4);
//	ja->setObject(CCString::create("85%"), kMyLocalKey_conditionTwoLine1);
//	ja->setObject(CCString::create("85%\n+ Change"), kMyLocalKey_conditionTwoLine2);
//	ja->setObject(CCString::create("100%"), kMyLocalKey_conditionTwoLine3);
//	ja->setObject(CCString::create("100%\n+ Change"), kMyLocalKey_conditionTwoLine4);
	ja->setObject(CCString::create("%d Stage"), kMyLocalKey_stageValue);
//	ja->setObject(CCString::create("My location %d"), kMyLocalKey_myrankValue);
	ja->setObject(CCString::create("Failure Ranking Information"), kMyLocalKey_failCheckRanking);
	ja->setObject(CCString::create("Stage"), kMyLocalKey_stage);
	ja->setObject(CCString::create("Choose the items that you will use during the game."), kMyLocalKey_selectUseItem);
	ja->setObject(CCString::create("Level %d"), kMyLocalKey_levelValue);
	ja->setObject(CCString::create("Power %s"), kMyLocalKey_powerValue);
	ja->setObject(CCString::create("Level %d Upgrade"), kMyLocalKey_upgradeLevelValue);
	ja->setObject(CCString::create("Level %d\nUpgrade"), kMyLocalKey_upgradeLevelValue2);
	ja->setObject(CCString::create("Up to three items\ncan be selected."), kMyLocalKey_totalItemSelectCount3);
	ja->setObject(CCString::create("You don’t have enough gold"), kMyLocalKey_goldNotEnought);
	ja->setObject(CCString::create("You don’t have enough ruby."), kMyLocalKey_rubyNotEnought);
	ja->setObject(CCString::create("You don’t have enough heart."), kMyLocalKey_heartNotEnought);
//	ja->setObject(CCString::create("Would you like to go buy?"), kMyLocalKey_goToShop);
	ja->setObject(CCString::create("The default speed"), kMyLocalKey_baseSpeedUpTitle);
	ja->setObject(CCString::create("The user starts with a fast state."), kMyLocalKey_baseSpeedUpContent);
	ja->setObject(CCString::create("Double items"), kMyLocalKey_doubleItemTitle);
	ja->setObject(CCString::create("Quickly create items."), kMyLocalKey_doubleItemContent);
	ja->setObject(CCString::create("자석"), kMyLocalKey_magnetTitle);
	ja->setObject(CCString::create("골드와 아이템을 빨아들입니다."), kMyLocalKey_magnetContent);
	ja->setObject(CCString::create("Additional time"), kMyLocalKey_longTimeTitle);
	ja->setObject(CCString::create("Game time increases."), kMyLocalKey_longTimeContent);
	ja->setObject(CCString::create("Items Draw"), kMyLocalKey_itemGachaTitle);
	ja->setObject(CCString::create("어떤 아이템이 나올까요?"), kMyLocalKey_itemGachaDefaultContent1);
	ja->setObject(CCString::create("뽑기에만 출몰하는 다양한 아이템!!"), kMyLocalKey_itemGachaDefaultContent2);
	ja->setObject(CCString::create("Resurrection challenge your items!"), kMyLocalKey_itemGachaContent);
	ja->setObject(CCString::create("RUBY SHOP"), kMyLocalKey_rubyShop);
	ja->setObject(CCString::create("GOLD SHOP"), kMyLocalKey_goldShop);
	ja->setObject(CCString::create("HEART SHOP"), kMyLocalKey_heartShop);
//	ja->setObject(CCString::create("구매확인"), kMyLocalKey_checkBuy);
	ja->setObject(CCString::create("Do you want to buy?"), kMyLocalKey_realBuy);
	ja->setObject(CCString::create("Option"), kMyLocalKey_option);
	ja->setObject(CCString::create("Coupon"), kMyLocalKey_regCoupon);
	ja->setObject(CCString::create("Community"), kMyLocalKey_community);
//	ja->setObject(CCString::create("Gametip"), kMyLocalKey_gametip);
//	ja->setObject(CCString::create("Tutorial"), kMyLocalKey_tutorial);
	ja->setObject(CCString::create("System"), kMyLocalKey_system);
	ja->setObject(CCString::create("Background sound"), kMyLocalKey_bgm);
	ja->setObject(CCString::create("Sound Effects"), kMyLocalKey_effect);
	ja->setObject(CCString::create("Joystick\nposition"), kMyLocalKey_joystickPosition);
	ja->setObject(CCString::create("Operation\nJoystick"), kMyLocalKey_joystickControl);
	ja->setObject(CCString::create("Safe Mode"), kMyLocalKey_safetyMode);
	ja->setObject(CCString::create("Notify"), kMyLocalKey_noti);
	ja->setObject(CCString::create("Push"), kMyLocalKey_pushNoti);
	ja->setObject(CCString::create("Message\nAlarm"), kMyLocalKey_messageNoti);
	ja->setObject(CCString::create("서버연결에 실패하였습니다."), kMyLocalKey_failedConnect);
	ja->setObject(CCString::create("메세지확인"), kMyLocalKey_confirmGift);
//	ja->setObject(CCString::create("선물을 받았습니다."), kMyLocalKey_getgift);
	ja->setObject(CCString::create("받은 메세지가 없습니다."), kMyLocalKey_nogift);
	ja->setObject(CCString::create("*받은 메세지는 30일 이후에 삭제됩니다."), kMyLocalKey_giftboxAlert);
	ja->setObject(CCString::create("<font color=#FFAA14 size=15>%s</font><br><font color=#FFFFFF>- %s -</font><br><font color=#CCCCCC size=10s>%s</font>"), kMyLocalKey_giftboxContent);
	ja->setObject(CCString::create("재접속"), kMyLocalKey_reConnect);
	ja->setObject(CCString::create("다른 기기로 연결되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert1);
	ja->setObject(CCString::create("서버와의 접속에 오류가 발생하였습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert2);
	ja->setObject(CCString::create("세션이 종료되었습니다.\n다시 로그인합니다."), kMyLocalKey_reConnectAlert3);
	ja->setObject(CCString::create("서버연결에 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_reConnectAlert4);
	ja->setObject(CCString::create("BLOCKED"), kMyLocalKey_blocked);
	ja->setObject(CCString::create("계정이 아래의 사유로 정지 되었습니다."), kMyLocalKey_blockedMsg);
	ja->setObject(CCString::create("Member ID : "), kMyLocalKey_memberID);
	ja->setObject(CCString::create("Version : "), kMyLocalKey_gameversion);
	ja->setObject(CCString::create("Secession"), kMyLocalKey_withdraw);
	ja->setObject(CCString::create("Customer Service"), kMyLocalKey_callCenter);
	ja->setObject(CCString::create("Are you sure you want to logout?"), kMyLocalKey_checkLogout);
	ja->setObject(CCString::create("Cancel"), kMyLocalKey_cancel);
	ja->setObject(CCString::create("게임 탈퇴"), kMyLocalKey_withdrawTitle);
	ja->setObject(CCString::create("Can not use the room when you leave the game,"), kMyLocalKey_withdrawContent1);
	ja->setObject(CCString::create("all data will be deleted permanently."), kMyLocalKey_withdrawContent2);
	ja->setObject(CCString::create("Are you sure you want to really leave?"), kMyLocalKey_withdrawContent3);
	ja->setObject(CCString::create("탈퇴하기"), kMyLocalKey_doWithdraw);
	ja->setObject(CCString::create("Off"), kMyLocalKey_lightOff);
	ja->setObject(CCString::create("On"), kMyLocalKey_lightOn);
	ja->setObject(CCString::create("Right"), kMyLocalKey_right);
	ja->setObject(CCString::create("Left"), kMyLocalKey_left);
	ja->setObject(CCString::create("Fixing"), kMyLocalKey_fix);
	ja->setObject(CCString::create("Movement"), kMyLocalKey_move);
	ja->setObject(CCString::create("Weekly cumulative ranking"), kMyLocalKey_weeklyranking);
	ja->setObject(CCString::create("%ddays after the reset"), kMyLocalKey_dayAfterReset);
	ja->setObject(CCString::create("%dtimes after the reset"), kMyLocalKey_hourAfterReset);
	ja->setObject(CCString::create("%dMinutes after the reset"), kMyLocalKey_secondAfterReset);
	ja->setObject(CCString::create("Gift Box"), kMyLocalKey_giftbox);
	ja->setObject(CCString::create("Accept all"), kMyLocalKey_allAccept);
	ja->setObject(CCString::create("티켓함"), kMyLocalKey_ticketBox);
	ja->setObject(CCString::create("도움함"), kMyLocalKey_helpBox);
	ja->setObject(CCString::create("도전함"), kMyLocalKey_challengeBox);
	ja->setObject(CCString::create("하트함"), kMyLocalKey_heartBox);
	ja->setObject(CCString::create("전체보기"), kMyLocalKey_allView);
//	ja->setObject(CCString::create("하트가 도착했어요."), kMyLocalKey_arriveHeart);
//	ja->setObject(CCString::create("도전장이 도착했어요."), kMyLocalKey_arriveChallenge);
//	ja->setObject(CCString::create("도전을 받아들이시겠습니까?"), kMyLocalKey_checkAcceptChallenge);
//	ja->setObject(CCString::create("도망가기"), kMyLocalKey_ttt);
//	ja->setObject(CCString::create("도전에 임하는 자세를 가집시다!!\n도망으로 간주, 패하셨습니다."), kMyLocalKey_tttContent);
//	ja->setObject(CCString::create("도전결과!!"), kMyLocalKey_resultChallenge);
//	ja->setObject(CCString::create("졌습니다. 브레이크 포인트 필요함. 위에 contentObj"), kMyLocalKey_loseContent);
//	ja->setObject(CCString::create("도움요청이 도착했어요."), kMyLocalKey_arriveHelp);
//	ja->setObject(CCString::create("상대방을 도와줍니다."), kMyLocalKey_acceptHelp);
//	ja->setObject(CCString::create("거절"), kMyLocalKey_deny);
//	ja->setObject(CCString::create("수락"), kMyLocalKey_accept);
//	ja->setObject(CCString::create("상대방의 도움이 왔어요!"), kMyLocalKey_arriveHelped);
//	ja->setObject(CCString::create("카드 정보 로딩"), kMyLocalKey_cardInfoLoading);
//	ja->setObject(CCString::create("도움을 받았습니다!!"), kMyLocalKey_iHelped);
//	ja->setObject(CCString::create("티켓요청이 도착했어요."), kMyLocalKey_arriveNeedTicket);
//	ja->setObject(CCString::create("티켓요청이 도착. 티켓은 퍼즐을 열 때 필요합니다. 친구를 도와주세요!!"), kMyLocalKey_arriveNeedTicketContent);
//	ja->setObject(CCString::create("보내기"), kMyLocalKey_send);
//	ja->setObject(CCString::create("티켓이 왔네요 어서 받으세요."), kMyLocalKey_arriveTicket);
//	ja->setObject(CCString::create("티켓이 도착했습니다. 티켓을 받으세요."), kMyLocalKey_arriveTicketContent);
//	ja->setObject(CCString::create("티켓받기"), kMyLocalKey_takeTicket);
//	ja->setObject(CCString::create("친구추가 요청이 왔습니다."), kMyLocalKey_arriveAddFriend);
//	ja->setObject(CCString::create("An error has occurred."), kMyLocalKey_whatError);
//	ja->setObject(CCString::create("I can not find the request arrived."), kMyLocalKey_arriveWhatError);
	ja->setObject(CCString::create("Default"), kMyLocalKey_defaultSort);
	ja->setObject(CCString::create("Take"), kMyLocalKey_takeOrder);
	ja->setObject(CCString::create("Rank"), kMyLocalKey_gradeOrder);
//	ja->setObject(CCString::create("Rotation"), kMyLocalKey_rotation);
	ja->setObject(CCString::create("Raising levels to defeat the boss can do it more easily!"), kMyLocalKey_upgradeSubMent);
	ja->setObject(CCString::create("Draw again"), kMyLocalKey_itemRegacha);
	ja->setObject(CCString::create("%.0f%% Discount"), kMyLocalKey_itemRegachaDiscountValue);
	ja->setObject(CCString::create("Use of this item"), kMyLocalKey_thisItemUse);
//	ja->setObject(CCString::create("Time"), kMyLocalKey_time);
//	ja->setObject(CCString::create("Gold"), kMyLocalKey_gold);
//	ja->setObject(CCString::create("Score"), kMyLocalKey_score);
	ja->setObject(CCString::create("총점"), kMyLocalKey_totalScore);
	ja->setObject(CCString::create("The next stage"), kMyLocalKey_nextStage);
	ja->setObject(CCString::create("again to"), kMyLocalKey_regame);
	ja->setObject(CCString::create("MAIN"), kMyLocalKey_toMain);
	ja->setObject(CCString::create("Please clear\nthe previous stage."), kMyLocalKey_beforeNotClearPuzzle);
	ja->setObject(CCString::create("이전액자를\n모두클리어하면\n다음액자가나타납니다."), kMyLocalKey_waitForUpdate);
//	ja->setObject(CCString::create("I'll wait!\n My collection\nof pictures of it!"), kMyLocalKey_diaryNoImg);
	ja->setObject(CCString::create("Reward"), kMyLocalKey_reward);
	ja->setObject(CCString::create("SPEED"), kMyLocalKey_speed);
	ja->setObject(CCString::create("Ready"), kMyLocalKey_ready);
	ja->setObject(CCString::create("상대찾기"), kMyLocalKey_endlessReady);
//	ja->setObject(CCString::create("God"), kMyLocalKey_rankA);
//	ja->setObject(CCString::create("Hero"), kMyLocalKey_rankB);
//	ja->setObject(CCString::create("Normal"), kMyLocalKey_rankC);
//	ja->setObject(CCString::create("Monkey"), kMyLocalKey_rankD);
//	ja->setObject(CCString::create("Buy"), kMyLocalKey_buy);
	ja->setObject(CCString::create("Push"), kMyLocalKey_controlClick);
	ja->setObject(CCString::create("Locked"), kMyLocalKey_locked);
//	ja->setObject(CCString::create("Please select the box."), kMyLocalKey_selectClearBox);
	
	ja->setObject(CCString::create(""), kMyLocalKey_titleLoadingBegin);
	ja->setObject(CCString::create("각 스테이지 별로 각기 다른 4장의 이미지를 획득해 보세요."), kMyLocalKey_titleLoading1);
	ja->setObject(CCString::create("공공장소나 대중교통 이용시에는 '대중교통 모드'로 Play 하세요. 획득한 이미지는 내카드에서 확인 할 수 있습니다."), kMyLocalKey_titleLoading2);
	ja->setObject(CCString::create("파란색 실루엣 영역을 획득해야 % 가 증가해요."), kMyLocalKey_titleLoading3);
	ja->setObject(CCString::create("'영역 다시 뽑기' 를 해서 넓은 영역으로 play 를 시작하면 좀 더 쉽게 높은 등급의 카드를 획득하기가 쉬워져요"), kMyLocalKey_titleLoading4);
	ja->setObject(CCString::create("'대중교통 모드' 는 play 중에 일시정지 버튼을 탭해서 설정할 수 있습니다."), kMyLocalKey_titleLoading5);
	ja->setObject(CCString::create("Clear 등급에 따라 획득되는 별의 개수가 다릅니다. 별이 모이면 다음 액자를 열 수 있어요."), kMyLocalKey_titleLoading7);
	ja->setObject(CCString::create("'업적' 창에서 달성한 업적을 보상받으세요. 보석을 드립니다."), kMyLocalKey_titleLoading8);
	ja->setObject(CCString::create("일일미션에 도전해 보세요. 일일 미션을 달성하시면 다양한 선물을 드립니다."), kMyLocalKey_titleLoading9);
	ja->setObject(CCString::create("주간랭킹에 도전해 보세요. 매주 새로운 선물을 푸짐하게 드립니다."), kMyLocalKey_titleLoading10);
	ja->setObject(CCString::create("change 글자를 모아서 특별한 이미지를 확인하세요."), kMyLocalKey_titleLoading11);
	ja->setObject(CCString::create("미사일을 업그레이드 하면 강력한 공격력으로 높은 등급 카드 획득이 가능합니다."), kMyLocalKey_titleLoading12);
	ja->setObject(CCString::create("아이템뽑기에서는 '부활' 아이템을 뽑을 수 있어요."), kMyLocalKey_titleLoading13);
	ja->setObject(CCString::create("스테이지마다 showtime 에 도전해 보세요."), kMyLocalKey_titleLoading14);
	ja->setObject(CCString::create("매주 새로운 이벤트가 열립니다. 공식 카페를 확인해 주세요."), kMyLocalKey_titleLoading15);
	ja->setObject(CCString::create("게임 내에서는 이름 대신 닉네임을 사용합니다."), kMyLocalKey_titleLoading16);
	ja->setObject(CCString::create("99% 라 너무 아쉽다면, 보너스 뽑기에 도전 해 보세요."), kMyLocalKey_titleLoading17);
	ja->setObject(CCString::create("이벤트 스테이지에서는 특별한 이미지와 선물이 기다립니다."), kMyLocalKey_titleLoading18);
	ja->setObject(CCString::create("스테이지별 미션 clear 가 어렵다면 아이템 뽑기를 통해 미션 전용 아이템을 사용해 보세요."), kMyLocalKey_titleLoading19);
	ja->setObject(CCString::create("한 번에 많은 영역을 획득할 수록 여러 개의 미사일이 발사되며, 공격을 성공한 만큼 콤보가 적용됩니다."), kMyLocalKey_titleLoading6);
	ja->setObject(CCString::create("이어하기는 두 번 까지만 사용할 수 있습니다."), kMyLocalKey_titleLoading20);
	ja->setObject(CCString::create("내카드 목록에서 카드 이미지를 터치하면 카드 상세 내용을 보실 수 있습니다."), kMyLocalKey_titleLoading21);
	ja->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading22);
	ja->setObject(CCString::create("85% 로 클리어 하면 별 1개를 획득합니다."), kMyLocalKey_titleLoading23);
	ja->setObject(CCString::create("change를 모두 모은 후 클리어하면 별 2개를 획득합니다."), kMyLocalKey_titleLoading24);
	ja->setObject(CCString::create("100%로 클리어 하면 별 3개를 획득합니다."), kMyLocalKey_titleLoading25);
	ja->setObject(CCString::create("change를 모두 모은 후 100%로 클리어하면 별 4개를 모을 수 있습니다."), kMyLocalKey_titleLoading26);
	ja->setObject(CCString::create("다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요."), kMyLocalKey_titleLoading27);
	ja->setObject(CCString::create("플레이 중 획득한 영역에 따라 골드를 드립니다."), kMyLocalKey_titleLoading28);
	ja->setObject(CCString::create("30%나 할인되는 종합 아이템 구매 찬스를 놓치지 마세요!"), kMyLocalKey_titleLoading29);
	ja->setObject(CCString::create("미션스테이지에서는 미션을 먼저 완료한 후 클리어가 가능합니다."), kMyLocalKey_titleLoading30);
	ja->setObject(CCString::create("미사일 공격 시 그리는 선에 미사일이 닿으면 유도 공격을 당해요."), kMyLocalKey_titleLoading31);
	ja->setObject(CCString::create("게임 중 타임 보너스 아이템을 먹으면 남은 시간에서 10초가 추가됩니다."), kMyLocalKey_titleLoading32);
	ja->setObject(CCString::create("선을 그리다가 꾸욱 버튼에서 손을 떼면 캐릭터가 안전 영역으로 되돌아 갑니다."), kMyLocalKey_titleLoading33);
	ja->setObject(CCString::create("보스에게 연속 공격을 성공시키면 콤보가 적용되며, 게임 결과에서 콤보 보너스를 받을 수 있습니다."), kMyLocalKey_titleLoading34);
	ja->setObject(CCString::create("내가 모은 별의 개수는 메인화면의 왼쪽 상단에 있는 별 아이콘을 통해 확인하실 수 있습니다."), kMyLocalKey_titleLoading35);
	ja->setObject(CCString::create("운석이 떨어지면 캐릭터에 맞지 않도록 주의하세요."), kMyLocalKey_titleLoading36);
	ja->setObject(CCString::create("조이스틱과 꾸욱 버튼의 위치를 바꾸고 싶다면 설정메뉴에서 조이스틱위치를 반대로 변경해 보세요."), kMyLocalKey_titleLoading37);
	ja->setObject(CCString::create("조이스틱의 위치를 고정하고 싶다면 설정메뉴에서 조이스틱조작을 고정으로 변경해 보세요"), kMyLocalKey_titleLoading38);
	ja->setObject(CCString::create("꾸욱 버튼을 누른채 한쪽 방향으로 그린 다음 되돌아 오면 한줄 그리기가 가능합니다."), kMyLocalKey_titleLoading39);
	ja->setObject(CCString::create("그리는 도중 줄이 꼬여도 당황하지 마세요. 줄이 꼬인 상태에서도 영역을 획득할 수 있습니다."), kMyLocalKey_titleLoading40);
	ja->setObject(CCString::create("작은 몬스터에는 HP가 존재하며, 공격이 성공할 때마다 HP가 줄어듭니다."), kMyLocalKey_titleLoading41);
	ja->setObject(CCString::create("액자 하나를 완성할 때마다 특별한 카드와 하트를 선물로 드립니다."), kMyLocalKey_titleLoading42);
	ja->setObject(CCString::create("클리어 점수는 영역획득점수와 콤보점수, 남은 타임 보너스가 합산된 점수입니다."), kMyLocalKey_titleLoading43);
	ja->setObject(CCString::create("고득점을 노린다면 연속 콤보를 성공시켜 보세요."), kMyLocalKey_titleLoading44);
	ja->setObject(CCString::create(""), kMyLocalKey_titleLoadingEnd);
	
	ja->setObject(CCString::create("START"), kMyLocalKey_gamestart);
	ja->setObject(CCString::create("대전 시작"), kMyLocalKey_battleStart);
	ja->setObject(CCString::create("Missile"), kMyLocalKey_mymissile);
	ja->setObject(CCString::create("Damage"), kMyLocalKey_power);
	ja->setObject(CCString::create("Gacha"), kMyLocalKey_gacha);
	ja->setObject(CCString::create("Try one day to complete the mission! Gifts for you!"), kMyLocalKey_stageListDown);
	ja->setObject(CCString::create("액자오픈조건"), kMyLocalKey_frameOpenConditionTitle);
	ja->setObject(CCString::create("이전액자클리어\n+별%d개이상"), kMyLocalKey_frameOpenConditionContent);
	ja->setObject(CCString::create("젬으로오픈"), kMyLocalKey_frameOpenConditionContentRuby);
	ja->setObject(CCString::create("%s\n%d:00-%d:00"), kMyLocalKey_frameOpenConditionContentTimeWeek);
	ja->setObject(CCString::create("%s월%s일%s:%s오픈"), kMyLocalKey_frameOpenConditionContentTimeDate);
	ja->setObject(CCString::create("자세히보기"), kMyLocalKey_detailView);
	ja->setObject(CCString::create("별 획득 방법"), kMyLocalKey_detailConditionPopupTitle);
	ja->setObject(CCString::create("<font newline=14>스테이지를 클리어 할 때마다<font newline=30>클리어 단계에 따라 별을 획득합니다.<font color=961>한 스테이지<font newline=14>에서 획득할 수 있는<font color=961>별 갯수는 총 10개<font newline=14>입니다."), kMyLocalKey_detailConditionPopupContent);
//	ja->setObject(CCString::create("<font color=999 size=12>스테이지에서 <font color=990 size=12>4가지 등급<font color=999 size=12 newline=13>을 모두 Clear하면<font color=990 size=12>별 10개<font color=999 size=12>를 모을 수 있습니다."), kMyLocalKey_detailConditionPopupContent2);
//	ja->setObject(CCString::create("에서 획득할 수 있는"), kMyLocalKey_detailConditionPopupContent3);
//	ja->setObject(CCString::create("별 갯수는 총 10개"), kMyLocalKey_detailConditionPopupContent4);
//	ja->setObject(CCString::create("입니다."), kMyLocalKey_detailConditionPopupContent5);
	ja->setObject(CCString::create("결제 실패"), kMyLocalKey_failPurchase);
	ja->setObject(CCString::create("구매 완료"), kMyLocalKey_successPurchase);
	ja->setObject(CCString::create("지금 상점으로 이동하시겠습니까?"), kMyLocalKey_enoughtGoodsContent);
//	ja->setObject(CCString::create("포기하기"), kMyLocalKey_giveup);
	ja->setObject(CCString::create("상점 바로가기"), kMyLocalKey_goShopButton);
	
	ja->setObject(CCString::create("액자를 플레이할 수 있는 시간이 지났습니다.\n홈으로 돌아갑니다."), kMyLocalKey_timeOutFrame);
	ja->setObject(CCString::create("바로입장"), kMyLocalKey_directEnter);
	ja->setObject(CCString::create("카드 승급 기회!!"), kMyLocalKey_rankUpTitle);
	ja->setObject(CCString::create("승급 성공시 한 단계 높은 카드를 획득할 수 있어요!"), kMyLocalKey_rankUpSubTitle);
	ja->setObject(CCString::create("현재 획득카드"), kMyLocalKey_recentTakeCard);
	ja->setObject(CCString::create("승급확률"), kMyLocalKey_rankUpRate);
	ja->setObject(CCString::create("승급하기"), kMyLocalKey_rankup);
	
	ja->setObject(CCString::create("구매하기"), kMyLocalKey_purchase);
	ja->setObject(CCString::create("첫구매시 할인"), kMyLocalKey_firstPurchaseSale);
	ja->setObject(CCString::create("%s할인은 첫 구매 회원님께만"), kMyLocalKey_firstPurchaseMent1);
	ja->setObject(CCString::create(" 적용됩니다."), kMyLocalKey_firstPurchaseMent2);
	ja->setObject(CCString::create("이 창을 닫으면 %s 할인 기회는 사라집니다."), kMyLocalKey_firstPurchaseMent3);
	ja->setObject(CCString::create("지금 묶음 아이템을 구매하시면"), kMyLocalKey_emptyItemSaleMent1);
	ja->setObject(CCString::create("%s 할인해 드립니다."), kMyLocalKey_emptyItemSaleMent2);
	ja->setObject(CCString::create("플레이를 도와주는 기능성 아이템을"), kMyLocalKey_stupidNpuHelpMent1);
	ja->setObject(CCString::create("%s할인된 가격으로 사용해보세요."), kMyLocalKey_stupidNpuHelpMent2);
	ja->setObject(CCString::create("묶음아이템 할인"), kMyLocalKey_packageItemSale);
	ja->setObject(CCString::create("5번째 구매시"), kMyLocalKey_mileageMent1);
	ja->setObject(CCString::create("아주 특별한 할인혜택의 기회가 있으니"), kMyLocalKey_mileageMent2);
	ja->setObject(CCString::create("혜택을 꼭! 확인해주세요."), kMyLocalKey_mileageMent3);
	ja->setObject(CCString::create("단 한번의 구매 찬스"), kMyLocalKey_eventShopTitle);
	ja->setObject(CCString::create("당신만을 위한 특별한 기회!"), kMyLocalKey_eventShopMent1);
	ja->setObject(CCString::create("%s 보너스 혜택을 드려요."), kMyLocalKey_eventShopMent2);
	ja->setObject(CCString::create("이 창을 닫으면 %s 보너스 기회는 사라집니다."), kMyLocalKey_eventShopMent3);
	ja->setObject(CCString::create("<font>미사일 레벨을 올리면 </font><font color=961>부하몬스터를 쉽게 죽이고,</font>"), kMyLocalKey_levelupGuideMent1);
	ja->setObject(CCString::create("<font color=961>패턴공격을 빨리 저지시킬 수 있으며,</font>"), kMyLocalKey_levelupGuideMent2);
	ja->setObject(CCString::create("<font color=961>고득점 기록에 도움이 됩니다.</font>"), kMyLocalKey_levelupGuideMent3);
	ja->setObject(CCString::create("<font>미사일 업그레이드를 하러 갈까요?</font>"), kMyLocalKey_levelupGuideMent4);
//	ja->setObject(CCString::create("다음에"), kMyLocalKey_next);
	ja->setObject(CCString::create("업그레이드"), kMyLocalKey_levelupGo);
	ja->setObject(CCString::create("업적"), kMyLocalKey_achievement);
	ja->setObject(CCString::create("모든보상 받기"), kMyLocalKey_allRewardGet);
	ja->setObject(CCString::create("종합 아이템 구매 찬스!"), kMyLocalKey_itemPackageChance);
	ja->setObject(CCString::create("아이템이 하나도 없네요."), kMyLocalKey_emptyItemSaleTitle);
	ja->setObject(CCString::create("첫구매 대폭할인!!"), kMyLocalKey_firstPurchaseSaleTitle);
	ja->setObject(CCString::create("사랑합니다. 고객님!"), kMyLocalKey_mileageTitle);
	ja->setObject(CCString::create("미사일 업그레이드"), kMyLocalKey_missileUpgrade);
	ja->setObject(CCString::create("<font>누적영역</font><font color=961>%d%%</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalPercent1);
	ja->setObject(CCString::create("%s%%"), kMyLocalKey_todaymissionTotalPercent2);
	ja->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalPercent3);
	ja->setObject(CCString::create("현재 획득영역"), kMyLocalKey_todaymissionTotalPercent4);
	ja->setObject(CCString::create("<font>누적점수</font><font color=961>%s점</font><font>을 획득하라</font>"), kMyLocalKey_todaymissionTotalScore1);
	ja->setObject(CCString::create("%s점"), kMyLocalKey_todaymissionTotalScore2);
	ja->setObject(CCString::create(" 획득하라!!"), kMyLocalKey_todaymissionTotalScore3);
	ja->setObject(CCString::create("현재 획득점수"), kMyLocalKey_todaymissionTotalScore4);
	ja->setObject(CCString::create("<font color=961>%s골드</font><font>를 획득하라</font>"), kMyLocalKey_todaymissionTotalTakeGold1);
	ja->setObject(CCString::create("%s골드"), kMyLocalKey_todaymissionTotalTakeGold2);
	ja->setObject(CCString::create("현재 획득골드"), kMyLocalKey_todaymissionTotalTakeGold3);
	ja->setObject(CCString::create("<font>부하몹</font><font color=961>%d마리</font><font>를 사냥하라</font>"), kMyLocalKey_todaymissionTotalCatch1);
	ja->setObject(CCString::create("%s마리"), kMyLocalKey_todaymissionTotalCatch2);
	ja->setObject(CCString::create(" 사냥하라!!"), kMyLocalKey_todaymissionTotalCatch3);
	ja->setObject(CCString::create("현재 잡은 부하몹"), kMyLocalKey_todaymissionTotalCatch4);
//	ja->setObject(CCString::create("오늘의 미션을 완료하였습니다."), kMyLocalKey_todaymissionSuccess);
	ja->setObject(CCString::create("완료"), kMyLocalKey_complete);
	ja->setObject(CCString::create("업적달성"), kMyLocalKey_achieveSuccess);
	ja->setObject(CCString::create("업적미완성"), kMyLocalKey_achieveNotSuccess);
	ja->setObject(CCString::create("업적보상"), kMyLocalKey_achieveReward);
	ja->setObject(CCString::create("달성한 업적이 없습니다."), kMyLocalKey_nothingSuccessAchieve);
	ja->setObject(CCString::create("보상받을 업적이 없습니다."), kMyLocalKey_nothingRewardAchieve);
	
	
	ja->setObject(CCString::create("영역획득"), kMyLocalKey_missionTitle0);
	ja->setObject(CCString::create("용감한 기사"), kMyLocalKey_missionTitle1);
	ja->setObject(CCString::create("부하몹 사냥"), kMyLocalKey_missionTitle2);
	ja->setObject(CCString::create("욕심쟁이"), kMyLocalKey_missionTitle3);
	ja->setObject(CCString::create("수집가"), kMyLocalKey_missionTitle4);
	ja->setObject(CCString::create("완벽주의자"), kMyLocalKey_missionTitle5);
	ja->setObject(CCString::create("결벽주의자"), kMyLocalKey_missionTitle6);
	ja->setObject(CCString::create("비지니스맨"), kMyLocalKey_missionTitle7);
	
	
	
	ja->setObject(CCString::create("<font color=961 size=13>%.0f%%이상<font color=999 size=13> 획득하라!"), kMyLocalKey_missionDiscription0);
	ja->setObject(CCString::create("<font color=999 size=13>보스의 에너지를 모두 소진시켜라!"), kMyLocalKey_missionDiscription1);
	ja->setObject(CCString::create("<font color=999 size=13>부하 몬스터를 가두어 잡으세요!"), kMyLocalKey_missionDiscription2);
	ja->setObject(CCString::create("<font color=999 size=13>정해진 횟수만큼 한번에 많이 먹으세요!"), kMyLocalKey_missionDiscription3);
	ja->setObject(CCString::create("<font color=999 size=13>정해진 숫자만큼 아이템을 모으세요!"), kMyLocalKey_missionDiscription4);
	ja->setObject(CCString::create("<font color=999 size=13>정해진 목표로 정확하게 영역을 획득하세요!"), kMyLocalKey_missionDiscription5);
	ja->setObject(CCString::create("<font color=999 size=13>CHANGE를 순서대로 획득하세요!"), kMyLocalKey_missionDiscription6);
	ja->setObject(CCString::create("<font color=999 size=13>제한시간 내에 클리어하세요!"), kMyLocalKey_missionDiscription7);
	
	ja->setObject(CCString::create(""), kMyLocalKey_missionCondition0);
	ja->setObject(CCString::create(""), kMyLocalKey_missionCondition1);
	ja->setObject(CCString::create("<font color=961 size=18>%d마리"), kMyLocalKey_missionCondition2);
	ja->setObject(CCString::create("<font color=961 size=18>%d%%이상 %d번"), kMyLocalKey_missionCondition3);
	ja->setObject(CCString::create("<font>아이템을 </font><font color=961 size=18>%d개</font><font> 모으세요!</font>"), kMyLocalKey_missionCondition4);
	ja->setObject(CCString::create(""), kMyLocalKey_missionCondition5);
	ja->setObject(CCString::create(""), kMyLocalKey_missionCondition6);
	ja->setObject(CCString::create("<font color=961 size=18>%d초"), kMyLocalKey_missionCondition7);
	
	
	ja->setObject(CCString::create("대쉬"), kMyLocalKey_item4title);
	ja->setObject(CCString::create("잠깐동안 빠른속도로 이동 가능합니다."), kMyLocalKey_item4ment);
	ja->setObject(CCString::create("부하몹제거"), kMyLocalKey_item5title);
	ja->setObject(CCString::create("부하 몬스터 한마리를 제거합니다."), kMyLocalKey_item5ment);
	ja->setObject(CCString::create("침묵"), kMyLocalKey_item7title);
	ja->setObject(CCString::create("잠깐동안 몬스터가 공격을 못하도록 합니다."), kMyLocalKey_item7ment);
	ja->setObject(CCString::create("부활"), kMyLocalKey_item10title);
	ja->setObject(CCString::create("마지막 순간에 부활합니다."), kMyLocalKey_item10ment);
	ja->setObject(CCString::create("시간추가"), kMyLocalKey_item8title);
	ja->setObject(CCString::create("제한시간을 늘려줍니다."), kMyLocalKey_item8ment);
	ja->setObject(CCString::create("일시정지"), kMyLocalKey_pause);
	ja->setObject(CCString::create("계속하기"), kMyLocalKey_continue);
	ja->setObject(CCString::create("나가기"), kMyLocalKey_ingameOut);
	ja->setObject(CCString::create("재시작"), kMyLocalKey_ingameReplay);
	ja->setObject(CCString::create("대중교통\n모드"), kMyLocalKey_publicPlaceMode);
//	ja->setObject(CCString::create("이어하기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass1);
//	ja->setObject(CCString::create("맵다시뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass2);
//	ja->setObject(CCString::create("업그레이드 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass3);
//	ja->setObject(CCString::create("아이템뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass4);
//	ja->setObject(CCString::create("1프로뽑기 이용권이 지급되었습니다."), kMyLocalKey_todaymissionSuccessPass5);
	ja->setObject(CCString::create("<font newline=16>오늘의 미션을 완료하였습니다.<font color=961 newline=16>보상은 선물함으로 지급됩니다."), kMyLocalKey_todaymissionSuccessCommon);
	ja->setObject(CCString::create("무료"), kMyLocalKey_free);
	ja->setObject(CCString::create("영역 다시뽑기"), kMyLocalKey_areaReGacha);
//	ja->setObject(CCString::create("버튼을 눌러주세요."), kMyLocalKey_pleaseClickButton);
//	ja->setObject(CCString::create("STOP"), kMyLocalKey_stop);
	ja->setObject(CCString::create("도전하기"), kMyLocalKey_go100percent);
	ja->setObject(CCString::create("계정연결"), kMyLocalKey_accountLink);
	ja->setObject(CCString::create("최고 레벨 %d"), kMyLocalKey_maxLevel);
	ja->setObject(CCString::create("업그레이드 완료"), kMyLocalKey_endUpgrade);
	ja->setObject(CCString::create("%d단계 카드 획득방법"), kMyLocalKey_cardLockedTitle);
	ja->setObject(CCString::create("<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent1);
	ja->setObject(CCString::create("<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent2);
	ja->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent3);
	ja->setObject(CCString::create("<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다."), kMyLocalKey_cardLockedMent4);
	ja->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTime);
	ja->setObject(CCString::create("<font color=999 size=13>남은시간"), kMyLocalKey_todaymissionRemainTimeMinute);
	ja->setObject(CCString::create("<font color=961 size=15>%dHour"), kMyLocalKey_todaymissionRemainTime2);
	ja->setObject(CCString::create("<font color=961 size=15>%dMinute"), kMyLocalKey_todaymissionRemainTimeMinute2);
	ja->setObject(CCString::create("<font color=961 size=15>모든 피스 획득!"), kMyLocalKey_puzzleSuccessTitle);
	ja->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 하트를 드릴께요!"), kMyLocalKey_puzzleSuccessMent);
	ja->setObject(CCString::create("<font color=961 size=15>놀라워요! <font color=961 size=15>퍼펙트 클리어!!"), kMyLocalKey_puzzlePerfectTitle);
	ja->setObject(CCString::create("<font newline=18>모든 스테이지의<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 젬를 드릴께요!"), kMyLocalKey_puzzlePerfectMent);
	ja->setObject(CCString::create("이어하시겠습니까?"), kMyLocalKey_doYouWantToContinue);
	ja->setObject(CCString::create("이어하기"), kMyLocalKey_onContinue);
	ja->setObject(CCString::create("그만하기"), kMyLocalKey_offContinue);
	ja->setObject(CCString::create("이번 스테이지는 아이템을 사용할 수 없습니다."), kMyLocalKey_notUseItem);
	ja->setObject(CCString::create("새로운 아이템 등장!"), kMyLocalKey_newItemTitle);
	ja->setObject(CCString::create("새로운 아이템을 선물로 드릴께요!"), kMyLocalKey_newItemMent);
	ja->setObject(CCString::create("보스가 쏜 미사일이\n선에 닿으면 불씨가 생겨 닿으면 죽어요."), kMyLocalKey_dieTutorial1);
	ja->setObject(CCString::create("선을 그을때 몬스터가\n선에 닿으면 파동이 생겨 죽어요!"), kMyLocalKey_dieTutorial2);
	ja->setObject(CCString::create(""), kMyLocalKey_dieTutorial3);
	ja->setObject(CCString::create("남은시간 "), kMyLocalKey_restTime);
	ja->setObject(CCString::create("%d Day"), kMyLocalKey_restTimeDay);
	ja->setObject(CCString::create("%d Hour"), kMyLocalKey_restTimeHour);
	ja->setObject(CCString::create("%d Min"), kMyLocalKey_restTimeMinute);
	ja->setObject(CCString::create("%d Sec"), kMyLocalKey_restTimeSecond);
	
	ja->setObject(CCString::create("ゲームを終了しますか?"), kMyLocalKey_exit);
	
	
	ja->setObject(CCString::create("Count Bomb"), kMyLocalKey_warning9);
	ja->setObject(CCString::create("Orange Saw"), kMyLocalKey_warning105);
	ja->setObject(CCString::create("Blue Saw"), kMyLocalKey_warning106);
	ja->setObject(CCString::create("Crash Laser"), kMyLocalKey_warning107);
	ja->setObject(CCString::create("Missile"), kMyLocalKey_warning108);
	ja->setObject(CCString::create("Rush"), kMyLocalKey_warning109);
	ja->setObject(CCString::create("Fire Work"), kMyLocalKey_warning110);
	ja->setObject(CCString::create("Sunflower"), kMyLocalKey_warning111);
	ja->setObject(CCString::create("Ice Bomb"), kMyLocalKey_warning112);
	ja->setObject(CCString::create("Guided Bomb"), kMyLocalKey_warning113);
	ja->setObject(CCString::create("Tornado"), kMyLocalKey_warning1001);
	ja->setObject(CCString::create("Flashing"), kMyLocalKey_warning1002);
	ja->setObject(CCString::create("Slow Zone"), kMyLocalKey_warning1003);
	ja->setObject(CCString::create("Thorn Prison"), kMyLocalKey_warning1004);
	ja->setObject(CCString::create("Freezing"), kMyLocalKey_warning1005);
	ja->setObject(CCString::create("Chaos"), kMyLocalKey_warning1006);
	ja->setObject(CCString::create("Teleport"), kMyLocalKey_warning1007);
  ja->setObject(CCString::create("Invisibility"), kMyLocalKey_warning1008);
	ja->setObject(CCString::create("Flame Spraying"), kMyLocalKey_warning1009);
	ja->setObject(CCString::create("ThunderBolt"), kMyLocalKey_warning1010);
	ja->setObject(CCString::create("Speed Laser"), kMyLocalKey_warning1011);
	ja->setObject(CCString::create("Radioactivity"), kMyLocalKey_warning1012);
	ja->setObject(CCString::create("Meteor"), kMyLocalKey_warning1013);
	ja->setObject(CCString::create("Falling Stone"), kMyLocalKey_warning1014);
	ja->setObject(CCString::create("Do not come"), kMyLocalKey_warning1015);
	ja->setObject(CCString::create("Dynamite Mine"), kMyLocalKey_warning1016);
	ja->setObject(CCString::create("Flame Stoker"), kMyLocalKey_warning1017);
	ja->setObject(CCString::create("Sudden Cloude"), kMyLocalKey_warning1018);
	ja->setObject(CCString::create("Boss Attacks Blocked"), kMyLocalKey_warningBossSuccess);
	
	ja->setObject(CCString::create("새로운 보스 공격!"), kMyLocalKey_newBossPattern);
	
	ja->setObject(CCString::create("맵에 부딪힐때마다 숫자가 줄고, 0이 되면 폭발해요."), kMyLocalKey_patternContent9);
	ja->setObject(CCString::create("하나의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent105);
	ja->setObject(CCString::create("여러개의 톱날이 획득영역을 부숴요."), kMyLocalKey_patternContent106);
	ja->setObject(CCString::create("레이저가 획득영역을 부숴요."), kMyLocalKey_patternContent107);
	ja->setObject(CCString::create("그리는도중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent108);
	ja->setObject(CCString::create("보스가 획득영역을 파괴하며 이동해요."), kMyLocalKey_patternContent109);
	ja->setObject(CCString::create("획득영역에서 폭발후 미사일을 퍼트려요."), kMyLocalKey_patternContent110);
	ja->setObject(CCString::create("사방으로 미사일을 발사해요."), kMyLocalKey_patternContent111);
	ja->setObject(CCString::create("연속으로 발사되어 획득영역을 부숴요."), kMyLocalKey_patternContent112);
	ja->setObject(CCString::create("따라와서 폭발해요."), kMyLocalKey_patternContent113);
	ja->setObject(CCString::create("맵의 일부를 일정시간 가립니다."), kMyLocalKey_patternContent1001);
	ja->setObject(CCString::create("일정시간 화면 전체를 뒤덮어 시야를 가려요."), kMyLocalKey_patternContent1002);
	ja->setObject(CCString::create("슬로우존 안에서는 이동속도가 느려져요."), kMyLocalKey_patternContent1003);
	ja->setObject(CCString::create("붉은 원의 테두리에 닿으면 생명 하나를 잃어요."), kMyLocalKey_patternContent1004);
	ja->setObject(CCString::create("일정시간 움직이지 못해요."), kMyLocalKey_patternContent1005);
	ja->setObject(CCString::create("조작 방향의 반대로 움직여요."), kMyLocalKey_patternContent1006);
	ja->setObject(CCString::create("보스가 다른위치로 순간이동해요."), kMyLocalKey_patternContent1007);
	ja->setObject(CCString::create("일정시간 보스가 보이지 않아요."), kMyLocalKey_patternContent1008);
	ja->setObject(CCString::create("획득영역에 불을 피워요."), kMyLocalKey_patternContent1009);
	ja->setObject(CCString::create("캐릭터를 따라오며 맵을 부숴요."), kMyLocalKey_patternContent1010);
	ja->setObject(CCString::create("그리는중 맞지 않도록 주의하세요."), kMyLocalKey_patternContent1011);
	ja->setObject(CCString::create("방사능이 퍼진곳을 조심하세요."), kMyLocalKey_patternContent1012);
	ja->setObject(CCString::create("대각선으로 떨어져 맵을 부숴요."), kMyLocalKey_patternContent1013);
	ja->setObject(CCString::create("위에서 아래로 운석이 떨어져요."), kMyLocalKey_patternContent1014);
	ja->setObject(CCString::create("닿지않도록 주의하세요."), kMyLocalKey_patternContent1015);
	ja->setObject(CCString::create("카운트가 끝나면 폭발해요."), kMyLocalKey_patternContent1016);
	ja->setObject(CCString::create("일정시간 맵을 따라 움직여요."), kMyLocalKey_patternContent1017);
	ja->setObject(CCString::create("언제 폭발할지 몰라요."), kMyLocalKey_patternContent1018);
	

	
	ja->setObject(CCString::create("가운데 빨간 보석이 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다."), kMyLocalKey_tutorial1);
	ja->setObject(CCString::create("먼저 영역 위를 이동하는 방법을 알려드리겠습니다.\n오른쪽 아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있습니다.\n조이스틱으로 캐릭터를 위로 이동시켜보세요."), kMyLocalKey_tutorial2);
	ja->setObject(CCString::create("캐릭터를 위로 이동시키기"), kMyLocalKey_tutorial3);
	ja->setObject(CCString::create("다음에는 영역을 획득하는 방법을 알아보도록 해요.\n왼쪽 아래의 꾸욱 버튼을 누르고 있으면\n영역 바깥으로 나갈 수 있습니다.\n보이는 것처럼 영역을 획득해보세요."), kMyLocalKey_tutorial4);
	ja->setObject(CCString::create("영역획득하기"), kMyLocalKey_tutorial5);
	ja->setObject(CCString::create("파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다."), kMyLocalKey_tutorial6);
	ja->setObject(CCString::create("<font strokesize=0>제한시간 내에 달성도 <font color=961 newline=13 strokesize=0>85%를 넘기면 클리어!!"), kMyLocalKey_tutorial7);
	ja->setObject(CCString::create("기본 튜토리얼을 모두 진행하셨습니다.\n본 게임으로 들아갑니다."), kMyLocalKey_tutorial8);
	
	ja->setObject(CCString::create("겹친선도 획득!"), kMyLocalKey_controlTip_lineTangle_title);
	ja->setObject(CCString::create("꼬인선! 걱정마세요! 그대로 획득할 수 있어요!"), kMyLocalKey_controlTip_lineTangle_content);
	ja->setObject(CCString::create("한줄 먹기!"), kMyLocalKey_controlTip_slimLine_title);
	ja->setObject(CCString::create("캐릭터를 왼쪽으로 이동 후 오른쪽으로 조작하면 OK!"), kMyLocalKey_controlTip_slimLine_content);
	
//	ja->setObject(CCString::create("미션"), kMyLocalKey_mission);
	ja->setObject(CCString::create("PvP"), kMyLocalKey_endlessMode);
	ja->setObject(CCString::create("주간연승랭킹"), kMyLocalKey_endlessWeeklyranking);
//	ja->setObject(CCString::create("정보"), kMyLocalKey_endlessMyInfo);
	ja->setObject(CCString::create("전적"), kMyLocalKey_endlessInfoScore);
	ja->setObject(CCString::create("<font color=999 strokecolor=000 strokesize=0>%d승 %d패 </font><font color=950 strokecolor=000 strokesize=0>(승률%d%%)</font>"), kMyLocalKey_endlessInfoScoreValue2);
//	ja->setObject(CCString::create("%d승 %d패 승률%d%%"), kMyLocalKey_endlessInfoScoreValue);
	ja->setObject(CCString::create("최고점수"), kMyLocalKey_endlessHighScore);
	ja->setObject(CCString::create("최고연승"), kMyLocalKey_endlessHighStraight);
	ja->setObject(CCString::create("<font size=16 color=#FFFFFF>%d</font><font size=10 color=#FFFFFF>연승</font>"), kMyLocalKey_endlessHighStraightValue1);
	ja->setObject(CCString::create("%d연승"), kMyLocalKey_endlessHighStraightValue2);
	ja->setObject(CCString::create("뭔가 문제가 발생하였습니다.\n다시 시도해주세요."), kMyLocalKey_endlessServerError);
	ja->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_endlessDownloadFail);
//	ja->setObject(CCString::create("포기하시면 1패가 추가됩니다.\n계속 하시겠습니까?"), kMyLocalKey_endlessCheckGiveup);
	
	ja->setObject(CCString::create("스페셜데이"), kMyLocalKey_attendanceSpecialTitle);
	ja->setObject(CCString::create("매일 매일 연속 출석!!"), kMyLocalKey_attendanceSeqTitle);
	ja->setObject(CCString::create("연속출석 일수에 따라 더욱 푸짐한 선물을 드립니다."), kMyLocalKey_attendanceSpecialContent);
	
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>골드"), kMyLocalKey_attendanceSpecialGoodsTypeGold);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>젬"), kMyLocalKey_attendanceSpecialGoodsTypeRuby);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem9);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem6);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>개"), kMyLocalKey_attendanceSpecialGoodsTypeItem11);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass1);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass2);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass3);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass4);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass5);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>장"), kMyLocalKey_attendanceSpecialGoodsTypePass6);
	ja->setObject(CCString::create("<font size=13.5>%d<font size=10>종류"), kMyLocalKey_attendanceSpecialGoodsTypeMany);
	
	ja->setObject(CCString::create("<font size=12>%d<font size=8>골드"), kMyLocalKey_attendanceGoodsTypeGold);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>젬"), kMyLocalKey_attendanceGoodsTypeRuby);
	ja->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem9);
	ja->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem6);
	ja->setObject(CCString::create("개"), kMyLocalKey_attendanceGoodsTypeItem11);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass1);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass2);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass3);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass4);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass5);
	ja->setObject(CCString::create("<font size=12>%d<font size=8>장"), kMyLocalKey_attendanceGoodsTypePass6);
	ja->setObject(CCString::create("선물상자"), kMyLocalKey_attendanceGoodsTypeMany);
	
	ja->setObject(CCString::create("출석체크"), kMyLocalKey_attendanceTitle);
	ja->setObject(CCString::create("지급된 보상은 수신함에서 확인 가능합니다."), kMyLocalKey_attendanceContent);
	ja->setObject(CCString::create("DAY"), kMyLocalKey_attendanceDay);
	
	ja->setObject(CCString::create("생명의 돌"), kMyLocalKey_buyMorphingTitle);
	ja->setObject(CCString::create("<font color=961 newline=18>생명의 돌을 사용하면 카드가 살아납니다.<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_buyMorphingContent);
	
	ja->setObject(CCString::create("보유갯수 :"), kMyLocalKey_buyMorphingTake);
	ja->setObject(CCString::create("개"), kMyLocalKey_buyMorphingTakeValue);
	
	ja->setObject(CCString::create("사용하기"), kMyLocalKey_buyMorphingBuy);
	
	ja->setObject(CCString::create("생명의 돌이 부족합니다."), kMyLocalKey_notEnoughtMorphingTitle);
	ja->setObject(CCString::create("<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다."), kMyLocalKey_notEnoughtMorphingContent);
	
	ja->setObject(CCString::create("쿠폰등록"), kMyLocalKey_couponTitle);
	ja->setObject(CCString::create("입력"), kMyLocalKey_couponContent);
	ja->setObject(CCString::create("쿠폰등록성공"), kMyLocalKey_couponSuccess);
	ja->setObject(CCString::create("선물함을 확인하세요."), kMyLocalKey_couponGiftbox);
	ja->setObject(CCString::create("쿠폰등록실패"), kMyLocalKey_couponFail);
	ja->setObject(CCString::create("이미 사용한 쿠폰입니다."), kMyLocalKey_couponAlready);
	ja->setObject(CCString::create("유효기간이 만료 쿠폰입니다."), kMyLocalKey_couponExpire);
	ja->setObject(CCString::create("잘못된 사용입니다."), kMyLocalKey_couponOserror);
	ja->setObject(CCString::create("쿠폰을 찾을 수 없습니다."), kMyLocalKey_couponDontfind);
	ja->setObject(CCString::create("통신이 원활하지 않습니다. 잠시후 다시 시도해주세요."), kMyLocalKey_couponOtherError);
	
	ja->setObject(CCString::create("액자 오픈 조건"), kMyLocalKey_notOpenPuzzleNotEnoughtStarTitle);
	ja->setObject(CCString::create("<font newline=16>별이 부족해요.<font newline=16>더 높은 등급으로 클리어해서<font color=961 newline=16>별 %d개 모아주세요."), kMyLocalKey_notOpenPuzzleNotEnoughtStarContent);
	
	ja->setObject(CCString::create("대전 준비"), kMyLocalKey_endlessStartSettingTitle);
	
	ja->setObject(CCString::create("영역점수"), kMyLocalKey_endlessCalcTitleAreaScore);
	ja->setObject(CCString::create("공격점수"), kMyLocalKey_endlessCalcTitleDamageScore);
	ja->setObject(CCString::create("콤보점수"), kMyLocalKey_endlessCalcTitleComboScore);
	ja->setObject(CCString::create("생명점수"), kMyLocalKey_endlessCalcTitleLifeScore);
	ja->setObject(CCString::create("시간점수"), kMyLocalKey_endlessCalcTitleTimeScore);
	ja->setObject(CCString::create("등급점수"), kMyLocalKey_endlessCalcTitleGradeScore);
	ja->setObject(CCString::create("획득골드"), kMyLocalKey_endlessCalcTitleTakeGold);
	ja->setObject(CCString::create("획득영역"), kMyLocalKey_endlessCalcTitleTakeArea);
	ja->setObject(CCString::create("피격점수"), kMyLocalKey_endlessCalcTitleDamagedScore);
	
	ja->setObject(CCString::create("승"), kMyLocalKey_win);
	ja->setObject(CCString::create("게임결과"), kMyLocalKey_gameResult);
	ja->setObject(CCString::create("실패"), kMyLocalKey_fail);
	ja->setObject(CCString::create("달성!!"), kMyLocalKey_achieve);
	ja->setObject(CCString::create("닫기"), kMyLocalKey_close);
	ja->setObject(CCString::create("%d연승중"), kMyLocalKey_endlessIngWin);
	
	ja->setObject(CCString::create("PVP 오픈조건"), kMyLocalKey_endlessOpenConditionTitle);
	ja->setObject(CCString::create("<font color=961 newline=16>PVP는 %d스테이지 플레이 후<font newline=16>자동으로 오픈됩니다."), kMyLocalKey_endlessOpenConditionContent);
	
	ja->setObject(CCString::create("대전패배"), kMyLocalKey_endlessKeepWin);
	
	ja->setObject(CCString::create("연승저장"), kMyLocalKey_endlessKeepWinTitle);
	ja->setObject(CCString::create("연승 정보는 유지되며, 다시 PvP 도전 시\n저장된 연승에 이어서 플레이할 수 있습니다."), kMyLocalKey_endlessKeepWinContent);
	
	ja->setObject(CCString::create("%d연승 도전"), kMyLocalKey_endlessReadyIngWin);
	
	ja->setObject(CCString::create("닉네임 오류"), kMyLocalKey_nicknameError);
	ja->setObject(CCString::create("<font color=#FFFFFF strokesize=1 >남은 판수 </font>"
																 "<font color=#FFAA14 size=12 strokesize=1>%d회 / </font>"
																 "<font color=#FFFFFF strokesize=1>성공시 </font>"
																 "<font color=#FFAA14 size=12 strokesize=1>%.1f%% 획득</font>"), kMyLocalKey_onePercentDesc2);

	
	ja->setObject(CCString::create("대전상대를 찾는 중입니다."), kMyLocalKey_rivalSearch);
	
	ja->setObject(CCString::create("%d레벨"), kMyLocalKey_rivalLevel);
	ja->setObject(CCString::create("%d 연승 중"), kMyLocalKey_rivalIngWin);
	ja->setObject(CCString::create("대전상대를 선택해주세요."), kMyLocalKey_rivalSelect);
	
	ja->setObject(CCString::create("업적완료"), kMyLocalKey_achieveSuccess2);
	
	ja->setObject(CCString::create("소녀들의 위치를 파악하고 있습니다."), kMyLocalKey_titleTempScript1);
	ja->setObject(CCString::create("소녀들의 의상을 점검하고 있습니다."), kMyLocalKey_titleTempScript2);
	ja->setObject(CCString::create("소녀들을 불러모으고 있습니다."), kMyLocalKey_titleTempScript3);
	

	ja->setObject(CCString::create("하트 공짜"), kMyLocalKey_heartFree);

	ja->setObject(CCString::create("지난주 랭킹 결과"), kMyLocalKey_rankRewardTitle);
	ja->setObject(CCString::create("보상선물들은 선물함에서 확인하세요."), kMyLocalKey_rankRewardContent);
	ja->setObject(CCString::create("주간 누적 랭킹"), kMyLocalKey_rankRewardStageTitle);
	ja->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardStageScore);
	ja->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardStageRank);
	ja->setObject(CCString::create("지난주 플레이횟수"), kMyLocalKey_rankRewardStagePlayCount);
	ja->setObject(CCString::create("PvP 랭킹"), kMyLocalKey_rankRewardEndlessTitle);
	ja->setObject(CCString::create("지난주 누적점수"), kMyLocalKey_rankRewardEndlessScore);
	ja->setObject(CCString::create("지난주 내순위"), kMyLocalKey_rankRewardEndlessRank);
	ja->setObject(CCString::create("지난주 연승기록"), kMyLocalKey_rankRewardEndlessVictory);
	ja->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardStageReward);
	ja->setObject(CCString::create("선물함에서\n%s를 받으세요."), kMyLocalKey_rankRewardEndlessReward);
	
	ja->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardStageRankValue);
	ja->setObject(CCString::create("위(%0.f%%)"), kMyLocalKey_rankRewardEndlessRankValue);
	
	ja->setObject(CCString::create("%s연승"), kMyLocalKey_rankRewardEndlessVictoryScript);
	
	ja->setObject(CCString::create("%d위"), kMyLocalKey_rankRewardRankValue);
	
	ja->setObject(CCString::create("%s골드"), kMyLocalKey_rankRewardGoodsTypeGold);
	ja->setObject(CCString::create("%s젬"), kMyLocalKey_rankRewardGoodsTypeRuby);
	ja->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem9);
	ja->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem6);
	ja->setObject(CCString::create("아이템"), kMyLocalKey_rankRewardGoodsTypeItem11);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass1);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass2);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass3);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass4);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass5);
	ja->setObject(CCString::create("티켓"), kMyLocalKey_rankRewardGoodsTypePass6);
	ja->setObject(CCString::create("선물상자"), kMyLocalKey_rankRewardGoodsTypeMany);
	
	ja->setObject(CCString::create("카드 승급 이벤트!"), kMyLocalKey_rankUpEventTitle);
	ja->setObject(CCString::create("승급 비용 공짜! 승급 확률 100%!!"), kMyLocalKey_rankUpEventContent);
	
	ja->setObject(CCString::create("아니오"), kMyLocalKey_no);
	ja->setObject(CCString::create("네"), kMyLocalKey_yes);
	ja->setObject(CCString::create("<font>기본 클리어조건 <font color=961 newline=17>%.0f%%를<font color=961>%.0f%%로 하향<font newline=17> 시켜드립니다.<font newline=17>적용하시겠습니까?"), kMyLocalKey_clearTimeEventMent);
	ja->setObject(CCString::create("이벤트"), kMyLocalKey_clearTimeEventTitle);
	
	ja->setObject(CCString::create("숨겨진 업적"), kMyLocalKey_achieveHiddenTitle);
	ja->setObject(CCString::create("저를 찾아주세요! 곳곳에 숨겨져 있어요!"), kMyLocalKey_achieveHiddenContent);
	
	ja->setObject(CCString::create("<font newline=20>액자를 선택하면<font color=961 newline=20>퍼즐화면으로 이동합니다."), kMyLocalKey_mainflowDimmed1);
	ja->setObject(CCString::create("<font newline=16>획득한<font color=999>별갯수"), kMyLocalKey_mainflowDimmed2);
	ja->setObject(CCString::create("<font newline=20>선물함"), kMyLocalKey_mainflowDimmed3);
	ja->setObject(CCString::create("<font newline=20>업적"), kMyLocalKey_mainflowDimmed4);
	ja->setObject(CCString::create("<font newline=20>설정"), kMyLocalKey_mainflowDimmed5);
	ja->setObject(CCString::create("<font newline=20>이벤트"), kMyLocalKey_mainflowDimmed6);
	ja->setObject(CCString::create("<font newline=20>랭킹"), kMyLocalKey_mainflowDimmed7);
	ja->setObject(CCString::create("<font newline=20>상점"), kMyLocalKey_mainflowDimmed8);
	ja->setObject(CCString::create("<font newline=20>카드"), kMyLocalKey_mainflowDimmed9);
	ja->setObject(CCString::create("<font newline=16>오늘의<font newline=16>미션"), kMyLocalKey_mainflowDimmed10);
	
	ja->setObject(CCString::create("모든 피스 획득!"), kMyLocalKey_puzzleClearCardTitle);
	ja->setObject(CCString::create("<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzleClearCardContent);
	ja->setObject(CCString::create("모든 카드 획득!"), kMyLocalKey_puzzlePerfectCardTitle);
	ja->setObject(CCString::create("<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!"), kMyLocalKey_puzzlePerfectCardContent);
	
	ja->setObject(CCString::create("%d연승 돌파!"), kMyLocalKey_endlessSeqWinRewardTitle);
	ja->setObject(CCString::create("%d연승을 축하드립니다!"), kMyLocalKey_endlessSeqWinRewardContent);
	ja->setObject(CCString::create("<font newline=18>%d골드를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeGold);
	ja->setObject(CCString::create("<font newline=18>%d젬을 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby);
	ja->setObject(CCString::create("<font newline=18>기본속도업아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9);
	ja->setObject(CCString::create("<font newline=18>더블아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6);
	ja->setObject(CCString::create("<font newline=18>자석아이템 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11);
	ja->setObject(CCString::create("<font newline=18>이어하기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass1);
	ja->setObject(CCString::create("<font newline=18>맵가챠 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass2);
	ja->setObject(CCString::create("<font newline=18>업그레이드 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass3);
	ja->setObject(CCString::create("<font newline=18>아이템뽑기 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass4);
	ja->setObject(CCString::create("<font newline=18>1프로찬스 이용권 %d개를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass5);
	ja->setObject(CCString::create("<font newline=18>생명의 돌을 %d개를 드립니다.<font newline=18>생명의 돌은 내카드 다이어리에서 사용하세요."), kMyLocalKey_endlessSeqWinRewardGoodsTypePass6);
	ja->setObject(CCString::create("<font newline=18>선물상자를 드립니다."), kMyLocalKey_endlessSeqWinRewardGoodsTypeMany);
	
	ja->setObject(CCString::create("퍼즐에서 획득한 카드입니다."), kMyLocalKey_cardSettingMyCardContent);
	ja->setObject(CCString::create("스페셜 카드 보기"), kMyLocalKey_cardSettingMyCardToSpecial);
	
	ja->setObject(CCString::create("모든 피스\n획득시!"), kMyLocalKey_cardSettingClearCardMent);
	ja->setObject(CCString::create("퍼펙트\n클리어시!"), kMyLocalKey_cardSettingPerfectCardMent);
	
	ja->setObject(CCString::create("스페셜 카드"), kMyLocalKey_cardSettingSpecialCardTitle);
	ja->setObject(CCString::create("이벤트를 통해 얻을 수 있는 카드입니다."), kMyLocalKey_cardSettingSpecialCardContent);
	ja->setObject(CCString::create("내 카드 보기"), kMyLocalKey_cardSettingSpecialCardToMyCard);
	
	ja->setObject(CCString::create("<font newline=16>PVP모드가<font newline=16>오픈되었어요!"), kMyLocalKey_endlessTutorialMent1);
	ja->setObject(CCString::create("<font newline=16>내 정보 입니다."), kMyLocalKey_endlessTutorialMent2);
	ja->setObject(CCString::create("<font>목록에서 <font color=961>다른 친구를 선택<font newline=16>해보세요.<font newline=16>다른 친구의 정보를 볼 수 있어요."), kMyLocalKey_endlessTutorialMent3);
	ja->setObject(CCString::create("<font color=961>대전준비를 눌러보세요."), kMyLocalKey_endlessTutorialMent4);
	ja->setObject(CCString::create("<font color=961>대전시작을 눌러보세요."), kMyLocalKey_endlessTutorialMent5);
	ja->setObject(CCString::create("<font color=961>내 미니맵"), kMyLocalKey_endlessTutorialMent6);
	ja->setObject(CCString::create("<font color=961>내 점수"), kMyLocalKey_endlessTutorialMent7);
	ja->setObject(CCString::create("<font color=961>내 장착아이템"), kMyLocalKey_endlessTutorialMent8);
	ja->setObject(CCString::create("<font color=961>상대 정보"), kMyLocalKey_endlessTutorialMent9);
	ja->setObject(CCString::create("<font color=961 newline=16>5콤보 달성시 마다<font color=961 newline=16>미사일을 발사하여<font color=961 newline=16>상대의 점수를 깎습니다."), kMyLocalKey_endlessTutorialMent10);
	ja->setObject(CCString::create("화면을 터치하면 게임 진행됩니다."), kMyLocalKey_endlessTutorialMent11);
	ja->setObject(CCString::create("<font color=961>나의 결과"), kMyLocalKey_endlessTutorialMent12);
	ja->setObject(CCString::create("<font color=961>상대방 결과"), kMyLocalKey_endlessTutorialMent13);
	ja->setObject(CCString::create("<font>스크롤하면 점수 내역을 볼 수 있어요."), kMyLocalKey_endlessTutorialMent14);
	ja->setObject(CCString::create("<font color=961>2연승에 도전할 수 있어요!"), kMyLocalKey_endlessTutorialMent15);
	ja->setObject(CCString::create("잠깐!"), kMyLocalKey_endlessTutorialMent16);
	ja->setObject(CCString::create("3연승을 할 때마다 생명의 돌을 1개\n받을 수 있습니다. 생명의 돌은 내가 가진\n카드에 생명을 불어넣을 수 있는\n특별한 아이템이니 소중히 사용해주세요.\n튜토리얼을 마치겠습니다. 그럼 전 이만!"), kMyLocalKey_endlessTutorialMent17);
	ja->setObject(CCString::create("<font>대전 상대를 선택해주세요."), kMyLocalKey_endlessTutorialMent18);
	
	ja->setObject(CCString::create("에피소드 %d"), kMyLocalKey_puzzleValue);
	
	ja->setObject(CCString::create("보너스 영역 뽑기"), kMyLocalKey_onePercentTutorial1);
	ja->setObject(CCString::create("<font color=#FFFFFF size=10 newline=12>안녕하세요!</font>"
																 "<font color=#FFFFFF size=10 newline=12>난 카츠라기라고 해요.</font>"
																 "<font color=#FFFFFF size=10 newline=12>아슬아슬하게 영역을</font>"
																 "<font color=#FFFFFF size=10 newline=24>100% 획득 못했군요.</font>"
																 "<font color=#FFFFFF size=10 newline=12>100% 영역획득 할 수 있게</font>"
																 "<font color=#FFFFFF size=10>내가 도와줄게요.</font>"), kMyLocalKey_onePercentTutorial2);
	ja->setObject(CCString::create("다  음"), kMyLocalKey_onePercentTutorial3);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=16>아래 게이지바에서 노란색 영역을 맞히면</font>"
								   "<font color=#FFAA14 size=12 newline=16>0.3%를 추가로 획득할 수 있습니다.</font>"
								   "<font color=#FFFFFF size=12 newline=30>기회는 총 3번 주어집니다.</font>"
								   "<font color=#FFAA14 size=14 newline=15>자 그럼 100% 에 도전해 볼까요?</font>"), kMyLocalKey_onePercentTutorial4);
	
	ja->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>뽑기 기회는 단 3번!!</font><font color=#FFFFFF size=12>부족한 영역을 채워 100%에 도전하세요.</font>"), kMyLocalKey_onePercentGame1);
	ja->setObject(CCString::create("<font color=#FFAA14 size=12 newline=16>100%에 성공시</font><font color=#FFAA14 size=12 tag=1>한 단계 높은 카드</font><font color=#FFFFFF size=12>를 획득할 수 있어요.</font>"), kMyLocalKey_onePercentGame2);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12>현재 획득 영역</font><font color=#FFFF00 size=30 strokecolor=000 strokesize=2 tag=1>99.9%</font>"), kMyLocalKey_onePercentGame3);
	ja->setObject(CCString::create("<font size=12 color=#FFFFFF newline=16>노란색 영역에 맞게</font>"
								   "<font size=12 color=#FFAA14>멈춰 버튼 터치</font><font size=12 color=#FFFFFF>하세요!</font>"), kMyLocalKey_onePercentGame4);
	ja->setObject(CCString::create("멈춰!"), kMyLocalKey_onePercentGame5);
	ja->setObject(CCString::create("100% 실패"), kMyLocalKey_onePercentGame6);
	ja->setObject(CCString::create("<font color=#FF0000 size=12>최종 획득 영역     </font>"
								   "<font color=#FF0000 size=30>%.1f%%</font>"), kMyLocalKey_onePercentGame7);
	ja->setObject(CCString::create("닫기"), kMyLocalKey_onePercentGame8);
	ja->setObject(CCString::create("100% 달성!!"), kMyLocalKey_onePercentGame9);
	ja->setObject(CCString::create("카드 받기"), kMyLocalKey_onePercentGame10);
	
	ja->setObject(CCString::create("보상내용"), kMyLocalKey_endlessRewardInfo);
	
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>가위 바위 보 중</font>"
								   "<font color=#FFAA14 size=12>하나를 선택 해 주세요.</font>"), kMyLocalKey_gababoContent1);
	ja->setObject(CCString::create("<font size=12 newline=13>연</font>"
								   "<font size=12 newline=13>승</font>"
								   "<font size=12 newline=13>보</font>"
								   "<font size=12 newline=13>상</font>"), kMyLocalKey_gababoContent2);
	ja->setObject(CCString::create("보상"), kMyLocalKey_gababoContent3);
	ja->setObject(CCString::create("1승"), kMyLocalKey_gababoContent4);
	ja->setObject(CCString::create("2승"), kMyLocalKey_gababoContent5);
	ja->setObject(CCString::create("3승"), kMyLocalKey_gababoContent6);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent7);
	ja->setObject(CCString::create("확 인"), kMyLocalKey_gababoContent8);
	ja->setObject(CCString::create("나"), kMyLocalKey_gababoContent9);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런, 무승부네요!</font>"
								   "<font color=#FFAA14 size=12>한번 더!</font>"), kMyLocalKey_gababoContent10);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>"), kMyLocalKey_gababoContent11);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>와우! 4판 모두 이기다니 정말 대단해요!</font>"
																 "<font color=#FFAA14 size=12 newline=14>최고의 보상을 드릴게요!</font>"
																 "<font color=#FFAA14 size=12 newline=14>보너스 게임은 퍼펙트 클리어시마다</font>"
																 "<font color=#FFAA14 size=12 newline=14>할 수 있어요!</font><font color=#FFFFFF size=12>그럼 다음에 또 만나요!</font>"), kMyLocalKey_gababoContent12);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>이런이런... 지고 말았네요.</font>"
																 "<font color=#FFFFFF size=12>너무 상심마세요!</font>"
																 "<font color=#FFAA14 size=12 newline=14>보너스게임은</font>"
																 "<font color=#FFAA14 size=12 newline=14>퍼펙트 클리어시마다 할 수 있어요!</font>"
																 "<font color=#FFFFFF size=12>다음엔 꼭 저를 이겨보세요!</font>"), kMyLocalKey_gababoContent13);
	ja->setObject(CCString::create("루우비~!"), kMyLocalKey_gababoContent14);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>보너스 게임에 오신 것을 환영합니다!</font>"
								   "<font color=#FFFFFF size=12 newline=14>벌써 퍼즐을 퍼펙트 클리어 하시다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12>게임 설명은 최초 1회만 해드리니 잘 봐주세요!</font>"), kMyLocalKey_gababoContent15);
	ja->setObject(CCString::create("다 음"), kMyLocalKey_gababoContent16);
	ja->setObject(CCString::create("<font color=#FFFFFF size=12 newline=14>게임은 가위바위보 입니다!</font>"
								   "<font color=#FFAA14 size=12 newline=14>가위바위보 중 하나를 선택하여 저를 이기시면 됩니다.</font>"
								   "<font color=#FFAA14 size=12 newline=14>연승 횟수에 따라 더 큰 보상이 있어요!</font>"
								   "<font color=#FFFFFF size=12 newline=14>그럼 행운을 빌어요!</font>"), kMyLocalKey_gababoContent17);
	
	ja->setObject(CCString::create("+%d"), kMyLocalKey_gababoReward);
	
	ja->setObject(CCString::create("PVP 튜토리얼 중에는 나가실 수 없습니다."), kMyLocalKey_notGohomeEndlessTutorialMent);
	ja->setObject(CCString::create("PVP 중에는 재시도할 수 없습니다."), kMyLocalKey_notReplayEndlessMent);
	
	ja->setObject(CCString::create("대중교통 모드"), kMyLocalKey_optionSecretTitle);
	ja->setObject(CCString::create("<font newline=16>대중교통모드가 적용중입니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 해제하실 수 있습니다."), kMyLocalKey_optionSecretOn);
	ja->setObject(CCString::create("<font newline=16>대중교통모드가 풀렸습니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 적용하실 수 있습니다."), kMyLocalKey_optionSecretOff);
	
	ja->setObject(CCString::create("카드 선물하기"), kMyLocalKey_cardGiftTitle);
	ja->setObject(CCString::create("<font color=999>내 아이디는 <font color=961>%s<font color=999> 입니다."), kMyLocalKey_cardGiftMyID);
	ja->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_cardGiftInputText);
	ja->setObject(CCString::create("검색"), kMyLocalKey_cardGiftSearch);
	ja->setObject(CCString::create("선물하기"), kMyLocalKey_cardGiftSend);
	ja->setObject(CCString::create("검색결과가 없습니다."), kMyLocalKey_cardGiftNotFound);
	
	ja->setObject(CCString::create("<font color=999>실패시 다음번 승급 성공확률이 <font color=961>%.0f%%<font color=999> 올라갑니다."), kMyLocalKey_rankUpFailRateUp);
	ja->setObject(CCString::create("<font color=999>이벤트를 놓치지 마세요!!"), kMyLocalKey_rankUpEventSubMent);
	
	ja->setObject(CCString::create("TIME OVER"), kMyLocalKey_failTitleTimeover);
	ja->setObject(CCString::create("MISSION FAIL"), kMyLocalKey_failTitleMissionfail);
	ja->setObject(CCString::create("GAME OVER"), kMyLocalKey_failTitleGameover);
	
	ja->setObject(CCString::create("STAGE CLEAR"), kMyLocalKey_clearTitle);
	ja->setObject(CCString::create("Warning!"), kMyLocalKey_warningDie);
	ja->setObject(CCString::create("쿠폰 번호를 입력해주세요."), kMyLocalKey_couponMent);
	
	ja->setObject(CCString::create("내 정보"), kMyLocalKey_nickTitle);
	ja->setObject(CCString::create("추가지급"), kMyLocalKey_plusGive);
	
	ja->setObject(CCString::create("정보"), kMyLocalKey_pvpInfo);
	ja->setObject(CCString::create("보상내용"), kMyLocalKey_pvpReward);
	
	ja->setObject(CCString::create("선택"), kMyLocalKey_choice);
	ja->setObject(CCString::create("스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다."), kMyLocalKey_stageImgLoadFail);
	
	ja->setObject(CCString::create("카드선물"), kMyLocalKey_cardGiftSuccessTitle);
	ja->setObject(CCString::create("카드를 선물하였습니다."), kMyLocalKey_cardGiftSuccessContent);
	ja->setObject(CCString::create("프로필"), kMyLocalKey_profile);
	ja->setObject(CCString::create("카드 속성"), kMyLocalKey_cardElemental);
	
	ja->setObject(CCString::create("터치"), kMyLocalKey_touch);
	ja->setObject(CCString::create("사운드"), kMyLocalKey_sound);
	ja->setObject(CCString::create("속성(%s)"), kMyLocalKey_elementalValue);
	
	ja->setObject(CCString::create("내카드수 "), kMyLocalKey_myCardCount);
	ja->setObject(CCString::create("다이어리 보기"), kMyLocalKey_diaryView);
	
	ja->setObject(CCString::create("보유장수 %d"), kMyLocalKey_cardTakeCnt);
	ja->setObject(CCString::create("<font newline=20>땅을 획득하려면<font color=961 newline=20>꾸욱 버튼을 누른상태에서<font newline=20>조이스틱을 움직여주세요!"), kMyLocalKey_buttonTutorialMent);
	
	ja->setObject(CCString::create("\"한 번 입력한 닉네임은 변경할 수 없으니 신중히 선택해 주세요!\""), kMyLocalKey_nickBottomMent);
	ja->setObject(CCString::create("아이디를 입력해주세요."), kMyLocalKey_pleaseInputID);
	ja->setObject(CCString::create("유효하지 않은 아이디 입니다."), kMyLocalKey_invalidID);
	ja->setObject(CCString::create("다음"), kMyLocalKey_tutorialNextButton);
	
	ja->setObject(CCString::create("<font newline=16>선택된</font><font>스테이지</font>"), kMyLocalKey_puzzleDimmed1);
	ja->setObject(CCString::create("<font>스테이지</font>"), kMyLocalKey_puzzleDimmed2);
	ja->setObject(CCString::create("<font newline=16>스테이지</font><font>클리어 정보</font>"), kMyLocalKey_puzzleDimmed3);
	ja->setObject(CCString::create("<font>게임준비</font>"), kMyLocalKey_puzzleDimmed4);
	
	ja->setObject(CCString::create("다시보지않기"), kMyLocalKey_noReview);
	ja->setObject(CCString::create("젬 100개"), kMyLocalKey_gem100);
	
	ja->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>PvP랭킹은 연승이 중요합니다.</font>"), kMyLocalKey_endlessOpeningMarquee1);
	ja->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>연승수가 같은 경우 누적점수로 랭킹이 결정됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee2);
	ja->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>PvP점수는 주간점수 랭킹에 추가되지 않습니다.</font>"), kMyLocalKey_endlessOpeningMarquee3);
	ja->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>한 주간의 기록 중 가장 높은 연승기록으로 랭킹이 결정됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee4);
	ja->setObject(CCString::create("<font size=12 strokesize=0.5 strokeopacity=150>랭킹은 매주 월요일 5시에 초기화 됩니다.</font>"), kMyLocalKey_endlessOpeningMarquee5);
	ja->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>이번주 플레이 누적 점수입니다.</font>"), kMyLocalKey_rankNewMarquee1);
	ja->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>PvP점수는 합산되지 않습니다.</font>"), kMyLocalKey_rankNewMarquee2);
	ja->setObject(CCString::create("<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>주간랭킹은 매주 월요일 5시에 초기화 됩니다.</font>"), kMyLocalKey_rankNewMarquee3);
	
	ja->setObject(CCString::create("드디어 오늘부터 닌자훈련을 시작하는구나!!\n할아버지의 명성에 걸 맞는\n훌륭한 닌자가 될꺼야!"), kMyLocalKey_scenarioMent1);
	ja->setObject(CCString::create("여기가 훈련장인가?\n인기척도 없고 전력으로 훈련할 수 있겠는데?!"), kMyLocalKey_scenarioMent2);
	ja->setObject(CCString::create("안녕하세요.\n저는 한조국립대 학생 리더인 이카루가입니다."), kMyLocalKey_scenarioMent3);
	ja->setObject(CCString::create("앗 안녕하세요.\n저는 아스카 라고합니다."), kMyLocalKey_scenarioMent4);
	ja->setObject(CCString::create("네. 전설의 닌자 한조님의 손녀라고\n들었습니다. 잘 부탁 드립니다.\n훈련 전에 선생님께서 간단한 시험을\n하신다는군요."), kMyLocalKey_scenarioMent5);
	ja->setObject(CCString::create("으윽, 시험은 그다지 좋아하지 않지만\n알겠습니다."), kMyLocalKey_scenarioMent6);
	ja->setObject(CCString::create("어떤 시험인가요?"), kMyLocalKey_scenarioMent7);
	ja->setObject(CCString::create("시노비 결계 속에서 대전하는 법을\n익히시면 됩니다."), kMyLocalKey_scenarioMent8);
	ja->setObject(CCString::create("네. 그러면 잘 부탁 드리겠습니다."), kMyLocalKey_scenarioMent9);
	ja->setObject(CCString::create("그전에 간단히 메뉴들을 소개해 드릴께요."), kMyLocalKey_scenarioMent10);
	ja->setObject(CCString::create("숙지하겠습니다."), kMyLocalKey_scenarioMent11);
	ja->setObject(CCString::create("그럼 시험장으로 이동 하죠.\n1번액자로 입장합니다."), kMyLocalKey_scenarioMent12);
	ja->setObject(CCString::create("이곳은 스테이지를 선택할 수 있는\n퍼즐화면입니다."), kMyLocalKey_scenarioMent13);
	ja->setObject(CCString::create("먼저 1스테이지로 이동하시죠."), kMyLocalKey_scenarioMent14);
	ja->setObject(CCString::create("이곳은 게임시작 전 아이템이나\n무기를 업그레이드 시킬 수 있는\n게임시작화면입니다."), kMyLocalKey_scenarioMent15);
	ja->setObject(CCString::create("게임을 시작하려면 하트가 필요합니다."), kMyLocalKey_scenarioMent16);
	ja->setObject(CCString::create("지금은 하트가 충분하니\n바로 시작해볼까요?"), kMyLocalKey_scenarioMent17);
	ja->setObject(CCString::create("이런곳은 처음인데 여긴 어디지?"), kMyLocalKey_scenarioMent18);
	ja->setObject(CCString::create("시노비 결계 속 입니다.\n일반인의 안전을 위해 닌자들은\n이곳에서 훈련하죠."), kMyLocalKey_scenarioMent19);
	ja->setObject(CCString::create("이곳은 처음이실테니\n컨트롤 방법부터 설명 드리겠습니다."), kMyLocalKey_scenarioMent20);
	ja->setObject(CCString::create("제한시간 내에 달성도 85%를 넘기면\n시험 합격입니다.\n그럼 시작해볼까요?"), kMyLocalKey_scenarioMent21);
	ja->setObject(CCString::create("잠깐!\n깜빡하고 지나갈 뻔했네요."), kMyLocalKey_scenarioMent22);
	ja->setObject(CCString::create("시험을 잘 볼 수 있게 도와드릴게요.\n \n왼쪽에 보이는 것이 지금 플레이하고 있는\n스테이지의 미니맵이예요.\n여기서 보스와 나의 위치를 파악하면 되겠죠?"), kMyLocalKey_scenarioMent23);
	ja->setObject(CCString::create("게임을 잠시 멈추거나 게임을 나가고 싶다면\n일시정지 버튼을 눌러주세요.\n일시정지에는 유용한 기능들이 있으니\n한번 확인해보세요."), kMyLocalKey_scenarioMent24);
	ja->setObject(CCString::create("여기에 점수와 획득한 골드, 그리고 콤보가\n표시됩니다. 자 이제 시작해 볼까요?"), kMyLocalKey_scenarioMent25);
	ja->setObject(CCString::create("축하합니다. 첫 번째 시험을 통과하셨군요.\n게임이 끝나면 이곳으로 오게 됩니다."), kMyLocalKey_scenarioMent26);
	ja->setObject(CCString::create("스테이지 랭킹"), kMyLocalKey_scenarioMent27);
	ja->setObject(CCString::create("점수 정보"), kMyLocalKey_scenarioMent28);
	ja->setObject(CCString::create("클리어 등급"), kMyLocalKey_scenarioMent29);
	ja->setObject(CCString::create("그럼 계속 해볼까요?\n5스테이지 클리어 후 메인화면에서\n뵙기로 하죠!"), kMyLocalKey_scenarioMent30);
	ja->setObject(CCString::create("네! 그럼 최선을 다 해보겠습니다!"), kMyLocalKey_scenarioMent31);
	ja->setObject(CCString::create("이런.. 다음엔 더 잘 할 수 있겠죠?\n여긴 게임이 끝나면 오게 됩니다."), kMyLocalKey_scenarioMent32);
	ja->setObject(CCString::create("휴.. 이걸로 시험끝!!\n드디어 훈련시작이네~!"), kMyLocalKey_scenarioMent33);
	ja->setObject(CCString::create("어랏. 아스카도 한조국립대에서 훈련하는거야?\n같이 최고의 닌자를 꿈꿀 수 있겠구나!!"), kMyLocalKey_scenarioMent34);
	ja->setObject(CCString::create("쉿. 일반 학생들이 다니는 교정 안에서는\n닌자에 관한 이야기는 하지 않는게 좋아.\n이곳의 닌자 양성 클래스는\n일반 학생들에겐 비밀이니까!"), kMyLocalKey_scenarioMent35);
	ja->setObject(CCString::create("흐응... 비밀이라니...\n조금 긴장하게 되.."), kMyLocalKey_scenarioMent36);
	ja->setObject(CCString::create("히바리, 긴장할 필요 까진 없어.\n언제나처럼 시노비결계 안에서 훈련하기\n때문에 보통 사람들은 볼 수 없으니까."), kMyLocalKey_scenarioMent37);
	ja->setObject(CCString::create("응, 알고는 있는데... 아스카짱 고마워\n앞으로나 완전 열심히 할꺼야!"), kMyLocalKey_scenarioMent38);
	ja->setObject(CCString::create("히바리 위험해!!!\n긴장을 늦추지 마. 적은 남아 있다구!!"), kMyLocalKey_scenarioMent39);
	ja->setObject(CCString::create("야규! 히바리를 도와주었구나!"), kMyLocalKey_scenarioMent40);
	ja->setObject(CCString::create("어... 어떻게 라는 건 잘 모르겠고.\n교내에도 아직 적이 남아 있으니\n주변의 적을 해치우자."), kMyLocalKey_scenarioMent41);
	ja->setObject(CCString::create("야규, 아까는 도와줘서 고마워.\n히바리가 거치적거리니까\n이대로면 모두한테 폐를 끼치게 돼."), kMyLocalKey_scenarioMent42);
	ja->setObject(CCString::create("히바리는 거치적 거리는 존재 따위가 아니야.\n우리들은 모두 강하다고 인정받았으니까.\n히바리의 강한 점은 내가 제일 잘 알고있고,\n또 모두들 알고 있어.\n그러니까 더 자신에게 자신감을 가져."), kMyLocalKey_scenarioMent43);
	ja->setObject(CCString::create("야규.."), kMyLocalKey_scenarioMent44);
	ja->setObject(CCString::create("내.. 내가 말하고 싶은 건 그것뿐이야..\n히바리, 내일은 히바리가 좋아하는\n우사네 찻집에 가자."), kMyLocalKey_scenarioMent45);
	ja->setObject(CCString::create("응!"), kMyLocalKey_scenarioMent46);
	ja->setObject(CCString::create("이카루가.. 그리고\n야규, 아스카, 히바리도 마침 딱 있었네!"), kMyLocalKey_scenarioMent47);
	ja->setObject(CCString::create("카츠라기님. 오늘 훈련은 끝났는데\n집에 안가세요?"), kMyLocalKey_scenarioMent48);
	ja->setObject(CCString::create("어, 응..음... 뭐라고 할까\n소화불량이라고나 할까...\n어쨌든 좀 더 몸을 움직이려고... 응?"), kMyLocalKey_scenarioMent49);
	ja->setObject(CCString::create("아앗! 저건 뭐지?"), kMyLocalKey_scenarioMent50);
	ja->setObject(CCString::create("소매치기예요!! 꼬마의 물건을 훔쳐가네요!!"), kMyLocalKey_scenarioMent51);
	ja->setObject(CCString::create("아이의 즐거움을 뺏는\n배짱 좋은 녀석이군.\n저런 썩은 자식은 우리들이 혼내줄테다!!"), kMyLocalKey_scenarioMent52);
	ja->setObject(CCString::create("우와! 실력이 일취월장인데요?\n벌써 실력이 많이 늘었네요.\n이젠 다른사람과 대전을 해볼까요?\n \n따라오세요."), kMyLocalKey_scenarioMent53);
	ja->setObject(CCString::create("이건 내 정보예요.\n보상내용도 나중에 확인해보세요."), kMyLocalKey_scenarioMent54);
	ja->setObject(CCString::create("다른 친구를 누르면 해당친구의 정보를\n볼 수 있어요.\n \nPVP를 시작해보죠."), kMyLocalKey_scenarioMent55);
	ja->setObject(CCString::create("제가 당신과 붙을 만한 상대 세분을\n골라봤어요.\n \n셋중 한명을 선택해주세요."), kMyLocalKey_scenarioMent56);
	ja->setObject(CCString::create("아아아!! 죄송해요!!\n평소 대전과 달라서 설명을 드려야하는걸\n제가 까먹었네요."), kMyLocalKey_scenarioMent57);
	ja->setObject(CCString::create("폭탄이예요! 5콤보시마다 미사일을\n발사해서 상대방의 점수를 깎을 수 있어요!\n조심하세요! 상대방이 절 공격 못하게!\n영역을 획득할 때 발사되는 미사일이\n보스를 타격하게 되면 콤보가 쌓이게 되고,\n콤보점수가 추가됩니다."), kMyLocalKey_scenarioMent58);
	ja->setObject(CCString::create("행운을 빌어요."), kMyLocalKey_scenarioMent59);
	ja->setObject(CCString::create("첫 대전이라 떨리시죠?\n \n이기면 보상이 있으니 행운을 빌어요.\n그럼 전 이만.."), kMyLocalKey_scenarioMent60);
	ja->setObject(CCString::create("PVP가 끝나면 이곳으로 와요."), kMyLocalKey_scenarioMent61);
	ja->setObject(CCString::create("모두 게임오버 되었을 때는\n피격점수로 승패를 가릅니다."), kMyLocalKey_scenarioMent62);
	
	ja->setObject(CCString::create("업적 메뉴에서는 게임 중 달성한 업적을\n확인하고 보상을 받을 수 있습니다.\n앞으로 해야 할 업적들이 정말 많군요!"), kMyLocalKey_kindTutorial1);
	ja->setObject(CCString::create("모든보상받기 버튼을 누르면 달성한 업적\n보상을 모두 한번에 받을 수 있어요!"), kMyLocalKey_kindTutorial2);
	ja->setObject(CCString::create("하하! 또 만나네요.\n내카드에서는 스테이지에서 획득한 카드를\n확인할 수 있어요."), kMyLocalKey_kindTutorial3);
	ja->setObject(CCString::create("내가 획득한 카드나 다이어리 보기 버튼을\n터치하면 카드의 상세 내용을 확인할 수\n있는 다이어리로 이동합니다."), kMyLocalKey_kindTutorial4);
	ja->setObject(CCString::create("상단의 정렬버튼을 눌러 여러가지 옵션으로\n정렬기능을 이용할 수도 있습니다."), kMyLocalKey_kindTutorial5);
	ja->setObject(CCString::create("어머! 여기서 또 만나네요?\n이번엔 랭킹에 대해 설명해 드릴께요.\n \n주간랭킹은 일주일 동안 획득한 스테이지\n점수를 합산하여 랭킹이 산정됩니다."), kMyLocalKey_kindTutorial6);
	ja->setObject(CCString::create("순위마다 보상 내용이 다르니\n잘 확인해 주세요."), kMyLocalKey_kindTutorial7);
	ja->setObject(CCString::create("드디어 뽑기상자가 등장했네요."), kMyLocalKey_kindTutorial8);
	ja->setObject(CCString::create("이건 뭐죠? 아이템인가요?"), kMyLocalKey_kindTutorial9);
	ja->setObject(CCString::create("특별한 아이템을 뽑을 수 있는 상자입니다.\n운이 좋으면 목숨 하나가 추가되는\n부활아이템을 얻을 수 있죠."), kMyLocalKey_kindTutorial10);
	ja->setObject(CCString::create("껄끄러운 게임미션이로군.\n히바리. 이번 게임에선 부하몬스터를\n표시된 개수만큼 가둬 잡아야 클리어할 수 있어.\n너라면 충분히 해낼 수 있을꺼야."), kMyLocalKey_kindTutorial11);
	ja->setObject(CCString::create("응! 야규짱!"), kMyLocalKey_kindTutorial12);
	ja->setObject(CCString::create("이번엔 게임미션이 조금 다른 것 같아요."), kMyLocalKey_kindTutorial13);
	ja->setObject(CCString::create("네. 주어진 개수만큼 생성되는 아이템을\n먹어야 클리어 됩니다.\n클리어하기 더 어려워지겠는데요?"), kMyLocalKey_kindTutorial14);
	ja->setObject(CCString::create("게임시간이 엄청나게 줄어들었어요.\n조금 서둘러야겠는걸요."), kMyLocalKey_kindTutorial15);
	ja->setObject(CCString::create("알고있어!! 게임 중에 생기는 시간아이템을\n적절히 먹으면서 플레이하면 문제없을꺼야!!"), kMyLocalKey_kindTutorial16);
	ja->setObject(CCString::create("앗! 이건 뭐죠?"), kMyLocalKey_kindTutorial17);
	ja->setObject(CCString::create("이 신발을 착용하면 게임을\n더 빠른 스피드로 시작할 수 있어요.\n무료로 몇 개 지급되니 사용해보세요."), kMyLocalKey_kindTutorial18);
	ja->setObject(CCString::create("새로운 아이템이 등장했네요."), kMyLocalKey_kindTutorial19);
	ja->setObject(CCString::create("게임 중에 아이템이 두 배로 더 많이\n나오도록 해주는 인법서군요.\n많은 도움이 될겁니다."), kMyLocalKey_kindTutorial20);
	ja->setObject(CCString::create("이건 자석인가?"), kMyLocalKey_kindTutorial21);
	ja->setObject(CCString::create("네. 게임 중에 생기는 골드나 아이템을\n더 쉽게 먹을 수 있도록 도와주는\n아이템입니다."), kMyLocalKey_kindTutorial22);
	
	ja->setObject(CCString::create("MAX"), kMyLocalKey_maxSpeed);
	ja->setObject(CCString::create("SHOW TIME"), kMyLocalKey_showtime);
	ja->setObject(CCString::create("MISS"), kMyLocalKey_miss);
	ja->setObject(CCString::create("COMBO"), kMyLocalKey_combo);
	ja->setObject(CCString::create("Scrolling is possible!"), kMyLocalKey_unlockedAreaScroll);
	ja->setObject(CCString::create("MISSION COMPLETE"), kMyLocalKey_missionComplete);
	ja->setObject(CCString::create("COLLECT CHANGE!!"), kMyLocalKey_collectChange);
	ja->setObject(CCString::create("정식오픈 후 사용할 수 있습니다."), kMyLocalKey_afterOpenCBT);
	ja->setObject(CCString::create("CARD CHANGE!"), kMyLocalKey_cardChange);
	ja->setObject(CCString::create("100% 쇼타임에서는 생명의 돌을 체험할 수 있어요."), kMyLocalKey_showtimeMorphingTouch);
	ja->setObject(CCString::create("카드강화"), kMyLocalKey_cardStrength);
	ja->setObject(CCString::create("업데이트 예정"), kMyLocalKey_updateTitle);
	ja->setObject(CCString::create("추후 업데이트 됩니다."), kMyLocalKey_updateContent);
	ja->setObject(CCString::create("준비중"), kMyLocalKey_communityNotOpenTitle);
	ja->setObject(CCString::create("준비입니다. 공식카페를 이용해 주세요."), kMyLocalKey_communityNotOpenContent);
	
	ja->setObject(CCString::create("조이스틱\n크기"), kMyLocalKey_joystickSize);
	ja->setObject(CCString::create("대각선\n사용"), kMyLocalKey_useSideDirection);
	ja->setObject(CCString::create("19+ 다이어리"), kMyLocalKey_toDiary19);
	
	ja->setObject(CCString::create("19+ 다이어리"), kMyLocalKey_toDiary19Title);
	ja->setObject(CCString::create("<font size=12 newline=15>보조앱(19+ 다이어리)이 없네요.</font><font size=12 newline=15>더욱 화끈하고! 쎈! 그녀들이 기다립니다.</font><font size=12>해당앱으로 연결하시겠습니까?</font>"), kMyLocalKey_toDiary19Content);
	ja->setObject(CCString::create("19+ 다이어리 다운 받기"), kMyLocalKey_toDiary19Button);
	ja->setObject(CCString::create("조이스틱\n진동"), kMyLocalKey_joystickVib);
	
	ja->setObject(CCString::create("조이스틱 크기조절"), kMyLocalKey_joystickSizeQuestionTitle);
	ja->setObject(CCString::create("<font newline=17>게임에서 사용되는 조이스틱의 크기를</font><font newline=17>9단계의 크기로 조절하고 있습니다.</font><font color=961>평균 5단계를 사용합니다.</font>"), kMyLocalKey_joystickSizeQuestionContent);

	setObject(ja, "ja");
}

void ReplaceString(std::string & strCallId, const char * pszBefore, const char * pszAfter )
{
	size_t iPos = strCallId.find( pszBefore );
	size_t iBeforeLen = strlen( pszBefore );
	while( iPos < std::string::npos )
	{
		strCallId.replace( iPos, iBeforeLen, pszAfter );
		iPos = strCallId.find( pszBefore, iPos );
	}
}

string MyLocal::printList(){
	CCDictionary* loDic = (CCDictionary*)(myLoc->objectForKey("ko"));
	CCArray *keys = loDic->allKeys();
	
	printf("################## Localpack ####################\n");
	int i=0;
	string r = "";
	while(1){
		int nextCnt = i;
		CCString* ob = (CCString *)(loDic->objectForKey(nextCnt));
		
		string a ="";
		
		if(ob)a=ob->getCString();
		
		ReplaceString(a,"\n","");
		//CCLOG("%d",i);
		
		std::ostringstream ostr;
		ostr << i;
		
		r += ostr.str();
		r += "\t"+a+"\n";
		
		
		i++;
		if(i>MyLocalKey::kMyLocalKey_lastkey)break;
	}
	
	CCLOG("\n %s",r.c_str());

	printf("################## Localpack ####################\n");

	//	string a = myLoc->printList();
	//	Json::Value param;
	//
	//	param["memberID"] = hspConnector::get()->getSocialID();
	//	param["content"] = a;
	//	param["category"] = "localPack";
	//
	//	hspConnector::get()->command("writelog", param, nullptr);
	//	

	
	return r;
}
