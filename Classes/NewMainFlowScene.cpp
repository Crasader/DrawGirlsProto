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
	
	setKeypadEnabled(true);
	
	int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	for(int i=1;i<=puzzle_count;i++)
	{
		int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
		int have_card_cnt = 0;
		int start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
		int stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
		
		int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=card_take_cnt;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			int card_stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
			if(card_stage_number >= start_stage && card_stage_number < start_stage+stage_count && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
				have_card_cnt++;
		}
		
		have_card_count_for_puzzle_index.push_back(have_card_cnt);
		
		
		
		vector<PuzzlePiecePath> puzzle_path_info;
		for(int j = start_stage;j<start_stage + stage_count;j++)
		{
			PuzzlePiecePath t_path;
			t_path.piece_no = NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i, j);
			t_path.stage_no = j;
			puzzle_path_info.push_back(t_path);
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
	}
	
	
	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kNewMainFlowZorder_back);
	
//	is_unlock_puzzle = mySGD->getIsUnlockPuzzle();
//	mySGD->setIsUnlockPuzzle(0);
	
	setTable();
	
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
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
			t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
			t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
			
			NoticeContent* t_container = NoticeContent::create(t_popup->getTouchPriority(), [=](CCObject* sender)
															   {
																   t_popup->removeFromParent();
															   }, mySGD->getNoticeList());
			t_popup->setContainerNode(t_container);
			addChild(t_popup, kNewMainFlowZorder_popup);
		}
		
		myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
	}
	
	is_menu_enable = true;
	
	
	
	
	if(myDSH->getPuzzleMapSceneShowType() == kPuzzleMapSceneShowType_clear)
	{
		myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		if(selected_card_number > 0)
		{
			int durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number) + 1;
			myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number, durability);
		}
		
		int take_level;
		if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 3;
		else if(mySGD->is_exchanged || mySGD->is_showtime)	take_level = 2;
		else												take_level = 1;
		
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
//		if(is_unlock_puzzle > 0)
//		{
//			is_menu_enable = false;
//			puzzle_table->setTouchEnabled(false);
//		}
	}
	
	new_stage_info_view = NewStageInfoView::create(-190);
	addChild(new_stage_info_view, kNewMainFlowZorder_right);
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	new_stage_info_view->setClickedStage(myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, puzzle_number));
	
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
	is_menu_enable = true;
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
	
	CCSize table_size = CCSizeMake(480*screen_scale_x, 245);
	
	//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
	//	temp_back->setOpacity(100);
	//	temp_back->setAnchorPoint(CCPointZero);
	//	temp_back->setPosition(ccp(myDSH->ui_zero_point.x, 160-table_size.height/2.f));
	//	addChild(temp_back, kMainFlowZorder_table);
	
	puzzle_table = CCTableView::create(this, table_size);
	puzzle_table->setAnchorPoint(CCPointZero);
	puzzle_table->setDirection(kCCScrollViewDirectionHorizontal);
	puzzle_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	puzzle_table->setPosition(ccp((-480.f*screen_scale_x+480.f)/2.f, 160-table_size.height/2.f));
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
	float xInitPosition = MAX(puzzle_table->minContainerOffset().x, -cellSizeForTable(puzzle_table).width*myPosition);
	xInitPosition = MIN(0, xInitPosition);
	puzzle_table->setContentOffsetInDuration(ccp(xInitPosition, 0), 0.3f);
}

