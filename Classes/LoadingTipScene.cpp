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
#include "TypingBox.h"
#include "MainFlowScene.h"

#include "StoryLayer.h"
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
	no_img = NULL;
	
	
	if(next_scene_name == "maingame" && NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i) != kCLEAR_default)
		is_mission_tip = true;
	else
		is_mission_tip = false;
	
	if(!is_mission_tip)
	{
		if(next_scene_name == "maingame" || next_scene_name == "playtutorial" || next_scene_name == "mainflow")
		{
			CCNode* tip_img = getCurtainTipImage();
			tip_img->setPosition(ccp(240,160));
			addChild(tip_img, kLoadingTipZorder_back, 9999);
			
			content_node = tip_img;
			
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
			
			content_node = tip_img;
			
			readyLoading();
		}
	}
	else
	{
		CCNode* tip_img = getMissionTipImage();
		tip_img->setPosition(ccp(240,160));
		addChild(tip_img, kLoadingTipZorder_back, 9999);
		
		content_node = tip_img;
		
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
	
	CCSprite* loading_tip_back = CCSprite::create("main_back.png");
	loading_tip_back->setPosition(ccp(0,0));
	loading_tip_back->setVisible(false);
	loading_tip_node->addChild(loading_tip_back, 0, 1);
	
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
	
	
	CCScale9Sprite* mission_back = CCScale9Sprite::create("common_mission.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	mission_back->setStringData("missionPannel");
	mission_back->setContentSize(CCSizeMake(256, 103));
	mission_back->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f+40));
	loading_tip_back->addChild(mission_back,3);
	
	ok_img = CCSprite::create("subbutton_purple4.png");// KS::loadCCBI<CCSprite*>(this, CCString::createWithFormat("button_ok_%s.ccbi", myLoc->getLocalCode()->getCString())->getCString()).first;
	ok_img->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f-90));
	loading_tip_back->addChild(ok_img);
	ok_img->setVisible(false);
	
	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
	ok_label->disableOuterStroke();
	ok_label->setPosition(ccpFromSize(ok_img->getContentSize()/2.f));
	ok_img->addChild(ok_label);
	
	CCScaleTo* t_scale1 = CCScaleTo::create(0.3f, 0.9f, 1.1f);
	CCScaleTo* t_scale2 = CCScaleTo::create(0.3f, 1.1f, 0.9f);
	CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, NULL);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
	
	ok_img->runAction(t_repeat);
	
	CCSprite* n_ok = CCSprite::create("subbutton_purple4.png");//CCString::createWithFormat("button_ok_%s.png", myLoc->getLocalCode()->getCString())->getCString());
	n_ok->setOpacity(0);
	CCSprite* s_ok = CCSprite::create("subbutton_purple4.png");//CCString::createWithFormat("button_ok_%s.png", myLoc->getLocalCode()->getCString())->getCString());
	s_ok->setOpacity(0);
	
	CCMenuItemLambda* ok_item = CCMenuItemSpriteLambda::create(n_ok, s_ok, [=](CCObject* sender)
															   {
																   this->ok_menu->setEnabled(false);
																   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																   onMinimumTime();
															   });
	ok_item->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f-90));
	ok_menu = CCMenuLambda::createWithItem(ok_item);
	ok_menu->setPosition(CCPointZero);
	loading_tip_back->addChild(ok_menu);
	ok_menu->setVisible(false);
	ok_menu->setTouchPriority(-600);
	
	
	int stage_number = mySD->getSilType();
	int mission_type = NSDS_GI(stage_number, kSDS_SI_missionType_i);
	
	CCSprite* title_img = CCSprite::create("mission_title.png");
	title_img->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height-24));
	mission_back->addChild(title_img);
	
	if(mission_type == kCLEAR_bossLifeZero)
	{
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setAnchorPoint(ccp(0.5f,0.5f));
		main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f));
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
		
		CCNode* junior_node = CCNode::create();
		junior_node->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(-25.5f,0));
		mission_back->addChild(junior_node);
		
		std::string juniorInfo = mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "junior");
		
		Json::Reader reader;
		Json::Value juniorJson;
		reader.parse(juniorInfo, juniorJson);
		
		int juniorIndex = 0;
		juniorJson = juniorJson[juniorIndex];
		{
			std::string juniorType = juniorJson["type"].asString();
			std::string ccbiName = juniorType;
			std::string ccbiname2 = ccbiName;
			
			if(ccbiname2.substr(0,1)=="1") {
				ccbiname2="bear";
			}
			////////////////////////////////////////////
			
			//		m_directionAngleDegree = m_well512.GetValue(0, 360);
			
			//		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
			//		ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
			
			//		cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
			
			
			std::string _ccbiName = (ccbiname2 + ".ccbi").c_str();
			CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
			cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(nodeLoader);
			CCNode* p = reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+_ccbiName).c_str(), this);
			
			CCSprite* m_juniorSprite = dynamic_cast<CCSprite*>(p);
			m_juniorSprite->addChild(reader->getAnimationManager());
			
			reader->release();
			
			if(m_juniorSprite != NULL)
			{
				junior_node->addChild(m_juniorSprite, 1001);
				m_juniorSprite->setScale(0.5f);
				m_juniorSprite->setPosition(CCPointZero);
			}
		}
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionCondition0)+mission_type)), catch_count)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(18,0));
		mission_back->addChild(t_condition_label);
		
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setAnchorPoint(ccp(0.5f,0.5f));
		main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-26));
		mission_back->addChild(main_ment);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_catcher))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_catcher, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
			
			
			
