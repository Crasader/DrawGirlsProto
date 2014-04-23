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
#include "LoadingLayer.h"
#include "KSLabelTTF.h"

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
			int stage_card_count = 4;//NSDS_GI(i, kSDS_SI_cardCount_i);
			have_not_cleared_stage = true;
			for(int j=1;j<=stage_card_count && have_not_cleared_stage;j++)
			{
				if(mySGD->isHasGottenCards(i, j) > 0)
					have_not_cleared_stage = false;
			}
			
			if(have_not_cleared_stage)
				not_cleared_stage_list.push_back(i);
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
				if(stage_number == 1 || mySGD->isClearPiece(stage_number) ||
				   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
					(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 ||
					 mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
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
				int stage_card_count = 4;//NSDS_GI(i, kSDS_SI_cardCount_i);
				for(int j=1;j<=stage_card_count;j++)
				{
					if(mySGD->isHasGottenCards(i, j) <= 0)
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
				if(i == 1 || mySGD->isClearPiece(i) ||
				   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, i) == 0 &&
					(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i) == 0 ||
					 mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i)))))
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
		
		bool is_not_empty_card[3] = {false,};
		
		clear_is_empty_piece = true;
		int played_stage_number = mySD->getSilType();
		int stage_card_count = 4;//NSDS_GI(played_stage_number, kSDS_SI_cardCount_i);
		for(int i=1;i<=stage_card_count;i++)
		{
			if(mySGD->isHasGottenCards(played_stage_number, i) > 0)
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
		
		if(mySGD->isHasGottenCards(mySD->getSilType(), take_level) == 0)
		{
			mySGD->setClearRewardGold(NSDS_GI(kSDS_CI_int1_reward_i, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			
			mySGD->addHasGottenCardNumber(NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
			
			keep_card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level);
			
			LoadingLayer* t_loading = LoadingLayer::create(-900);
			addChild(t_loading, kPuzzleZorder_popup+1);
			
			updateCardHistory(t_loading);
		}
		else
		{
			
		}
		
		PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
		bool is_change_history = false;
		
		if(!mySGD->isClearPiece(mySD->getSilType()))
		{
			t_history.is_clear[take_level-1] = true;
			t_history.clear_count = t_history.try_count;
			
			is_change_history = true;
		}
		else if(!t_history.is_clear[take_level-1])
		{
			t_history.is_clear[take_level-1] = true;
			
			is_change_history = true;
		}
		
		if(is_change_history)
			mySGD->setPieceHistory(t_history, nullptr);
	}
	
	
//	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
//	back_img->setPosition(ccp(240,160));
//	addChild(back_img, kPuzzleZorder_back);
	
	CCSpriteBatchNode* back_batch = CCSpriteBatchNode::create("ingame_side_pattern.png");
	back_batch->setPosition(ccp(240,160));
	addChild(back_batch, kPuzzleZorder_back);
	
	CCPoint base_position = ccp(-284,-180);
	
	for(int i=0;i*26 < 360;i++)
	{
		for(int j=0;j*48 < 568;j++)
		{
			CCSprite* t_back = CCSprite::createWithTexture(back_batch->getTexture());
			t_back->setAnchorPoint(ccp(0,0));
			t_back->setPosition(base_position + ccp(j*48, i*26));
			back_batch->addChild(t_back);
		}
	}
	
	piece_mode = kPieceMode_default;//(PieceMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode);
	setPuzzle();
	
	setTop();
	right_case = NULL;
	right_body = NULL;
	ready_menu = NULL;
	right_mode = kPuzzleRightMode_stage;
	saved_ranking_stage_number = -1;
	loading_progress_img = NULL;
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
		if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_stageSetting)
		{
			openSettingPopup();
		}
		else
		{
			puzzleOpenning();
			rightOpenning();
			topOpenning();
		}
		
		if(mySGD->is_before_stage_img_download)
		{
			mySGD->is_before_stage_img_download = false;
			topReopenning();
		}
		
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
			int recent_get_card_cnt = mySGD->getHasGottenCardsSize();
			if(selected_card_number > 0 && recent_get_card_cnt >= 2)
			{
				TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
				t_tutorial->initStep(kTutorialFlowStep_backClick);
				addChild(t_tutorial, kPuzzleZorder_popup);
				
				tutorial_node = t_tutorial;
			}
		}
	}
	
	addChild(KSTimer::create(5.f, [=](){startAutoTurnPiece();}));
	
	return true;
}

