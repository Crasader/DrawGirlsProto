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
#include "GivenFriendList.h"
#include "UnknownFriends.h"
#include "KnownFriend.h"
static CCSize cellSize3 = CCSizeMake(238, 38);
void JoinGameFriendPopup::myInit(CCObject* t_close, SEL_CallFunc d_close)
{
	
	
	DimmedPopup::init();
	this->setHideFinalAction(t_close, d_close);
	this->setBackground("friendoption_back.png");
	
	setTouchEnabled(true);
	
	m_currentSelectSprite = NULL;
	
	used_card_img = NULL;
	loading_card_number = 0;
	after_loading_card_number = 0;
	last_selected_card_number = 0;
	
	//		gray = CCSprite::create("back_gray.png");
	//		gray->setPosition(ccp(240,160));
	//		gray->setContentSize(CCSizeMake(600, 400));
	//		addChild(gray, kRP_Z_gray);
	
	
	
	CCSprite* back2 = CCSprite::create("friendsearch_back.png");
	back2->setPosition(ccp(240, 129));
	addChild(back2, kZorderJoinGameFriendBack);
//	CCScale9Sprite* back2 = CCScale9Sprite::create("popup_back2.png", CCRectMake(0, 0, 150, 150),
//																								 CCRectMake(5, 5, 150 - 5*2, 150 - 5*2));
//	back2->setPosition(ccp(240, 160));
//	back2->setContentSize(CCSizeMake(445, 260));
//	addChild(back2, kZorderJoinGameFriendBack);

	givenFriendList = GivenFriendList::create();
	addChild(givenFriendList,kZorderJoinGameFriendContent);



		
//	CCMenuLambda* _menu = CCMenuLambda::create();
//	_menu->setTouchPriority(-200);
//	back2->addChild(_menu);
//	_menu->setPosition(ccp(-25, -17));
//	
//	
//	CCMenuItemLambda* closeBtn = CCMenuItemImageLambda::create(
//																														 "cardchange_cancel.png", "cardchange_cancel.png",
//																														 [=](CCObject*){
//																															 (target_close->*delegate_close)();
//																															 removeFromParent();
//																														 });
//	closeBtn->setPosition(ccp(450, 258));
//	_menu->addChild(closeBtn);
	
	CCSprite* addFriend = CCSprite::create("friendsearch_plus.png");
	addFriend->setPosition(ccp(417, 202));
	addChild(addFriend, kZorderJoinGameFriendIdInput);

	CommonButton* closeBtn = CommonButton::createCloseButton(-200);
	closeBtn->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
		hspConnector::get()->removeTarget(givenFriendList);
		this->hidePopup();
	});
	
	closeBtn->setPosition(ccp(450, 255));
	this->addChild(closeBtn);

	// 친구 초대 이벤트
//	auto inviteEventBtn = CCMenuItemImageLambda::create
//	(
//	 "friendoption_invite.png", "friendoption_invite.png",
//	 [=](CCObject*){
//		 //																																 (target_close->*delegate_close)();
//		 
//		 InviteEventPopup* t_rp = InviteEventPopup::create(t_close, d_close);
//		 getParent()->addChild(t_rp, this->getZOrder());
//		 removeFromParent();
//		 
//	 });
//	inviteEventBtn->setPosition(ccp(172, 259));
//	inviteEventBtn->setOpacity(0);
//	_menu->addChild(inviteEventBtn);
	
	CommonButton* inviteEventBtn = CommonButton::create("친구초대이벤트", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	inviteEventBtn->setBackgroundTypeForDisabled(CommonButtonYellow);
	inviteEventBtn->setTitleColor(ccc3(200, 200, 200));
	inviteEventBtn->setTitleColorForDisable(ccc3(20, 0, 0));
	inviteEventBtn->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
		hspConnector::get()->removeTarget(givenFriendList);
		InviteEventPopup* t_rp = InviteEventPopup::create(t_close, d_close);
		t_rp->setOpenAnimation(false);
		getParent()->addChild(t_rp, this->getZOrder());
		removeFromParent();
	});
	inviteEventBtn->setPosition(ccp(169, 255));
	this->addChild(inviteEventBtn, kRP_Z_back+1);
	
	
	// 친구목록
