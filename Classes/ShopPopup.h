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

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum ShopCode{
	kSC_empty = 0,
	kSC_ruby,
	kSC_gold,
	kSC_heart,
	kSC_character
};

enum ShopBeforeCode{
	kShopBeforeCode_stagesetting = 0,
	kShopBeforeCode_mainflow
};

class HeartTime;
class LoadingLayer;
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
	
	CCSprite* main_case;
	CCTableView* character_table;
	
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
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__ShopPopup__) */