void PuzzleScene::updateCardHistory(CCNode *t_loading)
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["cardNo"] = keep_card_number;
	
	hspConnector::get()->command("updateCardHistory", param, [=](Json::Value result_data)
								 {
									 if(result_data["result"]["code"].asInt() == GDSUCCESS)
									 {
										 t_loading->removeFromParent();
									 }
									 else
									 {
										 updateCardHistory(t_loading);
									 }
								 });
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
	t_popup->replay_func = [=](){openSettingPopup();};
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
			int recent_get_card_cnt = mySGD->getHasGottenCardsSize();
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
	CCSprite* get_piece_title = CCSprite::create(CCString::createWithFormat("get_piece_title_%s.png", myLoc->getSupportLocalCode())->getCString());
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
	
	int start_stage = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
	
	for(int i=start_stage;i<start_stage+stage_count;i++)
	{
		PuzzlePiece* new_piece = (PuzzlePiece*)puzzle_node->getChildByTag(i);
		new_piece->simpleView();
	}
	
	CCSprite* light_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 272));
	light_img->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	light_img->setSkewX(10);
	light_img->setOpacity(0);
	light_img->setPosition(ccp(-130, 0));
	puzzle_node->addChild(light_img, 999);
	
	CCMoveTo* t_move1 = CCMoveTo::create(0.7f, ccp(130,0));
	CCMoveTo* t_move2 = CCMoveTo::create(0.f, ccp(-130,0));
	CCSequence* t_seq1 = CCSequence::create(t_move1, t_move2, NULL);
	
	CCFadeTo* t_fade1 = CCFadeTo::create(0.2f, 100);
	CCDelayTime* t_delay1 = CCDelayTime::create(0.3f);
	CCFadeTo* t_fade2 = CCFadeTo::create(0.2f, 0);
	CCSequence* t_seq2 = CCSequence::create(t_fade1, t_delay1, t_fade2, NULL);
	
	CCSpawn* t_spawn = CCSpawn::create(t_seq1, t_seq2, NULL);
	CCRepeat* t_repeat = CCRepeat::create(t_spawn, 2);
	
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzleScene::pumpPuzzle));
	CCCallFunc* t_call2 = CCCallFunc::create(light_img, callfunc_selector(CCSprite::removeFromParent));
	
	CCSequence* t_seq3 = CCSequence::create(t_repeat, t_call1, t_call2, NULL);
	
	light_img->runAction(t_seq3);
}

void PuzzleScene::pumpPuzzle()
{
	CCScaleTo* t_scale1 = CCScaleTo::create(0.2f, 1.05f);
	CCScaleTo* t_scale2 = CCScaleTo::create(0.2f, 1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(PuzzleScene::endSuccessPuzzleEffect));
	CCSequence* t_seq = CCSequence::create(t_scale1, t_scale2, t_call, NULL);
	
	puzzle_node->runAction(t_seq);
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
		PieceHistory t_history = mySGD->getPieceHistory(next_stage_number);
		t_history.is_open = true;
		mySGD->setPieceHistory(t_history, nullptr);
		
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
	t_popup->replay_func = [=](){openSettingPopup();};
	addChild(t_popup, kPuzzleZorder_popup);
}

