//
//  RivalSelectPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 31..
//
//

#ifndef __DGproto__RivalSelectPopup__
#define __DGproto__RivalSelectPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class ASPopupView;
class KSLabelTTF;
class CCMenuLambda;
class RivalSelectPopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static RivalSelectPopup* create(int t_touch_priority, function<void()> t_cancel_func, function<void()> t_selected_func);
	
private:
	int touch_priority;
	function<void()> cancel_func;
	function<void()> selected_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	CCNode* m_container;
	CCSprite* back_case;
	KSLabelTTF* title_label;
	CCNode* left_node;
	CCNode* center_node;
	CCNode* right_node;
	
	CCScale9Sprite* left_case;
	CCScale9Sprite* center_case;
	CCScale9Sprite* right_case;
	
	CCMenuLambda* select_menu;
	
	void startRivalAnimation();
	void selectRival(int t_index);
	void myInit(int t_touch_priority, function<void()> t_cancel_func, function<void()> t_selected_func);
};

#endif /* defined(__DGproto__RivalSelectPopup__) */
