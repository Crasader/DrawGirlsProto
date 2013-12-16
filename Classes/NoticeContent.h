//
//  NoticeContent.h
//  DGproto
//
//  Created by 사원3 on 2013. 12. 16..
//
//

#ifndef __DGproto__NoticeContent__
#define __DGproto__NoticeContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCMenuLambda.h"
#include "Json.h"
#include "GDWebSprite.h"
#include "StarGoldData.h"
#include <chrono>

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

enum NoticeContentMenuTag{
	kNoticeContentMenuTag_ok = 1,
	kNoticeContentMenuTag_check
};

class NoticeContent : public CCNode//, public CCBAnimationManagerDelegate
{
public:
	static NoticeContent* create(int t_touch_priority, function<void(CCObject*)> t_selector, Json::Value t_noti_list)
	{
		NoticeContent* t_ctc = new NoticeContent();
		t_ctc->myInit(t_touch_priority, t_selector, t_noti_list);
		t_ctc->autorelease();
		return t_ctc;
	}
	
private:
	int touch_priority;
	CCMenu* close_menu;
	Json::Value notice_list;
	function<void(CCObject*)> end_selector;
	CCSprite* show_content;
	CCScale9Sprite* case_back;
	CCLabelTTF* title_label;
	CCMenu* check_menu;
	CCSprite* check_img;
	
	bool is_not_show_three_day;
	
	int ing_close_cnt;
	
