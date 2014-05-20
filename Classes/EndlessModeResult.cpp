//
//  EndlessModeResult.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 20..
//
//

#include "EndlessModeResult.h"
#include "DataStorageHub.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "TouchSuctionLayer.h"
#include "CCMenuLambda.h"
#include "FlagSelector.h"
#include "FormSetter.h"
#include "ScrollBar.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "DownloadFile.h"
#include "StageImgLoader.h"
#include "StartSettingPopup.h"
#include "MainFlowScene.h"

enum EndlessModeResultZorder
{
	kEndlessModeResultZorder_gray = 0,
	kEndlessModeResultZorder_back,
	kEndlessModeResultZorder_content
};

bool EndlessModeResult::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	
	addChild(KSTimer::create(0.1f, [](){FormSetter::get()->start();}));
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	is_menu_enable = false;
	
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
	addChild(gray, kEndlessModeResultZorder_gray);
	
	gray->runAction(CCFadeTo::create(0.5f, 255));
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	suction->setTouchEnabled(true);
	addChild(suction);
	
	if(mySGD->endless_my_victory_on.getV())
	{
		mySGD->endless_my_victory_on = false;
		
		if(mySGD->getScore() > mySGD->endless_score.getV())
		{
			mySGD->endless_my_victory = mySGD->endless_my_victory.getV() + 1;
		}
		
		mySGD->endless_my_total_score = mySGD->endless_my_total_score.getV() + mySGD->getScore();
	}
	
	setMain();
	
	is_menu_enable = true;
	
	return true;
}

void EndlessModeResult::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

void EndlessModeResult::setMain()
{
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-22.f));
	addChild(main_case, kEndlessModeResultZorder_back);
	
