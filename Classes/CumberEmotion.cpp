#include "CumberEmotion.h"

CumberEmotion::CumberEmotion()
{
	m_currentEmotion = nullptr;
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
	
	return true;
}



void CumberEmotion::onTouchingLine()
{
	if(m_emotionState != EmotionState::kJoy)
	{
		presentationEmotion("emotion_joy.ccbi");
		m_emotionState = EmotionState::kJoy;
	}
}
void CumberEmotion::goStun()
{
	if(m_emotionState != EmotionState::kStun)
	{
		presentationEmotion("emotion_stun.ccbi");
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
		presentationEmotion("emotion_joy.ccbi");
		m_emotionState = EmotionState::kRidicule;
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
}