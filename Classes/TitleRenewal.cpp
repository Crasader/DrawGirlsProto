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

CCScene* TitleRenewalScene::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::create();
	
	// 'layer' is an autorelease object
	TitleRenewalScene *layer = TitleRenewalScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
	// add layer as a child to scene
	scene->addChild(layer);
	
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
	
	is_preloaded_effect = false;
	
//	std::chrono::time_point<std::chrono::system_clock> recent;
//    recent = std::chrono::system_clock::now();
//	std::time_t recent_time = std::chrono::system_clock::to_time_t(recent);
//	CCLOG("title init : %d", int(recent_time));
	
	AudioEngine::sharedInstance()->stopSound();
	
	is_menu_enable = false;

	white_back = CCSprite::create("whitePaper.png");
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	white_back->setScaleX(screen_scale_x);
	white_back->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	white_back->setPosition(ccp(240,160));
	addChild(white_back);
	
	auto splash = KS::loadCCBI<CCSprite*>(this, "splash_nhn.ccbi");
	splash.second->setAnimationCompletedCallbackLambda(this, [=](const char* seqName){
		splash.first->removeFromParent();
		endSplash();
	});
	splash.first->setPosition(ccp(240,160));
	
	addChild(splash.first);
	
	//	addChild(KSTimer::create(3.f, [=]()
	//	{
	splash.second->runAnimationsForSequenceNamed("Default Timeline");
	//	}));
	
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
//	CCSprite* ratings = CCSprite::create("game_ratings.png");
//	ratings->setPosition(ccp(240,160));
//	addChild(ratings);
//	
//	addChild(KSTimer::create(1.5f, [=]()
//	{
		realInit();
//	}));
}

void TitleRenewalScene::realInit()
{
	title_img = CCSprite::create("temp_title_back.png");
	title_img->setPosition(ccp(240,160));
	addChild(title_img);
	
	title_name = CCSprite::create("temp_title_name.png");
	title_name->setAnchorPoint(ccp(0.5,0));
	title_name->setPosition(ccp(240,50));//10));
	addChild(title_name, 1);
	
	CCPoint convert_position = CCPointZero;
	
	convert_position.x = (480/myDSH->screen_convert_rate - 480)/2.f;
	if(myDSH->ui_top > 320)
		convert_position.y = (myDSH->ui_top - 320)/2.f;
	
	CCSprite* ratings = CCSprite::create("game_ratings.png");
	ratings->setAnchorPoint(ccp(1,1));
	ratings->setPosition(ccpFromSize(title_img->getContentSize()/2.f) + ccp(240-10, 160-10) + convert_position);
	title_img->addChild(ratings);
	
	state_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_connectingServer), mySGD->getFont().c_str(), 15, CCSizeMake(350, 80), kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter);
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
	
	black_img = CCSprite::create("whitePaper.png");
	black_img->setColor(ccBLACK);
	black_img->setOpacity(0);
	black_img->setPosition(ccp(240,160));
	black_img->setScaleX(screen_scale_x);
	black_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(black_img, 3);
	
	Json::Value param;
	param["ManualLogin"] = true;
	
	hspConnector::get()->login(param, param, std::bind(&TitleRenewalScene::resultLogin, this, std::placeholders::_1));
	
	white_back->removeFromParent();
	
}

void TitleRenewalScene::resultLogin( Json::Value result_data )
{
	CCLOG("resultLogin data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
	if(result_data["error"]["isSuccess"].asBool())
	{
		Json::Value param;
		param["memberID"] = hspConnector::get()->getSocialID();
		hspConnector::get()->command("login", param, json_selector(this, TitleRenewalScene::resultHSLogin));
	}
	else
	{
		Json::Value param;
		param["ManualLogin"] = true;
		
		hspConnector::get()->login(param, param, std::bind(&TitleRenewalScene::resultLogin, this, std::placeholders::_1));
	}
}

void TitleRenewalScene::resultHSLogin(Json::Value result_data)
{
	GraphDogLib::JsonToLog("resultHSLogin", result_data);
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		successLogin();
	}
	else if(result_data["result"]["code"].asInt() == GDNEEDJOIN)
	{
		
		
		is_menu_enable = true;
		
		
		state_label->setString(myLoc->getLocalForKey(kMyLocalKey_connectingServer));
		
		
		nick_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));// CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
		nick_back->setContentSize(CCSizeMake(270,150));
		nick_back->setPosition(ccp(240,220));
