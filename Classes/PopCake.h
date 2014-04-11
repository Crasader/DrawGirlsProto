#ifndef POPCAKE_H
#define POPCAKE_H

#include "cocos2d.h"
#include "DataStorageHub.h"

USING_NS_CC;

#define ZS_SCROLL_SPEED_MAX_BASE	20
#define ZS_SCROLL_SPEED_DECEASE_BASE	0.2f
class MyNode;
class CommonButton;
class PopCake : public CCLayer
{
public:
	static CCScene* scene()
	{
		// 'scene' is an autorelease object
		CCScene *scene = CCScene::create();

		// 'layer' is an autorelease object
		PopCake *layer = PopCake::create();
		layer->setAnchorPoint(ccp(0.5,0));
		layer->setScale(myDSH->screen_convert_rate);
		layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
		// add layer as a child to scene
		scene->addChild(layer);

		// return the scene
		return scene;
	}
	PopCake();
	virtual ~PopCake();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static PopCake* create()
	{
		PopCake* t = new PopCake();
		t->init();
		t->autorelease();
		return t;
	}
	
	void successOriginalAction();
	void failOriginalAction();
	void successRgbAction();
	void failRgbAction();
	//virtual void registerWithTouchDispatcher();
	void moveAnimation();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	//virtual void registerWithTouchDispatcher(void);
	void moveListXY(CCPoint t_p);
	void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();

		pDispatcher->addStandardDelegate(this, -250);
	}
protected:

	CCNode* game_node;
	MyNode* n;
	CommonButton* cb;
	CommonButton* cb2;
	map<int, CCPoint> multiTouchData;
	CCPoint touch_p;
    long touchStartTime;
	long first_touch_time;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	bool isAnimated;
	bool is_spin_mode;
	float minimum_scale;
	CCSize screen_size;
	bool is_scrolling;
	std::string formSetter;
};

#endif
