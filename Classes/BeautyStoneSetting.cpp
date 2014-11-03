//
//  BeautyStoneSetting.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 3. 4..
//
//

#include "BeautyStoneSetting.h"
#include "DataStorageHub.h"
#include "TouchSuctionLayer.h"
#include "ASPopupView.h"
#include "StageImgLoader.h"
#include "CommonButton.h"
#include "ScrollMenu.h"
#include "BeautyStoneStrength.h"
#include "BeautyStoneSynthesis.h"

enum BeautyStoneSettingPopupZorder{
	kBeautyStoneSettingPopupZorder_gray = 1,
	kBeautyStoneSettingPopupZorder_back,
	kBeautyStoneSettingPopupZorder_content,
	kBeautyStoneSettingPopupZorder_popup
};

enum BeautyStoneSettingPopupMenuTag{
	kBeautyStoneSettingPopupMenuTag_close = 1,
	kBeautyStoneSettingPopupMenuTag_synthesis,
	kBeautyStoneSettingPopupMenuTag_strength,
	kBeautyStoneSettingPopupMenuTag_alignTake,
	kBeautyStoneSettingPopupMenuTag_alignRank,
	kBeautyStoneSettingPopupMenuTag_alignLevel,
	kBeautyStoneSettingPopupMenuTag_tip,
	kBeautyStoneSettingPopupMenuTag_slotBase = 10000,
	kBeautyStoneSettingPopupMenuTag_listBase = 20000,
};

void BeautyStoneSettingPopup::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

