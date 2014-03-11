//
//  StoryManager.h
//  DGproto
//
//  Created by 사원3 on 2014. 3. 11..
//
//

#ifndef __DGproto__StoryManager__
#define __DGproto__StoryManager__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class StoryManager : public CCLayer
{
public:
	static StoryManager* create(int t_touch_priority)
	{
		StoryManager* t_sm = new StoryManager();
		t_sm->myInit(t_touch_priority);
		t_sm->autorelease();
		return t_sm;
	}
	
private:
	
	int m_touch_priority;
	
	void myInit(int t_touch_priority)
	{
		m_touch_priority = t_touch_priority;
		
		setTouchEnabled(true);
	}
};

#endif /* defined(__DGproto__StoryManager__) */
