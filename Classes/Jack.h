//
//  Jack.h
//  DrawingJack
//
//  Created by 사원3 on 12. 11. 23..
//
//

#ifndef DrawingJack_Jack_h
#define DrawingJack_Jack_h

#include "cocos2d.h"
#include "GameData.h"
#include <queue>
#include "AudioEngine.h"
#include "OtherEffect.h"
#include "StarGoldData.h"

using namespace cocos2d;
using namespace std;
using namespace placeholders;
enum jackSpeed{
	jackSpeedSlow	= 1,
	jackSpeedNormal	= 2,
	jackSpeedFast	= 4
};

enum jackState{
	jackStateNormal = 0,
	jackStateDrawing,
	jackStateBackTracking
};

enum KeepDirection{
	kKeepDirection_empty = 0,
	kKeepDirection_left,
	kKeepDirection_right
};

enum jackZorder{
	kJackZ_main = 1,
	kJackZ_defaultBarrier,
	kJackZ_ActiveBarrier,
	kJackZ_stunEffect
};

class StopEffects : public CCNode
{
public:
	virtual void selfRemove() = 0;
	
};

class StunHammer : public StopEffects
{
public:
	static StunHammer* create(CCObject* t_jack, SEL_CallFunc d_stun)
	{
		StunHammer* t_sh = new StunHammer();
		t_sh->myInit(t_jack, d_stun);
		t_sh->autorelease();
		return t_sh;
	}
	
	void startAction()
	{
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		t_animation->addSpriteFrameWithFileName("stun_hammer1.png");
		t_animation->addSpriteFrameWithFileName("stun_hammer1.png");
		t_animation->addSpriteFrameWithFileName("stun_hammer2.png");
		
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		
		CCRotateBy* t_rotate = CCRotateBy::create(0.3, -90);
		
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_animate, t_rotate);
		
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(StunHammer::afterAction));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
		
		hammerImg->runAction(t_seq);
	}
	
	void showHammer()
	{
		CCSprite* t_hammer = CCSprite::create("stun_hammer1.png");
		t_hammer->setAnchorPoint(ccp(1.0,0.5));
		t_hammer->setRotation(90);
		
		t_hammer->setPosition(ccp(34,20));
		
		addChild(t_hammer);
		
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		t_animation->addSpriteFrameWithFileName("stun_hammer1.png");
		t_animation->addSpriteFrameWithFileName("stun_hammer1.png");
		t_animation->addSpriteFrameWithFileName("stun_hammer2.png");
		
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		
		CCRotateBy* t_rotate = CCRotateBy::create(0.3, -90);
		
		CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_animate, t_rotate);
		
		CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(StunHammer::deleteTempHammer), t_hammer);
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_spawn, t_call);
		
		t_hammer->runAction(t_seq);
	}
	
	void selfRemove()
	{
		myGD->communication("Jack_resetStopEffects");
		myGD->communication("Main_touchOn");
		removeFromParentAndCleanup(true);
	}
	
private:
	CCSprite* hammerImg;
	CCObject* target_jack;
	SEL_CallFunc delegate_stun;
	CCSprite* starImg;
	
	void deleteTempHammer(CCObject* t_hammer)
	{
		((CCNode*)t_hammer)->removeFromParentAndCleanup(true);
	}
	
	void afterAction()
	{
		hammerImg->removeFromParentAndCleanup(true);

		AudioEngine::sharedInstance()->playEffect("sound_stun_hit.mp3",false);
		(target_jack->*delegate_stun)();
		
		starImg = CCSprite::create("stun_star.png", CCRectMake(0, 0, 20, 13));
		starImg->setPosition(ccp(0,12));
		
		CCSprite* t_texture = CCSprite::create("stun_star.png");
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=0;i<3;i++)
		{
			t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(0, i*13, 20, 13));
		}
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		addChild(starImg);
		
		starImg->runAction(t_repeat);
	}
	
	void myInit(CCObject* t_jack, SEL_CallFunc d_stun)
	{
		target_jack = t_jack;
		delegate_stun = d_stun;
		
		hammerImg = CCSprite::create("stun_hammer1.png");
		hammerImg->setAnchorPoint(ccp(1.0,0.5));
		hammerImg->setRotation(90);
		
		hammerImg->setPosition(ccp(34,20));
		
		addChild(hammerImg);
	}
};

class IceFog : public StopEffects
{
public:
	static IceFog* create(CCObject* t_jack, SEL_CallFunc d_freeze)
	{
		IceFog* t_if = new IceFog();
		t_if->myInit(t_jack, d_freeze);
		t_if->autorelease();
		return t_if;
	}
	
	void startAction()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.3, 1.0);
		
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(IceFog::afterAction));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		fogImg->runAction(t_seq);
	}
	
	void showFog()
	{
		fogImg = CCSprite::create("fog.png");
		fogImg->setScale(0);
		addChild(fogImg);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.3, 1.0);
		
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(IceFog::deleteFog));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		fogImg->runAction(t_seq);
	}
	
	void selfRemove()
	{
		myGD->communication("Jack_resetStopEffects");
		myGD->communication("Main_touchOn");
		removeFromParentAndCleanup(true);
	}
	
