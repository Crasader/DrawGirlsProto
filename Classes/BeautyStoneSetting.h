//
//  BeautyStoneSetting.h
//  DGproto
//
//  Created by 사원3 on 2014. 3. 4..
//
//

#ifndef __DGproto__BeautyStoneSetting__
#define __DGproto__BeautyStoneSetting__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum BeautyStoneAlign{
	kBeautyStoneAlign_rank = 0,
	kBeautyStoneAlign_rankReverse,
	kBeautyStoneAlign_take,
	kBeautyStoneAlign_takeReverse,
	kBeautyStoneAlign_level,
	kBeautyStoneAlign_levelReverse
};

class BeautyStoneInfo
{
public:
	int m_type;
	int m_rank;
	int m_level;
	int m_id;
};

class TouchSuctionLayer;
class CommonButton;
class BeautyStoneSettingPopup : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(BeautyStoneSettingPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	static string convertToTypeName(int type_code);
	
private:
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	vector<CCNode*> slot_vector;
	
	vector<BeautyStoneInfo> have_stone_list;
	BeautyStoneAlign recent_align;
	void alignStoneList(BeautyStoneAlign t_align);
	CCTableView* have_stone_table;
	
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

#endif /* defined(__DGproto__BeautyStoneSetting__) */
