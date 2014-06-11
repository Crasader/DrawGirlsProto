//
//  RankUpPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 28..
//
//

#ifndef __DGproto__RankUpPopup__
#define __DGproto__RankUpPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class LoadingLayer;
class ASPopupView;
class RankUpPopup : public CCLayer, public CCBAnimationManagerDelegate
{
public:
	static RankUpPopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_rankup_func, function<void()> t_success_func, function<void()> t_fail_func, bool t_is_time_event = false);
	
	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> rankup_func;
	function<void()> success_func;
	function<void()> fail_func;
	
	bool is_menu_enable;
	bool is_time_event;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	
	CCNode* m_container;
	CCScale9Sprite* back_case;
	CCControlButton* giveup_button;
	CCControlButton* rankup_button;
	
	CCSprite* question_img;
	CCBAnimationManager* question_manager;
	CCSprite* rankup_rate_back;
	
	LoadingLayer* inapp_loading;
	ASPopupView* t_popup;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_rank_up_func, function<void()> t_success_func, function<void()> t_fail_func, bool t_is_time_event);
	void giveupAction(CCObject* sender, CCControlEvent t_event);
	void rankupAction(CCObject* sender, CCControlEvent t_event);
	
	void rankupAnimation();
	
	void requestItemDelivery();
	
	void resultSaveUserData(Json::Value result_data);
};

#endif /* defined(__DGproto__RankUpPopup__) */
