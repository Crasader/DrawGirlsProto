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
#include "LoadingTipScene.h"
#include "MyLocalization.h"
#include "RivalSelectPopup.h"
#include "AchieveNoti.h"
#include "CommonAnimation.h"
#include "EndlessSeqWinRewardPopup.h"
#include "StyledLabelTTF.h"
#include "TypingBox.h"
#include "FiveRocksCpp.h"

enum EndlessModeResultZorder
{
	kEndlessModeResultZorder_gray = 0,
	kEndlessModeResultZorder_back,
	kEndlessModeResultZorder_content
};

enum EndlessModeResultMenuTag{
	kMT_EMR_stop = 1,
	kMT_EMR_next
};

bool EndlessModeResult::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	
	left_table = NULL;
	right_table = NULL;
	
	refresh_achieve_func = nullptr;
	
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
	
	float play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
	
	if(mySD->getClearCondition() == kCLEAR_timeLimit)
		play_limit_time -= mySD->getClearConditionTimeLimit();
	
	if(mySGD->endless_my_victory_on.getV())
	{
		mySGD->temp_endless_play_limit_time = play_limit_time;
		mySGD->temp_replay_data = mySGD->replay_playing_info;
		mySGD->temp_endless_nick = mySGD->endless_nick.getV();
		mySGD->temp_endless_flag = mySGD->endless_flag.getV();
		mySGD->temp_endless_score = mySGD->endless_score.getV();
	}
	else
	{
//		is_calc = true;
		is_calc = false;
	}
	
	if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
		mySGD->area_score = mySGD->area_score.getV() + 100;
	
	left_life_base_score = mySGD->area_score.getV() + mySGD->damage_score.getV() + mySGD->combo_score.getV();
	left_life_decrease_score = (mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_lifeBonusCnt)].asInt()*30000*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d));
	left_time_base_score = left_life_base_score.getV() + left_life_decrease_score.getV();
	left_time_decrease_score = (mySGD->temp_endless_play_limit_time.getV()-mySGD->getGameTime())*500*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);
	left_grade_base_score = left_time_base_score.getV() + left_time_decrease_score.getV();
	if(mySGD->getStageGrade() <= 0)
		left_grade_decrease_score = left_grade_base_score.getV()*0.f;
	else
		left_grade_decrease_score = left_grade_base_score.getV()*(mySGD->getStageGrade()-1);
	left_damaged_score = -mySGD->damaged_score.getV();
	
	left_total_score = left_grade_base_score.getV() + left_grade_decrease_score.getV() + left_damaged_score.getV();
	
	
	right_area_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_areaScore), Json::Value()).asInt();
	right_damage_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_damageScore), Json::Value()).asInt();
	right_combo_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_comboScore), Json::Value()).asInt();
	
	right_life_cnt = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_lifeBonusCnt), Json::Value()).asInt();
	right_game_time = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_gameTime), Json::Value()).asInt();
	right_clear_grade = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_clearGrade), Json::Value()).asInt();
	
	
	right_life_base_score = right_area_score.getV() + right_damage_score.getV() + right_combo_score.getV();
	right_life_decrease_score = (right_life_cnt.getV()*30000*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d));
	right_time_base_score = right_life_base_score.getV() + right_life_decrease_score.getV();
	right_time_decrease_score = (mySGD->temp_endless_play_limit_time.getV()-right_game_time.getV())*500*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);
	right_grade_base_score = right_time_base_score.getV() + right_time_decrease_score.getV();
	if(right_clear_grade.getV() <= 0)
		right_grade_decrease_score = right_grade_base_score.getV()*0.f;
	else
		right_grade_decrease_score = right_grade_base_score.getV()*(right_clear_grade.getV()-1);
	right_damaged_score = -mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_scoreAttackedValue), Json::Value()).asInt();
	
	right_total_score = right_grade_base_score.getV() + right_grade_decrease_score.getV() + right_damaged_score.getV();
	
	if(mySGD->endless_my_victory_on.getV())
	{
		is_calc = true;
		mySGD->endless_my_victory_on = false;
		if(left_total_score.getV() > right_total_score.getV())
		{
			mySGD->endless_my_victory = mySGD->endless_my_victory.getV() + 1;
			mySGD->setUserdataEndlessIngWin(mySGD->endless_my_victory.getV());
			mySGD->endless_my_ing_win = mySGD->endless_my_ing_win.getV() + 1;
			mySGD->endless_my_ing_score = mySGD->endless_my_ing_score.getV() + int(left_total_score.getV());
		}
		else
		{
			fiverocks::FiveRocksBridge::trackEvent("Game", "PvPResult", ccsf("win %03d", mySGD->endless_my_victory.getV()), ccsf("Lv%03d", mySGD->getSelectedCharacterHistory().level.getV()));
			
			if(mySGD->pvp_continue_cnt >= 1)
			{
				string fiverocks_param1, fiverocks_param2;
				if(mySGD->pvp_continue_cnt == 1)
					fiverocks_param1 = "continue 01";
				else if(mySGD->pvp_continue_cnt <= 4)
					fiverocks_param1 = "continue 02~04";
				else if(mySGD->pvp_continue_cnt <= 8)
					fiverocks_param1 = "continue 05~08";
				else if(mySGD->pvp_continue_cnt <= 11)
					fiverocks_param1 = "continue 09~11";
				else
					fiverocks_param1 = "continue 12~";
				
				if(mySGD->endless_my_victory.getV() == 0)
					fiverocks_param2 = "win 00";
				else if(mySGD->endless_my_victory.getV() == 1)
					fiverocks_param2 = "win 01";
				else if(mySGD->endless_my_victory.getV() <= 4)
					fiverocks_param2 = "win 02~04";
				else if(mySGD->endless_my_victory.getV() <= 8)
					fiverocks_param2 = "win 05~08";
				else if(mySGD->endless_my_victory.getV() <= 11)
					fiverocks_param2 = "win 09~11";
				else
					fiverocks_param2 = "win 12~";
				
				fiverocks::FiveRocksBridge::trackEvent("UseGold", "PvPContinue", fiverocks_param1.c_str(), fiverocks_param2.c_str());
			}
			mySGD->endless_my_victory = 0;
			mySGD->setUserdataEndlessIngWin(mySGD->endless_my_victory.getV());
			mySGD->endless_my_ing_win = 0;
			mySGD->endless_my_ing_score = 0;
		}
		
//		mySGD->endless_my_total_score = mySGD->endless_my_total_score.getV() + mySGD->getScore();
	}
	
	for(int i=kAchievementCode_scoreHigh1;i<=kAchievementCode_scoreHigh3;i++)
	{
		if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
		   left_total_score.getV() >= myAchieve->getCondition((AchievementCode)i))
		{
			myAchieve->changeIngCount(AchievementCode(i), myAchieve->getCondition((AchievementCode)i));
			AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
	}
	
	mySGD->setUserdataAchieveHunter(mySGD->getUserdataAchieveHunter()+mySGD->hunt_value.getV());
	
	if(mySGD->is_exchanged)
		mySGD->setUserdataAchieveChangeMania(mySGD->getUserdataAchieveChangeMania() + 1);
	
	if(mySGD->is_showtime)
	{
		for(int i=kAchievementCode_perfect1;i<=kAchievementCode_perfect3;i++)
		{
			if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
			   mySGD->getUserdataAchievePerfect() + 1 >= myAchieve->getCondition((AchievementCode)i))
			{
				myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		mySGD->setUserdataAchievePerfect(mySGD->getUserdataAchievePerfect() + 1);
	}
	
	title_list.clear();
	left_content_list.clear();
	right_content_list.clear();
	
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleAreaScore));
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleDamageScore));
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleComboScore));
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleLifeScore));
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTimeScore));
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleGradeScore));
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleDamagedScore));
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTakeGold));
	title_list.push_back(myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTakeArea));
	
	left_content_list.push_back(KS::insert_separator(mySGD->area_score.getV()));
	left_content_list.push_back(KS::insert_separator(mySGD->damage_score.getV()));
	left_content_list.push_back(KS::insert_separator(mySGD->combo_score.getV()));
	left_content_list.push_back(KS::insert_separator(left_life_decrease_score.getV(), "%.0f"));
	left_content_list.push_back(KS::insert_separator(left_time_decrease_score.getV(), "%.0f"));
	left_content_list.push_back(KS::insert_separator(left_grade_decrease_score.getV(), "%.0f"));
	left_content_list.push_back(KS::insert_separator(left_damaged_score.getV()));
	left_content_list.push_back(KS::insert_separator(mySGD->getStageGold()));
	left_content_list.push_back(KS::insert_separator(int(mySGD->getPercentage()*100.f)) + "%");
	
	right_content_list.push_back(KS::insert_separator(right_area_score.getV()));
	right_content_list.push_back(KS::insert_separator(right_damage_score.getV()));
	right_content_list.push_back(KS::insert_separator(right_combo_score.getV()));
	right_content_list.push_back(KS::insert_separator(right_life_decrease_score.getV(), "%.0f"));
	right_content_list.push_back(KS::insert_separator(right_time_decrease_score.getV(), "%.0f"));
	right_content_list.push_back(KS::insert_separator(right_grade_decrease_score.getV(), "%.0f"));
	right_content_list.push_back(KS::insert_separator(right_damaged_score.getV()));
	right_content_list.push_back(KS::insert_separator(mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_takeGold), Json::Value()).asInt()));
	right_content_list.push_back(KS::insert_separator(int(mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_takeArea), Json::Value()).asFloat()*100.f)) + "%");
	
	setMain();
	
	ready_loading = LoadingLayer::create(-9999);
	addChild(ready_loading, 9999);
	
	send_command_list.clear();
	
	vector<CommandParam> t_achieve = myAchieve->updateAchieveHistoryVectorParam(nullptr);
	for(int i=0;i<t_achieve.size();i++)
	{
		send_command_list.push_back(t_achieve[i]);
	}
	
	if(is_calc)
	{
		Json::Value param;
		param.clear();
		param["memberID"] = myHSP->getMemberID();
		param["score"] = left_total_score.getV();// int(mySGD->getScore());
		param["victory"] = left_total_score.getV() > right_total_score.getV();
		param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
		param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
		Json::Value p1_data;
		p1_data["character"] = mySGD->getUserdataSelectedCharNO();
		p1_data["highPiece"] = mySGD->getUserdataHighPiece();
		param["data"] = p1_data;
		
		send_command_list.push_back(CommandParam("finishendlessplay", param, [=](Json::Value result_data)
												 {
													 TRACE();
													 GraphDogLib::JsonToLog("finishendlessplay", result_data);
													 TRACE();
													 if(result_data["result"]["code"].asInt() == GDSUCCESS)
													 {
														 TRACE();
														 if(result_data["sendGift"].asBool())
														 {
															 is_menu_enable = false;
															 EndlessSeqWinRewardPopup* t_popup = EndlessSeqWinRewardPopup::create(-999, [=](){is_menu_enable = true;}, result_data);
															 addChild(t_popup, 999);
														 }
														 TRACE();
													 }
												 }));
	}
	
	bool is_send_pvp_play_data = false;
	
	if(mySGD->getIsAlwaysSavePlaydata() == 1)
		is_send_pvp_play_data = true;
	else if(left_total_score.getV() > right_total_score.getV() && rand()%100 > mySGD->getSendPvpPlayDataRate())
		is_send_pvp_play_data = true;
	
	if(is_send_pvp_play_data)
	{
		Json::Value param2;
		param2.clear();
		param2["memberID"] = myHSP->getMemberID();
		param2["score"] = left_total_score.getV();//mySGD->getScore();
		param2["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
		param2["level"] = mySGD->endless_my_level.getV();
		param2["autoLevel"] = 1;
		param2["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
		param2["victory"] = mySGD->endless_my_victory.getV();
		
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_clearGrade)] = mySGD->getStageGrade();
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_gameTime)] = mySGD->getGameTime();
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_takeGold)] = mySGD->getStageGold();
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_totalScore)] = mySGD->getScore();
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_originalScore)] = mySGD->getBaseScore();
		
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_win)] = mySGD->endless_my_win.getV() + mySGD->endless_my_victory.getV();
		mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_lose)] = mySGD->endless_my_lose.getV() + 1;
		
		
		Json::FastWriter writer;
		param2["playData"] = writer.write(mySGD->replay_write_info);
		param2["pieceNo"] = mySGD->temp_replay_data[mySGD->getReplayKey(kReplayKey_stageNo)].asInt();
		
		send_command_list.push_back(CommandParam("saveendlessplaydata", param2, nullptr));
	}
	
	if(mySGD->is_changed_userdata)
		send_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	return true;
}

