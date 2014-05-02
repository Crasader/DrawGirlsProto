//
//  ClearPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 19..
//
//

#include "ClearPopup.h"

#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include "LogData.h"
#include "CardCase.h"
#include "CountingBMLabel.h"
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
#include "ASPopupView.h"
#include "StartSettingScene.h"
#include "MiniGamePopup.h"
#include "TutorialFlowStep.h"
#include "AchieveNoti.h"
#include "SendMessageUtil.h"
#include "CardAnimations.h"
#include "GDWebSprite.h"
#include "KSLabelTTF.h"
#include "FormSetter.h"
#include "TakeCardToDiary.h"
#include "LoadingLayer.h"
#include "FlagSelector.h"
#include "EventShopPopup.h"

typedef enum tMenuTagClearPopup{
	kMT_CP_ok = 1,
	kMT_CP_replay,
	kMT_CP_noti
}MenuTagClearPopup;

typedef enum tZorderClearPopup{
	kZ_CP_gray = 1,
	kZ_CP_back,
	kZ_CP_img,
	kZ_CP_table,
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
	
	AudioEngine::sharedInstance()->preloadEffectScene("Ending");
	
	is_take_star_effect = false;
	
	if(myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep) == kTutorialFlowStep_ingame)
		myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_homeClick);
	
	is_menu_enable = false;
//	is_loaded_list = false;
	is_end_popup_animation = false;
	is_end_take_card = false;
	
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
	
	send_command_list.clear();
	
//	Json::Value param;
//	param["key"] = CCSTR_CWF("stage_clear_%d", mySD->getSilType())->getCString();
//	
//	send_command_list.push_back(CommandParam("increaseStats", param, nullptr));
	
	if(mySGD->getScore() > myDSH->getIntegerForKey(kDSH_Key_allHighScore))
	{
		myDSH->setIntegerForKey(kDSH_Key_allHighScore, int(mySGD->getScore()));
	}
	
	Json::Value p1;
	p1["memberID"] = hspConnector::get()->getSocialID();
	p1["score"]=int(mySGD->getScore());
	Json::Value p1_data;
	p1_data["selectedcard"] = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	p1_data["allhighscore"] = mySGD->getScore();//myDSH->getIntegerForKey(kDSH_Key_allHighScore);
	p1_data["highstage"] = mySGD->suitable_stage;
	p1_data["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	p1_data["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	Json::FastWriter p1_data_writer;
	p1["data"] = p1_data_writer.write(p1_data);
	
	send_command_list.push_back(CommandParam("addweeklyscore", p1, nullptr));
	
	is_rank_changed = false;
//	if(mySGD->save_stage_rank_stageNumber == mySD->getSilType())
//	{
//		// 받아왔던 rank list 사용
//		string my_id = hspConnector::get()->getSocialID();
//		
//		vector<RankFriendInfo>::iterator iter = find(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), my_id);
//		if(iter != mySGD->save_stage_rank_list.end())
//		{
//			// found
//			if((*iter).score < mySGD->getScore())
//			{
//				Json::Value p;
//				p["memberID"]=hspConnector::get()->getSocialID();
//				p["score"]=int(mySGD->getScore());
//				p["stageNo"]=mySD->getSilType();
//				hspConnector::get()->command("setStageScore",p,NULL);
//				
//				
//				before_my_rank = (*iter).rank;
//				(*iter).score = mySGD->getScore();
//				struct t_FriendSort{
//					bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
//					{
//						return a.score > b.score;
//					}
//				} pred;
//				
//				sort(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), pred);
//				
//				for(int i=0;i<mySGD->save_stage_rank_list.size();i++)
//					mySGD->save_stage_rank_list[i].rank = i+1;
//				
//				vector<RankFriendInfo>::iterator iter2 = find(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), my_id);
//				if((*iter2).rank < before_my_rank)
//				{
//					is_rank_changed = true;
//					recent_my_rank = (*iter2).rank;
//					next_rank_info = (*(iter2+1));
//				}
//			}
//		}
//		else
//		{
//			CCLOG("not found myKakaoID");
//			
//			Json::Value p;
//			p["memberID"]=hspConnector::get()->getSocialID();
//			p["score"]=int(mySGD->getScore());
//			p["stageNo"]=mySD->getSilType();
//			hspConnector::get()->command("setStageScore",p,NULL);
//			
//			Json::Value my_kakao = hspConnector::get()->myKakaoInfo;
//			
//			RankFriendInfo fInfo;
//			fInfo.nickname = my_kakao["nickname"].asString();
//			fInfo.img_url = my_kakao["profile_image_url"].asString();
//			fInfo.user_id = my_kakao["user_id"].asString();
//			fInfo.score = mySGD->getScore();
//			fInfo.is_play = true;
//			mySGD->save_stage_rank_list.push_back(fInfo);
//			
//			struct t_FriendSort{
//				bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
//				{
//					return a.score > b.score;
//				}
//			} pred;
//			
//			sort(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), pred);
//			
//			for(int i=0;i<mySGD->save_stage_rank_list.size();i++)
//				mySGD->save_stage_rank_list[i].rank = i+1;
//			
//			vector<RankFriendInfo>::iterator iter2 = find(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), my_id);
//			if((*iter2).rank < mySGD->save_stage_rank_list.size())
//			{
//				is_rank_changed = true;
//				recent_my_rank = (*iter2).rank;
//				next_rank_info = (*(iter2+1));
//			}
//		}
//		
//		friend_list = mySGD->save_stage_rank_list;
//		is_loaded_list = true;
//	}
//	else
//	{
//		Json::Value p;
//		p["memberID"]=hspConnector::get()->getSocialID();
//		p["score"]=int(mySGD->getScore());
//		p["stageNo"]=mySD->getSilType();
//	Json::Value p_data;
//	p_data["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
//	Json::FastWriter p_data_writer;
//	p["data"] = p_data_writer.write(p_data);
//	
//	send_command_list.push_back(CommandParam("setStageScore",p,nullptr));
//	
//	}
    
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-22.f-450));
	addChild(main_case, kZ_CP_back);
	
	CCScale9Sprite* inner_left = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	inner_left->setContentSize(CCSizeMake(230, 215));
	inner_left->setPosition(main_case->getContentSize().width*0.26f, main_case->getContentSize().height*0.58f);
	main_case->addChild(inner_left);
	
	
	CCScale9Sprite* inner_right = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	inner_right->setContentSize(CCSizeMake(230, 215));
	inner_right->setPosition(main_case->getContentSize().width*0.74f, main_case->getContentSize().height*0.58f);
	main_case->addChild(inner_right);
	
	
	CCScale9Sprite* star_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	star_back->setContentSize(CCSizeMake(215, 60));
	star_back->setPosition(ccp(115,140));
	inner_left->addChild(star_back);
	
	
	CCScale9Sprite* time_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	time_back->setContentSize(CCSizeMake(215, 35));
	time_back->setPosition(ccp(115,93));
	inner_left->addChild(time_back);
	
	
	CCScale9Sprite* gold_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	gold_back->setContentSize(CCSizeMake(215, 35));
	gold_back->setPosition(ccp(115,60));
	inner_left->addChild(gold_back);
	
	
	CCScale9Sprite* score_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	score_back->setContentSize(CCSizeMake(215, 35));
	score_back->setPosition(ccp(115,27));
	inner_left->addChild(score_back);
	
	
	title = CCSprite::create(CCString::createWithFormat("ending_clear_%s.png", myLoc->getSupportLocalCode())->getCString());
	title->setScale(0);
	title->setPosition(ccp(132,240));
	main_case->addChild(title, kZ_CP_menu);
	
	
	CCSprite* stage_tab = CCSprite::create("ending_tab.png");
	stage_tab->setPosition(ccp(40,253));
	main_case->addChild(stage_tab);
	
	int stage_number = mySD->getSilType();
	
	if(mySGD->is_before_selected_event_stage)
	{
		KSLabelTTF* stage_number_label = KSLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(),	mySGD->getFont().c_str(), 14);
		stage_number_label->setColor(ccc3(255, 170, 20));
		stage_number_label->enableOuterStroke(ccc3(95, 65, 130), 1.f);
		stage_number_label->setPosition(ccp(24, 16));
		stage_tab->addChild(stage_number_label);
		
		mySGD->is_before_selected_event_stage = false;
	}
	else
	{
//		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
//		int piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		KSLabelTTF* piece_number_label = KSLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(),	mySGD->getFont().c_str(), 14);
		piece_number_label->setColor(ccc3(255, 170, 20));
		piece_number_label->enableOuterStroke(ccc3(95, 65, 130), 1.f);
		piece_number_label->setPosition(ccp(24, 16));
		stage_tab->addChild(piece_number_label);
	}
	
	int take_level;
	if(mySGD->is_showtime && mySGD->is_exchanged)	take_level = 4;
	else if(mySGD->is_showtime)					take_level = 3;
	else if(mySGD->is_exchanged)				take_level = 2;
	else										take_level = 1;
	
	int start_stage_number = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
	
	int minimum_count = 0;
	int maximum_count = 0;
	
	for(int i=start_stage_number;i<start_stage_number+stage_count;i++)
	{
		bool is_found = false;
		for(int j=4;j>=1 && !is_found;j--)
		{
			int check_card_number = NSDS_GI(i, kSDS_SI_level_int1_card_i, j);
			
			if(mySGD->isHasGottenCards(check_card_number) > 0)
			{
				is_found = true;
				if(j == 4)
					maximum_count++;
				minimum_count++;
			}
		}
	}
	
	myLog->addLog(kLOG_puzzleAchievementMinimum_i, -1, 100*minimum_count/stage_count);
	myLog->addLog(kLOG_puzzleAchievementMaximum_i, -1, 100*maximum_count/stage_count);
	
	
	send_command_list.push_back(myLog->getSendLogCommand(CCString::createWithFormat("clear_%d", stage_number)->getCString()));
	
	is_saved_user_data = false;
	
	ani_stars.clear();
	
	for(int i=0;i<4;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_empty.png");
		t_star->setPosition(ccp(53+i*48,195));
		main_case->addChild(t_star, kZ_CP_img);
	}
	
	for(int i=0;i<take_level;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_gold.png");
		t_star->setPosition(ccp(53+i*48,195));
		main_case->addChild(t_star, kZ_CP_table);
		t_star->setScale(2.f);
		t_star->setOpacity(0);
		t_star->setRotation(-200);
		
		ani_stars.push_back(t_star);
		
		CCSprite* t_star_ani = CCSprite::create("ending_star_gold.png");
		t_star_ani->setScale(2.f);
		t_star_ani->setOpacity(0);
		t_star_ani->setPosition(ccp(t_star->getContentSize().width/2.f, t_star->getContentSize().height/2.f));
		t_star->addChild(t_star_ani, 1, 1);
	}
	
	if(mySGD->is_clear_diary)
	{
		TakeCardToDiary* t_take_card_popup = TakeCardToDiary::create(NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, take_level), [=](){endTakeCard();});
		addChild(t_take_card_popup, kZ_CP_popup+5);
	}
	else
		endTakeCard();
	
	KSLabelTTF* time_ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_time), mySGD->getFont().c_str(), 15);
	time_ment->enableOuterStroke(ccc3(40, 15, 55), 1.f);
	time_ment->setPosition(ccp(48,148));
	main_case->addChild(time_ment, kZ_CP_img);
	
	CCSprite* time_img = CCSprite::create("ending_mark_time.png");
	time_img->setPosition(ccp(80,148));
	main_case->addChild(time_img, kZ_CP_img);
	
	time_label = KSLabelTTF::create(CCString::createWithFormat("%d", mySGD->getGameTime())->getCString(), mySGD->getFont().c_str(), 15);
	time_label->enableOuterStroke(ccc3(40, 15, 55), 1.f);
	time_label->setAnchorPoint(ccp(1,0.5));
	time_label->setPosition(ccp(220,148));
	main_case->addChild(time_label, kZ_CP_img);
	
	KSLabelTTF* gold_ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_gold), mySGD->getFont().c_str(), 15);
	gold_ment->enableOuterStroke(ccc3(40, 15, 55), 1.f);
	gold_ment->setPosition(ccp(48,115));
	main_case->addChild(gold_ment, kZ_CP_img);
	
	CCSprite* gold_img = CCSprite::create("ending_mark_gold.png");
	gold_img->setPosition(ccp(80,115));
	main_case->addChild(gold_img, kZ_CP_img);
	
	gold_label = KSLabelTTF::create(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString(), mySGD->getFont().c_str(), 15);
	gold_label->enableOuterStroke(ccc3(40, 15, 55), 1.f);
	gold_label->setAnchorPoint(ccp(1,0.5));
	gold_label->setPosition(ccp(220,115));
	main_case->addChild(gold_label, kZ_CP_img);
	
	KSLabelTTF* score_ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_score), mySGD->getFont().c_str(), 15);
	score_ment->enableOuterStroke(ccc3(40, 15, 55), 1.f);
	score_ment->setPosition(ccp(48,82));
	main_case->addChild(score_ment, kZ_CP_img);
	
	CCSprite* score_img = CCSprite::create("ending_mark_score.png");
	score_img->setPosition(ccp(80,82));
	main_case->addChild(score_img, kZ_CP_img);
	
	score_label = KSLabelTTF::create(CCString::createWithFormat("%.0f", mySGD->getBaseScore())->getCString(), mySGD->getFont().c_str(), 15);
	score_label->enableOuterStroke(ccc3(40, 15, 55), 1.f);
	score_label->setAnchorPoint(ccp(1,0.5));
	score_label->setPosition(ccp(220,82));
	main_case->addChild(score_label, kZ_CP_img);
	
	
	
	CCSprite* n_ok = CCSprite::create("ending_button.png");
	KSLabelTTF* n_ok_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_nextStage), mySGD->getFont().c_str(), 22);
	n_ok_label->setColor(ccc3(50, 30, 5));
	n_ok_label->enableOuterStroke(ccc3(50, 30, 5), 0.25f);
	n_ok_label->setPosition(ccp(n_ok->getContentSize().width/2.f, n_ok->getContentSize().height/2.f-3));
	n_ok->addChild(n_ok_label);
	
	CCSprite* s_ok = CCSprite::create("ending_button.png");
	s_ok->setColor(ccGRAY);
	KSLabelTTF* s_ok_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_nextStage), mySGD->getFont().c_str(), 22);
	s_ok_label->setColor(ccc3(50, 30, 5));
	s_ok_label->enableOuterStroke(ccc3(50, 30, 5), 0.25f);
	s_ok_label->setPosition(ccp(s_ok->getContentSize().width/2.f, s_ok->getContentSize().height/2.f-3));
	s_ok->addChild(s_ok_label);
	
	CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(ClearPopup::menuAction));
	ok_item->setTag(kMT_CP_ok);
	
	ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setVisible(false);
	ok_menu->setPosition(ccp(354,36));
	main_case->addChild(ok_menu, kZ_CP_menu);
	ok_menu->setTouchPriority(-200);
	