void PuzzleScene::hideFailPopup()
{
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	if(recent_step == kTutorialFlowStep_backClick)
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		int recent_get_card_cnt = mySGD->getHasGottenCardsSize();
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
	
	clear_is_first_perfect = !mySGD->getPuzzleHistory(puzzle_number).is_perfect;
	
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
			if(stage_number == 1 || mySGD->isClearPiece(stage_number) ||
			   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
				(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 || mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
			{
				PieceHistory t_history = mySGD->getPieceHistory(stage_number);
				
//				bool is_found = false;
//				int found_number = 0;
//				bool is_have_card[4] = {0,};
//				for(int k=4;k>=1;k--)
//				{
//					if(mySGD->isHasGottenCards(stage_number, k) > 0)
//					{
//						if(!is_found)
//						{
//							is_found = true;
//							found_number = k;
//						}
//						is_have_card[k-1] = true;
//					}
//				}
				
				if(!t_history.is_clear[0] || !t_history.is_clear[1] || !t_history.is_clear[2] || !t_history.is_clear[3])
					clear_is_first_perfect = false;
				
				if(mySGD->isClearPiece(stage_number))
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::pieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_piece, stage_number);
					t_piece->setTurnInfo(t_history.is_clear[0], t_history.is_clear[1], t_history.is_clear[2], t_history.is_clear[3]);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_color, piece_type);
					
//					addShadow(piece_type, piece_position, stage_number);
				}
				else // empty
				{
					is_puzzle_clear = false;
					
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::pieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_piece, stage_number);
					t_piece->setTurnInfo(false, false, false, false);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_empty, piece_type);
				}
				enable_stage_number = stage_number;
			}
			else
			{
				is_puzzle_clear = false;
				
				if(mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number))) // buy
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::buyPieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_strokePiece, stage_number);
					t_piece->setTurnInfo(false, false, false, false);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_buy, piece_type);
					
//					addShadow(piece_type, piece_position, stage_number);
				}
				else // lock
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::lockPieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_strokePiece, stage_number);
					t_piece->setTurnInfo(false, false, false, false);
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
	
	if(is_puzzle_clear && !mySGD->getPuzzleHistory(puzzle_number).is_clear)
	{
		clear_is_first_puzzle_success = true;
		
		PuzzleHistory t_history = mySGD->getPuzzleHistory(puzzle_number);
		
		if(clear_is_first_perfect)
			t_history.is_perfect = true;
		
		t_history.is_clear = true;
		mySGD->setPuzzleHistory(t_history, nullptr);
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
	int total_card_cnt = stage_count*4;
	
	int card_take_cnt = mySGD->getHasGottenCardsSize();
	for(int i=0;i<card_take_cnt;i++)
	{
		int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
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
		int recent_get_card_cnt = mySGD->getHasGottenCardsSize();
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
		int recent_get_card_cnt = mySGD->getHasGottenCardsSize();
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
		
		CCLabelTTF* title_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_openStage), mySGD->getFont().c_str(), 20);
		title_label->setPosition(ccp(0, 102));
		t_container->addChild(title_label);
		
		CCLabelTTF* content_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_openGoldValue), NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, t_stage_number))->getCString(), mySGD->getFont().c_str(), 18);
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
//				myDSH->setIntegerForKey(kDSH_Key_openStageCnt, myDSH->getIntegerForKey(kDSH_Key_openStageCnt)+1);
//				myDSH->setIntegerForKey(kDSH_Key_openStageNumber_int1, myDSH->getIntegerForKey(kDSH_Key_openStageCnt), t_stage_number);
//				myDSH->setBoolForKey(kDSH_Key_isOpenStage_int1, t_stage_number, true);
				
				PieceHistory t_history = mySGD->getPieceHistory(t_stage_number);
				t_history.is_open = true;
				mySGD->setPieceHistory(t_history, nullptr);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_gold);
//				save_userdata_list.push_back(kSaveUserData_Key_openStage);
				
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
				
				CCLabelTTF* open_puzzle_title_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_stageOpenTitle), mySGD->getFont().c_str(), 20);
				open_puzzle_title_label->setPosition(ccp(0, 102));
				open_puzzle_container->addChild(open_puzzle_title_label);
				
				CCLabelTTF* open_puzzle_content_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_stageOpenContent), mySGD->getFont().c_str(), 18);
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
		int recent_get_card_cnt = mySGD->getHasGottenCardsSize();
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
			openSettingPopup();
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
			
			stopAutoTurnPiece();
			addChild(KSTimer::create(5.f, [=](){startAutoTurnPiece();}));
//			is_auto_turn = false;
//			auto_turn_piece_frame = 0;
			
			is_menu_enable = true;
		}
	}
}

