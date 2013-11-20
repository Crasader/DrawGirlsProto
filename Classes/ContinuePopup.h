//
//  ContinuePopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 10..
//
//

#ifndef __DGproto__ContinuePopup__
#define __DGproto__ContinuePopup__

#include "cocos2d.h"
#include "StarGoldData.h"
#include "LogData.h"

USING_NS_CC;

enum CPL_MenuTag{
	kCPL_MT_end = 1,
	kCPL_MT_continue
};

enum CPL_Zorder{
	kCPL_Z_gray = 1,
	kCPL_Z_back,
	kCPL_Z_menu,
	kCPL_Z_effect
};

class ContinuePopup : public CCLayer
{
public:
	static ContinuePopup* create(CCObject* t_end, SEL_CallFunc d_end, CCObject* t_continue, SEL_CallFunc d_continue);
	
private:
	bool is_menu_enable;
	
	CCObject* target_end;
	SEL_CallFunc delegate_end;
	CCObject* target_continue;
	SEL_CallFunc delegate_continue;
	
	int touched_number;
	CCMenu* end_menu;
	CCMenu* continue_menu;
	
	void myInit(CCObject* t_end, SEL_CallFunc d_end, CCObject* t_continue, SEL_CallFunc d_continue);
	
	void menuAction(CCObject* sender);
	
	void endContinueEffect();
	
	void removeAction();
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__ContinuePopup__) */
