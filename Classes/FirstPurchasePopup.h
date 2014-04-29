//
//  FirstPurchasePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 29..
//
//

#ifndef __DGproto__FirstPurchasePopup__
#define __DGproto__FirstPurchasePopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class LoadingLayer;
class ASPopupView;
class FirstPurchasePopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static FirstPurchasePopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_purchase_func);
	
//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> purchase_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCScale9Sprite* back_case;
	CCControlButton* giveup_button;
	CCControlButton* purchase_button;
	
//	CCSprite* question_img;
//	CCBAnimationManager* question_manager;
//	CCSprite* rankup_rate_back;
	
	LoadingLayer* inapp_loading;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_purchase_func);
	void giveupAction(CCObject* sender, CCControlEvent t_event);
	void purchaseAction(CCObject* sender, CCControlEvent t_event);
	
	void requestItemDelivery();
	void resultSaveUserData(Json::Value result_data);
};

#endif /* defined(__DGproto__FirstPurchasePopup__) */
