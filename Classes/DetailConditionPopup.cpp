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
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(300,240));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupTitle), mySGD->getFont().c_str(), 21);
	title_label->setColor(ccc3(50, 250, 255));
	title_label->setPosition(ccp(0,70));
	m_container->addChild(title_label);
	
	

	
	KSLabelTTF* sub1_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent1), mySGD->getFont().c_str(), 12);
	sub1_label->setPosition(ccp(0,30));
	m_container->addChild(sub1_label);
	
	StyledLabelTTF* sub2_label = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_detailConditionPopupContent2), mySGD->getFont().c_str(), 12,999,StyledAlignment::kCenterAlignment);
	sub2_label->setPosition(ccp(0,-5));
	m_container->addChild(sub2_label);
	
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
	
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
	ok_label->setPosition(ccp(0,0));
	t_label->addChild(ok_label);
	
	CCScale9Sprite* ok_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	CCControlButton* ok_button = CCControlButton::create(t_label, ok_back);
	ok_button->addTargetWithActionForControlEvents(this, cccontrol_selector(DetailConditionPopup::closeAction), CCControlEventTouchUpInside);
	ok_button->setPreferredSize(CCSizeMake(100,45));
	ok_button->setPosition(ccp(0,-70));
	m_container->addChild(ok_button);
	
	ok_button->setTouchPriority(touch_priority);
	
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 255);is_menu_enable = true;}));
}

void DetailConditionPopup::closeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}