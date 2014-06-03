//
//  AchieveNoti.cpp
//  DGproto
//
//  Created by 사원3 on 2013. 12. 31..
//
//

#include "AchieveNoti.h"
#include "StarGoldData.h"
#include "KSLabelTTF.h"

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
	myAchieve->onNoti(t_code);
	
	if(mySGD->isEmptyAchieveNotiQueue() || mySGD->frontAchieveNotiQueue() == t_code)
	{
		setPosition(ccp(240,-80));
		CCSprite* back_img = CCSprite::create("achieve_noti_back.png");
		back_img->setPosition(CCPointZero);
		addChild(back_img);
		
		AchieveGroup t_group = myAchieve->getAchieveGroup(t_code);
		if(t_group.achieve_list.size() == 1)
		{
			CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
			t_crown->setPosition(ccp(20, back_img->getContentSize().height/2.f+8));
			back_img->addChild(t_crown);
		}
		else if(t_group.achieve_list.size() == 3)
		{
			int t_index = 0;
			for(int i=0;i<t_group.achieve_list.size();i++)
			{
				if(t_group.achieve_list[i] == t_code)
				{
					t_index = i;
				}
			}
			
			if(t_index == 0)
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_bronze.png");
				t_crown->setPosition(ccp(20, back_img->getContentSize().height/2.f+8));
				back_img->addChild(t_crown);
			}
			else if(t_index == 1)
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_silver.png");
				t_crown->setPosition(ccp(20, back_img->getContentSize().height/2.f+8));
				back_img->addChild(t_crown);
			}
			else if(t_index == 2)
			{
				CCSprite* t_crown = CCSprite::create("achievement_crown_gold.png");
				t_crown->setPosition(ccp(20, back_img->getContentSize().height/2.f+8));
				back_img->addChild(t_crown);
			}
		}
		
		KSLabelTTF* title_label = KSLabelTTF::create(myAchieve->getTitle(t_code).c_str(), mySGD->getFont().c_str(), 12);
		title_label->setColor(ccc3(255, 170, 20));
		title_label->setAnchorPoint(ccp(0,0.5));
		title_label->setPosition(ccp(-60,8));
		addChild(title_label);
		
		KSLabelTTF* content_label = KSLabelTTF::create(myAchieve->getContent(t_code).c_str(), mySGD->getFont().c_str(), 9);
		content_label->setAnchorPoint(ccp(0,0.5));
		content_label->setPosition(ccp(-60,-8));
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