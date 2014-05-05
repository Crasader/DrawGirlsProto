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
#include "StageImgLoader.h"
#include "CardCase.h"
#include "CardAnimations.h"
#include "StartSettingScene.h"
#include "CommonButton.h"
#include "ScrollMenu.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "FirstPurchasePopup.h"

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
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 0);
	else if(t_code == "wonToRuby2")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 1);
	else if(t_code == "wonToRuby3")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 2);
	else if(t_code == "wonToRuby4")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 3);
	else if(t_code == "wonToRuby5")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 4);
	else if(t_code == "wonToRuby6")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 5);
	else if(t_code == "rubyFromWon1")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 0);
	else if(t_code == "rubyFromWon2")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 1);
	else if(t_code == "rubyFromWon3")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 2);
	else if(t_code == "rubyFromWon4")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 3);
	else if(t_code == "rubyFromWon5")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 4);
	else if(t_code == "rubyFromWon6")
		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 5);
	else if(t_code == "rubyToGold1")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 0);
	else if(t_code == "rubyToGold2")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 1);
	else if(t_code == "rubyToGold3")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 2);
	else if(t_code == "rubyToGold4")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 3);
	else if(t_code == "rubyToGold5")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 4);
	else if(t_code == "rubyToGold6")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 5);
	else if(t_code == "goldFromRuby1")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 0);
	else if(t_code == "goldFromRuby2")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 1);
	else if(t_code == "goldFromRuby3")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 2);
	else if(t_code == "goldFromRuby4")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 3);
	else if(t_code == "goldFromRuby5")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 4);
	else if(t_code == "goldFromRuby6")
		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 5);
	else if(t_code == "rubyToHeart1")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 0);
	else if(t_code == "rubyToHeart2")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 1);
	else if(t_code == "rubyToHeart3")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 2);
	else if(t_code == "rubyToHeart4")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 3);
	else if(t_code == "rubyToHeart5")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 4);
	else if(t_code == "rubyToHeart6")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 5);
	else if(t_code == "heartFromRuby1")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 0);
	else if(t_code == "heartFromRuby2")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 1);
	else if(t_code == "heartFromRuby3")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 2);
	else if(t_code == "heartFromRuby4")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 3);
	else if(t_code == "heartFromRuby5")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 4);
	else if(t_code == "heartFromRuby6")
		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 5);
	
	return return_value;
}

