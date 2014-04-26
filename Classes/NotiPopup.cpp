// NotiPopup.cpp
//

#include "NotiPopup.h"
#include "AudioEngine.h"
#define LZZ_INLINE inline
using namespace std;
NotiPopup * NotiPopup::create (CCObject * t_close, SEL_CallFunc d_close)
        {
		NotiPopup* t_np = new NotiPopup();
		t_np->myInit(t_close, d_close);
		t_np->autorelease();
		return t_np;
	}
void NotiPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
        {
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kNP_Z_gray);
		
		back = CCSprite::create("option_noti_list_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kNP_Z_back);
		
		
		CCSprite* n_button = CCSprite::create("option_noti_list_button.png");
		CCSprite* s_button = CCSprite::create("option_noti_list_button.png");
		s_button->setColor(ccGRAY);
		
		CCMenuItem* button_item = CCMenuItemSprite::create(n_button, s_button, this, menu_selector(NotiPopup::menuAction));
		button_item->setTag(kNP_MT_itemBase);
		
		button_menu = CCMenu::createWithItem(button_item);
		button_menu->setPosition(getContentPosition(kNP_MT_itemBase));
		back->addChild(button_menu);
		
		CCSprite* n_close = CCSprite::create("ui_common_close.png");
		CCSprite* s_close = CCSprite::create("ui_common_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(NotiPopup::menuAction));
		close_item->setTag(kNP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kNP_MT_close));
		back->addChild(close_menu);
		
		is_list = true;
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
CCPoint NotiPopup::getContentPosition (int t_tag)
        {
		CCPoint return_value;
		
		if(t_tag == kNP_MT_close)				return_value = ccp(345,267);
		else if(t_tag == kNP_MT_itemBase)		return_value = ccp(240,210);
		else if(t_tag == kNP_MT_content_close)	return_value = ccp(240,71);
		
		return return_value;
	}
void NotiPopup::menuAction (CCObject * sender)
        {
		if(!is_menu_enable)
			return;
		
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kNP_MT_close)
		{
			(target_close->*delegate_close)();
			removeFromParent();
		}
		else if(tag == kNP_MT_itemBase)
		{
			touched_number = -1;
			back->removeFromParent();
			
			back = CCSprite::create("option_noti_content_back.png");
			back->setPosition(ccp(240,160));
			addChild(back, kNP_Z_back);
			
			CCSprite* n_content_close = CCSprite::create("option_short_close.png");
			CCSprite* s_content_close = CCSprite::create("option_short_close.png");
			s_content_close->setColor(ccGRAY);
			
			CCMenuItem* content_close_item = CCMenuItemSprite::create(n_content_close, s_content_close, this, menu_selector(NotiPopup::menuAction));
			content_close_item->setTag(kNP_MT_content_close);
			
			content_close_menu = CCMenu::createWithItem(content_close_item);
			content_close_menu->setPosition(getContentPosition(kNP_MT_content_close));
			back->addChild(content_close_menu);
			
			is_list = false;
			touched_number = 0;
			is_menu_enable = true;
		}
		else if(tag == kNP_MT_content_close)
		{
			touched_number = -1;
			back->removeFromParent();
			
			back = CCSprite::create("option_noti_list_back.png");
			back->setPosition(ccp(240,160));
			addChild(back, kNP_Z_back);
			
			
			CCSprite* n_button = CCSprite::create("option_noti_list_button.png");
			CCSprite* s_button = CCSprite::create("option_noti_list_button.png");
			s_button->setColor(ccGRAY);
			
			CCMenuItem* button_item = CCMenuItemSprite::create(n_button, s_button, this, menu_selector(NotiPopup::menuAction));
			button_item->setTag(kNP_MT_itemBase);
			
			button_menu = CCMenu::createWithItem(button_item);
			button_menu->setPosition(getContentPosition(kNP_MT_itemBase));
			back->addChild(button_menu);
			
			CCSprite* n_close = CCSprite::create("ui_common_close.png");
			CCSprite* s_close = CCSprite::create("ui_common_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(NotiPopup::menuAction));
			close_item->setTag(kNP_MT_close);
			
			close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(getContentPosition(kNP_MT_close));
			back->addChild(close_menu);
			
			is_list = true;
			touched_number = 0;
			is_menu_enable = true;
		}
	}
bool NotiPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number != 0)		return true;
		if(is_list)
		{
			if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kNP_MT_close;
			else if(button_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kNP_MT_itemBase;
		}
		else
		{
			if(content_close_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kNP_MT_content_close;
		}
		return true;
	}
void NotiPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(is_list)
		{
			if(touched_number == kNP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
			else if(touched_number == kNP_MT_itemBase)					button_menu->ccTouchMoved(pTouch, pEvent);
		}
		else
		{
			if(touched_number == kNP_MT_content_close)					content_close_menu->ccTouchMoved(pTouch, pEvent);
		}
	}
void NotiPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(is_list)
		{
			if(touched_number == kNP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
			else if(touched_number == kNP_MT_itemBase){	button_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		}
		else
		{
			if(touched_number == kNP_MT_content_close){	content_close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		}
	}
void NotiPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(is_list)
		{
			if(touched_number == kNP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
			else if(touched_number == kNP_MT_itemBase){	button_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		}
		else
		{
			if(touched_number == kNP_MT_content_close){	content_close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		}
	}
void NotiPopup::registerWithTouchDispatcher ()
        {
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
#undef LZZ_INLINE
