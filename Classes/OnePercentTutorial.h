#ifndef ONEPERCENTTUTORIAL_H
#define ONEPERCENTTUTORIAL_H

#include "cocos2d.h"
USING_NS_CC;


class OnePercentTutorial : public CCLayer
{
public:
	OnePercentTutorial();
	virtual ~OnePercentTutorial();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static OnePercentTutorial* create()
	{
		OnePercentTutorial* t = new OnePercentTutorial();
		t->init();
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
};

#endif
