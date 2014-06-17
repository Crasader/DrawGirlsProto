//
//  CouponPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 23..
//
//

#include "CouponPopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "ASPopupView.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "FormSetter.h"
CouponPopup* CouponPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	CouponPopup* t_mup = new CouponPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void CouponPopup::myInit(int t_touch_priority, function<void()> t_end_func)
{
	startFormSetter(this);
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
	m_container->setPosition(ccp(240,220));
	addChild(m_container);
	
	
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(270,150));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	
	CommonButton* cancel_button = CommonButton::createCloseButton(touch_priority);
	cancel_button->setPosition(ccp(back_case->getContentSize().width/2.f-22,back_case->getContentSize().height/2.f-25));
	cancel_button->setFunction([=](CCObject* sender)
							   {
								   if(!is_menu_enable)
									   return;
								   
								   is_menu_enable = false;
								   input_text1->setEnabled(false);
								   input_text1->removeFromParent();
								   
								   input_text2->setEnabled(false);
								   input_text2->removeFromParent();
									 
								   input_text3->setEnabled(false);
								   input_text3->removeFromParent();
								   AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
								   
									 CommonAnimation::closePopup(this, m_container, gray, [=](){
										 
									 }, [=](){
										 end_func(); removeFromParent();
									 });
							   });
	m_container->addChild(cancel_button);
	
	
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_couponTitle), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(0,back_case->getContentSize().height/2.f-25));
	m_container->addChild(title_label);
	
	CCScale9Sprite* nick_case1 = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	nick_case1->setAnchorPoint(ccp(0.5f, 0.5f));
	nick_case1->setContentSize(CCSizeMake(70,35));
	nick_case1->setPosition(ccp(-80.f, 9.5)); 			// dt (-128.0,2.5)
	m_container->addChild(nick_case1);
	
	CCScale9Sprite* nick_case2 = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	nick_case2->setAnchorPoint(ccp(0.5f, 0.5f));
	nick_case2->setContentSize(CCSizeMake(70,35));
	nick_case2->setPosition(ccp(0.f, 10.0)); 			// dt (-60.5,3.0)
	m_container->addChild(nick_case2);
	
	CCScale9Sprite* nick_case3 = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	nick_case3->setAnchorPoint(ccp(0.5f, 0.5f));
	nick_case3->setContentSize(CCSizeMake(70,35));
	nick_case3->setPosition(ccp(80.5f,10.5)); 			// dt (7.5,3.5)
	m_container->addChild(nick_case3);

	CCSprite* seperateBar1 = CCSprite::create("coupon_bar.png");
	seperateBar1->setPosition(ccp(-40, 10.5f));
	m_container->addChild(seperateBar1);
	
	
	CCSprite* seperateBar2 = CCSprite::create("coupon_bar.png");
	seperateBar2->setPosition(ccp(40, 10.5f));
	m_container->addChild(seperateBar2);
	
	CCScale9Sprite* t_back1 = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	t_back1->setOpacity(0);
	input_text1 = CCEditBox::create(CCSizeMake(45, 35), t_back1);
	input_text1->setPosition(ccp(127.0,230.0)); 			// dt (10.0,-1.0)
