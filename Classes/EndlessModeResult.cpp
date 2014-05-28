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
	
	if(mySGD->endless_my_victory_on.getV())
	{
		is_calc = true;
		mySGD->endless_my_victory_on = false;
		
		float play_limit_time = NSDS_GI(mySD->getSilType(), kSDS_SI_playtime_i);
		
		if(mySD->getClearCondition() == kCLEAR_timeLimit)
		{
			play_limit_time -= mySD->getClearConditionTimeLimit();
		}
		mySGD->temp_endless_play_limit_time = play_limit_time;
		mySGD->temp_replay_data = mySGD->replay_playing_info;
		mySGD->temp_endless_nick = mySGD->endless_nick.getV();
		mySGD->temp_endless_flag = mySGD->endless_flag.getV();
		
		int left_life_base_score = mySGD->area_score.getV() + mySGD->damage_score.getV() + mySGD->combo_score.getV();
		float left_life_decrease_score = left_life_base_score*(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_lifeBonusCnt)].asInt()*0.1f);
		float left_time_base_score = left_life_base_score + left_life_decrease_score;
		float left_time_decrease_score = left_time_base_score*(play_limit_time-mySGD->getGameTime())/play_limit_time;
		float left_grade_base_score = left_time_base_score + left_time_decrease_score;
		float left_grade_decrease_score = left_time_base_score*mySGD->getStageGrade()*0.5f;
		
		int left_total_score = left_grade_base_score + left_grade_decrease_score;
		
		
		int right_area_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_areaScore), Json::Value()).asInt();
		int right_damage_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_damageScore), Json::Value()).asInt();
		int right_combo_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_comboScore), Json::Value()).asInt();
		
		int right_life_cnt = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_lifeBonusCnt), Json::Value()).asInt();
		int right_game_time = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_gameTime), Json::Value()).asInt();
		int right_clear_grade = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_clearGrade), Json::Value()).asInt();
		
		
		int right_life_base_score = right_area_score + right_damage_score + right_combo_score;
		float right_life_decrease_score = right_life_base_score*(right_life_cnt*0.1f);
		float right_time_base_score = right_life_base_score + right_life_decrease_score;
		float right_time_decrease_score = right_time_base_score*(play_limit_time-right_game_time)/play_limit_time;
		float right_grade_base_score = right_time_base_score + right_time_decrease_score;
		float right_grade_decrease_score = right_time_base_score*right_clear_grade*0.5f;
		
		int right_total_score = right_grade_base_score + right_grade_decrease_score;
		
		if(left_total_score > right_total_score)
		{
			mySGD->endless_my_victory = mySGD->endless_my_victory.getV() + 1;
		}
		
		mySGD->endless_my_total_score = mySGD->endless_my_total_score.getV() + mySGD->getScore();
	}
	else
	{
//		is_calc = true;
		is_calc = false;
	}
	
	setMain();
	
	is_menu_enable = true;
	
	return true;
}

void EndlessModeResult::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

void EndlessModeResult::setMain()
{
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kEndlessModeResultZorder_back);
	
	CCScale9Sprite* left_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
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
	left_flag->setPosition(ccp(25,26));
	left_title->addChild(left_flag);
	
	KSLabelTTF* left_nick = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 13);
	left_nick->enableOuterStroke(ccBLACK, 1.f);
	left_nick->setPosition(ccp(97, 24));
	left_title->addChild(left_nick);
	
	left_top_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_top_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_top_back->setPosition(ccp(left_back->getContentSize().width/2.f, 111.f));
	left_back->addChild(left_top_back);
	
	string start_top_title, start_center_title, start_bottom_title;
	string start_top_left_content, start_top_right_content, start_center_left_content, start_center_right_content, start_bottom_left_content, start_bottom_right_content;
	string start_total_left_content, start_total_right_content;
	
	float play_limit_time = mySGD->temp_endless_play_limit_time.getV();
	
	int left_life_base_score = mySGD->area_score.getV() + mySGD->damage_score.getV() + mySGD->combo_score.getV();
	float left_life_decrease_score = left_life_base_score*(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_lifeBonusCnt)].asInt()*0.1f);
	float left_time_base_score = left_life_base_score + left_life_decrease_score;
	float left_time_decrease_score = left_time_base_score*(play_limit_time-mySGD->getGameTime())/play_limit_time;
	float left_grade_base_score = left_time_base_score + left_time_decrease_score;
	float left_grade_decrease_score = left_time_base_score*mySGD->getStageGrade()*0.5f;
	
	int left_total_score = left_grade_base_score + left_grade_decrease_score;
	
	
	int right_area_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_areaScore), Json::Value()).asInt();
	int right_damage_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_damageScore), Json::Value()).asInt();
	int right_combo_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_comboScore), Json::Value()).asInt();
	
	int right_life_cnt = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_lifeBonusCnt), Json::Value()).asInt();
	int right_game_time = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_gameTime), Json::Value()).asInt();
	int right_clear_grade = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_clearGrade), Json::Value()).asInt();
	
	
	int right_life_base_score = right_area_score + right_damage_score + right_combo_score;
	float right_life_decrease_score = right_life_base_score*(right_life_cnt*0.1f);
	float right_time_base_score = right_life_base_score + right_life_decrease_score;
	float right_time_decrease_score = right_time_base_score*(play_limit_time-right_game_time)/play_limit_time;
	float right_grade_base_score = right_time_base_score + right_time_decrease_score;
	float right_grade_decrease_score = right_time_base_score*right_clear_grade*0.5f;
	
	int right_total_score = right_grade_base_score + right_grade_decrease_score;
	
	if(is_calc)
	{
		start_top_title = myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleAreaScore);
		start_center_title = myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleDamageScore);
		start_bottom_title = myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleComboScore);
		
		start_top_left_content = CCString::createWithFormat("%d", mySGD->area_score.getV())->getCString();
		start_center_left_content = CCString::createWithFormat("%d", mySGD->damage_score.getV())->getCString();
		start_bottom_left_content = CCString::createWithFormat("%d", mySGD->combo_score.getV())->getCString();
		start_total_left_content = "0";
		
		start_top_right_content = CCString::createWithFormat("%d", right_area_score)->getCString();
		start_center_right_content = CCString::createWithFormat("%d", right_damage_score)->getCString();
		start_bottom_right_content = CCString::createWithFormat("%d", right_combo_score)->getCString();
		start_total_right_content = "0";
	}
	else
	{
		start_top_title = myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeGold);
		start_center_title = myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeArea);
		start_bottom_title = "";
		
		start_top_left_content = CCString::createWithFormat("%d", mySGD->getStageGold())->getCString();
		start_center_left_content = CCString::createWithFormat("%d", int(mySGD->getPercentage()*100.f))->getCString();
		start_bottom_left_content = "";
		start_total_left_content = CCString::createWithFormat("%d", left_total_score)->getCString();
		
		start_top_right_content = CCString::createWithFormat("%d", mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_takeGold), Json::Value()).asInt())->getCString();
		start_center_right_content = CCString::createWithFormat("%.0f", mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_takeArea), Json::Value()).asFloat())->getCString();
		start_bottom_right_content = "";
		start_total_right_content = CCString::createWithFormat("%d", right_total_score)->getCString();
	}
	
	left_top_title = KSLabelTTF::create(start_top_title.c_str(), mySGD->getFont().c_str(), 13);
	left_top_title->setAnchorPoint(ccp(0,0.5f));
	left_top_title->setPosition(ccp(10, left_top_back->getContentSize().height/2.f));
	left_top_back->addChild(left_top_title);
	
