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




enum class HorseRewardKind
{
	kGold,
	kRuby
};
class HorseRewardSprite : public CCSprite
{
public:
	virtual ~HorseRewardSprite(){}
	HorseRewardKind m_kind;
	int m_value; // 가치.
};

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
	HorseRewardSprite* m_reward;
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


class HorseGachaSub : public CCLayer
{
protected:
//	CCSprite* aHorse;
	CCPoint m_trackPosition;
	CCMenuLambda* m_menu;
	std::vector<int> m_arriveOrder;
	std::vector<CCPoint> m_horsePositions;
	std::vector<CCSprite*> m_horses; // 말.
	std::vector<HorseRewardSprite*> m_rewards; // 보상.
	Well512 m_well512;
	HorseSceneState m_state;
	float m_timer;
	CCSprite* m_horseBoard;
	CCNode* m_horseBoardNode;
	int m_selectedHorseIndex;
public:
	KSAlertView* m_parent;
	std::function<void(void)> m_callback;
	HorseGachaSub() : m_state(HorseSceneState::kPutBoard), m_timer(0)
	{
		
	}
	virtual ~HorseGachaSub()
	{
		
	}
	void setRewards(const std::vector<HorseRewardSprite*> v) { m_rewards = v; }
	void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN + 1,true);
	}
	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
	{
		
		return true;
	}
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	static __TYPE__* create(KSAlertView* av) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(av))
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
	static __TYPE__* create(std::function<void(void)> callback) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(callback))
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
	bool init(KSAlertView* av)
	{
		return init(av, nullptr);
	}
	bool init(std::function<void(void)> callback)
	{
		return init(nullptr, callback);
	}
	virtual bool init(KSAlertView* av, std::function<void(void)> callback);
	
	virtual void update(float dt);
	
	
};
#undef __TYPE__


#define __TYPE__ HorseGacha
class HorseGacha : public CCLayer
{
public:
	std::function<void(void)> m_closeCallback;
	HorseGacha();
	virtual ~HorseGacha();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(std::function<void(void)> closeCallback);
	static __TYPE__* create(std::function<void(void)> closeCallback) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(closeCallback))
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
	
	//virtual void registerWithTouchDispatcher();
};

#undef __TYPE__



