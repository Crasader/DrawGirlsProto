//
//  LoadingLayer.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 22..
//
//

#ifndef __DGproto__LoadingLayer__
#define __DGproto__LoadingLayer__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class LoadingLayer : public CCLayer
{
public:
	static LoadingLayer* create(int t_touch_priority = -500, bool is_ingame = false);
	void startLoading();
	virtual void onEnter();
private:
	CCSprite* gray;
	CCLabelTTF* loading_label;
	void myInit(int t_touch_priority, bool is_ingame);
	
	int touch_priority;
	
	string loading_text;
	int animation_frame;
	void loadingAnimation();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__LoadingLayer__) */