//	CCSprite* left_score_img = CCSprite::create("ending_mark_score.png");
//	left_score_img->setPosition(ccp(10 + left_score_title->getContentSize().width + 15,left_score_back->getContentSize().height/2.f));
//	left_score_back->addChild(left_score_img);
	
	left_top_content = KSLabelTTF::create(start_top_left_content.c_str(), mySGD->getFont().c_str(), 13);
	left_top_content->setAnchorPoint(ccp(1,0.5f));
	left_top_content->setPosition(ccp(left_top_back->getContentSize().width-10, left_top_back->getContentSize().height/2.f));
	left_top_back->addChild(left_top_content);
	
	
	left_center_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_center_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_center_back->setPosition(ccp(left_back->getContentSize().width/2.f, 81.f));
	left_back->addChild(left_center_back);
	
	left_center_title = KSLabelTTF::create(start_center_title.c_str(), mySGD->getFont().c_str(), 13);
	left_center_title->setAnchorPoint(ccp(0,0.5f));
	left_center_title->setPosition(ccp(10, left_center_back->getContentSize().height/2.f));
	left_center_back->addChild(left_center_title);
	
//	CCSprite* left_time_img = CCSprite::create("ending_mark_time.png");
//	left_time_img->setPosition(ccp(10 + left_time_title->getContentSize().width + 15,left_time_back->getContentSize().height/2.f));
//	left_time_back->addChild(left_time_img);
	
	left_center_content = KSLabelTTF::create(start_center_left_content.c_str(), mySGD->getFont().c_str(), 13);
	left_center_content->setAnchorPoint(ccp(1,0.5f));
	left_center_content->setPosition(ccp(left_center_back->getContentSize().width-10, left_center_back->getContentSize().height/2.f));
	left_center_back->addChild(left_center_content);
	
	
	left_bottom_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_bottom_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_bottom_back->setPosition(ccp(left_back->getContentSize().width/2.f, 51.f));
	left_back->addChild(left_bottom_back);
	
	left_bottom_title = KSLabelTTF::create(start_bottom_title.c_str(), mySGD->getFont().c_str(), 13);
	left_bottom_title->setAnchorPoint(ccp(0,0.5f));
	left_bottom_title->setPosition(ccp(10, left_bottom_back->getContentSize().height/2.f));
	left_bottom_back->addChild(left_bottom_title);
	
