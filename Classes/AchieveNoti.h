//
//  AchieveNoti.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 31..
//
//

#ifndef __DGproto__AchieveNoti__
#define __DGproto__AchieveNoti__

#include "cocos2d.h"
#include "AchieveData.h"

USING_NS_CC;
using namespace std;

class AchieveNoti : public CCNode
{
public:
	static AchieveNoti* create(AchievementCode t_code)
	{
		AchieveNoti* t_an = new AchieveNoti();
		t_an->myInit(t_code);
		t_an->autorelease();
		return t_an;
	}
	
private:
	
	void checkAchieveNotiQueue();
	
	void autoRemoveAction()
	{
		if(getParent() != NULL)
		{
			unschedule(schedule_selector(AchieveNoti::autoRemoveAction));
			removeFromParent();
		}
	}
	
	void myInit(AchievementCode t_code);
};

#endif /* defined(__DGproto__AchieveNoti__) */
