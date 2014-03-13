#include "FriendListPopup.h"
#include "RankPopup.h"
#include "SendMessageUtil.h"
#include "InviteEventPopup.h"
#include "JoinGameFriendPopup.h"
#include "UnknownFriends.h"
#include "KnownFriend.h"
#include "KHAlertView.h"
CCSize cellSize3 = CCSizeMake(238, 38);


using namespace boost;
void FriendListPopup::myInit(CCObject* t_close, SEL_CallFunc d_close)
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
	
	CCSprite* back2 = CCSprite::create("friendlist_back.png");
	back2->setPosition(ccp(240, 129));
	addChild(back2, 1);

	
//	CCMenuLambda* _menu = CCMenuLambda::create();
//	_menu->setTouchPriority(-200);
//	_menu->setPropaOnBegan(true);
//	addChild(_menu);
//	_menu->setPosition(ccp(0, 0));
	
	
	CommonButton* closeBtn = CommonButton::createCloseButton(-200);
	closeBtn->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
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
//	
//	_menu->addChild(inviteEventBtn);

	CommonButton* inviteEventBtn = CommonButton::create("친구초대이벤트", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	inviteEventBtn->setBackgroundTypeForDisabled(CommonButtonYellow);
	inviteEventBtn->setTitleColor(ccc3(200, 200, 200));
	inviteEventBtn->setTitleColorForDisable(ccc3(20, 0, 0));
	inviteEventBtn->setFunction([=](CCObject*){
			hspConnector::get()->removeTarget(this);
				 InviteEventPopup* t_rp = InviteEventPopup::create(t_close, d_close);
				 t_rp->setOpenAnimation(false);
				 getParent()->addChild(t_rp, this->getZOrder());
				 removeFromParent();
	});
	inviteEventBtn->setPosition(ccp(169, 255));
	this->addChild(inviteEventBtn, kRP_Z_back+1);
	
	
	
//	// 친구목록
//	auto friendList = CCMenuItemImageLambda::create
//	(
//	 "friendoption_list.png", "friendoption_list.png",
//	 [=](CCObject*){
//		 //																																 (target_close->*delegate_close)();
//	 });
//	friendList->setPosition(ccp(275, 258));
//	friendList->setOpacity(255);
//	_menu->addChild(friendList);
	
	CommonButton* friendList = CommonButton::create("친구 목록", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	friendList->setBackgroundTypeForDisabled(CommonButtonYellow);
	friendList->setTitleColor(ccc3(200, 200, 200));
	friendList->setTitleColorForDisable(ccc3(20, 0, 0));
	friendList->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);

	});
	friendList->setPosition(ccp(272, 255));
	this->addChild(friendList, kRP_Z_back+1);
	friendList->setEnabled(false);
	
//	// 게임 친구맺기
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
//	joinGameFriend->setPosition(ccp(378, 259));
//	joinGameFriend->setOpacity(0);
//	_menu->addChild(joinGameFriend);

	
	CommonButton* joinGameFriend = CommonButton::create("게임친구맺기", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	joinGameFriend->setBackgroundTypeForDisabled(CommonButtonYellow);
	joinGameFriend->setTitleColor(ccc3(200, 200, 200));
	joinGameFriend->setTitleColorForDisable(ccc3(20, 0, 0));
	joinGameFriend->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
		JoinGameFriendPopup* t_rp = JoinGameFriendPopup::create(t_close, d_close);
		t_rp->setOpenAnimation(false);
		getParent()->addChild(t_rp, this->getZOrder());
		removeFromParent();
	});
	joinGameFriend->setPosition(ccp(375, 255));
	this->addChild(joinGameFriend, kRP_Z_back+1);
	
	
	
	m_friendLimitFnt = CCLabelBMFont::create
	(CCString::createWithFormat("%lu/%d", UnknownFriends::getInstance()->getFriends().size(), mySGD->getGameFriendMax())->getCString(),
	 "allfont.fnt");
	m_friendLimitFnt->setPosition(ccp(170, 210));
	addChild(m_friendLimitFnt, 2);
	
	loadRank();
}

FriendListPopup* FriendListPopup::create( CCObject* t_close, SEL_CallFunc d_close )
{
	FriendListPopup* t_rp = new FriendListPopup();
	t_rp->myInit(t_close, d_close);
	t_rp->autorelease();
	return t_rp;
}

void FriendListPopup::finishedOpen()
{
	loadRank();
}



