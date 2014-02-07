//
//  BossLifeGage.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 7. 2..
//
//

#include "BossLifeGage.h"
#include "GageBar.h"

BossLifeGage* BossLifeGage::create()
{
	BossLifeGage* t_blg = new BossLifeGage();
	t_blg->myInit();
	t_blg->autorelease();
	return t_blg;
}

void BossLifeGage::setPosition( CCPoint t_p )
{
	CCNode::setPosition(t_p);

	yellow_bar->setPosition(yellow_bar->getPosition());
	red_bar->setPosition(red_bar->getPosition());
}

void BossLifeGage::setPercentage( float t_p )
{
	if(t_p > 1.f)
		t_p = 1.f;
	else if(t_p < 0.f)
		t_p = 0.f;
	m_percentage = t_p;

	red_bar->setPercentage(t_p);
	yellow_bar->actionPercentage(t_p);
	boss_icon->setPosition(ccp(-red_bar->getContentSize().width/2.f + red_bar->getContentSize().width*t_p - 1,1));
}

void BossLifeGage::myInit()
{
	yellow_bar = GageBar::create("maingame_ui_bossgage_sub.png", 1.f);
	addChild(yellow_bar);

	red_bar = GageBar::create("maingame_ui_bossgage.png", 1.f);
	addChild(red_bar);

	boss_icon = CCSprite::create("maingame_ui_bossicon.png");
	addChild(boss_icon);

	setPercentage(1.f);
}
