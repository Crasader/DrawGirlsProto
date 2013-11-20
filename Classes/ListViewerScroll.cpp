// ListViewerScroll.cpp
//

#include "ListViewerScroll.h"
#define LZZ_INLINE inline
using namespace std;
ListViewerScroll * ListViewerScroll::create (CCRect t_rect, ScrollingObject * t_link, string filename, CCPoint t_min, CCPoint t_max)
        {
		ListViewerScroll* t_lvs = new ListViewerScroll();
		t_lvs->myInit(t_rect, t_link, filename, t_min, t_max);
		t_lvs->autorelease();
		return t_lvs;
	}
void ListViewerScroll::setPercentage (float t_p)
        {
		CCPoint distance = ccpSub(max_position, min_position);
		distance = ccpAdd(min_position, ccpMult(distance, 1.f - t_p));
		if(distance.y < min_position.y)				distance.y = min_position.y;
		else if(distance.y > max_position.y)		distance.y = max_position.y;
		scroll_img->setPosition(distance);
	}
void ListViewerScroll::setPositionScrollY (float t_y)
        {
		if(t_y < min_position.y)		t_y = min_position.y;
		else if(t_y > max_position.y)	t_y = max_position.y;
		
		scroll_img->setPositionY(t_y);
		if(target_link)
			target_link->setPercentage(1.f - (t_y - min_position.y)/(max_position.y - min_position.y));
	}
bool ListViewerScroll::ccTouchBegan (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)
        {
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location = ccpSub(location, myDSH->ui_touch_convert);
		
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
void ListViewerScroll::ccTouchMoved (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)
        {
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location = ccpSub(location, myDSH->ui_touch_convert);
		
		float distance = location.y - touch_begin_point.y;
		setPositionScrollY(scroll_img->getPositionY() + distance);
		touch_begin_point = location;
	}
void ListViewerScroll::ccTouchEnded (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)
        {
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location = ccpSub(location, myDSH->ui_touch_convert);
		
		if(isVisible() && my_rect.containsPoint(location))
		{
			float distance = location.y - touch_begin_point.y;
			setPositionScrollY(scroll_img->getPositionY() + distance);
			touch_begin_point = location;
		}
		
		is_touching = false;
	}
void ListViewerScroll::ccTouchCancelled (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)
        {
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location = ccpSub(location, myDSH->ui_touch_convert);
		
		if(isVisible() && my_rect.containsPoint(location))
		{
			float distance = location.y - touch_begin_point.y;
			setPositionScrollY(scroll_img->getPositionY() + distance);
			touch_begin_point = location;
		}
		
		is_touching = false;
	}
void ListViewerScroll::registerWithTouchDispatcher ()
        {
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-2, false);
	}
void ListViewerScroll::myInit (CCRect t_rect, ScrollingObject * t_link, string filename, CCPoint t_min, CCPoint t_max)
        {
		my_rect = t_rect;
		target_link = t_link;
		
//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, t_rect.size.width, t_rect.size.height));
//		temp_back->setOpacity(100);
//		temp_back->setPosition(ccp(t_rect.origin.x+t_rect.size.width/2.f, t_rect.origin.y+t_rect.size.height/2.f));
//		addChild(temp_back);
		
		scroll_img = CCSprite::create(filename.c_str());
		addChild(scroll_img);
		
		min_position = t_min;
		max_position = t_max;
		
		is_touching = false;
		
		CCLayer::init();
	}
#undef LZZ_INLINE