private:
	CCObject* target_jack;
	SEL_CallFunc delegate_freeze;
	
	CCSprite* fogImg;
	CCSprite* iceImg;
	
	void deleteFog()
	{
		fogImg->removeFromParentAndCleanup(true);
	}
	
	void afterAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_ice_hold.mp3", false);
		(target_jack->*delegate_freeze)();
		
		fogImg->removeFromParentAndCleanup(true);
		iceImg = CCSprite::create("ice.png");
		addChild(iceImg);
	}
	
	void myInit(CCObject* t_jack, SEL_CallFunc d_freeze)
	{
		target_jack = t_jack;
		delegate_freeze = d_freeze;
		
		fogImg = CCSprite::create("fog.png");
		fogImg->setScale(0);
		addChild(fogImg);
	}
};

class Sleep : public StopEffects
{
public:
	
	static Sleep* create(CCObject* t_jack, SEL_CallFunc d_sleep)
	{
		Sleep* t_s = new Sleep();
		t_s->myInit(t_jack, d_sleep);
		t_s->autorelease();
		return t_s;
	}
	
	void startAction()
	{
		CCDelayTime* t_delay = CCDelayTime::create(1.0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Sleep::afterAction));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
	}
	
	void showCircle()
	{
		CircleCreater* t_cc = CircleCreater::create(ccYELLOW, 12);
		addChild(t_cc);
		
		CCDelayTime* t_delay = CCDelayTime::create(1.0);
		CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(Sleep::deleteCircle), t_cc);
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
	}
	
	void selfRemove()
	{
		myGD->communication("Jack_resetStopEffects");
		myGD->communication("Main_touchOn");
		removeFromParentAndCleanup(true);
	}
	
private:
	CCObject* target_jack;
	SEL_CallFunc delegate_sleep;
	CircleCreater* my_cc;
	CCSprite* sleepImg;
	
	void deleteCircle(CCObject* t_remove)
	{
		((CircleCreater*)t_remove)->stopCreate();
	}
	
	void afterAction()
	{
		AudioEngine::sharedInstance()->playEffect("sound_sleep.mp3", false);
		(target_jack->*delegate_sleep)();
		
		my_cc->stopCreate();
		// real sleep add
		sleepImg = CCSprite::create("sleep_zzz.png", CCRectMake(0, 0, 30, 25));
		sleepImg->setPosition(ccp(10,18));
		addChild(sleepImg);
		
		CCSprite* t_texture = CCSprite::create("sleep_zzz.png");
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=0;i<5;i++)
		{
			t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*30, 0, 30, 25));
		}
		
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
		
		sleepImg->runAction(t_repeat);
	}
	
	void myInit(CCObject* t_jack, SEL_CallFunc d_sleep)
	{
		target_jack = t_jack;
		delegate_sleep = d_sleep;
		
		my_cc = CircleCreater::create(ccYELLOW, 12);
		addChild(my_cc);
	}
};

class Chaos : public CCNode
{
public:
	static Chaos* create(CCObject* t_jack, SEL_CallFunc d_chaos)
	{
		Chaos* t_c = new Chaos();
		t_c->myInit(t_jack, d_chaos);
		t_c->autorelease();
		return t_c;
	}
	
	void startAction()
	{
		CCDelayTime* t_delay = CCDelayTime::create(1.0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Chaos::afterAction));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
	}
	
	void showCircle()
	{
		CircleCreater* t_cc = CircleCreater::create(ccBLUE, 12);
		addChild(t_cc);
		
		CCDelayTime* t_delay = CCDelayTime::create(1.0);
		CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(Chaos::deleteCircle), t_cc);
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
		
		runAction(t_seq);
	}
	
	void selfRemove()
	{
		myGD->communication("Jack_resetStopEffects");
		removeFromParentAndCleanup(true);
	}
	
private:
	CCObject* target_jack;
	SEL_CallFunc delegate_chaos;
	CircleCreater* my_cc;
	CCSprite* chaosImg;
	
	void deleteCircle(CCObject* t_remove)
	{
		((CircleCreater*)t_remove)->stopCreate();
	}
	
	void afterAction()
	{
		(target_jack->*delegate_chaos)();
		
		my_cc->stopCreate();
		
		chaosImg = CCSprite::create("chaos.png");
		chaosImg->setAnchorPoint(ccp(0.5,0));
		chaosImg->setRotation(-45);
		addChild(chaosImg);
		
		CCRotateTo* t_rotate_left = CCRotateTo::create(0.5, 45);
		CCRotateTo* t_rotate_right = CCRotateTo::create(0.5, -45);
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_rotate_left, t_rotate_right);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
		
		chaosImg->runAction(t_repeat);
	}
	
	void myInit(CCObject* t_jack, SEL_CallFunc d_chaos)
	{
		target_jack = t_jack;
		delegate_chaos = d_chaos;
		
		my_cc = CircleCreater::create(ccBLUE, 12);
		addChild(my_cc);
	}
};

class MissileBarrier : public CCSprite
{
public:
	static MissileBarrier* create()
	{
		MissileBarrier* t_mb = new MissileBarrier();
		t_mb->myInit();
		t_mb->autorelease();
		return t_mb;
	}
	
private:
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	void myInit()
	{
		AudioEngine::sharedInstance()->playEffect("sound_barrier_pass.mp3",false);
		CCTexture2D* t_texture = CCTextureCache::sharedTextureCache()->addImage("jack_missile_barrier.png");
		initWithTexture(t_texture, CCRectMake(0, 0, 38, 38));
		
		CCAnimation* t_animation = CCAnimation::create();
		t_animation->setDelayPerUnit(0.1);
		for(int i=0;i<3;i++)
			t_animation->addSpriteFrameWithTexture(t_texture, CCRectMake(38*i, 0, 38, 38));
		CCAnimate* t_animate = CCAnimate::create(t_animation);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MissileBarrier::selfRemove));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_animate, t_call);
		runAction(t_seq);
	}
};

