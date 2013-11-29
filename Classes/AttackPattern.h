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
#include "JsonBox.h"
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

class AttackPattern : public CCNode
{
public:
	
	void startSelfRemoveSchedule();
	
	virtual void stopMyAction(){}
	
protected:
	
	virtual void selfRemoveSchedule();
	
	KSCumberBase* m_cumber;
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




class AP_Missile6 : public AttackPattern
{
public:
	static AP_Missile6* create(CCPoint t_sp, int t_type);
	
	virtual void stopMyAction();
	
private:
	
	int type;
	CCSprite* beamImg;
	float beamBaseAngle;
	int ingFrame;
	CCPoint startPosition;
	
	void removeEffect();
	
	void selfRemove();
	
	void startMyAction();
	void myAction();
	
	void myInit(CCPoint t_sp, int t_type);
};

class AP_Missile9 : public AttackPattern
{
public:
	static AP_Missile9* create(int t_keepFrame, int t_shootFrame, float t_distance, CCSize mSize, int t_type);
	
	virtual void stopMyAction();
	
	void removeEffect();
	
private:
	
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
	
	void myInit(int t_keepFrame, int t_shootFrame, float t_distance, CCSize t_mSize, int t_type);
};

class AP_Missile11 : public AttackPattern
{
public:
	static AP_Missile11* create(CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize);
	
	virtual void stopMyAction();
	
private:
	
	
	virtual void selfRemoveSchedule();
	
	void myInit(CCPoint t_sp, int t_type, float t_speed, IntSize t_mSize);
};

class AP_Missile12 : public AttackPattern
{
public:
	static AP_Missile12* create(CCPoint t_sp, int t_type, int t_targetingFrame, int t_shootFrame);
	
	virtual void stopMyAction();
	
	
	
	

	
	void hidingAnimation(float dt);
//	void removeEffect()
//	{
//		unschedule(schedule_selector(AP_Missile12::myAction));
//		myGD->communication("MP_endIngActionAP");
//		myGD->communication("CP_onPatternEnd");
//		
//		if(targetingImg)
//		{
//			CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
//			CCCallFunc* t_call = CCCallFunc::create(this,
//													callfunc_selector(AP_Missile12::removeFromParent));
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
	deque<CCPoint> pJackArray;
	deque<CCPoint> visitPoint;
	CCSprite* targetingImg;
	CCSprite* wifiImg;
	
	SatelliteBeam* myBeam;
	
	virtual void selfRemoveSchedule();
	
	void startMyAction();
	
	void myAction();
	void myInit(CCPoint t_sp, int t_type, int t_targetingFrame, int t_shootFrame);
};

class AP_Missile14 : public AttackPattern
{
public:
	static AP_Missile14* create(CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize);
	
	virtual void stopMyAction();
	
private:
	
	
	virtual void selfRemoveSchedule();
	
	void myInit(CCPoint t_sp, int t_type, float t_speed, int t_tmCnt, IntSize t_mSize);
};

class AP_Missile15 : public AttackPattern // burn
{
public:
	static AP_Missile15* create(CCPoint t_sp, int t_tmCnt, int t_burnFrame);
	
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
	
	void myInit(CCPoint t_sp, int t_tmCnt, int t_burnFrame);
};

class AP_Missile16 : public AttackPattern
{
public:
	static AP_Missile16* create(int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea);
	
	virtual void stopMyAction();
	
	void removeEffect();
	
private:
	int crashArea;
	int type;
	int tmCnt;
	int mRate;
	int totalFrame;
	int ingFrame;
	string imgFilename;
	
	bool isRemoveEffect;
	
	Well512 m_well512;
	void selfRemove();
	
	virtual void selfRemoveSchedule();
	
	void startMyAction();
	
	void myAction();
	
