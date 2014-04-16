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
//#include "StartSettingScene.h"
#include "ASPopupView.h"
#include "CCMenuLambda.h"
#include "ClearPopup.h"
#include "FailPopup.h"
#include "TutorialFlowStep.h"
#include "AlertEngine.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"
#include "MyLocalization.h"
#include "SumranMailPopup.h"
#include "OptionPopup.h"
#include "AchievePopup.h"
#include "DiaryZoomPopup.h"
#include "StartSettingPopup.h"

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

enum PuzzleMenuTag{
	kPuzzleMenuTag_cancel = 0,
	kPuzzleMenuTag_rubyShop,
	kPuzzleMenuTag_goldShop,
	kPuzzleMenuTag_heartShop,
	kPuzzleMenuTag_postbox,
	kPuzzleMenuTag_achieve,
	kPuzzleMenuTag_option,
	kPuzzleMenuTag_tip,
	kPuzzleMenuTag_start,
	kPuzzleMenuTag_changeMode
};

bool PuzzleScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	setKeypadEnabled(true);
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	
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
			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(i, kSDS_SI_level_int1_card_i, 1)) <= 0 && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(i, kSDS_SI_level_int1_card_i, 2)) <= 0 && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(i, kSDS_SI_level_int1_card_i, 3)) <= 0)
//			if(!myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, i))
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
//		myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
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
		if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 4;
		else if(mySGD->is_showtime)							take_level = 3;
		else if(mySGD->is_exchanged)						take_level = 2;
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
			
			Json::Value param;
			param["memberID"] = hspConnector::get()->getKakaoID();
			param["cardNo"] = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level);
			
			hspConnector::get()->command("updateCardHistory", param, nullptr);
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
	right_case = NULL;
	setRight();
	
	CCSprite* n_ready = CCSprite::create("puzzle_right_ready.png");
	CCSprite* s_ready = CCSprite::create("puzzle_right_ready.png");
	s_ready->setColor(ccGRAY);
	
	CCMenuItem* ready_item = CCMenuItemSprite::create(n_ready, s_ready, this, menu_selector(PuzzleScene::menuAction));
	ready_item->setTag(kPuzzleMenuTag_start);
	
	ready_menu = CCMenu::createWithItem(ready_item);
	ready_menu->setPosition(ccp(480-65-6,28+8));
	addChild(ready_menu, kPuzzleZorder_top);
	
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
		puzzleOpenning();
		rightOpenning();
		
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

void PuzzleScene::startBacking()
{
	puzzleBacking();
	rightBacking();
	topBacking();
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
//	if(mySD->getSilType() >= 10000)
//	{
//		is_menu_enable = true;
//	}
//	else
//	{
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
//	}
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
	startBacking();
//	CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
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
	setRight();
	
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

void PuzzleScene::puzzleOpenning()
{
	CCPoint original_position = puzzle_node->getPosition();
	puzzle_node->setPositionX(puzzle_node->getPositionX()-600);
	KS::setOpacity(puzzle_node, 0);
	addChild(KSGradualValue<float>::create(1.f, 0.f, 0.5f, [=](float t)
										   {
											   puzzle_node->setPositionX(original_position.x - t*600.f);
											   KS::setOpacity(puzzle_node, 255-t*255);
										   }, [=](float t)
										   {
											   puzzle_node->setPositionX(original_position.x);
											   KS::setOpacity(puzzle_node, 255);
										   }));
}

void PuzzleScene::puzzleBacking()
{
	CCPoint original_position = puzzle_node->getPosition();
	addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
										   {
											   puzzle_node->setPositionX(original_position.x - t*600.f);
											   KS::setOpacity(puzzle_node, 255-t*255);
										   }, [=](float t)
										   {
											   puzzle_node->setPositionX(original_position.x - 600.f);
											   KS::setOpacity(puzzle_node, 0);
											   
											   CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
										   }));
}

