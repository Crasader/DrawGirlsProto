//
//  GachaDetailPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 10. 28..
//
//

#ifndef __DGproto__GachaDetailPopup__
#define __DGproto__GachaDetailPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "jsoncpp/json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class GachaDetailPopup : public CCLayer
{
public:
	static GachaDetailPopup* create(int t_touch_priority, Json::Value t_goods_info, function<void()> t_end_func);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	
	TouchSuctionLayer* suction;
	
	CCNode* m_container;
	CCSprite* back_case;
	
	Json::Value goods_info;
	
	void myInit(int t_touch_priority, Json::Value t_goods_info, function<void()> t_end_func);
};

#endif /* defined(__DGproto__GachaDetailPopup__) */
