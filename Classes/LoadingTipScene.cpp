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
#include "StyledLabelTTF.h"

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
		if(next_scene_name == "maingame" || next_scene_name == "playtutorial")
		{
			CCNode* tip_img = getCurtainTipImage();
			tip_img->setPosition(ccp(240,160));
			addChild(tip_img, kLoadingTipZorder_back, 9999);
			
			CCDelayTime* t_delay = CCDelayTime::create(0.6f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(LoadingTipScene::readyLoading));
			CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
			tip_img->runAction(t_seq);
		}
		else
		{
			CCSprite* tip_img = getLoadingTipImage();
			tip_img->setPosition(ccp(240,160));
			addChild(tip_img, kLoadingTipZorder_back, 9999);
			
			readyLoading();
		}
	}
	else
	{
		CCNode* tip_img = getMissionTipImage();
		tip_img->setPosition(ccp(240,160));
		addChild(tip_img, kLoadingTipZorder_back, 9999);
		
		CCDelayTime* t_delay = CCDelayTime::create(0.6f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(LoadingTipScene::readyLoading));
		CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
		tip_img->runAction(t_seq);
	}
		
	return true;
}

CCNode* LoadingTipScene::getMissionTipImage()
{
	CCNode* loading_tip_node = CCNode::create();
	
	
	CCSprite* loading_tip_back = CCSprite::create("temp_title_back2.png");
	loading_tip_back->setPosition(ccp(0,0));
	loading_tip_back->setVisible(false);
	loading_tip_node->addChild(loading_tip_back);
	
	CCDelayTime* back_delay = CCDelayTime::create(0.3f);
	CCShow* back_show = CCShow::create();
	CCSequence* back_seq = CCSequence::create(back_delay, back_show, NULL);
	
	loading_tip_back->runAction(back_seq);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	CCSprite* black_img = CCSprite::create("whitePaper.png");
	black_img->setColor(ccBLACK);
	black_img->setOpacity(0);
	black_img->setPosition(ccp(0,0));
	black_img->setScaleX(screen_scale_x);
	black_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	loading_tip_node->addChild(black_img, 3);
	
	CCFadeTo* black_fadein = CCFadeTo::create(0.3f, 255);
	CCFadeTo* black_fadeout = CCFadeTo::create(0.3f, 0);
	CCCallFunc* black_remove = CCCallFunc::create(black_img, callfunc_selector(CCSprite::removeFromParent));
	CCSequence* black_seq = CCSequence::create(black_fadein, black_fadeout, black_remove, NULL);
	black_img->runAction(black_seq);
	
	
//	CCSprite* left_curtain = CCSprite::create("curtain_left.png");
//	left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//	left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
//	left_curtain->setPosition(ccp(-240, 0));
//	loading_tip_node->addChild(left_curtain);
//	
//	CCMoveTo* left_in = CCMoveTo::create(0.5f, ccp(0,0));
//	left_curtain->runAction(left_in);
//	
//	CCSprite* right_curtain = CCSprite::create("curtain_left.png");
//	right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//	right_curtain->setFlipX(true);
//	right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
//	right_curtain->setPosition(ccp(240,0));
//	loading_tip_node->addChild(right_curtain);
//	
//	CCMoveTo* right_in = CCMoveTo::create(0.5f, ccp(0,0));
//	right_curtain->runAction(right_in);
	
	CCScale9Sprite* mission_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	mission_back->setContentSize(CCSizeMake(320, 200));
	mission_back->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f));
	loading_tip_back->addChild(mission_back);
	
	CCScale9Sprite* inner_back = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	inner_back->setContentSize(CCSizeMake(mission_back->getContentSize().width-15, 100));
	inner_back->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+10));
	mission_back->addChild(inner_back);
	
	CCScale9Sprite* mission_img = CCScale9Sprite::create("common_time.png", CCRectMake(0, 0, 22, 22), CCRectMake(10, 10, 2, 2));
	mission_img->setContentSize(CCSizeMake(65, 22));
	mission_img->setPosition(ccp(10+mission_img->getContentSize().width/2.f,mission_back->getContentSize().height-25));
	mission_back->addChild(mission_img);
	
	KSLabelTTF* mission_img_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mission), mySGD->getFont().c_str(), 12);
	mission_img_label->setColor(ccc3(255, 170, 20));
	mission_img_label->setPosition(ccp(mission_img->getContentSize().width/2.f, mission_img->getContentSize().height/2.f));
	mission_img->addChild(mission_img_label);
	
	ok_img = KS::loadCCBI<CCSprite*>(this, CCString::createWithFormat("button_ok_%s.ccbi", myLoc->getLocalCode()->getCString())->getCString()).first;
	ok_img->setPosition(ccp(mission_back->getContentSize().width*0.5f, mission_back->getContentSize().height*0.18f));
	mission_back->addChild(ok_img);
	ok_img->setVisible(false);
	
	CCSprite* n_ok = CCSprite::create(CCString::createWithFormat("button_ok_%s.png", myLoc->getLocalCode()->getCString())->getCString());
	n_ok->setOpacity(0);
	CCSprite* s_ok = CCSprite::create(CCString::createWithFormat("button_ok_%s.png", myLoc->getLocalCode()->getCString())->getCString());
	s_ok->setOpacity(0);
	
	CCMenuItemLambda* ok_item = CCMenuItemSpriteLambda::create(n_ok, s_ok, [=](CCObject* sender)
															   {
																   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																   onMinimumTime();
															   });
	ok_menu = CCMenuLambda::createWithItem(ok_item);
	ok_menu->setPosition(ccp(mission_back->getContentSize().width*0.5f, mission_back->getContentSize().height*0.18f));
	mission_back->addChild(ok_menu);
	ok_menu->setVisible(false);
	ok_menu->setTouchPriority(-600);
	
	
