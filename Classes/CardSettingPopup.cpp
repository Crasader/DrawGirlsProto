//
//  CardSettingPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#include "CardSettingPopup.h"
#include "MyLocalization.h"
#include "StageImgLoader.h"
//#include "CollectionBook.h"
#include "CollectionBookPopup.h"
#include "PuzzleMapScene.h"
#include "MainFlowScene.h"
#include "CardCase.h"
#include "GameData.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "CardListViewer.h"
#include "StageSettingPopup.h"
#include "CardStrengthPopup.h"
#include "TutorialFlowStep.h"
#include "TouchSuctionLayer.h"

void CardSettingPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool CardSettingPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	server_puzzle_list_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	server_event_list_count = NSDS_GI(kSDS_GI_eventCount_i);
	
	server_puzzle_list_no.push_back(0);
	server_puzzle_stage_count.push_back(0);
	server_puzzle_start_stage.push_back(0);
	for(int i=1;i<=server_puzzle_list_count;i++)
	{
		server_puzzle_list_no.push_back(NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i));
		server_puzzle_stage_count.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_stageCount_i));
		server_puzzle_start_stage.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_startStage_i));
	}
	
	recent_sort_type = myDSH->getIntegerForKey(kDSH_Key_cardSortType);
	recent_selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	
	is_menu_enable = false;
	
	setTouchEnabled(true);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCSS_Z_gray);
	
	main_case = CCSprite::create("cardsetting_back.png");
	main_case->setAnchorPoint(ccp(0,0));
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kCSS_Z_back);
	
	int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
	
	int total_stage_cnt = 0;
	for(int i=1;i<=puzzle_count;i++)
	{
		int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
		int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
		
		total_stage_cnt += stage_count;
	}
	
	int event_stage_count = NSDS_GI(kSDS_GI_eventCount_i);
	total_stage_cnt += event_stage_count;
	
	default_align_number_of_cell = (total_stage_cnt-1)/2 + 1;
	
	CCSize table_size = CCSizeMake(392, 180);
	CCPoint table_position = ccp(34, 55);
	
