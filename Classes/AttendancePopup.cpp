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
#include "AudioEngine.h"
#include "StyledLabelTTF.h"
#include "FormSetter.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
#include "ScrollBar.h"
#include "StyledLabelTTF.h"

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
	back_case->setContentSize(CCSizeMake(480,320));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	setFormSetter(back_case);

	CCScale9Sprite* back_in1 = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in1->setContentSize(CCSizeMake(back_case->getContentSize().width-15, 90));
	back_in1->setPosition(ccp(back_case->getContentSize().width/2.f, 235));
	back_case->addChild(back_in1);
	setFormSetter(back_in1);
	
	KSLabelTTF* title_label1 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceSpecialTitle), mySGD->getFont().c_str(), 15);
	title_label1->enableOuterStroke(ccBLACK,0.5);
	setFormSetter(title_label1);
	title_label1->setColor(ccc3(255, 170, 20));
	title_label1->setAnchorPoint(ccp(0.f,0.5f));
	title_label1->setPosition(ccp(15,290));
	back_case->addChild(title_label1);
	
	KSLabelTTF* content_label1 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceSpecialContent), mySGD->getFont().c_str(), 10);
	content_label1->setAnchorPoint(ccp(0,0.5f));
	content_label1->setPosition(ccp(title_label1->getContentSize().width+5, title_label1->getContentSize().height/2.f));
	title_label1->addChild(content_label1);
	setFormSetter(content_label1);
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 440, 75));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(ccp(0,0));
//	temp_back->setPosition(ccp(back_in1->getContentSize().width/2.f - 220, back_in1->getContentSize().height/2.f - 37.5f));
//	back_in1->addChild(temp_back);
	
	day_table = CCTableView::create(this, CCSizeMake(440, 75));
	day_table->setPosition(ccp(back_in1->getContentSize().width/2.f - 220, back_in1->getContentSize().height/2.f - 37.5f));
	day_table->setDirection(kCCScrollViewDirectionHorizontal);
	day_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	back_in1->addChild(day_table);
	day_table->setTouchPriority(touch_priority);
	
	
	CCScale9Sprite* back_in2 = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in2->setContentSize(CCSizeMake(back_case->getContentSize().width-15, 150));
	back_in2->setPosition(ccp(back_case->getContentSize().width/2.f, 85));
	back_case->addChild(back_in2);
	setFormSetter(back_in2);
	
	KSLabelTTF* title_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceTitle), mySGD->getFont().c_str(), 15);
	title_label2->enableOuterStroke(ccBLACK,0.5);
	setFormSetter(title_label2);
	title_label2->setColor(ccc3(255, 170, 20));
	title_label2->setAnchorPoint(ccp(0.f,0.5f));
	title_label2->setPosition(ccp(15,170));
	back_case->addChild(title_label2);
	
	KSLabelTTF* content_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceSpecialContent), mySGD->getFont().c_str(), 10);
	content_label2->setAnchorPoint(ccp(0,0.5f));
	content_label2->setPosition(ccp(title_label2->getContentSize().width+5, title_label2->getContentSize().height/2.f));
	title_label2->addChild(content_label2);
	setFormSetter(content_label2);
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccpFromSize(back_case->getContentSize()) + ccp(-25,-25));
	back_case->addChild(close_button);
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  mySGD->resetAttendance();
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
								  
								  CommonAnimation::closePopup(this, m_container, gray, [=](){
									  
								  }, [=](){
									  end_func(); removeFromParent();
								  });
							  });
	setFormSetter(close_button);
	
	
	dayCount = mySGD->attendance_data["dayCount"].asInt();
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 434, 130));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(ccp(0,0));
//	temp_back->setPosition(ccp(back_in2->getContentSize().width/2.f - 217, back_in2->getContentSize().height/2.f - 65));
//	back_in2->addChild(temp_back);
	
	CCScale9Sprite* scroll_back = CCScale9Sprite::create("cardsetting_scroll.png", CCRectMake(0, 0, 7, 13), CCRectMake(3, 6, 1, 1));
	scroll_back->setContentSize(CCSizeMake(7, 130-20));
	scroll_back->setPosition(ccpAdd(ccp(back_in2->getContentSize().width/2.f - 200, back_in2->getContentSize().height/2.f - 65), ccp(434-23, 130/2.f)));
	back_in2->addChild(scroll_back);
	
	reward_table = CCTableView::create(this, CCSizeMake(440, 130));
	reward_table->setPosition(ccp(back_in2->getContentSize().width/2.f - 220, back_in2->getContentSize().height/2.f - 65));
	reward_table->setDirection(kCCScrollViewDirectionVertical);
	reward_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	back_in2->addChild(reward_table);
	reward_table->setTouchPriority(touch_priority);
	
	CCScale9Sprite* scrollBar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
													   CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
	ScrollBar* m_scrollBar = ScrollBar::createScrollbar(reward_table, -3, NULL, scrollBar, touch_priority-1);
	m_scrollBar->setDynamicScrollSize(true);
	m_scrollBar->setVisible(true);
	
	
