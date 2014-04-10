//
//  DiaryZoomPopup.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 20..
//
//

#ifndef __DGproto__DiaryZoomPopup__
#define __DGproto__DiaryZoomPopup__

#include "cocos2d.h"
#include <deque>

USING_NS_CC;
using namespace std;
class CommonButton;
class MyNode;
class DiaryZoomPopup : public CCLayer
{
public:
	virtual bool init();
    CREATE_FUNC(DiaryZoomPopup);
	
	virtual void onEnterTransitionDidFinish();
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	bool is_before_no_diary;
	
private:
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCSprite* gray;
	
	CCNode* game_node;
	
	int ing_animation_frame;
	
	bool is_actioned;
	
	MyNode* first_img;
	CommonButton* next_button;
	bool is_animation;
	
	bool is_scrolling;
	bool is_before_scrolling;
	
	bool is_spin_mode;
	CommonButton* mode_button;
	
	CCPoint before_sub_position;
	CCPoint save_position;
	void moveChecking();
	
	float minimum_scale;
	CCSize screen_size;
	
	void showPopup();
	void endShowPopup();
	
	void hidePopup();
	void endHidePopup();
	
	void menuAction(CCObject* sender);
	
	void startTouchAction();
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
	long first_touch_time;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	
	map<int, CCPoint> multiTouchData;
	
	CCSize eye_ani_size;
	int loop_length;
	deque<int> animation_frame;
	
	void startStageAnimation();
	
	void frameAnimation();
	
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
	
};

#endif /* defined(__DGproto__DiaryZoomPopup__) */
