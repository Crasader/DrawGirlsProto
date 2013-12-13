//
//  ShopPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 11. 22..
//
//

#include "ShopPopup.h"
#include "StarGoldData.h"
#include "HeartTime.h"
#include "LoadingLayer.h"
#include "StageSettingPopup.h"
#include "KSUtil.h"
#include "StageImgLoader.h"

enum ShopPopup_Zorder{
	kSP_Z_back = 1,
	kSP_Z_content,
	kSP_Z_popup
};

enum ShopPopup_MenuTag{
	kSP_MT_close = 1,
	kSP_MT_ruby,
	kSP_MT_gold,
	kSP_MT_heart,
	kSP_MT_character,
	kSP_MT_content1,
	kSP_MT_content2,
	kSP_MT_content3,
	kSP_MT_content4,
	kSP_MT_content5,
	kSP_MT_content6,
	kSP_MT_characterBase = 10000,
	kSP_MT_characterUnlockBase = 20000
};

string ShopPopup::getPriceData(string t_code)
{
	string return_value;
	
	if(t_code == "wonToRuby1")
		return_value = "1100원";
	else if(t_code == "wonToRuby2")
		return_value = "4900원";
	else if(t_code == "wonToRuby3")
		return_value = "9800원";
	else if(t_code == "wonToRuby4")
		return_value = "29800원";
	else if(t_code == "wonToRuby5")
		return_value = "49800원";
	else if(t_code == "wonToRuby6")
		return_value = "99800원";
	else if(t_code == "rubyFromWon1")
		return_value = "10개";
	else if(t_code == "rubyFromWon2")
		return_value = "50개";
	else if(t_code == "rubyFromWon3")
		return_value = "100개";
	else if(t_code == "rubyFromWon4")
		return_value = "300개";
	else if(t_code == "rubyFromWon5")
		return_value = "500개";
	else if(t_code == "rubyFromWon6")
		return_value = "1000개";
	else if(t_code == "rubyToGold1")
		return_value = "10개";
	else if(t_code == "rubyToGold2")
		return_value = "20개";
	else if(t_code == "rubyToGold3")
		return_value = "50개";
	else if(t_code == "rubyToGold4")
		return_value = "100개";
	else if(t_code == "rubyToGold5")
		return_value = "300개";
	else if(t_code == "rubyToGold6")
		return_value = "500개";
	else if(t_code == "goldFromRuby1")
		return_value = "1만골드";
	else if(t_code == "goldFromRuby2")
		return_value = "2만골드";
	else if(t_code == "goldFromRuby3")
		return_value = "5만골드";
	else if(t_code == "goldFromRuby4")
		return_value = "10만골드";
	else if(t_code == "goldFromRuby5")
		return_value = "30만골드";
	else if(t_code == "goldFromRuby6")
		return_value = "50만골드";
	else if(t_code == "rubyToHeart1")
		return_value = "5개";
	else if(t_code == "rubyToHeart2")
		return_value = "10개";
	else if(t_code == "rubyToHeart3")
		return_value = "20개";
	else if(t_code == "rubyToHeart4")
		return_value = "40개";
	else if(t_code == "rubyToHeart5")
		return_value = "75개";
	else if(t_code == "rubyToHeart6")
		return_value = "100개";
	else if(t_code == "heartFromRuby1")
		return_value = "5개";
	else if(t_code == "heartFromRuby2")
		return_value = "10개";
	else if(t_code == "heartFromRuby3")
		return_value = "20개";
	else if(t_code == "heartFromRuby4")
		return_value = "40개";
	else if(t_code == "heartFromRuby5")
		return_value = "75개";
	else if(t_code == "heartFromRuby6")
		return_value = "100개";
	
	return return_value;
}

