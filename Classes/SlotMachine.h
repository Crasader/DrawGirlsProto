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




#define __TYPE__ SlotMachineSub

enum class SlotMachineState
{
	kPutBoard,
	kScrolling1,
	kScrolling2,
	kScrolling3,
	kShow,
	kQuestion
};

class SlotMachineSub : public CCLayer
{
protected:
//	CCSprite* aHorse;
	GachaPurchaseStartMode m_gachaMode;
	
	CCMenuLambda* m_menu;
	std::vector<RewardSprite*> m_rewards; // 보상.
	std::vector<std::string> m_rewardItems; // 보상 아이템들.
	mt19937 m_rEngine;                    // MT19937 난수 엔진
	
	Well512 m_well512;
	SlotMachineState m_state;
	float m_timer;
//	CCNode* m_slotNode[3];
	CCClippingNode* m_clipSlot[3];
	CCNode* m_nodeOfItems[3];
	int m_alreadyDeterminantOrder;
	std::vector<int> m_determinantSlotItems;
	int m_prevSlotIndex[3];
	bool m_firstSlotStopFirst;
	bool m_firstSlotStopSecond;
	bool m_firstSlotStopThird;
//	float m_itemTopY[3];
public:
	KSAlertView* m_parent;
	std::function<void(void)> m_callback;
	SlotMachineSub() : m_state(SlotMachineState::kPutBoard), m_timer(0)
	{
		
	}
	virtual ~SlotMachineSub()
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
	static __TYPE__* create(KSAlertView* av, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm = kGachaPurchaseStartMode_reward) \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(av, rs, gsm))
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
	static __TYPE__* create(std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm = kGachaPurchaseStartMode_reward)  \
	{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->init(callback, rs, gsm))
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
	bool init(KSAlertView* av, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm)
	{
		return init(av, nullptr, rs, gsm);
	}
	bool init(std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm)
	{
		return init(nullptr, callback, rs, gsm);
	}
	virtual bool init(KSAlertView* av, std::function<void(void)> callback, const vector<RewardSprite*>& rs, GachaPurchaseStartMode gsm);
	
	virtual void update(float dt);
	std::vector<int> rotationSlot(const std::vector<int>& slots, float scrollSpeed);
	
};
#undef __TYPE__



class SlotMachine : public CCLayer
{
public:
	std::function<void(void)> m_closeCallback;
	SlotMachine();
	virtual ~SlotMachine();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(std::function<void(void)> closeCallback);
	static SlotMachine* create(std::function<void(void)> closeCallback) \
	{ \
    SlotMachine *pRet = new SlotMachine(); \
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


