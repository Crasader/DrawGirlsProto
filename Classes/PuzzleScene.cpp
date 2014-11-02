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
#include "FlagSelector.h"
#include "GoodsLight.h"
#include "CardViewScene.h"
#include "CardLockedPopup.h"
#include "BuyPiecePopup.h"
#include "GaBaBo.h"
#include "CurtainNodeForBonusGame.h"
#include "AchieveNoti.h"
#include "JsGababo.h"
#include "TypingBox.h"
#include "LabelTTFMarquee.h"
#include "FiveRocksCpp.h"
#include "RealTimeMessage.h"

CCScene* PuzzleScene::scene()
{
    CCScene *scene = CCScene::create();
    PuzzleScene *layer = PuzzleScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer, 0, 1);
	
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
	kPuzzleMenuTag_event,
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
	
	mySGD->ui_scene_code = kUISceneCode_puzzle;
	
//	setBackKeyFunc([=](){
//		startBacking();
//	});
//	setBackKeyEnabled(true);
	
	setKeypadEnabled(true);
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//	CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames();
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
//	myDSH->unregiAllCcbAnimationManagers();
	
	call_rank_stage_number = -1;
	
	mySGD->is_endless_mode = false;
	mySGD->resetReplayPlayingInfo();
	
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
		if(myDSH->getIntegerForKey(kDSH_Key_showedScenario) == 1 && puzzle_number == 1)
		{
			myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, 1);
		}
		else
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
					
					myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, can_enter_stage_list[minimum_index]);
				}
				else // 못 깬 스테이지 중에 입장가능한 스테이지가 없다
				{
					int selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number);
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
						
						myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, cleared_stage_list[minimum_index]);
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
					
					myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, perfect_stage_list[minimum_index]);
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
					
					myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, not_perfect_stage_list[minimum_index]);
				}
			}
		}
	}
	else if(before_scene_name == "fail")
	{
		int selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number);
		if(selected_stage_number <= 0 || selected_stage_number < start_stage || selected_stage_number >= start_stage + stage_count) // 마지막 플레이 스테이지 기록이 없거나, 범위 밖에 있다
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
					
					myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, can_enter_stage_list[minimum_index]);
				}
				else // 못 깬 스테이지 중에 입장가능한 스테이지가 없다
				{
					int selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number);
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
						
						myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, cleared_stage_list[minimum_index]);
					}
				}
			}
			else
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
	}
	else if(before_scene_name == "clear")
	{
		int selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number);
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
	
	is_success_command = true;
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
		int take_level, before_take_level;
		take_level = mySGD->getStageGrade();
		before_take_level = mySGD->getStageGrade();//mySGD->getBeforeRankUpStageGrade();
		
//		if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 4;
//		else if(mySGD->is_showtime)							take_level = 3;
//		else if(mySGD->is_exchanged)						take_level = 2;
//		else												take_level = 1;
		
