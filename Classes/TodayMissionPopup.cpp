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
#include "CommonButton.h"
#include "FormSetter.h"
#include "CommonAnimation.h"
#include "ConvexGraph.h"

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
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	startFormSetter(this);
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_todaymission), mySGD->getFont().c_str(), 12);
	title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251, 113));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-12));
	back_case->addChild(back_in);
	
	
	CCScale9Sprite* in_in = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	in_in->setContentSize(CCSizeMake(236, 46));
	in_in->setPosition(ccp(back_in->getContentSize().width/2.f, 32));
	back_in->addChild(in_in);
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-22,back_case->getContentSize().height/2.f-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  closeAction(sender, CCControlEventTouchUpInside);
							  });
	m_container->addChild(close_button);
	
	KSLabelTTF* normal_stage_only = KSLabelTTF::create(getLocal(LK::kMyLocalKey_todayMissionOnlyNormalStage), mySGD->getFont().c_str(), 9);
	normal_stage_only->setColor(ccYELLOW);
	normal_stage_only->enableOuterStroke(ccBLACK, 0.7f, 180, true);
	normal_stage_only->setPosition(ccpFromSize(back_case->getContentSize()) + ccp(-normal_stage_only->getContentSize().width/2.f-45, -37));
	back_case->addChild(normal_stage_only);
	
	TodayMissionType mission_type = (TodayMissionType)mySGD->today_mission_info.mission_type.getV();
	
	Json::Value value1, value2, value3, value4, value5;
	value1["fillcolor"] = StyledLabelTTF::makeRGB(255, 255, 255);
	value1["font"] = mySGD->getFont();
	value1["size"] = 14.f;
	value1["strokecolor"]=StyledLabelTTF::makeRGB(0, 0, 0);
	value1["strokesize"]=0.5f;
	
	value2["fillcolor"] = StyledLabelTTF::makeRGB(255, 170, 20);
	value2["font"] = mySGD->getFont();
	value2["size"] = 14.f;
	value2["strokecolor"]=StyledLabelTTF::makeRGB(0, 0, 0);
	value2["strokesize"]=0.5f;
	
	value3["fillcolor"] = StyledLabelTTF::makeRGB(255, 255, 255);
	value3["font"] = mySGD->getFont();
	value3["size"] = 13.f;
	value3["strokecolor"]=StyledLabelTTF::makeRGB(0, 0, 0);
	value3["strokesize"]=0.5f;
	
	value4["fillcolor"] = StyledLabelTTF::makeRGB(255, 170, 20);
	value4["font"] = mySGD->getFont();
	value4["size"] = 13.f;
	value4["strokecolor"]=StyledLabelTTF::makeRGB(0, 0, 0);
	value4["strokesize"]=0.5f;
	
	value5["linebreak"] = true;
	value5["linespacing"] = 10.f;
	
	if(mission_type == kTodayMissionType_totalPercent)
	{
//		StyledLabelTTF* slttf = StyledLabelTTF::create({
//			{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalPercent1), value1.toStyledString()},
//			{CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalPercent2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.goal_count.getV())->getCString()).c_str())->getCString(), value2.toStyledString()},
//			{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalPercent3), value1.toStyledString()},
//			{"", value5.toStyledString()}
//		}, StyledAlignment::kCenterAlignment);
//		

		StyledLabelTTF* slttf = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalPercent1),mySGD->today_mission_info.goal_count.getV())->getCString(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		slttf->setOldAnchorPoint();
		slttf->setPosition(ccp(0, 25));
		m_container->addChild(slttf);
		
		
		if(mySGD->today_mission_info.ing_count.getV() < mySGD->today_mission_info.goal_count.getV())
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalPercent4), value3.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kLeftAlignment);
			under_label->setOldAnchorPoint();
			under_label->setPosition(ccp(-100, -27));
			m_container->addChild(under_label);
			
			StyledLabelTTF* under_label2 = StyledLabelTTF::create({
				{CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalPercent2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str())->getCString(), value4.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kRightAlignment);
			under_label2->setOldAnchorPoint();
			under_label2->setPosition(ccp(100, -27));
			m_container->addChild(under_label2);
		}
	}
	else if(mission_type == kTodayMissionType_totalScore)
	{
//		StyledLabelTTF* slttf = StyledLabelTTF::create({
//			{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalScore1), value1.toStyledString()},
//			{CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalScore2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.goal_count.getV())->getCString()).c_str())->getCString(), value2.toStyledString()},
//			{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalScore3), value1.toStyledString()},
//			{"", value5.toStyledString()}
//		}, StyledAlignment::kCenterAlignment);
		
		std::string score = KS::insert_separator(ccsf("%d", mySGD->today_mission_info.goal_count.getV()));
		StyledLabelTTF* slttf = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalScore1), score.c_str())->getCString(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		slttf->setOldAnchorPoint();
		slttf->setPosition(ccp(0, 25));
		m_container->addChild(slttf);
		
		
		if(mySGD->today_mission_info.ing_count.getV() < mySGD->today_mission_info.goal_count.getV())
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalScore4), value3.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kLeftAlignment);
			under_label->setOldAnchorPoint();
			under_label->setPosition(ccp(-100, -27));
			m_container->addChild(under_label);
			
			StyledLabelTTF* under_label2 = StyledLabelTTF::create({
				{CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalScore2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str())->getCString(), value4.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kRightAlignment);
			under_label2->setPosition(ccp(100, -27));
			under_label2->setOldAnchorPoint();
			m_container->addChild(under_label2);
		}
	}
	else if(mission_type == kTodayMissionType_totalTakeGold)
	{
//		StyledLabelTTF* slttf = StyledLabelTTF::create({
//			{CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalTakeGold1), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.goal_count.getV())->getCString()).c_str())->getCString(), value2.toStyledString()},
//			{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalTakeGold2), value1.toStyledString()},
//			{"", value5.toStyledString()}
//		}, StyledAlignment::kCenterAlignment);

		std::string number = KS::insert_separator(mySGD->today_mission_info.goal_count.getV());
		StyledLabelTTF* slttf = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalTakeGold1), number.c_str())->getCString(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		slttf->setOldAnchorPoint();
		slttf->setPosition(ccp(0, 25));
		m_container->addChild(slttf);
		
		
		if(mySGD->today_mission_info.ing_count.getV() < mySGD->today_mission_info.goal_count.getV())
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalTakeGold3), value3.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kLeftAlignment);
			under_label->setOldAnchorPoint();
			under_label->setPosition(ccp(-100, -27));
			m_container->addChild(under_label);
			
			StyledLabelTTF* under_label2 = StyledLabelTTF::create({
				{CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalTakeGold2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str())->getCString(), value4.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kRightAlignment);
			under_label2->setOldAnchorPoint();
			under_label2->setPosition(ccp(100, -27));
			m_container->addChild(under_label2);
		}
	}
	else if(mission_type == kTodayMissionType_totalCatch)
	{
//		StyledLabelTTF* slttf = StyledLabelTTF::create({
//			{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalCatch1), value1.toStyledString()},
//			{CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalCatch2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.goal_count.getV())->getCString()).c_str())->getCString(), value2.toStyledString()},
//			{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalCatch3), value1.toStyledString()},
//			{"", value5.toStyledString()}
//		}, StyledAlignment::kCenterAlignment);
		
		StyledLabelTTF* slttf = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalCatch1),mySGD->today_mission_info.goal_count.getV())->getCString(), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
		slttf->setOldAnchorPoint();
		slttf->setPosition(ccp(0, 25));
		m_container->addChild(slttf);
		
		
		if(mySGD->today_mission_info.ing_count.getV() < mySGD->today_mission_info.goal_count.getV())
		{
			StyledLabelTTF* under_label = StyledLabelTTF::create({
				{myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalCatch4), value3.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kLeftAlignment);
			under_label->setOldAnchorPoint();
			under_label->setPosition(ccp(-100, -27));
			m_container->addChild(under_label);
			
			StyledLabelTTF* under_label2 = StyledLabelTTF::create({
				{CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionTotalCatch2), KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str())->getCString(), value4.toStyledString()},
				{"", value5.toStyledString()}
			}, StyledAlignment::kRightAlignment);
			under_label2->setPosition(ccp(100, -27));
			under_label2->setOldAnchorPoint();
			m_container->addChild(under_label2);
		}
	}
	
	progress_bar = ConvexGraph::create("loading_progress_front2.png", CCRectMake(0, 0, 13, 13), CCRectMake(6, 6, 1, 1), CCSizeMake(201, 13), ConvexGraphType::width);
	progress_bar->setPosition(ccp(0,3));
	m_container->addChild(progress_bar);
	
	progress_bar->setCover("loading_progress_front1.png", "loading_progress_mask.png");
	progress_bar->setBack("loading_progress_back.png");
	
	progress_bar->setPercentage(100.f*mySGD->today_mission_info.ing_count.getV()/mySGD->today_mission_info.goal_count.getV());
	
