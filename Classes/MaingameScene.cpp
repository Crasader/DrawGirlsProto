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
	mControl = NULL;
	is_line_die = false;
	
	game_node = CCNode::create();
	game_node->setScale(myGD->game_scale);
	if(myGD->gamescreen_type == kGT_leftUI)			game_node->setPosition(ccp(50+myGD->boarder_value,0));
	else if(myGD->gamescreen_type == kGT_rightUI)	game_node->setPosition(ccp(myGD->boarder_value, 0));
	else											game_node->setPosition(ccp(0,0));
	addChild(game_node, myMSZorder);
	
	myMS = MapScanner::create();
	game_node->addChild(myMS, myMSZorder);
	
	return true;
}

void Maingame::onEnter()
{
	init_state = kMIS_movingGameNode;
	
	setTouchEnabled(true);
	
	CCLayer::onEnter();
	
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
	
	myUI = PlayUI::create();
	addChild(myUI, myUIZorder);
	myUI->setMaxBossLife(mySD->getBossMaxLife());//SelectedMapData::sharedInstance()->getMaxBossLife());
	myUI->setClearPercentage(SelectedMapData::sharedInstance()->getClearPercentage());
	
//	myCP->setUI_forEP(myUI, callfunc_selector(PlayUI::keepBossLife), callfunc_selector(PlayUI::checkBossLife));
	
	myMS->scanMap();
	
	myUI->setControlTD(this, callfunc_selector(Maingame::setControlGesture), callfunc_selector(Maingame::setControlButton), callfunc_selector(Maingame::setControlJoystick), callfunc_selector(Maingame::startControl));
	
	startScene();
}

void Maingame::startScene()
{
	AudioEngine::sharedInstance()->playSound("sound_back_maingame.mp3", true);
	AudioEngine::sharedInstance()->preloadEffectScene("Maingame");
	
	setControlJoystickButton();
	
	startCounting();
}

void Maingame::startCounting()
{
	CCTexture2D* t_texture;
//	if(SelectedMapData::sharedInstance()->getIsNoShield())
//	{
		t_texture = CCTextureCache::sharedTextureCache()->addImage("hard_condition.png");
		condition_spr = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 105, 117));
		addChild(condition_spr, conditionLabelZorder);
//	}
//	else
//	{
//		t_texture = CCTextureCache::sharedTextureCache()->addImage("easy_condition.png");
//		condition_spr = CCSprite::createWithTexture(t_texture, CCRectMake(0, 0, 105, 117));
//		addChild(condition_spr, conditionLabelZorder);
//	}
	condition_spr->setPosition(ccp(240,myDSH->ui_center_y+50));
	
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
//		myCP->movingMainCumber();
		myCP->onStartGame();
//		myCP->movingSubCumbers();
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
		countingLabel->setPosition(ccp(240,myDSH->ui_center_y));
		addChild(countingLabel, countingLabelZorder);
		
		StartMapGacha* t_smg = StartMapGacha::create(this, callfunc_selector(Maingame::gachaOn));
		addChild(t_smg, mControlZorder);
	}
}

void Maingame::gachaOn()
{
	myGD->resetGameData();
	mySGD->startMapGachaOn();
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

void Maingame::onExit()
{
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




