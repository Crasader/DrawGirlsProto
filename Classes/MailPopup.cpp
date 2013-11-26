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
#include "StageInfoDown.h"
#include "PuzzleMapScene.h"
#include "CardCase.h"
#include "StageImgLoader.h"

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
	profileImg->setScale(40.f / profileImg->getContentSize().width);
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
			 [=](CCObject* sender)
			 {
				 KSAlertView* av = KSAlertView::create();
				 //				 av->setVScroll(CCScale9Sprite::create("popup_bar_v.png", CCRectMake(0, 0, 23, 53),
				 //																							 CCRectMake(7, 7, 23 - 7*2, 53 - 7*2 - 4)));
				 //				 av->setHScroll(CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
				 //																							 CCRectMake(10, 7, 53 - 10*2, 23 - 7*2)));
				 
				 // 도망 버튼.
				 auto m0 = CCMenuItemImageLambda::create("ending_remove_card.png", "ending_remove_card.png",
																								 [=](CCObject* e){
																									 //																									 removeFromParent();
																									 CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
																									 KS::KSLog("%", mail);
																									 // 도망에 대한 처리가 결정이 안됨.
																									 
																									 
																								 });
				 av->addButton(m0);
				 // 수락버튼.
				 auto m1 = CCMenuItemImageLambda::create
				 ("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					[=](CCObject* e){
						//																									 removeFromParent();
						CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
						KS::KSLog("%", mail);
						int mailNo = m_mailList[idx]["no"].asInt();

						mySGD->setRemoveMessageMailNo(mailNo);
						mySGD->setRemoveMessageMemberId(m_mailList[idx]["memberID"].asInt64());
						mySGD->setAcceptChallengeTarget(contentObj["sender"].asString(), m_mailList[idx]["nickname"].asString(),
																						contentObj["score"].asFloat());
						mySD->setSilType(contentObj["challengestage"].asInt());
						mySGD->setIsAcceptChallenge(true);
						// ST 받고 성공시 창 띄움.. & sender->removeFromParent();
						addChild(StageInfoDown::create
										 (this,
											callfunc_selector(ThisClassType::onReceiveStageSuccess),
											this, callfunc_selector(ThisClassType::onReceiveStageFail)));
						
					});
				 av->addButton(m1);
				 
				 auto ttf = CCLabelTTF::create("도전을 수락합니까?", "", 12.f);
				 av->setContentNode(
														ttf
														);
				 
				 av->setCloseOnPress(false);
				 addChild(av, kMP_Z_helpAccept);
				 av->show();
			 }
			 );
			sendBtn->setPosition(ccp(190, 22));
			
			_menu->addChild(sendBtn,2);
			break;
		case kChallengeResult:
			comment = "도전결과!!";
			sendBtn = CCMenuItemImageLambda::create
			("card_mount.png", "card_mount.png",
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
				 auto m0 = CCMenuItemImageLambda::create("ending_remove_card.png", "ending_remove_card.png",
																								 [=](CCObject* e){
																									 //																									 removeFromParent();
																									 CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
																									 KS::KSLog("%", mail);
																									 // 도망에 대한 처리가 결정이 안됨.
																									 
																									 
																								 });
				 av->addButton(m0);

				 // 도움 수락버튼.
				 auto m1 = CCMenuItemImageLambda::create
				 ("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					[=](CCObject* e){
						CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
						KS::KSLog("%", mail);
						int mailNo = m_mailList[idx]["no"].asInt();
						
						mySGD->setRemoveMessageMailNo(mailNo);
						mySGD->setRemoveMessageMemberId(m_mailList[idx]["memberID"].asInt64());
//						mySGD->setAcceptChallengeTarget(contentObj["sender"].asString(), m_mailList[idx]["nickname"].asString(),
//																						contentObj["score"].asFloat());
						mySGD->setAcceptHelpTarget(contentObj["sender"].asString(), m_mailList[idx]["nickname"].asString());
						mySD->setSilType(contentObj["helpstage"].asInt());
						mySGD->setIsAcceptHelp(true);
						// ST 받고 성공시 창 띄움.. & sender->removeFromParent();
						addChild(StageInfoDown::create
										 (this,
											callfunc_selector(ThisClassType::onReceiveStageSuccess),
											this, callfunc_selector(ThisClassType::onReceiveStageFail)));
						//																									 Json::Value p;
						//																									 int mailNo = mail["no"].asInt();
						//																									 p["no"] = mailNo;
						//																									 p["memberID"] = mail["memberID"].asInt64();
						//
						//																									 iHelpYou(contentObj.get("helpstage", 0).asInt(),
						//																														mail["friendID"].asInt64(), mail["nickname"].asString(),
						//																														p);
						
						
						
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
		case kHelpResult:
			comment = "상대방의 도움이 왔어요!";
			sendBtn = CCMenuItemImageLambda::create
			("card_mount.png", "card_mount.png",
			 [=](CCObject*)
			 {
				 Json::Value p;
				 int mailNo = m_mailList[idx]["no"].asInt();
				 
				 p["no"] = mailNo;
				 p["memberID"] = m_mailList[idx]["memberID"].asInt64();
				 //삭제요청
#if 0
				 hspConnector::get()->command
				 (
					"removemessage",p,
					[=](Json::Value r)
#endif
					{
						KS::KSLog("%", contentObj);
						// 영호
#if 0
						contentObj["helpstage"].asInt(); // 이건 스테이지 번호
						contentObj["cardnumber"].asInt(); // 카드 번호.
						// 아래 setContentNode 에 그림이 들어간 노드를 넣어주면 됨...
						// 이거 작업하기 전에 나랑 이야기 ㄱㄱ.
#endif
						KSAlertView* av = KSAlertView::create();
						
						auto retStr = NSDS_GS(kSDS_CI_int1_imgInfo_s, contentObj["cardnumber"].asInt());
						KS::KSLog("retStr %", retStr);
						
						if(retStr == "") // 카드 정보 없음
						{
							download_card_number = contentObj["cardnumber"].asInt();
							CCSprite* card_img = CCSprite::create("ending_take_card_back.png");
							card_img->setScale(0.34f);
//							av->addChild(card_img);
							
							loading_card_img = card_img;

							CCLabelTTF* t_label = CCLabelTTF::create("카드 정보 로딩", mySGD->getFont().c_str(), 20);
							t_label->setColor(ccBLACK);
							t_label->setPosition(ccp(160,215));
							card_img->addChild(t_label);

							Json::Value param;
							param["noList"][0] = contentObj["cardnumber"].asInt();
							hspConnector::get()->command("getcardlist", param, json_selector(this, MailPopup::resultLoadedCardInfo));
							av->setContentNode(card_img);
						}
						else // 카드 정보 있음
						{
							av->setContentNode(addCardImg(contentObj["cardnumber"].asInt()));
//							av->addChild();
						}
						
						// 도움 수락버튼.
						auto m1 = CCMenuItemImageLambda::create
						("postbox_challenge_ok.png", "postbox_challenge_ok.png",
						 [=](CCObject* e){
						 });
						av->addButton(m1);
						
//						auto ttf = CCLabelTTF::create("스테이지 획득!!", "", 12.f);
//						av->setContentNode(
//															 ttf
//															 );
						addChild(av, kMP_Z_helpAccept);
						av->show();
					}
#if 0
					);
#endif
				 
			 }
			 );
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
		case kTicketResult:
			comment = "티켓이 왔네요 어서 받으세요.";
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
	
	score = CCLabelTTF::create(comment.c_str(),"Helvetica", 12.f);
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

void MailPopup::onReceiveStageSuccess()
{
	// 성공시 게임창...
	CCLog("sec");
	PuzzleMapScene* pms = dynamic_cast<PuzzleMapScene*>(target_close);
	CCAssert(pms, "!!");
	
	removeFromParent();
	pms->showAcceptStageSettingPopup();
//	PuzzleMapScene::showAcceptStageSettingPopup();
	
}
void MailPopup::onReceiveStageFail()
{
	mySGD->setRemoveMessageMemberId(0);
	mySGD->setRemoveMessageMailNo(0);
//	removeFromParent();
	CCLog("fail");
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

CCNode* MailPopup::addCardImg (int t_card_number)
{
	int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, t_card_number);
	int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
	
	CCSprite* card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_grade)->getCString());
	card_img->setScale(0.34f);
	
	if(card_grade == 3 && mySD->isAnimationStage(card_stage))
	{
		CCSize ani_size = mySD->getAnimationCutSize(card_stage);
		CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_grade)->getCString(),
											  CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(mySD->getAnimationPosition(card_stage));
		card_img->addChild(t_ani);
	}
	
	CardCase* t_case = CardCase::create(card_stage, card_grade);
	t_case->setPosition(CCPointZero);
	card_img->addChild(t_case);
	
	CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
	recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, t_card_number))->getCString());
	recent_durability_label->setPosition(ccpAdd(recent_durability_label->getPosition(), ccp(6,-1)));
	recent_durability_label->setFontSize(recent_durability_label->getFontSize()+3);
	
	t_case->getTotalDurabilityLabel()->removeFromParent();
	
	return card_img;
}

