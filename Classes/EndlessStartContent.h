//
//  EndlessStartContent.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 21..
//
//

#ifndef __DGproto__EndlessStartContent__
#define __DGproto__EndlessStartContent__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

//class KSLabelTTF;
class EndlessStartContent : public CCNode//, public CCBAnimationManagerDelegate
{
public:
	static EndlessStartContent* create(int t_touch_priority, function<void(CCObject*)> t_selector);
	
//	virtual void completedAnimationSequenceNamed (char const * name);
	function<void()> touch_func;
	
private:
	int touch_priority;
	function<void(CCObject*)> end_selector;
	
	bool is_menu_enable;
	
	void menuAction(CCObject* sender);
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector);
};

#endif /* defined(__DGproto__EndlessStartContent__) */
