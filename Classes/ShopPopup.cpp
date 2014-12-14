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
#include "AchieveNoti.h"
#include "CommonAnimation.h"
#include "StyledLabelTTF.h"
#include "FiveRocksCpp.h"
#include "CardGachaPopup.h"
#include "ConvexGraph.h"

enum ShopPopup_Zorder{
	kSP_Z_gray = 0,
	kSP_Z_back,
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
	kSP_MT_p1,
	kSP_MT_eventPack,
	kSP_MT_gacha,
	kSP_MT_cardGacha,
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

//string ShopPopup::getPriceData(string t_code)
//{
//	string return_value;
//	
//	if(t_code == "wonToRuby1")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 0);
//	else if(t_code == "wonToRuby2")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 1);
//	else if(t_code == "wonToRuby3")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 2);
//	else if(t_code == "wonToRuby4")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 3);
//	else if(t_code == "wonToRuby5")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 4);
//	else if(t_code == "wonToRuby6")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_priceName_s, 5);
//	else if(t_code == "rubyFromWon1")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 0);
//	else if(t_code == "rubyFromWon2")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 1);
//	else if(t_code == "rubyFromWon3")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 2);
//	else if(t_code == "rubyFromWon4")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 3);
//	else if(t_code == "rubyFromWon5")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 4);
//	else if(t_code == "rubyFromWon6")
//		return_value = NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, 5);
//	else if(t_code == "rubyToGold1")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 0);
//	else if(t_code == "rubyToGold2")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 1);
//	else if(t_code == "rubyToGold3")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 2);
//	else if(t_code == "rubyToGold4")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 3);
//	else if(t_code == "rubyToGold5")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 4);
//	else if(t_code == "rubyToGold6")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_priceName_s, 5);
//	else if(t_code == "goldFromRuby1")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 0);
//	else if(t_code == "goldFromRuby2")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 1);
//	else if(t_code == "goldFromRuby3")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 2);
//	else if(t_code == "goldFromRuby4")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 3);
//	else if(t_code == "goldFromRuby5")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 4);
//	else if(t_code == "goldFromRuby6")
//		return_value = NSDS_GS(kSDS_GI_shopGold_int1_countName_s, 5);
//	else if(t_code == "rubyToHeart1")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 0);
//	else if(t_code == "rubyToHeart2")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 1);
//	else if(t_code == "rubyToHeart3")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 2);
//	else if(t_code == "rubyToHeart4")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 3);
//	else if(t_code == "rubyToHeart5")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 4);
//	else if(t_code == "rubyToHeart6")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_priceName_s, 5);
//	else if(t_code == "heartFromRuby1")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 0);
//	else if(t_code == "heartFromRuby2")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 1);
//	else if(t_code == "heartFromRuby3")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 2);
//	else if(t_code == "heartFromRuby4")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 3);
//	else if(t_code == "heartFromRuby5")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 4);
//	else if(t_code == "heartFromRuby6")
//		return_value = NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, 5);
//	
//	CCLOG("%s : %s", t_code.c_str(), return_value.c_str());
//	
//	return return_value;
//}

void ShopPopup::addPriceReward(CCNode *t_target, int t_number)
{
	if(recent_shop_code == kSC_ruby)
	{
		StyledLabelTTF* ruby_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_shopRuby_int1_countName_s, t_number-1).c_str(), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
		ruby_label->setAnchorPoint(ccp(0.5f,0.5f));
		ruby_label->setPosition(ccpFromSize(t_target->getContentSize()/2.f) + ccp(0,-4));
		t_target->addChild(ruby_label);
		
//		KSLabelTTF* ruby_label = KSLabelTTF::create(getPriceData(CCString::createWithFormat("rubyFromWon%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 14);
//		ruby_label->setColor(ccc3(55, 35, 150));
//		ruby_label->disableOuterStroke();
//		ruby_label->setAnchorPoint(ccp(1.f,0.5f));
//		ruby_label->setPosition(ccpFromSize(t_target->getContentSize()) + ccp(-12,-18));
//		t_target->addChild(ruby_label);
	}
	else if(recent_shop_code == kSC_gold)
	{
		StyledLabelTTF* gold_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_shopGold_int1_countName_s, t_number-1).c_str(), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
		gold_label->setAnchorPoint(ccp(0.5f,0.5f));
		gold_label->setPosition(ccpFromSize(t_target->getContentSize()/2.f) + ccp(0,-4));
		t_target->addChild(gold_label);
		
//		KSLabelTTF* gold_label = KSLabelTTF::create(getPriceData(CCString::createWithFormat("goldFromRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 14);
//		gold_label->setColor(ccc3(55, 35, 150));
//		gold_label->disableOuterStroke();
//		gold_label->setAnchorPoint(ccp(1.f,0.5f));
//		gold_label->setPosition(ccpFromSize(t_target->getContentSize()) + ccp(-12,-18));
//		t_target->addChild(gold_label);
	}
	else if(recent_shop_code == kSC_heart)
	{
		StyledLabelTTF* heart_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_shopCoin_int1_countName_s, t_number-1).c_str(), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
		heart_label->setAnchorPoint(ccp(0.5f,0.5f));
		heart_label->setPosition(ccpFromSize(t_target->getContentSize()/2.f) + ccp(0,-4));
		t_target->addChild(heart_label);
		
