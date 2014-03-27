//
//  MainFlowScene.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 16..
//
//

#include "MainFlowScene.h"
#include "DataStorageHub.h"
#include "CountingBMLabel.h"
#include "StarGoldData.h"
#include "HeartTime.h"
#include "ShopPopup.h"
#include "OptionPopup.h"
#include "MailPopup.h"
#include "RankPopup.h"
#include "CardSettingPopup.h"
#include "GachaPurchase.h"
#include "EventPopup.h"
#include "ServerDataSave.h"
#include "ASPopupView.h"
#include "NoticeContent.h"
#include "TicketRequestContent.h"
#include "PuzzleScene.h"
#include "StageListDown.h"
#include "AchievePopup.h"
#include "TutorialFlowStep.h"
#include "ClearPopup.h"
#include "FailPopup.h"
#include "AlertEngine.h"
#include "PuzzleListShadow.h"
#include "InviteEventPopup.h"
#include "TouchSuctionLayer.h"
#include "ScrollMenu.h"
#include "MyLocalization.h"
#include "NewMainFlowScene.h"
#include "RankNewPopup.h"

CCScene* MainFlowScene::scene()
{
    CCScene *scene = CCScene::create();
    
    MainFlowScene *layer = MainFlowScene::create();
	layer->setAnchorPoint(ccp(0.5,0));
	layer->setScale(myDSH->screen_convert_rate);
	layer->setPosition(ccpAdd(layer->getPosition(), myDSH->ui_zero_point));
    scene->addChild(layer);
	
    return scene;
}

bool MainFlowScene::init()
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
	}
	
	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kMainFlowZorder_back);
	
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
	addChild(back_shadow_left, kMainFlowZorder_top);
	
	CCSprite* back_shadow_right = CCSprite::create("mainflow_back_shadow_right.png");
	back_shadow_right->setAnchorPoint(ccp(1.f,0.5f));
	back_shadow_right->setPosition(ccp(480+(screen_scale_x-1.f)*480.f/2.f,160));
	addChild(back_shadow_right, kMainFlowZorder_top);
	
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
			addChild(t_popup, kMainFlowZorder_popup);
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
		TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
		
		if(recent_step == kTutorialFlowStep_puzzleClick)
		{
			if(mySGD->getStar() == 0 && mySGD->getGold() == 0)
			{
				mySGD->setStar(myDSH->getDefaultRuby());
				mySGD->setGold(myDSH->getDefaultGold());
			}
			
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_puzzleClick);
			addChild(t_tutorial, kMainFlowZorder_popup);
			
			tutorial_node = t_tutorial;
			
			puzzle_table->setTouchEnabled(false);
		}
		else if(recent_step == kTutorialFlowStep_cardCollectionClick)
		{
			TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
			t_tutorial->initStep(kTutorialFlowStep_cardCollectionClick);
			addChild(t_tutorial, kMainFlowZorder_popup);
			
			tutorial_node = t_tutorial;
			
			puzzle_table->setTouchEnabled(false);
		}
		
		
//		if(is_unlock_puzzle > 0)
//		{
//			is_menu_enable = false;
//			puzzle_table->setTouchEnabled(false);
//		}
	}
	
//	if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_main))
//	{
//		myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_main, true);
//		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//		t_suction->target_touch_began = t_suction;
//		t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//		t_suction->setTouchEnabled(true);
//		
//		CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_main.png");
//		dimed_tip->setPosition(ccp(240,160));
//		t_suction->addChild(dimed_tip);
//		
//		addChild(t_suction, kMainFlowZorder_popup);
//	}
	
	return true;
}

void MainFlowScene::showClearPopup()
{
	is_menu_enable = false;
	
	ClearPopup* t_popup = ClearPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::hideClearPopup));
	addChild(t_popup, kPuzzleZorder_popup);
}

void MainFlowScene::hideClearPopup()
{
	is_menu_enable = true;
}

void MainFlowScene::showFailPopup()
{
	is_menu_enable = false;
	
	FailPopup* t_popup = FailPopup::create();
	t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::hideFailPopup));
	addChild(t_popup, kPuzzleZorder_popup);
}

void MainFlowScene::hideFailPopup()
{
	is_menu_enable = true;
}

void MainFlowScene::setTable()
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
	addChild(puzzle_table, kMainFlowZorder_table);
	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
	
	int puzzle_number;
	