//			
//			
//			CCNode* scenario_node = CCNode::create();
//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
//			loading_tip_back->addChild(scenario_node, 2);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			
//			CCSprite* yagyu = CCSprite::create("kt_cha_yagyu_1.png");
//			yagyu->setAnchorPoint(ccp(0,0));
//			yagyu->setPosition(ccp(240-240*screen_scale_x-yagyu->getContentSize().width, 160-160*screen_scale_y));
//			scenario_node->addChild(yagyu, 1);
//			
//			CCSprite* hibari = CCSprite::create("kt_cha_hibari_1.png");
//			hibari->setAnchorPoint(ccp(1,0));
//			hibari->setPosition(ccp(240+240*screen_scale_x+hibari->getContentSize().width, 160-160*screen_scale_y));
//			hibari->setVisible(false);
//			scenario_node->addChild(hibari, 1);
//			
//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
//			typing_box->setHide();
//			scenario_node->addChild(typing_box, 2);
//			
//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
//			scenario_node->addChild(typing_box2, 2);
//			
//			typing_box2->setTouchOffScrollAndButton();
//			typing_box2->setVisible(false);
//			typing_box2->setTouchSuction(false);
//			
//			typing_box->showAnimation(0.3f);
//			
//			function<void()> end_func2 = [=]()
//			{
//				addChild(KSTimer::create(0.1f, [=]()
//																 {
//																	 scenario_node->removeFromParent();
//																 }));
//			};
//			
//			function<void()> end_func1 = [=]()
//			{
//				yagyu->setVisible(false);
//				hibari->setVisible(true);
//				
//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																															{
//																																hibari->setPositionX(240+240*screen_scale_x+hibari->getContentSize().width - hibari->getContentSize().width*2.f/3.f*t);
//																															}, [=](float t)
//																															{
//																																hibari->setPositionX(240+240*screen_scale_x+hibari->getContentSize().width - hibari->getContentSize().width*2.f/3.f*t);
//																																
//																																typing_box2->setVisible(true);
//																																typing_box2->setTouchSuction(true);
//																																
//																																typing_box->setTouchSuction(false);
//																																
//																																typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial12), end_func2);
//																															}));
//				typing_box->setTouchOffScrollAndButton();
//				typing_box->setVisible(false);
//			};
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																														{
//																															yagyu->setPositionX(240-240*screen_scale_x-yagyu->getContentSize().width + yagyu->getContentSize().width*2.f/3.f*t);
//																														}, [=](float t)
//																														{
//																															yagyu->setPositionX(240-240*screen_scale_x-yagyu->getContentSize().width + yagyu->getContentSize().width*2.f/3.f*t);
//																															
//																															typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial11), end_func1);
//																														}));
		
		
			StoryLayer::startStory(this, "mission2",nullptr);
		
		}
		
			
			
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
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionCondition0)+mission_type)), percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,10));
		mission_back->addChild(t_condition_label);
		
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setAnchorPoint(ccp(0.5f,0.5f));
		main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-10));
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
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionCondition0)+mission_type)), count_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-26));
		mission_back->addChild(t_condition_label);
		
		CCSprite* item_img = CCSprite::create("mission_item_set.png");
		item_img->setPosition(ccpFromSize(mission_back->getContentSize()/2.f));
		mission_back->addChild(item_img);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_collector))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_collector, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
			
			
			
//			CCNode* scenario_node = CCNode::create();
//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
//			loading_tip_back->addChild(scenario_node, 2);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			
//			CCSprite* asuka = CCSprite::create("kt_cha_asuka_1.png");
//			asuka->setAnchorPoint(ccp(0,0));
//			asuka->setPosition(ccp(240-240*screen_scale_x-asuka->getContentSize().width, 160-160*screen_scale_y));
//			scenario_node->addChild(asuka, 1);
//			
//			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//			ikaruga->setAnchorPoint(ccp(1,0));
//			ikaruga->setPosition(ccp(240+240*screen_scale_x+ikaruga->getContentSize().width, 160-160*screen_scale_y));
//			ikaruga->setVisible(false);
//			scenario_node->addChild(ikaruga, 1);
//			
//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
//			typing_box->setHide();
//			scenario_node->addChild(typing_box, 2);
//			
//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
//			scenario_node->addChild(typing_box2, 2);
//			
//			typing_box2->setTouchOffScrollAndButton();
//			typing_box2->setVisible(false);
//			typing_box2->setTouchSuction(false);
//			
//			typing_box->showAnimation(0.3f);
//			
//			function<void()> end_func2 = [=]()
//			{
//				addChild(KSTimer::create(0.1f, [=]()
//										 {
//											 scenario_node->removeFromParent();
//										 }));
//			};
//			
//			function<void()> end_func1 = [=]()
//			{
//				asuka->setVisible(false);
//				ikaruga->setVisible(true);
//				
//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																	  {
//																		  ikaruga->setPositionX(240+240*screen_scale_x+ikaruga->getContentSize().width - ikaruga->getContentSize().width*2.f/3.f*t);
//																	  }, [=](float t)
//																	  {
//																		  ikaruga->setPositionX(240+240*screen_scale_x+ikaruga->getContentSize().width - ikaruga->getContentSize().width*2.f/3.f*t);
//																		  
//																		  typing_box2->setVisible(true);
//																		  typing_box2->setTouchSuction(true);
//																		  
//																		  typing_box->setTouchSuction(false);
//																		  
//																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial14), end_func2);
//																	  }));
//				typing_box->setTouchOffScrollAndButton();
//				typing_box->setVisible(false);
//			};
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																  {
//																	  asuka->setPositionX(240-240*screen_scale_x-asuka->getContentSize().width + asuka->getContentSize().width*2.f/3.f*t);
//																  }, [=](float t)
//																  {
//																	  asuka->setPositionX(240-240*screen_scale_x-asuka->getContentSize().width + asuka->getContentSize().width*2.f/3.f*t);
//																	  
//																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial13), end_func1);
//																  }));

			
			StoryLayer::startStory(this, "mission4",nullptr);
		}
		
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
		
		CCSprite* time_img = CCSprite::create("item8.png");
		time_img->setScale(0.5f);
		time_img->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(-20,0));
		mission_back->addChild(time_img);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionCondition0)+mission_type)), sec_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(20,0));
		mission_back->addChild(t_condition_label);
		
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setAnchorPoint(ccp(0.5f,0.5f));
		main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-26));
		mission_back->addChild(main_ment);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_business))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_business, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
