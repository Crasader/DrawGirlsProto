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
#include "AttackItem.h"
#include "FailScene.h"
#include "ControlType.h"
#include "ClearShowTime.h"
#include "GameItemManager.h"
#include "ZoomScriptScene.h"
#include "MissileDamageLabel.h"

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
	searchEyeZorder,
	clearshowtimeZorder,
	myUIZorder,
	mControlZorder,
	particleZorder,
	shutterZorder
};

enum MainInitState{
	kMIS_beforeInit = 1,
	kMIS_movingGameNode,
	kMIS_randomRectView,
	kMIS_startGame
};

enum GameNodeMovingDirection{
	kGNMD_up = -1,
	kGNMD_down = 1
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
//		mControl->setTouchEnabled(false);
		((ControlJoystickButton*)mControl)->stopMySchedule();
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
//			mControl->setTouchEnabled(true);
		}
	}
	
private:
	
	PlayUI* myUI;
	
	CCNode* game_node;
	
	Jack* myJack;
	MapScanner* myMS;
	PathManager* myPM;
	CumberParent* myCP;
	
	CCLayer* condition_spr;
	CCSprite* touch_img;
	
	GameItemManager* myGIM;
	
	CCSprite* bottom_shutter;
	CCSprite* top_shutter;
	
	ControlCommon* mControl;
	
	MainInitState init_state;
	GameNodeMovingDirection gamenode_moving_direction;
	
	void movingGameNode();
	int ignore_cnt;
	void randomingRectView();
	void finalSetting();
	void startScene();
	
	void setControlGesture()
	{
		myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level1));
		myGD->changeJackBaseSpeed(1.2f);
	}
	
	void setControlButton()
	{
		myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level2));
		myGD->changeJackBaseSpeed(1.5f);
	}
	
	void setControlJoystick()
	{
		myDSH->setIntegerForKey(kDSH_Key_jackBaseSpeed, int(JackBaseSpeedTag::kJackBaseSpeedTag_level3));
		myGD->changeJackBaseSpeed(2.f);
	}
	
	void setControlJoystickButton()
	{
		if(mControl)		mControl->removeFromParentAndCleanup(true);
		mControl = ControlJoystickButton::create(this, callfunc_selector(Maingame::readyBackTracking), myJack);
		((ControlJoystickButton*)mControl)->pauseBackTracking = callfunc_selector(Maingame::pauseBackTracking);
		addChild(mControl, mControlZorder);
	}
	
	void startControl()
	{
		mControl->startControl();
	}
	
	void removeConditionLabel()
	{
//		AudioEngine::sharedInstance()->playEffect("sound_ready.mp3", false);
		AudioEngine::sharedInstance()->playEffect("sound_gamestart.mp3", false);
		
		myGIM->startItemSetting();
		myGIM->startCounting();
		
		schedule(schedule_selector(Maingame::counting));
	}
	
	int countingCnt;
	
	void startCounting();
	void counting();
	
	void gachaOn();
	
	virtual void onEnter();
    virtual void onExit();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
	
	virtual void keyBackClicked();
	void alertAction(int t1, int t2);
	
	bool isCheckingBacking;
	
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
//			mControl->setTouchEnabled(true);
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
	
	void readyBackTracking()
	{
//		mControl->setTouchEnabled(false);
		((ControlJoystickButton*)mControl)->stopMySchedule();
		myJack->willBackTracking = true;
		if(!myJack->isMoving)
			startBackTracking();
	}
	void startBackTracking()
	{
		AudioEngine::sharedInstance()->playEffect("sound_returntojack.mp3", false);
		((ControlJoystickButton*)mControl)->isBacking = true;
		schedule(schedule_selector(Maingame::backTracking));
	}
	void stopBackTracking()
	{
		unschedule(schedule_selector(Maingame::backTracking));
		((ControlJoystickButton*)mControl)->isBacking = false;
		myJack->endBackTracking();
		myJack->willBackTracking = false;
//		if(!mControl->isStun)
//			mControl->setTouchEnabled(true);
	}
	void backTracking();
	
	void pauseBackTracking()
	{
		unschedule(schedule_selector(Maingame::backTracking));
		((ControlJoystickButton*)mControl)->isBacking = false;
		myJack->willBackTracking = false;
	}
	
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
//		mControl->setTouchEnabled(false);
	}
	
	void stopSpecialAttack()
	{
//		mControl->setTouchEnabled(true);
	}
	
	void gameover()
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
	
	void closeShutter()
	{
		endCloseShutter();
	}
	
	void shutterClosedSound()
	{
//		AudioEngine::sharedInstance()->playEffect("sound_shutter_closed.m4a", false);
	}
	
	void endCloseShutter()
	{
		if(mySGD->getIsCleared())
		{
			CCTransitionFadeTR* t_trans = CCTransitionFadeTR::create(1.f, ZoomScript::scene());
			CCDirector::sharedDirector()->replaceScene(t_trans);
		}
		else
		{
			myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_fail);
			CCDirector::sharedDirector()->replaceScene(PuzzleMapScene::scene());
		}
	}
	
	int move_to_boss_position_frame;
	CCPoint move_to_boss_position_ds;
	void startMoveToBossPosition()
	{
		startMoveToCCPoint(myGD->getMainCumberPoint().convertToCCP());
	}
	void startMoveToCCPoint(CCPoint t_point)
	{
		move_to_boss_position_frame = 0;
		CCPoint after_position = getObjectToGameNodePosition(t_point);
		CCPoint sub_position = ccpSub(after_position, game_node->getPosition());
		CCLog("boss : %.2f\t recent : %.2f", after_position.y, game_node->getPositionY());
		move_to_boss_position_ds = ccpMult(sub_position, 1.f/30.f);
		schedule(schedule_selector(Maingame::moveToBossPosition));
	}
	void moveToBossPosition()
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
	
	CCPoint getObjectToGameNodePosition(CCPoint t_p)
	{
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float y_value = -t_p.y*myGD->game_scale+480.f*frame_size.height/frame_size.width/2.f;
		if(y_value > 60)																	y_value = 60;
		else if(y_value < -490*myGD->game_scale+480*frame_size.height/frame_size.width)		y_value = -490*myGD->game_scale+480*frame_size.height/frame_size.width;
		
		CCPoint after_position;
		
		if(myGD->gamescreen_type == kGT_full)					after_position = ccp(myGD->boarder_value,y_value);
		else if(myGD->gamescreen_type == kGT_leftUI)			after_position = ccp(50+myGD->boarder_value,y_value);
		else if(myGD->gamescreen_type == kGT_rightUI)			after_position = ccp(myGD->boarder_value,y_value);
		
		return after_position;
	}
	
	CCPoint getObjectToGameNodePositionCoin(CCPoint t_p)
	{
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float x_value = t_p.x/320.f*(480.f-myGD->boarder_value*2.f) + myGD->boarder_value;
		if(myGD->gamescreen_type == kGT_leftUI)			x_value += 50.f;
		float y_value = t_p.y/320.f*(480.f-myGD->boarder_value*2.f);
		
//		x_value = x_value-game_node->getPositionX();
		y_value = y_value+game_node->getPositionY();
		
		return ccp(x_value, y_value);
	}
	
	CCPoint getGameNodePosition(){	return game_node->getPosition();	}
	
	CCPoint getGameNodeToObjectPosition(CCPoint t_p)
	{
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float y_value = -(t_p.y - 480.f*frame_size.height/frame_size.width/2.f)/myGD->game_scale;
		CCPoint after_position;
		after_position = ccp(160,y_value);
		
		return after_position;
	}
	
	void moveGamePosition(CCPoint t_p)
	{
//		if(!myGD->is_setted_jack)// || myGD->game_step == kGS_unlimited)
			game_node->setPosition(getObjectToGameNodePosition(t_p));
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
	
	void takeSpeedUpEffect(int t_step)
	{
		CCPoint jack_position = myGD->getJackPoint().convertToCCP();
		
		TakeSpeedUp* t_tsu = TakeSpeedUp::create(t_step);
		t_tsu->setScale(1.f/1.5f);
		t_tsu->setPosition(ccpAdd(jack_position, ccp(20,20)));
		game_node->addChild(t_tsu, goldZorder);
	}
	
	void showMissMissile(CCPoint t_position)
	{
		t_position.x += rand()%21 - 10;
		t_position.y += rand()%21 - 10;
		
		CCSprite* miss_label = CCSprite::create("missile_miss.png");
		miss_label->setScale(1.f/1.5f);
		miss_label->setPosition(t_position);
		game_node->addChild(miss_label, goldZorder);
		
		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(miss_label, callfunc_selector(CCSprite::removeFromParent));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		miss_label->runAction(t_seq);
	}
	
	void showDamageMissile(CCPoint t_position, int t_damage)
	{
		MissileDamageLabel* damage_label = MissileDamageLabel::create(t_damage);
		damage_label->setScale(1.f/1.5f);
		damage_label->setPosition(t_position);
		game_node->addChild(damage_label, goldZorder);
		
		damage_label->startMyAction();
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
	CCNode* gameNodePointer()
	{
		return game_node;
	}
	void showChangeCard()
	{
		ChangeCard* t_w = ChangeCard::create();
		addChild(t_w, goldZorder);
		t_w->startAction();
	}
	
	void showCoin()
	{
		myGIM->showCoin(this, callfuncCCpI_selector(Maingame::takeExchangeCoin));
	}
	
	void takeExchangeCoin(CCPoint t_start_position, int t_coin_number)
	{
		myUI->takeExchangeCoin(getObjectToGameNodePositionCoin(t_start_position), t_coin_number);
	}
	
	void startExchange()
	{
		mySD->exchangeSilhouette();
		myMS->exchangeMS();
	}
	
	void setLimitMap()
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
	
	CCPoint limitted_map_position;
	int changing_game_step_frame;
	void setUnlimitMap()
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
	
	void changingGameStep()
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
	
	void gameNodeChangingGameStep(CCPoint t_p, int t_step)
	{
		IntPoint jack_point = myGD->getJackPoint();
		CCPoint jack_position = jack_point.convertToCCP();
		jack_position = getObjectToGameNodePosition(jack_position);
		
		CCPoint after_position = ccpAdd(limitted_map_position, ccpMult(ccpSub(jack_position, t_p), t_step/15.f));
		game_node->setPosition(after_position);
	}
	
	void showAreaScroll()
	{
		AreaScroll* t_w = AreaScroll::create();
		addChild(t_w, goldZorder);
		t_w->startAction();
	}
	
	int shake_frame;
	void startShake(float t_direction_angle)
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
	
	void shaking()
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
	
	void stopShake()
	{
		unschedule(schedule_selector(Maingame::shaking));
		shake_frame = 0;
	}
};

#endif
