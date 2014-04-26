//
//  DetailConditionPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 25..
//
//

#ifndef __DGproto__DetailConditionPopup__
#define __DGproto__DetailConditionPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class DetailConditionPopup : public CCLayer
{
public:
	static DetailConditionPopup* create(int t_touch_priority, function<void()> t_end_func);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCScale9Sprite* back_case;
	
	void myInit(int t_touch_priority, function<void()> t_end_func);
	
	void closeAction(CCObject* sender, CCControlEvent t_event);
};

#endif /* defined(__DGproto__DetailConditionPopup__) */