//	no_review = CommonButton::create("다시보지않기", 13, CCSizeMake(100, 50), CommonButtonGreen, -600);
//	no_review->setPosition(ccp(mission_back->getContentSize().width*0.3f, mission_back->getContentSize().height*0.18f));
//	mission_back->addChild(no_review);
//	no_review->setVisible(false);
	
	int stage_number = mySD->getSilType();
	int mission_type = NSDS_GI(stage_number, kSDS_SI_missionType_i);
	
	KSLabelTTF* title_img = KSLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionTitle0+mission_type)), mySGD->getFont().c_str(), 15);
	title_img->setColor(ccc3(255, 170, 20));
	title_img->setAnchorPoint(ccp(0,0.5f));
	title_img->setPosition(ccp(mission_img->getPositionX() + mission_img->getContentSize().width/2.f+10, mission_img->getPositionY()));
	mission_back->addChild(title_img);
	
	if(mission_type == kCLEAR_bossLifeZero)
	{
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setOldAnchorPoint();
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+8));
		mission_back->addChild(main_ment);
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_bossLifeZero, true);
//								   onMinimumTime();
//							   });
	}
	else if(mission_type == kCLEAR_subCumberCatch)
	{
		int catch_count = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionCondition0+mission_type)), catch_count)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setOldAnchorPoint();
		t_condition_label->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+25));
		mission_back->addChild(t_condition_label);
		
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setOldAnchorPoint();
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-10));
		mission_back->addChild(main_ment);
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_subCumberCatch, true);
//								   onMinimumTime();
//							   });
	}
	else if(mission_type == kCLEAR_bigArea)
	{
		int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
		int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionCondition0+mission_type)), percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setOldAnchorPoint();
		t_condition_label->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+25));
		mission_back->addChild(t_condition_label);
		
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setOldAnchorPoint();
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-10));
		mission_back->addChild(main_ment);
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_bigArea, true);
//								   onMinimumTime();
//							   });
	}
	else if(mission_type == kCLEAR_itemCollect)
	{
		int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionCondition0+mission_type)), count_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setOldAnchorPoint();
		t_condition_label->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+25));
		mission_back->addChild(t_condition_label);
		
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setOldAnchorPoint();
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-10));
		mission_back->addChild(main_ment);
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_itemCollect, true);
//								   onMinimumTime();
//							   });
	}
	else if(mission_type == kCLEAR_perfect)
	{
		int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
		
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
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_perfect, true);
//								   onMinimumTime();
//							   });
	}
	else if(mission_type == kCLEAR_timeLimit)
	{
		int sec_value = mySDS->getIntegerForKey(kSDF_stageInfo, stage_number, "playtime") - mySD->getClearConditionTimeLimit();
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionCondition0+mission_type)), sec_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setOldAnchorPoint();
		t_condition_label->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+25));
		mission_back->addChild(t_condition_label);
		
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setOldAnchorPoint();
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-10));
		mission_back->addChild(main_ment);
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_timeLimit, true);
//								   onMinimumTime();
//							   });
	}
	else if(mission_type == kCLEAR_sequenceChange)
	{
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setOldAnchorPoint();
		main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+8));
		mission_back->addChild(main_ment);
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_sequenceChange, true);
//								   onMinimumTime();
//							   });
	}
	