	void myInit(int t_type, int t_tmCnt, int t_totalFrame, int t_crashArea);
};


class AP_Missile21 : public AttackPattern // blind
{
public:
	static AP_Missile21* create(CCPoint t_sp, int tf, int sc);
	
private:
	
	
	void myInit(CCPoint t_sp, int tf, int sc);
};



class AP_Missile23 : public AttackPattern // cobweb
{
public:
	static AP_Missile23* create(int t_frame);
	void updateCobweb();
private:
	
	int slowFrame;
	int ingFrame;
	bool is_stop;
	CCSprite* cobwebImg;
	FromToWithDuration<float> m_scaleFromTo;
	void startFrame();
	
	void framing();
	
	void removeCobweb();
	
	void stopFrame();
	
	void myInit(int t_frame);
};

class AP_Missile24 : public AttackPattern // sight out
{
public:
	static AP_Missile24* create(int t_frame);
	
	void updateSightOut();
	
private:
	
	int sightOutFrame;
	int ingFrame;
	
	void startFrame();
	
	void framing();
	
	void stopFrame();
	
	void myInit(int t_frame);
};

class AP_Missile26 : public AttackPattern // freeze
{
public:
	static AP_Missile26* create(int t_frame);
	
	void updateFreeze();
	
	void stopFrame();
	
private:
	
	int freezingFrame;
	int ingFrame;
	
	void startFrame();
	
	void framing();
	
	void myInit(int t_frame);
};


class AP_Missile32 : public AttackPattern // teleport
{
public:
	static AP_Missile32* create();
	
private:
	
	
	void myInit();
};

class AP_Missile33 : public AttackPattern // chaos
{
public:
	static AP_Missile33* create(int t_frame);
	
	void updateChaos();
	
	void stopFrame();
	
private:
	
	int chaosFrame;
	int ingFrame;
	
	void startFrame();
	
	void framing();
	
	void myInit(int t_frame);
};

// 무작위 원형 던지기.
class KSAttackPattern1 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSAttackPattern1);
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
class KSAttackPattern2 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSAttackPattern2);
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
class KSAttackPattern3 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSAttackPattern3);
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
class KSAttackPattern4 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSAttackPattern4);
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
class KSAttackPattern5 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSAttackPattern5);
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


class KSAttackPattern6 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSAttackPattern6);
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
class KSAttackPattern7 : public AttackPattern
{
public:
	virtual void selfRemoveSchedule();
	CREATE_FUNC_CCP(KSAttackPattern7);
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
class KSAttackPattern8 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSAttackPattern8);
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
class KSAttackPattern9 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSAttackPattern9);
	virtual void stopMyAction();

	void removeEffect();
	
private:
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
class KSTargetAttackPattern1 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern1);
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
class KSTargetAttackPattern2 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern2);
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
class KSTargetAttackPattern3 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern3);
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
class KSTargetAttackPattern4 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern4);
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

class KSTargetAttackPattern5 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern5);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	
	void update(float dt);
	virtual void stopMyAction();
	
protected:
	float speed;
	float crashsize;
	Well512 m_well512;
};
class KSTargetAttackPattern6 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern6);
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
class KSTargetAttackPattern7 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern7);
	virtual void stopMyAction();
	
	
	
	
	
	
	void hidingAnimation(float dt);
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	void selfRemove();
	
	virtual void selfRemoveSchedule();
	
	
	void startMyAction();
	
	void myAction();
	
	void lineCrashMap(CCPoint t_sp, float t_angle, int t_width, int t_height);
	
	void crashMapForIntPoint(IntPoint t_p);
	
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
	KSCumberBase* m_cumber;
	
	CCPoint jackPosition;
	
};
class KSTargetAttackPattern8 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern8);
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
	
};




class KSTargetAttackPattern9 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern9);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};



// 불꽃놀이
class KSTargetAttackPattern10 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern10);
	
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	Json::Value pattern;
	KSCumberBase* m_cumber;
};

// 움직이는 해바라기
class KSTargetAttackPattern11 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern11);
	
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	Json::Value m_pattern;
	KSCumberBase* m_cumber;
};

