//
//  AttackPattern.h
//  DrawingJack
//
//  Created by 사원3 on 12. 12. 21..
//
//

#ifndef DrawingJack_AttackPattern_h
#define DrawingJack_AttackPattern_h

#include "cocos2d.h"
#include "MissileUnit.h"
#include "GameData.h"
#include <deque>
#include "AudioEngine.h"
#include "KSUtil.h"
#include "Well512.h"
#include "ServerDataSave.h"
#include "KSCumberBase.h"
#include "jsoncpp/json.h"
#include "FromTo.h"
#include "jsoncpp/json.h"


using namespace cocos2d;
using namespace std;

#define CREATE_FUNC_CCP(A) static A* create(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData ) \
{ \
	A* t_m0 = new A(); \
	t_m0->myInit(t_sp, cb, patternData); \
	t_m0->autorelease(); \
	return t_m0; \
} \


#define CREATE_FUNC_CCP_JSON(A) static A* create(CCPoint t_sp, KSCumberBase* cb, const Json::Value& patternJson ) \
{ \
A* t_m0 = new A(); \
t_m0->myInit(t_sp, cb, patternJson); \
t_m0->autorelease(); \
return t_m0; \
} \

class AttackPattern : public CCNode
{
public:
	
	void startSelfRemoveSchedule(int baseChild = 0);
	
	virtual void stopMyAction(){}
	
	void setStartingWithEarly()
	{
		// 캐스팅 끝나고 몬스터 상태가 릴리즈가 될것이냐... 안될것이냐
		if(m_earlyRelease)
		{
			m_cumber->setAttackPattern(nullptr);
			//cumber->setAttackPattern(t_m9);
			
			
			myGD->communication("CP_onPatternEndOf", m_cumber);
		}
		else
		{
			m_cumber->setAttackPattern(this);
		}
	}
	void setEndingWithEarly()
	{
		if(m_earlyRelease)
		{
			// 이미 릴리즈 되서 아무행동도 하지 않음.
		}
		else
		{
			m_cumber->setAttackPattern(nullptr);
			myGD->communication("CP_onPatternEndOf", m_cumber);
		}
	}
protected:
	
	virtual void selfRemoveSchedule();
	
	KSCumberBase* m_cumber;
	int m_baseChildCount;
	int m_earlyRelease;
	
};

class SelfSpinMissile : public CCNode
{
public:
	static SelfSpinMissile* create(CCPoint t_sp, CCPoint t_dv, int t_mCnt, float t_r, string imgFilename, CCSize t_mSize);
	
private:
	CCPoint dv;
	float r;
	
	void startMove();
	void stopMove();
	void move();
	
	void myInit(CCPoint t_sp, CCPoint t_dv, int t_mCnt, float t_r, string imgFilename, CCSize t_mSize);
};





class FallingStoneWrapper : public AttackPattern
{
public:
	static FallingStoneWrapper* create(KSCumberBase* cb, Json::Value pattern, CCSize mSize, int t_type);
	
	virtual void stopMyAction();
	
	void removeEffect();
	
private:
	Json::Value patternData;
	int keepFrame;
	int shootFrame;
	int ingFrame;
	float distance;
	int type;
	CCSize mSize;
	
	bool isRemoveEffect;
	
	void selfRemove();
	
	void startMyAction();
	
	void myAction();
	
	void myInit(KSCumberBase* cb,Json::Value pattern, CCSize t_mSize, int t_type);
};

class Saw : public AttackPattern
{
public:
	static Saw* create(CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize);
	
	virtual void stopMyAction();
	
private:
	
	
	virtual void selfRemoveSchedule();
	
	void myInit(CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize);
};

class ThunderBoltWrapper : public AttackPattern
{
public:
	static ThunderBoltWrapper* create(CCPoint t_sp, KSCumberBase* cb, int t_type, int t_targetingFrame, int t_shootFrame, float speedr);
	
	virtual void stopMyAction();
	
	
	bool groundOfJack(int state);
	
	

	
	void hidingAnimation(float dt);
//	void removeEffect()
//	{
//		unschedule(schedule_selector(ThunderBoltWrapper::myAction));
//		myGD->communication("CP_onPatternEnd");
//		
//		if(targetingImg)
//		{
//			CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
//			CCCallFunc* t_call = CCCallFunc::create(this,
//													callfunc_selector(ThunderBoltWrapper::removeFromParent));
//			CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade1, t_call);
//			targetingImg->runAction(t_seq);
//		}
//	}
	
private:
	FromToWithDuration<float> fadeFromToDuration;
	int type;
	int targetingFrame;
	int shootFrame;
	int ingFrame;
	float speedRatio;
	float curIndex;
	vector<CCPoint> pJackArray;
	deque<CCPoint> visitPoint;
	CCSprite* targetingImg;
	CCSprite* wifiImg;
	