//		KSLabelTTF* heart_label = KSLabelTTF::create(getPriceData(CCString::createWithFormat("heartFromRuby%d", t_number)->getCString()).c_str(), mySGD->getFont().c_str(), 14);
//		heart_label->setColor(ccc3(55, 35, 150));
//		heart_label->disableOuterStroke();
//		heart_label->setAnchorPoint(ccp(1.f,0.5f));
//		heart_label->setPosition(ccpFromSize(t_target->getContentSize()) + ccp(-12,-18));
//		t_target->addChild(heart_label);
	}
	else if(recent_shop_code == kSC_p1)
	{
		StyledLabelTTF* p1_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_shopP1_int1_countName_s, t_number-1).c_str(), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
		p1_label->setAnchorPoint(ccp(0.5f,0.5f));
		p1_label->setPosition(ccpFromSize(t_target->getContentSize()/2.f) + ccp(0,-4));
		t_target->addChild(p1_label);
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
	if(before_code == ShopBeforeCode::kShopBeforeCode_mainflow)
		addGachaButton();
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
	else if(recent_shop_code == kSC_p1)
		p1_menu->setEnabled(true);
	else if(recent_shop_code == kSC_eventPack)
	{
		if(eventPack_menu)
			eventPack_menu->setEnabled(true);
	}
	
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
	else if(t_code == kSC_p1)
		p1_menu->setEnabled(false);
	else if(t_code == kSC_eventPack)
	{
		if(eventPack_menu)
			eventPack_menu->setEnabled(false);
	}
	
	
	if(recent_shop_code == kSC_character)
	{
		character_table->removeFromParent();
		suction->removeFromParent();
	}
	else if(recent_shop_code == kSC_eventPack)
	{
		if(eventPack_node)
		{
			eventPack_node->removeFromParent();
			eventPack_node = NULL;
		}
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
	else if(recent_shop_code == kSC_eventPack)
	{
		chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
		std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
		
		time_t sub_time = now_time_t - myDSH->getIntegerForKey(kDSH_Key_savedStartPackFirstTime);
		
		int expireSec = NSDS_GI(kSDS_GI_shopStartPack_expireSec_i);
		
		bool is_on_time_startPack = sub_time <= expireSec;
		bool is_buyed_startPack = NSDS_GI(kSDS_GI_shopStartPack_no_i) == mySGD->getUserdataOnlyOneBuyPack();
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool is_have_eventPack = NSDS_GB(kSDS_GI_shopEventPack_isHave_b);
		bool is_just_one = NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b);
		bool is_buyed_eventPack = NSDS_GI(kSDS_GI_shopEventPack_no_i) == mySGD->getUserdataOnlyOneBuyPack();
		bool is_on_time_eventPack = false;
		
		tm* now_tm = localtime(&now_time_t);
		string startDate = NSDS_GS(kSDS_GI_shopEventPack_startDate_s);
		string endDate = NSDS_GS(kSDS_GI_shopEventPack_endDate_s);
		
		int now_time_number = atoi((string("") + ccsf("%04d", now_tm->tm_year+1900) + ccsf("%02d", now_tm->tm_mon+1) + ccsf("%02d", now_tm->tm_mday)).c_str());
		int now_time_hms = atoi((string("") + ccsf("%02d", now_tm->tm_hour) + ccsf("%02d", now_tm->tm_min) + ccsf("%02d", now_tm->tm_sec)).c_str());
		
		if(atoi(startDate.substr(0,8).c_str()) <= now_time_number &&
		   atoi(endDate.substr(0,8).c_str()) >= now_time_number &&
		   NSDS_GI(kSDS_GI_shopEventPack_startTime_i) <= now_time_hms &&
		   NSDS_GI(kSDS_GI_shopEventPack_endTime_i) >= now_time_hms)
		{
			is_on_time_eventPack = true;
		}
		
		bool is_useable_eventPack = true;
		if(is_have_eventPack && is_on_time_eventPack)
		{
			if(is_just_one)
			{
				if(!is_buyed_eventPack)
					is_useable_eventPack = true;
				else
					is_useable_eventPack = false;
			}
			else
				is_useable_eventPack = true;
		}
		else
		{
			is_useable_eventPack = false;
		}
		
		
		
		if(!is_buyed_startPack && is_on_time_startPack) // 스타트팩을 구입한적 없는고 스타트팩 시간이 맞는 경우
		{
			// 스타트팩
			eventPack_node = CCNode::create();
			eventPack_node->setPosition(ccpFromSize(main_case->getContentSize()/2.f));
			main_case->addChild(eventPack_node, kSP_Z_content);
			
			CCSprite* n_img = mySIL->getLoadedImg("start_pack.png");
			CCSprite* s_img = mySIL->getLoadedImg("start_pack.png");
			s_img->setColor(ccGRAY);
			
			CCMenuItem* img_item = CCMenuItemSprite::create(n_img, s_img, this, menu_selector(ShopPopup::buyStartPack));
			
			CCMenu* img_menu = CCMenu::createWithItem(img_item);
			img_menu->setPosition(ccp(0,-15));
			eventPack_node->addChild(img_menu);
			
			img_menu->setTouchPriority(touch_priority-3);
		}
		else if(is_useable_eventPack) // 이벤트팩이 존재하고 해당 이벤트팩의 시간이 맞는 경우
		{
			// 이벤트팩
			eventPack_node = CCNode::create();
			eventPack_node->setPosition(ccpFromSize(main_case->getContentSize()/2.f));
			main_case->addChild(eventPack_node, kSP_Z_content);
			
			CCSprite* n_img = mySIL->getLoadedImg("event_pack.png");
			CCSprite* s_img = mySIL->getLoadedImg("event_pack.png");
			s_img->setColor(ccGRAY);
			
			CCMenuItem* img_item = CCMenuItemSprite::create(n_img, s_img, this, menu_selector(ShopPopup::buyEventPack));
			
			CCMenu* img_menu = CCMenu::createWithItem(img_item);
			img_menu->setPosition(ccp(0,-15));
			eventPack_node->addChild(img_menu);
			
			img_menu->setTouchPriority(touch_priority-3);
		}
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
		else if(recent_shop_code == kSC_p1)
			filename = "shop_p1_%d.png";
		
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
			SDS_KEY price_key;
			if(recent_shop_code == kSC_ruby)
			{
				sale_str = NSDS_GS(kSDS_GI_shopRuby_int1_sale_s, i-1);
				price_type = mySGD->getGoodsTypeToKey(kGoodsType_money);//NSDS_GS(kSDS_GI_shopRuby_int1_priceType_s, i-1);
				price_key = kSDS_GI_shopRuby_int1_priceName_s;
			}
			else if(recent_shop_code == kSC_gold)
			{
				sale_str = NSDS_GS(kSDS_GI_shopGold_int1_sale_s, i-1);
				price_type = mySGD->getGoodsTypeToKey(kGoodsType_ruby);//NSDS_GS(kSDS_GI_shopGold_int1_priceType_s, i-1);
				price_key = kSDS_GI_shopGold_int1_priceName_s;
			}
			else if(recent_shop_code == kSC_heart)
			{
				sale_str = NSDS_GS(kSDS_GI_shopCoin_int1_sale_s, i-1);
				price_type = mySGD->getGoodsTypeToKey(kGoodsType_ruby); //NSDS_GS(kSDS_GI_shopCoin_int1_priceType_s, i-1);
				price_key = kSDS_GI_shopCoin_int1_priceName_s;
			}
			else if(recent_shop_code == kSC_p1)
			{
				sale_str = NSDS_GS(kSDS_GI_shopP1_int1_sale_s, i-1);
				price_type = mySGD->getGoodsTypeToKey(kGoodsType_gold); //NSDS_GS(kSDS_GI_shopCoin_int1_priceType_s, i-1);
				price_key = kSDS_GI_shopP1_int1_priceName_s;
			}
			
			if(!sale_str.empty())
			{
				CCSprite* tab = CCSprite::create("shop_tab.png");
				tab->setPosition(ccp(-34,38.f));
				content_node->addChild(tab, 4);
				
				CCLabelTTF* sale_label = CCLabelTTF::create(sale_str.c_str(), mySGD->getFont().c_str(), 12);
				sale_label->setPosition(ccp(tab->getContentSize().width/2.f, tab->getContentSize().height/2.f));
				tab->addChild(sale_label);
			}
			
			if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_money))
			{
				KSLabelTTF* won_label = KSLabelTTF::create(NSDS_GS(price_key, i-1).c_str(), mySGD->getFont().c_str(), 14);
				won_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				won_label->setPosition(ccp(0, -27));
				content_node->addChild(won_label, 3);
			}
			else if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
			{
				KSLabelTTF* ruby_label = KSLabelTTF::create(NSDS_GS(price_key, i-1).c_str(), mySGD->getFont().c_str(), 14);
				ruby_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				ruby_label->setPosition(ccp(10, -27));
				content_node->addChild(ruby_label, 3);
				
				CCSprite* ruby_img = CCSprite::create("price_ruby_img.png");
				ruby_img->setPosition(ccp(-ruby_label->getContentSize().width/2.f-4, -27));
				content_node->addChild(ruby_img, 3);
				
				ruby_label->setPositionX(ruby_img->getContentSize().width/2.f-4);
			}
			else if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_gold))
			{
				KSLabelTTF* gold_label = KSLabelTTF::create(NSDS_GS(price_key, i-1).c_str(), mySGD->getFont().c_str(), 14);
				gold_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
				gold_label->setPosition(ccp(10, -27));
				content_node->addChild(gold_label, 3);
				
				CCSprite* gold_img = CCSprite::create("price_gold_img.png");
				gold_img->setPosition(ccp(-gold_label->getContentSize().width/2.f-2, -27));
				content_node->addChild(gold_img, 3);
				
				gold_label->setPositionX(gold_img->getContentSize().width/2.f-7);
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

void ShopPopup::buyStartPack(CCObject* sender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	is_menu_enable = false;
	
	createCheckBuyPopup([=]()
						{
							loading_layer = LoadingLayer::create();
							addChild(loading_layer, kSP_Z_popup);
							
							bool is_exchange = NSDS_GS(kSDS_GI_shopStartPack_pID_s) == "";
							
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//							mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopStartPack_exchangeID_s), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
//							
//							mySGD->setUserdataOnlyOneBuyPack(NSDS_GI(kSDS_GI_shopStartPack_no_i));
//							
//							vector<CommandParam> t_command_list;
//							t_command_list.clear();
//							t_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
//							mySGD->changeGoodsTransaction(t_command_list, [=](Json::Value result_data){
//								loading_layer->removeFromParent();
//								
//								if(result_data["result"]["code"].asInt() == GDSUCCESS)
//								{
//									if(is_continue)
//									{
//										is_menu_enable = false;
//										
//										if(is_add_gray)
//										{
//											addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
//																				   {
//																					   gray->setOpacity(255*t);
//																				   }, [=](float t)
//																				   {
//																					   gray->setOpacity(255*t);
//																					   gray->removeFromParent();
//																				   }));
//										}
//										
//										CommonAnimation::closePopup(this, main_case, nullptr, [=](){
//											
//										}, [=]()
//																	{
//																		if(target_final)
//																			(target_final->*delegate_final)();
//																		if(is_set_close_func)
//																			close_func();
//																		if(continue_end != nullptr)
//																			continue_end();
//																		removeFromParent();
//																	});
//									}
//									else
//									{
//										setShopCode(kSC_ruby);
//										is_menu_enable = true;
//									}
//									if(time_label)
//										time_label->setVisible(false);
//									eventPack_menu->setVisible(false);
//								}
//								else
//								{
//									is_menu_enable = true;
//									mySGD->clearChangeUserdata();
//									mySGD->clearChangeGoods();
//									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//								}
//							});
//							
////							mySGD->changeGoods([=](Json::Value result_data){
////								loading_layer->removeFromParent();
////								
////								if(result_data["result"]["code"].asInt() == GDSUCCESS)
////								{
////									if(is_continue)
////									{
////										is_menu_enable = false;
////										
////										if(is_add_gray)
////										{
////											addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
////																				   {
////																					   gray->setOpacity(255*t);
////																				   }, [=](float t)
////																				   {
////																					   gray->setOpacity(255*t);
////																					   gray->removeFromParent();
////																				   }));
////										}
////										
////										CommonAnimation::closePopup(this, main_case, nullptr, [=](){
////											
////										}, [=]()
////																	{
////																		if(target_final)
////																			(target_final->*delegate_final)();
////																		if(is_set_close_func)
////																			close_func();
////																		if(continue_end != nullptr)
////																			continue_end();
////																		removeFromParent();
////																	});
////									}
////									else
////									{
////										setShopCode(kSC_ruby);
////										is_menu_enable = true;
////									}
////								}
////								else
////								{
////									is_menu_enable = true;
////									mySGD->clearChangeGoods();
////									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
////								}
////							});
//							
//							
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//							
							if(is_exchange)
							{
								mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopStartPack_exchangeID_s), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
								
								mySGD->setUserdataOnlyOneBuyPack(NSDS_GI(kSDS_GI_shopStartPack_no_i));
								
								vector<CommandParam> t_command_list;
								t_command_list.clear();
								t_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
								mySGD->changeGoodsTransaction(t_command_list, [=](Json::Value result_data){
									loading_layer->removeFromParent();
									
									if(result_data["result"]["code"].asInt() == GDSUCCESS)
									{
										if(is_continue)
										{
											is_menu_enable = false;
											
											if(is_add_gray)
											{
												addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
																					   {
																						   gray->setOpacity(255*t);
																					   }, [=](float t)
																					   {
																						   gray->setOpacity(255*t);
																						   gray->removeFromParent();
																					   }));
											}
											
											CommonAnimation::closePopup(this, main_case, nullptr, [=](){
												
											}, [=]()
																		{
																			if(target_final)
																				(target_final->*delegate_final)();
																			if(is_set_close_func)
																				close_func();
																			if(continue_end != nullptr)
																				continue_end();
																			removeFromParent();
																		});
										}
										else
										{
											setShopCode(kSC_ruby);
											is_menu_enable = true;
										}
										if(time_label)
											time_label->setVisible(false);
										eventPack_menu->setVisible(false);
									}
									else
									{
										is_menu_enable = true;
										mySGD->clearChangeUserdata();
										mySGD->clearChangeGoods();
//										addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
									}
								});
								
//								mySGD->changeGoods([=](Json::Value result_data){
//									loading_layer->removeFromParent();
//									
//									if(result_data["result"]["code"].asInt() == GDSUCCESS)
//									{
//										if(is_continue)
//										{
//											is_menu_enable = false;
//											
//											if(is_add_gray)
//											{
//												addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
//																					   {
//																						   gray->setOpacity(255*t);
//																					   }, [=](float t)
//																					   {
//																						   gray->setOpacity(255*t);
//																						   gray->removeFromParent();
//																					   }));
//											}
//											
//											CommonAnimation::closePopup(this, main_case, nullptr, [=](){
//												
//											}, [=]()
//																		{
//																			if(target_final)
//																				(target_final->*delegate_final)();
//																			if(is_set_close_func)
//																				close_func();
//																			if(continue_end != nullptr)
//																				continue_end();
//																			removeFromParent();
//																		});
//										}
//										else
//										{
//											setShopCode(kSC_ruby);
//											is_menu_enable = true;
//										}
//									}
//									else
//									{
//										is_menu_enable = true;
//										mySGD->clearChangeGoods();
//										addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//									}
//								});
							}
							else
							{
								Json::Value param;
								param["productid"] = NSDS_GS(kSDS_GI_shopStartPack_pID_s);
								hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
									KS::KSLog("in-app test \n%", v);
									if(v["issuccess"].asInt())
									{
                                        Json::Value t_info = mySGD->getProductInfo(NSDS_GS(kSDS_GI_shopStartPack_pID_s));
                                        if(!t_info.empty())
										{
                                            myHSP->getAdXConnectEventInstance("Sale", t_info["price"].asString().c_str(), t_info["currency"].asString().c_str());
											fiverocks::FiveRocksBridge::trackPurchase("ShopPurchaseStartPack", t_info["currency"].asString().c_str(), t_info["price"].asDouble(), "");
											myHSP->IgawAdbrixBuy("ShopPurchaseStartPack");
										}
										requestItemDeliveryStartPack();
									}
									else
									{
										loading_layer->removeFromParent();
										
//										addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
										
										is_menu_enable = true;
									}
								});
							}
