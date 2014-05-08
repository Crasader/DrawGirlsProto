//
//  BugTestScene2.h
//  DGproto
//
//  Created by 사원3 on 2014. 5. 8..
//
//

#ifndef __DGproto__BugTestScene2__
#define __DGproto__BugTestScene2__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class BugTestScene2 : public cocos2d::CCLayer
{
public:
	// Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
	virtual bool init();
	
	// there's no 'id' in cpp, so we recommend to return the class instance pointer
	static cocos2d::CCScene* scene();
	
	// preprocessor macro for "static create()" constructor ( node() deprecated )
	CREATE_FUNC(BugTestScene2);
	
private:
	
	
};

#endif /* defined(__DGproto__BugTestScene2__) */
