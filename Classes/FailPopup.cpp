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
#include "DownloadFile.h"
#include "StageSettingPopup.h"
#include "PuzzleMapScene.h"
#include "KSUtil.h"
#include "KSAlertView.h"
#include "ShopPopup.h"
#include "ChallengeSend.h"
#include "HelpResultSend.h"
#include "SendMessageUtil.h"
#include "UnknownFriends.h"
#include "StartSettingScene.h"
#include "ASPopupView.h"
#include "CardAnimations.h"
#include "TouchSuctionLayer.h"
#include "KHAlertView.h"
#include "CommonButton.h"
#include "CardCase.h"
#include "AchieveNoti.h"

#include "HeartTime.h"
#include "MyLocalization.h"
#include "GDWebSprite.h"
#include "KSLabelTTF.h"

#include "LoadingTipScene.h"
#include "LoadingLayer.h"
#include "FlagSelector.h"
#include "EmptyItemSalePopup.h"
#include "EventShopPopup.h"
#include "TodayMissionPopup.h"

#include "FormSetter.h"

typedef enum tMenuTagFailPopup{
	kMT_FP_main = 1,
	kMT_FP_replay,
}MenuTagFailPopup;

typedef enum tZorderFailPopup{
	kZ_FP_gray = 1,
	kZ_FP_back,
	kZ_FP_img,
	kZ_FP_table,
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
	
	startFormSetter(this);
	
	AudioEngine::sharedInstance()->preloadEffectScene("Ending");
	
	is_menu_enable = false;
//	is_loaded_list = false;
	is_end_popup_animation = false;
	
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	myLog->addLog(kLOG_remainHeart_i, -1, myDSH->getIntegerForKey(kDSH_Key_heartCnt));
	
	send_command_list.clear();
	
//	Json::Value param;
//	param["key"] = CCSTR_CWF("stage_over_%d", mySD->getSilType())->getCString();
//	
//	send_command_list.push_back(CommandParam("increaseStats", param, nullptr));
	
	send_command_list.push_back(myLog->getSendLogCommand(CCString::createWithFormat("fail_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)))->getCString()));
	
	if(mySGD->getScore() > myDSH->getIntegerForKey(kDSH_Key_allHighScore))
	{
		myDSH->setIntegerForKey(kDSH_Key_allHighScore, int(mySGD->getScore()));
	}
	Json::Value p1;
	p1["memberID"] = hspConnector::get()->getSocialID();
	p1["score"] = int(mySGD->getScore());
	Json::Value p1_data;
	p1_data["selectedcard"] = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	p1_data["allhighscore"] = mySGD->getScore();//myDSH->getIntegerForKey(kDSH_Key_allHighScore);
	p1_data["highstage"] = mySGD->suitable_stage;
	p1_data["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	p1_data["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	Json::FastWriter p1_data_writer;
	p1["data"] = p1_data_writer.write(p1_data);
	
	send_command_list.push_back(CommandParam("addweeklyscore", p1, nullptr));
	
	
//	if(mySGD->save_stage_rank_stageNumber == mySD->getSilType())
//	{
//		string my_id = hspConnector::get()->getSocialID();
//		
//		vector<RankFriendInfo>::iterator iter = find(mySGD->save_stage_rank_list.begin(), mySGD->save_stage_rank_list.end(), my_id);
//		if(iter != mySGD->save_stage_rank_list.end())
//		{
//			if((*iter).score < mySGD->getScore())
//			{
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
//			}
//		}
//		else
//		{
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
//		}
//		
//		friend_list = mySGD->save_stage_rank_list;
//		is_loaded_list = true;
//	}
	
	
//	Json::Value p;
//	p["memberID"]=hspConnector::get()->getSocialID();
//	p["score"]=int(mySGD->getScore());
//	p["stageNo"]=mySD->getSilType();
//	Json::Value p_data;
//	p_data["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
//	Json::FastWriter p_data_writer;
//	p["data"] = p_data_writer.write(p_data);
//	
//	send_command_list.push_back(CommandParam("setStageScore",p,nullptr));
	
	
//	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
//	
//	if(selected_card_number > 0)
//	{
//		if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) <= 0)
//		{
//			// 소멸
//			
//			CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",selected_card_number)->getCString());
//			CardCase* cardCase = CardCase::create(selected_card_number);
//			card->addChild(cardCase);
//			
//			RemoveCardAnimation* b = RemoveCardAnimation::create(card,-210);
//			
//			b->setSkipFunc([this](){
//				CCLOG("skip Func");
//			});
//			b->setRepairFunc([=](){
//				CCLOG("repair Func");
//				
//				if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getCardDurabilityUpFee())
//				{
//					mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - mySGD->getCardDurabilityUpFee());
//					myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number, myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, selected_card_number));
//					
//					myDSH->saveUserData({kSaveUserData_Key_star}, nullptr);
//					
//					//복구하기
//					b->repair();
//				}
//				else
//				{
//					b->skip();
//				}
//			});
//			b->setCloseFunc([=](){
//				CCLOG("close Func");
//				if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) <= 0)
//					myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
//				this->endDecreaseCardDuration();
//			});
//			
//			b->start();
//			addChild(b, kZ_FP_popup);
//		}
//		else
//		{
//			// 내구 하락
//			int cardNo = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
//			
//			CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",cardNo)->getCString());
//			CardCase* cardCase = CardCase::create(cardNo);
//			card->addChild(cardCase);
//			
//			DownCardAnimation* b = DownCardAnimation::create(card,-210);
//			b->setCloseFunc([this](){
//				this->endDecreaseCardDuration();
//			});
//			b->start();
//			addChild(b, kZ_FP_popup);
//		}
//	}
//	else
//	{
		this->endDecreaseCardDuration();
//	}
	
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
	
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-14.f-450));
	addChild(main_case, kZ_FP_back);
	
	inner_left = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	inner_left->setContentSize(CCSizeMake(230, 209));
	inner_left->setPosition(main_case->getContentSize().width*0.26f, main_case->getContentSize().height*0.58f+3);
	main_case->addChild(inner_left);
	
	
	CCScale9Sprite* inner_right = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	inner_right->setContentSize(CCSizeMake(230, 209));
	inner_right->setPosition(main_case->getContentSize().width*0.74f, main_case->getContentSize().height*0.58f+3);
	main_case->addChild(inner_right);
	
	CCScale9Sprite* star_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	star_back->setContentSize(CCSizeMake(215, 60));
	star_back->setPosition(ccp(115,140));
	inner_left->addChild(star_back);
	
	
	play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
	
	if(mySD->getClearCondition() == kCLEAR_timeLimit)
		play_limit_time = play_limit_time.getV() - mySD->getClearConditionTimeLimit();
	
	left_life_base_score = mySGD->area_score.getV() + mySGD->damage_score.getV() + mySGD->combo_score.getV();
	left_life_decrease_score = (mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_lifeBonusCnt)].asInt()*30000*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d));
	left_time_base_score = left_life_base_score.getV() + left_life_decrease_score.getV();
	left_time_decrease_score = 0;//(play_limit_time.getV()-mySGD->getGameTime())*500*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);
	left_grade_base_score = left_time_base_score.getV() + left_time_decrease_score.getV();
	left_grade_decrease_score = left_grade_base_score.getV()*0.f;
	left_damaged_score = -mySGD->damaged_score.getV();
	
	left_total_score = left_grade_base_score.getV() + left_grade_decrease_score.getV() + left_damaged_score.getV();
	
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
	
	title_list.clear();
	left_content_list.clear();
	
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
	
