//
//  PuzzleScene.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 17..
//
//

#ifndef __DGproto__PuzzleScene__
#define __DGproto__PuzzleScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "jsoncpp/json.h"
#include "KnownFriend.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CountingBMLabel;
class HeartTime;

enum PuzzleRightCode{
	kPuzzleRightCode_reward = 1,
	kPuzzleRightCode_rank,
	kPuzzleRightCode_monster
};

class PuzzleRankFriendInfo
{
public:
	string nickname;
	string img_url;
	int64 user_id;
	float score;
	int rank;
	bool is_play;
	bool is_message_blocked;
	
	bool operator==(int64 t_id)
	{
		return user_id == t_id;
	};
};

class PuzzleScene : public CCLayer, public CCTableViewDelegate, public CCTableViewDataSource
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(PuzzleScene);
	
	HeartTime* heart_time;
private:
	
	bool is_menu_enable;
	
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
	CountingBMLabel* friend_point_label;
	
	CCSprite* friend_point_popup;
	void closeFriendPointPopup();
	
	int selected_stage_number;
	
	void setPuzzle();
	CCNode* puzzle_node;
	CCSpriteBatchNode* shadow_batchnode;
	void pieceAction(int t_stage_number);
	void buyPieceAction(int t_stage_number);
	void lockPieceAction(int t_stage_number);
	
	void menuAction(CCObject* sender);
	void setTop();
	
	PuzzleRightCode recent_right_code;
	void setRight();
	CCNode* right_case;
	
	void setRightHead();
	CCNode* right_head;
	void setRightContent();
	
	void setReward();
	CCNode* reward_node;
	CCMenu* start_menu;
	
	void setRank();
	CCNode* rank_node;
	CCMenu* challenge_menu;
	vector<PuzzleRankFriendInfo> friend_list;
	void resultGetStageScoreList(Json::Value result_data);
	CCTableView* rank_table;
	int selected_friend_idx;
	void cellAction(CCObject* sender);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
	void setMonster();
	CCNode* monster_node;
	CCMenu* monster_start_menu;
	
	void popupClose();
};

#endif /* defined(__DGproto__PuzzleScene__) */
