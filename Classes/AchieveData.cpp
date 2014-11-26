//
//  AchieveData.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 31..
//
//

#include "AchieveData.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "UnknownFriends.h"
#include "ServerDataSave.h"

void AchieveConditionRewardData::initReward(AchievementCode t_code)
{
	reward_list.clear();
	
	int list_size = NSDS_GI(kSDS_AI_int1_reward_count_i, t_code);
	for(int i=0;i<list_size;i++)
	{
		string t_type = NSDS_GS(kSDS_AI_int1_reward_int2_type_s, t_code, i+1);
		int t_count = NSDS_GI(kSDS_AI_int1_reward_int2_count_i, t_code, i+1);
		
		reward_list.push_back(AchieveRewardClass(t_type, t_count));
	}
}

void AchieveConditionReward::changeIngCount(AchievementCode t_code, int t_ing_count)
{
	if(getIngCount(t_code) != t_ing_count)
	{
		bool is_found = false;
		bool is_not_same = false;
		for(int i=0;!is_found && !is_not_same && i<changed_data.size();i++)
		{
			if(changed_data[i].m_type.getV() == t_code)
			{
				is_found = true;
				if(changed_data[i].getIngCount() == t_ing_count)
					is_not_same = true;
			}
		}
		
		if(!is_found && !is_not_same)
		{
			is_changed = true;
			
			AchieveConditionRewardData t_data = AchieveConditionRewardData(t_code, 0, "", "");
			t_data.setIngCount(t_ing_count);
			
			changed_data.push_back(t_data);
		}
	}
}

bool AchieveConditionReward::isChangedAchieve()
{
	return is_changed;
}

void AchieveConditionReward::changeComplete(AchievementCode t_code)
{
	bool is_found = false;
	for(int i=0;!is_found && i<changed_data.size();i++)
	{
		if(changed_data[i].m_type.getV() == t_code)
		{
			is_found = true;
			changed_data[i].setComplete();
		}
	}
	
	if(!is_found)
	{
		is_changed = true;
		
		AchieveConditionRewardData t_data = AchieveConditionRewardData(t_code, 0, "", "");
		t_data.setComplete();
		
		changed_data.push_back(t_data);
	}
}

void AchieveConditionReward::updateAchieve(jsonSelType t_callback)
{
	if(is_changed)
	{
		vector<CommandParam> t_command_list = updateAchieveHistoryVectorParam(t_callback);
		myHSP->command(t_command_list);
	}
	else
	{
		if(t_callback != nullptr)
		{
			Json::Value result_data;
			result_data["result"]["code"] = GDSUCCESS;
			t_callback(result_data);
		}
	}
}

void AchieveConditionReward::resultUpdateAchieveHistory(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		AchievementCode t_code = AchievementCode(result_data["archiveID"].asInt());
		data_map[t_code].setIngCount(result_data.get("count", Json::Value()).asInt());
		if(result_data["rewardDate"].asInt64() != 0)
			data_map[t_code].setComplete();
		
		for(auto iter = changed_data.begin();iter != changed_data.end();iter++)
		{
			if(iter->m_type.getV() == t_code)
			{
				bool is_success = true;
				if(iter->isCompleted())
				{
					if(result_data["rewardDate"].asInt64() == 0)
						is_success = false;
				}
				
				if(iter->getIngCount() != 0)
				{
					if(result_data["count"].asInt() != iter->getIngCount())
						is_success = false;
				}
				
				if(is_success)
					changed_data.erase(iter);
				
				break;
			}
		}
	}
	else
	{
		
	}
	
	if(!keep_callbacks.empty())
	{
		if(keep_callbacks[0] != nullptr)
		{
			keep_callbacks[0](result_data);
		}
		keep_callbacks.pop_front();
	}
}

void AchieveConditionReward::resultUpdateAchieveHistoryNotLast(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		AchievementCode t_code = AchievementCode(result_data["archiveID"].asInt());
		data_map[t_code].setIngCount(result_data.get("count", Json::Value()).asInt());
		if(result_data["rewardDate"].asInt64() != 0)
			data_map[t_code].setComplete();
		
		for(auto iter = changed_data.begin();iter != changed_data.end();iter++)
		{
			if(iter->m_type.getV() == t_code)
			{
				bool is_success = true;
				if(iter->isCompleted())
				{
					if(result_data["rewardDate"].asInt64() == 0)
						is_success = false;
				}
				
				if(iter->getIngCount() != 0)
				{
					if(result_data["count"].asInt() != iter->getIngCount())
						is_success = false;
				}
				
				if(is_success)
					changed_data.erase(iter);
				
				break;
			}
		}
	}
	else
	{
		
	}
}

