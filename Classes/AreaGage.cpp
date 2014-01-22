//
//  AreaGage.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 7. 2..
//
//

#include "AreaGage.h"

void AreaGage::setPosition( CCPoint t_p )
{
	CCNode::setPosition(t_p);
}

void AreaGage::setPercentage( float t_p )
{
	if(t_p > 1.f)			t_p = 1.f;
	else if(t_p < 0.f)		t_p = 0.f;
	
	if(t_p < clear_percent)
	{
		CCProgressTo* t_to1 = CCProgressTo::create(0.3f, t_p/clear_percent*100.f);
		gage_bar1->runAction(t_to1);
	}
	else if(t_p < 1.f)
	{
		CCProgressTo* t_to1 = CCProgressTo::create(0.2f, 100.f);
		gage_bar1->runAction(t_to1);
		
		CCDelayTime* t_delay2 = CCDelayTime::create(0.2f);
		CCProgressTo* t_to2 = CCProgressTo::create(0.1f, (t_p-clear_percent)/(1.f-clear_percent)*100.f);
		CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_delay2, t_to2);
		gage_bar2->runAction(t_seq2);
	}
	else
	{
		CCProgressTo* t_to1 = CCProgressTo::create(0.1f, 100.f);
		gage_bar1->runAction(t_to1);
		
		CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
		CCProgressTo* t_to2 = CCProgressTo::create(0.1f, 100.f);
		CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_delay2, t_to2);
		gage_bar2->runAction(t_seq2);
		
		CCDelayTime* t_delay3 = CCDelayTime::create(0.2f);
		CCProgressTo* t_to3 = CCProgressTo::create(0.1f, 100.f);
		CCSequence* t_seq3 = CCSequence::createWithTwoActions(t_delay3, t_to3);
		gage_bar3->runAction(t_seq3);
	}
	
	m_percentage = t_p;

}

void AreaGage::onChange()
{
	star1->runAction(CCScaleTo::create(0.5f, 0));
	star2->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.5f, 0), CCCallFunc::create(this, callfunc_selector(AreaGage::removeAndCreateStar))));
}

void AreaGage::removeAndCreateStar()
{
	CCPoint star1_position = star1->getPosition();
	CCPoint star2_position = star2->getPosition();
	star1->removeFromParent();
	star2->removeFromParent();
	
	star1 = CCSprite::create("piece_star_silver.png");
	star1->setScale(0);
	star1->setPosition(star1_position);
	addChild(star1);
	
	star1->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.3f, 1.5f), CCScaleTo::create(0.2f, 1)));
	
	star2 = CCSprite::create("piece_star_gold.png");
	star2->setScale(0);
	star2->setPosition(star2_position);
	addChild(star2);
	
	star2->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.3f, 1.5f), CCScaleTo::create(0.2f, 1)));
}

AreaGage* AreaGage::create(float t_clear_percent)
{
	AreaGage* t_blg = new AreaGage();
	t_blg->myInit(t_clear_percent);
	t_blg->autorelease();
	return t_blg;
}

void AreaGage::myInit(float t_clear_percent)
{
	clear_percent = t_clear_percent;

	gage_back = CCSprite::create("star_gage_back.png");
	gage_back->setPosition(CCPointZero);
	addChild(gage_back);
	
	gage_bar1 = CCProgressTimer::create(CCSprite::create("star_gage_bar1.png"));
	gage_bar1->setType(kCCProgressTimerTypeBar);
	gage_bar1->setMidpoint(ccp(0,0));
	gage_bar1->setBarChangeRate(ccp(1,0));
	gage_bar1->setPercentage(0);
	gage_bar1->setAnchorPoint(ccp(0,0.5));
	gage_bar1->setPosition(ccp(-gage_back->getContentSize().width/2.f, 0));
	addChild(gage_bar1);
	
	gage_bar2 = CCProgressTimer::create(CCSprite::create("star_gage_bar2.png"));
	gage_bar2->setType(kCCProgressTimerTypeBar);
	gage_bar2->setMidpoint(ccp(0,0));
	gage_bar2->setBarChangeRate(ccp(1,0));
	gage_bar2->setPercentage(0);
	gage_bar2->setAnchorPoint(ccp(0,0.5));
	gage_bar2->setPosition(ccpAdd(gage_bar1->getPosition(), ccp(gage_bar1->getSprite()->getContentSize().width, 0)));
	addChild(gage_bar2);
	
	gage_bar3 = CCProgressTimer::create(CCSprite::create("star_gage_bar3.png"));
	gage_bar3->setType(kCCProgressTimerTypeBar);
	gage_bar3->setMidpoint(ccp(0,0));
	gage_bar3->setBarChangeRate(ccp(1,0));
	gage_bar3->setPercentage(0);
	gage_bar3->setAnchorPoint(ccp(0,0.5));
	gage_bar3->setPosition(ccpAdd(gage_bar2->getPosition(), ccp(gage_bar2->getSprite()->getContentSize().width, 0)));
	addChild(gage_bar3);
	
	gage_case = CCSprite::create("star_gage_case.png");
	gage_case->setPosition(CCPointZero);
	addChild(gage_case);
	
	star1 = CCSprite::create("piece_star_bronze.png");
	star1->setPosition(ccp(gage_bar2->getPositionX(), 20));
	addChild(star1);
	
	star2 = CCSprite::create("piece_star_silver.png");
	star2->setPosition(ccp(gage_bar3->getPositionX(), 20));
	addChild(star2);
	
	setPercentage(0.f);
}
