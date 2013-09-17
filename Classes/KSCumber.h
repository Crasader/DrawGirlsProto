//
//  KSCumber.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#ifndef __DGproto__KSCumber__
#define __DGproto__KSCumber__

#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <vector>
#include "CumberEmotion.h"
#include "FromTo.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;

/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.
class KSCumber : public KSCumberBase
{
public:
	KSCumber() : m_speed(2.f), RADIUS(15.f), mEmotion(nullptr),
	LIMIT_COLLISION_PER_SEC(3), /// 초당 변수만큼 충돌시 스케일 줄임.
		FURY_DURATION(4.f), // 분노모드 초.
		teleportImg(NULL) // 텔레포트 이미지
	{
		m_state = (CUMBERSTATEMOVING);
	}
	virtual ~KSCumber(){}

	virtual void movingAndCrash(float dt);
	void normalMoving(float dt);
	void furyMoving(float dt);
	virtual void onStartMoving()
	{
		m_state = CUMBERSTATEMOVING;
		schedule(schedule_selector(KSCumberBase::movingAndCrash));
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
	}
	void attack(float dt);
	virtual bool init();
	CREATE_FUNC(KSCumber);
	virtual void setPosition(CCPoint t_sp)
	{
		KSCumberBase::setPosition(t_sp);
		gameData->setMainCumberPoint(ccp2ip(t_sp));
//		gameData->communication("Main_moveGamePosition", t_sp);
//		gameData->communication("VS_setMoveGamePosition", t_sp);
//		gameData->communication("Main_moveGamePosition", t_sp);
//		gameData->communication("Main_moveGamePosition", t_sp);
//		std::thread t1([](){;});
		
	}
	
	COLLISION_CODE crashWithX(IntPoint check_position);
	COLLISION_CODE crashLooper(const set<IntPoint>& v, IntPoint* cp);
	const float RADIUS;
	void showEmotion(EmotionType t_type)
	{
		if(mEmotion)
			mEmotion->selfRemove();
		mEmotion = Emotion::create(t_type, this, callfunc_selector(KSCumber::nullmEmotion));
		mEmotion->setPosition(ccp(30,20));
		addChild(mEmotion);
	}
	void nullmEmotion()
	{
		mEmotion = NULL;
	}
	void startDamageReaction(float userdata);
	virtual void startSpringCumber(float userdata){}
	virtual void startAnimationNoDirection();
	void damageReaction(float dt);
	void animationNoDirection(float dt);
	virtual void startAnimationDirection(){}
	virtual void onStartGame();
//	virtual void onEndGame();
	virtual void onPatternEnd();
	virtual void startInvisible();
	void invisibling(float dt);
	CCPoint getMissilePoint()
	{
		return getPosition() + ccp(0, 0);
	}
	virtual void getRandomPosition(IntPoint* ip, bool* finded);
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
	
	virtual void furyModeOn();
	void furyModeScheduler(float dt);
	virtual void furyModeOff();
	virtual void setGameover();
	void crashMapForPosition(CCPoint targetPt);
	void setCumberScale(float r)
	{
		m_scale.scale.init(m_scale.scale.getValue(), r, 0.005f);
	}
	float getCumberScale()
	{
		return m_scale.scale.getValue();
	}
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
		teleportImg->setScale(0);
		addChild(teleportImg);
		
		CCScaleTo* t_scale = CCScaleTo::create(0.2, 2.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(KSCumber::smaller));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		teleportImg->runAction(t_seq);
		AudioEngine::sharedInstance()->playEffect("sound_teleport.mp3",false);
	}
	virtual void smaller()
	{
		CCScaleTo* t_scale = CCScaleTo::create(0.2, 0.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(KSCumber::randomPosition));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
	}

protected:
	CCSprite* teleportImg;
	const float FURY_DURATION;
	const int LIMIT_COLLISION_PER_SEC; /// 초당 변수만큼 충돌시 스케일 줄임.
	bool isGameover;
	float m_speed;
//	CCSprite* m_headImg;
	CCSprite* m_headImg;
	CCBAnimationManager *mAnimationManager;
	int m_directionAngleDegree;
	IntPoint getMapPoint(CCPoint c){
		return IntPoint(round((c.x - 1) / pixelSize + 1.f),
						round((c.y - 1) / pixelSize + 1.f));
	}
	
	Well512 m_well512;
	Emotion* mEmotion;
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
	
	struct Invisible
	{
		int invisibleFrame;
		const int VISIBLE_FRAME;
		bool startInvisibleScheduler;
		float invisibleValue;
		Invisible() : VISIBLE_FRAME(300), startInvisibleScheduler(false){}
	}m_invisible;
	
	struct Scale
	{
		Scale() : SCALE_ADDER(0.1f), SCALE_SUBER(0.2f), scale(1.f, 1.f, 0.f),
		timer(0), autoIncreaseTimer(0), collisionStartTime(0), increaseTime(0),
		collisionCount(0){}
		const float SCALE_ADDER;
		const float SCALE_SUBER;
		int collisionCount; // 초당 충돌횟수 세기 위해
		float collisionStartTime;
		float timer; //
		
		float increaseTime;
		float autoIncreaseTimer;
		FromTo<float> scale; // 서서히 변하는것을 구현하기 위해.
	}m_scale;
	
	
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
};

#endif /* defined(__DGproto__KSCumber__) */