void ShopPopup::addPriceReward(CCNode *t_target, int t_number)
{
	if(recent_shop_code == kSC_ruby)
	{
		CCLabelTTF* won_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("wonToRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		won_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f-25));
		t_target->addChild(won_label);
		
		CCLabelTTF* ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("rubyFromWon%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		ruby_label->setColor(ccBLACK);
		ruby_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f+25));
		t_target->addChild(ruby_label);
	}
	else if(recent_shop_code == kSC_gold)
	{
		CCSprite* ruby_img = CCSprite::create("price_ruby_img.png");
		ruby_img->setPosition(ccp(t_target->getContentSize().width/2.f-30, t_target->getContentSize().height/2.f-25));
		t_target->addChild(ruby_img);
		
		CCLabelTTF* ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("rubyToGold%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		ruby_label->setPosition(ccp(t_target->getContentSize().width/2.f+10, t_target->getContentSize().height/2.f-25));
		t_target->addChild(ruby_label);
		
		CCLabelTTF* gold_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("goldFromRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		gold_label->setColor(ccBLACK);
		gold_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f+25));
		t_target->addChild(gold_label);
	}
	else if(recent_shop_code == kSC_heart)
	{
		CCSprite* ruby_img = CCSprite::create("price_ruby_img.png");
		ruby_img->setPosition(ccp(t_target->getContentSize().width/2.f-30, t_target->getContentSize().height/2.f-25));
		t_target->addChild(ruby_img);
		
		CCLabelTTF* ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("rubyToHeart%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		ruby_label->setPosition(ccp(t_target->getContentSize().width/2.f+10, t_target->getContentSize().height/2.f-25));
		t_target->addChild(ruby_label);
		
		CCLabelTTF* heart_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("heartFromRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		heart_label->setColor(ccBLACK);
		heart_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f+25));
		t_target->addChild(heart_label);
	}
}

void ShopPopup::setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

void ShopPopup::targetHeartTime(HeartTime *t_heartTime)
{
	target_heartTime = t_heartTime;
}

void ShopPopup::setShopCode(ShopCode t_code)
{
	if(t_code == recent_shop_code)
		return;
	
	if(recent_shop_code == kSC_character)
	{
		character_table->removeFromParent();
	}
	else if(recent_shop_code != kSC_empty)
	{
		main_case->removeChildByTag(kSP_MT_content1);
		main_case->removeChildByTag(kSP_MT_content2);
		main_case->removeChildByTag(kSP_MT_content3);
		main_case->removeChildByTag(kSP_MT_content4);
		main_case->removeChildByTag(kSP_MT_content5);
		main_case->removeChildByTag(kSP_MT_content6);
	}
	
	recent_shop_code = t_code;
	
	if(recent_shop_code == kSC_character)
	{
		CCSize table_size = CCSizeMake(440, 180);
//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//		temp_back->setAnchorPoint(CCPointZero);
//		temp_back->setPosition(ccp(240-table_size.width/2.f, 127-table_size.height/2.f));
//		main_case->addChild(temp_back, kSP_Z_content);
		
		last_select_idx = -1;
		
		character_table	= CCTableView::create(this, table_size);
		character_table->setAnchorPoint(CCPointZero);
		character_table->setDirection(kCCScrollViewDirectionHorizontal);
		character_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		character_table->setPosition(ccp(240-table_size.width/2.f, 127-table_size.height/2.f));
		
		character_table->setDelegate(this);
		main_case->addChild(character_table, kSP_Z_content);
		character_table->setTouchPriority(-170-1);
	}
	else
	{
		string filename;
		if(recent_shop_code == kSC_ruby)
			filename = "shop_ruby_button.png";
		else if(recent_shop_code == kSC_gold)
			filename = "shop_gold_button.png";
		else if(recent_shop_code == kSC_heart)
			filename = "shop_heart_button.png";
		
		for(int i=1;i<=6;i++)
		{
			CCSprite* n_content = CCSprite::create(filename.c_str());
			addPriceReward(n_content, i);
			CCSprite* s_content = CCSprite::create(filename.c_str());
			s_content->setColor(ccGRAY);
			addPriceReward(s_content, i);
			
			CCMenuItem* content_item = CCMenuItemSprite::create(n_content, s_content, this, menu_selector(ShopPopup::menuAction));
			content_item->setTag(kSP_MT_content1 + i - 1);
			
			CCMenu* content_menu = CCMenu::createWithItem(content_item);
			content_menu->setPosition(getContentPosition(kSP_MT_content1 + i - 1));
			main_case->addChild(content_menu, kSP_Z_content, kSP_MT_content1 + i - 1);
			
			content_menu->setTouchPriority(-170-1);
		}
	}
}

