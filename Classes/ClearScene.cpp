//
//  ClearScene.cpp
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#include "ClearScene.h"
#include "PuzzleMapScene.h"
#include "ScreenSide.h"
#include "StageImgLoader.h"
#include "StarGoldData.h"
#include "SilhouetteData.h"
#include "LogData.h"
#include "CardCase.h"
#include "CountingBMLabel.h"
#include "StageSettingScene.h"
#include "TakeCardPopup.h"

typedef enum tMenuTagClearScene{
	kMT_CS_ok = 1,
	kMT_CS_replay,
	kMT_CS_noti
}MenuTagClearScene;

typedef enum tZorderClearScene{
	kZ_CS_back = 1,
	kZ_CS_img,
	kZ_CS_menu,
	kZ_CS_popup
}ZorderClearScene;

CCScene* ClearScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    ClearScene *layer = ClearScene::create();
	
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    // add layer as a child to scene
    scene->addChild(layer);
	
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool ClearScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	
	myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
	
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
	hspConnector::get()->command("setweeklyscore", p1, NULL);
	
	Json::Value p;
	p["memberID"]=hspConnector::get()->getKakaoID();
	p["score"]=int(mySGD->getScore());
	p["stageNo"]=mySD->getSilType();
	hspConnector::get()->command("addStageScore",p,NULL);
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	if(selected_card_number > 0)
	{
		int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) + 1;
		myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number, durability);
	}
    
	CCSprite* clear_back = CCSprite::create("ending_back.png");
	clear_back->setPosition(ccp(240,160));
	addChild(clear_back, kZ_CS_back);
	
	CCSprite* top_case = CCSprite::create("test_ui_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,320.f));//(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	addChild(top_case, kZ_CS_img);
	
	CountingBMLabel* total_gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "etc_font.fnt", 0.3f);
	total_gold_label->setPosition(ccp(225,top_case->getContentSize().height/2.f));
	top_case->addChild(total_gold_label);
	
	mySGD->setGoldLabel(total_gold_label);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(295,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time);
	
	CCSprite* title = CCSprite::create("ending_clear.png");
	title->setPosition(ccp(137,237));
	addChild(title, kZ_CS_img);
	
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySD->getSilType())->getCString(), mySGD->getFont().c_str(), 18);
	stage_label->setAnchorPoint(ccp(1.f,0.5f));
	stage_label->setPosition(ccp(330,253));
	addChild(stage_label, kZ_CS_img);
	
	
	///////////////////////////// 딤드 로 팝업 띄움
	int stage_number = mySD->getSilType();
	int take_level;
	if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 3;
	else if(mySGD->is_exchanged || mySGD->is_showtime)	take_level = 2;
	else												take_level = 1;
	
	TakeCardPopup* t_popup = TakeCardPopup::create(stage_number, take_level);
	addChild(t_popup, kZ_CS_popup);
	/////////////////////////////////////////////
	
	if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)) == 0)
	{
		myDSH->setIntegerForKey(kDSH_Key_cardTakeCnt, myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt) + 1);
		myDSH->setIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level), myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt));
		myDSH->setIntegerForKey(kDSH_Key_takeCardNumber_int1, myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt), NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
		
		mySGD->addHasGottenCardNumber(NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
	}
	int card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level);
	myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, card_number, mySD->getCardDurability(mySD->getSilType(), take_level));
	
	
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
		
		addChild(star, kZ_CS_img);
	}
	
	score_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	score_label->setAnchorPoint(ccp(0.5,0.5));
	score_label->setPosition(ccp(175,147));
	score_label->setAlignment(kCCTextAlignmentRight);
	addChild(score_label, kZ_CS_img);
	
	gold_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	gold_label->setAnchorPoint(ccp(0.5,0.5));
	gold_label->setPosition(ccp(175,117));
	gold_label->setAlignment(kCCTextAlignmentRight);
	addChild(gold_label, kZ_CS_img);
	
	time_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	time_label->setAnchorPoint(ccp(0.5,0.5));
	time_label->setPosition(ccp(175,88));
	time_label->setAlignment(kCCTextAlignmentRight);
	addChild(time_label, kZ_CS_img);
	
	
	CCSprite* n_ok = CCSprite::create("ending_main.png");
	CCSprite* s_ok = CCSprite::create("ending_main.png");
	s_ok->setColor(ccGRAY);
	
	CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(ClearScene::menuAction));
	ok_item->setTag(kMT_CS_ok);
	
	ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setVisible(false);
	ok_menu->setPosition(ccp(330,33));
	addChild(ok_menu, kZ_CS_menu);
	
	
	CCSprite* n_replay = CCSprite::create("ending_replay.png");
	CCSprite* s_replay = CCSprite::create("ending_replay.png");
	s_replay->setColor(ccGRAY);
	
	CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(ClearScene::menuAction));
	replay_item->setTag(kMT_CS_replay);
	
	replay_menu = CCMenu::createWithItem(replay_item);
	replay_menu->setVisible(false);
	replay_menu->setPosition(ccp(150,33));
	addChild(replay_menu, kZ_CS_menu);
	
	
	is_menu_enable = true;
	
	ScreenSide* t_screen = ScreenSide::create();
	addChild(t_screen, 99999);
	
	
	
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
	hspConnector::get()->command("updateUserData", param2, json_selector(this, ClearScene::resultSavedUserData));
	
    return true;
}