//	CCScale9Sprite* time_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
//	time_back->setContentSize(CCSizeMake(215, 35));
//	time_back->setPosition(ccp(115,93));
//	inner_left->addChild(time_back);
//	
//	
//	CCScale9Sprite* gold_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
//	gold_back->setContentSize(CCSizeMake(215, 35));
//	gold_back->setPosition(ccp(115,60));
//	inner_left->addChild(gold_back);
//	
//	
//	CCScale9Sprite* score_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
//	score_back->setContentSize(CCSizeMake(215, 35));
//	score_back->setPosition(ccp(115,27));
//	inner_left->addChild(score_back);
	
	
	FailCode fail_code = mySGD->fail_code;
	
	if(fail_code == kFC_timeover)
	{
		CCSprite* title = CCSprite::create(CCString::createWithFormat("ending_timeover_%s.png", myLoc->getSupportLocalCode())->getCString());
		title->setPosition(ccp(inner_left->getContentSize().width/2.f,186));
		inner_left->addChild(title, kZ_FP_img);
	}
	else if(fail_code == kFC_missionfail)
	{
		CCSprite* title = CCSprite::create(CCString::createWithFormat("ending_missionfail_%s.png", myLoc->getSupportLocalCode())->getCString());
		title->setPosition(ccp(inner_left->getContentSize().width/2.f,186));
		inner_left->addChild(title, kZ_FP_img);
	}
	else
	{
		CCSprite* title = CCSprite::create(CCString::createWithFormat("ending_gameover_%s.png", myLoc->getSupportLocalCode())->getCString());
		title->setPosition(ccp(inner_left->getContentSize().width/2.f,186));
		inner_left->addChild(title, kZ_FP_img);
	}
	
	CCSprite* stage_tab = CCSprite::create("ending_tab.png");
	stage_tab->setPosition(ccp(40,253));
	main_case->addChild(stage_tab);
	
	if(mySGD->is_before_selected_event_stage)
	{
		int stage_number = mySD->getSilType();
		
		KSLabelTTF* stage_number_label = KSLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(),	mySGD->getFont().c_str(), 14);
		stage_number_label->setColor(ccc3(255, 170, 20));
		stage_number_label->enableOuterStroke(ccc3(95, 65, 130), 1.f);
		stage_number_label->setPosition(ccp(24, 16));
		stage_tab->addChild(stage_number_label);
		
		mySGD->is_before_selected_event_stage = false;
	}
	else
	{
		int stage_number = mySD->getSilType();
//		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
//		int piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		KSLabelTTF* piece_number_label = KSLabelTTF::create(CCString::createWithFormat("%d", stage_number)->getCString(),	mySGD->getFont().c_str(), 14);
		piece_number_label->setColor(ccc3(255, 170, 20));
		piece_number_label->enableOuterStroke(ccc3(95, 65, 130), 1.f);
		piece_number_label->setPosition(ccp(24, 16));
		stage_tab->addChild(piece_number_label);
	}
	
	for(int i=0;i<4;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_empty.png");
		t_star->setPosition(ccp(53+i*48,201));
		main_case->addChild(t_star, kZ_FP_img);
		setFormSetter(t_star);
	}
	
	CCScale9Sprite* left_total_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_total_back->setContentSize(CCSizeMake(inner_left->getContentSize().width-20, 35));
	left_total_back->setPosition(ccp(inner_left->getContentSize().width/2.f, 24));
	inner_left->addChild(left_total_back);
	
	KSLabelTTF* left_total_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_totalScore), mySGD->getFont().c_str(), 15);
	left_total_title->setColor(ccc3(255, 170, 20));
	left_total_title->setAnchorPoint(ccp(0,0.5f));
	left_total_title->setPosition(ccp(10, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_title);
	
	string start_total_left_content;
	start_total_left_content = "0";
	
	left_total_content = KSLabelTTF::create(start_total_left_content.c_str(), mySGD->getFont().c_str(), 15);
	left_total_content->setColor(ccc3(255, 170, 20));
	left_total_content->setAnchorPoint(ccp(1,0.5f));
	left_total_content->setPosition(ccp(left_total_back->getContentSize().width-10, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_content);
	
	CCRect left_rect = CCRectMake(inner_left->getContentSize().width/2.f-((480-30)/2.f-20)/2.f, 51.f-30.f/2.f+4, (480-30)/2.f-20, 72);
	
	//	CCSprite* left_size = CCSprite::create("whitePaper.png", CCRectMake(0, 0, left_rect.size.width, left_rect.size.height));
	//	left_size->setAnchorPoint(CCPointZero);
	//	left_size->setPosition(left_rect.origin);
	//	left_back->addChild(left_size);
	
	left_table = CCTableView::create(this, left_rect.size);
	left_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	left_table->setPosition(left_rect.origin);
	inner_left->addChild(left_table);
	left_table->setTouchPriority(-200);
	left_table->setTouchEnabled(false);
	
	left_table->setContentOffset(ccp(0,-9*28));
	
//	KSLabelTTF* time_ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_time), mySGD->getFont().c_str(), 15);
//	time_ment->enableOuterStroke(ccc3(40, 15, 55), 1.f);
//	time_ment->setPosition(ccp(48,153));
//	main_case->addChild(time_ment, kZ_FP_img);
//	setFormSetter(time_ment);
//	
//	CCSprite* time_img = CCSprite::create("ending_mark_time.png");
//	time_img->setPosition(ccp(80,153));
//	main_case->addChild(time_img, kZ_FP_img);
//	
//	time_label = KSLabelTTF::create("0", mySGD->getFont().c_str(), 15);
//	time_label->enableOuterStroke(ccc3(40, 15, 55), 1.f);
//	time_label->setAnchorPoint(ccp(1,0.5));
//	time_label->setPosition(ccp(220,153));
//	main_case->addChild(time_label, kZ_FP_img);
//	
//	KSLabelTTF* gold_ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_gold), mySGD->getFont().c_str(), 15);
//	gold_ment->enableOuterStroke(ccc3(40, 15, 55), 1.f);
//	gold_ment->setPosition(ccp(48,120));
//	main_case->addChild(gold_ment, kZ_FP_img);
//	
//	CCSprite* gold_img = CCSprite::create("ending_mark_gold.png");
//	gold_img->setPosition(ccp(80,120));
//	main_case->addChild(gold_img, kZ_FP_img);
//	
//	gold_label = KSLabelTTF::create("0", mySGD->getFont().c_str(), 15);
//	gold_label->enableOuterStroke(ccc3(40, 15, 55), 1.f);
//	gold_label->setAnchorPoint(ccp(1,0.5));
//	gold_label->setPosition(ccp(220,120));
//	main_case->addChild(gold_label, kZ_FP_img);
//	
//	if(mySGD->isTimeEvent(kTimeEventType_gold))
//	{
//		KSLabelTTF* gold_event = KSLabelTTF::create(CCString::createWithFormat("X%.1f", mySGD->getTimeEventFloatValue(kTimeEventType_gold))->getCString(), mySGD->getFont().c_str(), 14);
//		gold_event->setColor(ccc3(255, 190, 50));
//		gold_event->enableOuterStroke(ccBLACK, 1.f);
//		gold_event->setAnchorPoint(ccp(0,0.5));
//		gold_event->setPosition(ccp(gold_ment->getContentSize().width+3,gold_ment->getContentSize().height/2.f));
//		gold_ment->addChild(gold_event);
//		
//		KSLabelTTF* event_label = KSLabelTTF::create("EVENT", mySGD->getFont().c_str(), 10);
//		
//		CCScale9Sprite* event_back = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
//		event_back->setContentSize(CCSizeMake(event_label->getContentSize().width+18, 20));
//		event_back->setPosition(ccp(gold_event->getContentSize().width+4+event_back->getContentSize().width/2.f, gold_event->getContentSize().height/2.f));
//		gold_event->addChild(event_back);
//		
//		event_label->setPosition(ccpFromSize(event_back->getContentSize()/2.f));
//		event_back->addChild(event_label);
//	}
//	
//	KSLabelTTF* score_ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_score), mySGD->getFont().c_str(), 15);
//	score_ment->enableOuterStroke(ccc3(40, 15, 55), 1.f);
//	score_ment->setPosition(ccp(48,87));
//	main_case->addChild(score_ment, kZ_FP_img);
//	
//	CCSprite* score_img = CCSprite::create("ending_mark_score.png");
//	score_img->setPosition(ccp(80,87));
//	main_case->addChild(score_img, kZ_FP_img);
//	
//	score_label = KSLabelTTF::create("0", mySGD->getFont().c_str(), 15);
//	score_label->enableOuterStroke(ccc3(40, 15, 55), 1.f);
//	score_label->setAnchorPoint(ccp(1,0.5));
//	score_label->setPosition(ccp(220,87));
//	main_case->addChild(score_label, kZ_FP_img);
	
	
	CCSprite* n_ok = CCSprite::create("ending_button.png");
	
	KSLabelTTF* n_ok_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_toMain), mySGD->getFont().c_str(), 22);
	n_ok_label2->setColor(ccWHITE);
	n_ok_label2->disableOuterStroke();
	n_ok_label2->setOpacity(100);
	n_ok_label2->setPosition(ccp(n_ok->getContentSize().width/2.f, n_ok->getContentSize().height/2.f-1));
	n_ok->addChild(n_ok_label2);
	
	KSLabelTTF* n_ok_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_toMain), mySGD->getFont().c_str(), 22);
	n_ok_label->setColor(ccc3(50, 30, 5));
	n_ok_label->setPosition(ccp(n_ok->getContentSize().width/2.f, n_ok->getContentSize().height/2.f));
	n_ok->addChild(n_ok_label);
	
	CCSprite* s_ok = CCSprite::create("ending_button.png");
	s_ok->setColor(ccGRAY);
	
	KSLabelTTF* s_ok_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_toMain), mySGD->getFont().c_str(), 22);
	s_ok_label2->setColor(ccWHITE);
	s_ok_label2->disableOuterStroke();
	s_ok_label2->setOpacity(100);
	s_ok_label2->setPosition(ccp(s_ok->getContentSize().width/2.f, s_ok->getContentSize().height/2.f-1));
	s_ok->addChild(s_ok_label2);
	
	KSLabelTTF* s_ok_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_toMain), mySGD->getFont().c_str(), 22);
	s_ok_label->setColor(ccc3(50, 30, 5));
	s_ok_label->setPosition(ccp(s_ok->getContentSize().width/2.f, s_ok->getContentSize().height/2.f));
	s_ok->addChild(s_ok_label);
	
	CCMenuItem* main_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(FailPopup::menuAction));
	main_item->setTag(kMT_FP_main);
	
	main_menu = CCMenu::createWithItem(main_item);
	main_menu->setVisible(false);
	main_menu->setPosition(ccp(354,36));
	main_case->addChild(main_menu, kZ_FP_menu);
	main_menu->setTouchPriority(-190);
	setFormSetter(main_menu);
	
	
	CCSprite* n_replay = CCSprite::create("ending_button.png");
	
	n_replay_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_regame), mySGD->getFont().c_str(), 22);
	n_replay_label2->setColor(ccWHITE);
	n_replay_label2->disableOuterStroke();
	n_replay_label2->setOpacity(100);
	n_replay_label2->setPosition(ccp(n_replay->getContentSize().width/2.f, n_replay->getContentSize().height/2.f-1));
	n_replay->addChild(n_replay_label2);
	
	KSLabelTTF* n_replay_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_regame), mySGD->getFont().c_str(), 22);
	n_replay_label->setColor(ccc3(50, 30, 5));
	n_replay_label->setPosition(ccp(n_replay->getContentSize().width/2.f, n_replay->getContentSize().height/2.f));
	n_replay->addChild(n_replay_label);
	CCSprite* s_replay = CCSprite::create("ending_button.png");
	s_replay->setColor(ccGRAY);
	
	s_replay_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_regame), mySGD->getFont().c_str(), 22);
	s_replay_label2->setColor(ccWHITE);
	s_replay_label2->disableOuterStroke();
	s_replay_label2->setOpacity(100);
	s_replay_label2->setPosition(ccp(s_replay->getContentSize().width/2.f, s_replay->getContentSize().height/2.f-1));
	s_replay->addChild(s_replay_label2);
	
	KSLabelTTF* s_replay_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_regame), mySGD->getFont().c_str(), 22);
	s_replay_label->setColor(ccc3(50, 30, 5));
	s_replay_label->setPosition(ccp(s_replay->getContentSize().width/2.f, s_replay->getContentSize().height/2.f));
	s_replay->addChild(s_replay_label);
	
	CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(FailPopup::menuAction));
	replay_item->setTag(kMT_FP_replay);
	
	replay_menu = CCMenu::createWithItem(replay_item);
	replay_menu->setVisible(false);
	replay_menu->setPosition(ccp(125,36));
	main_case->addChild(replay_menu, kZ_FP_menu);
	replay_menu->setTouchPriority(-190);
	setFormSetter(replay_menu);
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	loading_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_img->setPosition(ccp(347,130));
	main_case->addChild(loading_img, kZ_FP_img);
	reader->release();
	
	if(mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_emptyItem) && mySGD->getGoodsValue(kGoodsType_item9) + mySGD->getGoodsValue(kGoodsType_item6) + mySGD->getGoodsValue(kGoodsType_item11) <= 0)
	{
		EmptyItemSalePopup* t_popup = EmptyItemSalePopup::create(-300, [=](){}, [=](){}, kPurchaseGuideType_emptyItem);
		addChild(t_popup, kZ_FP_popup+1);
	}
	else if(mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_stupidNpuHelp) && mySGD->getGoodsValue(kGoodsType_item9) + mySGD->getGoodsValue(kGoodsType_item6) + mySGD->getGoodsValue(kGoodsType_item11) <= 0 &&
			mySGD->getUserdataTotalPlayCount() >= mySGD->getStupidNpuHelpPlayCount() && mySGD->getUserdataFailCount() >= mySGD->getStupidNpuHelpFailCount())
	{
		EmptyItemSalePopup* t_popup = EmptyItemSalePopup::create(-300, [=](){}, [=](){}, kPurchaseGuideType_stupidNpuHelp);
		addChild(t_popup, kZ_FP_popup+1);
	}
	else if(mySGD->getPlayCountHighIsOn() != 0 && mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_eventRubyShop) && mySGD->getUserdataTotalPlayCount() >= mySGD->getPlayCountHighValue())
	{
		EventShopPopup* t_popup = EventShopPopup::create(-300, [=](){});
		addChild(t_popup, kZ_FP_popup+1);
	}
	
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
	send_command_list.push_back(CommandParam("getstagerankbyalluser", param2, json_selector(this, FailPopup::resultGetRank)));
	mySGD->keep_time_info.is_loaded = false;
	send_command_list.push_back(CommandParam("gettimeinfo", Json::Value(), json_selector(this, FailPopup::resultGetTime)));
	
	mySGD->setUserdataAchieveNoFail(0);
	
	for(int i=kAchievementCode_fail1;i<=kAchievementCode_fail3;i++)
	{
		if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
		   mySGD->getUserdataAchieveFail() + 1 >= myAchieve->getCondition((AchievementCode)i))
		{
			AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
	}
	
	mySGD->setUserdataAchieveFail(mySGD->getUserdataAchieveFail() + 1);
	
	mySGD->setUserdataAchieveHunter(mySGD->getUserdataAchieveHunter()+mySGD->hunt_value.getV());
	
	if(mySGD->is_exchanged)
		mySGD->setUserdataAchieveChangeMania(mySGD->getUserdataAchieveChangeMania() + 1);
	
	int seq_no_fail_cnt = mySGD->getUserdataAutoLevel()-1;
	if(seq_no_fail_cnt<0)seq_no_fail_cnt=0;
	mySGD->setUserdataAutoLevel(seq_no_fail_cnt);
	
	vector<CommandParam> t_achieve = myAchieve->updateAchieveHistoryVectorParam(nullptr);
	for(int i=0;i<t_achieve.size();i++)
	{
		send_command_list.push_back(t_achieve[i]);
	}
	
	mySGD->setUserdataFailCount(mySGD->getUserdataFailCount()+1);
	send_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	is_today_mission_success = mySGD->today_mission_info.is_success.getV();
	
	send_command_list.push_back(mySGD->getUpdateTodayMissionParam([=](Json::Value result_data)
																  {
																	  if(result_data["result"]["code"].asInt() == GDSUCCESS)
																	  {
																		  if(!is_today_mission_success && result_data["isSuccess"].asBool())
																			{
																				is_today_mission_success = true;
																				if(is_today_mission_success)
																				{
																					mySGD->is_today_mission_first = false;
																					TodayMissionPopup* t_popup = TodayMissionPopup::create(-280, [=](){});
																					addChild(t_popup, kZ_FP_popup);
																				}
																			}
																		  else
																			{
																				is_today_mission_success = false;
																			}
																	  }
																  }));
	
	LoadingLayer* t_loading = LoadingLayer::create(-9999);
	addChild(t_loading, 9999);
	
	tryTransaction(t_loading);
	
	
	is_saved_user_data = false;
	
	myDSH->saveAllUserData(json_selector(this, FailPopup::resultSavedUserData));
	
	CCNode* curtain_node = LoadingTipScene::getOpenCurtainNode(true);
	curtain_node->setPosition(ccp(240,160));
	addChild(curtain_node, kZ_FP_popup+5);
	
	return true;
}

