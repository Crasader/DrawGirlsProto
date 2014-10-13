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
#include "TypingBox.h"
#include "StyledLabelTTF.h"
#include "LabelTTFMarquee.h"
#include "FiveRocksCpp.h"

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
	is_end_popup_animation = false;
	
	refresh_achieve_func = nullptr;
	
    fail_label = NULL;
    
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	myLog->addLog(kLOG_remainHeart_i, -1, myDSH->getIntegerForKey(kDSH_Key_heartCnt));
	
	send_command_list.clear();
	
	
//	send_command_list.push_back(myLog->getSendLogCommand(CCString::createWithFormat("fail_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)))->getCString()));
	
	if(mySGD->getScore() > myDSH->getIntegerForKey(kDSH_Key_allHighScore))
	{
		myDSH->setIntegerForKey(kDSH_Key_allHighScore, int(mySGD->getScore()));
	}
	Json::Value p1;
	p1["memberID"] = hspConnector::get()->getSocialID();
	p1["score"] = int(mySGD->getScore());
	p1["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	p1["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	Json::Value p1_data;
	p1_data["character"] = mySGD->getUserdataSelectedCharNO();
	p1_data["highPiece"] = mySGD->getUserdataHighPiece();
	p1["data"] = p1_data;
	
	send_command_list.push_back(CommandParam("addweeklyscore", p1, nullptr));
	
	this->endDecreaseCardDuration();
	
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
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f-450));
	addChild(main_case, kZ_FP_back);
	
	inner_left = CCScale9Sprite::create("common_doubleblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	inner_left->setContentSize(CCSizeMake(209, 177));
	inner_left->setPosition(132, main_case->getContentSize().height*0.58f+3);
	main_case->addChild(inner_left);
	
	
	CCScale9Sprite* inner_right = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	inner_right->setContentSize(CCSizeMake(209, 177));
	inner_right->setPosition(347, main_case->getContentSize().height*0.58f+3);
	main_case->addChild(inner_right);
	
	CCScale9Sprite* star_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	star_back->setContentSize(CCSizeMake(193, 50));
	star_back->setPosition(ccp(inner_left->getContentSize().width/2.f,119));
	inner_left->addChild(star_back);
	
	
	play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
	
	if(mySD->getClearCondition() == kCLEAR_timeLimit)
		play_limit_time = play_limit_time.getV() - mySD->getClearConditionTimeLimit();
	
	left_life_base_score = mySGD->area_score.getV() + mySGD->damage_score.getV() + mySGD->combo_score.getV();
	left_life_decrease_score = (mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_lifeBonusCnt)].asInt()*30000*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d));
	left_time_base_score = left_life_base_score.getV() + left_life_decrease_score.getV();
	
	if(mySD->getClearCondition() != kCLEAR_hellMode)
		left_time_decrease_score = 0;//(play_limit_time.getV()-mySGD->getGameTime())*500*NSDS_GD(mySD->getSilType(), kSDS_SI_scoreRate_d);
	else
		left_time_decrease_score = mySGD->getBaseScore();
	
	left_grade_base_score = left_time_base_score.getV() + left_time_decrease_score.getV();
	left_grade_decrease_score = left_grade_base_score.getV()*0.f;
	left_damaged_score = -mySGD->damaged_score.getV();
	
	left_total_score = left_grade_base_score.getV() + left_grade_decrease_score.getV() + left_damaged_score.getV();
	
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
	
	title_list.clear();
	left_content_list.clear();
	
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
	
	FailCode fail_code = mySGD->fail_code;
	
	string title_str;
	if(fail_code == kFC_timeover)
		title_str = myLoc->getLocalForKey(LK::kMyLocalKey_failTitleTimeover);
	else if(fail_code == kFC_missionfail)
		title_str = myLoc->getLocalForKey(LK::kMyLocalKey_failTitleMissionfail);
	else
		title_str = myLoc->getLocalForKey(LK::kMyLocalKey_failTitleGameover);
	
	KSLabelTTF* title_label = KSLabelTTF::create(title_str.c_str(), mySGD->getFont().c_str(), 21.5f);
	title_label->setColor(ccc3(240, 45, 45));
	title_label->setPosition(ccp(inner_left->getContentSize().width/2.f,154));
	inner_left->addChild(title_label, kZ_FP_img);
	
	CCSprite* stage_tab = CCSprite::create("title_tab.png");
	stage_tab->setPosition(ccp(76,253));
	main_case->addChild(stage_tab);
	
	if(mySGD->is_before_selected_event_stage)
	{
		int stage_number = mySD->getSilType();
		
		KSLabelTTF* stage_number_label = KSLabelTTF::create(CCString::createWithFormat("STAGE  %d", stage_number)->getCString(),	mySGD->getFont().c_str(), 11);
		stage_number_label->setColor(ccc3(20, 40, 70));
		stage_number_label->disableOuterStroke();
		stage_number_label->setPosition(ccpFromSize(stage_tab->getContentSize()/2.f) + ccp(0,1));
		stage_tab->addChild(stage_number_label);
		
		mySGD->is_before_selected_event_stage = false;
	}
	else
	{
		int stage_number = mySD->getSilType();
//		int puzzle_number = NSDS_GI(stage_number, kSDS_SI_puzzle_i);
//		int piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number);
		
		KSLabelTTF* piece_number_label = KSLabelTTF::create(CCString::createWithFormat("STAGE  %d", stage_number)->getCString(),	mySGD->getFont().c_str(), 11);
		piece_number_label->setColor(ccc3(20, 40, 70));
		piece_number_label->disableOuterStroke();
		piece_number_label->setPosition(ccpFromSize(stage_tab->getContentSize()/2.f) + ccp(0,1));
		stage_tab->addChild(piece_number_label);
	}
	
	fiverocks::FiveRocksBridge::trackEvent("Game", "StageResult", "Fail", ccsf("Stage %03d", mySD->getSilType()));
	if(mySGD->ingame_continue_cnt > 0)
	{
		fiverocks::FiveRocksBridge::trackEvent("UseGem", "IngameContinue1", ccsf("Continue %02d", mySGD->ingame_continue_cnt), ccsf("Stage %03d", mySD->getSilType()));
	}
	
	for(int i=0;i<4;i++)
	{
		CCSprite* t_star = CCSprite::create("ending_star_empty.png");
		t_star->setPosition(ccp(inner_left->getPositionX() - 1.5f*48 + i*48,196));
		main_case->addChild(t_star, kZ_FP_img);
		setFormSetter(t_star);
	}
	
	CCScale9Sprite* left_total_back = CCScale9Sprite::create("common_lightgray.png", CCRectMake(0, 0, 18, 18), CCRectMake(8, 8, 2, 2));
	left_total_back->setContentSize(CCSizeMake(193, 20));
	left_total_back->setPosition(ccp(inner_left->getContentSize().width/2.f, 15));
	inner_left->addChild(left_total_back);
	
	KSLabelTTF* left_total_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_totalScore), mySGD->getFont().c_str(), 13);
	left_total_title->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
