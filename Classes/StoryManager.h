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
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
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
	bool is_delaying;
	
	void addMent(bool is_left, string t_name, string t_namefile, string t_ment, function<void(void)> t_end_func);
	
private:
	
	bool is_menting;
	int touch_count;
	bool is_boosting;
	function<void(void)> end_func;
	string recent_ment;
	CCLabelTTF* ment_label;
	
	int m_touch_priority;
	CCNode* ment_node;
	
	int ing_ment_cnt;
	int ment_recent_length;
	void startMent()
	{
		ing_ment_cnt = 0;
		is_menting = true;
		ment_recent_length = 0;
		schedule(schedule_selector(StoryManager::mentAction));
	}
	
	void mentAction();
	
	void myInit(int t_touch_priority)
	{
		touch_count = 0;
		is_boosting = false;
		is_delaying = false;
		is_menting = false;
		end_func = NULL;
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
