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
		return 7;
	}
	
	void setSilType(int t1)
	{
		myType = t1;
	}
	
	int getSilType()
	{
		return myType;
	}
	
	CLEAR_CONDITION getClearCondition(){	return getClearCondition(myType);	}
	CLEAR_CONDITION getClearCondition(int t_type) // stage
	{
		CLEAR_CONDITION return_value;
		
		if(t_type == 1)				return_value = kCLEAR_bossLifeZero;
		else if(t_type == 2)		return_value = kCLEAR_subCumberCatch;
		else if(t_type == 3)		return_value = kCLEAR_bigArea;
		else if(t_type == 4)		return_value = kCLEAR_itemCollect;
		else if(t_type == 5)		return_value = kCLEAR_perfect;
		else if(t_type == 6)		return_value = kCLEAR_sequenceChange;
		else						return_value = kCLEAR_default;
		
		return return_value;
	}
	
	float getBossMaxLife()
	{
		float return_value;
		
		CLEAR_CONDITION my_clear_condition = getClearCondition();
		
		if(my_clear_condition == kCLEAR_bossLifeZero)
		{
			if(myType == 1)				return_value = 500;
			else						return_value = 100000;
		}
		else	return_value = 100000;
		
		return return_value;
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
		
		if(t_type == kCLEAR_bossLifeZero)				return_value = "보스의 에너지를 모두 소진시켜라!";
		else if(t_type == kCLEAR_subCumberCatch)		return_value = "부하 몬스터를 가두어 잡으세요!";
		else if(t_type == kCLEAR_bigArea)				return_value = "정해진 횟수만큼 한번에 많이 먹으세요!";
		else if(t_type == kCLEAR_itemCollect)			return_value = "정해진 숫자만큼 아이템을 모으세요!";
		else if(t_type == kCLEAR_perfect)				return_value = "정해진 목표로 정확하게 영역을 획득하세요!";
		else if(t_type == kCLEAR_sequenceChange)		return_value = "CHANGE를 순서대로 획득하세요!";
		else if(t_type == kCLEAR_timeLimit)				return_value = "목표시간 내에 클리어하세요!";
		else											return_value = "";
		
		return return_value;
	}
	
	int getClearConditionCatchSubCumber(){	return getClearConditionCatchSubCumber(myType);	}
	int getClearConditionCatchSubCumber(int t_type)
	{
		int return_value;
		
		if(t_type == 2)		return_value = 5;
		else				return_value = 0;
		
		return return_value;
	}
	
	float getClearConditionBigAreaPer(){	return getClearConditionBigAreaPer(myType);	}
	float getClearConditionBigAreaPer(int t_type)
	{
		float return_value;
		
		if(t_type == 3)		return_value = 0.1f;
		else				return_value = 0;
		
		return return_value;
	}
	
	int getClearConditionBigAreaCnt(){	return getClearConditionBigAreaCnt(myType);	}
	int getClearConditionBigAreaCnt(int t_type)
	{
		int return_value;
		
		if(t_type == 3)		return_value = 3;
		else				return_value = 0;
		
		return return_value;
	}
	
	int getClearConditionItemCollect(){	return getClearConditionItemCollect(myType);	}
	int getClearConditionItemCollect(int t_type)
	{
		int return_value;
		
		if(t_type == 4)		return_value = 10;
		else				return_value = 0;
		
		return return_value;
	}
	
	float getClearConditionPerfectBase(){	return getClearConditionPerfectBase(myType);	}
	float getClearConditionPerfectBase(int t_type)
	{
		float return_value;
		
		if(t_type == 5)		return_value = 0.87f;
		else				return_value = 0;
		
		return return_value;
	}
	
	float getClearConditionPerfectRange(){	return getClearConditionPerfectRange(myType);	}
	float getClearConditionPerfectRange(int t_type)
	{
		float return_value;
		
		if(t_type == 5)		return_value = 0.01f;
		else				return_value = 0;
		
		return return_value;
	}
	
	string getScriptString(int level);
	
	void startSetting();
	
	void exchangeSilhouette();
	
	
private:
	int myType;
	
	void myInit()
	{
		
	}
	
	void startSetting1();
	void startSetting2();
	void startSetting3();
	void startSetting4();
	void startSetting5();
	void startSetting6();
	void startSetting7();
	void startSetting8();
	
	void exchangeSilhouette1();
	void exchangeSilhouette2();
	void exchangeSilhouette3();
	void exchangeSilhouette4();
	void exchangeSilhouette5();
	void exchangeSilhouette6();
	void exchangeSilhouette7();
	void exchangeSilhouette8();
};

#endif /* defined(__galsprototype__SilhouetteData__) */
