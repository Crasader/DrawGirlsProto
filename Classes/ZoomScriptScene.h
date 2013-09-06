//
//  ZoomScriptScene.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 6..
//
//

#ifndef __DGproto__ZoomScriptScene__
#define __DGproto__ZoomScriptScene__

#include "cocos2d.h"

USING_NS_CC;

class ZoomScript : public CCLayer
{
public:
	virtual bool init();
    static cocos2d::CCScene* scene();
    CREATE_FUNC(ZoomScript);
	
private:
	
	
};

#endif /* defined(__DGproto__ZoomScriptScene__) */
