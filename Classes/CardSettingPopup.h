//
//  CardSettingPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#ifndef __DGproto__CardSettingPopup__
#define __DGproto__CardSettingPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "StarGoldData.h"
#include "DownloadFile.h"
#include "StageImgLoader.h"

//#include <deque>
#include <map>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum DefaultCardCellType
{
	kTitle = 0,
	kStage
};

enum DefaultCardCellInCellType
{
	kPuzzle = -1,
	kEmpty = 0
};

class DefaultCardCellInfo
{
public:
	DefaultCardCellType m_type; // title or stage
	int first_cell_in_cell_type; // stage or DefaultCardCellInCellType::kPuzzle
	int second_cell_in_cell_type; // stage or DefaultCardCellInCellType::kPuzzle or DefaultCardCellInCellType::kEmpty
	int puzzle_number;
	
	DefaultCardCellInfo() : m_type(DefaultCardCellType::kTitle), first_cell_in_cell_type(0), second_cell_in_cell_type(0), puzzle_number(0){}
};

class CardListViewer;
class ListViewerScroll;
class IntPoint;
class CommonButton;
//class ScrollBar;
class KSLabelTTF;
class LoadingLayer;
class ConvexGraph;
class CardSettingPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(CardSettingPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
private:
	CCNode* tutorial_node;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* main_case;
	CCSprite* gray;
	
	CCMenu* tab_menu;
	
	CCMenuItemSprite* align_default_menu;
	CCMenuItemSprite* align_take_menu;
	CCMenuItemSprite* align_rank_menu;
	CCMenuItemSprite* event_card_menu;
	CCMenuItem* diary_menu;
	
	KSLabelTTF* n_take_label;
	KSLabelTTF* s_take_label;
	KSLabelTTF* n_rank_label;
	KSLabelTTF* s_rank_label;
	
	bool is_take_reverse;
	bool is_rank_reverse;
	
	int table_update_cnt;
	
	void defaultMenuSet();
	void takeMenuSet();
	void rankMenuSet();
	void eventMenuSet();
	
	void beforeMenuReset(int keep_type);
	
	int recent_selected_card_number;
	
	int recent_sort_type;
	
	CCTableView* card_table;
	vector<DownloadFile> card_download_list;
	int ing_download_cnt;
	int success_download_cnt;
	vector<DownloadImgInfo> download_set;
	vector<int> is_enable_index;
	void startFileDownloadSet();
	void checkDownloading();
	ConvexGraph* card_progress_timer;
	LoadingLayer* card_loading;
	bool is_downloading;
	int keep_version;
	
	CCScale9Sprite* take_count_back;
	KSLabelTTF* title_label;
	
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx);
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	int download_type;
	
	CCPoint getContentPosition(int t_tag);
	
	vector<CardSortInfo> card_list;
	vector<CardSortInfo> event_card_list;
	
	void characterClose();
	
	void alignChange();
	
	void changeSortType( CardSortType t_type, bool is_init = false );
	
	bool is_menu_enable;
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__CardSettingPopup__) */
