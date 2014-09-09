//
//  JoystickSizeQuestionPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 9. 9..
//
//

#include "JoystickSizeQuestionPopup.h"
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


JoystickSizeQuestionPopup* JoystickSizeQuestionPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	JoystickSizeQuestionPopup* t_mup = new JoystickSizeQuestionPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void JoystickSizeQuestionPopup::myInit(int t_touch_priority, function<void()> t_end_func)
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
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251, 113));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-12));
	back_case->addChild(back_in);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_joystickSizeQuestionTitle), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f, 0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	StyledLabelTTF* slttf = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_joystickSizeQuestionContent), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
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
	
	
	//	KSLabelTTF* close_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
	//	CCScale9Sprite* close_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	//	close_button = CCControlButton::create(close_label, close_back);
	//	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CardLockedPopup::closeAction), CCControlEventTouchUpInside);
	//	close_button->setPreferredSize(CCSizeMake(90,45));
	//	close_button->setPosition(ccp(0,-40));
	//	m_container->addChild(close_button);
	//
	//	close_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void JoystickSizeQuestionPopup::closeAction(CCObject* sender, CCControlEvent t_event)
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