void PuzzleScene::startAutoTurnPiece()
{
	is_auto_turn = false;
	auto_turn_piece_frame = 0;
	schedule(schedule_selector(PuzzleScene::autoTurnPiece), 1.f/5.f);
}
void PuzzleScene::autoTurnPiece()
{
	if(!is_menu_enable)
		return;
	
	
	int start_stage = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
	
	PuzzlePiece* t_piece = (PuzzlePiece*)puzzle_node->getChildByTag(start_stage+auto_turn_piece_frame);
	if(t_piece->is_simple)
		t_piece->turnPiece(piece_mode);
	else
		t_piece->simpleView();
	
	auto_turn_piece_frame++;
	
	if(auto_turn_piece_frame == stage_count)
	{
		stopAutoTurnPiece();
		addChild(KSTimer::create(5.f, [=](){startAutoTurnPiece();}));
	}
	
//	if(is_auto_turn)
//	{
//		if(auto_turn_piece_frame >= 60*3)
//		{
//			is_auto_turn = false;
//			auto_turn_piece_frame = 0;
//			
//			int start_stage = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
//			int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
//
//			for(int i=start_stage;i<start_stage+stage_count;i++)
//				((PuzzlePiece*)puzzle_node->getChildByTag(i))->turnPiece(piece_mode);
//		}
//	}
//	else
//	{
//		if(auto_turn_piece_frame >= 60*5)
//		{
//			is_auto_turn = true;
//			auto_turn_piece_frame = 0;
//			
//			int start_stage = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
//			int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
//			
//			for(int i=start_stage;i<start_stage+stage_count;i++)
//			{
//				PuzzlePiece* new_piece = (PuzzlePiece*)puzzle_node->getChildByTag(i);
//				new_piece->simpleView();
//			}
//		}
//	}
	
}
void PuzzleScene::stopAutoTurnPiece()
{
	unschedule(schedule_selector(PuzzleScene::autoTurnPiece));
}

