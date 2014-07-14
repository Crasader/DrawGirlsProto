//
//  BuyMorphingPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 22..
//
//

#ifndef __DGproto__BuyMorphingPopup__
#define __DGproto__BuyMorphingPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class LoadingLayer;
class BuyMorphingPopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static BuyMorphingPopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> buy_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	LoadingLayer* buy_loading;
	
	CCSprite* gray;
	CCNode* m_container;
	CCSprite* back_case;
	CCControlButton* cancel_button;
	CCControlButton* buy_button;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func);
	void cancelAction(CCObject* sender, CCControlEvent t_event);
	void buyAction(CCObject* sender, CCControlEvent t_event);
	void resultBuyMorphing(Json::Value result_data);
};

#endif /* defined(__DGproto__BuyMorphingPopup__) */