//	CCSprite* temp_table = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_table->setAnchorPoint(CCPointZero);
//	temp_table->setOpacity(100);
//	temp_table->setPosition(table_position);
//	main_case->addChild(temp_table, kCSS_Z_content);
	
	card_table = CCTableView::create(this, table_size);
	card_table->setAnchorPoint(CCPointZero);
	card_table->setDirection(kCCScrollViewDirectionVertical);
	card_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	card_table->setPosition(table_position);
	card_table->setDelegate(this);
	main_case->addChild(card_table, kCSS_Z_content);
	card_table->setTouchPriority(-180-5);
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-184);
	t_suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	t_suction->setTouchEnabled(true);
	addChild(t_suction);
	
	
	CCSprite* n_close = CCSprite::create("cardchange_cancel.png");
	CCSprite* s_close = CCSprite::create("cardchange_cancel.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(CardSettingPopup::menuAction));
	close_item->setTag(kCSS_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kCSS_MT_close));
	main_case->addChild(close_menu, kCSS_Z_content);
	close_menu->setTouchPriority(-185);
	
	
	CCSprite* n_strength = CCSprite::create("cardsetting_strength.png");
	CCSprite* s_strength = CCSprite::create("cardsetting_strength.png");
	s_strength->setColor(ccGRAY);
	
	CCMenuItem* strength_item = CCMenuItemSprite::create(n_strength, s_strength, this, menu_selector(CardSettingPopup::menuAction));
	strength_item->setTag(kCSS_MT_strength);
	
	CCMenu* strength_menu = CCMenu::createWithItem(strength_item);
	strength_menu->setPosition(ccp(78,34));
	main_case->addChild(strength_menu, kCSS_Z_content);
	strength_menu->setTouchPriority(-185);
	
	
	CCSprite* n_align_default = CCSprite::create("cardstrength_align_default.png");
	CCSprite* s_align_default = CCSprite::create("cardstrength_align_default.png");
	s_align_default->setColor(ccGRAY);
	
	CCMenuItem* align_default_item = CCMenuItemSprite::create(n_align_default, s_align_default, this, menu_selector(CardSettingPopup::menuAction));
	align_default_item->setTag(kCSS_MT_alignDefault);
	
	CCMenu* align_default_menu = CCMenu::createWithItem(align_default_item);
	align_default_menu->setPosition(ccp(248,34));
	main_case->addChild(align_default_menu, kCSS_Z_alignButton);
	align_default_menu->setTouchPriority(-185);
	
	
	CCSprite* n_align_take = CCSprite::create("cardstrength_align_take.png");
	CCSprite* s_align_take = CCSprite::create("cardstrength_align_take.png");
	s_align_take->setColor(ccGRAY);
	
	CCMenuItem* align_take_item = CCMenuItemSprite::create(n_align_take, s_align_take, this, menu_selector(CardSettingPopup::menuAction));
	align_take_item->setTag(kCSS_MT_alignTake);
	
	CCMenu* align_take_menu = CCMenu::createWithItem(align_take_item);
	align_take_menu->setPosition(ccp(330,34));
	main_case->addChild(align_take_menu, kCSS_Z_alignButton);
	align_take_menu->setTouchPriority(-185);
	
	
	CCSprite* n_align_rank = CCSprite::create("cardstrength_align_rank.png");
	CCSprite* s_align_rank = CCSprite::create("cardstrength_align_rank.png");
	s_align_rank->setColor(ccGRAY);
	
	CCMenuItem* align_rank_item = CCMenuItemSprite::create(n_align_rank, s_align_rank, this, menu_selector(CardSettingPopup::menuAction));
	align_rank_item->setTag(kCSS_MT_alignRank);
	
	CCMenu* align_rank_menu = CCMenu::createWithItem(align_rank_item);
	align_rank_menu->setPosition(ccp(412,34));
	main_case->addChild(align_rank_menu, kCSS_Z_alignButton);
	align_rank_menu->setTouchPriority(-185);
	
	CCSprite* title_case = CCSprite::create("cardsetting_title.png");
	title_case->setPosition(ccp(240,259));
	main_case->addChild(title_case, kCSS_Z_content);
	
	CCLabelTTF* take_card_count = CCLabelTTF::create(CCString::createWithFormat("%d 장", mySGD->getHasGottenCardsSize())->getCString(), mySGD->getFont().c_str(), 10);
	take_card_count->setPosition(ccp(398,258));
	main_case->addChild(take_card_count, kCSS_Z_content);
	
    return true;
}

void CardSettingPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void CardSettingPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardSettingPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardSettingPopup::endShowPopup()
{
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeClick)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_upgradeClick);
		addChild(t_tutorial, kCSS_Z_popup);
		
		tutorial_node = t_tutorial;
		
		card_table->setTouchEnabled(false);
	}
	
	is_menu_enable = true;
}

void CardSettingPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardSettingPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardSettingPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

CCPoint CardSettingPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCSS_MT_close)			return_value = ccp(452,259);
	else if(t_tag == kCSS_MT_cardBase)	return_value = ccp(245,213);
	
	return return_value;
}

void CardSettingPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeClick)
	{
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCSS_MT_strength)
		{
			myDSH->setIntegerForKey(kDSH_Key_tutorial_flowStep, kTutorialFlowStep_upgradeScript);
			is_menu_enable = false;
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
	}
	else
	{
		is_menu_enable = false;
		int tag = ((CCNode*)pSender)->getTag();
		
		if(tag == kCSS_MT_close)
		{
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_cardsInfo);
			save_userdata_list.push_back(kSaveUserData_Key_selectedCard);
			
			myDSH->saveUserData(save_userdata_list, nullptr);
			
			
			if(mySGD->before_cardsetting == kSceneCode_PuzzleMapScene)
				hidePopup();
			else if(mySGD->before_cardsetting == kSceneCode_StageSetting)
			{
				StageSettingPopup* t_popup = StageSettingPopup::create();
				t_popup->setHideFinalAction(target_final, delegate_final);
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				
				target_final = NULL;
				hidePopup();
			}
		}
		else if(tag == kCSS_MT_alignDefault)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_default)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_default);
				recent_sort_type = kCST_default;
				mySGD->changeSortType(kCST_default);
				alignChange();
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_alignRank)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_gradeDown)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_gradeDown);
				recent_sort_type = kCST_gradeDown;
				mySGD->changeSortType(kCST_gradeDown);
				alignChange();
			}
			else
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_gradeUp);
				recent_sort_type = kCST_gradeUp;
				mySGD->changeSortType(kCST_gradeUp);
				alignChange();
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_alignTake)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_cardSortType) != kCST_take)
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_take);
				recent_sort_type = kCST_take;
				mySGD->changeSortType(kCST_take);
				alignChange();
			}
			else
			{
				myDSH->setIntegerForKey(kDSH_Key_cardSortType, kCST_takeReverse);
				recent_sort_type = kCST_takeReverse;
				mySGD->changeSortType(kCST_takeReverse);
				alignChange();
			}
			
			is_menu_enable = true;
		}
		else if(tag == kCSS_MT_strength)
		{
			mySGD->setCardStrengthBefore(kCardStrengthBefore_cardSetting);
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
		else if(tag >= kCSS_MT_cardMenuBase && tag < kCSS_MT_noCardBase)
		{
			int clicked_card_number = tag-kCSS_MT_cardMenuBase;
			
			if(clicked_card_number != 0)
			{
				mySGD->selected_collectionbook = clicked_card_number;
				
				CollectionBookPopup* t_popup = CollectionBookPopup::create();
				t_popup->setHideFinalAction(target_final, delegate_final);
				getParent()->addChild(t_popup, kMainFlowZorder_popup);
				
				target_final = NULL;
				hidePopup();
				
				//			CCDirector::sharedDirector()->replaceScene(CollectionBook::scene());
			}
			else
			{
				is_menu_enable = true;
			}
		}
		else if(tag >= kCSS_MT_noCardBase)
		{
			is_menu_enable = true;
		}
	}
}


void CardSettingPopup::alignChange()
{
	card_table->reloadData();
}

void CardSettingPopup::cellAction( CCObject* sender )
{
	
}

