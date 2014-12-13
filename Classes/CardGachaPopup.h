//
//  CardGachaPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 12. 9..
//
//

#ifndef __DGproto__CardGachaPopup__
#define __DGproto__CardGachaPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <deque>
#include "jsoncpp/json.h"
#include "KSProtect.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class KSLabelTTF;
class LoadingLayer;
class CardGachaPopup : public CCLayer
{
public:
	static CardGachaPopup* create(int t_touch_priority);
	void setHideFinalAction(CCObject* t_target, SEL_CallFunc t_delegate);
	
private:
	int touch_priority;
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	bool is_menu_enable;
	CCSprite* gray;
	CCNode* main_case;
	float moving_speed;
	
	deque<CCSprite*> card_que;
	int card_type_size;
	CCSprite* mask_rect;
	CCRect mask_bounding_rect;
	
	CCClippingNode* clipping_node;
	CCControlButton* stop_button;
	CCControlButton* one_button;
	CCScale9Sprite* price_back;
	CCControlButton* five_button;
	CCControlButton* ok_button;
	CCSprite* cover_img;
	CCClippingNode* cover_clipping;
	CCSprite* big_card_img;
	CCSprite* t_gray;
	KSLabelTTF* sub_ment1;
	float positioning_value;
	KSLabelTTF* grade_card_take;
	
	LoadingLayer* loading_layer;
	CCSprite* left_character;
	CCSprite* right_character;
	
	int step_cnt;
	
	deque<KSProtectVar<int>> take_card_number_list;
	void oneAction(CCObject* t_sender, CCControlEvent t_event);
	void resultOne(Json::Value result_data);
	void fiveAction(CCObject* t_sender, CCControlEvent t_event);
	void resultFive(Json::Value result_data);
	void myStopAction(CCObject* t_sender, CCControlEvent t_event);
	void okAction(CCObject* t_sender, CCControlEvent t_event);
	void cardMoving();
	void cardPositioning();
	void myInit(int t_touch_priority);
};

#endif /* defined(__DGproto__CardGachaPopup__) */
