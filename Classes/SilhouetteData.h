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
	
	void startSetting();
	void startSetting1();
	void startSetting2();
	void exchangeSilhouette();
	void exchangeSilhouette1();
	void exchangeSilhouette2();
	
private:
	int myType;
	
	void myInit()
	{
		
	}
};

#endif /* defined(__galsprototype__SilhouetteData__) */
