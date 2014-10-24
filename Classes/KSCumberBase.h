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
#include "IntSeries.h"
#include "ProbSelector.h"
#include "Well512.h"
#include "jsoncpp/json.h"
#include <queue>
#include "Charges.h"


USING_NS_CC;
class CumberEmotion;
struct SnakeTrace
{
	CCPoint position; // 자취의 위치와
	float directionRad; // 자취의 방향.
	bool targeting; // 당시 타겟팅 되고 있었는지.
};
enum CumberZorder
{
	kEmotion = 15
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

// Moving 상태는 Attack/NoDirection/Direction 과 조합이 가능함. 나머지는 ㄴㄴ
typedef int CUMBER_STATE;

#define kCumberStateNothing (1)
// 일반적인 상태 돌아다닐 때, 공격은 오직 이 상태에서만 가능.

#define kCumberStateMoving (1 << 1)

// 캐스팅
#define kCumberStateCasting (1 << 2)
// 공격중일 때, 러시어택과는 좀 다름.



// 빙글 빙글...
#define kCumberStateNoDirection (1 << 6)
//   잭만 바라봐~
#define kCumberStateDirection (1 << 7)

// 맞고 있을 때...
#define kCumberStateDamaging (1 << 8)
// 분노모드.
#define kCumberStateFury (1 << 9)
// 게임오버.
#define kCumberStateGameover (1 << 10)

//#define BIT_CHECK(var, pos) (var &
// NoDirection 과 Direction 은 Casting 이나 Attack 과 함께 비트플래그로 사용됨.
enum MOVEMENT
{
	STRAIGHT_TYPE = 1,
	RANDOM_TYPE = 2,
	RIGHTANGLE_TYPE = 3,
	FOLLOW_TYPE = 4,
	
	CIRCLE_TYPE = 5,
	SNAKE_TYPE = 6,
	RUSH_TYPE = 7
};

template <typename T>
class FixedSizeDeque
{
protected:
	std::deque<T> m_deque;
	int m_maxSize;
public:
	FixedSizeDeque(int size) : m_maxSize(size){}
	FixedSizeDeque() { m_maxSize = INT_MAX; }
	const std::deque<T>& getSTL(){return m_deque;} // STL 의 큐가 필요할 때
	void push_back(const T& p);
	T& front();
	void pop_front();
	bool empty(){return m_deque.empty();}
};

struct LastPattern
{
	long exeTime;
	std::string exePattern;
};


class AttackPattern;
class CobWeb;
class SightOutAttack;
class FreezeAttack;
class ChaosAttack;
class KSCumberBase : public CCNode
{
public:
	KSCumberBase() : m_normalMovement(RANDOM_TYPE), m_drawMovement(FOLLOW_TYPE),
	LIMIT_COLLISION_PER_SEC(3), m_crashCount(0), /// 초당 변수만큼 충돌시 스케일 줄임.
	m_castingCancelCount(0), m_isStarted(false), m_healingFrameCount(0),
	m_damagedFrames(500), m_slience(false), m_adderCnt(0), m_reAttackCnt(0), m_furyCnt(0),
	m_cumberTimer(0), m_attackPattern(nullptr), m_cobWebAttack(nullptr), m_freezeAttack(nullptr),
	m_chaosAttack(nullptr), m_sightOutAttack(nullptr),
	m_speedRatioForStone(1.f), m_speedRatio(1.f),
	m_flipProperty(false), m_emotion(nullptr),
	m_stopFrameCount(0), m_castFrameCount(0), m_lastCastTime(0.f), m_deadState(false),
	m_crashAttackTime(0)
	{
		
	}
	virtual ~KSCumberBase()
	{
		CCLOG("huk hide.dfkfdjgfdsjgldfsjgldfjgldf!!!!!!!");
	}
	virtual void checkConfine(float dt) = 0;
	virtual bool init();
	
	int clear_condition;
	
	void startMoving();
	void stopMoving();
	void crashMapForIntPoint(IntPoint t_p);
	
	void setCumberState(int e);
	CUMBER_STATE getCumberState();
	CCNode* getBossEye() { return NULL; }
	void resetCastingCancelCount();
	int getCastingCancelCount();
	virtual void furyModeOn(int tf) = 0;
	virtual void setGameover();
	void bossDieBomb(float dt);
	virtual void stopCasting() = 0;
	virtual void cumberImgStartRotating(float gabage){} //## 임시.
	virtual void startAnimationNoDirection() = 0;
	virtual void startAnimationDirection(){startAnimationNoDirection();} // 기본으로 조준이 없으면 방사형으로
	virtual void stopAnimationNoDirection() = 0;
	virtual void stopAnimationDirection() = 0;
	
