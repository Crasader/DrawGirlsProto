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

	KSLabelTTF* main_title = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceTitle), mySGD->getFont().c_str(), 15);
	main_title->setColor(ccc3(255, 170, 20));
	main_title->setAnchorPoint(ccp(0,0));
	main_title->setPosition(ccp(20,295-main_title->getContentSize().height/2.f));
	back_case->addChild(main_title);
	setFormSetter(main_title);
	
	KSLabelTTF* main_content = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceContent), mySGD->getFont().c_str(), 12);
	main_content->setAnchorPoint(ccp(0,0));
	main_content->setPosition(ccp(main_title->getContentSize().width+5, 0));
	main_title->addChild(main_content);
	setFormSetter(main_content);
	
	
	CCScale9Sprite* back_in1 = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in1->setContentSize(CCSizeMake(350, 272));
	back_in1->setPosition(ccp(185, 145));
	back_case->addChild(back_in1);
	setFormSetter(back_in1);
	
	KSLabelTTF* title_label1 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceSeqTitle), mySGD->getFont().c_str(), 15);
	title_label1->enableOuterStroke(ccBLACK,0.5);
	setFormSetter(title_label1);
	title_label1->setColor(ccc3(255, 170, 20));
	title_label1->setAnchorPoint(ccp(0.f,0.f));
	title_label1->setPosition(ccp(14,242));
	back_in1->addChild(title_label1);
	
	KSLabelTTF* content_label1 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceSpecialContent), mySGD->getFont().c_str(), 10);
	content_label1->setAnchorPoint(ccp(0,0.f));
	content_label1->setPosition(ccp(title_label1->getContentSize().width+5, 0));
	title_label1->addChild(content_label1);
	setFormSetter(content_label1);
	
	dayCount = mySGD->attendance_data["dayCount"].asInt();
	
	int reward_list_size = mySGD->attendance_data["rewardList"].size();
	CCPoint base_position = ccp(33,208);
	CCSize cell_size = CCSizeMake(50,60);
	CCPoint d_position = ccp(cell_size.width-3,-cell_size.height+3);
	for(int i=0;i<reward_list_size;i++)
	{
		IntPoint t_offset = IntPoint(i%7, i/7);
		
		CCScale9Sprite* t_back;
		if(mySGD->getGoodsKeyToType(mySGD->attendance_data["rewardList"][i]["type"].asString()) == kGoodsType_ruby)
		{
			t_back = CCScale9Sprite::create("attendance_rubyday.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			t_back->setContentSize(cell_size);
			t_back->setPosition(base_position + ccp(d_position.x*t_offset.x, d_position.y*t_offset.y));
			back_in1->addChild(t_back);
		}
		else
		{
			t_back = CCScale9Sprite::create("rank_normal.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
			t_back->setContentSize(cell_size);
			t_back->setPosition(base_position + ccp(d_position.x*t_offset.x, d_position.y*t_offset.y));
			back_in1->addChild(t_back);
		}
		
		KSLabelTTF* list_number = KSLabelTTF::create(CCString::createWithFormat("%d", i+1)->getCString(), mySGD->getFont().c_str(), 10);
		list_number->setColor(ccc3(255, 170, 20));
		list_number->enableOuterStroke(ccBLACK, 1.f);
		list_number->setAnchorPoint(ccp(0,0));
		list_number->setPosition(ccp(8,42));
		t_back->addChild(list_number);
		
		KSLabelTTF* list_number_d = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceDay), mySGD->getFont().c_str(), 8);
		list_number_d->setColor(ccc3(255, 170, 20));
		list_number_d->enableOuterStroke(ccBLACK, 1.f);
		list_number_d->setAnchorPoint(ccp(0,0));
		list_number_d->setPosition(ccp(list_number->getContentSize().width+2,2));
		list_number->addChild(list_number_d);
		
		
		CCNode* t_goods_node = getGoodsNode(mySGD->attendance_data["rewardList"][i]["type"].asString(), ccp(25,30));
		t_back->addChild(t_goods_node);

		CCSprite* t_bottom_gray = CCSprite::create("attendance_gold_back.png");
		t_bottom_gray->setPosition(ccp(25,12));
		t_back->addChild(t_bottom_gray);
		
		CCNode* bottom_node = getGoodsSmallNodeAndCount(mySGD->attendance_data["rewardList"][i]["type"].asString(), mySGD->attendance_data["rewardList"][i]["count"].asInt());
		bottom_node->setPosition(ccpFromSize(t_bottom_gray->getContentSize()/2.f));
		t_bottom_gray->addChild(bottom_node);
		
//		StyledLabelTTF* script_label = StyledLabelTTF::create(CCString::createWithFormat(getGoodsTypeLocalData(mySGD->attendance_data["rewardList"][i]["type"].asString()).c_str(), mySGD->attendance_data["rewardList"][i]["count"].asInt())->getCString(),
//															  mySGD->getFont().c_str(), 8, 999, StyledAlignment::kCenterAlignment);
//		script_label->setPosition(ccp(30,20));
//		t_back->addChild(script_label);
		
		if(i <= dayCount-1)
		{
			CCSprite* check_img = CCSprite::create("attendance_day_check.png");
			check_img->setScale(0.8f);
			check_img->setPosition(ccpFromSize(cell_size/2.f));
			t_back->addChild(check_img);
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
	
	
	CCScale9Sprite* back_in2 = CCScale9Sprite::create("mainpopup_pupple1.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
	back_in2->setContentSize(CCSizeMake(112, 272));
	back_in2->setPosition(ccp(414, 145));
	back_case->addChild(back_in2);
	setFormSetter(back_in2);
	
	KSLabelTTF* title_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceSpecialTitle), mySGD->getFont().c_str(), 15);
	title_label2->enableOuterStroke(ccBLACK,0.5);
	setFormSetter(title_label2);
	title_label2->setColor(ccc3(255, 170, 20));
	title_label2->setAnchorPoint(ccp(0.5f,0.5f));
	title_label2->setPosition(ccp(back_in2->getContentSize().width/2.f,250));
	back_in2->addChild(title_label2);
	
	CCSprite* plus_img = CCSprite::create("attendance_plus.png");
	plus_img->setPosition(ccp(87,262));
	back_in2->addChild(plus_img);
	setFormSetter(plus_img);
	
	int day_list_size = mySGD->attendance_data["dayList"].size();
	
	base_position = ccp(back_in2->getContentSize().width/2.f,208);
	d_position = ccp(0,-60+3);
	
	for(int i=0;i<day_list_size;i++)
	{
		CCSprite* t_back = CCSprite::create("attendance_day_tag.png");
		t_back->setPosition(base_position + d_position*i);
		back_in2->addChild(t_back);
		
		KSLabelTTF* day_label = KSLabelTTF::create(graphdog->dateFormat("m/d", mySGD->attendance_data["dayList"][i]["startDate"].asString()).c_str(), mySGD->getFont().c_str(), 10);
		day_label->enableOuterStroke(ccBLACK, 1.f);
		day_label->setPosition(ccp(20,45.5f));
		t_back->addChild(day_label);
		
		Json::Value reward_list = mySGD->attendance_data["dayList"][i]["reward"];
		
		if(reward_list.size() >= 2)
		{
			CCNode* t_goods_node = getGoodsNode("many", ccp(t_back->getContentSize().width/2.f,30));
			t_back->addChild(t_goods_node);
			
			CCNode* bottom_node = getGoodsSmallNodeAndCount("many", 0);
			bottom_node->setPosition(ccp(t_back->getContentSize().width/2.f,13));
			t_back->addChild(bottom_node);
		}
		else
		{
			CCNode* t_goods_node = getGoodsNode(reward_list[0]["type"].asString(), ccp(t_back->getContentSize().width/2.f,30));
			t_back->addChild(t_goods_node);
			
			CCNode* bottom_node = getGoodsSmallNodeAndCount(reward_list[0]["type"].asString(), reward_list[0]["count"].asInt());
			bottom_node->setPosition(ccp(t_back->getContentSize().width/2.f,13));
			t_back->addChild(bottom_node);
		}
		
		if(graphdog->dateFormat("Ymd", mySGD->attendance_data["dayList"][i]["startDate"].asString()) == graphdog->dateFormat("Ymd", graphdog->getDate()))
		{
			CCSprite* check_img = CCSprite::create("attendance_day_check.png");
			check_img->setPosition(ccpFromSize(t_back->getContentSize()/2.f));
			t_back->addChild(check_img);
		}
	}
	
	
	

	
//	KSLabelTTF* content_label2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceSpecialContent), mySGD->getFont().c_str(), 10);
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
		
		is_menu_enable = true;
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

CCNode* AttendancePopup::getGoodsNode(string t_type, CCPoint base_position)
{
	GoodsType t_goods = mySGD->getGoodsKeyToType(t_type);

	CCNode* return_node = NULL;
	if(t_goods == kGoodsType_gold)
	{
		return_node = CCSprite::create("shop_gold2.png");
		return_node->setScale(0.7f);
		return_node->setPosition(base_position + ccp(0,-10));
	}
	else if(t_goods == kGoodsType_ruby)
	{
		return_node = CCSprite::create("shop_ruby2.png");
		return_node->setScale(0.7f);
		return_node->setPosition(base_position + ccp(0,-10));
	}
	else if(t_goods == kGoodsType_item9)
	{
		return_node = CCSprite::create("item9.png");
		return_node->setScale(0.55f);
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_item6)
	{
		return_node = CCSprite::create("item6.png");
		return_node->setScale(0.55f);
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_item11)
	{
		return_node = CCSprite::create("item11.png");
		return_node->setScale(0.55f);
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_pass1)
	{
		return_node = CCSprite::create("pass_ticket1.png");
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_pass2)
	{
		return_node = CCSprite::create("pass_ticket2.png");
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_pass3)
	{
		return_node = CCSprite::create("pass_ticket3.png");
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_pass4)
	{
		return_node = CCSprite::create("pass_ticket4.png");
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_pass5)
	{
		return_node = CCSprite::create("pass_ticket5.png");
		return_node->setPosition(base_position);
	}
	else if(t_goods == kGoodsType_pass6)
	{
		return_node = CCSprite::create("pass_ticket6.png");
		return_node->setPosition(base_position);
	}
	else
	{
		return_node = CCSprite::create("icon_box.png");
		return_node->setPosition(base_position);
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
		KSLabelTTF* count_label = KSLabelTTF::create((KS::insert_separator(ccsf("%d", t_count)) + myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeItem9)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(count_label);
	}
	else if(t_goods == kGoodsType_item6)
	{
		KSLabelTTF* count_label = KSLabelTTF::create((KS::insert_separator(ccsf("%d", t_count)) + myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeItem6)).c_str(), mySGD->getFont().c_str(), 10);
		return_node->addChild(count_label);
	}
	else if(t_goods == kGoodsType_item11)
	{
		KSLabelTTF* count_label = KSLabelTTF::create((KS::insert_separator(ccsf("%d", t_count)) + myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeItem11)).c_str(), mySGD->getFont().c_str(), 10);
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
	else
	{
		KSLabelTTF* count_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_attendanceGoodsTypeMany), mySGD->getFont().c_str(), 10);
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