// on "init" you need to initialize your instance
bool BeautyStoneSettingPopup::init()
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
	addChild(gray, kBeautyStoneSettingPopupZorder_gray);
	
	main_case = CCSprite::create("stonesetting_back.png");
	main_case->setAnchorPoint(ccp(0.5,0.5));
	main_case->setPosition(ccp(240,160-450));
	addChild(main_case, kBeautyStoneSettingPopupZorder_back);
	
	
	CommonButton* close_menu = CommonButton::createCloseButton(-184);
	close_menu->setPosition(ccp(452,256));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kBeautyStoneSettingPopupMenuTag_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kBeautyStoneSettingPopupZorder_content);
	
	
	CommonButton* synthesis_menu = CommonButton::create("뷰티스톤 합성", 13, CCSizeMake(110,40), CommonButtonYellow, -184);
	synthesis_menu->setTitleColor(ccBLACK);
	synthesis_menu->setPosition(ccp(260,256));
	main_case->addChild(synthesis_menu, kBeautyStoneSettingPopupZorder_content);
	synthesis_menu->setFunction([=](CCObject* sender)
								{
									CCNode* t_node = CCNode::create();
									t_node->setTag(kBeautyStoneSettingPopupMenuTag_synthesis);
									menuAction(t_node);
								});
	
	CommonButton* strength_menu = CommonButton::create("뷰티스톤 강화", 13, CCSizeMake(110,40), CommonButtonYellow, -184);
	strength_menu->setTitleColor(ccBLACK);
	strength_menu->setPosition(ccp(370,256));
	main_case->addChild(strength_menu, kBeautyStoneSettingPopupZorder_content);
	strength_menu->setFunction([=](CCObject* sender)
								{
									CCNode* t_node = CCNode::create();
									t_node->setTag(kBeautyStoneSettingPopupMenuTag_strength);
									menuAction(t_node);
								});
	
	CommonButton* align_take_menu = CommonButton::create("획득순", 13, CCSizeMake(65,40), CommonButtonGreen, -184);
	align_take_menu->setTitleColor(ccWHITE);
	align_take_menu->setPosition(ccp(290,220));
	main_case->addChild(align_take_menu, kBeautyStoneSettingPopupZorder_content);
	align_take_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kBeautyStoneSettingPopupMenuTag_alignTake);
									 menuAction(t_node);
								 });
	
	CommonButton* align_rank_menu = CommonButton::create("등급순", 13, CCSizeMake(65,40), CommonButtonGreen, -184);
	align_rank_menu->setTitleColor(ccWHITE);
	align_rank_menu->setPosition(ccp(355,220));
	main_case->addChild(align_rank_menu, kBeautyStoneSettingPopupZorder_content);
	align_rank_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kBeautyStoneSettingPopupMenuTag_alignRank);
									 menuAction(t_node);
								 });
	
	CommonButton* align_level_menu = CommonButton::create("레벨순", 13, CCSizeMake(65,40), CommonButtonGreen, -184);
	align_level_menu->setTitleColor(ccWHITE);
	align_level_menu->setPosition(ccp(420,220));
	main_case->addChild(align_level_menu, kBeautyStoneSettingPopupZorder_content);
	align_level_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(kBeautyStoneSettingPopupMenuTag_alignLevel);
									 menuAction(t_node);
								 });
	
	int selected_character_slot_cnt = 2;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1);
	
	slot_vector.clear();
	for(int i=0;i<3;i++)
	{
		if(i < selected_character_slot_cnt)
		{
			int slot_stone_id = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1, i+1);
			if(slot_stone_id != 0)
			{
				CCNode* slot_back = CCNode::create();
				slot_back->setPosition(ccp(97,204-i*79));
				main_case->addChild(slot_back, kBeautyStoneSettingPopupZorder_content);
				
				CCSprite* n_back = CCSprite::create("stonesetting_slot_back.png");
				CCSprite* s_back = CCSprite::create("stonesetting_slot_back.png");
				s_back->setColor(ccGRAY);
				
				CCMenuItem* slot_item = CCMenuItemSprite::create(n_back, s_back, this, menu_selector(BeautyStoneSettingPopup::menuAction));
				slot_item->setTag(kBeautyStoneSettingPopupMenuTag_slotBase + i);
				
				CCMenu* slot_menu = CCMenu::createWithItem(slot_item);
				slot_menu->setPosition(ccp(0,0));
				slot_back->addChild(slot_menu);
				slot_menu->setTouchPriority(-184);
				
				slot_vector.push_back(slot_back);
				
				CCSprite* stone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, slot_stone_id), myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, slot_stone_id))->getCString());
				stone_img->setPosition(ccp(0, 0));
				slot_back->addChild(stone_img);
				
				CCLabelTTF* stone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, slot_stone_id))->getCString(), mySGD->getFont().c_str(), 12);
				stone_level->setAnchorPoint(ccp(1,0));
				stone_level->setPosition(ccp(n_back->getContentSize().width/2.f-3, -n_back->getContentSize().height/2.f+3));
				slot_back->addChild(stone_level);
				
				CCLabelTTF* stone_type = CCLabelTTF::create(convertToTypeName(myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, slot_stone_id)).c_str(), mySGD->getFont().c_str(), 12);
				stone_type->setAnchorPoint(ccp(0,0.5));
				stone_type->setPosition(ccp(n_back->getContentSize().width/2.f+5, n_back->getContentSize().height*0.3));
				slot_back->addChild(stone_type);
				
				CCSprite* tag_img = CCSprite::create("stonesetting_ing.png");
				tag_img->setPosition(ccp(-20-n_back->getContentSize().width/2.f,39-n_back->getContentSize().height/2.f));
				slot_back->addChild(tag_img);
			}
			else
			{
				CCSprite* slot_back = CCSprite::create("stonesetting_slot_back.png");
				slot_back->setPosition(ccp(97,204-i*79));
				main_case->addChild(slot_back, kBeautyStoneSettingPopupZorder_content);
				
				slot_vector.push_back(slot_back);
				
				CCSprite* tag_img = CCSprite::create("stonesetting_no.png");
				tag_img->setPosition(ccp(-20,39));
				slot_back->addChild(tag_img);
			}
		}
		else
		{
			CCSprite* slot_lock = CCSprite::create("stonesetting_slot_lock.png");
			slot_lock->setPosition(ccp(133,204-i*79));
			main_case->addChild(slot_lock, kBeautyStoneSettingPopupZorder_content);
		}
	}
	
	
	
	have_stone_list.clear();
	// have_stone_list setting
	int total_beautystone_cnt = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
	for(int i=1;i<=total_beautystone_cnt;i++)
	{
		int beautystone_id = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i);
		BeautyStoneInfo t_info;
		t_info.m_id = beautystone_id;
		t_info.m_type = myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, beautystone_id);
		t_info.m_rank = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, beautystone_id);
		t_info.m_level = myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beautystone_id);
		have_stone_list.push_back(t_info);
	}
	
	recent_align = kBeautyStoneAlign_take;
	alignStoneList(kBeautyStoneAlign_takeReverse);
	
	
	CCSize table_size = CCSizeMake(190, 179);
	CCPoint table_position = ccp(260, 20);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(CCPointZero);