void PuzzleScene::openSettingPopup()
{
	is_menu_enable = false;
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
	
	mySD->setSilType(selected_stage_number);
	
	StartSettingPopup* t_popup = StartSettingPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
	addChild(t_popup, kPuzzleZorder_popup);
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
	if(!right_case)
	{
		right_case = CCNode::create();
		right_case->setPosition(ccp(480,puzzle_node->getPositionY()));
		addChild(right_case, kPuzzleZorder_right);
	}
	
	if(right_body)
	{
		right_body->removeFromParent();
		right_body = NULL;
	}
	
	right_body = CCScale9Sprite::create("puzzle_right_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	right_body->setContentSize(CCSizeMake(130, 280));
//	CCSprite* right_body = CCSprite::create("puzzle_right_body.png");
	right_body->setPosition(ccp(-right_body->getContentSize().width/2.f-6, -1));
	right_case->addChild(right_body);
	
	if(right_mode == kPuzzleRightMode_stage)
	{
		PieceHistory t_history = mySGD->getPieceHistory(selected_stage_number);
		
		bool is_have_card_list[4] = {false,};
		
		int stage_card_count = NSDS_GI(selected_stage_number, kSDS_SI_cardCount_i);
		for(int i=1;i<=stage_card_count && i <= 4;i++)
		{
			int step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, i);
			is_have_card_list[i-1] = t_history.is_clear[i-1];
			
			CCPoint step_position = ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height-58-(i-1)*46.5f);
			
			if(is_have_card_list[i-1])
			{
				CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("puzzle_right_sumcrop.png"));
				t_clipping->setPosition(step_position);
				right_body->addChild(t_clipping);
				
				t_clipping->setAlphaThreshold(0.1f);
				
				CCSprite* t_inner = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", step_card_number)->getCString()));
				t_inner->setScale(0.4f);
				
				t_inner->setPositionY(rand()%120-60);
				
				t_clipping->addChild(t_inner);
				
				int card_rank = NSDS_GI(kSDS_CI_int1_rank_i, step_card_number);
				for(int j=0;j<card_rank;j++)
				{
					CCSprite* t_star = CCSprite::create("puzzle_right_staron.png");
					t_star->setPosition(ccpAdd(step_position, ccp(-43.5f+j*13.5f,10)));
					right_body->addChild(t_star);
				}
				
				CommonButton* show_img = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_view), 12, CCSizeMake(40, 40), CommonButtonYellow, kCCMenuHandlerPriority);
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
			else
			{
				CCSprite* t_back = CCSprite::create("puzzle_right_lock.png");
				t_back->setPosition(step_position);
				right_body->addChild(t_back);
				
				string condition_string;
				if(i == 1)
					condition_string = myLoc->getLocalForKey(kMyLocalKey_condition1);
				else if(i == 2)
					condition_string = myLoc->getLocalForKey(kMyLocalKey_condition2);
				else if(i == 3)
					condition_string = myLoc->getLocalForKey(kMyLocalKey_condition3);
				else
					condition_string = myLoc->getLocalForKey(kMyLocalKey_condition4);
				
				KSLabelTTF* condition_label = KSLabelTTF::create(condition_string.c_str(), mySGD->getFont().c_str(), 10);
				condition_label->setAnchorPoint(ccp(0.f,0.5f));
				condition_label->setPosition(ccp(8,15));
				t_back->addChild(condition_label);
				
				int card_rank = NSDS_GI(kSDS_CI_int1_rank_i, step_card_number);
				for(int j=0;j<card_rank;j++)
				{
					CCSprite* t_star = CCSprite::create("puzzle_right_staroff.png");
					t_star->setPosition(ccpAdd(step_position, ccp(-43.5f+j*13.5f,10)));
					right_body->addChild(t_star);
				}
			}
		}
	}
	else if(right_mode == kPuzzleRightMode_ranking)
	{
		if(saved_ranking_stage_number == -1 || saved_ranking_stage_number != selected_stage_number)
		{
			if(!loading_progress_img)
			{
				CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
				CCBReader* reader = new CCBReader(nodeLoader);
				loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
				loading_progress_img->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height-58-70));
				right_body->addChild(loading_progress_img);
				reader->release();
			}
			
			Json::Value param;
			param["memberID"] = hspConnector::get()->getSocialID();
			param["stageNo"] = selected_stage_number;
			hspConnector::get()->command("getstagerankbyalluser", param, json_selector(this, PuzzleScene::resultGetRank));
		}
		else
		{
			resultGetRank(saved_ranking_data);
		}
	}
	
	if(ready_menu)
	{
		ready_menu->removeFromParent();
		ready_menu = NULL;
	}
	
	CCSprite* n_ready = CCSprite::create("puzzle_right_ready.png");
	CCLabelTTF* n_stage = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_stageValue), selected_stage_number)->getCString(),
												 mySGD->getFont().c_str(), 12);
	n_stage->setColor(ccBLACK);
	n_stage->setPosition(ccp(n_ready->getContentSize().width/2.f,n_ready->getContentSize().height/2.f+13));
	n_ready->addChild(n_stage);
	
	KSLabelTTF* n_ready_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ready), mySGD->getFont().c_str(), 20);
	n_ready_label->setColor(ccc3(47, 30, 6));
	n_ready_label->enableOuterStroke(ccc3(47, 30, 6), 0.25f);
	n_ready_label->setPosition(ccp(n_ready->getContentSize().width/2.f, n_ready->getContentSize().height*0.4f));
	n_ready->addChild(n_ready_label);
	
	CCSprite* s_ready = CCSprite::create("puzzle_right_ready.png");
	s_ready->setColor(ccGRAY);
	CCLabelTTF* s_stage = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_stageValue), selected_stage_number)->getCString(),
												 mySGD->getFont().c_str(), 12);
	s_stage->setColor(ccBLACK);
	s_stage->setPosition(ccp(s_ready->getContentSize().width/2.f,s_ready->getContentSize().height/2.f+13));
	s_ready->addChild(s_stage);
	
	KSLabelTTF* s_ready_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ready), mySGD->getFont().c_str(), 20);
	s_ready_label->setColor(ccc3(47, 30, 6));
	s_ready_label->enableOuterStroke(ccc3(47, 30, 6), 0.25f);
	s_ready_label->setPosition(ccp(s_ready->getContentSize().width/2.f, s_ready->getContentSize().height*0.4f));
	s_ready->addChild(s_ready_label);
	
	CCMenuItem* ready_item = CCMenuItemSprite::create(n_ready, s_ready, this, menu_selector(PuzzleScene::menuAction));
	ready_item->setTag(kPuzzleMenuTag_start);
	
	ready_menu = CCMenu::createWithItem(ready_item);
	ready_menu->setPosition(ccp(-65-6,-puzzle_node->getPositionY()+28+8));
	right_case->addChild(ready_menu);
	
	setRightTopButton();
}

