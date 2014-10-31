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
#include "GraphDog.h"

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
	
	back_case = CCSprite::create("attendance_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	setFormSetter(back_case);

	KSLabelTTF* main_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_attendanceTitle), mySGD->getFont().c_str(), 14);
	main_title->disableOuterStroke();
	main_title->setAnchorPoint(ccp(0.5f,0.5f));
	main_title->setPosition(ccp(61,back_case->getContentSize().height-24));
	back_case->addChild(main_title);
	setFormSetter(main_title);
	
	CommonAnimation::applyShadow(main_title);

	
	KSLabelTTF* main_content = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_attendanceContent), mySGD->getFont().c_str(), 10);
	main_content->disableOuterStroke();
	main_content->setAnchorPoint(ccp(0.5f,0.5f));
	main_content->setPosition(ccp(320, back_case->getContentSize().height-33.f));
	back_case->addChild(main_content);
	setFormSetter(main_content);
	
	
	KSLabelTTF* title_label1 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_attendanceSeqTitle), mySGD->getFont().c_str(), 13);
	title_label1->disableOuterStroke();
	setFormSetter(title_label1);
	title_label1->setAnchorPoint(ccp(0.f,0.f));
	title_label1->setPosition(ccp(38,247));
	back_case->addChild(title_label1);
	
	KSLabelTTF* content_label1 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_attendanceSpecialContent), mySGD->getFont().c_str(), 9);
	content_label1->disableOuterStroke();
	content_label1->setAnchorPoint(ccp(0,0.f));
	content_label1->setPosition(ccp(title_label1->getContentSize().width+5, 0));
	title_label1->addChild(content_label1);
	setFormSetter(content_label1);
	
	animation_stemp_list.clear();
	
	dayCount = mySGD->attendance_data["dayCount"].asInt();
	
	int reward_list_size = mySGD->attendance_data["rewardList"].size();
	CCPoint base_position = ccp(57,218);
	CCSize cell_size = CCSizeMake(47,55);
	CCPoint d_position = ccp(cell_size.width-3,-cell_size.height+3);
	for(int i=0;i<reward_list_size;i++)
	{
		IntPoint t_offset = IntPoint(i%7, i/7);
		
		CCScale9Sprite* t_back;
		if(mySGD->getGoodsKeyToType(mySGD->attendance_data["rewardList"][i]["type"].asString()) == kGoodsType_ruby)
		{
			t_back = CCScale9Sprite::create("mainpopup_purple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_back->setContentSize(cell_size + CCSizeMake(-4, -6));
			t_back->setPosition(base_position + ccp(d_position.x*t_offset.x, d_position.y*t_offset.y));
			back_case->addChild(t_back);
		}
		else
		{
			t_back = CCScale9Sprite::create("rank_normal2.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
			t_back->setContentSize(cell_size + CCSizeMake(-4, -6));
			t_back->setPosition(base_position + ccp(d_position.x*t_offset.x, d_position.y*t_offset.y));
			back_case->addChild(t_back);
		}
		

//		KSLabelTTF* list_number_d = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_attendanceDay), mySGD->getFont().c_str(), 8);
//		list_number_d->disableOuterStroke();
//		list_number_d->setAnchorPoint(ccp(0,0));
//		list_number_d->setPosition(ccp(list_number->getContentSize().width+2,0));
//		list_number->addChild(list_number_d);
		
		CCSprite* t_bottom_gray = CCSprite::create("attendance_gold_back.png");
		t_bottom_gray->setPosition(ccp(t_back->getContentSize().width/2.f,10));
		t_back->addChild(t_bottom_gray);
		
		CCNode* t_goods_node = getGoodsNode(mySGD->attendance_data["rewardList"][i]["type"].asString(), ccp(t_back->getContentSize().width/2.f,28));
		t_back->addChild(t_goods_node);
		
		
		string daystring = CCString::createWithFormat("%d%s", i+1,myLoc->getLocalForKey(LK::kMyLocalKey_attendanceDay))->getCString();
		KSLabelTTF* list_number = KSLabelTTF::create(daystring.c_str(), mySGD->getFont().c_str(), 10);
		list_number->disableOuterStroke();
		list_number->setAnchorPoint(ccp(0.5,0));
		list_number->setPosition(ccp(t_bottom_gray->getContentSize().width/2.f,37));
		t_back->addChild(list_number);
		

//		CCSprite* icon_img = getGoodsType(mySGD->attendance_data["rewardList"][i]["type"].asString());
//		t_bottom_gray->addChild(icon_img);
		
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", mySGD->attendance_data["rewardList"][i]["count"].asInt())).c_str(), mySGD->getFont().c_str(), 10);
		t_bottom_gray->addChild(count_label);
		
//		icon_img->setPosition(ccpFromSize(t_bottom_gray->getContentSize()/2.f) + ccp(-count_label->getContentSize().width/2.f, 0));
		count_label->setPosition(ccpFromSize(t_bottom_gray->getContentSize()/2.f) + ccp(0, -1));

		
		
		
//		CCNode* bottom_node = getGoodsSmallNodeAndCount(mySGD->attendance_data["rewardList"][i]["type"].asString(), mySGD->attendance_data["rewardList"][i]["count"].asInt());
//		bottom_node->setPosition(ccpFromSize(t_bottom_gray->getContentSize()/2.f));
//		t_bottom_gray->addChild(bottom_node);
		
//		StyledLabelTTF* script_label = StyledLabelTTF::create(CCString::createWithFormat(getGoodsTypeLocalData(mySGD->attendance_data["rewardList"][i]["type"].asString()).c_str(), mySGD->attendance_data["rewardList"][i]["count"].asInt())->getCString(),
//															  mySGD->getFont().c_str(), 8, 999, StyledAlignment::kCenterAlignment);
//		script_label->setPosition(ccp(30,20));
//		t_back->addChild(script_label);
		
		if(i <= dayCount-1)
		{
			CCSprite* grayback = CCSprite::create("attendance_day_check2.png");
			grayback->setPosition(ccpFromSize(t_back->getContentSize()/2.f));
			t_back->addChild(grayback);
			
			CCSprite* check_img = CCSprite::create("popup_check2.png");
			check_img->setPosition(ccpFromSize(t_back->getContentSize()/2.f));
			t_back->addChild(check_img);
			
			if(mySGD->is_on_attendance && i == dayCount-1)
			{
				check_img->setOpacity(0);
				animation_stemp_list.push_back(check_img);
			}
		}
	}
	
	
	
	
	
//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 440, 75));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(ccp(0,0));
//	temp_back->setPosition(ccp(back_in1->getContentSize().width/2.f - 220, back_in1->getContentSize().height/2.f - 37.5f));
//	back_in1->addChild(temp_back);
	
//	day_table = CCTableView::create(this, CCSizeMake(440, 75));
//	day_table->setPosition(ccp(back_in1->getContentSize().width/2.f - 220, back_in1->getContentSize().height/2.f - 37.5f));
//	day_table->setDirection(kCCScrollViewDirectionHorizontal);
//	day_table->setVerticalFillOrder(kCCTableViewFillTopDown);
//	back_in1->addChild(day_table);
//	day_table->setTouchPriority(touch_priority);
	
	
	KSLabelTTF* title_label2 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_attendanceSpecialTitle), mySGD->getFont().c_str(), 15);
	title_label2->disableOuterStroke();
	setFormSetter(title_label2);
	title_label2->setAnchorPoint(ccp(0.5f,0.5f));
	title_label2->setPosition(ccp(405,256));
	back_case->addChild(title_label2);

	KSLabelTTF* plus_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_plusGive), mySGD->getFont().c_str(), 8);
	plus_label->disableOuterStroke();
	plus_label->setPosition(ccp(434,271));
	back_case->addChild(plus_label);
	
