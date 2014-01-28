//
//  PuzzleScene.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 17..
//
//

#include "PuzzleScene.h"
#include "DataStorageHub.h"
#include "CountingBMLabel.h"
#include "StarGoldData.h"
#include "HeartTime.h"
#include "ShopPopup.h"
#include "ServerDataSave.h"
#include "MainFlowScene.h"
#include "StageImgLoader.h"
#include "UnknownFriends.h"
#include "GDWebSprite.h"
#include "CumberShowWindow.h"
#include "StartSettingScene.h"
#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "ClearPopup.h"
#include "FailPopup.h"
#include "TutorialFlowStep.h"
#include "AlertEngine.h"

CCScene* PuzzleScene::scene()
{
    CCScene *scene = CCScene::create();
    
    PuzzleScene *layer = PuzzleScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer);
	
    return scene;
}

bool PuzzleScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	
	friend_point_popup = NULL;
	start_menu = NULL;
	challenge_menu = NULL;
	rank_table = NULL;
	
	selected_friend_idx = -1;
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
		before_scene_name = "clear";
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_fail)
		before_scene_name = "fail";
	else
		before_scene_name = "other";
	
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
	
	if(before_scene_name == "other")
	{
		bool have_not_cleared_stage = false;
		vector<int> not_cleared_stage_list;
		vector<int> cleared_stage_list;
		
		for(int i=start_stage;i<start_stage+stage_count;i++)
		{
			if(!myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, i))
			{
				have_not_cleared_stage = true;
				not_cleared_stage_list.push_back(i);
			}
			else
				cleared_stage_list.push_back(i);
		}
		
		if(have_not_cleared_stage) // 아직 못 깬 스테이지가 있다
		{
			bool have_can_enter_stage = false;
			
			vector<int> can_enter_stage_list;
			vector<int> can_enter_stage_level_list;
			vector<int> can_enter_stage_pieceNumber_list;
			
			for(int i=0;i<not_cleared_stage_list.size();i++)
			{
				int stage_number = not_cleared_stage_list[i];
				if(stage_number == 1 || myDSH->getBoolForKey(kDSH_Key_isOpenStage_int1, stage_number) ||
				   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
					(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 ||
					 myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
				{
					have_can_enter_stage = true;
					can_enter_stage_list.push_back(stage_number);
					can_enter_stage_level_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_level_i, stage_number));
					can_enter_stage_pieceNumber_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number));
				}
			}
			
			if(have_can_enter_stage) // 못 깬 스테이지 중에 입장가능한 스테이지가 있다
			{
				// 못 깬 스테이지 중 입장 가능한 스테이지 중 가장 낮은 level의 스테이지 중 가장 낮은 피스번호를 가진 스테이지를 선택
				int minimum_index = 0;
				int minimum_level = can_enter_stage_level_list[minimum_index];
				int minimum_pieceNumber = can_enter_stage_pieceNumber_list[minimum_index];
				
				for(int i=1;i<can_enter_stage_list.size();i++)
				{
					if(can_enter_stage_level_list[i] <= minimum_level && can_enter_stage_pieceNumber_list[i] < minimum_pieceNumber)
					{
						minimum_index = i;
						minimum_level = can_enter_stage_level_list[minimum_index];
						minimum_pieceNumber = can_enter_stage_pieceNumber_list[minimum_index];
					}
				}
				
				selected_stage_number = can_enter_stage_list[minimum_index];
				myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
			}
			else // 못 깬 스테이지 중에 입장가능한 스테이지가 없다
			{
				selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number);
				if(selected_stage_number <= 0 || selected_stage_number <= start_stage || selected_stage_number >= start_stage + stage_count) // 마지막 플레이 스테이지 기록이 없거나, 범위 밖에 있다
				{
					// 깬 스테이지 중 가장 낮은 level의 스테이지 중 가장 낮은 피스번호를 가진 스테이지를 선택
					
					vector<int> cleared_stage_level_list;
					vector<int> cleared_stage_pieceNumber_list;
					
					for(int i=0;i<cleared_stage_list.size();i++)
					{
						int stage_number = cleared_stage_list[i];
						cleared_stage_level_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_level_i, stage_number));
						cleared_stage_pieceNumber_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, stage_number));
					}
					
					int minimum_index = 0;
					int minimum_level = cleared_stage_level_list[minimum_index];
					int minimum_pieceNumber = cleared_stage_pieceNumber_list[minimum_index];
					
					for(int i=1;i<cleared_stage_list.size();i++)
					{
						if(cleared_stage_level_list[i] <= minimum_level && cleared_stage_pieceNumber_list[i] < minimum_pieceNumber)
						{
							minimum_index = i;
							minimum_level = cleared_stage_level_list[minimum_index];
							minimum_pieceNumber = cleared_stage_pieceNumber_list[minimum_index];
						}
					}
					
					selected_stage_number = cleared_stage_list[minimum_index];
					myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
				}
			}
		}
		else // 모든 스테이지를 클리어는 했다
		{
			bool is_perfect = true;
			
			vector<int> perfect_stage_list;
			vector<int> perfect_stage_pieceNumber_list;
			
			vector<int> not_perfect_stage_list;
			vector<int> not_perfect_stage_level_list;
			vector<int> not_perfect_stage_pieceNumber_list;
			
			for(int i=start_stage;i<start_stage+stage_count;i++)
			{
				bool is_stage_perfect = true;
				for(int j=1;j<=3;j++)
				{
					if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(i, kSDS_SI_level_int1_card_i, j)) <= 0)
					{
						is_stage_perfect = false;
						break;
					}
				}
				
				if(is_stage_perfect)
				{
					perfect_stage_list.push_back(i);
					perfect_stage_pieceNumber_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, i));
				}
				else
				{
					is_perfect = false;
					not_perfect_stage_list.push_back(i);
					not_perfect_stage_level_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_level_i, i));
					not_perfect_stage_pieceNumber_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, i));
				}
			}
			
			if(is_perfect) // 각 피스의 모든 카드를 획득한 퍼펙트 상태이다
			{
				// 퍼펙트 스테이지 중 가장 낮은 피스번호를 가진 스테이지를 선택
				int minimum_index = 0;
				int minimum_pieceNumber = perfect_stage_pieceNumber_list[minimum_index];
				
				for(int i=1;i<perfect_stage_list.size();i++)
				{
					if(perfect_stage_pieceNumber_list[i] < minimum_pieceNumber)
					{
						minimum_index = i;
						minimum_pieceNumber = perfect_stage_pieceNumber_list[minimum_index];
					}
				}
				
				selected_stage_number = perfect_stage_list[minimum_index];
				myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
			}
			else // 퍼펙트 아닌 상태이다
			{
				// 미완성 스테이지 중 가장 낮은 level의 스테이지 중 가장 낮은 피스번호를 가진 스테이지를 선택
				int minimum_index = 0;
				int minimum_level = not_perfect_stage_level_list[minimum_index];
				int minimum_pieceNumber = not_perfect_stage_pieceNumber_list[minimum_index];
				
				for(int i=1;i<not_perfect_stage_list.size();i++)
				{
					if(not_perfect_stage_level_list[i] <= minimum_level && not_perfect_stage_pieceNumber_list[i] < minimum_pieceNumber)
					{
						minimum_index = i;
						minimum_level = not_perfect_stage_level_list[minimum_index];
						minimum_pieceNumber = not_perfect_stage_pieceNumber_list[minimum_index];
					}
				}
				
				selected_stage_number = not_perfect_stage_list[minimum_index];
				myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
			}
		}
	}
	else if(before_scene_name == "fail" || before_scene_name == "clear")
	{
		selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number);
		if(selected_stage_number <= 0 || selected_stage_number <= start_stage || selected_stage_number >= start_stage + stage_count) // 마지막 플레이 스테이지 기록이 없거나, 범위 밖에 있다
		{
			// 입장 가능한 스테이지 중 가장 낮은 level의 스테이지 중 가장 낮은 피스번호를 가진 스테이지를 선택
			
			vector<int> can_enter_stage_list;
			vector<int> can_enter_stage_level_list;
			vector<int> can_enter_stage_pieceNumber_list;
			
			for(int i=start_stage;i<start_stage+stage_count;i++)
			{
				if(i == 1 || myDSH->getBoolForKey(kDSH_Key_isOpenStage_int1, i) ||
				   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, i) == 0 &&
					(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i) == 0 ||
					 myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i)))))
				{
					can_enter_stage_list.push_back(i);
					can_enter_stage_level_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_level_i, i));
					can_enter_stage_pieceNumber_list.push_back(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, i));
				}
			}
			
			int minimum_index = 0;
			int minimum_level = can_enter_stage_level_list[minimum_index];
			int minimum_pieceNumber = can_enter_stage_pieceNumber_list[minimum_index];
			
			for(int i=1;i<can_enter_stage_list.size();i++)
			{
				if(can_enter_stage_level_list[i] <= minimum_level && can_enter_stage_pieceNumber_list[i] < minimum_pieceNumber)
				{
					minimum_index = i;
					minimum_level = can_enter_stage_level_list[minimum_index];
					minimum_pieceNumber = can_enter_stage_pieceNumber_list[minimum_index];
				}
			}
			
			selected_stage_number = can_enter_stage_list[minimum_index];
			myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
		}
	}
	
	
	
	
	
	
	
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
		myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(selected_card_number > 0)
		{
			int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) + 1;
			myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number, durability);
		}
		
		bool is_not_empty_card[3] = {false,};
		
		clear_is_empty_piece = true;
		int played_stage_number = mySD->getSilType();
		for(int i=1;i<=3;i++)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(played_stage_number, kSDS_SI_level_int1_card_i, i)) > 0)
			{
				clear_is_empty_piece = false;
				is_not_empty_card[i-1] = true;
			}
		}
		
		if(mySGD->getIsNotClearedStage())
		{
			for(int i=start_stage;i<start_stage+stage_count;i++)
			{
				if(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i) == played_stage_number)
				{
					clear_is_stage_unlock = true;
					next_stage_number = i;
					break;
				}
			}
		}
		
		
		int take_level;
		if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 3;
		else if(mySGD->is_exchanged || mySGD->is_showtime)	take_level = 2;
		else												take_level = 1;
		
		clear_star_take_level = take_level;
		clear_is_empty_star = !is_not_empty_card[take_level-1];
		
		
		if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)) == 0)
		{
			mySGD->setClearRewardGold(NSDS_GI(kSDS_CI_int1_reward_i, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			myDSH->setIntegerForKey(kDSH_Key_cardTakeCnt, myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt) + 1);
			myDSH->setIntegerForKey(kDSH_Key_hasGottenCard_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level), myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt));
			myDSH->setIntegerForKey(kDSH_Key_takeCardNumber_int1, myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt), NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
			
			myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level), NSDS_GI(kSDS_CI_int1_durability_i, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level), 1);
			myDSH->setIntegerForKey(kDSH_Key_cardMaxDurability_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level), NSDS_GI(kSDS_CI_int1_durability_i, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			myDSH->setStringForKey(kDSH_Key_cardPassive_int1, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level), NSDS_GS(kSDS_CI_int1_passive_s, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			
			mySGD->addHasGottenCardNumber(NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
		}
		else
		{
			int card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level);
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) == 0)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, card_number, NSDS_GI(kSDS_CI_int1_durability_i, card_number));
				myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, card_number, 1);
				myDSH->setIntegerForKey(kDSH_Key_cardMaxDurability_int1, card_number, NSDS_GI(kSDS_CI_int1_durability_i, card_number));
				myDSH->setStringForKey(kDSH_Key_cardPassive_int1, card_number, NSDS_GS(kSDS_CI_int1_passive_s, card_number));
			}
			else
			{
				myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, card_number, myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, card_number));
			}
		}
	}
	
	
	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kPuzzleZorder_back);
	
	piece_mode = kPieceMode_default;//(PieceMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode);
	setPuzzle();
	
	setTop();
	setRight();
	
	is_menu_enable = true;
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		showClearPopup();
	}
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_fail)
	{
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		showFailPopup();
	}
	else
	{
		TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
		
		if(recent_step == kTutorialFlowStep_pieceClick)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_pieceClick);
			addChild(t_tutorial, kPuzzleZorder_popup);
			
			tutorial_node = t_tutorial;
		}
		else if(recent_step == kTutorialFlowStep_readyClick)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_readyClick);
			addChild(t_tutorial, kPuzzleZorder_popup);
			
			tutorial_node = t_tutorial;
		}
		else if(recent_step == kTutorialFlowStep_pieceClick2)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_pieceClick2);
			addChild(t_tutorial, kPuzzleZorder_popup);
			
			tutorial_node = t_tutorial;
		}
		else if(recent_step == kTutorialFlowStep_readyClick2)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_readyClick2);
			addChild(t_tutorial, kPuzzleZorder_popup);
			
			tutorial_node = t_tutorial;
		}
		else if(recent_step == kTutorialFlowStep_backClick)
		{
			int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
			int recent_get_card_cnt = 0;
			int take_card_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
			for(int i=1;i<=take_card_cnt;i++)
			{
				int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
				if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
					recent_get_card_cnt++;
			}
			if(selected_card_number > 0 && recent_get_card_cnt >= 2)
			{
				TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
				t_tutorial->initStep(kTutorialFlowStep_backClick);
				addChild(t_tutorial, kPuzzleZorder_popup);
				
				tutorial_node = t_tutorial;
			}
		}
	}
	
	return true;
}