//		if(mySGD->isTimeEvent(kTimeEventType_card))
//		{
//			take_level += mySGD->getTimeEventIntValue(kTimeEventType_card);
//			if(take_level > 4)
//				take_level = 4;
//		}
		
		bool is_not_empty_card[4] = {false,};
		
		clear_star_take_level = before_take_level;
		
		clear_is_empty_piece = true;
		int played_stage_number = mySD->getSilType();
		int stage_card_count = 4;//NSDS_GI(played_stage_number, kSDS_SI_cardCount_i);
		for(int i=1;i<=4;i++)
		{
			if(mySGD->getPieceHistory(played_stage_number).is_clear[i-1].getV())
			{
				clear_is_empty_piece = false;
				is_not_empty_card[i-1] = true;
			}
		}
		
		clear_is_empty_star = !is_not_empty_card[before_take_level-1];
		
		if(!mySGD->getPieceHistory(played_stage_number).is_clear[before_take_level-1].getV())
		{
//			mySGD->setClearRewardGold(NSDS_GI(kSDS_CI_int1_reward_i, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			clear_is_perfect_piece = true;
		}
		else
		{
			clear_is_perfect_piece = false;
		}
		
		mySGD->addHasGottenCardNumber(NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
		
		keep_card_number = 0;
		
		for(int i=1;i<=4;i++)
		{
			if(i != before_take_level && !mySGD->getPieceHistory(played_stage_number).is_clear[i-1].getV())
			{
				clear_is_perfect_piece = false;
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
		
		keep_card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level);
		
		LoadingLayer* t_loading = LoadingLayer::create(-900);
		addChild(t_loading, kPuzzleZorder_popup+1);
		
		is_success_command = false;
		clear_command_list.clear();
		
		
		Json::Value transaction_param;
		transaction_param["memberID"] = hspConnector::get()->getMemberID();
		clear_command_list.push_back(CommandParam("starttransaction", transaction_param, [=](Json::Value result_data)
												  {
													  TRACE();
													  if(result_data["result"]["code"].asInt() == GDSUCCESS)
													  {
														  TRACE();
														  mySGD->network_check_cnt = 0;
														  
														  t_loading->removeFromParent();
														  is_success_command = true;
														  endReady();
													  }
													  else
													  {
														  TRACE();
														  mySGD->network_check_cnt++;
														  
														  if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
														  {
															  mySGD->network_check_cnt = 0;
															  
															  ASPopupView* alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4), [=](){
																  myHSP->command(this->clear_command_list, -1);
															  }, 1);
															  
//															  ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
//																  myHSP->command(this->clear_command_list);
//															  });
															  if(alert)
																  ((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
														  }
														  else
														  {
															  addChild(KSTimer::create(0.5f, [=]()
																					   {
																						   myHSP->command(this->clear_command_list, -1);
																					   }));
														  }
													  }
												  }));
		
		if(!mySGD->isTimeEvent(kTimeEventType_heart))
		{
			mySGD->addChangeGoods("clearHeartUp", kGoodsType_begin, 0, "", ccsf("%d", mySGD->getGameUseTime()), "클리어하트충전");
		}
		
		Json::Value card_param;
		card_param["memberID"] = hspConnector::get()->getSocialID();
		card_param["cardNo"] = keep_card_number;
		card_param["addCount"] = mySGD->getClearTakeCardCnt();
		
		clear_command_list.push_back(CommandParam("updateCardHistory", card_param, [=](Json::Value result_data)
												  {
													  TRACE();
													  if(result_data["result"]["code"].asInt() == GDSUCCESS)
														{
															TRACE();
															for(int i=kAchievementCode_cardCollection1;i<=kAchievementCode_cardCollection3;i++)
															{
																if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
																   mySGD->getHasGottenCardsSize() >= myAchieve->getCondition((AchievementCode)i))
																{
																	myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
																	AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
																	CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
																}
															}
															
															AchievementCode i = kAchievementCode_cardSet;
															if(!myAchieve->isCompleted(i) && !myAchieve->isAchieve(i))
															{
																int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, keep_card_number);
																bool is_success = true;
																for(int i=1;i<=4 && is_success;i++)
																{
																	int t_card_number = NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, i);
																	if(!mySGD->isHasGottenCards(t_card_number))
																		is_success = false;
																}
																
																if(is_success)
																{
																	if(!myAchieve->isNoti(AchievementCode(i)))
																	{
																		myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
																		AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
																		CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
																	}
																	myAchieve->updateAchieve(nullptr);
																}
															}
															
                                                            int gottenCardGroup = mySGD->getHasGottenCardsSize()/10;
                                                            fiverocks::FiveRocksBridge::setUserCohortVariable(2, ccsf("[카드보유 %d~%d]",gottenCardGroup*10,gottenCardGroup*10+9));
														}
												  }));
		
		
		
		PieceHistory t_history = mySGD->getPieceHistory(mySD->getSilType());
		bool is_change_history = false;
		
		if(!mySGD->isClearPiece(mySD->getSilType()))
		{
			t_history.is_clear[before_take_level-1] = true;
			t_history.clear_count = t_history.try_count.getV();
			
			is_change_history = true;
		}
		else if(!t_history.is_clear[before_take_level-1].getV())
		{
			t_history.is_clear[before_take_level-1] = true;
			
			is_change_history = true;
		}
		
		if(is_change_history)
		{
			clear_command_list.push_back(mySGD->getUpdatePieceHistoryParam(t_history, [=](Json::Value result_data)
											  {
												  TRACE();
												  if(result_data["result"]["code"] == GDSUCCESS)
													{
														TRACE();
													}
												  TRACE();
											  }));
		}

		
		int t_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
		
		PuzzleHistory pz_history = mySGD->getPuzzleHistory(t_puzzle_number);
		
		clear_is_first_perfect = !pz_history.is_perfect.getV();
		clear_is_first_puzzle_success = !pz_history.is_clear.getV();
		
		int t_start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
		int t_stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
		
		for(int i=t_start_stage;i<t_start_stage+t_stage_count;i++)
		{
			int t_stage_number = i;
			
			if(NSDS_GS(t_stage_number, kSDS_SI_type_s) == "normal") // normal 이 아닌 타입의 스테이지는 퍼즐 완성이나 퍼펙트에 영향을 주지 않음
			{
				if(t_stage_number == 1 || mySGD->getPieceHistory(t_stage_number).is_open.getV() ||
				   (NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, t_stage_number) == 0 &&
					(NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, t_stage_number) == 0 || mySGD->isClearPiece(NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, t_stage_number))))) // 입장 가능인가
				{
					PieceHistory t_history = mySGD->getPieceHistory(t_stage_number);
					
					if(mySGD->isClearPiece(t_stage_number))
					{
						if(!t_history.is_clear[0].getV() || !t_history.is_clear[1].getV() || !t_history.is_clear[2].getV() || !t_history.is_clear[3].getV())
						{
							clear_is_first_perfect = false;
						}
					}
					else // empty
					{
						clear_is_first_puzzle_success = false;
						clear_is_first_perfect = false;
					}
				}
				else
				{
					clear_is_first_puzzle_success = false;
					clear_is_first_perfect = false;
				}
			}
		}
		
		if(clear_is_first_puzzle_success || clear_is_first_perfect)
		{
			PuzzleHistory t_history = mySGD->getPuzzleHistory(puzzle_number);
			
			if(clear_is_first_perfect)
				t_history.is_perfect = true;
			if(clear_is_first_puzzle_success)
				t_history.is_clear = true;
			
			clear_command_list.push_back(mySGD->getUpdatePuzzleHistoryParam(t_history, [=](Json::Value result_data)
																			{
																				TRACE();
																				GraphDogLib::JsonToLog("clear or perfect puzzle", result_data);
																				TRACE();
																				if(result_data["result"]["code"].asInt() == GDSUCCESS)
																				{
																					TRACE();
																					if(result_data["sendGift"].asBool())
																					{
																						TRACE();
																						mySGD->new_puzzle_card_info = result_data["giftData"];
																						mySGD->is_new_puzzle_card = true;
																						
																						int new_card_number = mySGD->new_puzzle_card_info["cardNo"].asInt();
																						
																						mySGD->addHasGottenCardNumber(new_card_number);
																						if(mySGD->new_puzzle_card_info["isMorphing"].asBool())
																							mySGD->onCardMorphing(mySGD->new_puzzle_card_info["cardNo"].asInt());
																					}
																				}
																				TRACE();
																			}));
		}
		
		TRACE();
		myHSP->command(clear_command_list, -1);
	}
	
	
	CCSprite* back_img = CCSprite::create("main_back.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kPuzzleZorder_back);
	
	piece_mode = kPieceMode_thumb;//(PieceMode)myDSH->getIntegerForKey(kDSH_Key_puzzleMode);
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
			myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
			openBeforeSettingPopup();
		}
		else
		{
			puzzleOpenning();
			rightOpenning();
			topOpenning();
			
			if(myDSH->getIntegerForKey(kDSH_Key_showedScenario) == 1)
			{
				myDSH->setIntegerForKey(kDSH_Key_showedScenario, 2);
				
				CCNode* scenario_node = CCNode::create();
				addChild(scenario_node, 9999);
				
				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
				if(screen_scale_x < 1.f)
					screen_scale_x = 1.f;
				
				float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
				
				
				CCNode* t_stencil_node = CCNode::create();
				
				
				CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
				t_clipping->setAlphaThreshold(0.1f);
				
				float change_scale = 1.f;
				CCPoint change_origin = ccp(0,0);
				if(screen_scale_x > 1.f)
				{
					change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
					change_scale = screen_scale_x;
				}
				if(screen_scale_y > 1.f)
					change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
				CCSize win_size = CCDirector::sharedDirector()->getWinSize();
				t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
				
				
				CCSprite* t_gray = CCSprite::create("back_gray.png");
				t_gray->setScaleX(screen_scale_x);
				t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
				t_gray->setOpacity(0);
				t_gray->setPosition(ccp(240,160));
				t_clipping->addChild(t_gray);
				
				t_clipping->setInverted(true);
				scenario_node->addChild(t_clipping, 0);
				
				
				CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
				ikaruga->setAnchorPoint(ccp(0,0));
				ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width, 160-160*screen_scale_y));
				scenario_node->addChild(ikaruga, 1);
				
				TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(241, 78));
				typing_box->setHide();
				scenario_node->addChild(typing_box, 2);
				
				CCSprite* n_skip = CCSprite::create("kt_skip.png");
				CCSprite* s_skip = CCSprite::create("kt_skip.png");
				s_skip->setColor(ccGRAY);
				
				CCMenuLambda* skip_menu = CCMenuLambda::create();
				skip_menu->setPosition(ccp(240-240*screen_scale_x + 35, 160+160*screen_scale_y - 25 + 150));
				scenario_node->addChild(skip_menu, 3);
				skip_menu->setTouchPriority(-19999);
				skip_menu->setEnabled(false);
				
				CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [=](CCObject* sender)
																			 {
																				 skip_menu->setEnabled(false);
																				 
																				 is_menu_enable = true;
																				 CCNode* t_node = CCNode::create();
																				 t_node->setTag(kPuzzleMenuTag_start);
																				 menuAction(t_node);
																				 
																				 addChild(KSTimer::create(0.1f, [=]()
																										  {
																											  scenario_node->removeFromParent();
																										  }));
																			 });
				skip_menu->addChild(skip_item);
				
				typing_box->showAnimation(0.3f);
				
				function<void()> end_func3 = [=]()
				{
					skip_menu->setEnabled(false);
					
					is_menu_enable = true;
					CCNode* t_node = CCNode::create();
					t_node->setTag(kPuzzleMenuTag_start);
					menuAction(t_node);
					
					addChild(KSTimer::create(0.1f, [=]()
											 {
												 scenario_node->removeFromParent();
											 }));
				};
				
				function<void()> end_func2 = [=]()
				{
					ikaruga->setVisible(true);
					
					typing_box->setVisible(true);
					typing_box->setTouchSuction(true);
					
					typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent14), end_func3);
				};
				
				function<void()> end_func1 = [=]()
				{
					skip_menu->setVisible(false);
					
					ikaruga->setVisible(false);
					
					typing_box->setTouchOffScrollAndButton();
					typing_box->setVisible(false);
					
					CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
					t_arrow1->setScale(0.6f);
					t_arrow1->setRotation(-25);
					t_arrow1->setPosition(ccp(160,223));
					t_clipping->addChild(t_arrow1);
					
					StyledLabelTTF* t_ment1 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_puzzleDimmed1), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kLeftAlignment);
					t_ment1->setAnchorPoint(ccp(0,0.5f));
					t_ment1->setPosition(t_arrow1->getPosition() + ccp(-t_arrow1->getContentSize().width/2.f*t_arrow1->getScale() - 3 + 55, 10));
					t_clipping->addChild(t_ment1);
					
					CCSprite* t_arrow2 = CCSprite::create("kt_arrow_big.png");
					t_arrow2->setScale(0.6f);
					t_arrow2->setRotation(90);
					t_arrow2->setPosition(ccp(230,110));
					t_clipping->addChild(t_arrow2);
					
					CCSprite* t_arrow3 = CCSprite::create("kt_arrow_big.png");
					t_arrow3->setScale(0.6f);
					t_arrow3->setRotation(-25);
					t_arrow3->setPosition(ccp(170,58));
					t_clipping->addChild(t_arrow3);
					
					StyledLabelTTF* t_ment2 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_puzzleDimmed2), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kLeftAlignment);
					t_ment2->setAnchorPoint(ccp(0.f,0.5f));
					t_ment2->setPosition(ccp(195, 75));
					t_clipping->addChild(t_ment2);
					
					CCSprite* t_arrow4 = CCSprite::create("kt_arrow_big.png");
					t_arrow4->setScale(0.6f);
					t_arrow4->setRotation(-90);
					t_arrow4->setPosition(ccp(434.5f,270));
					t_clipping->addChild(t_arrow4);
					
					StyledLabelTTF* t_ment3 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_puzzleDimmed3), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
					t_ment3->setAnchorPoint(ccp(1,0.5f));
					t_ment3->setPosition(t_arrow4->getPosition() + ccp(-t_arrow4->getContentSize().width/2.f*t_arrow4->getScale() - 3, 0));
					t_clipping->addChild(t_ment3);
					
					CCSprite* t_arrow5 = CCSprite::create("kt_arrow_big.png");
					t_arrow5->setScale(0.6f);
					t_arrow5->setRotation(180);
					t_arrow5->setPosition(ccp(315,32));
					t_clipping->addChild(t_arrow5);
					
					StyledLabelTTF* t_ment4 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_puzzleDimmed4), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kRightAlignment);
					t_ment4->setAnchorPoint(ccp(1,0.5f));
					t_ment4->setPosition(t_arrow5->getPosition() + ccp(-t_arrow5->getContentSize().width/2.f*t_arrow5->getScale() - 3, 0));
					t_clipping->addChild(t_ment4);
					
					
					CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
					t_stencil1->setContentSize(CCSizeMake(70, 70));
					t_stencil1->setPosition(ccp(111, 174));//ccp(291, 234));
					t_stencil_node->addChild(t_stencil1);
					
					CCScale9Sprite* t_stencil2 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
					t_stencil2->setContentSize(CCSizeMake(70, 70));
					t_stencil2->setPosition(ccp(231, 174));//ccp(111, 174));
					t_stencil_node->addChild(t_stencil2);
					
					CCScale9Sprite* t_stencil3 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
					t_stencil3->setContentSize(CCSizeMake(70, 70));
					t_stencil3->setPosition(ccp(111, 54));
					t_stencil_node->addChild(t_stencil3);
					
					CCScale9Sprite* t_stencil4 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
					t_stencil4->setContentSize(CCSizeMake(125, 186));
					t_stencil4->setPosition(ccp(409.5f, 153.5f));
					t_stencil_node->addChild(t_stencil4);
					
					CCScale9Sprite* t_stencil5 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
					t_stencil5->setContentSize(CCSizeMake(135, 50));
					t_stencil5->setPosition(ccp(409.5f, 32));
					t_stencil_node->addChild(t_stencil5);
					
					TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
					scenario_node->addChild(t_suction);
					t_suction->setTouchEnabled(true);
					t_suction->touch_began_func = [=]()
					{
						skip_menu->setVisible(true);
						t_suction->is_on_touch_began_func = false;
						t_stencil_node->removeAllChildren();
						t_arrow1->removeFromParent();
						t_arrow2->removeFromParent();
						t_arrow3->removeFromParent();
						t_arrow4->removeFromParent();
						t_arrow5->removeFromParent();
						t_ment1->removeFromParent();
						t_ment2->removeFromParent();
						t_ment3->removeFromParent();
						t_ment4->removeFromParent();
						end_func2();
						t_suction->removeFromParent();
					};
					t_suction->is_on_touch_began_func = true;
					
					typing_box->setTouchSuction(false);
				};
				
				scenario_node->addChild(KSTimer::create(0.3f, [=]()
														{
															scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																						  {
																							  t_gray->setOpacity(t*255);
																							  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																							  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
																						  }, [=](float t)
																						  {
																							  t_gray->setOpacity(255);
																							  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																							  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
																							  skip_menu->setEnabled(true);
																							  
																							  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent13), end_func1);
																						  }));
														}));
			}
		}
		
		if(mySGD->is_before_stage_img_download)
		{
			mySGD->is_before_stage_img_download = false;
			topReopenning();
		}
		else
		{
			CCSprite* title_name = CCSprite::create("temp_title_name.png");
			title_name->setPosition(ccp(240,160));
			title_name->setOpacity(255);
			addChild(title_name, kPuzzleZorder_back);
			
			CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
			CCCallFunc* t_call = CCCallFunc::create(title_name, callfunc_selector(CCSprite::removeFromParent));
			CCSequence* t_seq = CCSequence::create(t_fade, t_call, NULL);
			title_name->runAction(t_seq);
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
		
		addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
	}
	
	return true;
}

void PuzzleScene::endReady()
{
	if(is_clear_close && is_success_command)
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
				{
					addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
					is_menu_enable = true;
				}
			}
		}
	}
}

