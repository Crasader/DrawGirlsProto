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
#include <random>


enum class RouletteState
{
	kPutBoard,
	kRotation,
	kStopping,
	kStoped,
	kQuestion
};

class RouletteSub : public CCLayer
{
protected:
	//	CCSprite* aHorse;
	GachaPurchaseStartMode m_gachaMode;
	
	CCMenuLambda* m_menu;
	std::vector<std::pair<float, RewardSprite*>> m_rewards; // 보상.
	mt19937 m_rEngine;                    // MT19937 난수 엔진
	
	Well512 m_well512;
	RouletteState m_state;
	float m_timer;

	int m_alreadyDeterminantOrder;
	CCNode* m_rotationBoard;
	CCSprite* m_circleBoard;
	CCSprite* m_guide;
	GachaCategory m_gachaCategory;
	//	float m_itemTopY[3];
public:
	KSAlertView* m_parentDialog;
	std::function<void(void)> m_callback;
	RouletteSub() : m_state(RouletteState::kPutBoard), m_timer(0)
	{
		
	}
	virtual ~RouletteSub()
	{
		
	}
//	void setRewards(const std::vector<RewardSprite*> v) { m_rewards = v; }
	void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN + 1,true);
	}
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		
		return true;
	}
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	static RouletteSub* create(KSAlertView* av, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, GachaCategory gc) \
	{ \
    RouletteSub *pRet = new RouletteSub(); \
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
	static RouletteSub* create(std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, GachaCategory gc)  \
	{ \
    RouletteSub *pRet = new RouletteSub(); \
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
	bool init(KSAlertView* av, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, GachaCategory gc)
	{
		return init(av, nullptr, rs, gsm, gc);
	}
	bool init(std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, GachaCategory gc)
	{
		return init(nullptr, callback, rs, gsm, gc);
	}
	virtual bool init(KSAlertView* av, std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm, GachaCategory gc);
	
	float diffDegree(float a, float b);
	virtual void update(float dt);
};




//class Roulette : public CCLayer
//{
//public:
	//std::function<void(void)> m_closeCallback;
	//Roulette();
	//virtual ~Roulette();
	////	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	//virtual bool init(std::function<void(void)> closeCallback);
	//static Roulette* create(std::function<void(void)> closeCallback) \
	//{ \
    //Roulette *pRet = new Roulette(); \
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


