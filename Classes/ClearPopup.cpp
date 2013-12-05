//
//  ClearPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 19..
//
//

#include "ClearPopup.h"

#include "StageImgLoader.h"
#include "StarGoldData.h"
#include "SilhouetteData.h"
#include "LogData.h"
#include "CardCase.h"
#include "CountingBMLabel.h"
#include "TakeCardPopup.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "HeartTime.h"
#include "DownloadFile.h"
#include "StageSettingPopup.h"
#include "PuzzleMapScene.h"
#include "ShopPopup.h"
#include "ChallengeSend.h"
#include "KSAlertView.h"
#include "HelpResultSend.h"
#include "UnknownFriends.h"


typedef enum tMenuTagClearPopup{
	kMT_CP_ok = 1,
	kMT_CP_replay,
	kMT_CP_noti,
	kMT_CP_rubyShop,
	kMT_CP_goldShop,
	kMT_CP_heartShop,
	kMT_CP_friendPoint,
	kMT_CP_friendPointClose,
	kMT_CP_heartTime
}MenuTagClearPopup;

typedef enum tZorderClearPopup{
	kZ_CP_gray = 1,
	kZ_CP_back,
	kZ_CP_img,
	kZ_CP_menu,
	kZ_CP_popup
}ZorderClearPopup;

void ClearPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool ClearPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	is_menu_enable = false;
	is_loaded_list = false;
	
	friend_point_popup = NULL;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kZ_CP_gray);
	
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	
	Json::Value param;
	param["key"] = CCSTR_CWF("stage_clear_%d", mySD->getSilType())->getCString();
	
	hspConnector::get()->command("increaseStats", param, NULL);
	
	if(mySGD->getScore() > myDSH->getIntegerForKey(kDSH_Key_allHighScore))
	{
		myDSH->setIntegerForKey(kDSH_Key_allHighScore, int(mySGD->getScore()));
	}
	
	Json::Value p1;
	p1["memberID"] = hspConnector::get()->getKakaoID();
	p1["score"]=int(mySGD->getScore());
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
    
	main_case = CCSprite::create("ending_back.png");
	main_case->setAnchorPoint(ccp(0,0));
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kZ_CP_back);
	
	top_case = CCSprite::create("test_ui_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f));//(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	addChild(top_case, kZ_CP_img);
	
	CountingBMLabel* ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "etc_font.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(94,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CountingBMLabel* total_gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "etc_font.fnt", 0.3f, "%d");
	total_gold_label->setPosition(ccp(185,top_case->getContentSize().height/2.f));
	top_case->addChild(total_gold_label);
	
	mySGD->setGoldLabel(total_gold_label);
	
	CountingBMLabel* friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "etc_font.fnt", 0.3f, "%d");
	friend_point_label->setPosition(ccp(427,top_case->getContentSize().height/2.f));
	top_case->addChild(friend_point_label);
	
	mySGD->setFriendPointLabel(friend_point_label);

	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(250,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time, 0, kMT_CP_heartTime);
	
	
	CCSprite* n_ruby = CCSprite::create("test_ui_shop.png");
	CCSprite* s_ruby = CCSprite::create("test_ui_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(ClearPopup::menuAction));
	ruby_item->setTag(kMT_CP_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(124,top_case->getContentSize().height/2.f-2));
	top_case->addChild(ruby_menu);
	
	CCSprite* n_gold = CCSprite::create("test_ui_shop.png");
	CCSprite* s_gold = CCSprite::create("test_ui_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(ClearPopup::menuAction));
	gold_item->setTag(kMT_CP_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(220,top_case->getContentSize().height/2.f-2));
	top_case->addChild(gold_menu);
	
	CCSprite* n_heart = CCSprite::create("test_ui_shop.png");
	CCSprite* s_heart = CCSprite::create("test_ui_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(ClearPopup::menuAction));
	heart_item->setTag(kMT_CP_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(369,top_case->getContentSize().height/2.f-2));
	top_case->addChild(heart_menu);
	
	CCSprite* n_friend_point = CCSprite::create("test_ui_shop.png");
	CCSprite* s_friend_point = CCSprite::create("test_ui_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(ClearPopup::menuAction));
	friend_point_item->setTag(kMT_CP_friendPoint);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(460,top_case->getContentSize().height/2.f-2));
	top_case->addChild(friend_point_menu);
	
	
	CCSprite* title = CCSprite::create("ending_clear.png");
	title->setPosition(ccp(137,237));
	main_case->addChild(title, kZ_CP_img);
	
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySD->getSilType())->getCString(), mySGD->getFont().c_str(), 18);
	stage_label->setAnchorPoint(ccp(1.f,0.5f));
	stage_label->setPosition(ccp(330,253));
	main_case->addChild(stage_label, kZ_CP_img);
	
	
	///////////////////////////// 딤드 로 팝업 띄움
	int stage_number = mySD->getSilType();
	int take_level;
	if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 3;
	else if(mySGD->is_exchanged || mySGD->is_showtime)	take_level = 2;
	else												take_level = 1;
	
	TakeCardPopup* t_popup = TakeCardPopup::create(stage_number, take_level, this, callfunc_selector(ClearPopup::endTakeCard));
	addChild(t_popup, kZ_CP_popup);
	/////////////////////////////////////////////
	
	
	int start_stage_number = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
	
	int minimum_count = 0;
	int maximum_count = 0;
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		bool is_found = false;
		for(int j=3;j>=1 && !is_found;j--)
		{
			int check_card_number = NSDS_GI(i, kSDS_SI_level_int1_card_i, j);
			int check_card_durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, check_card_number);
			
			if(check_card_durability > 0)
			{
				is_found = true;
				if(j == 3)
					maximum_count++;
				minimum_count++;
			}
		}
	}
	
	myLog->addLog(kLOG_puzzleAchievementMinimum_i, -1, 100*minimum_count/stage_count);
	myLog->addLog(kLOG_puzzleAchievementMaximum_i, -1, 100*maximum_count/stage_count);
	
	
	myLog->sendLog(CCString::createWithFormat("clear_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
	
	
	for(int i=0;i<take_level;i++)
	{
		CCSprite* star = CCSprite::create("ending_star.png");
		star->setAnchorPoint(ccp(0.5,0));
		if(i == 0)
			star->setPosition(ccp(135,174));
		else if(i == 1)
		{
			star->setScale(0.8f);
			star->setPosition(ccp(92,174));
		}
		else if(i == 2)
		{
			star->setScale(0.8f);
			star->setPosition(ccp(176,174));
		}
		
		main_case->addChild(star, kZ_CP_img);
	}
	
	score_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	score_label->setAnchorPoint(ccp(0.5,0.5));
	score_label->setPosition(ccp(175,147));
	score_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(score_label, kZ_CP_img);
	
	gold_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	gold_label->setAnchorPoint(ccp(0.5,0.5));
	gold_label->setPosition(ccp(175,117));
	gold_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(gold_label, kZ_CP_img);
	
	time_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	time_label->setAnchorPoint(ccp(0.5,0.5));
	time_label->setPosition(ccp(175,88));
	time_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(time_label, kZ_CP_img);
	
	
	CCSprite* n_ok = CCSprite::create("ending_main.png");
	CCSprite* s_ok = CCSprite::create("ending_main.png");
	s_ok->setColor(ccGRAY);
	
	CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(ClearPopup::menuAction));
	ok_item->setTag(kMT_CP_ok);
	
	ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setVisible(false);
	ok_menu->setPosition(ccp(330,33));
	main_case->addChild(ok_menu, kZ_CP_menu);
	
	
	if(!mySGD->getIsMeChallenge() && !mySGD->getIsAcceptChallenge() && !mySGD->getIsAcceptHelp())
	{
		CCSprite* n_replay = CCSprite::create("ending_replay.png");
		CCSprite* s_replay = CCSprite::create("ending_replay.png");
		s_replay->setColor(ccGRAY);
		
		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(ClearPopup::menuAction));
		replay_item->setTag(kMT_CP_replay);
		
		replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setVisible(false);
		replay_menu->setPosition(ccp(150,33));
		main_case->addChild(replay_menu, kZ_CP_menu);
	}
	else
	{
		replay_menu = NULL;
	}
	
	
	is_saved_user_data = false;
	
	myDSH->saveAllUserData(json_selector(this, ClearPopup::resultSavedUserData));
	
    return true;
}

