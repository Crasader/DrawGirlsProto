#include "FriendListPopup.h"

FriendListPopup::FriendListPopup()
{
	
}
FriendListPopup::~FriendListPopup()
{
}
//void FriendListPopup::registerWithTouchDispatcher()
//{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
//}

//bool FriendListPopup::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool FriendListPopup::init()
{
	CCLayer::init();
	
	return true;
}


