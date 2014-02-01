#ifndef GOLDCLICKER_H
#define GOLDCLICKER_H

#include "cocos2d.h"
USING_NS_CC;


class GoldClicker : public CCLayer
{
public:
	GoldClicker();
	virtual ~GoldClicker();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction);
	static GoldClicker* create(int priority, const std::function<void(CCObject*, SEL_CallFunc)>& hideFunction)
	{
		GoldClicker* t = new GoldClicker();
		t->init(priority, hideFunction);
		t->autorelease();
		return t;
	}
	void startSchedule()
	{
	}
	//virtual void registerWithTouchDispatcher();
};

#endif
