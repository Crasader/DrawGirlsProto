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
#include "FormSetter.h"
#include "CommonAnimation.h"


CardLockedPopup* CardLockedPopup::create(int t_touch_priority, function<void()> t_end_func, int t_step)
{
	CardLockedPopup* t_mup = new CardLockedPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_step);
	t_mup->autorelease();
	return t_mup;
}

void CardLockedPopup::myInit(int t_touch_priority, function<void()> t_end_func, int t_step)
{
	startFormSetter(this);
	
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-45, back_case->getContentSize().height-82));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-9));
	back_case->addChild(back_in);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_cardLockedTitle), t_step)->getCString(), mySGD->getFont().c_str(), 13);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f, 0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	StyledLabelTTF* slttf = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_cardLockedMent1+t_step-1)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
	setFormSetter(slttf);
	slttf->setOldAnchorPoint();
	slttf->setPosition(ccp(0,5));
	m_container->addChild(slttf);
	
	if(t_step == 1 || t_step == 3)
	{
		slttf->setPosition(ccp(0,-14));
	}
	else
	{
		float distance = 21.f;
		CCPoint base_position = ccp(0, -35);
		for(int i=1;i<=6;i++)
		{
			CCSprite* change_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", i)->getCString());
			change_img->setPosition(base_position + ccp(distance*(i-4)+10.5,0));
			m_container->addChild(change_img);
		}
	}
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25,back_case->getContentSize().height/2.f-22));
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
	
	
	CommonAnimation::openPopup(this, m_container, nullptr, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void CardLockedPopup::closeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	CommonAnimation::closePopup(this, m_container, nullptr, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
}