//	CCSprite* plus_img = CCSprite::create("attendance_plus.png");
//	plus_img->setPosition(ccp(87,262));
//	back_case->addChild(plus_img);
//	setFormSetter(plus_img);
	
	int day_list_size = mySGD->attendance_data["dayList"].size();
	
	base_position = ccp(405,210);
	d_position = ccp(0,-70);
	
	for(int i=0;i<day_list_size;i++)
	{
		CCSprite* t_back = CCSprite::create("attendance_specialday.png");
		t_back->setPosition(base_position + d_position*i);
		back_case->addChild(t_back);
		
		KSLabelTTF* day_label = KSLabelTTF::create(GraphDogLib::dateFormat("m/d", mySGD->attendance_data["dayList"][i]["startDate"].asString()).c_str(), mySGD->getFont().c_str(), 10);
		day_label->disableOuterStroke();
		day_label->setPosition(ccp(23,60.f));
		t_back->addChild(day_label);
		
		Json::Value reward_list = mySGD->attendance_data["dayList"][i]["reward"];
		
		if(reward_list.size() >= 2)
		{
			CCNode* t_goods_node = getGoodsNode("many", ccp(t_back->getContentSize().width/2.f,40));
			t_back->addChild(t_goods_node);
			
			CCNode* bottom_node = getGoodsSmallNodeAndCount("many", 0);
			bottom_node->setPosition(ccp(t_back->getContentSize().width/2.f,18));
			t_back->addChild(bottom_node);
		}
		else
		{
			CCNode* t_goods_node = getGoodsNode(reward_list[0]["type"].asString(), ccp(t_back->getContentSize().width/2.f,40));
			t_back->addChild(t_goods_node);
			
			CCNode* bottom_node = getGoodsSmallNodeAndCount(reward_list[0]["type"].asString(), reward_list[0]["count"].asInt());
			bottom_node->setPosition(ccp(t_back->getContentSize().width/2.f,18));
			t_back->addChild(bottom_node);
		}
		
		if(GraphDogLib::dateFormat("Ymd", mySGD->attendance_data["dayList"][i]["startDate"].asString()) == GraphDogLib::dateFormat("Ymd", graphdog->getDate()))
		{
			CCSprite* grayback = CCSprite::create("attendance_specialday_check.png");
			grayback->setPosition(ccpFromSize(t_back->getContentSize()/2.f));
			t_back->addChild(grayback);
			
			CCSprite* check_img = CCSprite::create("popup_check2.png");
			check_img->setPosition(ccpFromSize(t_back->getContentSize()/2.f));
			t_back->addChild(check_img);
			
			if(mySGD->is_on_attendance)
			{
				check_img->setOpacity(0);
				animation_stemp_list.push_back(check_img);
			}
		}
	}
	
	
	

	
