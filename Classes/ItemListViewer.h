//
//  ItemListViewer.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 1..
//
//

#ifndef __DGproto__ItemListViewer__
#define __DGproto__ItemListViewer__

#include "cocos2d.h"
#include "ListViewerScroll.h"

USING_NS_CC;
using namespace std;

class ItemListViewer : public CCLayer, public ScrollingObject
{
public:
	static ItemListViewer* create()
	{
		ItemListViewer* t_ilv = new ItemListViewer();
		t_ilv->myInit();
		t_ilv->autorelease();
		return t_ilv;
	}
	
	virtual void visit()
	{
		glEnable(GL_SCISSOR_TEST);
		
		//	int viewport [4];
		//	glGetIntegerv (GL_VIEWPORT, viewport);
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		CCSize rSize = CCEGLView::sharedOpenGLView()->getDesignResolutionSize(); // getSize
		
		float wScale = frame_size.width / rSize.width;
		float hScale = frame_size.height / rSize.height;
		float xMargine = 0;
		float yMargine = 0;
		
		if(wScale >= hScale)
		{
			wScale = hScale;
			xMargine = (frame_size.width - rSize.width*wScale)/2.f;
		}
		else
		{
			hScale = wScale;
			yMargine = (frame_size.height - rSize.height*hScale)/2.f;
		}
		
		//	float wScale = viewport[2] / rSize.width;
		//	float hScale = viewport[3] / rSize.height;
		//	float x = view_rect.origin.x*wScale + viewport[0];
		//	float y = view_rect.origin.y*hScale + viewport[1];
		//	float w = view_rect.size.width*wScale;
		//	float h = view_rect.size.height*hScale;
		
		float x = view_rect.origin.x*wScale + xMargine;
		float y = view_rect.origin.y*hScale + yMargine;
		float w = view_rect.size.width*wScale;
		float h = view_rect.size.height*hScale;
		
		glScissor(x,y,w,h);
		
		CCLayer::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
	
	virtual void setPositionY(float t_y)
	{
		if(t_y < 0)
		{
			t_y = 0;
			unschedule(schedule_selector(ItemListViewer::moveAnimation));
		}
		else if(t_y > max_positionY)
		{
			t_y = max_positionY;
			unschedule(schedule_selector(ItemListViewer::moveAnimation));
		}
		
		CCLayer::setPositionY(t_y);
		if(target_link)
			target_link->setPercentage(t_y / max_positionY);
	}
	
	void setPercentage(float t_p)
	{
		setPosition(ccp(getPositionX(),max_positionY*t_p));
	}
	
	void setMaxPositionY()
	{
		int cnt = getChildrenCount();
		
		float t_y = cnt*43 + 5;
		t_y -= view_rect.size.height;
		
		if(t_y > 0)
			max_positionY = t_y;
		else
			max_positionY = 0;
	}
	
	void setScroll(ScrollingObject* t_link)
	{
		target_link = t_link;
		setPositionY(getPositionY());
	}
	
private:
	
	CCRect view_rect;
	bool is_touching;
	int touched_index;
	CCPoint touch_begin_point;
	CCPoint first_touch_point;
	
	float max_positionY;
	
	float touchy;
	long touchStartTime;
    float touchStartY;
    float moveSpeed;
	bool isAnimated;
	
	ScrollingObject* target_link;
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location.y -= getPositionY();
		
		bool return_value = false;
		
		if(isVisible() && view_rect.containsPoint(location))
		{
			touch_begin_point = location;
			first_touch_point = touch_begin_point;
			is_touching = true;
			
			touchy=location.y;
			
			timeval time;
			gettimeofday(&time, NULL);
			touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
			touchStartY=location.y;
			
			for(int i=0;i<getChildrenCount() && touched_index == 0;i++)
			{
				CCNode* item_parent = (CCNode*)getChildren()->objectAtIndex(i);
				int tag = item_parent->getTag();
				CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(tag);
				if(item_menu->ccTouchBegan(pTouch, pEvent))		touched_index = tag;
			}
			
			return_value = true;
		}
		
		unschedule(schedule_selector(ItemListViewer::moveAnimation));
		isAnimated=false;
		
		return return_value;
	}
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location.y -= getPositionY();
		
