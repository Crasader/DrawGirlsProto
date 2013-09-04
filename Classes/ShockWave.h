//
//  ShockWave.h
//  DrawingJack
//
//  Created by 사원3 on 13. 2. 19..
//
//

#ifndef DrawingJack_ShockWave_h
#define DrawingJack_ShockWave_h

#include "cocos2d.h"
#include "GameData.h"
#include "AudioEngine.h"

using namespace cocos2d;
#include <functional>
using namespace placeholders;
class ShockWave : public CCSpriteBatchNode
{
public:
	static ShockWave* create(IntPoint t_createPoint, CCObject* t_emotion, SEL_CallFuncI d_emotion)
	{
		ShockWave* t_sw = new ShockWave();
		t_sw->myInit(t_createPoint, t_emotion, d_emotion);
		t_sw->autorelease();
		return t_sw;
	}
	
	void stopSW()
	{
		unschedule(schedule_selector(ShockWave::ingSW));
		if(!is_removing)
		{
			AudioEngine::sharedInstance()->stopEffect("sound_bomb_wave.mp3");
			is_removing = true;
			ing_frame = 0;
			schedule(schedule_selector(ShockWave::removeProcess));
		}
	}
	
private:
	CCPoint create_position;
	float radius;
	int ing_frame;
	GameData* myGD;
	bool is_removing;
	CCObject* emotion_target;
	SEL_CallFuncI emotion_delegate;
	
	
	void removeProcess()
	{
		ing_frame++;
		
		CCArray* my_child = getChildren();
		
		for(int i=0;i<getChildrenCount();i++)
		{
			CCSprite* t_child = (CCSprite*)my_child->objectAtIndex(i);
			t_child->setOpacity(t_child->getOpacity()-7);
		}
		
		if(ing_frame > 35)
		{
			unschedule(schedule_selector(ShockWave::removeProcess));
			removeFromParentAndCleanup(true);
		}
	}
	
	void startSW()
	{
		ing_frame = 0;
		schedule(schedule_selector(ShockWave::ingSW));
	}
	
	void ingSW()
	{
		if(ing_frame%15 == 0 && getChildrenCount() < 3)
		{
			CCSprite* t_sw = CCSprite::create("shock_wave.png");
			t_sw->setScale(0);
			addChild(t_sw);
		}
		
		CCArray* my_child = getChildren();
		
		for(int i=0;i<getChildrenCount();i++)
		{
			CCSprite* t_child = (CCSprite*)my_child->objectAtIndex(i);
			t_child->setScale(t_child->getScale()+0.04f);
		}
		radius += 80.f*0.04f;
		
		IntPoint jackPoint = myGD->getJackPoint();
		CCPoint jackPosition = ccp((jackPoint.x-1)*pixelSize+1,(jackPoint.y-1)*pixelSize+1);
		
		CCPoint subPosition = ccpSub(getPosition(), jackPosition);
		float distance = sqrtf(powf(subPosition.x, 2.f) + powf(subPosition.y, 2.f));
		
		if(radius > distance)
		{
			if(emotion_target && emotion_delegate)
				(emotion_target->*emotion_delegate)(6);
			myGD->communication("Jack_startDieEffect");
			myGD->communication("MP_resetTickingTimeBomb");
			stopSW();
		}
		
		ing_frame++;
	}
	
	void myInit(IntPoint t_createPoint, CCObject* t_emotion, SEL_CallFuncI d_emotion)
	{
		emotion_target = t_emotion;
		emotion_delegate = d_emotion;
		is_removing = false;
		myGD = GameData::sharedGameData();
		CCSprite* texture_spr = CCSprite::create("shock_wave.png");
		initWithTexture(texture_spr->getTexture(), kDefaultSpriteBatchCapacity);
		radius = 0;
		setPosition(ccp((t_createPoint.x-1)*pixelSize+1,(t_createPoint.y-1)*pixelSize+1));
		startSW();
	}
};

class SW_Parent : public CCNode
{
public:
	static SW_Parent* create()
	{
		SW_Parent* t_mySW = new SW_Parent();
		t_mySW->myInit();
		t_mySW->autorelease();
		return t_mySW;
	}
	
	void createSW(IntPoint t_create_point, CCObject* t_emotion, SEL_CallFuncI d_emotion)
	{
		if(getChildrenCount() == 0)
		{
			AudioEngine::sharedInstance()->playEffect("sound_bomb_wave.mp3", true);
			ShockWave* t_sw = ShockWave::create(t_create_point, t_emotion, d_emotion);
			addChild(t_sw);
		}
	}
	
	void createJDSW(IntPoint t_create_point, CCObject* t_emotion, SEL_CallFuncI d_emotion)
	{
		is_justDie = true;
		createSW(t_create_point, t_emotion, d_emotion);
	}
	
	void stopAllSW()
	{
		if(!is_justDie)
		{
			CCArray* my_child = getChildren();
			AudioEngine::sharedInstance()->stopEffect("sound_bomb_wave.mp3");
			for(int i=0;i<getChildrenCount();i++)
			{
				ShockWave* t_sw = (ShockWave*)my_child->objectAtIndex(i);
				t_sw->stopSW();
			}
		}
	}
	
private:
	GameData* myGD;
	bool is_justDie;
	
	void myInit()
	{
		is_justDie = false;
		myGD = GameData::sharedGameData();
		
//		myGD->regSW(this,
//					callfuncIpOC_selector(SW_Parent::createSW),
//					callfunc_selector(SW_Parent::stopAllSW),
//					callfuncIpOC_selector(SW_Parent::createJDSW));
		
		myGD->V_IpCCOCallfunci["SW_createSW"] = std::bind(&SW_Parent::createSW, this, _1, _2, _3);
		myGD->V_V["SW_stopAllSW"] = std::bind(&SW_Parent::stopAllSW, this);
		myGD->V_IpCCOCallfunci["SW_createJDSW"] = std::bind(&SW_Parent::createJDSW, this, _1, _2, _3);
	}
};

#endif
