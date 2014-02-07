//
//  AchieveNoti.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 31..
//
//

#include "AchieveNoti.h"
#include "StarGoldData.h"

void AchieveNoti::checkAchieveNotiQueue()
{
	mySGD->popAchieveNotiQueue();
	if(!mySGD->isEmptyAchieveNotiQueue())
	{
		AchieveNoti* t_noti = AchieveNoti::create(mySGD->frontAchieveNotiQueue());
		getParent()->addChild(t_noti);
	}
}

void AchieveNoti::myInit(AchievementCode t_code)
{
	if(mySGD->isEmptyAchieveNotiQueue() || mySGD->frontAchieveNotiQueue() == t_code)
	{
		setPosition(ccp(240,-80));
		CCSprite* back_img = CCSprite::create("achieve_noti_back.png");
		back_img->setPosition(CCPointZero);
		addChild(back_img);
		
		CCLabelTTF* title_label = CCLabelTTF::create(AchieveTitleContent::getAchievementScript(t_code).title.c_str(), mySGD->getFont().c_str(), 13);
		title_label->setAnchorPoint(ccp(0,0.5));
		title_label->setPosition(ccp(-60,10));
		addChild(title_label);
		
		CCLabelTTF* content_label = CCLabelTTF::create(AchieveTitleContent::getAchievementScript(t_code).content.c_str(), mySGD->getFont().c_str(), 10);
		content_label->setAnchorPoint(ccp(0,0.5));
		content_label->setPosition(ccp(-60,-12));
		addChild(content_label);
		
		CCMoveTo* t_show = CCMoveTo::create(0.5f, ccp(240,30));
		CCDelayTime* t_delay = CCDelayTime::create(1.5f);
		CCCallFunc* t_call1 = CCCallFunc::create(this, callfunc_selector(AchieveNoti::checkAchieveNotiQueue));
		CCMoveTo* t_hide = CCMoveTo::create(0.5f, ccp(240,-80));
		CCCallFunc* t_call2 = CCCallFunc::create(this, callfunc_selector(AchieveNoti::removeFromParent));
		CCSequence* t_seq = CCSequence::create(t_show, t_delay, t_call1, t_hide, t_call2, NULL);
		runAction(t_seq);
		
		if(mySGD->isEmptyAchieveNotiQueue())
			mySGD->pushAchieveNotiQueue(t_code);
	}
	else
	{
		mySGD->pushAchieveNotiQueue(t_code);
		schedule(schedule_selector(AchieveNoti::autoRemoveAction));
	}
}