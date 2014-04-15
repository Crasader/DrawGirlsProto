//
//  LoadingTipScene.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 1. 21..
//
//

#include "LoadingTipScene.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "ServerDataSave.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include "MaingameScene.h"
#include "NewMainFlowScene.h"
#include "EnumDefine.h"
#include "KSUtil.h"
#include "CommonButton.h"
#include "CCMenuLambda.h"
#include "PlayTutorial.h"

CCScene* LoadingTipScene::scene()
{
    CCScene *scene = CCScene::create();
    
    LoadingTipScene *layer = LoadingTipScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer);
	
    return scene;
}

LoadingTipScene* LoadingTipScene::getLoadingTipSceneLayer()
{
	LoadingTipScene *layer = LoadingTipScene::create();
//	layer->setAnchorPoint(ccp(0.5,0));
//	layer->setScale(myDSH->screen_convert_rate);
//	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    return layer;
}

enum LoadingTipZorder{
	kLoadingTipZorder_back = 1,
	kLoadingTipZorder_content,
	kLoadingTipZorder_popup
};

bool LoadingTipScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	
	next_scene_name = mySGD->getNextSceneName();
	
	
	if(next_scene_name == "maingame" && NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i) != kCLEAR_default)
		is_mission_tip = true;
	else
		is_mission_tip = false;
	
	if(!is_mission_tip)
	{
		if(next_scene_name == "maingame")
		{
			CCNode* tip_img = getCurtainTipImage();
			tip_img->setPosition(ccp(240,160));
			addChild(tip_img, kLoadingTipZorder_back);
			
			CCDelayTime* t_delay = CCDelayTime::create(0.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(LoadingTipScene::popRootScene));
			CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
			tip_img->runAction(t_seq);
		}
		else
		{
			CCSprite* tip_img = getLoadingTipImage();
			tip_img->setPosition(ccp(240,160));
			addChild(tip_img, kLoadingTipZorder_back);
			
			readyLoading();
		}
	}
	else
	{
		CCNode* tip_img = getMissionTipImage();
		tip_img->setPosition(ccp(240,160));
		addChild(tip_img, kLoadingTipZorder_back);
		
		readyLoading();
	}
		
	return true;
}

void LoadingTipScene::popRootScene()
{
	readyLoading();
}

