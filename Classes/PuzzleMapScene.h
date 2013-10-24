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

USING_NS_CC;
using namespace std;

enum MapModeState
{
	kMMS_default = 0,
	kMMS_firstTouchDefault,
	kMMS_firstTouchStage,
	kMMS_multiTouchZoom,
	kMMS_changeMode,
	kMMS_uiMode,
	kMMS_frameMode,
	kMMS_notLoadMode
};

enum PMS_Zorder{
	kPMS_Z_wall = 1,
	kPMS_Z_main,
	kPMS_Z_puzzle_back_side,
	kPMS_Z_puzzle_shadow,
	kPMS_Z_puzzle_back,
	kPMS_Z_stage = 1000,
	kPMS_Z_boarderStage = 2000,
	kPMS_Z_ui_button = 3000,
	kPMS_Z_popup
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
	kPMS_MT_showui,
	kPMS_MT_top,
	kPMS_MT_bottom,
	kPMS_MT_rubyShop,
	kPMS_MT_goldShop,
	kPMS_MT_lifeShop,
	kPMS_MT_loadingBack,
	kPMS_MT_loadPuzzleInfo,
	kPMS_MT_notLoaded,
	kPMS_MT_loaded
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
	
	virtual void visit()
	{
		glEnable(GL_SCISSOR_TEST);
		
		CCSize frame_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		
		float x = 0;
		float y = 0;
		float w = frame_size.width;
		float h = frame_size.height;
		
		glScissor(x,y,w,h);
		
		CCLayer::visit();
		
		glDisable(GL_SCISSOR_TEST);
	}
	
private:
	bool is_menu_enable;
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
	
