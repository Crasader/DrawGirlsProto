//
//  RealTimeMessage.h
//  DGproto
//
//  Created by 사원3 on 2014. 10. 20..
//
//

#ifndef __DGproto__RealTimeMessage__
#define __DGproto__RealTimeMessage__

#include "cocos2d.h"
#include "AchieveData.h"

USING_NS_CC;
using namespace std;

class RealTimeMessage : public CCNode
{
public:
	static RealTimeMessage* create(string t_message, CCPoint t_base_position = ccp(240,320))
	{
		RealTimeMessage* t_an = new RealTimeMessage();
		t_an->myInit(t_message, t_base_position);
		t_an->autorelease();
		return t_an;
	}
	
private:
	
	void checkRealTimeMessageQueue();
	
	void autoRemoveAction()
	{
		if(getParent() != NULL)
		{
			unschedule(schedule_selector(RealTimeMessage::autoRemoveAction));
			removeFromParent();
		}
	}
	
	CCPoint base_position;
	void myInit(string t_message, CCPoint t_base_position);
};

#endif /* defined(__DGproto__RealTimeMessage__) */