//	CCSprite* left_gold_img = CCSprite::create("ending_mark_gold.png");
//	left_gold_img->setPosition(ccp(10 + left_gold_title->getContentSize().width + 15,left_gold_back->getContentSize().height/2.f));
//	left_gold_back->addChild(left_gold_img);
	
	left_bottom_content = KSLabelTTF::create(start_bottom_left_content.c_str(), mySGD->getFont().c_str(), 13);
	left_bottom_content->setAnchorPoint(ccp(1,0.5f));
	left_bottom_content->setPosition(ccp(left_bottom_back->getContentSize().width-10, left_bottom_back->getContentSize().height/2.f));
	left_bottom_back->addChild(left_bottom_content);
	
	
	CCScale9Sprite* left_total_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	left_total_back->setContentSize(CCSizeMake(left_back->getContentSize().width-20, 35));
	left_total_back->setPosition(ccp(left_back->getContentSize().width/2.f, 21));
	left_back->addChild(left_total_back);
	
	KSLabelTTF* left_total_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_totalScore), mySGD->getFont().c_str(), 15);
	left_total_title->setColor(ccc3(255, 170, 20));
	left_total_title->setAnchorPoint(ccp(0,0.5f));
	left_total_title->setPosition(ccp(10, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_title);
	
	left_total_content = KSLabelTTF::create(start_total_left_content.c_str(), mySGD->getFont().c_str(), 15);
	left_total_content->setColor(ccc3(255, 170, 20));
	left_total_content->setAnchorPoint(ccp(1,0.5f));
	left_total_content->setPosition(ccp(left_total_back->getContentSize().width-10, left_total_back->getContentSize().height/2.f));
	left_total_back->addChild(left_total_content);
	
	
	
	
	CCSprite* n_stop = CCSprite::create("endless_ready.png");
	
	n_stop_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 22);
	n_stop_label2->setColor(ccWHITE);
	n_stop_label2->setOpacity(100);
	n_stop_label2->setPosition(ccp(n_stop->getContentSize().width/2.f, n_stop->getContentSize().height/2.f-2));
	n_stop->addChild(n_stop_label2);
	
	KSLabelTTF* n_stop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 22);
	n_stop_label->setColor(ccc3(50, 30, 5));
	n_stop_label->setPosition(ccp(n_stop->getContentSize().width/2.f, n_stop->getContentSize().height/2.f-1));
	n_stop->addChild(n_stop_label);
	
	CCSprite* s_stop = CCSprite::create("endless_ready.png");
	s_stop->setColor(ccGRAY);
	
	s_stop_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 22);
	s_stop_label2->setColor(ccWHITE);
	s_stop_label2->setOpacity(100);
	s_stop_label2->setPosition(ccp(s_stop->getContentSize().width/2.f, s_stop->getContentSize().height/2.f-2));
	s_stop->addChild(s_stop_label2);
	
	KSLabelTTF* s_stop_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 22);
	s_stop_label->setColor(ccc3(50, 30, 5));
	s_stop_label->setPosition(ccp(s_stop->getContentSize().width/2.f, s_stop->getContentSize().height/2.f-1));
	s_stop->addChild(s_stop_label);
	
	CCMenuItemLambda* stop_item = CCMenuItemSpriteLambda::create(n_stop, s_stop, [=](CCObject* sender)
																  {
																	  if(!is_menu_enable)
																		  return;
																	  
																	  is_menu_enable = false;
																	  
																	  if(mySGD->getScore() > mySGD->endless_score.getV())
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
																			
																			KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 15);
																			title_label->setColor(ccc3(255, 170, 20));
																			title_label->setAnchorPoint(ccp(0,0.5f));
																			title_label->setPosition(ccp(-back_case->getContentSize().width/2.f + 17,back_case->getContentSize().height/2.f-25));
																			t_container->addChild(title_label);
																			
																			KSLabelTTF* sub_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_endlessCheckGiveup), mySGD->getFont().c_str(), 12);
																			sub_label->setHorizontalAlignment(kCCTextAlignmentLeft);
																			sub_label->setAnchorPoint(ccp(0,0.5f));
																			sub_label->setPosition(ccp(-back_case->getContentSize().width/2.f + 17,5));
																			t_container->addChild(sub_label);
																			
																			CCSprite* t_gray = t_popup->getDimmedSprite();
																			
																			CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
																			close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25,back_case->getContentSize().height/2.f-25));
																			close_button->setFunction([=](CCObject* sender)
																									  {
																										  if(!t_popup->is_menu_enable)
																											  return;
																										  
																										  t_popup->is_menu_enable = false;
																										  
																										  t_popup->addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
																											  t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.f);}));}));
																										  
																										  t_popup->addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
																																						{
																																							t_gray->setOpacity(t);
																																							KS::setOpacity(t_container, t);
																																						}, [=](int t)
																																						{
																																							t_gray->setOpacity(0);
																																							KS::setOpacity(t_container, 0);
																																							is_menu_enable = true;
																																							t_popup->removeFromParent();
																																						}));
																									  });
																			t_container->addChild(close_button);
																			
																			t_popup->button_func_list.clear();
																			
																			t_popup->button_func_list.push_back([=](){
																				if(!t_popup->is_menu_enable)
																					return;
																				
																				t_popup->is_menu_enable = false;
																				
																				t_popup->addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
																					t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.f);}));}));
																				
																				t_popup->addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
																															  {
																																  t_gray->setOpacity(t);
																																  KS::setOpacity(t_container, t);
																															  }, [=](int t)
																															  {
																																  t_gray->setOpacity(0);
																																  KS::setOpacity(t_container, 0);
																																  
																																  
																																  ready_loading = LoadingLayer::create(-999);
																																  addChild(ready_loading, 999);
																																  
																																  vector<CommandParam> command_list;
																																  command_list.clear();
																																  
																																  Json::Value param;
																																  param.clear();
																																  param["memberID"] = myHSP->getMemberID();
																																  param["score"] = mySGD->endless_my_total_score.getV();
																																  param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
																																  param["level"] = mySGD->endless_my_level.getV();
																																  param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
																																  param["victory"] = mySGD->endless_my_victory.getV();
																																  
																																  command_list.push_back(CommandParam("setendlessrank", param, [=](Json::Value result_data)
																																									  {
																																										  if(result_data["result"]["code"].asInt() != GDSUCCESS)
																																										  {
																																											  addChild(KSTimer::create(0.1f, [=](){reSetEndlessRank();}));
																																										  }
																																										  else
																																										  {
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
																																											  
																																											  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
																																												  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
																																											  
																																											  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
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
																																										  }
																																									  }));
																																  
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
																																  
																																  command_list.push_back(CommandParam("saveendlessplaydata", param2, nullptr));
																																  
																																  myHSP->command(command_list);
																																  
																																  
																																  
																																  t_popup->removeFromParent();
																															  }));
																				
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
																			
																			
																			t_container->setScaleY(0.f);
																			
																			t_popup->addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.2f);
																				t_popup->addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(0.8f);
																					t_popup->addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){t_container->setScaleY(t);}, [=](float t){t_container->setScaleY(1.f);}));}));}));
																			
																			t_popup->addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
																														  {
																															  t_gray->setOpacity(t);
																															  KS::setOpacity(t_container, t);
																														  }, [=](int t)
																														  {
																															  t_gray->setOpacity(255);
																															  KS::setOpacity(t_container, 255);
																															  t_popup->is_menu_enable = true;
																														  }));
																		}
																	  else
																		{
																			ready_loading = LoadingLayer::create(-999);
																			addChild(ready_loading, 999);
																			
																			vector<CommandParam> command_list;
																			command_list.clear();
																			
																			Json::Value param;
																			param.clear();
																			param["memberID"] = myHSP->getMemberID();
																			param["score"] = mySGD->endless_my_total_score.getV();
																			param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
																			param["level"] = mySGD->endless_my_level.getV();
																			param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
																			param["victory"] = mySGD->endless_my_victory.getV();
																			
																			command_list.push_back(CommandParam("setendlessrank", param, [=](Json::Value result_data)
																												{
																													if(result_data["result"]["code"].asInt() != GDSUCCESS)
																													{
																														addChild(KSTimer::create(0.1f, [=](){reSetEndlessRank();}));
																													}
																													else
																													{
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
																														
																														addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
																															addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
																														
																														addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
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
																													}
																												}));
																			
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
																			
																			command_list.push_back(CommandParam("saveendlessplaydata", param2, nullptr));
																			
																			myHSP->command(command_list);
																		}
																  });
	stop_item->setPosition(ccp(-main_case->getContentSize().width/2.f+left_back->getPositionX(), 0));
	
	
	CCScale9Sprite* right_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
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
	right_flag->setPosition(ccp(25,26));
	right_title->addChild(right_flag);
	
	KSLabelTTF* right_nick = KSLabelTTF::create(mySGD->temp_endless_nick.getV().c_str(), mySGD->getFont().c_str(), 13);
	right_nick->enableOuterStroke(ccBLACK, 1.f);
	right_nick->setPosition(ccp(97, 24));
	right_title->addChild(right_nick);
	
	right_top_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_top_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20, 35));
	right_top_back->setPosition(ccp(right_back->getContentSize().width/2.f, 111.f));
	right_back->addChild(right_top_back);
	
	right_top_title = KSLabelTTF::create(start_top_title.c_str(), mySGD->getFont().c_str(), 13);
	right_top_title->setAnchorPoint(ccp(0,0.5f));
	right_top_title->setPosition(ccp(10, right_top_back->getContentSize().height/2.f));
	right_top_back->addChild(right_top_title);
	
//	CCSprite* right_score_img = CCSprite::create("ending_mark_score.png");
//	right_score_img->setPosition(ccp(10 + right_score_title->getContentSize().width + 15,right_score_back->getContentSize().height/2.f));
//	right_score_back->addChild(right_score_img);
	
	right_top_content = KSLabelTTF::create(start_top_right_content.c_str(), mySGD->getFont().c_str(), 13);
	right_top_content->setAnchorPoint(ccp(1,0.5f));
	right_top_content->setPosition(ccp(right_top_back->getContentSize().width-10, right_top_back->getContentSize().height/2.f));
	right_top_back->addChild(right_top_content);
	
	
	right_center_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_center_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20, 35));
	right_center_back->setPosition(ccp(right_back->getContentSize().width/2.f, 81.f));
	right_back->addChild(right_center_back);
	
	right_center_title = KSLabelTTF::create(start_center_title.c_str(), mySGD->getFont().c_str(), 13);
	right_center_title->setAnchorPoint(ccp(0,0.5f));
	right_center_title->setPosition(ccp(10, right_center_back->getContentSize().height/2.f));
	right_center_back->addChild(right_center_title);
	
