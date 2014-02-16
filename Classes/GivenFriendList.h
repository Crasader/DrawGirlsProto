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
#include "KHAlertView.h"
#include "CommonButton.h"
USING_NS_CC;

using namespace cocos2d::extension;
using namespace std;



#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);

class GivenFriendList : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static GivenFriendList* create()
	{
		GivenFriendList* t_rp = new GivenFriendList();
		t_rp->init();
		t_rp->autorelease();
		return t_rp;
	}

	bool init()
	{
		CCLayer::init();

				
		Json::Value p;
		p["memberID"]=hspConnector::get()->getKakaoID();
		p["type"] = 10; // 비지인 친구만 받겠다.
		p["limitDay"] = mySGD->getMsgRemoveDay();
		// 0 이 아니면 해당하는 타입의 메시지가 들어옴.

		hspConnector::get()->command("getmessagelist",p,this,[this](Json::Value r)
				{
				GraphDogLib::JsonToLog("getmessagelist", r);
				if(r["result"]["code"].asInt() != GDSUCCESS)
				return;
				this->drawMail(r);
				});
		return true;

	}

	void drawMail(Json::Value obj)
	{
		m_filteredMailList = obj["list"];
		m_mailTableView = CCTableView::create(this, CCSizeMake(215, 179));

		CCScale9Sprite* bar = CCScale9Sprite::create("postbox_bar.png");
		m_scrollBar = ScrollBar::createScrollbar(m_mailTableView, -2 - 10, NULL, bar);
		m_scrollBar->setDynamicScrollSize(false);
		m_scrollBar->setVisible(false);
		m_mailTableView->setAnchorPoint(CCPointZero);

		//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
		m_mailTableView->setDirection(kCCScrollViewDirectionVertical);

		//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
		m_mailTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

		//기준점 0,0
		// 좌표 수동으로 잡느라 이리 됨
		m_mailTableView->setPosition(ccp(29, 24));

		//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
		m_mailTableView->setDelegate(this);
		this->addChild(m_mailTableView);
		m_mailTableView->setTouchPriority(-200);
	}

	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx)
	{
		CCTableViewCell* realCell = new CCTableViewCell();
		realCell->init();
		realCell->autorelease();

		
		std::string cellBackFile = "friendsearch_cell.png";
		CCSprite* bg = CCSprite::create(cellBackFile.c_str());
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		realCell->addChild(bg,0);
		
			
		const Json::Value& mail = m_filteredMailList[idx];
		Json::Value contentObj;
		
		Json::Reader reader;
		reader.parse(mail["content"].asString(), contentObj);
		CCLabelTTF* nickFnt = CCLabelTTF::create(contentObj["nick"].asString().c_str(), "", 12);
		realCell->addChild(nickFnt);
		nickFnt->setPosition(ccp(60, 20));
		
			
		CCSprite* profileImg = GDWebSprite::create(contentObj["profile_url"].asString(), "ending_noimg.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setPosition(ccp(22, 21));
		profileImg->setScale(28.f / profileImg->getContentSize().width);

		realCell->addChild(profileImg);

		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setPosition(ccp(0, 0));
		_menu->setTouchPriority(-200);
		realCell->addChild(_menu, 1);
		
		CCMenuItemLambda* accept = CCMenuItemImageLambda::create
			("friendsearch_ok.png", "friendsearch_ok.png",
			 [=](CCObject*)
			 {
				 CCLog("accept");
				 KS::KSLog("%", mail);
				 // 여기는 mailpopup 쪽 알아봄.
				 Json::Value param;
				 //						memberID : string or number, 내카카오아이디
				 //						-> friendID : string or number, 추가할 게임친구 카카오아이디
				 //						-> friendMax :
				 param["memberID"] = hspConnector::get()->getKakaoID();
				 param["friendID"] = mail["friendID"].asString();
				 param["friendMax"] = mySGD->getGameFriendMax(); // magic number
				 hspConnector::get()->command ("addfriendeach", param,
																			 [=](Json::Value v) {
																				 KS::KSLog("%", v);
																				 std::string errorMessage;
																				 /*
																						errorCode 필드에 10030 값이 넘어오면 내친구인원이 초과
																						errorCode 필드에 10031값이 넘어오면 상대방 친구인원이 초과
																						*/
																				 if(v["result"]["code"].asInt() != GDSUCCESS){
																					 return;
																				 }

																				 // 편.삭.
																				 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
																											 [=](Json::Value r) {
																												 if(r["result"]["code"].asInt() != GDSUCCESS) {
																													 KHAlertView* exceptionPopup = KHAlertView::create(); 
																													 exceptionPopup->setTitleFileName("msg_error.png");
																													 exceptionPopup->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
																													 exceptionPopup->setWidth(240);
																													 exceptionPopup->setHeight(240);
																													 exceptionPopup->setTitleHeight(10);
																													 exceptionPopup->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
																													 exceptionPopup->setCenterY(150);

																													 CCNode* emptyNode = CCNode::create();
																													 auto ttf = CCLabelTTF::create("상대방을 추가할 수 없습니다.", mySGD->getFont().c_str(), 14.f); 
																													 ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
																													 //	con->setAnchorPoint(ccp(0, 0));
																													 //ttf->setAnchorPoint(ccp(0.5f, 0.5f));
																													 ttf->setColor(ccc3(255, 255, 255));
																													 ttf->setPosition(ccp(exceptionPopup->getContentRect().size.width / 2.f, -77));
																													 emptyNode->addChild(ttf);
																													 exceptionPopup->setContentNode(
																															 emptyNode
																															 );
																													 exceptionPopup->setContentSize(ttf->getDimensions());
																													 exceptionPopup->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
																																										 [=](CCObject* e) {
																																											 CCLog("ok!!");
																																										 });
																													 addChild(exceptionPopup, 1);
																													 exceptionPopup->show();																																									
																													 return;
																												 }
				 FriendData ufd;
				 ufd.userId = v["friendInfo"]["memberID"].asString();
				 ufd.joinDate = v["friendInfo"]["joinDate"].asUInt64();
				 ufd.lastDate = v["friendInfo"]["lastDate"].asUInt64();
				 ufd.nick = v["friendInfo"]["nick"].asString();
				 UnknownFriends::getInstance()->add(ufd);
											 });

																			 });
			 });
		_menu->addChild(accept);
		accept->setPosition(ccp(140, 22));
		
		
		CCMenuItemLambda* reject = CCMenuItemImageLambda::create("friendsearch_no.png", "friendsearch_no.png",
																														 [=](CCObject*)
																														 {
																															 CCLog("accept");
																															 KS::KSLog("%", mail);
																															 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
																																						 [=](Json::Value r)
																																						 {
																																						 });
																															 // 여기는 mailpopup 쪽 알아봄.
//																															 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(), [=](Json::Value obj)
//																																 {
//
//																																 });
																														 });
		_menu->addChild(reject);
		reject->setPosition(ccp(182, 22));
		return realCell;
	}

	virtual void scrollViewDidScroll(CCScrollView* view)
	{
		if(m_scrollBar)
		{
			m_scrollBar->setBarRefresh();
		}
	}

	virtual void scrollViewDidZoom(CCScrollView* view)
	{
	}
	virtual void tableCellTouched (CCTableView * table, CCTableViewCell * cell)
	{
		CCLog("touch!!");
	}
	virtual CCSize cellSizeForTable (CCTableView * table)
	{
		return CCSizeMake(188, 38);
	}
	virtual unsigned int numberOfCellsInTableView (CCTableView * table)
	{
		int t = m_filteredMailList.size();
		return t;
	}
	void removeMessage(int mailNo, long long memberID, std::function<void(Json::Value)> userFunction)
	{
		Json::Value p;
		//	int mailNo = mail["no"].asInt();
		p["no"] = mailNo;
		p["memberID"] = memberID;
		// 도전장 삭제요청
		hspConnector::get()->command( "removemessage",p, this,[=](Json::Value r) {
		Json::Value newMailList;
		if(r["result"]["code"].asInt() != GDSUCCESS)
		{
			return;
		}
		//테이블에서 없어진것 없애기
		for(int i=0;i<m_filteredMailList.size();i++){
			if(m_filteredMailList[i]["no"].asInt() != mailNo){
				newMailList.append(m_filteredMailList[i]);
			}
		}
		//테이블 리로드
		m_filteredMailList = newMailList;
		m_mailTableView->reloadData();
		if(userFunction != nullptr)
			userFunction(r);
		}
		);
	}
private:
	Json::Value m_filteredMailList; // 걸러진 데이터들
	CCTableView * m_mailTableView;
	ScrollBar * m_scrollBar;
};