void ShopPopup::addPriceReward(CCNode *t_target, int t_number)
{
	if(recent_shop_code == kSC_ruby)
	{
		KSLabelTTF* ruby_label = KSLabelTTF::create(getPriceData(CCString::createWithFormat("rubyFromWon%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		ruby_label->setColor(ccc3(255, 230, 0));
		ruby_label->enableOuterStroke(ccc3(0, 0, 0), 1);
		ruby_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f-5));
		t_target->addChild(ruby_label);
	}
	else if(recent_shop_code == kSC_gold)
	{
		KSLabelTTF* gold_label = KSLabelTTF::create(getPriceData(CCString::createWithFormat("goldFromRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		gold_label->setColor(ccc3(255, 230, 0));
		gold_label->enableOuterStroke(ccc3(0, 0, 0), 1);
		gold_label->setPosition(ccp(t_target->getContentSize().width/2.f, t_target->getContentSize().height/2.f-5));
		t_target->addChild(gold_label);
	}
	else if(recent_shop_code == kSC_heart)
	{
		KSLabelTTF* heart_label = KSLabelTTF::create(getPriceData(CCString::createWithFormat("heartFromRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
		heart_label->setColor(ccc3(255, 230, 0));
		heart_label->enableOuterStroke(ccc3(0, 0, 0), 1);
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
		character_menu->setEnabled(true);
	//		tab_menu->getChildByTag(kSP_MT_character)->setVisible(true);
	else if(recent_shop_code == kSC_card)
		card_menu->setEnabled(true);
	//		tab_menu->getChildByTag(kSP_MT_card)->setVisible(true);
	else if(recent_shop_code == kSC_ruby)
		ruby_menu->setEnabled(true);
	//		tab_menu->getChildByTag(kSP_MT_ruby)->setVisible(true);
	else if(recent_shop_code == kSC_gold)
		gold_menu->setEnabled(true);
	//		tab_menu->getChildByTag(kSP_MT_gold)->setVisible(true);
	else if(recent_shop_code == kSC_heart)
		heart_menu->setEnabled(true);
	//		tab_menu->getChildByTag(kSP_MT_heart)->setVisible(true);
	
	if(t_code == kSC_character)
		character_menu->setEnabled(false);
//		tab_menu->getChildByTag(kSP_MT_character)->setVisible(false);
	else if(t_code == kSC_card)
		card_menu->setEnabled(false);
//		tab_menu->getChildByTag(kSP_MT_card)->setVisible(false);
	else if(t_code == kSC_ruby)
		ruby_menu->setEnabled(false);
//		tab_menu->getChildByTag(kSP_MT_ruby)->setVisible(false);
	else if(t_code == kSC_gold)
		gold_menu->setEnabled(false);
//		tab_menu->getChildByTag(kSP_MT_gold)->setVisible(false);
	else if(t_code == kSC_heart)
		heart_menu->setEnabled(false);
//		tab_menu->getChildByTag(kSP_MT_heart)->setVisible(false);
	
	
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
	
	if(recent_shop_code == kSC_character)
	{
		CCSize table_size = CCSizeMake(423, 208);
		CCPoint table_position = ccp(241-table_size.width/2.f, 22);
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
		character_table->setTouchPriority(touch_priority-3);
		
		suction = TouchSuctionLayer::create(touch_priority-2);
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
			CCNode* content_node = CCNode::create();
			content_node->setPosition(getContentPosition(kSP_MT_content1 + i - 1));
			main_case->addChild(content_node, kSP_Z_content, kSP_MT_content1 + i - 1);
			
			CCSprite* n_content = CCSprite::create("shop_button_back.png");
			CCSprite* s_content = CCSprite::create("shop_button_back.png");
			s_content->setColor(ccGRAY);
			
			CCSprite* inner = CCSprite::create(CCString::createWithFormat(filename.c_str(), i)->getCString());
			addPriceReward(inner, i);
			content_node->addChild(inner, 2);
			
			string sale_str;
			string price_type;
			string price_key;
			if(recent_shop_code == kSC_ruby)
			{
				sale_str = NSDS_GS(kSDS_GI_shopRuby_int1_sale_s, i-1);
				price_type = NSDS_GS(kSDS_GI_shopRuby_int1_priceType_s, i-1);
				price_key = "wonToRuby%d";
			}
			else if(recent_shop_code == kSC_gold)
			{
				sale_str = NSDS_GS(kSDS_GI_shopGold_int1_sale_s, i-1);
				price_type = NSDS_GS(kSDS_GI_shopGold_int1_priceType_s, i-1);
				price_key = "rubyToGold%d";
			}
			else if(recent_shop_code == kSC_heart)
			{
				sale_str = NSDS_GS(kSDS_GI_shopCoin_int1_sale_s, i-1);
				price_type = NSDS_GS(kSDS_GI_shopCoin_int1_priceType_s, i-1);
				price_key = "rubyToHeart%d";
			}
			
			if(!sale_str.empty())
			{
				CCSprite* tab = CCSprite::create("shop_tab.png");
				tab->setPosition(ccp(32,33));
				content_node->addChild(tab, 4);
				
				CCLabelTTF* sale_label = CCLabelTTF::create(sale_str.c_str(), mySGD->getFont().c_str(), 10);
				sale_label->setPosition(ccp(tab->getContentSize().width/2.f, tab->getContentSize().height/2.f+3));
				tab->addChild(sale_label);
			}
			
			if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_money))
			{
				CCLabelTTF* won_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat(price_key.c_str(), i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				won_label->setPosition(ccp(0, -33));
				content_node->addChild(won_label, 3);
			}
			else if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
			{
				CCSprite* ruby_img = CCSprite::create("price_ruby_img.png");
				ruby_img->setPosition(ccp(-30, -33));
				content_node->addChild(ruby_img, 3);
				
				CCLabelTTF* ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat(price_key.c_str(), i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				ruby_label->setPosition(ccp(10, -33));
				content_node->addChild(ruby_label, 3);
			}
			else if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_gold))
			{
				CCSprite* gold_img = CCSprite::create("price_gold_img.png");
				gold_img->setPosition(ccp(-30, -33));
				content_node->addChild(gold_img, 3);
				
				CCLabelTTF* gold_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat(price_key.c_str(), i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
				gold_label->setPosition(ccp(10, -33));
				content_node->addChild(gold_label, 3);
			}
			
			CCMenuItem* content_item = CCMenuItemSprite::create(n_content, s_content, this, menu_selector(ShopPopup::menuAction));
			content_item->setTag(kSP_MT_content1 + i - 1);
			
			CCMenu* content_menu = CCMenu::createWithItem(content_item);
			content_menu->setPosition(CCPointZero);
			content_node->addChild(content_menu, 1);
			
			content_menu->setTouchPriority(touch_priority-4);
		}
	}
	else
	{
		setCardBuyMenu(ccp(100,125), kSP_MT_cardHigh, "shop_card_high.png", "price_ruby_img.png", card_price_high.getV());
		setCardBuyMenu(ccp(240,125), kSP_MT_cardMid, "shop_card_mid.png", "price_gold_img.png", card_price_mid.getV());
		setCardBuyMenu(ccp(380,125), kSP_MT_cardLow, "shop_card_low.png", "price_candy_img.png", card_price_low.getV());
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
	t_buy_menu->setTouchPriority(touch_priority-4);
}

enum CharacterCellZorder
{
	kCharacterCellZorder_back = 1,
	kCharacterCellZorder_content,
	kCharacterCellZorder_selected,
	kCharacterCellZorder_tab,
	kCharacterCellZorder_lock,
	kCharacterCellZorder_buy
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
		
		if(condition_type == mySGD->getGoodsTypeToKey(kGoodsType_gold))
			is_unlock_enable = mySGD->getGoodsValue(kGoodsType_gold) >= condition_value;
		else if(condition_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
			is_unlock_enable = mySGD->getGoodsValue(kGoodsType_ruby) >= condition_value;
		
		if(is_unlock_enable)
		{
			createCheckBuyPopup([=]()
								{
//									if(condition_type == "gold")
//										mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) - condition_value);
//									else if(condition_type == "ruby")
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - condition_value);
//									
//									myDSH->setIntegerForKey(kDSH_Key_selectedCharacter, tag-kSP_MT_characterUnlockBase);
//									myDSH->setBoolForKey(kDSH_Key_isCharacterUnlocked_int1, tag-kSP_MT_characterUnlockBase, true);
//									myDSH->saveUserData({kSaveUserData_Key_gold, kSaveUserData_Key_star, kSaveUserData_Key_character}, nullptr);
									
									if(last_select_idx != -1)
										character_table->updateCellAtIndex(last_select_idx);
									
									character_table->updateCellAtIndex(unlock_idx);
									is_menu_enable = true;
								});
		}
		else
		{
			if(condition_type == mySGD->getGoodsTypeToKey(kGoodsType_gold))
				addChild(ASPopupView::getCommonNoti(touch_priority-10, myLoc->getLocalForKey(kMyLocalKey_goldNotEnought), [=](){is_menu_enable = true;}), kSP_Z_popup);
			else if(condition_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
				addChild(ASPopupView::getCommonNoti(touch_priority-10, myLoc->getLocalForKey(kMyLocalKey_rubyNotEnought), [=](){is_menu_enable = true;}), kSP_Z_popup);
			CCLOG("not enough condition");
		}
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
	cell->addChild(t_tab, kCharacterCellZorder_tab);
	
	if(idx > 0 && !myDSH->getBoolForKey(kDSH_Key_isCharacterUnlocked_int1, idx))
	{
		CCSprite* lock_img = CCSprite::create("character_lock.png");
		lock_img->setPosition(ccp(62,103));
		cell->addChild(lock_img, kCharacterCellZorder_lock, kCharacterCellZorder_lock);
		
		string condition_type = NSDS_GS(kSDS_GI_characterInfo_int1_purchaseInfo_type_s, idx+1);
		int condition_value = NSDS_GI(kSDS_GI_characterInfo_int1_purchaseInfo_value_i, idx+1);
		
		string type_filename;
		if(condition_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
			type_filename = "price_ruby_img.png";
		else if(condition_type == mySGD->getGoodsTypeToKey(kGoodsType_gold))
			type_filename = "price_gold_img.png";
//		else if(condition_type == "social")
//			type_filename = "price_candy_img.png";
		
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
		
		ScrollMenu* unlock_menu = ScrollMenu::create(unlock_item, NULL);
		unlock_menu->setPosition(ccp(62,23));
		cell->addChild(unlock_menu, kCharacterCellZorder_buy);
		
		unlock_menu->setTouchPriority(touch_priority-1);
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
			
			ScrollMenu* select_menu = ScrollMenu::create(select_item, NULL);
			select_menu->setPosition(ccp(62,23));
			cell->addChild(select_menu, kCharacterCellZorder_selected);
			
			select_menu->setTouchPriority(touch_priority-1);
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
	CCLOG("cell touched!!");
}
CCSize ShopPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(124, 206);
}
unsigned int ShopPopup::numberOfCellsInTableView(CCTableView *table)
{
	return server_character_count;
}

void ShopPopup::setCloseFunc(function<void(void)> t_close_func)
{
	is_set_close_func = true;
	close_func = t_close_func;
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
	
	is_set_close_func = false;
	target_heartTime = NULL;
	
	touch_priority = -400;
	
	server_character_count = NSDS_GI(kSDS_GI_characterCount_i);
	
	is_menu_enable = false;
	
	
	main_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_case->setContentSize(CCSizeMake(480, 280));
	main_case->setPosition(ccp(240,160-22.f-450));
	addChild(main_case, kSP_Z_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_shop), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setPosition(ccp(40,256));
	main_case->addChild(title_label);
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_inner->setContentSize(CCSizeMake(445, 228));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f);
	main_case->addChild(main_inner);
	
	CommonButton* close_menu = CommonButton::createCloseButton(touch_priority-4);
	close_menu->setPosition(getContentPosition(kSP_MT_close));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kSP_MT_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kSP_Z_content);

	
	character_menu = CommonButton::create("무기상점", 12, CCSizeMake(83,38), CommonButtonPupple, touch_priority-4);
	character_menu->setTitleColor(ccWHITE);
	character_menu->setBackgroundTypeForDisabled(CommonButtonYellow);
	character_menu->setTitleColorForDisable(ccBLACK);
	character_menu->setPosition(getContentPosition(kSP_MT_character));
	character_menu->setFunction([=](CCObject* sender)
								{
									CCNode* t_node = CCNode::create();
									t_node->setTag(kSP_MT_character);
									menuAction(t_node);
								});
	character_menu->setVisible(false);
	main_case->addChild(character_menu, kSP_Z_content);
	
	card_menu = CommonButton::create("뷰티스톤상점", 12, CCSizeMake(83,38), CommonButtonPupple, touch_priority-4);
	card_menu->setTitleColor(ccWHITE);
	card_menu->setBackgroundTypeForDisabled(CommonButtonYellow);
	card_menu->setTitleColorForDisable(ccBLACK);
	card_menu->setPosition(getContentPosition(kSP_MT_card));
	card_menu->setFunction([=](CCObject* sender)
								{
									CCNode* t_node = CCNode::create();
									t_node->setTag(kSP_MT_card);
									menuAction(t_node);
								});
	main_case->addChild(card_menu, kSP_Z_content);
	card_menu->setVisible(false);
	
	ruby_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_rubyShop), 12, CCSizeMake(83,38), CommonButtonPupple, touch_priority-4);
	ruby_menu->setTitleColor(ccWHITE);
	ruby_menu->setBackgroundTypeForDisabled(CommonButtonYellow);
	ruby_menu->setTitleColorForDisable(ccBLACK);
	ruby_menu->setPosition(getContentPosition(kSP_MT_ruby));
	ruby_menu->setFunction([=](CCObject* sender)
								{
									CCNode* t_node = CCNode::create();
									t_node->setTag(kSP_MT_ruby);
									menuAction(t_node);
								});
	main_case->addChild(ruby_menu, kSP_Z_content);
	
	gold_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_goldShop), 12, CCSizeMake(83,38), CommonButtonPupple, touch_priority-4);
	gold_menu->setTitleColor(ccWHITE);
	gold_menu->setBackgroundTypeForDisabled(CommonButtonYellow);
	gold_menu->setTitleColorForDisable(ccBLACK);
	gold_menu->setPosition(getContentPosition(kSP_MT_gold));
	gold_menu->setFunction([=](CCObject* sender)
						   {
							   CCNode* t_node = CCNode::create();
							   t_node->setTag(kSP_MT_gold);
							   menuAction(t_node);
						   });
	main_case->addChild(gold_menu, kSP_Z_content);
	
	heart_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_heartShop), 12, CCSizeMake(83,38), CommonButtonPupple, touch_priority-4);
	heart_menu->setTitleColor(ccWHITE);
	heart_menu->setBackgroundTypeForDisabled(CommonButtonYellow);
	heart_menu->setTitleColorForDisable(ccBLACK);
	heart_menu->setPosition(getContentPosition(kSP_MT_heart));
	heart_menu->setFunction([=](CCObject* sender)
						   {
							   CCNode* t_node = CCNode::create();
							   t_node->setTag(kSP_MT_heart);
							   menuAction(t_node);
						   });
	main_case->addChild(heart_menu, kSP_Z_content);
	
//	tab_menu = CCMenu::create();
//	tab_menu->setPosition(CCPointZero);
//	main_case->addChild(tab_menu, kSP_Z_content);
//	tab_menu->setTouchPriority(-300-4);
//	
//	
//	
//	CCSprite* n_character = CCSprite::create("shop_character.png");
//	CCSprite* s_character = CCSprite::create("shop_character.png");
//	s_character->setColor(ccGRAY);
//	
//	CCMenuItem* character_item = CCMenuItemSprite::create(n_character, s_character, this, menu_selector(ShopPopup::menuAction));
//	character_item->setTag(kSP_MT_character);
//	character_item->setPosition(getContentPosition(kSP_MT_character));
//	
//	tab_menu->addChild(character_item);
//	
//	CCSprite* n_card = CCSprite::create("shop_card.png");
//	CCSprite* s_card = CCSprite::create("shop_card.png");
//	s_card->setColor(ccGRAY);
//	
//	CCMenuItem* card_item = CCMenuItemSprite::create(n_card, s_card, this, menu_selector(ShopPopup::menuAction));
//	card_item->setTag(kSP_MT_card);
//	card_item->setPosition(getContentPosition(kSP_MT_card));
//	
//	tab_menu->addChild(card_item);
//	
//	CCSprite* n_ruby = CCSprite::create("shop_ruby.png");
//	CCSprite* s_ruby = CCSprite::create("shop_ruby.png");
//	s_ruby->setColor(ccGRAY);
//	
//	CCMenuItem* ruby_item = CCMenuItemSprite::create(n_ruby, s_ruby, this, menu_selector(ShopPopup::menuAction));
//	ruby_item->setTag(kSP_MT_ruby);
//	ruby_item->setPosition(getContentPosition(kSP_MT_ruby));
//	
//	tab_menu->addChild(ruby_item);
//	
//	CCSprite* n_gold = CCSprite::create("shop_gold.png");
//	CCSprite* s_gold = CCSprite::create("shop_gold.png");
//	s_gold->setColor(ccGRAY);
//	
//	CCMenuItem* gold_item = CCMenuItemSprite::create(n_gold, s_gold, this, menu_selector(ShopPopup::menuAction));
//	gold_item->setTag(kSP_MT_gold);
//	gold_item->setPosition(getContentPosition(kSP_MT_gold));
//	
//	tab_menu->addChild(gold_item);
//	
//	CCSprite* n_heart = CCSprite::create("shop_heart.png");
//	CCSprite* s_heart = CCSprite::create("shop_heart.png");
//	s_heart->setColor(ccGRAY);
//	
//	CCMenuItem* heart_item = CCMenuItemSprite::create(n_heart, s_heart, this, menu_selector(ShopPopup::menuAction));
//	heart_item->setTag(kSP_MT_heart);
//	heart_item->setPosition(getContentPosition(kSP_MT_heart));
//	
//	tab_menu->addChild(heart_item);
	
	recent_shop_code = kSC_empty;
	
	for(int i=0;i<6;i++)
	{
		index_to_ruby.push_back(KSProtectVar<int>(NSDS_GI(kSDS_GI_shopRuby_int1_price_i, i)));
		index_to_gold.push_back(KSProtectVar<int>(NSDS_GI(kSDS_GI_shopGold_int1_price_i, i)));
		index_to_heart.push_back(KSProtectVar<int>(NSDS_GI(kSDS_GI_shopCoin_int1_price_i, i)));
		
		cash_to_ruby[index_to_ruby[i].getV()] = KSProtectVar<int>(NSDS_GI(kSDS_GI_shopRuby_int1_count_i, i));
		ruby_to_gold[index_to_gold[i].getV()] = KSProtectVar<int>(NSDS_GI(kSDS_GI_shopGold_int1_count_i, i));
		ruby_to_heart[index_to_heart[i].getV()] = KSProtectVar<int>(NSDS_GI(kSDS_GI_shopCoin_int1_count_i, i));
	}
	
	card_price_high = KSProtectVar<int>(100);
	card_price_mid = KSProtectVar<int>(10000);
	card_price_low = KSProtectVar<int>(10);
	
	showPopup();
	
    return true;
}

void ShopPopup::showPopup()
{
	setTouchEnabled(true);
	CCMoveTo* main_move = CCMoveTo::create(0.3f, ccp(240,160-22.f));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ShopPopup::endShowPopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void ShopPopup::endShowPopup()
{
	if(!mySGD->getUserdataIsFirstBuy() && mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_firstPurchase) && mySGD->getUserdataTotalPlayCount() >= mySGD->getFirstPurchasePlayCount() && !mySGD->getUserdataIsVIP())
	{
		FirstPurchasePopup* t_popup = FirstPurchasePopup::create(touch_priority-100, [=](){is_menu_enable = true;}, [=](){is_menu_enable = true;});
		addChild(t_popup, kSP_Z_popup);
		return;
	}
	
	is_menu_enable = true;
}

void ShopPopup::hidePopup()
{
	is_menu_enable = false;
	
	CCMoveTo* main_move = CCMoveTo::create(0.5f, ccp(240,160-22.f-450));
	CCCallFunc* main_call = CCCallFunc::create(this, callfunc_selector(ShopPopup::endHidePopup));
	CCSequence* main_seq = CCSequence::createWithTwoActions(main_move, main_call);
	main_case->runAction(main_seq);
}

void ShopPopup::endHidePopup()
{
	if(target_final)
		(target_final->*delegate_final)();
	if(is_set_close_func)
		close_func();
	removeFromParent();
}

CCPoint ShopPopup::getContentPosition(int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kSP_MT_close)
		return_value = ccp(453,257);
	else if(t_tag == kSP_MT_character)
		return_value = ccp(59,256);
	else if(t_tag == kSP_MT_card)
		return_value = ccp(141.5f,256);
	else if(t_tag == kSP_MT_ruby)
		return_value = ccp(225,256);//ccp(59,256);//ccp(224,256);
	else if(t_tag == kSP_MT_gold)
		return_value = ccp(305,256);//ccp(141.5f,256);//ccp(306,256);
	else if(t_tag == kSP_MT_heart)
		return_value = ccp(385,256);//ccp(224,256);//ccp(388.5f,256);
	else if(t_tag == kSP_MT_content1)
		return_value = ccp(100,177);
	else if(t_tag == kSP_MT_content2)
		return_value = ccp(240,177);
	else if(t_tag == kSP_MT_content3)
		return_value = ccp(380,177);
	else if(t_tag == kSP_MT_content4)
		return_value = ccp(100,72);
	else if(t_tag == kSP_MT_content5)
		return_value = ccp(240,72);
	else if(t_tag == kSP_MT_content6)
		return_value = ccp(380,72);
	
	return return_value;
}

