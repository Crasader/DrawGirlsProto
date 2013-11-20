#pragma once
#include "cocos2d.h"
#include "hspConnector.h"
#include "cocos-ext.h"
#include "KSUtil.h"
#include "CCMenuLambda.h"
#include "GDWebSprite.h"
#include "DataStorageHub.h"
#include <chrono>
#include "ScrollBar.h"
#include "ServerDataSave.h"
#include "StageImgLoader.h"
#include "SilhouetteData.h"
#include "CardCase.h"
#include "DownloadFile.h"
#include "StarGoldData.h"
USING_NS_CC;

using namespace cocos2d::extension;
using namespace std;

enum InvitePopupZorder{
	kInvite_Z_gray = 1,
	kInvite_Z_back,
	kInvite_Z_close,
	kInvite_Z_content,
	kInvite_Z_rankTable,
	kInvite_Z_send,
	kInvite_Z_profileImg,
	kInvite_Z_usedCardImg,
	kInvite_Z_count,
};

enum InviteTableViewTag{
	kInviteTagTitle = 0x1000,
	kInviteTagScore,
	kInviteTagRank,
	kInviteTagSend,
	kInviteTagMenu,
	kInviteTagProfileImg,
};

//enum InvitePopupMenuTag{
//	kRP_MT_close = 0x2000,
//	kRP_MT_invite,
//	kRP_MT_send,
//	kRP_MT_send_close,
//	kRP_MT_invite_close,
//	kRP_MT_invite_rank,
//	kRP_MT_invite_send,
//	kRP_MT_invite_send_close
//};

//enum RankPopupState{
//	kRP_State_rank = 1,
//	kRP_State_send,
//	kRP_State_invite,
//	kRP_State_invite_send
//};

namespace{
	CCSize cellSize3 = CCSizeMake(238, 47);
}
#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);


