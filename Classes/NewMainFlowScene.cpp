//
//  NewMainFlowScene.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 2. 7..
//
//

#include "NewMainFlowScene.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "ASPopupView.h"
#include "NoticeContent.h"
#include "ClearPopup.h"
#include "FailPopup.h"
#include "StageImgLoader.h"
#include "ScrollMenu.h"
#include "AlertEngine.h"
#include "ShopPopup.h"
#include "TouchSuctionLayer.h"
#include "MailPopup.h"
#include "OptionPopup.h"
#include "RankPopup.h"
#include "CardSettingPopup.h"
#include "InviteEventPopup.h"
#include "GachaPurchase.h"
#include "AchievePopup.h"
#include "NewStageInfoView.h"
#include "NewPuzzlePiece.h"
#include "CountingBMLabel.h"
#include "HeartTime.h"
#include "MyLocalization.h"
#include "CCMenuLambda.h"
#include "StartSettingScene.h"
#include "StageInfoDown.h"
#include "EventListDown.h"
#include "MainFlowScene.h"
#include "StageListDown.h"
#include "NewPieceManager.h"
#include "BeautyStoneSetting.h"
#include "PuzzleListShadow.h"
#include "StoryManager.h"
#include "GraySprite.h"
#include "StageNode.h"

CCScene* NewMainFlowScene::scene()
{
    CCScene *scene = CCScene::create();
    
    NewMainFlowScene *layer = NewMainFlowScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer);
	
    return scene;
}

bool NewMainFlowScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	warp_ani_manager.clear();
	stage_node_manager.clear();
	
	setKeypadEnabled(true);
	
	unlock_cover = NULL;
	
	clear_is_first_puzzle_success = false;
	clear_is_first_perfect = false;
	
	int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	for(int i=1;i<=puzzle_count;i++)
	{
		int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
		int have_card_cnt = 0;
		int start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
		int stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
		
		int card_take_cnt = mySGD->getHasGottenCardsSize();
		for(int i=0;i<card_take_cnt;i++)
		{
			int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
			int card_stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
			if(card_stage_number >= start_stage && card_stage_number < start_stage+stage_count)
				have_card_cnt++;
		}
		
		have_card_count_for_puzzle_index.push_back(have_card_cnt);
		
		bool is_perfect_puzzle = !mySGD->getPuzzleHistory(t_puzzle_number).is_perfect.getV();
		
		vector<PuzzlePiecePath> puzzle_path_info;
		for(int j = start_stage;j<start_stage + stage_count;j++)
		{
			PuzzlePiecePath t_path;
			t_path.piece_no = NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, j);
			t_path.stage_no = j;
			if(j == start_stage+stage_count-1)
				t_path.next_stage_no = -1;
			else
				t_path.next_stage_no = j+1;
			puzzle_path_info.push_back(t_path);
			
			if(is_perfect_puzzle)
			{
				for(int k=3;k>=1;k--)
				{
					int t_card_number = NSDS_GI(j, kSDS_SI_level_int1_card_i, k);
					if(t_card_number <= 0)
					{
						is_perfect_puzzle = false;
						break;
					}
					
					if(mySGD->isHasGottenCards(t_card_number) <= 0)
					{
						is_perfect_puzzle = false;
						break;
					}
				}
			}
		}
		
		PuzzleHistory t_history = mySGD->getPuzzleHistory(t_puzzle_number);
		bool is_changed_history = false;
		
		if(is_perfect_puzzle)
		{
			clear_is_first_perfect = true;
			
			is_changed_history = true;
			t_history.is_perfect = true;
		}
		
		struct t_PuzzlePiecePath{
			bool operator() (const PuzzlePiecePath& a, const PuzzlePiecePath& b)
			{
				return a.piece_no < b.piece_no;
			}
		} pred;
		
		sort(puzzle_path_info.begin(), puzzle_path_info.end(), pred);
		puzzle_piece_path[i] = puzzle_path_info;
		puzzle_piece_mode.push_back((int)kNewPuzzlePieceMode_default);
		
		
		if(!mySGD->getPuzzleHistory(t_puzzle_number).is_clear.getV())
		{
			bool is_all_clear_stage = true;
			for(int j=start_stage;j<start_stage+stage_count && is_all_clear_stage;j++)
			{
				if(!mySGD->isClearPiece(j))
					is_all_clear_stage = false;
			}
			
			if(is_all_clear_stage)
			{
				clear_is_first_puzzle_success = true;
				
				is_changed_history = true;
				t_history.is_clear = true;
				
				int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
				int next_puzzle_number = -1;
				for(int k=1;k<=puzzle_count;k++)
				{
					int tt_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, k);
					if(tt_puzzle_number == t_puzzle_number)
					{
						if(k+1 <= puzzle_count)
						{
							next_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, k+1);
							break;
						}
					}
				}
				
				if(next_puzzle_number != -1 && NSDS_GI(next_puzzle_number, kSDS_PZ_point_i) == 0 && NSDS_GI(next_puzzle_number, kSDS_PZ_ticket_i) == 0)
				{
					PuzzleHistory next_history = mySGD->getPuzzleHistory(next_puzzle_number);
					next_history.is_open = true;
					next_history.open_type = "무료(이전퍼즐완료)";
					mySGD->setPuzzleHistory(next_history, nullptr);
				}
			}
		}
		
		if(is_changed_history)
			mySGD->setPuzzleHistory(t_history, nullptr);
	}
	
	
	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kNewMainFlowZorder_back);
	
//	is_unlock_puzzle = mySGD->getIsUnlockPuzzle();
//	mySGD->setIsUnlockPuzzle(0);
	
	
	bool is_suitable_stage = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)) == mySGD->suitable_stage;
	
	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000 || myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_init || is_suitable_stage)
	{
		int history_count = mySGD->getPuzzleHistorySize();
		int open_puzzle_count = 0;
		for(int i=0;i<history_count;i++)
			if(mySGD->getPuzzleHistoryForIndex(i).is_open.getV())
				open_puzzle_count++;
		
		int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, open_puzzle_count);
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, t_puzzle_number);
		
		int t_puzzle_start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
		int t_puzzle_stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
		int last_stage_number = -1; // 플레이 할 수 있는
		for(int i = t_puzzle_start_stage;i<t_puzzle_start_stage+t_puzzle_stage_count && last_stage_number == -1;i++)
		{
			int stage_number = i;
			if((stage_number == 1 || mySGD->getPieceHistory(stage_number).is_open.getV() ||
			   (NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
				(NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 || mySGD->isClearPiece(NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number))))) && (i+1 == t_puzzle_start_stage+t_puzzle_stage_count ||
					(!((i+1 == 1 || mySGD->getPieceHistory(i+1).is_open.getV() ||
				   (NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, i+1) == 0 &&
					(NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i+1) == 0 ||
					 mySGD->isClearPiece(NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i+1)))))))))
			{
				last_stage_number = stage_number;
			}
		}
		
		if(last_stage_number == -1)
			last_stage_number = t_puzzle_start_stage;
		
		mySGD->suitable_stage = last_stage_number;
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, t_puzzle_number, last_stage_number);
	}
	
	selected_puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	if(selected_puzzle_number == 0)
	{
		selected_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, 1);
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, selected_puzzle_number);
	}
	selected_stage_number = myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, selected_puzzle_number);
	
	
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	CCSprite* back_shadow_left = CCSprite::create("mainflow_back_shadow_left.png");
	back_shadow_left->setAnchorPoint(ccp(0.f,0.5f));
	back_shadow_left->setPosition(ccp(-(screen_scale_x-1.f)*480.f/2.f,160));
	addChild(back_shadow_left, kNewMainFlowZorder_top);
	
	CCSprite* back_shadow_right = CCSprite::create("mainflow_back_shadow_right.png");
	back_shadow_right->setAnchorPoint(ccp(1.f,0.5f));
	back_shadow_right->setPosition(ccp(480+(screen_scale_x-1.f)*480.f/2.f,160));
	addChild(back_shadow_right, kNewMainFlowZorder_top);
	
	setTop();
	setBottom();
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_init) // 실행 후 첫 접근시
	{
		if(mySGD->getMustBeShowNotice())
		{
			ASPopupView* t_popup = ASPopupView::create(-200);
			
			float height_value = 320.f;
			if(myDSH->screen_convert_rate < 1.f)
				height_value = 320.f/myDSH->screen_convert_rate;
			
			if(height_value < myDSH->ui_top)
				height_value = myDSH->ui_top;
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
			t_popup->setDimmedPosition(ccp(240, 160));
			t_popup->setBasePosition(ccp(240, 160));
			
			NoticeContent* t_container = NoticeContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
															   {
																   t_popup->removeFromParent();
															   }, mySGD->getNoticeList());
			t_popup->setContainerNode(t_container);
			addChild(t_popup, kNewMainFlowZorder_popup);
		}
		
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		
		if(myDSH->getIntegerForKey(kDSH_Key_storyReadPoint) == 1)
		{
			character_img->setVisible(false);
			StoryManager* t_sm = StoryManager::create(-500);
			addChild(t_sm, kNewMainFlowZorder_popup);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			CCSprite* gray = CCSprite::create("back_gray.png");
			gray->setOpacity(0);
			gray->setPosition(ccp(0,0));
			gray->setScaleX(screen_scale_x);
			gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
			gray->runAction(CCFadeTo::create(0.5f, 255));
			t_sm->back_node->addChild(gray);
			
			CCSprite* ellebere = CCSprite::create("talk_char_dwarf.png");
			ellebere->setAnchorPoint(ccp(0.7f, 0));
			ellebere->setFlipX(true);
			ellebere->setPosition(ccp(100,0));
			ellebere->runAction(CCMoveTo::create(0.5f, ccp(0,0)));
			t_sm->right_node->addChild(ellebere);
			
			CCSprite* kei = CCSprite::create("talk_char_hero.png");
			kei->setAnchorPoint(ccp(0.2f,0));
			kei->setPosition(ccp(-100,0));
			kei->setVisible(false);
			t_sm->left_node->addChild(kei);
			
			CCSprite* snow = CCSprite::create("talk_char_princess.png");
			snow->setAnchorPoint(ccp(0.8f,0));
			snow->setFlipX(true);
			snow->setPosition(ccp(100,0));
			snow->setVisible(false);
			t_sm->right_node->addChild(snow);
		
			t_sm->addMent(false, "엘레베르", "talk_nametag_red.png", "이봐.. 일어나.. 일어나보라구..", [=]()
			{
			ellebere->runAction(CCScaleTo::create(0.2f, 0.7f));
			kei->setVisible(true);
			kei->runAction(CCMoveTo::create(0.5f, ccp(0,0)));
						  
			t_sm->addMent(true, myDSH->getStringForKey(kDSH_Key_nick), "talk_nametag_green.png", "음냐... 음.. 누.. 누구?", [=]()
			{
				kei->runAction(CCScaleTo::create(0.2f, 0.7f));
				ellebere->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.3f, ccp(100,0)), CCHide::create()));
											
				snow->setVisible(true);
				snow->runAction(CCMoveTo::create(0.5f, ccp(0,0)));
											
				t_sm->addMent(false, "스노우", "talk_nametag_red.png", "이제서야 깨어났군!!\n당신때문에 우리엄마가 마녀에게 납치당해버렸잖아!!", [=]()
				{
					snow->runAction(CCScaleTo::create(0.2f, 0.7f));
					kei->runAction(CCScaleTo::create(0.2f, 1.f));
															  
					t_sm->addMent(true, myDSH->getStringForKey(kDSH_Key_nick), "talk_nametag_green.png", "뭐? 마녀?\n무슨 이야기인지 하나도 모르겠네..\n여긴 어디지?", [=]()
					{
						kei->runAction(CCScaleTo::create(0.2f, 0.7f));
						snow->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.3f, ccp(100,0)), CCHide::create()));
						ellebere->setVisible(true);
						ellebere->runAction(CCSpawn::createWithTwoActions(CCScaleTo::create(0.2f, 1.f), CCMoveTo::create(0.3f, ccp(0,0))));
																				
						t_sm->addMent(false, "엘레베르", "talk_nametag_red.png", "아직 정신을 못차렸나보군. 이곳은 화이트왕국이라네.\n아름다운 소나무들이 자라는 곳으로 유명하지.\n하지만 사흘전 갑자기 나타난 마녀가 우리 왕비마마를 납치해간 이후\n왕국 곳곳엔 악령들이 득실되고 있어.", [=]()
						{
							t_sm->addMent(false, "엘레베르", "talk_nametag_red.png", "(계속)\n이 사태를 해결하지 않으면 왕국자체가\n악의 소굴로 변할 태세라네.\n모든일이 자네가 마녀의 봉인을 해제하면서 일어난 일이라네.", [=]()
							{
								ellebere->runAction(CCScaleTo::create(0.2f, 0.7f));
								kei->runAction(CCScaleTo::create(0.2f, 1.f));
								t_sm->addMent(true, myDSH->getStringForKey(kDSH_Key_nick), "talk_nametag_green.png", "아... 생각났다.. 그때 그 이상한 책을 열고..\n그래! 어떤 여자를 봤어요. 무서운 눈매의...\n도무지 알수가... 이해하기 힘든 일들이...", [=]()
								{
									kei->runAction(CCScaleTo::create(0.2f, 0.7f));
								t_sm->addMent(false, "악령", "talk_nametag_red.png", "쿠오오오아아앙", [=]()
								{
								  ellebere->runAction(CCScaleTo::create(0.2f, 1.f));
								  t_sm->addMent(false, "엘레베르", "talk_nametag_red.png", "이런 또 악령이 나타났구만.\n일단 악령을 처치한 후 이야기를 하지.\n악령이 있는곳엔 반드시 그림카드가 숨겨져있다네.\n악령을 피해 그걸 찾아야해! 이 마법붓을 이용하게!", [=]()
								  {
									  myDSH->setIntegerForKey(kDSH_Key_storyReadPoint, 2);
//									  myDSH->saveAllUserData(nullptr);
									  character_img->setVisible(true);
									t_sm->removeFromParent();
								  });
								});
								});
							});
						});
					});
				});
			});
			});
		}

	}
	
	is_menu_enable = true;
	
	
	clear_is_stage_unlock = false;
	clear_star_take_level = 1;
	clear_is_empty_star = false;
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
//		myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
		
		if(mySGD->getIsNotClearedStage())
		{
			int played_puzzle_number = NSDS_GI(mySD->getSilType(), kSDS_SI_puzzle_i);
			int start_stage = NSDS_GI(played_puzzle_number, kSDS_PZ_startStage_i);
			int stage_count = NSDS_GI(played_puzzle_number, kSDS_PZ_stageCount_i);
			
			for(int i=start_stage;i<start_stage+stage_count;i++)
			{
				if(NSDS_GI(played_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i) == mySD->getSilType())
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
//		clear_is_empty_star = take_level > mySGD->ingame_before_stage_rank;
		
		if(mySGD->isHasGottenCards(mySD->getSilType(), take_level) == 0)
		{
//			mySGD->setClearRewardGold(NSDS_GI(kSDS_CI_int1_reward_i, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level)));
			
			mySGD->addHasGottenCardNumber(NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
		}
		else
		{

		}
		
		if(myDSH->getIntegerForKey(kDSH_Key_selectedCard) == 0)
			myDSH->setIntegerForKey(kDSH_Key_selectedCard, NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_level));
	}
	
	bool is_clear_popup = false;
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		showClearPopup();
		is_clear_popup = true;
	}
	else if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_fail)
	{
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
		showFailPopup();
	}
	else
	{
//		if(is_unlock_puzzle > 0)
//		{
//			is_menu_enable = false;
//			puzzle_table->setTouchEnabled(false);
//		}
	}
	
	if(clear_is_first_puzzle_success || (!clear_is_first_perfect && clear_is_stage_unlock))
	{
		selected_stage_number = mySD->getSilType();
		selected_puzzle_number = NSDS_GI(selected_stage_number, kSDS_SI_puzzle_i);
	}
	
	setTable();
	
