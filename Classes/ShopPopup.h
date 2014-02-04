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

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum ShopCode{
	kSC_empty = 0,
	kSC_ruby,
	kSC_gold,
	kSC_heart,
	kSC_character,
	kSC_card
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
class ShopPopup : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(ShopPopup);
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	void targetHeartTime(HeartTime* t_heartTime);
	void setShopBeforeCode(ShopBeforeCode t_code);
	
	void setShopCode(ShopCode t_code);
	
private:
	
	ShopBeforeCode before_code;
	ShopCode recent_shop_code;
	HeartTime* target_heartTime;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	LoadingLayer* loading_layer;
	
	vector<KSProtectVar<int>> index_to_ruby;
	vector<KSProtectVar<int>> index_to_gold;
	vector<KSProtectVar<int>> index_to_heart;
	map<int, KSProtectVar<int>> cash_to_ruby;
	map<int, KSProtectVar<int>> ruby_to_gold;
	map<int, KSProtectVar<int>> ruby_to_heart;
	
	KSProtectVar<int> card_price_high; // ruby
	KSProtectVar<int> card_price_mid; // gold
	KSProtectVar<int> card_price_low; // friend point
	
	CCSprite* main_case;
	int server_character_count;
	CCTableView* character_table;
	
	void setCardBuyMenu(CCPoint t_point, int t_tag, string inner_filename, string type_filename, int price_value);
	
	CCMenu* tab_menu;
	
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
	
	void resultSetUserData(Json::Value result_data);
	
	CCPoint getContentPosition(int t_tag);
	
	string getPriceData(string t_code);
	void addPriceReward(CCNode* t_target, int t_number);
	
	int gacha_card_number;
	
	vector<DownloadFile> df_list;
	vector<CopyFile> cf_list;
	
	string card_gacha_type;
	void startCardGacha();
	void resultCardGacha(Json::Value result_data);
	CCMenu* replay_menu;
	void replayAction(CCObject* sender);
	
	vector<CommandParam> command_list;
	
	bool is_downloading;
	int ing_download_cnt;
	
	void startDownload();
	void successAction();
	void failAction();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__ShopPopup__) */
