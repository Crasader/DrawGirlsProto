//
//  TutorialScene.h
//  DGproto
//
//  Created by 사원3 on 13. 10. 30..
//
//

#ifndef __DGproto__TutorialScene__
#define __DGproto__TutorialScene__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

enum TutorialState{
	kTutorialState_list = 0,
	kTutorialState_main1_1,
	kTutorialState_main1_2,
	kTutorialState_main1_3,
	kTutorialState_main2_1,
	kTutorialState_main2_2,
	kTutorialState_startsetting_1,
	kTutorialState_startsetting_2,
	kTutorialState_diary_1,
	kTutorialState_diary_2,
	kTutorialState_diary_3,
	kTutorialState_gameprocess_1,
	kTutorialState_gameprocess_2,
	kTutorialState_gameprocess_3,
	kTutorialState_control_1,
	kTutorialState_control_2
};

class TutorialScene : public CCLayer
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(TutorialScene);
	
private:
	int loading_cnt;
	
	TutorialState recent_tutorial_state;
	
	bool is_menu_enable;
	
	CCNode* tutorial_container;
	CCLabelTTF* loading_label;
	CCLabelTTF* title_label;
	
	CCMenu* next_menu;
	
	void loadedImg(CCObject* sender);
	void menuAction(CCObject* sender);
	CCPoint getContentPosition(int t_tag);
	string getTitle(TutorialState t_state);
	string getFilename(TutorialState t_state);
	TutorialState getNextState(TutorialState t_state);
	void setTutorialContainer(TutorialState t_state);
};

#endif /* defined(__DGproto__TutorialScene__) */
