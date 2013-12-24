//
//  CardChangePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 19..
//
//

#include "CardChangePopup.h"
#include "DataStorageHub.h"
#include "CardCase.h"
#include "StageImgLoader.h"
#include "CumberShowWindow.h"

enum CardChangePopupZorder{
	kCardChangePopupZorder_gray = 1,
	kCardChangePopupZorder_back,
	kCardChangePopupZorder_content,
	kCardChangePopupZorder_popup
};

enum CardChangePopupMenuTag{
	kCardChangePopupMenuTag_close = 1,
	kCardChangePopupMenuTag_mount,
	kCardChangePopupMenuTag_release,
	kCardChangePopupMenuTag_alignRank,
	kCardChangePopupMenuTag_alignTake
};

void CardChangePopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool CardChangePopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	is_menu_enable = false;
	base_touch_priority = -200;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCardChangePopupZorder_gray);
	
	main_case = CCSprite::create("cardchange_back.png");
	main_case->setAnchorPoint(ccp(0,0));
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kCardChangePopupZorder_back);
	
	
	CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
	CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(CardChangePopup::menuAction));
	close_item->setTag(kCardChangePopupMenuTag_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(ccp(440, 261));
	main_case->addChild(close_menu, kCardChangePopupZorder_content);
	close_menu->setTouchPriority(base_touch_priority);
	
	
	CCSprite* n_align_take = CCSprite::create("cardchange_align_take.png");
	CCSprite* s_align_take = CCSprite::create("cardchange_align_take.png");
	s_align_take->setColor(ccGRAY);
	
	CCMenuItem* align_take_item = CCMenuItemSprite::create(n_align_take, s_align_take, this, menu_selector(CardChangePopup::menuAction));
	align_take_item->setTag(kCardChangePopupMenuTag_alignTake);
	
	CCMenu* align_take_menu = CCMenu::createWithItem(align_take_item);
	align_take_menu->setPosition(ccp(352, 113));
	main_case->addChild(align_take_menu, kCardChangePopupZorder_content);
	align_take_menu->setTouchPriority(base_touch_priority);
	
	
	CCSprite* n_align_rank = CCSprite::create("cardchange_align_rank.png");
	CCSprite* s_align_rank = CCSprite::create("cardchange_align_rank.png");
	s_align_rank->setColor(ccGRAY);
	
	CCMenuItem* align_rank_item = CCMenuItemSprite::create(n_align_rank, s_align_rank, this, menu_selector(CardChangePopup::menuAction));
	align_rank_item->setTag(kCardChangePopupMenuTag_alignRank);
	
	CCMenu* align_rank_menu = CCMenu::createWithItem(align_rank_item);
	align_rank_menu->setPosition(ccp(412, 113));
	main_case->addChild(align_rank_menu, kCardChangePopupZorder_content);
	align_rank_menu->setTouchPriority(base_touch_priority);
	
	selected_card = NULL;
	mount_menu = NULL;
	release_menu = NULL;
	clicked_card_number = 0;
	setSelectedCard(myDSH->getIntegerForKey(kDSH_Key_selectedCard));
	setHaveCardList();
	
	CCSize table_size = CCSizeMake(245, 71);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(ccp(200, 28));
//	main_case->addChild(temp_back, kCardChangePopupZorder_content);
	
	have_card_table = CCTableView::create(this, table_size);
	have_card_table->setAnchorPoint(CCPointZero);
	have_card_table->setDirection(kCCScrollViewDirectionHorizontal);
	have_card_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	have_card_table->setPosition(ccp(200, 28));
	have_card_table->setDelegate(this);
	main_case->addChild(have_card_table, kCardChangePopupZorder_content);
	have_card_table->setTouchPriority(base_touch_priority);
	
	setMonster();
	
    return true;
}