void PuzzleScene::updateCardHistory(CCNode *t_loading)
{
	TRACE();
	Json::Value param;
	param["memberID"] = hspConnector::get()->getSocialID();
	param["cardNo"] = keep_card_number;
	param["addCount"] = mySGD->getClearTakeCardCnt();
	
	hspConnector::get()->command("updateCardHistory", param, [=](Json::Value result_data)
								 {
									 TRACE();
									 if(result_data["result"]["code"].asInt() == GDSUCCESS)
									 {
										 TRACE();
										 mySGD->network_check_cnt = 0;
										 
										 t_loading->removeFromParent();
										 for(int i=kAchievementCode_cardCollection1;i<=kAchievementCode_cardCollection3;i++)
										 {
											 if(!myAchieve->isNoti(AchievementCode(i)) && !myAchieve->isCompleted((AchievementCode)i) &&
												mySGD->getHasGottenCardsSize() >= myAchieve->getCondition((AchievementCode)i))
											 {
												 myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
												 AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
												 CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
											 }
										 }
										 
										 AchievementCode i = kAchievementCode_cardSet;
										 if(!myAchieve->isCompleted(i) && !myAchieve->isAchieve(i))
										 {
											 int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, keep_card_number);
											 bool is_success = true;
											 for(int i=1;i<=4 && is_success;i++)
											 {
												 int t_card_number = NSDS_GI(card_stage, kSDS_SI_level_int1_card_i, i);
												 if(!mySGD->isHasGottenCards(t_card_number))
													 is_success = false;
											 }
											 
											 if(is_success)
											 {
												 if(!myAchieve->isNoti(AchievementCode(i)))
												 {
													 myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
													 AchieveNoti* t_noti = AchieveNoti::create((AchievementCode)i);
													 CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
												 }
												 myAchieve->updateAchieve(nullptr);
											 }
										 }
										 
									 }
									 else
									 {
										 TRACE();
										 mySGD->network_check_cnt++;
										 
										 if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
										 {
											 mySGD->network_check_cnt = 0;
											 
											 ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999, myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4), [=](){
												 updateCardHistory(t_loading);
											 }, 1);
											 if(alert)
												 ((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
										 }
										 else
										 {
											 addChild(KSTimer::create(0.5f, [=]()
																	  {
																		  updateCardHistory(t_loading);
																	  }));
										 }
									 }
									 TRACE();
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
	
	is_clear_close = false;
	
	ClearPopup* t_popup = ClearPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::hideClearPopup));
	t_popup->replay_func = [=](){openSettingPopup();};
	t_popup->goToMainFlow_func = [=](){is_menu_enable = false; startBacking();};
	t_popup->is_take_star_effect = clear_is_empty_star;
	t_popup->is_not_replay = clear_is_stage_unlock | clear_is_first_puzzle_success | clear_is_first_perfect | clear_is_perfect_piece;
	t_popup->refresh_achieve_func = bind(&PuzzleScene::countingAchievement, this);
	if(clear_is_stage_unlock)
		t_popup->onMainButton();
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
	is_clear_close = true;
	endReady();
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
	CCLOG("get piece animation");
	
	AudioEngine::sharedInstance()->playEffect("se_pieceget.mp3", false);
	
	CCSprite* get_piece_title = CCSprite::create(CCString::createWithFormat("get_piece_title_%s.png", myLoc->getSupportLocalCode())->getCString());
	PuzzlePiece* new_piece = (PuzzlePiece*)puzzle_node->getChildByTag(mySD->getSilType());
	get_piece_title->setPosition(puzzle_node->getPosition() + ccpAdd(new_piece->getPosition(), ccp(0, 45)));
	addChild(get_piece_title, kPuzzleZorder_top);
	
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
	CCLOG("get star animation : %d", mySD->getSilType());
	PuzzlePiece* new_piece = (PuzzlePiece*)puzzle_node->getChildByTag(mySD->getSilType());
	new_piece->startGetStarAnimation(clear_star_take_level, this, callfunc_selector(PuzzleScene::endGetStar));
}

void PuzzleScene::endGetStar()
{
	if(clear_is_perfect_piece)
	{
		CurtainNodeForBonusGame* bonusGame = CurtainNodeForBonusGame::create(kBonusGameCode_gababo, (int)Curtain::kTouchPriority, [=](){
			//		if(m_gameCode == kMiniGameCode_gababo)
			{
				Json::Value reward_info = mySGD->getAllClearReward();
				
				BonusGameReward gr1;
				gr1.spriteName = "morphing_heart3.png";
				gr1.desc = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_gababoReward), reward_info[0]["reward"][0]["count"].asInt());
				
				BonusGameReward gr2;
				gr2.spriteName = "morphing_heart3.png";
				gr2.desc = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_gababoReward), reward_info[1]["reward"][0]["count"].asInt());
				BonusGameReward gr3;
				gr3.spriteName = "morphing_heart3.png";
				gr3.desc = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_gababoReward), reward_info[2]["reward"][0]["count"].asInt());
				BonusGameReward gr4;
				gr4.spriteName = "morphing_heart3.png";
				gr4.desc = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_gababoReward), reward_info[3]["reward"][0]["count"].asInt());
//				GaBaBo* gbb = GaBaBo::create(-500, {gr1, gr2, gr3,gr4}, [=](int t_i)
//											 {
//												 if(clear_is_first_puzzle_success)
//												 {
//													 showSuccessPuzzleEffect();
//												 }
//												 else
//												 {
//													 if(clear_is_first_perfect)
//													 {
//														 showPerfectPuzzleEffect();
//													 }
//													 else
//													 {
//														 if(clear_is_stage_unlock)
//														 {
//															 showUnlockEffect();
//														 }
//														 else
//														 {
//															 addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
//															 is_menu_enable = true;
//														 }
//													 }
//												 }
//											 });
//				addChild(gbb, (int)Curtain::kBonusGame);
				JsGababo* gbb = JsGababo::create(-500, {gr1, gr2, gr3, gr4}, [=](int t_i)
												 {
													 mySGD->addChangeGoods(reward_info[t_i]["exchangeID"].asString());
													 
													 LoadingLayer* t_loading = LoadingLayer::create(-600);
													 addChild(t_loading, 1000);
													 
													 tryGababoReward(t_loading, [=]()
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
																 {
																	 addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
																	 is_menu_enable = true;
																 }
															 }
														 }
													 });
												 });
				addChild(gbb, (int)Curtain::kBonusGame);
			}
		});
		addChild(bonusGame, (int)Curtain::kCurtain);
	}
	else
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
				{
					addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
					is_menu_enable = true;
				}
			}
		}
	}
}

void PuzzleScene::showSuccessPuzzleEffect()
{
	CCLOG("success puzzle animation");
	addChild(KSGradualValue<float>::create(255, 0, 0.5f, [=](float t){
//		selected_piece_img->setOpacity(t);
	}, [=](float t){
//		selected_piece_img->setOpacity(t);
		selected_piece_img->setVisible(false);
	}));
//	if(selected_piece_img)
//	{
//		selected_piece_img->runAction(CCFadeTo::create(0.5f, 0));
//	}
	
	AudioEngine::sharedInstance()->playEffect("se_puzzleopen_1.mp3", false);
	
	int start_stage = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
	
	for(int i=start_stage;i<start_stage+stage_count;i++)
	{
		PuzzlePiece* new_piece = (PuzzlePiece*)puzzle_node->getChildByTag(i);
		new_piece->simpleView();
	}
	
	CCClippingNode* clipping_node = CCClippingNode::create(CCSprite::create("puzzle_clearmask_light.png"));
	clipping_node->setPosition(ccp(0,0));
	clipping_node->setAlphaThreshold(0.1f);
	puzzle_node->addChild(clipping_node, 999);
	
	CCSprite* inner_node = CCSprite::create("puzzle_clear_light.png");
	inner_node->setOpacity(130);
	inner_node->setPosition(ccp(-inner_node->getContentSize().width/2.f-clipping_node->getStencil()->getContentSize().width/2.f, 0));
	clipping_node->addChild(inner_node);
	
	inner_node->setBlendFunc(ccBlendFunc{GL_SRC_ALPHA, GL_ONE});
	
	CCMoveTo* t_move1 = CCMoveTo::create(1.f, ccp(inner_node->getContentSize().width/2.f+clipping_node->getStencil()->getContentSize().width/2.f, 0));
	CCMoveTo* t_move2 = CCMoveTo::create(0.f, ccp(-inner_node->getContentSize().width/2.f-clipping_node->getStencil()->getContentSize().width/2.f, 0));
	CCSequence* t_seq1 = CCSequence::create(t_move1, t_move2, NULL);
	
	CCRepeat* t_repeat = CCRepeat::create(t_seq1, 2);
	
	CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(PuzzleScene::pumpPuzzle));
	CCCallFunc* t_call2 = CCCallFunc::create(clipping_node, callfunc_selector(CCClippingNode::removeFromParent));
	
	CCSequence* t_seq3 = CCSequence::create(t_repeat, t_call1, t_call2, NULL);
	
	inner_node->runAction(t_seq3);
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
//	CurtainNodeForBonusGame* bonusGame = CurtainNodeForBonusGame::create(kBonusGameCode_gababo, (int)Curtain::kTouchPriority, [=](){
//		//		if(m_gameCode == kMiniGameCode_gababo)
//		{
//			BonusGameReward gr1;
//			gr1.spriteName = "shop_ruby2.png";
//			gr1.desc = "루우비~!";
//			
//			BonusGameReward gr2;
//			gr2.spriteName = "shop_ruby2.png";
//			gr2.desc = "루우비~!";
//			BonusGameReward gr3;
//			gr3.spriteName = "shop_ruby2.png";
//			gr3.desc = "루우비~!";
//			BonusGameReward gr4;
//			gr4.spriteName = "shop_ruby2.png";
//			gr4.desc = "루우비~!";
//			GaBaBo* gbb = GaBaBo::create(-500, {gr1, gr2, gr3,gr4}, [=](int t_i)
//										 {
	
	int loop_cnt = NSDS_GI(kSDS_GI_puzzleListCount_i);
	int found_unlock_puzzle_number = -1;
	for(int i=0;found_unlock_puzzle_number == -1 && i<loop_cnt;i++)
	{
		int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1);
		
		if(NSDS_GB(t_puzzle_number, kSDS_PZ_isEvent_b))
			continue;
		
		PuzzleOpenInfo t_info;
		t_info.is_open = mySGD->getPuzzleHistory(t_puzzle_number).is_open.getV();
		
		string puzzle_condition = NSDS_GS(t_puzzle_number, kSDS_PZ_condition_s);
		
		Json::Value condition_list;
		Json::Reader reader;
		reader.parse(puzzle_condition, condition_list);
		
		for(int i=0;found_unlock_puzzle_number == -1 && i<condition_list.size();i++)
		{
			Json::Value t_condition_and = condition_list[i];
			
			for(int j=0;found_unlock_puzzle_number == -1 && j<t_condition_and.size();j++)
			{
				Json::Value t_condition = t_condition_and[j];
				string t_type = t_condition["type"].asString();
				if(t_type == "p")
				{
					if(t_condition["value"].asInt() == myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber))
						found_unlock_puzzle_number = t_puzzle_number;
				}
			}
		}
	}
											 mySGD->setIsUnlockPuzzle(found_unlock_puzzle_number);
											 startBacking();
