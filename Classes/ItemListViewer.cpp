// ItemListViewer.cpp
//

#include "ItemListViewer.h"
#define LZZ_INLINE inline
using namespace std;
ItemListViewer * ItemListViewer::create ()
        {
		ItemListViewer* t_ilv = new ItemListViewer();
		t_ilv->myInit();
		t_ilv->autorelease();
		return t_ilv;
	}
void ItemListViewer::visit ()
        {
		glEnable(GL_SCISSOR_TEST);
		
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		CCSize rSize = myDSH->getDesignResolutionSize(); // getSize
		
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
		
		float x = (view_rect.origin.x+getParent()->getPositionX())*wScale + xMargine;
		float y = (view_rect.origin.y+getParent()->getPositionY())*hScale + yMargine;
		float w = view_rect.size.width*wScale;
		float h = view_rect.size.height*hScale;
		
		glScissor(x,y,w,h);
		
		CCLayer::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
void ItemListViewer::setPositionY (float t_y)
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
void ItemListViewer::setPercentage (float t_p)
        {
		setPosition(ccp(getPositionX(),max_positionY*t_p));
	}
void ItemListViewer::setMaxPositionY ()
        {
		int cnt = getChildrenCount();
		
		float t_y = cnt*46 + 5;
		t_y -= view_rect.size.height;
		
		if(t_y > 0)
			max_positionY = t_y;
		else
			max_positionY = 0;
	}
void ItemListViewer::setScroll (ScrollingObject * t_link)
        {
		target_link = t_link;
		setPositionY(getPositionY());
	}
bool ItemListViewer::ccTouchBegan (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)
        {
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		location = myDSH->wideWidthFixTouch(location);
		
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
void ItemListViewer::ccTouchMoved (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)
        {
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		location = myDSH->wideWidthFixTouch(location);
		
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
void ItemListViewer::ccTouchEnded (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)
        {
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		location = myDSH->wideWidthFixTouch(location);
		
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
void ItemListViewer::ccTouchCancelled (cocos2d::CCTouch * pTouch, cocos2d::CCEvent * pEvent)
        {
		if(!is_touching)		return;
		
		CCTouch* touch = pTouch;
		CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
		location = myDSH->wideWidthFixTouch(location);
		
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
void ItemListViewer::registerWithTouchDispatcher ()
        {
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
	}
void ItemListViewer::myInit ()
        {
		view_rect = CCRectMake(183, 76, 246, 146);
		
//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, view_rect.size.width, view_rect.size.height));
//		temp_back->setOpacity(100);
//		temp_back->setPosition(ccp(view_rect.origin.x+view_rect.size.width/2.f, view_rect.origin.y+view_rect.size.height/2.f));
//		addChild(temp_back);
		
		
		setPosition(CCPointZero);
		
		is_touching = false;
		touched_index = 0;
		
		CCLayer::init();
	}
void ItemListViewer::moveList (float dy)
        {
		if(dy > 10)			dy = 10;
		else if(dy < -10)	dy = -10;
		
		setPositionY(getPositionY()+dy);//after_y);
	}
void ItemListViewer::moveAnimation ()
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
#undef LZZ_INLINE