//	KSLabelTTF* content_label2 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_attendanceSpecialContent), mySGD->getFont().c_str(), 10);
//	content_label2->setAnchorPoint(ccp(0,0.5f));
//	content_label2->setPosition(ccp(title_label2->getContentSize().width+5, title_label2->getContentSize().height/2.f));
//	title_label2->addChild(content_label2);
//	setFormSetter(content_label2);
	
	
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
	

//	CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 434, 130));
//	temp_back->setOpacity(100);
//	temp_back->setAnchorPoint(ccp(0,0));
//	temp_back->setPosition(ccp(back_in2->getContentSize().width/2.f - 217, back_in2->getContentSize().height/2.f - 65));
//	back_in2->addChild(temp_back);
	
//	CCScale9Sprite* scroll_back = CCScale9Sprite::create("cardsetting_scroll.png", CCRectMake(0, 0, 7, 13), CCRectMake(3, 6, 1, 1));
//	scroll_back->setContentSize(CCSizeMake(7, 130-20));
//	scroll_back->setPosition(ccpAdd(ccp(back_in2->getContentSize().width/2.f - 200, back_in2->getContentSize().height/2.f - 65), ccp(434-23, 130/2.f)));
//	back_in2->addChild(scroll_back);
	
//	reward_table = CCTableView::create(this, CCSizeMake(440, 130));
//	reward_table->setPosition(ccp(back_in2->getContentSize().width/2.f - 220, back_in2->getContentSize().height/2.f - 65));
//	reward_table->setDirection(kCCScrollViewDirectionVertical);
//	reward_table->setVerticalFillOrder(kCCTableViewFillTopDown);
//	back_in2->addChild(reward_table);
//	reward_table->setTouchPriority(touch_priority);
	