//										 });
//			addChild(gbb, (int)Curtain::kBonusGame);
//		}
//	});
//	addChild(bonusGame, (int)Curtain::kCurtain);
}

void PuzzleScene::showPerfectPuzzleEffect()
{
	CCLOG("perfect puzzle animation");
	
	if(selected_piece_img)
	{
		selected_piece_img->setVisible(false);
//		addChild(KSGradualValue<float>::create(255, 0, 0.5f, [=](float t){
//			selected_piece_img->setOpacity(t);
//		}, [=](float t){
//			selected_piece_img->setOpacity(t);
//		}));
	}
	
	int start_stage = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_stageCount_i);
	
	AudioEngine::sharedInstance()->playEffect("se_pieceget.mp3", false);
	
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
	mySGD->setIsPerfectPuzzle(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)+1);
	startBacking();
}

void PuzzleScene::showUnlockEffect()
{
	CCLOG("unlock piece animation");
	
	if(unlock_cover)
	{
		AudioEngine::sharedInstance()->playEffect("se_pieceopen.mp3", false);
		
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
	
	setPieceClick(next_stage_number);
	setRight();
	
	addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
	is_menu_enable = true;
}

void PuzzleScene::showFailPopup()
{
	is_menu_enable = false;
	
	FailPopup* t_popup = FailPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::hideFailPopup));
	t_popup->replay_func = [=](){openSettingPopup();};
	t_popup->goToMainFlow_func = [=](){is_menu_enable = false; startBacking();};
	t_popup->refresh_achieve_func = bind(&PuzzleScene::countingAchievement, this);
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
	
	addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
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
//	setBackKeyEnabled(false);
	
	CCSprite* title_name = CCSprite::create("temp_title_name.png");
	title_name->setPosition(ccp(240,160));
	title_name->setOpacity(0);
	addChild(title_name, kPuzzleZorder_back);
	
	CCFadeTo* t_fade = CCFadeTo::create(0.5f, 255);
	title_name->runAction(t_fade);
	
	CCPoint original_position = puzzle_node->getPosition();
	addChild(KSGradualValue<float>::create(0.f, 1.f, 0.5f, [=](float t)
										   {
											   puzzle_node->setPositionX(original_position.x - t*600.f);
											   KS::setOpacity(puzzle_node, 255-t*255);
										   }, [=](float t)
										   {
											   puzzle_node->setPositionX(original_position.x - 600.f);
											   KS::setOpacity(puzzle_node, 0);
											   
											   mySGD->resetLabels();
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
	
	CCSprite* top = mySIL->getUnsafeLoadedImg(CCString::createWithFormat("puzzle%d_original_top.png", puzzle_number)->getCString());
	top->setAnchorPoint(ccp(0.5, 1));
	top->setPosition(ccp(0, puzzle_size.height/2.f));
	puzzle_node->addChild(top, kPuzzleNodeZorder_puzzle);
	
	CCSprite* bottom = mySIL->getUnsafeLoadedImg(CCString::createWithFormat("puzzle%d_original_bottom.png", puzzle_number)->getCString());
	bottom->setAnchorPoint(ccp(0.5, 0));
	bottom->setPosition(ccp(0, -puzzle_size.height/2.f));
	puzzle_node->addChild(bottom, kPuzzleNodeZorder_puzzle);
	
	CCSprite* left = mySIL->getUnsafeLoadedImg(CCString::createWithFormat("puzzle%d_original_left.png", puzzle_number)->getCString());
	left->setAnchorPoint(ccp(0, 0.5));
	left->setPosition(ccp(-puzzle_size.width/2.f, 0));
	puzzle_node->addChild(left, kPuzzleNodeZorder_puzzle);
	
	CCSprite* right = mySIL->getUnsafeLoadedImg(CCString::createWithFormat("puzzle%d_original_right.png", puzzle_number)->getCString());
	right->setAnchorPoint(ccp(1, 0.5));
	right->setPosition(ccp(puzzle_size.width/2.f, 0));
	puzzle_node->addChild(right, kPuzzleNodeZorder_puzzle);
	
	int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
	int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
	
	bool is_puzzle_clear = true;
	bool must_be_change_selected_stage_number = false;
	int enable_stage_number = -1;
	
	unlock_cover = NULL;
	
	int selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number);
	
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
			if(stage_number == 1 || mySGD->getPieceHistory(stage_number).is_open.getV() ||
			   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
				(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 || mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
			{
				PieceHistory t_history = mySGD->getPieceHistory(stage_number);
				
				if(mySGD->isClearPiece(stage_number))
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::pieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_piece, stage_number);
					t_piece->setTurnInfo(t_history.is_clear[0].getV(), t_history.is_clear[1].getV(), t_history.is_clear[2].getV(), t_history.is_clear[3].getV());
					t_piece->initWithPieceInfo(piece_mode, kPieceType_color, piece_type);
				}
				else // empty
				{
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
				if(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) <= 0 || mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number))) // buy
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::buyPieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_strokePiece, stage_number);
					t_piece->setTurnInfo(false, false, false, false);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_buy, piece_type);
				}
				else // lock
				{
					PuzzlePiece* t_piece = PuzzlePiece::create(stage_number, stage_level, this, callfuncI_selector(PuzzleScene::lockPieceAction));
					t_piece->setPosition(piece_position);
					puzzle_node->addChild(t_piece, kPuzzleNodeZorder_strokePiece, stage_number);
					t_piece->setTurnInfo(false, false, false, false);
					t_piece->initWithPieceInfo(piece_mode, kPieceType_lock, piece_type);
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
			CCSprite* piece = mySIL->getUnsafeLoadedImg(CCString::createWithFormat("puzzle%d_original_piece%d.png", puzzle_number, i)->getCString());
			piece->setAnchorPoint(ccp(0.5,0.5));
			piece->setPosition(piece_position);
			puzzle_node->addChild(piece, kPuzzleNodeZorder_puzzle);
		}
	}
	
	if(must_be_change_selected_stage_number && enable_stage_number != -1) // 현재 선택된 스테이지가 선택 불가 스테이지라면
	{
		selected_stage_number = enable_stage_number;
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, selected_stage_number);
	}
	
	setPieceClick(selected_stage_number);
	
//	have_card_cnt_case = CCSprite::create("have_card_cnt_case.png");
//	have_card_cnt_case->setPosition(ccp(0, puzzle_size.height/2.f-have_card_cnt_case->getContentSize().height/2.f));
//	have_card_cnt_case->setVisible(false);
	
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
	
	if(NSDS_GB(puzzle_number, kSDS_PZ_isEvent_b))
	{
		CCSprite* t_twinkle = KS::loadCCBI<CCSprite*>(this, "puzzle_twinkle.ccbi").first;
		t_twinkle->setPosition(ccp(0,0));
		puzzle_node->addChild(t_twinkle, kPuzzleNodeZorder_haveCardCntCase);
	}
	
//	CCLabelTTF* have_card_cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d / %d", have_card_cnt, total_card_cnt)->getCString(), mySGD->getFont().c_str(), 13);
//	have_card_cnt_label->setPosition(ccp(have_card_cnt_case->getContentSize().width/2.f-20, have_card_cnt_case->getContentSize().height/2.f));
//	have_card_cnt_case->addChild(have_card_cnt_label);
//	
//	puzzle_node->addChild(have_card_cnt_case, kPuzzleNodeZorder_haveCardCntCase);
//	
//	CCSprite* n_change_mode = CCSprite::create("puzzle_change_mode.png");
//	CCSprite* s_change_mode = CCSprite::create("puzzle_change_mode.png");
//	s_change_mode->setColor(ccGRAY);
//	
//	CCMenuItem* change_mode_item = CCMenuItemSprite::create(n_change_mode, s_change_mode, this, menu_selector(PuzzleScene::menuAction));
//	change_mode_item->setTag(kPuzzleMenuTag_changeMode);
//	
//	CCMenu* change_mode_menu = CCMenu::createWithItem(change_mode_item);
//	change_mode_menu->setPosition(ccp(puzzle_size.width/2.f-n_change_mode->getContentSize().width/2.f, puzzle_size.height/2.f-n_change_mode->getContentSize().height/2.f));
//	puzzle_node->addChild(change_mode_menu, kPuzzleNodeZorder_changeMode);
//	change_mode_menu->setTouchPriority(kCCMenuHandlerPriority-1);
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
	{
		int before_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber));
		PuzzlePiece* target_piece = (PuzzlePiece*)puzzle_node->getChildByTag(before_stage_number);
		target_piece->unRegSelectImg();
		selected_piece_img->removeFromParent();
		selected_piece_img = NULL;
	}
	
	myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), t_stage_number);
	
	PuzzlePiece* target_piece = (PuzzlePiece*)puzzle_node->getChildByTag(t_stage_number);
	string WorH = target_piece->getWorH();
	
	selected_piece_img = CCNode::create();
	selected_piece_img->setPosition(target_piece->getPosition());
	puzzle_node->addChild(selected_piece_img, kPuzzleNodeZorder_selected);
	
	CCSprite* light_img = KS::loadCCBI<CCSprite*>(this, ("piece_selected_" + WorH + ".ccbi").c_str()).first;
	selected_piece_img->addChild(light_img);
	
	target_piece->regSelectImg(selected_piece_img);
}

void PuzzleScene::pieceAction(int t_stage_number)
{
	CCLOG("pieceAction : %d", t_stage_number);
	
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
		AudioEngine::sharedInstance()->playEffect("se_piece.mp3", false);
		
		if(selected_piece_img && myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)) == t_stage_number)
		{
			CCNode* t_node = CCNode::create();
			t_node->setTag(kPuzzleMenuTag_start);
			menuAction(t_node);
		}
		else
		{
			setPieceClick(t_stage_number);
			
			setRight();
		}
	}
}

void PuzzleScene::buyPieceAction(int t_stage_number)
{
	CCLOG("buyPieceAction : %d", t_stage_number);
	
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
		
		AudioEngine::sharedInstance()->playEffect("se_lock.mp3", false);
		
		BuyPiecePopup* t_popup = BuyPiecePopup::create(-200, [=](){is_menu_enable = true;}, [=]()
													   {
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
														   
														   is_menu_enable = true;
														   pieceAction(t_stage_number);
													   }, t_stage_number);
		addChild(t_popup, kPuzzleZorder_popup);
	}
}

void PuzzleScene::lockPieceAction(int t_stage_number)
{
	if(!is_menu_enable)
		return;
	
	CCLOG("lockPieceAction : %d", t_stage_number);
	
	AudioEngine::sharedInstance()->playEffect("se_lock.mp3", false);
}

