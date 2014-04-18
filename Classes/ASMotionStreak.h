//
//  ASMotionStreak.h
//  DGproto
//
//  Created by 사원3 on 2014. 4. 18..
//
//

#ifndef __DGproto__ASMotionStreak__
#define __DGproto__ASMotionStreak__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class ASMotionStreak : public CCMotionStreak
{
public:
	static ASMotionStreak* create(float fade, float minSeg, float stroke, ccColor3B color, const char* path)
	{
		ASMotionStreak *pRet = new ASMotionStreak();
		if (pRet && pRet->initWithFade(fade, minSeg, stroke, color, path))
		{
			pRet->autorelease();
			return pRet;
		}
		
		CC_SAFE_DELETE(pRet);
		return NULL;
	}
	
	virtual void setOpacity(GLubyte opacity)
	{
		
	}
	
	GLubyte getOpacity(void)
	{
		return 0;
	}
	
private:
	
	
};

#endif /* defined(__DGproto__ASMotionStreak__) */
