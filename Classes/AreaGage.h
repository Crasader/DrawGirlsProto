//
//  AreaGage.h
//  DrawingJack
//
//  Created by 사원3 on 13. 7. 2..
//
//

#ifndef __DrawingJack__AreaGage__
#define __DrawingJack__AreaGage__

#include "cocos2d.h"
#include "GageBar.h"

class AreaGage : public CCNode
{
public:
	static AreaGage* create();
	
	virtual void setPosition(CCPoint t_p);
	
	void setPercentage(float t_p);
	
private:
	GageBar* green_bar;
	CCSprite* area_icon;
	
	float m_percentage;
	
	void myInit();
};

#endif /* defined(__DrawingJack__AreaGage__) */
