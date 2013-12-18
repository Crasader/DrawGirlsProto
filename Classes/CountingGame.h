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
	CountingGame()
	{}
	virtual ~CountingGame()
	{
		
	}
	static __TYPE__* create()
	{
    __TYPE__ *pRet = new __TYPE__();
    if (pRet && pRet->init())
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
		CountingGame *layer = CountingGame::create();
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
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, INT_MIN,true);
	}
	bool init();
	void update(float dt);
	void createObject(float dt);
protected:
	std::mt19937 m_rEngine;
	std::vector<CCSprite*> m_objects;
	int m_goalCount;
	CCMenuLambda* m_menu;
};
#undef __TYPE__

