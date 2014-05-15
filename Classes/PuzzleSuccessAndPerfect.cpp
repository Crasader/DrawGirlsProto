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
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	CCScale9Sprite* back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(320,250));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
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
	title_label->setPosition(ccp(0,80));
	m_container->addChild(title_label);
	
	StyledLabelTTF* ment_label = StyledLabelTTF::create(ment_string.c_str(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
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
	
	CCScale9Sprite* close_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	CCControlButton* close_button = CCControlButton::create(c_label, close_back);
	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(PuzzleSuccessAndPerfect::closeAction), CCControlEventTouchUpInside);
	close_button->setPreferredSize(CCSizeMake(80,50));
	close_button->setPosition(ccp(0,-70));
	m_container->addChild(close_button);
	
	close_button->setTouchPriority(touch_priority);
	
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t)
										 {
											 KS::setOpacity(m_container, 255);
											 is_menu_enable = true;
										 }));
}

void PuzzleSuccessAndPerfect::closeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}