//#endif
						});
}

void ShopPopup::buyEventPack(CCObject* sender)
{
	if(!is_menu_enable)
	{
		return;
	}
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	is_menu_enable = false;
	
	createCheckBuyPopup([=]()
						{
							loading_layer = LoadingLayer::create();
							addChild(loading_layer, kSP_Z_popup);
							
							bool is_exchange = NSDS_GS(kSDS_GI_shopEventPack_pID_s) == "";
							
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//							mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopStartPack_exchangeID_s), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
//							
//							vector<CommandParam> t_command_list;
//							t_command_list.clear();
//							if(NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b))
//							{
//								mySGD->setUserdataOnlyOneBuyPack(NSDS_GI(kSDS_GI_shopEventPack_no_i));
//								t_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
//							}
//							
//							mySGD->changeGoodsTransaction(t_command_list, [=](Json::Value result_data){
//								loading_layer->removeFromParent();
//								
//								if(result_data["result"]["code"].asInt() == GDSUCCESS)
//								{
//									if(is_continue)
//									{
//										is_menu_enable = false;
//										
//										if(is_add_gray)
//										{
//											addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
//																				   {
//																					   gray->setOpacity(255*t);
//																				   }, [=](float t)
//																				   {
//																					   gray->setOpacity(255*t);
//																					   gray->removeFromParent();
//																				   }));
//										}
//										
//										CommonAnimation::closePopup(this, main_case, nullptr, [=](){
//											
//										}, [=]()
//																	{
//																		if(target_final)
//																			(target_final->*delegate_final)();
//																		if(is_set_close_func)
//																			close_func();
//																		if(continue_end != nullptr)
//																			continue_end();
//																		removeFromParent();
//																	});
//									}
//									else
//									{
//										is_menu_enable = true;
//									}
//									if(NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b))
//									{
//										if(time_label)
//											time_label->setVisible(false);
//										eventPack_menu->setVisible(false);
//										setShopCode(kSC_ruby);
//									}
//								}
//								else
//								{
//									is_menu_enable = true;
//									if(NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b))
//										mySGD->clearChangeUserdata();
//									mySGD->clearChangeGoods();
//									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//								}
//							});
//							
//							//							mySGD->changeGoods([=](Json::Value result_data){
//							//								loading_layer->removeFromParent();
//							//
//							//								if(result_data["result"]["code"].asInt() == GDSUCCESS)
//							//								{
//							//									if(is_continue)
//							//									{
//							//										is_menu_enable = false;
//							//
//							//										if(is_add_gray)
//							//										{
//							//											addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
//							//																				   {
//							//																					   gray->setOpacity(255*t);
//							//																				   }, [=](float t)
//							//																				   {
//							//																					   gray->setOpacity(255*t);
//							//																					   gray->removeFromParent();
//							//																				   }));
//							//										}
//							//
//							//										CommonAnimation::closePopup(this, main_case, nullptr, [=](){
//							//
//							//										}, [=]()
//							//																	{
//							//																		if(target_final)
//							//																			(target_final->*delegate_final)();
//							//																		if(is_set_close_func)
//							//																			close_func();
//							//																		if(continue_end != nullptr)
//							//																			continue_end();
//							//																		removeFromParent();
//							//																	});
//							//									}
//							//									else
//							//									{
//							//										setShopCode(kSC_ruby);
//							//										is_menu_enable = true;
//							//									}
//							//								}
//							//								else
//							//								{
//							//									is_menu_enable = true;
//							//									mySGD->clearChangeGoods();
//							//									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//							//								}
//							//							});
//							
//							
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//							
							if(is_exchange)
							{
								mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopStartPack_exchangeID_s), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
								
								vector<CommandParam> t_command_list;
								t_command_list.clear();
								if(NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b))
								{
									mySGD->setUserdataOnlyOneBuyPack(NSDS_GI(kSDS_GI_shopEventPack_no_i));
									t_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
								}
								
								mySGD->changeGoodsTransaction(t_command_list, [=](Json::Value result_data){
									loading_layer->removeFromParent();
									
									if(result_data["result"]["code"].asInt() == GDSUCCESS)
									{
										if(is_continue)
										{
											is_menu_enable = false;
											
											if(is_add_gray)
											{
												addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
																					   {
																						   gray->setOpacity(255*t);
																					   }, [=](float t)
																					   {
																						   gray->setOpacity(255*t);
																						   gray->removeFromParent();
																					   }));
											}
											
											CommonAnimation::closePopup(this, main_case, nullptr, [=](){
												
											}, [=]()
																		{
																			if(target_final)
																				(target_final->*delegate_final)();
																			if(is_set_close_func)
																				close_func();
																			if(continue_end != nullptr)
																				continue_end();
																			removeFromParent();
																		});
										}
										else
										{
											is_menu_enable = true;
										}
										if(NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b))
										{
											if(time_label)
												time_label->setVisible(false);
											eventPack_menu->setVisible(false);
											setShopCode(kSC_ruby);
										}
									}
									else
									{
										is_menu_enable = true;
										if(NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b))
											mySGD->clearChangeUserdata();
										mySGD->clearChangeGoods();
//										addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
									}
								});
								
								//								mySGD->changeGoods([=](Json::Value result_data){
								//									loading_layer->removeFromParent();
								//
								//									if(result_data["result"]["code"].asInt() == GDSUCCESS)
								//									{
								//										if(is_continue)
								//										{
								//											is_menu_enable = false;
								//
								//											if(is_add_gray)
								//											{
								//												addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
								//																					   {
								//																						   gray->setOpacity(255*t);
								//																					   }, [=](float t)
								//																					   {
								//																						   gray->setOpacity(255*t);
								//																						   gray->removeFromParent();
								//																					   }));
								//											}
								//
								//											CommonAnimation::closePopup(this, main_case, nullptr, [=](){
								//
								//											}, [=]()
								//																		{
								//																			if(target_final)
								//																				(target_final->*delegate_final)();
								//																			if(is_set_close_func)
								//																				close_func();
								//																			if(continue_end != nullptr)
								//																				continue_end();
								//																			removeFromParent();
								//																		});
								//										}
								//										else
								//										{
								//											setShopCode(kSC_ruby);
								//											is_menu_enable = true;
								//										}
								//									}
								//									else
								//									{
								//										is_menu_enable = true;
								//										mySGD->clearChangeGoods();
								//										addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
								//									}
								//								});
							}
							else
							{
								Json::Value param;
								param["productid"] = NSDS_GS(kSDS_GI_shopEventPack_pID_s);
								hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
									KS::KSLog("in-app test \n%", v);
									if(v["issuccess"].asInt())
									{
                                        Json::Value t_info = mySGD->getProductInfo(NSDS_GS(kSDS_GI_shopEventPack_pID_s));
                                        if(!t_info.empty())
										{
                                            myHSP->getAdXConnectEventInstance("Sale", t_info["price"].asString().c_str(), t_info["currency"].asString().c_str());
											fiverocks::FiveRocksBridge::trackPurchase("ShopPurchaseEventPack", t_info["currency"].asString().c_str(), t_info["price"].asDouble(), "");
											myHSP->IgawAdbrixBuy("ShopPurchaseEventPack");
										}
										requestItemDeliveryEventPack();
									}
									else
									{
										loading_layer->removeFromParent();
										
//										addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
										
										is_menu_enable = true;
									}
								});
							}
//#endif
						});
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
				addChild(ASPopupView::getCommonNoti(touch_priority-10, myLoc->getLocalForKey(LK::kMyLocalKey_goldNotEnought), [=](){is_menu_enable = true;}, CCPointZero), kSP_Z_popup);
			else if(condition_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
				addChild(ASPopupView::getCommonNoti(touch_priority-10, myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought), [=](){is_menu_enable = true;}, CCPointZero), kSP_Z_popup);
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
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
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
	
//	CCLabelTTF* comment_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_comment_s, idx+1).c_str(), mySGD->getFont().c_str(), 11);
//	comment_label->setPosition(ccp(62,58));
//	cell->addChild(comment_label, kCharacterCellZorder_content);
	
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