//	left_total_title->setColor(ccc3(255, 170, 20));
	left_total_title->setAnchorPoint(ccp(0,0.5f));
	left_total_title->setPosition(ccp(8, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_title);
	
	string start_total_left_content;
	start_total_left_content = "0";
	
	left_total_content = KSLabelTTF::create(start_total_left_content.c_str(), mySGD->getFont().c_str(), 15);
	left_total_content->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	left_total_content->setAnchorPoint(ccp(1,0.5f));
	left_total_content->setPosition(ccp(left_total_back->getContentSize().width-8, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_content);
	
	CCRect left_rect = CCRectMake(inner_left->getContentSize().width/2.f-((480-30)/2.f-20)/2.f, 51.f-30.f/2.f-9, (480-30)/2.f-20, 65);
	
//	CCSprite* left_size = CCSprite::create("whitePaper.png", CCRectMake(0, 0, left_rect.size.width, left_rect.size.height));
//	left_size->setOpacity(100);
//	left_size->setAnchorPoint(CCPointZero);
//	left_size->setPosition(left_rect.origin);
//	inner_left->addChild(left_size);
	
	left_table = CCTableView::create(this, left_rect.size);
	left_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	left_table->setPosition(left_rect.origin);
	inner_left->addChild(left_table);
	left_table->setTouchPriority(-200);
	left_table->setTouchEnabled(false);
	
	left_table->setContentOffset(ccp(0,-9*21));
	
	
	CCLabelTTF* t_ok_node = CCLabelTTF::create();
	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_toMain), mySGD->getFont().c_str(), 27.5f);
	ok_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	t_ok_node->addChild(ok_label);
	
	main_menu = CCControlButton::create(t_ok_node, CCScale9Sprite::create("mainbutton_purple.png", CCRectMake(0, 0, 215, 65), CCRectMake(107, 32, 1, 1)));
	main_menu->setStringData("backkey");
	main_menu->setPreferredSize(CCSizeMake(215, 65));
	main_menu->setTag(kMT_FP_main);
	main_menu->addTargetWithActionForControlEvents(this, cccontrol_selector(FailPopup::controlButtonAction), CCControlEventTouchUpInside);
	main_menu->setPosition(ccp(347,45));
	main_menu->setEnabled(false);
	main_menu->setVisible(false);
	main_case->addChild(main_menu, kZ_FP_menu);
	main_menu->setTouchPriority(-190);
	
	
	CCLabelTTF* t_replay_node = CCLabelTTF::create();
	KSLabelTTF* replay_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_regame), mySGD->getFont().c_str(), 27.5f);
	replay_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	t_replay_node->addChild(replay_label);
	
	replay_menu = CCControlButton::create(t_replay_node, CCScale9Sprite::create("mainbutton_green.png", CCRectMake(0, 0, 215, 65), CCRectMake(107, 32, 1, 1)));
	replay_menu->setPreferredSize(CCSizeMake(215, 65));
	replay_menu->setTag(kMT_FP_replay);
	replay_menu->addTargetWithActionForControlEvents(this, cccontrol_selector(FailPopup::controlButtonAction), CCControlEventTouchUpInside);
	replay_menu->setPosition(ccp(132,45));
	replay_menu->setEnabled(false);
	replay_menu->setVisible(false);
	main_case->addChild(replay_menu, kZ_FP_menu);
	replay_menu->setTouchPriority(-190);
	
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	loading_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_img = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_img->setPosition(ccp(347,130));
	main_case->addChild(loading_img, kZ_FP_img);