void EndlessModeResult::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
	tryTransaction();
}

void EndlessModeResult::tryTransaction()
{
	TRACE();
	mySGD->changeGoodsTransaction(send_command_list, [=](Json::Value result_data)
								  {
									  TRACE();
									  if(result_data["result"]["code"].asInt() == GDSUCCESS)
									  {
										  TRACE();
										  mySGD->network_check_cnt = 0;
										  
										  ready_loading->removeFromParent();
										  ready_loading = NULL;
										  
										  if(refresh_achieve_func != nullptr)
										  {
											  addChild(KSTimer::create(0.1f, refresh_achieve_func));
										  }
									  }
									  else
									  {
										  TRACE();
										  mySGD->network_check_cnt++;
										  
										  if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
										  {
											  mySGD->network_check_cnt = 0;
											  
											  ASPopupView* alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
												  tryTransaction();
											  }, 1);
											  
//											  ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
//												  tryTransaction();
//											  });
											  if(alert)
												  ((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
										  }
										  else
										  {
											  addChild(KSTimer::create(0.5f, [=]()
																	   {
																		   tryTransaction();
																	   }));
										  }
									  }
									  TRACE();
								  });
}

void EndlessModeResult::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

CCTableViewCell* EndlessModeResult::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSize cell_size = cellSizeForTable(table);
	
	string back_filename;
	string title = title_list[idx];
	string content;
	
	CCRect back_rect1, back_rect2;
	if(idx < 9)
	{
		back_filename = "common_lightgray.png";
		back_rect1 = CCRectMake(0, 0, 18, 18);
		back_rect2 = CCRectMake(8, 8, 2, 2);
	}
	else
	{
		back_filename = "common_deepgray.png";
		back_rect1 = CCRectMake(0, 0, 20, 20);
		back_rect2 = CCRectMake(9, 9, 2, 2);
	}
	
	if(table == left_table)
	{
		content = left_content_list[idx];
	}
	else if(table == right_table)
	{
		content = right_content_list[idx];
	}
	
	CCScale9Sprite* t_back = CCScale9Sprite::create(back_filename.c_str(), back_rect1, back_rect2);
	t_back->setContentSize(CCSizeMake(193, 19));
	t_back->setPosition(ccp(cell_size.width/2.f, cell_size.height/2.f));
	cell->addChild(t_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), 12);
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(8, t_back->getContentSize().height/2.f));
	t_back->addChild(title_label);
	
	if(idx == 7)
	{
		if(mySGD->isTimeEvent(kTimeEventType_gold))
		{
			KSLabelTTF* gold_event = KSLabelTTF::create(CCString::createWithFormat("X%.1f", mySGD->getTimeEventFloatValue(kTimeEventType_gold))->getCString(), mySGD->getFont().c_str(), 14);
			gold_event->setColor(ccc3(255, 190, 50));
			gold_event->enableOuterStroke(ccBLACK, 1.f);
			gold_event->setAnchorPoint(ccp(0,0.5));
			gold_event->setPosition(ccp(title_label->getContentSize().width+3,title_label->getContentSize().height/2.f));
			title_label->addChild(gold_event);
			
			KSLabelTTF* event_label = KSLabelTTF::create("EVENT", mySGD->getFont().c_str(), 10);
			
			CCScale9Sprite* event_back = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
			event_back->setContentSize(CCSizeMake(event_label->getContentSize().width+18, 20));
			event_back->setPosition(ccp(gold_event->getContentSize().width+4+event_back->getContentSize().width/2.f, gold_event->getContentSize().height/2.f));
			gold_event->addChild(event_back);
			
			event_label->setPosition(ccpFromSize(event_back->getContentSize()/2.f));
			event_back->addChild(event_label);
		}
	}
	
	KSLabelTTF* content_label = KSLabelTTF::create(content.c_str(), mySGD->getFont().c_str(), 13);
	content_label->setAnchorPoint(ccp(1,0.5f));
	content_label->setPosition(ccp(t_back->getContentSize().width-8, t_back->getContentSize().height/2.f));
	t_back->addChild(content_label);
	
	return cell;
}

unsigned int EndlessModeResult::numberOfCellsInTableView(CCTableView *table)
{
	return 9;
}

void EndlessModeResult::controlButtonAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int tag = ((CCNode*)sender)->getTag();
	
    CCLOG("clicked button : %d", tag);
    
	if(tag == kMT_EMR_stop)
	{
		if(left_total_score.getV() > right_total_score.getV())
		{
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
			
			CCSprite* case_back = CCSprite::create("popup_small_back.png");
			case_back->setPosition(CCPointZero);
			t_container->addChild(case_back);
			
			CCScale9Sprite* content_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
			content_back->setContentSize(CCSizeMake(251, 113));
			content_back->setPosition(ccp(0.0,-15.5)); 			// dt (0.0,-4.5)
			t_container->addChild(content_back);
			
			KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessKeepWinTitle), mySGD->getFont().c_str(), 12);
			title_label->disableOuterStroke();
			title_label->setPosition(ccp(-85,case_back->getContentSize().height/2.f-35));
			t_container->addChild(title_label);
			
			KSLabelTTF* ment_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessKeepWinContent), mySGD->getFont().c_str(), 12);
			ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
			content_back->addChild(ment_label);
			
			CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
			close_button->setPosition(ccp(case_back->getContentSize().width/2.f - 25, case_back->getContentSize().height/2.f - 22));
			close_button->setFunction([=](CCObject* sender)
									  {
										  if(!t_popup->is_menu_enable)
											  return;
										  
										  t_popup->is_menu_enable = false;
										  
										  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
										  
										  CommonAnimation::closePopup(t_popup, t_container, t_popup->getDimmedSprite(), [=](){
											  
										  }, [=](){
											  is_menu_enable = true;
											  t_popup->removeFromParent();
										  });
									  });
			t_container->addChild(close_button);
			
			
			CommonButton* ok_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), 12, CCSizeMake(101, 44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), t_popup->getTouchPriority()-5);
			ok_button->setPosition(ccp(0,case_back->getContentSize().height/2.f*-1+45));
			ok_button->setFunction([=](CCObject* sender)
									  {
										  if(!t_popup->is_menu_enable)
											  return;
										  
										  t_popup->is_menu_enable = false;
										  
										  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
										  
										  CommonAnimation::closePopup(this, t_container, t_popup->getDimmedSprite(), [=](){
											  
										  }, [=](){
											  t_popup->getDimmedSprite()->setOpacity(0);
											  KS::setOpacity(t_container, 0);
											  
											  
											  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f,
																					 [=](float t)
																					 {
																						 gray->setOpacity(255*t);
																					 }, [=](float t)
																					 {
																						 gray->setOpacity(0);
																						 if(target_final && delegate_final)
																							 (target_final->*delegate_final)();
																						 removeFromParent();
																					 }));
											  
											  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f,
																					 [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
																						 addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f,
																																[=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
											  
											  addChild(KSGradualValue<int>::create(255, 0, 0.15f,
																				   [=](int t)
																				   {
																					   KS::setOpacity(main_case, t);
																				   }, [=](int t)
																				   {
																					   KS::setOpacity(main_case, 0);
																				   }));
											  
											  t_popup->removeFromParent();
										  });
									  });
			t_container->addChild(ok_button);
			
			
			content_back->setContentSize(content_back->getContentSize() + CCSizeMake(0, -50));
			content_back->setPositionY(content_back->getPositionY() + 25);

			
			ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
			
			t_popup->getDimmedSprite()->setOpacity(0);
			CommonAnimation::openPopup(t_popup, t_container, t_popup->getDimmedSprite(), nullptr, [=]()
									   {
										   t_popup->is_menu_enable = true;
									   });
			addChild(t_popup, 999);
		}
		else
		{
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
			
			CommonAnimation::closePopup(this, main_case, nullptr, [=](){
				
			}, [=](){
				//																				end_func(); removeFromParent();
			});
		}
	}
	else if(tag == kMT_EMR_next)
	{
		ready_loading = LoadingLayer::create(-999);
		addChild(ready_loading, 999);
		
		vector<CommandParam> command_list;
		command_list.clear();
		
		Json::Value param3;
		param3["memberID"] = myHSP->getMemberID();
		
		command_list.push_back(CommandParam("starttransaction", param3, nullptr));
		
		Json::Value param;
		param["memberID"] = myHSP->getMemberID();
		param["win"] = mySGD->getUserdataEndlessIngWin();//mySGD->getUserdataAutoLevel();
		param["highPiece"] = mySGD->getUserdataHighPiece();
		command_list.push_back(CommandParam("getendlessplayriver", param, json_selector(this, EndlessModeResult::resultGetEndlessPlayData)));
		
		TRACE();
		myHSP->command(command_list);
	}
}

void EndlessModeResult::setMain()
{
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kEndlessModeResultZorder_back);
	
	left_back = CCScale9Sprite::create("common_doubleblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	left_back->setContentSize(CCSizeMake(209, 177));
	left_back->setPosition(ccp(132, main_case->getContentSize().height*0.58f+3));
	main_case->addChild(left_back);
	
	CCSprite* me_back = CCSprite::create("endless_na.png");
	me_back->setPosition(ccp(20,left_back->getContentSize().height-18));
	left_back->addChild(me_back);
//	KSLabelTTF* me_label = KSLabelTTF::create("Me", mySGD->getFont().c_str(), 13);
//	me_label->disableOuterStroke();
//	me_label->setPosition(ccpFromSize(me_back->getContentSize()/2.f));
//	me_back->addChild(me_label);
	
	string left_result;
	if(mySGD->getStageGrade() <= 0)
		left_result = myLoc->getLocalForKey(LK::kMyLocalKey_failTitleGameover);
	else
		left_result = myLoc->getLocalForKey(LK::kMyLocalKey_clearTitle);
	
	KSLabelTTF* left_result_label = KSLabelTTF::create(left_result.c_str(), mySGD->getFont().c_str(), 21.5f);
	if(mySGD->getStageGrade() <= 0)
		left_result_label->setColor(ccc3(240, 45, 45));
	else
		left_result_label->setGradientColor(ccc4(255, 230, 10, 255), ccc4(255, 130, 7, 255), ccp(0,-1));
	left_result_label->setPosition(ccp(left_back->getContentSize().width/2.f, 154));
	left_back->addChild(left_result_label);

	
	CCScale9Sprite* left_star_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	left_star_back->setContentSize(CCSizeMake(193,50));
	left_star_back->setPosition(ccp(left_back->getContentSize().width/2.f, 119));
	left_back->addChild(left_star_back);
	
	for(int i=0;i<4;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_empty.png");
		t_star->setPosition(ccp(30+i*48,left_star_back->getContentSize().height/2.f));
		left_star_back->addChild(t_star);
	}
	
	int star_count = mySGD->getStageGrade();
	
	vector<function<void()>> left_star_animation_list;
	left_star_animation_list.clear();
	
	for(int i=0;i<star_count;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_gold.png");
		t_star->setPosition(ccp(30+i*48,left_star_back->getContentSize().height/2.f));
		left_star_back->addChild(t_star);
		
		if(is_calc)
		{
			t_star->setScale(2.f);
			t_star->setRotation(-200);
			t_star->setVisible(false);
			
			CCSprite* t_star2 = CCSprite::create("ending_star_gold.png");
			t_star2->setPosition(ccp(t_star->getContentSize().width/2.f, t_star->getContentSize().height/2.f));
			t_star2->setScale(2.f);
			t_star->addChild(t_star2);
			
			function<void()> t_animation;
			t_animation = [=]()
			{
				t_star->addChild(KSTimer::create(0.1f+i*0.2f, [=]()
												 {
													 AudioEngine::sharedInstance()->playEffect("se_resultstar.mp3", false);
													 t_star->setOpacity(0);
													 t_star2->setOpacity(0);
													 t_star->setVisible(true);
													 t_star->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																									{
																										t_star->setScale(2.f-t);
																										t_star->setRotation(-200+t*200);
																										t_star->setOpacity(t*255);
																									}, [=](float t)
																									{
																										t_star->setScale(1.f);
																										t_star->setRotation(0);
																										t_star->setOpacity(255);
																										t_star2->setOpacity(255);
																										addStarParticle(t_star);
																										
																										t_star->addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																																					   {
																																						   t_star2->setScale(2.f-t);
																																						   t_star2->setOpacity(255-t*55);
																																					   }, [=](float t)
																																					   {
																																						   t_star2->removeFromParent();
																																					   }));
																									}));
												 }));
			};
			
			left_star_animation_list.push_back(t_animation);
		}
		else
		{
			addStarParticle(t_star);
		}
	}
	
	CCSprite* left_title = CCSprite::create("endless_nickname.png");
	left_title->setPosition(ccp(left_back->getContentSize().width/2.f, 177.f));
	left_back->addChild(left_title);
	
	string my_flag = myDSH->getStringForKey(kDSH_Key_flag);
	CCSprite* left_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(my_flag).c_str());
	left_flag->setScale(0.8f);
	left_flag->setPosition(ccp(0,16));
	left_title->addChild(left_flag);
	
	KSLabelTTF* left_nick = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 13);
	left_nick->setColor(ccc3(20, 50, 70));
	left_nick->disableOuterStroke();
	left_nick->setPosition(ccp(0, 16));
	left_title->addChild(left_nick);
	
	float left_f_width = left_flag->getContentSize().width * left_flag->getScale();
	float left_n_width = left_nick->getContentSize().width * left_nick->getScale();
	
	left_flag->setPositionX(left_title->getContentSize().width/2.f - left_n_width/2.f-3);
	left_nick->setPositionX(left_title->getContentSize().width/2.f + left_f_width/2.f+3);
	
	
	
	CCRect left_rect = CCRectMake(left_back->getContentSize().width/2.f-((480-30)/2.f-20)/2.f, 51.f-30.f/2.f-9, (480-30)/2.f-20, 65);
	
