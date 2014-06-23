#include "OnePercentTutorial.h"

OnePercentTutorial::OnePercentTutorial()
{
	
}
OnePercentTutorial::~OnePercentTutorial()
{
}
//void OnePercentTutorial::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool OnePercentTutorial::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool OnePercentTutorial::init()
{
	CCLayer::init();
	
	return true;
}