//	CCScale9Sprite* scrollBar = CCScale9Sprite::create("cardsetting_scrollbutton.png",
//													   CCRect(0, 0, 12, 33), CCRectMake(5, 5, 3, 20));
//	ScrollBar* m_scrollBar = ScrollBar::createScrollbar(reward_table, -3, NULL, scrollBar, touch_priority-1);
//	m_scrollBar->setDynamicScrollSize(true);
//	m_scrollBar->setVisible(true);
//	
//	is_reloaded = false;
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
		
		for(int i=0;i<animation_stemp_list.size();i++)
		{
			CCSprite* t_stemp = animation_stemp_list[i];
			
			float original_scale = t_stemp->getScale();
			t_stemp->setScale(original_scale+5.f);
			
			if(i==0)
			{
				addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
													   {
														   t_stemp->setScale(original_scale+5.f - 5.f*t);
														   t_stemp->setOpacity(255*t);
													   }, [=](float t)
													   {
														   t_stemp->setScale(original_scale);
														   t_stemp->setOpacity(255);
													   }));
			}
			else
			{
				addChild(KSTimer::create(i*0.3f, [=]()
				{
					addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
														   {
															   t_stemp->setScale(original_scale+5.f - 5.f*t);
															   t_stemp->setOpacity(255*t);
														   }, [=](float t)
														   {
															   t_stemp->setScale(original_scale);
															   t_stemp->setOpacity(255);
														   }));
				}));
			}
		}
		
//		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
//																					 {
//																						 if(!is_reloaded)
//																							{
//																								is_reloaded = true;
//																								reward_table->reloadData();
//																								day_table->reloadData();
//																							}
//																					 }, [=](float t)
//																					 {
//																						 
//																					 }));
	});
}

