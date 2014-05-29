//
//  ASPopupView.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 26..
//
//

#ifndef __DGproto__ASPopupView__
#define __DGproto__ASPopupView__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "StarGoldData.h"
#include "KSLabelTTF.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ASPopupView : public CCLayer
{
public:
	static ASPopupView* create(int t_touch_priority)
	{
		ASPopupView* t_popup = new ASPopupView();
		t_popup->myInit(t_touch_priority);
		t_popup->autorelease();
		return t_popup;
	}
	
	void setDimmedPosition(CCPoint t_point)
	{
		dimmed_sprite->setPosition(t_point);
	}
	
	void setDimmedSize(CCSize t_size)
	{
		dimmed_sprite->setScaleX(t_size.width/480.f);
		dimmed_sprite->setScaleY(t_size.height/320.f);
	}
	
	void setBasePosition(CCPoint t_point)
	{
		base_position = t_point;
		if(m_container)
			m_container->setPosition(t_point);
	}
	
	void setContainerNode(CCNode* t_container)
	{
		if(m_container)
			removeChild(m_container);
		
		m_container = t_container;
		m_container->setPosition(base_position);
		addChild(m_container);
	}
	
	CCSprite* getDimmedSprite()
	{
		return dimmed_sprite;
	}
	
	virtual int getTouchPriority()
	{
		return touch_priority;
	}
	
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_comment);
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_comment, function<void()> close_func, CCPoint t_position = CCPointZero, bool Xbutton = false);
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_title, string t_comment, function<void()> close_func, CCPoint t_position = CCPointZero, bool XButton = false);
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_title, CCNode* ment_label, function<void()> close_func, CCPoint t_position = CCPointZero, bool Xbutton = false);
	static ASPopupView* getNotEnoughtGoodsGoShopPopup(int t_touch_priority, GoodsType t_type, function<void()> goshop_func, function<void()> cancel_func = nullptr);

	bool is_menu_enable;
	vector<function<void()>> button_func_list;
	
	void buttonAction(CCObject* sender, CCControlEvent t_event);
	
protected:
	CCSprite* dimmed_sprite;
	CCPoint base_position;
	CCNode* m_container;
	int touch_priority;
	
	void myInit(int t_touch_priority)
	{
		is_menu_enable = false;
		touch_priority = t_touch_priority;
		setTouchEnabled(true);
		
		setZOrder(INT32_MAX);
		
		m_container = NULL;
		
		base_position = ccp(240,160);
		
		dimmed_sprite = CCSprite::create("back_gray.png");
		dimmed_sprite->setPosition(base_position);
		addChild(dimmed_sprite);
	}
	
	virtual void registerWithTouchDispatcher ()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, touch_priority, true);
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		CCLOG("touch swallow popup");
		return true;
	}
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		
	}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		
	}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		
	}
};

#endif /* defined(__DGproto__ASPopupView__) */
