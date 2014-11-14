#pragma once


#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <set>
//#include "CumberEmotion.h"
#include <deque>
#include "Jack.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;


/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.


class KSSnakeBase : public KSCumberBase, public CCBSelectorResolver
{
public:
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
public:
	virtual void completedAnimationSequenceNamed(const char *name_);
	KSSnakeBase() :
	RADIUS(110.f / 4.f), // 머리에 대한 충돌 반지름
	BODY_RADIUS(70/4.f), // 몸통에 대한 충돌 반지름
	TAIL_RADIUS(50/4.f), // 꼬리에 대한 충돌 반지름
	//	mEmotion(nullptr),
	LIMIT_COLLISION_PER_SEC(3), /// 초당 변수만큼 충돌시 스케일 줄임.
	ATTACK_POINT_X(-18), // 가운데 위치로부터 떨어진 공격포인턴데, 축소한 그림에서의 기준.
	ATTACK_POINT_Y(0),   // 가운데 위치로부터 떨어진 공격포인턴데, 축소한 그림에서의 기준.
	BODY_MARGIN(20),     // 몸통 사이의 거리.
	TAIL_MARGIN(40)      // 몸통과 꼬리사이의 거리.
	
	{
		setCumberState(kCumberStateNothing);
	}
	
	void removeFromParent()
	{
		KSCumberBase::removeFromParent();
		CCLOG("nonono1");
	}
	
	void removeFromParentAndCleanup(bool cleanup)
	{
		KSCumberBase::removeFromParentAndCleanup(cleanup);
		CCLOG("nonono2");
	}
	
	virtual ~KSSnakeBase();
	
	virtual void checkConfine(float dt);
	virtual void onStartMoving();
	virtual void onStopMoving();
	virtual void onPatternEnd();
	virtual void onStartGame();
	virtual void stopCasting();
	void runTimeline( Json::Value patternInfo );
	virtual void crashMapForPosition(CCPoint targetPt);
	virtual bool init(const std::string& ccbiFile, bool isNotShowWindow);
	static KSSnakeBase* create(const std::string& ccbiFile, bool isNotShowWindow = true) \
	{ \
    KSSnakeBase *pRet = new KSSnakeBase(); \
    if (pRet && pRet->init(ccbiFile, isNotShowWindow)) \
    { \
			pRet->autorelease(); \
			return pRet; \
    } \
    else \
    { \
			delete pRet; \
			pRet = NULL; \
			return NULL; \
    } \
	}
	virtual void setPosition(const CCPoint& t_sp);
	virtual void setPositionX(float t_x);
	virtual void setPositionY(float t_y);
	
	virtual const CCPoint& getPosition();
	virtual float getPositionX();
	virtual float getPositionY();
	
	
	COLLISION_CODE crashLooper(const set<IntPoint>& v, IntPoint* cp);
	
	//	void showEmotion(EmotionType t_type)
	//	{
	//		if(mEmotion)
	//			mEmotion->selfRemove();
	//		mEmotion = Emotion::create(t_type, this, callfunc_selector(Apple::nullmEmotion));
	//		mEmotion->setPosition(ccpAdd(getPosition(), ccp(30,20)));
	//		addChild(mEmotion, 11);
	//	}
	//	void nullmEmotion()
	//	{
	//		mEmotion = NULL;
	//	}
	void setHeadAndBodies();
	virtual void attackBehavior(Json::Value pattern);
	virtual bool startDamageReaction(float damage, float angle, bool castCancel, bool stiffen);
	virtual void startAnimationNoDirection();
	virtual void startAnimationDirection();
	virtual void stopAnimationNoDirection();
	virtual void stopAnimationDirection();
	//	virtual void startSpringCumber(float userdata)
	//	{
	//		startDamageReaction(userdata);
	//	}
	void damageReaction(float dt);
	void animationNoDirection(float dt);
	void animationDirection(float dt);
	
	
	void scaleAdjustment(float dt);
	CCPoint getMissilePoint();
	
	virtual void startInvisible(int totalframe);
	void invisibling(float dt);
	
	virtual void furyModeOn(int tf);
	void furyModeScheduler(float dt);
	virtual void furyModeOff();
	
	
	
	virtual void randomPosition();
	
	virtual void onTargetingJack(CCPoint jackPosition);
	virtual COLLISION_CODE getCrashCode(IntPoint point, IntPoint* checkPosition);
	float getRadius();
	void attachEmotion();
	void setGameover();
	virtual void selfBomb(Json::Value param){}
protected:
	const float RADIUS;
	const float BODY_RADIUS;
	const float TAIL_RADIUS;
	const float BODY_MARGIN;
	const float TAIL_MARGIN;
	const int LIMIT_COLLISION_PER_SEC; /// 초당 변수만큼 충돌시 스케일 줄임.
	const float ATTACK_POINT_X;
	const float ATTACK_POINT_Y;
	bool isGameover;
	CCBAnimationManager* m_headAnimationManager;
	vector<CCBAnimationManager*> m_bodyAnimationManagers;
	CCBAnimationManager* m_tailAnimationManager;
	
	CCSprite* m_headImg;
	vector<CCSprite*> m_Bodies;
	CCSprite* m_tailImg;
	IntPoint getMapPoint(CCPoint c);
	
	
	//	Emotion* mEmotion;
	
	deque< SnakeTrace > m_cumberTrace; // back 은 항상 머리를 가르킴.
	
	
	/// 방사형 에니메이션 용.
	struct NoDirection
	{
		float rotationDeg;
		float distance;
		float timer;
		CCPoint startingPoint;
		int rotationCnt;
		int state; // 1 : 도는상태, 2 : 다시 제 위치로 돌아가는 상태
	}m_noDirection;
	
	/// 지향형 에니메이션 용.
	struct Direction
	{
		float lastDirectionDeg;
		int state; // 1 : 바라보는 상태, 2 : 다시 제 방향 보는 상태.
		float timer;
		void initVars(){
			state = 1;
			lastDirectionDeg = 0.f;
			timer = 0.f;
		}
	}m_direction;
	
	
protected:
	std::string m_atype;
	std::string currentTimeline;
	std::string currentTimelineFooter; // _b _m _e 같은것들.
	bool m_attackCanceled;
	int m_repeatNumber;
	CCBAnimationManager *mAnimationManager;
	
};



