//
//  ShopPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 22..
//
//

#ifndef __DGproto__ShopPopup__
#define __DGproto__ShopPopup__

#include "cocos2d.h"
#include "hspConnector.h"

USING_NS_CC;
using namespace std;

enum ShopCode{
	kSC_empty = 0,
	kSC_ruby,
	kSC_gold,
	kSC_heart
};

class HeartTime;
class LoadingLayer;
class ShopPopup : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(ShopPopup);
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	void targetHeartTime(HeartTime* t_heartTime);
	
	void setShopCode(ShopCode t_code);
	
private:
	
	ShopCode recent_shop_code;
	HeartTime* target_heartTime;
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	LoadingLayer* loading_layer;
	
	CCSprite* main_case;
	
	bool is_menu_enable;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	void resultSetUserData(Json::Value result_data);
	
	CCPoint getContentPosition(int t_tag);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__ShopPopup__) */