//	CCSprite* show_ani_img;
//	
//	float distance_x = 50.f;
//	for(int i=0;i<7;i++)
//	{
//		CCPoint check_point;
//		if(i < 4)
//			check_point = ccp(back_in->getContentSize().width/2.f - distance_x*1.5f + distance_x*i, 165);
//		else
//			check_point = ccp(back_in->getContentSize().width/2.f - distance_x + distance_x*(i%4), 122);
//		
//		CCSprite* t_back = CCSprite::create("attendance_day.png");
//		t_back->setPosition(check_point);
//		back_in->addChild(t_back);
//		
//		KSLabelTTF* day_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceDay), mySGD->getFont().c_str(), 10);
//		day_label->enableOuterStroke(ccBLACK, 0.5f);
//		day_label->setPosition(ccp(t_back->getContentSize().width/2.f, 20));
//		t_back->addChild(day_label);
//		
//		KSLabelTTF* day_value = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 20);
//		day_value->enableOuterStroke(ccBLACK, 1.f);
//		day_value->setPosition(ccp(t_back->getContentSize().width/2.f, 30));
//		t_back->addChild(day_value);
//		
//		CCSprite* present_back = CCSprite::create("attendance_present_back.png");
//		present_back->setAnchorPoint(ccp(0,0));
//		present_back->setPosition(ccp(t_back->getContentSize().width/2.f+3, t_back->getContentSize().height/2.f-4));
//		t_back->addChild(present_back);
//		
//		string present_type = mySGD->attendance_data["rewardList"][i]["type"].asString();
//		if(present_type == mySGD->getGoodsTypeToKey(kGoodsType_gold))
//		{
//			CCSprite* t_present = CCSprite::create("attendance_gold.png");
//			t_present->setPosition(ccp(present_back->getContentSize().width/2.f, present_back->getContentSize().height/2.f));
//			present_back->addChild(t_present);
//		}
//		else if(present_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
//		{
//			CCSprite* t_present = CCSprite::create("attendance_ruby.png");
//			t_present->setPosition(ccp(present_back->getContentSize().width/2.f, present_back->getContentSize().height/2.f));
//			present_back->addChild(t_present);
//		}
//		else
//		{
//			CCSprite* t_present = CCSprite::create("attendance_present.png");
//			t_present->setPosition(ccp(present_back->getContentSize().width/2.f, present_back->getContentSize().height/2.f));
//			present_back->addChild(t_present);
//		}
//		
//		if(i+1 < dayCount)
//		{
//			CCSprite* check_img = CCSprite::create("attendance_day_check.png");
//			check_img->setPosition(check_point);
//			back_in->addChild(check_img);
//		}
//		else if(i+1 == dayCount)
//		{
//			show_ani_img = CCSprite::create("attendance_day_check.png");
//			show_ani_img->setPosition(check_point);
//			back_in->addChild(show_ani_img);
//			show_ani_img->setVisible(false);
//		}
//	}
	
	
//	StyledLabelTTF* slttf = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceContent), mySGD->getFont().c_str(), 13, 999, StyledAlignment::kCenterAlignment);
//	setFormSetter(slttf);
//	slttf->setPosition(ccp(back_case->getContentSize().width/2.f,80));
//	back_in->addChild(slttf);
//	
//	slttf->setOldAnchorPoint();
//	
//	
//	KSLabelTTF* close_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_ok), mySGD->getFont().c_str(), 13);
//	CCScale9Sprite* close_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
//	close_button = CCControlButton::create(close_label, close_back);
//	close_button->addTargetWithActionForControlEvents(this, cccontrol_selector(AttendancePopup::closeAction), CCControlEventTouchUpInside);
//	close_button->setPreferredSize(CCSizeMake(100,40));
//	close_button->setPosition(ccp(back_in->getContentSize().width/2.f,30));
//	back_in->addChild(close_button);
//	
//	close_button->setTouchPriority(touch_priority);
	
	is_reloaded = false;
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
//		show_ani_img->setOpacity(0);
//		show_ani_img->setScale(5.f);
//		show_ani_img->setVisible(true);
		
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																					 {
																						 if(!is_reloaded)
																							{
																								is_reloaded = true;
																								reward_table->reloadData();
																								day_table->reloadData();
																							}
//																						 show_ani_img->setScale(5.f-4.f*t);
//																						 show_ani_img->setOpacity(255*t);
																					 }, [=](float t)
																					 {
//																						 show_ani_img->setScale(1.f);
//																						 show_ani_img->setOpacity(255);
																						 
																					 }));
		
		is_menu_enable = true;
	});
}