//	CCSprite* left_size = CCSprite::create("whitePaper.png", CCRectMake(0, 0, left_rect.size.width, left_rect.size.height));
//	left_size->setAnchorPoint(CCPointZero);
//	left_size->setPosition(left_rect.origin);
//	left_back->addChild(left_size);
	
	left_table = CCTableView::create(this, left_rect.size);
	left_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	left_table->setPosition(left_rect.origin);
	left_back->addChild(left_table);
	left_table->setTouchPriority(touch_priority);
	left_table->setTouchEnabled(false);
	
	
	if(is_calc)
		left_table->setContentOffset(ccp(0,-9*21));
	else
		left_table->setContentOffset(ccp(0,0*21));
	
	
	right_back = CCScale9Sprite::create("common_doubleblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	right_back->setContentSize(CCSizeMake(209,177));
	right_back->setPosition(ccp(main_case->getContentSize().width-132, main_case->getContentSize().height*0.58f+3));
	main_case->addChild(right_back);
	
//	CCSprite* you_back = CCSprite::create("endless_you.png");
//	you_back->setPosition(ccp(right_back->getContentSize().width-20,right_back->getContentSize().height-18));
//	right_back->addChild(you_back);
//	KSLabelTTF* you_label = KSLabelTTF::create("You", mySGD->getFont().c_str(), 13);
//	you_label->disableOuterStroke();
//	you_label->setPosition(ccpFromSize(you_back->getContentSize()/2.f));
//	you_back->addChild(you_label);
	
	string right_result;
	if(right_clear_grade.getV() <= 0)
		right_result = myLoc->getLocalForKey(LK::kMyLocalKey_failTitleGameover);
	else
		right_result = myLoc->getLocalForKey(LK::kMyLocalKey_clearTitle);
	
	KSLabelTTF* right_result_label = KSLabelTTF::create(right_result.c_str(), mySGD->getFont().c_str(), 21.5f);
	if(right_clear_grade.getV() <= 0)
		right_result_label->setColor(ccc3(240, 45, 45));
	else
		right_result_label->setGradientColor(ccc4(255, 230, 10, 255), ccc4(255, 130, 7, 255), ccp(0,-1));
	right_result_label->setPosition(ccp(right_back->getContentSize().width/2.f, 154));
	right_back->addChild(right_result_label);
	
	CCScale9Sprite* right_star_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	right_star_back->setContentSize(CCSizeMake(193,50));
	right_star_back->setPosition(ccp(right_back->getContentSize().width/2.f, 119));
	right_back->addChild(right_star_back);
	
	for(int i=0;i<4;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_empty.png");
		t_star->setPosition(ccp(30+i*48,right_star_back->getContentSize().height/2.f));
		right_star_back->addChild(t_star);
	}
	
	int right_star_count = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_clearGrade), Json::Value()).asInt(); // 상대방 별 갯수
	
	vector<function<void()>> right_star_animation_list;
	right_star_animation_list.clear();
	
	for(int i=0;i<right_star_count;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_gold.png");
		t_star->setPosition(ccp(30+i*48,right_star_back->getContentSize().height/2.f));
		right_star_back->addChild(t_star);
		
		if(is_calc)
		{
			t_star->setScale(2.f);
			t_star->setRotation(-200);
			t_star->setVisible(false);
			
			CCSprite* t_star2 = CCSprite::create("ending_star_gold.png");
			t_star2->setPosition(ccp(t_star->getContentSize().width/2.f, t_star->getContentSize().height/2.f));
			t_star2->setScale(2.f);
			t_star->addChild(t_star2);
			
			function<void()> t_animation;
			t_animation = [=]()
			{
				t_star->addChild(KSTimer::create(0.1f+i*0.2f, [=]()
												 {
													 if(i >= star_count)
														 AudioEngine::sharedInstance()->playEffect("se_resultstar.mp3", false);
													 t_star->setOpacity(0);
													 t_star2->setOpacity(0);
													 t_star->setVisible(true);
													 t_star->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																									{
																										t_star->setScale(2.f-t);
																										t_star->setRotation(-200+t*200);
																										t_star->setOpacity(t*255);
																									}, [=](float t)
																									{
																										t_star->setScale(1.f);
																										t_star->setRotation(0);
																										t_star->setOpacity(255);
																										t_star2->setOpacity(255);
																										addStarParticle(t_star);
																										
																										t_star->addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																																					   {
																																						   t_star2->setScale(2.f-t);
																																						   t_star2->setOpacity(255-t*55);
																																					   }, [=](float t)
																																					   {
																																						   t_star2->removeFromParent();
																																					   }));
																									}));
												 }));
			};
			
			right_star_animation_list.push_back(t_animation);
		}
		else
		{
			addStarParticle(t_star);
		}
	}
	
	CCSprite* right_title = CCSprite::create("endless_nickname.png");
	right_title->setPosition(ccp(right_back->getContentSize().width/2.f, 177.f));
	right_back->addChild(right_title);
	
	string t_right_flag = mySGD->temp_endless_flag.getV();
	CCSprite* right_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(t_right_flag).c_str());
	right_flag->setScale(0.9f);
	right_flag->setPosition(ccp(0,16));
	right_title->addChild(right_flag);
	
	KSLabelTTF* right_nick = KSLabelTTF::create(mySGD->temp_endless_nick.getV().c_str(), mySGD->getFont().c_str(), 13);
	right_nick->setColor(ccc3(20, 50, 70));
	right_nick->disableOuterStroke();
	right_nick->setPosition(ccp(0, 16));
	right_title->addChild(right_nick);
	
	float right_f_width = right_flag->getContentSize().width * right_flag->getScale();
	float right_n_width = right_nick->getContentSize().width * right_nick->getScale();
	
	right_flag->setPositionX(right_title->getContentSize().width/2.f - right_n_width/2.f-3);
	right_nick->setPositionX(right_title->getContentSize().width/2.f + right_f_width/2.f+3);
	
	
	CCRect right_rect = CCRectMake(right_back->getContentSize().width/2.f-((480-30)/2.f-20)/2.f, 51.f-30.f/2.f-9, (480-30)/2.f-20, 65);
	
//	CCSprite* right_size = CCSprite::create("whitePaper.png", CCRectMake(0, 0, right_rect.size.width, right_rect.size.height));
//	right_size->setAnchorPoint(CCPointZero);
//	right_size->setPosition(right_rect.origin);
//	right_back->addChild(right_size);
	
	right_table = CCTableView::create(this, right_rect.size);
	right_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	right_table->setPosition(right_rect.origin);
	right_back->addChild(right_table);
	right_table->setTouchPriority(touch_priority);
	right_table->setTouchEnabled(false);
	
	
	if(is_calc)
		right_table->setContentOffset(ccp(0,-9*21));
	else
		right_table->setContentOffset(ccp(0,0*21));
	
	
		
	CCScale9Sprite* left_total_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	left_total_back->setContentSize(CCSizeMake(193, 20));
	left_total_back->setPosition(ccp(left_back->getContentSize().width/2.f, 15));
	left_back->addChild(left_total_back);
	
	KSLabelTTF* left_total_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_totalScore), mySGD->getFont().c_str(), 13);
	left_total_title->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
//	left_total_title->setColor(ccc3(255, 170, 20));
	left_total_title->setAnchorPoint(ccp(0,0.5f));
	left_total_title->setPosition(ccp(8, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_title);
	
	string start_total_left_content;
	if(is_calc)
		start_total_left_content = "0";
	else
		start_total_left_content = KS::insert_separator(left_total_score.getV());
	
	left_total_content = KSLabelTTF::create(start_total_left_content.c_str(), mySGD->getFont().c_str(), 15);
	left_total_content->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	left_total_content->setAnchorPoint(ccp(1,0.5f));
	left_total_content->setPosition(ccp(left_total_back->getContentSize().width-8, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_content);
	
	
	string stop_string;
	if(left_total_score.getV() <= right_total_score.getV())
		stop_string = myLoc->getLocalForKey(LK::kMyLocalKey_endlessKeepWin);
	else
		stop_string = myLoc->getLocalForKey(LK::kMyLocalKey_endlessKeepWinTitle);
	
	CCLabelTTF* t_stop_node = CCLabelTTF::create();
	KSLabelTTF* stop_label = KSLabelTTF::create(stop_string.c_str(), mySGD->getFont().c_str(), 27.5f);
	stop_label->disableOuterStroke();
	t_stop_node->addChild(stop_label);
	
	stop_button = CCControlButton::create(t_stop_node, CCScale9Sprite::create("mainbutton_green.png", CCRectMake(0, 0, 215, 65), CCRectMake(107, 32, 1, 1)));
	stop_button->setPreferredSize(CCSizeMake(215, 65));
	stop_button->setTag(kMT_EMR_stop);
	stop_button->addTargetWithActionForControlEvents(this, cccontrol_selector(EndlessModeResult::controlButtonAction), CCControlEventTouchUpInside);
	stop_button->setPosition(ccp(left_back->getPositionX(), 45));
	stop_button->setTouchPriority(touch_priority-2);
	main_case->addChild(stop_button);
	
		
	CCScale9Sprite* right_total_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	right_total_back->setContentSize(CCSizeMake(193, 20));
	right_total_back->setPosition(ccp(right_back->getContentSize().width/2.f, 15));
	right_back->addChild(right_total_back);
	
	KSLabelTTF* right_total_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_totalScore), mySGD->getFont().c_str(), 13);
	right_total_title->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
