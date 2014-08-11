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
	
	auto findIter = m_mapper.find(languageType);
	if(findIter == m_mapper.end())
	{
		return CCString::create("")->getCString();
	}
	else
	{
		return m_mapper[languageType][key].c_str();
	}
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
																											 "<font color=#FFFFFF newline=24>(%s, %d)</font>"
																											 "<font color=#FFAA14 newline=24>이 게임 기록을 불러오시겠습니까?</font>"
																											 "<font color=#FFFFFF newline=12>게스트사용자인 경우 이전 게임 기록을 불러오면</font>"
																											"<font color=#FFFFFF newline=12>현재 게임 기록(%s, %d)은 삭제됩니다.</font>";

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
	
	obj[LK::kForeverDeleteAccount1] = "<font color=#FFFFFF newline=14>현재 기록은 영구히 삭제되며</font>"
	"<font color=#FFFFFF>%s 의 이전 기록을 로드합니다.</font>";
	obj[LK::kForeverDeleteAccount2] = "<font newline=14>%s 의 이전 기록은 영구히</font>"
	"<font >삭제되며 현재기록을 유지합니다.</font>";
	obj[LK::kUnlinkAccount1] = "<font color=#FF00000 newline=14>%s 와의 연동을 해제합니다.</font>";
	obj[LK::kLinkAccount1] = "<font>%s 와 연동합니다.</font>";

	
	m_mapper[__FUNCTION__] = obj;
}
void KsLocal::en()
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
	
	obj[LK::kBonusCong] = "<font color=#FFFFFF newline=12>축하합니다.</font>"
																										 "<font color=#FFFFFF newline=12>보상선물은 선물함에서 확인하세요.</font>"
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
	"<font color=#FFFFFF newline=24>(%s, %d)</font>"
	"<font color=#FFAA14 newline=24>이 게임 기록을 불러오시겠습니까?</font>"
	"<font color=#FFFFFF newline=12>게스트사용자인 경우 이전 게임 기록을 불러오면</font>"
	"<font color=#FFFFFF newline=12>현재 게임 기록(%s, %d)은 삭제됩니다.</font>";

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

	m_mapper[__FUNCTION__] = obj;
}
void KsLocal::ja()
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
	
	obj[LK::kBonusCong] = "<font color=#FFFFFF newline=12>축하합니다.</font>"
																										 "<font color=#FFFFFF newline=12>보상선물은 선물함에서 확인하세요.</font>"
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
	"<font color=#FFFFFF newline=24>(%s, %d)</font>"
	"<font color=#FFAA14 newline=24>이 게임 기록을 불러오시겠습니까?</font>"
	"<font color=#FFFFFF newline=12>게스트사용자인 경우 이전 게임 기록을 불러오면</font>"
	"<font color=#FFFFFF newline=12>현재 게임 기록(%s, %d)은 삭제됩니다.</font>";

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

	m_mapper[__FUNCTION__] = obj;
}