CCNode* LoadingTipScene::getMissionTipImage()
{
	CCNode* loading_tip_node = CCNode::create();
	
//	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
	
	CCSprite* left_curtain = CCSprite::create("curtain_left.png");
	left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
	left_curtain->setPosition(ccp(-240, 0));
	loading_tip_node->addChild(left_curtain);
	
	CCMoveTo* left_in = CCMoveTo::create(0.5f, ccp(0,0));
	left_curtain->runAction(left_in);
	
	CCSprite* right_curtain = CCSprite::create("curtain_left.png");
	right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	right_curtain->setFlipX(true);
	right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
	right_curtain->setPosition(ccp(240,0));
	loading_tip_node->addChild(right_curtain);
	
	CCMoveTo* right_in = CCMoveTo::create(0.5f, ccp(0,0));
	right_curtain->runAction(right_in);
	
	CCSprite* mission_back = CCSprite::create("mission_back.png");
	mission_back->setPosition(ccp(0, 0));
	loading_tip_node->addChild(mission_back);
	
	CCSprite* title_img;
	
	ok_img = KS::loadCCBI<CCSprite*>(this, "tutorial_nextbutton.ccbi").first;
	ok_img->setPosition(ccp(mission_back->getContentSize().width*0.7f, mission_back->getContentSize().height*0.18f));
	mission_back->addChild(ok_img);
	ok_img->setVisible(false);
	
	CCSprite* n_ok = CCSprite::create("tutorial_next.png");
	n_ok->setOpacity(0);
	CCSprite* s_ok = CCSprite::create("tutorial_next.png");
	s_ok->setOpacity(0);
	
	CCMenuItemLambda* ok_item = CCMenuItemSpriteLambda::create(n_ok, s_ok, [=](CCObject* sender)
															   {
																   onMinimumTime();
															   });
	ok_menu = CCMenuLambda::createWithItem(ok_item);
	ok_menu->setPosition(ccp(mission_back->getContentSize().width*0.7f, mission_back->getContentSize().height*0.18f));
	mission_back->addChild(ok_menu);
	ok_menu->setVisible(false);
	
	
	no_review = CommonButton::create("다시보지않기", 13, CCSizeMake(100, 50), CommonButtonGreen, -200);
	no_review->setPosition(ccp(mission_back->getContentSize().width*0.3f, mission_back->getContentSize().height*0.18f));
	mission_back->addChild(no_review);
	no_review->setVisible(false);
	
	
	int stage_number = mySD->getSilType();
	int mission_type = NSDS_GI(stage_number, kSDS_SI_missionType_i);
	
	
	if(mission_type == kCLEAR_bossLifeZero)
	{
//		NSDS_GI(stage_number, kSDS_SI_missionOptionEnergy_i);
		title_img = CCSprite::create("mission_title_bosslifezero.png");
		
		CCLabelTTF* main1_ment = CCLabelTTF::create("공격으로 보스몹의 에너지를", mySGD->getFont().c_str(), 17);
		main1_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+18));
		mission_back->addChild(main1_ment);
		
		CCLabelTTF* main2_ment = CCLabelTTF::create("모두 소진시키세요.", mySGD->getFont().c_str(), 17);
		main2_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-7));
		mission_back->addChild(main2_ment);
		
		CCLabelTTF* sub_ment = CCLabelTTF::create("보스의 에너지가 다 소진되어도 게임은 계속...", mySGD->getFont().c_str(), 12);
		sub_ment->setColor(ccc3(125, 125, 125));
		sub_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-37));
		mission_back->addChild(sub_ment);
		
		no_review->setFunction([=](CCObject* sender)
							   {
								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_bossLifeZero, true);
								   onMinimumTime();
							   });
	}
	else if(mission_type == kCLEAR_subCumberCatch)
	{
		int catch_count = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		title_img = CCSprite::create("mission_title_subcumbercatch.png");
		
		CCSprite* catch_count_img = CCSprite::create("mission_catch_count.png");
		catch_count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
		mission_back->addChild(catch_count_img);
		
		CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d마리", catch_count)->getCString(), mySGD->getFont().c_str(), 23);
		count_label->setColor(ccc3(255, 240, 0));
		count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+50, mission_back->getContentSize().height/2.f+13));
		mission_back->addChild(count_label);
		
		CCLabelTTF* main_ment = CCLabelTTF::create("부하몹을 가두어 잡으세요.", mySGD->getFont().c_str(), 17);
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
		mission_back->addChild(main_ment);
		
		no_review->setFunction([=](CCObject* sender)
							   {
								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_subCumberCatch, true);
								   onMinimumTime();
							   });
	}
	else if(mission_type == kCLEAR_bigArea)
	{
		int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
		int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		title_img = CCSprite::create("mission_title_bigarea.png");
		
		CCSprite* count_img = CCSprite::create("mission_count.png");
		count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-40, mission_back->getContentSize().height/2.f+13));
		mission_back->addChild(count_img);
		
		CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d%% x %d", percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 23);
		count_label->setColor(ccc3(255, 240, 0));
		count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+60, mission_back->getContentSize().height/2.f+13));
		mission_back->addChild(count_label);
		
		CCLabelTTF* main_ment = CCLabelTTF::create(CCString::createWithFormat("한번에 %d%%이상 영역을 %d번 획득하세요!", percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 17);
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
		mission_back->addChild(main_ment);
		
		no_review->setFunction([=](CCObject* sender)
							   {
								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_bigArea, true);
								   onMinimumTime();
							   });
	}
	else if(mission_type == kCLEAR_itemCollect)
	{
		int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		
		title_img = CCSprite::create("mission_title_itemcollect.png");
		
		CCSprite* count_img = CCSprite::create("mission_count2.png");
		count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
		mission_back->addChild(count_img);
		
		CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d개", count_value)->getCString(), mySGD->getFont().c_str(), 23);
		count_label->setColor(ccc3(255, 240, 0));
		count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+40, mission_back->getContentSize().height/2.f+13));
		mission_back->addChild(count_label);
		
		CCLabelTTF* main_ment = CCLabelTTF::create(CCString::createWithFormat("%d개의 아이템을 획득하세요.", count_value)->getCString(), mySGD->getFont().c_str(), 17);
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
		mission_back->addChild(main_ment);
		
		no_review->setFunction([=](CCObject* sender)
							   {
								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_itemCollect, true);
								   onMinimumTime();
							   });
	}
	else if(mission_type == kCLEAR_perfect)
	{
		int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
		
		title_img = CCSprite::create("mission_title_perfect.png");
		
		CCSprite* count_img = CCSprite::create("mission_area.png");
		count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+20));
		mission_back->addChild(count_img);
		
		CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d%%", percent_value)->getCString(), mySGD->getFont().c_str(), 23);
		count_label->setColor(ccc3(255, 240, 0));
		count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+40, mission_back->getContentSize().height/2.f+20));
		mission_back->addChild(count_label);
		
		CCLabelTTF* main_ment = CCLabelTTF::create(CCString::createWithFormat("정확한 %d%%로 게임을 클리어하세요.", percent_value)->getCString(), mySGD->getFont().c_str(), 17);
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-12));
		mission_back->addChild(main_ment);
		
		CCLabelTTF* sub_ment = CCLabelTTF::create("이 미션에서는 3단계 카드를 얻을 수 없어요!", mySGD->getFont().c_str(), 12);
		sub_ment->setColor(ccc3(125, 125, 125));
		sub_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-33));
		mission_back->addChild(sub_ment);
		
		no_review->setFunction([=](CCObject* sender)
							   {
								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_perfect, true);
								   onMinimumTime();
							   });
	}
	else if(mission_type == kCLEAR_timeLimit)
	{
		int sec_value = NSDS_GI(stage_number, kSDS_SI_missionOptionSec_i);
		
		title_img = CCSprite::create("mission_title_timelimit.png");
		
		CCSprite* count_img = CCSprite::create("mission_time.png");
		count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
		mission_back->addChild(count_img);
		
		CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d초", sec_value)->getCString(), mySGD->getFont().c_str(), 23);
		count_label->setColor(ccc3(255, 240, 0));
		count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+40, mission_back->getContentSize().height/2.f+13));
		mission_back->addChild(count_label);
		
		CCLabelTTF* main_ment = CCLabelTTF::create("정해진 시간 내 클리어하세요.", mySGD->getFont().c_str(), 17);
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
		mission_back->addChild(main_ment);
		
		no_review->setFunction([=](CCObject* sender)
							   {
								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_timeLimit, true);
								   onMinimumTime();
							   });
	}
	else if(mission_type == kCLEAR_sequenceChange)
	{
		title_img = CCSprite::create("mission_title_sequencechange.png");
		
		CCLabelTTF* main1_ment = CCLabelTTF::create("게임 중 나오는 ", mySGD->getFont().c_str(), 17);
		main1_ment->setAnchorPoint(ccp(1,0.5));
		main1_ment->setPosition(ccp(mission_back->getContentSize().width/2.f-13, mission_back->getContentSize().height/2.f+20));
		mission_back->addChild(main1_ment);
		
		for(int i=0;i<6;i++)
		{
			CCSprite* t_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", i+1)->getCString());
			t_img->setPosition(ccp(main1_ment->getPositionX() + 9 + 18*i, mission_back->getContentSize().height/2.f+20));
			mission_back->addChild(t_img);
		}
		
		CCLabelTTF* main2_ment = CCLabelTTF::create("를", mySGD->getFont().c_str(), 17);
		main2_ment->setAnchorPoint(ccp(0,0.5));
		main2_ment->setPosition(ccp(main1_ment->getPositionX() + 18*6, mission_back->getContentSize().height/2.f+20));
		mission_back->addChild(main2_ment);
		
		CCLabelTTF* main3_ment = CCLabelTTF::create("순서대로 획득하세요!", mySGD->getFont().c_str(), 17);
		main3_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-6));
		mission_back->addChild(main3_ment);
		
		CCLabelTTF* sub_ment = CCLabelTTF::create("이 미션에서는 1단계 카드를 얻을 수 없어요!", mySGD->getFont().c_str(), 12);
		sub_ment->setColor(ccc3(125, 125, 125));
		sub_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-33));
		mission_back->addChild(sub_ment);
		
		no_review->setFunction([=](CCObject* sender)
							   {
								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_sequenceChange, true);
								   onMinimumTime();
							   });
	}
	
	title_img->setPosition(ccp(mission_back->getContentSize().width/2.f+20, mission_back->getContentSize().height/2.f+68));
	mission_back->addChild(title_img);
	
	KS::setOpacity(mission_back, 0);
	loading_tip_node->addChild(KSGradualValue<int>::create(-255, 255, 1.f, [=](int t)
								{
									if(t >= 0)
										KS::setOpacity(mission_back, t);
								}, [=](int t)
								{
									KS::setOpacity(mission_back, 255);
								}));
	
	return loading_tip_node;
}

