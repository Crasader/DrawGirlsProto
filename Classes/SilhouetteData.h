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

class SilhouetteData : public CCObject
{
public:
	bool silData[162][217];
	int must_cnt;
	
	static SilhouetteData* sharedSilhouetteData()
	{
		static SilhouetteData* mySD = NULL;
		if(mySD == NULL)
		{
			mySD = new SilhouetteData();
			mySD->myInit();
		}
		return mySD;
	}
	
	void setSilType(int t1)
	{
		myType = t1;
		if(myType == 1)
		{
			my_clear_condition = kCLEAR_bossLifeZero;
		}
		else
		{
			my_clear_condition = kCLEAR_default;
		}
	}
	
	int getSilType()
	{
		return myType;
	}
	
	CLEAR_CONDITION getClearCondition()
	{
		return my_clear_condition;
	}
	
	float getBossMaxLife()
	{
		float return_value;
		if(my_clear_condition == kCLEAR_bossLifeZero)
		{
			if(myType == 1)
			{
				return_value = 500;
			}
			else
			{
				return_value = 100000;
			}
		}
		else
		{
			return_value = 100000;
		}
		return return_value;
	}
	
	string getConditionTitle()
	{
		string return_value;
		
		if(my_clear_condition == kCLEAR_bossLifeZero)
		{
			return_value = "용감한 기사";
		}
		else
		{
			return_value = "";
		}
		
		return return_value;
	}
	
	string getConditionContent()
	{
		string return_value;
		
		if(my_clear_condition == kCLEAR_bossLifeZero)
		{
			return_value = "보스의 에너지를 모두 소진시켜라!";
		}
		else
		{
			return_value = "";
		}
		
		return return_value;
	}
	
	string getScriptString(int level);
	
	void startSetting();
	
	void exchangeSilhouette();
	
	
private:
	int myType;
	CLEAR_CONDITION my_clear_condition;
	
	void myInit()
	{
		
	}
	
	void startSetting1();
	void startSetting2();
	void startSetting3();
	void startSetting4();
	void startSetting5();
	
	void exchangeSilhouette1();
	void exchangeSilhouette2();
	void exchangeSilhouette3();
	void exchangeSilhouette4();
	void exchangeSilhouette5();
};

#endif /* defined(__galsprototype__SilhouetteData__) */