//	if(!mySGD->getIsMeChallenge() && !mySGD->getIsAcceptChallenge() && !mySGD->getIsAcceptHelp())
//	{
		CCSprite* n_replay = CCSprite::create("ending_button.png");
	KSLabelTTF* n_replay_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_regame), mySGD->getFont().c_str(), 22);
	n_replay_label->setColor(ccc3(50, 30, 5));
	n_replay_label->enableOuterStroke(ccc3(50, 30, 5), 0.25f);
	n_replay_label->setPosition(ccp(n_replay->getContentSize().width/2.f, n_replay->getContentSize().height/2.f-3));
	n_replay->addChild(n_replay_label);
		CCSprite* s_replay = CCSprite::create("ending_button.png");
		s_replay->setColor(ccGRAY);
	KSLabelTTF* s_replay_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_regame), mySGD->getFont().c_str(), 22);
	s_replay_label->setColor(ccc3(50, 30, 5));
	s_replay_label->enableOuterStroke(ccc3(50, 30, 5), 0.25f);
	s_replay_label->setPosition(ccp(s_replay->getContentSize().width/2.f, s_replay->getContentSize().height/2.f-3));
	s_replay->addChild(s_replay_label);
		
		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(ClearPopup::menuAction));
		replay_item->setTag(kMT_CP_replay);
		
		replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setVisible(false);
		replay_menu->setPosition(ccp(125,36));
		main_case->addChild(replay_menu, kZ_CP_menu);
		replay_menu->setTouchPriority(-200);
//	}
//	else
//	{
//		if((mySGD->getIsMeChallenge() && mySGD->getScore() < mySGD->getMeChallengeTargetScore()) || (mySGD->getIsAcceptChallenge() && mySGD->getScore() < mySGD->getAcceptChallengeScore()))
//		{
//			CCSprite* n_replay = CCSprite::create("ending_replay2.png");
//			CCSprite* s_replay = CCSprite::create("ending_replay2.png");
//			s_replay->setColor(ccGRAY);
//			
//			CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(ClearPopup::menuAction));
//			replay_item->setTag(kMT_CP_replay);
//			
//			replay_menu = CCMenu::createWithItem(replay_item);
//			replay_menu->setVisible(false);
//			replay_menu->setPosition(ccp(130,38));
//			main_case->addChild(replay_menu, kZ_CP_menu);
//			replay_menu->setTouchPriority(-200);
//		}
//		else
//			replay_menu = NULL;
//	}
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	loading_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_img->setPosition(ccp(347,130));
	main_case->addChild(loading_img, kZ_CP_img);
	reader->release();
	
	Json::Value param2;
	param2["myScore"]=int(mySGD->getScore());
	param2["stageNo"]=mySD->getSilType();
	param2["memberID"] = hspConnector::get()->getSocialID();
	Json::Value p2_data;
	p2_data["selectedcard"] = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	p2_data["allhighscore"] = mySGD->getScore();//myDSH->getIntegerForKey(kDSH_Key_allHighScore);
	p2_data["highstage"] = mySGD->suitable_stage;
	p2_data["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	p2_data["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	Json::FastWriter p2_data_writer;
	param2["data"] = p2_data_writer.write(p2_data);
	
	send_command_list.push_back(CommandParam("getstagerankbyalluser", param2, json_selector(this, ClearPopup::resultGetRank)));
	mySGD->keep_time_info.is_loaded = false;
	send_command_list.push_back(CommandParam("gettimeinfo", Json::Value(), json_selector(this, ClearPopup::resultGetTime)));
	
	mySGD->setUserdataFailCount(0);
	if(mySGD->is_changed_userdata)
		send_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	LoadingLayer* t_loading = LoadingLayer::create(-9999);
	addChild(t_loading, 9999);
	
	tryTransaction(t_loading);
	
    return true;
}

