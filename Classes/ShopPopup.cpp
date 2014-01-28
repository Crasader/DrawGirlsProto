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
#include "MainFlowScene.h"
#include "PuzzleScene.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
#include "ASPopupView.h"
#include "TouchSuctionLayer.h"

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
	kSP_MT_card,
	kSP_MT_content1,
	kSP_MT_content2,
	kSP_MT_content3,
	kSP_MT_content4,
	kSP_MT_content5,
	kSP_MT_content6,
	kSP_MT_cardHigh,
	kSP_MT_cardMid,
	kSP_MT_cardLow,
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
		return_value = "10개 구입";
	else if(t_code == "rubyFromWon2")
		return_value = "50개 구입";
	else if(t_code == "rubyFromWon3")
		return_value = "100개 구입";
	else if(t_code == "rubyFromWon4")
		return_value = "300개 구입";
	else if(t_code == "rubyFromWon5")
		return_value = "500개 구입";
	else if(t_code == "rubyFromWon6")
		return_value = "1000개 구입";
	else if(t_code == "rubyToGold1")
		return_value = "10";
	else if(t_code == "rubyToGold2")
		return_value = "20";
	else if(t_code == "rubyToGold3")
		return_value = "50";
	else if(t_code == "rubyToGold4")
		return_value = "100";
	else if(t_code == "rubyToGold5")
		return_value = "300";
	else if(t_code == "rubyToGold6")
		return_value = "500";
	else if(t_code == "goldFromRuby1")
		return_value = "1만골드 구입";
	else if(t_code == "goldFromRuby2")
		return_value = "2만골드 구입";
	else if(t_code == "goldFromRuby3")
		return_value = "5만골드 구입";
	else if(t_code == "goldFromRuby4")
		return_value = "10만골드 구입";
	else if(t_code == "goldFromRuby5")
		return_value = "30만골드 구입";
	else if(t_code == "goldFromRuby6")
		return_value = "50만골드 구입";
	else if(t_code == "rubyToHeart1")
		return_value = "5";
	else if(t_code == "rubyToHeart2")
		return_value = "10";
	else if(t_code == "rubyToHeart3")
		return_value = "20";
	else if(t_code == "rubyToHeart4")
		return_value = "40";
	else if(t_code == "rubyToHeart5")
		return_value = "75";
	else if(t_code == "rubyToHeart6")
		return_value = "100";
	else if(t_code == "heartFromRuby1")
		return_value = "5개 구입";
	else if(t_code == "heartFromRuby2")
		return_value = "10개 구입";
	else if(t_code == "heartFromRuby3")
		return_value = "20개 구입";
	else if(t_code == "heartFromRuby4")
		return_value = "40개 구입";
	else if(t_code == "heartFromRuby5")
		return_value = "75개 구입";
	else if(t_code == "heartFromRuby6")
		return_value = "100개 구입";
	
	return return_value;
}

void ShopPopup::addPriceReward(CCNode *t_target, int t_number)
{
	if(recent_shop_code == kSC_ruby)
	{
		CCLabelTTF* ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("rubyFromWon%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		ruby_label->setColor(ccORANGE);
		ruby_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f-5));
		t_target->addChild(ruby_label);
	}
	else if(recent_shop_code == kSC_gold)
	{
		CCLabelTTF* gold_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("goldFromRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		gold_label->setColor(ccORANGE);
		gold_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f-5));
		t_target->addChild(gold_label);
	}
	else if(recent_shop_code == kSC_heart)
	{
		CCLabelTTF* heart_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("heartFromRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		heart_label->setColor(ccORANGE);
		heart_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f-5));
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

void ShopPopup::setShopBeforeCode(ShopBeforeCode t_code)
{
	before_code = t_code;
}