//	CCSprite* right_time_img = CCSprite::create("ending_mark_time.png");
//	right_time_img->setPosition(ccp(10 + right_time_title->getContentSize().width + 15,right_time_back->getContentSize().height/2.f));
//	right_time_back->addChild(right_time_img);
	
	right_center_content = KSLabelTTF::create(start_center_right_content.c_str(), mySGD->getFont().c_str(), 13);
	right_center_content->setAnchorPoint(ccp(1,0.5f));
	right_center_content->setPosition(ccp(right_center_back->getContentSize().width-10, right_center_back->getContentSize().height/2.f));
	right_center_back->addChild(right_center_content);
	
	
	right_bottom_back = CCScale9Sprite::create("mainpopup_pupple3.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_bottom_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20, 35));
	right_bottom_back->setPosition(ccp(right_back->getContentSize().width/2.f, 51.f));
	right_back->addChild(right_bottom_back);
	
	right_bottom_title = KSLabelTTF::create(start_bottom_title.c_str(), mySGD->getFont().c_str(), 13);
	right_bottom_title->setAnchorPoint(ccp(0,0.5f));
	right_bottom_title->setPosition(ccp(10, right_bottom_back->getContentSize().height/2.f));
	right_bottom_back->addChild(right_bottom_title);
	
//	CCSprite* right_gold_img = CCSprite::create("ending_mark_gold.png");
//	right_gold_img->setPosition(ccp(10 + right_gold_title->getContentSize().width + 15,right_gold_back->getContentSize().height/2.f));
//	right_gold_back->addChild(right_gold_img);
	
	right_bottom_content = KSLabelTTF::create(start_bottom_right_content.c_str(), mySGD->getFont().c_str(), 13);
	right_bottom_content->setAnchorPoint(ccp(1,0.5f));
	right_bottom_content->setPosition(ccp(right_bottom_back->getContentSize().width-10, right_bottom_back->getContentSize().height/2.f));
	right_bottom_back->addChild(right_bottom_content);
	
	
	CCScale9Sprite* right_total_back = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	right_total_back->setContentSize(CCSizeMake(right_back->getContentSize().width-20, 35));
	right_total_back->setPosition(ccp(right_back->getContentSize().width/2.f, 21));
	right_back->addChild(right_total_back);
	
	KSLabelTTF* right_total_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_totalScore), mySGD->getFont().c_str(), 15);
	right_total_title->setColor(ccc3(255, 170, 20));
	right_total_title->setAnchorPoint(ccp(0,0.5f));
	right_total_title->setPosition(ccp(10, right_total_back->getContentSize().height/2.f));
	right_total_back->addChild(right_total_title);
	
	
	right_total_content = KSLabelTTF::create(start_total_right_content.c_str(), mySGD->getFont().c_str(), 15);
	right_total_content->setColor(ccc3(255, 170, 20));
	right_total_content->setAnchorPoint(ccp(1,0.5f));
	right_total_content->setPosition(ccp(right_total_back->getContentSize().width-10, right_total_back->getContentSize().height/2.f));
	right_total_back->addChild(right_total_content);
	
	
	
	
	
	
	
	CCSprite* n_next = CCSprite::create("endless_ready.png");
	
	n_next_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_nextStage), mySGD->getFont().c_str(), 22);
	n_next_label2->setColor(ccWHITE);
	n_next_label2->setOpacity(100);
	n_next_label2->setPosition(ccp(n_next->getContentSize().width/2.f, n_next->getContentSize().height/2.f-2));
	n_next->addChild(n_next_label2);
	
	KSLabelTTF* n_next_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_nextStage), mySGD->getFont().c_str(), 22);
	n_next_label->setColor(ccc3(50, 30, 5));
	n_next_label->setPosition(ccp(n_next->getContentSize().width/2.f, n_next->getContentSize().height/2.f-1));
	n_next->addChild(n_next_label);
	
	CCSprite* s_next = CCSprite::create("endless_ready.png");
	s_next->setColor(ccGRAY);
	
	s_next_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_nextStage), mySGD->getFont().c_str(), 22);
	s_next_label2->setColor(ccWHITE);
	s_next_label2->setOpacity(100);
	s_next_label2->setPosition(ccp(s_next->getContentSize().width/2.f, s_next->getContentSize().height/2.f-2));
	s_next->addChild(s_next_label2);
	
	KSLabelTTF* s_next_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_nextStage), mySGD->getFont().c_str(), 22);
	s_next_label->setColor(ccc3(50, 30, 5));
	s_next_label->setPosition(ccp(s_next->getContentSize().width/2.f, s_next->getContentSize().height/2.f-1));
	s_next->addChild(s_next_label);
	
	CCMenuItemLambda* next_item = CCMenuItemSpriteLambda::create(n_next, s_next, [=](CCObject* sender)
																 {
																	 if(!is_menu_enable)
																		 return;
																	 
																	 is_menu_enable = false;
																	 
																	 ready_loading = LoadingLayer::create(-999);
																	 addChild(ready_loading, 999);

																	 Json::Value param;
																	 param["memberID"] = myHSP->getMemberID();
																	 myHSP->command("getendlessplaydata", param, json_selector(this, EndlessModeResult::resultGetEndlessPlayData));
																 });
	next_item->setPosition(ccp(-main_case->getContentSize().width/2.f+right_back->getPositionX(), 0));
	
	bottom_menu = CCMenuLambda::create();
	bottom_menu->setPosition(ccp(main_case->getContentSize().width/2.f,35));
	bottom_menu->setTouchPriority(touch_priority);
	main_case->addChild(bottom_menu);
	
	bottom_menu->addChild(stop_item);
	bottom_menu->addChild(next_item);
	
	if(mySGD->getScore() <= mySGD->endless_score.getV())
	{
		next_item->setVisible(false);
	}
	
	if(is_calc)
	{
		bottom_menu->setVisible(false);
	}
	else
	{
		if(left_total_score > right_total_score)
		{
			// win
			
			CCSprite* win_case = CCSprite::create("endless_winner.png");
			win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
			main_case->addChild(win_case);
			
			CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
			win_label->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f+10));
			win_case->addChild(win_label);
			
			CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
			win_ment->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f-25));
			win_case->addChild(win_ment);
			
			win_case->setRotation(-15);
			
			
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
			particle2->setPosition(win_case->getPosition());
			main_case->addChild(particle2);
		}
		else
		{
			// lose
			
			CCSprite* win_case = CCSprite::create("endless_loser.png");
			win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
			main_case->addChild(win_case);
		}
	}
	
	CCSprite* vs_img = CCSprite::create("endless_vs.png");
	vs_img->setPosition(ccp(main_case->getContentSize().width/2.f, 210));
	main_case->addChild(vs_img);
	
	main_case->setScaleY(0.f);
	
	
	CCNode* curtain_node = LoadingTipScene::getOpenCurtainNode();
	curtain_node->setPosition(ccp(240,160));
	addChild(curtain_node, 999);
	
	addChild(KSTimer::create(0.6f, [=]()
	{
		addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
			addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.8f);
				addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.f);}));}));}));
		
		addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
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
												 KS::setOpacity(main_case, 255);
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
													 
													 addChild(KSTimer::create(1.5f, [=](){startCalcAnimation();}));
												 }
												 else
												 {
													 is_menu_enable = true;
												 }
											 }));
	}));
}

