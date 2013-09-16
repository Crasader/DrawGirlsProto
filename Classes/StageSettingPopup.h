//
//  StageSettingPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 10..
//
//

#ifndef __DGproto__StageSettingPopup__
#define __DGproto__StageSettingPopup__

#include "cocos2d.h"
#include "SilhouetteData.h"
#include "MaingameScene.h"
#include "ItemBuyPopup.h"
#include <deque>

USING_NS_CC;
using namespace std;

enum SSPL_MenuTag{
	kSSPL_MT_start = 1,
	kSSPL_MT_cancel,
	kSSPL_MT_itemBase
};

enum SSPL_Zorder{
	kSSPL_Z_back = 1,
	kSSPL_Z_selectedImg,
	kSSPL_Z_menu,
	kSSPL_Z_itemImg,
	kSSPL_Z_popup
};

enum SSPL_ChildTag{
	kSSPL_CT_selectedBase = 10,
	kSSPL_CT_itemCntBase = 20,
	kSSPL_CT_menuBase = 30
};

class StageSettingPopupLayer : public CCLayer
{
public:
	static StageSettingPopupLayer* create(int stage, CCObject* t_worldmap, SEL_CallFunc d_cancel)
	{
		StageSettingPopupLayer* t_sspl = new StageSettingPopupLayer();
		t_sspl->myInit(stage, t_worldmap, d_cancel);
		t_sspl->autorelease();
		return t_sspl;
	}
	
private:
	bool is_menu_enable;
	
	int stage_number;
	CCObject* target_worldmap;
	SEL_CallFunc delegate_cancel;
	
	int touched_number;
	CCMenu* start_menu;
	CCMenu* cancel_menu;
	
	vector<ITEM_CODE> item_list;
	deque<bool> is_selected_item;
	
	CCLabelTTF* item_script;
	
	void myInit(int stage, CCObject* t_worldmap, SEL_CallFunc d_cancel)
	{
		stage_number = stage;
		target_worldmap = t_worldmap;
		delegate_cancel = d_cancel;
		
		CLEAR_CONDITION clr_condition = mySD->getClearCondition(stage);
		item_list = mySD->getStageItemList(stage);

		CCSprite* sspl_back = CCSprite::create("sspl_back.png");
		sspl_back->setPosition(ccp(240,160));
		addChild(sspl_back, kSSPL_Z_back);
		
		CCLabelTTF* stage_number = CCLabelTTF::create(CCString::createWithFormat("%d", stage)->getCString(), mySGD->getFont().c_str(), 20);
		stage_number->setPosition(ccp(195,272));
		addChild(stage_number, kSSPL_Z_menu);
		
		itemSetting();
		missionSetting(clr_condition);
		
		CCSprite* n_start = CCSprite::create("sspl_start.png");
		CCSprite* s_start = CCSprite::create("sspl_start.png");
		s_start->setColor(ccGRAY);
		
		CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(StageSettingPopupLayer::menuAction));
		start_item->setTag(kSSPL_MT_start);
		
		start_menu = CCMenu::createWithItem(start_item);
		start_menu->setPosition(ccp(337,65));
		addChild(start_menu, kSSPL_Z_menu);
		
		CCSprite* n_cancel = CCSprite::create("sspl_cancel.png");
		CCSprite* s_cancel = CCSprite::create("sspl_cancel.png");
		s_cancel->setColor(ccGRAY);
		
		CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(StageSettingPopupLayer::menuAction));
		cancel_item->setTag(kSSPL_MT_cancel);
		
		cancel_menu = CCMenu::createWithItem(cancel_item);
		cancel_menu->setPosition(ccp(412,267));
		addChild(cancel_menu, kSSPL_Z_menu);
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kSSPL_MT_start)
		{
			myGD->resetGameData();
			mySD->setSilType(stage_number);
			
			deque<bool> is_using_item;
			for(int i=kIC_attack;i<=kIC_randomChange;i++)
				is_using_item.push_back(false);
			
			for(int i=0;i<is_selected_item.size();i++)
			{
				if(is_selected_item[i])
				{
					myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[i])-1);
					is_using_item[item_list[i]] = true;
				}
			}
			
			for(int i=kIC_attack;i<=kIC_randomChange;i++)
				mySGD->setIsUsingItem(ITEM_CODE(i), is_using_item[i]);
			
			CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
			pEGLView->setDesignResolutionSize(480, 320, kResolutionFixedWidth);
			
			mySGD->setGameStart();
			CCDirector::sharedDirector()->replaceScene(Maingame::scene());
		}
		else if(tag == kSSPL_MT_cancel)
		{
			(target_worldmap->*delegate_cancel)();
			removeFromParent();
		}
		else if(tag >= kSSPL_MT_itemBase)
		{
			int clicked_item_number = tag-kSSPL_MT_itemBase;
			
			if(myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[clicked_item_number]) > 0)
			{
				if(is_selected_item[clicked_item_number])
				{
					is_selected_item[clicked_item_number] = false;
					removeChildByTag(kSSPL_CT_selectedBase+clicked_item_number);
				}
				else
				{
					is_selected_item[clicked_item_number] = true;
					CCSprite* selected_img = CCSprite::create("button_item_selected.png");
					selected_img->setPosition(ccp(102+(clicked_item_number%2)*80, 186-(clicked_item_number/2)*65));
					addChild(selected_img, kSSPL_Z_selectedImg, kSSPL_CT_selectedBase+clicked_item_number);
				}
			}
			else
			{
				ItemBuyPopup* t_ibp = ItemBuyPopup::create(item_list[clicked_item_number], clicked_item_number, this, callfuncII_selector(StageSettingPopupLayer::buySuccessItem));
				addChild(t_ibp, kSSPL_Z_popup);
			}
			
			item_script->setString(mySD->getItemScript(item_list[clicked_item_number]).c_str());
			
			is_menu_enable = true;
		}
	}
	
	void buySuccessItem(int t_clicked_item_number, int cnt)
	{
		removeChildByTag(kSSPL_CT_itemCntBase+t_clicked_item_number);
		myDSH->setIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number], myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number])+cnt);
		
		int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, item_list[t_clicked_item_number]);

		CCSprite* cnt_img = CCSprite::create("item_cnt_label.png");
		cnt_img->setPosition(ccpAdd(ccp(102+(t_clicked_item_number%2)*80, 186-(t_clicked_item_number/2)*65), ccp(-25,17)));
		addChild(cnt_img, kSSPL_Z_itemImg, kSSPL_CT_itemCntBase+t_clicked_item_number);
		
		CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 12);
		cnt_label->setPosition(ccp(cnt_img->getContentSize().width/2.f, cnt_img->getContentSize().height/2.f));
		cnt_img->addChild(cnt_label);
		
		is_selected_item[t_clicked_item_number] = true;
		CCSprite* selected_img = CCSprite::create("button_item_selected.png");
		selected_img->setPosition(ccp(102+(t_clicked_item_number%2)*80, 186-(t_clicked_item_number/2)*65));
		addChild(selected_img, kSSPL_Z_selectedImg, kSSPL_CT_selectedBase+t_clicked_item_number);
	}
	
	void itemSetting()
	{
		for(int i=0;i<item_list.size();i++)
		{
			ITEM_CODE t_ic = item_list[i];
			
			CCSprite* n_item = CCSprite::create("button_item_normal.png");
			CCSprite* s_item = CCSprite::create("button_item_normal.png");
			s_item->setColor(ccGRAY);
			
			CCMenuItem* item_item = CCMenuItemSprite::create(n_item, s_item, this, menu_selector(StageSettingPopupLayer::menuAction));
			item_item->setTag(kSSPL_MT_itemBase+i);
			
			CCMenu* item_menu = CCMenu::createWithItem(item_item);
			item_menu->setPosition(ccp(102+(i%2)*80, 186-(i/2)*65));
			addChild(item_menu, kSSPL_Z_menu, kSSPL_CT_menuBase+i);
			
			CCSprite* item_img = CCSprite::create(CCString::createWithFormat("item%d.png", t_ic)->getCString());
			item_img->setPosition(item_menu->getPosition());
			addChild(item_img, kSSPL_Z_itemImg);
			
			int item_cnt = myDSH->getIntegerForKey(kDSH_Key_haveItemCnt_int1, t_ic);
			if(item_cnt > 0)
			{
				CCSprite* cnt_img = CCSprite::create("item_cnt_label.png");
				cnt_img->setPosition(ccpAdd(item_menu->getPosition(), ccp(-25,17)));
				addChild(cnt_img, kSSPL_Z_itemImg, kSSPL_CT_itemCntBase+i);
				
				CCLabelTTF* cnt_label = CCLabelTTF::create(CCString::createWithFormat("%d", item_cnt)->getCString(), mySGD->getFont().c_str(), 12);
				cnt_label->setPosition(ccp(cnt_img->getContentSize().width/2.f, cnt_img->getContentSize().height/2.f));
				cnt_img->addChild(cnt_label);
				
				if(mySGD->isBeforeUseItem(item_list[i]))
				{
					is_selected_item.push_back(true);
					CCSprite* selected_img = CCSprite::create("button_item_selected.png");
					selected_img->setPosition(ccp(102+(i%2)*80, 186-(i/2)*65));
					addChild(selected_img, kSSPL_Z_selectedImg, kSSPL_CT_selectedBase+i);
				}
				else
					is_selected_item.push_back(false);
			}
			else
			{
				CCSprite* cnt_img = CCSprite::create("item_cnt_plus.png");
				cnt_img->setPosition(ccpAdd(item_menu->getPosition(), ccp(25,-17)));
				addChild(cnt_img, kSSPL_Z_itemImg, kSSPL_CT_itemCntBase+i);
				
				is_selected_item.push_back(false);
			}
		}
		
		if(item_list.size() > 0)
		{
			item_script = CCLabelTTF::create(mySD->getItemScript(ITEM_CODE(-1)).c_str(), mySGD->getFont().c_str(), 12, CCSizeMake(140, 60), kCCTextAlignmentCenter);
			item_script->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
			item_script->setColor(ccBLACK);
			item_script->setPosition(ccp(142,68));
			addChild(item_script, kSSPL_Z_itemImg);
		}
	}
	
	void missionSetting(CLEAR_CONDITION clr_condition)
	{
		CCSprite* clr_cdt_img = CCSprite::create(CCString::createWithFormat("condition%d_menu.png", clr_condition)->getCString());
		clr_cdt_img->setPosition(ccp(337, 182));
		addChild(clr_cdt_img, kSSPL_Z_menu);
		
		if(clr_condition == kCLEAR_subCumberCatch)
		{
			int catch_cnt = mySD->getClearConditionCatchSubCumber(stage_number);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", catch_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		else if(clr_condition == kCLEAR_bigArea)
		{
			float clr_cdt_per = mySD->getClearConditionBigAreaPer(stage_number);
			int clr_cdt_cnt = mySD->getClearConditionBigAreaCnt(stage_number);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d", clr_cdt_per*100.f, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		else if(clr_condition == kCLEAR_itemCollect)
		{
			int collect_cnt = mySD->getClearConditionItemCollect(stage_number);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", collect_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		else if(clr_condition == kCLEAR_perfect)
		{
			float perfect_per = mySD->getClearConditionPerfectBase(stage_number);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%.0f%%", perfect_per*100.f)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		else if(clr_condition == kCLEAR_sequenceChange)
		{
			CCSprite* c_coin = CCSprite::create("exchange_1_act.png");
			c_coin->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(c_coin, kSSPL_Z_menu);
		}
		else if(clr_condition == kCLEAR_timeLimit)
		{
			int clr_time = mySD->getClearConditionTimeLimit(stage_number);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", clr_time)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		
		CCLabelTTF* content = CCLabelTTF::create(mySD->getConditionContent(stage_number).c_str(), mySGD->getFont().c_str(), 13, CCSizeMake(140, 60), kCCTextAlignmentCenter);
		content->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
		content->setColor(ccBLACK);
		content->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-55)));
		addChild(content, kSSPL_Z_menu);
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == 0)
		{
			if(start_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kSSPL_MT_start;
			else if(cancel_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kSSPL_MT_cancel;
			else
			{
				int item_list_cnt = item_list.size();
				for(int i=0;i<item_list_cnt && touched_number == 0;i++)
				{
					if(((CCMenu*)getChildByTag(kSSPL_CT_menuBase+i))->ccTouchBegan(pTouch, pEvent))
						touched_number = kSSPL_MT_itemBase+i;
				}
			}
		}
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == 0)		return;
		if(touched_number == kSSPL_MT_start)						start_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kSSPL_MT_cancel)					cancel_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number >= kSSPL_MT_itemBase)
		{
			((CCMenu*)getChildByTag(kSSPL_CT_menuBase+(touched_number-kSSPL_MT_itemBase)))->ccTouchMoved(pTouch, pEvent);
		}
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == 0)		return;
		if(touched_number == kSSPL_MT_start)						start_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kSSPL_MT_cancel)					cancel_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number >= kSSPL_MT_itemBase)
			((CCMenu*)getChildByTag(kSSPL_CT_menuBase+(touched_number-kSSPL_MT_itemBase)))->ccTouchEnded(pTouch, pEvent);
		touched_number = 0;
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == 0)		return;
		if(touched_number == kSSPL_MT_start)						start_menu->ccTouchCancelled(pTouch, pEvent);
		else if(touched_number == kSSPL_MT_cancel)					cancel_menu->ccTouchCancelled(pTouch, pEvent);
		else if(touched_number >= kSSPL_MT_itemBase)
			((CCMenu*)getChildByTag(kSSPL_CT_menuBase+(touched_number-kSSPL_MT_itemBase)))->ccTouchCancelled(pTouch, pEvent);
		touched_number = 0;
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -150, true);
	}
};

#endif /* defined(__DGproto__StageSettingPopup__) */
