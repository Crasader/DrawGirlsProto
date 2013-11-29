//
//  MinsuScene.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 29..
//
//

#ifndef __DGproto__MinsuScene__
#define __DGproto__MinsuScene__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class MinsuScene : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
	
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
	
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MinsuScene);
	
private:
	
};

#endif /* defined(__DGproto__MinsuScene__) */