void NewMainFlowScene::cellAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	int tag = ((CCNode*)sender)->getTag();

	is_menu_enable = false;
	
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
//		CCLog("puzzle_number : %d", puzzle_number);
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
//		if(mySGD->getStar() >= NSDS_GI(puzzle_number, kSDS_PZ_point_i))
//		{
//			CCSprite* n_buy = CCSprite::create("popup2_buy.png");
//			CCSprite* s_buy = CCSprite::create("popup2_buy.png");
//			s_buy->setColor(ccGRAY);
//			
//			CCMenuItemSpriteLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender){
//				mySGD->setStar(mySGD->getStar() - NSDS_GI(puzzle_number, kSDS_PZ_point_i));
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
CCTableViewCell* NewMainFlowScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	if(idx == numberOfCellsInTableView(table)-1)
	{
		CCNode* puzzle_node = CCNode::create();
		puzzle_node->setPosition(ccp(173.5f,111));
		cell->addChild(puzzle_node);
		
		CCSprite* puzzle_left = CCSprite::create("temp_puzzle_stencil_left.png");
		puzzle_left->setAnchorPoint(ccp(0,0.5));
		puzzle_left->setPosition(ccp(-161,0));
		puzzle_node->addChild(puzzle_left);
		
		CCSprite* puzzle_right = CCSprite::create("temp_puzzle_stencil_right.png");
		puzzle_right->setAnchorPoint(ccp(1,0.5));
		puzzle_right->setPosition(ccp(161,0));
		puzzle_node->addChild(puzzle_right);
		
		CCSprite* puzzle_top = CCSprite::create("temp_puzzle_stencil_top.png");
		puzzle_top->setAnchorPoint(ccp(0.5,1));
		puzzle_top->setPosition(ccp(0,111));
		puzzle_node->addChild(puzzle_top);
		
		CCSprite* puzzle_bottom = CCSprite::create("temp_puzzle_stencil_bottom.png");
		puzzle_bottom->setAnchorPoint(ccp(0.5,0));
		puzzle_bottom->setPosition(ccp(0,-111));
		puzzle_node->addChild(puzzle_bottom);
		
		
		for(int y = 0;y<4;y++)
		{
			for(int x = 0;x<6;x++)
			{
				string piece_type;
				if((x+y)%2 == 0)
					piece_type = "h";
				else
					piece_type = "w";
				
				CCSprite* puzzle_piece = CCSprite::create(("temp_puzzle_back_p" + piece_type + ".png").c_str());
				puzzle_piece->setPosition(ccp(-161+36+x*50, -111+36+150-y*50));
				puzzle_node->addChild(puzzle_piece);
			}
		}
		
		CCLabelTTF* update_ment = CCLabelTTF::create("업데이트는 계속 됩니다.\nComing soon", mySGD->getFont().c_str(), 15);
		update_ment->setPosition(ccp(0,0));
		puzzle_node->addChild(update_ment);
		
		CCSprite* puzzle_before_bridge = CCSprite::create("temp_puzzle_bridge_front_ph.png");
		puzzle_before_bridge->setPosition(ccp(-161+36-1*50, -111+36+150-0*50));
		puzzle_node->addChild(puzzle_before_bridge);
	}
	else
	{
		int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx+1);
		cell->setTag(puzzle_number);
		
		if(puzzle_number == is_unlock_puzzle)
		{
			if(NSDS_GI(puzzle_number, kSDS_PZ_point_i) <= 0 || NSDS_GI(puzzle_number, kSDS_PZ_ticket_i) <= 0)
				myDSH->setIntegerForKey(kDSH_Key_openPuzzleCnt, myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1);
		}
		
		vector<PuzzlePiecePath> puzzle_path = puzzle_piece_path[puzzle_number];
		int puzzle_path_idx = 0;
		
		CCNode* puzzle_node = CCNode::create();
		puzzle_node->setPosition(ccp(173.5f,111));
		cell->addChild(puzzle_node);
		
		if(puzzle_number == 1 || myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1 >= puzzle_number)
			//	if(puzzle_number == 1 || 9999+1 >= puzzle_number)
		{
			CCSprite* puzzle_left = CCSprite::create("temp_puzzle_stencil_left.png");
			puzzle_left->setAnchorPoint(ccp(0,0.5));
			puzzle_left->setPosition(ccp(-161,0));
			puzzle_node->addChild(puzzle_left);
			
			CCSprite* puzzle_right = CCSprite::create("temp_puzzle_stencil_right.png");
			puzzle_right->setAnchorPoint(ccp(1,0.5));
			puzzle_right->setPosition(ccp(161,0));
			puzzle_node->addChild(puzzle_right);
			
			CCSprite* puzzle_top = CCSprite::create("temp_puzzle_stencil_top.png");
			puzzle_top->setAnchorPoint(ccp(0.5,1));
			puzzle_top->setPosition(ccp(0,111));
			puzzle_node->addChild(puzzle_top);
			
			CCSprite* puzzle_bottom = CCSprite::create("temp_puzzle_stencil_bottom.png");
			puzzle_bottom->setAnchorPoint(ccp(0.5,0));
			puzzle_bottom->setPosition(ccp(0,-111));
			puzzle_node->addChild(puzzle_bottom);
			
			
			for(int y = 0;y<4;y++)
			{
				for(int x = 0;x<6;x++)
				{
					int piece_number = y*6+x+1;
					bool is_stage = false;
					int stage_number = -1;
					if(piece_number == puzzle_path[puzzle_path_idx].piece_no)
					{
						is_stage = true;
						stage_number = puzzle_path[puzzle_path_idx].stage_no;
						puzzle_path_idx++;
					}
					
					if(is_stage)
					{
						bool is_buy, is_lock;
						if(stage_number == 1 || myDSH->getBoolForKey(kDSH_Key_isOpenStage_int1, stage_number) ||
						   (NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number) == 0 &&
							(NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number) == 0 || myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))))
						{
							is_buy = false;
							is_lock = false;
						}
						else
						{
							if(myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, stage_number)))
								is_buy = true;
							else
								is_lock = true;
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
						
						NewPuzzlePiece* t_piece = NewPuzzlePiece::create(stage_number, clicked_func, (NewPuzzlePieceMode)puzzle_piece_mode[idx], is_buy, is_lock);
						t_piece->setPosition(ccp(-161+36+x*50, -111+36+150-y*50));
						puzzle_node->addChild(t_piece);
					}
					else
					{
						string piece_type;
						if((x+y)%2 == 0)
							piece_type = "h";
						else
							piece_type = "w";
						
						CCSprite* puzzle_piece = CCSprite::create(("temp_puzzle_stencil_p" + piece_type + ".png").c_str());
						puzzle_piece->setPosition(ccp(-161+36+x*50, -111+36+150-y*50));
						puzzle_node->addChild(puzzle_piece);
					}
				}
			}
			if(puzzle_number != 1)
			{
				CCSprite* puzzle_before_bridge = CCSprite::create("temp_puzzle_bridge_front_ph.png");
				puzzle_before_bridge->setPosition(ccp(-161+36-1*50, -111+36+150-0*50));
				puzzle_node->addChild(puzzle_before_bridge);
			}
			
			CCSprite* puzzle_after_bridge = CCSprite::create("temp_puzzle_bridge_front_ph.png");
			puzzle_after_bridge->setPosition(ccp(-161+36+6*50, -111+36+150-0*50));
			puzzle_node->addChild(puzzle_after_bridge);
		