//	mission_back->setScale(1.5f);
//	KS::setOpacity(mission_back, 0);
//	loading_tip_node->addChild(KSGradualValue<int>::create(-255, 102, 0.7f, [=](int t)
//								{
//									if(t >= 0)
//									{
//										mission_back->setScale(1.5f - t/204.f);
//										KS::setOpacity(mission_back, t/0.2f*0.5f);
//										n_ok->setOpacity(0);
//										s_ok->setOpacity(0);
//									}
//								}, [=](int t)
//								{
//									mission_back->setScale(1.f);
//									KS::setOpacity(mission_back, 255);
//									n_ok->setOpacity(0);
//									s_ok->setOpacity(0);
//								}));
	
	return loading_tip_node;
}

CCNode* LoadingTipScene::getOpenCurtainNode(bool is_gameover)
{
	CCNode* loading_tip_node = CCNode::create();
	
	if(!is_gameover && NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i) != kCLEAR_default)
	{
		CCSprite* loading_tip_back = CCSprite::create("temp_title_back2.png");
		loading_tip_back->setPosition(ccp(0,0));
		loading_tip_node->addChild(loading_tip_back);
		
		CCDelayTime* back_delay = CCDelayTime::create(0.3f);
		CCCallFunc* back_remove = CCCallFunc::create(loading_tip_back, callfunc_selector(CCSprite::removeFromParent));
		CCSequence* back_seq = CCSequence::create(back_delay, back_remove, NULL);
		
		loading_tip_back->runAction(back_seq);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* black_img = CCSprite::create("whitePaper.png");
		black_img->setColor(ccBLACK);
		black_img->setOpacity(0);
		black_img->setPosition(ccp(0,0));
		black_img->setScaleX(screen_scale_x);
		black_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		loading_tip_node->addChild(black_img, 3);
		
		CCFadeTo* black_fadein = CCFadeTo::create(0.3f, 255);
		CCFadeTo* black_fadeout = CCFadeTo::create(0.3f, 0);
		CCCallFunc* black_remove = CCCallFunc::create(black_img, callfunc_selector(CCSprite::removeFromParent));
		CCSequence* black_seq = CCSequence::create(black_fadein, black_fadeout, black_remove, NULL);
		black_img->runAction(black_seq);
		
		
		
		
//		CCSprite* left_curtain = CCSprite::create("curtain_left.png");
//		left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//		left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
//		left_curtain->setPosition(ccp(0, 0));
//		loading_tip_node->addChild(left_curtain);
//		
//		CCDelayTime* left_delay = CCDelayTime::create(0.5f);
//		CCMoveTo* left_in = CCMoveTo::create(0.5f, ccp(-300,0));
//		CCSequence* left_seq = CCSequence::create(left_delay, left_in, NULL);
//		left_curtain->runAction(left_seq);
//		
//		CCSprite* right_curtain = CCSprite::create("curtain_left.png");
//		right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//		right_curtain->setFlipX(true);
//		right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
//		right_curtain->setPosition(ccp(0,0));
//		loading_tip_node->addChild(right_curtain);
//		
//		CCDelayTime* right_delay = CCDelayTime::create(0.5f);
//		CCMoveTo* right_in = CCMoveTo::create(0.5f, ccp(300,0));
//		CCSequence* right_seq = CCSequence::create(right_delay, right_in, NULL);
//		right_curtain->runAction(right_seq);
		
		CCScale9Sprite* mission_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		mission_back->setContentSize(CCSizeMake(320, 200));
		mission_back->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f));
		loading_tip_back->addChild(mission_back);
		
		CCScale9Sprite* inner_back = CCScale9Sprite::create("mainpopup_pupple4.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		inner_back->setContentSize(CCSizeMake(mission_back->getContentSize().width-15, 100));
		inner_back->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+10));
		mission_back->addChild(inner_back);
		
		CCScale9Sprite* mission_img = CCScale9Sprite::create("common_time.png", CCRectMake(0, 0, 22, 22), CCRectMake(10, 10, 2, 2));
		mission_img->setContentSize(CCSizeMake(65, 22));
		mission_img->setPosition(ccp(10+mission_img->getContentSize().width/2.f,mission_back->getContentSize().height-25));
		mission_back->addChild(mission_img);
		
		KSLabelTTF* mission_img_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mission), mySGD->getFont().c_str(), 12);
		mission_img_label->setColor(ccc3(255, 170, 20));
		mission_img_label->setPosition(ccp(mission_img->getContentSize().width/2.f, mission_img->getContentSize().height/2.f));
		mission_img->addChild(mission_img_label);
		
		int stage_number = mySD->getSilType();
		int mission_type = NSDS_GI(stage_number, kSDS_SI_missionType_i);
		
		KSLabelTTF* title_img = KSLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionTitle0+mission_type)), mySGD->getFont().c_str(), 15);
		title_img->setColor(ccc3(255, 170, 20));
		title_img->setAnchorPoint(ccp(0,0.5f));
		title_img->setPosition(ccp(mission_img->getPositionX() + mission_img->getContentSize().width/2.f+10, mission_img->getPositionY()));
		mission_back->addChild(title_img);
		
		
		if(mission_type == kCLEAR_bossLifeZero)
		{
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setOldAnchorPoint();
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+8));
			mission_back->addChild(main_ment);
			