void PuzzleScene::showClearPopup()
{
	is_menu_enable = false;
	
	ClearPopup* t_popup = ClearPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::hideClearPopup));
	addChild(t_popup, kPuzzleZorder_popup);
}

void PuzzleScene::hideClearPopup()
{
	if(mySD->getSilType() >= 10000)
	{
		is_menu_enable = true;
	}
	else
	{
		TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
		if(recent_step == kTutorialFlowStep_pieceType)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_pieceType);
			addChild(t_tutorial, kPuzzleZorder_popup);
			
			tutorial_node = t_tutorial;
		}
		else if(recent_step == kTutorialFlowStep_backClick)
		{
			int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
			int recent_get_card_cnt = 0;
			int take_card_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
			for(int i=1;i<=take_card_cnt;i++)
			{
				int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
				if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
					recent_get_card_cnt++;
			}
			if(selected_card_number > 0 && recent_get_card_cnt >= 2)
			{
				TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
				t_tutorial->initStep(kTutorialFlowStep_backClick);
				addChild(t_tutorial, kPuzzleZorder_popup);
				
				tutorial_node = t_tutorial;
			}
		}
		
		if(clear_is_empty_piece)
			showGetPuzzle();
		else
		{
			if(clear_is_empty_star)
				showGetStar();
			else
			{
				if(clear_is_stage_unlock)
					showUnlockEffect();
				else
					is_menu_enable = true;
			}
		}
	}
}

enum PuzzleNodeZorder{
	kPuzzleNodeZorder_center,
	kPuzzleNodeZorder_shadow,
	kPuzzleNodeZorder_puzzle,
	kPuzzleNodeZorder_piece,
	kPuzzleNodeZorder_strokePiece,
	kPuzzleNodeZorder_selected,
	kPuzzleNodeZorder_haveCardCntCase,
	kPuzzleNodeZorder_changeMode,
	kPuzzleNodeZorder_getPieceEffect
};

void PuzzleScene::showGetPuzzle()
{
	CCLog("get piece animation");
	CCSprite* get_piece_title = CCSprite::create("get_piece_title.png");
	PuzzlePiece* new_piece = (PuzzlePiece*)puzzle_node->getChildByTag(mySD->getSilType());
	get_piece_title->setPosition(ccpAdd(new_piece->getPosition(), ccp(0, 45)));
	puzzle_node->addChild(get_piece_title, kPuzzleNodeZorder_getPieceEffect);
	
	new_piece->startGetPieceAnimation(this, callfuncCCp_selector(PuzzleScene::createGetPuzzleParticle));
	
	CCDelayTime* t_delay = CCDelayTime::create(1.f);
	CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzleScene::endGetPuzzle));
	CCCallFunc* t_call2 = CCCallFunc::create(get_piece_title, callfunc_selector(CCSprite::removeFromParent));
	CCSequence* t_seq = CCSequence::create(t_delay, t_fade, t_call1, t_call2, NULL);
	get_piece_title->runAction(t_seq);
}

void PuzzleScene::createGetPuzzleParticle(CCPoint t_point)
{
	random_device rd;
	default_random_engine e1(rd());
	uniform_real_distribution<float> uniform_dist(-50, 50);
	
	CCPoint random_value;
	random_value.x = uniform_dist(e1);
	random_value.y = uniform_dist(e1);
	
	CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(150);
	t_particle->setPositionType(kCCPositionTypeRelative);
	t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("get_piece_particle.png"));
	t_particle->setEmissionRate(400);
	t_particle->setAngle(90.0);
	t_particle->setAngleVar(45.0);
	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
	t_particle->setBlendFunc(blendFunc);
	t_particle->setDuration(0.1);
	t_particle->setEmitterMode(kCCParticleModeGravity);
	t_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
	t_particle->setStartColorVar(ccc4f(0.57f, 0.57f, 0.54f, 0.f));
	t_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0.f));
	t_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
	t_particle->setStartSize(10.0);
	t_particle->setStartSizeVar(5.0);
	t_particle->setEndSize(20.0);
	t_particle->setEndSizeVar(5.0);
	t_particle->setGravity(ccp(0,-400));
	t_particle->setRadialAccel(0.0);
	t_particle->setRadialAccelVar(0.0);
	t_particle->setSpeed(150);
	t_particle->setSpeedVar(70.0);
	t_particle->setTangentialAccel(0);
	t_particle->setTangentialAccelVar(0);
	t_particle->setTotalParticles(150);
	t_particle->setLife(0.40);
	t_particle->setLifeVar(0.5);
	t_particle->setStartSpin(0);
	t_particle->setStartSpinVar(180);
	t_particle->setEndSpin(0);
	t_particle->setEndSpinVar(180);
	t_particle->setPosVar(ccp(10,10));
	t_particle->setPosition(ccpAdd(t_point, random_value));
	t_particle->setAutoRemoveOnFinish(true);
	puzzle_node->addChild(t_particle, kPuzzleNodeZorder_getPieceEffect);
}