//	right_total_title->setColor(ccc3(255, 170, 20));
	right_total_title->setAnchorPoint(ccp(0,0.5f));
	right_total_title->setPosition(ccp(8, right_total_back->getContentSize().height/2.f));
	right_total_back->addChild(right_total_title);
	
	string start_total_right_content;
	if(is_calc)
		start_total_right_content = "0";
	else
		start_total_right_content = KS::insert_separator(right_total_score.getV());
	
	right_total_content = KSLabelTTF::create(start_total_right_content.c_str(), mySGD->getFont().c_str(), 15);
	right_total_content->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	right_total_content->setAnchorPoint(ccp(1,0.5f));
	right_total_content->setPosition(ccp(right_total_back->getContentSize().width-8, right_total_back->getContentSize().height/2.f));
	right_total_back->addChild(right_total_content);
	
	
	string next_button_str;
	if(mySGD->endless_my_victory.getV() > 0)
		next_button_str = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_endlessReadyIngWin), mySGD->endless_my_victory.getV()+1)->getCString();
	else
		next_button_str = myLoc->getLocalForKey(LK::kMyLocalKey_nextStage);
	
	CCLabelTTF* t_next_node = CCLabelTTF::create();
	KSLabelTTF* next_label = KSLabelTTF::create(next_button_str.c_str(), mySGD->getFont().c_str(), 27.5f);
	next_label->disableOuterStroke();
	t_next_node->addChild(next_label);
	
	next_button = CCControlButton::create(t_next_node, CCScale9Sprite::create("mainbutton_purple.png", CCRectMake(0, 0, 215, 65), CCRectMake(107, 32, 1, 1)));
	next_button->setPreferredSize(CCSizeMake(215, 65));
	next_button->setTag(kMT_EMR_next);
	next_button->addTargetWithActionForControlEvents(this, cccontrol_selector(EndlessModeResult::controlButtonAction), CCControlEventTouchUpInside);
	next_button->setPosition(ccp(right_back->getPositionX(), 45));
	next_button->setTouchPriority(touch_priority-2);
	main_case->addChild(next_button);
	
	if(left_total_score.getV() <= right_total_score.getV())
	{
		next_button->setVisible(false);
		stop_button->setPosition(ccp(main_case->getContentSize().width/2.f, 45));
	}
	
	CCSprite* vs_img = CCSprite::create("endless_vs.png");
	vs_img->setPosition(ccp(main_case->getContentSize().width/2.f, 230));
	main_case->addChild(vs_img);
	
	if(is_calc)
	{
		next_button->setVisible(false);
		stop_button->setVisible(false);
	}
	else
	{
		if(left_total_score.getV() > right_total_score.getV())
		{
			// win
			
			result_stamp = CCSprite::create("endless_winner.png");
			result_stamp->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
			main_case->addChild(result_stamp);
			
			CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
			win_label->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f+10));
			result_stamp->addChild(win_label);
			
			CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode().c_str())->getCString());
			win_ment->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f-25));
			result_stamp->addChild(win_ment);
			
			result_stamp->setRotation(-15);
			
			
			CCParticleSystemQuad* particle2 = CCParticleSystemQuad::createWithTotalParticles(10);
			particle2->setPositionType(kCCPositionTypeRelative);
			particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
			particle2->setEmissionRate(80);
			particle2->setAngle(360.0);
			particle2->setAngleVar(0.0);
			ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
			particle2->setBlendFunc(blendFunc);
			particle2->setDuration(-1.0);
			particle2->setEmitterMode(kCCParticleModeGravity);
			particle2->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
			particle2->setStartColorVar(ccc4f(0,0,0,0.f));
			particle2->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
			particle2->setEndColorVar(ccc4f(0, 0, 0, 0.f));
			particle2->setStartSize(25.0);
			particle2->setStartSizeVar(10.0);
			particle2->setEndSize(0.0);
			particle2->setEndSizeVar(0.0);
			particle2->setGravity(ccp(0,0));
			particle2->setRadialAccel(3.0);
			particle2->setRadialAccelVar(0.0);
			particle2->setSpeed(0);
			particle2->setSpeedVar(0.0);
			particle2->setTangentialAccel(0);
			particle2->setTangentialAccelVar(0);
			particle2->setTotalParticles(10);
			particle2->setLife(0.8);
			particle2->setLifeVar(0.25);
			particle2->setStartSpin(0.0);
			particle2->setStartSpinVar(50.f);
			particle2->setEndSpin(0.0);
			particle2->setEndSpinVar(60.f);
			particle2->setPosVar(ccp(80,80));
			particle2->setPosition(result_stamp->getPosition());
			main_case->addChild(particle2);
		}
		else
		{
			// lose
			
			result_stamp = CCSprite::create("endless_loser.png");
			result_stamp->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
			main_case->addChild(result_stamp);
		}
		
		TouchSuctionLayer* t_suction_layer = TouchSuctionLayer::create(touch_priority-1);
		t_suction_layer->touch_began_func = [=]()
		{
			result_stamp->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																 {
																	 KS::setOpacity(result_stamp, 255-t*155);
																 }, [=](float t)
																 {
																	 KS::setOpacity(result_stamp, 100);
																 }));
			t_suction_layer->removeFromParent();
		};
		t_suction_layer->is_on_touch_began_func = true;
		addChild(t_suction_layer);
		t_suction_layer->setTouchEnabled(true);
	}
	
	main_case->setScaleY(0.f);
	
	if(is_calc)
	{
		CCNode* curtain_node = LoadingTipScene::getOpenCurtainNode();
		curtain_node->setPosition(ccp(240,160));
		addChild(curtain_node, 999);
	}
	
	addChild(KSTimer::create(0.6f, [=]()
	{
		
		CommonAnimation::openPopup(this, main_case, gray, [=](){
			
		}, [=](){
			if(is_calc)
			{
				for(int i=0;i<left_star_animation_list.size();i++)
				{
					left_star_animation_list[i]();
				}
				
				for(int i=0;i<right_star_animation_list.size();i++)
				{
					right_star_animation_list[i]();
				}
				
				addChild(KSTimer::create(1.3f, [=](){startCalcAnimation();}));
			}
			else
			{
				is_menu_enable = true;
			}
		});
		
		
	}));
}