class Jack : public CCNode
{
public:
	static Jack* create()
	{
		Jack* myJack = new Jack();
		myJack->myInit();
		myJack->autorelease();
		return myJack;
	}
	
	virtual void setPosition(CCPoint t_sp)
	{
		CCNode::setPosition(t_sp);
		myGD->communication("Main_moveGamePosition", t_sp);
		myGD->communication("VS_setMoveGamePosition", t_sp);
	}
	
	void changeDirection(IntDirection t_d, IntDirection t_sd)
	{
		if(isReverse)
		{
			t_d = reverseDirection(t_d);
			t_sd = reverseDirection(t_sd);
		}
		
		IntPoint jp = myGD->getJackPoint();
		if(myGD->mapState[jp.x-1][jp.y] != mapOldline && myGD->mapState[jp.x-1][jp.y] != mapNewline &&
		   myGD->mapState[jp.x+1][jp.y] != mapOldline && myGD->mapState[jp.x+1][jp.y] != mapNewline &&
		   myGD->mapState[jp.x][jp.y-1] != mapOldline && myGD->mapState[jp.x][jp.y-1] != mapNewline &&
		   myGD->mapState[jp.x][jp.y+1] != mapOldline && myGD->mapState[jp.x][jp.y+1] != mapNewline)
		{
			escapeJack();
		}
		
		IntDirection c_d = directionStop;
		IntDirection c_sd = directionStop;
		if(t_d == directionLeftDown)
		{
			c_d = t_sd;
			if(t_sd == directionLeft)						c_sd = directionDown;
			else if(t_sd == directionDown)					c_sd = directionLeft;
		}
		else if(t_d == directionRightDown)
		{
			c_d = t_sd;
			if(t_sd == directionDown)						c_sd = directionRight;
			else if(t_sd == directionRight)					c_sd = directionDown;
		}
		else if(t_d == directionRightUp)
		{
			c_d = t_sd;
			if(t_sd == directionRight)						c_sd = directionUp;
			else if(t_sd == directionUp)					c_sd = directionRight;
		}
		else if(t_d == directionLeftUp)
		{
			c_d = t_sd;
			if(t_sd == directionLeft)						c_sd = directionUp;
			else if(t_sd == directionUp)					c_sd = directionLeft;
		}
		else
		{
			c_d = t_d;
			c_sd = t_sd;
		}
		
		if(direction == directionStop && t_d != directionStop) // move start
		{
			IntVector dv = IntVector::directionVector(t_d);
			IntVector cv = IntVector::directionVector(c_d);
			
			if(!isDrawingOn && myGD->mapState[jp.x+cv.dx][jp.y+cv.dy] == mapEmpty)
			{
				IntVector sdv = IntVector::directionVector(c_sd);
				if(!myGD->mapState[jp.x+sdv.dx][jp.y+sdv.dy] == mapOldline)
				{
					return;
				}
				else
				{
					check_turn_cnt = 0;
					direction = t_d;
					no_draw_direction = c_d;
					afterDirection = direction;
					secondDirection = t_sd;
					no_draw_secondDirection = c_sd;
					startMove();
					return;
				}
//				isDrawingOn = true;
			}
//			else if(!isDrawingOn && myGD->mapState[jp.x+dv.dx][jp.y+dv.dy] == mapOldget)											// if gesture
//			{
//				IntVector sdv = IntVector::directionVector(t_sd);
//				int loop_cnt = 0;
//				while(myGD->mapState[jp.x+sdv.dx][jp.y+sdv.dy] == mapOldline && loop_cnt < 3)
//				{
//					loop_cnt++;
//					if(myGD->mapState[jp.x+sdv.dx+dv.dx][jp.y+sdv.dy+dv.dy] == mapOldline)
//					{
//						check_turn_cnt = 4;
//						direction = t_sd;
//						afterDirection = direction;
//						secondDirection = t_d;
//						loop_cnt = 5;
//						startMove();
//						break;
//					}
//					else
//					{
//						IntVector t_sdv = IntVector::directionVector(t_sd);
//						sdv.dx += t_sdv.dx;
//						sdv.dy += t_sdv.dy;
//					}
//				}
//				if(loop_cnt < 5)
//				{
//					sdv = IntVector::reverseDirectionVector(t_sd);
//					loop_cnt = 0;
//					while(myGD->mapState[jp.x+sdv.dx][jp.y+sdv.dy] == mapOldline && loop_cnt < 3)
//					{
//						loop_cnt++;
//						if(myGD->mapState[jp.x+sdv.dx+dv.dx][jp.y+sdv.dy+dv.dy] == mapOldline)
//						{
//							check_turn_cnt = 4;
//							direction = IntVector::getReverseDirection(t_sd);
//							afterDirection = direction;
//							secondDirection = t_d;
//							loop_cnt = 5;
//							startMove();
//							break;
//						}
//						else
//						{
//							IntVector t_sdv = IntVector::reverseDirectionVector(t_sd);
//							sdv.dx += t_sdv.dx;
//							sdv.dy += t_sdv.dy;
//						}
//					}
//				}
//				
//				return;
//			}
//			else if(myGD->mapState[jp.x+dv.dx][jp.y+dv.dy] == mapNewline)
//			{
//				check_turn_cnt = 4;
//				isReverseGesture = true;
//				direction = t_sd;
//				afterDirection = t_d;
//				secondDirection = t_d;
//				keep_direction = kKeepDirection_empty;
//				startMove();
//				return;
//			}
			check_turn_cnt = 0;
			direction = t_d;
			no_draw_direction = c_d;
			afterDirection = direction;
			secondDirection = t_sd;
			no_draw_secondDirection = c_sd;
			startMove();
		}
//		else if(t_d == directionStop) // move stop
//		{
//			afterDirection = t_d;
//			secondDirection = t_sd;
//		}
		else // real change direction
		{
			IntVector dv = IntVector::directionVector(t_d);
			IntVector cv = IntVector::directionVector(c_d);
			
			if(!isDrawingOn && (myGD->mapState[jp.x+cv.dx][jp.y+cv.dy] == mapEmpty || myGD->mapState[jp.x+cv.dx][jp.y+cv.dy] == mapOldget))
			{
				IntVector sdv = IntVector::directionVector(c_sd);
				if(!myGD->mapState[jp.x+sdv.dx][jp.y+sdv.dy] == mapOldline)
				{
					if(isMoving)
						stopMove();
				}
				else
				{
					check_turn_cnt = 0;
					direction = t_d;
					no_draw_direction = c_d;
					afterDirection = direction;
					secondDirection = t_sd;
					no_draw_secondDirection = c_sd;
//					startMove();
				}
				return;
			}
			
			if(t_d == direction)
			{
				check_turn_cnt = 0;
			}
			
//			if((t_d != directionStop && t_sd != directionStop) && myGD->mapState[jp.x+dv.dx][jp.y+dv.dy] == mapNewline)
//			{
//				isReverseGesture = true;
//				reverseTurnCnt = 0;
//				afterDirection = t_sd;
//				secondDirection = t_d;
//			}
//			else
//			{
//				if(isReverseGesture)	isReverseGesture = false;
				afterDirection = t_d;
				secondDirection = t_sd;
				no_draw_direction = c_d;
				no_draw_secondDirection = c_sd;
				keep_direction = kKeepDirection_empty;
//			}
		}
//		if()
//		{
//			direction = directionStop;
//			afterDirection = directionStop;
//			secondDirection = directionStop;
//		}
	}
	
