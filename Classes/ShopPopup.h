//
//  ShopPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 22..
//
//

#ifndef __DGproto__ShopPopup__
#define __DGproto__ShopPopup__

#include "cocos2d.h"
#include "hspConnector.h"
#include "cocos-ext.h"
#include <map>
#include "KSProtect.h"
#include "DownloadFile.h"
#include "StageImgLoader.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum ShopCode{
	kSC_empty = 0,
	kSC_ruby,
	kSC_gold,
	kSC_heart,
	kSC_character,
	kSC_card,
	kSC_p1,
	kSC_eventPack
};

enum ShopBeforeCode{
	kShopBeforeCode_stagesetting = 0,
	kShopBeforeCode_mainflow,
	kShopBeforeCode_puzzle,
	kShopBeforeCode_startsetting
};

class HeartTime;
class LoadingLayer;
class TouchSuctionLayer;
class CommonButton;
class KSLabelTTF;
class ConvexGraph;
class ShopPopup : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(ShopPopup);
	
	void setCloseFunc(function<void(void)> t_close_func);
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	void targetHeartTime(HeartTime* t_heartTime);
	void setShopBeforeCode(ShopBeforeCode t_code);
	void addGachaButton();
	
	void addGray();
	
	void setShopCode(ShopCode t_code);
	
	bool is_continue;
	function<void()> continue_end;
	
private:
	bool is_add_gray;
	bool is_set_close_func;
	function<void(void)> close_func;
	
	CCSprite* gray;
	
	CCMenu* tab_menu;
	
	ShopBeforeCode before_code;
	ShopCode recent_shop_code;
	HeartTime* target_heartTime;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	LoadingLayer* loading_layer;
	TouchSuctionLayer* result_stone_layer;
	
	int touch_priority;
	
//	vector<KSProtectVar<int>> index_to_ruby;
//	vector<KSProtectVar<int>> index_to_gold;
//	vector<KSProtectVar<int>> index_to_heart;
//	map<int, KSProtectVar<int>> cash_to_ruby;
//	map<int, KSProtectVar<int>> ruby_to_gold;
//	map<int, KSProtectVar<int>> ruby_to_heart;
	
	KSProtectVar<int> card_price_high; // ruby
	KSProtectVar<int> card_price_mid; // gold
	KSProtectVar<int> card_price_low; // friend point
	
	function<void()> fail_func;
	function<void()> success_func;
	
	CCSprite* main_case;
	int server_character_count;
	CCTableView* character_table;
	
	void setCardBuyMenu(CCPoint t_point, int t_tag, string inner_filename, string type_filename, int price_value);
	
	CommonButton* character_menu;
	CommonButton* card_menu;
	
	CCMenuItem* ruby_menu;
	CCMenuItem* gold_menu;
	CCMenuItem* heart_menu;
	CCMenuItem* p1_menu;
	CCMenuItem* eventPack_menu;
	CCNode* eventPack_node;
	KSLabelTTF* time_label;
	
//	CCMenu* tab_menu;
	
	TouchSuctionLayer* suction;
	
	int last_select_idx;
	
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	bool is_menu_enable;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
    bool is_use_goods_type_gold;
    
	void buyStartPack(CCObject* sender);
	void buyEventPack(CCObject* sender);
	
	void resultSetUserData(Json::Value result_data);
	
	CCPoint getContentPosition(int t_tag);
	
//	string getPriceData(string t_code);
	void addPriceReward(CCNode* t_target, int t_number);
	
	int gacha_card_number;
	
	vector<DownloadFile> df_list;
//	vector<CopyFile> cf_list;
	
	string card_gacha_type;
	void startCardGacha();
	void resultCardGacha(Json::Value result_data);
	CommonButton* replay_menu;
	void replayAction(CCObject* sender);
	
	vector<CommandParam> command_list;
	vector<DownloadFile> card_download_list;
	LoadingLayer* card_loading;
	ConvexGraph* card_progress_timer;
	int success_download_cnt;
	vector<DownloadImgInfo> download_set;
	vector<int> is_enable_index;
	void startFileDownloadSet();
	void checkDownloading();
	
	bool is_downloading;
	int ing_download_cnt;
	
	void startDownload();
	void successAction();
	void failAction();
	
	void createCheckBuyPopup(function<void()> buy_action);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
	
	void requestItemDelivery();
	void requestItemDeliveryStartPack();
	void requestItemDeliveryEventPack();
};

#endif /* defined(__DGproto__ShopPopup__) */
