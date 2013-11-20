//
//  ChallengePopup.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 1..
//
//

#ifndef __DGproto__ChallengePopup__
#define __DGproto__ChallengePopup__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum ChallengePopupZorder{
	kCP_Z_gray = 1,
	kCP_Z_back,
	kCP_Z_content
};

enum ChallengePopupMenuTag{
	kCP_MT_return = 1
};

class ChallengePopup : public CCLayer
{
public:
	static ChallengePopup* create(CCObject* t_return, SEL_CallFunc d_return);
	
private:
	bool is_menu_enable;
	
	int touched_number;
	CCMenu* return_menu;
	
	CCObject* target_return;
	SEL_CallFunc delegate_return;
	
	void myInit(CCObject* t_return, SEL_CallFunc d_return);
	
	CCPoint getContentPosition(int t_tag);
	
	void menuAction(CCObject* sender);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__ChallengePopup__) */