void PuzzleScene::resultGetRank(Json::Value result_data)
{
	if(loading_progress_img)
	{
		loading_progress_img->removeFromParent();
		loading_progress_img = NULL;
	}
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		saved_ranking_data = result_data;
		saved_ranking_stage_number = result_data["stageNo"].asInt();
		Json::Value user_list = result_data["list"];
		
		CCSprite* graph_back = CCSprite::create("puzzle_rank_graph.png");
		graph_back->setPosition(ccp(right_body->getContentSize().width/2.f,220));
		right_body->addChild(graph_back);
		
		KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
		t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		t_rank_a->setPosition(ccp(16,8));
		graph_back->addChild(t_rank_a);
		
		KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
		t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		t_rank_b->setPosition(ccp(16+28,8));
		graph_back->addChild(t_rank_b);
		
		KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
		t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		t_rank_c->setPosition(ccp(17+56,8));
		graph_back->addChild(t_rank_c);
		
		KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
		t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
		t_rank_d->setPosition(ccp(17+84,8));
		graph_back->addChild(t_rank_d);
		
		int alluser = result_data["alluser"].asInt();
		int myrank = result_data["myrank"].asInt();
		
		CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", alluser)->getCString(), mySGD->getFont().c_str(), 10);
		all_user_label->setColor(ccc3(255, 50, 50));
		all_user_label->setAnchorPoint(ccp(1,0.5));
		all_user_label->setPosition(ccp(right_body->getContentSize().width-10, 204));
		right_body->addChild(all_user_label);
		
		CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_myrankValue), myrank)->getCString(), mySGD->getFont().c_str(), 10);
		my_rank_label->setAnchorPoint(ccp(1,0.5));
		my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
		right_body->addChild(my_rank_label);
		
		float rank_percent = 1.f*myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("puzzle_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,0));
		rank_percent_case->setPosition(ccp(10+115,220));
		right_body->addChild(rank_percent_case);
		
		KSLabelTTF* percent_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", rank_percent*100.f)->getCString(), mySGD->getFont().c_str(), 11);
		percent_label->setColor(ccc3(255, 170, 20));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 1);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f+2));
		rank_percent_case->addChild(percent_label);
		
		CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(10 + 115.f*rank_percent,220));
		rank_percent_case->runAction(t_move);
		
//		delay_index = 0;
		int limit_count = 3;
		for(int i=0;i<user_list.size() && i<limit_count;i++)
		{
			string case_name;
			if(myrank == i+1)
			{
				case_name = "mainpopup_pupple1.png";
				limit_count++;
			}
			else
			{
				case_name = "rank_normal.png";
			}
			
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			list_cell_case->setContentSize(CCSizeMake(125, 40));
			list_cell_case->setPosition(ccp(right_body->getContentSize().width/2.f,180-i*34.5f));
			right_body->addChild(list_cell_case);
			
			CCPoint rank_position = ccp(20,20);
			if(i == 0)
			{
				CCSprite* gold_medal = CCSprite::create("rank_gold.png");
				gold_medal->setPosition(rank_position);
				list_cell_case->addChild(gold_medal);
			}
			else if(i == 1)
			{
				CCSprite* silver_medal = CCSprite::create("rank_silver.png");
				silver_medal->setPosition(rank_position);
				list_cell_case->addChild(silver_medal);
			}
			else if(i == 2)
			{
				CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
				bronze_medal->setPosition(rank_position);
				list_cell_case->addChild(bronze_medal);
			}
			else
			{
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 12);
				rank_label->enableOuterStroke(ccBLACK, 1);
				rank_label->setPosition(rank_position);
				list_cell_case->addChild(rank_label);
			}
			
			Json::Reader reader;
			Json::Value read_data;
			reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
			
			KSLabelTTF* nick_label = KSLabelTTF::create(read_data.get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12); // user_list[i]["nick"].asString().c_str()
			nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
			nick_label->setPosition(ccp(78,28));
			list_cell_case->addChild(nick_label);
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",user_list[i]["score"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setColor(ccc3(255, 170, 20));
			score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
			score_label->setPosition(ccp(78,12));
			list_cell_case->addChild(score_label);
			
