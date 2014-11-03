//
//  BeautyStoneStrength.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 5..
//
//

#include "BeautyStoneStrength.h"
#include "DataStorageHub.h"
#include <random>
#include "TouchSuctionLayer.h"
#include "ASPopupView.h"
#include "StageImgLoader.h"
#include "CommonButton.h"
#include "BeautyStoneSetting.h"
#include "ScrollMenu.h"
#include "BeautyStoneSynthesis.h"

enum BeautyStoneStrengthPopupZorder{
	kBeautyStoneStrengthPopupZorder_gray = 1,
	kBeautyStoneStrengthPopupZorder_back,
	kBeautyStoneStrengthPopupZorder_content,
	kBeautyStoneStrengthPopupZorder_popup
};

enum BeautyStoneStrengthPopupMenuTag{
	kBeautyStoneStrengthPopupMenuTag_close = 1,
	kBeautyStoneStrengthPopupMenuTag_strengthStone,
	kBeautyStoneStrengthPopupMenuTag_offeringStone,
	kBeautyStoneStrengthPopupMenuTag_highStrength,
	kBeautyStoneStrengthPopupMenuTag_normalStrength,
	kBeautyStoneStrengthPopupMenuTag_alignRank,
	kBeautyStoneStrengthPopupMenuTag_alignLevel,
	kBeautyStoneStrengthPopupMenuTag_tip,
	kBeautyStoneStrengthPopupMenuTag_synthesis,
	kBeautyStoneStrengthPopupMenuTag_setting
};

void BeautyStoneStrengthPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool BeautyStoneStrengthPopup::init()
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
	addChild(gray, kBeautyStoneStrengthPopupZorder_gray);
	
	main_case = CCSprite::create("stonestrength_back.png");
	main_case->setAnchorPoint(ccp(0.5,0.5));
	main_case->setPosition(ccp(240,160-450));
	addChild(main_case, kBeautyStoneStrengthPopupZorder_back);
	
	CCLabelTTF* title_label = CCLabelTTF::create("뷰티스톤 강화", mySGD->getFont().c_str(), 17);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setPosition(ccp(75,255));
	main_case->addChild(title_label, kBeautyStoneStrengthPopupZorder_content);
	
	CommonButton* synthesis_menu = CommonButton::create("뷰티스톤 합성", 13, CCSizeMake(110,40), CommonButtonYellow, -184);
	synthesis_menu->setTitleColor(ccBLACK);
	synthesis_menu->setPosition(ccp(260,256));
	main_case->addChild(synthesis_menu, kBeautyStoneStrengthPopupZorder_content);
	synthesis_menu->setFunction([=](CCObject* sender)
								{
									CCNode* t_node = CCNode::create();
									t_node->setTag(kBeautyStoneStrengthPopupMenuTag_synthesis);
									menuAction(t_node);
								});
	
	CommonButton* strength_menu = CommonButton::create("뷰티스톤 관리", 13, CCSizeMake(110,40), CommonButtonYellow, -184);
	strength_menu->setTitleColor(ccBLACK);
	strength_menu->setPosition(ccp(370,256));
	main_case->addChild(strength_menu, kBeautyStoneStrengthPopupZorder_content);
	strength_menu->setFunction([=](CCObject* sender)
							   {
								   CCNode* t_node = CCNode::create();
								   t_node->setTag(kBeautyStoneStrengthPopupMenuTag_setting);
								   menuAction(t_node);
							   });
	
	CommonButton* close_menu = CommonButton::createCloseButton(-184);
	close_menu->setPosition(ccp(452,256));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kBeautyStoneStrengthPopupMenuTag_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kBeautyStoneStrengthPopupZorder_content);
	
	
	
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
	
	
	CCMenuItem* high_strength_item = CCMenuItemSprite::create(n_high_strength, s_high_strength, this, menu_selector(BeautyStoneStrengthPopup::menuAction));
	high_strength_item->setTag(kBeautyStoneStrengthPopupMenuTag_highStrength);
	
	CCMenu* high_strength_menu = CCMenu::createWithItem(high_strength_item);
	high_strength_menu->setPosition(ccp(415,201));
	main_case->addChild(high_strength_menu, kBeautyStoneStrengthPopupZorder_content);
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
	
	CCMenuItem* normal_strength_item = CCMenuItemSprite::create(n_normal_strength, s_normal_strength, this, menu_selector(BeautyStoneStrengthPopup::menuAction));
	normal_strength_item->setTag(kBeautyStoneStrengthPopupMenuTag_normalStrength);
	
	CCMenu* normal_strength_menu = CCMenu::createWithItem(normal_strength_item);
	normal_strength_menu->setPosition(ccp(415,147));
	main_case->addChild(normal_strength_menu, kBeautyStoneStrengthPopupZorder_content);
	normal_strength_menu->setTouchPriority(-184);
	
	
	
	CommonButton* align_rank_menu = CommonButton::create("등급순", 14, CCSizeMake(75,45), CommonButtonYellow, -184);
	align_rank_menu->setTitleColor(ccBLACK);
	align_rank_menu->setPosition(ccp(416,35));
	main_case->addChild(align_rank_menu, kBeautyStoneStrengthPopupZorder_content);
	align_rank_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kBeautyStoneStrengthPopupMenuTag_alignRank);
									 menuAction(t_node);
								 });
	
	
	CommonButton* align_take_menu = CommonButton::create("레벨순", 14, CCSizeMake(75,45), CommonButtonYellow, -184);
	align_take_menu->setTitleColor(ccBLACK);
	align_take_menu->setPosition(ccp(416,72));
	main_case->addChild(align_take_menu, kBeautyStoneStrengthPopupZorder_content);
	align_take_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kBeautyStoneStrengthPopupMenuTag_alignLevel);
									 menuAction(t_node);
								 });
	
	CCScale9Sprite* strength_title = CommonButton::getBackgroundByType(CommonButtonOrange);
	strength_title->setContentSize(CCSizeMake(145, 35));
	strength_title->setPosition(ccp(110,212));
	CCLabelTTF* strength_title_label = CCLabelTTF::create("강화 뷰티스톤", mySGD->getFont().c_str(), 12);
	strength_title_label->setPosition(ccp(strength_title->getContentSize().width/2.f, strength_title->getContentSize().height/2.f));
	strength_title->addChild(strength_title_label);
	main_case->addChild(strength_title, kBeautyStoneStrengthPopupZorder_content);
	
	
	CCScale9Sprite* offering_title = CommonButton::getBackgroundByType(CommonButtonGreen);
	offering_title->setContentSize(CCSizeMake(145, 35));
	offering_title->setPosition(ccp(290,212));
	CCLabelTTF* offering_title_label = CCLabelTTF::create("재료 뷰티스톤", mySGD->getFont().c_str(), 12);
	offering_title_label->setPosition(ccp(offering_title->getContentSize().width/2.f, offering_title->getContentSize().height/2.f));
	offering_title->addChild(offering_title_label);
	main_case->addChild(offering_title, kBeautyStoneStrengthPopupZorder_content);
	
	
	strength_node = NULL;
	offering_node = NULL;
	strength_probability = NULL;
	
	
	setStrengthNode(-1);
	setOfferingNode(-1);
	
	setOfferingList();
	
	CCSize table_size = CCSizeMake(315, 73);
	CCPoint table_position = ccp(54, 17);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(table_position);
//	main_case->addChild(temp_back, kBeautyStoneStrengthPopupZorder_content);
	
	offering_table = CCTableView::create(this, table_size);
	offering_table->setAnchorPoint(CCPointZero);
	offering_table->setDirection(kCCScrollViewDirectionHorizontal);
	offering_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	offering_table->setPosition(table_position);
	offering_table->setDelegate(this);
	main_case->addChild(offering_table, kBeautyStoneStrengthPopupZorder_content);
	offering_table->setTouchPriority(-180);
	
	TouchSuctionLayer* suction = TouchSuctionLayer::create(-181);
	suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	suction->setTouchEnabled(true);
	main_case->addChild(suction);
	
	table_tab = NULL;
	setTableTab();
	
	
	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(BeautyStoneStrengthPopup::menuAction));
	tip_item->setTag(kBeautyStoneStrengthPopupMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
	addChild(tip_menu, kBeautyStoneStrengthPopupZorder_content);
	
	tip_menu->setTouchPriority(-183);
	
	
    return true;
}

