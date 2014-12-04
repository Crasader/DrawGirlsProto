//
//  RealTimeMessage.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 10. 20..
//
//

#include "RealTimeMessage.h"
#include "StarGoldData.h"
//#include "KSLabelTTF.h"
#include "StyledLabelTTF.h"

void RealTimeMessage::checkRealTimeMessageQueue()
{
	mySGD->popRealTimeMessageQueue();
	if(!mySGD->isEmptyRealTimeMessageQueue())
	{
		RealTimeMessage* t_message = RealTimeMessage::create(mySGD->frontRealTimeMessageQueue());
		getParent()->addChild(t_message);
	}
}

void RealTimeMessage::myInit(string t_message, CCPoint t_base_position)
{
	base_position = t_base_position;
	
	if(mySGD->isEmptyRealTimeMessageQueue() || mySGD->frontRealTimeMessageQueue() == t_message)
	{
		setPosition(base_position + ccp(0,80));
		back_img = CCScale9Sprite::create("real_time_message_back.png", CCRectMake(0, 0, 218, 43), CCRectMake(108, 21, 2, 1));
		back_img->setPosition(CCPointZero);
		addChild(back_img);
		
//		KSLabelTTF* title_label = KSLabelTTF::create(myAchieve->getTitle(t_code).c_str(), mySGD->getFont().c_str(), 12);
//		title_label->setColor(ccc3(255, 50, 50));
//		title_label->setAnchorPoint(ccp(0,0.5));
//		title_label->setPosition(ccp(-60,8));
//		addChild(title_label);
		
		content_label = StyledLabelTTF::create(t_message.c_str(), mySGD->getFont().c_str(), 12, 922, StyledAlignment::kCenterAlignment);
		content_label->setAnchorPoint(ccp(0.5f,0.5f));
		
		back_img->setContentSize(CCSizeMake(MAX(content_label->getContentSize().width+10, 218), MAX(content_label->getContentSize().height+10, 43)));
		
		content_label->setPosition(ccpFromSize(back_img->getContentSize()/2.f));
		back_img->addChild(content_label);
		
		setPosition(base_position + ccp(0,back_img->getContentSize().height/2.f));
		
		CCMoveTo* t_show = CCMoveTo::create(0.5f, base_position + ccp(0,-back_img->getContentSize().height/2.f));
		CCCallFunc* t_start_animation = CCCallFunc::create(this, callfunc_selector(RealTimeMessage::startShake));
		CCDelayTime* t_delay = CCDelayTime::create(5.f);
		CCCallFunc* t_stop_animation = CCCallFunc::create(this, callfunc_selector(RealTimeMessage::stopShake));
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(RealTimeMessage::checkRealTimeMessageQueue));
		CCMoveTo* t_hide = CCMoveTo::create(0.5f, base_position + ccp(0,back_img->getContentSize().height/2.f));
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(RealTimeMessage::removeFromParent));
		CCSequence* t_seq = CCSequence::create(t_show, t_start_animation, t_delay, t_stop_animation, t_call1, t_hide, t_call2, NULL);
		runAction(t_seq);
		
		if(mySGD->isEmptyRealTimeMessageQueue())
			mySGD->pushRealTimeMessageQueue(t_message);
	}
	else
	{
		mySGD->pushRealTimeMessageQueue(t_message);
		schedule(schedule_selector(RealTimeMessage::autoRemoveAction));
	}
}

void RealTimeMessage::startShake()
{
	CCSequence* t_action = CCSequence::create(CCRotateTo::create(0.05f, -1), CCRotateTo::create(0.05f, 1), NULL);
	CCRepeatForever* t_repeat = CCRepeatForever::create(t_action);
	back_img->runAction(t_repeat);
	
	CCSequence* t_action2 = CCSequence::create(CCRotateTo::create(0.05f, 1), CCRotateTo::create(0.05f, -1), NULL);
	CCRepeatForever* t_repeat2 = CCRepeatForever::create(t_action2);
	content_label->runAction(t_repeat2);
}
void RealTimeMessage::stopShake()
{
	back_img->stopAllActions();
	back_img->setRotation(0);
	
	content_label->stopAllActions();
	content_label->setRotation(0);
}