//	reader->release();
	
	is_end_take_diary = false;
	is_end_network = false;
	
	if(myDSH->getIntegerForKey(kDSH_Key_showedScenario) == 6)
	{
		myDSH->setIntegerForKey(kDSH_Key_showedScenario, 1000);
		
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
		
		
		CCSprite* t_gray = CCSprite::create("whitePaper.png");
		t_gray->setScaleX(screen_scale_x);
		t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
		t_gray->setColor(ccBLACK);
		t_gray->setOpacity(0);
		t_gray->setPosition(ccp(240,160));
		t_clipping->addChild(t_gray);
		
		t_clipping->setInverted(true);
		scenario_node->addChild(t_clipping, 0);
		
		
		CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
		ikaruga->setAnchorPoint(ccp(0,0));
		ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
		scenario_node->addChild(ikaruga, 1);
		
		CCSprite* asuka = CCSprite::create("kt_cha_asuka_1.png");
		asuka->setAnchorPoint(ccp(1,0));
		asuka->setPosition(ccp(240+240*screen_scale_x+asuka->getContentSize().width - asuka->getContentSize().width*2.f/3.f, 160-160*screen_scale_y));
		scenario_node->addChild(asuka);
		asuka->setVisible(false);
		
		TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
		typing_box->setHide();
		scenario_node->addChild(typing_box, 2);
		
		TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 60), ccp(255, 60));
		scenario_node->addChild(typing_box2, 2);
		
		typing_box2->setTouchOffScrollAndButton();
		typing_box2->setVisible(false);
		typing_box2->setTouchSuction(false);
		
		CCSprite* n_skip = CCSprite::create("kt_skip.png");
		CCSprite* s_skip = CCSprite::create("kt_skip.png");
		s_skip->setColor(ccGRAY);
		
		CCMenuLambda* skip_menu = CCMenuLambda::create();
		skip_menu->setPosition(ccp(240-240*screen_scale_x + 35, 160+160*screen_scale_y - 25 + 150));
		scenario_node->addChild(skip_menu, 3);
		skip_menu->setTouchPriority(-19999);
		skip_menu->setEnabled(false);
		
		CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [=](CCObject* sender)
																	 {
																		 skip_menu->setEnabled(false);
																		 
																		 addChild(KSTimer::create(0.1f, [=]()
																								  {
																									  if(myDSH->getIntegerForKey(kDSH_Key_savedStartPackFirstTime) == 0)
																									  {
																										  chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
																										  std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
																										  myDSH->setIntegerForKey(kDSH_Key_savedStartPackFirstTime, now_time_t);
																										  CCLOG("saved StartPack time : %ld", now_time_t);
																									  }
																									  
																										is_end_take_diary = true;
																										if(is_end_take_diary && is_end_network)
																										{
																											if(is_today_mission_success)
																											{
																												mySGD->is_today_mission_first = false;
																												TodayMissionPopup* t_popup = TodayMissionPopup::create(-280, [=](){});
																												addChild(t_popup, kZ_FP_popup);
																											}
																										}
																									  
																									  scenario_node->removeFromParent();
																								  }));
																	 });
		skip_menu->addChild(skip_item);
		
		typing_box->showAnimation(0.3f);
		
		function<void()> end_func4 = [=]()
		{
			skip_menu->setEnabled(false);
			
			addChild(KSTimer::create(0.1f, [=]()
									 {
										 if(myDSH->getIntegerForKey(kDSH_Key_savedStartPackFirstTime) == 0)
										 {
											 chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
											 std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
											 myDSH->setIntegerForKey(kDSH_Key_savedStartPackFirstTime, now_time_t);
											 CCLOG("saved StartPack time : %ld", now_time_t);
										 }
										 
										 is_end_take_diary = true;
										 if(is_end_take_diary && is_end_network)
										 {
											 if(is_today_mission_success)
											 {
												 mySGD->is_today_mission_first = false;
												 TodayMissionPopup* t_popup = TodayMissionPopup::create(-280, [=](){});
												 addChild(t_popup, kZ_FP_popup);
											 }
										 }
										 
										 scenario_node->removeFromParent();
									 }));
		};
		
		function<void()> end_func3 = [=]()
		{
			TypingBox::changeTypingBox(typing_box, typing_box2, ikaruga, asuka);
			typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent31), end_func4);
		};
		
		function<void()> end_func2 = [=]()
		{
			ikaruga->setVisible(true);
			
			typing_box->setVisible(true);
			typing_box->setTouchSuction(true);
			
			typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent30), end_func3);
		};
		
		function<void()> end_func1 = [=]()
		{
			skip_menu->setVisible(false);
			
			ikaruga->setVisible(false);
			
			typing_box->setTouchOffScrollAndButton();
			typing_box->setVisible(false);
			
			CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
			t_arrow1->setScale(0.5f);
			t_arrow1->setRotation(-90);
			t_arrow1->setPosition(ccp(132,20-14) + ccp(0,280*0.58f+33.5f) + ccp(0,48));
			t_clipping->addChild(t_arrow1);
			
			StyledLabelTTF* t_ment1 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent29), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_ment1->setAnchorPoint(ccp(0.5f,0));
			t_ment1->setPosition(t_arrow1->getPosition() + ccp(0, t_arrow1->getContentSize().height/2.f*t_arrow1->getScale() + 3));
			t_clipping->addChild(t_ment1);
			
			CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_stencil1->setContentSize(CCSizeMake(200, 145));
			t_stencil1->setPosition(ccp(132,20-14) + ccp(0,280*0.58f-11.5f));
			t_stencil_node->addChild(t_stencil1);
			
			
			CCSprite* t_arrow2 = CCSprite::create("kt_arrow_big.png");
			t_arrow2->setScale(0.5f);
			t_arrow2->setRotation(90);
			t_arrow2->setPosition(ccp(132,20-14) + ccp(0,280*0.58f-115.f) + ccp(0,5));
			t_clipping->addChild(t_arrow2);
			
			StyledLabelTTF* t_ment2 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent28), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_ment2->setAnchorPoint(ccp(0.5f,1));
			t_ment2->setPosition(t_arrow2->getPosition() + ccp(0, -t_arrow2->getContentSize().height/2.f*t_arrow2->getScale() - 3));
			t_clipping->addChild(t_ment2);
			
			
			
			CCSprite* t_arrow3 = CCSprite::create("kt_arrow_big.png");
			t_arrow3->setScale(0.5f);
			t_arrow3->setRotation(90);
			t_arrow3->setPosition(ccp(347,20-14) + ccp(0,280*0.58f-115) + ccp(0,5));
			t_clipping->addChild(t_arrow3);
			
			StyledLabelTTF* t_ment3 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent27), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_ment3->setAnchorPoint(ccp(0.5f,1));
			t_ment3->setPosition(t_arrow3->getPosition() + ccp(0, -t_arrow3->getContentSize().height/2.f*t_arrow3->getScale() - 3));
			t_clipping->addChild(t_ment3);
			
			CCScale9Sprite* t_stencil3 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_stencil3->setContentSize(CCSizeMake(210, 180));
			t_stencil3->setPosition(ccp(347,20-14) + ccp(0,280*0.58f+3));
			t_stencil_node->addChild(t_stencil3);
			
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
			scenario_node->addChild(t_suction);
			t_suction->setTouchEnabled(true);
			t_suction->touch_began_func = [=]()
			{
				skip_menu->setVisible(true);
				t_suction->is_on_touch_began_func = false;
				t_stencil_node->removeAllChildren();
				t_arrow1->removeFromParent();
				t_arrow2->removeFromParent();
				t_arrow3->removeFromParent();
				t_ment1->removeFromParent();
				t_ment2->removeFromParent();
				t_ment3->removeFromParent();
				end_func2();
				t_suction->removeFromParent();
			};
			t_suction->is_on_touch_began_func = true;
			
			typing_box->setTouchSuction(false);
		};
		
		scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
															  {
																  t_gray->setOpacity(t*255*0.8f);
																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
															  }, [=](float t)
															  {
																  t_gray->setOpacity(255*0.8f);
																  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
																  skip_menu->setEnabled(true);
																  
																  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent32), end_func1);
															  }));
	}
	else if(mySD->getSilType() >= 5 && mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_emptyItem) && mySGD->getGoodsValue(kGoodsType_item9) + mySGD->getGoodsValue(kGoodsType_item6) + mySGD->getGoodsValue(kGoodsType_item11) <= 0)
	{
		EmptyItemSalePopup* t_popup = EmptyItemSalePopup::create(-300, [=]()
		{
			is_end_take_diary = true;
			if(is_end_take_diary && is_end_network)
			{
				if(is_today_mission_success)
				{
					mySGD->is_today_mission_first = false;
					TodayMissionPopup* t_popup = TodayMissionPopup::create(-280, [=](){});
					addChild(t_popup, kZ_FP_popup);
				}
			}
		}, [=](){}, kPurchaseGuideType_emptyItem);
		addChild(t_popup, kZ_FP_popup+1);
	}
	else if(mySD->getSilType() >= 5 && mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_stupidNpuHelp) && mySGD->getGoodsValue(kGoodsType_item9) + mySGD->getGoodsValue(kGoodsType_item6) + mySGD->getGoodsValue(kGoodsType_item11) <= 0 &&
			mySGD->getUserdataTotalPlayCount() >= mySGD->getStupidNpuHelpPlayCount() && mySGD->getUserdataFailCount()+1 >= mySGD->getStupidNpuHelpFailCount())
	{
		EmptyItemSalePopup* t_popup = EmptyItemSalePopup::create(-300, [=]()
		{
			is_end_take_diary = true;
			if(is_end_take_diary && is_end_network)
			{
				if(is_today_mission_success)
				{
					mySGD->is_today_mission_first = false;
					TodayMissionPopup* t_popup = TodayMissionPopup::create(-280, [=](){});
					addChild(t_popup, kZ_FP_popup);
				}
			}
		}, [=](){}, kPurchaseGuideType_stupidNpuHelp);
		addChild(t_popup, kZ_FP_popup+1);
	}
