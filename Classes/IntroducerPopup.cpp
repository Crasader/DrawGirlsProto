//
//  IntroducerPopup.cpp
//  DGproto
//
//  Created by ksoo k on 2014. 9. 13..
//
//

#include "IntroducerPopup.h"
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
#include "StyledLabelTTF.h"

IntroducerPopup* IntroducerPopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_success_func)
{
	IntroducerPopup* t_mup = new IntroducerPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_success_func);
	t_mup->autorelease();
	return t_mup;
}

void IntroducerPopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_success_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	success_func = t_success_func;
	
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
	
	
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	
	CommonButton* cancel_button = CommonButton::createCloseButton(touch_priority);
	cancel_button->setPosition(ccp(back_case->getContentSize().width/2.f-22,back_case->getContentSize().height/2.f-25));
	cancel_button->setFunction([=](CCObject* sender)
							   {
								   if(!is_menu_enable)
									   return;
								   
//								   setBackKeyEnabled(false);
								   
								   is_menu_enable = false;
								   input_text1->setEnabled(false);
								   input_text1->removeFromParent();
								   
								   AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
								   
								   CommonAnimation::closePopup(this, m_container, gray, [=](){
									   
								   }, [=](){
									   end_func(); removeFromParent();
								   });
							   });
	m_container->addChild(cancel_button);
	
	
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_introducerTitle), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	StyledLabelTTF* ment_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_introducerContent), mySGD->getIntroduceStage(), 5), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	ment_label->setAnchorPoint(ccp(0.5f,0.5f));
//	ment_label->enableOuterStroke(ccBLACK, 0.3f, (GLubyte)50, true);
	ment_label->setPosition(ccp(0,30));
	m_container->addChild(ment_label);
	
	
	CCScale9Sprite* nick_case1 = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	nick_case1->setAnchorPoint(ccp(0.5f, 0.5f));
	nick_case1->setContentSize(CCSizeMake(212,35));
	nick_case1->setPosition(ccpFromSize(m_container->getContentSize() / 2.f).x, -5);
	m_container->addChild(nick_case1);
	
	//	CCScale9Sprite* nick_case2 = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	//	nick_case2->setAnchorPoint(ccp(0.5f, 0.5f));
	//	nick_case2->setContentSize(CCSizeMake(70,35));
	//	nick_case2->setPosition(ccp(0.f, 0)); 			// dt (-60.5,3.0)
	//	m_container->addChild(nick_case2);
	//
	//	CCScale9Sprite* nick_case3 = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	//	nick_case3->setAnchorPoint(ccp(0.5f, 0.5f));
	//	nick_case3->setContentSize(CCSizeMake(70,35));
	//	nick_case3->setPosition(ccp(87.f,0)); 			// dt (7.5,3.5)
	//	m_container->addChild(nick_case3);
	
	//	CCSprite* seperateBar1 = CCSprite::create("coupon_bar.png");
	//	seperateBar1->setPosition(ccp(-43, 0.f));
	//	m_container->addChild(seperateBar1);
	//
	//
	//	CCSprite* seperateBar2 = CCSprite::create("coupon_bar.png");
	//	seperateBar2->setPosition(ccp(43, 0.f));
	//	m_container->addChild(seperateBar2);
	
	CCScale9Sprite* t_back1 = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	t_back1->setOpacity(0);
	input_text1 = CCEditBox::create(CCSizeMake(190.f, 35), t_back1);
	input_text1->setPosition(ccp(234.5f,219.f-5.f));
	CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text1, 99999);
	
	//	CCScale9Sprite* t_back2 = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	//	t_back2->setOpacity(0);
	//	input_text2 = CCEditBox::create(CCSizeMake(55, 35), t_back2);
	//	input_text2->setPosition(ccp(209.5 + 30.f,219.f)); 			// dt (22.5,0.0)
	//	CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text2, 99999);
	//
	//	CCScale9Sprite* t_back3 = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	//	t_back3->setOpacity(0);
	//	input_text3 = CCEditBox::create(CCSizeMake(55, 35), t_back3);
	//	input_text3->setPosition(ccp(289.5 + 37.f,219.f)); 			// dt (34.5,-3.0)
	//	CCDirector::sharedDirector()->getRunningScene()->getChildByTag(1)->addChild(input_text3, 99999);
	
	
	initiateEditBox(input_text1);
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_introducerOk), mySGD->getFont().c_str(), 12);
	ok_label->disableOuterStroke();
	t_label->addChild(ok_label);
	
	CCScale9Sprite* ok_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	CCControlButton* ok_button = CCControlButton::create(t_label, ok_back);
	ok_button->addTargetWithActionForControlEvents(this, cccontrol_selector(IntroducerPopup::couponAction), CCControlEventTouchUpInside);
	ok_button->setPreferredSize(CCSizeMake(101,44));
	ok_button->setPosition(ccp(0,-50));
	m_container->addChild(ok_button);
	
	ok_button->setTouchPriority(touch_priority);
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		//		t_back->setOpacity(0);
		
	}, [=](){
		//		t_back->setOpacity(0);
		is_menu_enable = true;
		input_text1->setEnabled(true);
		input_text1->setVisible(true);
		
//		setBackKeyFunc([=](){
//			if(!is_menu_enable)
//				return;
//			
//			is_menu_enable = false;
//			input_text1->setEnabled(false);
//			input_text1->removeFromParent();
//			
//			AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
//			
//			CommonAnimation::closePopup(this, m_container, gray, [=](){
//				
//			}, [=](){
//				end_func(); removeFromParent();
//			});
//		});
//		
//		setBackKeyEnabled(true);
//		
//		setKeypadEnabled(true);
		
		//		input_text2->setEnabled(true);
		//		input_text2->setVisible(true);
		//		input_text3->setEnabled(true);
		//		input_text3->setVisible(true);
		
	});
}

