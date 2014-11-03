//
//  GachaRefreshCheckPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 11. 3..
//
//

#ifndef __DGproto__GachaRefreshCheckPopup__
#define __DGproto__GachaRefreshCheckPopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class GachaRefreshCheckPopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static GachaRefreshCheckPopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_action_func);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> action_func;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCSprite* back_case;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_action_func);
	void closeAction(CCObject* sender, CCControlEvent t_event);
	void actionAction(CCObject* sender, CCControlEvent t_event);
};

#endif /* defined(__DGproto__GachaRefreshCheckPopup__) */
