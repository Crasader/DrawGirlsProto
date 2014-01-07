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
#include "KSProtect.h"
#include <map>
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "UnknownFriends.h"
#include "ServerDataSave.h"
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
	
	int getRecentValue(AchievementCode t_code) // -1 인지 먼저 확인한 후 들어옴, condition 과 비교되는 값을 보여줌, 즉 달성률 확인용
	{
		int return_value;
		
		if(t_code == kAchievementCode_gold1 || t_code == kAchievementCode_gold2 || t_code == kAchievementCode_gold3)
		{	return_value = mySGD->getGold();	}
		else if(t_code == kAchievementCode_ruby1 || t_code == kAchievementCode_ruby2 || t_code == kAchievementCode_ruby3)
		{	return_value = mySGD->getStar();	}
		else if(t_code == kAchievementCode_heart1 || t_code == kAchievementCode_heart2 || t_code == kAchievementCode_heart3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_sendHeartCnt);	}
		else if(t_code == kAchievementCode_social1 || t_code == kAchievementCode_social2 || t_code == kAchievementCode_social3)
		{	return_value = mySGD->getFriendPoint();	}
		else if(t_code == kAchievementCode_bonusGame1 || t_code == kAchievementCode_bonusGame2 || t_code == kAchievementCode_bonusGame3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_playBonusGameCnt);	}
		else if(t_code == kAchievementCode_mapGacha1 || t_code == kAchievementCode_mapGacha2 || t_code == kAchievementCode_mapGacha3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_mapGachaCnt);	}
		else if(t_code == kAchievementCode_luckySeven1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven1);	}
		else if(t_code == kAchievementCode_luckySeven2)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven2);	}
		else if(t_code == kAchievementCode_luckySeven3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_luckySeven3);	}
		else if(t_code == kAchievementCode_feverMania1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_feverMania1);	}
		else if(t_code == kAchievementCode_feverMania2 || kAchievementCode_feverMania3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_totalFeverCnt);	}
		else if(t_code == kAchievementCode_comboMania1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_comboMania1);	}
		else if(t_code == kAchievementCode_comboMania2)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_comboMania2);	}
		else if(t_code == kAchievementCode_comboMania3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_comboMania3);	}
		else if(t_code == kAchievementCode_noFail1 || t_code == kAchievementCode_noFail2 || t_code == kAchievementCode_noFail3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_seqNoFailCnt);	}
		else if(t_code == kAchievementCode_attacker1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_attacker1);	}
		else if(t_code == kAchievementCode_attacker2)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_attacker2);	}
		else if(t_code == kAchievementCode_attacker3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_attacker3);	}
		else if(t_code == kAchievementCode_hunter1 || t_code == kAchievementCode_hunter2 || t_code == kAchievementCode_hunter3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_catchMonsterCnt);	}
		else if(t_code == kAchievementCode_dieHard1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_dieHard1);	}
		else if(t_code == kAchievementCode_dieHard2)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_dieHard2);	}
		else if(t_code == kAchievementCode_dieHard3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_dieHard3);	}
		else if(t_code == kAchievementCode_scoreHigh1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_scoreHigh1);	}
		else if(t_code == kAchievementCode_scoreHigh2)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_scoreHigh2);	}
		else if(t_code == kAchievementCode_scoreHigh3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_scoreHigh3);	}
		else if(t_code == kAchievementCode_allPiece1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_allPiece1);	}
		else if(t_code == kAchievementCode_allPiece2)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_allPiece2);	}
		else if(t_code == kAchievementCode_allPiece3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_allPiece3);	}
		else if(t_code == kAchievementCode_fastClear1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear1);	}
		else if(t_code == kAchievementCode_fastClear2)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear2);	}
		else if(t_code == kAchievementCode_fastClear3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_fastClear3);	}
		else if(t_code == kAchievementCode_continue1 || t_code == kAchievementCode_continue2 || t_code == kAchievementCode_continue3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_continueCnt);	}
		else if(t_code == kAchievementCode_attendance1 || t_code == kAchievementCode_attendance2 || t_code == kAchievementCode_attendance3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_attendanceCnt);	}
		else if(t_code == kAchievementCode_changeMania1 || t_code == kAchievementCode_changeMania2 || t_code == kAchievementCode_changeMania3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_changeCnt);	}
		else if(t_code == kAchievementCode_fail1 || t_code == kAchievementCode_fail2 || t_code == kAchievementCode_fail3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_failCnt);	}
		else if(t_code == kAchievementCode_cardCollection1 || t_code == kAchievementCode_cardCollection2 || t_code == kAchievementCode_cardCollection3)
		{
			int recent_take_card_number = 0;
			
			int take_card_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
			for(int i=1;i<=take_card_cnt;i++)
			{
				int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
				if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
					recent_take_card_number++;
			}
			
			return_value = recent_take_card_number;
		}
		else if(t_code == kAchievementCode_friend1 || t_code == kAchievementCode_friend2 || t_code == kAchievementCode_friend3)
		{
			int recent_game_friend_cnt = UnknownFriends::getInstance()->getFriends().size();
			return recent_game_friend_cnt;
		}
		else if(t_code == kAchievementCode_invite1 || t_code == kAchievementCode_invite2 || t_code == kAchievementCode_invite3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_inviteCnt);	}
		else if(t_code == kAchievementCode_challenger1 || t_code == kAchievementCode_challenger2 || t_code == kAchievementCode_challenger3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_challengeCnt);	}
		else if(t_code == kAchievementCode_help1 || t_code == kAchievementCode_help2 || t_code == kAchievementCode_help3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_helpCnt);	}
		else if(t_code == kAchievementCode_helper1 || t_code == kAchievementCode_helper2 || t_code == kAchievementCode_helper3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_helpAcceptCnt);	}
		else if(t_code == kAchievementCode_defender1 || t_code == kAchievementCode_defender2 || t_code == kAchievementCode_defender3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_challengeAcceptCnt);	}
		else if(t_code == kAchievementCode_upgrade1 || t_code == kAchievementCode_upgrade2 || t_code == kAchievementCode_upgrade3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_upgradeSuccessCnt);	}
		else if(t_code == kAchievementCode_perfect1 || t_code == kAchievementCode_perfect2 || t_code == kAchievementCode_perfect3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_perfectClearCnt);	}
		else if(t_code == kAchievementCode_seqAttendance1 || t_code == kAchievementCode_seqAttendance2 || t_code == kAchievementCode_seqAttendance3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_seqAttendanceCnt);	}
		else if(t_code == kAchievementCode_gacha1)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_gacha1Cnt);	}
		else if(t_code == kAchievementCode_gacha2)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_gacha2Cnt);	}
		else if(t_code == kAchievementCode_gacha3)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_gacha3Cnt);	}
		else if(t_code == kAchievementCode_gacha4)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_gacha4Cnt);	}
		else if(t_code == kAchievementCode_gachaKing)
		{
			return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_gachaKing);
			if(getRecentValue(kAchievementCode_gacha1) >= getCondition(kAchievementCode_gacha1) &&
			   getRecentValue(kAchievementCode_gacha2) >= getCondition(kAchievementCode_gacha2) &&
			   getRecentValue(kAchievementCode_gacha3) >= getCondition(kAchievementCode_gacha3) &&
			   getRecentValue(kAchievementCode_gacha4) >= getCondition(kAchievementCode_gacha4))
				return_value = 1;
		}
		else if(t_code == kAchievementCode_weeklyKing)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_weeklyTopCnt);	}
		else if(t_code == kAchievementCode_puzzleKing)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_puzzleKing);	}
		else if(t_code == kAchievementCode_characterCollection)
		{
			int unlocked_character_cnt = 0;
			for(int i=1;i<NSDS_GI(kSDS_GI_characterCount_i);i++)
			{
				if(myDSH->getBoolForKey(kDSH_Key_isCharacterUnlocked_int1, i))
					unlocked_character_cnt++;
			}
			return_value = unlocked_character_cnt;
		}
		else if(t_code == kAchievementCode_luckyGuy)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_startLuckyCnt);	}
		else if(t_code == kAchievementCode_goldMania)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_goldMania);	}
		else if(t_code == kAchievementCode_tutorial)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_tutorial);	}
		else if(t_code == kAchievementCode_cardSet)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_cardSet);	}
		
		else if(t_code == kAchievementCode_reviewer)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_reviewer);	}
		
		else if(t_code == kAchievementCode_hidden_returnee) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_returnee);	}
		else if(t_code == kAchievementCode_hidden_bigHand1) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_bigHand1);	}
		else if(t_code == kAchievementCode_hidden_bigHand2) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_bigHand2);	}
		else if(t_code == kAchievementCode_hidden_holder) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_holder);	}
		else if(t_code == kAchievementCode_hidden_noSound) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_noSound);	}
		else if(t_code == kAchievementCode_hidden_shopper1) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_enterShopCnt);	}
		else if(t_code == kAchievementCode_hidden_shopper2 || t_code == kAchievementCode_hidden_shopper3) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieve_itemBuyCnt);	}
		else if(t_code == kAchievementCode_hidden_breathtaking1) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_breathtaking1);	}
		else if(t_code == kAchievementCode_hidden_breathtaking2) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_breathtaking2);	}
		else if(t_code == kAchievementCode_hidden_dieEasy) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_dieEasy);	}
		else if(t_code == kAchievementCode_hidden_speedMania) // (히든)
		{	return_value = myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, kAchievementCode_hidden_speedMania);	}
		
		return return_value;
	}
	
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