//			CCLabelTTF* main1_ment = CCLabelTTF::create("공격으로 보스몹의 에너지를", mySGD->getFont().c_str(), 17);
//			main1_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+18));
//			mission_back->addChild(main1_ment);
//			
//			CCLabelTTF* main2_ment = CCLabelTTF::create("모두 소진시키세요.", mySGD->getFont().c_str(), 17);
//			main2_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-7));
//			mission_back->addChild(main2_ment);
//			
//			CCLabelTTF* sub_ment = CCLabelTTF::create("보스의 에너지가 다 소진되어도 게임은 계속...", mySGD->getFont().c_str(), 12);
//			sub_ment->setColor(ccc3(125, 125, 125));
//			sub_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-37));
//			mission_back->addChild(sub_ment);
		}
		else if(mission_type == kCLEAR_subCumberCatch)
		{
			int catch_count = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);

			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionCondition0+mission_type)), catch_count)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setOldAnchorPoint();
			t_condition_label->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+25));
			mission_back->addChild(t_condition_label);
			
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-10));
			main_ment->setOldAnchorPoint();
			mission_back->addChild(main_ment);
			
//			CCSprite* catch_count_img = CCSprite::create("mission_catch_count.png");
//			catch_count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
//			mission_back->addChild(catch_count_img);
//			
//			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d마리", catch_count)->getCString(), mySGD->getFont().c_str(), 23);
//			count_label->setColor(ccc3(255, 240, 0));
//			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+50, mission_back->getContentSize().height/2.f+13));
//			mission_back->addChild(count_label);
//			
//			CCLabelTTF* main_ment = CCLabelTTF::create("부하몹을 가두어 잡으세요.", mySGD->getFont().c_str(), 17);
//			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
//			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_bigArea)
		{
			int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
			int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);

			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionCondition0+mission_type)), percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+25));
			t_condition_label->setOldAnchorPoint();
			mission_back->addChild(t_condition_label);
			
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-10));
			main_ment->setOldAnchorPoint();
			mission_back->addChild(main_ment);
			
//			CCSprite* count_img = CCSprite::create("mission_count.png");
//			count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-40, mission_back->getContentSize().height/2.f+13));
//			mission_back->addChild(count_img);
//			
//			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d%% x %d", percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 23);
//			count_label->setColor(ccc3(255, 240, 0));
//			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+60, mission_back->getContentSize().height/2.f+13));
//			mission_back->addChild(count_label);
//			
//			CCLabelTTF* main_ment = CCLabelTTF::create(CCString::createWithFormat("한번에 %d%%이상 영역을 %d번 획득하세요!", percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 17);
//			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
//			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_itemCollect)
		{
			int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);

			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionCondition0+mission_type)), count_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setOldAnchorPoint();
			t_condition_label->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+25));
			mission_back->addChild(t_condition_label);
			
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setOldAnchorPoint();
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-10));
			mission_back->addChild(main_ment);
			
