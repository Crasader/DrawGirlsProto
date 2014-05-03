//
//  TodayMissionPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 2..
//
//

#include "TodayMissionPopup.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "StyledLabelTTF.h"

TodayMissionPopup* TodayMissionPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	TodayMissionPopup* t_mup = new TodayMissionPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void TodayMissionPopup::myInit(int t_touch_priority, function<void()> t_end_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(320,250));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_todaymission), mySGD->getFont().c_str(), 21);
	title_label->setColor(ccc3(50, 250, 255));
	title_label->setPosition(ccp(0,80));
	m_container->addChild(title_label);
	
	
	TodayMissionType mission_type = (TodayMissionType)mySGD->today_mission_info.mission_type.getV();
	
	Json::Value value1, value2, value3, value4, value5;
	value1["fillcolor"] = StyledLabelTTF::makeRGB(255, 255, 255);
	value1["font"] = mySGD->getFont();
	value1["size"] = 16.f;
	
	value2["fillcolor"] = StyledLabelTTF::makeRGB(255, 255, 0);
	value2["font"] = mySGD->getFont();
	value2["size"] = 16.f;
	
	value3["fillcolor"] = StyledLabelTTF::makeRGB(255, 255, 255);
	value3["font"] = mySGD->getFont();
	value3["size"] = 12.f;
	
	value4["fillcolor"] = StyledLabelTTF::makeRGB(255, 255, 0);
	value4["font"] = mySGD->getFont();
	value4["size"] = 18.f;
	
	value5["linebreak"] = true;
	value5["linespacing"] = 10.f;
	
	if(mission_type == kTodayMissionType_totalPercent)
	{
		StyledLabelTTF* slttf = StyledLabelTTF::create({
			{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalPercent1), value1.toStyledString()},
			{CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalPercent2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.goal_count.getV())->getCString()).c_str())->getCString(), value2.toStyledString()},
			{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalPercent3), value1.toStyledString()},
			{"", value5.toStyledString()}
		}, StyledAlignment::kCenterAlignment);
		slttf->setPosition(ccp(0, 45));
		m_container->addChild(slttf);
		
		
		if(mySGD->today_mission_info.ing_count.getV() >= mySGD->today_mission_info.goal_count.getV())
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(kMyLocalKey_todaymissionSuccess), value1.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			under_label->setPosition(ccp(0, -20));
			m_container->addChild(under_label);
		}
		else
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalPercent4), value3.toStyledString()},
				{CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalPercent2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str())->getCString(), value4.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			under_label->setPosition(ccp(0, -20));
			m_container->addChild(under_label);
		}
	}
	else if(mission_type == kTodayMissionType_totalScore)
	{
		StyledLabelTTF* slttf = StyledLabelTTF::create({
			{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalScore1), value1.toStyledString()},
			{CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalScore2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.goal_count.getV())->getCString()).c_str())->getCString(), value2.toStyledString()},
			{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalScore3), value1.toStyledString()},
			{"", value5.toStyledString()}
		}, StyledAlignment::kCenterAlignment);
		slttf->setPosition(ccp(0, 45));
		m_container->addChild(slttf);
		
		
		if(mySGD->today_mission_info.ing_count.getV() >= mySGD->today_mission_info.goal_count.getV())
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(kMyLocalKey_todaymissionSuccess), value1.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			under_label->setPosition(ccp(0, -20));
			m_container->addChild(under_label);
		}
		else
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalScore4), value3.toStyledString()},
				{CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalScore2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str())->getCString(), value4.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			under_label->setPosition(ccp(0, -20));
			m_container->addChild(under_label);
		}
	}
	else if(mission_type == kTodayMissionType_totalTakeGold)
	{
		StyledLabelTTF* slttf = StyledLabelTTF::create({
			{CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalTakeGold1), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.goal_count.getV())->getCString()).c_str())->getCString(), value2.toStyledString()},
			{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalTakeGold2), value1.toStyledString()},
			{"", value5.toStyledString()}
		}, StyledAlignment::kCenterAlignment);
		slttf->setPosition(ccp(0, 45));
		m_container->addChild(slttf);
		
		
		if(mySGD->today_mission_info.ing_count.getV() >= mySGD->today_mission_info.goal_count.getV())
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(kMyLocalKey_todaymissionSuccess), value1.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			under_label->setPosition(ccp(0, -20));
			m_container->addChild(under_label);
		}
		else
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalTakeGold3), value3.toStyledString()},
				{CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalTakeGold1), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str())->getCString(), value4.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			under_label->setPosition(ccp(0, -20));
			m_container->addChild(under_label);
		}
	}
	else if(mission_type == kTodayMissionType_totalCatch)
	{
		StyledLabelTTF* slttf = StyledLabelTTF::create({
			{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalCatch1), value1.toStyledString()},
			{CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalCatch2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.goal_count.getV())->getCString()).c_str())->getCString(), value2.toStyledString()},
			{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalCatch3), value1.toStyledString()},
			{"", value5.toStyledString()}
		}, StyledAlignment::kCenterAlignment);
		slttf->setPosition(ccp(0, 45));
		m_container->addChild(slttf);
		
		
		if(mySGD->today_mission_info.ing_count.getV() >= mySGD->today_mission_info.goal_count.getV())
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(kMyLocalKey_todaymissionSuccess), value1.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			under_label->setPosition(ccp(0, -20));
			m_container->addChild(under_label);
		}
		else
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalCatch4), value3.toStyledString()},
				{CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_todaymissionTotalCatch2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str())->getCString(), value4.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kCenterAlignment);
			under_label->setPosition(ccp(0, -20));
			m_container->addChild(under_label);
		}
	}
	
	CCSprite* progress_back = CCSprite::create("loading_progress_back.png");
	progress_back->setPosition(ccp(0,10));
	m_container->addChild(progress_back);
	
	progress_bar = CCProgressTimer::create(CCSprite::create("loading_progress_front.png"));
	progress_bar->setType(kCCProgressTimerTypeBar);
	progress_bar->setMidpoint(ccp(0,0));
	progress_bar->setBarChangeRate(ccp(1,0));
	progress_bar->setPercentage(100.f*mySGD->today_mission_info.ing_count.getV()/mySGD->today_mission_info.goal_count.getV());
	progress_bar->setPosition(ccp(0, 10));
	progress_bar->setVisible(false);
	m_container->addChild(progress_bar);
	
	
	if(mySGD->today_mission_info.ing_count.getV() >= mySGD->today_mission_info.goal_count.getV())
	{
		CCScale9Sprite* stamp_case = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		stamp_case->setContentSize(CCSizeMake(75, 45));
		stamp_case->setPosition(ccp(100,20));
		stamp_case->setRotation(-15);
		m_container->addChild(stamp_case);
		
		
		KSLabelTTF* complete_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_complete), mySGD->getFont().c_str(), 20);
		complete_label->setColor(ccYELLOW);
		complete_label->setPosition(ccp(37.5f,22.5f));
		stamp_case->addChild(complete_label);
	}
	
	
	KSLabelTTF* close_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
	CCScale9Sprite* close_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	close_button = CCControlButton::create(close_label, close_back);
	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(TodayMissionPopup::closeAction), CCControlEventTouchUpInside);
	close_button->setPreferredSize(CCSizeMake(90,50));
	close_button->setPosition(ccp(0,-70));
	m_container->addChild(close_button);
	
	close_button->setTouchPriority(touch_priority);
	
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t)
										 {
											 KS::setOpacity(m_container, 255);
											 progress_bar->setVisible(true);
											 is_menu_enable = true;
										 }));
}

void TodayMissionPopup::closeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	progress_bar->setVisible(false);
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}