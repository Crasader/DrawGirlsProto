//
//  AreaGage.cpp
//  DrawingJack
//
//  Created by 사원3 on 13. 7. 2..
//
//

#include "AreaGage.h"
#include "DataStorageHub.h"
#include "GraySprite.h"
#include "StarGoldData.h"

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
		gage_bar1->stopAllActions();
		CCProgressTo* t_to1 = CCProgressTo::create(0.3f, t_p/clear_percent*100.f);
		gage_bar1->runAction(t_to1);
		
		gage_bar2->stopAllActions();
		gage_bar2->runAction(CCProgressTo::create(0.1f, 0.f));
		
		gage_bar3->stopAllActions();
		gage_bar3->runAction(CCProgressTo::create(0.1f, 0.f));
	}
	else if(t_p < 1.f)
	{
		if(m_percentage < clear_percent)
		{
			gage_bar1->stopAllActions();
			CCProgressTo* t_to1 = CCProgressTo::create(0.2f, 100.f);
			gage_bar1->runAction(t_to1);
		}
		
		gage_bar2->stopAllActions();
		CCDelayTime* t_delay2 = CCDelayTime::create(0.2f);
		CCProgressTo* t_to2 = CCProgressTo::create(0.1f, (t_p-clear_percent)/(1.f-clear_percent)*100.f);
		CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_delay2, t_to2);
		gage_bar2->runAction(t_seq2);
		
		gage_bar3->stopAllActions();
		gage_bar3->runAction(CCProgressTo::create(0.1f, 0.f));
	}
	else
	{
		if(m_percentage < clear_percent)
		{
			gage_bar1->stopAllActions();
			CCProgressTo* t_to1 = CCProgressTo::create(0.1f, 100.f);
			gage_bar1->runAction(t_to1);
		}
		
		gage_bar2->stopAllActions();
		CCDelayTime* t_delay2 = CCDelayTime::create(0.1f);
		CCProgressTo* t_to2 = CCProgressTo::create(0.1f, 100.f);
		CCSequence* t_seq2 = CCSequence::createWithTwoActions(t_delay2, t_to2);
		gage_bar2->runAction(t_seq2);
		
		gage_bar3->stopAllActions();
		CCDelayTime* t_delay3 = CCDelayTime::create(0.2f);
		CCProgressTo* t_to3 = CCProgressTo::create(0.1f, 100.f);
		CCSequence* t_seq3 = CCSequence::createWithTwoActions(t_delay3, t_to3);
		gage_bar3->runAction(t_seq3);
	}
	
	m_percentage = t_p;

}

void AreaGage::onChange()
{
	star1->removeAllChildren();
	star2->removeAllChildren();
	
	CCPoint base_position = ccp(-9,0);
	CCPoint d_position = ccp(9,0);
	for(int i=0;i<3;i++)
	{
		GraySprite* star_img = GraySprite::create("star_on.png");
		int card_number3 = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 3);
		star_img->setGray(mySGD->getHasGottenCardDataForCardNumber(card_number3).count.getV() > 0);//mySGD->is_cleared_grade[2] == 1);
		star_img->setPosition(base_position + d_position*i);
		star1->addChild(star_img);
	}
	
	base_position = ccp(-13.5f,0);
	d_position = ccp(9,0);
	for(int i=0;i<4;i++)
	{
		GraySprite* star_img = GraySprite::create("star_on.png");
		int card_number4 = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 4);
		star_img->setGray(mySGD->getHasGottenCardDataForCardNumber(card_number4).count.getV() > 0);//mySGD->is_cleared_grade[3] == 1);
		star_img->setPosition(base_position + d_position*i);
		star2->addChild(star_img);
	}
	
//	star1->runAction(CCScaleTo::create(0.5f, 0));
//	star2->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.5f, 0), CCCallFunc::create(this, callfunc_selector(AreaGage::removeAndCreateStar))));
}

