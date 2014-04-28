#ifndef GABABO_H
#define GABABO_H

#include "cocos2d.h"
USING_NS_CC;

#include "DataStorageHub.h"
#include "CCMenuLambda.h"



static int 	kAttackGa = 1;
static int	kAttackBa = 2;
static int	kAttackBo = 3; 
class GaBaBo : public CCLayer
{
public:
	GaBaBo();
	virtual ~GaBaBo();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static GaBaBo* create()
	{
		GaBaBo* t = new GaBaBo();
		t->init();
		t->autorelease();
		return t;
	}
	static CCScene* scene()
	{
		// 'scene' is an autorelease object
		CCScene *scene = CCScene::create();
		
		// 'layer' is an autorelease object
		GaBaBo *layer = GaBaBo::create();
		layer->setAnchorPoint(ccp(0.5,0));
		layer->setScale(myDSH->screen_convert_rate);
		layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
		
		scene->addChild(layer);
		return scene;
	}
	void update(float dt);
	void initGameTime()
	{
		m_remainTime = 9.f;
	}
	void loadImage(int step);
	int getCurrentTimeStamp();
	//virtual void registerWithTouchDispatcher();
protected:
	CCMenuItemToggleLambda* m_ba, *m_ga, *m_bo;
	int m_computerThink;
	std::map<int, CCSprite*> m_computerThinkSprites;
	float m_remainTime;
	bool m_resultShowing;
	CCLabelBMFont* m_remainTimeFnt;
	CCSprite* m_stepSprite;
	int m_step;
	long long m_lastChangeTime;
	int m_winCount, m_drawCount, m_loseCount;
};

#endif
