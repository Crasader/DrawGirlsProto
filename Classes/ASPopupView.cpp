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
#include "FormSetter.h"

ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment)
{
	return getCommonNoti(t_touch_priority, t_comment, [](){});
}


ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_title, string t_comment, function<void()> close_func, CCPoint t_position)
{
	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	startFormSetter(t_popup);
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
	
	if(t_position.equals(CCPointZero))
	{
		if(mySGD->is_on_maingame)
		{
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		}
		else
		{
			t_popup->setDimmedPosition(ccp(240, 160));
			t_popup->setBasePosition(ccp(240, 160));
		}
	}
	else
	{
		t_popup->setDimmedPosition(t_position);
		t_popup->setBasePosition(t_position);
	}
	
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	setFormSetter(case_back);
	case_back->setPosition(CCPointZero);
	t_container->addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	setFormSetter(content_back);
	content_back->setPosition(ccp(0,-10));
	t_container->addChild(content_back);
	
	CCLabelTTF* title_label = CCLabelTTF::create(t_title.c_str(), mySGD->getFont().c_str(), 16);
	setFormSetter(title_label);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setPosition(ccp(0,0));
	t_container->addChild(title_label);
	
	CCLabelTTF* ment_label = CCLabelTTF::create(t_comment.c_str(), mySGD->getFont().c_str(), 12);
	setFormSetter(ment_label);
	ment_label->setPosition(ccp(0,5));
	t_container->addChild(ment_label);
	
	case_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+60+80, ment_label->getContentSize().height + 40+80));
	content_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+40+85, ment_label->getContentSize().height+76));
	title_label->setPosition(ccp(0,case_back->getContentSize().height/2-21));
	
	CommonButton* close_button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ok), 15, CCSizeMake(70, 40), CommonButtonLightPupple, t_popup->getTouchPriority()-5);
	setFormSetter(close_button);
	close_button->setPosition(ccp(0,case_back->getContentSize().height/2.f*-1+20+18));
	close_button->setFunction([=](CCObject* sender)
														{
															AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
															close_func();
															t_popup->removeFromParent();
														});
	t_container->addChild(close_button);
	
	return t_popup;
}


ASPopupView* ASPopupView::getCommonNoti(int t_touch_priority, string t_comment, function<void()> close_func, CCPoint t_position)
{
	ASPopupView* t_popup = ASPopupView::create(t_touch_priority);
	startFormSetter(t_popup);
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
	
	if(t_position.equals(CCPointZero))
	{
		if(mySGD->is_on_maingame)
		{
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		}
		else
		{
			t_popup->setDimmedPosition(ccp(240, 160));
			t_popup->setBasePosition(ccp(240, 160));
		}
	}
	else
	{
		t_popup->setDimmedPosition(t_position);
		t_popup->setBasePosition(t_position);
	}
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	setFormSetter(case_back);
	case_back->setPosition(CCPointZero);
	t_container->addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	setFormSetter(content_back);
	content_back->setPosition(CCPointZero);
	t_container->addChild(content_back);

	
	CCLabelTTF* ment_label = CCLabelTTF::create(t_comment.c_str(), mySGD->getFont().c_str(), 12);
	setFormSetter(ment_label);
	ment_label->setPosition(ccp(0,20));
	t_container->addChild(ment_label);
	
	case_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+60+80, ment_label->getContentSize().height + 40+80));
	content_back->setContentSize(CCSizeMake(ment_label->getContentSize().width+40+85, ment_label->getContentSize().height+20+85));

	CommonButton* close_button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ok), 15, CCSizeMake(70, 40), CommonButtonLightPupple, t_popup->getTouchPriority()-5);
	setFormSetter(close_button);
	close_button->setPosition(ccp(0,case_back->getContentSize().height/2.f*-1+20+18));
	close_button->setFunction([=](CCObject* sender)
							  {
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  close_func();
								  t_popup->removeFromParent();
							  });
	t_container->addChild(close_button);
	
	return t_popup;
}

