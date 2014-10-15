// WithdrawPopup.cpp
//

#include "WithdrawPopup.h"
#include "DataStorageHub.h"
#include "TitleRenewal.h"
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
	
	CCSprite* n_withdraw = CCSprite::create("option_withdraw.png");
	CCSprite* s_withdraw = CCSprite::create("option_withdraw.png");
	s_withdraw->setColor(ccGRAY);
	
	CCMenuItem* withdraw_item = CCMenuItemSprite::create(n_withdraw, s_withdraw, this, menu_selector(WithdrawPopup::menuAction));
	withdraw_item->setTag(kWP_MT_withdraw);
	
	withdraw_menu = CCMenu::createWithItem(withdraw_item);
	withdraw_menu->setPosition(getContentPosition(kWP_MT_withdraw));
	addChild(withdraw_menu, kWP_Z_content);
	
	
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

void WithdrawPopup::resultDropoutuser(Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		myDSH->clear();
		myDSH->resetDSH();
		CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
	}
	else
	{
		CCLOG("fail dropoutuser");
		touched_number = 0;
	}
}

CCPoint WithdrawPopup::getContentPosition (int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kWP_MT_close)			return_value = ccp(240,97);
	else if(t_tag == kWP_MT_withdraw)	return_value = ccp(240,155);
	
	return return_value;
}
void WithdrawPopup::menuAction (CCObject * sender)
{
	if(!is_menu_enable)
		return;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);

	is_menu_enable = false;
	
	int tag = ((CCNode*)sender)->getTag();
	
	if(tag == kWP_MT_close)
	{
		(target_close->*delegate_close)();
		removeFromParent();
	}
	else if(tag == kWP_MT_withdraw)
	{
		Json::Value param;
		param["memberID"] = hspConnector::get()->getMemberID();
		hspConnector::get()->command("dropoutuser", param, json_selector(this, WithdrawPopup::resultDropoutuser));
		touched_number = -1;
	}
}
bool WithdrawPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number != 0)		return true;
	if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kWP_MT_close;
	else if(withdraw_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kWP_MT_withdraw;
	return true;
}
void WithdrawPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kWP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
	else if(touched_number == kWP_MT_withdraw)					withdraw_menu->ccTouchMoved(pTouch, pEvent);
}
void WithdrawPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kWP_MT_close){			close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
	else if(touched_number == kWP_MT_withdraw){	withdraw_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
}
void WithdrawPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
	if(touched_number == kWP_MT_close){			close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	else if(touched_number == kWP_MT_withdraw){	withdraw_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
}
void WithdrawPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}
#undef LZZ_INLINE