void EndlessModeResult::startCalcAnimation()
{
	function<void(float, float, float, float, float, function<void()>)> t_func1 = [=](float before_y, float left_decrease_value, float left_base_value, float right_decrease_value, float right_base_value, function<void()> end_func)
	{
		if(this->is_left_calc_end && this->is_right_calc_end)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
			
			addChild(KSGradualValue<float>::create(0.f, 1.f, 0.15f, [=](float t)
												   {
													   left_table->setContentOffset(ccp(0, before_y+21*t));
													   right_table->setContentOffset(ccp(0, before_y+21*t));
												   }, [=](float t)
												   {
													   left_table->setContentOffset(ccp(0, before_y+21));
													   right_table->setContentOffset(ccp(0, before_y+21));
													   
													   this->is_left_calc_end = this->is_right_calc_end = false;
													   AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
													   startLeftCalcAnimation(left_decrease_value, left_base_value, 0.3f, NULL, [=]()
																			  {
																				  this->is_left_calc_end = true;
																				  end_func();
																			  });
													   startRightCalcAnimation(right_decrease_value, right_base_value, 0.3f, NULL, [=]()
																			   {
																				   this->is_right_calc_end = true;
																				   end_func();
																			   });
													   
													   
												   }));
		}
	};
	
	is_left_calc_end = is_right_calc_end = true;
	
	t_func1(-9*21, mySGD->area_score.getV(), 0, right_area_score.getV(), 0, [=]()
	{
		t_func1(-8*21, mySGD->damage_score.getV(), mySGD->area_score.getV(), right_damage_score.getV(), right_area_score.getV(), [=]()
		{
			t_func1(-7*21, mySGD->combo_score.getV(), mySGD->area_score.getV()+mySGD->damage_score.getV(), right_combo_score.getV(), right_area_score.getV()+right_damage_score.getV(), [=]()
			{
				t_func1(-6*21, left_life_decrease_score.getV(), left_life_base_score.getV(), right_life_decrease_score.getV(), right_life_base_score.getV(), [=]()
				{
					t_func1(-5*21, left_time_decrease_score.getV(), left_time_base_score.getV(), right_time_decrease_score.getV(), right_time_base_score.getV(), [=]()
					{
						t_func1(-4*21, left_grade_decrease_score.getV(), left_grade_base_score.getV(), right_grade_decrease_score.getV(), right_grade_base_score.getV(), [=]()
						{
							t_func1(-3*21, left_damaged_score.getV(), left_grade_base_score.getV()+left_grade_decrease_score.getV(), right_damaged_score.getV(), right_grade_base_score.getV()+right_grade_decrease_score.getV(), [=]()
							{
								if(this->is_left_calc_end && this->is_right_calc_end)
								{
									AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
									
									addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t)
																		   {
																			   left_table->setContentOffset(ccp(0, -2*21+42*t));
																			   right_table->setContentOffset(ccp(0, -2*21+42*t));
																		   }, [=](float t)
																		   {
																			   left_table->setContentOffset(ccp(0, -2*21+42));
																			   right_table->setContentOffset(ccp(0, -2*21+42));
																			   
																			   left_table->setTouchEnabled(true);
																			   right_table->setTouchEnabled(true);
																			   
																			   left_table->CCScrollView::setDelegate(this);
																			   right_table->CCScrollView::setDelegate(this);
																			   
																			   if(left_total_score.getV() > right_total_score.getV())
																			   {
																				   // win
																				   
																				   result_stamp = CCSprite::create("endless_winner.png");
																				   result_stamp->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
																				   main_case->addChild(result_stamp);
																				   
																				   CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
																				   win_label->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f+10));
																				   result_stamp->addChild(win_label);
																				   
																				   CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode().c_str())->getCString());
																				   win_ment->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f-25));
																				   result_stamp->addChild(win_ment);
																				   
																				   result_stamp->setRotation(-15);
																				   
																				   
																				   KS::setOpacity(result_stamp, 0);
																				   result_stamp->setScale(2.5f);
																				   
																				   CCParticleSystemQuad* particle1 = CCParticleSystemQuad::createWithTotalParticles(100);
																				   particle1->setPositionType(kCCPositionTypeRelative);
																				   particle1->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
																				   particle1->setEmissionRate(300);
																				   particle1->setAngle(180.0);
																				   particle1->setAngleVar(180.0);
																				   ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
																				   particle1->setBlendFunc(blendFunc);
																				   particle1->setDuration(0.3f);
																				   particle1->setEmitterMode(kCCParticleModeGravity);
																				   particle1->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
																				   particle1->setStartColorVar(ccc4f(0,0,0,0.f));
																				   particle1->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
																				   particle1->setEndColorVar(ccc4f(0, 0, 0, 0.f));
																				   particle1->setStartSize(40.0);
																				   particle1->setStartSizeVar(10.0);
																				   particle1->setEndSize(0.0);
																				   particle1->setEndSizeVar(0.0);
																				   particle1->setGravity(ccp(0,-100));
																				   particle1->setRadialAccel(50.0);
																				   particle1->setRadialAccelVar(20.0);
																				   particle1->setSpeed(50);
																				   particle1->setSpeedVar(30.0);
																				   particle1->setTangentialAccel(0);
																				   particle1->setTangentialAccelVar(0);
																				   particle1->setTotalParticles(100);
																				   particle1->setLife(2.0);
																				   particle1->setLifeVar(0.5);
																				   particle1->setStartSpin(0.0);
																				   particle1->setStartSpinVar(0.f);
																				   particle1->setEndSpin(0.0);
																				   particle1->setEndSpinVar(0.f);
																				   particle1->setPosVar(ccp(90,90));
																				   particle1->setPosition(result_stamp->getPosition());
																				   particle1->setAutoRemoveOnFinish(true);
																				   main_case->addChild(particle1);
																				   
																				   addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																														  {
																															  KS::setOpacity(result_stamp, t*255);
																															  result_stamp->setScale(2.5f-t*1.5f);
																														  }, [=](float t)
																														  {
																															  KS::setOpacity(result_stamp, 255);
																															  result_stamp->setScale(1.f);
																															  
																															  CCParticleSystemQuad* particle2 = CCParticleSystemQuad::createWithTotalParticles(10);
																															  particle2->setPositionType(kCCPositionTypeRelative);
																															  particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
																															  particle2->setEmissionRate(80);
																															  particle2->setAngle(360.0);
																															  particle2->setAngleVar(0.0);
																															  particle2->setBlendFunc(blendFunc);
																															  particle2->setDuration(-1.0);
																															  particle2->setEmitterMode(kCCParticleModeGravity);
																															  particle2->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
																															  particle2->setStartColorVar(ccc4f(0,0,0,0.f));
																															  particle2->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
																															  particle2->setEndColorVar(ccc4f(0, 0, 0, 0.f));
																															  particle2->setStartSize(25.0);
																															  particle2->setStartSizeVar(10.0);
																															  particle2->setEndSize(0.0);
																															  particle2->setEndSizeVar(0.0);
																															  particle2->setGravity(ccp(0,0));
																															  particle2->setRadialAccel(3.0);
																															  particle2->setRadialAccelVar(0.0);
																															  particle2->setSpeed(0);
																															  particle2->setSpeedVar(0.0);
																															  particle2->setTangentialAccel(0);
																															  particle2->setTangentialAccelVar(0);
																															  particle2->setTotalParticles(10);
																															  particle2->setLife(0.8);
																															  particle2->setLifeVar(0.25);
																															  particle2->setStartSpin(0.0);
																															  particle2->setStartSpinVar(50.f);
																															  particle2->setEndSpin(0.0);
																															  particle2->setEndSpinVar(60.f);
																															  particle2->setPosVar(ccp(80,80));
																															  particle2->setPosition(result_stamp->getPosition());
																															  main_case->addChild(particle2);
																															  
																															  if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) != 1)
																															  {
																																  TouchSuctionLayer* t_suction_layer = TouchSuctionLayer::create(touch_priority-1);
																																  t_suction_layer->touch_began_func = [=]()
																																  {
																																	  result_stamp->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																																														   {
																																															   KS::setOpacity(result_stamp, 255-t*155);
																																														   }, [=](float t)
																																														   {
																																															   KS::setOpacity(result_stamp, 100);
																																														   }));
																																	  t_suction_layer->removeFromParent();
																																  };
																																  t_suction_layer->is_on_touch_began_func = true;
																																  addChild(t_suction_layer);
																																  t_suction_layer->setTouchEnabled(true);
																															  }
																															  else
																															  {
																																  result_stamp->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																																													   {
																																														   KS::setOpacity(result_stamp, 255-t*155);
																																													   }, [=](float t)
																																													   {
																																														   KS::setOpacity(result_stamp, 100);
																																													   }));
																															  }
																															  
																															  
																															  is_menu_enable = true;
																															  if(left_total_score.getV() <= right_total_score.getV())
																															  {
																																  stop_button->setVisible(true);
																															  }
																															  else
																															  {
																																  stop_button->setVisible(true);
																																  next_button->setVisible(true);
																															  }
																														  }));
																			   }
																			   else
																			   {
																				   // lose
																				   
																				   result_stamp = CCSprite::create("endless_loser.png");
																				   result_stamp->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
																				   main_case->addChild(result_stamp);
																				   
																				   KS::setOpacity(result_stamp, 0);
																				   result_stamp->setScale(2.5f);
																				   
																				   addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																														  {
																															  KS::setOpacity(result_stamp, t*255);
																															  result_stamp->setScale(2.5f-t*1.5f);
																														  }, [=](float t)
																														  {
																															  KS::setOpacity(result_stamp, 255);
																															  result_stamp->setScale(1.f);
																															  
																															  if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) != 1)
																															  {
																																  TouchSuctionLayer* t_suction_layer = TouchSuctionLayer::create(touch_priority-1);
																																  t_suction_layer->touch_began_func = [=]()
																																  {
																																	  result_stamp->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																																														   {
																																															   KS::setOpacity(result_stamp, 255-t*155);
																																														   }, [=](float t)
																																														   {
																																															   KS::setOpacity(result_stamp, 100);
																																														   }));
																																	  t_suction_layer->removeFromParent();
																																  };
																																  t_suction_layer->is_on_touch_began_func = true;
																																  addChild(t_suction_layer);
																																  t_suction_layer->setTouchEnabled(true);
																															  }
																															  else
																															  {
																																  result_stamp->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
																																													   {
																																														   KS::setOpacity(result_stamp, 255-t*155);
																																													   }, [=](float t)
																																													   {
																																														   KS::setOpacity(result_stamp, 100);
																																													   }));
																															  }
																															  
																															  is_menu_enable = true;
																															  if(left_total_score.getV() <= right_total_score.getV())
																															  {
																																  stop_button->setVisible(true);
																															  }
																															  else
																															  {
																																  stop_button->setVisible(true);
																																  next_button->setVisible(true);
																															  }
																														  }));
																			   }
																			   
																			   if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
																				{
																					CCNode* scenario_node = CCNode::create();
																					addChild(scenario_node, 9999);
																					
																					CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
																					float screen_scale_x = screen_size.width/screen_size.height/1.5f;
																					if(screen_scale_x < 1.f)
																						screen_scale_x = 1.f;
																					
																					float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
																					
																					
																					CCNode* t_stencil_node = CCNode::create();
																					
																					
																					CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
																					t_clipping->setAlphaThreshold(0.1f);
																					
																					float change_scale = 1.f;
																					CCPoint change_origin = ccp(0,0);
																					if(screen_scale_x > 1.f)
																					{
																						change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
																						change_scale = screen_scale_x;
																					}
																					if(screen_scale_y > 1.f)
																						change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
																					CCSize win_size = CCDirector::sharedDirector()->getWinSize();
																					t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
																					
																					
																					CCSprite* t_gray = CCSprite::create("back_gray.png");
																					t_gray->setScaleX(screen_scale_x);
																					t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
																					t_gray->setOpacity(0);
																					t_gray->setPosition(ccp(240,160));
																					t_clipping->addChild(t_gray);
																					
																					t_clipping->setInverted(true);
																					scenario_node->addChild(t_clipping, 0);
																					
																					CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
																					ikaruga->setAnchorPoint(ccp(0,0));
																					ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
																					scenario_node->addChild(ikaruga, 1);
																					
																					TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
																					typing_box->setHide();
																					scenario_node->addChild(typing_box, 2);
																																										
																					typing_box->showAnimation(0.3f);
																					
																					function<void()> end_func5 = [=]()
																					{
																						is_menu_enable = true;
																						addChild(KSTimer::create(0.1f, [=]()
																												 {
																													 scenario_node->removeFromParent();
																												 }));
																					};
																					
																					function<void()> end_func4 = [=]()
																					{
																						myDSH->setIntegerForKey(kDSH_Key_isShowEndlessModeTutorial, -1);
																						
																						typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent17), end_func5);
																						
//																						ikaruga->setVisible(false);
//																						
//																						typing_box->setTouchOffScrollAndButton();
//																						typing_box->setTouchSuction(false);
//																						typing_box->setVisible(false);
//																						
////																						t_gray->removeFromParent();
//																						
//																						CCNode* t_popup_node = CCNode::create();
//																						t_popup_node->setPosition(ccp(240,160));
//																						addChild(t_popup_node, 9999);
//																						
//																						TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
//																						t_popup_node->addChild(t_suction);
//																						t_suction->setTouchEnabled(true);
//																						
//																						CCSprite* out_back = CCSprite::create("popup_small_back.png");
//																						out_back->setPosition(ccp(0,0));
//																						t_popup_node->addChild(out_back);
//																						
//																						KSLabelTTF* t_title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent16), mySGD->getFont().c_str(), 15);
//																						t_title_label->disableOuterStroke();
//																						t_title_label->setPosition(ccpFromSize(out_back->getContentSize()/2.f) + ccp(-85, out_back->getContentSize().height/2.f-35));
//																						out_back->addChild(t_title_label);
//																						
//																						CommonButton* t_close_button = CommonButton::createCloseButton(-99999);
//																						t_close_button->setPosition(ccp(out_back->getContentSize().width - 25, out_back->getContentSize().height - 22));
//																						t_close_button->setFunction([=](CCObject* sender)
//																													{
//																														if(!is_menu_enable)
//																															return;
//																														
//																														is_menu_enable = false;
//																														
//																														AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
//																														
//																														t_close_button->setEnabled(false);
//																														CommonAnimation::closePopup(this, t_popup_node, NULL);
//																														addChild(KSGradualValue<float>::create(255, 0, 0.25f, [=](float t)
//																																							   {
//																																							   }, [=](float t)
//																																							   {
//																																								   is_menu_enable = true;
//																																								   addChild(KSTimer::create(0.1f, [=]()
//																																															{
//																																																scenario_node->removeFromParent();
//																																															}));
//																																								   t_popup_node->removeFromParent();
//																																							   }));
//																													});
//																						out_back->addChild(t_close_button);
//																						t_close_button->setEnabled(false);
//																						
//																						CCScale9Sprite* in_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
//																						in_back->setPosition(ccp(out_back->getContentSize().width/2.f, out_back->getContentSize().height/2.f-12.f));
//																						in_back->setContentSize(CCSizeMake(251, 113));
//																						out_back->addChild(in_back);
//																						
//																						StyledLabelTTF* t_content_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent17), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
//																						t_content_label->setAnchorPoint(ccp(0.5f, 0.5f));
//																						t_content_label->setPosition(ccpFromSize(in_back->getContentSize()/2.f));
//																						in_back->addChild(t_content_label);
//																						
//																						CommonAnimation::openPopup(this, t_popup_node, NULL, nullptr, [=](){t_close_button->setEnabled(true);});
																					};
																					
																					function<void()> end_func3 = [=]()
																					{
                                                                                        myDSH->setIntegerForKey(kDSH_Key_isShowEndlessModeTutorial, -1);
																						ikaruga->setVisible(true);
																						
																						typing_box->setVisible(true);
																						typing_box->setTouchSuction(true);
																						typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent62), end_func5);
																					};
																					
																					function<void()> end_func2 = [=]()
																					{
																						CCScale9Sprite* t_stencil2 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
																						t_stencil2->setContentSize(right_back->getContentSize() + CCSizeMake(10, 10));
																						t_stencil2->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + right_back->getPosition());
																						t_stencil_node->addChild(t_stencil2);
																						
																						CCSprite* t_arrow2 = CCSprite::create("kt_arrow_big.png");
																						t_arrow2->setScale(0.6f);
																						t_arrow2->setRotation(-90);
																						t_arrow2->setPosition(t_stencil2->getPosition() + ccp(-25,t_stencil2->getContentSize().height/2.f + 25));
																						t_clipping->addChild(t_arrow2);
																						
																						StyledLabelTTF* t_ment2 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent13), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kLeftAlignment);
																						t_ment2->setAnchorPoint(ccp(0.f,0.5f));
																						t_ment2->setPosition(t_arrow2->getPosition() + ccp(t_arrow2->getContentSize().height/2.f*t_arrow2->getScale() + 3, 0));
																						t_clipping->addChild(t_ment2);
																						
																						CCNode* scroll_ment_node = CCNode::create();
																						scroll_ment_node->setPosition(main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + left_back->getPosition() + ccpFromSize(left_back->getContentSize()/(-2.f)) +
																													  left_table->getPosition() + ccp((480-30)/2.f-20, 90)/2.f + ccp(0,-13));
																						t_clipping->addChild(scroll_ment_node);
																						
																						CCSprite* scroll_arrow1 = CCSprite::create("kt_arrow.png");
																						scroll_arrow1->setPosition(ccp(0,20));
																						scroll_ment_node->addChild(scroll_arrow1);
																						
																						CCSprite* scroll_arrow2 = CCSprite::create("kt_arrow.png");
																						scroll_arrow2->setRotation(180);
																						scroll_arrow2->setPosition(ccp(0,-20));
																						scroll_ment_node->addChild(scroll_arrow2);
																						
																						StyledLabelTTF* scroll_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent14), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
																						scroll_label->setAnchorPoint(ccp(0.5f,0.5f));
																						scroll_label->setPosition(ccp(0,0));
																						scroll_ment_node->addChild(scroll_label);
																						
																						
																						
																						TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
																						scenario_node->addChild(t_suction);
																						t_suction->setTouchEnabled(true);
																						t_suction->touch_began_func = [=]()
																						{
																							t_suction->is_on_touch_began_func = false;
																							
																							t_stencil_node->removeAllChildren();
																							t_arrow2->removeFromParent();
																							t_ment2->removeFromParent();
																							scroll_ment_node->removeFromParent();
																							end_func3();
																							t_suction->removeFromParent();
																						};
																						t_suction->is_on_touch_began_func = true;
																					};
																					
																					function<void()> end_func1 = [=]()
																					{
																						ikaruga->setVisible(false);
																						
																						typing_box->setTouchOffScrollAndButton();
																						typing_box->setTouchSuction(false);
																						typing_box->setVisible(false);
																						
																						CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
																						t_stencil1->setContentSize(left_back->getContentSize() + CCSizeMake(10, 10));
																						t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + left_back->getPosition());
																						t_stencil_node->addChild(t_stencil1);
																						
																						CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
																						t_arrow1->setScale(0.6f);
																						t_arrow1->setRotation(-90);
																						t_arrow1->setPosition(t_stencil1->getPosition() + ccp(0,t_stencil1->getContentSize().height/2.f + 25));
																						t_clipping->addChild(t_arrow1);
																						
																						StyledLabelTTF* t_ment1 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent12), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kLeftAlignment);
																						t_ment1->setAnchorPoint(ccp(0.f,0.5f));
																						t_ment1->setPosition(t_arrow1->getPosition() + ccp(t_arrow1->getContentSize().height/2.f*t_arrow1->getScale() + 3, 0));
																						t_clipping->addChild(t_ment1);
																						
																						CCNode* scroll_ment_node = CCNode::create();
																						scroll_ment_node->setPosition(main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + right_back->getPosition() + ccpFromSize(right_back->getContentSize()/(-2.f)) +
																													  right_table->getPosition() + ccp(right_back->getContentSize().width-20, 90)/2.f + ccp(9,-13));
																						t_clipping->addChild(scroll_ment_node);
																						
																						CCSprite* scroll_arrow1 = CCSprite::create("kt_arrow.png");
																						scroll_arrow1->setPosition(ccp(0,20));
																						scroll_ment_node->addChild(scroll_arrow1);
																						
																						CCSprite* scroll_arrow2 = CCSprite::create("kt_arrow.png");
																						scroll_arrow2->setRotation(180);
																						scroll_arrow2->setPosition(ccp(0,-20));
																						scroll_ment_node->addChild(scroll_arrow2);
																						
																						StyledLabelTTF* scroll_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent14), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
																						scroll_label->setAnchorPoint(ccp(0.5f,0.5f));
																						scroll_label->setPosition(ccp(0,0));
																						scroll_ment_node->addChild(scroll_label);
																						
																						TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
																						scenario_node->addChild(t_suction);
																						t_suction->setTouchEnabled(true);
																						t_suction->touch_began_func = [=]()
																						{
																							t_suction->is_on_touch_began_func = false;
																							t_stencil_node->removeAllChildren();
																							t_arrow1->removeFromParent();
																							t_ment1->removeFromParent();
																							scroll_ment_node->removeFromParent();
																							end_func2();
																							t_suction->removeFromParent();
																						};
																						t_suction->is_on_touch_began_func = true;
																					};
																					
																					scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																																		  {
																																			  t_gray->setOpacity(t*255);
																																			  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																																		  }, [=](float t)
																																		  {
																																			  t_gray->setOpacity(255);
																																			  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																																			  
																																			  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent61), end_func1);
																																		  }));
																				}

																		   }));
								}
							});
						});
					});
				});
			});
		});
	});
	