//			CCNode* scenario_node = CCNode::create();
//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
//			loading_tip_back->addChild(scenario_node, 2);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			
//			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//			ikaruga->setAnchorPoint(ccp(0,0));
//			ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//			scenario_node->addChild(ikaruga, 1);
//			
//			CCSprite* katsuragi = CCSprite::create("kt_cha_katsuragi_1.png");
//			katsuragi->setAnchorPoint(ccp(1,0));
//			katsuragi->setPosition(ccp(240+240*screen_scale_x+katsuragi->getContentSize().width, 160-160*screen_scale_y));
//			katsuragi->setVisible(false);
//			scenario_node->addChild(katsuragi, 1);
//			
//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
//			typing_box->setHide();
//			scenario_node->addChild(typing_box, 2);
//			
//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
//			scenario_node->addChild(typing_box2, 2);
//			
//			typing_box2->setTouchOffScrollAndButton();
//			typing_box2->setVisible(false);
//			typing_box2->setTouchSuction(false);
//			
//			typing_box->showAnimation(0.3f);
//			
//			function<void()> end_func2 = [=]()
//			{
//				addChild(KSTimer::create(0.1f, [=]()
//										 {
//											 scenario_node->removeFromParent();
//										 }));
//			};
//			
//			function<void()> end_func1 = [=]()
//			{
//				ikaruga->setVisible(false);
//				katsuragi->setVisible(true);
//				
//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																	  }, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																		  
//																		  typing_box2->setVisible(true);
//																		  typing_box2->setTouchSuction(true);
//																		  
//																		  typing_box->setTouchSuction(false);
//																		  
//																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial16), end_func2);
//																	  }));
//				typing_box->setTouchOffScrollAndButton();
//				typing_box->setVisible(false);
//			};
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  }, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																	  
//																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial15), end_func1);
//																  }));

			
			StoryLayer::startStory(this, "mission7",nullptr);
		}
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_timeLimit, true);
//								   onMinimumTime();
//							   });
	}
	else if(mission_type == kCLEAR_sequenceChange)
	{
		StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		main_ment->setAnchorPoint(ccp(0.5f,0.5f));
		main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,0));
		mission_back->addChild(main_ment);
		
//		no_review->setFunction([=](CCObject* sender)
//							   {
//								   myDSH->setBoolForKey(kDSH_Key_mission_willNeverWatch_sequenceChange, true);
//								   onMinimumTime();
//							   });
	}
	else if(mission_type == kCLEAR_hellMode)
	{
//		int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
//		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition8), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
		mission_back->addChild(t_condition_label);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_hell))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_hell, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
			//			CCNode* scenario_node = CCNode::create();
			//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
			//			loading_tip_back->addChild(scenario_node, 2);
			//
			//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			//			if(screen_scale_x < 1.f)
			//				screen_scale_x = 1.f;
			//
			//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
			//
			//
			//			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
			//			ikaruga->setAnchorPoint(ccp(0,0));
			//			ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
			//			scenario_node->addChild(ikaruga, 1);
			//
			//			CCSprite* katsuragi = CCSprite::create("kt_cha_katsuragi_1.png");
			//			katsuragi->setAnchorPoint(ccp(1,0));
			//			katsuragi->setPosition(ccp(240+240*screen_scale_x+katsuragi->getContentSize().width, 160-160*screen_scale_y));
			//			katsuragi->setVisible(false);
			//			scenario_node->addChild(katsuragi, 1);
			//
			//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
			//			typing_box->setHide();
			//			scenario_node->addChild(typing_box, 2);
			//
			//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
			//			scenario_node->addChild(typing_box2, 2);
			//
			//			typing_box2->setTouchOffScrollAndButton();
			//			typing_box2->setVisible(false);
			//			typing_box2->setTouchSuction(false);
			//
			//			typing_box->showAnimation(0.3f);
			//
			//			function<void()> end_func2 = [=]()
			//			{
			//				addChild(KSTimer::create(0.1f, [=]()
			//										 {
			//											 scenario_node->removeFromParent();
			//										 }));
			//			};
			//
			//			function<void()> end_func1 = [=]()
			//			{
			//				ikaruga->setVisible(false);
			//				katsuragi->setVisible(true);
			//
			//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
			//																	  {
			//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
			//																	  }, [=](float t)
			//																	  {
			//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
			//
			//																		  typing_box2->setVisible(true);
			//																		  typing_box2->setTouchSuction(true);
			//
			//																		  typing_box->setTouchSuction(false);
			//
			//																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial16), end_func2);
			//																	  }));
			//				typing_box->setTouchOffScrollAndButton();
			//				typing_box->setVisible(false);
			//			};
			//
			//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
			//																  {
			//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
			//																  }, [=](float t)
			//																  {
			//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
			//
			//																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial15), end_func1);
			//																  }));
			
			
			StoryLayer::startStory(this, "mission8",nullptr);
		}
	}
	else if(mission_type == kCLEAR_percentage)
	{
		int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition9), condition_value), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
		mission_back->addChild(t_condition_label);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_percent))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_percent, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
			
