// FailHelpPopup.cpp
//

#include "FailHelpPopup.h"
#define LZZ_INLINE inline
using namespace std;
FailHelpPopup * FailHelpPopup::create (CCObject * t_close, SEL_CallFunc d_close)
        {
		FailHelpPopup* t_fhp = new FailHelpPopup();
		t_fhp->myInit(t_close, d_close);
		t_fhp->autorelease();
		return t_fhp;
	}
void FailHelpPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
        {
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kFHP_Z_gray);
		
		back = CCSprite::create("fail_help_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kFHP_Z_back);
		
		
		CCSprite* n_help = CCSprite::create("postbox_receive_help.png");
		CCSprite* s_help = CCSprite::create("postbox_receive_help.png");
		s_help->setColor(ccGRAY);
		
		CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailHelpPopup::menuAction));
		help_item->setTag(kFHP_MT_help);
		
		help_menu = CCMenu::createWithItem(help_item);
		help_menu->setPosition(getContentPosition(kFHP_MT_help));
		back->addChild(help_menu);
		
		
		CCSprite* n_close = CCSprite::create("ui_common_close.png");
		CCSprite* s_close = CCSprite::create("ui_common_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(FailHelpPopup::menuAction));
		close_item->setTag(kFHP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(getContentPosition(kFHP_MT_close));
		back->addChild(close_menu);
		
		is_list = true;
		
		is_menu_enable = true;
		touched_number = 0;
		setTouchEnabled(true);
	}
CCPoint FailHelpPopup::getContentPosition (int t_tag)
        {
		CCPoint return_value;
		
		if(t_tag == kFHP_MT_close)			return_value = ccp(343,283);
		else if(t_tag == kFHP_MT_help)		return_value = ccp(240,230);
		else if(t_tag == kFHP_MT_help_close)return_value = ccp(343,270);
		
		return return_value;
	}
void FailHelpPopup::menuAction (CCObject * sender)
        {
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kFHP_MT_close)
		{
			(target_close->*delegate_close)();
			removeFromParent();
		}
		else if(tag == kFHP_MT_help)
		{
			touched_number = -1;
			back->removeFromParent();
			
			back = CCSprite::create("fail_help_send_back.png");
			back->setPosition(ccp(240,160));
			addChild(back, kFHP_Z_back);
			
			CCSprite* n_help_close = CCSprite::create("ui_common_close.png");
			CCSprite* s_help_close = CCSprite::create("ui_common_close.png");
			s_help_close->setColor(ccGRAY);
			
			CCMenuItem* help_close_item = CCMenuItemSprite::create(n_help_close, s_help_close, this, menu_selector(FailHelpPopup::menuAction));
			help_close_item->setTag(kFHP_MT_help_close);
			
			help_close_menu = CCMenu::createWithItem(help_close_item);
			help_close_menu->setPosition(getContentPosition(kFHP_MT_help_close));
			back->addChild(help_close_menu);
			
			is_list = false;
			
			touched_number = 0;
			is_menu_enable = true;
		}
		else if(tag == kFHP_MT_help_close)
		{
			touched_number = -1;
			back->removeFromParent();
			
			back = CCSprite::create("fail_help_back.png");
			back->setPosition(ccp(240,160));
			addChild(back, kFHP_Z_back);
			
			
			CCSprite* n_help = CCSprite::create("postbox_receive_help.png");
			CCSprite* s_help = CCSprite::create("postbox_receive_help.png");
			s_help->setColor(ccGRAY);
			
			CCMenuItem* help_item = CCMenuItemSprite::create(n_help, s_help, this, menu_selector(FailHelpPopup::menuAction));
			help_item->setTag(kFHP_MT_help);
			
			help_menu = CCMenu::createWithItem(help_item);
			help_menu->setPosition(getContentPosition(kFHP_MT_help));
			back->addChild(help_menu);
			
			
			CCSprite* n_close = CCSprite::create("ui_common_close.png");
			CCSprite* s_close = CCSprite::create("ui_common_close.png");
			s_close->setColor(ccGRAY);
			
			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(FailHelpPopup::menuAction));
			close_item->setTag(kFHP_MT_close);
			
			close_menu = CCMenu::createWithItem(close_item);
			close_menu->setPosition(getContentPosition(kFHP_MT_close));
			back->addChild(close_menu);
			
			is_list = true;
			
			touched_number = 0;
			is_menu_enable = true;
		}
	}
bool FailHelpPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number != 0)		return true;
		
		if(is_list)
		{
			if(close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kFHP_MT_close;
			else if(help_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kFHP_MT_help;
		}
		else
		{
			if(help_close_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kFHP_MT_help_close;
		}
		
		return true;
	}
void FailHelpPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kFHP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kFHP_MT_help)						help_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kFHP_MT_help_close)				help_close_menu->ccTouchMoved(pTouch, pEvent);
	}
void FailHelpPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kFHP_MT_close){						close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kFHP_MT_help){					help_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kFHP_MT_help_close){				help_close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
	}
void FailHelpPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
        {
		if(touched_number == kFHP_MT_close){						close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kFHP_MT_help){					help_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kFHP_MT_help_close){				help_close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
	}
void FailHelpPopup::registerWithTouchDispatcher ()
        {
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
#undef LZZ_INLINE
