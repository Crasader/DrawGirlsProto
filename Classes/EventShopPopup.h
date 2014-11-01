//
//  EventShopPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 30..
//
//

#ifndef __DGproto__EventShopPopup__
#define __DGproto__EventShopPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class LoadingLayer;
class EventShopPopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static EventShopPopup* create(int t_touch_priority, function<void()> t_end_func);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCSprite* back_case;
	
	LoadingLayer* loading_layer;
	
	void myInit(int t_touch_priority, function<void()> t_end_func);
	
	void menuAction(CCObject* sender);
	void controlAction(CCObject* sender, CCControlEvent t_event);
	void createCheckBuyPopup(function<void()> buy_action);
	void requestItemDelivery();
	
	//	void resultSaveUserData(Json::Value result_data);
};

#endif /* defined(__DGproto__EventShopPopup__) */