//	main_case->setScaleY(0.f);
//	
//	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
//		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.8f);
//			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.f);}));}));}));
//	
//	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t)
//										 {
//											 KS::setOpacity(main_case, 255);
//											 
//											 is_menu_enable = true;
//										 }));
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	left_back->setContentSize(CCSizeMake((main_case->getContentSize().width-30)/2.f, 212));
	left_back->setPosition(ccp(10+left_back->getContentSize().width/2.f, 165));
	main_case->addChild(left_back);
	
	CCScale9Sprite* left_star_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_star_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 60));
	left_star_back->setPosition(ccp(left_back->getContentSize().width/2.f, 154));
	left_back->addChild(left_star_back);
	
	int star_count = 4;
	for(int i=0;i<star_count;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_gold.png");
		t_star->setPosition(ccp(30+i*48,left_star_back->getContentSize().height/2.f));
		left_star_back->addChild(t_star);
	}
	
	CCSprite* left_title = CCSprite::create("endless_nickname.png");
	left_title->setPosition(ccp(left_back->getContentSize().width/2.f, 193.5f));
	left_back->addChild(left_title);
	
	string my_flag = myDSH->getStringForKey(kDSH_Key_flag);
	CCSprite* left_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(my_flag).c_str());
	left_flag->setPosition(ccp(25,26));
	left_title->addChild(left_flag);
	
	KSLabelTTF* left_nick = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 13);
	left_nick->enableOuterStroke(ccBLACK, 1.f);
	left_nick->setPosition(ccp(97, 24));
	left_title->addChild(left_nick);
	
	CCScale9Sprite* left_score_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_score_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_score_back->setPosition(ccp(left_back->getContentSize().width/2.f, 111.f));
	left_back->addChild(left_score_back);
	
	KSLabelTTF* left_score_title = KSLabelTTF::create("점수", mySGD->getFont().c_str(), 13);
	left_score_title->setAnchorPoint(ccp(0,0.5f));
	left_score_title->setPosition(ccp(10, left_score_back->getContentSize().height/2.f));
	left_score_back->addChild(left_score_title);
	
	KSLabelTTF* left_score_content = KSLabelTTF::create("9999", mySGD->getFont().c_str(), 13);
	left_score_content->setAnchorPoint(ccp(1,0.5f));
	left_score_content->setPosition(ccp(left_score_back->getContentSize().width-10, left_score_back->getContentSize().height/2.f));
	left_score_back->addChild(left_score_content);
	
	
	CCScale9Sprite* left_time_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_time_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_time_back->setPosition(ccp(left_back->getContentSize().width/2.f, 81.f));
	left_back->addChild(left_time_back);
	
	KSLabelTTF* left_time_title = KSLabelTTF::create("타임", mySGD->getFont().c_str(), 13);
	left_time_title->setAnchorPoint(ccp(0,0.5f));
	left_time_title->setPosition(ccp(10, left_time_back->getContentSize().height/2.f));
	left_time_back->addChild(left_time_title);
	
	KSLabelTTF* left_time_content = KSLabelTTF::create("9999", mySGD->getFont().c_str(), 13);
	left_time_content->setAnchorPoint(ccp(1,0.5f));
	left_time_content->setPosition(ccp(left_time_back->getContentSize().width-10, left_time_back->getContentSize().height/2.f));
	left_time_back->addChild(left_time_content);
	
	
	CCScale9Sprite* left_gold_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_gold_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_gold_back->setPosition(ccp(left_back->getContentSize().width/2.f, 51.f));
	left_back->addChild(left_gold_back);
	
	KSLabelTTF* left_gold_title = KSLabelTTF::create("골드", mySGD->getFont().c_str(), 13);
	left_gold_title->setAnchorPoint(ccp(0,0.5f));
	left_gold_title->setPosition(ccp(10, left_gold_back->getContentSize().height/2.f));
	left_gold_back->addChild(left_gold_title);
	
	KSLabelTTF* left_gold_content = KSLabelTTF::create("9999", mySGD->getFont().c_str(), 13);
	left_gold_content->setAnchorPoint(ccp(1,0.5f));
	left_gold_content->setPosition(ccp(left_gold_back->getContentSize().width-10, left_gold_back->getContentSize().height/2.f));
	left_gold_back->addChild(left_gold_content);
	
	
	CCScale9Sprite* left_total_score_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_total_score_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_total_score_back->setPosition(ccp(left_back->getContentSize().width/2.f, 21));
	left_back->addChild(left_total_score_back);
	
	KSLabelTTF* left_total_score_title = KSLabelTTF::create("총점", mySGD->getFont().c_str(), 15);
	left_total_score_title->setColor(ccc3(255, 170, 20));
	left_total_score_title->setAnchorPoint(ccp(0,0.5f));
	left_total_score_title->setPosition(ccp(10, left_total_score_back->getContentSize().height/2.f));
	left_total_score_back->addChild(left_total_score_title);
	
	KSLabelTTF* left_total_score_content = KSLabelTTF::create("9999", mySGD->getFont().c_str(), 15);
	left_total_score_content->setColor(ccc3(255, 170, 20));
	left_total_score_content->setAnchorPoint(ccp(1,0.5f));
	left_total_score_content->setPosition(ccp(left_total_score_back->getContentSize().width-10, left_total_score_back->getContentSize().height/2.f));
	left_total_score_back->addChild(left_total_score_content);
	
	
	CCSprite* n_stop = CCSprite::create("endless_ready.png");
	KSLabelTTF* n_stop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ready), mySGD->getFont().c_str(), 22);
	n_stop_label->setColor(ccc3(50, 30, 5));
	n_stop_label->setPosition(ccp(n_stop->getContentSize().width/2.f, n_stop->getContentSize().height/2.f-1));
	n_stop->addChild(n_stop_label);
	
	CCSprite* s_stop = CCSprite::create("endless_ready.png");
	s_stop->setColor(ccGRAY);
	KSLabelTTF* s_stop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ready), mySGD->getFont().c_str(), 22);
	s_stop_label->setColor(ccc3(50, 30, 5));
	s_stop_label->setPosition(ccp(s_stop->getContentSize().width/2.f, s_stop->getContentSize().height/2.f-1));
	s_stop->addChild(s_stop_label);
	
	CCMenuItemLambda* stop_item = CCMenuItemSpriteLambda::create(n_stop, s_stop, [=](CCObject* sender)
																  {
																	  if(!is_menu_enable)
																		  return;
																	  
																	  is_menu_enable = false;
																	  
																	  ready_loading = LoadingLayer::create(-999);
																	  addChild(ready_loading, 999);
																	  
																	  vector<CommandParam> command_list;
																	  command_list.clear();
																	  
																	  Json::Value param;
																	  param.clear();
																	  param["memberID"] = myHSP->getMemberID();
																	  param["score"] = mySGD->endless_my_total_score.getV();
																	  param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
																	  param["level"] = mySGD->endless_my_level.getV();
																	  param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
																	  param["victory"] = mySGD->endless_my_victory.getV();
																	  
																	  command_list.push_back(CommandParam("setendlessrank", param, [=](Json::Value result_data)
																										  {
																											  if(result_data["result"]["code"].asInt() != GDSUCCESS)
																												{
																													addChild(KSTimer::create(0.1f, [=](){reSetEndlessRank();}));
																												}
																											  else
																											  {
																												  ready_loading->removeFromParent();
																												  ready_loading = NULL;
																												  
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
																											  }
																										  }));
																	  
																	  Json::Value param2;
																	  param2.clear();
																	  param2["memberID"] = myHSP->getMemberID();
																	  param2["score"] = mySGD->getScore();
																	  param2["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
																	  param2["level"] = mySGD->endless_my_level.getV();
																	  param2["autoLevel"] = 1;
																	  param2["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
																	  param2["victory"] = mySGD->endless_my_victory.getV();
																	  Json::FastWriter writer;
																	  param2["playData"] = writer.write(mySGD->replay_write_info);
																	  
																	  command_list.push_back(CommandParam("saveendlessplaydata", param2, nullptr));
																	  
																	  myHSP->command(command_list);
																  });
	stop_item->setPosition(ccp(-main_case->getContentSize().width/2.f+left_back->getPositionX(), 0));
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_back->setContentSize(CCSizeMake((main_case->getContentSize().width-30)/2.f, 180));
	right_back->setPosition(ccp(main_case->getContentSize().width-10-right_back->getContentSize().width/2.f, 149));
	main_case->addChild(right_back);
	
	
	
	
	CCSprite* n_next = CCSprite::create("endless_ready.png");
	KSLabelTTF* n_next_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ready), mySGD->getFont().c_str(), 22);
	n_next_label->setColor(ccc3(50, 30, 5));
	n_next_label->setPosition(ccp(n_next->getContentSize().width/2.f, n_next->getContentSize().height/2.f-1));
	n_next->addChild(n_next_label);
	
	CCSprite* s_next = CCSprite::create("endless_ready.png");
	s_next->setColor(ccGRAY);
	KSLabelTTF* s_next_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ready), mySGD->getFont().c_str(), 22);
	s_next_label->setColor(ccc3(50, 30, 5));
	s_next_label->setPosition(ccp(s_next->getContentSize().width/2.f, s_next->getContentSize().height/2.f-1));
	s_next->addChild(s_next_label);
	
	CCMenuItemLambda* next_item = CCMenuItemSpriteLambda::create(n_next, s_next, [=](CCObject* sender)
																 {
																	 if(!is_menu_enable)
																		 return;
																	 
																	 is_menu_enable = false;
																	 
																	 ready_loading = LoadingLayer::create(-999);
																	 addChild(ready_loading, 999);

																	 Json::Value param;
																	 param["memberID"] = myHSP->getMemberID();
																	 myHSP->command("getendlessplaydata", param, json_selector(this, EndlessModeResult::resultGetEndlessPlayData));
																 });
	next_item->setPosition(ccp(-main_case->getContentSize().width/2.f+right_back->getPositionX(), 0));
	
	CCMenuLambda* bottom_menu = CCMenuLambda::create();
	bottom_menu->setPosition(ccp(main_case->getContentSize().width/2.f,35));
	bottom_menu->setTouchPriority(touch_priority);
	main_case->addChild(bottom_menu);
	
	bottom_menu->addChild(stop_item);
	bottom_menu->addChild(next_item);
	
	if(mySGD->getScore() <= mySGD->endless_score.getV())
	{
		next_item->setEnabled(false);
	}
	else
	{
		
	}
	
	CCSprite* vs_img = CCSprite::create("endless_vs.png");
	vs_img->setPosition(ccp(main_case->getContentSize().width/2.f, 210));
	main_case->addChild(vs_img);
}

