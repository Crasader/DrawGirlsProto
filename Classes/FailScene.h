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
	
private:
	
	bool is_menu_enable;
	
	void alertAction(int t1, int t2);
	virtual void keyBackClicked(void);
};

#endif /* defined(__galsprototype__FailScene__) */
