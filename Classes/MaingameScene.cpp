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
	
//	TempAd* temp_ad = TempAd::create();
//	addChild(temp_ad, 9999);
	
//	resetTouchBeganPoint();
	
//	isStun = false;
	isCheckingBacking = false;
	
	myGD = GameData::sharedGameData();
	myDSH = DataStorageHub::sharedInstance();
	
//	device_rate = myDSH->getDeviceRate();
	
//	myGD->regMain(this, callfunc_selector(Maingame::startBackTracking),
//				  callfunc_selector(Maingame::allStopSchedule),
//				  callfunc_selector(Maingame::gameover),
//				  callfunc_selector(Maingame::touchEnd),
//				  callfunc_selector(Maingame::touchOn),
//				  callfunc_selector(Maingame::stunBackTracking),
//				  callfunc_selector(Maingame::startSpecialAttack),
//				  callfunc_selector(Maingame::stopSpecialAttack),
//				  callfuncCCp_selector(Maingame::moveGamePosition),
//				  callfuncCCpI_selector(Maingame::goldGettingEffect),
//				  callfuncFBCCp_selector(Maingame::percentageGettingEffect),
//				  callfuncIp_selector(Maingame::showLineDiePosition),
//				  callfunc_selector(Maingame::resetIsLineDie),
//				  callfuncI_selector(Maingame::showWarning),
//				  callfunc_selector(Maingame::showCoin),
//				  callfunc_selector(Maingame::startExchange),
//				  callfunc_selector(Maingame::showTakeCoin),
//				  callfunc_selector(Maingame::showChangeCard));
	
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
	
	
	myGD->initStartRect();

	
	game_node = CCNode::create();
	game_node->setScale(1.5f);
	addChild(game_node, myMSZorder);
	
	myMS = MapScanner::create();
	game_node->addChild(myMS, myMSZorder);
	
	myPM = PathManager::create();
	game_node->addChild(myPM, myPMZorder);
	
//	int missile_type = myDSH->getIntegerForKey(kDSH_Key_lastSelectedElement);
//	if(missile_type == kMyElementalWind)
//	{
//		WindItem* t_wi = WindItem::create();
//		game_node->addChild(t_wi, attackItemZorder);
//		t_wi->startCast();
//	}
//	else if(missile_type == kMyElementalLightning)
//	{
//		LightningItem* t_li = LightningItem::create();
//		game_node->addChild(t_li, attackItemZorder);
//		t_li->startCast();
//	}
//	else if(missile_type == kMyElementalLife)
//	{
//		LifeItem* t_li = LifeItem::create();
//		game_node->addChild(t_li, attackItemZorder);
//		t_li->startCast();
//	}
//	else if(missile_type == kMyElementalWater)
//	{
//		IceItem* t_ii = IceItem::create();
//		game_node->addChild(t_ii, attackItemZorder);
//		t_ii->startCast();
//	}
//	else if(missile_type == kMyElementalFire)
//	{
//		FireItem* t_fi = FireItem::create();
//		game_node->addChild(t_fi, attackItemZorder);
//		t_fi->startCast();
//	}
//	else if(missile_type == kMyElementalPlasma)
//	{
//		PlasmaItem* t_pi = PlasmaItem::create();
//		game_node->addChild(t_pi, attackItemZorder);
//		t_pi->startCast();
//	}
//	
//	FastItem* fast_i = FastItem::create();
//	game_node->addChild(fast_i, attackItemZorder);
//	fast_i->startCast();
	
	myGIM = GameItemManager::create();
	game_node->addChild(myGIM, attackItemZorder);
	
	myCP = CumberParent::create();
	game_node->addChild(myCP, myCPZorder);
	
	myJack = Jack::create();
	game_node->addChild(myJack, myJackZorder);
	
	myUI = PlayUI::create();
	addChild(myUI, myUIZorder);
	myUI->setMaxBossLife(SelectedMapData::sharedInstance()->getMaxBossLife());
	myUI->setClearPercentage(SelectedMapData::sharedInstance()->getClearPercentage());
	
	myCP->setUI_forEP(myUI, callfunc_selector(PlayUI::keepBossLife), callfunc_selector(PlayUI::checkBossLife));
	
	myMS->scanMap();
	
	mControl = NULL;
	
	is_line_die = false;
	
	myUI->setControlTD(this, callfunc_selector(Maingame::setControlGesture), callfunc_selector(Maingame::setControlButton), callfunc_selector(Maingame::setControlJoystick), callfunc_selector(Maingame::startControl));
	
//	setScale(MY_SCALE_RATE);
	
//	ScreenSide* t_screen = ScreenSide::create();
//	addChild(t_screen, 99999);

	return true;
}

