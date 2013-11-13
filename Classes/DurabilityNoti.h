//
//  DurabilityNoti.h
//  DGproto
//
//  Created by 사원3 on 13. 11. 11..
//
//

#ifndef __DGproto__DurabilityNoti__
#define __DGproto__DurabilityNoti__

#include "cocos2d.h"
#include "StarGoldData.h"

USING_NS_CC;
using namespace std;

enum DurabilityNotiZorder{
	kDN_Z_gray = 1,
	kDN_Z_back,
	kDN_Z_content
};

enum DurabilityNotiMenuTag{
	kDN_MT_cancel = 1,
	kDN_MT_ok
};

class DurabilityNoti : public CCLayer
{
public:
	static DurabilityNoti* create(CCObject* t_cancel, SEL_MenuHandler d_cancel, CCObject* t_ok, SEL_MenuHandler d_ok)
	{
		DurabilityNoti* t_tnp = new DurabilityNoti();
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
	SEL_MenuHandler delegate_cancel;
	CCObject* target_ok;
	SEL_MenuHandler delegate_ok;
	
	CCSprite* back_img;
	
	void myInit(CCObject* t_cancel, SEL_MenuHandler d_cancel, CCObject* t_ok, SEL_MenuHandler d_ok)
	{
		target_cancel = t_cancel;
		delegate_cancel = d_cancel;
		target_ok = t_ok;
		delegate_ok = d_ok;
		
		back_img = NULL;
		cancel_menu = NULL;
		ok_menu = NULL;
		
		CCSprite* gray = CCSprite::create("back_gray.png");
		gray->setPosition(ccp(240,160));
		addChild(gray, kDN_Z_gray);
		
		setBack();
		
		is_menu_enable = true;
		
		touched_number = 0;
		
		setTouchEnabled(true);
	}
	
	void setBack()
	{
		back_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 350, 250));
		back_img->setPosition(ccp(240,160));
		addChild(back_img, kDN_Z_back);
		
		CCLabelTTF* noti_label = CCLabelTTF::create("내구도 1 입니다.\n실패시 카드가 사라집니다.\n그래도 플레이 하시겠습니까?", mySGD->getFont().c_str(), 20, CCSizeMake(300,200), kCCTextAlignmentCenter,	kCCVerticalTextAlignmentCenter);
		noti_label->setColor(ccBLACK);
		noti_label->setAnchorPoint(ccp(0.5,0.5));
		noti_label->setPosition(ccp(240,200));
		addChild(noti_label, kDN_Z_content);
		
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
		
		CCMenuItem* ok_item = CCMenuItemSprite::create(n_ok, s_ok, target_ok, delegate_ok);
		ok_item->setTag(2);
		ok_menu = CCMenu::createWithItem(ok_item);
		ok_menu->setPosition(ccp(310, 90));
		addChild(ok_menu, kDN_Z_content);
		
		
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
		
		CCMenuItem* cancel_item = CCMenuItemSprite::create(n_cancel, s_cancel, target_cancel, delegate_cancel);
		cancel_item->setTag(3);
		cancel_menu = CCMenu::createWithItem(cancel_item);
		cancel_menu->setPosition(ccp(180, 90));
		addChild(cancel_menu, kDN_Z_content);
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		
		
		return return_value;
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number != 0)		return true;
		if(cancel_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kDN_MT_cancel;
		else if(ok_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kDN_MT_ok;
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kDN_MT_cancel)							cancel_menu->ccTouchMoved(pTouch, pEvent);
		else if(touched_number == kDN_MT_ok)						ok_menu->ccTouchMoved(pTouch, pEvent);
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kDN_MT_cancel){			cancel_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		else if(touched_number == kDN_MT_ok){			ok_menu->ccTouchEnded(pTouch, pEvent);			touched_number = 0;	}
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		if(touched_number == kDN_MT_cancel){			cancel_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		else if(touched_number == kDN_MT_ok){			ok_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
};

#endif /* defined(__DGproto__DurabilityNoti__) */
