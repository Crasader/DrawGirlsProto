//
//  ItemGachaPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 21..
//
//

#ifndef __DGproto__ItemGachaPopup__
#define __DGproto__ItemGachaPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class LoadingLayer;
class ItemGachaPopup : public CCLayer, public CCBAnimationManagerDelegate
{
public:
	static ItemGachaPopup* create(int t_touch_priority, function<void()> t_end_func, function<void(int)> t_gacha_on_func);
	
	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void(int)> gacha_on_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCScale9Sprite* back_case;
	CCControlButton* regacha_button;
	CCControlButton* use_button;
	
	int item_type;
	CCSprite* item_img;
	CCSprite* question_img;
	CCBAnimationManager* question_manager;
	
	LoadingLayer* loading_layer;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void(int)> t_gacha_on_func);
	void useAction(CCObject* sender, CCControlEvent t_event);
	void regachaAction(CCObject* sender, CCControlEvent t_event);
	
	void resultSaveUserData(Json::Value result_data);
};

#endif /* defined(__DGproto__ItemGachaPopup__) */
