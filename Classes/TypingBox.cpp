//
//  TypingBox.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 7. 23..
//
//

#include "TypingBox.h"
#include "TouchSuctionLayer.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "AudioEngine.h"
#include "MyLocalization.h"
#include "KSUtil.h"
#include "StyledLabelTTF.h"
#include "DataStorageHub.h"

TypingBox* TypingBox::create(int t_touch_priority, string t_box_filename, CCRect t_box_9_out, CCRect t_box_9_in, CCRect t_box_in_label, CCSize t_box_in_size, CCPoint t_box_position, CCPoint t_button_position)
{
	TypingBox* t_tb = new TypingBox();
	t_tb->ignoreAnchorPointForPosition(false);
	t_tb->myInit(t_touch_priority, t_box_filename, t_box_9_out, t_box_9_in, t_box_in_label, t_box_in_size, t_box_position, t_button_position);
	t_tb->autorelease();
	return t_tb;
}

void TypingBox::myInit(int t_touch_priority, string t_box_filename, CCRect t_box_9_out, CCRect t_box_9_in, CCRect t_box_in_label, CCSize t_box_in_size, CCPoint t_box_position, CCPoint t_button_position)
{
	touch_priority = t_touch_priority;
	box_filename = t_box_filename;
	box_9_out = t_box_9_out;
	box_9_in = t_box_9_in;
	box_in_label = t_box_in_label;
	box_in_size = t_box_in_size;
	box_position = t_box_position;
	
	if(t_button_position.equals(ccp(0,0)))
	{
		button_position = ccp(480,0) + ccp((480.f/myDSH->screen_convert_rate-480.f)/2.f,-myDSH->ui_zero_point.y) + ccp(-56,32.5f);
	}
	else
		button_position = t_button_position;
	
	box_string = "";
	
	is_menu_enable = false;
	is_typing = false;
	end_func = nullptr;
	
	StyledLabelTTF* t_label = StyledLabelTTF::create("", mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
	
	suction = TouchSuctionLayer::create(touch_priority);
	suction->touch_began_func = nullptr;
	suction->is_on_touch_began_func = false;
	addChild(suction);
	suction->setTouchEnabled(true);
	
	box_img = CCScale9Sprite::create(box_filename.c_str(), box_9_out, box_9_in);
	box_img->setContentSize(CCSizeMake(box_in_label.origin.x + box_in_size.width + (box_9_out.size.width-box_in_label.origin.x-box_in_label.size.width), box_in_label.origin.y + box_in_size.height + (box_9_out.size.height-box_in_label.origin.y-box_in_label.size.height)));
	box_img->setPosition(box_position);
	addChild(box_img);
	
	
	typing_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 11);
	typing_label->disableOuterStroke();
	typing_label->setHorizontalAlignment(CCTextAlignment::kCCTextAlignmentLeft);
	typing_label->setVerticalAlignment(CCVerticalTextAlignment::kCCVerticalTextAlignmentTop);
	typing_label->setAnchorPoint(ccp(0,1));
	typing_label->setPosition(ccp(0, 0));
	
	scroll_view = CCScrollView::create(box_in_size);
	scroll_view->setPosition(ccp(box_in_label.origin.x, box_9_out.size.height-box_in_label.origin.y-box_in_label.size.height));
	scroll_view->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
	scroll_view->setContainer(typing_label);
	scroll_view->setContentOffset(ccp(0, scroll_view->minContainerOffset().y));
	scroll_view->setDelegate(this);
	scroll_view->setTouchPriority(touch_priority-1);
	box_img->addChild(scroll_view);
	scroll_view->setTouchEnabled(false);
	
	CCLabelTTF* label_node = CCLabelTTF::create();
	KSLabelTTF* next_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_tutorialNextButton), mySGD->getFont().c_str(), 12);
	label_node->addChild(next_label);
	
	next_button = CCControlButton::create(label_node, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	next_button->addTargetWithActionForControlEvents(this, cccontrol_selector(TypingBox::controlButtonAction), CCControlEventTouchUpInside);
	next_button->setPreferredSize(CCSizeMake(92, 45));
	next_button->setPosition(button_position);
	addChild(next_button);
	next_button->setTouchPriority(touch_priority-2);
	next_button->setVisible(false);
	next_button->setTouchEnabled(false);
}

void TypingBox::setBoxScale(float t_s)
{
	box_img->setScale(t_s);
}

void TypingBox::changeTypingBox(TypingBox* from_tb, TypingBox* to_tb, CCSprite* from_cha, CCSprite* to_cha)
{
	from_cha->setVisible(false);
	to_cha->setVisible(true);
	
	from_tb->setTouchOffScrollAndButton();
	from_tb->setVisible(false);
	
	to_tb->setVisible(true);
	to_tb->setTouchSuction(true);
	from_tb->setTouchSuction(false);
}

