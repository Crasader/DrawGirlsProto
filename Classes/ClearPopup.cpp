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
#include "ASPopupView.h"
#include "StartSettingScene.h"
#include "MiniGamePopup.h"
#include "TutorialFlowStep.h"
#include "AchieveNoti.h"
#include "SendMessageUtil.h"
#include "CardAnimations.h"

typedef enum tMenuTagClearPopup{
	kMT_CP_ok = 1,
	kMT_CP_replay,
	kMT_CP_noti
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
	
	AudioEngine::sharedInstance()->preloadEffectScene("Ending");
	
	if(myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep) == kTutorialFlowStep_ingame)
		myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_homeClick);
	
	is_menu_enable = false;
	is_loaded_list = false;
	
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
	hspConnector::get()->command("addweeklyscore", p1, NULL);
	
	
	is_rank_changed = false;
	if(mySGD->save_stage_rank_stageNumber == mySD->getSilType())
	{
		// 받아왔던 rank list 사용
		string my_id = hspConnector::get()->getKakaoID();
		
		vector<RankFriendInfo>::iterator iter = find(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), my_id);
		if(iter != mySGD->save_stage_rank_list.end())
		{
			// found
			if((*iter).score < mySGD->getScore())
			{
				Json::Value p;
				p["memberID"]=hspConnector::get()->getKakaoID();
				p["score"]=int(mySGD->getScore());
				p["stageNo"]=mySD->getSilType();
				hspConnector::get()->command("setStageScore",p,NULL);
				
				
				before_my_rank = (*iter).rank;
				(*iter).score = mySGD->getScore();
				struct t_FriendSort{
					bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
					{
						return a.score > b.score;
					}
				} pred;
				
				sort(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), pred);
				
				for(int i=0;i<mySGD->save_stage_rank_list.size();i++)
					mySGD->save_stage_rank_list[i].rank = i+1;
				
				vector<RankFriendInfo>::iterator iter2 = find(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), my_id);
				if((*iter2).rank < before_my_rank)
				{
					is_rank_changed = true;
					recent_my_rank = (*iter2).rank;
					next_rank_info = (*(iter2+1));
				}
			}
		}
		else
		{
			CCLog("not found myKakaoID");
			
			Json::Value p;
			p["memberID"]=hspConnector::get()->getKakaoID();
			p["score"]=int(mySGD->getScore());
			p["stageNo"]=mySD->getSilType();
			hspConnector::get()->command("setStageScore",p,NULL);
			
			Json::Value my_kakao = hspConnector::get()->myKakaoInfo;
			
			RankFriendInfo fInfo;
			fInfo.nickname = my_kakao["nickname"].asString();
			fInfo.img_url = my_kakao["profile_image_url"].asString();
			fInfo.user_id = my_kakao["user_id"].asString();
			fInfo.score = mySGD->getScore();
			fInfo.is_play = true;
			mySGD->save_stage_rank_list.push_back(fInfo);
			
			struct t_FriendSort{
				bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
				{
					return a.score > b.score;
				}
			} pred;
			
			sort(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), pred);
			
			for(int i=0;i<mySGD->save_stage_rank_list.size();i++)
				mySGD->save_stage_rank_list[i].rank = i+1;
			
			vector<RankFriendInfo>::iterator iter2 = find(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), my_id);
			if((*iter2).rank < mySGD->save_stage_rank_list.size())
			{
				is_rank_changed = true;
				recent_my_rank = (*iter2).rank;
				next_rank_info = (*(iter2+1));
			}
		}
		
		friend_list = mySGD->save_stage_rank_list;
		is_loaded_list = true;
	}
	else
	{
		Json::Value p;
		p["memberID"]=hspConnector::get()->getKakaoID();
		p["score"]=int(mySGD->getScore());
		p["stageNo"]=mySD->getSilType();
		hspConnector::get()->command("setStageScore",p,NULL);
	}
    
	main_case = CCSprite::create("ending_back.png");
	main_case->setAnchorPoint(ccp(0,0));
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kZ_CP_back);
	
		
	CCSprite* title = CCSprite::create("ending_clear.png");
	title->setPosition(ccp(131,233.5f));
	main_case->addChild(title, kZ_CP_img);
	
	if(mySGD->is_before_selected_event_stage)
	{
		int stage_number = mySD->getSilType();
		
		CCLabelTTF* stage_number_label = CCLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(),	mySGD->getFont().c_str(), 10);
		stage_number_label->setPosition(ccp(60, main_case->getContentSize().height-65));
		main_case->addChild(stage_number_label, kZ_CP_img);
		
		mySGD->is_before_selected_event_stage = false;
	}
	else
	{
		int stage_number = mySD->getSilType();
		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
		int piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		CCLabelTTF* piece_number_label = CCLabelTTF::create(CCString::createWithFormat("%d-%d", puzzle_number, piece_number)->getCString(),	mySGD->getFont().c_str(), 10);
		piece_number_label->setPosition(ccp(60, main_case->getContentSize().height-65));
		main_case->addChild(piece_number_label, kZ_CP_img);
	}
	
	
	///////////////////////////// 딤드 로 팝업 띄움
	int stage_number = mySD->getSilType();
	int take_level;
	if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 3;
	else if(mySGD->is_exchanged || mySGD->is_showtime)	take_level = 2;
	else												take_level = 1;
	
	int take_card_number = NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, take_level);
	
	if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, take_card_number) > 0)
	{
		// 강화
		
		float strength_rate = ((NSDS_GI(kSDS_CI_int1_rank_i, take_card_number)*10.f + 1)*NSDS_GI(kSDS_CI_int1_durability_i, take_card_number))/((NSDS_GI(kSDS_CI_int1_rank_i, take_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, take_card_number));
		CCLog("strength_rate : %.3f", strength_rate);
		
		random_device rd;
		default_random_engine e1(rd());
		uniform_real_distribution<float> uniform_dist(0.f, 1.f);
		
		float result_value = uniform_dist(e1);
		CCLog("result value : %.3f", result_value);
		
		CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png",stage_number,take_level)->getCString());
		CardCase* cardCase = CardCase::create(take_card_number);
		card->addChild(cardCase);
		
		
		CCSprite* card2 = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png",stage_number,take_level)->getCString());
		CardCase* cardCase2 = CardCase::create(take_card_number);
		card2->addChild(cardCase2);
		
		
		StrengthCardAnimation* b = StrengthCardAnimation::create(card,card2,-190);
		
		b->setCloseFunc([this](){
			CCLog("close Func");
			this->endTakeCard();
		});
		
		if(result_value <= strength_rate)
		{
			CCLog("success");
			
			b->startSuccess("카드레벨 +1");
			
			myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number, myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, take_card_number)+1);
			myDSH->saveUserData({kSaveUserData_Key_cardsInfo}, nullptr);
		}
		else
		{
			CCLog("fail");
			
			b->startFail("강화 실패");
		}
		
		addChild(b, kZ_CP_popup);
	}
	else
	{
		// 획득
		
		CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png",stage_number,take_level)->getCString());
		CardCase* cardCase = CardCase::create(take_card_number);
		card->addChild(cardCase);
		
		TakeCardAnimation* b = TakeCardAnimation::create(card,-190);
		b->setCloseFunc([this](){
			CCLog("close Func");
			this->endTakeCard();
		});
		b->start();
		addChild(b, kZ_CP_popup);
	}
	