void PuzzleScene::setPuzzle()
{
	CCSize puzzle_size = CCSizeMake(332, 272);
	
	puzzle_node = CCNode::create();
	puzzle_node->setPosition(ccp(puzzle_size.width/2.f+5, puzzle_size.height/2.f+8));
	addChild(puzzle_node, kPuzzleZorder_puzzle);
	
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
//	CCSprite* center = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_center.png", puzzle_number)->getCString());
//	center->setPosition(CCPointZero);
//	puzzle_node->addChild(center, kPuzzleNodeZorder_center);
	
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
	
//	CCSprite* embo_top = CCSprite::create("puzzle_embo_top.png");
//	embo_top->setPosition(top->getPosition());
//	puzzle_node->addChild(embo_top, kPuzzleNodeZorder_puzzle);
//	
//	CCSprite* embo_bottom = CCSprite::create("puzzle_embo_bottom.png");
//	embo_bottom->setPosition(bottom->getPosition());
//	puzzle_node->addChild(embo_bottom, kPuzzleNodeZorder_puzzle);
//	
//	CCSprite* embo_left = CCSprite::create("puzzle_embo_left.png");
//	embo_left->setPosition(left->getPosition());
//	puzzle_node->addChild(embo_left, kPuzzleNodeZorder_puzzle);
//	
//	CCSprite* embo_right = CCSprite::create("puzzle_embo_right.png");
//	embo_right->setPosition(right->getPosition());
//	puzzle_node->addChild(embo_right, kPuzzleNodeZorder_puzzle);
	
	int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
	
	bool is_puzzle_clear = true;
	
	bool must_be_change_selected_stage_number = false;
	int enable_stage_number = -1;
	
	unlock_cover = NULL;
	
	clear_is_first_perfect = !myDSH->getBoolForKey(kDSH_Key_isPerfectPuzzle_int1, puzzle_number);
	
	for(int i=0;i<20;i++)
	{
		CCPoint piece_position = ccpAdd(ccp(-puzzle_size.width/2.f, -puzzle_size.height/2.f), ccp((i%5*120+92)/2,((3-i/5)*120+92)/2));
		
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
					PieceType t_type;
					if(myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, stage_number))
						t_type = kPieceType_color;
					else
						t_type = kPieceType_empty;
					
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::pieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_piece, stage_number);
					t_piece->setTurnInfo(is_have_card[0], is_have_card[1], is_have_card[2]);
					t_piece->initWithPieceInfo(piece_mode, t_type, piece_type);
					
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
	change_mode_menu->setPosition(ccp(puzzle_size.width/2.f-n_change_mode->getContentSize().width/2.f, puzzle_size.height/2.f-n_change_mode->getContentSize().height/2.f));
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
		
		setRight();
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
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));
		
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
				startBacking();
//				CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
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
			
			StartSettingPopup* t_popup = StartSettingPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
			addChild(t_popup, kPuzzleZorder_popup);
