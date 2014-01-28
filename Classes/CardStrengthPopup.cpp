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
#include "StartSettingScene.h"
#include "MainFlowScene.h"
#include "CardCase.h"
#include <random>
#include "TutorialFlowStep.h"
#include "TouchSuctionLayer.h"
#include "ASPopupView.h"
#include "CardChangePopup.h"
#include "CollectionBookPopup.h"
#include "GraySprite.h"

enum CardStrengthPopupZorder{
	kCardStrengthPopupZorder_gray = 1,
	kCardStrengthPopupZorder_back,
	kCardStrengthPopupZorder_content,
	kCardStrengthPopupZorder_highContent,
	kCardStrengthPopupZorder_popup
};

enum CardStrengthPopupMenuTag{
	kCardStrengthPopupMenuTag_close = 1,
	kCardStrengthPopupMenuTag_strengthCard,
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
	replay_menu = NULL;
	
	card_strength_before = mySGD->getCardStrengthBefore();
	
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
	
	
	CCSprite* n_close = CCSprite::create("cardchange_cancel.png");
	CCSprite* s_close = CCSprite::create("cardchange_cancel.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(CardStrengthPopup::menuAction));
	close_item->setTag(kCardStrengthPopupMenuTag_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_close));
	main_case->addChild(close_menu, kCardStrengthPopupZorder_content);
	close_menu->setTouchPriority(-184);
	
	
	CCSprite* n_high_strength = CCSprite::create("cardstrength_high_strength.png");
	CCSprite* n_ruby_img = CCSprite::create("price_ruby_img.png");
	n_ruby_img->setScale(0.7f);
	n_ruby_img->setPosition(ccp(n_high_strength->getContentSize().width/2.f-18, n_high_strength->getContentSize().height/2.f-10));
	n_high_strength->addChild(n_ruby_img);
	
	CCLabelTTF* n_ruby_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getCardUpgradeRubyFee())->getCString(), mySGD->getFont().c_str(), 10);
	n_ruby_label->setPosition(ccp(n_high_strength->getContentSize().width/2.f+8, n_high_strength->getContentSize().height/2.f-11));
	n_high_strength->addChild(n_ruby_label);
	
	
	CCSprite* s_high_strength = CCSprite::create("cardstrength_high_strength.png");
	s_high_strength->setColor(ccGRAY);
	CCSprite* s_ruby_img = CCSprite::create("price_ruby_img.png");
	s_ruby_img->setScale(0.7f);
	s_ruby_img->setPosition(ccp(s_high_strength->getContentSize().width/2.f-18, s_high_strength->getContentSize().height/2.f-10));
	s_high_strength->addChild(s_ruby_img);
	
	CCLabelTTF* s_ruby_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getCardUpgradeRubyFee())->getCString(), mySGD->getFont().c_str(), 10);
	s_ruby_label->setPosition(ccp(s_high_strength->getContentSize().width/2.f+8, s_high_strength->getContentSize().height/2.f-11));
	s_high_strength->addChild(s_ruby_label);
	
	
	CCMenuItem* high_strength_item = CCMenuItemSprite::create(n_high_strength, s_high_strength, this, menu_selector(CardStrengthPopup::menuAction));
	high_strength_item->setTag(kCardStrengthPopupMenuTag_highStrength);
	
	CCMenu* high_strength_menu = CCMenu::createWithItem(high_strength_item);
	high_strength_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_highStrength));
	main_case->addChild(high_strength_menu, kCardStrengthPopupZorder_content);
	high_strength_menu->setTouchPriority(-184);
	
	
	CCSprite* n_normal_strength = CCSprite::create("cardstrength_normal_strength.png");
	CCSprite* n_gold_img = CCSprite::create("price_gold_img.png");
	n_gold_img->setScale(0.7f);
	n_gold_img->setPosition(ccp(n_normal_strength->getContentSize().width/2.f-18, n_normal_strength->getContentSize().height/2.f-10));
	n_normal_strength->addChild(n_gold_img);
	
	CCLabelTTF* n_gold_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getCardUpgradeGoldFee())->getCString(), mySGD->getFont().c_str(), 10);
	n_gold_label->setPosition(ccp(n_normal_strength->getContentSize().width/2.f+8, n_normal_strength->getContentSize().height/2.f-11));
	n_normal_strength->addChild(n_gold_label);
	
	CCSprite* s_normal_strength = CCSprite::create("cardstrength_normal_strength.png");
	s_normal_strength->setColor(ccGRAY);
	CCSprite* s_gold_img = CCSprite::create("price_gold_img.png");
	s_gold_img->setScale(0.7f);
	s_gold_img->setPosition(ccp(s_normal_strength->getContentSize().width/2.f-18, s_normal_strength->getContentSize().height/2.f-10));
	s_normal_strength->addChild(s_gold_img);
	
	CCLabelTTF* s_gold_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getCardUpgradeGoldFee())->getCString(), mySGD->getFont().c_str(), 10);
	s_gold_label->setPosition(ccp(s_normal_strength->getContentSize().width/2.f+8, s_normal_strength->getContentSize().height/2.f-11));
	s_normal_strength->addChild(s_gold_label);
	
	CCMenuItem* normal_strength_item = CCMenuItemSprite::create(n_normal_strength, s_normal_strength, this, menu_selector(CardStrengthPopup::menuAction));
	normal_strength_item->setTag(kCardStrengthPopupMenuTag_normalStrength);
	
	CCMenu* normal_strength_menu = CCMenu::createWithItem(normal_strength_item);
	normal_strength_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_normalStrength));
	main_case->addChild(normal_strength_menu, kCardStrengthPopupZorder_content);
	normal_strength_menu->setTouchPriority(-184);
	
	
	CCSprite* n_align_rank = CCSprite::create("cardstrength_align_rank.png");
	CCSprite* s_align_rank = CCSprite::create("cardstrength_align_rank.png");
	s_align_rank->setColor(ccGRAY);
	
	CCMenuItem* align_rank_item = CCMenuItemSprite::create(n_align_rank, s_align_rank, this, menu_selector(CardStrengthPopup::menuAction));
	align_rank_item->setTag(kCardStrengthPopupMenuTag_alignRank);
	
	CCMenu* align_rank_menu = CCMenu::createWithItem(align_rank_item);
	align_rank_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_alignRank));
	main_case->addChild(align_rank_menu, kCardStrengthPopupZorder_content);
	align_rank_menu->setTouchPriority(-184);
	
	
	CCSprite* n_align_take = CCSprite::create("cardstrength_align_take.png");
	CCSprite* s_align_take = CCSprite::create("cardstrength_align_take.png");
	s_align_take->setColor(ccGRAY);
	
	CCMenuItem* align_take_item = CCMenuItemSprite::create(n_align_take, s_align_take, this, menu_selector(CardStrengthPopup::menuAction));
	align_take_item->setTag(kCardStrengthPopupMenuTag_alignTake);
	
	CCMenu* align_take_menu = CCMenu::createWithItem(align_take_item);
	align_take_menu->setPosition(getContentPosition(kCardStrengthPopupMenuTag_alignTake));
	main_case->addChild(align_take_menu, kCardStrengthPopupZorder_content);
	align_take_menu->setTouchPriority(-184);
	
	
	CCSprite* strength_tab = CCSprite::create("cardstrength_tab_strength.png");
	strength_tab->setPosition(ccp(135,225));
	main_case->addChild(strength_tab, kCardStrengthPopupZorder_highContent);
	
	CCSprite* offering_tab = CCSprite::create("cardstrength_tab_offering.png");
	offering_tab->setPosition(ccp(229,225));
	main_case->addChild(offering_tab, kCardStrengthPopupZorder_highContent);
	
	
	CCSprite* plus_mark = CCSprite::create("cardstrength_plus.png");
	plus_mark->setPosition(ccp(198.5f,175));
	main_case->addChild(plus_mark, kCardStrengthPopupZorder_highContent);
	
	
	strength_node = NULL;
	offering_node = NULL;
	strength_probability = NULL;
	
	setStrengthNode(mySGD->getStrengthTargetCardNumber());
	setOfferingNode(0);
	
	setOfferingList();
	
	CCSize table_size = CCSizeMake(341, 78);
	CCPoint table_position = ccp(29, 20);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(table_position);
