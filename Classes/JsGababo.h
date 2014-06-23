#ifndef JSGABABO_H
#define JSGABABO_H

#include "cocos2d.h"
USING_NS_CC;


class JsGababo : public CCLayer
{
public:
	JsGababo();
	virtual ~JsGababo();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static JsGababo* create()
	{
		JsGababo* t = new JsGababo();
		t->init();
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
};

#endif
