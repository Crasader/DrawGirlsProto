//
//  CardListViewer.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 2..
//
//

#ifndef __DGproto__CardListViewer__
#define __DGproto__CardListViewer__

#include "cocos2d.h"
#include "ListViewerScroll.h"
USING_NS_CC;

using namespace std;

enum CSS_Zorder{
	kCSS_Z_gray = 1,
	kCSS_Z_back,
	kCSS_Z_selectedImg,
	kCSS_Z_content,
	kCSS_Z_select,
	kCSS_Z_check,
	kCSS_Z_alignList,
	kCSS_Z_alignButton,
	kCSS_Z_popup
};

enum CSS_MenuTag{
	kCSS_MT_close = 1,
	kCSS_MT_alignDefault,
	kCSS_MT_alignTake,
	kCSS_MT_alignRank,
	kCSS_MT_eventCard,
	kCSS_MT_diary,
	kCSS_MT_event,
	kCSS_MT_strength,
	kCSS_MT_tip,
	kCSS_MT_character,
	kCSS_MT_cardGacha,
	kCSS_MT_cardBase = 1000000,
	kCSS_MT_cardMenuBase = 2000000,
	kCSS_MT_noCardBase = 3000000
};

class CLV_Node : public CCNode
{
public:
	static CLV_Node* create(int t_card_stage, int t_card_level, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect);
	
	static CLV_Node* create(int t_card_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect);
	
	void viewCheck();
	
	bool isSetted();
	
	int getMyTag();
	
private:
	
	int card_stage;
	int card_level;
	int card_number;
	
	bool is_card_number_setted;
	
	CCObject* target_menu;
	SEL_MenuHandler delegate_menu;
	CCPoint my_position;
	CCRect parent_view_rect;
	CCSize my_size;
	
	bool is_setted;
	
	int my_tag;
	
	void setChild();
	
	void myInit(int t_card_stage, int t_card_level, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect);
	
	void myInit(int t_card_number, CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect);
	
	void dataSet(CCObject* t_menu, SEL_MenuHandler d_menu, CCPoint t_position, CCRect t_rect);
};

class CardListViewer : public CCLayer, public ScrollingObject
{
public:
	static CardListViewer* create();
	
	virtual void visit();
	virtual void setPositionY(float t_y);
	void setPercentage(float t_p);
	void setMaxPositionY();
	void setScroll(ScrollingObject* t_link);
	
	CCRect getViewRect();
	
private:
	
	CCRect view_rect;
	bool is_touching;
	int touched_index;
	CCPoint touch_begin_point;
	CCPoint first_touch_point;
	
	float max_positionY;
	
	float touchy;
	long touchStartTime;
    float touchStartY;
    float moveSpeed;
	bool isAnimated;
	
	ScrollingObject* target_link;
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
	
	void myInit();
	void moveList(float dy);
	void moveAnimation();
};

#endif /* defined(__DGproto__CardListViewer__) */