void ShopPopup::resultSetUserData(Json::Value result_data)
{
	if(loading_layer)
	{
		loading_layer->removeFromParent();
		loading_layer = NULL;
	}
	
	is_menu_enable = true;
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		CCLOG("userdata was save to server");
	}
	else
	{
		CCLOG("fail!! save userdata to server");
		fail_func();
	}
}

void ShopPopup::menuAction(CCObject* pSender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
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
	else if(tag >= kSP_MT_content1 && tag <= kSP_MT_content6)
	{
		if(recent_shop_code == kSC_ruby)
		{
			createCheckBuyPopup([=]()
								{
									loading_layer = LoadingLayer::create();
									addChild(loading_layer, kSP_Z_popup);
									
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
									mySGD->addChangeGoods(kGoodsType_ruby, NSDS_GI(kSDS_GI_shopRuby_int1_count_i, tag-kSP_MT_content1), "루비구매(IOS-인앱결제)", "", "", true);
									
									mySGD->changeGoods([=](Json::Value result_data){
										loading_layer->removeFromParent();
										
										if(result_data["result"]["code"].asInt() == GDSUCCESS)
										{
											
										}
										else
										{
											mySGD->clearChangeGoods();
											addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
										}
										is_menu_enable = true;
									});
									
									
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
									Json::Value param;
									param["productid"] = mySGD->getInappProduct(tag-kSP_MT_content1);
									hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
										KS::KSLog("in-app test \n%", v);
										if(v["issuccess"].asInt())
										{
											requestItemDelivery();
										}
										else
										{
											loading_layer->removeFromParent();
											
											addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
											
											is_menu_enable = true;
										}
									});
#endif
								});
		}
		else if(recent_shop_code == kSC_gold)
		{
//			string price_type = NSDS_GS(kSDS_GI_shopGold_int1_priceType_s, tag-kSP_MT_content1);
			
			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_gold[tag-kSP_MT_content1].getV())
			{
				createCheckBuyPopup([=]()
									{
										loading_layer = LoadingLayer::create();
										addChild(loading_layer, kSP_Z_popup);
										
										mySGD->addChangeGoods(kGoodsType_ruby, -index_to_gold[tag-kSP_MT_content1].getV(), "골드구매");
										mySGD->addChangeGoods(kGoodsType_gold, ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV(), "루비로구매");
										
										fail_func = [=]()
										{
											mySGD->clearChangeGoods();
										};
										
										mySGD->changeGoods(json_selector(this, ShopPopup::resultSetUserData));
									});
			}
			else
			{
				addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-10, kGoodsType_ruby, [=]()
																	{
																		setShopCode(kSC_ruby);
																	}), kSP_Z_popup);
				is_menu_enable = true;
				CCLOG("not enough ruby!!!");
			}
		}
		else if(recent_shop_code == kSC_heart)
		{
//			string price_type = NSDS_GS(kSDS_GI_shopGold_int1_priceType_s, tag-kSP_MT_content1);
			
			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_heart[tag-kSP_MT_content1].getV())
			{
				createCheckBuyPopup([=]()
									{
										loading_layer = LoadingLayer::create();
										addChild(loading_layer, kSP_Z_popup);
										
										mySGD->addChangeGoods(kGoodsType_ruby, -index_to_heart[tag-kSP_MT_content1].getV(), "하트구매");
										
										
										myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
										
										if(target_heartTime)
										{
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
											else if(before_code == kShopBeforeCode_startsetting)
												((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
										}
										
										fail_func = [=]()
										{
											mySGD->clearChangeGoods();
											myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) - ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
											
											if(target_heartTime)
											{
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
												else if(before_code == kShopBeforeCode_startsetting)
													((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
											}
										};
										
										mySGD->changeGoods(json_selector(this, ShopPopup::resultSetUserData));
									});
			}
			else
			{
				addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-10, kGoodsType_ruby, [=]()
																	{
																		setShopCode(kSC_ruby);
																	}), kSP_Z_popup);
				is_menu_enable = true;
				CCLOG("not enough ruby!!!");
			}
		}
	}
