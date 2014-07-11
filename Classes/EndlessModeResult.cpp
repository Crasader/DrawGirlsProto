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
	
	left_table = NULL;
	right_table = NULL;
	
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
			myAchieve->changeIngCount(AchievementCode(i), left_total_score.getV());
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
				AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
				CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
			}
		}
		
		mySGD->setUserdataAchievePerfect(mySGD->getUserdataAchievePerfect() + 1);
	}
	
	title_list.clear();
	left_content_list.clear();
	right_content_list.clear();
	
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleAreaScore));
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleDamageScore));
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleComboScore));
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleLifeScore));
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTimeScore));
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleGradeScore));
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleDamagedScore));
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeGold));
	title_list.push_back(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeArea));
	
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
	
	if(mySGD->is_changed_userdata)
		send_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	Json::Value param;
	param.clear();
	param["memberID"] = myHSP->getMemberID();
	param["score"] = int(mySGD->getScore());
	param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	param["victory"] = left_total_score.getV() > right_total_score.getV();
	
	send_command_list.push_back(CommandParam("finishendlessplay", param, [=](Json::Value result_data)
											 {
												 GraphDogLib::JsonToLog("finishendlessplay", result_data);
												 
												 if(result_data["result"]["code"].asInt() == GDSUCCESS)
													{
														if(result_data["sendGift"].asBool())
														{
															is_menu_enable = false;
															EndlessSeqWinRewardPopup* t_popup = EndlessSeqWinRewardPopup::create(-999, [=](){is_menu_enable = true;}, result_data);
															addChild(t_popup, 999);
														}
													}
											 }));
	
	if(left_total_score.getV() > right_total_score.getV() || mySGD->getIsAlwaysSavePlaydata() == 1)
	{
		Json::Value param2;
		param2.clear();
		param2["memberID"] = myHSP->getMemberID();
		param2["score"] = mySGD->getScore();
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
	
	tryTransaction();
	
	return true;
}

void EndlessModeResult::tryTransaction()
{
	mySGD->changeGoodsTransaction(send_command_list, [=](Json::Value result_data)
								  {
									  if(result_data["result"]["code"].asInt() == GDSUCCESS)
									  {
										  mySGD->network_check_cnt = 0;
										  
										  ready_loading->removeFromParent();
										  ready_loading = NULL;
									  }
									  else
									  {
										  mySGD->network_check_cnt++;
										  
										  if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
										  {
											  mySGD->network_check_cnt = 0;
											  
											  ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
												  tryTransaction();
											  });
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
								  });
}

void EndlessModeResult::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

CCTableViewCell* EndlessModeResult::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSize cell_size = cellSizeForTable(table);
	
	string back_filename;
	string title = title_list[idx];
	string content;
	
	if(idx < 9)
	{
		back_filename = "mainpopup_pupple3.png";
	}
	else
	{
		back_filename = "mainpopup_pupple2.png";
	}
	
	if(table == left_table)
	{
		content = left_content_list[idx];
	}
	else if(table == right_table)
	{
		content = right_content_list[idx];
	}
	
	CCScale9Sprite* t_back = CCScale9Sprite::create(back_filename.c_str(), CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	t_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	t_back->setPosition(ccp(cell_size.width/2.f, cell_size.height/2.f));
	cell->addChild(t_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title.c_str(), mySGD->getFont().c_str(), 13);
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(10, t_back->getContentSize().height/2.f));
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
	content_label->setPosition(ccp(t_back->getContentSize().width-10, t_back->getContentSize().height/2.f));
	t_back->addChild(content_label);
	
	return cell;
}

unsigned int EndlessModeResult::numberOfCellsInTableView(CCTableView *table)
{
	return 9;
}

