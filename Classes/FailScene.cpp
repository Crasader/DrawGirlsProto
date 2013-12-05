//
//  FailScene.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#include "FailScene.h"
//#include "StartingScene.h"
//#include "WorldMapScene.h"
#include "PuzzleMapScene.h"
#include "ScreenSide.h"
#include "StarGoldData.h"
#include "EnumDefine.h"
//#include "StageSettingPopup.h"
#include "StageSettingScene.h"
#include "FailHelpPopup.h"
#include "LogData.h"
#include "CountingBMLabel.h"
#include "DecreaseCardDurabilityPopup.h"

typedef enum tMenuTagFailScene{
	kMT_FS_main = 1,
	kMT_FS_replay
//	kMT_FS_help
}MenuTagFailScene;

typedef enum tZorderFailScene{
	kZ_FS_back = 1,
	kZ_FS_img,
	kZ_FS_menu,
	kZ_FS_popup
}ZorderFailScene;

CCScene* FailScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    FailScene *layer = FailScene::create();
	
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool FailScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
	is_loaded_list = false;
	
	setKeypadEnabled(true);
	
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
		DecreaseCardDurabilityPopup* t_popup = DecreaseCardDurabilityPopup::create(NSDS_GI(kSDS_CI_int1_stage_i, selected_card_number), NSDS_GI(kSDS_CI_int1_grade_i, selected_card_number), this, callfunc_selector(FailScene::endCall));
		addChild(t_popup, kZ_FS_popup);
		
		int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number);
		if(durability <= 0)
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
	}
	
	CCSprite* fail_back = CCSprite::create("ending_back.png");
	fail_back->setPosition(ccp(240,160));
	addChild(fail_back, kZ_FS_back);
	
	CCSprite* top_case = CCSprite::create("test_ui_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,320.f));//(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	addChild(top_case, kZ_FS_img);
	
	CountingBMLabel* ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "etc_font.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(94,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CountingBMLabel* total_gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "etc_font.fnt", 0.3f, "%d");
	total_gold_label->setPosition(ccp(185,top_case->getContentSize().height/2.f));
	top_case->addChild(total_gold_label);
	
	mySGD->setGoldLabel(total_gold_label);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(250,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time);
	
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySD->getSilType())->getCString(), mySGD->getFont().c_str(), 18);
	stage_label->setAnchorPoint(ccp(1.f,0.5f));
	stage_label->setPosition(ccp(330,253));
	addChild(stage_label, kZ_FS_img);
	
	FailCode fail_code = mySGD->fail_code;
	
	if(fail_code == kFC_gameover)
	{
		CCSprite* title = CCSprite::create("ending_gameover.png");
		title->setPosition(ccp(137,237));
		addChild(title, kZ_FS_img);
	}
	else if(fail_code == kFC_timeover)
	{
		CCSprite* title = CCSprite::create("ending_timeover.png");
		title->setPosition(ccp(137,237));
		addChild(title, kZ_FS_img);
	}
	else if(fail_code == kFC_missionfail)
	{
		CCSprite* title = CCSprite::create("ending_missionfail.png");
		title->setPosition(ccp(137,237));
		addChild(title, kZ_FS_img);
		
//		CLEAR_CONDITION clr_cdt = mySD->getClearCondition();
//		
//		CCSprite* fail_condition = CCSprite::create(CCString::createWithFormat("condition%d_menu.png", clr_cdt)->getCString());
//		fail_condition->setPosition(ccp(240,170));
//		addChild(fail_condition, kZ_FS_img);
//		
//		if(clr_cdt == kCLEAR_subCumberCatch)
//		{
//			int catch_cnt = mySD->getClearConditionCatchSubCumber();
//			
//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", catch_cnt)->getCString(), mySGD->getFont().c_str(), 12);
//			clr_cdt_label->setPosition(ccp(240,165));
//			addChild(clr_cdt_label, kZ_FS_img);
//		}
//		else if(clr_cdt == kCLEAR_bigArea)
//		{
//			float clr_cdt_per = mySD->getClearConditionBigAreaPer();
//			int clr_cdt_cnt = mySD->getClearConditionBigAreaCnt();
//			
//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d", clr_cdt_per*100.f, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
//			clr_cdt_label->setPosition(ccp(240,165));
//			addChild(clr_cdt_label, kZ_FS_img);
//		}
//		else if(clr_cdt == kCLEAR_itemCollect)
//		{
//			int collect_cnt = mySD->getClearConditionItemCollect();
//			
//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", collect_cnt)->getCString(), mySGD->getFont().c_str(), 12);
//			clr_cdt_label->setPosition(ccp(240,165));
//			addChild(clr_cdt_label, kZ_FS_img);
//		}
//		else if(clr_cdt == kCLEAR_perfect)
//		{
//			float perfect_per = mySD->getClearConditionPerfectBase();
//			
//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f%%", perfect_per*100.f)->getCString(), mySGD->getFont().c_str(), 12);
//			clr_cdt_label->setPosition(ccp(240,165));
//			addChild(clr_cdt_label, kZ_FS_img);
//		}
//		else if(clr_cdt == kCLEAR_sequenceChange)
//		{
//			CCSprite* c_coin = CCSprite::create("exchange_1_act.png");
//			c_coin->setPosition(ccp(240,165));
//			addChild(c_coin, kZ_FS_img);
//		}
//		else if(clr_cdt == kCLEAR_timeLimit)
//		{
//			int clr_time = mySD->getClearConditionTimeLimit();
//			
//			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", clr_time)->getCString(), mySGD->getFont().c_str(), 12);
//			clr_cdt_label->setPosition(ccp(240,165));
//			addChild(clr_cdt_label, kZ_FS_img);
//		}
	}
	
	score_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	score_label->setAnchorPoint(ccp(0.5,0.5));
	score_label->setPosition(ccp(175,147));
	score_label->setAlignment(kCCTextAlignmentRight);
	addChild(score_label, kZ_FS_img);
	
	gold_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	gold_label->setAnchorPoint(ccp(0.5,0.5));
	gold_label->setPosition(ccp(175,117));
	gold_label->setAlignment(kCCTextAlignmentRight);
	addChild(gold_label, kZ_FS_img);
	
	time_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	time_label->setAnchorPoint(ccp(0.5,0.5));
	time_label->setPosition(ccp(175,88));
	time_label->setAlignment(kCCTextAlignmentRight);
	addChild(time_label, kZ_FS_img);
	
	CCSprite* n_main = CCSprite::create("ending_main.png");
	CCSprite* s_main = CCSprite::create("ending_main.png");
	s_main->setColor(ccGRAY);
	
	CCMenuItem* main_item = CCMenuItemSprite::create(n_main, s_main, this, menu_selector(FailScene::menuAction));
	main_item->setTag(kMT_FS_main);
	
	main_menu = CCMenu::createWithItem(main_item);
	main_menu->setVisible(false);
	main_menu->setPosition(ccp(330,33));
	addChild(main_menu, kZ_FS_menu);
	
	
	CCSprite* n_replay = CCSprite::create("ending_replay.png");
	CCSprite* s_replay = CCSprite::create("ending_replay.png");
	s_replay->setColor(ccGRAY);
	
	CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(FailScene::menuAction));
	replay_item->setTag(kMT_FS_replay);
	
	replay_menu = CCMenu::createWithItem(replay_item);
	replay_menu->setVisible(false);
	replay_menu->setPosition(ccp(150,33));
	addChild(replay_menu, kZ_FS_menu);
	
	
