//
//  ConditionPopup.cpp
//  DGproto
//
//  Created by 사원3 on 13. 9. 12..
//
//

#include "ConditionPopup.h"
#include "StarGoldData.h"
#include "SilhouetteData.h"
#include "DataStorageHub.h"

ConditionPopup* ConditionPopup::create( CCObject* t_close, SEL_CallFunc d_close )
{
	ConditionPopup* t_cdt = new ConditionPopup();
	t_cdt->myInit(t_close, d_close);
	t_cdt->autorelease();
	return t_cdt;
}

void ConditionPopup::holdingPopup()
{
	unschedule(schedule_selector(ConditionPopup::hiding));
	cdt_back->stopAllActions();
	content->stopAllActions();

	cdt_back->setOpacity(255);
	content->setOpacity(255);

	startHiding();
}

void ConditionPopup::startHiding()
{
	ing_frame = 0;
	end_frame = 60;
	schedule(schedule_selector(ConditionPopup::hiding));
}

void ConditionPopup::hiding()
{
	ing_frame++;

	if(ing_frame >= end_frame)
	{
		unschedule(schedule_selector(ConditionPopup::hiding));
		CCFadeTo* fade1 = CCFadeTo::create(1.f, 0);
		CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ConditionPopup::endCall));
		CCSequence* t_seq = CCSequence::createWithTwoActions(fade1, t_call);

		CCFadeTo* fade2 = CCFadeTo::create(0.9f, 0);

		cdt_back->runAction(t_seq);
		content->runAction(fade2);
	}
}

void ConditionPopup::endCall()
{
	(target_close->*delegate_close)();
	removeFromParent();
}

void ConditionPopup::myInit( CCObject* t_close, SEL_CallFunc d_close )
{
	target_close = t_close;
	delegate_close = d_close;

	cdt_back = CCSprite::create("condition_box.png");
	if(myGD->gamescreen_type == kGT_leftUI)		cdt_back->setPosition(ccp(140,myDSH->ui_center_y));
	else if(myGD->gamescreen_type == kGT_rightUI)	cdt_back->setPosition(ccp(480-140,myDSH->ui_center_y));
	else								cdt_back->setPosition(ccp(350,myDSH->ui_top-30));
	addChild(cdt_back, kCDT_Z_back);

	content = CCLabelTTF::create(mySD->getConditionContent().c_str(), mySGD->getFont().c_str(), 14, CCSizeMake(150, 45), kCCTextAlignmentCenter);
	content->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
	content->setColor(ccBLACK);
	content->setPosition(ccp(cdt_back->getContentSize().width/2.f,cdt_back->getContentSize().height/2.f));
	cdt_back->addChild(content, kCDT_Z_content);

	startHiding();
}