//	else if(mySGD->getPlayCountHighIsOn() != 0 && mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_eventRubyShop) && mySGD->getUserdataTotalPlayCount() >= mySGD->getPlayCountHighValue()) // test
//	{
//		EventShopPopup* t_popup = EventShopPopup::create(-300, [=]()
//		{
//			if(is_today_mission_success)
//			{
//				mySGD->is_today_mission_first = false;
//				TodayMissionPopup* t_popup = TodayMissionPopup::create(-280, [=](){});
//				addChild(t_popup, kZ_FP_popup);
//			}
//		});
//		addChild(t_popup, kZ_FP_popup+1);
//	}
	else
	{
		is_end_take_diary = true;
		if(is_end_take_diary && is_end_network)
		{
			if(is_today_mission_success)
			{
				mySGD->is_today_mission_first = false;
				TodayMissionPopup* t_popup = TodayMissionPopup::create(-280, [=](){});
				addChild(t_popup, kZ_FP_popup);
			}
		}
	}
	
	Json::Value param2;
	param2["myScore"]=int(mySGD->getScore());
	param2["stageNo"]=mySD->getSilType();
	param2["memberID"] = hspConnector::get()->getSocialID();
	param2["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	param2["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	Json::Value p2_data;
	p2_data["character"] = mySGD->getUserdataSelectedCharNO();
	p2_data["highPiece"] = mySGD->getUserdataHighPiece();
	param2["data"] = p2_data;
	
	send_command_list.push_back(CommandParam("getstagerankbyalluser", param2, json_selector(this, FailPopup::resultGetRank)));
	mySGD->keep_time_info.is_loaded = false;
	send_command_list.push_back(CommandParam("gettimeinfo", Json::Value(), json_selector(this, FailPopup::resultGetTime)));
	
	if(mySD->getSilType() >= mySGD->getIntroduceStage() && mySGD->getIntroducerID()>0)
	{
		Json::Value t_param;
		t_param["memberID"] = myHSP->getSocialID();
		t_param["content"] = myLoc->getLocalForKey(LK::kMyLocalKey_introducerCompleteReward);
		send_command_list.push_back(CommandParam("completeIntroducer", t_param, json_selector(this, FailPopup::resultCompleteIntroducer)));
	}
	
	mySGD->setUserdataAchieveNoFail(0);
	
	for(int i=kAchievementCode_fail1;i<=kAchievementCode_fail3;i++)
	{
		if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
		   mySGD->getUserdataAchieveFail() + 1 >= myAchieve->getCondition((AchievementCode)i))
		{
			myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
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
	
	is_today_mission_success = false;//mySGD->today_mission_info.is_success.getV();
	
	send_command_list.push_back(mySGD->getUpdateTodayMissionParam([=](Json::Value result_data)
																  {
																	  TRACE();
																	  if(result_data["result"]["code"].asInt() == GDSUCCESS)
																	  {
																		  TRACE();
                                                                          if(result_data["isFirstCheck"].asBool())
                                                                          {
                                                                              is_today_mission_success = true;
                                                                          }
																		  
																		  else
																			{
																				is_today_mission_success = false;
																			}
																			
																			if(is_end_take_diary && is_end_network)
																			{
																				if(is_today_mission_success)
																				{
																					mySGD->is_today_mission_first = false;
																					TodayMissionPopup* t_popup = TodayMissionPopup::create(-280, [=](){});
																					addChild(t_popup, kZ_FP_popup);
																				}
																			}
																	  }
																	  TRACE();
																  }));
	
	
	is_saved_user_data = false;
	
	CCNode* curtain_node = LoadingTipScene::getOpenCurtainNode(true);
	curtain_node->setPosition(ccp(240,160));
	addChild(curtain_node, kZ_FP_popup+5);
	
	return true;
}

void FailPopup::resultCompleteIntroducer(Json::Value result_data)
{
	if(result_data["result"]["code"] == GDSUCCESS)
	{
		mySGD->setIntroducerID(mySGD->getIntroducerID()*-1);
	}
}

void FailPopup::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
	LoadingLayer* t_loading = LoadingLayer::create(-9999);
	addChild(t_loading, 9999);
	
	tryTransaction(t_loading);
	
	is_saved_user_data = true;
//	myDSH->saveAllUserData(json_selector(this, FailPopup::resultSavedUserData));
}

