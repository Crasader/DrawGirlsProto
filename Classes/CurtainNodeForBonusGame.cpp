#include "CurtainNodeForBonusGame.h"

CurtainNodeForBonusGame::CurtainNodeForBonusGame()
{
	m_titleBonusGame = nullptr;
	m_titleStr = nullptr;
	m_contentBack = nullptr;
	 m_startMenu = nullptr;
	m_onPressStartButton = nullptr;
	m_obtainReward = nullptr;
}
CurtainNodeForBonusGame::~CurtainNodeForBonusGame()
{
}
void CurtainNodeForBonusGame::registerWithTouchDispatcher()
{
CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
pDispatcher->addTargetedDelegate(this, m_touchPriority, true);
}

bool CurtainNodeForBonusGame::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

	return true;
}

bool CurtainNodeForBonusGame::init()
{
	CCLayer::init();
	
	return true;
}


