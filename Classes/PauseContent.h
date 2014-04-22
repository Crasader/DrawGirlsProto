//
//  PauseContent.h
//  DGproto
//
//  Created by 사원3 on 2014. 2. 3..
//
//

#ifndef __DGproto__PauseContent__
#define __DGproto__PauseContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum PauseContentMenuTag{
	kPauseContentMenuTag_resume = 0,
	kPauseContentMenuTag_gohome,
	kPauseContentMenuTag_replay
};
class PauseContent : public CCNode
{
public:
	static PauseContent* create(int t_touch_priority, function<void(void)> t_resume, function<void(void)> t_gohome, function<void(void)> t_replay);
	
	void startShow();
	
private:
	int touch_priority;
	function<void(void)> resume_selector;
	function<void(void)> gohome_selector;
	function<void(void)> replay_selector;
	
	bool is_menu_enable;
	
	void menuAction(CCObject* sender);
	void resumeAction(CCObject* sender, CCControlEvent t_event);
	void gohomeAction(CCObject* sender, CCControlEvent t_event);
	void replayAction(CCObject* sender, CCControlEvent t_event);
	
	CCMenuItemLambda* on_item;
	CCMenuItemLambda* off_item;
	
	void endShow()
	{
		is_menu_enable = true;
	}
	void startHide();
	void endHide()
	{
		resume_selector();
	}
	
	void myInit(int t_touch_priority, function<void(void)> t_resume, function<void(void)> t_gohome, function<void(void)> t_replay);
};

#endif /* defined(__DGproto__PauseContent__) */