void CardChangePopup::setSelectedCard(int t_card_number)
{
	if(selected_card)
	{
		selected_card->removeFromParent();
		selected_card = NULL;
	}
	if(mount_menu)
	{
		mount_menu->removeFromParent();
		mount_menu = NULL;
	}
	if(release_menu)
	{
		release_menu->removeFromParent();
		release_menu = NULL;
	}
	
	if(t_card_number <= 0)
		return;
	
	int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, t_card_number);
	int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
	
	selected_card = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_grade)->getCString());
	selected_card->setScale(0.36f);
	selected_card->setPosition(ccp(107,144));
	main_case->addChild(selected_card);
	
	if(card_grade == 3 && mySD->isAnimationStage(card_stage))
	{
		CCSize ani_size = mySD->getAnimationCutSize(card_stage);
		CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_grade)->getCString(),
											  CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(mySD->getAnimationPosition(card_stage));
		selected_card->addChild(t_ani);
	}
	
	CardCase* t_case = CardCase::create(card_stage, card_grade);
	t_case->setPosition(CCPointZero);
	selected_card->addChild(t_case);
	
	if(t_card_number == myDSH->getIntegerForKey(kDSH_Key_selectedCard)) // mounted card
	{
		addMountedCase();
		
		CCSprite* n_release = CCSprite::create("cardchange_release.png");
		CCSprite* s_release = CCSprite::create("cardchange_release.png");
		s_release->setColor(ccGRAY);
		
		CCMenuItem* release_item = CCMenuItemSprite::create(n_release, s_release, this, menu_selector(CardChangePopup::menuAction));
		release_item->setTag(kCardChangePopupMenuTag_release);
		
		release_menu = CCMenu::createWithItem(release_item);
		release_menu->setPosition(ccpAdd(selected_card->getPosition(), ccp(0,-100)));
		main_case->addChild(release_menu);
		release_menu->setTouchPriority(base_touch_priority);
	}
	else if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, t_card_number) > 0) // mountable card
	{
		CCSprite* n_mount = CCSprite::create("cardchange_mount.png");
		CCSprite* s_mount = CCSprite::create("cardchange_mount.png");
		s_mount->setColor(ccGRAY);
		
		CCMenuItem* mount_item = CCMenuItemSprite::create(n_mount, s_mount, this, menu_selector(CardChangePopup::menuAction));
		mount_item->setTag(kCardChangePopupMenuTag_mount);
		
		mount_menu = CCMenu::createWithItem(mount_item);
		mount_menu->setPosition(ccpAdd(selected_card->getPosition(), ccp(0,-100)));
		main_case->addChild(mount_menu);
		mount_menu->setTouchPriority(base_touch_priority);
	}
	
	clicked_card_number = t_card_number;
}

void CardChangePopup::addMountedCase()
{
	CCSprite* top_case = CCSprite::create("card_case_check_top.png");
	top_case->setPosition(ccp(160,430));
	selected_card->addChild(top_case, 1, kCARDCASE_Tag_mounted_top);
	
	CCSprite* bottom_case = CCSprite::create("card_case_check_bottom.png");
	bottom_case->setPosition(ccp(160,0));
	selected_card->addChild(bottom_case, 1, kCARDCASE_Tag_mounted_bottom);
	
	CCSprite* left_case = CCSprite::create("card_case_check_left.png");
	left_case->setPosition(ccp(0,215));
	selected_card->addChild(left_case, 1, kCARDCASE_Tag_mounted_left);
	
	CCSprite* right_case = CCSprite::create("card_case_check_right.png");
	right_case->setPosition(ccp(320,215));
	selected_card->addChild(right_case, 1, kCARDCASE_Tag_mounted_right);
}

void CardChangePopup::setHaveCardList()
{
	int loop_length = mySGD->getHasGottenCardsSize();
	for(int i=0;i<loop_length;i++)
	{
		int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
		if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
		{
			CardSortInfo t_info = mySGD->getHasGottenCardData(i);
			have_card_list.push_back(t_info);
		}
	}
	
	alignHaveCardList(kCardChangeAlign_takeReverse);
}

void CardChangePopup::alignHaveCardList(CardChangeAlign t_align)
{
	recent_align = t_align;
	
	if(t_align == kCardChangeAlign_rank)
	{
		struct t_CardSortGradeUp{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank > b.rank;
			}
		} pred;
		
		sort(have_card_list.begin(), have_card_list.end(), pred);
	}
	else if(t_align == kCardChangeAlign_rankReverse)
	{
		struct t_CardSortGradeDown{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank < b.rank;
			}
		} pred;
		
		sort(have_card_list.begin(), have_card_list.end(), pred);
	}
	else if(t_align == kCardChangeAlign_take)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number > b.take_number;
			}
		} pred;
		
		sort(have_card_list.begin(), have_card_list.end(), pred);
	}
	else if(t_align == kCardChangeAlign_takeReverse)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number < b.take_number;
			}
		} pred;
		
		sort(have_card_list.begin(), have_card_list.end(), pred);
	}
}

enum CardChangeTableCellZorder{
	kCardChangeTableCellZorder_img = 0,
	kCardChangeTableCellZorder_noimg,
	kCardChangeTableCellZorder_touched,
	kCardChangeTableCellZorder_mounted
};