//	else if(tag == kSP_MT_content2)
//	{
//		if(recent_shop_code == kSC_ruby)
//		{
//			createCheckBuyPopup([=]()
//								{
//									CCLOG("4900won!!!");
//									
//									loading_layer = LoadingLayer::create();
//									addChild(loading_layer, kSP_Z_popup);
//									
//									mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
//									
//									vector<SaveUserData_Key> save_userdata_list;
//									save_userdata_list.push_back(kSaveUserData_Key_star);
//									myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//								});
//		}
//		else if(recent_shop_code == kSC_gold)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_gold[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_gold[tag-kSP_MT_content1].getV());
//										mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										save_userdata_list.push_back(kSaveUserData_Key_gold);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//		else if(recent_shop_code == kSC_heart)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_heart[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_heart[tag-kSP_MT_content1].getV());
//										myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
//										
//										CCNode* target_parent = target_heartTime->getParent();
//										CCPoint heart_time_position = target_heartTime->getPosition();
//										int heart_time_tag = target_heartTime->getTag();
//										
//										target_heartTime->removeFromParent();
//										target_heartTime = HeartTime::create();
//										target_heartTime->setPosition(heart_time_position);
//										target_parent->addChild(target_heartTime, 0, heart_time_tag);
//										if(before_code == kShopBeforeCode_stagesetting)
//											((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_mainflow)
//											((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_puzzle)
//											((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_startsetting)
//											((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//	}
//	else if(tag == kSP_MT_content3)
//	{
//		if(recent_shop_code == kSC_ruby)
//		{
//			createCheckBuyPopup([=]()
//								{
//									CCLOG("9800won!!!");
//									
//									loading_layer = LoadingLayer::create();
//									addChild(loading_layer, kSP_Z_popup);
//									
//									mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
//									
//									vector<SaveUserData_Key> save_userdata_list;
//									save_userdata_list.push_back(kSaveUserData_Key_star);
//									myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//								});
//		}
//		else if(recent_shop_code == kSC_gold)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_gold[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_gold[tag-kSP_MT_content1].getV());
//										mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										save_userdata_list.push_back(kSaveUserData_Key_gold);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//		else if(recent_shop_code == kSC_heart)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_heart[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_heart[tag-kSP_MT_content1].getV());
//										myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
//										
//										CCNode* target_parent = target_heartTime->getParent();
//										CCPoint heart_time_position = target_heartTime->getPosition();
//										int heart_time_tag = target_heartTime->getTag();
//										
//										target_heartTime->removeFromParent();
//										target_heartTime = HeartTime::create();
//										target_heartTime->setPosition(heart_time_position);
//										target_parent->addChild(target_heartTime, 0, heart_time_tag);
//										if(before_code == kShopBeforeCode_stagesetting)
//											((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_mainflow)
//											((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_puzzle)
//											((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_startsetting)
//											((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//	}
//	else if(tag == kSP_MT_content4)
//	{
//		if(recent_shop_code == kSC_ruby)
//		{
//			createCheckBuyPopup([=]()
//								{
//									CCLOG("29800won!!!");
//									
//									loading_layer = LoadingLayer::create();
//									addChild(loading_layer, kSP_Z_popup);
//									
//									mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
//									
//									vector<SaveUserData_Key> save_userdata_list;
//									save_userdata_list.push_back(kSaveUserData_Key_star);
//									myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//								});
//		}
//		else if(recent_shop_code == kSC_gold)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_gold[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_gold[tag-kSP_MT_content1].getV());
//										mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										save_userdata_list.push_back(kSaveUserData_Key_gold);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//		else if(recent_shop_code == kSC_heart)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_heart[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_heart[tag-kSP_MT_content1].getV());
//										myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
//										
//										CCNode* target_parent = target_heartTime->getParent();
//										CCPoint heart_time_position = target_heartTime->getPosition();
//										int heart_time_tag = target_heartTime->getTag();
//										
//										target_heartTime->removeFromParent();
//										target_heartTime = HeartTime::create();
//										target_heartTime->setPosition(heart_time_position);
//										target_parent->addChild(target_heartTime, 0, heart_time_tag);
//										if(before_code == kShopBeforeCode_stagesetting)
//											((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_mainflow)
//											((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_puzzle)
//											((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_startsetting)
//											((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//	}
//	else if(tag == kSP_MT_content5)
//	{
//		if(recent_shop_code == kSC_ruby)
//		{
//			createCheckBuyPopup([=]()
//								{
//									CCLOG("49800won!!!");
//									
//									loading_layer = LoadingLayer::create();
//									addChild(loading_layer, kSP_Z_popup);
//									
//									mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
//									
//									vector<SaveUserData_Key> save_userdata_list;
//									save_userdata_list.push_back(kSaveUserData_Key_star);
//									myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//								});
//		}
//		else if(recent_shop_code == kSC_gold)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_gold[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_gold[tag-kSP_MT_content1].getV());
//										mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										save_userdata_list.push_back(kSaveUserData_Key_gold);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//		else if(recent_shop_code == kSC_heart)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_heart[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_heart[tag-kSP_MT_content1].getV());
//										myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
//										
//										CCNode* target_parent = target_heartTime->getParent();
//										CCPoint heart_time_position = target_heartTime->getPosition();
//										int heart_time_tag = target_heartTime->getTag();
//										
//										target_heartTime->removeFromParent();
//										target_heartTime = HeartTime::create();
//										target_heartTime->setPosition(heart_time_position);
//										target_parent->addChild(target_heartTime, 0, heart_time_tag);
//										if(before_code == kShopBeforeCode_stagesetting)
//											((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_mainflow)
//											((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_puzzle)
//											((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_startsetting)
//											((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//	}
//	else if(tag == kSP_MT_content6)
//	{
//		if(recent_shop_code == kSC_ruby)
//		{
//			createCheckBuyPopup([=]()
//								{
//									CCLOG("99800won!!!");
//									
//									loading_layer = LoadingLayer::create();
//									addChild(loading_layer, kSP_Z_popup);
//									
//									mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) + cash_to_ruby[index_to_ruby[tag-kSP_MT_content1].getV()].getV());
//									
//									vector<SaveUserData_Key> save_userdata_list;
//									save_userdata_list.push_back(kSaveUserData_Key_star);
//									myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//								});
//		}
//		else if(recent_shop_code == kSC_gold)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_gold[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_gold[tag-kSP_MT_content1].getV());
//										mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold) + ruby_to_gold[index_to_gold[tag-kSP_MT_content1].getV()].getV());
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										save_userdata_list.push_back(kSaveUserData_Key_gold);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//		else if(recent_shop_code == kSC_heart)
//		{
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_heart[tag-kSP_MT_content1].getV())
//			{
//				createCheckBuyPopup([=]()
//									{
//										loading_layer = LoadingLayer::create();
//										addChild(loading_layer, kSP_Z_popup);
//										
//										mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby) - index_to_heart[tag-kSP_MT_content1].getV());
//										myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
//										
//										CCNode* target_parent = target_heartTime->getParent();
//										CCPoint heart_time_position = target_heartTime->getPosition();
//										int heart_time_tag = target_heartTime->getTag();
//										
//										target_heartTime->removeFromParent();
//										target_heartTime = HeartTime::create();
//										target_heartTime->setPosition(heart_time_position);
//										target_parent->addChild(target_heartTime, 0, heart_time_tag);
//										if(before_code == kShopBeforeCode_stagesetting)
//											((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_mainflow)
//											((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_puzzle)
//											((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										else if(before_code == kShopBeforeCode_startsetting)
//											((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//										
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_star);
//										myDSH->saveUserData(save_userdata_list, json_selector(this, ShopPopup::resultSetUserData));
//									});
//			}
//			else
//			{
//				addChild(ASPopupView::getCommonNoti(-310, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//				CCLOG("not enough ruby!!!");
//			}
//		}
//	}
	else if(tag == kSP_MT_cardHigh)
	{
		if(mySGD->getGoodsValue(kGoodsType_ruby) >= card_price_high.getV())
		{
			createCheckBuyPopup([=]()
								{
//									mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby)-card_price_high.getV());
									
									int have_stone_cnt = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
									have_stone_cnt++;
									int self_stone_id = myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID);
									self_stone_id++;
									myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, have_stone_cnt);
									myDSH->setIntegerForKey(kDSH_Key_selfBeautyStoneID, self_stone_id);
									myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, have_stone_cnt, self_stone_id);
									
									myDSH->setIntegerForKey(kDSH_Key_beautyStoneType_int1, self_stone_id, rand()%7);
									myDSH->setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, self_stone_id, 1);
									
									int random_value = rand()%1000;
									int rank_value;
									if(random_value < 500)
										rank_value = 2;
									else if(random_value < 800)
										rank_value = 3;
									else
										rank_value = 4;
									
									myDSH->setIntegerForKey(kDSH_Key_beautyStoneRank_int1, self_stone_id, rank_value);
									
									result_stone_layer = TouchSuctionLayer::create(-501);
									CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
									float screen_scale_x = screen_size.width/screen_size.height/1.5f;
									if(screen_scale_x < 1.f)
										screen_scale_x = 1.f;
									
									CCSprite* gray = CCSprite::create("back_gray.png");
									gray->setPosition(ccp(240,160));
									gray->setScaleX(screen_scale_x);
									gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
									result_stone_layer->addChild(gray);
									
									CCSprite* stone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, self_stone_id), myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, self_stone_id))->getCString());
									stone_img->setPosition(ccp(240, 160));
									result_stone_layer->addChild(stone_img);
									
									CCLabelTTF* stone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, self_stone_id))->getCString(), mySGD->getFont().c_str(), 12);
									stone_level->setAnchorPoint(ccp(1,0));
									stone_level->setPosition(ccp(stone_img->getContentSize().width/2.f-3, -stone_img->getContentSize().height/2.f+3));
									stone_img->addChild(stone_level);
									result_stone_layer->target_touch_began = result_stone_layer;
									result_stone_layer->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
									addChild(result_stone_layer, kSP_Z_popup);
									result_stone_layer->setVisible(false);
									
									
									Json::Value param;
									param["memberID"] = hspConnector::get()->getSocialID();
									
									Json::Value data;
									data[myDSH->getKey(kDSH_Key_savedStar)] = myDSH->getIntegerForKey(kDSH_Key_savedStar);
									{
										for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
										{
											int slot_count = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
											for(int j=1;j<=slot_count;j++)
												data[myDSH->getKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2)][i-1][j] = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i-1, j);
										}
										
										data[myDSH->getKey(kDSH_Key_selfBeautyStoneID)] = myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID);
										int have_beauty_stone_cnt = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
										data[myDSH->getKey(kDSH_Key_haveBeautyStoneCnt)] = have_beauty_stone_cnt;
										for(int i=1;i<=have_beauty_stone_cnt;i++)
										{
											int beauty_stone_id = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i);
											data[myDSH->getKey(kDSH_Key_haveBeautyStoneID_int1)][i] = beauty_stone_id;
											data[myDSH->getKey(kDSH_Key_beautyStoneType_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, beauty_stone_id);
											data[myDSH->getKey(kDSH_Key_beautyStoneRank_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, beauty_stone_id);
											data[myDSH->getKey(kDSH_Key_beautyStoneLevel_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beauty_stone_id);
										}
									}
									
									Json::FastWriter writer;
									param["data"] = writer.write(data);
									param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
									
									command_list.push_back(CommandParam("updateUserData", param, nullptr));
									
									loading_layer = LoadingLayer::create();
									addChild(loading_layer, kSP_Z_popup);
									
									card_gacha_type = "ruby";
									
									startCardGacha();
								});
		}
		else
		{
			addChild(ASPopupView::getCommonNoti(touch_priority-10, "루비가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
		}
	}
	else if(tag == kSP_MT_cardMid)
	{
		if(mySGD->getGoodsValue(kGoodsType_gold) >= card_price_mid.getV())
		{
			createCheckBuyPopup([=]()
								{
//									mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold)-card_price_mid.getV());
									
									int have_stone_cnt = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
									have_stone_cnt++;
									int self_stone_id = myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID);
									self_stone_id++;
									myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, have_stone_cnt);
									myDSH->setIntegerForKey(kDSH_Key_selfBeautyStoneID, self_stone_id);
									myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, have_stone_cnt, self_stone_id);
									
									myDSH->setIntegerForKey(kDSH_Key_beautyStoneType_int1, self_stone_id, rand()%7);
									myDSH->setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, self_stone_id, 1);
									
									int random_value = rand()%1000;
									int rank_value;
									if(random_value < 500)
										rank_value = 1;
									else if(random_value < 800)
										rank_value = 2;
									else
										rank_value = 3;
									
									myDSH->setIntegerForKey(kDSH_Key_beautyStoneRank_int1, self_stone_id, rank_value);
									
									result_stone_layer = TouchSuctionLayer::create(-501);
									CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
									float screen_scale_x = screen_size.width/screen_size.height/1.5f;
									if(screen_scale_x < 1.f)
										screen_scale_x = 1.f;
									
									CCSprite* gray = CCSprite::create("back_gray.png");
									gray->setPosition(ccp(240,160));
									gray->setScaleX(screen_scale_x);
									gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
									result_stone_layer->addChild(gray);
									
									CCSprite* stone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, self_stone_id), myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, self_stone_id))->getCString());
									stone_img->setPosition(ccp(240, 160));
									result_stone_layer->addChild(stone_img);
									
									CCLabelTTF* stone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, self_stone_id))->getCString(), mySGD->getFont().c_str(), 12);
									stone_level->setAnchorPoint(ccp(1,0));
									stone_level->setPosition(ccp(stone_img->getContentSize().width/2.f-3, -stone_img->getContentSize().height/2.f+3));
									stone_img->addChild(stone_level);
									result_stone_layer->target_touch_began = result_stone_layer;
									result_stone_layer->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
									addChild(result_stone_layer, kSP_Z_popup);
									result_stone_layer->setVisible(false);
									
									
									Json::Value param;
									param["memberID"] = hspConnector::get()->getSocialID();
									
									Json::Value data;
									data[myDSH->getKey(kDSH_Key_savedGold)] = myDSH->getIntegerForKey(kDSH_Key_savedGold);
									{
										for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
										{
											int slot_count = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
											for(int j=1;j<=slot_count;j++)
												data[myDSH->getKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2)][i-1][j] = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i-1, j);
										}
										
										data[myDSH->getKey(kDSH_Key_selfBeautyStoneID)] = myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID);
										int have_beauty_stone_cnt = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
										data[myDSH->getKey(kDSH_Key_haveBeautyStoneCnt)] = have_beauty_stone_cnt;
										for(int i=1;i<=have_beauty_stone_cnt;i++)
										{
											int beauty_stone_id = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i);
											data[myDSH->getKey(kDSH_Key_haveBeautyStoneID_int1)][i] = beauty_stone_id;
											data[myDSH->getKey(kDSH_Key_beautyStoneType_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, beauty_stone_id);
											data[myDSH->getKey(kDSH_Key_beautyStoneRank_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, beauty_stone_id);
											data[myDSH->getKey(kDSH_Key_beautyStoneLevel_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beauty_stone_id);
										}
									}
									
									Json::FastWriter writer;
									param["data"] = writer.write(data);
									param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
									
									command_list.push_back(CommandParam("updateUserData", param, nullptr));
									
									loading_layer = LoadingLayer::create();
									addChild(loading_layer, kSP_Z_popup);
									
									card_gacha_type = "gold";
									
									startCardGacha();
								});
		}
		else
		{
			addChild(ASPopupView::getCommonNoti(touch_priority-10, "골드가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
		}
	}
	else if(tag == kSP_MT_cardLow)
	{
//		if(mySGD->getFriendPoint() >= card_price_low.getV())
//		{
//			createCheckBuyPopup([=]()
//								{
//									mySGD->setFriendPoint(mySGD->getFriendPoint()-card_price_low.getV());
//									
//									int have_stone_cnt = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
//									have_stone_cnt++;
//									int self_stone_id = myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID);
//									self_stone_id++;
//									myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneCnt, have_stone_cnt);
//									myDSH->setIntegerForKey(kDSH_Key_selfBeautyStoneID, self_stone_id);
//									myDSH->setIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, have_stone_cnt, self_stone_id);
//									
//									myDSH->setIntegerForKey(kDSH_Key_beautyStoneType_int1, self_stone_id, rand()%7);
//									myDSH->setIntegerForKey(kDSH_Key_beautyStoneLevel_int1, self_stone_id, 1);
//									
//									int random_value = rand()%1000;
//									int rank_value;
//									if(random_value < 400)
//										rank_value = 1;
//									else if(random_value < 700)
//										rank_value = 2;
//									else if(random_value < 900)
//										rank_value = 3;
//									else
//										rank_value = 4;
//									
//									myDSH->setIntegerForKey(kDSH_Key_beautyStoneRank_int1, self_stone_id, rank_value);
//									
//									result_stone_layer = TouchSuctionLayer::create(-501);
//									CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//									float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//									if(screen_scale_x < 1.f)
//										screen_scale_x = 1.f;
//									
//									CCSprite* gray = CCSprite::create("back_gray.png");
//									gray->setPosition(ccp(240,160));
//									gray->setScaleX(screen_scale_x);
//									gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
//									result_stone_layer->addChild(gray);
//									
//									CCSprite* stone_img = CCSprite::create(CCString::createWithFormat("beautystone_%d_%d.png", myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, self_stone_id), myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, self_stone_id))->getCString());
//									stone_img->setPosition(ccp(240, 160));
//									result_stone_layer->addChild(stone_img);
//									
//									CCLabelTTF* stone_level = CCLabelTTF::create(CCString::createWithFormat("Lv.%d", myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, self_stone_id))->getCString(), mySGD->getFont().c_str(), 12);
//									stone_level->setAnchorPoint(ccp(1,0));
//									stone_level->setPosition(ccp(stone_img->getContentSize().width/2.f-3, -stone_img->getContentSize().height/2.f+3));
//									stone_img->addChild(stone_level);
//									result_stone_layer->target_touch_began = result_stone_layer;
//									result_stone_layer->delegate_touch_began = callfunc_selector(TouchSuctionLayer::removeFromParent);
//									addChild(result_stone_layer, kSP_Z_popup);
//									result_stone_layer->setVisible(false);
//									
//									
//									Json::Value param;
//									param["memberID"] = hspConnector::get()->getSocialID();
//									
//									Json::Value data;
//									data[myDSH->getKey(kDSH_Key_savedFriendPoint)] = myDSH->getIntegerForKey(kDSH_Key_savedFriendPoint);
//									{
//										for(int i=1;i<=NSDS_GI(kSDS_GI_characterCount_i);i++)
//										{
//											int slot_count = NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
//											for(int j=1;j<=slot_count;j++)
//												data[myDSH->getKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2)][i-1][j] = myDSH->getIntegerForKey(kDSH_Key_selectedCharacter_int1_weaponSlot_int2, i-1, j);
//										}
//										
//										data[myDSH->getKey(kDSH_Key_selfBeautyStoneID)] = myDSH->getIntegerForKey(kDSH_Key_selfBeautyStoneID);
//										int have_beauty_stone_cnt = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneCnt);
//										data[myDSH->getKey(kDSH_Key_haveBeautyStoneCnt)] = have_beauty_stone_cnt;
//										for(int i=1;i<=have_beauty_stone_cnt;i++)
//										{
//											int beauty_stone_id = myDSH->getIntegerForKey(kDSH_Key_haveBeautyStoneID_int1, i);
//											data[myDSH->getKey(kDSH_Key_haveBeautyStoneID_int1)][i] = beauty_stone_id;
//											data[myDSH->getKey(kDSH_Key_beautyStoneType_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneType_int1, beauty_stone_id);
//											data[myDSH->getKey(kDSH_Key_beautyStoneRank_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneRank_int1, beauty_stone_id);
//											data[myDSH->getKey(kDSH_Key_beautyStoneLevel_int1)][i] = myDSH->getIntegerForKey(kDSH_Key_beautyStoneLevel_int1, beauty_stone_id);
//										}
//									}
//									
//									Json::FastWriter writer;
//									param["data"] = writer.write(data);
//									param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
//									
//									command_list.push_back(CommandParam("updateUserData", param, nullptr));
//									
//									loading_layer = LoadingLayer::create();
//									addChild(loading_layer, kSP_Z_popup);
//									
//									card_gacha_type = "social";
//									
//									startCardGacha();
//								});
//		}
//		else
//		{
//			addChild(ASPopupView::getCommonNoti(-310, "소셜 포인트가 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
//		}
	}
}