//	TakeCardPopup* t_popup = TakeCardPopup::create(stage_number, take_level, this, callfunc_selector(ClearPopup::endTakeCard));
//	addChild(t_popup, kZ_CP_popup);
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
	
	
	myLog->sendLog(CCString::createWithFormat("clear_%d", stage_number)->getCString());
	
	
	if(mySGD->is_ingame_before_have_stage_cards[0])
	{
		bronze_star = CCSprite::create("ending_star_bronze.png");
		bronze_star->setPosition(ccp(89,173));
		main_case->addChild(bronze_star, kZ_CP_img);
	}
	else
		bronze_star = NULL;
	
	if(mySGD->is_ingame_before_have_stage_cards[1])
	{
		silver_star = CCSprite::create("ending_star_silver.png");
		silver_star->setPosition(ccp(175,173));
		main_case->addChild(silver_star, kZ_CP_img);
	}
	else
		silver_star = NULL;
	
	if(mySGD->is_ingame_before_have_stage_cards[2])
	{
		gold_star = CCSprite::create("ending_star_gold.png");
		gold_star->setPosition(ccp(132,177));
		main_case->addChild(gold_star, kZ_CP_img);
	}
	else
		gold_star = NULL;
	
	take_star_animation_node = NULL;
	
	if(!bronze_star && take_level == 1)
	{
		bronze_star = CCSprite::create("ending_star_bronze.png");
		bronze_star->setPosition(ccp(89,173));
		main_case->addChild(bronze_star, kZ_CP_img);
		
		bronze_star->setScale(0);
		
		take_star_animation_node = bronze_star;
	}
	else if(!silver_star && take_level == 2)
	{
		silver_star = CCSprite::create("ending_star_silver.png");
		silver_star->setPosition(ccp(175,173));
		main_case->addChild(silver_star, kZ_CP_img);
		
		silver_star->setScale(0);
		
		take_star_animation_node = silver_star;
	}
	else if(!gold_star && take_level == 3)
	{
		gold_star = CCSprite::create("ending_star_gold.png");
		gold_star->setPosition(ccp(132,177));
		main_case->addChild(gold_star, kZ_CP_img);
		
		gold_star->setScale(0);
		
		take_star_animation_node = gold_star;
	}
	
	
	score_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	score_label->setAnchorPoint(ccp(1,0.5));
	score_label->setPosition(ccp(230,81));
	score_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(score_label, kZ_CP_img);
	
	gold_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	gold_label->setAnchorPoint(ccp(1,0.5));
	gold_label->setPosition(ccp(230,109));
	gold_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(gold_label, kZ_CP_img);
	
	time_label = CCLabelBMFont::create("0", "mb_white_font.fnt");
	time_label->setAnchorPoint(ccp(1,0.5));
	time_label->setPosition(ccp(230,135));
	time_label->setAlignment(kCCTextAlignmentRight);
	main_case->addChild(time_label, kZ_CP_img);
	
	string ok_filename;
	
	if(mySGD->getIsAcceptHelp())
	{
		ok_filename = "ending_main.png";
	}
	else if(mySGD->getIsMeChallenge())
	{
		if(mySGD->getScore() >= mySGD->getMeChallengeTargetScore())
			ok_filename = "ending_main.png";
		else
			ok_filename = "ending_fail.png";
	}
	else if(mySGD->getIsAcceptChallenge())
	{
		if(mySGD->getScore() >= mySGD->getAcceptChallengeScore())
			ok_filename = "ending_main.png";
		else
			ok_filename = "ending_fail.png";
	}
	else
	{
		ok_filename = "ending_next.png";
	}
	
	
	CCSprite* n_ok = CCSprite::create(ok_filename.c_str());
	CCSprite* s_ok = CCSprite::create(ok_filename.c_str());
	s_ok->setColor(ccGRAY);
	
	CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(ClearPopup::menuAction));
	ok_item->setTag(kMT_CP_ok);
	
	ok_menu = CCMenu::createWithItem(ok_item);
	ok_menu->setVisible(false);
	ok_menu->setPosition(ccp(348.5f,41));
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
		replay_menu->setPosition(ccp(130,41));
		main_case->addChild(replay_menu, kZ_CP_menu);
	}
	else
	{
		if((mySGD->getIsMeChallenge() && mySGD->getScore() < mySGD->getMeChallengeTargetScore()) || (mySGD->getIsAcceptChallenge() && mySGD->getScore() < mySGD->getAcceptChallengeScore()))
		{
			CCSprite* n_replay = CCSprite::create("ending_replay2.png");
			CCSprite* s_replay = CCSprite::create("ending_replay2.png");
			s_replay->setColor(ccGRAY);
			
			CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(ClearPopup::menuAction));
			replay_item->setTag(kMT_CP_replay);
			
			replay_menu = CCMenu::createWithItem(replay_item);
			replay_menu->setVisible(false);
			replay_menu->setPosition(ccp(130,41));
			main_case->addChild(replay_menu, kZ_CP_menu);
		}
		else
			replay_menu = NULL;
	}
	
	
	is_saved_user_data = false;
	myDSH->saveAllUserData(json_selector(this, ClearPopup::resultSavedUserData));
	
    return true;
}

