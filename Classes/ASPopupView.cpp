//
//  ASPopupView.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 26..
//
//

#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "DataStorageHub.h"
#include "CommonButton.h"
#include "AudioEngine.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment)
{
	return getCommonNoti(t_touch_priority, t_comment, [](){});
}

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment, function<void()> close_func)
{
	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	float height_value = 320.f;
	if(myDSH->screen_convert_rate < 1.f)
		height_value = 320.f/myDSH->screen_convert_rate;
	
	if(height_value < myDSH->ui_top)
		height_value = myDSH->ui_top;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, 160));
	t_popup->setBasePosition(ccp(240, 160));
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	case_back->setPosition(CCPointZero);
	t_container->addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	content_back->setPosition(CCPointZero);
	t_container->addChild(content_back);
	
	CCLabelTTF* ment_label = CCLabelTTF::create(t_comment.c_str(), mySGD->getFont().c_str(), 15);
	ment_label->setPosition(ccp(0,0));
	t_container->addChild(ment_label);
	
	case_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+60, ment_label->getContentSize().height + 40));
	content_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+40, ment_label->getContentSize().height + 20));
	
	CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
	close_button->setPosition(ccp(case_back->getContentSize().width/2.f-10,
								  case_back->getContentSize().height/2.f-10));
	close_button->setFunction([=](CCObject* sender)
							  {
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  close_func();
								  t_popup->removeFromParent();
							  });
	t_container->addChild(close_button);
	
	return t_popup;
}

ASPopupView* ASPopupView::getNotEnoughtGoodsGoShopPopup(int t_touch_priority, GoodsType t_type, function<void()> goshop_func)
{
	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	float height_value = 320.f;
	if(myDSH->screen_convert_rate < 1.f)
		height_value = 320.f/myDSH->screen_convert_rate;
	
	if(height_value < myDSH->ui_top)
		height_value = myDSH->ui_top;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, 160));
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	t_popup->setBasePosition(ccp(240, 160));
	
	CCScale9Sprite* back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(300,240));
	back_case->setPosition(ccp(0,0));
	t_container->addChild(back_case);
	
	string title_string;
	if(t_type == kGoodsType_ruby)
		title_string = myLoc->getLocalForKey(kMyLocalKey_rubyNotEnought);
	else if(t_type == kGoodsType_gold)
		title_string = myLoc->getLocalForKey(kMyLocalKey_goldNotEnought);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title_string.c_str(), mySGD->getFont().c_str(), 18);
	title_label->setColor(ccc3(255, 100, 100));
	title_label->setPosition(ccp(0,60));
	t_container->addChild(title_label);
	
	KSLabelTTF* sub_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_enoughtGoodsContent), mySGD->getFont().c_str(), 15);
	sub_label->setPosition(ccp(0,20));
	t_container->addChild(sub_label);
	
	t_popup->button_func_list.clear();
	t_popup->button_func_list.push_back([=](){
		if(!t_popup->is_menu_enable)
			return;
		
		t_popup->is_menu_enable = false;
		
		t_popup->addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
			t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.f);}));}));
		
		t_popup->addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(t_container, t);}, [=](int t){KS::setOpacity(t_container, 0); t_popup->removeFromParent();}));
		
	});
	
	CCLabelTTF* t1_label = CCLabelTTF::create();
	
	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 13);
	giveup_label->setPosition(ccp(0,0));
	t1_label->addChild(giveup_label);
	
	CCScale9Sprite* giveup_back = CCScale9Sprite::create("subpop_gray.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	CCControlButton* giveup_button = CCControlButton::create(t1_label, giveup_back);
	giveup_button->addTargetWithActionForControlEvents(t_popup, cccontrol_selector(ASPopupView::buttonAction), CCControlEventTouchUpInside);
	giveup_button->setTag(0);
	giveup_button->setPreferredSize(CCSizeMake(80,45));
	giveup_button->setPosition(ccp(-45,-60));
	t_container->addChild(giveup_button);
	
	giveup_button->setTouchPriority(t_popup->getTouchPriority()-5);
	
	
	t_popup->button_func_list.push_back([=](){
		if(!t_popup->is_menu_enable)
			return;
		
		t_popup->is_menu_enable = false;
		
		t_popup->addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
			t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.f);}));}));
		
		t_popup->addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(t_container, t);}, [=](int t){KS::setOpacity(t_container, 0); goshop_func(); t_popup->removeFromParent();}));
		
	});
	
	CCLabelTTF* t2_label = CCLabelTTF::create();
	
	KSLabelTTF* goshop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_goShopButton), mySGD->getFont().c_str(), 13);
	goshop_label->setPosition(ccp(0,0));
	t2_label->addChild(goshop_label);
	
	CCScale9Sprite* goshop_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	CCControlButton* goshop_button = CCControlButton::create(t2_label, goshop_back);
	goshop_button->addTargetWithActionForControlEvents(t_popup, cccontrol_selector(ASPopupView::buttonAction), CCControlEventTouchUpInside);
	goshop_button->setTag(1);
	goshop_button->setPreferredSize(CCSizeMake(80,45));
	goshop_button->setPosition(ccp(45,-60));
	t_container->addChild(goshop_button);
	
	goshop_button->setTouchPriority(t_popup->getTouchPriority()-5);
	
	
	
	
	t_container->setScaleY(0.f);
	
	t_popup->addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
		t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.8f);
			t_popup->addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.f);}));}));}));
	
	t_popup->addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(t_container, t);}, [=](int t){KS::setOpacity(t_container, 255);t_popup->is_menu_enable = true;}));
	
	
	return t_popup;
}

void ASPopupView::buttonAction(CCObject* sender, CCControlEvent t_event)
{
	int tag = ((CCNode*)sender)->getTag();
	
	button_func_list[tag]();
}