void FailPopup::controlButtonAction(CCObject* sender, CCControlEvent t_event)
{
	menuAction(sender);
}

void FailPopup::tryTransaction(CCNode* t_loading)
{
	TRACE();
	if(loading_img)
	{
		loading_img->setVisible(true);
	}
	
	mySGD->changeGoodsTransaction(send_command_list, [=](Json::Value result_data)
								  {
									  TRACE();
									  if(result_data["result"]["code"].asInt() == GDSUCCESS)
									  {
										  CCLOG("FailPopup transaction success");
										  
										  mySGD->network_check_cnt = 0;
										  
										  t_loading->removeFromParent();
										  
										  if(refresh_achieve_func != nullptr)
										  {
											  addChild(KSTimer::create(0.1f, refresh_achieve_func));
										  }
											
											is_end_network = true;
									  }
									  else
									  {
										  CCLOG("FailPopup transaction fail");
										  
										  mySGD->network_check_cnt++;
										  
										  if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
										  {
											  mySGD->network_check_cnt = 0;
											  
											  ASPopupView* alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
												  tryTransaction(t_loading);
											  }, 1);
											  
//											  ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
//												  tryTransaction(t_loading);
//											  });
											  if(alert)
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
									  TRACE();
								  });
}

FailPopup::~FailPopup()
{
	hspConnector::get()->removeTarget(this);
}

