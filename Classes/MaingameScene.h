//
//  MaingameScene.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#ifndef DrawingJack_MaingameScene_h
#define DrawingJack_MaingameScene_h

#include "cocos2d.h"
#include "GameData.h"
#include "Jack.h"
#include "MapScanner.h"
#include "PathManager.h"
#include "Cumber.h"
#include "PlayUI.h"
#include "SelectedMapData.h"
#include "AttackItem.h"
#include "ClearScene.h"
#include "FailScene.h"
#include "ShowTimeScene.h"
#include "ControlType.h"
#include "ClearShowTime.h"
#include "GameItemManager.h"
//#include "ZoomScriptScene.h"

using namespace cocos2d;

enum Mainzorder{
	myMSZorder = 1,
	myPMZorder,
	attackItemZorder,
	myCPZorder,
	myJackZorder,
	controlImgZorder,
	countingLabelZorder,
	conditionLabelZorder,
	goldZorder,
	clearshowtimeZorder,
	myUIZorder,
	mControlZorder,
	particleZorder,
	shutterZorder
};

class Maingame : public CCLayer
{
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(Maingame);
	
	void touchEnd()
	{
		myJack->isStun = true;
		mControl->isStun = true;
		mControl->setTouchEnabled(false);
		if(mControl->mType == kCT_Type_Joystick_button)
			myJack->setTouchPointByJoystick(CCPointZero, directionStop, true);
		myJack->changeDirection(directionStop, directionStop);
		if(myJack->getJackState() == jackStateDrawing)
		{
			stunBackTracking();
		}
	}
	
	void touchOn()
	{
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
			mControl->setTouchEnabled(true);
		}
	}
	
private:
	
	PlayUI* myUI;
	
	CCNode* game_node;
	
	Jack* myJack;
	MapScanner* myMS;
	PathManager* myPM;
	CumberParent* myCP;
	GameData* myGD;
	DataStorageHub* myDSH;
	CCSprite* keepTexture;
	CCSprite* countingLabel;
	CCSprite* condition_spr;
	
	GameItemManager* myGIM;
	
	CCSprite* bottom_shutter;
	CCSprite* top_shutter;
	
	ControlCommon* mControl;
	
	void startScene();
	
	void setControlGesture()
	{
		myGD->changeJackBaseSpeed(1.2f);
		
//		if(mControl)		mControl->removeFromParentAndCleanup(true);
//		mControl = ControlOriginalGesture::create(this, callfunc_selector(Maingame::readyBackTracking), myJack);
//		addChild(mControl, mControlZorder);
	}
	
	void setControlButton()
	{
		myGD->changeJackBaseSpeed(1.5f);
		
//		if(mControl)		mControl->removeFromParentAndCleanup(true);
//		mControl = ControlButtonSide::create(this, callfunc_selector(Maingame::readyBackTracking), myJack);
//		addChild(mControl, mControlZorder);
	}
	
	void setControlJoystick()
	{
		myGD->changeJackBaseSpeed(2.f);
		
//		if(mControl)		mControl->removeFromParentAndCleanup(true);
//		mControl = ControlJoystick::create(this, callfunc_selector(Maingame::readyBackTracking), myJack);
//		addChild(mControl, mControlZorder);
	}
	
	void setControlJoystickButton()
	{
		if(mControl)		mControl->removeFromParentAndCleanup(true);
		mControl = ControlJoystickButton::create(this, callfunc_selector(Maingame::readyBackTracking), myJack);
		addChild(mControl, mControlZorder);
	}
	
	void startControl()
	{
		mControl->startControl();
	}
	
	void removeConditionLabel()
	{
		condition_spr->removeFromParentAndCleanup(true);
//		AudioEngine::sharedInstance()->playEffect("sound_ready.mp3", false);
		AudioEngine::sharedInstance()->playEffect("sound_gamestart.mp3", false);
		countingLabel = CCSprite::createWithTexture(keepTexture->getTexture(), CCRectMake(0, 0, 200, 80));
		countingLabel->setScale(0.5);
		countingLabel->setPosition(ccp(240,160+DataStorageHub::sharedInstance()->ui_height_center_control));
		addChild(countingLabel, countingLabelZorder);
		
		myGIM->startItemSetting();
		myGIM->startCounting();
		
		schedule(schedule_selector(Maingame::counting));
	}
	
	int countingCnt;
	