//	if(is_unlock_puzzle > 0)
//		puzzle_number = is_unlock_puzzle;
//	else
		puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	if(puzzle_number == 0 || puzzle_number < 10000)
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, NSDS_GI(kSDS_GI_eventList_int1_no_i, 1));
	
	int myPosition = puzzle_table->minContainerOffset().x;
	for(int i=0; i<numberOfCellsInTableView(puzzle_table); i++)
	{
		if(puzzle_number == NSDS_GI(kSDS_GI_eventList_int1_no_i, i+1))
		{
			myPosition = i+1;
			break;
		}
	}
	float xInitPosition = MAX(puzzle_table->minContainerOffset().x, -cellSizeForTable(puzzle_table).width*myPosition + puzzle_table->getViewSize().width / 2.f);
	xInitPosition = MIN(0, xInitPosition);
	puzzle_table->setContentOffsetInDuration(ccp(xInitPosition, 0), 0.3f);
}

enum MainFlowTableCellTag{
	kMainFlowTableCellTag_openBase = 0,
	kMainFlowTableCellTag_buyBase = 10000,
	kMainFlowTableCellTag_ticketBase = 20000
};

void MainFlowScene::puzzleLoadSuccess()
{
	CCDirector::sharedDirector()->replaceScene(PuzzleScene::scene());
}

void MainFlowScene::cellAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	int tag = ((CCNode*)sender)->getTag();
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_puzzleClick)
	{
		if(tag < kMainFlowTableCellTag_buyBase && tag - kMainFlowTableCellTag_openBase == 1)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_pieceClick);
			is_menu_enable = false;
			
			int puzzle_number = tag - kMainFlowTableCellTag_openBase;
			myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, puzzle_number);
			
			StageListDown* t_sld = StageListDown::create(this, callfunc_selector(MainFlowScene::puzzleLoadSuccess), puzzle_number);
			addChild(t_sld, kMainFlowZorder_popup);
		}
	}
	else if(recent_step == kTutorialFlowStep_cardCollectionClick)
	{
		
	}
	else
	{
		is_menu_enable = false;
		
//		if(tag < kMainFlowTableCellTag_buyBase)
//		{
			int puzzle_number = tag - kMainFlowTableCellTag_openBase;
			myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, puzzle_number);
			
			StageListDown* t_sld = StageListDown::create(this, callfunc_selector(MainFlowScene::puzzleLoadSuccess), puzzle_number);
			addChild(t_sld, kMainFlowZorder_popup);
