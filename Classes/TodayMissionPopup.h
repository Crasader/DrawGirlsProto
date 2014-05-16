//
//  TodayMissionPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 2..
//
//

#ifndef __DGproto__TodayMissionPopup__
#define __DGproto__TodayMissionPopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class TodayMissionPopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static TodayMissionPopup* create(int t_touch_priority, function<void()> t_end_func);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	
	CCNode* m_container;
	CCScale9Sprite* back_case;
	CCControlButton* close_button;
	CCProgressTimer* progress_bar;
	
	void myInit(int t_touch_priority, function<void()> t_end_func);
	void closeAction(CCObject* sender, CCControlEvent t_event);
};


#endif /* defined(__DGproto__TodayMissionPopup__) */
