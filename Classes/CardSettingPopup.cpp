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
#include "CommonButton.h"
#include "KSLabelTTF.h"

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
	
	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000)
	{
		server_puzzle_list_count = NSDS_GI(kSDS_GI_eventListCount_i);
		
		server_puzzle_list_no.push_back(0);
		server_puzzle_stage_count.push_back(0);
		server_puzzle_start_stage.push_back(0);
		for(int i=1;i<=server_puzzle_list_count;i++)
		{
			server_puzzle_list_no.push_back(NSDS_GI(kSDS_GI_eventList_int1_no_i, i));
			server_puzzle_stage_count.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_stageCount_i));
			server_puzzle_start_stage.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_startStage_i));
		}
	}
	else
	{
		server_puzzle_list_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
		
		server_puzzle_list_no.push_back(0);
		server_puzzle_stage_count.push_back(0);
		server_puzzle_start_stage.push_back(0);
		for(int i=1;i<=server_puzzle_list_count;i++)
		{
			server_puzzle_list_no.push_back(NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i));
			server_puzzle_stage_count.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_stageCount_i));
			server_puzzle_start_stage.push_back(NSDS_GI(server_puzzle_list_no[i], kSDS_PZ_startStage_i));
		}
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
	
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-450));
	addChild(main_case, kCSS_Z_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create("컬렉션", mySGD->getFont().c_str(), 17);
	title_label->setPosition(ccp(40,256));
	main_case->addChild(title_label);
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_inner->setContentSize(CCSizeMake(460, 232));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.44f);
	main_case->addChild(main_inner);
	
	
	int puzzle_count = server_puzzle_list_count;
	int total_stage_cnt = 0;
	
	if(myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber) > 10000)
	{
		for(int i=1;i<=puzzle_count;i++)
		{
			int puzzle_number = NSDS_GI(kSDS_GI_eventList_int1_no_i, i);
			int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
			
			total_stage_cnt += stage_count;
		}
	}
	else
	{
		for(int i=1;i<=puzzle_count;i++)
		{
			int puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
			int stage_count = NSDS_GI(puzzle_number, kSDS_PZ_stageCount_i);
			
			total_stage_cnt += stage_count;
		}
	}
	
	default_align_number_of_cell = total_stage_cnt;
	
	CCSize table_size = CCSizeMake(410, 208);
	CCPoint table_position = ccp(36, 20);
	
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
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(-185);
	close_menu->setPosition(getContentPosition(kCSS_MT_close));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kCSS_MT_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kCSS_Z_content);
	
	
