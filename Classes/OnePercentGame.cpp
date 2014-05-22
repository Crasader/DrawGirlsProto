#include "OnePercentGame.h"
#include "FormSetter.h"

OnePercentGame::OnePercentGame()
{
	m_cursorDirection = 1;	
	m_99State = 1; // 결제하기 직전.

	m_validSize = 50;
	m_touchEnable = true;
}
OnePercentGame::~OnePercentGame()
{
}
void OnePercentGame::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}

bool OnePercentGame::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
{
	CCTouch* touch = pTouch;

	CCPoint location(ccp(0, 0));
	location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());

	return true;
}

bool OnePercentGame::init()
{
	CCLayer::init();
	startFormSetter(this);
	return true;
}


