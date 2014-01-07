#pragma once

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "Well512.h"
#include "CCMenuLambda.h"
#include <random>
USING_NS_CC;
#define __TYPE__ CountingGame
class CountingGame : public CCLayer
{
public:
	CountingGame() : m_remainTime(5)
	{}
	virtual ~CountingGame()
	{
		
	}
	static CountingGame* create(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
	{
    CountingGame* pRet = new CountingGame();
    if (pRet && pRet->init(priority, hideFunction))
    {
			pRet->autorelease();
			return pRet;
    }
    else
    {
			delete pRet;
			pRet = NULL;
			return NULL;
    }
	}
	static CCScene* scene()
	{
		// 'scene' is an autorelease object
		CCScene *scene = CCScene::create();
		
		// 'layer' is an autorelease object
		CountingGame *layer = CountingGame::create(0, nullptr);
		layer->setAnchorPoint(ccp(0.5,0));
		layer->setScale(myDSH->screen_convert_rate);
		layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));

		scene->addChild(layer);
		return scene;
	}
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher()
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_priority,true);
	}
	void startSchedule();
	bool init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction);
	void update(float dt);
	void createObject(float dt);
protected:

	CCLabelBMFont* m_timeFnt;
	int m_startTime;
	int m_remainTime;
	CCClippingNode* m_thiz;
	int m_priority;
	std::function<void(CCObject*, SEL_CallFunc)> m_hideFunction;
	float remainTime;
	std::mt19937 m_rEngine;
	std::vector<CCSprite*> m_objects;
	int m_goalCount;
	CCMenuLambda* m_menu;
};
#undef __TYPE__

