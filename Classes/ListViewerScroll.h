//
//  ListViewerScroll.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 1..
//
//

#ifndef __DGproto__ListViewerScroll__
#define __DGproto__ListViewerScroll__

#include "cocos2d.h"
#include "DataStorageHub.h"

USING_NS_CC;
using namespace std;

class ScrollingObject
{
public:
	virtual void setPercentage(float t_p) = 0;
};

class ListViewerScroll : public CCLayer, public ScrollingObject
{
public:
	static ListViewerScroll* create(CCRect t_rect, ScrollingObject* t_link, string filename, CCPoint t_min, CCPoint t_max)
	{
		ListViewerScroll* t_lvs = new ListViewerScroll();
		t_lvs->myInit(t_rect, t_link, filename, t_min, t_max);
		t_lvs->autorelease();
		return t_lvs;
	}
	
	virtual void setPercentage(float t_p)
	{
		CCPoint distance = ccpSub(max_position, min_position);
		distance = ccpAdd(min_position, ccpMult(distance, 1.f - t_p));
		if(distance.y < min_position.y)				distance.y = min_position.y;
		else if(distance.y > max_position.y)		distance.y = max_position.y;
		scroll_img->setPosition(distance);
	}
	
	void setPositionScrollY(float t_y)
	{
		if(t_y < min_position.y)		t_y = min_position.y;
		else if(t_y > max_position.y)	t_y = max_position.y;
		
		scroll_img->setPositionY(t_y);
		if(target_link)
			target_link->setPercentage(1.f - (t_y - min_position.y)/(max_position.y - min_position.y));
	}
	
private:
	CCRect my_rect;
	ScrollingObject* target_link;
	CCSprite* scroll_img;
	CCPoint min_position;
	CCPoint max_position;
	
	CCPoint touch_begin_point;
	bool is_touching;
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location = ccpSub(location, myDSH->ui_zero_point);
		
		bool return_value = false;
		
		if(isVisible() && my_rect.containsPoint(location))
		{
			touch_begin_point = location;
			is_touching = true;
			
			setPercentage(1.f - (touch_begin_point.y - min_position.y) / (max_position.y - min_position.y));
			setPositionScrollY(scroll_img->getPositionY());
			
			return_value = true;
		}
		
		return return_value;
	}
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location = ccpSub(location, myDSH->ui_zero_point);
		
		float distance = location.y - touch_begin_point.y;
		setPositionScrollY(scroll_img->getPositionY() + distance);
		touch_begin_point = location;
	}
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location = ccpSub(location, myDSH->ui_zero_point);
		
		if(isVisible() && my_rect.containsPoint(location))
		{
			float distance = location.y - touch_begin_point.y;
			setPositionScrollY(scroll_img->getPositionY() + distance);
			touch_begin_point = location;
		}
		
		is_touching = false;
	}
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location = ccpSub(location, myDSH->ui_zero_point);
		
		if(isVisible() && my_rect.containsPoint(location))
		{
			float distance = location.y - touch_begin_point.y;
			setPositionScrollY(scroll_img->getPositionY() + distance);
			touch_begin_point = location;
		}
		
		is_touching = false;
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-2, false);
	}
	
	
	void myInit(CCRect t_rect, ScrollingObject* t_link, string filename, CCPoint t_min, CCPoint t_max)
	{
		my_rect = t_rect;
		target_link = t_link;
		
		scroll_img = CCSprite::create(filename.c_str());
		addChild(scroll_img);
		
		min_position = t_min;
		max_position = t_max;
		
		is_touching = false;
		
		CCLayer::init();
	}
};

#endif /* defined(__DGproto__ListViewerScroll__) */
