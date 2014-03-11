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
	
	CCNode* back_node;
	CCNode* left_node;
	CCNode* right_node;
	CCNode* front_node;
	
private:
	
	int m_touch_priority;
	CCNode* ment_node;
	
	void myInit(int t_touch_priority)
	{
		m_touch_priority = t_touch_priority;
		
		setTouchEnabled(true);
		
		back_node = CCNode::create();
		back_node->setPosition(ccp(240,160));
		addChild(back_node, 1);
		
		left_node = CCNode::create();
		left_node->setPosition(ccp(0,0));
		addChild(left_node, 2);
		
		right_node = CCNode::create();
		right_node->setPosition(ccp(480,0));
		addChild(right_node, 2);
		
		front_node = CCNode::create();
		front_node->setPosition(ccp(240,160));
		addChild(front_node, 3);
		
		ment_node = CCNode::create();
		ment_node->setPosition(ccp(240,160));
		addChild(ment_node, 4);
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__DGproto__StoryManager__) */
