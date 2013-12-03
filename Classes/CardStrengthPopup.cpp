//
//  CardStrengthPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 3..
//
//

#include "CardStrengthPopup.h"
#include "DataStorageHub.h"
#include "CardSettingPopup.h"
#include "PuzzleMapScene.h"
#include "CardCase.h"

enum CardStrengthPopupZorder{
	kCardStrengthPopupZorder_gray = 1,
	kCardStrengthPopupZorder_back,
	kCardStrengthPopupZorder_content
};

enum CardStrengthPopupMenuTag{
	kCardStrengthPopupMenuTag_close = 1,
	kCardStrengthPopupMenuTag_targetCard,
	kCardStrengthPopupMenuTag_offeringCard,
	kCardStrengthPopupMenuTag_highStrength,
	kCardStrengthPopupMenuTag_normalStrength,
	kCardStrengthPopupMenuTag_alignRank,
	kCardStrengthPopupMenuTag_alignTake
};

void CardStrengthPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool CardStrengthPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	is_menu_enable = false;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCardStrengthPopupZorder_gray);
	
	main_case = CCSprite::create("cardstrength_back.png");
	main_case->setAnchorPoint(ccp(0,0));
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kCardStrengthPopupZorder_back);
	
	
	CCSprite* n_close = CCSprite::create("cardsetting_close.png");
	CCSprite* s_close = CCSprite::create("cardsetting_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(CardStrengthPopup::menuAction));
	close_item->setTag(kCardStrengthPopupMenuTag_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_close));
	main_case->addChild(close_menu, kCardStrengthPopupZorder_content);
	
	
	CCSprite* n_high_strength = CCSprite::create("cardstrength_high_strength.png");
	CCSprite* s_high_strength = CCSprite::create("cardstrength_high_strength.png");
	s_high_strength->setColor(ccGRAY);
	
	CCMenuItem* high_strength_item = CCMenuItemSprite::create(n_high_strength, s_high_strength, this, menu_selector(CardStrengthPopup::menuAction));
	high_strength_item->setTag(kCardStrengthPopupMenuTag_highStrength);
	
	CCMenu* high_strength_menu = CCMenu::createWithItem(high_strength_item);
	high_strength_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_highStrength));
	main_case->addChild(high_strength_menu, kCardStrengthPopupZorder_content);
	
	
	CCSprite* n_normal_strength = CCSprite::create("cardstrength_normal_strength.png");
	CCSprite* s_normal_strength = CCSprite::create("cardstrength_normal_strength.png");
	s_normal_strength->setColor(ccGRAY);
	
	CCMenuItem* normal_strength_item = CCMenuItemSprite::create(n_normal_strength, s_normal_strength, this, menu_selector(CardStrengthPopup::menuAction));
	normal_strength_item->setTag(kCardStrengthPopupMenuTag_normalStrength);
	
	CCMenu* normal_strength_menu = CCMenu::createWithItem(normal_strength_item);
	normal_strength_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_normalStrength));
	main_case->addChild(normal_strength_menu, kCardStrengthPopupZorder_content);
	
	
	CCSprite* n_align_rank = CCSprite::create("cardstrength_align_rank.png");
	CCSprite* s_align_rank = CCSprite::create("cardstrength_align_rank.png");
	s_align_rank->setColor(ccGRAY);
	
	CCMenuItem* align_rank_item = CCMenuItemSprite::create(n_align_rank, s_align_rank, this, menu_selector(CardStrengthPopup::menuAction));
	align_rank_item->setTag(kCardStrengthPopupMenuTag_alignRank);
	
	CCMenu* align_rank_menu = CCMenu::createWithItem(align_rank_item);
	align_rank_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_alignRank));
	main_case->addChild(align_rank_menu, kCardStrengthPopupZorder_content);
	
	
	CCSprite* n_align_take = CCSprite::create("cardstrength_align_take.png");
	CCSprite* s_align_take = CCSprite::create("cardstrength_align_take.png");
	s_align_take->setColor(ccGRAY);
	
	CCMenuItem* align_take_item = CCMenuItemSprite::create(n_align_take, s_align_take, this, menu_selector(CardStrengthPopup::menuAction));
	align_take_item->setTag(kCardStrengthPopupMenuTag_alignTake);
	
	CCMenu* align_take_menu = CCMenu::createWithItem(align_take_item);
	align_take_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_alignTake));
	main_case->addChild(align_take_menu, kCardStrengthPopupZorder_content);
	
	
	
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, selected_card_number);
	int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, selected_card_number);
	
	target_card = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
	target_card->setScale(1.1f);
	target_card->setPosition(getContentPosition(kCardStrengthPopupMenuTag_targetCard));
	main_case->addChild(target_card, kCardStrengthPopupZorder_content);
	
	CCSprite* no_img = CCSprite::create("cardsetting_noimg.png");
	no_img->setScale(1.f/0.92f);
	no_img->setPosition(ccp(target_card->getContentSize().width/2.f, target_card->getContentSize().height/2.f));
	target_card->addChild(no_img, kCardStrengthPopupZorder_content);
	
	CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
	mini_rank->setPosition(ccp(17,17));
	no_img->addChild(mini_rank);
	
	CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, selected_card_number))->getCString(), mySGD->getFont().c_str(), 8);
	t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
	mini_rank->addChild(t_rank);
	
	
	dur_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, selected_card_number),
															  mySD->getCardDurability(card_stage, card_grade))->getCString(), mySGD->getFont().c_str(), 10);
	dur_label->setPosition(ccpAdd(target_card->getPosition(), ccp(93,54)));
	main_case->addChild(dur_label);
	
	up_dur_label = CCLabelTTF::create("+0", mySGD->getFont().c_str(), 14);
	up_dur_label->setPosition(ccpAdd(dur_label->getPosition(), ccp(20,3)));
	main_case->addChild(up_dur_label);
	
	
	pow_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_missile_power_i, selected_card_number))->getCString(), mySGD->getFont().c_str(), 10);
	pow_label->setPosition(ccpAdd(target_card->getPosition(), ccp(93,35)));
	main_case->addChild(pow_label);
	
	up_pow_label = CCLabelTTF::create("+0", mySGD->getFont().c_str(), 14);
	up_pow_label->setPosition(ccpAdd(pow_label->getPosition(), ccp(20,3)));
	main_case->addChild(up_pow_label);
	
	
	spd_label = CCLabelTTF::create(CCString::createWithFormat("%.1f", NSDS_GD(kSDS_CI_int1_missile_speed_d, selected_card_number))->getCString(), mySGD->getFont().c_str(), 10);
	spd_label->setPosition(ccpAdd(target_card->getPosition(), ccp(93,16)));
	main_case->addChild(spd_label);
	
	up_spd_label = CCLabelTTF::create("+0", mySGD->getFont().c_str(), 14);
	up_spd_label->setPosition(ccpAdd(spd_label->getPosition(), ccp(20,3)));
	main_case->addChild(up_spd_label);
	
	
	dex_label = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_missile_dex_i, selected_card_number))->getCString(), mySGD->getFont().c_str(), 10);
	dex_label->setPosition(ccpAdd(target_card->getPosition(), ccp(93,-3)));
	main_case->addChild(dex_label);
	
	up_dex_label = CCLabelTTF::create("+0", mySGD->getFont().c_str(), 14);
	up_dex_label->setPosition(ccpAdd(dex_label->getPosition(), ccp(20,3)));
	main_case->addChild(up_dex_label);
	
	string missile_type_code = NSDS_GS(kSDS_CI_int1_missile_type_s, selected_card_number).c_str();
	int missile_type_number = MissileDamageData::getMissileType(missile_type_code.c_str());
	
	int elemental_number = missile_type_number%10;
	int level_number = missile_type_number/10 + 1;
	
	string type_name;
	
	if(elemental_number == kElementCode_empty)
		type_name = "empty";
	else if(elemental_number == kElementCode_life)
		type_name = "life";
	else if(elemental_number == kElementCode_fire)
		type_name = "fire";
	else if(elemental_number == kElementCode_water)
		type_name = "water";
	else if(elemental_number == kElementCode_wind)
		type_name = "wind";
	else if(elemental_number == kElementCode_lightning)
		type_name = "lightning";
	else if(elemental_number == kElementCode_plasma)
		type_name = "plasma";
	
	if(elemental_number == kElementCode_empty || elemental_number == kElementCode_fire || elemental_number == kElementCode_life || elemental_number == kElementCode_water)
	{
		SpinBasicMissile* missile_img = SpinBasicMissile::create(type_name.c_str(), level_number, ccpAdd(target_card->getPosition(), ccp(105,17)), false);
		missile_img->setScale(0.75f);
		main_case->addChild(missile_img, kCardStrengthPopupZorder_content);
	}
	else
	{
		SpinUpgradeMissile* missile_img = SpinUpgradeMissile::create(type_name.c_str(), level_number, ccpAdd(target_card->getPosition(), ccp(105,17)), false);
		missile_img->setScale(0.75f);
		main_case->addChild(missile_img, kCardStrengthPopupZorder_content);
	}
	
	
	setOfferingList();
	
	CCSize table_size = CCSizeMake(350, 70);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(ccp(97, 29));