void EndlessModeResult::startCalcAnimation()
{
	is_left_calc_end = is_right_calc_end = false;
	
	AudioEngine::sharedInstance()->playEffect("sound_calc.mp3", true);
	
	float play_limit_time = mySGD->temp_endless_play_limit_time.getV();
	
	int left_life_base_score = mySGD->area_score.getV() + mySGD->damage_score.getV() + mySGD->combo_score.getV();
	float left_life_decrease_score = left_life_base_score*(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_lifeBonusCnt)].asInt()*0.1f);
	float left_time_base_score = left_life_base_score + left_life_decrease_score;
	float left_time_decrease_score = left_time_base_score*(play_limit_time-mySGD->getGameTime())/play_limit_time;
	float left_grade_base_score = left_time_base_score + left_time_decrease_score;
	float left_grade_decrease_score = left_time_base_score*mySGD->getStageGrade()*0.5f;
	
	int left_total_score = left_grade_base_score + left_grade_decrease_score;
	
	
	int right_area_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_areaScore), Json::Value()).asInt();
	int right_damage_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_damageScore), Json::Value()).asInt();
	int right_combo_score = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_comboScore), Json::Value()).asInt();
	
	int right_life_cnt = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_lifeBonusCnt), Json::Value()).asInt();
	int right_game_time = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_gameTime), Json::Value()).asInt();
	int right_clear_grade = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_clearGrade), Json::Value()).asInt();
	
	int right_take_gold = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_takeGold), Json::Value()).asInt();
	float right_take_area = mySGD->temp_replay_data.get(mySGD->getReplayKey(kReplayKey_takeArea), Json::Value()).asFloat();
	
	int right_life_base_score = right_area_score + right_damage_score + right_combo_score;
	float right_life_decrease_score = right_life_base_score*(right_life_cnt*0.1f);
	float right_time_base_score = right_life_base_score + right_life_decrease_score;
	float right_time_decrease_score = right_time_base_score*(play_limit_time-right_game_time)/play_limit_time;
	float right_grade_base_score = right_time_base_score + right_time_decrease_score;
	float right_grade_decrease_score = right_time_base_score*right_clear_grade*0.5f;
	
	int right_total_score = right_grade_base_score + right_grade_decrease_score;
	
	
