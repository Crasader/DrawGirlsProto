//
//  EndlessSeqWinRewardPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 6. 16..
//
//

#ifndef __DGproto__EndlessSeqWinRewardPopup__
#define __DGproto__EndlessSeqWinRewardPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TouchSuctionLayer;
class EndlessSeqWinRewardPopup : public CCLayer
{
public:
	static EndlessSeqWinRewardPopup* create(int t_touch_priority, function<void()> t_end_func, Json::Value t_reward);
	
private:
	int touch_priority;
	function<void()> end_func;
	
	bool is_menu_enable;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	CCNode* m_container;
	CCScale9Sprite* back_case;
	CCControlButton* close_button;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, Json::Value t_reward);
	CCNode* getRewardNode(string t_type);
	string getRewardString(string t_type, int t_count);
};

#endif /* defined(__DGproto__EndlessSeqWinRewardPopup__) */
