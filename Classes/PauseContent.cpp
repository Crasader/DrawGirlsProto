//
//  PauseContent.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 2. 3..
//
//

#include "PauseContent.h"

#include "CCMenuLambda.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "CommonButton.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
#include "ASPopupView.h"

PauseContent* PauseContent::create(int t_touch_priority, function<void(void)> t_resume, function<void(void)> t_gohome, function<void(void)> t_replay)
{
	PauseContent* t_ctc = new PauseContent();
	t_ctc->myInit(t_touch_priority, t_resume, t_gohome, t_replay);
	t_ctc->autorelease();
	return t_ctc;
}

void PauseContent::startShow()
{
	CommonAnimation::openPopup(this, this, nullptr, [=](){
		
	}, [=](){
		endShow();
	});
	setPosition(ccp(240, myDSH->ui_center_y));
}

void PauseContent::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kPauseContentMenuTag_resume)
	{
		startHide();
	}
	else if(tag == kPauseContentMenuTag_gohome)
	{
		gohome_selector();
	}
	else if(tag == kPauseContentMenuTag_replay)
	{
		replay_selector();
	}
}

void PauseContent::resumeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	is_menu_enable = false;
	
	startHide();
}
void PauseContent::gohomeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;

	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	if(myDSH->getIntegerForKey(kDSH_Key_showedScenario) == 5)
	{
		myDSH->setIntegerForKey(kDSH_Key_showedScenario, 6);
	}
	
	if(mySGD->is_endless_mode && myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
	{
		addChild(ASPopupView::getCommonNoti(touch_priority-10, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_notGohomeEndlessTutorialMent), [=](){is_menu_enable = true;}, ccp(0.1f,0.1f)), 9999);
	}
	else
		gohome_selector();
}
void PauseContent::replayAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;

	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	if(myDSH->getIntegerForKey(kDSH_Key_showedScenario) == 5)
	{
		myDSH->setIntegerForKey(kDSH_Key_showedScenario, 6);
	}
	
	if(mySGD->is_endless_mode)
	{
		addChild(ASPopupView::getCommonNoti(touch_priority-10, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_notReplayEndlessMent), [=](){is_menu_enable = true;}, ccp(0.1f,0.1f)), 9999);
	}
	else
		replay_selector();
}

void PauseContent::startHide()
{
	addChild(KSTimer::create(0.2f, [=](){
		endHide();
	}));
	
	CommonAnimation::closePopup(this, this, nullptr, [=](){
		
	}, [=](){
//		end_func(); removeFromParent();
	});
}

void PauseContent::myInit(int t_touch_priority, function<void(void)> t_resume, function<void(void)> t_gohome, function<void(void)> t_replay)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	resume_selector = t_resume;
	gohome_selector = t_gohome;
	replay_selector = t_replay;
	
	CCSprite* case_back = CCSprite::create("popup_small_back.png");
	case_back->setPosition(CCPointZero);
	addChild(case_back);
	
	
	CCLabelTTF* title_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_pause), mySGD->getFont().c_str(), 12);
	title_label->setPosition(ccp(-85, 60));
	addChild(title_label);
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority-1);
	close_button->setPosition(ccpFromSize(case_back->getContentSize()) + ccp(-23, -20));
	close_button->setFunction([=](CCObject* sender){
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		startHide();
	});
	case_back->addChild(close_button);
	
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12,12,2,2));
	content_back->setPosition(ccp(-50,-14));
	addChild(content_back);
	
	content_back->setContentSize(CCSizeMake(151,113));
	
	
	CCLabelTTF* t1_label = CCLabelTTF::create();
	
	KSLabelTTF* resume_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_continue), mySGD->getFont().c_str(), 15);
	resume_label->disableOuterStroke();
	resume_label->setPosition(ccp(0,0));
	t1_label->addChild(resume_label);
	
	CCScale9Sprite* resume_back = CCScale9Sprite::create("subbutton_purple3.png");
	
	CCControlButton* resume_button = CCControlButton::create(t1_label, resume_back);
	resume_button->addTargetWithActionForControlEvents(this, cccontrol_selector(PauseContent::resumeAction), CCControlEventTouchUpInside);
	resume_button->setPreferredSize(CCSizeMake(100,39));
	resume_button->setPosition(ccp(81,34));
	addChild(resume_button);
	
	resume_button->setTouchPriority(touch_priority-1);
	

	
	CCLabelTTF* t2_label = CCLabelTTF::create();
	
	KSLabelTTF* gohome_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ingameOut), mySGD->getFont().c_str(), 15);
	gohome_label->disableOuterStroke();
	gohome_label->setPosition(ccp(0,0));
	t2_label->addChild(gohome_label);
	
	CCScale9Sprite* gohome_back = CCScale9Sprite::create("subbutton_purple3.png");
	
	CCControlButton* gohome_button = CCControlButton::create(t2_label, gohome_back);
	gohome_button->addTargetWithActionForControlEvents(this, cccontrol_selector(PauseContent::gohomeAction), CCControlEventTouchUpInside);
	gohome_button->setPreferredSize(CCSizeMake(100,39));
	gohome_button->setPosition(ccp(81,-8));
	addChild(gohome_button);
	
	gohome_button->setTouchPriority(touch_priority-1);
	
