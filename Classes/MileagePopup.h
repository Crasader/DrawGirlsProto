//
//  MileagePopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 29..
//
//

#ifndef __DGproto__MileagePopup__
#define __DGproto__MileagePopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class MileagePopup : public CCLayer//, public CCBAnimationManagerDelegate
{
public:
	static MileagePopup* create(int t_touch_priority, function<void()> t_end_func, int t_count);
	
	//	virtual void completedAnimationSequenceNamed (char const * name);
	
private:
	int touch_priority;
	function<void()> end_func;

	int mileage_count;
	
	bool is_menu_enable;
	
	CCNode* m_container;
	CCScale9Sprite* back_case;
	
	void myInit(int t_touch_priority, function<void()> t_end_func, int t_count);
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__MileagePopup__) */
