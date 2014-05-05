//
//  ShowItemContent.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 9..
//
//

#ifndef __DGproto__ShowItemContent__
#define __DGproto__ShowItemContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"
#include "EnumDefine.h"
#include "StarGoldData.h"
#include "KSLabelTTF.h"
#include "SilhouetteData.h"
#include "MyLocalization.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ShowItemContent : public CCNode
{
public:
	static ShowItemContent* create(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list)
	{
		ShowItemContent* t_ctc = new ShowItemContent();
		t_ctc->myInit(t_touch_priority, t_selector, t_item_list);
		t_ctc->autorelease();
		return t_ctc;
	}
	
private:
	int touch_priority;
	CCMenu* close_menu;
	vector<int> item_list;
	function<void(CCObject*)> end_selector;
	CCSprite* show_content;
	KSLabelTTF* item_title;
	KSLabelTTF* item_ment;
	int ing_close_cnt;
	
	bool is_menu_enable;
	
	
	void menuAction(CCObject* sender)
	{
		ing_close_cnt++;
		
		if(ing_close_cnt >= item_list.size())
		{
			startHideAnimation();
		}
		else
		{
			show_content->removeFromParent();
			show_content = CCSprite::create(CCString::createWithFormat("item%d.png", item_list[ing_close_cnt])->getCString());
			show_content->setPosition(ccp(0, 23));
			addChild(show_content);
			
			item_title->setString(convertToItemCodeToItemName((ITEM_CODE)item_list[ing_close_cnt]).c_str());
			
			item_ment->setString(mySD->getItemScript((ITEM_CODE)item_list[ing_close_cnt]).c_str());
		}
	}
	
	void startShowAnimation()
	{
		setScale(0);
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 1.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ShowItemContent::endShowAnimation));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		runAction(t_seq);
	}
	void endShowAnimation()
	{
		is_menu_enable = true;
	}
	
	void startHideAnimation()
	{
		is_menu_enable = false;
		CCScaleTo* t_scale = CCScaleTo::create(0.3f, 0.f);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ShowItemContent::endHideAnimation));
		CCSequence* t_seq = CCSequence::createWithTwoActions(t_scale, t_call);
		runAction(t_seq);
	}
	void endHideAnimation()
	{
		end_selector(NULL);
	}
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector, const vector<int>& t_item_list)
	{
		is_menu_enable = false;
		
		touch_priority = t_touch_priority;
		end_selector = t_selector;
		
		for(int i=0;i<t_item_list.size();i++)
			item_list.push_back(t_item_list[i]);
		
		
		CCSprite* back_light = CCSprite::create("newitem_back.png");
		back_light->setScale(2.f);
		back_light->setPosition(CCPointZero);
		addChild(back_light);
		
		CCRotateBy* t_rotate = CCRotateBy::create(1.f, 100);
		CCRepeatForever* t_repeat = CCRepeatForever::create(t_rotate);
		back_light->runAction(t_repeat);
		
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(ccp(0,3));
		addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(120, 154));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setPosition(ccp(0,25));
		addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(100, 90));
		
		ing_close_cnt = 0;
		
		show_content = CCSprite::create(CCString::createWithFormat("item%d.png", item_list[ing_close_cnt])->getCString());
		show_content->setPosition(ccp(0, 23));
		addChild(show_content);
		
		item_title = KSLabelTTF::create(convertToItemCodeToItemName((ITEM_CODE)item_list[ing_close_cnt]).c_str(), mySGD->getFont().c_str(), 12);
		item_title->setColor(ccc3(255,100,30));
		item_title->enableOuterStroke(ccBLACK, 1);
		item_title->setPosition(ccp(0,-33));
		addChild(item_title);
		
		item_ment = KSLabelTTF::create(mySD->getItemScript((ITEM_CODE)item_list[ing_close_cnt]).c_str(), mySGD->getFont().c_str(), 9);
		item_ment->setColor(ccc3(255,100,30));
		item_ment->setPosition(ccp(0,-55));
		addChild(item_ment);
		
		
		KSLabelTTF* title_img = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_newItemTitle), mySGD->getFont().c_str(), 20);
		title_img->setPosition(ccp(0, 115));
		addChild(title_img);
		
		KSLabelTTF* bonus_ment_img = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_newItemMent), mySGD->getFont().c_str(), 17);
		bonus_ment_img->setPosition(ccp(0,-115));
		addChild(bonus_ment_img);
		
		CCSprite* t_tab = CCSprite::create("shop_tab.png");
		t_tab->setPosition(ccp(30,71));
		addChild(t_tab, 2);
		
		CCLabelTTF* new_label = CCLabelTTF::create("new", mySGD->getFont().c_str(), 10);
		new_label->setPosition(ccp(t_tab->getContentSize().width/2.f, t_tab->getContentSize().height/2.f+5));
		t_tab->addChild(new_label);
		
		
		CCSprite* n_close = CCSprite::create("whitePaper.png");
		n_close->setOpacity(0);
		CCSprite* s_close = CCSprite::create("whitePaper.png");
		s_close->setOpacity(0);
		
		CCMenuItemSprite* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(ShowItemContent::menuAction));
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setTouchPriority(touch_priority-1);
		close_menu->setPosition(ccp(0,0));
		addChild(close_menu);
		
		startShowAnimation();
	}
	
	string convertToItemCodeToItemName(ITEM_CODE t_code)
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
};

#endif /* defined(__DGproto__ShowItemContent__) */