void PuzzleScene::endGetPuzzle()
{
	showGetStar();
}

void PuzzleScene::showGetStar()
{
	CCLog("get star animation : %d", mySD->getSilType());
	PuzzlePiece* new_piece = (PuzzlePiece*)puzzle_node->getChildByTag(mySD->getSilType());
	new_piece->startGetStarAnimation(clear_star_take_level, this, callfunc_selector(PuzzleScene::endGetStar));
}

void PuzzleScene::endGetStar()
{
	if(clear_is_first_puzzle_success)
	{
		showSuccessPuzzleEffect();
	}
	else
	{
		if(clear_is_first_perfect)
		{
			showPerfectPuzzleEffect();
		}
		else
		{
			if(clear_is_stage_unlock)
			{
				showUnlockEffect();
			}
			else
				is_menu_enable = true;
		}
	}
}

void PuzzleScene::showSuccessPuzzleEffect()
{
	CCLog("success puzzle animation");
	endSuccessPuzzleEffect();
}

void PuzzleScene::endSuccessPuzzleEffect()
{
	mySGD->setIsUnlockPuzzle(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)+1);
	CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
}

void PuzzleScene::showPerfectPuzzleEffect()
{
	CCLog("perfect puzzle animation");
	
	int start_stage = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
	
	CCLog("start_stage : %d, stage_count : %d", start_stage, stage_count);
	
	for(int i=start_stage;i<start_stage+stage_count;i++)
	{
		PuzzlePiece* new_piece = (PuzzlePiece*)puzzle_node->getChildByTag(i);
		new_piece->startGetPieceAnimation(this, callfuncCCp_selector(PuzzleScene::createGetPuzzleParticle));
	}
	
	CCDelayTime* t_delay = CCDelayTime::create(2.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleScene::endPerfectPuzzleEffect));
	CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
	
	runAction(t_seq);
}
void PuzzleScene::endPerfectPuzzleEffect()
{
	is_menu_enable = true;
}

void PuzzleScene::showUnlockEffect()
{
	CCLog("unlock piece animation");
	if(unlock_cover)
	{
		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleScene::endUnlockEffect));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
		
		unlock_cover->runAction(t_seq);
	}
	else
	{
		endUnlockEffect();
	}
}

void PuzzleScene::endUnlockEffect()
{
	if(unlock_cover)
	{
		unlock_cover->removeFromParent();
		unlock_cover = NULL;
	}
	
	selected_stage_number = next_stage_number;
	setPieceClick(selected_stage_number);
	setRightContent();
	
	is_menu_enable = true;
}

void PuzzleScene::showFailPopup()
{
	is_menu_enable = false;
	
	FailPopup* t_popup = FailPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::hideFailPopup));
	addChild(t_popup, kPuzzleZorder_popup);
}

void PuzzleScene::hideFailPopup()
{
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	if(recent_step == kTutorialFlowStep_backClick)
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		int recent_get_card_cnt = 0;
		int take_card_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=take_card_cnt;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
				recent_get_card_cnt++;
		}
		if(selected_card_number > 0 && recent_get_card_cnt >= 2)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_backClick);
			addChild(t_tutorial, kPuzzleZorder_popup);
			
			tutorial_node = t_tutorial;
		}
	}
	is_menu_enable = true;
}

enum PuzzleMenuTag{
	kPuzzleMenuTag_cancel = 0,
	kPuzzleMenuTag_rubyShop,
	kPuzzleMenuTag_goldShop,
	kPuzzleMenuTag_heartShop,
	kPuzzleMenuTag_friendPointContent,
	kPuzzleMenuTag_friendPointClose,
	kPuzzleMenuTag_tip,
	kPuzzleMenuTag_start,
	kPuzzleMenuTag_challenge,
	kPuzzleMenuTag_rightReward,
	kPuzzleMenuTag_rightRank,
	kPuzzleMenuTag_rightMonster,
	kPuzzleMenuTag_changeMode
};

void PuzzleScene::setPuzzle()
{
	CCSize puzzle_size = CCSizeMake(326, 268);
	
	puzzle_node = CCNode::create();
	puzzle_node->setPosition(ccp(puzzle_size.width/2.f+8, puzzle_size.height/2.f+10));
	addChild(puzzle_node, kPuzzleZorder_puzzle);
	
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	CCSprite* center = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_center.png", puzzle_number)->getCString());
	center->setPosition(CCPointZero);
	puzzle_node->addChild(center, kPuzzleNodeZorder_center);
	
	CCSprite* top = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_original_top.png", puzzle_number)->getCString());
	top->setAnchorPoint(ccp(0.5, 1));
	top->setPosition(ccp(0, puzzle_size.height/2.f));
	puzzle_node->addChild(top, kPuzzleNodeZorder_puzzle);
	
	CCSprite* bottom = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_original_bottom.png", puzzle_number)->getCString());
	bottom->setAnchorPoint(ccp(0.5, 0));
	bottom->setPosition(ccp(0, -puzzle_size.height/2.f));
	puzzle_node->addChild(bottom, kPuzzleNodeZorder_puzzle);
	
	CCSprite* left = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_original_left.png", puzzle_number)->getCString());
	left->setAnchorPoint(ccp(0, 0.5));
	left->setPosition(ccp(-puzzle_size.width/2.f, 0));
	puzzle_node->addChild(left, kPuzzleNodeZorder_puzzle);
	
	CCSprite* right = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_original_right.png", puzzle_number)->getCString());
	right->setAnchorPoint(ccp(1, 0.5));
	right->setPosition(ccp(puzzle_size.width/2.f, 0));
	puzzle_node->addChild(right, kPuzzleNodeZorder_puzzle);
	
	CCSprite* embo_top = CCSprite::create("puzzle_embo_top.png");
	embo_top->setPosition(top->getPosition());
	puzzle_node->addChild(embo_top, kPuzzleNodeZorder_puzzle);
	
	CCSprite* embo_bottom = CCSprite::create("puzzle_embo_bottom.png");
	embo_bottom->setPosition(bottom->getPosition());
	puzzle_node->addChild(embo_bottom, kPuzzleNodeZorder_puzzle);
	
	CCSprite* embo_left = CCSprite::create("puzzle_embo_left.png");
	embo_left->setPosition(left->getPosition());
	puzzle_node->addChild(embo_left, kPuzzleNodeZorder_puzzle);
	
	CCSprite* embo_right = CCSprite::create("puzzle_embo_right.png");
	embo_right->setPosition(right->getPosition());
	puzzle_node->addChild(embo_right, kPuzzleNodeZorder_puzzle);
	
	int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
	
	bool is_puzzle_clear = true;
	
	bool must_be_change_selected_stage_number = false;
	int enable_stage_number = -1;
	
	unlock_cover = NULL;
	
	clear_is_first_perfect = !myDSH->getBoolForKey(kDSH_Key_isPerfectPuzzle_int1, puzzle_number);
	
	for(int i=0;i<20;i++)
	{
		CCPoint piece_position = ccpAdd(ccp(-puzzle_size.width/2.f, -puzzle_size.height/2.f), ccp((i%5*120+86)/2,((3-i/5)*120+88)/2));
		
		string piece_type;
		if(i%2 == 0)
			piece_type = "h";
		else
			piece_type = "w";
		
		int stage_number, stage_piece_number;
		
		bool is_stage_piece = false;
		for(int j=start_stage;j<start_stage+stage_count && !is_stage_piece;j++)
		{
			stage_piece_number = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, j);
			if(i+1 == stage_piece_number)
			{
				is_stage_piece = true;
				stage_number = j;
			}
		}
		
		
		if(is_stage_piece)
		{
			int stage_level = SDS_GI(kSDF_puzzleInfo, puzzle_number, CCString::createWithFormat("stage%d_level", stage_number)->getCString());
			if(stage_number == 1 || myDSH->getBoolForKey(kDSH_Key_isOpenStage_int1, stage_number) ||
			   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
				(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 || myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
			{
				bool is_found = false;
				int found_number = 0;
				bool is_have_card[3] = {0,};
				for(int k=3;k>=1;k--)
				{
					int card_number = SDS_GI(kSDF_stageInfo, stage_number, CCString::createWithFormat("level%d_card", k)->getCString());
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
				
				if(!is_have_card[0] || !is_have_card[1] || !is_have_card[2])
					clear_is_first_perfect = false;
				
				if(found_number >= 1 && found_number <= 3)
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::pieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_piece, stage_number);
					t_piece->setTurnInfo(is_have_card[0], is_have_card[1], is_have_card[2]);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_color, piece_type);
					
//					addShadow(piece_type, piece_position, stage_number);
				}
				else // empty
				{
					is_puzzle_clear = false;
					
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::pieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_piece, stage_number);
					t_piece->setTurnInfo(false, false, false);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_empty, piece_type);
				}
				enable_stage_number = stage_number;
			}
			else
			{
				is_puzzle_clear = false;
				
				if(myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number))) // buy
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::buyPieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_strokePiece, stage_number);
					t_piece->setTurnInfo(false, false, false);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_buy, piece_type);
					