//			CCSprite* count_img = CCSprite::create("mission_count2.png");
//			count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
//			mission_back->addChild(count_img);
//			
//			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d개", count_value)->getCString(), mySGD->getFont().c_str(), 23);
//			count_label->setColor(ccc3(255, 240, 0));
//			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+40, mission_back->getContentSize().height/2.f+13));
//			mission_back->addChild(count_label);
//			
//			CCLabelTTF* main_ment = CCLabelTTF::create(CCString::createWithFormat("%d개의 아이템을 획득하세요.", count_value)->getCString(), mySGD->getFont().c_str(), 17);
//			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
//			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_perfect)
		{
			int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
			
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
			int sec_value = mySDS->getIntegerForKey(kSDF_stageInfo, stage_number, "playtime") - mySD->getClearConditionTimeLimit();

			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionCondition0+mission_type)), sec_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+25));
			t_condition_label->setOldAnchorPoint();
			mission_back->addChild(t_condition_label);
			
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-10));
			main_ment->setOldAnchorPoint();
			mission_back->addChild(main_ment);
			
//			CCSprite* count_img = CCSprite::create("mission_time.png");
//			count_img->setPosition(ccp(mission_back->getContentSize().width/2.f-30, mission_back->getContentSize().height/2.f+13));
//			mission_back->addChild(count_img);
//			
//			CCLabelTTF* count_label = CCLabelTTF::create(CCString::createWithFormat("%d초", sec_value)->getCString(), mySGD->getFont().c_str(), 23);
//			count_label->setColor(ccc3(255, 240, 0));
//			count_label->setPosition(ccp(mission_back->getContentSize().width/2.f+40, mission_back->getContentSize().height/2.f+13));
//			mission_back->addChild(count_label);
//			
//			CCLabelTTF* main_ment = CCLabelTTF::create("정해진 시간 내 클리어하세요.", mySGD->getFont().c_str(), 17);
//			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-23));
//			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_sequenceChange)
		{
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(kMyLocalKey_missionDiscription0+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f+8));
			mission_back->addChild(main_ment);
			main_ment->setOldAnchorPoint();
//			CCLabelTTF* main1_ment = CCLabelTTF::create("게임 중 나오는 ", mySGD->getFont().c_str(), 17);
//			main1_ment->setAnchorPoint(ccp(1,0.5));
//			main1_ment->setPosition(ccp(mission_back->getContentSize().width/2.f-13, mission_back->getContentSize().height/2.f+20));
//			mission_back->addChild(main1_ment);
//			
//			for(int i=0;i<6;i++)
//			{
//				CCSprite* t_img = CCSprite::create(CCString::createWithFormat("exchange_%d_act.png", i+1)->getCString());
//				t_img->setPosition(ccp(main1_ment->getPositionX() + 9 + 18*i, mission_back->getContentSize().height/2.f+20));
//				mission_back->addChild(t_img);
//			}
//			
//			CCLabelTTF* main2_ment = CCLabelTTF::create("를", mySGD->getFont().c_str(), 17);
//			main2_ment->setAnchorPoint(ccp(0,0.5));
//			main2_ment->setPosition(ccp(main1_ment->getPositionX() + 18*6, mission_back->getContentSize().height/2.f+20));
//			mission_back->addChild(main2_ment);
//			
//			CCLabelTTF* main3_ment = CCLabelTTF::create("순서대로 획득하세요!", mySGD->getFont().c_str(), 17);
//			main3_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-6));
//			mission_back->addChild(main3_ment);
//			
//			CCLabelTTF* sub_ment = CCLabelTTF::create("이 미션에서는 1단계 카드를 얻을 수 없어요!", mySGD->getFont().c_str(), 12);
//			sub_ment->setColor(ccc3(125, 125, 125));
//			sub_ment->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height/2.f-33));
//			mission_back->addChild(sub_ment);
		}
		