//	startLeftCalcAnimation(mySGD->area_score.getV(), 0, 1.f, left_top_content, [=]()
//	{
//		startLeftCalcAnimation(mySGD->damage_score.getV(), mySGD->area_score.getV(), 1.f, left_center_content, [=]()
//		{
//			startLeftCalcAnimation(mySGD->combo_score.getV(), mySGD->area_score.getV()+mySGD->damage_score.getV(), 1.f, left_bottom_content, [=]()
//			{
//				flipLeft(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleLifeScore), left_life_decrease_score,
//						 myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTimeScore), left_time_decrease_score,
//						 myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleGradeScore), left_grade_decrease_score, [=]()
//				{
//					startLeftCalcAnimation(left_life_decrease_score, left_life_base_score, 1.f, left_top_content, [=]()
//					{
//						startLeftCalcAnimation(left_time_decrease_score, left_time_base_score, 1.f, left_center_content, [=]()
//						{
//							startLeftCalcAnimation(left_grade_decrease_score, left_grade_base_score, 1.f, left_bottom_content, [=]()
//							{
//								flipLeft(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeGold), mySGD->getStageGold(),
//										 myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeArea), mySGD->getPercentage()*100.f,
//										 "", 0, [=]()
//								{
//									is_left_calc_end = true;
//									if(is_left_calc_end && is_right_calc_end)
//									{
//										AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
//										
//										if(left_total_score > right_total_score)
//										{
//											// win
//											
//											CCSprite* win_case = CCSprite::create("endless_winner.png");
//											win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
//											main_case->addChild(win_case);
//											
//											CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
//											win_label->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f+10));
//											win_case->addChild(win_label);
//											
//											CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
//											win_ment->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f-25));
//											win_case->addChild(win_ment);
//											
//											win_case->setRotation(-15);
//											
//											
//											KS::setOpacity(win_case, 0);
//											win_case->setScale(2.5f);
//											
//											CCParticleSystemQuad* particle1 = CCParticleSystemQuad::createWithTotalParticles(100);
//											particle1->setPositionType(kCCPositionTypeRelative);
//											particle1->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//											particle1->setEmissionRate(300);
//											particle1->setAngle(180.0);
//											particle1->setAngleVar(180.0);
//											ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
//											particle1->setBlendFunc(blendFunc);
//											particle1->setDuration(0.3f);
//											particle1->setEmitterMode(kCCParticleModeGravity);
//											particle1->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
//											particle1->setStartColorVar(ccc4f(0,0,0,0.f));
//											particle1->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
//											particle1->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//											particle1->setStartSize(40.0);
//											particle1->setStartSizeVar(10.0);
//											particle1->setEndSize(0.0);
//											particle1->setEndSizeVar(0.0);
//											particle1->setGravity(ccp(0,-100));
//											particle1->setRadialAccel(50.0);
//											particle1->setRadialAccelVar(20.0);
//											particle1->setSpeed(50);
//											particle1->setSpeedVar(30.0);
//											particle1->setTangentialAccel(0);
//											particle1->setTangentialAccelVar(0);
//											particle1->setTotalParticles(100);
//											particle1->setLife(2.0);
//											particle1->setLifeVar(0.5);
//											particle1->setStartSpin(0.0);
//											particle1->setStartSpinVar(0.f);
//											particle1->setEndSpin(0.0);
//											particle1->setEndSpinVar(0.f);
//											particle1->setPosVar(ccp(90,90));
//											particle1->setPosition(win_case->getPosition());
//											particle1->setAutoRemoveOnFinish(true);
//											main_case->addChild(particle1);
//											
//											addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
//																				   {
//																					   KS::setOpacity(win_case, t*255);
//																					   win_case->setScale(2.5f-t*1.5f);
//																				   }, [=](float t)
//																				   {
//																					   KS::setOpacity(win_case, 255);
//																					   win_case->setScale(1.f);
//																					   
//																					   CCParticleSystemQuad* particle2 = CCParticleSystemQuad::createWithTotalParticles(10);
//																					   particle2->setPositionType(kCCPositionTypeRelative);
//																					   particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//																					   particle2->setEmissionRate(80);
//																					   particle2->setAngle(360.0);
//																					   particle2->setAngleVar(0.0);
//																					   particle2->setBlendFunc(blendFunc);
//																					   particle2->setDuration(-1.0);
//																					   particle2->setEmitterMode(kCCParticleModeGravity);
//																					   particle2->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
//																					   particle2->setStartColorVar(ccc4f(0,0,0,0.f));
//																					   particle2->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
//																					   particle2->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//																					   particle2->setStartSize(25.0);
//																					   particle2->setStartSizeVar(10.0);
//																					   particle2->setEndSize(0.0);
//																					   particle2->setEndSizeVar(0.0);
//																					   particle2->setGravity(ccp(0,0));
//																					   particle2->setRadialAccel(3.0);
//																					   particle2->setRadialAccelVar(0.0);
//																					   particle2->setSpeed(0);
//																					   particle2->setSpeedVar(0.0);
//																					   particle2->setTangentialAccel(0);
//																					   particle2->setTangentialAccelVar(0);
//																					   particle2->setTotalParticles(10);
//																					   particle2->setLife(0.8);
//																					   particle2->setLifeVar(0.25);
//																					   particle2->setStartSpin(0.0);
//																					   particle2->setStartSpinVar(50.f);
//																					   particle2->setEndSpin(0.0);
//																					   particle2->setEndSpinVar(60.f);
//																					   particle2->setPosVar(ccp(80,80));
//																					   particle2->setPosition(win_case->getPosition());
//																					   main_case->addChild(particle2);
//																					   
//																					   
//																					   is_menu_enable = true;
//																					   bottom_menu->setVisible(true);
//																				   }));
//										}
//										else
//										{
//											// lose
//											
//											CCSprite* win_case = CCSprite::create("endless_loser.png");
//											win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
//											main_case->addChild(win_case);
//											
//											KS::setOpacity(win_case, 0);
//											win_case->setScale(2.5f);
//											
//											addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
//																				   {
//																					   KS::setOpacity(win_case, t*255);
//																					   win_case->setScale(2.5f-t*1.5f);
//																				   }, [=](float t)
//																				   {
//																					   KS::setOpacity(win_case, 255);
//																					   win_case->setScale(1.f);
//																					   
//																					   is_menu_enable = true;
//																					   bottom_menu->setVisible(true);
//																				   }));
//										}
//									}
//								});
//							});
//						});
//					});
//				});
//			});
//		});
//	});
	
	startLeftCalcAnimation(mySGD->area_score.getV(), 0, 0.5f, left_top_content, [=]()
   {
	   flipTarget(left_top_back, left_top_title, left_top_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleLifeScore), left_life_decrease_score);
	   startLeftCalcAnimation(mySGD->damage_score.getV(), mySGD->area_score.getV(), 0.5f, left_center_content, [=]()
	  {
		  flipTarget(left_center_back, left_center_title, left_center_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTimeScore), left_time_decrease_score);
		  startLeftCalcAnimation(mySGD->combo_score.getV(), mySGD->area_score.getV()+mySGD->damage_score.getV(), 0.5f, left_bottom_content, [=]()
		 {
			 flipTarget(left_bottom_back, left_bottom_title, left_bottom_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleGradeScore), left_grade_decrease_score);
			 startLeftCalcAnimation(left_life_decrease_score, left_life_base_score, 0.5f, left_top_content, [=]()
			{
				flipTarget(left_top_back, left_top_title, left_top_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeGold), mySGD->getStageGold());
				startLeftCalcAnimation(left_time_decrease_score, left_time_base_score, 0.5f, left_center_content, [=]()
			   {
				   flipTarget(left_center_back, left_center_title, left_center_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeArea), mySGD->getPercentage()*100.f);
				   startLeftCalcAnimation(left_grade_decrease_score, left_grade_base_score, 0.5f, left_bottom_content, [=]()
				  {
					  flipTarget(left_bottom_back, left_bottom_title, left_bottom_content, "", 0);
					  
					  is_left_calc_end = true;
					  if(is_left_calc_end && is_right_calc_end)
					  {
						  AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
						  
						  if(left_total_score > right_total_score)
						  {
							  // win
							  
							  CCSprite* win_case = CCSprite::create("endless_winner.png");
							  win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
							  main_case->addChild(win_case);
							  
							  CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
							  win_label->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f+10));
							  win_case->addChild(win_label);
							  
							  CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
							  win_ment->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f-25));
							  win_case->addChild(win_ment);
							  
							  win_case->setRotation(-15);
							  
							  
							  KS::setOpacity(win_case, 0);
							  win_case->setScale(2.5f);
							  
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
							  particle1->setPosition(win_case->getPosition());
							  particle1->setAutoRemoveOnFinish(true);
							  main_case->addChild(particle1);
							  
							  addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																	 {
																		 KS::setOpacity(win_case, t*255);
																		 win_case->setScale(2.5f-t*1.5f);
																	 }, [=](float t)
																	 {
																		 KS::setOpacity(win_case, 255);
																		 win_case->setScale(1.f);
																		 
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
																		 particle2->setPosition(win_case->getPosition());
																		 main_case->addChild(particle2);
																		 
																		 
																		 is_menu_enable = true;
																		 bottom_menu->setVisible(true);
																	 }));
						  }
						  else
						  {
							  // lose
							  
							  CCSprite* win_case = CCSprite::create("endless_loser.png");
							  win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
							  main_case->addChild(win_case);
							  
							  KS::setOpacity(win_case, 0);
							  win_case->setScale(2.5f);
							  
							  addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																	 {
																		 KS::setOpacity(win_case, t*255);
																		 win_case->setScale(2.5f-t*1.5f);
																	 }, [=](float t)
																	 {
																		 KS::setOpacity(win_case, 255);
																		 win_case->setScale(1.f);
																		 
																		 is_menu_enable = true;
																		 bottom_menu->setVisible(true);
																	 }));
						  }
					  }
				  });
			   });
			});
		 });
	  });
   });
	
	