void FailPopup::tryTransaction(CCNode* t_loading)
{
	mySGD->changeGoodsTransaction(send_command_list, [=](Json::Value result_data)
								  {
									  if(result_data["result"]["code"].asInt() == GDSUCCESS)
									  {
										  CCLOG("FailPopup transaction success");
										  
										  mySGD->network_check_cnt = 0;
										  
										  t_loading->removeFromParent();
									  }
									  else
									  {
										  CCLOG("FailPopup transaction fail");
										  
										  mySGD->network_check_cnt++;
										  
										  if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
										  {
											  mySGD->network_check_cnt = 0;
											  
											  ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
												  tryTransaction(t_loading);
											  });
											  ((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
										  }
										  else
										  {
											  addChild(KSTimer::create(0.5f, [=]()
																	   {
																		   tryTransaction(t_loading);
																	   }));
										  }
									  }
								  });
}

FailPopup::~FailPopup()
{
	hspConnector::get()->removeTarget(this);
}

void FailPopup::resultGetTime(Json::Value result_data)
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
		bool is_open = mySGD->getPuzzleHistory(puzzle_number).is_open.getV();
		
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

void FailPopup::resultGetRank(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		
		CCSprite* graph_back = CCSprite::create("ending_graph.png");
		graph_back->setPosition(ccp(355,228));
		main_case->addChild(graph_back, kZ_FP_img);
		setFormSetter(graph_back);
		
//		KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
//		t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_a->setPosition(ccp(25,8));
//		graph_back->addChild(t_rank_a);
//		
//		KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
//		t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_b->setPosition(ccp(25+49,8));
//		graph_back->addChild(t_rank_b);
//		
//		KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
//		t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_c->setPosition(ccp(25+98,8));
//		graph_back->addChild(t_rank_c);
//		
//		KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
//		t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_d->setPosition(ccp(25+147,8));
//		graph_back->addChild(t_rank_d);
		
		int alluser = result_data["alluser"].asInt();
		int myrank = result_data["myrank"].asInt();
		
//		CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", alluser)->getCString(), mySGD->getFont().c_str(), 10);
//		all_user_label->setColor(ccc3(255, 50, 50));
//		all_user_label->setAnchorPoint(ccp(1,0.5));
//		all_user_label->setPosition(ccp(main_case->getContentSize().width-30, 210));
//		main_case->addChild(all_user_label, kZ_FP_img);
//		
//		CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_myrankValue), myrank)->getCString(), mySGD->getFont().c_str(), 10);
//		my_rank_label->setAnchorPoint(ccp(1,0.5));
//		my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
//		main_case->addChild(my_rank_label, kZ_FP_img);
		
		float rank_percent = alluser == 0 ? 1.f : 1.f * myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,0));
		rank_percent_case->setPosition(ccp(257+195,231));
		main_case->addChild(rank_percent_case, kZ_FP_img);
		setFormSetter(rank_percent_case);
		
		KSLabelTTF* percent_label = KSLabelTTF::create("100%", mySGD->getFont().c_str(), 13);
		addChild(KSGradualValue<float>::create(100.f, rank_percent*100.f,
																					 2.f * (1.f - rank_percent), [=](float t){
																					 percent_label->setString(ccsf("%.0f%%", t));
		}, [=](float t){
			percent_label->setString(ccsf("%.0f%%", t));
		}));
		percent_label->setColor(ccc3(255, 170, 20));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f+2));
		rank_percent_case->addChild(percent_label, kZ_FP_img);
		
		CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(257 + 195.f*rank_percent,231));
		rank_percent_case->runAction(t_move);
		
		Json::Value user_list = result_data["list"];
		
		int delay_index = 0;
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
			list_cell_case->setContentSize(CCSizeMake(217, 40));
			list_cell_case->setPosition(ccp(355,197-i*36));
			main_case->addChild(list_cell_case, kZ_FP_img);
			setFormSetter(list_cell_case);
			
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
			selectedFlagSpr->setPosition(ccp(52,20));
			selectedFlagSpr->setScale(0.8);
			list_cell_case->addChild(selectedFlagSpr);
			
			KSLabelTTF* nick_label = KSLabelTTF::create(read_data.get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12); // user_list[i]["nick"].asString().c_str()
			nick_label->setAnchorPoint(ccp(0,0.5));
			nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
			nick_label->setPosition(ccp(67,20));
			list_cell_case->addChild(nick_label);
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",user_list[i]["score"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setAnchorPoint(ccp(1,0.5));
			score_label->setColor(ccc3(255, 170, 20));
			score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
			score_label->setPosition(ccp(200,20));
			list_cell_case->addChild(score_label);
			
			CCPoint original_position = list_cell_case->getPosition();
			list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
			CCDelayTime* t_delay = CCDelayTime::create(0.5f + delay_index*0.2f);
			CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
			CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
			list_cell_case->runAction(t_seq);
			delay_index++;
		}
		
		if(myrank > 3)
		{
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			list_cell_case->setContentSize(CCSizeMake(217, 40));
			list_cell_case->setPosition(ccp(355,197-3*36));
			main_case->addChild(list_cell_case, kZ_FP_img);
			
			KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", myrank)->getCString(), mySGD->getFont().c_str(), 12);
			rank_label->enableOuterStroke(ccBLACK, 1);
			rank_label->setPosition(ccp(20,20));
			list_cell_case->addChild(rank_label);
			
			string flag = myDSH->getStringForKey(kDSH_Key_flag);
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setPosition(ccp(52,20));
			selectedFlagSpr->setScale(0.8);
			list_cell_case->addChild(selectedFlagSpr);
			
			KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12);
			nick_label->setAnchorPoint(ccp(0,0.5));
			nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
			nick_label->setPosition(ccp(67,20));
			list_cell_case->addChild(nick_label);
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",int(mySGD->getScore()))->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setAnchorPoint(ccp(1,0.5));
			score_label->setColor(ccc3(255, 170, 20));
			score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
			score_label->setPosition(ccp(200,20));
			list_cell_case->addChild(score_label);
			
			CCPoint original_position = list_cell_case->getPosition();
			list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
			CCDelayTime* t_delay = CCDelayTime::create(0.5f + delay_index*0.2f);
			CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
			CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
			list_cell_case->runAction(t_seq);
		}
	}
	else
	{
		CCLabelTTF* fail_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
		fail_label->setPosition(loading_img->getPosition());
		main_case->addChild(fail_label, kZ_FP_img);
	}
	
	loading_img->removeFromParent();
}