//		loading_tip_node->addChild(KSGradualValue<int>::create(255, 0, 0.2f, [=](int t)
//															   {
//																   mission_back->setScale(1.f + (255-t)/510.f);
//																   KS::setOpacity(mission_back, t);
//															   }, [=](int t)
//															   {
//																   mission_back->setScale(1.5f);
//																   KS::setOpacity(mission_back, 0);
//															   }));
	}
	else
	{
		int selected_loading_tip = mySGD->before_curtain_tip_type;
		
		CCSprite* loading_tip_back = CCSprite::create("temp_title_back2.png");
		loading_tip_back->setPosition(ccp(0,0));
		loading_tip_node->addChild(loading_tip_back);
		
		CCDelayTime* back_delay = CCDelayTime::create(0.3f);
		CCCallFunc* back_remove = CCCallFunc::create(loading_tip_back, callfunc_selector(CCSprite::removeFromParent));
		CCSequence* back_seq = CCSequence::create(back_delay, back_remove, NULL);
		
		loading_tip_back->runAction(back_seq);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* black_img = CCSprite::create("whitePaper.png");
		black_img->setColor(ccBLACK);
		black_img->setOpacity(0);
		black_img->setPosition(ccp(0,0));
		black_img->setScaleX(screen_scale_x);
		black_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		loading_tip_node->addChild(black_img, 3);
		
		CCFadeTo* black_fadein = CCFadeTo::create(0.3f, 255);
		CCFadeTo* black_fadeout = CCFadeTo::create(0.3f, 0);
		CCCallFunc* black_remove = CCCallFunc::create(black_img, callfunc_selector(CCSprite::removeFromParent));
		CCSequence* black_seq = CCSequence::create(black_fadein, black_fadeout, black_remove, NULL);
		black_img->runAction(black_seq);
		
		KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(selected_loading_tip+kMyLocalKey_titleLoadingBegin+1)), mySGD->getFont().c_str(), 15, CCSizeMake(350, 200), CCTextAlignment::kCCTextAlignmentCenter, CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
		content_label->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f));
		loading_tip_back->addChild(content_label);
		
//		string tip_filename = "loading_tip_";
//		if(selected_loading_tip == 0)
//			tip_filename += "achievement";
//		else if(selected_loading_tip == 1)
//			tip_filename += "bosstip1";
//		else if(selected_loading_tip == 2)
//			tip_filename += "bosstip2";
//		else if(selected_loading_tip == 3)
//			tip_filename += "newpuzzle";
//		else
//			tip_filename += "bosstip1";
//		
//		tip_filename += ".png";
//		
//		CCSprite* content_img = CCSprite::create(tip_filename.c_str());
//		content_img->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f));
//		loading_tip_back->addChild(content_img);
	}
	
	CCDelayTime* t_delay = CCDelayTime::create(0.6f);
	CCCallFunc* t_call = CCCallFunc::create(loading_tip_node, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
	loading_tip_node->runAction(t_seq);
	
	return loading_tip_node;
}

CCNode* LoadingTipScene::getCurtainTipImage()
{
	int total_loading_tip = kMyLocalKey_titleLoadingEnd - kMyLocalKey_titleLoadingBegin - 1;
	int selected_loading_tip = rand()%total_loading_tip;
	
	CCNode* loading_tip_node = CCNode::create();
	
	CCSprite* loading_tip_back = CCSprite::create("temp_title_back2.png");
	loading_tip_back->setPosition(ccp(0,0));
	loading_tip_back->setVisible(false);
	loading_tip_node->addChild(loading_tip_back);
	
	CCDelayTime* back_delay = CCDelayTime::create(0.3f);
	CCShow* back_show = CCShow::create();
	CCSequence* back_seq = CCSequence::create(back_delay, back_show, NULL);
	
	loading_tip_back->runAction(back_seq);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	CCSprite* black_img = CCSprite::create("whitePaper.png");
	black_img->setColor(ccBLACK);
	black_img->setOpacity(0);
	black_img->setPosition(ccp(0,0));
	black_img->setScaleX(screen_scale_x);
	black_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	loading_tip_node->addChild(black_img, 3);
	
	CCFadeTo* black_fadein = CCFadeTo::create(0.3f, 255);
	CCFadeTo* black_fadeout = CCFadeTo::create(0.3f, 0);
	CCCallFunc* black_remove = CCCallFunc::create(black_img, callfunc_selector(CCSprite::removeFromParent));
	CCSequence* black_seq = CCSequence::create(black_fadein, black_fadeout, black_remove, NULL);
	black_img->runAction(black_seq);
	
//	CCSprite* left_curtain = CCSprite::create("curtain_left.png");
//	left_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//	left_curtain->setAnchorPoint(ccp(1.f, 0.5f));
//	left_curtain->setPosition(ccp(-240, 0));
//	loading_tip_node->addChild(left_curtain);
//	
//	CCMoveTo* left_in = CCMoveTo::create(0.5f, ccp(0,0));
//	left_curtain->runAction(left_in);
//	
//	CCSprite* right_curtain = CCSprite::create("curtain_left.png");
//	right_curtain->setScale(1.f/myDSH->screen_convert_rate * ((myDSH->puzzle_ui_top < 320.f ? 320.f : myDSH->puzzle_ui_top)/320.f));
//	right_curtain->setFlipX(true);
//	right_curtain->setAnchorPoint(ccp(0.f, 0.5f));
//	right_curtain->setPosition(ccp(240,0));
//	loading_tip_node->addChild(right_curtain);
//	
//	CCMoveTo* right_in = CCMoveTo::create(0.5f, ccp(0,0));
//	right_curtain->runAction(right_in);
	
	mySGD->before_curtain_tip_type = selected_loading_tip;
	
	//	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
	
	KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(selected_loading_tip+kMyLocalKey_titleLoadingBegin+1)), mySGD->getFont().c_str(), 15, CCSizeMake(350, 200), CCTextAlignment::kCCTextAlignmentCenter, CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	content_label->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f));
	loading_tip_back->addChild(content_label);
	
