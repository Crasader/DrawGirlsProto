//
//  DiaryZoom.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 2..
//
//

#ifndef __DGproto__DiaryZoom__
#define __DGproto__DiaryZoom__

#include "cocos2d.h"
#include "DataStorageHub.h"
#include <deque>

USING_NS_CC;
using namespace std;

class DiaryZoom : public CCLayer
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(DiaryZoom);
	
	virtual void onEnterTransitionDidFinish();
	
private:
	CCNode* game_node;
	
	int ing_animation_frame;
	
	bool is_touched_menu;
	bool is_actioned;
	
	CCSprite* first_img;
	CCMenu* next_button;
	
	bool is_animation;
	
	float minimum_scale;
	CCSize screen_size;
	
	void menuAction(CCObject* sender);
	
	void startTouchAction();
	
	bool isAnimated;
	CCPoint touch_p;
    long touchStartTime;
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

#endif /* defined(__DGproto__DiaryZoom__) */
