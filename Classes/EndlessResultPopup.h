#ifndef ENDLESSRESULTPOPUP_H
#define ENDLESSRESULTPOPUP_H

#include "cocos2d.h"
USING_NS_CC;

#include "cocos-ext.h"
USING_NS_CC_EXT;
enum EndlessResultPopupZorder{
	kEndlessResultPopupZorder_back = 1,
	kEndlessResultPopupZorder_main,
	kEndlessResultPopupZorder_top,
	kEndlessResultPopupZorder_popup
};
class EndlessResultPopup : public CCLayer
{
public:
	EndlessResultPopup();
	virtual ~EndlessResultPopup();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init();
	static EndlessResultPopup* create()
	{
		EndlessResultPopup* t = new EndlessResultPopup();
		t->init();
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
protected:
	CCSprite* gray;
	CCScale9Sprite* main_case;
};

#endif