	void backTrackingAtAfterMoving(IntPoint t_p)
	{
		if(t_p.isNull())
			return;
		
		if(isMoving)
		{
			btPoint = t_p;
		}
		else
		{
			myGD->setJackPoint(t_p);
			setPosition(ccp((t_p.x-1)*pixelSize+1, (t_p.y-1)*pixelSize+1));
			if(myState != jackStateBackTracking)
				myGD->communication("Main_startBackTracking");
			setJackState(jackStateBackTracking);
		}
	}
	
	void endBackTracking()
	{
		setJackState(jackStateNormal);
		
		afterDirection = directionStop;
		secondDirection = directionStop;
		keep_direction = kKeepDirection_empty;
//		isReverseGesture = false;
		
		for(int i=mapWidthInnerBegin;i<mapWidthInnerEnd;i++)
		{
			for(int j=mapHeightInnerBegin;j<mapHeightInnerEnd;j++)
			{
				if(myGD->mapState[i][j] == mapNewline)
					myGD->mapState[i][j] = mapEmpty;
			}
		}
	}
	
	void changeSpeed(float t_s)
	{
		if(t_s > 4.f)			t_s = 4.f;
		else if(t_s < 0.8f)		t_s = 0.8f;
		
		after_speed = t_s;
		if(test_speed < after_speed)
		{
			speed_change_img = SpeedChangeEffect::create(true);
			addChild(speed_change_img, kJackZ_stunEffect);
			speed_change_img->startAction();
		}
		else if(test_speed > after_speed)
		{
			speed_change_img = SpeedChangeEffect::create(false);
			addChild(speed_change_img, kJackZ_stunEffect);
			speed_change_img->startAction();
		}
	}
	
	void createHammer()
	{
		if(!t_se)
		{
			t_se = StunHammer::create(this, callfunc_selector(Jack::stunJack));
			addChild(t_se, kJackZ_stunEffect);
			((StunHammer*)t_se)->startAction();
		}
		else
		{
			((StunHammer*)t_se)->showHammer();
		}
	}
	
	void createFog()
	{
		if(!t_se)
		{
			t_se = IceFog::create(this, callfunc_selector(Jack::stunJack));
			addChild(t_se, kJackZ_stunEffect);
			((IceFog*)t_se)->startAction();
		}
		else
		{
			((IceFog*)t_se)->showFog();
		}
	}
	
	void createSleep()
	{
		if(!t_se)
		{
			t_se = Sleep::create(this, callfunc_selector(Jack::stunJack));
			addChild(t_se, kJackZ_stunEffect);
			((Sleep*)t_se)->startAction();
		}
		else
		{
			((Sleep*)t_se)->showCircle();
		}
	}
	
	void createChaos()
	{
		if(!t_chaos)
		{
			t_chaos = Chaos::create(this, callfunc_selector(Jack::reverseOn));
			addChild(t_chaos, kJackZ_stunEffect);
			t_chaos->startAction();
		}
		else
		{
			t_chaos->showCircle();
		}
	}
	
	void reverseOn()
	{
		isReverse = true;
	}
	
	void reverseOff()
	{
		isReverse = false;
		t_chaos->removeFromParentAndCleanup(true);
		t_chaos = NULL;
	}
	
	void stunJack()
	{
		myGD->communication("Main_touchEnd");
		if(isDrawingOn)
			isDrawingOn = false;
	}
	
	IntDirection getDirection(){	return direction;	}
	IntDirection getSecondDirection(){	return secondDirection;	}
	jackState getJackState(){	return myState;	}
	
