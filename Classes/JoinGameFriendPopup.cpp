//
//  JoinGameFriendPopup.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 11. 20..
//
//

#include "JoinGameFriendPopup.h"

#include "RankPopup.h"
#include "SendMessageUtil.h"
#include "FriendListPopup.h"
#include "InviteEventPopup.h"
#include "KSAlertView.h"
#include <boost/format.hpp>
static CCSize cellSize3 = CCSizeMake(238, 47);
void JoinGameFriendPopup::myInit(CCObject* t_close, SEL_CallFunc d_close)
{
	setTouchEnabled(true);
	target_close = t_close;
	delegate_close = d_close;
	
	
	
	
	
	m_currentSelectSprite = NULL;
	
	used_card_img = NULL;
	loading_card_number = 0;
	after_loading_card_number = 0;
	last_selected_card_number = 0;
	
	//		gray = CCSprite::create("back_gray.png");
	//		gray->setPosition(ccp(240,160));
	//		gray->setContentSize(CCSizeMake(600, 400));
	//		addChild(gray, kRP_Z_gray);
	
	CCSprite* back = CCSprite::create("rank_back.png");
	back->setPosition(ccp(240,160));
	addChild(back, kZorderJoinGameFriendBack);
	
	
	CCSprite* back2 = CCSprite::create("rank_gamefriend_back.png");
	back2->setPosition(ccp(240,160));
	addChild(back2, kZorderJoinGameFriendBack);
//	CCScale9Sprite* back2 = CCScale9Sprite::create("popup_back2.png", CCRectMake(0, 0, 150, 150),
//																								 CCRectMake(5, 5, 150 - 5*2, 150 - 5*2));
//	back2->setPosition(ccp(240, 160));
//	back2->setContentSize(CCSizeMake(445, 260));
//	addChild(back2, kZorderJoinGameFriendBack);
	
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
	closeBtn->setPosition(ccp(440, 290));
	_menu->addChild(closeBtn);
	
	auto weekRank = CCMenuItemImageLambda::create
	(
	 "rank_friend_rank.png", "rank_friend_rank.png",
	 [=](CCObject*){
		 //																																 (target_close->*delegate_close)();
		 RankPopup* t_rp = RankPopup::create(t_close, d_close);
		 getParent()->addChild(t_rp, this->getZOrder());
		 removeFromParent();
	 });
	weekRank->setPosition(ccp(67, 290));
	weekRank->setOpacity(0);
	_menu->addChild(weekRank, 5);
	
	// 친구 초대 이벤트
	auto inviteEventBtn = CCMenuItemImageLambda::create
	(
	 "rank_default_invite.png", "rank_default_invite.png",
	 [=](CCObject*){
		 //																																 (target_close->*delegate_close)();
		 
		 InviteEventPopup* t_rp = InviteEventPopup::create(t_close, d_close);
		 getParent()->addChild(t_rp, this->getZOrder());
		 removeFromParent();
		 
	 });
	inviteEventBtn->setPosition(ccp(169, 290));
	inviteEventBtn->setOpacity(0);
	
	_menu->addChild(inviteEventBtn);
	
	// 친구목록
	auto friendList = CCMenuItemImageLambda::create
	(
	 "rank_friend_list.png", "rank_friend_list.png",
	 [=](CCObject*){
		 //																																 (target_close->*delegate_close)();
		 FriendListPopup* t_rp = FriendListPopup::create(t_close, d_close);
		 getParent()->addChild(t_rp, this->getZOrder());
		 removeFromParent();
		 
	 });
	friendList->setPosition(ccp(270, 290));
	friendList->setOpacity(0);
	_menu->addChild(friendList);
	
	// 게임 친구맺기
	auto joinGameFriend = CCMenuItemImageLambda::create
	(
	 "rank_gamefriend.png", "rank_gamefriend.png",
	 [=](CCObject*){
		 //																																 (target_close->*delegate_close)();
		 
		 
	 });
	joinGameFriend->setPosition(ccp(370, 290));
	joinGameFriend->setOpacity(255);
	_menu->addChild(joinGameFriend);
	
	auto addFriend = CCMenuItemImageLambda::create
	("rank_gamefriend_plus.png", "rank_gamefriend_plus.png",
	 [=](CCObject* t)
	 {
		 // "abkcje34454" -> 45843068445565 -> "45843068445565" 과정.
		 this->searchById
		 ( KS::longLongToStr((KS::strToLongLong(m_searchIdEditBox->getText(), 36)), 10) );
			});
		 
		 
	
	addFriend->setPosition(380, 320 - 72);
	_menu->addChild(addFriend, kZorderJoinGameFriendIdInput + 1);
	
	
	m_searchIdEditBox = CCEditBox::create(CCSizeMake(190, 25), CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
	
	m_searchIdEditBox->setPosition(ccp(260, 320 - 72 - 3));
	m_searchIdEditBox->setPlaceHolder("입력해주세요.");
	m_searchIdEditBox->setTouchPriority(-200);
	m_searchIdEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_searchIdEditBox->setFont(mySGD->getFont().c_str(), 15);
	m_searchIdEditBox->setInputMode(kEditBoxInputModeSingleLine);
	m_searchIdEditBox->setDelegate(this);
	addChild(m_searchIdEditBox, kZorderJoinGameFriendIdInput);
	
	loadRank();
}

JoinGameFriendPopup* JoinGameFriendPopup::create( CCObject* t_close, SEL_CallFunc d_close )
{
	JoinGameFriendPopup* t_rp = new JoinGameFriendPopup();
	t_rp->myInit(t_close, d_close);
	t_rp->autorelease();
	return t_rp;
}

void JoinGameFriendPopup::finishedOpen()
{
	loadRank();
}



void JoinGameFriendPopup::loadRank()
{
	std::function<void(Json::Value e)> p1 = bind(&ThisClassType::drawRank, this, std::placeholders::_1);
	//step1 ƒ´ƒ´ø¿ƒ£±∏∏Ò∑œ ∑ŒµÂ
	Json::Value param;
	param["limit"] = 40;
	hspConnector::get()->command("getuserlistbyrandom", param,
															 p1);
}

void JoinGameFriendPopup::drawRank( Json::Value obj )
{
	if(obj["result"]["code"].asInt() != GDSUCCESS)
		return;
	
	m_randomList = obj["list"];
	//≈◊¿Ã∫Ì ∫‰ ª˝º∫ Ω√¿€ /////////////////////////////////////////////////////////////////////////////////////////
	
	//320x320 ≈◊¿Ã∫Ì ∫‰ ª˝º∫
	rankTableView = JoinFriendTableView::create(this, CCSizeMake(424, 195), NULL);
	//		CCScale9Sprite* bar = CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
	//																		1						 CCRectMake(10, 7, 53 - 10*2, 23 - 7*2));
	CCScale9Sprite* bar = CCScale9Sprite::create("card_scroll.png");
	m_scrollBar = ScrollBar::createScrollbar(rankTableView, -2, NULL, bar);
	m_scrollBar->setDynamicScrollSize(false);
	rankTableView->setAnchorPoint(CCPointZero);
	
	//kCCScrollViewDirectionVertical : ºº∑Œ Ω∫≈©∑—, kCCScrollViewDirectionHorizontal : ∞°∑Œ Ω∫≈©∑—
	rankTableView->setDirection(kCCScrollViewDirectionVertical);
	
	//√ﬂ∞°Ω√ ¡§∑ƒ ±‚¡ÿ º≥¡§ kCCTableViewFillTopDown : æ∆∑°∫Œ∫–¿∏∑Œ √ﬂ∞°µ , kCCTableViewFillBottomUp : ¿ßø°º≠ ∫Œ≈Õ √ﬂ∞°µ .
	rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	
	//±‚¡ÿ¡° 0,0
	rankTableView->setPosition(ccp(33, 320 - 291));
	
	//µ•¿Ã≈Õ∏¶ ∞°¡Æø¿∞Ì≥™ ≈Õƒ° ¿Ã∫•∆Æ∏¶ π›»Ø«ÿ¡Ÿ ¥Î∏Æ¿⁄∏¶ ¿Ã ≈¨∑°Ω∫∑Œ º≥¡§.
	rankTableView->setDelegate(this);
	this->addChild(rankTableView, kZorderJoinGameFriendRankTable);
	rankTableView->setTouchPriority(-200);
}

CCTableViewCell* JoinGameFriendPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCLabelTTF* title;
	CCMenuItemLambda* sendBtn;
	CCLabelTTF* score;
	Json::Value& member = m_randomList[idx];
	KS::KSLog("%", member);
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	
	
	
	std::string cellBackFile = "rank_gamefriend_cell.png";
	
	
	CCSprite* bg = CCSprite::create(cellBackFile.c_str());
	bg->setPosition(CCPointZero);
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,0);
	
	CCSprite* profileImg = GDWebSprite::create(member["profile_image_url"].asString(), "no_img.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kTagGameFriendProfileImg);
	profileImg->setPosition(ccp(27, 22));
	profileImg->setScale(45.f / profileImg->getContentSize().width);
	cell->addChild(profileImg, kZorderJoinGameFriendProfileImg);
	
	
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setPosition(ccp(0, 0));
	_menu->setTouchPriority(-300);
	_menu->setTag(kTagGameFriendMenu);
	cell->addChild(_menu, kZorderJoinGameFriendMenu);
	
	

	{
		sendBtn = CCMenuItemImageLambda::create
		("rank_gamefriend_call.png", "rank_gamefriend_call.png",
		 [=](CCObject* sender){
			 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
			 int idx = (int)obj->getUserData();
			 ////////////////////////////////
			 // ¬ ¡ˆ∫∏≥ª±‚ - HSP
			 ////////////////////////////////
			 
			 
			 Json::Value p;
			 Json::Value contentJson;
			 
			 contentJson["msg"] = "unknownreq";
			 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
			 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
			 //				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
			 p["content"] = GraphDogLib::JsonObjectToString(contentJson);
			 std::string recvId = member["memberID"].asString();
			 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
			 p["receiverMemberID"] = recvId;
			 p["senderMemberID"]=hspConnector::get()->getKakaoID();
			 p["type"] = kUnknownFriendRequest;
			 
			 hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																		{
																			//		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																			//		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																			//		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																			
																			
																			GraphDogLib::JsonToLog("sendMessage", r);
																			if(r["result"]["code"].asInt() != GDSUCCESS)
																				return;
																			
																			obj->removeFromParent();
																			
//																			CCMenuItemImageLambda* sendBtn1 = CCMenuItemImageLambda::create("rank_cell_notinvite.png", "rank_cell_notinvite.png",
//																																																			[](CCObject*){});
//																			sendBtn1->setPosition(ccp(360,22));
//																			_menu->addChild(sendBtn1,2);
																		});
		 });
	}
	
	
	sendBtn->setPosition(ccp(360, 22));
	sendBtn->setTag(kTagGameFriendSend);
	_menu->addChild(sendBtn, kZorderJoinGameFriendSend);
	
	title = CCLabelTTF::create("","Helvetica",12);
	title->setPosition(ccp(90,28));
	title->setAnchorPoint(CCPointZero);
	title->setTag(kTagGameFriendNickname);
	cell->addChild(title, 2);
	
	score = CCLabelTTF::create("","Helvetica",20);
	score->setPosition(ccp(90,5));
	score->setAnchorPoint(CCPointZero);
	score->setTag(kTagGameFriendLastDate);
	cell->addChild(score,2);
	
	sendBtn->setUserData((void *)idx);
	std::string titleStr = member["nick"].asString().c_str();
	if(titleStr == "")
		titleStr = member["memberID"].asString();
	
	title->setString(titleStr.c_str());