void AreaGage::removeAndCreateStar()
{
//	CCPoint star1_position = star1->getPosition();
//	CCPoint star2_position = star2->getPosition();
//	star1->removeFromParent();
//	star2->removeFromParent();
//	
//	star1 = CCSprite::create("gage_star_silver.png");
//	star1->setScale(0);
//	star1->setPosition(star1_position);
//	addChild(star1);
//	
//	star1->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.3f, 1.5f), CCScaleTo::create(0.2f, 1)));
//	
//	star2 = CCSprite::create("gage_star_gold.png");
//	star2->setScale(0);
//	star2->setPosition(star2_position);
//	addChild(star2);
//	
//	star2->runAction(CCSequence::createWithTwoActions(CCScaleTo::create(0.3f, 1.5f), CCScaleTo::create(0.2f, 1)));
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
	
	CCSprite* gage_case = CCSprite::create("star_gage_case.png");
	gage_case->setPosition(CCPointZero);
	addChild(gage_case, 2);
	
//	ccColor3B gage_color;
//	
//	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber)-1;
//	puzzle_number %= 7;
//	puzzle_number++;
//	
//	if(puzzle_number == 1)
//		gage_color = ccc3(0, 0, 255);
//	else if(puzzle_number == 2)
//		gage_color = ccc3(240, 110, 170);
//	else if(puzzle_number == 3)
//		gage_color = ccc3(255, 245, 104);
//	else if(puzzle_number == 4)
//		gage_color = ccc3(57, 181, 74);
//	else if(puzzle_number == 5)
//		gage_color = ccc3(168, 100, 168);
//	else if(puzzle_number == 6)
//		gage_color = ccc3(0, 191, 243);
//	else if(puzzle_number == 7)
//		gage_color = ccc3(242, 101, 34);
//	else
//		gage_color = ccc3(0, 0, 255);
	
	gage_bar1 = CCProgressTimer::create(CCSprite::create("star_gage_bar1.png"));
//	gage_bar1->getSprite()->setColor(gage_color);
	gage_bar1->setType(kCCProgressTimerTypeBar);
	gage_bar1->setMidpoint(ccp(0,0));
	gage_bar1->setBarChangeRate(ccp(1,0));
	gage_bar1->setPercentage(0);
	gage_bar1->setAnchorPoint(ccp(0,0.5));
	gage_bar1->setPosition(ccp(-gage_back->getContentSize().width/2.f, 0));
	addChild(gage_bar1);
	
	gage_bar2 = CCProgressTimer::create(CCSprite::create("star_gage_bar2.png"));
//	gage_bar2->getSprite()->setColor(gage_color);
	gage_bar2->setType(kCCProgressTimerTypeBar);
	gage_bar2->setMidpoint(ccp(0,0));
	gage_bar2->setBarChangeRate(ccp(1,0));
	gage_bar2->setPercentage(0);
	gage_bar2->setAnchorPoint(ccp(0,0.5));
	gage_bar2->setPosition(ccpAdd(gage_bar1->getPosition(), ccp(gage_bar1->getSprite()->getContentSize().width, 0)));
	addChild(gage_bar2);
	
	gage_bar3 = CCProgressTimer::create(CCSprite::create("star_gage_bar3.png"));
//	gage_bar3->getSprite()->setColor(gage_color);
	gage_bar3->setType(kCCProgressTimerTypeBar);
	gage_bar3->setMidpoint(ccp(0,0));
	gage_bar3->setBarChangeRate(ccp(1,0));
	gage_bar3->setPercentage(0);
	gage_bar3->setAnchorPoint(ccp(0,0.5));
	gage_bar3->setPosition(ccpAdd(gage_bar2->getPosition(), ccp(gage_bar2->getSprite()->getContentSize().width, 0)));
	addChild(gage_bar3);
	
	float w1 = gage_bar1->getSprite()->getContentSize().width;
	float w2 = gage_bar2->getSprite()->getContentSize().width;
	float w3 = gage_bar3->getSprite()->getContentSize().width;
	
	gage_bar1->setPositionX(-(w1+w2+w3)/2.f);
	gage_bar2->setPositionX(gage_bar1->getPositionX()+w1);
	gage_bar3->setPositionX(gage_bar2->getPositionX()+w2);
	
//	gage_bar4 = CCProgressTimer::create(CCSprite::create("star_gage_bar3.png"));
//	gage_bar4->getSprite()->setColor(gage_color);
//	gage_bar4->setType(kCCProgressTimerTypeBar);
//	gage_bar4->setMidpoint(ccp(0,0));
//	gage_bar4->setBarChangeRate(ccp(1,0));
//	gage_bar4->setPercentage(0);
//	gage_bar4->setAnchorPoint(ccp(0,0.5));
//	gage_bar4->setPosition(ccpAdd(gage_bar3->getPosition(), ccp(gage_bar3->getSprite()->getContentSize().width, 0)));
//	addChild(gage_bar4);
	
//	gage_case = CCSprite::create("star_gage_case.png");
//	gage_case->setPosition(CCPointZero);
//	addChild(gage_case);
	
	star1 = CCNode::create();
	star1->setPosition(ccp(gage_bar2->getPositionX(), 11));
	addChild(star1, 3);
	
	{
		GraySprite* star_img = GraySprite::create("star_on.png");
		int card_number1 = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 1);
		star_img->setGray(mySGD->getHasGottenCardDataForCardNumber(card_number1).count.getV() > 0);//mySGD->is_cleared_grade[0] == 1);
		star_img->setPosition(ccp(0,0));
		star1->addChild(star_img);
	}
	
	star2 = CCNode::create();// CCSprite::create("gage_star_gold.png");
	star2->setPosition(ccp(gage_bar3->getPositionX(), 11));
	addChild(star2, 3);
	
	CCPoint base_position = ccp(-4.5f,0);
	CCPoint d_position = ccp(9,0);
	for(int i=0;i<2;i++)
	{
		GraySprite* star_img = GraySprite::create("star_on.png");
		int card_number2 = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, 2);
		star_img->setGray(mySGD->getHasGottenCardDataForCardNumber(card_number2).count.getV() > 0);//mySGD->is_cleared_grade[1] == 1);
		star_img->setPosition(base_position + d_position*i);
		star2->addChild(star_img);
	}
	
//	star3 = CCSprite::create("gage_star_gold.png");
//	star3->setPosition(ccp(gage_bar4->getPositionX(), 1));
//	addChild(star3);
	
	setPercentage(0.f);
}