//	new_stage_info_view = NewStageInfoView::create(-150, std::bind(&NewMainFlowScene::changeButtonChallenge, this, std::placeholders::_1, std::placeholders::_2));
//	addChild(new_stage_info_view, kNewMainFlowZorder_right);
//	new_stage_info_view->setVisible(false);
	
	pieceAction(selected_stage_number);
	
	if(is_clear_popup)
	{
		puzzle_table->setTouchEnabled(false);
	}
	
	return true;
}

void NewMainFlowScene::showClearPopup()
{
	is_menu_enable = false;
	
	ClearPopup* t_popup = ClearPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::hideClearPopup));
	addChild(t_popup, kNewMainFlowZorder_popup);
}

void NewMainFlowScene::hideClearPopup()
{
	if(myDSH->getIntegerForKey(kDSH_Key_storyReadPoint) == 2 && mySD->getSilType() == 1)
	{
		character_img->setVisible(false);
		StoryManager* t_sm = StoryManager::create(-500);
		addChild(t_sm, kNewMainFlowZorder_popup);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(0,0));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		gray->runAction(CCFadeTo::create(0.5f, 255));
		t_sm->back_node->addChild(gray);
		
		CCSprite* ellebere_left = CCSprite::create("talk_char_dwarf.png");
		ellebere_left->setAnchorPoint(ccp(0.3f, 0));
		ellebere_left->setPosition(ccp(-100,0));
		ellebere_left->setVisible(false);
		t_sm->left_node->addChild(ellebere_left);
		
		CCSprite* ellebere_right = CCSprite::create("talk_char_dwarf.png");
		ellebere_right->setAnchorPoint(ccp(0.7f,0));
		ellebere_right->setFlipX(true);
		ellebere_right->setPosition(ccp(100,0));
		ellebere_right->setVisible(false);
		t_sm->right_node->addChild(ellebere_right);
		
		CCSprite* kei = CCSprite::create("talk_char_hero.png");
		kei->setAnchorPoint(ccp(0.2f,0));
		kei->setPosition(ccp(-100,0));
		kei->setVisible(false);
		t_sm->left_node->addChild(kei);
		
		CCSprite* snow = CCSprite::create("talk_char_princess.png");
		snow->setAnchorPoint(ccp(0.8f,0));
		snow->setFlipX(true);
		snow->setPosition(ccp(100,0));
		snow->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
		t_sm->right_node->addChild(snow);
		
		t_sm->addMent(false, "스노우", "talk_nametag_red.png", "여기에 또 그림이...\n이건 분명 월터가 대장장이의 딸인데..\n신기하다 그림이 움직여..", [=]()
		{
			snow->runAction(CCScaleTo::create(0.2f, 0.7f));
			ellebere_left->setVisible(true);
			ellebere_left->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
						  
			t_sm->addMent(true, "엘레베르", "talk_nametag_red.png", "역시 이 그림에도 상당한 마법에너지가 느껴지는군요.\n마녀가 이 사람을 그림에 가둔것 같습니다.\n악령들은 이 그림에 기생해 마법에너지를 먹으며\n사람들에게 피해를 주는 것이죠.", [=]()
			{
				ellebere_left->runAction(CCScaleTo::create(0.2f, 0.7f));
				snow->runAction(CCScaleTo::create(0.2f, 1.f));
											
				t_sm->addMent(false, "스노우", "talk_nametag_red.png", "이건 마녀의 짓이 분명해. 의도적으로 왕국의 곳곳에 그림을 숨겨놓고\n악령들을 불러모으고 있나봐. 어서 마녀를 잡아야해.", [=]()
				{
					snow->runAction(CCScaleTo::create(0.2f, 0.7f));
					kei->setVisible(true);
					kei->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
					ellebere_left->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.3f, ccp(-100,0)), CCHide::create()));
															  
					t_sm->addMent(true, myDSH->getStringForKey(kDSH_Key_nick), "talk_nametag_green.png", "저.. 저는 어떻게 해야 하죠?\n집으로 가는 길이 어떻게 되는건지..", [=]()
					{
						kei->runAction(CCScaleTo::create(0.2f, 0.7f));
						ellebere_right->setVisible(true);
						ellebere_right->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
						snow->runAction(CCSequence::createWithTwoActions(CCMoveTo::create(0.3f, ccp(100,0)), CCHide::create()));
																				
						t_sm->addMent(false, "엘레베르", "talk_nametag_red.png", "돌아가려면 마녀를 찾아야해.\n그전에 그림을 모두 회수 해야하고...    \n늦었어. 바로 출발하지.", [=]()
						{
							character_img->setVisible(true);
							myDSH->setIntegerForKey(kDSH_Key_storyReadPoint, 3);
//							myDSH->saveAllUserData(nullptr);
							t_sm->removeFromParent();
						});
					});
				});
			});
		});
	}
	else if(myDSH->getIntegerForKey(kDSH_Key_storyReadPoint) == 3 && mySD->getSilType() == 2)
	{
		character_img->setVisible(false);
		StoryManager* t_sm = StoryManager::create(-500);
		addChild(t_sm, kNewMainFlowZorder_popup);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(0,0));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		gray->runAction(CCFadeTo::create(0.5f, 255));
		t_sm->back_node->addChild(gray);
		
		CCSprite* ellebere_right = CCSprite::create("talk_char_dwarf.png");
		ellebere_right->setAnchorPoint(ccp(0.7f,0));
		ellebere_right->setFlipX(true);
		ellebere_right->setPosition(ccp(100,0));
		ellebere_right->setVisible(false);
		t_sm->right_node->addChild(ellebere_right);
		
		CCSprite* kei = CCSprite::create("talk_char_hero.png");
		kei->setAnchorPoint(ccp(0.2f,0));
		kei->setPosition(ccp(-100,0));
		kei->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
		t_sm->left_node->addChild(kei);
		
		CCSprite* snow = CCSprite::create("talk_char_princess.png");
		snow->setAnchorPoint(ccp(0.8f,0));
		snow->setFlipX(true);
		snow->setPosition(ccp(100,0));
		snow->setVisible(false);
		t_sm->right_node->addChild(snow);
		
		t_sm->addMent(true, myDSH->getStringForKey(kDSH_Key_nick), "talk_nametag_green.png", "여기 이상한 보석이 떨어졌어요. 이건 뭐죠?", [=]()
		{
			kei->runAction(CCScaleTo::create(0.2f, 0.7f));
			snow->setVisible(true);
			snow->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
						  
			t_sm->addMent(false, "스노우", "talk_nametag_red.png", "우와!! 예쁘다!! 이거 나줘!!!", [=]()
			{
				snow->runAction(CCSequence::createWithTwoActions(CCSpawn::createWithTwoActions(CCMoveTo::create(0.3f, ccp(100,0)), CCScaleTo::create(0.3f, 0.7f)), CCHide::create()));
				ellebere_right->setVisible(true);
				ellebere_right->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
											
				t_sm->addMent(false, "엘레베르", "talk_nametag_red.png", "이건 뷰티스톤이라 부르는 마법의 돌이라네.\n가끔 악령들에게서 얻을 수 있는 것이지.\n무기에 장착해보게. 악령을 퇴치할때 요긴하게 사용할 수 있을게야.", [=]()
				{
					character_img->setVisible(true);
					myDSH->setIntegerForKey(kDSH_Key_storyReadPoint, 4);
//					myDSH->saveAllUserData(nullptr);
					t_sm->removeFromParent();
				});
			});
		});
	}
	else if(myDSH->getIntegerForKey(kDSH_Key_storyReadPoint) == 4 && mySD->getSilType() == 4)
	{
		character_img->setVisible(false);
		StoryManager* t_sm = StoryManager::create(-500);
		addChild(t_sm, kNewMainFlowZorder_popup);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setOpacity(0);
		gray->setPosition(ccp(0,0));
		gray->setScaleX(screen_scale_x);
		gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		gray->runAction(CCFadeTo::create(0.5f, 255));
		t_sm->back_node->addChild(gray);
		
		CCSprite* ellebere_right = CCSprite::create("talk_char_dwarf.png");
		ellebere_right->setAnchorPoint(ccp(0.7f,0));
		ellebere_right->setFlipX(true);
		ellebere_right->setPosition(ccp(100,0));
		ellebere_right->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
		t_sm->right_node->addChild(ellebere_right);
		
		CCSprite* kei = CCSprite::create("talk_char_hero.png");
		kei->setAnchorPoint(ccp(0.2f,0));
		kei->setPosition(ccp(-100,0));
		kei->setVisible(false);
		t_sm->left_node->addChild(kei);
		
		t_sm->addMent(false, "엘레베르", "talk_nametag_red.png", "점점 악령이 강해지는구만.\n더 강한 뷰티스톤을 만들어야겠어.", [=]()
		{
			ellebere_right->runAction(CCScaleTo::create(0.2f, 0.7f));
			kei->setVisible(true);
			kei->runAction(CCMoveTo::create(0.3f, ccp(0,0)));
						  
			t_sm->addMent(true, myDSH->getStringForKey(kDSH_Key_nick), "talk_nametag_green.png", "뷰티스톤을 더 강하게 만들수도 있나요?", [=]()
			{
				kei->runAction(CCScaleTo::create(0.2f, 0.7f));
				ellebere_right->runAction(CCScaleTo::create(0.2f, 1.f));
											
				t_sm->addMent(false, "엘레베르", "talk_nametag_red.png", "뷰티스톤은 다른 뷰티스톤으로 강화할 수 있다네.\n강화를 하면 뷰티스톤의 레벨이 올라가고 더 강력해지지.", [=]()
				{
					character_img->setVisible(true);
					myDSH->setIntegerForKey(kDSH_Key_storyReadPoint, 5);
//					myDSH->saveAllUserData(nullptr);
					t_sm->removeFromParent();
				});
			});
		});
	}
	
//	is_menu_enable = true;
	int get_puzzle_number = NSDS_GI(mySD->getSilType(), kSDS_SI_puzzle_i);
	int history_count = mySGD->getPuzzleHistorySize();
	int open_puzzle_count = 0;
	for(int i=0;i<history_count;i++)
		if(mySGD->getPuzzleHistoryForIndex(i).is_open.getV())
			open_puzzle_count++;
	
	if(clear_is_first_puzzle_success)
		open_puzzle_count--;
	bool is_found = false;
	clear_found_puzzle_idx = -1;
	for(int i=0;i<open_puzzle_count && !is_found;i++)
	{
		if(get_puzzle_number == NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1))
		{
			is_found = true;
			clear_found_puzzle_idx = i;
		}
	}
	
	if(clear_is_empty_star)
		showGetStar();
	else
	{
		if(clear_is_stage_unlock)
			showUnlockEffect();
		else
		{
			is_menu_enable = true;
			puzzle_table->setTouchEnabled(true);
		}
	}
}

