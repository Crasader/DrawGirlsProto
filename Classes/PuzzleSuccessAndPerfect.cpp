//
//  PuzzleSuccessAndPerfect.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 5..
//
//

#include "PuzzleSuccessAndPerfect.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "StyledLabelTTF.h"
#include "CommonAnimation.h"
PuzzleSuccessAndPerfect* PuzzleSuccessAndPerfect::create(int t_touch_priority, function<void()> t_end_func, bool t_is_success)
{
	PuzzleSuccessAndPerfect* t_mup = new PuzzleSuccessAndPerfect();
	t_mup->myInit(t_touch_priority, t_end_func, t_is_success);
	t_mup->autorelease();
	return t_mup;
}

void PuzzleSuccessAndPerfect::myInit(int t_touch_priority, function<void()> t_end_func, bool t_is_success)
{
	is_success = t_is_success;
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	
	CCScale9Sprite* back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_case->setContentSize(CCSizeMake(240,220));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
	back_case->addChild(back_in);
	
	string title_string, ment_string;
	if(is_success)
	{
		title_string = myLoc->getLocalForKey(kMyLocalKey_puzzleSuccessTitle);
		ment_string = myLoc->getLocalForKey(kMyLocalKey_puzzleSuccessMent);
	}
	else
	{
		title_string = myLoc->getLocalForKey(kMyLocalKey_puzzlePerfectTitle);
		ment_string = myLoc->getLocalForKey(kMyLocalKey_puzzlePerfectMent);
	}
	
	StyledLabelTTF* title_label = StyledLabelTTF::create(title_string.c_str(), mySGD->getFont().c_str(), 21, 299, StyledAlignment::kCenterAlignment);
	title_label->setOldAnchorPoint();
	title_label->setPosition(ccp(0,back_case->getContentSize().height/2.f-25));
	m_container->addChild(title_label);
	
	StyledLabelTTF* ment_label = StyledLabelTTF::create(ment_string.c_str(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
	ment_label->setOldAnchorPoint();
	ment_label->setPosition(ccp(0,50));
	m_container->addChild(ment_label);
	
	
	if(is_success)
	{
		CCSprite* center_img = CCSprite::create("shop_coin3.png");
		center_img->setPosition(ccp(0,-30));
		m_container->addChild(center_img);
	}
	else
	{
		CCSprite* center_img = CCSprite::create("shop_ruby3.png");
		center_img->setPosition(ccp(0,-30));
		m_container->addChild(center_img);
		
		KSLabelTTF* reward_label = KSLabelTTF::create(CCString::createWithFormat("+%d", mySGD->getPuzzlePerfectRewardRuby())->getCString(), mySGD->getFont().c_str(), 20);
		reward_label->enableOuterStroke(ccBLACK, 1);
		reward_label->setPosition(ccp(center_img->getContentSize().width/2.f, center_img->getContentSize().height/2.f+3));
		center_img->addChild(reward_label);
	}
	
	CCLabelTTF* c_label = CCLabelTTF::create();
	KSLabelTTF* close_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 15);
	close_label->setPosition(ccp(0,0));
	c_label->addChild(close_label);
	
	CCScale9Sprite* close_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	CCControlButton* close_button = CCControlButton::create(c_label, close_back);
	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(PuzzleSuccessAndPerfect::closeAction), CCControlEventTouchUpInside);
	close_button->setPreferredSize(CCSizeMake(100,45));
	close_button->setPosition(ccp(0,-70));
	m_container->addChild(close_button);
	
	close_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void PuzzleSuccessAndPerfect::closeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
}