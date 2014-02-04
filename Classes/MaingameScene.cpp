//
//  MaingameScene.cpp
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#include "MaingameScene.h"
#include "AudioEngine.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "StarGoldData.h"
#include "StartMapGacha.h"
#include "SearchEye.h"
#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "ControlTutorialContent.h"
#include "PatternTutorialContent.h"
#include "RentCardAniContent.h"
#include "PuzzleScene.h"
#include "MainFlowScene.h"
#include "AchieveNoti.h"
#include "PauseContent.h"
#include "ContinueContent.h"
#include "StartSettingScene.h"
#include "AcceptChallengeAniContent.h"
//#include "ScreenSide.h"

CCScene* Maingame::scene()
{
    CCScene *scene = CCScene::create();
    Maingame *layer = Maingame::create();
    scene->addChild(layer);
	
    return scene;
}

bool Maingame::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setTag(0);
	AudioEngine::sharedInstance()->startGame();
	
	setKeypadEnabled(true);
	
	replay_continue_count = 0;
	replay_continue_label = NULL;
	
	isCheckingBacking = false;
	
	init_state = kMIS_beforeInit;
	
	myGD->V_V["Main_startBackTracking"] = std::bind(&Maingame::startBackTracking, this);
	myGD->V_V["Main_allStopSchedule"] = std::bind(&Maingame::allStopSchedule, this);
	myGD->V_V["Main_gameover"] = std::bind(&Maingame::gameover, this);
	myGD->V_V["Main_touchEnd"] = std::bind(&Maingame::touchEnd, this);
	myGD->V_V["Main_touchOn"] = std::bind(&Maingame::touchOn, this);
	myGD->V_V["Main_stunBackTracking"] = std::bind(&Maingame::stunBackTracking, this);
	myGD->V_V["Main_startSpecialAttack"] = std::bind(&Maingame::startSpecialAttack, this);
	myGD->V_V["Main_stopSpecialAttack"] = std::bind(&Maingame::stopSpecialAttack, this);
	myGD->V_CCP["Main_moveGamePosition"] = std::bind(&Maingame::moveGamePosition, this, _1);
	myGD->V_CCPI["Main_goldGettingEffect"] = std::bind(&Maingame::goldGettingEffect, this, _1, _2);
	myGD->V_FBCCP["Main_percentageGettingEffect"] = std::bind(&Maingame::percentageGettingEffect,
															  this, _1, _2, _3);
	myGD->V_Ip["Main_showLineDiePosition"] = std::bind(&Maingame::showLineDiePosition, this, _1);
	myGD->V_V["Main_resetIsLineDie"] = std::bind(&Maingame::resetIsLineDie, this);
	myGD->V_I["Main_showWarning"] = std::bind(&Maingame::showWarning, this, _1);
	myGD->V_Str["Main_showDetailMessage"] = std::bind(&Maingame::showDetailMessage, this, _1);
	myGD->V_Str["Main_showTextMessage"] = std::bind(&Maingame::showTextMessage, this, _1);
	myGD->V_V["Main_showCoin"] = std::bind(&Maingame::showCoin, this);
	myGD->V_V["Main_startExchange"] = std::bind(&Maingame::startExchange, this);
	myGD->V_V["Main_showTakeCoin"] = std::bind(&Maingame::showTakeCoin, this);
	myGD->V_V["Main_showChangeCard"] = std::bind(&Maingame::showChangeCard, this);
	myGD->V_V["Main_startMoveToBossPosition"] = std::bind(&Maingame::startMoveToBossPosition, this);
	myGD->CCN_V["Main_gameNodePointer"] = std::bind(&Maingame::gameNodePointer, this);
	myGD->V_V["Main_setUnlimitMap"] = std::bind(&Maingame::setUnlimitMap, this);
	myGD->V_F["Main_startShake"] = std::bind(&Maingame::startShake, this, _1);
	myGD->V_CCP["Main_startMoveToCCPoint"] = std::bind(&Maingame::startMoveToCCPoint, this, _1);
	myGD->V_I["Main_takeSpeedUpEffect"] = std::bind(&Maingame::takeSpeedUpEffect, this, _1);
	myGD->V_CCP["Main_showMissMissile"] = std::bind(&Maingame::showMissMissile, this, _1);
	myGD->V_CCPI["Main_showDamageMissile"] = std::bind(&Maingame::showDamageMissile, this, _1, _2);
	myGD->CCP_V["Main_getGameNodePosition"] = std::bind(&Maingame::getGameNodePosition, this);
	myGD->V_V["Main_hideThumb"] = std::bind(&Maingame::hideThumb, this);
	myGD->V_V["Main_showDrawButtonTutorial"] = std::bind(&Maingame::showDrawButtonTutorial, this);
	myGD->V_V["Main_hideDrawButtonTutorial"] = std::bind(&Maingame::hideDrawButtonTutorial, this);
	myGD->V_V["Main_showPause"] = std::bind(&Maingame::showPause, this);
	myGD->V_TDTD["Main_showContinue"] = std::bind(&Maingame::showContinue, this, _1, _2, _3, _4);
	
	mControl = NULL;
	is_line_die = false;
	
	game_node = CCNode::create();
	game_node->setScale((480.f-myGD->boarder_value*2)/(320.f));
	if(myGD->gamescreen_type == kGT_leftUI)			game_node->setPosition(ccp(50+myGD->boarder_value,0));
	else if(myGD->gamescreen_type == kGT_rightUI)	game_node->setPosition(ccp(myGD->boarder_value, 0));
	else											game_node->setPosition(ccp(myGD->boarder_value,0));
	addChild(game_node, myMSZorder);
	
	myGD->F_V["Main_getGameNodeScale"] = std::bind(&CCNode::getScale, game_node);
	
	myMS = MapScanner::create();
	game_node->addChild(myMS, myMSZorder);
	
	return true;
}

void Maingame::showDrawButtonTutorial()
{
	if(mControl)
		((ControlJoystickButton*)mControl)->showDrawButtonTutorial();
}

void Maingame::hideDrawButtonTutorial()
{
	if(mControl)
		((ControlJoystickButton*)mControl)->hideDrawButtonTutorial();
}

void Maingame::onEnterTransitionDidFinish()
{
	init_state = kMIS_movingGameNode;
	
	setTouchEnabled(true);
	
	gamenode_moving_direction = kGNMD_up;
	
	touch_img = CCSprite::create("touch_before_start.png");
	touch_img->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(touch_img, myPMZorder);
	
	CCFadeTo* fade1 = CCFadeTo::create(1.f, 0);
	CCFadeTo* fade2 = CCFadeTo::create(1.f, 255);
	CCDelayTime* delay1 = CCDelayTime::create(0.3f);
	CCRepeatForever* repeat1 = CCRepeatForever::create(CCSequence::create(fade1, fade2, delay1, NULL));
	
	touch_img->runAction(repeat1);
	
	schedule(schedule_selector(Maingame::movingGameNode));
}

void Maingame::movingGameNode()
{
	CCPoint after_position = ccpAdd(game_node->getPosition(), ccp(0,gamenode_moving_direction*4));
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	
	if(after_position.y < -430*game_node->getScale()+480*screen_size.height/screen_size.width)
	{
		after_position.y = -430*game_node->getScale()+480*screen_size.height/screen_size.width;
		gamenode_moving_direction = kGNMD_down;
	}
	else if(after_position.y > 0)
	{
		after_position.y = 0;
		gamenode_moving_direction = kGNMD_up;
	}
	game_node->setPosition(after_position);
}

