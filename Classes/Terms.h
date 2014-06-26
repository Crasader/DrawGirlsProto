#ifndef TERMS_H
#define TERMS_H

#include "cocos2d.h"
USING_NS_CC;


class Terms : public CCLayer
{
public:
	Terms();
	virtual ~Terms();
	//	bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual bool init(int touchP, std::function<void(void)> finalFunctor);
	static Terms* create(int touchP, std::function<void(void)> finalFunctor)
	{
		Terms* t = new Terms();
		t->init(touchP, finalFunctor);
		t->autorelease();
		return t;
	}
	//virtual void registerWithTouchDispatcher();
protected:
	bool m_leftChecked;
	CCSprite* m_leftCheckSprite;
	bool m_rightChecked;
	CCSprite* m_rightCheckSprite;
};

#endif