//	CommonButton* strength_menu = CommonButton::create("카드강화", 12, CCSizeMake(100,40), CommonButtonYellow, -185);
//	strength_menu->setTitleColor(ccBLACK);
//	strength_menu->setPosition(ccp(78,31));
//	main_case->addChild(strength_menu, kCSS_Z_content);
//	strength_menu->setFunction([=](CCObject* sender)
//							   {
//								   CCNode* t_node = CCNode::create();
//								   t_node->setTag(kCSS_MT_strength);
//								   menuAction(t_node);
//							   });
	
	
	align_default_menu = CommonButton::create("기본정렬", 12, CCSizeMake(75,37), CommonButtonYellowUp, -185);
	align_default_menu->setPosition(ccp(254,256));
	main_case->addChild(align_default_menu, kCSS_Z_alignButton);
	align_default_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kCSS_MT_alignDefault);
									 menuAction(t_node);
								 });
	align_default_menu->setBackgroundTypeForDisabled(CommonButtonYellowDown);
	
	
	align_take_menu = CommonButton::create("획득순", 12, CCSizeMake(75,37), CommonButtonYellowUp, -185);
	align_take_menu->setPosition(ccp(327,256));
	main_case->addChild(align_take_menu, kCSS_Z_alignButton);
	align_take_menu->setFunction([=](CCObject* sender)
									{
										CCNode* t_node = CCNode::create();
										t_node->setTag(kCSS_MT_alignTake);
										menuAction(t_node);
									});
	align_take_menu->setBackgroundTypeForDisabled(CommonButtonYellowDown);
	
	
	align_rank_menu = CommonButton::create("등급순", 12, CCSizeMake(75,37), CommonButtonYellowUp, -185);
	align_rank_menu->setPosition(ccp(400,256));
	main_case->addChild(align_rank_menu, kCSS_Z_alignButton);
	align_rank_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kCSS_MT_alignRank);
									 menuAction(t_node);
								 });
	align_rank_menu->setBackgroundTypeForDisabled(CommonButtonYellowDown);
	
	
	KSLabelTTF* take_card_ment = KSLabelTTF::create("획득한 카드 수", mySGD->getFont().c_str(), 12);
	take_card_ment->setPosition(ccp(110,255));
	main_case->addChild(take_card_ment, kCSS_Z_content);
	
	int open_stage_card_count = 0;
	int all_stage_card_count = 0;
	for(int i=1;i<=puzzle_count;i++)
	{
		int t_puzzle_number = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
		int t_start_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_startStage_i);
		int t_stage_count = NSDS_GI(t_puzzle_number, kSDS_PZ_stageCount_i);
		
		bool is_puzzle_open = myDSH->getBoolForKey(kDSH_Key_openPuzzleCnt)+1 >= i;
		
		for(int j=t_start_stage;j<t_start_stage+t_stage_count;j++)
		{
			int t_stage_card_count = NSDS_GI(j, kSDS_SI_cardCount_i);
			
			int condition_gold = NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, j);
			bool is_on_condition_gold = condition_gold == 0;
			
			int condition_stage = NSDS_GI(t_puzzle_number, kSDS_PZ_stage_int1_condition_stage_i, j);
			bool is_on_condition_stage = (condition_stage == 0) || (myDSH->getBoolForKey(kDSH_Key_isClearStage_int1, condition_stage));
			
			if(is_puzzle_open && ((is_on_condition_gold && is_on_condition_stage) || myDSH->getBoolForKey(kDSH_Key_isOpenStage_int1, j))) // open stage
				open_stage_card_count += t_stage_card_count;
			all_stage_card_count += t_stage_card_count;
		}
	}
	
	KSLabelTTF* take_card_count = KSLabelTTF::create(CCString::createWithFormat("%d / %d / %d", mySGD->getHasGottenCardsSize(), open_stage_card_count, all_stage_card_count)->getCString(), mySGD->getFont().c_str(), 12);
	take_card_count->setColor(ccc3(255, 150, 50));
	take_card_count->setPosition(ccp(185,255));
	main_case->addChild(take_card_count, kCSS_Z_content);
	
	
	
//	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
//	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
//	s_tip->setColor(ccGRAY);
//	
//	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(CardSettingPopup::menuAction));
//	tip_item->setTag(kCSS_MT_tip);
//	
//	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
//	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
//	addChild(tip_menu, kCSS_Z_content);
//	
//	tip_menu->setTouchPriority(-185);
	
	
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
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
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
	else
	{
//		if(!myDSH->getBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardsetting))
//		{
//			myDSH->setBoolForKey(kDSH_Key_was_opened_tutorial_dimed_cardsetting, true);
//			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//			t_suction->target_touch_began = t_suction;
//			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//			t_suction->setTouchEnabled(true);
//			
//			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardsetting.png");
//			dimed_tip->setPosition(ccp(240,160));
//			t_suction->addChild(dimed_tip);
//			
//			addChild(t_suction, kCSS_Z_popup);
//		}
	}
	
	is_menu_enable = true;
}

void CardSettingPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
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
	
	if(t_tag == kCSS_MT_close)			return_value = ccp(452,256);
	else if(t_tag == kCSS_MT_cardBase)	return_value = ccp(245,210);
	
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
			mySGD->setStrengthTargetCardNumber(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
			mySGD->setCardStrengthBefore(kCardStrengthBefore_cardSetting);
			CardStrengthPopup* t_popup = CardStrengthPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
			
			target_final = NULL;
			hidePopup();
		}
		else if(tag == kCSS_MT_tip)
		{
			TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
			t_suction->target_touch_began = t_suction;
			t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			t_suction->setTouchEnabled(true);
			
			CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardsetting.png");
			dimed_tip->setPosition(ccp(240,160));
			t_suction->addChild(dimed_tip);
			
			addChild(t_suction, kCSS_Z_popup);
			
			is_menu_enable = true;
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
		
//		int t_idx = idx*2;
		int found_stage1 = -1;
		int selected_cnt = 0;
		for(int i=1;i<=puzzle_count && found_stage1 == -1;i++)
		{
			int stage_count = server_puzzle_stage_count[i];
			
			if(idx >= selected_cnt && idx < selected_cnt+stage_count)
			{
				int start_stage = server_puzzle_start_stage[i];
				found_stage1 = idx-selected_cnt+start_stage;
			}
			else
				selected_cnt += stage_count;
		}
		
//		if(found_stage1 == -1)
//		{
//			int event_stage_count = server_event_list_count;
//			if(t_idx >= selected_cnt && t_idx < selected_cnt+event_stage_count)
//				found_stage1 = t_idx-selected_cnt+event_stage_count;
//			else
//				CCLog("not found stage");
//		}
		
		if(found_stage1 != -1)
		{
			KSLabelTTF* stage_number_label = KSLabelTTF::create(CCString::createWithFormat("%d", found_stage1)->getCString(), mySGD->getFont().c_str(), 20);
			stage_number_label->setColor(ccc3(255, 150, 50));
			stage_number_label->enableOuterStroke(ccBLACK, 1.f);
			stage_number_label->setAnchorPoint(ccp(0,0.5));
			stage_number_label->setPosition(ccp(10,67));
			cell->addChild(stage_number_label);
			
			KSLabelTTF* stage_ment_label = KSLabelTTF::create("스테이지", mySGD->getFont().c_str(), 13);
			stage_ment_label->enableOuterStroke(ccBLACK, 1.f);
			stage_ment_label->setAnchorPoint(ccp(0,0.5));
			stage_ment_label->setPosition(ccp(10,47));
			cell->addChild(stage_ment_label);
			
			int stage_card_count = 4;//NSDS_GI(found_stage1, kSDS_SI_cardCount_i);
			for(int i=1;i<=stage_card_count;i++)
			{
				int card_number = mySGD->isHasGottenCards(found_stage1, i);
				CCPoint card_position = ccp(100.f + (i-1)*(81.f), 50.f);
				if(card_number > 0)
				{
					GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																												  NSDS_GI(found_stage1, kSDS_SI_level_int1_card_i, i))->getCString()));
					CCSprite* n_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase%d.png", i)->getCString());
					n_case->setPosition(ccp(n_card->getContentSize().width/2.f, n_card->getContentSize().height/2.f));
					n_card->addChild(n_case);
					
					CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width, n_card->getContentSize().height));
					n_node->setOpacity(0);
					n_card->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
					n_node->addChild(n_card);
					
					GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																												  NSDS_GI(found_stage1, kSDS_SI_level_int1_card_i, i))->getCString()));
					s_card->setColor(ccGRAY);
					CCSprite* s_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase%d.png", i)->getCString());
					s_case->setPosition(ccp(s_card->getContentSize().width/2.f, s_card->getContentSize().height/2.f));
					s_card->addChild(s_case);
					
					CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width, s_card->getContentSize().height));
					s_node->setOpacity(0);
					s_card->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
					s_node->addChild(s_card);
					
					CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
					t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
					
					ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
					t_card_menu->setPosition(card_position);
					cell->addChild(t_card_menu);
					t_card_menu->setTouchPriority(-180-3);
					
