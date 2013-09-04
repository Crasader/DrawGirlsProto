//
//  ShowTimeScene.h
//  DrawingJack
//
//  Created by 사원3 on 13. 3. 8..
//
//

#ifndef DrawingJack_ShowTimeScene_h
#define DrawingJack_ShowTimeScene_h

#include "cocos2d.h"
#include "AudioEngine.h"
#include <map>

using namespace cocos2d;
using namespace std;

enum ColorType{
	kColorType_yellow_red = 1,
	kColorType_green,
	kColorType_blue,
	kColorType_white
};

class Firecracker : public CCNode
{
public:
	static Firecracker* create()
	{
		Firecracker* t_f = new Firecracker();
		t_f->myInit();
		t_f->autorelease();
		return t_f;
	}
	
private:
	float move_speed;
	float speed_distance;
	CCParticleSystemQuad* moving_particle;
	CCParticleSystemQuad* bomb_particle;
	ColorType myColor;
	
	void startMoving()
	{
		AudioEngine::sharedInstance()->playEffect("sound_firecracker_start.mp3", false);
		ccColor4F startColor = getColorValue(myColor, true);
		ccColor4F endColor = getColorValue(myColor, false);
		
		moving_particle = new CCParticleSystemQuad();
		moving_particle->initWithTotalParticles(50);
		CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage("firecracker_moving_particle.png");
		moving_particle->setTexture(texture);
		moving_particle->setEmissionRate(50.00);
		moving_particle->setAngle(90.0);
		moving_particle->setAngleVar(360.0);
		ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
		moving_particle->setBlendFunc(blendFunc);
		moving_particle->setDuration(-1.0);
		moving_particle->setEmitterMode(kCCParticleModeGravity);
		moving_particle->setStartColor(startColor);
		ccColor4F startColorVar = {0,0,0,0};
		moving_particle->setStartColorVar(startColorVar);
		moving_particle->setEndColor(endColor);
		ccColor4F endColorVar = {0,0,0,1};
		moving_particle->setEndColorVar(endColorVar);
		moving_particle->setStartSize(10.00);
		moving_particle->setStartSizeVar(5.0);
		moving_particle->setEndSize(-1.0);
		moving_particle->setEndSizeVar(0.0);
		moving_particle->setGravity(ccp(0,-100));
		moving_particle->setRadialAccel(0.0);
		moving_particle->setRadialAccelVar(0.0);
		moving_particle->setSpeed(20);
		moving_particle->setSpeedVar(5.0);
		moving_particle->setTangentialAccel(0);
		moving_particle->setTangentialAccelVar(0);
		moving_particle->setTotalParticles(50);
		moving_particle->setLife(1.00);
		moving_particle->setLifeVar(0.5);
		moving_particle->setStartSpin(0.0);
		moving_particle->setStartSpinVar(0.0);
		moving_particle->setEndSpin(0.0);
		moving_particle->setEndSpinVar(0.0);
		moving_particle->autorelease();
		moving_particle->setPosition(CCPointZero);
		moving_particle->setPosVar(ccp(0,0));
		
		addChild(moving_particle);
		schedule(schedule_selector(Firecracker::moving));
	}
	void moving()
	{
		if(move_speed < 0)
		{
			AudioEngine::sharedInstance()->playEffect("sound_firecracker_final.mp3", false);
			moving_particle->setDuration(0);
			unschedule(schedule_selector(Firecracker::moving));
			
			ccColor4F startColor = getColorValue(myColor, true);
			ccColor4F endColor = getColorValue(myColor, false);
			int rand_value = rand()%4+1;
			
			bomb_particle = new CCParticleSystemQuad();
			bomb_particle->initWithTotalParticles(100);
			CCTexture2D* texture = CCTextureCache::sharedTextureCache()->addImage(CCString::createWithFormat("firecracker_bomb_particle%d.png", rand_value)->getCString());
			bomb_particle->setTexture(texture);
			bomb_particle->setEmissionRate(500.00);
			bomb_particle->setAngle(90.0);
			bomb_particle->setAngleVar(360.0);
			ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
			bomb_particle->setBlendFunc(blendFunc);
			bomb_particle->setDuration(0.10);
			bomb_particle->setEmitterMode(kCCParticleModeGravity);
			bomb_particle->setStartColor(startColor);
			ccColor4F startColorVar = {0,0,0,0};
			bomb_particle->setStartColorVar(startColorVar);
			bomb_particle->setEndColor(endColor);
			ccColor4F endColorVar = {0,0,0,1};
			bomb_particle->setEndColorVar(endColorVar);
			bomb_particle->setStartSize(10.00);
			bomb_particle->setStartSizeVar(5.0);
			bomb_particle->setEndSize(-1.0);
			bomb_particle->setEndSizeVar(0.0);
			bomb_particle->setGravity(ccp(0,-100));
			bomb_particle->setRadialAccel(0.0);
			bomb_particle->setRadialAccelVar(0.0);
			bomb_particle->setSpeed(70);
			bomb_particle->setSpeedVar(40.0);
			bomb_particle->setTangentialAccel(0);
			bomb_particle->setTangentialAccelVar(0);
			bomb_particle->setTotalParticles(100);
			bomb_particle->setLife(1.00);
			bomb_particle->setLifeVar(0.5);
			bomb_particle->setStartSpin(0.0);
			bomb_particle->setStartSpinVar(0.0);
			bomb_particle->setEndSpin(0.0);
			bomb_particle->setEndSpinVar(0.0);
			bomb_particle->autorelease();
			bomb_particle->setPosition(CCPointZero);
			bomb_particle->setPosVar(ccp(0,0));
			
			addChild(bomb_particle);
			
			CCDelayTime* t_delay = CCDelayTime::create(1.5f);
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Firecracker::selfRemove));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			runAction(t_seq);
		}
		else
		{
			setPosition(ccpAdd(getPosition(), ccp(0,move_speed)));
			move_speed -= speed_distance;
		}
	}
	
	void selfRemove()
	{
		removeFromParentAndCleanup(true);
	}
	
	ccColor4F getColorValue(ColorType t_type, bool is_start)
	{
		ccColor4F return_value;
		if(myColor == kColorType_yellow_red)
		{
			if(is_start)
			{
				return_value.r = 1.0;
				return_value.g = 0.8;
				return_value.b = 0.4;
				return_value.a = 1.0;
			}
			else
			{
				return_value.r = 0.58;
				return_value.g = 0;
				return_value.b = 0;
				return_value.a = 1.0;
			}
		}
		else if(myColor == kColorType_green)
		{
			if(is_start)
			{
				return_value.r = 0.25;
				return_value.g = 0.5;
				return_value.b = 0.0;
				return_value.a = 1.0;
			}
			else
			{
				return_value.r = 0.86;
				return_value.g = 1.0;
				return_value.b = 0.82;
				return_value.a = 1.0;
			}
		}
		else if(myColor == kColorType_blue)
		{
			if(is_start)
			{
				return_value.r = 0.0;
				return_value.g = 0.25;
				return_value.b = 0.50;
				return_value.a = 1.0;
			}
			else
			{
				return_value.r = 0.68;
				return_value.g = 0.81;
				return_value.b = 1.0;
				return_value.a = 1.0;
			}
		}
		else if(myColor == kColorType_white)
		{
			if(is_start)
			{
				return_value.r = 0.4;
				return_value.g = 0.4;
				return_value.b = 0.4;
				return_value.a = 1.0;
			}
			else
			{
				return_value.r = 1.0;
				return_value.g = 1.0;
				return_value.b = 1.0;
				return_value.a = 1.0;
			}
		}
		return return_value;
	}
	
	void myInit()
	{
		CCPoint start_position;
		start_position.x = rand()%260 + 30;
		start_position.y = rand()%280;
		
		move_speed = rand()%3+3;
		speed_distance = (rand()%10 + 5)/100.f;
		
		myColor = ColorType(rand()%4 + 1);
		
		setPosition(start_position);
		startMoving();
	}
};

