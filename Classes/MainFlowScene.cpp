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
#include "TicketRequestContent.h"
#include "PuzzleScene.h"
#include "StageListDown.h"

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

enum MainFlowZorder{
	kMainFlowZorder_back = 1,
	kMainFlowZorder_table,
	kMainFlowZorder_top,
	kMainFlowZorder_uiButton,
	kMainFlowZorder_popup
};

bool MainFlowScene::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	friend_point_popup = NULL;
	
	CCSprite* back_img = CCSprite::create("mainflow_back_wall.png");
	back_img->setPosition(ccp(240,160));
	addChild(back_img, kMainFlowZorder_back);
	
	setTable();
	
	setTop();
	setBottom();
	
	is_menu_enable = true;
	
	return true;
}

void MainFlowScene::setTable()
{
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	CCSize table_size = CCSizeMake(480*screen_scale_x, 180);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(ccp(myDSH->ui_zero_point.x, 160-table_size.height/2.f));
//	addChild(temp_back, kMainFlowZorder_table);
	
	puzzle_table = CCTableView::create(this, table_size);
	puzzle_table->setAnchorPoint(CCPointZero);
	puzzle_table->setDirection(kCCScrollViewDirectionHorizontal);
	puzzle_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	puzzle_table->setPosition(ccp(myDSH->ui_zero_point.x, 160-table_size.height/2.f));
	puzzle_table->setDelegate(this);
	addChild(puzzle_table, kMainFlowZorder_table);
	puzzle_table->setTouchPriority(kCCMenuHandlerPriority+1);
	
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
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
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag < kMainFlowTableCellTag_buyBase)
	{
		int puzzle_number = tag - kMainFlowTableCellTag_openBase;
		myDSH->setIntegerForKey(kDSH_Key_selectedPuzzleNumber, puzzle_number);
		
		StageListDown* t_sld = StageListDown::create(this, callfunc_selector(MainFlowScene::puzzleLoadSuccess), puzzle_number);
		addChild(t_sld, kMainFlowZorder_popup);
	}
	else if(tag < kMainFlowTableCellTag_ticketBase) // buyBase
	{
		int puzzle_number = tag - kMainFlowTableCellTag_buyBase;
		CCLog("puzzle_number : %d", puzzle_number);
		
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
		
		CCLabelTTF* title_label = CCLabelTTF::create("지금 열기", mySGD->getFont().c_str(), 20);
		title_label->setPosition(ccp(0, 102));
		t_container->addChild(title_label);
		
		CCLabelTTF* content_label = CCLabelTTF::create(CCString::createWithFormat("%d Ruby 로 오픈", NSDS_GI(puzzle_number, kSDS_PZ_point_i))->getCString(), mySGD->getFont().c_str(), 18);
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
		
		if(mySGD->getStar() >= NSDS_GI(puzzle_number, kSDS_PZ_point_i))
		{
			CCSprite* n_buy = CCSprite::create("popup2_buy.png");
			CCSprite* s_buy = CCSprite::create("popup2_buy.png");
			s_buy->setColor(ccGRAY);
			
			CCMenuItemSpriteLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender){
				mySGD->setStar(mySGD->getStar() - NSDS_GI(puzzle_number, kSDS_PZ_point_i));
				myDSH->setIntegerForKey(kDSH_Key_openPuzzleCnt, myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_openPuzzle);
				
				myDSH->saveUserData(save_userdata_list, nullptr);
				
				CCTableViewCell* cell_node = (CCTableViewCell*)(((CCNode*)sender)->getParent()->getParent());
				puzzle_table->updateCellAtIndex(cell_node->getIdx());
//				puzzle_table->updateCellAtIndex(puzzle_number-1);
				
				is_menu_enable = true;
				t_popup->removeFromParent();
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
	else // ticketBase
	{
		int puzzle_number = tag - kMainFlowTableCellTag_ticketBase;
		CCLog("puzzle_number : %d", puzzle_number);
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
		
		TicketRequestContent* t_container = TicketRequestContent::create(t_popup->getTouchPriority(), puzzle_number);
		t_popup->setContainerNode(t_container);
		addChild(t_popup);
		
		t_container->setRemoveAction([=](){
			is_menu_enable = true;
			t_popup->removeFromParent();
		});
	}
}
CCTableViewCell* MainFlowScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, idx+1);
	cell->setTag(puzzle_number);
	if(puzzle_number == 1 || myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt) >= puzzle_number)
	{
		CCSprite* n_open_back = CCSprite::create("mainflow_puzzle_open_back.png");
		CCSprite* s_open_back = CCSprite::create("mainflow_puzzle_open_back.png");
		s_open_back->setColor(ccGRAY);
		
		CCMenuItem* open_item = CCMenuItemSprite::create(n_open_back, s_open_back, this, menu_selector(MainFlowScene::cellAction));
		open_item->setTag(kMainFlowTableCellTag_openBase + puzzle_number);
		
		ScrollMenu* open_menu = ScrollMenu::create(open_item, NULL);
		open_menu->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f));
		cell->addChild(open_menu);
		
		CCLabelTTF* title_label = CCLabelTTF::create(NSDS_GS(puzzle_number, kSDS_PZ_title_s).c_str(), mySGD->getFont().c_str(), 15);
		title_label->setColor(ccBLACK);
		title_label->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+67));
		cell->addChild(title_label);
		
		CCSprite* puzzle_thumbnail = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", puzzle_number)->getCString());
		puzzle_thumbnail->setPosition(ccp(cellSizeForTable(table).width/2.f-1, cellSizeForTable(table).height/2.f+13));
		cell->addChild(puzzle_thumbnail);
		
		int have_card_cnt = 0;
		int start_stage = NSDS_GI(puzzle_number, kSDS_PZ_startStage_i);
		int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
		int total_card_cnt = stage_count*3;
		
		int card_take_cnt = myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt);
		for(int i=1;i<=card_take_cnt;i++)
		{
			int card_number = myDSH->getIntegerForKey(kDSH_Key_takeCardNumber_int1, i);
			int card_stage_number = NSDS_GI(kSDS_CI_int1_stage_i, card_number);
			if(card_stage_number >= start_stage && card_stage_number < start_stage+stage_count)
				have_card_cnt++;
		}
		
		CCLabelTTF* rate_label = CCLabelTTF::create(CCString::createWithFormat("%d / %d", have_card_cnt, total_card_cnt)->getCString(), mySGD->getFont().c_str(), 15);
		rate_label->setAnchorPoint(ccp(0,0.5));
		rate_label->setHorizontalAlignment(kCCTextAlignmentLeft);
		rate_label->setPosition(ccp(cellSizeForTable(table).width/2.f-10, cellSizeForTable(table).height/2.f-47));
		cell->addChild(rate_label);
		
		CCProgressTimer* rate_timer = CCProgressTimer::create(CCSprite::create("mainflow_puzzle_bar.png"));
		rate_timer->setType(kCCProgressTimerTypeBar);
		rate_timer->setMidpoint(ccp(0,0));
		rate_timer->setBarChangeRate(ccp(1,0));
		rate_timer->setPercentage(100.f*have_card_cnt/total_card_cnt);
		rate_timer->setPosition(ccp(cellSizeForTable(table).width/2.f-1, cellSizeForTable(table).height/2.f-66));
		cell->addChild(rate_timer);
	}
	else
	{
		CCSprite* close_back = CCSprite::create("mainflow_puzzle_lock_back.png");
		close_back->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f));
		cell->addChild(close_back);
		
		CCLabelTTF* title_label = CCLabelTTF::create(NSDS_GS(puzzle_number, kSDS_PZ_title_s).c_str(), mySGD->getFont().c_str(), 15);
		title_label->setColor(ccBLACK);
		title_label->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+67));
		cell->addChild(title_label);
		
		if(myDSH->getBoolForKey(kDSH_Key_isClearedPuzzle_int1, puzzle_number))
		{
			CCSprite* need_ticket_img = CCSprite::create("mainflow_puzzle_needticket.png");
			need_ticket_img->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f+15));
			cell->addChild(need_ticket_img);
			
			CCLabelTTF* ticket_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt),
																					 NSDS_GI(puzzle_number, kSDS_PZ_ticket_i))->getCString(), mySGD->getFont().c_str(), 13);
			ticket_label->setPosition(ccp(cellSizeForTable(table).width/2.f+15, cellSizeForTable(table).height/2.f-4));
			cell->addChild(ticket_label);
			
			CCSprite* n_buy = CCSprite::create("mainflow_puzzle_open_buy.png");
			CCSprite* s_buy = CCSprite::create("mainflow_puzzle_open_buy.png");
			s_buy->setColor(ccGRAY);
			
			CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, this, menu_selector(MainFlowScene::cellAction));
			buy_item->setTag(kMainFlowTableCellTag_buyBase + puzzle_number);
			
			ScrollMenu* buy_menu = ScrollMenu::create(buy_item, NULL);
			buy_menu->setPosition(ccp(cellSizeForTable(table).width/2.f-28, cellSizeForTable(table).height/2.f-53));
			cell->addChild(buy_menu);
			
			CCSprite* n_ticket = CCSprite::create("mainflow_puzzle_open_ticket.png");
			CCSprite* s_ticket = CCSprite::create("mainflow_puzzle_open_ticket.png");
			s_ticket->setColor(ccGRAY);
			
			CCMenuItem* ticket_item = CCMenuItemSprite::create(n_ticket, s_ticket, this, menu_selector(MainFlowScene::cellAction));
			ticket_item->setTag(kMainFlowTableCellTag_ticketBase + puzzle_number);
			
			ScrollMenu* ticket_menu = ScrollMenu::create(ticket_item, NULL);
			ticket_menu->setPosition(ccp(cellSizeForTable(table).width/2.f+28, cellSizeForTable(table).height/2.f-53));
			cell->addChild(ticket_menu);
		}
		else
		{
			CCSprite* not_clear_img = CCSprite::create("mainflow_puzzle_lock_base.png");
			not_clear_img->setPosition(ccp(cellSizeForTable(table).width/2.f, cellSizeForTable(table).height/2.f-18));
			cell->addChild(not_clear_img);
		}
	}
	
	return cell;
}
void MainFlowScene::scrollViewDidScroll(CCScrollView* view){}
void MainFlowScene::scrollViewDidZoom(CCScrollView* view){}
void MainFlowScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell){}
CCSize MainFlowScene::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(141, 180);
}
unsigned int MainFlowScene::numberOfCellsInTableView(CCTableView *table)
{
	return NSDS_GI(kSDS_GI_puzzleListCount_i);
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
	kMainFlowMenuTag_friendPointClose,
	kMainFlowMenuTag_postbox,
	kMainFlowMenuTag_option,
	kMainFlowMenuTag_rank,
	kMainFlowMenuTag_shop,
	kMainFlowMenuTag_cardSetting,
	kMainFlowMenuTag_puzzleSetting,
	kMainFlowMenuTag_gacha,
	kMainFlowMenuTag_event
};