void Maingame::randomingRectView()
{
	if(ignore_cnt < 10)
		ignore_cnt++;
	
	myMS->randomingRectView(game_node->getPosition());
	
	if(ignore_cnt >= 10 && ignore_cnt <= 15)
	{
		setTouchEnabled(true);
		ignore_cnt = 20;
	}
}

void Maingame::controlStunOff()
{
	mControl->isStun = false;
	((ControlJoystickButton*)mControl)->resetTouch();
}

void Maingame::finalSetting()
{
	init_state = kMIS_startGame;
	
//	myGD->initStartRect();
	
	myPM = PathManager::create();
	game_node->addChild(myPM, myPMZorder);
	
	myGIM = GameItemManager::create();
	game_node->addChild(myGIM, attackItemZorder);
	
	myCP = CumberParent::create();
	game_node->addChild(myCP, myCPZorder);
	
	myJack = Jack::create();
	game_node->addChild(myJack, myJackZorder);
	myJack->initStartPosition(game_node->getPosition());
	
	game_node->setScale(myGD->game_scale);
	
	myGD->is_setted_jack = true;
	
	myUI = PlayUI::create();
	addChild(myUI, myUIZorder);
	myUI->setMaxBossLife(mySD->getBossMaxLife());
	myUI->setClearPercentage(0.85f);
	
//	myCP->setUI_forEP(myUI, callfunc_selector(PlayUI::keepBossLife), callfunc_selector(PlayUI::checkBossLife));
	
	myMS->scanMap();
	myGD->communication("VS_setSceneNode", this);
	
//	myUI->setControlTD(this, callfunc_selector(Maingame::setControlGesture), callfunc_selector(Maingame::setControlButton), callfunc_selector(Maingame::setControlJoystick), callfunc_selector(Maingame::startControl));
	
	search_eye = SearchEye::create();
	search_eye->setPosition(CCPointZero);
	addChild(search_eye, searchEyeZorder);
	
	search_eye->startSearch();
	
	startScene();
	
	float thumb_scale = 0.17f;
	
	thumb_texture = CCRenderTexture::create(320, 430);
	thumb_texture->setScale(thumb_scale);
	thumb_texture->setPosition(ccp(58-160.f*thumb_scale,myDSH->ui_top-90-215.f*thumb_scale));
	addChild(thumb_texture, myUIZorder);
	
	thumb_base_position = ccp(58-320.f*thumb_scale,myDSH->ui_top-90-430.f*thumb_scale);
	
	CCDelayTime* thumb_delay = CCDelayTime::create(0.3f);
	CCCallFunc* thumb_call = CCCallFunc::create(this, callfunc_selector(Maingame::refreshThumb));
	CCSequence* thumb_seq = CCSequence::createWithTwoActions(thumb_delay, thumb_call);
	CCRepeatForever* thumb_repeat = CCRepeatForever::create(thumb_seq);
	thumb_texture->runAction(thumb_repeat);
	
	boss_thumb = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 3, 3));
	boss_thumb->setColor(ccRED);
	CCNode* boss_pointer = myGD->getCommunicationNode("CP_getMainCumberPointer");
	boss_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(boss_pointer->getPosition(), thumb_scale)));
	addChild(boss_thumb, myUIZorder);
	
	character_thumb = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 2, 2));
	character_thumb->setColor(ccGREEN);
	character_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(myJack->getPosition(), thumb_scale)));
	addChild(character_thumb, myUIZorder);
	
	sub_thumbs = new CCArray(1);
	
	if(mySGD->is_play_replay)
	{
		replay_all_node = CCNode::create();
		replay_all_node->setPosition(CCPointZero);
		addChild(replay_all_node, myUIZorder);
		
		if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() > 0)
		{
			replay_thumb_texture = CCRenderTexture::create(320, 430);
			replay_thumb_texture->setScale(thumb_scale);
			replay_thumb_texture->setPosition(ccp(480-(58-160.f*thumb_scale),myDSH->ui_top-90-215.f*thumb_scale));
			replay_all_node->addChild(replay_thumb_texture);
			
			myGD->V_I["Main_refreshReplayThumb"] = std::bind(&Maingame::refreshReplayThumb, this, _1);
			
			
			replay_boss = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 3, 3));
			replay_boss->setColor(ccRED);
			replay_boss->setVisible(false);
			replay_boss->setPosition(ccp(480-(58-160.f*thumb_scale),myDSH->ui_top-90-215.f*thumb_scale));
			replay_all_node->addChild(replay_boss);
			
			replay_character = CCSprite::create("whitePaper.png", CCRectMake(0,0,2,2));
			replay_character->setColor(ccGREEN);
			replay_character->setVisible(false);
			replay_character->setPosition(ccp(480-(58-160.f*thumb_scale),myDSH->ui_top-90-215.f*thumb_scale));
			replay_all_node->addChild(replay_character);
			
			replay_sub = new CCArray(1);
			
			myGD->V_I["Main_refreshReplayPosition"] = std::bind(&Maingame::refreshReplayPosition, this, _1);
		}
		
		if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_scoreTime)].size() > 0)
		{
			replay_score = CCLabelBMFont::create("0", "etc_font.fnt");
			replay_score->setScale(0.7f);
			replay_score->setPosition(ccp(480-(58-160.f*thumb_scale),myDSH->ui_top-90-215.f*thumb_scale - 215.f*thumb_scale+10));
			replay_all_node->addChild(replay_score);
			
			myGD->V_I["Main_refreshReplayScore"] = std::bind(&Maingame::refreshReplayScore, this, _1);
		}
		
		CCLabelTTF* replay_nick = CCLabelTTF::create(mySGD->getAcceptChallengeNick().c_str(), mySGD->getFont().c_str(), 10);
		replay_nick->setPosition(ccp(480-(58-160.f*thumb_scale),myDSH->ui_top-90-215.f*thumb_scale + 215.f*thumb_scale-10));
		replay_all_node->addChild(replay_nick);
	}
	
	CCArray* sub_array = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
	for(int i=0;i<sub_array->count();i++)
	{
		CCSprite* sub_position_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 2, 2));
		sub_position_img->setColor(ccYELLOW);
		CCNode* sub_pointer = (CCNode*)sub_array->objectAtIndex(i);
		sub_position_img->setPosition(ccpAdd(thumb_base_position, ccpMult(sub_pointer->getPosition(), thumb_scale)));
		addChild(sub_position_img, myUIZorder);
		
		sub_thumbs->addObject(sub_position_img);
	}
	
	if(mySD->getSilType() == 1)
	{
		if(!myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_control))
		{
			myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, kSpecialTutorialCode_control, true);
			CCNode* exit_target = this;
			exit_target->onExit();
			
			ASPopupView* t_popup = ASPopupView::create(-200);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
			
			ControlTutorialContent* t_container = ControlTutorialContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																				 {
																					 mControl->isStun = false;
																					 exit_target->onEnter();
																					 
																					 CCMoveBy* t_move = CCMoveBy::create(0.5f, ccp(-480,0));
																					 CCCallFunc* t_call = CCCallFunc::create(t_popup, callfunc_selector(CCNode::removeFromParent));
																					 CCSequence* t_seq = CCSequence::create(t_move, t_call, NULL);
																					 t_popup->runAction(t_seq);
																					 
//																					 t_popup->removeFromParent();
																				 });
			t_popup->setContainerNode(t_container);
			exit_target->getParent()->addChild(t_popup);
		}
	}
	else
	{
		Json::Reader reader;
		Json::Value root;
		reader.parse(mySDS->getStringForKey(kSDF_stageInfo, mySD->getSilType(), "boss"), root);
		Json::Value boss = root[0u];
		Json::Value patterns = boss["pattern"];
		
		vector<int> pattern_code;
		
		for(int i=0;i<patterns.size();i++)
		{
			int t_code = patterns[i]["pattern"].asInt();
			if(!myDSH->getBoolForKey(kDSH_Key_hasShowTutorial_int1, t_code))
				pattern_code.push_back(t_code);
		}
		
		if(pattern_code.size() > 0)
		{
			CCNode* exit_target = this;
			exit_target->onExit();
			
			ASPopupView* t_popup = ASPopupView::create(-200);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
			
			PatternTutorialContent* t_container = PatternTutorialContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																				 {
																					 mControl->isStun = false;
																					 
																					 for(int i=0;i<pattern_code.size();i++)
																						 myDSH->setBoolForKey(kDSH_Key_hasShowTutorial_int1, pattern_code[i], true);
																					 
																					 exit_target->onEnter();
																					 t_popup->removeFromParent();
																				 }, pattern_code);
			t_popup->setContainerNode(t_container);
			exit_target->getParent()->addChild(t_popup);
		}
	}
}