	SatelliteBeam* myBeam;
	
	virtual void selfRemoveSchedule();
	
	void startMyAction();
	
	void myAction();
	void myInit(CCPoint t_sp, KSCumberBase* cb, int t_type, int t_targetingFrame, int t_shootFrame, float speedr);
};

class BigSaw : public AttackPattern
{
public:
	static BigSaw* create(CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize);
	
	virtual void stopMyAction();
	
private:
	
	
	virtual void selfRemoveSchedule();
	
	void myInit(CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize);
};

class FlameWrapper : public AttackPattern // burn
{
public:
	static FlameWrapper* create(CCPoint t_sp, KSCumberBase* cb, int t_tmCnt, int t_burnFrame);
	
	virtual void stopMyAction();
	
	void removeEffect();
	
private:
	
	int mType;
	int tmCnt;
	float baseAngle;
	float minAngle;
	float maxAngle;
	int burnFrame;
	int ingFrame;
	int createBurnFrame;
	CCParticleSystemQuad* myParticle;
	float baseDistance;
	CCPoint myPosition;
	
	bool is_remove_called;
	
	void selfRemove();
	
	virtual void selfRemoveSchedule();
	
	void particleRemove();
	
	void startMyAction();
	
	void myAction();
	
	void initParticle();
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, int t_tmCnt, int t_burnFrame);
};

class MeteorWrapper : public AttackPattern
{
public:
	static MeteorWrapper* create(int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea, float enableP, KSCumberBase* cb);
	
	virtual void stopMyAction();
	
//	void removeEffect();
	
private:
	float enableProb;
	int crashArea;
	int type;
	int tmCnt;
	int mRate;
	int totalFrame;
	int ingFrame;
	string imgFilename;
	int crashCount;
	bool isRemoveEffect;
	
	Well512 m_well512;
	void selfRemove();
	
	virtual void selfRemoveSchedule();
	
	void startMyAction();
	
	void myAction();
	
	void myInit(int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea, float enableP, KSCumberBase* cb);
	
	void accumCrashCount(int n);
};


class TornadoWrapper : public AttackPattern // blind
{
public:
	static TornadoWrapper* create(CCPoint t_sp, int tf, int sc, KSCumberBase* cb);
	
private:
	
	
	void myInit(CCPoint t_sp, int tf, int sc, KSCumberBase* cb);
};



//class AP_Missile23 : public AttackPattern // cobweb
//{
//public:
	//static AP_Missile23* create(int t_frame);
	//void updateCobweb();
//private:
	
	//int slowFrame;
	//int ingFrame;
	//bool is_stop;
	//CCSprite* cobwebImg;
	//FromToWithDuration<float> m_scaleFromTo;
	//void startFrame();
	
	//void framing();
	
	//void removeCobweb();
	
	//void stopFrame();
	
	//void myInit(int t_frame);
//};

class SightOutAttack : public AttackPattern // sight out
{
public:
	static SightOutAttack* create(int t_frame, KSCumberBase* cb);
	
	void updateSightOut();
	
private:
	
	int sightOutFrame;
	int ingFrame;
	
	void startFrame();
	
	void framing();
	void stopMyAction(){stopFrame(); }
	void stopFrame();
	
	void myInit(int t_frame, KSCumberBase* cb);
};

class FreezeAttack : public AttackPattern // freeze
{
public:
	static FreezeAttack* create(int t_frame, KSCumberBase* cb);
	virtual ~FreezeAttack();
	
	void updateFreeze();
	void stopMyAction(){stopFrame(); }
	void stopFrame();
	
private:
	
	int freezingFrame;
	int ingFrame;
	
	void startFrame();
	
	void framing();
	
	void myInit(int t_frame, KSCumberBase* cb);
};


class TeleportWrapper : public AttackPattern // teleport
{
public:
	static TeleportWrapper* create(KSCumberBase* cumber);
	
private:
	KSCumberBase* m_cumber;
	
	
	void myInit(KSCumberBase* cumber);
};

class ChaosAttack : public AttackPattern // chaos
{
public:
	static ChaosAttack* create(int t_frame, KSCumberBase* cb);
	