void ShopPopup::addGachaButton()
{
	CCSprite* n_gacha_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gacha), mySGD->getFont().c_str(), 12.5f);
	n_gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_gacha_label->setPosition(ccpFromSize(n_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	n_gacha_img->addChild(n_gacha_label);
	
	CCSprite* s_gacha_img = CCSprite::create("subbutton_pink.png");
	s_gacha_img->setColor(ccGRAY);
	KSLabelTTF* s_gacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_gacha), mySGD->getFont().c_str(), 12.5f);
	s_gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_gacha_label->setPosition(ccpFromSize(s_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	s_gacha_img->addChild(s_gacha_label);
	
	
	CCMenuItem* gacha_menu = CCMenuItemSprite::create(n_gacha_img, s_gacha_img, this, menu_selector(ShopPopup::menuAction));
	gacha_menu->setTag(kSP_MT_gacha);
	gacha_menu->setPosition(ccp(395,13));
	tab_menu->addChild(gacha_menu);
	
	
	CCSprite* n_card_gacha_img = CCSprite::create("subbutton_pink.png");
	KSLabelTTF* n_card_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardGacha), mySGD->getFont().c_str(), 12.5f);
	n_card_gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_card_gacha_label->setPosition(ccpFromSize(n_card_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	n_card_gacha_img->addChild(n_card_gacha_label);
	
	CCSprite* s_card_gacha_img = CCSprite::create("subbutton_pink.png");
	s_card_gacha_img->setColor(ccGRAY);
	KSLabelTTF* s_card_gacha_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardGacha), mySGD->getFont().c_str(), 12.5f);
	s_card_gacha_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_card_gacha_label->setPosition(ccpFromSize(s_card_gacha_img->getContentSize()/2.f) + ccp(0,-1));
	s_card_gacha_img->addChild(s_card_gacha_label);
	
	
	CCMenuItem* card_gacha_menu = CCMenuItemSprite::create(n_card_gacha_img, s_card_gacha_img, this, menu_selector(ShopPopup::menuAction));
	card_gacha_menu->setTag(kSP_MT_cardGacha);
	card_gacha_menu->setPosition(ccp(293,13));
	tab_menu->addChild(card_gacha_menu);
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
	
	is_add_gray = false;
	
	is_continue = false;
	continue_end = nullptr;
	
	success_func = nullptr;
	
    is_use_goods_type_gold = false;
    
	is_set_close_func = false;
	target_heartTime = NULL;
	
	eventPack_menu = NULL;
	eventPack_node = NULL;
	
	touch_priority = -400;
	
	server_character_count = NSDS_GI(kSDS_GI_characterCount_i);
	
	is_menu_enable = false;
	
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240, 160 - 14));
	addChild(main_case, kSP_Z_back);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_shop), mySGD->getFont().c_str(), 15);
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);

	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 204));
	main_inner->setPosition(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f);
	main_case->addChild(main_inner);
	
	CommonButton* close_menu = CommonButton::createCloseButton(touch_priority-4);
	close_menu->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
							{
								CCNode* t_node = CCNode::create();
								t_node->setTag(kSP_MT_close);
								menuAction(t_node);
							});
	main_case->addChild(close_menu, kSP_Z_content);

	
	character_menu = CommonButton::create("무기상점", 12, CCSizeMake(83,38), CommonButtonPupple, touch_priority-4);
	character_menu->setTitleColor(ccWHITE);
	character_menu->setBackgroundTypeForDisabled(CommonButtonYellowUp);
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
	card_menu->setBackgroundTypeForDisabled(CommonButtonYellowUp);
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
	
	tab_menu = CCMenu::create();
	tab_menu->setPosition(CCPointZero);
	main_case->addChild(tab_menu, kSP_Z_content);
	
	tab_menu->setTouchPriority(touch_priority-5);
	
	CCSprite* n_ruby_img = CCSprite::create("shoptap_down.png");
	KSLabelTTF* n_ruby_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rubyShop), mySGD->getFont().c_str(), 12.5f);
	n_ruby_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_ruby_label->disableOuterStroke();
	n_ruby_label->setPosition(ccpFromSize(n_ruby_img->getContentSize()/2.f) + ccp(0,2));
	n_ruby_img->addChild(n_ruby_label);
	
	CCSprite* s_ruby_img = CCSprite::create("shoptap_down.png");
	s_ruby_img->setColor(ccGRAY);
	KSLabelTTF* s_ruby_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rubyShop), mySGD->getFont().c_str(), 12.5f);
	s_ruby_label->setColor(ccGRAY);
	s_ruby_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_ruby_label->setPosition(ccpFromSize(s_ruby_img->getContentSize()/2.f) + ccp(0,2));
	s_ruby_img->addChild(s_ruby_label);
	
	CCSprite* d_ruby_img = CCSprite::create("shoptap_up.png");
	KSLabelTTF* d_ruby_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rubyShop), mySGD->getFont().c_str(), 12.5f);
	d_ruby_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_ruby_label->setPosition(ccpFromSize(d_ruby_img->getContentSize()/2.f) + ccp(0,2));
	d_ruby_img->addChild(d_ruby_label);
	
	
	ruby_menu = CCMenuItemSprite::create(n_ruby_img, s_ruby_img, d_ruby_img, this, menu_selector(ShopPopup::menuAction));
	ruby_menu->setTag(kSP_MT_ruby);
	ruby_menu->setPosition(getContentPosition(kSP_MT_ruby));
	tab_menu->addChild(ruby_menu, kSP_Z_content);
	
	
	CCSprite* n_gold_img = CCSprite::create("shoptap_down.png");
	KSLabelTTF* n_gold_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_goldShop), mySGD->getFont().c_str(), 12.5f);
	n_gold_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_gold_label->setPosition(ccpFromSize(n_gold_img->getContentSize()/2.f) + ccp(0,2));
	n_gold_img->addChild(n_gold_label);
	
	CCSprite* s_gold_img = CCSprite::create("shoptap_down.png");
	s_gold_img->setColor(ccGRAY);
	KSLabelTTF* s_gold_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_goldShop), mySGD->getFont().c_str(), 12.5f);
	s_gold_label->setColor(ccGRAY);
	s_gold_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_gold_label->setPosition(ccpFromSize(s_gold_img->getContentSize()/2.f) + ccp(0,2));
	s_gold_img->addChild(s_gold_label);
	
	CCSprite* d_gold_img = CCSprite::create("shoptap_up.png");
	KSLabelTTF* d_gold_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_goldShop), mySGD->getFont().c_str(), 12.5f);
	d_gold_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_gold_label->setPosition(ccpFromSize(d_gold_img->getContentSize()/2.f) + ccp(0,2));
	d_gold_img->addChild(d_gold_label);
	
	
	gold_menu = CCMenuItemSprite::create(n_gold_img, s_gold_img, d_gold_img, this, menu_selector(ShopPopup::menuAction));
	gold_menu->setTag(kSP_MT_gold);
	gold_menu->setPosition(getContentPosition(kSP_MT_gold));
	tab_menu->addChild(gold_menu, kSP_Z_content);
	
	
	CCSprite* n_heart_img = CCSprite::create("shoptap_down.png");
	KSLabelTTF* n_heart_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_heartShop), mySGD->getFont().c_str(), 12.5f);
	n_heart_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_heart_label->setPosition(ccpFromSize(n_heart_img->getContentSize()/2.f) + ccp(0,2));
	n_heart_img->addChild(n_heart_label);
	
	CCSprite* s_heart_img = CCSprite::create("shoptap_down.png");
	s_heart_img->setColor(ccGRAY);
	KSLabelTTF* s_heart_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_heartShop), mySGD->getFont().c_str(), 12.5f);
	s_heart_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_heart_label->setColor(ccGRAY);
	s_heart_label->setPosition(ccpFromSize(s_heart_img->getContentSize()/2.f) + ccp(0,2));
	s_heart_img->addChild(s_heart_label);
	
	CCSprite* d_heart_img = CCSprite::create("shoptap_up.png");
	KSLabelTTF* d_heart_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_heartShop), mySGD->getFont().c_str(), 12.5f);
	d_heart_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_heart_label->setPosition(ccpFromSize(d_heart_img->getContentSize()/2.f) + ccp(0,2));
	d_heart_img->addChild(d_heart_label);
	
	
	heart_menu = CCMenuItemSprite::create(n_heart_img, s_heart_img, d_heart_img, this, menu_selector(ShopPopup::menuAction));
	heart_menu->setTag(kSP_MT_heart);
	heart_menu->setPosition(getContentPosition(kSP_MT_heart));
	tab_menu->addChild(heart_menu, kSP_Z_content);
	
	
	CCSprite* n_p1_img = CCSprite::create("shoptap_down.png");
	KSLabelTTF* n_p1_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_p1Shop), mySGD->getFont().c_str(), 12.5f);
	n_p1_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	n_p1_label->setPosition(ccpFromSize(n_p1_img->getContentSize()/2.f) + ccp(0,2));
	n_p1_img->addChild(n_p1_label);
	
	CCSprite* s_p1_img = CCSprite::create("shoptap_down.png");
	s_p1_img->setColor(ccGRAY);
	KSLabelTTF* s_p1_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_p1Shop), mySGD->getFont().c_str(), 12.5f);
	s_p1_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	s_p1_label->setColor(ccGRAY);
	s_p1_label->setPosition(ccpFromSize(s_p1_img->getContentSize()/2.f) + ccp(0,2));
	s_p1_img->addChild(s_p1_label);
	
	CCSprite* d_p1_img = CCSprite::create("shoptap_up.png");
	KSLabelTTF* d_p1_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_p1Shop), mySGD->getFont().c_str(), 12.5f);
	d_p1_label->enableOuterStroke(ccBLACK, 0.3f, 50, true);
	d_p1_label->setPosition(ccpFromSize(d_p1_img->getContentSize()/2.f) + ccp(0,2));
	d_p1_img->addChild(d_p1_label);
	
	
	p1_menu = CCMenuItemSprite::create(n_p1_img, s_p1_img, d_p1_img, this, menu_selector(ShopPopup::menuAction));
	p1_menu->setTag(kSP_MT_p1);
	p1_menu->setPosition(getContentPosition(kSP_MT_p1));
	tab_menu->addChild(p1_menu, kSP_Z_content);
	
	time_label = NULL;
	
	
	chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
	std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
	///////////////////////// 스타트팩 보여줄 수 있는지 판단 /////////////////////////////////////
	time_t sub_time = now_time_t - myDSH->getIntegerForKey(kDSH_Key_savedStartPackFirstTime);
	
	int expireSec = NSDS_GI(kSDS_GI_shopStartPack_expireSec_i);
	
	bool is_on_time_startPack = sub_time <= expireSec;
	bool is_buyed_startPack = NSDS_GI(kSDS_GI_shopStartPack_no_i) == mySGD->getUserdataOnlyOneBuyPack();
	///////////////////////// 이벤트팩 보여줄 수 있는지 판단 ////////////////////////////////////
	bool is_have_eventPack = NSDS_GB(kSDS_GI_shopEventPack_isHave_b);
	bool is_just_one = NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b);
	bool is_buyed_eventPack = NSDS_GI(kSDS_GI_shopEventPack_no_i) == mySGD->getUserdataOnlyOneBuyPack();
	bool is_on_time_eventPack = false;
	
	tm* now_tm = localtime(&now_time_t);
	string startDate = NSDS_GS(kSDS_GI_shopEventPack_startDate_s);
	string endDate = NSDS_GS(kSDS_GI_shopEventPack_endDate_s);
	
	int now_time_number = atoi((string("") + ccsf("%04d", now_tm->tm_year+1900) + ccsf("%02d", now_tm->tm_mon+1) + ccsf("%02d", now_tm->tm_mday)).c_str());
	int now_time_hms = atoi((string("") + ccsf("%02d", now_tm->tm_hour) + ccsf("%02d", now_tm->tm_min) + ccsf("%02d", now_tm->tm_sec)).c_str());
	
	if(atoi(startDate.substr(0,8).c_str()) <= now_time_number &&
	   atoi(endDate.substr(0,8).c_str()) >= now_time_number &&
	   NSDS_GI(kSDS_GI_shopEventPack_startTime_i) <= now_time_hms &&
	   NSDS_GI(kSDS_GI_shopEventPack_endTime_i) >= now_time_hms)
	{
		is_on_time_eventPack = true;
	}
	
	bool is_useable_eventPack = true;
	if(is_have_eventPack && is_on_time_eventPack)
	{
		if(is_just_one)
		{
			if(!is_buyed_eventPack)
				is_useable_eventPack = true;
			else
				is_useable_eventPack = false;
		}
		else
			is_useable_eventPack = true;
	}
	else
	{
		is_useable_eventPack = false;
	}
	
	if((!is_buyed_startPack && is_on_time_startPack) || is_useable_eventPack) // 스타트팩을 구입한적 없고 스타트팩 시간이 맞는 경우 || 이벤트팩이 존재하고 해당 이벤트팩의 시간이 맞는 경우
	{
		CCSprite* n_eventPack_img = CCSprite::create("shoptap_down.png");
		KSLabelTTF* n_eventPack_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_event), mySGD->getFont().c_str(), 12.5f);
		n_eventPack_label->disableOuterStroke();
		n_eventPack_label->setPosition(ccpFromSize(n_eventPack_img->getContentSize()/2.f) + ccp(0,2));
		n_eventPack_img->addChild(n_eventPack_label);
		
		CCSprite* s_eventPack_img = CCSprite::create("shoptap_down.png");
		s_eventPack_img->setColor(ccGRAY);
		KSLabelTTF* s_eventPack_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_event), mySGD->getFont().c_str(), 12.5f);
		s_eventPack_label->disableOuterStroke();
		s_eventPack_label->setColor(ccGRAY);
		s_eventPack_label->setPosition(ccpFromSize(s_eventPack_img->getContentSize()/2.f) + ccp(0,2));
		s_eventPack_img->addChild(s_eventPack_label);
		
		CCSprite* d_eventPack_img = CCSprite::create("shoptap_up.png");
		KSLabelTTF* d_eventPack_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_event), mySGD->getFont().c_str(), 12.5f);
		d_eventPack_label->disableOuterStroke();
		d_eventPack_label->setPosition(ccpFromSize(d_eventPack_img->getContentSize()/2.f) + ccp(0,2));
		d_eventPack_img->addChild(d_eventPack_label);
		
		
		eventPack_menu = CCMenuItemSprite::create(n_eventPack_img, s_eventPack_img, d_eventPack_img, this, menu_selector(ShopPopup::menuAction));
		eventPack_menu->setTag(kSP_MT_eventPack);
		eventPack_menu->setPosition(getContentPosition(kSP_MT_eventPack));
		tab_menu->addChild(eventPack_menu, kSP_Z_content);
		
		
		
		if(!is_buyed_startPack && is_on_time_startPack)// 스타트팩을 구입한적 없고 스타트팩 시간이 맞는경우
		{
			time_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 12);
			time_label->setPosition(getContentPosition(kSP_MT_eventPack) + ccp(0,20));
			main_case->addChild(time_label, kSP_Z_content+1);

			CCSprite* time_back = CCSprite::create("mainflow_new3.png");
			time_back->setPosition(ccpFromSize(time_label->getContentSize()/2.f));
			time_label->addChild(time_back, -1);
	
			time_label->addChild(KSSchedule::create([=](float dt)
							   {
								   chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
								   std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
								   
								   time_t sub_time = now_time_t - myDSH->getIntegerForKey(kDSH_Key_savedStartPackFirstTime);
								   
								   int expireSec = NSDS_GI(kSDS_GI_shopStartPack_expireSec_i);
								   
								   if(time_label->isVisible() && sub_time <= expireSec)
									{
										string show_time = "";
										int e_s_sub_time = expireSec - sub_time;
										show_time += ccsf("%02d:", e_s_sub_time/(60*60));
										e_s_sub_time = e_s_sub_time%(60*60);
										show_time += ccsf("%02d:", e_s_sub_time/60);
										e_s_sub_time = e_s_sub_time%60;
										show_time += ccsf("%02d", e_s_sub_time);
										
										time_label->setString(show_time.c_str());
										time_back->setPosition(ccpFromSize(time_label->getContentSize()/2.f));
										return true;
									}
								   else
									{
										time_label->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCHide::create()));
										eventPack_menu->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCHide::create()));
										return false;
									}
								   