vector<CommandParam> AchieveConditionReward::updateAchieveHistoryVectorParam(jsonSelType t_callback)
{
	vector<CommandParam> t_command_list;
	t_command_list.clear();
	for(int i=0;i<changed_data.size();i++)
	{
		Json::Value param;
		param["memberID"] = myHSP->getSocialID();
		int t_type = changed_data[i].m_type.getV();
		param["archiveID"] = t_type;
		if(!isAchieve(AchievementCode(t_type)) && changed_data[i].getIngCount() >= getCondition(AchievementCode(t_type)))
			param["updateClearDate"] = true;
		if(changed_data[i].isCompleted())
			param["updateRewardDate"] = true;
		if(changed_data[i].getIngCount() != 0)
			param["count"] = changed_data[i].getIngCount();
		
		if(i >= changed_data.size()-1)
		{
			t_command_list.push_back(CommandParam("updatearchivementhistory", param, json_selector(this, AchieveConditionReward::resultUpdateAchieveHistory)));
			keep_callbacks.push_back(t_callback);
		}
		else
			t_command_list.push_back(CommandParam("updatearchivementhistory", param, json_selector(this, AchieveConditionReward::resultUpdateAchieveHistoryNotLast)));
	}
	
	return t_command_list;
}

AchieveGroup AchieveConditionReward::getAchieveGroup(AchievementCode t_code)
{
	for(auto iter=achieve_group_list.begin();iter!=achieve_group_list.end();iter++)
	{
		AchieveGroup t_group = iter->second;
		
		for(int j=0;j<t_group.achieve_list.size();j++)
		{
			if(t_group.achieve_list[j] == t_code)
			{
				return t_group;
			}
		}
	}
	
	return AchieveGroup();
}

AchievementCode AchieveConditionReward::getRecentCodeFromGroup(AchievementCode t_code)
{
	AchieveGroup t_group = getAchieveGroup(t_code);
	
	int state_value = -1; // complete
	
	AchievementCode recent_code;
	
	for(int i=0;i<t_group.achieve_list.size();i++)
	{
		state_value = 0;
		
		recent_code = t_group.achieve_list[i];
		if(myAchieve->isCompleted(recent_code))
			state_value = -1;
		else if(myAchieve->isAchieve(recent_code))
		{
			state_value = 1;
			break;
		}
		else
			break;
	}
	
	return recent_code;
}

void AchieveConditionReward::initAchievement()
{
	data_map.clear();
	achieve_group_list.clear();
	
	int list_size = NSDS_GI(kSDS_AI_count_i);
	for(int i=0;i<list_size;i++)
	{
		AchievementCode t_type = (AchievementCode)NSDS_GI(kSDS_AI_int1_id_i, i+1);
		string t_title = NSDS_GS(kSDS_AI_int1_title_s, t_type);
		string t_content = NSDS_GS(kSDS_AI_int1_content_s, t_type);
		int t_goal = NSDS_GI(kSDS_AI_int1_goal_i, t_type);
		
		data_map[t_type] = AchieveConditionRewardData(t_type, t_goal, t_title, t_content);
		data_map[t_type].initReward(t_type);
		
		auto iter = achieve_group_list.find(NSDS_GI(kSDS_AI_int1_groupNo_i, t_type));
		if(iter == achieve_group_list.end())
		{
			AchieveGroup t_group;
			t_group.achieve_list.push_back(t_type);
			achieve_group_list[NSDS_GI(kSDS_AI_int1_groupNo_i, t_type)] = t_group;
		}
		else
		{
			iter->second.achieve_list.push_back(t_type);
//			achieve_group_list[NSDS_GI(kSDS_AI_int1_groupNo_i, t_type)] = iter->second;
		}
	}
}

void AchieveConditionReward::initHistory(Json::Value t_list)
{
	for(int i=0;i<t_list.size();i++)
	{
		AchievementCode t_type = (AchievementCode)t_list[i]["archiveID"].asInt();
		
		if(t_list[i]["rewardDate"].asInt64() != 0)
			data_map[t_type].setComplete();
		data_map[t_type].setIngCount(t_list[i]["count"].asInt());
	}
}

int AchieveConditionReward::isHaveRewardCount()
{
	int count = 0;
	for(auto iter = data_map.begin(); iter!=data_map.end() ; iter++)
	{
		if(!(iter->second.isCompleted()) && isAchieve(iter->first))
			count++;
	}
	
	return count;
}

void AchieveConditionReward::myInit()
{
	data_map.clear();
	is_changed = false;
	changed_data.clear();
	achieve_group_list.clear();
}

bool AchieveConditionReward::isCompleted(AchievementCode t_code)
{
	return data_map[t_code].isCompleted();
}

