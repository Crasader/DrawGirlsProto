#pragma once


#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <vector>
//#include "CumberEmotion.h"
#include "FromTo.h"
#include "cocos-ext.h"

using namespace cocos2d::extension;


class CircleBossCCB : public CCSprite,// public CCBSelectorResolver        // CCMenuItem이나 CCControl 버튼의 콜백함수를 소스와 연결
public CCBMemberVariableAssigner,  // 멤버변수나 커스텀프로퍼티를 소스와 연결
public CCNodeLoaderListener       // ccbi파일 로딩 완료후 콜백함수 연결
{
	
public:
	CREATE_FUNC(CircleBossCCB);
	CircleBossCCB()
	{
		
	}
	virtual ~CircleBossCCB()
	{
		for(auto i : m_s)
		{
			CC_SAFE_RELEASE(i);
		}
	}
	bool onAssignCCBCustomProperty(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::extension::CCBValue *pCCBValue);
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode);
	virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader);
public:
	vector<CCSprite*> m_s;
};

class CircleCCBPieceBase : public KSCumberBase, public CCBAnimationManagerDelegate
{
public:
	virtual void runTimeline(Json::Value patternInfo) = 0;
};
/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.
class KSCircleBase : public CircleCCBPieceBase, public CCBSelectorResolver
{
public:
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName);
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
public:
	virtual void runTimeline(Json::Value patternInfo);
	
	virtual void completedAnimationSequenceNamed(const char *name_);
	KSCircleBase() : RADIUS(15.f), m_lookToLeft(true)
	{
		setCumberState(kCumberStateNothing);
	}
	void removeFromParent()
	{
		CircleCCBPieceBase::removeFromParent();
		CCLOG("nonono1");
	}
	void removeFromParentAndCleanup(bool cleanup)
	{
		CircleCCBPieceBase::removeFromParentAndCleanup(cleanup);
		CCLOG("nonono2");
	}
	virtual ~KSCircleBase(){
		CCLOG("~KSCircle");
	}
	
	virtual void checkConfine(float dt);
	virtual void onStartMoving();
	virtual void onStopMoving();
	virtual void stopCasting();
	virtual bool init(const string& ccbiName);
	static KSCircleBase* create(const string& ccbiName) \
	{ \
    KSCircleBase *pRet = new KSCircleBase(); \
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
	//		mEmotion = Emotion::create(t_type, this, callfunc_selector(Apricot::nullmEmotion));
	//		mEmotion->setPosition(ccp(30,20));
	//		addChild(mEmotion);
	//	}
	//	void nullmEmotion()
	//	{
	//		mEmotion = NULL;
	//	}
	bool startDamageReaction(float damage, float angle, bool castCancel, bool stiffen);
	virtual void attackBehavior(Json::Value _pattern);
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
	
	
	virtual void stopAnimationNoDirection();
	virtual void stopAnimationDirection();
	virtual COLLISION_CODE getCrashCode(IntPoint point, IntPoint* checkPosition);
	float getRadius();
	void attachEmotion();
	void setGameover();

protected:
	bool m_lookToLeft;
	std::string m_atype;
	std::string currentTimeline;
	std::string currentTimelineFooter; // _b _m _e 같은것들.
	bool m_attackCanceled;
	int m_repeatNumber;
	bool isGameover;
	int lastCastNum;
	//	CCSprite* m_headImg;
	CircleBossCCB* m_headImg;
	void update(float dt);
	CCBAnimationManager *mAnimationManager;
	IntPoint getMapPoint(CCPoint c);
	
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

class CCBReader;
class CircleLoader : public CCSpriteLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CircleLoader, loader); //로더 입니다. 네이밍이 비슷해서.. 주의하세요.
	/*
	 * 아래와 같은 구문입니다.
	 
	 static TestLoaderLoader * loader()
	 {
	 TestLoaderLoader * ptr = new TestLoaderLoader();
	 if(ptr != NULL)
	 {
	 ptr->autorelease();
	 return ptr;
	 }
	 CC_SAFE_DELETE(ptr);
	 return NULL;
	 }
	 
	 */
	
	
protected:
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CircleBossCCB); //레이어 입니다. 네이밍이 비슷해서.. 주의하세요.
	/*
	 * 아래와 같은 구문입니다.
	 
	 virtual TestLoaderLayer * createCCNode(CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader)
	 {
	 return TestLoaderLayer::create();
	 }
	 */
	
	
};