void TypingBox::setTouchOffScrollAndButton()
{
	scroll_view->setTouchEnabled(false);
	next_button->setTouchEnabled(false);
	next_button->setVisible(false);
}

void TypingBox::setTouchSuction(bool t_b)
{
	suction->setTouchEnabled(t_b);
}

void TypingBox::setHide()
{
	box_img->setPositionY(-300);
}

void TypingBox::showAnimation(float t_time)
{
	addChild(KSGradualValue<float>::create(0.f, 1.f, t_time, [=](float t)
										   {
											   box_img->setPositionY(-300 + (300+box_position.y)*t);
										   }, [=](float t)
										   {
											   box_img->setPositionY(box_position.y);
										   }));
}

void TypingBox::controlButtonAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	if(end_func != nullptr)
	{
		end_func();
	}
}

void TypingBox::startTyping(string t_box_string, function<void()> t_end_func)
{
	scroll_view->setTouchEnabled(false);
	next_button->setVisible(false);
	next_button->setTouchEnabled(false);
	typing_label->setString("");
	scroll_view->setContentOffset(ccp(0, scroll_view->minContainerOffset().y));
	
	end_func = t_end_func;
	box_string = t_box_string;
	
	basic_string<wchar_t> t_string;
	utf8::utf8to16(box_string.begin(), box_string.end(), back_inserter(t_string));
	text_length = t_string.length();
	
	add_string_frame = 0;
	typing_frame = 0;
	is_typing = true;
	
	suction->touch_began_func = [=]()
	{
		suction->is_on_touch_began_func = false;
		if(is_typing)
		{
			typing_frame = text_length;
			basic_string<wchar_t> t_string;
			utf8::utf8to16(box_string.begin(), box_string.end(), back_inserter(t_string));
			t_string = t_string.substr(0, typing_frame);
			string conver;
			utf8::utf16to8(t_string.begin(), t_string.end(), back_inserter(conver));
			typing_label->setString(conver.c_str());
			
			if(typing_label->getContentSize().height <= box_in_size.height)
			{
				scroll_view->setContentOffset(ccp(0, scroll_view->minContainerOffset().y));
			}
			else
			{
				scroll_view->setContentOffset(ccp(0,0));
			}
			
			stopTyping();
		}
	};
	suction->is_on_touch_began_func = true;
	typing_sound_number = 1;
	schedule(schedule_selector(TypingBox::typing));
}

void TypingBox::stopTyping()
{
	//끝날때도 한번 재생
//	AudioEngine::sharedInstance()->playEffect("sound_crashed_map.mp3", false);
	
	unschedule(schedule_selector(TypingBox::typing));
	is_typing = false;
	suction->is_on_touch_began_func = false;
	suction->touch_began_func = nullptr;
	scroll_view->setTouchEnabled(true);
	next_button->setVisible(true);
	next_button->setTouchEnabled(true);
	is_menu_enable = true;
}

void TypingBox::typing()
{
	add_string_frame++;
	
	if(add_string_frame >= 3)
	{
		add_string_frame = 0;
		
		typing_frame++;
		
		if(typing_frame <= text_length)
		{
			basic_string<wchar_t> t_string;
			
				
			utf8::utf8to16(box_string.begin(), box_string.end(), back_inserter(t_string));
		
			//띄워쓰기나 줄바꿈 있을때 소리를 재생
			if(!(t_string[typing_frame]==' ' || t_string[typing_frame]=='\n'))
			{
				AudioEngine::sharedInstance()->playEffect("se_typing_5.mp3", false);//ccsf("se_typing_%d.mp3", typing_sound_number++), false);
				if(typing_sound_number > 4)
					typing_sound_number = 1;
			}
			
			t_string = t_string.substr(0, typing_frame);
			string conver;
			utf8::utf16to8(t_string.begin(), t_string.end(), back_inserter(conver));
			if(typing_frame < text_length)
				conver = conver + "_";
			typing_label->setString(conver.c_str());
			
			if(typing_label->getContentSize().height <= box_in_size.height)
			{
				scroll_view->setContentOffset(ccp(0, scroll_view->minContainerOffset().y));
			}
			else
			{
				scroll_view->setContentOffset(ccp(0,0));
			}
			
			if(typing_frame == text_length)
			{

				stopTyping();
			}
		}
		else
		{
			stopTyping();
		}
	}
}

void TypingBox::scrollViewDidScroll(CCScrollView* view)
{
	
}
void TypingBox::scrollViewDidZoom(CCScrollView* view)
{
	
}

void TypingBox::keyBackClicked(void)
{
//	onBackKeyAction();
}