void Maingame::startScene()
{
	AudioEngine::sharedInstance()->playSound("sound_back_maingame.mp3", true);
	AudioEngine::sharedInstance()->preloadEffectScene("Maingame");
	
	setControlJoystickButton();
	
	startCounting();
}

void Maingame::checkAcceptChallenge()
{
	if(mySGD->getIsAcceptChallenge())
	{
		CCNode* exit_target = this;
		exit_target->onExit();
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		AcceptChallengeAniContent* t_container = AcceptChallengeAniContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																				   {
																					   checkFriendCard();
																					   mControl->isStun = false;
																					   exit_target->onEnter();
																					   t_popup->removeFromParent();
																				   }, hspConnector::get()->getKakaoProfileURL(), hspConnector::get()->getKakaoNickname(),
																				   mySGD->getAcceptChallengeProfile(), mySGD->getAcceptChallengeNick());
		t_popup->setContainerNode(t_container);
		exit_target->getParent()->addChild(t_popup);
	}
	else
	{
		checkFriendCard();
	}
}

void Maingame::checkFriendCard()
{
	if(mySGD->isUsingItem(kIC_rentCard) && mySGD->getSelectedFriendCardData().card_number != 0)
	{
		CCNode* exit_target = this;
		exit_target->onExit();
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		RentCardAniContent* t_container = RentCardAniContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
																			 {
																				 myUI->setUseFriendCard();
																				 mControl->isStun = false;
																				 exit_target->onEnter();
																				 t_popup->removeFromParent();
																			 });
		t_popup->setContainerNode(t_container);
		exit_target->getParent()->addChild(t_popup);
	}
}

void Maingame::startCounting()
{
	CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
	CCBReader* reader = new CCBReader(nodeLoader);
	condition_spr = dynamic_cast<CCLayer*>(reader->readNodeGraphFromFile("ui_ready.ccbi",this));
	addChild(condition_spr, conditionLabelZorder);
	reader->release();
	
	condition_spr->setPosition(ccp(240,myDSH->ui_center_y+myDSH->ui_top*0.1f));
	
	CCDelayTime* t_delay = CCDelayTime::create(0.1f);
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(Maingame::checkAcceptChallenge));
	CCDelayTime* t_delay1 = CCDelayTime::create(0.65f);
	CCCallFunc* t_ms_startGame = CCCallFunc::create(myMS, callfunc_selector(MapScanner::startGame));
	CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::removeConditionLabel));
	runAction(CCSequence::create(t_delay, t_call1, t_delay1, t_ms_startGame, t_delay2, t_call, NULL));
	
	countingCnt = 0;
	
	myJack->isStun = true;
	startControl();
}

void Maingame::counting()
{
	countingCnt++;
	
	if(countingCnt/60 >= 2)
	{
		setTag(1);
		myJack->isStun = false;
		condition_spr->removeFromParent();
		unschedule(schedule_selector(Maingame::counting));
//		setTouchEnabled(true);
//		myCP->movingMainCumber();
		myCP->onStartGame();
//		myCP->movingSubCumbers();
		myCP->startAutoAttacker();
		myUI->startCounting();
		myGD->setIsGameover(false);
	}
	else if(countingCnt/60 >= 1 && countingCnt%60 == 0)
	{
//		AudioEngine::sharedInstance()->playEffect("sound_go.mp3", false);
		if(mySGD->getGold() >= mySGD->getGachaMapFee())
		{
			StartMapGacha* t_smg = StartMapGacha::create(this, callfunc_selector(Maingame::gachaOn));
			addChild(t_smg, clearshowtimeZorder);
		}
	}
}

void Maingame::gachaOn()
{
	myLog->addLog(kLOG_gacha_startMap, -1);
	mySGD->setGold(mySGD->getGold() - mySGD->getGachaMapFee());
	myGD->resetGameData();
	mySGD->startMapGachaOn();
	
	int map_gacha_cnt = myDSH->getIntegerForKey(kDSH_Key_achieve_mapGachaCnt)+1;
	myDSH->setIntegerForKey(kDSH_Key_achieve_mapGachaCnt, map_gacha_cnt);
	
	AchieveConditionReward* shared_acr = AchieveConditionReward::sharedInstance();
	
	for(int i=kAchievementCode_mapGacha1;i<=kAchievementCode_mapGacha3;i++)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) == 0 &&
		   map_gacha_cnt >= shared_acr->getCondition((AchievementCode)i))
		{
			myDSH->setIntegerForKey(kDSH_Key_achieveData_int1_value, i, 1);
			AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
	}
	
	vector<SaveUserData_Key> save_userdata_list;
	
	save_userdata_list.push_back(kSaveUserData_Key_gold);
	save_userdata_list.push_back(kSaveUserData_Key_achieve);
	
	myDSH->saveUserData(save_userdata_list, nullptr);
	
	CCDirector::sharedDirector()->replaceScene(Maingame::scene());
}

bool Maingame::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(init_state == kMIS_movingGameNode)
	{
		unschedule(schedule_selector(Maingame::movingGameNode));
		setTouchEnabled(false);
		init_state = kMIS_randomRectView;
		ignore_cnt = 0;
		randomingRectView();
		schedule(schedule_selector(Maingame::randomingRectView), 1.f/30.f);
		
		setLimitMap();
	}
	else if(init_state == kMIS_randomRectView)
	{
		unschedule(schedule_selector(Maingame::randomingRectView));
		myMS->stopRandomingRectView();
		touch_img->removeFromParent();
		init_state = kMIS_startGame;
		setTouchEnabled(false);
		finalSetting();
	}
	
	return true;
}

void Maingame::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void Maingame::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void Maingame::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
	
}

void Maingame::backTracking()
{
	IntPoint afterJackPoint = myPM->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
		return;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}
	
	afterJackPoint = myPM->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
		return;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}
}

void Maingame::onEnter()
{
	CCLayer::onEnter();
}

void Maingame::onExit()
{
	touchEnd();
	((ControlJoystickButton*)mControl)->invisibleControl();
	CCLayer::onExit();
}

