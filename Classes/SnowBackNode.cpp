//
//  SnowBackNode.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 12. 16..
//
//

#include "SnowBackNode.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "KSUtil.h"

void SnowBackNode::myInit()
{
	if(mySGD->is_snow_inited)
		return;
	wind_value = 0.f;
	frame_created = 10;
	ing_frame_created = 0;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	screen_width = 480.f*screen_scale_x;
	screen_height = 320.f*myDSH->ui_top/320.f/myDSH->screen_convert_rate;
	
	CCSpriteBatchNode::initWithFile("title_santa_snow.png", kDefaultSpriteBatchCapacity);
	setPosition(ccp(240,160));
	
	mySGD->is_snow_inited = true;
}

SnowNode* SnowNode::create()
{
	SnowNode* t_node = new SnowNode();
	t_node->initWithFile("title_santa_snow.png");
	t_node->autorelease();
	return t_node;
}
void SnowNode::myInit(function<void(int)> t_func)
{
	ing_frame = 0;
	action_func = t_func;
	schedule(schedule_selector(SnowNode::mySchedule));
}

void SnowNode::mySchedule()
{
	if(ing_frame >= 60*10)
	{
		unschedule(schedule_selector(SnowNode::mySchedule));
		removeFromParent();
		return;
	}
	
	action_func(ing_frame++);
}

void SnowBackNode::snowingSchedule()
{
	wind_value = wind_value + (rand()%11-5)/100.f;
	if(wind_value > 1.5f)
		wind_value = 1.5f;
	else if(wind_value < -1.5f)
		wind_value = -1.5f;
	
	if(getChildrenCount() < 50)
	{
		if(ing_frame_created >= frame_created)
		{
			ing_frame_created = 0;
			
			SnowNode* t_snow = SnowNode::create();
			int z_value = 100 + (rand()%41 - 20);
			t_snow->setScale(z_value/100.f - 0.5f);
			t_snow->setPosition(ccp(rand()%int(screen_width + 151) - int((screen_width + 151)/2.f),screen_height/2.f));
			addChild(t_snow, z_value);
			float gravity_value = 0.7f + (rand()%41 - 20)/100.f;
			
			CCRotateBy* t_rotate = CCRotateBy::create(1.f, 80 + rand()%61);
			CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
			t_snow->runAction(t_repeat);
			
			t_snow->myInit([=](int t_i)
						   {
							   float rand_value = (rand()%21 + 50)/100.f;
							   t_snow->setPosition(t_snow->getPosition() + ccp(this->wind_value*rand_value*(100.f/z_value), -gravity_value*z_value/100.f));
							   if(t_snow->getPositionX() < -screen_width/2.f-50 || t_snow->getPositionX() > screen_width/2.f+50 || t_snow->getPositionY() < -screen_height/2.f-10)
							   {
								   t_snow->removeFromParent();
							   }
						   });
		}
		else
			ing_frame_created++;
	}
}

void SnowBackNode::onEnter()
{
	CCSpriteBatchNode::onEnter();
	schedule(schedule_selector(SnowBackNode::snowingSchedule));
}