CCTableViewCell* AttendancePopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	if(table == reward_table)
	{
		int cell_count = (mySGD->attendance_data["rewardList"].size()-idx*7)%7;
		if(cell_count == 0)
			cell_count = 7;
		int start_idx = idx*7;
		
		for(int i=start_idx;i<start_idx+cell_count;i++)
		{
			CCScale9Sprite* t_back = CCScale9Sprite::create("rank_normal.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			t_back->setContentSize(CCSizeMake(60, 67));
			t_back->setPosition(ccp(30 + (i-start_idx)*60,32.5f));
			cell->addChild(t_back);
			
			KSLabelTTF* list_number = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 13);
			list_number->setColor(ccc3(255, 170, 20));
			list_number->enableOuterStroke(ccBLACK, 1.f);
			list_number->setPosition(ccp(13,52));
			t_back->addChild(list_number);
			
			CCNode* t_goods_node = getGoodsNode(mySGD->attendance_data["rewardList"][i]["type"].asString());
			t_goods_node->setPosition(ccp(30,24));
			t_back->addChild(t_goods_node);
			
			StyledLabelTTF* script_label = StyledLabelTTF::create(CCString::createWithFormat(getGoodsTypeLocalData(mySGD->attendance_data["rewardList"][i]["type"].asString()).c_str(), mySGD->attendance_data["rewardList"][i]["count"].asInt())->getCString(),
																  mySGD->getFont().c_str(), 8, 999, StyledAlignment::kCenterAlignment);
			script_label->setPosition(ccp(30,20));
			t_back->addChild(script_label);
			
			if(i <= dayCount-1)
			{
				CCSprite* check_img = CCSprite::create("attendance_day_check.png");
				check_img->setPosition(ccp(30,33.5f));
				t_back->addChild(check_img);
			}
		}
	}
	else if(table == day_table)
	{
		CCScale9Sprite* t_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
		t_back->setContentSize(CCSizeMake(60, 75));
		t_back->setPosition(ccp(30,37.5f));
		cell->addChild(t_back);
		
		CCSprite* t_tag = CCSprite::create("attendance_day_tag.png");
		t_tag->setPosition(ccp(27,57));
		t_back->addChild(t_tag);
		
		KSLabelTTF* day_label = KSLabelTTF::create(graphdog->dateFormat("m/d", mySGD->attendance_data["dayList"][idx]["startDate"].asString()).c_str(), mySGD->getFont().c_str(), 12);
		day_label->enableOuterStroke(ccBLACK, 1.f);
		day_label->setPosition(ccpFromSize(t_tag->getContentSize()/2.f) + ccp(-2,1));
		t_tag->addChild(day_label);
		
		Json::Value reward_list = mySGD->attendance_data["dayList"][idx]["reward"];
		
		if(reward_list.size() >= 2)
		{
			CCNode* t_goods_node = getGoodsNode("many");
			t_goods_node->setPosition(ccp(30,24.f));
			t_back->addChild(t_goods_node);
			
			StyledLabelTTF* script_label = StyledLabelTTF::create(CCString::createWithFormat(getGoodsTypeLocalData("many").c_str(), int(reward_list.size()))->getCString(),
																  mySGD->getFont().c_str(), 10, 999, StyledAlignment::kCenterAlignment);
			script_label->setPosition(ccp(30,20));
			t_back->addChild(script_label);
		}
		else
		{
			CCNode* t_goods_node = getGoodsNode(reward_list[0]["type"].asString());
			t_goods_node->setPosition(ccp(30,24.f));
			t_back->addChild(t_goods_node);
			
			StyledLabelTTF* script_label = StyledLabelTTF::create(CCString::createWithFormat(getGoodsTypeLocalData(reward_list[0]["type"].asString()).c_str(), reward_list[0]["count"].asInt())->getCString(),
																  mySGD->getFont().c_str(), 10, 999, StyledAlignment::kCenterAlignment);
			script_label->setPosition(ccp(30,20));
			t_back->addChild(script_label);
		}
	}
	
	return cell;
}