void MailPopup::resultLoadedCardInfo (Json::Value result_data)
{
	CCLog("resultLoadedCardData data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	
	if(result_data["state"].asString() == "ok")
	{
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt());
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt());
			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt());
			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt());
			
			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["stage"].asInt());
			NSDS_SI(t_card["stage"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
			Json::Value t_card_missile = t_card["missile"];
			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str());
			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt());
			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt());
			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble());
			
			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str());
			
			Json::Value t_ability = t_card["ability"];
			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), t_ability.size());
			for(int j=0;j<t_ability.size();j++)
			{
				Json::Value t_abil = t_ability[j];
				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt());
				
				Json::Value t_option;
				if(!t_abil["option"].isObject())                    t_option["key"]="value";
				else												t_option =t_abil["option"];
				
				if(t_abil["type"].asInt() == kIC_attack)
					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt());
				else if(t_abil["type"].asInt() == kIC_addTime)
					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt());
				else if(t_abil["type"].asInt() == kIC_fast)
					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt());
				else if(t_abil["type"].asInt() == kIC_silence)
					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt());
				else if(t_abil["type"].asInt() == kIC_doubleItem)
					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
				else if(t_abil["type"].asInt() == kIC_longTime)
					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt());
				else if(t_abil["type"].asInt() == kIC_bossLittleEnergy)
					NSDS_SI(kSDS_CI_int1_abilityBossLittleEnergyOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
				else if(t_abil["type"].asInt() == kIC_subSmallSize)
					NSDS_SI(kSDS_CI_int1_abilitySubSmallSizeOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
				else if(t_abil["type"].asInt() == kIC_smallArea)
					NSDS_SI(kSDS_CI_int1_abilitySmallAreaOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
				else if(t_abil["type"].asInt() == kIC_widePerfect)
					NSDS_SI(kSDS_CI_int1_abilityWidePerfectOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt());
			}
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
			bool is_add_cf = false;
			
			if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = t_imgInfo["size"].asInt();
				t_df.img = t_imgInfo["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("stage%d_level%d_visible.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
				df_list.push_back(t_df);
				// ================================
				
				CopyFile t_cf;
				t_cf.from_filename = t_df.filename.c_str();
				t_cf.to_filename = CCSTR_CWF("stage%d_level%d_thumbnail.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
				cf_list.push_back(t_cf);
				
				is_add_cf = true;
			}
			
			Json::Value t_aniInfo = t_card["aniInfo"];
			NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool());
			if(t_aniInfo["isAni"].asBool())
			{
				Json::Value t_detail = t_aniInfo["detail"];
				NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt());
				
				Json::Value t_loopSeq = t_detail["loopSeq"];
				for(int j=0;j<t_loopSeq.size();j++)
					NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt());
				
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt());
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt());
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt());
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt());
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt());
				
				if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_detail["size"].asInt();
					t_df.img = t_detail["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("stage%d_level%d_animation.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
				if(is_add_cf)
				{
					CopyFile t_cf = cf_list.back();
					cf_list.pop_back();
					t_cf.is_ani = true;
					t_cf.cut_width = t_detail["cutWidth"].asInt();
					t_cf.cut_height = t_detail["cutHeight"].asInt();
					t_cf.position_x = t_detail["positionX"].asInt();
					t_cf.position_y = t_detail["positionY"].asInt();
					
					t_cf.ani_filename = CCSTR_CWF("stage%d_level%d_animation.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
					
					cf_list.push_back(t_cf);
				}
			}
			
			NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString());
			
			Json::Value t_silImgInfo = t_card["silImgInfo"];
			NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool());
			if(t_silImgInfo["isSil"].asBool())
			{
				if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_silImgInfo["size"].asInt();
					t_df.img = t_silImgInfo["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("stage%d_level%d_invisible.png", t_card["stage"].asInt(), t_card["grade"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
			}
		}
		
		if(df_list.size() > 0) // need download
		{
			startDownloadCardImage();
		}
		else
		{
			CCNode* loading_parent = loading_card_img->getParent();
			CCPoint loading_position = loading_card_img->getPosition();
			
			loading_card_img->removeFromParent();
			
			loading_parent->addChild(addCardImg(download_card_number));
		}
	}
	else
	{
		failAction();
	}
}

