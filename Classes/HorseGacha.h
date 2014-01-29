#pragma once

#include "cocos2d.h"
#include <functional>
USING_NS_CC;
#include "CCMenuLambda.h"
#include "KSAlertView.h"
#include "KSUtil.h"
#include <vector>
#include "Well512.h"
#include "ProbSelector.h"
#include "FromTo.h"
#include "KSUtil.h"
#include "StarGoldData.h"
#include "GachaPurchase.h"
#include "GachaReward.h"


class CCMenuItemToggleWithTopHorseLambda : public CCMenuItemToggleLambda
{
public:
	static CCMenuItemToggleWithTopHorseLambda * createWithTarget(std::function<void(CCObject*)> selector, CCMenuItemLambda* item, ...)
	{
		va_list args;
		va_start(args, item);
		CCMenuItemToggleWithTopHorseLambda *pRet = new CCMenuItemToggleWithTopHorseLambda();
		pRet->initWithTarget( selector, item, args);
		pRet->autorelease();
		va_end(args);
		return pRet;
	}
	CCMenuItemToggleLambda* m_hatTop;
	RewardSprite* m_reward;
};


#define __TYPE__ HorseGachaSub

enum class HorseSceneState
{
	kPutBoard,
	kUnjiHorseReward,
	kHorseSelect,
	kCanStart, // 스타트 가능한 상태
	kRun,
	kAllArive,
	kShowReward1,
	kFinish
};

class HorseSprite : public CCNode
{
public:
	CCSprite* m_horseSprite;
	float m_horseSpeed;
	std::deque<float> m_speedPerFrame; // 프레임 별로 가지는 스피드. 미리 정해논 스피드로 달림. 마음아픔.
	float m_totalDistance;
	bool m_arrive;	

};
class HorseGachaSub : public CCLayer
{
protected:
//	CCSprite* aHorse;
	GachaPurchaseStartMode m_gachaMode;

	CCPoint m_trackPosition;
	CCMenuLambda* m_menu;
	std::vector<int> m_arriveOrder;
	std::vector<CCPoint> m_horsePositions;
	std::vector<HorseSprite*> m_horses; // 말.
	std::vector<RewardSprite*> m_rewards; // 보상.
	mt19937 m_rEngine;                    // MT19937 난수 엔진
	int m_rankCounter;	
	Well512 m_well512;
	HorseSceneState m_state;
	float m_timer;
	//CCSprite* m_horseBoard;
	CCNode* m_horseBoardNode;
	int m_selectedHorseIndex;
	int m_alreadyDeterminantOrder; // 이미 정해져 있는 나의 등수... 슬프다.
	GachaCategory m_gachaCategory;
public:
	KSAlertView* m_parent;
	std::function<void(void)> m_callback;
	HorseGachaSub() : m_state(HorseSceneState::kPutBoard), m_timer(0), m_rankCounter(1)
	{
		
	}
	virtual ~HorseGachaSub()
	{
		
	}
	void setRewards(const std::vector<RewardSprite*> v) { m_rewards = v; }
	void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN + 1,true);
	}
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		
		return true;
	}
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	static __TYPE__* create(KSAlertView* av, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm ,
			GachaCategory gc) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(av, rs, gsm, gc))
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
	static __TYPE__* create(std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm,
			GachaCategory gc)  \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(callback, rs, gsm, gc))
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
	bool init(KSAlertView* av, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, 
			GachaCategory gc)
	{
		return init(av, nullptr, rs, gsm, gc);
	}
	bool init(std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, 
			GachaCategory gc)
	{
		return init(nullptr, callback, rs, gsm, gc);
	}
	virtual bool init(KSAlertView* av, std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, 
			GachaCategory gc);
	
	virtual void update(float dt);
	
	
};
#undef __TYPE__


//#define __TYPE__ HorseGacha
//class HorseGacha : public CCLayer
//{
//public:
	//std::function<void(void)> m_closeCallback;
	//HorseGacha();
	//virtual ~HorseGacha();
	////	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//virtual bool init(std::function<void(void)> closeCallback);
	//static __TYPE__* create(std::function<void(void)> closeCallback) \
	//{ \
    //__TYPE__ *pRet = new __TYPE__(); \
    //if (pRet && pRet->init(closeCallback))
    //{ \
			//pRet->autorelease(); \
			//return pRet; \
    //} \
    //else \
    //{ \
			//delete pRet; \
			//pRet = NULL; \
			//return NULL; \
    //} \
	//}
	
	////virtual void registerWithTouchDispatcher();
//};

//#undef __TYPE__