//void NewMainFlowScene::showGetPuzzle()
//{
//	CCLOG("get piece animation");
//	CCTableViewCell* t_cell = puzzle_table->cellAtIndex(clear_found_puzzle_idx);
//	
//	if(t_cell)
//	{
//		CCNode* t_puzzle_node = t_cell->getChildByTag(1);
//		
//		CCSprite* get_piece_title = CCSprite::create("get_piece_title.png");
//		NewPuzzlePiece* new_piece = (NewPuzzlePiece*)t_puzzle_node->getChildByTag(mySD->getSilType());
//		get_piece_title->setPosition(ccpAdd(new_piece->getPosition(), ccp(0, 45)));
//		t_puzzle_node->addChild(get_piece_title);
//		
//		new_piece->startGetPieceAnimation([=](CCPoint t_point)
//										  {
//											  createGetPuzzleParticle(t_puzzle_node, t_point);
//										  });
//		
//		CCDelayTime* t_delay = CCDelayTime::create(1.f);
//		CCFadeTo* t_fade = CCFadeTo::create(1.f, 0);
//		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(NewMainFlowScene::endGetPuzzle));
//		CCCallFunc* t_call2 = CCCallFunc::create(get_piece_title, callfunc_selector(CCSprite::removeFromParent));
//		CCSequence* t_seq = CCSequence::create(t_delay, t_fade, t_call1, t_call2, NULL);
//		get_piece_title->runAction(t_seq);
//	}
//	else
//	{
//		endGetPuzzle();
//	}
//}
//
//void NewMainFlowScene::createGetPuzzleParticle(CCNode* t_node, CCPoint t_point)
//{
//	random_device rd;
//	default_random_engine e1(rd());
//	uniform_real_distribution<float> uniform_dist(-50, 50);
//	
//	CCPoint random_value;
//	random_value.x = uniform_dist(e1);
//	random_value.y = uniform_dist(e1);
//	
//	CCParticleSystemQuad* t_particle = CCParticleSystemQuad::createWithTotalParticles(150);
//	t_particle->setPositionType(kCCPositionTypeRelative);
//	t_particle->setTexture(CCTextureCache::sharedTextureCache()->addImage("get_piece_particle.png"));
//	t_particle->setEmissionRate(400);
//	t_particle->setAngle(90.0);
//	t_particle->setAngleVar(45.0);
//	ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE};
//	t_particle->setBlendFunc(blendFunc);
//	t_particle->setDuration(0.1);
//	t_particle->setEmitterMode(kCCParticleModeGravity);
//	t_particle->setStartColor(ccc4f(1.f, 1.f, 1.f, 1.f));
//	t_particle->setStartColorVar(ccc4f(0.57f, 0.57f, 0.54f, 0.f));
//	t_particle->setEndColor(ccc4f(1.f, 1.f, 1.f, 0.f));
//	t_particle->setEndColorVar(ccc4f(0.f, 0.f, 0.f, 0.f));
//	t_particle->setStartSize(10.0);
//	t_particle->setStartSizeVar(5.0);
//	t_particle->setEndSize(20.0);
//	t_particle->setEndSizeVar(5.0);
//	t_particle->setGravity(ccp(0,-400));
//	t_particle->setRadialAccel(0.0);
//	t_particle->setRadialAccelVar(0.0);
//	t_particle->setSpeed(150);
//	t_particle->setSpeedVar(70.0);
//	t_particle->setTangentialAccel(0);
//	t_particle->setTangentialAccelVar(0);
//	t_particle->setTotalParticles(150);
//	t_particle->setLife(0.40);
//	t_particle->setLifeVar(0.5);
//	t_particle->setStartSpin(0);
//	t_particle->setStartSpinVar(180);
//	t_particle->setEndSpin(0);
//	t_particle->setEndSpinVar(180);
//	t_particle->setPosVar(ccp(10,10));
//	t_particle->setPosition(ccpAdd(t_point, random_value));
//	t_particle->setAutoRemoveOnFinish(true);
//	t_node->addChild(t_particle);
//}
//
//void NewMainFlowScene::endGetPuzzle()
//{
//	showGetStar();
//}

void NewMainFlowScene::showGetStar()
{
	CCLOG("get star animation : %d", mySD->getSilType());
	
	map<int, CCNode*>::iterator iter = stage_node_manager.find(mySD->getSilType());
	if(iter == stage_node_manager.end() || iter->second == NULL)
	{
		endGetStar();
	}
	else
	{
		((StageNode*)(iter->second))->startGetStarAnimation([=](){endGetStar();});
	}
	
//	CCTableViewCell* t_cell = puzzle_table->cellAtIndex(clear_found_puzzle_idx);
//	
//	if(t_cell)
//	{
//		CCNode* t_puzzle_node = t_cell->getChildByTag(1);
//		
//		NewPuzzlePiece* new_piece = (NewPuzzlePiece*)t_puzzle_node->getChildByTag(mySD->getSilType());
//		new_piece->startGetStarAnimation(clear_star_take_level, [=]()
//										 {
//											 endGetStar();
//										 });
//	}
//	else
//	{
//		endGetStar();
//	}
}

void NewMainFlowScene::endGetStar()
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
				is_menu_enable = true;
				puzzle_table->setTouchEnabled(true);
			}
		}
	}
}

void NewMainFlowScene::showSuccessPuzzleEffect()
{
	int t_puzzle_number = NSDS_GI(mySD->getSilType(), kSDS_SI_puzzle_i);
	CCLOG("success puzzle animation : %d", t_puzzle_number);
	
	map<int, CCBAnimationManager*>::iterator iter = warp_ani_manager.find(t_puzzle_number);
	if(iter != warp_ani_manager.end())
	{
		(*iter).second->runAnimationsForSequenceNamed("open");
	}
	
	puzzle_table->setContentOffsetInDuration(ccpAdd(puzzle_table->getContentOffset(), ccp(-cellSizeForTable(puzzle_table).width, 0)), 0.3f);
	CCDelayTime* t_delay = CCDelayTime::create(0.3f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(NewMainFlowScene::endSuccessPuzzleEffect));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
	runAction(t_seq);
}

void NewMainFlowScene::endSuccessPuzzleEffect()
{
	int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, clear_found_puzzle_idx+2);
	if(mySIL->addImage(CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, "original")->getCString()))
	{
		int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
		pieceAction(start_stage);
	}
	
	is_menu_enable = true;
	puzzle_table->setTouchEnabled(true);
//	mySGD->setIsUnlockPuzzle(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)+1);
//	CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
}

void NewMainFlowScene::showPerfectPuzzleEffect()
{
	CCLOG("perfect puzzle animation");
	
//	CCTableViewCell* t_cell = puzzle_table->cellAtIndex(clear_found_puzzle_idx);
//	
//	if(t_cell)
//	{
//		CCNode* t_puzzle_node = t_cell->getChildByTag(1);
//		
//		int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, clear_found_puzzle_idx);
//		
//		int start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
//		int stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
//		
//		CCLOG("start_stage : %d, stage_count : %d", start_stage, stage_count);
//		
//		for(int i=start_stage;i<start_stage+stage_count;i++)
//		{
//			NewPuzzlePiece* new_piece = (NewPuzzlePiece*)t_puzzle_node->getChildByTag(i);
//			new_piece->startGetPieceAnimation([=](CCPoint t_point)
//											  {
//												  createGetPuzzleParticle(t_puzzle_node, t_point);
//											  });
//		}
//		
//		CCDelayTime* t_delay = CCDelayTime::create(2.f);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(NewMainFlowScene::endPerfectPuzzleEffect));
//		CCSequence* t_seq = CCSequence::create(t_delay, t_call, NULL);
//		
//		runAction(t_seq);
//	}
//	else
//	{
		endPerfectPuzzleEffect();
//	}
}
void NewMainFlowScene::endPerfectPuzzleEffect()
{
	is_menu_enable = true;
	puzzle_table->setTouchEnabled(true);
}

void NewMainFlowScene::showUnlockEffect()
{
	CCLOG("unlock piece animation : %d", next_stage_number);
	
	map<int, CCNode*>::iterator iter = stage_node_manager.find(next_stage_number);
	if(iter == stage_node_manager.end() || iter->second == NULL)
	{
		endUnlockEffect();
	}
	else
	{
		((StageNode*)(iter->second))->startUnlockAnimation([=](){endUnlockEffect();});
	}
	
//	if(unlock_cover)
//	{
//		CCFadeTo* t_fade = CCFadeTo::create(0.5f, 0);
//		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(NewMainFlowScene::endUnlockEffect));
//		CCSequence* t_seq = CCSequence::createWithTwoActions(t_fade, t_call);
//		
//		unlock_cover->runAction(t_seq);
//	}
//	else
//	{
//		endUnlockEffect();
//	}
}

void NewMainFlowScene::endUnlockEffect()
{
	if(unlock_cover)
	{
		unlock_cover->removeFromParent();
		unlock_cover = NULL;
	}
	
	selected_stage_number = next_stage_number;
	selected_puzzle_number = NSDS_GI(next_stage_number, kSDS_SI_puzzle_i);
	next_stage_number = -1;
	clear_is_stage_unlock = false;
	
	pieceAction(selected_stage_number);
	
	is_menu_enable = true;
	puzzle_table->setTouchEnabled(true);
}

void NewMainFlowScene::showFailPopup()
{
	is_menu_enable = false;
	
	FailPopup* t_popup = FailPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::hideFailPopup));
	addChild(t_popup, kNewMainFlowZorder_popup);
}

void NewMainFlowScene::hideFailPopup()
{
	is_menu_enable = true;
}

void NewMainFlowScene::setTable()
{
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	table_size = CCSizeMake(480*screen_scale_x, myDSH->ui_top/myDSH->screen_convert_rate); // 245
	CCPoint table_position = ccp((-480.f*screen_scale_x+480.f)/2.f,-(myDSH->ui_top/myDSH->screen_convert_rate-320)/2.f);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(table_position);
//	addChild(temp_back, kNewMainFlowZorder_table);
	
	puzzle_table = CCTableView::create(this, table_size);
	puzzle_table->setAnchorPoint(CCPointZero);
	puzzle_table->setDirection(kCCScrollViewDirectionHorizontal);
	puzzle_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	puzzle_table->setPosition(table_position);//table_size.height/2.f));
	puzzle_table->setDelegate(this);
	addChild(puzzle_table, kNewMainFlowZorder_table);
	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
	
	int puzzle_number;
	
//	if(is_unlock_puzzle > 0)
//		puzzle_number = is_unlock_puzzle;
//	else
		puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	if(puzzle_number == 0)
	{
		puzzle_number = 1;
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, puzzle_number);
	}
	
	int myPosition = puzzle_table->minContainerOffset().x;
	for(int i=0; i<numberOfCellsInTableView(puzzle_table); i++)
	{
		if(puzzle_number == NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i+1))
		{
			myPosition = i;
			break;
		}
	}
	
	if(clear_is_first_puzzle_success)
		myPosition--;
	
	float xInitPosition = MAX(puzzle_table->minContainerOffset().x, -cellSizeForTable(puzzle_table).width*myPosition);
	xInitPosition = MIN(0, xInitPosition);
	
	puzzle_table->setContentOffset(ccp(xInitPosition, 0));
	
//	puzzle_table->setContentOffsetInDuration(ccp(xInitPosition, 0), 0.3f);
}

void NewMainFlowScene::cellAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	int tag = ((CCNode*)sender)->getTag();

	is_menu_enable = false;
	
	int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, tag);
	CCLOG("open puzzle : %d", puzzle_number);
	
	
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
	
	CCLabelTTF* title_label = CCLabelTTF::create("퍼즐 오픈", mySGD->getFont().c_str(), 20);
	title_label->setPosition(ccp(0, 102));
	t_container->addChild(title_label);
	
	CCLabelTTF* content_label = CCLabelTTF::create("다음 퍼즐을 열려면 티켓이 필요해요", mySGD->getFont().c_str(), 18);
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
	
	
	CommonButton* buy_button = CommonButton::create("", CommonButtonYellow);
	buy_button->setTouchPriority(t_popup->getTouchPriority()-1);
	buy_button->setBackgroundTypeForDisabled(CommonButtonPupple);
	buy_button->setSize(CCSizeMake(80, 45));
	buy_button->setPosition(ccp(-70,-95));
	buy_button->setPrice(PriceTypeRuby, NSDS_GI(puzzle_number, kSDS_PZ_point_i));
	buy_button->setFunction([=](CCObject* sender)
							{
//								mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - NSDS_GI(puzzle_number, kSDS_PZ_point_i));
								
								int history_count = mySGD->getPuzzleHistorySize();
								int open_puzzle_count = 0;
								for(int i=0;i<history_count;i++)
									if(mySGD->getPuzzleHistoryForIndex(i).is_open.getV())
										open_puzzle_count++;
								
								int tt_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, open_puzzle_count+1);
								PuzzleHistory t_history = mySGD->getPuzzleHistory(tt_puzzle_number);
								t_history.is_open = true;
								t_history.open_type = "젬소모";
								mySGD->setPuzzleHistory(t_history, nullptr);
								
								vector<SaveUserData_Key> save_userdata_list;
								
								save_userdata_list.push_back(kSaveUserData_Key_star);
								
//								myDSH->saveUserData(save_userdata_list, nullptr);
								
								puzzle_table->updateCellAtIndex(tag);
								puzzle_table->updateCellAtIndex(tag-1);
								
								is_menu_enable = true;
								t_popup->removeFromParent();
							});
	
	t_container->addChild(buy_button);
	
	if(mySGD->getGoodsValue(kGoodsType_ruby) < NSDS_GI(puzzle_number, kSDS_PZ_point_i))
		buy_button->setEnabled(false);
	
	CommonButton* ticket_button = CommonButton::create("티켓 요청", CommonButtonYellow);
	ticket_button->setTouchPriority(t_popup->getTouchPriority()-1);
	ticket_button->setBackgroundTypeForDisabled(CommonButtonPupple);
	ticket_button->setContentSize(CCSizeMake(80, 45));
	ticket_button->setPosition(ccp(70,-95));
	ticket_button->setFunction([=](CCObject* sender)
							{
								is_menu_enable = true;
								t_popup->removeFromParent();
								
//								mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - NSDS_GI(puzzle_number, kSDS_PZ_point_i));
//								myDSH->setIntegerForKey(kDSH_Key_openPuzzleCnt, myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1);
//								
//								vector<SaveUserData_Key> save_userdata_list;
//								
//								save_userdata_list.push_back(kSaveUserData_Key_star);
//								save_userdata_list.push_back(kSaveUserData_Key_openPuzzle);
//								
//								myDSH->saveUserData(save_userdata_list, nullptr);
//								
//								int found_idx = -1;
//								for(int i=0;i<numberOfCellsInTableView(puzzle_table) && found_idx == -1;i++)
//								{
//									CCTableViewCell* t_cell = puzzle_table->cellAtIndex(i);
//									if(t_cell)
//									{
//										int cell_card_number = t_cell->getTag();
//										if(cell_card_number == puzzle_number)
//											found_idx = i;
//									}
//								}
//								if(found_idx != -1)
//									puzzle_table->updateCellAtIndex(found_idx);
//								
//								is_menu_enable = true;
//								t_popup->removeFromParent();
							});
	
	t_container->addChild(ticket_button);
	
	
