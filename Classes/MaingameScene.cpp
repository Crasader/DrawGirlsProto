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
#include "PausePopupLayer.h"
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
	
	myUI->setControlTD(this, callfunc_selector(Maingame::setControlGesture), callfunc_selector(Maingame::setControlButton), callfunc_selector(Maingame::setControlJoystick), callfunc_selector(Maingame::startControl));
	
	SearchEye* search_eye = SearchEye::create();
	search_eye->setPosition(CCPointZero);
	addChild(search_eye, searchEyeZorder);
	
	search_eye->startSearch();
	
	startScene();
	
	thumb_texture = CCRenderTexture::create(320, 430);
	thumb_texture->setScale(0.2f);
	thumb_texture->setPosition(ccp(65-160.f*0.2f,140-215.f*0.2f));
	addChild(thumb_texture, myUIZorder);
	
	thumb_base_position = ccp(65-320.f*0.2f,140-430.f*0.2f);
	
	CCDelayTime* thumb_delay = CCDelayTime::create(0.3f);
	CCCallFunc* thumb_call = CCCallFunc::create(this, callfunc_selector(Maingame::refreshThumb));
	CCSequence* thumb_seq = CCSequence::createWithTwoActions(thumb_delay, thumb_call);
	CCRepeatForever* thumb_repeat = CCRepeatForever::create(thumb_seq);
	thumb_texture->runAction(thumb_repeat);
	
	character_thumb = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 2, 2));
	character_thumb->setColor(ccGREEN);
	character_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(myJack->getPosition(), 0.2f)));
	addChild(character_thumb, myUIZorder);
	
	boss_thumb = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 3, 3));
	boss_thumb->setColor(ccRED);
	CCNode* boss_pointer = myGD->getCommunicationNode("CP_getMainCumberPointer");
	boss_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(boss_pointer->getPosition(), 0.2f)));
	addChild(boss_thumb, myUIZorder);
	
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
																					 t_popup->removeFromParent();
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
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(Maingame::checkFriendCard));
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
	AudioEngine::sharedInstance()->unloadEffectScene("Maingame");
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

	if(!mySGD->getIsCleared())
		mySGD->gameOver(myUI->getScore(), myUI->getPercentage(), myUI->getUseTime());

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
		CCTransitionFadeTR* t_trans = CCTransitionFadeTR::create(1.f, ZoomScript::scene());
		CCDirector::sharedDirector()->replaceScene(t_trans);
	}
	else
	{
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_fail);
//		CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
		if(mySD->getSilType() >= 10000)
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
	if(y_value > 60)																		y_value = 60;
	else if(y_value < -490*myGD->game_scale+480*frame_size.height/frame_size.width)			y_value = -490*myGD->game_scale+480*frame_size.height/frame_size.width;
	
	float x_value = -t_p.x*myGD->game_scale+480.f/2.f;
	if(x_value > myGD->boarder_value)														x_value = myGD->boarder_value;
	else if(x_value < -320*myGD->game_scale-myGD->boarder_value+480.f)						x_value = -320*myGD->game_scale-myGD->boarder_value+480.f;

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
		character_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(myJack->getPosition(), 0.2f)));
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
	boss_thumb->setPosition(ccpAdd(thumb_base_position, ccpMult(boss_pointer->getPosition(), 0.2f)));
	
	t_vs->visit();
}