//	CCSprite* n_help = CCSprite::create("ending_help.png");
//	CCSprite* s_help = CCSprite::create("ending_help.png");
//	s_help->setColor(ccGRAY);
//	
//	CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailScene::menuAction));
//	help_item->setTag(kMT_FS_help);
//	
//	help_menu = CCMenu::createWithItem(help_item);
//	help_menu->setVisible(false);
//	help_menu->setPosition(getContentPosition(kMT_FS_help));
//	addChild(help_menu, kZ_FS_menu);
	
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
	
	is_saved_user_data = false;
	
	myDSH->saveAllUserData(json_selector(this, FailScene::resultSavedUserData));
	
    return true;
}

void FailScene::endCall()
{
	
}

void FailScene::onEnter()
{
	CCLayer::onEnter();
	
	startCalcAnimation();
}

void FailScene::startCalcAnimation()
{
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
	startScoreAnimation();
}

void FailScene::startScoreAnimation()
{
	keep_score = mySGD->getScore();
	decrease_score = keep_score;
	increase_score = 0.f;
	schedule(schedule_selector(FailScene::scoreAnimation));
}

void FailScene::scoreAnimation(float dt)
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

void FailScene::stopScoreAnimation()
{
	unschedule(schedule_selector(FailScene::scoreAnimation));
	score_label->setString(CCString::createWithFormat("%.0f", mySGD->getScore())->getCString());
	startGoldAnimation();
}