//	if(tag < kMainFlowTableCellTag_buyBase)
//	{
//		int puzzle_number = tag - kMainFlowTableCellTag_openBase;
//		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, puzzle_number);
//		
//		StageListDown* t_sld = StageListDown::create(this, callfunc_selector(MainFlowScene::puzzleLoadSuccess), puzzle_number);
//		addChild(t_sld, kMainFlowZorder_popup);
//	}
//	else if(tag < kMainFlowTableCellTag_ticketBase) // buyBase
//	{
//		int puzzle_number = tag - kMainFlowTableCellTag_buyBase;
//		CCLOG("puzzle_number : %d", puzzle_number);
//		
//		ASPopupView* t_popup = ASPopupView::create(-200);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
//		
//		CCNode* t_container = CCNode::create();
//		t_popup->setContainerNode(t_container);
//		addChild(t_popup);
//		
//		CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
//		case_back->setPosition(CCPointZero);
//		t_container->addChild(case_back);
//		
//		case_back->setContentSize(CCSizeMake(230, 250));
//		
//		CCScale9Sprite* content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//		content_back->setPosition(ccp(0,2));
//		t_container->addChild(content_back);
//		
//		content_back->setContentSize(CCSizeMake(202, 146));
//		
//		CCLabelTTF* title_label = CCLabelTTF::create("지금 열기", mySGD->getFont().c_str(), 20);
//		title_label->setPosition(ccp(0, 102));
//		t_container->addChild(title_label);
//		
//		CCLabelTTF* content_label = CCLabelTTF::create(CCString::createWithFormat("%d Ruby 로 오픈", NSDS_GI(puzzle_number, kSDS_PZ_point_i))->getCString(), mySGD->getFont().c_str(), 18);
//		content_label->setPosition(CCPointZero);
//		t_container->addChild(content_label);
//		
//		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
//		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
//		s_close->setColor(ccGRAY);
//		
//		CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
//																			{
//																				is_menu_enable = true;
//																				t_popup->removeFromParent();
//																			});
//		
//		CCMenuLambda* close_menu = CCMenuLambda::createWithItem(close_item);
//		close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
//		close_menu->setPosition(ccp(92,105));
//		t_container->addChild(close_menu);
//		
//		if(mySGD->getGoodsValue(kGoodsType_ruby) >= NSDS_GI(puzzle_number, kSDS_PZ_point_i))
//		{
//			CCSprite* n_buy = CCSprite::create("popup2_buy.png");
//			CCSprite* s_buy = CCSprite::create("popup2_buy.png");
//			s_buy->setColor(ccGRAY);
//			
//			CCMenuItemSpriteLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender){
//				mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - NSDS_GI(puzzle_number, kSDS_PZ_point_i));
//				myDSH->setIntegerForKey(kDSH_Key_openPuzzleCnt, myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1);
//				
//				vector<SaveUserData_Key> save_userdata_list;
//				
//				save_userdata_list.push_back(kSaveUserData_Key_star);
//				save_userdata_list.push_back(kSaveUserData_Key_openPuzzle);
//				
//				myDSH->saveUserData(save_userdata_list, nullptr);
//				
//				int found_idx = -1;
//				for(int i=0;i<numberOfCellsInTableView(puzzle_table) && found_idx == -1;i++)
//				{
//					CCTableViewCell* t_cell = puzzle_table->cellAtIndex(i);
//					if(t_cell)
//					{
//						int cell_card_number = t_cell->getTag();
//						if(cell_card_number == puzzle_number)
//							found_idx = i;
//					}
//				}
//				if(found_idx != -1)
//					puzzle_table->updateCellAtIndex(found_idx);
//				
//				is_menu_enable = true;
//				t_popup->removeFromParent();
//			});
//			
//			CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
//			buy_menu->setTouchPriority(t_popup->getTouchPriority()-1);
//			buy_menu->setPosition(ccp(0,-95));
//			t_container->addChild(buy_menu);
//		}
//		else
//		{
//			CCSprite* buy_img = CCSprite::create("popup2_buy.png");
//			buy_img->setColor(ccc3(100, 100, 100));
//			buy_img->setPosition(ccp(0,-95));
//			t_container->addChild(buy_img);
//		}
//	}
//	else // ticketBase
//	{
//		int puzzle_number = tag - kMainFlowTableCellTag_ticketBase;
//		
//		ASPopupView* t_popup = ASPopupView::create(-200);
//		
//		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//		if(screen_scale_x < 1.f)
//			screen_scale_x = 1.f;
//		
//		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
//		
//		TicketRequestContent* t_container = TicketRequestContent::create(t_popup->getTouchPriority(), puzzle_number);
//		t_popup->setContainerNode(t_container);
//		addChild(t_popup);
//		
//		t_container->setRemoveAction([=](){
//			is_menu_enable = true;
//			t_popup->removeFromParent();
//		});
//	}
}

enum NewMainFlowMenuTag{
	kNewMainFlowMenuTag_rubyShop = 1,
	kNewMainFlowMenuTag_goldShop,
	kNewMainFlowMenuTag_heartShop,
	kNewMainFlowMenuTag_friendPointContent,
	kNewMainFlowMenuTag_postbox,
	kNewMainFlowMenuTag_option,
	kNewMainFlowMenuTag_tip,
	kNewMainFlowMenuTag_rank,
	kNewMainFlowMenuTag_shop,
	kNewMainFlowMenuTag_beautystone,
	kNewMainFlowMenuTag_cardSetting,
	kNewMainFlowMenuTag_friendManagement,
	kNewMainFlowMenuTag_gacha,
	kNewMainFlowMenuTag_achievement,
	kNewMainFlowMenuTag_event,
	kNewMainFlowMenuTag_ready,
	kNewMainFlowMenuTag_changeMode
};

void NewMainFlowScene::goStartSetting(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	selected_puzzle_number = NSDS_GI(tag, kSDS_SI_puzzle_i);
	selected_stage_number = tag;
	
	myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, selected_puzzle_number);
	myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, selected_puzzle_number, selected_stage_number);
	
	mySD->setSilType(selected_stage_number);
	
	info_down_popup = StageInfoDown::create(this, callfunc_selector(NewMainFlowScene::successStageInfoDown), this, callfunc_selector(NewMainFlowScene::startCancel));
	addChild(info_down_popup, kNewMainFlowZorder_popup);
}

void NewMainFlowScene::lockedStage(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	
	is_menu_enable = true;
}

void NewMainFlowScene::notBuyedStage(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	
	is_menu_enable = true;
}

CCTableViewCell* NewMainFlowScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	if(idx == NSDS_GI(kSDS_GI_puzzleListCount_i))
	{
		ListScaler* t_ls = ListScaler::create();
		t_ls->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+10));
		cell->addChild(t_ls);
		
		t_ls->startMyAction();
		
		CCSprite* t_img = CCSprite::create("not_open_book.png");
		t_img->setPosition(ccp(0, 0));
		t_ls->addChild(t_img);
		
		CCLabelTTF* update_ment = CCLabelTTF::create("업데이트...", mySGD->getFont().c_str(), 14);
		update_ment->setPosition(ccp(t_img->getContentSize().width/2.f, t_img->getContentSize().height/2.f));
		t_img->addChild(update_ment);
	}
	else
	{
		int history_count = mySGD->getPuzzleHistorySize();
		int open_puzzle_count = 0;
		for(int i=0;i<history_count;i++)
			if(mySGD->getPuzzleHistoryForIndex(i).is_open.getV())
				open_puzzle_count++;
		
		if(idx == 0 || open_puzzle_count >= idx+1)
		{
			int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx+1);
			
			CCTexture2D* t_texture = mySIL->addImage(CCString::createWithFormat("puzzle%d_map.png", puzzle_number)->getCString());
			
			if(t_texture)
			{
				ListScaler* t_ls = ListScaler::create();
				t_ls->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+10));
				t_ls->puzzle_number = puzzle_number;
				cell->addChild(t_ls);
				
				t_ls->startMyAction();
				
				CCSprite* t_img = CCSprite::createWithTexture(t_texture);
				t_img->setPosition(ccp(0,0));
				t_ls->addChild(t_img);
				
				t_ls->setCenterline();
				
				CCPoint start_warp_position = ccp(NSDS_GI(puzzle_number, kSDS_PZ_startWarp_x_d), NSDS_GI(puzzle_number, kSDS_PZ_startWarp_y_d));
				auto start_warp_ccbi = KS::loadCCBI<CCSprite*>(this, "main_warp.ccbi");
				start_warp_ccbi.first->setPosition(start_warp_position);
				t_img->addChild(start_warp_ccbi.first);
				
				start_warp_ccbi.second->runAnimationsForSequenceNamed("opened");
				
				if(idx == 0)
				{
					CCSprite* n_start_warp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
					n_start_warp->setOpacity(0);
					CCSprite* s_start_warp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
					s_start_warp->setOpacity(0);
					
					CCMenuItemLambda* start_warp_item = CCMenuItemSpriteLambda::create(n_start_warp, s_start_warp, [=](CCObject* sender)
																					   {
																						   puzzle_table->setContentOffsetInDuration(ccp(0,0), 0.3f);
																					   });
					
					CCMenuLambda* start_warp_menu = CCMenuLambda::createWithItem(start_warp_item);
					start_warp_menu->setPosition(start_warp_position);
					t_img->addChild(start_warp_menu);
				}
				else
				{
					CCSprite* n_start_warp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
					n_start_warp->setOpacity(0);
					CCSprite* s_start_warp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
					s_start_warp->setOpacity(0);
					
					CCMenuItemLambda* start_warp_item = CCMenuItemSpriteLambda::create(n_start_warp, s_start_warp, [=](CCObject* sender)
																					   {
																						   CCLOG("idx : %d", idx);
																						   CCLOG("go to x : %.1f", -(idx-1.f)*480.f);
																						   puzzle_table->setContentOffsetInDuration(ccp(-(idx-1.f)*480.f,0), 0.3f);
																					   });
					
					CCMenuLambda* start_warp_menu = CCMenuLambda::createWithItem(start_warp_item);
					start_warp_menu->setPosition(start_warp_position);
					t_img->addChild(start_warp_menu);
				}
				
				CCPoint end_warp_position = ccp(NSDS_GI(puzzle_number, kSDS_PZ_lastWarp_x_d), NSDS_GI(puzzle_number, kSDS_PZ_lastWarp_y_d));
				auto end_warp_ccbi = KS::loadCCBI<CCSprite*>(this, "main_warp.ccbi");
				end_warp_ccbi.first->setPosition(end_warp_position);
				t_img->addChild(end_warp_ccbi.first);
				
				if(mySGD->getOpenPuzzleCount() >= idx+2)
				{
					end_warp_ccbi.second->runAnimationsForSequenceNamed("opened");
					CCSprite* n_end_warp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
					n_end_warp->setOpacity(0);
					CCSprite* s_end_warp = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
					s_end_warp->setOpacity(0);
					
					CCMenuItemLambda* end_warp_item = CCMenuItemSpriteLambda::create(n_end_warp, s_end_warp, [=](CCObject* sender)
																					   {
																						   CCLOG("idx : %d", idx);
																						   CCLOG("go to x : %.1f", -(idx+1.f)*480.f);
																						   puzzle_table->setContentOffsetInDuration(ccp(-(idx+1.f)*480.f,0), 0.3f);
																					   });
					
					CCMenuLambda* end_warp_menu = CCMenuLambda::createWithItem(end_warp_item);
					end_warp_menu->setPosition(end_warp_position);
					t_img->addChild(end_warp_menu);
				}
				else
					end_warp_ccbi.second->runAnimationsForSequenceNamed("close");
				
				warp_ani_manager[puzzle_number] = end_warp_ccbi.second;
				
				
				int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
				int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
				
				for(int i=start_stage;i<start_stage+stage_count;i++)
				{
					if(i < start_stage+stage_count-1)
					{
						CCPoint before_position = ccp(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_x_d, i), NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_y_d, i));
						CCPoint recent_position = ccp(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_x_d, i+1), NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_y_d, i+1));
						
						CCPoint sub_position = ccpSub(recent_position, before_position);
						float dist_value = sqrtf(powf(sub_position.x, 2.f) + powf(sub_position.y, 2.f));
						int dot_cnt = dist_value/17 + 1;
						
						CCPoint d_position = ccpMult(sub_position, 1.f/dot_cnt);
						dot_cnt--;
						for(int j=0;j<dot_cnt;j++)
						{
							CCSprite* t_dot = CCSprite::create("stage_link_point.png");
							t_dot->setPosition(ccpAdd(before_position, ccpMult(d_position, j+1)));
							t_img->addChild(t_dot);
						}
					}
					
					bool is_buy, is_lock;
					if(i == 1 || mySGD->isClearPiece(i) ||
					   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, i) == 0 &&
						(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i) == 0 ||
						 mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i)))))
					{
						is_buy = false;
						is_lock = false;
					}
					else
					{
						if(mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, i)))
						{
							is_buy = true;
							is_lock = false;
						}
						else
						{
							is_buy = false;
							is_lock = true;
						}
					}
					
					if(is_buy)
					{
						StageNode* t_sn = StageNode::create(puzzle_number, i, this, menu_selector(NewMainFlowScene::notBuyedStage), [=](int t_int){ /*stage_node_manager[i] = NULL;*/ });
						t_sn->setViewMode(kStageNodeViewMode_buy);
						t_img->addChild(t_sn);
						stage_node_manager[i] = t_sn;
					}
					else if(is_lock)
					{
						StageNode* t_sn = StageNode::create(puzzle_number, i, this, menu_selector(NewMainFlowScene::lockedStage), [=](int t_int){ /*stage_node_manager[i] = NULL;*/ });
						t_sn->setViewMode(kStageNodeViewMode_lock);
						t_img->addChild(t_sn);
						stage_node_manager[i] = t_sn;
					}
					else
					{
						StageNode* t_sn = StageNode::create(puzzle_number, i, this, menu_selector(NewMainFlowScene::goStartSetting), [=](int t_int){ /*stage_node_manager[i] = NULL;*/ });
						if(clear_is_stage_unlock && next_stage_number == i)
							t_sn->setViewMode(kStageNodeViewMode_lock);
						else
							t_sn->setViewMode(kStageNodeViewMode_open);
						t_img->addChild(t_sn);
						stage_node_manager[i] = t_sn;
					}
				}
			}
			else // 정보를 받은적 없다.
			{
				ListScaler* t_ls = ListScaler::create();
				t_ls->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+10));
				cell->addChild(t_ls);
				
				t_ls->startMyAction();
				
				CCSprite* t_img = CCSprite::create("not_open_book.png");
				t_img->setPosition(ccp(0, 0));
				t_ls->addChild(t_img);
				
				CommonButton* puzzle_open_button = CommonButton::create("챕터 열기", 15, CCSizeMake(150,100), CommonButtonYellow, kCCMenuHandlerPriority);
				puzzle_open_button->setPosition(ccp(t_img->getContentSize().width/2.f, t_img->getContentSize().height/2.f));
				puzzle_open_button->setFunction([=](CCObject* sender)
												{
													open_puzzle_number = puzzle_number;
													StageListDown* t_sld = StageListDown::create(this, callfunc_selector(NewMainFlowScene::puzzleLoadSuccess), puzzle_number);
													addChild(t_sld, kNewMainFlowZorder_popup);
												});
				t_img->addChild(puzzle_open_button);
			}
		}
		else // 아직 열수 없는 퍼즐
		{
			ListScaler* t_ls = ListScaler::create();
			t_ls->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+10));
			cell->addChild(t_ls);
			
			t_ls->startMyAction();
			
			CCSprite* t_img = CCSprite::create("not_open_book.png");
			t_img->setPosition(ccp(0, 0));
			t_ls->addChild(t_img);
			
			CCLabelTTF* update_ment = CCLabelTTF::create("이전 스테이지를 모두 클리어 해야 합니다.", mySGD->getFont().c_str(), 14);
			update_ment->setPosition(ccp(t_img->getContentSize().width/2.f, t_img->getContentSize().height/2.f));
			t_img->addChild(update_ment);
		}
	}