//								   tm* sub_time_tm = gmtime(&sub_time);
//								   
//								   int	tm_sec;		/* seconds after the minute [0-60] */
//								   int	tm_min;		/* minutes after the hour [0-59] */
//								   int	tm_hour;	/* hours since midnight [0-23] */
//								   int	tm_mday;	/* day of the month [1-31] */
//								   int	tm_mon;		/* months since January [0-11] */
//								   int	tm_year;	/* years since 1900 */
//								   int	tm_wday;	/* days since Sunday [0-6] */
//								   int	tm_yday;	/* days since January 1 [0-365] */
//								   int	tm_isdst;	/* Daylight Savings Time flag */
//								   long	tm_gmtoff;	/* offset from CUT in seconds */
//								   char	*tm_zone;	/* timezone abbreviation */
							   }));
		}
		else if(is_useable_eventPack)
		{
			addChild(KSSchedule::create([=](float dt)
										{
											bool is_have_eventPack = NSDS_GB(kSDS_GI_shopEventPack_isHave_b);
											bool is_just_one = NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b);
											bool is_buyed_eventPack = NSDS_GI(kSDS_GI_shopEventPack_no_i) == mySGD->getUserdataOnlyOneBuyPack();
											bool is_on_time_eventPack = false;
											
											chrono::time_point<std::chrono::system_clock> now_time = chrono::system_clock::now();
											std::time_t now_time_t = chrono::system_clock::to_time_t(now_time);
											
											tm* now_tm = localtime(&now_time_t);
											string startDate = NSDS_GS(kSDS_GI_shopEventPack_startDate_s);
											string endDate = NSDS_GS(kSDS_GI_shopEventPack_endDate_s);
											
											int now_time_number = atoi((string("") + ccsf("%04d", now_tm->tm_year+1900) + ccsf("%02d", now_tm->tm_mon+1) + ccsf("%02d", now_tm->tm_mday)).c_str());
											int now_time_hms = atoi((string("") + ccsf("%02d", now_tm->tm_hour) + ccsf("%02d", now_tm->tm_min) + ccsf("%02d", now_tm->tm_sec)).c_str());
											
											if(atoi(startDate.substr(0,8).c_str()) <= now_time_number &&
											   atoi(endDate.substr(0,8).c_str()) >= now_time_number &&
											   NSDS_GI(kSDS_GI_shopEventPack_startTime_i) <= now_time_hms &&
											   NSDS_GI(kSDS_GI_shopEventPack_endTime_i) >= now_time_hms)
											{
												is_on_time_eventPack = true;
											}
											
											bool is_useable_eventPack = true;
											if(is_have_eventPack && is_on_time_eventPack)
											{
												if(is_just_one)
												{
													if(!is_buyed_eventPack)
														is_useable_eventPack = true;
													else
														is_useable_eventPack = false;
												}
												else
													is_useable_eventPack = true;
											}
											else
											{
												is_useable_eventPack = false;
											}
											
											if(!is_useable_eventPack)
											{
												eventPack_menu->runAction(CCSequence::createWithTwoActions(CCDelayTime::create(0.1f), CCHide::create()));
											}
											
											return is_useable_eventPack;
										}));
		}
	}
	
	
	
	CommonAnimation::openPopup(this, main_case, nullptr, [=](){
		
	}, [=](){
	});
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
	
//	for(int i=0;i<6;i++)
//	{
//		index_to_ruby.push_back(KSProtectVar<int>(NSDS_GI(kSDS_GI_shopRuby_int1_price_i, i)));
//		index_to_gold.push_back(KSProtectVar<int>(NSDS_GI(kSDS_GI_shopGold_int1_price_i, i)));
//		index_to_heart.push_back(KSProtectVar<int>(NSDS_GI(kSDS_GI_shopCoin_int1_price_i, i)));
//		
//		cash_to_ruby[index_to_ruby[i].getV()] = KSProtectVar<int>(NSDS_GI(kSDS_GI_shopRuby_int1_count_i, i));
//		ruby_to_gold[index_to_gold[i].getV()] = KSProtectVar<int>(NSDS_GI(kSDS_GI_shopGold_int1_count_i, i));
//		ruby_to_heart[index_to_heart[i].getV()] = KSProtectVar<int>(NSDS_GI(kSDS_GI_shopCoin_int1_count_i, i));
//	}
	
	card_price_high = KSProtectVar<int>(100);
	card_price_mid = KSProtectVar<int>(10000);
	card_price_low = KSProtectVar<int>(10);
	
	showPopup();
	
	int i = kAchievementCode_hidden_shopper1;
	int after_value = myAchieve->getRecentValue(kAchievementCode_hidden_shopper1) + 1;
	
	if(!myAchieve->isCompleted(AchievementCode(i)) && !myAchieve->isAchieve(AchievementCode(i)))
	{
		myAchieve->changeIngCount(AchievementCode(i), after_value);
		if(!myAchieve->isNoti(AchievementCode(i)) && after_value >= myAchieve->getCondition(kAchievementCode_hidden_shopper1))
		{
			myAchieve->changeIngCount((AchievementCode)i, myAchieve->getCondition((AchievementCode)i));
			AchieveNoti* t_noti = AchieveNoti::create(AchievementCode(i));
			CCDirector::sharedDirector()->getRunningScene()->addChild(t_noti);
		}
		myAchieve->updateAchieve(nullptr);
	}
	
    return true;
}

void ShopPopup::addGray()
{
	is_add_gray = true;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kSP_Z_gray);
	
	addChild(KSGradualValue<float>::create(0.f, 1.f, 0.25f, [=](float t)
										   {
											   gray->setOpacity(255*t);
										   }, [=](float t)
										   {
											   gray->setOpacity(255*t);
										   }));
}

void ShopPopup::showPopup()
{
	setTouchEnabled(true);
	CommonAnimation::openPopup(this, main_case, nullptr, [=](){
		
	}, bind(&ShopPopup::endShowPopup, this));
}

void ShopPopup::endShowPopup()
{
	if(!mySGD->getUserdataIsFirstBuy() && mySGD->isPossibleShowPurchasePopup(kPurchaseGuideType_firstPurchase) && mySGD->getUserdataTotalPlayCount() >= mySGD->getFirstPurchasePlayCount() && !mySGD->getUserdataIsVIP())
	{
		FirstPurchasePopup* t_popup = FirstPurchasePopup::create(touch_priority-100, [=](){is_menu_enable = true;}, [=](){is_menu_enable = true;});
		addChild(t_popup, kSP_Z_popup);
		return;
	}
	else
	{
		is_menu_enable = true;
	}
}