void FriendListPopup::loadRank()
{
	m_scoreList.insert(m_scoreList.end(), UnknownFriends::getInstance()->getFriends().begin(),
										 UnknownFriends::getInstance()->getFriends().end());
	m_scoreList.insert(m_scoreList.end(), KnownFriends::getInstance()->getFriends().begin(),
										 KnownFriends::getInstance()->getFriends().end());
	//		 m_scoreList.insert(UnknownFriends::getInstance()->getFriends());
	//		 m_
	drawRank();

}

void FriendListPopup::drawRank()
{
//	m_scoreList = obj;
	//320x320 ≈◊¿Ã∫Ì ∫‰ ª˝º∫
	rankTableView = FriendListTableView::create(this, CCSizeMake(424, 179), NULL);
	//		CCScale9Sprite* bar = CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
	//																		1						 CCRectMake(10, 7, 53 - 10*2, 23 - 7*2));
//	CCScale9Sprite* bar = CCScale9Sprite::create("card_scroll.png");
//	m_scrollBar = ScrollBar::createScrollbar(rankTableView, -2, NULL, bar);
//	m_scrollBar->setDynamicScrollSize(false);
	rankTableView->setAnchorPoint(CCPointZero);
	
	//kCCScrollViewDirectionVertical : ºº∑Œ Ω∫≈©∑—, kCCScrollViewDirectionHorizontal : ∞°∑Œ Ω∫≈©∑—
	rankTableView->setDirection(kCCScrollViewDirectionVertical);
	
	//√ﬂ∞°Ω√ ¡§∑ƒ ±‚¡ÿ º≥¡§ kCCTableViewFillTopDown : æ∆∑°∫Œ∫–¿∏∑Œ √ﬂ∞°µ , kCCTableViewFillBottomUp : ¿ßø°º≠ ∫Œ≈Õ √ﬂ∞°µ .
	rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	
	//±‚¡ÿ¡° 0,0
	rankTableView->setPosition(ccp(38, 23));
	
	//µ•¿Ã≈Õ∏¶ ∞°¡Æø¿∞Ì≥™ ≈Õƒ° ¿Ã∫•∆Æ∏¶ π›»Ø«ÿ¡Ÿ ¥Î∏Æ¿⁄∏¶ ¿Ã ≈¨∑°Ω∫∑Œ º≥¡§.
	rankTableView->setDelegate(this);
	this->addChild(rankTableView, 1);
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
	
	//≈◊¿Ã∫Ì ∫‰ ª˝º∫ ≥°/////////////////////////////////////////////////////////////////////////////////////////
	
	
}