//	startLeftCalcAnimation(mySGD->area_score.getV(), 0, 0.5f, left_top_content, [=]()
//   {
//	   flipTarget(left_top_back, left_top_title, left_top_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleLifeScore), left_life_decrease_score);
//	   startLeftCalcAnimation(mySGD->damage_score.getV(), mySGD->area_score.getV(), 0.5f, left_center_content, [=]()
//	  {
//		  flipTarget(left_center_back, left_center_title, left_center_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTimeScore), left_time_decrease_score);
//		  startLeftCalcAnimation(mySGD->combo_score.getV(), mySGD->area_score.getV()+mySGD->damage_score.getV(), 0.5f, left_bottom_content, [=]()
//		 {
//			 flipTarget(left_bottom_back, left_bottom_title, left_bottom_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleGradeScore), left_grade_decrease_score);
//			 startLeftCalcAnimation(left_life_decrease_score, left_life_base_score, 0.5f, left_top_content, [=]()
//			{
//				flipTarget(left_top_back, left_top_title, left_top_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTakeGold), mySGD->getStageGold());
//				startLeftCalcAnimation(left_time_decrease_score, left_time_base_score, 0.5f, left_center_content, [=]()
//			   {
//				   flipTarget(left_center_back, left_center_title, left_center_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTakeArea), mySGD->getPercentage()*100.f);
//				   startLeftCalcAnimation(left_grade_decrease_score, left_grade_base_score, 0.5f, left_bottom_content, [=]()
//				  {
//					  flipTarget(left_bottom_back, left_bottom_title, left_bottom_content, "", 0);
//					  
//					  is_left_calc_end = true;
//					  if(is_left_calc_end && is_right_calc_end)
//					  {
//						  AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
//						  
//						  if(left_total_score > right_total_score)
//						  {
//							  // win
//							  
//							  CCSprite* win_case = CCSprite::create("endless_winner.png");
//							  win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
//							  main_case->addChild(win_case);
//							  
//							  CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
//							  win_label->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f+10));
//							  win_case->addChild(win_label);
//							  
//							  CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
//							  win_ment->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f-25));
//							  win_case->addChild(win_ment);
//							  
//							  win_case->setRotation(-15);
//							  
//							  
//							  KS::setOpacity(win_case, 0);
//							  win_case->setScale(2.5f);
//							  
//							  CCParticleSystemQuad* particle1 = CCParticleSystemQuad::createWithTotalParticles(100);
//							  particle1->setPositionType(kCCPositionTypeRelative);
//							  particle1->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//							  particle1->setEmissionRate(300);
//							  particle1->setAngle(180.0);
//							  particle1->setAngleVar(180.0);
//							  ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
//							  particle1->setBlendFunc(blendFunc);
//							  particle1->setDuration(0.3f);
//							  particle1->setEmitterMode(kCCParticleModeGravity);
//							  particle1->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
//							  particle1->setStartColorVar(ccc4f(0,0,0,0.f));
//							  particle1->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
//							  particle1->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//							  particle1->setStartSize(40.0);
//							  particle1->setStartSizeVar(10.0);
//							  particle1->setEndSize(0.0);
//							  particle1->setEndSizeVar(0.0);
//							  particle1->setGravity(ccp(0,-100));
//							  particle1->setRadialAccel(50.0);
//							  particle1->setRadialAccelVar(20.0);
//							  particle1->setSpeed(50);
//							  particle1->setSpeedVar(30.0);
//							  particle1->setTangentialAccel(0);
//							  particle1->setTangentialAccelVar(0);
//							  particle1->setTotalParticles(100);
//							  particle1->setLife(2.0);
//							  particle1->setLifeVar(0.5);
//							  particle1->setStartSpin(0.0);
//							  particle1->setStartSpinVar(0.f);
//							  particle1->setEndSpin(0.0);
//							  particle1->setEndSpinVar(0.f);
//							  particle1->setPosVar(ccp(90,90));
//							  particle1->setPosition(win_case->getPosition());
//							  particle1->setAutoRemoveOnFinish(true);
//							  main_case->addChild(particle1);
//							  
//							  addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
//																	 {
//																		 KS::setOpacity(win_case, t*255);
//																		 win_case->setScale(2.5f-t*1.5f);
//																	 }, [=](float t)
//																	 {
//																		 KS::setOpacity(win_case, 255);
//																		 win_case->setScale(1.f);
//																		 
//																		 CCParticleSystemQuad* particle2 = CCParticleSystemQuad::createWithTotalParticles(10);
//																		 particle2->setPositionType(kCCPositionTypeRelative);
//																		 particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//																		 particle2->setEmissionRate(80);
//																		 particle2->setAngle(360.0);
//																		 particle2->setAngleVar(0.0);
//																		 particle2->setBlendFunc(blendFunc);
//																		 particle2->setDuration(-1.0);
//																		 particle2->setEmitterMode(kCCParticleModeGravity);
//																		 particle2->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
//																		 particle2->setStartColorVar(ccc4f(0,0,0,0.f));
//																		 particle2->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
//																		 particle2->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//																		 particle2->setStartSize(25.0);
//																		 particle2->setStartSizeVar(10.0);
//																		 particle2->setEndSize(0.0);
//																		 particle2->setEndSizeVar(0.0);
//																		 particle2->setGravity(ccp(0,0));
//																		 particle2->setRadialAccel(3.0);
//																		 particle2->setRadialAccelVar(0.0);
//																		 particle2->setSpeed(0);
//																		 particle2->setSpeedVar(0.0);
//																		 particle2->setTangentialAccel(0);
//																		 particle2->setTangentialAccelVar(0);
//																		 particle2->setTotalParticles(10);
//																		 particle2->setLife(0.8);
//																		 particle2->setLifeVar(0.25);
//																		 particle2->setStartSpin(0.0);
//																		 particle2->setStartSpinVar(50.f);
//																		 particle2->setEndSpin(0.0);
//																		 particle2->setEndSpinVar(60.f);
//																		 particle2->setPosVar(ccp(80,80));
//																		 particle2->setPosition(win_case->getPosition());
//																		 main_case->addChild(particle2);
//																		 
//																		 
//																		 is_menu_enable = true;
//																		 bottom_menu->setVisible(true);
//																	 }));
//						  }
//						  else
//						  {
//							  // lose
//							  
//							  CCSprite* win_case = CCSprite::create("endless_loser.png");
//							  win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
//							  main_case->addChild(win_case);
//							  
//							  KS::setOpacity(win_case, 0);
//							  win_case->setScale(2.5f);
//							  
//							  addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
//																	 {
//																		 KS::setOpacity(win_case, t*255);
//																		 win_case->setScale(2.5f-t*1.5f);
//																	 }, [=](float t)
//																	 {
//																		 KS::setOpacity(win_case, 255);
//																		 win_case->setScale(1.f);
//																		 
//																		 is_menu_enable = true;
//																		 bottom_menu->setVisible(true);
//																	 }));
//						  }
//					  }
//				  });
//			   });
//			});
//		 });
//	  });
//   });
	
	
	
