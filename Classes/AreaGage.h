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

USING_NS_CC;

class AreaGage : public CCNode
{
public:
	static AreaGage* create(float t_clear_percent);
	
	virtual void setPosition(CCPoint t_p);
	
	void setPercentage(float t_p);
	void onChange();
	
private:
	CCSprite* gage_back;
	CCProgressTimer* gage_bar1;
	CCProgressTimer* gage_bar2;
	CCProgressTimer* gage_bar3;
//	CCProgressTimer* gage_bar4;
//	CCSprite* gage_case;
	
	CCNode* star1;
	CCNode* star2;
//	CCSprite* star3;
	
	float clear_percent;
	float m_percentage;
	
	void removeAndCreateStar();
	
	void myInit(float t_clear_percent);
};

#endif /* defined(__DrawingJack__AreaGage__) */