CCTableViewCell* FriendListPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCLabelTTF* title;
	CCMenuItemLambda* sendBtn, *deleteBtn;
	CCLabelTTF* score;
	FriendData* member = &m_scoreList[idx];
	KS::KSLog("%", *member);
	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CCPoint sendBtnPosition = ccp(324, 18);
	
	
	std::string cellBackFile = "friendlist_list.png";
	
	
	CCSprite* bg = CCSprite::create(cellBackFile.c_str());
	bg->setPosition(ccp(0,-2));
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,0);
	
	CCSprite* profileImg = GDWebSprite::create((*member).profileUrl, "ending_noimg.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kProfileImg);
	profileImg->setPosition(ccp(30, 20));
	profileImg->setScale(29.f / profileImg->getContentSize().width);
	cell->addChild(profileImg, 1);
	
	
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setPosition(ccp(0, 0));
	_menu->setTouchPriority(-300);
	_menu->setTag(kFriendTableTagMenu);
	cell->addChild(_menu, 1);
	int remainTime = ::getHeartSendingRemainTime( (*member).userId, mySGD->getHeartSendCoolTime()	);
	if(remainTime <= 0)
	{
		sendBtn = CCMenuItemImageLambda::create
		("friendlist_coinsend.png", "friendlist_coinsend.png",
		 [=](CCObject* sender){
			 KHAlertView* av = KHAlertView::create(); 
			 // av->setTitleFileName("msg_challenge.png");
			 av->setCloseButton(CCMenuItemImageLambda::create("cardchange_cancel.png", "cardchange_cancel.png",
					 [=](CCObject*){
					 }
					 ));
			 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
			 av->setWidth(240);
			 av->setHeight(240);
			 av->setTitleHeight(10);
			 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
			 av->setCenterY(150);

			 CCNode* emptyNode = CCNode::create();
			 auto ttf = CCLabelTTF::create(str(format("'%||'님에게\n하트를 선물하고,\n카톡 메시지를 보내시겠습니까?\n(하트를 선물하면 소셜포인트 +%|| 드려요.") % member->nick % mySGD->getSPSendHeart()).c_str(), mySGD->getFont().c_str(), 12.f); 
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
					 [=](CCObject* e)
					 {
						 CCLog("ok!!");
						 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
//						 int idx = (int)obj->getUserData();
						 ////////////////////////////////
						 // ¬ ¡ˆ∫∏≥ª±‚ - HSP
						 ////////////////////////////////


						 Json::Value p;
						 Json::Value contentJson;

						 contentJson["msg"] = "";
						 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
						 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
						 //				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
						 std::string recvId = (*member).userId;
						 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
						 //			 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
						 p["receiverMemberID"] = recvId;
						 p["senderMemberID"] = hspConnector::get()->getKakaoID();
						 p["type"] = kHeart;

						 hspConnector::get()->command("sendMessage", p, this,[=](Json::Value r) {
							 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
							 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
							 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];


							 GraphDogLib::JsonToLog("sendMessage", r);
							 if(r["result"]["code"].asInt() != GDSUCCESS){
								 return;
							 }	
							 mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPSendHeart());
							 myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v) {
							 });
							 ostringstream oss;
							 oss << (*member).userId;
							 std::string userIdStr = oss.str();
							 ::setHeartSendTime((*member).userId);
							 obj->removeFromParent(); // 버튼 삭제.

							 //CCMenuItemImageLambda* sendBtn1 = CCMenuItemImageLambda::create("friendlist_coinsend.png", "friendlist_coinsend.png",
							 //[](CCObject*){});
							 //sendBtn1->setPosition(sendBtnPosition);
							 //sendBtn1->setColor(ccc3(100, 100, 100));
							 //_menu->addChild(sendBtn1,2);
							 std::string remainStr = ::getRemainTimeMsg( remainTime );
							 CCLabelTTF* remainFnt = CCLabelTTF::create(remainStr.c_str(), mySGD->getFont().c_str(), 12.f);
							 remainFnt->setPosition(sendBtnPosition);
							 cell->addChild(remainFnt, 1);
							 ////////////////////////////////
							 // ¬ ¡ˆ∫∏≥ª±‚ - ƒ´ƒ´ø¿
							 ////////////////////////////////
							 Json::Value p2;
							 p2["receiver_id"] = (*member).userId;
							 p2["message"] = "하트 받아라.";
							 hspConnector::get()->kSendMessage(p2, [=](Json::Value r) {
								 GraphDogLib::JsonToLog("kSendMessage", r);
								 setInviteSendTime((*member).userId);
							 });
						 });
					 });
			 addChild(av, 99999999);
			 av->show();

		 });
	}
	else
	{
		sendBtn = CCMenuItemImageLambda::create("friendlist_coinsend.png", "friendlist_coinsend.png",
																						[](CCObject*){
																							CCLog("not send");
																						});
		sendBtn->setColor(ccc3(100, 100, 100));
		sendBtn->setVisible(false);
		std::string remainStr = ::getRemainTimeMsg( remainTime );
		CCLabelTTF* remainFnt = CCLabelTTF::create(remainStr.c_str(), mySGD->getFont().c_str(), 12.f);
		remainFnt->setPosition(sendBtnPosition);
		cell->addChild(remainFnt, 1);
	}
	
	
	sendBtn->setPosition(sendBtnPosition);
	sendBtn->setTag(kFriendSendHeart);
	sendBtn->setUserData((void *)idx);
	_menu->addChild(sendBtn,2);
	if((*member).unknownFriend)
	{
		deleteBtn = CCMenuItemImageLambda::create
		("friendlist_delete.png", "friendlist_delete.png",
		 [=](CCObject* sender){
//			 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
//			 int idx = (int)obj->getUserData();
			 Json::Value param;
			 
			 param["memberID"] = hspConnector::get()->getKakaoID();
			 param["friendID"] = (*member).userId;
			 KHAlertView* av = KHAlertView::create(); 
			 av->setCloseOnPress(false);
			 av->setCloseButton(CCMenuItemImageLambda::create("cardchange_cancel.png", "cardchange_cancel.png",
																												[=](CCObject*){
																													av->removeFromParent();
																												}
																											 ));
			 // av->setTitleFileName("msg_challenge.png");
			 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
			 av->setWidth(240);
			 av->setHeight(240);
			 av->setTitleHeight(10);
			 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
			 av->setCenterY(150);

			 CCNode* emptyNode = CCNode::create();
			 auto ttf = CCLabelTTF::create(str(format("%|| 님을 정말 삭제하시겠습니까?\n인연은 소중합니다.") % member->nick).c_str(), mySGD->getFont().c_str(), 14.f); 
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
			 //av->addButton(CommonButton::create("취소", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
										 //[=](CCObject* e) {
											 //CCLog("ok!!");
											 //av->removeFromParent();
										 //});
			 av->addButton(CommonButton::create("친구 삭제", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
										 [=](CCObject* e) {
											 CCLog("ok!!");
											 hspConnector::get()->command ("removefriendeach", param, 
																										 [=](Json::Value r) {
																											 if(r["result"]["code"].asInt() != GDSUCCESS) {
																												 av->removeFromParent();
																												 return;
																											 }
																											 av->removeFromParent();

																											 UnknownFriends::getInstance()->deleteById((*member).userId);

																											 int targetIndex = -1;
																											 for(int i=0; i<m_scoreList.size(); i++) {
																												 if(m_scoreList[i].userId == (*member).userId) {
																													 targetIndex = i;
																													 break;
																												 }
																											 }

																											 if(targetIndex != -1) {
																												 m_scoreList.erase(m_scoreList.begin() + targetIndex);
																												 //								removeFromIndex(m_scoreList, targetIndex);
																												 UnknownFriends::getInstance()->deleteById((*member).userId);
																												 rankTableView->reloadData();
																												 m_friendLimitFnt->setString
																													 (CCString::createWithFormat
																														("%lu.%d",
																														 UnknownFriends::getInstance()->getFriends().size(),
																														 mySGD->getGameFriendMax())->getCString());
																											 }
																										 });
										 });

			 addChild(av, kZorderPopup);
			 av->show();

			 
			 
		 }
		 );
		deleteBtn->setPosition(ccp(380, 20));
		deleteBtn->setTag(kZorderDeleteFriend);
		deleteBtn->setUserData((void *)idx);
		_menu->addChild(deleteBtn, 2);
	}
	
	
	
	
	title = CCLabelTTF::create("",mySGD->getFont().c_str(),15);
	title->setPosition(ccp(55,18));
	title->setAnchorPoint(CCPointZero);
	title->setTag(kUserNickName);
	title->setColor(ccc3(20, 0, 0));
	cell->addChild(title,2);
	
	
	score = CCLabelTTF::create("",mySGD->getFont().c_str(),10);
	score->setPosition(ccp(115,6));
	score->setAnchorPoint(CCPointZero);
	score->setTag(kLastConnectDate);
	score->setColor(ccc3(20, 0, 0));
	cell->addChild(score,2);
	

	
	
	
	
	
	
	//sendBtn->setUserData((void *)&member);
	ostringstream oss2;
	oss2 << (*member).userId;
	if(oss2.str() == hspConnector::get()->getKakaoID()){
		sendBtn->setVisible(false);
	}else{
		sendBtn->setVisible(true);
	}
	
	title->setString((*member).nick.c_str());
	
	
	//int month, day, hour, minute;
	//::timeSpliter(member->lastDate, 0, &month, &day, &hour, &minute, 0);
	//score->setString(boost::str(boost::format("%||/%|| %||:%||") % month % day % hour % minute).c_str());
	score->setString(::getPastTimeMsg(hspConnector::get()->getLastTimestampOnServer(), member->lastTime).c_str());
	//rank->setString((*member)["rankingGrade"].asString().c_str());
	
	return cell;
}

void FriendListPopup::scrollViewDidScroll( CCScrollView* view )
{
//	if(m_scrollBar)
//	{
//		m_scrollBar->setBarRefresh();
//	}
}

void FriendListPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void FriendListPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	
}

CCSize FriendListPopup::cellSizeForTable( CCTableView *table )
{
	return cellSize3;
}

unsigned int FriendListPopup::numberOfCellsInTableView( CCTableView *table )
{
	return m_scoreList.size();
}

void FriendListPopup::touchFriend( CCObject*, CCControlEvent )
{
	
}

CCPoint FriendListPopup::getContentPosition( int t_tag )
{
	CCPoint return_value;
	
	
	return return_value;
}



FriendListTableView* FriendListTableView::create( CCTableViewDataSource* dataSource, CCSize size, CCNode *container )
{
	FriendListTableView *table = new FriendListTableView();
	table->initWithViewSize(size, container);
	table->autorelease();
	table->setDataSource(dataSource);
	table->_updateCellPositions();
	table->_updateContentSize();
	
	return table;
}
