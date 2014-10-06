//
//  JoystickPositionSelectPopup.cpp
//  DGproto
//
//  Created by ParkJeongSoon on 2014. 9. 22..
//
//

#include "JoystickPositionSelectPopup.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "StyledLabelTTF.h"
#include "CommonButton.h"
#include "CommonAnimation.h"


JoystickPositionSelectPopup* JoystickPositionSelectPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	JoystickPositionSelectPopup* t_mup = new JoystickPositionSelectPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void JoystickPositionSelectPopup::myInit(int t_touch_priority, function<void()> t_end_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(m_container);
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251, 113));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-12));
	back_case->addChild(back_in);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_joystickPositionSelectTitle), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f, 0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	StyledLabelTTF* slttf = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_joystickPositionSelectContent), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
	slttf->setAnchorPoint(ccp(0.5f,0.5f));
	slttf->setPosition(ccp(0,18));
	m_container->addChild(slttf);
	
	
    CCLabelTTF* l_label = CCLabelTTF::create();
    KSLabelTTF* left_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_left), mySGD->getFont().c_str(), 15);
    left_label->setPosition(ccp(0,0));
    l_label->addChild(left_label);
    
    CCSprite* recommend_back = CCSprite::create("white_speech_bubble.png");
    recommend_back->setPosition(ccp(30,20));
    l_label->addChild(recommend_back);
    
    KSLabelTTF* recommend_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_joystickPositionSelectRecommend), mySGD->getFont().c_str(), 13);
    recommend_label->setColor(ccRED);
    recommend_label->enableOuterStroke(ccYELLOW, 0.3f, 50, true);
    recommend_label->setPosition(ccpFromSize(recommend_back->getContentSize()/2.f));
    recommend_back->addChild(recommend_label);
    
    CCControlButton* left_button = CCControlButton::create(l_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
    left_button->addTargetWithActionForControlEvents(this, cccontrol_selector(JoystickPositionSelectPopup::leftAction), CCControlEventTouchUpInside);
	left_button->setPreferredSize(CCSizeMake(92,60));
	left_button->setPosition(ccp(-55,-34));
	m_container->addChild(left_button);
	
	left_button->setTouchPriority(touch_priority);
    
    
	CCLabelTTF* r_label = CCLabelTTF::create();
    KSLabelTTF* right_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_right), mySGD->getFont().c_str(), 15);
    right_label->setPosition(ccp(0,0));
    r_label->addChild(right_label);
    
    CCControlButton* right_button = CCControlButton::create(r_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
    right_button->addTargetWithActionForControlEvents(this, cccontrol_selector(JoystickPositionSelectPopup::rightAction), CCControlEventTouchUpInside);
	right_button->setPreferredSize(CCSizeMake(92,60));
	right_button->setPosition(ccp(55,-34));
	m_container->addChild(right_button);
	
	right_button->setTouchPriority(touch_priority);
    
    
	
    //	KSLabelTTF* close_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
    //	CCScale9Sprite* close_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
    //	close_button = CCControlButton::create(close_label, close_back);
    //	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CardLockedPopup::closeAction), CCControlEventTouchUpInside);
    //	close_button->setPreferredSize(CCSizeMake(90,45));
    //	close_button->setPosition(ccp(0,-40));
    //	m_container->addChild(close_button);
    //
    //	close_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, nullptr, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void JoystickPositionSelectPopup::leftAction(CCObject* sender, CCControlEvent t_event)
{
    if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
    
    myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, kControlJoystickDirection_left);
    
    closeAction(sender, t_event);
}

void JoystickPositionSelectPopup::rightAction(CCObject* sender, CCControlEvent t_event)
{
    if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
    
    myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, kControlJoystickDirection_right);
    
    closeAction(sender, t_event);
}

void JoystickPositionSelectPopup::closeAction(CCObject* sender, CCControlEvent t_event)
{
	CommonAnimation::closePopup(this, m_container, nullptr, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
}