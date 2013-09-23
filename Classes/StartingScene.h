//
//  StartingScene.h
//  galsprototype
//
//  Created by 사원3 on 13. 8. 23..
//
//

#ifndef __galsprototype__StartingScene__
#define __galsprototype__StartingScene__

#include "cocos2d.h"

USING_NS_CC;

class StartingScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(StartingScene);
	
private:
	CCSprite* recent_left_img;
	CCSprite* recent_right_img;
	CCSprite* after_left_img;
	CCSprite* after_right_img;
	
	CCSprite* covered_left_img;
	CCSprite* covered_right_img;
	
	CCSprite* animation_img;
	SEL_CallFunc end_animation_delegate;
	
	CCPoint begin_point;
	
	int touch_end_direction;
	
	int touch_direction;
	
	bool is_touch_enable;
	
	float animation_angle;
	
	void startNextPage();
	void startPrePage();
	
	void ingPage();
	
	void endNextPage();
	void endPrePage();
	
	void endPage();
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void ccTouchCancelled(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	virtual void registerWithTouchDispatcher();
};

#endif /* defined(__galsprototype__StartingScene__) */