void FailPopup::endDecreaseCardDuration()
{
	closePopup();
}

void FailPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
	startCalcAnimation();
}

void FailPopup::showPopup()
{
	gray->setOpacity(255);
//	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
//	gray->runAction(gray_fade);

	main_case->setPosition(ccp(240,160-14.f));
	endShowPopup();
//	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
//	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(FailPopup::endShowPopup));
//	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
//	main_case->runAction(main_seq);
}

void FailPopup::endShowPopup()
{
	
}

void FailPopup::hidePopup()
{
	is_menu_enable = false;
//	rankTableView->setTouchEnabled(false);
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-14.f-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(FailPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void FailPopup::endHidePopup()
{
	AudioEngine::sharedInstance()->unloadEffectScene("Ending");
	
	if(target_final)
		(target_final->*delegate_final)();
	if(is_go_to_mainflow)
		goToMainFlow_func();
	removeFromParent();
}

void FailPopup::startCalcAnimation()
{
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
//	startTimeAnimation();
	
	function<void(float, float, float, function<void()>)> t_func1 = [=](float before_y, float left_decrease_value, float left_base_value, function<void()> end_func)
	{
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.15f, [=](float t)
											   {
												   left_table->setContentOffset(ccp(0, before_y+28*t));
											   }, [=](float t)
											   {
												   left_table->setContentOffset(ccp(0, before_y+28));
												   
												   AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
												   startLeftCalcAnimation(left_decrease_value, left_base_value, 0.3f, NULL, [=]()
																		  {
																			  end_func();
																		  });
											   }));
	};
	
	t_func1(-9*28, mySGD->area_score.getV(), 0, [=]()
	{
		t_func1(-8*28, mySGD->damage_score.getV(), mySGD->area_score.getV(), [=]()
		{
			t_func1(-7*28, mySGD->combo_score.getV(), mySGD->area_score.getV()+mySGD->damage_score.getV(), [=]()
			{
				t_func1(-6*28, left_life_decrease_score.getV(), left_life_base_score.getV(), [=]()
				{
					t_func1(-5*28, left_time_decrease_score.getV(), left_time_base_score.getV(), [=]()
					{
						t_func1(-4*28, left_grade_decrease_score.getV(), left_grade_base_score.getV(), [=]()
						{
							t_func1(-3*28, left_damaged_score.getV(), left_grade_base_score.getV()+left_grade_decrease_score.getV(), [=]()
							{
								AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
								
								addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t)
								{
									left_table->setContentOffset(ccp(0, -2*28+56*t));
								}, [=](float t)
								{
									left_table->setContentOffset(ccp(0, -2*28+56));
									
									left_table->setTouchEnabled(true);
								
									if(is_today_mission_success)
									{
										TodayMissionPopup* t_popup = TodayMissionPopup::create(-300, [=](){});
										addChild(t_popup, kZ_FP_popup);
									}
									
									is_end_popup_animation = true;
									
									closePopup();
								}));
							});
						});
					});
				});
			});
		});
	});
}