CCTableViewCell* CardSettingPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CardSortType sort_type = (CardSortType)recent_sort_type;
	
	if(sort_type == kCST_default)
	{
		int puzzle_count = server_puzzle_list_count;
		
		int t_idx = idx*2;
		int found_stage1 = -1;
		int selected_cnt = 0;
		for(int i=1;i<=puzzle_count && found_stage1 == -1;i++)
		{
			int stage_count = server_puzzle_stage_count[i];
			
			if(t_idx >= selected_cnt && t_idx < selected_cnt+stage_count)
			{
				int start_stage = server_puzzle_start_stage[i];
				found_stage1 = t_idx-selected_cnt+start_stage;
			}
			else
				selected_cnt += stage_count;
		}
		
		if(found_stage1 == -1)
		{
			int event_stage_count = server_event_list_count;
			if(t_idx >= selected_cnt && t_idx < selected_cnt+event_stage_count)
				found_stage1 = t_idx-selected_cnt+event_stage_count;
			else
				CCLog("not found stage");
		}
		
		if(found_stage1 != -1)
		{
			for(int i=1;i<=3;i++)
			{
				int card_number = getSGDisHasGottenCards(found_stage1, i);
				CCPoint card_position = ccp(32.f + (i-1)*(64.f+1.f), 43.f);
				if(card_number > 0)
				{
					CCSprite* n_no = CCSprite::create("cardsetting_noimg.png");
					CCPoint no_minus_half_size = ccp(-n_no->getContentSize().width/2.f, -n_no->getContentSize().height/2.f);
					GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png",
																												  found_stage1, i)->getCString()));
					n_card->setScale(0.85f);
					n_card->setPosition(ccp(n_no->getContentSize().width/2.f, n_no->getContentSize().height/2.f));
					n_card->setGray(getCardDurability(card_number) <= 0);
					n_no->addChild(n_card, -1);
					
					CCSprite* s_no = CCSprite::create("cardsetting_noimg.png");
					s_no->setColor(ccGRAY);
					
					GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png",
																												  found_stage1, i)->getCString()));
					s_card->setScale(0.85f);
					s_card->setPosition(ccp(s_no->getContentSize().width/2.f, s_no->getContentSize().height/2.f));
					if(getCardDurability(card_number) <= 0)
						s_card->setColor(ccc3(60, 60, 60));
					else
						s_card->setColor(ccGRAY);
					s_no->addChild(s_card, -1);
					
					CCMenuItem* t_card_item = CCMenuItemSprite::create(n_no, s_no, this, menu_selector(CardSettingPopup::menuAction));
					t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
					
					ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
					t_card_menu->setPosition(card_position);
					cell->addChild(t_card_menu);
					t_card_menu->setTouchPriority(-180-3);
					
					CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", getCardDurability(card_number),
																							 getCardMaxDurability(card_number))->getCString(),
																  mySGD->getFont().c_str(), 9);
					t_durability->setAnchorPoint(ccp(0.5f,0.5f));
					t_durability->setColor(ccBLACK);
					t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
					t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
					t_durability->setPosition(ccpAdd(card_position, ccp(13, no_minus_half_size.y+12)));
					cell->addChild(t_durability);
					
					CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
					mini_rank->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(12,12))));
					cell->addChild(mini_rank);
					
					CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", getCardRank(card_number))->getCString(), mySGD->getFont().c_str(), 8);
					t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
					mini_rank->addChild(t_rank);
					
					CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", getCardLevel(card_number))->getCString(), mySGD->getFont().c_str(), 8);
					t_card_level_label->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(17,69))));
					cell->addChild(t_card_level_label);
					
					if(recent_selected_card_number == card_number)
					{
						CCSprite* select_img = CCSprite::create("card_check.png");
						select_img->setPosition(card_position);
						cell->addChild(select_img);
					}
				}
				else
				{
					CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
					t_no->setPosition(card_position);
					cell->addChild(t_no);
					
					CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CardSettingPopup::menuAction));
					t_card_item->setTag(kCSS_MT_noCardBase+card_number);
					
					ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
					t_card_menu->setPosition(card_position);
					cell->addChild(t_card_menu);
					t_card_menu->setTouchPriority(-180-3);
				}
			}
		}
		
		{ // next stage
			int t_idx = idx*2+1;
			int found_stage1 = -1;
			int selected_cnt = 0;
			for(int i=1;i<=puzzle_count && found_stage1 == -1;i++)
			{
				int stage_count = server_puzzle_stage_count[i];
				
				if(t_idx >= selected_cnt && t_idx < selected_cnt+stage_count)
				{
					int start_stage = server_puzzle_start_stage[i];
					found_stage1 = t_idx-selected_cnt+start_stage;
				}
				else
					selected_cnt += stage_count;
			}
			
			if(found_stage1 == -1)
			{
				int event_stage_count = server_event_list_count;
				if(t_idx >= selected_cnt && t_idx < selected_cnt+event_stage_count)
					found_stage1 = t_idx-selected_cnt+event_stage_count;
				else
					CCLog("not found stage");
			}
			
			if(found_stage1 != -1)
			{
				for(int i=1;i<=3;i++)
				{
					int card_number = getSGDisHasGottenCards(found_stage1, i);
					CCPoint card_position = ccp(32.f + 3*(64.f+1.f) + (i-1)*(64.f+1.f), 43.f);
					if(card_number > 0)
					{
						CCSprite* n_no = CCSprite::create("cardsetting_noimg.png");
						CCPoint no_minus_half_size = ccp(-n_no->getContentSize().width/2.f, -n_no->getContentSize().height/2.f);
						GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png",
																													  found_stage1, i)->getCString()));
						n_card->setScale(0.85f);
						n_card->setPosition(ccp(n_no->getContentSize().width/2.f, n_no->getContentSize().height/2.f));
						n_card->setGray(getCardDurability(card_number) <= 0);
						n_no->addChild(n_card, -1);
						
						CCSprite* s_no = CCSprite::create("cardsetting_noimg.png");
						s_no->setColor(ccGRAY);
						
						GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png",
																													  found_stage1, i)->getCString()));
						s_card->setScale(0.85f);
						s_card->setPosition(ccp(s_no->getContentSize().width/2.f, s_no->getContentSize().height/2.f));
						if(getCardDurability(card_number) <= 0)
							s_card->setColor(ccc3(60, 60, 60));
						else
							s_card->setColor(ccGRAY);
						s_no->addChild(s_card, -1);
						
						CCMenuItem* t_card_item = CCMenuItemSprite::create(n_no, s_no, this, menu_selector(CardSettingPopup::menuAction));
						t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
						
						ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
						t_card_menu->setPosition(card_position);
						cell->addChild(t_card_menu);
						t_card_menu->setTouchPriority(-180-3);
						
						CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", getCardDurability(card_number),
																								 getCardMaxDurability(card_number))->getCString(),
																	  mySGD->getFont().c_str(), 9);
						t_durability->setAnchorPoint(ccp(0.5f,0.5f));
						t_durability->setColor(ccBLACK);
						t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
						t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
						t_durability->setPosition(ccpAdd(card_position, ccp(13, no_minus_half_size.y+12)));
						cell->addChild(t_durability);
						
						CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
						mini_rank->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(12,12))));
						cell->addChild(mini_rank);
						
						CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", getCardRank(card_number))->getCString(), mySGD->getFont().c_str(), 8);
						t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
						mini_rank->addChild(t_rank);
						
						CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", getCardLevel(card_number))->getCString(), mySGD->getFont().c_str(), 8);
						t_card_level_label->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(17,69))));
						cell->addChild(t_card_level_label);
						
						if(recent_selected_card_number == card_number)
						{
							CCSprite* select_img = CCSprite::create("card_check.png");
							select_img->setPosition(card_position);
							cell->addChild(select_img);
						}
					}
					else
					{
						CCSprite* t_no = CCSprite::create("cardsetting_noimg.png");
						t_no->setPosition(card_position);
						cell->addChild(t_no);
						
						CCMenuItem* t_card_item = CCMenuItemImage::create("cardsetting_cardmenu.png", "cardsetting_cardmenu.png", this, menu_selector(CardSettingPopup::menuAction));
						t_card_item->setTag(kCSS_MT_noCardBase+card_number);
						
						ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
						t_card_menu->setPosition(card_position);
						cell->addChild(t_card_menu);
						t_card_menu->setTouchPriority(-180-3);
					}
				}
			}
		}
	}
	else
	{
		for(int i=idx*6;i<idx*6+6 && i<mySGD->getHasGottenCardsSize();i++)
		{
			int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
			int card_stage = getCardStage(card_number);
			int card_grade = getCardGrade(card_number);
			int card_durability = getCardDurability(card_number);
			CCPoint card_position = ccp(32.f + (i-idx*6)*(64.f+1.f), 43.f);
			
			CCSprite* n_no = CCSprite::create("cardsetting_noimg.png");
			CCPoint no_minus_half_size = ccp(-n_no->getContentSize().width/2.f, -n_no->getContentSize().height/2.f);
			GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png",
																										  card_stage, card_grade)->getCString()));
			n_card->setScale(0.85f);
			n_card->setPosition(ccp(n_no->getContentSize().width/2.f, n_no->getContentSize().height/2.f));
			n_card->setGray(card_durability <= 0);
			n_no->addChild(n_card, -1);
			
			CCSprite* s_no = CCSprite::create("cardsetting_noimg.png");
			s_no->setColor(ccGRAY);
			
			GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png",
																										  card_stage, card_grade)->getCString()));
			s_card->setScale(0.85f);
			s_card->setPosition(ccp(s_no->getContentSize().width/2.f, s_no->getContentSize().height/2.f));
			if(card_durability <= 0)
				s_card->setColor(ccc3(60, 60, 60));
			else
				s_card->setColor(ccGRAY);
			s_no->addChild(s_card, -1);
			
			CCMenuItem* t_card_item = CCMenuItemSprite::create(n_no, s_no, this, menu_selector(CardSettingPopup::menuAction));
			t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
			
			ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
			t_card_menu->setPosition(card_position);
			cell->addChild(t_card_menu);
			t_card_menu->setTouchPriority(-180-3);
			
			CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d/%d", card_durability,
																					 getCardMaxDurability(card_number))->getCString(),
														  mySGD->getFont().c_str(), 9);
			t_durability->setAnchorPoint(ccp(0.5f,0.5f));
			t_durability->setColor(ccBLACK);
			t_durability->setHorizontalAlignment(kCCTextAlignmentLeft);
			t_durability->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
			t_durability->setPosition(ccpAdd(card_position, ccp(13, no_minus_half_size.y+12)));
			cell->addChild(t_durability);
			
			CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
			mini_rank->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(12,12))));
			cell->addChild(mini_rank);
			
			CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", getCardRank(card_number))->getCString(), mySGD->getFont().c_str(), 8);
			t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
			mini_rank->addChild(t_rank);
			
			CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", getCardLevel(card_number))->getCString(), mySGD->getFont().c_str(), 8);
			t_card_level_label->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(17,69))));
			cell->addChild(t_card_level_label);
			

			if(recent_selected_card_number == card_number)
			{
				CCSprite* select_img = CCSprite::create("card_check.png");
				select_img->setPosition(card_position);
				cell->addChild(select_img);
			}
		}
	}
	
	return cell;
}