//			CCPoint original_position = list_cell_case->getPosition();
//			list_cell_case->setPosition(ccpAdd(original_position, ccp(0, -500)));
//			
//			cell_action_list.push_back([=](){
//				CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
//				CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
//				CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
//				list_cell_case->runAction(t_seq);
//				delay_index++;
//			});
		}
		
		if(myrank > 3)
		{
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			list_cell_case->setContentSize(CCSizeMake(125, 40));
			list_cell_case->setPosition(ccp(right_body->getContentSize().width/2.f,180-3*34.5f));
			right_body->addChild(list_cell_case);
			
			KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", myrank)->getCString(), mySGD->getFont().c_str(), 12);
			rank_label->enableOuterStroke(ccBLACK, 1);
			rank_label->setPosition(ccp(20,20));
			list_cell_case->addChild(rank_label);
			
			KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12);
			nick_label->enableOuterStroke(ccc3(50, 25, 0), 1);
			nick_label->setPosition(ccp(78,28));
			list_cell_case->addChild(nick_label);
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",result_data["myscore"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setColor(ccc3(255, 170, 20));
			score_label->enableOuterStroke(ccc3(50, 25, 0), 1.f);
			score_label->setPosition(ccp(78,12));
			list_cell_case->addChild(score_label);
			
//			CCPoint original_position = list_cell_case->getPosition();
//			list_cell_case->setPosition(ccpAdd(original_position, ccp(0, -500)));
//			
//			cell_action_list.push_back([=](){
//				CCDelayTime* t_delay = CCDelayTime::create(delay_index*0.2f);
//				CCMoveTo* t_move = CCMoveTo::create(0.4f, original_position);
//				CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
//				list_cell_case->runAction(t_seq);
//			});
		}
	}
	else
	{
		CCLabelTTF* fail_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
		fail_label->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height-58-70));
		right_body->addChild(fail_label);
	}
}

void PuzzleScene::setRightTopButton()
{
	if(!stage_button)
	{
		stage_button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_stage), 12, CCSizeMake(66,35), CommonButtonYellowDown, kCCMenuHandlerPriority);
		stage_button->setPosition(ccp(-65-6-29, 118.5f));
		right_case->addChild(stage_button, 5);
		stage_button->setBackgroundTypeForDisabled(CommonButtonYellowUp);
		stage_button->setFunction([=](CCObject* sender)
								  {
									  if(!is_menu_enable)
										  return;
									  right_mode = kPuzzleRightMode_stage;
									  setRight();
								  });
	}
	if(!ranking_button)
	{
		ranking_button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ranking), 12, CCSizeMake(66,35), CommonButtonYellowDown, kCCMenuHandlerPriority);
		ranking_button->setPosition(ccp(-65-6+29, 118.5f));
		right_case->addChild(ranking_button, 5);
		ranking_button->setBackgroundTypeForDisabled(CommonButtonYellowUp);
		ranking_button->setFunction([=](CCObject* sender)
								  {
									  if(!is_menu_enable)
										  return;
									  right_mode = kPuzzleRightMode_ranking;
									  setRight();
								  });
	}
	
	if(right_mode == kPuzzleRightMode_stage)
	{
		stage_button->setEnabled(false);
		ranking_button->setEnabled(true);
		
		right_case->reorderChild(stage_button, 6);
		right_case->reorderChild(ranking_button, 5);
	}
	else
	{
		ranking_button->setEnabled(false);
		stage_button->setEnabled(true);
		
		right_case->reorderChild(stage_button, 5);
		right_case->reorderChild(ranking_button, 6);
	}
}

