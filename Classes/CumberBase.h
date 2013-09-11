//
//  CumberBase.h
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 20..
//
//

#ifndef __DrawingJack__CumberBase__
#define __DrawingJack__CumberBase__

#include "cocos2d.h"
#include "EnumDefine.h"
#include "GameData.h"
#include "OtherEffect.h"

using namespace cocos2d;

class SilenceEffect : public CCSprite
{
public:
	static SilenceEffect* create()
	{
		SilenceEffect* t_se = new SilenceEffect();
		if(t_se)
		{
			t_se->myInit();
			t_se->autorelease();
			return t_se;
		}
		CC_SAFE_DELETE(t_se);
		return NULL;
	}
	
	void startAction()
	{
		action_frame = 0;
		schedule(schedule_selector(SilenceEffect::myAction));
	}
	
private:
	int action_frame;
	
	void myAction()
	{
		action_frame++;
		if(action_frame == 20)
		{
			CCSprite* t_d1 = CCSprite::create("silence_effect_dot.png");
			t_d1->setPosition(ccp(7,11));
			addChild(t_d1, 1, 1);
		}
		else if(action_frame == 40)
		{
			CCSprite* t_d2 = CCSprite::create("silence_effect_dot.png");
			t_d2->setPosition(ccp(14,11));
			addChild(t_d2, 1, 2);
		}
		else if(action_frame == 60)
		{
			CCSprite* t_d3 = CCSprite::create("silence_effect_dot.png");
			t_d3->setPosition(ccp(21,11));
			addChild(t_d3, 1, 3);
		}
		else if(action_frame == 110)
		{
			removeAllChildrenWithCleanup(true);
			action_frame = 0;
		}
	}
	
	void myInit()
	{
		initWithFile("silence_effect_main.png");
	}
};


class Cumber : public CCNode
{
public:
	virtual void createSheild()
	{
		
	}
	
	int isSheild;
	
	void changeSpeed(bool is_down)
	{
		if(!is_slowed && is_down)
		{
			speed_change_img = SpeedChangeEffect::create(false);
			addChild(speed_change_img);
			speed_change_img->startAction();
		}
		else if(is_slowed && !is_down)
		{
			speed_change_img = SpeedChangeEffect::create(true);
			addChild(speed_change_img);
			speed_change_img->startAction();
		}
		
		is_slowed = is_down;
	}
	
	virtual ~Cumber()
	{
		
	}
	
protected:
	cumberState myState;
	CCSprite* cumberImg;
	int directionAngle;
	IntPoint mapPoint;
	CCSprite* sheildImg;
	float move_speed;
	bool is_slowed;
	SpeedChangeEffect* speed_change_img;
	float myScale;
	float maxScale;
	float minScale;
	int move_frame;
	int areacrash_frame_cnt;
	
	virtual void startChangeState()
	{
		schedule(schedule_selector(Cumber::changeState), 1);
	}
	virtual void stopChangeState()
	{
		unschedule(schedule_selector(Cumber::changeState));
	}
	
	virtual void changeState()
	{
		
	}
	
};

#endif /* defined(__DrawingJack__CumberBase__) */
