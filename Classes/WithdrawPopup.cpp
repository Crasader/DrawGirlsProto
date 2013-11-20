// WithdrawPopup.cpp
//

#include "WithdrawPopup.h"
#define LZZ_INLINE inline
using namespace std;
WithdrawPopup * WithdrawPopup::create (CCObject * t_close, SEL_CallFunc d_close)
        {
		WithdrawPopup* t_wp = new WithdrawPopup();
		t_wp->myInit(t_close, d_close);
		t_wp->autorelease();
		return t_wp;
	}
void WithdrawPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
        {
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kWP_Z_gray);
		
		CCSprite* back = CCSprite::create("option_withdraw_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kWP_Z_back);
		
		
		CCSprite* n_close = CCSprite::create("option_long_close.png");
		CCSprite* s_close = CCSprite::create("option_long_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(WithdrawPopup::menuAction));
		close_item->setTag(kWP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kWP_MT_close));
		addChild(close_menu, kWP_Z_content);
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
CCPoint WithdrawPopup::getContentPosition (int t_tag)
        {
		CCPoint return_value;
		
		if(t_tag == kWP_MT_close)			return_value = ccp(240,97);
		
		return return_value;
	}
void WithdrawPopup::menuAction (CCObject * sender)
        {
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kWP_MT_close)
		{
			(target_close->*delegate_close)();
		}
		
		removeFromParent();
	}
bool WithdrawPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number != 0)		return true;
		if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kWP_MT_close;
		return true;
	}
void WithdrawPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kWP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
	}
void WithdrawPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kWP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	}
void WithdrawPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kWP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	}
void WithdrawPopup::registerWithTouchDispatcher ()
        {
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
#undef LZZ_INLINE