//	CCSprite* progress_back = CCSprite::create("loading_progress_back.png");
//	progress_back->setPosition(ccp(0,3));
//	m_container->addChild(progress_back);
//	
//	progress_bar = CCProgressTimer::create(CCSprite::create("loading_progress_front.png"));
//	progress_bar->setType(kCCProgressTimerTypeBar);
//	progress_bar->setMidpoint(ccp(0,0));
//	progress_bar->setBarChangeRate(ccp(1,0));
//	progress_bar->setPercentage(100.f*mySGD->today_mission_info.ing_count.getV()/mySGD->today_mission_info.goal_count.getV());
//	progress_bar->setPosition(ccp(0,3));
//	progress_bar->setVisible(false);
//	m_container->addChild(progress_bar);
	
//	KSLabelTTF* progress_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", mySGD->today_mission_info.ing_count.getV())->getCString()).c_str(), mySGD->getFont().c_str(), 15.5f);
//	progress_label->setAnchorPoint(ccp(0,0.5));
//	progress_label->setPosition(ccp(-progress_bar->getSprite()->getContentSize().width/2.f+10, -11));
//	m_container->addChild(progress_label);
	
	CCSprite* present_img = CCSprite::create("today_mission_reward.png");
	present_img->setPosition(ccp(201/2.f, 5));
	m_container->addChild(present_img);
	
	
	if(mySGD->today_mission_info.ing_count.getV() >= mySGD->today_mission_info.goal_count.getV())
	{
		string reward_ment;
		reward_ment = myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionSuccessCommon);
		//		GoodsType reward_type = mySGD->getGoodsKeyToType(mySGD->today_mission_info.reward_type.getV());
		//
		//		if(reward_type == kGoodsType_pass1)
		//			reward_ment = myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionSuccessPass1);
		//		else if(reward_type == kGoodsType_pass2)
		//			reward_ment = myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionSuccessPass2);
		//		else if(reward_type == kGoodsType_pass3)
		//			reward_ment = myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionSuccessPass3);
		//		else if(reward_type == kGoodsType_pass4)
		//			reward_ment = myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionSuccessPass4);
		//		else if(reward_type == kGoodsType_pass5)
		//			reward_ment = myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionSuccessPass5);
		
		StyledLabelTTF* under_label = StyledLabelTTF::create(reward_ment.c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
		setFormSetter(under_label);
		under_label->setPosition(ccp(0, -29.f));
		under_label->setOldAnchorPoint();
		m_container->addChild(under_label);
		
		
		CCScale9Sprite* stamp_case = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		stamp_case->setContentSize(CCSizeMake(75, 45));
		stamp_case->setPosition(ccp(0,7));
		stamp_case->setRotation(-15);
		m_container->addChild(stamp_case);
		
		
		KSLabelTTF* complete_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_complete), mySGD->getFont().c_str(), 18);
		complete_label->setColor(ccYELLOW);
		complete_label->setPosition(ccp(37.5f,22.5f));
		stamp_case->addChild(complete_label);
	}
	else
	{
		CCSprite* line_img = CCSprite::create("cardsetting_line.png");
		line_img->setPosition(ccp(0,-36.5f));
		m_container->addChild(line_img);
		line_img->setScaleX(210/line_img->getContentSize().width);
		
		long long sub_value = mySGD->today_mission_info.resetTimestamp.getV() - graphdog->getServerTimestamp();
		string ment_string;
		if(sub_value >= 3600)
			ment_string = myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionRemainTime);
		else
			ment_string = myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionRemainTimeMinute);
		
		StyledLabelTTF* remain_label = StyledLabelTTF::create(ment_string.c_str(), mySGD->getFont().c_str(), 12 * 4.f, 999, StyledAlignment::kLeftAlignment);
