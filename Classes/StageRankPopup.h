//
//  StageRankPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 21..
//
//

#ifndef __DGproto__StageRankPopup__
#define __DGproto__StageRankPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class StageRankFriendInfo
{
public:
	string nickname;
	string img_url;
	string user_id;
	float score;
	bool is_play;
	bool is_message_blocked;
	
	bool operator==(string t_id)
	{
		return user_id == t_id;
	};
};

enum StageRankFriendCell_Tag
{
	kSRFC_T_score = 1,
	kSRFC_T_rank,
	kSRFC_T_nickname,
	kSRFC_T_img,
	kSRFC_T_menuBase = 100000
};

enum StageRankFriendCell_Zorder
{
	kSRFC_Z_case = 1,
	kSRFC_Z_img
};

enum StageRankPopupZorder{
	kSRP_Z_gray = 1,
	kSRP_Z_back,
	kSRP_Z_content
};

enum StageRankPopupMenuTag{
	kSRP_MT_close = 1
};

class StageRankPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static StageRankPopup* create(CCObject* t_close, SEL_CallFunc d_close, int t_stage_number);
	
private:
	bool is_menu_enable;
	
	int stage_number;
	
	int touched_number;
	CCMenu* close_menu;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	vector<StageRankFriendInfo> friend_list;
	CCTableView* rankTableView;
	
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
    
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
	
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void resultLoadFriends(Json::Value result_data);
	void resultGetStageScoreList(Json::Value result_data);
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	CCSprite* back_img;
	CCSprite* gray;
	
	CCLabelTTF* loading_label;
	
	virtual void onEnter();
	
	void menuAction(CCObject* sender);
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close, int t_stage_number);
	
	void setBack();
	
	CCPoint getContentPosition(int t_tag);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__StageRankPopup__) */
