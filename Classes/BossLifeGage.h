//
//  BossLifeGage.h
//  DrawingJack
//
//  Created by 사원3 on 13. 7. 2..
//
//

#ifndef __DrawingJack__BossLifeGage__
#define __DrawingJack__BossLifeGage__

#include "cocos2d.h"

USING_NS_CC;
class GageBar;
class BossLifeGage : public CCNode
{
public:
	static BossLifeGage* create();
	
	virtual void setPosition(CCPoint t_p);
	
	void setPercentage(float t_p);
	
private:
	
	GageBar* red_bar;
	GageBar* yellow_bar;
	CCSprite* boss_icon;
	
	float m_percentage;
	
	void myInit();
};

#endif /* defined(__DrawingJack__BossLifeGage__) */
