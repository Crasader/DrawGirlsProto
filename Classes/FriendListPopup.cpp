#include "FriendListPopup.h"
#include "RankPopup.h"
#include "SendMessageUtil.h"
#include "InviteEventPopup.h"
#include "JoinGameFriendPopup.h"
#include "UnknownFriends.h"
#include "KnownFriend.h"

#include "KSAlertView.h"


CCSize cellSize3 = CCSizeMake(238, 47);
void FriendListPopup::myInit(CCObject* t_close, SEL_CallFunc d_close)
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
	addChild(back, 0);
	
	CCSprite* back2 = CCSprite::create("rank_friend_back.png");
	back2->setPosition(ccp(240, 160));
	addChild(back2, 0);
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setTouchPriority(-200);
	_menu->setPropaOnBegan(true);
	addChild(_menu);
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
	 });
	friendList->setPosition(ccp(270, 290));
	friendList->setOpacity(255);
	_menu->addChild(friendList);
	
	// 게임 친구맺기
	auto joinGameFriend = CCMenuItemImageLambda::create
	(
	 "rank_gamefriend.png", "rank_gamefriend.png",
	 [=](CCObject*){
		 //																																 (target_close->*delegate_close)();
		 JoinGameFriendPopup* t_rp = JoinGameFriendPopup::create(t_close, d_close);
		 getParent()->addChild(t_rp, this->getZOrder());
		 removeFromParent();
		 
	 });
	joinGameFriend->setPosition(ccp(370, 290));
	joinGameFriend->setOpacity(0);
	_menu->addChild(joinGameFriend);
	
	m_friendLimitFnt = CCLabelBMFont::create
	(CCString::createWithFormat("%lu/%d", UnknownFriends::getInstance()->getFriends().size(), mySGD->getGameFriendMax())->getCString(),
	 "allfont.fnt");
	m_friendLimitFnt->setPosition(ccp(141, 320 - 74));
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
	rankTableView = FriendListTableView::create(this, CCSizeMake(424, 195), NULL);
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
	
	
	
	
	std::string cellBackFile = "rank_gamefriend_cell.png";
	
	
	CCSprite* bg = CCSprite::create(cellBackFile.c_str());
	bg->setPosition(CCPointZero);
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,0);
	
	CCSprite* profileImg = GDWebSprite::create((*member).profileUrl, "no_img.png");
	profileImg->setAnchorPoint(ccp(0.5, 0.5));
	profileImg->setTag(kProfileImg);
	profileImg->setPosition(ccp(27, 22));
	profileImg->setScale(45.f / profileImg->getContentSize().width);
	cell->addChild(profileImg, 1);
	
	
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setPosition(ccp(0, 0));
	_menu->setTouchPriority(-300);
	_menu->setTag(kFriendTableTagMenu);
	cell->addChild(_menu, 1);
	
	if(::getHeartIsSendable( (*member).userId, mySGD->getHeartSendCoolTime() ))
	{
		sendBtn = CCMenuItemImageLambda::create
		("rank_cell_send.png", "rank_cell_send.png",
		 [=](CCObject* sender){
			 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
			 int idx = (int)obj->getUserData();
			 ////////////////////////////////
			 // ¬ ¡ˆ∫∏≥ª±‚ - HSP
			 ////////////////////////////////
			 
			 
			 Json::Value p;
			 Json::Value contentJson;
			 
			 contentJson["msg"] = "";
			 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
			 //				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
			 p["content"] = GraphDogLib::JsonObjectToString(contentJson);
			 std::string recvId = (*member).userId;
			 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
//			 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
			 p["receiverMemberID"] = recvId;
			 p["senderMemberID"] = hspConnector::get()->getKakaoID();
			 p["type"] = kHeart;
			 
			 hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																		{
																			//		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																			//		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																			//		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																			
																			
																			GraphDogLib::JsonToLog("sendMessage", r);
																			if(r["result"]["code"].asInt() != GDSUCCESS)
																				return;
																			
																			mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPSendHeart());
																			myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v)
																													{
																														
																													});
																			ostringstream oss;
																			oss << (*member).userId;
																			std::string userIdStr = oss.str();
																			::setHeartSendTime((*member).userId);
																			obj->removeFromParent(); // 버튼 삭제.
																			
																			CCMenuItemImageLambda* sendBtn1 = CCMenuItemImageLambda::create("rank_cell_notsend.png", "rank_cell_notsend.png",
																																																			[](CCObject*){});
																			sendBtn1->setPosition(ccp(205,22));
																			_menu->addChild(sendBtn1,2);
																			////////////////////////////////
																			// ¬ ¡ˆ∫∏≥ª±‚ - ƒ´ƒ´ø¿
																			////////////////////////////////
																			Json::Value p2;
																			p2["receiver_id"] = (*member).userId;
																			p2["message"] = "하트 받아라.";
																			hspConnector::get()->kSendMessage(p2, [=](Json::Value r)
																																				{
																																					GraphDogLib::JsonToLog("kSendMessage", r);
																																					setInviteSendTime((*member).userId);
																																				});
																		});
		 });
	}
	else
	{
		sendBtn = CCMenuItemImageLambda::create("rank_cell_send.png", "rank_cell_send.png",
																						[](CCObject*){});
	}
	
	
	sendBtn->setPosition(ccp(300,22));
	sendBtn->setTag(kFriendSendHeart);
	sendBtn->setUserData((void *)idx);
	_menu->addChild(sendBtn,2);
	if((*member).unknownFriend)
	{
		deleteBtn = CCMenuItemImageLambda::create
		("rank_friend_list_cancel.png", "rank_friend_list_cancel.png",
		 [=](CCObject* sender){
			 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
			 int idx = (int)obj->getUserData();
			 Json::Value param;
			 
			 param["memberID"] = hspConnector::get()->getKakaoID();
			 param["friendID"] = (*member).userId;
			 KSAlertView* av = KSAlertView::create();
			 av->setCloseOnPress(false);
			 av->setBack9(CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0,0, 150, 150), CCRectMake(13, 45, 122, 92)));
			 
			 av->setBorderScale(0.9f);
			 av->setCenterY(150);
			 
			 auto ttf = CCLabelTTF::create("정말삭제하시겠습니까? 다시 못만날지도...", "", 12.f);
			 ttf->setColor(ccc3(0, 0, 0));
			 
			 av->setContentNode(
													ttf
													);
			 av->setContentSize(ttf->getDimensions());
			 
			 av->addButton(CCMenuItemImageLambda::create
										 (
											"rank_cancel.png",
											"rank_cancel.png",
											[=](CCObject* e){
												av->removeFromParent();
											}
											));
			 
			 av->addButton
			 (
				CCMenuItemImageLambda::create
				(
				 "ui_common_ok.png",
				 "ui_common_ok.png",
				 [=](CCObject* e){
					 
					 hspConnector::get()->command
					 ("removefriendeach", param, [=](Json::Value r)
						{
							if(r["result"]["code"].asInt() != GDSUCCESS)
							{
								av->removeFromParent();
								return;
							}
							av->removeFromParent();
							
							UnknownFriends::getInstance()->deleteById((*member).userId);
							
							int targetIndex = -1;
							for(int i=0; i<m_scoreList.size(); i++)
							{
								if(m_scoreList[i].userId == (*member).userId)
								{
									targetIndex = i;
									break;
								}
							}
							
							if(targetIndex != -1)
							{
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
							// m_scoreList 에서 "user_id" 찾아서 지워서 reload  해야됨.
							
							//												 loadRank();
						});
				 }
				 ));
			 addChild(av, kZorderPopup);
			 av->show();
			 
			 
		 }
		 );
		deleteBtn->setPosition(ccp(350, 22));
		deleteBtn->setTag(kZorderDeleteFriend);
		deleteBtn->setUserData((void *)idx);
		_menu->addChild(deleteBtn, 2);
	}
	
	
	
	
	title = CCLabelTTF::create("","Helvetica",12);
	title->setPosition(ccp(90,28));
	title->setAnchorPoint(CCPointZero);
	title->setTag(kUserNickName);
	cell->addChild(title,2);
	
	
	score = CCLabelTTF::create("","Helvetica",20);
	score->setPosition(ccp(90,5));
	score->setAnchorPoint(CCPointZero);
	score->setTag(kLastConnectDate);
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
	score->setString("최종접속 : ...");
	//rank->setString((*member)["rankingGrade"].asString().c_str());
	
	return cell;
}

void FriendListPopup::scrollViewDidScroll( CCScrollView* view )
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
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