void EndlessModeResult::reSetEndlessRank()
{
	Json::Value param;
	param.clear();
	param["memberID"] = myHSP->getMemberID();
	param["score"] = mySGD->endless_my_total_score.getV();
	param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	param["level"] = mySGD->endless_my_level.getV();
	param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	param["victory"] = mySGD->endless_my_victory.getV();
	
	myHSP->command("setendlessrank", param, [=](Json::Value result_data)
										{
											if(result_data["result"]["code"].asInt() != GDSUCCESS)
											{
												addChild(KSTimer::create(0.1f, [=](){reSetEndlessRank();}));
											}
											else
											{
												ready_loading->removeFromParent();
												ready_loading = NULL;
												
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
											}
										});
}

void EndlessModeResult::resultGetEndlessPlayData(Json::Value result_data)
{
	GraphDogLib::JsonToLog("getendlessplaydata : %s", result_data);
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->resetReplayPlayingInfo();
		
		if(result_data["rivar"]["playData"].isNull())
		{
			Json::Value read_data;
			Json::Reader reader;
			reader.parse(result_data["rivar"]["playData"].asString(), read_data);
			mySGD->setReplayPlayingInfo(read_data);
		}
		
		mySGD->endless_memberID = result_data["rivar"]["memberID"].asInt();
		mySGD->endless_nick = result_data["rivar"]["nick"].asString();
		mySGD->endless_flag = result_data["rivar"]["flag"].asString();
		mySGD->endless_victory = result_data["rivar"]["victory"].asInt();
		mySGD->endless_autoLevel = result_data["rivar"]["autoLevel"].asInt();
		mySGD->endless_level = result_data["rivar"]["level"].asInt();
		mySGD->endless_score = result_data["rivar"]["score"].asInt();
		mySGD->endless_regDate = result_data["rivar"]["regDate"].asInt64();
		
		saveStageInfo(result_data["stageInfo"]);
	}
	else
	{
		ready_loading->removeFromParent();
		ready_loading = NULL;
		
		addChild(ASPopupView::getCommonNoti(-999, "뭔가 문제가 발생하였습니다.\n다시 시도해주세요."), 999);
	}
}

