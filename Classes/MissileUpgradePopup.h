//
//  MissileUpgradePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 20..
//
//

#ifndef __DGproto__MissileUpgradePopup__
#define __DGproto__MissileUpgradePopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"
#include "EnumDefine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class LoadingLayer;
class TouchSuctionLayer;
class KSLabelTTF;
class MissileUpgradePopup : public CCLayer
{
public:
	static MissileUpgradePopup* create(int t_touch_priority, function<void()> t_end_func, function<void()> t_upgrade_func);
	
private:
	int touch_priority;
	function<void()> end_func;
	function<void()> upgrade_func;
	
	bool is_menu_enable;
	int use_gold_value;
	
	TouchSuctionLayer* suction;
	
	CCSprite* gray;
	
	CCNode* m_container;
	CCSprite* back_case;
	KSLabelTTF* missile_data_level;
	KSLabelTTF* missile_data_power;
	CCNode* missile_img;
	KSLabelTTF* upgrade_label;
	CCSprite* price_type;
	KSLabelTTF* price_label;
	CCScale9Sprite* price_back;
	CCControlButton* upgrade_button;
	LoadingLayer* loading_layer;
	
	CCNode* upgrade_action_node;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_upgrade_func);

	int before_gold;
	int before_level;
	int before_damage;
	
	void upgradeAction(CCObject* sender, CCControlEvent t_event);
	void resultLevelUp(Json::Value result_data);
	void resultSaveUserData(Json::Value result_data);
	void setAfterUpgrade();
	void attachUpgradePopup(CCPoint keepPosition, StoneType stoneType, int subType, int level);
};

#endif /* defined(__DGproto__MissileUpgradePopup__) */