void ClearPopup::showPopup()
{
	int seq_no_fail_cnt = myDSH->getIntegerForKey(kDSH_Key_achieve_seqNoFailCnt)+1;
	myDSH->setIntegerForKey(kDSH_Key_achieve_seqNoFailCnt, seq_no_fail_cnt);
	
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
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void ClearPopup::endHidePopup()
{
	AudioEngine::sharedInstance()->unloadEffectScene("Ending");
	
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void ClearPopup::checkChallengeOrHelp()
{
	if(mySGD->getIsAcceptHelp())
	{
		////////////////// ksks
		CCLog("zzzz");
		addChild(HelpResultSend::create(mySGD->getAcceptHelpId(), true, [=](){closePopup();}), kZ_CP_popup);
	}
	else if(mySGD->getIsMeChallenge())
	{
		//  파라메터 (내사진,내점수,내닉네임,상대사진,상대점수,상대닉네임,터치우선순위)
		hspConnector* t_hsp = hspConnector::get();
		FightResultAnimation* b = FightResultAnimation::create(t_hsp->getKakaoProfileURL(),mySGD->getScore(),t_hsp->getKakaoNickname(),
															   mySGD->getMeChallengeTargetProfile(),mySGD->getMeChallengeTargetScore(),mySGD->getMeChallengeTargetNick(),
															   -200);
		b->setCancelFunc([](){
			CCLog("닫기눌렀을때");
		});
		b->setCloseFunc([=](){
			CCLog("창 다닫혔을때");
			checkMiniGame();
		});
		
		addChild(b,kZ_CP_popup);
		
		if(mySGD->getScore() > mySGD->getMeChallengeTargetScore())
		{
			b->setSendFunc([=](){
				CCLog("승리시 확인눌렀을때->메세지전송");
			
				Json::Value p;
				Json::Value contentJson;
				contentJson["msg"] = (mySGD->getMeChallengeTargetNick() + "님에게 도전!");
				contentJson["challengestage"] = mySD->getSilType();
				contentJson["score"] = mySGD->getScore();
				contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
				contentJson["replaydata"] = mySGD->replay_write_info;
				contentJson["profile"] = t_hsp->getKakaoProfileURL();
				p["content"] = GraphDogLib::JsonObjectToString(contentJson);
				std::string recvId = mySGD->getMeChallengeTarget();
				p["receiverMemberID"] = recvId;
				
				p["senderMemberID"] = hspConnector::get()->getKakaoID();
				
				p["type"] = kChallengeRequest;
				hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
											 {
												 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
												 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
												 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
												 
												 //																		setHelpSendTime(recvId);
												 if(r["result"]["code"].asInt() != GDSUCCESS)
												 {
													 // 에러.
													 return;
												 }
												 
												 setChallengeSendTime(mySGD->getMeChallengeTarget());
												 //																	 friend_list.erase(friend_list.begin() + tag);
												 GraphDogLib::JsonToLog("sendMessage", r);
												 
												 //																		obj->removeFromParent();
												 
												 Json::Value p2;
												 p2["receiver_id"] = recvId;
												 p2["message"] = "도전을 신청한다!!";
												 hspConnector::get()->kSendMessage
												 (p2, [=](Json::Value r)
												  {
													  GraphDogLib::JsonToLog("kSendMessage", r);
												  });
											 });
			});
			
			b->startWin(); //내가이겼을때
			
		}
		else
		{
			b->setConfirmFunc([](){
				CCLog("패배시 확인눌렀을때");
			});
			
			b->startLose(); //내가졌을때
		}
		
		// ksks
//		addChild(ChallengeSend::create(mySGD->getMeChallengeTarget(), mySGD->getMeChallengeTargetNick(), mySGD->getScore(),
//									   ChallengeCategory::kRequest), kZ_CP_popup);
		//		getMeChallengeTarget
	}
	else if(mySGD->getIsAcceptChallenge())
	{
		mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)] = 0;
		/////////////////// ksks
		//		mySGD->getAcceptChallengeId(), mySGD->getAcceptChallengeNick(), mySGD->getAcceptChallengeScore();
		
		//  파라메터 (내사진,내점수,내닉네임,상대사진,상대점수,상대닉네임,터치우선순위)
		hspConnector* t_hsp = hspConnector::get();
		FightResultAnimation* b = FightResultAnimation::create(t_hsp->getKakaoProfileURL(),mySGD->getScore(),t_hsp->getKakaoNickname(),
															   mySGD->getAcceptChallengeProfile(),mySGD->getAcceptChallengeScore(),mySGD->getAcceptChallengeNick(),
															   -200);
		b->setCancelFunc([](){
			CCLog("닫기눌렀을때");
		});
		b->setCloseFunc([=](){
			CCLog("창 다닫혔을때");
			checkMiniGame();
		});
		
		addChild(b,kZ_CP_popup);
		
		if(mySGD->getAcceptChallengeScore() < mySGD->getScore())
		{
			b->setSendFunc([=](){
				CCLog("승리시 확인눌렀을때->메세지전송");
				
				Json::Value p;
				Json::Value contentJson;
				contentJson["msg"] = (mySGD->getAcceptChallengeNick() + "님에게 도전!");
				contentJson["challengestage"] = mySD->getSilType();
				contentJson["score"] = mySGD->getScore();
				contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
				contentJson["replaydata"] = mySGD->replay_write_info;
				contentJson["profile"] = t_hsp->getKakaoProfileURL();
				p["content"] = GraphDogLib::JsonObjectToString(contentJson);
				std::string recvId = mySGD->getAcceptChallengeId();
				p["receiverMemberID"] = recvId;
				
				p["senderMemberID"] = hspConnector::get()->getKakaoID();
				
				p["type"] = kChallengeRequest;
				hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
											 {
												 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
												 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
												 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
												 
												 //																		setHelpSendTime(recvId);
												 if(r["result"]["code"].asInt() != GDSUCCESS)
												 {
													 // 에러.
													 return;
												 }
												 
												 setChallengeSendTime(mySGD->getAcceptChallengeId());
												 //																	 friend_list.erase(friend_list.begin() + tag);
												 GraphDogLib::JsonToLog("sendMessage", r);
												 
												 //																		obj->removeFromParent();
												 
												 Json::Value p2;
												 p2["receiver_id"] = recvId;
												 p2["message"] = "도전을 신청한다!!";
												 hspConnector::get()->kSendMessage
												 (p2, [=](Json::Value r)
												  {
													  GraphDogLib::JsonToLog("kSendMessage", r);
												  });
											 });
				
				checkMiniGame();
			});
			
			b->startWin(); //내가이겼을때
		}
		else
		{
			b->setConfirmFunc([](){
				CCLog("패배시 확인눌렀을때");
			});
			
			b->startLose(); //내가졌을때
		}
	}
	else
	{
		if(is_rank_changed)
		{
			hspConnector* t_hsp = hspConnector::get();
			//  파라메터 (내사진,내점수,내등수,내닉네임,상대사진,상대점수,상대등수,상대닉네임,터치우선순위)
			ChangeRankAnimation* b = ChangeRankAnimation::create(t_hsp->getKakaoProfileURL(),mySGD->getScore(),recent_my_rank,t_hsp->getKakaoNickname(),
																 next_rank_info.img_url, next_rank_info.score, recent_my_rank+1, next_rank_info.nickname,
																 -200);
			
			
			b->setCancelFunc([](){
				CCLog("닫기눌렀을때");
			});
			b->setCloseFunc([=](){
				CCLog("창 다닫혔을때");
				checkMiniGame();
			});
			b->setSendFunc([=](){
				CCLog("확인버튼눌렀을때->메세지전송");
				Json::Value p;
				Json::Value contentJson;
				contentJson["msg"] = (next_rank_info.nickname + "님에게 도전!");
				contentJson["challengestage"] = mySD->getSilType();
				contentJson["score"] = mySGD->getScore();
				contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
				contentJson["replaydata"] = mySGD->replay_write_info;
				contentJson["profile"] = t_hsp->getKakaoProfileURL();
				p["content"] = GraphDogLib::JsonObjectToString(contentJson);
				std::string recvId = next_rank_info.user_id;
				p["receiverMemberID"] = recvId;
				
				p["senderMemberID"] = hspConnector::get()->getKakaoID();
				
				p["type"] = kChallengeRequest;
				hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
											 {
												 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
												 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
												 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
												 
												 //																		setHelpSendTime(recvId);
												 if(r["result"]["code"].asInt() != GDSUCCESS)
												 {
													 // 에러.
													 return;
												 }
												 
												 setChallengeSendTime(next_rank_info.user_id);
												 //																	 friend_list.erase(friend_list.begin() + tag);
												 GraphDogLib::JsonToLog("sendMessage", r);
												 
												 //																		obj->removeFromParent();
												 
												 Json::Value p2;
												 p2["receiver_id"] = recvId;
												 p2["message"] = "도전을 신청한다!!";
												 hspConnector::get()->kSendMessage
												 (p2, [=](Json::Value r)
												  {
													  GraphDogLib::JsonToLog("kSendMessage", r);
												  });
											 });
			});
			
			b->start();
			
			addChild(b,kZ_CP_popup);
		}
		else
		{
			checkMiniGame();
		}
	}
	
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
	startCalcAnimation();
	if(take_star_animation_node)
	{
		CCDelayTime* t_delay = CCDelayTime::create(0.5f);
		CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 1.5f);
		CCScaleTo* t_scale2 = CCScaleTo::create(0.2f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ClearPopup::checkRentCard));
		CCSequence* t_seq = CCSequence::create(t_delay, t_scale1, t_scale2, t_call, NULL);
		take_star_animation_node->runAction(t_seq);
	}
	else
	{
		checkRentCard();
	}
}

