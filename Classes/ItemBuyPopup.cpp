// ItemBuyPopup.cpp
//

#include "ItemBuyPopup.h"
#include "DataStorageHub.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"
#define LZZ_INLINE inline
using namespace std;
ItemBuyPopup * ItemBuyPopup::create (ITEM_CODE t_item_type, int t_clicked_item_number, CCObject * t_buy, SEL_CallFuncII d_buy)
{
	ItemBuyPopup* t_ibp = new ItemBuyPopup();
	t_ibp->myInit(t_item_type, t_clicked_item_number, t_buy, d_buy);
	t_ibp->autorelease();
	return t_ibp;
}

void ItemBuyPopup::setCloseTargetDelegate(CCObject *t_close, SEL_CallFunc d_close)
{
	target_close = t_close;
	delegate_close = d_close;
}

void ItemBuyPopup::myInit (ITEM_CODE t_item_type, int t_clicked_item_number, CCObject * t_buy, SEL_CallFuncII d_buy)
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
void ItemBuyPopup::menuAction (CCObject * sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kIBP_MT_buy)
	{
		string item_currency = mySD->getItemCurrency(item_type);
		
		if(item_currency == "gold")
		{
			if(mySD->getItemPrice(item_type) <= mySGD->getGoodsValue(kGoodsType_gold))
			{
//				mySGD->setGold(mySGD->getGoodsValue(kGoodsType_gold)-mySD->getItemPrice(item_type));
				(target_buy->*delegate_buy)(clicked_item_number, 1);
			}
		}
		else if(item_currency == "ruby")
		{
			if(mySD->getItemPrice(item_type) <= mySGD->getGoodsValue(kGoodsType_ruby))
			{
//				mySGD->setStar(mySGD->getGoodsValue(kGoodsType_ruby)-mySD->getItemPrice(item_type));
				(target_buy->*delegate_buy)(clicked_item_number, 1);
			}
		}
	}
	else if(tag == kIBP_MT_close)
	{
		
	}
	
	(target_close->*delegate_close)();
	
	removeFromParent();
}
bool ItemBuyPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number != 0)		return true;
	if(buy_menu->ccTouchBegan(pTouch, pEvent))					touched_number = kIBP_MT_buy;
	else if(close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kIBP_MT_close;
	return true;
}
void ItemBuyPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kIBP_MT_buy)							buy_menu->ccTouchMoved(pTouch, pEvent);
	else if(touched_number == kIBP_MT_close)					close_menu->ccTouchMoved(pTouch, pEvent);
}
void ItemBuyPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kIBP_MT_buy){			buy_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	else if(touched_number == kIBP_MT_close){	close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
}
void ItemBuyPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kIBP_MT_buy){			buy_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	else if(touched_number == kIBP_MT_close){	close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
}
void ItemBuyPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
#undef LZZ_INLINE