//			CCNode* scenario_node = CCNode::create();
//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
//			loading_tip_back->addChild(scenario_node, 2);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			
//			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//			ikaruga->setAnchorPoint(ccp(0,0));
//			ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//			scenario_node->addChild(ikaruga, 1);
//			
//			CCSprite* katsuragi = CCSprite::create("kt_cha_katsuragi_1.png");
//			katsuragi->setAnchorPoint(ccp(1,0));
//			katsuragi->setPosition(ccp(240+240*screen_scale_x+katsuragi->getContentSize().width, 160-160*screen_scale_y));
//			katsuragi->setVisible(false);
//			scenario_node->addChild(katsuragi, 1);
//			
//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
//			typing_box->setHide();
//			scenario_node->addChild(typing_box, 2);
//			
//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
//			scenario_node->addChild(typing_box2, 2);
//			
//			typing_box2->setTouchOffScrollAndButton();
//			typing_box2->setVisible(false);
//			typing_box2->setTouchSuction(false);
//			
//			typing_box->showAnimation(0.3f);
//			
//			function<void()> end_func2 = [=]()
//			{
//				addChild(KSTimer::create(0.1f, [=]()
//										 {
//											 scenario_node->removeFromParent();
//										 }));
//			};
//			
//			function<void()> end_func1 = [=]()
//			{
//				ikaruga->setVisible(false);
//				katsuragi->setVisible(true);
//				
//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																	  }, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																		  
//																		  typing_box2->setVisible(true);
//																		  typing_box2->setTouchSuction(true);
//																		  
//																		  typing_box->setTouchSuction(false);
//																		  
//																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial24), end_func2);
//																	  }));
//				typing_box->setTouchOffScrollAndButton();
//				typing_box->setVisible(false);
//			};
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  }, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																	  
//																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial23), end_func1);
//																  }));
			
			
			StoryLayer::startStory(this, "mission9",nullptr);
		}
	}
	else if(mission_type == kCLEAR_score)
	{
		int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition10), KS::insert_separator(condition_value).c_str()), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
		mission_back->addChild(t_condition_label);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_score))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_score, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
			
//			CCNode* scenario_node = CCNode::create();
//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
//			loading_tip_back->addChild(scenario_node, 2);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			
//			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//			ikaruga->setAnchorPoint(ccp(0,0));
//			ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//			scenario_node->addChild(ikaruga, 1);
//			
//			CCSprite* katsuragi = CCSprite::create("kt_cha_katsuragi_1.png");
//			katsuragi->setAnchorPoint(ccp(1,0));
//			katsuragi->setPosition(ccp(240+240*screen_scale_x+katsuragi->getContentSize().width, 160-160*screen_scale_y));
//			katsuragi->setVisible(false);
//			scenario_node->addChild(katsuragi, 1);
//			
//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
//			typing_box->setHide();
//			scenario_node->addChild(typing_box, 2);
//			
//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
//			scenario_node->addChild(typing_box2, 2);
//			
//			typing_box2->setTouchOffScrollAndButton();
//			typing_box2->setVisible(false);
//			typing_box2->setTouchSuction(false);
//			
//			typing_box->showAnimation(0.3f);
//			
//			function<void()> end_func2 = [=]()
//			{
//				addChild(KSTimer::create(0.1f, [=]()
//										 {
//											 scenario_node->removeFromParent();
//										 }));
//			};
//			
//			function<void()> end_func1 = [=]()
//			{
//				ikaruga->setVisible(false);
//				katsuragi->setVisible(true);
//				
//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																	  }, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																		  
//																		  typing_box2->setVisible(true);
//																		  typing_box2->setTouchSuction(true);
//																		  
//																		  typing_box->setTouchSuction(false);
//																		  
//																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial26), end_func2);
//																	  }));
//				typing_box->setTouchOffScrollAndButton();
//				typing_box->setVisible(false);
//			};
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  }, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																	  
//																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial25), end_func1);
//																  }));

			
			StoryLayer::startStory(this, "mission10",nullptr);
		}
	}
	else if(mission_type == kCLEAR_combo)
	{
		int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition11), condition_value), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
		mission_back->addChild(t_condition_label);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_combo))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_combo, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