void BeautyStoneStrengthPopup::setTableTab()
{
	if(table_tab)
	{
		table_tab->removeFromParent();
		table_tab = NULL;
	}
	
	if(strength_stone_idx == -1)
	{
		table_tab = CCSprite::create("stonestrength_table_tab_strength.png");
		table_tab->setPosition(ccp(38,61));
		main_case->addChild(table_tab, kBeautyStoneStrengthPopupZorder_content);
	}
	else
	{
		table_tab = CCSprite::create("stonestrength_table_tab_offering.png");
		table_tab->setPosition(ccp(38,61));
		main_case->addChild(table_tab, kBeautyStoneStrengthPopupZorder_content);
	}
}

void BeautyStoneStrengthPopup::setStrengthNode(int t_strength_idx)
{
	if(strength_node)
	{
		strength_node->removeFromParent();
		strength_node = NULL;
	}
	
	strength_stone_idx = t_strength_idx;
	
	if(strength_stone_idx == -1)
	{
		
	}
	else
	{
		int strength_stone_id = strength_stone_idx;
		
		strength_node = CCNode::create();
		strength_node->setPosition(ccp(85,157));
		main_case->addChild(strength_node, kBeautyStoneStrengthPopupZorder_content);
		
		CCSprite* n_back = CCSprite::create("stonestrength_slot_back.png");
		CCSprite* s_back = CCSprite::create("stonestrength_slot_back.png");
		s_back->setColor(ccGRAY);
		
		CCMenuItem* slot_item = CCMenuItemSprite::create(n_back, s_back, this, menu_selector(BeautyStoneStrengthPopup::menuAction));
		slot_item->setTag(kBeautyStoneStrengthPopupMenuTag_strengthStone);
		
		CCMenu* slot_menu = CCMenu::createWithItem(slot_item);
		slot_menu->setPosition(ccp(0,0));
		strength_node->addChild(slot_menu);
		
		CCSprite* stone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, strength_stone_id), myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, strength_stone_id))->getCString());
		stone_img->setPosition(ccp(0, 0));
		strength_node->addChild(stone_img);
		
		CCLabelTTF* stone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, strength_stone_id))->getCString(), mySGD->getFont().c_str(), 12);
		stone_level->setAnchorPoint(ccp(1,0));
		stone_level->setPosition(ccp(n_back->getContentSize().width/2.f-3, -n_back->getContentSize().height/2.f+3));
		strength_node->addChild(stone_level);
		
		CCLabelTTF* stone_type = CCLabelTTF::create(BeautyStoneSettingPopup::convertToTypeName(myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, strength_stone_id)).c_str(), mySGD->getFont().c_str(), 12);
		stone_type->setAnchorPoint(ccp(0,0.5));
		stone_type->setPosition(ccp(n_back->getContentSize().width/2.f+5, n_back->getContentSize().height*0.3));
		strength_node->addChild(stone_type);
		
		slot_menu->setTouchPriority(-184);
	}
}

void BeautyStoneStrengthPopup::setOfferingNode(int t_offering_idx)
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
	
	offering_stone_idx = t_offering_idx;
	
	if(offering_stone_idx == -1)
	{
		
	}
	else
	{
		int offering_stone_id = offering_stone_idx;
		
		offering_node = CCNode::create();
		offering_node->setPosition(ccp(260,157));
		main_case->addChild(offering_node, kBeautyStoneStrengthPopupZorder_content);
		
		CCSprite* n_back = CCSprite::create("stonestrength_slot_back.png");
		CCSprite* s_back = CCSprite::create("stonestrength_slot_back.png");
		s_back->setColor(ccGRAY);
		
		CCMenuItem* slot_item = CCMenuItemSprite::create(n_back, s_back, this, menu_selector(BeautyStoneStrengthPopup::menuAction));
		slot_item->setTag(kBeautyStoneStrengthPopupMenuTag_offeringStone);
		
		CCMenu* slot_menu = CCMenu::createWithItem(slot_item);
		slot_menu->setPosition(ccp(0,0));
		offering_node->addChild(slot_menu);
		
		CCSprite* stone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, offering_stone_id), myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, offering_stone_id))->getCString());
		stone_img->setPosition(ccp(0, 0));
		offering_node->addChild(stone_img);
		
		CCLabelTTF* stone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, offering_stone_id))->getCString(), mySGD->getFont().c_str(), 12);
		stone_level->setAnchorPoint(ccp(1,0));
		stone_level->setPosition(ccp(n_back->getContentSize().width/2.f-3, -n_back->getContentSize().height/2.f+3));
		offering_node->addChild(stone_level);
		
		CCLabelTTF* stone_type = CCLabelTTF::create(BeautyStoneSettingPopup::convertToTypeName(myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, offering_stone_id)).c_str(), mySGD->getFont().c_str(), 12);
		stone_type->setAnchorPoint(ccp(0,0.5));
		stone_type->setPosition(ccp(n_back->getContentSize().width/2.f+5, n_back->getContentSize().height*0.3));
		offering_node->addChild(stone_type);
		
		slot_menu->setTouchPriority(-184);
		
		strength_probability = CCSprite::create("cardsetting_probability.png");
		strength_probability->setPosition(ccp(198.5,147));
		main_case->addChild(strength_probability, kBeautyStoneStrengthPopupZorder_content);
		