//	startRightCalcAnimation(right_area_score, 0, 1.f, right_top_content, [=]()
//	{
//	   startRightCalcAnimation(right_damage_score, right_area_score, 1.f, right_center_content, [=]()
//	  {
//		  startRightCalcAnimation(right_combo_score, right_area_score+right_damage_score, 1.f, right_bottom_content, [=]()
//		 {
//			 flipRight(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleLifeScore), right_life_decrease_score,
//					  myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTimeScore), right_time_decrease_score,
//					  myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleGradeScore), right_grade_decrease_score, [=]()
//			  {
//				  startRightCalcAnimation(right_life_decrease_score, right_life_base_score, 1.f, right_top_content, [=]()
//				 {
//					 startRightCalcAnimation(right_time_decrease_score, right_time_base_score, 1.f, right_center_content, [=]()
//					{
//						startRightCalcAnimation(right_grade_decrease_score, right_grade_base_score, 1.f, right_bottom_content, [=]()
//					   {
//						   flipRight(myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeGold), right_take_gold,
//									myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeArea), right_take_area*100.f,
//									"", 0, [=]()
//									{
//										is_right_calc_end = true;
//										if(is_left_calc_end && is_right_calc_end)
//										{
//											AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
//											
//											if(left_total_score > right_total_score)
//											{
//												// win
//												
//												CCSprite* win_case = CCSprite::create("endless_winner.png");
//												win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
//												main_case->addChild(win_case);
//												
//												CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
//												win_label->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f+10));
//												win_case->addChild(win_label);
//												
//												CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
//												win_ment->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f-25));
//												win_case->addChild(win_ment);
//												
//												win_case->setRotation(-15);
//												
//												
//												KS::setOpacity(win_case, 0);
//												win_case->setScale(2.5f);
//												
//												CCParticleSystemQuad* particle1 = CCParticleSystemQuad::createWithTotalParticles(100);
//												particle1->setPositionType(kCCPositionTypeRelative);
//												particle1->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//												particle1->setEmissionRate(300);
//												particle1->setAngle(180.0);
//												particle1->setAngleVar(180.0);
//												ccBlendFunc blendFunc = {GL_ONE, GL_ONE};
//												particle1->setBlendFunc(blendFunc);
//												particle1->setDuration(0.3f);
//												particle1->setEmitterMode(kCCParticleModeGravity);
//												particle1->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
//												particle1->setStartColorVar(ccc4f(0,0,0,0.f));
//												particle1->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
//												particle1->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//												particle1->setStartSize(40.0);
//												particle1->setStartSizeVar(10.0);
//												particle1->setEndSize(0.0);
//												particle1->setEndSizeVar(0.0);
//												particle1->setGravity(ccp(0,-100));
//												particle1->setRadialAccel(50.0);
//												particle1->setRadialAccelVar(20.0);
//												particle1->setSpeed(50);
//												particle1->setSpeedVar(30.0);
//												particle1->setTangentialAccel(0);
//												particle1->setTangentialAccelVar(0);
//												particle1->setTotalParticles(100);
//												particle1->setLife(2.0);
//												particle1->setLifeVar(0.5);
//												particle1->setStartSpin(0.0);
//												particle1->setStartSpinVar(0.f);
//												particle1->setEndSpin(0.0);
//												particle1->setEndSpinVar(0.f);
//												particle1->setPosVar(ccp(90,90));
//												particle1->setPosition(win_case->getPosition());
//												particle1->setAutoRemoveOnFinish(true);
//												main_case->addChild(particle1);
//												
//												addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
//																					   {
//																						   KS::setOpacity(win_case, t*255);
//																						   win_case->setScale(2.5f-t*1.5f);
//																					   }, [=](float t)
//																					   {
//																						   KS::setOpacity(win_case, 255);
//																						   win_case->setScale(1.f);
//																						   
//																						   CCParticleSystemQuad* particle2 = CCParticleSystemQuad::createWithTotalParticles(10);
//																						   particle2->setPositionType(kCCPositionTypeRelative);
//																						   particle2->setTexture(CCTextureCache::sharedTextureCache()->addImage("particle6.png"));
//																						   particle2->setEmissionRate(80);
//																						   particle2->setAngle(360.0);
//																						   particle2->setAngleVar(0.0);
//																						   particle2->setBlendFunc(blendFunc);
//																						   particle2->setDuration(-1.0);
//																						   particle2->setEmitterMode(kCCParticleModeGravity);
//																						   particle2->setStartColor(ccc4f(1.f, 0.992f, 0.784f, 1.f));
//																						   particle2->setStartColorVar(ccc4f(0,0,0,0.f));
//																						   particle2->setEndColor(ccc4f(0.f,0.f,0.f,1.f));
//																						   particle2->setEndColorVar(ccc4f(0, 0, 0, 0.f));
//																						   particle2->setStartSize(25.0);
//																						   particle2->setStartSizeVar(10.0);
//																						   particle2->setEndSize(0.0);
//																						   particle2->setEndSizeVar(0.0);
//																						   particle2->setGravity(ccp(0,0));
//																						   particle2->setRadialAccel(3.0);
//																						   particle2->setRadialAccelVar(0.0);
//																						   particle2->setSpeed(0);
//																						   particle2->setSpeedVar(0.0);
//																						   particle2->setTangentialAccel(0);
//																						   particle2->setTangentialAccelVar(0);
//																						   particle2->setTotalParticles(10);
//																						   particle2->setLife(0.8);
//																						   particle2->setLifeVar(0.25);
//																						   particle2->setStartSpin(0.0);
//																						   particle2->setStartSpinVar(50.f);
//																						   particle2->setEndSpin(0.0);
//																						   particle2->setEndSpinVar(60.f);
//																						   particle2->setPosVar(ccp(80,80));
//																						   particle2->setPosition(win_case->getPosition());
//																						   main_case->addChild(particle2);
//																						   
//																						   
//																						   is_menu_enable = true;
//																						   bottom_menu->setVisible(true);
//																					   }));
//											}
//											else
//											{
//												// lose
//												
//												CCSprite* win_case = CCSprite::create("endless_loser.png");
//												win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
//												main_case->addChild(win_case);
//												
//												KS::setOpacity(win_case, 0);
//												win_case->setScale(2.5f);
//												
//												addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
//																					   {
//																						   KS::setOpacity(win_case, t*255);
//																						   win_case->setScale(2.5f-t*1.5f);
//																					   }, [=](float t)
//																					   {
//																						   KS::setOpacity(win_case, 255);
//																						   win_case->setScale(1.f);
//																						   
//																						   is_menu_enable = true;
//																						   bottom_menu->setVisible(true);
//																					   }));
//											}
//										}
//									});
//					   });
//					});
//				 });
//			  });
//		 });
//	  });
//	});
	
	startRightCalcAnimation(right_area_score, 0, 0.5f, right_top_content, [=]()
	{
		flipTarget(right_top_back, right_top_title, right_top_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleLifeScore), right_life_decrease_score);
		startRightCalcAnimation(right_damage_score, right_area_score, 0.5f, right_center_content, [=]()
		{
			flipTarget(right_center_back, right_center_title, right_center_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTimeScore), right_time_decrease_score);
			startRightCalcAnimation(right_combo_score, right_area_score+right_damage_score, 0.5f, right_bottom_content, [=]()
			{
				flipTarget(right_bottom_back, right_bottom_title, right_bottom_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleGradeScore), right_grade_decrease_score);
				startRightCalcAnimation(right_life_decrease_score, right_life_base_score, 0.5f, right_top_content, [=]()
				{
					flipTarget(right_top_back, right_top_title, right_top_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeGold), right_take_gold);
					startRightCalcAnimation(right_time_decrease_score, right_time_base_score, 0.5f, right_center_content, [=]()
					{
						flipTarget(right_center_back, right_center_title, right_center_content, myLoc->getLocalForKey(kMyLocalKey_endlessCalcTitleTakeArea), right_take_area*100.f);
						startRightCalcAnimation(right_grade_decrease_score, right_grade_base_score, 0.5f, right_bottom_content, [=]()
						{
							flipTarget(right_bottom_back, right_bottom_title, right_bottom_content, "", 0);
							
							is_right_calc_end = true;
							if(is_left_calc_end && is_right_calc_end)
							{
								AudioEngine::sharedInstance()->stopEffect("sound_calc.mp3");
								
								if(left_total_score > right_total_score)
								{
									// win
									
									CCSprite* win_case = CCSprite::create("endless_winner.png");
									win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
									main_case->addChild(win_case);
									
									CCLabelBMFont* win_label = CCLabelBMFont::create(CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "winfont.fnt");
									win_label->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f+10));
									win_case->addChild(win_label);
									
									CCSprite* win_ment = CCSprite::create(CCString::createWithFormat("endless_win_%s.png", myLoc->getLocalCode()->getCString())->getCString());
									win_ment->setPosition(ccp(win_case->getContentSize().width/2.f, win_case->getContentSize().height/2.f-25));
									win_case->addChild(win_ment);
									
									win_case->setRotation(-15);
									
									
									KS::setOpacity(win_case, 0);
									win_case->setScale(2.5f);
									
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
									particle1->setPosition(win_case->getPosition());
									particle1->setAutoRemoveOnFinish(true);
									main_case->addChild(particle1);
									
									addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																		   {
																			   KS::setOpacity(win_case, t*255);
																			   win_case->setScale(2.5f-t*1.5f);
																		   }, [=](float t)
																		   {
																			   KS::setOpacity(win_case, 255);
																			   win_case->setScale(1.f);
																			   
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
																			   particle2->setPosition(win_case->getPosition());
																			   main_case->addChild(particle2);
																			   
																			   
																			   is_menu_enable = true;
																			   bottom_menu->setVisible(true);
																		   }));
								}
								else
								{
									// lose
									
									CCSprite* win_case = CCSprite::create("endless_loser.png");
									win_case->setPosition(ccp(main_case->getContentSize().width/2.f,main_case->getContentSize().height*0.6f));
									main_case->addChild(win_case);
									
									KS::setOpacity(win_case, 0);
									win_case->setScale(2.5f);
									
									addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t)
																		   {
																			   KS::setOpacity(win_case, t*255);
																			   win_case->setScale(2.5f-t*1.5f);
																		   }, [=](float t)
																		   {
																			   KS::setOpacity(win_case, 255);
																			   win_case->setScale(1.f);
																			   
																			   is_menu_enable = true;
																			   bottom_menu->setVisible(true);
																		   }));
								}
							}
						});
					});
				});
			});
		});
	});
}

