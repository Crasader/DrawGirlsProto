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

USING_NS_CC;

enum SSPL_MenuTag{
	kSSPL_MT_start = 1,
	kSSPL_MT_cancel
};

enum SSPL_Zorder{
	kSSPL_Z_back = 1,
	kSSPL_Z_menu
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
	
	void myInit(int stage, CCObject* t_worldmap, SEL_CallFunc d_cancel)
	{
		stage_number = stage;
		target_worldmap = t_worldmap;
		delegate_cancel = d_cancel;
		
		CLEAR_CONDITION clr_condition = mySD->getClearCondition(stage);
		

		CCSprite* sspl_back = CCSprite::create("sspl_back.png");
		sspl_back->setPosition(ccp(240,160));
		addChild(sspl_back, kSSPL_Z_back);
		
		CCLabelTTF* stage_number = CCLabelTTF::create(CCString::createWithFormat("%d", stage)->getCString(), mySGD->getFont().c_str(), 20);
		stage_number->setPosition(ccp(195,272));
		addChild(stage_number, kSSPL_Z_menu);
		
		CCSprite* clr_cdt_img = CCSprite::create(CCString::createWithFormat("condition%d_menu.png", clr_condition)->getCString());
		clr_cdt_img->setPosition(ccp(337, 182));
		addChild(clr_cdt_img, kSSPL_Z_menu);
		
		if(clr_condition == kCLEAR_subCumberCatch)
		{
			int catch_cnt = mySD->getClearConditionCatchSubCumber(stage);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", catch_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		else if(clr_condition == kCLEAR_bigArea)
		{
			float clr_cdt_per = mySD->getClearConditionBigAreaPer(stage);
			int clr_cdt_cnt = mySD->getClearConditionBigAreaCnt(stage);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%2.0f%%:%d", clr_cdt_per*100.f, clr_cdt_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		else if(clr_condition == kCLEAR_itemCollect)
		{
			int collect_cnt = mySD->getClearConditionItemCollect(stage);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", collect_cnt)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		else if(clr_condition == kCLEAR_perfect)
		{
			float perfect_per = mySD->getClearConditionPerfectBase(stage);
			
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
			int clr_time = mySD->getClearConditionTimeLimit(stage);
			
			CCLabelTTF* clr_cdt_label = CCLabelTTF::create(CCString::createWithFormat("%d", clr_time)->getCString(), mySGD->getFont().c_str(), 12);
			clr_cdt_label->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-5)));
			addChild(clr_cdt_label, kSSPL_Z_menu);
		}
		
		CCLabelTTF* content = CCLabelTTF::create(mySD->getConditionContent(stage).c_str(), mySGD->getFont().c_str(), 13, CCSizeMake(140, 60), kCCTextAlignmentCenter);
		content->setColor(ccBLACK);
		content->setPosition(ccpAdd(clr_cdt_img->getPosition(), ccp(0,-68)));
		addChild(content, kSSPL_Z_menu);
		
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
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		touched_number = 0;
		if(start_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kSSPL_MT_start;
		else if(cancel_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kSSPL_MT_cancel;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kSSPL_MT_start)						start_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kSSPL_MT_cancel)					cancel_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kSSPL_MT_start)						start_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kSSPL_MT_cancel)					cancel_menu->ccTouchEnded(pTouch, pEvent);
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kSSPL_MT_start)						start_menu->ccTouchEnded(pTouch, pEvent);
		else if(touched_number == kSSPL_MT_cancel)					cancel_menu->ccTouchEnded(pTouch, pEvent);
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -150, true);
	}
};

#endif /* defined(__DGproto__StageSettingPopup__) */
