//
//  KSCumberBase.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#ifndef __DGproto__KSCumberBase__
#define __DGproto__KSCumberBase__
#include "cocos2d.h"
#include "MainCumber.h"
USING_NS_CC;


enum COLLISION_CODE
{
	kCOLLISION_NONE = 0,
	kCOLLISION_JACK = 1,
	kCOLLISION_MAP = 2,
	kCOLLISION_NEWLINE = 3,
	kCOLLISION_OUTLINE = 4,
};


enum CUMBER_STATE{
	CUMBERSTATESTOP = 0,
	CUMBERSTATEMOVING,		// 1
	CUMBERSTATEATTACKREADY,	// 2
	CUMBERSTATEATTACK,		// 3
	CUMBERSTATECRASHREADY,	// 4
	CUMBERSTATECRASH		// 5
};

class KSCumberBase : public MainCumber//CCNode
{
public:
	KSCumberBase() :
		m_state(CUMBERSTATESTOP)
	{
		
	}
	
	virtual ~KSCumberBase()
	{
	
	}
	
	
	void startMoving()
	{
//		if(!isGameover)
		{
			
			
			onStartMoving();
		}
	}
	void stopMoving()
	{
		
//		unschedule(schedule_selector(KSCumberBase::movingAndCrash));
//		schedule(schedule_selector(KSCumberBase::processCrash));
//		schedule(crash)
		onStopMoving();
		
	}
	virtual void movingAndCrash(float dt) = 0;
	virtual void startAttackReaction(int userdata) = 0;
	virtual void onStartMoving() = 0;
	virtual void onStopMoving() = 0;
	void setCumberState(int e)
	{
		m_state = (CUMBER_STATE)e;
	}
	
protected:
	CUMBER_STATE m_state;
};

#endif /* defined(__DGproto__KSCumberBase__) */
