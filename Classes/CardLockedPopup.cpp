//
//  CardLockedPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#include "CardLockedPopup.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "StyledLabelTTF.h"
#include "CommonButton.h"

CardLockedPopup* CardLockedPopup::create(int t_touch_priority, function<void()> t_end_func, int t_step)
{
	CardLockedPopup* t_mup = new CardLockedPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_step);
	t_mup->autorelease();
	return t_mup;
}

void CardLockedPopup::myInit(int t_touch_priority, function<void()> t_end_func, int t_step)
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
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(230,120));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(220, 80));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-15));
	back_case->addChild(back_in);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_cardLockedTitle), t_step)->getCString(), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255,170,20));
	title_label->setAnchorPoint(ccp(0, 0.5f));
	title_label->setPosition(ccp(-back_case->getContentSize().width/2.f+17,37));
	m_container->addChild(title_label);
	
	
	StyledLabelTTF* slttf = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_cardLockedMent1+t_step-1)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kLeftAlignment);
	slttf->setPosition(ccp(-back_case->getContentSize().width/2.f+19,5));
	m_container->addChild(slttf);
	
	if(t_step == 1 || t_step == 3)
	{
		slttf->setPosition(ccp(-back_case->getContentSize().width/2.f+19,-14));
	}
	else
	{
		float distance = 21.f;
		CCPoint base_position = ccp(-back_case->getContentSize().width/2.f+32, -35);
		for(int i=1;i<=6;i++)
		{
			CCSprite* change_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", i)->getCString());
			change_img->setPosition(base_position + ccp(distance*(i-1),0));
			m_container->addChild(change_img);
		}
	}
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-22,back_case->getContentSize().height/2.f-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  closeAction(sender, CCControlEventTouchUpInside);
							  });
	m_container->addChild(close_button);
	
	
//	KSLabelTTF* close_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
//	CCScale9Sprite* close_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
//	close_button = CCControlButton::create(close_label, close_back);
//	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CardLockedPopup::closeAction), CCControlEventTouchUpInside);
//	close_button->setPreferredSize(CCSizeMake(90,45));
//	close_button->setPosition(ccp(0,-40));
//	m_container->addChild(close_button);
//	
//	close_button->setTouchPriority(touch_priority);
	
	
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

void CardLockedPopup::closeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}