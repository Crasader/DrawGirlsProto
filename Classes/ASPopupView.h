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
#include "DataStorageHub.h"

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
	static ASPopupView* createDimmed(int t_touch_priority)
	{
		ASPopupView* t_popup = new ASPopupView();
		t_popup->myInit(t_touch_priority);
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
	
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, 160));
		t_popup->setBasePosition(ccp(240, 160));

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
//	static ASPopupView* getCommonNoti(int t_touch_priority, const char* t_title, const char* t_comment);
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_title, string t_comment);
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_comment, function<void()> close_func, CCPoint t_position, bool Xbutton = false);
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_title, string t_comment, function<void()> close_func, CCPoint t_position = CCPointZero, bool XButton = false);
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_title, CCNode* ment_label, function<void()> close_func, float titleFontSize = 12.f, CCPoint t_position = CCPointZero, bool Xbutton = false);
	static ASPopupView* getCommonNoti2(int t_touch_priority, string t_title, CCNode* ment_label, function<void()> close_func, CCPoint t_position = CCPointZero, bool Xbutton = false);
	static ASPopupView* getNotEnoughtGoodsGoShopPopup(int t_touch_priority, GoodsType t_type, function<void()> goshop_func, function<void()> cancel_func = nullptr);
	
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_title, string t_comment,
																					function<void()> close_func,
																		float titleFontSize, CCPoint t_position, bool Xbutton, bool towButton,function<void()> close_func2);
	
	static ASPopupView* getCommonNoti(int t_touch_priority, string t_title, CCNode* ment_label,
																		function<void()> close_func,
																		float titleFontSize, CCPoint t_position, bool Xbutton, bool towButton,function<void()> close_func2);
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