void PuzzleScene::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	
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
			
			mySD->setSilType(myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number));
			
			StartSettingPopup* t_popup = StartSettingPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
			t_popup->goToMainFlow_func = [=](){is_menu_enable = false; startBacking();};
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
//			showSuccessPuzzleEffect();
			startBacking();
//			CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
		}
		else if(tag == kPuzzleMenuTag_rubyShop)
		{
//			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_afterOpenCBT), [=](){is_menu_enable = true;}), 9999);
			showShopPopup(kSC_ruby);
		}
		else if(tag == kPuzzleMenuTag_goldShop)
		{
			showShopPopup(kSC_gold);
		}
		else if(tag == kPuzzleMenuTag_heartShop)
		{
			showShopPopup(kSC_heart);
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
			
			t_popup->open_message_popup_func = [=]()
			{
				is_menu_enable = false;
				SumranMailPopup* t_pp = SumranMailPopup::create(this, callfunc_selector(PuzzleScene::mailPopupClose), bind(&PuzzleScene::heartRefresh, this));
				addChild(t_pp, kPuzzleZorder_popup);
				
				postbox_count_case->setVisible(false);
			};
		}
		else if(tag == kPuzzleMenuTag_event)
		{
			
			is_menu_enable = true;
			string serverUrl = myHSP->getServerAddress();
			string os="ios";
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			os="android";
#endif
			string lang = KS::getLocalCode();
			string cc = myHSP->getCountryCode();
			string tz = myHSP->getTimeZone();
			myHSP->openHSPUrl(serverUrl+"/event.php?lang="+lang+"&country="+cc+"&timezone="+tz+"&os="+os);
			
			
//			CurtainNodeForBonusGame* bonusGame = CurtainNodeForBonusGame::create(kBonusGameCode_gababo, (int)Curtain::kTouchPriority, [=](){
//				//		if(m_gameCode == kMiniGameCode_gababo)
//				{
//					Json::Value reward_info = mySGD->getAllClearReward();
//					
//					BonusGameReward gr1;
//					gr1.spriteName = "morphing_heart2.png";
//					gr1.desc = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_gababoReward), reward_info[0]["reward"][0]["count"].asInt());
//					
//					BonusGameReward gr2;
//					gr2.spriteName = "morphing_heart2.png";
//					gr2.desc = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_gababoReward), reward_info[1]["reward"][0]["count"].asInt());
//					BonusGameReward gr3;
//					gr3.spriteName = "morphing_heart2.png";
//					gr3.desc = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_gababoReward), reward_info[2]["reward"][0]["count"].asInt());
//					BonusGameReward gr4;
//					gr4.spriteName = "morphing_heart2.png";
//					gr4.desc = ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_gababoReward), reward_info[3]["reward"][0]["count"].asInt());
////					GaBaBo* gbb = GaBaBo::create(-500, {gr1, gr2, gr3,gr4}, [=](int t_i)
////																			 {
////																				 is_menu_enable = true;
////																			 });
////					addChild(gbb, (int)Curtain::kBonusGame);
//					JsGababo* gbb = JsGababo::create(-500, {gr1, gr2, gr3, gr4}, [=](int t_i)
//																			 {
//																				 mySGD->addChangeGoods(reward_info[t_i]["exchangeID"].asString());
//																				 
//																				 LoadingLayer* t_loading = LoadingLayer::create(-600);
//																				 addChild(t_loading, 1000);
//																				 
//																				 tryGababoReward(t_loading, [=](){is_menu_enable = true;});
//																			 });
//					addChild(gbb, (int)Curtain::kBonusGame);
//				}
//			});
//			addChild(bonusGame, (int)Curtain::kCurtain);
//
////			showSuccessPuzzleEffect();
		}
		else if(tag == kPuzzleMenuTag_achieve)
		{
			AchievePopup* t_ap = AchievePopup::create();
			addChild(t_ap, kPuzzleZorder_popup);
			
			t_ap->setHideFinalAction(this, callfunc_selector(PuzzleScene::achievePopupClose));
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
//				have_card_cnt_case->setVisible(true);
			}
			else if(piece_mode == kPieceMode_thumb)
			{
				piece_mode = kPieceMode_default;
//				have_card_cnt_case->setVisible(false);
			}
			
			myDSH->setIntegerForKey(kDSH_Key_puzzleMode, piece_mode);
			
			for(int i=start_stage;i<start_stage+stage_count;i++)
				((PuzzlePiece*)puzzle_node->getChildByTag(i))->turnPiece(piece_mode);
			
			stopAutoTurnPiece();
			addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
//			is_auto_turn = false;
//			auto_turn_piece_frame = 0;
			
			is_menu_enable = true;
		}
	}
}

void PuzzleScene::tryGababoReward(CCNode* t_loading, function<void()> success_func)
{
	mySGD->changeGoods([=](Json::Value result_data)
					   {
						   if(result_data["result"]["code"].asInt() == GDSUCCESS)
							{
								mySGD->network_check_cnt = 0;
								
								t_loading->removeFromParent();
								success_func();
							}
						   else
							{
								mySGD->network_check_cnt++;
								
								if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
								{
									mySGD->network_check_cnt = 0;
									
									ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999, myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4), [=](){
										tryGababoReward(t_loading, success_func);
									}, 1);
									
//									ASPopupView *alert = ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
//										tryGababoReward(t_loading, success_func);
//									});
									if(alert)
										((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
								}
								else
								{
									addChild(KSTimer::create(0.5f, [=]()
															 {
																 tryGababoReward(t_loading, success_func);
															 }));
								}
							}
					   });
}

void PuzzleScene::onEnterTransitionDidFinish()
{
    CCLayer::onEnterTransitionDidFinish();
    TRACE();
}

void PuzzleScene::showShopPopup(ShopCode t_code)
{
	ShopPopup* t_shop = ShopPopup::create();
	t_shop->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
	t_shop->targetHeartTime(heart_time);
	t_shop->setShopCode(t_code);
	t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
	addChild(t_shop, kPuzzleZorder_popup+100);
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
	
//	if(t_piece->is_simple)
//		t_piece->turnPiece(kPieceMode_thumb);
//	else
//	{
		if(t_piece->piece_mode == kPieceMode_default)
			t_piece->turnPiece(kPieceMode_thumb);
		else if(t_piece->piece_mode == kPieceMode_thumb)
			t_piece->turnPiece(kPieceMode_default);
//	}
	
	auto_turn_piece_frame++;
	
	if(auto_turn_piece_frame == stage_count)
	{
		stopAutoTurnPiece();
		addChild(KSTimer::create(3.f, [=](){startAutoTurnPiece();}));
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

void PuzzleScene::openBeforeSettingPopup()
{
	is_menu_enable = false;
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number, mySD->getSilType());
	
	StartSettingPopup* t_popup = StartSettingPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
	t_popup->goToMainFlow_func = [=](){is_menu_enable = false; startBacking();};
	addChild(t_popup, kPuzzleZorder_popup);
}

void PuzzleScene::openSettingPopup()
{
	is_menu_enable = false;
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	mySD->setSilType(myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number));
	
	StartSettingPopup* t_popup = StartSettingPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(PuzzleScene::popupClose));
	t_popup->goToMainFlow_func = [=](){is_menu_enable = false; startBacking();};
	addChild(t_popup, kPuzzleZorder_popup);
}

