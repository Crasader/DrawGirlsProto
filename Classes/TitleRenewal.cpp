//
//  TitleRenewal.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 9..
//
//

#include "TitleRenewal.h"
#include "StarGoldData.h"
#include "utf8.h"
#include "KSUtil.h"
#include <chrono>
#include "MainFlowScene.h"
#include "TutorialFlowStep.h"
#include "CommonButton.h"
#include "NewMainFlowScene.h"
#include "StoryView.h"
#include "ASPopupView.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "KSLabelTTF.h"
#include <random>
#include "AudioEngine.h"
#include "FormSetter.h"
#include "AchieveData.h"
#include "Terms.h"
#include "ConvexGraph.h"
#include <algorithm>
#include "FiveRocksCpp.h"
#include "TypingBox.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "HSPCore.h"
#endif
CCScene* TitleRenewalScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	TRACE();
	TitleRenewalScene *layer = TitleRenewalScene::create();
	TRACE();
	layer->setAnchorPoint(ccp(0.5,0));
	TRACE();
	layer->setScale(myDSH->screen_convert_rate);
	TRACE();
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	// add layer as a child to scene
	TRACE();
	scene->addChild(layer);
	TRACE();
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TitleRenewalScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	mySGD->ui_scene_code = kUISceneCode_empty;
	
	TRACE();
	is_preloaded_effect = false;
	
	card_data_version = -1;
	hell_data_version = -1;
	
	is_downloading = false;
	
	loginCnt=0;
//	std::chrono::time_point<std::chrono::system_clock> recent;
//    recent = std::chrono::system_clock::now();
//	std::time_t recent_time = std::chrono::system_clock::to_time_t(recent);
//	CCLOG("title init : %d", int(recent_time));
	
	AudioEngine::sharedInstance()->stopSound();
	
	is_menu_enable = false;
	white_back = NULL;
	TRACE();
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_init)
	{
		white_back = CCSprite::create("whitePaper.png");
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		white_back->setScaleX(screen_scale_x);
		white_back->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		white_back->setPosition(ccp(240,160));
		addChild(white_back);
		
		CCSprite* splash = CCSprite::create("splash_nhn_litqoo.png");
		splash->setPosition(ccp(240,160));
		addChild(splash);
		TRACE();
		
		addChild(KSTimer::create(1.f, [=]()
														 {TRACE();
															 
			addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																						 {TRACE();
																							 
													   splash->setOpacity(255-255*t);
												   }, [=](float t)
																						 {TRACE();
																							 
													   splash->setOpacity(255-255*t);
													   splash->removeFromParent();
													   
													   CCSprite* marvelous_splash = CCSprite::create("splash_marvelous.png");
													   marvelous_splash->setPosition(ccp(240,160));
													   addChild(marvelous_splash);
													   
													   addChild(KSTimer::create(1.f, [=]()
																											{TRACE();
																												
																					addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																																								 {TRACE();
																																									 
																															   marvelous_splash->setOpacity(255-255*t);
																														   }, [=](float t)
																																								 {TRACE();
																																									 
																															   marvelous_splash->setOpacity(255-255*t);
																															   marvelous_splash->removeFromParent();
																															   
																															   endSplash();
																														   }));
																				}));
												   }));
		}));
		
//		auto splash = KS::loadCCBI<CCSprite*>(this, "splash_nhn.ccbi");
//		splash.second->setAnimationCompletedCallbackLambda(this, [=](const char* seqName){
//			splash.first->removeFromParent();
//			endSplash();
//		});
//		splash.first->setPosition(ccp(240,160));
//		
//		addChild(splash.first);
//		
//		//	addChild(KSTimer::create(3.f, [=]()
//		//	{
//		splash.second->runAnimationsForSequenceNamed("Default Timeline");
		//		//	}));
		TRACE();
	}
	else
	{
		TRACE();
		endSplash();
	}
	
//	vector<string> preload_list;
//	preload_list.clear();
//	
//	for(int i=0;i<=26;i++)
//		preload_list.push_back(CCString::createWithFormat("TOAST_GalaxyS3_cropA_%05d.png", i)->getCString());
//	preload_list.push_back("TOAST_GalaxyS3_typeA_1280x720_all.png");
//	preload_list.push_back("TOAST_GalaxyS3_typeA_1280x720_BG.png");
//	
//	splash_load_cnt = preload_list.size();
//	splash_ing_cnt = 0;
//	
//	for(int i=0;i<splash_load_cnt;i++)
//		CCTextureCache::sharedTextureCache()->addImageAsync(preload_list[i].c_str(), this, callfuncO_selector(TitleRenewalScene::loadCounting));
	
	
	
//	Json::Value param;
//	param["memberID"] = 88899626759589914L;
//	param["error"]["isSuccess"] = true;
//	GraphDog::get()->setKakaoMemberID("88899626759589914");
//	GraphDog::get()->setHSPMemberNo(88899626759589914L);
//	resultLogin(param);
	
	
	
//	Json::Value t_result_data;
//	hspConnector::get()->myKakaoInfo["user_id"] = 88741857374149376L;
//	hspConnector::get()->myKakaoInfo["nickname"] = "YH";
//	graphdog->setKakaoMemberID(hspConnector::get()->getSocialID());
//	t_result_data["error"]["isSuccess"] = true;
//	resultLogin(t_result_data);
	
	return true;
}

void TitleRenewalScene::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
	
//	std::chrono::time_point<std::chrono::system_clock> recent;
//    recent = std::chrono::system_clock::now();
//	std::time_t recent_time = std::chrono::system_clock::to_time_t(recent);
//	CCLOG("title onEnter Finish : %d", int(recent_time));
}

void TitleRenewalScene::loadCounting(CCObject* sender)
{
	splash_ing_cnt++;
	
	if(splash_ing_cnt == splash_load_cnt)
	{
//		auto splash = KS::loadCCBI<CCSprite*>(this, "splash_nhn.ccbi");
//		splash.second->setAnimationCompletedCallbackLambda(this, [=](){
//			splash.first->removeFromParent();
//			endSplash();
//		});
//		splash.first->setPosition(ccp(240,160));
//		
//		addChild(splash.first);
//		
//		//	addChild(KSTimer::create(3.f, [=]()
//		//	{
//		splash.second->runAnimationsForSequenceNamed("Default Timeline");
//		//	}));
	}
}

void TitleRenewalScene::endSplash()
{
	TRACE();
	
//	setBackKeyFunc([=](){
//		AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(TitleRenewalScene::alertAction));
//	});
//	
//	setBackKeyEnabled(true);
	
	setKeypadEnabled(true);
	
//	CCSprite* ratings = CCSprite::create("game_ratings.png");
//	ratings->setPosition(ccp(240,160));
//	addChild(ratings);
//	
//	addChild(KSTimer::create(1.5f, [=]()
//	{
//		realInit();
//	}));
	
	auto t_title = KS::loadCCBI<CCSprite*>(this, "title_01.ccbi");
	title_img = t_title.first;
	title_img->setPosition(ccp(240,160));
	addChild(title_img);
	
	title_manager = t_title.second;
	
	title_manager->runAnimationsForSequenceNamed("Default Timeline");
//	title_img = CCSprite::create("temp_title_back.png");
//	title_img->setPosition(ccp(240,160));
//	addChild(title_img);
//	
//	title_name = CCSprite::create("temp_title_name.png");
//	title_name->setAnchorPoint(ccp(0.5,0));
//	title_name->setPosition(ccp(240,50));//10));
//	addChild(title_name, 1);
	
	CCPoint convert_position = CCPointZero;
	
	convert_position.x = (480/myDSH->screen_convert_rate - 480)/2.f;
	if(myDSH->ui_top > 320)
		convert_position.y = (myDSH->ui_top - 320)/2.f;
	
	CCSprite* ratings = CCSprite::create("game_ratings.png");
	ratings->setAnchorPoint(ccp(1,1));
	ratings->setPosition(ccpFromSize(title_img->getContentSize()/2.f) + ccp(240-10, 160-10) + convert_position);
	title_img->addChild(ratings);
	
	state_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_connectingServer), mySGD->getFont().c_str(), 15, CCSizeMake(350, 80), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
	state_label->enableOuterStroke(ccBLACK, 1.f);
	state_label->setPosition(ccp(240,30)); // 190
	addChild(state_label, 2);
	
	CCSequence* t_seq = CCSequence::create(CCDelayTime::create(0.4f), CCHide::create(), CCDelayTime::create(0.4f), CCShow::create(), NULL);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
	state_label->runAction(t_repeat);
	
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	TRACE();
	
	black_img = CCSprite::create("whitePaper.png");
	black_img->setColor(ccBLACK);
	black_img->setOpacity(0);
	black_img->setPosition(ccp(240,160));
	black_img->setScaleX(screen_scale_x);
	black_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(black_img, 3);
	
	if(white_back)
		white_back->removeFromParent();
	
	if(myDSH->getBoolForKey(kDSH_Key_isCheckTerms))
	{
		TRACE();
		
		realInit();
	}
	else
	{
		termsFunctor = [=](Json::Value result_data)
		{
			TRACE();
			
			GraphDogLib::JsonToLog("isUsimKorean", result_data);
			if(!result_data["korean"].asBool()) // 내국인이면서 동의했음 or 외국인
			{
				myDSH->setBoolForKey(kDSH_Key_isCheckTerms, true);
				realInit();
			}
			else // 내국인이면서 동의안함. 꺼버리기
			{
				myHSP->getIsUsimKorean(termsFunctor);
				//									   exit(1);
			}
		};
		myHSP->getIsUsimKorean(termsFunctor);
	}
}

void TitleRenewalScene::realInit()
{
	TRACE();
	
	
	TRACE();
	Json::Value param;
	param["ManualLogin"] = true;
//	param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
	

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    #ifdef LQTEST
	TRACE();
        param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
    #else
		TRACE();
        param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSP_OAUTHPROVIDER_GAMECENTER);
    #endif
    
#else
		TRACE();
    param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#endif
	
	
//#endif
	TRACE();
	CCLOG("logintype ================ %d", param["LoginType"].asInt());
	hspConnector::get()->login(param, param, std::bind(&TitleRenewalScene::resultLogin, this, std::placeholders::_1));
}

void TitleRenewalScene::resultLogin( Json::Value result_data )
{
	TRACE();
	
	CCLOG("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	/*
	 점검시 로그j
	 {"error":{"isSuccess":false,"code":8197},"serviceprop":"08","playable":false}
	 */
	if(myDSH->getStringForKey(kDSH_Key_timeZone)==""){
		string tz = myHSP->getTimeZone();
		myDSH->setStringForKey(kDSH_Key_timeZone, tz);
		CCLOG("saved time zone : ",tz.c_str());
	}
		

	
	if(result_data["error"]["isSuccess"].asBool())
	{
		
		if(myDSH->getStringForKey(kDSH_Key_savedMemberID) == "")
		{
			myDSH->setStringForKey(kDSH_Key_savedMemberID, myHSP->getSocialID());
		}
		else
		{
			if(myHSP->getSocialID() != myDSH->getStringForKey(kDSH_Key_savedMemberID))
			{
				CCLOG("resetalldata");
//				setBackKeyEnabled(false);
				
				bool is_check_terms = myDSH->getBoolForKey(kDSH_Key_isCheckTerms);
				bool is_bgm_off = myDSH->getBoolForKey(kDSH_Key_bgmOff);
				bool is_effect_off = myDSH->getBoolForKey(kDSH_Key_effectOff);
				int control_joystick_direction = myDSH->getIntegerForKey(kDSH_Key_controlJoystickDirection);
				bool is_joystick_center_not_fixed = myDSH->getBoolForKey(kDSH_Key_isJoystickCenterNotFixed);
				int joystick_size = myDSH->getIntegerForKey(kDSH_Key_joystickSize);
				bool is_off_joystick_vib = myDSH->getBoolForKey(kDSH_Key_isOffJoystickVib);
				bool is_enable_irregular_direction = myDSH->getBoolForKey(kDSH_Key_isEnableIrregularDirection);
				bool is_safety_mode = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
				bool is_push_off = myDSH->getBoolForKey(kDSH_Key_isPushOff);
				bool is_message_off = myDSH->getBoolForKey(kDSH_Key_isMessageOff);
				
				SaveData::sharedObject()->resetAllData();
				myDSH->removeCache();
				mySDS->removeCache();
				myDSH->setIntegerForKey(kDSH_Key_clientVersion, mySGD->client_version);
				
				myDSH->setBoolForKey(kDSH_Key_bgmOff, is_bgm_off, false);
				myDSH->setBoolForKey(kDSH_Key_effectOff, is_effect_off, false);
				myDSH->setIntegerForKey(kDSH_Key_controlJoystickDirection, control_joystick_direction, false);
				myDSH->setBoolForKey(kDSH_Key_isJoystickCenterNotFixed, is_joystick_center_not_fixed, false);
				myDSH->setIntegerForKey(kDSH_Key_joystickSize, joystick_size, false);
				myDSH->setBoolForKey(kDSH_Key_isOffJoystickVib, is_off_joystick_vib, false);
				myDSH->setBoolForKey(kDSH_Key_isEnableIrregularDirection, is_enable_irregular_direction, false);
				myDSH->setBoolForKey(kDSH_Key_isSafetyMode, is_safety_mode, false);
				myDSH->setBoolForKey(kDSH_Key_isPushOff, is_push_off, false);
				myDSH->setBoolForKey(kDSH_Key_isMessageOff, is_message_off, false);
				myDSH->setBoolForKey(kDSH_Key_isCheckTerms, is_check_terms);
				
                GraphDog::get()->removeTarget(this);
				CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
				return;
			}
		}
		
		
		Json::Value param;
		param["memberID"] = hspConnector::get()->getSocialID();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#ifdef LQTEST
		param["loginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#else
		param["loginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSP_OAUTHPROVIDER_GAMECENTER);
#endif
		
#else
		param["loginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#endif
		hspConnector::get()->command("login", param, json_selector(this, TitleRenewalScene::resultHSLogin));
	}
	else
	{
		CCLog("login error = %s", result_data["error"].get("localizedDescription", "NONE_LOCAL").asString().c_str());

		TRACE();
		
		auto tryLogin = [=](){
			if(result_data["error"].get("localizedDescription", "").asString() == "")
			{
				Json::Value param;
				param["ManualLogin"] = true;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#ifdef LQTEST
				param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#else
				param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSP_OAUTHPROVIDER_GAMECENTER);
#endif
    
#else
				TRACE();
				param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#endif
//				param["LoginType"] = (int)HSPLogin::GUEST;
				hspConnector::get()->login(param, param, std::bind(&TitleRenewalScene::resultLogin, this, std::placeholders::_1));
			}
			else
			{
				Json::Value param;
				param["ManualLogin"] = true;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#ifdef LQTEST
				param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#else
				param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSP_OAUTHPROVIDER_GAMECENTER);
#endif
    
#else
				param["LoginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#endif
				
				
				hspConnector::get()->login(param, param, std::bind(&TitleRenewalScene::resultLogin, this, std::placeholders::_1));
			}
		};
		
		//오류나도 3번은 자동 로그인 시도
		// 점검이면 그냥 재시도k
		if(result_data["error"]["code"].asInt() == 8197)
		{
			addChild(KSTimer::create(3, tryLogin));
		}
		// 점검이 아니면 그냥 재시도 계속...
		else
		{
			if(loginCnt<3){
				this->loginCnt++;
				CCLOG("failed login , try login %d",loginCnt+1);
				addChild(KSTimer::create(3, tryLogin));
			}
			else{
				loginCnt=0;
				ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert2),
																												tryLogin);
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
			}
		}
		
	}
}

void TitleRenewalScene::resultHSLogin(Json::Value result_data)
{
	GraphDogLib::JsonToLog("resultHSLogin", result_data);
	TRACE();
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		successLogin();
	}
	else if(result_data["result"]["code"].asInt() == GDNEEDJOIN)
	{
		is_menu_enable = true;
		
		state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_connectingServer));
		state_label->stopAllActions();
		state_label->setVisible(false);
		
		
		nick_back = CCSprite::create("popup_small_back.png");
		nick_back->setPosition(ccp(240,220));
		addChild(nick_back,100);

		KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_nickTitle), mySGD->getFont().c_str(), 12);
		title_label->disableOuterStroke();
		title_label->setPosition(ccpFromSize(nick_back->getContentSize()/2.f) + ccp(-85, nick_back->getContentSize().height/2.f-35));
		nick_back->addChild(title_label);
		
		CCScale9Sprite* flag_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		flag_back->setContentSize(CCSizeMake(251, 58));
		flag_back->setPosition(ccpFromSize(nick_back->getContentSize()/2.f) + ccp(0,15));
		nick_back->addChild(flag_back);
		
		CCScale9Sprite* selector_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
		selector_back->setContentSize(CCSizeMake(184, 46));
		selector_back->setPosition(ccpFromSize(flag_back->getContentSize()/2.f) + ccp(25,0));
		flag_back->addChild(selector_back);

		CCScale9Sprite* nick_case = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		nick_case->setContentSize(CCSizeMake(251,35));
		nick_case->setPosition(ccp(nick_back->getContentSize().width/2.f,56));
		nick_back->addChild(nick_case);
		
		
		CCScale9Sprite* t_back = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		t_back->setOpacity(0);
		
		input_text = CCEditBox::create(CCSizeMake(160, 35), t_back);
		input_text->setPosition(ccp(110,56));
		input_text->setPlaceHolder(myLoc->getLocalForKey(LK::kMyLocalKey_inputPlease));
		input_text->setReturnType(kKeyboardReturnTypeDone);
		input_text->setFont(mySGD->getFont().c_str(), 13);
		input_text->setInputMode(kEditBoxInputModeSingleLine);
		input_text->setDelegate(this);
		nick_back->addChild(input_text,3);
		
		flag = FlagSelector::create();
		flag->setPosition(35,94);
		nick_back->addChild(flag,100000);
		
		CommonButton* ok_menu = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), 13, CCSizeMake(101, 44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), kCCMenuHandlerPriority);
		ok_menu->setPosition(ccp(237,56));
		ok_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kTitleRenewal_MT_nick);
								 menuAction(t_node);
							 });
		nick_back->addChild(ok_menu, 10, kTitleRenewal_MT_nick);
	
	KSLabelTTF* bottom_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_nickBottomMent), mySGD->getFont().c_str(), 9);
	bottom_label->disableOuterStroke();
	bottom_label->setColor(ccBLACK);
	bottom_label->setPosition(ccp(nick_back->getContentSize().width/2.f, 28));
	nick_back->addChild(bottom_label);
		
		TRACE();

		
	}
	else if(result_data["result"]["code"].asInt() == GDBLOCKEDUSER)
	{
		
		TRACE();

		CCLog("ffff %s",result_data["blockReason"].asString().c_str());
		CCLog("ffff %s",CCString::createWithFormat("%s \n\n %s",myLoc->getLocalForKey(LK::kMyLocalKey_blockedMsg),result_data["blockReason"].asString().c_str())->getCString());
		
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_blocked), CCString::createWithFormat("%s \n%s",myLoc->getLocalForKey(LK::kMyLocalKey_blockedMsg),result_data["blockReason"].asString().c_str())->getCString(),[](){
			//mySGD->resetLabels();
			//CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
	}
	else
	{
		TRACE();
		
		Json::Value param;
		param["memberID"] = hspConnector::get()->getSocialID();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#ifdef LQTEST
		param["loginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#else
		param["loginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSP_OAUTHPROVIDER_GAMECENTER);
#endif
		
#else
		param["loginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
#endif
		hspConnector::get()->command("login", param, json_selector(this, TitleRenewalScene::resultHSLogin));
		
//		Json::Value param;
//		param["memberID"] = hspConnector::get()->getSocialID();
//		param["loginType"] = myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);
//		hspConnector::get()->command("login", param, json_selector(this, TitleRenewalScene::resultLogin));
	}
}

void TitleRenewalScene::nextPreloadStep()
{
	TRACE();

	unschedule(schedule_selector(TitleRenewalScene::checkThreadPreload));
	
	audio_preload_step++;
	
	if(audio_preload_step > 3)
	{
		is_preloaded_effect = true;
		
		CCLOG("end preload effects !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
		
		if(is_loaded_cgp && is_loaded_server && is_preloaded_effect && is_loaded_productInfo)
		{
			CCDelayTime* t_delay = CCDelayTime::create(2.f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleRenewalScene::changeScene));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			runAction(t_seq);
		}
	}
	else
	{
		TRACE();

		state_label->setString(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_titleTempScript1)+audio_preload_step-1)));
		AudioEngine::sharedInstance()->preloadThreadAction(audio_preload_step);
		schedule(schedule_selector(TitleRenewalScene::checkThreadPreload));
	}
}