//	auto friendList = CCMenuItemImageLambda::create
//	(
//	 "friendoption_list.png", "friendoption_list.png",
//	 [=](CCObject*){
//		 //																																 (target_close->*delegate_close)();
//		 FriendListPopup* t_rp = FriendListPopup::create(t_close, d_close);
//		 getParent()->addChild(t_rp, this->getZOrder());
//		 removeFromParent();
//	 });
//	friendList->setPosition(ccp(275, 259));
//	friendList->setOpacity(0);
//	_menu->addChild(friendList);
	
	CommonButton* friendList = CommonButton::create("친구 목록", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	friendList->setBackgroundTypeForDisabled(CommonButtonYellow);
	friendList->setTitleColor(ccc3(200, 200, 200));
	friendList->setTitleColorForDisable(ccc3(20, 0, 0));
	friendList->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
		hspConnector::get()->removeTarget(givenFriendList);
		FriendListPopup* t_rp = FriendListPopup::create(t_close, d_close);
		t_rp->setOpenAnimation(false);
		getParent()->addChild(t_rp, this->getZOrder());
		removeFromParent();
	});
	friendList->setPosition(ccp(272, 255));
	this->addChild(friendList, kRP_Z_back+1);
	
	
	// 게임 친구맺기
//	auto joinGameFriend = CCMenuItemImageLambda::create
//	(
//	 "friendoption_surch.png", "friendoption_surch.png",
//	 [=](CCObject*){
//		 //																																 (target_close->*delegate_close)();
//		 JoinGameFriendPopup* t_rp = JoinGameFriendPopup::create(t_close, d_close);
//		 getParent()->addChild(t_rp, this->getZOrder());
//		 removeFromParent();
//		 
//	 });
//	
//	joinGameFriend->setPosition(ccp(378, 259));
//	joinGameFriend->setOpacity(255);
//	_menu->addChild(joinGameFriend);
	
	CommonButton* joinGameFriend = CommonButton::create("게임친구맺기", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	joinGameFriend->setBackgroundTypeForDisabled(CommonButtonYellow);
	joinGameFriend->setTitleColor(ccc3(200, 200, 200));
	joinGameFriend->setTitleColorForDisable(ccc3(20, 0, 0));
	joinGameFriend->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
		hspConnector::get()->removeTarget(givenFriendList);

	});
	joinGameFriend->setPosition(ccp(375, 255));
	
	
	this->addChild(joinGameFriend, kRP_Z_back+1);
	joinGameFriend->setEnabled(false);
	
	CCScale9Sprite* tempBack = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	tempBack->setOpacity(0);
	m_searchIdEditBox = CCEditBox::create(CCSizeMake(190, 25), tempBack);
//	m_searchIdEditBox->getBackgroundSprite()->setVisible(false);
//	m_searchIdEditBox->getBackgroundSprite()->setOpacity(0);
	m_searchIdEditBox->setPosition(ccp(398, 201));
	m_searchIdEditBox->setPlaceHolder("입력해주세요.");
	m_searchIdEditBox->setTouchPriority(-200);
	m_searchIdEditBox->setInputMode(kEditBoxInputModeSingleLine);
	m_searchIdEditBox->setReturnType(kKeyboardReturnTypeDone);
	m_searchIdEditBox->setFont(mySGD->getFont().c_str(), 12);
	m_searchIdEditBox->setInputMode(kEditBoxInputModeSingleLine);
	m_searchIdEditBox->setDelegate(this);
	addChild(m_searchIdEditBox, kZorderJoinGameFriendIdInput);
//	rank_gamefriend_search.png
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
	//step1 ƒ´ƒ´ø¿ƒ£±∏∏Ò∑œ ∑ŒµÂ
	Json::Value param;
	param["limit"] = 40;
	hspConnector::get()->command("getuserlistbyrandom", param,this,
			bind(&ThisClassType::drawRank, this, std::placeholders::_1));
}