//		CCSprite* n_open_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());//CCSprite::create("mainflow_puzzle_open_back.png");
//		CCSprite* s_open_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());//CCSprite::create("mainflow_puzzle_open_back.png");
//		s_open_back->setColor(ccGRAY);
//		
//		CCMenuItem* open_item = CCMenuItemSprite::create(n_open_back, s_open_back, this, menu_selector(NewMainFlowScene::cellAction));
//		open_item->setTag(kMainFlowTableCellTag_openBase + puzzle_number);
//		
//		ScrollMenu* open_menu = ScrollMenu::create(open_item, NULL);
//		open_menu->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f));
//		cell->addChild(open_menu);
//		
//		int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
//		int total_card_cnt = stage_count*3;
//		
//		CCLabelTTF* rate_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", have_card_count_for_puzzle_index[idx], total_card_cnt)->getCString(), mySGD->getFont().c_str(), 10);
//		rate_label->setPosition(ccp(cellSizeForTable(table).width/2.f-25, cellSizeForTable(table).height/2.f-81));
//		cell->addChild(rate_label);
//		
//		CCProgressTimer* rate_timer = CCProgressTimer::create(CCSprite::create("mainflow_puzzle_bar.png"));
//		rate_timer->setType(kCCProgressTimerTypeBar);
//		rate_timer->setMidpoint(ccp(0,0));
//		rate_timer->setBarChangeRate(ccp(1,0));
//		rate_timer->setPercentage(100.f*have_card_count_for_puzzle_index[idx]/total_card_cnt);
//		rate_timer->setPosition(ccp(cellSizeForTable(table).width/2.f+22, cellSizeForTable(table).height/2.f-80));
//		cell->addChild(rate_timer);
//		
//		
//		PuzzleListShadow* shadow_node = PuzzleListShadow::create(this, cell, ccpAdd(ccp((-480.f*screen_scale_x+480.f)/2.f, 160-table_size.height/2.f), ccp(table_size.width/2.f, table_size.height/2.f)), ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f), ccp(1.f,0), ccp(0.2f,0));
//		cell->addChild(shadow_node, -1);
//		shadow_node->startAction();
//		
//		CCSprite* shadow_img = CCSprite::create("mainflow_puzzle_shadow.png");
//		shadow_node->addChild(shadow_img, -1);
		}
		else
		{
			CCSprite* puzzle_left = CCSprite::create("temp_puzzle_stencil_left.png");
			puzzle_left->setAnchorPoint(ccp(0,0.5));
			puzzle_left->setPosition(ccp(-161,0));
			puzzle_node->addChild(puzzle_left);
			
			CCSprite* puzzle_right = CCSprite::create("temp_puzzle_stencil_right.png");
			puzzle_right->setAnchorPoint(ccp(1,0.5));
			puzzle_right->setPosition(ccp(161,0));
			puzzle_node->addChild(puzzle_right);
			
			CCSprite* puzzle_top = CCSprite::create("temp_puzzle_stencil_top.png");
			puzzle_top->setAnchorPoint(ccp(0.5,1));
			puzzle_top->setPosition(ccp(0,111));
			puzzle_node->addChild(puzzle_top);
			
			CCSprite* puzzle_bottom = CCSprite::create("temp_puzzle_stencil_bottom.png");
			puzzle_bottom->setAnchorPoint(ccp(0.5,0));
			puzzle_bottom->setPosition(ccp(0,-111));
			puzzle_node->addChild(puzzle_bottom);
			
			
			for(int y = 0;y<4;y++)
			{
				for(int x = 0;x<6;x++)
				{
					int piece_number = y*6+x+1;
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
					
					CCSprite* puzzle_piece = CCSprite::create(("temp_puzzle_back_p" + piece_type + ".png").c_str());
					puzzle_piece->setPosition(ccp(-161+36+x*50, -111+36+150-y*50));
					puzzle_node->addChild(puzzle_piece);
					
					if(is_stage)
						puzzle_piece->setColor(ccGREEN);
				}
			}
			
			CCSprite* puzzle_before_bridge = CCSprite::create("temp_puzzle_bridge_front_ph.png");
			puzzle_before_bridge->setPosition(ccp(-161+36-1*50, -111+36+150-0*50));
			puzzle_node->addChild(puzzle_before_bridge);
			
			CCSprite* puzzle_after_bridge = CCSprite::create("temp_puzzle_bridge_front_ph.png");
			puzzle_after_bridge->setPosition(ccp(-161+36+6*50, -111+36+150-0*50));
			puzzle_node->addChild(puzzle_after_bridge);
			
//		CCSprite* close_back = CCSprite::create("mainflow_puzzle_lock_back.png");
//		close_back->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f));
//		cell->addChild(close_back);
//		
//		CCLabelTTF* title_label = CCLabelTTF::create(NSDS_GS(puzzle_number, kSDS_PZ_title_s).c_str(), mySGD->getFont().c_str(), 10);
//		title_label->setColor(ccBLACK);
//		title_label->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f-57));
//		cell->addChild(title_label);
//		
//		if(myDSH->getBoolForKey(kDSH_Key_isClearedPuzzle_int1, puzzle_number-1))
//		{
//			CCSprite* need_ticket_img = CCSprite::create("mainflow_puzzle_needticket.png");
//			need_ticket_img->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+36));
//			cell->addChild(need_ticket_img);
//			
//			CCLabelTTF* ticket_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt),
//																					 NSDS_GI(puzzle_number, kSDS_PZ_ticket_i))->getCString(), mySGD->getFont().c_str(), 13);
//			ticket_label->setPosition(ccp(cellSizeForTable(table).width/2.f+15, cellSizeForTable(table).height/2.f+27));
//			cell->addChild(ticket_label);
//			
//			CCSprite* n_buy = CCSprite::create("mainflow_puzzle_open_buy.png");
//			CCSprite* s_buy = CCSprite::create("mainflow_puzzle_open_buy.png");
//			s_buy->setColor(ccGRAY);
//			
//			CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, this, menu_selector(MainFlowScene::cellAction));
//			buy_item->setTag(kMainFlowTableCellTag_buyBase + puzzle_number);
//			
//			ScrollMenu* buy_menu = ScrollMenu::create(buy_item, NULL);
//			buy_menu->setPosition(ccp(cellSizeForTable(table).width/2.f-24, cellSizeForTable(table).height/2.f-16));
//			cell->addChild(buy_menu);
//			
//			CCSprite* n_ticket = CCSprite::create("mainflow_puzzle_open_ticket.png");
//			CCSprite* s_ticket = CCSprite::create("mainflow_puzzle_open_ticket.png");
//			s_ticket->setColor(ccGRAY);
//			
//			CCMenuItem* ticket_item = CCMenuItemSprite::create(n_ticket, s_ticket, this, menu_selector(MainFlowScene::cellAction));
//			ticket_item->setTag(kMainFlowTableCellTag_ticketBase + puzzle_number);
//			
//			ScrollMenu* ticket_menu = ScrollMenu::create(ticket_item, NULL);
//			ticket_menu->setPosition(ccp(cellSizeForTable(table).width/2.f+24, cellSizeForTable(table).height/2.f-16));
//			cell->addChild(ticket_menu);
//		}
//		else
//		{
//			CCSprite* not_clear_img = CCSprite::create("mainflow_puzzle_lock_base.png");
//			not_clear_img->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+33));
//			cell->addChild(not_clear_img);
//		}
//		
//		PuzzleListShadow* shadow_node = PuzzleListShadow::create(this, cell, ccpAdd(ccp((-480.f*screen_scale_x+480.f)/2.f, 160-table_size.height/2.f), ccp(table_size.width/2.f, table_size.height/2.f)), ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f), ccp(1.f,0), ccp(0.2f,0));
//		cell->addChild(shadow_node, -1);
//		shadow_node->startAction();
//		
//		CCSprite* shadow_img = CCSprite::create("mainflow_puzzle_shadow.png");
//		shadow_node->addChild(shadow_img, -1);
		}
	