	bool willBackTracking;
	bool isMoving;
	bool isStun;
	
	bool isDrawingOn;
	
	StopEffects* t_se;
	
	void stopMove()
	{
		direction = directionStop;
		afterDirection = directionStop;
		secondDirection = directionStop;
		before_x_direction = directionStop;
		before_x_cnt = 0;
		isMoving = false;
		unschedule(schedule_selector(Jack::moveTest));
		escapeJack();
	}
	
	void stopJack()
	{
		unschedule(schedule_selector(Jack::moveTest));
	}
	
	void startDieEffect() // after coding
	{
		return;
		if(!isDie)
		{
			if(getJackState() == jackStateDrawing)
			{
				jack_drawing->setVisible(false);
			}
			isDrawingOn = false;
			
			myGD->removeMapNewline();
			myGD->communication("PM_cleanPath");
			
			isStun = true;
			
			myGD->communication("Main_startSpecialAttack");
//			AudioEngine::sharedInstance()->playEffect("sound_jack_die.mp3", false);
			AudioEngine::sharedInstance()->playEffect("sound_die_jack.mp3", false);
			isDie = true;
			dieEffectCnt = 0;
			jackImg->removeFromParentAndCleanup(true);
			jackImg = CCSprite::create("jack_die.png");
			jackImg->setScale(0.2f);
			addChild(jackImg, kJackZ_main);
			schedule(schedule_selector(Jack::dieEffect));
		}
	}
	
	void showMB()
	{
		if(!isDie)
		{
			MissileBarrier* t_mb = MissileBarrier::create();
			t_mb->setScale(0.8f);
			addChild(t_mb, kJackZ_ActiveBarrier);
		}
	}
	
	IntDirection getRecentDirection()
	{
		return direction;
	}
	
	void setTouchPointByJoystick(CCPoint t_p, IntDirection t_direction, bool is_touchEnd)
	{
		if(!joystickSpr_byJoystick)
		{
			joystickSpr_byJoystick = CCSprite::create("control_joystick_small_circle.png");
			addChild(joystickSpr_byJoystick, kJackZ_defaultBarrier);
		}
		if(!touchPointSpr_byJoystick)
		{
			touchPointSpr_byJoystick = CCSprite::create("control_joystick_small_ball.png");
			addChild(touchPointSpr_byJoystick, kJackZ_defaultBarrier);
		}
		if(!directionSpr_byJoystick)
		{
			directionSpr_byJoystick = CCSprite::create("control_joystick_arrow.png");
			addChild(directionSpr_byJoystick, kJackZ_defaultBarrier);
		}
		
		if(is_touchEnd || t_direction == directionStop)
		{
			touchPointSpr_byJoystick->setVisible(false);
			directionSpr_byJoystick->setVisible(false);
			joystickSpr_byJoystick->setVisible(false);
			return;
		}
		else
		{
			touchPointSpr_byJoystick->setVisible(true);
			directionSpr_byJoystick->setVisible(true);
			joystickSpr_byJoystick->setVisible(true);
		}
		
		touchPointSpr_byJoystick->setPosition(ccpMult(t_p, 0.385f));
		if(t_direction == directionLeft)
		{
			directionSpr_byJoystick->setRotation(-90);
			directionSpr_byJoystick->setPosition(ccp(-30,0));
		}
		else if(t_direction == directionDown)
		{
			directionSpr_byJoystick->setRotation(-180);
			directionSpr_byJoystick->setPosition(ccp(0,-30));
		}
		else if(t_direction == directionRight)
		{
			directionSpr_byJoystick->setRotation(90);
			directionSpr_byJoystick->setPosition(ccp(30,0));
		}
		else if(t_direction == directionUp)
		{
			directionSpr_byJoystick->setRotation(0);
			directionSpr_byJoystick->setPosition(ccp(0,30));
		}
		else if(t_direction == directionStop)
		{
			directionSpr_byJoystick->setVisible(false);
		}
	}
	
	void takeSpeedUpItem()
	{
		if(speed_up_value >= 0.5f)
		{
			myGD->communication("MP_createJackMissile", rand()%(kElementCode_plasma+1), 1, 0.7f);
		}
		else
		{
			speed_up_value += 0.1f;
			changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);
		}
	}
	
	float getSpeedUpValue()
	{
		return speed_up_value;
	}
	
	float getAlphaSpeed()
	{
		return alpha_speed_value;
	}
	
	void setAlphaSpeed(float t_s)
	{
		alpha_speed_value = t_s;
		changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);
	}
	
	void initStartPosition(CCPoint t_p)
	{
		int base_value = roundf(-t_p.y/myGD->game_scale/2.f);
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_height = roundf(480*screen_size.height/screen_size.width/2.f);
		
		IntPoint checking_point = IntPoint(80,base_value+roundf(screen_height/myGD->game_scale/2.f));
		
		int map_end_check_cnt = 0;
		bool is_found = false;
		for(int i=0;!is_found && map_end_check_cnt < 2;i++)
		{
			if(i%2 == 0)
				checking_point.x -= i;
			else
				checking_point.x += i;
			
			if(!checking_point.isInnerMap())
			{
				map_end_check_cnt++;
				continue;
			}
			
			if(myGD->mapState[checking_point.x][checking_point.y] == mapOldline)
			{
				is_found = true;
				myGD->setJackPoint(checking_point);
				afterPoint = checking_point;
				setPosition(ccp((checking_point.x-1)*pixelSize+1, (checking_point.y-1)*pixelSize+1));
				break;
			}
		}
		
		if(!is_found)
		{
			CCLog("faskdhfn;asjbfv;kjqdhbf;kvuhqasdk;cn");
		}
	}
	
	void setJackState(jackState t_s)
	{
		myState = t_s;
		myGD->setJackState(myState);
		if(myState == jackStateNormal)
		{
			jackImg->setColor(ccWHITE);
			jackImg->setVisible(true);
			jack_drawing->setVisible(false);
			if(!is_hard && !jack_barrier->isVisible())
				jack_barrier->setVisible(true);
		}
		else if(myState == jackStateDrawing)
		{
			jack_drawing->setVisible(true);
			jackImg->setVisible(false);
			if(!is_hard && jack_barrier->isVisible())
				jack_barrier->setVisible(false);
		}
		else if(myState == jackStateBackTracking)
		{
			jackImg->setColor(ccGRAY);
			if(!is_hard && jack_barrier->isVisible())
				jack_barrier->setVisible(false);
		}
	}
	
