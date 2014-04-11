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

enum PuzzleZorder{
	kPuzzleZorder_back = 1,
	kPuzzleZorder_puzzle,
	kPuzzleZorder_right,
	kPuzzleZorder_top,
	kPuzzleZorder_popup
};

class RankFriendInfo;
class PuzzleScene : public CCLayer
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
	
	void heartRefresh();
	
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
	
	CCSprite* postbox_count_case;
	CCLabelTTF* postbox_count_label;
	
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
	
	void setRight();
	CCNode* right_case;
	
	void countingMessage();
	
	void popupClose();
	void mailPopupClose();
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__PuzzleScene__) */
