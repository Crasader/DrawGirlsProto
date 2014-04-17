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

PauseContent* PauseContent::create(int t_touch_priority, function<void(void)> t_resume, function<void(void)> t_gohome, function<void(void)> t_replay)
{
	PauseContent* t_ctc = new PauseContent();
	t_ctc->myInit(t_touch_priority, t_resume, t_gohome, t_replay);
	t_ctc->autorelease();
	return t_ctc;
}

void PauseContent::startShow()
{
	setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.f);
				endShow();}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(this, t);}, [=](int t){KS::setOpacity(this, 255);}));
	
	setPosition(ccp(240, myDSH->ui_center_y));
}

void PauseContent::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
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
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	case_back->setPosition(CCPointZero);
	addChild(case_back);
	
	case_back->setContentSize(CCSizeMake(180, 230));
	
	CCLabelTTF* title_label = CCLabelTTF::create("일시정지", mySGD->getFont().c_str(), 15);
	title_label->setColor(ccBLACK);
	title_label->setPosition(ccp(0, 97));
	addChild(title_label);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6));
	content_back->setPosition(ccp(0,-10));
	addChild(content_back);
	
	content_back->setContentSize(CCSizeMake(160,180));
	
	CommonButton* resume_menu = CommonButton::create("계속하기", 14, CCSizeMake(150,45), CommonButtonOrange, touch_priority-1);
	resume_menu->setPosition(ccp(0,53));
	resume_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kPauseContentMenuTag_resume);
								 menuAction(t_node);
							 });
	addChild(resume_menu);
	
	
	CommonButton* gohome_menu = CommonButton::create("나가기", 14, CCSizeMake(150,45), CommonButtonOrange, touch_priority-1);
	gohome_menu->setPosition(ccp(0, 11));
	gohome_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kPauseContentMenuTag_gohome);
								 menuAction(t_node);
							 });
	addChild(gohome_menu);
	
	
	CommonButton* replay_menu = CommonButton::create("재시작", 14, CCSizeMake(150,45), CommonButtonOrange, touch_priority-1);
	replay_menu->setBackgroundTypeForDisabled(CommonButtonPupple);
	replay_menu->setTitleColorForDisable(ccGRAY);
	replay_menu->setPosition(ccp(0,-31));
	replay_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kPauseContentMenuTag_replay);
								 menuAction(t_node);
							 });
	addChild(replay_menu);
	
	CCLabelTTF* safety_label = CCLabelTTF::create("대중교통모드", mySGD->getFont().c_str(), 12);
	safety_label->setPosition(ccp(-30,-70));
	addChild(safety_label);
	
	bool is_safety = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
	string on_off_str;
	if(is_safety)
		on_off_str = "on";
	else
		on_off_str = "off";
	CommonButton* on_off = CommonButton::create(on_off_str, 14, CCSizeMake(60, 45), CommonButtonOrange, touch_priority-1);
	on_off->setPosition(ccp(40,-70));
	on_off->setFunction([=](CCObject* sender)
						{
							bool t_safety = !myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
							myDSH->setBoolForKey(kDSH_Key_isSafetyMode, t_safety);
							mySGD->is_safety_mode = t_safety;
							if(t_safety)
								on_off->setTitle("on");
							else
								on_off->setTitle("off");
						});
	addChild(on_off);
	
	replay_menu->setEnabled(true);
}