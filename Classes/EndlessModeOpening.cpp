//
//  EndlessModeOpening.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 19..
//
//

#include "EndlessModeOpening.h"
#include "DataStorageHub.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "TouchSuctionLayer.h"

enum EndlessModeOpeningZorder
{
	kEndlessModeOpeningZorder_gray = 0,
	kEndlessModeOpeningZorder_back,
	kEndlessModeOpeningZorder_content
};

bool EndlessModeOpening::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	
	touch_priority = -300;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kEndlessModeOpeningZorder_gray);
	
	gray->runAction(CCFadeTo::create(0.5f, 255));
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	suction->setTouchEnabled(true);
	addChild(suction);
	
	is_menu_enable = false;
	
	setMain();
	
	return true;
}

void EndlessModeOpening::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

void EndlessModeOpening::setMain()
{
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-22.f));
	addChild(main_case, kEndlessModeOpeningZorder_back);
	
	main_case->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t)
										 {
											 KS::setOpacity(main_case, 255);
											 
											 is_menu_enable = true;
										 }));
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	left_back->setContentSize(CCSizeMake(200, 232));
	left_back->setPosition(ccp(10+left_back->getContentSize().width/2.f, main_case->getContentSize().height*0.44f));
	main_case->addChild(left_back);
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_back->setContentSize(CCSizeMake(250, 232));
	right_back->setPosition(ccp(main_case->getContentSize().width-10-right_back->getContentSize().width/2.f, main_case->getContentSize().height*0.44f));
	main_case->addChild(right_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create("무한모드", mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(17,main_case->getContentSize().height-25));
	main_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(main_case->getContentSize().width-25, main_case->getContentSize().height-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  is_menu_enable = false;
								  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
																		 {
																			 gray->setOpacity(255*t);
																		 }, [=](float t)
																		 {
																			 gray->setOpacity(0);
																			 if(target_final && delegate_final)
																				 (target_final->*delegate_final)();
																			 removeFromParent();
																		 }));
								  
								  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
									  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
								  
								  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t){KS::setOpacity(main_case, 0);}));
							  });
	main_case->addChild(close_button);
}


