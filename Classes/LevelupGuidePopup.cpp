//
//  LevelupGuidePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 1..
//
//

#include "LevelupGuidePopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "ASPopupView.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "PuzzleScene.h"
#include "FormSetter.h"
#include "StarGoldData.h"
#include "CommonAnimation.h"
#include "StyledLabelTTF.h"

LevelupGuidePopup* LevelupGuidePopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_rightnow_func)
{
	LevelupGuidePopup* t_mup = new LevelupGuidePopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_rightnow_func);
	t_mup->autorelease();
	return t_mup;
}

void LevelupGuidePopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_rightnow_func)
{
	mySGD->showPurchasePopup(kPurchaseGuideType_levelupGuide);
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	rightnow_func = t_rightnow_func;
	
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
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-45, back_case->getContentSize().height-120));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f+8));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missileUpgrade), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
								  
									CommonAnimation::closePopup(this, m_container, gray, [=](){
										
									}, [=](){
										end_func(); removeFromParent();
									});
							  });
	back_case->addChild(close_button);
	
	StyledLabelTTF* ment1_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGuideMent1), mySGD->getFont().c_str(), 12.f, 999, StyledAlignment::kCenterAlignment);// KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGuideMent1), mySGD->getFont().c_str(), 12.f);
//	ment1_label->setColor(ccc3(255, 170, 20));
//	ment1_label->disableOuterStroke();
	ment1_label->setAnchorPoint(ccp(0.5f,0.5f));
	ment1_label->setPosition(ccp(back_in->getContentSize().width/2.f,back_in->getContentSize().height-13));
	back_in->addChild(ment1_label);
	
	StyledLabelTTF* ment2_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGuideMent2), mySGD->getFont().c_str(), 12.f, 999, StyledAlignment::kCenterAlignment);//KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGuideMent2), mySGD->getFont().c_str(), 12.f);
//	ment2_label->setColor(ccc3(255, 170, 20));
//	ment2_label->disableOuterStroke();
	ment2_label->setAnchorPoint(ccp(0.5f,0.5f));
	ment2_label->setPosition(ccp(back_in->getContentSize().width/2.f,back_in->getContentSize().height-28));
	back_in->addChild(ment2_label);
	
	StyledLabelTTF* ment3_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGuideMent3), mySGD->getFont().c_str(), 12.f, 999, StyledAlignment::kCenterAlignment);//KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGuideMent3), mySGD->getFont().c_str(), 12.f);
//	ment3_label->disableOuterStroke();
	ment3_label->setAnchorPoint(ccp(0.5f,0.5f));
	ment3_label->setPosition(ccp(back_in->getContentSize().width/2.f,back_in->getContentSize().height-43));
	back_in->addChild(ment3_label);
	
	StyledLabelTTF* ment4_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGuideMent4), mySGD->getFont().c_str(), 12.f, 999, StyledAlignment::kCenterAlignment);//KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGuideMent4), mySGD->getFont().c_str(), 12.f);
//	ment4_label->disableOuterStroke();
	ment4_label->setAnchorPoint(ccp(0.5f,0.5f));
	ment4_label->setPosition(ccp(back_in->getContentSize().width/2.f,back_in->getContentSize().height-58));
	back_in->addChild(ment4_label);
	
//	ment3_label->setPositionX(back_in->getContentSize().width/2.f-ment4_label->getContentSize().width/2.f);
//	ment4_label->setPositionX(back_in->getContentSize().width/2.f+ment3_label->getContentSize().width/2.f);
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	KSLabelTTF* rightnow_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_levelupGo), mySGD->getFont().c_str(), 13);
	rightnow_label->disableOuterStroke();
	r_label->addChild(rightnow_label);
	
	CCScale9Sprite* rightnow_back = CCScale9Sprite::create("achievement_button_success.png");
	
	rightnow_button = CCControlButton::create(r_label, rightnow_back);
	rightnow_button->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelupGuidePopup::rightnowAction), CCControlEventTouchUpInside);
	rightnow_button->setPreferredSize(CCSizeMake(101,44));
	rightnow_button->setPosition(ccp(0,-51));
	m_container->addChild(rightnow_button);
	
	rightnow_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void LevelupGuidePopup::nextAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	CommonAnimation::closePopup(this, m_container, nullptr, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
}

void LevelupGuidePopup::rightnowAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		rightnow_func(); removeFromParent();
	});
}