//			
//			CCNode* scenario_node = CCNode::create();
//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
//			loading_tip_back->addChild(scenario_node, 2);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			
//			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//			ikaruga->setAnchorPoint(ccp(0,0));
//			ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//			scenario_node->addChild(ikaruga, 1);
//			
//			CCSprite* katsuragi = CCSprite::create("kt_cha_katsuragi_1.png");
//			katsuragi->setAnchorPoint(ccp(1,0));
//			katsuragi->setPosition(ccp(240+240*screen_scale_x+katsuragi->getContentSize().width, 160-160*screen_scale_y));
//			katsuragi->setVisible(false);
//			scenario_node->addChild(katsuragi, 1);
//			
//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
//			typing_box->setHide();
//			scenario_node->addChild(typing_box, 2);
//			
//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
//			scenario_node->addChild(typing_box2, 2);
//			
//			typing_box2->setTouchOffScrollAndButton();
//			typing_box2->setVisible(false);
//			typing_box2->setTouchSuction(false);
//			
//			typing_box->showAnimation(0.3f);
//			
//			function<void()> end_func2 = [=]()
//			{
//				addChild(KSTimer::create(0.1f, [=]()
//										 {
//											 scenario_node->removeFromParent();
//										 }));
//			};
//			
//			function<void()> end_func1 = [=]()
//			{
//				ikaruga->setVisible(false);
//				katsuragi->setVisible(true);
//				
//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																	  }, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																		  
//																		  typing_box2->setVisible(true);
//																		  typing_box2->setTouchSuction(true);
//																		  
//																		  typing_box->setTouchSuction(false);
//																		  
//																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial28), end_func2);
//																	  }));
//				typing_box->setTouchOffScrollAndButton();
//				typing_box->setVisible(false);
//			};
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  }, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																	  
//																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial27), end_func1);
//																  }));

			
			StoryLayer::startStory(this, "mission11",nullptr);
		}
	}
	else if(mission_type == kCLEAR_gold)
	{
		int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition12), KS::insert_separator(condition_value).c_str()), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
		mission_back->addChild(t_condition_label);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_gold))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_gold, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
			
//			CCNode* scenario_node = CCNode::create();
//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
//			loading_tip_back->addChild(scenario_node, 2);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			
//			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//			ikaruga->setAnchorPoint(ccp(0,0));
//			ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//			scenario_node->addChild(ikaruga, 1);
//			
//			CCSprite* katsuragi = CCSprite::create("kt_cha_katsuragi_1.png");
//			katsuragi->setAnchorPoint(ccp(1,0));
//			katsuragi->setPosition(ccp(240+240*screen_scale_x+katsuragi->getContentSize().width, 160-160*screen_scale_y));
//			katsuragi->setVisible(false);
//			scenario_node->addChild(katsuragi, 1);
//			
//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
//			typing_box->setHide();
//			scenario_node->addChild(typing_box, 2);
//			
//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
//			scenario_node->addChild(typing_box2, 2);
//			
//			typing_box2->setTouchOffScrollAndButton();
//			typing_box2->setVisible(false);
//			typing_box2->setTouchSuction(false);
//			
//			typing_box->showAnimation(0.3f);
//			
//			function<void()> end_func2 = [=]()
//			{
//				addChild(KSTimer::create(0.1f, [=]()
//										 {
//											 scenario_node->removeFromParent();
//										 }));
//			};
//			
//			function<void()> end_func1 = [=]()
//			{
//				ikaruga->setVisible(false);
//				katsuragi->setVisible(true);
//				
//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																	  }, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																		  
//																		  typing_box2->setVisible(true);
//																		  typing_box2->setTouchSuction(true);
//																		  
//																		  typing_box->setTouchSuction(false);
//																		  
//																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial30), end_func2);
//																	  }));
//				typing_box->setTouchOffScrollAndButton();
//				typing_box->setVisible(false);
//			};
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  }, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																	  
//																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial29), end_func1);
//																  }));

			
			StoryLayer::startStory(this, "mission12",nullptr);
		}
	}
	else if(mission_type == kCLEAR_turns)
	{
		int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
		
		StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition13), condition_value), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
		t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
		t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
		mission_back->addChild(t_condition_label);
		
		if(!myDSH->getBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_turn))
		{
			CCPoint center_position = ok_item->getPosition();
			ok_item->setPosition(center_position + ccp(60,0));
			ok_img->setPosition(center_position + ccp(60,0));
			
			no_img = CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1));
			no_img->setContentSize(CCSizeMake(120, 45));
			no_img->setPosition(center_position + ccp(-50,0));
			loading_tip_back->addChild(no_img);
			no_img->setVisible(false);
			
			KSLabelTTF* no_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_noReview), mySGD->getFont().c_str(), 13);
			no_label->disableOuterStroke();
			no_label->setPosition(ccpFromSize(no_img->getContentSize()/2.f));
			no_img->addChild(no_label);
			
			CCSprite* n_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			n_no->setOpacity(0);
			CCSprite* s_no = CCSprite::create("whitePaper.png", CCRectMake(0, 0, no_img->getContentSize().width, no_img->getContentSize().height));
			s_no->setOpacity(0);
			
			CCMenuItemLambda* no_item = CCMenuItemSpriteLambda::create(n_no, s_no, [=](CCObject* sender)
																	   {
																		   myDSH->setBoolForKey(kDSH_Key_showedKindTutorial_int1, KindTutorialType::kNewMission_turn, true);
																		   
																		   this->ok_menu->setEnabled(false);
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   onMinimumTime();
																	   });
			no_item->setPosition(center_position + ccp(-50,0));
			ok_menu->addChild(no_item);
			
			
			
			