void ClearPopup::checkMiniGame()
{
	bool is_minigame_stage = NSDS_GB(mySD->getSilType(), kSDS_SI_minigame_b);
	if(is_minigame_stage && !myDSH->getBoolForKey(kDSH_Key_minigame_int1_isPlayed, mySD->getSilType()))
	{
		int minigame_played_cnt = myDSH->getIntegerForKey(kDSH_Key_minigame_playedCnt)+1;
		myDSH->setIntegerForKey(kDSH_Key_minigame_playedCnt, minigame_played_cnt, false);
		myDSH->setIntegerForKey(kDSH_Key_minigame_int1_stageNumber, minigame_played_cnt, mySD->getSilType(), false);
		myDSH->setBoolForKey(kDSH_Key_minigame_int1_isPlayed, mySD->getSilType(), true, false);
		myDSH->fFlush();
		MiniGamePopup* t_popup = MiniGamePopup::create((MiniGameCode)(rand()%(kMiniGameCode_dodge+1)), bind(&ClearPopup::closePopup, this));
		addChild(t_popup, kZ_CP_popup);
	}
	else
	{
		closePopup();
	}
}

void ClearPopup::closePopup()
{
	
}

void ClearPopup::checkRentCard()
{
	if(mySGD->getWasUsedFriendCard())
	{
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		addChild(t_popup, kZ_CP_popup);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
		case_back->setPosition(CCPointZero);
		t_container->addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(330, 265));
		
		CCLabelTTF* title_img = CCLabelTTF::create("보답하기", mySGD->getFont().c_str(), 13);
		title_img->setPosition(ccp(0, 111));
		t_container->addChild(title_img);
		
		CCLabelTTF* ment_label = CCLabelTTF::create(CCString::createWithFormat("%s님의 카드가 도움이 되었나요?\n하트로 고마움을 표현하세요.", mySGD->getSelectedFriendCardData().nick.c_str())->getCString(),
													mySGD->getFont().c_str(), 20);
		ment_label->setPosition(CCPointZero);
		t_container->addChild(ment_label);
		
		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
																			{
																				checkChallengeOrHelp();
																				t_popup->removeFromParent();
																			});
		
		CCMenuLambda* close_menu = CCMenuLambda::createWithItem(close_item);
		close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
		close_menu->setPosition(ccp(140,112));
		t_container->addChild(close_menu);
		
		
		CCSprite* n_send_heart = CCSprite::create("help_send.png");
		CCSprite* s_send_heart = CCSprite::create("help_send.png");
		s_send_heart->setColor(ccGRAY);
		
		CCMenuItemSpriteLambda* send_heart_item = CCMenuItemSpriteLambda::create(n_send_heart, s_send_heart, [=](CCObject* sender)
																				 {
																					 // 경수
																					 // 하트 보내기 작업
																					 // 보낼 유저 id : mySGD->getSelectedFriendCardData().userId
																					 checkChallengeOrHelp();
																					 t_popup->removeFromParent();
																				 });
		
		CCMenuLambda* send_heart_menu = CCMenuLambda::createWithItem(send_heart_item);
		send_heart_menu->setTouchPriority(t_popup->getTouchPriority()-1);
		send_heart_menu->setPosition(ccp(0,-100));
		t_container->addChild(send_heart_menu);
	}
	else
	{
		checkChallengeOrHelp();
	}
}

