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
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	is_menu_enable = false;
	
	gohome_selector();
}
void PauseContent::replayAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	is_menu_enable = false;
	
	replay_selector();
}

void PauseContent::startHide()
{
	addChild(KSTimer::create(0.2f, [=](){
		endHide();
	}));
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(this, t);}, [=](int t){KS::setOpacity(this, 0);}));
}

void PauseContent::myInit(int t_touch_priority, function<void(void)> t_resume, function<void(void)> t_gohome, function<void(void)> t_replay)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	resume_selector = t_resume;
	gohome_selector = t_gohome;
	replay_selector = t_replay;
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	case_back->setPosition(CCPointZero);
	addChild(case_back);
	
	case_back->setContentSize(CCSizeMake(300, 186));
	
	
	CCLabelTTF* title_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_pause), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccORANGE);
	title_label->setPosition(ccp(-105, 70));
	addChild(title_label);
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority-1);
	close_button->setPosition(ccp(125, 70));
	close_button->setFunction([=](CCObject* sender){
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		startHide();
	});
	addChild(close_button);
	
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24,24,2,2));
	content_back->setPosition(ccp(0,-13));
	addChild(content_back);
	
	content_back->setContentSize(CCSizeMake(280,140));
	
	
	CCLabelTTF* t1_label = CCLabelTTF::create();
	
	KSLabelTTF* resume_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_continue), mySGD->getFont().c_str(), 18);
	resume_label->enableOuterStroke(ccBLACK, 1.f);
	resume_label->setPosition(ccp(0,0));
	t1_label->addChild(resume_label);
	
	CCScale9Sprite* resume_back = CCScale9Sprite::create("rank_normal.png", CCRectMake(0,0,40,40), CCRectMake(19, 19, 2, 2));
	
	CCControlButton* resume_button = CCControlButton::create(t1_label, resume_back);
	resume_button->addTargetWithActionForControlEvents(this, cccontrol_selector(PauseContent::resumeAction), CCControlEventTouchUpInside);
	resume_button->setPreferredSize(CCSizeMake(120,45));
	resume_button->setPosition(ccp(75,30));
	addChild(resume_button);
	
	resume_button->setTouchPriority(touch_priority-1);
	
	
//	CommonButton* resume_menu = CommonButton::create("계속하기", 14, CCSizeMake(150,45), CommonButtonOrange, touch_priority-1);
//	resume_menu->setPosition(ccp(0,53));
//	resume_menu->setFunction([=](CCObject* sender)
//							 {
//								 CCNode* t_node = CCNode::create();
//								 t_node->setTag(kPauseContentMenuTag_resume);
//								 menuAction(t_node);
//							 });
//	addChild(resume_menu);

	
	CCLabelTTF* t2_label = CCLabelTTF::create();
	
	KSLabelTTF* gohome_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ingameOut), mySGD->getFont().c_str(), 18);
	gohome_label->enableOuterStroke(ccBLACK, 1.f);
	gohome_label->setPosition(ccp(0,0));
	t2_label->addChild(gohome_label);
	
	CCScale9Sprite* gohome_back = CCScale9Sprite::create("rank_normal.png", CCRectMake(0,0,40,40), CCRectMake(19, 19, 2, 2));
	
	CCControlButton* gohome_button = CCControlButton::create(t2_label, gohome_back);
	gohome_button->addTargetWithActionForControlEvents(this, cccontrol_selector(PauseContent::gohomeAction), CCControlEventTouchUpInside);
	gohome_button->setPreferredSize(CCSizeMake(120,45));
	gohome_button->setPosition(ccp(75,-12));
	addChild(gohome_button);
	
	gohome_button->setTouchPriority(touch_priority-1);
	