void ShopPopup::hidePopup()
{
	is_menu_enable = false;
	
	if(is_add_gray)
	{
		addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
											{
												gray->setOpacity(255*t);
											}, [=](float t)
											{
												gray->setOpacity(255*t);
												gray->removeFromParent();
											}));
	}
	
	CommonAnimation::closePopup(this, main_case, nullptr, [=](){
		
	}, bind(&ShopPopup::endHidePopup, this));
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
		return_value = ccp(59,256.5f);
	else if(t_tag == kSP_MT_card)
		return_value = ccp(141.5f,256.5f);
	else if(t_tag == kSP_MT_ruby)
		return_value = ccp(268,256.5f);//ccp(59,256);//ccp(224,256);
	else if(t_tag == kSP_MT_gold)
		return_value = ccp(333,256.5f);//ccp(141.5f,256);//ccp(306,256);
	else if(t_tag == kSP_MT_heart)
		return_value = ccp(398,256.5f);//ccp(224,256);//ccp(388.5f,256);
	else if(t_tag == kSP_MT_p1)
		return_value = ccp(203,256.5f);
	else if(t_tag == kSP_MT_eventPack)
		return_value = ccp(138,256.5f);
	else if(t_tag == kSP_MT_content1)
		return_value = ccp(100,177);
	else if(t_tag == kSP_MT_content2)
		return_value = ccp(240,177);
	else if(t_tag == kSP_MT_content3)
		return_value = ccp(380,177);
	else if(t_tag == kSP_MT_content4)
		return_value = ccp(100,74);
	else if(t_tag == kSP_MT_content5)
		return_value = ccp(240,74);
	else if(t_tag == kSP_MT_content6)
		return_value = ccp(380,74);
	
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
		if(success_func != nullptr)
		{
			success_func();
			success_func = nullptr;
		}
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_successPurchase)), 9999);
		CCLOG("userdata was save to server");
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		CCLOG("fail!! not enought property");
		fail_func();
        if(!is_use_goods_type_gold)
            addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought)), 9999);
        else
            addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_goldNotEnought)), 9999);
	}
	else
	{
		CCLOG("fail!! save userdata to server");
		fail_func();
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
	}
    
    is_use_goods_type_gold = false;
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
//		myDSH->saveUserData({kSaveUserData_Key_character}, nullptr);
		hidePopup();
	}
	else if(tag == kSP_MT_ruby)
	{
//		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_afterOpenCBT), [=](){is_menu_enable = true;}), 9999);
		
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
	else if(tag == kSP_MT_p1)
	{
		setShopCode(kSC_p1);
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_eventPack)
	{
		setShopCode(kSC_eventPack);
		is_menu_enable = true;
	}
	else if(tag == kSP_MT_gacha)
	{
		target_final = NULL;
		delegate_final = NULL;
		hidePopup();
		
		((MainFlowScene*)getParent())->showGachaPopup();
	}
	else if(tag == kSP_MT_cardGacha)
	{
		card_loading = LoadingLayer::create(-9999);
		addChild(card_loading, 9999);
		card_loading->startLoading();
		
		myHSP->command("getcardgachalist", Json::Value(), [=](Json::Value result_data)
					   {
						   if(result_data["result"]["code"].asInt() == GDSUCCESS)
						   {
							   mySGD->card_gacha_no = result_data["no"].asInt();
							   mySGD->card_gacha_msg = result_data["msg"].asString();
							   mySGD->card_gacha_list.clear();
							   card_download_list.clear();
							   
							   Json::Value cards = result_data["cardlist"];
							   for(int i=0;i<cards.size();i++)
							   {
								   Json::Value t_card = cards[i];
								   KSProtectVar<int> t_gacha_card_no;
								   t_gacha_card_no = t_card["no"].asInt();
								   mySGD->card_gacha_list.push_back(t_gacha_card_no);
								   if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
									   continue;
								   NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
								   NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
								   
								   NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
								   
								   NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
								   NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
								   
								   Json::Value t_imgInfo = t_card["imgInfo"];
								   
								   //			bool is_add_cf = false;
								   
								   if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
								   {
									   // check, after download ----------
									   DownloadFile t_df;
									   t_df.size = t_imgInfo["size"].asInt();
									   t_df.img = t_imgInfo["img"].asString().c_str();
									   t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
									   t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
									   
									   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
									   if(iter == card_download_list.end())
									   {
										   card_download_list.push_back(t_df);
										   // ================================
									   }
								   }
								   
								   Json::Value t_aniInfo = t_card["aniInfo"];
								   NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
								   if(t_aniInfo["isAni"].asBool())
								   {
									   Json::Value t_detail = t_aniInfo["detail"];
									   NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
									   
									   Json::Value t_loopSeq = t_detail["loopSeq"];
									   for(int j=0;j<t_loopSeq.size();j++)
										   NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
									   
									   NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
									   NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
									   NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
									   NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
									   NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
									   
									   if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
									   {
										   // check, after download ----------
										   DownloadFile t_df;
										   t_df.size = t_detail["size"].asInt();
										   t_df.img = t_detail["img"].asString().c_str();
										   t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
										   t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
										   
										   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
										   if(iter == card_download_list.end())
											   card_download_list.push_back(t_df);
										   // ================================
									   }
								   }
								   
								   NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
								   NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
								   NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
								   NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
								   NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
								   
								   NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
								   NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
								   NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
								   
								   int sound_cnt = t_card["sound"].size();
								   NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
								   for(int j=1;j<=sound_cnt;j++)
								   {
									   NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
								   }
								   
								   NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
								   
								   Json::Value t_silImgInfo = t_card["silImgInfo"];
								   NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
								   if(t_silImgInfo["isSil"].asBool())
								   {
									   if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
									   {
										   // check, after download ----------
										   DownloadFile t_df;
										   t_df.size = t_silImgInfo["size"].asInt();
										   t_df.img = t_silImgInfo["img"].asString().c_str();
										   t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
										   t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
										   
										   auto iter = find(card_download_list.begin(), card_download_list.end(), t_df);
										   if(iter == card_download_list.end())
											   card_download_list.push_back(t_df);
										   // ================================
									   }
								   }
								   
								   Json::Value t_faceInfo = t_card["faceInfo"];
								   if(!t_faceInfo.isNull() && t_faceInfo.asString() != "")
								   {
									   NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
									   NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
									   {
										   DownloadFile t_df1;
										   t_df1.size = t_faceInfo["size"].asInt();
										   t_df1.img = t_faceInfo["ccbi"].asString().c_str();
										   t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
										   t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df1);
									   }
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
									   {
										   DownloadFile t_df2;
										   t_df2.size = t_faceInfo["size"].asInt();
										   t_df2.img = t_faceInfo["plist"].asString().c_str();
										   t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
										   t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df2);
									   }
									   
									   if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
									   {
										   DownloadFile t_df3;
										   t_df3.size = t_faceInfo["size"].asInt();
										   t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
										   t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
										   t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
										   card_download_list.push_back(t_df3);
									   }
								   }
								   else
								   {
									   NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
								   }
								   mySDS->fFlush(t_card["piece"].asInt(), kSDS_SI_base);
							   }
							   
							   mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
							   
							   if(card_download_list.size() > 0)
							   {
								   card_progress_timer = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
								   card_progress_timer->setPosition(ccp(240,100));
								   card_loading->addChild(card_progress_timer);
								   
								   card_progress_timer->setCover("loading_progress_front1.png", "loading_progress_mask.png");
								   card_progress_timer->setBack("loading_progress_back.png");
								   
								   ing_download_cnt = 1;
								   success_download_cnt = 0;
								   
								   download_set.clear();
								   is_enable_index.clear();
								   for(int i=0;i<5;i++)
								   {
									   is_enable_index.push_back(i);
								   }
								   
								   startFileDownloadSet();
							   }
							   else
							   {
								   card_loading->removeFromParent();
								   
								   CardGachaPopup* t_popup = CardGachaPopup::create(-200);
								   t_popup->setHideFinalAction(target_final, delegate_final);
								   getParent()->addChild(t_popup, getZOrder());
								   
								   target_final = NULL;
								   hidePopup();
							   }
						   }
						   else
						   {
							   is_menu_enable = true;
							   card_loading->removeFromParent();
							   addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_endlessServerError)), 999);
						   }
					   });
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
//			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_afterOpenCBT), [=](){is_menu_enable = true;}), 9999);;
			
			
			createCheckBuyPopup([=]()
								{
									loading_layer = LoadingLayer::create();
									addChild(loading_layer, kSP_Z_popup);
									
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//									mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopRuby_int1_exchangeID_s, tag-kSP_MT_content1), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
//									
//									mySGD->changeGoods([=](Json::Value result_data){
//										loading_layer->removeFromParent();
//										
//										if(result_data["result"]["code"].asInt() == GDSUCCESS)
//										{
//											if(is_continue)
//											{
//												is_menu_enable = false;
//												
//												if(is_add_gray)
//												{
//													addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
//																						   {
//																							   gray->setOpacity(255*t);
//																						   }, [=](float t)
//																						   {
//																							   gray->setOpacity(255*t);
//																							   gray->removeFromParent();
//																						   }));
//												}
//												
//												CommonAnimation::closePopup(this, main_case, nullptr, [=](){
//													
//												}, [=]()
//																			{
//																				if(target_final)
//																					(target_final->*delegate_final)();
//																				if(is_set_close_func)
//																					close_func();
//																				if(continue_end != nullptr)
//																					continue_end();
//																				removeFromParent();
//																			});
//											}
//											else
//											{
//												is_menu_enable = true;
//											}
//										}
//										else
//										{
//											is_menu_enable = true;
//											mySGD->clearChangeGoods();
//											addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//										}
//									});
//									
//									
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
									Json::Value param;
									param["productid"] = mySGD->getInappProduct(tag-kSP_MT_content1);
									hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
										KS::KSLog("in-app test \n%", v);
										if(v["issuccess"].asInt())
										{
                                            Json::Value t_info = mySGD->getProductInfo(mySGD->getInappProduct(tag-kSP_MT_content1));
                                            if(!t_info.empty())
											{
                                                myHSP->getAdXConnectEventInstance("Sale", t_info["price"].asString().c_str(), t_info["currency"].asString().c_str());
												fiverocks::FiveRocksBridge::trackPurchase(ccsf("ShopPurchaseGemCode%d", tag-kSP_MT_content1+1), t_info["currency"].asString().c_str(), t_info["price"].asDouble(), "");
												myHSP->IgawAdbrixBuy(ccsf("ShopPurchaseGemCode%d", tag-kSP_MT_content1+1));
											}
											requestItemDelivery();
										}
										else
										{
											loading_layer->removeFromParent();
											
//											addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
											
											is_menu_enable = true;
										}
									});
//#endif
								});
		}
		else if(recent_shop_code == kSC_gold)
		{
//			string price_type = NSDS_GS(kSDS_GI_shopGold_int1_priceType_s, tag-kSP_MT_content1);
			
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_gold[tag-kSP_MT_content1].getV())
//			{
				createCheckBuyPopup([=]()
									{
										loading_layer = LoadingLayer::create();
										addChild(loading_layer, kSP_Z_popup);
										
										mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopGold_int1_exchangeID_s, tag-kSP_MT_content1), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
										
										fail_func = [=]()
										{
											mySGD->clearChangeGoods();
										};
										
										success_func = [=]()
										{
											fiverocks::FiveRocksBridge::trackEvent("GetGold", "Get_Purchase", ccsf("ShopPurchase%d", tag-kSP_MT_content1+1), ccsf("Puzzle %03d", myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)));
										};
										
										mySGD->changeGoods(json_selector(this, ShopPopup::resultSetUserData));
									});
