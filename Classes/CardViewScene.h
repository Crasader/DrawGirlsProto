//
//  CardViewScene.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#ifndef __DGproto__CardViewScene__
#define __DGproto__CardViewScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include <map>
#include <deque>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CommonButton;
class MyNode;
class EffectSprite;
class CardViewScene : public CCLayer
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(CardViewScene);
	
	virtual void onEnterTransitionDidFinish();
	
private:
	CCNode* game_node;
	
	int ing_animation_frame;
	
	//	bool is_touched_menu;
	bool is_actioned;
	
	MyNode* first_img;
	
	CommonButton* next_button;
	EffectSprite* safety_img;
	
	bool is_scrolling;
	bool is_before_scrolling;
	
	bool is_spin_mode;
	CommonButton* mode_button;
	
	CCPoint save_position;
	void moveChecking();
	
	void nextScene();
	
	CCSprite* zoom_img;
	
	
	float minimum_scale;
	CCSize screen_size;
	
	void menuAction(CCObject* sender);
	
	void startScript();
	
	SEL_CallFunc delegate_typing_after;
	void typingAnimation();
	
	void startTouchAction();
	
	void showtimeFirstAction();
	void showtimeSecondAction();
	void showtimeThirdAction();
	void showtimeForthAction();
	
	bool is_rankup;
	void rankupAction();
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
	long first_touch_time;
	int first_touch_p;
	CCPoint first_touch_point;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	
	map<int, CCPoint> multiTouchData;
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
	
};

#endif /* defined(__DGproto__CardViewScene__) */