//		float strength_rate = ((NSDS_GI(kSDS_CI_int1_rank_i, offering_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, offering_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, offering_card_number))/((NSDS_GI(kSDS_CI_int1_rank_i, strength_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, strength_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, strength_card_number));
		float strength_rate = 1.f;
		
		if(strength_rate > 1.f)
			strength_rate = 1.f;
		
		CCLabelTTF* probability_label = CCLabelTTF::create(CCString::createWithFormat("%.1f", strength_rate*100.f)->getCString(), mySGD->getFont().c_str(), 10);
		probability_label->setAnchorPoint(ccp(1.f,0.5f));
		probability_label->setPosition(ccp(strength_probability->getContentSize().width-18, strength_probability->getContentSize().height/2.f));
		strength_probability->addChild(probability_label);
	}
}

void BeautyStoneStrengthPopup::setOfferingList()
{
	int loop_length = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
	for(int i=0;i<loop_length;i++)
	{
		int stone_id = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i+1);
		
		BeautyStoneInfo t_info;
		t_info.m_id = stone_id;
		t_info.m_type = myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, stone_id);
		t_info.m_rank = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, stone_id);
		t_info.m_level = myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, stone_id);
		
		offering_list.push_back(t_info);
	}
	
	alignOfferingList(kBeautyStoneStrengthAlign_rank);
}

void BeautyStoneStrengthPopup::alignOfferingList(BeautyStoneStrengthAlign t_align)
{
	recent_align = t_align;
	
	if(t_align == kBeautyStoneStrengthAlign_rank)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_rank > b.m_rank;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kBeautyStoneStrengthAlign_rankReverse)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_rank < b.m_rank;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kBeautyStoneStrengthAlign_level)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_level > b.m_level;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kBeautyStoneStrengthAlign_levelReverse)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_level < b.m_level;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
}

CCTableViewCell* BeautyStoneStrengthPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCPoint base_position = ccp(25,40);
	
	CCSprite* n_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", offering_list[idx].m_type, offering_list[idx].m_rank)->getCString());
	CCSprite* s_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", offering_list[idx].m_type, offering_list[idx].m_rank)->getCString());
	s_img->setColor(ccGRAY);
	
	CCMenuItem* img_item = CCMenuItemSprite::create(n_img, s_img, this, menu_selector(BeautyStoneStrengthPopup::cellAction));
	img_item->setTag(idx);
	
	ScrollMenu* img_menu = ScrollMenu::create(img_item, NULL);
	img_menu->setPosition(base_position);
	cell->addChild(img_menu);
	
	img_menu->setTouchPriority(-179);
	
	CCLabelTTF* beautystone_type = CCLabelTTF::create(BeautyStoneSettingPopup::convertToTypeName(offering_list[idx].m_type).c_str(), mySGD->getFont().c_str(), 10);
	beautystone_type->setPosition(ccpAdd(base_position, ccp(0,-20)));
	cell->addChild(beautystone_type);
	
	CCLabelTTF* beautystone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", offering_list[idx].m_level)->getCString(), mySGD->getFont().c_str(), 10);
	beautystone_level->setPosition(ccpAdd(base_position, ccp(0,-10)));
	cell->addChild(beautystone_level);
	
	return cell;
}