//		}
//		else if(tag < kMainFlowTableCellTag_ticketBase) // buyBase
//		{
//			int puzzle_number = tag - kMainFlowTableCellTag_buyBase;
//			CCLog("puzzle_number : %d", puzzle_number);
//			
//			ASPopupView* t_popup = ASPopupView::create(-200);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
//			
//			CCNode* t_container = CCNode::create();
//			t_popup->setContainerNode(t_container);
//			addChild(t_popup);
//			
//			CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
//			case_back->setPosition(CCPointZero);
//			t_container->addChild(case_back);
//			
//			case_back->setContentSize(CCSizeMake(230, 250));
//			
//			CCScale9Sprite* content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//			content_back->setPosition(ccp(0,2));
//			t_container->addChild(content_back);
//			
//			content_back->setContentSize(CCSizeMake(202, 146));
//			
//			CCLabelTTF* title_label = CCLabelTTF::create("지금 열기", mySGD->getFont().c_str(), 20);
//			title_label->setPosition(ccp(0, 102));
//			t_container->addChild(title_label);
//			
//			CCLabelTTF* content_label = CCLabelTTF::create(CCString::createWithFormat("%d Ruby 로 오픈", NSDS_GI(puzzle_number, kSDS_PZ_point_i))->getCString(), mySGD->getFont().c_str(), 18);
//			content_label->setPosition(CCPointZero);
//			t_container->addChild(content_label);
//			
//			CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
//			CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
//			s_close->setColor(ccGRAY);
//			
//			CCMenuItemSpriteLambda* close_item = CCMenuItemSpriteLambda::create(n_close, s_close, [=](CCObject* sender)
//																				{
//																					is_menu_enable = true;
//																					t_popup->removeFromParent();
//																				});
//			
//			CCMenuLambda* close_menu = CCMenuLambda::createWithItem(close_item);
//			close_menu->setTouchPriority(t_popup->getTouchPriority()-1);
//			close_menu->setPosition(ccp(92,105));
//			t_container->addChild(close_menu);
//			
//			if(mySGD->getStar() >= NSDS_GI(puzzle_number, kSDS_PZ_point_i))
//			{
//				CCSprite* n_buy = CCSprite::create("popup2_buy.png");
//				CCSprite* s_buy = CCSprite::create("popup2_buy.png");
//				s_buy->setColor(ccGRAY);
//				
//				CCMenuItemSpriteLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender){
//					mySGD->setStar(mySGD->getStar() - NSDS_GI(puzzle_number, kSDS_PZ_point_i));
//					myDSH->setIntegerForKey(kDSH_Key_openPuzzleCnt, myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1);
//					
//					vector<SaveUserData_Key> save_userdata_list;
//					
//					save_userdata_list.push_back(kSaveUserData_Key_star);
//					save_userdata_list.push_back(kSaveUserData_Key_openPuzzle);
//					
//					myDSH->saveUserData(save_userdata_list, nullptr);
//					
//					int found_idx = -1;
//					for(int i=0;i<numberOfCellsInTableView(puzzle_table) && found_idx == -1;i++)
//					{
//						CCTableViewCell* t_cell = puzzle_table->cellAtIndex(i);
//						if(t_cell)
//						{
//							int cell_card_number = t_cell->getTag();
//							if(cell_card_number == puzzle_number)
//								found_idx = i;
//						}
//					}
//					if(found_idx != -1)
//						puzzle_table->updateCellAtIndex(found_idx);
//					
//					is_menu_enable = true;
//					t_popup->removeFromParent();
//				});
//				
//				CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
//				buy_menu->setTouchPriority(t_popup->getTouchPriority()-1);
//				buy_menu->setPosition(ccp(0,-95));
//				t_container->addChild(buy_menu);
//			}
//			else
//			{
//				CCSprite* buy_img = CCSprite::create("popup2_buy.png");
//				buy_img->setColor(ccc3(100, 100, 100));
//				buy_img->setPosition(ccp(0,-95));
//				t_container->addChild(buy_img);
//			}
//		}
//		else // ticketBase
//		{
//			int puzzle_number = tag - kMainFlowTableCellTag_ticketBase;
//			
//			ASPopupView* t_popup = ASPopupView::create(-200);
//			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
//			
//			TicketRequestContent* t_container = TicketRequestContent::create(t_popup->getTouchPriority(), puzzle_number);
//			t_popup->setContainerNode(t_container);
//			addChild(t_popup);
//			
//			t_container->setRemoveAction([=](){
//				is_menu_enable = true;
//				t_popup->removeFromParent();
//			});
//		}
	}
}
CCTableViewCell* MainFlowScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	CCSize table_size = CCSizeMake(480*screen_scale_x, 245);
	
	int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx+1);
	cell->setTag(puzzle_number);
	
//	if(puzzle_number == is_unlock_puzzle)
//	{
//		if(NSDS_GI(puzzle_number, kSDS_PZ_point_i) <= 0 || NSDS_GI(puzzle_number, kSDS_PZ_ticket_i) <= 0)
//			myDSH->setIntegerForKey(kDSH_Key_openPuzzleCnt, myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1);
//	}
	
//	if(puzzle_number == 1 || myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1 >= puzzle_number)
//	if(puzzle_number == 1 || 9999+1 >= puzzle_number)
	{
		CCSprite* n_open_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());//CCSprite::create("mainflow_puzzle_open_back.png");
		CCSprite* s_open_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());//CCSprite::create("mainflow_puzzle_open_back.png");
		s_open_back->setColor(ccGRAY);
		
		CCMenuItem* open_item = CCMenuItemSprite::create(n_open_back, s_open_back, this, menu_selector(MainFlowScene::cellAction));
		open_item->setTag(kMainFlowTableCellTag_openBase + puzzle_number);
		
		ScrollMenu* open_menu = ScrollMenu::create(open_item, NULL);
		open_menu->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f));
		cell->addChild(open_menu);
		
//		CCLabelTTF* title_label = CCLabelTTF::create(NSDS_GS(puzzle_number, kSDS_PZ_title_s).c_str(), mySGD->getFont().c_str(), 15);
//		title_label->setColor(ccBLACK);
//		title_label->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+67));
//		cell->addChild(title_label);
		