//	startRightCalcAnimation(right_area_score, 0, 0.5f, right_top_content, [=]()
//	{
//		flipTarget(right_top_back, right_top_title, right_top_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleLifeScore), right_life_decrease_score);
//		startRightCalcAnimation(right_damage_score, right_area_score, 0.5f, right_center_content, [=]()
//		{
//			flipTarget(right_center_back, right_center_title, right_center_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTimeScore), right_time_decrease_score);
//			startRightCalcAnimation(right_combo_score, right_area_score+right_damage_score, 0.5f, right_bottom_content, [=]()
//			{
//				flipTarget(right_bottom_back, right_bottom_title, right_bottom_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleGradeScore), right_grade_decrease_score);
//				startRightCalcAnimation(right_life_decrease_score, right_life_base_score, 0.5f, right_top_content, [=]()
//				{
//					flipTarget(right_top_back, right_top_title, right_top_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTakeGold), right_take_gold);
//					startRightCalcAnimation(right_time_decrease_score, right_time_base_score, 0.5f, right_center_content, [=]()
//					{
//						flipTarget(right_center_back, right_center_title, right_center_content, myLoc->getLocalForKey(LK::kMyLocalKey_endlessCalcTitleTakeArea), right_take_area*100.f);
//						startRightCalcAnimation(right_grade_decrease_score, right_grade_base_score, 0.5f, right_bottom_content, [=]()
//						{
//							flipTarget(right_bottom_back, right_bottom_title, right_bottom_content, "", 0);
//							
//							is_right_calc_end = true;
//							if(is_left_calc_end && is_right_calc_end)
//							{
//								AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
//								
//								if(left_total_score > right_total_score)
//								{
//									// win
//									
//									CCSprite* win_case = CCSprite::create("endless_winner.png");
//									win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
//									main_case->addChild(win_case);
//									
//									CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
//									win_label->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f+10));
//									win_case->addChild(win_label);
//									
//									CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
//									win_ment->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f-25));
//									win_case->addChild(win_ment);
//									
//									win_case->setRotation(-15);
//									
//									
//									KS::setOpacity(win_case, 0);
//									win_case->setScale(2.5f);
//									
//									CCParticleSystemQuad* particle1 = CCParticleSystemQuad::createWithTotalParticles(100);
//									particle1->setPositionType(kCCPositionTypeRelative);
//									particle1->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//									particle1->setEmissionRate(300);
//									particle1->setAngle(180.0);
//									particle1->setAngleVar(180.0);
//									ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
//									particle1->setBlendFunc(blendFunc);
//									particle1->setDuration(0.3f);
//									particle1->setEmitterMode(kCCParticleModeGravity);
//									particle1->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
//									particle1->setStartColorVar(ccc4f(0,0,0,0.f));
//									particle1->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
//									particle1->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//									particle1->setStartSize(40.0);
//									particle1->setStartSizeVar(10.0);
//									particle1->setEndSize(0.0);
//									particle1->setEndSizeVar(0.0);
//									particle1->setGravity(ccp(0,-100));
//									particle1->setRadialAccel(50.0);
//									particle1->setRadialAccelVar(20.0);
//									particle1->setSpeed(50);
//									particle1->setSpeedVar(30.0);
//									particle1->setTangentialAccel(0);
//									particle1->setTangentialAccelVar(0);
//									particle1->setTotalParticles(100);
//									particle1->setLife(2.0);
//									particle1->setLifeVar(0.5);
//									particle1->setStartSpin(0.0);
//									particle1->setStartSpinVar(0.f);
//									particle1->setEndSpin(0.0);
//									particle1->setEndSpinVar(0.f);
//									particle1->setPosVar(ccp(90,90));
//									particle1->setPosition(win_case->getPosition());
//									particle1->setAutoRemoveOnFinish(true);
//									main_case->addChild(particle1);
//									
//									addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
//																		   {
//																			   KS::setOpacity(win_case, t*255);
//																			   win_case->setScale(2.5f-t*1.5f);
//																		   }, [=](float t)
//																		   {
//																			   KS::setOpacity(win_case, 255);
//																			   win_case->setScale(1.f);
//																			   
//																			   CCParticleSystemQuad* particle2 = CCParticleSystemQuad::createWithTotalParticles(10);
//																			   particle2->setPositionType(kCCPositionTypeRelative);
//																			   particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//																			   particle2->setEmissionRate(80);
//																			   particle2->setAngle(360.0);
//																			   particle2->setAngleVar(0.0);
//																			   particle2->setBlendFunc(blendFunc);
//																			   particle2->setDuration(-1.0);
//																			   particle2->setEmitterMode(kCCParticleModeGravity);
//																			   particle2->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
//																			   particle2->setStartColorVar(ccc4f(0,0,0,0.f));
//																			   particle2->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
//																			   particle2->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//																			   particle2->setStartSize(25.0);
//																			   particle2->setStartSizeVar(10.0);
//																			   particle2->setEndSize(0.0);
//																			   particle2->setEndSizeVar(0.0);
//																			   particle2->setGravity(ccp(0,0));
//																			   particle2->setRadialAccel(3.0);
//																			   particle2->setRadialAccelVar(0.0);
//																			   particle2->setSpeed(0);
//																			   particle2->setSpeedVar(0.0);
//																			   particle2->setTangentialAccel(0);
//																			   particle2->setTangentialAccelVar(0);
//																			   particle2->setTotalParticles(10);
//																			   particle2->setLife(0.8);
//																			   particle2->setLifeVar(0.25);
//																			   particle2->setStartSpin(0.0);
//																			   particle2->setStartSpinVar(50.f);
//																			   particle2->setEndSpin(0.0);
//																			   particle2->setEndSpinVar(60.f);
//																			   particle2->setPosVar(ccp(80,80));
//																			   particle2->setPosition(win_case->getPosition());
//																			   main_case->addChild(particle2);
//																			   
//																			   
//																			   is_menu_enable = true;
//																			   bottom_menu->setVisible(true);
//																		   }));
//								}
//								else
//								{
//									// lose
//									
//									CCSprite* win_case = CCSprite::create("endless_loser.png");
//									win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
//									main_case->addChild(win_case);
//									
//									KS::setOpacity(win_case, 0);
//									win_case->setScale(2.5f);
//									
//									addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
//																		   {
//																			   KS::setOpacity(win_case, t*255);
//																			   win_case->setScale(2.5f-t*1.5f);
//																		   }, [=](float t)
//																		   {
//																			   KS::setOpacity(win_case, 255);
//																			   win_case->setScale(1.f);
//																			   
//																			   is_menu_enable = true;
//																			   bottom_menu->setVisible(true);
//																		   }));
//								}
//							}
//						});
//					});
//				});
//			});
//		});
//	});
}

void EndlessModeResult::startLeftCalcAnimation(float t_keep_value, float t_base_value, float t_calc_time, KSLabelTTF* t_decrease_target, function<void()> t_end_func)
{
	if(t_keep_value >= 0.f)
	{
		is_left_decrease = true;
		
		keep_left_value = t_keep_value;
	}
	else
	{
		is_left_decrease = false;
		
		keep_left_value = -t_keep_value;
	}
	
	base_left_value = t_base_value;
	decrease_left_value = keep_left_value;
	increase_left_value = 0.f;
	
	left_calc_time = t_calc_time;
	left_decrease_target = t_decrease_target;
	
	left_end_func = t_end_func;
	schedule(schedule_selector(EndlessModeResult::leftCalcAnimation));
}
void EndlessModeResult::leftCalcAnimation(float dt)
{
	if(decrease_left_value > 0)
	{
		int decreaseUnit = keep_left_value / left_calc_time * dt;
		
		if(decrease_left_value < decreaseUnit)
		{
			increase_left_value += decrease_left_value;
			decrease_left_value = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_left_value += decrease_left_value;
				decrease_left_value = 0;
			}
			else {
				decrease_left_value -= decreaseUnit;
				increase_left_value += decreaseUnit;
			}
		}
//		left_decrease_target->setString(CCString::createWithFormat("%.0f",decrease_left_value)->getCString());
		if(is_left_decrease)
			left_total_content->setString(KS::insert_separator(base_left_value + increase_left_value, "%.0f").c_str());
		else
			left_total_content->setString(KS::insert_separator(base_left_value - increase_left_value, "%.0f").c_str());
	}
	else
		stopLeftCalcAnimation();
}
void EndlessModeResult::stopLeftCalcAnimation()
{
	unschedule(schedule_selector(EndlessModeResult::leftCalcAnimation));
	left_end_func();
}

void EndlessModeResult::flipLeft(string t_top_title, int t_top_content, string t_center_title, int t_center_content, string t_bottom_title, int t_bottom_content, function<void()> t_end_func)
{
//	addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//								  {
//									  left_top_back->setScaleY(1.f-t);
//								  }, [=](float t)
//								  {
//									  left_top_back->setScaleY(0.f);
//									  left_top_title->setString(t_top_title.c_str());
//									  left_top_content->setString(CCString::createWithFormat("%d", t_top_content)->getCString());
//									  
//									  addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//																			 {
//																				 left_top_back->setScaleY(t);
//																			 }, [=](float t)
//																			 {
//																				 left_top_back->setScaleY(1.f);
//																			 }));
//								  }));
//	
//	addChild(KSTimer::create(6.f/30.f, [=]()
//							 {
//								 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//																		{
//																			left_center_back->setScaleY(1.f-t);
//																		}, [=](float t)
//																		{
//																			left_center_back->setScaleY(0.f);
//																			left_center_title->setString(t_center_title.c_str());
//																			left_center_content->setString(CCString::createWithFormat("%d", t_center_content)->getCString());
//																			
//																			addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//																												   {
//																													   left_center_back->setScaleY(t);
//																												   }, [=](float t)
//																												   {
//																													   left_center_back->setScaleY(1.f);
//																												   }));
//																		}));
//							 }));
//	
//	addChild(KSTimer::create(13.f/30.f, [=]()
//							 {
//								 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//																		{
//																			left_bottom_back->setScaleY(1.f-t);
//																		}, [=](float t)
//																		{
//																			left_bottom_back->setScaleY(0.f);
//																			left_bottom_title->setString(t_bottom_title.c_str());
//																			left_bottom_content->setString(CCString::createWithFormat("%d", t_bottom_content)->getCString());
//																			
//																			addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//																												   {
//																													   left_bottom_back->setScaleY(t);
//																												   }, [=](float t)
//																												   {
//																													   left_bottom_back->setScaleY(1.f);
//																													   t_end_func();
//																												   }));
//																		}));
//							 }));
}

void EndlessModeResult::flipTarget(CCScale9Sprite* t_back, KSLabelTTF* t_title, KSLabelTTF* t_content, string t_flip_title, int t_flip_content)
{
	addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
										   {
											   t_back->setScaleY(1.f-t);
										   }, [=](float t)
										   {
											   t_back->setScaleY(0.f);
											   t_title->setString(t_flip_title.c_str());
											   t_content->setString(CCString::createWithFormat("%d", t_flip_content)->getCString());
											   
											   addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																					  {
																						  t_back->setScaleY(t);
																					  }, [=](float t)
																					  {
																						  t_back->setScaleY(1.f);
																					  }));
										   }));
}

void EndlessModeResult::startRightCalcAnimation(float t_keep_value, float t_base_value, float t_calc_time, KSLabelTTF* t_decrease_target, function<void()> t_end_func)
{
	if(t_keep_value >= 0.f)
	{
		is_right_decrease = true;
		
		keep_right_value = t_keep_value;
	}
	else
	{
		is_right_decrease = false;
		
		keep_right_value = -t_keep_value;
	}
	
	base_right_value = t_base_value;
	decrease_right_value = keep_right_value;
	increase_right_value = 0.f;
	right_calc_time = t_calc_time;
	right_decrease_target = t_decrease_target;
	
	right_end_func = t_end_func;
	schedule(schedule_selector(EndlessModeResult::rightCalcAnimation));
}
void EndlessModeResult::rightCalcAnimation(float dt)
{
	if(decrease_right_value > 0)
	{
		int decreaseUnit = keep_right_value / right_calc_time * dt;
		
		if(decrease_right_value < decreaseUnit)
		{
			increase_right_value += decrease_right_value;
			decrease_right_value = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_right_value += decrease_right_value;
				decrease_right_value = 0;
			}
			else {
				decrease_right_value -= decreaseUnit;
				increase_right_value += decreaseUnit;
			}
		}
//		right_decrease_target->setString(CCString::createWithFormat("%.0f",decrease_right_value)->getCString());
		if(is_right_decrease)
			right_total_content->setString(KS::insert_separator(base_right_value + increase_right_value, "%.0f").c_str());
		else
			right_total_content->setString(KS::insert_separator(base_right_value - increase_right_value, "%.0f").c_str());
	}
	else
		stopRightCalcAnimation();
}
void EndlessModeResult::stopRightCalcAnimation()
{
	unschedule(schedule_selector(EndlessModeResult::rightCalcAnimation));
	right_end_func();
}