void ShopPopup::createCheckBuyPopup(function<void()> buy_action)
{
	ASPopupView* t_popup = ASPopupView::create(touch_priority-5);
	
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
	addChild(t_popup, kSP_Z_popup);
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	case_back->setPosition(ccp(0,0));
	t_container->addChild(case_back);
	
	case_back->setContentSize(CCSizeMake(220, 190));
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	content_back->setPosition(ccp(0,25));
	t_container->addChild(content_back);
	
	content_back->setContentSize(CCSizeMake(200, 120));
	
	CCLabelTTF* ment_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_realBuy),	mySGD->getFont().c_str(), 15);
	ment_label->setPosition(ccp(0,25));
	t_container->addChild(ment_label);
	
	
	
	CommonButton* cancel_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
	cancel_button->setPosition(ccp(100,85));
	cancel_button->setFunction([=](CCObject* sender)
							   {
								   is_menu_enable = true;
								   t_popup->removeFromParent();
							   });
	t_container->addChild(cancel_button);
	
	
	CommonButton* ok_button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ok), 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
	ok_button->setPosition(ccp(0,-65));
	ok_button->setFunction([=](CCObject* sender)
						   {
							   buy_action();
							   t_popup->removeFromParent();
						   });
	t_container->addChild(ok_button);
}

