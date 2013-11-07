//
//  SilhouetteData.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 27..
//
//

#ifndef __galsprototype__SilhouetteData__
#define __galsprototype__SilhouetteData__

#include "cocos2d.h"
#include "GameData.h"
#include "EnumDefine.h"
#include <deque>
#include "ServerDataSave.h"

USING_NS_CC;
using namespace std;

#define mySD SilhouetteData::sharedSilhouetteData()

class SilhouetteData : public CCObject
{
public:
	bool silData[162][217];
	int must_cnt;
	
	static SilhouetteData* sharedSilhouetteData()
	{
		static SilhouetteData* t_SD = NULL;
		if(t_SD == NULL)
		{
			t_SD = new SilhouetteData();
			t_SD->myInit();
		}
		return t_SD;
	}
	
	void setSilType(int t1)
	{
		myType = t1;
		
		if(isAnimationStage())
		{
			setAnimationLoop(myType);
		}
	}
	
	void setAnimationLoop(int t_type)
	{
		animation_frame.clear();
		int loop_length = NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
		for(int i=0;i<loop_length;i++)
			animation_frame.push_back(NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3), i));
	}
	
	int getSilType()
	{
		return myType;
	}
	
	int getCardDurability(int stage, int level)
	{
		return NSDS_GI(kSDS_CI_int1_durability_i, NSDS_GI(stage, kSDS_SI_level_int1_card_i, level));
	}
	
	string getCardOptionScript(int stage, int level)
	{
		string return_value;
		
		return_value = "ㅁ니우라ㅓㅁ";
		
		return return_value;
	}
	
	void setCardOptions(deque<int>& t_list, int card_number)
	{
		int ability_cnt = NSDS_GI(kSDS_CI_int1_abilityCnt_i, card_number);
		
		for(int i=0;i<ability_cnt;i++)
			t_list.push_back(NSDS_GI(kSDS_CI_int1_ability_int2_type_i, card_number, i));
	}
	
	CLEAR_CONDITION getClearCondition(){	return getClearCondition(myType);	}
	CLEAR_CONDITION getClearCondition(int t_type) // stage
	{
		return CLEAR_CONDITION(NSDS_GI(t_type, kSDS_SI_missionType_i));
	}
	
	float getBossMaxLife()
	{
		JsonBox::Value t_json;
		t_json.loadFromString(NSDS_GS(myType, kSDS_SI_boss_s));
		JsonBox::Array t_array = t_json.getArray();
		JsonBox::Object t_boss = t_array[0].getObject();
		return float(t_boss["hp"].getInt());
	}
	
	string getConditionTitle()
	{
		string return_value;
		
		CLEAR_CONDITION my_clear_condition = getClearCondition();
		
		if(my_clear_condition == kCLEAR_bossLifeZero)			return_value = "용감한 기사";
		else if(my_clear_condition == kCLEAR_subCumberCatch)	return_value = "부하몹 사냥";
		else if(my_clear_condition == kCLEAR_bigArea)			return_value = "욕심쟁이";
		else if(my_clear_condition == kCLEAR_itemCollect)		return_value = "수집가";
		else if(my_clear_condition == kCLEAR_perfect)			return_value = "완벽주의자";
		else if(my_clear_condition == kCLEAR_sequenceChange)	return_value = "결벽주의자";
		else if(my_clear_condition == kCLEAR_timeLimit)			return_value = "비지니스맨";
		else													return_value = "";
		
		return return_value;
	}
	
	string getConditionContent(){	return getConditionContent(myType);	}
	string getConditionContent(int t_type)
	{
		string return_value;
		
		t_type = getClearCondition(t_type);
		
		if(t_type == kCLEAR_bossLifeZero)				return_value = "보스의 에너지를 모두 소진시켜라!";
		else if(t_type == kCLEAR_subCumberCatch)		return_value = "부하 몬스터를 가두어 잡으세요!";
		else if(t_type == kCLEAR_bigArea)				return_value = "정해진 횟수만큼 한번에 많이 먹으세요!";
		else if(t_type == kCLEAR_itemCollect)			return_value = "정해진 숫자만큼 아이템을 모으세요!";
		else if(t_type == kCLEAR_perfect)				return_value = "정해진 목표로 정확하게 영역을 획득하세요!";
		else if(t_type == kCLEAR_sequenceChange)		return_value = "CHANGE를 순서대로 획득하세요!";
		else if(t_type == kCLEAR_timeLimit)				return_value = "목표시간 내에 클리어하세요!";
		else											return_value = "85%이상 획득하라!";
		
		return return_value;
	}
	
	int getClearConditionCatchSubCumber(){	return getClearConditionCatchSubCumber(myType);	}
	int getClearConditionCatchSubCumber(int t_type){	return NSDS_GI(t_type, kSDS_SI_missionOptionCount_i);	}
	
	float getClearConditionBigAreaPer(){	return getClearConditionBigAreaPer(myType);	}
	float getClearConditionBigAreaPer(int t_type){		return NSDS_GI(t_type, kSDS_SI_missionOptionPercent_i)/100.f;	}
	
	int getClearConditionBigAreaCnt(){	return getClearConditionBigAreaCnt(myType);	}
	int getClearConditionBigAreaCnt(int t_type){	return NSDS_GI(t_type, kSDS_SI_missionOptionCount_i);	}
	
	int getClearConditionItemCollect(){	return getClearConditionItemCollect(myType);	}
	int getClearConditionItemCollect(int t_type){	return NSDS_GI(t_type, kSDS_SI_missionOptionCount_i);	}
	
	float getClearConditionPerfectBase(){	return getClearConditionPerfectBase(myType);	}
	float getClearConditionPerfectBase(int t_type){		return NSDS_GI(t_type, kSDS_SI_missionOptionPercent_i)/100.f;	}
	
	float getClearConditionPerfectRange(){	return getClearConditionPerfectRange(myType);	}
	float getClearConditionPerfectRange(int t_type){	return 0.01f;	}
	
	int getClearConditionTimeLimit(){	return getClearConditionTimeLimit(myType);	}
	int getClearConditionTimeLimit(int t_type){		return NSDS_GI(t_type, kSDS_SI_missionOptionSec_i);	}
	
	vector<ITEM_CODE> getStageItemList(){	return getStageItemList(myType);	}
	vector<ITEM_CODE> getStageItemList(int t_type)
	{
		vector<ITEM_CODE> return_value;
		
		int item_cnt = NSDS_GI(t_type, kSDS_SI_shopItemsCnt_i);
		
		for(int i=0;i<item_cnt;i++)
			return_value.push_back(ITEM_CODE(NSDS_GI(t_type, kSDS_SI_shopItems_int1_type_i, i)));
		
		return return_value;
	}
	
	string getItemScript(ITEM_CODE t_code)
	{
		string return_value;
		
		if(t_code == kIC_attack)				return_value = "획득시 보스에게 미사일 공격을 가한다.";
		else if(t_code == kIC_speedUp)			return_value = "획득시 유저의 속도가 올라간다.";
		else if(t_code == kIC_addTime)			return_value = "획득시 남은 시간이 일정량 증가한다.";
		else if(t_code == kIC_fast)				return_value = "획득시 일시적으로 빠르게 움직인다.";
		else if(t_code == kIC_critical)			return_value = "획득시 전체 몬스터를 대상으로 공격을 가한다.";
		else if(t_code == kIC_subOneDie)		return_value = "획득시 부하 몬스터 1마리를 제거한다.";
		else if(t_code == kIC_doubleItem)		return_value = "아이템을 빠르게 생성한다.";
		else if(t_code == kIC_silence)			return_value = "획득시 보스 몬스터가 일정시간동안 공격하지 않는다.";
		else if(t_code == kIC_subNothing)		return_value = "부하 몬스터가 없이 게임을 시작한다.";
		else if(t_code == kIC_longTime)			return_value = "게임시간을 증가시킨다.";
		else if(t_code == kIC_bossLittleEnergy)	return_value = "보스 몬스터의 에너지를 낮춘다.";
		else if(t_code == kIC_subSmallSize)		return_value = "부하 몬스터의 크기를 줄인다.";
		else if(t_code == kIC_smallArea)		return_value = "한번에 먹어야될 영역의 크기를 낮춘다.";
		else if(t_code == kIC_widePerfect)		return_value = "기존 목표영역의 범위를 늘려준다.";
		else if(t_code == kIC_randomChange)		return_value = "CHANGE를 순서와 상관없이 먹어도 클리어 된다.";
		else									return_value = "아이템을 사용할 수 있습니다.";
		
		return return_value;
	}
	
	float getItemPrice(ITEM_CODE t_code)
	{
		float return_value;
		
		int shop_item_cnt = NSDS_GI(myType, kSDS_SI_shopItemsCnt_i);
		bool is_found = false;
		for(int i=0;i<shop_item_cnt && !is_found;i++)
		{
			if(t_code == NSDS_GI(myType, kSDS_SI_shopItems_int1_type_i, i))
			{
				is_found = true;
				return_value = NSDS_GI(myType, kSDS_SI_shopItems_int1_price_i, i);
			}
		}		
//		if(t_code == kIC_attack)				return_value = 250.f;
//		else if(t_code == kIC_speedUp)			return_value = 250.f;
//		else if(t_code == kIC_addTime)			return_value = 350.f;
//		else if(t_code == kIC_fast)				return_value = 700.f;
//		else if(t_code == kIC_critical)			return_value = 500.f;
//		else if(t_code == kIC_subOneDie)		return_value = 100.f;
//		else if(t_code == kIC_doubleItem)		return_value = 300.f;
//		else if(t_code == kIC_silence)			return_value = 200.f;
//		else if(t_code == kIC_subNothing)		return_value = 200.f;
//		else if(t_code == kIC_longTime)			return_value = 70.f;
//		else if(t_code == kIC_bossLittleEnergy)	return_value = 100.f;
//		else if(t_code == kIC_subSmallSize)		return_value = 70.f;
//		else if(t_code == kIC_smallArea)		return_value = 100.f;
//		else if(t_code == kIC_widePerfect)		return_value = 100.f;
//		else if(t_code == kIC_randomChange)		return_value = 100.f;
//		else									return_value = 0.f;
		
		return return_value;
	}
	
	int getSilenceItemOption(){	return NSDS_GI(myType, kSDS_SI_itemOptionSilenceSec_i);	}
	int getDoubleItemOption(){	return NSDS_GI(myType, kSDS_SI_itemOptionDoubleItemPercent_i);	}
	
	int getCardDoubleItemOption(int card_number){	return NSDS_GI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, card_number);	}
	
	int getLongTimeItemOption(){	return NSDS_GI(myType, kSDS_SI_itemOptionLongTimeSec_i);	}
	int getCardLongTimeItemOption(int card_number){	return NSDS_GI(kSDS_CI_int1_abilityLongTimeOptionSec_i, card_number);	}
	
	int getBossLittleEnergyItemOption(){	return NSDS_GI(myType, kSDS_SI_itemOptionBossLittleEnergyPercent_i);	}
	int getCardBossLittleEnergyItemOption(int card_number){		return NSDS_GI(kSDS_CI_int1_abilityBossLittleEnergyOptionPercent_i, card_number);	}
	
	int getSubSmallSizeItemOption(){	return NSDS_GI(myType, kSDS_SI_itemOptionSubSmallSizePercent_i);	}
	int getCardSubSmallSizeItemOption(int card_number){		return NSDS_GI(kSDS_CI_int1_abilitySubSmallSizeOptionPercent_i, card_number);	}
	
	int getSmallAreaItemOption(){		return NSDS_GI(myType, kSDS_SI_itemOptionSmallAreaPercent_i);	}
	int getCardSmallAreaItemOption(int card_number){	return NSDS_GI(kSDS_CI_int1_abilitySmallAreaOptionPercent_i, card_number);	}
	
	int getWidePerfectItemOption(){		return NSDS_GI(myType, kSDS_SI_itemOptionWidePerfectPercent_i);	}
	int getCardWidePerfectItemOption(int card_number){	return NSDS_GI(kSDS_CI_int1_abilityWidePerfectOptionPercent_i, card_number);	}
	
	bool isAnimationStage(){	return isAnimationStage(myType);	}
	bool isAnimationStage(int t_type){	return NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));	}
	
	CCSize getAnimationCutSize(){	return getAnimationCutSize(myType);	}
	CCSize getAnimationCutSize(int t_type)
	{
		CCSize return_value;
		
		return_value.width = NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
		return_value.height = NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
		
		return return_value;
	}
	
	int getAnimationCutLength(){	return getAnimationCutLength(myType);	}
	int getAnimationCutLength(int t_type){	return NSDS_GI(kSDS_CI_int1_aniInfoDetailCutLength_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));	}
	
	CCPoint getAnimationPosition(){	return getAnimationPosition(myType);	}
	CCPoint getAnimationPosition(int t_type)
	{
		CCPoint return_value;
		
		return_value.x = NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
		return_value.y = NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));
		
		return return_value;
	}
	
	int getAnimationLoopLength(){	return getAnimationLoopLength(myType);	}
	int getAnimationLoopLength(int t_type){	return NSDS_GI(kSDS_CI_int1_aniInfoDetailLoopLength_i, NSDS_GI(t_type, kSDS_SI_level_int1_card_i, 3));	}
	
	int getAnimationLoopPoint(int t_frame){	return animation_frame[t_frame];	}
	
	string getScriptString(int level);
	string getScriptString(int t_type, int level);
	
	void startSetting();
	
	void exchangeSilhouette();
	
	
private:
	int myType;
	deque<int> animation_frame;
	
	void myInit()
	{
		
	}
};

#endif /* defined(__galsprototype__SilhouetteData__) */
