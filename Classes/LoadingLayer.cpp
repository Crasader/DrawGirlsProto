//
//  LoadingLayer.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 22..
//
//

#include "LoadingLayer.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"

LoadingLayer* LoadingLayer::create(int t_touch_priority, bool is_ingame)
{
	LoadingLayer* t_tnp = new LoadingLayer();
	t_tnp->myInit(t_touch_priority, is_ingame);
	t_tnp->autorelease();
	return t_tnp;
}

void LoadingLayer::myInit(int t_touch_priority, bool is_ingame)
{
	touch_priority = t_touch_priority;
	
	float y_value;
	if(is_ingame)
		y_value = myDSH->ui_center_y;
	else
		y_value = 160;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,y_value));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	loading_label = CCLabelTTF::create("", mySGD->getFont().c_str(), 30);
	loading_label->setOpacity(0);
	
	loading_label->setPosition(ccp(240,y_value));
	addChild(loading_label);
	
	loading_text = "Loading...";
	
	setTouchEnabled(true);
}

void LoadingLayer::setPositions(CCPoint t_position)
{
	gray->setPosition(t_position);
	loading_label->setPosition(t_position);
}

void LoadingLayer::onEnter()
{
	CCLayer::onEnter();
	CCFadeTo* gray_fade = CCFadeTo::create(0.3f, 255);
	gray->runAction(gray_fade);
	
	CCFadeTo* loading_fade = CCFadeTo::create(0.3f, 255);
	loading_label->runAction(loading_fade);
	startLoading();
}

void LoadingLayer::startLoading()
{
	animation_frame = 0;
	schedule(schedule_selector(LoadingLayer::loadingAnimation), 0.1f);
}

void LoadingLayer::loadingAnimation()
{
	animation_frame++;
	
	if(animation_frame >= 1 && animation_frame <= 10)
		loading_label->setString(loading_text.substr(0, animation_frame).c_str());
	else if(animation_frame <= 15)
		loading_label->setVisible(animation_frame%2 == 0);
	else
	{
		loading_label->setString("");
		animation_frame = 0;
	}
}

bool LoadingLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	CCLOG("touch swallow : LoadingLayer");
	return true;
}

void LoadingLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void LoadingLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void LoadingLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void LoadingLayer::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, touch_priority, true);
}