//		remain_label->setScale(1 / 4.f);
		remain_label->setOldAnchorPoint();
		remain_label->setPosition(ccp(-99, -47));
		m_container->addChild(remain_label);
		
		string ment_string2;
		if(sub_value >= 3600)
			ment_string2 = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionRemainTime2), (int)(sub_value/3600.f))->getCString();
		else
			ment_string2 = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionRemainTimeMinute2), (int)(sub_value/60.f))->getCString();
		
		StyledLabelTTF* remain_label2 = StyledLabelTTF::create(ment_string2.c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kRightAlignment);
		remain_label2->setOldAnchorPoint();
		remain_label2->setPosition(ccp(100, -47));
		m_container->addChild(remain_label2);
	}
	
	
//	CCLabelTTF* c_label = CCLabelTTF::create();
//	KSLabelTTF* close_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
//	close_label->setPosition(ccp(0,10));
//	c_label->addChild(close_label);
//	
//	CCScale9Sprite* remain_back = CCScale9Sprite::create("subpop_darkred.png", CCRectMake(0, 0, 30, 30), CCRectMake(14, 14, 2, 2));
//	remain_back->setContentSize(CCSizeMake(120, 30));
//	remain_back->setPosition(ccp(0,-10));
//	c_label->addChild(remain_back);
//	
//	long long sub_value = mySGD->today_mission_info.resetTimestamp.getV() - graphdog->getServerTimestamp();
//	string ment_string;
//	if(sub_value >= 3600)
//		ment_string = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionRemainTime), (int)(sub_value/3600.f))->getCString();
//	else
//		ment_string = CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_todaymissionRemainTimeMinute), (int)(sub_value/60.f))->getCString();
//	
//	StyledLabelTTF* remain_label = StyledLabelTTF::create(ment_string.c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
//	remain_label->setPosition(ccp(remain_back->getContentSize().width/2.f, remain_back->getContentSize().height/2.f));
//	remain_back->addChild(remain_label);
//	
//	CCScale9Sprite* close_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
//	close_button = CCControlButton::create(c_label, close_back);
//	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(TodayMissionPopup::closeAction), CCControlEventTouchUpInside);
//	close_button->setPreferredSize(CCSizeMake(150,55));
//	close_button->setPosition(ccp(0,-70));
//	m_container->addChild(close_button);
//	
//	close_button->setTouchPriority(touch_priority);
	
	CommonAnimation::openPopup(this, m_container, gray,
														 [=](){ // ing Functor
															 
														 },
														 [=](){ // end Functor
//															 progress_bar->setVisible(true);
															 is_menu_enable = true;
														 });
}

void TodayMissionPopup::closeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
//	progress_bar->setVisible(false);
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
}