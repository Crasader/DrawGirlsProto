//
//  TouchSuctionLayer.h
//  DGproto
//
//  Created by 사원3 on 2014. 1. 9..
//
//

#ifndef __DGproto__TouchSuctionLayer__
#define __DGproto__TouchSuctionLayer__

#include "cocos2d.h"

USING_NS_CC;

class TouchSuctionLayer : public CCLayer
{
public:
	static TouchSuctionLayer* create(int t_touch_priority);
	CCObject* target_touch_began;
	SEL_CallFunc delegate_touch_began;
	void setNotSwallowRect(CCRect t_rect);
private:
	void myInit(int t_touch_priority);
	int touch_priority;
	CCRect not_swallow_rect;
	bool is_setted_not_swallow_rect;
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__TouchSuctionLayer__) */