void MainFlowScene::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
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
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(MainFlowScene::menuAction));
			close_item->setTag(kMainFlowMenuTag_friendPointClose);
			
			CCMenu* close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(ccp(friend_point_popup->getContentSize().width/2.f, 25));
			friend_point_popup->addChild(close_menu);
			
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(410,menu_node->getPositionY()-12));
			CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainFlowScene::popupClose));
			CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
			friend_point_popup->runAction(t_seq);
		}
		else
			is_menu_enable = true;
	}
	else if(tag == kMainFlowMenuTag_friendPointClose)
	{
		CCNode* menu_node = ((CCNode*)sender)->getParent();
		CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(410,menu_node->getPositionY() + friend_point_popup->getContentSize().height));
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(MainFlowScene::closeFriendPointPopup));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_move, t_call);
		friend_point_popup->runAction(t_seq);
	}
	else if(tag == kMainFlowMenuTag_postbox)
	{
		MailPopup* t_pp = MailPopup::create(this, callfunc_selector(MainFlowScene::popupClose));
		addChild(t_pp, kMainFlowZorder_popup);
	}
	else if(tag == kMainFlowMenuTag_option)
	{
		OptionPopup* t_popup = OptionPopup::create();
		t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
		addChild(t_popup, kMainFlowZorder_popup);
	}
	else if(tag == kMainFlowMenuTag_rank)
	{
		RankPopup* t_rp = RankPopup::create(this, callfunc_selector(MainFlowScene::popupClose));
		addChild(t_rp, kMainFlowZorder_popup);
	}
	else if(tag == kMainFlowMenuTag_shop)
	{
		ShopPopup* t_shop = ShopPopup::create();
		t_shop->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
		t_shop->targetHeartTime(heart_time);
		t_shop->setShopCode(kSC_character);
		t_shop->setShopBeforeCode(kShopBeforeCode_mainflow);
		addChild(t_shop, kMainFlowZorder_popup);
	}
	else if(tag == kMainFlowMenuTag_cardSetting)
	{
		mySGD->before_cardsetting = kSceneCode_PuzzleMapScene;
		CardSettingPopup* t_popup = CardSettingPopup::create();
		t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
		addChild(t_popup, kMainFlowZorder_popup);
	}
	else if(tag == kMainFlowMenuTag_puzzleSetting)
	{
		is_menu_enable = true;
	}
	else if(tag == kMainFlowMenuTag_gacha)
	{
		GachaPurchase* t_gp = GachaPurchase::create();
		addChild(t_gp, kMainFlowZorder_popup);
		
		t_gp->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
		t_gp->setOutAllObjectAction(NULL, NULL);
		t_gp->setInAllObjectAction(NULL, NULL);
	}
	else if(tag == kMainFlowMenuTag_event)
	{
		EventPopup* t_popup = EventPopup::create();
		t_popup->setHideFinalAction(this, callfunc_selector(MainFlowScene::popupClose));
		addChild(t_popup, kMainFlowZorder_popup);
	}
}