//	if(puzzle_number == is_unlock_puzzle)
//	{
//		CCSprite* close_back = CCSprite::create("mainflow_puzzle_lock_back.png");
//		close_back->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f));
//		cell->addChild(close_back);
//		
//		CCDelayTime* t_delay = CCDelayTime::create(0.25f);
//		CCFadeTo* t_fade1 = CCFadeTo::create(0.4f, 0);
//		CCCallFunc* t_remove_self = CCCallFunc::create(close_back, callfunc_selector(CCNode::removeFromParent));
//		CCSequence* t_seq = CCSequence::create(t_delay, t_fade1, t_remove_self, NULL);
//		close_back->runAction(t_seq);
//		
//		
//		CCSprite* not_clear_img = CCSprite::create("mainflow_puzzle_lock_base.png");
//		not_clear_img->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+33));
//		cell->addChild(not_clear_img);
//		
//		CCDelayTime* t_delay1 = CCDelayTime::create(0.25f);
//		CCFadeTo* t_fade2 = CCFadeTo::create(0.4f, 0);
//		CCCallFunc* t_touch_on = CCCallFunc::create(this, callfunc_selector(MainFlowScene::endUnlockAnimation));
//		CCCallFunc* t_remove_self2 = CCCallFunc::create(not_clear_img, callfunc_selector(CCNode::removeFromParent));
//		CCSequence* t_seq2 = CCSequence::create(t_delay1, t_fade2, t_touch_on, t_remove_self2, NULL);
//		not_clear_img->runAction(t_seq2);
//		
//		is_unlock_puzzle = 0;
//	}
	
	}
	return cell;
}