//					addShadow(piece_type, piece_position, stage_number);
				}
				else // lock
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::lockPieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_strokePiece, stage_number);
					t_piece->setTurnInfo(false, false, false);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_lock, piece_type);
					
//					addShadow(piece_type, piece_position, stage_number);
				}
				
				if(selected_stage_number == stage_number)
					must_be_change_selected_stage_number = true;
			}
			
			if(clear_is_stage_unlock && stage_number == next_stage_number)
			{
				unlock_cover = CCSprite::create(CCString::createWithFormat("piece_lock_%s.png", piece_type.c_str())->getCString());
				unlock_cover->setPosition(piece_position);
				puzzle_node->addChild(unlock_cover, kPuzzleNodeZorder_haveCardCntCase);
			}
		}
		else
		{
			CCSprite* piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number, i)->getCString());
			piece->setAnchorPoint(ccp(0.5,0.5));
			piece->setPosition(piece_position);
			puzzle_node->addChild(piece, kPuzzleNodeZorder_puzzle);
			
			CCSprite* embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", piece_type.c_str())->getCString());
			embo->setPosition(ccp(piece->getContentSize().width/2.f, piece->getContentSize().height/2.f));
			piece->addChild(embo);
			
//			CCSprite* t_shadow = CCSprite::create("puzzle_shadow_1_piece.png", CCRectMake(piece_type == "h" ? 0 : 92, 0, 92, 92));
//			t_shadow->setPosition(piece_position);
//			shadow_batchnode->addChild(t_shadow);
		}
	}
	
	clear_is_first_puzzle_success = false;
	
	if(is_puzzle_clear && !myDSH->getBoolForKey(kDSH_Key_isClearedPuzzle_int1, puzzle_number))
	{
		clear_is_first_puzzle_success = true;
		
		if(clear_is_first_perfect)
			myDSH->setBoolForKey(kDSH_Key_isPerfectPuzzle_int1, puzzle_number, true);
		
		myDSH->setBoolForKey(kDSH_Key_isClearedPuzzle_int1, puzzle_number, true);
		myDSH->saveUserData({kSaveUserData_Key_openPuzzle}, nullptr);
	}
	
	if(must_be_change_selected_stage_number && enable_stage_number != -1) // 현재 선택된 스테이지가 선택 불가 스테이지라면
	{
		selected_stage_number = enable_stage_number;
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
	}
	
	setPieceClick(selected_stage_number);
	
	have_card_cnt_case = CCSprite::create("have_card_cnt_case.png");
	have_card_cnt_case->setPosition(ccp(0, puzzle_size.height/2.f-have_card_cnt_case->getContentSize().height/2.f));
	have_card_cnt_case->setVisible(false);
	
	int have_card_cnt = 0;
	int total_card_cnt = stage_count*3;
	
	int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
	for(int i=1;i<=card_take_cnt;i++)
	{
		int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
		int card_stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
		if(card_stage_number >= start_stage && card_stage_number < start_stage+stage_count)
			have_card_cnt++;
	}
	
	CCLabelTTF* have_card_cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d / %d", have_card_cnt, total_card_cnt)->getCString(), mySGD->getFont().c_str(), 13);
	have_card_cnt_label->setPosition(ccp(have_card_cnt_case->getContentSize().width/2.f-20, have_card_cnt_case->getContentSize().height/2.f));
	have_card_cnt_case->addChild(have_card_cnt_label);
	
	puzzle_node->addChild(have_card_cnt_case, kPuzzleNodeZorder_haveCardCntCase);
	
	CCSprite* n_change_mode = CCSprite::create("puzzle_change_mode.png");
	CCSprite* s_change_mode = CCSprite::create("puzzle_change_mode.png");
	s_change_mode->setColor(ccGRAY);
	
	CCMenuItem* change_mode_item = CCMenuItemSprite::create(n_change_mode, s_change_mode, this, menu_selector(PuzzleScene::menuAction));
	change_mode_item->setTag(kPuzzleMenuTag_changeMode);
	
	CCMenu* change_mode_menu = CCMenu::createWithItem(change_mode_item);
	change_mode_menu->setPosition(ccp(puzzle_size.width/2.f-n_change_mode->getContentSize().width/2.f+5, puzzle_size.height/2.f-n_change_mode->getContentSize().height/2.f+8));
	puzzle_node->addChild(change_mode_menu, kPuzzleNodeZorder_changeMode);
	change_mode_menu->setTouchPriority(kCCMenuHandlerPriority-1);
}

//void PuzzleScene::addShadow(string piece_type, CCPoint piece_position, int t_stage_number)
//{
//	CCSprite* t_shadow = CCSprite::create("puzzle_shadow_1_piece.png", CCRectMake(piece_type == "h" ? 0 : 92, 0, 92, 92));
//	t_shadow->setPosition(piece_position);
//	shadow_batchnode->addChild(t_shadow);
//	
//	t_shadow->setTag(t_stage_number);
//}

void PuzzleScene::setPieceClick(int t_stage_number)
{
	if(selected_piece_img)
		selected_piece_img->removeFromParent();
	
	PuzzlePiece* target_piece = (PuzzlePiece*)puzzle_node->getChildByTag(selected_stage_number);
	string WorH = target_piece->getWorH();
	selected_piece_img = CCSprite::create(("piece_selected_" + WorH + ".png").c_str());
	selected_piece_img->setPosition(target_piece->getPosition());
	puzzle_node->addChild(selected_piece_img, kPuzzleNodeZorder_selected);
}

void PuzzleScene::pieceAction(int t_stage_number)
{
	CCLog("pieceAction : %d", t_stage_number);
	
	if(!is_menu_enable)
		return;
	
	bool is_action = true;
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_readyClick || recent_step == kTutorialFlowStep_readyClick2 || recent_step == kTutorialFlowStep_pieceType)
		is_action = false;
	else if(recent_step == kTutorialFlowStep_pieceClick || recent_step == kTutorialFlowStep_pieceClick2)
	{
		int check_puzzle_number = 1;
		int start_stage = NSDS_GI(check_puzzle_number, kSDS_PZ_startStage_i);
		if(recent_step == kTutorialFlowStep_pieceClick)
		{
			if(t_stage_number != start_stage)
				is_action = false;
			else
			{
				myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_readyClick);
				removeChild(tutorial_node);
				
				TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
				t_tutorial->initStep(kTutorialFlowStep_readyClick);
				addChild(t_tutorial, kPuzzleZorder_popup);
				
				tutorial_node = t_tutorial;
			}
		}
		else if(recent_step == kTutorialFlowStep_pieceClick2)
		{
			if(t_stage_number != start_stage+1)
				is_action = false;
			else
			{
				myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_readyClick2);
				removeChild(tutorial_node);
				
				TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
				t_tutorial->initStep(kTutorialFlowStep_readyClick2);
				addChild(t_tutorial, kPuzzleZorder_popup);
				
				tutorial_node = t_tutorial;
			}
		}
	}
	else if(recent_step == kTutorialFlowStep_backClick)
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		int recent_get_card_cnt = 0;
		int take_card_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=take_card_cnt;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
				recent_get_card_cnt++;
		}
		if(selected_card_number > 0 && recent_get_card_cnt >= 2)
		{
			is_action = false;
		}
	}
	
	if(is_action)
	{
		selected_stage_number = t_stage_number;
		setPieceClick(selected_stage_number);
		
		setRightContent();
	}
}