void ClearPopup::tryTransaction(CCNode* t_loading)
{
	mySGD->changeGoodsTransaction(send_command_list, [=](Json::Value result_data)
								  {
									  if(result_data["result"]["code"].asInt() == GDSUCCESS)
									  {
										  CCLOG("ClearPopup transaction success");
										  
										  t_loading->removeFromParent();
									  }
									  else
									  {
										  CCLOG("ClearPopup transaction fail");
										  
										  addChild(KSTimer::create(0.1f, [=](){tryTransaction(t_loading);}));
									  }
								  });
}

ClearPopup::~ClearPopup()
{
	hspConnector::get()->removeTarget(this);
}

void ClearPopup::resultGetTime(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->keep_time_info.timestamp = result_data["timestamp"].asUInt();
		mySGD->keep_time_info.weekNo = result_data["weekNo"].asUInt();
		mySGD->keep_time_info.weekday = result_data["weekday"].asInt();
		mySGD->keep_time_info.date = result_data["date"].asInt64();
		mySGD->keep_time_info.hour = result_data["hour"].asInt();
		mySGD->keep_time_info.is_loaded = true;
		
		is_go_to_mainflow = false;
		
		int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		bool is_open = mySGD->getPuzzleHistory(puzzle_number).is_open;
		
		if(!is_open)
		{
			string puzzle_condition = NSDS_GS(puzzle_number, kSDS_PZ_condition_s);
			
			Json::Value condition_list;
			Json::Reader reader;
			reader.parse(puzzle_condition, condition_list);
			
			if(condition_list.size() <= 0)
				is_open = true;
			
			bool is_base_condition_success = true;
			
			for(int i=0;!is_open && i<condition_list.size();i++)
			{
				Json::Value t_condition_and = condition_list[i];
				
				bool and_open = true;
//				bool is_time_condition = false;
				
				for(int j=0;and_open && j<t_condition_and.size();j++)
				{
					Json::Value t_condition = t_condition_and[j];
					string t_type = t_condition["type"].asString();
					if(t_type == "p")
					{
						if(!mySGD->getPuzzleHistory(t_condition["value"].asInt()).is_clear)
						{
							and_open = false;
							is_base_condition_success = false;
						}
					}
					else if(t_type == "s")
					{
						if(mySGD->getClearStarCount() < t_condition["value"].asInt())
						{
							and_open = false;
							is_base_condition_success = false;
						}
					}
					else if(t_type == "r")
					{
						and_open = false;
					}
					else if(t_type == "w")
					{
//						is_time_condition = true;
						if(!mySGD->keep_time_info.is_loaded)
							and_open = false;
						else
						{
							int weekday = t_condition["weekday"].asInt();
							if(mySGD->keep_time_info.weekday.getV() != -1 && mySGD->keep_time_info.weekday.getV() != weekday)
								and_open = false;
							if(mySGD->keep_time_info.hour.getV() < t_condition["s"].asInt() || mySGD->keep_time_info.hour.getV() >= t_condition["e"].asInt())
								and_open = false;
						}
					}
					else if(t_type == "d")
					{
//						is_time_condition = true;
						if(mySGD->keep_time_info.date.getV() < t_condition["s"].asInt64() || mySGD->keep_time_info.date.getV() >= t_condition["e"].asInt64())
							and_open = false;
					}
				}
				
				if(and_open)
				{
					is_open = true;
//					if(!is_time_condition)
//					{
//						PuzzleHistory t_history = mySGD->getPuzzleHistory(puzzle_number);
//						t_history.is_open = true;
//						t_history.open_type = "무료";
//						mySGD->setPuzzleHistory(t_history, nullptr);
//					}
				}
			}
			
			if(!is_open)
			{
				is_go_to_mainflow = true;
				replay_menu->setEnabled(false);
			}
		}
	}
}
								
void ClearPopup::resultGetRank(Json::Value result_data)
{
	cell_action_list.clear();
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		
		CCSprite* graph_back = CCSprite::create("ending_graph.png");
		graph_back->setPosition(ccp(355,230));
		main_case->addChild(graph_back, kZ_CP_img);
		
		KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
		t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		t_rank_a->setPosition(ccp(25,8));
		graph_back->addChild(t_rank_a);
		
		KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
		t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		t_rank_b->setPosition(ccp(25+49,8));
		graph_back->addChild(t_rank_b);
		
		KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
		t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		t_rank_c->setPosition(ccp(25+98,8));
		graph_back->addChild(t_rank_c);
		
		KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
		t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		t_rank_d->setPosition(ccp(25+147,8));
		graph_back->addChild(t_rank_d);
		
		int alluser = result_data["alluser"].asInt();
		int myrank = result_data["myrank"].asInt();
		
		CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", alluser)->getCString(), mySGD->getFont().c_str(), 10);
		all_user_label->setColor(ccc3(255, 50, 50));
		all_user_label->setAnchorPoint(ccp(1,0.5));
		all_user_label->setPosition(ccp(main_case->getContentSize().width-30, 210));
		main_case->addChild(all_user_label, kZ_CP_img);
		
		CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_myrankValue), myrank)->getCString(), mySGD->getFont().c_str(), 10);
		my_rank_label->setAnchorPoint(ccp(1,0.5));
		my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
		main_case->addChild(my_rank_label, kZ_CP_img);
		my_rank_label->setOpacity(0);
		
		float rank_percent = 1.f*myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,0));
		rank_percent_case->setPosition(ccp(257+195,230));
		main_case->addChild(rank_percent_case, kZ_CP_img);
		
		KSLabelTTF* percent_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", rank_percent*100.f)->getCString(), mySGD->getFont().c_str(), 13);
		percent_label->setColor(ccc3(255, 170, 20));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f+2));
		rank_percent_case->addChild(percent_label, kZ_CP_img);
		percent_label->setOpacity(0);
		
		
		cell_action_list.push_back([=](){
			CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(257 + 195.f*rank_percent,230));
			rank_percent_case->runAction(t_move);
			
			CCDelayTime* t_delay1 = CCDelayTime::create(1.f);
			CCFadeTo* t_fade1 = CCFadeTo::create(0.5f, 255);
			CCSequence* t_seq1 = CCSequence::create(t_delay1, t_fade1, NULL);
			my_rank_label->runAction(t_seq1);
			
			CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
			CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 255);
			CCSequence* t_seq2 = CCSequence::create(t_delay2, t_fade2, NULL);
			percent_label->runAction(t_seq2);
		});
		
		Json::Value user_list = result_data["list"];
		
		delay_index = 0;
		int limit_count = 3;
		for(int i=0;i<user_list.size() && i<limit_count;i++)
		{
			string case_name;
			if(myrank == i+1)
			{
				case_name = "mainpopup_pupple1.png";
				limit_count++;
			}
			else
			{
				case_name = "rank_normal.png";
			}
			
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			list_cell_case->setContentSize(CCSizeMake(210, 40));
			list_cell_case->setPosition(ccp(355,185-i*35));
			main_case->addChild(list_cell_case, kZ_CP_img);
			
			CCPoint rank_position = ccp(20,20);
			if(i == 0)
			{
				CCSprite* gold_medal = CCSprite::create("rank_gold.png");
				gold_medal->setPosition(rank_position);
				list_cell_case->addChild(gold_medal);
			}
			else if(i == 1)
			{
				CCSprite* silver_medal = CCSprite::create("rank_silver.png");
				silver_medal->setPosition(rank_position);
				list_cell_case->addChild(silver_medal);
			}
			else if(i == 2)
			{
				CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
				bronze_medal->setPosition(rank_position);
				list_cell_case->addChild(bronze_medal);
			}
			else
			{
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 12);
				rank_label->enableOuterStroke(ccBLACK, 1);
				rank_label->setPosition(rank_position);
				list_cell_case->addChild(rank_label);
			}
			
			Json::Reader reader;
			Json::Value read_data;
			reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
			
			string flag = read_data.get("flag", "kr").asString().c_str();
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setPosition(ccp(42,20));
			selectedFlagSpr->setScale(0.5);
			list_cell_case->addChild(selectedFlagSpr);
			
			KSLabelTTF* nick_label = KSLabelTTF::create(read_data.get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12); // user_list[i]["nick"].asString().c_str()
			nick_label->setAnchorPoint(ccp(0,0.5));
			nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
			nick_label->setPosition(ccp(55,20));
			list_cell_case->addChild(nick_label);
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",user_list[i]["score"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setAnchorPoint(ccp(1,0.5));
			score_label->setColor(ccc3(255, 170, 20));
			score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
			score_label->setPosition(ccp(200,20));
			list_cell_case->addChild(score_label);
			
			CCPoint original_position = list_cell_case->getPosition();
			list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
			
			cell_action_list.push_back([=](){
				CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
				CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
				CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
				list_cell_case->runAction(t_seq);
				delay_index++;
			});
		}
		
		if(myrank > 3)
		{
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			list_cell_case->setContentSize(CCSizeMake(210, 40));
			list_cell_case->setPosition(ccp(355,185-3*35));
			main_case->addChild(list_cell_case, kZ_CP_img);
			
			KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", myrank)->getCString(), mySGD->getFont().c_str(), 12);
			rank_label->enableOuterStroke(ccBLACK, 1);
			rank_label->setPosition(ccp(20,20));
			list_cell_case->addChild(rank_label);
			
			string flag = myDSH->getStringForKey(kDSH_Key_flag);
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setPosition(ccp(42,20));
			selectedFlagSpr->setScale(0.5);
			list_cell_case->addChild(selectedFlagSpr);
			
			KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12);
			nick_label->setAnchorPoint(ccp(0,0.5));
			nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
			nick_label->setPosition(ccp(55,20));
			list_cell_case->addChild(nick_label);
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",int(mySGD->getScore()))->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setAnchorPoint(ccp(1,0.5));
			score_label->setColor(ccc3(255, 170, 20));
			score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
			score_label->setPosition(ccp(200,20));
			list_cell_case->addChild(score_label);
			
			CCPoint original_position = list_cell_case->getPosition();
			list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
			
			cell_action_list.push_back([=](){
				CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
				CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
				CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
				list_cell_case->runAction(t_seq);
			});
		}
		
		is_saved_user_data = true;
		endLoad();
		
		if(is_end_popup_animation)
		{
			for(int i=0;i<cell_action_list.size();i++)
				cell_action_list[i]();
		}
		
		loading_img->removeFromParent();
	}
	else
	{
		hspConnector::get()->command(send_command_list);
		
//		CCLabelTTF* fail_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
//		fail_label->setPosition(loading_img->getPosition());
//		main_case->addChild(fail_label, kZ_CP_img);
	}
}