void ClearPopup::resultLoadFriends(Json::Value result_data)
{
//	CCLog("resultLoadFriends : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
//	if(result_data["status"].asInt() == 0)
//	{
//		Json::Value appfriends = result_data["app_friends_info"];
//		appfriends.append(hspConnector::get()->myKakaoInfo);
//		
		Json::Value p;
//		for(int i=0; i<appfriends.size();i++)
//		{
//			ClearFriendRank t_friend_info;
//			t_friend_info.nickname = appfriends[i]["nickname"].asString().c_str();
//			t_friend_info.img_url = appfriends[i]["profile_image_url"].asString().c_str();
//			t_friend_info.user_id = appfriends[i]["user_id"].asString().c_str();
//			t_friend_info.score = 0;
//			t_friend_info.is_play = false;
//			friend_list.push_back(t_friend_info);
//			
//			p["memberIDList"].append(appfriends[i]["user_id"].asString());
//		}
		
		Json::Value my_kakao = hspConnector::get()->myKakaoInfo;
		
		RankFriendInfo fInfo;
		fInfo.nickname = my_kakao["nickname"].asString();
		fInfo.img_url = my_kakao["profile_image_url"].asString();
		fInfo.user_id = my_kakao["user_id"].asString();
		fInfo.score = 0;
		fInfo.is_play = false;
		friend_list.push_back(fInfo);
		
		p["memberIDList"].append(my_kakao["user_id"].asString());
		
		for(auto i : KnownFriends::getInstance()->getFriends())
		{
			RankFriendInfo fInfo;
			fInfo.nickname = i.nick;
			fInfo.img_url = i.profileUrl;
			fInfo.user_id = i.userId;
			fInfo.score = 0;
			fInfo.is_play = false;
			fInfo.is_message_blocked = i.messageBlocked;
			friend_list.push_back(fInfo);
			
			p["memberIDList"].append(i.userId);
		}
		for(auto i : UnknownFriends::getInstance()->getFriends())
		{
			RankFriendInfo fInfo;
			fInfo.nickname = i.nick + "[unknown]";
			fInfo.img_url = i.profileUrl;
			fInfo.user_id = i.userId;
			fInfo.score = 0;
			fInfo.is_play = false;
			fInfo.is_message_blocked = i.messageBlocked;
			friend_list.push_back(fInfo);
			
			p["memberIDList"].append(i.userId);
		}
		
		
		p["stageNo"]=mySD->getSilType();
		hspConnector::get()->command("getstagescorelist",p,json_selector(this, ClearPopup::resultGetStageScoreList));
//	}
//	else
//	{
//		is_loaded_list = true;
//		endLoad();
//	}
}

