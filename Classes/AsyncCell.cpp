#include "AsyncCell.h"

#include "AsyncSprite.h"
AsyncCell::AsyncCell()
{
	
}
AsyncCell::~AsyncCell()
{
	AsyncSpriteManager::getInstance()->unregisterObject(this);
}
//void AsyncCell::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool AsyncCell::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool AsyncCell::init()
{
	CCTableViewCell::init();
		
	AsyncSpriteManager::getInstance()->registerObject(this);
	return true;
}