		if(isVisible() && view_rect.containsPoint(location))
		{
			float distance = location.y - touch_begin_point.y;
			setPositionY(getPositionY() + distance);
			touch_begin_point = location;
			
			if(fabsf(first_touch_point.y - location.y) > 5.f)
			{
				if(touched_index != 0)
				{
					CCNode* item_parent = getChildByTag(touched_index);
					CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
					item_menu->ccTouchCancelled(pTouch, pEvent);
					touched_index = 0;
				}
			}
		}
		else
		{
			timeval time;
			gettimeofday(&time, NULL);
			long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
			float _y = location.y-touchStartY;
			float _spd = _y/_time*10000;
			
			
			if(isAnimated == false && abs((int)_spd)>2){
				if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
				else			moveSpeed = ((int)_spd*10)/10.f+2;
				schedule(schedule_selector(ItemListViewer::moveAnimation));
			}
			
			is_touching = false;
			
			if(touched_index != 0)
			{
				CCNode* item_parent = getChildByTag(touched_index);
				CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
				item_menu->ccTouchCancelled(pTouch, pEvent);
				touched_index = 0;
			}
		}
	}
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location.y -= getPositionY();
		
		if(isVisible() && view_rect.containsPoint(location))
		{
			float distance = location.y - touch_begin_point.y;
			setPositionY(getPositionY() + distance);
			touch_begin_point = location;
			
			timeval time;
			gettimeofday(&time, NULL);
			long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
			float _y = location.y-touchStartY;
			float _spd = _y/_time*10000;
			
			
			if(isAnimated == false && abs((int)_spd)>2){
				if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
				else			moveSpeed = ((int)_spd*10)/10.f+2;
				schedule(schedule_selector(ItemListViewer::moveAnimation));
			}
		}
		
		if(touched_index != 0)
		{
			CCNode* item_parent = getChildByTag(touched_index);
			CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
			item_menu->ccTouchEnded(pTouch, pEvent);
			touched_index = 0;
		}
		
		is_touching = false;
	}
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(CCNode::convertToNodeSpace(touch->getLocationInView()));
		location.y -= getPositionY();
		
		if(isVisible() && view_rect.containsPoint(location))
		{
			float distance = location.y - touch_begin_point.y;
			setPositionY(getPositionY() + distance);
			touch_begin_point = location;
			
			timeval time;
			gettimeofday(&time, NULL);
			long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
			float _y = location.y-touchStartY;
			float _spd = _y/_time*10000;
			
			
			if(isAnimated == false && abs((int)_spd)>2){
				if(_spd > 0)	moveSpeed = ((int)_spd*10)/10.f-2;
				else			moveSpeed = ((int)_spd*10)/10.f+2;
				schedule(schedule_selector(ItemListViewer::moveAnimation));
			}
		}
		
		if(touched_index != 0)
		{
			CCNode* item_parent = getChildByTag(touched_index);
			CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(touched_index);
			item_menu->ccTouchCancelled(pTouch, pEvent);
			touched_index = 0;
		}
		
		is_touching = false;
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
	}
	
	
	void myInit()
	{
		view_rect = CCRectMake(197, 60, 234, 156);
		
		setPosition(CCPointZero);
		
		is_touching = false;
		touched_index = 0;
		
		CCLayer::init();
	}
	
	void moveList(float dy)
	{
		if(dy > 10)			dy = 10;
		else if(dy < -10)	dy = -10;
		
		setPositionY(getPositionY()+dy);//after_y);
	}
	
	void moveAnimation()
	{
		isAnimated=true;
		
		if(moveSpeed > 0)		moveSpeed -= 0.1;
		else if(moveSpeed < 0)	moveSpeed += 0.1;
		
		if((moveSpeed < 0.1 && moveSpeed > -0.1) || isAnimated == false)
		{
			unschedule(schedule_selector(ItemListViewer::moveAnimation));
			moveSpeed = 0;
			isAnimated = false;
		}
		else
			moveList(moveSpeed);
	}
};

#endif /* defined(__DGproto__ItemListViewer__) */
