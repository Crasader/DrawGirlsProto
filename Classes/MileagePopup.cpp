//
//  MileagePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 29..
//
//

#include "MileagePopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "ASPopupView.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "PuzzleScene.h"
#include "FormSetter.h"
#include "CommonAnimation.h"

MileagePopup* MileagePopup::create(int t_touch_priority, function<void()> t_end_func, int t_count)
{
	MileagePopup* t_mup = new MileagePopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_count);
	t_mup->autorelease();
	return t_mup;
}

void MileagePopup::myInit(int t_touch_priority, function<void()> t_end_func, int t_count)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;

	mileage_count = t_count;
	
	setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(320,240));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mileageTitle), mySGD->getFont().c_str(), 21);
	title_label->setColor(ccc3(50, 250, 255));
	title_label->setPosition(ccp(0,70));
	m_container->addChild(title_label);
	
	KSLabelTTF* sub_ment1 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mileageMent1), mySGD->getFont().c_str(), 12);
	sub_ment1->setColor(ccYELLOW);
	sub_ment1->setPosition(ccp(0,39));
	m_container->addChild(sub_ment1);
	
	
	KSLabelTTF* sub_ment2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mileageMent2), mySGD->getFont().c_str(), 12);
	sub_ment2->setPosition(ccp(0,22));
	m_container->addChild(sub_ment2);
	
	KSLabelTTF* sub_ment3 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_mileageMent3), mySGD->getFont().c_str(), 12);
	sub_ment3->setColor(ccYELLOW);
	sub_ment3->setPosition(ccp(0,5));
	m_container->addChild(sub_ment3);
	
	
	for(int i=0;i<5;i++)
	{
		CCSprite* empty_circle = CCSprite::create("mileage_off.png");
		empty_circle->setPosition(ccp(-90+i*45,-55));
		m_container->addChild(empty_circle);
		
		KSLabelTTF* count_label = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont2().c_str(), 14);
		count_label->setPosition(ccp(empty_circle->getContentSize().width/2.f, empty_circle->getContentSize().height/2.f));
		empty_circle->addChild(count_label);
		
		if(i < mileage_count-1)
		{
			CCSprite* mileage_on = CCSprite::create("mileage_on.png");
			mileage_on->setPosition(empty_circle->getPosition());
			m_container->addChild(mileage_on);
		}
	}
	
	CommonAnimation::openPopup(this, m_container, nullptr, [=](){
		
	}, [=](){
		CCSprite* mileage_on = CCSprite::create("mileage_on.png");
		mileage_on->setPosition(ccp(-90+(mileage_count-1)*45,-55));
		m_container->addChild(mileage_on);
		mileage_on->setScale(3.f);
		mileage_on->setOpacity(0);
		
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t){
			mileage_on->setOpacity(t*255);
			mileage_on->setScale(3.f-2.f*t);
		}, [=](float t){
			mileage_on->setOpacity(255);
			mileage_on->setScale(1.f);
			
			is_menu_enable = true;
		}));
	});
}

bool MileagePopup::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	if(!is_menu_enable)
		return true;
	
	is_menu_enable = false;
	
	CommonAnimation::closePopup(this, m_container, nullptr, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
	
	return true;
}

void MileagePopup::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void MileagePopup::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void MileagePopup::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void MileagePopup::registerWithTouchDispatcher()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, touch_priority, true);
}