void FailPopup::resultGetTime(Json::Value result_data)
{
	TRACE();
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		TRACE();
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
						if(!mySGD->getPuzzleHistory(t_condition["value"].asInt()).is_clear.getV())
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
	TRACE();
}

void FailPopup::resultGetRank(Json::Value result_data)
{
	TRACE();
    
    if(fail_label)
    {
        fail_label->removeFromParent();
        fail_label = NULL;
    }
    
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		TRACE();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		
		CCSprite* graph_back = CCSprite::create("ending_graph.png");
		graph_back->setPosition(ccp(347,240));
		main_case->addChild(graph_back, kZ_FP_img);
		setFormSetter(graph_back);
		
//		KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
//		t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_a->setPosition(ccp(25,8));
//		graph_back->addChild(t_rank_a);
//		
//		KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
//		t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_b->setPosition(ccp(25+49,8));
//		graph_back->addChild(t_rank_b);
//		
//		KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
//		t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_c->setPosition(ccp(25+98,8));
//		graph_back->addChild(t_rank_c);
//		
//		KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
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
//		CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_myrankValue), myrank)->getCString(), mySGD->getFont().c_str(), 10);
//		my_rank_label->setAnchorPoint(ccp(1,0.5));
//		my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
//		main_case->addChild(my_rank_label, kZ_FP_img);
		
		float rank_percent = alluser == 0 ? 1.f : 1.f * myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,1));
		rank_percent_case->setPosition(ccp(249+195,240));
		main_case->addChild(rank_percent_case, kZ_FP_img);
		setFormSetter(rank_percent_case);
		
		KSLabelTTF* percent_label = KSLabelTTF::create("100%", mySGD->getFont().c_str(), 12);
		addChild(KSGradualValue<float>::create(100.f, rank_percent*100.f,
																					 2.f * (1.f - rank_percent), [=](float t){
																					 percent_label->setString(ccsf("%.0f%%", t));
		}, [=](float t){
			percent_label->setString(ccsf("%.0f%%", t));
		}));