//		CCSprite* puzzle_thumbnail = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());
//		puzzle_thumbnail->setPosition(ccp(cellSizeForTable(table).width/2.f-1, cellSizeForTable(table).height/2.f+13));
//		cell->addChild(puzzle_thumbnail);
		
		int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
		int total_card_cnt = stage_count*3;
		
		CCLabelTTF* rate_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", have_card_count_for_puzzle_index[idx], total_card_cnt)->getCString(), mySGD->getFont().c_str(), 10);
		rate_label->setPosition(ccp(cellSizeForTable(table).width/2.f-25, cellSizeForTable(table).height/2.f-81));
		cell->addChild(rate_label);
		
		CCProgressTimer* rate_timer = CCProgressTimer::create(CCSprite::create("mainflow_puzzle_bar.png"));
		rate_timer->setType(kCCProgressTimerTypeBar);
		rate_timer->setMidpoint(ccp(0,0));
		rate_timer->setBarChangeRate(ccp(1,0));
		rate_timer->setPercentage(100.f*have_card_count_for_puzzle_index[idx]/total_card_cnt);
		rate_timer->setPosition(ccp(cellSizeForTable(table).width/2.f+22, cellSizeForTable(table).height/2.f-80));
		cell->addChild(rate_timer);

		
		PuzzleListShadow* shadow_node = PuzzleListShadow::create(this, cell, ccpAdd(ccp((-480.f*screen_scale_x+480.f)/2.f, 160-table_size.height/2.f), ccp(table_size.width/2.f, table_size.height/2.f)), ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f), ccp(1.f,0), ccp(0.2f,0));
		cell->addChild(shadow_node, -1);
		shadow_node->startAction();
		
		CCSprite* shadow_img = CCSprite::create("mainflow_puzzle_shadow.png");
		shadow_node->addChild(shadow_img, -1);
	}
//	else
//	{
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
//	}
	
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
	
	return cell;
}

//void MainFlowScene::endUnlockAnimation()
//{
//	puzzle_table->setTouchEnabled(true);
//	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
//	is_menu_enable = true;
//}

void MainFlowScene::scrollViewDidScroll(CCScrollView* view){}
void MainFlowScene::scrollViewDidZoom(CCScrollView* view){}
void MainFlowScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell){}
CCSize MainFlowScene::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(141, 245);
}
unsigned int MainFlowScene::numberOfCellsInTableView(CCTableView *table)
{
	return NSDS_GI(kSDS_GI_puzzleListCount_i);// eventListCount_i);
}

//	if(t_tag == kPMS_MT_event)				return_value = ccp(420,-(myDSH->puzzle_ui_top-320.f)/2.f - 100.f); // after move animation
//	else if(t_tag == kPMS_MT_screen)		return_value = ccp(455,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f - 19.f);
//	else if(t_tag == kPMS_MT_showui)		return_value = ccp(240,-(myDSH->puzzle_ui_top-320.f)/2.f + 10.f);
//	else if(t_tag == kPMS_MT_left)			return_value = ccp(75, 180.f);
//	else if(t_tag == kPMS_MT_right)			return_value = ccp(405, 180.f);
//	else if(t_tag == kPMS_MT_up)			return_value = ccp(395, 250);
//	else if(t_tag == kPMS_MT_selectedCard)	return_value = ccp(35, 120.f);
//	else if(t_tag == kPMS_MT_trophy)		return_value = ccp(80, 240.f);
//	else if(t_tag == kPMS_MT_memo)			return_value = ccp(430, 120.f);
//	else if(t_tag == kPMS_MT_titleBox)		return_value = ccp(240, 265);

enum MainFlowMenuTag{
	kMainFlowMenuTag_rubyShop = 1,
	kMainFlowMenuTag_goldShop,
	kMainFlowMenuTag_heartShop,
	kMainFlowMenuTag_friendPointContent,
	kMainFlowMenuTag_postbox,
	kMainFlowMenuTag_option,
	kMainFlowMenuTag_tip,
	kMainFlowMenuTag_rank,
	kMainFlowMenuTag_shop,
	kMainFlowMenuTag_cardSetting,
	kMainFlowMenuTag_friendManagement,
	kMainFlowMenuTag_gacha,
	kMainFlowMenuTag_achievement,
	kMainFlowMenuTag_event
};