void CardSettingPopup::scrollViewDidScroll( CCScrollView* view )
{
	
}

void CardSettingPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void CardSettingPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	
}

CCSize CardSettingPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(392, 84);
}

unsigned int CardSettingPopup::numberOfCellsInTableView( CCTableView *table )
{
	CardSortType sort_type = CardSortType(recent_sort_type);
	
	if(sort_type == kCST_default)
	{
		return default_align_number_of_cell;
	}
	else
	{
		return (mySGD->getHasGottenCardsSize()-1)/6+1;
	}
}

bool CardSettingPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLog("touch swallow!!");
	
	return true;
}
void CardSettingPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardSettingPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}

int CardSettingPopup::getCardDurability(int t_card_number)
{
	map<int, int>::iterator iter = save_DSH_cardDurability_int1.find(t_card_number);
	if(iter == save_DSH_cardDurability_int1.end())
	{
		save_DSH_cardDurability_int1[t_card_number] = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, t_card_number);
		return save_DSH_cardDurability_int1[t_card_number];
	}
	else
		return iter->second;
}

int CardSettingPopup::getCardMaxDurability(int t_card_number)
{
	map<int, int>::iterator iter = save_DSH_cardMaxDurability_int1.find(t_card_number);
	if(iter == save_DSH_cardMaxDurability_int1.end())
	{
		save_DSH_cardMaxDurability_int1[t_card_number] = myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, t_card_number);
		return save_DSH_cardMaxDurability_int1[t_card_number];
	}
	else
		return iter->second;
}

