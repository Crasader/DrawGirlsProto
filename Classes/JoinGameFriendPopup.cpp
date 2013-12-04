#include "JoinGameFriendPopup.h"

JoinGameFriendPopup::JoinGameFriendPopup()
{
	
}
JoinGameFriendPopup::~JoinGameFriendPopup()
{
}
//void JoinGameFriendPopup::registerWithTouchDispatcher()
//{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
//}

//bool JoinGameFriendPopup::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool JoinGameFriendPopup::init()
{
	CCLayer::init();
	
	return true;
}