private:
	CCSprite* touchPointSpr_byJoystick;
	CCSprite* directionSpr_byJoystick;
	CCSprite* joystickSpr_byJoystick;
	
	bool isReverse;
	
	KeepDirection keep_direction; // rotary

	IntPoint btPoint;
	IntPoint move_before_point;
		   
	jackState myState;
	jackState afterState;
	jackSpeed mySpeed;
	jackSpeed afterSpeed;
	IntDirection direction;
	IntDirection afterDirection;
	IntDirection secondDirection;
	
	IntDirection no_draw_direction;
	IntDirection no_draw_secondDirection;
	
	IntDirection before_x_direction;
	int before_x_cnt;
	
	float test_speed;
	float after_speed;
	bool is_end_turn;
	
	bool is_double_moving;
	
	bool is_hard; // is_noShield
	
	int check_turn_cnt;
	
	Chaos* t_chaos;
	
	CCSprite* jackImg;
	CCSprite* jack_drawing;
	CCSprite* jack_barrier;
	SpeedChangeEffect* speed_change_img;
	
	int moveValue;
	
//	bool isReverseGesture;
	int reverseTurnCnt;
	
	IntPoint afterPoint;
	
	int dieEffectCnt;
	bool isDie;
	
	float speed_up_value;
	float alpha_speed_value;
	
	IntDirection reverseDirection(IntDirection t_d)
	{
		IntDirection returnDirection;
		if(t_d == directionLeftUp)					returnDirection = directionRightDown;
		else if(t_d == directionLeft)				returnDirection = directionRight;
		else if(t_d == directionLeftDown)			returnDirection = directionRightUp;
		else if(t_d == directionDown)				returnDirection = directionUp;
		else if(t_d == directionRightDown)			returnDirection = directionLeftUp;
		else if(t_d == directionRight)				returnDirection = directionLeft;
		else if(t_d == directionRightUp)			returnDirection = directionLeftDown;
		else if(t_d == directionUp)					returnDirection = directionDown;
		else										returnDirection = directionStop;
		
		return returnDirection;
	}
	
	void dieEffect()
	{
		dieEffectCnt++;
		if(dieEffectCnt < 45)
		{
			jackImg->setScale(0.2f + dieEffectCnt*0.02f);
		}
		else if(dieEffectCnt > 80)
		{
			unschedule(schedule_selector(Jack::dieEffect));
			
			if(myGD->getCommunicationBool("UI_beRevivedJack"))
			{
				speed_up_value = 0.f;
				changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);
				
				isDie = false;
				isStun = false;
				
				dieEscapeJack();
				
				if(myGD->getIsGameover())
					endGame();
				else
				{
					jackImg->removeFromParentAndCleanup(true);
					
					CCTexture2D* jack_texture = CCTextureCache::sharedTextureCache()->addImage("jack2.png");
					
					jackImg = CCSprite::createWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
					jackImg->setScale(0.8f);
					addChild(jackImg, kJackZ_main);
					
					CCAnimation* jack_animation = CCAnimation::create();
					jack_animation->setDelayPerUnit(0.1f);
					jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
					jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
					jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(23, 0, 23, 23));
					
					CCAnimate* jack_animate = CCAnimate::create(jack_animation);
					CCRepeatForever* jack_repeat = CCRepeatForever::create(jack_animate);
					jackImg->runAction(jack_repeat);
					
					setTouchPointByJoystick(CCPointZero, directionStop, true);
					setJackState(jackStateNormal);
					
					myGD->communication("GIM_dieCreateItem");
					myGD->communication("Main_resetIsLineDie");
					myGD->communication("Main_stopSpecialAttack");
				}
			}
			else
			{
				myGD->communication("UI_showContinuePopup", this, callfunc_selector(Jack::endGame), this, callfunc_selector(Jack::continueGame));
			}
		}
	}
	
	void continueGame()
	{
		speed_up_value = 0.f;
		changeSpeed(myGD->jack_base_speed + speed_up_value + alpha_speed_value);
		
		isDie = false;
		isStun = false;
		jackImg->removeFromParentAndCleanup(true);
		
		CCTexture2D* jack_texture = CCTextureCache::sharedTextureCache()->addImage("jack2.png");
		
		jackImg = CCSprite::createWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
		jackImg->setScale(0.8f);
		addChild(jackImg, kJackZ_main);
		
		CCAnimation* jack_animation = CCAnimation::create();
		jack_animation->setDelayPerUnit(0.1f);
		jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
		jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
		jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(23, 0, 23, 23));
		
		CCAnimate* jack_animate = CCAnimate::create(jack_animation);
		CCRepeatForever* jack_repeat = CCRepeatForever::create(jack_animate);
		jackImg->runAction(jack_repeat);
		
		
		setTouchPointByJoystick(CCPointZero, directionStop, true);
		
		setJackState(jackStateNormal);
		
		dieEscapeJack();
		
		myGD->communication("GIM_dieCreateItem");
		
		myGD->communication("Main_resetIsLineDie");
		
		myGD->communication("Main_stopSpecialAttack");
	}
	
	void endGame()
	{
		mySGD->fail_code = kFC_gameover;
		myGD->setIsGameover(true);
		myGD->communication("Main_allStopSchedule");
		myGD->communication("Main_gameover");
	}
	
	void searchAndMoveOldline(IntMoveState searchFirstMoveState);
	
	void escapeJack()
	{
		if(afterPoint.isInnerMap())
		{
			if(myGD->mapState[afterPoint.x-1][afterPoint.y] == mapOldget &&
			   myGD->mapState[afterPoint.x+1][afterPoint.y] == mapOldget &&
			   myGD->mapState[afterPoint.x][afterPoint.y-1] == mapOldget &&
			   myGD->mapState[afterPoint.x][afterPoint.y+1] == mapOldget)
			{
				IntMoveState searchFirstMoveState = IntMoveState(afterPoint.x, afterPoint.y, directionStop);
				searchAndMoveOldline(searchFirstMoveState);
			}
		}
	}
	
	void dieEscapeJack()
	{
		IntMoveState searchFirstMoveState = IntMoveState(afterPoint.x, afterPoint.y, directionStop);
		searchAndMoveOldline(searchFirstMoveState);
	}
	
	void startMove()
	{
		is_end_turn = true;
		
		isMoving = true;
		moveValue = 0;
		move_loop_cnt = 0;
		moveTest();
		schedule(schedule_selector(Jack::moveTest));
	}
	
	int move_loop_cnt;
	
	void moveTest();
	
	void resetStopEffects()
	{
		t_se = NULL;
		t_chaos = NULL;
	}
	
	void positionRefresh()
	{
		setPosition(getPosition());
	}
	
	void myInit()
	{
		before_x_direction = directionStop;
		before_x_cnt = 0;
		keep_direction = kKeepDirection_empty;
		isDrawingOn = false;
//		isReverseGesture = false;
		isReverse = false;
		t_se = NULL;
		t_chaos = NULL;
		isStun = false;
		isDie = false;
		is_double_moving = false;
		
		myGD->V_F["Jack_changeSpeed"] = std::bind(&Jack::changeSpeed, this, _1);
		myGD->V_V["Jack_startDieEffect"] = std::bind(&Jack::startDieEffect, this);
		myGD->V_V["Jack_createHammer"] = std::bind(&Jack::createHammer, this);
		myGD->V_V["Jack_createFog"] = std::bind(&Jack::createFog, this);
		myGD->V_V["Jack_createSleep"] = std::bind(&Jack::createSleep, this);
		myGD->V_V["Jack_createChaos"] = std::bind(&Jack::createChaos, this);
		myGD->V_V["Jack_reverseOff"] = std::bind(&Jack::reverseOff, this);
		myGD->V_V["Jack_resetStopEffects"] = std::bind(&Jack::resetStopEffects, this);
		myGD->V_V["Jack_showMB"] = std::bind(&Jack::showMB, this);
		myGD->V_V["Jack_takeSpeedUpItem"] = std::bind(&Jack::takeSpeedUpItem, this);
		myGD->F_V["Jack_getAlphaSpeed"] = std::bind(&Jack::getAlphaSpeed, this);
		myGD->V_F["Jack_setAlphaSpeed"] = std::bind(&Jack::setAlphaSpeed, this, _1);
		myGD->F_V["Jack_getSpeedUpValue"] = std::bind(&Jack::getSpeedUpValue, this);
		myGD->V_V["Jack_positionRefresh"] = std::bind(&Jack::positionRefresh, this);
		
		
		
		isMoving = false;
		willBackTracking = false;
		btPoint = IntPoint();
		direction = directionStop;
		afterDirection = directionStop;
		
		//////////////////////////////////////////////////////////// move test ////////////////////////////////
		speed_up_value = 0.f;
		alpha_speed_value = 0.f;
		test_speed = myGD->jack_base_speed + speed_up_value + alpha_speed_value;
		after_speed = test_speed;
		//////////////////////////////////////////////////////////// move test ////////////////////////////////
		
		myState = jackStateNormal;
		afterState = jackStateNormal;
		
		CCTexture2D* jack_texture = CCTextureCache::sharedTextureCache()->addImage("jack2.png");
		
		jackImg = CCSprite::createWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
		jackImg->setScale(0.8f);
		addChild(jackImg, kJackZ_main);
		
		CCAnimation* jack_animation = CCAnimation::create();
		jack_animation->setDelayPerUnit(0.1f);
		jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
		jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(0, 0, 23, 23));
		jack_animation->addSpriteFrameWithTexture(jack_texture, CCRectMake(23, 0, 23, 23));
		
		CCAnimate* jack_animate = CCAnimate::create(jack_animation);
		CCRepeatForever* jack_repeat = CCRepeatForever::create(jack_animate);
		jackImg->runAction(jack_repeat);
		
		
		jack_drawing = CCSprite::create("jack.png"); // jack_drawing.png
		jack_drawing->setScale(0.8f);
		jack_drawing->setVisible(false);
		addChild(jack_drawing, kJackZ_defaultBarrier);
		
