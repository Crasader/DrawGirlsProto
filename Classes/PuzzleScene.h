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
#include "PuzzlePiece.h"

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
	string user_id;
	float score;
	int rank;
	bool is_play;
	bool is_message_blocked;
	
	bool operator==(string t_id)
	{
		return user_id == t_id;
	};
};

enum PuzzleZorder{
	kPuzzleZorder_back = 1,
	kPuzzleZorder_puzzle,
	kPuzzleZorder_right,
	kPuzzleZorder_top,
	kPuzzleZorder_popup
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
	CCNode* tutorial_node;
private:
	
	bool clear_is_empty_piece;
	bool clear_is_empty_star;
	int clear_star_take_level;
	bool clear_is_first_puzzle_success;
	bool clear_is_first_perfect;
	bool clear_is_stage_unlock;
	
	bool is_menu_enable;
	
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
	CountingBMLabel* friend_point_label;
	
	function<void(void)> close_friend_point_action;
	void closeFriendPoint();
	
	int selected_stage_number;
	
	int next_stage_number;
	
	void showClearPopup();
	void hideClearPopup();
	void showGetPuzzle();
	void createGetPuzzleParticle(CCPoint t_point);
	void endGetPuzzle();
	void showGetStar();
	void endGetStar();
	void showSuccessPuzzleEffect();
	void endSuccessPuzzleEffect();
	void showPerfectPuzzleEffect();
	void endPerfectPuzzleEffect();
	CCSprite* unlock_cover;
	void showUnlockEffect();
	void endUnlockEffect();
	void showFailPopup();
	void hideFailPopup();
	
	string before_scene_name;
	
	PieceMode piece_mode;
	void setPuzzle();
	CCNode* puzzle_node;
//	CCSpriteBatchNode* shadow_batchnode;
//	void addShadow(string piece_type, CCPoint piece_position, int t_stage_number);
	CCSprite* selected_piece_img;
	void setPieceClick(int t_stage_number);
	void pieceAction(int t_stage_number);
	void buyPieceAction(int t_stage_number);
	void lockPieceAction(int t_stage_number);
	CCSprite* have_card_cnt_case;
	
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
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
    virtual CCSize cellSizeForTable(CCTableView *table);
    virtual unsigned int numberOfCellsInTableView(CCTableView *table);
	
//	void setMonster();
//	CCNode* monster_node;
//	CCMenu* monster_start_menu;
	
	void popupClose();
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__PuzzleScene__) */
