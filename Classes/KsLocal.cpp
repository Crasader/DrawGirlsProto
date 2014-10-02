#include "KSLocal.h"


void KsLocal::initLocal()
{	
	ko();
	en();
	ja();
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
std::string KsLocal::getLocalCode()
{
	string tempCode;
	tempCode = getCurrentLanguageJNI();
	return tempCode;
}
#endif
const char* KsLocal::getSupportLocalCode()
{
	languageType = getLocalCode();
	bool supportable = false;
	auto foundIter = m_mapper.find(languageType);
	if(foundIter != m_mapper.end())
	{
		supportable = true;
	}
	else
	{
		supportable = false;
	}
	
	if(!supportable)
	{
		languageType = "en";
	}
	
	return languageType.c_str();
}

const char* KsLocal::getLocalForKey( LK key )
{
	languageType = getSupportLocalCode();
	
	vector<string> langTypes;
	langTypes.push_back(languageType);
	vector<string> entries = {"en", "ja", "ko"};
	langTypes.insert(langTypes.end(), entries.begin(), entries.end());
	for(auto& lang : langTypes)
	{
		auto findIter = m_mapper.find(lang);
		if(findIter == m_mapper.end())
		{
//			return CCString::create("")->getCString();
		}
		else
		{
			if(m_mapper[lang].find(key) != m_mapper[lang].end())
			{
				return m_mapper[lang][key].c_str();
			}
		}
		
	}
	return CCString::create("")->getCString();
}

KsLocal* KsLocal::sharedInstance()
{
	static KsLocal* ksLocal = NULL;
	if(ksLocal == NULL)
	{
		ksLocal = new KsLocal();
	}
	return ksLocal;
}

void KsLocal::setLocal()
{
	languageType = getLocalCode();
}
void KsLocal::ko()
{
	std::map<LK, std::string> obj;
	obj[LK::kItem1] = "게임을 시작 중 입니다...";
	obj[LK::kReGacha] = "다시 뽑기";
	obj[LK::kRewardConfirm] = "확인";
	obj[LK::kRewardTitle] = "게임 결과";
	obj[LK::kBonusGame] = "보너스 게임";
	obj[LK::kBonusGameStart] = "게임 시작";
	obj[LK::kBonusGameDesc] = "게임 설명";
	obj[LK::kBonusGameGababo] = "가위! 바위! 보!";
	obj[LK::kGababoDesc1] = "<font color=#FFFFFF size=11 newline=20>캐릭터와 눈치를 보며</font>"
												 "<font color=#FFFFFF size=11>가위바위보를 해보세요!</font>";
	obj[LK::kBonusGameTouch] = "터치! 터치!";
	obj[LK::kTouchDesc1] = "<font color=#FFFFFF size=11 newline=20>최대한 많이 터치를 하세요.</font>"
																														 "<font color=#FFFFFF size=11>돈이 쏟아져요!</font>";
	
	obj[LK::kBonusCong] = "<font color=#FFFFFF newline=15>축하합니다.</font>"
																										 "<font color=#FFFFFF newline=15>생명의 돌은 다이어리에서 사용할수있어요.</font>"
																										 "<font color=#FFFFFF>그럼 퍼펙트 클리어 후 다시 만나요!</font>";
	
	
	
	obj[LK::kAccountConf] = "계정 설정";
	obj[LK::kLinkingGoogle] = "구글과 연결되어 있습니다.";
	obj[LK::kLinkingFacebook] = "페이스북과 연결되어 있습니다.";
	obj[LK::kLinkingX] = "연결이 안되어 있습니다.";
	obj[LK::kLinkingUnknown] = "알 수 없습니다.";
	obj[LK::kLinkingAccount] = "연결 계정";
	obj[LK::kMemberNo] = "회원번호";
	obj[LK::kSaveDesc1] = "게임을 저장하시려면\n Google+ ID를\n연결해 주세요.";
	obj[LK::kGoogleButton] = "GOOGLE+\nID 연결";
	obj[LK::kSaveDesc2] = "여러기기에서 게임을\n 즐기시려면 facebook ID를 \n 연결해 주세요.";
	obj[LK::kFacebookButton] = "FACEBOOK\n ID 연결";
	obj[LK::kWarningDesc] = "유의사항";
	obj[LK::kAnotherHistory] = "<font color=#FFFFFF newline=12>%s 에 연결된</font>"
																											 "<font color=#FFFFFF newline=12>다른 게임 기록이 있습니다.</font>"
																											 "<font color=#FFFFFF newline=24>(%s, %d Stage)</font>"
																											 "<font color=#FFAA14 newline=24>이 게임 기록을 불러오시겠습니까?</font>"
																											 "<font color=#FFFFFF newline=12>게스트사용자인 경우 이전 게임 기록을 불러오면</font>"
																											"<font color=#FFFFFF newline=12>현재 기록(%s, %dStage)은 삭제됩니다.</font>";

	obj[LK::kLoadPreviousData] = "이전 기록 불러오기";
	obj[LK::kSaveCurrentData] = "현재 기록을 저장";
																											 	obj[LK::kNetworkError] = "<font color=#FFFFFF newline=12>통신 에러</font>"
																																								"<font color=#FFAA14>다시 시도해 주세요.</font>";
	obj[LK::kCantLinking] = "<font color=#FFFFFF newline=14>연결 할 수 없습니다.</font>"
																																									"<font color=#FFAA14>다시 시도해 주세요.</font>";
	obj[LK::kRetryLinking] = "";
	obj[LK::kAccountDesc] = "<font color=#FFAA14 newline=12>%s 와 연결되어 있습니다.</font>"
																		"<font color=#FFFFFF newline=12>%s 로 연결하시면</font>"
																		"<font color=#FFAA14 newline=24>이어서 게임을 하실 수 있으나</font>"
																		"<font color=#FFFFFF newline=12>%s 로는 이어서 게임을</font>"
																		"<font color=#FFFFFF newline=24>하실 수 없습니다.</font>"
																		"<font color=#FFAA14 newline=12>연결 하시겠습니까?</font>";
	obj[LK::kLinkingCommand] = "연결하기";
	obj[LK::kLinked] = "연결됨";
	obj[LK::kForeverDeleteAccount1] = "<font color=#FFFFFF newline=14>현재 기록은 영구히 삭제되며</font>"
	"<font color=#FFFFFF>%s 의 이전 기록을 로드합니다.</font>";
	obj[LK::kForeverDeleteAccount2] = "<font newline=14>%s 의 이전 기록은 영구히</font>"
	"<font >삭제되며 현재기록을 유지합니다.</font>";
	obj[LK::kUnlinkAccount1] = "<font color=#FF00000 newline=14>%s 와의 연동을 해제합니다.</font>";
	obj[LK::kLinkAccount1] = "<font>%s 와 연동합니다.</font>";
	
	obj[LK::kFriendList] = "친구";
	obj[LK::kFriendListTitle] = "친구목록";
	obj[LK::kFriendListMenu] = "친구목록";
	obj[LK::kFriendAddMenu] = "친구추가";
	obj[LK::kFriendManageMenu] = "친구관리";
	obj[LK::kFriendLimitDesc] = "친구는 50명 까지 등록 가능합니다.";
	obj[LK::kFriendMyCount] = "내 친구수 %d명";
	obj[LK::kFriendLastConnection] = "최종접속 %s";
	obj[LK::kFriendCurrentStage] = "%d스테이지";
	obj[LK::kFriendHeartSend] = "   보내기";
	obj[LK::kFriendHeartReSend] = "다시 보낼 수 있어요.";
	
	obj[LK::kFriendInputNickName] = "닉네임을 입력해주세요.";
	obj[LK::kFriendAddFriend] = " + 친구요청";
	obj[LK::kFriendAddingFriend] = "친구요청중";
	obj[LK::kFriendSearch] = "검색";
	obj[LK::kFriendMailAccept] = "수락";
	obj[LK::kFriendMailReject] = "거절";
	obj[LK::kFriendRemoveFriend] = "- 친구삭제";
	obj[LK::kFriendConfirm] = "확인";
	obj[LK::kFriendRemoveFriendQ] = "<font newline=13>%s 님과</font><font>친구를 끊으시겠습니까?</font>";
	obj[LK::kFriendAddLimitMe] = "더 이상 추가를 할 수 없습니다.";
	obj[LK::kFriendAddLimitYou] = "상대방의 친구가 너무 많습니다.";
	obj[LK::kFriendNotFountFriend] = "찾을 수 없습니다.";
	obj[LK::kFriendVote1] = "이 친구를 추천합니다!!";
	obj[LK::kFriendVote2] = "<font color=#FFFFFF newline=14>이 게임을 추천해준 친구의 닉네임을 입력하세요.</font>"
													"<font color=#FFAA14>추천인 입력시 보상으로 %d젬을 드립니다.</font>";
	obj[LK::kFriendInputSkip] = "SKIP";
	obj[LK::kFriendVoterInput] = "추천인 입력";
	obj[LK::kFriendKakaoTalkFriend] = "카톡친구초대";
	obj[LK::kFriendDontFind] = "회원을 찾을 수 없습니다.";
	obj[LK::kFriendNoti] = "알림";
	obj[LK::kFriendError] = "오류가 발생했습니다.";
	obj[LK::kFriendSuccessfullyRecommend] = "성공적으로 추천하셨습니다.";
	obj[LK::kFriendVotePromotion1] = "<font color=#FFFFFF>친구 추천 현재 </font><font color=#FFAA14>%d명</font>";
	obj[LK::kFriendVotePromotion2] = "<font color=#FFFFFF newline=13>이 게임을 추천하여 추천인으로 입력받으세요.</font>"
																		"<font color=#FFFFFF>친구가 %d스테이지 이상 진입하면 보상을 드려요.</font>";
	obj[LK::kFriendRecommend] = "친구추천";
	obj[LK::kFriendGiftDesc] = "친구끼리는 카드를 선물할 수 있습니다.";
	obj[LK::kFriendGiftSend] = "보내기";
	obj[LK::kFriendGiftTitle] = "카드선물하기";
	obj[LK::kFriendGiftQ] = "%s 님께 카드선물을 하시겠습니까?";
	obj[LK::kFriendGiftYes] = "예";
	obj[LK::kFriendNothingExist] = "등록된 친구가 없습니다.";
	obj[LK::kFriendCardGiftErrorMessage] = "<font color=#FFFFFF newline=13>카드선물은 %d 시간에 한번 가능합니다.</font>"
																					"<font color=#FFFFFF>앞으로 %d시간 %d분 남았습니다.</font>";
	
	obj[LK::kFriendNickInputPlz] = "닉네임을 입력해주세요.";
	
	
	obj[LK::kMyLocalKey_connectingServer] = "서버에 접속 중입니다...";
	obj[LK::kMyLocalKey_inputPlease] = " 닉네임을 입력해주세요.";
	obj[LK::kMyLocalKey_ok] = "확인";
	obj[LK::kMyLocalKey_downImgInfo] = "이미지 정보를 받아옵니다.";
	obj[LK::kMyLocalKey_failLoadInfo] = "정보 불러오기 실패";
	obj[LK::kMyLocalKey_replay] = "재시도";
	obj[LK::kMyLocalKey_downImgFail] = "이미지 받아오기 실패";
	obj[LK::kMyLocalKey_sameNick] = "사용할 수 없는 닉네임 입니다.";
	obj[LK::kMyLocalKey_invalidNick] = "사용할 수 없는 닉네임 입니다.";
	obj[LK::kMyLocalKey_longNick] = "닉네임이 너무 깁니다.";
	obj[LK::kMyLocalKey_shortNick] = "닉네임이 너무 짧습니다.";
	obj[LK::kMyLocalKey_openRightNow] = "지금 열기";
	obj[LK::kMyLocalKey_openRubyValue] = "RUBY %d 로 오픈";
	obj[LK::kMyLocalKey_cgpNormalTitle] = "CGP 일반 보상";
	obj[LK::kMyLocalKey_cgpNormalContent] = "보상받기를 눌러주세요.";
	obj[LK::kMyLocalKey_getReward] = "보상받기";
	obj[LK::kMyLocalKey_take] = "받기";
	obj[LK::kMyLocalKey_cgpAllPopupTitle] = "CGP 전체 팝업 보상";
	obj[LK::kMyLocalKey_shop] = "상점";
	obj[LK::kMyLocalKey_ranking] = "랭킹";
	obj[LK::kMyLocalKey_mycard] = "내카드";
	obj[LK::kMyLocalKey_todaymission] = "오늘의 임무";
	obj[LK::kMyLocalKey_event] = "이벤트";
	obj[LK::kMyLocalKey_openStage] = "스페셜 스테이지";
	obj[LK::kMyLocalKey_openStageContent] = "스페셜 스테이지를 오픈하시겠습니까?";
	obj[LK::kMyLocalKey_doOpen] = "오픈하기";
	obj[LK::kMyLocalKey_view] = "보기";
	obj[LK::kMyLocalKey_condition1] = "영역 85%";
	obj[LK::kMyLocalKey_condition2] = "영역 100%";
	obj[LK::kMyLocalKey_condition3] = "체인지 + 영역 85%";
	obj[LK::kMyLocalKey_condition4] = "체인지 + 영역 100%";
	obj[LK::kMyLocalKey_stageValue] = "%d 스테이지";
	obj[LK::kMyLocalKey_failCheckRanking] = "랭킹 정보 확인 실패";
	obj[LK::kMyLocalKey_stage] = "스테이지";
	obj[LK::kMyLocalKey_selectUseItem] = "게임에 사용할 아이템을 선택하세요.";
	obj[LK::kMyLocalKey_levelValue] = "레벨 %d";
	obj[LK::kMyLocalKey_powerValue] = "파워 %s";
	obj[LK::kMyLocalKey_upgradeLevelValue] = "레벨 %d 업그레이드";
	obj[LK::kMyLocalKey_upgradeLevelValue2] = "레벨 %d\n업그레이드";
	obj[LK::kMyLocalKey_totalItemSelectCount3] = "아이템은 최대 3개까지\n선택이 가능합니다.";
	obj[LK::kMyLocalKey_goldNotEnought] = "골드가 부족합니다.";
	obj[LK::kMyLocalKey_rubyNotEnought] = "젬이 부족합니다.";
	obj[LK::kMyLocalKey_heartNotEnought] = "하트가 부족합니다.";
	obj[LK::kMyLocalKey_baseSpeedUpTitle] = "기본속도향상";
	obj[LK::kMyLocalKey_baseSpeedUpContent] = "유저 속도가 빠른상태로 시작합니다.";
	obj[LK::kMyLocalKey_doubleItemTitle] = "더블아이템";
	obj[LK::kMyLocalKey_doubleItemContent] = "아이템을 빠르게 생성합니다.";
	obj[LK::kMyLocalKey_longTimeTitle] = "시간추가";
	obj[LK::kMyLocalKey_longTimeContent] = "게임시간을 증가시킵니다.";
	obj[LK::kMyLocalKey_magnetTitle] = "자석";
	obj[LK::kMyLocalKey_magnetContent] = "골드와 아이템을 빨아들입니다.";
	obj[LK::kMyLocalKey_itemGachaTitle] = "아이템 뽑기";
	obj[LK::kMyLocalKey_itemGachaDefaultContent1] = "뽑기에만 등장하는 아이템들!";
	obj[LK::kMyLocalKey_itemGachaDefaultContent2] = "침묵, 대쉬, 목숨하나더, 부하몹제거";
	obj[LK::kMyLocalKey_itemGachaContent] = "부활 아이템에 도전하세요!";
	obj[LK::kMyLocalKey_rubyShop] = "젬상점";
	obj[LK::kMyLocalKey_goldShop] = "골드상점";
	"하트상점";obj[LK::kMyLocalKey_heartShop] = 
	"구매하시겠습니까?";obj[LK::kMyLocalKey_realBuy] = 
	"설정";obj[LK::kMyLocalKey_option] = 
	"쿠폰등록";obj[LK::kMyLocalKey_regCoupon] = 
	"커뮤니티";obj[LK::kMyLocalKey_community] = 
	"시스템";obj[LK::kMyLocalKey_system] = 
	"배경음";obj[LK::kMyLocalKey_bgm] = 
	"효과음";obj[LK::kMyLocalKey_effect] = 
	"조이스틱\n위치";obj[LK::kMyLocalKey_joystickPosition] = 
	"조이스틱\n조작";obj[LK::kMyLocalKey_joystickControl] = 
	"대중교통\n모드";obj[LK::kMyLocalKey_safetyMode] = 
	"알림";obj[LK::kMyLocalKey_noti] = 
	"푸쉬알림";obj[LK::kMyLocalKey_pushNoti] = 
	"메세지\n알림";obj[LK::kMyLocalKey_messageNoti] = 
	"서버연결에 실패하였습니다.";obj[LK::kMyLocalKey_failedConnect] = 
	"메세지확인";obj[LK::kMyLocalKey_confirmGift] = 
	"받은 메세지가 없습니다.";obj[LK::kMyLocalKey_nogift] = 
	"*받은 메세지는 30일 이후에 삭제됩니다.";obj[LK::kMyLocalKey_giftboxAlert] = 
	"재접속";obj[LK::kMyLocalKey_reConnect] = 
	"다른 기기로 연결되었습니다.\n다시 로그인합니다.";obj[LK::kMyLocalKey_reConnectAlert1] = 
	"서버와의 접속에 오류가 발생하였습니다.\n다시 로그인합니다.";obj[LK::kMyLocalKey_reConnectAlert2] = 
	"세션이 종료되었습니다.\n다시 로그인합니다.";obj[LK::kMyLocalKey_reConnectAlert3] = 
	"서버연결에 실패하였습니다.\n다시 시도합니다.";obj[LK::kMyLocalKey_reConnectAlert4] = 
	"BLOCKED";obj[LK::kMyLocalKey_blocked] = 
	"계정이 아래의 사유로 정지 되었습니다.";obj[LK::kMyLocalKey_blockedMsg] = 
	"회원ID : ";obj[LK::kMyLocalKey_memberID] = 
	"게임버젼 : ";obj[LK::kMyLocalKey_gameversion] = 
	"게임탈퇴";obj[LK::kMyLocalKey_withdraw] = 
	"고객센터";obj[LK::kMyLocalKey_callCenter] = 
	"로그아웃 하시겠습니까?";obj[LK::kMyLocalKey_checkLogout] = 
	"취소";obj[LK::kMyLocalKey_cancel] = 
	"게임 탈퇴";obj[LK::kMyLocalKey_withdrawTitle] = 
	"탈퇴시 게임을 이용하실 수 없으며";obj[LK::kMyLocalKey_withdrawContent1] = 
	"모든 데이터가 영구적으로 삭제됩니다.";obj[LK::kMyLocalKey_withdrawContent2] = 
	"정말 탈퇴하시겠습니까?";obj[LK::kMyLocalKey_withdrawContent3] = 
	"탈퇴하기";obj[LK::kMyLocalKey_doWithdraw] = 
	"꺼짐";obj[LK::kMyLocalKey_lightOff] = 
	"켜짐";obj[LK::kMyLocalKey_lightOn] = 
	"오른쪽";obj[LK::kMyLocalKey_right] = 
	"왼쪽";obj[LK::kMyLocalKey_left] = 
	"고정";obj[LK::kMyLocalKey_fix] = 
	"이동";obj[LK::kMyLocalKey_move] = 
	"주간랭킹";obj[LK::kMyLocalKey_weeklyranking] = 
	"%d일후 리셋";obj[LK::kMyLocalKey_dayAfterReset] = 
	"%d시간후 리셋";obj[LK::kMyLocalKey_hourAfterReset] = 
	"%d분후 리셋";obj[LK::kMyLocalKey_secondAfterReset] = 
	"우편함";obj[LK::kMyLocalKey_giftbox] = 
	"<font color=#FFAA14 size=15>%s</font><br><font color=#CCCCCC size=10s>%s</font>";obj[LK::kMyLocalKey_giftboxContent] = 
	"모든선물 받기";obj[LK::kMyLocalKey_allAccept] = 
	"티켓함";obj[LK::kMyLocalKey_ticketBox] = 
	"도움함";obj[LK::kMyLocalKey_helpBox] = 
	"도전함";obj[LK::kMyLocalKey_challengeBox] = 
	"하트함";obj[LK::kMyLocalKey_heartBox] = 
	"전체보기";obj[LK::kMyLocalKey_allView] = 
	"기본정렬";obj[LK::kMyLocalKey_defaultSort] = 
	"획득순";obj[LK::kMyLocalKey_takeOrder] = 
	"등급순";obj[LK::kMyLocalKey_gradeOrder] = 
	"레벨을 올리면 보스를 좀 더 쉽게 물리칠 수 있어요!";obj[LK::kMyLocalKey_upgradeSubMent] = 
	"버리고 다시 뽑기";obj[LK::kMyLocalKey_itemRegacha] = 
	"%.0f%% 할인";obj[LK::kMyLocalKey_itemRegachaDiscountValue] = 
	"이 아이템 사용";obj[LK::kMyLocalKey_thisItemUse] = 
	"총점";obj[LK::kMyLocalKey_totalScore] = 
	"다음스테이지";obj[LK::kMyLocalKey_nextStage] = 
	"다시하기";obj[LK::kMyLocalKey_regame] = 
	"메인으로";obj[LK::kMyLocalKey_toMain] = 
	"이전 스테이지를\n클리어 해주세요!";obj[LK::kMyLocalKey_beforeNotClearPuzzle] = 
	"이전액자를\n모두클리어하면\n다음액자가나타납니다.";obj[LK::kMyLocalKey_waitForUpdate] = 
	"보상";obj[LK::kMyLocalKey_reward] = 
	"스피드";obj[LK::kMyLocalKey_speed] = 
	"준비하기";obj[LK::kMyLocalKey_ready] = 
	"상대찾기";obj[LK::kMyLocalKey_endlessReady] = 
	"꾸욱";obj[LK::kMyLocalKey_controlClick] = 
	"잠김";obj[LK::kMyLocalKey_locked] = 
	"";obj[LK::kMyLocalKey_titleLoadingBegin] = 
	"각 스테이지 별로 각기 다른 4장의 이미지를 획득해 보세요.";obj[LK::kMyLocalKey_titleLoading1] = 
	"공공장소나 대중교통 이용 시에는 '대중교통 모드'로 Play 하세요. 획득한 이미지는 내 카드 메뉴에서 확인 할 수 있습니다.";obj[LK::kMyLocalKey_titleLoading2] = 
	"파란색 실루엣 영역을 획득해야 % 가 증가해요.";obj[LK::kMyLocalKey_titleLoading3] = 
	"'영역 다시 뽑기' 를 해서 넓은 영역으로 play 를 시작하면 높은 등급의 카드를 획득하기가 쉬워져요";obj[LK::kMyLocalKey_titleLoading4] = 
	"'대중교통 모드' 는 play 중에 일시정지 버튼을 탭해서 설정할 수 있습니다.";obj[LK::kMyLocalKey_titleLoading5] = 
	"Clear 등급에 따라 획득되는 별의 개수가 다릅니다. 별이 모이면 다음 액자를 열 수 있어요.";obj[LK::kMyLocalKey_titleLoading7] = 
	"'업적' 창에서 달성한 업적을 보상받으세요. 달성한 정도에 따라 젬을 드립니다.";obj[LK::kMyLocalKey_titleLoading8] = 
	"일일임무에 도전해 보세요. 일일 임무을 달성하시면 다양한 선물을 드립니다.";obj[LK::kMyLocalKey_titleLoading9] = 
	"주간랭킹에 도전해 보세요. 매주 새로운 선물을 푸짐하게 드립니다.";obj[LK::kMyLocalKey_titleLoading10] = 
	"change 글자를 모아서 특별한 이미지를 확인하세요.";obj[LK::kMyLocalKey_titleLoading11] = 
	"미사일을 업그레이드 하면 강력한 공격력으로 높은 등급 카드 획득이 가능합니다.";obj[LK::kMyLocalKey_titleLoading12] = 
	"아이템뽑기에서는 '부활', '침묵', '시간추가' 등 레어 아이템을 뽑을 수 있어요.";obj[LK::kMyLocalKey_titleLoading13] = 
	"스테이지마다 showtime 에 도전해 보세요.";obj[LK::kMyLocalKey_titleLoading14] = 
	"매주 새로운 이벤트가 열립니다. 공식 카페를 확인해 주세요.";obj[LK::kMyLocalKey_titleLoading15] = 
	"게임 내에서는 이름 대신 닉네임을 사용합니다.";obj[LK::kMyLocalKey_titleLoading16] = 
	"99% 라 너무 아쉽다면, 보너스 뽑기에 도전 해 보세요.";obj[LK::kMyLocalKey_titleLoading17] = 
	"보너스팩을 플레이하면 특별한 이미지와 선물을 받을 수 있습니다.";obj[LK::kMyLocalKey_titleLoading18] = 
	"스테이지별 임무 clear 가 어렵다면 아이템 뽑기를 통해 임무 전용 아이템을 사용해 보세요.";obj[LK::kMyLocalKey_titleLoading19] = 
	"한 번에 많은 영역을 획득할 수록 여러 개의 미사일이 발사되며, 공격을 성공한 만큼 콤보가 적용됩니다.";obj[LK::kMyLocalKey_titleLoading6] = 
	"이어하기는 횟수 제한 없이 사용할 수 있습니다. 목표한 등급을 꼭 획득하세요!";obj[LK::kMyLocalKey_titleLoading20] = 
	"내카드 목록에서 카드 이미지를 터치하면 카드 상세 내용을 보실 수 있습니다.";obj[LK::kMyLocalKey_titleLoading21] = 
	"다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요.";obj[LK::kMyLocalKey_titleLoading22] = 
	"85% 로 클리어 하면 별 1개를 획득합니다.";obj[LK::kMyLocalKey_titleLoading23] = 
	"change를 모두 모은 후 클리어하면 별 3개를 획득합니다.";obj[LK::kMyLocalKey_titleLoading24] = 
	"100%로 클리어 하면 별 2개를 획득합니다.";obj[LK::kMyLocalKey_titleLoading25] = 
	"change를 모두 모은 후 100%로 클리어하면 별 4개를 모을 수 있습니다.";obj[LK::kMyLocalKey_titleLoading26] = 
	"다이어리에는 캐릭터마다 재미있는 특징들이 숨어있어요.";obj[LK::kMyLocalKey_titleLoading27] = 
	"플레이 중 획득한 영역에 따라 골드를 드립니다.";obj[LK::kMyLocalKey_titleLoading28] = 
	"30%나 할인되는 종합 아이템 구매 찬스를 놓치지 마세요!";obj[LK::kMyLocalKey_titleLoading29] = 
	"임무스테이지에서는 임무을 먼저 완료한 후 클리어가 가능합니다.";obj[LK::kMyLocalKey_titleLoading30] = 
	"미사일 공격 시 그리는 선에 미사일이 닿으면 유도 공격을 당해요.";obj[LK::kMyLocalKey_titleLoading31] = 
	"게임 중 시간추가 아이템을 먹으면 남은 시간에서 10초가 추가됩니다.";obj[LK::kMyLocalKey_titleLoading32] = 
	"선을 그리다가 꾸욱 버튼에서 손을 떼면 캐릭터가 안전 영역으로 되돌아 갑니다.";obj[LK::kMyLocalKey_titleLoading33] = 
	"보스에게 연속 공격을 성공시키면 콤보가 적용되며, 게임 결과에서 콤보 보너스를 받을 수 있습니다.";obj[LK::kMyLocalKey_titleLoading34] = 
	"내가 모은 별의 개수는 메인화면의 왼쪽 상단에 있는 별 아이콘을 통해 확인하실 수 있습니다.";obj[LK::kMyLocalKey_titleLoading35] = 
	"운석이 떨어지면 캐릭터에 맞지 않도록 주의하세요.";obj[LK::kMyLocalKey_titleLoading36] = 
	"조이스틱과 꾸욱 버튼의 위치를 바꾸고 싶다면 설정메뉴에서 조이스틱위치를 반대로 변경해 보세요.";obj[LK::kMyLocalKey_titleLoading37] = 
	"조이스틱의 위치를 고정하고 싶다면 설정메뉴에서 조이스틱조작을 고정으로 변경해 보세요.";obj[LK::kMyLocalKey_titleLoading38] = 
	"꾸욱 버튼을 누른채 한쪽 방향으로 그린 다음 되돌아 오면 한줄 그리기가 가능합니다.";obj[LK::kMyLocalKey_titleLoading39] = 
	"그리는 도중 줄이 꼬여도 당황하지 마세요. 줄이 꼬인 상태에서도 영역을 획득할 수 있습니다.";obj[LK::kMyLocalKey_titleLoading40] = 
	"작은 몬스터에는 HP가 존재하며, 공격이 성공할 때마다 HP가 줄어듭니다.";obj[LK::kMyLocalKey_titleLoading41] = 
	"에피소드 하나를 완성할 때마다 특별한 카드를 선물로 드립니다.";obj[LK::kMyLocalKey_titleLoading42] = 
	"클리어 점수는 영역획득점수와 콤보점수, 남은 타임 보너스가 합산된 점수입니다.";obj[LK::kMyLocalKey_titleLoading43] = 
	"고득점을 노린다면 연속 콤보를 성공시켜 보세요.";obj[LK::kMyLocalKey_titleLoading44] = 
    "친구를 초대하고 추천인 보상을 받으세요. 추천을 받은 횟수에 따라 더 많은 보상을 받을 수 있습니다.";obj[LK::kMyLocalKey_titleLoading45] = 
    "친구에게 카드를 선물해 보세요. 카드 선물은 내카드>다이어리에서 보낼 수 있습니다.";obj[LK::kMyLocalKey_titleLoading46] = 
    "PvP모드에서는 달성한 연승에 따라 다양한 보상을 받을 수 있습니다.";obj[LK::kMyLocalKey_titleLoading47] = 
    "6스테이지까지 완료하면 PvP모드를 플레이 할 수 있습니다.";obj[LK::kMyLocalKey_titleLoading48] = 
    "설정 메뉴에서 조작 옵션을 나에게 맞게 설정할 수 있습니다.";obj[LK::kMyLocalKey_titleLoading49] = 
	"성인 이라면 S다이어리 앱이 필수! 성인버전의 카드를 볼 수 있습니다.";obj[LK::kMyLocalKey_titleLoading50] = 
	"";obj[LK::kMyLocalKey_titleLoadingEnd] = 
	"게임시작";obj[LK::kMyLocalKey_gamestart] = 
	"대전 시작";obj[LK::kMyLocalKey_battleStart] = 
	"내 미사일";obj[LK::kMyLocalKey_mymissile] = 
	"파워";obj[LK::kMyLocalKey_power] = 
	"뽑기";obj[LK::kMyLocalKey_gacha] = 
	"일일임무을 완료해보세요!\n우편함으로 선물을 드려요! ^^";obj[LK::kMyLocalKey_stageListDown] = 
	"액자오픈조건";obj[LK::kMyLocalKey_frameOpenConditionTitle] = 
	"이전액자클리어\n+별%d개이상";obj[LK::kMyLocalKey_frameOpenConditionContent] = 
	"이전액자클리어\n+별%d개이상\n+%s골드";obj[LK::kMyLocalKey_frameOpenConditionContentRuby] = 
	"%s\n%d:00-%d:00";obj[LK::kMyLocalKey_frameOpenConditionContentTimeWeek] = 
	"%s월%s일%s:%s오픈";obj[LK::kMyLocalKey_frameOpenConditionContentTimeDate] = 
	"자세히보기";obj[LK::kMyLocalKey_detailView] = 
	"별 획득 방법";obj[LK::kMyLocalKey_detailConditionPopupTitle] = 
	"<font newline=14 strokecolor=000>스테이지를 클리어 할 때마다<font newline=30>클리어 단계에 따라 별을 획득합니다.<font color=961>한 스테이지<font newline=14>에서 획득할 수 있는<font color=961>별 갯수는 총 10개<font newline=14>입니다.";obj[LK::kMyLocalKey_detailConditionPopupContent] = 
	"결제 실패";obj[LK::kMyLocalKey_failPurchase] = 
	"구매 완료";obj[LK::kMyLocalKey_successPurchase] = 
	"지금 상점으로 이동하시겠습니까?";obj[LK::kMyLocalKey_enoughtGoodsContent] = 
	"상점 바로가기";obj[LK::kMyLocalKey_goShopButton] = 
	"액자를 플레이할 수 있는 시간이 지났습니다.\n홈으로 돌아갑니다.";obj[LK::kMyLocalKey_timeOutFrame] = 
	"열기";obj[LK::kMyLocalKey_directEnter] = 
	"카드 승급 기회!!";obj[LK::kMyLocalKey_rankUpTitle] = 
	"승급 성공시 한 단계 높은 카드를 획득할 수 있어요!";obj[LK::kMyLocalKey_rankUpSubTitle] = 
	"현재 획득카드";obj[LK::kMyLocalKey_recentTakeCard] = 
	"승급확률";obj[LK::kMyLocalKey_rankUpRate] = 
	"승급하기";obj[LK::kMyLocalKey_rankup] = 
	"구매하기";obj[LK::kMyLocalKey_purchase] = 
	"첫구매시 할인";obj[LK::kMyLocalKey_firstPurchaseSale] = 
	"%s할인은 첫 구매 회원님께만";obj[LK::kMyLocalKey_firstPurchaseMent1] = 
	" 적용됩니다.";obj[LK::kMyLocalKey_firstPurchaseMent2] = 
	"이 창을 닫으면 %s 할인 기회는 사라집니다.";obj[LK::kMyLocalKey_firstPurchaseMent3] = 
	"지금 묶음 아이템을 구매하시면";obj[LK::kMyLocalKey_emptyItemSaleMent1] = 
	"%s 할인해 드립니다.";obj[LK::kMyLocalKey_emptyItemSaleMent2] = 
	"플레이를 도와주는 기능성 아이템을";obj[LK::kMyLocalKey_stupidNpuHelpMent1] = 
	"%s할인된 가격으로 사용해보세요.";obj[LK::kMyLocalKey_stupidNpuHelpMent2] = 
	"묶음아이템 할인";obj[LK::kMyLocalKey_packageItemSale] = 
	"5번째 구매시";obj[LK::kMyLocalKey_mileageMent1] = 
	"아주 특별한 할인혜택의 기회가 있으니";obj[LK::kMyLocalKey_mileageMent2] = 
	"혜택을 꼭! 확인해주세요.";obj[LK::kMyLocalKey_mileageMent3] = 
	"단 한번의 구매 찬스";obj[LK::kMyLocalKey_eventShopTitle] = 
	"당신만을 위한 특별한 기회!";obj[LK::kMyLocalKey_eventShopMent1] = 
	"%s 보너스 혜택을 드려요.";obj[LK::kMyLocalKey_eventShopMent2] = 
	"이 창을 닫으면 %s 보너스 기회는 사라집니다.";obj[LK::kMyLocalKey_eventShopMent3] = 
	"<font>미사일 레벨을 올리면 </font><font color=961>부하몬스터를 쉽게 죽이고,</font>";obj[LK::kMyLocalKey_levelupGuideMent1] = 
	"<font color=961>패턴공격을 빨리 저지시킬 수 있으며,</font>";obj[LK::kMyLocalKey_levelupGuideMent2] = 
	"<font color=961>고득점 기록에 도움이 됩니다.</font>";obj[LK::kMyLocalKey_levelupGuideMent3] = 
	"<font>미사일 업그레이드를 하러 갈까요?</font>";obj[LK::kMyLocalKey_levelupGuideMent4] = 
	"업그레이드";obj[LK::kMyLocalKey_levelupGo] = 
	"업적";obj[LK::kMyLocalKey_achievement] = 
	"모든보상 받기";obj[LK::kMyLocalKey_allRewardGet] = 
	"종합 아이템 구매 찬스!";obj[LK::kMyLocalKey_itemPackageChance] = 
	"아이템이 하나도 없네요.";obj[LK::kMyLocalKey_emptyItemSaleTitle] = 
	"첫구매 대폭할인!!";obj[LK::kMyLocalKey_firstPurchaseSaleTitle] = 
	"사랑합니다. 고객님!";obj[LK::kMyLocalKey_mileageTitle] = 
	"미사일 업그레이드";obj[LK::kMyLocalKey_missileUpgrade] = 
	"<font>누적영역</font><font color=961>%d%%</font><font strokesize=0.5 strokeopacity=150>를 획득하라</font>";obj[LK::kMyLocalKey_todaymissionTotalPercent1] = 
	"%s%%";obj[LK::kMyLocalKey_todaymissionTotalPercent2] = 
	" 획득하라!!";obj[LK::kMyLocalKey_todaymissionTotalPercent3] = 
	"현재 획득영역 ";obj[LK::kMyLocalKey_todaymissionTotalPercent4] = 
	"<font>누적점수</font><font color=961>%s점</font><font strokesize=0.5 strokeopacity=150>을 획득하라</font>";obj[LK::kMyLocalKey_todaymissionTotalScore1] = 
	"%s점";obj[LK::kMyLocalKey_todaymissionTotalScore2] = 
	" 획득하라!!";obj[LK::kMyLocalKey_todaymissionTotalScore3] = 
	"현재 획득점수 ";obj[LK::kMyLocalKey_todaymissionTotalScore4] = 
	"<font color=961>%s골드</font><font strokesize=0.5 strokeopacity=150>를 획득하라</font>";obj[LK::kMyLocalKey_todaymissionTotalTakeGold1] = 
	"%s골드";obj[LK::kMyLocalKey_todaymissionTotalTakeGold2] = 
	"현재 획득골드 ";obj[LK::kMyLocalKey_todaymissionTotalTakeGold3] = 
	"<font>부하몹</font><font color=961>%d마리</font><font strokesize=0.5 strokeopacity=150>를 획득하라</font>";obj[LK::kMyLocalKey_todaymissionTotalCatch1] = 
	"%s마리";obj[LK::kMyLocalKey_todaymissionTotalCatch2] = 
	" 사냥하라!!";obj[LK::kMyLocalKey_todaymissionTotalCatch3] = 
	"현재 잡은 부하몹 ";obj[LK::kMyLocalKey_todaymissionTotalCatch4] = 
	"완료";obj[LK::kMyLocalKey_complete] = 
	"업적달성";obj[LK::kMyLocalKey_achieveSuccess] = 
	"업적미완성";obj[LK::kMyLocalKey_achieveNotSuccess] = 
	"업적보상";obj[LK::kMyLocalKey_achieveReward] = 
	"달성한 업적이 없습니다.";obj[LK::kMyLocalKey_nothingSuccessAchieve] = 
	"보상받을 업적이 없습니다.";obj[LK::kMyLocalKey_nothingRewardAchieve] = 
	"영역획득";obj[LK::kMyLocalKey_missionTitle0] = 
	"용감한 기사";obj[LK::kMyLocalKey_missionTitle1] = 
	"부하몹 사냥";obj[LK::kMyLocalKey_missionTitle2] = 
	"욕심쟁이";obj[LK::kMyLocalKey_missionTitle3] = 
	"수집가";obj[LK::kMyLocalKey_missionTitle4] = 
	"완벽주의자";obj[LK::kMyLocalKey_missionTitle5] = 
	"결벽주의자";obj[LK::kMyLocalKey_missionTitle6] = 
	"비지니스맨";obj[LK::kMyLocalKey_missionTitle7] = 
	"<font color=961 size=13>%.0f%%이상<font color=999 size=13> 획득하라!";obj[LK::kMyLocalKey_missionDiscription0] = 
	"<font color=999 size=13>보스의 에너지를 모두 소진시켜라!";obj[LK::kMyLocalKey_missionDiscription1] = 
	"<font color=999 size=13>부하 몬스터를 가두어 잡으세요!";obj[LK::kMyLocalKey_missionDiscription2] = 
	"<font color=999 size=13>정해진 횟수만큼 한번에 많이 먹으세요!";obj[LK::kMyLocalKey_missionDiscription3] = 
	"<font color=999 size=13>정해진 숫자만큼 아이템을 모으세요!";obj[LK::kMyLocalKey_missionDiscription4] = 
	"<font color=999 size=13>정해진 목표로 정확하게 영역을 획득하세요!";obj[LK::kMyLocalKey_missionDiscription5] = 
	"<font color=999 size=13>CHANGE를 순서대로 획득하세요!";obj[LK::kMyLocalKey_missionDiscription6] = 
	"<font color=999 size=13>제한시간 내에 클리어하세요!";obj[LK::kMyLocalKey_missionDiscription7] = 
	"";obj[LK::kMyLocalKey_missionCondition0] = 
	"";obj[LK::kMyLocalKey_missionCondition1] = 
	"<font color=961 size=18>%d마리";obj[LK::kMyLocalKey_missionCondition2] = 
	"<font color=961 size=18>%d%%이상 %d번";obj[LK::kMyLocalKey_missionCondition3] = 
	"<font>아이템을 </font><font color=961 size=18>%d개</font><font> 모으세요!</font>";obj[LK::kMyLocalKey_missionCondition4] = 
	"";obj[LK::kMyLocalKey_missionCondition5] = 
	"";obj[LK::kMyLocalKey_missionCondition6] = 
	"<font color=961 size=18>%d초";obj[LK::kMyLocalKey_missionCondition7] = 
	"대쉬";obj[LK::kMyLocalKey_item4title] = 
	"잠깐동안 빠른속도로 이동 가능합니다.";obj[LK::kMyLocalKey_item4ment] = 
	"부하몹제거";obj[LK::kMyLocalKey_item5title] = 
	"부하 몬스터 한마리를 제거합니다.";obj[LK::kMyLocalKey_item5ment] = 
	"침묵";obj[LK::kMyLocalKey_item7title] = 
	"잠깐동안 몬스터가 공격을 못하도록 합니다.";obj[LK::kMyLocalKey_item7ment] = 
	"부활";obj[LK::kMyLocalKey_item10title] = 
	"마지막 순간에 부활합니다.";obj[LK::kMyLocalKey_item10ment] = 
	"시간추가";obj[LK::kMyLocalKey_item8title] = 
	"제한시간을 늘려줍니다.";obj[LK::kMyLocalKey_item8ment] = 
	"일시정지";obj[LK::kMyLocalKey_pause] = 
	"계속하기";obj[LK::kMyLocalKey_continue] = 
	"나가기";obj[LK::kMyLocalKey_ingameOut] = 
	"재시작";obj[LK::kMyLocalKey_ingameReplay] = 
	"대중교통\n모드";obj[LK::kMyLocalKey_publicPlaceMode] = 
	"<font newline=16>오늘의 임무을 완료하였습니다.<font color=961 newline=16>보상은 우편함으로 지급됩니다.";obj[LK::kMyLocalKey_todaymissionSuccessCommon] = 
	"무료";obj[LK::kMyLocalKey_free] = 
	"영역 다시뽑기";obj[LK::kMyLocalKey_areaReGacha] = 
	"도전하기";obj[LK::kMyLocalKey_go100percent] = 
	"계정연결";obj[LK::kMyLocalKey_accountLink] = 
	"최고 레벨 %d";obj[LK::kMyLocalKey_maxLevel] = 
	"업그레이드 완료";obj[LK::kMyLocalKey_endUpgrade] = 
	"%d단계 카드 획득방법";obj[LK::kMyLocalKey_cardLockedTitle] = 
	"<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다.";obj[LK::kMyLocalKey_cardLockedMent1] = 
	"<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다.";obj[LK::kMyLocalKey_cardLockedMent2] = 
	"<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 85% 획득<font color=999 size=12>하면 됩니다.";obj[LK::kMyLocalKey_cardLockedMent3] = 
	"<font color=961 size=12>체인지 알파벳 6개<font color=999 size=12 newline=18>를 모두 모은후<font color=961 size=12>영역을 100% 획득<font color=999 size=12>하면 됩니다.";obj[LK::kMyLocalKey_cardLockedMent4] = 
	"<font color=999 size=13 strokesize=0.5 strokeopacity=150>남은시간";obj[LK::kMyLocalKey_todaymissionRemainTime] = 
	"<font color=999 size=13 strokesize=0.5 strokeopacity=150>남은시간";obj[LK::kMyLocalKey_todaymissionRemainTimeMinute] = 
	"<font color=961 size=15>%dHour";obj[LK::kMyLocalKey_todaymissionRemainTime2] = 
	"<font color=961 size=15>%dMinute";obj[LK::kMyLocalKey_todaymissionRemainTimeMinute2] = 
	"<font color=961 size=15>모든 피스 획득!";obj[LK::kMyLocalKey_puzzleSuccessTitle] = 
	"<font newline=18>모든 스테이지의<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 하트를 드릴께요!";obj[LK::kMyLocalKey_puzzleSuccessMent] = 
	"<font color=961 size=15>놀라워요! <font color=961 size=15>퍼펙트 클리어!!";obj[LK::kMyLocalKey_puzzlePerfectTitle] = 
	"<font newline=18>모든 스테이지의<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 젬를 드릴께요!";obj[LK::kMyLocalKey_puzzlePerfectMent] = 
	"이어하시겠습니까?";obj[LK::kMyLocalKey_doYouWantToContinue] = 
	"이어하기";obj[LK::kMyLocalKey_onContinue] = 
	"그만하기";obj[LK::kMyLocalKey_offContinue] = 
	"이번 스테이지는 아이템을 사용할 수 없습니다.";obj[LK::kMyLocalKey_notUseItem] = 
	"새로운 아이템 등장!";obj[LK::kMyLocalKey_newItemTitle] = 
	"새로운 아이템을 선물로 드릴께요!";obj[LK::kMyLocalKey_newItemMent] = 
	"보스가 쏜 미사일이\n선에 닿으면 불씨가 생겨 닿으면 죽어요.";obj[LK::kMyLocalKey_dieTutorial1] = 
	"선을 그을때 몬스터가\n선에 닿으면 파동이 생겨 죽어요!";obj[LK::kMyLocalKey_dieTutorial2] = 
	"";obj[LK::kMyLocalKey_dieTutorial3] = 
	"남은시간 ";obj[LK::kMyLocalKey_restTime] = 
	"%d Day";obj[LK::kMyLocalKey_restTimeDay] = 
	"%d Hour";obj[LK::kMyLocalKey_restTimeHour] = 
	"%d Min";obj[LK::kMyLocalKey_restTimeMinute] = 
	"%d Sec";obj[LK::kMyLocalKey_restTimeSecond] = 
	"게임을 종료하시겠습니까?";obj[LK::kMyLocalKey_exit] = 
	"카운트 폭탄";obj[LK::kMyLocalKey_warning9] = 
	"주황 톱니바퀴";obj[LK::kMyLocalKey_warning105] = 
	"파란 톱니바퀴";obj[LK::kMyLocalKey_warning106] = 
	"파괴 레이저";obj[LK::kMyLocalKey_warning107] = 
	"미사일";obj[LK::kMyLocalKey_warning108] = 
	"돌진";obj[LK::kMyLocalKey_warning109] = 
	"불꽃놀이";obj[LK::kMyLocalKey_warning110] = 
	"해바라기";obj[LK::kMyLocalKey_warning111] = 
	"얼음폭탄";obj[LK::kMyLocalKey_warning112] = 
	"유도폭탄";obj[LK::kMyLocalKey_warning113] = 
	"토네이도";obj[LK::kMyLocalKey_warning1001] = 
	"태양권";obj[LK::kMyLocalKey_warning1002] = 
	"슬로우존";obj[LK::kMyLocalKey_warning1003] = 
	"가시감옥";obj[LK::kMyLocalKey_warning1004] = 
	"빙결";obj[LK::kMyLocalKey_warning1005] = 
	"혼란";obj[LK::kMyLocalKey_warning1006] = 
	"순간이동";obj[LK::kMyLocalKey_warning1007] = 
	"투명망토";obj[LK::kMyLocalKey_warning1008] = 
	"화염 방사";obj[LK::kMyLocalKey_warning1009] = 
	"벼락";obj[LK::kMyLocalKey_warning1010] = 
	"스피드레이저";obj[LK::kMyLocalKey_warning1011] = 
	"방사능";obj[LK::kMyLocalKey_warning1012] = 
	"메테오";obj[LK::kMyLocalKey_warning1013] = 
	"운석";obj[LK::kMyLocalKey_warning1014] = 
	"뾰족풍차";obj[LK::kMyLocalKey_warning1015] = 
	"다이너마이트 지뢰";obj[LK::kMyLocalKey_warning1016] = 
	"불꽃스토커";obj[LK::kMyLocalKey_warning1017] = 
	"문득구름";obj[LK::kMyLocalKey_warning1018] = 
	"거대화";obj[LK::kMyLocalKey_warning1019] = 
	"부하몬스터 소환";obj[LK::kMyLocalKey_warning1020] = 
	"보스공격저지";obj[LK::kMyLocalKey_warningBossSuccess] = 
	"마지막 목숨";obj[LK::kMyLocalKey_warningLastLife] = 
	"새로운 보스 공격!";obj[LK::kMyLocalKey_newBossPattern] = 
	"맵에 부딪힐때마다 숫자가 줄고, 0이 되면 폭발해요.";obj[LK::kMyLocalKey_patternContent9] = 
	"하나의 톱날이 획득영역을 부숴요.";obj[LK::kMyLocalKey_patternContent105] = 
	"여러개의 톱날이 획득영역을 부숴요.";obj[LK::kMyLocalKey_patternContent106] = 
	"레이저가 획득영역을 부숴요.";obj[LK::kMyLocalKey_patternContent107] = 
	"그리는도중 맞지 않도록 주의하세요.";obj[LK::kMyLocalKey_patternContent108] = 
	"보스가 획득영역을 파괴하며 이동해요.";obj[LK::kMyLocalKey_patternContent109] = 
	"획득영역에서 폭발후 미사일을 퍼트려요.";obj[LK::kMyLocalKey_patternContent110] = 
	"사방으로 미사일을 발사해요.";obj[LK::kMyLocalKey_patternContent111] = 
	"연속으로 발사되어 획득영역을 부숴요.";obj[LK::kMyLocalKey_patternContent112] = 
	"따라와서 폭발해요.";obj[LK::kMyLocalKey_patternContent113] = 
	"맵의 일부를 일정시간 가립니다.";obj[LK::kMyLocalKey_patternContent1001] = 
	"일정시간 화면 전체를 뒤덮어 시야를 가려요.";obj[LK::kMyLocalKey_patternContent1002] = 
	"슬로우존 안에서는 이동속도가 느려져요.";obj[LK::kMyLocalKey_patternContent1003] = 
	"붉은 원의 테두리에 닿으면 생명 하나를 잃어요.";obj[LK::kMyLocalKey_patternContent1004] = 
	"일정시간 움직이지 못해요.";obj[LK::kMyLocalKey_patternContent1005] = 
	"조작 방향의 반대로 움직여요.";obj[LK::kMyLocalKey_patternContent1006] = 
	"보스가 다른위치로 순간이동해요.";obj[LK::kMyLocalKey_patternContent1007] = 
	"일정시간 보스가 보이지 않아요.";obj[LK::kMyLocalKey_patternContent1008] = 
	"획득영역에 불을 피워요.";obj[LK::kMyLocalKey_patternContent1009] = 
	"캐릭터를 따라오며 맵을 부숴요.";obj[LK::kMyLocalKey_patternContent1010] = 
	"그리는중 맞지 않도록 주의하세요.";obj[LK::kMyLocalKey_patternContent1011] = 
	"방사능이 퍼진곳을 조심하세요.";obj[LK::kMyLocalKey_patternContent1012] = 
	"대각선으로 떨어져 맵을 부숴요.";obj[LK::kMyLocalKey_patternContent1013] = 
	"위에서 아래로 운석이 떨어져요.";obj[LK::kMyLocalKey_patternContent1014] = 
	"닿지않도록 주의하세요.";obj[LK::kMyLocalKey_patternContent1015] = 
	"카운트가 끝나면 폭발해요.";obj[LK::kMyLocalKey_patternContent1016] = 
	"일정시간 맵을 따라 움직여요.";obj[LK::kMyLocalKey_patternContent1017] = 
	"언제 폭발할지 몰라요.";obj[LK::kMyLocalKey_patternContent1018] = 
	"보스몬스터가 거대해 져요.";obj[LK::kMyLocalKey_patternContent1019] = 
	"부하몬스터를 새로 소환합니다.";obj[LK::kMyLocalKey_patternContent1020] = 
	"가운데 빨간 보석이 캐릭터 입니다.\n캐릭터를 이동시켜서 영역 가장자리를 이동할 수도 있고\n영역을 획득할 수도 있습니다.";obj[LK::kMyLocalKey_tutorial1] = 
	"먼저 영역 위를 이동하는 방법을 알려드리겠습니다.\n오른쪽 아래에 조이스틱이 있습니다.\n이 조이스틱으로 캐릭터를 원하는 방향으로 이동시킬 수 있습니다.\n조이스틱으로 캐릭터를 위로 이동시켜보세요.";obj[LK::kMyLocalKey_tutorial2] = 
	"캐릭터를 위로 이동시키기";obj[LK::kMyLocalKey_tutorial3] = 
	"다음에는 영역을 획득하는 방법을 알아보도록 해요.\n왼쪽 아래의 꾸욱 버튼을 누르고 있으면\n영역 바깥으로 나갈 수 있습니다.\n보이는 것처럼 영역을 획득해보세요.";obj[LK::kMyLocalKey_tutorial4] = 
	"영역획득하기";obj[LK::kMyLocalKey_tutorial5] = 
	"파란 실루엣 영역을 획득해야 게임 달성도가 올라갑니다.";obj[LK::kMyLocalKey_tutorial6] = 
	"<font strokesize=0>제한시간 내에 달성도 <font color=961 newline=13 strokesize=0>85%를 넘기면 클리어!!";obj[LK::kMyLocalKey_tutorial7] = 
	"기본 튜토리얼을 모두 진행하셨습니다.\n본 게임으로 들아갑니다.";obj[LK::kMyLocalKey_tutorial8] = 
	"겹친선도 획득!";obj[LK::kMyLocalKey_controlTip_lineTangle_title] = 
	"꼬인선! 걱정마세요! 그대로 획득할 수 있어요!";obj[LK::kMyLocalKey_controlTip_lineTangle_content] = 
	"한줄 먹기!";obj[LK::kMyLocalKey_controlTip_slimLine_title] = 
	"캐릭터를 왼쪽으로 이동 후 오른쪽으로 조작하면 OK!";obj[LK::kMyLocalKey_controlTip_slimLine_content] = 
	"PvP";obj[LK::kMyLocalKey_endlessMode] = 
	"주간연승랭킹";obj[LK::kMyLocalKey_endlessWeeklyranking] = 
	"전적";obj[LK::kMyLocalKey_endlessInfoScore] = 
	"<font color=999 strokecolor=000 strokesize=0>%d승 %d패 </font><font color=950 strokecolor=000 strokesize=0>(승률%d%%)</font>";obj[LK::kMyLocalKey_endlessInfoScoreValue2] = 
	"최고점수";obj[LK::kMyLocalKey_endlessHighScore] = 
	"최고연승";obj[LK::kMyLocalKey_endlessHighStraight] = 
	"<font size=16 color=#FFFFFF>%d</font><font size=10 color=#FFFFFF>연승</font>";obj[LK::kMyLocalKey_endlessHighStraightValue1] = 
	"%d연승";obj[LK::kMyLocalKey_endlessHighStraightValue2] = 
	"뭔가 문제가 발생하였습니다.\n다시 시도해주세요.";obj[LK::kMyLocalKey_endlessServerError] = 
	"스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다.";obj[LK::kMyLocalKey_endlessDownloadFail] = 
	"스페셜데이";obj[LK::kMyLocalKey_attendanceSpecialTitle] = 
	"매일 매일 출석 하자!!";obj[LK::kMyLocalKey_attendanceSeqTitle] = 
	"누적출석 일수에 따라 더욱 푸짐한 선물을 드립니다.";obj[LK::kMyLocalKey_attendanceSpecialContent] = 
	"<font size=13.5>%d<font size=10>골드";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypeGold] = 
	"<font size=13.5>%d<font size=10>젬";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypeRuby] = 
	"<font size=13.5>%d<font size=10>개";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypeItem9] = 
	"<font size=13.5>%d<font size=10>개";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypeItem6] = 
	"<font size=13.5>%d<font size=10>개";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypeItem11] = 
	"<font size=13.5>%d<font size=10>장";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypePass1] = 
	"<font size=13.5>%d<font size=10>장";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypePass2] = 
	"<font size=13.5>%d<font size=10>장";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypePass3] = 
	"<font size=13.5>%d<font size=10>장";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypePass4] = 
	"<font size=13.5>%d<font size=10>장";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypePass5] = 
	"<font size=13.5>%d<font size=10>장";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypePass6] = 
	"<font size=13.5>%d<font size=10>종류";obj[LK::kMyLocalKey_attendanceSpecialGoodsTypeMany] = 
	"<font size=12>%d<font size=8>골드";obj[LK::kMyLocalKey_attendanceGoodsTypeGold] = 
	"<font size=12>%d<font size=8>젬";obj[LK::kMyLocalKey_attendanceGoodsTypeRuby] = 
	"개";obj[LK::kMyLocalKey_attendanceGoodsTypeItem9] = 
	"개";obj[LK::kMyLocalKey_attendanceGoodsTypeItem6] = 
	"개";obj[LK::kMyLocalKey_attendanceGoodsTypeItem11] = 
	"<font size=12>%d<font size=8>장";obj[LK::kMyLocalKey_attendanceGoodsTypePass1] = 
	"<font size=12>%d<font size=8>장";obj[LK::kMyLocalKey_attendanceGoodsTypePass2] = 
	"<font size=12>%d<font size=8>장";obj[LK::kMyLocalKey_attendanceGoodsTypePass3] = 
	"<font size=12>%d<font size=8>장";obj[LK::kMyLocalKey_attendanceGoodsTypePass4] = 
	"<font size=12>%d<font size=8>장";obj[LK::kMyLocalKey_attendanceGoodsTypePass5] = 
	"<font size=12>%d<font size=8>장";obj[LK::kMyLocalKey_attendanceGoodsTypePass6] = 
	"선물상자";obj[LK::kMyLocalKey_attendanceGoodsTypeMany] = 
	"출석체크";obj[LK::kMyLocalKey_attendanceTitle] = 
	"지급된 보상은 수신함에서 확인 가능합니다.";obj[LK::kMyLocalKey_attendanceContent] = 
	"일차";obj[LK::kMyLocalKey_attendanceDay] = 
	"생명의 돌";obj[LK::kMyLocalKey_buyMorphingTitle] = 
	"<font color=961 newline=18>생명의 돌을 사용하면 카드가 살아납니다.<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다.";obj[LK::kMyLocalKey_buyMorphingContent] = 
	"보유갯수 :";obj[LK::kMyLocalKey_buyMorphingTake] = 
	"개";obj[LK::kMyLocalKey_buyMorphingTakeValue] = 
	"사용하기";obj[LK::kMyLocalKey_buyMorphingBuy] = 
	"생명의 돌이 부족합니다.";obj[LK::kMyLocalKey_notEnoughtMorphingTitle] = 
	"<font newline=18>생명의 돌은 PVP 및 각종 이벤트에서<font newline=18>보상으로 얻을 수 있습니다.";obj[LK::kMyLocalKey_notEnoughtMorphingContent] = 
	"쿠폰등록";obj[LK::kMyLocalKey_couponTitle] = 
	"입력";obj[LK::kMyLocalKey_couponContent] = 
	"쿠폰등록성공";obj[LK::kMyLocalKey_couponSuccess] = 
	"우편함을 확인하세요.";obj[LK::kMyLocalKey_couponGiftbox] = 
	"쿠폰등록실패";obj[LK::kMyLocalKey_couponFail] = 
	"이미 사용한 쿠폰입니다.";obj[LK::kMyLocalKey_couponAlready] = 
	"유효기간이 만료 쿠폰입니다.";obj[LK::kMyLocalKey_couponExpire] = 
	"잘못된 사용입니다.";obj[LK::kMyLocalKey_couponOserror] = 
	"쿠폰을 찾을 수 없습니다.";obj[LK::kMyLocalKey_couponDontfind] = 
	"통신이 원활하지 않습니다.\n잠시후 다시 시도해주세요.";obj[LK::kMyLocalKey_couponOtherError] = 
	"액자 오픈 조건";obj[LK::kMyLocalKey_notOpenPuzzleNotEnoughtStarTitle] = 
	"<font newline=16>별이 부족해요.<font newline=16>더 높은 등급으로 클리어해서<font color=961 newline=16>별 %d개 모아주세요.";obj[LK::kMyLocalKey_notOpenPuzzleNotEnoughtStarContent] = 
	"대전 준비";obj[LK::kMyLocalKey_endlessStartSettingTitle] = 
	"영역점수";obj[LK::kMyLocalKey_endlessCalcTitleAreaScore] = 
	"공격점수";obj[LK::kMyLocalKey_endlessCalcTitleDamageScore] = 
	"콤보점수";obj[LK::kMyLocalKey_endlessCalcTitleComboScore] = 
	"생명점수";obj[LK::kMyLocalKey_endlessCalcTitleLifeScore] = 
	"시간점수";obj[LK::kMyLocalKey_endlessCalcTitleTimeScore] = 
	"등급점수";obj[LK::kMyLocalKey_endlessCalcTitleGradeScore] = 
	"획득골드";obj[LK::kMyLocalKey_endlessCalcTitleTakeGold] = 
	"획득영역";obj[LK::kMyLocalKey_endlessCalcTitleTakeArea] = 
	"피격점수";obj[LK::kMyLocalKey_endlessCalcTitleDamagedScore] = 
	"승";obj[LK::kMyLocalKey_win] = 
	"게임결과";obj[LK::kMyLocalKey_gameResult] = 
	"실패";obj[LK::kMyLocalKey_fail] = 
	"달성!!";obj[LK::kMyLocalKey_achieve] = 
	"닫기";obj[LK::kMyLocalKey_close] = 
	"%d연승중";obj[LK::kMyLocalKey_endlessIngWin] = 
	"PVP 오픈조건";obj[LK::kMyLocalKey_endlessOpenConditionTitle] = 
	"<font color=961 newline=16>PVP는 %d스테이지 플레이 후<font newline=16>자동으로 오픈됩니다.";obj[LK::kMyLocalKey_endlessOpenConditionContent] = 
	"대전패배";obj[LK::kMyLocalKey_endlessKeepWin] = 
	"연승저장";obj[LK::kMyLocalKey_endlessKeepWinTitle] = 
	"연승 정보는 유지되며, 다시 PvP 도전 시\n저장된 연승에 이어서 플레이할 수 있습니다.";obj[LK::kMyLocalKey_endlessKeepWinContent] = 
	"%d연승 도전";obj[LK::kMyLocalKey_endlessReadyIngWin] = 
	"닉네임 오류";obj[LK::kMyLocalKey_nicknameError] = 
	"<font color=#FFFFFF strokesize=1 >남은 판수 </font><font color=#FFAA14 size=12 strokesize=1>%d회 / </font><font color=#FFFFFF strokesize=1>성공시 </font><font color=#FFAA14 size=12 strokesize=1>%.1f%% 획득</font>";obj[LK::kMyLocalKey_onePercentDesc2] =
	"대전상대를 찾는 중입니다.";obj[LK::kMyLocalKey_rivalSearch] = 
	"%d레벨";obj[LK::kMyLocalKey_rivalLevel] = 
	"%d 연승 중";obj[LK::kMyLocalKey_rivalIngWin] = 
	"대전상대를 선택해주세요.";obj[LK::kMyLocalKey_rivalSelect] = 
	"업적완료";obj[LK::kMyLocalKey_achieveSuccess2] = 
	"소녀들의 위치를 파악하고 있습니다.";obj[LK::kMyLocalKey_titleTempScript1] = 
	"소녀들의 의상을 점검하고 있습니다.";obj[LK::kMyLocalKey_titleTempScript2] = 
	"소녀들을 불러모으고 있습니다.";obj[LK::kMyLocalKey_titleTempScript3] = 
	"하트 공짜";obj[LK::kMyLocalKey_heartFree] = 
	"지난주 랭킹 결과";obj[LK::kMyLocalKey_rankRewardTitle] = 
	"보상선물들은 우편함에서 확인하세요.";obj[LK::kMyLocalKey_rankRewardContent] = 
	"주간 누적 랭킹";obj[LK::kMyLocalKey_rankRewardStageTitle] = 
	"지난주 누적점수";obj[LK::kMyLocalKey_rankRewardStageScore] = 
	"지난주 내순위";obj[LK::kMyLocalKey_rankRewardStageRank] = 
	"지난주 플레이횟수";obj[LK::kMyLocalKey_rankRewardStagePlayCount] = 
	"PvP 랭킹";obj[LK::kMyLocalKey_rankRewardEndlessTitle] = 
	"지난주 누적점수";obj[LK::kMyLocalKey_rankRewardEndlessScore] = 
	"지난주 내순위";obj[LK::kMyLocalKey_rankRewardEndlessRank] = 
	"지난주 연승기록";obj[LK::kMyLocalKey_rankRewardEndlessVictory] = 
	"우편함에서\n%s를 받으세요.";obj[LK::kMyLocalKey_rankRewardStageReward] = 
	"우편함에서\n%s를 받으세요.";obj[LK::kMyLocalKey_rankRewardEndlessReward] = 
	"위(%0.f%%)";obj[LK::kMyLocalKey_rankRewardStageRankValue] = 
	"위(%0.f%%)";obj[LK::kMyLocalKey_rankRewardEndlessRankValue] = 
	"%s연승";obj[LK::kMyLocalKey_rankRewardEndlessVictoryScript] = 
	"%d위";obj[LK::kMyLocalKey_rankRewardRankValue] = 
	"%s골드";obj[LK::kMyLocalKey_rankRewardGoodsTypeGold] = 
	"%s젬";obj[LK::kMyLocalKey_rankRewardGoodsTypeRuby] = 
	"아이템";obj[LK::kMyLocalKey_rankRewardGoodsTypeItem9] = 
	"아이템";obj[LK::kMyLocalKey_rankRewardGoodsTypeItem6] = 
	"아이템";obj[LK::kMyLocalKey_rankRewardGoodsTypeItem11] = 
	"티켓";obj[LK::kMyLocalKey_rankRewardGoodsTypePass1] = 
	"티켓";obj[LK::kMyLocalKey_rankRewardGoodsTypePass2] = 
	"티켓";obj[LK::kMyLocalKey_rankRewardGoodsTypePass3] = 
	"티켓";obj[LK::kMyLocalKey_rankRewardGoodsTypePass4] = 
	"티켓";obj[LK::kMyLocalKey_rankRewardGoodsTypePass5] = 
	"티켓";obj[LK::kMyLocalKey_rankRewardGoodsTypePass6] = 
	"선물상자";obj[LK::kMyLocalKey_rankRewardGoodsTypeMany] = 
	"카드 승급 이벤트!";obj[LK::kMyLocalKey_rankUpEventTitle] = 
	"승급 비용 공짜! 승급 확률 100%!!";obj[LK::kMyLocalKey_rankUpEventContent] = 
	"아니오";obj[LK::kMyLocalKey_no] = 
	"네";obj[LK::kMyLocalKey_yes] = 
	"<font>기본 클리어조건 <font color=961 newline=17>%.0f%%를<font color=961>%.0f%%로 하향<font newline=17> 시켜드립니다.<font newline=17>적용하시겠습니까?";obj[LK::kMyLocalKey_clearTimeEventMent] = 
	"이벤트";obj[LK::kMyLocalKey_clearTimeEventTitle] = 
	"숨겨진 업적";obj[LK::kMyLocalKey_achieveHiddenTitle] = 
	"저를 찾아주세요! 곳곳에 숨겨져 있어요!";obj[LK::kMyLocalKey_achieveHiddenContent] = 
	"<font newline=20>액자를 선택하면<font color=961 newline=20>퍼즐화면으로 이동합니다.";obj[LK::kMyLocalKey_mainflowDimmed1] = 
	"<font newline=16>획득한<font color=999>별갯수";obj[LK::kMyLocalKey_mainflowDimmed2] = 
	"<font newline=20>우편함";obj[LK::kMyLocalKey_mainflowDimmed3] = 
	"<font newline=20>업적";obj[LK::kMyLocalKey_mainflowDimmed4] = 
	"<font newline=20>설정";obj[LK::kMyLocalKey_mainflowDimmed5] = 
	"<font newline=20>이벤트";obj[LK::kMyLocalKey_mainflowDimmed6] = 
	"<font newline=20>랭킹";obj[LK::kMyLocalKey_mainflowDimmed7] = 
	"<font newline=20>상점";obj[LK::kMyLocalKey_mainflowDimmed8] = 
	"<font newline=20>카드";obj[LK::kMyLocalKey_mainflowDimmed9] = 
	"<font newline=16>오늘의<font newline=16>임무";obj[LK::kMyLocalKey_mainflowDimmed10] = 
	"모든 피스 획득!";obj[LK::kMyLocalKey_puzzleClearCardTitle] = 
	"<font color=961>모든 피스를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!";obj[LK::kMyLocalKey_puzzleClearCardContent] = 
	"모든 카드 획득!";obj[LK::kMyLocalKey_puzzlePerfectCardTitle] = 
	"<font color=961>모든 카드를 획득<font newline=18>하셨네요!<font color=961 newline=18>보상으로 특별한 카드 한 장을 드릴께요!";obj[LK::kMyLocalKey_puzzlePerfectCardContent] = 
	"%d연승 돌파!";obj[LK::kMyLocalKey_endlessSeqWinRewardTitle] = 
	"%d연승을 축하드립니다!";obj[LK::kMyLocalKey_endlessSeqWinRewardContent] = 
	"<font newline=15>%d골드를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeGold] = 
	"<font newline=15>%d젬을 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeRuby] = 
	"<font newline=15>기본속도업아이템 %d개를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeItem9] = 
	"<font newline=15>더블아이템 %d개를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeItem6] = 
	"<font newline=15>자석아이템 %d개를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeItem11] = 
	"<font newline=15>이어하기 이용권 %d개를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass1] = 
	"<font newline=15>맵가챠 이용권 %d개를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass2] = 
	"<font newline=15>업그레이드 이용권 %d개를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass3] = 
	"<font newline=15>아이템뽑기 이용권 %d개를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass4] = 
	"<font newline=15>1프로찬스 이용권 %d개를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass5] = 
	"<font newline=15>생명의 돌을 %d개를 드립니다.<font newline=15>생명의 돌은 내카드 다이어리에서 사용하세요.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypePass6] = 
	"<font newline=15>선물상자를 드립니다.";obj[LK::kMyLocalKey_endlessSeqWinRewardGoodsTypeMany] = 
	"퍼즐에서 획득한 카드입니다.";obj[LK::kMyLocalKey_cardSettingMyCardContent] = 
	"스페셜 카드 보기";obj[LK::kMyLocalKey_cardSettingMyCardToSpecial] = 
	"모든 피스\n획득시!";obj[LK::kMyLocalKey_cardSettingClearCardMent] = 
	"퍼펙트\n클리어시!";obj[LK::kMyLocalKey_cardSettingPerfectCardMent] = 
	"스페셜 카드";obj[LK::kMyLocalKey_cardSettingSpecialCardTitle] = 
	"이벤트를 통해 얻을 수 있는 카드입니다.";obj[LK::kMyLocalKey_cardSettingSpecialCardContent] = 
	"내 카드 보기";obj[LK::kMyLocalKey_cardSettingSpecialCardToMyCard] = 
	"<font newline=16>PVP모드가<font newline=16>오픈되었어요!";obj[LK::kMyLocalKey_endlessTutorialMent1] = 
	"<font newline=16>내 정보 입니다.";obj[LK::kMyLocalKey_endlessTutorialMent2] = 
	"<font>목록에서 <font color=961>다른 친구를 선택<font newline=16>해보세요.<font newline=16>다른 친구의 정보를 볼 수 있어요.";obj[LK::kMyLocalKey_endlessTutorialMent3] = 
	"<font color=961>대전준비를 눌러보세요.";obj[LK::kMyLocalKey_endlessTutorialMent4] = 
	"<font color=961>대전시작을 눌러보세요.";obj[LK::kMyLocalKey_endlessTutorialMent5] = 
	"<font color=961>내 미니맵";obj[LK::kMyLocalKey_endlessTutorialMent6] = 
	"<font color=961>내 점수";obj[LK::kMyLocalKey_endlessTutorialMent7] = 
	"<font color=961>내 장착아이템";obj[LK::kMyLocalKey_endlessTutorialMent8] = 
	"<font color=961>상대 정보";obj[LK::kMyLocalKey_endlessTutorialMent9] = 
	"<font color=961 newline=16>5콤보 달성시 마다<font color=961 newline=16>미사일을 발사하여<font color=961 newline=16>상대의 점수를 깎습니다.";obj[LK::kMyLocalKey_endlessTutorialMent10] = 
	"화면을 터치하면 게임 진행됩니다.";obj[LK::kMyLocalKey_endlessTutorialMent11] = 
	"<font color=961>나의 결과";obj[LK::kMyLocalKey_endlessTutorialMent12] = 
	"<font color=961>상대방 결과";obj[LK::kMyLocalKey_endlessTutorialMent13] = 
	"<font>스크롤하면 점수 내역을 볼 수 있어요.";obj[LK::kMyLocalKey_endlessTutorialMent14] = 
	"<font color=961>2연승에 도전할 수 있어요!";obj[LK::kMyLocalKey_endlessTutorialMent15] = 
	"잠깐!";obj[LK::kMyLocalKey_endlessTutorialMent16] = 
	"3연승을 할 때마다 생명의 돌을 1개\n받을 수 있습니다. 생명의 돌은 내가 가진\n카드에 생명을 불어넣을 수 있는\n특별한 아이템이니 소중히 사용해주세요.\n튜토리얼을 마치겠습니다. 그럼 전 이만!";obj[LK::kMyLocalKey_endlessTutorialMent17] = 
	"<font>대전 상대를 선택해주세요.";obj[LK::kMyLocalKey_endlessTutorialMent18] = 
	"에피소드 %d";obj[LK::kMyLocalKey_puzzleValue] = 
	"보너스 영역 뽑기";obj[LK::kMyLocalKey_onePercentTutorial1] = 
	"<font color=#FFFFFF size=10 newline=12>안녕하세요!</font>"
								   "<font color=#FFFFFF size=10 newline=12>난 카츠라기라고 해요.</font>"
								   "<font color=#FFFFFF size=10 newline=12>아슬아슬하게 영역을</font>"
								   "<font color=#FFFFFF size=10 newline=24>100% 획득 못했군요.</font>"
								   "<font color=#FFFFFF size=10 newline=12>100% 영역획득 할 수 있게</font>"
								   "<font color=#FFFFFF size=10>내가 도와줄게요.</font>";obj[LK::kMyLocalKey_onePercentTutorial2] = 
	"다  음";obj[LK::kMyLocalKey_onePercentTutorial3] = 
	"<font color=#FFFFFF size=10 newline=14>아래 게이지바에서 </font>"
								   "<font color=#FFFFFF size=10 newline=14>노란색 영역을 맞히면 퍼센트를</font>"
								   "<font color=#FFFFFF size=10 newline=28>추가로 획득할 수 있어요.</font>"
								   "<font color=#FFFFFF size=10 newline=14>기회는 총 3번 이에요!</font>"
								   "<font color=#FFFFFF size=10 newline=14>100% 에 도전해 볼까요?</font>";obj[LK::kMyLocalKey_onePercentTutorial4] = 
	"<font color=#FFAA14 size=11 newline=16>뽑기 기회는 단 3번!!</font><font color=#FFFFFF size=11>부족한 영역을 채워 100%에 도전하세요.</font>";obj[LK::kMyLocalKey_onePercentGame1] = 
	"<font color=#FFAA14 size=11 newline=16>100%에 성공시</font><font color=#FFAA14 size=11 tag=1>한 단계 높은 카드</font><font color=#FFFFFF size=11>를 획득할 수 있어요.</font>";obj[LK::kMyLocalKey_onePercentGame2] = 
	"<font color=#FFFFFF size=13>현재 획득 영역</font><font color=#FFFF00 size=35 strokecolor=000 strokesize=2 tag=1>99.9%</font>";obj[LK::kMyLocalKey_onePercentGame3] = 
	"<font size=12 color=#FFFFFF newline=16>포인터가 노란색 영역에</font>"
								   "<font size=12 color=#FFFFFF newline=16>들어왔을 때</font><font size=12 color=#FFFFFF>멈춰 버튼을 눌러주세요!</font>";obj[LK::kMyLocalKey_onePercentGame4] = 
	"멈춰!";obj[LK::kMyLocalKey_onePercentGame5] = 
	"100% 실패";obj[LK::kMyLocalKey_onePercentGame6] = 
	"<font color=#FF0000 size=12>최종 획득 영역     </font>"
								   "<font color=#FF0000 size=30>%.1f%%</font>";obj[LK::kMyLocalKey_onePercentGame7] = 
	"닫기";obj[LK::kMyLocalKey_onePercentGame8] = 
	"100% 달성!!";obj[LK::kMyLocalKey_onePercentGame9] = 
	"카드 받기";obj[LK::kMyLocalKey_onePercentGame10] = 
	"보상내용";obj[LK::kMyLocalKey_endlessRewardInfo] = 
	"<font color=#FFFFFF size=12>가위 바위 보 중</font>"
								   "<font color=#FFAA14 size=12>하나를 선택 해 주세요.</font>";obj[LK::kMyLocalKey_gababoContent1] = 
	"<font size=12>연승보상</font>";obj[LK::kMyLocalKey_gababoContent2] = 
	"보상";obj[LK::kMyLocalKey_gababoContent3] = 
	"1승";obj[LK::kMyLocalKey_gababoContent4] = 
	"2승";obj[LK::kMyLocalKey_gababoContent5] = 
	"3승";obj[LK::kMyLocalKey_gababoContent6] = 
	"<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>";obj[LK::kMyLocalKey_gababoContent7] = 
	"확 인";obj[LK::kMyLocalKey_gababoContent8] = 
	"나";obj[LK::kMyLocalKey_gababoContent9] = 
	"<font color=#FFFFFF size=12 newline=14>이런, 무승부네요!</font>"
								   "<font color=#FFAA14 size=12>한번 더!</font>";obj[LK::kMyLocalKey_gababoContent10] = 
	"<font color=#FFFFFF size=12 newline=14>와우! 이겼어요!</font>"
								   "<font color=#FFAA14 size=12>더 좋은 보상을 향해 한번 더!</font>";obj[LK::kMyLocalKey_gababoContent11] = 
	"<font color=#FFFFFF size=12 newline=14>와우! 4판 모두 이기다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>최고의 보상을 드릴게요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>보너스 게임은 퍼펙트 클리어시마다</font>"
								   "<font color=#FFAA14 size=12 newline=14>할 수 있어요!</font><font color=#FFFFFF size=12>그럼 다음에 또 만나요!</font>";obj[LK::kMyLocalKey_gababoContent12] = 
	"<font color=#FFFFFF size=12 newline=14>이런이런... 지고 말았네요.</font>"
								   "<font color=#FFFFFF size=12>너무 상심마세요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>보너스게임은</font>"
								   "<font color=#FFAA14 size=12 newline=14>퍼펙트 클리어시마다 할 수 있어요!</font>"
								   "<font color=#FFFFFF size=12>다음엔 꼭 저를 이겨보세요!</font>";obj[LK::kMyLocalKey_gababoContent13] = 
	"루우비~!";obj[LK::kMyLocalKey_gababoContent14] = 
	"<font color=#FFFFFF size=12 newline=14>보너스 게임에</font>"
								   "<font color=#FFFFFF size=12 newline=14>오신 것을 환영합니다.</font>"
								   "<font color=#FFFFFF size=12 newline=14>전 히바리라고 해요.</font>"
								   "<font color=#FFFFFF size=12 newline=14>벌써 퍼즐을 퍼펙트 클리어</font>"
								   "<font color=#FFFFFF size=12 newline=28>하시다니 정말 대단해요!</font>"
								   "<font color=#FFAA14 size=12 newline=14>게임 설명은 최초 1회만</font>"
								   "<font color=#FFAA14 size=12> 해드리니 잘 봐주세요!</font>"
								   obj[LK::kMyLocalKey_gababoContent15] = 
	"다 음";obj[LK::kMyLocalKey_gababoContent16] = 
	"<font color=#FFFFFF size=12 newline=43>게임은 가위바위보 입니다!</font>"
								   "<font color=#FFAA14 size=12 newline=14>버튼 3개중 하나를 선택하여</font>"
								   "<font color=#FFAA14 size=12 newline=14>저에게 이기시면 됩니다.</font>"
								   "<font color=#FFAA14 size=12 newline=14>연승 횟수에 따라</font>"
								   "<font color=#FFAA14 size=12 newline=14>더 큰 보상이 있어요!</font>"
								   "<font color=#FFFFFF size=12 newline=14>그럼 행운을 빌어요!</font>";obj[LK::kMyLocalKey_gababoContent17] = 
	"+%d";obj[LK::kMyLocalKey_gababoReward] = 
	"PVP 튜토리얼 중에는 나가실 수 없습니다.";obj[LK::kMyLocalKey_notGohomeEndlessTutorialMent] = 
	"PVP 중에는 재시도할 수 없습니다.";obj[LK::kMyLocalKey_notReplayEndlessMent] = 
	"대중교통 모드";obj[LK::kMyLocalKey_optionSecretTitle] = 
	"<font newline=16>대중교통모드가 적용중입니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 해제하실 수 있습니다.";obj[LK::kMyLocalKey_optionSecretOn] = 
	"<font newline=16>대중교통모드가 풀렸습니다.<font newline=16>게임화면 내에 있는 설정창을 통해<font newline=16>대중교통모드를 적용하실 수 있습니다.";obj[LK::kMyLocalKey_optionSecretOff] = 
	"카드 선물하기";obj[LK::kMyLocalKey_cardGiftTitle] = 
	"<font color=999>내 아이디는 <font color=961>%s<font color=999> 입니다.";obj[LK::kMyLocalKey_cardGiftMyID] = 
	"아이디를 입력해주세요.";obj[LK::kMyLocalKey_cardGiftInputText] = 
	"검색";obj[LK::kMyLocalKey_cardGiftSearch] = 
	"선물하기";obj[LK::kMyLocalKey_cardGiftSend] = 
	"검색결과가 없습니다.";obj[LK::kMyLocalKey_cardGiftNotFound] = 
	"<font color=999>실패시 다음번 승급 성공확률이 <font color=961>%.0f%%<font color=999> 올라갑니다.";obj[LK::kMyLocalKey_rankUpFailRateUp] = 
	"<font color=999>이벤트를 놓치지 마세요!!";obj[LK::kMyLocalKey_rankUpEventSubMent] = 
	"타임 오버";obj[LK::kMyLocalKey_failTitleTimeover] = 
	"임무 실패";obj[LK::kMyLocalKey_failTitleMissionfail] = 
	"게임 오버";obj[LK::kMyLocalKey_failTitleGameover] = 
	"스테이지 클리어";obj[LK::kMyLocalKey_clearTitle] = 
	"모든 퍼즐 클리어";obj[LK::kMyLocalKey_allPuzzleClearTitle] = 
	"<font newline=15>마지막 퍼즐을 클리어하셨습니다.<font newline=15>다음 퍼즐은 곧 업데이트 됩니다.<font newline=15>아직 획득하지 못한 카드들을<font>모두 모아보세요.";obj[LK::kMyLocalKey_allPuzzleClearMent] = 
	"주의!";obj[LK::kMyLocalKey_warningDie] = 
	"쿠폰 번호를 입력해주세요.";obj[LK::kMyLocalKey_couponMent] = 
	"내 정보";obj[LK::kMyLocalKey_nickTitle] = 
	"추가지급";obj[LK::kMyLocalKey_plusGive] = 
	"정보";obj[LK::kMyLocalKey_pvpInfo] = 
	"보상내용";obj[LK::kMyLocalKey_pvpReward] = 
	"선택";obj[LK::kMyLocalKey_choice] = 
	"스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다.";obj[LK::kMyLocalKey_stageImgLoadFail] = 
	"카드선물";obj[LK::kMyLocalKey_cardGiftSuccessTitle] = 
	"카드를 선물하였습니다.";obj[LK::kMyLocalKey_cardGiftSuccessContent] = 
	"프로필";obj[LK::kMyLocalKey_profile] = 
	"카드 속성";obj[LK::kMyLocalKey_cardElemental] = 
	"터치";obj[LK::kMyLocalKey_touch] = 
	"사운드";obj[LK::kMyLocalKey_sound] = 
	"속성(%s)";obj[LK::kMyLocalKey_elementalValue] = 
	"내카드수 ";obj[LK::kMyLocalKey_myCardCount] = 
	"다이어리 보기";obj[LK::kMyLocalKey_diaryView] = 
	"보유장수 %d";obj[LK::kMyLocalKey_cardTakeCnt] = 
	"<font newline=20>땅을 획득하려면<font color=961 newline=20>꾸욱 버튼을 누른상태에서<font newline=20>조이스틱을 움직여주세요!";obj[LK::kMyLocalKey_buttonTutorialMent] = 
	"\"한 번 입력한 닉네임은 변경할 수 없으니 신중히 선택해 주세요!\"";obj[LK::kMyLocalKey_nickBottomMent] = 
	"아이디를 입력해주세요.";obj[LK::kMyLocalKey_pleaseInputID] = 
	"유효하지 않은 아이디 입니다.";obj[LK::kMyLocalKey_invalidID] = 
	"다음";obj[LK::kMyLocalKey_tutorialNextButton] = 
	"<font newline=16>선택된</font><font>스테이지</font>";obj[LK::kMyLocalKey_puzzleDimmed1] = 
	"<font>스테이지</font>";obj[LK::kMyLocalKey_puzzleDimmed2] = 
	"<font newline=16>스테이지</font><font>클리어 정보</font>";obj[LK::kMyLocalKey_puzzleDimmed3] = 
	"<font>게임준비</font>";obj[LK::kMyLocalKey_puzzleDimmed4] = 
	"<font>클리어 조건</font>";obj[LK::kMyLocalKey_stageSettingDimmed1] = 
	"<font>시작버튼</font>";obj[LK::kMyLocalKey_stageSettingDimmed2] = 
	"다시보지않기";obj[LK::kMyLocalKey_noReview] = 
	"젬 100개";obj[LK::kMyLocalKey_gem100] = 
	"<font size=12 strokesize=0.5 strokeopacity=150>PvP랭킹은 연승이 중요합니다.</font>";obj[LK::kMyLocalKey_endlessOpeningMarquee1] = 
	"<font size=12 strokesize=0.5 strokeopacity=150>연승수가 같은 경우 누적점수로 랭킹이 결정됩니다.</font>";obj[LK::kMyLocalKey_endlessOpeningMarquee2] = 
	"<font size=12 strokesize=0.5 strokeopacity=150>PvP점수는 주간점수 랭킹에 추가되지 않습니다.</font>";obj[LK::kMyLocalKey_endlessOpeningMarquee3] = 
	"<font size=12 strokesize=0.5 strokeopacity=150>한 주간의 기록 중 가장 높은 연승기록으로 랭킹이 결정됩니다.</font>";obj[LK::kMyLocalKey_endlessOpeningMarquee4] = 
	"<font size=12 strokesize=0.5 strokeopacity=150>랭킹은 매주 월요일 5시에 초기화 됩니다.</font>";obj[LK::kMyLocalKey_endlessOpeningMarquee5] = 
	"<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>이번주 플레이 누적 점수입니다.</font>";obj[LK::kMyLocalKey_rankNewMarquee1] = 
	"<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>PvP점수는 합산되지 않습니다.</font>";obj[LK::kMyLocalKey_rankNewMarquee2] = 
	"<font size=13 strokesize=0.5 strokeopacity=150 strokecolor=#000000>주간랭킹은 매주 월요일 5시에 초기화 됩니다.</font>";obj[LK::kMyLocalKey_rankNewMarquee3] = 
	"드디어 오늘부터 닌자훈련을 시작하는구나!!\n할아버지의 명성에 걸 맞는\n훌륭한 닌자가 될꺼야!";obj[LK::kMyLocalKey_scenarioMent1] = 
	"여기가 훈련장인가?\n인기척도 없고 전력으로 훈련할 수 있겠는데?!";obj[LK::kMyLocalKey_scenarioMent2] = 
	"안녕하세요.\n저는 한조국립대 학생 리더인 이카루가입니다.";obj[LK::kMyLocalKey_scenarioMent3] = 
	"앗 안녕하세요.\n저는 아스카 라고합니다.";obj[LK::kMyLocalKey_scenarioMent4] = 
	"네. 전설의 닌자 한조님의 손녀라고\n들었습니다. 잘 부탁 드립니다.\n훈련 전에 선생님께서 간단한 시험을\n하신다는군요.";obj[LK::kMyLocalKey_scenarioMent5] = 
	"으윽, 시험은 그다지 좋아하지 않지만\n알겠습니다.";obj[LK::kMyLocalKey_scenarioMent6] = 
	"어떤 시험인가요?";obj[LK::kMyLocalKey_scenarioMent7] = 
	"시노비 결계 속에서 대전하는 법을\n익히시면 됩니다.";obj[LK::kMyLocalKey_scenarioMent8] = 
	"네. 그러면 잘 부탁 드리겠습니다.";obj[LK::kMyLocalKey_scenarioMent9] = 
	"그럼 간단히 메뉴들을 소개해 드리겠습니다.";obj[LK::kMyLocalKey_scenarioMent10] = 
	"숙지하겠습니다.";obj[LK::kMyLocalKey_scenarioMent11] = 
	"그럼 시험장으로 이동 하죠.\n1번액자로 입장합니다.";obj[LK::kMyLocalKey_scenarioMent12] = 
	"이곳은 스테이지를 선택할 수 있는\n퍼즐화면입니다.";obj[LK::kMyLocalKey_scenarioMent13] = 
	"먼저 1스테이지로 이동하시죠.";obj[LK::kMyLocalKey_scenarioMent14] = 
	"이곳은 게임시작 전 아이템이나\n무기를 업그레이드 시킬 수 있는\n게임시작화면입니다.";obj[LK::kMyLocalKey_scenarioMent15] = 
	"게임을 시작하려면 하트가 필요합니다.";obj[LK::kMyLocalKey_scenarioMent16] = 
	"지금은 하트가 충분하니\n바로 시작해볼까요?";obj[LK::kMyLocalKey_scenarioMent17] = 
	"오랜만이네요. 시노비 결계속..";obj[LK::kMyLocalKey_scenarioMent18] = 
	"네 우리 닌자는 일반인의 안전을 위해\n이곳에서 훈련하죠.";obj[LK::kMyLocalKey_scenarioMent19] = 
	"컨트롤 방법부터 다시 익혀볼까요?";obj[LK::kMyLocalKey_scenarioMent20] = 
	"옵션에는 조이스틱 크기조절, 대각선이동,\n조이스틱고정 등 다양한 컨트롤 옵션이 있어요.\n제한시간 내에 달성도 85%를 넘기면\n시험 합격입니다.\n그럼 시작해볼까요?";obj[LK::kMyLocalKey_scenarioMent21] = 
	"잠깐!\n깜빡하고 지나갈 뻔했네요.";obj[LK::kMyLocalKey_scenarioMent22] = 
	"시험을 잘 볼 수 있게 도와드릴게요.\n \n왼쪽에 보이는 것이 지금 플레이하고 있는\n스테이지의 미니맵이예요.\n여기서 보스와 나의 위치를 파악하면 되겠죠?";obj[LK::kMyLocalKey_scenarioMent23] = 
	"게임을 잠시 멈추거나 게임을 나가고 싶다면\n일시정지 버튼을 눌러주세요.\n일시정지에는 유용한 기능들이 있으니\n한번 확인해보세요.";obj[LK::kMyLocalKey_scenarioMent24] = 
	"여기에 점수와 획득한 골드, 그리고 콤보가\n표시됩니다. 자 이제 시작해 볼까요?";obj[LK::kMyLocalKey_scenarioMent25] = 
	"축하합니다. 첫 번째 시험을 통과하셨군요.\n게임이 끝나면 이곳으로 오게 됩니다.";obj[LK::kMyLocalKey_scenarioMent26] = 
	"스테이지 랭킹";obj[LK::kMyLocalKey_scenarioMent27] = 
	"점수 정보";obj[LK::kMyLocalKey_scenarioMent28] = 
	"클리어 등급";obj[LK::kMyLocalKey_scenarioMent29] = 
	"그럼 계속 해볼까요?\n5스테이지 클리어 후 메인화면에서\n뵙기로 하죠!";obj[LK::kMyLocalKey_scenarioMent30] = 
	"네! 그럼 최선을 다 해보겠습니다!";obj[LK::kMyLocalKey_scenarioMent31] = 
	"이런.. 다음엔 더 잘 할 수 있겠죠?\n여긴 게임이 끝나면 오게 됩니다.";obj[LK::kMyLocalKey_scenarioMent32] = 
	"휴.. 이걸로 시험끝!!\n드디어 훈련시작이네~!";obj[LK::kMyLocalKey_scenarioMent33] = 
	"어랏. 아스카도 한조국립대에서 훈련하는거야?\n같이 최고의 닌자를 꿈꿀 수 있겠구나!!";obj[LK::kMyLocalKey_scenarioMent34] = 
	"쉿. 일반 학생들이 다니는 교정 안에서는\n닌자에 관한 이야기는 하지 않는게 좋아.\n이곳의 닌자 양성 클래스는\n일반 학생들에겐 비밀이니까!";obj[LK::kMyLocalKey_scenarioMent35] = 
	"흐응... 비밀이라니...\n조금 긴장하게 되..";obj[LK::kMyLocalKey_scenarioMent36] = 
	"히바리, 긴장할 필요 까진 없어.\n언제나처럼 시노비결계 안에서 훈련하기\n때문에 보통 사람들은 볼 수 없으니까.";obj[LK::kMyLocalKey_scenarioMent37] = 
	"응, 알고는 있는데... 아스카짱 고마워\n앞으로나 완전 열심히 할꺼야!";obj[LK::kMyLocalKey_scenarioMent38] = 
	"히바리 위험해!!!\n긴장을 늦추지 마. 적은 남아 있다구!!";obj[LK::kMyLocalKey_scenarioMent39] = 
	"야규! 히바리를 도와주었구나!";obj[LK::kMyLocalKey_scenarioMent40] = 
	"어... 어떻게 라는 건 잘 모르겠고.\n교내에도 아직 적이 남아 있으니\n주변의 적을 해치우자.";obj[LK::kMyLocalKey_scenarioMent41] = 
	"야규, 아까는 도와줘서 고마워.\n히바리가 거치적거리니까\n이대로면 모두한테 폐를 끼치게 돼.";obj[LK::kMyLocalKey_scenarioMent42] = 
	"히바리는 거치적 거리는 존재 따위가 아니야.\n우리들은 모두 강하다고 인정받았으니까.\n히바리의 강한 점은 내가 제일 잘 알고있고,\n또 모두들 알고 있어.\n그러니까 더 자신에게 자신감을 가져.";obj[LK::kMyLocalKey_scenarioMent43] = 
	"야규..";obj[LK::kMyLocalKey_scenarioMent44] = 
	"내.. 내가 말하고 싶은 건 그것뿐이야..\n히바리, 내일은 히바리가 좋아하는\n우사네 찻집에 가자.";obj[LK::kMyLocalKey_scenarioMent45] = 
	"응!";obj[LK::kMyLocalKey_scenarioMent46] = 
	"이카루가.. 그리고\n야규, 아스카, 히바리도 마침 딱 있었네!";obj[LK::kMyLocalKey_scenarioMent47] = 
	"카츠라기님. 오늘 훈련은 끝났는데\n집에 안가세요?";obj[LK::kMyLocalKey_scenarioMent48] = 
	"어, 응..음... 뭐라고 할까\n소화불량이라고나 할까...\n어쨌든 좀 더 몸을 움직이려고... 응?";obj[LK::kMyLocalKey_scenarioMent49] = 
	"아앗! 저건 뭐지?";obj[LK::kMyLocalKey_scenarioMent50] = 
	"소매치기예요!! 꼬마의 물건을 훔쳐가네요!!";obj[LK::kMyLocalKey_scenarioMent51] = 
	"아이의 즐거움을 뺏는\n배짱 좋은 녀석이군.\n저런 썩은 자식은 우리들이 혼내줄테다!!";obj[LK::kMyLocalKey_scenarioMent52] = 
	"우와! 실력이 일취월장인데요?\n벌써 실력이 많이 늘었네요.\n이젠 다른사람과 대전을 해볼까요?\n \n따라오세요.";obj[LK::kMyLocalKey_scenarioMent53] = 
	"이건 내 정보예요.\n보상내용도 나중에 확인해보세요.";obj[LK::kMyLocalKey_scenarioMent54] = 
	"다른 친구를 누르면 해당친구의 정보를\n볼 수 있어요.\n \nPVP를 시작해보죠.";obj[LK::kMyLocalKey_scenarioMent55] = 
	"제가 당신과 붙을 만한 상대 세분을\n골라봤어요.\n \n셋중 한명을 선택해주세요.";obj[LK::kMyLocalKey_scenarioMent56] = 
	"아아아!! 죄송해요!!\n평소 대전과 달라서 설명을 드려야하는걸\n제가 까먹었네요.";obj[LK::kMyLocalKey_scenarioMent57] = 
	"폭탄이예요! 5콤보시마다 미사일을\n발사해서 상대방의 점수를 깎을 수 있어요!\n조심하세요! 상대방이 절 공격 못하게!\n영역을 획득할 때 발사되는 미사일이\n보스를 타격하게 되면 콤보가 쌓이게 되고,\n콤보점수가 추가됩니다.";obj[LK::kMyLocalKey_scenarioMent58] = 
	"행운을 빌어요.";obj[LK::kMyLocalKey_scenarioMent59] = 
	"첫 대전이라 떨리시죠?\n \n이기면 보상이 있으니 행운을 빌어요.\n그럼 전 이만..";obj[LK::kMyLocalKey_scenarioMent60] = 
	"PVP가 끝나면 이곳으로 와요.";obj[LK::kMyLocalKey_scenarioMent61] = 
	"모두 게임오버 되었을 때는\n피격점수로 승패를 가릅니다.";obj[LK::kMyLocalKey_scenarioMent62] = 
	"업적 메뉴에서는 게임 중 달성한 업적을\n확인하고 보상을 받을 수 있습니다.\n앞으로 해야 할 업적들이 정말 많군요!";obj[LK::kMyLocalKey_kindTutorial1] = 
	"모든보상받기 버튼을 누르면 달성한 업적\n보상을 모두 한번에 받을 수 있어요!";obj[LK::kMyLocalKey_kindTutorial2] = 
	"하하! 또 만나네요.\n내카드에서는 스테이지에서 획득한 카드를\n확인할 수 있어요.";obj[LK::kMyLocalKey_kindTutorial3] = 
	"내가 획득한 카드나 다이어리 보기 버튼을\n터치하면 카드의 상세 내용을 확인할 수\n있는 다이어리로 이동합니다.";obj[LK::kMyLocalKey_kindTutorial4] = 
	"상단의 정렬버튼을 눌러 여러가지 옵션으로\n정렬기능을 이용할 수도 있습니다.";obj[LK::kMyLocalKey_kindTutorial5] = 
	"어머! 여기서 또 만나네요?\n이번엔 랭킹에 대해 설명해 드릴께요.\n \n주간랭킹은 일주일 동안 획득한 스테이지\n점수를 합산하여 랭킹이 산정됩니다.";obj[LK::kMyLocalKey_kindTutorial6] = 
	"순위마다 보상 내용이 다르니\n잘 확인해 주세요.";obj[LK::kMyLocalKey_kindTutorial7] = 
	"드디어 뽑기상자가 등장했네요.";obj[LK::kMyLocalKey_kindTutorial8] = 
	"이건 뭐죠? 아이템인가요?";obj[LK::kMyLocalKey_kindTutorial9] = 
	"특별한 아이템을 뽑을 수 있는 상자입니다.\n운이 좋으면 목숨 하나가 추가되는\n부활아이템을 얻을 수 있죠.";obj[LK::kMyLocalKey_kindTutorial10] = 
	"껄끄러운 게임임무이로군.\n히바리. 이번 게임에선 부하몬스터를\n표시된 개수만큼 가둬 잡아야 클리어할 수 있어.\n너라면 충분히 해낼 수 있을꺼야.";obj[LK::kMyLocalKey_kindTutorial11] = 
	"응! 야규짱!";obj[LK::kMyLocalKey_kindTutorial12] = 
	"이번엔 게임임무이 조금 다른 것 같아요.";obj[LK::kMyLocalKey_kindTutorial13] = 
	"네. 주어진 개수만큼 생성되는 아이템을\n먹어야 클리어 됩니다.\n클리어하기 더 어려워지겠는데요?";obj[LK::kMyLocalKey_kindTutorial14] = 
	"게임시간이 엄청나게 줄어들었어요.\n조금 서둘러야겠는걸요.";obj[LK::kMyLocalKey_kindTutorial15] = 
	"알고있어!! 게임 중에 생기는 시간아이템을\n적절히 먹으면서 플레이하면 문제없을꺼야!!";obj[LK::kMyLocalKey_kindTutorial16] = 
	"앗! 이건 뭐죠?";obj[LK::kMyLocalKey_kindTutorial17] = 
	"이 신발을 착용하면 게임을\n더 빠른 스피드로 시작할 수 있어요.\n무료로 몇 개 지급되니 사용해보세요.";obj[LK::kMyLocalKey_kindTutorial18] = 
	"새로운 아이템이 등장했네요.";obj[LK::kMyLocalKey_kindTutorial19] = 
	"게임 중에 아이템이 두 배로 더 많이\n나오도록 해주는 인법서군요.\n많은 도움이 될겁니다.";obj[LK::kMyLocalKey_kindTutorial20] = 
	"이건 자석인가?";obj[LK::kMyLocalKey_kindTutorial21] = 
	"네. 게임 중에 생기는 골드나 아이템을\n더 쉽게 먹을 수 있도록 도와주는\n아이템입니다.";obj[LK::kMyLocalKey_kindTutorial22] = 
	"최고스피드!";obj[LK::kMyLocalKey_maxSpeed] = 
	"쇼타임!";obj[LK::kMyLocalKey_showtime] = 
	"미스";obj[LK::kMyLocalKey_miss] = 
	"콤보";obj[LK::kMyLocalKey_combo] = 
	"잠금이 해제되었습니다!";obj[LK::kMyLocalKey_unlockedAreaScroll] = 
	"임무성공!";obj[LK::kMyLocalKey_missionComplete] = 
	"체인지를 모으세요!";obj[LK::kMyLocalKey_collectChange] = 
	"정식오픈 후 사용할 수 있습니다.";obj[LK::kMyLocalKey_afterOpenCBT] = 
	"카드 체인지!";obj[LK::kMyLocalKey_cardChange] = 
	"100% 쇼타임에서는 생명의 돌을 체험할 수 있어요.";obj[LK::kMyLocalKey_showtimeMorphingTouch] = 
	"카드강화";obj[LK::kMyLocalKey_cardStrength] = 
	"업데이트 예정";obj[LK::kMyLocalKey_updateTitle] = 
	"추후 업데이트 됩니다.";obj[LK::kMyLocalKey_updateContent] = 
	"준비중";obj[LK::kMyLocalKey_communityNotOpenTitle] = 
	"준비입니다. 공식카페를 이용해 주세요.";obj[LK::kMyLocalKey_communityNotOpenContent] = 
	"조이스틱\n크기";obj[LK::kMyLocalKey_joystickSize] = 
	"대각선\n사용";obj[LK::kMyLocalKey_useSideDirection] = 
	"S다이어리";obj[LK::kMyLocalKey_toDiary19] = 
	"S 다이어리";obj[LK::kMyLocalKey_toDiary19Title] = 
	"<font size=12 newline=15>S 다이어리가 없네요.</font><font size=12 newline=15>더욱 화끈하고! 쎈! 그녀들이 기다립니다.</font><font size=12>해당앱으로 연결하시겠습니까?</font>";obj[LK::kMyLocalKey_toDiary19Content] = 
	"S 다이어리 다운 받기";obj[LK::kMyLocalKey_toDiary19Button] = 
	"조이스틱\n진동";obj[LK::kMyLocalKey_joystickVib] = 
	"조이스틱 크기조절";obj[LK::kMyLocalKey_joystickSizeQuestionTitle] = 
	"<font newline=17>게임에서 사용되는 조이스틱의 크기를</font><font newline=17>9단계의 크기로 조절하고 있습니다.</font><font color=961>평균 5단계를 사용합니다.</font>";obj[LK::kMyLocalKey_joystickSizeQuestionContent] = 
	"헬모드";obj[LK::kMyLocalKey_missionTitle8] = 
	"목표 달성률";obj[LK::kMyLocalKey_missionTitle9] = 
	"목표 점수";obj[LK::kMyLocalKey_missionTitle10] = 
	"목표 콤보";obj[LK::kMyLocalKey_missionTitle11] = 
	"목표 골드";obj[LK::kMyLocalKey_missionTitle12] = 
	"목표 턴수";obj[LK::kMyLocalKey_missionTitle13] = 
	"<font>오래 버티세요!";obj[LK::kMyLocalKey_missionDiscription8] = 
	"<font color=961 size=13>%d%%이상<font color=999 size=13> 획득하라!";obj[LK::kMyLocalKey_missionDiscription9] = 
	"<font>높은 점수를 달성하세요!";obj[LK::kMyLocalKey_missionDiscription10] = 
	"<font>높은 콤보를 달성하세요!";obj[LK::kMyLocalKey_missionDiscription11] = 
	"<font>많은 골드를 획득하세요!";obj[LK::kMyLocalKey_missionDiscription12] = 
	"<font>작은 턴으로 게임을 클리어 하세요!";obj[LK::kMyLocalKey_missionDiscription13] = 
	"<font>오래 버티세요!";obj[LK::kMyLocalKey_missionCondition8] = 
	"<font color=961 size=13>%d%%이상<font color=999 size=13> 획득하라!";obj[LK::kMyLocalKey_missionCondition9] = 
	"<font color=961>%s점<font color=999>이상으로 클리어하세요!";obj[LK::kMyLocalKey_missionCondition10] = 
	"<font color=961>%d콤보<font color=999>를 달성하세요!";obj[LK::kMyLocalKey_missionCondition11] = 
	"<font color=961>%s골드<font color=999>를 획득하세요!";obj[LK::kMyLocalKey_missionCondition12] = 
	"<font color=961>%d턴<font color=999>안에 게임을 클리어 하세요!";obj[LK::kMyLocalKey_missionCondition13] = 
	"%s점";obj[LK::kMyLocalKey_mission10Label] = 
	"%d콤보";obj[LK::kMyLocalKey_mission11Label] = 
	"%s골드";obj[LK::kMyLocalKey_mission12Label] = 
	"/%d턴";obj[LK::kMyLocalKey_mission13Label] = 
	"추천인등록";obj[LK::kMyLocalKey_recommenderReg] = 
	"정말 이어하시겠습니까?";obj[LK::kMyLocalKey_realContinue] = 
	"계정연결하세요!";obj[LK::kMyLocalKey_accountLinkLeadTitle] = 
	"<font newline=15>아직 계정연결 안하셨어요?!</font><font newline=15>열심히 쌓은 데이터 잃어버리지 않게</font><font>서둘러 연결하세요!</font>";obj[LK::kMyLocalKey_accountLinkLeadContent] = 
	"설정창 바로가기";obj[LK::kMyLocalKey_accountLinkLeadButton] = 
	"추천합니다!";obj[LK::kMyLocalKey_introducerTitle] = 
	"<font newline=15 strokesize=0.3 strokeopacity=50>추천인을 등록해주세요.</font><font newline=15 strokesize=0.3 strokeopacity=50>등록 즉시 보상을 드립니다.";obj[LK::kMyLocalKey_introducerContent] = 
	"추천인 닉네임";obj[LK::kMyLocalKey_introducerPlaceHolder] = 
	"확인";obj[LK::kMyLocalKey_introducerOk] = 
	"이미 추천인 등록을 하셨습니다.";obj[LK::kMyLocalKey_introducerAfter] = 
	"닉네임을 입력해주세요.";obj[LK::kMyLocalKey_introducerPleaseInputNick] = 
	"해당 닉네임을 찾을 수 없습니다.";obj[LK::kMyLocalKey_introducerDonFindNick] = 
	"%s 님을 추천인으로 등록하셨습니다.";obj[LK::kMyLocalKey_introducerSuccess] = 
	"자신을 추천인으로 등록할 수 없습니다.";obj[LK::kMyLocalKey_introducerSelfNotIntroduce] = 
	"부활석";obj[LK::kMyLocalKey_p1Shop] = 
	"추천인 입력 보상";obj[LK::kMyLocalKey_introducerInputReward] = 
	"추천인 일정 스테이지 달성 보상";obj[LK::kMyLocalKey_introducerCompleteReward] = 
	"젬 이어하기";obj[LK::kMyLocalKey_onContinueGem] = 
	"부활석 이어하기";obj[LK::kMyLocalKey_onContinueP1] = 
	"이벤트카드";obj[LK::kMyLocalKey_eventCard] = 
	"액자오픈";obj[LK::kMyLocalKey_puzzleOpenTitle] = 
	"<font newline=16>액자를 오픈하려면</font><font color=961>%s골드</font><font newline=16>가 필요합니다.</font><font>오픈하시겠습니까?</font>";obj[LK::kMyLocalKey_puzzleOpenContent] = 
	"오픈하기";obj[LK::kMyLocalKey_puzzleOpenButton] = 
    "보유수:%d";obj[LK::kMyLocalKey_takeCount] = 
    "조이스틱위치";obj[LK::kMyLocalKey_joystickPositionSelectTitle] = 
    "<font newline=18>조이스틱위치를 정해주세요.</font><font>설정에서 바꿀 수 있어요.</font>";obj[LK::kMyLocalKey_joystickPositionSelectContent] = 
    "추천";obj[LK::kMyLocalKey_joystickPositionSelectRecommend] = 
    "<font strokecolor=#410523 strokesize=2.5 strokeopacity=255>아쉽군요. </font><font color=961 strokecolor=#410523 strokesize=2.5 strokeopacity=255>%d콤보</font><font strokecolor=#410523 strokesize=2.5 strokeopacity=255>가 부족해요!</font>";obj[LK::kMyLocalKey_missionFailContextCombo] = 
    "<font strokecolor=#410523 strokesize=2.5 strokeopacity=255>아쉽군요. </font><font color=961 strokecolor=#410523 strokesize=2.5 strokeopacity=255>%d골드</font><font strokecolor=#410523 strokesize=2.5 strokeopacity=255>가 부족해요!</font>";obj[LK::kMyLocalKey_missionFailContextGold] = 
    "<font strokecolor=#410523 strokesize=2.5 strokeopacity=255>아쉽군요. 아이템 </font><font color=961 strokecolor=#410523 strokesize=2.5 strokeopacity=255>%d개</font><font strokecolor=#410523 strokesize=2.5 strokeopacity=255>가 부족해요!</font>";obj[LK::kMyLocalKey_missionFailContextItemCollect] = 
    "<font strokecolor=#410523 strokesize=2.5 strokeopacity=255>아쉽군요. </font><font color=961 strokecolor=#410523 strokesize=2.5 strokeopacity=255>%d점</font><font strokecolor=#410523 strokesize=2.5 strokeopacity=255>이 부족해요!</font>";obj[LK::kMyLocalKey_missionFailContextScore] = 
    "<font strokecolor=#410523 strokesize=2.5 strokeopacity=255>아쉽군요. 부하 </font><font color=961 strokecolor=#410523 strokesize=2.5 strokeopacity=255>%d마리</font><font strokecolor=#410523 strokesize=2.5 strokeopacity=255>가 부족해요!</font>";obj[LK::kMyLocalKey_missionFailContextSubCumberCatch] = 
    "<font strokecolor=#410523 strokesize=2.5 strokeopacity=255>아쉽군요. </font><font color=961 strokecolor=#410523 strokesize=2.5 strokeopacity=255>%d턴</font><font strokecolor=#410523 strokesize=2.5 strokeopacity=255>이 지나버렸어요!</font>";obj[LK::kMyLocalKey_missionFailContextTurns] = 
	"헬모드";obj[LK::kMyLocalKey_hellMode] = 
	"마스터카드를 획득하면 캐릭터를 장착할 수 있어요.";obj[LK::kMyLocalKey_hellMarquee] = 
	"<font color=961 strokesize=1 strokecolor=000 strokeopacity=100>%d스테이지</font><font strokesize=1 strokecolor=000 strokeopacity=100 newline=15> 클리어 후</font><font strokesize=1 strokecolor=000 strokeopacity=100>오픈 됩니다.</font>";obj[LK::kMyLocalKey_hellNotOpen] = 
	"내캐릭터";obj[LK::kMyLocalKey_mycharacter] = 
	"미사일\n업그레이드";obj[LK::kMyLocalKey_missileUpgrade2] = 
	"캐릭터변경";obj[LK::kMyLocalKey_changeCharacter] = 
	"캐릭터선택";obj[LK::kMyLocalKey_selectCharacter] = 
	"캐릭터는 마스터피스를 획득하시면 됩니다.";obj[LK::kMyLocalKey_characterMarquee] = 
	"선택중";obj[LK::kMyLocalKey_characterSelected] = 
	"선  택";obj[LK::kMyLocalKey_characterSelect] = 
	"캐릭터 카드 획득!";obj[LK::kMyLocalKey_takeCharacterCard] = 
	"<font newline=16>이미 획득한 카드 입니다.</font><font newline=16>헬모드에서는 다양한 캐릭터 카드를</font><font>획득할 수 있어요.</font>";obj[LK::kMyLocalKey_haveCharacterCard] = 
	"골드";obj[LK::kMyLocalKey_hellTitleGold] = 
	"타임";obj[LK::kMyLocalKey_hellTitleTime] = 
	"%s sec";obj[LK::kMyLocalKey_hellContentTime] = 
	"획득!";obj[LK::kMyLocalKey_hellTakeCharacter] = 
	"<font newline=16>이런.. 조금 더 힘내요!</font><font color=961>클리어시 캐릭터카드</font><font newline=16>를</font><font>획득할 수 있어요!</font>";obj[LK::kMyLocalKey_failHellMode] = 
	"<font strokecolor=000 strokesize=0.5 strokeopacity=60 newline=15>캐릭터카드를</font><font strokecolor=000 strokesize=0.5 strokeopacity=60>획득하세요!</font>";obj[LK::kMyLocalKey_needHaveCharacterCard] = 

	
		// add friend !
	m_mapper[__FUNCTION__] = obj;
}
void KsLocal::en()
{
}
void KsLocal::ja()
{
}