class InviteTableView : public CCTableView
{
public:
	static InviteTableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
	{
    InviteTableView *table = new InviteTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
		
    return table;
	}
};
class InviteEventPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static InviteEventPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		InviteEventPopup* t_rp = new InviteEventPopup();
		t_rp->myInit(t_close, d_close);
		t_rp->autorelease();
		return t_rp;
	}
	virtual ~InviteEventPopup(){
		StageImgLoader::sharedInstance()->removeTD();
	}
	void finishedOpen(){
		loadRank();
	}
	
	int getInviteIsSendable(std::string userId, int base_s = 60 * 60 * 24 * 31) // 31일.
	{
		auto end = chrono::system_clock::now();
		auto currentSecond = chrono::system_clock::to_time_t(end);
		int ii = myDSH->getUserIntForStr("invite_" + userId, 0);
		if(ii + base_s < currentSecond)
		{
			return 1;
		}
		else
			return 0;
		
		//		if(ii + base_s < GameSystem::getCurrentTime_s())
		//		{
		//			return 1;
		//		}
		//		else
		//			return 0;
	}
	void setInviteSendTime(string userId)
	{
		auto end = chrono::system_clock::now();
		auto currentSecond = chrono::system_clock::to_time_t(end);
		myDSH->setUserIntForStr("invite_" + userId, currentSecond);
		myDSH->setUserIntForStr("invitecount", myDSH->getUserIntForStr("invitecount", 0) + 1);
		//		saveData->setKeyValue(fbid, GameSystem::getCurrentTime_s());
	}
	
	
	void myInit(CCObject* t_close, SEL_CallFunc d_close);
	
	
	
	void loadRank()
	{
		
		
		std::function<void(Json::Value e)> p1 = bind(&ThisClassType::drawRank, this, std::placeholders::_1);
		//step1 카카오친구목록 로드
		hspConnector::get()->kLoadFriends(Json::Value(),[p1](Json::Value fInfo)
																			{
																				CCLog("step1 %s",GraphDogLib::JsonObjectToString(fInfo).c_str());
																				
																				
																				
																				Json::Value appfriends = fInfo["friends_info"];
																				p1(appfriends);
																				
																			});
		
		
	}
	
	
	void drawRank(Json::Value obj){
		m_scoreList = obj;
		//테이블 뷰 생성 시작 /////////////////////////////////////////////////////////////////////////////////////////
		
		//320x320 테이블 뷰 생성
		rankTableView = InviteTableView::create(this, CCSizeMake(227, 233), NULL);
		//		CCScale9Sprite* bar = CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
		//																		1						 CCRectMake(10, 7, 53 - 10*2, 23 - 7*2));
		CCScale9Sprite* bar = CCScale9Sprite::create("card_scroll.png");
		m_scrollBar = ScrollBar::createScrollbar(rankTableView, -2, NULL, bar);
		m_scrollBar->setDynamicScrollSize(false);
		rankTableView->setAnchorPoint(CCPointZero);
		
		//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
		rankTableView->setDirection(kCCScrollViewDirectionVertical);
		
		//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
		rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
		
		//기준점 0,0
		rankTableView->setPosition(ccp(230, 28));
		
		//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
		rankTableView->setDelegate(this);
		this->addChild(rankTableView, kInvite_Z_rankTable);
		rankTableView->setTouchPriority(-200);
		
//		rankTableView->setContentOffsetInDuration(
//																							ccp(
//																									0, yInitPosition)
//																							, 0.7f);
//		int myPosition = rankTableView->minContainerOffset().y;
//		for(int i=0; i<m_scoreList.size(); i++)
//		{
//			if(m_scoreList[i]["user_id"].asString() == hspConnector::get()->getKakaoID())
//			{
//				myPosition = m_scoreList.size() - i - 1;
//				break;
//			}
//		}
//		float yInitPosition = MAX(rankTableView->minContainerOffset().y, -cellSize3.height*myPosition + rankTableView->getViewSize().height / 2.f);
//		yInitPosition = MIN(0, yInitPosition);
		
		//테이블 뷰 생성 끝/////////////////////////////////////////////////////////////////////////////////////////
	}
	
	
	////////////////////////////////////////////////////////
	// touch button ////////////////////////////////////////
	////////////////////////////////////////////////////////
	

	
	
	
	
	
	////////////////////////////////////////////////////////
	// tableview	////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx){
		
		CCLabelTTF* title;
		CCMenuItemLambda* sendBtn;
		CCLabelTTF* score;
		CCLabelTTF* rank;
		Json::Value* member = &m_scoreList[idx];
		KS::KSLog("%", *member);
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		
		
		
		
		std::string cellBackFile = "invite_cell.png";
		
		
		CCSprite* bg = CCSprite::create(cellBackFile.c_str());
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,0);
		
		CCSprite* profileImg = GDWebSprite::create((*member)["profile_image_url"].asString(), "ending_take_particle.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setTag(kInviteTagProfileImg);
		profileImg->setPosition(ccp(27, 22));
		profileImg->setScale(45.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kInvite_Z_profileImg);
		
		
		
		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setPosition(ccp(0, 0));
		_menu->setTouchPriority(-300);
		_menu->setTag(kInviteTagMenu);
		cell->addChild(_menu, kInvite_Z_send);
		

		if(getInviteIsSendable( m_scoreList[idx]["user_id"].asString() ))
		{
			sendBtn = CCMenuItemImageLambda::create
			("rank_cell_invite.png", "rank_cell_invite.png",
			 [=](CCObject* sender){
				 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
				 int idx = (int)obj->getUserData();
				 ////////////////////////////////
				 // 쪽지보내기 - HSP
				 ////////////////////////////////
				 
				 
				 Json::Value p;
				 Json::Value contentJson;
				 
				 contentJson["msg"] = "invite";
				 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
				 //				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
				 p["content"] = GraphDogLib::JsonObjectToString(contentJson);
				 std::string recvId = m_scoreList[idx]["user_id"].asString();
				 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' 제거
				 p["receiverMemberID"] = recvId;
				 p["senderMemberID"]=hspConnector::get()->getKakaoID();
				 p["type"]=kInvite;
				 
				 hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																			{
																				
																				//		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																				//		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																				//		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																				
																				
																				GraphDogLib::JsonToLog("sendMessage", r);
																				
																				obj->removeFromParent();
																				
																				CCMenuItemImageLambda* sendBtn1 = CCMenuItemImageLambda::create("rank_cell_notinvite.png", "rank_cell_notinvite.png",
																																																				[](CCObject*){});
																				sendBtn1->setPosition(ccp(205,22));
																				_menu->addChild(sendBtn1,2);
																				////////////////////////////////
																				// 쪽지보내기 - 카카오
																				////////////////////////////////
																				Json::Value p2;
																				p2["receiver_id"] = m_scoreList[idx]["user_id"].asString();
																				p2["message"] = "초대합니다.";
																				hspConnector::get()->kSendMessage(p2, [=](Json::Value r)
																																					{
																																						GraphDogLib::JsonToLog("kSendMessage", r);
																																						this->setInviteSendTime(m_scoreList[idx]["user_id"].asString());
																																						m_currentInviteCount++;
																																						m_inviteCountFnt->setString(CCString::createWithFormat("%d", m_currentInviteCount)->getCString());
																																						
																																					});
																			});
			 });
		}
		else
		{
			sendBtn = CCMenuItemImageLambda::create("rank_cell_notinvite.png", "rank_cell_notinvite.png",
																							[](CCObject*){});
		}
		
		
		sendBtn->setPosition(ccp(180,22));
		sendBtn->setTag(kInviteTagSend);
		_menu->addChild(sendBtn,2);
		
		title = CCLabelTTF::create("","Helvetica",12);
		title->setPosition(ccp(90,28));
		title->setAnchorPoint(CCPointZero);
		title->setTag(kInviteTagTitle);
		cell->addChild(title,2);
		
		
		score = CCLabelTTF::create("","Helvetica",20);
		score->setPosition(ccp(90,5));
		score->setAnchorPoint(CCPointZero);
		score->setTag(kInviteTagScore);
		cell->addChild(score,2);
		
		rank = CCLabelTTF::create("","Helvetica",25);
		rank->setPosition(ccp(10,10));
		rank->setAnchorPoint(CCPointZero);
		rank->setTag(kInviteTagRank);
		cell->addChild(rank,2);
		
		
		
		
		
		sendBtn->setUserData((void *)idx);
		//sendBtn->setUserData((void *)&member);
		if((*member)["user_id"].asString()==hspConnector::get()->getKakaoID()){
			sendBtn->setVisible(false);
		}else{
			sendBtn->setVisible(true);
		}
		title->setString((*member)["nickname"].asString().c_str());
		score->setString("초대합니다~!");
		//rank->setString((*member)["rankingGrade"].asString().c_str());
		
		return cell;
	}
	
	virtual void scrollViewDidScroll(CCScrollView* view) {
		if(m_scrollBar)
		{
			m_scrollBar->setBarRefresh();
		}
	}
	
	virtual void scrollViewDidZoom(CCScrollView* view) {
		
	}
	
	CCSprite* used_card_img;
	int loading_card_number;
	int after_loading_card_number;
	int last_selected_card_number;
	vector<DownloadFile> df_list;
	vector<CopyFile> cf_list;
	
	
	
	int ing_download_cnt;
	
	
	
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){
		
		
		
	}
	virtual CCSize cellSizeForTable(CCTableView *table){
		return cellSize3;
	}
	
	virtual unsigned int numberOfCellsInTableView(CCTableView *table){
		return m_scoreList.size();
	}
	
	
	void touchFriend(CCObject*, CCControlEvent){
		
		
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		
		return return_value;
	}
	
	void menuAction(CCObject* sender)
	{
		//		if(!is_menu_enable)
		//			return;
		//
		//		is_menu_enable = false;
		//
		//		int tag = ((CCNode*)sender)->getTag();
		//
		//		if(tag == kRP_MT_close)
		//		{
		//			(target_close->*delegate_close)();
		//			removeFromParent();
		//		}
		//		else if(tag == kRP_MT_invite)
		//		{
		//			touched_number = -1;
		////			back->removeFromParent();
		////
		////			back = CCSprite::create("rank_invite_back.png");
		////			back->setPosition(ccp(240,160));
		////			addChild(back, kRP_Z_back);
		////
		////			CCSprite* n_invite_close = CCSprite::create("ui_common_close.png");
		////			CCSprite* s_invite_close = CCSprite::create("ui_common_close.png");
		////			s_invite_close->setColor(ccGRAY);
		////
		////			CCMenuItem* invite_close_item = CCMenuItemSprite::create(n_invite_close, s_invite_close, this, menu_selector(RankPopup::menuAction));
		////			invite_close_item->setTag(kRP_MT_invite_close);
		////
		////			invite_close_menu = CCMenu::createWithItem(invite_close_item);
		////			invite_close_menu->setPosition(getContentPosition(kRP_MT_invite_close));
		////			back->addChild(invite_close_menu);
		////
		////
		////			CCSprite* n_invite_rank = CCSprite::create("rank_invite_rank.png");
		////			CCSprite* s_invite_rank = CCSprite::create("rank_invite_rank.png");
		////			s_invite_rank->setColor(ccGRAY);
		////
		////			CCMenuItem* invite_rank_item = CCMenuItemSprite::create(n_invite_rank, s_invite_rank, this, menu_selector(RankPopup::menuAction));
		////			invite_rank_item->setTag(kRP_MT_invite_rank);
		////
		////			invite_rank_menu = CCMenu::createWithItem(invite_rank_item);
		////			invite_rank_menu->setPosition(getContentPosition(kRP_MT_invite_rank));
		////			back->addChild(invite_rank_menu);
		////
		////
		////			CCSprite* n_invite_send = CCSprite::create("rank_invite_send.png");
		////			CCSprite* s_invite_send = CCSprite::create("rank_invite_send.png");
		////			s_invite_send->setColor(ccGRAY);
		////
		////			CCMenuItem* invite_send_item = CCMenuItemSprite::create(n_invite_send, s_invite_send, this, menu_selector(RankPopup::menuAction));
		////			invite_send_item->setTag(kRP_MT_invite_send);
		////
		////			invite_send_menu = CCMenu::createWithItem(invite_send_item);
		////			invite_send_menu->setPosition(getContentPosition(kRP_MT_invite_send));
		////			back->addChild(invite_send_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_invite;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_send)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_send_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_send_close = CCSprite::create("option_short_close.png");
		//			CCSprite* s_send_close = CCSprite::create("option_short_close.png");
		//			s_send_close->setColor(ccGRAY);
		//
		//			CCMenuItem* send_close_item = CCMenuItemSprite::create(n_send_close, s_send_close, this, menu_selector(RankPopup::menuAction));
		//			send_close_item->setTag(kRP_MT_send_close);
		//
		//			send_close_menu = CCMenu::createWithItem(send_close_item);
		//			send_close_menu->setPosition(getContentPosition(kRP_MT_send_close));
		//			back->addChild(send_close_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_send;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_invite_rank)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_close = CCSprite::create("ui_common_close.png");
		//			CCSprite* s_close = CCSprite::create("ui_common_close.png");
		//			s_close->setColor(ccGRAY);
		//
		//			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(RankPopup::menuAction));
		//			close_item->setTag(kRP_MT_close);
		//
		//			close_menu = CCMenu::createWithItem(close_item);
		//			close_menu->setPosition(getContentPosition(kRP_MT_close));
		//			back->addChild(close_menu);
		//
		//
		//			CCSprite* n_invite = CCSprite::create("rank_invite.png");
		//			CCSprite* s_invite = CCSprite::create("rank_invite.png");
		//			s_invite->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_item = CCMenuItemSprite::create(n_invite, s_invite, this, menu_selector(RankPopup::menuAction));
		//			invite_item->setTag(kRP_MT_invite);
		//
		//			invite_menu = CCMenu::createWithItem(invite_item);
		//			invite_menu->setPosition(getContentPosition(kRP_MT_invite));
		//			back->addChild(invite_menu);
		//
		//
		//			CCSprite* n_send = CCSprite::create("rank_send.png");
		//			CCSprite* s_send = CCSprite::create("rank_send.png");
		//			s_send->setColor(ccGRAY);
		//
		//			CCMenuItem* send_item = CCMenuItemSprite::create(n_send, s_send, this, menu_selector(RankPopup::menuAction));
		//			send_item->setTag(kRP_MT_send);
		//
		//			send_menu = CCMenu::createWithItem(send_item);
		//			send_menu->setPosition(getContentPosition(kRP_MT_send));
		//			back->addChild(send_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_rank;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_invite_close)
		//		{
		//			(target_close->*delegate_close)();
		//			removeFromParent();
		//		}
		//		else if(tag == kRP_MT_invite_send)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_invite_send_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_invite_send_close = CCSprite::create("option_short_close.png");
		//			CCSprite* s_invite_send_close = CCSprite::create("option_short_close.png");
		//			s_invite_send_close->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_send_close_item = CCMenuItemSprite::create(n_invite_send_close, s_invite_send_close, this, menu_selector(RankPopup::menuAction));
		//			invite_send_close_item->setTag(kRP_MT_invite_send_close);
		//
		//			invite_send_close_menu = CCMenu::createWithItem(invite_send_close_item);
		//			invite_send_close_menu->setPosition(getContentPosition(kRP_MT_invite_send_close));
		//			back->addChild(invite_send_close_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_invite_send;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_send_close)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_close = CCSprite::create("ui_common_close.png");
		//			CCSprite* s_close = CCSprite::create("ui_common_close.png");
		//			s_close->setColor(ccGRAY);
		//
		//			CCMenuItem* close_item = CCMenuItemSprite::create(n_close, s_close, this, menu_selector(RankPopup::menuAction));
		//			close_item->setTag(kRP_MT_close);
		//
		//			close_menu = CCMenu::createWithItem(close_item);
		//			close_menu->setPosition(getContentPosition(kRP_MT_close));
		//			back->addChild(close_menu);
		//
		//
		//			CCSprite* n_invite = CCSprite::create("rank_invite.png");
		//			CCSprite* s_invite = CCSprite::create("rank_invite.png");
		//			s_invite->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_item = CCMenuItemSprite::create(n_invite, s_invite, this, menu_selector(RankPopup::menuAction));
		//			invite_item->setTag(kRP_MT_invite);
		//
		//			invite_menu = CCMenu::createWithItem(invite_item);
		//			invite_menu->setPosition(getContentPosition(kRP_MT_invite));
		//			back->addChild(invite_menu);
		//
		//
		//			CCSprite* n_send = CCSprite::create("rank_send.png");
		//			CCSprite* s_send = CCSprite::create("rank_send.png");
		//			s_send->setColor(ccGRAY);
		//
		//			CCMenuItem* send_item = CCMenuItemSprite::create(n_send, s_send, this, menu_selector(RankPopup::menuAction));
		//			send_item->setTag(kRP_MT_send);
		//
		//			send_menu = CCMenu::createWithItem(send_item);
		//			send_menu->setPosition(getContentPosition(kRP_MT_send));
		//			back->addChild(send_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_rank;
		//			is_menu_enable = true;
		//		}
		//		else if(tag == kRP_MT_invite_send_close)
		//		{
		//			touched_number = -1;
		//			back->removeFromParent();
		//
		//			back = CCSprite::create("rank_invite_back.png");
		//			back->setPosition(ccp(240,160));
		//			addChild(back, kRP_Z_back);
		//
		//			CCSprite* n_invite_close = CCSprite::create("ui_common_close.png");
		//			CCSprite* s_invite_close = CCSprite::create("ui_common_close.png");
		//			s_invite_close->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_close_item = CCMenuItemSprite::create(n_invite_close, s_invite_close, this, menu_selector(RankPopup::menuAction));
		//			invite_close_item->setTag(kRP_MT_invite_close);
		//
		//			invite_close_menu = CCMenu::createWithItem(invite_close_item);
		//			invite_close_menu->setPosition(getContentPosition(kRP_MT_invite_close));
		//			back->addChild(invite_close_menu);
		//
		//
		//			CCSprite* n_invite_rank = CCSprite::create("rank_invite_rank.png");
		//			CCSprite* s_invite_rank = CCSprite::create("rank_invite_rank.png");
		//			s_invite_rank->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_rank_item = CCMenuItemSprite::create(n_invite_rank, s_invite_rank, this, menu_selector(RankPopup::menuAction));
		//			invite_rank_item->setTag(kRP_MT_invite_rank);
		//
		//			invite_rank_menu = CCMenu::createWithItem(invite_rank_item);
		//			invite_rank_menu->setPosition(getContentPosition(kRP_MT_invite_rank));
		//			back->addChild(invite_rank_menu);
		//
		//
		//			CCSprite* n_invite_send = CCSprite::create("rank_invite_send.png");
		//			CCSprite* s_invite_send = CCSprite::create("rank_invite_send.png");
		//			s_invite_send->setColor(ccGRAY);
		//
		//			CCMenuItem* invite_send_item = CCMenuItemSprite::create(n_invite_send, s_invite_send, this, menu_selector(RankPopup::menuAction));
		//			invite_send_item->setTag(kRP_MT_invite_send);
		//
		//			invite_send_menu = CCMenu::createWithItem(invite_send_item);
		//			invite_send_menu->setPosition(getContentPosition(kRP_MT_invite_send));
		//			back->addChild(invite_send_menu);
		//
		//			touched_number = 0;
		//			my_state = kRP_State_invite;
		//			is_menu_enable = true;
		//		}
	}
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		return true;
		
		//		if(touched_number != 0)		return true;
		//		if(my_state == kRP_State_rank)
		//		{
		//			if(close_menu->ccTouchBegan(pTouch, pEvent))				touched_number = kRP_MT_close;
		//			else if(invite_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kRP_MT_invite;
		//			else if(send_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kRP_MT_send;
		//		}
		//		else if(my_state == kRP_State_invite)
		//		{
		//			if(invite_close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kRP_MT_invite_close;
		//			else if(invite_rank_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kRP_MT_invite_rank;
		//			else if(invite_send_menu->ccTouchBegan(pTouch, pEvent))		touched_number = kRP_MT_invite_send;
		//		}
		//		else if(my_state == kRP_State_send)
		//		{
		//			if(send_close_menu->ccTouchBegan(pTouch, pEvent))			touched_number = kRP_MT_send_close;
		//		}
		//		else if(my_state == kRP_State_invite_send)
		//		{
		//			if(invite_send_close_menu->ccTouchBegan(pTouch, pEvent))	touched_number = kRP_MT_invite_send_close;
		//		}
		
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
		//		if(touched_number == kRP_MT_close)							close_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite)					invite_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_send)						send_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite_close)				invite_close_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite_rank)				invite_rank_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite_send)				invite_send_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_send_close)				send_close_menu->ccTouchMoved(pTouch, pEvent);
		//		else if(touched_number == kRP_MT_invite_send_close)			invite_send_close_menu->ccTouchMoved(pTouch, pEvent);
	}
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
		//(target_close->*delegate_close)();
		//removeFromParent();
		
		//		if(touched_number == kRP_MT_close){							close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite){					invite_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_send){						send_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_close){				invite_close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_rank){				invite_rank_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_send){				invite_send_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_send_close){				send_close_menu->ccTouchEnded(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_send_close){		invite_send_close_menu->ccTouchEnded(pTouch, pEvent);	touched_number = 0;	}
	}
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
		//		if(touched_number == kRP_MT_close){							close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite){					invite_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_send){						send_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_close){				invite_close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_rank){				invite_rank_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_send){				invite_send_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_send_close){				send_close_menu->ccTouchCancelled(pTouch, pEvent);		touched_number = 0;	}
		//		else if(touched_number == kRP_MT_invite_send_close){		invite_send_close_menu->ccTouchCancelled(pTouch, pEvent);	touched_number = 0;	}
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
	
protected:
	
	Json::Value m_scoreList;
	
	InviteTableView* rankTableView;
	
	bool is_menu_enable;
	
	int touched_number;
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	CCControlButton *closeBtn;
	
	CCSprite* m_currentSelectSprite;
	ScrollBar* m_scrollBar;
	
	int m_currentInviteCount;
	CCLabelBMFont* m_inviteCountFnt;
};


