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
//	vector<string> entries = {"en", "ja"};
	langTypes.insert(langTypes.end(), {"en", "ja", "ko"});
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
													"<font color=#FFAA14>추천인 입력시 보상으로 5젬을 드립니다.</font>";
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
	
		// add friend !
	m_mapper[__FUNCTION__] = obj;
}
void KsLocal::en()
{
}
void KsLocal::ja()
{
}