void IntroducerPopup::couponAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
//	setBackKeyEnabled(false);
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	if(string(input_text1->getText()) == "")
	{
		input_text1->setVisible(false);
		addChild(ASPopupView::getCommonNoti(touch_priority-100, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_introducerPleaseInputNick), [=]()
		{
			is_menu_enable = true;
			input_text1->setVisible(true);
//			setBackKeyEnabled(true);
		}), 999);
		return;
	}
	else if(string(input_text1->getText()) == myDSH->getStringForKey(kDSH_Key_nick))
	{
		input_text1->setVisible(false);
		addChild(ASPopupView::getCommonNoti(touch_priority-100, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_introducerSelfNotIntroduce), [=]()
		{
			is_menu_enable = true;
			input_text1->setVisible(true);
//			setBackKeyEnabled(true);
		}), 999);
		return;
	}
	
	input_text1->setVisible(false);
	loading_layer = LoadingLayer::create(touch_priority-100);
	addChild(loading_layer, 999);
	
	Json::Value param;
	param["nick"] = input_text1->getText();
	myHSP->command("getuserdata", param, json_selector(this, IntroducerPopup::resultUserData));
}

void IntroducerPopup::resultUserData(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		Json::Value param;
		param["memberID"] = myHSP->getSocialID();
		param["nick"] = input_text1->getText();
		param["content"] = myLoc->getLocalForKey(LK::kMyLocalKey_introducerInputReward);
		myHSP->command("saveIntroducer", param, json_selector(this, IntroducerPopup::resultSaveIntroducer));
	}
	else if(result_data["result"]["code"].asInt() == GDDONTFIND)
	{
		mySGD->network_check_cnt = 0;
		
		addChild(ASPopupView::getCommonNoti(-99999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_introducerDonFindNick), [=]()
		{
			is_menu_enable = true;
			input_text1->setVisible(true);
//			setBackKeyEnabled(true);
		}), 9999);
		
		loading_layer->removeFromParent();
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				string input_data = input_text1->getText();
				
				Json::Value t_param;
				t_param["nick"] = input_data;
				myHSP->command("getuserdata", t_param, json_selector(this, IntroducerPopup::resultUserData));
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 string input_data = input_text1->getText();
										 
										 Json::Value t_param;
										 t_param["nick"] = input_data;
										 myHSP->command("getuserdata", t_param, json_selector(this, IntroducerPopup::resultUserData));
									 }));
		}
	}
}