enum CharacterCellZorder
{
	kCharacterCellZorder_back = 1,
	kCharacterCellZorder_content,
	kCharacterCellZorder_lock,
	kCharacterCellZorder_selected
};

void ShopPopup::cellAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag >= kSP_MT_characterUnlockBase) // unlock
	{
		bool is_unlock_enable = false;
		
		CCNode* menu_node = ((CCNode*)sender)->getParent();
		CCNode* cell_node = menu_node->getParent();
		int unlock_idx = ((CCTableViewCell*)cell_node)->getIdx();
		
		string condition_type = NSDS_GS(kSDS_GI_characterInfo_int1_purchaseInfo_type_s, unlock_idx+1);
		int condition_value = NSDS_GI(kSDS_GI_characterInfo_int1_purchaseInfo_value_i, unlock_idx+1);
		
		if(condition_type == "gold")
			is_unlock_enable = mySGD->getGold() >= condition_value;
		else if(condition_type == "ruby")
			is_unlock_enable = mySGD->getStar() >= condition_value;
		
		if(is_unlock_enable)
		{
			if(condition_type == "gold")
				mySGD->setGold(mySGD->getGold() - condition_value);
			else if(condition_type == "ruby")
				mySGD->setStar(mySGD->getStar() - condition_value);
			
			myDSH->setIntegerForKey(kDSH_Key_selectedCharacter, tag-kSP_MT_characterUnlockBase);
			myDSH->setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, tag-kSP_MT_characterUnlockBase, true);
			myDSH->saveUserData({kSaveUserData_Key_gold, kSaveUserData_Key_star, kSaveUserData_Key_character}, nullptr);
			
			if(last_select_idx != -1)
				character_table->updateCellAtIndex(last_select_idx);
			
			character_table->updateCellAtIndex(unlock_idx);
		}
		else
		{
			CCLog("not enough condition");
		}
		
		is_menu_enable = true;
	}
	else // select
	{
		myDSH->setIntegerForKey(kDSH_Key_selectedCharacter, tag-kSP_MT_characterBase);
		
		if(last_select_idx != -1)
			character_table->updateCellAtIndex(last_select_idx);
		
		CCNode* menu_node = ((CCNode*)sender)->getParent();
		CCNode* cell_node = menu_node->getParent();
		character_table->updateCellAtIndex(((CCTableViewCell*)cell_node)->getIdx());
		
		is_menu_enable = true;
	}
}