void MainFlowScene::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	int tag = ((CCNode*)sender)->getTag();
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_puzzleClick)
	{
		return;
	}
	else if(recent_step == kTutorialFlowStep_cardCollectionClick)
	{
		if(tag == kMainFlowMenuTag_cardSetting)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_upgradeClick);
			is_menu_enable = false;
			
			mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
			CardSettingPopup* t_popup = CardSettingPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::tutorialCardSettingClose));
			addChild(t_popup, kMainFlowZorder_popup);
			
			removeChild(tutorial_node);
		}
	}
	else
	{
		is_menu_enable = false;
		
		if(tag == kMainFlowMenuTag_rubyShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_ruby);
			t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
			addChild(t_shop, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_goldShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_gold);
			t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
			addChild(t_shop, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_heartShop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_heart);
			t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
			addChild(t_shop, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_friendPointContent)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-300);
			t_suction->setTouchEnabled(true);
			t_suction->target_touch_began = this;
			t_suction->delegate_touch_began = callfunc_selector(MainFlowScene::closeFriendPoint);
			
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
			addChild(t_suction, kMainFlowZorder_top-1);
			
			is_menu_enable = true;
		}
		else if(tag == kMainFlowMenuTag_postbox)
		{
			MailPopup* t_pp = MailPopup::create(this, callfunc_selector(MainFlowScene::mailPopupClose), bind(&MainFlowScene::heartRefresh, this));
			addChild(t_pp, kMainFlowZorder_popup);
			
			postbox_count_case->setVisible(false);
		}
		else if(tag == kMainFlowMenuTag_option)
		{
			OptionPopup* t_popup = OptionPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			addChild(t_popup, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_tip)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
			t_suction->target_touch_began = t_suction;
			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			t_suction->setTouchEnabled(true);
			
			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_main.png");
			dimed_tip->setPosition(ccp(240,160));
			t_suction->addChild(dimed_tip);
			
			addChild(t_suction, kMainFlowZorder_popup);
			
			is_menu_enable = true;
		}
		else if(tag == kMainFlowMenuTag_rank)
		{
			RankNewPopup* t_popup = RankNewPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			addChild(t_popup, kMainFlowZorder_popup);
			
//			RankPopup* t_rp = RankPopup::create(this, callfunc_selector(MainFlowScene::popupClose));
//			addChild(t_rp, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_shop)
		{
			ShopPopup* t_shop = ShopPopup::create();
			t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_shop->targetHeartTime(heart_time);
			t_shop->setShopCode(kSC_ruby);
			t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
			addChild(t_shop, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_cardSetting)
		{
			puzzle_table->setTouchEnabled(false);
			mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
			CardSettingPopup* t_popup = CardSettingPopup::create();
			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::tutorialCardSettingClose));
			addChild(t_popup, kMainFlowZorder_popup);
		}
		else if(tag == kMainFlowMenuTag_friendManagement)
		{
			is_menu_enable = true;

			InviteEventPopup* t_rp = InviteEventPopup::create(this, callfunc_selector(MainFlowScene::tutorialCardSettingClose));

			addChild(t_rp, kMainFlowZorder_popup);

		}
		else if(tag == kMainFlowMenuTag_gacha)
		{
			GachaPurchase* t_gp = GachaPurchase::create();
			addChild(t_gp, kMainFlowZorder_popup);
			
			t_gp->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
			t_gp->setOutAllObjectAction(NULL, NULL);
			t_gp->setInAllObjectAction(this, callfunc_selector(MainFlowScene::popupClose));
		}
		else if(tag == kMainFlowMenuTag_achievement)
		{
			AchievePopup* t_ap = AchievePopup::create();
			addChild(t_ap, kMainFlowZorder_popup);
			
			t_ap->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
		}
		else if(tag == kMainFlowMenuTag_event)
		{
			CCDirector::sharedDirector()->replaceScene(NewMainFlowScene::scene());
			
//			is_menu_enable = true; // 임시
//			EventPopup* t_popup = EventPopup::create();
//			t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
//			addChild(t_popup, kMainFlowZorder_popup);
		}
	}
}

