//
//  PuzzleSuccessAndPerfect.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 5..
//
//

#ifndef __DGproto__PuzzleSuccessAndPerfect__
#define __DGproto__PuzzleSuccessAndPerfect__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class PuzzleSuccessAndPerfect : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static PuzzleSuccessAndPerfect* create(int t_touch_priority, function<void()> t_end_func, bool t_is_success);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	bool is_success;
	
	TouchSuctionLayer* suction;
	CCSprite* gray;
	CCNode* m_container;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, bool t_is_success);
	void closeAction(CCObject* sender, CCControlEvent t_event);
};

#endif /* defined(__DGproto__PuzzleSuccessAndPerfect__) */
