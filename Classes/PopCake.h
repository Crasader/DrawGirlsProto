#ifndef POPCAKE_H
#define POPCAKE_H

#include "cocos2d.h"
#include "DataStorageHub.h"

USING_NS_CC;

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
//		layer->setAnchorPoint(ccp(0.5,0));
//		layer->setScale(myDSH->screen_convert_rate);
//		layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
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
	
	MyNode* n;
	CommonButton* cb;
	CommonButton* cb2;
	void successOriginalAction();
	void failOriginalAction();
	void successRgbAction();
	void failRgbAction();
	//virtual void registerWithTouchDispatcher();
};

#endif