//	gohome_button->setEnabled(!mySGD->is_hell_mode);
	
	
	
	CCLabelTTF* t3_label = CCLabelTTF::create();
	
	KSLabelTTF* replay_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ingameReplay), mySGD->getFont().c_str(), 15);
	replay_label->disableOuterStroke();
	replay_label->setPosition(ccp(0,0));
	t3_label->addChild(replay_label);
	
	CCScale9Sprite* replay_back = CCScale9Sprite::create("subbutton_purple3.png");
	
	CCControlButton* replay_button = CCControlButton::create(t3_label, replay_back);
	replay_button->addTargetWithActionForControlEvents(this, cccontrol_selector(PauseContent::replayAction), CCControlEventTouchUpInside);
	replay_button->setPreferredSize(CCSizeMake(100,39));
	replay_button->setPosition(ccp(81,-50));
	addChild(replay_button);
	
	replay_button->setTouchPriority(touch_priority-1);
	
//	replay_button->setEnabled(!mySGD->is_hell_mode);
	

	
	CCNode* safety_node = CCNode::create();
	safety_node->setPosition(ccp(-60,25));
	addChild(safety_node);
	
	
	KSLabelTTF* safety_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_publicPlaceMode), mySGD->getFont().c_str(), 11);
	safety_label->disableOuterStroke();
	safety_label->setPosition(ccp(-41,-73));
	safety_node->addChild(safety_label);
	
	CCSprite* line_img = CCSprite::create("stop_scroll.png");
	line_img->setPosition(ccp(30,-73));
	safety_node->addChild(line_img);
	
	CCMenuLambda* on_off_menu = CCMenuLambda::create();
	on_off_menu->setPosition(ccp(30,-73));
	safety_node->addChild(on_off_menu);
	on_off_menu->setTouchPriority(touch_priority-1);
	
	CCSprite* n_on = CCSprite::create("subbutton_purple2.png");
	KSLabelTTF* n_on_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOn), mySGD->getFont().c_str(), 12);
	n_on_label->disableOuterStroke();
	n_on_label->setPosition(ccp(n_on->getContentSize().width/2.f, n_on->getContentSize().height/2.f));
	n_on->addChild(n_on_label);
	
	CCSprite* s_on = CCSprite::create("subbutton_purple2.png");
	s_on->setColor(ccGRAY);
	KSLabelTTF* s_on_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOn), mySGD->getFont().c_str(), 12);
	s_on_label->disableOuterStroke();
	s_on_label->setPosition(ccp(s_on->getContentSize().width/2.f, s_on->getContentSize().height/2.f));
	s_on->addChild(s_on_label);
	
	on_item = CCMenuItemSpriteLambda::create(n_on, s_on, [=](CCObject* sender){
		this->on_item->setVisible(false);
		this->off_item->setVisible(true);
		
		bool t_safety = !myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
		myDSH->setBoolForKey(kDSH_Key_isSafetyMode, t_safety);
		mySGD->is_safety_mode = t_safety;
	});
	on_item->setPosition(ccp(-16,0));
	
	on_off_menu->addChild(on_item);
	
	
	CCSprite* n_off = CCSprite::create("subbutton_purple2.png");
	KSLabelTTF* n_off_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOff), mySGD->getFont().c_str(), 12);
	n_off_label->disableOuterStroke();
	n_off_label->setPosition(ccp(n_off->getContentSize().width/2.f, n_off->getContentSize().height/2.f));
	n_off->addChild(n_off_label);
	
	CCSprite* s_off = CCSprite::create("subbutton_purple2.png");
	s_off->setColor(ccGRAY);
	KSLabelTTF* s_off_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOff), mySGD->getFont().c_str(), 12);
	s_off_label->disableOuterStroke();
	s_off_label->setPosition(ccp(s_off->getContentSize().width/2.f, s_off->getContentSize().height/2.f));
	s_off->addChild(s_off_label);
	
	off_item = CCMenuItemSpriteLambda::create(n_off, s_off, [=](CCObject* sender){
		this->on_item->setVisible(true);
		this->off_item->setVisible(false);
		
		bool t_safety = !myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
		myDSH->setBoolForKey(kDSH_Key_isSafetyMode, t_safety);
		mySGD->is_safety_mode = t_safety;
	});
	off_item->setPosition(ccp(16,0));
	
	on_off_menu->addChild(off_item);
	
	
	
	bool is_safety = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
	if(is_safety)
		off_item->setVisible(false);
	else
		on_item->setVisible(false);
	
	
	
	CCNode* sound_node = CCNode::create();
	sound_node->setPosition(ccp(-60,95));
	addChild(sound_node);
	
	
	KSLabelTTF* sound_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_bgm), mySGD->getFont().c_str(), 11.5f);
	sound_label->disableOuterStroke();
	sound_label->setPosition(ccp(-41,-73));
	sound_node->addChild(sound_label);
	
	CCSprite* line2_img = CCSprite::create("stop_scroll.png");
	line2_img->setPosition(ccp(30,-73));
	sound_node->addChild(line2_img);
	
	CCMenuLambda* on_off_menu2 = CCMenuLambda::create();
	on_off_menu2->setPosition(ccp(30,-73));
	sound_node->addChild(on_off_menu2);
	on_off_menu2->setTouchPriority(touch_priority-1);
	
	CCSprite* n_on2 = CCSprite::create("subbutton_purple2.png");
	KSLabelTTF* n_on_label2 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOn), mySGD->getFont().c_str(), 12);
	n_on_label2->disableOuterStroke();
	n_on_label2->setPosition(ccp(n_on2->getContentSize().width/2.f, n_on2->getContentSize().height/2.f));
	n_on2->addChild(n_on_label2);
	
	CCSprite* s_on2 = CCSprite::create("subbutton_purple2.png");
	s_on2->setColor(ccGRAY);
	KSLabelTTF* s_on_label2 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOn), mySGD->getFont().c_str(), 12);
	s_on_label2->disableOuterStroke();
	s_on_label2->setPosition(ccp(s_on2->getContentSize().width/2.f, s_on2->getContentSize().height/2.f));
	s_on2->addChild(s_on_label2);
	
	on_item2 = CCMenuItemSpriteLambda::create(n_on2, s_on2, [=](CCObject* sender){
		this->on_item2->setVisible(false);
		this->off_item2->setVisible(true);
		
		bool t_sound = !myDSH->getBoolForKey(kDSH_Key_bgmOff);
		myDSH->setBoolForKey(kDSH_Key_bgmOff, t_sound);
		AudioEngine::sharedInstance()->setSoundOnOff(!myDSH->getBoolForKey(kDSH_Key_bgmOff));
	});
	on_item2->setPosition(ccp(-16,0));
	
	on_off_menu2->addChild(on_item2);
	
	
	CCSprite* n_off2 = CCSprite::create("subbutton_purple2.png");
	KSLabelTTF* n_off_label2 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOff), mySGD->getFont().c_str(), 12);
	n_off_label2->disableOuterStroke();
	n_off_label2->setPosition(ccp(n_off2->getContentSize().width/2.f, n_off2->getContentSize().height/2.f));
	n_off2->addChild(n_off_label2);
	
	CCSprite* s_off2 = CCSprite::create("subbutton_purple2.png");
	s_off2->setColor(ccGRAY);
	KSLabelTTF* s_off_label2 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOff), mySGD->getFont().c_str(), 12);
	s_off_label2->disableOuterStroke();
	s_off_label2->setPosition(ccp(s_off2->getContentSize().width/2.f, s_off2->getContentSize().height/2.f));
	s_off2->addChild(s_off_label2);
	
	off_item2 = CCMenuItemSpriteLambda::create(n_off2, s_off2, [=](CCObject* sender){
		this->on_item2->setVisible(true);
		this->off_item2->setVisible(false);
		
		bool t_sound = !myDSH->getBoolForKey(kDSH_Key_bgmOff);
		myDSH->setBoolForKey(kDSH_Key_bgmOff, t_sound);
		AudioEngine::sharedInstance()->setSoundOnOff(!myDSH->getBoolForKey(kDSH_Key_bgmOff));
	});
	off_item2->setPosition(ccp(16,0));
	
	on_off_menu2->addChild(off_item2);
	
	
	
	bool is_sound = myDSH->getBoolForKey(kDSH_Key_bgmOff);
	if(is_sound)
		on_item2->setVisible(false);
	else
		off_item2->setVisible(false);
	
	
	CCNode* effect_node = CCNode::create();
	effect_node->setPosition(ccp(-60,60));
	addChild(effect_node);
	
	
	KSLabelTTF* effect_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_effect), mySGD->getFont().c_str(), 11.5f);
	effect_label->disableOuterStroke();
	effect_label->setPosition(ccp(-41,-73));
	effect_node->addChild(effect_label);
	
	CCSprite* line3_img = CCSprite::create("stop_scroll.png");
	line3_img->setPosition(ccp(30,-73));
	effect_node->addChild(line3_img);
	
	CCMenuLambda* on_off_menu3 = CCMenuLambda::create();
	on_off_menu3->setPosition(ccp(30,-73));
	effect_node->addChild(on_off_menu3);
	on_off_menu3->setTouchPriority(touch_priority-1);
	
	CCSprite* n_on3 = CCSprite::create("subbutton_purple2.png");
	KSLabelTTF* n_on_label3 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOn), mySGD->getFont().c_str(), 12);
	n_on_label3->disableOuterStroke();
	n_on_label3->setPosition(ccp(n_on3->getContentSize().width/2.f, n_on3->getContentSize().height/2.f));
	n_on3->addChild(n_on_label3);
	
	CCSprite* s_on3 = CCSprite::create("subbutton_purple2.png");
	s_on3->setColor(ccGRAY);
	KSLabelTTF* s_on_label3 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOn), mySGD->getFont().c_str(), 12);
	s_on_label3->disableOuterStroke();
	s_on_label3->setPosition(ccp(s_on3->getContentSize().width/2.f, s_on3->getContentSize().height/2.f));
	s_on3->addChild(s_on_label3);
	
	on_item3 = CCMenuItemSpriteLambda::create(n_on3, s_on3, [=](CCObject* sender){
		this->on_item3->setVisible(false);
		this->off_item3->setVisible(true);
		
		bool t_effect = !myDSH->getBoolForKey(kDSH_Key_effectOff);
		myDSH->setBoolForKey(kDSH_Key_effectOff, t_effect);
		AudioEngine::sharedInstance()->setEffectOnOff(!myDSH->getBoolForKey(kDSH_Key_effectOff));
	});
	on_item3->setPosition(ccp(-16,0));
	
	on_off_menu3->addChild(on_item3);
	
	
	CCSprite* n_off3 = CCSprite::create("subbutton_purple2.png");
	KSLabelTTF* n_off_label3 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOff), mySGD->getFont().c_str(), 12);
	n_off_label3->disableOuterStroke();
	n_off_label3->setPosition(ccp(n_off3->getContentSize().width/2.f, n_off3->getContentSize().height/2.f));
	n_off3->addChild(n_off_label3);
	
	CCSprite* s_off3 = CCSprite::create("subbutton_purple2.png");
	s_off3->setColor(ccGRAY);
	KSLabelTTF* s_off_label3 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_lightOff), mySGD->getFont().c_str(), 12);
	s_off_label3->disableOuterStroke();
	s_off_label3->setPosition(ccp(s_off3->getContentSize().width/2.f, s_off3->getContentSize().height/2.f));
	s_off3->addChild(s_off_label3);
	
	off_item3 = CCMenuItemSpriteLambda::create(n_off3, s_off3, [=](CCObject* sender){
		this->on_item3->setVisible(true);
		this->off_item3->setVisible(false);
		
		bool t_effect = !myDSH->getBoolForKey(kDSH_Key_effectOff);
		myDSH->setBoolForKey(kDSH_Key_effectOff, t_effect);
		AudioEngine::sharedInstance()->setEffectOnOff(!myDSH->getBoolForKey(kDSH_Key_effectOff));
	});
	off_item3->setPosition(ccp(16,0));
	
	on_off_menu3->addChild(off_item3);
	
	
	
	bool is_effect = myDSH->getBoolForKey(kDSH_Key_effectOff);
	if(is_effect)
		on_item3->setVisible(false);
	else
		off_item3->setVisible(false);
	
//	CommonButton* on_off = CommonButton::create(on_off_str, 14, CCSizeMake(60, 45), CommonButtonOrange, touch_priority-1);
//	on_off->setPosition(ccp(40,-70));
//	on_off->setFunction([=](CCObject* sender)
//						{
//							bool t_safety = !myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
//							myDSH->setBoolForKey(kDSH_Key_isSafetyMode, t_safety);
//							mySGD->is_safety_mode = t_safety;
//							if(t_safety)
//								on_off->setTitle("켜짐");
//							else
//								on_off->setTitle("꺼짐");
//						});
//	addChild(on_off);
	
//	replay_menu->setEnabled(true);
}