void NewMainFlowScene::pieceAction(int t_stage_number)
{
	CCLog("pieceAction : %d", t_stage_number);
}
void NewMainFlowScene::buyPieceAction(int t_stage_number)
{
	CCLog("buyPieceAction : %d", t_stage_number);
}
void NewMainFlowScene::lockPieceAction(int t_stage_number)
{
	CCLog("lockPieceAction : %d", t_stage_number);
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
	return CCSizeMake(347, 222);
}
unsigned int NewMainFlowScene::numberOfCellsInTableView(CCTableView *table)
{
	return NSDS_GI(kSDS_GI_puzzleListCount_i)+1;
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
	kNewMainFlowMenuTag_cardSetting,
	kNewMainFlowMenuTag_friendManagement,
	kNewMainFlowMenuTag_gacha,
	kNewMainFlowMenuTag_achievement,
	kNewMainFlowMenuTag_event,
	kNewMainFlowMenuTag_ready
};

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
		stencil_node->setPosition(ccp(336,229+160.f*(screen_scale_y-1.f)));
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
		cliping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
		cliping_node->setAlphaThreshold(0.05f);
		cliping_node->setPosition(CCPointZero);
		t_suction->addChild(cliping_node);
		
		CCSprite* inner_img = CCSprite::create("candy_popup.png");
		inner_img->setPosition(ccp(336,229+160.f*(screen_scale_y-1.f)+150));
		cliping_node->addChild(inner_img);
		
		CCMoveTo* t_move_down = CCMoveTo::create(0.3f, ccp(336,229+160.f*(screen_scale_y-1.f)));
		inner_img->runAction(t_move_down);
		
		close_friend_point_action = [=](){
			t_suction->target_touch_began = NULL;
			t_suction->delegate_touch_began = NULL;
			
			CCMoveTo* t_move_up = CCMoveTo::create(0.3f, ccp(336,229+160.f*(screen_scale_y-1.f)+150));
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
		is_menu_enable = true; // 임시
		//			EventPopup* t_popup = EventPopup::create();
		//			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
		//			addChild(t_popup, kMainFlowZorder_popup);
	}
	else if(tag == kNewMainFlowMenuTag_ready)
	{
		is_menu_enable = true;
	}
}