//	temp_back->setPosition(table_position);
//	main_case->addChild(temp_back, kBeautyStoneSettingPopupZorder_content);
	
	have_stone_table = CCTableView::create(this, table_size);
	have_stone_table->setAnchorPoint(CCPointZero);
	have_stone_table->setDirection(kCCScrollViewDirectionVertical);
	have_stone_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	have_stone_table->setPosition(table_position);
	have_stone_table->setDelegate(this);
	main_case->addChild(have_stone_table, kBeautyStoneSettingPopupZorder_content);
	have_stone_table->setTouchPriority(-180);
	
	TouchSuctionLayer* suction = TouchSuctionLayer::create(-181);
	suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
	suction->setTouchEnabled(true);
	main_case->addChild(suction);
	
	
	CCSprite* n_tip = CCSprite::create("mainflow_tip.png");
	CCSprite* s_tip = CCSprite::create("mainflow_tip.png");
	s_tip->setColor(ccGRAY);
	
	CCMenuItem* tip_item = CCMenuItemSprite::create(n_tip, s_tip, this, menu_selector(BeautyStoneSettingPopup::menuAction));
	tip_item->setTag(kBeautyStoneSettingPopupMenuTag_tip);
	
	CCMenu* tip_menu = CCMenu::createWithItem(tip_item);
	tip_menu->setPosition(ccp(465,(myDSH->puzzle_ui_top-320.f)/2.f+320.f-3 -13));
	addChild(tip_menu, kBeautyStoneSettingPopupZorder_content);
	
	tip_menu->setTouchPriority(-183);
	
	
    return true;
}

void BeautyStoneSettingPopup::alignStoneList(BeautyStoneAlign t_align)
{
	recent_align = t_align;
	if(recent_align == kBeautyStoneAlign_take)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_id < b.m_id;
			}
		} pred;
		
		sort(have_stone_list.begin(), have_stone_list.end(), pred);
	}
	else if(recent_align == kBeautyStoneAlign_takeReverse)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_id > b.m_id;
			}
		} pred;
		
		sort(have_stone_list.begin(), have_stone_list.end(), pred);
	}
	else if(recent_align == kBeautyStoneAlign_rank)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_rank < b.m_rank;
			}
		} pred;
		
		sort(have_stone_list.begin(), have_stone_list.end(), pred);
	}
	else if(recent_align == kBeautyStoneAlign_rankReverse) // rank
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_rank > b.m_rank;
			}
		} pred;
		
		sort(have_stone_list.begin(), have_stone_list.end(), pred);
	}
	else if(recent_align == kBeautyStoneAlign_level) // rank
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_level < b.m_level;
			}
		} pred;
		
		sort(have_stone_list.begin(), have_stone_list.end(), pred);
	}
	else if(recent_align == kBeautyStoneAlign_levelReverse)
	{
		struct t_StoneSort{
			bool operator() (const BeautyStoneInfo& a, const BeautyStoneInfo& b)
			{
				return a.m_level > b.m_level;
			}
		} pred;
		
		sort(have_stone_list.begin(), have_stone_list.end(), pred);
	}
}

string BeautyStoneSettingPopup::convertToTypeName(int type_code)
{
	string return_value;
	if(type_code == 0)
		return_value = "유도형";
	else if(type_code == 1)
		return_value = "방사형";
	else if(type_code == 2)
		return_value = "레이저";
	else if(type_code == 3)
		return_value = "지뢰형";
	else if(type_code == 4)
		return_value = "소환형";
	else if(type_code == 5)
		return_value = "범위형";
	else if(type_code == 6)
		return_value = "전체공격";
	return return_value;
}