CCTableViewCell* ShopPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCSprite* character_back = CCSprite::create("character_back.png");
	character_back->setPosition(ccp(60, 88));
	cell->addChild(character_back, kCharacterCellZorder_back);
	
	CCLabelTTF* name_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, idx+1).c_str(), mySGD->getFont().c_str(), 13);
	name_label->setColor(ccBLACK);
	name_label->setPosition(ccp(60,152));
	cell->addChild(name_label, kCharacterCellZorder_content);
	
	auto character_pair = KS::loadCCBIForFullPath<CCSprite*>(this, StageImgLoader::sharedInstance()->getDocumentPath()+NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, idx+1)+".ccbi");
	
	CCSprite* character_img = character_pair.first;
	character_img->setPosition(ccp(60,100));
	cell->addChild(character_img, kCharacterCellZorder_content);
	character_pair.second->runAnimationsForSequenceNamed("shop");
	
	CCLabelTTF* comment_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_comment_s, idx+1).c_str(), mySGD->getFont().c_str(), 13);
	comment_label->setColor(ccBLACK);
	comment_label->setPosition(ccp(60,53));
	cell->addChild(comment_label, kCharacterCellZorder_content);
	
	if(idx > 0 && !myDSH->getBoolForKey(kDSH_Key_isCharacterUnlocked_int1, idx))
	{
		CCSprite* lock_img = CCSprite::create("character_lock.png");
		lock_img->setPosition(ccp(60,110));
		cell->addChild(lock_img, kCharacterCellZorder_lock, kCharacterCellZorder_lock);
		
		string condition_type = NSDS_GS(kSDS_GI_characterInfo_int1_purchaseInfo_type_s, idx+1);
		int condition_value = NSDS_GI(kSDS_GI_characterInfo_int1_purchaseInfo_value_i, idx+1);
		
		CCSprite* n_unlock = CCSprite::create("character_buy.png");
		CCLabelTTF* n_condition_label = CCLabelTTF::create(CCString::createWithFormat("%s %d", condition_type.c_str(), condition_value)->getCString(), mySGD->getFont().c_str(), 12);
		n_condition_label->setColor(ccBLACK);
		n_condition_label->setPosition(ccp(n_unlock->getContentSize().width/2.f, n_unlock->getContentSize().height/2.f));
		n_unlock->addChild(n_condition_label);
		
		CCSprite* s_unlock = CCSprite::create("character_buy.png");
		s_unlock->setColor(ccGRAY);
		CCLabelTTF* s_condition_label = CCLabelTTF::create(CCString::createWithFormat("%s %d", condition_type.c_str(), condition_value)->getCString(), mySGD->getFont().c_str(), 12);
		s_condition_label->setColor(ccBLACK);
		s_condition_label->setPosition(ccp(s_unlock->getContentSize().width/2.f, s_unlock->getContentSize().height/2.f));
		s_unlock->addChild(s_condition_label);
		
		
		CCMenuItem* unlock_item = CCMenuItemSprite::create(n_unlock, s_unlock, this, menu_selector(ShopPopup::cellAction));
		unlock_item->setTag(kSP_MT_characterUnlockBase+idx);
		
		CCMenu* unlock_menu = CCMenu::createWithItem(unlock_item);
		unlock_menu->setPosition(ccp(60,25));
		cell->addChild(unlock_menu, kCharacterCellZorder_content);
		
		unlock_menu->setTouchPriority(-170-2);
	}
	else
	{
		if(idx == myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))
		{
			last_select_idx = idx;
			CCSprite* selected_img = CCSprite::create("character_on.png");
			selected_img->setPosition(ccp(60, 88));
			cell->addChild(selected_img, kCharacterCellZorder_selected, kCharacterCellZorder_selected);
		}
		else
		{
			CCSprite* n_select = CCSprite::create("character_select.png");
			CCSprite* s_select = CCSprite::create("character_select.png");
			s_select->setColor(ccGRAY);
			
			CCMenuItem* select_item = CCMenuItemSprite::create(n_select, s_select, this, menu_selector(ShopPopup::cellAction));
			select_item->setTag(kSP_MT_characterBase+idx);
			
			CCMenu* select_menu = CCMenu::createWithItem(select_item);
			select_menu->setPosition(ccp(60,25));
			cell->addChild(select_menu, kCharacterCellZorder_content);
			
			select_menu->setTouchPriority(-170-2);
		}
	}
	
	return cell;
}
void ShopPopup::scrollViewDidScroll(CCScrollView* view)
{
	
}
void ShopPopup::scrollViewDidZoom(CCScrollView* view)
{
	
}
void ShopPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	CCLog("cell touched!!");
}
CCSize ShopPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(120, 176);
}
unsigned int ShopPopup::numberOfCellsInTableView(CCTableView *table)
{
	return NSDS_GI(kSDS_GI_characterCount_i);
}

