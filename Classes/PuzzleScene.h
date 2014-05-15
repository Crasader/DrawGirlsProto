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
#include "ShopPopup.h"

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

enum PuzzleRightMode{
	kPuzzleRightMode_stage = 0,
	kPuzzleRightMode_ranking
};

class RankFriendInfo;
class CommonButton;
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
	
	void showShopPopup(ShopCode t_code);
	
	virtual ~PuzzleScene()
	{
		hspConnector::get()->removeTarget(this);
	}
private:
	
	int keep_card_number;
	void updateCardHistory(CCNode* t_loading);
	
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
//	CCLabelTTF* postbox_count_label;
	CCSprite* achievement_count_case;
	
	function<void(void)> close_friend_point_action;
	void closeFriendPoint();
	
//	int selected_stage_number;
	
	int next_stage_number;
	
	void startBacking();
	
	void openBeforeSettingPopup();
	void openSettingPopup();
	
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
	
	bool is_auto_turn;
	int auto_turn_piece_frame;
	void startAutoTurnPiece();
	void autoTurnPiece();
	void stopAutoTurnPiece();
	
	string before_scene_name;
	
	PieceMode piece_mode;
	void puzzleOpenning();
	void puzzleBacking();
	void setPuzzle();
	CCNode* puzzle_node;
//	CCSpriteBatchNode* shadow_batchnode;
//	void addShadow(string piece_type, CCPoint piece_position, int t_stage_number);
	CCSprite* selected_piece_img;
	void setPieceClick(int t_stage_number);
	void pieceAction(int t_stage_number);
	void buyPieceAction(int t_stage_number);
	void lockPieceAction(int t_stage_number);
//	CCSprite* have_card_cnt_case;
	
	void pumpPuzzle();
	
	void menuAction(CCObject* sender);
	
	vector<CCNode*> top_list;
	void topOpenning();
	void topReopenning();
	void topBacking();
	void setTop();
	
	CommonButton* stage_button;
	CommonButton* ranking_button;
	void setRightTopButton();
	
	CCMenu* ready_menu;
	
	void rightOpenning();
	void rightBacking();
	void setRight();
	CCNode* right_case;
	
	CCSprite* loading_progress_img;
	void resultGetRank(Json::Value result_data);
	
	CCScale9Sprite* right_body;
	
	PuzzleRightMode right_mode;
	int saved_ranking_stage_number;
	Json::Value saved_ranking_data;
	
	void countingMessage();
	void countingAchievement();
	
	void popupClose();
	void mailPopupClose();
	void achievePopupClose();
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked();
};

#endif /* defined(__DGproto__PuzzleScene__) */