//		is_hard = SelectedMapData::sharedInstance()->getIsNoShield();
		is_hard = false;
		
//		if(!is_hard)
//		{
			CCSprite* t_texture = CCSprite::create("jack_barrier.png");
			
			jack_barrier = CCSprite::createWithTexture(t_texture->getTexture(), CCRectMake(100, 0, 25, 25));
			jack_barrier->setScale(0.8f);
			addChild(jack_barrier, kJackZ_defaultBarrier);
			
			CCAnimation* t_animation = CCAnimation::create();
			t_animation->setDelayPerUnit(0.1);
			for(int i=0;i<5;i++)
				t_animation->addSpriteFrameWithTexture(t_texture->getTexture(), CCRectMake(i*25, 0, 25, 25));
			CCAnimate* t_animate = CCAnimate::create(t_animation);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_animate);
			
			jack_barrier->runAction(t_repeat);
//		}
		
//		setStartPosition();
	}
	
	void setStartPosition()
	{
		IntMoveState searchFirstMoveState = IntMoveState(160/2, 215/2, directionStop);
		searchAndMoveOldline(searchFirstMoveState);
	}
	
	void keepDirectionAction(IntPoint jp, IntDirection t_d)
	{
		IntVector left_vector = IntVector::directionVector(IntVector::getLeftDirection(t_d));
		IntVector right_vector = IntVector::directionVector(IntVector::getRightDirection(t_d));
		
		IntPoint left_point = IntPoint(jp.x+left_vector.dx, jp.y+left_vector.dy);
		IntPoint right_point = IntPoint(jp.x+right_vector.dx, jp.y+right_vector.dy);
		
		if(left_point.isInnerMap() && right_point.isInnerMap() &&
		   (((myGD->mapState[left_point.x][left_point.y] == mapOldget || myGD->mapState[left_point.x][left_point.y] == mapOldline) &&
			 (myGD->mapState[right_point.x][right_point.y] == mapOldget || myGD->mapState[right_point.x][right_point.y] == mapOldline)) ||
			(myGD->mapState[left_point.x][left_point.y] == mapEmpty && myGD->mapState[right_point.x][right_point.y] == mapEmpty)))
		{
			
		}
		else
		{
			if(left_point.isInnerMap() && (myGD->mapState[left_point.x][left_point.y] == mapOldget || myGD->mapState[left_point.x][left_point.y] == mapOldline))
				keep_direction = kKeepDirection_left;
			else if(right_point.isInnerMap() && (myGD->mapState[right_point.x][right_point.y] == mapOldget || myGD->mapState[right_point.x][right_point.y] == mapOldline))
				keep_direction = kKeepDirection_right;
		}
	}
	
	bool rotarySelection(IntPoint jp, IntDirection t_d)
	{
//		if(myGD->mapState[jp.x][jp.y] != mapOldline)
//			return false;
		
		IntVector left_vector = IntVector::directionVector(IntVector::getLeftDirection(t_d));
		IntVector right_vector = IntVector::directionVector(IntVector::getRightDirection(t_d));
		IntVector direct_vector = IntVector::directionVector(t_d);
		
		IntPoint left_point = IntPoint(jp.x+left_vector.dx, jp.y+left_vector.dy);
		IntPoint right_point = IntPoint(jp.x+right_vector.dx, jp.y+right_vector.dy);
		IntPoint direct_point = IntPoint(jp.x+direct_vector.dx, jp.y+direct_vector.dy);
		
		IntPoint return_point = IntPoint(jp.x-direct_vector.dx, jp.y-direct_vector.dy);
		
		int rotary_cnt = 0;
		bool is_left = false;
		bool is_right = false;
		bool is_direct = false;
		
		int oldget_cnt = 0;
		
		if(left_point.isInnerMap() && myGD->mapState[left_point.x][left_point.y] == mapOldline)
		{
			is_left = true;
			rotary_cnt++;
		}
		else if(left_point.isInnerMap() && myGD->mapState[left_point.x][left_point.y] == mapOldget)
		{
			oldget_cnt++;
		}
		
		if(right_point.isInnerMap() && myGD->mapState[right_point.x][right_point.y] == mapOldline)
		{
			is_right = true;
			rotary_cnt++;
		}
		else if(right_point.isInnerMap() && myGD->mapState[right_point.x][right_point.y] == mapOldget)
		{
			oldget_cnt++;
		}
		
		if(direct_point.isInnerMap() && myGD->mapState[direct_point.x][direct_point.y] == mapOldline)
		{
			is_direct = true;
			rotary_cnt++;
		}
		else if(direct_point.isInnerMap() && myGD->mapState[direct_point.x][direct_point.y] == mapOldget)
		{
			oldget_cnt++;
		}
		
		if(return_point.isInnerMap() && myGD->mapState[return_point.x][return_point.y] == mapOldget)
		{
			oldget_cnt++;
		}
		
		
		if(rotary_cnt >= 2 && oldget_cnt >= 2)
		{
			return false;
		}
		
		if(rotary_cnt >= 2 && keep_direction != kKeepDirection_empty)
		{
			if(keep_direction == kKeepDirection_left && is_right)
			{
				check_turn_cnt = 4;
				direction = IntVector::getRightDirection(t_d);
				afterDirection = direction;
			}
			else if(keep_direction == kKeepDirection_right && is_left)
			{
				check_turn_cnt = 4;
				direction = IntVector::getLeftDirection(t_d);
				afterDirection = direction;
			}
			else	return false;
			
			return true;
		}
		else
			return false;
	}
};

#endif