//			CCNode* scenario_node = CCNode::create();
//			scenario_node->setPosition(ccpFromSize(loading_tip_back->getContentSize()/2.f) - ccp(240,160));
//			loading_tip_back->addChild(scenario_node, 2);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			
//			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
//			ikaruga->setAnchorPoint(ccp(0,0));
//			ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
//			scenario_node->addChild(ikaruga, 1);
//			
//			CCSprite* katsuragi = CCSprite::create("kt_cha_katsuragi_1.png");
//			katsuragi->setAnchorPoint(ccp(1,0));
//			katsuragi->setPosition(ccp(240+240*screen_scale_x+katsuragi->getContentSize().width, 160-160*screen_scale_y));
//			katsuragi->setVisible(false);
//			scenario_node->addChild(katsuragi, 1);
//			
//			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 80), ccp(225, 90));
//			typing_box->setHide();
//			scenario_node->addChild(typing_box, 2);
//			
//			TypingBox* typing_box2 = TypingBox::create(-9999, "kt_talkbox_blue.png", CCRectMake(0, 0, 85, 115), CCRectMake(22, 76, 23, 14), CCRectMake(22, 26, 23, 64), CCSizeMake(210, 80), ccp(255, 90));
//			scenario_node->addChild(typing_box2, 2);
//			
//			typing_box2->setTouchOffScrollAndButton();
//			typing_box2->setVisible(false);
//			typing_box2->setTouchSuction(false);
//			
//			typing_box->showAnimation(0.3f);
//			
//			function<void()> end_func2 = [=]()
//			{
//				addChild(KSTimer::create(0.1f, [=]()
//										 {
//											 scenario_node->removeFromParent();
//										 }));
//			};
//			
//			function<void()> end_func1 = [=]()
//			{
//				ikaruga->setVisible(false);
//				katsuragi->setVisible(true);
//				
//				scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																	  }, [=](float t)
//																	  {
//																		  katsuragi->setPositionX(240+240*screen_scale_x+katsuragi->getContentSize().width - katsuragi->getContentSize().width*2.f/3.f*t);
//																		  
//																		  typing_box2->setVisible(true);
//																		  typing_box2->setTouchSuction(true);
//																		  
//																		  typing_box->setTouchSuction(false);
//																		  
//																		  typing_box2->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial32), end_func2);
//																	  }));
//				typing_box->setTouchOffScrollAndButton();
//				typing_box->setVisible(false);
//			};
//			
//			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																  }, [=](float t)
//																  {
//																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
//																	  
//																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_kindTutorial31), end_func1);
//																  }));
			
			StoryLayer::startStory(this, "mission13",nullptr);
		}
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
	
	if(!mySGD->is_endless_mode && !is_gameover && NSDS_GI(mySD->getSilType(), kSDS_SI_missionType_i) != kCLEAR_default)
	{
		CCSprite* loading_tip_back = CCSprite::create("main_back.png");
		loading_tip_back->setPosition(ccp(0,0));
		loading_tip_node->addChild(loading_tip_back, 0, 1);
		
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
		
		
		CCScale9Sprite* mission_back = CCScale9Sprite::create("common_mission.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		mission_back->setContentSize(CCSizeMake(256, 103));
		mission_back->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, loading_tip_back->getContentSize().height/2.f+40));
		loading_tip_back->addChild(mission_back,3);
		
		
		int stage_number = mySD->getSilType();
		int mission_type = NSDS_GI(stage_number, kSDS_SI_missionType_i);
		
		CCSprite* title_img = CCSprite::create("mission_title.png");
		title_img->setPosition(ccp(mission_back->getContentSize().width/2.f, mission_back->getContentSize().height-24));
		mission_back->addChild(title_img);
		
		
		if(mission_type == kCLEAR_bossLifeZero)
		{
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setAnchorPoint(ccp(0.5f,0.5f));
			main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_subCumberCatch)
		{
			int catch_count = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			
			CCNode* junior_node = CCNode::create();
			junior_node->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(-25.5f,0));
			mission_back->addChild(junior_node);
			
			std::string juniorInfo = mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "junior");
			
			Json::Reader reader;
			Json::Value juniorJson;
			reader.parse(juniorInfo, juniorJson);
			
			int juniorIndex = 0;
			juniorJson = juniorJson[juniorIndex];
			{
				std::string juniorType = juniorJson["type"].asString();
				std::string ccbiName = juniorType;
				std::string ccbiname2 = ccbiName;
				
				if(ccbiname2.substr(0,1)=="1") {
					ccbiname2="bear";
				}
				////////////////////////////////////////////
				
				//		m_directionAngleDegree = m_well512.GetValue(0, 360);
				
				//		CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
				//		ccNodeLoaderLibrary->registerCCNodeLoader("CircleBossCCB", CircleLoader::loader());
				
				//		cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
				
				
				std::string _ccbiName = (ccbiname2 + ".ccbi").c_str();
				CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
				cocos2d::extension::CCBReader* reader = new cocos2d::extension::CCBReader(nodeLoader);
				CCNode* p = reader->readNodeGraphFromFileForFullPath((mySIL->getDocumentPath()+_ccbiName).c_str(), loading_tip_node);
				
				CCSprite* m_juniorSprite = dynamic_cast<CCSprite*>(p);
				m_juniorSprite->addChild(reader->getAnimationManager());
				//			m_juniorAnimation = reader->getAnimationManager();
				//			m_juniorAnimation->setDelegate(this);
				reader->release();
				
				if(m_juniorSprite != NULL)
				{
					junior_node->addChild(m_juniorSprite, 1001);
					m_juniorSprite->setScale(0.5f);
					m_juniorSprite->setPosition(CCPointZero);
				}
			}
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionCondition0)+mission_type)), catch_count)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(18,0));
			mission_back->addChild(t_condition_label);
			
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setAnchorPoint(ccp(0.5f,0.5f));
			main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-26));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_bigArea)
		{
			int percent_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
			int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionCondition0)+mission_type)), percent_value, count_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,10));
			mission_back->addChild(t_condition_label);
			
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setAnchorPoint(ccp(0.5f,0.5f));
			main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-10));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_itemCollect)
		{
			int count_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionCondition0)+mission_type)), count_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-26));
			mission_back->addChild(t_condition_label);
			
			CCSprite* item_img = CCSprite::create("mission_item_set.png");
			item_img->setPosition(ccpFromSize(mission_back->getContentSize()/2.f));
			mission_back->addChild(item_img);
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
			
			CCSprite* time_img = CCSprite::create("item8.png");
			time_img->setScale(0.5f);
			time_img->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(-20,0));
			mission_back->addChild(time_img);
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionCondition0)+mission_type)), sec_value)->getCString(), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(20,0));
			mission_back->addChild(t_condition_label);
			
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setAnchorPoint(ccp(0.5f,0.5f));
			main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-26));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_sequenceChange)
		{
			StyledLabelTTF* main_ment = StyledLabelTTF::create(myLoc->getLocalForKey(LK(int(LK::kMyLocalKey_missionDiscription0)+mission_type)), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
			main_ment->setAnchorPoint(ccp(0.5f,0.5f));
			main_ment->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,0));
			mission_back->addChild(main_ment);
		}
		else if(mission_type == kCLEAR_hellMode)
		{
			//		int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			//
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition8), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
			mission_back->addChild(t_condition_label);
		}
		else if(mission_type == kCLEAR_percentage)
		{
			int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionPercent_i);
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition9), condition_value), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
			mission_back->addChild(t_condition_label);
		}
		else if(mission_type == kCLEAR_score)
		{
			int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition10), KS::insert_separator(condition_value).c_str()), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
			mission_back->addChild(t_condition_label);
		}
		else if(mission_type == kCLEAR_combo)
		{
			int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition11), condition_value), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
			mission_back->addChild(t_condition_label);
		}
		else if(mission_type == kCLEAR_gold)
		{
			int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition12), KS::insert_separator(condition_value).c_str()), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
			mission_back->addChild(t_condition_label);
		}
		else if(mission_type == kCLEAR_turns)
		{
			int condition_value = NSDS_GI(stage_number, kSDS_SI_missionOptionCount_i);
			
			StyledLabelTTF* t_condition_label = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_missionCondition13), condition_value), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
			t_condition_label->setAnchorPoint(ccp(0.5f,0.5f));
			t_condition_label->setPosition(ccpFromSize(mission_back->getContentSize()/2.f) + ccp(0,-8));
			mission_back->addChild(t_condition_label);
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
		
		int back_number = mySGD->loading_tip_back_number;
		if(back_number == 0)
			back_number = 1;
		
		string back_img_filename;
		if(myDSH->getBoolForKey(kDSH_Key_isSafetyMode))
		{
			back_img_filename = "main_back.png";
		}
		else
		{
			back_img_filename = ccsf("loading_%d.png", back_number);
		}
		CCSprite* loading_tip_back = CCSprite::create(back_img_filename.c_str());
		loading_tip_back->setPosition(ccp(0,0));
		loading_tip_node->addChild(loading_tip_back, 0, 1);
		
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
		
		
//		if(back_number != 5)
//		{
			CCScale9Sprite* content_back = CCScale9Sprite::create("loading_tipbox.png", CCRectMake(0, 0, 55, 55), CCRectMake(27, 27, 1, 1));
			content_back->setContentSize(CCSizeMake(400, 80));
			content_back->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, 65));
			loading_tip_back->addChild(content_back);
			
			CCSprite* tip_title = CCSprite::create("loading_tip.png");
			tip_title->setPosition(ccp(35, content_back->getContentSize().height-5));
			content_back->addChild(tip_title);
		
		//	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
		
			KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(LK(selected_loading_tip+int(LK::kMyLocalKey_titleLoadingBegin)+1)), mySGD->getFont().c_str(), 13, CCSizeMake(350, 100), CCTextAlignment::kCCTextAlignmentCenter, CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
			content_label->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, 65));
			loading_tip_back->addChild(content_label);