void ShopPopup::startCardGacha()
{
	Json::Value param;
	param["type"] = card_gacha_type;
	command_list.push_back(CommandParam("getcardinfobyrand", param, json_selector(this, ShopPopup::resultCardGacha)));
	hspConnector::get()->command(command_list);
}

void ShopPopup::resultCardGacha(Json::Value result_data)
{
	CCLOG("resultCardGacha : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		command_list.clear();
		
		loading_layer->removeFromParent();
		result_stone_layer->setVisible(true);
		result_stone_layer->setTouchEnabled(true);
		is_menu_enable = true;
		
//		Json::Value t_card = result_data["cardInfo"];
//		NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
//		
//		NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
//		NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
//		NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
//		
//		Json::Value t_card_missile = t_card["missile"];
//		NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//		NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//		NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//		
//		NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//		
//		Json::Value t_ability = t_card["ability"];
//		NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//		for(int j=0;j<t_ability.size();j++)
//		{
//			Json::Value t_abil = t_ability[j];
//			NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//			
//			Json::Value t_option;
//			if(!t_abil["option"].isObject())                    t_option["key"]="value";
//			else												t_option =t_abil["option"];
//			
//			if(t_abil["type"].asInt() == kIC_attack)
//				NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_addTime)
//				NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_fast)
//				NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_silence)
//				NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_doubleItem)
//				NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_longTime)
//				NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_bossLittleEnergy)
//				NSDS_SI(kSDS_CI_int1_abilityBossLittleEnergyOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_subSmallSize)
//				NSDS_SI(kSDS_CI_int1_abilitySubSmallSizeOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_smallArea)
//				NSDS_SI(kSDS_CI_int1_abilitySmallAreaOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_widePerfect)
//				NSDS_SI(kSDS_CI_int1_abilityWidePerfectOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//		}
//		
//		Json::Value t_imgInfo = t_card["imgInfo"];
//		
//		bool is_add_cf = false;
//		
//		if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
//		{
//			// check, after download ----------
//			DownloadFile t_df;
//			t_df.size = t_imgInfo["size"].asInt();
//			t_df.img = t_imgInfo["img"].asString().c_str();
//			t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//			t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
//			df_list.push_back(t_df);
//			// ================================
//			
//			CopyFile t_cf;
//			t_cf.from_filename = t_df.filename.c_str();
//			t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//			cf_list.push_back(t_cf);
//			
//			is_add_cf = true;
//		}
//		
//		Json::Value t_aniInfo = t_card["aniInfo"];
//		NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
//		if(t_aniInfo["isAni"].asBool())
//		{
//			Json::Value t_detail = t_aniInfo["detail"];
//			NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
//			
//			Json::Value t_loopSeq = t_detail["loopSeq"];
//			for(int j=0;j<t_loopSeq.size();j++)
//				NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
//			
//			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
//			
//			if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
//			{
//				// check, after download ----------
//				DownloadFile t_df;
//				t_df.size = t_detail["size"].asInt();
//				t_df.img = t_detail["img"].asString().c_str();
//				t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//				t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
//				df_list.push_back(t_df);
//				// ================================
//			}
//			
//			if(is_add_cf)
//			{
//				CopyFile t_cf = cf_list.back();
//				cf_list.pop_back();
//				t_cf.is_ani = true;
//				t_cf.cut_width = t_detail["cutWidth"].asInt();
//				t_cf.cut_height = t_detail["cutHeight"].asInt();
//				t_cf.position_x = t_detail["positionX"].asInt();
//				t_cf.position_y = t_detail["positionY"].asInt();
//				t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//				cf_list.push_back(t_cf);
//			}
//		}
//		
//		NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
//		NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].toStyledString(), false);
//		
//		Json::Value t_silImgInfo = t_card["silImgInfo"];
//		NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
//		if(t_silImgInfo["isSil"].asBool())
//		{
//			if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
//			{
//				// check, after download ----------
//				DownloadFile t_df;
//				t_df.size = t_silImgInfo["size"].asInt();
//				t_df.img = t_silImgInfo["img"].asString().c_str();
//				t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
//				t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
//				df_list.push_back(t_df);
//				// ================================
//			}
//		}
//		
//		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//		
//		gacha_card_number = t_card["no"].asInt();
//		
//		if(df_list.size() + cf_list.size() > 0) // need download
//		{
//			ing_download_cnt = 1;
//			is_downloading = true;
//			startDownload();
//		}
//		else
//		{
//			loading_layer->removeFromParent();
//			
//			if(myDSH->getIntegerForKey(kDSH_Key_cardDurability_int1, gacha_card_number) > 0)
//			{
//				myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, gacha_card_number, myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, gacha_card_number));
//				
//				// 강화
//				float strength_rate = ((NSDS_GI(kSDS_CI_int1_rank_i, gacha_card_number)*10.f + 1)*NSDS_GI(kSDS_CI_int1_durability_i, gacha_card_number))/((NSDS_GI(kSDS_CI_int1_rank_i, gacha_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, gacha_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, gacha_card_number));
//				CCLOG("strength_rate : %.3f", strength_rate);
//				
//				random_device rd;
//				default_random_engine e1(rd());
//				uniform_real_distribution<float> uniform_dist(0.f, 1.f);
//				
//				float result_value = uniform_dist(e1);
//				CCLOG("result value : %.3f", result_value);
//				
//				CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",gacha_card_number)->getCString());
//				CardCase* cardCase = CardCase::create(gacha_card_number);
//				card->addChild(cardCase);
//				
//				
//				CCSprite* card2 = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",gacha_card_number)->getCString());
//				CardCase* cardCase2 = CardCase::create(gacha_card_number, 1, NSDS_GS(kSDS_CI_int1_passive_s, gacha_card_number));
//				card2->addChild(cardCase2);
//				
//				
//				StrengthCardAnimation* b = StrengthCardAnimation::create(card,card2,-350);
//				
//				b->setCloseFunc([this](){
//					CCLOG("close Func");
//					this->is_menu_enable = true;
//				});
//				
//				if(result_value <= strength_rate)
//				{
//					CCLOG("success");
//					
//					b->startSuccess("카드레벨 +1");
//					
//					myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, gacha_card_number, myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, gacha_card_number)+1);
//					myDSH->saveUserData({kSaveUserData_Key_cardsInfo}, nullptr);
//				}
//				else
//				{
//					CCLOG("fail");
//					
//					b->startFail("강화 실패");
//				}
//				
//				addChild(b, kSP_Z_popup);
//			}
//			else
//			{
//				if(myDSH->getIntegerForKey(kDSH_Key_hasGottenCard_int1, gacha_card_number) == 0)
//				{
//					myDSH->setIntegerForKey(kDSH_Key_cardTakeCnt, myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt) + 1);
//					myDSH->setIntegerForKey(kDSH_Key_hasGottenCard_int1, gacha_card_number, myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt));
//					myDSH->setIntegerForKey(kDSH_Key_takeCardNumber_int1, myDSH->getIntegerForKey(kDSH_Key_cardTakeCnt), gacha_card_number);
//					
//					myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, gacha_card_number, NSDS_GI(kSDS_CI_int1_durability_i, gacha_card_number));
//					myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, gacha_card_number, 1);
//					myDSH->setIntegerForKey(kDSH_Key_cardMaxDurability_int1, gacha_card_number, NSDS_GI(kSDS_CI_int1_durability_i, gacha_card_number));
//					myDSH->setStringForKey(kDSH_Key_cardPassive_int1, gacha_card_number, NSDS_GS(kSDS_CI_int1_passive_s, gacha_card_number));
//					
//					mySGD->addHasGottenCardNumber(gacha_card_number);
//				}
//				else
//				{
//					myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, gacha_card_number, NSDS_GI(kSDS_CI_int1_durability_i, gacha_card_number));
//					myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, gacha_card_number, 1);
//					myDSH->setIntegerForKey(kDSH_Key_cardMaxDurability_int1, gacha_card_number, NSDS_GI(kSDS_CI_int1_durability_i, gacha_card_number));
//					myDSH->setStringForKey(kDSH_Key_cardPassive_int1, gacha_card_number, NSDS_GS(kSDS_CI_int1_passive_s, gacha_card_number));
//				}
//				
//				// 획득
//				CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",gacha_card_number)->getCString());
//				CardCase* cardCase = CardCase::create(gacha_card_number);
//				card->addChild(cardCase);
//				
//				TakeCardAnimation* b = TakeCardAnimation::create(card,-350);
//				b->setCloseFunc([this](){
//					CCLOG("close Func");
//					this->is_menu_enable = true;
//				});
//				b->start();
//				addChild(b, kSP_Z_popup);
//			}
//		}
	}
	else
	{
		replay_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, touch_priority-50);
		replay_menu->setPosition(ccp(240,100));
		replay_menu->setFunction([=](CCObject* sender)
								 {
									 CCNode* t_node = CCNode::create();
									 t_node->setTag(1);
									 replayAction(t_node);
								 });
		
		addChild(replay_menu, kSP_Z_popup);
	}
}

