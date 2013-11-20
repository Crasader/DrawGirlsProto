//
//  InviteEventPopup.cpp
//  DGproto
//
//  Created by ksoo k on 2013. 11. 20..
//
//

#include "InviteEventPopup.h"

#include "RankPopup.h"
void InviteEventPopup::myInit(CCObject* t_close, SEL_CallFunc d_close)
{
	setTouchEnabled(true);
	target_close = t_close;
	delegate_close = d_close;
	
	m_currentInviteCount = myDSH->getUserIntForStr("invitecount", 0);
	m_inviteCountFnt = CCLabelBMFont::create(CCString::createWithFormat("%d", m_currentInviteCount)->getCString(),
																					 "combo.fnt");
	m_inviteCountFnt->setPosition(ccp(160, 220));
	addChild(m_inviteCountFnt, kInvite_Z_count);
	
	
	
	m_currentSelectSprite = NULL;
	
	used_card_img = NULL;
	loading_card_number = 0;
	after_loading_card_number = 0;
	last_selected_card_number = 0;
	
	//		gray = CCSprite::create("back_gray.png");
	//		gray->setPosition(ccp(240,160));
	//		gray->setContentSize(CCSizeMake(600, 400));
	//		addChild(gray, kRP_Z_gray);
	
	CCSprite* back = CCSprite::create("invite_back.png");
	back->setPosition(ccp(240,160));
	addChild(back, kInvite_Z_back);
	
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
	
	CCMenuItemLambda* rankBtn = CCMenuItemImageLambda::create(
																														"rank_friend_rank.png", "rank_friend_rank.png",
																														[=](CCObject*){
																															//																																 (target_close->*delegate_close)();
																															RankPopup* t_rp = RankPopup::create(t_close, d_close);
																															getParent()->addChild(t_rp, this->getZOrder());
																															removeFromParent();
																															
																														});
	rankBtn->setPosition(ccp(380, 290));
	
	_menu->addChild(rankBtn);
	
	
	loadRank();
}

InviteEventPopup* InviteEventPopup::create( CCObject* t_close, SEL_CallFunc d_close )
{
	InviteEventPopup* t_rp = new InviteEventPopup();
	t_rp->myInit(t_close, d_close);
	t_rp->autorelease();
	return t_rp;
}

void InviteEventPopup::finishedOpen()
{
	loadRank();
}

int InviteEventPopup::getInviteIsSendable( std::string userId, int base_s /*= 60 * 60 * 24 * 31*/ ) /* 31일. */
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

void InviteEventPopup::setInviteSendTime( string userId )
{
	auto end = chrono::system_clock::now();
	auto currentSecond = chrono::system_clock::to_time_t(end);
	myDSH->setUserIntForStr("invite_" + userId, currentSecond);
	myDSH->setUserIntForStr("invitecount", myDSH->getUserIntForStr("invitecount", 0) + 1);
	//		saveData->setKeyValue(fbid, GameSystem::getCurrentTime_s());
}

void InviteEventPopup::loadRank()
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

void InviteEventPopup::drawRank( Json::Value obj )
{
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

CCTableViewCell* InviteEventPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
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

	CCSprite* profileImg = GDWebSprite::create((*member)["profile_image_url"].asString(), "no_img.png");
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

void InviteEventPopup::scrollViewDidScroll( CCScrollView* view )
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
}

void InviteEventPopup::scrollViewDidZoom( CCScrollView* view )
{

}

void InviteEventPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{

}

CCSize InviteEventPopup::cellSizeForTable( CCTableView *table )
{
	return cellSize3;
}

unsigned int InviteEventPopup::numberOfCellsInTableView( CCTableView *table )
{
	return m_scoreList.size();
}

void InviteEventPopup::touchFriend( CCObject*, CCControlEvent )
{

}

CCPoint InviteEventPopup::getContentPosition( int t_tag )
{
	CCPoint return_value;


	return return_value;
}















InviteTableView* InviteTableView::create( CCTableViewDataSource* dataSource, CCSize size, CCNode *container )
{
	InviteTableView *table = new InviteTableView();
	table->initWithViewSize(size, container);
	table->autorelease();
	table->setDataSource(dataSource);
	table->_updateCellPositions();
	table->_updateContentSize();

	return table;
}
