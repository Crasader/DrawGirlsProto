//
//  BeautyStoneSynthesis.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 6..
//
//

#include "BeautyStoneSynthesis.h"
#include "DataStorageHub.h"
#include <random>
#include "TouchSuctionLayer.h"
#include "ASPopupView.h"
#include "StageImgLoader.h"
#include "CommonButton.h"
#include "BeautyStoneSetting.h"
#include "ScrollMenu.h"
#include "BeautyStoneStrength.h"

enum BeautyStoneSynthesisPopupZorder{
	kBeautyStoneSynthesisPopupZorder_gray = 1,
	kBeautyStoneSynthesisPopupZorder_back,
	kBeautyStoneSynthesisPopupZorder_content,
	kBeautyStoneSynthesisPopupZorder_popup
};

enum BeautyStoneSynthesisPopupMenuTag{
	kBeautyStoneSynthesisPopupMenuTag_close = 1,
	kBeautyStoneSynthesisPopupMenuTag_leftStone,
	kBeautyStoneSynthesisPopupMenuTag_rightStone,
	kBeautyStoneSynthesisPopupMenuTag_synthesis,
	kBeautyStoneSynthesisPopupMenuTag_alignRank,
	kBeautyStoneSynthesisPopupMenuTag_alignLevel,
	kBeautyStoneSynthesisPopupMenuTag_tip,
	kBeautyStoneSynthesisPopupMenuTag_strength,
	kBeautyStoneSynthesisPopupMenuTag_setting
};

void BeautyStoneSynthesisPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool BeautyStoneSynthesisPopup::init()
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
	addChild(gray, kBeautyStoneSynthesisPopupZorder_gray);
	
	main_case = CCSprite::create("stonestrength_back.png");
	main_case->setAnchorPoint(ccp(0.5,0.5));
	main_case->setPosition(ccp(240,160-450));
	addChild(main_case, kBeautyStoneSynthesisPopupZorder_back);
	
	CCLabelTTF* title_label = CCLabelTTF::create("뷰티스톤 합성", mySGD->getFont().c_str(), 17);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setPosition(ccp(75,255));
	main_case->addChild(title_label, kBeautyStoneSynthesisPopupZorder_content);
	
	CommonButton* strength_menu = CommonButton::create("뷰티스톤 강화", 13, CCSizeMake(110,40), CommonButtonYellow, -184);
	strength_menu->setTitleColor(ccBLACK);
	strength_menu->setPosition(ccp(260,256));
	main_case->addChild(strength_menu, kBeautyStoneSynthesisPopupZorder_content);
	strength_menu->setFunction([=](CCObject* sender)
								{
									CCNode* t_node = CCNode::create();
									t_node->setTag(kBeautyStoneSynthesisPopupMenuTag_synthesis);
									menuAction(t_node);
								});
	
	CommonButton* setting_menu = CommonButton::create("뷰티스톤 관리", 13, CCSizeMake(110,40), CommonButtonYellow, -184);
	setting_menu->setTitleColor(ccBLACK);
	setting_menu->setPosition(ccp(370,256));
	main_case->addChild(setting_menu, kBeautyStoneSynthesisPopupZorder_content);
	setting_menu->setFunction([=](CCObject* sender)
							   {
								   CCNode* t_node = CCNode::create();
								   t_node->setTag(kBeautyStoneSynthesisPopupMenuTag_setting);
								   menuAction(t_node);
							   });
	
	CommonButton* close_menu = CommonButton::createCloseButton(-184);
	close_menu->setPosition(ccp(452,256));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kBeautyStoneSynthesisPopupMenuTag_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kBeautyStoneSynthesisPopupZorder_content);
	
	
	CommonButton* synthesis_menu = CommonButton::create("합성", 14, CCSizeMake(80, 55), CommonButtonBlue, -184);
	synthesis_menu->setTitleColor(ccWHITE);
	synthesis_menu->setPosition(ccp(415,170));
	main_case->addChild(synthesis_menu, kBeautyStoneSynthesisPopupZorder_content);
	synthesis_menu->setFunction([=](CCObject* sender)
								{
									CCNode* t_node = CCNode::create();
									t_node->setTag(kBeautyStoneSynthesisPopupMenuTag_synthesis);
									menuAction(t_node);
								});
	
	
	
	CommonButton* align_rank_menu = CommonButton::create("등급순", 14, CCSizeMake(75,45), CommonButtonYellow, -184);
	align_rank_menu->setTitleColor(ccBLACK);
	align_rank_menu->setPosition(ccp(416,35));
	main_case->addChild(align_rank_menu, kBeautyStoneSynthesisPopupZorder_content);
	align_rank_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kBeautyStoneSynthesisPopupMenuTag_alignRank);
									 menuAction(t_node);
								 });
	
	
	CommonButton* align_take_menu = CommonButton::create("레벨순", 14, CCSizeMake(75,45), CommonButtonYellow, -184);
	align_take_menu->setTitleColor(ccBLACK);
	align_take_menu->setPosition(ccp(416,72));
	main_case->addChild(align_take_menu, kBeautyStoneSynthesisPopupZorder_content);
	align_take_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kBeautyStoneSynthesisPopupMenuTag_alignLevel);
									 menuAction(t_node);
								 });
	
	CCScale9Sprite* strength_title = CommonButton::getBackgroundByType(CommonButtonOrange);
	strength_title->setContentSize(CCSizeMake(145, 35));
	strength_title->setPosition(ccp(110,212));
	CCLabelTTF* strength_title_label = CCLabelTTF::create("합성 뷰티스톤 1", mySGD->getFont().c_str(), 12);
	strength_title_label->setPosition(ccp(strength_title->getContentSize().width/2.f, strength_title->getContentSize().height/2.f));
	strength_title->addChild(strength_title_label);
	main_case->addChild(strength_title, kBeautyStoneSynthesisPopupZorder_content);
	
	
	CCScale9Sprite* offering_title = CommonButton::getBackgroundByType(CommonButtonGreen);
	offering_title->setContentSize(CCSizeMake(145, 35));
	offering_title->setPosition(ccp(290,212));
	CCLabelTTF* offering_title_label = CCLabelTTF::create("합성 뷰티스톤 2", mySGD->getFont().c_str(), 12);
	offering_title_label->setPosition(ccp(offering_title->getContentSize().width/2.f, offering_title->getContentSize().height/2.f));
	offering_title->addChild(offering_title_label);
	main_case->addChild(offering_title, kBeautyStoneSynthesisPopupZorder_content);
	
	
	strength_node = NULL;
	offering_node = NULL;
	
	
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
	main_case->addChild(offering_table, kBeautyStoneSynthesisPopupZorder_content);
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
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(BeautyStoneSynthesisPopup::menuAction));
	tip_item->setTag(kBeautyStoneSynthesisPopupMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
	addChild(tip_menu, kBeautyStoneSynthesisPopupZorder_content);
	
	tip_menu->setTouchPriority(-183);
	
	
    return true;
}

void BeautyStoneSynthesisPopup::setTableTab()
{
	if(table_tab)
	{
		table_tab->removeFromParent();
		table_tab = NULL;
	}
	
	if(strength_stone_idx == -1)
	{
		table_tab = CCSprite::create("stone_compose1.png");
		table_tab->setPosition(ccp(38,61));
		main_case->addChild(table_tab, kBeautyStoneSynthesisPopupZorder_content);
	}
	else
	{
		table_tab = CCSprite::create("stone_compose2.png");
		table_tab->setPosition(ccp(38,61));
		main_case->addChild(table_tab, kBeautyStoneSynthesisPopupZorder_content);
	}
}

void BeautyStoneSynthesisPopup::setStrengthNode(int t_strength_idx)
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
		main_case->addChild(strength_node, kBeautyStoneSynthesisPopupZorder_content);
		
		CCSprite* n_back = CCSprite::create("stonestrength_slot_back.png");
		CCSprite* s_back = CCSprite::create("stonestrength_slot_back.png");
		s_back->setColor(ccGRAY);
		
		CCMenuItem* slot_item = CCMenuItemSprite::create(n_back, s_back, this, menu_selector(BeautyStoneSynthesisPopup::menuAction));
		slot_item->setTag(kBeautyStoneSynthesisPopupMenuTag_leftStone);
		
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