//		percent_label->setColor(ccc3(255, 170, 20));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f-3));
		rank_percent_case->addChild(percent_label, kZ_FP_img);
		
		CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(249 + 195.f*rank_percent,240));
		rank_percent_case->runAction(t_move);
		
		Json::Value user_list = result_data["list"];
		
		int delay_index = 0;
		int limit_count = 3;
		for(int i=0;i<user_list.size() && i<limit_count;i++)
		{
			string case_name;
			if(myrank == i+1)
			{
				case_name = "mainpopup_purple.png";
				limit_count++;
			}
			else
			{
				case_name = "rank_normal2.png";
			}
			
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			list_cell_case->setContentSize(CCSizeMake(204, 31));
			list_cell_case->setPosition(ccp(347,191-i*32));
			main_case->addChild(list_cell_case, kZ_FP_img);
			setFormSetter(list_cell_case);
			
			CCPoint rank_position = ccp(17,15.5f);
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
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 10);
				rank_label->setPosition(ccp(33,rank_position.y+8));
				list_cell_case->addChild(rank_label);
			}
			
			Json::Reader reader;
			Json::Value read_data;
			reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
			
			string flag = user_list[i].get("flag", "kr").asString().c_str();
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			if(i >= 3)
				selectedFlagSpr->setPosition(ccp(33,15.5f-5));
			else
				selectedFlagSpr->setPosition(ccp(49,15.5f));
			selectedFlagSpr->setScale(0.8);
			list_cell_case->addChild(selectedFlagSpr);
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12.5f);
			if(t_nick_size->getContentSize().width > 70)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 70, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font strokecolor=000 strokesize=0.3f strokeopacity=50>" + user_list[i].get("nick", Json::Value()).asString() + "</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.5f);
				nick_marquee->setAnchorPoint(ccp(0,0.5f));
				nick_marquee->setPosition(ccp(64,15.5f));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12.5f); // user_list[i]["nick"].asString().c_str()
				nick_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				nick_label->setAnchorPoint(ccp(0,0.5f));
				nick_label->setPosition(ccp(64,15.5f));
				list_cell_case->addChild(nick_label);
			}
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",user_list[i]["score"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 13);
			score_label->disableOuterStroke();
			score_label->setAnchorPoint(ccp(1,0.5));
			score_label->setColor(ccc3(55, 35, 150));
			score_label->setPosition(ccp(198,15.5f));
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
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_purple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			list_cell_case->setContentSize(CCSizeMake(204, 31));
			list_cell_case->setPosition(ccp(347,191-3*32));
			main_case->addChild(list_cell_case, kZ_FP_img);
			
			KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", myrank)->getCString(), mySGD->getFont().c_str(), 10);
			rank_label->setPosition(ccp(33,15.5f+8));
			list_cell_case->addChild(rank_label);
			
			string flag = myDSH->getStringForKey(kDSH_Key_flag);
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setPosition(ccp(33,15.5f-5));
			selectedFlagSpr->setScale(0.8);
			list_cell_case->addChild(selectedFlagSpr);
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12.5f);
			if(t_nick_size->getContentSize().width > 70)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 70, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font strokecolor=000 strokesize=0.3f strokeopacity=50>" + myDSH->getStringForKey(kDSH_Key_nick) + "</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.5f);
				nick_marquee->setAnchorPoint(ccp(0,0.5f));
				nick_marquee->setPosition(ccp(64,15.5f));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12.5f); // user_list[i]["nick"].asString().c_str()
				nick_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				nick_label->setAnchorPoint(ccp(0,0.5f));
				nick_label->setPosition(ccp(64,15.5f));
				list_cell_case->addChild(nick_label);
			}
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",int(mySGD->getScore()))->getCString()).c_str(), mySGD->getFont().c_str(), 13);
			score_label->disableOuterStroke();
			score_label->setAnchorPoint(ccp(1,0.5));
			score_label->setColor(ccc3(55, 35, 150));
			score_label->setPosition(ccp(198,15.5f));
			list_cell_case->addChild(score_label);
			
			CCPoint original_position = list_cell_case->getPosition();
			list_cell_case->setPosition(ccpAdd(original_position, ccp(500, 0)));
			CCDelayTime* t_delay = CCDelayTime::create(0.5f + delay_index*0.2f);
			CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
			CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
			list_cell_case->runAction(t_seq);
		}
		if(loading_img)
		{
			loading_img->removeFromParent();
			loading_img = NULL;
		}
	}
	else
	{
		TRACE();
		fail_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
		if(loading_img)
			fail_label->setPosition(loading_img->getPosition());
		else
			fail_label->setPosition(ccp(-500,-500));
		main_case->addChild(fail_label, kZ_FP_img);
		loading_img->setVisible(false);
	}
	TRACE();
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
		main_menu->setEnabled(true);
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) > 0)
		{
			if(replay_menu)
			{
				replay_menu->setVisible(true);
				replay_menu->setEnabled(true);
			}
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
//		myDSH->saveAllUserData(json_selector(this, FailPopup::resultSavedUserData));
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
		main_menu->setEnabled(true);
		main_menu->setVisible(true);
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) > 0)
		{
			if(replay_menu)
			{
				replay_menu->setEnabled(true);
				replay_menu->setVisible(true);
			}
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
			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_timeOutFrame)), 9999);
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
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	if(table == left_table)
	{
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
		
		content = left_content_list[idx];
		
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

	//		CCLOG("%s", m_scoreList[cell->getIdx()]["user_id"].asString().c_str());
}

CCSize FailPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake((480-30)/2.f-20, 21);//CCSizeMake(210, 45);
}

unsigned int FailPopup::numberOfCellsInTableView( CCTableView *table )
{
	return 9;// friend_list.size();
}