	virtual void movingAndCrash(float dt);
	
	virtual void cumberAttack(float dt);
	virtual void followProcess(float dt);
	virtual void timeMeasure(float dt)
	{
		m_cumberTimer += 1/60.f;
	}
	void speedAdjustment(float dt);
	void selfHealing(float dt);
	void cumberFrame(float dt);
	virtual bool startDamageReaction(float damage, float angle, bool castCancel, bool stiffen);
	//	virtual void startSpringCumber(float userdata) = 0;
	virtual void onStartMoving() = 0;
	virtual void onStopMoving() = 0;
	virtual void attackBehavior(Json::Value pattern) = 0;
	virtual void onStartGame(); // = 0;
	//	virtual void onEndGame(){} // = 0;
	virtual void onPatternEnd() // = 0;
	{
	}
	virtual void startInvisible(int totalframe){} // = 0;
	virtual void startSwell(float scale, int totalFrame);	
	virtual void lightSmaller();
	virtual void onJackDie();
	virtual void onJackRevived();
	virtual void endTeleport();
	virtual void startTeleport();
	virtual void randomPosition() = 0;
	virtual void smaller();
	virtual void onTargetingJack(CCPoint jackPosition){}
	
	COLLISION_CODE crashWithX(IntPoint check_position);
	virtual COLLISION_CODE getCrashCode(IntPoint point, IntPoint* checkPosition) = 0;
	void setCumberScale(float r);
	float getCumberScale();
	void onCanceledCasting();
	void randomMoving(float dt); /// 무작위 움직임.
	void straightMoving(float dt); /// 당구공 움직임.
	void followMoving(float dt); /// 따라가는 움직임.
	void rightAngleMoving(float dt); /// 직각 움직임.
	void circleMoving(float dt); /// 원형 움직임.
	void snakeMoving(float dt);  /// 뱀형 움직임
	void rushMoving(float dt); /// 개돌 움직임. 
	void earthwarmMoving(float dt); /// 지렁이 움직임
	virtual void crashMapForPosition(CCPoint targetPt) = 0;
	void settingScale(float startScale, float minScale, float maxScale);
	struct FuryRule
	{
		// gainPercent 이상이고, 유저와의 거리가 userDistance 이상일 때
		// 매 프레임 percent 확률로 분노함.
		float gainPercent;
		float userDistance;
		float percent;
	}m_furyRule;
	struct CloseRule
	{
		int conditionSeconds;
		CloseRule()
		{
			conditionSeconds = 6;
		}
	}m_closeRule;	
	void restoreBossData()
	{
		assignBossData(m_properties);
	}
	// 보스에게 데이터 넣음
	void assignBossData(Json::Value data);
	// ??
	void applyPassiveData(const std::string& passive);
	// 오토 밸런스 적용
	void applyAutoBalance(bool isExchange=false);
	// 분노룰 세팅
	void settingFuryRule();
	// 지능 세팅
	void settingAI(int ai);
	void settingSpeed(float startSpeed, float minSpeed, float maxSpeed);
	void settingMovement(enum MOVEMENT normal, enum MOVEMENT draw, enum MOVEMENT fury);
	// 어떤 패턴을 쓰는지 세팅함.
	void settingPattern(Json::Value pattern);
	// 피통 세팅.
	void settingHp(float hp);
	// 공격 확률 정의
	void settingAttackPercent(float ap);
	void decreaseLife(float damage);
	float getLife();
	void setLife(float t);
	float getTotalLife();
	void setTotalLife(float t);
	// 보스가 움직이는 속도의 곱하기 값
	void setSpeedRatio(float sr);
	void setSlience(bool s);
	void caughtAnimation();
	// 보스가 갇혔으면 true
	bool bossIsClosed();
	// 랜덤으로 위치를 잡음.
	void getRandomPosition(IntPoint* ip, bool* finded);
	// 플레이어 근처로 랜덤 위치 잡음.
	void getRandomPositionToJack(IntPoint* ip, bool* finded);
	virtual float getRadius() = 0;
	float aiProbAdder();
	// 잭이 선을 그리러 나갔을 때 호출됨.
	void onJackDrawLine();
	int getAiValue();
	float getAgility();
	void setAgility(float ag);
	void aggroExec();
	void unAggroExec();
	float getSpeed();
	void setSpeedRatioForStone(CCNode* stonePointer, float speedRatio);
	CCNode* getSlowStonePointer();
	virtual void attachEmotion() = 0;
	void observeStopBoss(float dt);
	bool isMovable(){
		return (m_cumberState & kCumberStateMoving) && ((m_cumberState & kCumberStateDamaging) == 0);
	}
public:
	int m_stopFrameCount;
	int m_castFrameCount;
	struct FollowMoving
	{
		FollowMoving() : timer(1.1f), lastMapCollisionTime(0), collisionCount(0){}
		float lastMapCollisionTime;
		int collisionCount;
		float timer;
		float followDegree;
		float stopTimer; //멈춤카운터
	}m_follow;
	struct Scale
	{
		Scale() : SCALE_ADDER(0.1f), SCALE_SUBER(0.2f), scale(0.6f, 0.6f, 0.f),
		timer(0), autoIncreaseTimer(0), collisionStartTime(0), increaseTime(0),
		collisionCount(0)
		{}
		float SCALE_ADDER;
		float SCALE_SUBER;
		int collisionCount; // 초당 충돌횟수 세기 위해
		float collisionStartTime;
		float timer; //
		