//		}

		
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
	int total_loading_tip = int(LK::kMyLocalKey_titleLoadingEnd) - int(LK::kMyLocalKey_titleLoadingBegin) - 1;
	int selected_loading_tip = rand()%total_loading_tip;
	
	CCNode* loading_tip_node = CCNode::create();
	
	string back_img_filename;
	int rand_value;
	if(myDSH->getBoolForKey(kDSH_Key_isSafetyMode))
	{
		back_img_filename = "main_back.png";
	}
	else
	{
		rand_value = rand()%4+1;
		mySGD->loading_tip_back_number = rand_value;
		
		back_img_filename = ccsf("loading_%d.png", rand_value);
	}
	CCSprite* loading_tip_back = CCSprite::create(back_img_filename.c_str());
	loading_tip_back->setPosition(ccp(0,0));
	loading_tip_back->setVisible(false);
	loading_tip_node->addChild(loading_tip_back, 0, 1);
	
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
	
//	if(rand_value != 5)
//	{
		CCScale9Sprite* content_back = CCScale9Sprite::create("loading_tipbox.png", CCRectMake(0, 0, 55, 55), CCRectMake(27, 27, 1, 1));
		content_back->setContentSize(CCSizeMake(400, 80));
		content_back->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, 65));
		loading_tip_back->addChild(content_back);
		
		CCSprite* tip_title = CCSprite::create("loading_tip.png");
		tip_title->setPosition(ccp(35, content_back->getContentSize().height-5));
		content_back->addChild(tip_title);
	
	//	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
	
	
		KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(LK(selected_loading_tip+int(LK::kMyLocalKey_titleLoadingBegin)+1)), mySGD->getFont().c_str(), 13, CCSizeMake(350, 100), CCTextAlignment::kCCTextAlignmentCenter, CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
		content_label->setPosition(ccp(loading_tip_back->getContentSize().width/2.f, 65));
		loading_tip_back->addChild(content_label);
