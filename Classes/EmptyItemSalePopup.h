//
//  EmptyItemSalePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 29..
//
//

#ifndef __DGproto__EmptyItemSalePopup__
#define __DGproto__EmptyItemSalePopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"
#include "StarGoldData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class LoadingLayer;
class ASPopupView;
class EmptyItemSalePopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static EmptyItemSalePopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_purchase_func, PurchaseGuideType t_type);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> purchase_func;
	
	PurchaseGuideType m_type;
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	CCNode* m_container;
	CCSprite* back_case;
	CCControlButton* giveup_button;
	CCControlButton* purchase_button;
	CCControlButton* purchase_set_button;
	
	//	CCSprite* question_img;
	//	CCBAnimationManager* question_manager;
	//	CCSprite* rankup_rate_back;
	
	LoadingLayer* inapp_loading;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_purchase_func, PurchaseGuideType t_type);
	void giveupAction(CCObject* sender, CCControlEvent t_event);
	void purchaseAction(CCObject* sender, CCControlEvent t_event);
	void purchaseSetAction(CCObject* sender, CCControlEvent t_event);
	
	
//	void requestItemDelivery();
//	void resultSaveUserData(Json::Value result_data);
};

#endif /* defined(__DGproto__EmptyItemSalePopup__) */
