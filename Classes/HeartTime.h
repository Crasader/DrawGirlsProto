//
//  HeartTime.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 5..
//
//

#ifndef __DGproto__HeartTime__
#define __DGproto__HeartTime__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include <chrono>

USING_NS_CC;
using namespace std;

#define CHARGE_TIME 480
#define HEART_DISTANCE 20

class HeartTime : public CCNode
{
public:
	static HeartTime* create()
	{
		HeartTime* t_ht = new HeartTime();
		t_ht->myInit();
		t_ht->autorelease();
		return t_ht;
	}
	
	bool startGame()
	{
		if(heart_list.size() <= 0)
		{
			return false;
		}
		else
		{
			if(heart_list.size() >= 5)
			{
				chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
				chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
				int recent_time_second = recent_time_value.count();
				myDSH->setIntegerForKey(kDSH_Key_heartTime, recent_time_second);
			}
			
			myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)-1);
			
			CCSprite* t_heart = heart_list.back();
			heart_list.pop_back();
			removeChild(t_heart);
			
			return true;
		}
	}
	
private:
	vector<CCSprite*> heart_list;
	CCLabelTTF* state_label;
	
	bool is_checking;
	
	void checkingTime()
	{
		chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
		chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
		int recent_time_second = recent_time_value.count();
		int saved_time_second = myDSH->getIntegerForKey(kDSH_Key_heartTime);
		
		int sub_value = recent_time_second - saved_time_second;
		if(sub_value >= CHARGE_TIME)
		{
			myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
			myDSH->setIntegerForKey(kDSH_Key_heartTime, saved_time_second);
			CCSprite* t_heart = CCSprite::create("test_ui_heart.png");
			t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
			addChild(t_heart);
			
			heart_list.push_back(t_heart);
			
			if(heart_list.size() >= 5)
			{
				unschedule(schedule_selector(HeartTime::checkingTime));
				state_label->setString("MAX");
			}
			else
			{
				int minute_value = 5;
				int second_value = 0;
				state_label->setString(CCString::createWithFormat("%d:%02d", minute_value, second_value)->getCString());
			}
		}
		else
		{
			sub_value = CHARGE_TIME - sub_value;
			int minute_value = sub_value/60;
			int second_value = sub_value%60 + 1;
			
			state_label->setString(CCString::createWithFormat("%d:%02d", minute_value, second_value)->getCString());
		}
	}
	
	void myInit()
	{
		is_checking = false;
		int loop_cnt = myDSH->getIntegerForKey(kDSH_Key_heartCnt);
		for(int i=0;i<loop_cnt;i++)
		{
			CCSprite* t_heart = CCSprite::create("test_ui_heart.png");
			t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
			addChild(t_heart);
			
			heart_list.push_back(t_heart);
		}
		
		chrono::time_point<chrono::system_clock> recent_time = chrono::system_clock::now();
		chrono::duration<double> recent_time_value = recent_time.time_since_epoch();
		int recent_time_second = recent_time_value.count();
		int saved_time_second = myDSH->getIntegerForKey(kDSH_Key_heartTime);
		
		if(loop_cnt < 5)
		{
			// charge
			int sub_value = recent_time_second - saved_time_second;
			int charge_cnt = sub_value/CHARGE_TIME;
			for(int i=0;i<charge_cnt && heart_list.size() < 5;i++)
			{
				CCSprite* t_heart = CCSprite::create("test_ui_heart.png");
				t_heart->setPosition(ccp(heart_list.size()*HEART_DISTANCE,0));
				addChild(t_heart);
				
				heart_list.push_back(t_heart);
				sub_value -= CHARGE_TIME;
			}
			
			myDSH->setIntegerForKey(kDSH_Key_heartCnt, int(heart_list.size()));
			
			if(heart_list.size() >= 5)
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
			int sub_value = CHARGE_TIME - (recent_time_second - myDSH->getIntegerForKey(kDSH_Key_heartTime));
			int minute_value = sub_value/60;
			int second_value = sub_value%60 + 1;
			state_label = CCLabelTTF::create(CCString::createWithFormat("%d:%02d", minute_value, second_value)->getCString(), mySGD->getFont().c_str(), 13);
			state_label->setPosition(ccp(HEART_DISTANCE*5+15,0));
			addChild(state_label);
			
			schedule(schedule_selector(HeartTime::checkingTime), 1.f);
		}
		else
		{
			state_label = CCLabelTTF::create("MAX", mySGD->getFont().c_str(), 13);
			state_label->setPosition(ccp(HEART_DISTANCE*5+15,0));
			addChild(state_label);
		}
	}
};

#endif /* defined(__DGproto__HeartTime__) */
