//
//  StartSettingPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 16..
//
//

#ifndef __DGproto__StartSettingPopup__
#define __DGproto__StartSettingPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <deque>
#include "EnumDefine.h"
#include "hspConnector.h"
#include "KSProtect.h"
#include "CCMenuLambda.h"
#include "CumberShowWindow.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum StartSettingPopupZorder{
	kStartSettingPopupZorder_back = 1,
	kStartSettingPopupZorder_main,
	kStartSettingPopupZorder_top,
	kStartSettingPopupZorder_popup
};

class LoadingLayer;
class CommonButton;
class KSLabelTTF;
class StartSettingPopup : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
	std::function<void()> goToMainFlow_func;
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StartSettingPopup);
private:
	
	int touch_priority;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	bool is_menu_enable;
	
	CCSprite* gray;
	
	CCNode* tutorial_node;
	
	CommonButton* back_button;
	
	function<void()> tutorial_success_func;
	function<void()> tutorial_fail_func;
	
	CCNode* missile_img;
	
	void setMain();
	CCSprite* main_case;
	
	bool is_before_selected_event_stage;
	
	deque<bool> is_have_item;
	
	int clicked_item_idx;
	vector<ITEM_CODE> item_list;
	deque<bool> is_selected_item;
	deque<int> card_options;
	vector<int> show_item_popup;
	void itemAction(CCObject* sender);
	
	CCSprite* gacha_item;
	CCSprite* gacha_item_cover;
	
	CCLabelTTF* item_title_label;
	CCLabelTTF* option_label;
	CommonButton* buy_button;
	CCNode* cumber_node;
	CumberShowWindow* t_cumber;
	
	int gacha_item_frame;
	ITEM_CODE selected_gacha_item;
	bool is_first_gacha;
	int gacha_item_type;
	void startItemGacha();
	void goItemGacha(Json::Value result_data);
	void itemGachaAction();
	void stopItemGacha();
	void endItemGacha();
	void menuAction(CCObject* sender);
	
	void startButtonAction(CCObject* sender, CCControlEvent t_event);
	
	void closeAction();
	void characterClose();
	
	CCScale9Sprite* left_back;
	
	CCSprite* character_img;
	CCBAnimationManager* character_manager;
	function<void()> repeat_character_action;
	
	CCMenu* upgrade_menu;
	KSLabelTTF* character_level_label;
	KSLabelTTF* missile_level_label;
	KSLabelTTF* character_power_label;
	KSLabelTTF* missile_power_label;
	KSLabelTTF* total_power_label;
	void upgradeAction(CCObject* sender);
	void changeCharacterAction(CCObject* sender);
	
	CCMenuLambda* item_gacha_menu;
	bool is_clicked_gacha_menu;
	void gachaMenuCreate();
	CCSprite* gacha_clicked_img;
	
	bool was_end_startAction;
	bool was_end_removeMessage;
	LoadingLayer* start_loading;
	void callStart();
	void realStartAction(bool is_use_heart = true);
	void acceptStartAction();
	void finalSetting();
	void finalRemoveMessage(Json::Value result_data);
	void goToGame();
	void cancelGame();
	void finalAcceptStartAction(Json::Value result_data);
	void finalStartAction(Json::Value result_data);
	
	int getSelectedItemCount();
	
	void popupClose();
	void popupCloseCardSetting();
	void buySuccessItem(int t_clicked_item_idx, int cnt);
	
	string convertToItemCodeToItemName(ITEM_CODE t_code);
	
	void attachMissilePreview(CCPoint keepPosition, StoneType stoneType, int level);
	bool is_go_to_mainflow;
};

#endif /* defined(__DGproto__StartSettingPopup__) */