	virtual ~ChaosAttack();
	void updateChaos();
	void stopMyAction(){stopFrame(); }
	void stopFrame();
	
private:
	
	int chaosFrame;
	int ingFrame;
	
	void startFrame();
	
	void framing();
	
	void myInit(int t_frame, KSCumberBase* cb);
};

// 무작위 원형 던지기.
class UnusedMissile5 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile5);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	void update(float dt);
	virtual void stopMyAction();
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	int m_frameCnt;
	
	CCPoint m_position;
	CCSpriteBatchNode* batchNode;
	Well512 m_well512;
};

/// 골뱅이 패턴.
class UnusedMissile6 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile6);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	void update(float dt);
	virtual void stopMyAction();
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_color;
	
	int m_frameCnt;
	CCPoint m_position;
	int angle;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};

// 해바라기 패턴.
class UnusedMissile7 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile7);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	void update(float dt);
	virtual void stopMyAction();
protected:
	int m_numberPerFrame;
	float m_bulletSpeed;
	int m_color;
	
	int m_frameCnt;
	CCPoint m_position;
	int angle;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};

// 해바라기 패턴의 연속. 각도 변경 없음.
class UnusedMissile8 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile8);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	int m_frameCnt;
	CCPoint m_position;
	float angle;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};

/// 해바라기 패턴인데 각도가 바뀜.
class UnusedMissile9 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile9);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	int m_frameCnt;
	CCPoint m_position;
	float angle;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};


class UnusedMissile10 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile10);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	int m_frameCnt;
	CCPoint m_position;
	float angle;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};

/// 해바라긴데 각도를 달리하면서 쏨, 드르륵 쏘고 좀 쉬다가 드르륵...
class UnusedMissile11 : public AttackPattern
{
public:
	virtual void selfRemoveSchedule();
	CREATE_FUNC_CCP(UnusedMissile11);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	int m_term;
	int m_idleValue;
	int m_fireCount;
	int m_frameCnt;
	CCPoint m_position;
	float angle;
	Well512 m_well512;
	
	CCSpriteBatchNode* batchNode;
};

// 무궁화 패턴
class Mugunghwa : public AttackPattern
{
public:
	CREATE_FUNC_CCP(Mugunghwa);
	virtual void selfRemoveSchedule();
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	int m_frameCnt;
	CCPoint m_position;
	float angle;
	float angle2;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};

// 당구공.
class CaromWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(CaromWrapper);
	virtual void stopMyAction();

	void removeEffect();
	
private:
	Json::Value pattern;
	float t_move_speed;
	float t_cushion_cnt;
	bool t_is_big_bomb;
	int t_tmCnt;
	bool isRemoveEffect;
	CCPoint t_sp;
	virtual void selfRemoveSchedule();
	int m_crashArea;
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	void update(float dt);
};


// 조준형1 : 부채꼴.
class UnusedMissile1 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile1);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	int m_frameCnt;
	CCPoint m_position;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};


// 조준형 : 부채꼴의 연속, 캐릭터를 따라감.
class UnusedMissile2 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile2);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	int m_frameCnt;
	CCPoint m_position;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};

// 조준형 : 부채꼴의 연속, 캐릭터를 안 따라감.
class UnusedMissile3 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile3);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	CCPoint firstJackPosition;
	int m_frameCnt;
	CCPoint m_position;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};

// 조준형 : 부채꼴의 연속, 중심각의 랜덤성을 부여.
class UnusedMissile4 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(UnusedMissile4);
	virtual void selfRemoveSchedule();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_perFrame;
	int m_totalFrame;
	float m_bulletSpeed;
	int m_numberPerFrame;
	int m_color;
	
	CCPoint firstJackPosition;
	int fireCount;
	int m_frameCnt;
	CCPoint m_position;
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
};

class SawWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(SawWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	
	void update(float dt);
	virtual void stopMyAction();
	
protected:
	float speed;
	float crashsize;
	Well512 m_well512;
};
class SmallSawWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(SmallSawWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
	
protected:
	Well512 m_well512;
	int number;
	float speed;
	float crashsize;
};

