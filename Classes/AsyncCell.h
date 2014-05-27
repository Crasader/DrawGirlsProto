#ifndef ASYNCCELL_H
#define ASYNCCELL_H

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;


class AsyncCell : public CCTableViewCell
{
public:
	AsyncCell();
	virtual ~AsyncCell();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static AsyncCell* create()
	{
		AsyncCell* t = new AsyncCell();
		t->init();
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
};

#endif
