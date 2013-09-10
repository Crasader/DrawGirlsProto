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
	}
	
	int getSilType()
	{
		return myType;
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
	
	void exchangeSilhouette1();
	void exchangeSilhouette2();
	void exchangeSilhouette3();
	void exchangeSilhouette4();
	void exchangeSilhouette5();
};

#endif /* defined(__galsprototype__SilhouetteData__) */