void NewMainFlowScene::setBottom()
{
	CCNode* bottom_case = CCNode::create();
	bottom_case->setAnchorPoint(ccp(0.5f,0.5f));
	bottom_case->setPosition(ccp(240,-(myDSH->puzzle_ui_top-320.f)/2.f+3));
	addChild(bottom_case, kNewMainFlowZorder_uiButton);
	
	CCSprite* n_rank = CCSprite::create("mainflow_new_rank.png");
	CCSprite* s_rank = CCSprite::create("mainflow_new_rank.png");
	s_rank->setColor(ccGRAY);
	
	CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(NewMainFlowScene::menuAction));
	rank_item->setTag(kNewMainFlowMenuTag_rank);
	
	CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
	rank_menu->setPosition(ccp(-212, n_rank->getContentSize().height/2.f));
	bottom_case->addChild(rank_menu);
	
	
	CCSprite* n_shop = CCSprite::create("mainflow_new_shop.png");
	CCSprite* s_shop = CCSprite::create("mainflow_new_shop.png");
	s_shop->setColor(ccGRAY);
	
	CCMenuItem* shop_item = CCMenuItemSprite::create(n_shop, s_shop, this, menu_selector(NewMainFlowScene::menuAction));
	shop_item->setTag(kNewMainFlowMenuTag_shop);
	
	CCMenu* shop_menu = CCMenu::createWithItem(shop_item);
	shop_menu->setPosition(ccp(-153, n_shop->getContentSize().height/2.f));
	bottom_case->addChild(shop_menu);
	
	
	CCSprite* n_cardsetting = CCSprite::create("mainflow_new_cardsetting.png");
	CCSprite* s_cardsetting = CCSprite::create("mainflow_new_cardsetting.png");
	s_cardsetting->setColor(ccGRAY);
	
	CCMenuItem* cardsetting_item = CCMenuItemSprite::create(n_cardsetting, s_cardsetting, this, menu_selector(NewMainFlowScene::menuAction));
	cardsetting_item->setTag(kNewMainFlowMenuTag_cardSetting);
	
	CCMenu* cardsetting_menu = CCMenu::createWithItem(cardsetting_item);
	cardsetting_menu->setPosition(ccp(-94, n_cardsetting->getContentSize().height/2.f));
	bottom_case->addChild(cardsetting_menu);
	
	
	CCSprite* n_friendmanagement = CCSprite::create("mainflow_new_friendmanagement.png");
	CCSprite* s_friendmanagement = CCSprite::create("mainflow_new_friendmanagement.png");
	s_friendmanagement->setColor(ccGRAY);
	
	CCMenuItem* friendmanagement_item = CCMenuItemSprite::create(n_friendmanagement, s_friendmanagement, this, menu_selector(NewMainFlowScene::menuAction));
	friendmanagement_item->setTag(kNewMainFlowMenuTag_friendManagement);
	
	CCMenu* friendmanagement_menu = CCMenu::createWithItem(friendmanagement_item);
	friendmanagement_menu->setPosition(ccp(-35, n_friendmanagement->getContentSize().height/2.f));
	bottom_case->addChild(friendmanagement_menu);
	
	
	CCSprite* n_gacha = CCSprite::create("mainflow_new_gacha.png");
	CCSprite* s_gacha = CCSprite::create("mainflow_new_gacha.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(NewMainFlowScene::menuAction));
	gacha_item->setTag(kNewMainFlowMenuTag_gacha);
	
	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
	gacha_menu->setPosition(ccp(24, n_gacha->getContentSize().height/2.f));
	bottom_case->addChild(gacha_menu);
	
	
	CCSprite* n_event = CCSprite::create("mainflow_new_event.png");
	CCSprite* s_event = CCSprite::create("mainflow_new_event.png");
	s_event->setColor(ccGRAY);
	
	CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(NewMainFlowScene::menuAction));
	event_item->setTag(kNewMainFlowMenuTag_event);
	
	CCMenu* event_menu = CCMenu::createWithItem(event_item);
	event_menu->setPosition(ccp(83, n_event->getContentSize().height/2.f));
	bottom_case->addChild(event_menu);
	
	
	CCSprite* n_ready = CCSprite::create("mainflow_new_ready.png");
	CCLabelTTF* n_ment = CCLabelTTF::create("준비하기", mySGD->getFont().c_str(), 17);
	n_ment->setPosition(ccp(n_ready->getContentSize().width/2.f-2, n_ready->getContentSize().height/2.f+5));
	n_ready->addChild(n_ment);
	CCSprite* s_ready = CCSprite::create("mainflow_new_ready.png");
	s_ready->setColor(ccGRAY);
	CCLabelTTF* s_ment = CCLabelTTF::create("준비하기", mySGD->getFont().c_str(), 17);
	s_ment->setPosition(ccp(s_ready->getContentSize().width/2.f-2, s_ready->getContentSize().height/2.f+5));
	s_ready->addChild(s_ment);
	
	CCMenuItem* ready_item = CCMenuItemSprite::create(n_ready, s_ready, this, menu_selector(NewMainFlowScene::menuAction));
	ready_item->setTag(kNewMainFlowMenuTag_ready);
	
	CCMenu* ready_menu = CCMenu::createWithItem(ready_item);
	ready_menu->setPosition(ccp(178, n_ready->getContentSize().height/2.f));
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
	top_case->setPosition(ccp(80,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_case, kNewMainFlowZorder_top);
	
	CCSprite* top_case1 = CCSprite::create("mainflow_top2.png");
	top_case1->setAnchorPoint(ccp(1.f,1.f));
	top_case1->setPosition(ccp(0,top_case->getContentSize().height));
	top_case->addChild(top_case1);
	
	CCSprite* top_case2 = CCSprite::create("mainflow_top2.png");
	top_case2->setAnchorPoint(ccp(0.f,1.f));
	top_case2->setPosition(ccp(top_case->getContentSize().width,top_case->getContentSize().height));
	top_case->addChild(top_case2);
	
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
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
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
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
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
	
	friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
	friend_point_label->setPosition(ccp(294,top_case->getContentSize().height/2.f-5));
	top_case->addChild(friend_point_label);
	
	mySGD->setFriendPointLabel(friend_point_label);
	
	CCSprite* n_friend_point = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_friend_point = CCSprite::create("mainflow_top_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(NewMainFlowScene::menuAction));
	friend_point_item->setTag(kNewMainFlowMenuTag_friendPointContent);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(316,top_case->getContentSize().height/2.f));
	top_case->addChild(friend_point_menu);
	
	CCSprite* n_postbox = CCSprite::create("mainflow_postbox.png");
	CCSprite* s_postbox = CCSprite::create("mainflow_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(NewMainFlowScene::menuAction));
	postbox_item->setTag(kNewMainFlowMenuTag_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccp(-53,top_case->getContentSize().height/2.f));
	top_case->addChild(postbox_menu);
	
	postbox_count_case = CCSprite::create("mainflow_postbox_count.png");
	postbox_count_case->setPosition(ccp(-41,top_case->getContentSize().height/2.f+6));
	top_case->addChild(postbox_count_case);
	postbox_count_case->setVisible(false);
	
	postbox_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
	postbox_count_label->setColor(ccc3(95, 60, 30));
	postbox_count_label->setPosition(ccp(postbox_count_case->getContentSize().width/2.f-0.5f, postbox_count_case->getContentSize().height/2.f+0.5f));
	postbox_count_case->addChild(postbox_count_label);
	
	countingMessage();
	
	
	
	CCSprite* n_achievement = CCSprite::create("mainflow_new_achievement.png");
	CCSprite* s_achievement = CCSprite::create("mainflow_new_achievement.png");
	s_achievement->setColor(ccGRAY);
	
	CCMenuItem* achievement_item = CCMenuItemSprite::create(n_achievement, s_achievement, this, menu_selector(NewMainFlowScene::menuAction));
	achievement_item->setTag(kNewMainFlowMenuTag_achievement);
	
	CCMenu* achievement_menu = CCMenu::createWithItem(achievement_item);
	achievement_menu->setPosition(ccp(-22, top_case->getContentSize().height/2.f));
	top_case->addChild(achievement_menu);
	
	achievement_count_case = CCSprite::create("mainflow_postbox_count.png");
	achievement_count_case->setPosition(ccp(-10,top_case->getContentSize().height/2.f+6));
	top_case->addChild(achievement_count_case);
	achievement_count_case->setVisible(false);
	
	achievement_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
	achievement_count_label->setColor(ccc3(95, 60, 30));
	achievement_count_label->setPosition(ccp(achievement_count_case->getContentSize().width/2.f-0.5f, achievement_count_case->getContentSize().height/2.f+0.5f));
	achievement_count_case->addChild(achievement_count_label);
	
	countingAchievement();
	
	
	
	
	CCSprite* n_option = CCSprite::create("mainflow_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(NewMainFlowScene::menuAction));
	option_item->setTag(kNewMainFlowMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(350,top_case->getContentSize().height/2.f));
	top_case->addChild(option_menu);
	
	CCSprite* n_tip = CCSprite::create("mainflow_new_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_new_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(NewMainFlowScene::menuAction));
	tip_item->setTag(kNewMainFlowMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(382,top_case->getContentSize().height/2.f));
	top_case->addChild(tip_menu);
}

void NewMainFlowScene::countingMessage()
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

void NewMainFlowScene::countingAchievement()
{
	int reward_achieve_count = 0;
	for(int i=kAchievementCode_base+1;i<kAchievementCode_end;i++)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
		   AchieveConditionReward::sharedInstance()->isAchieve((AchievementCode)i))
			reward_achieve_count++;
	}
	
	for(int i=kAchievementCode_hidden_base+1;i<kAchievementCode_hidden_end;i++)
	{
		if(myDSH->getIntegerForKey(kDSH_Key_achieveData_int1_value, i) != -1 &&
		   AchieveConditionReward::sharedInstance()->isAchieve((AchievementCode)i))
			reward_achieve_count++;
	}
	
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
}

void NewMainFlowScene::closeFriendPoint()
{
	close_friend_point_action();
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
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(NewMainFlowScene::alertAction));
}