//
//  OnePercentGacha.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 12..
//
//

#ifndef __DGproto__OnePercentGacha__
#define __DGproto__OnePercentGacha__

#include "cocos2d.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"

USING_NS_CC;
using namespace std;

enum OnePercentGachaZorder{
	kOnePercentGacha_Z_gray = 1,
	kOnePercentGacha_Z_back,
	kOnePercentGacha_Z_content
};

enum OnePercentGachaMenuTag{
	kOnePercentGacha_MT_cancel = 1,
	kOnePercentGacha_MT_ok
};

class OnePercentGacha : public CCLayer
{
public:
	static OnePercentGacha* create(CCObject* t_cancel, SEL_CallFunc d_cancel, CCObject* t_ok, SEL_CallFunc d_ok)
	{
		OnePercentGacha* t_tnp = new OnePercentGacha();
		t_tnp->myInit(t_cancel, d_cancel, t_ok, d_ok);
		t_tnp->autorelease();
		return t_tnp;
	}
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* cancel_menu;
	CCMenu* ok_menu;
	
	CCObject* target_cancel;
	SEL_CallFunc delegate_cancel;
	CCObject* target_ok;
	SEL_CallFunc delegate_ok;
	
	CCSprite* back_img;
	
	void myInit(CCObject* t_cancel, SEL_CallFunc d_cancel, CCObject* t_ok, SEL_CallFunc d_ok)
	{
		target_cancel = t_cancel;
		delegate_cancel = d_cancel;
		target_ok = t_ok;
		delegate_ok = d_ok;
		
		back_img = NULL;
		cancel_menu = NULL;
		ok_menu = NULL;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,myDSH->ui_center_y));
		gray->setScaleY(myDSH->ui_top/320.f);
		addChild(gray, kOnePercentGacha_Z_gray);
		
		setBack();
		
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
		
		if(tag == kOnePercentGacha_MT_ok)
		{
			mySGD->setGold(mySGD->getGold()-500);
			(target_ok->*delegate_ok)();
		}
		else if(tag == kOnePercentGacha_MT_cancel)
		{
			(target_cancel->*delegate_cancel)();
		}
		
		removeFromParent();
	}
	
	void setBack()
	{
		back_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 350, 250));
		back_img->setPosition(ccp(240,myDSH->ui_center_y));
		addChild(back_img, kOnePercentGacha_Z_back);
		
		CCLabelTTF* noti_label = CCLabelTTF::create("아쉽게도 1%가 부족합니다.\n1%를 얻기 위한 가챠를 돌리시겠습니까?\n돌리고 싶으면 500원", mySGD->getFont().c_str(), 20, CCSizeMake(300,200), kCCTextAlignmentCenter,	kCCVerticalTextAlignmentCenter);
		noti_label->setColor(ccBLACK);
		noti_label->setAnchorPoint(ccp(0.5,0.5));
		noti_label->setPosition(ccp(175,165));
		back_img->addChild(noti_label, kOnePercentGacha_Z_content);
		
		CCSprite* n_ok = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 100, 70));
		n_ok->setColor(ccc3(200, 200, 200));
		CCLabelTTF* n_label = CCLabelTTF::create("OK", mySGD->getFont().c_str(), 15);
		n_label->setColor(ccBLACK);
		n_label->setAnchorPoint(ccp(0.5,0.5));
		n_label->setPosition(ccp(50,35));
		n_ok->addChild(n_label);
		
		CCSprite* s_ok = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 100, 70));
		s_ok->setColor(ccc3(130, 130, 130));
		CCLabelTTF* s_label = CCLabelTTF::create("OK", mySGD->getFont().c_str(), 15);
		s_label->setColor(ccBLACK);
		s_label->setAnchorPoint(ccp(0.5,0.5));
		s_label->setPosition(ccp(50,35));
		s_ok->addChild(s_label);
		
		CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, this, menu_selector(OnePercentGacha::menuAction));
		ok_item->setTag(kOnePercentGacha_MT_ok);
		ok_menu = CCMenu::createWithItem(ok_item);
		ok_menu->setPosition(ccp(245, 55));
		back_img->addChild(ok_menu, kOnePercentGacha_Z_content);
		
		
		CCSprite* n_cancel = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 100, 70));
		n_cancel->setColor(ccc3(200, 200, 200));
		CCLabelTTF* n_c_label = CCLabelTTF::create("CANCEL", mySGD->getFont().c_str(), 15);
		n_c_label->setColor(ccBLACK);
		n_c_label->setAnchorPoint(ccp(0.5,0.5));
		n_c_label->setPosition(ccp(50,35));
		n_cancel->addChild(n_c_label);
		
		CCSprite* s_cancel = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 100, 70));
		s_cancel->setColor(ccc3(130, 130, 130));
		CCLabelTTF* s_c_label = CCLabelTTF::create("CANCEL", mySGD->getFont().c_str(), 15);
		s_c_label->setColor(ccBLACK);
		s_c_label->setAnchorPoint(ccp(0.5,0.5));
		s_c_label->setPosition(ccp(50,35));
		s_cancel->addChild(s_c_label);
		
		CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, this, menu_selector(OnePercentGacha::menuAction));
		cancel_item->setTag(kOnePercentGacha_MT_cancel);
		cancel_menu = CCMenu::createWithItem(cancel_item);
		cancel_menu->setPosition(ccp(115, 55));
		back_img->addChild(cancel_menu, kOnePercentGacha_Z_content);
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		
		
		return return_value;
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(cancel_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kOnePercentGacha_MT_cancel;
		else if(ok_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kOnePercentGacha_MT_ok;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kOnePercentGacha_MT_cancel)							cancel_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kOnePercentGacha_MT_ok)						ok_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kOnePercentGacha_MT_cancel){			cancel_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kOnePercentGacha_MT_ok){			ok_menu->ccTouchEnded(pTouch, pEvent);			touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kOnePercentGacha_MT_cancel){			cancel_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		else if(touched_number == kOnePercentGacha_MT_ok){			ok_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__OnePercentGacha__) */
