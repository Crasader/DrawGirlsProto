//
//  HellModeResult.h
//  DGproto
//
//  Created by 사원3 on 2014. 9. 23..
//
//

#ifndef __DGproto__HellModeResult__
#define __DGproto__HellModeResult__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "StarGoldData.h"
#include "hspConnector.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

//enum ClearFriendCell_Tag
//{
//	kCFC_T_score = 1,
//	kCFC_T_rank,
//	kCFC_T_nickname,
//	kCFC_T_img
//};
//
//enum ClearFriendCell_Zorder
//{
//	kCFC_Z_case = 1,
//	kCFC_Z_img
//};

class KSLabelTTF;
class HellModeResult : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HellModeResult);
	
	virtual void onEnter();
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	virtual void onEnterTransitionDidFinish();
	
	function<void()> replay_func;
	
	virtual ~HellModeResult();
	
private:
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCSprite* main_case;
	
	vector<function<void()>> cell_action_list;
	int delay_index;
	
	KSLabelTTF* title;
	
	void controlButtonAction(CCObject* sender, CCControlEvent t_event);
	
	CCScale9Sprite* inner_left;
	
	Json::Value rank_data;
	
	bool is_menu_enable;
	
	bool is_saved_user_data;
	bool is_end_popup_animation;
	bool is_end_take_card;
	
	vector<CommandParam> send_command_list;
	
	CCControlButton* ok_menu;
	CCControlButton* replay_menu;
	
	bool is_rank_changed;
	int before_my_rank;
	int recent_my_rank;
	
	CCTableView* rankTableView;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	void endTakeCard();
	
	void closePopup();
	void endLoad();
	
	void tryTransaction(CCNode* t_loading);
	void resultSavedUserData(Json::Value result_data);
	
	CCSprite* loading_img;
	
	void resultGetRank(Json::Value result_data);
	void resultGetTime(Json::Value result_data);
	
	std::function<void(void)> frontFlip, backFlip;
};

#endif /* defined(__DGproto__HellModeResult__) */