void JoinGameFriendPopup::drawRank( Json::Value obj )
{
	if(obj["result"]["code"].asInt() != GDSUCCESS)
		return;
	
	//m_randomList = obj["list"];
	m_randomList.clear();
	Json::Value listResult = obj["list"];
	for(int i=0; i<listResult.size(); i++)
	{
		std::string memberId = listResult[i]["memberID"].asString();
		auto ptr1 = UnknownFriends::getInstance()->findById(memberId);
		auto ptr2 = KnownFriends::getInstance()->findById(memberId);
		if(!ptr1 && !ptr2)
		{
			m_randomList.append(listResult[i]);
		}
	}

		
	KS::KSLog("%", m_randomList);
	//≈◊¿Ã∫Ì ∫‰ ª˝º∫ Ω√¿€ /////////////////////////////////////////////////////////////////////////////////////////
	
	//320x320 ≈◊¿Ã∫Ì ∫‰ ª˝º∫
	rankTableView = JoinFriendTableView::create(this, CCSizeMake(215, 165), NULL);
	//		CCScale9Sprite* bar = CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
	//																		1						 CCRectMake(10, 7, 53 - 10*2, 23 - 7*2));
//	CCScale9Sprite* bar = CCScale9Sprite::create("card_scroll.png");
//	m_scrollBar = ScrollBar::createScrollbar(rankTableView, -2 - 8, NULL, bar);
//	m_scrollBar->setDynamicScrollSize(false);
	rankTableView->setAnchorPoint(CCPointZero);
	
	//kCCScrollViewDirectionVertical : ºº∑Œ Ω∫≈©∑—, kCCScrollViewDirectionHorizontal : ∞°∑Œ Ω∫≈©∑—
	rankTableView->setDirection(kCCScrollViewDirectionVertical);
	
	//√ﬂ∞°Ω√ ¡§∑ƒ ±‚¡ÿ º≥¡§ kCCTableViewFillTopDown : æ∆∑°∫Œ∫–¿∏∑Œ √ﬂ∞°µ , kCCTableViewFillBottomUp : ¿ßø°º≠ ∫Œ≈Õ √ﬂ∞°µ .
	rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	
	//±‚¡ÿ¡° 0,0
	rankTableView->setPosition(ccp(243, 23));
	
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
	
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	
	
	
	std::string cellBackFile = "friendsearch_cell.png";
	
	
	CCSprite* bg = CCSprite::create(cellBackFile.c_str());
	bg->setPosition(CCPointZero);
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,0);
	
	CCSprite* profileImg = GDWebSprite::create(member["profile_image_url"].asString(), "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kTagGameFriendProfileImg);
	profileImg->setPosition(ccp(22, 21));
	profileImg->setScale(28.f / profileImg->getContentSize().width);
	cell->addChild(profileImg, kZorderJoinGameFriendProfileImg);
	
	
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setPosition(ccp(0, 0));
	_menu->setTouchPriority(-300);
	_menu->setPropaOnBegan(true);
	_menu->setTag(kTagGameFriendMenu);
	cell->addChild(_menu, kZorderJoinGameFriendMenu);
	
	

	{
		sendBtn = CCMenuItemImageLambda::create
		("friendsearch_call.png", "friendsearch_call.png",
		 [=](CCObject* sender){
			 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
//			 int idx = (int)obj->getUserData();
			 ////////////////////////////////
			 // ¬ ¡ˆ∫∏≥ª±‚ - HSP
			 ////////////////////////////////
			 
			 
			 Json::Value p;
			 Json::Value contentJson;
			 
			 contentJson["msg"] = "unknownreq";
			 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
			 contentJson["profile_url"] = hspConnector::get()->myKakaoInfo["profile_image_url"].asString();
			 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
			 //				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
			 p["content"] = GraphDogLib::JsonObjectToString(contentJson);
			 std::string recvId = member["memberID"].asString();
			 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
			 p["receiverMemberID"] = recvId;
			 p["senderMemberID"]=hspConnector::get()->getSocialID();
			 p["type"] = kUnknownFriendRequest;
			 
			 hspConnector::get()->command("sendMessage", p,this, [=](Json::Value r)
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
	
	
	sendBtn->setPosition(ccp(170, 22));
	sendBtn->setTag(kTagGameFriendSend);
	_menu->addChild(sendBtn, kZorderJoinGameFriendSend);
	
	title = CCLabelTTF::create("",mySGD->getFont().c_str(),12);
	title->setPosition(ccp(40,20));
	title->setAnchorPoint(CCPointZero);
	title->setTag(kTagGameFriendNickname);
	title->setColor(ccc3(20, 0, 0));
	cell->addChild(title, 2);
	
	score = CCLabelTTF::create("",mySGD->getFont().c_str(),10);
	score->setPosition(ccp(40,8));
	score->setAnchorPoint(CCPointZero);
	score->setTag(kTagGameFriendLastDate);
	score->setColor(ccc3(20, 0, 0));
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
//	if(m_scrollBar)
//	{
//		m_scrollBar->setBarRefresh();
//	}
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
void JoinGameFriendPopup::searchByIndex(int64_t userIndex)
{
	
	Json::Value contentJson;
	
	contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
	//				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
	
	Json::Value param;
	param["userIndex"] = userIndex;
	hspConnector::get()->command("getUserData", param,this,
															 [=](Json::Value t)
															 {
																 KS::KSLog("zz %", t);
																 if(t["state"] == "error")
																 {
																	 
																	 KHAlertView* av = KHAlertView::create(); 
																	 av->setTitleFileName("msg_error.png");
																	 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
																	 av->setWidth(240);
																	 av->setHeight(240);
																	 av->setTitleHeight(10);
																	 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
																	 av->setCenterY(150);

																	 CCNode* emptyNode = CCNode::create();
																	 auto ttf = CCLabelTTF::create("아이디를 찾을 수 없습니다.", mySGD->getFont().c_str(), 14.f); 
																	 ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
																	 //	con->setAnchorPoint(ccp(0, 0));
																	 //ttf->setAnchorPoint(ccp(0.5f, 0.5f));
																	 ttf->setColor(ccc3(255, 255, 255));
																	 ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, -77));
																	 emptyNode->addChild(ttf);
																	 av->setContentNode(
																			 emptyNode
																			 );
																	 av->setContentSize(ttf->getDimensions());
																	 av->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
																								 [=](CCObject* e) {
																									 CCLOG("ok!!");
																								 });
			

																	 addChild(av, kZorderPopup);
																	 av->show();
																	 return;
																 }
																 Json::Value p;
																 p["content"] = GraphDogLib::JsonObjectToString(contentJson);
																 std::string recvId = t["memberID"].asString();
																 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
																 p["receiverMemberID"] = recvId;
																 p["senderMemberID"] = hspConnector::get()->getSocialID();
																 p["type"] = kUnknownFriendRequest;
																 hspConnector::get()->command
																	 ("sendMessage", p, [=](Json::Value r) {
																		 GraphDogLib::JsonToLog("sendMessage", r);
																		 if(r["result"]["code"].asInt() != GDSUCCESS){
																			 return;
																		 }	
																		 KHAlertView* av = KHAlertView::create(); 
																		 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
																		 av->setWidth(240);
																		 av->setHeight(240);
																		 av->setTitleHeight(10);
																		 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
																		 av->setCenterY(150);

																		 CCNode* emptyNode = CCNode::create();
																		 auto ttf = CCLabelTTF::create("신청했습니다. 수락을 기다리세요.", mySGD->getFont().c_str(), 14.f); 
																		 ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
																		 //	con->setAnchorPoint(ccp(0, 0));
																		 //ttf->setAnchorPoint(ccp(0.5f, 0.5f));
																		 ttf->setColor(ccc3(255, 255, 255));
																		 ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, -77));
																		 emptyNode->addChild(ttf);
																		 av->setContentNode(
																				 emptyNode
																				 );
																		 av->setContentSize(ttf->getDimensions());
																		 av->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
																									 [=](CCObject* e) {
																										 CCLOG("ok!!");
																									 });

																		 addChild(av, kZorderPopup);
																		 av->show();

																	 });
															 });
	
	
	
}
void JoinGameFriendPopup::editBoxReturn(CCEditBox* editBox)
{
	std::string input = editBox->getText();
	long long _id = KS::strToLongLong(input);
	this->searchByIndex(_id - 1000);
//	std::ostringstream oss;
//	oss << _id;
//	std::string strId = oss.str();
//	this->searchById(strId);
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


