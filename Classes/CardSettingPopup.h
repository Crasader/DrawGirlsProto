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
class ScrollBar;
class KSLabelTTF;
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
	
	CCScale9Sprite* main_case;
	CCSprite* gray;
	
	CommonButton* align_default_menu;
	CommonButton* align_take_menu;
	CommonButton* align_rank_menu;
	
	ScrollBar* m_scrollBar;
	
	int server_puzzle_list_count;
	vector<int> server_puzzle_list_no;
	vector<int> server_puzzle_stage_count;
	vector<int> server_puzzle_start_stage;
	vector<DefaultCardCellInfo> default_cell_info;
	vector<CardSortInfo> not_default_card_list;
	
	vector<CardSortInfo> special_card_list;
	
	bool is_special_align_reverse;
	
	int recent_selected_card_number;
	
	int recent_sort_type;
	
	map<int, CCPoint> align_default_position_list;
	
	unsigned int default_align_number_of_cell;
	CCTableView* card_table;
	
	CCScale9Sprite* take_count_back;
	KSLabelTTF* title_content;
	KSLabelTTF* title_label;
	KSLabelTTF* n_special_show_label;
	KSLabelTTF* s_special_show_label;
	
	bool is_normal_table;
	
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
	
	CCPoint getContentPosition(int t_tag);
//	void removeMountingCard();
//	void mountingCard(int card_stage, int card_level);
	
	void alignChange();
//	void addMountedCase();
//	void removeMountedCase();
	
	void specialChangeSortType(bool is_reverse);
	void changeSortType( CardSortType t_type );
	
	bool is_menu_enable;
	
	void addDefaultAlignCard(int t_card_number, CCPoint t_position, CCNode* t_node, string t_ment);
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
};

#endif /* defined(__DGproto__CardSettingPopup__) */