void EndlessModeResult::saveStageInfo(Json::Value result_data)
{
	df_list.clear();
	cf_list.clear();
	
	stage_number = result_data["no"].asInt();
	
	NSDS_SI(stage_number, kSDS_SI_puzzle_i, result_data["puzzle"].asInt(), false);
	NSDS_SI(stage_number, kSDS_SI_playtime_i, result_data["playtime"].asInt(), false);
	NSDS_SD(stage_number, kSDS_SI_scoreRate_d, result_data["scoreRate"].asDouble(), false);
	NSDS_SD(stage_number, kSDS_SI_scale_d, result_data["scale"].asDouble(), false);
	NSDS_SB(stage_number, kSDS_SI_minigame_b, result_data["minigame"].asBool(), false);
	
	Json::Value t_mission = result_data["mission"];
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
	
	
	Json::Value shopItems = result_data["shopItems"];
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
	
	Json::Value defItems = result_data["defItems"];
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
	
	Json::Value cards = result_data["cards"];
	NSDS_SI(stage_number, kSDS_SI_cardCount_i, cards.size(), false);
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
			df_list.push_back(t_df);
			// ================================
			
			CopyFile t_cf;
			t_cf.from_filename = t_df.filename.c_str();
			t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
			cf_list.push_back(t_cf);
			
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
				df_list.push_back(t_df);
				// ================================
			}
			
			if(is_add_cf)
			{
				CopyFile t_cf = cf_list.back();
				cf_list.pop_back();
				t_cf.is_ani = true;
				t_cf.cut_width = t_detail["cutWidth"].asInt();
				t_cf.cut_height = t_detail["cutHeight"].asInt();
				t_cf.position_x = t_detail["positionX"].asInt();
				t_cf.position_y = t_detail["positionY"].asInt();
				
				t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
				
				cf_list.push_back(t_cf);
			}
		}
		
		NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
		NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
		NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
		
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
				df_list.push_back(t_df);
				// ================================
			}
		}
	}
	
	NSDS_SI(stage_number, kSDS_SI_level_i, result_data["level"].asInt(), false);
	
	NSDS_SS(stage_number, kSDS_SI_boss_s, result_data["boss"].asString(), false);
	NSDS_SS(stage_number, kSDS_SI_junior_s, result_data["junior"].asString(), false);
	NSDS_SI(stage_number, kSDS_SI_autoBalanceTry_i, result_data["autoBalanceTry"].asInt(), false);
	
	if(df_list.size() > 0) // need download
	{
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		download_version = result_data["version"].asInt();
		ing_download_cnt = 1;
		ing_download_per = 0;
		//		download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString(), "allfont.fnt");
		//		download_state->setPosition(ccp(240,130));
		//		addChild(download_state, kSID_Z_content);
		startDownload();
	}
	else
	{
		SDS_SI(kSDF_stageInfo, stage_number, "version", result_data["version"].asInt(), false);
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		successGetStageInfo();
	}
}

