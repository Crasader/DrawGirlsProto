#include "CumberEmotion.h"
#include "ProbSelector.h"

CumberEmotion::CumberEmotion()
{
	m_currentEmotion = nullptr;
	m_boredTimer = 0.f;
}
CumberEmotion::~CumberEmotion()
{
}
//void CumberEmotion::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool CumberEmotion::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool CumberEmotion::init()
{
	CCNode::init();
	scheduleUpdate();
	return true;
}



void CumberEmotion::onTouchingLine()
{
	if(m_emotionState != EmotionState::kJoy)
	{
		presentationEmotion("emoticon_pleasant.ccbi");
		m_emotionState = EmotionState::kJoy;
	}
}
void CumberEmotion::goStun()
{
	if(m_emotionState != EmotionState::kStun)
	{
		presentationEmotion("emoticon_stun.ccbi");
		m_emotionState = EmotionState::kStun;
	}
}
void CumberEmotion::releaseStun()
{
	if(m_currentEmotion)
	{
		m_currentEmotion->removeFromParent();
		m_currentEmotion = nullptr;
	}
}
void CumberEmotion::onKillingJack()
{
	if(m_emotionState != EmotionState::kRidicule)
	{
		presentationEmotion("emoticon_ridicule.ccbi");
		m_emotionState = EmotionState::kRidicule;
	}
}

void CumberEmotion::toBored()
{
	if(m_emotionState != EmotionState::kBored)
	{
		presentationEmotion("emoticon_yawn.ccbi");
		m_emotionState = EmotionState::kBored;
	}
}
void CumberEmotion::toCry()
{
	if(m_emotionState != EmotionState::kCry)
	{
		presentationEmotion("emoticon_cry.ccbi");
		m_emotionState = EmotionState::kCry;
	}
}
void CumberEmotion::toAnger()
{
	if(m_emotionState != EmotionState::kAnger)
	{
		presentationEmotion("emoticon_anger.ccbi");
		m_emotionState = EmotionState::kAnger;
	}
}
void CumberEmotion::presentationEmotion(const std::string& emotion)
{
	if(m_currentEmotion)
	{
		m_currentEmotion->removeFromParent();
	}
	auto t1  = KS::loadCCBI<CCSprite*>(this, emotion.c_str());
	m_currentEmotion = t1.first;
	
	t1.second->setAnimationCompletedCallbackLambda(this, [=](){
		m_currentEmotion->removeFromParent();
		m_currentEmotion = nullptr;
		m_emotionState = EmotionState::kNone;
	});
	addChild(m_currentEmotion);
	m_currentEmotion->setPositionY(m_currentEmotion->getPositionY() + 50);
	m_boredTimer = 0.f;
}
void CumberEmotion::update(float dt)
{
	if(m_emotionState == EmotionState::kNone)
	{
		m_boredTimer += 1.f / 60.f;
		if(m_boredTimer >= 4.f)
		{
			ProbSelector ps = {0.10, 0.95};
			if(ps.getResult() == 0)
			{
				toBored();
			}
		}
	}
	
}