void ShopPopup::setShopCode(ShopCode t_code)
{
	if(t_code == recent_shop_code)
		return;
	
	if(recent_shop_code == kSC_character)
	{
		character_table->removeFromParent();
		suction->removeFromParent();
	}
	else if(recent_shop_code != kSC_empty)
	{
		if(recent_shop_code != kSC_card)
		{
			main_case->removeChildByTag(kSP_MT_content1);
			main_case->removeChildByTag(kSP_MT_content2);
			main_case->removeChildByTag(kSP_MT_content3);
			main_case->removeChildByTag(kSP_MT_content4);
			main_case->removeChildByTag(kSP_MT_content5);
			main_case->removeChildByTag(kSP_MT_content6);
		}
		else
		{
			main_case->removeChildByTag(kSP_MT_cardHigh);
			main_case->removeChildByTag(kSP_MT_cardMid);
			main_case->removeChildByTag(kSP_MT_cardLow);
		}
	}
	
	recent_shop_code = t_code;
	
	setCharacterMenu();
	setCardMenu();
	setRubyMenu();
	setGoldMenu();
	setHeartMenu();
	
	if(recent_shop_code == kSC_character)
	{
		CCSize table_size = CCSizeMake(423, 208);
		CCPoint table_position = ccp(241-table_size.width/2.f, 25);
//		CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//		temp_back->setOpacity(100);
//		temp_back->setAnchorPoint(CCPointZero);
//		temp_back->setPosition(table_position);
//		main_case->addChild(temp_back, kSP_Z_content);
		
		last_select_idx = -1;
		
		character_table	= CCTableView::create(this, table_size);
		character_table->setAnchorPoint(CCPointZero);
		character_table->setDirection(kCCScrollViewDirectionHorizontal);
		character_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		character_table->setPosition(table_position);
		
		character_table->setDelegate(this);
		main_case->addChild(character_table, kSP_Z_content);
		character_table->setTouchPriority(-170-3);
		
		suction = TouchSuctionLayer::create(-172);
		suction->setNotSwallowRect(CCRectMake(table_position.x, table_position.y, table_size.width, table_size.height));
		suction->setTouchEnabled(true);
		addChild(suction);
	}
	else if(recent_shop_code != kSC_card)
	{
		string filename;
		if(recent_shop_code == kSC_ruby)
			filename = "shop_ruby%d.png";
		else if(recent_shop_code == kSC_gold)
			filename = "shop_gold%d.png";
		else if(recent_shop_code == kSC_heart)
			filename = "shop_coin%d.png";
		
		for(int i=1;i<=6;i++)
		{
			CCSprite* main_content = CCSprite::create("shop_button_back.png");
			main_content->setPosition(getContentPosition(kSP_MT_content1 + i - 1));
			main_case->addChild(main_content, kSP_Z_content, kSP_MT_content1 + i - 1);
			
			CCSprite* inner = CCSprite::create(CCString::createWithFormat(filename.c_str(), i)->getCString());
			inner->setPosition(ccp(main_content->getContentSize().width/2.f, main_content->getContentSize().height/2.f));
			addPriceReward(inner, i);
			main_content->addChild(inner);
			
			CCSprite* tab = CCSprite::create("shop_tab.png");
			tab->setPosition(ccp(97,83));
			main_content->addChild(tab);
			
			
			CCSprite* n_button = CCSprite::create("buy_button.png");
			CCSprite* s_button = CCSprite::create("buy_button.png");
			s_button->setColor(ccGRAY);
			
			if(recent_shop_code == kSC_ruby)
			{
				CCLabelTTF* n_won_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("wonToRuby%d", i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				n_won_label->setPosition(ccp(n_button->getContentSize().width/2.f, n_button->getContentSize().height/2.f));
				n_button->addChild(n_won_label);
				
				CCLabelTTF* s_won_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("wonToRuby%d", i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				s_won_label->setPosition(ccp(s_button->getContentSize().width/2.f, s_button->getContentSize().height/2.f));
				s_button->addChild(s_won_label);
			}
			else if(recent_shop_code == kSC_gold)
			{
				CCSprite* n_ruby_img = CCSprite::create("price_ruby_img.png");
				n_ruby_img->setPosition(ccp(n_button->getContentSize().width/2.f-30, n_button->getContentSize().height/2.f));
				n_button->addChild(n_ruby_img);
				
				CCLabelTTF* n_ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("rubyToGold%d", i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				n_ruby_label->setPosition(ccp(n_button->getContentSize().width/2.f+10, n_button->getContentSize().height/2.f));
				n_button->addChild(n_ruby_label);
				
				CCSprite* s_ruby_img = CCSprite::create("price_ruby_img.png");
				s_ruby_img->setPosition(ccp(s_button->getContentSize().width/2.f-30, s_button->getContentSize().height/2.f));
				s_ruby_img->setColor(ccGRAY);
				s_button->addChild(s_ruby_img);
				
				CCLabelTTF* s_ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("rubyToGold%d", i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				s_ruby_label->setPosition(ccp(s_button->getContentSize().width/2.f+10, s_button->getContentSize().height/2.f));
				s_button->addChild(s_ruby_label);
			}
			else if(recent_shop_code == kSC_heart)
			{
				CCSprite* n_ruby_img = CCSprite::create("price_ruby_img.png");
				n_ruby_img->setPosition(ccp(n_button->getContentSize().width/2.f-30, n_button->getContentSize().height/2.f));
				n_button->addChild(n_ruby_img);
				
				CCLabelTTF* n_ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("rubyToHeart%d", i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				n_ruby_label->setPosition(ccp(n_button->getContentSize().width/2.f+10, n_button->getContentSize().height/2.f));
				n_button->addChild(n_ruby_label);
				
				CCSprite* s_ruby_img = CCSprite::create("price_ruby_img.png");
				s_ruby_img->setPosition(ccp(s_button->getContentSize().width/2.f-30, s_button->getContentSize().height/2.f));
				s_ruby_img->setColor(ccGRAY);
				s_button->addChild(s_ruby_img);
				
				CCLabelTTF* s_ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat("rubyToHeart%d", i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				s_ruby_label->setPosition(ccp(s_button->getContentSize().width/2.f+10, s_button->getContentSize().height/2.f));
				s_button->addChild(s_ruby_label);
			}
			
			CCMenuItem* content_item = CCMenuItemSprite::create(n_button, s_button, this, menu_selector(ShopPopup::menuAction));
			content_item->setTag(kSP_MT_content1 + i - 1);
			
			CCMenu* content_menu = CCMenu::createWithItem(content_item);
			content_menu->setPosition(ccp(main_content->getContentSize().width/2.f, 17));
			main_content->addChild(content_menu);
			
			content_menu->setTouchPriority(-170-4);
		}
	}
	else
	{
		setCardBuyMenu(ccp(100,128), kSP_MT_cardHigh, "shop_card_high.png", "price_ruby_img.png", card_price_high.getV());
		setCardBuyMenu(ccp(240,128), kSP_MT_cardMid, "shop_card_mid.png", "price_gold_img.png", card_price_mid.getV());
		setCardBuyMenu(ccp(380,128), kSP_MT_cardLow, "shop_card_low.png", "price_candy_img.png", card_price_low.getV());
	}
}

void ShopPopup::setCardBuyMenu(CCPoint t_point, int t_tag, string inner_filename, string type_filename, int price_value)
{
	CCSprite* t_buy_back = CCSprite::create("buy_back.png");
	t_buy_back->setPosition(t_point);
	main_case->addChild(t_buy_back, kSP_Z_content, t_tag);
	CCSprite* t_inner = CCSprite::create(inner_filename.c_str());
	t_inner->setPosition(ccp(t_buy_back->getContentSize().width/2.f, t_buy_back->getContentSize().height/2.f+15));
	t_buy_back->addChild(t_inner);
	
	CCSprite* t_tab = CCSprite::create("shop_tab.png");
	t_tab->setPosition(ccp(94,192));
	t_buy_back->addChild(t_tab);
	
	CCSprite* n_t_buy_button = CCSprite::create("buy_button.png");
	CCSprite* n_t_type = CCSprite::create(type_filename.c_str());
	n_t_type->setPosition(ccp(n_t_buy_button->getContentSize().width/2.f-25, n_t_buy_button->getContentSize().height/2.f));
	n_t_buy_button->addChild(n_t_type);
	CCLabelTTF* n_t_price = CCLabelTTF::create(CCString::createWithFormat("%d", price_value)->getCString(), mySGD->getFont().c_str(), 12);
	n_t_price->setPosition(ccp(n_t_buy_button->getContentSize().width/2.f+10, n_t_buy_button->getContentSize().height/2.f));
	n_t_buy_button->addChild(n_t_price);
	
	CCSprite* s_t_buy_button = CCSprite::create("buy_button.png");
	s_t_buy_button->setColor(ccGRAY);
	CCSprite* s_t_type = CCSprite::create(type_filename.c_str());
	s_t_type->setColor(ccGRAY);
	s_t_type->setPosition(ccp(s_t_buy_button->getContentSize().width/2.f-25, s_t_buy_button->getContentSize().height/2.f));
	s_t_buy_button->addChild(s_t_type);
	CCLabelTTF* s_t_price = CCLabelTTF::create(CCString::createWithFormat("%d", price_value)->getCString(), mySGD->getFont().c_str(), 12);
	s_t_price->setPosition(ccp(s_t_buy_button->getContentSize().width/2.f+10, s_t_buy_button->getContentSize().height/2.f));
	s_t_buy_button->addChild(s_t_price);
	
	CCMenuItem* t_buy_item = CCMenuItemSprite::create(n_t_buy_button, s_t_buy_button, this, menu_selector(ShopPopup::menuAction));
	t_buy_item->setTag(t_tag);
	
	CCMenu* t_buy_menu = CCMenu::createWithItem(t_buy_item);
	t_buy_menu->setPosition(ccp(t_buy_back->getContentSize().width/2.f, t_buy_back->getContentSize().height/2.f-79));
	t_buy_back->addChild(t_buy_menu);
	t_buy_menu->setTouchPriority(-170-4);
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
			if(condition_type == "gold")
				addChild(ASPopupView::getCommonNoti(-210, "골드가 부족합니다."), kSP_Z_popup);
			else if(condition_type == "ruby")
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
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
	
	CCSprite* character_back = CCSprite::create("buy_back.png");
	character_back->setPosition(ccp(62, 103));
	cell->addChild(character_back, kCharacterCellZorder_back);
	
	CCLabelTTF* name_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, idx+1).c_str(), mySGD->getFont().c_str(), 10);
	name_label->setColor(ccORANGE);
	name_label->setPosition(ccp(62,80));
	cell->addChild(name_label, kCharacterCellZorder_content);
	
	auto character_pair = KS::loadCCBIForFullPath<CCSprite*>(this, StageImgLoader::sharedInstance()->getDocumentPath()+NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, idx+1)+".ccbi");
	
	CCSprite* character_img = character_pair.first;
	character_img->setPosition(ccp(47,120));
	cell->addChild(character_img, kCharacterCellZorder_content);
	character_pair.second->runAnimationsForSequenceNamed("shop");
	
	CCLabelTTF* comment_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_comment_s, idx+1).c_str(), mySGD->getFont().c_str(), 11);
	comment_label->setPosition(ccp(62,58));
	cell->addChild(comment_label, kCharacterCellZorder_content);
	
	CCSprite* t_tab = CCSprite::create("shop_tab.png");
	t_tab->setPosition(ccp(94,192));
	cell->addChild(t_tab, kCharacterCellZorder_content);
	
	if(idx > 0 && !myDSH->getBoolForKey(kDSH_Key_isCharacterUnlocked_int1, idx))
	{
		CCSprite* lock_img = CCSprite::create("character_lock.png");
		lock_img->setPosition(ccp(62,103));
		cell->addChild(lock_img, kCharacterCellZorder_lock, kCharacterCellZorder_lock);
		
		string condition_type = NSDS_GS(kSDS_GI_characterInfo_int1_purchaseInfo_type_s, idx+1);
		int condition_value = NSDS_GI(kSDS_GI_characterInfo_int1_purchaseInfo_value_i, idx+1);
		
		string type_filename;
		if(condition_type == "ruby")
			type_filename = "price_ruby_img.png";
		else if(condition_type == "gold")
			type_filename = "price_gold_img.png";
		else if(condition_type == "social")
			type_filename = "price_candy_img.png";
		
		CCSprite* n_unlock = CCSprite::create("buy_button.png");
		CCSprite* n_t_type = CCSprite::create(type_filename.c_str());
		n_t_type->setPosition(ccp(n_unlock->getContentSize().width/2.f-25, n_unlock->getContentSize().height/2.f));
		n_unlock->addChild(n_t_type);
		CCLabelTTF* n_t_price = CCLabelTTF::create(CCString::createWithFormat("%d", condition_value)->getCString(), mySGD->getFont().c_str(), 12);
		n_t_price->setPosition(ccp(n_unlock->getContentSize().width/2.f+10, n_unlock->getContentSize().height/2.f));
		n_unlock->addChild(n_t_price);
		
		
		CCSprite* s_unlock = CCSprite::create("buy_button.png");
		s_unlock->setColor(ccGRAY);
		CCSprite* s_t_type = CCSprite::create(type_filename.c_str());
		s_t_type->setPosition(ccp(s_unlock->getContentSize().width/2.f-25, s_unlock->getContentSize().height/2.f));
		s_unlock->addChild(s_t_type);
		CCLabelTTF* s_t_price = CCLabelTTF::create(CCString::createWithFormat("%d", condition_value)->getCString(), mySGD->getFont().c_str(), 12);
		s_t_price->setPosition(ccp(s_unlock->getContentSize().width/2.f+10, s_unlock->getContentSize().height/2.f));
		s_unlock->addChild(s_t_price);
		
		
		CCMenuItem* unlock_item = CCMenuItemSprite::create(n_unlock, s_unlock, this, menu_selector(ShopPopup::cellAction));
		unlock_item->setTag(kSP_MT_characterUnlockBase+idx);
		
		CCMenu* unlock_menu = CCMenu::createWithItem(unlock_item);
		unlock_menu->setPosition(ccp(62,23));
		cell->addChild(unlock_menu, kCharacterCellZorder_selected);
		
		unlock_menu->setTouchPriority(-170-1);
	}
	else
	{
		if(idx == myDSH->getIntegerForKey(kDSH_Key_selectedCharacter))
		{
			last_select_idx = idx;
			CCSprite* selected_img = CCSprite::create("character_on.png");
			selected_img->setPosition(ccp(62, 103));
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
			select_menu->setPosition(ccp(62,23));
			cell->addChild(select_menu, kCharacterCellZorder_selected);
			
			select_menu->setTouchPriority(-170-1);
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
	return CCSizeMake(124, 206);
}
unsigned int ShopPopup::numberOfCellsInTableView(CCTableView *table)
{
	return server_character_count;
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
	
	server_character_count = NSDS_GI(kSDS_GI_characterCount_i);
	
	is_menu_enable = false;
	
	main_case = CCSprite::create("shop_back.png");
	main_case->setAnchorPoint(CCPointZero);
	main_case->setPosition(ccp(0,-320));
	addChild(main_case, kSP_Z_back);
	
	
	CCSprite* n_close = CCSprite::create("cardchange_cancel.png");
	CCSprite* s_close = CCSprite::create("cardchange_cancel.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(ShopPopup::menuAction));
	close_item->setTag(kSP_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kSP_MT_close));
	main_case->addChild(close_menu, kSP_Z_content);
	
	close_menu->setTouchPriority(-170-4);
	
	character_menu = NULL;
	card_menu = NULL;
	ruby_menu = NULL;
	gold_menu = NULL;
	heart_menu = NULL;
	
	recent_shop_code = kSC_empty;
	
	index_to_ruby.push_back(KSProtectVar<int>(1100));
	index_to_ruby.push_back(KSProtectVar<int>(4900));
	index_to_ruby.push_back(KSProtectVar<int>(9800));
	index_to_ruby.push_back(KSProtectVar<int>(29800));
	index_to_ruby.push_back(KSProtectVar<int>(49800));
	index_to_ruby.push_back(KSProtectVar<int>(99800));
	
	index_to_gold.push_back(KSProtectVar<int>(10));
	index_to_gold.push_back(KSProtectVar<int>(20));
	index_to_gold.push_back(KSProtectVar<int>(50));
	index_to_gold.push_back(KSProtectVar<int>(100));
	index_to_gold.push_back(KSProtectVar<int>(300));
	index_to_gold.push_back(KSProtectVar<int>(500));
	
	index_to_heart.push_back(KSProtectVar<int>(5));
	index_to_heart.push_back(KSProtectVar<int>(10));
	index_to_heart.push_back(KSProtectVar<int>(20));
	index_to_heart.push_back(KSProtectVar<int>(40));
	index_to_heart.push_back(KSProtectVar<int>(75));
	index_to_heart.push_back(KSProtectVar<int>(100));
	
	cash_to_ruby[index_to_ruby[0].getV()] = KSProtectVar<int>(10);
	cash_to_ruby[index_to_ruby[1].getV()] = KSProtectVar<int>(50);
	cash_to_ruby[index_to_ruby[2].getV()] = KSProtectVar<int>(100);
	cash_to_ruby[index_to_ruby[3].getV()] = KSProtectVar<int>(300);
	cash_to_ruby[index_to_ruby[4].getV()] = KSProtectVar<int>(500);
	cash_to_ruby[index_to_ruby[5].getV()] = KSProtectVar<int>(1000);
	
	ruby_to_gold[index_to_gold[0].getV()] = KSProtectVar<int>(10000);
	ruby_to_gold[index_to_gold[1].getV()] = KSProtectVar<int>(20000);
	ruby_to_gold[index_to_gold[2].getV()] = KSProtectVar<int>(50000);
	ruby_to_gold[index_to_gold[3].getV()] = KSProtectVar<int>(100000);
	ruby_to_gold[index_to_gold[4].getV()] = KSProtectVar<int>(300000);
	ruby_to_gold[index_to_gold[5].getV()] = KSProtectVar<int>(500000);
	
	ruby_to_heart[index_to_heart[0].getV()] = KSProtectVar<int>(5);
	ruby_to_heart[index_to_heart[1].getV()] = KSProtectVar<int>(10);
	ruby_to_heart[index_to_heart[2].getV()] = KSProtectVar<int>(20);
	ruby_to_heart[index_to_heart[3].getV()] = KSProtectVar<int>(40);
	ruby_to_heart[index_to_heart[4].getV()] = KSProtectVar<int>(75);
	ruby_to_heart[index_to_heart[5].getV()] = KSProtectVar<int>(100);
	
	card_price_high = KSProtectVar<int>(100);
	card_price_mid = KSProtectVar<int>(10000);
	card_price_low = KSProtectVar<int>(10);
	
	showPopup();
	
    return true;
}

void ShopPopup::setCharacterMenu()
{
	if(character_menu)
	{
		character_menu->removeFromParent();
		character_menu = NULL;
	}
	
	if(recent_shop_code != kSC_character)
	{
		CCSprite* n_character = CCSprite::create("shop_character.png");
		CCSprite* s_character = CCSprite::create("shop_character.png");
		s_character->setColor(ccGRAY);
		
		CCMenuItem* character_item = CCMenuItemSprite::create(n_character, s_character, this, menu_selector(ShopPopup::menuAction));
		character_item->setTag(kSP_MT_character);
		
		character_menu = CCMenu::createWithItem(character_item);
		character_menu->setPosition(getContentPosition(kSP_MT_character));
		main_case->addChild(character_menu, kSP_Z_content);
		
		((CCMenu*)character_menu)->setTouchPriority(-170-4);
	}
}

void ShopPopup::setCardMenu()
{
	if(card_menu)
	{
		card_menu->removeFromParent();
		card_menu = NULL;
	}
	
	if(recent_shop_code != kSC_card)
	{
		CCSprite* n_card = CCSprite::create("shop_card.png");
		CCSprite* s_card = CCSprite::create("shop_card.png");
		s_card->setColor(ccGRAY);
		
		CCMenuItem* card_item = CCMenuItemSprite::create(n_card, s_card, this, menu_selector(ShopPopup::menuAction));
		card_item->setTag(kSP_MT_card);
		
		card_menu = CCMenu::createWithItem(card_item);
		card_menu->setPosition(getContentPosition(kSP_MT_card));
		main_case->addChild(card_menu, kSP_Z_content);
		
		((CCMenu*)card_menu)->setTouchPriority(-170-4);
	}
}

void ShopPopup::setRubyMenu()
{
	if(ruby_menu)
	{
		ruby_menu->removeFromParent();
		ruby_menu = NULL;
	}
	
	if(recent_shop_code != kSC_ruby)
	{
		CCSprite* n_ruby = CCSprite::create("shop_ruby.png");
		CCSprite* s_ruby = CCSprite::create("shop_ruby.png");
		s_ruby->setColor(ccGRAY);
		
		CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(ShopPopup::menuAction));
		ruby_item->setTag(kSP_MT_ruby);
		
		ruby_menu = CCMenu::createWithItem(ruby_item);
		ruby_menu->setPosition(getContentPosition(kSP_MT_ruby));
		main_case->addChild(ruby_menu, kSP_Z_content);
		
		((CCMenu*)ruby_menu)->setTouchPriority(-170-4);
	}
}

void ShopPopup::setGoldMenu()
{
	if(gold_menu)
	{
		gold_menu->removeFromParent();
		gold_menu = NULL;
	}
	
	if(recent_shop_code != kSC_gold)
	{
		CCSprite* n_gold = CCSprite::create("shop_gold.png");
		CCSprite* s_gold = CCSprite::create("shop_gold.png");
		s_gold->setColor(ccGRAY);
		
		CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(ShopPopup::menuAction));
		gold_item->setTag(kSP_MT_gold);
		
		gold_menu = CCMenu::createWithItem(gold_item);
		gold_menu->setPosition(getContentPosition(kSP_MT_gold));
		main_case->addChild(gold_menu, kSP_Z_content);
		
		((CCMenu*)gold_menu)->setTouchPriority(-170-4);
	}
}

void ShopPopup::setHeartMenu()
{
	if(heart_menu)
	{
		heart_menu->removeFromParent();
		heart_menu = NULL;
	}
	
	if(recent_shop_code != kSC_heart)
	{
		CCSprite* n_heart = CCSprite::create("shop_heart.png");
		CCSprite* s_heart = CCSprite::create("shop_heart.png");
		s_heart->setColor(ccGRAY);
		
		CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(ShopPopup::menuAction));
		heart_item->setTag(kSP_MT_heart);
		
		heart_menu = CCMenu::createWithItem(heart_item);
		heart_menu->setPosition(getContentPosition(kSP_MT_heart));
		main_case->addChild(heart_menu, kSP_Z_content);
		
		((CCMenu*)heart_menu)->setTouchPriority(-170-4);
	}
}