void ClearPopup::showPopup()
{
	int autobalanceTry = NSDS_GI(mySD->getSilType(), kSDS_SI_autoBalanceTry_i);
	int seq_no_fail_cnt = myDSH->getIntegerForKey(kDSH_Key_achieve_seqNoFailCnt)+2;
	
	if(autobalanceTry>0){
		myDSH->setIntegerForKey(kDSH_Key_achieve_seqNoFailCnt, seq_no_fail_cnt);
		CCLOG("up autobalance,kDSH_Key_achieve_seqNoFailCnt value %d",seq_no_fail_cnt);
	}else{
		CCLOG("no up autobalance,kDSH_Key_achieve_seqNoFailCnt value %d",seq_no_fail_cnt);
	}
	
	AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
	
	for(int i=kAchievementCode_noFail1;i<=kAchievementCode_noFail3;i++)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
		   seq_no_fail_cnt == shared_acr->getCondition((AchievementCode)i))
		{
			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
			AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
	}
	
	gray->setOpacity(255);
//	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
//	gray->runAction(gray_fade);

	main_case->setPosition(ccp(240,160-22.f));
	endShowPopup();
//	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
//	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::endShowPopup));
//	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
//	main_case->runAction(main_seq);
}

void ClearPopup::endShowPopup()
{
	
}

void ClearPopup::hidePopup()
{
	is_menu_enable = false;
//	rankTableView->setTouchEnabled(false);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-22.f-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void ClearPopup::endHidePopup()
{
	AudioEngine::sharedInstance()->unloadEffectScene("Ending");
	
	if(target_final)
		(target_final->*delegate_final)();
	
	if(is_go_to_mainflow)
		goToMainFlow_func();
	
	removeFromParent();
}

void ClearPopup::checkChallengeOrHelp()
{
//	if(mySGD->getIsAcceptHelp())
//	{
//		////////////////// ksks
//		CCLOG("zzzz");
//		addChild(HelpResultSend::create(mySGD->getAcceptHelpId(), true, [=](){closePopup();}), kZ_CP_popup);
//	}
//	else if(mySGD->getIsMeChallenge())
//	{
//		//  파라메터 (내사진,내점수,내닉네임,상대사진,상대점수,상대닉네임,터치우선순위)
//		hspConnector* t_hsp = hspConnector::get();
//		FightResultAnimation* b = FightResultAnimation::create(t_hsp->getKakaoProfileURL(),mySGD->getScore(),t_hsp->getKakaoNickname(),
//															   mySGD->getMeChallengeTargetProfile(),mySGD->getMeChallengeTargetScore(),mySGD->getMeChallengeTargetNick(),
//															   -200);
//		b->setCancelFunc([](){
//			CCLOG("닫기눌렀을때");
//		});
//		b->setCloseFunc([=](){
//			CCLOG("창 다닫혔을때");
//			checkMiniGame();
//		});
//		
//		addChild(b,kZ_CP_popup);
//		
//		if(mySGD->getScore() > mySGD->getMeChallengeTargetScore())
//		{
//			b->setSendFunc([=](){
//				CCLOG("승리시 확인눌렀을때->메세지전송");
//			
//				Json::Value p;
//				Json::Value contentJson;
//				contentJson["msg"] = (mySGD->getMeChallengeTargetNick() + "님에게 도전!");
//				contentJson["challengestage"] = mySD->getSilType();
//				contentJson["score"] = mySGD->getScore();
//				contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//				contentJson["replaydata"] = mySGD->replay_write_info;
//				contentJson["profile"] = t_hsp->getKakaoProfileURL();
//				p["content"] = GraphDogLib::JsonObjectToString(contentJson);
//				std::string recvId = mySGD->getMeChallengeTarget();
//				p["receiverMemberID"] = recvId;
//				
//				p["senderMemberID"] = hspConnector::get()->getSocialID();
//				
//				p["type"] = kChallengeRequest;
//				hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
//											 {
//												 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//												 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//												 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//												 
//												 //																		setHelpSendTime(recvId);
//												 if(r["result"]["code"].asInt() != GDSUCCESS)
//												 {
//													 // 에러.
//													 return;
//												 }
//												 
//												 setChallengeSendTime(mySGD->getMeChallengeTarget());
//												 //																	 friend_list.erase(friend_list.begin() + tag);
//												 GraphDogLib::JsonToLog("sendMessage", r);
//												 
//												 //																		obj->removeFromParent();
//												 
////												 Json::Value p2;
////												 p2["receiver_id"] = recvId;
////												 p2["message"] = "도전을 신청한다!!";
////												 hspConnector::get()->kSendMessage
////												 (p2, [=](Json::Value r)
////												  {
////													  GraphDogLib::JsonToLog("kSendMessage", r);
////												  });
//											 });
//			});
//			
//			b->startWin(); //내가이겼을때
//			
//		}
//		else
//		{
//			b->setConfirmFunc([](){
//				CCLOG("패배시 확인눌렀을때");
//			});
//			
//			b->startLose(); //내가졌을때
//		}
//		
//		// ksks
////		addChild(ChallengeSend::create(mySGD->getMeChallengeTarget(), mySGD->getMeChallengeTargetNick(), mySGD->getScore(),
////									   ChallengeCategory::kRequest), kZ_CP_popup);
//		//		getMeChallengeTarget
//	}
//	else if(mySGD->getIsAcceptChallenge())
//	{
//		mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)] = 0;
//		/////////////////// ksks
//		//		mySGD->getAcceptChallengeId(), mySGD->getAcceptChallengeNick(), mySGD->getAcceptChallengeScore();
//		
//		//  파라메터 (내사진,내점수,내닉네임,상대사진,상대점수,상대닉네임,터치우선순위)
//		hspConnector* t_hsp = hspConnector::get();
//		FightResultAnimation* b = FightResultAnimation::create(t_hsp->getKakaoProfileURL(),mySGD->getScore(),t_hsp->getKakaoNickname(),
//															   mySGD->getAcceptChallengeProfile(),mySGD->getAcceptChallengeScore(),mySGD->getAcceptChallengeNick(),
//															   -200);
//		b->setCancelFunc([](){
//			CCLOG("닫기눌렀을때");
//		});
//		b->setCloseFunc([=](){
//			CCLOG("창 다닫혔을때");
//			checkMiniGame();
//		});
//		
//		addChild(b,kZ_CP_popup);
//		
//		if(mySGD->getAcceptChallengeScore() < mySGD->getScore())
//		{
//			b->setSendFunc([=](){
//				CCLOG("승리시 확인눌렀을때->메세지전송");
//				
//				Json::Value p;
//				Json::Value contentJson;
//				contentJson["msg"] = (mySGD->getAcceptChallengeNick() + "님에게 도전!");
//				contentJson["challengestage"] = mySD->getSilType();
//				contentJson["score"] = mySGD->getScore();
//				contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//				contentJson["replaydata"] = mySGD->replay_write_info;
//				contentJson["profile"] = t_hsp->getKakaoProfileURL();
//				p["content"] = GraphDogLib::JsonObjectToString(contentJson);
//				std::string recvId = mySGD->getAcceptChallengeId();
//				p["receiverMemberID"] = recvId;
//				
//				p["senderMemberID"] = hspConnector::get()->getSocialID();
//				
//				p["type"] = kChallengeRequest;
//				hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
//											 {
//												 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//												 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//												 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//												 
//												 //																		setHelpSendTime(recvId);
//												 if(r["result"]["code"].asInt() != GDSUCCESS)
//												 {
//													 // 에러.
//													 return;
//												 }
//												 
//												 setChallengeSendTime(mySGD->getAcceptChallengeId());
//												 //																	 friend_list.erase(friend_list.begin() + tag);
//												 GraphDogLib::JsonToLog("sendMessage", r);
//												 
//												 //																		obj->removeFromParent();
//												 
////												 Json::Value p2;
////												 p2["receiver_id"] = recvId;
////												 p2["message"] = "도전을 신청한다!!";
////												 hspConnector::get()->kSendMessage
////												 (p2, [=](Json::Value r)
////												  {
////													  GraphDogLib::JsonToLog("kSendMessage", r);
////												  });
//											 });
//				
//				checkMiniGame();
//			});
//			
//			b->startWin(); //내가이겼을때
//		}
//		else
//		{
//			b->setConfirmFunc([](){
//				CCLOG("패배시 확인눌렀을때");
//			});
//			
//			b->startLose(); //내가졌을때
//		}
//	}
//	else
//	{
//		if(is_rank_changed)
//		{
//			hspConnector* t_hsp = hspConnector::get();
//			//  파라메터 (내사진,내점수,내등수,내닉네임,상대사진,상대점수,상대등수,상대닉네임,터치우선순위)
//			ChangeRankAnimation* b = ChangeRankAnimation::create(t_hsp->getKakaoProfileURL(),mySGD->getScore(),recent_my_rank,t_hsp->getKakaoNickname(),
//																 next_rank_info.img_url, next_rank_info.score, recent_my_rank+1, next_rank_info.nickname,
//																 -200);
//			
//			
//			b->setCancelFunc([](){
//				CCLOG("닫기눌렀을때");
//			});
//			b->setCloseFunc([=](){
//				CCLOG("창 다닫혔을때");
//				checkMiniGame();
//			});
//			b->setSendFunc([=](){
//				CCLOG("확인버튼눌렀을때->메세지전송");
//				Json::Value p;
//				Json::Value contentJson;
//				contentJson["msg"] = (next_rank_info.nickname + "님에게 도전!");
//				contentJson["challengestage"] = mySD->getSilType();
//				contentJson["score"] = mySGD->getScore();
//				contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//				contentJson["replaydata"] = mySGD->replay_write_info;
//				contentJson["profile"] = t_hsp->getKakaoProfileURL();
//				p["content"] = GraphDogLib::JsonObjectToString(contentJson);
//				std::string recvId = next_rank_info.user_id;
//				p["receiverMemberID"] = recvId;
//				
//				p["senderMemberID"] = hspConnector::get()->getSocialID();
//				
//				p["type"] = kChallengeRequest;
//				hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
//											 {
//												 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//												 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//												 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//												 
//												 //																		setHelpSendTime(recvId);
//												 if(r["result"]["code"].asInt() != GDSUCCESS)
//												 {
//													 // 에러.
//													 return;
//												 }
//												 
//												 setChallengeSendTime(next_rank_info.user_id);
//												 //																	 friend_list.erase(friend_list.begin() + tag);
//												 GraphDogLib::JsonToLog("sendMessage", r);
//												 
//												 //																		obj->removeFromParent();
//												 
////												 Json::Value p2;
////												 p2["receiver_id"] = recvId;
////												 p2["message"] = "도전을 신청한다!!";
////												 hspConnector::get()->kSendMessage
////												 (p2, [=](Json::Value r)
////												  {
////													  GraphDogLib::JsonToLog("kSendMessage", r);
////												  });
//											 });
//			});
//			
//			b->start();
//			
//			addChild(b,kZ_CP_popup);
//		}
//		else
//		{
			checkMiniGame();
//		}
//	}
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_homeClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_homeClick);
		addChild(t_tutorial, kZ_CP_popup);
		
		tutorial_node = t_tutorial;
	}
}

