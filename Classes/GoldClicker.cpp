#include "GoldClicker.h"

GoldClicker::GoldClicker()
{
	
}
GoldClicker::~GoldClicker()
{
}
//void GoldClicker::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool GoldClicker::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool GoldClicker::init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
{
	CCLayer::init();
	
	CCSprite* back = CCSprite::create("minigame_bg.png");
	addChild(back);
	back->setPosition(ccp(240, 160));
		
	return true;
}