CCNode* LoadingTipScene::getOpenCurtainNode()
{
	CCNode* loading_tip_node = CCNode::create();
	
	if(NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i) != kCLEAR_default)
	{
		CCSprite* left_curtain = CCSprite::create("curtain_left.png");
		left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
		left_curtain->setPosition(ccp(0, 0));
		loading_tip_node->addChild(left_curtain);
		
		CCDelayTime* left_delay = CCDelayTime::create(0.5f);
		CCMoveTo* left_in = CCMoveTo::create(0.5f, ccp(-240,0));
		CCSequence* left_seq = CCSequence::create(left_delay, left_in, NULL);
		left_curtain->runAction(left_seq);
		
		CCSprite* right_curtain = CCSprite::create("curtain_left.png");
		right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		right_curtain->setFlipX(true);
		right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
		right_curtain->setPosition(ccp(0,0));
		loading_tip_node->addChild(right_curtain);
		
		CCDelayTime* right_delay = CCDelayTime::create(0.5f);
		CCMoveTo* right_in = CCMoveTo::create(0.5f, ccp(240,0));
		CCSequence* right_seq = CCSequence::create(right_delay, right_in, NULL);
		right_curtain->runAction(right_seq);
		
		CCSprite* mission_back = CCSprite::create("mission_back.png");
		mission_back->setPosition(ccp(0, 0));
		loading_tip_node->addChild(mission_back);
		
		CCSprite* title_img;
		
		int stage_number = mySD->getSilType();
		int mission_type = NSDS_GI(stage_number, kSDS_SI_missionType_i);
		
		
		if(mission_type == kCLEAR_bossLifeZero)
		{
			//		NSDS_GI(stage_number, kSDS_SI_missionOptionEnergy_i);
			title_img = CCSprite::create("mission_title_bosslifezero.png");
			
			CCLabelTTF* main1_ment = CCLabelTTF::create("공격으로 보스몹의 에너지를", mySGD->getFont().c_str(), 17);
			main1_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+18));
			mission_back->addChild(main1_ment);
			
			CCLabelTTF* main2_ment = CCLabelTTF::create("모두 소진시키세요.", mySGD->getFont().c_str(), 17);
			main2_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-7));
			mission_back->addChild(main2_ment);
			
			CCLabelTTF* sub_ment = CCLabelTTF::create("보스의 에너지가 다 소진되어도 게임은 계속...", mySGD->getFont().c_str(), 12);
			sub_ment->setColor(ccc3(125, 125, 125));
			sub_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-37));
			mission_back->addChild(sub_ment);
		}
		else if(mission_type == kCLEAR_subCumberCatch)
		{
			int catch_count = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			title_img = CCSprite::create("mission_title_subcumbercatch.png");
			
			CCSprite* catch_count_img = CCSprite::create("mission_catch_count.png");
			catch_count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
			mission_back->addChild(catch_count_img);
			
			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d마리", catch_count)->getCString(), mySGD->getFont().c_str(), 23);
			count_label->setColor(ccc3(255, 240, 0));
			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+50, mission_back->getContentSize().height/2.f+13));
			mission_back->addChild(count_label);
			
			CCLabelTTF* main_ment = CCLabelTTF::create("부하몹을 가두어 잡으세요.", mySGD->getFont().c_str(), 17);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_bigArea)
		{
			int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
			int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			title_img = CCSprite::create("mission_title_bigarea.png");
			
			CCSprite* count_img = CCSprite::create("mission_count.png");
			count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-40, mission_back->getContentSize().height/2.f+13));
			mission_back->addChild(count_img);
			
			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d%% x %d", percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 23);
			count_label->setColor(ccc3(255, 240, 0));
			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+60, mission_back->getContentSize().height/2.f+13));
			mission_back->addChild(count_label);
			
			CCLabelTTF* main_ment = CCLabelTTF::create(CCString::createWithFormat("한번에 %d%%이상 영역을 %d번 획득하세요!", percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 17);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_itemCollect)
		{
			int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			
			title_img = CCSprite::create("mission_title_itemcollect.png");
			
			CCSprite* count_img = CCSprite::create("mission_count2.png");
			count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
			mission_back->addChild(count_img);
			
			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d개", count_value)->getCString(), mySGD->getFont().c_str(), 23);
			count_label->setColor(ccc3(255, 240, 0));
			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+40, mission_back->getContentSize().height/2.f+13));
			mission_back->addChild(count_label);
			
			CCLabelTTF* main_ment = CCLabelTTF::create(CCString::createWithFormat("%d개의 아이템을 획득하세요.", count_value)->getCString(), mySGD->getFont().c_str(), 17);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_perfect)
		{
			int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
			
			title_img = CCSprite::create("mission_title_perfect.png");
			
			CCSprite* count_img = CCSprite::create("mission_area.png");
			count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+20));
			mission_back->addChild(count_img);
			
			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d%%", percent_value)->getCString(), mySGD->getFont().c_str(), 23);
			count_label->setColor(ccc3(255, 240, 0));
			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+40, mission_back->getContentSize().height/2.f+20));
			mission_back->addChild(count_label);
			
			CCLabelTTF* main_ment = CCLabelTTF::create(CCString::createWithFormat("정확한 %d%%로 게임을 클리어하세요.", percent_value)->getCString(), mySGD->getFont().c_str(), 17);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-12));
			mission_back->addChild(main_ment);
			
			CCLabelTTF* sub_ment = CCLabelTTF::create("이 미션에서는 3단계 카드를 얻을 수 없어요!", mySGD->getFont().c_str(), 12);
			sub_ment->setColor(ccc3(125, 125, 125));
			sub_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-33));
			mission_back->addChild(sub_ment);
		}
		else if(mission_type == kCLEAR_timeLimit)
		{
			int sec_value = NSDS_GI(stage_number, kSDS_SI_missionOptionSec_i);
			
			title_img = CCSprite::create("mission_title_timelimit.png");
			
			CCSprite* count_img = CCSprite::create("mission_time.png");
			count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
			mission_back->addChild(count_img);
			
			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d초", sec_value)->getCString(), mySGD->getFont().c_str(), 23);
			count_label->setColor(ccc3(255, 240, 0));
			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+40, mission_back->getContentSize().height/2.f+13));
			mission_back->addChild(count_label);
			
			CCLabelTTF* main_ment = CCLabelTTF::create("정해진 시간 내 클리어하세요.", mySGD->getFont().c_str(), 17);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_sequenceChange)
		{
			title_img = CCSprite::create("mission_title_sequencechange.png");
			
			CCLabelTTF* main1_ment = CCLabelTTF::create("게임 중 나오는 ", mySGD->getFont().c_str(), 17);
			main1_ment->setAnchorPoint(ccp(1,0.5));
			main1_ment->setPosition(ccp(mission_back->getContentSize().width/2.f-13, mission_back->getContentSize().height/2.f+20));
			mission_back->addChild(main1_ment);
			
			for(int i=0;i<6;i++)
			{
				CCSprite* t_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", i+1)->getCString());
				t_img->setPosition(ccp(main1_ment->getPositionX() + 9 + 18*i, mission_back->getContentSize().height/2.f+20));
				mission_back->addChild(t_img);
			}
			
			CCLabelTTF* main2_ment = CCLabelTTF::create("를", mySGD->getFont().c_str(), 17);
			main2_ment->setAnchorPoint(ccp(0,0.5));
			main2_ment->setPosition(ccp(main1_ment->getPositionX() + 18*6, mission_back->getContentSize().height/2.f+20));
			mission_back->addChild(main2_ment);
			
			CCLabelTTF* main3_ment = CCLabelTTF::create("순서대로 획득하세요!", mySGD->getFont().c_str(), 17);
			main3_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-6));
			mission_back->addChild(main3_ment);
			
			CCLabelTTF* sub_ment = CCLabelTTF::create("이 미션에서는 1단계 카드를 얻을 수 없어요!", mySGD->getFont().c_str(), 12);
			sub_ment->setColor(ccc3(125, 125, 125));
			sub_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-33));
			mission_back->addChild(sub_ment);
		}
		
		title_img->setPosition(ccp(mission_back->getContentSize().width/2.f+20, mission_back->getContentSize().height/2.f+68));
		mission_back->addChild(title_img);
		
		loading_tip_node->addChild(KSGradualValue<int>::create(255, 0, 0.5f, [=](int t)
															   {
																   KS::setOpacity(mission_back, t);
															   }, [=](int t)
															   {
																   KS::setOpacity(mission_back, 0);
															   }));
	}
	else
	{
		int selected_loading_tip = mySGD->before_curtain_tip_type;
		
		CCSprite* left_curtain = CCSprite::create("curtain_left.png");
		left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
		left_curtain->setPosition(ccp(0, 0));
		loading_tip_node->addChild(left_curtain);
		
		CCDelayTime* left_delay = CCDelayTime::create(0.5f);
		CCMoveTo* left_in = CCMoveTo::create(0.5f, ccp(-240,0));
		CCSequence* left_seq = CCSequence::create(left_delay, left_in, NULL);
		left_curtain->runAction(left_seq);
		
		CCSprite* right_curtain = CCSprite::create("curtain_left.png");
		right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
		right_curtain->setFlipX(true);
		right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
		right_curtain->setPosition(ccp(0,0));
		loading_tip_node->addChild(right_curtain);
		
		CCDelayTime* right_delay = CCDelayTime::create(0.5f);
		CCMoveTo* right_in = CCMoveTo::create(0.5f, ccp(240,0));
		CCSequence* right_seq = CCSequence::create(right_delay, right_in, NULL);
		right_curtain->runAction(right_seq);
		
		//	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
		string tip_filename = "loading_tip_";
		if(selected_loading_tip == 0)
			tip_filename += "achievement";
		else if(selected_loading_tip == 1)
			tip_filename += "bonusgame";
		else if(selected_loading_tip == 2)
			tip_filename += "bosstip1";
		else if(selected_loading_tip == 3)
			tip_filename += "bosstip2";
		else if(selected_loading_tip == 4)
			tip_filename += "cardsetting";
		else if(selected_loading_tip == 5)
			tip_filename += "challenge";
		else if(selected_loading_tip == 6)
			tip_filename += "change";
		else if(selected_loading_tip == 7)
			tip_filename += "clear";
		else if(selected_loading_tip == 8)
			tip_filename += "continue";
		else if(selected_loading_tip == 9)
			tip_filename += "newitem1";
		else if(selected_loading_tip == 10)
			tip_filename += "newitem2";
		else if(selected_loading_tip == 11)
			tip_filename += "newpuzzle";
		else if(selected_loading_tip == 12)
			tip_filename += "option";
		else if(selected_loading_tip == 13)
			tip_filename += "shop";
		else
			tip_filename += "bonusgame";
		
		tip_filename += ".png";
		
		CCSprite* content_img = CCSprite::create(tip_filename.c_str());
		content_img->setPosition(ccp(0, 0));
		content_img->setOpacity(255);
		loading_tip_node->addChild(content_img);
		
		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
		content_img->runAction(t_fade);
	}
	
	return loading_tip_node;
}

