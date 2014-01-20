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
	CCSprite* change1 = CCSprite::create("star_change.png");
	change1->setScale(0);
	star1->setPosition(ccp(gage_bar2->getPositionX()-(star1->getContentSize().width+change1->getContentSize().width)/2.f+star1->getContentSize().width/2.f+2, 20));
	change1->setPosition(ccp(gage_bar2->getPositionX()-(star1->getContentSize().width+change1->getContentSize().width)/2.f+star1->getContentSize().width+change1->getContentSize().width/2.f-2, 20));
	addChild(change1);
	CCScaleTo* t_scale1_1 = CCScaleTo::create(0.5f, 1.5f);
	CCScaleTo* t_scale1_2 = CCScaleTo::create(0.3f, 1.f);
	CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_scale1_1, t_scale1_2);
	change1->runAction(t_seq1);
	
	CCSprite* change2 = CCSprite::create("star_change.png");
	change2->setScale(0);
	star2->setPosition(ccp(gage_bar3->getPositionX()-(star2->getContentSize().width+change2->getContentSize().width)/2.f+star2->getContentSize().width/2.f+2, 20));
	change2->setPosition(ccp(gage_bar3->getPositionX()-(star2->getContentSize().width+change2->getContentSize().width)/2.f+star2->getContentSize().width+change2->getContentSize().width/2.f-2, 20));
	addChild(change2);
	CCScaleTo* t_scale2_1 = CCScaleTo::create(0.5f, 1.5f);
	CCScaleTo* t_scale2_2 = CCScaleTo::create(0.3f, 1.f);
	CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_scale2_1, t_scale2_2);
	change2->runAction(t_seq2);
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
	
	star1 = CCSprite::create("star_one.png");
	star1->setPosition(ccp(gage_bar2->getPositionX(), 20));
	addChild(star1);
	
	star2 = CCSprite::create("star_two.png");
	star2->setPosition(ccp(gage_bar3->getPositionX(), 20));
	addChild(star2);
	
	setPercentage(0.f);
}