//			CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
		}
	}
	
	if(is_action)
	{
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kPuzzleMenuTag_cancel)
		{
			startBacking();
//			CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
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
		else if(tag == kPuzzleMenuTag_postbox)
		{
			SumranMailPopup* t_pp = SumranMailPopup::create(this, callfunc_selector(PuzzleScene::mailPopupClose), bind(&PuzzleScene::heartRefresh, this));
			addChild(t_pp, kPuzzleZorder_popup);
			
			postbox_count_case->setVisible(false);
		}
		else if(tag == kPuzzleMenuTag_option)
		{
			OptionPopup* t_popup = OptionPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
			addChild(t_popup, kPuzzleZorder_popup);
		}
		else if(tag == kPuzzleMenuTag_achieve)
		{
			AchievePopup* t_ap = AchievePopup::create();
			addChild(t_ap, kPuzzleZorder_popup);
			
			t_ap->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
		}
		else if(tag == kPuzzleMenuTag_tip)
		{
			is_menu_enable = true;
		}
		else if(tag == kPuzzleMenuTag_start)
		{
			int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
			myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
			
			mySD->setSilType(selected_stage_number);
			
			StartSettingPopup* t_popup = StartSettingPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
			addChild(t_popup, kPuzzleZorder_popup);
//			CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
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

void PuzzleScene::mailPopupClose()
{
	countingMessage();
	is_menu_enable = true;
}

void PuzzleScene::heartRefresh()
{
	CCPoint heart_position = heart_time->getPosition();
	CCNode* heart_parent = heart_time->getParent();
	
	heart_time->removeFromParent();
	
	heart_time = HeartTime::create();
	heart_time->setPosition(heart_position);
	heart_parent->addChild(heart_time);
}

void PuzzleScene::rightOpenning()
{
	CCPoint original_position = right_case->getPosition();
	right_case->setPositionX(right_case->getPositionX()+300);
	KS::setOpacity(right_case, 0);
	addChild(KSGradualValue<float>::create(1.f, 0.f, 0.5f, [=](float t)
										   {
											   right_case->setPositionX(original_position.x + t*300.f);
											   KS::setOpacity(right_case, 255-t*255);
										   }, [=](float t)
										   {
											   right_case->setPositionX(original_position.x);
											   KS::setOpacity(right_case, 255);
										   }));
	
	CCPoint ready_position = ready_menu->getPosition();
	ready_menu->setPositionX(ready_menu->getPositionX()+300);
	KS::setOpacity(ready_menu, 0);
	addChild(KSGradualValue<float>::create(1.f, 0.f, 0.5f, [=](float t)
										   {
											   ready_menu->setPositionX(ready_position.x + t*300.f);
											   KS::setOpacity(ready_menu, 255-t*255);
										   }, [=](float t)
										   {
											   ready_menu->setPositionX(ready_position.x);
											   KS::setOpacity(ready_menu, 255);
										   }));
}

void PuzzleScene::rightBacking()
{
	CCPoint original_position = right_case->getPosition();
	addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
										   {
											   right_case->setPositionX(original_position.x + t*300.f);
											   KS::setOpacity(right_case, 255-t*255);
										   }, [=](float t)
										   {
											   right_case->setPositionX(original_position.x + 300.f);
											   KS::setOpacity(right_case, 0);
										   }));
	
	CCPoint ready_position = ready_menu->getPosition();
	addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
										   {
											   ready_menu->setPositionX(ready_position.x + t*300.f);
											   KS::setOpacity(ready_menu, 255-t*255);
										   }, [=](float t)
										   {
											   ready_menu->setPositionX(ready_position.x + 300.f);
											   KS::setOpacity(ready_menu, 0);
										   }));
}

void PuzzleScene::setRight()
{
	if(right_case)
	{
		right_case->removeFromParent();
		right_case = NULL;
	}
	
	right_case = CCNode::create();
	right_case->setPosition(ccp(480,puzzle_node->getPositionY()));
	addChild(right_case, kPuzzleZorder_right);
	
	CCSprite* right_body = CCSprite::create("puzzle_right_body.png");
	right_body->setPosition(ccp(-right_body->getContentSize().width/2.f-6, -1));
	right_case->addChild(right_body);
	
	CCLabelTTF* stage_label = CCLabelTTF::create(CCString::createWithFormat("%d 스테이지", selected_stage_number)->getCString(),
												 mySGD->getFont().c_str(), 12);
	stage_label->setPosition(ccp(right_body->getContentSize().width/2.f,right_body->getContentSize().height-23));
	right_body->addChild(stage_label);
	
	bool is_have_card_list[4] = {false,};
	
	int stage_card_count = NSDS_GI(selected_stage_number, kSDS_SI_cardCount_i);
	for(int i=1;i<=stage_card_count && i <= 4;i++)
	{
		int step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, i);
		is_have_card_list[i-1] = myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, step_card_number) > 0;
		
		CCPoint step_position = ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height-62-(i-1)*45);
		
		if(is_have_card_list[i-1])
		{
			CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("puzzle_right_sumcrop.png"));
			t_clipping->setPosition(step_position);
			right_body->addChild(t_clipping);
			
			t_clipping->setAlphaThreshold(0.1f);
			
			CCSprite* t_inner = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", step_card_number)->getCString()));
			t_inner->setScale(0.4f);
			t_clipping->addChild(t_inner);
			
			int card_rank = NSDS_GI(kSDS_CI_int1_rank_i, step_card_number);
			for(int j=0;j<card_rank;j++)
			{
				CCSprite* t_star = CCSprite::create("puzzle_right_staron.png");
				t_star->setPosition(ccpAdd(step_position, ccp(-43.5f+j*13.5f,9.5f)));
				right_body->addChild(t_star);
			}
			
			CommonButton* show_img = CommonButton::create("보기", 12, CCSizeMake(40, 40), CommonButtonYellow, kCCMenuHandlerPriority);
			show_img->setPosition(ccpAdd(step_position, ccp(33,0)));
			show_img->setFunction([=](CCObject* sender)
								  {
									  if(!is_menu_enable)
										  return;
									  
									  is_menu_enable = false;
									  
									  mySGD->selected_collectionbook = step_card_number;
									  
									  DiaryZoomPopup* t_popup = DiaryZoomPopup::create();
									  t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
									  t_popup->is_before_no_diary = true;
									  addChild(t_popup, kPuzzleZorder_popup);
								  });
			right_body->addChild(show_img);
		}
	}
}