void ClearPopup::resultGetStageScoreList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value score_list = result_data["list"];
		for(int i=0;i<score_list.size();i++)
		{
			if(score_list[i]["memberID"].asString() == hspConnector::get()->getKakaoID())
			{
				if(mySGD->getScore() < score_list[i]["score"].asFloat())
				{
					CCSprite* high_score_img = CCSprite::create("ending_highscore.png");
					high_score_img->setPosition(ccp(105, 81));
					main_case->addChild(high_score_img, kZ_CP_img);
				}
			}
			
			vector<RankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString().c_str());
			if(iter != friend_list.end())
			{
				(*iter).score = score_list[i]["score"].asFloat();
				(*iter).is_play = true;
			}
			else
				CCLog("not found friend memberID");
		}
		
		auto beginIter = std::remove_if(friend_list.begin(), friend_list.end(), [=](RankFriendInfo t_info)
										{
											return !t_info.is_play;
										});
		friend_list.erase(beginIter, friend_list.end());
		
		struct t_FriendSort{
			bool operator() (const RankFriendInfo& a, const RankFriendInfo& b)
			{
				return a.score > b.score;
			}
		} pred;
		
		sort(friend_list.begin(), friend_list.end(), pred);
		
		for(int i=0;i<friend_list.size();i++)
			friend_list[i].rank = i+1;
		
		// create cell
		
		//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 195, 176));
		//		temp_back->setAnchorPoint(CCPointZero);
		//		temp_back->setOpacity(100);
		//		temp_back->setPosition(ccp(246, 65));
		//		addChild(temp_back, kZ_CP_menu);
		
		rankTableView = CCTableView::create(this, CCSizeMake(208, 199));
		
		rankTableView->setAnchorPoint(CCPointZero);
		rankTableView->setDirection(kCCScrollViewDirectionVertical);
		rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		rankTableView->setPosition(ccp(243, 62.5f));
		
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
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		is_saved_user_data = true;
		endLoad();
		if(is_loaded_list)
		{
			rankTableView = CCTableView::create(this, CCSizeMake(208, 199));
			
			rankTableView->setAnchorPoint(CCPointZero);
			rankTableView->setDirection(kCCScrollViewDirectionVertical);
			rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
			rankTableView->setPosition(ccp(243, 62.5f));
			
			rankTableView->setDelegate(this);
			main_case->addChild(rankTableView, kZ_CP_menu);
			rankTableView->setTouchPriority(-200);
		}
		else
		{
			resultLoadFriends(Json::Value());
		}
		
