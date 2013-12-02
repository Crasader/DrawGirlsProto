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
	kSP_MT_content1,
	kSP_MT_content2,
	kSP_MT_content3,
	kSP_MT_content4,
	kSP_MT_content5,
	kSP_MT_content6
};

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
	
	if(recent_shop_code != kSC_empty)
	{
		main_case->removeChildByTag(kSP_MT_content1);
		main_case->removeChildByTag(kSP_MT_content2);
		main_case->removeChildByTag(kSP_MT_content3);
		main_case->removeChildByTag(kSP_MT_content4);
		main_case->removeChildByTag(kSP_MT_content5);
		main_case->removeChildByTag(kSP_MT_content6);
	}
	
	recent_shop_code = t_code;
	
	string filename;
	if(recent_shop_code == kSC_ruby)
		filename = "shop_ruby%d.png";
	else if(recent_shop_code == kSC_gold)
		filename = "shop_gold%d.png";
	else if(recent_shop_code == kSC_heart)
		filename = "shop_heart%d.png";
	
	for(int i=1;i<=6;i++)
	{
		CCSprite* n_content = CCSprite::create(CCString::createWithFormat(filename.c_str(), i)->getCString());
		CCSprite* s_content = CCSprite::create(CCString::createWithFormat(filename.c_str(), i)->getCString());
		s_content->setColor(ccGRAY);
		
		CCMenuItem* content_item = CCMenuItemSprite::create(n_content, s_content, this, menu_selector(ShopPopup::menuAction));
		content_item->setTag(kSP_MT_content1 + i - 1);
		
		CCMenu* content_menu = CCMenu::createWithItem(content_item);
		content_menu->setPosition(getContentPosition(kSP_MT_content1 + i - 1));
		main_case->addChild(content_menu, kSP_Z_content, kSP_MT_content1 + i - 1);
		
		content_menu->setTouchPriority(-170-1);
	}
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
	
	
	CCSprite* n_close = CCSprite::create("shop_close.png");
	CCSprite* s_close = CCSprite::create("shop_close.png");
	s_close->setColor(ccGRAY);
	
	CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(ShopPopup::menuAction));
	close_item->setTag(kSP_MT_close);
	
	CCMenu* close_menu = CCMenu::createWithItem(close_item);
	close_menu->setPosition(getContentPosition(kSP_MT_close));
	main_case->addChild(close_menu, kSP_Z_content);
	
	close_menu->setTouchPriority(-170-1);
	
	
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
		return_value = ccp(420,28);
	else if(t_tag == kSP_MT_ruby)
		return_value = ccp(62,28);
	else if(t_tag == kSP_MT_gold)
		return_value = ccp(158,28);
	else if(t_tag == kSP_MT_heart)
		return_value = ccp(254,28);
	else if(t_tag == kSP_MT_content1)
		return_value = ccp(93,220);
	else if(t_tag == kSP_MT_content2)
		return_value = ccp(240,220);
	else if(t_tag == kSP_MT_content3)
		return_value = ccp(387,220);
	else if(t_tag == kSP_MT_content4)
		return_value = ccp(93,110);
	else if(t_tag == kSP_MT_content5)
		return_value = ccp(240,110);
	else if(t_tag == kSP_MT_content6)
		return_value = ccp(387,110);
	
	return return_value;
}

void ShopPopup::resultSetUserData(Json::Value result_data)
{
	loading_layer->removeFromParent();
	if(result_data["state"].asString() == "ok")
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
				((StageSettingPopup*)target_parent)->heart_time = target_heartTime;
				
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
				((StageSettingPopup*)target_parent)->heart_time = target_heartTime;
				
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
				((StageSettingPopup*)target_parent)->heart_time = target_heartTime;
				
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
				((StageSettingPopup*)target_parent)->heart_time = target_heartTime;
				
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
				((StageSettingPopup*)target_parent)->heart_time = target_heartTime;
				
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
				((StageSettingPopup*)target_parent)->heart_time = target_heartTime;
				
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