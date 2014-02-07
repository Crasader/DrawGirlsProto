//
//  AchieveData.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 31..
//
//

#ifndef __DGproto__AchieveData__
#define __DGproto__AchieveData__

#include "cocos2d.h"
#include <map>
#include "KSProtect.h"
#include "EnumDefine.h"

using namespace std;

enum AchieveRewardType
{
	kAchieveRewardType_gold = 0,
	kAchieveRewardType_ruby
};

class AchieveConditionRewardData
{
public:
	int getCondition()
	{
		return condition.getV();
	}
	AchieveRewardType getRewardType()
	{
		return (AchieveRewardType)reward_type.getV();
	}
	int getRewardValue()
	{
		return reward_value.getV();
	}
	
	AchieveConditionRewardData(int t_condition, int t_reward_type, int t_reward_value) : condition(t_condition), reward_type(t_reward_type), reward_value(t_reward_value)
	{}
	AchieveConditionRewardData() : condition(0), reward_type(0), reward_value(0)
	{}
	
private:
	KSProtectVar<int> condition;
	KSProtectVar<int> reward_type;
	KSProtectVar<int> reward_value;
};

class AchieveConditionReward
{
public:
	static AchieveConditionReward* sharedInstance()
	{
		static AchieveConditionReward* t_sg = NULL;
		if(t_sg == NULL)
		{
			t_sg = new AchieveConditionReward();
			t_sg->myInit();
		}
		return t_sg;
	}
	
	int getCondition(AchievementCode t_code)
	{
		return data_map[t_code].getCondition();
	}
	AchieveRewardType getRewardType(AchievementCode t_code)
	{
		return data_map[t_code].getRewardType();
	}
	int getRewardValue(AchievementCode t_code)
	{
		return data_map[t_code].getRewardValue();
	}
	
	bool isAchieve(AchievementCode t_code);
	
	int getRecentValue(AchievementCode t_code);
	
private:
	
	map<AchievementCode, AchieveConditionRewardData> data_map;
	
	void myInit();
};

class AchieveTitleContent
{
public:
	string title;
	string content;
	
	static AchieveTitleContent getAchievementScript(AchievementCode t_code);
};

#endif /* defined(__DGproto__AchieveData__) */