CCTableViewCell* CardChangePopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, have_card_list[idx].card_number);
	int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, have_card_list[idx].card_number);
	
	CCPoint card_center_position = ccp(30, 35);
	
	CCSprite* have_card = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
	have_card->setScale(0.73f);
	have_card->setPosition(card_center_position);
	cell->addChild(have_card, kCardChangeTableCellZorder_img);
	
	CCSprite* no_img = CCSprite::create("cardchange_noimg.png");
	no_img->setPosition(card_center_position);
	cell->addChild(no_img, kCardChangeTableCellZorder_noimg);
	
	CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
	mini_rank->setPosition(ccp(17,17));
	no_img->addChild(mini_rank);
	
	CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, have_card_list[idx].card_number))->getCString(), mySGD->getFont().c_str(), 8);
	t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
	mini_rank->addChild(t_rank);
	
	CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, have_card_list[idx].card_number))->getCString(), mySGD->getFont().c_str(), 8);
	t_card_level_label->setPosition(ccp(17,60));
	cell->addChild(t_card_level_label, kCardChangeTableCellZorder_noimg);
	
	if(have_card_list[idx].card_number == myDSH->getIntegerForKey(kDSH_Key_selectedCard))
	{
		CCSprite* small_selected_img = CCSprite::create("cardchange_selected.png");
		small_selected_img->setPosition(card_center_position);
		cell->addChild(small_selected_img, kCardChangeTableCellZorder_mounted);
	}
	else if(have_card_list[idx].card_number == clicked_card_number)
	{
		CCSprite* clicked_img = CCSprite::create("cardchange_clicked.png");
		clicked_img->setPosition(card_center_position);
		cell->addChild(clicked_img, kCardChangeTableCellZorder_touched);
	}
	
	cell->setTag(have_card_list[idx].card_number);
	
	return cell;
}
void CardChangePopup::scrollViewDidScroll(CCScrollView* view){}
void CardChangePopup::scrollViewDidZoom(CCScrollView* view){}
void CardChangePopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if(!is_menu_enable)
		return;
	
	int touched_card_number = have_card_list[cell->getIdx()].card_number;
	if(touched_card_number == clicked_card_number)
		return;
	
	is_menu_enable = false;
	
	if(clicked_card_number > 0)
	{
		int found_idx = -1;
		for(int i=0;i<numberOfCellsInTableView(table) && found_idx == -1;i++)
		{
			CCTableViewCell* t_cell = table->cellAtIndex(i);
			if(t_cell)
			{
				int cell_card_number = t_cell->getTag();
				if(cell_card_number == clicked_card_number)
					found_idx = i;
			}
		}
		setSelectedCard(touched_card_number);
		if(found_idx != -1)
			table->updateCellAtIndex(found_idx);
		table->updateCellAtIndex(cell->getIdx());
	}
	else
	{
		setSelectedCard(touched_card_number);
		table->updateCellAtIndex(cell->getIdx());
	}
	
	is_menu_enable = true;
}
CCSize CardChangePopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(56, 71);
}
unsigned int CardChangePopup::numberOfCellsInTableView(CCTableView *table)
{
	return have_card_list.size();
}

void CardChangePopup::setMonster()
{
	CumberShowWindow* t_monster = CumberShowWindow::create(myDSH->getIntegerForKey(kDSH_Key_lastSelectedStageForPuzzle_int1, myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)), kCumberShowWindowSceneCode_cardChange);
	t_monster->setPosition(ccp(256,183));
	main_case->addChild(t_monster);
}

void CardChangePopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void CardChangePopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardChangePopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardChangePopup::endShowPopup()
{
	is_menu_enable = true;
}

void CardChangePopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardChangePopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardChangePopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void CardChangePopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kCardChangePopupMenuTag_close)
	{
		hidePopup();
	}
	else if(tag == kCardChangePopupMenuTag_alignRank)
	{
		if(recent_align == kCardChangeAlign_rankReverse)
			alignHaveCardList(kCardChangeAlign_rank);
		else
			alignHaveCardList(kCardChangeAlign_rankReverse);
		
		have_card_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kCardChangePopupMenuTag_alignTake)
	{
		if(recent_align == kCardChangeAlign_takeReverse)
			alignHaveCardList(kCardChangeAlign_take);
		else
			alignHaveCardList(kCardChangeAlign_takeReverse);
		
		have_card_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kCardChangePopupMenuTag_mount)
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, clicked_card_number);
		if(selected_card_number > 0)
		{
			int found_idx = -1;
			for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
			{
				CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
				if(t_cell)
				{
					int cell_card_number = t_cell->getTag();
					if(cell_card_number == selected_card_number)
						found_idx = i;
				}
			}
			if(found_idx != -1)
				have_card_table->updateCellAtIndex(found_idx);
		}
		
		setSelectedCard(clicked_card_number);
		int found_idx = -1;
		for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
		{
			CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
			if(t_cell)
			{
				int cell_card_number = t_cell->getTag();
				if(cell_card_number == clicked_card_number)
					found_idx = i;
			}
		}
		if(found_idx != -1)
			have_card_table->updateCellAtIndex(found_idx);
		is_menu_enable = true;
	}
	else if(tag == kCardChangePopupMenuTag_release)
	{
		int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
		myDSH->setIntegerForKey(kDSH_Key_selectedCard, 0);
		clicked_card_number = 0;
		int found_idx = -1;
		for(int i=0;i<numberOfCellsInTableView(have_card_table) && found_idx == -1;i++)
		{
			CCTableViewCell* t_cell = have_card_table->cellAtIndex(i);
			if(t_cell)
			{
				int cell_card_number = t_cell->getTag();
				if(cell_card_number == selected_card_number)
					found_idx = i;
			}
		}
		if(found_idx != -1)
			have_card_table->updateCellAtIndex(found_idx);
		
		setSelectedCard(clicked_card_number);
		is_menu_enable = true;
	}
}