class FirecrackerParent : public CCNode
{
public:
	static FirecrackerParent* create()
	{
		FirecrackerParent* t_fp = new FirecrackerParent();
		t_fp->myInit();
		t_fp->autorelease();
		return t_fp;
	}
	
private:
	int acting_cnt;
	
	void startActing()
	{
		acting_cnt = 0;
		schedule(schedule_selector(FirecrackerParent::acting));
	}
	void acting()
	{
		acting_cnt++;
		
		if(acting_cnt%10 == 0)
		{
			bool is_create = false;
			if(getChildrenCount() == 0)										is_create = true;
			else if(getChildrenCount() >= 1 && getChildrenCount() <= 3)		is_create = (rand()%(getChildrenCount()+1) == 0);
			else if(getChildrenCount() >= 4)								is_create = (rand()%10 == 0);
			
			if(is_create)
			{
				acting_cnt = 0;
				Firecracker* t_f = Firecracker::create();
				addChild(t_f);
			}
		}
	}
	
	void myInit()
	{
		startActing();
	}
};

class ShowTime : public CCLayer
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(ShowTime);
	
	virtual void onEnter();
	virtual void onExit();
	
private:
//	CCLabelTTF* touch_label;
	int twinkle_cnt;
	void touchOn();
	void startTwinkle();
	void twinkle();
	void stopTwinkle();
	
	CCMenu* check_menu;
	
	CCSprite* bottom_shutter;
	CCSprite* top_shutter;
	
	CCSprite* bonus_img;
	CCSprite* bonus_eye;
	
	void closeShutter();
	void shutterClosedSound();
	void endCloseShutter();
	
	virtual void keyBackClicked();
	void alertAction(int t1, int t2);
	