// on "init" you need to initialize your instance
bool ShopPopup::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
	
	is_menu_enable = false;
	
	main_case = CCSprite::create("shop_back.png");
	main_case->setAnchorPoint(ccp(0.5,0.5));
	main_case->setPosition(ccp(240,-176));
	addChild(main_case, kSP_Z_back);
	
	
	CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
	CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(ShopPopup::menuAction));
	close_item->setTag(kSP_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kSP_MT_close));
	main_case->addChild(close_menu, kSP_Z_content);
	
	close_menu->setTouchPriority(-170-1);
	
	
	CCSprite* n_character = CCSprite::create("shop_character.png");
	CCSprite* s_character = CCSprite::create("shop_character.png");
	s_character->setColor(ccGRAY);
	
	CCMenuItem* character_item = CCMenuItemSprite::create(n_character, s_character, this, menu_selector(ShopPopup::menuAction));
	character_item->setTag(kSP_MT_character);
	
	CCMenu* character_menu = CCMenu::createWithItem(character_item);
	character_menu->setPosition(getContentPosition(kSP_MT_character));
	main_case->addChild(character_menu, kSP_Z_content);
	
	character_menu->setTouchPriority(-170-1);
	
	
	CCSprite* n_ruby = CCSprite::create("shop_ruby.png");
	CCSprite* s_ruby = CCSprite::create("shop_ruby.png");
	s_ruby->setColor(ccGRAY);
	
	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(ShopPopup::menuAction));
	ruby_item->setTag(kSP_MT_ruby);
	
	CCMenu* ruby_menu = CCMenu::createWithItem(ruby_item);
	ruby_menu->setPosition(getContentPosition(kSP_MT_ruby));
	main_case->addChild(ruby_menu, kSP_Z_content);
	
	ruby_menu->setTouchPriority(-170-1);
	
	
	CCSprite* n_gold = CCSprite::create("shop_gold.png");
	CCSprite* s_gold = CCSprite::create("shop_gold.png");
	s_gold->setColor(ccGRAY);
	
	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(ShopPopup::menuAction));
	gold_item->setTag(kSP_MT_gold);
	
	CCMenu* gold_menu = CCMenu::createWithItem(gold_item);
	gold_menu->setPosition(getContentPosition(kSP_MT_gold));
	main_case->addChild(gold_menu, kSP_Z_content);
	
	gold_menu->setTouchPriority(-170-1);
	
	
	CCSprite* n_heart = CCSprite::create("shop_heart.png");
	CCSprite* s_heart = CCSprite::create("shop_heart.png");
	s_heart->setColor(ccGRAY);
	
	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(ShopPopup::menuAction));
	heart_item->setTag(kSP_MT_heart);
	
	CCMenu* heart_menu = CCMenu::createWithItem(heart_item);
	heart_menu->setPosition(getContentPosition(kSP_MT_heart));
	main_case->addChild(heart_menu, kSP_Z_content);
	
	heart_menu->setTouchPriority(-170-1);
	
	recent_shop_code = kSC_empty;
	
	showPopup();
	
    return true;
}

void ShopPopup::showPopup()
{
	setTouchEnabled(true);
	CCMoveTo* main_move = CCMoveTo::create(0.3f, ccp(240,-176+320));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ShopPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void ShopPopup::endShowPopup()
{
	is_menu_enable = true;
}

void ShopPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,-176));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ShopPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void ShopPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	removeFromParent();
}

CCPoint ShopPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kSP_MT_close)
		return_value = ccp(440,262);
	else if(t_tag == kSP_MT_character)
		return_value = ccp(61,261);
	else if(t_tag == kSP_MT_ruby)
		return_value = ccp(150,261);
	else if(t_tag == kSP_MT_gold)
		return_value = ccp(239,261);
	else if(t_tag == kSP_MT_heart)
		return_value = ccp(328,261);
	else if(t_tag == kSP_MT_content1)
		return_value = ccp(93,182);
	else if(t_tag == kSP_MT_content2)
		return_value = ccp(240,182);
	else if(t_tag == kSP_MT_content3)
		return_value = ccp(387,182);
	else if(t_tag == kSP_MT_content4)
		return_value = ccp(93,72);
	else if(t_tag == kSP_MT_content5)
		return_value = ccp(240,72);
	else if(t_tag == kSP_MT_content6)
		return_value = ccp(387,72);
	
	return return_value;
}

