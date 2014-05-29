//
//  ShowItemContent.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 9..
//
//

#include "ShowItemContent.h"

#include "MyLocalization.h"
ShowItemContent* ShowItemContent::create(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list)
{
	ShowItemContent* t_ctc = new ShowItemContent();
	t_ctc->myInit(t_touch_priority, t_selector, t_item_list);
	t_ctc->autorelease();
	return t_ctc;
}

void ShowItemContent::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	ing_close_cnt++;
	
	if(ing_close_cnt >= item_list.size())
	{
		startHideAnimation();
	}
	else
	{
		show_content->removeFromParent();
		show_content = CCSprite::create(CCString::createWithFormat("item%d.png", item_list[ing_close_cnt])->getCString());
		show_content->setPosition(ccp(0, 40));
		addChild(show_content);
		
		item_title->setString(convertToItemCodeToItemName((ITEM_CODE)item_list[ing_close_cnt]).c_str());
		
		item_ment->setString(mySD->getItemScript((ITEM_CODE)item_list[ing_close_cnt]).c_str());
	}
}

void ShowItemContent::startShowAnimation()
{
	setScale(0);
	CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ShowItemContent::endShowAnimation));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	runAction(t_seq);
}

void ShowItemContent::endShowAnimation()
{
	is_menu_enable = true;
}

void ShowItemContent::startHideAnimation()
{
	is_menu_enable = false;
	CCScaleTo* t_scale = CCScaleTo::create(0.2f, 0.f);
	CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ShowItemContent::endHideAnimation));
	CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
	runAction(t_seq);
}
void ShowItemContent::endHideAnimation()
{
	end_selector(NULL);
}

void ShowItemContent::myInit(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_selector = t_selector;
	
	for(int i=0;i<t_item_list.size();i++)
		item_list.push_back(t_item_list[i]);
	
	
//	CCSprite* back_light = CCSprite::create("newitem_back.png");
//	back_light->setScale(2.f);
//	back_light->setPosition(CCPointZero);
//	addChild(back_light);
//	
//	CCRotateBy* t_rotate = CCRotateBy::create(1.f, 100);
//	CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
//	back_light->runAction(t_repeat);
	
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	case_back->setContentSize(CCSizeMake(240, 250));
	case_back->setPosition(ccp(0,0));
	addChild(case_back);
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	content_back->setContentSize(CCSizeMake(case_back->getContentSize().width-20, case_back->getContentSize().height-50));
	content_back->setPosition(ccp(case_back->getContentSize().width/2.f,case_back->getContentSize().height/2.f-15));
	case_back->addChild(content_back);
	
	
	ing_close_cnt = 0;
	
	show_content = CCSprite::create(CCString::createWithFormat("item%d.png", item_list[ing_close_cnt])->getCString());
	show_content->setPosition(ccp(0, 40));
	addChild(show_content);
	
	item_title = KSLabelTTF::create(convertToItemCodeToItemName((ITEM_CODE)item_list[ing_close_cnt]).c_str(), mySGD->getFont().c_str(), 12);
	item_title->setColor(ccc3(255,170,20));
	item_title->enableOuterStroke(ccBLACK, 1);
	item_title->setPosition(ccp(0,-10));
	addChild(item_title);
	
	item_ment = KSLabelTTF::create(mySD->getItemScript((ITEM_CODE)item_list[ing_close_cnt]).c_str(), mySGD->getFont().c_str(), 12);
	item_ment->setColor(ccWHITE);
	item_ment->setPosition(ccp(0,-32));
	addChild(item_ment);
	
	if(item_ment->getContentSize().width > 200)
	{
		case_back->setContentSize(CCSizeMake(item_ment->getContentSize().width+40,case_back->getContentSize().height));
		content_back->setContentSize(CCSizeMake(item_ment->getContentSize().width+20,content_back->getContentSize().height));
	}
	
	
	KSLabelTTF* title_img = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_newItemTitle), mySGD->getFont().c_str(), 15);
	title_img->setColor(ccc3(255,170,20));
	title_img->setAnchorPoint(ccp(0,0.5f));
	title_img->setPosition(ccp(-case_back->getContentSize().width/2.f+17, 3+case_back->getContentSize().height/2.f-25));
	addChild(title_img);
	
	KSLabelTTF* bonus_ment_img = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_newItemMent), mySGD->getFont().c_str(), 12);
	bonus_ment_img->setColor(ccc3(255,170,20));
	bonus_ment_img->setPosition(ccp(0,-55));
	addChild(bonus_ment_img);
	
	//		CCSprite* t_tab = CCSprite::create("shop_tab.png");
	//		t_tab->setPosition(ccp(30,71));
	//		addChild(t_tab, 2);
	//
	//		CCLabelTTF* new_label = CCLabelTTF::create("new", mySGD->getFont().c_str(), 10);
	//		new_label->setPosition(ccp(t_tab->getContentSize().width/2.f, t_tab->getContentSize().height/2.f+5));
	//		t_tab->addChild(new_label);
	
	
	close_menu = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ok), 13, CCSizeMake(90,40), CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16,16,2,2)), touch_priority-1);
	close_menu->setPosition(ccp(0,-85));
	close_menu->setFunction([=](CCObject* sender)
							{
								menuAction(sender);
							});
	addChild(close_menu);
	startShowAnimation();
}

string ShowItemContent::convertToItemCodeToItemName(ITEM_CODE t_code)
{
	string return_value;
	if(t_code == kIC_attack)				return_value = "Attack";
	else if(t_code == kIC_speedUp)			return_value = "SpeedUp";
	else if(t_code == kIC_addTime)			return_value = "AddTime";
	else if(t_code == kIC_fast)				return_value = "Fast";
	else if(t_code == kIC_subOneDie)		return_value = "SubOneDie";
	else if(t_code == kIC_doubleItem)		return_value = myLoc->getLocalForKey(kMyLocalKey_doubleItemTitle);
	else if(t_code == kIC_silence)			return_value = "Silence";
	else if(t_code == kIC_longTime)			return_value = myLoc->getLocalForKey(kMyLocalKey_longTimeTitle);
	else if(t_code == kIC_baseSpeedUp)		return_value = myLoc->getLocalForKey(kMyLocalKey_baseSpeedUpTitle);
	else if(t_code == kIC_itemGacha)		return_value = myLoc->getLocalForKey(kMyLocalKey_itemGachaTitle);
	
	return return_value.c_str();
}