//	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void registerWithTouchDispatcher(void);
	
	map<int, CCPoint> multiTouchData;
	bool is_touched_menu;
	float zoom_base_distance;
	bool is_action;
	
	void menuAction(CCObject* sender)
	{
		if(!is_action)
		{
			is_action = true;
			closeShutter();
		}
	}
	
	void randomFlicker()
	{
		int random_value = rand()%16;
		
		if(random_value >= 2 && random_value <= 4)
			random_value = 7;
		else if(random_value >= 5 && random_value <= 10)
			random_value = 10;
		else if(random_value >= 11 && random_value <= 13)
			random_value = 13;
		else if(random_value == 14 || random_value == 15)
			random_value = 16;
		else
			random_value++;
		
		CCDelayTime* t_delay = CCDelayTime::create(random_value*0.1f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ShowTime::flicking));
		CCAction* t_seq = CCSequence::create(t_delay, t_call, NULL);
		bonus_eye->runAction(t_seq);
	}
	
	void flicking()
	{
		CCCallFunc* half_call = CCCallFunc::create(this, callfunc_selector(ShowTime::halfFlicker));
		CCDelayTime* half_delay = CCDelayTime::create(0.1);
		CCCallFunc* full_call = CCCallFunc::create(this, callfunc_selector(ShowTime::fullFlicker));
		CCDelayTime* full_delay = CCDelayTime::create(0.1);
		CCCallFunc* half2_call = CCCallFunc::create(this, callfunc_selector(ShowTime::halfFlicker));
		CCDelayTime* half2_delay = CCDelayTime::create(0.1);
		CCCallFunc* recovery_call = CCCallFunc::create(this, callfunc_selector(ShowTime::recoveryFlicker));
		
		CCAction* t_seq = CCSequence::create(half_call, half_delay, full_call, full_delay, half2_call, half2_delay, recovery_call, NULL);
		bonus_eye->runAction(t_seq);
	}
	
	void halfFlicker()
	{
		bonus_eye->setTextureRect(CCRectMake(100, 0, 100, 50));
	}
	
	void fullFlicker()
	{
		bonus_eye->setTextureRect(CCRectMake(200, 0, 100, 50));
	}
	
	void recoveryFlicker()
	{
		bonus_eye->setTextureRect(CCRectMake(0, 0, 100, 50));
		randomFlicker();
	}
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	void moveAnimationReturn();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
	{
		CCSetIterator iter;
		CCTouch *touch;
		
		for (iter = pTouches->begin(); iter != pTouches->end(); ++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
			
			multiTouchData[(int)touch] = location;
			
			touch_p = location;
			
			timeval time;
			gettimeofday(&time, NULL);
			touchStartTime = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec;
			touchStart_p = location;
			
			this->unschedule(schedule_selector(ShowTime::moveAnimation));
			
			isAnimated=false;
			
			if(multiTouchData.size() == 1)
			{
				if(!is_touched_menu && check_menu->ccTouchBegan(touch, pEvent))
				{
					is_touched_menu = true;
				}
			}
			else if(multiTouchData.size() == 2)
			{
				CCPoint sub_point = CCPointZero;
				map<int, CCPoint>::iterator it;
				for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
				{
					sub_point = ccpMult(sub_point, -1);
					sub_point = ccpAdd(sub_point, it->second);
				}
				
				zoom_base_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
			}
			else
			{
				if(is_touched_menu)
				{
					check_menu->ccTouchCancelled(touch, pEvent);
					is_touched_menu = false;
				}
			}
		}
	}
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				o_it->second = location;
				if(multiTouchData.size() == 1)
				{
					if(is_touched_menu)
					{
						check_menu->ccTouchMoved(touch, pEvent);
					}
					
					this->moveListXY(ccpSub(touch_p, location));
					touch_p = location;
				}
				else if(multiTouchData.size() == 2)
				{
					CCPoint sub_point = CCPointZero;
					map<int, CCPoint>::iterator it;
					for(it = multiTouchData.begin();it!=multiTouchData.end();it++)
					{
						sub_point = ccpMult(sub_point, -1);
						sub_point = ccpAdd(sub_point, it->second);
					}
					
					float changed_distance = sqrtf(powf(sub_point.x, 2.f) + powf(sub_point.y, 2.f));
					float after_scale = bonus_img->getScale()*changed_distance/zoom_base_distance;
					if(after_scale > 3.f)			after_scale = 3.f;
					else if(after_scale < 1.f)		after_scale = 1.f;
					zoom_base_distance = changed_distance;
					bonus_img->setScale(after_scale);
					
					if(bonus_img->getPositionX() > 160.f+(bonus_img->getScale()-1.f)*160.f)			bonus_img->setPositionX(160.f+(bonus_img->getScale()-1.f)*160.f);
					if(bonus_img->getPositionX() < 160.f-(bonus_img->getScale()-1.f)*160.f)			bonus_img->setPositionX(160.f-(bonus_img->getScale()-1.f)*160.f);
					if(bonus_img->getPositionY() > 240.f+(bonus_img->getScale()-1.f)*240.f)			bonus_img->setPositionY(240.f+(bonus_img->getScale()-1.f)*240.f);
					if(bonus_img->getPositionY() < 240.f-(bonus_img->getScale()-1.f)*240.f)			bonus_img->setPositionY(240.f-(bonus_img->getScale()-1.f)*240.f);
					
//					bonus_img->setPosition(ccpMult(ccp(160,240), after_scale));
				}
			}
		}
	}
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
	{
		CCSetIterator iter;
		CCTouch* touch;
		
		for(iter = pTouches->begin();iter != pTouches->end();++iter)
		{
			touch = (CCTouch*)(*iter);
			CCPoint location = CCDirector::sharedDirector()->convertToGL(touch->getLocationInView());
			
			map<int, CCPoint>::iterator o_it;
			o_it = multiTouchData.find((int)touch);
			if(o_it != multiTouchData.end())
			{
				multiTouchData.erase((int)touch);
				
				if(multiTouchData.size() == 0)
				{
					if(is_touched_menu)
					{
						check_menu->ccTouchEnded(touch, pEvent);
						is_touched_menu = false;
					}
					
					timeval time;
					gettimeofday(&time, NULL);
					long _time = ((unsigned long long)time.tv_sec * 1000000) + time.tv_usec - touchStartTime;
					CCPoint _spd = ccpMult(ccpSub(location, touchStart_p), 1.f/_time*10000);
					
					float spd_value = sqrtf(powf(_spd.x, 2.f) + powf(_spd.y, 2.f));
					if(isAnimated == false && fabsf(spd_value) > 2)
					{
						moveSpeed_p = _spd;
						this->schedule(schedule_selector(ShowTime::moveAnimation));
					}
				}
			}
		}
	}
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
	{
		ccTouchesEnded(pTouches, pEvent);
	}
};

#endif
