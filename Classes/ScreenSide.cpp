//
//  ScreenSide.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 4. 23..
//
//

#include "ScreenSide.h"


ScreenSide* ScreenSide::create()
{
	ScreenSide* t_ss = new ScreenSide();
	t_ss->myInit();
	t_ss->autorelease();
	return t_ss;
}

void ScreenSide::myInit()
{
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	
	float screen_rate = screen_size.width/screen_size.height;
	float design_rate = 1.5f;
	
	if(screen_rate > design_rate) // long width
	{
		CCSprite* t_bottom = CCSprite::create("screen_side_top.png");
		t_bottom->setAnchorPoint(ccp(0.5,0.0));
		t_bottom->setRotation(90);
		t_bottom->setPosition(ccp(480,160));
		addChild(t_bottom);
		
		CCSprite* t_top = CCSprite::create("screen_side_top.png");
		t_top->setAnchorPoint(ccp(0.5,0.0));
		t_top->setRotation(-90);
		t_top->setPosition(ccp(0,160));
		addChild(t_top);
	}
	else if(screen_rate < design_rate) // long height
	{
		CCSprite* t_left = CCSprite::create("screen_side_left.png");
		t_left->setAnchorPoint(ccp(1.0,0.5));
		t_left->setRotation(-90);
		t_left->setPosition(ccp(240,0));
		addChild(t_left);
		
		CCSprite* t_right = CCSprite::create("screen_side_left.png");
		t_right->setAnchorPoint(ccp(1.0,0.5));
		t_right->setRotation(90);
		t_right->setPosition(ccp(240,320));
		addChild(t_right);
	}
}