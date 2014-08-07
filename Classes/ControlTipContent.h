//
//  ControlTipContent.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 16..
//
//

#ifndef __DGproto__ControlTipContent__
#define __DGproto__ControlTipContent__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

//class KSLabelTTF;
class CommonButton;
class ControlTipContent : public CCNode, public CCBAnimationManagerDelegate
{
public:
	static ControlTipContent* create(int t_touch_priority, function<void(CCObject*)> t_selector, int t_code);
	
	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	CCMenu* close_menu;
	function<void(CCObject*)> end_selector;
	
	bool is_first;
	
	bool is_menu_enable;
	CCSprite* close_button;
	CCBAnimationManager* ccb_manager;
	CCClippingNode* area_take_sample;
	
	void menuAction(CCObject* sender);
	string getCCBfilenameForTipNumber(int t_code);
	int getTitleLocalKeyForTipNumber(int t_code);
	int getContentLocalKeyForTipNumber(int t_code);
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector, int t_code);
};

#endif /* defined(__DGproto__ControlTipContent__) */
