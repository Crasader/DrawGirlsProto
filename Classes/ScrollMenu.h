#ifndef _ScrollMenu_H_
#define _ScrollMenu_H_

#include <cocos2d.h>

using namespace cocos2d;

class ScrollMenu:public CCMenu
{
public:
	ScrollMenu();
	virtual ~ScrollMenu();
	
	CREATE_FUNC(ScrollMenu);
	
	static ScrollMenu* create(CCMenuItem* item,...);
	static ScrollMenu* createWithArray(CCArray* pArrayOfItems);
	static ScrollMenu* createWithItems(CCMenuItem* item, va_list args);
	
	bool isTouching;
	CCPoint m_touchesStart;
	CCPoint m_touchesEnd;
	int touch_priority;
	
	virtual void setTouchPriority(int t_touch_priority);
	
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
	virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
};

#endif // _ScrollMenu_H_