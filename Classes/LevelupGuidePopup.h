//
//  LevelupGuidePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 1..
//
//

#ifndef __DGproto__LevelupGuidePopup__
#define __DGproto__LevelupGuidePopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class LevelupGuidePopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static LevelupGuidePopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_rightnow_func);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> rightnow_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCScale9Sprite* back_case;
	CCControlButton* next_button;
	CCControlButton* rightnow_button;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_rightnow_func);
	void nextAction(CCObject* sender, CCControlEvent t_event);
	void rightnowAction(CCObject* sender, CCControlEvent t_event);
};

#endif /* defined(__DGproto__LevelupGuidePopup__) */