/*
	float puzzle_width = 326.f;
	float puzzle_width_half = puzzle_width/2.f;
	float puzzle_height = 226.f;
	float puzzle_height_half = puzzle_height/2.f;
	float side_width = 38.f;
	float piece_size = 50.f;
	int piece_width_count = 6;
	int piece_height_count = 4;
	float margine_width = (piece_size - puzzle_width+piece_size*piece_width_count)/2.f;
	
	
	if(idx == numberOfCellsInTableView(table)-1)
	{
		CCNode* puzzle_node = CCNode::create();
		puzzle_node->setPosition(ccp(puzzle_width_half+margine_width,puzzle_height_half-4-55));
		cell->addChild(puzzle_node);
		
		CCSprite* puzzle_left = CCSprite::create("temp_puzzle_piece_left.png");
		puzzle_left->setAnchorPoint(ccp(0,0.5));
		puzzle_left->setPosition(ccp(-puzzle_width_half,0));
		puzzle_node->addChild(puzzle_left);
		
		CCSprite* puzzle_right = CCSprite::create("temp_puzzle_piece_right.png");
		puzzle_right->setAnchorPoint(ccp(1,0.5));
		puzzle_right->setPosition(ccp(puzzle_width_half,0));
		puzzle_node->addChild(puzzle_right);
		
		CCSprite* puzzle_top = CCSprite::create("temp_puzzle_piece_top.png");
		puzzle_top->setAnchorPoint(ccp(0.5,1));
		puzzle_top->setPosition(ccp(0,puzzle_height_half));
		puzzle_node->addChild(puzzle_top);
		
		CCSprite* puzzle_bottom = CCSprite::create("temp_puzzle_piece_bottom.png");
		puzzle_bottom->setAnchorPoint(ccp(0.5,0));
		puzzle_bottom->setPosition(ccp(0,-puzzle_height_half));
		puzzle_node->addChild(puzzle_bottom);
		
		
		for(int y = 0;y<piece_height_count;y++)
		{
			for(int x = 0;x<piece_width_count;x++)
			{
				string piece_type;
				if((x+y)%2 == 0)
					piece_type = "h";
				else
					piece_type = "w";
				
				CCSprite* puzzle_piece = CCSprite::create(("temp_piece_off_" + piece_type + ".png").c_str());
				puzzle_piece->setPosition(ccp(-puzzle_width_half+side_width+x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-y*piece_size));
				puzzle_node->addChild(puzzle_piece);
			}
		}
		
		CCLabelTTF* update_ment = CCLabelTTF::create("업데이트는 계속 됩니다.\nComing soon", mySGD->getFont().c_str(), 15);
		update_ment->setPosition(ccp(0,0));
		puzzle_node->addChild(update_ment);
		
		int before_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx);
		int before_piece_no = NSDS_GI(before_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, NSDS_GI(before_puzzle_number, kSDS_PZ_startStage_i)+NSDS_GI(before_puzzle_number, kSDS_PZ_stageCount_i)-1);
		string before_piece_type;
		CCPoint before_piece_position;
		int before_x = (before_piece_no-1)%int(piece_width_count)-6;
		int before_y = (before_piece_no-1)/int(piece_width_count);
		if((before_x+7+before_y)%2 == 0)
			before_piece_type = "h";
		else
			before_piece_type = "w";
		before_piece_position = ccp(-puzzle_width_half+side_width+before_x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-before_y*piece_size);
		
		CCSprite* puzzle_before_bridge = CCSprite::create(("temp_puzzle_bridge_back_p" + before_piece_type + ".png").c_str());
		puzzle_before_bridge->setPosition(before_piece_position);
		puzzle_node->addChild(puzzle_before_bridge);
	}
	else
	{
		int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx+1);
		cell->setTag(puzzle_number);
		
		vector<PuzzlePiecePath> puzzle_path = puzzle_piece_path[puzzle_number];
		int puzzle_path_idx = 0;
		
		CCNode* puzzle_node = CCNode::create();
		puzzle_node->setPosition(ccp(puzzle_width_half+margine_width,puzzle_height_half-4-55));
		cell->addChild(puzzle_node, 0, 1);
		
		if(idx == 0 || myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1 >= idx+1)
			//	if(puzzle_number == 1 || 9999+1 >= puzzle_number)
		{
			if(mySIL->addImage(CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, "original")->getCString()))
			{
				bool is_last_puzzle = false;
				if(idx+1 < numberOfCellsInTableView(table)-1)
				{
					// 뒷 퍼즐이 있다
					if(myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1 < idx+2)
					{
						// 다음 퍼즐이 잠겨있다
						is_last_puzzle = true;
					}
					
				}
				else
				{
					// 뒷 퍼즐이 없다
					is_last_puzzle = true;
				}
				
				CCSprite* puzzle_left = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_left.png", puzzle_number, "original")->getCString());
				puzzle_left->setAnchorPoint(ccp(0,0.5));
				puzzle_left->setPosition(ccp(-puzzle_width_half,0));
				puzzle_node->addChild(puzzle_left);
				
				CCSprite* puzzle_right = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_right.png", puzzle_number, "original")->getCString());
				puzzle_right->setAnchorPoint(ccp(1,0.5));
				puzzle_right->setPosition(ccp(puzzle_width_half,0));
				puzzle_node->addChild(puzzle_right);
				
				CCSprite* puzzle_top = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_top.png", puzzle_number, "original")->getCString());
				puzzle_top->setAnchorPoint(ccp(0.5,1));
				puzzle_top->setPosition(ccp(0,puzzle_height_half));
				puzzle_node->addChild(puzzle_top);
				
				CCSprite* puzzle_bottom = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_bottom.png", puzzle_number, "original")->getCString());
				puzzle_bottom->setAnchorPoint(ccp(0.5,0));
				puzzle_bottom->setPosition(ccp(0,-puzzle_height_half));
				puzzle_node->addChild(puzzle_bottom);
				
				bool is_selected_stage_puzzle = false;
				int is_selected_stage_number;
				
				bool is_next_selected_stage_puzzle = false;
				CCPoint next_selected_stage_position;
				string next_selected_stage_piece_type;
				
				for(int y = 0;y<piece_height_count;y++)
				{
					for(int x = 0;x<piece_width_count;x++)
					{
						int piece_number = y*piece_width_count+x+1;
						bool is_stage = false;
						int stage_number = -1;
						if(puzzle_path_idx < puzzle_path.size() && piece_number == puzzle_path[puzzle_path_idx].piece_no)
						{
							is_stage = true;
							stage_number = puzzle_path[puzzle_path_idx].stage_no;
							puzzle_path_idx++;
						}
						
						if(is_stage)
						{
							bool is_buy, is_lock;
							if(stage_number == 1 || mySGD->isClearPiece(stage_number) ||
							   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
								(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 || mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
							{
								is_buy = false;
								is_lock = false;
							}
							else
							{
								if(mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))
								{
									is_buy = true;
									is_lock = false;
								}
								else
								{
									is_buy = false;
									is_lock = true;
								}
							}
							function<void(int)> clicked_func;
							if(is_buy)
							{
								clicked_func = [=](int t_stage_number){	buyPieceAction(t_stage_number);	};
							}
							else if(is_lock)
							{
								clicked_func = [=](int t_stage_number){	lockPieceAction(t_stage_number);	};
							}
							else
							{
								clicked_func = [=](int t_stage_number){	pieceAction(t_stage_number);	};
							}
							
							CCAssert(stage_number > 0 && stage_number < 100000, "what?");
							
							NewPuzzlePiece* t_piece = NewPuzzlePiece::create(stage_number, clicked_func, (NewPuzzlePieceMode)puzzle_piece_mode[idx], is_buy, is_lock);
							t_piece->setPosition(ccp(-puzzle_width_half+side_width+x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-y*piece_size));
							puzzle_node->addChild(t_piece, 0, stage_number);
							
							if(stage_number == selected_stage_number)
							{
								is_selected_stage_puzzle = true;
								is_selected_stage_number = selected_stage_number;
							}
							
							if(stage_number == next_stage_number)
							{
								is_next_selected_stage_puzzle = true;
								if((x+y)%2 == 0)
									next_selected_stage_piece_type = "h";
								else
									next_selected_stage_piece_type = "w";
								next_selected_stage_position = ccp(-puzzle_width_half+side_width+x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-y*piece_size);
							}
							
							if(is_last_puzzle && !is_buy && !is_lock && (puzzle_path[puzzle_path_idx-1].next_stage_no == -1 ||
							   !((puzzle_path[puzzle_path_idx-1].next_stage_no == 1 || mySGD->isClearPiece(puzzle_path[puzzle_path_idx-1].next_stage_no) ||
								(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, puzzle_path[puzzle_path_idx-1].next_stage_no) == 0 &&
								(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, puzzle_path[puzzle_path_idx-1].next_stage_no) == 0 ||
								mySGD->isClearPiece(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, puzzle_path[puzzle_path_idx-1].next_stage_no))))))))
							{
								CCSprite* my_profile = GDWebSprite::create(hspConnector::get()->getKakaoProfileURL(), "temp_piece_frame_noimg.png");
								my_profile->setAnchorPoint(ccp(0.5,0.5));
								my_profile->setPosition(t_piece->getPosition());
								puzzle_node->addChild(my_profile);
								CCSprite* profile_frame = CCSprite::create("temp_piece_frame.png");
								profile_frame->setPosition(t_piece->getPosition());
								puzzle_node->addChild(profile_frame);
							}
							
							if(NSDS_GI(stage_number, kSDS_SI_missionType_i) != kCLEAR_default)
							{
								CCSprite* mission_img = CCSprite::create("puzzle_mission.png");
								mission_img->setPosition(ccpAdd(t_piece->getPosition(), ccp(12,11)));
								puzzle_node->addChild(mission_img);
							}
						}
						else
						{
							CCSprite* puzzle_piece = mySIL->getLoadedImg(CCString::createWithFormat("puzzle%d_%s_piece%d.png", puzzle_number, "original", piece_number)->getCString());
							puzzle_piece->setPosition(ccp(-puzzle_width_half+side_width+x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-y*piece_size));
							puzzle_node->addChild(puzzle_piece);
						}
					}
				}
				
				if(idx > 0)
				{
					int before_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx);
					int before_piece_no = NSDS_GI(before_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, NSDS_GI(before_puzzle_number, kSDS_PZ_startStage_i)+NSDS_GI(before_puzzle_number, kSDS_PZ_stageCount_i)-1);
					string before_piece_type;
					CCPoint before_piece_position;
					int before_x = (before_piece_no-1)%int(piece_width_count)-6;
					int before_y = (before_piece_no-1)/int(piece_width_count);
					if((before_x+7+before_y)%2 == 0)
						before_piece_type = "h";
					else
						before_piece_type = "w";
					before_piece_position = ccp(-puzzle_width_half+side_width+before_x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-before_y*piece_size);
					
					CCSprite* puzzle_before_bridge = CCSprite::create(("temp_puzzle_bridge_front_p" + before_piece_type + ".png").c_str());
					puzzle_before_bridge->setPosition(before_piece_position);
					puzzle_node->addChild(puzzle_before_bridge);
				}
				
				int last_piece_no = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, NSDS_GI(puzzle_number, kSDS_PZ_startStage_i)+NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i)-1);
				string last_piece_type;
				CCPoint last_piece_position;
				int last_x = (last_piece_no-1)%int(piece_width_count)+1;
				int last_y = (last_piece_no-1)/int(piece_width_count);
				if((last_x+last_y)%2 == 0)
					last_piece_type = "h";
				else
					last_piece_type = "w";
				last_piece_position = ccp(-puzzle_width_half+side_width+last_x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-last_y*piece_size);
				
				if(myDSH->getBoolForKey(kDSH_Key_isClearedPuzzle_int1, puzzle_number))
				{
					if(myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1 >= idx+2) // open
					{
						CCSprite* puzzle_after_bridge = CCSprite::create(("temp_puzzle_bridge_front_p" + last_piece_type + ".png").c_str());
						puzzle_after_bridge->setPosition(last_piece_position);
						puzzle_node->addChild(puzzle_after_bridge);
					}
					else // buy or ticket
					{
						CCSprite* n_bridge = CCSprite::create(("temp_puzzle_bridge_front_p" + last_piece_type + ".png").c_str());
						n_bridge->setColor(ccGREEN);
						n_bridge->setPosition(ccp(-12.f,-12.f));
						CCSprite* s_bridge = CCSprite::create(("temp_puzzle_bridge_front_p" + last_piece_type + ".png").c_str());
						s_bridge->setColor(ccGRAY);
						s_bridge->setPosition(ccp(-12.f,-12.f));
						
						CCMenuItem* bridge_item = CCMenuItemSprite::create(n_bridge, s_bridge, this, menu_selector(NewMainFlowScene::cellAction));
						bridge_item->setContentSize(CCSizeMake(50, 50));
						bridge_item->setTag(idx+2);
						
						ScrollMenu* bridge_menu = ScrollMenu::create(bridge_item, NULL);
						bridge_menu->setPosition(last_piece_position);
						puzzle_node->addChild(bridge_menu);
					}
				}
				else
				{
					CCSprite* puzzle_after_bridge = CCSprite::create(("temp_puzzle_bridge_back_p" + last_piece_type + ".png").c_str());
					puzzle_after_bridge->setPosition(last_piece_position);
					puzzle_node->addChild(puzzle_after_bridge);
				}
				
				CCSprite* n_change_mode = CCSprite::create("puzzle_change_mode.png");
				CCSprite* s_change_mode = CCSprite::create("puzzle_change_mode.png");
				s_change_mode->setColor(ccGRAY);
				
				
				CCMenuItem* change_mode_item = CCMenuItemSprite::create(n_change_mode, s_change_mode, this, menu_selector(NewMainFlowScene::menuAction));
				change_mode_item->setTag(kNewMainFlowMenuTag_changeMode);
				
				int menu_y = piece_height_count;
				if(last_y == menu_y)
					menu_y--;
				
				ScrollMenu* change_mode_menu = ScrollMenu::create(change_mode_item, NULL);
				change_mode_menu->setPosition(ccp(-puzzle_width_half+side_width+piece_width_count*piece_size, -puzzle_height_half+side_width+(piece_size*menu_y)-piece_size));
				change_mode_menu->setTag(idx);
				puzzle_node->addChild(change_mode_menu);
				
				if(is_selected_stage_puzzle)
				{
					NewPieceManager::sharedInstance()->regiSelectedPiece(is_selected_stage_number);
				}
				
				if(is_next_selected_stage_puzzle)
				{
					unlock_cover = CCSprite::create(CCString::createWithFormat("temp_piece_lock_%s.png", next_selected_stage_piece_type.c_str())->getCString());
					unlock_cover->setPosition(next_selected_stage_position);
					puzzle_node->addChild(unlock_cover);
				}
			}
			else
			{
				CCSprite* puzzle_left = CCSprite::create("temp_puzzle_piece_left.png");
				puzzle_left->setAnchorPoint(ccp(0,0.5));
				puzzle_left->setPosition(ccp(-puzzle_width_half,0));
				puzzle_node->addChild(puzzle_left);
				
				CCSprite* puzzle_right = CCSprite::create("temp_puzzle_piece_right.png");
				puzzle_right->setAnchorPoint(ccp(1,0.5));
				puzzle_right->setPosition(ccp(puzzle_width_half,0));
				puzzle_node->addChild(puzzle_right);
				
				CCSprite* puzzle_top = CCSprite::create("temp_puzzle_piece_top.png");
				puzzle_top->setAnchorPoint(ccp(0.5,1));
				puzzle_top->setPosition(ccp(0,puzzle_height_half));
				puzzle_node->addChild(puzzle_top);
				
				CCSprite* puzzle_bottom = CCSprite::create("temp_puzzle_piece_bottom.png");
				puzzle_bottom->setAnchorPoint(ccp(0.5,0));
				puzzle_bottom->setPosition(ccp(0,-puzzle_height_half));
				puzzle_node->addChild(puzzle_bottom);
				
				
				for(int y = 0;y<piece_height_count;y++)
				{
					for(int x = 0;x<piece_width_count;x++)
					{
						int piece_number = y*piece_width_count+x+1;
						bool is_stage = false;
						int stage_number = -1;
						if(piece_number == puzzle_path[puzzle_path_idx].piece_no)
						{
							is_stage = true;
							stage_number = puzzle_path[puzzle_path_idx].stage_no;
							puzzle_path_idx++;
						}
						
						string piece_type;
						if((x+y)%2 == 0)
							piece_type = "h";
						else
							piece_type = "w";
						
						CCSprite* puzzle_piece;
						if(is_stage)
							puzzle_piece = CCSprite::create(("temp_piece_lock_" + piece_type + ".png").c_str());
						else
							puzzle_piece = CCSprite::create(("temp_piece_off_" + piece_type + ".png").c_str());
						
						puzzle_piece->setPosition(ccp(-puzzle_width_half+side_width+x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-y*piece_size));
						puzzle_node->addChild(puzzle_piece);
					}
				}
				
				
				int before_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx);
				int before_piece_no = NSDS_GI(before_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, NSDS_GI(before_puzzle_number, kSDS_PZ_startStage_i)+NSDS_GI(before_puzzle_number, kSDS_PZ_stageCount_i)-1);
				string before_piece_type;
				CCPoint before_piece_position;
				int before_x = (before_piece_no-1)%int(piece_width_count)-6;
				int before_y = (before_piece_no-1)/int(piece_width_count);
				if((before_x+7+before_y)%2 == 0)
					before_piece_type = "h";
				else
					before_piece_type = "w";
				before_piece_position = ccp(-puzzle_width_half+side_width+before_x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-before_y*piece_size);
				
				CCSprite* puzzle_before_bridge = CCSprite::create(("temp_puzzle_bridge_back_p" + before_piece_type + ".png").c_str());
				puzzle_before_bridge->setPosition(before_piece_position);
				puzzle_node->addChild(puzzle_before_bridge);
				
				
				int last_piece_no = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, NSDS_GI(puzzle_number, kSDS_PZ_startStage_i)+NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i)-1);
				string last_piece_type;
				CCPoint last_piece_position;
				int last_x = (last_piece_no-1)%int(piece_width_count)+1;
				int last_y = (last_piece_no-1)/int(piece_width_count);
				if((last_x+last_y)%2 == 0)
					last_piece_type = "h";
				else
					last_piece_type = "w";
				last_piece_position = ccp(-puzzle_width_half+side_width+last_x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-last_y*piece_size);
				
				CCSprite* puzzle_after_bridge = CCSprite::create(("temp_puzzle_bridge_back_p" + last_piece_type + ".png").c_str());
				puzzle_after_bridge->setPosition(last_piece_position);
				puzzle_node->addChild(puzzle_after_bridge);
				
				CommonButton* puzzle_open_button = CommonButton::create("퍼즐 열기", 15, CCSizeMake(150,100), CommonButtonYellow, kCCMenuHandlerPriority);
				puzzle_open_button->setPosition(CCPointZero);
				puzzle_open_button->setFunction([=](CCObject* sender)
												{
													open_puzzle_number = puzzle_number;
													StageListDown* t_sld = StageListDown::create(this, callfunc_selector(NewMainFlowScene::puzzleLoadSuccess), puzzle_number);
													addChild(t_sld, kNewMainFlowZorder_popup);
												});
				puzzle_node->addChild(puzzle_open_button);
			}
		}
		else
		{
			CCSprite* puzzle_left = CCSprite::create("temp_puzzle_piece_left.png");
			puzzle_left->setAnchorPoint(ccp(0,0.5));
			puzzle_left->setPosition(ccp(-puzzle_width_half,0));
			puzzle_node->addChild(puzzle_left);
			
			CCSprite* puzzle_right = CCSprite::create("temp_puzzle_piece_right.png");
			puzzle_right->setAnchorPoint(ccp(1,0.5));
			puzzle_right->setPosition(ccp(puzzle_width_half,0));
			puzzle_node->addChild(puzzle_right);
			
			CCSprite* puzzle_top = CCSprite::create("temp_puzzle_piece_top.png");
			puzzle_top->setAnchorPoint(ccp(0.5,1));
			puzzle_top->setPosition(ccp(0,puzzle_height_half));
			puzzle_node->addChild(puzzle_top);
			
			CCSprite* puzzle_bottom = CCSprite::create("temp_puzzle_piece_bottom.png");
			puzzle_bottom->setAnchorPoint(ccp(0.5,0));
			puzzle_bottom->setPosition(ccp(0,-puzzle_height_half));
			puzzle_node->addChild(puzzle_bottom);
			
			
			for(int y = 0;y<piece_height_count;y++)
			{
				for(int x = 0;x<piece_width_count;x++)
				{
					int piece_number = y*piece_width_count+x+1;
					bool is_stage = false;
					int stage_number = -1;
					if(piece_number == puzzle_path[puzzle_path_idx].piece_no)
					{
						is_stage = true;
						stage_number = puzzle_path[puzzle_path_idx].stage_no;
						puzzle_path_idx++;
					}
					
					string piece_type;
					if((x+y)%2 == 0)
						piece_type = "h";
					else
						piece_type = "w";
					
					CCSprite* puzzle_piece;
					if(is_stage)
						puzzle_piece = CCSprite::create(("temp_piece_lock_" + piece_type + ".png").c_str());
					else
						puzzle_piece = CCSprite::create(("temp_piece_off_" + piece_type + ".png").c_str());
					
					
					puzzle_piece->setPosition(ccp(-puzzle_width_half+side_width+x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-y*piece_size));
					puzzle_node->addChild(puzzle_piece);
				}
			}
			
			int before_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx);
			int before_piece_no = NSDS_GI(before_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, NSDS_GI(before_puzzle_number, kSDS_PZ_startStage_i)+NSDS_GI(before_puzzle_number, kSDS_PZ_stageCount_i)-1);
			string before_piece_type;
			CCPoint before_piece_position;
			int before_x = (before_piece_no-1)%int(piece_width_count)-6;
			int before_y = (before_piece_no-1)/int(piece_width_count);
			if((before_x+7+before_y)%2 == 0)
				before_piece_type = "h";
			else
				before_piece_type = "w";
			before_piece_position = ccp(-puzzle_width_half+side_width+before_x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-before_y*piece_size);
			
			if(myDSH->getBoolForKey(kDSH_Key_isClearedPuzzle_int1, NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx)))
			{
				CCSprite* n_bridge = CCSprite::create(("temp_puzzle_bridge_front_p" + before_piece_type + ".png").c_str());
				n_bridge->setColor(ccGREEN);
				n_bridge->setPosition(ccp(-12.f,-12.f));
				CCSprite* s_bridge = CCSprite::create(("temp_puzzle_bridge_front_p" + before_piece_type + ".png").c_str());
				s_bridge->setColor(ccGRAY);
				s_bridge->setPosition(ccp(-12.f,-12.f));
				
				CCMenuItem* bridge_item = CCMenuItemSprite::create(n_bridge, s_bridge, this, menu_selector(NewMainFlowScene::cellAction));
				bridge_item->setContentSize(CCSizeMake(50, 50));
				bridge_item->setTag(idx+1);
				
				ScrollMenu* bridge_menu = ScrollMenu::create(bridge_item, NULL);
				bridge_menu->setPosition(before_piece_position);
				puzzle_node->addChild(bridge_menu);
			}
			else
			{
				CCSprite* puzzle_before_bridge = CCSprite::create(("temp_puzzle_bridge_back_p" + before_piece_type + ".png").c_str());
				puzzle_before_bridge->setPosition(before_piece_position);
				puzzle_node->addChild(puzzle_before_bridge);
			}
			
			
			int last_piece_no = NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, NSDS_GI(puzzle_number, kSDS_PZ_startStage_i)+NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i)-1);
			string last_piece_type;
			CCPoint last_piece_position;
			int last_x = (last_piece_no-1)%int(piece_width_count)+1;
			int last_y = (last_piece_no-1)/int(piece_width_count);
			if((last_x+last_y)%2 == 0)
				last_piece_type = "h";
			else
				last_piece_type = "w";
			last_piece_position = ccp(-puzzle_width_half+side_width+last_x*piece_size, -puzzle_height_half+side_width+(piece_size*(piece_height_count-1))-last_y*piece_size);
			
			CCSprite* puzzle_after_bridge = CCSprite::create(("temp_puzzle_bridge_back_p" + last_piece_type + ".png").c_str());
			puzzle_after_bridge->setPosition(last_piece_position);
			puzzle_node->addChild(puzzle_after_bridge);
			
		}
	
	}
 */
	return cell;
}

