//
//  ClearScene.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#ifndef __galsprototype__ClearScene__
#define __galsprototype__ClearScene__

#include "cocos2d.h"
#include "AlertEngine.h"
#include "MyLocalization.h"
#include "hspConnector.h"

USING_NS_CC;

class ClearScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(ClearScene);
	
	virtual void onEnter();
	
private:
	
	bool is_menu_enable;
	bool is_saved_user_data;
	
	CCMenu* ok_menu;
	
	CCLabelBMFont* score_label;
	CCLabelBMFont* percentage_label;
	CCLabelBMFont* time_label;
	
	float keep_percentage;
	float decrease_percentage;
	float increase_percentage;
	void startPercentageAnimation();
	void percentageAnimation(float dt);
	void stopPercentageAnimation();
	
	float keep_score;
	float decrease_score;
	float increase_score;
	void startScoreAnimation();
	void scoreAnimation(float dt);
	void stopScoreAnimation();
	
	float keep_time;
	float decrease_time;
	float increase_time;
	void startTimeAnimation();
	void timeAnimation(float dt);
	void stopTimeAnimation();
	
	void startCalcAnimation();
	
	void realEnd();
	
	void resultSavedUserData(Json::Value result_data);
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
};

#endif /* defined(__galsprototype__ClearScene__) */
