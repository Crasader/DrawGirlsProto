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
#include "hspConnector.h"
#include "StarGoldData.h"

#define myAchieve AchieveConditionReward::sharedInstance()

using namespace std;

enum AchieveRewardType
{
	kAchieveRewardType_gold,
	kAchieveRewardType_ruby,
	kAchieveRewardType_package
};

class AchieveRewardClass
{
public:
	KSProtectStr reward_type;
	KSProtectVar<int> reward_value;
	
	AchieveRewardClass(string t_type, int t_value) : reward_type(t_type), reward_value(t_value)
	{
		
	}
};

class AchieveConditionRewardData
{
public:
	
	string achieve_title;
	string achieve_content;
	KSProtectVar<int> m_type;
	bool is_noti;
	
	bool isCompleted()
	{
		return is_completed.getV();
	}
	void setComplete()
	{
		is_completed = true;
	}
	int getIngCount()
	{
		return ing_count.getV();
	}
	void setIngCount(int t_ing)
	{
		ing_count = t_ing;
	}
	int getCondition()
	{
		return condition.getV();
	}
	AchieveRewardType getRewardType()
	{
		if(reward_list.size() >= 2)
			return kAchieveRewardType_package;
		else
		{
			GoodsType t_type = mySGD->getGoodsKeyToType(reward_list[0].reward_type.getV());
			
			if(t_type == kGoodsType_gold)
				return kAchieveRewardType_gold;
			else if(t_type == kGoodsType_ruby)
				return kAchieveRewardType_ruby;
			else
				return kAchieveRewardType_gold;
		}
	}
	AchieveRewardType getRewardTypeForIndex(int t_index)
	{
		GoodsType t_type = mySGD->getGoodsKeyToType(reward_list[t_index].reward_type.getV());
		
		if(t_type == kGoodsType_gold)
			return kAchieveRewardType_gold;
		else if(t_type == kGoodsType_ruby)
			return kAchieveRewardType_ruby;
		else
			return kAchieveRewardType_gold;
	}
	int getRewardValue()
	{
		if(reward_list.size() >= 2)
			return reward_list.size();
		else
		{
			return reward_list[0].reward_value.getV();
		}
	}
	int getRewardValueForIndex(int t_index)
	{
		return reward_list[t_index].reward_value.getV();
	}
	
	AchieveConditionRewardData(AchievementCode t_type, int t_condition, string t_title, string t_content) :
	m_type(t_type), condition(t_condition), achieve_title(t_title), achieve_content(t_content)
	{
		is_noti = false;
		is_completed = false;
		ing_count = 0;
		reward_list.clear();
	}
	AchieveConditionRewardData() : m_type(0), condition(0), achieve_title(""), achieve_content("")
	{
		is_noti = false;
		is_completed = false;
		ing_count = 0;
		reward_list.clear();
	}
	
	void initReward(AchievementCode t_type);
	
private:
	KSProtectVar<bool> is_completed;
	KSProtectVar<int> ing_count;
	KSProtectVar<int> condition;
	vector<AchieveRewardClass> reward_list;
};

class AchieveGroup
{
public:
	vector<AchievementCode> achieve_list;
	
	AchieveGroup()
	{
		achieve_list.clear();
	}
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
	
	bool isNoti(AchievementCode t_code)
	{
		return data_map[t_code].is_noti;
	}
	
	void onNoti(AchievementCode t_code)
	{
		data_map[t_code].is_noti = true;
	}
	
	int getIngCount(AchievementCode t_code)
	{
		return data_map[t_code].getIngCount();
	}
	
	void changeIngCount(AchievementCode t_code, int t_ing_count);
	void changeComplete(AchievementCode t_code);
	void updateAchieve(jsonSelType t_callback);
	void resultUpdateAchieveHistory(Json::Value result_data);
	void resultUpdateAchieveHistoryNotLast(Json::Value result_data);
	vector<CommandParam> updateAchieveHistoryVectorParam(jsonSelType t_callback);
	
	int getCondition(AchievementCode t_code)
	{
		return data_map[t_code].getCondition();
	}
	
	int getPresentationCondition(AchievementCode t_code)
	{
		if(t_code == kAchievementCode_luckySeven1 || t_code == kAchievementCode_luckySeven2 || t_code == kAchievementCode_luckySeven3)
		{
			return 1;
		}
		else
			return data_map[t_code].getCondition();
	}
	
	AchieveRewardType getRewardType(AchievementCode t_code)
	{
		return data_map[t_code].getRewardType();
	}
	AchieveRewardType getRewardTypeForIndex(AchievementCode t_code, int t_index)
	{
		return data_map[t_code].getRewardTypeForIndex(t_index);
	}
	int getRewardValue(AchievementCode t_code)
	{
		return data_map[t_code].getRewardValue();
	}
	int getRewardValueForIndex(AchievementCode t_code, int t_index)
	{
		return data_map[t_code].getRewardValueForIndex(t_index);
	}
	
	string getTitle(AchievementCode t_code)
	{
		return data_map[t_code].achieve_title;
	}
	string getContent(AchievementCode t_code)
	{
		return data_map[t_code].achieve_content;
	}
	
	int isHaveRewardCount();
	
	bool isCompleted(AchievementCode t_code);
	bool isAchieve(AchievementCode t_code);
	
	int getRecentValue(AchievementCode t_code);
	
	void initAchievement();
	void initHistory(Json::Value t_list);
	
	map<int, AchieveGroup> achieve_group_list;
	AchieveGroup getAchieveGroup(AchievementCode t_code);
	AchievementCode getRecentCodeFromGroup(AchievementCode t_code);
	
	bool isChangedAchieve();
	
private:
	
	bool is_changed;
	vector<AchieveConditionRewardData> changed_data;
	map<AchievementCode, AchieveConditionRewardData> data_map;
	jsonSelType keep_callback;
	void myInit();
};

#endif /* defined(__DGproto__AchieveData__) */
