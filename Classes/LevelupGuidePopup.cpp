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
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(260,210));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_missileUpgrade), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(17, back_case->getContentSize().height-25));
	back_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
								  
								  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
									  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
								  
								  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
																				{
																					gray->setOpacity(t);
																					KS::setOpacity(m_container, t);
																				}, [=](int t)
																				{
																					gray->setOpacity(0);
																					KS::setOpacity(m_container, 0);
																					end_func();
																					removeFromParent();
																				}));
							  });
	back_case->addChild(close_button);
	
	KSLabelTTF* ment1_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGuideMent1), mySGD->getFont().c_str(), 13);
	ment1_label->setColor(ccc3(255, 170, 20));
	ment1_label->setAnchorPoint(ccp(0,0.5f));
	ment1_label->setPosition(ccp(15,back_in->getContentSize().height-20));
	back_in->addChild(ment1_label);
	
	KSLabelTTF* ment2_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGuideMent2), mySGD->getFont().c_str(), 13);
	ment2_label->setColor(ccc3(255, 170, 20));
	ment2_label->setAnchorPoint(ccp(0,0.5f));
	ment2_label->setPosition(ccp(15,back_in->getContentSize().height-40));
	back_in->addChild(ment2_label);
	
	KSLabelTTF* ment3_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGuideMent3), mySGD->getFont().c_str(), 13);
	ment3_label->setAnchorPoint(ccp(0,0.5f));
	ment3_label->setPosition(ccp(15,back_in->getContentSize().height-70));
	back_in->addChild(ment3_label);
	
	KSLabelTTF* ment4_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGuideMent4), mySGD->getFont().c_str(), 13);
	ment4_label->setAnchorPoint(ccp(0,0.5f));
	ment4_label->setPosition(ccp(15,back_in->getContentSize().height-90));
	back_in->addChild(ment4_label);
	
	
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	KSLabelTTF* rightnow_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_levelupGo), mySGD->getFont().c_str(), 13);
	r_label->addChild(rightnow_label);
	
	CCScale9Sprite* rightnow_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	rightnow_button = CCControlButton::create(r_label, rightnow_back);
	rightnow_button->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelupGuidePopup::rightnowAction), CCControlEventTouchUpInside);
	rightnow_button->setPreferredSize(CCSizeMake(100,45));
	rightnow_button->setPosition(ccp(0,-63));
	m_container->addChild(rightnow_button);
	
	rightnow_button->setTouchPriority(touch_priority);
	
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
	{
		gray->setOpacity(t);
		KS::setOpacity(m_container, t);
	}, [=](int t)
										 {
											 gray->setOpacity(255);
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