//	main_case->addChild(temp_back, kCardStrengthPopupZorder_content);
	
	offering_table = CCTableView::create(this, table_size);
	offering_table->setAnchorPoint(CCPointZero);
	offering_table->setDirection(kCCScrollViewDirectionHorizontal);
	offering_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	offering_table->setPosition(table_position);
	offering_table->setDelegate(this);
	main_case->addChild(offering_table, kCardStrengthPopupZorder_content);
	offering_table->setTouchPriority(-180);
	
	TouchSuctionLayer* suction = TouchSuctionLayer::create(-181);
	suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	suction->setTouchEnabled(true);
	main_case->addChild(suction);
	
	table_tab = NULL;
	setTableTab();
	
    return true;
}

void CardStrengthPopup::setTableTab()
{
	if(table_tab)
	{
		table_tab->removeFromParent();
		table_tab = NULL;
	}
	
	if(strength_card_number == 0)
	{
		table_tab = CCSprite::create("cardstrength_table_tab_strength.png");
		table_tab->setPosition(ccp(60,101));
		main_case->addChild(table_tab, kCardStrengthPopupZorder_highContent);
	}
	else
	{
		table_tab = CCSprite::create("cardstrength_table_tab_offering.png");
		table_tab->setPosition(ccp(60,101));
		main_case->addChild(table_tab, kCardStrengthPopupZorder_highContent);
	}
}

void CardStrengthPopup::setStrengthNode(int t_card_number)
{
	if(strength_node)
	{
		strength_node->removeFromParent();
		strength_node = NULL;
	}
	
	strength_card_number = t_card_number;
	
	if(strength_card_number == 0)
	{
		strength_node = CCSprite::create("cardstrength_frame.png");
		strength_node->setPosition(ccp(152,174));
		main_case->addChild(strength_node, kCardStrengthPopupZorder_content);
		
		CCSprite* tip = CCSprite::create("cardstrength_tip_strength.png");
		tip->setPosition(ccp(strength_node->getContentSize().width/2.f, strength_node->getContentSize().height/2.f));
		strength_node->addChild(tip);
		
		strength_idx = -1;
	}
	else
	{
		int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, strength_card_number);
		int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, strength_card_number);
		
		strength_node = CCNode::create();
		strength_node->setPosition(ccp(152,174));
		main_case->addChild(strength_node, kCardStrengthPopupZorder_content);
		
		CCSprite* n_strength = CCSprite::create("cardstrength_frame.png");
		CCSprite* n_card = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
		n_card->setScale(1.2f);
		n_card->setPosition(ccp(n_strength->getContentSize().width/2.f, n_strength->getContentSize().height/2.f));
		n_strength->addChild(n_card, -1);
		
		CCSprite* s_strength = CCSprite::create("cardstrength_frame.png");
		s_strength->setColor(ccGRAY);
		CCSprite* s_card = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
		s_card->setScale(1.2f);
		s_card->setColor(ccGRAY);
		s_card->setPosition(ccp(s_strength->getContentSize().width/2.f, s_strength->getContentSize().height/2.f));
		s_strength->addChild(s_card, -1);
		
		CCMenuItem* strength_item = CCMenuItemSprite::create(n_strength, s_strength, this, menu_selector(CardStrengthPopup::menuAction));
		strength_item->setTag(kCardStrengthPopupMenuTag_strengthCard);
		
		CCMenu* strength_menu = CCMenu::createWithItem(strength_item);
		strength_menu->setPosition(CCPointZero);
		strength_node->addChild(strength_menu);
		
		strength_menu->setTouchPriority(-184);
		
		CCPoint minus_content_half = ccp(-n_strength->getContentSize().width/2.f, -n_strength->getContentSize().height/2.f);
		
		CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
		mini_rank->setPosition(ccpAdd(minus_content_half, ccp(15,16)));
		strength_node->addChild(mini_rank);
		
		CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, strength_card_number))->getCString(), mySGD->getFont().c_str(), 8);
		t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
		mini_rank->addChild(t_rank);
		
		