// 폭탄 여러개 던지기
class KSTargetAttackPattern12 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern12);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	Json::Value m_pattern;
	int m_frame;
	int m_totalFrame;
	KSCumberBase* m_cumber;
};


// 리버 스크럽.
class KSTargetAttackPattern13 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSTargetAttackPattern13);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	int m_frame;
	int m_totalFrame;
	KSCumberBase* m_cumber;
};



class KSSpecialAttackPattern1 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern1);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	CCPoint m_position;
	KSCumberBase* m_cumber;
	int m_totalFrame;
	int m_scale;
};

class KSSpecialAttackPattern2 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern2);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern3 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern3);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern4 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern4);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern5 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern5);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern6 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern6);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern7 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern7);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern8 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern8);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern9 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern9);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern10 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern10);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};


class KSSpecialAttackPattern11 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern11);
//	virtual void stopMyAction()
//	{
//		unscheduleUpdate();
//		
//		myGD->communication("MP_endIngActionAP");
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
	KSCumberBase* m_cumber;
	int totalFrame;
	int type;
	CCSprite* beamImg;
	float beamBaseAngle;
	int ingFrame;
	CCPoint startPosition;
};

class KSSpecialAttackPattern12 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern12);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	CCPoint m_position;
	KSCumberBase* m_cumber;
	int area;
	int totalFrame;
	int movingFrame;
};

class KSSpecialAttackPattern13 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern13);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern14 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern14);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern15 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern15);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern16 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern16);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern17 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern17);
	virtual ~KSSpecialAttackPattern17();
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
	int m_totalFrame;
	int m_number;
	float m_speed;
};

class KSSpecialAttackPattern18 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern18);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	Json::Value m_pattern;
	KSCumberBase* m_cumber;
	Well512 m_well512;
};

class KSSpecialAttackPattern19 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern19);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern20 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern20);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};

class KSSpecialAttackPattern21 : public AttackPattern
{
public:
	CREATE_FUNC_CCP(KSSpecialAttackPattern21);
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	virtual void stopMyAction();
	void update(float dt);
protected:
	KSCumberBase* m_cumber;
};


/*
 class KSSpecialAttackPattern1 : public AttackPattern
 {
 public:
 CREATE_FUNC_CCP(KSSpecialAttackPattern1);
 void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData)
 {
 m_cumber = cb;
 scheduleUpdate();
 }
 virtual void stopMyAction()
 {
 unscheduleUpdate();
 
 myGD->communication("MP_endIngActionAP");
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
class Cobweb : public AttackPattern // cobweb
{
public:
//	static Cobweb* create(int t_frame)
	static Cobweb* create(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
	
	void updateCobweb();
	
private:

	Json::Value pattern;
	KSCumberBase* cumber;
	int slowFrame;
	int ingFrame;
	bool is_stop;
	enum {kElse, kInner, kOuter, kFrameTerminated} state;
	CCSprite* cobwebImg;
	FromToWithDuration<float> m_scaleFromTo;
	void startFrame();
	
	void framing();
	
	void removeCobweb();
	
	void stopFrame();
	
	void myInit(CCPoint t_sp, KSCumberBase* cb, const std::string& patternData);
};


class PrisonPattern : public AttackPattern // prison
{
public:
	static PrisonPattern* create(CCPoint t_sp, float radius, int totalFrame);
	
	void startMyAction();
	void hidingAnimation(float dt);
	
	void myAction();
	
	void stopMyAction();
	
	void myInit(CCPoint t_sp, float radius, int totalFrame); // create 0.5 second;

private:
	CCSprite* m_prisonSprite;
	CCPoint m_initialJackPosition;
	float m_radius;
	
	int m_totalFrame;
	int m_frameCount;
	
	FromToWithDuration<float> m_fadeFromToDuration;
};




#endif
