//
//  AcceptChallengeAniContent.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 2. 3..
//
//

#include "AcceptChallengeAniContent.h"
#include "CCMenuLambda.h"
#include "StarGoldData.h"
#include "GDWebSprite.h"


void AcceptChallengeAniContent::myInit(int t_touch_priority, function<void(CCObject*)> t_selector, string t_profile1, string t_nick1, string t_profile2, string t_nick2)
{
	touch_priority = t_touch_priority;
	end_selector = t_selector;
	
	base_node = CCNode::create();
	base_node->setPosition(ccp(0,320));
	addChild(base_node);
	
	CCLabelTTF* challenge_start = CCLabelTTF::create("도전 시작", mySGD->getFont().c_str(), 30);
	challenge_start->setPosition(ccp(0,100));
	base_node->addChild(challenge_start);
	
	
	nick1_label = CCLabelTTF::create(t_nick1.c_str(), mySGD->getFont().c_str(), 13);
	nick1_label->setPosition(ccp(-100,-40));
	base_node->addChild(nick1_label);
	
	CCSprite* profile1_img = GDWebSprite::create(t_profile1, "ending_noimg.png");
	profile1_img->setAnchorPoint(ccp(0.5,0.5));
	profile1_img->setScale(2.f);
	profile1_img->setPosition(ccp(nick1_label->getContentSize().width/2.f,nick1_label->getContentSize().height/2.f+30));
	nick1_label->addChild(profile1_img);
	
	
	nick2_label = CCLabelTTF::create(t_nick2.c_str(), mySGD->getFont().c_str(), 13);
	nick2_label->setPosition(ccp(100,-40));
	base_node->addChild(nick2_label);
	
	CCSprite* profile2_img = GDWebSprite::create(t_profile2, "ending_noimg.png");
	profile2_img->setAnchorPoint(ccp(0.5,0.5));
	profile2_img->setScale(2.f);
	profile2_img->setPosition(ccp(nick2_label->getContentSize().width/2.f,nick2_label->getContentSize().height/2.f+30));
	nick2_label->addChild(profile2_img);
	
	startMyAnimation();
}