void PuzzleScene::topOpenning()
{
	CCPoint original_position = top_list[0]->getPosition();
	top_list[0]->setPosition(original_position + ccp(-150,0));
	top_list[0]->runAction(CCMoveTo::create(0.5f, original_position));
}

void PuzzleScene::topReopenning()
{
	for(int i=1;i<top_list.size();i++)
	{
		CCPoint original_position = top_list[i]->getPosition();
		top_list[i]->setPositionY(original_position.y+100);
		
		CCDelayTime* t_delay = CCDelayTime::create(i*0.1f);
		CCMoveTo* t_move = CCMoveTo::create(0.2f, original_position);
		CCSequence* t_seq = CCSequence::create(t_delay, t_move, NULL);
		
		top_list[i]->runAction(t_seq);
	}
}

void PuzzleScene::topBacking()
{
	top_list[0]->runAction(CCMoveTo::create(0.5f, ccpAdd(top_list[0]->getPosition(), ccp(-150,0)))); // cancel
//	top_list[1]->runAction(CCMoveTo::create(0.5f, ccp(78,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3))); // top_heart
//	top_list[2]->runAction(CCMoveTo::create(0.5f, ccp(216,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3))); // top_gold
//	top_list[3]->runAction(CCMoveTo::create(0.5f, ccp(325,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3))); // top_ruby
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
	top_heart->setPosition(ccp(78+41,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
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
	top_gold->setPosition(ccp(216+27,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
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
	top_ruby->setPosition(ccp(325+12,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
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
	
	top_list.push_back(postbox_menu);
	
	postbox_count_case = CCSprite::create("mainflow_new.png");//"mainflow_postbox_count.png");
	postbox_count_case->setPosition(ccp(406,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-9));
	addChild(postbox_count_case, kPuzzleZorder_top);
	postbox_count_case->setVisible(false);
	
//	postbox_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
//	postbox_count_label->setColor(ccc3(95, 60, 30));
//	postbox_count_label->setPosition(ccp(postbox_count_case->getContentSize().width/2.f-0.5f, postbox_count_case->getContentSize().height/2.f+0.5f));
//	postbox_count_case->addChild(postbox_count_label);
	
	countingMessage();
	
	
	CCSprite* n_achieve = CCSprite::create("mainflow_new_achievement.png");
	CCSprite* s_achieve = CCSprite::create("mainflow_new_achievement.png");
	s_achieve->setColor(ccGRAY);
	
	CCMenuItem* achieve_item = CCMenuItemSprite::create(n_achieve, s_achieve, this, menu_selector(PuzzleScene::menuAction));
	achieve_item->setTag(kPuzzleMenuTag_achieve);
	
	CCMenu* achieve_menu = CCMenu::createWithItem(achieve_item);
	achieve_menu->setPosition(ccp(427,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(achieve_menu, kPuzzleZorder_top);
	
	top_list.push_back(achieve_menu);
	
	CCSprite* n_option = CCSprite::create("mainflow_new_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_new_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(PuzzleScene::menuAction));
	option_item->setTag(kPuzzleMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(460,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-15));
	addChild(option_menu, kPuzzleZorder_top);
	
	top_list.push_back(option_menu);
}

void PuzzleScene::countingMessage()
{
	Json::Value p;
	p["memberID"]=hspConnector::get()->getSocialID();
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
										 
//										 if(message_list.size() < 10)
//										 {
//											 postbox_count_label->setFontSize(10);
//											 postbox_count_label->setString(CCString::createWithFormat("%d", message_list.size())->getCString());
//										 }
//										 else if(message_list.size() < 100)
//										 {
//											 postbox_count_label->setFontSize(7);
//											 postbox_count_label->setString(CCString::createWithFormat("%d", message_list.size())->getCString());
//										 }
//										 else
//										 {
//											 postbox_count_label->setFontSize(8);
//											 postbox_count_label->setString("...");
//										 }
									 }
									 else
									 {
										 postbox_count_case->setVisible(false);
//										 postbox_count_label->setString("0");
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