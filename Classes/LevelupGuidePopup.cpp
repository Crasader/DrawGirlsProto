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
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(320,240));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	KSLabelTTF* ment1_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGuideMent1), mySGD->getFont().c_str(), 15);
	ment1_label->setPosition(ccp(0,60));
	m_container->addChild(ment1_label);
	
	KSLabelTTF* ment2_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGuideMent2), mySGD->getFont().c_str(), 15);
	ment2_label->setPosition(ccp(0,40));
	m_container->addChild(ment2_label);
	
	KSLabelTTF* ment3_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGuideMent3), mySGD->getFont().c_str(), 15);
	ment3_label->setPosition(ccp(0,10));
	m_container->addChild(ment3_label);
	
	KSLabelTTF* ment4_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGuideMent4), mySGD->getFont().c_str(), 15);
	ment4_label->setPosition(ccp(0,-10));
	m_container->addChild(ment4_label);
	
	
	KSLabelTTF* next_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_next), mySGD->getFont().c_str(), 13);
	CCScale9Sprite* next_back = CCScale9Sprite::create("subpop_gray.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	next_button = CCControlButton::create(next_label, next_back);
	next_button->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelupGuidePopup::nextAction), CCControlEventTouchUpInside);
	next_button->setPreferredSize(CCSizeMake(90,60));
	next_button->setPosition(ccp(-55,-60));
	m_container->addChild(next_button);
	
	next_button->setTouchPriority(touch_priority);
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	KSLabelTTF* rightnow_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGo), mySGD->getFont().c_str(), 13);
	r_label->addChild(rightnow_label);
	
	CCScale9Sprite* rightnow_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	rightnow_button = CCControlButton::create(r_label, rightnow_back);
	rightnow_button->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelupGuidePopup::rightnowAction), CCControlEventTouchUpInside);
	rightnow_button->setPreferredSize(CCSizeMake(90,60));
	rightnow_button->setPosition(ccp(55,-60));
	m_container->addChild(rightnow_button);
	
	rightnow_button->setTouchPriority(touch_priority);
	
	
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

void LevelupGuidePopup::nextAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}

void LevelupGuidePopup::rightnowAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); rightnow_func(); removeFromParent();}));
}