//
//  FailPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 19..
//
//

#include "FailPopup.h"
#include "StarGoldData.h"
#include "EnumDefine.h"
#include "FailHelpPopup.h"
#include "LogData.h"
#include "CountingBMLabel.h"
#include "DecreaseCardDurabilityPopup.h"
#include "DownloadFile.h"
#include "StageSettingPopup.h"
#include "PuzzleMapScene.h"

typedef enum tMenuTagFailPopup{
	kMT_FP_main = 1,
	kMT_FP_replay
	//	kMT_FP_help
}MenuTagFailPopup;

typedef enum tZorderFailPopup{
	kZ_FP_gray = 1,
	kZ_FP_back,
	kZ_FP_img,
	kZ_FP_menu,
	kZ_FP_popup
}ZorderFailPopup;

void FailPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool FailPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	is_menu_enable = false;
	is_loaded_list = false;
	
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	myLog->addLog(kLOG_remainHeart_i, -1, myDSH->getIntegerForKey(kDSH_Key_heartCnt));
	
	Json::Value param;
	param["key"] = CCSTR_CWF("stage_over_%d", mySD->getSilType())->getCString();
	
	hspConnector::get()->command("increaseStats", param, NULL);
	
	myLog->sendLog(CCString::createWithFormat("fail_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
	
	if(mySGD->getScore() > myDSH->getIntegerForKey(kDSH_Key_allHighScore))
	{
		myDSH->setIntegerForKey(kDSH_Key_allHighScore, int(mySGD->getScore()));
	}
	Json::Value p1;
	p1["memberID"] = hspConnector::get()->getKakaoID();
	p1["score"] = int(mySGD->getScore());
	Json::Value p1_data;
	p1_data["selectedcard"] = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	p1_data["allhighscore"] = myDSH->getIntegerForKey(kDSH_Key_allHighScore);
	Json::FastWriter p1_data_writer;
	p1["data"] = p1_data_writer.write(p1_data);
	hspConnector::get()->command("setweeklyscore", p1, NULL);
	
	Json::Value p;
	p["memberID"]=hspConnector::get()->getKakaoID();
	p["score"]=int(mySGD->getScore());
	p["stageNo"]=mySD->getSilType();
	hspConnector::get()->command("addStageScore",p,NULL);
	
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	if(selected_card_number > 0)
	{
		DecreaseCardDurabilityPopup* t_popup = DecreaseCardDurabilityPopup::create(NSDS_GI(kSDS_CI_int1_stage_i, selected_card_number), NSDS_GI(kSDS_CI_int1_grade_i, selected_card_number), this, callfunc_selector(FailPopup::endDecreaseCardDuration));
		addChild(t_popup, kZ_FP_popup);
		
		int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number);
		if(durability <= 0)
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
	}
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kZ_FP_gray);
	
	main_case = CCSprite::create("ending_back.png");
	main_case->setAnchorPoint(ccp(0,0));
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kZ_FP_back);
	
	top_case = CCSprite::create("test_ui_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f));//(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	addChild(top_case, kZ_FP_img);
	
	CountingBMLabel* total_gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "etc_font.fnt", 0.3f, "%d");
	total_gold_label->setPosition(ccp(225,top_case->getContentSize().height/2.f));
	top_case->addChild(total_gold_label);
	
	mySGD->setGoldLabel(total_gold_label);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(295,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time);
	
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySD->getSilType())->getCString(), mySGD->getFont().c_str(), 18);
	stage_label->setAnchorPoint(ccp(1.f,0.5f));
	stage_label->setPosition(ccp(330,253));
	main_case->addChild(stage_label, kZ_FP_img);
	
	FailCode fail_code = mySGD->fail_code;
	
	if(fail_code == kFC_gameover)
	{
		CCSprite* title = CCSprite::create("ending_gameover.png");
		title->setPosition(ccp(137,237));
		main_case->addChild(title, kZ_FP_img);
	}
	else if(fail_code == kFC_timeover)
	{
		CCSprite* title = CCSprite::create("ending_timeover.png");
		title->setPosition(ccp(137,237));
		main_case->addChild(title, kZ_FP_img);
	}
	else if(fail_code == kFC_missionfail)
	{
		CCSprite* title = CCSprite::create("ending_missionfail.png");
		title->setPosition(ccp(137,237));
		main_case->addChild(title, kZ_FP_img);
		
		//		CLEAR_CONDITION clr_cdt = mySD->getClearCondition();
		//
		//		CCSprite* fail_condition = CCSprite::create(CCString::createWithFormat("condition%d_menu.png", clr_cdt)->getCString());
		//		fail_condition->setPosition(ccp(240,170));
		//		addChild(fail_condition, kZ_FP_img);
		//
		//		if(clr_cdt == kCLEAR_subCumberCatch)
		//		{
		//			int catch_cnt = mySD->getClearConditionCatchSubCumber();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", catch_cnt)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_bigArea)
		//		{
		//			float clr_cdt_per = mySD->getClearConditionBigAreaPer();
		//			int clr_cdt_cnt = mySD->getClearConditionBigAreaCnt();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d", clr_cdt_per*100.f, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_itemCollect)
		//		{
		//			int collect_cnt = mySD->getClearConditionItemCollect();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", collect_cnt)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_perfect)
		//		{
		//			float perfect_per = mySD->getClearConditionPerfectBase();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f%%", perfect_per*100.f)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_sequenceChange)
		//		{
		//			CCSprite* c_coin = CCSprite::create("exchange_1_act.png");
		//			c_coin->setPosition(ccp(240,165));
		//			addChild(c_coin, kZ_FP_img);
		//		}
		//		else if(clr_cdt == kCLEAR_timeLimit)
		//		{
		//			int clr_time = mySD->getClearConditionTimeLimit();
		//
		//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", clr_time)->getCString(), mySGD->getFont().c_str(), 12);
		//			clr_cdt_label->setPosition(ccp(240,165));
		//			addChild(clr_cdt_label, kZ_FP_img);
		//		}
	}
	
	score_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	score_label->setAnchorPoint(ccp(0.5,0.5));
	score_label->setPosition(ccp(175,147));
	score_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(score_label, kZ_FP_img);
	
	gold_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	gold_label->setAnchorPoint(ccp(0.5,0.5));
	gold_label->setPosition(ccp(175,117));
	gold_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(gold_label, kZ_FP_img);
	
	time_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	time_label->setAnchorPoint(ccp(0.5,0.5));
	time_label->setPosition(ccp(175,88));
	time_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(time_label, kZ_FP_img);
	
	CCSprite* n_main = CCSprite::create("ending_main.png");
	CCSprite* s_main = CCSprite::create("ending_main.png");
	s_main->setColor(ccGRAY);
	
	CCMenuItem* main_item = CCMenuItemSprite::create(n_main, s_main, this, menu_selector(FailPopup::menuAction));
	main_item->setTag(kMT_FP_main);
	
	main_menu = CCMenu::createWithItem(main_item);
	main_menu->setVisible(false);
	main_menu->setPosition(ccp(330,33));
	main_case->addChild(main_menu, kZ_FP_menu);
	
	
	if(!mySGD->getIsMeChallenge())
	{
		CCSprite* n_replay = CCSprite::create("ending_replay.png");
		CCSprite* s_replay = CCSprite::create("ending_replay.png");
		s_replay->setColor(ccGRAY);
		
		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(FailPopup::menuAction));
		replay_item->setTag(kMT_FP_replay);
		
		replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setVisible(false);
		replay_menu->setPosition(ccp(150,33));
		main_case->addChild(replay_menu, kZ_FP_menu);
	}
	else
	{
		replay_menu = NULL;
	}
	
	
	//	CCSprite* n_help = CCSprite::create("ending_help.png");
	//	CCSprite* s_help = CCSprite::create("ending_help.png");
	//	s_help->setColor(ccGRAY);
	//
	//	CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailPopup::menuAction));
	//	help_item->setTag(kMT_FP_help);
	//
	//	help_menu = CCMenu::createWithItem(help_item);
	//	help_menu->setVisible(false);
	//	help_menu->setPosition(getContentPosition(kMT_FP_help));
	//	addChild(help_menu, kZ_FP_menu);
	
	
	is_saved_user_data = false;
	
	Json::Value param2;
	param2["memberID"] = hspConnector::get()->getKakaoID();
	
	Json::Value data;
	data[myDSH->getKey(kDSH_Key_savedStar)] = myDSH->getIntegerForKey(kDSH_Key_savedStar);
	data[myDSH->getKey(kDSH_Key_savedGold)] = myDSH->getIntegerForKey(kDSH_Key_savedGold);
	
	for(int i=kIC_attack;i<=kIC_randomChange;i++)
		data[myDSH->getKey(kDSH_Key_haveItemCnt_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, i);
	
	data[myDSH->getKey(kDSH_Key_cardTakeCnt)] = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
	int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
	for(int i=1;i<=card_take_cnt;i++)
	{
		int take_card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
		data[myDSH->getKey(kDSH_Key_takeCardNumber_int1)][i] = take_card_number;
		data[myDSH->getKey(kDSH_Key_hasGottenCard_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number);
		data[myDSH->getKey(kDSH_Key_cardDurability_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number);
		data[myDSH->getKey(kDSH_Key_inputTextCard_int1)][i] = myDSH->getStringForKey(kDSH_Key_inputTextCard_int1, take_card_number);
	}
	
	data[myDSH->getKey(kDSH_Key_allHighScore)] = myDSH->getIntegerForKey(kDSH_Key_allHighScore);
	data[myDSH->getKey(kDSH_Key_selectedCard)] = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	Json::FastWriter writer;
	param2["data"] = writer.write(data);
	hspConnector::get()->command("updateUserData", param2, json_selector(this, FailPopup::resultSavedUserData));
	
    return true;
}

void FailPopup::endDecreaseCardDuration()
{
	/////////////////123123123
}

void FailPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
	startCalcAnimation();
}

void FailPopup::showPopup()
{
	CCMoveTo* top_move = CCMoveTo::create(0.3f, ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	top_case->runAction(top_move);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(FailPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void FailPopup::endShowPopup()
{
	is_menu_enable = true;
}

void FailPopup::hidePopup()
{
	is_menu_enable = false;
	rankTableView->setTouchEnabled(false);
	
	CCMoveTo* top_move = CCMoveTo::create(0.3f, ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f));
	top_case->runAction(top_move);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(FailPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void FailPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void FailPopup::startCalcAnimation()
{
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
	startScoreAnimation();
}

void FailPopup::startScoreAnimation()
{
	keep_score = mySGD->getScore();
	decrease_score = keep_score;
	increase_score = 0.f;
	schedule(schedule_selector(FailPopup::scoreAnimation));
}

void FailPopup::scoreAnimation(float dt)
{
	if(decrease_score > 0)
	{
		int decreaseUnit = keep_score / 1.f * dt;
		
		if(decrease_score < decreaseUnit)
		{
			increase_score += decrease_score;
			decrease_score = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_score += decrease_score;
				decrease_score = 0;
			}
			else {
				decrease_score -= decreaseUnit;
				increase_score += decreaseUnit;
			}
		}
		score_label->setString(CCString::createWithFormat("%.0f",increase_score)->getCString());
	}
	else
		stopScoreAnimation();
}

void FailPopup::stopScoreAnimation()
{
	unschedule(schedule_selector(FailPopup::scoreAnimation));
	score_label->setString(CCString::createWithFormat("%.0f", mySGD->getScore())->getCString());
	startGoldAnimation();
}

void FailPopup::startGoldAnimation()
{
	keep_gold = mySGD->getStageGold();
	decrease_gold = keep_gold;
	increase_gold = 0.f;
	schedule(schedule_selector(FailPopup::goldAnimation));
}

void FailPopup::goldAnimation(float dt)
{
	if(decrease_gold > 0)
	{
		int decreaseUnit = keep_gold / 0.5f * dt;
		
		if(decrease_gold < decreaseUnit)
		{
			increase_gold += decrease_gold;
			decrease_gold = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_gold += decrease_gold;
				decrease_gold = 0;
			}
			else {
				decrease_gold -= decreaseUnit;
				increase_gold	+= decreaseUnit;
			}
		}
		gold_label->setString(CCString::createWithFormat("%.0f",increase_gold)->getCString());
	}
	else
		stopGoldAnimation();
}

void FailPopup::stopGoldAnimation()
{
	unschedule(schedule_selector(FailPopup::goldAnimation));
	gold_label->setString(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString());
	startTimeAnimation();
}

void FailPopup::startTimeAnimation()
{
	keep_time = mySGD->getGameTime();
	decrease_time = keep_time;
	increase_time = 0.f;
	schedule(schedule_selector(FailPopup::timeAnimation));
}

void FailPopup::timeAnimation(float dt)
{
	if(decrease_time > 0)
	{
		int decreaseUnit = keep_time / 0.5f * dt;
		
		if(decrease_time < decreaseUnit)
		{
			increase_time += decrease_time;
			decrease_time = 0;
		}
		else {
			if(decreaseUnit <= 0)
			{
				increase_time += decrease_time;
				decrease_time = 0;
			}
			else {
				decrease_time -= decreaseUnit;
				increase_time	+= decreaseUnit;
			}
		}
		time_label->setString(CCString::createWithFormat("%.0f",increase_time)->getCString());
	}
	else
		stopTimeAnimation();
}

void FailPopup::stopTimeAnimation()
{
	unschedule(schedule_selector(FailPopup::timeAnimation));
	time_label->setString(CCString::createWithFormat("%d", mySGD->getGameTime())->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
}

void FailPopup::resultSavedUserData(Json::Value result_data)
{
	if(result_data["state"] == "ok")
	{
		is_saved_user_data = true;
		endLoad();
		
		hspConnector::get()->kLoadFriends(json_selector(this, FailPopup::resultLoadFriends));
	}
	else
	{
		Json::Value param2;
		param2["memberID"] = hspConnector::get()->getKakaoID();
		
		Json::Value data;
		data[myDSH->getKey(kDSH_Key_savedStar)] = myDSH->getIntegerForKey(kDSH_Key_savedStar);
		data[myDSH->getKey(kDSH_Key_savedGold)] = myDSH->getIntegerForKey(kDSH_Key_savedGold);
		
		for(int i=kIC_attack;i<=kIC_randomChange;i++)
			data[myDSH->getKey(kDSH_Key_haveItemCnt_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, i);
		
		data[myDSH->getKey(kDSH_Key_cardTakeCnt)] = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=card_take_cnt;i++)
		{
			int take_card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			data[myDSH->getKey(kDSH_Key_takeCardNumber_int1)][i] = take_card_number;
			data[myDSH->getKey(kDSH_Key_hasGottenCard_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, take_card_number);
			data[myDSH->getKey(kDSH_Key_cardDurability_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number);
			data[myDSH->getKey(kDSH_Key_inputTextCard_int1)][i] = myDSH->getStringForKey(kDSH_Key_inputTextCard_int1, take_card_number);
		}
		
		data[myDSH->getKey(kDSH_Key_allHighScore)] = myDSH->getIntegerForKey(kDSH_Key_allHighScore);
		data[myDSH->getKey(kDSH_Key_selectedCard)] = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		
		Json::FastWriter writer;
		param2["data"] = writer.write(data);
		hspConnector::get()->command("updateUserData", param2, json_selector(this, FailPopup::resultSavedUserData));
	}
}

void FailPopup::resultLoadFriends(Json::Value result_data)
{
	CCLog("resultLoadFriends : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["status"].asInt() == 0)
	{
		Json::Value appfriends = result_data["app_friends_info"];
		appfriends.append(hspConnector::get()->myKakaoInfo);
		
		Json::Value p;
		for(int i=0; i<appfriends.size();i++)
		{
			FailFriendRank t_friend_info;
			t_friend_info.nickname = appfriends[i]["nickname"].asString().c_str();
			t_friend_info.img_url = appfriends[i]["profile_image_url"].asString().c_str();
			t_friend_info.user_id = appfriends[i]["user_id"].asString().c_str();
			t_friend_info.score = 0;
			t_friend_info.is_play = false;
			t_friend_info.is_message_blocked = appfriends[i]["message_blocked"].asBool();
			friend_list.push_back(t_friend_info);
			
			p["memberIDList"].append(appfriends[i]["user_id"].asString());
		}
		
		p["stageNo"]=mySD->getSilType();
		hspConnector::get()->command("getstagescorelist",p,json_selector(this, FailPopup::resultGetStageScoreList));
	}
	else
	{
		is_loaded_list = true;
		endLoad();
	}
}

void FailPopup::resultGetStageScoreList(Json::Value result_data)
{
	CCLog("resultGetStageScoreList : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["state"].asString() == "ok")
	{
		Json::Value score_list = result_data["list"];
		for(int i=0;i<score_list.size();i++)
		{
			vector<FailFriendRank>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString().c_str());
			if(iter != friend_list.end())
			{
				(*iter).score = score_list[i]["score"].asFloat();
				(*iter).is_play = true;
			}
			else
				CCLog("not found friend memberID");
		}
		
		auto beginIter = std::remove_if(friend_list.begin(), friend_list.end(), [=](FailFriendRank t_info)
										{
											return !t_info.is_play;
										});
		friend_list.erase(beginIter, friend_list.end());
		
		struct t_FriendSort{
			bool operator() (const FailFriendRank& a, const FailFriendRank& b)
			{
				return a.score > b.score;
			}
		} pred;
		
		sort(friend_list.begin(), friend_list.end(), pred);
		
		// create cell
		
		//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 195, 176));
		//		temp_back->setAnchorPoint(CCPointZero);
		//		temp_back->setOpacity(100);
		//		temp_back->setPosition(ccp(246, 65));
		//		addChild(temp_back, kZ_CS_menu);
		
		rankTableView = CCTableView::create(this, CCSizeMake(195, 176));
		
		rankTableView->setAnchorPoint(CCPointZero);
		rankTableView->setDirection(kCCScrollViewDirectionVertical);
		rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		rankTableView->setPosition(ccp(246, 65));
		
		rankTableView->setDelegate(this);
		main_case->addChild(rankTableView, kZ_FP_menu);
		rankTableView->setTouchPriority(kCCMenuHandlerPriority+1);
		
		//		int myPosition = rankTableView->minContainerOffset().y;
		//		for(int i=0; i<friend_list.size(); i++)
		//		{
		//			if(friend_list[i].user_id == hspConnector::get()->getKakaoID())
		//			{
		//				myPosition = friend_list.size() - i - 1;
		//				break;
		//			}
		//		}
		//		float yInitPosition = MAX(rankTableView->minContainerOffset().y, -cellSizeForTable(rankTableView).height*myPosition + rankTableView->getViewSize().height / 2.f);
		//		yInitPosition = MIN(0, yInitPosition);
		//		rankTableView->setContentOffsetInDuration(ccp(0, yInitPosition), 0.7f);
	}
	is_loaded_list = true;
	endLoad();
}

void FailPopup::endLoad()
{
	if(is_saved_user_data && is_loaded_list)
	{
		main_menu->setVisible(true);
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) > 0)
		{
			if(!mySGD->getIsMeChallenge())
				replay_menu->setVisible(true);
		}
	}
}

CCPoint FailPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kMT_FP_main)			return_value = ccp(100,70);
	else if(t_tag == kMT_FP_replay)		return_value = ccp(240,70);
	//	else if(t_tag == kMT_FP_help)		return_value = ccp(380,70);
	
	return return_value;
}

void FailPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	int tag = ((CCNode*)pSender)->getTag();
	
	is_menu_enable = false;
	
	if(tag == kMT_FP_main)
	{
		if(mySGD->getIsMeChallenge())
			mySGD->setIsMeChallenge(false);
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		mySGD->resetLabels();
		hidePopup();
	}
	else if(tag == kMT_FP_replay)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		mySGD->resetLabels();
		
		StageSettingPopup* t_popup = StageSettingPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kPMS_Z_popup);
		
		target_final = NULL;
		hidePopup();
	}
}