// 태양 광선이 나에게로 ... ㅎㅎ
class CrashLazerWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(CrashLazerWrapper);
	virtual void stopMyAction();
	
	
	
	
	
	
	void hidingAnimation(float dt);
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	void selfRemove();
	
	virtual void selfRemoveSchedule();
	
	
	void startMyAction();
	
	void myAction();
	
	void lineCrashMap(CCPoint t_sp, float t_angle, int t_width, int t_height);
	
	void crashMapForIntPoint(IntPoint t_p);
	int crashMapForIntRect(IntRect t_r);
	
	CCPoint spinTransform(CCPoint t_tp, CCPoint t_bp, float t_angle); // jack, boss, angle;
protected:
	float m_crashSize;
	FromToWithDuration<float> fadeFromToDuration;
	int type;
	CCPoint sp;
	int createRingFrame;
	int chargeFrame;
	int crashFrame;
	int ingFrame;
	float angle;
	CCPoint dv;
	CCSprite* lazer_main;
	int dcolor;
	CCSprite* t_bead;
	CCRect crashRect;
	CCSprite* lazer_sub;
	float dscale;
	
	CCPoint jackPosition;
	int crashCount;
	bool attacked;
};
class CommonBulletPattern : public AttackPattern
{
public:
	CREATE_FUNC_CCP(CommonBulletPattern);
	virtual void selfRemoveSchedule();
	virtual void stopMyAction();
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	
	void initGuns();
	void update(float dt);	bool m_isCurve; // 커브인가
	bool m_havePath; // 패스를 가지고 있는가
	float m_randomDegree; // 랜덤각.
	int m_oneShotNumber; // 쉬지 않고 쏘는 개수.
	int m_oneShotTerm; // 쐈다가 쉬는 프레임수.
	int m_gunNumber; // 총의 개수.
	bool m_isSuper; // 슈퍼 미사일인가.
	vector<CCPoint> m_path;
	int m_frame;
	int m_totalFrame;
	enum TargetType
	{
		kToUser = 1,
		kCCW = 2,
		kCW = 3
	}m_targetingType;
	float m_rotationDegreeVelocity; // 회전각 속도.
	int m_color;
	float m_totalDegree; // 전체 각도.
	struct Gun
	{
		float bulletSpeed;	// 총알 속도
		int fireTerm;		// 자체 쏘는 텀.
		float initDegree;	// 초기화된 각도
		FromTo<float> degree; // 현재 각도
		int fireCount;		// oneShot 을 세기 위한 변수.
		int idleCount;		// 쐈다가 쉬는 프레임수를 세기위함.
	};
	
	vector<Gun> pan; // 판은 총들을 가짐.
	
	//	CCPoint firstJackPosition; // 잭의 초기 위치.
	//	CCPoint m_position; // 공격 당시의 보스의 위치
	Well512 m_well512;
	CCSpriteBatchNode* batchNode;
	CCNode* missileNode;
};




class CrashingRush : public AttackPattern
{
public:
	CREATE_FUNC_CCP(CrashingRush);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
};



// 불꽃놀이
class FireWorkWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(FireWorkWrapper);
	
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	Json::Value pattern;
};

// 움직이는 해바라기
class MovingSunflowerWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(MovingSunflowerWrapper);
	
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	Json::Value m_pattern;
};

// 폭탄 여러개 던지기
class ThrowBombWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(ThrowBombWrapper);
	virtual ~ThrowBombWrapper()
	{
		CCLOG("bomb throw destroy");
	}
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
	void targetTraceUpdate(float dt);
protected:
	Json::Value m_pattern;
	int m_frame;
	int m_totalFrame;
	CCSprite* m_targetSprite;
};


// 리버 스크럽.
class ScarabWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(ScarabWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_frame;
	int m_totalFrame;
	Json::Value m_pattern;
};







class FreezingWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(FreezingWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
};

class ChaosWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(ChaosWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
};




class LazerScanWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(LazerScanWrapper);
//	virtual void stopMyAction()
//	{
//		unscheduleUpdate();
//		
//		myGD->communication("CP_onPatternEnd");
//		
//		startSelfRemoveSchedule();
//	}
	virtual void stopMyAction();
	
	void removeEffect();
	
	void selfRemove();
	
	void startMyAction();
	void myAction();
	
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	
	void update(float dt);
protected:
	int totalFrame;
	int type;
	CCSprite* beamImg;
	float beamBaseAngle;
	int ingFrame;
	CCPoint startPosition;
};

class RadioactivityWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(RadioactivityWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	CCPoint m_position;
	int area;
	int totalFrame;
	int movingFrame;
	float enableRatio;
};



class WindMillWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(WindMillWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
};

class DynamiteWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(DynamiteWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
};

class AlongOfTheLineWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(AlongOfTheLineWrapper);
	virtual ~AlongOfTheLineWrapper();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_totalFrame;
	int m_number;
	float m_speed;
};

class CloudWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(CloudWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	Json::Value m_pattern;
	Well512 m_well512;
};

class PutChildWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP(PutChildWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	Json::Value m_pattern;
};
class KSSpecialAttackPattern19 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern19);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
};

class KSSpecialAttackPattern20 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern20);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
};

class KSSpecialAttackPattern21 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern21);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
};


/*
 class TornadoWrapper : public AttackPattern
 {
 public:
 CREATE_FUNC_CCP(TornadoWrapper);
 void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
 {
 m_cumber = cb;
 scheduleUpdate();
 }
 virtual void stopMyAction()
 {
 unscheduleUpdate();
 
 myGD->communication("CP_onPatternEnd");
 
 startSelfRemoveSchedule();
 }
 void update(float dt)
 {
 
 }
 protected:
 KSCumberBase* m_cumber;
 };
 */
class CobWeb : public AttackPattern // cobweb
{
public:
//	static Cobweb* create(int t_frame)
	static CobWeb* create(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	
	void updateCobWeb();
	virtual ~CobWeb();
private:

	Json::Value pattern;
	int slowFrame;
	int ingFrame;
	bool is_stop;
	float decreaseSpeed;
	enum {kElse, kInner, kOuter, kFrameTerminated} state;
	CCSprite* cobwebImg;
	FromToWithDuration<float> m_scaleFromTo;
	void startFrame();
	
	void framing();
	
	void removeCobWeb();
	void stopMyAction(){stopFrame(); }
	void stopFrame();
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
};


class PrisonPattern : public AttackPattern // prison
{
public:
	static PrisonPattern* create(CCPoint t_sp, float radius, int totalFrame, KSCumberBase* cb);
	
	void startMyAction();
	void hidingAnimation(float dt);
	
	void myAction();
	
	void stopMyAction();
	
	void myInit(CCPoint t_sp, float radius, int totalFrame, KSCumberBase* cb); // create 0.5 second;

private:
	CCSprite* m_prisonSprite;
	CCPoint m_initialJackPosition;
	float m_radius;
	
	int m_totalFrame;
	int m_frameCount;
	
	FromToWithDuration<float> m_fadeFromToDuration;
};

class RunDownSawWrapper : public AttackPattern 
{
public:
	CREATE_FUNC_CCP(RunDownSawWrapper);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	void update(float dt);
	virtual void stopMyAction();
protected:
	//float speed;
	//float crashSize;
	Well512 m_well512;
	Json::Value m_pattern;
};



class GodOfDeath : public AttackPattern
{
public:
	CREATE_FUNC_CCP(GodOfDeath);
	virtual ~GodOfDeath()
	{
		CCLOG("~GodOfDeath");
	}
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	void update(float dt);
	virtual void stopMyAction();
protected:
	//float speed;
	//float crashSize;
	float m_followFrames;
	float m_followSpeed;
	bool m_isFollow;
	int m_frameCount;
	float m_alpha;
	bool m_killed;
	
	CCSprite* m_godOfDeathSprite;
	Well512 m_well512;
	Json::Value m_pattern;
	CCBAnimationManager* m_manager;
};
class MeshWrapper : public AttackPattern
{
public:
	CREATE_FUNC_CCP_JSON(MeshWrapper);
	virtual ~MeshWrapper()
	{
		CCLOG("~MeshWrapper");
	}
	void myInit(CCPoint t_sp, KSCumberBase* cb, const Json::Value& patternJson);
	void update(float dt);
	virtual void stopMyAction();
protected:
	//float speed;
	//float crashSize;
	Well512 m_well512;
	Json::Value m_pattern;
	int m_currentFrames;
};

class JunirBombWrapper : public AttackPattern // teleport
{
public:
	static JunirBombWrapper* create(KSCumberBase* cumber, Json::Value param);
	
private:
//	KSCumberBase* m_cumber;
	
	
	void myInit(KSCumberBase* cumber, Json::Value param);
};

class WiperMissileWrapper : public AttackPattern // teleport
{
public:
	static WiperMissileWrapper* create(KSCumberBase* cumber, Json::Value param);
	
private:
	//	KSCumberBase* m_cumber;
	
	
	void myInit(KSCumberBase* cumber, Json::Value param);
	CCSpriteBatchNode* batchNode;
	virtual void stopMyAction();
};


#endif