//	auto end = chrono::system_clock::now();
//	auto currentSecond = chrono::system_clock::to_time_t(end);
	
	score->setString("최종접속 : ...");
	//rank->setString((*member)["rankingGrade"].asString().c_str());
	
	return cell;
}

void JoinGameFriendPopup::scrollViewDidScroll( CCScrollView* view )
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
}

void JoinGameFriendPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void JoinGameFriendPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	
}

CCSize JoinGameFriendPopup::cellSizeForTable( CCTableView *table )
{
	return cellSize3;
}

unsigned int JoinGameFriendPopup::numberOfCellsInTableView( CCTableView *table )
{
	return m_randomList.size();
}

void JoinGameFriendPopup::touchFriend( CCObject*, CCControlEvent )
{
	
}

CCPoint JoinGameFriendPopup::getContentPosition( int t_tag )
{
	CCPoint return_value;
	
	
	return return_value;
}
void JoinGameFriendPopup::searchById(const std::string& userId)
{
	Json::Value p;
	Json::Value contentJson;
	
	contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
	//				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
	p["content"] = GraphDogLib::JsonObjectToString(contentJson);
	std::string recvId = userId;
	recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
	p["receiverMemberID"] = recvId;
	p["senderMemberID"] = hspConnector::get()->getKakaoID();
	p["type"] = kUnknownFriendRequest;
	
	Json::Value param;
	param["memberID"] = recvId;
	hspConnector::get()->command("getUserData", param,
															 [=](Json::Value t)
															 {
																 KS::KSLog("zz %", t);
																 if(t["state"] == "error")
																 {
																	 KSAlertView* av = KSAlertView::create();
																	 av->setCloseOnPress(true);
																	 av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
																	 
																	 av->setBorderScale(0.9f);
																	 av->setCenterY(150);
																	 
																	 auto ttf = CCLabelTTF::create("아이디를 찾을 수 없습니다.", "", 12.f);
																	 ttf->setColor(ccc3(0, 0, 0));
																	 
																	 av->setContentNode(
																											ttf
																											);
																	 av->setContentSize(ttf->getDimensions());
																	 
																	 av->addButton(CCMenuItemImageLambda::create
																								 (
																									"ui_common_ok.png",
																									"ui_common_ok.png",
																									[=](CCObject* e){
																									}
																									));
																	 addChild(av, kZorderPopup);
																	 av->show();
																	 return;
																 }

																 
																 hspConnector::get()->command
																 ("sendMessage", p, [=](Json::Value r)
																	{
																		GraphDogLib::JsonToLog("sendMessage", r);
																		if(r["result"]["code"].asInt() != GDSUCCESS)
																			return;
																		
																		KSAlertView* av = KSAlertView::create();
																		av->setCloseOnPress(true);
																		av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
																		
																		av->setBorderScale(0.9f);
																		av->setCenterY(150);
																		
																		auto ttf = CCLabelTTF::create("신청하였습니다. 수락을 기다리세요.", "", 12.f);
																		ttf->setColor(ccc3(0, 0, 0));
																		
																		av->setContentNode(
																											 ttf
																											 );
																		av->setContentSize(ttf->getDimensions());
																		
																		av->addButton(CCMenuItemImageLambda::create
																									(
																									 "ui_common_ok.png",
																									 "ui_common_ok.png",
																									 [=](CCObject* e){
																									 }
																									 ));
																		addChild(av, kZorderPopup);
																		av->show();
																		
																	});
															 });
	
	
	
}
void JoinGameFriendPopup::editBoxReturn(CCEditBox* editBox)
{
//	this->searchById(editBox->getText());
}













JoinFriendTableView* JoinFriendTableView::create( CCTableViewDataSource* dataSource, CCSize size, CCNode *container )
{
	JoinFriendTableView *table = new JoinFriendTableView();
	table->initWithViewSize(size, container);
	table->autorelease();
	table->setDataSource(dataSource);
	table->_updateCellPositions();
	table->_updateContentSize();
	
	return table;
}