void MainFlowScene::setBottom()
{
	CCNode* bottom_case = CCNode::create();
	bottom_case->setAnchorPoint(ccp(0.5f,0.5f));
	bottom_case->setPosition(ccp(240,-(myDSH->puzzle_ui_top-320.f)/2.f));
	addChild(bottom_case, kMainFlowZorder_uiButton);
	
	CCSprite* n_rank = CCSprite::create("mainflow_rank.png");
	CCSprite* s_rank = CCSprite::create("mainflow_rank.png");
	s_rank->setColor(ccGRAY);
	
	CCMenuItem* rank_item = CCMenuItemSprite::create(n_rank, s_rank, this, menu_selector(MainFlowScene::menuAction));
	rank_item->setTag(kMainFlowMenuTag_rank);
	
	CCMenu* rank_menu = CCMenu::createWithItem(rank_item);
	rank_menu->setPosition(ccp(-200, n_rank->getContentSize().height/2.f));
	bottom_case->addChild(rank_menu);
	
	CCSprite* n_shop = CCSprite::create("mainflow_shop.png");
	CCSprite* s_shop = CCSprite::create("mainflow_shop.png");
	s_shop->setColor(ccGRAY);
	
	CCMenuItem* shop_item = CCMenuItemSprite::create(n_shop, s_shop, this, menu_selector(MainFlowScene::menuAction));
	shop_item->setTag(kMainFlowMenuTag_shop);
	
	CCMenu* shop_menu = CCMenu::createWithItem(shop_item);
	shop_menu->setPosition(ccp(-120, n_shop->getContentSize().height/2.f));
	bottom_case->addChild(shop_menu);
	
	CCSprite* n_cardsetting = CCSprite::create("mainflow_cardsetting.png");
	CCSprite* s_cardsetting = CCSprite::create("mainflow_cardsetting.png");
	s_cardsetting->setColor(ccGRAY);
	
	CCMenuItem* cardsetting_item = CCMenuItemSprite::create(n_cardsetting, s_cardsetting, this, menu_selector(MainFlowScene::menuAction));
	cardsetting_item->setTag(kMainFlowMenuTag_cardSetting);
	
	CCMenu* cardsetting_menu = CCMenu::createWithItem(cardsetting_item);
	cardsetting_menu->setPosition(ccp(-40, n_cardsetting->getContentSize().height/2.f));
	bottom_case->addChild(cardsetting_menu);
	
	CCSprite* n_puzzlesetting = CCSprite::create("mainflow_puzzlesetting.png");
	CCSprite* s_puzzlesetting = CCSprite::create("mainflow_puzzlesetting.png");
	s_puzzlesetting->setColor(ccGRAY);
	
	CCMenuItem* puzzlesetting_item = CCMenuItemSprite::create(n_puzzlesetting, s_puzzlesetting, this, menu_selector(MainFlowScene::menuAction));
	puzzlesetting_item->setTag(kMainFlowMenuTag_puzzleSetting);
	
	CCMenu* puzzlesetting_menu = CCMenu::createWithItem(puzzlesetting_item);
	puzzlesetting_menu->setPosition(ccp(40, n_puzzlesetting->getContentSize().height/2.f));
	bottom_case->addChild(puzzlesetting_menu);
	
	CCSprite* n_gacha = CCSprite::create("mainflow_gacha.png");
	CCSprite* s_gacha = CCSprite::create("mainflow_gacha.png");
	s_gacha->setColor(ccGRAY);
	
	CCMenuItem* gacha_item = CCMenuItemSprite::create(n_gacha, s_gacha, this, menu_selector(MainFlowScene::menuAction));
	gacha_item->setTag(kMainFlowMenuTag_gacha);
	
	CCMenu* gacha_menu = CCMenu::createWithItem(gacha_item);
	gacha_menu->setPosition(ccp(120, n_gacha->getContentSize().height/2.f));
	bottom_case->addChild(gacha_menu);
	
	CCSprite* n_event = CCSprite::create("mainflow_event.png");
	CCSprite* s_event = CCSprite::create("mainflow_event.png");
	s_event->setColor(ccGRAY);
	
	CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(MainFlowScene::menuAction));
	event_item->setTag(kMainFlowMenuTag_event);
	
	CCMenu* event_menu = CCMenu::createWithItem(event_item);
	event_menu->setPosition(ccp(200, n_event->getContentSize().height/2.f));
	bottom_case->addChild(event_menu);
}