void ClearPopup::endTakeCard()
{
	is_end_take_card = true;
//	if(is_saved_user_data)
//	{
//		for(int i=0;i<cell_action_list.size();i++)
//			cell_action_list[i]();
//	}
	
	CCScaleTo* title_scale1 = CCScaleTo::create(1.f/6.f, 1.1f);
	CCScaleTo* title_scale2 = CCScaleTo::create(1.f/12.f, 1.f);
	CCSequence* title_seq = CCSequence::create(title_scale1, title_scale2, NULL);
	title->runAction(title_seq);
	
	int step_count = ani_stars.size();
	
	if(step_count >= 1)
	{
		for(int i=0;i<step_count-1;i++)
		{
			ani_stars[i]->addChild(KSTimer::create(0.5f+i*0.2f, [=](){
				starSound();
				ani_stars[i]->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t){
					ani_stars[i]->setScale(2.f-t);
					ani_stars[i]->setRotation(-200+t*200);
					ani_stars[i]->setOpacity(t*255);
				}, [=](float t){
					ani_stars[i]->setScale(1.f);
					ani_stars[i]->setRotation(0);
					ani_stars[i]->setOpacity(255);
					
					CCSprite* t_star_ani = (CCSprite*)ani_stars[i]->getChildByTag(1);
					t_star_ani->setOpacity(255);
					
					ani_stars[i]->addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t){
						t_star_ani->setScale(2.f-t);
						t_star_ani->setOpacity(255-t*55);
					}, [=](float t){
						addStarParticle(ani_stars[i]);
						t_star_ani->setOpacity(0);
						
						CCSprite* back_star = CCSprite::create("ending_star_gold.png");
						back_star->setPosition(ani_stars[i]->getPosition());
						main_case->addChild(back_star, kZ_CP_img);
					}));
				}));
			}));
		}
		
		ani_stars[step_count-1]->addChild(KSTimer::create(0.5f+(step_count-1)*0.2f, [=](){
			starSound();
			ani_stars[step_count-1]->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t){
				ani_stars[step_count-1]->setScale(2.f-t);
				ani_stars[step_count-1]->setRotation(-200+t*200);
				ani_stars[step_count-1]->setOpacity(t*255);
			}, [=](float t){
				ani_stars[step_count-1]->setScale(1.f);
				ani_stars[step_count-1]->setRotation(0);
				ani_stars[step_count-1]->setOpacity(255);
				
				CCSprite* t_star_ani = (CCSprite*)ani_stars[step_count-1]->getChildByTag(1);
				t_star_ani->setOpacity(255);
				
				ani_stars[step_count-1]->addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t){
					t_star_ani->setScale(2.f-t);
					t_star_ani->setOpacity(255-t*55);
				}, [=](float t){
					addStarParticle(ani_stars[step_count-1]);
					t_star_ani->setOpacity(0);
					
					CCSprite* back_star = CCSprite::create("ending_star_gold.png");
					back_star->setPosition(ani_stars[step_count-1]->getPosition());
					main_case->addChild(back_star, kZ_CP_img);
					
					startCalcAnimation();
//					checkRentCard();
				}));
			}));
		}));
	}
	else
	{
		closePopup();
//		checkRentCard();
	}
}

void ClearPopup::starSound()
{
	AudioEngine::sharedInstance()->playEffect("se_resultstar.mp3", false);
}

void ClearPopup::checkMiniGame()
{
//	bool is_minigame_stage = NSDS_GB(mySD->getSilType(), kSDS_SI_minigame_b);
//	if(is_minigame_stage && !myDSH->getBoolForKey(kDSH_Key_minigame_int1_isPlayed, mySD->getSilType()))
//	{
//		int minigame_played_cnt = myDSH->getIntegerForKey(kDSH_Key_minigame_playedCnt)+1;
//		myDSH->setIntegerForKey(kDSH_Key_minigame_playedCnt, minigame_played_cnt, false);
//		myDSH->setIntegerForKey(kDSH_Key_minigame_int1_stageNumber, minigame_played_cnt, mySD->getSilType(), false);
//		myDSH->setBoolForKey(kDSH_Key_minigame_int1_isPlayed, mySD->getSilType(), true, false);
//		myDSH->fFlush();
//		myDSH->saveUserData({kSaveUserData_Key_minigame}, nullptr);
//		MiniGamePopup* t_popup = MiniGamePopup::create(kMiniGameCode_touchtouch, bind(&ClearPopup::closePopup, this));
//		addChild(t_popup, kZ_CP_popup);
//	}
//	else
//	{
		closePopup();
//	}
}

void ClearPopup::closePopup()
{
	is_end_popup_animation = true;
	if(is_end_popup_animation && is_saved_user_data)// && is_loaded_list)
	{
		is_menu_enable = true;
	}
}

