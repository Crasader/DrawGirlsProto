//
//  TouchSuctionLayer.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 1. 9..
//
//

#include "TouchSuctionLayer.h"

TouchSuctionLayer* TouchSuctionLayer::create(int t_touch_priority)
{
	TouchSuctionLayer* t_tnp = new TouchSuctionLayer();
	t_tnp->myInit(t_touch_priority);
	t_tnp->autorelease();
	return t_tnp;
}

void TouchSuctionLayer::myInit(int t_touch_priority)
{
	touch_priority = t_touch_priority;
	target_touch_began = NULL;
	delegate_touch_began = NULL;
}

bool TouchSuctionLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(target_touch_began && delegate_touch_began)
		(target_touch_began->*delegate_touch_began)();
	else
		CCLog("touch swallow : TouchSuctionLayer");
	return true;
}

void TouchSuctionLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void TouchSuctionLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void TouchSuctionLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void TouchSuctionLayer::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, touch_priority, true);
}