//nick_back->setScale(0)
		addChild(nick_back,100);

		CCScale9Sprite* flag_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		flag_back->setContentSize(CCSizeMake(240, 40));
		flag_back->setPosition(ccp(nick_back->getContentSize().width/2.f,76));
		nick_back->addChild(flag_back);

		CCScale9Sprite* nick_case = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		nick_case->setContentSize(CCSizeMake(236,35));
		nick_case->setPosition(ccp(nick_back->getContentSize().width/2.f,35));
		nick_back->addChild(nick_case);
		
		CCScale9Sprite* t_back = CCScale9Sprite::create("nickname_box.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		t_back->setOpacity(0);
//		t_back->setInsetBottom(16);
//		t_back->setInsetTop(34-16*2);
//		t_back->setInsetLeft(13);
//		t_back->setInsetRight(34-13*2);
		
		input_text = CCEditBox::create(CCSizeMake(160, 35), t_back);
		input_text->setPosition(ccp(110,35));
		input_text->setPlaceHolder(myLoc->getLocalForKey(kMyLocalKey_inputPlease));
		input_text->setReturnType(kKeyboardReturnTypeDone);
		input_text->setFont(mySGD->getFont().c_str(), 15);
		input_text->setInputMode(kEditBoxInputModeSingleLine);
		input_text->setDelegate(this);
		nick_back->addChild(input_text,3);
		//FormSetter::get()->addObject("t1", input_text);
		
		flag = FlagSelector::create();
		flag->setPosition(35,60);
		nick_back->addChild(flag,100000);
		//FormSetter::get()->addObject("t2", flag);
		
		CommonButton* ok_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ok), 14, CCSizeMake(60, 35), CCScale9Sprite::create("nickname_ok.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1)), kCCMenuHandlerPriority);
		ok_menu->setPosition(ccp(221,35));
		ok_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kTitleRenewal_MT_nick);
								 menuAction(t_node);
							 });
		nick_back->addChild(ok_menu, 10, kTitleRenewal_MT_nick);
		
		
//		nick_back->setPosition(ccp(240,-500));
	//	nick_back->runAction(CCEaseBounceOut::create(CCMoveTo::create(0.3, ccp(240,220))));
		//FormSetter::get()->addObject("t3", ok_menu);
	}
	else if(result_data["result"]["code"].asInt() == GDBLOCKEDUSER)
	{
		
		CCLog("ffff %s",result_data["blockReason"].asString().c_str());
		CCLog("ffff %s",CCString::createWithFormat("%s \n\n %s",myLoc->getLocalForKey(kMyLocalKey_blockedMsg),result_data["blockReason"].asString().c_str())->getCString());
		
		ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_blocked), CCString::createWithFormat("%s \n%s",myLoc->getLocalForKey(kMyLocalKey_blockedMsg),result_data["blockReason"].asString().c_str())->getCString(),[](){
			//mySGD->resetLabels();
			//CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
		});
		((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
	}
	else
	{
		Json::Value param;
		param["memberID"] = hspConnector::get()->getSocialID();
		hspConnector::get()->command("login", param, json_selector(this, TitleRenewalScene::resultLogin));
	}
}