void MailPopup::successAction ()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);
	
	if(ing_download_cnt >= df_list.size())
	{
		for(int i=0;i<cf_list.size();i++)
		{
			CCSprite* target_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
			target_img->setAnchorPoint(ccp(0,0));
			
			if(cf_list[i].is_ani)
			{
				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
				target_img->addChild(ani_img);
			}
			
			target_img->setScale(0.2f);
			
			CCRenderTexture* t_texture = CCRenderTexture::create(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY());
			t_texture->setSprite(target_img);
			t_texture->begin();
			t_texture->getSprite()->visit();
			t_texture->end();
			
			t_texture->saveToFile(cf_list[i].to_filename.c_str(), kCCImageFormatPNG);
		}
		
		df_list.clear();
		cf_list.clear();
		
		CCNode* loading_parent = loading_card_img->getParent();
		CCPoint loading_position = loading_card_img->getPosition();
		
		loading_card_img->removeFromParent();
		
		loading_parent->addChild(addCardImg(download_card_number));
	}
	else
	{
		ing_download_cnt++;
		startDownload();
	}
}
void MailPopup::failAction ()
{
	CCNode* loading_parent = loading_card_img->getParent();
	CCPoint loading_position = loading_card_img->getPosition();
	
	loading_card_img->removeFromParent();
	
	CCSprite* card_img = CCSprite::create("ending_take_card_back.png");
	card_img->setScale(0.34f);
	card_img->setPosition(ccp(99.f,156.f));
	
	CCLabelTTF* t_label = CCLabelTTF::create("정보 로드 실패", mySGD->getFont().c_str(), 20);
	t_label->setColor(ccBLACK);
	t_label->setPosition(ccp(160,215));
	card_img->addChild(t_label);
	
	loading_parent->addChild(card_img);
}
void MailPopup::startDownloadCardImage ()
{
	CCLog("start download card img");
	ing_download_cnt = 1;
	startDownload();
}
void MailPopup::startDownload ()
{
	CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename,
												  this, callfunc_selector(MailPopup::successAction), this, callfunc_selector(MailPopup::failAction));
}

#undef LZZ_INLINE
