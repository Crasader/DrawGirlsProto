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
	static PLV_Node* create(int t_puzzle_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect);
	
	void viewCheck();
	
	bool isSetted();
	
	int getPuzzleNumber();
	
	void touchCancel();
	
private:
	
	int puzzle_number;
	CCObject* target_menu;
	SEL_MenuHandler delegate_menu;
	CCPoint my_position;
	CCRect parent_view_rect;
	CCSize my_size;
	
	bool is_setted;
	
	void setChild();
	
	void myInit(int t_puzzle_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect);
};

class PuzzleListView : public CCLayer
{
public:
	static PuzzleListView* create();
	
	virtual void visit();
	virtual void setPositionX(float t_x);
	void setPercentage(float t_p);
	void setMinPositionX();
	void touchCancel();
	
	void startViewCheck();
	
	void stopViewCheck();
	
	CCRect getViewRect();
	
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
	
	void viewChecking();
};

#endif /* defined(__DGproto__PuzzleListView__) */
