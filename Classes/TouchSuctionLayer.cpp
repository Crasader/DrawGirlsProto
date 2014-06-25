//
//  TouchSuctionLayer.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 1. 9..
//
//

#include "TouchSuctionLayer.h"
#include "TouchSwallowManagement.h"

TouchSuctionLayer* TouchSuctionLayer::create(int t_touch_priority)
{
	TouchSuctionLayer* t_tnp = new TouchSuctionLayer();
	t_tnp->myInit(t_touch_priority);
	t_tnp->autorelease();
	return t_tnp;
}

void TouchSuctionLayer::setNotSwallowRect(CCRect t_rect)
{
	not_swallow_rect = t_rect;
	is_setted_not_swallow_rect = true;
}

void TouchSuctionLayer::setSwallowRect(CCRect t_rect)
{
	swallow_rect = t_rect;
	is_setted_swallow_rect = true;
}

void TouchSuctionLayer::setSwallowMent(string t_ment)
{
	swallow_ment = t_ment;
}

TouchSuctionLayer::~TouchSuctionLayer()
{
	TouchSwallowManagement::sharedInstance()->removeSwallowLayer(this);
}

void TouchSuctionLayer::touchFuncOn()
{
	is_on_touch_began_func = true;
}

void TouchSuctionLayer::myInit(int t_touch_priority)
{
	touch_priority = t_touch_priority;
	target_touch_began = NULL;
	delegate_touch_began = NULL;
	is_setted_not_swallow_rect = false;
	is_setted_swallow_rect = false;
	is_on_touch_began_func = false;
	
	swallow_ment = "touch swallow : TouchSuctionLayer";
	
	TouchSwallowManagement::sharedInstance()->addSwallowLayer(this);
}

bool TouchSuctionLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(is_setted_not_swallow_rect)
	{
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = convertToNodeSpace(touchLocation);
		
		if(not_swallow_rect.containsPoint(local))
			return false;
		else
		{
			if(target_touch_began && delegate_touch_began)
				(target_touch_began->*delegate_touch_began)();
			else
				CCLOG("%s : %d", swallow_ment.c_str(), touch_priority);
		}
	}
	else if(is_setted_swallow_rect)
	{
		CCPoint touchLocation = pTouch->getLocation();
		CCPoint local = convertToNodeSpace(touchLocation);
		
		if(!swallow_rect.containsPoint(local))
			return false;
		else
		{
			if(target_touch_began && delegate_touch_began)
				(target_touch_began->*delegate_touch_began)();
			else
				CCLOG("%s : %d", swallow_ment.c_str(), touch_priority);
		}
	}
	else
	{
		if(target_touch_began && delegate_touch_began)
			(target_touch_began->*delegate_touch_began)();
		else
			CCLOG("%s : %d", swallow_ment.c_str(), touch_priority);
		
		if(is_on_touch_began_func)
		{
			CCLOG("setted touch_func");
			touch_began_func();
		}
	}
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