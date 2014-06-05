//
//  DetailConditionPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 25..
//
//

#include "DetailConditionPopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "ASPopupView.h"
#include "MyLocalization.h"
#include "FormSetter.h"
#include "StyledLabelTTF.h"
#include "CommonAnimation.h"


DetailConditionPopup* DetailConditionPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	DetailConditionPopup* t_mup = new DetailConditionPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void DetailConditionPopup::myInit(int t_touch_priority, function<void()> t_end_func)
{
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
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_case->setContentSize(CCSizeMake(250,120));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
	back_case->addChild(back_in);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupTitle), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(0,back_case->getContentSize().height/2.f-25));
	m_container->addChild(title_label);
	
	
	StyledLabelTTF* content_label = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent), mySGD->getFont().c_str(), 12,999,StyledAlignment::kCenterAlignment);
	content_label->setOldAnchorPoint();
	content_label->setPosition(ccp(0,40));
	m_container->addChild(content_label);
	
	

	
//	KSLabelTTF* sub1_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent1), mySGD->getFont().c_str(), 12);
//	sub1_label->setPosition(ccp(0,30));
//	m_container->addChild(sub1_label);
//	
//	StyledLabelTTF* sub2_label = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent2), mySGD->getFont().c_str(), 12,999,StyledAlignment::kCenterAlignment);
//	sub2_label->setPosition(ccp(0,-5));
//	m_container->addChild(sub2_label);
	
//
//	KSLabelTTF* sub2_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent2), mySGD->getFont().c_str(), 12);
//	sub2_label->setColor(ccYELLOW);
//	sub2_label->setAnchorPoint(ccp(1,0.5f));
//	m_container->addChild(sub2_label);
//	
//	KSLabelTTF* sub3_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent3), mySGD->getFont().c_str(), 12);
//	sub3_label->setAnchorPoint(ccp(0, 0.5f));
//	m_container->addChild(sub3_label);
	
//	float x_value;
//	if(sub2_label->getContentSize().width > sub3_label->getContentSize().width)
//		x_value = sub2_label->getContentSize().width - (sub2_label->getContentSize().width+sub3_label->getContentSize().width)/2.f;
//	else
//		x_value = -(sub3_label->getContentSize().width - (sub2_label->getContentSize().width+sub3_label->getContentSize().width)/2.f);
//	
//	sub2_label->setPosition(ccp(x_value, -10));
//	sub3_label->setPosition(ccp(x_value, -10));
	
	
//	KSLabelTTF* sub4_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent4), mySGD->getFont().c_str(), 12);
//	sub4_label->setColor(ccYELLOW);
//	sub4_label->setAnchorPoint(ccp(1,0.5f));
//	sub4_label->setPosition(ccp(0,-25));
//	m_container->addChild(sub4_label);
//	
//	KSLabelTTF* sub5_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent5), mySGD->getFont().c_str(), 12);
//	sub5_label->setAnchorPoint(ccp(0,0.5f));
//	sub5_label->setPosition(ccp(0,-25));
//	m_container->addChild(sub5_label);
//	
//	if(sub4_label->getContentSize().width > sub5_label->getContentSize().width)
//		x_value = sub4_label->getContentSize().width - (sub4_label->getContentSize().width+sub5_label->getContentSize().width)/2.f;
//	else
//		x_value = -(sub5_label->getContentSize().width - (sub4_label->getContentSize().width+sub5_label->getContentSize().width)/2.f);
//	
//	sub4_label->setPosition(ccp(x_value, -25));
//	sub5_label->setPosition(ccp(x_value, -25));
	
	
//	CCLabelTTF* t_label = CCLabelTTF::create();
//	
//	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
//	ok_label->setPosition(ccp(0,0));
//	t_label->addChild(ok_label);
//	
//	CCScale9Sprite* ok_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
//	
//	CCControlButton* ok_button = CCControlButton::create(t_label, ok_back);
//	ok_button->addTargetWithActionForControlEvents(this, cccontrol_selector(DetailConditionPopup::closeAction), CCControlEventTouchUpInside);
//	ok_button->setPreferredSize(CCSizeMake(100,45));
//	ok_button->setPosition(ccp(0,-65));
//	m_container->addChild(ok_button);
//	
//	ok_button->setTouchPriority(touch_priority);

	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	setFormSetter(close_button);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f - 22, back_case->getContentSize().height/2.f - 22));
	close_button->setFunction([=](CCObject* sender)
														{
															this->closeAction(this,CCControlEventTouchUpInside);
														});
	m_container->addChild(close_button);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void DetailConditionPopup::closeAction(CCObject* sender, CCControlEvent t_event)
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