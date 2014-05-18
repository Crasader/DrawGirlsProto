#ifndef ENDLESSSETTINGPOPUP_H
#define ENDLESSSETTINGPOPUP_H

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;

#include "ScrollBar.h"


enum EndlessSettingPopupZorder{
	kEndlessSettingPopupZorder_back = 1,
	kEndlessStartSettingPopupZorder_main,
	kEndlessStartSettingPopupZorder_top,
	kEndlessStartSettingPopupZorder_popup
};
class EndlessSettingPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	EndlessSettingPopup();
	virtual ~EndlessSettingPopup();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static EndlessSettingPopup* create()
	{
		EndlessSettingPopup* t = new EndlessSettingPopup();
		t->init();
		t->autorelease();
		return t;
	}
	void addMyPosition(CCNode* main_case);
	void addRankingTable(CCNode* node);
	//virtual void registerWithTouchDispatcher();
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx) {
		return cellSizeForTable(table);
	};
	/**
	 * cell height for a given table.
	 *
	 * @param table table to hold the instances of Class
	 * @return cell size
	 */
	virtual CCSize cellSizeForTable(CCTableView *table) {
		return CCSizeMake(230, 33);
//		return CCSizeZero;
	};
	/**
	 * a cell instance at a given index
	 *
	 * @param idx index to search for a cell
	 * @return cell found at idx
	 */
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	/**
	 * Returns number of cells in a given table view.
	 *
	 * @return number of cells
	 */
	virtual unsigned int numberOfCellsInTableView(CCTableView *table)
	{
		return 3;
	}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell)
	{
		
	}
	virtual void scrollViewDidScroll(CCScrollView* view)
	{
		if(m_scrollBar)
		{
			m_scrollBar->setBarRefresh();
		}
	}
	virtual void scrollViewDidZoom(CCScrollView* view)
	{
		
	}
protected:
	CCSprite* gray;
	CCScale9Sprite* main_case;
	
	
	CCTableView* m_rankingTable;
	ScrollBar * m_scrollBar;
};

#endif
