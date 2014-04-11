//
//  RankNewPopup.h
//  DGproto
//
//  Created by 사원3 on 2014. 3. 27..
//
//

#ifndef __DGproto__RankNewPopup__
#define __DGproto__RankNewPopup__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "hspConnector.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class RankNewPopup : public CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    // a selector callback
    void menuAction(CCObject* pSender);
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(RankNewPopup);
	
	virtual void onEnter();
	
	void setHideFinalAction(CCObject* t_final, SEL_CallFunc d_final);
	
	virtual ~RankNewPopup();
	
private:
	
	CCObject* target_final;
	SEL_CallFunc delegate_final;
	
	CCScale9Sprite* main_case;
	CCSprite* gray;
	
	void showPopup();
	void endShowPopup();
	void hidePopup();
	void endHidePopup();
	
	bool is_menu_enable;
	
	virtual bool ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent);
	virtual void ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent);
	virtual void registerWithTouchDispatcher ();
	
	CCSprite* loading_img;
	void resultGetRank(Json::Value result_data);
};


#endif /* defined(__DGproto__RankNewPopup__) */