//		hspConnector::get()->kLoadFriends(json_selector(this, ClearPopup::resultLoadFriends));
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
	
	AudioEngine::sharedInstance()->stopAllEffects();
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_homeClick)
	{
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kMT_CP_ok)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_pieceType);
			is_menu_enable = false;
			mySGD->setIsMeChallenge(false);
			mySGD->setIsAcceptChallenge(false);
			mySGD->setIsAcceptHelp(false);
			mySGD->selectFriendCard();
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
			mySGD->setIsMeChallenge(false);
			mySGD->setIsAcceptChallenge(false);
			mySGD->setIsAcceptHelp(false);
			mySGD->selectFriendCard();
			AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
			//		mySGD->resetLabels();
			hidePopup();
		}
		else if(tag == kMT_CP_replay)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
			//		mySGD->resetLabels();
			
			if(mySD->getSilType() >= 10000)
				mySGD->is_before_selected_event_stage = true;
			
			CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
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
	CCLabelTTF* nickname_label;
	CCLabelTTF* score_label;
	RankFriendInfo* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();

	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kCFC_T_img);
	profileImg->setPosition(ccp(52, 22));
	profileImg->setScale(35.f / profileImg->getContentSize().width);
	cell->addChild(profileImg, kCFC_Z_img);
	
	string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
	string cell_id = (*member).user_id;
	
	if(my_id != cell_id && KnownFriends::getInstance()->findById(cell_id) != nullptr)
	{
		CCSprite* kakao_sign = CCSprite::create("puzzle_right_rank_kakao.png");
		kakao_sign->setPosition(ccp(41,29));
		cell->addChild(kakao_sign, kCFC_Z_img);
	}
	
	CCSprite* bg;
	if((*member).user_id == hspConnector::get()->getKakaoID())
	{
		bg = CCSprite::create("ending_cell_me.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kCFC_Z_case);
		
		if(idx == 0)
		{
			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_gold.png");
			rank_img->setPosition(ccp(21,21));
			rank_img->setTag(kCFC_T_rank);
			cell->addChild(rank_img, kCFC_Z_img);
		}
		else if(idx == 1)
		{
			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_silver.png");
			rank_img->setPosition(ccp(21,21));
			rank_img->setTag(kCFC_T_rank);
			cell->addChild(rank_img, kCFC_Z_img);
		}
		else if(idx == 2)
		{
			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_bronze.png");
			rank_img->setPosition(ccp(21,21));
			rank_img->setTag(kCFC_T_rank);
			cell->addChild(rank_img, kCFC_Z_img);
		}
		else
		{
			CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 15);
			rank_label->setPosition(ccp(21,21));
			rank_label->setTag(kCFC_T_rank);
			cell->addChild(rank_label,kCFC_Z_img);
		}
	}
	else if(idx == 0)
	{
		bg = CCSprite::create("ending_cell_gold.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kCFC_Z_case);
		
		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_gold.png");
		rank_img->setPosition(ccp(21,21));
		rank_img->setTag(kCFC_T_rank);
		cell->addChild(rank_img, kCFC_Z_img);
	}
	else if(idx == 1)
	{
		bg = CCSprite::create("ending_cell_silver.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kCFC_Z_case);
		
		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_silver.png");
		rank_img->setPosition(ccp(21,21));
		rank_img->setTag(kCFC_T_rank);
		cell->addChild(rank_img, kCFC_Z_img);
	}
	else if(idx == 2)
	{
		bg = CCSprite::create("ending_cell_bronze.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kCFC_Z_case);
		
		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_bronze.png");
		rank_img->setPosition(ccp(21,21));
		rank_img->setTag(kCFC_T_rank);
		cell->addChild(rank_img, kCFC_Z_img);
	}
	else
	{
		bg = CCSprite::create("ending_cell_normal.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,kCFC_Z_case);
		
		CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 15);
		rank_label->setPosition(ccp(21,21));
		rank_label->setTag(kCFC_T_rank);
		cell->addChild(rank_label,kCFC_Z_img);
	}
	
	nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
	nickname_label->setPosition(ccp(130,28));
	nickname_label->setTag(kCFC_T_nickname);
	cell->addChild(nickname_label,kCFC_Z_img);
	
	score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 12);
	score_label->setColor(ccBLACK);
	score_label->setPosition(ccp(130,13));
	score_label->setTag(kCFC_T_score);
	cell->addChild(score_label,kCFC_Z_img);

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
	return CCSizeMake(210, 45);
}

unsigned int ClearPopup::numberOfCellsInTableView( CCTableView *table )
{
	return friend_list.size();
}
