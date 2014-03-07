//
//  BeautyStoneStrength.h
//  DGproto
//
//  Created by 사원3 on 2014. 3. 5..
//
//

#ifndef __DGproto__BeautyStoneStrength__
#define __DGproto__BeautyStoneStrength__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum BeautyStoneStrengthAlign{
	kBeautyStoneStrengthAlign_rank = 0,
	kBeautyStoneStrengthAlign_rankReverse,
	kBeautyStoneStrengthAlign_level,
	kBeautyStoneStrengthAlign_levelReverse
};

class BeautyStoneInfo;
class TouchSuctionLayer;
class CommonButton;
class BeautyStoneStrengthPopup : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(BeautyStoneStrengthPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	CCSprite* table_tab;
	
	int strength_stone_idx;
	CCNode* strength_node;
	int offering_stone_idx;
	CCNode* offering_node;
	
	void setStrengthNode(int t_strength_idx);
	void setOfferingNode(int t_offering_idx);
	void setTableTab();
	
	CCSprite* strength_probability;
	
	CCTableView* offering_table;
	vector<BeautyStoneInfo> offering_list;
	
	BeautyStoneStrengthAlign recent_align;
	
	void setOfferingList();
	void alignOfferingList(BeautyStoneStrengthAlign t_align);
	
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	bool is_menu_enable;
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__BeautyStoneStrength__) */