bool AchieveConditionReward::isAchieve(AchievementCode t_code)
{
	bool return_value;
	
	return_value = getCondition(t_code) > 0 && getRecentValue(t_code) >= getCondition(t_code);
	
	return return_value;
}

int AchieveConditionReward::getRecentValue(AchievementCode t_code) // -1 인지 먼저 확인한 후 들어옴, condition 과 비교되는 값을 보여줌, 즉 달성률 확인용
{
	int return_value;
	
	if(t_code == kAchievementCode_gold1 || t_code == kAchievementCode_gold2 || t_code == kAchievementCode_gold3)
	{
		if(data_map[t_code].getIngCount() >= getCondition(t_code))
			return_value = data_map[t_code].getIngCount();
		else
			return_value = mySGD->getGoodsValue(kGoodsType_gold);
	}
	else if(t_code == kAchievementCode_ruby1 || t_code == kAchievementCode_ruby2 || t_code == kAchievementCode_ruby3)
	{
		if(data_map[t_code].getIngCount() >= getCondition(t_code))
			return_value = data_map[t_code].getIngCount();
		else
			return_value = mySGD->getGoodsValue(kGoodsType_ruby);
	}
	else if(t_code == kAchievementCode_mapGacha1 || t_code == kAchievementCode_mapGacha2 || t_code == kAchievementCode_mapGacha3)
	{	return_value = mySGD->getUserdataAchieveMapGacha();	}
	else if(t_code == kAchievementCode_luckySeven1)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_luckySeven2)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_luckySeven3)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_comboMania1)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_comboMania2)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_comboMania3)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_noFail1 || t_code == kAchievementCode_noFail2 || t_code == kAchievementCode_noFail3)
	{	return_value = mySGD->getUserdataAchieveNoFail();	}
	else if(t_code == kAchievementCode_attacker1)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_attacker2)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_attacker3)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_hunter1 || t_code == kAchievementCode_hunter2 || t_code == kAchievementCode_hunter3)
	{	return_value = mySGD->getUserdataAchieveHunter();	}
//	else if(t_code == kAchievementCode_dieHard1)
//	{	return_value = data_map[t_code].getIngCount();	}
//	else if(t_code == kAchievementCode_dieHard2)
//	{	return_value = data_map[t_code].getIngCount();	}
//	else if(t_code == kAchievementCode_dieHard3)
//	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_scoreHigh1)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_scoreHigh2)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_scoreHigh3)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_fastClear1)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_fastClear2)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_fastClear3)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_changeMania1 || t_code == kAchievementCode_changeMania2 || t_code == kAchievementCode_changeMania3)
	{	return_value = mySGD->getUserdataAchieveChangeMania();	}
	else if(t_code == kAchievementCode_fail1 || t_code == kAchievementCode_fail2 || t_code == kAchievementCode_fail3)
	{	return_value = mySGD->getUserdataAchieveFail();	}
	else if(t_code == kAchievementCode_cardCollection1 || t_code == kAchievementCode_cardCollection2 || t_code == kAchievementCode_cardCollection3)
	{
		return_value = mySGD->getHasGottenCardsSize();
	}
	else if(t_code == kAchievementCode_perfect1 || t_code == kAchievementCode_perfect2 || t_code == kAchievementCode_perfect3)
	{	return_value = mySGD->getUserdataAchievePerfect();	}
//	else if(t_code == kAchievementCode_seqAttendance1 || t_code == kAchievementCode_seqAttendance2 || t_code == kAchievementCode_seqAttendance3)
//	{	return_value = mySGD->getUserdataAchieveSeqAttendance();	}
	else if(t_code == kAchievementCode_luckyGuy)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_tutorial)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_cardSet)
	{	return_value = data_map[t_code].getIngCount();	}
	
//	else if(t_code == kAchievementCode_reviewer)
//	{	return_value = data_map[t_code].getIngCount();	}
	
//	else if(t_code == kAchievementCode_hidden_returnee) // (히든)
//	{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_returnee);	}
	else if(t_code == kAchievementCode_hidden_bigHand1 || t_code == kAchievementCode_hidden_bigHand2) // (히든)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_hidden_holder) // (히든)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_hidden_noSound) // (히든)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_hidden_shopper1) // (히든)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_hidden_shopper2 || t_code == kAchievementCode_hidden_shopper3) // (히든)
	{	return_value = mySGD->getUserdataAchieveItemBuyCount();	}
	else if(t_code == kAchievementCode_hidden_breathtaking1 || t_code == kAchievementCode_hidden_breathtaking2) // (히든)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_hidden_dieEasy) // (히든)
	{	return_value = data_map[t_code].getIngCount();	}
	else if(t_code == kAchievementCode_hidden_speedMania) // (히든)
	{	return_value = data_map[t_code].getIngCount();	}
	
	return return_value;
}