CCNode* LoadingTipScene::getCurtainTipImage()
{
	int total_loading_tip = 14;
	int selected_loading_tip = rand()%total_loading_tip;
	
	CCNode* loading_tip_node = CCNode::create();
	CCSprite* left_curtain = CCSprite::create("curtain_left.png");
	left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
	left_curtain->setPosition(ccp(-240, 0));
	loading_tip_node->addChild(left_curtain);
	
	CCMoveTo* left_in = CCMoveTo::create(0.5f, ccp(0,0));
	left_curtain->runAction(left_in);
	
	CCSprite* right_curtain = CCSprite::create("curtain_left.png");
	right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
	right_curtain->setFlipX(true);
	right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
	right_curtain->setPosition(ccp(240,0));
	loading_tip_node->addChild(right_curtain);
	
	CCMoveTo* right_in = CCMoveTo::create(0.5f, ccp(0,0));
	right_curtain->runAction(right_in);
	
	mySGD->before_curtain_tip_type = selected_loading_tip;
	
	//	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
	string tip_filename = "loading_tip_";
	if(selected_loading_tip == 0)
		tip_filename += "achievement";
	else if(selected_loading_tip == 1)
		tip_filename += "bonusgame";
	else if(selected_loading_tip == 2)
		tip_filename += "bosstip1";
	else if(selected_loading_tip == 3)
		tip_filename += "bosstip2";
	else if(selected_loading_tip == 4)
		tip_filename += "cardsetting";
	else if(selected_loading_tip == 5)
		tip_filename += "challenge";
	else if(selected_loading_tip == 6)
		tip_filename += "change";
	else if(selected_loading_tip == 7)
		tip_filename += "clear";
	else if(selected_loading_tip == 8)
		tip_filename += "continue";
	else if(selected_loading_tip == 9)
		tip_filename += "newitem1";
	else if(selected_loading_tip == 10)
		tip_filename += "newitem2";
	else if(selected_loading_tip == 11)
		tip_filename += "newpuzzle";
	else if(selected_loading_tip == 12)
		tip_filename += "option";
	else if(selected_loading_tip == 13)
		tip_filename += "shop";
	else
		tip_filename += "bonusgame";
	
	tip_filename += ".png";
	
	CCSprite* content_img = CCSprite::create(tip_filename.c_str());
	content_img->setPosition(ccp(0, 0));
	content_img->setOpacity(0);
	loading_tip_node->addChild(content_img);
	
	CCDelayTime* t_delay = CCDelayTime::create(0.5f);
	CCFadeTo* t_fade = CCFadeTo::create(0.5f, 255);
	CCSequence* t_seq = CCSequence::create(t_delay, t_fade, NULL);
	content_img->runAction(t_seq);
	
	return loading_tip_node;
}