//		CCLabelTTF* dur_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, strength_card_number),
//																  myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number))->getCString(), mySGD->getFont().c_str(), 10);
//		dur_label->setPosition(ccpAdd(minus_content_half, ccp(93,54)));
//		strength_node->addChild(dur_label);
//		
//		CCLabelTTF* up_dur_label = CCLabelTTF::create("+0", mySGD->getFont().c_str(), 14);
//		up_dur_label->setPosition(ccpAdd(dur_label->getPosition(), ccp(20,3)));
//		strength_node->addChild(up_dur_label);
		
		
		CCLabelTTF* pow_label = CCLabelTTF::create(CCString::createWithFormat("%d", int(NSDS_GI(kSDS_CI_int1_missile_power_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 10);
		pow_label->setAnchorPoint(ccp(1,0.5));
		pow_label->setPosition(ccpAdd(minus_content_half, ccp(-8,87)));
		strength_node->addChild(pow_label);
		
//		CCLabelTTF* up_pow_label = CCLabelTTF::create(CCString::createWithFormat("+%d", int(NSDS_GI(kSDS_CI_int1_missile_power_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*0.1f+1.f) - NSDS_GI(kSDS_CI_int1_missile_power_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 14);
//		up_pow_label->setPosition(ccpAdd(pow_label->getPosition(), ccp(20,3)));
//		strength_node->addChild(up_pow_label);
		
		
		CCLabelTTF* spd_label = CCLabelTTF::create(CCString::createWithFormat("%.1f", NSDS_GD(kSDS_CI_int1_missile_speed_d, strength_card_number))->getCString(), mySGD->getFont().c_str(), 10);
		spd_label->setAnchorPoint(ccp(1,0.5));
		spd_label->setPosition(ccpAdd(minus_content_half, ccp(-8,62)));
		strength_node->addChild(spd_label);
		
//		CCLabelTTF* up_spd_label = CCLabelTTF::create("+0", mySGD->getFont().c_str(), 14);
//		up_spd_label->setPosition(ccpAdd(spd_label->getPosition(), ccp(20,3)));
//		strength_node->addChild(up_spd_label);
		
		
		CCLabelTTF* dex_label = CCLabelTTF::create(CCString::createWithFormat("%d", int(NSDS_GI(kSDS_CI_int1_missile_dex_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 10);
		dex_label->setAnchorPoint(ccp(1,0.5));
		dex_label->setPosition(ccpAdd(minus_content_half, ccp(-8,74.5f)));
		strength_node->addChild(dex_label);
		
//		CCLabelTTF* up_dex_label = CCLabelTTF::create(CCString::createWithFormat("+%d", int(NSDS_GI(kSDS_CI_int1_missile_dex_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*0.1f+1.f) - NSDS_GI(kSDS_CI_int1_missile_dex_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 14);
//		up_dex_label->setPosition(ccpAdd(dex_label->getPosition(), ccp(20,3)));
//		strength_node->addChild(up_dex_label);
		
		CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))->getCString(), mySGD->getFont().c_str(), 8);
		t_card_level_label->setPosition(ccpAdd(minus_content_half, ccp(17,93)));
		strength_node->addChild(t_card_level_label, kCardStrengthPopupZorder_content);
		
		string missile_type_code = NSDS_GS(kSDS_CI_int1_missile_type_s, strength_card_number).c_str();
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
			SpinBasicMissile* missile_img = SpinBasicMissile::create(type_name.c_str(), level_number, ccpAdd(minus_content_half, ccp(-89,8)), false);
			missile_img->setScale(0.75f);
			strength_node->addChild(missile_img, kCardStrengthPopupZorder_content);
		}
		else
		{
			SpinUpgradeMissile* missile_img = SpinUpgradeMissile::create(type_name.c_str(), level_number, ccpAdd(minus_content_half, ccp(-89,8)), false);
			missile_img->setScale(0.75f);
			strength_node->addChild(missile_img, kCardStrengthPopupZorder_content);
		}
	}
}

void CardStrengthPopup::setOfferingNode(int t_card_number)
{
	if(offering_node)
	{
		offering_node->removeFromParent();
		offering_node = NULL;
	}
	
	if(strength_probability)
	{
		strength_probability->removeFromParent();
		strength_probability = NULL;
	}
	
	offering_card_number = t_card_number;
	
	if(offering_card_number == 0)
	{
		offering_node = CCSprite::create("cardstrength_frame.png");
		offering_node->setPosition(ccp(246,174));
		main_case->addChild(offering_node, kCardStrengthPopupZorder_content);
		
		if(strength_card_number != 0)
		{
			CCSprite* tip = CCSprite::create("cardstrength_tip_offering.png");
			tip->setPosition(ccp(offering_node->getContentSize().width/2.f, offering_node->getContentSize().height/2.f-5));
			offering_node->addChild(tip);
		}
		
		offering_idx = -1;
	}
	else
	{
		int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, offering_card_number);
		int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, offering_card_number);
		
		offering_node = CCNode::create();
		offering_node->setPosition(ccp(246,174));
		main_case->addChild(offering_node, kCardStrengthPopupZorder_content);
		
		CCSprite* n_offering = CCSprite::create("cardstrength_frame.png");
		CCSprite* n_card = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
		n_card->setScale(1.2f);
		n_card->setPosition(ccp(n_offering->getContentSize().width/2.f, n_offering->getContentSize().height/2.f));
		n_offering->addChild(n_card, -1);
		
		CCSprite* s_offering = CCSprite::create("cardstrength_frame.png");
		s_offering->setColor(ccGRAY);
		CCSprite* s_card = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", card_stage, card_grade)->getCString()));
		s_card->setScale(1.2f);
		s_card->setColor(ccGRAY);
		s_card->setPosition(ccp(s_offering->getContentSize().width/2.f, s_offering->getContentSize().height/2.f));
		s_offering->addChild(s_card, -1);
		
		CCMenuItem* offering_item = CCMenuItemSprite::create(n_offering, s_offering, this, menu_selector(CardStrengthPopup::menuAction));
		offering_item->setTag(kCardStrengthPopupMenuTag_offeringCard);
		
		CCMenu* offering_menu = CCMenu::createWithItem(offering_item);
		offering_menu->setPosition(CCPointZero);
		offering_node->addChild(offering_menu);
		
		offering_menu->setTouchPriority(-184);
		
		CCPoint minus_content_half = ccp(-n_offering->getContentSize().width/2.f, -n_offering->getContentSize().height/2.f);
		
		CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
		mini_rank->setPosition(ccpAdd(minus_content_half, ccp(15,16)));
		offering_node->addChild(mini_rank);
		
		CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, offering_card_number))->getCString(), mySGD->getFont().c_str(), 8);
		t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
		mini_rank->addChild(t_rank);
		
		