//	float device_rate;
	
	void startCounting();
	void counting();
	
	virtual void onEnter();
    virtual void onExit();
	
//	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
//	
//	virtual void registerWithTouchDispatcher(void);
	
	virtual void keyBackClicked();
	void alertAction(int t1, int t2);
	
	bool isCheckingBacking;
//	bool isStun;
//	
//	CCPoint touch_began_point;
//	bool is_gestured;
//	int gesture_cnt;
//	IntDirection beforeGesture;
//	
//	void resetTouchBeganPoint()
//	{
//		touch_began_point = ccp(-1.f, -1.f);
//		is_gestured = false;
//		gesture_cnt = 0;
//		beforeGesture = directionStop;
//	}
//	
//	bool isSetTouchBeganPoint()
//	{
//		if(touch_began_point.x < 0 || touch_began_point.y < 0)
//			return false;
//		else
//			return true;
//	}
	
	void checkingBacking()
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
			mControl->setTouchEnabled(true);
		}
	}
	
	void stunBackTracking()
	{
		myJack->willBackTracking = true;
		if(!myJack->isMoving)
		{
			isCheckingBacking = true;
			AudioEngine::sharedInstance()->playEffect("sound_returntojack.mp3", false);
			schedule(schedule_selector(Maingame::stunBacking));
		}
	}
	void stunBacking()
	{
		IntPoint afterJackPoint = myPM->pathBackTracking();
		
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
	
	void readyBackTracking()
	{
		mControl->setTouchEnabled(false);
		myJack->willBackTracking = true;
		if(!myJack->isMoving)
			startBackTracking();
	}
	void startBackTracking()
	{
		AudioEngine::sharedInstance()->playEffect("sound_returntojack.mp3", false);
		schedule(schedule_selector(Maingame::backTracking));
	}
	void stopBackTracking()
	{
		unschedule(schedule_selector(Maingame::backTracking));
		myJack->endBackTracking();
		myJack->willBackTracking = false;
		if(!mControl->isStun)
			mControl->setTouchEnabled(true);
	}
	void backTracking();
	
	void allStopSchedule()
	{
		myJack->stopJack();
		// myMS : nothing schedule
		// myPM : nothing schedule
		myCP->allStopSchedule();
		stopBackTracking();
		mControl->setTouchEnabled(false);
	}
	
	void startSpecialAttack()
	{
		myJack->changeDirection(directionStop, directionStop);
		mControl->setTouchEnabled(false);
	}
	
	void stopSpecialAttack()
	{
		mControl->setTouchEnabled(true);
	}
	
	void gameover()
	{
		StarGoldData* mySGD = StarGoldData::sharedInstance();
		mySGD->resetLabels();
		
		if(!mySGD->getIsCleared())
			mySGD->gameOver(myUI->getScore(), myUI->getPercentage(), myUI->getGameTime());
		
		mControl->setVisible(false);
		
		AudioEngine::sharedInstance()->stopSound();
		
		if(mySGD->getIsCleared())
		{
			AudioEngine::sharedInstance()->playEffect("sound_clear_bgm.mp3", false);
			AudioEngine::sharedInstance()->playEffect("sound_clear_ment.mp3", false);
			ClearShowTime* t_cst = ClearShowTime::create(myUI->getIsExchanged(), myUI->getPercentage() >= 0.9f, game_node, this, callfunc_selector(Maingame::closeShutter));
			game_node->addChild(t_cst, clearshowtimeZorder);
			
			t_cst->startClearView();
			
			if(myUI->getPercentage() >= 1.f)
			{
				CCSprite* spin_light = CCSprite::create("showtime_spin_light.png");
				spin_light->setOpacity(0);
				spin_light->setPosition(ccp(240,160));
				addChild(spin_light, particleZorder);
				
				CCFadeTo* t_fade1 = CCFadeTo::create(0.5f, 255);
				CCRotateBy* t_rotate1 = CCRotateBy::create(0.5f, -100);
				CCSpawn* t_spawn1 = CCSpawn::createWithTwoActions(t_fade1, t_rotate1);
				
				CCDelayTime* t_delay1 = CCDelayTime::create(0.3f);
				CCRotateBy* t_rotate2 = CCRotateBy::create(0.3f, -60);
				CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_delay1, t_rotate2);
				
				CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 0);
				CCRotateBy* t_rotate3 = CCRotateBy::create(0.5f, -100);
				CCSpawn* t_spawn3 = CCSpawn::createWithTwoActions(t_fade2, t_rotate3);
				
				CCAction* t_seq1 = CCSequence::create(t_spawn1, t_spawn2, t_spawn3, NULL);
				spin_light->runAction(t_seq1);
				
				
				CCSprite* bakper = CCSprite::create("bakper.png");
				bakper->setScale(0);
				bakper->setOpacity(0);
				bakper->setPosition(ccp(240,160));
				addChild(bakper, particleZorder);
				
				CCFadeTo* t_fade3 = CCFadeTo::create(0.5f, 255);
				CCScaleTo* t_scale1 = CCScaleTo::create(0.5f, 1.f);
				CCSpawn* t_spawn4 = CCSpawn::createWithTwoActions(t_fade3, t_scale1);
				
				CCDelayTime* t_delay2 = CCDelayTime::create(0.3f);
				
				CCFadeTo* t_fade4 = CCFadeTo::create(0.5f, 0);
				CCScaleTo* t_scale2 = CCScaleTo::create(0.5f, 0.f);
				CCSpawn* t_spawn5 = CCSpawn::createWithTwoActions(t_fade4, t_scale2);
				
				CCAction* t_seq2 = CCSequence::create(t_spawn4, t_delay2, t_spawn5, NULL);
				bakper->runAction(t_seq2);
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
	
	void closeShutter()
	{
//		AudioEngine::sharedInstance()->playEffect("sound_shuttermove_start.m4a", false);
		
//		bottom_shutter = CCSprite::create("loading_bottom.png");
//		bottom_shutter->setAnchorPoint(ccp(0.5,1));
//		bottom_shutter->setPosition(ccp(160,-10));
//		addChild(bottom_shutter, shutterZorder);
		
//		top_shutter = CCSprite::create("loading_top.png");
//		top_shutter->setAnchorPoint(ccp(0.5,0));
//		top_shutter->setPosition(ccp(160,490));
//		addChild(top_shutter, shutterZorder);
		
//		CCMoveTo* bottom_move = CCMoveTo::create(0.5f, ccp(160,240));
//		CCMoveTo* top_move = CCMoveTo::create(0.5f, ccp(160,240));
//		CCCallFunc* top_sound = CCCallFunc::create(this, callfunc_selector(Maingame::shutterClosedSound));
//		CCDelayTime* top_delay = CCDelayTime::create(0.1f);
//		CCCallFunc* top_call = CCCallFunc::create(this, callfunc_selector(Maingame::endCloseShutter));
//		CCAction* top_seq = CCSequence::create(top_move, top_sound, top_delay, top_call, NULL);
//		
//		bottom_shutter->runAction(bottom_move);
//		top_shutter->runAction(top_seq);
		
		endCloseShutter();
	}
	
	void shutterClosedSound()
	{
//		AudioEngine::sharedInstance()->playEffect("sound_shutter_closed.m4a", false);
	}
	
	void endCloseShutter()
	{
//		bottom_shutter->removeFromParentAndCleanup(true);
//		top_shutter->removeFromParentAndCleanup(true);
		CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
		pEGLView->setDesignResolutionSize(480, 320, kResolutionNoBorder);
		
		if(StarGoldData::sharedInstance()->getIsCleared())
			CCDirector::sharedDirector()->replaceScene(ClearScene::scene());
		else
			CCDirector::sharedDirector()->replaceScene(FailScene::scene());
	}
	
//	void changeJackSpeed()
//	{
////		int randValue = rand()%3;
////		if(randValue == 0)			myJack->changeSpeed(jackSpeedSlow);
////		else if(randValue == 1)		myJack->changeSpeed(jackSpeedNormal);
////		else						myJack->changeSpeed(jackSpeedFast);
//	}
	
	void moveGamePosition(CCPoint t_p)
	{
//		game_node->setPosition(ccp((280-t_p.x)*1.25f-70.f,(160-t_p.y)*1.25f-43.f));
		
		if(t_p.y < 70+DataStorageHub::sharedInstance()->bottom_base/1.5f-DataStorageHub::sharedInstance()->ui_jack_center_control*1.5f/2.f)
			t_p.y = 70+DataStorageHub::sharedInstance()->bottom_base/1.5f-DataStorageHub::sharedInstance()->ui_jack_center_control*1.5f/2.f;
		else if(t_p.y > 430-65+DataStorageHub::sharedInstance()->upper_limit-DataStorageHub::sharedInstance()->bottom_base/1.5f-DataStorageHub::sharedInstance()->ui_jack_center_control*1.5f/2.f)//430-65
			t_p.y = 430-65+DataStorageHub::sharedInstance()->upper_limit-DataStorageHub::sharedInstance()->bottom_base/1.5f-DataStorageHub::sharedInstance()->ui_jack_center_control*1.5f/2.f;
		game_node->setPosition(ccp((199-160)*1.5f-70.f/1.5f*1.25f,(160-t_p.y)*1.5f-73.f+DataStorageHub::sharedInstance()->bottom_base-DataStorageHub::sharedInstance()->ui_jack_center_control));
	}
	
	void goldGettingEffect(CCPoint t_p, int t_i)
	{
		GetGold* t_gg = GetGold::create(t_p, t_i);
		game_node->addChild(t_gg, goldZorder);
		t_gg->startMyAction();
	}
	
	void percentageGettingEffect(float t_f, bool t_b, CCPoint t_p)
	{
		GetPercentage* t_gp = GetPercentage::create(t_f, t_b);
		t_gp->setPosition(ccpAdd(t_p, ccp(0,30)));
		game_node->addChild(t_gp, goldZorder);
	}
	
	bool is_line_die;
	
	void showLineDiePosition(IntPoint t_p)
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
	
	void removeLineDieEffect(CCObject* t_obj)
	{
		game_node->removeChild((CCNode*)t_obj, true);
	}
	
	void resetIsLineDie()
	{
		is_line_die = false;
	}
	
	void showWarning(int t1)
	{
		Warning* t_w = Warning::create(t1);
		addChild(t_w, goldZorder);
		t_w->startAction();
	}
	
	void showTakeCoin()
	{
		TakeCoin* t_w = TakeCoin::create();
		addChild(t_w, goldZorder);
		t_w->startAction();
	}
	
	void showChangeCard()
	{
		ChangeCard* t_w = ChangeCard::create();
		addChild(t_w, goldZorder);
		t_w->startAction();
	}
	
	void showCoin()
	{
		myGIM->showCoin(myUI, callfuncI_selector(PlayUI::takeExchangeCoin));
	}
	
	void startExchange()
	{
		SilhouetteData::sharedSilhouetteData()->exchangeSilhouette();
		myMS->exchangeMS();
	}
	
};

#endif
