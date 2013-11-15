#pragma once

#include "cocos2d.h"
#include "hspConnector.h"
#include "cocos-ext.h"
#include "DataStorageHub.h"
#include "GDWebSprite.h"
#include "ServerDataSave.h"
#include "CCMenuLambda.h"

#include "KSUtil.h"
#include "GDWebSprite.h"
#include "EnumDefine.h"
#include "ScrollBar.h"
USING_NS_CC;

using namespace cocos2d::extension;
using namespace std;

enum MailPopupZorder{
	kMP_Z_gray = 1,
	kMP_Z_back,
	kMP_Z_close,
	kMP_Z_content,
	kMP_Z_mailTable,
	kMP_Z_profileImg
};

enum MailTableViewTag{
	kMP_MT_title = 1,
	kMP_MT_score,
	kMP_MT_rank,
	kMP_MT_send,
	kMP_MT_getheart,
	kMP_MT_profileImg
};

enum MailPopupMenuTag{
	kMP_MT_close = 1,
	kMP_MT_invite,
	kMP_MT_send_close,
	kMP_MT_invite_close,
	kMP_MT_invite_rank,
	kMP_MT_invite_send,
	kMP_MT_invite_send_close
};


namespace {
	CCSize mailCellSize = CCSizeMake(238, 47);
}
class MailPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static MailPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		MailPopup* t_rp = new MailPopup();
		t_rp->myInit(t_close, d_close);
		t_rp->autorelease();
		return t_rp;
	}
	
	void finishedOpen(){
		loadMail();
	}
	
	void finishedClose(){
		
		(target_close->*delegate_close)();
		removeFromParent();
	}
	
		
	

	
	void myInit(CCObject* t_close, SEL_CallFunc d_close)
	{
		target_close = t_close;
		delegate_close = d_close;
		
		CCSprite* back = CCSprite::create("postbox_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kMP_Z_back);
		
		


		
		setTouchEnabled(true);
		
		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setTouchPriority(-200);
		back->addChild(_menu);
		_menu->setPosition(ccp(0, 0));
		
		CCMenuItemLambda* closeBtn = CCMenuItemImageLambda::create(
																															 "cardsetting_close.png", "cardsetting_close.png",
																															 [=](CCObject*){
																																 (target_close->*delegate_close)();
																																 removeFromParent();
																																 
																															 });
		closeBtn->setPosition(ccp(325, 290));
		_menu->addChild(closeBtn, kMP_Z_close);
		
		loadMail();
	}
	
	
	
	void loadMail()
	{
		Json::Value p;
		p["memberID"]=hspConnector::get()->getKakaoID();
		p["type"]=0; // 모든 타입의 메시지를 받겠다는 뜻.
		// 0 이 아니면 해당하는 타입의 메시지가 들어옴.
		
		hspConnector::get()->command("getmessagelist",p,[this](Json::Value r)
																 {
																	 GraphDogLib::JsonToLog("getmessagelist", r);
																	 this->drawMail(r);
																 });
	}
	
	
	void drawMail(Json::Value obj){
		
		m_mailList=obj["list"];
		hspConnector::get()->kLoadFriends(Json::Value(),[=](Json::Value fInfo)
					{
						CCLog("step1 %s",GraphDogLib::JsonObjectToString(fInfo).c_str());
						auto app_friends = fInfo["app_friends_info"];
						Json::Value userIdKeyValue;
						// m_mailList 와 app_friends 를 합쳐야됨.
						//
						for(int i=0; i<app_friends.size(); i++)
						{
							userIdKeyValue[app_friends[i]["user_id"].asString()] =
								app_friends[i];
						}
						KS::KSLog("%", userIdKeyValue);
						KS::KSLog("%", m_mailList);
						for(int i=0; i<m_mailList.size(); i++)
						{
							std::string user_id = m_mailList[i]["friendID"].asString();
							m_mailList[i]["nickname"] = userIdKeyValue[user_id]["nickname"];
							m_mailList[i]["profile_image_url"] = userIdKeyValue[user_id]["profile_image_url"];
						}

						//테이블 뷰 생성 시작 /////////////////////////////////////////////////////////////////////////////////////////
						
						//320x320 테이블 뷰 생성
						mailTableView = CCTableView::create(this, CCSizeMake(244.f, 222.f));
						
						CCScale9Sprite* bar = CCScale9Sprite::create("card_scroll.png");
						m_scrollBar = ScrollBar::createScrollbar(mailTableView, -2, NULL, bar);
						m_scrollBar->setDynamicScrollSize(false);
						
						mailTableView->setAnchorPoint(CCPointZero);
						
						//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
						mailTableView->setDirection(kCCScrollViewDirectionVertical);
						
						//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
						mailTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
						
						//기준점 0,0
						// 좌표 수동으로 잡느라 이리 됨
						mailTableView->setPosition(ccp(159/2.f, 39/2.f) + ccp(159/2.f, 39/2.f - 4.f));
						
						//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
						mailTableView->setDelegate(this);
						this->addChild(mailTableView, kMP_Z_mailTable);
						mailTableView->setTouchPriority(-200);
						//테이블 뷰 생성 끝/////////////////////////////////////////////////////////////////////////////////////////
					});
	}
	
	
	////////////////////////////////////////////////////////
	// touch button ////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	void closePopup(CCControlButton *obj, CCControlEvent event){
		this->runAction(CCSequence::create(CCMoveBy::create(0.5, CCPoint(0,-400)),CCCallFunc::create(this, callfunc_selector(MailPopup::finishedClose)),NULL));
	}
	
	
	
	void removeMail(CCObject* _obj){
		CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(_obj);
		int idx = (int)obj->getUserData();
		
		Json::Value p;
		int mailNo =m_mailList[idx]["no"].asInt();
		
		p["no"]=mailNo;
		p["memberID"]=m_mailList[idx]["memberID"].asInt64();
		
		
		//삭제요청
		hspConnector::get()->command("removemessage",p,[this,obj,mailNo](Json::Value r)
																 {
																	 Json::Value newMailList;
																	 
																	 if(r.get("state","fail").asString()=="ok"){
																		 
																		 
																		 
																		 //테이블에서 없어진것 없애기
																		 for(int i=0;i<m_mailList.size();i++){
																			 if(m_mailList[i]["no"].asInt()!=mailNo){
																				 newMailList.append(m_mailList[i]);
																			 }
																		 }
																		 
																		 
																		 //테이블 리로드
																		 m_mailList=newMailList;
																		 this->mailTableView->reloadData();
																		 
																		 //하트올리기
																		 if(myDSH->getIntegerForKey(kDSH_Key_heartCnt)<5){
																			 myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
																		 }
																		 
																	 }
																 });
		
	}
	
	
	
	////////////////////////////////////////////////////////
	// tableview	////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx){

		CCLabelTTF* title;
		CCMenuItemLambda* sendBtn;
		CCLabelTTF* score;
		Json::Reader reader;
		Json::Value contentObj;
		
		Json::Value *mail = &m_mailList[idx]; //hspConnector::get()->getMailByIndex(idx);
		reader.parse((*mail)["content"].asString(), contentObj);


		KS::KSLog("%", *mail);
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		
		std::string cellBackFile = "ui_common_cell.png";
		
		
		CCSprite* bg = CCSprite::create(cellBackFile.c_str());
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,0);
		
		CCSprite* profileImg = GDWebSprite::create((*mail)["profile_image_url"].asString(), "ending_take_particle.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setTag(kMP_MT_profileImg);
		profileImg->setPosition(ccp(25, 22));
		profileImg->setScale(45.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kMP_Z_profileImg);
		
		
		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setPosition(ccp(0, 0));
		_menu->setTouchPriority(-200);
		_menu->setTag(kMP_MT_send);
		cell->addChild(_menu, kMP_MT_getheart);

		sendBtn = CCMenuItemImageLambda::create
		("postbox_cell_receive.png", "postbox_cell_receive.png",
		 bind(&MailPopup::removeMail, this, std::placeholders::_1));
		sendBtn->setPosition(ccp(190, 22));

		_menu->addChild(sendBtn,2);
		
		title = CCLabelTTF::create(((*mail)["nickname"].asString() + "님의").c_str(), "Helvetica",12);
		title->setPosition(ccp(30,28));
		title->setAnchorPoint(CCPointZero);
		title->setTag(kMP_MT_title);
		cell->addChild(title,2);

		std::string comment;
		int type = (*mail)["type"].asInt();
		switch(type)
		{
			case kHeart:
				comment = "하트가 도착했어요.";
				break;
			case kChallenge:
				comment = "도전장이 도착했어요.";
				break;
			case kHelpRequest:
				comment = "도움요청이 도착했어요.";
				break;
			case kTicketRequest:
				comment = "티켓요청이 도착했어요.";
				break;
			default:
				comment = "??요청이 도착했어요.";
		}
		
		score = CCLabelTTF::create(comment.c_str(),"Helvetica",20);
		score->setPosition(ccp(30,5));
		score->setAnchorPoint(CCPointZero);
		score->setTag(kMP_MT_score);
		cell->addChild(score,2);
		
		sendBtn->setUserData((void *)idx);
		
//		title->setString((*mail)["regDate"].asString().c_str());
//		score->setString((*mail)["friendID"].asString().c_str());
//		rank->setString((*mail)["type"].asString().c_str());
		

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
	
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell){
		
		CCLog("touch!!");
		
	}
	virtual CCSize cellSizeForTable(CCTableView *table){
		return mailCellSize;
	}
	
	virtual unsigned int numberOfCellsInTableView(CCTableView *table){
		return m_mailList.size();
	}
	
	
	void touchFriend(CCObject*, CCControlEvent){
		
		
	}
	
	CCPoint getContentPosition(int t_tag)
	{
		CCPoint return_value;
		
		if(t_tag == kMP_MT_invite)					return_value = ccp(240,43);
		else if(t_tag == kMP_MT_close)				return_value = ccp(345,283);
		else if(t_tag == kMP_MT_send)				return_value = ccp(240,230);
		else if(t_tag == kMP_MT_send_close)			return_value = ccp(193,59);
		else if(t_tag == kMP_MT_invite_close)		return_value = ccp(345,283);
		else if(t_tag == kMP_MT_invite_rank)		return_value = ccp(240,43);
		else if(t_tag == kMP_MT_invite_send)		return_value = ccp(240,230);
		else if(t_tag == kMP_MT_invite_send_close)	return_value = ccp(193,59);
		
		return return_value;
	}
	
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
	{
		return true;
	}
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
	{
	
	}
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
	{
	
	}
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
	{
	}
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
	
protected:
	Json::Value m_mailList;
	
	CCTableView* mailTableView;
	
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	ScrollBar* m_scrollBar;
};