//			}
//			else
//			{
//				addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-10, kGoodsType_ruby, [=]()
//																	{
//																		setShopCode(kSC_ruby);
//																	}), kSP_Z_popup);
//				is_menu_enable = true;
//				CCLOG("not enough ruby!!!");
//			}
		}
		else if(recent_shop_code == kSC_heart)
		{
//			string price_type = NSDS_GS(kSDS_GI_shopGold_int1_priceType_s, tag-kSP_MT_content1);
			
//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_heart[tag-kSP_MT_content1].getV())
//			{
				createCheckBuyPopup([=]()
									{
										loading_layer = LoadingLayer::create();
										addChild(loading_layer, kSP_Z_popup);
										
										mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopCoin_int1_exchangeID_s, tag-kSP_MT_content1), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
										
//										myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) + ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
										
										if(target_heartTime)
										{
											target_heartTime->refreshHeartTime();
//											CCNode* target_parent = target_heartTime->getParent();
//											CCPoint heart_time_position = target_heartTime->getPosition();
//											int heart_time_tag = target_heartTime->getTag();
//											
//											target_heartTime->removeFromParent();
//											target_heartTime = HeartTime::create();
//											target_heartTime->setPosition(heart_time_position);
//											target_parent->addChild(target_heartTime, 0, heart_time_tag);
//											if(before_code == kShopBeforeCode_stagesetting)
//												((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
//											else if(before_code == kShopBeforeCode_mainflow)
//												((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//											else if(before_code == kShopBeforeCode_puzzle)
//												((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//											else if(before_code == kShopBeforeCode_startsetting)
//												((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
										}
										
										fail_func = [=]()
										{
											mySGD->clearChangeGoods();
//											myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt) - ruby_to_heart[index_to_heart[tag-kSP_MT_content1].getV()].getV());
											
											if(target_heartTime)
											{
												target_heartTime->refreshHeartTime();
//												CCNode* target_parent = target_heartTime->getParent();
//												CCPoint heart_time_position = target_heartTime->getPosition();
//												int heart_time_tag = target_heartTime->getTag();
//												
//												target_heartTime->removeFromParent();
//												target_heartTime = HeartTime::create();
//												target_heartTime->setPosition(heart_time_position);
//												target_parent->addChild(target_heartTime, 0, heart_time_tag);
//												if(before_code == kShopBeforeCode_stagesetting)
//													((StageSettingPopup*)(target_parent->getParent()))->heart_time = target_heartTime;
//												else if(before_code == kShopBeforeCode_mainflow)
//													((MainFlowScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//												else if(before_code == kShopBeforeCode_puzzle)
//													((PuzzleScene*)(target_parent->getParent()))->heart_time = target_heartTime;
//												else if(before_code == kShopBeforeCode_startsetting)
//													((StartSettingScene*)(target_parent->getParent()))->heart_time = target_heartTime;
											}
										};
										
										success_func = [=]()
										{
											fiverocks::FiveRocksBridge::trackEvent("UseGem", "Get_Heart", ccsf("ShopPurchase%d", tag-kSP_MT_content1+1), ccsf("Puzzle %03d", myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)));
										};
										
										mySGD->changeGoods(json_selector(this, ShopPopup::resultSetUserData));
									});
//			}
//			else
//			{
//				addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-10, kGoodsType_ruby, [=]()
//																	{
//																		setShopCode(kSC_ruby);
//																	}), kSP_Z_popup);
//				is_menu_enable = true;
//				CCLOG("not enough ruby!!!");
//			}
		}
		else if(recent_shop_code == kSC_p1)
		{
			//			string price_type = NSDS_GS(kSDS_GI_shopGold_int1_priceType_s, tag-kSP_MT_content1);
			
			//			if(mySGD->getGoodsValue(kGoodsType_ruby) >= index_to_gold[tag-kSP_MT_content1].getV())
			//			{
			createCheckBuyPopup([=]()
								{
									loading_layer = LoadingLayer::create();
									addChild(loading_layer, kSP_Z_popup);
									
                                    is_use_goods_type_gold = true;
                                    
									mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopP1_int1_exchangeID_s, tag-kSP_MT_content1), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
									
									fail_func = [=]()
									{
										mySGD->clearChangeGoods();
									};
									
									success_func = [=]()
									{
										fiverocks::FiveRocksBridge::trackEvent("GetP1", "Get_Purchase", ccsf("ShopPurchase%d", tag-kSP_MT_content1+1), ccsf("Puzzle %03d", myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)));
									};
									
									mySGD->changeGoods(json_selector(this, ShopPopup::resultSetUserData));
								});
			//			}
			//			else
			//			{
			//				addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-10, kGoodsType_ruby, [=]()
			//																	{
			//																		setShopCode(kSC_ruby);
			//																	}), kSP_Z_popup);
			//				is_menu_enable = true;
			//				CCLOG("not enough ruby!!!");
			//			}
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
//				addChild(ASPopupView::getCommonNoti(-310, "젬이 부족합니다.", [=](){is_menu_enable = true;}), kSP_Z_popup);
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
											int slot_count = 2;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
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
			addChild(ASPopupView::getCommonNoti(touch_priority-10, "젬이 부족합니다.", [=](){is_menu_enable = true;}, CCPointZero), kSP_Z_popup);
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
											int slot_count = 2;//NSDS_GI(kSDS_GI_characterInfo_int1_statInfo_slotCnt_i, i);
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
			addChild(ASPopupView::getCommonNoti(touch_priority-10, "골드가 부족합니다.", [=](){is_menu_enable = true;}, CCPointZero), kSP_Z_popup);
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

void ShopPopup::startFileDownloadSet()
{
	int max_thread_cnt = 5;
	int total_download_cnt = card_download_list.size();
	int before_download_size = download_set.size();
	
	for(int i=download_set.size();ing_download_cnt <= total_download_cnt && i<max_thread_cnt;i++)
	{
		DownloadImgInfo t_info;
		if(card_download_list.size() > 0 && ing_download_cnt <= card_download_list.size())
		{
			t_info.download_url = card_download_list[ing_download_cnt-1].img;
			t_info.download_size = card_download_list[ing_download_cnt-1].size;
			t_info.download_filename = card_download_list[ing_download_cnt-1].filename;
		}
		else
		{
			break;
		}
		
		t_info.is_fail = false;
		t_info.is_end = false;
		download_set.push_back(t_info);
		
		ing_download_cnt++;
	}
	
	if(download_set.size() > 0)
	{
		for(int i=before_download_size;i<download_set.size();i++)
		{
			int will_download_index = is_enable_index.back();
			CCLOG("start download idx : %d / filename : %s", will_download_index, download_set[i].download_filename.c_str());
			mySIL->downloadImg(download_set[i], will_download_index);
			is_enable_index.pop_back();
		}
		if(!is_downloading)
		{
			is_downloading = true;
			schedule(schedule_selector(ShopPopup::checkDownloading));
		}
	}
	else
	{
		unschedule(schedule_selector(ShopPopup::checkDownloading));
		
		
		for(int i=0;i<card_download_list.size();i++)
		{
			SDS_SS(kSDF_cardInfo, card_download_list[i].key,
				   card_download_list[i].img, false);
		}
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		card_loading->removeFromParent();
		
		CardGachaPopup* t_popup = CardGachaPopup::create(-200);
		t_popup->setHideFinalAction(target_final, delegate_final);
		getParent()->addChild(t_popup, getZOrder());
		
		target_final = NULL;
		hidePopup();
	}
}

void ShopPopup::checkDownloading()
{
	int max_thread_cnt = 5;
	bool is_successed = false;
	for(int i=0;i<max_thread_cnt;i++)
	{
		if(mySIL->downloading_list[i].is_end)
		{
			if(mySIL->downloading_list[i].is_fail)
			{
				CCLOG("fail download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					CCLOG("start download idx : %d / filename : %s", i, iter->download_filename.c_str());
					mySIL->downloadImg((*iter), i);
				}
				else
				{
					CCLOG("not found fail set");
				}
			}
			else
			{
				CCLOG("success download idx : %d / filename : %s", i, mySIL->downloading_list[i].download_filename.c_str());
				auto iter = find(download_set.begin(), download_set.end(), DownloadImgInfo(mySIL->downloading_list[i].download_filename));
				if(iter != download_set.end())
				{
					download_set.erase(iter);
				}
				else
				{
					CCLOG("not found success set");
				}
				is_enable_index.push_back(i);
				success_download_cnt++;
				is_successed = true;
			}
		}
	}
	
	float download_percent = 100.f*success_download_cnt/int(card_download_list.size());
	
	card_progress_timer->setPercentage(download_percent);
	
	if(is_successed || success_download_cnt >= card_download_list.size())
	{
		startFileDownloadSet();
	}
}

void ShopPopup::createCheckBuyPopup(function<void()> buy_action)
{
//	ASPopupView* t_popup = ASPopupView::create(touch_priority-5);
//	
//	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//	if(screen_scale_x < 1.f)
//		screen_scale_x = 1.f;
//	
//	float height_value = 320.f;
//	if(myDSH->screen_convert_rate < 1.f)
//		height_value = 320.f/myDSH->screen_convert_rate;
//	
//	if(height_value < myDSH->ui_top)
//		height_value = myDSH->ui_top;
//	
//	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
//	t_popup->setDimmedPosition(ccp(240, 160));
//	t_popup->setBasePosition(ccp(240, 160));
//	
//	CCNode* t_container = CCNode::create();
//	t_popup->setContainerNode(t_container);
//	addChild(t_popup, kSP_Z_popup);
//	
//	CCSprite* back_case = CCSprite::create("popup_small_back.png");
//	back_case->setPosition(ccp(0,0));
//	t_container->addChild(back_case);
//	
//	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
//	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-45, back_case->getContentSize().height-140));
//	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f+16.5f));
//	back_case->addChild(back_in);
//	
//	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_checkBuy), mySGD->getFont().c_str(), 12);
//	title_label->disableOuterStroke();
//	title_label->setAnchorPoint(ccp(0.5f,0.5f));
//	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
//	t_container->addChild(title_label);
//	
//	KSLabelTTF* sub_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_realBuy), mySGD->getFont().c_str(), 12);
//	sub_label->disableOuterStroke();
//	sub_label->setAnchorPoint(ccp(0.5f,0.5f));
//	sub_label->setPosition(ccp(0,16));
//	t_container->addChild(sub_label);
//	
//	CCSprite* gray = t_popup->getDimmedSprite();
//	
//	CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
//	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-22,back_case->getContentSize().height/2.f-25));
//	close_button->setFunction([=](CCObject* sender)
//							  {
//								  if(!t_popup->is_menu_enable)
//									  return;
//								  
//								  t_popup->is_menu_enable = false;
//								  
//									CommonAnimation::closePopup(t_popup, t_container, gray, [=](){
//										
//									}, [=](){
//										is_menu_enable = true;
//										t_popup->removeFromParent();
//									});
//							  });
//	t_container->addChild(close_button);
//	
//	t_popup->button_func_list.clear();
//	
//	t_popup->button_func_list.push_back([=](){
//		if(!t_popup->is_menu_enable)
//			return;
//		
//		t_popup->is_menu_enable = false;
//		
//		CommonAnimation::closePopup(this, t_container, gray, [=](){
//			
//		}, [=](){
//			buy_action();
//			t_popup->removeFromParent();
//		});
//		
//	});
//	
//	CCLabelTTF* t2_label = CCLabelTTF::create();
//	
//	KSLabelTTF* ok_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
//	ok_label->disableOuterStroke();
//	ok_label->setPosition(ccp(0,0));
//	t2_label->addChild(ok_label);
//	
//	CCScale9Sprite* ok_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
//	
//	CCControlButton* ok_button = CCControlButton::create(t2_label, ok_back);
//	ok_button->addTargetWithActionForControlEvents(t_popup, cccontrol_selector(ASPopupView::buttonAction), CCControlEventTouchUpInside);
//	ok_button->setTag(0);
//	ok_button->setPreferredSize(CCSizeMake(150,50));
//	ok_button->setPosition(ccp(0,-42));
//	t_container->addChild(ok_button);
//	
//	ok_button->setTouchPriority(t_popup->getTouchPriority()-5);
//	
//	
//	CommonAnimation::openPopup(this, t_container, gray, [=](){
//		
//	}, [=](){
//		t_popup->is_menu_enable = true;
//	});



	
	addChild(ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_realBuy),[=](){
		
		buy_action();

	},12.f,CCPointZero,true,true,[=](){

		is_menu_enable = true;

	}));
	
	
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
//		NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
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
		replay_menu = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, touch_priority-50);
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
//		for(int i=0;i<cf_list.size();i++)
//		{
//			mySIL->removeTextureCache(cf_list[i].from_filename);
//			mySIL->removeTextureCache(cf_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
////			if(cf_list[i].is_ani)
////			{
////				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
////				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
////				target_img->addChild(ani_img);
////			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->begin();
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(cf_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", cf_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
		
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
//				myDSH->saveUserData({kSaveUserData_Key_cardsInfo}, nullptr);
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
	
	replay_menu = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_replay), 12, CCSizeMake(80,45), CommonButtonYellow, touch_priority-50);
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
            
            mySGD->network_check_cnt = 0;
			
			mySGD->initProperties(t["list"]);