//	}
	
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
	int total_loading_tip = int(LK::kMyLocalKey_titleLoadingEnd) - int(LK::kMyLocalKey_titleLoadingBegin) - 1;
	int selected_loading_tip = rand()%total_loading_tip;
	
	CCSprite* loading_tip_back = CCSprite::create("loading_tip_back.png");
	
	KSLabelTTF* content_label = KSLabelTTF::create(myLoc->getLocalForKey(LK(selected_loading_tip+int(LK::kMyLocalKey_titleLoadingBegin)+1)), mySGD->getFont().c_str(), 13, CCSizeMake(350, 200), CCTextAlignment::kCCTextAlignmentCenter, CCVerticalTextAlignment::kCCVerticalTextAlignmentCenter);
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
	
	loading_tip_back->setTag(1);
	
	return loading_tip_back;
}

void LoadingTipScene::readyLoading()
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
//	myDSH->unregiAllCcbAnimationManagers();
	
	sil_load_list.clear();
	default_load_list.clear();
	
//	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//	CCBReader* reader = new CCBReader(nodeLoader);
//	CCSprite* loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
	loading_progress_img = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_progress_img->setPosition(ccpFromSize(content_node->getChildByTag(1)->getContentSize()/2.f) + ccp(0,-45));
	content_node->getChildByTag(1)->addChild(loading_progress_img, 1);
//	reader->release();
	
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
		
//		default_load_list.push_back("mainflow_back_shadow_left.png");
//		default_load_list.push_back("mainflow_back_shadow_right.png");
	}
	else
	{
		default_load_list.push_back("whitePaper.png");
	}
	
	total_load_img = sil_load_list.size() + default_load_list.size();
	ing_load_img = 0;
	
	progress_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", (100.f*ing_load_img)/total_load_img)->getCString(), mySGD->getFont().c_str(), 11);
	progress_label->enableOuterStroke(ccBLACK, 1.f);
	progress_label->setPosition(ccpFromSize(content_node->getChildByTag(1)->getContentSize()/2.f) + ccp(0,-45));
	content_node->getChildByTag(1)->addChild(progress_label, 1);
	
	if((next_scene_name == "maingame" || next_scene_name == "playtutorial") && !is_mission_tip)
	{
		loading_progress_img->setPosition(ccpFromSize(content_node->getChildByTag(1)->getContentSize()/2.f) + ccp(0,-40));
		progress_label->setPosition(ccpFromSize(content_node->getChildByTag(1)->getContentSize()/2.f) + ccp(0,-40));
	}
	
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
	
//	if(mission_type == kCLEAR_bossLifeZero && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_bossLifeZero))
//		onMinimumTime();
//	else if(mission_type == kCLEAR_subCumberCatch && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_subCumberCatch))
//		onMinimumTime();
//	else if(mission_type == kCLEAR_bigArea && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_bigArea))
//		onMinimumTime();
//	else if(mission_type == kCLEAR_itemCollect && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_itemCollect))
//		onMinimumTime();
//	else if(mission_type == kCLEAR_perfect && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_perfect))
//		onMinimumTime();
//	else if(mission_type == kCLEAR_timeLimit && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_timeLimit))
//		onMinimumTime();
//	else if(mission_type == kCLEAR_sequenceChange && myDSH->getBoolForKey(kDSH_Key_mission_willNeverWatch_sequenceChange))
//		onMinimumTime();
//	else
	{
		ok_img->setVisible(true);
		if(no_img)
			no_img->setVisible(true);
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
		{
			CCLOG("try %s", sil_load_list[ing_load_img].c_str());
			mySIL->addImageAsync(sil_load_list[ing_load_img].c_str(), this, callfuncO_selector(LoadingTipScene::countingFunc));
		}
		else
		{
			CCTextureCache::sharedTextureCache()->addImageAsync(default_load_list[i-sil_load_list.size()].c_str(), this, callfuncO_selector(LoadingTipScene::countingFunc));
			CCLOG("try %s", default_load_list[i-sil_load_list.size()].c_str());
		}
	}
}

void LoadingTipScene::countingFunc(CCObject *sender)
{
	ing_load_img++;
	progress_label->setString(CCString::createWithFormat("%.0f%%", (100.f*ing_load_img)/total_load_img)->getCString());
	if(ing_load_img == total_load_img)
	{
		progress_label->removeFromParent();
		loading_progress_img->removeFromParent();
		if(is_minimum_time)
		{
			is_minimum_time = false;
			endLoadingTip();
		}
	}
}

void LoadingTipScene::endLoadingTip()
{
	mySGD->resetLabels();
    
    CCLOG("replace scene : %s", next_scene_name.c_str());
    
	if(next_scene_name == "maingame")
		CCDirector::sharedDirector()->replaceScene(Maingame::scene());
	else if(next_scene_name == "newmainflow")
		CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
	else if(next_scene_name == "playtutorial")
		CCDirector::sharedDirector()->replaceScene(PlayTutorial::scene());
	else if(next_scene_name == "mainflow")
		CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
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
	
	CommonButton::callBackKey();
	//AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(LoadingTipScene::alertAction));
}