		float increaseTime;
		float autoIncreaseTimer;
		FromTo<float> scale; // 서서히 변하는것을 구현하기 위해.
	}m_scale;
	struct Swell
	{
		Swell() : scale(1.f), isStartSwell(false), totalFrame(0)
		{
		}
		//FromTo<float> swellScale;
		float scale;
		bool isStartSwell;
		int totalFrame;
	}m_swell;

	void swelling(float dt);
	struct Invisible
	{
		int invisibleFrame;
		int VISIBLE_FRAME;
		bool startInvisibleScheduler;
		float invisibleValue;
		Invisible() : VISIBLE_FRAME(300), startInvisibleScheduler(false){}
	}m_invisible;
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
				   centerPoint.x < mapLoopRange::mapWidthInnerEnd &&
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
		SnakeMoving() : MIN_RADIUS(80.f), MAX_RADIUS(150.f), lastMovingTime(0){}
		
		float lastMovingTime; // 마지막으로 움직인 시간을 기억함. 오랜만이라면 변수 재 설정.
		//		CCPoint centerPosition;
		//		CCPoint relocationPosition;
		int sign;
		//		float goalAngleRad; // 돌아야 하는 총 각도.
		//		float angleRad; // 현재 돈 각도
		//		float shortRadianRatio; // 짧은 반지름 비율 : (0, 1]
		const float MIN_RADIUS; // 최소 반지름
		const float MAX_RADIUS; // 최대 반지름
		
		float longRadius, shortRadius;
		CCPoint startPosition;
		CCPoint targetPosition;
		float targetTheta;
		float startXPosition;
		void setRelocation(const CCPoint& cumberP, Well512& m_well512);
		double smps(std::function<double(double)> f, double a, double b, int n)
		{
			int i;
			double h = (b - a ) / n, sum = 0;
			for(int i=0; i<n; i++)
			{
				sum += (f(a+i*h) + 4*f(a+i*h + h/2) + f(a+(i+1)*h)) * h / 6;
			}
			return sum;
		}
	}m_snake;
	MOVEMENT m_normalMovement; // 평상시 움직임.
	MOVEMENT m_originalNormalMovement;  // 평사시 움직임의 백업본.
	MOVEMENT m_drawMovement;   // 땅을 그릴 때의 움직임.
	MOVEMENT m_furyMovement;	   // 분노 모드시 움직임.
	CUMBER_STATE m_cumberState;
	vector<ChargeParent*> m_charges;
	vector<ChargeParent*>& getCharges()
	{
		return m_charges;
	}
	// 부수기 공격을 했을 때의 시간
	float m_crashAttackTime;
	float m_lastCastTime;
	float m_cumberTimer;
	struct FuryMode
	{
		int furyFrameCount;
		int totalFrame;
		int colorRef;
		int colorDir;
		void startFury(int tf)
		{
			totalFrame = tf;
			furyFrameCount = 0;
			firstMoving = true;
			colorRef = 255;
			colorDir = -1; // 처음에 레드를 제외한 다른 컬러는 감소시킴.
		}
		bool firstMoving; // 처음 분노를 시작했다면 true
	}m_furyMode;
protected:
	
	
	//선그을때 공격하는걸 제한하는 카운터
	int m_adderCnt;
	//재공격카운터
	int m_reAttackCnt;
	//분노카운터
	int m_furyCnt;
	