//			mySGD->refreshGoodsData(t["list"]["type"].asString(), t["list"]["count"].asInt());
			
			loading_layer->removeFromParent();
			if(is_continue)
			{
				is_menu_enable = false;
				
				if(is_add_gray)
				{
					addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
														   {
															   gray->setOpacity(255*t);
														   }, [=](float t)
														   {
															   gray->setOpacity(255*t);
															   gray->removeFromParent();
														   }));
				}
				
				CommonAnimation::closePopup(this, main_case, nullptr, [=](){
					
				}, [=]()
											{
												if(target_final)
													(target_final->*delegate_final)();
												if(is_set_close_func)
													close_func();
												if(continue_end != nullptr)
													continue_end();
												removeFromParent();
											});
			}
			else
			{
				is_menu_enable = true;
			}
		}
		else
		{
            mySGD->network_check_cnt++;
            
            if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
            {
                mySGD->network_check_cnt = 0;
                
                ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
                    requestItemDelivery();
                }, 1);
                if(alert)
                    ((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
            }
            else
            {
                addChild(KSTimer::create(0.5f, [=]()
                                         {
                                             requestItemDelivery();
                                         }));
            }
		}
	});
}

void ShopPopup::requestItemDeliveryStartPack()
{
	mySGD->setUserdataOnlyOneBuyPack(NSDS_GI(kSDS_GI_shopStartPack_no_i));
	
	vector<CommandParam> t_command_list;
	t_command_list.clear();
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getSocialID();
	
	t_command_list.push_back(CommandParam("starttransaction", transaction_param, nullptr));
	
	t_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getMemberID();
	
	t_command_list.push_back(CommandParam("requestItemDelivery", param, [=](Json::Value t){
		if(t["result"]["code"].asInt() == GDSUCCESS)
		{
            CCLOG("inapp success!! refresh!!! 7-> %s",t.toStyledString().c_str());
			TRACE();
            
			
            mySGD->network_check_cnt = 0;
            
			mySGD->initProperties(t["list"]);
			//			mySGD->refreshGoodsData(t["list"]["type"].asString(), t["list"]["count"].asInt());
			
			loading_layer->removeFromParent();
			if(is_continue)
			{
				is_menu_enable = false;
				
				if(is_add_gray)
				{
					addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
														   {
															   gray->setOpacity(255*t);
														   }, [=](float t)
														   {
															   gray->setOpacity(255*t);
															   gray->removeFromParent();
														   }));
				}
				
				CommonAnimation::closePopup(this, main_case, nullptr, [=](){
					
				}, [=]()
											{
												if(target_final)
													(target_final->*delegate_final)();
												if(is_set_close_func)
													close_func();
												if(continue_end != nullptr)
													continue_end();
												removeFromParent();
											});
			}
			else
			{
				setShopCode(kSC_ruby);
				is_menu_enable = true;
			}
			if(time_label)
				time_label->setVisible(false);
			eventPack_menu->setVisible(false);
		}
		else
		{
            mySGD->network_check_cnt++;
            
            if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
            {
                mySGD->network_check_cnt = 0;
                
                ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
                    requestItemDeliveryStartPack();
                }, 1);
                if(alert)
                    ((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
            }
            else
            {
                addChild(KSTimer::create(0.5f, [=]()
                                         {
                                             requestItemDeliveryStartPack();
                                         }));
            }
		}
	}));
//	GraphDog::get()->command("requestItemDelivery", param, [=](Json::Value t){
//		if(t["result"]["code"].asInt() == GDSUCCESS)
//		{
//			CCLOG("inapp success!! refresh!!!");
//			
//			mySGD->initProperties(t["list"]);
//			//			mySGD->refreshGoodsData(t["list"]["type"].asString(), t["list"]["count"].asInt());
//			
//			loading_layer->removeFromParent();
//			if(is_continue)
//			{
//				is_menu_enable = false;
//				
//				if(is_add_gray)
//				{
//					addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
//														   {
//															   gray->setOpacity(255*t);
//														   }, [=](float t)
//														   {
//															   gray->setOpacity(255*t);
//															   gray->removeFromParent();
//														   }));
//				}
//				
//				CommonAnimation::closePopup(this, main_case, nullptr, [=](){
//					
//				}, [=]()
//											{
//												if(target_final)
//													(target_final->*delegate_final)();
//												if(is_set_close_func)
//													close_func();
//												if(continue_end != nullptr)
//													continue_end();
//												removeFromParent();
//											});
//			}
//			else
//			{
//				setShopCode(kSC_ruby);
//				is_menu_enable = true;
//			}
//		}
//		else if(t["result"]["code"].asInt() == 2016) // GDNOTINGWORK
//		{
//			addChild(KSTimer::create(3.f, [=](){requestItemDeliveryStartPack();}));
//		}
//		else
//		{
//			addChild(KSTimer::create(3.f, [=](){requestItemDeliveryStartPack();}));
//		}
//	});
	
	myHSP->command(t_command_list);
}

void ShopPopup::requestItemDeliveryEventPack()
{
	mySGD->setUserdataOnlyOneBuyPack(NSDS_GI(kSDS_GI_shopEventPack_no_i));
	
	vector<CommandParam> t_command_list;
	t_command_list.clear();
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getSocialID();
	
	t_command_list.push_back(CommandParam("starttransaction", transaction_param, nullptr));
	
	t_command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	Json::Value param;
	param["memberID"] = hspConnector::get()->getMemberID();
	
	t_command_list.push_back(CommandParam("requestItemDelivery", param, [=](Json::Value t){
		if(t["result"]["code"].asInt() == GDSUCCESS)
		{
			CCLOG("inapp success!! refresh!!!");
			
            mySGD->network_check_cnt = 0;
            
			mySGD->initProperties(t["list"]);
			//			mySGD->refreshGoodsData(t["list"]["type"].asString(), t["list"]["count"].asInt());
			
			loading_layer->removeFromParent();
			if(is_continue)
			{
				is_menu_enable = false;
				
				if(is_add_gray)
				{
					addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
														   {
															   gray->setOpacity(255*t);
														   }, [=](float t)
														   {
															   gray->setOpacity(255*t);
															   gray->removeFromParent();
														   }));
				}
				
				CommonAnimation::closePopup(this, main_case, nullptr, [=](){
					
				}, [=]()
											{
												if(target_final)
													(target_final->*delegate_final)();
												if(is_set_close_func)
													close_func();
												if(continue_end != nullptr)
													continue_end();
												removeFromParent();
											});
			}
			else
			{
				is_menu_enable = true;
			}
			if(NSDS_GB(kSDS_GI_shopEventPack_isJustOne_b))
			{
				if(time_label)
					time_label->setVisible(false);
				eventPack_menu->setVisible(false);
				setShopCode(kSC_ruby);
			}
		}
		else
		{
            mySGD->network_check_cnt++;
            
            if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
            {
                mySGD->network_check_cnt = 0;
                
                ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
                    requestItemDeliveryEventPack();
                }, 1);
                if(alert)
                    ((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
            }
            else
            {
                addChild(KSTimer::create(0.5f, [=]()
                                         {
                                             requestItemDeliveryEventPack();
                                         }));
            }
		}
	}));
	//	GraphDog::get()->command("requestItemDelivery", param, [=](Json::Value t){
	//		if(t["result"]["code"].asInt() == GDSUCCESS)
	//		{
	//			CCLOG("inapp success!! refresh!!!");
	//
	//			mySGD->initProperties(t["list"]);
	//			//			mySGD->refreshGoodsData(t["list"]["type"].asString(), t["list"]["count"].asInt());
	//
	//			loading_layer->removeFromParent();
	//			if(is_continue)
	//			{
	//				is_menu_enable = false;
	//
	//				if(is_add_gray)
	//				{
	//					addChild(KSGradualValue<float>::create(1.f, 0.f, 0.25f, [=](float t)
	//														   {
	//															   gray->setOpacity(255*t);
	//														   }, [=](float t)
	//														   {
	//															   gray->setOpacity(255*t);
	//															   gray->removeFromParent();
	//														   }));
	//				}
	//
	//				CommonAnimation::closePopup(this, main_case, nullptr, [=](){
	//
	//				}, [=]()
	//											{
	//												if(target_final)
	//													(target_final->*delegate_final)();
	//												if(is_set_close_func)
	//													close_func();
	//												if(continue_end != nullptr)
	//													continue_end();
	//												removeFromParent();
	//											});
	//			}
	//			else
	//			{
	//				setShopCode(kSC_ruby);
	//				is_menu_enable = true;
	//			}
	//		}
	//		else if(t["result"]["code"].asInt() == 2016) // GDNOTINGWORK
	//		{
	//			addChild(KSTimer::create(3.f, [=](){requestItemDeliveryStartPack();}));
	//		}
	//		else
	//		{
	//			addChild(KSTimer::create(3.f, [=](){requestItemDeliveryStartPack();}));
	//		}
	//	});
	
	myHSP->command(t_command_list);
}