void ClearScene::resultSavedUserData(Json::Value result_data)
{
	if(result_data["state"] == "ok")
	{
		is_saved_user_data = true;
		
		ok_menu->setVisible(true);
		replay_menu->setVisible(true);
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
		hspConnector::get()->command("updateUserData", param2, json_selector(this, ClearScene::resultSavedUserData));
	}
}

void ClearScene::onEnter()
{
	CCLayer::onEnter();
	
	startCalcAnimation();
}

void ClearScene::startCalcAnimation()
{
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
	startScoreAnimation();
}

void ClearScene::startScoreAnimation()
{
	keep_score = mySGD->getScore();
	decrease_score = keep_score;
	increase_score = 0.f;
	schedule(schedule_selector(ClearScene::scoreAnimation));
}

void ClearScene::scoreAnimation(float dt)
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

void ClearScene::stopScoreAnimation()
{
	unschedule(schedule_selector(ClearScene::scoreAnimation));
	score_label->setString(CCString::createWithFormat("%.0f", mySGD->getScore())->getCString());
	startGoldAnimation();
}

void ClearScene::startGoldAnimation()
{
	keep_gold = mySGD->getStageGold();
	decrease_gold = keep_gold;
	increase_gold = 0.f;
	schedule(schedule_selector(ClearScene::goldAnimation));
}

void ClearScene::goldAnimation(float dt)
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

void ClearScene::stopGoldAnimation()
{
	unschedule(schedule_selector(ClearScene::goldAnimation));
	gold_label->setString(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString());
	startTimeAnimation();
}

void ClearScene::startTimeAnimation()
{
	keep_time = mySGD->getGameTime();
	decrease_time = keep_time;
	increase_time = 0.f;
	schedule(schedule_selector(ClearScene::timeAnimation));
}

void ClearScene::timeAnimation(float dt)
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

void ClearScene::stopTimeAnimation()
{
	unschedule(schedule_selector(ClearScene::timeAnimation));
	time_label->setString(CCString::createWithFormat("%d", mySGD->getGameTime())->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
}

void ClearScene::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	is_menu_enable = false;
	AudioEngine::sharedInstance()->stopAllEffects();
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kMT_CS_ok)
	{
		mySGD->resetLabels();
		realEnd();
	}
	else if(tag == kMT_CS_replay)
	{
		mySGD->resetLabels();
		CCDirector::sharedDirector()->replaceScene(StageSettingScene::scene());
	}
}


void ClearScene::realEnd()
{
	CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
}

void ClearScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void ClearScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(ClearScene::alertAction));
}