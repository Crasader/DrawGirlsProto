//
//  EventPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 30..
//
//

#ifndef __DGproto__EventPopup__
#define __DGproto__EventPopup__

#include "cocos2d.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "StageInfoDown.h"

USING_NS_CC;
using namespace std;

enum EventPopupZorder{
	kEP_Z_gray = 1,
	kEP_Z_back,
	kEP_Z_selected,
	kEP_Z_content,
	kEP_Z_popup
};

enum EventPopupMenuTag{
	kEP_MT_close = 1,
	kEP_MT_start,
	kEP_MT_eventStageBase = 10000
};

class EventPopup : public CCLayer
{
public:
	static EventPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		EventPopup* t_ep = new EventPopup();
		t_ep->myInit(t_close, d_close);
		t_ep->autorelease();
		return t_ep;
	}
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* close_menu;
	CCMenu* start_menu;
	
	CCSprite* selected_event_button;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	int selected_event_code;
	int el_length;
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close)
	{
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kEP_Z_gray);
		
		CCSprite* back = CCSprite::create("event_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kEP_Z_back);
		
		
		CCSprite* n_close = CCSprite::create("ui_common_close.png");
		CCSprite* s_close = CCSprite::create("ui_common_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(EventPopup::menuAction));
		close_item->setTag(kEP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kEP_MT_close));
		addChild(close_menu, kEP_Z_content);
		
		
		CCSprite* n_start = CCSprite::create("event_start.png");
		CCSprite* s_start = CCSprite::create("event_start.png");
		s_start->setColor(ccGRAY);
		
		CCMenuItem* start_item = CCMenuItemSprite::create(n_start, s_start, this, menu_selector(EventPopup::menuAction));
		start_item->setTag(kEP_MT_start);
		
		start_menu = CCMenu::createWithItem(start_item);
		start_menu->setPosition(getContentPosition(kEP_MT_start));
		addChild(start_menu, kEP_Z_content);
		
		
		el_length = SDS_GI(kSDF_gameInfo, "event_count");
		for(int i=0;i<el_length;i++)
		{
			int event_code = SDS_GI(kSDF_gameInfo, CCString::createWithFormat("event%d_code", i)->getCString());
			CCSprite* n_event = mySIL->getLoadedImg(CCString::createWithFormat("event%d_thumbnail.png", i)->getCString());
			CCSprite* s_event = mySIL->getLoadedImg(CCString::createWithFormat("event%d_thumbnail.png", i)->getCString());
			s_event->setColor(ccGRAY);
			
			CCMenuItem* event_item = CCMenuItemSprite::create(n_event, s_event, this, menu_selector(EventPopup::menuAction));
			event_item->setTag(event_code);
			
			CCMenu* event_menu = CCMenu::createWithItem(event_item);
			event_menu->setPosition(getContentPosition(event_code));
			addChild(event_menu, kEP_Z_content, kEP_MT_eventStageBase + i);
		}
		
		is_menu_enable = true;
		selected_event_code = 0;
		touched_number = 0;
		
		selected_event_button = NULL;
		
		setTouchEnabled(true);
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kEP_MT_close)						return_value = ccp(430,280);
		else if(t_tag == kEP_MT_start)					return_value = ccp(385,48);
		else if(t_tag >= kEP_MT_eventStageBase)			return_value = ccp(128,234 - (t_tag - kEP_MT_eventStageBase - 1)*45);
		
		return return_value;
	}
	
	void startCancel()
	{
		is_menu_enable = true;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kEP_MT_close)
		{
			(target_close->*delegate_close)();
			removeFromParent();
		}
		else if(tag == kEP_MT_start)
		{
			if(selected_event_code == 0)
			{
				is_menu_enable = true;
			}
			else
			{
				mySD->setSilType(selected_event_code);
				
				StageInfoDown* t_sid = StageInfoDown::create(this, callfunc_selector(EventPopup::startCancel));
				addChild(t_sid, kEP_Z_popup);
			}
		}
		else if(tag >= kEP_MT_eventStageBase)
		{
			if(tag != selected_event_code)
			{
				selected_event_code = tag;
				
				if(!selected_event_button)
				{
					selected_event_button = CCSprite::create("event_button_selected.png");
					addChild(selected_event_button, kEP_Z_selected);
				}
				selected_event_button->setPosition(getContentPosition(selected_event_code));
				is_menu_enable = true;
			}
			else
			{
				mySD->setSilType(selected_event_code);
				
				StageInfoDown* t_sid = StageInfoDown::create(this, callfunc_selector(EventPopup::startCancel));
				addChild(t_sid, kEP_Z_popup);
			}
		}
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kEP_MT_close;
		else if(start_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kEP_MT_start;
		else
		{
			for(int i=0;i<el_length && touched_number == 0;i++)
			{
				if(((CCMenu*)getChildByTag(kEP_MT_eventStageBase + i))->ccTouchBegan(pTouch, pEvent))		touched_number = kEP_MT_eventStageBase + i;
			}
		}
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kEP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kEP_MT_start)						start_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number != 0)
		{
			((CCMenu*)getChildByTag(touched_number))->ccTouchMoved(pTouch, pEvent);
		}
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kEP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kEP_MT_start){	start_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number != 0)
		{
			((CCMenu*)getChildByTag(touched_number))->ccTouchEnded(pTouch, pEvent);
			touched_number = 0;
		}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kEP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kEP_MT_start){	start_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number != 0)
		{
			((CCMenu*)getChildByTag(touched_number))->ccTouchCancelled(pTouch, pEvent);
			touched_number = 0;
		}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__EventPopup__) */
