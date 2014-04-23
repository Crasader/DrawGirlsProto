//
//  TicketRequestContent.h
//  DGproto
//
//  Created by 사원3 on 2013. 11. 26..
//
//

#ifndef __DGproto__TicketRequestContent__
#define __DGproto__TicketRequestContent__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "StarGoldData.h"
#include "hspConnector.h"
#include "GDWebSprite.h"
#include "KSUtil.h"
#include "SendMessageUtil.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace std;

class TicketFriendInfo
{
public:
	string nickname;
	string img_url;
	std::string user_id;
	bool is_message_blocked;
	
	bool operator==(std::string t_id)
	{
		return user_id == t_id;
	};
};

enum TicketRequestContentMenuTag{
	kTRC_MT_close = 1,
	kTRC_MT_request
};

enum TicketRequestContentCellChildTag{
	kTRC_CCT_check = 1
};

class TicketRequestContent : public CCNode, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static TicketRequestContent* create(int t_touch_priority, int t_puzzle_number)
	{
		TicketRequestContent* n_trc = new TicketRequestContent();
		n_trc->myInit(t_touch_priority, t_puzzle_number);
		n_trc->autorelease();
		return n_trc;
	}
	
	void setRemoveAction(function<void()> t_selector)
	{
		remove_selector = t_selector;
	}
	