//		CCLabelTTF* dur_label = CCLabelTTF::create(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, offering_card_number),
//																			  myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, offering_card_number))->getCString(), mySGD->getFont().c_str(), 10);
//		dur_label->setPosition(ccpAdd(minus_content_half, ccp(93,54)));
//		offering_node->addChild(dur_label);
//		
//		CCLabelTTF* up_dur_label = CCLabelTTF::create("+0", mySGD->getFont().c_str(), 14);
//		up_dur_label->setPosition(ccpAdd(dur_label->getPosition(), ccp(20,3)));
//		offering_node->addChild(up_dur_label);
		
		
		CCLabelTTF* pow_label = CCLabelTTF::create(CCString::createWithFormat("%d", int(NSDS_GI(kSDS_CI_int1_missile_power_i, offering_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 10);
		pow_label->setAnchorPoint(ccp(1,0.5));
		pow_label->setPosition(ccpAdd(minus_content_half, ccp(157,87)));
		offering_node->addChild(pow_label);
		
//		CCLabelTTF* up_pow_label = CCLabelTTF::create(CCString::createWithFormat("+%d", int(NSDS_GI(kSDS_CI_int1_missile_power_i, offering_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))*0.1f+1.f) - NSDS_GI(kSDS_CI_int1_missile_power_i, offering_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 14);
//		up_pow_label->setPosition(ccpAdd(pow_label->getPosition(), ccp(20,3)));
//		offering_node->addChild(up_pow_label);
		
		
		CCLabelTTF* spd_label = CCLabelTTF::create(CCString::createWithFormat("%.1f", NSDS_GD(kSDS_CI_int1_missile_speed_d, offering_card_number))->getCString(), mySGD->getFont().c_str(), 10);
		spd_label->setAnchorPoint(ccp(1,0.5));
		spd_label->setPosition(ccpAdd(minus_content_half, ccp(157,62)));
		offering_node->addChild(spd_label);
		
//		CCLabelTTF* up_spd_label = CCLabelTTF::create("+0", mySGD->getFont().c_str(), 14);
//		up_spd_label->setPosition(ccpAdd(spd_label->getPosition(), ccp(20,3)));
//		offering_node->addChild(up_spd_label);
		
		
		CCLabelTTF* dex_label = CCLabelTTF::create(CCString::createWithFormat("%d", int(NSDS_GI(kSDS_CI_int1_missile_dex_i, offering_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 10);
		dex_label->setAnchorPoint(ccp(1,0.5));
		dex_label->setPosition(ccpAdd(minus_content_half, ccp(157,74.5f)));
		offering_node->addChild(dex_label);
		
//		CCLabelTTF* up_dex_label = CCLabelTTF::create(CCString::createWithFormat("+%d", int(NSDS_GI(kSDS_CI_int1_missile_dex_i, offering_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))*0.1f+1.f) - NSDS_GI(kSDS_CI_int1_missile_dex_i, offering_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 14);
//		up_dex_label->setPosition(ccpAdd(dex_label->getPosition(), ccp(20,3)));
//		offering_node->addChild(up_dex_label);
		
		CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))->getCString(), mySGD->getFont().c_str(), 8);
		t_card_level_label->setPosition(ccpAdd(minus_content_half, ccp(17,93)));
		offering_node->addChild(t_card_level_label, kCardStrengthPopupZorder_content);
		
		string missile_type_code = NSDS_GS(kSDS_CI_int1_missile_type_s, offering_card_number).c_str();
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
			SpinBasicMissile* missile_img = SpinBasicMissile::create(type_name.c_str(), level_number, ccpAdd(minus_content_half, ccp(130,8)), false);
			missile_img->setScale(0.75f);
			offering_node->addChild(missile_img, kCardStrengthPopupZorder_content);
		}
		else
		{
			SpinUpgradeMissile* missile_img = SpinUpgradeMissile::create(type_name.c_str(), level_number, ccpAdd(minus_content_half, ccp(130,8)), false);
			missile_img->setScale(0.75f);
			offering_node->addChild(missile_img, kCardStrengthPopupZorder_content);
		}
		
		strength_probability = CCSprite::create("cardsetting_probability.png");
		strength_probability->setPosition(ccp(198.5,150));
		main_case->addChild(strength_probability, kCardStrengthPopupZorder_highContent);
		
		float strength_rate = ((NSDS_GI(kSDS_CI_int1_rank_i, offering_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, offering_card_number))/((NSDS_GI(kSDS_CI_int1_rank_i, strength_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number));
		
		CCLabelTTF* probability_label = CCLabelTTF::create(CCString::createWithFormat("%.1f", strength_rate*100.f)->getCString(), mySGD->getFont().c_str(), 10);
		probability_label->setAnchorPoint(ccp(1.f,0.5f));
		probability_label->setPosition(ccp(strength_probability->getContentSize().width-17, strength_probability->getContentSize().height/2.f-2));
		strength_probability->addChild(probability_label);
	}
}

void CardStrengthPopup::setOfferingList()
{
	int loop_length = mySGD->getHasGottenCardsSize();
	for(int i=0;i<loop_length;i++)
	{
		int card_number = mySGD->getHasGottenCardsDataCardNumber(i);
		if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, card_number) > 0)
		{
			CardSortInfo t_info = mySGD->getHasGottenCardData(i);
			offering_list.push_back(t_info);
		}
	}
	
	alignOfferingList(kCardStrengthAlign_takeReverse);
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
	offering_card->setScale(0.75f);
	offering_card->setPosition(ccp(30, 39));
	cell->addChild(offering_card);
	
	CCSprite* card_case;
	if(offering_list[idx].card_number == strength_card_number)
	{
		if(strength_idx == -1)
			strength_idx = idx;
		card_case = CCSprite::create("cardstrength_list_strength.png");
		card_case->setPosition(ccp(30, 40));
		cell->addChild(card_case);
	}
	else if(offering_list[idx].card_number == offering_card_number)
	{
		if(offering_idx == -1)
			offering_idx = idx;
		card_case = CCSprite::create("cardstrength_list_offering.png");
		card_case->setPosition(ccp(30, 40));
		cell->addChild(card_case);
	}
	else
	{
		card_case = CCSprite::create("cardstrength_list_normal.png");
		card_case->setPosition(ccp(30, 40));
		cell->addChild(card_case);
	}
	
	CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
	mini_rank->setPosition(ccp(15,15));
	card_case->addChild(mini_rank);
	
	CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, offering_list[idx].card_number))->getCString(), mySGD->getFont().c_str(), 8);
	t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
	mini_rank->addChild(t_rank);
	
	CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_list[idx].card_number))->getCString(), mySGD->getFont().c_str(), 8);
	t_card_level_label->setPosition(ccp(17,63));
	cell->addChild(t_card_level_label);
	
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
	
	int clicked_card_number = offering_list[cell->getIdx()].card_number;
	if(clicked_card_number == offering_card_number)
	{
		// 재료 카드 해제
		is_menu_enable = false;
		int keep_idx = offering_idx;
		setOfferingNode(0);
		if(keep_idx >= 0)
			table->updateCellAtIndex(keep_idx);
		is_menu_enable = true;
	}
	else if(clicked_card_number == strength_card_number)
	{
		if(offering_card_number != 0)
		{
			// 강화 카드 해제 불가
		}
		else
		{
			is_menu_enable = false;
			int keep_idx = strength_idx;
			setStrengthNode(0);
			if(keep_idx >= 0)
				table->updateCellAtIndex(keep_idx);
			
			setTableTab();
			is_menu_enable = true;
		}
	}
	else
	{
		if(strength_card_number == 0)
		{
			strength_idx = cell->getIdx();
			setStrengthNode(clicked_card_number);
			setOfferingNode(0);
			setTableTab();
			
			table->updateCellAtIndex(cell->getIdx());
		}
		else if(offering_card_number == 0)
		{
			offering_idx = cell->getIdx();
			setOfferingNode(clicked_card_number);
			
			table->updateCellAtIndex(cell->getIdx());
		}
		else
		{
			int keep_idx = offering_idx;
			offering_idx = cell->getIdx();
			setOfferingNode(clicked_card_number);
			
			if(keep_idx >= 0)
				table->updateCellAtIndex(keep_idx);
			table->updateCellAtIndex(offering_idx);
		}
		
		is_menu_enable = true;
	}
}
CCSize CardStrengthPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(60, 78);
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
	TutorialFlowStep recent_step = (TutorialFlowStep)myDSH->getIntegerForKey(kDSH_Key_tutorial_flowStep);
	
	if(recent_step == kTutorialFlowStep_upgradeScript)
	{
		TutorialFlowStepLayer* t_tutorial = TutorialFlowStepLayer::create();
		t_tutorial->initStep(kTutorialFlowStep_upgradeScript);
		addChild(t_tutorial, kCardStrengthPopupZorder_popup);
	}
	
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
	
	if(t_tag == kCardStrengthPopupMenuTag_close)						return_value = ccp(452,259);
	else if(t_tag == kCardStrengthPopupMenuTag_highStrength)			return_value = ccp(415,204);
	else if(t_tag == kCardStrengthPopupMenuTag_normalStrength)			return_value = ccp(415,150);
	else if(t_tag == kCardStrengthPopupMenuTag_alignRank)				return_value = ccp(416,78);
	else if(t_tag == kCardStrengthPopupMenuTag_alignTake)				return_value = ccp(416,39);
	
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
		if(card_strength_before == kCardStrengthBefore_cardSetting)
		{
			CardSettingPopup* t_popup = CardSettingPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
		}
		else if(card_strength_before == kCardStrengthBefore_cardChange)
		{
			CardChangePopup* t_popup = CardChangePopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kStartSettingZorder_popup);
		}
		else if(card_strength_before == kCardStrengthBefore_diary)
		{
			if(strength_card_number > 0)
				mySGD->selected_collectionbook = strength_card_number;
			else if(myDSH->getIntegerForKey(kDSH_Key_selectedCard) > 0)
				mySGD->selected_collectionbook = myDSH->getIntegerForKey(kDSH_Key_selectedCard);
			else
				mySGD->selected_collectionbook = offering_list[0].card_number;
			
			CollectionBookPopup* t_popup = CollectionBookPopup::create();
			t_popup->setHideFinalAction(target_final, delegate_final);
			getParent()->addChild(t_popup, kMainFlowZorder_popup);
		}
		
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
		// 재료 카드 해제
		int keep_idx = offering_idx;
		setOfferingNode(0);
		if(keep_idx >= 0)
			offering_table->updateCellAtIndex(keep_idx);
		
		is_menu_enable = true;
	}
	else if(tag == kCardStrengthPopupMenuTag_strengthCard)
	{
		if(offering_card_number != 0)
		{
			// 강화 카드 해제 불가
			is_menu_enable = true;
		}
		else
		{
			int keep_idx = strength_idx;
			setStrengthNode(0);
			if(keep_idx >= 0)
				offering_table->updateCellAtIndex(keep_idx);
			setTableTab();
			is_menu_enable = true;
		}
	}
	else if(tag == kCardStrengthPopupMenuTag_highStrength)
	{
		if(mySGD->getStar() >= mySGD->getCardUpgradeRubyFee() && offering_card_number > 0)
		{
			mySGD->setStar(mySGD->getStar() - mySGD->getCardUpgradeRubyFee());
			myDSH->saveUserData({kSaveUserData_Key_star}, nullptr);
			
			float strength_rate = ((NSDS_GI(kSDS_CI_int1_rank_i, offering_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, offering_card_number))/((NSDS_GI(kSDS_CI_int1_rank_i, strength_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number));
			CCLog("strength_rate : %.3f", strength_rate);
			
			myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, offering_card_number, 0);
			myDSH->setIntegerForKey(kDSH_Key_cardMaxDurability_int1, offering_card_number, NSDS_GI(kSDS_CI_int1_durability_i, offering_card_number));
			myDSH->setStringForKey(kDSH_Key_cardPassive_int1, offering_card_number, NSDS_GS(kSDS_CI_int1_passive_s, offering_card_number));
			
			// 결과 창 생성
			result_popup = TouchSuctionLayer::create(-210);
			result_popup->target_touch_began = result_popup;
			result_popup->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			CCSprite* rp_gray = CCSprite::create("back_gray.png");
			rp_gray->setPosition(ccp(240,160));
			rp_gray->setScaleX(screen_scale_x);
			rp_gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
			result_popup->addChild(rp_gray);
			
			int strength_card_stage = NSDS_GI(kSDS_CI_int1_stage_i, strength_card_number);
			int strength_card_grade = NSDS_GI(kSDS_CI_int1_grade_i, strength_card_number);
			
			CCSprite* strength_card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", strength_card_stage, strength_card_grade)->getCString());
			strength_card_img->setScale(0.43f);
			strength_card_img->setPosition(ccp(180,170));
			result_popup->addChild(strength_card_img);
			
			if(strength_card_grade == 3 && mySD->isAnimationStage(strength_card_stage))
			{
				CCSize ani_size = mySD->getAnimationCutSize(strength_card_stage);
				CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", strength_card_stage, strength_card_grade)->getCString(),
													  CCRectMake(0, 0, ani_size.width, ani_size.height));
				t_ani->setPosition(mySD->getAnimationPosition(strength_card_stage));
				strength_card_img->addChild(t_ani);
			}
			
			int offering_card_stage = NSDS_GI(kSDS_CI_int1_stage_i, offering_card_number);
			int offering_card_grade = NSDS_GI(kSDS_CI_int1_grade_i, offering_card_number);
			
			bool is_piece_remove = true;
			
			for(int i=1;i<=3 && is_piece_remove;i++)
			{
				if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(offering_card_stage, kSDS_SI_level_int1_card_i, i)) > 0)
					is_piece_remove = false;
			}
			
			string comment_string;
			if(is_piece_remove)
				comment_string = "재료카드와 퍼즐조각이 소진됩니다.";
			else
				comment_string = "재료카드가 소진됩니다.";
			
			CCLabelTTF* comment_label = CCLabelTTF::create(comment_string.c_str(), mySGD->getFont().c_str(), 15);
			comment_label->setPosition(ccp(180,50));
			result_popup->addChild(comment_label);
			
			CCSprite* offering_card_img = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", offering_card_stage, offering_card_grade)->getCString()));
			offering_card_img->setScale(0.73f);
			offering_card_img->setPosition(ccp(340, 70));
			result_popup->addChild(offering_card_img);
			
			CCSprite* card_case = CCSprite::create("cardstrength_list_normal.png");
			card_case->setPosition(ccp(340, 70));
			result_popup->addChild(card_case);
			
			CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
			mini_rank->setPosition(ccp(17,17));
			card_case->addChild(mini_rank);
			
			CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, offering_card_number))->getCString(), mySGD->getFont().c_str(), 8);
			t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
			mini_rank->addChild(t_rank);
			
			CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))->getCString(),
																mySGD->getFont().c_str(), 10);
			t_card_level_label->setPosition(ccp(19,75));
			card_case->addChild(t_card_level_label);
			
			string t_WorH;
			if(NSDS_GI(NSDS_GI(offering_card_stage, kSDS_SI_puzzle_i), kSDS_PZ_stage_int1_pieceNo_i) % 2 == 0)
				t_WorH = "w";
			else
				t_WorH = "h";
			
			CCPoint offering_piece_point = ccp(420,70);
			
			if(is_piece_remove)
			{
				string t_WorH;
				if(NSDS_GI(NSDS_GI(offering_card_stage, kSDS_SI_puzzle_i), kSDS_PZ_stage_int1_pieceNo_i) % 2 == 0)
					t_WorH = "w";
				else
					t_WorH = "h";
				
				CCPoint offering_piece_point = ccp(420,70);
				
				int offering_card_puzzle_number = NSDS_GI(offering_card_stage, kSDS_SI_puzzle_i);
				if(offering_card_puzzle_number != 0)
				{
					GraySprite* piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", offering_card_puzzle_number,
																												 NSDS_GI(offering_card_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i,
																														 offering_card_stage)-1)->getCString()));
					piece->setPosition(offering_piece_point);
					result_popup->addChild(piece);
					
					CCSprite* embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", t_WorH.c_str())->getCString());
					embo->setPosition(ccp(piece->getContentSize().width/2.f, piece->getContentSize().height/2.f));
					piece->addChild(embo);
				}
			}
			
			
			
			for(auto iter = offering_list.begin();iter != offering_list.end();iter++)
			{
				if((*iter).card_number == offering_card_number)
				{
					offering_list.erase(iter);
					break;
				}
			}
			
			random_device rd;
			default_random_engine e1(rd());
			uniform_real_distribution<float> uniform_dist(0.f, 1.f);
			
			float result_value = uniform_dist(e1);
			CCLog("result value : %.3f", result_value);
			
			if(result_value <= strength_rate)
			{
				CCLog("success");
				
				CCLabelTTF* result_label = CCLabelTTF::create("강화 성공!", mySGD->getFont().c_str(), 20);
				result_label->setPosition(ccp(240,290));
				result_popup->addChild(result_label);
				
				float success_type_rate = uniform_dist(e1);
				if(success_type_rate <= 0.5f)
				{
					CCLabelTTF* power_up_label = CCLabelTTF::create(CCString::createWithFormat("POW +%d", int(NSDS_GI(kSDS_CI_int1_missile_power_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*0.1f+1.f) - NSDS_GI(kSDS_CI_int1_missile_power_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 14);
					power_up_label->setAnchorPoint(ccp(0,0.5));
					power_up_label->setPosition(ccp(270,180));
					result_popup->addChild(power_up_label);
					
					CCLabelTTF* dex_up_label = CCLabelTTF::create(CCString::createWithFormat("DEX +%d", int(NSDS_GI(kSDS_CI_int1_missile_dex_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*0.1f+1.f) - NSDS_GI(kSDS_CI_int1_missile_dex_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 14);
					dex_up_label->setAnchorPoint(ccp(0,0.5));
					dex_up_label->setPosition(ccp(270,150));
					result_popup->addChild(dex_up_label);
					
					
					
					uniform_int_distribution<int> uniform_dist_int(1, 3);
					int level_up_value = uniform_dist_int(e1);
					CCLog("level up value : %d", level_up_value);
					myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number, myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)+1);
				}
				else if(success_type_rate <= 0.7f)
				{
					CCLabelTTF* dur_up_label = CCLabelTTF::create("내구도 +1", mySGD->getFont().c_str(), 14);
					dur_up_label->setAnchorPoint(ccp(0,0.5));
					dur_up_label->setPosition(ccp(270,165));
					result_popup->addChild(dur_up_label);
					
					CCLog("max durability up");
					myDSH->setIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number, myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number)+1);
					myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, strength_card_number, myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, strength_card_number)+1);
				}
				else
				{
					string passive_string = myDSH->getStringForKey(kDSH_Key_cardPassive_int1, strength_card_number).c_str();
					
					if(passive_string == "")
					{
						CCLog("passive up");
						
						Json::Value data;
						
						data["operator"] = "*(1-x)";
						uniform_int_distribution<int> uniform_dist_int(1, 5);
						int passive_up_type = uniform_dist_int(e1);
						float passive_up_value = uniform_dist(e1);
						
						if(passive_up_value <= 0.3f)
							passive_up_value = 0.1f;
						else if(passive_up_value <= 0.55f)
							passive_up_value = 0.2f;
						else if(passive_up_value <= 0.75f)
							passive_up_value = 0.3f;
						else if(passive_up_value <= 0.9f)
							passive_up_value = 0.4f;
						else
							passive_up_value = 0.5f;
						
						if(passive_up_type == 1)
							data["ai"] = double(passive_up_value);
						else if(passive_up_type == 2)
							data["speed"] = double(passive_up_value);
						else if(passive_up_type == 3)
							data["scale"] = double(passive_up_value);
						else if(passive_up_type == 4)
							data["hp"] = double(passive_up_value);
						else
							data["agi"] = double(passive_up_value);
						
						Json::FastWriter data_writer;
						myDSH->setStringForKey(kDSH_Key_cardPassive_int1, strength_card_number, data_writer.write(data));
						
						CCLabelTTF* passive_change_label = CCLabelTTF::create(CCString::createWithFormat("패시브 변경\n%s", data_writer.write(data).c_str())->getCString(), mySGD->getFont().c_str(), 14);
						passive_change_label->setAnchorPoint(ccp(0.5,0.5));
						passive_change_label->setPosition(ccp(340,165));
						result_popup->addChild(passive_change_label);
					}
					else
					{
						Json::Reader reader;
						Json::Value passive_data;
						reader.parse(passive_string, passive_data);
						
						string operator_string = passive_data["operator"].asString();
						
						vector<string> empty_data_list;
						if(passive_data["ai"].asDouble() == 0)
							empty_data_list.push_back("ai");
						if(passive_data["speed"].asDouble() == 0)
							empty_data_list.push_back("speed");
						if(passive_data["scale"].asDouble() == 0)
							empty_data_list.push_back("scale");
						if(passive_data["hp"].asDouble() == 0)
							empty_data_list.push_back("hp");
						if(passive_data["agi"].asDouble() == 0)
							empty_data_list.push_back("agi");
						
						if(passive_data["operator"].asString() == "-" || empty_data_list.empty())
						{
							CCLabelTTF* dur_up_label = CCLabelTTF::create("내구도 +1", mySGD->getFont().c_str(), 14);
							dur_up_label->setAnchorPoint(ccp(0,0.5));
							dur_up_label->setPosition(ccp(270,165));
							result_popup->addChild(dur_up_label);
							
							CCLog("max durability up");
							myDSH->setIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number, myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number)+1);
							myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, strength_card_number, myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, strength_card_number)+1);
						}
						else
						{
							CCLog("passive up");
							
							Json::Value data;
							
							data["operator"] = "*(1-x)";
							uniform_int_distribution<int> uniform_dist_int(0, empty_data_list.size()-1);
							int passive_up_type = uniform_dist_int(e1);
							float passive_up_value = uniform_dist(e1);
							
							if(passive_up_value <= 0.3f)
								passive_up_value = 0.1f;
							else if(passive_up_value <= 0.55f)
								passive_up_value = 0.2f;
							else if(passive_up_value <= 0.75f)
								passive_up_value = 0.3f;
							else if(passive_up_value <= 0.9f)
								passive_up_value = 0.4f;
							else
								passive_up_value = 0.5f;
							
							data[empty_data_list[passive_up_type].c_str()] = double(passive_up_value);
							
							Json::FastWriter data_writer;
							myDSH->setStringForKey(kDSH_Key_cardPassive_int1, strength_card_number, data_writer.write(data));
							CCLabelTTF* passive_change_label = CCLabelTTF::create(CCString::createWithFormat("패시브 변경\n%s", data_writer.write(data).c_str())->getCString(), mySGD->getFont().c_str(), 14);
							passive_change_label->setAnchorPoint(ccp(0.5,0.5));
							passive_change_label->setPosition(ccp(340,165));
							result_popup->addChild(passive_change_label);
						}
					}
				}
			}
			else
			{
				CCLog("fail");
				CCLabelTTF* result_label = CCLabelTTF::create("강화 실패!", mySGD->getFont().c_str(), 20);
				result_label->setPosition(ccp(240,290));
				result_popup->addChild(result_label);
			}
			
			// 결과 창 카드 케이스 생성
			CardCase* t_case = CardCase::create(strength_card_stage, strength_card_grade);
			t_case->setPosition(CCPointZero);
			strength_card_img->addChild(t_case);
			
			addChild(result_popup, kCardStrengthPopupZorder_popup);
			
			loading_img = CCSprite::create("back_gray.png");
			loading_img->setPosition(ccp(240,160));
			loading_img->setScaleX(screen_scale_x);
			loading_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
			addChild(loading_img, kCardStrengthPopupZorder_popup);
			
			loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 30);
			loading_label->setPosition(ccp(240,160));
			addChild(loading_label, kCardStrengthPopupZorder_popup);
			
			
			myDSH->saveUserData({kSaveUserData_Key_cardsInfo}, json_selector(this, CardStrengthPopup::resultStrength));
			
			setOfferingNode(0);
		}
		else
		{
			if(offering_card_number <= 0)
				addChild(ASPopupView::getCommonNoti(-210, "재료 카드를 선택해주세요."), kCardStrengthPopupZorder_popup);
			else
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kCardStrengthPopupZorder_popup);
			is_menu_enable = true;
		}
	}
	else if(tag == kCardStrengthPopupMenuTag_normalStrength)
	{
		if(mySGD->getGold() >= mySGD->getCardUpgradeGoldFee() && offering_card_number > 0)
		{
			mySGD->setGold(mySGD->getGold() - mySGD->getCardUpgradeGoldFee());
			myDSH->saveUserData({kSaveUserData_Key_gold}, nullptr);
			
			float strength_rate = ((NSDS_GI(kSDS_CI_int1_rank_i, offering_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, offering_card_number))/((NSDS_GI(kSDS_CI_int1_rank_i, strength_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number));
			CCLog("strength_rate : %.3f", strength_rate);
			
			myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, offering_card_number, 0);
			myDSH->setIntegerForKey(kDSH_Key_cardMaxDurability_int1, offering_card_number, NSDS_GI(kSDS_CI_int1_durability_i, offering_card_number));
			myDSH->setStringForKey(kDSH_Key_cardPassive_int1, offering_card_number, NSDS_GS(kSDS_CI_int1_passive_s, offering_card_number));
			
			
			// 결과 창 생성
			result_popup = TouchSuctionLayer::create(-210);
			result_popup->target_touch_began = result_popup;
			result_popup->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			CCSprite* rp_gray = CCSprite::create("back_gray.png");
			rp_gray->setPosition(ccp(240,160));
			rp_gray->setScaleX(screen_scale_x);
			rp_gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
			result_popup->addChild(rp_gray);
			
			int strength_card_stage = NSDS_GI(kSDS_CI_int1_stage_i, strength_card_number);
			int strength_card_grade = NSDS_GI(kSDS_CI_int1_grade_i, strength_card_number);
			
			CCSprite* strength_card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", strength_card_stage, strength_card_grade)->getCString());
			strength_card_img->setScale(0.43f);
			strength_card_img->setPosition(ccp(180,170));
			result_popup->addChild(strength_card_img);
			
			if(strength_card_grade == 3 && mySD->isAnimationStage(strength_card_stage))
			{
				CCSize ani_size = mySD->getAnimationCutSize(strength_card_stage);
				CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", strength_card_stage, strength_card_grade)->getCString(),
													  CCRectMake(0, 0, ani_size.width, ani_size.height));
				t_ani->setPosition(mySD->getAnimationPosition(strength_card_stage));
				strength_card_img->addChild(t_ani);
			}
			
			int offering_card_stage = NSDS_GI(kSDS_CI_int1_stage_i, offering_card_number);
			int offering_card_grade = NSDS_GI(kSDS_CI_int1_grade_i, offering_card_number);
			
			bool is_piece_remove = true;
			
			for(int i=1;i<=3 && is_piece_remove;i++)
			{
				if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, NSDS_GI(offering_card_stage, kSDS_SI_level_int1_card_i, i)) > 0)
					is_piece_remove = false;
			}
			
			string comment_string;
			if(is_piece_remove)
				comment_string = "재료카드와 퍼즐조각이 소진됩니다.";
			else
				comment_string = "재료카드가 소진됩니다.";
			
			CCLabelTTF* comment_label = CCLabelTTF::create(comment_string.c_str(), mySGD->getFont().c_str(), 15);
			comment_label->setPosition(ccp(180,50));
			result_popup->addChild(comment_label);
			
			
			
			CCSprite* offering_card_img = CCSprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("stage%d_level%d_thumbnail.png", offering_card_stage, offering_card_grade)->getCString()));
			offering_card_img->setScale(0.8f);
			offering_card_img->setPosition(ccp(340, 70));
			result_popup->addChild(offering_card_img);
			
			CCSprite* card_case = CCSprite::create("cardstrength_list_normal.png");
			card_case->setPosition(ccp(340, 70));
			result_popup->addChild(card_case);
			
			CCSprite* mini_rank = CCSprite::create("cardsetting_mini_rank.png");
			mini_rank->setPosition(ccp(15,15));
			card_case->addChild(mini_rank);
			
			CCLabelTTF* t_rank = CCLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_rank_i, offering_card_number))->getCString(), mySGD->getFont().c_str(), 8);
			t_rank->setPosition(ccp(mini_rank->getContentSize().width/2.f, mini_rank->getContentSize().height/2.f-1));
			mini_rank->addChild(t_rank);
			
			CCLabelTTF* t_card_level_label = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))->getCString(),
																mySGD->getFont().c_str(), 10);
			t_card_level_label->setPosition(ccp(19,65));
			card_case->addChild(t_card_level_label);
			
			
			if(is_piece_remove)
			{
				string t_WorH;
				if(NSDS_GI(NSDS_GI(offering_card_stage, kSDS_SI_puzzle_i), kSDS_PZ_stage_int1_pieceNo_i) % 2 == 0)
					t_WorH = "w";
				else
					t_WorH = "h";
				
				CCPoint offering_piece_point = ccp(420,70);
				
				int offering_card_puzzle_number = NSDS_GI(offering_card_stage, kSDS_SI_puzzle_i);
				if(offering_card_puzzle_number != 0)
				{
					GraySprite* piece = GraySprite::createWithTexture(mySIL->addImage(CCString::createWithFormat("puzzle%d_original_piece%d.png", offering_card_puzzle_number,
																												   NSDS_GI(offering_card_puzzle_number, kSDS_PZ_stage_int1_pieceNo_i,
																														   offering_card_stage)-1)->getCString()));
					piece->setPosition(offering_piece_point);
					result_popup->addChild(piece);
					
					CCSprite* embo = CCSprite::create(CCString::createWithFormat("piece_embo_%s.png", t_WorH.c_str())->getCString());
					embo->setPosition(ccp(piece->getContentSize().width/2.f, piece->getContentSize().height/2.f));
					piece->addChild(embo);
				}
			}
			
			for(auto iter = offering_list.begin();iter != offering_list.end();iter++)
			{
				if((*iter).card_number == offering_card_number)
				{
					offering_list.erase(iter);
					break;
				}
			}
			
			random_device rd;
			default_random_engine e1(rd());
			uniform_real_distribution<float> uniform_dist(0.f, 1.f);
			
			float result_value = uniform_dist(e1);
			CCLog("result value : %.3f", result_value);
			
			if(result_value <= strength_rate)
			{
				CCLabelTTF* result_label = CCLabelTTF::create("강화 성공!", mySGD->getFont().c_str(), 20);
				result_label->setPosition(ccp(240,290));
				result_popup->addChild(result_label);
				
				CCLabelTTF* power_up_label = CCLabelTTF::create(CCString::createWithFormat("POW +%d", int(NSDS_GI(kSDS_CI_int1_missile_power_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*0.1f+1.f) - NSDS_GI(kSDS_CI_int1_missile_power_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 14);
				power_up_label->setAnchorPoint(ccp(0,0.5));
				power_up_label->setPosition(ccp(270,180));
				result_popup->addChild(power_up_label);
				
				CCLabelTTF* dex_up_label = CCLabelTTF::create(CCString::createWithFormat("DEX +%d", int(NSDS_GI(kSDS_CI_int1_missile_dex_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*0.1f+1.f) - NSDS_GI(kSDS_CI_int1_missile_dex_i, strength_card_number)*((myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)-1)*0.1f+1.f)))->getCString(), mySGD->getFont().c_str(), 14);
				dex_up_label->setAnchorPoint(ccp(0,0.5));
				dex_up_label->setPosition(ccp(270,150));
				result_popup->addChild(dex_up_label);
				
				CCLog("success");
				myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number, myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number)+1);
			}
			else
			{
				CCLabelTTF* result_label = CCLabelTTF::create("강화 실패!", mySGD->getFont().c_str(), 20);
				result_label->setPosition(ccp(240,290));
				result_popup->addChild(result_label);
				
				CCLog("fail");
			}
			
			// 결과 창 카드 케이스 생성
			CardCase* t_case = CardCase::create(strength_card_stage, strength_card_grade);
			t_case->setPosition(CCPointZero);
			strength_card_img->addChild(t_case);
			
			result_popup->setVisible(false);
			addChild(result_popup, kCardStrengthPopupZorder_popup);
			
			loading_img = CCSprite::create("back_gray.png");
			loading_img->setPosition(ccp(240,160));
			loading_img->setScaleX(screen_scale_x);
			loading_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
			addChild(loading_img, kCardStrengthPopupZorder_popup);
			
			loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 30);
			loading_label->setPosition(ccp(240,160));
			addChild(loading_label, kCardStrengthPopupZorder_popup);
			
			
			myDSH->saveUserData({kSaveUserData_Key_cardsInfo}, json_selector(this, CardStrengthPopup::resultStrength));
			
			setOfferingNode(0);
		}
		else
		{
			if(offering_card_number <= 0)
				addChild(ASPopupView::getCommonNoti(-210, "재료 카드를 선택해주세요."), kCardStrengthPopupZorder_popup);
			else
				addChild(ASPopupView::getCommonNoti(-210, "골드가 부족합니다."), kCardStrengthPopupZorder_popup);
			is_menu_enable = true;
		}
	}
}