void TitleRenewalScene::checkThreadPreload()
{
	if(AudioEngine::sharedInstance()->is_thread_preloaded)
	{
		TRACE();

		nextPreloadStep();
	}
}

void TitleRenewalScene::successLogin()
{
	TRACE();

	state_label->setVisible(true);
	CCSequence* t_seq = CCSequence::create(CCDelayTime::create(0.4f), CCHide::create(), CCDelayTime::create(0.4f), CCShow::create(), NULL);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
	state_label->runAction(t_repeat);
	
	audio_preload_step = 0;
	
	addChild(KSTimer::create(1.f/60.f, [=]()
	{
//		nextPreloadStep();
		state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_titleTempScript1));
		
		addChild(KSTimer::create(1.f/60.f, [=]()
		{
			
			AudioEngine::sharedInstance()->preloadEffectTitleStep(1);
			
			addChild(KSTimer::create(20.f/60.f, [=]()
			{
				state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_titleTempScript2));
				TRACE();
				addChild(KSTimer::create(1.f/60.f, [=]()
				{
					AudioEngine::sharedInstance()->preloadEffectTitleStep(2);
					TRACE();
					addChild(KSTimer::create(1.f/60.f, [=]()
					{
						state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_titleTempScript3));
						TRACE();
						addChild(KSTimer::create(1.f/60.f, [=]()
						{
							AudioEngine::sharedInstance()->preloadEffectTitleStep(3);
							
							addChild(KSTimer::create(1.f/60.f, [=]()
							{
								is_preloaded_effect = true;
								CCLOG("end preload effects !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
								TRACE();
								CCLog("%d %d %d", is_loaded_cgp, is_loaded_server, is_preloaded_effect);
								if(is_loaded_cgp && is_loaded_server && is_preloaded_effect && is_loaded_productInfo)
								{
									CCDelayTime* t_delay = CCDelayTime::create(2.f);
									TRACE();
									CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleRenewalScene::changeScene));
									CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
									runAction(t_seq);
								}
							}));
						}));
					}));
				}));
			}));
		}));
	}));
	
//	if(myLog->getLogCount() > 0)
//	{
//		myLog->sendLog(CCString::createWithFormat("ting_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)))->getCString());
//	}
	
	AudioEngine::sharedInstance()->playEffect("ment_title.mp3");
	addChild(KSTimer::create(2.8f, [=](){AudioEngine::sharedInstance()->playSound("bgm_ui.mp3", true);}));
	
    is_loaded_productInfo = false;
    
	is_loaded_server = false;
	
	receive_cnt = 0;
	
    Json::Value request_param;
	request_param["memberID"] = hspConnector::get()->getMemberID();
	command_list.push_back(CommandParam("requestItemDelivery", request_param, json_selector(this, TitleRenewalScene::resultRequestItemDelivery)));
    
	Json::Value commonsetting_param;
	command_list.push_back(CommandParam("getcommonsetting", commonsetting_param, json_selector(this, TitleRenewalScene::resultGetCommonSetting)));
	
	Json::Value puzzleevent_param;
	command_list.push_back(CommandParam("getpuzzleevent", puzzleevent_param, json_selector(this, TitleRenewalScene::resultGetPuzzleEvent)));
	
	Json::Value achievelist_param;
	achievelist_param["version"] = NSDS_GI(kSDS_AI_version_i);
	command_list.push_back(CommandParam("getarchivementlist", achievelist_param, json_selector(this, TitleRenewalScene::resultGetAchieveList)));
	
	Json::Value shopdata_param;
	shopdata_param["version"] = NSDS_GI(kSDS_GI_shopVersion_i);
	command_list.push_back(CommandParam("getshoplist", shopdata_param, json_selector(this, TitleRenewalScene::resultGetShopList)));
	
	Json::Value hell_param;
	hell_param["version"] = NSDS_GI(kSDS_GI_hellMode_version_i);
	command_list.push_back(CommandParam("gethellmodelist", hell_param, json_selector(this, TitleRenewalScene::resultGetHellModeList)));
	
	Json::Value puzzlelist_param;
	puzzlelist_param["version"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
	command_list.push_back(CommandParam("getpuzzlelist", puzzlelist_param, json_selector(this, TitleRenewalScene::resultGetPuzzleList)));
	
	Json::Value character_param;
	character_param["version"] = NSDS_GI(kSDS_GI_characterVersion_i);
	command_list.push_back(CommandParam("getcharacterlist", character_param, json_selector(this, TitleRenewalScene::resultGetCharacterInfo)));
	
	Json::Value monster_param;
	monster_param["version"] = NSDS_GI(kSDS_GI_monsterVersion_i);
	command_list.push_back(CommandParam("getmonsterlist", monster_param, json_selector(this, TitleRenewalScene::resultGetMonsterList)));
	
	Json::Value notice_param;
	notice_param["buildNo"] = myHSP->getVersionCode();
	command_list.push_back(CommandParam("getnoticelist", notice_param, json_selector(this, TitleRenewalScene::resultGetNoticeList)));
	
	Json::Value timeevent_param;
	command_list.push_back(CommandParam("gettimeevent", timeevent_param, json_selector(this, TitleRenewalScene::resultGetTimeEvent)));
	
	Json::Value attendance_param;
	attendance_param["memberID"] = myHSP->getMemberID();
	command_list.push_back(CommandParam("checkattendenceevent", attendance_param, json_selector(this, TitleRenewalScene::resultCheckAttendanceEvent)));
	
	Json::Value rank_reward_param;
	rank_reward_param["memberID"] = myHSP->getMemberID();
	command_list.push_back(CommandParam("checkweeklyreward", rank_reward_param, json_selector(this, TitleRenewalScene::resultCheckWeeklyReward)));
	
	Json::Value achieve_param;
	achieve_param["memberID"] = myHSP->getSocialID();
	command_list.push_back(CommandParam("getarchivementhistory", achieve_param, json_selector(this, TitleRenewalScene::resultGetAchieveHistory)));
	
	Json::Value card_param;
	card_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("getCardHistory", card_param, json_selector(this, TitleRenewalScene::resultGetCardHistory)));
	
	Json::Value puzzle_param;
	puzzle_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("getPuzzleHistory", puzzle_param, json_selector(this, TitleRenewalScene::resultGetPuzzleHistory)));
	
	Json::Value piece_param;
	piece_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("getPieceHistory", piece_param, json_selector(this, TitleRenewalScene::resultGetPieceHistory)));
	
	Json::Value character_history_param;
	character_history_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("getcharacterhistory", character_history_param, json_selector(this, TitleRenewalScene::resultGetCharacterHistory)));
	
	Json::Value todaymission_param;
	todaymission_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("gettodaymission", todaymission_param, json_selector(this, TitleRenewalScene::resultGetTodayMission)));
	
	Json::Value properties_param;
	properties_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("getuserproperties", properties_param, json_selector(this, TitleRenewalScene::resultGetUserProperties)));
	
	Json::Value userdata_param;
	userdata_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("getUserData", userdata_param, json_selector(this, TitleRenewalScene::resultGetUserData)));
	
	Json::Value heart_param;
	heart_param["memberID"] = myHSP->getMemberID();
	command_list.push_back(CommandParam("getheart", heart_param, json_selector(this, TitleRenewalScene::resultGetHeart)));
	
	Json::Value timeinfo_param;
	command_list.push_back(CommandParam("gettimeinfo", timeinfo_param, json_selector(this, TitleRenewalScene::resultGetTimeInfo)));
	
	
	//		command_list.push_back(CommandParam("getpathinfo", Json::Value(), json_selector(this, TitleRenewalScene::resultGetPathInfo)));
	
	startCommand();
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    myHSP->requestProductInfos(this, json_selector(this, TitleRenewalScene::resultRequestProductInfos));
#else
    is_loaded_productInfo = true;
#endif
    
	is_loaded_cgp = false;
	
	addChild(KSTimer::create(5.f, [=]()
							 {
								 if(is_loaded_cgp)
									 return;
								 
								 is_loaded_cgp = true;
								 mySGD->cgp_data["promotionstate"] = "CGP_NONE";
								 
								 endingAction();
							 }));
	
	std::function<void(Json::Value)> pf;
	pf = [=](Json::Value v){
		KS::KSLog("CGP : %", v);
		/* 출력값
		 typecode : 1 = 버튼, 2 = 배너, 3 = 팝업, 4 = 엔딩, 5 = 무료 충전
		 버튼일 경우 eventurl, buttonurl, bubbletext 가 유효
		 배너일 경우 bannerlandurl 과 bannerporturl 이 유효
		 팝업, 엔딩, 무료충전 일 경우엔 처리모름 무시.
		 
		 }
		 */
		
		if(is_loaded_cgp)
			return;
		
		is_loaded_cgp = true;
		mySGD->cgp_data = v;
		
		endingAction();
		
//		std::string pState = v["promotionstate"].asString();
//		
//		// 아무것도 하지마!!
//		if(pState == "CGP_NONE")
//		{
//			/* 출력값
//			 {
//			 "callback" : null,
//			 "param" : null,
//			 "promotionstate" : "CGP_NONE"
//			 }
//			 
//			 */
//		}
//		// 홍보해야 될 것이 존재
//		else if(pState == "CGP_PROMOTION_EXISTS")
//		{
//			/* 출력값
//			 {
//			 "bubbletext" : "",
//			 "buttonurl" : "http://images.hangame.co.kr/mobile/cgp/10150_wara/wara_cgp.png",
//			 "callback" : null,
//			 "eventurl" : "",
//			 "param" : null,
//			 "promotionstate" : "CGP_PROMOTION_EXISTS",
//			 "typecode" : 1
//			 }
//			 
//			 */
//			CommonButton* cb = CommonButton::create("cxvlxcvmzxkclvk", 14, CCSizeMake(100, 50), CommonButtonBlue, -300);
//			cb->setPosition(ccp(240, 160));
//			addChild(cb, 999999);
//			cb->setFunction([=](CCObject* t){
//				hspConnector::get()->launchPromotion();
//				
//				cb->removeFromParent();
//			});
//		}
//		// 일반 보상
//		else if(pState == "CGP_REWARD_REQUIRED")
//		{
//			/* 출력값
//			 {
//			 "callback" : null,
//			 "param" : null,
//			 "promotionstate" : "CGP_REWARD_REQUIRED",
//			 "rewards" :
//			 [
//			 {
//			 "promotiontype" : 2,
//			 "rewardcode" : "10289_test",
//			 "rewardvalue" : 1
//			 }
//			 ]
//			 }
//			 */
//			Json::Value rewards = v["rewards"];
//			for(int i=0; i<rewards.size(); i++)
//			{
//				int rewardValue = rewards[i]["rewardvalue"].asInt();
//				CCLOG("reward !!! : %d", rewardValue);
//			}
//			
////			hspConnector::get()->checkCGP(param, Json::Value(), this, pf);
//		}
//		// 전체 팝업보상
//		else if(pState == "CGP_PROMOTION_REWARD_EXISTS")
//		{
//			Json::Value rewards = v["rewards"];
//			for(int i=0; i<rewards.size(); i++)
//			{
//				int rewardValue = rewards[i]["rewardvalue"].asInt();
//				CCLOG("reward !!! : %d", rewardValue);
//			}
//			
////			hspConnector::get()->checkCGP(param, Json::Value(), this, pf);
//		}
	};
	hspConnector::get()->checkCGP(Json::Value(), Json::Value(), this, pf);
}

void TitleRenewalScene::startCommand()
{
	is_receive_fail = false;
	receive_cnt += command_list.size();
	hspConnector::get()->command(command_list);
	command_list.clear();
}