void IntroducerPopup::resultSaveIntroducer(Json::Value result_data)
{
	if(result_data["result"]["code"] == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		loading_layer->removeFromParent();
		
		mySGD->setIntroducerID(result_data["introducerID"].asString());
		
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_introducerSuccess), input_text1->getText()), [=]()
											{
												is_menu_enable = false;
												input_text1->setEnabled(false);
												input_text1->removeFromParent();
												
												CommonAnimation::closePopup(this, m_container, gray, [=](){
													
												}, [=](){
													success_func(); removeFromParent();
												});
											}), 9999);
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				Json::Value param;
				param["memberID"] = myHSP->getSocialID();
				param["nick"] = input_text1->getText();
				param["content"] = myLoc->getLocalForKey(LK::kMyLocalKey_introducerInputReward);
				myHSP->command("saveIntroducer", param, json_selector(this, IntroducerPopup::resultSaveIntroducer));
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 Json::Value param;
										 param["memberID"] = myHSP->getSocialID();
										 param["nick"] = input_text1->getText();
										 param["content"] = myLoc->getLocalForKey(LK::kMyLocalKey_introducerInputReward);
										 myHSP->command("saveIntroducer", param, json_selector(this, IntroducerPopup::resultSaveIntroducer));
									 }));
		}
	}
}

void IntroducerPopup::resultUseCoupon(Json::Value result_data)
{
	CCLOG("resultUseCoupon : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		createResultPopup(myLoc->getLocalForKey(LK::kMyLocalKey_couponSuccess), myLoc->getLocalForKey(LK::kMyLocalKey_couponGiftbox), true);
	}
	else if(result_data["result"]["code"].asInt() == GDALREADY)
	{
		createResultPopup(myLoc->getLocalForKey(LK::kMyLocalKey_couponFail), myLoc->getLocalForKey(LK::kMyLocalKey_couponAlready));
	}
	else if(result_data["result"]["code"].asInt() == GDEXPIRE)
	{
		createResultPopup(myLoc->getLocalForKey(LK::kMyLocalKey_couponFail), myLoc->getLocalForKey(LK::kMyLocalKey_couponExpire));
	}
	else if(result_data["result"]["code"].asInt() == GDOSERROR)
	{
		createResultPopup(myLoc->getLocalForKey(LK::kMyLocalKey_couponFail), myLoc->getLocalForKey(LK::kMyLocalKey_couponOserror));
	}
	else if(result_data["result"]["code"].asInt() == GDDONTFIND)
	{
		createResultPopup(myLoc->getLocalForKey(LK::kMyLocalKey_couponFail), myLoc->getLocalForKey(LK::kMyLocalKey_couponDontfind));
	}
	else
	{
		createResultPopup(myLoc->getLocalForKey(LK::kMyLocalKey_couponFail), myLoc->getLocalForKey(LK::kMyLocalKey_couponOtherError));
	}
	loading_layer->removeFromParent();
}

