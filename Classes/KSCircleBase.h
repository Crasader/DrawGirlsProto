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

class CircleCCBPieceBase : public KSCumberBase, public CCBAnimationManagerDelegate
{
public:
	virtual void runTimeline(Json::Value patternInfo) = 0;
};
/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.
class KSCircleBase : public CircleCCBPieceBase, public CCBSelectorResolver
{
public:
	virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName){
		CCLog("%s", pSelectorName);
		return NULL;
	}
	virtual SEL_CallFuncN onResolveCCBCCCallFuncSelector(CCObject * pTarget, const char* pSelectorName) {
		CCLog("%s", pSelectorName);
		return NULL;
	}
	virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
	{
		CCLog("%s", pSelectorName);
		return NULL;
	}
public:
	virtual void runTimeline(Json::Value patternInfo)
	{
		if(currentTimelineFooter == "") // 아무것도 공격중이 아니면 발싸 !!
		{
			std::string timeline = patternInfo["pattern"].asString();
			m_atype = patternInfo.get("atype", "special").asString();
			m_repeatNumber = patternInfo.get("repeat", 1).asInt();
			m_attackCanceled = false;
			currentTimeline = timeline;
			currentTimelineFooter = "_b";
			mAnimationManager->runAnimationsForSequenceNamed((currentTimeline + currentTimelineFooter).c_str());
		}
		
	}
	
	virtual void completedAnimationSequenceNamed(const char *name_)
	{
		string name = name_;
		if(name.size() < 2)
			return;
		char lastChar = name[name.size() - 1];
		char lastPrevChar = name[name.size() - 2];
		std::string tl(name.begin(), name.end() - 2);
		if(lastPrevChar != '_')
			return;
		if(lastChar == 'b')
		{
			// 캔슬이 되었는지의 여부를 알아야 됨.
			// middle 액션을 하기전에 속성을 설정함.
			
			if(m_atype == "crash")
			{
				AudioEngine::sharedInstance()->stopEffect("sound_casting_crash.mp3");
			}
			else if(m_atype == "special")
			{
				AudioEngine::sharedInstance()->stopEffect("sound_casting_option.mp3");
			}
			else // normal
			{
				AudioEngine::sharedInstance()->stopEffect("sound_casting_attack.mp3");
			}
			
			if(m_attackCanceled) // 맞아서 캔슬이 되었다면
			{
				currentTimelineFooter = "_e";
				mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
			}
			else
			{
				currentTimelineFooter = "_m";
				mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
			}
		}
		else if(lastChar == 'm')
		{
			
			// 반복을 시킬 건지 검사하고 캔슬이 되었다면 캔슬 동작을 작동시킴.
			// 캔슬이면 속성을 해제함.
			if(m_attackCanceled) // 캔슬
			{
				currentTimelineFooter = "_e";
				mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
			}
			else
			{
				m_repeatNumber--;
				if(m_repeatNumber > 0)
				{
					currentTimelineFooter = "_m";
					mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
				}
				else
				{
					currentTimelineFooter = "_e";
					mAnimationManager->runAnimationsForSequenceNamed((tl + currentTimelineFooter).c_str());
				}
			}
		}
		else if(lastChar == 'e')
		{
			currentTimelineFooter = "";
			m_state = CUMBERSTATEMOVING;
			mAnimationManager->runAnimationsForSequenceNamed("Default Timeline");
			myGD->communication("MS_resetRects", false);
		}
	}
	KSCircleBase() : RADIUS(15.f)
	{
		m_state = (CUMBERSTATEMOVING);
	}
	virtual ~KSCircleBase(){
		
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
	virtual void stopCasting()
	{
		myGD->communication("MP_bombCumber", this);
		// 방사형으로 돌아가고 있는 중이라면
		if(m_state == CUMBERSTATENODIRECTION)
		{
			CCLog("m_state == CUMBERSTATENODIRECTION");
			m_noDirection.state = 2; // 돌아가라고 상태 변경때림.
		}
	}
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
	virtual void attackBehavior(Json::Value _pattern)
	{
		std::string pattern = _pattern["pattern"].asString();
		if(pattern == "109")
		{
			m_state = CUMBERSTATESTOP;
		}
		else if( pattern == "1007")
		{
			m_state = CUMBERSTATESTOP;
		}
		else if(pattern.size() >= 2 && pattern[0] == 'a' && pattern[1] == 't') // ccb 관련 공격.
		{
			m_state = CUMBERSTATESTOP;
			//			startAnimationNoDirection();
		}
		else
		{
			std::string atype = _pattern.get("atype", "normal").asString();
			std::map<std::string, int> mapper = {{"normal", 1}, {"special", 2}, {"crash", 3}};
			if(mapper.find(atype) == mapper.end())
				lastCastNum = 1;
			else
				lastCastNum = mapper[atype];
			
			mAnimationManager->runAnimationsForSequenceNamed(CCString::createWithFormat("cast%dstart", lastCastNum)->getCString());
			
			std::string target = _pattern.get("target", "no").asString();
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
	std::string m_atype;
	std::string currentTimeline;
	std::string currentTimelineFooter; // _b _m _e 같은것들.
	bool m_attackCanceled;
	int m_repeatNumber;
	bool isGameover;
	int lastCastNum;
	//	CCSprite* m_headImg;
	CircleBossCCB* m_headImg;
	void update(float dt)
	{
		
		CCNode* endP = myGD->getCommunicationNode("Main_gameNodePointer");
		auto function = [&](CCNode* node)->CCAffineTransform
		{
			CCAffineTransform t = this->nodeToParentTransform();
			for (CCNode *p = node->getParent(); p != endP; p = p->getParent())
			{
				if( p == NULL)
					return CCAffineTransformIdentity;
				else
					t = CCAffineTransformConcat(t, p->nodeToParentTransform());
			}
			return t;
		};
		
		
		
		for(auto i : m_headImg->m_s)
		{
			CCPoint ret = CCPointApplyAffineTransform(i->getPosition(), function(i));
			if(currentTimelineFooter == "_m")
			{
				if(m_atype == "crash")
				{
					crashMapForPosition(ret);
				}
				else if(m_atype == "normal")
				{
					IntPoint bulletPoint = ccp2ip(ret);
					IntPoint jackPoint = myGD->getJackPoint();
					int sizex = i->getContentSize().width * getCumberScale() * 1.2f / 2.f;
					int sizey = i->getContentSize().height * getCumberScale() * 1.2 / 2.f;
					auto mapState = myGD->mapState;
					
					//					if(myGD->getJackState() == jackStateDrawing &&
					//						 (bulletPoint - jackPoint).length() <= sizex)
					//					{
					//						myGD->communication("CP_jackCrashDie");
					//						myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
					//					}
					//					else
					{
						for(int y = bulletPoint.y - sizey; y<=bulletPoint.y + sizey; y++)
						{
							for(int x = bulletPoint.x - sizex; x<=bulletPoint.x + sizex; x++)
							{
								//CCLog("\t %d %d", x, y);
								if(IntPoint(x, y).isInnerMap() &&
									 mapState[x][y] == mapType::mapNewline)
								{
									myGD->communication("PM_addPathBreaking", IntPoint(x, y));
									goto end;
								}
							}
						}
					end:
						;
					}
					
				}
				else if(m_atype == "special")
				{
					IntPoint bulletPoint = ccp2ip(ret);
					IntPoint jackPoint = myGD->getJackPoint();
					if( (bulletPoint - jackPoint).length() <= 2)
					{
						myGD->communication("CP_jackCrashDie");
						myGD->communication("Jack_startDieEffect", DieType::kDieType_other);
					}
				}
			}
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