void MainFlowScene::setBottom()
{
	CCNode* bottom_case = CCNode::create();
	bottom_case->setAnchorPoint(ccp(0.5f,0.5f));
	bottom_case->setPosition(ccp(240,-(myDSH->puzzle_ui_top-320.f)/2.f+3));
	addChild(bottom_case, kMainFlowZorder_uiButton);
	
	CCSprite* n_rank = CCSprite::create("mainflow_rank.png");
	CCSprite* s_rank = CCSprite::create("mainflow_rank.png");
	s_rank->setColor(ccGRAY);
	
	CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(MainFlowScene::menuAction));
	rank_item->setTag(kMainFlowMenuTag_rank);
	
	CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
	rank_menu->setPosition(ccp(-205, n_rank->getContentSize().height/2.f));
	bottom_case->addChild(rank_menu);
	
//	CCSprite* n_friendmanagement = CCSprite::create("mainflow_friendmanagement.png");
//	CCSprite* s_friendmanagement = CCSprite::create("mainflow_friendmanagement.png");
//	s_friendmanagement->setColor(ccGRAY);
//	
//	CCMenuItem* friendmanagement_item = CCMenuItemSprite::create(n_friendmanagement, s_friendmanagement, this, menu_selector(MainFlowScene::menuAction));
//	friendmanagement_item->setTag(kMainFlowMenuTag_friendManagement);
//	
//	CCMenu* friendmanagement_menu = CCMenu::createWithItem(friendmanagement_item);
//	friendmanagement_menu->setPosition(ccp(-139, n_friendmanagement->getContentSize().height/2.f));
//	bottom_case->addChild(friendmanagement_menu);
	
	CCSprite* n_shop = CCSprite::create("mainflow_shop.png");
	CCSprite* s_shop = CCSprite::create("mainflow_shop.png");
	s_shop->setColor(ccGRAY);
	
	CCMenuItem* shop_item = CCMenuItemSprite::create(n_shop, s_shop, this, menu_selector(MainFlowScene::menuAction));
	shop_item->setTag(kMainFlowMenuTag_shop);
	
	CCMenu* shop_menu = CCMenu::createWithItem(shop_item);
	shop_menu->setPosition(ccp(-73, n_shop->getContentSize().height/2.f));
	bottom_case->addChild(shop_menu);
	
	CCSprite* n_cardsetting = CCSprite::create("mainflow_cardsetting.png");
	CCSprite* s_cardsetting = CCSprite::create("mainflow_cardsetting.png");
	s_cardsetting->setColor(ccGRAY);
	
	CCMenuItem* cardsetting_item = CCMenuItemSprite::create(n_cardsetting, s_cardsetting, this, menu_selector(MainFlowScene::menuAction));
	cardsetting_item->setTag(kMainFlowMenuTag_cardSetting);
	
	CCMenu* cardsetting_menu = CCMenu::createWithItem(cardsetting_item);
	cardsetting_menu->setPosition(ccp(-7, n_cardsetting->getContentSize().height/2.f));
	bottom_case->addChild(cardsetting_menu);
	
//	CCSprite* n_gacha = CCSprite::create("mainflow_gacha.png");
//	CCSprite* s_gacha = CCSprite::create("mainflow_gacha.png");
//	s_gacha->setColor(ccGRAY);
//	
//	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(MainFlowScene::menuAction));
//	gacha_item->setTag(kMainFlowMenuTag_gacha);
//	
//	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
//	gacha_menu->setPosition(ccp(59, n_gacha->getContentSize().height/2.f));
//	bottom_case->addChild(gacha_menu);
	
	CCSprite* n_achievement = CCSprite::create("mainflow_achievement.png");
	CCSprite* s_achievement = CCSprite::create("mainflow_achievement.png");
	s_achievement->setColor(ccGRAY);
	
	CCMenuItem* achievement_item = CCMenuItemSprite::create(n_achievement, s_achievement, this, menu_selector(MainFlowScene::menuAction));
	achievement_item->setTag(kMainFlowMenuTag_achievement);
	
	CCMenu* achievement_menu = CCMenu::createWithItem(achievement_item);
	achievement_menu->setPosition(ccp(-139, n_achievement->getContentSize().height/2.f));//ccp(125, n_achievement->getContentSize().height/2.f));
	bottom_case->addChild(achievement_menu);
	
	CCSprite* n_event = CCSprite::create("mainflow_event.png");
	CCSprite* s_event = CCSprite::create("mainflow_event.png");
	s_event->setColor(ccGRAY);
	
	CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(MainFlowScene::menuAction));
	event_item->setTag(kMainFlowMenuTag_event);
	
	CCMenu* event_menu = CCMenu::createWithItem(event_item);
	event_menu->setPosition(ccp(201, n_event->getContentSize().height/2.f-3));
	bottom_case->addChild(event_menu);
}