void IntroducerPopup::createResultPopup(string title, string content, bool is_success)
{
	input_text1->setVisible(false);
	input_text1->setEnabled(false);
	
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
	
	CCSprite* back_case = CCSprite::create("popup_small_back.png");
	t_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	//	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-45, back_case->getContentSize().height-82));
	back_in->setContentSize(CCSizeMake(502 / 2.f, 226 / 2.f));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, 83/*back_case->getContentSize().height/2.f-9*/));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	t_container->addChild(title_label);
	
	KSLabelTTF* sub_label = KSLabelTTF::create(content.c_str(), mySGD->getFont().c_str(), 13);
	sub_label->setColor(ccc3(255, 170, 20));
	sub_label->disableOuterStroke();
	sub_label->setAnchorPoint(ccp(0.5f,0.5f));
	sub_label->setPosition(ccpFromSize(back_in->getContentSize()/2.f) + ccp(0, 5));
	back_in->addChild(sub_label);
	
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
	
	CommonButton* t_close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
	t_close_button->setPosition(ccp(back_case->getContentSize().width/2.f-22,back_case->getContentSize().height/2.f-25));
	t_close_button->setFunction([=](CCObject* sender)
								{
									if(!t_popup->is_menu_enable)
										return;
									
									t_popup->is_menu_enable = false;
									
									AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
									
									CommonAnimation::closePopup(this, t_container, gray, [=](){
										
									}, [=](){
										if(is_success)
										{
											input_text1->setEnabled(false);
											input_text1->removeFromParent();
											
											
											CommonAnimation::closePopup(this, m_container, this->gray, [=](){
												
											}, [=](){
												//												end_func();
												success_func();
												removeFromParent();
											});
										}
										else
										{
											input_text1->setVisible(true);
											input_text1->setEnabled(true);
											
											this->is_menu_enable = true;
                                            t_popup->removeFromParent();
										}
										
										
									});
								});
	t_container->addChild(t_close_button);
	
	//	t_popup->button_func_list.clear();
	//
	//	t_popup->button_func_list.push_back([=](){
	//		if(!t_popup->is_menu_enable)
	//			return;
	//
	//		t_popup->is_menu_enable = false;
	//
	//		AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	//
	//		CommonAnimation::closePopup(this, t_container, gray, [=](){
	//
	//		}, [=](){
	//			if(is_success)
	//			{
	//				input_text1->setEnabled(false);
	//				input_text1->removeFromParent();
	//
	//				input_text2->setEnabled(false);
	//				input_text2->removeFromParent();
	//
	//				input_text3->setEnabled(false);
	//				input_text3->removeFromParent();
	//
	//				CommonAnimation::closePopup(this, m_container, this->gray, [=](){
	//
	//				}, [=](){
	//					end_func();
	//					success_func();
	//					removeFromParent();
	//				});
	//			}
	//			else
	//			{
	//				input_text1->setVisible(true);
	//				input_text1->setEnabled(true);
	//
	//				input_text2->setVisible(true);
	//				input_text2->setEnabled(true);
	//
	//				input_text3->setVisible(true);
	//				input_text3->setEnabled(true);
	//
	//				this->is_menu_enable = true;
	//			}
	//
	//			t_popup->removeFromParent();
	//
	//		});
	//
	//	});
	//
	//	CCLabelTTF* t2_label = CCLabelTTF::create();
	//
	//	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
	//	ok_label->setPosition(ccp(0,0));
	//	t2_label->addChild(ok_label);
	//
	//	CCScale9Sprite* ok_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	//
	//	CCControlButton* ok_button = CCControlButton::create(t2_label, ok_back);
	//	ok_button->addTargetWithActionForControlEvents(t_popup, cccontrol_selector(ASPopupView::buttonAction), CCControlEventTouchUpInside);
	//	ok_button->setTag(0);
	//	ok_button->setPreferredSize(CCSizeMake(110,45));
	//	ok_button->setPosition(ccp(0,-30));
	//	t_container->addChild(ok_button);
	//
	//	ok_button->setTouchPriority(t_popup->getTouchPriority()-5);
	
	
	CommonAnimation::openPopup(this, t_container, gray, [=](){
		
	}, [=](){
		t_popup->is_menu_enable = true;
	});
}

void IntroducerPopup::editBoxEditingDidBegin(CCEditBox* editBox)
{
	CCLOG("edit begin");
}
void IntroducerPopup::editBoxEditingDidEnd(CCEditBox* editBox)
{
	CCLOG("edit end");
}
void IntroducerPopup::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	CCLOG("edit changed : %s", text.c_str());
	//	if(text.size() >= )
	//	{
	//		if(editBox == input_text1)
	//		{
	//			input_text2->touchDownAction(nullptr, CCControlEvent());
	//		}
	//		else if(editBox == input_text2)
	//		{
	//			input_text3->touchDownAction(nullptr, CCControlEvent());
	//		}
	//	}
}
void IntroducerPopup::editBoxReturn(CCEditBox* editBox)
{
	CCLOG("edit return");
}
void IntroducerPopup::initiateEditBox(CCEditBox* editbox)
{
	editbox->setAnchorPoint(ccp(0.5f, 0.5f));
	editbox->setPlaceHolder(myLoc->getLocalForKey(LK::kMyLocalKey_introducerPlaceHolder));
	editbox->setReturnType(kKeyboardReturnTypeDone);
	editbox->setFont(mySGD->getFont().c_str(), 15);
	editbox->setInputMode(kEditBoxInputModeSingleLine);
	editbox->setInputFlag(cocos2d::extension::EditBoxInputFlag::kEditBoxInputFlagInitialCapsAllCharacters);
	editbox->setDelegate(this);
	//	m_container->addChild(input_text);
	editbox->setTouchPriority(touch_priority);
	editbox->setMaxLength(12);
	editbox->setEnabled(false);
	editbox->setVisible(false);
}

void IntroducerPopup::keyBackClicked(void)
{
//	onBackKeyAction();
}