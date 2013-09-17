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
#include "KSUtil.h"
#include "CumberEmotion.h"
#include "AudioEngine.h"

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
	CUMBERSTATESTOP = 1,
	CUMBERSTATEMOVING = 1 << 1,		// 1
	CUMBERSTATEATTACKREADY = 1 << 2,	// 2
	CUMBERSTATEATTACK = 1 << 3,		// 3
	CUMBERSTATECRASHREADY = 1 << 4,	// 4
	CUMBERSTATECRASH = 1 << 5,		// 5
	CUMBERSTATEDAMAGING = 1 << 6,    // 6 맞고 있을 때...
	CUMBERSTATENODIRECTION = 1 << 7,  // 빙글 빙글...
	CUMBERSTATEDIRECTION = 1 << 8,  //   잭만 바라봐~
	CUMBERSTATEFURY = 1 << 9 // 분노모드.
};

class KSCumberBase : public CCNode
{
public:
	KSCumberBase()
//		m_state(CUMBERSTATESTOP)
	{
		
	}
	
	virtual ~KSCumberBase()
	{

	}
	virtual bool init()
	{
		CCNode::init();
		mEmotion = NULL;
		
//		MainCumber::init();
//		my_eye = NULL;
//		mEmotion = NULL;
//		map_visit_cnt = 0;
//		
//		is_silenced = false;
//		is_casting = false;
//		is_slowed = false;
//		noattack_cnt = 0;
//		isSheild = 0;
//		myState = cumberStateStop;
//		isGameover = false;
//		isFuryMode = false;
//		ableTickingTimeBomb = false;
//		is_moving_shoot = false;
//		teleportImg = NULL;
//		move_speed = 1.6f;
//
//		before_keep_frame_cnt = 0;
//		keep_number = 0;
//		attack_rate = 400.f;
//		is_attackOn = true;
//		max_visit_cnt = 8;
//		myScale = 1.5f;
//		maxScale = 1.7f;
//		minScale = 0.7f;
//		move_frame = 0;
//		areacrash_frame_cnt = 0;
		return true;
	}
	
	void startMoving()
	{
//		if(!isGameover)
		{
			
			
//			onStartMoving();
		}
	}
	void stopMoving()
	{
		
//		unschedule(schedule_selector(KSCumberBase::movingAndCrash));
//		schedule(schedule_selector(KSCumberBase::processCrash));
//		schedule(crash)
		onStopMoving();
	}
	void crashMapForIntPoint(IntPoint t_p)
	{
		IntPoint jackPoint = myGD->getJackPoint();
		
		if(t_p.isInnerMap() && (myGD->mapState[t_p.x][t_p.y] == mapOldline || myGD->mapState[t_p.x][t_p.y] == mapOldget)) // just moment, only map crash
		{
			myGD->mapState[t_p.x][t_p.y] = mapEmpty;
			for(int k = -1;k<=1;k++)
			{
				for(int l = -1;l<=1;l++)
				{
					if(k == 0 && l == 0)	continue;
					if(myGD->mapState[t_p.x+k][t_p.y+l] == mapOldget)		myGD->mapState[t_p.x+k][t_p.y+l] = mapOldline;
				}
			}
			myGD->communication("EP_crashed");
			myGD->communication("MFP_createNewFragment", t_p);
			myGD->communication("VS_divideRect", t_p);
		}
		
		if(jackPoint.x == t_p.x && jackPoint.y == t_p.y)
		{
//			showEmotion(kEmotionType_fun);
			myGD->communication("Jack_startDieEffect");
//			if(isFuryMode) //##
//				furyModeOff();
		}
		
		if(t_p.isInnerMap() && myGD->mapState[t_p.x][t_p.y] == mapNewline)
		{
			//					myGD->communication("PM_pathChainBomb", t_p);

			myGD->communication("Jack_startDieEffect");
			myGD->communication("Main_showLineDiePosition", t_p);
//			if(isFuryMode) //##
//				furyModeOff();
		}
	}
	
	void setCumberState(int e)
	{
		m_state = (CUMBER_STATE)e;
	}
	CUMBER_STATE getCumberState()
	{
		return m_state;
	}
	
	
	void showEmotion(EmotionType t_type)
	{
		if(mEmotion)
			mEmotion->selfRemove();
		mEmotion = Emotion::create(t_type, this, callfunc_selector(KSCumberBase::nullmEmotion));
		mEmotion->setPosition(ccp(30,20));
		addChild(mEmotion);
	}
	void nullmEmotion()
	{
		mEmotion = NULL;
	}
	CCNode* getBossEye() { return NULL; }
	
	virtual void furyModeOn() = 0;
	virtual void setGameover() = 0;
	virtual void cumberImgStartRotating(float gabage){} //## 임시.
	virtual void startAnimationNoDirection() = 0;
	virtual void startAnimationDirection() = 0;
	virtual void movingAndCrash(float dt) = 0;
	virtual void startDamageReaction(float userdata) = 0;
	//	virtual void startSpringCumber(float userdata) = 0;
	virtual void onStartMoving() = 0;
	virtual void onStopMoving() = 0;
	virtual void onStartGame(){} // = 0;
	//	virtual void onEndGame(){} // = 0;
	virtual void onPatternEnd() // = 0;
	{
	}
	virtual void startInvisible(){} // = 0;
	
	virtual void lightSmaller() = 0;
	
	virtual void endTeleport() = 0;
	virtual void startTeleport() = 0;
	virtual void smaller() = 0;
protected:
	CUMBER_STATE m_state;
	Emotion* mEmotion;
	
};

#endif /* defined(__DGproto__KSCumberBase__) */