void EndlessModeResult::setMain()
{
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kEndlessModeResultZorder_back);
	
	left_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	left_back->setContentSize(CCSizeMake((main_case->getContentSize().width-30)/2.f, 212));
	left_back->setPosition(ccp(10+left_back->getContentSize().width/2.f, 165));
	main_case->addChild(left_back);
	
	CCScale9Sprite* left_star_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_star_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 60));
	left_star_back->setPosition(ccp(left_back->getContentSize().width/2.f, 154));
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
	}
	
	CCSprite* left_title = CCSprite::create("endless_nickname.png");
	left_title->setPosition(ccp(left_back->getContentSize().width/2.f, 193.5f));
	left_back->addChild(left_title);
	
	string my_flag = myDSH->getStringForKey(kDSH_Key_flag);
	CCSprite* left_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(my_flag).c_str());
	left_flag->setPosition(ccp(0,26));
	left_title->addChild(left_flag);
	
	KSLabelTTF* left_nick = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 13);
	left_nick->enableOuterStroke(ccBLACK, 1.f);
	left_nick->setPosition(ccp(0, 26));
	left_title->addChild(left_nick);
	
	float left_f_width = left_flag->getContentSize().width * left_flag->getScale();
	float left_n_width = left_nick->getContentSize().width * left_nick->getScale();
	
	left_flag->setPositionX(left_title->getContentSize().width/2.f - left_n_width/2.f-3);
	left_nick->setPositionX(left_title->getContentSize().width/2.f + left_f_width/2.f+3);
	
	
	
	CCRect left_rect = CCRectMake(left_back->getContentSize().width/2.f-((480-30)/2.f-20)/2.f, 51.f-30.f/2.f, (480-30)/2.f-20, 90);
	
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
		left_table->setContentOffset(ccp(0,-9*30));
	else
		left_table->setContentOffset(ccp(0,0*30));
	
	
	right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_back->setContentSize(CCSizeMake((main_case->getContentSize().width-30)/2.f, 212));
	right_back->setPosition(ccp(main_case->getContentSize().width-10-right_back->getContentSize().width/2.f, 165));
	main_case->addChild(right_back);
	
	CCScale9Sprite* right_star_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_star_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20, 60));
	right_star_back->setPosition(ccp(right_back->getContentSize().width/2.f, 154));
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
	}
	
	CCSprite* right_title = CCSprite::create("endless_nickname.png");
	right_title->setPosition(ccp(right_back->getContentSize().width/2.f, 193.5f));
	right_back->addChild(right_title);
	
	string t_right_flag = mySGD->temp_endless_flag.getV();
	CCSprite* right_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(t_right_flag).c_str());
	right_flag->setPosition(ccp(0,26));
	right_title->addChild(right_flag);
	
	KSLabelTTF* right_nick = KSLabelTTF::create(mySGD->temp_endless_nick.getV().c_str(), mySGD->getFont().c_str(), 13);
	right_nick->enableOuterStroke(ccBLACK, 1.f);
	right_nick->setPosition(ccp(0, 26));
	right_title->addChild(right_nick);
	
	float right_f_width = right_flag->getContentSize().width * right_flag->getScale();
	float right_n_width = right_nick->getContentSize().width * right_nick->getScale();
	
	right_flag->setPositionX(right_title->getContentSize().width/2.f - right_n_width/2.f-3);
	right_nick->setPositionX(right_title->getContentSize().width/2.f + right_f_width/2.f+3);
	
	
	
	CCRect right_rect = CCRectMake(right_back->getContentSize().width/2.f-(right_back->getContentSize().width-20)/2.f, 51.f-30.f/2.f, right_back->getContentSize().width-20, 90);
	
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
		right_table->setContentOffset(ccp(0,-9*30));
	else
		right_table->setContentOffset(ccp(0,0*30));
	
	
		
	CCScale9Sprite* left_total_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_total_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_total_back->setPosition(ccp(left_back->getContentSize().width/2.f, 21));
	left_back->addChild(left_total_back);
	
	KSLabelTTF* left_total_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_totalScore), mySGD->getFont().c_str(), 15);
	left_total_title->setColor(ccc3(255, 170, 20));
	left_total_title->setAnchorPoint(ccp(0,0.5f));
	left_total_title->setPosition(ccp(10, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_title);
	
	string start_total_left_content;
	if(is_calc)
		start_total_left_content = "0";
	else
		start_total_left_content = KS::insert_separator(left_total_score.getV());
	
	left_total_content = KSLabelTTF::create(start_total_left_content.c_str(), mySGD->getFont().c_str(), 15);
	left_total_content->setColor(ccc3(255, 170, 20));
	left_total_content->setAnchorPoint(ccp(1,0.5f));
	left_total_content->setPosition(ccp(left_total_back->getContentSize().width-10, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_content);
	
	
	string stop_string;
	if(left_total_score.getV() <= right_total_score.getV())
		stop_string = myLoc->getLocalForKey(kMyLocalKey_endlessKeepWin);
	else
		stop_string = myLoc->getLocalForKey(kMyLocalKey_endlessKeepWinTitle);
	
	CCSprite* n_stop = CCSprite::create("endless_ready.png");
	
	n_stop_label2 = KSLabelTTF::create(stop_string.c_str(), mySGD->getFont().c_str(), 22);
	n_stop_label2->setColor(ccWHITE);
	n_stop_label2->setOpacity(100);
	n_stop_label2->disableOuterStroke();
	n_stop_label2->setPosition(ccp(n_stop->getContentSize().width/2.f, n_stop->getContentSize().height/2.f-2));
	n_stop->addChild(n_stop_label2);
	
	KSLabelTTF* n_stop_label = KSLabelTTF::create(stop_string.c_str(), mySGD->getFont().c_str(), 22);
	n_stop_label->setColor(ccc3(50, 30, 5));
	n_stop_label->setPosition(ccp(n_stop->getContentSize().width/2.f, n_stop->getContentSize().height/2.f-1));
	n_stop->addChild(n_stop_label);
	
	CCSprite* s_stop = CCSprite::create("endless_ready.png");
	s_stop->setColor(ccGRAY);
	
	s_stop_label2 = KSLabelTTF::create(stop_string.c_str(), mySGD->getFont().c_str(), 22);
	s_stop_label2->setColor(ccWHITE);
	s_stop_label2->setOpacity(100);
	s_stop_label2->disableOuterStroke();
	s_stop_label2->setPosition(ccp(s_stop->getContentSize().width/2.f, s_stop->getContentSize().height/2.f-2));
	s_stop->addChild(s_stop_label2);
	
	KSLabelTTF* s_stop_label = KSLabelTTF::create(stop_string.c_str(), mySGD->getFont().c_str(), 22);
	s_stop_label->setColor(ccc3(50, 30, 5));
	s_stop_label->setPosition(ccp(s_stop->getContentSize().width/2.f, s_stop->getContentSize().height/2.f-1));
	s_stop->addChild(s_stop_label);
	
	CCMenuItemLambda* stop_item = CCMenuItemSpriteLambda::create(n_stop, s_stop, [=](CCObject* sender)
																  {
																	  if(!is_menu_enable)
																		  return;
																	  
																	  is_menu_enable = false;
																	  
																	  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																	  
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
																			addChild(t_popup, 999);
																			
																			CCScale9Sprite* back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
																			back_case->setContentSize(CCSizeMake(240,140));
																			back_case->setPosition(ccp(0,0));
																			t_container->addChild(back_case);
																			
																			CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
																			back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
																			back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
																			back_case->addChild(back_in);
																			
																			KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessKeepWinTitle), mySGD->getFont().c_str(), 15);
																			title_label->setColor(ccc3(255, 170, 20));
																			title_label->setAnchorPoint(ccp(0.5f,0.5f));
																			title_label->setPosition(ccp(0,back_case->getContentSize().height/2.f-25));
																			t_container->addChild(title_label);
																			
																			KSLabelTTF* sub_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessKeepWinContent), mySGD->getFont().c_str(), 12);
																			sub_label->setHorizontalAlignment(kCCTextAlignmentCenter);
																			sub_label->setAnchorPoint(ccp(0.5f,0.5f));
																			sub_label->setPosition(ccp(0,5));
																			t_container->addChild(sub_label);
																			
																			CCSprite* t_gray = t_popup->getDimmedSprite();
																			
																			CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
																			close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25,back_case->getContentSize().height/2.f-25));
																			close_button->setFunction([=](CCObject* sender)
																																{
																																	if(!t_popup->is_menu_enable)
																																		return;
																																	
																																	t_popup->is_menu_enable = false;
																																	
																																	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																																	
																																	CommonAnimation::closePopup(t_popup, t_container, t_gray, [=](){
																																		
																																	}, [=](){
																																		is_menu_enable = true;
																																		t_popup->removeFromParent();
																																	});
																																});
																			t_container->addChild(close_button);
																			
																			t_popup->button_func_list.clear();
																			
																			t_popup->button_func_list.push_back([=](){
																				if(!t_popup->is_menu_enable)
																					return;
																				
																				t_popup->is_menu_enable = false;
																				
																				AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																				
																				CommonAnimation::closePopup(this, t_container, t_gray, [=](){
																					
																				}, [=](){
																					t_gray->setOpacity(0);
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
																															 if(t > 100)
																															 {
																																 n_stop_label2->setOpacity(100);
																																 s_stop_label2->setOpacity(100);
																																 n_next_label2->setOpacity(100);
																																 s_next_label2->setOpacity(100);
																															 }
																														 }, [=](int t)
																														 {
																															 KS::setOpacity(main_case, 0);
																															 n_stop_label2->setOpacity(0);
																															 s_stop_label2->setOpacity(0);
																															 n_next_label2->setOpacity(0);
																															 s_next_label2->setOpacity(0);
																														 }));
																					
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
																			ok_button->setPosition(ccp(0,-35));
																			t_container->addChild(ok_button);
																			
																			ok_button->setTouchPriority(t_popup->getTouchPriority()-5);
																			
																			
																			t_popup->is_menu_enable = false;
																			CommonAnimation::openPopup(this, t_container, t_gray, [=](){

																			}, [=](){
																				t_popup->is_menu_enable = true;
																			});
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
																			
																			n_stop_label2->setOpacity(100);
																			s_stop_label2->setOpacity(100);
																			n_next_label2->setOpacity(100);
																			s_next_label2->setOpacity(100);
																			CommonAnimation::closePopup(this, main_case, nullptr, [=](){
																				
																			}, [=](){
//																				end_func(); removeFromParent();
																			});
																		}
																  });
	stop_item->setPosition(ccp(-main_case->getContentSize().width/2.f+left_back->getPositionX(), 0));
	
	
		
	CCScale9Sprite* right_total_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_total_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20, 35));
	right_total_back->setPosition(ccp(right_back->getContentSize().width/2.f, 21));
	right_back->addChild(right_total_back);
	
	KSLabelTTF* right_total_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_totalScore), mySGD->getFont().c_str(), 15);
	right_total_title->setColor(ccc3(255, 170, 20));
	right_total_title->setAnchorPoint(ccp(0,0.5f));
	right_total_title->setPosition(ccp(10, right_total_back->getContentSize().height/2.f));
	right_total_back->addChild(right_total_title);
	
	string start_total_right_content;
	if(is_calc)
		start_total_right_content = "0";
	else
		start_total_right_content = KS::insert_separator(right_total_score.getV());
	
	right_total_content = KSLabelTTF::create(start_total_right_content.c_str(), mySGD->getFont().c_str(), 15);
	right_total_content->setColor(ccc3(255, 170, 20));
	right_total_content->setAnchorPoint(ccp(1,0.5f));
	right_total_content->setPosition(ccp(right_total_back->getContentSize().width-10, right_total_back->getContentSize().height/2.f));
	right_total_back->addChild(right_total_content);
	
	
	
	
	
	
	string next_button_str;
	if(mySGD->endless_my_victory.getV() > 0)
		next_button_str = CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_endlessReadyIngWin), mySGD->endless_my_victory.getV()+1)->getCString();
	else
		next_button_str = myLoc->getLocalForKey(kMyLocalKey_nextStage);
	
	CCSprite* n_next = CCSprite::create("endless_ready.png");
	
	n_next_label2 = KSLabelTTF::create(next_button_str.c_str(), mySGD->getFont().c_str(), 22);
	n_next_label2->setColor(ccWHITE);
	n_next_label2->setOpacity(100);
	n_next_label2->disableOuterStroke();
	n_next_label2->setPosition(ccp(n_next->getContentSize().width/2.f, n_next->getContentSize().height/2.f-2));
	n_next->addChild(n_next_label2);
	
	KSLabelTTF* n_next_label = KSLabelTTF::create(next_button_str.c_str(), mySGD->getFont().c_str(), 22);
	n_next_label->setColor(ccc3(50, 30, 5));
	n_next_label->setPosition(ccp(n_next->getContentSize().width/2.f, n_next->getContentSize().height/2.f-1));
	n_next->addChild(n_next_label);
	
	CCSprite* s_next = CCSprite::create("endless_ready.png");
	s_next->setColor(ccGRAY);
	
	s_next_label2 = KSLabelTTF::create(next_button_str.c_str(), mySGD->getFont().c_str(), 22);
	s_next_label2->setColor(ccWHITE);
	s_next_label2->setOpacity(100);
	s_next_label2->disableOuterStroke();
	s_next_label2->setPosition(ccp(s_next->getContentSize().width/2.f, s_next->getContentSize().height/2.f-2));
	s_next->addChild(s_next_label2);
	
	KSLabelTTF* s_next_label = KSLabelTTF::create(next_button_str.c_str(), mySGD->getFont().c_str(), 22);
	s_next_label->setColor(ccc3(50, 30, 5));
	s_next_label->setPosition(ccp(s_next->getContentSize().width/2.f, s_next->getContentSize().height/2.f-1));
	s_next->addChild(s_next_label);
	
	CCMenuItemLambda* next_item = CCMenuItemSpriteLambda::create(n_next, s_next, [=](CCObject* sender)
																 {
																	 if(!is_menu_enable)
																		 return;
																	 
																	 is_menu_enable = false;
																	 
																	 AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																	 
																	 ready_loading = LoadingLayer::create(-999);
																	 addChild(ready_loading, 999);

																	 vector<CommandParam> command_list;
																	 command_list.clear();
																	 
																	 Json::Value param3;
																	 param3["memberID"] = myHSP->getMemberID();
																	 
																	 command_list.push_back(CommandParam("starttransaction", param3, nullptr));
																	 
																	 Json::Value param;
																	 param["memberID"] = myHSP->getMemberID();
																	 param["autoLevel"] = mySGD->getUserdataAutoLevel();
																	 param["highPiece"] = mySGD->getUserdataHighPiece();
																	 command_list.push_back(CommandParam("getendlessplayriver", param, json_selector(this, EndlessModeResult::resultGetEndlessPlayData)));
																	 
																	 myHSP->command(command_list);
																 });
	next_item->setPosition(ccp(-main_case->getContentSize().width/2.f+right_back->getPositionX(), 0));
	
	bottom_menu = CCMenuLambda::create();
	bottom_menu->setPosition(ccp(main_case->getContentSize().width/2.f,35));
	bottom_menu->setTouchPriority(touch_priority-2);
	main_case->addChild(bottom_menu);
	
	bottom_menu->addChild(stop_item);
	bottom_menu->addChild(next_item);
	
	if(left_total_score.getV() <= right_total_score.getV())
	{
		next_item->setVisible(false);
		stop_item->setPosition(ccp(0, 0));
	}
	
	CCSprite* vs_img = CCSprite::create("endless_vs.png");
	vs_img->setPosition(ccp(main_case->getContentSize().width/2.f, 210));
	main_case->addChild(vs_img);
	
	if(is_calc)
	{
		bottom_menu->setVisible(false);
	}
	else
	{
		if(left_total_score.getV() > right_total_score.getV())
		{
			// win
			
			result_stamp = CCSprite::create("endless_winner.png");
			result_stamp->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
			main_case->addChild(result_stamp);
			
			CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_ing_win.getV())->getCString(), "winfont.fnt");
			win_label->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f+10));
			result_stamp->addChild(win_label);
			
			CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
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
			