CCTableViewCell* BeautyStoneSettingPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int start_idx = idx*4;
	for(int i=start_idx;i<have_stone_list.size() && i<start_idx+4;i++)
	{
		CCPoint base_position = ccp(cellSizeForTable(table).width/8*((i-start_idx)*2+1),cellSizeForTable(table).height/2.f);
		
		CCSprite* n_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", have_stone_list[i].m_type, have_stone_list[i].m_rank)->getCString());
		CCSprite* s_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", have_stone_list[i].m_type, have_stone_list[i].m_rank)->getCString());
		s_img->setColor(ccGRAY);
		
		CCMenuItem* img_item = CCMenuItemSprite::create(n_img, s_img, this, menu_selector(BeautyStoneSettingPopup::menuAction));
		img_item->setTag(kBeautyStoneSettingPopupMenuTag_listBase + i);
		
		ScrollMenu* img_menu = ScrollMenu::create(img_item, NULL);
		img_menu->setPosition(base_position);
		cell->addChild(img_menu);
		
		img_menu->setTouchPriority(-179);
		
		CCLabelTTF* beautystone_type = CCLabelTTF::create(convertToTypeName(have_stone_list[i].m_type).c_str(), mySGD->getFont().c_str(), 10);
		beautystone_type->setPosition(ccpAdd(base_position, ccp(0,-20)));
		cell->addChild(beautystone_type);
		
		CCLabelTTF* beautystone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", have_stone_list[i].m_level)->getCString(), mySGD->getFont().c_str(), 10);
		beautystone_level->setPosition(ccpAdd(base_position, ccp(0,-10)));
		cell->addChild(beautystone_level);
	}
	
	return cell;
}
void BeautyStoneSettingPopup::scrollViewDidScroll(CCScrollView* view)
{
	
}
void BeautyStoneSettingPopup::scrollViewDidZoom(CCScrollView* view)
{
	
}
void BeautyStoneSettingPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}
CCSize BeautyStoneSettingPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(190, 55);
}
unsigned int BeautyStoneSettingPopup::numberOfCellsInTableView(CCTableView *table)
{
	return (have_stone_list.size()+3)/4;
}

void BeautyStoneSettingPopup::onEnter()
{
	CCLayer::onEnter();
	
	showPopup();
}