void ClearPopup::checkRentCard()
{
//	if(mySGD->getWasUsedFriendCard())
//	{
//		ASPopupView* t_popup = ASPopupView::create(-200);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		float height_value = 320.f;
//		if(myDSH->screen_convert_rate < 1.f)
//			height_value = 320.f/myDSH->screen_convert_rate;
//		
//		if(height_value < myDSH->ui_top)
//			height_value = myDSH->ui_top;
//		
//		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
//		t_popup->setDimmedPosition(ccp(240, 160));
//		t_popup->setBasePosition(ccp(240, 160));
//		
//		CCNode* t_container = CCNode::create();
//		t_popup->setContainerNode(t_container);
//		addChild(t_popup, kZ_CP_popup);
//		
//		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//		case_back->setPosition(CCPointZero);
//		t_container->addChild(case_back);
//		
//		case_back->setContentSize(CCSizeMake(260, 260));
//		
//		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//		content_back->setPosition(ccp(0,5));
//		t_container->addChild(content_back);
//		
//		content_back->setContentSize(CCSizeMake(240, 160));
//		
//		CCSprite* title_img = CCSprite::create("message_title_rentcard_thanks.png");
//		title_img->setPosition(ccp(0,107));
//		t_container->addChild(title_img);
//		
//		CCLabelTTF* ment1_label = CCLabelTTF::create(CCString::createWithFormat("%s님의", mySGD->getSelectedFriendCardData().nick.c_str())->getCString(),
//													mySGD->getFont().c_str(), 15);
//		ment1_label->setPosition(ccp(0,55));
//		t_container->addChild(ment1_label);
//		
//		CCLabelTTF* ment2_label = CCLabelTTF::create("카드가 도움이 되었나요?", mySGD->getFont().c_str(), 15);
//		ment2_label->setPosition(ccp(0,25));
//		t_container->addChild(ment2_label);
//		
//		CCLabelTTF* ment3_label = CCLabelTTF::create("하트으로 고마움을 표현하세요.", mySGD->getFont().c_str(), 15);
//		ment3_label->setPosition(ccp(0,-5));
//		t_container->addChild(ment3_label);
//		
//		CCLabelTTF* ment4_label = CCLabelTTF::create("(하트을 선물하면 ", mySGD->getFont().c_str(), 12);
//		ment4_label->setAnchorPoint(ccp(1,0.5));
//		ment4_label->setPosition(ccp(-10,-40));
//		t_container->addChild(ment4_label);
//		
//		CCLabelTTF* ment5_label = CCLabelTTF::create(CCString::createWithFormat("소셜포인트 +%d", mySGD->getSPRentCardThanks())->getCString(), mySGD->getFont().c_str(), 12);
//		ment5_label->setColor(ccYELLOW);
//		ment5_label->setAnchorPoint(ccp(0,0.5));
//		ment5_label->setPosition(ccp(-10,-40));
//		t_container->addChild(ment5_label);
//		
//		CCLabelTTF* ment6_label = CCLabelTTF::create(" 드려요.)", mySGD->getFont().c_str(), 12);
//		ment6_label->setAnchorPoint(ccp(0,0.5));
//		ment6_label->setPosition(ccp(ment5_label->getPositionX()+ment5_label->getContentSize().width,-40));
//		t_container->addChild(ment6_label);
//		
//		
//		CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-1);
//		close_button->setPosition(ccp(111,107));
//		close_button->setFunction([=](CCObject* sender)
//								  {
//									  checkChallengeOrHelp();
//									  t_popup->removeFromParent();
//								  });
//		t_container->addChild(close_button);
//		
//		
//		CommonButton* send_coin_button = CommonButton::create("선물하기", 15, CCSizeMake(100,50), CommonButtonOrange, t_popup->getTouchPriority()-1);
//		send_coin_button->setPosition(ccp(0,-102));
//		send_coin_button->setFunction([=](CCObject* sender)
//									  {
//											Json::Value p;
//											Json::Value contentJson;
//
//											contentJson["msg"] = "하트받으쇼~";
//											contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//											p["content"] = GraphDogLib::JsonObjectToString(contentJson);
//											p["receiverMemberID"] = mySGD->getSelectedFriendCardData().userId;
//											p["senderMemberID"]=hspConnector::get()->getSocialID();
//											p["type"]=kHeart;
//											p["nickname"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//
//											hspConnector::get()->command("sendMessage", p, this,[=](Json::Value r) {
//												//		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//												//		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//												//		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//												GraphDogLib::JsonToLog("sendMessage", r);
//												if(r["result"]["code"].asInt() != GDSUCCESS){
//													return;
//												}
//												mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPRentCardThanks());
//												checkChallengeOrHelp();
//												t_popup->removeFromParent();
//											});
//									  });
//		t_container->addChild(send_coin_button);
//	}
//	else
	{
		checkChallengeOrHelp();
	}
}

//void ClearPopup::resultLoadFriends(Json::Value result_data)
//{
//	Json::Value p;
//	
//	p["stageNo"]=mySD->getSilType();
//	hspConnector::get()->command("getstagescorelist",p,json_selector(this, ClearPopup::resultGetStageScoreList));
//}

//void ClearPopup::resultGetStageScoreList(Json::Value result_data)
//{
//	if(result_data["result"]["code"].asInt() == GDSUCCESS)
//	{
//		Json::Value score_list = result_data["list"];
//		for(int i=0;i<score_list.size();i++)
//		{
//			if(score_list[i]["memberID"].asString() == hspConnector::get()->getSocialID())
//			{
//				if(mySGD->getScore() > score_list[i]["score"].asFloat())
//				{
//					CCSprite* high_score_img = CCSprite::create("ending_highscore.png");
//					high_score_img->setPosition(ccp(105, 78));
//					main_case->addChild(high_score_img, kZ_CP_img);
//				}
//			}
//			
//			vector<RankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString().c_str());
//			if(iter != friend_list.end())
//			{
//				(*iter).score = score_list[i]["score"].asFloat();
//				(*iter).is_play = true;
//			}
//			else
//				CCLOG("not found friend memberID");
//		}
//		
//		auto beginIter = std::remove_if(friend_list.begin(), friend_list.end(), [=](RankFriendInfo t_info)
//										{
//											return !t_info.is_play;
//										});
//		friend_list.erase(beginIter, friend_list.end());
//		
//		struct t_FriendSort{
//			bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
//			{
//				return a.score > b.score;
//			}
//		} pred;
//		
//		sort(friend_list.begin(), friend_list.end(), pred);
//		
//		for(int i=0;i<friend_list.size();i++)
//			friend_list[i].rank = i+1;
//		
//		// create cell
//		
//		//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 195, 176));
//		//		temp_back->setAnchorPoint(CCPointZero);
//		//		temp_back->setOpacity(100);
//		//		temp_back->setPosition(ccp(246, 65));
//		//		addChild(temp_back, kZ_CP_menu);
//		
//		rankTableView = CCTableView::create(this, CCSizeMake(208, 199));
//		
//		rankTableView->setAnchorPoint(CCPointZero);
//		rankTableView->setDirection(kCCScrollViewDirectionVertical);
//		rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
//		rankTableView->setPosition(ccp(243, 59.5f));
//		
//		rankTableView->setDelegate(this);
//		main_case->addChild(rankTableView, kZ_CP_table);
//		rankTableView->setTouchPriority(-200);
//		
//		//		int myPosition = rankTableView->minContainerOffset().y;
//		//		for(int i=0; i<friend_list.size(); i++)
//		//		{
//		//			if(friend_list[i].user_id == hspConnector::get()->getSocialID())
//		//			{
//		//				myPosition = friend_list.size() - i - 1;
//		//				break;
//		//			}
//		//		}
//		//		float yInitPosition = MAX(rankTableView->minContainerOffset().y, -cellSizeForTable(rankTableView).height*myPosition + rankTableView->getViewSize().height / 2.f);
//		//		yInitPosition = MIN(0, yInitPosition);
//		//		rankTableView->setContentOffsetInDuration(ccp(0, yInitPosition), 0.7f);
//	}
//	is_loaded_list = true;
//	endLoad();
//}

void ClearPopup::resultSavedUserData(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		is_saved_user_data = true;
		endLoad();
//		if(is_loaded_list)
//		{
//			rankTableView = CCTableView::create(this, CCSizeMake(208, 199));
//			
//			rankTableView->setAnchorPoint(CCPointZero);
//			rankTableView->setDirection(kCCScrollViewDirectionVertical);
//			rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
//			rankTableView->setPosition(ccp(243, 59.5f));
//			
//			rankTableView->setDelegate(this);
//			main_case->addChild(rankTableView, kZ_CP_table);
//			rankTableView->setTouchPriority(-200);
//		}
//		else
//		{
//			resultLoadFriends(Json::Value());
//		}
		
//		hspConnector::get()->kLoadFriends(json_selector(this, ClearPopup::resultLoadFriends));
	}
	else
	{
		myDSH->saveAllUserData(json_selector(this, ClearPopup::resultSavedUserData));
	}
}

void ClearPopup::endLoad()
{
	if(is_end_popup_animation && is_saved_user_data)// && is_loaded_list)
	{
		if(mySGD->getPlayCountHighIsOn() != 0 && mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_eventRubyShop) && mySGD->getUserdataTotalPlayCount() >= mySGD->getPlayCountHighValue())
		{
			EventShopPopup* t_popup = EventShopPopup::create(-300, [=](){});
			addChild(t_popup, kZ_CP_popup);
		}
		
		is_menu_enable = true;
	}
}

