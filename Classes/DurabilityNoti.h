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
#include "StageSettingPopup.h"

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
	static DurabilityNoti* create(CCObject* t_cancel, SEL_MenuHandler d_cancel, CCObject* t_ok, SEL_MenuHandler d_ok);
	
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
	
	void myInit(CCObject* t_cancel, SEL_MenuHandler d_cancel, CCObject* t_ok, SEL_MenuHandler d_ok);
	
	void setBack();
	
	CCPoint getContentPosition(int t_tag);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__DurabilityNoti__) */