void NewMainFlowScene::puzzleLoadSuccess()
{
	CCPoint t_point = puzzle_table->getContentOffset();
	puzzle_table->reloadData();
	puzzle_table->setContentOffset(t_point);
	
	int start_stage = NSDS_GI(open_puzzle_number, kSDS_PZ_startStage_i);
	pieceAction(start_stage);
}

void NewMainFlowScene::pieceAction(int t_stage_number)
{
	CCLOG("pieceAction : %d", t_stage_number);
	
//	new_stage_info_view->setClickedStage(t_stage_number);
	
	changeButtonChallenge(false, t_stage_number);
	
	selected_stage_number = t_stage_number;
	selected_puzzle_number = NSDS_GI(selected_stage_number, kSDS_SI_puzzle_i);
	
//	NewPieceManager::sharedInstance()->regiSelectedPiece(selected_stage_number);
	
//	if(selected_stage_cell_idx != -1)
//	{
//		puzzle_table->updateCellAtIndex(selected_stage_cell_idx);
//		
//		bool is_found = false;
//		int found_idx = -1;
//		int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
//		for(int i=1;i<=puzzle_count && !is_found;i++)
//		{
//			if(selected_puzzle_number == NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i))
//			{
//				is_found = true;
//				found_idx = i-1;
//			}
//		}
//		
//		if(is_found)
//		{
//			puzzle_table->updateCellAtIndex(found_idx);
//		}
//		else
//		{
//			puzzle_table->reloadData();
//		}
//	}
//	else
//	{
//		puzzle_table->updateCellAtIndex(0);
//	}
}

