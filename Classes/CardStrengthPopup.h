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
	
	CCSprite* target_card;
	CCMenu* offering_menu;
	
	int recent_offering_number;
	
	CCLabelTTF* pow_label;
	CCLabelTTF* spd_label;
	CCLabelTTF* dex_label;
	CCLabelTTF* dur_label;
	
	CCLabelTTF* up_pow_label;
	CCLabelTTF* up_spd_label;
	CCLabelTTF* up_dex_label;
	CCLabelTTF* up_dur_label;
	
	CCTableView* offering_table;
	vector<CardSortInfo> offering_list;
	
	CardStrengthAlign recent_align;
	
	CCSprite* loading_img;
	CCLabelTTF* loading_label;
	CCMenu* replay_menu;
	
	void replayAction(CCObject* sender);
	
	CCNode* strength_card_node;
	void setStrengthCardNode(CCNode* t_node);
	
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
};

#endif /* defined(__DGproto__CardStrengthPopup__) */
