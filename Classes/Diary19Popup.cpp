//
//  Diary19Popup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 9. 5..
//
//

#include "Diary19Popup.h"
#include "TouchSuctionLayer.h"
#include "DataStorageHub.h"
//#include "MyLocalization.h"
#include "KsLocal.h"
#include "KSLabelTTF.h"
#include "StyledLabelTTF.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "LoadingLayer.h"
#include "hspConnector.h"
#include "ASPopupView.h"

Diary19Popup* Diary19Popup::create(int t_touch_priority, function<void()> t_end_func, bool t_is_ingame)
{
	Diary19Popup* t_mup = new Diary19Popup();
	t_mup->myInit(t_touch_priority, t_end_func, t_is_ingame);
	t_mup->autorelease();
	return t_mup;
}

void Diary19Popup::myInit(int t_touch_priority, function<void()> t_end_func, bool t_is_ingame)
{
	is_menu_enable = false;
	is_ingame = t_is_ingame;
	
	if(is_ingame)
		center_position = ccp(240, myDSH->ui_center_y);
	else
		center_position = ccp(240,160);
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(center_position);
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(center_position);
	addChild(m_container);
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251,64));
	back_in->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(0,10));
	back_case->addChild(back_in);
	
	string title_str, content_str;
	title_str = myLoc->getLocalForKey(LK::kMyLocalKey_sDiary);
	content_str = myLoc->getLocalForKey(LK::kMyLocalKey_toShowAdultImgForSdiaryNeed);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title_str.c_str(), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(-85, back_case->getContentSize().height/2.f-35));
	back_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;

								  is_menu_enable = false;

								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

								  CommonAnimation::closePopup(this, m_container, gray, [=](){

								  }, [=](){
									  end_func();
									  removeFromParent();
								  });
							  });
	back_case->addChild(close_button);
	
	KSLabelTTF* content_label = KSLabelTTF::create(content_str.c_str(), mySGD->getFont().c_str(), 12);
	content_label->setAnchorPoint(ccp(0.5f,0.5f));
	content_label->setPosition(ccpFromSize(back_in->getContentSize() / 2.f));
	back_in->addChild(content_label);
	
	
//	t_loading = LoadingLayer::create(touch_priority-100);
//	addChild(t_loading, 999);
//	t_loading->startLoading();
//	
//	Json::Value mdc_param;
//	mdc_param["memberID"] = myHSP->getSocialID();
//	
//	myHSP->command("makediarycode", mdc_param, json_selector(this, Diary19Popup::resultMakeDiaryCode));
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	KSLabelTTF* rightnow_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_download), mySGD->getFont().c_str(), 13);
	rightnow_label->disableOuterStroke();
	r_label->addChild(rightnow_label);
	
	CCScale9Sprite* rightnow_back = CCScale9Sprite::create("achievement_button_success.png");
	
	CCControlButton* rightnow_button = CCControlButton::create(r_label, rightnow_back);
	rightnow_button->addTargetWithActionForControlEvents(this, cccontrol_selector(Diary19Popup::buttonAction), CCControlEventTouchUpInside);
	rightnow_button->setPreferredSize(CCSizeMake(100,44));
	rightnow_button->setPosition(ccp(0,-50));
	m_container->addChild(rightnow_button);
	
	rightnow_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
	}, [=](){
		is_menu_enable = true;
	});
}

void Diary19Popup::resultMakeDiaryCode(Json::Value result_data)
{
//	CCLog("resultMakeDiaryCode : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		t_loading->removeFromParent();
		KSLabelTTF* code_label = KSLabelTTF::create(result_data["diaryCode"].asString().c_str(), mySGD->getFont().c_str(), 18);
		code_label->setColor(ccc3(255, 170, 20));
		code_label->enableOuterStroke(ccBLACK, 1.f, 180, true);
		code_label->setPosition(ccpFromSize(back_in->getContentSize() / 2.f) + ccp(0,-20));
		back_in->addChild(code_label);
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				Json::Value mdc_param;
				mdc_param["memberID"] = myHSP->getSocialID();
				
				myHSP->command("makediarycode", mdc_param, json_selector(this, Diary19Popup::resultMakeDiaryCode));
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 Json::Value mdc_param;
										 mdc_param["memberID"] = myHSP->getSocialID();
										 
										 myHSP->command("makediarycode", mdc_param, json_selector(this, Diary19Popup::resultMakeDiaryCode));
									 }));
		}
	}
}

void Diary19Popup::buttonAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
//	myHSP->openUrl(mySGD->getDiaryDownUrl().c_str());
	myHSP->openUrl(mySGD->getDiaryStoreUrl().c_str());
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		end_func();
		removeFromParent();
	});
}