//	main_case->addChild(temp_back, kCardStrengthPopupZorder_content);
	
	offering_table = CCTableView::create(this, table_size);
	offering_table->setAnchorPoint(CCPointZero);
	offering_table->setDirection(kCCScrollViewDirectionHorizontal);
	offering_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	offering_table->setPosition(ccp(97, 29));
	offering_table->setDelegate(this);
	main_case->addChild(offering_table, kCardStrengthPopupZorder_content);
	offering_table->setTouchPriority(kCCMenuHandlerPriority+1);
	
	
	offering_menu = NULL;
	
	
    return true;
}

void CardStrengthPopup::setOfferingList()
{
	int selected_card_number = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
	int loop_length = mySGD->getHasGottenCardsSize();
	for(int i=0;i<loop_length;i++)
	{
		int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
		if(selected_card_number != card_number && myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
		{
			CardSortInfo t_info = mySGD->getHasGottenCardData(i);
			offering_list.push_back(t_info);
		}
	}
	
	alignOfferingList(kCardStrengthAlign_takeReverse);
	
	recent_offering_number = -1;
}

void CardStrengthPopup::alignOfferingList(CardStrengthAlign t_align)
{
	recent_align = t_align;
	
	if(t_align == kCardStrengthAlign_rank)
	{
		struct t_CardSortGradeUp{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank > b.rank;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kCardStrengthAlign_rankReverse)
	{
		struct t_CardSortGradeDown{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.rank < b.rank;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kCardStrengthAlign_take)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number > b.take_number;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kCardStrengthAlign_takeReverse)
	{
		struct t_CardSortTake{
			bool operator() (const CardSortInfo& a, const CardSortInfo& b)
			{
				return a.take_number < b.take_number;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
}

CCTableViewCell* CardStrengthPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, offering_list[idx].card_number);
	int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, offering_list[idx].card_number);
	
	CCSprite* offering_card = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
	offering_card->setScale(0.73f);
	offering_card->setPosition(ccp(30, 35));
	cell->addChild(offering_card);
	
	CCSprite* no_img = CCSprite::create("cardsetting_noimg.png");
	no_img->setScale(0.8f);
	no_img->setPosition(ccp(30, 35));
	cell->addChild(no_img);
	
	CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
	mini_rank->setPosition(ccp(17,17));
	no_img->addChild(mini_rank);
	
	CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, offering_list[idx].card_number))->getCString(), mySGD->getFont().c_str(), 8);
	t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
	mini_rank->addChild(t_rank);
	
	return cell;
}
void CardStrengthPopup::scrollViewDidScroll(CCScrollView* view)
{
	
}
void CardStrengthPopup::scrollViewDidZoom(CCScrollView* view)
{
	
}
void CardStrengthPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	if(!is_menu_enable)
		return;
	
	int selected_card_number = offering_list[cell->getIdx()].card_number;
	if(recent_offering_number == selected_card_number)
		return;
	
	is_menu_enable = false;
	
	recent_offering_number = selected_card_number;
	if(offering_menu)
		offering_menu->removeFromParent();
	
	int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, selected_card_number);
	int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, selected_card_number);
	
	CCSprite* n_offering = CCSprite::create("cardsetting_noimg.png");
	n_offering->setScale(1.1f);
	CCSprite* n_card_img = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
	n_card_img->setScale(1.05f/1.1f);
	n_card_img->setPosition(ccp(n_offering->getContentSize().width/2.f, n_offering->getContentSize().height/2.f));
	n_offering->addChild(n_card_img, -1);
	
	CCSprite* n_mini_rank = CCSprite::create("cardsetting_mini_rank.png");
	n_mini_rank->setPosition(ccp(17,17));
	n_offering->addChild(n_mini_rank);
	
	CCLabelTTF* n_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, selected_card_number))->getCString(), mySGD->getFont().c_str(), 8);
	n_rank->setPosition(ccp(n_mini_rank->getContentSize().width/2.f, n_mini_rank->getContentSize().height/2.f-1));
	n_mini_rank->addChild(n_rank);
	
	
	CCSprite* s_offering = CCSprite::create("cardsetting_noimg.png");
	s_offering->setScale(1.1f);
	s_offering->setColor(ccGRAY);
	CCSprite* s_card_img = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
	s_card_img->setScale(1.05f/1.1f);
	s_card_img->setPosition(ccp(s_offering->getContentSize().width/2.f, s_offering->getContentSize().height/2.f));
	s_card_img->setColor(ccGRAY);
	s_offering->addChild(s_card_img, -1);
	
	CCSprite* s_mini_rank = CCSprite::create("cardsetting_mini_rank.png");
	s_mini_rank->setPosition(ccp(17,17));
	s_mini_rank->setColor(ccGRAY);
	s_offering->addChild(s_mini_rank);
	
	CCLabelTTF* s_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, selected_card_number))->getCString(), mySGD->getFont().c_str(), 8);
	s_rank->setPosition(ccp(s_mini_rank->getContentSize().width/2.f, s_mini_rank->getContentSize().height/2.f-1));
	s_mini_rank->addChild(s_rank);
	
	
	CCMenuItem* offering_item = CCMenuItemSprite::create(n_offering, s_offering, this, menu_selector(CardStrengthPopup::menuAction));
	offering_item->setTag(kCardStrengthPopupMenuTag_offeringCard);
	
	offering_menu = CCMenu::createWithItem(offering_item);
	offering_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_offeringCard));
	main_case->addChild(offering_menu, kCardStrengthPopupZorder_content);
	
	is_menu_enable = true;
}
CCSize CardStrengthPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(60, 70);
}
unsigned int CardStrengthPopup::numberOfCellsInTableView(CCTableView *table)
{
	return offering_list.size();
}

void CardStrengthPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void CardStrengthPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,0));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardStrengthPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardStrengthPopup::endShowPopup()
{
	is_menu_enable = true;
}

void CardStrengthPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(CardStrengthPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void CardStrengthPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

CCPoint CardStrengthPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kCardStrengthPopupMenuTag_close)						return_value = ccp(440,290);
	else if(t_tag == kCardStrengthPopupMenuTag_highStrength)			return_value = ccp(408,210);
	else if(t_tag == kCardStrengthPopupMenuTag_normalStrength)			return_value = ccp(408,150);
	else if(t_tag == kCardStrengthPopupMenuTag_alignRank)				return_value = ccp(60,85);
	else if(t_tag == kCardStrengthPopupMenuTag_alignTake)				return_value = ccp(60,45);
	else if(t_tag == kCardStrengthPopupMenuTag_targetCard)				return_value = ccp(77,181);
	else if(t_tag == kCardStrengthPopupMenuTag_offeringCard)			return_value = ccp(299,178);
	
	return return_value;
}

void CardStrengthPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kCardStrengthPopupMenuTag_close)
	{
		CardSettingPopup* t_popup = CardSettingPopup::create();
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, kPMS_Z_popup);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == kCardStrengthPopupMenuTag_alignRank)
	{
		if(recent_align == kCardStrengthAlign_rankReverse)
			alignOfferingList(kCardStrengthAlign_rank);
		else
			alignOfferingList(kCardStrengthAlign_rankReverse);

		offering_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kCardStrengthPopupMenuTag_alignTake)
	{
		if(recent_align == kCardStrengthAlign_takeReverse)
			alignOfferingList(kCardStrengthAlign_take);
		else
			alignOfferingList(kCardStrengthAlign_takeReverse);
		
		offering_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kCardStrengthPopupMenuTag_offeringCard)
	{
		if(offering_menu)
		{
			offering_menu->removeFromParent();
			recent_offering_number = -1;
			offering_menu = NULL;
		}
		
		is_menu_enable = true;
	}
}