//			if(t > 100)
//			{
//				n_stop_label2->setOpacity(100);
//				s_stop_label2->setOpacity(100);
//				n_next_label2->setOpacity(100);
//				s_next_label2->setOpacity(100);
//			}
		}, [=](){
			n_stop_label2->setOpacity(100);
			s_stop_label2->setOpacity(100);
			n_next_label2->setOpacity(100);
			s_next_label2->setOpacity(100);
			
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
													   left_table->setContentOffset(ccp(0, before_y+30*t));
													   right_table->setContentOffset(ccp(0, before_y+30*t));
												   }, [=](float t)
												   {
													   left_table->setContentOffset(ccp(0, before_y+30));
													   right_table->setContentOffset(ccp(0, before_y+30));
													   
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
	
	t_func1(-9*30, mySGD->area_score.getV(), 0, right_area_score.getV(), 0, [=]()
	{
		t_func1(-8*30, mySGD->damage_score.getV(), mySGD->area_score.getV(), right_damage_score.getV(), right_area_score.getV(), [=]()
		{
			t_func1(-7*30, mySGD->combo_score.getV(), mySGD->area_score.getV()+mySGD->damage_score.getV(), right_combo_score.getV(), right_area_score.getV()+right_damage_score.getV(), [=]()
			{
				t_func1(-6*30, left_life_decrease_score.getV(), left_life_base_score.getV(), right_life_decrease_score.getV(), right_life_base_score.getV(), [=]()
				{
					t_func1(-5*30, left_time_decrease_score.getV(), left_time_base_score.getV(), right_time_decrease_score.getV(), right_time_base_score.getV(), [=]()
					{
						t_func1(-4*30, left_grade_decrease_score.getV(), left_grade_base_score.getV(), right_grade_decrease_score.getV(), right_grade_base_score.getV(), [=]()
						{
							t_func1(-3*30, left_damaged_score.getV(), left_grade_base_score.getV()+left_grade_decrease_score.getV(), right_damaged_score.getV(), right_grade_base_score.getV()+right_grade_decrease_score.getV(), [=]()
							{
								if(this->is_left_calc_end && this->is_right_calc_end)
								{
									AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
									
									addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t)
																		   {
																			   left_table->setContentOffset(ccp(0, -2*30+60*t));
																			   right_table->setContentOffset(ccp(0, -2*30+60*t));
																		   }, [=](float t)
																		   {
																			   left_table->setContentOffset(ccp(0, -2*30+60));
																			   right_table->setContentOffset(ccp(0, -2*30+60));
																			   
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
																				   
																				   CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_ing_win.getV())->getCString(), "winfont.fnt");
																				   win_label->setPosition(ccp(result_stamp->getContentSize().width/2.f, result_stamp->getContentSize().height/2.f+10));
																				   result_stamp->addChild(win_label);
																				   
																				   CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
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
																															  bottom_menu->setVisible(true);
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
																															  bottom_menu->setVisible(true);
																														  }));
																			   }
																			   
																			   if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
																				{
																					CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
																					float screen_scale_x = screen_size.width/screen_size.height/1.5f;
																					if(screen_scale_x < 1.f)
																						screen_scale_x = 1.f;
																					
																					CCNode* t_stencil_node = CCNode::create();
																					CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
																					t_stencil1->setContentSize(left_back->getContentSize() + CCSizeMake(10, 10));
																					t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + left_back->getPosition());
																					t_stencil_node->addChild(t_stencil1);
																					
																					CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
																					t_clipping->setAlphaThreshold(0.1f);
																					
																					float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
																					
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
																					
																					CCSprite* t_arrow1 = CCSprite::create("main_tutorial_arrow1.png");
																					t_arrow1->setRotation(180);
																					t_arrow1->setPosition(t_stencil1->getPosition() + ccp(0,t_stencil1->getContentSize().height/2.f + 10));
																					t_clipping->addChild(t_arrow1);
																					
																					StyledLabelTTF* t_ment1 = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent12), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
																					t_ment1->setAnchorPoint(ccp(0.5f,0.f));
																					t_ment1->setPosition(t_arrow1->getPosition() + ccp(0, t_arrow1->getContentSize().height/2.f + 3));
																					t_clipping->addChild(t_ment1);
																					
																					CCNode* scroll_ment_node = CCNode::create();
																					scroll_ment_node->setPosition(main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + right_back->getPosition() + ccpFromSize(right_back->getContentSize()/(-2.f)) +
																												  right_table->getPosition() + ccp(right_back->getContentSize().width-20, 90)/2.f);
																					t_clipping->addChild(scroll_ment_node);
																					
																					CCSprite* scroll_arrow1 = CCSprite::create("main_tutorial_arrow1.png");
																					scroll_arrow1->setPosition(ccp(0,20));
																					scroll_ment_node->addChild(scroll_arrow1);
																					
																					CCSprite* scroll_arrow2 = CCSprite::create("main_tutorial_arrow1.png");
																					scroll_arrow2->setRotation(180);
																					scroll_arrow2->setPosition(ccp(0,-20));
																					scroll_ment_node->addChild(scroll_arrow2);
																					
																					StyledLabelTTF* scroll_label = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent14), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
																					scroll_label->setAnchorPoint(ccp(0.5f,0.5f));
																					scroll_label->setPosition(ccp(0,0));
																					scroll_ment_node->addChild(scroll_label);
																					
																					
																					TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
																					addChild(t_suction);
																					t_suction->setTouchEnabled(true);
																					
																					t_clipping->setInverted(true);
																					addChild(t_clipping, 9999);
																					
																					addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
																														   {
																															   t_gray->setOpacity(t*255);
																														   }, [=](float t)
																														   {
																															   t_gray->setOpacity(255);
																															   
																															   t_suction->is_on_touch_began_func = true;
																															   t_suction->touch_began_func = [=]()
																															   {
																																   t_suction->is_on_touch_began_func = false;
																																   
																																   t_arrow1->removeFromParent();
																																   t_ment1->removeFromParent();
																																   
																																   t_stencil1->setContentSize(right_back->getContentSize() + CCSizeMake(10, 10));
																																   t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + right_back->getPosition());
																																   
																																   CCSprite* t_arrow2 = CCSprite::create("main_tutorial_arrow1.png");
																																   t_arrow2->setRotation(180);
																																   t_arrow2->setPosition(t_stencil1->getPosition() + ccp(0,t_stencil1->getContentSize().height/2.f + 10));
																																   t_clipping->addChild(t_arrow2);
																																   
																																   StyledLabelTTF* t_ment2 = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent13), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kLeftAlignment);
																																   t_ment2->setAnchorPoint(ccp(0.5f,0.f));
																																   t_ment2->setPosition(t_arrow2->getPosition() + ccp(0, t_arrow2->getContentSize().height/2.f + 3));
																																   t_clipping->addChild(t_ment2);
																																   
																																   scroll_ment_node->setPosition(main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + left_back->getPosition() + ccpFromSize(left_back->getContentSize()/(-2.f)) +
																																								 left_table->getPosition() + ccp((480-30)/2.f-20, 90)/2.f);
																																   
																																   t_suction->is_on_touch_began_func = true;
																																   t_suction->touch_began_func = [=]()
																																   {
																																	   t_suction->is_on_touch_began_func = false;
																																	   
																																	   t_arrow2->removeFromParent();
																																	   t_ment2->removeFromParent();
																																	   
																																	   scroll_ment_node->removeFromParent();
																																	   
																																	   t_stencil1->setContentSize(CCSizeMake(200, 55) + CCSizeMake(10, 10));
																																	   t_stencil1->setPosition(main_case->getPosition() + ccpFromSize(main_case->getContentSize()/(-2.f)) + bottom_menu->getPosition() + ccp(-main_case->getContentSize().width/2.f+right_back->getPositionX(), 0));
																																	   
																																	   CCSprite* t_arrow3 = CCSprite::create("main_tutorial_arrow1.png");
																																	   t_arrow3->setRotation(180);
																																	   t_arrow3->setPosition(t_stencil1->getPosition() + ccp(0, t_stencil1->getContentSize().height/2.f + 15));
																																	   t_clipping->addChild(t_arrow3);
																																	   
																																	   StyledLabelTTF* t_ment3 = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent15), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
																																	   t_ment3->setAnchorPoint(ccp(0.5f,0.f));
																																	   t_ment3->setPosition(t_arrow3->getPosition() + ccp(0, t_arrow3->getContentSize().height/2.f + 3));
																																	   t_clipping->addChild(t_ment3);
																																	   
																																	   t_suction->is_on_touch_began_func = true;
																																	   t_suction->touch_began_func = [=]()
																																	   {
																																		   t_suction->is_on_touch_began_func = false;
																																		   
																																		   t_arrow3->removeFromParent();
																																		   t_ment3->removeFromParent();
																																		   
																																		   t_stencil1->setContentSize(CCSizeMake(10, 10));
																																		   t_stencil1->setPosition(ccp(-500,-500));
																																		   
																																		   CCNode* t_popup_node = CCNode::create();
																																		   t_popup_node->setPosition(ccp(240,160));
																																		   t_clipping->addChild(t_popup_node);
																																		   
																																		   CCScale9Sprite* out_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
																																		   out_back->setContentSize(CCSizeMake(250, 130));
																																		   out_back->setPosition(ccp(0,0));
																																		   t_popup_node->addChild(out_back);
																																		   
																																		   KSLabelTTF* t_title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent16), mySGD->getFont().c_str(), 15);
																																		   t_title_label->setColor(ccc3(255,170,20));
																																		   t_title_label->setPosition(ccp(out_back->getContentSize().width/2.f, out_back->getContentSize().height-25));
																																		   out_back->addChild(t_title_label);
																																		   
																																		   CommonButton* t_close_button = CommonButton::createCloseButton(-99999);
																																		   t_close_button->setPosition(ccp(out_back->getContentSize().width - 25, out_back->getContentSize().height - 25));
																																		   t_close_button->setFunction([=](CCObject* sender)
																																									   {
																																										   if(!is_menu_enable)
																																											   return;
																																										   
																																										   is_menu_enable = false;
																																										   
																																										   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																																										   
																																										   t_close_button->setEnabled(false);
																																										   CommonAnimation::closePopup(this, t_popup_node, NULL);
																																										   addChild(KSGradualValue<float>::create(255, 0, 0.25f, [=](float t)
																																																				  {
																																																					  t_gray->setOpacity(t);
																																																				  }, [=](float t)
																																																				  {
																																																					  t_gray->setOpacity(0);
																																																					  
																																																					  myDSH->setIntegerForKey(kDSH_Key_isShowEndlessModeTutorial, -1);
																																																					  
																																																					  ready_loading = LoadingLayer::create(-999);
																																																					  addChild(ready_loading, 999);
																																																					  
																																																					  vector<CommandParam> command_list;
																																																					  command_list.clear();
																																																					  
																																																					  Json::Value param3;
																																																					  param3["memberID"] = myHSP->getMemberID();
																																																					  
																																																					  command_list.push_back(CommandParam("starttransaction", param3, nullptr));
																																																					  
																																																					  Json::Value param;
																																																					  param["memberID"] = myHSP->getMemberID();
																																																					  param["autoLevel"] = mySGD->getUserdataAutoLevel();
																																																					  param["highPiece"] = mySGD->getUserdataHighPiece();
																																																					  command_list.push_back(CommandParam("getendlessplayriver", param, json_selector(this, EndlessModeResult::resultGetEndlessPlayData)));
																																																					  
																																																					  myHSP->command(command_list);
																																																					  
																																																					  t_suction->removeFromParent();
																																																					  
																																																					  t_clipping->removeFromParent();
																																																				  }));
																																									   });
																																		   out_back->addChild(t_close_button);
																																		   t_close_button->setEnabled(false);
																																		   
																																		   CCScale9Sprite* in_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
																																		   in_back->setPosition(ccp(out_back->getContentSize().width/2.f, out_back->getContentSize().height/2.f-15.5f));
																																		   in_back->setContentSize(out_back->getContentSize() + CCSizeMake(-11, -41.5f));
																																		   out_back->addChild(in_back);
																																		   
																																		   StyledLabelTTF* t_content_label = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessTutorialMent17), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
																																		   t_content_label->setAnchorPoint(ccp(0.5f, 0.5f));
																																		   t_content_label->setPosition(ccpFromSize(in_back->getContentSize()/2.f));
																																		   in_back->addChild(t_content_label);
																																		   
																																		   CommonAnimation::openPopup(this, t_popup_node, NULL, nullptr, [=](){t_close_button->setEnabled(true);});
																																	   };
																																   };
																															   };
																															   
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
//	   flipTarget(left_top_back, left_top_title, left_top_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleLifeScore), left_life_decrease_score);
//	   startLeftCalcAnimation(mySGD->damage_score.getV(), mySGD->area_score.getV(), 0.5f, left_center_content, [=]()
//	  {
//		  flipTarget(left_center_back, left_center_title, left_center_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTimeScore), left_time_decrease_score);
//		  startLeftCalcAnimation(mySGD->combo_score.getV(), mySGD->area_score.getV()+mySGD->damage_score.getV(), 0.5f, left_bottom_content, [=]()
//		 {
//			 flipTarget(left_bottom_back, left_bottom_title, left_bottom_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleGradeScore), left_grade_decrease_score);
//			 startLeftCalcAnimation(left_life_decrease_score, left_life_base_score, 0.5f, left_top_content, [=]()
//			{
//				flipTarget(left_top_back, left_top_title, left_top_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeGold), mySGD->getStageGold());
//				startLeftCalcAnimation(left_time_decrease_score, left_time_base_score, 0.5f, left_center_content, [=]()
//			   {
//				   flipTarget(left_center_back, left_center_title, left_center_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeArea), mySGD->getPercentage()*100.f);
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
//		flipTarget(right_top_back, right_top_title, right_top_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleLifeScore), right_life_decrease_score);
//		startRightCalcAnimation(right_damage_score, right_area_score, 0.5f, right_center_content, [=]()
//		{
//			flipTarget(right_center_back, right_center_title, right_center_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTimeScore), right_time_decrease_score);
//			startRightCalcAnimation(right_combo_score, right_area_score+right_damage_score, 0.5f, right_bottom_content, [=]()
//			{
//				flipTarget(right_bottom_back, right_bottom_title, right_bottom_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleGradeScore), right_grade_decrease_score);
//				startRightCalcAnimation(right_life_decrease_score, right_life_base_score, 0.5f, right_top_content, [=]()
//				{
//					flipTarget(right_top_back, right_top_title, right_top_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeGold), right_take_gold);
//					startRightCalcAnimation(right_time_decrease_score, right_time_base_score, 0.5f, right_center_content, [=]()
//					{
//						flipTarget(right_center_back, right_center_title, right_center_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeArea), right_take_area*100.f);
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
	Json::Value param;
	param.clear();
	param["memberID"] = myHSP->getMemberID();
	param["score"] = int(mySGD->getScore());
	param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	param["level"] = mySGD->endless_my_level.getV();
	param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	param["victory"] = left_total_score.getV() > right_total_score.getV();
	
	myHSP->command("finishendlessplay", param, [=](Json::Value result_data)
										{
											if(result_data["result"]["code"].asInt() != GDSUCCESS)
											{
												mySGD->network_check_cnt++;
												
												if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
												{
													mySGD->network_check_cnt = 0;
													
													ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
														reSetEndlessRank();
													});
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
										});
}

void EndlessModeResult::resultGetEndlessPlayData(Json::Value result_data)
{
	GraphDogLib::JsonToLog("getendlessplaydata : %s", result_data);
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
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
//		NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
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
		NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
		NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
		
		NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
		NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
		NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
		
		int sound_cnt = t_card["sound"].size();
		NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
		for(int j=1;j<=sound_cnt;j++)
		{
			NSDS_SI(kSDS_CI_int1_soundNumber_int1_i, t_card["no"].asInt(), j, t_card["sound"][j-1].asInt(), false);
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
															 
															 n_stop_label2->setOpacity(100);
															 s_stop_label2->setOpacity(100);
															 n_next_label2->setOpacity(100);
															 s_next_label2->setOpacity(100);
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