void MainFlowScene::setTop()
{
	CCSprite* top_case = CCSprite::create("mainflow_top.png");
	top_case->setAnchorPoint(ccp(0.5f,1.f));
	top_case->setPosition(ccp(240,(myDSH->puzzle_ui_top-320.f)/2.f + 320.f));
	addChild(top_case, kMainFlowZorder_top);
	
	heart_time = HeartTime::create();
	heart_time->setPosition(ccp(65,top_case->getContentSize().height/2.f));
	top_case->addChild(heart_time);
	
	CCSprite* n_heart = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_heart = CCSprite::create("mainflow_top_shop.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(MainFlowScene::menuAction));
	heart_item->setTag(kMainFlowMenuTag_heartShop);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(ccp(178,top_case->getContentSize().height/2.f-2));
	top_case->addChild(heart_menu);
	
	gold_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getGold())->getCString(), "etc_font.fnt", 0.3f, "%d");
	gold_label->setPosition(ccp(237,top_case->getContentSize().height/2.f));
	top_case->addChild(gold_label);
	
	mySGD->setGoldLabel(gold_label);
	
	CCSprite* n_gold = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_gold = CCSprite::create("mainflow_top_shop.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(MainFlowScene::menuAction));
	gold_item->setTag(kMainFlowMenuTag_goldShop);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(ccp(271,top_case->getContentSize().height/2.f-2));
	top_case->addChild(gold_menu);
	
	ruby_label = CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getStar())->getCString(), "etc_font.fnt", 0.3f, "%d");
	ruby_label->setPosition(ccp(326,top_case->getContentSize().height/2.f));
	top_case->addChild(ruby_label);
	
	mySGD->setStarLabel(ruby_label);
	
	CCSprite* n_ruby = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_ruby = CCSprite::create("mainflow_top_shop.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(MainFlowScene::menuAction));
	ruby_item->setTag(kMainFlowMenuTag_rubyShop);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(ccp(356,top_case->getContentSize().height/2.f-2));
	top_case->addChild(ruby_menu);
	
	friend_point_label =  CountingBMLabel::create(CCString::createWithFormat("%d", mySGD->getFriendPoint())->getCString(), "etc_font.fnt", 0.3f, "%d");
	friend_point_label->setPosition(ccp(410,top_case->getContentSize().height/2.f));
	top_case->addChild(friend_point_label);
	
	mySGD->setFriendPointLabel(friend_point_label);
	
	CCSprite* n_friend_point = CCSprite::create("mainflow_top_shop.png");
	CCSprite* s_friend_point = CCSprite::create("mainflow_top_shop.png");
	s_friend_point->setColor(ccGRAY);
	
	CCMenuItem* friend_point_item = CCMenuItemSprite::create(n_friend_point, s_friend_point, this, menu_selector(MainFlowScene::menuAction));
	friend_point_item->setTag(kMainFlowMenuTag_friendPointContent);
	
	CCMenu* friend_point_menu = CCMenu::createWithItem(friend_point_item);
	friend_point_menu->setPosition(ccp(440,top_case->getContentSize().height/2.f-2));
	top_case->addChild(friend_point_menu);
	
	CCSprite* n_postbox = CCSprite::create("mainflow_postbox.png");
	CCSprite* s_postbox = CCSprite::create("mainflow_postbox.png");
	s_postbox->setColor(ccGRAY);
	
	CCMenuItem* postbox_item = CCMenuItemSprite::create(n_postbox, s_postbox, this, menu_selector(MainFlowScene::menuAction));
	postbox_item->setTag(kMainFlowMenuTag_postbox);
	
	CCMenu* postbox_menu = CCMenu::createWithItem(postbox_item);
	postbox_menu->setPosition(ccp(472,top_case->getContentSize().height/2.f-2));
	top_case->addChild(postbox_menu);
	
	CCSprite* n_option = CCSprite::create("mainflow_option.png");
	CCSprite* s_option = CCSprite::create("mainflow_option.png");
	s_option->setColor(ccGRAY);
	
	CCMenuItem* option_item = CCMenuItemSprite::create(n_option, s_option, this, menu_selector(MainFlowScene::menuAction));
	option_item->setTag(kMainFlowMenuTag_option);
	
	CCMenu* option_menu = CCMenu::createWithItem(option_item);
	option_menu->setPosition(ccp(505,top_case->getContentSize().height/2.f-2));
	top_case->addChild(option_menu);
}

void MainFlowScene::popupClose()
{
	is_menu_enable = true;
}

void MainFlowScene::closeFriendPointPopup()
{
	friend_point_popup->removeFromParent();
	friend_point_popup = NULL;
	is_menu_enable = true;
}