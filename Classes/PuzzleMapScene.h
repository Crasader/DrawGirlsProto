//
//  PuzzleMapScene.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 19..
//
//

#ifndef __DGproto__PuzzleMapScene__
#define __DGproto__PuzzleMapScene__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include "StagePiece.h"
#include "hspConnector.h"
#include "ServerDataSave.h"
#include "StageListDown.h"
#include "PuzzleListView.h"
#include "cocos-ext.h"
#include "PuzzleCache.h"
#include "CountingBMLabel.h"

USING_NS_CC;
using namespace std;
using namespace extension;

enum MapModeState
{
	kMMS_default = 0,
	kMMS_firstTouchDefault,
	kMMS_firstTouchStage,
	kMMS_multiTouchZoom,
	kMMS_changeMode,
	kMMS_uiMode,
	kMMS_frameMode,
	kMMS_notLoadMode,
	kMMS_loadChangingMode
};

enum PMS_Zorder{
	kPMS_Z_wall = 1,
	kPMS_Z_main,
	kPMS_Z_arrows,
	kPMS_Z_puzzle_back_side,
	kPMS_Z_puzzle_shadow,
	kPMS_Z_puzzle_back,
	kPMS_Z_stage = 1000,
	kPMS_Z_boarderStage = 2000,
	kPMS_Z_ui_button = 3000,
	kPMS_Z_popup,
	kPMS_Z_helpRequest
};

enum PMS_MenuTag{
	kPMS_MT_stageBase = 0,
	kPMS_MT_option = 10001,
	kPMS_MT_gacha,
	kPMS_MT_rank,
	kPMS_MT_postbox,
	kPMS_MT_diary,
	kPMS_MT_event,
	kPMS_MT_eventClose,
	kPMS_MT_screen,
	kPMS_MT_left,
	kPMS_MT_right,
	kPMS_MT_up,
	kPMS_MT_tutorial,
	kPMS_MT_showui,
	kPMS_MT_top,
	kPMS_MT_heartTime,
	kPMS_MT_bottom,
	kPMS_MT_rubyShop,
	kPMS_MT_goldShop,
	kPMS_MT_lifeShop,
	kPMS_MT_loadingBack,
	kPMS_MT_loadPuzzleInfo,
	kPMS_MT_notLoaded,
	kPMS_MT_loaded,
	kPMS_MT_notOpenedPuzzle,
	kPMS_MT_notClearBeforePuzzle,
	kPMS_MT_buyPuzzle,
	kPMS_MT_callTicket,
	kPMS_MT_puzzleOpenTitle,
	kPMS_MT_ticketCnt
};

class CreatingPuzzleInfo
{
public:
	int puzzle_number;
	CCPoint img_cut_point;
	string piece_key;
	int piece_number;
};

class PuzzleMapScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(PuzzleMapScene);
	
	virtual void visit();
	void showAcceptStageSettingPopup();
private:
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
	
	MapModeState map_mode_state;
	
	bool is_puzzle_clear;
	
	CCNode* main_node;
	
	CCNode* map_node;
	CCNode* after_map_node;
	CCSpriteBatchNode* shadow_batchnode;
	int touched_stage_number;
	
	CCPoint getStagePosition(int stage);
	CCPoint getUiButtonPosition(int t_tag);
	
	PuzzleMode my_puzzle_mode;
	int recent_puzzle_number;
	
	int start_stage_number;
	int stage_count;
	void setMapNode();
	void setUIs();
	
	CCSprite* addShadow(int i, string t_type, CCPoint sp_position);
	
	int change_frame;
	float change_dy;
	float change_dscale;
	CCPoint change_dpoint;
	float change_top_dy;
	float change_bottom_dy;
	void startChangeUiMode();
	void changeUiMode();
	void stopChangeUiMode();
	
	void startChangeMapMode();
	void changeMapMode();
	void stopChangeMapMode();
	
	void startChangeFrameMode();
	void stopChangeFrameMode();
	
	void startReturnUiMode();
	void stopReturnUiMode();
	
	void resultLogin(Json::Value result_data);
	void startSceneSetting();
	
	void showEventButton();
	void onEventButton();
	void hideEventButton();
	
	//	void showPopup(int stage);
	void stageCancel();
	void popupClose();
	
	float minimum_scale;
	float maximum_scale;
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	
	map<int, CCPoint> multiTouchData;
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	
	bool is_gesturable_map_mode;
	
	CCTableView* frame_table_view;
	
	void loadSuccess();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	void endMovingMapNode();
	void loadPuzzleInfo(CCObject* sender);
	
	void endMovingMapNodeNotOpenPuzzle();
	void notOpenPuzzleAction(CCObject* sender);
	
	void endMovingMapNodeNotClearPuzzle();
	void notClearPuzzleAction(CCObject* sender);
	
	void endLoadPuzzleInfo();
	
	int ing_caching_cnt;
	int total_caching_cnt;
	
	deque<CreatingPuzzleInfo> loaded_imgs;
	bool is_creating_puzzle;
	void creatingPuzzle();
	
	void cachingPuzzleImg2();
	
	void endLoadedMovingMapNode();
	
	bool is_from_call_map_mode;
	
	MapModeState original_mms;
	void endTingMapNode();
	
	void endSwitchMapNode();
	
	void endDirectChangePuzzle();
	
	CCNode* createMapNode();
	
	void stageAction(CCObject* sender);
	
	MapModeState before_map_mode_state;
	int ing_check_puzzle;
	void startPuzzleModeChange(PuzzleMode t_mode);
	void ingPuzzleModeChange();
	void stopPuzzleModeChange();
	
	void changePiece(CCObject* sender);
	
	void endChangePiece(CCObject* sender);
	
	void resetStagePiece();
	CountingBMLabel* ruby_label;
	CountingBMLabel* gold_label;
	void showStageSettingPopup();
	void hideStageSettingPopup();
	
	void showClearPopup();
	void hideClearPopup();
	void showGetPuzzle();
	void createGetPuzzleParticle(CCPoint t_point);
	void endGetPuzzle();
	void showFailPopup();
	void hideFailPopup();
	
	
	
	void showCardSettingPopup();
	void hideCardSettingPopup();
	
	void showOptionPopup();
	void hideOptionPopup();
	
	void showEventPopup();
	void hideEventPopup();
	
	void puzzleUiDisable();
	void puzzleUiEnable();
	
	virtual void registerWithTouchDispatcher(void);
	
	PuzzleListView* puzzle_list_view;
	void puzzleAction(CCObject* sender);
};


#endif /* defined(__DGproto__PuzzleMapScene__) */