void TitleRenewalScene::successLogin()
{
	addChild(KSTimer::create(1.f/60.f, [=]()
	{
//		AudioEngine::sharedInstance()->preloadEffectScene("Title");
		state_label->setString(myLoc->getLocalForKey(kMyLocalKey_titleTempScript1));
		
		addChild(KSTimer::create(1.f/60.f, [=]()
		{
			AudioEngine::sharedInstance()->preloadEffectTitleStep(1);
			
			addChild(KSTimer::create(20.f/60.f, [=]()
			{
				state_label->setString(myLoc->getLocalForKey(kMyLocalKey_titleTempScript2));
				
				addChild(KSTimer::create(1.f/60.f, [=]()
				{
					AudioEngine::sharedInstance()->preloadEffectTitleStep(2);
					
					addChild(KSTimer::create(1.f/60.f, [=]()
					{
						state_label->setString(myLoc->getLocalForKey(kMyLocalKey_titleTempScript3));
						
						addChild(KSTimer::create(1.f/60.f, [=]()
						{
							AudioEngine::sharedInstance()->preloadEffectTitleStep(3);
							
							addChild(KSTimer::create(1.f/60.f, [=]()
							{
								is_preloaded_effect = true;
								CCLOG("end preload effects !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
								
								if(is_loaded_cgp && is_loaded_server && is_preloaded_effect)
								{
									CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
									CCTextureCache::sharedTextureCache()->removeUnusedTextures();
									
									CCDelayTime* t_delay = CCDelayTime::create(2.f);
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
	
	if(myLog->getLogCount() > 0)
	{
		myLog->sendLog(CCString::createWithFormat("ting_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)))->getCString());
	}
	
	AudioEngine::sharedInstance()->playEffect("ment_title.mp3");
	addChild(KSTimer::create(2.8f, [=](){AudioEngine::sharedInstance()->playSound("bgm_ui.mp3", true);}));
	
	is_loaded_server = false;
	
	receive_cnt = 0;
	
	command_list.push_back(CommandParam("getcommonsetting", Json::Value(), json_selector(this, TitleRenewalScene::resultGetCommonSetting)));
	
	Json::Value achievelist_param;
	achievelist_param["version"] = NSDS_GI(kSDS_AI_version_i);
	command_list.push_back(CommandParam("getarchivementlist", achievelist_param, json_selector(this, TitleRenewalScene::resultGetAchieveList)));
	
	Json::Value shopdata_param;
	shopdata_param["version"] = NSDS_GI(kSDS_GI_shopVersion_i);
	command_list.push_back(CommandParam("getshoplist", shopdata_param, json_selector(this, TitleRenewalScene::resultGetShopList)));
	
	Json::Value puzzlelist_param;
	puzzlelist_param["version"] = NSDS_GI(kSDS_GI_puzzleListVersion_i);
	command_list.push_back(CommandParam("getpuzzlelist", puzzlelist_param, json_selector(this, TitleRenewalScene::resultGetPuzzleList)));
	
	Json::Value character_param;
	character_param["version"] = NSDS_GI(kSDS_GI_characterVersion_i);
	command_list.push_back(CommandParam("getcharacterlist", character_param, json_selector(this, TitleRenewalScene::resultGetCharacterInfo)));
	
	Json::Value monster_param;
	monster_param["version"] = NSDS_GI(kSDS_GI_monsterVersion_i);
	command_list.push_back(CommandParam("getmonsterlist", monster_param, json_selector(this, TitleRenewalScene::resultGetMonsterList)));
	
	command_list.push_back(CommandParam("getnoticelist", Json::Value(), json_selector(this, TitleRenewalScene::resultGetNoticeList)));
	
	command_list.push_back(CommandParam("gettimeevent", Json::Value(), json_selector(this, TitleRenewalScene::resultGetTimeEvent)));
	
	
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
	
	command_list.push_back(CommandParam("gettimeinfo", Json::Value(), json_selector(this, TitleRenewalScene::resultGetTimeInfo)));
	
	
	//		command_list.push_back(CommandParam("getpathinfo", Json::Value(), json_selector(this, TitleRenewalScene::resultGetPathInfo)));
	
	startCommand();
	
	is_loaded_cgp = false;
	
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
					title_img = CCSprite::create("temp_title_back2.png");
					title_img->setPosition(ccp(240,160));
					addChild(title_img);
					
					title_name->setPosition(ccp(240,160));
					
//					CCSprite* logo_img = CCSprite::create("temp_title_sumlanlogo.png");
//					logo_img->setPosition(ccp(475-logo_img->getContentSize().width/2.f, 315-logo_img->getContentSize().height/2.f));
//					addChild(logo_img, 1);
					
					CCSprite* progress_back = CCSprite::create("temp_title_loading_back.png");
					progress_back->setPosition(ccp(240,80));
					addChild(progress_back, 1);
					
					progress_timer = CCProgressTimer::create(CCSprite::create("temp_title_loading_center.png"));
					progress_timer->setType(kCCProgressTimerTypeBar);
					progress_timer->setMidpoint(ccp(0,0));
					progress_timer->setBarChangeRate(ccp(1,0));
					progress_timer->setPercentage(0);
					progress_timer->setPosition(ccp(240, 80));
					addChild(progress_timer, 1);
					
					CCSprite* progress_top = CCSprite::create("temp_title_loading_front.png");
					progress_top->setPosition(ccp(240,80));
					addChild(progress_top, 1);
					
					
					addChild(KSGradualValue<float>::create(1.f, 0.f, 0.8f, [=](float t){
						black_img->setOpacity(t*255);
					}, [=](float t){
						black_img->setOpacity(0);
						black_img->removeFromParent();
						
						tip_list.clear();
						
						for(int i=kMyLocalKey_titleLoadingBegin+1;i<kMyLocalKey_titleLoadingEnd;i++)
						{
							tip_list.push_back(myLoc->getLocalForKey((MyLocalKey)i));
						}
						
						random_shuffle(tip_list.begin(), tip_list.end());
						
						recent_tip_index = 0;
						
						state_label->setString(tip_list[recent_tip_index].c_str());
						
						addChild(KSTimer::create(4.f, [=](){changeTipMent();}));
						
						ing_download_cnt = 1;
						ing_download_per = 0;
						is_downloading = true;
						
						if(!download_state)
						{
							download_state = CCLabelBMFont::create("", "allfont.fnt");
							download_state->setPosition(ccp(240,80));
							addChild(download_state, 2);
						}
						
						startFileDownload();
					}));
				}));
			}
		}
		else if(is_receive_fail)
		{
			state_label->setString(myLoc->getLocalForKey(kMyLocalKey_failLoadInfo));
			
			CommonButton* replay_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, kCCMenuHandlerPriority);
			replay_menu->setPosition(ccp(240,160));
			replay_menu->setFunction([=](CCObject* sender)
									 {
										 CCNode* t_node = CCNode::create();
										 t_node->setTag(kTitleRenewal_MT_replay);
										 menuAction(t_node);
									 });
			
			addChild(replay_menu, 0, kTitleRenewal_MT_replay);
			
			is_menu_enable = true;
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
		
		mySGD->setEventString(result_data["eventString"].asString());
		mySGD->setIsAlwaysSavePlaydata(result_data["isAlwaysSavePlaydata"].asInt());
		mySGD->setPlayContinueFeeEndless(result_data["playContinueFeeEndless"].asInt());
	}
	else
	{
		is_receive_fail = true;
		command_list.push_back(CommandParam("getcommonsetting", Json::Value(), json_selector(this, TitleRenewalScene::resultGetCommonSetting)));
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
		
		graphdog->setCommandFinishedFunc([](){ mySGD->refreshTimeEvent(); });
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
			
			NSDS_SI(kSDS_GI_shopRuby_int1_count_i, i-1, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_countName_s, i-1, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopRuby_int1_price_i, i-1, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_priceType_s, i-1, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_priceName_s, i-1, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_sale_s, i-1, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_pID_s, i-1, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopRuby_int1_exchangeID_s, i-1, t_data["exchangeID"].asString(), false);
		}
		
		for(int i=1;i<=6;i++)
		{
			string t_key = CCString::createWithFormat("s_g_%d", i)->getCString();
			Json::Value t_data = result_data[t_key.c_str()];
			
			NSDS_SI(kSDS_GI_shopGold_int1_count_i, i-1, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_countName_s, i-1, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopGold_int1_price_i, i-1, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_priceType_s, i-1, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_priceName_s, i-1, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_sale_s, i-1, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopGold_int1_exchangeID_s, i-1, t_data["exchangeID"].asString(), false);
		}
		
		for(int i=1;i<=6;i++)
		{
			string t_key = CCString::createWithFormat("s_h_%d", i)->getCString();
			Json::Value t_data = result_data[t_key.c_str()];
			
			NSDS_SI(kSDS_GI_shopCoin_int1_count_i, i-1, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_countName_s, i-1, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopCoin_int1_price_i, i-1, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_priceType_s, i-1, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_priceName_s, i-1, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_sale_s, i-1, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopCoin_int1_exchangeID_s, i-1, t_data["exchangeID"].asString(), false);
		}
		
		for(int i=1;i<=6;i++)
		{
			string t_key = CCString::createWithFormat("es_r_%d", i)->getCString();
			Json::Value t_data = result_data[t_key.c_str()];
			
			NSDS_SI(kSDS_GI_shopEventRuby_int1_count_i, i-1, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopEventRuby_int1_countName_s, i-1, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopEventRuby_int1_price_i, i-1, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopEventRuby_int1_priceType_s, i-1, t_data["priceType"].asString(), false);
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
			NSDS_SI(kSDS_GI_shopItem_int1_price_i, t_code, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceType_s, t_code, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceName_s, t_code, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_sale_s, t_code, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_exchangeID_s, t_code, t_data["exchangeID"].asString(), false);
			
			
			t_code = kIC_doubleItem;
			t_key = CCString::createWithFormat("s_i_%d", t_code)->getCString();
			
			t_data.clear();
			t_data = result_data[t_key.c_str()];
			
			NSDS_SS(kSDS_GI_shopItem_int1_countName_s, t_code, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopItem_int1_price_i, t_code, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceType_s, t_code, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceName_s, t_code, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_sale_s, t_code, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_exchangeID_s, t_code, t_data["exchangeID"].asString(), false);
			
			
			t_code = kIC_magnet;
			t_key = CCString::createWithFormat("s_i_%d", t_code)->getCString();
			
			t_data.clear();
			t_data = result_data[t_key.c_str()];
			
			NSDS_SS(kSDS_GI_shopItem_int1_countName_s, t_code, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopItem_int1_price_i, t_code, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceType_s, t_code, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_priceName_s, t_code, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_sale_s, t_code, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopItem_int1_exchangeID_s, t_code, t_data["exchangeID"].asString(), false);
		}
		
		Json::FastWriter t_writer;
		int t_index = 0;
		{
			Json::Value t_data = result_data["pg_fp"];
			
			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_count_i, t_index, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_countName_s, t_index, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_price_i, t_index, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceType_s, t_index, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, t_index, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_sale_s, t_index, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_data_s, t_index, t_writer.write(t_data["data"]), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_pID_s, t_index, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_exchangeID_s, t_index, t_data["exchangeID"].asString(), false);
			
			t_index++;
		}
		{
			Json::Value t_data = result_data["pg_ei"];
			
			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_count_i, t_index, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_countName_s, t_index, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_price_i, t_index, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceType_s, t_index, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, t_index, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_sale_s, t_index, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_data_s, t_index, t_writer.write(t_data["data"]), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_pID_s, t_index, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_exchangeID_s, t_index, t_data["exchangeID"].asString(), false);
			
			t_index++;
		}
		{
			Json::Value t_data = result_data["pg_snh"];
			
			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_count_i, t_index, t_data["count"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_countName_s, t_index, t_data["countName"].asString(), false);
			NSDS_SI(kSDS_GI_shopPurchaseGuide_int1_price_i, t_index, t_data["price"].asInt(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceType_s, t_index, t_data["priceType"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, t_index, t_data["priceName"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_sale_s, t_index, t_data["sale"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_data_s, t_index, t_writer.write(t_data["data"]), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_pID_s, t_index, t_data["pID"].asString(), false);
			NSDS_SS(kSDS_GI_shopPurchaseGuide_int1_exchangeID_s, t_index, t_data["exchangeID"].asString(), false);
			
			t_index++;
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
		command_list.push_back(CommandParam("getnoticelist", Json::Value(), json_selector(this, TitleRenewalScene::resultGetCommonSetting)));
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
			NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_gold_d, i, character_list[i-1]["statInfo"]["gold"].asDouble(), false);
			NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_percent_d, i, character_list[i-1]["statInfo"]["percent"].asDouble(), false);
			NSDS_SI(kSDS_GI_characterInfo_int1_statInfo_feverTime_i, i, character_list[i-1]["statInfo"]["feverTime"].asInt(), false);
			NSDS_SD(kSDS_GI_characterInfo_int1_statInfo_speed_d, i, character_list[i-1]["statInfo"]["speed"].asDouble(), false);
			NSDS_SI(kSDS_GI_characterInfo_int1_statInfo_life_i, i, character_list[i-1]["statInfo"]["life"].asInt(), false);
			NSDS_SI(kSDS_GI_characterInfo_int1_statInfo_lineColor_i, i, character_list[i-1]["statInfo"]["color"].asInt(), false);
			NSDS_SI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i, character_list[i-1]["statInfo"]["slot"].asInt(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, i, character_list[i-1]["resourceInfo"]["ccbiID"].asString(), false);
			
			if(NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbi_s, i) != character_list[i-1]["resourceInfo"]["ccbi"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = 100;
				t_df.img = character_list[i-1]["resourceInfo"]["ccbi"].asString().c_str();
				t_df.filename = character_list[i-1]["resourceInfo"]["ccbiID"].asString() + ".ccbi";
				t_df.key = CCSTR_CWF("ci%d_ri_ccbi", i)->getCString();
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
				character_download_list.push_back(t_df);
				// ================================
			}
			
			NSDS_SI(kSDS_GI_characterInfo_int1_resourceInfo_size_i, i, character_list[i-1]["resourceInfo"]["size"].asInt(), false);
			NSDS_SS(kSDS_GI_characterInfo_int1_comment_s, i, character_list[i-1]["comment"].asString(), false);
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
					t_df1.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString() + "_head.ccbi";
					t_df1.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + "_head.ccbi";
					t_df1.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df1);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df1);
					// ================================
					
					// check, after download ----------
					DownloadFile t_df2;
					t_df2.size = 100;
					t_df2.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString() + "_body.ccbi";
					t_df2.filename = monster_list[i-1]["resourceInfo"]["ccbiID"].asString() + "_body.ccbi";
					t_df2.key = CCSTR_CWF("mi%d_ri_ccbi", i)->getCString();
					
					iter = find(monster_download_list.begin(), monster_download_list.end(), t_df2);
					if(iter == monster_download_list.end())
						monster_download_list.push_back(t_df2);
					// ================================
					
					// check, after download ----------
					DownloadFile t_df3;
					t_df3.size = 100;
					t_df3.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString() + "_tail.ccbi";
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
					t_df.img = monster_list[i-1]["resourceInfo"]["ccbi"].asString() + ".ccbi";
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
		myDSH->loadAllUserData(result_data);
		
		if(myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep) != kTutorialFlowStep_puzzleClick)
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_end);
		
		mySGD->initUserdata(result_data);
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
		card_data_load_list.clear();
		
		mySGD->initTakeCardInfo(result_data["list"], card_data_load_list);
		
		if(card_data_load_list.size() > 0)
		{
			Json::Value card_param;
			for(int i=0;i<card_data_load_list.size();i++)
				card_param["noList"][i] = card_data_load_list[i];
			command_list.push_back(CommandParam("getcardlist", card_param, json_selector(this, TitleRenewalScene::resultLoadedCardData)));
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
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
			Json::Value t_card_missile = t_card["missile"];
			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
			
			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
			
			Json::Value t_ability = t_card["ability"];
			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
			for(int j=0;j<t_ability.size();j++)
			{
				Json::Value t_abil = t_ability[j];
				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
				
				Json::Value t_option;
				if(!t_abil["option"].isObject())                    t_option["key"]="value";
				else												t_option =t_abil["option"];
				
				if(t_abil["type"].asInt() == kIC_attack)
					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_addTime)
					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_fast)
					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_silence)
					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_doubleItem)
					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_longTime)
					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
				else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
					NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
			}
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
			bool is_add_cf = false;
			
			if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = t_imgInfo["size"].asInt();
				t_df.img = t_imgInfo["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
				card_download_list.push_back(t_df);
				// ================================
				
				CopyFile t_cf;
				t_cf.from_filename = t_df.filename.c_str();
				t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
				card_reduction_list.push_back(t_cf);
				
				is_add_cf = true;
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
					card_download_list.push_back(t_df);
					// ================================
				}
				
				if(is_add_cf)
				{
					CopyFile t_cf = card_reduction_list.back();
					card_reduction_list.pop_back();
					t_cf.is_ani = true;
					t_cf.cut_width = t_detail["cutWidth"].asInt();
					t_cf.cut_height = t_detail["cutHeight"].asInt();
					t_cf.position_x = t_detail["positionX"].asInt();
					t_cf.position_y = t_detail["positionY"].asInt();
					t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
					card_reduction_list.push_back(t_cf);
				}
			}
			
			NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
			NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
			NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
			
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
					card_download_list.push_back(t_df);
					// ================================
				}
			}
		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		mySGD->resetHasGottenCards();
	}
	else
	{
		is_receive_fail = true;
		Json::Value card_param;
		for(int i=0;i<card_data_load_list.size();i++)
			card_param["noList"][i] = card_data_load_list[i];
		command_list.push_back(CommandParam("getcardlist", card_param, json_selector(this, TitleRenewalScene::resultLoadedCardData)));
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
				Json::Value reward_info = puzzle_list[i]["rewardInfo"];
				Json::Value level_info = puzzle_list[i]["levelInfo"];
				Json::Value condition_info = puzzle_list[i]["conditionInfo"];
				int start_stage = puzzle_list[i]["startStage"].asInt();
				for(int j=0;j<path_info.size();j++)
				{
					NSDS_SI(start_stage+j, kSDS_SI_puzzle_i, puzzle_number, false);
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, start_stage + j, path_info[j].asInt(), false);
					NSDS_SI(start_stage+j, kSDS_SI_level_i, level_info[j].asInt(), false);
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, start_stage+j, condition_info[j]["gold"].asInt(), false);
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, start_stage+j, condition_info[j]["no"].asInt(), false);
					Json::Value t_stage_cards = card_info[j];
					for(int k=0;k<t_stage_cards.size();k++)
					{
						NSDS_SI(start_stage+j, kSDS_SI_level_int1_card_i, k+1, t_stage_cards[k].asInt(), false);
						NSDS_SI(kSDS_CI_int1_reward_i, t_stage_cards[k].asInt(), reward_info[j][k].asInt(), false);
					}
					mySDS->fFlush(start_stage+j, kSDS_SI_base);
				}
				mySDS->fFlush(kSDS_CI_base);

				NSDS_SI(puzzle_number, kSDS_PZ_startWarp_x_d, puzzle_list[i]["startPosition"]["x"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_startWarp_y_d, puzzle_list[i]["startPosition"]["y"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_lastWarp_x_d, puzzle_list[i]["endPosition"]["x"].asInt(), false);
				NSDS_SI(puzzle_number, kSDS_PZ_lastWarp_y_d, puzzle_list[i]["endPosition"]["y"].asInt(), false);
				
				Json::Value coordinateInfo_list = puzzle_list[i]["coordinateInfo"];
				for(int j=0;j<coordinateInfo_list.size();j++)
				{
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_x_d, start_stage+j, coordinateInfo_list[j]["x"].asInt(), false);
					NSDS_SI(puzzle_number, kSDS_PZ_stage_int1_y_d, start_stage+j, coordinateInfo_list[j]["y"].asInt(), false);
				}
				
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
	if(is_loaded_cgp && is_loaded_server && is_preloaded_effect)
	{
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	
	
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
	mySGD->is_safety_mode = myDSH->getBoolForKey(kDSH_Key_isSafetyMode);
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_init);
	CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
//	CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
//	CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
//	CCDirector::sharedDirector()->replaceScene(PlayTutorial::scene());
}

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
		
		schedule(schedule_selector(TitleRenewalScene::downloadingFileAction));
	}
	else if(puzzle_download_list.size() > 0 && ing_download_cnt <= character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		CCLOG("%d : %s", ing_download_cnt, puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img,
													  puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].size,
													  puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].filename,
													  this, callfunc_selector(TitleRenewalScene::successDownloadAction), this, callfunc_selector(TitleRenewalScene::failDownloadAction));
		
		schedule(schedule_selector(TitleRenewalScene::downloadingFileAction));
	}
	else
	{
		endingCheck();
	}
}

void TitleRenewalScene::successDownloadAction()
{
	unschedule(schedule_selector(TitleRenewalScene::downloadingFileAction));
	if(ing_download_cnt < character_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, character_download_list[ing_download_cnt-1].key, character_download_list[ing_download_cnt-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, character_download_list[ing_download_cnt-1].key, character_download_list[ing_download_cnt-1].img, false);
		NSDS_SI(kSDS_GI_characterVersion_i, character_download_version, false);
		mySDS->fFlush(kSDS_GI_characterCount_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, monster_download_list[ing_download_cnt-character_download_list.size()-1].key, monster_download_list[ing_download_cnt-character_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, monster_download_list[ing_download_cnt-character_download_list.size()-1].key, monster_download_list[ing_download_cnt-character_download_list.size()-1].img, false);
		NSDS_SI(kSDS_GI_monsterVersion_i, monster_download_version, false);
		mySDS->fFlush(kSDS_GI_monsterCount_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size() + card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].key,
			   card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size() + card_download_list.size())
	{
		SDS_SS(kSDF_cardInfo, card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].key,
			   card_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-1].img, false);
		for(int i=0;i<card_reduction_list.size();i++)
		{
			CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].from_filename.c_str()));
			target_img->setAnchorPoint(ccp(0,0));
			
			if(card_reduction_list[i].is_ani)
			{
				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(card_reduction_list[i].ani_filename.c_str()),
																CCRectMake(0, 0, card_reduction_list[i].cut_width, card_reduction_list[i].cut_height));
				ani_img->setPosition(ccp(card_reduction_list[i].position_x, card_reduction_list[i].position_y));
				target_img->addChild(ani_img);
			}
			
			target_img->setScale(0.2f);
			
			CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
			t_texture->setSprite(target_img);
			t_texture->begin();
			t_texture->getSprite()->visit();
			t_texture->end();
			
			t_texture->saveToFile(card_reduction_list[i].to_filename.c_str(), kCCImageFormatPNG);
		}
		
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt < character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].key,
			   puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
		startFileDownload();
	}
	else if(ing_download_cnt == character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size())
	{
		SDS_SS(kSDF_gameInfo, puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].key,
			   puzzle_download_list[ing_download_cnt-character_download_list.size()-monster_download_list.size()-card_download_list.size()-1].img, false);
		ing_download_cnt++;
		ing_download_per = 0.f;
		
		if(ing_download_cnt <= int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))
		download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
		
		
		for(int j=0;j<puzzle_download_list.size() && j < puzzle_download_list_puzzle_number.size();j++)
		{
			if(puzzle_download_list[j].key == "map")
				continue;
			
			CCImage *img = new CCImage;
			img->initWithImageFileThreadSafe((mySIL->getDocumentPath() + puzzle_download_list[j].filename).c_str()); //퍼즐이미지를 불러옵니다.
			
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
		
		
		endingCheck();
	}
	
	float rate = 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size());
	if(rate > 100.f)
		rate = 100.f;
	else if(rate < 0.f)
		rate = 0.f;
	
	CCProgressFromTo* t_to = CCProgressFromTo::create(0.5f, progress_timer->getPercentage(), rate);
	progress_timer->runAction(t_to);
}

