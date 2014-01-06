//
//  CardChangePopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 19..
//
//

#ifndef __DGproto__CardChangePopup__
#define __DGproto__CardChangePopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum CardChangeAlign{
	kCardChangeAlign_rank = 0,
	kCardChangeAlign_rankReverse,
	kCardChangeAlign_take,
	kCardChangeAlign_takeReverse
};
class CardSortInfo;
class CardChangePopup : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CardChangePopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	CCNode* tutorial_node;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	CCSprite* selected_card;
	
	int base_touch_priority;
	int clicked_card_number;
	
	CCTableView* have_card_table;
	vector<CardSortInfo> have_card_list;
	
	CardChangeAlign recent_align;
	
	void setSelectedCard(int t_card_number);
	void addMountedCase();
	CCMenu* mount_menu;
	CCMenu* release_menu;
	void setHaveCardList();
	void alignHaveCardList(CardChangeAlign t_align);
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void setMonster();
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	bool is_menu_enable;
};

#endif /* defined(__DGproto__CardChangePopup__) */