void PuzzleScene::mailPopupClose()
{
	TRACE();
	addChild(KSTimer::create(1.f, [=](){
		countingMessage();
	}));
	is_menu_enable = true;
    TRACE();
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
		
		
		loading_progress_img=NULL;
		myHSP->removeTarget(this);
	}
	
	right_body = CCSprite::create("puzzle_right_body.png");
	right_body->setPosition(ccp(-right_body->getContentSize().width/2.f-6, 23));
	right_case->addChild(right_body);
	
	int selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber));
	
	if(right_mode == kPuzzleRightMode_stage)
	{
//		PieceHistory t_history = mySGD->getPieceHistory(selected_stage_number);
		
		bool is_have_card_list[4] = {false,};
		
		int stage_card_count = NSDS_GI(selected_stage_number, kSDS_SI_cardCount_i);
		for(int i=1;i<=stage_card_count && i <= 4;i++)
		{
			int step_card_number = NSDS_GI(selected_stage_number, kSDS_SI_level_int1_card_i, i);
			is_have_card_list[i-1] = mySGD->isHasGottenCards(step_card_number);
			
			CCPoint step_position = ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height-53-(i-1)*46.5f);
			
			if(is_have_card_list[i-1])
			{
				CCClippingNode* t_clipping = CCClippingNode::create(CCSprite::create("puzzle_right_sumcrop.png"));
				
				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
				if(screen_scale_x < 1.f)
					screen_scale_x = 1.f;
				
				float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
				
				float change_scale = 1.f;
				CCPoint change_origin = ccp(0,0);
				if(screen_scale_x > 1.f)
				{
					change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
					change_scale = screen_scale_x;
				}
				if(screen_scale_y > 1.f)
					change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
				CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//				t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
				
				
				t_clipping->setPosition(step_position);
				right_body->addChild(t_clipping);
				
				t_clipping->setAlphaThreshold(0.1f);
				
				CCSprite* t_inner = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_visible.png", step_card_number)->getCString()));
				t_inner->setScale(0.4f);
				
				t_inner->setPositionY(rand()%120-60);
				
//				if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, step_card_number))
//				{
//					CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, step_card_number)).first;
//					ccb_img->setPosition(ccp(160,215));
//					t_inner->addChild(ccb_img);
//				}
				
				t_clipping->addChild(t_inner);
				
				
				if(mySGD->getPieceHistory(selected_stage_number).is_clear[i-1].getV())
				{
					int card_rank = NSDS_GI(kSDS_CI_int1_rank_i, step_card_number);
					for(int j=0;j<card_rank;j++)
					{
						CCSprite* t_star = CCSprite::create("star_on.png");
						t_star->setPosition(ccpAdd(step_position, ccp(-48.f+j*13.5f,10)));
						right_body->addChild(t_star);
					}
				}
				
				CCSprite* n_show = CCSprite::create("puzzle_right_view.png");
				KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_view), mySGD->getFont().c_str(), 12);
				n_label->disableOuterStroke();
				n_label->setPosition(ccpFromSize(n_show->getContentSize()/2.f) + ccp(2,0));
				n_show->addChild(n_label);
				
				CCSprite* s_show = CCSprite::create("puzzle_right_view.png");
				s_show->setColor(ccGRAY);
				KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_view), mySGD->getFont().c_str(), 12);
				s_label->disableOuterStroke();
				s_label->setPosition(ccpFromSize(s_show->getContentSize()/2.f) + ccp(2,0));
				s_show->addChild(s_label);
				
				CCMenuItemLambda* show_item = CCMenuItemSpriteLambda::create(n_show, s_show, [=](CCObject* sender)
																			 {
																				 if(!is_menu_enable)
																					 return;
																				 
																				 is_menu_enable = false;
																				 
																				 mySGD->selected_collectionbook = step_card_number;
																				 
																				 CCDirector::sharedDirector()->pushScene(CardViewScene::scene([=](){is_menu_enable = true;}));
																			 });
				
				CCMenuLambda* show_menu = CCMenuLambda::createWithItem(show_item);
				show_menu->setPosition(ccpAdd(step_position, ccp(44,0)));
				right_body->addChild(show_menu);
			}
			else
			{
				CCSprite* n_back = CCSprite::create("puzzle_right_lock.png");
				CCSprite* s_back = CCSprite::create("puzzle_right_lock.png");
				s_back->setColor(ccGRAY);
				
				CCMenuItemSpriteLambda* back_item = CCMenuItemSpriteLambda::create(n_back, s_back, [=](CCObject* sender){
					if(!is_menu_enable)
						return;
					
					is_menu_enable = false;
					
					AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
					CardLockedPopup* t_popup = CardLockedPopup::create(-999, [=](){is_menu_enable = true;}, i);
					addChild(t_popup, kPuzzleZorder_popup);
				});
				
				CCMenuLambda* back_menu = CCMenuLambda::createWithItem(back_item);
				back_menu->setPosition(step_position);
				right_body->addChild(back_menu);
				
				
				string condition_string;
				if(i == 1)
					condition_string = myLoc->getLocalForKey(LK::kMyLocalKey_condition1);
				else if(i == 2)
					condition_string = myLoc->getLocalForKey(LK::kMyLocalKey_condition2);
				else if(i == 3)
					condition_string = myLoc->getLocalForKey(LK::kMyLocalKey_condition3);
				else
					condition_string = myLoc->getLocalForKey(LK::kMyLocalKey_condition4);
				
				KSLabelTTF* condition_label = KSLabelTTF::create(condition_string.c_str(), mySGD->getFont().c_str(), 10);
				condition_label->disableOuterStroke();
				condition_label->setAnchorPoint(ccp(0.f,0.5f));
				condition_label->setPosition(step_position + ccp(8-64,15-23.5f));
				right_body->addChild(condition_label);
				
				if(mySGD->getPieceHistory(selected_stage_number).is_clear[i-1].getV())
				{
					int card_rank = NSDS_GI(kSDS_CI_int1_rank_i, step_card_number);
					for(int j=0;j<card_rank;j++)
					{
						CCSprite* t_star = CCSprite::create("star_on.png");
						t_star->setPosition(ccpAdd(step_position, ccp(-48.f+j*13.5f,10)));
						right_body->addChild(t_star);
					}
				}
				else
				{
					int card_rank = NSDS_GI(kSDS_CI_int1_rank_i, step_card_number);
					for(int j=0;j<card_rank;j++)
					{
						CCSprite* t_star = CCSprite::create("star_off.png");
						t_star->setPosition(ccpAdd(step_position, ccp(-48.f+j*13.5f,10)));
						right_body->addChild(t_star);
					}
				}
				
				if(NSDS_GB(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber), kSDS_PZ_isEvent_b))
				{
					CCSprite* n_event = CCSprite::create("puzzle_event.png");
					n_event->setPosition(step_position + ccp(17,11.5f));
					right_body->addChild(n_event);
				}
			}
		}
	}
	else if(right_mode == kPuzzleRightMode_ranking)
	{
		if(saved_ranking_stage_number == -1 || saved_ranking_stage_number != selected_stage_number)
		{
			TRACE();
			if(!loading_progress_img)
			{
//				CCNodeLoaderLibrary* nodeLoader = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
//				CCBReader* reader = new CCBReader(nodeLoader);
//				loading_progress_img = dynamic_cast<CCSprite*>(reader->readNodeGraphFromFile("loading.ccbi",this));
				loading_progress_img = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
				loading_progress_img->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height-58-70));
				right_body->addChild(loading_progress_img);
//				reader->release();
			}
			TRACE();
			Json::Value param;
			param["memberID"] = hspConnector::get()->getSocialID();
			param["stageNo"] = selected_stage_number;
			call_rank_stage_number = selected_stage_number;
			hspConnector::get()->command("getstagerankbyalluser", param, this,json_selector(this, PuzzleScene::resultGetRank));
		}
		else
		{
			call_rank_stage_number = saved_ranking_stage_number;
			resultGetRank(saved_ranking_data);
		}
	}
	
	if(ready_menu)
	{
		ready_menu->removeFromParent();
		ready_menu = NULL;
	}
	
	CCSprite* n_ready = CCSprite::create("puzzle_right_ready.png");
	
	CCLabelTTF* n_stage = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_stageValue), selected_stage_number)->getCString(),
												 mySGD->getFont().c_str(), 11);
	n_stage->setPosition(ccp(n_ready->getContentSize().width/2.f,n_ready->getContentSize().height/2.f+9));
	n_ready->addChild(n_stage);
	
	KSLabelTTF* n_ready_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ready), mySGD->getFont().c_str(), 18.5f);
	n_ready_label->disableOuterStroke();
	n_ready_label->setPosition(ccp(n_ready->getContentSize().width/2.f, n_ready->getContentSize().height*0.4f-1));
	n_ready->addChild(n_ready_label);
	
	CCSprite* s_ready = CCSprite::create("puzzle_right_ready.png");
	s_ready->setColor(ccGRAY);
	
	CCLabelTTF* s_stage = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_stageValue), selected_stage_number)->getCString(),
												 mySGD->getFont().c_str(), 11);
	s_stage->setPosition(ccp(s_ready->getContentSize().width/2.f,s_ready->getContentSize().height/2.f+9));
	s_ready->addChild(s_stage);
	
	KSLabelTTF* s_ready_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ready), mySGD->getFont().c_str(), 18.5f);
	s_ready_label->disableOuterStroke();
	s_ready_label->setPosition(ccp(s_ready->getContentSize().width/2.f, s_ready->getContentSize().height*0.4f-1));
	s_ready->addChild(s_ready_label);
	
	CCMenuItem* ready_item = CCMenuItemSprite::create(n_ready, s_ready, this, menu_selector(PuzzleScene::menuAction));
	ready_item->setTag(kPuzzleMenuTag_start);
	
	ready_menu = CCMenu::createWithItem(ready_item);
	ready_menu->setPosition(ccp(-65-6,-puzzle_node->getPositionY()+28+4));
	right_case->addChild(ready_menu);
	
	setRightTopButton();
}

void PuzzleScene::resultGetRank(Json::Value result_data)
{
	TRACE();
	if(loading_progress_img)
	{
		loading_progress_img->removeFromParent();
		loading_progress_img = NULL;
	}
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		TRACE();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		
		saved_ranking_data = result_data;
		saved_ranking_stage_number = result_data["stageNo"].asInt();
		
		if(saved_ranking_stage_number != call_rank_stage_number)
			return;
		
		Json::Value user_list = result_data["list"];
		
		CCSprite* graph_back = CCSprite::create("puzzle_rank_graph.png");
		graph_back->setPosition(ccp(right_body->getContentSize().width/2.f,177));
		right_body->addChild(graph_back);
		
//		KSLabelTTF* t_rank_a = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankA), mySGD->getFont().c_str(), 9);
//		t_rank_a->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_a->setPosition(ccp(16,8));
//		graph_back->addChild(t_rank_a);
//		
//		KSLabelTTF* t_rank_b = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankB), mySGD->getFont().c_str(), 9);
//		t_rank_b->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_b->setPosition(ccp(16+28,8));
//		graph_back->addChild(t_rank_b);
//		
//		KSLabelTTF* t_rank_c = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankC), mySGD->getFont().c_str(), 9);
//		t_rank_c->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_c->setPosition(ccp(17+56,8));
//		graph_back->addChild(t_rank_c);
//		
//		KSLabelTTF* t_rank_d = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankD), mySGD->getFont().c_str(), 9);
//		t_rank_d->enableOuterStroke(ccc3(41, 41, 41), 1.f);
//		t_rank_d->setPosition(ccp(17+84,8));
//		graph_back->addChild(t_rank_d);
		
		int alluser = result_data["alluser"].asInt();
		int myrank = result_data["myrank"].asInt();
		
//		CCLabelTTF* all_user_label = CCLabelTTF::create(CCString::createWithFormat("/%d", alluser)->getCString(), mySGD->getFont().c_str(), 10);
//		all_user_label->setColor(ccc3(255, 50, 50));
//		all_user_label->setAnchorPoint(ccp(1,0.5));
//		all_user_label->setPosition(ccp(right_body->getContentSize().width-10, 204));
//		right_body->addChild(all_user_label);
//		
//		CCLabelTTF* my_rank_label = CCLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_myrankValue), myrank)->getCString(), mySGD->getFont().c_str(), 10);
//		my_rank_label->setAnchorPoint(ccp(1,0.5));
//		my_rank_label->setPosition(ccp(all_user_label->getPositionX()-all_user_label->getContentSize().width, all_user_label->getPositionY()));
//		right_body->addChild(my_rank_label);
		
		float rank_percent = alluser == 0 ? 1.f : 1.f * myrank/alluser;
		
		CCSprite* rank_percent_case = CCSprite::create("gameresult_rank_percent.png");//"puzzle_rank_percent.png");
		rank_percent_case->setAnchorPoint(ccp(0.5,1));
		rank_percent_case->setPosition(ccp(10+115,177));
		right_body->addChild(rank_percent_case);
		
		KSLabelTTF* percent_label = KSLabelTTF::create("100%", mySGD->getFont().c_str(), 12);
		percent_label->addChild(KSGradualValue<float>::create(100.f, rank_percent*100.f,
																					 2.f * (1.f - rank_percent), [=](float t){
																					 percent_label->setString(ccsf("%.0f%%", t));
		}, [=](float t){
			percent_label->setString(ccsf("%.0f%%", t));
		}));
		percent_label->enableOuterStroke(ccc3(50, 25, 0), 0.5f);
		percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1, rank_percent_case->getContentSize().height/2.f-3));
		rank_percent_case->addChild(percent_label);
		
		CCMoveTo* t_move = CCMoveTo::create(2.f*(1.f-rank_percent), ccp(10 + 115.f*rank_percent,177));
		rank_percent_case->runAction(t_move);
		