	Json::Value m_properties;
	bool m_isStarted;
	struct BossDie
	{
		std::vector<int> m_bossDieBombFrameNumbers;
		int m_bossDieFrameCount;
	}m_bossDie;

	
	const int LIMIT_COLLISION_PER_SEC; /// 초당 변수만큼 충돌시 스케일 줄임.
	
//	Emotion* mEmotion;
	Well512 m_well512;
	int m_directionAngleDegree;
	std::deque<int> outlineCountRatio;
	float m_attackPercent;
	float m_startScale, m_minScale, m_maxScale;
	float m_startSpeed, m_minSpeed, m_maxSpeed;
//	FromTo<float> m_speed;
	
	float m_agility;
	float m_speed;
	float m_speedRatio;
	float m_speedRatioForStone;
	CCNode* m_slowStonePointer;
	bool m_slience;
	int m_crashCount;
	int m_aiValue;
	bool m_flipProperty;
	int m_castingCancelCount; // 캐스팅이 취소당한 횟수를 셈.
	IntPoint m_mapPoint; // 자기 자신의 맵포인트를 저장함. setPosition 할 때 마다 수정해줘야함.
	int m_healingFrameCount;
//	enum MOVEMENT m_normalMode, m_drawMode;
	int m_frameCount;
	
	
	FixedSizeDeque<int> m_damagedFrames; // 맞았을 때의 프레임을 기록.
	

	
	
	
	
	
	struct EarthwarmMoving
	{
		EarthwarmMoving() : lastChangeDirectionTime(0)
		{
		}
		float angleRad; // 지금 돌고 있는 각도.
		float lastChangeDirectionTime; // 방향이 아예 바꼈을 때의 시간.
	}m_earthwarm;	
	
	struct DamageData
	{
		float m_damageX;
		float m_damageY;
		float timer;
		float stiffenSecond;
		bool setStiffen(float stiffenDuration)
		{
			// 들어오는 시간이 더 크거나 스턴이 안걸려 있으면
			if(stiffenSecond - timer < stiffenDuration || timer < stiffenSecond)
			{
				timer = 0;
				stiffenSecond = stiffenDuration;
				return true;
			}
			else
			{
				return false;
			}
		}

		
	}m_damageData;
	

	
	CC_SYNTHESIZE(LastPattern, m_lastPattern, LastPattern);
//	CC_SYNTHESIZE(AttackPattern*, m_attackPattern, AttackPattern);
    protected: AttackPattern* m_attackPattern;
    public: virtual AttackPattern* getAttackPattern(void) const { return m_attackPattern; }
    public: virtual void setAttackPattern(AttackPattern* var){ m_attackPattern = var;
    }
    

	CC_SYNTHESIZE(CobWeb*, m_cobWebAttack, CobWebAttack);
	CC_SYNTHESIZE(FreezeAttack*, m_freezeAttack, FreezeAttack);
	CC_SYNTHESIZE(ChaosAttack*, m_chaosAttack, ChaosAttack);
	CC_SYNTHESIZE(SightOutAttack*, m_sightOutAttack, SightOutAttack);
	CC_SYNTHESIZE(float, m_damageMeasure, DamageMeasure);
	CC_SYNTHESIZE(float, m_remainHp, RemainHp);
	CC_SYNTHESIZE(float, m_totalHp, TotalHp);
	CC_SYNTHESIZE(CumberEmotion*, m_emotion, Emotion);
	CC_SYNTHESIZE(bool, m_deadState, DeadState);
	CC_SYNTHESIZE(bool, m_deadTime, DeadTime);

	// 공격할 패턴의 번호를 가지고 있음. percent 가 공격을 쓸 확률
	CC_SYNTHESIZE_PASS_BY_REF(vector<Json::Value>, m_attacks, Attacks);

protected:	deque<Json::Value> m_attackQueue;
public:	deque<Json::Value>& getAttackQueue()
	{
		return m_attackQueue;
	}
//	CC_SYNTHESIZE(vector<ChargeParent*>, m_charges, Charges);
//	vector<ChargeParent*> m_charges;
//	CC_SYNTHESIZE(ChargeParent*, m_chargeParent, ChargeParent);
};

#endif /* defined(__DGproto__KSCumberBase__) */
