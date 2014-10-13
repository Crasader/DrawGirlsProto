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
//class EffectSprite;
class GraySprite;
class CCMenuLambda;
class CardViewScene : public CCLayer, public CCBAnimationManagerDelegate
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene(function<void()> t_end_func);
    CREATE_FUNC(CardViewScene);
	
	virtual void onEnterTransitionDidFinish();
	
	function<void()> temp_end_func;
	
	virtual void completedAnimationSequenceNamed(const char *name);
	
private:
	CCNode* game_node;
	
	CCBAnimationManager* t_manager;
	
	int ing_animation_frame;
	
	//	bool is_touched_menu;
	bool is_actioned;
	
	MyNode* first_img;
	CCBAnimationManager* ccb_manager;
	CCSprite* t_ccb_img;
	CCSprite* sound_img;
	
	CommonButton* next_button;
//	EffectSprite* safety_img;
	int touch_mode;
	bool is_scrolling;
	bool is_before_scrolling;
	
	bool is_spin_mode;
	CommonButton* mode_button;
	
	CCPoint save_position;
	void moveChecking();
	
	void nextScene();
	
	bool is_morphing;
	CCSprite* zoom_img;
	
	CCSprite* morphing_img;
	CommonButton* buy_morphing;
	
	float minimum_scale;
	CCSize screen_size;
	CCPoint old_center_pos;
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
	unsigned long long touchStartTime;
	unsigned long long first_touch_time;
	int first_touch_p;
	CCPoint first_touch_point;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	CCSprite* card_back;
	float zoom_base_distance;
	float zoom_base_radian;
	
	map<int, CCPoint> multiTouchData;
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
	virtual void keyBackClicked();
	
};

#endif /* defined(__DGproto__CardViewScene__) */
