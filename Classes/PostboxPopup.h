//
//  PostboxPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 30..
//
//

#ifndef __DGproto__PostboxPopup__
#define __DGproto__PostboxPopup__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum PostboxPopupZorder{
	kPP_Z_gray = 1,
	kPP_Z_back,
	kPP_Z_content
};

enum PostboxPopupMenuTag{
	kPP_MT_close = 1,
	kPP_MT_challenge_close,
	kPP_MT_help_close,
	kPP_MT_ticketrequest_close,
	kPP_MT_noti_close,
	kPP_MT_receive_base = 100
};

enum PostboxPopupState{
	kPP_State_postbox = 1,
	kPP_State_challenge,
	kPP_State_help,
	kPP_State_ticketrequest,
	kPP_State_noti
};

enum PostboxReceived{
	kPR_challenge = 1,
	kPR_heart,
	kPR_help,
	kPR_noti,
	kPR_ticketrequest
};

class PostboxPopup : public CCLayer
{
public:
	static PostboxPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		PostboxPopup* t_pp = new PostboxPopup();
		t_pp->myInit(t_close, d_close);
		t_pp->autorelease();
		return t_pp;
	}
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* close_menu;
	CCMenu* challenge_close_menu;
	CCMenu* help_close_menu;
	CCMenu* ticketrequest_close_menu;
	CCMenu* noti_close_menu;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	PostboxPopupState my_state;
	
	CCSprite* back;
	
	vector<PostboxReceived> received_list;
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close)
	{
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kPP_Z_gray);
		
		back = CCSprite::create("postbox_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kPP_Z_back);
		
		
		CCSprite* n_close = CCSprite::create("ui_common_close.png");
		CCSprite* s_close = CCSprite::create("ui_common_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(PostboxPopup::menuAction));
		close_item->setTag(kPP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kPP_MT_close));
		back->addChild(close_menu);
		
		for(int i=kPR_challenge;i<=kPR_ticketrequest;i++)
			received_list.push_back(PostboxReceived(i));
		
		createReceivedList();
		
		my_state = kPP_State_postbox;
		
		is_menu_enable = true;
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	void createReceivedList()
	{
		for(int i=0;i<received_list.size();i++)
		{
			PostboxReceived t_pr = received_list[i];
			
			string filename;
			
			if(t_pr == kPR_challenge)		filename = "postbox_receive_challenge.png";
			else if(t_pr == kPR_heart)		filename = "postbox_receive_heart.png";
			else if(t_pr == kPR_help)		filename = "postbox_receive_help.png";
			else if(t_pr == kPR_noti)		filename = "postbox_receive_noti.png";
			else if(t_pr == kPR_ticketrequest)	filename = "postbox_receive_ticketrequest.png";
			
			CCSprite* n_receive = CCSprite::create(filename.c_str());
			CCSprite* s_receive = CCSprite::create(filename.c_str());
			s_receive->setColor(ccGRAY);
			
			CCMenuItem* receive_item = CCMenuItemSprite::create(n_receive, s_receive, this, menu_selector(PostboxPopup::menuAction));
			receive_item->setTag(kPP_MT_receive_base + i);
			
			CCMenu* receive_menu = CCMenu::createWithItem(receive_item);
			receive_menu->setPosition(ccpAdd(getContentPosition(kPP_MT_receive_base), ccpMult(ccp(0,-44), i)));
			back->addChild(receive_menu, 1, kPP_MT_receive_base + i);
		}
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kPP_MT_close)					return_value = ccp(345,283);
		else if(t_tag == kPP_MT_receive_base)		return_value = ccp(240,230);
		else if(t_tag == kPP_MT_challenge_close)	return_value = ccp(183,71);
		else if(t_tag == kPP_MT_help_close)			return_value = ccp(183,71);
		else if(t_tag == kPP_MT_ticketrequest_close)return_value = ccp(345,283);
		else if(t_tag == kPP_MT_noti_close)			return_value = ccp(240,71);
		
		return return_value;
	}
	
	void backPostbox()
	{
		back->removeFromParent();
		
		back = CCSprite::create("postbox_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kPP_Z_back);
		
		
		CCSprite* n_close = CCSprite::create("ui_common_close.png");
		CCSprite* s_close = CCSprite::create("ui_common_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(PostboxPopup::menuAction));
		close_item->setTag(kPP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kPP_MT_close));
		back->addChild(close_menu);
		
		createReceivedList();
		
		my_state = kPP_State_postbox;
	}
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kPP_MT_close)
		{
			(target_close->*delegate_close)();
			removeFromParent();
		}
		else if(tag == kPP_MT_challenge_close)
		{
			touched_number = -1;
			
			backPostbox();
			
			touched_number = 0;
			is_menu_enable = true;
		}
		else if(tag == kPP_MT_help_close)
		{
			touched_number = -1;
			
			backPostbox();
			
			touched_number = 0;
			is_menu_enable = true;
		}
		else if(tag == kPP_MT_ticketrequest_close)
		{
			touched_number = -1;
			
			backPostbox();
			
			touched_number = 0;
			is_menu_enable = true;
		}
		else if(tag == kPP_MT_noti_close)
		{
			touched_number = -1;
			
			backPostbox();
			
			touched_number = 0;
			is_menu_enable = true;
		}
		else if(tag >= kPP_MT_receive_base)
		{
			int index = tag - kPP_MT_receive_base;
			PostboxReceived t_pr = received_list[index];
			
			if(t_pr == kPR_challenge)
			{
				touched_number = -1;
				back->removeFromParent();
				
				back = CCSprite::create("postbox_challenge_back.png");
				back->setPosition(ccp(240,160));
				addChild(back, kPP_Z_back);
				
				CCSprite* n_challenge_close = CCSprite::create("option_short_close.png");
				CCSprite* s_challenge_close = CCSprite::create("option_short_close.png");
				s_challenge_close->setColor(ccGRAY);
				
				CCMenuItem* challenge_close_item = CCMenuItemSprite::create(n_challenge_close, s_challenge_close, this, menu_selector(PostboxPopup::menuAction));
				challenge_close_item->setTag(kPP_MT_challenge_close);
				
				challenge_close_menu = CCMenu::createWithItem(challenge_close_item);
				challenge_close_menu->setPosition(getContentPosition(kPP_MT_challenge_close));
				back->addChild(challenge_close_menu);
				
				my_state = kPP_State_challenge;
				touched_number = 0;
				is_menu_enable = true;
			}
			else if(t_pr == kPR_heart)
			{
				received_list.erase(received_list.begin() + index);
				touched_number = -1;
				
				backPostbox();
				
				touched_number = 0;
				is_menu_enable = true;
			}
			else if(t_pr == kPR_help)
			{
				touched_number = -1;
				back->removeFromParent();
				
				back = CCSprite::create("postbox_help_back.png");
				back->setPosition(ccp(240,160));
				addChild(back, kPP_Z_back);
				
				CCSprite* n_help_close = CCSprite::create("option_short_close.png");
				CCSprite* s_help_close = CCSprite::create("option_short_close.png");
				s_help_close->setColor(ccGRAY);
				
				CCMenuItem* help_close_item = CCMenuItemSprite::create(n_help_close, s_help_close, this, menu_selector(PostboxPopup::menuAction));
				help_close_item->setTag(kPP_MT_help_close);
				
				help_close_menu = CCMenu::createWithItem(help_close_item);
				help_close_menu->setPosition(getContentPosition(kPP_MT_help_close));
				back->addChild(help_close_menu);
				
				my_state = kPP_State_help;
				touched_number = 0;
				is_menu_enable = true;
			}
			else if(t_pr == kPR_noti)
			{
				touched_number = -1;
				
				received_list.erase(received_list.begin()+index);
				
				back->removeFromParent();
				
				back = CCSprite::create("postbox_noti_back.png");
				back->setPosition(ccp(240,160));
				addChild(back, kPP_Z_back);
				
				CCSprite* n_noti_close = CCSprite::create("option_short_close.png");
				CCSprite* s_noti_close = CCSprite::create("option_short_close.png");
				s_noti_close->setColor(ccGRAY);
				
				CCMenuItem* noti_close_item = CCMenuItemSprite::create(n_noti_close, s_noti_close, this, menu_selector(PostboxPopup::menuAction));
				noti_close_item->setTag(kPP_MT_noti_close);
				
				noti_close_menu = CCMenu::createWithItem(noti_close_item);
				noti_close_menu->setPosition(getContentPosition(kPP_MT_noti_close));
				back->addChild(noti_close_menu);
				
				my_state = kPP_State_noti;
				touched_number = 0;
				is_menu_enable = true;
			}
			else if(t_pr == kPR_ticketrequest)
			{
				touched_number = -1;
				back->removeFromParent();
				
				back = CCSprite::create("postbox_ticketrequest_back.png");
				back->setPosition(ccp(240,160));
				addChild(back, kPP_Z_back);
				
				CCSprite* n_ticketrequest_close = CCSprite::create("ui_common_close.png");
				CCSprite* s_ticketrequest_close = CCSprite::create("ui_common_close.png");
				s_ticketrequest_close->setColor(ccGRAY);
				
				CCMenuItem* ticketrequest_close_item = CCMenuItemSprite::create(n_ticketrequest_close, s_ticketrequest_close, this, menu_selector(PostboxPopup::menuAction));
				ticketrequest_close_item->setTag(kPP_MT_ticketrequest_close);
				
				ticketrequest_close_menu = CCMenu::createWithItem(ticketrequest_close_item);
				ticketrequest_close_menu->setPosition(getContentPosition(kPP_MT_ticketrequest_close));
				back->addChild(ticketrequest_close_menu);
				
				my_state = kPP_State_ticketrequest;
				touched_number = 0;
				is_menu_enable = true;
			}
		}
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		
		if(my_state == kPP_State_postbox)
		{
			if(close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kPP_MT_close;
			else
			{
				for(int i=0;i<received_list.size() && touched_number == 0;i++)
					if(((CCMenu*)back->getChildByTag(kPP_MT_receive_base+i))->ccTouchBegan(pTouch, pEvent))		touched_number = kPP_MT_receive_base+i;
			}
		}
		else if(my_state == kPP_State_challenge)
		{
			if(challenge_close_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kPP_MT_challenge_close;
		}
		else if(my_state == kPP_State_help)
		{
			if(help_close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kPP_MT_help_close;
		}
		else if(my_state == kPP_State_ticketrequest)
		{
			if(ticketrequest_close_menu->ccTouchBegan(pTouch, pEvent))	touched_number = kPP_MT_ticketrequest_close;
		}
		else if(my_state == kPP_State_noti)
		{
			if(noti_close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kPP_MT_noti_close;
		}
		
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kPP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kPP_MT_challenge_close)			challenge_close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kPP_MT_help_close)				help_close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kPP_MT_ticketrequest_close)		ticketrequest_close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kPP_MT_noti_close)				noti_close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number >= kPP_MT_receive_base)				((CCMenu*)back->getChildByTag(touched_number))->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kPP_MT_close){							close_menu->ccTouchEnded(pTouch, pEvent);					touched_number = 0;	}
		else if(touched_number == kPP_MT_challenge_close){			challenge_close_menu->ccTouchEnded(pTouch, pEvent);			touched_number = 0;	}
		else if(touched_number == kPP_MT_help_close){				help_close_menu->ccTouchEnded(pTouch, pEvent);				touched_number = 0;	}
		else if(touched_number == kPP_MT_ticketrequest_close){		ticketrequest_close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kPP_MT_noti_close){				noti_close_menu->ccTouchEnded(pTouch, pEvent);				touched_number = 0;	}
		else if(touched_number >= kPP_MT_receive_base){				((CCMenu*)back->getChildByTag(touched_number))->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kPP_MT_close){							close_menu->ccTouchCancelled(pTouch, pEvent);					touched_number = 0;	}
		else if(touched_number == kPP_MT_challenge_close){			challenge_close_menu->ccTouchCancelled(pTouch, pEvent);			touched_number = 0;	}
		else if(touched_number == kPP_MT_help_close){				help_close_menu->ccTouchCancelled(pTouch, pEvent);				touched_number = 0;	}
		else if(touched_number == kPP_MT_ticketrequest_close){		ticketrequest_close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kPP_MT_noti_close){				noti_close_menu->ccTouchCancelled(pTouch, pEvent);				touched_number = 0;	}
		else if(touched_number >= kPP_MT_receive_base){				((CCMenu*)back->getChildByTag(touched_number))->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__PostboxPopup__) */