//		delay_index = 0;
		int limit_count = 3;
		for(int i=0;i<user_list.size() && i<limit_count;i++)
		{
			string case_name;
			if(myrank == i+1)
			{
				case_name = "mainpopup_purple.png";
				limit_count++;
			}
			else
			{
				case_name = "rank_normal2.png";
			}
			
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			list_cell_case->setContentSize(CCSizeMake(124, 36));
			list_cell_case->setPosition(ccp(right_body->getContentSize().width/2.f,130-i*36.f));
			right_body->addChild(list_cell_case);
			
			CCPoint rank_position = ccp(14,list_cell_case->getContentSize().height/2.f);
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
				KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 10);
				rank_label->disableOuterStroke();
				rank_label->setPosition(ccp(25.5f, rank_position.y+8));//rank_position);
				list_cell_case->addChild(rank_label);
			}

			Json::Reader reader;
			Json::Value read_data;
			reader.parse(user_list[i].get("data", Json::Value()).asString(), read_data);
			
			string flag = user_list[i].get("flag", "kr").asString().c_str();
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			if(i >= 3)
				selectedFlagSpr->setPosition(ccp(25.5f, rank_position.y-5));
			else
				selectedFlagSpr->setPosition(ccp(37,list_cell_case->getContentSize().height/2.f));
			selectedFlagSpr->setScale(0.7);
			list_cell_case->addChild(selectedFlagSpr);
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12.5f);
			if(t_nick_size->getContentSize().width > 70)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 70, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font strokecolor=000 strokesize=0.3 strokeopacity=50>" + user_list[i].get("nick", Json::Value()).asString() + "</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.5f);
				nick_marquee->setAnchorPoint(ccp(0.5f,0.5f));
				nick_marquee->setPosition(ccp(84,list_cell_case->getContentSize().height/2.f + 7));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(user_list[i].get("nick", Json::Value()).asString().c_str(), mySGD->getFont().c_str(), 12.5f); // user_list[i]["nick"].asString().c_str()
				nick_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				nick_label->setPosition(ccp(84,list_cell_case->getContentSize().height/2.f + 7));
				list_cell_case->addChild(nick_label);
			}
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",user_list[i]["score"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setColor(ccc3(54, 36, 148));
			score_label->disableOuterStroke();
			score_label->setPosition(ccp(84,list_cell_case->getContentSize().height/2.f - 7));
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
			CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_purple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			list_cell_case->setContentSize(CCSizeMake(124, 36));
			list_cell_case->setPosition(ccp(right_body->getContentSize().width/2.f,130-3*36.f));
			right_body->addChild(list_cell_case);
			
			KSLabelTTF* rank_label = KSLabelTTF::create(KS::insert_separator(myrank).c_str(), mySGD->getFont().c_str(), 10);
			rank_label->disableOuterStroke();
			rank_label->setPosition(ccp(25.5f,list_cell_case->getContentSize().height/2.f+8));
			list_cell_case->addChild(rank_label);
			
			
			string flag = myDSH->getStringForKey(kDSH_Key_flag);
			CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
			selectedFlagSpr->setPosition(ccp(25.5f,list_cell_case->getContentSize().height/2.f-5));
			selectedFlagSpr->setScale(0.7);
			list_cell_case->addChild(selectedFlagSpr);
			
			CCLabelTTF* t_nick_size = CCLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12.5f);
			if(t_nick_size->getContentSize().width > 70)
			{
				LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 70, 15, "");
				nick_marquee->setSpace(30);
				nick_marquee->addText(("<font strokecolor=000 strokesize=0.3f strokeopacity=50>" + myDSH->getStringForKey(kDSH_Key_nick) + "</font>").c_str());
				nick_marquee->startMarquee();
				nick_marquee->setFontSize(12.5f);
				nick_marquee->setAnchorPoint(ccp(0.5f,0.5f));
				nick_marquee->setPosition(ccp(84,list_cell_case->getContentSize().height/2.f + 7));
				list_cell_case->addChild(nick_marquee);
			}
			else
			{
				KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12.5f); // user_list[i]["nick"].asString().c_str()
				nick_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				nick_label->setPosition(ccp(84,list_cell_case->getContentSize().height/2.f + 7));
				list_cell_case->addChild(nick_label);
			}
			
			KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",result_data["myscore"].asInt())->getCString()).c_str(), mySGD->getFont().c_str(), 12);
			score_label->setColor(ccc3(54, 36, 148));
			score_label->disableOuterStroke();
			score_label->setPosition(ccp(84,list_cell_case->getContentSize().height/2.f - 7));
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
		TRACE();
		CCLabelTTF* fail_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_failCheckRanking), mySGD->getFont().c_str(), 12);
		fail_label->setPosition(ccp(right_body->getContentSize().width/2.f, right_body->getContentSize().height-58-70));
		right_body->addChild(fail_label);
	}
}