CCSprite* LoadingTipScene::getLoadingTipImage()
{
	int total_loading_tip = 14;
	int selected_loading_tip = rand()%total_loading_tip;
	
	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
	string tip_filename = "loading_tip_";
	if(selected_loading_tip == 0)
		tip_filename += "achievement";
	else if(selected_loading_tip == 1)
		tip_filename += "bonusgame";
	else if(selected_loading_tip == 2)
		tip_filename += "bosstip1";
	else if(selected_loading_tip == 3)
		tip_filename += "bosstip2";
	else if(selected_loading_tip == 4)
		tip_filename += "cardsetting";
	else if(selected_loading_tip == 5)
		tip_filename += "challenge";
	else if(selected_loading_tip == 6)
		tip_filename += "change";
	else if(selected_loading_tip == 7)
		tip_filename += "clear";
	else if(selected_loading_tip == 8)
		tip_filename += "continue";
	else if(selected_loading_tip == 9)
		tip_filename += "newitem1";
	else if(selected_loading_tip == 10)
		tip_filename += "newitem2";
	else if(selected_loading_tip == 11)
		tip_filename += "newpuzzle";
	else if(selected_loading_tip == 12)
		tip_filename += "option";
	else if(selected_loading_tip == 13)
		tip_filename += "shop";
	else
		tip_filename += "bonusgame";
	
	tip_filename += ".png";
	
	CCSprite* content_img = CCSprite::create(tip_filename.c_str());
	content_img->setPosition(ccp(0, 0));
	loading_tip_back->addChild(content_img);
	
	return loading_tip_back;
}