void PuzzleScene::buyPieceAction(int t_stage_number)
{
	CCLog("buyPieceAction : %d", t_stage_number);
	
	if(!is_menu_enable)
		return;
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	bool is_action = true;
	if(recent_step == kTutorialFlowStep_pieceClick || recent_step == kTutorialFlowStep_readyClick || recent_step == kTutorialFlowStep_pieceType ||
	   recent_step == kTutorialFlowStep_pieceClick2 || recent_step == kTutorialFlowStep_readyClick2)
	{
		is_action = false;
	}
	else if(recent_step == kTutorialFlowStep_backClick)
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		int recent_get_card_cnt = 0;
		int take_card_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=take_card_cnt;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
				recent_get_card_cnt++;
		}
		if(selected_card_number > 0 && recent_get_card_cnt >= 2)
		{
			is_action = false;
		}
	}
	
	if(is_action)
	{
		is_menu_enable = false;
		
		int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		addChild(t_popup);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
		case_back->setPosition(CCPointZero);
		t_container->addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(230, 250));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setPosition(ccp(0,2));
		t_container->addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(202, 146));
		
		CCLabelTTF* title_label = CCLabelTTF::create("스테이지 열기", mySGD->getFont().c_str(), 20);
		title_label->setPosition(ccp(0, 102));
		t_container->addChild(title_label);
		
		CCLabelTTF* content_label = CCLabelTTF::create(CCString::createWithFormat("%d gold 로 오픈", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, t_stage_number))->getCString(), mySGD->getFont().c_str(), 18);
		content_label->setPosition(CCPointZero);
		t_container->addChild(content_label);
		
		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
																			{
																				is_menu_enable = true;
																				t_popup->removeFromParent();
																			});
		
		CCMenuLambda* close_menu = CCMenuLambda::createWithItem(close_item);
		close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
		close_menu->setPosition(ccp(92,105));
		t_container->addChild(close_menu);
		
		if(mySGD->getGold() >= NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, t_stage_number))
		{
			CCSprite* n_buy = CCSprite::create("popup2_buy.png");
			CCSprite* s_buy = CCSprite::create("popup2_buy.png");
			s_buy->setColor(ccGRAY);
			
			CCMenuItemSpriteLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender){
				mySGD->setGold(mySGD->getGold() - NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, t_stage_number));
				myDSH->setIntegerForKey(kDSH_Key_openStageCnt, myDSH->getIntegerForKey(kDSH_Key_openStageCnt)+1);
				myDSH->setIntegerForKey(kDSH_Key_openStageNumber_int1, myDSH->getIntegerForKey(kDSH_Key_openStageCnt), t_stage_number);
				myDSH->setBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number, true);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				save_userdata_list.push_back(kSaveUserData_Key_openStage);
				
				myDSH->saveUserData(save_userdata_list, nullptr);
				
				
				CCNode* open_puzzle_container = CCNode::create();
				t_popup->setContainerNode(open_puzzle_container);
				
				CCScale9Sprite* open_puzzle_case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
				open_puzzle_case_back->setPosition(CCPointZero);
				open_puzzle_container->addChild(open_puzzle_case_back);
				
				open_puzzle_case_back->setContentSize(CCSizeMake(230, 250));
				
				CCScale9Sprite* open_puzzle_content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
				open_puzzle_content_back->setPosition(ccp(0,2));
				open_puzzle_container->addChild(open_puzzle_content_back);
				
				open_puzzle_content_back->setContentSize(CCSizeMake(202, 146));
				
				CCLabelTTF* open_puzzle_title_label = CCLabelTTF::create("스테이지 오픈", mySGD->getFont().c_str(), 20);
				open_puzzle_title_label->setPosition(ccp(0, 102));
				open_puzzle_container->addChild(open_puzzle_title_label);
				
				CCLabelTTF* open_puzzle_content_label = CCLabelTTF::create("새로운 스테이지가\n오픈 되었습니다.", mySGD->getFont().c_str(), 18);
				open_puzzle_content_label->setPosition(CCPointZero);
				open_puzzle_container->addChild(open_puzzle_content_label);
				
				CCSprite* n_op_ok = CCSprite::create("popup2_ok.png");
				CCSprite* s_op_ok = CCSprite::create("popup2_ok.png");
				s_op_ok->setColor(ccGRAY);
				
				CCMenuItemSpriteLambda* op_ok_item = CCMenuItemSpriteLambda::create(n_op_ok, s_op_ok, [=](CCObject* sender){
					is_menu_enable = true;
					t_popup->removeFromParent();
				});
				
				CCMenuLambda* op_ok_menu = CCMenuLambda::createWithItem(op_ok_item);
				op_ok_menu->setTouchPriority(t_popup->getTouchPriority()-1);
				op_ok_menu->setPosition(ccp(0,-95));
				open_puzzle_container->addChild(op_ok_menu);
				
				PuzzlePiece* b_piece = (PuzzlePiece*)puzzle_node->getChildByTag(t_stage_number);
				int stage_level = b_piece->getLevel();
				CCPoint piece_position = b_piece->getPosition();
				string piece_type = b_piece->getWorH();
				
				puzzle_node->removeChildByTag(t_stage_number);
//				shadow_batchnode->removeChildByTag(t_stage_number);
				
				PuzzlePiece* a_piece = PuzzlePiece::create(t_stage_number, stage_level, this, callfuncI_selector(PuzzleScene::pieceAction));
				a_piece->setPosition(piece_position);
				puzzle_node->addChild(a_piece, kPuzzleNodeZorder_piece, t_stage_number);
				a_piece->initWithPieceInfo(piece_mode, kPieceType_empty, piece_type);
			});
			
			CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
			buy_menu->setTouchPriority(t_popup->getTouchPriority()-1);
			buy_menu->setPosition(ccp(0,-95));
			t_container->addChild(buy_menu);
		}
		else
		{
			CCSprite* buy_img = CCSprite::create("popup2_buy.png");
			buy_img->setColor(ccc3(100, 100, 100));
			buy_img->setPosition(ccp(0,-95));
			t_container->addChild(buy_img);
		}
	}
}

void PuzzleScene::lockPieceAction(int t_stage_number)
{
	CCLog("lockPieceAction : %d", t_stage_number);
}

void PuzzleScene::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	bool is_action = true;
	if(recent_step == kTutorialFlowStep_pieceClick || recent_step == kTutorialFlowStep_pieceType || recent_step == kTutorialFlowStep_pieceClick2)
		is_action = false;
	else if(recent_step == kTutorialFlowStep_backClick)
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		int recent_get_card_cnt = 0;
		int take_card_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=take_card_cnt;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
				recent_get_card_cnt++;
		}
		if(selected_card_number > 0 && recent_get_card_cnt >= 2)
		{
			is_action = false;
			int tag = ((CCNode*)sender)->getTag();
			if(tag == kPuzzleMenuTag_cancel)
			{
				is_menu_enable = false;
				myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_cardCollectionClick);
				CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
			}
		}
	}
	else if(recent_step == kTutorialFlowStep_readyClick || recent_step == kTutorialFlowStep_readyClick2)
	{
		is_action = false;
		int tag = ((CCNode*)sender)->getTag();
		if(tag == kPuzzleMenuTag_start)
		{
			is_menu_enable = false;
			if(recent_step == kTutorialFlowStep_readyClick)
				myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_startClick);
			else if(recent_step == kTutorialFlowStep_readyClick2)
				myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_emptyCardClick);
			
			int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
			myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
			
			mySD->setSilType(selected_stage_number);
			
			CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
		}
	}
	
	if(is_action)
	{
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kPuzzleMenuTag_cancel)
		{
			CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
		}
		else if(tag == kPuzzleMenuTag_rubyShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_ruby);
			t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
			addChild(t_shop, kPuzzleZorder_popup);
		}
		else if(tag == kPuzzleMenuTag_goldShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_gold);
			t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
			addChild(t_shop, kPuzzleZorder_popup);
		}
		else if(tag == kPuzzleMenuTag_heartShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_heart);
			t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
			addChild(t_shop, kPuzzleZorder_popup);
		}
		else if(tag == kPuzzleMenuTag_friendPointContent)
		{
			if(!friend_point_popup)
			{
				CCNode* menu_node = ((CCNode*)sender)->getParent();
				CCNode* top_node = menu_node->getParent();
				friend_point_popup = CCSprite::create("candy_popup.png");
				friend_point_popup->setAnchorPoint(ccp(0.5,1.f));
				friend_point_popup->setPosition(ccp(410,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
				top_node->addChild(friend_point_popup, -1);
				
				CCSprite* n_close = CCSprite::create("candy_popup_close.png");
				CCSprite* s_close = CCSprite::create("candy_popup_close.png");
				s_close->setColor(ccGRAY);
				
				CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(PuzzleScene::menuAction));
				close_item->setTag(kPuzzleMenuTag_friendPointClose);
				
				CCMenu* close_menu = CCMenu::createWithItem(close_item);
				close_menu->setPosition(ccp(friend_point_popup->getContentSize().width/2.f, 25));
				friend_point_popup->addChild(close_menu);
				
				CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(410,menu_node->getPositionY()-12));
				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleScene::popupClose));
				CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
				friend_point_popup->runAction(t_seq);
			}
			else
				is_menu_enable = true;
		}
		else if(tag == kPuzzleMenuTag_friendPointClose)
		{
			CCNode* menu_node = ((CCNode*)sender)->getParent();
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(410,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleScene::closeFriendPointPopup));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
			friend_point_popup->runAction(t_seq);
		}
		else if(tag == kPuzzleMenuTag_tip)
		{
			is_menu_enable = true;
		}
		else if(tag == kPuzzleMenuTag_rightReward)
		{
			recent_right_code = kPuzzleRightCode_reward;
			setRightHead();
			setRightContent();
			is_menu_enable = true;
		}
		else if(tag == kPuzzleMenuTag_rightRank)
		{
			recent_right_code = kPuzzleRightCode_rank;
			setRightHead();
			setRightContent();
			is_menu_enable = true;
		}
		else if(tag == kPuzzleMenuTag_rightMonster)
		{
			recent_right_code = kPuzzleRightCode_monster;
			setRightHead();
			setRightContent();
			is_menu_enable = true;
		}
		else if(tag == kPuzzleMenuTag_start)
		{
			int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
			myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
			
			mySD->setSilType(selected_stage_number);
			CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
		}
		else if(tag == kPuzzleMenuTag_challenge)
		{
			if(selected_friend_idx != -1)
			{
				int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
				myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
				mySD->setSilType(selected_stage_number);
				mySGD->setIsMeChallenge(true);
				mySGD->setMeChallengeTarget(friend_list[selected_friend_idx].user_id, friend_list[selected_friend_idx].nickname);
				CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
			}
			else
			{
				CCLog("not selected friend");
				is_menu_enable = true;
			}
		}
		else if(tag == kPuzzleMenuTag_changeMode)
		{
			int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
			int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
			int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
			
			if(piece_mode == kPieceMode_default)
			{
				piece_mode = kPieceMode_thumb;
				have_card_cnt_case->setVisible(true);
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				piece_mode = kPieceMode_ranker;
				have_card_cnt_case->setVisible(false);
			}
			else if(piece_mode == kPieceMode_ranker)
			{
				piece_mode = kPieceMode_default;
				have_card_cnt_case->setVisible(false);
			}
			
			myDSH->setIntegerForKey(kDSH_Key_puzzleMode, piece_mode);
			
			for(int i=start_stage;i<start_stage+stage_count;i++)
				((PuzzlePiece*)puzzle_node->getChildByTag(i))->turnPiece(piece_mode);
			
			is_menu_enable = true;
		}
	}
}