void PuzzleScene::setRightTopButton()
{
	if(!right_top_menu)
	{
		right_top_menu = CCMenuLambda::create();
		right_top_menu->setPosition(CCPointZero);
		right_case->addChild(right_top_menu, 5);
	}
	
	if(!stage_button)
	{
		CCSprite* n_stage = CCSprite::create("puzzle_right_top_off.png");
		KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_stage), mySGD->getFont().c_str(), 12.5f);
		n_label->disableOuterStroke();
		n_label->setPosition(ccpFromSize(n_stage->getContentSize()/2.f));
		n_stage->addChild(n_label);
		CCSprite* s_stage = CCSprite::create("puzzle_right_top_off.png");
		s_stage->setColor(ccGRAY);
		KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_stage), mySGD->getFont().c_str(), 12.5f);
		s_label->disableOuterStroke();
		s_label->setColor(ccGRAY);
		s_label->setPosition(ccpFromSize(s_stage->getContentSize()/2.f));
		s_stage->addChild(s_label);
		CCSprite* d_stage = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, n_stage->getContentSize().width, n_stage->getContentSize().height));
		KSLabelTTF* d_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_stage), mySGD->getFont().c_str(), 12.5f);
		d_label->setColor(ccc3(20, 50, 70));
		d_label->disableOuterStroke();
		d_label->setPosition(ccpFromSize(d_stage->getContentSize()/2.f));
		d_stage->addChild(d_label);
		
		stage_button = CCMenuItemSpriteLambda::create(n_stage, s_stage, d_stage, [=](CCObject* sender)
													  {
														  if(!this->is_menu_enable)
															  return;
														  this->right_mode = kPuzzleRightMode_stage;
														  setRight();
													  });
		
		stage_button->setPosition(ccp(-65-5.5f-31.f, 115.5f));
		right_top_menu->addChild(stage_button);
	}
	if(!ranking_button)
	{
		CCSprite* n_ranking = CCSprite::create("puzzle_right_top_off.png");
		KSLabelTTF* n_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ranking), mySGD->getFont().c_str(), 12.5f);
		n_label->disableOuterStroke();
		n_label->setPosition(ccpFromSize(n_ranking->getContentSize()/2.f));
		n_ranking->addChild(n_label);
		CCSprite* s_ranking = CCSprite::create("puzzle_right_top_off.png");
		s_ranking->setColor(ccGRAY);
		KSLabelTTF* s_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ranking), mySGD->getFont().c_str(), 12.5f);
		s_label->disableOuterStroke();
		s_label->setColor(ccGRAY);
		s_label->setPosition(ccpFromSize(s_ranking->getContentSize()/2.f));
		s_ranking->addChild(s_label);
		CCSprite* d_ranking = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, n_ranking->getContentSize().width, n_ranking->getContentSize().height));
		KSLabelTTF* d_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ranking), mySGD->getFont().c_str(), 12.5f);
		d_label->setColor(ccc3(20, 50, 70));
		d_label->disableOuterStroke();
		d_label->setPosition(ccpFromSize(d_ranking->getContentSize()/2.f));
		d_ranking->addChild(d_label);
		
		ranking_button = CCMenuItemSpriteLambda::create(n_ranking, s_ranking, d_ranking, [=](CCObject* sender)
													  {
														  if(!this->is_menu_enable)
															  return;
														  this->right_mode = kPuzzleRightMode_ranking;
														  setRight();
													  });
		
		ranking_button->setPosition(ccp(-65-5.5f+31.f, 115.5f));
		right_top_menu->addChild(ranking_button);
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
	cancel_item->setStringData("backkey");
	
	CCMenu* cancel_menu = CCMenu::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(28,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-22));
	addChild(cancel_menu, kPuzzleZorder_top);
	
	top_list.push_back(cancel_menu);
	
	
	CCSprite* top_heart = CCSprite::create("mainflow_top_heart.png");
	top_heart->setAnchorPoint(ccp(0.5f,1.f));
	top_heart->setPosition(ccp(108,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-8));
	addChild(top_heart, kPuzzleZorder_top);
	
	top_list.push_back(top_heart);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(top_heart->getContentSize().width/2.f-49,top_heart->getContentSize().height/2.f));
	top_heart->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(PuzzleScene::menuAction));
	heart_item->setTag(kPuzzleMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(top_heart->getContentSize().width/2.f+49.5f,top_heart->getContentSize().height/2.f));
	top_heart->addChild(heart_menu);
	
	
	CCSprite* top_gold = CCSprite::create("mainflow_top_gold.png");
	top_gold->setAnchorPoint(ccp(0.5f,1.f));
	top_gold->setPosition(ccp(221.5f,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-8));
	addChild(top_gold, kPuzzleZorder_top);
	
	top_list.push_back(top_gold);
	
	CCSprite* gold_img = CCSprite::create("price_gold_img.png");
	gold_img->setPosition(ccp(top_gold->getContentSize().width/2.f-39.f, top_gold->getContentSize().height/2.f+1));
	top_gold->addChild(gold_img);
	
	GoodsLight* gold_light = GoodsLight::create(CCSprite::create("price_gold_img_mask.png"));
	gold_light->setPosition(ccp(gold_img->getContentSize().width/2.f, gold_img->getContentSize().height/2.f));
	gold_img->addChild(gold_light);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_gold))->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(top_gold->getContentSize().width/2.f,top_gold->getContentSize().height/2.f-6));
	top_gold->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(PuzzleScene::menuAction));
	gold_item->setTag(kPuzzleMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(top_gold->getContentSize().width/2.f+40,top_gold->getContentSize().height/2.f));
	top_gold->addChild(gold_menu);
	
	
	CCSprite* top_ruby = CCSprite::create("mainflow_top_ruby.png");
	top_ruby->setAnchorPoint(ccp(0.5f,1.f));
	top_ruby->setPosition(ccp(313,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-8));
	addChild(top_ruby, kPuzzleZorder_top);
	
	top_list.push_back(top_ruby);
	
	CCSprite* ruby_img = CCSprite::create("price_ruby_img.png");
	ruby_img->setPosition(ccp(top_ruby->getContentSize().width/2.f-27, top_gold->getContentSize().height/2.f));
	top_ruby->addChild(ruby_img);
	
	GoodsLight* ruby_light = GoodsLight::create(CCSprite::create("price_ruby_img_mask.png"));
	ruby_light->setPosition(ccp(top_ruby->getContentSize().width/2.f-27, ruby_img->getContentSize().height/2.f));
	ruby_img->addChild(ruby_light);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_ruby))->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(top_ruby->getContentSize().width/2.f - 1,top_ruby->getContentSize().height/2.f-6));
	top_ruby->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(PuzzleScene::menuAction));
	ruby_item->setTag(kPuzzleMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(top_ruby->getContentSize().width/2.f+28,top_ruby->getContentSize().height/2.f));
	top_ruby->addChild(ruby_menu);
	
	
	CCSprite* n_option = CCSprite::create("mainflow_new_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_new_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(PuzzleScene::menuAction));
	option_item->setTag(kPuzzleMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(429,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-20));
	addChild(option_menu, kPuzzleZorder_top);
	
	
	
	CCNode* achieve_node = CCNode::create();
	addChild(achieve_node, kPuzzleZorder_top);
	
	CCSprite* n_achieve = CCSprite::create("mainflow_new_achievement.png");
	CCSprite* s_achieve = CCSprite::create("mainflow_new_achievement.png");
	s_achieve->setColor(ccGRAY);
	
	CCMenuItem* achieve_item = CCMenuItemSprite::create(n_achieve, s_achieve, this, menu_selector(PuzzleScene::menuAction));
	achieve_item->setTag(kPuzzleMenuTag_achieve);
	
	CCMenu* achieve_menu = CCMenu::createWithItem(achieve_item);
	achieve_menu->setPosition(ccp(401,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-20));
	achieve_node->addChild(achieve_menu);
	
	CCNode* postbox_node = CCNode::create();
	addChild(postbox_node, kPuzzleZorder_top);
	
	CCSprite* n_postbox = CCSprite::create("mainflow_new_postbox.png");
	CCSprite* s_postbox = CCSprite::create("mainflow_new_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(PuzzleScene::menuAction));
	postbox_item->setTag(kPuzzleMenuTag_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccp(374,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-20));
	postbox_node->addChild(postbox_menu);
	
	top_list.push_back(postbox_node);
	
	top_list.push_back(achieve_node);
	
	top_list.push_back(option_menu);
	
	
	postbox_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	postbox_count_case->setContentSize(CCSizeMake(20, 20));
	postbox_count_case->setPosition(postbox_menu->getPosition() + ccp(6,6));
	postbox_node->addChild(postbox_count_case);
	postbox_count_case->setVisible(false);
	
	CCScaleTo* t_scale1 = CCScaleTo::create(0.1f, 1.3f);
	CCScaleTo* t_scale2 = CCScaleTo::create(0.2f, 1.f);
	CCDelayTime* t_delay1 = CCDelayTime::create(0.5f);
	CCScaleTo* t_scale3 = CCScaleTo::create(0.1f, 1.3f);
	CCScaleTo* t_scale4 = CCScaleTo::create(0.2f, 1.f);
	CCDelayTime* t_delay2 = CCDelayTime::create(5.f);
	CCSequence* t_seq1 = CCSequence::create(t_scale1, t_scale2, t_delay1, t_scale3, t_scale4, t_delay2, NULL);
	CCRepeatForever* t_repeat1 = CCRepeatForever::create(t_seq1);
	postbox_count_case->runAction(t_repeat1);
	
	postbox_count_case->setVisible(false);
	
//	postbox_count_case->addChild(KSSchedule::create([=](float dt)
//													{
//														if(postbox_menu->getPositionY() >= (myDSH->puzzle_ui_top-320.f)/2.f + 320.f-20)
//															KS::setOpacity(postbox_count_case, 0);
//														else
//															KS::setOpacity(postbox_count_case, 255);
//														return true;
//													}));
	
	postbox_count_label = KSLabelTTF::create("0", mySGD->getFont().c_str(), 8);
	postbox_count_label->setColor(ccc3(255, 255, 255));
	postbox_count_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	postbox_count_label->setPosition(ccp(postbox_count_case->getContentSize().width/2.f-0.5f, postbox_count_case->getContentSize().height/2.f+0.5f));
	postbox_count_case->addChild(postbox_count_label);
	
	TRACE();
	addChild(KSTimer::create(1.f, [=](){
		countingMessage();
	}));
	TRACE();
	
	
	achievement_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	achievement_count_case->setContentSize(CCSizeMake(20, 20));
	achievement_count_case->setPosition(achieve_menu->getPosition() + ccp(6,6));
	achieve_node->addChild(achievement_count_case);
	
	achievement_count_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 8);
	achievement_count_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	achievement_count_label->setPosition(ccp(achievement_count_case->getContentSize().width/2.f, achievement_count_case->getContentSize().height/2.f + 0));
	achievement_count_case->addChild(achievement_count_label);
	
	CCScaleTo* t_scale5 = CCScaleTo::create(0.1f, 1.3f);
	CCScaleTo* t_scale6 = CCScaleTo::create(0.2f, 1.f);
	CCDelayTime* t_delay3 = CCDelayTime::create(0.5f);
	CCScaleTo* t_scale7 = CCScaleTo::create(0.1f, 1.3f);
	CCScaleTo* t_scale8 = CCScaleTo::create(0.2f, 1.f);
	CCDelayTime* t_delay4 = CCDelayTime::create(5.f);
	CCSequence* t_seq2 = CCSequence::create(t_scale5, t_scale6, t_delay3, t_scale7, t_scale8, t_delay4, NULL);
	CCRepeatForever* t_repeat2 = CCRepeatForever::create(t_seq2);
	achievement_count_case->runAction(t_repeat2);
	
	achievement_count_case->setVisible(false);
	
//	achievement_count_case->addChild(KSSchedule::create([=](float dt)
//														{
//															if(achieve_menu->getPositionY() >= (myDSH->puzzle_ui_top-320.f)/2.f + 320.f-20)
//																KS::setOpacity(achievement_count_case, 0);
//															else
//																KS::setOpacity(achievement_count_case, 255);
//															return true;
//														}));
	TRACE();
	countingAchievement();
	TRACE();
	
	
	CCSprite* n_event = CCSprite::create("mainflow_new_event.png");
	CCSprite* s_event = CCSprite::create("mainflow_new_event.png");
	s_event->setColor(ccGRAY);
	
	CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(PuzzleScene::menuAction));
	event_item->setTag(kPuzzleMenuTag_event);
	
	CCMenu* event_menu = CCMenu::createWithItem(event_item);
	event_menu->setPosition(ccp(458,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-20));
	addChild(event_menu, kPuzzleZorder_top);
	
	top_list.push_back(event_menu);
}

void PuzzleScene::countingMessage()
{
	TRACE();
	postbox_count_case->setVisible(false);
	
	vector<CommandParam> command_list;
	command_list.clear();
	
	Json::Value p;
	p["memberID"]=hspConnector::get()->getSocialID();
	// 0 이 아니면 해당하는 타입의 메시지가 들어옴.
	
	//USE GETMESSAGELIST
	command_list.push_back(CommandParam("checkgiftboxhistory",p,[=](Json::Value r)
										{
											TRACE();
											GraphDogLib::JsonToLog("checkgiftboxhistory", r);
											TRACE();
											if(r["result"]["code"].asInt() != GDSUCCESS)
											{
												TRACE();
												return;
											}
											
											int message_cnt = r.get("haveNewGiftCnt", 0).asInt();
											
											if(message_cnt > 0)
											{
												int t_count = message_cnt;
												int base_width = 20;
												while (t_count/10 > 0)
												{
													base_width+=5;
													t_count /= 10;
												}
												
												postbox_count_case->setContentSize(CCSizeMake(base_width, 20));
											}
											
											postbox_count_case->setVisible(message_cnt > 0);
											postbox_count_label->setString(CCString::createWithFormat("%d", message_cnt)->getCString());
											postbox_count_label->setPosition(ccpFromSize(postbox_count_case->getContentSize()/2.f));
											TRACE();
										}));
	
	Json::Value real_time_message_param;
	real_time_message_param["version"] = NSDS_GI(kSDS_GI_realTimeMessageVersion_i);
	command_list.push_back(CommandParam("getrealtimemessage", real_time_message_param, [=](Json::Value result_data)
										{
											if(result_data["result"]["code"].asInt() == GDSUCCESS)
											{
												NSDS_SI(kSDS_GI_realTimeMessageVersion_i, result_data["version"].asInt());
												
												float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
												
												RealTimeMessage* t_message = RealTimeMessage::create(result_data["message"].asString().c_str(), ccp(240,160+160*screen_scale_y));
												addChild(t_message, 99999999);
												
//												KSLabelTTF* real_message = KSLabelTTF::create(result_data["message"].asString().c_str(), mySGD->getFont().c_str(), 25);
//												real_message->setColor(ccc3(255, 100, 100));
//												real_message->enableOuterStroke(ccBLACK, 2.5f, 255, true);
//												real_message->setPosition(ccp(240,160));
//												addChild(real_message, 99999999);
//												
//												addChild(KSTimer::create(3.f, [=](){
//													addChild(KSGradualValue<float>::create(1.f, 0.f, 1.f, [=](float t)
//																						   {
//																							   real_message->setOpacity(255*t);
//																						   }, [=](float t)
//																						   {
//																							   real_message->setOpacity(255*t);
//																							   real_message->removeFromParent();
//																						   }));}));
											}
										}));
	
	myHSP->command(command_list, -1);
	
    TRACE();
}

void PuzzleScene::countingAchievement()
{
	achievement_count_case->setVisible(false);
	int reward_count = 0;
	
	for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
	{
		if(!myAchieve->isCompleted((AchievementCode)i) &&
		   myAchieve->isAchieve((AchievementCode)i))
		{
			reward_count++;
		}
	}
	
	for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
	{
		if(!myAchieve->isCompleted((AchievementCode)i) &&
		   myAchieve->isAchieve((AchievementCode)i))
		{
			reward_count++;
		}
	}
	
	if(reward_count > 0)
	{
		int t_count = reward_count;
		int base_width = 20;
		while (t_count/10 > 0)
		{
			base_width+=5;
			t_count /= 10;
		}
		
		achievement_count_case->setContentSize(CCSizeMake(base_width, 20));
		
		achievement_count_case->setVisible(true);
		achievement_count_label->setString(CCString::createWithFormat("%d", reward_count)->getCString());
	}
	
	achievement_count_label->setPosition(ccp(achievement_count_case->getContentSize().width/2.f, achievement_count_case->getContentSize().height/2.f + 0));
}

void PuzzleScene::achievePopupClose()
{
	countingAchievement();
	is_menu_enable = true;
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
	
	CommonButton::callBackKey();
	//AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(PuzzleScene::alertAction));
//	onBackKeyAction();
}