void BeautyStoneStrengthPopup::cellAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	int stone_id = offering_list[tag].m_id;
	
	if(strength_stone_idx == -1)
	{
		if(offering_list[tag].m_level >= 5)
		{
			addChild(ASPopupView::getCommonNoti(-200, "더 이상 강화할 수 없는 뷰티스톤 입니다."), kBeautyStoneStrengthPopupZorder_popup);
		}
		else
		{
			setStrengthNode(stone_id);
			setTableTab();
		}
	}
	else
	{
		if(strength_stone_idx != stone_id)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, strength_stone_idx) != offering_list[tag].m_type)
			{
				addChild(ASPopupView::getCommonNoti(-200, "같은 타입의 뷰티스톤으로 강화할 수 있습니다."), kBeautyStoneStrengthPopupZorder_popup);
			}
			else
			{
				setOfferingNode(stone_id);
			}
		}
	}
	
	
	is_menu_enable = true;
}

void BeautyStoneStrengthPopup::scrollViewDidScroll(CCScrollView* view)
{
	
}
void BeautyStoneStrengthPopup::scrollViewDidZoom(CCScrollView* view)
{
	
}
void BeautyStoneStrengthPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}
CCSize BeautyStoneStrengthPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(50, 73);
}
unsigned int BeautyStoneStrengthPopup::numberOfCellsInTableView(CCTableView *table)
{
	return offering_list.size();
}

void BeautyStoneStrengthPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void BeautyStoneStrengthPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(BeautyStoneStrengthPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void BeautyStoneStrengthPopup::endShowPopup()
{
	is_menu_enable = true;
}

void BeautyStoneStrengthPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(BeautyStoneStrengthPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void BeautyStoneStrengthPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void BeautyStoneStrengthPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kBeautyStoneStrengthPopupMenuTag_close)
	{
//		target_final = NULL;
		hidePopup();
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_alignRank)
	{
		if(recent_align == kBeautyStoneStrengthAlign_rankReverse)
			alignOfferingList(kBeautyStoneStrengthAlign_rank);
		else
			alignOfferingList(kBeautyStoneStrengthAlign_rankReverse);
		
		offering_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_alignLevel)
	{
		if(recent_align == kBeautyStoneStrengthAlign_levelReverse)
			alignOfferingList(kBeautyStoneStrengthAlign_level);
		else
			alignOfferingList(kBeautyStoneStrengthAlign_levelReverse);
		
		offering_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_offeringStone)
	{
		// 재료 카드 해제
//		int keep_idx = offering_idx;
		setOfferingNode(-1);
//		if(keep_idx >= 0)
//			offering_table->updateCellAtIndex(keep_idx);
		
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_strengthStone)
	{
		if(offering_stone_idx != -1)
		{
			// 강화 카드 해제 불가
			is_menu_enable = true;
		}
		else
		{
//			int keep_idx = strength_idx;
			setStrengthNode(-1);
//			if(keep_idx >= 0)
//				offering_table->updateCellAtIndex(keep_idx);
			setTableTab();
			is_menu_enable = true;
		}
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_highStrength)
	{
		if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getCardUpgradeRubyFee() && offering_stone_idx > -1)
		{
//			mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - mySGD->getCardUpgradeRubyFee());
			float strength_rate = 1.f;
			
			int character_count = NSDS_GI(kSDS_GI_characterCount_i);
			for(int i=0;i<character_count;i++)
			{
				int character_slot_count = 2;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i+1);
				for(int j=0;j<character_slot_count;j++)
				{
					if(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i+1, j+1) == offering_stone_idx)
						myDSH->setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i+1, j+1, 0);
				}
			}
			
			int have_stone_count = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
			bool is_found = false;
			for(int i=0;i<have_stone_count;i++)
			{
				if(!is_found)
				{
					if(myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i+1) == offering_stone_idx)
						is_found = true;
				}
				else
					myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i, myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i+1));
			}
			
			myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, have_stone_count-1);
			
			vector<BeautyStoneInfo>::iterator iter;
			for(iter = offering_list.begin();iter != offering_list.end();iter++)
			{
				if((*iter).m_id == offering_stone_idx)
					offering_list.erase(iter);
			}
			
			random_device rd;
			default_random_engine e1(rd());
			uniform_real_distribution<float> uniform_dist(0.f, 1.f);

			float result_value = uniform_dist(e1);
			CCLOG("result value : %.3f", result_value);

			
			if(result_value <= strength_rate)
			{
				CCLOG("success");
				myDSH->setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, strength_stone_idx, myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, strength_stone_idx)+1);
				vector<BeautyStoneInfo>::iterator iter2;
				for(iter2 = offering_list.begin();iter2 != offering_list.end();iter2++)
				{
					if((*iter2).m_id == strength_stone_idx)
					{
						(*iter2).m_level++;
					}
				}
				
				TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-250);
				addChild(t_suction, kBeautyStoneStrengthPopupZorder_popup);
				
				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
				if(screen_scale_x < 1.f)
					screen_scale_x = 1.f;
				
				CCSprite* loading_img = CCSprite::create("back_gray.png");
				loading_img->setPosition(ccp(240,160));
				loading_img->setScaleX(screen_scale_x);
				loading_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
				t_suction->addChild(loading_img);
				
				CCLabelTTF* loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 30);
				loading_label->setPosition(ccp(240,160));
				t_suction->addChild(loading_label);

				