string AttendancePopup::getGoodsTypeLocalData(string t_type)
{
	GoodsType t_goods = mySGD->getGoodsKeyToType(t_type);
	
	string return_value = "";
	if(t_goods == kGoodsType_gold)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeGold);
	}
	else if(t_goods == kGoodsType_ruby)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeRuby);
	}
	else if(t_goods == kGoodsType_item9)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeItem9);
	}
	else if(t_goods == kGoodsType_item6)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeItem6);
	}
	else if(t_goods == kGoodsType_item11)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeItem11);
	}
	else if(t_goods == kGoodsType_pass1)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypePass1);
	}
	else if(t_goods == kGoodsType_pass2)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypePass2);
	}
	else if(t_goods == kGoodsType_pass3)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypePass3);
	}
	else if(t_goods == kGoodsType_pass4)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypePass4);
	}
	else if(t_goods == kGoodsType_pass5)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypePass5);
	}
	else if(t_goods == kGoodsType_pass6)
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypePass6);
	}
	else
	{
		return_value = myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeMany);
	}
	
	return return_value;
}

CCNode* AttendancePopup::getGoodsNode(string t_type)
{
	GoodsType t_goods = mySGD->getGoodsKeyToType(t_type);

	CCNode* return_node = NULL;
	if(t_goods == kGoodsType_gold)
	{
		return_node = CCSprite::create("shop_gold2.png");
		return_node->setScale(0.8f);
	}
	else if(t_goods == kGoodsType_ruby)
	{
		return_node = CCSprite::create("shop_ruby2.png");
	}
	else if(t_goods == kGoodsType_item9)
	{
		return_node = CCSprite::create("item9.png");
	}
	else if(t_goods == kGoodsType_item6)
	{
		return_node = CCSprite::create("item6.png");
	}
	else if(t_goods == kGoodsType_item11)
	{
		return_node = CCSprite::create("item11.png");
	}
	else if(t_goods == kGoodsType_pass1)
	{
		return_node = CCSprite::create("pass_ticket1.png");
	}
	else if(t_goods == kGoodsType_pass2)
	{
		return_node = CCSprite::create("pass_ticket2.png");
	}
	else if(t_goods == kGoodsType_pass3)
	{
		return_node = CCSprite::create("pass_ticket3.png");
	}
	else if(t_goods == kGoodsType_pass4)
	{
		return_node = CCSprite::create("pass_ticket4.png");
	}
	else if(t_goods == kGoodsType_pass5)
	{
		return_node = CCSprite::create("pass_ticket5.png");
	}
	else if(t_goods == kGoodsType_pass6)
	{
		return_node = CCSprite::create("pass_ticket6.png");
	}
	else
	{
		return_node = CCSprite::create("mainflow_etc_event.png");
	}
	
	return return_node;
}

unsigned int AttendancePopup::numberOfCellsInTableView(CCTableView *table)
{
	if(table == reward_table)
		return mySGD->attendance_data["rewardList"].size()/7 + (mySGD->attendance_data["rewardList"].size()%7 > 0 ? 1 : 0);
	else if(table == day_table)
		return mySGD->attendance_data["dayList"].size();
	else
		return 0;
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
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
}