void Maingame::registerWithTouchDispatcher()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
}

void Maingame::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void Maingame::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(Maingame::alertAction));
}

void Maingame::touchEnd()
{
	myJack->isStun = true;
	mControl->isStun = true;
	//		mControl->setTouchEnabled(false);
//	((ControlJoystickButton*)mControl)->stopMySchedule();
	if(mControl->mType == kCT_Type_Joystick_button)
		myJack->setTouchPointByJoystick(CCPointZero, directionStop, true);
	myJack->changeDirection(directionStop, directionStop);
	if(myJack->getJackState() == jackStateDrawing)
	{
		stunBackTracking();
	}
}

void Maingame::touchOn()
{
//	((ControlJoystickButton*)mControl)->resetTouch();
	
	if(isCheckingBacking)
	{
		schedule(schedule_selector(Maingame::checkingBacking));
	}
	else
	{
		myJack->isStun = false;
		mControl->isStun = false;
		if(myJack->t_se)
		{
			myJack->t_se->selfRemove();
		}
		myJack->t_se = NULL;
		//			mControl->setTouchEnabled(true);
	}
}

void Maingame::setControlGesture()
{
	myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level1));
	myGD->changeJackBaseSpeed(1.2f);
}

void Maingame::setControlButton()
{
	myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level2));
	myGD->changeJackBaseSpeed(1.5f);
}

void Maingame::setControlJoystick()
{
	myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level3));
	myGD->changeJackBaseSpeed(2.f);
}

void Maingame::setControlJoystickButton()
{
	if(mControl)		mControl->removeFromParentAndCleanup(true);
	mControl = ControlJoystickButton::create(this, callfunc_selector(Maingame::readyBackTracking), myJack);
	((ControlJoystickButton*)mControl)->pauseBackTracking = callfunc_selector(Maingame::pauseBackTracking);
	addChild(mControl, mControlZorder);
}

void Maingame::startControl()
{
	mControl->startControl();
}

void Maingame::removeConditionLabel()
{
	//		AudioEngine::sharedInstance()->playEffect("sound_ready.mp3", false);
	AudioEngine::sharedInstance()->playEffect("sound_gamestart.mp3", false);

	myGIM->startItemSetting();
	myGIM->startCounting();

	schedule(schedule_selector(Maingame::counting));
}

void Maingame::checkingBacking()
{
	if(!isCheckingBacking)
	{
		unschedule(schedule_selector(Maingame::checkingBacking));
		myJack->isStun = false;
		mControl->isStun = false;
		if(myJack->t_se)
		{
			myJack->t_se->selfRemove();
		}
		myJack->t_se = NULL;
		//			mControl->setTouchEnabled(true);
	}
}

void Maingame::stunBackTracking()
{
	myJack->willBackTracking = true;
	if(!myJack->isMoving)
	{
		isCheckingBacking = true;
		AudioEngine::sharedInstance()->playEffect("sound_returntojack.mp3", false);
		schedule(schedule_selector(Maingame::stunBacking));
	}
}

void Maingame::stunBacking()
{
	IntPoint afterJackPoint = myPM->pathBackTracking();

	if(afterJackPoint.isNull())
	{
		unschedule(schedule_selector(Maingame::stunBacking));
		myJack->endBackTracking();
		myJack->willBackTracking = false;
		isCheckingBacking = false;
		return;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}

	afterJackPoint = myPM->pathBackTracking();

	if(afterJackPoint.isNull())
	{
		unschedule(schedule_selector(Maingame::stunBacking));
		myJack->endBackTracking();
		myJack->willBackTracking = false;
		isCheckingBacking = false;
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}
}

void Maingame::readyBackTracking()
{
	//		mControl->setTouchEnabled(false);
	((ControlJoystickButton*)mControl)->stopMySchedule();
	myJack->willBackTracking = true;
	if(!myJack->isMoving)
		startBackTracking();
}

void Maingame::startBackTracking()
{
	AudioEngine::sharedInstance()->playEffect("sound_returntojack.mp3", false);
	((ControlJoystickButton*)mControl)->isBacking = true;
	schedule(schedule_selector(Maingame::backTracking));
}

void Maingame::stopBackTracking()
{
	unschedule(schedule_selector(Maingame::backTracking));
	((ControlJoystickButton*)mControl)->isBacking = false;
	myJack->endBackTracking();
	myJack->willBackTracking = false;
	//		if(!mControl->isStun)
	//			mControl->setTouchEnabled(true);
}

void Maingame::pauseBackTracking()
{
	unschedule(schedule_selector(Maingame::backTracking));
	((ControlJoystickButton*)mControl)->isBacking = false;
	myJack->willBackTracking = false;
}

void Maingame::allStopSchedule()
{
	myJack->stopJack();
	// myMS : nothing schedule
	// myPM : nothing schedule
	myCP->allStopSchedule();
	stopBackTracking();
	mControl->setTouchEnabled(false);
}

void Maingame::startSpecialAttack()
{
	myJack->changeDirection(directionStop, directionStop);
	mControl->isStun = true;
	//		mControl->setTouchEnabled(false);
}

void Maingame::gameover()
{
	mySGD->resetLabels();

	int clear_info;
	if(!mySGD->getIsCleared())
	{
		mySGD->gameOver(myUI->getScore(), myUI->getPercentage(), myUI->getUseTime());
		clear_info = -1;
	}
	else
		clear_info = 1;

	myGD->communication("UI_writeGameOver", clear_info);
	
	mControl->setVisible(false);

	AudioEngine::sharedInstance()->stopSound();

	if(mySGD->getIsCleared())
	{
		AudioEngine::sharedInstance()->playEffect("sound_clear_bgm.mp3", false);
		AudioEngine::sharedInstance()->playEffect("sound_clear_ment.mp3", false);
		ClearShowTime* t_cst = ClearShowTime::create(myUI->getIsExchanged(), myUI->getPercentage() >= 1.f, game_node, this, callfunc_selector(Maingame::closeShutter));
		game_node->addChild(t_cst, clearshowtimeZorder);

		t_cst->startClearView();

		if(myUI->getPercentage() >= 1.f)
		{
			CCSprite* spin_light = CCSprite::create("showtime_spin_light.png");
			spin_light->setOpacity(0);
			spin_light->setPosition(ccp(240,myDSH->ui_center_y));
			addChild(spin_light, particleZorder);

			CCScaleTo* light_scale = CCScaleTo::create(1.2f, 2.f);//2.f/3.f, 2);
			CCRotateTo* light_rotate = CCRotateTo::create(1.2f, -90);//2.f/3.f, -90);

			CCFadeTo* light_fade1 = CCFadeTo::create(0.3f, 255);//1.f/6.f, 255);
			CCFadeTo* light_fade2 = CCFadeTo::create(0.3f, 20);//1.f/6.f, 20);
			CCFadeTo* light_fade3 = CCFadeTo::create(0.3f, 255);//1.f/6.f, 255);
			CCFadeTo* light_fade4 = CCFadeTo::create(0.3f, 0);//1.f/6.f, 0);
			CCCallFunc* light_call = CCCallFunc::create(spin_light, callfunc_selector(CCSprite::removeFromParent));
			CCSequence* light_seq = CCSequence::create(light_fade1, light_fade2, light_fade3, light_fade4, light_call, NULL);

			CCSpawn* light_spawn = CCSpawn::create(light_scale, light_rotate, light_seq, NULL);
			spin_light->runAction(light_spawn); // 1.3


			CCSprite* bakper = CCSprite::create("bakper.png");
			bakper->setOpacity(0);
			bakper->setPosition(ccp(240,myDSH->ui_center_y));
			addChild(bakper, particleZorder);

			CCScaleTo* bak_scale1 = CCScaleTo::create(32.4f/60.f, 1.3f);
			CCScaleTo* bak_scale2 = CCScaleTo::create(18.f/60.f, 0.9f);
			CCScaleTo* bak_scale3 = CCScaleTo::create(7.2f/60.f,1.2f);
			CCScaleTo* bak_scale4 = CCScaleTo::create(14.4f/60.f,0.f);
			CCSequence* bak_seq1 = CCSequence::create(bak_scale1, bak_scale2, bak_scale3, bak_scale4, NULL);

			CCFadeTo* bak_fade1 = CCFadeTo::create(32.4f/60.f, 255);
			CCDelayTime* bak_delay = CCDelayTime::create(25.2f/60.f);
			CCFadeTo* bak_fade2 = CCFadeTo::create(14.4f/60.f, 0);
			CCCallFunc* bak_call = CCCallFunc::create(bakper, callfunc_selector(CCSprite::removeFromParent));
			CCSequence* bak_seq2 = CCSequence::create(bak_fade1, bak_delay, bak_fade2, bak_call, NULL);

			CCSpawn* bak_spawn = CCSpawn::create(bak_seq1, bak_seq2, NULL);
			bakper->runAction(bak_spawn);
		}

		myUI->setVisible(false);
	}
	else
	{
		AudioEngine::sharedInstance()->playEffect("sound_gameover_bgm.mp3", false);
		AudioEngine::sharedInstance()->playEffect("sound_gameover_ment.mp3", false);

		closeShutter();
	}
}