void ClearPopup::onEnter()
{
	CCLayer::onEnter();
	showPopup();
}

void ClearPopup::startCalcAnimation()
{
//	startTimeAnimation();
//	startGoldAnimation();
	
	KSLabelTTF* star_bonus_label = KSLabelTTF::create(CCString::createWithFormat("별보너스 x%.1f", mySGD->getStageGrade()*0.5f+1.f)->getCString(), mySGD->getFont().c_str(), 15);
	star_bonus_label->setColor(ccBLUE);
	star_bonus_label->enableOuterStroke(ccBLACK, 2.f);
	star_bonus_label->setPosition(ccp(53+48+24,195));
	main_case->addChild(star_bonus_label, kZ_CP_table);
	
	star_bonus_label->setScale(0.5f);
	star_bonus_label->setOpacity(0);
	
	main_case->addChild(KSGradualValue<float>::create(0.f, 1.f, 9.f/30.f, [=](float t){
		star_bonus_label->setScale(0.5f + t*0.8f);
		star_bonus_label->setOpacity(t*255);
	}, [=](float t){
		star_bonus_label->setScale(1.3f);
		star_bonus_label->setOpacity(255);
		
		main_case->addChild(KSTimer::create(5.f/30.f, [=](){
			main_case->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t){
				star_bonus_label->setScale(1.3f-t*0.8f);
				star_bonus_label->setPosition(ccpAdd(ccp(125,195), ccpMult(ccp(65,-113), t)));
				
			}, [=](float t){
				star_bonus_label->setScale(0.5f);
				star_bonus_label->setPosition(ccp(190,82));
			}));
		}));
		
		main_case->addChild(KSTimer::create(9.f/30.f, [=](){
			main_case->addChild(KSGradualValue<float>::create(1.f, 0.f, 4.f/30.f, [=](float t){
				star_bonus_label->setOpacity(t*255);
			}, [=](float t){
				star_bonus_label->setOpacity(0);
				is_end_call_score_calc = true;
				end_score_calc_func = [=]()
				{
					KSLabelTTF* time_bonus_label = KSLabelTTF::create("타임 보너스", mySGD->getFont().c_str(), 15);
					time_bonus_label->setColor(ccBLUE);
					time_bonus_label->enableOuterStroke(ccBLACK, 2.f);
					time_bonus_label->setPosition(ccp(170,148)); // 170 148
					main_case->addChild(time_bonus_label, kZ_CP_table);
					
					time_bonus_label->setScale(0.5f);
					time_bonus_label->setOpacity(0);
					
					main_case->addChild(KSGradualValue<float>::create(0.f, 1.f, 9.f/30.f, [=](float t){
						time_bonus_label->setScale(0.5f + t*0.8f);
						time_bonus_label->setOpacity(t*255);
					}, [=](float t){
						time_bonus_label->setScale(1.3f);
						time_bonus_label->setOpacity(255);
						
						main_case->addChild(KSTimer::create(5.f/30.f, [=](){
							main_case->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t){
								time_bonus_label->setScale(1.3f-t*0.8f);
								time_bonus_label->setPosition(ccpAdd(ccp(170,148), ccpMult(ccp(20,-66), t)));
								
							}, [=](float t){
								time_bonus_label->setScale(0.5f);
								time_bonus_label->setPosition(ccp(190,82));
							}));
						}));
						
						main_case->addChild(KSTimer::create(9.f/30.f, [=](){
							main_case->addChild(KSGradualValue<float>::create(1.f, 0.f, 4.f/30.f, [=](float t){
								time_bonus_label->setOpacity(t*255);
							}, [=](float t){
								time_bonus_label->setOpacity(0);
								is_end_popup_animation = true;
								is_end_call_score_calc = true;
								end_score_calc_func = [=]()
								{
									float delay_time = 0;
									if(is_saved_user_data)
									{
										delay_time = 0.5f;
										for(int i=0;i<cell_action_list.size();i++)
											cell_action_list[i]();
									}
									
									KS::setOpacity(replay_menu, 0);
									KS::setOpacity(ok_menu, 0);
									
									replay_menu->setVisible(true);
									ok_menu->setVisible(true);
									
									CCPoint replay_origin_position = replay_menu->getPosition();
									replay_menu->setPosition(replay_origin_position + ccp(0,-12));
									main_case->addChild(KSGradualValue<float>::create(0.f, 1.f, 7.f/30.f, [=](float t){
										replay_menu->setPosition(replay_origin_position + ccpMult(ccp(0,-12), 1.f-t));
										KS::setOpacity(replay_menu, t*255);
									}, [=](float t){
										replay_menu->setPosition(replay_origin_position);
										KS::setOpacity(replay_menu, 255);
									}));
									
									CCPoint ok_origin_position = ok_menu->getPosition();
									ok_menu->setPosition(ok_origin_position + ccp(0,-12));
									main_case->addChild(KSTimer::create(4.f/30.f, [=](){
										main_case->addChild(KSGradualValue<float>::create(0.f, 1.f, 7.f/30.f, [=](float t){
											ok_menu->setPosition(ok_origin_position + ccpMult(ccp(0,-12), 1.f-t));
											KS::setOpacity(ok_menu, t*255);
										}, [=](float t){
											ok_menu->setPosition(ok_origin_position);
											KS::setOpacity(ok_menu, 255);
											closePopup();
										}));
									}));
								};
								
								float play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
								base_score = mySGD->getBaseScore() + mySGD->getBaseScore()*(mySGD->getStageGrade()*0.5f);
								startScoreAnimation(mySGD->getBaseScore()*((play_limit_time-mySGD->getGameTime())/play_limit_time));
							}));
						}));
					}));
				};
				base_score = mySGD->getBaseScore();
				startScoreAnimation(mySGD->getBaseScore()*(mySGD->getStageGrade()*0.5f));
			}));
		}));
	}));
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
		int decreaseUnit = keep_gold / 1.f * dt;
		
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
}

void ClearPopup::startScoreAnimation(float t_score)
{
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
	keep_score = t_score;
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
		score_label->setString(KS::insert_separator(CCString::createWithFormat("%.0f",base_score + increase_score)->getCString()).c_str());
	}
	else
		stopScoreAnimation();
}

void ClearPopup::stopScoreAnimation()
{
	unschedule(schedule_selector(ClearPopup::scoreAnimation));
	score_label->setString(KS::insert_separator(CCString::createWithFormat("%.0f", base_score + keep_score)->getCString()).c_str());
	AudioEngine::sharedInstance()->stopAllEffects();
	
	if(is_end_call_score_calc)
		end_score_calc_func();
}

void ClearPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	AudioEngine::sharedInstance()->stopAllEffects();
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_homeClick)
	{
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kMT_CP_ok)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_pieceType);
			is_menu_enable = false;
			AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
			//		mySGD->resetLabels();
			hidePopup();
			removeChild(tutorial_node);
		}
	}
	else
	{
		is_menu_enable = false;
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kMT_CP_ok)
		{
			if(is_go_to_mainflow)
			{
				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_timeOutFrame)), 9999);
			}
			
			AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
			AudioEngine::sharedInstance()->playSound("bgm_ui.mp3", true);
			//		mySGD->resetLabels();
			
			if(is_take_star_effect)
			{
				CCPoint star_origin_position = ccp(25,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-22);
				
				CCNode* t_star_node = CCNode::create();
				t_star_node->setPosition(star_origin_position + ccp(0,150));
				addChild(t_star_node, kZ_CP_popup);
				
				CCSprite* total_star = KS::loadCCBI<CCSprite*>(this, "main_star.ccbi").first;
				total_star->setPosition(ccp(0,0));
				t_star_node->addChild(total_star);
				
				KSLabelTTF* star_count = KSLabelTTF::create(CCString::createWithFormat("%d", mySGD->getClearStarCount()-mySGD->getStageGrade())->getCString(), mySGD->getFont().c_str(), 12);
				star_count->enableOuterStroke(ccBLACK, 0.8f);
				star_count->setPosition(ccp(0,0));
				t_star_node->addChild(star_count);
				
				addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t){
					t_star_node->setPosition(star_origin_position + ccpMult(ccp(0,150), 1.f-t));
				}, [=](float t){
					t_star_node->setPosition(star_origin_position);
					
					float delay_time = 0.f;
					if(ani_stars.size() > 0)
						delay_time = 13.f + (ani_stars.size()-1)*19.f;
					else
						delay_time = 30.f;
					
					addChild(KSTimer::create(delay_time/30.f, [=]()
					{
						addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t){
							t_star_node->setScale(1.f+t*0.2f);
						}, [=](float t){
							t_star_node->setScale(1.2f);
							addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t){
								t_star_node->setScale(1.2f - t*0.2f);
							}, [=](float t){
								t_star_node->setScale(1.f);
								addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t){
									t_star_node->setPosition(star_origin_position + ccpMult(ccp(0,150), 1.f-t));
								}, [=](float t){
									t_star_node->setPosition(star_origin_position + ccp(0,150));
									hidePopup();
								}));
							}));
						}));
					}));
					
					for(int i=0;i<ani_stars.size();i++)
					{
						ani_stars[i]->setOpacity(0);
						CCSprite* t_star_ani = (CCSprite*)ani_stars[i]->getChildByTag(1);
						t_star_ani->setOpacity(255);
						t_star_ani->setScale(1.f);
						
						CCPoint origin_position = ani_stars[i]->getPosition();
						CCPoint sub_position = ccp(25,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-42) - ani_stars[i]->getPosition();
						CCPoint dis_position;
						
						if(i == 0)
						{
							dis_position = ccp(-20,0);
							
							addChild(KSGradualValue<float>::create(-1.f, 1.f, 6.f/30.f, [=](float t){
								t_star_ani->setScale(1.2f-fabsf(t)*0.2f);
							}, [=](float t){
								t_star_ani->setScale(1.f);
								
								addChild(KSGradualValue<float>::create(0.f, 1.f, 9.f/30.f, [=](float t){
									if(t < 0.5f)
										ani_stars[i]->setPosition(origin_position + ccpMult(sub_position,t) + ccpMult(dis_position, t*2.f));
									else
										ani_stars[i]->setPosition(origin_position + ccpMult(sub_position,t) + ccpMult(dis_position, (1.f-t)*2.f));
//									ani_stars[i]->setPosition(origin_position + ccpMult(sub_position,t));
								}, [=](float t){
									ani_stars[i]->setPosition(ccp(25,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-42));
									addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t){
										t_star_ani->setScale(1.f + t*0.3f);
										t_star_ani->setOpacity(255-t*255);
									}, [=](float t){
										t_star_ani->setScale(1.3f);
										t_star_ani->setOpacity(0);
										star_count->setString(CCString::createWithFormat("%d", mySGD->getClearStarCount()-mySGD->getStageGrade()+i+1)->getCString());
									}));
								}));
								
								addChild(KSGradualValue<float>::create(0.f, 300, 15.f/30.f, [=](float t){
									t_star_ani->setRotation(t);
								}, [=](float t){
									t_star_ani->removeFromParent();
								}));
								
								
							}));
						}
						else
						{
							if(i == 1)
								dis_position = ccp(-10,10);
							else if(i == 2)
								dis_position = ccp(10,10);
							else if(i == 3)
								dis_position = ccp(20,20);
							
							addChild(KSTimer::create(13.f/30.f + (i-1)*19.f/30.f, [=](){
								addChild(KSGradualValue<float>::create(-1.f, 1.f, 6.f/30.f, [=](float t){
									t_star_ani->setScale(1.2f-fabsf(t)*0.2f);
								}, [=](float t){
									t_star_ani->setScale(1.f);
									
									addChild(KSGradualValue<float>::create(0.f, 1.f, 9.f/30.f, [=](float t){
										if(t < 0.5f)
											ani_stars[i]->setPosition(origin_position + ccpMult(sub_position,t) + ccpMult(dis_position, t*2.f));
										else
											ani_stars[i]->setPosition(origin_position + ccpMult(sub_position,t) + ccpMult(dis_position, (1.f-t)*2.f));
//										ani_stars[i]->setPosition(origin_position + ccpMult(sub_position,t));
									}, [=](float t){
										ani_stars[i]->setPosition(ccp(25,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-42));
										addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t){
											t_star_ani->setScale(1.f + t*0.3f);
											t_star_ani->setOpacity(255-t*255);
										}, [=](float t){
											t_star_ani->setScale(1.3f);
											t_star_ani->setOpacity(0);
											star_count->setString(CCString::createWithFormat("%d", mySGD->getClearStarCount()-mySGD->getStageGrade()+i+1)->getCString());
										}));
									}));
									
									addChild(KSGradualValue<float>::create(0.f, 300, 15.f/30.f, [=](float t){
										t_star_ani->setRotation(t);
									}, [=](float t){
										t_star_ani->removeFromParent();
									}));
									
									
								}));
							}));
						}
					}
					
				}));
			}
			else
			{
				hidePopup();
			}
		}
		else if(tag == kMT_CP_replay)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
			//		mySGD->resetLabels();
			
			if(mySD->getSilType() >= 10000)
				mySGD->is_before_selected_event_stage = true;
			
			addChild(KSGradualValue<float>::create(1.f, 0.f, 0.5f, [=](float t)
												   {
													   gray->setOpacity(t*255);
													   main_case->setPosition(ccp(240,160-(450-450*t)));
												   }, [=](float t)
												   {
													   gray->setOpacity(0);
													   main_case->setPosition(ccp(240,160-450));
													   replay_func();
													   removeFromParent();
												   }));
//			CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
			//		StageSettingPopup* t_popup = StageSettingPopup::create();
			//		t_popup->setHideFinalAction(target_final, delegate_final);
			//		getParent()->addChild(t_popup, kPMS_Z_popup);
			//
			//		target_final = NULL;
			//		hidePopup();
		}
	}
}

void ClearPopup::cellAction( CCObject* sender )
{

}

CCTableViewCell* ClearPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
//	CCLabelTTF* nickname_label;
//	CCLabelTTF* score_label;
//	RankFriendInfo* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();

//	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
//	profileImg->setAnchorPoint(ccp(0.5, 0.5));
//	profileImg->setTag(kCFC_T_img);
//	profileImg->setPosition(ccp(52, 22));
//	cell->addChild(profileImg, -1);
//	
//	string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
//	string cell_id = (*member).user_id;
//	
//	if(my_id != cell_id && KnownFriends::getInstance()->findById(cell_id) != nullptr)
//	{
//		CCSprite* kakao_sign = CCSprite::create("puzzle_right_rank_kakao.png");
//		kakao_sign->setPosition(ccp(41,29));
//		cell->addChild(kakao_sign, kCFC_Z_img);
//	}
//	
//	CCSprite* bg;
//	if((*member).user_id == hspConnector::get()->getSocialID())
//	{
//		bg = CCSprite::create("ending_cell_me.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kCFC_Z_case);
//		
//		if(idx == 0)
//		{
//			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_gold.png");
//			rank_img->setPosition(ccp(21,21));
//			rank_img->setTag(kCFC_T_rank);
//			cell->addChild(rank_img, kCFC_Z_img);
//		}
//		else if(idx == 1)
//		{
//			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_silver.png");
//			rank_img->setPosition(ccp(21,21));
//			rank_img->setTag(kCFC_T_rank);
//			cell->addChild(rank_img, kCFC_Z_img);
//		}
//		else if(idx == 2)
//		{
//			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_bronze.png");
//			rank_img->setPosition(ccp(21,21));
//			rank_img->setTag(kCFC_T_rank);
//			cell->addChild(rank_img, kCFC_Z_img);
//		}
//		else
//		{
//			CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 15);
//			rank_label->setPosition(ccp(21,21));
//			rank_label->setTag(kCFC_T_rank);
//			cell->addChild(rank_label,kCFC_Z_img);
//		}
//	}
//	else if(idx == 0)
//	{
//		bg = CCSprite::create("ending_cell_gold.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kCFC_Z_case);
//		
//		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_gold.png");
//		rank_img->setPosition(ccp(21,21));
//		rank_img->setTag(kCFC_T_rank);
//		cell->addChild(rank_img, kCFC_Z_img);
//	}
//	else if(idx == 1)
//	{
//		bg = CCSprite::create("ending_cell_silver.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kCFC_Z_case);
//		
//		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_silver.png");
//		rank_img->setPosition(ccp(21,21));
//		rank_img->setTag(kCFC_T_rank);
//		cell->addChild(rank_img, kCFC_Z_img);
//	}
//	else if(idx == 2)
//	{
//		bg = CCSprite::create("ending_cell_bronze.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kCFC_Z_case);
//		
//		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_bronze.png");
//		rank_img->setPosition(ccp(21,21));
//		rank_img->setTag(kCFC_T_rank);
//		cell->addChild(rank_img, kCFC_Z_img);
//	}
//	else
//	{
//		bg = CCSprite::create("ending_cell_normal.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kCFC_Z_case);
//		
//		CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 15);
//		rank_label->setPosition(ccp(21,21));
//		rank_label->setTag(kCFC_T_rank);
//		cell->addChild(rank_label,kCFC_Z_img);
//	}
//	
//	nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
//	nickname_label->enableStroke(ccBLACK, 0.5f);
//	nickname_label->setPosition(ccp(130,28));
//	nickname_label->setTag(kCFC_T_nickname);
//	cell->addChild(nickname_label,kCFC_Z_img);
//	
//	score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 12);
//	score_label->setColor(ccBLACK);
//	score_label->setPosition(ccp(130,13));
//	score_label->setTag(kCFC_T_score);
//	cell->addChild(score_label,kCFC_Z_img);

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
	//		CCLOG("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize ClearPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(210, 45);
}

unsigned int ClearPopup::numberOfCellsInTableView( CCTableView *table )
{
	return 0;//friend_list.size();
}

CCParticleSystemQuad* ClearPopup::getStarParticle()
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

void ClearPopup::addStarParticle(CCNode* t_node)
{
	CCParticleSystemQuad* t_particle = getStarParticle();
	t_particle->setPosition(t_node->getContentSize().width/2.f, t_node->getContentSize().height/2.f);
	t_node->addChild(t_particle, 1, 2);
}
