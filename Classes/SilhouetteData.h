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
	
	int getLastUpdateStageNumber()
	{
		return SDS_GI(kSDF_gameInfo, "stage_count");
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
		int loop_length = SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_detail_loopLength", t_type*10+2)->getCString());
		for(int i=0;i<loop_length;i++)
		{
			animation_frame.push_back(SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_detail_loopSeq_%d", t_type*10+2, i)->getCString()));
		}
	}
	
	int getSilType()
	{
		return myType;
	}
	
	int getCardDurability(int stage, int level)
	{
		return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_durability", stage*10+level-1)->getCString());
	}
	
	string getCardOptionScript(int stage, int level)
	{
		string return_value;
		
		return_value = "ㅁ니우라ㅓㅁ";
		
		return return_value;
	}
	
	void setCardOptions(deque<int>& t_list, int card_number)
	{
		int ability_cnt = SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_ability_cnt", card_number)->getCString());
		
		for(int i=0;i<ability_cnt;i++)
		{
			t_list.push_back(SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_ability_%d_type", card_number, i)->getCString()));
		}
	}
	
	CLEAR_CONDITION getClearCondition(){	return getClearCondition(myType);	}
	CLEAR_CONDITION getClearCondition(int t_type) // stage
	{
		return CLEAR_CONDITION(SDS_GI(kSDF_stageInfo, t_type, "mission_type"));
	}
	
	float getBossMaxLife()
	{
		JsonBox::Value t_json;
		t_json.loadFromString(SDS_GS(kSDF_stageInfo, myType, "boss"));
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
	int getClearConditionCatchSubCumber(int t_type){	return SDS_GI(kSDF_stageInfo, t_type, "mission_option_count");	}
	
	float getClearConditionBigAreaPer(){	return getClearConditionBigAreaPer(myType);	}
	float getClearConditionBigAreaPer(int t_type){		return SDS_GI(kSDF_stageInfo, t_type, "mission_option_percent")/100.f;	}
	
	int getClearConditionBigAreaCnt(){	return getClearConditionBigAreaCnt(myType);	}
	int getClearConditionBigAreaCnt(int t_type){	return SDS_GI(kSDF_stageInfo, t_type, "mission_option_count");	}
	
	int getClearConditionItemCollect(){	return getClearConditionItemCollect(myType);	}
	int getClearConditionItemCollect(int t_type){	return SDS_GI(kSDF_stageInfo, t_type, "mission_option_count");	}
	
	float getClearConditionPerfectBase(){	return getClearConditionPerfectBase(myType);	}
	float getClearConditionPerfectBase(int t_type){		return SDS_GI(kSDF_stageInfo, t_type, "mission_option_percent")/100.f;	}
	
	float getClearConditionPerfectRange(){	return getClearConditionPerfectRange(myType);	}
	float getClearConditionPerfectRange(int t_type){	return 0.01f;	}
	
	int getClearConditionTimeLimit(){	return getClearConditionTimeLimit(myType);	}
	int getClearConditionTimeLimit(int t_type){		return SDS_GI(kSDF_stageInfo, t_type, "mission_option_sec");	}
	
	vector<ITEM_CODE> getStageItemList(){	return getStageItemList(myType);	}
	vector<ITEM_CODE> getStageItemList(int t_type)
	{
		vector<ITEM_CODE> return_value;
		
		int item_cnt = SDS_GI(kSDF_stageInfo, t_type, "shopItems_cnt");
		
		for(int i=0;i<item_cnt;i++)
			return_value.push_back(ITEM_CODE(SDS_GI(kSDF_stageInfo, t_type, CCString::createWithFormat("shopItems_%d_type", i)->getCString())));
		
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
		
		if(t_code == kIC_attack)				return_value = 10.f;
		else if(t_code == kIC_speedUp)			return_value = 10.f;
		else if(t_code == kIC_addTime)			return_value = 10.f;
		else if(t_code == kIC_fast)				return_value = 10.f;
		else if(t_code == kIC_critical)			return_value = 20.f;
		else if(t_code == kIC_subOneDie)		return_value = 10.f;
		else if(t_code == kIC_doubleItem)		return_value = 20.f;
		else if(t_code == kIC_silence)			return_value = 20.f;
		else if(t_code == kIC_subNothing)		return_value = 10.f;
		else if(t_code == kIC_longTime)			return_value = 10.f;
		else if(t_code == kIC_bossLittleEnergy)	return_value = 20.f;
		else if(t_code == kIC_subSmallSize)		return_value = 10.f;
		else if(t_code == kIC_smallArea)		return_value = 20.f;
		else if(t_code == kIC_widePerfect)		return_value = 10.f;
		else if(t_code == kIC_randomChange)		return_value = 20.f;
		else									return_value = 0.f;
		
		return return_value;
	}
	
	int getSilenceItemOption(){	return SDS_GI(kSDF_stageInfo, myType, "itemOption_silence_sec");	}
	int getDoubleItemOption(){	return SDS_GI(kSDF_stageInfo, myType, "itemOption_doubleItem_percent");	}
	
	int getCardDoubleItemOption(int card_number){	return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_ability_doubleItem_option_percent", card_number)->getCString());	}
	
	int getLongTimeItemOption(){	return SDS_GI(kSDF_stageInfo, myType, "itemOption_longTime_sec");	}
	int getCardLongTimeItemOption(int card_number){	return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_ability_longTime_option_sec", card_number)->getCString());	}
	
	int getBossLittleEnergyItemOption(){	return SDS_GI(kSDF_stageInfo, myType, "itemOption_bossLittleEnergy_percent");	}
	int getCardBossLittleEnergyItemOption(int card_number){		return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_ability_bossLittleEnergy_percent", card_number)->getCString());	}
	
	int getSubSmallSizeItemOption(){	return SDS_GI(kSDF_stageInfo, myType, "itemOption_subSmallSize_percent");	}
	int getCardSubSmallSizeItemOption(int card_number){		return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_ability_subSmallSize_percent", card_number)->getCString());	}
	
	int getSmallAreaItemOption(){		return SDS_GI(kSDF_stageInfo, myType, "itemOption_smallArea_percent");	}
	int getCardSmallAreaItemOption(int card_number){	return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_ability_smallArea_percent", card_number)->getCString());	}
	
	int getWidePerfectItemOption(){		return SDS_GI(kSDF_stageInfo, myType, "itemOption_widePerfect_percent");	}
	int getCardWidePerfectItemOption(int card_number){	return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_ability_widePerfect_percent", card_number)->getCString());	}
	
	bool isAnimationStage(){	return isAnimationStage(myType);	}
	bool isAnimationStage(int t_type){	return SDS_GB(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_isAni", t_type*10+2)->getCString());	}
	
	CCSize getAnimationCutSize(){	return getAnimationCutSize(myType);	}
	CCSize getAnimationCutSize(int t_type)
	{
		CCSize return_value;
		
		return_value.width = SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_detail_cutWidth", t_type*10+2)->getCString());
		return_value.height = SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_detail_cutHeight", t_type*10+2)->getCString());
		
		return return_value;
	}
	
	int getAnimationCutLength(){	return getAnimationCutLength(myType);	}
	int getAnimationCutLength(int t_type){	return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_detail_cutLength", t_type*10+2)->getCString());	}
	
	CCPoint getAnimationPosition(){	return getAnimationPosition(myType);	}
	CCPoint getAnimationPosition(int t_type)
	{
		CCPoint return_value;
		
		return_value.x = SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_detail_positionX", t_type*10+2)->getCString());
		return_value.y = SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_detail_positionY", t_type*10+2)->getCString());
		
		return return_value;
	}
	
	int getAnimationLoopLength(){	return getAnimationLoopLength(myType);	}
	int getAnimationLoopLength(int t_type){	return SDS_GI(kSDF_cardInfo, CCString::createWithFormat("%d_aniInfo_detail_loopLength", t_type*10+2)->getCString());	}
	
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