//	input_text1->setPosition(ccp(117.0,231.0)); 			// dt (117.0,4.0)
	CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text1, 99999);
	
	CCScale9Sprite* t_back2 = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	t_back2->setOpacity(0);
	input_text2 = CCEditBox::create(CCSizeMake(45, 35), t_back2);
	input_text2->setPosition(ccp(206.5,230.0)); 			// dt (22.5,0.0)
	CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text2, 99999);
	
	CCScale9Sprite* t_back3 = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	t_back3->setOpacity(0);
	input_text3 = CCEditBox::create(CCSizeMake(45, 35), t_back3);
	input_text3->setPosition(ccp(286.5,230.0)); 			// dt (34.5,-3.0)
	CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text3, 99999);
	
	
	initiateEditBox(input_text1);
	initiateEditBox(input_text2);
	initiateEditBox(input_text3);

	setFormSetter(input_text1);
	setFormSetter(input_text2);
	setFormSetter(input_text3);
	
	setFormSetter(nick_case1);
	setFormSetter(nick_case2);
	setFormSetter(nick_case3);
		
	CCLabelTTF* t_label = CCLabelTTF::create();
	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
	t_label->addChild(ok_label);
	
	CCScale9Sprite* ok_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	CCControlButton* ok_button = CCControlButton::create(t_label, ok_back);
	ok_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CouponPopup::couponAction), CCControlEventTouchUpInside);
	ok_button->setPreferredSize(CCSizeMake(160,45));
	ok_button->setPosition(ccp(0,-40));
	m_container->addChild(ok_button);
	
	ok_button->setTouchPriority(touch_priority);

	CommonAnimation::openPopup(this, m_container, gray, [=](){
//		t_back->setOpacity(0);
		
	}, [=](){
//		t_back->setOpacity(0);
		is_menu_enable = true;
		input_text1->setEnabled(true);
		input_text1->setVisible(true);
		
		input_text2->setEnabled(true);
		input_text2->setVisible(true);
		input_text3->setEnabled(true);
		input_text3->setVisible(true);
		
	});
}

void CouponPopup::couponAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	loading_layer = LoadingLayer::create(touch_priority-100);
	addChild(loading_layer, 999);
	
	Json::Value param;
	std::string couponText = std::string(input_text1->getText()) + input_text2->getText() + input_text3->getText();
	param["memberID"] = myHSP->getMemberID();
	param["cuponCode"] = couponText;

	myHSP->command("usecupon", param, json_selector(this, CouponPopup::resultUseCoupon));
}

void CouponPopup::resultUseCoupon(Json::Value result_data)
{
	CCLOG("resultUseCoupon : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		createResultPopup(myLoc->getLocalForKey(kMyLocalKey_couponSuccess), myLoc->getLocalForKey(kMyLocalKey_couponGiftbox));
	}
	else if(result_data["result"]["code"].asInt() == GDALREADY)
	{
		createResultPopup(myLoc->getLocalForKey(kMyLocalKey_couponFail), myLoc->getLocalForKey(kMyLocalKey_couponAlready));
	}
	else if(result_data["result"]["code"].asInt() == GDEXPIRE)
	{
		createResultPopup(myLoc->getLocalForKey(kMyLocalKey_couponFail), myLoc->getLocalForKey(kMyLocalKey_couponExpire));
	}
	else if(result_data["result"]["code"].asInt() == GDOSERROR)
	{
		createResultPopup(myLoc->getLocalForKey(kMyLocalKey_couponFail), myLoc->getLocalForKey(kMyLocalKey_couponOserror));
	}
	else if(result_data["result"]["code"].asInt() == GDDONTFIND)
	{
		createResultPopup(myLoc->getLocalForKey(kMyLocalKey_couponFail), myLoc->getLocalForKey(kMyLocalKey_couponDontfind));
	}
	else
	{
		createResultPopup(myLoc->getLocalForKey(kMyLocalKey_couponFail), myLoc->getLocalForKey(kMyLocalKey_couponOtherError));
	}
	loading_layer->removeFromParent();
}

void CouponPopup::createResultPopup(string title, string content)
{
	input_text1->setVisible(false);
	input_text1->setEnabled(false);
	
	input_text2->setVisible(false);
	input_text2->setEnabled(false);
	
	input_text3->setVisible(false);
	input_text3->setEnabled(false);
	ASPopupView* t_popup = ASPopupView::create(touch_priority-5);
	
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
	addChild(t_popup, 999);
	
	CCScale9Sprite* back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(240,140));
//	back_case->setPosition(ccp(0,0));
	t_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(0,back_case->getContentSize().height/2.f-25));
	t_container->addChild(title_label);
	
	KSLabelTTF* sub_label = KSLabelTTF::create(content.c_str(), mySGD->getFont().c_str(), 12);
	sub_label->setAnchorPoint(ccp(0.5f,0.5f));
	sub_label->setPosition(ccp(0,10));
	t_container->addChild(sub_label);
	
	CCSprite* gray = t_popup->getDimmedSprite();
	