//				myDSH->saveAllUserData([=](Json::Value result_data)
//									   {
//										   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//										   {
//											
//										   }
//										   else
//										   {
//											   
//										   }
//										   
//										   t_suction->removeFromParent();
//										   setStrengthNode(strength_stone_idx);
//										   offering_table->reloadData();
//										   setOfferingNode(-1);
//										   
//										   addChild(ASPopupView::getCommonNoti(-250, "강화 성공!!"), kBeautyStoneStrengthPopupZorder_popup);
//										   
//										   is_menu_enable = true;
//									   });
			}
			else
			{
				CCLOG("fail");
				
				TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-250);
				addChild(t_suction, kBeautyStoneStrengthPopupZorder_popup);
				
				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
				if(screen_scale_x < 1.f)
					screen_scale_x = 1.f;
				
				CCSprite* loading_img = CCSprite::create("back_gray.png");
				loading_img->setPosition(ccp(240,160));
				loading_img->setScaleX(screen_scale_x);
				loading_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
				t_suction->addChild(loading_img);
				
				CCLabelTTF* loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 30);
				loading_label->setPosition(ccp(240,160));
				t_suction->addChild(loading_label);
				
//				myDSH->saveAllUserData([=](Json::Value result_data)
//									   {
//										   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//										   {
//											   
//										   }
//										   else
//										   {
//											   
//										   }
//										   
//										   t_suction->removeFromParent();
//										   offering_table->reloadData();
//										   setOfferingNode(-1);
//										   
//										   addChild(ASPopupView::getCommonNoti(-250, "강화 실패!!"), kBeautyStoneStrengthPopupZorder_popup);
//										   
//										   is_menu_enable = true;
//									   });
			}
		}
		else
		{
			if(offering_stone_idx <= -1)
				addChild(ASPopupView::getCommonNoti(-210, "재료 스톤을 선택해주세요."), kBeautyStoneStrengthPopupZorder_popup);
			else
				addChild(ASPopupView::getCommonNoti(-210, "젬이 부족합니다."), kBeautyStoneStrengthPopupZorder_popup);
			is_menu_enable = true;
		}
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_normalStrength)
	{
		if(mySGD->getGoodsValue(kGoodsType_gold) >= mySGD->getCardUpgradeGoldFee() && offering_stone_idx > -1)
		{
//			mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) - mySGD->getCardUpgradeGoldFee());
			float strength_rate = 1.f;
			
			int character_count = NSDS_GI(kSDS_GI_characterCount_i);
			for(int i=0;i<character_count;i++)
			{
				int character_slot_count = 2;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i+1);
				for(int j=0;j<character_slot_count;j++)
				{
					if(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i+1, j+1) == offering_stone_idx)
						myDSH->setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i+1, j+1, 0);
				}
			}
			
			int have_stone_count = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
			bool is_found = false;
			for(int i=0;i<have_stone_count;i++)
			{
				if(!is_found)
				{
					if(myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i+1) == offering_stone_idx)
						is_found = true;
				}
				else
					myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i, myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i+1));
			}
			
			myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, have_stone_count-1);
			
			vector<BeautyStoneInfo>::iterator iter;
			for(iter = offering_list.begin();iter != offering_list.end();iter++)
			{
				if((*iter).m_id == offering_stone_idx)
				{
					offering_list.erase(iter);
					break;
				}
			}
			
			random_device rd;
			default_random_engine e1(rd());
			uniform_real_distribution<float> uniform_dist(0.f, 1.f);
			
			float result_value = uniform_dist(e1);
			CCLOG("result value : %.3f", result_value);
			
			
			if(result_value <= strength_rate)
			{
				CCLOG("success");
				myDSH->setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, strength_stone_idx, myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, strength_stone_idx)+1);
				vector<BeautyStoneInfo>::iterator iter2;
				for(iter2 = offering_list.begin();iter2 != offering_list.end();iter2++)
				{
					if((*iter2).m_id == strength_stone_idx)
					{
						(*iter2).m_level++;
					}
				}
				
				TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-250);
				addChild(t_suction, kBeautyStoneStrengthPopupZorder_popup);
				
				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
				if(screen_scale_x < 1.f)
					screen_scale_x = 1.f;
				
				CCSprite* loading_img = CCSprite::create("back_gray.png");
				loading_img->setPosition(ccp(240,160));
				loading_img->setScaleX(screen_scale_x);
				loading_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
				t_suction->addChild(loading_img);
				
				CCLabelTTF* loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 30);
				loading_label->setPosition(ccp(240,160));
				t_suction->addChild(loading_label);
				
				
