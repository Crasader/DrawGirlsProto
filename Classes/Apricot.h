#pragma once


#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <vector>
//#include "CumberEmotion.h"
#include "FromTo.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;


class BossCCB : public CCSprite,// public CCBSelectorResolver        // CCMenuItem이나 CCControl 버튼의 콜백함수를 소스와 연결
public CCBMemberVariableAssigner,  // 멤버변수나 커스텀프로퍼티를 소스와 연결
public CCNodeLoaderListener       // ccbi파일 로딩 완료후 콜백함수 연결
{
	
public:
	CREATE_FUNC(BossCCB);
	BossCCB()
	{
		
	}
	virtual ~BossCCB()
	{
		for(auto i : m_s)
		{
			CC_SAFE_RELEASE(i);
		}
	}
	bool onAssignCCBCustomProperty(cocos2d::CCObject *pTarget, const char *pMemberVariableName, cocos2d::extension::CCBValue *pCCBValue)
	{
    return false;
	}
	virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char * pMemberVariableName, CCNode * pNode)
	{
		/*
     * CCB_MEMBERVARIABLEASSIGNER_GLUE parameters
     * 1. 타겟. 커스텀 클래스, 여기선 this (this == pTarget)
     * 2. 매칭시킬 변수 이름. pMemberVariableName == "m_pSprBg" 코코스빌더에서 지정한 이름과 같아야 매칭이 됩니다.
     * 3. 멤버변수의 타입. (CCSprite*)
     * 4. 헤더에 선언한 실제 멤버변수 this->m_pSprBg
     */
//    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,   "m_7", CCSprite *, m_7);
		CCSprite* temp = 0;
		if (pTarget == this) {
			temp = dynamic_cast<CCSprite*>(pNode);
			temp->retain();
			m_s.push_back(temp);
			return true;
			
		}
    /*
     * 성공하면 위에서 return true 됩니다.
     */
    return false;
	}
	virtual void onNodeLoaded(CCNode * pNode, CCNodeLoader * pNodeLoader)
	{
	}
public:
	vector<CCSprite*> m_s;
};
/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.
class Apricot : public KSCumberBase, public CCBAnimationManagerDelegate
{
public:
	virtual void completedAnimationSequenceNamed(const char *name_)
	{
		string name = name_;
		if(name == "attack_b")
		{
			// 캔슬이 되었는지의 여부를 알아야 됨.
			// middle 액션을 하기전에 속성을 설정함.
			
		}
		else if(name == "attack_m")
		{
			// 반복을 시킬 건지 검사하고 캔슬이 되었다면 캔슬 동작을 작동시킴.
			// 캔슬이면 속성을 해제함.
		}
		else if(name == "attack_e")
		{
			// 우흥~
		}
	}
	Apricot() : RADIUS(15.f),// mEmotion(nullptr),

	teleportImg(NULL) // 텔레포트 이미지
	{
		m_state = (CUMBERSTATEMOVING);
	}
	virtual ~Apricot(){
		
	}
	
	virtual void onStartMoving()
	{
		m_state = CUMBERSTATEMOVING;
		schedule(schedule_selector(KSCumberBase::movingAndCrash));
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
	}