//	CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
//	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25,back_case->getContentSize().height/2.f-25));
//	close_button->setFunction([=](CCObject* sender)
//							  {
//								  if(!t_popup->is_menu_enable)
//									  return;
//								  
//								  t_popup->is_menu_enable = false;
//								  
//								  t_popup->addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
//									  t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.f);}));}));
//								  
//								  t_popup->addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
//																				{
//																					gray->setOpacity(t);
//																					KS::setOpacity(t_container, t);
//																				}, [=](int t)
//																				{
//																					gray->setOpacity(0);
//																					KS::setOpacity(t_container, 0);
//																					is_menu_enable = true;
//																					input_text->setVisible(true);
//																					input_text->setEnabled(true);
//																					t_popup->removeFromParent();
//																				}));
//							  });
//	t_container->addChild(close_button);
	
	t_popup->button_func_list.clear();
	
	t_popup->button_func_list.push_back([=](){
		if(!t_popup->is_menu_enable)
			return;
		
		t_popup->is_menu_enable = false;
		
		CommonAnimation::closePopup(this, t_container, gray, [=](){
			
		}, [=](){
			is_menu_enable = true;
			input_text1->setVisible(true);
			input_text1->setEnabled(true);
			
			input_text2->setVisible(true);
			input_text2->setEnabled(true);
			
			input_text3->setVisible(true);
			input_text3->setEnabled(true);
			t_popup->removeFromParent();
			
		});
		
	});
	
	CCLabelTTF* t2_label = CCLabelTTF::create();
	
	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
	ok_label->setPosition(ccp(0,0));
	t2_label->addChild(ok_label);
	
	CCScale9Sprite* ok_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	CCControlButton* ok_button = CCControlButton::create(t2_label, ok_back);
	ok_button->addTargetWithActionForControlEvents(t_popup, cccontrol_selector(ASPopupView::buttonAction), CCControlEventTouchUpInside);
	ok_button->setTag(0);
	ok_button->setPreferredSize(CCSizeMake(110,45));
	ok_button->setPosition(ccp(0,-30));
	t_container->addChild(ok_button);
	
	ok_button->setTouchPriority(t_popup->getTouchPriority()-5);
	
	
	CommonAnimation::openPopup(this, t_container, gray, [=](){
		
	}, [=](){
		t_popup->is_menu_enable = true;
	});
}

void CouponPopup::editBoxEditingDidBegin(CCEditBox* editBox)
{
	CCLOG("edit begin");
}
void CouponPopup::editBoxEditingDidEnd(CCEditBox* editBox)
{
	CCLOG("edit end");
}
void CouponPopup::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	CCLOG("edit changed : %s", text.c_str());
	if(text.size() == 4)
	{
		if(editBox == input_text1)
		{
			input_text2->touchDownAction(nullptr, CCControlEvent());
		}
		else if(editBox == input_text2)
		{
			input_text3->touchDownAction(nullptr, CCControlEvent());
		}
	}
}
void CouponPopup::editBoxReturn(CCEditBox* editBox)
{
	CCLOG("edit return");
}
void CouponPopup::initiateEditBox(CCEditBox* editbox)
{
	editbox->setAnchorPoint(ccp(0, 0.5f));
	editbox->setPlaceHolder(myLoc->getLocalForKey(kMyLocalKey_couponContent));
	editbox->setReturnType(kKeyboardReturnTypeDone);
	editbox->setFont(mySGD->getFont().c_str(), 15);
	editbox->setInputMode(kEditBoxInputModeSingleLine);
	editbox->setDelegate(this);
//	m_container->addChild(input_text);
	editbox->setTouchPriority(touch_priority);
	editbox->setMaxLength(4);
	editbox->setEnabled(false);
	editbox->setVisible(false);
}