void TitleRenewalScene::endingCheck()
{
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
	
	download_state->setString(CCSTR_CWF("%.0f%%", 100.f*ing_download_cnt/int(character_download_list.size() + monster_download_list.size() + card_download_list.size() + puzzle_download_list.size()))->getCString());
}

void TitleRenewalScene::failDownloadAction()
{
	state_label->setString(myLoc->getLocalForKey(kMyLocalKey_downImgFail));
	
	CommonButton* replay_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, kCCMenuHandlerPriority);
	replay_menu->setPosition(ccp(240,160));
	replay_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(kTitleRenewal_MT_redown);
								 menuAction(t_node);
							 });
	
	addChild(replay_menu, 0, kTitleRenewal_MT_redown);
	
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
		state_label->setString(myLoc->getLocalForKey(kMyLocalKey_downImgInfo));
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
			is_menu_enable = true;
		}
	}
	else if(tag >= kTitleRenewal_MT_puzzleBase)
	{
		tag -= kTitleRenewal_MT_puzzleBase;
		
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, tag);
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
										 //state_label->setString(myLoc->getLocalForKey(kMyLocalKey_successLogin));
										 myDSH->setStringForKey(kDSH_Key_nick, input_text->getText());
										 setTouchEnabled(false);
										 nick_back->removeFromParent();
										 removeChildByTag(kTitleRenewal_MT_nick);
										 //input_text->removeFromParent();
										 //flag->removeFromParent();
										 myDSH->saveUserData({kSaveUserData_Key_nick}, nullptr);
										 
										 successLogin();
									 }
									 else if(result_data["result"]["code"].asInt() == GDDUPLICATEDNICK)
									 {
										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(kMyLocalKey_nicknameError),
																												 myLoc->getLocalForKey(kMyLocalKey_sameNick), nullptr, CCPointZero, true), 999);
										 
										 is_menu_enable = true;
									 }
									 else if(result_data["result"]["code"].asInt() == GDFAULTYNICK)
									 {
										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(kMyLocalKey_nicknameError),
																												 myLoc->getLocalForKey(kMyLocalKey_invalidNick), nullptr, CCPointZero, true), 999);

										 is_menu_enable = true;
									 }
									 else if(result_data["result"]["code"].asInt() == GDALREADYMEMBER)
									 {
										 //state_label->setString(myLoc->getLocalForKey(kMyLocalKey_successLogin));
										 myDSH->setStringForKey(kDSH_Key_nick, input_text->getText());
										 setTouchEnabled(false);
										 nick_back->removeFromParent();
										 removeChildByTag(kTitleRenewal_MT_nick);
										 //input_text->removeFromParent();
										 //flag->removeFromParent();
										 
										 myDSH->saveUserData({kSaveUserData_Key_nick}, nullptr);
										 
										 successLogin();
									 }
									 else if(result_data["result"]["code"].asInt() == GDLONGNAME)
									 {
										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(kMyLocalKey_nicknameError),
																												 myLoc->getLocalForKey(kMyLocalKey_longNick), nullptr, CCPointZero, true), 999);
										 
//										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(kMyLocalKey_longNick)), 999);
										 is_menu_enable = true;
									 }
									 else if(result_data["result"]["code"].asInt() == GDSHORTNAME)
									 {
										 addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(kMyLocalKey_nicknameError),
															myLoc->getLocalForKey(kMyLocalKey_shortNick), nullptr, CCPointZero, true), 999);
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
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(TitleRenewalScene::alertAction));
}
