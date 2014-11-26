//
//  PuzzlePiece.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 19..
//
//

#include "PuzzlePiece.h"


void PuzzlePiece::setStar(CCSprite* piece_img)
{
	//		int t_grade = 0;
	//		if(is_have_card[3])
	//			t_grade = 4;
	//		else if(is_have_card[2])
	//			t_grade = 3;
	//		else if(is_have_card[1])
	//			t_grade = 2;
	//		else if(is_have_card[0])
	//			t_grade = 1;
	
	for(int i=3;i>=0;i--)
	{
		if(is_have_card[i])
		{
			CCSprite* star_img = CCSprite::create("gage_star_gold.png");
			star_img->setPosition(ccp(piece_img->getContentSize().width/2.f+i*7, piece_img->getContentSize().height/2.f-20));
			piece_img->addChild(star_img);
		}
		else if(mySGD->isHasGottenCards(stage_number, i+1) == 0)
		{
			CCSprite* star_img = CCSprite::create("gage_star_gold_down.png");
			star_img->setPosition(ccp(piece_img->getContentSize().width/2.f+i*7, piece_img->getContentSize().height/2.f-20));
			piece_img->addChild(star_img);
		}
		else
		{
			GraySprite* star_img = GraySprite::create("gage_star_gold.png");
			star_img->setPosition(ccp(piece_img->getContentSize().width/2.f+i*7, piece_img->getContentSize().height/2.f-20));
			star_img->setGray(true);
			piece_img->addChild(star_img);
		}
	}
	
	//		if(t_grade >= 4)
	//		{
	//			CCSprite* star_img = CCSprite::create("gage_star_gold.png");
	//			star_img->setPosition(ccpAdd(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f), ccp(cosf((45.f-90.f)/180.f*M_PI)*10.f, sinf((45.f-90.f)/180.f*M_PI)*10.f)));
	//			piece_img->addChild(star_img);
	//		}
	//		else
	//		{
	//			CCSprite* star_img = CCSprite::create("gage_star_gold_down.png");
	//			star_img->setPosition(ccpAdd(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f), ccp(cosf((45.f-90.f)/180.f*M_PI)*10.f, sinf((45.f-90.f)/180.f*M_PI)*10.f)));
	//			piece_img->addChild(star_img);
	//		}
	//
	//		if(t_grade >= 3)
	//		{
	//			CCSprite* star_img = CCSprite::create("gage_star_gold.png");
	//			star_img->setPosition(ccpAdd(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f), ccp(cosf((45.f+180.f)/180.f*M_PI)*10.f, sinf((45.f+180.f)/180.f*M_PI)*10.f)));
	//			piece_img->addChild(star_img);
	//		}
	//		else
	//		{
	//			CCSprite* star_img = CCSprite::create("gage_star_gold_down.png");
	//			star_img->setPosition(ccpAdd(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f), ccp(cosf((45.f+180.f)/180.f*M_PI)*10.f, sinf((45.f+180.f)/180.f*M_PI)*10.f)));
	//			piece_img->addChild(star_img);
	//		}
	//
	//		if(t_grade >= 2)
	//		{
	//			CCSprite* star_img = CCSprite::create("gage_star_gold.png");
	//			star_img->setPosition(ccpAdd(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f), ccp(cosf((45.f)/180.f*M_PI)*10.f, sinf((45.f)/180.f*M_PI)*10.f)));
	//			piece_img->addChild(star_img);
	//		}
	//		else
	//		{
	//			CCSprite* star_img = CCSprite::create("gage_star_gold_down.png");
	//			star_img->setPosition(ccpAdd(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f), ccp(cosf((45.f)/180.f*M_PI)*10.f, sinf((45.f)/180.f*M_PI)*10.f)));
	//			piece_img->addChild(star_img);
	//		}
	//
	//		if(t_grade >= 1)
	//		{
	//			CCSprite* star_img = CCSprite::create("gage_star_gold.png");
	//			star_img->setPosition(ccpAdd(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f), ccp(cosf((45.f+90.f)/180.f*M_PI)*10.f, sinf((45.f+90.f)/180.f*M_PI)*10.f)));
	//			piece_img->addChild(star_img);
	//		}
	//		else
	//		{
	//			CCSprite* star_img = CCSprite::create("gage_star_gold_down.png");
	//			star_img->setPosition(ccpAdd(ccp(piece_img->getContentSize().width/2.f, piece_img->getContentSize().height/2.f), ccp(cosf((45.f+90.f)/180.f*M_PI)*10.f, sinf((45.f+90.f)/180.f*M_PI)*10.f)));
	//			piece_img->addChild(star_img);
	//		}
}