//	CommonButton* gohome_menu = CommonButton::create("나가기", 14, CCSizeMake(150,45), CommonButtonOrange, touch_priority-1);
//	gohome_menu->setPosition(ccp(0, 11));
//	gohome_menu->setFunction([=](CCObject* sender)
//							 {
//								 CCNode* t_node = CCNode::create();
//								 t_node->setTag(kPauseContentMenuTag_gohome);
//								 menuAction(t_node);
//							 });
//	addChild(gohome_menu);
	
	CCLabelTTF* t3_label = CCLabelTTF::create();
	
	KSLabelTTF* replay_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ingameReplay), mySGD->getFont().c_str(), 18);
	replay_label->enableOuterStroke(ccBLACK, 1.f);
	replay_label->setPosition(ccp(0,0));
	t3_label->addChild(replay_label);
	
	CCScale9Sprite* replay_back = CCScale9Sprite::create("rank_normal.png", CCRectMake(0,0,40,40), CCRectMake(19, 19, 2, 2));
	
	CCControlButton* replay_button = CCControlButton::create(t3_label, replay_back);
	replay_button->addTargetWithActionForControlEvents(this, cccontrol_selector(PauseContent::replayAction), CCControlEventTouchUpInside);
	replay_button->setPreferredSize(CCSizeMake(120,45));
	replay_button->setPosition(ccp(75,-54));
	addChild(replay_button);
	
	replay_button->setTouchPriority(touch_priority-1);
	
	if(mySGD->is_endless_mode)
		replay_button->setEnabled(false);
	
	