//CCTableViewCell* AttendancePopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
//{
//	CCTableViewCell* cell = new CCTableViewCell();
//	cell->init();
//	cell->autorelease();
//	
//	if(table == reward_table)
//	{
//		int cell_count = (mySGD->attendance_data["rewardList"].size()-idx*7)%7;
//		if(cell_count == 0)
//			cell_count = 7;
//		int start_idx = idx*7;
//		
//		for(int i=start_idx;i<start_idx+cell_count;i++)
//		{
//			CCScale9Sprite* t_back = CCScale9Sprite::create("rank_normal.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
//			t_back->setContentSize(CCSizeMake(60, 67));
//			t_back->setPosition(ccp(30 + (i-start_idx)*60,32.5f));
//			cell->addChild(t_back);
//			
//			KSLabelTTF* list_number = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 13);
//			list_number->setColor(ccc3(255, 170, 20));
//			list_number->enableOuterStroke(ccBLACK, 1.f);
//			list_number->setPosition(ccp(13,52));
//			t_back->addChild(list_number);
//			
//			CCNode* t_goods_node = getGoodsNode(mySGD->attendance_data["rewardList"][i]["type"].asString());
//			t_goods_node->setPosition(ccp(30,24));
//			t_back->addChild(t_goods_node);
//			
//			StyledLabelTTF* script_label = StyledLabelTTF::create(CCString::createWithFormat(getGoodsTypeLocalData(mySGD->attendance_data["rewardList"][i]["type"].asString()).c_str(), mySGD->attendance_data["rewardList"][i]["count"].asInt())->getCString(),
//																  mySGD->getFont().c_str(), 8, 999, StyledAlignment::kCenterAlignment);
//			script_label->setPosition(ccp(30,20));
//			t_back->addChild(script_label);
//			
//			if(i <= dayCount-1)
//			{
//				CCSprite* check_img = CCSprite::create("attendance_day_check.png");
//				check_img->setPosition(ccp(30,33.5f));
//				t_back->addChild(check_img);
//			}
//		}
//	}
//	else if(table == day_table)
//	{
//		CCScale9Sprite* t_back = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
//		t_back->setContentSize(CCSizeMake(60, 75));
//		t_back->setPosition(ccp(30,37.5f));
//		cell->addChild(t_back);
//		
//		CCSprite* t_tag = CCSprite::create("attendance_day_tag.png");
//		t_tag->setPosition(ccp(27,57));
//		t_back->addChild(t_tag);
//		
//		KSLabelTTF* day_label = KSLabelTTF::create(graphdog->dateFormat("m/d", mySGD->attendance_data["dayList"][idx]["startDate"].asString()).c_str(), mySGD->getFont().c_str(), 12);
//		day_label->enableOuterStroke(ccBLACK, 1.f);
//		day_label->setPosition(ccpFromSize(t_tag->getContentSize()/2.f) + ccp(-2,1));
//		t_tag->addChild(day_label);
//		
//		Json::Value reward_list = mySGD->attendance_data["dayList"][idx]["reward"];
//		
//		if(reward_list.size() >= 2)
//		{
//			CCNode* t_goods_node = getGoodsNode("many");
//			t_goods_node->setPosition(ccp(30,24.f));
//			t_back->addChild(t_goods_node);
//			
//			StyledLabelTTF* script_label = StyledLabelTTF::create(getGoodsTypeLocalData("many").c_str(),
//																  mySGD->getFont().c_str(), 10, 999, StyledAlignment::kCenterAlignment);
//			script_label->setPosition(ccp(30,20));
//			t_back->addChild(script_label);
//		}
//		else
//		{
//			CCNode* t_goods_node = getGoodsNode(reward_list[0]["type"].asString());
//			t_goods_node->setPosition(ccp(30,24.f));
//			t_back->addChild(t_goods_node);
//			
//			StyledLabelTTF* script_label = StyledLabelTTF::create(CCString::createWithFormat(getGoodsTypeLocalData(reward_list[0]["type"].asString()).c_str(), reward_list[0]["count"].asInt())->getCString(),
//																  mySGD->getFont().c_str(), 10, 999, StyledAlignment::kCenterAlignment);
//			script_label->setPosition(ccp(30,20));
//			t_back->addChild(script_label);
//		}
//	}
//	
//	return cell;
//}

string AttendancePopup::getGoodsTypeLocalData(string t_type)
{
	GoodsType t_goods = mySGD->getGoodsKeyToType(t_type);
	
	string return_value = "";
	if(t_goods == kGoodsType_gold)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeGold);
	}
	else if(t_goods == kGoodsType_ruby)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeRuby);
	}
	else if(t_goods == kGoodsType_item9)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeItem9);
	}
	else if(t_goods == kGoodsType_item6)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeItem6);
	}
	else if(t_goods == kGoodsType_item11)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeItem11);
	}
	else if(t_goods == kGoodsType_pass1)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypePass1);
	}
	else if(t_goods == kGoodsType_pass2)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypePass2);
	}
	else if(t_goods == kGoodsType_pass3)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypePass3);
	}
	else if(t_goods == kGoodsType_pass4)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypePass4);
	}
	else if(t_goods == kGoodsType_pass5)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypePass5);
	}
	else if(t_goods == kGoodsType_pass6)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypePass6);
	}
	else if(t_goods == kGoodsType_pass7)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypePass7);
	}
	else if(t_goods == kGoodsType_pass8)
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypePass8);
	}
	else
	{
		return_value = myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeMany);
	}
	
	return return_value;
}

