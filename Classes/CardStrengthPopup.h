//
//  CardStrengthPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 3..
//
//

#ifndef __DGproto__CardStrengthPopup__
#define __DGproto__CardStrengthPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum CardStrengthAlign{
	kCardStrengthAlign_rank = 0,
	kCardStrengthAlign_rankReverse,
	kCardStrengthAlign_take,
	kCardStrengthAlign_takeReverse
};

class CardSortInfo;
class TouchSuctionLayer;
class StrengthCardAnimation;
class CommonButton;
class CardStrengthPopup : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CardStrengthPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	CCSprite* table_tab;
	
	int card_strength_before;
	
	int strength_card_number;
	CCNode* strength_node;
	int offering_card_number;
	CCNode* offering_node;
	
	int strength_idx;
	int offering_idx;
	
	void setStrengthNode(int t_card_number);
	void setOfferingNode(int t_card_number);
	void setTableTab();
	
	CCSprite* strength_probability;
	
//	TouchSuctionLayer* result_popup;
	
	CCTableView* offering_table;
	vector<CardSortInfo> offering_list;
	
	CardStrengthAlign recent_align;
	
	StrengthCardAnimation* result_popup;
	string result_string;
	int save_offering_number;
	bool is_success;
	CCSprite* loading_img;
	CCLabelTTF* loading_label;
	CommonButton* replay_menu;
	
	void replayAction(CCObject* sender);
	
	void setOfferingList();
	void alignOfferingList(CardStrengthAlign t_align);
	
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void resultStrength(Json::Value result_data);
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	CCPoint getContentPosition(int t_tag);
	
	bool is_menu_enable;
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__CardStrengthPopup__) */