void ShopPopup::resultSetUserData(Json::Value result_data)
{
	loading_layer->removeFromParent();
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLog("userdata was save to server");
	}
	else
	{
		CCLog("fail!! save userdata to server");
	}
}

void ShopPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	is_menu_enable = false;
	int tag = ((CCNode*)pSender)->getTag();
	
	if(tag == kSP_MT_close)
	{
		myDSH->saveUserData({kSaveUserData_Key_character}, nullptr);
		hidePopup();
	}
	else if(tag == kSP_MT_ruby)
	{
		setShopCode(kSC_ruby);
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_gold)
	{
		setShopCode(kSC_gold);
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_heart)
	{
		setShopCode(kSC_heart);
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_character)
	{
		setShopCode(kSC_character);
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_content1)
	{
		if(recent_shop_code == kSC_ruby)
		{
			CCLog("1100won!!!");
			
			loading_layer = LoadingLayer::create();
			addChild(loading_layer, kSP_Z_popup);
			
			mySGD->setStar(mySGD->getStar() + 10);
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= 10)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 10);
				mySGD->setGold(mySGD->getGold() + 10000);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= 5)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 5);
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + 5);
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_content2)
	{
		if(recent_shop_code == kSC_ruby)
		{
			CCLog("4900won!!!");
			
			loading_layer = LoadingLayer::create();
			addChild(loading_layer, kSP_Z_popup);
			
			mySGD->setStar(mySGD->getStar() + 50);
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= 20)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 20);
				mySGD->setGold(mySGD->getGold() + 20000);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= 10)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 10);
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + 10);
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_content3)
	{
		if(recent_shop_code == kSC_ruby)
		{
			CCLog("9800won!!!");
			
			loading_layer = LoadingLayer::create();
			addChild(loading_layer, kSP_Z_popup);
			
			mySGD->setStar(mySGD->getStar() + 100);
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= 50)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 50);
				mySGD->setGold(mySGD->getGold() + 50000);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= 20)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 20);
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + 20);
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_content4)
	{
		if(recent_shop_code == kSC_ruby)
		{
			CCLog("29800won!!!");
			
			loading_layer = LoadingLayer::create();
			addChild(loading_layer, kSP_Z_popup);
			
			mySGD->setStar(mySGD->getStar() + 300);
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= 100)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 100);
				mySGD->setGold(mySGD->getGold() + 100000);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= 40)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 40);
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + 40);
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_content5)
	{
		if(recent_shop_code == kSC_ruby)
		{
			CCLog("49800won!!!");
			
			loading_layer = LoadingLayer::create();
			addChild(loading_layer, kSP_Z_popup);
			
			mySGD->setStar(mySGD->getStar() + 500);
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= 300)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 300);
				mySGD->setGold(mySGD->getGold() + 300000);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= 75)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 75);
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + 75);
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_content6)
	{
		if(recent_shop_code == kSC_ruby)
		{
			CCLog("99800won!!!");
			
			loading_layer = LoadingLayer::create();
			addChild(loading_layer, kSP_Z_popup);
			
			mySGD->setStar(mySGD->getStar() + 1000);
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= 500)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 500);
				mySGD->setGold(mySGD->getGold() + 500000);
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= 100)
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - 100);
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + 100);
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				CCLog("not enough ruby!!!");
			}
		}
		is_menu_enable = true;
	}
}

bool ShopPopup::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void ShopPopup::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void ShopPopup::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void ShopPopup::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void ShopPopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
