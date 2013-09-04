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