void CardStrengthPopup::replayAction(CCObject* sender)
{
	replay_menu->removeFromParent();
	myDSH->saveUserData({kSaveUserData_Key_cardsInfo}, json_selector(this, CardStrengthPopup::resultStrength));
}

void CardStrengthPopup::resultStrength(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		loading_img->removeFromParent();
		loading_label->removeFromParent();
		
		setStrengthNode(strength_card_number);
		offering_table->reloadData();
		
		result_popup->setVisible(true);
		result_popup->setTouchEnabled(true);
		
		is_menu_enable = true;
	}
	else
	{
		CCSprite* n_replay = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_replay = CCSprite::create("item_buy_popup_close.png");
		s_replay->setColor(ccGRAY);
		
		CCMenuItem* replay_item = CCMenuItemSprite::create(n_replay, s_replay, this, menu_selector(CardStrengthPopup::replayAction));
		
		replay_menu = CCMenu::createWithItem(replay_item);
		replay_menu->setPosition(ccp(240,100));
		addChild(replay_menu, kCardStrengthPopupZorder_popup);
		replay_menu->setTouchPriority(-181);
	}
}

bool CardStrengthPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLog("touch swallow!!");
	
	return true;
}
void CardStrengthPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardStrengthPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardStrengthPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void CardStrengthPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -179, true);
}