void EndlessModeResult::startDownload()
{
	CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	mySIL->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(EndlessModeResult::successAction), this, callfunc_selector(EndlessModeResult::failAction));
	
	//	schedule(schedule_selector(StageInfoDown::downloadingAction));
}

void EndlessModeResult::successAction()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
	
	if(ing_download_cnt >= df_list.size())
	{
		for(int i=0;i<cf_list.size();i++)
		{
			CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
			target_img->setAnchorPoint(ccp(0,0));
			
			if(cf_list[i].is_ani)
			{
				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
				target_img->addChild(ani_img);
			}
			
			target_img->setScale(0.2f);
			
			CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
			t_texture->setSprite(target_img);
			t_texture->begin();
			t_texture->getSprite()->visit();
			t_texture->end();
			
			t_texture->saveToFile(cf_list[i].to_filename.c_str(), kCCImageFormatPNG);
		}
		
		NSDS_SI(stage_number, kSDS_SI_version_i, download_version, false);
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		//		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		
		successGetStageInfo();
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
		//		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		startDownload();
	}
}

void EndlessModeResult::failAction()
{
	addChild(ASPopupView::getCommonNoti(-1000, "스테이지 이미지를 다운로드 하는데 실패하였습니다.\n다시 시도합니다.", [=]()
										{
											startDownload();
										}));
}

void EndlessModeResult::successGetStageInfo()
{
	ready_loading->removeFromParent();
	ready_loading = NULL;
	
	addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
										   {
											   gray->setOpacity(255*t);
										   }, [=](float t)
										   {
											   gray->setOpacity(0);
											   mySGD->is_endless_mode = true;
											   mySD->setSilType(stage_number);
											   
											   StartSettingPopup* t_popup = StartSettingPopup::create();
											   t_popup->setHideFinalAction(getParent(), callfunc_selector(MainFlowScene::showEndlessResult));
											   getParent()->addChild(t_popup, kMainFlowZorder_popup);
											   removeFromParent();
										   }));
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t){KS::setOpacity(main_case, 0);}));
}