	MapModeState map_mode_state;
	
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
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	void endMovingMapNode()
	{
		map_mode_state = kMMS_notLoadMode;
		map_node->removeFromParent();
		map_node = after_map_node;
		after_map_node = NULL;
		
		CCSprite* n_load = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 70, 50));
		CCLabelTTF* n_label = CCLabelTTF::create("퍼즐 정보 로드 하기", mySGD->getFont().c_str(), 18);
		n_label->setColor(ccBLACK);
		n_label->setPosition(ccp(35,25));
		n_load->addChild(n_label);
		
		CCSprite* s_load = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 70, 50));
		s_load->setColor(ccGRAY);
		CCLabelTTF* s_label = CCLabelTTF::create("퍼즐 정보 로드 하기", mySGD->getFont().c_str(), 18);
		s_label->setColor(ccBLACK);
		s_label->setPosition(ccp(35,25));
		s_load->addChild(s_label);
		
		CCMenuItem* load_item = CCMenuItemSprite::create(n_load, s_load, this, menu_selector(PuzzleMapScene::loadPuzzleInfo));
		
		CCMenu* load_menu = CCMenu::createWithItem(load_item);
		load_menu->setPosition(ccp(240,160));
		addChild(load_menu, kPMS_Z_popup, kPMS_MT_loadPuzzleInfo);
	}
	
	void loadPuzzleInfo(CCObject* sender)
	{
		map_mode_state = kMMS_changeMode;
		removeChildByTag(kPMS_MT_loadPuzzleInfo);
		
		StageListDown* t_sld = StageListDown::create(this, callfunc_selector(PuzzleMapScene::endLoadPuzzleInfo), recent_puzzle_number);
		addChild(t_sld, kPMS_Z_popup);
	}
	
	void endLoadPuzzleInfo()
	{
		removeChildByTag(kPMS_MT_loadingBack);
		map_node->removeChildByTag(99999);
		map_node->removeChildByTag(99998);
		
		switchMapNode(map_node);
		endSwitchMapNode();
	}
	
	void switchMapNode(CCNode* t_node)
	{
		CCSprite* map_back_center = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_center.png", recent_puzzle_number)->getCString());
		map_back_center->setPosition(CCPointZero);
		t_node->addChild(map_back_center, kPMS_Z_puzzle_back_side);
		
		CCSize center_size = CCSizeMake(520.f, 340.f);
		
		CCSprite* map_back_left = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_left.png", recent_puzzle_number)->getCString());
		map_back_left->setAnchorPoint(ccp(0.f,0.5f));
		map_back_left->setPosition(ccp(-center_size.width/2.f, 0));
		t_node->addChild(map_back_left, kPMS_Z_puzzle_back);
		
		CCSprite* map_back_right = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_right.png", recent_puzzle_number)->getCString());
		map_back_right->setAnchorPoint(ccp(1.f,0.5f));
		map_back_right->setPosition(ccp(center_size.width/2.f, 0));
		t_node->addChild(map_back_right, kPMS_Z_puzzle_back);
		
		CCSprite* map_back_top = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_top.png", recent_puzzle_number)->getCString());
		map_back_top->setAnchorPoint(ccp(0.5f,1.f));
		map_back_top->setPosition(ccp(0, center_size.height/2.f));
		t_node->addChild(map_back_top, kPMS_Z_puzzle_back);
		
		CCSprite* map_back_bottom = mySIL->getLoadedImg(CCSTR_CWF("puzzle%d_bottom.png", recent_puzzle_number)->getCString());
		map_back_bottom->setAnchorPoint(ccp(0.5f,0.f));
		map_back_bottom->setPosition(ccp(0,-center_size.height/2.f));
		t_node->addChild(map_back_bottom, kPMS_Z_puzzle_back);
		
		shadow_batchnode = CCSpriteBatchNode::create("test_map_shadow.png");
		shadow_batchnode->setPosition(CCPointZero);
		t_node->addChild(shadow_batchnode, kPMS_Z_puzzle_shadow);
		
		CCRect stage_rect = CCRectMake(-30.f, -30.f, 60.f, 60.f);
		
		start_stage_number = NSDS_GI(recent_puzzle_number, kSDS_PZ_startStage_i);
		stage_count = NSDS_GI(recent_puzzle_number, kSDS_PZ_stageCount_i);
		
		for(int i = start_stage_number;i<start_stage_number+stage_count;i++)
		{
			int stage_level = NSDS_GI(recent_puzzle_number, kSDS_PZ_stage_int1_level_i, i);
			
			bool is_found = false;
			int found_number = 0;
			bool is_have_card[3] = {0,};
			for(int k=3;k>=1;k--)
			{
				int card_number = NSDS_GI(i, kSDS_SI_level_int1_card_i, k);
				int card_durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number);
				if(card_durability > 0)
				{
					if(!is_found)
					{
						is_found = true;
						found_number = k;
					}
					is_have_card[k-1] = true;
				}
			}
			
			CCPoint sp_position = ccp(NSDS_GD(recent_puzzle_number, kSDS_PZ_stage_int1_x_d, i),
									  NSDS_GD(recent_puzzle_number, kSDS_PZ_stage_int1_y_d, i));
			
			if(found_number == 3)
			{
				StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_stage%d_piece.png", recent_puzzle_number, i)->getCString(),
													  i, stage_level, sp_position, stage_rect, false, false,
													  NSDS_GS(recent_puzzle_number, kSDS_PZ_stage_int1_pieceType_s, i),
													  this, menu_selector(PuzzleMapScene::stageAction));
				t_sp->mySetTouchEnable(false);
				
				if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		t_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
				else														t_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				
				t_sp->setChangable(CCSTR_CWF("puzzle%d_stage%d_thumbnail.png", recent_puzzle_number, i)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
				t_sp->setPuzzleMode(my_puzzle_mode);
				t_sp->shadow_node = addShadow(i, NSDS_GS(recent_puzzle_number, kSDS_PZ_stage_int1_pieceType_s, i).c_str(), sp_position);
			}
			else if(found_number == 2)
			{
				StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_stage%d_piece.png", recent_puzzle_number, i)->getCString(),
													  i, stage_level, sp_position, stage_rect, false, true,
													  NSDS_GS(recent_puzzle_number, kSDS_PZ_stage_int1_pieceType_s, i),
													  this, menu_selector(PuzzleMapScene::stageAction));
				t_sp->mySetTouchEnable(false);
				
				if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		t_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
				else														t_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				
				t_sp->setChangable(CCSTR_CWF("puzzle%d_stage%d_thumbnail.png", recent_puzzle_number, i)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
				t_sp->setPuzzleMode(my_puzzle_mode);
				t_sp->shadow_node = addShadow(i, NSDS_GS(recent_puzzle_number, kSDS_PZ_stage_int1_pieceType_s, i).c_str(), sp_position);
			}
			else if(found_number == 1)
			{
				StagePiece* t_sp = StagePiece::create(CCSTR_CWF("puzzle%d_stage%d_piece.png", recent_puzzle_number, i)->getCString(),
													  i, stage_level, sp_position, stage_rect, true, true,
													  NSDS_GS(recent_puzzle_number, kSDS_PZ_stage_int1_pieceType_s, i),
													  this, menu_selector(PuzzleMapScene::stageAction));
				t_sp->mySetTouchEnable(false);
				
				if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		t_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
				else														t_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
				
				t_sp->setChangable(CCSTR_CWF("puzzle%d_stage%d_thumbnail.png", recent_puzzle_number, i)->getCString(), is_have_card[0], is_have_card[1], is_have_card[2]);
				t_sp->setPuzzleMode(my_puzzle_mode);
				t_sp->shadow_node = addShadow(i, NSDS_GS(recent_puzzle_number, kSDS_PZ_stage_int1_pieceType_s, i).c_str(), sp_position);
			}
			else
			{
				StagePiece* t_sp = StagePiece::create("test_puzzle_empty.png",
													  i, stage_level, sp_position, stage_rect, false, false,
													  NSDS_GS(recent_puzzle_number, kSDS_PZ_stage_int1_pieceType_s, i),
													  this, menu_selector(PuzzleMapScene::stageAction));
				t_sp->mySetTouchEnable(false);
				
				if(my_puzzle_mode == kPM_default && t_sp->isBoarder())		t_node->addChild(t_sp, kPMS_Z_boarderStage + t_sp->getStageNumber(), t_sp->getStageNumber());
				else														t_node->addChild(t_sp, kPMS_Z_stage + t_sp->getStageNumber(), t_sp->getStageNumber());
			}
		}
	}
	
	void endLoadedMovingMapNode()
	{
		map_node->removeFromParent();
		map_node = after_map_node;
		after_map_node = NULL;
		
		endSwitchMapNode();
	}
	
	MapModeState original_mms;
	void endTingMapNode()
	{
		map_mode_state = original_mms;
		is_menu_enable = true;
	}
	
	void endSwitchMapNode()
	{
		map_mode_state = kMMS_uiMode;
		is_menu_enable = true;
	}
	
	CCNode* createMapNode()
	{
		CCNode* t_node = CCNode::create();
		
		int puzzle_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
		int found_index = -1;
		for(int i=0;i<puzzle_cnt && found_index == -1;i++)
		{
			if(recent_puzzle_number == NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1))
				found_index = i+1;
		}
		
		if(NSDS_GI(kSDS_GI_puzzleList_int1_version_i, found_index) > NSDS_GI(recent_puzzle_number, kSDS_PZ_version_i))
		{
			CCSprite* t_back = CCSprite::create("whitePaper.png");
			t_back->setColor(ccBLUE);
			t_back->setScaleX(520.f/480.f);
			t_back->setScaleY(340.f/320.f);
			t_back->setPosition(CCPointZero);
			t_node->addChild(t_back, 0, 99999);
			
			CCLabelTTF* t_title = CCLabelTTF::create(NSDS_GS(kSDS_GI_puzzleList_int1_title_s, found_index).c_str(), mySGD->getFont().c_str(), 23);
			t_title->setPosition(ccp(0,100));
			t_node->addChild(t_title, 0, 99998);
			
			t_node->setTag(kPMS_MT_notLoaded);
		}
		else
		{
			t_node->setTag(kPMS_MT_loaded);
		}
		
		
		t_node->getCamera()->setEyeXYZ(0, -1.f, 2.f);
		t_node->setScale(0.53f);
		
		return t_node;
	}
	
	void stageAction(CCObject* sender);
	
	MapModeState before_map_mode_state;
	int ing_check_puzzle;
	void startPuzzleModeChange(PuzzleMode t_mode)
	{
		before_map_mode_state = map_mode_state;
		map_mode_state = kMMS_changeMode;
		my_puzzle_mode = t_mode;
		myDSH->setIntegerForKey(kDSH_Key_puzzleMode, my_puzzle_mode);
		ing_check_puzzle = start_stage_number;
		schedule(schedule_selector(PuzzleMapScene::ingPuzzleModeChange), 2.f/60.f);
	}
	void ingPuzzleModeChange()
	{
		bool is_found = false;
		CCNode* found_node;
		
		while(!is_found && ing_check_puzzle < start_stage_number+stage_count)
		{
			StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(ing_check_puzzle);
			if(t_sp->isChangable())
			{
				t_sp->shadow_node->stopAllActions();
				t_sp->shadow_node->setOpacity(0);
				found_node = t_sp;
				is_found = true;
			}
			ing_check_puzzle++;
		}
		
		if(is_found)
		{
			CCOrbitCamera* t_orbit1 = CCOrbitCamera::create(0.2f, 1.f, 0, 0, 90, 0, 0);
			CCCallFuncO* t_call1 = CCCallFuncO::create(this, callfuncO_selector(PuzzleMapScene::changePiece), found_node);
			CCOrbitCamera* t_orbit2 = CCOrbitCamera::create(0.2f, 1.f, 0, -90, 90, 0, 0);
			CCCallFuncO* t_call2 = CCCallFuncO::create(this, callfuncO_selector(PuzzleMapScene::endChangePiece), found_node);
			CCSequence* t_seq = CCSequence::create(t_orbit1, t_call1, t_orbit2, t_call2, NULL);
			found_node->runAction(t_seq);
		}
		
		if(ing_check_puzzle >= start_stage_number+stage_count)
		{
			stopPuzzleModeChange();
		}
	}
	void stopPuzzleModeChange()
	{
		unschedule(schedule_selector(PuzzleMapScene::ingPuzzleModeChange));
		
		is_menu_enable = true;
		map_mode_state = before_map_mode_state;
	}
	
	void changePiece(CCObject* sender);
	
	void endChangePiece(CCObject* sender)
	{
		CCFadeTo* t_fade = CCFadeTo::create(0.3f, 255);
		((StagePiece*)sender)->shadow_node->runAction(t_fade);
	}
	
	void resetStagePiece()
	{
		if(touched_stage_number != 0)
		{
			StagePiece* t_sp = (StagePiece*)map_node->getChildByTag(touched_stage_number);
			if(t_sp)
				t_sp->touchCancelled(NULL, NULL);
			touched_stage_number = 0;
		}
	}
	
	virtual void registerWithTouchDispatcher(void);
};


#endif /* defined(__DGproto__PuzzleMapScene__) */
