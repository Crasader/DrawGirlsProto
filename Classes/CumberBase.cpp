//
//  CumberBase.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 20..
//
//

#include "CumberBase.h"

SilenceEffect* SilenceEffect::create()
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

void SilenceEffect::startAction()
{
	action_frame = 0;
	schedule(schedule_selector(SilenceEffect::myAction));
}

void SilenceEffect::myAction()
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

void SilenceEffect::myInit()
{
	initWithFile("silence_effect_main.png");
}

void Cumber::createSheild()
{

}

void Cumber::changeSpeed( bool is_down )
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

void Cumber::startChangeState()
{
	schedule(schedule_selector(Cumber::changeState), 1);
}

void Cumber::stopChangeState()
{
	unschedule(schedule_selector(Cumber::changeState));
}

void Cumber::changeState()
{

}