CCNode* AttendancePopup::getGoodsNode(string t_type, CCPoint base_position)
{
	GoodsType t_goods = mySGD->getGoodsKeyToType(t_type);

	CCNode* return_node = NULL;
	if(t_goods == kGoodsType_gold)
	{
		return_node = CCSprite::create("shop_gold2.png");
		return_node->setScale(0.6f);
		return_node->setPosition(base_position + ccp(0,-10));
	}
	else if(t_goods == kGoodsType_ruby)
	{
		return_node = CCSprite::create("shop_ruby2.png");
		return_node->setScale(0.6f);
		return_node->setPosition(base_position + ccp(0,-10));
	}
	else if(t_goods == kGoodsType_item9)
	{
		return_node = CCSprite::create("icon_i9.png");
//		return_node->setScale(0.55f);
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_item6)
	{
		return_node = CCSprite::create("icon_i6.png");
//		return_node->setScale(0.55f);
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_item11)
	{
		return_node = CCSprite::create("icon_i11.png");
//		return_node->setScale(0.55f);
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_pass1)
	{
		return_node = CCSprite::create("pass_ticket1.png");
		return_node->setPosition(base_position + ccp(0,-2));
	}
	else if(t_goods == kGoodsType_pass2)
	{
		return_node = CCSprite::create("pass_ticket2.png");
		return_node->setPosition(base_position + ccp(0,-2));
	}
	else if(t_goods == kGoodsType_pass3)
	{
		return_node = CCSprite::create("pass_ticket3.png");
		return_node->setPosition(base_position + ccp(0,-2));
	}
	else if(t_goods == kGoodsType_pass4)
	{
		return_node = CCSprite::create("pass_ticket4.png");
		return_node->setPosition(base_position + ccp(0,-2));
	}
	else if(t_goods == kGoodsType_pass5)
	{
		return_node = CCSprite::create("pass_ticket5.png");
		return_node->setPosition(base_position + ccp(0,-2));
	}
	else if(t_goods == kGoodsType_pass6)
	{
		return_node = CCSprite::create("pass_ticket6.png");
		return_node->setPosition(base_position + ccp(0,-2));
	}
	else if(t_goods == kGoodsType_pass7)
	{
		return_node = CCSprite::create("icon_p7.png");
		return_node->setPosition(base_position + ccp(0,-2));
	}
	else if(t_goods == kGoodsType_pass8)
	{
		return_node = CCSprite::create("icon_p8.png");
		return_node->setPosition(base_position + ccp(0,-2));
	}
	else if(t_goods == kGoodsType_heart)
	{
		return_node = CCSprite::create("icon_h.png");
		return_node->setPosition(base_position);
	}
	else
	{
		return_node = CCSprite::create("icon_box.png");
		return_node->setPosition(base_position);
	}
	
	return return_node;
}

CCSprite* AttendancePopup::getGoodsType(string t_type)
{
	GoodsType t_goods = mySGD->getGoodsKeyToType(t_type);
	
	CCSprite* return_node = NULL;
	if(t_goods == kGoodsType_gold)
	{
		return_node = CCSprite::create("icon_g.png");
//		return_node->setScale(0.7f);
	}
	else if(t_goods == kGoodsType_ruby)
	{
		return_node = CCSprite::create("icon_r.png");
//		return_node->setScale(0.7f);
	}
	else if(t_goods == kGoodsType_item9)
	{
		return_node = CCSprite::create("icon_i9.png");
//		return_node->setScale(0.55f);
	}
	else if(t_goods == kGoodsType_item6)
	{
		return_node = CCSprite::create("icon_i6.png");
//		return_node->setScale(0.55f);
	}
	else if(t_goods == kGoodsType_item11)
	{
		return_node = CCSprite::create("icon_i11.png");
//		return_node->setScale(0.55f);
	}
	else if(t_goods == kGoodsType_pass1)
	{
		return_node = CCSprite::create("icon_p1.png");
	}
	else if(t_goods == kGoodsType_pass2)
	{
		return_node = CCSprite::create("icon_p2.png");
	}
	else if(t_goods == kGoodsType_pass3)
	{
		return_node = CCSprite::create("icon_p3.png");
	}
	else if(t_goods == kGoodsType_pass4)
	{
		return_node = CCSprite::create("icon_p4.png");
	}
	else if(t_goods == kGoodsType_pass5)
	{
		return_node = CCSprite::create("icon_p5.png");
	}
	else if(t_goods == kGoodsType_pass6)
	{
		return_node = CCSprite::create("icon_p6.png");
	}
	else if(t_goods == kGoodsType_pass7)
	{
		return_node = CCSprite::create("icon_p7.png");
	}
	else if(t_goods == kGoodsType_pass8)
	{
		return_node = CCSprite::create("icon_p8.png");
	}
	else if(t_goods == kGoodsType_heart)
	{
		return_node = CCSprite::create("icon_h.png");
	}
	else
	{
		return_node = CCSprite::create("icon_box.png");
	}
	
	return return_node;
}

