//
//  ClearTimeEventContent.h
//  DGproto
//
//  Created by 사원3 on 2014. 6. 12..
//
//

#ifndef __DGproto__ClearTimeEventContent__
#define __DGproto__ClearTimeEventContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ClearTimeEventContent : public CCNode
{
public:
	static ClearTimeEventContent* create(int t_touch_priority, function<void(void)> t_no, function<void(void)> t_yes);
	
	void startShow();
	
private:
	int touch_priority;
	function<void(void)> no_selector;
	function<void(void)> yes_selector;
	
	function<void(void)> selected_selector;
	
	bool is_menu_enable;
	
	void noAction(CCObject* sender, CCControlEvent t_event);
	void yesAction(CCObject* sender, CCControlEvent t_event);
	
	void endShow()
	{
		is_menu_enable = true;
	}
	void startHide();
	void endHide()
	{
//		resume_selector();
	}
	
	void myInit(int t_touch_priority, function<void(void)> t_no, function<void(void)> t_yes);
};

#endif /* defined(__DGproto__ClearTimeEventContent__) */