void FailPopup::popupClose()
{
	is_menu_enable = true;
}

void FailPopup::closeReplayPopup()
{
	is_menu_enable = true;
}

void FailPopup::cellAction( CCObject* sender )
{
	int tag = ((CCNode*)sender)->getTag();
	tag -= kFFC_T_menuBase;

	CCLog("help memberID : %s", friend_list[tag].user_id.c_str());
}

CCTableViewCell* FailPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCLabelTTF* nickname_label;
	CCLabelTTF* score_label;
	CCLabelTTF* rank_label;
	FailFriendRank* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();

	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kFFC_T_img);
	profileImg->setPosition(ccp(27, 20));
	profileImg->setScale(33.f / profileImg->getContentSize().width);
	cell->addChild(profileImg, kFFC_Z_img);

	CCSprite* bg = CCSprite::create("ending_cell_back.png");
	bg->setPosition(CCPointZero);
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,kFFC_Z_case);

	nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
	nickname_label->setPosition(ccp(47,22));
	nickname_label->setAnchorPoint(CCPointZero);
	nickname_label->setTag(kFFC_T_nickname);
	cell->addChild(nickname_label,kFFC_Z_img);

	score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 18);
	score_label->setPosition(ccp(47,0));
	score_label->setAnchorPoint(CCPointZero);
	score_label->setTag(kFFC_T_score);
	cell->addChild(score_label,kFFC_Z_img);

	rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 10);
	rank_label->setPosition(ccp(12,3));
	rank_label->setAnchorPoint(CCPointZero);
	rank_label->setTag(kFFC_T_rank);
	cell->addChild(rank_label,kFFC_Z_img);

	if((*member).user_id == hspConnector::get()->getKakaoID())
	{
		CCSprite* meBack = CCSprite::create("ending_cell_selected.png");
		meBack->setPosition(ccp(meBack->getContentSize().width - bg->getContentSize().width, 0));
		meBack->setAnchorPoint(CCPointZero);
		cell->addChild(meBack,kFFC_Z_case);
	}
	else
	{
		if(!mySGD->getIsMeChallenge())
		{
			if(!(*member).is_message_blocked)
			{
				CCSprite* n_help = CCSprite::create("ending_help_on.png");
				CCSprite* s_help = CCSprite::create("ending_help_on.png");
				s_help->setColor(ccGRAY);
				
				CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailPopup::cellAction));
				help_item->setTag(kFFC_T_menuBase + idx);
				
				CCMenu* help_menu = CCMenu::createWithItem(help_item);
				help_menu->setPosition(ccp(165,21));
				cell->addChild(help_menu, kFFC_Z_img);
			}
			else
			{
				CCSprite* not_help = CCSprite::create("ending_help_off.png");
				not_help->setPosition(ccp(165,21));
				cell->addChild(not_help, kFFC_Z_img);
			}
		}
	}

	return cell;
}

void FailPopup::scrollViewDidScroll( CCScrollView* view )
{

}

void FailPopup::scrollViewDidZoom( CCScrollView* view )
{

}

void FailPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	// 영호
	//		CCLog("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize FailPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(195, 45);
}

unsigned int FailPopup::numberOfCellsInTableView( CCTableView *table )
{
	return friend_list.size();
}