void ShopPopup::showPopup()
{
	setTouchEnabled(true);
	CCMoveTo* main_move = CCMoveTo::create(0.3f, ccp(0,0));
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
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(0,-320));
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
		return_value = ccp(453,260);
	else if(t_tag == kSP_MT_character)
		return_value = ccp(59,259);
	else if(t_tag == kSP_MT_card)
		return_value = ccp(141.5f,259);
	else if(t_tag == kSP_MT_ruby)
		return_value = ccp(224,259);
	else if(t_tag == kSP_MT_gold)
		return_value = ccp(306,259);
	else if(t_tag == kSP_MT_heart)
		return_value = ccp(388.5f,259);
	else if(t_tag == kSP_MT_content1)
		return_value = ccp(100,180);
	else if(t_tag == kSP_MT_content2)
		return_value = ccp(240,180);
	else if(t_tag == kSP_MT_content3)
		return_value = ccp(380,180);
	else if(t_tag == kSP_MT_content4)
		return_value = ccp(100,75);
	else if(t_tag == kSP_MT_content5)
		return_value = ccp(240,75);
	else if(t_tag == kSP_MT_content6)
		return_value = ccp(380,75);
	
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
	else if(tag == kSP_MT_card)
	{
		setShopCode(kSC_card);
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_content1)
	{
		if(recent_shop_code == kSC_ruby)
		{
			CCLog("1100won!!!");
			
			loading_layer = LoadingLayer::create();
			addChild(loading_layer, kSP_Z_popup);
			
			mySGD->setStar(mySGD->getStar() + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= index_to_gold[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_gold[tag-kSP_MT_content1].getV());
				mySGD->setGold(mySGD->getGold() + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= index_to_heart[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_heart[tag-kSP_MT_content1].getV());
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				if(before_code == kShopBeforeCode_stagesetting)
					((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_mainflow)
					((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_puzzle)
					((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
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
			
			mySGD->setStar(mySGD->getStar() + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= index_to_gold[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_gold[tag-kSP_MT_content1].getV());
				mySGD->setGold(mySGD->getGold() + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= index_to_heart[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_heart[tag-kSP_MT_content1].getV());
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				if(before_code == kShopBeforeCode_stagesetting)
					((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_mainflow)
					((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_puzzle)
					((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
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
			
			mySGD->setStar(mySGD->getStar() + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= index_to_gold[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_gold[tag-kSP_MT_content1].getV());
				mySGD->setGold(mySGD->getGold() + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= index_to_heart[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_heart[tag-kSP_MT_content1].getV());
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				if(before_code == kShopBeforeCode_stagesetting)
					((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_mainflow)
					((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_puzzle)
					((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
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
			
			mySGD->setStar(mySGD->getStar() + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= index_to_gold[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_gold[tag-kSP_MT_content1].getV());
				mySGD->setGold(mySGD->getGold() + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= index_to_heart[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_heart[tag-kSP_MT_content1].getV());
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				if(before_code == kShopBeforeCode_stagesetting)
					((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_mainflow)
					((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_puzzle)
					((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
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
			
			mySGD->setStar(mySGD->getStar() + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= index_to_gold[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_gold[tag-kSP_MT_content1].getV());
				mySGD->setGold(mySGD->getGold() + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= index_to_heart[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_heart[tag-kSP_MT_content1].getV());
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				if(before_code == kShopBeforeCode_stagesetting)
					((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_mainflow)
					((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_puzzle)
					((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
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
			
			mySGD->setStar(mySGD->getStar() + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
			
			vector<SaveUserData_Key> save_userdata_list;
			
			save_userdata_list.push_back(kSaveUserData_Key_star);
			
			myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
		}
		else if(recent_shop_code == kSC_gold)
		{
			if(mySGD->getStar() >= index_to_gold[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_gold[tag-kSP_MT_content1].getV());
				mySGD->setGold(mySGD->getGold() + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				save_userdata_list.push_back(kSaveUserData_Key_gold);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
				CCLog("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
			if(mySGD->getStar() >= index_to_heart[tag-kSP_MT_content1].getV())
			{
				loading_layer = LoadingLayer::create();
				addChild(loading_layer, kSP_Z_popup);
				
				mySGD->setStar(mySGD->getStar() - index_to_heart[tag-kSP_MT_content1].getV());
				myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
				
				CCNode* target_parent = target_heartTime->getParent();
				CCPoint heart_time_position = target_heartTime->getPosition();
				int heart_time_tag = target_heartTime->getTag();
				
				target_heartTime->removeFromParent();
				target_heartTime = HeartTime::create();
				target_heartTime->setPosition(heart_time_position);
				target_parent->addChild(target_heartTime, 0, heart_time_tag);
				if(before_code == kShopBeforeCode_stagesetting)
					((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_mainflow)
					((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				else if(before_code == kShopBeforeCode_puzzle)
					((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
				
				vector<SaveUserData_Key> save_userdata_list;
				
				save_userdata_list.push_back(kSaveUserData_Key_star);
				
				myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
			}
			else
			{
				addChild(ASPopupView::getCommonNoti(-210, "루비가 부족합니다."), kSP_Z_popup);
				CCLog("not enough ruby!!!");
			}
		}
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_cardHigh)
	{
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_cardMid)
	{
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_cardLow)
	{
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