//	CommonButton* replay_menu = CommonButton::create("재시작", 14, CCSizeMake(150,45), CommonButtonOrange, touch_priority-1);
//	replay_menu->setBackgroundTypeForDisabled(CommonButtonPupple);
//	replay_menu->setTitleColorForDisable(ccGRAY);
//	replay_menu->setPosition(ccp(0,-31));
//	replay_menu->setFunction([=](CCObject* sender)
//							 {
//								 CCNode* t_node = CCNode::create();
//								 t_node->setTag(kPauseContentMenuTag_replay);
//								 menuAction(t_node);
//							 });
//	addChild(replay_menu);
	
	CCNode* safety_node = CCNode::create();
	safety_node->setPosition(ccp(-60,18));
	addChild(safety_node);
	
	
	KSLabelTTF* safety_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_publicPlaceMode), mySGD->getFont().c_str(), 13);
	safety_label->enableOuterStroke(ccBLACK, 1.f);
	safety_label->setPosition(ccp(-45,-73));
	safety_node->addChild(safety_label);
	
	CCSprite* line_img = CCSprite::create("option_scroll.png");
	line_img->setScaleX(0.7f);
	line_img->setPosition(ccp(25,-73));
	safety_node->addChild(line_img);
	
	CCMenuLambda* on_off_menu = CCMenuLambda::create();
	on_off_menu->setPosition(ccp(25,-73));
	safety_node->addChild(on_off_menu);
	on_off_menu->setTouchPriority(touch_priority-1);
	
	CCSprite* n_on = CCSprite::create("option_button.png");
	KSLabelTTF* n_on_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOn), mySGD->getFont().c_str(), 11);
	n_on_label->enableOuterStroke(ccBLACK, 0.5f);
	n_on_label->setPosition(ccp(n_on->getContentSize().width/2.f, n_on->getContentSize().height/2.f));
	n_on->addChild(n_on_label);
	
	CCSprite* s_on = CCSprite::create("option_button.png");
	s_on->setColor(ccGRAY);
	KSLabelTTF* s_on_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOn), mySGD->getFont().c_str(), 11);
	s_on_label->enableOuterStroke(ccBLACK, 0.5f);
	s_on_label->setPosition(ccp(s_on->getContentSize().width/2.f, s_on->getContentSize().height/2.f));
	s_on->addChild(s_on_label);
	
	on_item = CCMenuItemSpriteLambda::create(n_on, s_on, [=](CCObject* sender){
		this->on_item->setVisible(false);
		this->off_item->setVisible(true);
		
		bool t_safety = !myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
		myDSH->setBoolForKey(kDSH_Key_isSafetyMode, t_safety);
		mySGD->is_safety_mode = t_safety;
	});
	on_item->setPosition(ccp(-15,0));
	
	on_off_menu->addChild(on_item);
	
	
	CCSprite* n_off = CCSprite::create("option_button.png");
	KSLabelTTF* n_off_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOff), mySGD->getFont().c_str(), 11);
	n_off_label->enableOuterStroke(ccBLACK, 0.5f);
	n_off_label->setPosition(ccp(n_off->getContentSize().width/2.f, n_off->getContentSize().height/2.f));
	n_off->addChild(n_off_label);
	
	CCSprite* s_off = CCSprite::create("option_button.png");
	s_off->setColor(ccGRAY);
	KSLabelTTF* s_off_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOff), mySGD->getFont().c_str(), 11);
	s_off_label->enableOuterStroke(ccBLACK, 0.5f);
	s_off_label->setPosition(ccp(s_off->getContentSize().width/2.f, s_off->getContentSize().height/2.f));
	s_off->addChild(s_off_label);
	
	off_item = CCMenuItemSpriteLambda::create(n_off, s_off, [=](CCObject* sender){
		this->on_item->setVisible(true);
		this->off_item->setVisible(false);
		
		bool t_safety = !myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
		myDSH->setBoolForKey(kDSH_Key_isSafetyMode, t_safety);
		mySGD->is_safety_mode = t_safety;
	});
	off_item->setPosition(ccp(15,0));
	
	on_off_menu->addChild(off_item);
	
	
	
	bool is_safety = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
	if(is_safety)
		off_item->setVisible(false);
	else
		on_item->setVisible(false);
	
	
	
	CCNode* sound_node = CCNode::create();
	sound_node->setPosition(ccp(-60,102));
	addChild(sound_node);
	
	
	KSLabelTTF* sound_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_bgm), mySGD->getFont().c_str(), 13);
	sound_label->enableOuterStroke(ccBLACK, 1.f);
	sound_label->setPosition(ccp(-45,-73));
	sound_node->addChild(sound_label);
	
	CCSprite* line2_img = CCSprite::create("option_scroll.png");
	line2_img->setScaleX(0.7f);
	line2_img->setPosition(ccp(25,-73));
	sound_node->addChild(line2_img);
	
	CCMenuLambda* on_off_menu2 = CCMenuLambda::create();
	on_off_menu2->setPosition(ccp(25,-73));
	sound_node->addChild(on_off_menu2);
	on_off_menu2->setTouchPriority(touch_priority-1);
	
	CCSprite* n_on2 = CCSprite::create("option_button.png");
	KSLabelTTF* n_on_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOn), mySGD->getFont().c_str(), 11);
	n_on_label2->enableOuterStroke(ccBLACK, 0.5f);
	n_on_label2->setPosition(ccp(n_on2->getContentSize().width/2.f, n_on2->getContentSize().height/2.f));
	n_on2->addChild(n_on_label2);
	
	CCSprite* s_on2 = CCSprite::create("option_button.png");
	s_on2->setColor(ccGRAY);
	KSLabelTTF* s_on_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOn), mySGD->getFont().c_str(), 11);
	s_on_label2->enableOuterStroke(ccBLACK, 0.5f);
	s_on_label2->setPosition(ccp(s_on2->getContentSize().width/2.f, s_on2->getContentSize().height/2.f));
	s_on2->addChild(s_on_label2);
	
	on_item2 = CCMenuItemSpriteLambda::create(n_on2, s_on2, [=](CCObject* sender){
		this->on_item2->setVisible(false);
		this->off_item2->setVisible(true);
		
		bool t_sound = !myDSH->getBoolForKey(kDSH_Key_bgmOff);
		myDSH->setBoolForKey(kDSH_Key_bgmOff, t_sound);
		AudioEngine::sharedInstance()->setSoundOnOff(!myDSH->getBoolForKey(kDSH_Key_bgmOff));
	});
	on_item2->setPosition(ccp(-15,0));
	
	on_off_menu2->addChild(on_item2);
	
	
	CCSprite* n_off2 = CCSprite::create("option_button.png");
	KSLabelTTF* n_off_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOff), mySGD->getFont().c_str(), 11);
	n_off_label2->enableOuterStroke(ccBLACK, 0.5f);
	n_off_label2->setPosition(ccp(n_off2->getContentSize().width/2.f, n_off2->getContentSize().height/2.f));
	n_off2->addChild(n_off_label2);
	
	CCSprite* s_off2 = CCSprite::create("option_button.png");
	s_off2->setColor(ccGRAY);
	KSLabelTTF* s_off_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOff), mySGD->getFont().c_str(), 11);
	s_off_label2->enableOuterStroke(ccBLACK, 0.5f);
	s_off_label2->setPosition(ccp(s_off2->getContentSize().width/2.f, s_off2->getContentSize().height/2.f));
	s_off2->addChild(s_off_label2);
	
	off_item2 = CCMenuItemSpriteLambda::create(n_off2, s_off2, [=](CCObject* sender){
		this->on_item2->setVisible(true);
		this->off_item2->setVisible(false);
		
		bool t_sound = !myDSH->getBoolForKey(kDSH_Key_bgmOff);
		myDSH->setBoolForKey(kDSH_Key_bgmOff, t_sound);
		AudioEngine::sharedInstance()->setSoundOnOff(!myDSH->getBoolForKey(kDSH_Key_bgmOff));
	});
	off_item2->setPosition(ccp(15,0));
	
	on_off_menu2->addChild(off_item2);
	
	
	
	bool is_sound = myDSH->getBoolForKey(kDSH_Key_bgmOff);
	if(is_sound)
		on_item2->setVisible(false);
	else
		off_item2->setVisible(false);
	
	
	CCNode* effect_node = CCNode::create();
	effect_node->setPosition(ccp(-60,60));
	addChild(effect_node);
	
	
	KSLabelTTF* effect_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_effect), mySGD->getFont().c_str(), 13);
	effect_label->enableOuterStroke(ccBLACK, 1.f);
	effect_label->setPosition(ccp(-45,-73));
	effect_node->addChild(effect_label);
	
	CCSprite* line3_img = CCSprite::create("option_scroll.png");
	line3_img->setScaleX(0.7f);
	line3_img->setPosition(ccp(25,-73));
	effect_node->addChild(line3_img);
	
	CCMenuLambda* on_off_menu3 = CCMenuLambda::create();
	on_off_menu3->setPosition(ccp(25,-73));
	effect_node->addChild(on_off_menu3);
	on_off_menu3->setTouchPriority(touch_priority-1);
	
	CCSprite* n_on3 = CCSprite::create("option_button.png");
	KSLabelTTF* n_on_label3 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOn), mySGD->getFont().c_str(), 11);
	n_on_label3->enableOuterStroke(ccBLACK, 0.5f);
	n_on_label3->setPosition(ccp(n_on3->getContentSize().width/2.f, n_on3->getContentSize().height/2.f));
	n_on3->addChild(n_on_label3);
	
	CCSprite* s_on3 = CCSprite::create("option_button.png");
	s_on3->setColor(ccGRAY);
	KSLabelTTF* s_on_label3 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOn), mySGD->getFont().c_str(), 11);
	s_on_label3->enableOuterStroke(ccBLACK, 0.5f);
	s_on_label3->setPosition(ccp(s_on3->getContentSize().width/2.f, s_on3->getContentSize().height/2.f));
	s_on3->addChild(s_on_label3);
	
	on_item3 = CCMenuItemSpriteLambda::create(n_on3, s_on3, [=](CCObject* sender){
		this->on_item3->setVisible(false);
		this->off_item3->setVisible(true);
		
		bool t_effect = !myDSH->getBoolForKey(kDSH_Key_effectOff);
		myDSH->setBoolForKey(kDSH_Key_effectOff, t_effect);
		AudioEngine::sharedInstance()->setEffectOnOff(!myDSH->getBoolForKey(kDSH_Key_effectOff));
	});
	on_item3->setPosition(ccp(-15,0));
	
	on_off_menu3->addChild(on_item3);
	
	
	CCSprite* n_off3 = CCSprite::create("option_button.png");
	KSLabelTTF* n_off_label3 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOff), mySGD->getFont().c_str(), 11);
	n_off_label3->enableOuterStroke(ccBLACK, 0.5f);
	n_off_label3->setPosition(ccp(n_off3->getContentSize().width/2.f, n_off3->getContentSize().height/2.f));
	n_off3->addChild(n_off_label3);
	
	CCSprite* s_off3 = CCSprite::create("option_button.png");
	s_off3->setColor(ccGRAY);
	KSLabelTTF* s_off_label3 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_lightOff), mySGD->getFont().c_str(), 11);
	s_off_label3->enableOuterStroke(ccBLACK, 0.5f);
	s_off_label3->setPosition(ccp(s_off3->getContentSize().width/2.f, s_off3->getContentSize().height/2.f));
	s_off3->addChild(s_off_label3);
	
	off_item3 = CCMenuItemSpriteLambda::create(n_off3, s_off3, [=](CCObject* sender){
		this->on_item3->setVisible(true);
		this->off_item3->setVisible(false);
		
		bool t_effect = !myDSH->getBoolForKey(kDSH_Key_effectOff);
		myDSH->setBoolForKey(kDSH_Key_effectOff, t_effect);
		AudioEngine::sharedInstance()->setEffectOnOff(!myDSH->getBoolForKey(kDSH_Key_effectOff));
	});
	off_item3->setPosition(ccp(15,0));
	
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