void Maingame::onEnter()
{
//	setTouchEnabled(false);
	CCLayer::onEnter();
	
//	AudioEngine::sharedInstance()->playEffect("sound_shuttermove_start.m4a", false);
	
	
//	top_shutter = CCSprite::create("loading_top.png");
//	top_shutter->setAnchorPoint(ccp(0.5,0));
//	top_shutter->setPosition(ccp(160,240));
//	addChild(top_shutter, shutterZorder);
//	
//	CCMoveTo* bottom_move = CCMoveTo::create(0.5f, ccp(160,-10));
//	CCMoveTo* top_move = CCMoveTo::create(0.5f, ccp(160,490));
//	CCCallFunc* top_call = CCCallFunc::create(this, callfunc_selector(Maingame::startScene));
//	CCSequence* top_seq = CCSequence::createWithTwoActions(top_move, top_call);
//	
//	bottom_shutter->runAction(bottom_move);
//	top_shutter->runAction(top_seq);
	
	startScene();
}

void Maingame::startScene()
{
//	bottom_shutter->removeFromParentAndCleanup(true);
//	top_shutter->removeFromParentAndCleanup(true);
	
	AudioEngine::sharedInstance()->playSound("sound_back_maingame.mp3", true);
	AudioEngine::sharedInstance()->preloadEffectScene("Maingame");
	
//	ControlType recent_type = ControlType(myDSH->getIntegerForKey(kDSH_Key_lastSelectedControler));
//	if(recent_type == kControlType_unsetted)
//	{
//		recent_type = kControlType_joystick_button;
//		myDSH->setIntegerForKey(kDSH_Key_lastSelectedControler, kControlType_joystick_button);
//		myDSH->setIntegerForKey(kDSH_Key_controlOriginX, 280);
//		myDSH->setIntegerForKey(kDSH_Key_controlOriginY, 160);
//		myUI->showPause();
//	}
//	else if(!myDSH->getBoolForKey(kDSH_Key_checkedNewControlJoystick))
//	{
//		myUI->showPause();
//	}
//	
//	if(recent_type == kControlType_gesture)
//	{
//		setControlGesture();
//	}
//	else if(recent_type == kControlType_button)
//	{
//		setControlButton();
//	}
//	else if(recent_type == kControlType_joystick_button)
//	{
		setControlJoystickButton();
//	}
	
	startCounting();
}

void Maingame::startCounting()
{
	CCTexture2D* t_texture;
	if(SelectedMapData::sharedInstance()->getIsNoShield())
	{
		t_texture = CCTextureCache::sharedTextureCache()->addImage("hard_condition.png");
		condition_spr = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 105, 117));
		addChild(condition_spr, conditionLabelZorder);
	}
	else
	{
		t_texture = CCTextureCache::sharedTextureCache()->addImage("easy_condition.png");
		condition_spr = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 105, 117));
		addChild(condition_spr, conditionLabelZorder);
	}
	condition_spr->setPosition(ccp(240,220+DataStorageHub::sharedInstance()->ui_height_center_control));
	
	CCAnimation* t_animation = CCAnimation::create();
	t_animation->setDelayPerUnit(0.15);
	for(int i=0;i<3;i++)
		t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*105, 0, 105, 117));
	for(int i=0;i<2;i++)
		t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(i*105, 117, 105, 117));
	
	CCAnimate* t_animate = CCAnimate::create(t_animation);
	CCFadeOut* t_fo = CCFadeOut::create(1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Maingame::removeConditionLabel));
	condition_spr->runAction(CCSequence::create(t_animate, t_fo, t_call, NULL));
	
	countingCnt = 0;
	keepTexture = CCSprite::create("maingame_ui_start_count.png");
	keepTexture->setPosition(ccp(-500,-500));
	addChild(keepTexture);
	countingLabel = NULL;
}

void Maingame::counting()
{
	countingCnt++;
	
	if(countingLabel)
	{
		countingLabel->setScale(0.5 + countingCnt%60*0.015);
		countingLabel->setOpacity(255 - countingCnt%60*3);
	}
	
	if(countingCnt/60 >= 2)
	{
		setTag(1);
		unschedule(schedule_selector(Maingame::counting));
//		setTouchEnabled(true);
		startControl();
		myCP->movingMainCumber();
		myCP->movingSubCumbers();
		myCP->startAutoAttacker();
		myUI->startCounting();
		myGD->setIsGameover(false);
		countingLabel->removeFromParentAndCleanup(true);
		keepTexture->removeFromParentAndCleanup(true);
	}
	else if(countingCnt/60 >= 1 && countingCnt%60 == 0)
	{
//		AudioEngine::sharedInstance()->playEffect("sound_go.mp3", false);
		countingLabel->removeFromParentAndCleanup(true);
		countingLabel = CCSprite::createWithTexture(keepTexture->getTexture(), CCRectMake(0, 80, 200, 80));
		countingLabel->setScale(0.5);
		countingLabel->setPosition(ccp(240,160+DataStorageHub::sharedInstance()->ui_height_center_control));
		addChild(countingLabel, countingLabelZorder);
	}
}