void BeautyStoneSettingPopup::showPopup()
{
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 255);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(BeautyStoneSettingPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void BeautyStoneSettingPopup::endShowPopup()
{
	is_menu_enable = true;
}

void BeautyStoneSettingPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCFadeTo* gray_fade = CCFadeTo::create(0.4f, 0);
	gray->runAction(gray_fade);
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(BeautyStoneSettingPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void BeautyStoneSettingPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

void BeautyStoneSettingPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kBeautyStoneSettingPopupMenuTag_close)
	{
//		target_final = NULL;
		hidePopup();
	}
	else if(tag == kBeautyStoneSettingPopupMenuTag_alignRank)
	{
		if(recent_align == kBeautyStoneAlign_rankReverse)
			alignStoneList(kBeautyStoneAlign_rank);
		else
			alignStoneList(kBeautyStoneAlign_rankReverse);

		have_stone_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneSettingPopupMenuTag_alignTake)
	{
		if(recent_align == kBeautyStoneAlign_takeReverse)
			alignStoneList(kBeautyStoneAlign_take);
		else
			alignStoneList(kBeautyStoneAlign_takeReverse);
		
		have_stone_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneSettingPopupMenuTag_alignLevel)
	{
		if(recent_align == kBeautyStoneAlign_levelReverse)
			alignStoneList(kBeautyStoneAlign_level);
		else
			alignStoneList(kBeautyStoneAlign_levelReverse);

		have_stone_table->reloadData();
		is_menu_enable = true;
	}
	else if(tag == kBeautyStoneSettingPopupMenuTag_synthesis)
	{
		BeautyStoneSynthesisPopup* t_popup = BeautyStoneSynthesisPopup::create();
		getParent()->addChild(t_popup, getZOrder());
		t_popup->setHideFinalAction(target_final, delegate_final);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == kBeautyStoneSettingPopupMenuTag_strength)
	{
		BeautyStoneStrengthPopup* t_popup = BeautyStoneStrengthPopup::create();
		getParent()->addChild(t_popup, getZOrder());
		t_popup->setHideFinalAction(target_final, delegate_final);
		
		target_final = NULL;
		hidePopup();
	}
	else if(tag == kBeautyStoneSettingPopupMenuTag_tip)
	{
		TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-200);
		t_suction->target_touch_began = t_suction;
		t_suction->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
		t_suction->setTouchEnabled(true);
		
		CCSprite* dimed_tip = CCSprite::create("tutorial_dimed_cardstrength.png");
		dimed_tip->setPosition(ccp(240,160));
		t_suction->addChild(dimed_tip);
		
		addChild(t_suction, kBeautyStoneSettingPopupZorder_popup);
		
		is_menu_enable = true;
	}
	else if(tag >= kBeautyStoneSettingPopupMenuTag_slotBase && tag < kBeautyStoneSettingPopupMenuTag_listBase)
	{
		int slot_idx = tag - kBeautyStoneSettingPopupMenuTag_slotBase;
		
		CCNode* slot_node = slot_vector[slot_idx];
		CCPoint slot_position = slot_node->getPosition();
		
		slot_node->removeFromParent();
		
		CCSprite* slot_back = CCSprite::create("stonesetting_slot_back.png");
		slot_back->setPosition(slot_position);
		main_case->addChild(slot_back, kBeautyStoneSettingPopupZorder_content);
		
		slot_vector[slot_idx] = slot_back;
		
		CCSprite* tag_img = CCSprite::create("stonesetting_no.png");
		tag_img->setPosition(ccp(-20,39));
		slot_back->addChild(tag_img);
		
		myDSH->setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1, slot_idx+1, 0);
		
		is_menu_enable = true;
	}
	else if(tag >= kBeautyStoneSettingPopupMenuTag_listBase)
	{
		int list_idx = tag - kBeautyStoneSettingPopupMenuTag_listBase;
		
		BeautyStoneInfo clicked_stone_info = have_stone_list[list_idx];
		
		ASPopupView* t_popup = ASPopupView::create(-200);
		
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
		addChild(t_popup, kBeautyStoneSettingPopupZorder_popup);
		
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setContentSize(CCSizeMake(240, 160));
		t_container->addChild(case_back);
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setContentSize(CCSizeMake(220, 90));
		content_back->setPosition(ccp(0,25));
		t_container->addChild(content_back);
		
		
		CCSprite* n_back = CCSprite::create("stonesetting_slot_back.png");
		n_back->setPosition(ccp(-60, 30));
		t_container->addChild(n_back);
		
		
		CCSprite* stone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, clicked_stone_info.m_id ), myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, clicked_stone_info.m_id))->getCString());
		stone_img->setPosition(ccp(n_back->getContentSize().width/2.f, n_back->getContentSize().height/2.f));
		n_back->addChild(stone_img);
		
		CCLabelTTF* stone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, clicked_stone_info.m_id))->getCString(), mySGD->getFont().c_str(), 12);
		stone_level->setAnchorPoint(ccp(1,0));
		stone_level->setPosition(ccp(n_back->getContentSize().width-3, 3));
		n_back->addChild(stone_level);
		
		CCLabelTTF* stone_type = CCLabelTTF::create(convertToTypeName(myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, clicked_stone_info.m_id)).c_str(), mySGD->getFont().c_str(), 12);
		stone_type->setAnchorPoint(ccp(0,0.5));
		stone_type->setPosition(ccp(n_back->getContentSize().width+5, n_back->getContentSize().height*0.8));
		n_back->addChild(stone_type);

		
		
		CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-1);
		close_button->setPosition(ccp(100, 60));
		close_button->setFunction([=](CCObject* sender)
								  {
									  t_popup->removeFromParent();
									  is_menu_enable = true;
								  });
		t_container->addChild(close_button);
		
		
		CommonButton* synthesis_button = CommonButton::create("합성", 12, CCSizeMake(70, 40), CommonButtonOrange, t_popup->getTouchPriority()-1);
		synthesis_button->setTitleColor(ccWHITE);
		synthesis_button->setPosition(ccp(-80,-50));
		t_container->addChild(synthesis_button);
		
		CommonButton* strength_button = CommonButton::create("강화", 12, CCSizeMake(70, 40), CommonButtonOrange, t_popup->getTouchPriority()-1);
		strength_button->setTitleColor(ccWHITE);
		strength_button->setPosition(ccp(0,-50));
		t_container->addChild(strength_button);
		
		CommonButton* mounting_button = CommonButton::create("장착", 12, CCSizeMake(70, 40), CommonButtonOrange, t_popup->getTouchPriority()-1);
		mounting_button->setTitleColor(ccWHITE);
		mounting_button->setPosition(ccp(80,-50));
		t_container->addChild(mounting_button);
		
		synthesis_button->setFunction([=](CCObject* sender)
								  {
									  t_popup->removeFromParent();
									  is_menu_enable = true;
									  CCNode* t_node = CCNode::create();
									  t_node->setTag(kBeautyStoneSettingPopupMenuTag_synthesis);
									  menuAction(t_node);
								  });
		
		
		strength_button->setFunction([=](CCObject* sender)
								   {
									   t_popup->removeFromParent();
									   is_menu_enable = true;
									   CCNode* t_node = CCNode::create();
									   t_node->setTag(kBeautyStoneSettingPopupMenuTag_strength);
									   menuAction(t_node);
								   });
		
		
		mounting_button->setFunction([=](CCObject* sender)
								  {
									  t_popup->removeFromParent();
									  is_menu_enable = true;
									  
									  int slot_cnt = 2;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1);
									  int found_idx = -1;
									  bool is_setted = false;
									  for(int i=0;i<slot_cnt && !is_setted;i++)
									  {
										  if(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1, i+1) == 0 && found_idx == -1)
											  found_idx = i;
										  if(myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1, i+1) == have_stone_list[list_idx].m_id)
											  is_setted = true;
									  }
									  
									  if(is_setted)
										  addChild(ASPopupView::getCommonNoti(t_popup->getTouchPriority(), "이미 장착되어 있습니다."), kBeautyStoneSettingPopupZorder_popup);
									  else if(found_idx == -1)
										  addChild(ASPopupView::getCommonNoti(t_popup->getTouchPriority(), "장착할 빈 슬롯이 없습니다."), kBeautyStoneSettingPopupZorder_popup);
									  else
									  {
										  CCNode* slot_node = slot_vector[found_idx];
										  CCPoint slot_position = slot_node->getPosition();
										  
										  slot_node->removeFromParent();
										  
										  CCNode* slot_back = CCNode::create();
										  slot_back->setPosition(slot_position);
										  main_case->addChild(slot_back, kBeautyStoneSettingPopupZorder_content);
										  
										  CCSprite* n_back = CCSprite::create("stonesetting_slot_back.png");
										  CCSprite* s_back = CCSprite::create("stonesetting_slot_back.png");
										  s_back->setColor(ccGRAY);
										  
										  CCMenuItem* slot_item = CCMenuItemSprite::create(n_back, s_back, this, menu_selector(BeautyStoneSettingPopup::menuAction));
										  slot_item->setTag(kBeautyStoneSettingPopupMenuTag_slotBase + found_idx);
										  
										  CCMenu* slot_menu = CCMenu::createWithItem(slot_item);
										  slot_menu->setPosition(ccp(0,0));
										  slot_back->addChild(slot_menu);
										  slot_menu->setTouchPriority(-184);
										  
										  slot_vector[found_idx] = slot_back;
										  
										  CCSprite* stone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, clicked_stone_info.m_id), myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, clicked_stone_info.m_id))->getCString());
										  stone_img->setPosition(ccp(0, 0));
										  slot_back->addChild(stone_img);
										  
										  CCLabelTTF* stone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, clicked_stone_info.m_id))->getCString(), mySGD->getFont().c_str(), 12);
										  stone_level->setAnchorPoint(ccp(1,0));
										  stone_level->setPosition(ccp(n_back->getContentSize().width/2.f-3, -n_back->getContentSize().height/2.f+3));
										  slot_back->addChild(stone_level);
										  
										  CCLabelTTF* stone_type = CCLabelTTF::create(convertToTypeName(myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, clicked_stone_info.m_id)).c_str(), mySGD->getFont().c_str(), 12);
										  stone_type->setAnchorPoint(ccp(0,0.5));
										  stone_type->setPosition(ccp(n_back->getContentSize().width/2.f+5, n_back->getContentSize().height*0.3));
										  slot_back->addChild(stone_type);
										  
										  CCSprite* tag_img = CCSprite::create("stonesetting_ing.png");
										  tag_img->setPosition(ccp(-20-n_back->getContentSize().width/2.f,39-n_back->getContentSize().height/2.f));
										  slot_back->addChild(tag_img);
										  
										  myDSH->setIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, myDSH->getIntegerForKey(kDSH_Key_selectedCharacter)+1, found_idx+1, have_stone_list[list_idx].m_id);
									  }
								  });
	}
}

bool BeautyStoneSettingPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	CCLOG("touch swallow!!");
	
	return true;
}
void BeautyStoneSettingPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneSettingPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneSettingPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void BeautyStoneSettingPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -179, true);
}