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
//#include "CumberEmotion.h"
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

struct AttackProperty
{
	enum AttackDisposition
	{
		SPECIAL, NODIR, DIR
	}category;
	std::string property; // 속성들... json 형태로 저장될거임.
	
	bool operator==(const AttackProperty& ap)
	{
		return this->category == ap.category && this->property == ap.property;
	}
	
	bool operator<(const AttackProperty& ap) const
	{
		return this->property < ap.property;
	}
	AttackProperty(enum AttackDisposition ad, const std::string& ac) : category(ad), property(ac)
	{}
	AttackProperty(){}
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
	CUMBERSTATEFURY = 1 << 9, // 분노모드.
	CUMBERSTATEGAMEOVER = 1 << 10 // 게임오버.
};

enum MOVEMENT
{
	STRAIGHT_TYPE = 1,
	RANDOM_TYPE = 2,
	FOLLOW_TYPE = 3,
	RIGHTANGLE_TYPE = 4,
	CIRCLE_TYPE = 5,
	SNAKE_TYPE = 6
};

class KSCumberBase : public CCNode
{
public:
	KSCumberBase() : m_normalMovement(RANDOM_TYPE), m_drawMovement(FOLLOW_TYPE),
	LIMIT_COLLISION_PER_SEC(3), m_crashCount(0), /// 초당 변수만큼 충돌시 스케일 줄임.
	m_castingCancelCount(0)
	
//		m_state(CUMBERSTATESTOP)
	{
		
	}
	
	virtual ~KSCumberBase()
	{

	}
	virtual bool init()
	{
		CCNode::init();
//		mEmotion = NULL;
		schedule(schedule_selector(ThisClassType::speedAdjustment));
		
	
		
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
	
	
//	void showEmotion(EmotionType t_type)
//	{
//		if(mEmotion)
//			mEmotion->selfRemove();
//		mEmotion = Emotion::create(t_type, this, callfunc_selector(KSCumberBase::nullmEmotion));
//		mEmotion->setPosition(ccp(30,20));
//		addChild(mEmotion);
//	}
//	void nullmEmotion()
//	{
//		mEmotion = NULL;
//	}
	CCNode* getBossEye() { return NULL; }
	void resetCastingCancelCount()
	{
		m_castingCancelCount = 0;
	}
	int getCastingCancelCount()
	{
		return m_castingCancelCount;
	}
	virtual void furyModeOn(int tf) = 0;
	virtual void setGameover()
	{
		m_state = CUMBERSTATEGAMEOVER;
		
//		m_scale.scale.init(m_scale.scale.getValue(), 0.f, 0.03f);
//		runAction(CCScaleTo::create(2.f, 0.01f));
		m_minScale = 0.f;
		m_bossDie.m_bossDieBombFrameNumbers.push_back(m_well512.GetValue(0, 30));
		m_bossDie.m_bossDieBombFrameNumbers.push_back(m_well512.GetValue(30, 60));
		m_bossDie.m_bossDieBombFrameNumbers.push_back(m_well512.GetValue(60, 90));
		m_bossDie.m_bossDieFrameCount = 0;
		schedule(schedule_selector(ThisClassType::bossDieBomb));
//		int number = m_well512.GetValue(3, 4);
//		for(int i=0; i<number; i++)
//		{
//			scheduleOnce(schedule_selector(ThisClassType::bossDieBomb), m_well512.GetFloatValue(0.3f, 1.f));
//		}	
	}
	void bossDieBomb(float dt);
	virtual void cumberImgStartRotating(float gabage){} //## 임시.
	virtual void startAnimationNoDirection() = 0;
	virtual void startAnimationDirection(){startAnimationNoDirection();} // 기본으로 조준이 없으면 방사형으로
	virtual void stopAnimationNoDirection() = 0;
	virtual void stopAnimationDirection() = 0;
	
	virtual void movingAndCrash(float dt)
	{
		IntPoint mapPoint = m_mapPoint;
		
		// 갇혀있는지 검사함. 갇혀있으면 없앰.
		if(myGD->mapState[mapPoint.x][mapPoint.y] != mapEmpty &&
		   myGD->mapState[mapPoint.x-1][mapPoint.y] != mapEmpty &&
		   myGD->mapState[mapPoint.x+1][mapPoint.y] != mapEmpty &&
		   myGD->mapState[mapPoint.x][mapPoint.y-1] != mapEmpty &&
		   myGD->mapState[mapPoint.x][mapPoint.y+1] != mapEmpty)
		{
			
			
			myGD->communication("CP_removeSubCumber", this);
			
			if(mySD->getClearCondition() == kCLEAR_subCumberCatch)
			{
				caughtAnimation();
			}
			else
			{
				removeFromParentAndCleanup(true);
			}
			return;
		}
		
		if(m_state == CUMBERSTATEFURY)
		{
			m_furyMode.furyFrameCount++;
		}	

		auto movingBranch = [&](MOVEMENT movement)
		{
			switch(movement)
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
				case SNAKE_TYPE:
					snakeMoving(dt);
			}
		};
		
		if(m_state == CUMBERSTATEFURY)
		{
			movingBranch(m_furyMovement);
		}
		else
		{
			if(myGD->getJackState() == jackStateNormal)
			{
				movingBranch(m_normalMovement);
			}
			else
			{
				movingBranch(m_drawMovement);
			}
		}
		
		
		
	}
	
