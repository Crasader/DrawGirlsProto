//
//  AttendancePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#include "AttendancePopup.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "StarGoldData.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "StyledLabelTTF.h"
#include "FormSetter.h"
#include "CommonAnimation.h"

AttendancePopup* AttendancePopup::create(int t_touch_priority, function<void()> t_end_func)
{
	AttendancePopup* t_mup = new AttendancePopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void AttendancePopup::myInit(int t_touch_priority, function<void()> t_end_func)
{
	startFormSetter(this);
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
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
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(270,245));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);

	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceTitle), mySGD->getFont().c_str(), 15);
	title_label->enableOuterStroke(ccBLACK,0.5);
	setFormSetter(title_label);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(0,back_case->getContentSize().height/2-22));
	m_container->addChild(title_label);
	
	int dayCount = mySGD->attendance_data["dayCount"].asInt();
	
	CCSprite* show_ani_img;
	
	float distance_x = 50.f;
	for(int i=0;i<7;i++)
	{
		CCPoint check_point;
		if(i < 4)
			check_point = ccp(back_in->getContentSize().width/2.f - distance_x*1.5f + distance_x*i, 165);
		else
			check_point = ccp(back_in->getContentSize().width/2.f - distance_x + distance_x*(i%4), 122);
		
		CCSprite* t_back = CCSprite::create("attendance_day.png");
		t_back->setPosition(check_point);
		back_in->addChild(t_back);
		
		KSLabelTTF* day_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceDay), mySGD->getFont().c_str(), 10);
		day_label->enableOuterStroke(ccBLACK, 0.5f);
		day_label->setPosition(ccp(t_back->getContentSize().width/2.f, 20));
		t_back->addChild(day_label);
		
		KSLabelTTF* day_value = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 20);
		day_value->enableOuterStroke(ccBLACK, 1.f);
		day_value->setPosition(ccp(t_back->getContentSize().width/2.f, 30));
		t_back->addChild(day_value);
		
		CCSprite* present_back = CCSprite::create("attendance_present_back.png");
		present_back->setAnchorPoint(ccp(0,0));
		present_back->setPosition(ccp(t_back->getContentSize().width/2.f+3, t_back->getContentSize().height/2.f-4));
		t_back->addChild(present_back);
		
		string present_type = mySGD->attendance_data["rewardList"][i]["type"].asString();
		if(present_type == mySGD->getGoodsTypeToKey(kGoodsType_gold))
		{
			CCSprite* t_present = CCSprite::create("attendance_gold.png");
			t_present->setPosition(ccp(present_back->getContentSize().width/2.f, present_back->getContentSize().height/2.f));
			present_back->addChild(t_present);
		}
		else if(present_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
		{
			CCSprite* t_present = CCSprite::create("attendance_ruby.png");
			t_present->setPosition(ccp(present_back->getContentSize().width/2.f, present_back->getContentSize().height/2.f));
			present_back->addChild(t_present);
		}
		else
		{
			CCSprite* t_present = CCSprite::create("attendance_present.png");
			t_present->setPosition(ccp(present_back->getContentSize().width/2.f, present_back->getContentSize().height/2.f));
			present_back->addChild(t_present);
		}
		
		if(i+1 < dayCount)
		{
			CCSprite* check_img = CCSprite::create("attendance_day_check.png");
			check_img->setPosition(check_point);
			back_in->addChild(check_img);
		}
		else if(i+1 == dayCount)
		{
			show_ani_img = CCSprite::create("attendance_day_check.png");
			show_ani_img->setPosition(check_point);
			back_in->addChild(show_ani_img);
			show_ani_img->setVisible(false);
		}
	}
	
	
	StyledLabelTTF* slttf = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceContent), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
	setFormSetter(slttf);
	slttf->setPosition(ccp(back_case->getContentSize().width/2.f,80));
	back_in->addChild(slttf);
	
	slttf->setOldAnchorPoint();
	
	
	KSLabelTTF* close_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
	CCScale9Sprite* close_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	close_button = CCControlButton::create(close_label, close_back);
	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(AttendancePopup::closeAction), CCControlEventTouchUpInside);
	close_button->setPreferredSize(CCSizeMake(100,40));
	close_button->setPosition(ccp(back_in->getContentSize().width/2.f,30));
	back_in->addChild(close_button);
	
	close_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		show_ani_img->setOpacity(0);
		show_ani_img->setScale(5.f);
		show_ani_img->setVisible(true);
		
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																					 {
																						 show_ani_img->setScale(5.f-4.f*t);
																						 show_ani_img->setOpacity(255*t);
																					 }, [=](float t)
																					 {
																						 show_ani_img->setScale(1.f);
																						 show_ani_img->setOpacity(255);
																					 }));
		
		is_menu_enable = true;
	});
}

void AttendancePopup::closeAction()
{
	closeAction(NULL, CCControlEventTouchUpInside);
}

void AttendancePopup::closeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	mySGD->resetAttendance();
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
	{
		gray->setOpacity(t);
		KS::setOpacity(m_container, t);
	}, [=](int t)
	{
		gray->setOpacity(0);
		KS::setOpacity(m_container, 0); end_func(); removeFromParent();
	}));
}