//
//  JoystickPositionSelectPopup.h
//  DGproto
//
//  Created by ParkJeongSoon on 2014. 9. 22..
//
//

#ifndef __DGproto__JoystickPositionSelectPopup__
#define __DGproto__JoystickPositionSelectPopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class JoystickPositionSelectPopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static JoystickPositionSelectPopup* create(int t_touch_priority, function<void()> t_end_func);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCSprite* back_case;
	CCControlButton* close_button;
	
	void myInit(int t_touch_priority, function<void()> t_end_func);
	void closeAction(CCObject* sender, CCControlEvent t_event);
    void leftAction(CCObject* sender, CCControlEvent t_event);
    void rightAction(CCObject* sender, CCControlEvent t_event);
};

#endif /* defined(__DGproto__JoystickPositionSelectPopup__) */