void EndlessModeResult::startLeftCalcAnimation(float t_keep_value, float t_base_value, float t_calc_time, KSLabelTTF* t_decrease_target, function<void()> t_end_func)
{
	keep_left_value = t_keep_value;
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
		left_total_content->setString(CCString::createWithFormat("%.0f",base_left_value + increase_left_value)->getCString());
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
	addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
								  {
									  left_top_back->setScaleY(1.f-t);
								  }, [=](float t)
								  {
									  left_top_back->setScaleY(0.f);
									  left_top_title->setString(t_top_title.c_str());
									  left_top_content->setString(CCString::createWithFormat("%d", t_top_content)->getCString());
									  
									  addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																			 {
																				 left_top_back->setScaleY(t);
																			 }, [=](float t)
																			 {
																				 left_top_back->setScaleY(1.f);
																			 }));
								  }));
	
	addChild(KSTimer::create(6.f/30.f, [=]()
							 {
								 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																		{
																			left_center_back->setScaleY(1.f-t);
																		}, [=](float t)
																		{
																			left_center_back->setScaleY(0.f);
																			left_center_title->setString(t_center_title.c_str());
																			left_center_content->setString(CCString::createWithFormat("%d", t_center_content)->getCString());
																			
																			addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																												   {
																													   left_center_back->setScaleY(t);
																												   }, [=](float t)
																												   {
																													   left_center_back->setScaleY(1.f);
																												   }));
																		}));
							 }));
	
	addChild(KSTimer::create(13.f/30.f, [=]()
							 {
								 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																		{
																			left_bottom_back->setScaleY(1.f-t);
																		}, [=](float t)
																		{
																			left_bottom_back->setScaleY(0.f);
																			left_bottom_title->setString(t_bottom_title.c_str());
																			left_bottom_content->setString(CCString::createWithFormat("%d", t_bottom_content)->getCString());
																			
																			addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																												   {
																													   left_bottom_back->setScaleY(t);
																												   }, [=](float t)
																												   {
																													   left_bottom_back->setScaleY(1.f);
																													   t_end_func();
																												   }));
																		}));
							 }));
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
	keep_right_value = t_keep_value;
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
		right_total_content->setString(CCString::createWithFormat("%.0f",base_right_value + increase_right_value)->getCString());
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
	addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
										   {
											   right_top_back->setScaleY(1.f-t);
										   }, [=](float t)
										   {
											   right_top_back->setScaleY(0.f);
											   right_top_title->setString(t_top_title.c_str());
											   right_top_content->setString(CCString::createWithFormat("%d", t_top_content)->getCString());
											   
											   addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																					  {
																						  right_top_back->setScaleY(t);
																					  }, [=](float t)
																					  {
																						  right_top_back->setScaleY(1.f);
																					  }));
										   }));
	
	addChild(KSTimer::create(6.f/30.f, [=]()
							 {
								 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																		{
																			right_center_back->setScaleY(1.f-t);
																		}, [=](float t)
																		{
																			right_center_back->setScaleY(0.f);
																			right_center_title->setString(t_center_title.c_str());
																			right_center_content->setString(CCString::createWithFormat("%d", t_center_content)->getCString());
																			
																			addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																												   {
																													   right_center_back->setScaleY(t);
																												   }, [=](float t)
																												   {
																													   right_center_back->setScaleY(1.f);
																												   }));
																		}));
							 }));
	
	addChild(KSTimer::create(13.f/30.f, [=]()
							 {
								 addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t)
																		{
																			right_bottom_back->setScaleY(1.f-t);
																		}, [=](float t)
																		{
																			right_bottom_back->setScaleY(0.f);
																			right_bottom_title->setString(t_bottom_title.c_str());
																			right_bottom_content->setString(CCString::createWithFormat("%d", t_bottom_content)->getCString());
																			
																			addChild(KSGradualValue<float>::create(0.f, 1.f, 3.f/30.f, [=](float t)
																												   {
																													   right_bottom_back->setScaleY(t);
																												   }, [=](float t)
																												   {
																													   right_bottom_back->setScaleY(1.f);
																													   t_end_func();
																												   }));
																		}));
							 }));
}

void EndlessModeResult::reSetEndlessRank()
{
	Json::Value param;
	param.clear();
	param["memberID"] = myHSP->getMemberID();
	param["score"] = mySGD->endless_my_total_score.getV();
	param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
	param["level"] = mySGD->endless_my_level.getV();
	param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
	param["victory"] = mySGD->endless_my_victory.getV();
	
	myHSP->command("setendlessrank", param, [=](Json::Value result_data)
										{
											if(result_data["result"]["code"].asInt() != GDSUCCESS)
											{
												addChild(KSTimer::create(0.1f, [=](){reSetEndlessRank();}));
											}
											else
											{
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
												
												addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
													addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
												
												addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(main_case, t);}, [=](int t){KS::setOpacity(main_case, 0);}));
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
		
		mySGD->endless_memberID = result_data["rival"]["memberID"].asInt64();
		mySGD->endless_nick = result_data["rival"]["nick"].asString();
		mySGD->endless_flag = result_data["rival"]["flag"].asString();
		mySGD->endless_victory = result_data["rival"]["victory"].asInt();
		mySGD->endless_autoLevel = result_data["rival"]["autoLevel"].asInt();
		mySGD->endless_level = result_data["rival"]["level"].asInt();
		mySGD->endless_score = result_data["rival"]["score"].asInt();
		mySGD->endless_regDate = result_data["rival"]["regDate"].asInt64();
		
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
		NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
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
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
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
}