void LoadingTipScene::readyLoading()
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	
	sil_load_list.clear();
	default_load_list.clear();
	
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_progress_img->setPosition(ccp(240,38));
	addChild(loading_progress_img, kLoadingTipZorder_content);
	reader->release();
	
	if(next_scene_name == "maingame")
	{
		sil_load_list.push_back(CCString::createWithFormat("card%d_visible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))->getCString());
		sil_load_list.push_back(CCString::createWithFormat("card%d_invisible.png", NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1))->getCString());
		
//		default_load_list.push_back("top_back.png");
//		default_load_list.push_back("bottom_back.png");
		default_load_list.push_back("normal_frame_top.png");
		default_load_list.push_back("normal_frame_bottom.png");
		default_load_list.push_back("normal_frame_left.png");
		default_load_list.push_back("normal_frame_right.png");
	}
	else if(next_scene_name == "newmainflow")
	{
		int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		
		sil_load_list.push_back(CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, "original")->getCString());
		sil_load_list.push_back(CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, "original")->getCString());
		sil_load_list.push_back(CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, "original")->getCString());
		sil_load_list.push_back(CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, "original")->getCString());
		
		default_load_list.push_back("mainflow_back_wall.png");
		default_load_list.push_back("mainflow_back_shadow_left.png");
		default_load_list.push_back("mainflow_back_shadow_right.png");
	}
	else
	{
		default_load_list.push_back("mainflow_back_wall.png");
	}
	
	total_load_img = sil_load_list.size() + default_load_list.size();
	ing_load_img = 0;
	
	progress_label = CCLabelBMFont::create(CCString::createWithFormat("%.0f", (100.f*ing_load_img)/total_load_img)->getCString(), "allfont.fnt");
	progress_label->setPosition(ccp(240,38));
	addChild(progress_label, kLoadingTipZorder_content);
	
	is_minimum_time = false;
	
	if(!is_mission_tip)
	{
		CCDelayTime* t_delay = CCDelayTime::create(1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(LoadingTipScene::onMinimumTime));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
	else
	{
		CCDelayTime* t_delay = CCDelayTime::create(2.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(LoadingTipScene::showButton));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		runAction(t_seq);
	}
	
	startLoading();
}

