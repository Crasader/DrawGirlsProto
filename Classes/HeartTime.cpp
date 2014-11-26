// HeartTime.cpp
//

#include "HeartTime.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "GoodsLight.h"
#include <chrono>

#define LZZ_INLINE inline
HeartTime * HeartTime::create ()
{
	HeartTime* t_ht = new HeartTime();
	t_ht->myInit();
	t_ht->autorelease();
	return t_ht;
}

HeartTime::~HeartTime()
{
	if(mySGD->getHeartTime() == this)
		mySGD->setHeartTime(NULL);
}

bool HeartTime::startGame ()
{
	if(heart_list.size() <= 0)
	{
		return false;
	}
	else
	{
		if(heart_list.size() >= mySGD->getHeartMax())
		{
			chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
			chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
			int recent_time_second = recent_time_value.count();
			myDSH->setIntegerForKey(kDSH_Key_heartTime, recent_time_second);
		}
		
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) <= mySGD->getHeartMax())
		{
//			myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)-1);
			
			CCSprite* t_heart = heart_list.back();
			heart_list.pop_back();
			removeChild(t_heart);
			
			state_label->setString("");
		}
		else
		{
//			myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)-1);
			
			state_label->setString(CCString::createWithFormat("+%d", myDSH->getIntegerForKey(kDSH_Key_heartCnt)-mySGD->getHeartMax())->getCString());
		}
		
		return true;
	}
}

void HeartTime::refreshHeartTime()
{
	unschedule(schedule_selector(HeartTime::checkingTime));
	removeAllChildren();
	heart_list.clear();
	
	is_on_light = false;
	is_checking = false;
	int loop_cnt = myDSH->getIntegerForKey(kDSH_Key_heartCnt);
	for(int i=0;i<loop_cnt && i < mySGD->getHeartMax();i++)
	{
		CCSprite* t_heart = CCSprite::create("mainflow_heart.png");
		t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
		addChild(t_heart);
		
		heart_list.push_back(t_heart);
	}
	
	chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
	chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
	int recent_time_second = recent_time_value.count();
	int saved_time_second = myDSH->getIntegerForKey(kDSH_Key_heartTime);
	
	if(loop_cnt < mySGD->getHeartMax())
	{
		// charge
		int sub_value = recent_time_second - saved_time_second;
		int charge_cnt = sub_value/mySGD->getHeartCoolTime();
		for(int i=0;i<charge_cnt && heart_list.size() < mySGD->getHeartMax();i++)
		{
			CCSprite* t_heart = CCSprite::create("mainflow_heart.png");
			t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
			addChild(t_heart);
			
			heart_list.push_back(t_heart);
			sub_value -= mySGD->getHeartCoolTime();
		}
		
		myDSH->setIntegerForKey(kDSH_Key_heartCnt, int(heart_list.size()));
		
		if(heart_list.size() >= mySGD->getHeartMax())
			myDSH->setIntegerForKey(kDSH_Key_heartTime, recent_time_second);
		else
		{
			int keep_time_second = recent_time_second - sub_value;
			myDSH->setIntegerForKey(kDSH_Key_heartTime, keep_time_second);
			
			is_checking = true;
		}
	}
	
	if(is_checking)
	{
		int sub_value = mySGD->getHeartCoolTime() - (recent_time_second - myDSH->getIntegerForKey(kDSH_Key_heartTime));
		int minute_value = sub_value/60;
		int second_value = sub_value%60;
		state_label = CCLabelBMFont::create(CCString::createWithFormat("%d:%02d", minute_value, second_value)->getCString(), "mainflow_top_font1.fnt");
		state_label->setAnchorPoint(ccp(0.5,0.5));
		state_label->setPosition(ccp(73,-6));
		addChild(state_label);
		
		schedule(schedule_selector(HeartTime::checkingTime), 1.f);
	}
	else
	{
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) == mySGD->getHeartMax())
		{
			state_label = CCLabelBMFont::create("MAX", "mainflow_top_font1.fnt");
			state_label->setAnchorPoint(ccp(0.5,0.5));
			state_label->setPosition(ccp(73,-6));
			addChild(state_label);
		}
		else
		{
			state_label = CCLabelBMFont::create(CCString::createWithFormat("+%d", myDSH->getIntegerForKey(kDSH_Key_heartCnt)-mySGD->getHeartMax())->getCString(), "mainflow_top_font1.fnt");
			state_label->setAnchorPoint(ccp(0.5,0.5));
			state_label->setPosition(ccp(73,-6));
			addChild(state_label);
		}
	}
}

void HeartTime::backHeart ()
{
//	myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
//	CCSprite* t_heart = CCSprite::create("mainflow_heart.png");
//	t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
//	addChild(t_heart);
//	
//	if(is_on_light)
//	{
//		GoodsLight* heart_light = GoodsLight::create(CCSprite::create("mainflow_heart_mask.png"));
//		heart_light->setPosition(ccp(t_heart->getContentSize().width/2.f, t_heart->getContentSize().height/2.f));
//		t_heart->addChild(heart_light);
//	}
//	
//	heart_list.push_back(t_heart);
}

