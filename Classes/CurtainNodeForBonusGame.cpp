#include "CurtainNodeForBonusGame.h"

CurtainNodeForBonusGame::CurtainNodeForBonusGame()
{
	
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