private:
	int touch_priority;
	int puzzle_number;
	
	function<void()> remove_selector;
	bool is_menu_enable;
	
	CCMenu* close_menu;
	CCMenuItem* request_item;
	
	CCTableView* friend_table_view;
	
	vector<TicketFriendInfo> checked_friend_list;
	vector<TicketFriendInfo> friend_list;
	
	void menuAction(CCObject* sender)
	{
		if(!is_menu_enable)
			return;
		
		is_menu_enable = false;
		
		int tag = ((CCNode*)sender)->getTag();
		
		if(tag == kTRC_MT_close)
		{
			remove_selector();
		}
		else if(tag == kTRC_MT_request)
		{
			close_menu->setVisible(false);
			// 경수
			
//			p["content"] = GraphDogLib::JsonObjectToString(contentJson);
			
			Json::Value arr;
//			요청 보낼 목록
			for(int i=0;i<checked_friend_list.size();i++)
			{
				arr.append(checked_friend_list[i].user_id);
			}
			Json::Value p;
			Json::Value contentJson;
			contentJson["puzzlenumber"] = puzzle_number;
			contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
			p["receiverMemberIDList"] = arr;
			p["senderMemberID"] = hspConnector::get()->getSocialID();
			p["type"] = kTicketRequest;
			p["content"] = GraphDogLib::JsonObjectToString(contentJson);
			// 성공이든 실패든 콜백 받은 후에 remove_selector(); // popup 닫기임
			hspConnector::get()->command
			("sendmessagebylist", p, [=](Json::Value r)
			 {
				 KS::KSLog("%", r);
				 if(r["result"]["code"].asInt() != GDSUCCESS)
				 {
					 remove_selector();
					 return;
				 }
				 
				 for(auto i : checked_friend_list)
				 {
					 ::setTicketSendTime(i.user_id, puzzle_number);
				 }
				 remove_selector();
			 });
		}
	}
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx)
	{
		TicketFriendInfo* member = &friend_list[idx];
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		
		CCSprite* bg = CCSprite::create("popup2_cell_back.png");
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg);
		
		CCSprite* profileImg = GDWebSprite::create((*member).img_url, "ending_noimg.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setPosition(ccp(65, 22));
		profileImg->setScale(30.f / profileImg->getContentSize().width);
		cell->addChild(profileImg);
		
		CCLabelTTF* nickname_label = CCLabelTTF::create((*member).nickname.c_str(), mySGD->getFont().c_str(), 15);
		nickname_label->setAnchorPoint(ccp(0,0.5));
		nickname_label->setPosition(ccp(85,21));
		cell->addChild(nickname_label);
		
		auto iter = find(checked_friend_list.begin(), checked_friend_list.end(), (*member).user_id);
		
		if(iter != checked_friend_list.end())
		{
			CCSprite* check = CCSprite::create("popup2_cell_check.png");
			check->setPosition(ccp(0, 0));
			check->setAnchorPoint(CCPointZero);
			check->setTag(kTRC_CCT_check);
			cell->addChild(check);
		}
		else
		{
			
		}
		
		cell->setTag(idx);
		
		return cell;
	}
	
	virtual void scrollViewDidScroll(CCScrollView* view)
	{
		
	}
	
    virtual void scrollViewDidZoom(CCScrollView* view)
	{
		
	}
    
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell)
	{
		auto iter = find(checked_friend_list.begin(), checked_friend_list.end(), friend_list[cell->getTag()].user_id);
		if(iter != checked_friend_list.end())
		{
			cell->removeChildByTag(kTRC_CCT_check);
			checked_friend_list.erase(iter);
		}
		else
		{
			CCSprite* check = CCSprite::create("popup2_cell_check.png");
			check->setPosition(ccp(0, 0));
			check->setAnchorPoint(CCPointZero);
			check->setTag(kTRC_CCT_check);
			cell->addChild(check);
			checked_friend_list.push_back(TicketFriendInfo(friend_list[cell->getTag()]));
		}
		
		request_item->setEnabled(checked_friend_list.size() > 0);
	}
    virtual CCSize cellSizeForTable(CCTableView *table)
	{
		return CCSizeMake(220, 45);
	}
	
    virtual unsigned int numberOfCellsInTableView(CCTableView *table)
	{
		return friend_list.size();
	}
	
	void resultLoadFriends(Json::Value result_data)
	{
		CCLog("resultLoadFriends : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
		if(result_data["status"].asInt() == 0)
		{
			Json::Value appfriends = result_data["app_friends_info"];
			
			for(int i=0; i<appfriends.size();i++)
			{
				if(::getIsNotTicketUser(appfriends[i]["user_id"].asString(), puzzle_number))
					continue;
				if(appfriends[i]["message_blocked"].asBool())
					continue;
				TicketFriendInfo t_friend_info;
				t_friend_info.nickname = appfriends[i]["nickname"].asString().c_str();
				t_friend_info.img_url = appfriends[i]["profile_image_url"].asString().c_str();
				t_friend_info.user_id = appfriends[i]["user_id"].asString();
				friend_list.push_back(t_friend_info);
			}
			
			CCSize table_size = CCSizeMake(220, 171);
			
//			CCSprite* temp_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_size.width, table_size.height));
//			temp_back->setOpacity(100);
//			temp_back->setAnchorPoint(CCPointZero);
//			temp_back->setPosition(ccp(-table_size.width/2.f,-table_size.height/2.f));
//			addChild(temp_back);
			
			friend_table_view = CCTableView::create(this, table_size);
			
			friend_table_view->setAnchorPoint(CCPointZero);
			friend_table_view->setDirection(kCCScrollViewDirectionVertical);
			friend_table_view->setVerticalFillOrder(kCCTableViewFillTopDown);
			friend_table_view->setPosition(ccp(-table_size.width/2.f,-table_size.height/2.f));
			
			friend_table_view->setDelegate(this);
			addChild(friend_table_view);
			friend_table_view->setTouchPriority(touch_priority-1);
		}
		else
		{
			
		}
		is_menu_enable = true;
		close_menu->setVisible(true);
	}
	
	void myInit(int t_touch_priority, int t_puzzle_number)
	{
		puzzle_number = t_puzzle_number;
		touch_priority = t_touch_priority;
		is_menu_enable = false;
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
		case_back->setPosition(CCPointZero);
		addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(250, 280));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		content_back->setPosition(ccp(0,0));
		addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(222, 176));
		
		CCLabelTTF* title_label = CCLabelTTF::create("친구 목록", mySGD->getFont().c_str(), 20);
		title_label->setPosition(ccp(0, 118));
		addChild(title_label);
		
		CCSprite* n_close = CCSprite::create("item_buy_popup_close.png");
		CCSprite* s_close = CCSprite::create("item_buy_popup_close.png");
		s_close->setColor(ccGRAY);
		
		CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(TicketRequestContent::menuAction));
		close_item->setTag(kTRC_MT_close);
		
		close_menu = CCMenu::createWithItem(close_item);
		close_menu->setVisible(false);
		close_menu->setTouchPriority(touch_priority-1);
		close_menu->setPosition(ccp(100,120));
		addChild(close_menu);
		
		CCSprite* n_request = CCSprite::create("popup2_ticketrequest.png");
		CCSprite* s_request = CCSprite::create("popup2_ticketrequest.png");
		s_request->setColor(ccGRAY);
		CCSprite* d_request = CCSprite::create("popup2_ticketrequest.png");
		d_request->setColor(ccc3(100,100,100));
		
		request_item = CCMenuItemSprite::create(n_request, s_request, d_request, this, menu_selector(TicketRequestContent::menuAction));
		request_item->setTag(kTRC_MT_request);
		request_item->setEnabled(false);
		
		CCMenu* request_menu = CCMenu::createWithItem(request_item);
		request_menu->setTouchPriority(touch_priority-1);
		request_menu->setPosition(0,-110);
		addChild(request_menu);
		
		
		//hspConnector::get()->kLoadFriends(json_selector(this, TicketRequestContent::resultLoadFriends));
	}
};

#endif /* defined(__DGproto__TicketRequestContent__) */
