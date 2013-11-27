//
//  StageSettingPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 19..
//
//

#ifndef __DGproto__StageSettingPopup__
#define __DGproto__StageSettingPopup__

#include "cocos2d.h"

#include <deque>

#include "EnumDefine.h"
#include "jsoncpp/json.h"
#include "GDLib.h"

USING_NS_CC;
using namespace std;

enum SSP_MenuTag{
	kSSP_MT_start = 1,
	kSSP_MT_back = 2,
	kSSP_MT_changeCard = 3,
	kSSP_MT_challenge = 4,
	kSSP_MT_gacha = 5,
	kSSP_MT_info = 6,
	kSSP_MT_rubyShop = 7,
	kSSP_MT_goldShop = 8,
	kSSP_MT_heartShop = 9,
	kSSP_MT_heartTime = 10,
	kSSP_MT_itemBase = 100,
	kSSP_MT_itemBuy = 200,
	kSSP_MT_selectedBase = 300,
	kSSP_MT_itemCntBase = 400,
	kSSP_MT_noti = 9999,
	kSSP_MT_noti_ok,
	kSSP_MT_noti_cancel
};

class HeartTime;
class ItemListViewer;
class ListViewerScroll;
class LoadingLayer;
class StageSettingPopup : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StageSettingPopup);
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	HeartTime* heart_time;
	
private:
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	CCSprite* main_case;
	CCSprite* top_case;
	
	int selected_stage;
	vector<ITEM_CODE> item_list;
	deque<bool> is_selected_item;
	deque<int> card_options;
	bool is_menu_enable;
	
	ItemListViewer* my_ilv;
	ListViewerScroll* t_lvs;
	
	LoadingLayer* start_loading;
	
	void callStart();
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	CCPoint getContentPosition(int t_tag);
	void popupClose();
	
	void itemSetting();
	
	void buySuccessItem(int t_clicked_item_number, int cnt);
	
	string convertToItemCodeToItemName(ITEM_CODE t_code);
	
	void cheatAction(CCObject* sender);
	void realStartAction();
	void acceptStartAction();
	void finalSetting();
	void finalStartAction(Json::Value result_data);
	bool was_end_startAction;
	bool was_end_removeMessage;
	void finalAcceptStartAction(Json::Value result_data);
	void finalRemoveMessage(Json::Value result_data);
	
	void goToGame();
	void cancelGame();
};

#endif /* defined(__DGproto__StageSettingPopup__) */