void TitleRenewalScene::checkReceive()
{
	CCLOG("receive_cnt : %d", receive_cnt);
	if(receive_cnt == 0)
	{
		if(command_list.empty())
		{
			fiverocks::FiveRocksBridge::setUserId(myHSP->getSocialID().c_str());
			fiverocks::FiveRocksBridge::setUserLevel(mySGD->getUserdataCharLevel());
            
            int highPieceGroup = mySGD->getUserdataHighPiece();
            if(highPieceGroup!=0)highPieceGroup-1;
            highPieceGroup=highPieceGroup/5;
            fiverocks::FiveRocksBridge::setUserCohortVariable(1, ccsf("[스테이지 %d~%d]",highPieceGroup*5+1,highPieceGroup*5+5));
            int gottenCardGroup = mySGD->getHasGottenCardsSize()/10;
            fiverocks::FiveRocksBridge::setUserCohortVariable(2, ccsf("[카드보유 %d~%d]",gottenCardGroup*10,gottenCardGroup*10+9));
            int vipGroup =  mySGD->getGoodsValue(kGoodsType_money);
            
            string vipGroupString;
            
            if(vipGroup==0){
                vipGroupString="[결제01-0]";
            }else if(vipGroup<=1000){
                vipGroupString="[결제02-1~1,000]";
            }else if(vipGroup<=10000){
                vipGroupString="[결제03-1,001~10,000]";
            }else if(vipGroup<=30000){
                vipGroupString="[결제04-10,001~30,000]";
            }else if(vipGroup<=50000){
                vipGroupString="[결제05-30,001~50,000]";
            }else if(vipGroup<=100000){
                vipGroupString="[결제06-50,001~100,000]";
            }else if(vipGroup<=300000){
                vipGroupString="[결제07-100,001~300,000]";
            }else if(vipGroup<=500000){
                vipGroupString="[결제08-300,001~500,000]";
            }else if(vipGroup<=1000000){
                vipGroupString="[결제09-500,001~1,000,000]";
            }else{
                vipGroupString="[결제10-1,000,000~]";
            }
            
            fiverocks::FiveRocksBridge::setUserCohortVariable(3, vipGroupString.c_str());
			
			mySGD->network_check_cnt = 0;
			
			if(0 >= character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
			{
				endingCheck();
			}
			else
			{
				addChild(KSGradualValue<float>::create(0.f, 1.f, 0.8f, [=](float t){
					black_img->setOpacity(t*255);
				}, [=](float t){
					black_img->setOpacity(255);
					
					state_label->setPosition(ccp(240,30));//130
					
					title_img->removeFromParent();
					title_img = CCSprite::create(ccsf("loading_%d.png", rand()%4+1));
					title_img->setPosition(ccp(240,160));
					addChild(title_img);
					
					CCSprite* title_name = CCSprite::create("temp_title_name.png");
					title_name->setAnchorPoint(ccp(0.5,0));
					title_name->setPosition(ccp(240,137));//10));
					addChild(title_name, 1);
					
//					CCSprite* logo_img = CCSprite::create("temp_title_sumlanlogo.png");
//					logo_img->setPosition(ccp(475-logo_img->getContentSize().width/2.f, 315-logo_img->getContentSize().height/2.f));
//					addChild(logo_img, 1);
					
					progress_timer = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
					progress_timer->setPosition(ccp(240,80));
					addChild(progress_timer, 1);
					
					progress_timer->setCover("loading_progress_front1.png", "loading_progress_mask.png");
					progress_timer->setBack("loading_progress_back.png");
					
					
//					CCSprite* progress_back = CCSprite::create("loading_progress_back.png");
//					progress_back->setPosition(ccp(240,80));
//					addChild(progress_back, 1);
//					
//					progress_timer = CCProgressTimer::create(CCSprite::create("loading_progress_front.png"));
//					progress_timer->setType(kCCProgressTimerTypeBar);
//					progress_timer->setMidpoint(ccp(0,0));
//					progress_timer->setBarChangeRate(ccp(1,0));
//					progress_timer->setPercentage(0);
//					progress_timer->setPosition(ccp(240, 80));
//					addChild(progress_timer, 1);
					
					
					
					addChild(KSGradualValue<float>::create(1.f, 0.f, 0.8f, [=](float t){
						black_img->setOpacity(t*255);
					}, [=](float t){
						black_img->setOpacity(0);
						black_img->removeFromParent();
						
						tip_list.clear();
						
						for(int i=int(LK::kMyLocalKey_titleLoadingBegin)+1;i<int(LK::kMyLocalKey_titleLoadingEnd);i++)
						{
							tip_list.push_back(myLoc->getLocalForKey((LK)i));
						}
						
						random_shuffle(tip_list.begin(), tip_list.end());
						
						recent_tip_index = 0;
						
						state_label->setString(tip_list[recent_tip_index].c_str());
						
						addChild(KSTimer::create(4.f, [=](){changeTipMent();}));
						
						ing_download_cnt = 1;
						success_download_cnt = 0;
						ing_download_per = 0;
						
						if(!download_state)
						{
							download_state = KSLabelTTF::create("", mySGD->getFont().c_str(), 10);
							download_state->enableOuterStroke(ccBLACK, 1.f);
							download_state->setPosition(ccp(240,80));
							addChild(download_state, 2);
						}
						
						download_set.clear();
						
						is_enable_index.clear();
						for(int i=0;i<5;i++)
						{
							is_enable_index.push_back(i);
						}
						
						startFileDownloadSet();
					}));
				}));
			}
		}
		else if(is_receive_fail)
		{
			mySGD->network_check_cnt++;
			
			if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
			{
				mySGD->network_check_cnt = 0;
				
				ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
					startCommand();
				}, 1);
				if(alert)
					((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
			}
			else
			{
				addChild(KSTimer::create(0.5f, [=]()
										 {
											 startCommand();
										 }));
			}
			
//			state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_failLoadInfo));
//			
//			CommonButton* replay_menu = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, kCCMenuHandlerPriority);
//			replay_menu->setPosition(ccp(240,160));
//			replay_menu->setFunction([=](CCObject* sender)
//									 {
//										 CCNode* t_node = CCNode::create();
//										 t_node->setTag(kTitleRenewal_MT_replay);
//										 menuAction(t_node);
//									 });
//			
//			addChild(replay_menu, 0, kTitleRenewal_MT_replay);
//			
//			is_menu_enable = true;
		}
		else
		{
			startCommand();
		}
	}
}

void TitleRenewalScene::changeTipMent()
{
	recent_tip_index++;
	recent_tip_index %= int(tip_list.size());
	
	state_label->setString(tip_list[recent_tip_index].c_str());
	
	addChild(KSTimer::create(4.f, [=](){changeTipMent();}));
}

void TitleRenewalScene::resultGetPuzzleEvent(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->setEventString(result_data["data"]["title"].asString());
		mySGD->setSpecialEventPuzzleNumber(result_data["data"]["puzzleNo"].asInt());
	}
	else
	{
		is_receive_fail = true;
		command_list.push_back(CommandParam("getpuzzleevent", Json::Value(), json_selector(this, TitleRenewalScene::resultGetPuzzleEvent)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultRequestItemDelivery(Json::Value result_data)
{
    
    if(result_data["result"]["code"].asInt() == GDSUCCESS)
    {
        mySGD->initProperties(result_data["list"]);
    }else if(result_data["result"]["code"].asInt() == GDNOTINGWORK){
    
    }
    else
    {
        is_receive_fail = true;
        Json::Value request_param;
        request_param["memberID"] = myHSP->getSocialID();
        request_param["retry"] = false;
        command_list.push_back(CommandParam("requestItemDelivery", request_param, json_selector(this, TitleRenewalScene::resultRequestItemDelivery)));
    }
    
    receive_cnt--;
    checkReceive();
}
                           
void TitleRenewalScene::resultGetCommonSetting(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		if(mySGD->getAppVersion() < result_data[mySGD->getAppType()].asInt())
		{
			exit(1);
			return;
		}
		mySGD->setHeartMax(result_data["heartMax"].asInt());
		mySGD->setHeartCoolTime(result_data["heartCoolTime"].asInt());
		mySGD->setGameFriendMax(result_data["gameFriendMax"].asInt());
		mySGD->setHelpCoolTime(result_data["helpCoolTime"].asInt());
		mySGD->setChallengeCoolTime(result_data["challengeCoolTime"].asInt());
		mySGD->setMsgRemoveDay(result_data["msgRemoveDay"].asInt());
		mySGD->setGachaGoldFee(result_data["gachaGoldFee"].asInt());
		mySGD->setGachaRubyFee(result_data["gachaRubyFee"].asInt());
		mySGD->setGachaSocialFee(result_data["gachaSocialFee"].asInt());
		mySGD->setGachaGoldFeeRetry(result_data["gachaGoldFeeRetry"].asInt());
		mySGD->setGachaRubyFeeRetry(result_data["gachaRubyFeeRetry"].asInt());
		mySGD->setGachaSocialFeeRetry(result_data["gachaSocialFeeRetry"].asInt());
		mySGD->setCardUpgradeGoldFee(result_data["cardUpgradeGoldFee"].asInt());
		mySGD->setCardUpgradeRubyFee(result_data["cardUpgradeRubyFee"].asInt());
		mySGD->setHeartSendCoolTime(result_data["heartSendCoolTime"].asInt());
		mySGD->setInviteMaxADay(result_data["inviteMaxADay"].asInt());
		mySGD->setInviteCoolDay(result_data["inviteCoolDay"].asInt());
		mySGD->setPlayContinueFee(result_data["playContinueFee"].asInt());
		mySGD->setCardDurabilityUpFee(result_data["cardDurabilityUpFee"].asInt());
		mySGD->setGachaMapFee(result_data["gachaMapFee"].asInt());
		mySGD->setRemoveFriendCoolTime(result_data["removeFriendCoolTime"].asInt());
		myDSH->setDefaultGold(result_data["defaultGold"].asInt());
		myDSH->setDefaultRuby(result_data["defaultRuby"].asInt());
		mySGD->setSPSendHeart(result_data["SPSendHeart"].asInt());
		mySGD->setSPSendTicket(result_data["SPSendTicket"].asInt());
		mySGD->setSPFinishedChallenge(result_data["SPFinishedChallenge"].asInt());
		mySGD->setSPHelpChallenge(result_data["SPHelpChallenge"].asInt());
		mySGD->setSPSendBoast(result_data["SPSendBoast"].asInt());
		mySGD->setSPGetTime(result_data["SPGetTime"].asInt());
		mySGD->setSPGetHeart(result_data["SPGetHeart"].asInt());
		mySGD->setGachaOnePercentFee(result_data["gachaOnePercentFee"].asInt());
		mySGD->setSessionTime(result_data["sessionTime"].asInt());
		
		mySGD->setBonusItemCnt(kIC_doubleItem, result_data["bonusItemCntDoubleItem"].asInt());
		mySGD->setBonusItemCnt(kIC_magnet, result_data["bonusItemCntMagnet"].asInt());
		mySGD->setBonusItemCnt(kIC_baseSpeedUp, result_data["bonusItemCntBaseSpeedUp"].asInt());
		mySGD->setAiAdderOnDrewOrDamaged(result_data["aiAdderOnDrewOrDamaged"].asFloat());
		mySGD->setFuryPercent(result_data["furyPercent"].asFloat());
		mySGD->setSPRentCardThanks(result_data["SPRentCardThanks"].asInt());
		
		mySGD->setItemGachaGoldFee(result_data["itemGachaGoldFee"].asInt());
		mySGD->setItemGachaReplayGoldFee(result_data["itemGachaReplayGoldFee"].asInt());
		mySGD->setUpgradeGoldFee(result_data["upgradeGoldFee"].asInt());
		mySGD->setIngameTutorialRewardGold(result_data["ingameTutorialRewardGold"].asInt());
		
		mySGD->setRankUpConditionCount(result_data["rankUpConditionCount"].asInt());
		mySGD->setRankUpBaseRate(result_data["rankUpBaseRate"].asFloat());
		mySGD->setRankUpRateDistance(result_data["rankUpRateDistance"].asFloat());
		mySGD->setRankUpRubyFee(result_data["rankUpRubyFee"].asInt());
		
		mySGD->setFirstPurchasePlayCount(result_data["firstPurchasePlayCount"].asInt());
		mySGD->setFirstPurchaseReviewSecond(result_data["firstPurchaseReviewSecond"].asInt64());
		mySGD->setEmptyItemReviewSecond(result_data["emptyItemReviewSecond"].asInt64());
		mySGD->setStupidNpuHelpReviewSecond(result_data["stupidNpuHelpReviewSecond"].asInt64());
		mySGD->setStupidNpuHelpPlayCount(result_data["stupidNpuHelpPlayCount"].asInt());
		mySGD->setStupidNpuHelpFailCount(result_data["stupidNpuHelpFailCount"].asInt());
		mySGD->setEventRubyShopReviewSecond(result_data["eventRubyShopReviewSecond"].asInt64());
		mySGD->setPlayCountHighValue(result_data["playCountHighValue"].asInt());
		
		mySGD->setEmptyItemIsOn(result_data["emptyItemIsOn"].asInt());
		mySGD->setStupidNpuHelpIsOn(result_data["stupidNpuHelpIsOn"].asInt());
		mySGD->setPlayCountHighIsOn(result_data["playCountHighIsOn"].asInt());
		
		mySGD->setLevelupGuidePlayCount(result_data["levelupGuidePlayCount"].asInt());
		mySGD->setLevelupGuideConditionLevel(result_data["levelupGuideConditionLevel"].asInt());
		mySGD->setLevelupGuideIsOn(result_data["levelupGuideIsOn"].asInt());
		mySGD->setLevelupGuideReviewSecond(result_data["levelupGuideReviewSecond"].asInt64());
		
		mySGD->setItem9OpenStage(result_data["item9OpenStage"].asInt());
		mySGD->setItem6OpenStage(result_data["item6OpenStage"].asInt());
		mySGD->setItem11OpenStage(result_data["item11OpenStage"].asInt());
		mySGD->setItemGachaOpenStage(result_data["itemGachaOpenStage"].asInt());
		
		mySGD->setPuzzlePerfectRewardRuby(result_data["puzzlePerfectRewardRuby"].asInt());
		mySGD->setEndlessMinPiece(result_data["endlessMinPiece"].asInt());
		
		mySGD->setIsAlwaysSavePlaydata(result_data["isAlwaysSavePlaydata"].asInt());
		mySGD->setPlayContinueFeeEndless(result_data["playContinueFeeEndless"].asInt());
		
		mySGD->setAllClearReward(result_data["allClearReward"].asString());
		mySGD->setClearTakeCardCnt(result_data["clearTakeCardCnt"].asInt());
		mySGD->setUnlockFrameCnt(result_data["unlockFrameCnt"].asInt());
		mySGD->setSendPvpPlayDataRate(result_data["sendPvpPlayDataRate"].asInt());
		
		mySGD->setIntroduceStage(result_data["introduceStage"].asInt());
		mySGD->setKakaoMsg(result_data["kakaoMsg"].asString());
		mySGD->setAddGemReward(result_data["addGemReward"].asInt());
		
		mySGD->setDiaryDownUrl(result_data["appDownUrls"]["diary"].asString());
		mySGD->setIosMenuVisible(result_data["iosMenuVisible"].asInt());
		mySGD->setIosHideVer(result_data["iosHideVer"].asString());
		mySGD->setGoldBalance(result_data["goldBalance"].asString());
		
		mySGD->setNmlGc(result_data["nmlGc"].asInt());
		mySGD->setPrmGc(result_data["prmGc"].asInt());
		mySGD->setGachaRefreshTime(result_data["gachaRefreshTime"].asInt());
		
		mySGD->setPvpWinExp(result_data["pvpWinExp"].asInt());
		mySGD->setPvpLoseExp(result_data["pvpLoseExp"].asInt());
		mySGD->setGachaCharExp(result_data["gachaCharExp"].asInt());
		mySGD->setGachaRefreshGem(result_data["gachaRefreshGem"].asInt());
	}
	else
	{
		is_receive_fail = true;
		command_list.push_back(CommandParam("getcommonsetting", Json::Value(), json_selector(this, TitleRenewalScene::resultGetCommonSetting)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetHellModeList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		hell_data_version = result_data["version"].asInt();
		
		Json::Value list_data = result_data["list"];
		int list_count = list_data.size();
		NSDS_SI(kSDS_GI_hellMode_listCount_i, list_count, false);
		for(int i=0;i<list_count;i++)
		{
			Json::Value t_data = list_data[i];
			NSDS_SI(kSDS_GI_hellMode_int1_pieceNo_i, i+1, t_data["pieceNo"].asInt(), false);
			NSDS_SI(kSDS_GI_hellMode_int1_openPieceNo_i, i+1, t_data["openPieceNo"].asInt(), false);
			NSDS_SS(kSDS_GI_hellMode_int1_title_s, i+1, t_data["title"].asString(), false);
			NSDS_SS(kSDS_GI_hellMode_int1_content_s, i+1, t_data["content"].asString(), false);
			NSDS_SI(kSDS_GI_hellMode_int1_characterNo_i, i+1, t_data["characterNo"].asInt(), false);
			
			string img_url = t_data["cellImgInfo"].asString();
			if(NSDS_GS(kSDS_GI_hellMode_int1_cellImgInfo_s, i+1) != img_url)
			{
				NSDS_SS(kSDS_GI_hellMode_int1_cellImgInfo_s, i+1, img_url, false);
				
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 0;
				t_df.img = img_url;
				t_df.filename = ccsf("hell_cell%d_img.png", i+1);
				t_df.key = ccsf("hl%dcellImg", i+1);
				
				auto iter = find(character_download_list.begin(), character_download_list.end(), t_df);
				if(iter == character_download_list.end())
					character_download_list.push_back(t_df);
				// ================================
			}
			
			Json::Value stage_info = t_data["pieceInfo"];
			
			int stage_number = stage_info["no"].asInt();
			int puzzle_number = stage_info["puzzle"].asInt();
			
			NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_level_i, stage_number, stage_info["level"].asInt(), false);
			NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number, stage_info["pieceNo"].asInt(), false);
			
			if(!stage_info["condition"].isNull())
			{
				NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number, stage_info["condition"]["gold"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number, stage_info["condition"]["no"].asInt(), false);
			}
			
			CCLOG("saved version : %d", NSDS_GI(stage_number, kSDS_SI_version_i));
			if(NSDS_GI(stage_number, kSDS_SI_version_i) < stage_info["version"].asInt())
			{
				NSDS_SI(stage_number, kSDS_SI_version_i, stage_info["version"].asInt(), false);
				
				NSDS_SI(stage_number, kSDS_SI_puzzle_i, puzzle_number, false);
				NSDS_SI(stage_number, kSDS_SI_playtime_i, stage_info["playtime"].asInt(), false);
				NSDS_SD(stage_number, kSDS_SI_scoreRate_d, stage_info["scoreRate"].asDouble(), false);
				NSDS_SD(stage_number, kSDS_SI_scale_d, stage_info["scale"].asDouble(), false);
				//				NSDS_SB(stage_number, kSDS_SI_minigame_b, stage_list[i]["minigame"].asBool(), false);
				NSDS_SS(stage_number, kSDS_SI_type_s, stage_info["type"].asString(), false);
				
				Json::Value t_mission = stage_info["mission"];
				NSDS_SI(stage_number, kSDS_SI_missionType_i, t_mission["type"].asInt(), false);
				
				Json::Value t_option;
				if(!t_mission["option"].isObject())			t_option["key"]="value";
				else										t_option =t_mission["option"];
				
				if(t_mission["type"].asInt() == kCLEAR_bossLifeZero)
					NSDS_SI(stage_number, kSDS_SI_missionOptionEnergy_i, t_option["energy"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_subCumberCatch)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_bigArea)
				{
					NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
				}
				else if(t_mission["type"].asInt() == kCLEAR_itemCollect)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_perfect)
					NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_timeLimit)
					NSDS_SI(stage_number, kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_hellMode)
					NSDS_SI(stage_number, kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_percentage)
					NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_score)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["score"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_combo)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["combo"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_gold)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["gold"].asInt(), false);
				else if(t_mission["type"].asInt() == kCLEAR_turns)
					NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["turns"].asInt(), false);
				
				
				Json::Value shopItems = stage_info["shopItems"];
				NSDS_SI(stage_number, kSDS_SI_shopItemsCnt_i, shopItems.size(), false);
				for(int i=0;i<shopItems.size();i++)
				{
					Json::Value t_item = shopItems[i];
					NSDS_SS(stage_number, kSDS_SI_shopItems_int1_currency_s, i, t_item["currency"].asString(), false);
					NSDS_SI(stage_number, kSDS_SI_shopItems_int1_type_i, i, t_item["type"].asInt(), false);
					NSDS_SI(stage_number, kSDS_SI_shopItems_int1_price_i, i, t_item["price"].asInt(), false);
					
					
					Json::Value t_option;
					if(!t_item["option"].isObject())				t_option["key"]="value";
					else											t_option =t_item["option"];
					
					if(t_item["type"].asInt() == kIC_attack)
						NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option.get("power",0).asInt(), false);
					else if(t_item["type"].asInt() == kIC_addTime)
						NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option.get("sec",0).asInt(), false);
					else if(t_item["type"].asInt() == kIC_fast)
						NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option.get("sec",0).asInt(), false);
					else if(t_item["type"].asInt() == kIC_silence)
						NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option.get("sec",0).asInt(), false);
					else if(t_item["type"].asInt() == kIC_doubleItem)
						NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_longTime)
						NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_baseSpeedUp)
						NSDS_SI(stage_number, kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
				}
				
				Json::Value defItems = stage_info["defItems"];
				NSDS_SI(stage_number, kSDS_SI_defItemsCnt_i, defItems.size(), false);
				for(int i=0;i<defItems.size();i++)
				{
					Json::Value t_item = defItems[i];
					NSDS_SI(stage_number, kSDS_SI_defItems_int1_type_i, i, t_item["type"].asInt(), false);
					
					Json::Value t_option;
					if(!t_item["option"].isObject())				t_option["key"]="value";
					else											t_option =t_item["option"];
					
					if(t_item["type"].asInt() == kIC_attack)
						NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option["power"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_addTime)
						NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_fast)
						NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_silence)
						NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_doubleItem)
						NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_longTime)
						NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
					else if(t_item["type"].asInt() == kIC_baseSpeedUp)
						NSDS_SI(stage_number, kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
				}
				
				Json::Value cards = stage_info["cards"];
				NSDS_SI(stage_number, kSDS_SI_cardCount_i, cards.size(), false);
				for(int i=0;i<cards.size();i++)
				{
					Json::Value t_card = cards[i];
					NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
					NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
					NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
					NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
					NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
					NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
					NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
					
					NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
					
					Json::Value t_imgInfo = t_card["imgInfo"];
					
//					bool is_add_cf = false;
					
					if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
					{
						// check, after download ----------
						DownloadFile t_sf;
						t_sf.size = t_imgInfo["size"].asInt();
						t_sf.img = t_imgInfo["img"].asString().c_str();
						t_sf.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
						t_sf.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
						
						auto iter = find(card_download_list.begin(), card_download_list.end(), t_sf);
						if(iter == card_download_list.end())
							card_download_list.push_back(t_sf);
						// ================================
						
//						CopyFile t_cf;
//						t_cf.from_filename = t_sf.filename.c_str();
//						t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//						card_reduction_list.push_back(t_cf);
//						
//						is_add_cf = true;
					}
					
					Json::Value t_aniInfo = t_card["aniInfo"];
					NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
					if(t_aniInfo["isAni"].asBool())
					{
						Json::Value t_detail = t_aniInfo["detail"];
						NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
						
						Json::Value t_loopSeq = t_detail["loopSeq"];
						for(int j=0;j<t_loopSeq.size();j++)
							NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
						
						NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
						NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
						NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
						NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
						NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
						
						if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
						{
							// check, after download ----------
							DownloadFile t_sf;
							t_sf.size = t_detail["size"].asInt();
							t_sf.img = t_detail["img"].asString().c_str();
							t_sf.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
							t_sf.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
							
							auto iter = find(card_download_list.begin(), card_download_list.end(), t_sf);
							if(iter == card_download_list.end())
								card_download_list.push_back(t_sf);
							// ================================
						}
						
//						if(is_add_cf)
//						{
//							CopyFile t_cf = card_reduction_list.back();
//							card_reduction_list.pop_back();
//							t_cf.is_ani = true;
//							t_cf.cut_width = t_detail["cutWidth"].asInt();
//							t_cf.cut_height = t_detail["cutHeight"].asInt();
//							t_cf.position_x = t_detail["positionX"].asInt();
//							t_cf.position_y = t_detail["positionY"].asInt();
//							
//							t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//							
//							card_reduction_list.push_back(t_cf);
//						}
					}
					
					NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
					NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
					NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
					NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
					NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
					
					NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
					NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
					NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
					
					int sound_cnt = t_card["sound"].size();
					NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
					for(int j=1;j<=sound_cnt;j++)
					{
						CCLOG("sound is %s",t_card["sound"][j-1].asString().c_str());
						NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
					}
					
					NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
					
					Json::Value t_silImgInfo = t_card["silImgInfo"];
					NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
					if(t_silImgInfo["isSil"].asBool())
					{
						if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
						{
							// check, after download ----------
							DownloadFile t_sf;
							t_sf.size = t_silImgInfo["size"].asInt();
							t_sf.img = t_silImgInfo["img"].asString().c_str();
							t_sf.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
							t_sf.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
							
							auto iter = find(card_download_list.begin(), card_download_list.end(), t_sf);
							if(iter == card_download_list.end())
								card_download_list.push_back(t_sf);
							// ================================
						}
					}
					
					Json::Value t_faceInfo = t_card["faceInfo"];
					if(!t_faceInfo.isNull() && t_faceInfo.asString() != "")
					{
						NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
						NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
						
						DownloadFile t_df1;
						t_df1.size = t_faceInfo["size"].asInt();
						t_df1.img = t_faceInfo["ccbi"].asString().c_str();
						t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
						t_df1.key = mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str();
						card_download_list.push_back(t_df1);
						
						DownloadFile t_df2;
						t_df2.size = t_faceInfo["size"].asInt();
						t_df2.img = t_faceInfo["plist"].asString().c_str();
						t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
						t_df2.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str();
						card_download_list.push_back(t_df2);
						
						DownloadFile t_df3;
						t_df3.size = t_faceInfo["size"].asInt();
						t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
						t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
						t_df3.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str();
						card_download_list.push_back(t_df3);
						
//						if(!is_add_cf)
//						{
//							CopyFile t_cf;
//							t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//							t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//							card_reduction_list.push_back(t_cf);
//							
//							is_add_cf = true;
//						}
//						
//						CopyFile t_cf = card_reduction_list.back();
//						card_reduction_list.pop_back();
//						t_cf.is_ccb = true;
//						t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//						
//						card_reduction_list.push_back(t_cf);
					}
				}
				
				//				NSDS_SI(stage_number, kSDS_SI_level_i, stage_list[i]["level"].asInt(), false);
				
				NSDS_SS(stage_number, kSDS_SI_boss_s, stage_info["boss"].asString(), false);
				NSDS_SS(stage_number, kSDS_SI_junior_s, stage_info["junior"].asString(), false);
				NSDS_SI(stage_number, kSDS_SI_autoBalanceTry_i, stage_info["autoBalanceTry"].asInt(), false);
				
				mySDS->fFlush(stage_number, kSDS_SI_base);
			}
		}
		
		mySDS->fFlush(kSDS_GI_eventListVersion_i);
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		
	}
	else
	{
		is_receive_fail = true;
		Json::Value hell_param;
		hell_param["version"] = NSDS_GI(kSDS_GI_hellMode_version_i);
		command_list.push_back(CommandParam("gethellmodelist", hell_param, json_selector(this, TitleRenewalScene::resultGetHellModeList)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetHeart(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->heartRefreshSuccess(result_data);
	}
	else
	{
		is_receive_fail = true;
		Json::Value heart_param;
		heart_param["memberID"] = myHSP->getMemberID();
		command_list.push_back(CommandParam("getheart", heart_param, json_selector(this, TitleRenewalScene::resultGetHeart)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultCheckAttendanceEvent(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->initAttendance(result_data);
	}
	else
	{
		is_receive_fail = true;
		Json::Value attendance_param;
		attendance_param["memberID"] = myHSP->getMemberID();
		command_list.push_back(CommandParam("checkattendenceevent", attendance_param, json_selector(this, TitleRenewalScene::resultCheckAttendanceEvent)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultCheckWeeklyReward(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->initRankReward(result_data);
	}
	else
	{
		is_receive_fail = true;
		Json::Value rank_reward_param;
		rank_reward_param["memberID"] = myHSP->getMemberID();
		command_list.push_back(CommandParam("checkweeklyreward", rank_reward_param, json_selector(this, TitleRenewalScene::resultCheckWeeklyReward)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetTimeEvent(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->initTimeEventList(result_data["list"]);
	}
	else
	{
		is_receive_fail = true;
		command_list.push_back(CommandParam("gettimeevent", Json::Value(), json_selector(this, TitleRenewalScene::resultGetTimeEvent)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetTimeInfo(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->recent_week_no = result_data["weekNo"].asInt();
	}
	else
	{
		is_receive_fail = true;
		command_list.push_back(CommandParam("gettimeinfo", Json::Value(), json_selector(this, TitleRenewalScene::resultGetTimeInfo)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetAchieveList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value t_list = result_data["list"];
		int list_size = t_list.size();
		
		int real_list_size = 0;
		
		for(int i=1;i<list_size;i++)
		{
			Json::Value achieve_group = t_list[i];
			int group_size = achieve_group.size();
			
			for(int j=0;j<group_size;j++)
			{
				real_list_size++;
				
				Json::Value achieve_info = achieve_group[j];
				
				NSDS_SI(kSDS_AI_int1_id_i, real_list_size, achieve_info["id"].asInt(), false);
				NSDS_SS(kSDS_AI_int1_title_s, achieve_info["id"].asInt(), achieve_info["title"].asString(), false);
				NSDS_SS(kSDS_AI_int1_content_s, achieve_info["id"].asInt(), achieve_info["content"].asString(), false);
				NSDS_SI(kSDS_AI_int1_goal_i, achieve_info["id"].asInt(), achieve_info["goal"].asInt(), false);
				
				Json::Value t_reward = achieve_info["reward"];
				int reward_size = t_reward.size();
				NSDS_SI(kSDS_AI_int1_reward_count_i, achieve_info["id"].asInt(), reward_size, false);
				for(int k=0;k<reward_size;k++)
				{
					NSDS_SS(kSDS_AI_int1_reward_int2_type_s, achieve_info["id"].asInt(), k+1, t_reward[k]["type"].asString(), false);
					NSDS_SI(kSDS_AI_int1_reward_int2_count_i, achieve_info["id"].asInt(), k+1, t_reward[k]["count"].asInt(), false);
				}
				
				NSDS_SS(kSDS_AI_int1_exchangeID_s, achieve_info["id"].asInt(), achieve_info["exchangeID"].asString(), false);
				NSDS_SI(kSDS_AI_int1_groupNo_i, achieve_info["id"].asInt(), achieve_info["groupNo"].asInt(), false);
			}
		}
		
		NSDS_SI(kSDS_AI_count_i, real_list_size, false);
		
		NSDS_SI(kSDS_AI_version_i, result_data["version"].asInt(), false);
		mySDS->fFlush(kSDS_AI_base);
		
		myAchieve->initAchievement();
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		myAchieve->initAchievement();
	}
	else
	{
		is_receive_fail = true;
		command_list.push_back(CommandParam("getarchivementlist", Json::Value(), json_selector(this, TitleRenewalScene::resultGetAchieveList)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetShopList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value result_list = result_data["list"];
		
		for(int i=1;i<=6;i++)
		{
			string t_key = CCString::createWithFormat("s_r_%d", i)->getCString();
			Json::Value t_data = result_data[t_key.c_str()];
			
//			NSDS_SI(kSDS_GI_shopRuby_int1_count_i, i-1, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_countName_s, i-1, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopRuby_int1_price_i, i-1, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopRuby_int1_priceType_s, i-1, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_priceName_s, i-1, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_sale_s, i-1, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_pID_s, i-1, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_exchangeID_s, i-1, t_data["exchangeID"].asString(), false);
		}
		
		for(int i=1;i<=6;i++)
		{
			string t_key = CCString::createWithFormat("s_g_%d", i)->getCString();
			Json::Value t_data = result_data[t_key.c_str()];
			
//			NSDS_SI(kSDS_GI_shopGold_int1_count_i, i-1, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_countName_s, i-1, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopGold_int1_price_i, i-1, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopGold_int1_priceType_s, i-1, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_priceName_s, i-1, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_sale_s, i-1, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_exchangeID_s, i-1, t_data["exchangeID"].asString(), false);
		}
		
		for(int i=1;i<=6;i++)
		{
			string t_key = CCString::createWithFormat("s_h_%d", i)->getCString();
			Json::Value t_data = result_data[t_key.c_str()];
			
//			NSDS_SI(kSDS_GI_shopCoin_int1_count_i, i-1, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_countName_s, i-1, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopCoin_int1_price_i, i-1, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopCoin_int1_priceType_s, i-1, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_priceName_s, i-1, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_sale_s, i-1, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_exchangeID_s, i-1, t_data["exchangeID"].asString(), false);
		}
		
		for(int i=1;i<=6;i++)
		{
			string t_key = CCString::createWithFormat("es_r_%d", i)->getCString();
			Json::Value t_data = result_data[t_key.c_str()];
			
//			NSDS_SI(kSDS_GI_shopEventRuby_int1_count_i, i-1, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopEventRuby_int1_countName_s, i-1, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopEventRuby_int1_price_i, i-1, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopEventRuby_int1_priceType_s, i-1, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopEventRuby_int1_priceName_s, i-1, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopEventRuby_int1_sale_s, i-1, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopEventRuby_int1_pID_s, i-1, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopEventRuby_int1_exchangeID_s, i-1, t_data["exchangeID"].asString(), false);
		}
		
		{
			int t_code = kIC_baseSpeedUp;
			string t_key = CCString::createWithFormat("s_i_%d", t_code)->getCString();
			
			Json::Value t_data = result_data[t_key.c_str()];
			
			NSDS_SS(kSDS_GI_shopItem_int1_countName_s, t_code, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopItem_int1_price_i, t_code, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopItem_int1_priceType_s, t_code, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceName_s, t_code, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_sale_s, t_code, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_exchangeID_s, t_code, t_data["exchangeID"].asString(), false);
			
			
			t_code = kIC_doubleItem;
			t_key = CCString::createWithFormat("s_i_%d", t_code)->getCString();
			
			t_data.clear();
			t_data = result_data[t_key.c_str()];
			
			NSDS_SS(kSDS_GI_shopItem_int1_countName_s, t_code, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopItem_int1_price_i, t_code, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopItem_int1_priceType_s, t_code, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceName_s, t_code, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_sale_s, t_code, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_exchangeID_s, t_code, t_data["exchangeID"].asString(), false);
			
			
			t_code = kIC_magnet;
			t_key = CCString::createWithFormat("s_i_%d", t_code)->getCString();
			
			t_data.clear();
			t_data = result_data[t_key.c_str()];
			
			NSDS_SS(kSDS_GI_shopItem_int1_countName_s, t_code, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopItem_int1_price_i, t_code, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopItem_int1_priceType_s, t_code, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceName_s, t_code, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_sale_s, t_code, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_exchangeID_s, t_code, t_data["exchangeID"].asString(), false);
		}
		
		Json::FastWriter t_writer;
		int t_index = 0;
		{
			Json::Value t_data = result_data["pg_fp"];
			
//			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_count_i, t_index, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_countName_s, t_index, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_price_i, t_index, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceType_s, t_index, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, t_index, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_sale_s, t_index, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_data_s, t_index, t_writer.write(t_data["data"]), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_pID_s, t_index, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_exchangeID_s, t_index, t_data["exchangeID"].asString(), false);
			
			t_index++;
		}
		{
			Json::Value t_data = result_data["pg_ei"];
			
//			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_count_i, t_index, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_countName_s, t_index, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_price_i, t_index, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceType_s, t_index, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, t_index, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_sale_s, t_index, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_data_s, t_index, t_writer.write(t_data["data"]), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_pID_s, t_index, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_exchangeID_s, t_index, t_data["exchangeID"].asString(), false);
			
			t_index++;
		}
		{
			Json::Value t_data = result_data["pg_snh"];
			
//			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_count_i, t_index, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_countName_s, t_index, t_data["countName"].asString(), false);
//			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_price_i, t_index, t_data["price"].asInt(), false);
//			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceType_s, t_index, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, t_index, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_sale_s, t_index, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_data_s, t_index, t_writer.write(t_data["data"]), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_pID_s, t_index, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_exchangeID_s, t_index, t_data["exchangeID"].asString(), false);
			
			t_index++;
		}
		
		for(int i=1;i<=6;i++)
		{
			string t_key = CCString::createWithFormat("s_p1_%d", i)->getCString();
			Json::Value t_data = result_data[t_key.c_str()];
			
			NSDS_SS(kSDS_GI_shopP1_int1_countName_s, i-1, t_data["countName"].asString(), false);
			NSDS_SS(kSDS_GI_shopP1_int1_priceName_s, i-1, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopP1_int1_sale_s, i-1, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopP1_int1_exchangeID_s, i-1, t_data["exchangeID"].asString(), false);
		}
		
		Json::Value startPack = result_data["startPack"];
		NSDS_SI(kSDS_GI_shopStartPack_no_i, startPack["no"].asInt(), false);
		NSDS_SS(kSDS_GI_shopStartPack_pID_s, startPack["pID"].asString(), false);
		NSDS_SS(kSDS_GI_shopStartPack_exchangeID_s, startPack["exchangeID"].asString(), false);
		
		Json::Value sp_data = startPack["data"];
		NSDS_SI(kSDS_GI_shopStartPack_expireSec_i, sp_data["expireSec"].asInt(), false);
		if(NSDS_GS(kSDS_GI_shopStartPack_img_s) != sp_data["img"].asString())
		{
			// check, after download ----------
			DownloadFile t_df;
			t_df.size = 0;
			t_df.img = sp_data["img"].asString();
			t_df.filename = "start_pack.png";
			t_df.key = "sspImg";
			
			auto iter = find(character_download_list.begin(), character_download_list.end(), t_df);
			if(iter == character_download_list.end())
				character_download_list.push_back(t_df);
			// ================================
		}
		NSDS_SS(kSDS_GI_shopStartPack_img_s, sp_data["img"].asString(), false);
		
		
		Json::Value eventPack = result_data["eventPack"];
		bool is_have_eventPack = eventPack["data"]["isHave"].asBool();
		NSDS_SB(kSDS_GI_shopEventPack_isHave_b, is_have_eventPack, false);
		if(is_have_eventPack)
		{
			NSDS_SI(kSDS_GI_shopEventPack_no_i, eventPack["no"].asInt(), false);
			NSDS_SS(kSDS_GI_shopEventPack_pID_s, eventPack["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopEventPack_exchangeID_s, eventPack["exchangeID"].asString(), false);
			NSDS_SS(kSDS_GI_shopEventPack_startDate_s, eventPack["startDate"].asString(), false);
			NSDS_SS(kSDS_GI_shopEventPack_endDate_s, eventPack["endDate"].asString(), false);
			NSDS_SI(kSDS_GI_shopEventPack_startTime_i, eventPack["startTime"].asInt(), false);
			NSDS_SI(kSDS_GI_shopEventPack_endTime_i, eventPack["endTime"].asInt(), false);
			
			Json::Value ep_data = eventPack["data"];
			NSDS_SI(kSDS_GI_shopEventPack_isJustOne_b, ep_data["isJustOne"].asBool(), false);
			if(NSDS_GS(kSDS_GI_shopEventPack_img_s) != ep_data["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 0;
				t_df.img = ep_data["img"].asString();
				t_df.filename = "event_pack.png";
				t_df.key = "sepImg";
				
				auto iter = find(character_download_list.begin(), character_download_list.end(), t_df);
				if(iter == character_download_list.end())
					character_download_list.push_back(t_df);
				// ================================
			}
			NSDS_SS(kSDS_GI_shopEventPack_img_s, ep_data["img"].asString(), false);
		}
		
		
		NSDS_SB(kSDS_GI_shop_isEvent_b, result_data["event"].asBool(), false);
		NSDS_SI(kSDS_GI_shopVersion_i, result_data["version"].asInt());
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		
	}
	else
	{
		is_receive_fail = true;
		Json::Value shop_param;
		shop_param["version"] = NSDS_GI(kSDS_GI_shopVersion_i);
		command_list.push_back(CommandParam("getshoplist", shop_param, json_selector(this, TitleRenewalScene::resultGetShopList)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetNoticeList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->resetNoticeList(result_data["list"]);
	}
	else
	{
		is_receive_fail = true;
		Json::Value notice_param;
		notice_param["buildNo"] = myHSP->getVersionCode();
		command_list.push_back(CommandParam("getnoticelist", notice_param, json_selector(this, TitleRenewalScene::resultGetNoticeList)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetCharacterInfo(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value character_list = result_data["list"];
		int character_count = character_list.size();
		NSDS_SI(kSDS_GI_characterCount_i, character_count, false);
		for(int i=1;i<=character_count;i++)
		{
			NSDS_SI(kSDS_GI_characterInfo_int1_no_i, i, character_list[i-1]["no"].asInt(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_name_s, i, character_list[i-1]["name"].asString(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_purchaseInfo_type_s, i, character_list[i-1]["purchaseInfo"]["type"].asString(), false);
			NSDS_SI(kSDS_GI_characterInfo_int1_purchaseInfo_value_i, i, character_list[i-1]["purchaseInfo"]["value"].asInt(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_scriptInfo_s, i, character_list[i-1]["scriptInfo"].asString(), false);
			Json::Value stat_info = character_list[i-1]["statInfo"];
			Json::Value pattern_info, mission_info, missile_info, comment_info;
			Json::Reader t_reader;
			t_reader.parse(character_list[i-1]["patternInfo"].asString(), pattern_info);
			t_reader.parse(character_list[i-1]["missionInfo"].asString(), mission_info);
			t_reader.parse(character_list[i-1]["missileInfo"].asString(), missile_info);
			t_reader.parse(character_list[i-1]["comment"].asString(), comment_info);
			int max_level = stat_info.size();
			NSDS_SI(kSDS_GI_characterInfo_int1_maxLevel_i, i, max_level, false);
			for(int j=0;j<max_level;j++)
			{
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_gold_d, i, j+1, stat_info[j]["gold"].asDouble(), false);
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_percent_d, i, j+1, stat_info[j]["percent"].asDouble(), false);
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_speed_d, i, j+1, stat_info[j]["speed"].asDouble(), false);
				NSDS_SI(kSDS_GI_characterInfo_int1_statInfo_int2_life_i, i, j+1, stat_info[j]["life"].asInt(), false);
				NSDS_SI(kSDS_GI_characterInfo_int1_statInfo_int2_lineColor_i, i, j+1, stat_info[j]["color"].asInt(), false);
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_rewindSpd_d, i, j+1, stat_info[j]["rewindSpd"].asDouble(), false);
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_monsterWave_d, i, j+1, stat_info[j]["monsterWave"].asDouble(), false);
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_missileWave_d, i, j+1, stat_info[j]["missileWave"].asDouble(), false);
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_magnetic_d, i, j+1, stat_info[j]["magnetic"].asDouble(), false);
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_power_d, i, j+1, stat_info[j]["power"].asDouble(), false);
				NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_int2_score_d, i, j+1, stat_info[j]["score"].asDouble(), false);
				NSDS_SS(kSDS_GI_characterInfo_int1_patternInfo_int2_s, i, j+1, pattern_info[j].asString(), false);
				NSDS_SS(kSDS_GI_characterInfo_int1_missionInfo_int2_s, i, j+1, mission_info[j].asString(), false);
				NSDS_SS(kSDS_GI_characterInfo_int1_missileInfo_int2_s, i, j+1, missile_info[j].asString(), false);
				NSDS_SS(kSDS_GI_characterInfo_int1_comment_int2_s, i, j+1, comment_info[j].asString(), false);
			}
			NSDS_SS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, i, character_list[i-1]["resourceInfo"]["ccbiID"].asString(), false);
			
			if(NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbi_s, i) != character_list[i-1]["resourceInfo"]["ccbi"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 100;
				t_df.img = character_list[i-1]["resourceInfo"]["ccbi"].asString().c_str();
				t_df.filename = character_list[i-1]["resourceInfo"]["ccbiID"].asString() + ".ccbi";
				t_df.key = CCSTR_CWF("ci%d_ri_ccbi", i)->getCString();
				
				auto iter = find(character_download_list.begin(), character_download_list.end(), t_df);
				if(iter == character_download_list.end())
					character_download_list.push_back(t_df);
				// ================================
			}
			
			NSDS_SS(kSDS_GI_characterInfo_int1_resourceInfo_imageID_s, i, character_list[i-1]["resourceInfo"]["imageID"].asString(), false);
			
			if(NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_plist_s, i) != character_list[i-1]["resourceInfo"]["plist"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 100;
				t_df.img = character_list[i-1]["resourceInfo"]["plist"].asString().c_str();
				t_df.filename = character_list[i-1]["resourceInfo"]["imageID"].asString() + ".plist";
				t_df.key = CCSTR_CWF("ci%d_ri_plist", i)->getCString();
				
				auto iter = find(character_download_list.begin(), character_download_list.end(), t_df);
				if(iter == character_download_list.end())
					character_download_list.push_back(t_df);
				// ================================
			}
			
			
			if(NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_pvrccz_s, i) != character_list[i-1]["resourceInfo"]["pvrccz"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 100;
				t_df.img = character_list[i-1]["resourceInfo"]["pvrccz"].asString().c_str();
				t_df.filename = character_list[i-1]["resourceInfo"]["imageID"].asString() + ".pvr.ccz";
				t_df.key = CCSTR_CWF("ci%d_ri_pvrccz", i)->getCString();
				
				auto iter = find(character_download_list.begin(), character_download_list.end(), t_df);
				if(iter == character_download_list.end())
					character_download_list.push_back(t_df);
				// ================================
			}
			
			NSDS_SI(kSDS_GI_characterInfo_int1_resourceInfo_size_i, i, character_list[i-1]["resourceInfo"]["size"].asInt(), false);
		}
		
		if(character_download_list.size() > 0)
			character_download_version = result_data["version"].asInt();
		else
			NSDS_SI(kSDS_GI_characterVersion_i, result_data["version"].asInt(), false);
		
		mySDS->fFlush(kSDS_GI_characterCount_i);
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		
	}
	else
	{
		is_receive_fail = true;
		Json::Value character_param;
		character_param["version"] = NSDS_GI(kSDS_GI_characterVersion_i);
		command_list.push_back(CommandParam("getcharacterlist", character_param, json_selector(this, TitleRenewalScene::resultGetCharacterInfo)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetMonsterList(Json::Value result_data)
{
	CCLOG("result getmonsterlist : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value monster_list = result_data["list"];
		int monster_count = monster_list.size();
		NSDS_SI(kSDS_GI_monsterCount_i, monster_count, false);
		
		vector<DownloadFile>::iterator iter;
		
		for(int i=1;i<=monster_count;i++)
		{
			NSDS_SI(kSDS_GI_monsterInfo_int1_no_i, i, monster_list[i-1]["no"].asInt(), false);
			NSDS_SS(kSDS_GI_monsterInfo_int1_name_s, i, monster_list[i-1]["name"].asString(), false);
			NSDS_SB(kSDS_GI_monsterInfo_int1_isBoss_b, i, monster_list[i-1]["isBoss"].asBool(), false);
			NSDS_SS(kSDS_GI_monsterInfo_int1_resourceInfo_ccbiID_s, i, monster_list[i-1]["resourceInfo"]["ccbiID"].asString(), false);
			
			if(!monster_list[i-1]["script"].isNull())
			{
				NSDS_SS(kSDS_GI_monsterInfo_int1_script_start_s, i, monster_list[i-1]["script"]["start"].asString(), false);
				NSDS_SS(kSDS_GI_monsterInfo_int1_script_clear_s, i, monster_list[i-1]["script"]["clear"].asString(), false);
				NSDS_SS(kSDS_GI_monsterInfo_int1_script_fail_s, i, monster_list[i-1]["script"]["fail"].asString(), false);
			}
			
			string monster_type = monster_list[i-1]["type"].asString();
			if(monster_type == "snake")
			{
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_ccbi_s, i) != monster_list[i-1]["resourceInfo"]["ccbi"].asString())
				{
					// check, after download ----------
					DownloadFile t_df1;
					t_df1.size = 100;
					t_df1.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString();
					t_df1.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + "_head.ccbi";
					t_df1.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df1);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df1);
					// ================================
					
					string body_img = monster_list[i-1]["resourceInfo"]["ccbi"].asString().c_str();
					GraphDogLib::replaceString(body_img, "_head.ccbi", "_body.ccbi");
					
					// check, after download ----------
					DownloadFile t_df2;
					t_df2.size = 100;
					t_df2.img = body_img.c_str();
					t_df2.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + "_body.ccbi";
					t_df2.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df2);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df2);
					// ================================
					
					string tail_img = monster_list[i-1]["resourceInfo"]["ccbi"].asString().c_str();
					GraphDogLib::replaceString(tail_img, "_head.ccbi", "_tail.ccbi");
					
					// check, after download ----------
					DownloadFile t_df3;
					t_df3.size = 100;
					t_df3.img = tail_img.c_str();
					t_df3.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + "_tail.ccbi";
					t_df3.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df3);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df3);
					// ================================
				}
				
				NSDS_SS(kSDS_GI_monsterInfo_int1_resourceInfo_imageID_s, i, monster_list[i-1]["resourceInfo"]["imageID"].asString(), false);
				
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_plist_s, i) != monster_list[i-1]["resourceInfo"]["plist"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["plist"].asString().c_str();
					t_df.filename = monster_list[i-1]["resourceInfo"]["imageID"].asString() + ".plist";
					t_df.key = CCSTR_CWF("mi%d_ri_plist", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df);
					// ================================
				}
				
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_pvrccz_s, i) != monster_list[i-1]["resourceInfo"]["pvrccz"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["pvrccz"].asString().c_str();
					t_df.filename = monster_list[i-1]["resourceInfo"]["imageID"].asString() + ".pvr.ccz";
					t_df.key = CCSTR_CWF("mi%d_ri_pvrccz", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df);
					// ================================
				}
				
				NSDS_SI(kSDS_GI_monsterInfo_int1_resourceInfo_size_i, i, monster_list[i-1]["resourceInfo"]["size"].asInt(), false);
			}
			else
			{
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_ccbi_s, i) != monster_list[i-1]["resourceInfo"]["ccbi"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString();
					t_df.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + ".ccbi";
					t_df.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df);
					// ================================
				}
				
				NSDS_SS(kSDS_GI_monsterInfo_int1_resourceInfo_imageID_s, i, monster_list[i-1]["resourceInfo"]["imageID"].asString(), false);
				
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_plist_s, i) != monster_list[i-1]["resourceInfo"]["plist"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["plist"].asString().c_str();
					t_df.filename = monster_list[i-1]["resourceInfo"]["imageID"].asString() + ".plist";
					t_df.key = CCSTR_CWF("mi%d_ri_plist", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df);
					// ================================
				}
				
				if(NSDS_GS(kSDS_GI_monsterInfo_int1_resourceInfo_pvrccz_s, i) != monster_list[i-1]["resourceInfo"]["pvrccz"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = 100;
					t_df.img = monster_list[i-1]["resourceInfo"]["pvrccz"].asString().c_str();
					t_df.filename = monster_list[i-1]["resourceInfo"]["imageID"].asString() + ".pvr.ccz";
					t_df.key = CCSTR_CWF("mi%d_ri_pvrccz", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df);
					// ================================
				}
				
				NSDS_SI(kSDS_GI_monsterInfo_int1_resourceInfo_size_i, i, monster_list[i-1]["resourceInfo"]["size"].asInt(), false);
			}
		}
		
		if(monster_download_list.size() > 0)
			monster_download_version = result_data["version"].asInt();
		else
			NSDS_SI(kSDS_GI_monsterVersion_i, result_data["version"].asInt(), false);
		
		mySDS->fFlush(kSDS_GI_characterCount_i);
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		
	}
	else
	{
		is_receive_fail = true;
		Json::Value monster_param;
		monster_param["version"] = NSDS_GI(kSDS_GI_monsterVersion_i);
		command_list.push_back(CommandParam("getmonsterlist", monster_param, json_selector(this, TitleRenewalScene::resultGetMonsterList)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetUserData( Json::Value result_data )
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		hspConnector::get()->myKakaoInfo["userIndex"] = result_data["userIndex"].asInt64();
		Json::Value data1;
		Json::Reader reader1;
		reader1.parse(result_data["data"].asString(), data1);
		
		myDSH->resetDSH();
//		myDSH->loadAllUserData(result_data);
		
		if(myDSH->getStringForKey(kDSH_Key_nick) != result_data["nick"].asString())
		{
			myDSH->setStringForKey(kDSH_Key_nick, result_data["nick"].asString());
		}
		
        if(myDSH->getStringForKey(kDSH_Key_flag) != result_data["flag"].asString())
        {
            myDSH->setStringForKey(kDSH_Key_flag, result_data["flag"].asString());
        }
        
		if(myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep) != kTutorialFlowStep_puzzleClick)
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_end);
		
		mySGD->initUserdata(result_data);
		
		int scenario_value = myDSH->getIntegerForKey(kDSH_Key_showedScenario);
		if(scenario_value < 1000 && mySGD->getUserdataHighPiece() >= 1)
		{
			scenario_value = 1000;
		}
		int t_loop_cnt = mySGD->getPuzzleHistorySize();
		for(int i=0;i<t_loop_cnt;i++)
		{
			PuzzleHistory t_history = mySGD->getPuzzleHistoryForIndex(i);
			int t_puzzle_number = t_history.puzzle_number.getV();
			if(t_puzzle_number > 1 && t_history.is_open.getV() && scenario_value < 1000*t_puzzle_number)
			{
				scenario_value = t_puzzle_number*1000;
			}
		}
		
		myDSH->setIntegerForKey(kDSH_Key_showedScenario, scenario_value);
		
		int pvp_tuto_number = myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial);
		if(mySGD->getUserdataEndlessScore() > 0 && pvp_tuto_number != -1)
		{
			myDSH->setIntegerForKey(kDSH_Key_isShowEndlessModeTutorial, -1);
		}
		
		if(mySGD->getUserdataHighPiece() > mySGD->getItem9OpenStage())
			myDSH->setBoolForKey(kDSH_Key_isShowItem_int1, int(kIC_baseSpeedUp), true);
		if(mySGD->getUserdataHighPiece() > mySGD->getItem6OpenStage())
			myDSH->setBoolForKey(kDSH_Key_isShowItem_int1, int(kIC_doubleItem), true);
		if(mySGD->getUserdataHighPiece() > mySGD->getItem11OpenStage())
			myDSH->setBoolForKey(kDSH_Key_isShowItem_int1, int(kIC_magnet), true);
		if(mySGD->getUserdataHighPiece() > mySGD->getItemGachaOpenStage())
			myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, int(KindTutorialType::kNewItem_gacha), true);
		
		mySGD->user_index = result_data["userIndex"].asInt64();
	}
	else
	{
		is_receive_fail = true;
		Json::Value userdata_param;
		userdata_param["memberID"] = hspConnector::get()->getSocialID();
		command_list.push_back(CommandParam("getUserData", userdata_param, json_selector(this, TitleRenewalScene::resultGetUserData)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetAchieveHistory(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		myAchieve->initHistory(result_data["list"]);
	}
	else
	{
		is_receive_fail = true;
		Json::Value achieve_param;
		achieve_param["memberID"] = hspConnector::get()->getSocialID();
		command_list.push_back(CommandParam("getarchivementhistory", achieve_param, json_selector(this, TitleRenewalScene::resultGetAchieveHistory)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetCardHistory(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->total_card_cnt = result_data["lastCardNo"].asInt();
		
		card_data_load_list.clear();
		
		mySGD->initTakeCardInfo(result_data["list"], card_data_load_list);
		
		if(card_data_load_list.size() > 0)
		{
			bool is_zero_version = false;
			for(int i=0;!is_zero_version && i<card_data_load_list.size();i++)
			{
				if(NSDS_GI(kSDS_CI_int1_version_i, card_data_load_list[i]) == 0)
					is_zero_version = true;
			}
			
			Json::Value card_param;
			card_param["version"] = (is_zero_version ? 0 : NSDS_GI(kSDS_GI_card_version_i));
			for(int i=0;i<card_data_load_list.size();i++)
				card_param["noList"][i] = card_data_load_list[i];
			command_list.push_back(CommandParam("getcardlistbylist", card_param, json_selector(this, TitleRenewalScene::resultLoadedCardData)));
		}
		else
		{
			mySGD->resetHasGottenCards();
		}
	}
	else
	{
		is_receive_fail = true;
		Json::Value card_param;
		card_param["memberID"] = hspConnector::get()->getSocialID();
		command_list.push_back(CommandParam("getCardHistory", card_param, json_selector(this, TitleRenewalScene::resultGetCardHistory)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetPieceHistory(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		if(result_data["list"].size() == 0)
		{
			PieceHistory t_history = mySGD->getPieceHistory(NSDS_GI(NSDS_GI(kSDS_GI_puzzleList_int1_no_i, 1), kSDS_PZ_startStage_i));
			t_history.is_open = true;
			t_history.open_type = "무료(첫스테이지)";
			
			mySGD->setPieceHistory(t_history, nullptr);
		}
		else
		{
			mySGD->initPieceHistory(result_data["list"]);
		}
	}
	else
	{
		is_receive_fail = true;
		Json::Value piece_param;
		piece_param["memberID"] = hspConnector::get()->getSocialID();
		command_list.push_back(CommandParam("getPieceHistory", piece_param, json_selector(this, TitleRenewalScene::resultGetPieceHistory)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetCharacterHistory(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->initCharacterHistory(result_data["list"]);
	}
	else
	{
		is_receive_fail = true;
		Json::Value character_history_param;
		character_history_param["memberID"] = hspConnector::get()->getSocialID();
		command_list.push_back(CommandParam("getcharacterhistory", character_history_param, json_selector(this, TitleRenewalScene::resultGetCharacterHistory)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetTodayMission(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->initTodayMission(result_data);
	}
	else
	{
		is_receive_fail = true;
		Json::Value todaymission_param;
		todaymission_param["memberID"] = hspConnector::get()->getSocialID();
		command_list.push_back(CommandParam("gettodaymission", todaymission_param, json_selector(this, TitleRenewalScene::resultGetTodayMission)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetPuzzleHistory(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		if(result_data["list"].size() == 0)
		{
			PuzzleHistory t_history = mySGD->getPuzzleHistory(NSDS_GI(kSDS_GI_puzzleList_int1_no_i, 1));
			t_history.is_open = true;
			t_history.open_type = "무료(첫퍼즐)";
			
			mySGD->setPuzzleHistory(t_history, nullptr);
		}
		else
		{
			mySGD->initPuzzleHistory(result_data["list"]);
		}
	}
	else
	{
		is_receive_fail = true;
		Json::Value puzzle_param;
		puzzle_param["memberID"] = hspConnector::get()->getSocialID();
		command_list.push_back(CommandParam("getPuzzleHistory", puzzle_param, json_selector(this, TitleRenewalScene::resultGetPuzzleHistory)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetUserProperties(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->initProperties(result_data["list"]);
	}
	else
	{
		is_receive_fail = true;
		Json::Value properties_param;
		properties_param["memberID"] = hspConnector::get()->getSocialID();
		command_list.push_back(CommandParam("getuserproperties", properties_param, json_selector(this, TitleRenewalScene::resultGetUserProperties)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultLoadedCardData( Json::Value result_data )
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		card_data_version = result_data["version"].asInt();
		
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			
			if(t_card["piece"].asInt() == 0 || t_card["grade"].asInt() == 0 || t_card["no"].asInt() == 0)
			{
				CCLOG("piece : %d | grade : %d | card : %d", t_card["piece"].asInt(), t_card["grade"].asInt(), t_card["no"].asInt());
			}
			
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
//			Json::Value t_card_missile = t_card["missile"];
//			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//			
//			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//			
//			Json::Value t_ability = t_card["ability"];
//			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//			for(int j=0;j<t_ability.size();j++)
//			{
//				Json::Value t_abil = t_ability[j];
//				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//				
//				Json::Value t_option;
//				if(!t_abil["option"].isObject())                    t_option["key"]="value";
//				else												t_option =t_abil["option"];
//				
//				if(t_abil["type"].asInt() == kIC_attack)
//					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_addTime)
//					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_fast)
//					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_silence)
//					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_doubleItem)
//					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_longTime)
//					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//					NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//			}
			
			NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
//			bool is_add_cf = false;
			
			if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = t_imgInfo["size"].asInt();
				t_df.img = t_imgInfo["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
				
				auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
				if(iter == card_download_list.end())
				{
					card_download_list.push_back(t_df);
					// ================================
					
//					CopyFile t_cf;
//					t_cf.from_filename = t_df.filename.c_str();
//					t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//					card_reduction_list.push_back(t_cf);
//					
//					is_add_cf = true;
				}
			}
			
			Json::Value t_aniInfo = t_card["aniInfo"];
			NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
			if(t_aniInfo["isAni"].asBool())
			{
				Json::Value t_detail = t_aniInfo["detail"];
				NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
				
				Json::Value t_loopSeq = t_detail["loopSeq"];
				for(int j=0;j<t_loopSeq.size();j++)
					NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
				
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
				
				if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_detail["size"].asInt();
					t_df.img = t_detail["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
					
					auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
					if(iter == card_download_list.end())
						card_download_list.push_back(t_df);
					// ================================
				}
				
//				if(is_add_cf)
//				{
//					CopyFile t_cf = card_reduction_list.back();
//					card_reduction_list.pop_back();
//					t_cf.is_ani = true;
//					t_cf.cut_width = t_detail["cutWidth"].asInt();
//					t_cf.cut_height = t_detail["cutHeight"].asInt();
//					t_cf.position_x = t_detail["positionX"].asInt();
//					t_cf.position_y = t_detail["positionY"].asInt();
//					t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//					card_reduction_list.push_back(t_cf);
//				}
			}
			
			NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
			NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
			NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
			NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
			NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
			
			int sound_cnt = t_card["sound"].size();
			NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
			for(int j=1;j<=sound_cnt;j++)
			{
				NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
			}
			
			NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
			
			Json::Value t_silImgInfo = t_card["silImgInfo"];
			NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
			if(t_silImgInfo["isSil"].asBool())
			{
				if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_silImgInfo["size"].asInt();
					t_df.img = t_silImgInfo["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
					
					auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
					if(iter == card_download_list.end())
						card_download_list.push_back(t_df);
					// ================================
				}
			}
			
			Json::Value t_faceInfo = t_card["faceInfo"];
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "")
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
				DownloadFile t_df1;
				t_df1.size = t_faceInfo["size"].asInt();
				t_df1.img = t_faceInfo["ccbi"].asString().c_str();
				t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
				t_df1.key = mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str();
				card_download_list.push_back(t_df1);
				
				DownloadFile t_df2;
				t_df2.size = t_faceInfo["size"].asInt();
				t_df2.img = t_faceInfo["plist"].asString().c_str();
				t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
				t_df2.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str();
				card_download_list.push_back(t_df2);
				
				DownloadFile t_df3;
				t_df3.size = t_faceInfo["size"].asInt();
				t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
				t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
				t_df3.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str();
				card_download_list.push_back(t_df3);
				
//				if(!is_add_cf)
//				{
//					CopyFile t_cf;
//					t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//					t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//					card_reduction_list.push_back(t_cf);
//					
//					is_add_cf = true;
//				}
//				
//				CopyFile t_cf = card_reduction_list.back();
//				card_reduction_list.pop_back();
//				t_cf.is_ccb = true;
//				t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//				
//				card_reduction_list.push_back(t_cf);
			}
			mySDS->fFlush(t_card["piece"].asInt(), kSDS_SI_base);
		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		mySGD->resetHasGottenCards();
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		mySGD->resetHasGottenCards();
	}
	else
	{
		is_receive_fail = true;
		bool is_zero_version = false;
		for(int i=0;!is_zero_version && i<card_data_load_list.size();i++)
		{
			if(NSDS_GI(kSDS_CI_int1_version_i, card_data_load_list[i]) == 0)
				is_zero_version = true;
		}
		
		Json::Value card_param;
		card_param["version"] = (is_zero_version ? 0 : NSDS_GI(kSDS_GI_card_version_i));
		for(int i=0;i<card_data_load_list.size();i++)
			card_param["noList"][i] = card_data_load_list[i];
		command_list.push_back(CommandParam("getcardlistbylist", card_param, json_selector(this, TitleRenewalScene::resultLoadedCardData)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultGetPuzzleList( Json::Value result_data )
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		if(result_data["version"] > NSDS_GI(kSDS_GI_puzzleListVersion_i))
		{
			Json::Value puzzle_list = result_data["list"];
			
			NSDS_SI(kSDS_GI_puzzleListCount_i, puzzle_list.size(), false);
//			NSDS_SI(kSDS_GI_puzzleListCount_i, 3, false);
			
			int puzzle_cnt = puzzle_list.size();
			for(int i=0;i<puzzle_cnt;i++)
			{
				int puzzle_number = puzzle_list[i]["order"].asInt();
				
				if(NSDS_GI(kSDS_GI_puzzleListVersion_i) == 0)
				{
					NSDS_SI(puzzle_number, kSDS_PZ_version_i, 0);
				}
				
				NSDS_SI(kSDS_GI_puzzleList_int1_no_i, i+1, puzzle_number, false);
				NSDS_SS(kSDS_GI_puzzleList_int1_title_s, i+1, puzzle_list[i]["title"].asString().c_str(), false);
				NSDS_SS(puzzle_number, kSDS_PZ_title_s, puzzle_list[i]["title"].asString().c_str(), false);
				NSDS_SB(puzzle_number, kSDS_PZ_isEvent_b, puzzle_list[i]["isEvent"].asBool(), false);
//				NSDS_SI(kSDS_GI_puzzleList_int1_version_i, i+1, puzzle_list[i]["version"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_startStage_i, puzzle_list[i]["startStage"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_stageCount_i, puzzle_list[i]["stageCount"].asInt(), false);
				
				NSDS_SS(puzzle_number, kSDS_PZ_condition_s, puzzle_list[i]["condition"].asString(), false);
				
				NSDS_SI(puzzle_number, kSDS_PZ_color_r_d, puzzle_list[i]["color"]["r"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_color_g_d, puzzle_list[i]["color"]["g"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_color_b_d, puzzle_list[i]["color"]["b"].asInt(), false);
				
//				NSDS_SI(puzzle_number, kSDS_PZ_point_i, puzzle_list[i]["point"].asInt(), false);
//				NSDS_SI(puzzle_number, kSDS_PZ_ticket_i, puzzle_list[i]["ticket"].asInt(), false);
				
				Json::Value path_info = puzzle_list[i]["pathInfo"];
				Json::Value card_info = puzzle_list[i]["cardInfo"];
//				Json::Value reward_info = puzzle_list[i]["rewardInfo"];
//				Json::Value level_info = puzzle_list[i]["levelInfo"];
				Json::Value condition_info = puzzle_list[i]["conditionInfo"];
				int start_stage = puzzle_list[i]["startStage"].asInt();
				for(int j=0;j<path_info.size();j++)
				{
					NSDS_SI(start_stage+j, kSDS_SI_puzzle_i, puzzle_number, false);
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, start_stage + j, path_info[j].asInt(), false);
//					NSDS_SI(start_stage+j, kSDS_SI_level_i, level_info[j].asInt(), false);
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, start_stage+j, condition_info[j]["gold"].asInt(), false);
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, start_stage+j, condition_info[j]["no"].asInt(), false);
					Json::Value t_stage_cards = card_info[j];
					for(int k=0;k<t_stage_cards.size();k++)
					{
						NSDS_SI(start_stage+j, kSDS_SI_level_int1_card_i, k+1, t_stage_cards[k].asInt(), false);
//						NSDS_SI(kSDS_CI_int1_reward_i, t_stage_cards[k].asInt(), reward_info[j][k].asInt(), false);
					}
					mySDS->fFlush(start_stage+j, kSDS_SI_base);
				}
				mySDS->fFlush(kSDS_CI_base);

//				NSDS_SI(puzzle_number, kSDS_PZ_startWarp_x_d, puzzle_list[i]["startPosition"]["x"].asInt(), false);
//				NSDS_SI(puzzle_number, kSDS_PZ_startWarp_y_d, puzzle_list[i]["startPosition"]["y"].asInt(), false);
//				NSDS_SI(puzzle_number, kSDS_PZ_lastWarp_x_d, puzzle_list[i]["endPosition"]["x"].asInt(), false);
//				NSDS_SI(puzzle_number, kSDS_PZ_lastWarp_y_d, puzzle_list[i]["endPosition"]["y"].asInt(), false);
				
//				Json::Value coordinateInfo_list = puzzle_list[i]["coordinateInfo"];
//				for(int j=0;j<coordinateInfo_list.size();j++)
//				{
//					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_x_d, start_stage+j, coordinateInfo_list[j]["x"].asInt(), false);
//					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_y_d, start_stage+j, coordinateInfo_list[j]["y"].asInt(), false);
//				}
				
//				if(puzzle_number == 1 || myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1 >= puzzle_number)
//				{
//					if(NSDS_GS(puzzle_number, kSDS_PZ_map_s) != puzzle_list[i]["map"]["image"].asString())
//					{
//						DownloadFile t_df;
//						t_df.size = puzzle_list[i]["map"]["size"].asInt();
//						t_df.img = puzzle_list[i]["map"]["image"].asString().c_str();
//						t_df.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, "map")->getCString();
//						t_df.key = "map";
//						puzzle_download_list.push_back(t_df);
//						puzzle_download_list_puzzle_number.push_back(puzzle_number);
//					}
//					if(NSDS_GS(puzzle_number, kSDS_PZ_center_s) != puzzle_list[i]["center"]["image"].asString())
//					{
//						// check, after download ----------
//						DownloadFile t_df;
//						t_df.size = puzzle_list[i]["center"]["size"].asInt();
//						t_df.img = puzzle_list[i]["center"]["image"].asString().c_str();
//						t_df.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, "center")->getCString();
//						t_df.key = "center";
//						puzzle_download_list.push_back(t_df);
//						puzzle_download_list_puzzle_number.push_back(puzzle_number);
//						// ================================
//					}
//					if(NSDS_GS(puzzle_number, kSDS_PZ_original_s) != puzzle_list[i]["original"]["image"].asString())
//					{
//						DownloadFile t_cut;
//						t_cut.size = puzzle_list[i]["original"]["size"].asInt();
//						t_cut.img = puzzle_list[i]["original"]["image"].asString().c_str();
//						t_cut.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, "original")->getCString();
//						t_cut.key = "original";
//						puzzle_download_list.push_back(t_cut);
//						puzzle_download_list_puzzle_number.push_back(puzzle_number);
//					}
//					if(NSDS_GS(puzzle_number, kSDS_PZ_face_s) != puzzle_list[i]["face"]["image"].asString())
//					{
//						DownloadFile t_cut;
//						t_cut.size = puzzle_list[i]["face"]["size"].asInt();
//						t_cut.img = puzzle_list[i]["face"]["image"].asString().c_str();
//						t_cut.filename = CCSTR_CWF("puzzle%d_%s.png", puzzle_number, "face")->getCString();
//						t_cut.key = "face";
//						puzzle_download_list.push_back(t_cut);
//						puzzle_download_list_puzzle_number.push_back(puzzle_number);
//					}
//				}
				
				mySDS->fFlush(puzzle_list[i]["order"].asInt(), kSDS_PZ_base);
				
				if(myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number) == 0)
				{
					bool is_found = false;
					int found_stage_number = -1;
					int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
					for(int j = start_stage;j<start_stage+stage_count && !is_found;j++)
					{
						if(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, j) == 0 && NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, j) == 0)
						{
							is_found = true;
							found_stage_number = j;
						}
					}
					
					if(is_found)
						myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, found_stage_number);
					else
						myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, start_stage);
				}
				
				Json::Value thumbnail = puzzle_list[i]["thumbnail"];
				if(NSDS_GS(kSDS_GI_puzzleList_int1_thumbnail_s, i+1) != thumbnail["image"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = thumbnail["size"].asInt();
					t_df.img = thumbnail["image"].asString().c_str();
					t_df.filename = CCSTR_CWF("puzzleList%d_thumbnail.png", i+1)->getCString();
					t_df.key = CCSTR_CWF("puzzleList%d_thumbnail", i+1)->getCString();
					
					auto iter = find(puzzle_download_list.begin(), puzzle_download_list.end(), t_df);
					if(iter == puzzle_download_list.end())
						puzzle_download_list.push_back(t_df);
					// ================================
				}
			}
			
			if(puzzle_download_list.size() > 0)
				puzzlelist_download_version = result_data["version"].asInt();
			else
				NSDS_SI(kSDS_GI_puzzleListVersion_i, result_data["version"].asInt(), false);
		}
		
		mySDS->fFlush(kSDS_GI_characterCount_i);
		
		if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) == 0)
			myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, NSDS_GI(kSDS_GI_puzzleList_int1_no_i, 1));
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		
	}
	else
	{
		is_receive_fail = true;
		Json::Value puzzlelist_param;
		puzzlelist_param["version"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
		command_list.push_back(CommandParam("getpuzzlelist", puzzlelist_param, json_selector(this, TitleRenewalScene::resultGetPuzzleList)));
	}
	
	receive_cnt--;
	checkReceive();
}

void TitleRenewalScene::resultRequestProductInfos(Json::Value result_data)
{
    TRACE();
    if(result_data["issuccess"].asBool())
    {
        
        TRACE();
        mySGD->product_infos = result_data;
        
        is_loaded_productInfo = true;
        endingAction();
    }
    else
    {
        TRACE();
        addChild(KSTimer::create(1.f, [=]()
                                 {
                                     myHSP->requestProductInfos(this, json_selector(this, TitleRenewalScene::resultRequestProductInfos));
                                 }));
    }
    
    TRACE();
}

//void TitleRenewalScene::resultGetPathInfo(Json::Value result_data)
//{
//	if(result_data["result"]["code"].asInt() == GDSUCCESS)
//	{
//		Json::Value puzzle_list = result_data["puzzlelist"];
//		
//		for(int i=0;i<puzzle_list.size();i++)
//		{
//			Json::Value path_info = puzzle_list[i];
//			
//			int start_stage = path_info["stageStart"].asInt();
//			int puzzle_number = path_info["puzzleNo"].asInt();
//			
//			Json::Value path_list = path_info["path"];
//			
//			for(int j=0;j<path_list.size();j++)
//				NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, start_stage + j, path_list[j].asInt(), false);
//			
//			mySDS->fFlush(puzzle_number, kSDS_PZ_base);
//		}
//	}
//	else
//	{
//		is_receive_fail = true;
//		command_list.push_back(CommandParam("getpathinfo", Json::Value(), json_selector(this, TitleRenewalScene::resultGetPathInfo)));
//	}
//	
//	receive_cnt--;
//	checkReceive();
//}

void TitleRenewalScene::endingAction()
{
	CCLOG("ttttt is_loaded_cgp : %d | is_loaded_server : %d | is_preloaded_effect : %d | is_loaded_productInfo : %d", is_loaded_cgp, is_loaded_server, is_preloaded_effect, is_loaded_productInfo);
	
	if(is_loaded_cgp && is_loaded_server && is_preloaded_effect && is_loaded_productInfo)
	{
//	if(myDSH->getIntegerForKey(kDSH_Key_storyReadPoint) == 0)
//	{
//		StoryView* t_sv = StoryView::create();
//		t_sv->setFunc([=]()
//					  {
//						  myDSH->setIntegerForKey(kDSH_Key_storyReadPoint, 1);
//						  myDSH->saveAllUserData(nullptr);
//						  CCDelayTime* t_delay = CCDelayTime::create(2.f);
//						  CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleRenewalScene::changeScene));
//						  CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
//						  runAction(t_seq);
//					  });
//		addChild(t_sv);
//	}
//	else
//	{
		CCDelayTime* t_delay = CCDelayTime::create(2.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(TitleRenewalScene::changeScene));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
//	}
	}
}

void TitleRenewalScene::changeScene()
{
//	setBackKeyEnabled(false);
	
	TRACE();
	mySGD->is_safety_mode = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_init);
    GraphDog::get()->removeTarget(this);
	CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
//	CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
//	CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
//	CCDirector::sharedDirector()->replaceScene(PlayTutorial::scene());
}

void TitleRenewalScene::startFileDownloadSet()
{
	int max_thread_cnt = 5;
	int total_download_cnt = character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size();
	int before_download_size = download_set.size();
	
	for(int i=download_set.size();ing_download_cnt <= total_download_cnt && i<max_thread_cnt;i++)
	{
		DownloadImgInfo t_info;
		if(character_download_list.size() > 0 && ing_download_cnt <= character_download_list.size())
		{
			t_info.download_url = character_download_list[ing_download_cnt-1].img;
			t_info.download_size = character_download_list[ing_download_cnt-1].size;
			t_info.download_filename = character_download_list[ing_download_cnt-1].filename;
		}
		else if(monster_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size())
		{
			t_info.download_url = monster_download_list[ing_download_cnt-character_download_list.size()-1].img;
			t_info.download_size = monster_download_list[ing_download_cnt-character_download_list.size()-1].size;
			t_info.download_filename = monster_download_list[ing_download_cnt-character_download_list.size()-1].filename;
		}
		else if(card_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size())
		{
			t_info.download_url = card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img;
			t_info.download_size = card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].size;
			t_info.download_filename = card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].filename;
		}
		else if(puzzle_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
		{
			t_info.download_url = puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img;
			t_info.download_size = puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].size;
			t_info.download_filename = puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].filename;
		}
		else
		{
			break;
		}
		
		t_info.is_fail = false;
		t_info.is_end = false;
		download_set.push_back(t_info);
		
		ing_download_cnt++;
	}
	
	if(download_set.size() > 0)
	{
		for(int i=before_download_size;i<download_set.size();i++)
		{
			int will_download_index = is_enable_index.back();
			CCLOG("start download idx : %d / filename : %s", will_download_index, download_set[i].download_filename.c_str());
			mySIL->downloadImg(download_set[i], will_download_index);
			is_enable_index.pop_back();
		}
		if(!is_downloading)
		{
			is_downloading = true;
			schedule(schedule_selector(TitleRenewalScene::checkDownloading));
		}
	}
	else
	{
		unschedule(schedule_selector(TitleRenewalScene::checkDownloading));
		
		// reduce and divide
		
		// divide
		for(int j=0;j<puzzle_download_list.size() && j < puzzle_download_list_puzzle_number.size();j++)
		{
			if(puzzle_download_list[j].key == "map")
				continue;
			
			CCImage *img = new CCImage;
			img->initWithEncryptedImageFileFullPath((mySIL->getDocumentPath() + puzzle_download_list[j].filename).c_str()); //퍼즐이미지를 불러옵니다.
			
			CCImage *st_w, *st_h;
			
			if(puzzle_download_list[j].key == "face")
			{
				st_w = new CCImage;
				st_w->initWithImageFile("stage_scissor.png"); //피스조각(가로형)을 불러옵니다.
				
				st_h = new CCImage;
				st_h->initWithImageFile("stage_scissor.png"); //피스조각(세로형)을 불러옵니다.
			}
			else
			{
				st_w = new CCImage;
				st_w->initWithImageFile("temp_puzzle_stencil_pw.png"); //피스조각(가로형)을 불러옵니다.
				
				st_h = new CCImage;
				st_h->initWithImageFile("temp_puzzle_stencil_ph.png"); //피스조각(세로형)을 불러옵니다.
			}
			
			
			int puzzleCol=6,puzzleRow=4;
			float puzzleColDis=100.f, puzzleRowDis=100.f, puzzleOffsetX=76.f, puzzleOffsetY=76.f;
			float faceColDis=132.f, faceRowDis=132.f; //172, 172
			float puzzleWidth=652.f,puzzleHeight=452.f;
			
			int puzzle_number = puzzle_download_list_puzzle_number[j];
			
			for(int i=0;i<puzzleCol*puzzleRow;i++){
				//피스의 좌표를 구합니다. 퍼즐은 6*4 개로 이루어져있습니다.
				int x = i%puzzleCol;
				int y = i/puzzleCol;
				
				CCImage *st = st_h;
				//				if(i%2==0)st=st_w; //피스는 i가 짝수일때 st_w 이미지를 이용하여 자르고 홀수일때 st_h 이미지를 이용하여 자릅니다.
				if((x+(puzzleRow-1-y))%2 == 1)
					st=st_w;
				
				//저장할파일명을 지정합니다.
				string filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, puzzle_download_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol)+1)->getCString();
				
				//원본파일에서 자를 위치를 계산합니다.
				int cutx, cuty;
				if(puzzle_download_list[j].key == "face")
				{
					cutx = x*faceColDis+puzzleOffsetX;
					cuty = y*faceRowDis+puzzleOffsetY;
				}
				else if(puzzle_download_list[j].key == "original" || puzzle_download_list[j].key == "center")
				{
					cutx =x*puzzleColDis+puzzleOffsetX;
					cuty =y*puzzleRowDis+puzzleOffsetY;
				}
				
				//자르고 저장합니다.
				bool isSuccess = PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				//실패했으면 한번더 자르게 해줍니다.
				if(!isSuccess){
					i--;
					continue;
				}
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			st_w->release(); //가로 피스 메모리해제
			st_h->release(); //세로 피스 메모리해제
			
			//가장자리 자르기
			//위쪽부터 잘라봅니다.
			{
				CCImage *st = new CCImage;
				st->initWithImageFile("temp_puzzle_stencil_top.png");
				
				int cutx =puzzleWidth/2;
				int cuty =puzzleHeight-st->getHeight()/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			//아래쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				st->initWithImageFile("temp_puzzle_stencil_bottom.png");
				
				int cutx =puzzleWidth/2;
				int cuty =st->getHeight()/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			//왼쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				st->initWithImageFile("temp_puzzle_stencil_left.png");
				
				int cutx =st->getWidth()/2;
				int cuty =puzzleHeight/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			//오른쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				st->initWithImageFile("temp_puzzle_stencil_right.png");
				
				int cutx =puzzleWidth-st->getWidth()/2;
				int cuty =puzzleHeight/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			//메모리해제
			img->release();
		}
		
//        if(card_reduction_list.size() > 0)
//        {
//            reduce_frame = 0;
//            
//            schedule(schedule_selector(TitleRenewalScene::reduceAction));
//        }
//        else
//        {
            if(character_download_list.size() > 0)
            {
                for(int i=0;i<character_download_list.size();i++)
                {
                    SDS_SS(kSDF_gameInfo, character_download_list[i].key, character_download_list[i].img, false);
                }
                NSDS_SI(kSDS_GI_characterVersion_i, character_download_version, false);
                mySDS->fFlush(kSDS_GI_characterCount_i);
            }
            
            if(monster_download_list.size() > 0)
            {
                for(int i=0;i<monster_download_list.size();i++)
                {
                    SDS_SS(kSDF_gameInfo, monster_download_list[i].key, monster_download_list[i].img, false);
                }
                NSDS_SI(kSDS_GI_monsterVersion_i, monster_download_version, false);
                mySDS->fFlush(kSDS_GI_monsterCount_i);
            }
            
            if(card_download_list.size() > 0)
            {
                for(int i=0;i<card_download_list.size();i++)
                {
                    SDS_SS(kSDF_cardInfo, card_download_list[i].key,
                           card_download_list[i].img, false);
                }
                mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
            }
            
            if(puzzle_download_list.size() > 0)
            {
                for(int i=0;i<puzzle_download_list.size();i++)
                {
                    SDS_SS(kSDF_gameInfo, puzzle_download_list[i].key,
                           puzzle_download_list[i].img, false);
                }
                mySDS->fFlush(kSDS_GI_base);
            }
            
            endingCheck();
            
//        }
        
//		// reduce
//		for(int i=0;i<card_reduction_list.size();i++)
//		{
//			mySIL->removeTextureCache(card_reduction_list[i].from_filename);
//			mySIL->removeTextureCache(card_reduction_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(card_reduction_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
//			if(card_reduction_list[i].is_ani)
//			{
//				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].ani_filename.c_str()),
//																CCRectMake(0, 0, card_reduction_list[i].cut_width, card_reduction_list[i].cut_height));
//				ani_img->setPosition(ccp(card_reduction_list[i].position_x, card_reduction_list[i].position_y));
//				target_img->addChild(ani_img);
//			}
//			
//			if(card_reduction_list[i].is_ccb)
//			{
//				CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + card_reduction_list[i].ccb_filename.c_str()).first;
//				face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
//				target_img->addChild(face_img);
//			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->beginWithClear(0, 0, 0, 0);
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", card_reduction_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
//		
//		if(character_download_list.size() > 0)
//		{
//			for(int i=0;i<character_download_list.size();i++)
//			{
//				SDS_SS(kSDF_gameInfo, character_download_list[i].key, character_download_list[i].img, false);
//			}
//			NSDS_SI(kSDS_GI_characterVersion_i, character_download_version, false);
//			mySDS->fFlush(kSDS_GI_characterCount_i);
//		}
//		
//		if(monster_download_list.size() > 0)
//		{
//			for(int i=0;i<monster_download_list.size();i++)
//			{
//				SDS_SS(kSDF_gameInfo, monster_download_list[i].key, monster_download_list[i].img, false);
//			}
//			NSDS_SI(kSDS_GI_monsterVersion_i, monster_download_version, false);
//			mySDS->fFlush(kSDS_GI_monsterCount_i);
//		}
//		
//		if(card_download_list.size() > 0)
//		{
//			for(int i=0;i<card_download_list.size();i++)
//			{
//				SDS_SS(kSDF_cardInfo, card_download_list[i].key,
//					   card_download_list[i].img, false);
//			}
//			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//		}
//		
//		if(puzzle_download_list.size() > 0)
//		{
//			for(int i=0;i<puzzle_download_list.size();i++)
//			{
//				SDS_SS(kSDF_gameInfo, puzzle_download_list[i].key,
//					   puzzle_download_list[i].img, false);
//			}
//			mySDS->fFlush(kSDS_GI_base);
//		}
//		
//		endingCheck();
	}
}

void TitleRenewalScene::reduceAction()
{
//    float download_percent = 100.f*reduce_frame/card_reduction_list.size();
//	if(download_percent > 100.f)
//		download_percent = 100.f;
//    
//	download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
//	progress_timer->setPercentage(download_percent);
//    
//    int i = reduce_frame;
//    
//    mySIL->removeTextureCache(card_reduction_list[i].from_filename);
//    mySIL->removeTextureCache(card_reduction_list[i].to_filename);
//    
//    CCSprite* target_img = new CCSprite();
//    target_img->initWithTexture(mySIL->addImage(card_reduction_list[i].from_filename.c_str()));
//    target_img->setAnchorPoint(ccp(0,0));
//    
////    if(card_reduction_list[i].is_ani)
////    {
////        CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].ani_filename.c_str()),
////                                                        CCRectMake(0, 0, card_reduction_list[i].cut_width, card_reduction_list[i].cut_height));
////        ani_img->setPosition(ccp(card_reduction_list[i].position_x, card_reduction_list[i].position_y));
////        target_img->addChild(ani_img);
////    }
////    
////    if(card_reduction_list[i].is_ccb)
////    {
////        CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + card_reduction_list[i].ccb_filename.c_str()).first;
////        face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
////        target_img->addChild(face_img);
////    }
//    
//    target_img->setScale(0.4f);
//    
//    CCRenderTexture* t_texture = new CCRenderTexture();
//    t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//    t_texture->setSprite(target_img);
//    t_texture->beginWithClear(0, 0, 0, 0);
//    t_texture->getSprite()->visit();
//    t_texture->end();
//    
//    if(!(t_texture->saveToFileNoAlpha(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//        CCLOG("failed!!! card reduce : %s", card_reduction_list[i].to_filename.c_str());
//    
//    t_texture->release();
//    target_img->release();
//    
//    if(i % 3 == 0)
//    {
//        CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//    }
//    
//    reduce_frame++;
//    
//    
//    if(reduce_frame >= card_reduction_list.size())
//    {
//        unschedule(schedule_selector(TitleRenewalScene::reduceAction));
//        
//        if(character_download_list.size() > 0)
//		{
//			for(int i=0;i<character_download_list.size();i++)
//			{
//				SDS_SS(kSDF_gameInfo, character_download_list[i].key, character_download_list[i].img, false);
//			}
//			NSDS_SI(kSDS_GI_characterVersion_i, character_download_version, false);
//			mySDS->fFlush(kSDS_GI_characterCount_i);
//		}
//		
//		if(monster_download_list.size() > 0)
//		{
//			for(int i=0;i<monster_download_list.size();i++)
//			{
//				SDS_SS(kSDF_gameInfo, monster_download_list[i].key, monster_download_list[i].img, false);
//			}
//			NSDS_SI(kSDS_GI_monsterVersion_i, monster_download_version, false);
//			mySDS->fFlush(kSDS_GI_monsterCount_i);
//		}
//		
//		if(card_download_list.size() > 0)
//		{
//			for(int i=0;i<card_download_list.size();i++)
//			{
//				SDS_SS(kSDF_cardInfo, card_download_list[i].key,
//					   card_download_list[i].img, false);
//			}
//			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//		}
//		
//		if(puzzle_download_list.size() > 0)
//		{
//			for(int i=0;i<puzzle_download_list.size();i++)
//			{
//				SDS_SS(kSDF_gameInfo, puzzle_download_list[i].key,
//					   puzzle_download_list[i].img, false);
//			}
//			mySDS->fFlush(kSDS_GI_base);
//		}
//        
//		endingCheck();
//    }
}

void TitleRenewalScene::checkDownloading()
{
	int max_thread_cnt = 5;
	bool is_successed = false;
	for(int i=0;i<max_thread_cnt;i++)
	{
		if(mySIL->downloading_list[i].is_end)
		{
			if(mySIL->downloading_list[i].is_fail)
			{
				CCLOG("fail download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					CCLOG("start download idx : %d / filename : %s", i, iter->download_filename.c_str());
					mySIL->downloadImg((*iter), i);
				}
				else
				{
					CCLOG("not found fail set");
				}
			}
			else
			{
				CCLOG("success download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					download_set.erase(iter);
				}
				else
				{
					CCLOG("not found success set");
				}
				is_enable_index.push_back(i);
				success_download_cnt++;
				is_successed = true;
			}
		}
	}
	
	float download_percent = 100.f*success_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
	if(download_percent > 100.f)
		download_percent = 100.f;
	download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
	
	progress_timer->setPercentage(download_percent);
//	progress_timer->stopAllActions();
//	CCProgressFromTo* t_to = CCProgressFromTo::create(0.2f, progress_timer->getPercentage(), download_percent);
//	progress_timer->runAction(t_to);
	
	if(is_successed || success_download_cnt >= character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		startFileDownloadSet();
	}
}

//void TitleRenewalScene::successDownloadActionSet(string t_filename)
//{
//	success_download_cnt++;
//	float download_percent = 100.f*success_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
//	if(download_percent > 100.f)
//		download_percent = 100.f;
//	download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
//	
//	progress_timer->stopAllActions();
//	CCProgressFromTo* t_to = CCProgressFromTo::create(0.5f, progress_timer->getPercentage(), download_percent);
//	progress_timer->runAction(t_to);
//
//	vector<DownloadImgInfo>::iterator iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(t_filename));
//	
//	if(iter != download_set.end())
//	{
//		download_set.erase(iter);
//	}
//	
//	rest_download_cnt--;
//	
//	if(rest_download_cnt <= 0)
//	{
//		if(ing_download_cnt > character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size() && download_set.size() == 0)
//		{
//			// reduce and divide
//			
//			// reduce
//			for(int i=0;i<card_reduction_list.size();i++)
//			{
//				CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].from_filename.c_str()));
//				target_img->setAnchorPoint(ccp(0,0));
//				
//				if(card_reduction_list[i].is_ani)
//				{
//					CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].ani_filename.c_str()),
//																	CCRectMake(0, 0, card_reduction_list[i].cut_width, card_reduction_list[i].cut_height));
//					ani_img->setPosition(ccp(card_reduction_list[i].position_x, card_reduction_list[i].position_y));
//					target_img->addChild(ani_img);
//				}
//				
//				target_img->setScale(0.2f);
//				
//				CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
//				t_texture->setSprite(target_img);
//				t_texture->begin();
//				t_texture->getSprite()->visit();
//				t_texture->end();
//				
//				t_texture->saveToFile(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG);
//			}
//			
//			// divide
//			for(int j=0;j<puzzle_download_list.size() && j < puzzle_download_list_puzzle_number.size();j++)
//			{
//				if(puzzle_download_list[j].key == "map")
//					continue;
//				
//				CCImage *img = new CCImage;
//				img->initWithImageFileThreadSafe((mySIL->getDocumentPath() + puzzle_download_list[j].filename).c_str()); //퍼즐이미지를 불러옵니다.
//				
//				CCImage *st_w, *st_h;
//				
//				if(puzzle_download_list[j].key == "face")
//				{
//					st_w = new CCImage;
//					st_w->initWithImageFile("stage_scissor.png"); //피스조각(가로형)을 불러옵니다.
//					
//					st_h = new CCImage;
//					st_h->initWithImageFile("stage_scissor.png"); //피스조각(세로형)을 불러옵니다.
//				}
//				else
//				{
//					st_w = new CCImage;
//					st_w->initWithImageFile("temp_puzzle_stencil_pw.png"); //피스조각(가로형)을 불러옵니다.
//					
//					st_h = new CCImage;
//					st_h->initWithImageFile("temp_puzzle_stencil_ph.png"); //피스조각(세로형)을 불러옵니다.
//				}
//				
//				
//				int puzzleCol=6,puzzleRow=4;
//				float puzzleColDis=100.f, puzzleRowDis=100.f, puzzleOffsetX=76.f, puzzleOffsetY=76.f;
//				float faceColDis=132.f, faceRowDis=132.f; //172, 172
//				float puzzleWidth=652.f,puzzleHeight=452.f;
//				
//				int puzzle_number = puzzle_download_list_puzzle_number[j];
//				
//				for(int i=0;i<puzzleCol*puzzleRow;i++){
//					//피스의 좌표를 구합니다. 퍼즐은 6*4 개로 이루어져있습니다.
//					int x = i%puzzleCol;
//					int y = i/puzzleCol;
//					
//					CCImage *st = st_h;
//					//				if(i%2==0)st=st_w; //피스는 i가 짝수일때 st_w 이미지를 이용하여 자르고 홀수일때 st_h 이미지를 이용하여 자릅니다.
//					if((x+(puzzleRow-1-y))%2 == 1)
//						st=st_w;
//					
//					//저장할파일명을 지정합니다.
//					string filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, puzzle_download_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol)+1)->getCString();
//					
//					//원본파일에서 자를 위치를 계산합니다.
//					int cutx, cuty;
//					if(puzzle_download_list[j].key == "face")
//					{
//						cutx = x*faceColDis+puzzleOffsetX;
//						cuty = y*faceRowDis+puzzleOffsetY;
//					}
//					else if(puzzle_download_list[j].key == "original" || puzzle_download_list[j].key == "center")
//					{
//						cutx =x*puzzleColDis+puzzleOffsetX;
//						cuty =y*puzzleRowDis+puzzleOffsetY;
//					}
//					
//					//자르고 저장합니다.
//					bool isSuccess = PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					//실패했으면 한번더 자르게 해줍니다.
//					if(!isSuccess){
//						i--;
//						continue;
//					}
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				
//				st_w->release(); //가로 피스 메모리해제
//				st_h->release(); //세로 피스 메모리해제
//				
//				//가장자리 자르기
//				//위쪽부터 잘라봅니다.
//				{
//					CCImage *st = new CCImage;
//					st->initWithImageFile("temp_puzzle_stencil_top.png");
//					
//					int cutx =puzzleWidth/2;
//					int cuty =puzzleHeight-st->getHeight()/2;
//					
//					string filename =CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
//					PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					st->release(); //메모리해제
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				
//				//아래쪽 잘라봅니다.
//				{
//					CCImage *st = new CCImage;
//					st->initWithImageFile("temp_puzzle_stencil_bottom.png");
//					
//					int cutx =puzzleWidth/2;
//					int cuty =st->getHeight()/2;
//					
//					string filename =CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
//					PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					st->release(); //메모리해제
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				
//				//왼쪽 잘라봅니다.
//				{
//					CCImage *st = new CCImage;
//					st->initWithImageFile("temp_puzzle_stencil_left.png");
//					
//					int cutx =st->getWidth()/2;
//					int cuty =puzzleHeight/2;
//					
//					string filename =CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
//					PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					st->release(); //메모리해제
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				//오른쪽 잘라봅니다.
//				{
//					CCImage *st = new CCImage;
//					st->initWithImageFile("temp_puzzle_stencil_right.png");
//					
//					int cutx =puzzleWidth-st->getWidth()/2;
//					int cuty =puzzleHeight/2;
//					
//					string filename =CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
//					PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
//					
//					st->release(); //메모리해제
//					
//					//테스트로 한번 붙여봅니다.
//					//				CCSprite *spr =  mySIL->getLoadedImg(filename);
//					//				spr->setAnchorPoint(ccp(0.5,0.5));
//					//				spr->setPosition(ccp(cutx/2,cuty/2));
//					//				addChild(spr,1000);
//				}
//				//메모리해제
//				img->release();
//			}
//			
//			if(character_download_list.size() > 0)
//			{
//				for(int i=0;i<character_download_list.size();i++)
//				{
//					SDS_SS(kSDF_gameInfo, character_download_list[i].key, character_download_list[i].img, false);
//				}
//				NSDS_SI(kSDS_GI_characterVersion_i, character_download_version, false);
//				mySDS->fFlush(kSDS_GI_characterCount_i);
//			}
//			
//			if(monster_download_list.size() > 0)
//			{
//				for(int i=0;i<monster_download_list.size();i++)
//				{
//					SDS_SS(kSDF_gameInfo, monster_download_list[i].key, monster_download_list[i].img, false);
//				}
//				NSDS_SI(kSDS_GI_monsterVersion_i, monster_download_version, false);
//				mySDS->fFlush(kSDS_GI_monsterCount_i);
//			}
//			
//			if(card_download_list.size() > 0)
//			{
//				for(int i=0;i<card_download_list.size();i++)
//				{
//					SDS_SS(kSDF_cardInfo, card_download_list[i].key,
//						   card_download_list[i].img, false);
//				}
//				mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//			}
//			
//			if(puzzle_download_list.size() > 0)
//			{
//				for(int i=0;i<puzzle_download_list.size();i++)
//				{
//					SDS_SS(kSDF_gameInfo, puzzle_download_list[i].key,
//						   puzzle_download_list[i].img, false);
//				}
//				mySDS->fFlush(kSDS_GI_base);
//			}
//			
//			endingCheck();
//		}
//		else
//		{
//			startFileDownloadSet();
//		}
//	}
//}
//
//void TitleRenewalScene::failDownloadActionSet(string t_filename)
//{
//	CCLOG("download fail : %s", t_filename.c_str());
//	
//	rest_download_cnt--;
//	
//	if(rest_download_cnt <= 0)
//	{
//		startFileDownloadSet();
//	}
//}

void TitleRenewalScene::startFileDownload()
{
	if(character_download_list.size() > 0 && ing_download_cnt <= character_download_list.size())
	{
		CCLOG("%d : %s", ing_download_cnt, character_download_list[ing_download_cnt-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(character_download_list[ing_download_cnt-1].img,
													  character_download_list[ing_download_cnt-1].size,
													  character_download_list[ing_download_cnt-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
	}
	else if(monster_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size())
	{
		CCLOG("%d : %s", ing_download_cnt, monster_download_list[ing_download_cnt-character_download_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(monster_download_list[ing_download_cnt-character_download_list.size()-1].img,
													  monster_download_list[ing_download_cnt-character_download_list.size()-1].size,
													  monster_download_list[ing_download_cnt-character_download_list.size()-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
	}
	else if(card_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size())
	{
		CCLOG("%d : %s", ing_download_cnt, card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img,
													  card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].size,
													  card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
	}
	else if(puzzle_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		CCLOG("%d : %s", ing_download_cnt, puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img,
													  puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].size,
													  puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
	}
	else
	{
		endingCheck();
	}
}

void TitleRenewalScene::successDownloadAction()
{
	if(ing_download_cnt < character_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, character_download_list[ing_download_cnt-1].key, character_download_list[ing_download_cnt-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
		if(download_percent > 100.f)
			download_percent = 100.f;
		download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, character_download_list[ing_download_cnt-1].key, character_download_list[ing_download_cnt-1].img, false);
		NSDS_SI(kSDS_GI_characterVersion_i, character_download_version, false);
		mySDS->fFlush(kSDS_GI_characterCount_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
		if(download_percent > 100.f)
			download_percent = 100.f;
		download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, monster_download_list[ing_download_cnt-character_download_list.size()-1].key, monster_download_list[ing_download_cnt-character_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
		if(download_percent > 100.f)
			download_percent = 100.f;
		download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, monster_download_list[ing_download_cnt-character_download_list.size()-1].key, monster_download_list[ing_download_cnt-character_download_list.size()-1].img, false);
		NSDS_SI(kSDS_GI_monsterVersion_i, monster_download_version, false);
		mySDS->fFlush(kSDS_GI_monsterCount_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
		if(download_percent > 100.f)
			download_percent = 100.f;
		download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size() + card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].key,
			   card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
		if(download_percent > 100.f)
			download_percent = 100.f;
		download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size() + card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].key,
			   card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img, false);
//		for(int i=0;i<card_reduction_list.size();i++)
//		{
//			mySIL->removeTextureCache(card_reduction_list[i].from_filename);
//			mySIL->removeTextureCache(card_reduction_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(card_reduction_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
////			if(card_reduction_list[i].is_ani)
////			{
////				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].ani_filename.c_str()),
////																CCRectMake(0, 0, card_reduction_list[i].cut_width, card_reduction_list[i].cut_height));
////				ani_img->setPosition(ccp(card_reduction_list[i].position_x, card_reduction_list[i].position_y));
////				target_img->addChild(ani_img);
////			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->begin();
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", card_reduction_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
		if(download_percent > 100.f)
			download_percent = 100.f;
		download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].key,
			   puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
		if(download_percent > 100.f)
			download_percent = 100.f;
		download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].key,
			   puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		
		if(ing_download_cnt <= int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))
		{
			float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
			if(download_percent > 100.f)
				download_percent = 100.f;
			download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
		}
		
		
		for(int j=0;j<puzzle_download_list.size() && j < puzzle_download_list_puzzle_number.size();j++)
		{
			if(puzzle_download_list[j].key == "map")
				continue;
			
			CCImage *img = new CCImage;
			img->initWithEncryptedImageFileFullPath((mySIL->getDocumentPath() + puzzle_download_list[j].filename).c_str()); //퍼즐이미지를 불러옵니다.
			
			CCImage *st_w, *st_h;
			
			if(puzzle_download_list[j].key == "face")
			{
				st_w = new CCImage;
				st_w->initWithImageFile("stage_scissor.png"); //피스조각(가로형)을 불러옵니다.
				
				st_h = new CCImage;
				st_h->initWithImageFile("stage_scissor.png"); //피스조각(세로형)을 불러옵니다.
			}
			else
			{
				st_w = new CCImage;
				st_w->initWithImageFile("temp_puzzle_stencil_pw.png"); //피스조각(가로형)을 불러옵니다.
				
				st_h = new CCImage;
				st_h->initWithImageFile("temp_puzzle_stencil_ph.png"); //피스조각(세로형)을 불러옵니다.
			}
			
			
			int puzzleCol=6,puzzleRow=4;
			float puzzleColDis=100.f, puzzleRowDis=100.f, puzzleOffsetX=76.f, puzzleOffsetY=76.f;
			float faceColDis=132.f, faceRowDis=132.f; //172, 172
			float puzzleWidth=652.f,puzzleHeight=452.f;
			
			int puzzle_number = puzzle_download_list_puzzle_number[j];
			
			for(int i=0;i<puzzleCol*puzzleRow;i++){
				//피스의 좌표를 구합니다. 퍼즐은 6*4 개로 이루어져있습니다.
				int x = i%puzzleCol;
				int y = i/puzzleCol;
				
				CCImage *st = st_h;
//				if(i%2==0)st=st_w; //피스는 i가 짝수일때 st_w 이미지를 이용하여 자르고 홀수일때 st_h 이미지를 이용하여 자릅니다.
				if((x+(puzzleRow-1-y))%2 == 1)
					st=st_w;
				
				//저장할파일명을 지정합니다.
				string filename =CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, puzzle_download_list[j].key.c_str(), (x+(puzzleRow-1-y)*puzzleCol)+1)->getCString();
				
				//원본파일에서 자를 위치를 계산합니다.
				int cutx, cuty;
				if(puzzle_download_list[j].key == "face")
				{
					cutx = x*faceColDis+puzzleOffsetX;
					cuty = y*faceRowDis+puzzleOffsetY;
				}
				else if(puzzle_download_list[j].key == "original" || puzzle_download_list[j].key == "center")
				{
					cutx =x*puzzleColDis+puzzleOffsetX;
					cuty =y*puzzleRowDis+puzzleOffsetY;
				}
				
				//자르고 저장합니다.
				bool isSuccess = PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				//실패했으면 한번더 자르게 해줍니다.
				if(!isSuccess){
					i--;
					continue;
				}
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			st_w->release(); //가로 피스 메모리해제
			st_h->release(); //세로 피스 메모리해제
			
			//가장자리 자르기
			//위쪽부터 잘라봅니다.
			{
				CCImage *st = new CCImage;
				st->initWithImageFile("temp_puzzle_stencil_top.png");
				
				int cutx =puzzleWidth/2;
				int cuty =puzzleHeight-st->getHeight()/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			//아래쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				st->initWithImageFile("temp_puzzle_stencil_bottom.png");
				
				int cutx =puzzleWidth/2;
				int cuty =st->getHeight()/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			
			//왼쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				st->initWithImageFile("temp_puzzle_stencil_left.png");
				
				int cutx =st->getWidth()/2;
				int cuty =puzzleHeight/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			//오른쪽 잘라봅니다.
			{
				CCImage *st = new CCImage;
				st->initWithImageFile("temp_puzzle_stencil_right.png");
				
				int cutx =puzzleWidth-st->getWidth()/2;
				int cuty =puzzleHeight/2;
				
				string filename =CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, puzzle_download_list[j].key.c_str())->getCString();
				PuzzleCache::getInstance()->cutImageAndSave(st, img, {cutx,cuty}, true,mySIL->getDocumentPath().c_str()+filename);
				
				st->release(); //메모리해제
				
				//테스트로 한번 붙여봅니다.
				//				CCSprite *spr =  mySIL->getLoadedImg(filename);
				//				spr->setAnchorPoint(ccp(0.5,0.5));
				//				spr->setPosition(ccp(cutx/2,cuty/2));
				//				addChild(spr,1000);
			}
			//메모리해제
			img->release();
		}
		
		mySDS->fFlush(kSDS_GI_base);
		
		endingCheck();
	}
	
	float rate = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
	if(rate > 100.f)
		rate = 100.f;
	else if(rate < 0.f)
		rate = 0.f;

	progress_timer->setPercentage(rate);
//	CCProgressFromTo* t_to = CCProgressFromTo::create(0.5f, progress_timer->getPercentage(), rate);
//	progress_timer->runAction(t_to);
}

void TitleRenewalScene::endingCheck()
{
	if(card_data_version != -1)
		NSDS_SI(kSDS_GI_card_version_i, card_data_version, false);
	
	if(hell_data_version != -1)
		NSDS_SI(kSDS_GI_hellMode_version_i, hell_data_version, false);
	
	if(puzzle_download_list.size() > 0)
		NSDS_SI(kSDS_GI_puzzleListVersion_i, puzzlelist_download_version, false);
	
	mySDS->fFlush(kSDS_GI_characterCount_i);
	
	is_loaded_server = true;
	
	endingAction();
}

void TitleRenewalScene::downloadingFileAction()
{
	float t_per = StageImgLoader::sharedInstance()->getDownloadPercentage();
	
	if(t_per < 0.f)			t_per = 0.f;
	else if(t_per > 1.f)	t_per = 1.f;
	
	ing_download_per = t_per;
	
	float download_percent = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
	if(download_percent > 100.f)
		download_percent = 100.f;
	download_state->setString(CCSTR_CWF("%.0f%%", download_percent)->getCString());
}

void TitleRenewalScene::failDownloadAction()
{
	state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_downImgFail));
	
	CommonButton* replay_menu = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, kCCMenuHandlerPriority);
	replay_menu->setPosition(ccp(240,160));
	replay_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kTitleRenewal_MT_redown);
								 menuAction(t_node);
							 });
	
	addChild(replay_menu, 99, kTitleRenewal_MT_redown);
	
	is_menu_enable = true;
}

void TitleRenewalScene::editBoxEditingDidBegin(CCEditBox* editBox)
{
	CCLOG("edit begin");
}
void TitleRenewalScene::editBoxEditingDidEnd(CCEditBox* editBox)
{
	CCLOG("edit end");
}
void TitleRenewalScene::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
	CCLOG("edit changed : %s", text.c_str());
}
void TitleRenewalScene::editBoxReturn(CCEditBox* editBox)
{
	CCLOG("edit return");
}

void TitleRenewalScene::menuAction( CCObject* sender )
{
	if(!is_menu_enable)		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kTitleRenewal_MT_replay)
	{
		removeChildByTag(kTitleRenewal_MT_replay);
		//		(save_target->*save_delegate)();
		startCommand();
	}
	else if(tag == kTitleRenewal_MT_redown)
	{
		removeChildByTag(kTitleRenewal_MT_redown);
		state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_downImgInfo));
		ing_download_cnt--;
		
		if(ing_download_cnt < 1)
			ing_download_cnt = 1;
		
		ing_download_per = 0;
		is_downloading = true;
		startFileDownload();
	}
	else if(tag == kTitleRenewal_MT_nick)
	{
		string comp_not_ok = "";
		if(input_text->getText() != comp_not_ok)
		{
			joinAction();
		}
		else
		{
			
			addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_nicknameError),
																					myLoc->getLocalForKey(LK::kMyLocalKey_shortNick), nullptr, CCPointZero, true), 999);
			
			is_menu_enable = true;
		}
	}
	else if(tag >= kTitleRenewal_MT_puzzleBase)
	{
//		setBackKeyEnabled(false);
		tag -= kTitleRenewal_MT_puzzleBase;
		
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, tag);
        GraphDog::get()->removeTarget(this);
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
}

void TitleRenewalScene::joinAction()
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["nick"] = input_text->getText();
	param["flag"] = flag->getFlag();
	hspConnector::get()->command("join", param, [=](Json::Value result_data)
								 {
									 if(result_data["result"]["code"].asInt() == GDSUCCESS)
									 {
										 //state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_successLogin));
										 myDSH->setStringForKey(kDSH_Key_nick, input_text->getText());
                                         myDSH->setStringForKey(kDSH_Key_flag, flag->getFlag());
										 setTouchEnabled(false);
										 nick_back->removeFromParent();
										 removeChildByTag(kTitleRenewal_MT_nick);
										 //input_text->removeFromParent();
										 //flag->removeFromParent();
//										 myDSH->saveUserData({kSaveUserData_Key_nick}, nullptr);
										 
										
										 
										 successLogin();
									 }
									 else if(result_data["result"]["code"].asInt() == GDDUPLICATEDNICK)
									 {
										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_nicknameError),
																												 myLoc->getLocalForKey(LK::kMyLocalKey_sameNick), nullptr, CCPointZero, true), 999);
										 
										 is_menu_enable = true;
									 }
									 else if(result_data["result"]["code"].asInt() == GDFAULTYNICK)
									 {
										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_nicknameError),
																												 myLoc->getLocalForKey(LK::kMyLocalKey_invalidNick), nullptr, CCPointZero, true), 999);

										 is_menu_enable = true;
									 }
									 else if(result_data["result"]["code"].asInt() == GDALREADYMEMBER)
									 {
										 //state_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_successLogin));
										 myDSH->setStringForKey(kDSH_Key_nick, input_text->getText());
                                         myDSH->setStringForKey(kDSH_Key_flag, flag->getFlag());
										 setTouchEnabled(false);
										 nick_back->removeFromParent();
										 removeChildByTag(kTitleRenewal_MT_nick);
										 //input_text->removeFromParent();
										 //flag->removeFromParent();
										 
//										 myDSH->saveUserData({kSaveUserData_Key_nick}, nullptr);
										 
										 successLogin();
									 }
									 else if(result_data["result"]["code"].asInt() == GDLONGNAME)
									 {
										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_nicknameError),
																												 myLoc->getLocalForKey(LK::kMyLocalKey_longNick), nullptr, CCPointZero, true), 999);
										 
//										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_longNick)), 999);
										 is_menu_enable = true;
									 }
									 else if(result_data["result"]["code"].asInt() == GDSHORTNAME)
									 {
										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_nicknameError),
															myLoc->getLocalForKey(LK::kMyLocalKey_shortNick), nullptr, CCPointZero, true), 999);
										 is_menu_enable = true;
									 }
									 else
									 {
										 joinAction();
									 }
								 });
}

void TitleRenewalScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void TitleRenewalScene::keyBackClicked()
{
	
	CommonButton::callBackKey();
	//AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(TitleRenewalScene::alertAction));
//	onBackKeyAction();
}