void ClearPopup::showPopup()
{
	top_case->setPosition(ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
//	CCMoveTo* top_move = CCMoveTo::create(0.3f, ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
//	top_case->runAction(top_move);
	
	gray->setOpacity(255);
//	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
//	gray->runAction(gray_fade);

	main_case->setPosition(ccp(0,0));
	endShowPopup();
//	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
//	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::endShowPopup));
//	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
//	main_case->runAction(main_seq);
}

void ClearPopup::endShowPopup()
{
	is_menu_enable = true;
}

void ClearPopup::hidePopup()
{
	is_menu_enable = false;
	rankTableView->setTouchEnabled(false);
	
	CCMoveTo* top_move = CCMoveTo::create(0.3f, ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f + 33.f));
	top_case->runAction(top_move);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void ClearPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void ClearPopup::endTakeCard()
{
	if(mySGD->getIsMeChallenge())
	{
		// ksks
		addChild(ChallengeSend::create(mySGD->getMeChallengeTarget(), mySGD->getMeChallengeTargetNick(), mySGD->getScore(),
																	 ChallengeCategory::kRequest),
						 kZ_CP_popup);
//		getMeChallengeTarget
	}
	
	if(mySGD->getIsAcceptChallenge())
	{
		/////////////////// ksks
//		mySGD->getAcceptChallengeId(), mySGD->getAcceptChallengeNick(), mySGD->getAcceptChallengeScore();
		addChild(ChallengeSend::create(mySGD->getMeChallengeTarget(), mySGD->getMeChallengeTargetNick(), mySGD->getScore(),
																	 ChallengeCategory::kRequestReply),
						 kZ_CP_popup);
	}
	
	if(mySGD->getIsAcceptHelp())
	{
		////////////////// ksks
		CCLog("zzzz");
		addChild(HelpResultSend::create(mySGD->getAcceptHelpId(), true), kZ_CP_popup);


		
	}
	
}

void ClearPopup::resultLoadFriends(Json::Value result_data)
{
	CCLog("resultLoadFriends : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["status"].asInt() == 0)
	{
		Json::Value appfriends = result_data["app_friends_info"];
		appfriends.append(hspConnector::get()->myKakaoInfo);
		
		Json::Value p;
		for(int i=0; i<appfriends.size();i++)
		{
			ClearFriendRank t_friend_info;
			t_friend_info.nickname = appfriends[i]["nickname"].asString().c_str();
			t_friend_info.img_url = appfriends[i]["profile_image_url"].asString().c_str();
			t_friend_info.user_id = appfriends[i]["user_id"].asString().c_str();
			t_friend_info.score = 0;
			t_friend_info.is_play = false;
			friend_list.push_back(t_friend_info);
			
			p["memberIDList"].append(appfriends[i]["user_id"].asString());
		}
		for(auto i : UnknownFriends::getInstance()->getFriends())
		{
			ClearFriendRank fInfo;
			fInfo.nickname = i.nick + "[unknown]";
			fInfo.img_url = "";
			fInfo.user_id = i.userId;
			fInfo.score = 0;
			fInfo.is_play = false;
			friend_list.push_back(fInfo);
			
			p["memberIDList"].append(i.userId);
		}
		
		
		p["stageNo"]=mySD->getSilType();
		hspConnector::get()->command("getstagescorelist",p,json_selector(this, ClearPopup::resultGetStageScoreList));
	}
	else
	{
		is_loaded_list = true;
		endLoad();
	}
}

void ClearPopup::resultGetStageScoreList(Json::Value result_data)
{
	CCLog("resultGetStageScoreList : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["state"].asString() == "ok")
	{
		Json::Value score_list = result_data["list"];
		for(int i=0;i<score_list.size();i++)
		{
			vector<ClearFriendRank>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString().c_str());
			if(iter != friend_list.end())
			{
				(*iter).score = score_list[i]["score"].asFloat();
				(*iter).is_play = true;
			}
			else
				CCLog("not found friend memberID");
		}
		
		auto beginIter = std::remove_if(friend_list.begin(), friend_list.end(), [=](ClearFriendRank t_info)
										{
											return !t_info.is_play;
										});
		friend_list.erase(beginIter, friend_list.end());
		
		struct t_FriendSort{
			bool operator() (const ClearFriendRank& a, const ClearFriendRank& b)
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
		//		addChild(temp_back, kZ_CP_menu);
		
		rankTableView = CCTableView::create(this, CCSizeMake(195, 176));
		
		rankTableView->setAnchorPoint(CCPointZero);
		rankTableView->setDirection(kCCScrollViewDirectionVertical);
		rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		rankTableView->setPosition(ccp(246, 65));
		
		rankTableView->setDelegate(this);
		main_case->addChild(rankTableView, kZ_CP_menu);
		rankTableView->setTouchPriority(-200);
		
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

void ClearPopup::resultSavedUserData(Json::Value result_data)
{
	if(result_data["state"] == "ok")
	{
		is_saved_user_data = true;
		endLoad();
		
		hspConnector::get()->kLoadFriends(json_selector(this, ClearPopup::resultLoadFriends));
	}
	else
	{
		myDSH->saveAllUserData(json_selector(this, ClearPopup::resultSavedUserData));
	}
}

void ClearPopup::endLoad()
{
	if(is_saved_user_data && is_loaded_list)
	{
		ok_menu->setVisible(true);
		if(replay_menu)
			replay_menu->setVisible(true);
	}
}

void ClearPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
	startCalcAnimation();
}

void ClearPopup::startCalcAnimation()
{
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
	startScoreAnimation();
}

void ClearPopup::startScoreAnimation()
{
	keep_score = mySGD->getScore();
	decrease_score = keep_score;
	increase_score = 0.f;
	schedule(schedule_selector(ClearPopup::scoreAnimation));
}

void ClearPopup::scoreAnimation(float dt)
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

void ClearPopup::stopScoreAnimation()
{
	unschedule(schedule_selector(ClearPopup::scoreAnimation));
	score_label->setString(CCString::createWithFormat("%.0f", mySGD->getScore())->getCString());
	startGoldAnimation();
}

void ClearPopup::startGoldAnimation()
{
	keep_gold = mySGD->getStageGold();
	decrease_gold = keep_gold;
	increase_gold = 0.f;
	schedule(schedule_selector(ClearPopup::goldAnimation));
}

void ClearPopup::goldAnimation(float dt)
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

void ClearPopup::stopGoldAnimation()
{
	unschedule(schedule_selector(ClearPopup::goldAnimation));
	gold_label->setString(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString());
	startTimeAnimation();
}

void ClearPopup::startTimeAnimation()
{
	keep_time = mySGD->getGameTime();
	decrease_time = keep_time;
	increase_time = 0.f;
	schedule(schedule_selector(ClearPopup::timeAnimation));
}

void ClearPopup::timeAnimation(float dt)
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

void ClearPopup::stopTimeAnimation()
{
	unschedule(schedule_selector(ClearPopup::timeAnimation));
	time_label->setString(CCString::createWithFormat("%d", mySGD->getGameTime())->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
}

void ClearPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	is_menu_enable = false;
	AudioEngine::sharedInstance()->stopAllEffects();
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kMT_CP_ok)
	{
		mySGD->setIsMeChallenge(false);
		mySGD->setIsAcceptChallenge(false);
		mySGD->setIsAcceptHelp(false);
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		mySGD->resetLabels();
		hidePopup();
	}
	else if(tag == kMT_CP_replay)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		mySGD->resetLabels();
		
		StageSettingPopup* t_popup = StageSettingPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kPMS_Z_popup);
		
		target_final = NULL;
		hidePopup();
//		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
	else if(tag == kMT_CP_rubyShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(NULL, NULL);
		t_shop->targetHeartTime((HeartTime*)(top_case->getChildByTag(kMT_CP_heartTime)));
		t_shop->setShopCode(kSC_ruby);
		addChild(t_shop, kZ_CP_popup);
		is_menu_enable = true;
	}
	else if(tag == kMT_CP_goldShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(NULL, NULL);
		t_shop->targetHeartTime((HeartTime*)(top_case->getChildByTag(kMT_CP_heartTime)));
		t_shop->setShopCode(kSC_gold);
		addChild(t_shop, kZ_CP_popup);
		is_menu_enable = true;
	}
	else if(tag == kMT_CP_heartShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(NULL, NULL);
		t_shop->targetHeartTime((HeartTime*)(top_case->getChildByTag(kMT_CP_heartTime)));
		t_shop->setShopCode(kSC_heart);
		addChild(t_shop, kZ_CP_popup);
		is_menu_enable = true;
	}
	else if(tag == kMT_CP_friendPoint)
	{
		if(!friend_point_popup)
		{
			CCNode* menu_node = ((CCNode*)pSender)->getParent();
			CCNode* top_node = menu_node->getParent();
			friend_point_popup = CCSprite::create("candy_popup.png");
			friend_point_popup->setAnchorPoint(ccp(0.5,1.f));
			friend_point_popup->setPosition(ccp(427,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
			top_node->addChild(friend_point_popup, -1);
			
			CCSprite* n_close = CCSprite::create("candy_popup_close.png");
			CCSprite* s_close = CCSprite::create("candy_popup_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(ClearPopup::menuAction));
			close_item->setTag(kMT_CP_friendPointClose);
			
			CCMenu* close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(ccp(friend_point_popup->getContentSize().width/2.f, 25));
			friend_point_popup->addChild(close_menu);
			
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(427,menu_node->getPositionY()-12));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::endShowPopup));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
			friend_point_popup->runAction(t_seq);
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kMT_CP_friendPointClose)
	{
		CCNode* menu_node = ((CCNode*)pSender)->getParent();
		CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(427,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::closeFriendPointPopup));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
		friend_point_popup->runAction(t_seq);
	}
}

void ClearPopup::closeFriendPointPopup()
{
	friend_point_popup->removeFromParent();
	friend_point_popup = NULL;
	is_menu_enable = true;
}

void ClearPopup::cellAction( CCObject* sender )
{

}

CCTableViewCell* ClearPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCLabelTTF* nickname_label;
	CCLabelTTF* score_label;
	CCLabelTTF* rank_label;
	ClearFriendRank* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();

	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kCFC_T_img);
	profileImg->setPosition(ccp(62, 20));
	profileImg->setScale(33.f / profileImg->getContentSize().width);
	cell->addChild(profileImg, kCFC_Z_img);

	CCSprite* bg = CCSprite::create("ending_cell_back.png");
	bg->setPosition(CCPointZero);
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,kCFC_Z_case);

	if((*member).user_id == hspConnector::get()->getKakaoID())
	{
		CCSprite* meBack = CCSprite::create("ending_cell_selected.png");
		meBack->setPosition(ccp(meBack->getContentSize().width - bg->getContentSize().width, 0));
		meBack->setAnchorPoint(CCPointZero);
		cell->addChild(meBack,kCFC_Z_case);
	}

	nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
	nickname_label->setPosition(ccp(90,22));
	nickname_label->setAnchorPoint(CCPointZero);
	nickname_label->setTag(kCFC_T_nickname);
	cell->addChild(nickname_label,kCFC_Z_img);

	score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 18);
	score_label->setPosition(ccp(90,0));
	score_label->setAnchorPoint(CCPointZero);
	score_label->setTag(kCFC_T_score);
	cell->addChild(score_label,kCFC_Z_img);

	rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 23);
	rank_label->setPosition(ccp(10,4));
	rank_label->setAnchorPoint(CCPointZero);
	rank_label->setTag(kCFC_T_rank);
	cell->addChild(rank_label,kCFC_Z_img);

	return cell;
}

void ClearPopup::scrollViewDidScroll( CCScrollView* view )
{

}

void ClearPopup::scrollViewDidZoom( CCScrollView* view )
{

}

void ClearPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	//		CCLog("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize ClearPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(195, 45);
}

unsigned int ClearPopup::numberOfCellsInTableView( CCTableView *table )
{
	return friend_list.size();
}