void Maingame::closeShutter()
{
	endCloseShutter();
}

void Maingame::endCloseShutter()
{
	if(mySGD->getIsCleared())
	{
		AudioEngine::sharedInstance()->unloadEffectScene("Maingame");
		
		CCTransitionFadeTR* t_trans = CCTransitionFadeTR::create(1.f, ZoomScript::scene());
		CCDirector::sharedDirector()->replaceScene(t_trans);
	}
	else
	{
		AudioEngine::sharedInstance()->unloadEffectScene("Maingame");
		
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_fail);
//		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
		if(mySD->getSilType() >= 10000 || mySGD->getIsAcceptChallenge() || mySGD->getIsAcceptHelp())
			CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
		else
			CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
	}
}

void Maingame::startMoveToBossPosition()
{
	startMoveToCCPoint(myGD->getMainCumberPoint().convertToCCP());
}

void Maingame::startMoveToCCPoint( CCPoint t_point )
{
	move_to_boss_position_frame = 0;
	CCPoint after_position = getObjectToGameNodePosition(t_point);
	CCPoint sub_position = ccpSub(after_position, game_node->getPosition());
	CCLog("boss : %.2f\t recent : %.2f", after_position.y, game_node->getPositionY());
	move_to_boss_position_ds = ccpMult(sub_position, 1.f/30.f);
	schedule(schedule_selector(Maingame::moveToBossPosition));
}

void Maingame::moveToBossPosition()
{
	move_to_boss_position_frame++;

	CCPoint after_position = ccpAdd(game_node->getPosition(), move_to_boss_position_ds);
	game_node->setPosition(after_position);
	myGD->communication("VS_setMoveGamePosition", getGameNodeToObjectPosition(after_position));

	if(move_to_boss_position_frame >= 30)
	{
		unschedule(schedule_selector(Maingame::moveToBossPosition));
	}
}

CCPoint Maingame::getObjectToGameNodePosition( CCPoint t_p )
{
	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float y_value = -t_p.y*myGD->game_scale+480.f*frame_size.height/frame_size.width/2.f;
	
	if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
	{
		if(y_value > 60)																		y_value = 60;
		else if(y_value < -490*myGD->game_scale+480*frame_size.height/frame_size.width)			y_value = -490*myGD->game_scale+480*frame_size.height/frame_size.width;
	}
	
	float x_value = -t_p.x*myGD->game_scale+480.f/2.f;
	if(!myDSH->getBoolForKey(kDSH_Key_isAlwaysCenterCharacter))
	{
		if(x_value > myGD->boarder_value+50)														x_value = myGD->boarder_value+50;
		else if(x_value < -320*myGD->game_scale-myGD->boarder_value+480.f-50)						x_value = -320*myGD->game_scale-myGD->boarder_value+480.f-50;
	}

	CCPoint after_position;

//	if(myGD->gamescreen_type == kGT_full)					after_position = ccp(myGD->boarder_value,y_value);
//	else if(myGD->gamescreen_type == kGT_leftUI)			after_position = ccp(50+myGD->boarder_value,y_value);
//	else if(myGD->gamescreen_type == kGT_rightUI)			after_position = ccp(myGD->boarder_value,y_value);
	
	after_position = ccp(x_value, y_value);

	return after_position;
}

CCPoint Maingame::getObjectToGameNodePositionCoin( CCPoint t_p )
{
	float scale_value = NSDS_GD(mySD->getSilType(), kSDS_SI_scale_d);
	if(scale_value < 0.1f)
		scale_value = 1.f;
	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float x_value = t_p.x/320.f*(720.f*scale_value-myGD->boarder_value*2.f);
	float y_value = t_p.y/320.f*(720.f*scale_value-myGD->boarder_value*2.f);

	x_value = x_value+game_node->getPositionX();
	y_value = y_value+game_node->getPositionY();

	return ccp(x_value, y_value);
}

CCPoint Maingame::getGameNodeToObjectPosition( CCPoint t_p )
{
	CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float y_value = -(t_p.y - 480.f*frame_size.height/frame_size.width/2.f)/myGD->game_scale;
	float x_value = -(t_p.x - 480.f/2.f)/myGD->game_scale;
	
//	if(y_value > 60)																		y_value = 60;
//	else if(y_value < -490*myGD->game_scale+480*frame_size.height/frame_size.width)			y_value = -490*myGD->game_scale+480*frame_size.height/frame_size.width;
//	if(x_value > myGD->boarder_value)														x_value = myGD->boarder_value;
//	else if(x_value < -320*myGD->game_scale-myGD->boarder_value+480.f)						x_value = -320*myGD->game_scale-myGD->boarder_value+480.f;
	
	CCPoint after_position;
	after_position = ccp(x_value,y_value);

	return after_position;
}

void Maingame::moveGamePosition( CCPoint t_p )
{
	//		if(!myGD->is_setted_jack)// || myGD->game_step == kGS_unlimited)
	game_node->setPosition(getObjectToGameNodePosition(t_p));
	if(character_thumb)
		character_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(myJack->getPosition(), thumb_texture->getScale())));
}

CCPoint Maingame::getGameNodePosition()
{
	return game_node->getPosition();
}

void Maingame::goldGettingEffect( CCPoint t_p, int t_i )
{
	GetGold* t_gg = GetGold::create(t_p, t_i);
	game_node->addChild(t_gg, goldZorder);
	t_gg->startMyAction();
}

