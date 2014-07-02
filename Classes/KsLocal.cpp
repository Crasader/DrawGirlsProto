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
	m_mapper[__FUNCTION__] = obj;
}
void KsLocal::en()
{
	std::map<LK, std::string> obj;
	obj[LK::kItem1] = "게임을 시작 중 입니다...";
	obj[LK::kReGacha] = "다시 뽑기";
	obj[LK::kRewardConfirm] = "확인";
	obj[LK::kRewardTitle] = "게임 결과";
	m_mapper[__FUNCTION__] = obj;
}
void KsLocal::ja()
{
	std::map<LK, std::string> obj;
	obj[LK::kItem1] = "게임을 시작 중 입니다...";
	obj[LK::kReGacha] = "다시 뽑기";
	obj[LK::kRewardConfirm] = "확인";
	obj[LK::kRewardTitle] = "게임 결과";
	m_mapper[__FUNCTION__] = obj;
}