//void FailPopup::startScoreAnimation()
//{
//	keep_score = mySGD->getScore();
//	decrease_score = keep_score;
//	increase_score = 0.f;
//	schedule(schedule_selector(FailPopup::scoreAnimation));
//}

void FailPopup::closePopup()
{
	is_end_popup_animation = true;
	
	if(is_end_popup_animation && is_saved_user_data)// && is_loaded_list)
	{
		main_menu->setVisible(true);
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) > 0)
		{
			if(replay_menu)
				replay_menu->setVisible(true);
		}
		is_menu_enable = true;
	}
}


void FailPopup::startLeftCalcAnimation(float t_keep_value, float t_base_value, float t_calc_time, KSLabelTTF* t_decrease_target, function<void()> t_end_func)
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
	schedule(schedule_selector(FailPopup::leftCalcAnimation));
}
void FailPopup::leftCalcAnimation(float dt)
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
void FailPopup::stopLeftCalcAnimation()
{
	unschedule(schedule_selector(FailPopup::leftCalcAnimation));
	left_end_func();
}


//void FailPopup::scoreAnimation(float dt)
//{
//	if(decrease_score > 0)
//	{
//		int decreaseUnit = keep_score / 1.f * dt;
//		
//		if(decrease_score < decreaseUnit)
//		{
//			increase_score += decrease_score;
//			decrease_score = 0;
//		}
//		else {
//			if(decreaseUnit <= 0)
//			{
//				increase_score += decrease_score;
//				decrease_score = 0;
//			}
//			else {
//				decrease_score -= decreaseUnit;
//				increase_score += decreaseUnit;
//			}
//		}
//		score_label->setString(KS::insert_separator(CCString::createWithFormat("%.0f",increase_score)->getCString()).c_str());
//	}
//	else
//		stopScoreAnimation();
//}
//
//void FailPopup::stopScoreAnimation()
//{
//	unschedule(schedule_selector(FailPopup::scoreAnimation));
//	score_label->setString(KS::insert_separator(CCString::createWithFormat("%.0f", mySGD->getScore())->getCString()).c_str());
//	AudioEngine::sharedInstance()->stopAllEffects();
//}
//
//void FailPopup::startGoldAnimation()
//{
//	keep_gold = mySGD->getStageGold();
//	decrease_gold = keep_gold;
//	increase_gold = 0.f;
//	schedule(schedule_selector(FailPopup::goldAnimation));
//}
//
//void FailPopup::goldAnimation(float dt)
//{
//	if(decrease_gold > 0)
//	{
//		int decreaseUnit = keep_gold / 0.5f * dt;
//		
//		if(decrease_gold < decreaseUnit)
//		{
//			increase_gold += decrease_gold;
//			decrease_gold = 0;
//		}
//		else {
//			if(decreaseUnit <= 0)
//			{
//				increase_gold += decrease_gold;
//				decrease_gold = 0;
//			}
//			else {
//				decrease_gold -= decreaseUnit;
//				increase_gold += decreaseUnit;
//			}
//		}
//		gold_label->setString(CCString::createWithFormat("%.0f",increase_gold)->getCString());
//	}
//	else
//		stopGoldAnimation();
//}
//
//void FailPopup::stopGoldAnimation()
//{
//	unschedule(schedule_selector(FailPopup::goldAnimation));
//	gold_label->setString(CCString::createWithFormat("%d", mySGD->getStageGold())->getCString());
//	startScoreAnimation();
//}
//
//void FailPopup::startTimeAnimation()
//{
//	keep_time = mySGD->getGameTime();
//	decrease_time = keep_time;
//	increase_time = 0.f;
//	schedule(schedule_selector(FailPopup::timeAnimation));
//}
//
//void FailPopup::timeAnimation(float dt)
//{
//	if(decrease_time > 0)
//	{
//		int decreaseUnit = keep_time / 0.5f * dt;
//		
//		if(decrease_time < decreaseUnit)
//		{
//			increase_time += decrease_time;
//			decrease_time = 0;
//		}
//		else {
//			if(decreaseUnit <= 0)
//			{
//				increase_time += decrease_time;
//				decrease_time = 0;
//			}
//			else {
//				decrease_time -= decreaseUnit;
//				increase_time	+= decreaseUnit;
//			}
//		}
//		time_label->setString(CCString::createWithFormat("%.0f",increase_time)->getCString());
//	}
//	else
//		stopTimeAnimation();
//}
//
//void FailPopup::stopTimeAnimation()
//{
//	unschedule(schedule_selector(FailPopup::timeAnimation));
//	time_label->setString(CCString::createWithFormat("%d", mySGD->getGameTime())->getCString());
//	startGoldAnimation();
//}