void Maingame::percentageGettingEffect( float t_f, bool t_b, CCPoint t_p )
{
	GetPercentage* t_gp = GetPercentage::create(t_f, t_b);
	
	CCPoint add_point;
	if(t_p.x < 30.f)
		add_point = ccp(30.f, 0.f);
	else if(t_p.x > 290.f)
		add_point = ccp(-30.f, 0.f);
	else
	{
		if(t_p.y > 400.f)
			add_point = ccp(0, -20.f);
		else
			add_point = ccp(0, 20.f);
	}
	
	t_gp->setPosition(ccpAdd(t_p, add_point));
	game_node->addChild(t_gp, goldZorder);
}

void Maingame::takeSpeedUpEffect( int t_step )
{
	CCPoint jack_position = myGD->getJackPoint().convertToCCP();

	CCPoint add_point;
	if(jack_position.x < 30.f)
		add_point = ccp(30.f, 20.f);
	else if(jack_position.x > 290.f)
		add_point = ccp(-30.f, 20.f);
	else
	{
		if(jack_position.y > 400.f)
			add_point = ccp(30.f, -20.f);
		else
			add_point = ccp(30.f, 20.f);
	}
	
	TakeSpeedUp* t_tsu = TakeSpeedUp::create(t_step);
	t_tsu->setScale(1.f/myGD->game_scale);
	t_tsu->setPosition(ccpAdd(jack_position, add_point));
	game_node->addChild(t_tsu, goldZorder);
}

void Maingame::showMissMissile( CCPoint t_position )
{
	t_position.x += rand()%21 - 10;
	t_position.y += rand()%21 - 10;

	CCSprite* miss_label = CCSprite::create("missile_miss.png");
	miss_label->setScale(1.f/myGD->game_scale);
	miss_label->setPosition(t_position);
	game_node->addChild(miss_label, goldZorder);

	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call = CCCallFunc::create(miss_label, callfunc_selector(CCSprite::removeFromParent));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);

	miss_label->runAction(t_seq);
}

void Maingame::showDamageMissile( CCPoint t_position, int t_damage )
{
	CCNode* container = CCNode::create();
	container->setScale(1.f/myGD->game_scale);
	container->setPosition(t_position);
	game_node->addChild(container, goldZorder);
	
	CountingBMLabel* damage_label = CountingBMLabel::create("1", "missile_damage_label.fnt", 0.3f, "%d");
	container->addChild(damage_label, goldZorder);
	
	damage_label->setString(CCString::createWithFormat("%d", t_damage)->getCString());
	
	CCDelayTime* t_delay = CCDelayTime::create(0.5f);
	CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
	CCCallFunc* t_call = CCCallFunc::create(container, callfunc_selector(CCNode::removeFromParent));
	CCSequence* t_seq = CCSequence::create(t_delay, t_fade, t_call, NULL);
	damage_label->runAction(t_seq);
	
//	MissileDamageLabel* damage_label = MissileDamageLabel::create(t_damage);
//	damage_label->setScale(1.f/1.5f);
//	damage_label->setPosition(t_position);
//	game_node->addChild(damage_label, goldZorder);
//
//	damage_label->startMyAction();
}

void Maingame::showLineDiePosition( IntPoint t_p )
{
	if(!is_line_die)
	{
		is_line_die = true;
		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("line_die.png");
		CCSprite* line_die = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 80, 90));
		line_die->setPosition(ccp((t_p.x-1.f)*pixelSize+1.f, (t_p.y-1.f)*pixelSize+1.f));
		game_node->addChild(line_die, goldZorder);

		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=0;i<5;i++)
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*80, 0, 80, 90));
		CCAnimate* t_animate = CCAnimate::create(t_animation);

		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
		CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(Maingame::removeLineDieEffect), line_die);

		CCAction* t_action = CCSequence::create(t_animate, t_fade, t_call, NULL);

		line_die->runAction(t_action);
	}
}

void Maingame::removeLineDieEffect( CCObject* t_obj )
{
	game_node->removeChild((CCNode*)t_obj, true);
}

void Maingame::resetIsLineDie()
{
	is_line_die = false;
}

void Maingame::showWarning( int t1 )
{
	Warning* t_w = Warning::create(t1);
	addChild(t_w, goldZorder);
	t_w->startAction();
}

void Maingame::showDetailMessage(const std::string& fileName)
{
	DetailWarning* w = DetailWarning::create(fileName);
	addChild(w, goldZorder);
}
void Maingame::showTextMessage(const std::string& text)
{
	CCLabelTTF* msg = CCLabelTTF::create(text.c_str(), "", 28.f);
	addChild(msg, goldZorder);
	addChild(KSGradualValue<float>::create(600, 240, 0.7f, [=](float t)
				{
					msg->setPosition(ccp(t, myDSH->ui_center_y));
				},
				[=](float t)
				{
					addChild(KSGradualValue<float>::create(255, 0, 2.f, [=](float t)
							{
								msg->setOpacity(t);
							},
							[=](float t)
							{
								msg->removeFromParent();
							}));
				}));
}
void Maingame::showTakeCoin()
{
	TakeCoin* t_w = TakeCoin::create();
	addChild(t_w, goldZorder);
	t_w->startAction();
}

CCNode* Maingame::gameNodePointer()
{
	return game_node;
}

void Maingame::showChangeCard()
{
	ChangeCard* t_w = ChangeCard::create();
	addChild(t_w, goldZorder);
	t_w->startAction();
}

void Maingame::showCoin()
{
	myGIM->showCoin(this, callfuncCCpI_selector(Maingame::takeExchangeCoin));
}

void Maingame::takeExchangeCoin( CCPoint t_start_position, int t_coin_number )
{
	myUI->takeExchangeCoin(getObjectToGameNodePositionCoin(t_start_position), t_coin_number);
}

void Maingame::startExchange()
{
	mySD->exchangeSilhouette();
	myMS->exchangeMS();
	
	myGD->communication("UI_writeImageChange");
}

void Maingame::setLimitMap()
{
	myGD->game_step = kGS_limited;
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();

	float screen_to_map_center_y = (-game_node->getPositionY() + 240.f*screen_size.height/screen_size.width)/game_node->getScale();

	int screen_to_map_top = floorf((screen_to_map_center_y + 240.f*screen_size.height/screen_size.width/game_node->getScale())/2.f);
	int screen_to_map_bottom = ceilf((screen_to_map_center_y - 240.f*screen_size.height/screen_size.width/game_node->getScale())/2.f);

	myGD->limited_step_top = screen_to_map_top;
	myGD->limited_step_bottom = screen_to_map_bottom;

	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(j < screen_to_map_bottom || j > screen_to_map_top)
		{
			for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
			{
				myGD->mapState[i][j] = mapOutline;
			}
		}
	}

	myGD->communication("MS_setTopBottomBlock");
}

void Maingame::setUnlimitMap()
{
	myGD->game_step = kGS_changing;

	showAreaScroll();

	limitted_map_position = game_node->getPosition();
	myGD->communication("VS_setLimittedMapPosition");
	myGD->communication("MS_startRemoveBlock");

	for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
	{
		if(j < myGD->limited_step_bottom || j > myGD->limited_step_top)
		{
			for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
			{
				if(myGD->mapState[i][j] == mapOutline)
					myGD->mapState[i][j] = mapEmpty;
			}
		}
	}
	changing_game_step_frame = 0;
	schedule(schedule_selector(Maingame::changingGameStep));
}

void Maingame::changingGameStep()
{
	changing_game_step_frame++;
	gameNodeChangingGameStep(limitted_map_position, changing_game_step_frame);
	myGD->communication("VS_changingGameStep", changing_game_step_frame);

	if(changing_game_step_frame >= 15)
	{
		unschedule(schedule_selector(Maingame::changingGameStep));
		myGD->game_step = kGS_unlimited;
	}
}