void BeautyStoneSynthesisPopup::setOfferingNode(int t_offering_idx)
{
	if(offering_node)
	{
		offering_node->removeFromParent();
		offering_node = NULL;
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
		main_case->addChild(offering_node, kBeautyStoneSynthesisPopupZorder_content);
		
		CCSprite* n_back = CCSprite::create("stonestrength_slot_back.png");
		CCSprite* s_back = CCSprite::create("stonestrength_slot_back.png");
		s_back->setColor(ccGRAY);
		
		CCMenuItem* slot_item = CCMenuItemSprite::create(n_back, s_back, this, menu_selector(BeautyStoneSynthesisPopup::menuAction));
		slot_item->setTag(kBeautyStoneSynthesisPopupMenuTag_rightStone);
		
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
	}
}

void BeautyStoneSynthesisPopup::setOfferingList()
{
	int loop_length = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
	for(int i=0;i<loop_length;i++)
	{
		int stone_id = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i+1);
		if(myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, stone_id) >= 5)
		{
			BeautyStoneInfo t_info;
			t_info.m_id = stone_id;
			t_info.m_type = myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, stone_id);
			t_info.m_rank = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, stone_id);
			t_info.m_level = myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, stone_id);
			
			offering_list.push_back(t_info);
		}
	}
	
	alignOfferingList(kBeautyStoneSynthesisAlign_rank);
}

void BeautyStoneSynthesisPopup::alignOfferingList(BeautyStoneSynthesisAlign t_align)
{
	recent_align = t_align;
	
	if(t_align == kBeautyStoneSynthesisAlign_rank)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_rank > b.m_rank;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kBeautyStoneSynthesisAlign_rankReverse)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_rank < b.m_rank;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kBeautyStoneSynthesisAlign_level)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_level > b.m_level;
			}
		} pred;
		
		sort(offering_list.begin(), offering_list.end(), pred);
	}
	else if(t_align == kBeautyStoneSynthesisAlign_levelReverse)
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

CCTableViewCell* BeautyStoneSynthesisPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCPoint base_position = ccp(25,40);
	
	CCSprite* n_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", offering_list[idx].m_type, offering_list[idx].m_rank)->getCString());
	CCSprite* s_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", offering_list[idx].m_type, offering_list[idx].m_rank)->getCString());
	s_img->setColor(ccGRAY);
	
	CCMenuItem* img_item = CCMenuItemSprite::create(n_img, s_img, this, menu_selector(BeautyStoneSynthesisPopup::cellAction));
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

void BeautyStoneSynthesisPopup::cellAction(CCObject *sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	int stone_id = offering_list[tag].m_id;
	
	if(strength_stone_idx == -1)
	{
		setStrengthNode(stone_id);
		setTableTab();
	}
	else
	{
		if(strength_stone_idx != stone_id)
		{
			if(myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, strength_stone_idx) != offering_list[tag].m_rank)
				addChild(ASPopupView::getCommonNoti(-250, "같은 등급의 뷰티스톤끼리만 합성할 수 있습니다."), kBeautyStoneSynthesisPopupZorder_popup);
			else
				setOfferingNode(stone_id);
		}
	}
	
	is_menu_enable = true;
}

void BeautyStoneSynthesisPopup::scrollViewDidScroll(CCScrollView* view)
{
	
}
void BeautyStoneSynthesisPopup::scrollViewDidZoom(CCScrollView* view)
{
	
}
void BeautyStoneSynthesisPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
}
CCSize BeautyStoneSynthesisPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(50, 73);
}
unsigned int BeautyStoneSynthesisPopup::numberOfCellsInTableView(CCTableView *table)
{
	return offering_list.size();
}

void BeautyStoneSynthesisPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void BeautyStoneSynthesisPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(BeautyStoneSynthesisPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void BeautyStoneSynthesisPopup::endShowPopup()
{
	is_menu_enable = true;
}

void BeautyStoneSynthesisPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(BeautyStoneSynthesisPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void BeautyStoneSynthesisPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void BeautyStoneSynthesisPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kBeautyStoneSynthesisPopupMenuTag_close)
	{
		//		target_final = NULL;
		hidePopup();
	}
	else if(tag == kBeautyStoneSynthesisPopupMenuTag_alignRank)
	{
		if(recent_align == kBeautyStoneSynthesisAlign_rankReverse)
			alignOfferingList(kBeautyStoneSynthesisAlign_rank);
		else
			alignOfferingList(kBeautyStoneSynthesisAlign_rankReverse);
		
		offering_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneSynthesisPopupMenuTag_alignLevel)
	{
		if(recent_align == kBeautyStoneSynthesisAlign_levelReverse)
			alignOfferingList(kBeautyStoneSynthesisAlign_level);
		else
			alignOfferingList(kBeautyStoneSynthesisAlign_levelReverse);
		
		offering_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneSynthesisPopupMenuTag_rightStone)
	{
		// 재료 카드 해제
		//		int keep_idx = offering_idx;
		setOfferingNode(-1);
		//		if(keep_idx >= 0)
		//			offering_table->updateCellAtIndex(keep_idx);
		
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneSynthesisPopupMenuTag_leftStone)
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
	else if(tag == kBeautyStoneSynthesisPopupMenuTag_synthesis)
	{
		if(strength_stone_idx > -1 && offering_stone_idx > -1)
		{
			
			ASPopupView* t_popup = ASPopupView::create(-250);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			float height_value = 320.f;
			if(myDSH->screen_convert_rate < 1.f)
				height_value = 320.f/myDSH->screen_convert_rate;
			
			if(height_value < myDSH->ui_top)
				height_value = myDSH->ui_top;
			
			t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
			t_popup->setDimmedPosition(ccp(240, 160));
			t_popup->setBasePosition(ccp(240, 160));
			
			CCNode* t_container = CCNode::create();
			t_popup->setContainerNode(t_container);
			addChild(t_popup, kBeautyStoneSynthesisPopupZorder_popup);
			
			CCLabelTTF* title_label = CCLabelTTF::create("뷰티스톤 합성", mySGD->getFont().c_str(), 18);
			title_label->setColor(ccYELLOW);
			title_label->setPosition(ccp(0,110));
			t_container->addChild(title_label);
			
			CCLabelTTF* comment_label = CCLabelTTF::create("합성시 사용된 두 개의 뷰티스톤은 소진되며,\n한 단계 높은 등급의 뷰티스톤 하나를 랜덤으로 얻게 됩니다.", mySGD->getFont().c_str(), 12);
			comment_label->setColor(ccc3(255, 255, 100));
			comment_label->setPosition(ccp(0,50));
			t_container->addChild(comment_label);
			
			CCLabelTTF* question_label = CCLabelTTF::create("합성하시겠습니까?", mySGD->getFont().c_str(), 15);
			question_label->setColor(ccc3(255, 200, 0));
			question_label->setPosition(ccp(0,-20));
			t_container->addChild(question_label);
			
			CommonButton* cancel_menu = CommonButton::create("나가기", 12, CCSizeMake(75,45), CommonButtonYellow, t_popup->getTouchPriority()-1);
			cancel_menu->setTitleColor(ccBLACK);
			cancel_menu->setPosition(ccp(-50, -50));
			t_container->addChild(cancel_menu);
			cancel_menu->setFunction([=](CCObject* sender)
									 {
										 is_menu_enable = true;
										 t_popup->removeFromParent();
									 });
			
			CommonButton* ok_menu = CommonButton::create("확인", 12, CCSizeMake(75, 45), CommonButtonYellow, t_popup->getTouchPriority()-1);
			ok_menu->setTitleColor(ccBLACK);
			ok_menu->setPosition(ccp(50,-50));
			t_container->addChild(ok_menu);
			ok_menu->setFunction([=](CCObject* sender)
								 {
									 int base_stone_rank = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, strength_stone_idx) + 1;
									 if(base_stone_rank > 5)
										 base_stone_rank = 5;
									 
									 // 각 슬롯에 장착된 뷰티스톤일 경우 해제
									 int character_count = NSDS_GI(kSDS_GI_characterCount_i);
									 for(int i=0;i<character_count;i++)
									 {
										 int character_slot_count = 2;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i+1);
										 for(int j=0;j<character_slot_count;j++)
										 {
											 int t_slot_idx = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i+1, j+1);
											 if(t_slot_idx == offering_stone_idx || t_slot_idx == strength_stone_idx)
												 myDSH->setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i+1, j+1, 0);
										 }
									 }
									 
									 // 가지고 있는 뷰티스톤 당기기
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
									 
									 
									 have_stone_count = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
									 is_found = false;
									 for(int i=0;i<have_stone_count;i++)
									 {
										 if(!is_found)
										 {
											 if(myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i+1) == strength_stone_idx)
												 is_found = true;
										 }
										 else
											 myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i, myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i+1));
									 }
									 myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, have_stone_count-1);
									 
									 
									 // offering_list 에서도 제거
									 vector<BeautyStoneInfo>::iterator iter;
									 for(iter = offering_list.begin();iter != offering_list.end();iter++)
									 {
										 if((*iter).m_id == offering_stone_idx)
										 {
											 offering_list.erase(iter);
											 break;
										 }
									 }
									 
									 for(iter = offering_list.begin();iter != offering_list.end();iter++)
									 {
										 if((*iter).m_id == strength_stone_idx)
										 {
											 offering_list.erase(iter);
											 break;
										 }
									 }
									 
									 
									 myDSH->setIntegerForKey(kDSH_Key_selfBeautyStoneID, myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID)+1);
									 myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt)+1);
									 myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt), myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID));
									 myDSH->setIntegerForKey(kDSH_Key_beautyStoneType_int1, myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID), rand()%7);
									 myDSH->setIntegerForKey(kDSH_Key_beautyStoneRank_int1, myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID), base_stone_rank);
									 myDSH->setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID), 1);
									 
									 TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-250);
									 addChild(t_suction, kBeautyStoneSynthesisPopupZorder_popup);
									 
									 CCSprite* loading_img = CCSprite::create("back_gray.png");
									 loading_img->setPosition(ccp(240,160));
									 loading_img->setScaleX(screen_scale_x);
									 loading_img->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
									 t_suction->addChild(loading_img);
									 
									 CCLabelTTF* loading_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 30);
									 loading_label->setPosition(ccp(240,160));
									 t_suction->addChild(loading_label);
									 
									 
