//
//  CharacterSelectPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 9. 21..
//
//

#ifndef __DGproto__CharacterSelectPopup__
#define __DGproto__CharacterSelectPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "KSProtect.h"
#include "jsoncpp/json.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;


class TouchSuctionLayer;
class CharacterInfo
{
public:
	int m_number;
	int m_index;
	string m_name;
	int m_damage;
	int m_level;
	string m_comment;
	string m_character;
	int m_card;
	KSProtectVar<bool> is_have;
};
class LoadingLayer;
class CharacterSelectPopup : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CharacterSelectPopup);
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	TouchSuctionLayer* suction;
	int touch_priority;
	CCSprite* main_case;
	CCTableView* character_table;
	bool is_menu_enable;
	
	LoadingLayer* change_loading;
	
	int list_cnt;
	int send_character_number;
	vector<CharacterInfo> history_list;
	int selected_character_number;
	
	CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	void scrollViewDidScroll(CCScrollView* view);
	void scrollViewDidZoom(CCScrollView* view);
	void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	CCSize cellSizeForTable(CCTableView *table);
	unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void detailAction(CCObject* sender);
	void notHaveAction(CCObject* sender);
	void characterChangeAction(CCObject* sender);
	void resultUpdateCharacterHistory(Json::Value result_data);
	void gachaClose();
	void resetInfo();
};

#endif /* defined(__DGproto__CharacterSelectPopup__) */
