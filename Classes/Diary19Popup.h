//
//  Diary19Popup.h
//  DGproto
//
//  Created by 사원3 on 2014. 9. 5..
//
//

#ifndef __DGproto__Diary19Popup__
#define __DGproto__Diary19Popup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "jsoncpp/json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class LoadingLayer;
class Diary19Popup : public CCLayer
{
public:
	static Diary19Popup* create(int t_touch_priority, function<void()> t_end_func, bool t_is_ingame = false);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	bool is_ingame;
	CCPoint center_position;
	
	TouchSuctionLayer* suction;
	LoadingLayer* t_loading;
	
	CCSprite* gray;
	CCNode* m_container;
	CCSprite* back_case;
	CCScale9Sprite* back_in;
	
	void buttonAction(CCObject* sender, CCControlEvent t_event);
	void resultMakeDiaryCode(Json::Value result_data);
	void myInit(int t_touch_priority, function<void()> t_end_func, bool t_is_ingame);
};

#endif /* defined(__DGproto__Diary19Popup__) */