void PuzzleScene::setRight()
{
//	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//	if(screen_scale_x < 1.f)
//		screen_scale_x = 1.f;
	
	right_case = CCNode::create();
	right_case->setPosition(ccp(480,160));
	addChild(right_case, kPuzzleZorder_right);
	
	CCSprite* right_body = CCSprite::create("puzzle_right_body.png");
	right_body->setPosition(ccp(-right_body->getContentSize().width/2.f-5, 7));
	right_case->addChild(right_body);
	
	recent_right_code = kPuzzleRightCode_reward;
	
	right_head = CCNode::create();
	right_head->setPosition(ccp(-right_body->getContentSize().width/2.f-5, 7+right_body->getContentSize().height/2.f-30));
	right_case->addChild(right_head);
	setRightHead();
	
	reward_node = CCNode::create();
	reward_node->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height/2.f-28));
	reward_node->setTag(0);
	right_body->addChild(reward_node);
	reward_node->setVisible(false);
	
	rank_node = CCNode::create();
	rank_node->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height/2.f-28));
	rank_node->setTag(0);
	right_body->addChild(rank_node);
	rank_node->setVisible(false);
	
//	monster_node = CCNode::create();
//	monster_node->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height/2.f));
//	monster_node->setTag(0);
//	right_body->addChild(monster_node);
//	monster_node->setVisible(false);
	setRightContent();
}

void PuzzleScene::setRightHead()
{
	right_head->removeAllChildren();
	
	if(recent_right_code == kPuzzleRightCode_reward)
	{
//		CCSprite* n_monster = CCSprite::create("puzzle_right_monster_off.png");
//		CCSprite* s_monster = CCSprite::create("puzzle_right_monster_off.png");
//		s_monster->setColor(ccGRAY);
//		
//		CCMenuItem* monster_item = CCMenuItemSprite::create(n_monster, s_monster, this, menu_selector(PuzzleScene::menuAction));
//		monster_item->setTag(kPuzzleMenuTag_rightMonster);
//		
//		CCMenu* monster_menu = CCMenu::createWithItem(monster_item);
//		monster_menu->setPosition(ccp(n_monster->getContentSize().width-4, 0));
//		right_head->addChild(monster_menu);
		
		
		CCSprite* n_rank = CCSprite::create("puzzle_right_rank_off.png");
		CCSprite* s_rank = CCSprite::create("puzzle_right_rank_off.png");
		s_rank->setColor(ccGRAY);
		
		CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(PuzzleScene::menuAction));
		rank_item->setTag(kPuzzleMenuTag_rightRank);
		
		CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
		rank_menu->setPosition(ccp(32, -11));
		right_head->addChild(rank_menu);
		
		
//		CCSprite* reward_img = CCSprite::create("puzzle_right_reward_on.png");
//		reward_img->setPosition(ccp(-33, -12));
//		right_head->addChild(reward_img);
	}
	else if(recent_right_code == kPuzzleRightCode_rank)
	{
		CCSprite* n_reward = CCSprite::create("puzzle_right_reward_off.png");
		CCSprite* s_reward = CCSprite::create("puzzle_right_reward_off.png");
		s_reward->setColor(ccGRAY);
		
		CCMenuItem* reward_item = CCMenuItemSprite::create(n_reward, s_reward, this, menu_selector(PuzzleScene::menuAction));
		reward_item->setTag(kPuzzleMenuTag_rightReward);
		
		CCMenu* reward_menu = CCMenu::createWithItem(reward_item);
		reward_menu->setPosition(ccp(-33, -11));
		right_head->addChild(reward_menu);
		
		
//		CCSprite* n_monster = CCSprite::create("puzzle_right_monster_off.png");
//		CCSprite* s_monster = CCSprite::create("puzzle_right_monster_off.png");
//		s_monster->setColor(ccGRAY);
//		
//		CCMenuItem* monster_item = CCMenuItemSprite::create(n_monster, s_monster, this, menu_selector(PuzzleScene::menuAction));
//		monster_item->setTag(kPuzzleMenuTag_rightMonster);
//		
//		CCMenu* monster_menu = CCMenu::createWithItem(monster_item);
//		monster_menu->setPosition(ccp(n_monster->getContentSize().width-4, 0));
//		right_head->addChild(monster_menu);
		
		
//		CCSprite* rank_img = CCSprite::create("puzzle_right_rank_on.png");
//		rank_img->setPosition(ccp(33,-12));
//		right_head->addChild(rank_img);
	}
	else if(recent_right_code == kPuzzleRightCode_monster)
	{
		CCSprite* n_reward = CCSprite::create("puzzle_right_reward_off.png");
		CCSprite* s_reward = CCSprite::create("puzzle_right_reward_off.png");
		s_reward->setColor(ccGRAY);
		
		CCMenuItem* reward_item = CCMenuItemSprite::create(n_reward, s_reward, this, menu_selector(PuzzleScene::menuAction));
		reward_item->setTag(kPuzzleMenuTag_rightReward);
		
		CCMenu* reward_menu = CCMenu::createWithItem(reward_item);
		reward_menu->setPosition(ccp(-n_reward->getContentSize().width+4, 0));
		right_head->addChild(reward_menu);
		
		
		CCSprite* n_rank = CCSprite::create("puzzle_right_rank_off.png");
		CCSprite* s_rank = CCSprite::create("puzzle_right_rank_off.png");
		s_rank->setColor(ccGRAY);
		
		CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(PuzzleScene::menuAction));
		rank_item->setTag(kPuzzleMenuTag_rightRank);
		
		CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
		rank_menu->setPosition(CCPointZero);
		right_head->addChild(rank_menu);
		
		
//		CCSprite* monster_img = CCSprite::create("puzzle_right_monster_on.png");
//		monster_img->setPosition(ccp(monster_img->getContentSize().width-4, 0));
//		right_head->addChild(monster_img);
	}
}

void PuzzleScene::setRightContent()
{
	right_head->removeChildByTag(999);
	int puzzle_number = NSDS_GI(selected_stage_number, kSDS_SI_puzzle_i);
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("%d-%d", puzzle_number, NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, selected_stage_number))->getCString(),
												 mySGD->getFont().c_str(), 12);
	stage_label->setPosition(ccp(-28,14));
	right_head->addChild(stage_label, 0, 999);
	
	if(recent_right_code == kPuzzleRightCode_reward)
	{
		setReward();
		
		reward_node->setVisible(true);
		rank_node->setVisible(false);
//		monster_node->setVisible(false);
	}
	else if(recent_right_code == kPuzzleRightCode_rank)
	{
		setRank();
		
		reward_node->setVisible(false);
		rank_node->setVisible(true);
//		monster_node->setVisible(false);
	}
	else if(recent_right_code == kPuzzleRightCode_monster)
	{
//		setMonster();
		
		reward_node->setVisible(false);
		rank_node->setVisible(false);
//		monster_node->setVisible(true);
	}
	if(start_menu)
		start_menu->setTouchEnabled(reward_node->isVisible());
	if(challenge_menu)
		challenge_menu->setTouchEnabled(rank_node->isVisible());
	if(rank_table)
		rank_table->setTouchEnabled(rank_node->isVisible());
//	if(monster_start_menu)
//		monster_start_menu->setTouchEnabled(monster_node->isVisible());
}

