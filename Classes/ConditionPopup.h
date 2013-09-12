//
//  ConditionPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 12..
//
//

#ifndef __DGproto__ConditionPopup__
#define __DGproto__ConditionPopup__

#include "cocos2d.h"
#include "StarGoldData.h"
#include "SilhouetteData.h"

USING_NS_CC;

enum CDT_Zorder{
	kCDT_Z_back = 1,
	kCDT_Z_content
};

class ConditionPopup : public CCLayer
{
public:
	static ConditionPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		ConditionPopup* t_cdt = new ConditionPopup();
		t_cdt->myInit(t_close, d_close);
		t_cdt->autorelease();
		return t_cdt;
	}
	
private:
	bool is_touch_enable;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close)
	{
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* cdt_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 320, 240));
		cdt_back->setPosition(ccp(240,160+DataStorageHub::sharedInstance()->ui_height_center_control));
		addChild(cdt_back, kCDT_Z_back);
		
		CCLabelTTF* title = CCLabelTTF::create(SilhouetteData::sharedSilhouetteData()->getConditionTitle().c_str(), StarGoldData::sharedInstance()->getFont().c_str(), 20);
		title->setColor(ccBLACK);
		title->setPosition(ccp(240,200+DataStorageHub::sharedInstance()->ui_height_center_control));
		addChild(title, kCDT_Z_content);
		
		CCLabelTTF* content = CCLabelTTF::create(SilhouetteData::sharedSilhouetteData()->getConditionContent().c_str(), StarGoldData::sharedInstance()->getFont().c_str(), 10, CCSizeMake(240, 160), kCCTextAlignmentCenter);
		content->setColor(ccBLACK);
		content->setPosition(ccp(240,60+DataStorageHub::sharedInstance()->ui_height_center_control));
		addChild(content, kCDT_Z_content);
		
		is_touch_enable = true;
		
		setTouchEnabled(true);
		
		StarGoldData::sharedInstance()->is_paused = true;
		CCDirector::sharedDirector()->pause();
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		is_touch_enable = false;
		setTouchEnabled(false);
		
		(target_close->*delegate_close)();
		
		StarGoldData::sharedInstance()->is_paused = false;
		CCDirector::sharedDirector()->resume();
		removeFromParent();
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
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -150, true);
	}
};

#endif /* defined(__DGproto__ConditionPopup__) */
