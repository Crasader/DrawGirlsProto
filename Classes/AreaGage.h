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
	static AreaGage* create()
	{
		AreaGage* t_blg = new AreaGage();
		t_blg->myInit();
		t_blg->autorelease();
		return t_blg;
	}
	
	virtual void setPosition(CCPoint t_p)
	{
		CCNode::setPosition(t_p);
		
		green_bar->setPosition(green_bar->getPosition());
	}
	
	void setPercentage(float t_p)
	{
		if(t_p > 1.f)
			t_p = 1.f;
		else if(t_p < 0.f)
			t_p = 0.f;
		m_percentage = t_p;
		
		green_bar->actionPercentage(t_p);
		area_icon->setPosition(ccp(-green_bar->getContentSize().width/2.f + green_bar->getContentSize().width*t_p - 1,1));
	}
	
private:
	GageBar* green_bar;
	CCSprite* area_icon;
	
	float m_percentage;
	
	void myInit()
	{
		green_bar = GageBar::create("maingame_ui_areagage.png", 0.f);
		addChild(green_bar);
		
		area_icon = CCSprite::create("maingame_ui_areaicon.png");
		addChild(area_icon);
		
		setPercentage(0.f);
	}
};

#endif /* defined(__DrawingJack__AreaGage__) */