void PuzzleScene::setReward()
{
	if(reward_node->getTag() != selected_stage_number)
	{
		reward_node->removeAllChildren();
		
		bool is_have_card_list[3] = {false,};
		int step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, 1);
		is_have_card_list[0] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, step_card_number) > 0;
		CCSprite* card_img1;
		if(is_have_card_list[0])
		{
			card_img1 = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", selected_stage_number, 1)->getCString()));
			card_img1->setScale(58.f/card_img1->getContentSize().width);
			reward_node->addChild(card_img1);
		}
		else
		{
			card_img1 = CCSprite::create("puzzle_reward_cardback.png");
			reward_node->addChild(card_img1);
			
			CCSprite* condition = CCSprite::create("puzzle_reward_condition1.png");
			condition->setPosition(ccp(card_img1->getContentSize().width/2.f, card_img1->getContentSize().height/2.f));
			card_img1->addChild(condition);
		}
		card_img1->setPosition(ccp(-32, 44));
		
		
		step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, 2);
		is_have_card_list[1] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, step_card_number) > 0;
		CCSprite* card_img2;
		CCSprite* card_img3;
		if(is_have_card_list[1])
		{
			card_img2 = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", selected_stage_number, 2)->getCString()));
			card_img2->setScale(58.f/card_img2->getContentSize().width);
			reward_node->addChild(card_img2);
			
			card_img3 = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", selected_stage_number, 2)->getCString()));
			card_img3->setScale(58.f/card_img3->getContentSize().width);
			reward_node->addChild(card_img3);
		}
		else
		{
			card_img2 = CCSprite::create("puzzle_reward_cardback.png");
			reward_node->addChild(card_img2);
			
			CCSprite* condition = CCSprite::create("puzzle_reward_condition2.png");
			condition->setPosition(ccp(card_img2->getContentSize().width/2.f, card_img2->getContentSize().height/2.f));
			card_img2->addChild(condition);
			
			card_img3 = CCSprite::create("puzzle_reward_cardback.png");
			reward_node->addChild(card_img3);
			
			CCSprite* condition2 = CCSprite::create("puzzle_reward_condition3.png");
			condition2->setPosition(ccp(card_img3->getContentSize().width/2.f, card_img3->getContentSize().height/2.f));
			card_img3->addChild(condition2);
		}
		card_img2->setPosition(ccp(32, 44));
		card_img3->setPosition(ccp(-32, -40));
		
		
		step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, 3);
		is_have_card_list[2] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, step_card_number) > 0;
		CCSprite* card_img4;
		if(is_have_card_list[2])
		{
			card_img4 = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", selected_stage_number, 3)->getCString()));
			card_img4->setScale(58.f/card_img4->getContentSize().width);
			reward_node->addChild(card_img4);
		}
		else
		{
			card_img4 = CCSprite::create("puzzle_reward_cardback.png");
			reward_node->addChild(card_img4);
			
			CCSprite* condition = CCSprite::create("puzzle_reward_condition4.png");
			condition->setPosition(ccp(card_img4->getContentSize().width/2.f, card_img4->getContentSize().height/2.f));
			card_img4->addChild(condition);
		}
		card_img4->setPosition(ccp(32, -40));
		
		
		
		CCSprite* reward_back = CCSprite::create("puzzle_reward_back.png");
		reward_back->setPosition(ccp(0,2));
		reward_node->addChild(reward_back);
		
		
		
		step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, 1);
		CCLabelTTF* step_reward1 = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
		step_reward1->setPosition(ccpAdd(card_img1->getPosition(), ccp(14, -31)));
		reward_node->addChild(step_reward1);
		
		if(is_have_card_list[0])
		{
			CCSprite* t_complete1 = CCSprite::create("stageinfo_complete.png");
			t_complete1->setPosition(ccpAdd(card_img1->getPosition(), ccp(-12, 19)));
			reward_node->addChild(t_complete1);
		}
		
		
		step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, 2);
		CCLabelTTF* step_reward2 = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
		step_reward2->setPosition(ccpAdd(card_img2->getPosition(), ccp(14, -31)));
		reward_node->addChild(step_reward2);
		
		CCLabelTTF* step_reward3 = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
		step_reward3->setPosition(ccpAdd(card_img3->getPosition(), ccp(14, -31)));
		reward_node->addChild(step_reward3);
		
		if(is_have_card_list[1])
		{
			CCSprite* t_complete2 = CCSprite::create("stageinfo_complete.png");
			t_complete2->setPosition(ccpAdd(card_img2->getPosition(), ccp(-12, 19)));
			reward_node->addChild(t_complete2);
			
			CCSprite* t_complete3 = CCSprite::create("stageinfo_complete.png");
			t_complete3->setPosition(ccpAdd(card_img3->getPosition(), ccp(-12, 19)));
			reward_node->addChild(t_complete3);
		}
		
		
		step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, 3);
		CCLabelTTF* step_reward4 = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_reward_i, step_card_number))->getCString(), mySGD->getFont().c_str(), 8);
		step_reward4->setPosition(ccpAdd(card_img4->getPosition(), ccp(14, -31)));
		reward_node->addChild(step_reward4);
		
		if(is_have_card_list[2])
		{
			CCSprite* t_complete4 = CCSprite::create("stageinfo_complete.png");
			t_complete4->setPosition(ccpAdd(card_img4->getPosition(), ccp(-12, 19)));
			reward_node->addChild(t_complete4);
		}
		
		
		CCSprite* n_start = CCSprite::create("puzzle_right_ready.png");
		CCSprite* s_start = CCSprite::create("puzzle_right_ready.png");
		s_start->setColor(ccGRAY);
		
		CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(PuzzleScene::menuAction));
		start_item->setTag(kPuzzleMenuTag_start);
		
		start_menu = CCMenu::createWithItem(start_item);
		start_menu->setPosition(ccp(0, -112));
		start_menu->setTouchEnabled(false);
		reward_node->addChild(start_menu);
		
		reward_node->setTag(selected_stage_number);
	}
}

void PuzzleScene::setRank()
{
	if(rank_node->getTag() != selected_stage_number)
	{
		friend_list.clear();
		rank_node->removeAllChildren();
		rank_table = NULL;
		
		Json::Value p;
		Json::Value my_info = hspConnector::get()->myKakaoInfo;
		
		PuzzleRankFriendInfo t_my_info;
		t_my_info.nickname = my_info["nickname"].asString();
		t_my_info.img_url = my_info["profile_image_url"].asString();
		t_my_info.user_id = my_info["user_id"].asString();
		t_my_info.score = 0;
		t_my_info.is_play = false;
		t_my_info.is_message_blocked = my_info["message_blocked"].asBool();
		friend_list.push_back(t_my_info);
		
		p["memberIDList"].append(t_my_info.user_id);
		
		for(auto i : KnownFriends::getInstance()->getFriends())
		{
			PuzzleRankFriendInfo t_friend_info;
			t_friend_info.nickname = i.nick;
			t_friend_info.img_url = i.profileUrl;
			t_friend_info.user_id = i.userId;
			t_friend_info.score = 0;
			t_friend_info.is_play = false;
			t_friend_info.is_message_blocked = i.messageBlocked;
			friend_list.push_back(t_friend_info);
			
			p["memberIDList"].append(i.userId);
		}
		
		for(auto i : UnknownFriends::getInstance()->getFriends())
		{
			PuzzleRankFriendInfo fInfo;
			fInfo.nickname = i.nick + "[unknown]";
			fInfo.img_url = "";
			fInfo.user_id = i.userId;
			fInfo.score = 0;
			fInfo.is_play = false;
			fInfo.is_message_blocked = false;
			friend_list.push_back(fInfo);
			
			p["memberIDList"].append(i.userId);
		}
		
		p["stageNo"] = selected_stage_number;
		hspConnector::get()->command("getstagescorelist",p,json_selector(this, PuzzleScene::resultGetStageScoreList));
		
		
		
		CCSprite* n_challenge = CCSprite::create("puzzle_right_ready.png");
		CCSprite* s_challenge = CCSprite::create("puzzle_right_ready.png");
		s_challenge->setColor(ccGRAY);
		
		CCMenuItem* challenge_item = CCMenuItemSprite::create(n_challenge, s_challenge, this, menu_selector(PuzzleScene::menuAction));
		challenge_item->setTag(kPuzzleMenuTag_challenge);
		
		challenge_menu = CCMenu::createWithItem(challenge_item);
		challenge_menu->setPosition(ccp(0, -112));
		challenge_menu->setTouchEnabled(false);
		rank_node->addChild(challenge_menu);
		
		rank_node->setTag(selected_stage_number);
	}
}

void PuzzleScene::resultGetStageScoreList(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS && result_data["param"]["stageNo"].asInt() == selected_stage_number && !rank_table)
	{
		Json::Value score_list = result_data["list"];
		for(int i=0;i<score_list.size();i++)
		{
			vector<PuzzleRankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), score_list[i]["memberID"].asString());
			if(iter != friend_list.end())
			{
				(*iter).score = score_list[i]["score"].asFloat();
				(*iter).is_play = true;
			}
			else
				CCLog("not found friend memberID : %s", score_list[i]["memberID"].asString().c_str());
		}
		
		struct t_FriendSort{
			bool operator() (const PuzzleRankFriendInfo& a, const PuzzleRankFriendInfo& b)
			{
				return a.score > b.score;
			}
		} pred;
		
		sort(friend_list.begin(), friend_list.end(), pred);
		
		for(int i=0;i<friend_list.size();i++)
			friend_list[i].rank = i+1;
		
//		float my_score = 0.f;
//		
//		vector<PuzzleRankFriendInfo>::iterator iter = find(friend_list.begin(), friend_list.end(), hspConnector::get()->myKakaoInfo["user_id"].asString());
//		if(iter != friend_list.end())
//		{
//			my_score = (*iter).score;
//			friend_list.erase(iter);
//		}
//		else
//			CCLog("not found my info");
//		
//		
//		CCLabelTTF* my_score_title = CCLabelTTF::create("내 기록", mySGD->getFont().c_str(), 10);
//		my_score_title->setPosition(ccp(0,85));
//		rank_node->addChild(my_score_title);
//		
//		CCLabelTTF* my_score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", my_score)->getCString(), mySGD->getFont().c_str(), 15);
//		my_score_label->setPosition(ccp(0,71));
//		rank_node->addChild(my_score_label);
		
		
		selected_friend_idx = -1;
		
		CCSize table_size = CCSizeMake(130, 168);
		