void NewMainFlowScene::changeButtonChallenge(bool t_b, int t_stage)
{
	CCPoint before_position = ready_menu->getPosition();
	CCNode* before_parant = ready_menu->getParent();
	
	ready_menu->removeFromParent();
	ready_menu = NULL;
	
	string middle_str;
	if(t_b)
		middle_str = "challenge";
	else
		middle_str = "ready";
	
	CCSprite* n_ready = CCSprite::create(CCString::createWithFormat("mainflow_new_%s.png", middle_str.c_str())->getCString());
	CCLabelTTF* n_stage_number = CCLabelTTF::create(CCString::createWithFormat("%d 스테이지", t_stage)->getCString(), mySGD->getFont().c_str(), 11);
	n_stage_number->setPosition(ccp(n_ready->getContentSize().width/2.f+16, n_ready->getContentSize().height/2.f-11));
	n_ready->addChild(n_stage_number);
	
	CCSprite* s_ready = CCSprite::create(CCString::createWithFormat("mainflow_new_%s.png", middle_str.c_str())->getCString());
	s_ready->setColor(ccGRAY);
	CCLabelTTF* s_stage_number = CCLabelTTF::create(CCString::createWithFormat("%d 스테이지", t_stage)->getCString(), mySGD->getFont().c_str(), 11);
	s_stage_number->setPosition(ccp(s_ready->getContentSize().width/2.f+16, s_ready->getContentSize().height/2.f-11));
	s_ready->addChild(s_stage_number);
	
	
	CCMenuItem* ready_item = CCMenuItemSprite::create(n_ready, s_ready, this, menu_selector(NewMainFlowScene::menuAction));
	ready_item->setTag(kNewMainFlowMenuTag_ready);
	
	ready_menu = CCMenu::createWithItem(ready_item);
	ready_menu->setPosition(before_position);
	before_parant->addChild(ready_menu);
}

void NewMainFlowScene::buyPieceAction(int t_stage_number)
{
	CCLOG("buyPieceAction : %d", t_stage_number);
//	new_stage_info_view->setClickedStage(t_stage_number);
}
void NewMainFlowScene::lockPieceAction(int t_stage_number)
{
	CCLOG("lockPieceAction : %d", t_stage_number);
//	new_stage_info_view->setClickedStage(t_stage_number);
}

//void MainFlowScene::endUnlockAnimation()
//{
//	puzzle_table->setTouchEnabled(true);
//	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
//	is_menu_enable = true;
//}

void NewMainFlowScene::scrollViewDidScroll(CCScrollView* view){}
void NewMainFlowScene::scrollViewDidZoom(CCScrollView* view){}
void NewMainFlowScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell){}
CCSize NewMainFlowScene::cellSizeForTable(CCTableView *table)
{
	return table_size;
}
unsigned int NewMainFlowScene::numberOfCellsInTableView(CCTableView *table)
{
	return NSDS_GI(kSDS_GI_puzzleListCount_i)+1;
}


void NewMainFlowScene::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	int tag = ((CCNode*)sender)->getTag();
	
	is_menu_enable = false;
	
	if(tag == kNewMainFlowMenuTag_rubyShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::popupClose));
		t_shop->targetHeartTime(heart_time);
		t_shop->setShopCode(kSC_ruby);
		t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
		addChild(t_shop, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_goldShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::popupClose));
		t_shop->targetHeartTime(heart_time);
		t_shop->setShopCode(kSC_gold);
		t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
		addChild(t_shop, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_heartShop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::popupClose));
		t_shop->targetHeartTime(heart_time);
		t_shop->setShopCode(kSC_heart);
		t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
		addChild(t_shop, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_friendPointContent)
	{
		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-300);
		t_suction->setTouchEnabled(true);
		t_suction->target_touch_began = this;
		t_suction->delegate_touch_began = callfunc_selector(NewMainFlowScene::closeFriendPoint);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
		CCSprite* stencil_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 78, 150));
		stencil_node->setPosition(ccp(294,229+160.f*(screen_scale_y-1.f)));
		CCClippingNode* cliping_node = CCClippingNode::create(stencil_node);
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
//		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setPosition(CCPointZero);
		t_suction->addChild(cliping_node);
		
		CCSprite* inner_img = CCSprite::create("candy_popup.png");
		inner_img->setPosition(ccp(294,229+160.f*(screen_scale_y-1.f)+150));
		cliping_node->addChild(inner_img);
		
		CCMoveTo* t_move_down = CCMoveTo::create(0.3f, ccp(294,229+160.f*(screen_scale_y-1.f)));
		inner_img->runAction(t_move_down);
		
		close_friend_point_action = [=](){
			t_suction->target_touch_began = NULL;
			t_suction->delegate_touch_began = NULL;
			
			CCMoveTo* t_move_up = CCMoveTo::create(0.3f, ccp(294,229+160.f*(screen_scale_y-1.f)+150));
			CCCallFunc* t_call = CCCallFunc::create(t_suction, callfunc_selector(CCLayer::removeFromParent));
			CCSequence* t_seq = CCSequence::create(t_move_up, t_call, NULL);
			inner_img->runAction(t_seq);
		};
		addChild(t_suction, kNewMainFlowZorder_top-1);
		
		is_menu_enable = true;
	}
	else if(tag == kNewMainFlowMenuTag_postbox)
	{
		MailPopup* t_pp = MailPopup::create(this, callfunc_selector(NewMainFlowScene::mailPopupClose), bind(&NewMainFlowScene::heartRefresh, this));
		addChild(t_pp, kNewMainFlowZorder_popup);
		
		postbox_count_case->setVisible(false);
	}
	else if(tag == kNewMainFlowMenuTag_option)
	{
		OptionPopup* t_popup = OptionPopup::create();
		t_popup->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::popupClose));
		addChild(t_popup, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_tip)
	{
		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
		t_suction->target_touch_began = t_suction;
		t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
		t_suction->setTouchEnabled(true);
		
		CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_main.png");
		dimed_tip->setPosition(ccp(240,160));
		t_suction->addChild(dimed_tip);
		
		addChild(t_suction, kNewMainFlowZorder_popup);
		
		is_menu_enable = true;
	}
	else if(tag == kNewMainFlowMenuTag_rank)
	{
		RankPopup* t_rp = RankPopup::create(this, callfunc_selector(NewMainFlowScene::popupClose));
		addChild(t_rp, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_shop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::popupClose));
		t_shop->targetHeartTime(heart_time);
		t_shop->setShopCode(kSC_character);
		t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
		addChild(t_shop, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_beautystone)
	{
		puzzle_table->setTouchEnabled(false);
		BeautyStoneSettingPopup* t_popup = BeautyStoneSettingPopup::create();
		t_popup->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::tutorialCardSettingClose));
		addChild(t_popup, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_cardSetting)
	{
		puzzle_table->setTouchEnabled(false);
		mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
		CardSettingPopup* t_popup = CardSettingPopup::create();
		t_popup->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::tutorialCardSettingClose));
		addChild(t_popup, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_friendManagement)
	{
		is_menu_enable = true;
		
		InviteEventPopup* t_rp = InviteEventPopup::create(this, callfunc_selector(NewMainFlowScene::tutorialCardSettingClose));
		
		addChild(t_rp, kNewMainFlowZorder_popup);
		
	}
	else if(tag == kNewMainFlowMenuTag_gacha)
	{
		GachaPurchase* t_gp = GachaPurchase::create();
		addChild(t_gp, kNewMainFlowZorder_popup);
		
		t_gp->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::popupClose));
		t_gp->setOutAllObjectAction(NULL, NULL);
		t_gp->setInAllObjectAction(this, callfunc_selector(NewMainFlowScene::popupClose));
	}
	else if(tag == kNewMainFlowMenuTag_achievement)
	{
		AchievePopup* t_ap = AchievePopup::create();
		addChild(t_ap, kNewMainFlowZorder_popup);
		
		t_ap->setHideFinalAction(this, callfunc_selector(NewMainFlowScene::achievePopupClose));
		
		achievement_count_case->setVisible(false);
	}
	else if(tag == kNewMainFlowMenuTag_event)
	{
		EventListDown* t_eld = EventListDown::create(this, callfunc_selector(NewMainFlowScene::successEventListDown));
		addChild(t_eld, kNewMainFlowZorder_popup);
		
		//			EventPopup* t_popup = EventPopup::create();
		//			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
		//			addChild(t_popup, kMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_ready)
	{
//		if(new_stage_info_view->getSelectedIdx() != -1)
//		{
//			RankFriendInfo challenge_info = new_stage_info_view->getSelectedIdxRankFriendInfo();
//			mySGD->setMeChallengeTarget(challenge_info.user_id, challenge_info.nickname, challenge_info.score, challenge_info.img_url);
//		}
		
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, selected_puzzle_number);
		myDSH->setIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, selected_puzzle_number, selected_stage_number);
		
		mySD->setSilType(selected_stage_number);
		
		info_down_popup = StageInfoDown::create(this, callfunc_selector(NewMainFlowScene::successStageInfoDown), this, callfunc_selector(NewMainFlowScene::startCancel));
		addChild(info_down_popup, kNewMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_changeMode)
	{
		CCNode* target_menu = ((CCNode*)sender)->getParent();
		int t_idx = target_menu->getTag();
		puzzle_piece_mode[t_idx] = (puzzle_piece_mode[t_idx]+1)%(kNewPuzzlePieceMode_thumbnail+1);
		
		CCTableViewCell* t_cell = puzzle_table->cellAtIndex(t_idx);
		
		if(t_cell)
		{
			CCNode* t_puzzle_node = t_cell->getChildByTag(1);
			
			int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, t_idx+1);
			
			int t_start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
			int t_stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
			
			for(int i=t_start_stage;i<t_start_stage+t_stage_count;i++)
			{
				NewPuzzlePiece* t_piece = (NewPuzzlePiece*)t_puzzle_node->getChildByTag(i);
				t_piece->turnPiece((NewPuzzlePieceMode)puzzle_piece_mode[t_idx]);
			}
			
			target_menu->setVisible(false);
			
			CCDelayTime* t_delay = CCDelayTime::create(0.3f);
			CCCallFuncO* t_call = CCCallFuncO::create(this, callfuncO_selector(NewMainFlowScene::endChangeMode), target_menu);
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_delay, t_call);
			target_menu->runAction(t_seq);
			
//			puzzle_table->updateCellAtIndex(t_puzzle_number-1);
		}
		else
			is_menu_enable = true;
	}
}

void NewMainFlowScene::endChangeMode(CCObject* sender)
{
	((CCNode*)sender)->setVisible(true);
	is_menu_enable = true;
}

void NewMainFlowScene::successEventListDown()
{
	CCDirector::sharedDirector()->replaceScene(MainFlowScene::scene());
}

void NewMainFlowScene::successStageInfoDown()
{
	CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
}
void NewMainFlowScene::startCancel()
{
	info_down_popup->removeFromParent();
	is_menu_enable = true;
}