//					CCPoint no_minus_half_size = ccp(-n_card->getContentSize().width/2.f, -n_card->getContentSize().height/2.f);
//					
//					CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number))->getCString(),
//																  mySGD->getFont().c_str(), 7);
//					t_durability->setAnchorPoint(ccp(0.5f,0.5f));
//					t_durability->setPosition(ccpAdd(card_position, ccp(20, no_minus_half_size.y+14)));
//					cell->addChild(t_durability);
//					
//					CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, card_number))->getCString(), mySGD->getFont().c_str(), 8);
//					t_card_level_label->setPosition(ccpAdd(card_position, ccpAdd(no_minus_half_size, ccp(48,73))));
//					cell->addChild(t_card_level_label);
					
					if(recent_selected_card_number == card_number)
					{
						CCSprite* select_img = CCSprite::create("card_check.png");
						select_img->setPosition(card_position);
						cell->addChild(select_img);
					}
				}
				else
				{
					CCSprite* n_card_img = CCSprite::create("cardsetting_blank.png");
					
					string condition_text;
					if(i == 1)
						condition_text = "85%";
					else if(i == 2)
						condition_text = "85%\n+ 체인지";
					else if(i == 3)
						condition_text = "100%";
					else
						condition_text = "100%\n+ 체인지";
					
					KSLabelTTF* n_condition = KSLabelTTF::create(condition_text.c_str(), mySGD->getFont().c_str(), 14);
					n_condition->enableOuterStroke(ccBLACK, 1.f);
					n_condition->setPosition(ccp(n_card_img->getContentSize().width/2.f, n_card_img->getContentSize().height/2.f));
					n_card_img->addChild(n_condition);
					
					CCSprite* n_case_img = CCSprite::create(CCString::createWithFormat("cardsetting_minicase%d.png", i)->getCString());
					n_case_img->setPosition(ccp(n_card_img->getContentSize().width/2.f, n_card_img->getContentSize().height/2.f));
					n_card_img->addChild(n_case_img);
					
					CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card_img->getContentSize().width, n_card_img->getContentSize().height));
					n_node->setOpacity(0);
					n_card_img->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
					n_node->addChild(n_card_img);
					
					
					CCSprite* s_card_img = CCSprite::create("cardsetting_blank.png");
					
					KSLabelTTF* s_condition = KSLabelTTF::create(condition_text.c_str(), mySGD->getFont().c_str(), 14);
					s_condition->enableOuterStroke(ccBLACK, 1.f);
					s_condition->setPosition(ccp(s_card_img->getContentSize().width/2.f, s_card_img->getContentSize().height/2.f));
					s_card_img->addChild(s_condition);
					
					CCSprite* s_case_img = CCSprite::create(CCString::createWithFormat("cardsetting_minicase%d.png", i)->getCString());
					s_case_img->setPosition(ccp(s_card_img->getContentSize().width/2.f, s_card_img->getContentSize().height/2.f));
					s_card_img->addChild(s_case_img);
					s_card_img->setColor(ccGRAY);
					
					CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card_img->getContentSize().width, s_card_img->getContentSize().height));
					s_node->setOpacity(0);
					s_card_img->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
					s_node->addChild(s_card_img);
					
					CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
					t_card_item->setTag(kCSS_MT_noCardBase+card_number);
					
					ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
					t_card_menu->setPosition(card_position);
					cell->addChild(t_card_menu);
					t_card_menu->setTouchPriority(-180-3);
				}
			}
		}
	}
	else
	{
		for(int i=idx*5;i<idx*5+5 && i<mySGD->getHasGottenCardsSize();i++)
		{
			int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
//			int card_durability = myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number);
			CCPoint card_position = ccp(42.f + (i-idx*5)*(81.f), 50.f);
			
			int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, card_number);
			
			GraySprite* n_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  card_number)->getCString()));
//			n_card->setGray(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) <= 0);
			CCSprite* n_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase%d.png", card_grade)->getCString());
			n_case->setPosition(ccp(n_card->getContentSize().width/2.f, n_card->getContentSize().height/2.f));
			n_card->addChild(n_case);
			
			CCSprite* n_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, n_card->getContentSize().width, n_card->getContentSize().height));
			n_node->setOpacity(0);
			n_card->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
			n_node->addChild(n_card);
			
			GraySprite* s_card = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("card%d_thumbnail.png",
																										  card_number)->getCString()));
//			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) <= 0)
//				s_card->setColor(ccc3(60, 60, 60));
//			else
				s_card->setColor(ccGRAY);
			CCSprite* s_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase%d.png", card_grade)->getCString());
			s_case->setPosition(ccp(s_card->getContentSize().width/2.f, s_card->getContentSize().height/2.f));
			s_card->addChild(s_case);
			
			CCSprite* s_node = CCSprite::create("whitePaper.png", CCRectMake(0, 0, s_card->getContentSize().width, s_card->getContentSize().height));
			s_node->setOpacity(0);
			s_card->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
			s_node->addChild(s_card);
			
			CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CardSettingPopup::menuAction));
			t_card_item->setTag(kCSS_MT_cardMenuBase+card_number);
			
			ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
			t_card_menu->setPosition(card_position);
			cell->addChild(t_card_menu);
			t_card_menu->setTouchPriority(-180-3);
			
//			CCPoint no_minus_half_size = ccp(-n_card->getContentSize().width/2.f, -n_card->getContentSize().height/2.f);
//			
//			CCLabelTTF* t_durability = CCLabelTTF::create(CCString::createWithFormat("%d", card_durability)->getCString(),
//														  mySGD->getFont().c_str(), 7);
//			t_durability->setAnchorPoint(ccp(0.5f,0.5f));
//			t_durability->setPosition(ccpAdd(card_position, ccp(20, no_minus_half_size.y+14)));
//			cell->addChild(t_durability);
//			
//			
//			CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, card_number))->getCString(), mySGD->getFont().c_str(), 8);
//			t_card_level_label->setPosition(ccpSub(card_position, ccpAdd(no_minus_half_size, ccp(48,73))));
//			cell->addChild(t_card_level_label);
			

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
	return CCSizeMake(392, 105);
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
