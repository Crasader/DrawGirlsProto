//
//  CardLockedPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#ifndef __DGproto__CardLockedPopup__
#define __DGproto__CardLockedPopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class CardLockedPopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static CardLockedPopup* create(int t_touch_priority, function<void()> t_end_func, int t_step);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCSprite* back_case;
	CCControlButton* close_button;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, int t_step);
	void closeAction(CCObject* sender, CCControlEvent t_event);
};

#endif /* defined(__DGproto__CardLockedPopup__) */
