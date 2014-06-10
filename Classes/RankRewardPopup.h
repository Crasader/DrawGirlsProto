//
//  RankRewardPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 6. 10..
//
//

#ifndef __DGproto__RankRewardPopup__
#define __DGproto__RankRewardPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class RankRewardPopup : public CCLayer
{
public:
	static RankRewardPopup* create(int t_touch_priority, function<void()> t_end_func);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	CCNode* m_container;
	CCScale9Sprite* back_case;
	
	CCNode* getRewardImg(string t_goods_str);
	CCNode* getTakeRewardImg(string t_goods_str);
	CCNode* getTakeRewardScript(Json::Value reward_list);
	
	void myInit(int t_touch_priority, function<void()> t_end_func);
};

#endif /* defined(__DGproto__RankRewardPopup__) */