void ShopPopup::startDownload()
{
	if(ing_download_cnt <= df_list.size())
	{
		CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
		StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename,
													  this, callfunc_selector(ShopPopup::successAction), this, callfunc_selector(ShopPopup::failAction));
	}
}

void ShopPopup::successAction()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
	
	if(ing_download_cnt < df_list.size())
	{
		ing_download_cnt++;
		startDownload();
	}
	else
	{
		for(int i=0;i<cf_list.size();i++)
		{
			CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
			target_img->setAnchorPoint(ccp(0,0));
			
			if(cf_list[i].is_ani)
			{
				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
				target_img->addChild(ani_img);
			}
			
			target_img->setScale(0.2f);
			
			CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
			t_texture->setSprite(target_img);
			t_texture->begin();
			t_texture->getSprite()->visit();
			t_texture->end();
			
			t_texture->saveToFile(cf_list[i].to_filename.c_str(), kCCImageFormatPNG);
		}
		
		// 완료
		loading_layer->removeFromParent();
		
		if(mySGD->isHasGottenCards(gacha_card_number) > 0)
		{
//			myDSH->setIntegerForKey(kDSH_Key_cardDurability_int1, gacha_card_number, myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, gacha_card_number));
			
			// 강화
			float strength_rate;// = ((NSDS_GI(kSDS_CI_int1_rank_i, gacha_card_number)*10.f + 1)*NSDS_GI(kSDS_CI_int1_durability_i, gacha_card_number))/((NSDS_GI(kSDS_CI_int1_rank_i, gacha_card_number)*10.f + myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, gacha_card_number))*myDSH->getIntegerForKey(kDSH_Key_cardMaxDurability_int1, gacha_card_number));
			CCLOG("strength_rate : %.3f", strength_rate);
			
			random_device rd;
			default_random_engine e1(rd());
			uniform_real_distribution<float> uniform_dist(0.f, 1.f);
			
			float result_value = uniform_dist(e1);
			CCLOG("result value : %.3f", result_value);
			
			CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",gacha_card_number)->getCString());
			CardCase* cardCase = CardCase::create(gacha_card_number);
			card->addChild(cardCase);
			
			
			CCSprite* card2 = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",gacha_card_number)->getCString());
			CardCase* cardCase2 = CardCase::create(gacha_card_number, 1, NSDS_GS(kSDS_CI_int1_passive_s, gacha_card_number));
			card2->addChild(cardCase2);
			
			
			StrengthCardAnimation* b = StrengthCardAnimation::create(card,card2,-350);
			
			b->setCloseFunc([this](){
				CCLOG("close Func");
				this->is_menu_enable = true;
			});
			
			if(result_value <= strength_rate)
			{
				CCLOG("success");
				
				b->startSuccess("카드레벨 +1");
				
//				myDSH->setIntegerForKey(kDSH_Key_cardLevel_int1, gacha_card_number, myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, gacha_card_number)+1);
				myDSH->saveUserData({kSaveUserData_Key_cardsInfo}, nullptr);
			}
			else
			{
				CCLOG("fail");
				
				b->startFail("강화 실패");
			}
			
			addChild(b, kSP_Z_popup);
		}
		else
		{
			if(mySGD->isHasGottenCards(gacha_card_number) == 0)
			{
				mySGD->addHasGottenCardNumber(gacha_card_number);
			}
			else
			{
				
			}
			
			// 획득
			CCSprite* card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png",gacha_card_number)->getCString());
			CardCase* cardCase = CardCase::create(gacha_card_number);
			card->addChild(cardCase);
			
			TakeCardAnimation* b = TakeCardAnimation::create(card,-350);
			b->setCloseFunc([this](){
				CCLOG("close Func");
				this->is_menu_enable = true;
			});
			b->start();
			addChild(b, kSP_Z_popup);
		}
	}
}

