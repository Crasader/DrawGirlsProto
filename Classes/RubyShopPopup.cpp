//// RubyShopPopup.cpp
////
//
//#include "RubyShopPopup.h"
//#define LZZ_INLINE inline
//using namespace std;
//RubyShopPopup * RubyShopPopup::create (CCObject * t_close, SEL_CallFunc d_close)
//        {
//		RubyShopPopup* t_rsp = new RubyShopPopup();
//		t_rsp->myInit(t_close, d_close);
//		t_rsp->autorelease();
//		return t_rsp;
//	}
//void RubyShopPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
//        {
//		target_close = t_close;
//		delegate_close = d_close;
//		
//		CCSprite* gray = CCSprite::create("back_gray.png");
//		gray->setPosition(ccp(240,160));
//		addChild(gray, kRSP_Z_gray);
//		
//		CCSprite* back = CCSprite::create("shop_rubyback.png");
//		back->setPosition(ccp(240,160));
//		addChild(back, kRSP_Z_back);
//		
//		
//		CCSprite* n_close = CCSprite::create("shop_close.png");
//		CCSprite* s_close = CCSprite::create("shop_close.png");
//		s_close->setColor(ccGRAY);
//		
//		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(RubyShopPopup::menuAction));
//		close_item->setTag(kRSP_MT_close);
//		
//		close_menu = CCMenu::createWithItem(close_item);
//		close_menu->setPosition(getContentPosition(kRSP_MT_close));
//		addChild(close_menu, kRSP_Z_content);
//		
//		is_menu_enable = true;
//		
//		touched_number = 0;
//		
//		setTouchEnabled(true);
//	}
//CCPoint RubyShopPopup::getContentPosition (int t_tag)
//        {
//		CCPoint return_value;
//		
//		if(t_tag == kRSP_MT_close)	return_value = ccp(380,50);
//		
//		return return_value;
//	}
//void RubyShopPopup::menuAction (CCObject * sender)
//        {
//		if(!is_menu_enable)
//			return;
//		
//		is_menu_enable = false;
//		
//		int tag = ((CCNode*)sender)->getTag();
//		
//		if(tag == kRSP_MT_close)
//		{
//			(target_close->*delegate_close)();
//		}
//		
//		removeFromParent();
//	}
//bool RubyShopPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
//        {
//		if(touched_number != 0)		return true;
//		if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kRSP_MT_close;
//		return true;
//	}
//void RubyShopPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
//        {
//		if(touched_number == kRSP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
//	}
//void RubyShopPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
//        {
//		if(touched_number == kRSP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
//	}
//void RubyShopPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
//        {
//		if(touched_number == kRSP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
//	}
//void RubyShopPopup::registerWithTouchDispatcher ()
//        {
//		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//		pDispatcher->addTargetedDelegate(this, -170, true);
//	}
//#undef LZZ_INLINE
