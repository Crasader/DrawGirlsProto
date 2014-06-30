#ifndef ACCOUNTMANAGERPOPUP_H
#define ACCOUNTMANAGERPOPUP_H

#include "cocos2d.h"
USING_NS_CC;


class AccountManagerPopup : public CCLayer
{
public:
	AccountManagerPopup();
	virtual ~AccountManagerPopup();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(int touchP);
	static AccountManagerPopup* create(int touchP)
	{
		AccountManagerPopup* t = new AccountManagerPopup();
		t->init(touchP);
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
};

#endif
