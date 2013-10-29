//
//  PuzzleListView.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 29..
//
//

#ifndef __DGproto__PuzzleListView__
#define __DGproto__PuzzleListView__

#include "cocos2d.h"
#include "ListViewerScroll.h"
#include "StageImgLoader.h"
#include "DataStorageHub.h"
#include "SilhouetteData.h"
#include "StarGoldData.h"

USING_NS_CC;
using namespace std;

class PLV_Node : public CCNode
{
public:
	static PLV_Node* create(int t_puzzle_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect)
	{
		PLV_Node* t_n = new PLV_Node();
		t_n->myInit(t_puzzle_number, t_menu, d_menu, t_position, t_rect);
		t_n->autorelease();
		return t_n;
	}
	
	void viewCheck()
	{
		CCPoint parent_position = getParent()->getPosition();
		CCRect tt_rect = CCRectMake(parent_position.x+my_position.x-my_size.width/2.f, parent_position.y+my_position.y-my_size.height/2.f, my_size.width, my_size.height);
		if(parent_view_rect.intersectsRect(tt_rect))
		{
			if(!is_setted)
				setChild();
		}
		else
		{
			is_setted = false;
			removeAllChildren();
		}
	}
	
	bool isSetted(){	return is_setted;	}
	
	int getPuzzleNumber(){		return puzzle_number;	}
	
	void touchCancel()
	{
		((CCMenu*)getChildByTag(puzzle_number))->ccTouchCancelled(NULL, NULL);
	}
	
private:
	
	int puzzle_number;
	CCObject* target_menu;
	SEL_MenuHandler delegate_menu;
	CCPoint my_position;
	CCRect parent_view_rect;
	CCSize my_size;
	
	bool is_setted;
	
	void setChild()
	{
		int puzzle_count = NSDS_GI(kSDS_GI_puzzleListCount_i);
		int found_index = 0;
		for(int i=1;i<=puzzle_count && found_index == 0;i++)
		{
			int puzzle_no = NSDS_GI(kSDS_GI_puzzleList_int1_no_i, i);
			if(puzzle_no == puzzle_number)
				found_index = i;
		}
		
		CCSprite* n_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", found_index)->getCString());
//		CCLabelTTF* n_number_label = CCLabelTTF::create(CCString::createWithFormat("%d", puzzle_number)->getCString(), mySGD->getFont().c_str(), 30);
//		n_number_label->setColor(ccBLACK);
//		n_number_label->setPosition(ccp(74,65));
//		n_back->addChild(n_number_label);
		
		CCSprite* s_back = mySIL->getLoadedImg(CCString::createWithFormat("puzzleList%d_thumbnail.png", found_index)->getCString());
		s_back->setColor(ccGRAY);
//		CCLabelTTF* s_number_label = CCLabelTTF::create(CCString::createWithFormat("%d", puzzle_number)->getCString(), mySGD->getFont().c_str(), 30);
//		s_number_label->setColor(ccBLACK);
//		s_number_label->setPosition(ccp(74,65));
//		s_back->addChild(s_number_label);
		
		CCMenuItem* back_item = CCMenuItemSprite::create(n_back, s_back, target_menu, delegate_menu);
		back_item->setTag(puzzle_number);
		
		CCMenu* back_menu = CCMenu::createWithItem(back_item);
		back_menu->setPosition(CCPointZero);
		back_menu->setTouchEnabled(false);
		addChild(back_menu, 0, puzzle_number);
		
		is_setted = true;;
	}
	
	void myInit(int t_puzzle_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect)
	{
		puzzle_number = t_puzzle_number;
		target_menu = t_menu;
		delegate_menu = d_menu;
		my_position = t_position;
		parent_view_rect = t_rect;
		
		setPosition(my_position);
		
		my_size = CCSizeMake(135, 115);
		is_setted = false;
		
		CCRect tt_rect = CCRectMake(my_position.x-my_size.width/2.f, my_position.y-my_size.height/2.f, my_size.width, my_size.height);
		if(parent_view_rect.intersectsRect(tt_rect))
		{
			setChild();
		}
	}
};

class PuzzleListView : public CCLayer
{
public:
	static PuzzleListView* create()
	{
		PuzzleListView* t_clv = new PuzzleListView();
		t_clv->myInit();
		t_clv->autorelease();
		return t_clv;
	}
	
	virtual void visit();
	virtual void setPositionX(float t_x);
	void setPercentage(float t_p)
	{
		setPosition(ccp(min_positionX*t_p,getPositionY()));
		for(int i=0;i<getChildrenCount();i++)
		{
			CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
			((PLV_Node*)t_child)->viewCheck();
		}
	}
	void setMinPositionX();
	void touchCancel()
	{
		if(touched_index != 0)
		{
			CCNode* t_child = (CCNode*)getChildByTag(touched_index);
			((PLV_Node*)t_child)->touchCancel();
		}
	}
	
	void startViewCheck()
	{
		schedule(schedule_selector(PuzzleListView::viewChecking));
	}
	
	void stopViewCheck()
	{
		unschedule(schedule_selector(PuzzleListView::viewChecking));
	}
	
	CCRect getViewRect(){	return view_rect;	}
	
private:
	
	CCRect view_rect;
	bool is_touching;
	int touched_index;
	CCPoint touch_begin_point;
	CCPoint first_touch_point;
	
	float min_positionX;
	
	float touchx;
	long touchStartTime;
    float touchStartX;
    float moveSpeed;
	bool isAnimated;
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	
	void myInit();
	void moveList(float dx);
	void moveAnimation();
	
	void viewChecking()
	{
		for(int i=0;i<getChildrenCount();i++)
		{
			CCNode* t_child = (CCNode*)getChildren()->objectAtIndex(i);
			((PLV_Node*)t_child)->viewCheck();
		}
	}
};

#endif /* defined(__DGproto__PuzzleListView__) */
