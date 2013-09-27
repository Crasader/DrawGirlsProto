

//
//  Cherry.h
//  DGproto
//
//  Created by ksoo k on 13. 9. 6..
//
//

#pragma once

#include "KSCumberBase.h"
#include "GameData.h"
#include "Well512.h"
#include <vector>
#include "CumberEmotion.h"
#include "FromTo.h"
#include "cocos-ext.h"
using namespace cocos2d::extension;

/// KSCumberBase 로 부터 derived 된 클래스가 몬스터의 이미지를 가져야 할 듯 싶다.
class Cherry : public KSCumberBase
{
public:
	Cherry() : RADIUS(15.f), mEmotion(nullptr),
	
	FURY_DURATION(4.f), // 분노모드 초.
	teleportImg(NULL) // 텔레포트 이미지
	{
		m_state = (CUMBERSTATEMOVING);
	}
	virtual ~Cherry(){}
	
	void normalMoving(float dt);
	
	
	virtual void onStartMoving()
	{
		m_state = CUMBERSTATEMOVING;
		schedule(schedule_selector(KSCumberBase::movingAndCrash));
	}
	virtual void onStopMoving()
	{
		m_state = CUMBERSTATESTOP;
	}
	void cumberAttack(float dt);
	virtual bool init();
	CREATE_FUNC(Cherry);
	virtual void setPosition(const CCPoint& t_sp)
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
		mEmotion = Emotion::create(t_type, this, callfunc_selector(Cherry::nullmEmotion));
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
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Cherry::smaller));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		teleportImg->runAction(t_seq);
		AudioEngine::sharedInstance()->playEffect("sound_teleport.mp3",false);
	}
	virtual void smaller()
	{
		CCBlink* t_scale = CCBlink::create(0.5, 8);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(Cherry::randomPosition));
		
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		
		runAction(t_seq);
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
protected:
	CCSprite* teleportImg;
	const float FURY_DURATION;
	
	bool isGameover;
	int lastCastNum;
	//	CCSprite* m_headImg;
	CCSprite* m_headImg;
	CCBAnimationManager *mAnimationManager;
	IntPoint getMapPoint(CCPoint c){
		return IntPoint(round((c.x - 1) / pixelSize + 1.f),
						round((c.y - 1) / pixelSize + 1.f));
	}
	
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
	
	
	
	
	
};