void Maingame::gameNodeChangingGameStep( CCPoint t_p, int t_step )
{
	IntPoint jack_point = myGD->getJackPoint();
	CCPoint jack_position = jack_point.convertToCCP();
	jack_position = getObjectToGameNodePosition(jack_position);

	CCPoint after_position = ccpAdd(limitted_map_position, ccpMult(ccpSub(jack_position, t_p), t_step/15.f));
	game_node->setPosition(after_position);
}

void Maingame::showAreaScroll()
{
	AreaScroll* t_w = AreaScroll::create();
	addChild(t_w, goldZorder);
	t_w->startAction();
}

void Maingame::startShake( float t_direction_angle )
{
	stopAllActions();

	CCPoint change_position = ccp(cosf(t_direction_angle/180.f*M_PI), sinf(t_direction_angle/180.f*M_PI));
	change_position = ccpMult(change_position, 3.f/sqrtf(powf(change_position.x, 2.f) + powf(change_position.y, 2.f)));

	setPosition(ccpAdd(getPosition(), change_position));
	CCDelayTime* t_delay = CCDelayTime::create(0.1f);
	CCMoveTo* t_move = CCMoveTo::create(0.2f, CCPointZero);
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_move);
	runAction(t_seq);

	//		shake_frame = 0;
	//		schedule(schedule_selector(Maingame::shaking), 1.f/20.f);
}

void Maingame::shaking()
{
	shake_frame++;

	if(shake_frame == 1)
	{
		setRotation(-1.5f);
		setPosition(ccp(-1,-2));
	}
	else if(shake_frame == 2)
	{
		setRotation(1.f);
		setPosition(ccp(1,0));
	}
	else if(shake_frame == 3)
	{
		setRotation(-1.2f);
		setPosition(ccp(-2,1));
	}
	else if(shake_frame == 4)
	{
		setRotation(0.7f);
		setPosition(ccp(1,-1));
	}
	else if(shake_frame == 5)
	{
		setRotation(0);
		setPosition(CCPointZero);

		stopShake();
	}
}

void Maingame::stopShake()
{
	unschedule(schedule_selector(Maingame::shaking));
	shake_frame = 0;
}

void Maingame::refreshThumb()
{
	VisibleSprite* t_vs = (VisibleSprite*)myMS->getVisibleSprite();
	thumb_texture->beginWithClear(0, 0, 0.3f, 0.5f);
	t_vs->visitForThumb();
	thumb_texture->end();
	
	CCNode* boss_pointer = myGD->getCommunicationNode("CP_getMainCumberPointer");
	boss_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(boss_pointer->getPosition(), thumb_texture->getScale())));
	
	CCArray* sub_array = myGD->getCommunicationArray("CP_getSubCumberArrayPointer");
	while(sub_thumbs->count() > sub_array->count())
	{
		CCNode* sub_position_img = (CCNode*)sub_thumbs->lastObject();
		sub_thumbs->removeObject(sub_position_img);
		sub_position_img->removeFromParent();
	}
	while (sub_thumbs->count() < sub_array->count())
	{
		CCSprite* sub_position_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 2, 2));
		sub_position_img->setColor(ccYELLOW);
		addChild(sub_position_img, myUIZorder);
		
		sub_thumbs->addObject(sub_position_img);
	}
	
	for(int i=0;i<sub_array->count();i++)
	{
		CCNode* sub_position_img = (CCNode*)sub_thumbs->objectAtIndex(i);
		CCNode* sub_pointer = (CCNode*)sub_array->objectAtIndex(i);
		sub_position_img->setPosition(ccpAdd(thumb_base_position, ccpMult(sub_pointer->getPosition(), thumb_texture->getScale())));//thumb_scale)));
	}
	
	t_vs->visit();
}

void Maingame::refreshReplayThumb(int temp_time)
{
	int play_index = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_playIndex)].asInt();
	if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() <= play_index || mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)][play_index].asInt() > temp_time)
		return;
	
	VisibleSprite* t_vs = (VisibleSprite*)myMS->getVisibleSprite();
	replay_thumb_texture->beginWithClear(0, 0.3f, 0, 0.5f);
	t_vs->replayVisitForThumb(temp_time);
	replay_thumb_texture->end();
	
	t_vs->visit();
}

void Maingame::refreshReplayScore(int temp_time)
{
	int score_index = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_timeStamp)][temp_time][mySGD->getReplayKey(kReplayKey_timeStamp_scoreIndex)].asInt();
	if(score_index <= 0)
		return;
	if(mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_scoreTime)][score_index].asInt() > temp_time)
		return;
	
	replay_score->setString(CCString::createWithFormat("%d", mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_scoreData)][score_index].asInt())->getCString());
}

