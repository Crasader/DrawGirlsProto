// MailPopup.cpp
//

#include "MailPopup.h"

#include "hspConnector.h"

#include "DataStorageHub.h"
#include "GDWebSprite.h"
#include "ServerDataSave.h"
#include "CCMenuLambda.h"

#include "KSUtil.h"
#include "GDWebSprite.h"
#include "EnumDefine.h"
#include "ScrollBar.h"
#include "KSAlertView.h"

#define LZZ_INLINE inline

using namespace std;
namespace
{
  CCSize mailCellSize = CCSizeMake(238, 47);
}
MailPopup * MailPopup::create (CCObject * t_close, SEL_CallFunc d_close)
{
	MailPopup* t_rp = new MailPopup();
	t_rp->myInit(t_close, d_close);
	t_rp->autorelease();
	return t_rp;
}
void MailPopup::finishedOpen ()
{
	loadMail();
}
void MailPopup::finishedClose ()
{
	
	(target_close->*delegate_close)();
	removeFromParent();
}
void MailPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
{
	target_close = t_close;
	delegate_close = d_close;
	
	m_popupState = PostBoxState::kNoMenu;
	m_alignTexts[AlignText::kTotal] = CCSprite::create("postbox_align_all.png");
	m_alignTexts[AlignText::kNews] = CCSprite::create("postbox_align_news.png");
	m_alignTexts[AlignText::kHeart] = CCSprite::create("postbox_align_heart.png");
	m_alignTexts[AlignText::kChallenge] = CCSprite::create("postbox_align_challenge.png");
	m_alignTexts[AlignText::kHelp] = CCSprite::create("postbox_align_help.png");
	m_alignTexts[AlignText::kTicket] = CCSprite::create("postbox_align_ticket.png");
	m_alignTexts[AlignText::kClose] = CCSprite::create("postbox_align_close.png");
	
	

	
	setTouchEnabled(true);
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setTouchPriority(-200);
	
	CCSprite* back = CCSprite::create("postbox_back.png");
	back->setPosition(ccp(240,160));
	addChild(back, kMP_Z_back);
	back->addChild(_menu);
	_menu->setPosition(ccp(0, 0));
	
	// 현재 상태 보여주는 버튼 생성.
	CCMenuItemLambda* stateButtonBack = CCMenuItemImageLambda::create
	("postbox_align_back.png", "postbox_align_back.png",
	 [=](CCObject* sender)
	 {
		 if(m_popupState == PostBoxState::kNoMenu)
		 {
			 showLeftMenuToggle(true);
		 }
		 else if(m_popupState == PostBoxState::kMenu)
		 {
			 showLeftMenuToggle(false);
		 }
	 }
	 );
	
	const int leftMenuX = 54;
	const int leftMenuY = 240;
	stateButtonBack->setPosition(ccp(leftMenuX, leftMenuY));
	
	_menu->addChild(stateButtonBack);
	
	// 버튼안 텍스트 넣기
	CCPoint textPosition = (stateButtonBack->getPosition());
	for(auto i : m_alignTexts)
	{
		back->addChild(i.second);
		i.second->setPosition(textPosition);
		
	}
	
	// 왼쪽 메뉴 세팅.
	CCMenuItemImageLambda* m0 = CCMenuItemImageLambda::create
	("postbox_aligntotal.png", "postbox_aligntotal.png",
	 [=](CCObject* sender)
	{
		showLeftMenuToggle(false);
		setAlignText(AlignText::kTotal);
		CCLog("postbox_aligntotal.png");
	 });
//	m0->setAnchorPoint(ccp(0.5f, 1));
	m0->setPosition(ccp(leftMenuX, leftMenuY - 25));
	_menu->addChild(m0, kMP_Z_back);
	
	CCMenuItemImageLambda* m1 = CCMenuItemImageLambda::create
	("postbox_alignnews.png", "postbox_alignnews.png",
	 [=](CCObject* sender)
	 {
		 showLeftMenuToggle(false);
		 setAlignText(AlignText::kNews);
		 CCLog("postbox_alignnews.png");
	 });
	m1->setAnchorPoint(ccp(0.5f, 1));
	m1->setPosition(ccp(leftMenuX, leftMenuY - 18 - 29));
	_menu->addChild(m1, kMP_Z_back);
	
	CCMenuItemImageLambda* m2 = CCMenuItemImageLambda::create
	("postbox_alignheart.png", "postbox_alignheart.png",
	 [=](CCObject* sender)
	 {
		 showLeftMenuToggle(false);
		 setAlignText(AlignText::kHeart);
		 CCLog("postbox_alignheart.png");
	 });
	m2->setAnchorPoint(ccp(0.5f, 1));
	m2->setPosition(ccp(leftMenuX, leftMenuY - 18 - 29 - 29));
	_menu->addChild(m2, kMP_Z_back);
	
	CCMenuItemImageLambda* m3 = CCMenuItemImageLambda::create
	("postbox_alignhelp.png", "postbox_alignhelp.png",
	 [=](CCObject* sender)
	 {
		 showLeftMenuToggle(false);
		 setAlignText(AlignText::kHelp);
		 CCLog("postbox_alignhelp.png");
	 });
	m3->setAnchorPoint(ccp(0.5f, 1));
	m3->setPosition(ccp(leftMenuX,leftMenuY - 18 - 29 - 29 - 29));
	_menu->addChild(m3, kMP_Z_back);
	
	CCMenuItemImageLambda* m4 = CCMenuItemImageLambda::create
	("postbox_alignticket.png", "postbox_alignticket.png",
	 [=](CCObject* sender)
	 {
		 showLeftMenuToggle(false);
		 setAlignText(AlignText::kTicket);
		 CCLog("postbox_alignticket.png");
	 });
	m4->setAnchorPoint(ccp(0.5f, 1));
	m4->setPosition(ccp(leftMenuX, leftMenuY - 18 - 29 - 29 - 29 - 29));
	_menu->addChild(m4, kMP_Z_back);

	m_menuList.push_back(m0);
	m_menuList.push_back(m1);
	m_menuList.push_back(m2);
	m_menuList.push_back(m3);
	m_menuList.push_back(m4);
	
	
	
	CCMenuItemLambda* closeBtn = CCMenuItemImageLambda::create(
																														 "cardsetting_close.png", "cardsetting_close.png",
																														 [=](CCObject*){
																															 (target_close->*delegate_close)();
																															 removeFromParent();
																															 
																														 });
	closeBtn->setPosition(ccp(325, 290));
	_menu->addChild(closeBtn, kMP_Z_close);
	
	
	showLeftMenuToggle(false);
	setAlignText(AlignText::kTotal);
	loadMail();
}
void MailPopup::loadMail ()
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
void MailPopup::drawMail (Json::Value obj)
{
	
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
void MailPopup::closePopup (CCControlButton * obj, CCControlEvent event)
{
	this->runAction(CCSequence::create(CCMoveBy::create(0.5, CCPoint(0,-400)),CCCallFunc::create(this, callfunc_selector(MailPopup::finishedClose)),NULL));
}
void MailPopup::removeMail (CCObject * _obj)
{
	
	
}
CCTableViewCell * MailPopup::tableCellAtIndex (CCTableView * table, unsigned int idx)
{
	
	CCLabelTTF* title;
	CCMenuItemLambda* sendBtn;
	CCLabelTTF* score;
	Json::Reader reader;
	Json::Value contentObj;
	
	const Json::Value& mail = m_mailList[idx]; //hspConnector::get()->getMailByIndex(idx);
	reader.parse((mail)["content"].asString(), contentObj);
	
	
	KS::KSLog("%", mail);
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	std::string cellBackFile = "ui_common_cell.png";
	
	
	CCSprite* bg = CCSprite::create(cellBackFile.c_str());
	bg->setPosition(CCPointZero);
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,0);
	
	CCSprite* profileImg = GDWebSprite::create((mail)["profile_image_url"].asString(), "ending_take_particle.png");
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
	
	
	
	title = CCLabelTTF::create(((mail)["nickname"].asString() + "님의").c_str(), "Helvetica",12);
	title->setPosition(ccp(30,28));
	title->setAnchorPoint(CCPointZero);
	title->setTag(kMP_MT_title);
	cell->addChild(title,2);
	
	std::string comment;
	int type = (mail)["type"].asInt();
	switch(type)
	{
		case kHeart:
			comment = "하트가 도착했어요.";
			sendBtn = CCMenuItemImageLambda::create
			("postbox_cell_receive.png", "postbox_cell_receive.png",
			 [=](CCObject* sender)
			 {
				 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
				 int idx = (int)obj->getUserData();
				 
				 Json::Value p;
				 int mailNo = m_mailList[idx]["no"].asInt();
				 
				 p["no"] = mailNo;
				 p["memberID"] = m_mailList[idx]["memberID"].asInt64();
				 
				 
				 //삭제요청
				 hspConnector::get()->command
				 (
					"removemessage",p,
					[=](Json::Value r)
					{
						Json::Value newMailList;
						
						if(r.get("state","fail").asString() == "ok"){
							
							
							
							//테이블에서 없어진것 없애기
							for(int i=0;i<m_mailList.size();i++){
								if(m_mailList[i]["no"].asInt() != mailNo){
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
			 );
			sendBtn->setPosition(ccp(190, 22));
			
			_menu->addChild(sendBtn,2);
			
			break;
		case kChallengeRequest:
			comment = "도전장이 도착했어요.";
			sendBtn = CCMenuItemImageLambda::create
			("postbox_challenge_ok.png", "postbox_challenge_ok.png",
			 [=](CCObject*)
			 {
			 }
			 );
			sendBtn->setPosition(ccp(190, 22));
			
			_menu->addChild(sendBtn,2);
			break;
		case kHelpRequest:
			comment = "도움요청이 도착했어요.";
			
			sendBtn = CCMenuItemImageLambda::create
			("postbox_help_ok.png", "postbox_help_ok.png",
			 [=](CCObject*)
			 {
				 KSAlertView* av = KSAlertView::create();
//				 av->setVScroll(CCScale9Sprite::create("popup_bar_v.png", CCRectMake(0, 0, 23, 53),
//																							 CCRectMake(7, 7, 23 - 7*2, 53 - 7*2 - 4)));
//				 av->setHScroll(CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
//																							 CCRectMake(10, 7, 53 - 10*2, 23 - 7*2)));
				 
				 // 도움 수락버튼.
				 auto m1 = CCMenuItemImageLambda::create("postbox_challenge_ok.png", "postbox_challenge_ok.png",
																								 [=](CCObject* e){
																									 removeFromParent();
																									 KS::KSLog("%", mail);
																									 Json::Value p;
																									 int mailNo = mail["no"].asInt();
																									 p["no"] = mailNo;
																									 p["memberID"] = mail["memberID"].asInt64();
																									 
																									 iHelpYou(contentObj.get("helpstage", 0).asInt(),
																														mail["friendID"].asInt64(), mail["nickname"].asString(),
																														p);
																									 
																								 });
				 av->addButton(m1);
				 
				 auto ttf = CCLabelTTF::create("?asd?", "", 12.f);
				 av->setContentNode(
														ttf
														);

				 
				 addChild(av, kMP_Z_helpAccept);
				 av->show();
			 });
			sendBtn->setPosition(ccp(190, 22));
			
			_menu->addChild(sendBtn,2);
			break;
		case kTicketRequest:
			comment = "티켓요청이 도착했어요.";
			sendBtn = CCMenuItemImageLambda::create
			("postbox_challenge_ok.png", "postbox_challenge_ok.png",
			 [=](CCObject*)
			 {
			 }
			 );
			sendBtn->setPosition(ccp(190, 22));
			
			_menu->addChild(sendBtn,2);
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
void MailPopup::scrollViewDidScroll (CCScrollView * view)
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
}
void MailPopup::scrollViewDidZoom (CCScrollView * view)
{
	
}
void MailPopup::tableCellTouched (CCTableView * table, CCTableViewCell * cell)
{
	
	CCLog("touch!!");
	
}
CCSize MailPopup::cellSizeForTable (CCTableView * table)
{
	return mailCellSize;
}
unsigned int MailPopup::numberOfCellsInTableView (CCTableView * table)
{
	return m_mailList.size();
}
void MailPopup::touchFriend (CCObject * a, CCControlEvent b)
{
	
	
}
CCPoint MailPopup::getContentPosition (int t_tag)
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
bool MailPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
{
	return true;
}
void MailPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void MailPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
{
	
}
void MailPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
{
}
void MailPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}

void MailPopup::setAlignText(AlignText at)
{
	for(auto i : m_alignTexts)
	{
		i.second->setVisible(false);
	}
	m_alignTexts[at]->setVisible(true);
}
void MailPopup::showLeftMenuToggle(bool show)
{
	if(show)
	{
		m_popupState = PostBoxState::kMenu;
		for(auto i : m_menuList)
		{
			i->setVisible(true);
		}
	}
	else
	{
		m_popupState = PostBoxState::kNoMenu;
		for(auto i : m_menuList)
		{
			i->setVisible(false);
		}
	}
}

// 도움 수락을 누른 상태.
void MailPopup::iHelpYou(int stage, long long user_id, const std::string& nick, Json::Value removeInfo)
{

	//삭제요청 하는 방법임. 삭제해야할 정보는 removeInfo 에 들어있음 신경 ㄴㄴ해.	
#if 0
	hspConnector::get()->command
	(
	 "removemessage", removeInfo,
	 [=](Json::Value r)
	 {
		 if(r.get("state","fail").asString() == "ok"){
		 }
	 });
#endif
	
	
}
#undef LZZ_INLINE
