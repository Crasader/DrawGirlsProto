//
//  GachaRefreshCheckPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 11. 3..
//
//

#include "GachaRefreshCheckPopup.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "KsLocal.h"
#include "StyledLabelTTF.h"
#include "KSLabelTTF.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"
#include "CommonAnimation.h"


GachaRefreshCheckPopup* GachaRefreshCheckPopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_action_func)
{
	GachaRefreshCheckPopup* t_mup = new GachaRefreshCheckPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_action_func);
	t_mup->autorelease();
	return t_mup;
}

void GachaRefreshCheckPopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_action_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	action_func = t_action_func;
	
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
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251, 65));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f+9));
	back_case->addChild(back_in);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noti), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f, 0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	StyledLabelTTF* slttf = StyledLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_nowRefreshNeedGem), mySGD->getGachaRefreshGem()), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
	slttf->setAnchorPoint(ccp(0.5f,0.5f));
	slttf->setPosition(ccpFromSize(back_in->getContentSize()/2.f));
	back_in->addChild(slttf);
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25,back_case->getContentSize().height/2.f-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  closeAction(sender, CCControlEventTouchUpInside);
							  });
	m_container->addChild(close_button);
	
	
	CCLabelTTF* t_action_label = CCLabelTTF::create();
	KSLabelTTF* action_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_refresh), mySGD->getFont().c_str(), 12);
	t_action_label->addChild(action_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("common_shadowgray2.png", CCRectMake(0, 0, 23, 23), CCRectMake(11, 11, 1, 1));
	CCSprite* price_icon = CCSprite::create("icon_r.png");
	KSLabelTTF* price_label = KSLabelTTF::create(KS::insert_separator(mySGD->getGachaRefreshGem()).c_str(), mySGD->getFont().c_str(), 12);
	price_back->setContentSize(CCSizeMake(5+price_icon->getContentSize().width+price_label->getContentSize().width+5, 23));
	price_icon->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_label->getContentSize().width/2.f-5, 0));
	price_label->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_icon->getContentSize().width/2.f-5, 0));
	price_back->addChild(price_icon);
	price_back->addChild(price_label);
	t_action_label->addChild(price_back);
	
	action_label->setPosition(ccp(-price_back->getContentSize().width/2.f-3,0));
	price_back->setPosition(ccp(action_label->getContentSize().width/2.f+3,-1));
	
	CCScale9Sprite* action_back = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0,0,92,45), CCRectMake(45, 22, 2, 1));
	CCControlButton*	action_button = CCControlButton::create(t_action_label, action_back);
	action_button->addTargetWithActionForControlEvents(this, cccontrol_selector(GachaRefreshCheckPopup::actionAction), CCControlEventTouchUpInside);
	action_button->setPreferredSize(CCSizeMake(92+60,45));
	action_button->setPosition(ccp(0,-49));
	m_container->addChild(action_button);
	
	action_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void GachaRefreshCheckPopup::closeAction(CCObject* sender, CCControlEvent t_event)
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

void GachaRefreshCheckPopup::actionAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		action_func(); removeFromParent();
	});
}