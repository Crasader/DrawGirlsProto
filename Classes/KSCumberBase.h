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
#include "FromTo.h"
#include "ProbSelector.h"
#include "Well512.h"
#include "Jack.h"
#include "JsonBox.h"

USING_NS_CC;

struct SnakeTrace
{
	CCPoint position; // 자취의 위치와
	float directionRad; // 자취의 방향.
};

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

enum MOVEMENT
{
	STRAIGHT_TYPE = 1,
	RANDOM_TYPE = 2,
	FOLLOW_TYPE = 3,
	RIGHTANGLE_TYPE = 4,
	CIRCLE_TYPE = 5
};

class KSCumberBase : public CCNode
{
public:
	KSCumberBase() : m_normalMovement(RANDOM_TYPE), m_drawMovement(FOLLOW_TYPE),
	LIMIT_COLLISION_PER_SEC(3) /// 초당 변수만큼 충돌시 스케일 줄임.
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
	void crashMapForIntPoint(IntPoint t_p);
	
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
	virtual void movingAndCrash(float dt)
	{
		if(m_state == CUMBERSTATEFURY)
		{
			m_furyMode.furyFrameCount++;
		}
		
		if(gameData->getJackState() == jackStateNormal)
		{
			switch(m_normalMovement)
			{
				case STRAIGHT_TYPE:
					straightMoving(dt);
					break;
				case RANDOM_TYPE:
					randomMoving(dt);
					break;
				case FOLLOW_TYPE:
					followMoving(dt);
					break;
				case RIGHTANGLE_TYPE:
					rightAngleMoving(dt);
					break;
				case CIRCLE_TYPE:
					circleMoving(dt);
					break;
			}
		}
		else
		{
			switch(m_drawMovement)
			{
				case STRAIGHT_TYPE:
					straightMoving(dt);
					break;
				case RANDOM_TYPE:
					randomMoving(dt);
					break;
				case FOLLOW_TYPE:
					followMoving(dt);
					break;
				case RIGHTANGLE_TYPE:
					rightAngleMoving(dt);
					break;
				case CIRCLE_TYPE:
					circleMoving(dt);
					break;
			}
		}
	}
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
	virtual COLLISION_CODE getCrashCode(IntPoint point, IntPoint* checkPosition) = 0;
	void setCumberScale(float r)
	{
		m_scale.scale.init(m_scale.scale.getValue(), r, 0.005f);
	}
	float getCumberScale()
	{
		return m_scale.scale.getValue();
	}
	void randomMoving(float dt); /// 무작위 움직임.
	void straightMoving(float dt); /// 당구공 움직임.
	void followMoving(float dt); /// 따라가는 움직임.
	void rightAngleMoving(float dt); /// 직각 움직임.
	void circleMoving(float dt); /// 원형 움직임.
	virtual void crashMapForPosition(CCPoint targetPt) = 0;
	void settingScale(float startScale, float minScale, float maxScale)
	{
		m_startScale = startScale;
		m_minScale = minScale;
		m_maxScale = maxScale;
		
		m_scale.SCALE_ADDER = m_scale.SCALE_SUBER = (m_maxScale - m_minScale) / 5.f;
		m_scale.scale.init(m_startScale, m_startScale, 0.f);
	}
	void settingSpeed(float startSpeed, float minSpeed, float maxSpeed)
	{
		m_speed = m_startSpeed = startSpeed;
		m_minSpeed = minSpeed;
		m_maxSpeed = maxSpeed;
	}
	void settingMovement(enum MOVEMENT normal, enum MOVEMENT draw)
	{
		m_normalMovement = normal;
		m_drawMovement = draw;
	}
	void settingPattern(JsonBox::Object pattern)
	{
		for(auto i : pattern)
		{
			int patternNumber = atoi(i.first.c_str()); // 패턴 넘버
			int ratio = i.second["percent"].getInt();  // 빈번도
			for(int j = 0; j<ratio; j++)
			{
				m_attacks.push_back(patternNumber);
			}			
		}
	}
	void settingAttackPercent(float ap)
	{
		m_attackPercent = ap;
	}
protected:
	std::vector<int> m_attacks; // 공격할 패턴의 번호를 가지고 있음. 많이 가질 수 있을 수록 해당 패턴 쓸 확률 높음.
	const int LIMIT_COLLISION_PER_SEC; /// 초당 변수만큼 충돌시 스케일 줄임.
	CUMBER_STATE m_state;
	MOVEMENT m_normalMovement; // 평상시 움직임.
	MOVEMENT m_drawMovement;   // 땅을 그릴 때의 움직임.
	Emotion* mEmotion;
	Well512 m_well512;
	int m_directionAngleDegree;
	float m_speed;
	float m_attackPercent;
	float m_startScale, m_minScale, m_maxScale;
	float m_startSpeed, m_minSpeed, m_maxSpeed;
//	enum MOVEMENT m_normalMode, m_drawMode;
	
	struct FuryMode
	{
		
		float furyTimer;
		int furyFrameCount;
		void startFury()
		{
			
			furyTimer = 0.f;
			furyFrameCount = 0;
		}
	}m_furyMode;

	struct Scale
	{
		Scale() : SCALE_ADDER(0.1f), SCALE_SUBER(0.2f), scale(0.6f, 0.6f, 0.f),
		timer(0), autoIncreaseTimer(0), collisionStartTime(0), increaseTime(0),
		collisionCount(0){}
		float SCALE_ADDER;
		float SCALE_SUBER;
		int collisionCount; // 초당 충돌횟수 세기 위해
		float collisionStartTime;
		float timer; //
		
		float increaseTime;
		float autoIncreaseTimer;
		FromTo<float> scale; // 서서히 변하는것을 구현하기 위해.
	}m_scale;
	
	struct FollowMoving
	{
		FollowMoving() : timer(0.f){}
		float lastMapCollisionTime;
		float timer;
	}m_follow;
	
	struct CircleMoving
	{
		CircleMoving() : MIN_RADIUS(20.f), MAX_RADIUS(100.f){}
		IntPoint centerPoint;
		IntPoint relocationPoint;
		int sign;
//		float goalAngleRad; // 돌아야 하는 총 각도.
		float angleRad; // 현재 돈 각도
		const float MIN_RADIUS; // 최소 반지름
		const float MAX_RADIUS; // 최대 반지름
		void setRelocation(const CCPoint& cumberP, Well512& m_well512)
		{
			sign = m_well512.GetPlusMinus();
			bool valid = false;
			while(!valid)
			{
				float r = m_well512.GetFloatValue(MIN_RADIUS, MAX_RADIUS);
				float rad = m_well512.GetFloatValue(0, 2*M_PI);
				relocationPoint = ccp2ip(cumberP);
				centerPoint = IntPoint(r * cos(rad) + relocationPoint.x, r * sin(rad) + relocationPoint.y);
				angleRad = atan2(relocationPoint.y - centerPoint.y, relocationPoint.x - centerPoint.x);
				if(mapLoopRange::mapWidthInnerBegin <= centerPoint.x &&
				   centerPoint.y < mapLoopRange::mapWidthInnerEnd &&
				   mapLoopRange::mapHeightOutlineBegin <= centerPoint.y &&
				   centerPoint.y < mapLoopRange::mapHeightOutlineEnd)
				{				
					valid = true;
				}
				
			}			
		}
	}m_circle;
};

#endif /* defined(__DGproto__KSCumberBase__) */
