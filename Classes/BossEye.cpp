//
//  BossEye.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 5. 29..
//
//

#include "BossEye.h"

BossEye* BossEye::create(int t_type)
{
	BossEye* t_be = new BossEye();
	t_be->myInit(t_type);
	t_be->autorelease();
	return t_be;
}

void BossEye::setStep(int t_step)
{
	if(my_step%2 == 0)
		unschedule(schedule_selector(BossEye::myAnimation));
	
	my_step = t_step;
	
	if(my_step%2 == 0)
	{
		frame_cnt = 0;
		schedule(schedule_selector(BossEye::myAnimation));
	}
	else
	{
		setTextureRect(CCRectMake((my_step-1)/2*frame_size.width, 0, frame_size.width, frame_size.height));
	}
}

void BossEye::myAnimation()
{
	if(frame_cnt == 0)				setTextureRect(CCRectMake((my_step-2)/2*frame_size.width, 0, frame_size.width, frame_size.height));
	else if(frame_cnt == 30)		setTextureRect(CCRectMake(my_step/2*frame_size.width, 0, frame_size.width, frame_size.height));
	
	frame_cnt++;
	if(frame_cnt >= 40)			frame_cnt = 0;
}

void BossEye::myInit(int t_type)
{
	if(t_type == 1)
	{
		frame_size = CCSizeMake(55, 19);
		initWithFile("chapter26_boss_eye.png", CCRectMake(0, 0, frame_size.width, frame_size.height));
		setPosition(ccp(frame_size.width/2.f,30));
	}
	else if(t_type == 2)
	{
		frame_size = CCSizeMake(65, 20);
		initWithFile("chapter36_boss_eye.png", CCRectMake(0, 0, frame_size.width, frame_size.height));
		setPosition(ccp(frame_size.width/2.f,25));
	}
	else if(t_type == 3)
	{
		frame_size = CCSizeMake(55, 23);
		initWithFile("chapter46_boss_eye.png", CCRectMake(0, 0, frame_size.width, frame_size.height));
		setPosition(ccp(frame_size.width/2.f,40));
	}
	
	my_step = 1;
	setStep(1);
}