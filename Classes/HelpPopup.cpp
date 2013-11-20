// HelpPopup.cpp
//

#include "HelpPopup.h"
#define LZZ_INLINE inline
using namespace std;
HelpPopup * HelpPopup::create (CCObject * t_close, SEL_CallFunc d_close)
        {
		HelpPopup* t_hp = new HelpPopup();
		t_hp->myInit(t_close, d_close);
		t_hp->autorelease();
		return t_hp;
	}
void HelpPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
        {
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kHP_Z_gray);
		
		CCSprite* back = CCSprite::create("option_help_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kHP_Z_back);
		
		
		CCSprite* n_close = CCSprite::create("option_short_close.png");
		CCSprite* s_close = CCSprite::create("option_short_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(HelpPopup::menuAction));
		close_item->setTag(kHP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kHP_MT_close));
		addChild(close_menu, kHP_Z_content);
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
CCPoint HelpPopup::getContentPosition (int t_tag)
        {
		CCPoint return_value;
		
		if(t_tag == kHP_MT_close)	return_value = ccp(183,71);
		
		return return_value;
	}
void HelpPopup::menuAction (CCObject * sender)
        {
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kHP_MT_close)
		{
			(target_close->*delegate_close)();
		}
		
		removeFromParent();
	}
bool HelpPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number != 0)		return true;
		if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kHP_MT_close;
		return true;
	}
void HelpPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kHP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
	}
void HelpPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kHP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	}
void HelpPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kHP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	}
void HelpPopup::registerWithTouchDispatcher ()
        {
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
#undef LZZ_INLINE
