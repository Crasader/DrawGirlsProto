#pragma once

#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <vector>
//#include "CumberEmotion.h"
#include "FromTo.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;

/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.
class KSJuniorBase : public KSCumberBase
{
public:
	KSJuniorBase() : RADIUS(15.f)
	{
		m_cumberState = kCumberStateNothing;
		CCLOG("%s %d kCumberStateMoving", __FILE__, __LINE__);
	}
	virtual ~KSJuniorBase(){}
	
	
	virtual void stopCasting()
	{
		return;
	}
	virtual void onStartMoving();
	virtual void onStopMoving();
	void cumberAttack(float dt);
	
	static KSJuniorBase* create(const string& ccbiName) \
	{ \
    KSJuniorBase *pRet = new KSJuniorBase(); \
    if (pRet && pRet->init(ccbiName)) \
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
	virtual bool init(const string& ccbiName);
	virtual void setPosition(const CCPoint& t_sp);
	virtual void setPositionX(float t_x);
	virtual void setPositionY(float t_y);
	virtual const CCPoint& getPosition();
	

	COLLISION_CODE crashLooper(const set<IntPoint>& v, IntPoint* cp);
	const float RADIUS;
	//	void showEmotion(EmotionType t_type)
	//	{
	//		if(mEmotion)
	//			mEmotion->selfRemove();
	//		mEmotion = Emotion::create(t_type, this, callfunc_selector(KSJuniorBase::nullmEmotion));
	//		mEmotion->setPosition(ccp(30,20));
	//		addChild(mEmotion);
	//	}
	//	void nullmEmotion()
	//	{
	//		mEmotion = NULL;
	//	}
	bool startDamageReaction(float damage, float angle, bool castCancel, bool stiffen);
	virtual void attackBehavior(Json::Value pattern)
	{
		// nothing.
	}
//	void assignJuniorData(Json::Value data);
//	void applyPassiveData(const std::string& passive);
//	void applyAutoBalance();
	virtual void checkConfine(float dt);
	virtual void startSpringCumber(float userdata){}
	virtual void startAnimationNoDirection();
	void damageReaction(float dt);
	void animationNoDirection(float dt);
	//	virtual void startAnimationDirection(){}
	virtual void onStartGame();
	//	virtual void onEndGame();
	virtual void onPatternEnd();
	virtual void startInvisible(int totalframe);
	void invisibling(float dt);
	CCPoint getMissilePoint();

	virtual void setScale(float scale);
	virtual void setScaleX(float x);
	virtual void setScaleY(float y);
	virtual void randomPosition();
	
	virtual void furyModeOn(int tf);
	void furyModeScheduler(float dt);
	virtual void furyModeOff();
	
	virtual void crashMapForPosition(CCPoint targetPt);
	
	
	void scaleAdjustment(float dt);
	

	virtual void endTeleport();
	virtual void startTeleport();
	virtual void smaller();
	virtual void stopAnimationNoDirection();
	virtual void stopAnimationDirection()
	{
		//		m_direction.state = 2;
	}
	virtual COLLISION_CODE getCrashCode(IntPoint point, IntPoint* checkPosition);
	float getRadius();
	void attachEmotion() { }
	void setGameover();
	virtual void selfBomb(Json::Value param);
protected:

	
	
	bool isGameover;
	int lastCastNum;
	//	CCSprite* m_headImg;
	CCSprite* m_headImg;
	CCBAnimationManager *mAnimationManager;
	IntPoint getMapPoint(CCPoint c){
		return IntPoint(round((c.x - 1) / pixelSize + 1.f),
										round((c.y - 1) / pixelSize + 1.f));
	}
	
	//	Emotion* mEmotion;
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
	
	

	
};






