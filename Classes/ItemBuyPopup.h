//
//  ItemBuyPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 14..
//
//

#ifndef __DGproto__ItemBuyPopup__
#define __DGproto__ItemBuyPopup__

#include "cocos2d.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"

USING_NS_CC;
using namespace std;

enum IBP_MenuTag{
	kIBP_MT_buy = 1,
	kIBP_MT_close
};

enum IBP_Zorder{
	kIBP_Z_back = 1,
	kIBP_Z_case,
	kIBP_Z_menu
};

class ItemBuyPopup : public CCLayer
{
public:
	static ItemBuyPopup* create(ITEM_CODE t_item_type, int t_clicked_item_number, CCObject* t_buy, SEL_CallFuncII d_buy)
	{
		ItemBuyPopup* t_ibp = new ItemBuyPopup();
		t_ibp->myInit(t_item_type, t_clicked_item_number, t_buy, d_buy);
		t_ibp->autorelease();
		return t_ibp;
	}
	
private:
	bool is_menu_enable;
	
	ITEM_CODE item_type;
	int clicked_item_number;
	
	int touched_number;
	CCMenu* buy_menu;
	CCMenu* close_menu;
	
	CCObject* target_buy;
	SEL_CallFuncII delegate_buy;
	
	void myInit(ITEM_CODE t_item_type, int t_clicked_item_number, CCObject* t_buy, SEL_CallFuncII d_buy)
	{
		item_type = t_item_type;
		clicked_item_number = t_clicked_item_number;
		target_buy = t_buy;
		delegate_buy = d_buy;
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		CCSprite* back = CCSprite::create("back_gray.png");
		back->setPosition(ccp(240,160));
		back->setScaleX(screen_scale_x);
		back->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
		addChild(back, kIBP_Z_back);
		
		
		CCSprite* buy_case = CCSprite::create("item_buy_popup_back.png");
		buy_case->setPosition(ccp(240, 160));
		addChild(buy_case, kIBP_Z_case);
		
//		CCSprite* item_img = CCSprite::create(CCString::createWithFormat("item%d.png", item_type)->getCString());
//		item_img->setPosition(ccp(50,85));
//		buy_case->addChild(item_img);
//		
//		CCLabelTTF* item_price = CCLabelTTF::create(CCString::createWithFormat("%.0f원", mySD->getItemPrice(item_type))->getCString(), mySGD->getFont().c_str(), 25);
//		item_price->setPosition(ccp(140,83));
//		buy_case->addChild(item_price);
		
		
		CCSprite* n_buy = CCSprite::create("itemshop_buy.png");
		CCSprite* s_buy = CCSprite::create("itemshop_buy.png");
		s_buy->setColor(ccGRAY);
		
		CCMenuItem* buy_item = CCMenuItemSprite::create(n_buy, s_buy, this, menu_selector(ItemBuyPopup::menuAction));
		buy_item->setTag(kIBP_MT_buy);
		
		buy_menu = CCMenu::createWithItem(buy_item);
		buy_menu->setPosition(ccp(240,115));
		addChild(buy_menu, kIBP_Z_menu);
		
		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(ItemBuyPopup::menuAction));
		close_item->setTag(kIBP_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setPosition(ccp(355,215));
		addChild(close_menu, kIBP_Z_menu);
		
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
		
		if(tag == kIBP_MT_buy)
		{
			if(mySD->getItemPrice(item_type) <= mySGD->getGold())
			{
				mySGD->setGold(mySGD->getGold()-mySD->getItemPrice(item_type));
				(target_buy->*delegate_buy)(clicked_item_number, 1);
			}
		}
		else if(tag == kIBP_MT_close)
		{
			
		}
		
		removeFromParent();
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(buy_menu->ccTouchBegan(pTouch, pEvent))					touched_number = kIBP_MT_buy;
		else if(close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kIBP_MT_close;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kIBP_MT_buy)							buy_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kIBP_MT_close)					close_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kIBP_MT_buy){			buy_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kIBP_MT_close){	close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kIBP_MT_buy){			buy_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kIBP_MT_close){	close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__ItemBuyPopup__) */