void ShopPopup::failAction()
{
	is_downloading = false;
	
	replay_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, touch_priority-50);
	replay_menu->setPosition(ccp(240,100));
	replay_menu->setFunction([=](CCObject* sender)
							 {
								 CCNode* t_node = CCNode::create();
								 t_node->setTag(2);
								 replayAction(t_node);
							 });
	
	addChild(replay_menu, kSP_Z_popup);
}

void ShopPopup::replayAction(CCObject* sender)
{
	int tag = ((CCNode*)sender)->getTag();
	
	replay_menu->removeFromParent();
	
	if(tag == 1)
		startCardGacha();
	else if(tag == 2)
	{
		is_downloading = true;
		startDownload();
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
	pDispatcher->addTargetedDelegate(this, touch_priority, true);
}

void ShopPopup::requestItemDelivery()
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getMemberID();
	GraphDog::get()->command("requestItemDelivery", param, [=](Json::Value t){
		if(t["result"]["code"].asInt() == GDSUCCESS)
		{
			CCLOG("inapp success!! refresh!!!");
			
			mySGD->initProperties(t["list"]);
//			mySGD->refreshGoodsData(t["list"]["type"].asString(), t["list"]["count"].asInt());
			
			loading_layer->removeFromParent();
			is_menu_enable = true;
		}
		else if(t["result"]["code"].asInt() == 2016) // GDNOTINGWORK
		{
			addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
		}
		else
		{
			addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
		}
	});
}