void NewMainFlowScene::setBeautystoneMenu()
{
	beautystone_node->removeAllChildren();
	
	int character_number = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter);
	
	int slot_cnt = 2;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, character_number+1);
	
	bool is_id = false;
	vector<int> stone_id_vector;
	for(int i=0;i<slot_cnt;i++)
	{
		int stone_id = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, character_number+1, i+1);
		stone_id_vector.push_back(stone_id);
		if(stone_id != 0)
			is_id = true;
	}
	
	if(is_id)
	{
		for(int i=0;i<stone_id_vector.size();i++)
		{
			CCPoint base_position = ccp(0, 10+(slot_cnt-0.5f-i)*45);
			if(stone_id_vector[i] == 0)
			{
				CCSprite* beautystone_img = CCSprite::create("empty_beautystone.png");
				beautystone_img->setScale(0.8f);
				beautystone_img->setPosition(base_position);
				beautystone_node->addChild(beautystone_img);
			}
			else
			{
				int b_type = myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, stone_id_vector[i]);
				int b_rank = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, stone_id_vector[i]);
				int b_level = myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, stone_id_vector[i]);
				
				CCSprite* beautystone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", b_type, b_rank)->getCString());
				beautystone_img->setScale(0.8f);
				beautystone_img->setPosition(base_position);
				beautystone_node->addChild(beautystone_img);
				
				CCLabelTTF* beautystone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", b_level)->getCString(), mySGD->getFont().c_str(), 10);
				beautystone_level->setPosition(ccpAdd(base_position, ccp(0,-15)));
				beautystone_node->addChild(beautystone_level);
			}
		}
		
		
		CCSprite* n_beautystone = CCSprite::create("mainflow_new_beautystone_on.png");
		CCSprite* s_beautystone = CCSprite::create("mainflow_new_beautystone_on.png");
		s_beautystone->setColor(ccGRAY);
		
		CCMenuItem* beautystone_item = CCMenuItemSprite::create(n_beautystone, s_beautystone, this, menu_selector(NewMainFlowScene::menuAction));
		beautystone_item->setTag(kNewMainFlowMenuTag_beautystone);
		
		CCMenu* beautystone_menu = CCMenu::createWithItem(beautystone_item);
		beautystone_menu->setPosition(ccp(0, n_beautystone->getContentSize().height/2.f));
		beautystone_node->addChild(beautystone_menu);
	}
	else
	{
		CCSprite* n_beautystone = CCSprite::create("mainflow_new_beautystone_off.png");
		CCSprite* s_beautystone = CCSprite::create("mainflow_new_beautystone_off.png");
		s_beautystone->setColor(ccGRAY);
		
		CCMenuItem* beautystone_item = CCMenuItemSprite::create(n_beautystone, s_beautystone, this, menu_selector(NewMainFlowScene::menuAction));
		beautystone_item->setTag(kNewMainFlowMenuTag_beautystone);
		
		CCMenu* beautystone_menu = CCMenu::createWithItem(beautystone_item);
		beautystone_menu->setPosition(ccp(0, n_beautystone->getContentSize().height/2.f));
		beautystone_node->addChild(beautystone_menu);
	}
}

void NewMainFlowScene::setBottom()
{
	CCNode* bottom_case = CCNode::create();
	bottom_case->setAnchorPoint(ccp(0.5f,0.5f));
	bottom_case->setPosition(ccp(240,-(myDSH->puzzle_ui_top-320.f)/2.f+3));
	addChild(bottom_case, kNewMainFlowZorder_uiButton);
	
	character_img = CCSprite::create("talk_char_hero.png");
	character_img->setAnchorPoint(ccp(0.2f,0));
	character_img->setPosition(ccp(-240,0));
	bottom_case->addChild(character_img);
	
	beautystone_node = CCNode::create();
	beautystone_node->setPosition(ccp(-215, 0));
	bottom_case->addChild(beautystone_node);
	
	setBeautystoneMenu();
	
	CCSprite* n_shop = CCSprite::create("mainflow_new_character.png");
	CCSprite* s_shop = CCSprite::create("mainflow_new_character.png");
	s_shop->setColor(ccGRAY);
	
	CCMenuItem* shop_item = CCMenuItemSprite::create(n_shop, s_shop, this, menu_selector(NewMainFlowScene::menuAction));
	shop_item->setTag(kNewMainFlowMenuTag_shop);
	
	CCMenu* shop_menu = CCMenu::createWithItem(shop_item);
	shop_menu->setPosition(ccp(-159, n_shop->getContentSize().height/2.f));
	bottom_case->addChild(shop_menu);
	
	
	CCSprite* n_cardsetting = CCSprite::create("mainflow_new_collection.png");
	CCSprite* s_cardsetting = CCSprite::create("mainflow_new_collection.png");
	s_cardsetting->setColor(ccGRAY);
	
	CCMenuItem* cardsetting_item = CCMenuItemSprite::create(n_cardsetting, s_cardsetting, this, menu_selector(NewMainFlowScene::menuAction));
	cardsetting_item->setTag(kNewMainFlowMenuTag_cardSetting);
	
	CCMenu* cardsetting_menu = CCMenu::createWithItem(cardsetting_item);
	cardsetting_menu->setPosition(ccp(-103, n_cardsetting->getContentSize().height/2.f));
	bottom_case->addChild(cardsetting_menu);
	
	
	CCSprite* n_rank = CCSprite::create("mainflow_new_rank.png");
	CCSprite* s_rank = CCSprite::create("mainflow_new_rank.png");
	s_rank->setColor(ccGRAY);
	
	CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(NewMainFlowScene::menuAction));
	rank_item->setTag(kNewMainFlowMenuTag_rank);
	
	CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
	rank_menu->setPosition(ccp(-47, n_rank->getContentSize().height/2.f));//ccp(-215, n_rank->getContentSize().height/2.f));
	bottom_case->addChild(rank_menu);
	
	
	CCSprite* n_friendmanagement = CCSprite::create("mainflow_new_friendmanagement.png");
	CCSprite* s_friendmanagement = CCSprite::create("mainflow_new_friendmanagement.png");
	s_friendmanagement->setColor(ccGRAY);
	
	CCMenuItem* friendmanagement_item = CCMenuItemSprite::create(n_friendmanagement, s_friendmanagement, this, menu_selector(NewMainFlowScene::menuAction));
	friendmanagement_item->setTag(kNewMainFlowMenuTag_friendManagement);
	
	CCMenu* friendmanagement_menu = CCMenu::createWithItem(friendmanagement_item);
	friendmanagement_menu->setPosition(ccp(9, n_friendmanagement->getContentSize().height/2.f));//ccp(-47, n_friendmanagement->getContentSize().height/2.f));
	bottom_case->addChild(friendmanagement_menu);
	
	
	CCSprite* n_gacha = CCSprite::create("mainflow_new_gacha.png");
	CCSprite* s_gacha = CCSprite::create("mainflow_new_gacha.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(NewMainFlowScene::menuAction));
	gacha_item->setTag(kNewMainFlowMenuTag_gacha);
	
	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
	gacha_menu->setPosition(ccp(65, n_gacha->getContentSize().height/2.f));//ccp(9, n_gacha->getContentSize().height/2.f));
	bottom_case->addChild(gacha_menu);
	
	
	CCSprite* n_ready = CCSprite::create("mainflow_new_ready.png");
	CCSprite* s_ready = CCSprite::create("mainflow_new_ready.png");
	s_ready->setColor(ccGRAY);
	
	CCMenuItem* ready_item = CCMenuItemSprite::create(n_ready, s_ready, this, menu_selector(NewMainFlowScene::menuAction));
	ready_item->setTag(kNewMainFlowMenuTag_ready);
	
	ready_menu = CCMenu::createWithItem(ready_item);
	ready_menu->setPosition(ccp(168, n_ready->getContentSize().height/2.f));
	bottom_case->addChild(ready_menu);
}

void NewMainFlowScene::heartRefresh()
{
	CCPoint heart_position = heart_time->getPosition();
	CCNode* heart_parent = heart_time->getParent();
	
	heart_time->removeFromParent();
	
	heart_time = HeartTime::create();
	heart_time->setPosition(heart_position);
	heart_parent->addChild(heart_time);
}

void NewMainFlowScene::setTop()
{
	CCSprite* top_case = CCSprite::create("mainflow_new_top.png");
	top_case->setAnchorPoint(ccp(0.f,1.f));
	top_case->setPosition(ccp(0,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_case, kNewMainFlowZorder_top);
	
//	CCSprite* top_case1 = CCSprite::create("mainflow_top2.png");
//	top_case1->setAnchorPoint(ccp(1.f,1.f));
//	top_case1->setPosition(ccp(0,top_case->getContentSize().height));
//	top_case->addChild(top_case1);
//	
//	CCSprite* top_case2 = CCSprite::create("mainflow_top2.png");
//	top_case2->setAnchorPoint(ccp(0.f,1.f));
//	top_case2->setPosition(ccp(top_case->getContentSize().width,top_case->getContentSize().height));
//	top_case->addChild(top_case2);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(16,top_case->getContentSize().height/2.f-0.5f));
	top_case->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(NewMainFlowScene::menuAction));
	heart_item->setTag(kNewMainFlowMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(91,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_menu);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_gold))->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(147,top_case->getContentSize().height/2.f-5));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(NewMainFlowScene::menuAction));
	gold_item->setTag(kNewMainFlowMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(175,top_case->getContentSize().height/2.f));
	top_case->addChild(gold_menu);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGoodsValue(kGoodsType_ruby))->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(223,top_case->getContentSize().height/2.f-5));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(NewMainFlowScene::menuAction));
	ruby_item->setTag(kNewMainFlowMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(246,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_menu);
	
//	friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
//	friend_point_label->setPosition(ccp(294,top_case->getContentSize().height/2.f-5));
//	top_case->addChild(friend_point_label);
	
//	mySGD->setFriendPointLabel(friend_point_label);
	
	CCSprite* n_friend_point = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_friend_point = CCSprite::create("mainflow_top_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(NewMainFlowScene::menuAction));
	friend_point_item->setTag(kNewMainFlowMenuTag_friendPointContent);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(316,top_case->getContentSize().height/2.f));
	top_case->addChild(friend_point_menu);
	
	CCSprite* n_postbox = CCSprite::create("mainflow_new_postbox.png");
	CCSprite* s_postbox = CCSprite::create("mainflow_new_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(NewMainFlowScene::menuAction));
	postbox_item->setTag(kNewMainFlowMenuTag_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccp(352,top_case->getContentSize().height/2.f));
	top_case->addChild(postbox_menu);
	
	postbox_count_case = CCSprite::create("mainflow_new.png");//"mainflow_postbox_count.png");
	postbox_count_case->setPosition(ccp(364,top_case->getContentSize().height/2.f+6));
	top_case->addChild(postbox_count_case);
	postbox_count_case->setVisible(false);
	
//	postbox_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
//	postbox_count_label->setColor(ccc3(95, 60, 30));
//	postbox_count_label->setPosition(ccp(postbox_count_case->getContentSize().width/2.f-0.5f, postbox_count_case->getContentSize().height/2.f+0.5f));
//	postbox_count_case->addChild(postbox_count_label);
	
	countingMessage();
	
	
	
	CCSprite* n_achievement = CCSprite::create("mainflow_new_achievement.png");
	CCSprite* s_achievement = CCSprite::create("mainflow_new_achievement.png");
	s_achievement->setColor(ccGRAY);
	
	CCMenuItem* achievement_item = CCMenuItemSprite::create(n_achievement, s_achievement, this, menu_selector(NewMainFlowScene::menuAction));
	achievement_item->setTag(kNewMainFlowMenuTag_achievement);
	
	CCMenu* achievement_menu = CCMenu::createWithItem(achievement_item);
	achievement_menu->setPosition(ccp(391, top_case->getContentSize().height/2.f));
	top_case->addChild(achievement_menu);
	
	achievement_count_case = CCSprite::create("mainflow_postbox_count.png");
	achievement_count_case->setPosition(ccp(403,top_case->getContentSize().height/2.f+6));
	top_case->addChild(achievement_count_case);
	achievement_count_case->setVisible(false);
	
	achievement_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
	achievement_count_label->setColor(ccc3(95, 60, 30));
	achievement_count_label->setPosition(ccp(achievement_count_case->getContentSize().width/2.f-0.5f, achievement_count_case->getContentSize().height/2.f + 0));
	achievement_count_case->addChild(achievement_count_label);
	
	countingAchievement();
	
	
	
	
	CCSprite* n_option = CCSprite::create("mainflow_new_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_new_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(NewMainFlowScene::menuAction));
	option_item->setTag(kNewMainFlowMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(430,top_case->getContentSize().height/2.f));
	top_case->addChild(option_menu);
	
	CCSprite* n_tip = CCSprite::create("mainflow_new_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_new_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(NewMainFlowScene::menuAction));
	tip_item->setTag(kNewMainFlowMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(463,top_case->getContentSize().height/2.f));
	top_case->addChild(tip_menu);
	
	
	CCSprite* n_event = CCSprite::create("mainflow_new_event.png");
	CCSprite* s_event = CCSprite::create("mainflow_new_event.png");
	s_event->setColor(ccGRAY);
	
	CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(NewMainFlowScene::menuAction));
	event_item->setTag(kNewMainFlowMenuTag_event);
	
	CCMenu* event_menu = CCMenu::createWithItem(event_item);
	event_menu->setPosition(ccp(450,-n_event->getContentSize().height/2.f-5));
	top_case->addChild(event_menu);

}

void NewMainFlowScene::countingMessage()
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

void NewMainFlowScene::countingAchievement()
{
	int reward_achieve_count = myAchieve->isHaveRewardCount();
	
	if(reward_achieve_count > 0)
	{
		achievement_count_case->setVisible(true);
		
		if(reward_achieve_count < 10)
		{
			achievement_count_label->setFontSize(10);
			achievement_count_label->setString(CCString::createWithFormat("%d", reward_achieve_count)->getCString());
		}
		else if(reward_achieve_count < 100)
		{
			achievement_count_label->setFontSize(7);
			achievement_count_label->setString(CCString::createWithFormat("%d", reward_achieve_count)->getCString());
		}
		else
		{
			achievement_count_label->setFontSize(8);
			achievement_count_label->setString("...");
		}
	}
	else
	{
		achievement_count_case->setVisible(false);
		achievement_count_label->setString("0");
	}
}

void NewMainFlowScene::popupClose()
{
	is_menu_enable = true;
}

void NewMainFlowScene::mailPopupClose()
{
	countingMessage();
	is_menu_enable = true;
}

void NewMainFlowScene::achievePopupClose()
{
	countingAchievement();
	is_menu_enable = true;
}

void NewMainFlowScene::tutorialCardSettingClose()
{
	is_menu_enable = true;
	puzzle_table->setTouchEnabled(true);
	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
	setBeautystoneMenu();
}

void NewMainFlowScene::closeFriendPoint()
{
	close_friend_point_action();
}

NewMainFlowScene::~NewMainFlowScene()
{
	hspConnector::get()->removeTarget(this);
}

void NewMainFlowScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void NewMainFlowScene::keyBackClicked()
{
	
	CommonButton::callBackKey();
//	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(LK::kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(NewMainFlowScene::alertAction));
}