//		CCSprite* temp_table = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//		temp_table->setAnchorPoint(CCPointZero);
//		temp_table->setPosition(ccp(-table_size.width/2.f, -table_size.height/2.f+2));
//		temp_table->setOpacity(100);
//		rank_node->addChild(temp_table);
		
		rank_table = CCTableView::create(this, table_size);
		rank_table->setAnchorPoint(CCPointZero);
		rank_table->setDirection(kCCScrollViewDirectionVertical);
		rank_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		rank_table->setPosition(ccp(-table_size.width/2.f, -table_size.height/2.f+2));
		
		rank_table->setDelegate(this);
		rank_node->addChild(rank_table);
		rank_table->setTouchPriority(kCCMenuHandlerPriority+1);
		
		bool is_table_touch_enable = true;
		TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
		if(recent_step == kTutorialFlowStep_pieceClick || recent_step == kTutorialFlowStep_readyClick || recent_step == kTutorialFlowStep_pieceType ||
		   recent_step == kTutorialFlowStep_pieceClick2 || recent_step == kTutorialFlowStep_readyClick2)
			is_table_touch_enable = false;
		else if(recent_step == kTutorialFlowStep_backClick)
		{
			//if check
			is_table_touch_enable = false;
		}
		
		rank_table->setTouchEnabled(rank_node->isVisible() && is_table_touch_enable);
	}
}

CCTableViewCell* PuzzleScene::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	PuzzleRankFriendInfo* member = &friend_list[idx];
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
	string cell_id = (*member).user_id;
	
	CCSprite* back_img;
	if(my_id == cell_id)
		back_img = CCSprite::create("puzzle_right_ranklist_me.png");
	else if(idx == 0)
		back_img = CCSprite::create("puzzle_right_ranklist_gold.png");
	else if(idx == 1)
		back_img = CCSprite::create("puzzle_right_ranklist_silver.png");
	else if(idx == 2)
		back_img = CCSprite::create("puzzle_right_ranklist_bronze.png");
	else
		back_img = CCSprite::create("puzzle_right_ranklist_normal.png");
	back_img->setPosition(CCPointZero);
	back_img->setAnchorPoint(CCPointZero);
	cell->addChild(back_img);
	
	CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setPosition(ccp(21, 21));
	profileImg->setScale(29.f / profileImg->getContentSize().width);
	cell->addChild(profileImg);
	
	if(my_id != cell_id && KnownFriends::getInstance()->findById(cell_id) != nullptr)
	{
		CCSprite* kakao_sign = CCSprite::create("puzzle_right_rank_kakao.png");
		kakao_sign->setPosition(ccp(10,28));
		cell->addChild(kakao_sign);
	}
	
	CCLabelTTF* nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 10);
	nickname_label->setPosition(ccp(89,27));
	cell->addChild(nickname_label);
	
	CCLabelTTF* score_label = CCLabelTTF::create(CCString::createWithFormat("%.0f", (*member).score)->getCString(), mySGD->getFont().c_str(), 8);
	score_label->setColor(ccBLACK);
	score_label->setPosition(ccp(89,12));
	cell->addChild(score_label);
	
	if(idx == 0)
	{
		CCSprite* medal_img = CCSprite::create("puzzle_right_rank_gold.png");
		medal_img->setPosition(ccp(50,20));
		cell->addChild(medal_img);
	}
	else if(idx == 1)
	{
		CCSprite* medal_img = CCSprite::create("puzzle_right_rank_silver.png");
		medal_img->setPosition(ccp(50,20));
		cell->addChild(medal_img);
	}
	else if(idx == 2)
	{
		CCSprite* medal_img = CCSprite::create("puzzle_right_rank_bronze.png");
		medal_img->setPosition(ccp(50,20));
		cell->addChild(medal_img);
	}
	else
	{
		CCLabelTTF* rank_label = CCLabelTTF::create(CCString::createWithFormat("%d", (*member).rank)->getCString(), mySGD->getFont().c_str(), 14);
		rank_label->setPosition(ccp(50,20));
		cell->addChild(rank_label);
	}
	
	if(selected_friend_idx == idx)
	{
		CCSprite* selected_img = CCSprite::create("puzzle_right_rank_selected.png");
		selected_img->setPosition(CCPointZero);
		selected_img->setAnchorPoint(CCPointZero);
		cell->addChild(selected_img);
	}
	
	return cell;
}

void PuzzleScene::scrollViewDidScroll(CCScrollView* view){}
void PuzzleScene::scrollViewDidZoom(CCScrollView* view){}
void PuzzleScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLog("touched cell idx : %d", cell->getIdx());
	
	string touched_id = friend_list[cell->getIdx()].user_id;
	string my_id = hspConnector::get()->myKakaoInfo["user_id"].asString();
	
	if(touched_id != my_id)
	{
		if(selected_friend_idx == -1)
		{
			selected_friend_idx = cell->getIdx();
			table->updateCellAtIndex(selected_friend_idx);
		}
		else if (cell->getIdx() != selected_friend_idx)
		{
			int keep_idx = selected_friend_idx;
			selected_friend_idx = cell->getIdx();
			table->updateCellAtIndex(keep_idx);
			table->updateCellAtIndex(selected_friend_idx);
		}
		else
		{
			int keep_idx = selected_friend_idx;
			selected_friend_idx = -1;
			table->updateCellAtIndex(keep_idx);
		}
	}
}
CCSize PuzzleScene::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(110, 40);
}
unsigned int PuzzleScene::numberOfCellsInTableView(CCTableView *table)
{
	return friend_list.size();
}

//void PuzzleScene::setMonster()
//{
//	if(monster_node->getTag() != selected_stage_number)
//	{
//		monster_node->removeAllChildren();
//		
//		CCSprite* monster_back = CCSprite::create("puzzle_monster_back.png");
//		monster_back->setPosition(ccp(0,0));
//		monster_node->addChild(monster_back);
//		
//		CumberShowWindow* monster_img = CumberShowWindow::create(selected_stage_number);
//		monster_img->setPosition(ccp(0,0));
//		monster_node->addChild(monster_img);
//		
//		CCSprite* n_start = CCSprite::create("puzzle_right_start.png");
//		CCSprite* s_start = CCSprite::create("puzzle_right_start.png");
//		s_start->setColor(ccGRAY);
//		
//		CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(PuzzleScene::menuAction));
//		start_item->setTag(kPuzzleMenuTag_start);
//		
//		monster_start_menu = CCMenu::createWithItem(start_item);
//		monster_start_menu->setPosition(ccp(0, -120));
//		monster_start_menu->setTouchEnabled(false);
//		monster_node->addChild(monster_start_menu);
//		
//		monster_node->setTag(selected_stage_number);
//	}
//}

void PuzzleScene::setTop()
{
	CCSprite* top_case = CCSprite::create("mainflow_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(258,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_case, kPuzzleZorder_top);
	
	CCSprite* n_cancel = CCSprite::create("puzzle_cancel.png");
	CCSprite* s_cancel = CCSprite::create("puzzle_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(PuzzleScene::menuAction));
	cancel_item->setTag(kPuzzleMenuTag_cancel);
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(-32,top_case->getContentSize().height/2.f));
	top_case->addChild(cancel_menu);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(15,top_case->getContentSize().height/2.f+1));
	top_case->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(PuzzleScene::menuAction));
	heart_item->setTag(kPuzzleMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(120,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_menu);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(178,top_case->getContentSize().height/2.f-5));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(PuzzleScene::menuAction));
	gold_item->setTag(kPuzzleMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(212,top_case->getContentSize().height/2.f));
	top_case->addChild(gold_menu);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(261,top_case->getContentSize().height/2.f-5));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(PuzzleScene::menuAction));
	ruby_item->setTag(kPuzzleMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(287,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_menu);
	
	friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	friend_point_label->setPosition(ccp(338,top_case->getContentSize().height/2.f-5));
	top_case->addChild(friend_point_label);
	
	mySGD->setFriendPointLabel(friend_point_label);
	
	CCSprite* n_friend_point = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_friend_point = CCSprite::create("mainflow_top_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(PuzzleScene::menuAction));
	friend_point_item->setTag(kPuzzleMenuTag_friendPointContent);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(362,top_case->getContentSize().height/2.f));
	top_case->addChild(friend_point_menu);
	
	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(PuzzleScene::menuAction));
	tip_item->setTag(kPuzzleMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(394, top_case->getContentSize().height/2.f));
	top_case->addChild(tip_menu);
}

void PuzzleScene::popupClose()
{
	is_menu_enable = true;
}

void PuzzleScene::closeFriendPointPopup()
{
	friend_point_popup->removeFromParent();
	friend_point_popup = NULL;
	is_menu_enable = true;
}

void PuzzleScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void PuzzleScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(PuzzleScene::alertAction));
}