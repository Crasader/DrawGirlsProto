//
//  CardListViewer.cpp
//  DGproto
//
//  Created by 사원3 on 13. 10. 2..
//
//

#include "CardListViewer.h"


void CardListViewer::visit()
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
	
	float x = view_rect.origin.x*wScale + xMargine;
	float y = view_rect.origin.y*hScale + yMargine;
	float w = view_rect.size.width*wScale;
	float h = view_rect.size.height*hScale;
	
	glScissor(x,y,w,h);
	
	CCLayer::visit();
	
	glDisable(GL_SCISSOR_TEST);
}

void CardListViewer::setPositionY(float t_y)
{
	if(t_y < 0)
	{
		t_y = 0;
		unschedule(schedule_selector(CardListViewer::moveAnimation));
	}
	else if(t_y > max_positionY)
	{
		t_y = max_positionY;
		unschedule(schedule_selector(CardListViewer::moveAnimation));
	}
	
	CCLayer::setPositionY(t_y);
	
	for(int i=0;i<getChildrenCount();i++)
	{
		CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
		int tag = t_child->getTag();
		if(tag == kCSS_MT_selectedCheck || tag == kCSS_MT_checkMark)		continue;
		((CLV_Node*)t_child)->viewCheck();
	}
	
	if(target_link)
		target_link->setPercentage(t_y / max_positionY);
}

void CardListViewer::setMaxPositionY()
{
	int cnt = getChildrenCount()/3;
	
	float t_y = cnt*82 + 5;
	t_y -= view_rect.size.height;
	
	if(t_y > 0)
		max_positionY = t_y;
	else
		max_positionY = 0;
}

void CardListViewer::setScroll(ScrollingObject* t_link)
{
	target_link = t_link;
	setPositionY(getPositionY());
}

bool CardListViewer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
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
			if(tag == kCSS_MT_selectedCheck || tag == kCSS_MT_checkMark || tag == kCSS_MT_noCardBase)	continue; // kCSS_selectedCheck || kCSS_checkMark
			if(!((CLV_Node*)item_parent)->isSetted())		continue;
			CCMenu* item_menu = (CCMenu*)item_parent->getChildByTag(tag);
			if(item_menu->ccTouchBegan(pTouch, pEvent))
				touched_index = tag;
		}
		
		return_value = true;
	}
	
	unschedule(schedule_selector(CardListViewer::moveAnimation));
	isAnimated=false;
	
	return return_value;
}

void CardListViewer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
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
			schedule(schedule_selector(CardListViewer::moveAnimation));
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

void CardListViewer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
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
			schedule(schedule_selector(CardListViewer::moveAnimation));
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

void CardListViewer::ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if(!is_touching)
	{
		return;
	}
	
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
			schedule(schedule_selector(CardListViewer::moveAnimation));
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

void CardListViewer::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority-1, false);
}

void CardListViewer::moveList(float dy)
{
	if(dy > 10)			dy = 10;
	else if(dy < -10)	dy = -10;
	
	setPositionY(getPositionY()+dy);//after_y);
}

void CardListViewer::moveAnimation()
{
	isAnimated=true;
	
	if(moveSpeed > 0)		moveSpeed -= 0.1;
	else if(moveSpeed < 0)	moveSpeed += 0.1;
	
	if((moveSpeed < 0.1 && moveSpeed > -0.1) || isAnimated == false)
	{
		unschedule(schedule_selector(CardListViewer::moveAnimation));
		moveSpeed = 0;
		isAnimated = false;
	}
	else
		moveList(moveSpeed);
}

void CardListViewer::myInit()
{
	view_rect = CCRectMake(210, 25, 215, 236);//CCRectMake(237, 22, 189, 203)
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 215, 236));
//	temp_back->setOpacity(150);
//	temp_back->setPosition(ccp(210+107.5, 25+118));
//	addChild(temp_back);
	
	setPosition(CCPointZero);
	
	is_touching = false;
	touched_index = 0;
	
	CCLayer::init();
}