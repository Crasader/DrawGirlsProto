//
//  CardGachaPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 12. 9..
//
//

#ifndef __DGproto__CardGachaPopup__
#define __DGproto__CardGachaPopup__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CardGachaPopup : public CCLayer
{
public:
	static CardGachaPopup* create(int t_touch_priority);
	void setHideFinalAction(CCObject* t_target, SEL_CallFunc t_delegate);
	
private:
	int touch_priority;
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	bool is_menu_enable;
	CCSprite* gray;
	CCSprite* main_case;
	
	void myInit(int t_touch_priority);
};

#endif /* defined(__DGproto__CardGachaPopup__) */
