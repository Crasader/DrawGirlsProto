//
//  ZoomScriptScene.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 6..
//
//

#ifndef __DGproto__ZoomScriptScene__
#define __DGproto__ZoomScriptScene__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "DataStorageHub.h"
#include "SilhouetteData.h"
#include <map>
#include "BackKeyBase.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class CommonButton;
class MyNode;
class EffectSprite;
class KSLabelTTF;
class ZoomScript : public CCLayer, public CCBAnimationManagerDelegate//, public BackKeyBase
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(ZoomScript);
	
	virtual void onEnterTransitionDidFinish();
	
	virtual ~ZoomScript()
	{
		CCLOG("~ZoomScript");
	}
	
	virtual void completedAnimationSequenceNamed(const char *name);
	
private:
	CCBAnimationManager* t_manager;
	
	CCNode* game_node;
	int silType;
	bool is_showtime;
//	bool is_exchanged;
	
	bool is_ccb_end;
	bool is_next_on;
	
	int text_length;
	string save_text;
	int typing_frame;
	
	int ing_animation_frame;
	
//	bool is_touched_menu;
	bool is_actioned;
	
	CCBAnimationManager* ccb_manager;
	
	MyNode* first_img;
	CCLabelTTF* script_label;
	CCScale9Sprite* script_case;
	
	CommonButton* next_button;
	
	int touch_mode;
	KSLabelTTF* showtime_back;
	MyNode* second_img;
//	KSLabelTTF* showtime_morphing_label;
	
	int typing_sound_number;
	
	MyNode* third_img;
	
	EffectSprite* safety_img;
	int target_code;
	MyNode* target_node;
	CCSprite* white_paper;
	
	bool is_scrolling;
	bool is_before_scrolling;
	
	bool is_spin_mode;
	CommonButton* mode_button;
	
	CCPoint save_position;
	void moveChecking();
	
	void nextScene();
	
	bool is_morphing;
	CCSprite* zoom_img;
	
	
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
	
	void showtimeFifthAction();
	void showtimeSixthAction();
	void showtimeSeventhAction();
	
	bool is_rankup;
	void rankupAction();
	
	bool is_time_event_card_on;
	
	bool isAnimated;
	CCPoint touch_p;
    unsigned long long touchStartTime;
	unsigned long long first_touch_time;
	int first_touch_p;
	CCPoint first_touch_point;
	CCPoint touchStart_p;
	CCPoint moveSpeed_p;
	float zoom_base_distance;
	float zoom_base_radian;
	CCSprite* card_back;
	
	map<int, CCPoint> multiTouchData;
	
	CCSize eye_ani_size;
	int loop_length;
	deque<int> animation_frame;
	
	void startStageAnimation();
	
	void frameAnimation();
	
	virtual void keyBackClicked(void);
	
	void moveListXY(CCPoint t_p);
	void moveAnimation();
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
	virtual void registerWithTouchDispatcher(void);
	
	void alertAction(int t1, int t2);
	
};

#endif /* defined(__DGproto__ZoomScriptScene__) */