//#define minimumDistance	30.f
//bool Maingame::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
//{
//	CCPoint location = pTouch->getLocationInView();
//	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);
//	
//	if(!myJack->willBackTracking &&
//	   convertedLocation.y < 430 &&
//	   convertedLocation.y > 0 &&
//	   convertedLocation.x > 0 &&
//	   convertedLocation.x < 320)
//	{
//		touch_began_point = convertedLocation;
//	}
//	
//	return true;
//}
//
//void Maingame::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
//{
//	if(!isSetTouchBeganPoint())
//		return;
//	
//	CCPoint location = pTouch->getLocationInView();
//	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);
//	
//	// calc direction
//	CCSize distanceSize = CCSizeMake(convertedLocation.x - touch_began_point.x, convertedLocation.y - touch_began_point.y);
//	float distanceValue = sqrt(pow(distanceSize.width, 2.0) + pow(distanceSize.height, 2.0));
//	float angle = atan2(distanceSize.height, distanceSize.width)/M_PI*180.0; // -180 ~ 180
//	
//	if(distanceValue > minimumDistance/device_rate*((is_gestured || (myJack->isDrawingOn && myJack->getRecentDirection() == directionStop)) ? 2 : 1.3))
//	{
//		IntDirection angleDirection;
//		IntDirection secondDirection;
//		
//		IntPoint jackPoint = myGD->getJackPoint();
//		
//		if(angle < -135.f)
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < -90.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else if(angle < -45.f)
//		{
//			angleDirection = directionDown;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 0.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerBegin)				secondDirection = directionUp;
//			else												secondDirection = directionDown;
//		}
//		else if(angle < 45.f)
//		{
//			angleDirection = directionRight;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		else if(angle < 90.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerEnd-1)				secondDirection = directionLeft;
//			else												secondDirection = directionRight;
//		}
//		else if(angle < 135.f)
//		{
//			angleDirection = directionUp;
//			
//			if(jackPoint.x == mapWidthInnerBegin)				secondDirection = directionRight;
//			else												secondDirection = directionLeft;
//		}
//		else
//		{
//			angleDirection = directionLeft;
//			
//			if(jackPoint.y == mapHeightInnerEnd-1)				secondDirection = directionDown;
//			else												secondDirection = directionUp;
//		}
//		
//		myJack->changeDirection(angleDirection, secondDirection);
//		touch_began_point = convertedLocation;
//		if(angleDirection != beforeGesture)
//		{
//			AudioEngine::sharedInstance()->playEffect("sound_jack_drawing.mp3", false);
//			beforeGesture = angleDirection;
//			if(is_gestured)
//				gesture_cnt++;
//		}
//		is_gestured = true;
////		resetTouchBeganPoint();
//	}
//}

//void Maingame::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
//{
//	if(!isSetTouchBeganPoint())
//		return;
//	
//	if(!isStun)
//	{
//		if(!is_gestured)
//		{
//			if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
//			{
//				if(myJack->getRecentDirection() == directionStop)
//				{
//					readyBackTracking();
//				}
//				else
//				{
//					myJack->changeDirection(directionStop, directionStop);
//				}
//			}
//			else
//			{
//				myJack->changeDirection(directionStop, directionStop);
//			}
//		}
//		else if(gesture_cnt > 0)
//		{
//			myJack->changeDirection(directionStop, directionStop);
//		}
//		
//	}
//	resetTouchBeganPoint();
//}
//
//void Maingame::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
//{
//	if(!isSetTouchBeganPoint())
//		return;
//	
//	if(!isStun)
//	{
//		if(!is_gestured)
//		{
//			if(myJack->isDrawingOn && myJack->getJackState() == jackStateDrawing)
//			{
//				if(myJack->getRecentDirection() == directionStop)
//				{
//					readyBackTracking();
//				}
//				else
//				{
//					myJack->changeDirection(directionStop, directionStop);
//				}
//			}
//			else
//			{
//				myJack->changeDirection(directionStop, directionStop);
//			}
//		}
//		else if(gesture_cnt > 0)
//		{
//			myJack->changeDirection(directionStop, directionStop);
//		}
//	}
//	resetTouchBeganPoint();
//}

void Maingame::backTracking()
{
	IntPoint afterJackPoint = myPM->pathBackTracking();
	
	if(afterJackPoint.isNull())
	{
		stopBackTracking();
	}
	else
	{
		myJack->backTrackingAtAfterMoving(afterJackPoint);
	}
}

void Maingame::onExit()
{
	AudioEngine::sharedInstance()->unloadEffectScene("Maingame");
	CCLayer::onExit();
}

//void Maingame::registerWithTouchDispatcher()
//{
//	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -110, true);
//}

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