void HeartTime::onLight()
{
	is_on_light = true;
	for(int i=0;i<heart_list.size();i++)
	{
		CCSprite* t_heart = heart_list[i];
		GoodsLight* heart_light = GoodsLight::create(CCSprite::create("mainflow_heart_mask.png"));
		heart_light->setPosition(ccp(t_heart->getContentSize().width/2.f, t_heart->getContentSize().height/2.f));
		t_heart->addChild(heart_light);
	}
}

bool HeartTime::isStartable ()
{
	if(heart_list.size() <= 0)
		return false;
	else
		return true;
}
void HeartTime::checkingTime ()
{
	chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
	chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
	int recent_time_second = recent_time_value.count();
	int saved_time_second = myDSH->getIntegerForKey(kDSH_Key_heartTime);
	
	int sub_value = recent_time_second - saved_time_second;
	if(sub_value >= mySGD->getHeartCoolTime())
	{
		myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
		myDSH->setIntegerForKey(kDSH_Key_heartTime, recent_time_second);
		CCSprite* t_heart = CCSprite::create("mainflow_heart.png");
		t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
		addChild(t_heart);
		
		if(is_on_light)
		{
			GoodsLight* heart_light = GoodsLight::create(CCSprite::create("mainflow_heart_mask.png"));
			heart_light->setPosition(ccp(t_heart->getContentSize().width/2.f, t_heart->getContentSize().height/2.f));
			t_heart->addChild(heart_light);
		}
		
		heart_list.push_back(t_heart);
		
		if(heart_list.size() >= mySGD->getHeartMax())
		{
			unschedule(schedule_selector(HeartTime::checkingTime));
			state_label->setString("MAX");
		}
		else
		{
			int minute_value = mySGD->getHeartCoolTime()/60;
			int second_value = mySGD->getHeartCoolTime()%60;
			state_label->setString(CCString::createWithFormat("%d:%02d", minute_value, second_value)->getCString());
		}
	}
	else
	{
		sub_value = mySGD->getHeartCoolTime() - sub_value;
		int minute_value = sub_value/60;
		int second_value = sub_value%60 + 1;
		
		state_label->setString(CCString::createWithFormat("%d:%02d", minute_value, second_value)->getCString());
	}
}
void HeartTime::myInit ()
{
	is_on_light = false;
	setPosition(CCPointZero);
	is_checking = false;
	int loop_cnt = myDSH->getIntegerForKey(kDSH_Key_heartCnt);
	for(int i=0;i<loop_cnt && i < mySGD->getHeartMax();i++)
	{
		CCSprite* t_heart = CCSprite::create("mainflow_heart.png");
		t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
		addChild(t_heart);
		
		heart_list.push_back(t_heart);
	}
	
	chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
	chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
	int recent_time_second = recent_time_value.count();
	int saved_time_second = myDSH->getIntegerForKey(kDSH_Key_heartTime);
	
	if(loop_cnt < mySGD->getHeartMax())
	{
		// charge
		int sub_value = recent_time_second - saved_time_second;
		int charge_cnt = sub_value/mySGD->getHeartCoolTime();
		for(int i=0;i<charge_cnt && heart_list.size() < mySGD->getHeartMax();i++)
		{
			CCSprite* t_heart = CCSprite::create("mainflow_heart.png");
			t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
			addChild(t_heart);
			
			heart_list.push_back(t_heart);
			sub_value -= mySGD->getHeartCoolTime();
		}
		
		myDSH->setIntegerForKey(kDSH_Key_heartCnt, int(heart_list.size()));
		
		if(heart_list.size() >= mySGD->getHeartMax())
			myDSH->setIntegerForKey(kDSH_Key_heartTime, recent_time_second);
		else
		{
			int keep_time_second = recent_time_second - sub_value;
			myDSH->setIntegerForKey(kDSH_Key_heartTime, keep_time_second);
			
			is_checking = true;
		}
	}
	
	if(is_checking)
	{
		int sub_value = mySGD->getHeartCoolTime() - (recent_time_second - myDSH->getIntegerForKey(kDSH_Key_heartTime));
		int minute_value = sub_value/60;
		int second_value = sub_value%60 + 1;
		state_label = CCLabelBMFont::create(CCString::createWithFormat("%d:%02d", minute_value, second_value)->getCString(), "mainflow_top_font1.fnt");
		state_label->setAnchorPoint(ccp(0.5,0.5));
		state_label->setPosition(ccp(73,-6));
		addChild(state_label);
		
		schedule(schedule_selector(HeartTime::checkingTime), 1.f);
	}
	else
	{
		if(myDSH->getIntegerForKey(kDSH_Key_heartCnt) == mySGD->getHeartMax())
		{
			state_label = CCLabelBMFont::create("MAX", "mainflow_top_font1.fnt");
			state_label->setAnchorPoint(ccp(0.5,0.5));
			state_label->setPosition(ccp(73,-6));
			addChild(state_label);
		}
		else
		{
			state_label = CCLabelBMFont::create(CCString::createWithFormat("+%d", myDSH->getIntegerForKey(kDSH_Key_heartCnt)-mySGD->getHeartMax())->getCString(), "mainflow_top_font1.fnt");
			state_label->setAnchorPoint(ccp(0.5,0.5));
			state_label->setPosition(ccp(73,-6));
			addChild(state_label);
		}
	}
	
	mySGD->setHeartTime(this);
}
#undef LZZ_INLINE