int CardSettingPopup::getCardRank(int t_card_number)
{
	map<int, int>::iterator iter = save_SDS_CI_rank_int1.find(t_card_number);
	if(iter == save_SDS_CI_rank_int1.end())
	{
		save_SDS_CI_rank_int1[t_card_number] = NSDS_GI(kSDS_CI_int1_rank_i, t_card_number);
		return save_SDS_CI_rank_int1[t_card_number];
	}
	else
		return iter->second;
}

int CardSettingPopup::getCardStage(int t_card_number)
{
	map<int, int>::iterator iter = save_SDS_CI_stage_int1.find(t_card_number);
	if(iter == save_SDS_CI_stage_int1.end())
	{
		save_SDS_CI_stage_int1[t_card_number] = NSDS_GI(kSDS_CI_int1_stage_i, t_card_number);
		return save_SDS_CI_stage_int1[t_card_number];
	}
	else
		return iter->second;
}

int CardSettingPopup::getCardGrade(int t_card_number)
{
	map<int, int>::iterator iter = save_SDS_CI_grade_int1.find(t_card_number);
	if(iter == save_SDS_CI_grade_int1.end())
	{
		save_SDS_CI_grade_int1[t_card_number] = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
		return save_SDS_CI_grade_int1[t_card_number];
	}
	else
		return iter->second;
}

int CardSettingPopup::getCardLevel(int t_card_number)
{
	map<int, int>::iterator iter = save_DSH_cardLevel_int1.find(t_card_number);
	if(iter == save_DSH_cardLevel_int1.end())
	{
		save_DSH_cardLevel_int1[t_card_number] = myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, t_card_number);
		return save_DSH_cardLevel_int1[t_card_number];
	}
	else
		return iter->second;
}

int CardSettingPopup::getSGDisHasGottenCards(int t_stage, int t_grade)
{
	string t_key = CCString::createWithFormat("%d %d", t_stage, t_grade)->getCString();
	map<string, int>::iterator iter = save_SGD_isHasGottenCards.find(t_key);
	if(iter == save_SGD_isHasGottenCards.end())
	{
		save_SGD_isHasGottenCards[t_key] = mySGD->isHasGottenCards(t_stage, t_grade);
		return save_SGD_isHasGottenCards[t_key];
	}
	else
		return iter->second;
}