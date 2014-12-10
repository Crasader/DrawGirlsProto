//
//  CardGachaPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 12. 9..
//
//

#include "CardGachaPopup.h"
#include "TouchSuctionLayer.h"
#include "DataStorageHub.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "AudioEngine.h"
#include "CardSettingPopup.h"

CardGachaPopup* CardGachaPopup::create(int t_touch_priority)
{
	CardGachaPopup* t_popup = new CardGachaPopup();
	if(t_popup && t_popup->init())
	{
		t_popup->myInit(t_touch_priority);
		t_popup->autorelease();
		return t_popup;
	}
	else
	{
		CC_SAFE_DELETE(t_popup);
		return NULL;
	}
}

void CardGachaPopup::setHideFinalAction(CCObject* t_target, SEL_CallFunc t_delegate)
{
	target_final = t_target;
	delegate_final = t_delegate;
}

enum CardGachaPopupZorder
{
	kCardGachaPopupZorder_gray = 0,
	kCardGachaPopupZorder_main_case
};

void CardGachaPopup::myInit(int t_touch_priority)
{
	is_menu_enable = false;
	touch_priority = t_touch_priority;
	target_final = NULL;
	delegate_final = NULL;
	
	TouchSuctionLayer* suction = TouchSuctionLayer::create(touch_priority);
	suction->setTouchEnabled(true);
	addChild(suction);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCardGachaPopupZorder_gray);
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kCardGachaPopupZorder_main_case);
	
	
	
	
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority-1);
	close_button->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  CardSettingPopup* t_popup = CardSettingPopup::create();
								  t_popup->setHideFinalAction(target_final, delegate_final);
								  getParent()->addChild(t_popup, getZOrder());
								  
								  target_final = NULL;
								  
								  CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
															  {
																  if(target_final && delegate_final)
																	  (target_final->*delegate_final)();
																  removeFromParent();
															  });
							  });
	main_case->addChild(close_button);
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){}, [=]()
							   {
								   is_menu_enable = true;
							   });
}