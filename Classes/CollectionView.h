//
//  CollectionView.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 20..
//
//

#ifndef __DGproto__CollectionView__
#define __DGproto__CollectionView__

#include "cocos2d.h"

USING_NS_CC;

class CollectionInnerLine : public CCNode
{
public:
	static CollectionInnerLine* create(int t_line_number);
	virtual void visit();

	bool is_touch_enable;
	bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
	{
		touched_index = -1;
		int loop_cnt = menus_array->count();
		for(int i=0;i<loop_cnt;i++)
		{
			CCMenu* t_m = (CCMenu*)menus_array->objectAtIndex(i);
			if(t_m->ccTouchBegan(pTouch, pEvent))
			{
				touched_index = i;
				return true;
			}
		}
		return false;
	}
	void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent)
	{
		if(touched_index <= -1)
			return;
		
		CCMenu* t_m = (CCMenu*)menus_array->objectAtIndex(touched_index);
		t_m->ccTouchMoved(pTouch, pEvent);
	}
	void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent)
	{
		if(touched_index <= -1)
			return;
		
		CCMenu* t_m = (CCMenu*)menus_array->objectAtIndex(touched_index);
		t_m->ccTouchEnded(pTouch, pEvent);
	}
	void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent)
	{
		if(touched_index <= -1)
			return;
		
		CCMenu* t_m = (CCMenu*)menus_array->objectAtIndex(touched_index);
		t_m->ccTouchCancelled(pTouch, pEvent);
	}

	virtual ~CollectionInnerLine()
	{
		menus_array->release();
	}

private:
	int line_number;
	float real_position_y;
	CCArray* menus_array;
	int touched_index;


	void menuAction(CCObject* sender);

	void myInit(int t_line_number);

	void createCard(int t_stage, int t_level, int index);
	void createUncover(int index);
};

class CollectionView : public CCLayer
{
public:
	static CollectionView* create();
	
	virtual void visit();
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	
private:
	CCNode* inner_view;
	CCRect view_rect;
	
	int touched_index;
	
	void myInit();
	void setInnerView();
	
	float touchy;
	long touchStartTime;
    float touchStartY;
    float moveSpeed;
	bool isAnimated;
	
	void moveList(float dy);
	
	void moveAnimation();
	void moveAnimationUp();
	void moveAnimationDown();
};

#endif /* defined(__DGproto__CollectionView__) */