	virtual void cumberAttack(float dt);
	void speedAdjustment(float dt);
	virtual bool startDamageReaction(float damage, float angle);
	//	virtual void startSpringCumber(float userdata) = 0;
	virtual void onStartMoving() = 0;
	virtual void onStopMoving() = 0;
	virtual void attackBehavior(Json::Value pattern) = 0;
	virtual void onStartGame(){} // = 0;
	//	virtual void onEndGame(){} // = 0;
	virtual void onPatternEnd() // = 0;
	{
	}
	virtual void startInvisible(int totalframe){} // = 0;
	
	virtual void lightSmaller() = 0;
	
	virtual void endTeleport() = 0;
	virtual void startTeleport() = 0;
	virtual void smaller() = 0;
	virtual void onTargetingJack(CCPoint jackPosition){}
	COLLISION_CODE crashWithX(IntPoint check_position)
	{
		if(check_position.x < mapLoopRange::mapWidthInnerBegin || check_position.x >= mapLoopRange::mapWidthInnerEnd ||
			 check_position.y < mapLoopRange::mapHeightInnerBegin || check_position.y >= mapLoopRange::mapHeightInnerEnd ||
			 myGD->mapState[check_position.x][check_position.y] == mapType::mapOutline)
		{
			// 나갔을 시.
			return COLLISION_CODE::kCOLLISION_OUTLINE;
		}
		
		// 이미 그려진 곳에 충돌했을 경우.
		if(myGD->mapState[check_position.x][check_position.y] == mapOldline ||
			 myGD->mapState[check_position.x][check_position.y] == mapOldget)
		{
			return COLLISION_CODE::kCOLLISION_MAP;
		}
		
		if(myGD->mapState[check_position.x][check_position.y] == mapNewline)
		{
			return COLLISION_CODE::kCOLLISION_NEWLINE;
		}
		IntPoint jackPoint = myGD->getJackPoint();
		if(jackPoint.x == check_position.x && jackPoint.y == check_position.y)
		{
			return COLLISION_CODE::kCOLLISION_JACK;
		}
		return COLLISION_CODE::kCOLLISION_NONE;
	}
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
	void snakeMoving(float dt);  /// 뱀형 움직임
	virtual void crashMapForPosition(CCPoint targetPt) = 0;
	void settingScale(float startScale, float minScale, float maxScale)
	{
		m_startScale = startScale;
		m_minScale = minScale;
		m_maxScale = maxScale;
		
		m_scale.SCALE_ADDER = m_scale.SCALE_SUBER = (m_maxScale - m_minScale) / 5.f;
		m_scale.scale.init(m_startScale, m_startScale, 0.f);
	}
	struct FuryRule
	{
		// gainPercent 이상이고, 유저와의 거리가 userDistance 이상일 때
		// 매 프레임 percent 확률로 분노함.
		float gainPercent;
		float userDistance;
		float percent;
		
		// ltPercent 보다 작고 gtCount 보다 클 때 분노함.
		float ltPercent;
		float gtCount;
	}m_furyRule;
	void settingFuryRule(Json::Value fury)
	{
		m_furyRule.gainPercent = fury["gainpercent"].asDouble();
		m_furyRule.userDistance = fury["userdistance"].asDouble();
		m_furyRule.percent = fury["percent"].asDouble();
		m_furyRule.ltPercent = fury["ltpercent"].asDouble();
		m_furyRule.gtCount = fury["gtcount"].asDouble();
//		m_furyRule
	}
	void settingAI(int ai)
	{
		m_aiValue = ai;
	}
	void settingSpeed(float startSpeed, float minSpeed, float maxSpeed)
	{
		m_speed = m_startSpeed = startSpeed;
//		m_speed.init(m_startSpeed, m_startSpeed, 0.1f);

		m_minSpeed = minSpeed;
		m_maxSpeed = maxSpeed;
	}
	void settingMovement(enum MOVEMENT normal, enum MOVEMENT draw, enum MOVEMENT fury)
	{
		m_normalMovement = normal;
		m_drawMovement = draw;
		m_furyMovement = fury;
	}
	void settingPattern(Json::Value pattern)
	{
		for(auto iter = pattern.begin(); iter != pattern.end(); ++iter)
		{
//			int ratio = (*iter)["percent"].asInt(); // 빈번도
//			for(int j = 0; j<ratio; j++)
			{
				m_attacks.push_back(pattern[iter.index()]);
			}
		}
		
		for(auto i : m_attacks)
		{
			KS::KSLog("%", i);
		}
	}
	void settingHp(float hp)
	{
		m_remainHp = m_totalHp = hp;
	}
	void settingAttackPercent(float ap)
	{
		m_attackPercent = ap;
	}
	void decreaseLife(float damage)
	{
		m_remainHp -= damage;
		if(m_remainHp <= 0)
		{
			myGD->communication("CP_removeSubCumber", this);
		}
	}
	float getLife()
	{
		return m_remainHp;
	}
	void setLife(float t)
	{
		m_remainHp = t;
	}
	float getTotalLife()
	{
		return m_totalHp;
	}
	void setTotalLife(float t)
	{
		m_totalHp = t;
	}
	void setSpeedRatio(float sr)
	{
		m_speedRatio = sr;
	}
	void setSlience(bool s)
	{
		m_slience = s;
	}
	void caughtAnimation()
	{
		myGD->communication("UI_catchSubCumber");
		myGD->communication("CP_createSubCumber", myGD->getMainCumberPoint());
	}
	// 보스가 갇혔으면 true
	bool bossIsClosed()
	{
		int greaterNumber = count_if(outlineCountRatio.begin(), outlineCountRatio.end(), [](int i){return i >= 20;} );
		bool closedBoss = false;
		if((float)greaterNumber / (float)outlineCountRatio.size() >= 0.8f)
		{
			closedBoss = true;
		}
		return closedBoss;
	}
protected:
		