void PuzzleScene::topBacking()
{
	top_list[0]->runAction(CCMoveTo::create(0.5f, ccpAdd(top_list[0]->getPosition(), ccp(0,100)))); // cancel
	top_list[1]->runAction(CCMoveTo::create(0.5f, ccp(78,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3))); // top_heart
	top_list[2]->runAction(CCMoveTo::create(0.5f, ccp(216,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3))); // top_gold
	top_list[3]->runAction(CCMoveTo::create(0.5f, ccp(325,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3))); // top_ruby
}

void PuzzleScene::setTop()
{
	top_list.clear();
	
//	CCSprite* top_case = CCSprite::create("mainflow_top.png");
//	top_case->setAnchorPoint(ccp(0.5f,1.f));
//	top_case->setPosition(ccp(258,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
//	addChild(top_case, kPuzzleZorder_top);
	
	CCSprite* n_cancel = CCSprite::create("puzzle_cancel.png");
	CCSprite* s_cancel = CCSprite::create("puzzle_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(PuzzleScene::menuAction));
	cancel_item->setTag(kPuzzleMenuTag_cancel);
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(25,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(cancel_menu, kPuzzleZorder_top);
	
	top_list.push_back(cancel_menu);
	
	
	CCSprite* top_heart = CCSprite::create("mainflow_top_heart.png");
	top_heart->setAnchorPoint(ccp(0.5f,1.f));
	top_heart->setPosition(ccp(78+35,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_heart, kPuzzleZorder_top);
	
	top_list.push_back(top_heart);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(15,top_heart->getContentSize().height/2.f));
	top_heart->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(PuzzleScene::menuAction));
	heart_item->setTag(kPuzzleMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(top_heart->getContentSize().width-n_heart->getContentSize().width/2.f+5,top_heart->getContentSize().height/2.f));
	top_heart->addChild(heart_menu);
	
	
	CCSprite* top_gold = CCSprite::create("mainflow_top_gold.png");
	top_gold->setAnchorPoint(ccp(0.5f,1.f));
	top_gold->setPosition(ccp(216+23,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_gold, kPuzzleZorder_top);
	
	top_list.push_back(top_gold);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(top_gold->getContentSize().width/2.f + 1,top_gold->getContentSize().height/2.f-5));
	top_gold->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(PuzzleScene::menuAction));
	gold_item->setTag(kPuzzleMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(top_gold->getContentSize().width-n_gold->getContentSize().width/2.f+5,top_gold->getContentSize().height/2.f));
	top_gold->addChild(gold_menu);
	
	
	CCSprite* top_ruby = CCSprite::create("mainflow_top_ruby.png");
	top_ruby->setAnchorPoint(ccp(0.5f,1.f));
	top_ruby->setPosition(ccp(325+10,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_ruby, kPuzzleZorder_top);
	
	top_list.push_back(top_ruby);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(top_ruby->getContentSize().width/2.f + 1,top_ruby->getContentSize().height/2.f-5));
	top_ruby->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(PuzzleScene::menuAction));
	ruby_item->setTag(kPuzzleMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(top_ruby->getContentSize().width-n_ruby->getContentSize().width/2.f+5,top_ruby->getContentSize().height/2.f));
	top_ruby->addChild(ruby_menu);
	
	
	CCSprite* n_postbox = CCSprite::create("mainflow_new_postbox.png");
	CCSprite* s_postbox = CCSprite::create("mainflow_new_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(PuzzleScene::menuAction));
	postbox_item->setTag(kPuzzleMenuTag_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccp(394,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(postbox_menu, kPuzzleZorder_top);
	
	postbox_count_case = CCSprite::create("mainflow_postbox_count.png");
	postbox_count_case->setPosition(ccp(406,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-9));
	addChild(postbox_count_case, kPuzzleZorder_top);
	postbox_count_case->setVisible(false);
	
	postbox_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
	postbox_count_label->setColor(ccc3(95, 60, 30));
	postbox_count_label->setPosition(ccp(postbox_count_case->getContentSize().width/2.f-0.5f, postbox_count_case->getContentSize().height/2.f+0.5f));
	postbox_count_case->addChild(postbox_count_label);
	
	countingMessage();
	
	
	CCSprite* n_achieve = CCSprite::create("mainflow_new_achievement.png");
	CCSprite* s_achieve = CCSprite::create("mainflow_new_achievement.png");
	s_achieve->setColor(ccGRAY);
	
	CCMenuItem* achieve_item = CCMenuItemSprite::create(n_achieve, s_achieve, this, menu_selector(PuzzleScene::menuAction));
	achieve_item->setTag(kPuzzleMenuTag_achieve);
	
	CCMenu* achieve_menu = CCMenu::createWithItem(achieve_item);
	achieve_menu->setPosition(ccp(427,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(achieve_menu, kPuzzleZorder_top);
	
	
	CCSprite* n_option = CCSprite::create("mainflow_new_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_new_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(PuzzleScene::menuAction));
	option_item->setTag(kPuzzleMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(460,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(option_menu, kPuzzleZorder_top);
}

void PuzzleScene::countingMessage()
{
	Json::Value p;
	p["memberID"]=hspConnector::get()->getKakaoID();
	p["type"]=0; // 모든 타입의 메시지를 받겠다는 뜻.
	p["limitDay"] = mySGD->getMsgRemoveDay();
	// 0 이 아니면 해당하는 타입의 메시지가 들어옴.
	
	hspConnector::get()->command("getmessagelist",p,[this](Json::Value r)
								 {
									 GraphDogLib::JsonToLog("getmessagelist", r);
									 if(r["result"]["code"].asInt() != GDSUCCESS)
										 return;
									 Json::Value message_list = r["list"];
									 if(message_list.size() > 0)
									 {
										 postbox_count_case->setVisible(true);
										 
										 if(message_list.size() < 10)
										 {
											 postbox_count_label->setFontSize(10);
											 postbox_count_label->setString(CCString::createWithFormat("%d", message_list.size())->getCString());
										 }
										 else if(message_list.size() < 100)
										 {
											 postbox_count_label->setFontSize(7);
											 postbox_count_label->setString(CCString::createWithFormat("%d", message_list.size())->getCString());
										 }
										 else
										 {
											 postbox_count_label->setFontSize(8);
											 postbox_count_label->setString("...");
										 }
									 }
									 else
									 {
										 postbox_count_case->setVisible(false);
										 postbox_count_label->setString("0");
									 }
								 });
}

void PuzzleScene::popupClose()
{
	is_menu_enable = true;
}

void PuzzleScene::closeFriendPoint()
{
	close_friend_point_action();
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