void Maingame::refreshReplayPosition(int temp_time)
{
	if(mySGD->replay_write_info[mySGD->getReplayKey(kReplayKey_timeStamp)].size() <= temp_time)
		return;
	
	CCPoint replay_base_position = ccpAdd(replay_thumb_texture->getPosition(), ccp(-160.f*replay_thumb_texture->getScale(), -215.f*replay_thumb_texture->getScale()));
	
	Json::Value position_data = mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_timeStamp)][temp_time];
	
	CCPoint character_position;
	character_position.x = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_characterPositionX)].asFloat();
	character_position.y = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_characterPositionY)].asFloat();
	replay_character->setPosition(ccpAdd(replay_base_position, ccpMult(character_position, replay_thumb_texture->getScale())));
	replay_character->setVisible(true);
	
	CCPoint boss_position;
	boss_position.x = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_bossPositionX)].asFloat();
	boss_position.y = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_bossPositionY)].asFloat();
	replay_boss->setPosition(ccpAdd(replay_base_position, ccpMult(boss_position, replay_thumb_texture->getScale())));
	replay_boss->setVisible(true);
	

	int monster_count = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_monster)].size();
	
	while(replay_sub->count() > monster_count)
	{
		CCNode* sub_position_img = (CCNode*)replay_sub->lastObject();
		replay_sub->removeObject(sub_position_img);
		sub_position_img->removeFromParent();
	}
	while(replay_sub->count() < monster_count)
	{
		CCSprite* sub_position_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 2, 2));
		sub_position_img->setColor(ccYELLOW);
		replay_all_node->addChild(sub_position_img);
		
		replay_sub->addObject(sub_position_img);
	}
	
	for(int i=0;i<replay_sub->count();i++)
	{
		CCNode* sub_position_img = (CCNode*)replay_sub->objectAtIndex(i);
		CCPoint cumber_position;
		cumber_position.x = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_monster)][i][mySGD->getReplayKey(kReplayKey_timeStamp_monster_x)].asFloat();
		cumber_position.y = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_monster)][i][mySGD->getReplayKey(kReplayKey_timeStamp_monster_y)].asFloat();
		sub_position_img->setPosition(ccpAdd(replay_base_position, ccpMult(cumber_position, replay_thumb_texture->getScale())));
	}
	
	if(position_data[mySGD->getReplayKey(kReplayKey_timeStamp_isDie)].asBool())
	{
		CCLabelTTF* die_label = CCLabelTTF::create("Life -1", mySGD->getFont().c_str(), 10);
		die_label->setScale(0.5f);
		die_label->setColor(ccRED);
		die_label->setPosition(replay_character->getPosition());
		replay_all_node->addChild(die_label);
		
		CCScaleTo* t_scale = CCScaleTo::create(1.5f, 5.f);
		CCFadeTo* t_fade = CCFadeTo::create(1.5f, 0);
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_fade);
		CCCallFunc* t_call = CCCallFunc::create(die_label, callfunc_selector(CCLabelTTF::removeFromParent));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
		die_label->runAction(t_seq);
	}
	
	if(position_data[mySGD->getReplayKey(kReplayKey_timeStamp_isImageChange)].asBool())
	{
		CCLabelTTF* change_label = CCLabelTTF::create("체인지", mySGD->getFont().c_str(), 14);
		change_label->setColor(ccYELLOW);
		change_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale()+10)));
		replay_all_node->addChild(change_label);
	}
	
	int game_info = position_data[mySGD->getReplayKey(kReplayKey_timeStamp_gameInfo)].asInt();
	if(game_info == 1)
	{
		CCLabelTTF* clear_label = CCLabelTTF::create("클리어", mySGD->getFont().c_str(), 14);
		clear_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale()-10)));
		replay_all_node->addChild(clear_label);
		
		replay_character->setVisible(false);
		replay_boss->setVisible(false);
		
		int replay_sub_loop = replay_sub->count();
		for(int i=0;i<replay_sub_loop;i++)
		{
			((CCSprite*)replay_sub->objectAtIndex(i))->setVisible(false);
		}
	}
	else if(game_info == -1)
	{
		CCLabelTTF* game_over_label = CCLabelTTF::create("게임오버", mySGD->getFont().c_str(), 12);
		game_over_label->setColor(ccBLACK);
		game_over_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale()-10)));
		replay_all_node->addChild(game_over_label);
		
		replay_character->setVisible(false);
		replay_boss->setVisible(false);
		
		int replay_sub_loop = replay_sub->count();
		for(int i=0;i<replay_sub_loop;i++)
		{
			((CCSprite*)replay_sub->objectAtIndex(i))->setVisible(false);
		}
	}
	
	if(position_data[mySGD->getReplayKey(kReplayKey_timeStamp_isContinue)].asBool())
	{
		replay_continue_count++;
		if(!replay_continue_label)
		{
			replay_continue_label = CCLabelTTF::create("이어하기 : 1", mySGD->getFont().c_str(), 8);
			replay_continue_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), -5)));
			replay_all_node->addChild(replay_continue_label);
		}
		else
			replay_continue_label->setString(CCString::createWithFormat("이어하기 : %d", replay_continue_count)->getCString());
		
		CCLabelTTF* continue_label = CCLabelTTF::create("이어하기!!", mySGD->getFont().c_str(), 10);
		continue_label->setScale(0.5f);
		continue_label->setColor(ccMAGENTA);
		continue_label->setPosition(ccpAdd(replay_base_position, ccp(160.f*replay_thumb_texture->getScale(), 215.f*replay_thumb_texture->getScale())));
		replay_all_node->addChild(continue_label);
		
		CCScaleTo* t_scale = CCScaleTo::create(1.5f, 5.f);
		CCFadeTo* t_fade = CCFadeTo::create(1.5f, 0);
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_scale, t_fade);
		CCCallFunc* t_call = CCCallFunc::create(continue_label, callfunc_selector(CCLabelTTF::removeFromParent));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
		continue_label->runAction(t_seq);
	}
}

void Maingame::hideThumb()
{
	search_eye->setVisible(false);
	thumb_texture->setVisible(false);
	character_thumb->setVisible(false);
	boss_thumb->setVisible(false);
	
	int sub_loop = sub_thumbs->count();
	for(int i=0;i<sub_loop;i++)
	{
		CCSprite* t_sub = (CCSprite*)sub_thumbs->objectAtIndex(i);
		t_sub->setVisible(false);
	}
	
	if(mySGD->is_play_replay && mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_mapTime)].size() > 0)
	{
		replay_all_node->setVisible(false);
	}
}

void Maingame::showPause()
{
	bool t_jack_stun = myJack->isStun;
	
	CCNode* exit_target = this;
	exit_target->onExit();
	
	ASPopupView* t_popup = ASPopupView::create(-200);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
	t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
	
	PauseContent* t_container = PauseContent::create(t_popup->getTouchPriority(), [=]()
	{
		mControl->isStun = false;
		myJack->isStun = t_jack_stun;
		exit_target->onEnter();
		t_popup->removeFromParent();
		cancelHome();
	}, [=]()
	{
		mControl->isStun = false;
		myJack->isStun = t_jack_stun;
		exit_target->onEnter();
		t_popup->removeFromParent();
		goHome();
	}, [=]()
	{
		mControl->isStun = false;
		myJack->isStun = t_jack_stun;
		exit_target->onEnter();
		t_popup->removeFromParent();
		goReplay();
	});
	
	t_popup->setContainerNode(t_container);
	exit_target->getParent()->addChild(t_popup);
	t_container->startShow();
}
void Maingame::goHome ()
{
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	
	myLog->sendLog(CCString::createWithFormat("home_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
	AudioEngine::sharedInstance()->stopSound();
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_fail);
	
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
	gameover();
}
void Maingame::goReplay ()
{
	myDSH->setIntegerForKey(kDSH_Key_achieve_seqNoFailCnt, 0);
	myLog->addLog(kLOG_getCoin_i, -1, mySGD->getStageGold());
	
	myLog->sendLog(CCString::createWithFormat("replay_%d", myDSH->getIntegerForKey(kDSH_Key_lastSelectedStage))->getCString());
	AudioEngine::sharedInstance()->stopAllEffects();
	AudioEngine::sharedInstance()->stopSound();
	
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
	mySGD->gameOver(0, 0, 0);
	mySGD->resetLabels();
	myGD->resetGameData();
	
	myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stageSetting);
	CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
}
void Maingame::cancelHome ()
{
	startControl();
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
}
void Maingame::showContinue(CCObject * t_end, SEL_CallFunc d_end, CCObject * t_continue, SEL_CallFunc d_continue)
{
	bool t_jack_stun = myJack->isStun;
	
	CCNode* exit_target = this;
	exit_target->onExit();
	
	ASPopupView* t_popup = ASPopupView::create(-200);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
	t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
	
	ContinueContent* t_container = ContinueContent::create(t_popup->getTouchPriority(), [=]()
														   {
															   t_popup->removeFromParent();
															   mControl->isStun = false;
															   myJack->isStun = t_jack_stun;
															   exit_target->onEnter();
															   mySGD->is_paused = false;
															   AudioEngine::sharedInstance()->setAppFore();
															   (t_end->*d_end)();
														   }, [=]()
														   {
															   t_popup->removeFromParent();
															   mControl->isStun = false;
															   myJack->isStun = t_jack_stun;
															   exit_target->onEnter();
															   (t_continue->*d_continue)();
															   continueAction();
														   });
	
	t_popup->setContainerNode(t_container);
	exit_target->getParent()->addChild(t_popup);
	t_container->startShow();
}
void Maingame::continueAction()
{
	myLog->addLog(kLOG_action_continue, -1);
	
	mySGD->setStar(mySGD->getStar() - mySGD->getPlayContinueFee());
	myDSH->saveUserData({kSaveUserData_Key_star}, nullptr);
	
	startControl();
	mySGD->is_paused = false;
	AudioEngine::sharedInstance()->setAppFore();
}