void LoadingTipScene::showButton()
{
	int mission_type = NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i);
	
	if(mission_type == kCLEAR_bossLifeZero && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_bossLifeZero))
		onMinimumTime();
	else if(mission_type == kCLEAR_subCumberCatch && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_subCumberCatch))
		onMinimumTime();
	else if(mission_type == kCLEAR_bigArea && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_bigArea))
		onMinimumTime();
	else if(mission_type == kCLEAR_itemCollect && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_itemCollect))
		onMinimumTime();
	else if(mission_type == kCLEAR_perfect && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_perfect))
		onMinimumTime();
	else if(mission_type == kCLEAR_timeLimit && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_timeLimit))
		onMinimumTime();
	else if(mission_type == kCLEAR_sequenceChange && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_sequenceChange))
		onMinimumTime();
	else
	{
		ok_img->setVisible(true);
		ok_menu->setVisible(true);
		no_review->setVisible(true);
	}
}

void LoadingTipScene::onMinimumTime()
{
	is_minimum_time = true;
	if(ing_load_img == total_load_img)
	{
		is_minimum_time = false;
		endLoadingTip();
	}
}

void LoadingTipScene::startLoading()
{
	for(int i=0;i<total_load_img;i++)
	{
		if(i < sil_load_list.size())
			mySIL->addImageAsync(sil_load_list[ing_load_img].c_str(), this, callfuncO_selector(LoadingTipScene::countingFunc));
		else
			CCTextureCache::sharedTextureCache()->addImageAsync(default_load_list[i-sil_load_list.size()].c_str(), this, callfuncO_selector(LoadingTipScene::countingFunc));
	}
}

void LoadingTipScene::countingFunc(CCObject *sender)
{
	ing_load_img++;
	progress_label->setString(CCString::createWithFormat("%d\t%d", ing_load_img, total_load_img)->getCString());
	
	if(ing_load_img == total_load_img && is_minimum_time)
	{
		is_minimum_time = false;
		endLoadingTip();
	}
}

void LoadingTipScene::endLoadingTip()
{
	if(next_scene_name == "maingame")
		CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	else if(next_scene_name == "newmainflow")
		CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
	else if(next_scene_name == "playtutorial")
		CCDirector::sharedDirector()->replaceScene(PlayTutorial::scene());
}

void LoadingTipScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void LoadingTipScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(LoadingTipScene::alertAction));
}