void FailPopup::resultSavedUserData(Json::Value result_data)
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
//			main_case->addChild(rankTableView, kZ_FP_table);
//			rankTableView->setTouchPriority(kCCMenuHandlerPriority+1);
//		}
//		else
//		{
//			resultLoadFriends(Json::Value());
//		}
//		hspConnector::get()->kLoadFriends(json_selector(this, FailPopup::resultLoadFriends));
	}
	else
	{
		myDSH->saveAllUserData(json_selector(this, FailPopup::resultSavedUserData));
	}
}

//void FailPopup::resultLoadFriends(Json::Value result_data)
//{
////	CCLOG("resultLoadFriends : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
////	if(result_data["status"].asInt() == 0)
////	{
////		Json::Value appfriends = result_data["app_friends_info"];
////		appfriends.append(hspConnector::get()->myKakaoInfo);
////		
//		Json::Value p;
////		for(int i=0; i<appfriends.size();i++)
////		{
////			FailFriendRank t_friend_info;
////			t_friend_info.nickname = appfriends[i]["nickname"].asString().c_str();
////			t_friend_info.img_url = appfriends[i]["profile_image_url"].asString().c_str();
////			t_friend_info.user_id = appfriends[i]["user_id"].asString().c_str();
////			t_friend_info.score = 0;
////			t_friend_info.is_play = false;
////			t_friend_info.is_message_blocked = appfriends[i]["message_blocked"].asBool();
////			friend_list.push_back(t_friend_info);
////			
////			p["memberIDList"].append(appfriends[i]["user_id"].asString());
////		}
//
//	Json::Value my_kakao = hspConnector::get()->myKakaoInfo;
//	
//	RankFriendInfo fInfo;
//	fInfo.nickname = my_kakao["nickname"].asString();
//	fInfo.img_url = my_kakao["profile_image_url"].asString();
//	fInfo.user_id = my_kakao["user_id"].asString();
//	fInfo.score = 0;
//	fInfo.is_play = false;
//	friend_list.push_back(fInfo);
//	
//	p["memberIDList"].append(my_kakao["user_id"].asString());
//	
//	for(auto i : KnownFriends::getInstance()->getFriends())
//	{
//		RankFriendInfo fInfo;
//		fInfo.nickname = i.nick;
//		fInfo.img_url = i.profileUrl;
//		fInfo.user_id = i.userId;
//		fInfo.score = 0;
//		fInfo.is_play = false;
//		fInfo.is_message_blocked = i.messageBlocked;
//		friend_list.push_back(fInfo);
//		
//		p["memberIDList"].append(i.userId);
//	}
//	for(auto i : UnknownFriends::getInstance()->getFriends())
//	{
//		RankFriendInfo fInfo;
//		fInfo.nickname = i.nick;
//		fInfo.img_url = "";
//		fInfo.user_id = i.userId;
//		fInfo.score = 0;
//		fInfo.is_play = false;
//		fInfo.is_message_blocked = i.messageBlocked;
//		friend_list.push_back(fInfo);
//		
//		p["memberIDList"].append(i.userId);
//	}
//	
//	
//	p["stageNo"]=mySD->getSilType();
//	hspConnector::get()->command("getstagescorelist",p,json_selector(this, FailPopup::resultGetStageScoreList));
////	}
////	else
////	{
////		is_loaded_list = true;
////		endLoad();
////	}
//}