	virtual bool init();
	CREATE_FUNC(Apricot);
	virtual void setPosition(const CCPoint& t_sp)
	{
		//		CCLog("setPos %f %f", t_sp.x, t_sp.y);
		//		KSCumberBase::setPosition(t_sp);
		m_headImg->setPosition(t_sp);
		myGD->setMainCumberPoint(ccp2ip(t_sp));
		m_mapPoint = ccp2ip(t_sp);
		//		myGD->communication("Main_moveGamePosition", t_sp);
		//		myGD->communication("VS_setMoveGamePosition", t_sp);
		//		myGD->communication("Main_moveGamePosition", t_sp);
		//		myGD->communication("Main_moveGamePosition", t_sp);
		//		std::thread t1([](){;});
	}
	virtual void setPositionX(float t_x)
	{
		setPosition(ccp(t_x, getPositionY()));
	}
	virtual void setPositionY(float t_y)
	{
		setPosition(ccp(getPositionX(), t_y));
	}
	virtual const CCPoint& getPosition()
	{
		return m_headImg->getPosition();
	}
	

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
	bool startDamageReaction(float damage, float angle);
	virtual void attackBehavior(Json::Value pattern)
	{
		lastCastNum = m_well512.GetValue(1, 1);
		mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());
		if(!(pattern["pattern"].asString() == "109"))
		{
			std::string target = pattern.get("target", "no").asString();
			if( target == "yes") // 타게팅이라면 조준하라
				startAnimationDirection();
			else if(target == "no") // 타게팅이 아니면 돌아라
				startAnimationNoDirection();
		}
	}
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
	CCPoint getMissilePoint()
	{
		return getPosition() + ccp(0, 0);
	}

	virtual void setScale(float scale)
	{
		m_headImg->setScale(scale);
	}
	virtual void setScaleX(float x)
	{
		m_headImg->setScaleX(x);
	}
	virtual void setScaleY(float y)
	{
		m_headImg->setScaleY(y);
	}
	virtual void randomPosition();
	
	virtual void furyModeOn(int tf);
	void furyModeScheduler(float dt);
	virtual void furyModeOff();

	virtual void crashMapForPosition(CCPoint targetPt);
	
	
	void scaleAdjustment(float dt);
	
	virtual void lightSmaller()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.2, 0.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(KSCumberBase::endTeleport));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		if(teleportImg)
			teleportImg->runAction(t_seq);
	}
	
	virtual void endTeleport()
	{
		teleportImg->removeFromParentAndCleanup(true);
		teleportImg = NULL;
		startMoving();
		myGD->communication("CP_onPatternEnd");
	}
	virtual void startTeleport()
	{
		if(teleportImg)
		{
			teleportImg->removeFromParentAndCleanup(true);
			teleportImg = NULL;
		}
		
		teleportImg = CCSprite::create("teleport_light.png");
		teleportImg->setScale(0.01f);
		addChild(teleportImg);
		
		CCBlink* t_scale = CCBlink::create(0.5, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Apricot::smaller));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		teleportImg->runAction(t_seq);
		AudioEngine::sharedInstance()->playEffect("sound_teleport.mp3",false);
	}
	virtual void smaller()
	{
		CCBlink* t_scale = CCBlink::create(0.5, 8);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Apricot::randomPosition));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
	}
	virtual void stopAnimationNoDirection()
	{
		m_noDirection.state = 2;
	}
	virtual void stopAnimationDirection()
	{
//		m_direction.state = 2;
	}
	virtual COLLISION_CODE getCrashCode(IntPoint point, IntPoint* checkPosition){
		IntPoint afterPoint = point;
		float half_distance = RADIUS*getCumberScale(); // 20.f : radius for base scale 1.f
		int ip_half_distance = half_distance / 2;
		set<IntPoint> ips;
		for(int i=afterPoint.x-ip_half_distance;i<=afterPoint.x+ip_half_distance;i++)
		{
			for(int j=afterPoint.y-ip_half_distance;j<=afterPoint.y+ip_half_distance;j++)
			{
				float calc_distance = sqrtf(powf((afterPoint.x - i)*1,2) + powf((afterPoint.y - j)*1, 2));
				if(calc_distance < ip_half_distance)
				{
					ips.insert(IntPoint(i, j));
				}
			}
		}
		
		COLLISION_CODE collisionCode = crashLooper(ips, checkPosition);
		return collisionCode;
	}
	float getRadius()
	{
		return RADIUS;
	}
protected:
	CCSprite* teleportImg;

	
	bool isGameover;
	int lastCastNum;
	//	CCSprite* m_headImg;
	BossCCB* m_headImg;
	void update(float dt)
	{
		CCNode* endP = myGD->getCommunicationNode("Main_gameNodePointer");
		auto function = [&](CCNode* node)->CCAffineTransform
		{
			CCAffineTransform t = this->nodeToParentTransform();
			for (CCNode *p = node->getParent(); p != endP; p = p->getParent())
        t = CCAffineTransformConcat(t, p->nodeToParentTransform());
			
			return t;
		};
		
		
		for(auto i : m_headImg->m_s)
		{
			
			CCPoint ret = CCPointApplyAffineTransform(i->getPosition(), function(i));
			crashMapForPosition(ret);
		}
		
	}
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
	
	struct DamageData
	{
		float m_damageX;
		float m_damageY;
		float timer;
	}m_damageData;
	
	
	
	
	
	
	
};

class CCBReader;
class ApricotLoader : public CCSpriteLoader
{
public:
	CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ApricotLoader, loader); //로더 입니다. 네이밍이 비슷해서.. 주의하세요.
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
	CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(BossCCB); //레이어 입니다. 네이밍이 비슷해서.. 주의하세요.
	/*
	 * 아래와 같은 구문입니다.
	 
	 virtual TestLoaderLayer * createCCNode(CCNode * pParent, cocos2d::extension::CCBReader * pCCBReader)
	 {
	 return TestLoaderLayer::create();
	 }
	 */
	
	
};