void FailScene::startGoldAnimation()
{
	keep_gold = mySGD->getStageGold();
	decrease_gold = keep_gold;
	increase_gold = 0.f;
	schedule(schedule_selector(FailScene::goldAnimation));
}

void FailScene::goldAnimation(float dt)
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

void FailScene::stopGoldAnimation()
{
	unschedule(schedule_selector(FailScene::goldAnimation));
	gold_label->setString(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString());
	startTimeAnimation();
}

void FailScene::startTimeAnimation()
{
	keep_time = mySGD->getGameTime();
	decrease_time = keep_time;
	increase_time = 0.f;
	schedule(schedule_selector(FailScene::timeAnimation));
}

void FailScene::timeAnimation(float dt)
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

void FailScene::stopTimeAnimation()
{
	unschedule(schedule_selector(FailScene::timeAnimation));
	time_label->setString(CCString::createWithFormat("%d", mySGD->getGameTime())->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
}

void FailScene::resultSavedUserData(Json::Value result_data)
{
	if(result_data["state"] == "ok")
	{
		is_saved_user_data = true;
		endLoad();
		
		hspConnector::get()->kLoadFriends(json_selector(this, FailScene::resultLoadFriends));
	}
	else
	{
		myDSH->saveAllUserData(json_selector(this, FailScene::resultSavedUserData));
	}
}

void FailScene::resultLoadFriends(Json::Value result_data)
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
		hspConnector::get()->command("getstagescorelist",p,json_selector(this, FailScene::resultGetStageScoreList));
	}
	else
	{
		is_loaded_list = true;
		endLoad();
	}
}

void FailScene::resultGetStageScoreList(Json::Value result_data)
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
		addChild(rankTableView, kZ_FS_menu);
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

void FailScene::endLoad()
{
	if(is_saved_user_data && is_loaded_list)
	{
		main_menu->setVisible(true);
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) > 0)
			replay_menu->setVisible(true);
	}
}

CCPoint FailScene::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kMT_FS_main)			return_value = ccp(100,70);
	else if(t_tag == kMT_FS_replay)		return_value = ccp(240,70);
//	else if(t_tag == kMT_FS_help)		return_value = ccp(380,70);
	
	return return_value;
}

void FailScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kMT_FS_main)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		mySGD->resetLabels();
		is_menu_enable = false;
//		CCDirector::sharedDirector()->replaceScene(StartingScene::scene());
		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
	}
	else if(tag == kMT_FS_replay)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		mySGD->resetLabels();
		is_menu_enable = false;
//		myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)-1);
//		StageSettingPopupLayer* t_sspl = StageSettingPopupLayer::create(mySD->getSilType(), this, callfunc_selector(FailScene::closeReplayPopup));
//		addChild(t_sspl, kZ_FS_popup);
		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
//	else if(tag == kMT_FS_help)
//	{
//		is_menu_enable = false;
//		FailHelpPopup* t_fhp = FailHelpPopup::create(this, callfunc_selector(FailScene::popupClose));
//		addChild(t_fhp, kZ_FS_popup);
//	}
}

void FailScene::popupClose()
{
	is_menu_enable = true;
}

void FailScene::closeReplayPopup()
{
	is_menu_enable = true;
}

void FailScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void FailScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(FailScene::alertAction));
}

void FailScene::cellAction( CCObject* sender )
{
	int tag = ((CCNode*)sender)->getTag();
	tag -= kFFC_T_menuBase;

	CCLog("help memberID : %s", friend_list[tag].user_id.c_str());
}

CCTableViewCell* FailScene::tableCellAtIndex( CCTableView *table, unsigned int idx )
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
		if(!(*member).is_message_blocked)
		{
			CCSprite* n_help = CCSprite::create("ending_help_on.png");
			CCSprite* s_help = CCSprite::create("ending_help_on.png");
			s_help->setColor(ccGRAY);

			CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailScene::cellAction));
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

	return cell;
}

void FailScene::scrollViewDidScroll( CCScrollView* view )
{

}

void FailScene::scrollViewDidZoom( CCScrollView* view )
{

}

void FailScene::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	//		CCLog("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize FailScene::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(195, 45);
}

unsigned int FailScene::numberOfCellsInTableView( CCTableView *table )
{
	return friend_list.size();
}