ASPopupView* ASPopupView::getNotEnoughtGoodsGoShopPopup(int t_touch_priority, GoodsType t_type, function<void()> goshop_func, function<void()> cancel_func)
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
	
	CCScale9Sprite* back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(240,140));
	back_case->setPosition(ccp(0,0));
	t_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
	back_case->addChild(back_in);
	
	string title_string;
	if(t_type == kGoodsType_ruby)
		title_string = myLoc->getLocalForKey(kMyLocalKey_rubyNotEnought);
	else if(t_type == kGoodsType_gold)
		title_string = myLoc->getLocalForKey(kMyLocalKey_goldNotEnought);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title_string.c_str(), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(-back_case->getContentSize().width/2.f + 17,back_case->getContentSize().height/2.f-25));
	t_container->addChild(title_label);
	
	KSLabelTTF* sub_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_enoughtGoodsContent), mySGD->getFont().c_str(), 12);
	sub_label->setAnchorPoint(ccp(0,0.5f));
	sub_label->setPosition(ccp(-back_case->getContentSize().width/2.f + 17,10));
	t_container->addChild(sub_label);
	
	CCSprite* gray = t_popup->getDimmedSprite();
	
	CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25,back_case->getContentSize().height/2.f-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!t_popup->is_menu_enable)
									  return;
								  
								  t_popup->is_menu_enable = false;
								  
								  t_popup->addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
									  t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.f);}));}));
								  
								  t_popup->addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
								  {
									  gray->setOpacity(t);
									  KS::setOpacity(t_container, t);
								  }, [=](int t)
								  {
									  gray->setOpacity(0);
									  KS::setOpacity(t_container, 0);
									  if(cancel_func != nullptr)
										  cancel_func();
									  t_popup->removeFromParent();
								  }));
							  });
	t_container->addChild(close_button);
	
	t_popup->button_func_list.clear();
	
	t_popup->button_func_list.push_back([=](){
		if(!t_popup->is_menu_enable)
			return;
		
		t_popup->is_menu_enable = false;
		
		t_popup->addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
			t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.f);}));}));
		
		t_popup->addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
		{
			gray->setOpacity(t);
			KS::setOpacity(t_container, t);
		}, [=](int t)
		{
			gray->setOpacity(0);
			KS::setOpacity(t_container, 0); goshop_func(); t_popup->removeFromParent();
		}));
		
	});
	
	CCLabelTTF* t2_label = CCLabelTTF::create();
	
	KSLabelTTF* goshop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_goShopButton), mySGD->getFont().c_str(), 13);
	goshop_label->setPosition(ccp(0,0));
	t2_label->addChild(goshop_label);
	
	CCScale9Sprite* goshop_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	CCControlButton* goshop_button = CCControlButton::create(t2_label, goshop_back);
	goshop_button->addTargetWithActionForControlEvents(t_popup, cccontrol_selector(ASPopupView::buttonAction), CCControlEventTouchUpInside);
	goshop_button->setTag(0);
	goshop_button->setPreferredSize(CCSizeMake(110,45));
	goshop_button->setPosition(ccp(0,-30));
	t_container->addChild(goshop_button);
	
	goshop_button->setTouchPriority(t_popup->getTouchPriority()-5);
	
	
	t_container->setScaleY(0.f);
	
	t_popup->addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
		t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.8f);
			t_popup->addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.f);}));}));}));
	
	t_popup->addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
	{
		gray->setOpacity(t);
		KS::setOpacity(t_container, t);
	}, [=](int t)
	{
		gray->setOpacity(255);
		KS::setOpacity(t_container, 255);t_popup->is_menu_enable = true;
	}));
	
	
	return t_popup;
}

void ASPopupView::buttonAction(CCObject* sender, CCControlEvent t_event)
{
	int tag = ((CCNode*)sender)->getTag();
	
	button_func_list[tag]();
}