void EndlessModeResult::flipRight(string t_top_title, int t_top_content, string t_center_title, int t_center_content, string t_bottom_title, int t_bottom_content, function<void()> t_end_func)
{
//	addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//										   {
//											   right_top_back->setScaleY(1.f-t);
//										   }, [=](float t)
//										   {
//											   right_top_back->setScaleY(0.f);
//											   right_top_title->setString(t_top_title.c_str());
//											   right_top_content->setString(CCString::createWithFormat("%d", t_top_content)->getCString());
//											   
//											   addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//																					  {
//																						  right_top_back->setScaleY(t);
//																					  }, [=](float t)
//																					  {
//																						  right_top_back->setScaleY(1.f);
//																					  }));
//										   }));
//	
//	addChild(KSTimer::create(6.f/30.f, [=]()
//							 {
//								 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//																		{
//																			right_center_back->setScaleY(1.f-t);
//																		}, [=](float t)
//																		{
//																			right_center_back->setScaleY(0.f);
//																			right_center_title->setString(t_center_title.c_str());
//																			right_center_content->setString(CCString::createWithFormat("%d", t_center_content)->getCString());
//																			
//																			addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//																												   {
//																													   right_center_back->setScaleY(t);
//																												   }, [=](float t)
//																												   {
//																													   right_center_back->setScaleY(1.f);
//																												   }));
//																		}));
//							 }));
//	
//	addChild(KSTimer::create(13.f/30.f, [=]()
//							 {
//								 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
//																		{
//																			right_bottom_back->setScaleY(1.f-t);
//																		}, [=](float t)
//																		{
//																			right_bottom_back->setScaleY(0.f);
//																			right_bottom_title->setString(t_bottom_title.c_str());
//																			right_bottom_content->setString(CCString::createWithFormat("%d", t_bottom_content)->getCString());
//																			
//																			addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
//																												   {
//																													   right_bottom_back->setScaleY(t);
//																												   }, [=](float t)
//																												   {
//																													   right_bottom_back->setScaleY(1.f);
//																													   t_end_func();
//																												   }));
//																		}));
//							 }));
}

void EndlessModeResult::reSetEndlessRank()
{
	TRACE();
	Json::Value param;
	param.clear();
	param["memberID"] = myHSP->getMemberID();
	param["score"] = int(mySGD->getScore());
	param["victory"] = left_total_score.getV() > right_total_score.getV();
	param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	Json::Value p1_data;
	p1_data["character"] = mySGD->getUserdataSelectedCharNO();
	p1_data["highPiece"] = mySGD->getUserdataHighPiece();
	param["data"] = p1_data;

	myHSP->command("finishendlessplay", param, [=](Json::Value result_data)
										{
											TRACE();
											if(result_data["result"]["code"].asInt() != GDSUCCESS)
											{
												TRACE();
												mySGD->network_check_cnt++;
												
												if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
												{
													mySGD->network_check_cnt = 0;
													
													ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
														reSetEndlessRank();
													}, 1);
													if(alert)
														((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
												}
												else
												{
													addChild(KSTimer::create(0.5f, [=]()
																			 {
																				 reSetEndlessRank();
																			 }));
												}
											}
											else
											{
												TRACE();
												mySGD->network_check_cnt = 0;
												
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
												
												CommonAnimation::closePopup(this, main_case, nullptr, [=](){
													
												}, [=](){
//													end_func(); removeFromParent();
												});
											}
											TRACE();
										});
}

void EndlessModeResult::resultGetEndlessPlayData(Json::Value result_data)
{
	TRACE();
	GraphDogLib::JsonToLog("getendlessplaydata : %s", result_data);
	TRACE();
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		TRACE();
		mySGD->resetReplayPlayingInfo();
		
		if(!result_data["rival"]["playData"].isNull())
		{
			Json::Value read_data;
			Json::Reader reader;
			reader.parse(result_data["rival"]["playData"].asString(), read_data);
			mySGD->setReplayPlayingInfo(read_data);
		}
		
		mySGD->dummy_list = result_data["dummy"];
		
		mySGD->endless_memberID = result_data["rival"]["memberID"].asInt64();
	
		mySGD->endless_score = result_data["rival"]["score"].asInt();
		mySGD->endless_regDate = result_data["rival"]["regDate"].asInt64();
		
		mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_stageNo)] = result_data["stageInfo"]["realNo"].asInt();
		
		saveStageInfo(result_data["stageInfo"]);
	}
	else
	{
		TRACE();
		ready_loading->removeFromParent();
		ready_loading = NULL;
		
		addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_endlessServerError)), 999);
	}
	TRACE();
}

void EndlessModeResult::saveStageInfo(Json::Value result_data)
{
	df_list.clear();
//	cf_list.clear();
	
	stage_number = result_data["no"].asInt();
	
	NSDS_SI(stage_number, kSDS_SI_puzzle_i, result_data["puzzle"].asInt(), false);
	NSDS_SI(stage_number, kSDS_SI_playtime_i, result_data["playtime"].asInt(), false);
	NSDS_SD(stage_number, kSDS_SI_scoreRate_d, result_data["scoreRate"].asDouble(), false);
	NSDS_SD(stage_number, kSDS_SI_scale_d, result_data["scale"].asDouble(), false);
//	NSDS_SB(stage_number, kSDS_SI_minigame_b, result_data["minigame"].asBool(), false);
	NSDS_SS(stage_number, kSDS_SI_type_s, result_data["type"].asString(), false);
	
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
		NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
		NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
		
//		NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
//		NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
		NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
		
//		Json::Value t_card_missile = t_card["missile"];
//		NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//		NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//		NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//		
//		NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//		
//		Json::Value t_ability = t_card["ability"];
//		NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//		for(int j=0;j<t_ability.size();j++)
//		{
//			Json::Value t_abil = t_ability[j];
//			NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//			
//			Json::Value t_option;
//			if(!t_abil["option"].isObject())                    t_option["key"]="value";
//			else												t_option =t_abil["option"];
//			
//			if(t_abil["type"].asInt() == kIC_attack)
//				NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_addTime)
//				NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_fast)
//				NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_silence)
//				NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_doubleItem)
//				NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_longTime)
//				NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//				NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//		}
		
		NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
		
		Json::Value t_imgInfo = t_card["imgInfo"];
		
//		bool is_add_cf = false;
		
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
			
//			CopyFile t_cf;
//			t_cf.from_filename = t_df.filename.c_str();
//			t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//			cf_list.push_back(t_cf);
//			
//			is_add_cf = true;
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
			
//			if(is_add_cf)
//			{
//				CopyFile t_cf = cf_list.back();
//				cf_list.pop_back();
//				t_cf.is_ani = true;
//				t_cf.cut_width = t_detail["cutWidth"].asInt();
//				t_cf.cut_height = t_detail["cutHeight"].asInt();
//				t_cf.position_x = t_detail["positionX"].asInt();
//				t_cf.position_y = t_detail["positionY"].asInt();
//				
//				t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//				
//				cf_list.push_back(t_cf);
//			}
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
				df_list.push_back(t_df);
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
			df_list.push_back(t_df1);
			
			DownloadFile t_df2;
			t_df2.size = t_faceInfo["size"].asInt();
			t_df2.img = t_faceInfo["plist"].asString().c_str();
			t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
			t_df2.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str();
			df_list.push_back(t_df2);
			
			DownloadFile t_df3;
			t_df3.size = t_faceInfo["size"].asInt();
			t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
			t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
			t_df3.key = mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str();
			df_list.push_back(t_df3);
			
//			if(!is_add_cf)
//			{
//				CopyFile t_cf;
//				t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//				t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//				cf_list.push_back(t_cf);
//				
//				is_add_cf = true;
//			}
//			
//			CopyFile t_cf = cf_list.back();
//			cf_list.pop_back();
//			t_cf.is_ccb = true;
//			t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//			
//			cf_list.push_back(t_cf);
		}
	}
	
//	NSDS_SI(stage_number, kSDS_SI_level_i, result_data["level"].asInt(), false);
	
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
//		for(int i=0;i<cf_list.size();i++)
//		{
//			mySIL->removeTextureCache(cf_list[i].from_filename);
//			mySIL->removeTextureCache(cf_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
////			if(cf_list[i].is_ani)
////			{
////				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
////				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
////				target_img->addChild(ani_img);
////			}
////			
////			if(cf_list[i].is_ccb)
////			{
////				CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + cf_list[i].ccb_filename.c_str()).first;
////				face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
////				target_img->addChild(face_img);
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
//			if(!(t_texture->saveToFileNoAlpha(cf_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", cf_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
		
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
	addChild(ASPopupView::getCommonNoti(-1000, myLoc->getLocalForKey(LK::kMyLocalKey_stageImgLoadFail), [=]()
										{
											startDownload();
										}, CCPointZero));
}

void EndlessModeResult::successGetStageInfo()
{
	ready_loading->removeFromParent();
	ready_loading = NULL;
	
	RivalSelectPopup* t_popup = RivalSelectPopup::create(touch_priority-100, [=](){is_menu_enable = true;}, [=]()
														 {
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
															 
															 CommonAnimation::closePopup(this, main_case, gray, [=](){
																 
															 }, [=](){
//																 end_func(); removeFromParent();
															 });
														 });
	addChild(t_popup, 999);
}

void EndlessModeResult::scrollViewDidScroll(CCScrollView* view)
{
	if(view == left_table && right_table)
	{
		if(!right_table->getContentOffset().equals(left_table->getContentOffset()))
			right_table->setContentOffset(left_table->getContentOffset());
		left_table->scrollViewDidScroll(left_table);
	}
	else if(view == right_table && left_table)
	{
		if(!left_table->getContentOffset().equals(right_table->getContentOffset()))
			left_table->setContentOffset(right_table->getContentOffset());
		right_table->scrollViewDidScroll(right_table);
	}
}

void EndlessModeResult::scrollViewDidZoom(CCScrollView* view)
{
	
}

CCParticleSystemQuad* EndlessModeResult::getStarParticle()
{
	CCParticleSystemQuad* particle = CCParticleSystemQuad::createWithTotalParticles(50);
	particle->setPositionType(kCCPositionTypeRelative);
	//particle->setAutoRemoveOnFinish(true);
	particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
	particle->setEmissionRate(25);
	particle->setAngle(90.0);
	particle->setAngleVar(120.0);
	ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
	particle->setBlendFunc(blendFunc);
	particle->setDuration(-1);
	particle->setEmitterMode(kCCParticleModeGravity);
	particle->setStartColor(ccc4f(1.f, 0.81f, 0.15f, 1.f));
	particle->setStartColorVar(ccc4f(0,0,0,0.f));
	particle->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
	particle->setEndColorVar(ccc4f(0, 0, 0, 0.f));
	particle->setStartSize(10.0);
	particle->setStartSizeVar(5.0);
	particle->setEndSize(0.0);
	particle->setEndSizeVar(0.0);
	particle->setGravity(ccp(0,0));
	particle->setRadialAccel(0.0);
	particle->setRadialAccelVar(0.0);
	particle->setSpeed(0);
	particle->setSpeedVar(0.0);
	particle->setTangentialAccel(0);
	particle->setTangentialAccelVar(0);
	particle->setTotalParticles(50);
	particle->setLife(1.0);
	particle->setLifeVar(1.0);
	particle->setStartSpin(0.0);
	particle->setStartSpinVar(0.f);
	particle->setEndSpin(0.0);
	particle->setEndSpinVar(0.f);
	particle->setPosVar(ccp(20,20));
	particle->setPosition(ccp(0,0));
	return particle;
}

void EndlessModeResult::addStarParticle(CCNode* t_node)
{
	CCParticleSystemQuad* t_particle = getStarParticle();
	t_particle->setPosition(t_node->getContentSize().width/2.f, t_node->getContentSize().height/2.f);
	t_node->addChild(t_particle);
}