//				myDSH->saveAllUserData([=](Json::Value result_data)
//									   {
//										   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//										   {
//											   
//										   }
//										   else
//										   {
//											   
//										   }
//										   
//										   t_suction->removeFromParent();
//										   setStrengthNode(strength_stone_idx);
//										   offering_table->reloadData();
//										   setOfferingNode(-1);
//										   
//										   addChild(ASPopupView::getCommonNoti(-250, "강화 성공!!"), kBeautyStoneStrengthPopupZorder_popup);
//										   
//										   is_menu_enable = true;
//									   });
			}
			else
			{
				CCLOG("fail");
				
				TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-250);
				addChild(t_suction, kBeautyStoneStrengthPopupZorder_popup);
				
				CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
				float screen_scale_x = screen_size.width/screen_size.height/1.5f;
				if(screen_scale_x < 1.f)
					screen_scale_x = 1.f;
				
				CCSprite* loading_img = CCSprite::create("back_gray.png");
				loading_img->setPosition(ccp(240,160));
				loading_img->setScaleX(screen_scale_x);
				loading_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
				t_suction->addChild(loading_img);
				
				CCLabelTTF* loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 30);
				loading_label->setPosition(ccp(240,160));
				t_suction->addChild(loading_label);
				
//				myDSH->saveAllUserData([=](Json::Value result_data)
//									   {
//										   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//										   {
//											   
//										   }
//										   else
//										   {
//											   
//										   }
//										   
//										   t_suction->removeFromParent();
//										   offering_table->reloadData();
//										   setOfferingNode(-1);
//										   
//										   addChild(ASPopupView::getCommonNoti(-250, "강화 실패!!"), kBeautyStoneStrengthPopupZorder_popup);
//										   
//										   is_menu_enable = true;
//									   });
			}
		}
		else
		{
			if(offering_stone_idx <= -1)
				addChild(ASPopupView::getCommonNoti(-210, "재료 스톤을 선택해주세요."), kBeautyStoneStrengthPopupZorder_popup);
			else
				addChild(ASPopupView::getCommonNoti(-210, "골드가 부족합니다."), kBeautyStoneStrengthPopupZorder_popup);
			is_menu_enable = true;
		}
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_tip)
	{
//		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
//		t_suction->target_touch_began = t_suction;
//		t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//		t_suction->setTouchEnabled(true);
//		
//		CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardstrength.png");
//		dimed_tip->setPosition(ccp(240,160));
//		t_suction->addChild(dimed_tip);
//		
//		addChild(t_suction, kCardStrengthPopupZorder_popup);
		
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_synthesis)
	{
		BeautyStoneSynthesisPopup* t_popup = BeautyStoneSynthesisPopup::create();
		getParent()->addChild(t_popup, getZOrder());
		t_popup->setHideFinalAction(target_final, delegate_final);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == kBeautyStoneStrengthPopupMenuTag_setting)
	{
		BeautyStoneSettingPopup* t_popup = BeautyStoneSettingPopup::create();
		getParent()->addChild(t_popup, getZOrder());
		t_popup->setHideFinalAction(target_final, delegate_final);
		
		target_final = NULL;
		hidePopup();
	}
}

bool BeautyStoneStrengthPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLOG("touch swallow!!");
	
	return true;
}
void BeautyStoneStrengthPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneStrengthPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneStrengthPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneStrengthPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -179, true);
}