//void FailPopup::resultGetStageScoreList(Json::Value result_data)
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
//					main_case->addChild(high_score_img, kZ_FP_img);
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
//																		{
//																			return !t_info.is_play;
//																		});
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
////		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 208, 199));
////		temp_back->setAnchorPoint(CCPointZero);
////		temp_back->setOpacity(100);
////		temp_back->setPosition(ccp(243, 62.5f));
////		main_case->addChild(temp_back, kZ_FP_menu);
//		
//		rankTableView = CCTableView::create(this, CCSizeMake(208, 199));
//		
//		rankTableView->setAnchorPoint(CCPointZero);
//		rankTableView->setDirection(kCCScrollViewDirectionVertical);
//		rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
//		rankTableView->setPosition(ccp(243, 59.5f));
//		
//		rankTableView->setDelegate(this);
//		main_case->addChild(rankTableView, kZ_FP_table);
//		rankTableView->setTouchPriority(-190);
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
//		
//		if(suction)
//		{
//			suction->removeFromParent();
//			suction = NULL;
//		}
//		
//		suction = TouchSuctionLayer::create(-190+1);
//		suction->setNotSwallowRect(CCRectMake(243, 59.5f, 208, 199));
//		suction->setTouchEnabled(true);
//		main_case->addChild(suction);
//		
//	}
//	is_loaded_list = true;
//	endLoad();
//}

void FailPopup::endLoad()
{
	if(is_end_popup_animation && is_saved_user_data)// && is_loaded_list)
	{
		main_menu->setVisible(true);
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) > 0)
		{
			if(replay_menu)
				replay_menu->setVisible(true);
		}
		is_menu_enable = true;
	}
}

CCPoint FailPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kMT_FP_main)			return_value = ccp(100,67);
	else if(t_tag == kMT_FP_replay)		return_value = ccp(240,67);
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
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	
	is_menu_enable = false;
	
	if(tag == kMT_FP_main)
	{
		if(is_go_to_mainflow)
		{
			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_timeOutFrame)), 9999);
		}
		
		AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
		AudioEngine::sharedInstance()->playSound("bgm_ui.mp3", true);
//		mySGD->resetLabels();
		hidePopup();
	}
	else if(tag == kMT_FP_replay)
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
		
//		CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
//		StageSettingPopup* t_popup = StageSettingPopup::create();
//		t_popup->setHideFinalAction(target_final, delegate_final);
//		getParent()->addChild(t_popup, kPMS_Z_popup);
//		
//		target_final = NULL;
//		hidePopup();
	}
}

void FailPopup::cellAction( CCObject* sender )
{
//	int tag = (int)((CCNode*)sender)->getUserData();
//	CCLOG("help memberID : %s", friend_list[tag].user_id.c_str());
//	//김현수 89987036974054944
//	std::string user_id = friend_list[tag].user_id;
//	KS::KSLog("% %" , friend_list[tag].nickname, friend_list[tag].user_id);
//
//	KHAlertView* av = KHAlertView::create(); 
//	av->setTitleFileName("msg_help_request.png");
//	av->setCloseButton(CCMenuItemImageLambda::create("cardchange_cancel.png", "cardchange_cancel.png",
//																									 [=](CCObject*){
//																									 }
//																									));
//	av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
//	av->setWidth(240);
//	av->setHeight(240);
//	av->setTitleHeight(10);
//	av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
//	av->setCenterY(150);
//
//	CCNode* emptyNode = CCNode::create();
//	// friend_list[tag].nickname // 받을 친구의 닉네임.
//	auto ttf = CCLabelTTF::create("도움을 요청합니다.", mySGD->getFont().c_str(), 14.f); 
//	ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
//	//	con->setAnchorPoint(ccp(0, 0));
//	//ttf->setAnchorPoint(ccp(0.5f, 0.5f));
//	ttf->setColor(ccc3(255, 255, 255));
//	ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, -77));
//	emptyNode->addChild(ttf);
//	av->setContentNode(
//			emptyNode
//			);
//	av->setContentSize(ttf->getDimensions());
//	av->addButton(CommonButton::create("보내기", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
//								[=](CCObject* e) {
//									CCLOG("ok!!");
//									Json::Value p;
//									Json::Value contentJson;
//
//									contentJson["msg"] = (friend_list[tag].nickname + "님~ 못깨겠다. 좀 도와도...");
//									contentJson["helpstage"] = mySD->getSilType();
//									contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//
//									KS::KSLog("%", hspConnector::get()->myKakaoInfo);
//									//				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//									p["content"] = GraphDogLib::JsonObjectToString(contentJson);
//									std::string recvId = user_id;
//									recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
//									p["receiverMemberID"] = recvId;
//									p["senderMemberID"] = hspConnector::get()->getSocialID();
//									p["type"] = kHelpRequest;
//									hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
//																							 {
//																								 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//																								 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//																								 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//
//																								 //																		setHelpSendTime(recvId);
//																								 if(r["result"]["code"].asInt() != GDSUCCESS)
//										return;
//
//									setHelpSendTime(user_id);
//									friend_list.erase(friend_list.begin() + tag);
//									rankTableView->reloadData();
//									
//									 KHAlertView* av = KHAlertView::create(); 
//						 // av->setTitleFileName("msg_challenge.png");
//						 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
//						 av->setWidth(240);
//						 av->setHeight(240);
//						 av->setTitleHeight(10);
//						 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
//						 av->setCenterY(150);
//
//						 CCNode* emptyNode = CCNode::create();
//						 auto ttf = CCLabelTTF::create("정상적으로 도움요청이 이루어졌습니다.\n친구분이 도와주길 기다리세요.^^", mySGD->getFont().c_str(), 12.f); 
//						 ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
//						 //	con->setAnchorPoint(ccp(0, 0));
//						 //ttf->setAnchorPoint(ccp(0.5f, 0.5f));
//						 ttf->setColor(ccc3(255, 255, 255));
//						 ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, -77));
//						 emptyNode->addChild(ttf);
//						 av->setContentNode(
//								 emptyNode
//								 );
//						 av->setContentSize(ttf->getDimensions());
//						 av->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
//													 [=](CCObject* e) {
//														 CCLOG("ok!!");
//													 });
//
//						 addChild(av, kPMS_Z_helpRequest);
//						 av->show();
//
//
//						 //																		obj->removeFromParent();
////						 Json::Value p2;
////						 p2["receiver_id"] = recvId;
////						 p2["message"] = "저를 도와주세요. 실력파인 당신이 저를 도와주시면 3대가 축복을 받으실거에요~^^";
////						 hspConnector::get()->kSendMessage
////							 (p2, [=](Json::Value r) {
////								 GraphDogLib::JsonToLog("kSendMessage", r);
////							 });
//																							 });
//								});
//	
//
//	addChild(av, kPMS_Z_helpRequest);
//	av->show();
}