//	string tip_filename = "loading_tip_";
//	if(selected_loading_tip == 0)
//		tip_filename += "achievement";
//	else if(selected_loading_tip == 1)
//		tip_filename += "bosstip1";
//	else if(selected_loading_tip == 2)
//		tip_filename += "bosstip2";
//	else if(selected_loading_tip == 3)
//		tip_filename += "newpuzzle";
//	else
//		tip_filename += "bosstip1";
//	
//	tip_filename += ".png";
//	
//	CCSprite* content_img = CCSprite::create(tip_filename.c_str());
//	content_img->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f));
//	loading_tip_back->addChild(content_img);
	
	return loading_tip_node;
}

CCSprite* LoadingTipScene::getLoadingTipImage()
{
	int total_loading_tip = kMyLocalKey_titleLoadingEnd - kMyLocalKey_titleLoadingBegin - 1;
	int selected_loading_tip = rand()%total_loading_tip;
	
	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
	
	KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(MyLocalKey(selected_loading_tip+kMyLocalKey_titleLoadingBegin+1)), mySGD->getFont().c_str(), 15, CCSizeMake(350, 200), CCTextAlignment::kCCTextAlignmentCenter, CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
	content_label->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f));
	loading_tip_back->addChild(content_label);
	
//	string tip_filename = "loading_tip_";
//	if(selected_loading_tip == 0)
//		tip_filename += "achievement";
//	else if(selected_loading_tip == 1)
//		tip_filename += "bosstip1";
//	else if(selected_loading_tip == 2)
//		tip_filename += "bosstip2";
//	else if(selected_loading_tip == 3)
//		tip_filename += "newpuzzle";
//	else
//		tip_filename += "bosstip1";
//	
//	tip_filename += ".png";
//	
//	CCSprite* content_img = CCSprite::create(tip_filename.c_str());
//	content_img->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f));
//	loading_tip_back->addChild(content_img);
	
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
		default_load_list.push_back("fever_coin_bronze.png");
	}
	else if(next_scene_name == "newmainflow")
	{
		int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		
		sil_load_list.push_back(CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, "original")->getCString());
		sil_load_list.push_back(CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, "original")->getCString());
		sil_load_list.push_back(CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, "original")->getCString());
		sil_load_list.push_back(CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, "original")->getCString());
		
		default_load_list.push_back("mainflow_back_shadow_left.png");
		default_load_list.push_back("mainflow_back_shadow_right.png");
	}
	else
	{
		default_load_list.push_back("whitePaper.png");
	}
	
	total_load_img = sil_load_list.size() + default_load_list.size();
	ing_load_img = 0;
	
	progress_label = KSLabelTTF::create(CCString::createWithFormat("%.0f", (100.f*ing_load_img)/total_load_img)->getCString(), mySGD->getFont().c_str(), 11);
	progress_label->enableOuterStroke(ccBLACK, 1.f);
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
//		no_review->setVisible(true);
	}
}

void LoadingTipScene::onMinimumTime()
{
//	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

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
	progress_label->setString(CCString::createWithFormat("%.0f", (100.f*ing_load_img)/total_load_img)->getCString());
	
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