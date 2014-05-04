//
//  BuyPiecePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#ifndef __DGproto__BuyPiecePopup__
#define __DGproto__BuyPiecePopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LoadingLayer;
class TouchSuctionLayer;
class KSLabelTTF;
class BuyPiecePopup : public CCLayer
{
public:
	static BuyPiecePopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func, int t_stage_number);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> buy_func;
	
	bool is_menu_enable;
	
	int stage_number;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCScale9Sprite* back_case;
	LoadingLayer* loading_layer;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func, int t_stage_number);
	
	void buyAction(CCObject* sender, CCControlEvent t_event);
};

#endif /* defined(__DGproto__BuyPiecePopup__) */