CCTableViewCell* FailPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
//	CCLabelTTF* nickname_label;
//	CCLabelTTF* score_label;
//	RankFriendInfo* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	if(table == left_table)
	{
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
		
		content = left_content_list[idx];
		
		CCScale9Sprite* t_back = CCScale9Sprite::create(back_filename.c_str(), CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		t_back->setContentSize(CCSizeMake(inner_left->getContentSize().width-20, 35));
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
	}
	
	
	
//	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
//	profileImg->setAnchorPoint(ccp(0.5, 0.5));
//	profileImg->setTag(kFFC_T_img);
//	profileImg->setPosition(ccp(52, 22));
//	cell->addChild(profileImg, -1);
//	
//	string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
//	string cell_id = (*member).user_id;
//	
//	//if(my_id != cell_id && KnownFriends::getInstance()->findById(cell_id) != nullptr)
//	if(KnownFriends::getInstance()->findById(cell_id) != nullptr)
//	{
//		CCSprite* kakao_sign = CCSprite::create("puzzle_right_rank_kakao.png");
//		kakao_sign->setPosition(ccp(41,29));
//		cell->addChild(kakao_sign, kFFC_Z_img);
//	}
//	
//	CCSprite* bg;
//	if((*member).user_id == hspConnector::get()->getSocialID())
//	{
//		bg = CCSprite::create("ending_cell_me.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kFFC_Z_case);
//		
//		if(idx == 0)
//		{
//			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_gold.png");
//			rank_img->setPosition(ccp(21,21));
//			rank_img->setTag(kFFC_T_rank);
//			cell->addChild(rank_img, kFFC_Z_img);
//		}
//		else if(idx == 1)
//		{
//			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_silver.png");
//			rank_img->setPosition(ccp(21,21));
//			rank_img->setTag(kFFC_T_rank);
//			cell->addChild(rank_img, kFFC_Z_img);
//		}
//		else if(idx == 2)
//		{
//			CCSprite* rank_img = CCSprite::create("puzzle_right_rank_bronze.png");
//			rank_img->setPosition(ccp(21,21));
//			rank_img->setTag(kFFC_T_rank);
//			cell->addChild(rank_img, kFFC_Z_img);
//		}
//		else
//		{
//			CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 15);
//			rank_label->setPosition(ccp(21,21));
//			rank_label->setTag(kFFC_T_rank);
//			cell->addChild(rank_label,kFFC_Z_img);
//		}
//	}
//	else if(idx == 0)
//	{
//		bg = CCSprite::create("ending_cell_gold.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kFFC_Z_case);
//		
//		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_gold.png");
//		rank_img->setPosition(ccp(21,21));
//		rank_img->setTag(kFFC_T_rank);
//		cell->addChild(rank_img, kFFC_Z_img);
//	}
//	else if(idx == 1)
//	{
//		bg = CCSprite::create("ending_cell_silver.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kFFC_Z_case);
//		
//		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_silver.png");
//		rank_img->setPosition(ccp(21,21));
//		rank_img->setTag(kFFC_T_rank);
//		cell->addChild(rank_img, kFFC_Z_img);
//	}
//	else if(idx == 2)
//	{
//		bg = CCSprite::create("ending_cell_bronze.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kFFC_Z_case);
//		
//		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_bronze.png");
//		rank_img->setPosition(ccp(21,21));
//		rank_img->setTag(kFFC_T_rank);
//		cell->addChild(rank_img, kFFC_Z_img);
//	}
//	else
//	{
//		bg = CCSprite::create("ending_cell_normal.png");
//		bg->setPosition(CCPointZero);
//		bg->setAnchorPoint(CCPointZero);
//		cell->addChild(bg,kFFC_Z_case);
//		
//		CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 15);
//		rank_label->setPosition(ccp(21,21));
//		rank_label->setTag(kFFC_T_rank);
//		cell->addChild(rank_label,kFFC_Z_img);
//	}
//	
//	nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 12);
//	nickname_label->enableStroke(ccBLACK, 0.5f);
//	nickname_label->setPosition(ccp(114,28));
//	nickname_label->setTag(kFFC_T_nickname);
//	cell->addChild(nickname_label,kFFC_Z_img);
//	
//	score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 12);
//	score_label->setColor(ccBLACK);
//	score_label->setPosition(ccp(114,13));
//	score_label->setTag(kFFC_T_score);
//	cell->addChild(score_label,kFFC_Z_img);
//	
//	
//	if((*member).user_id == hspConnector::get()->getSocialID())
//	{
//		
//	}
//	else
//	{
//		if(!mySGD->getIsMeChallenge() && !mySGD->getIsAcceptChallenge() && !mySGD->getIsAcceptHelp())
//		{
//			if(!(*member).is_message_blocked)
//			{
//				int remain_time = ::getIsNotHelpableUser((*member).user_id.c_str(), mySGD->getHelpCoolTime());
//				if(remain_time <= 0)
//				{
//					CCSprite* n_help = CCSprite::create("ending_help_on.png");
//					CCSprite* s_help = CCSprite::create("ending_help_on.png");
//					s_help->setColor(ccGRAY);
//					
//					CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailPopup::cellAction));
//					help_item->setTag(kFFC_T_menuBase);
//					help_item->setUserData((void*)idx);
//					ScrollMenu* help_menu = ScrollMenu::create(help_item, NULL);
//					help_menu->setPosition(ccp(180,21));
//					cell->addChild(help_menu, kFFC_Z_img);
//					help_menu->setTouchPriority(-190+2);
//				}
//				else
//				{
//					CCSprite* d_help = CCSprite::create("ending_help_on.png");
//					d_help->setColor(ccGRAY);
//					d_help->setPosition(ccp(180,21));
//					cell->addChild(d_help, kFFC_Z_img);
//					
//					std::string remainStr = ::getRemainTimeMsg( remain_time );
//					CCLabelTTF* remainFnt = CCLabelTTF::create(remainStr.c_str(), mySGD->getFont().c_str(), 12.f);
//					remainFnt->setPosition(ccp(d_help->getContentSize().width/2.f, d_help->getContentSize().height/2.f));
//					d_help->addChild(remainFnt);
//				}
//			}
//		}
//	}
	
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

	//		CCLOG("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize FailPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake((480-30)/2.f-20, 28);//CCSizeMake(210, 45);
}

unsigned int FailPopup::numberOfCellsInTableView( CCTableView *table )
{
	return 9;// friend_list.size();
}
