//
//  ClearTimeEventContent.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 6. 12..
//
//

#include "ClearTimeEventContent.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "CommonButton.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
#include "FormSetter.h"
#include "StyledLabelTTF.h"

ClearTimeEventContent* ClearTimeEventContent::create(int t_touch_priority, function<void(void)> t_no, function<void(void)> t_yes)
{
	ClearTimeEventContent* t_ctc = new ClearTimeEventContent();
	t_ctc->myInit(t_touch_priority, t_no, t_yes);
	t_ctc->autorelease();
	return t_ctc;
}

void ClearTimeEventContent::startShow()
{
	CommonAnimation::openPopup(this, this, nullptr, [=](){
		
	}, [=](){
		endShow();
	});
	setPosition(ccp(240, myDSH->ui_center_y));
}

void ClearTimeEventContent::noAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	is_menu_enable = false;
	
	selected_selector = no_selector;
	startHide();
}
void ClearTimeEventContent::yesAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	is_menu_enable = false;
	
	selected_selector = yes_selector;
	startHide();
}

void ClearTimeEventContent::startHide()
{
	addChild(KSTimer::create(0.2f, [=](){
		endHide();
	}));
	
	CommonAnimation::closePopup(this, this, nullptr, [=](){
		
	}, [=](){
		selected_selector();
		getParent()->removeFromParent();
		//		end_func(); removeFromParent();
	});
}

void ClearTimeEventContent::myInit(int t_touch_priority, function<void(void)> t_no, function<void(void)> t_yes)
{
	startFormSetter(this);
	
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	no_selector = t_no;
	yes_selector = t_yes;
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	case_back->setPosition(CCPointZero);
	addChild(case_back);
	
	case_back->setContentSize(CCSizeMake(220, 186));
	setFormSetter(case_back);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_clearTimeEventTitle), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setPosition(ccp(0, 70));
	addChild(title_label);
	setFormSetter(title_label);
	
	
//	CommonButton* close_button = CommonButton::createCloseButton(touch_priority-1);
//	close_button->setPosition(ccp(125, 70));
//	close_button->setFunction([=](CCObject* sender){
//		if(!is_menu_enable)
//			return;
//		
//		is_menu_enable = false;
//		
//		startHide();
//	});
//	addChild(close_button);
	
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24,24,2,2));
	content_back->setPosition(ccp(0,-13));
	addChild(content_back);
	
	content_back->setContentSize(CCSizeMake(206,140));
	setFormSetter(content_back);
	
	StyledLabelTTF* ment_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_clearTimeEventMent), 85.f, mySGD->getTimeEventFloatValue(kTimeEventType_clear))->getCString(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	ment_label->setPosition(ccp(0,35));
	addChild(ment_label);
	setFormSetter(ment_label);
	
	CCLabelTTF* t1_label = CCLabelTTF::create();
	
	KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_no), mySGD->getFont().c_str(), 13);
	no_label->enableOuterStroke(ccBLACK, 1.f);
	no_label->setPosition(ccp(0,0));
	t1_label->addChild(no_label);
	
	CCScale9Sprite* no_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0,0,40,40), CCRectMake(19, 19, 2, 2));
	
	CCControlButton* no_button = CCControlButton::create(t1_label, no_back);
	no_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ClearTimeEventContent::noAction), CCControlEventTouchUpInside);
	no_button->setPreferredSize(CCSizeMake(90,40));
	no_button->setPosition(ccp(-45,-50));
	addChild(no_button);
	setFormSetter(no_button);
	
	no_button->setTouchPriority(touch_priority-1);
	
	
	CCLabelTTF* t2_label = CCLabelTTF::create();
	
	KSLabelTTF* yes_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_yes), mySGD->getFont().c_str(), 13);
	yes_label->setColor(ccc3(255, 170, 20));
	yes_label->enableOuterStroke(ccBLACK, 1.f);
	yes_label->setPosition(ccp(0,0));
	t2_label->addChild(yes_label);
	
	CCSprite* event_img = CCSprite::create("puzzle_event.png");
	event_img->setPosition(ccp(10,15));
	t2_label->addChild(event_img);
	setFormSetter(event_img);
	
	CCScale9Sprite* yes_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0,0,40,40), CCRectMake(19, 19, 2, 2));
	
	CCControlButton* yes_button = CCControlButton::create(t2_label, yes_back);
	yes_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ClearTimeEventContent::yesAction), CCControlEventTouchUpInside);
	yes_button->setPreferredSize(CCSizeMake(90,40));
	yes_button->setPosition(ccp(45,-50));
	addChild(yes_button);
	setFormSetter(yes_button);
	
	yes_button->setTouchPriority(touch_priority-1);
}