//									 myDSH->saveAllUserData([=](Json::Value result_data)
//															{
//																if(result_data["result"]["code"].asInt() == GDSUCCESS)
//																{
//																	
//																}
//																else
//																{
//																	
//																}
//																
//																t_suction->removeFromParent();
//																setStrengthNode(-1);
//																offering_table->reloadData();
//																setOfferingNode(-1);
//																
//																addChild(ASPopupView::getCommonNoti(-250, "합성 성공!!"), kBeautyStoneSynthesisPopupZorder_popup);
//																
//																is_menu_enable = true;
//															});
									 t_popup->removeFromParent();
								 });
			
		}
		else
		{
			addChild(ASPopupView::getCommonNoti(-210, "합성할 스톤을 선택해주세요."), kBeautyStoneSynthesisPopupZorder_popup);
			is_menu_enable = true;
		}
	}
	else if(tag == kBeautyStoneSynthesisPopupMenuTag_tip)
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
	else if(tag == kBeautyStoneSynthesisPopupMenuTag_strength)
	{
		BeautyStoneStrengthPopup* t_popup = BeautyStoneStrengthPopup::create();
		getParent()->addChild(t_popup, getZOrder());
		t_popup->setHideFinalAction(target_final, delegate_final);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == kBeautyStoneSynthesisPopupMenuTag_setting)
	{
		BeautyStoneSettingPopup* t_popup = BeautyStoneSettingPopup::create();
		getParent()->addChild(t_popup, getZOrder());
		t_popup->setHideFinalAction(target_final, delegate_final);
		
		target_final = NULL;
		hidePopup();
	}
}

bool BeautyStoneSynthesisPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLOG("touch swallow!!");
	
	return true;
}
void BeautyStoneSynthesisPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneSynthesisPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneSynthesisPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneSynthesisPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -179, true);
}