	struct BossDie
	{
		std::vector<int> m_bossDieBombFrameNumbers;
		int m_bossDieFrameCount;
	}m_bossDie;
	
	std::vector<Json::Value> m_attacks; // 공격할 패턴의 번호를 가지고 있음. percent 가 공격을 쓸 확률
	const int LIMIT_COLLISION_PER_SEC; /// 초당 변수만큼 충돌시 스케일 줄임.
	CUMBER_STATE m_state;
	MOVEMENT m_normalMovement; // 평상시 움직임.
	MOVEMENT m_drawMovement;   // 땅을 그릴 때의 움직임.
	MOVEMENT m_furyMovement;	   // 분노 모드시 움직임.
//	Emotion* mEmotion;
	Well512 m_well512;
	int m_directionAngleDegree;
	std::deque<int> outlineCountRatio;
	float m_attackPercent;
	float m_startScale, m_minScale, m_maxScale;
	float m_startSpeed, m_minSpeed, m_maxSpeed;
//	FromTo<float> m_speed;
	float m_speed;
	float m_remainHp;
	float m_totalHp;
	float m_speedRatio;
	bool m_slience;
	int m_crashCount;
	int m_aiValue;
	int m_castingCancelCount; // 캐스팅이 취소당한 횟수를 셈.
	IntPoint m_mapPoint; // 자기 자신의 맵포인트를 저장함. setPosition 할 때 마다 수정해줘야함.
//	enum MOVEMENT m_normalMode, m_drawMode;
	
	struct FuryMode
	{
		int furyFrameCount;
		int totalFrame;
		void startFury(int tf)
		{
			totalFrame = tf;
		}
	}m_furyMode;

	struct Invisible
	{
		int invisibleFrame;
		int VISIBLE_FRAME;
		bool startInvisibleScheduler;
		float invisibleValue;
		Invisible() : VISIBLE_FRAME(300), startInvisibleScheduler(false){}
	}m_invisible;
	
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
		CircleMoving() : MIN_RADIUS(20.f), MAX_RADIUS(100.f), lastMovingTime(0){}
		
		float lastMovingTime; // 마지막으로 움직인 시간을 기억함. 오랜만이라면 변수 재 설정.
		CCPoint centerPosition;
		CCPoint relocationPosition;
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
				relocationPosition = cumberP;
				centerPosition = ccp(r * cos(rad) + relocationPosition.x, r * sin(rad) + relocationPosition.y);
				angleRad = 0;//atan2(relocationPosition.y - centerPosition.y, relocationPosition.x - centerPosition.x);
				
				IntPoint centerPoint = ccp2ip(centerPosition);
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
	
	struct SnakeMoving
	{
		SnakeMoving() : MIN_RADIUS(50.f), MAX_RADIUS(100.f), lastMovingTime(0){}
		
		float lastMovingTime; // 마지막으로 움직인 시간을 기억함. 오랜만이라면 변수 재 설정.
		CCPoint centerPosition;
		CCPoint relocationPosition;
		int sign;
		//		float goalAngleRad; // 돌아야 하는 총 각도.
		float angleRad; // 현재 돈 각도
		float shortRadianRatio; // 짧은 반지름 비율 : (0, 1]
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
				shortRadianRatio = m_well512.GetFloatValue(0.0001f, 1.f);
				relocationPosition = cumberP;
				centerPosition = ccp(r * cos(rad) + relocationPosition.x, r * sin(rad) + relocationPosition.y);
				angleRad = 0;//atan2(relocationPosition.y - centerPosition.y, relocationPosition.x - centerPosition.x);
				
				IntPoint centerPoint = ccp2ip(centerPosition);
				if(mapLoopRange::mapWidthInnerBegin <= centerPoint.x &&
				   centerPoint.y < mapLoopRange::mapWidthInnerEnd &&
				   mapLoopRange::mapHeightOutlineBegin <= centerPoint.y &&
				   centerPoint.y < mapLoopRange::mapHeightOutlineEnd)
				{
					valid = true;
				}
				
			}
		}
	}m_snake;
};

#endif /* defined(__DGproto__KSCumberBase__) */