CCNode* AttendancePopup::getGoodsSmallNodeAndCount(string t_type, int t_count)
{
	GoodsType t_goods = mySGD->getGoodsKeyToType(t_type);
	
	CCNode* return_node = CCNode::create();
	return_node->setPosition(ccp(0,0));
	if(t_goods == kGoodsType_gold)
	{
		CCSprite* t_icon = CCSprite::create("price_gold_img.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_ruby)
	{
		CCSprite* t_icon = CCSprite::create("price_ruby_img.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_item9)
	{
		KSLabelTTF* count_label = KSLabelTTF::create((KS::insert_separator(ccsf("%d", t_count)) + myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeItem9)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(count_label);
	}
	else if(t_goods == kGoodsType_item6)
	{
		KSLabelTTF* count_label = KSLabelTTF::create((KS::insert_separator(ccsf("%d", t_count)) + myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeItem6)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(count_label);
	}
	else if(t_goods == kGoodsType_item11)
	{
		KSLabelTTF* count_label = KSLabelTTF::create((KS::insert_separator(ccsf("%d", t_count)) + myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeItem11)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(count_label);
	}
	else if(t_goods == kGoodsType_pass1)
	{
		CCSprite* t_icon = CCSprite::create("pass_ticket1.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_pass2)
	{
		CCSprite* t_icon = CCSprite::create("pass_ticket2.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_pass3)
	{
		CCSprite* t_icon = CCSprite::create("pass_ticket3.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_pass4)
	{
		CCSprite* t_icon = CCSprite::create("pass_ticket4.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_pass5)
	{
		CCSprite* t_icon = CCSprite::create("pass_ticket5.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_pass6)
	{
		CCSprite* t_icon = CCSprite::create("pass_ticket6.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_pass7)
	{
		CCSprite* t_icon = CCSprite::create("icon_p7.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else if(t_goods == kGoodsType_pass8)
	{
		CCSprite* t_icon = CCSprite::create("icon_p8.png");
		t_icon->setScale(0.6f);
		KSLabelTTF* count_label = KSLabelTTF::create(KS::insert_separator(ccsf("%d", t_count)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(t_icon);
		return_node->addChild(count_label);
		t_icon->setPosition(ccp(-count_label->getContentSize().width/2.f-3, 0));
		count_label->setPosition(ccp(t_icon->getContentSize().width*t_icon->getScale()/2.f-3, 0));
	}
	else
	{
		KSLabelTTF* count_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_attendanceGoodsTypeMany), mySGD->getFont().c_str(), 10);
		return_node->addChild(count_label);
	}
	
	return return_node;
}

//unsigned int AttendancePopup::numberOfCellsInTableView(CCTableView *table)
//{
//	if(table == reward_table)
//		return mySGD->attendance_data["rewardList"].size()/7 + (mySGD->attendance_data["rewardList"].size()%7 > 0 ? 1 : 0);
//	else if(table == day_table)
//		return mySGD->attendance_data["dayList"].size();
//	else
//		return 0;
//}

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