	bool is_menu_enable;
	
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kNoticeContentMenuTag_ok)
		{
			if(is_not_show_three_day)
			{
				chrono::time_point<chrono::system_clock> chrono_now_time = chrono::system_clock::now();
				time_t now_time = chrono::system_clock::to_time_t(chrono_now_time);
				struct tm* struct_time = localtime(&now_time);
				string time_string = "";
				
				int year = struct_time->tm_year+1900;
				int mon = struct_time->tm_mon+1;
				int day = struct_time->tm_mday;
				
				int base_mon_day;
				
				if(mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12) // 1~31
					base_mon_day = 31;
				else if(mon == 2)
				{
					if(year%4 == 0)
					{
						if(year%100 == 0)
						{
							if(year%400 == 0)
								base_mon_day = 29;
							else
								base_mon_day = 28;
						}
						else
							base_mon_day = 29;
					}
					else
						base_mon_day = 28;
				}
				else // 1~30
					base_mon_day = 30;
				
				if(day+3 > base_mon_day)
				{
					if(mon == 12)
					{
						year++;
						mon = 1;
					}
					else
						mon++;
					day = day+3-base_mon_day;
				}
				else
					day += 3;
				
				time_string += CCString::createWithFormat("%04d", year)->getCString();
				time_string += CCString::createWithFormat("%02d", mon)->getCString();
				time_string += CCString::createWithFormat("%02d", day)->getCString();
				time_string += CCString::createWithFormat("%02d", struct_time->tm_hour)->getCString();
				time_string += CCString::createWithFormat("%02d", struct_time->tm_min)->getCString();
				time_string += CCString::createWithFormat("%02d", struct_time->tm_sec)->getCString();
				
				myDSH->setStringForKey(kDSH_Key_noticeViewDate_int1, notice_list[ing_close_cnt]["no"].asInt(), time_string.c_str());
			}
			
			ing_close_cnt++;
			
			if(ing_close_cnt >= notice_list.size())
			{
				end_selector(NULL);
			}
			else
			{
				is_not_show_three_day = false;
				check_img->setVisible(is_not_show_three_day);
				
				title_label->setString(notice_list[ing_close_cnt]["title"].asString().c_str());
				
				show_content->removeFromParent();
				CCSprite* default_node = CCSprite::create("whitePaper.png", CCRectMake(0,0, notice_list[ing_close_cnt]["imgInfo"]["w"].asInt(), notice_list[ing_close_cnt]["imgInfo"]["h"].asInt()));
				show_content = GDWebSprite::create(notice_list[ing_close_cnt]["imgInfo"]["img"].asString(), default_node, this, callfunc_selector(NoticeContent::loadedAction));
				show_content->setPosition(ccp(-notice_list[ing_close_cnt]["imgInfo"]["w"].asInt()/2.f, -notice_list[ing_close_cnt]["imgInfo"]["h"].asInt()/2.f-16));
				addChild(show_content);
				
				case_back->setContentSize(CCSizeMake(notice_list[ing_close_cnt]["imgInfo"]["w"].asInt() + 17, notice_list[ing_close_cnt]["imgInfo"]["h"].asInt() + 66));
				
				is_menu_enable = true;
			}
		}
		else if(tag == kNoticeContentMenuTag_check)
		{
			is_not_show_three_day = !is_not_show_three_day;
			check_img->setVisible(is_not_show_three_day);
			is_menu_enable = true;
		}
	}
	
	void loadedAction()
	{
		show_content->setPosition(ccp(-show_content->getContentSize().width/2.f, -show_content->getContentSize().height/2.f-16));
	}
	
	void myInit(int t_touch_priority, function<void(CCObject*)> t_selector, Json::Value t_noti_list)
	{
		touch_priority = t_touch_priority;
		end_selector = t_selector;
		
		notice_list = t_noti_list;
		
		case_back = CCScale9Sprite::create("popup3_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
		case_back->setPosition(CCPointZero);
		addChild(case_back);
		
		ing_close_cnt = 0;
		
		case_back->setContentSize(CCSizeMake(notice_list[ing_close_cnt]["imgInfo"]["w"].asInt() + 17, notice_list[ing_close_cnt]["imgInfo"]["h"].asInt() + 66)); // 333 + 17 , 199 + 66
		
		title_label = CCLabelTTF::create(notice_list[ing_close_cnt]["title"].asString().c_str(), mySGD->getFont().c_str(), 15);
		title_label->setPosition(ccp(0, notice_list[ing_close_cnt]["imgInfo"]["h"].asInt()/2.f+11));
		addChild(title_label);
		
		CCSprite* n_close = CCSprite::create("gacha_ok.png");
		CCSprite* s_close = CCSprite::create("gacha_ok.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItemSprite* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(NoticeContent::menuAction));
		close_item->setTag(kNoticeContentMenuTag_ok);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setTouchPriority(touch_priority-1);
		close_menu->setPosition(ccp(0,-notice_list[ing_close_cnt]["imgInfo"]["h"].asInt()/2.f-11));
		addChild(close_menu);
		
		is_not_show_three_day = false;
		
		CCSprite* n_check = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
		CCSprite* s_check = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
		s_check->setColor(ccGRAY);
		
		CCMenuItemSprite* check_item = CCMenuItemSprite::create(n_check, s_check, this, menu_selector(NoticeContent::menuAction));
		check_item->setTag(kNoticeContentMenuTag_check);
		
		check_menu = CCMenu::createWithItem(check_item);
		check_menu->setTouchPriority(touch_priority-1);
		check_menu->setPosition(ccp(-notice_list[ing_close_cnt]["imgInfo"]["w"].asInt()/2.f+30, -notice_list[ing_close_cnt]["imgInfo"]["h"].asInt()/2.f-11));
		addChild(check_menu);
		
		check_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 50, 50));
		check_img->setColor(ccGREEN);
		check_img->setPosition(ccp(-notice_list[ing_close_cnt]["imgInfo"]["w"].asInt()/2.f+30, -notice_list[ing_close_cnt]["imgInfo"]["h"].asInt()/2.f-11));
		addChild(check_img);
		check_img->setVisible(is_not_show_three_day);
		
		CCSprite* default_node = CCSprite::create("whitePaper.png", CCRectMake(0,0, notice_list[ing_close_cnt]["imgInfo"]["w"].asInt(), notice_list[ing_close_cnt]["imgInfo"]["h"].asInt()));
		show_content = GDWebSprite::create(notice_list[ing_close_cnt]["imgInfo"]["img"].asString(), default_node, this, callfunc_selector(NoticeContent::loadedAction));
		show_content->setPosition(ccp(-notice_list[ing_close_cnt]["imgInfo"]["w"].asInt()/2.f, -notice_list[ing_close_cnt]["imgInfo"]["h"].asInt()/2.f-16));
		addChild(show_content);
		
		is_menu_enable = true;
	}
};

#endif /* defined(__DGproto__NoticeContent__) */
