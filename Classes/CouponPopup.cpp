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

CouponPopup* CouponPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	CouponPopup* t_mup = new CouponPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void CouponPopup::myInit(int t_touch_priority, function<void()> t_end_func)
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
	m_container->setPosition(ccp(240,220));
	addChild(m_container);
	
	
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(270,150));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* t_back = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	t_back->setOpacity(0);
	
	
	CommonButton* cancel_button = CommonButton::createCloseButton(touch_priority);
	cancel_button->setPosition(ccp(back_case->getContentSize().width/2.f-22,back_case->getContentSize().height/2.f-25));
	cancel_button->setFunction([=](CCObject* sender)
							   {
								   if(!is_menu_enable)
									   return;
								   
								   is_menu_enable = false;
								   this->input_text->setEnabled(false);
								   this->input_text->removeFromParent();
								   
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
	
	CCScale9Sprite* nick_case = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	nick_case->setContentSize(CCSizeMake(236,35));
	nick_case->setPosition(ccp(0,7));
	m_container->addChild(nick_case);
	
	
	input_text = CCEditBox::create(CCSizeMake(200, 35), t_back);
	input_text->setPosition(ccp(240,227));
	input_text->setPlaceHolder(myLoc->getLocalForKey(kMyLocalKey_couponContent));
	input_text->setReturnType(kKeyboardReturnTypeDone);
	input_text->setFont(mySGD->getFont().c_str(), 15);
	input_text->setInputMode(kEditBoxInputModeSingleLine);
	input_text->setDelegate(this);
//	m_container->addChild(input_text);
	input_text->setTouchPriority(touch_priority);
	input_text->setEnabled(false);
	input_text->setVisible(false);
	
	CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text, 99999);
	
		
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

	t_back->setOpacity(0);
	CommonAnimation::openPopup(this, m_container, gray, [=](){
//		t_back->setOpacity(0);
		
	}, [=](){
//		t_back->setOpacity(0);
		is_menu_enable = true;
		input_text->setEnabled(true);
		input_text->setVisible(true);
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
	param["memberID"] = myHSP->getMemberID();
	param["cuponCode"] = input_text->getText();
	
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
	input_text->setVisible(false);
	input_text->setEnabled(false);
	
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
	back_case->setPosition(ccp(0,0));
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
		
		CommonAnimation::closePopup(this, m_container, gray, [=](){
			
		}, [=](){
			is_menu_enable = true;
			input_text->setVisible(true);
			input_text->setEnabled(true);
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
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
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
}
void CouponPopup::editBoxReturn(CCEditBox* editBox)
{
	CCLOG("edit return");
}