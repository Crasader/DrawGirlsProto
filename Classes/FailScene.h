//
//  FailScene.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#ifndef __galsprototype__FailScene__
#define __galsprototype__FailScene__

#include "cocos2d.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "hspConnector.h"
#include "HeartTime.h"

USING_NS_CC;

class FailScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(FailScene);
	
	virtual void onEnter();
	
private:
	
	bool is_menu_enable;
	bool is_saved_user_data;
	
	CCMenu* main_menu;
	CCMenu* replay_menu;
//	CCMenu* help_menu;
	
	CCLabelBMFont* score_label;
	CCLabelBMFont* gold_label;
	CCLabelBMFont* time_label;
	
	HeartTime* heart_time;
	
	void closeReplayPopup();
	void popupClose();
	
	
	float keep_gold;
	float decrease_gold;
	float increase_gold;
	
	float keep_score;
	float decrease_score;
	float increase_score;
	
	float keep_time;
	float decrease_time;
	float increase_time;
	
	void startCalcAnimation();
	void startScoreAnimation();
	void scoreAnimation(float dt);
	void stopScoreAnimation();
	void startGoldAnimation();
	void goldAnimation(float dt);
	void stopGoldAnimation();
	void startTimeAnimation();
	void timeAnimation(float dt);
	void stopTimeAnimation();
	
	CCPoint getContentPosition(int t_tag);
	
	void resultSavedUserData(Json::Value result_data);
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
};

#endif /* defined(__galsprototype__FailScene__) */