void MainFlowScene::heartRefresh()
{
	CCPoint heart_position = heart_time->getPosition();
	CCNode* heart_parent = heart_time->getParent();
	
	heart_time->removeFromParent();
	
	heart_time = HeartTime::create();
	heart_time->setPosition(heart_position);
	heart_parent->addChild(heart_time);
}

void MainFlowScene::setTop()
{
	CCSprite* top_case = CCSprite::create("mainflow_top.png");
	top_case->setAnchorPoint(ccp(0.f,1.f));
	top_case->setPosition(ccp(0,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f-3));
	addChild(top_case, kMainFlowZorder_top);
	
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
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(MainFlowScene::menuAction));
	heart_item->setTag(kMainFlowMenuTag_heartShop);
	
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
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(MainFlowScene::menuAction));
	gold_item->setTag(kMainFlowMenuTag_goldShop);
	
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
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(MainFlowScene::menuAction));
	ruby_item->setTag(kMainFlowMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(287,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_menu);
	
//	friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "mainflow_top_font1.fnt", 0.3f, "%d");
//	friend_point_label->setPosition(ccp(338,top_case->getContentSize().height/2.f-5));
//	top_case->addChild(friend_point_label);
//	
//	mySGD->setFriendPointLabel(friend_point_label);
	
	CCSprite* n_friend_point = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_friend_point = CCSprite::create("mainflow_top_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(MainFlowScene::menuAction));
	friend_point_item->setTag(kMainFlowMenuTag_friendPointContent);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(362,top_case->getContentSize().height/2.f));
	top_case->addChild(friend_point_menu);
	
	CCSprite* n_postbox = CCSprite::create("mainflow_postbox.png");
	CCSprite* s_postbox = CCSprite::create("mainflow_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(MainFlowScene::menuAction));
	postbox_item->setTag(kMainFlowMenuTag_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccp(395,top_case->getContentSize().height/2.f));
	top_case->addChild(postbox_menu);
	
	postbox_count_case = CCSprite::create("mainflow_postbox_count.png");
	postbox_count_case->setPosition(ccp(407,top_case->getContentSize().height/2.f+6));
	top_case->addChild(postbox_count_case);
	postbox_count_case->setVisible(false);
	
	postbox_count_label = CCLabelTTF::create("0", mySGD->getFont().c_str(), 10);
	postbox_count_label->setColor(ccc3(95, 60, 30));
	postbox_count_label->setPosition(ccp(postbox_count_case->getContentSize().width/2.f-0.5f, postbox_count_case->getContentSize().height/2.f+0.5f));
	postbox_count_case->addChild(postbox_count_label);
	
	countingMessage();
	
	CCSprite* n_option = CCSprite::create("mainflow_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(MainFlowScene::menuAction));
	option_item->setTag(kMainFlowMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(427,top_case->getContentSize().height/2.f));
	top_case->addChild(option_menu);
	
	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(MainFlowScene::menuAction));
	tip_item->setTag(kMainFlowMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(465,top_case->getContentSize().height/2.f));
	top_case->addChild(tip_menu);
}

void MainFlowScene::countingMessage()
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

void MainFlowScene::popupClose()
{
	is_menu_enable = true;
}

void MainFlowScene::mailPopupClose()
{
	countingMessage();
	is_menu_enable = true;
}

void MainFlowScene::tutorialCardSettingClose()
{
	is_menu_enable = true;
	puzzle_table->setTouchEnabled(true);
	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
}

void MainFlowScene::closeFriendPoint()
{
	close_friend_point_action();
}

MainFlowScene::~MainFlowScene()
{
	hspConnector::get()->removeTarget(this);
}

void MainFlowScene::alertAction(int t1, int t2)
{
	if(t1 == 1 && t2 == 0)
	{
		CCDirector::sharedDirector()->end();
	}
}

void MainFlowScene::keyBackClicked()
{
	AlertEngine::sharedInstance()->addDoubleAlert("Exit", MyLocal::sharedInstance()->getLocalForKey(kMyLocalKey_exit), "Ok", "Cancel", 1, this, alertfuncII_selector(MainFlowScene::alertAction));
}
