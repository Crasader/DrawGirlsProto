// RankPopup.cpp
//

#include "RankPopup.h"
#include "FriendListPopup.h"
#include "JoinGameFriendPopup.h"
#include "InviteEventPopup.h"
#include "UnknownFriends.h"
#include "KnownFriend.h"
#include "SendMessageUtil.h"

#include <boost/format.hpp>
#include "KHAlertView.h"
#define LZZ_INLINE inline
using namespace cocos2d::extension;
using namespace std;
namespace
{
	CCSize cellSize = CCSizeMake(238, 38);
}
void KSEaseBackOut::update (float time)
{
	//    float overshoot = 1.70158f;
    float overshoot = 0.1f;
	
    time = time - 1;
    m_pInner->update(time * time * ((overshoot + 1) * time + overshoot) + 1);
}
CCActionInterval * KSEaseBackOut::reverse ()
{
    return CCEaseBackIn::create(m_pInner->reverse());
}
CCObject * KSEaseBackOut::copyWithZone (CCZone * pZone)
{
    CCZone* pNewZone = NULL;
    CCEaseBackOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
		//in case of being called at sub class
		pCopy = (CCEaseBackOut*)(pZone->m_pCopyObject);
    }
    else
    {
		pCopy = new CCEaseBackOut();
		pNewZone = new CCZone(pCopy);
    }
	
    pCopy->initWithAction((CCActionInterval *)(m_pInner->copy()->autorelease()));
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}


CCEaseBackOut * KSEaseBackOut::create (CCActionInterval * pAction)
{
    CCEaseBackOut *pRet = new CCEaseBackOut();
    if (pRet)
    {
		if (pRet->initWithAction(pAction))
		{
			pRet->autorelease();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(pRet);
		}
    }
	
    return pRet;
	
}


RankTableView * RankTableView::create (CCTableViewDataSource * dataSource, CCSize size, CCNode * container)
{
    RankTableView *table = new RankTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
	
    return table;
}
void RankTableView::setContentOffsetInDuration (CCPoint offset, float dt)
{
    CCFiniteTimeAction *scroll, *expire;
    
    scroll = KSEaseBackOut::create( CCMoveTo::create(dt, offset) );
    expire = CCCallFuncN::create(this, (SEL_CallFuncN)(&CCScrollView::stoppedAnimatedScroll));
    m_pContainer->runAction(CCSequence::create(scroll, expire, NULL));
    this->schedule(schedule_selector(CCScrollView::performedAnimatedScroll));
}












RankPopup * RankPopup::create (CCObject * t_close, SEL_CallFunc d_close)
{
	RankPopup* t_rp = new RankPopup();
	t_rp->myInit(t_close, d_close);
	t_rp->autorelease();
	return t_rp;
}
RankPopup::~ RankPopup ()
{
	StageImgLoader::sharedInstance()->removeTD();
}
//void RankPopup::finishedOpen ()
//{
//	loadRank();
//}


void RankPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
{
	
	DimmedPopup::init();//if(DimmedPopup::init()==false)
	
	setTouchEnabled(true);
	m_currentSelectSprite = NULL;
	m_highScore = NULL;
	m_highStage = NULL;
	used_card_img = NULL;
	loading_card_number = 0;
	after_loading_card_number = 0;
	last_selected_card_number = 0;
	
	this->setHideFinalAction(t_close,d_close);
	
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	

	this->setBackground("rank_friend_rank_back.png");
	//CCSprite* back = CCSprite::create("rank_back.png");
	//back->setPosition(ccp(240,160));
	//addChild(back, kRP_Z_back);
	
	

	
//	CCMenuLambda* _menu = CCMenuLambda::create();
//	_menu->setTouchPriority(-200);
//	addChild(_menu, kRP_Z_back + 1);
//	_menu->setPosition(ccp(0, 0));
//	_menu->setPropaOnBegan(true);
//	CCMenuItemLambda* closeBtn = CCMenuItemImageLambda::create(
//															   "cardchange_cancel.png", "cardchange_cancel.png",
//															   [=](CCObject*){
//																   (target_close->*delegate_close)();
//																   removeFromParent();
//															   });
//	closeBtn->setPosition(ccp(450, 258));
//	_menu->addChild(closeBtn);

	CommonButton* closeBtn = CommonButton::createCloseButton(-200);
	closeBtn->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
		this->hidePopup();
	});
	
	closeBtn->setPosition(ccp(450, 255));
	this->addChild(closeBtn);
	

	// 분류 지정.
	
	// 카톡 친구.
	
	m_onlyKatok = CommonButton::create("카톡친구", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	m_onlyKatok->setBackgroundTypeForDisabled(CommonButtonYellow);
	m_onlyKatok->setTitleColor(ccc3(200, 200, 200));
	m_onlyKatok->setTitleColorForDisable(ccc3(20, 0, 0));
	m_onlyKatok->setFunction([=](CCObject*){
				 if(m_rankCategory != RankCategory::kKnownFriend)
				 {
					 m_rankCategory = RankCategory::kKnownFriend;
					 if(rankTableView)
					 {
						 rankTableView->removeFromParent();
						 rankTableView = nullptr;
					 }
					 m_currentSelectSprite = nullptr;
					 loadRank();
					 m_onlyKatok->setEnabled(false);
					 m_onlyGameFriend->setEnabled(true);
					 m_totalFriend->setEnabled(true);
				 }
	});
	m_onlyKatok->setPosition(ccp(272, 255));
	this->addChild(m_onlyKatok, kRP_Z_back+1);
//	m_onlyKatok = CCMenuItemImageLambda::create
//	("rank_friend_rank_1.png", "rank_friend_rank_1.png",
//	 [=](CCObject* t)
//	 {
//		 if(m_rankCategory != RankCategory::kKnownFriend)
//		 {
//			 m_rankCategory = RankCategory::kKnownFriend;
//			 if(rankTableView)
//			 {
//				 rankTableView->removeFromParent();
//				 rankTableView = nullptr;
//			 }
//			 m_currentSelectSprite = nullptr;
//			 loadRank();
//			 m_onlyKatok->setOpacity(255);
//			 m_onlyGameFriend->setOpacity(0);
//			 m_totalFriend->setOpacity(0);
//		 }
//	 });
//	
//	m_onlyKatok->setPosition(ccp(275, 259));
//	_menu->addChild(m_onlyKatok, 3);
	
	
	m_onlyGameFriend = CommonButton::create("게임친구", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	m_onlyGameFriend->setBackgroundTypeForDisabled(CommonButtonYellow);
	m_onlyGameFriend->setTitleColor(ccc3(200, 200, 200));
	m_onlyGameFriend->setTitleColorForDisable(ccc3(20, 0, 0));
	m_onlyGameFriend->setFunction([=](CCObject*){
		if(m_rankCategory != RankCategory::kTotalFriend)
		{
			m_rankCategory = RankCategory::kTotalFriend;
			if(rankTableView)
			{
				rankTableView->removeFromParent();
				rankTableView = nullptr;
			}
			m_currentSelectSprite = nullptr;
			loadRank();
			m_onlyKatok->setEnabled(true);
			m_onlyGameFriend->setEnabled(false);
			m_totalFriend->setEnabled(true);
		}
	});
	m_onlyGameFriend->setPosition(ccp(170, 255));
	this->addChild(m_onlyGameFriend, kRP_Z_back+1);
	
	m_totalFriend = CommonButton::create("전체유저", 12, CCSizeMake(100,37), CommonButtonGray, -200);
	m_totalFriend->setBackgroundTypeForDisabled(CommonButtonYellow);
	m_totalFriend->setTitleColor(ccc3(200, 200, 200));
	m_totalFriend->setTitleColorForDisable(ccc3(20, 0, 0));
	m_totalFriend->setFunction([=](CCObject*){
		if(m_rankCategory != RankCategory::kRealTotalFriend)
		{
			m_rankCategory = RankCategory::kRealTotalFriend;
			if(rankTableView)
			{
				rankTableView->removeFromParent();
				rankTableView = nullptr;
			}
			m_currentSelectSprite = nullptr;
			loadRank();
			
			m_onlyKatok->setEnabled(true);
			m_onlyGameFriend->setEnabled(true);
			m_totalFriend->setEnabled(false);
		}
	});
	m_totalFriend->setPosition(ccp(375, 255));
	this->addChild(m_totalFriend, kRP_Z_back+1);
	
	// 전체 친구
//	m_totalFriend = CCMenuItemImageLambda::create
//	("rank_friend_rank_3.png", "rank_friend_rank_3.png",
//	 [=](CCObject* t)
//	 {
//		 if(m_rankCategory != RankCategory::kRealTotalFriend) {
//			 m_rankCategory = RankCategory::kRealTotalFriend;
//			 if(rankTableView) {
//				 rankTableView->removeFromParent();
//				 rankTableView = nullptr;
//			 }
//			 m_currentSelectSprite = nullptr;
//			 loadRank();
//			 m_onlyKatok->setOpacity(0);
//			 m_onlyGameFriend->setOpacity(0);
//			 m_totalFriend->setOpacity(255);
//		 }
//		 //if(m_rankCategory != RankCategory::kTotalFriend)
//		 //{
//			 //m_rankCategory = RankCategory::kTotalFriend;
//			 //if(rankTableView)
//			 //{
//				 //rankTableView->removeFromParent();
//				 //rankTableView = nullptr;
//			 //}
//			 //m_currentSelectSprite = nullptr;
//			 //loadRank();
//			 //m_onlyKatok->setOpacity(0);
//			 //m_onlyGameFriend->setOpacity(0);
//			 //m_totalFriend->setOpacity(255);
//		 //}
//	 });
//	
//	m_totalFriend->setPosition(ccp(378, 259));
//	_menu->addChild(m_totalFriend, 3);
	
	m_rankCategory = RankCategory::kTotalFriend;
	
	loadRank();
	
//	m_onlyKatok->setOpacity(0);
//	m_onlyGameFriend->setOpacity(255);
//	m_totalFriend->setOpacity(0);
	
	
	m_onlyKatok->setEnabled(true);
	m_onlyGameFriend->setEnabled(false);
	m_totalFriend->setEnabled(true);
	
	addChild(KSTimer::create(0.5f, [=]()
													 {
														 for(int i=0; i<m_scoreList.size(); i++)
														 {
															 
															 if(m_scoreList[i]["user_id"].asString().c_str() == hspConnector::get()->getSocialID())
																 
															 {
																 touchCellIndex(i);
																 break;
															 }
														 }
													 }));
}
void RankPopup::loadRank ()
{
	std::function<void(Json::Value e)> p1 = bind(&RankPopup::drawRank, this, std::placeholders::_1);
	std::function<void(Json::Value e)> p2 = bind(&RankPopup::drawTotalRank, this, std::placeholders::_1);

	
	/*
	 "app_friends_info" : [
	 {
	 "friend_nickname" : "",
	 "hashed_talk_user_id" : "BoMOQ0MOgwY",
	 "message_blocked" : false,
	 "nickname" : "경수2",
	 "profile_image_url" : "",
	 "user_id" : "90014050894642625"
	 },
	 */
	
	if(m_rankCategory == RankCategory::kRealTotalFriend)
	{
		Json::Value p;
		p["memberID"] = hspConnector::get()->getSocialID();
		hspConnector::get()->command("getallweeklyscorelist",p,this,[=](Json::Value obj) {
			if(obj["result"]["code"].asInt() != GDSUCCESS){
				return;
			}

			KS::KSLog("%", obj);
			Json::Value scorelist;
			//return;
			//for(unsigned int i=0;i<appfriends.size();i++){
				//string mid = appfriends[i]["user_id"].asString();
				//scorelist[mid]=appfriends[i];
			//}

			for(unsigned int j=0;j<obj["list"].size();j++){
				string mid = obj["list"][j]["memberID"].asString();
				scorelist[mid]["scoreInfo"]=obj["list"][j];
			}
			GraphDogLib::JsonToLog("result", scorelist);
			Json::Value scorearray;
			for(auto iter = scorelist.begin(); iter != scorelist.end(); ++iter) {
				Json::Value temp = scorelist[iter.key().asString()];
				temp["user_id"] = iter.key().asString();
				scorearray.append(temp);
			}

			// 정렬 함
			// Selection Sort
			int N = scorearray.size();
			for (int i = 0; i < (N - 1); i++) {
				int minIndex = i;

				// Find the index of the minimum element
				for (int j = i + 1; j < N; j++) {
					if (scorearray[j]["scoreInfo"].get("score", -1).asInt() > scorearray[minIndex]["scoreInfo"].get("score", -1).asInt()) {
						minIndex = j;
					}
				}

				// Swap if i-th element not already smallest
				if (minIndex > i) {
					scorearray[i].swap(scorearray[minIndex]);
					//            swap(a[i], a[minIndex]);
				}
			}

//			{
//				"friends_nickname" : "영호",
//				"hashed_talk_user_id" : "AM1JWVlJzQA",
//				"message_blocked" : false,
//				"nickname" : "영호",
//				"profile_image_url" : "http://th-p4.talk.kakao.co.kr/th/talkp/wkeGgMCyHT/CZWvl7uekgzKEBYHlBGkV1/ribpwj_110x110_c.jpg",
//				"scoreInfo" :
//				{
//					"data" : "{\"allhighscore\":94707,\"selectedcard\":0}\n",
//					"memberID" : 88741857374149376,
//					"no" : 138,
//					"regDate" : 20140203214344,
//					"regWeek" : 2014006,
//					"score" : 2158276
//				},
//				"user_id" : "88741857374149376"
//			}
			// 위 것들이 scorearray 임.
			//결과 돌려줌
			p2(scorearray);
		});
	}
	else
	{
		Json::Value appfriends;
		if(m_rankCategory == RankCategory::kTotalFriend)
		{
			for(auto i : KnownFriends::getInstance()->getFriends())
			{
				Json::Value v;
				v["friends_nickname"] = i.nick;
				v["hashed_talk_user_id"] = i.hashedTalkUserId;
				v["message_blocked"] = i.messageBlocked;
				v["nickname"] = i.nick;
				v["profile_image_url"] = i.profileUrl;
				v["user_id"] = i.userId;
				appfriends.append(v);
			}
			for(auto i : UnknownFriends::getInstance()->getFriends())
			{
				Json::Value v;
				v["friends_nickname"] = i.nick;
				v["hashed_talk_user_id"] = i.hashedTalkUserId;
				v["message_blocked"] = i.messageBlocked;
				v["nickname"] = i.nick;
				v["profile_image_url"] = i.profileUrl;
				v["user_id"] = i.userId;
				appfriends.append(v);
			}
		}
		else if(m_rankCategory == RankCategory::kKnownFriend)
		{
			for(auto i : KnownFriends::getInstance()->getFriends())
			{
				Json::Value v;
				v["friends_nickname"] = i.nick;
				v["hashed_talk_user_id"] = i.hashedTalkUserId;
				v["message_blocked"] = i.messageBlocked;
				v["nickname"] = i.nick;
				v["profile_image_url"] = i.profileUrl;
				v["user_id"] = i.userId;
				appfriends.append(v);
			}
		}
		appfriends.append(hspConnector::get()->myKakaoInfo);
		Json::Value p;



		if(m_rankCategory == RankCategory::kKnownFriend)
		{
			for(auto i : KnownFriends::getInstance()->getFriends())
			{
				p["memberIDList"].append(i.userId);
			}
		}
		else if(m_rankCategory == RankCategory::kTotalFriend)
		{
			for(auto i : KnownFriends::getInstance()->getFriends())
			{
				p["memberIDList"].append(i.userId);
			}
			for(auto i : UnknownFriends::getInstance()->getFriends())
			{
				p["memberIDList"].append(i.userId);
			}
		}


		p["memberIDList"].append(hspConnector::get()->getSocialID());
		//step2 위클리스코어 목록 읽어옴
		hspConnector::get()->command("getweeklyscorelist",p,this,[p1,appfriends,this](Json::Value obj) {
			// 남은 초.... obj["remainTime"].asInt();
			int remainSeconds = obj["remainTime"].asInt();
			std::string remainStr = ::getRemainTimeMsg(remainSeconds);
			
			CCLabelTTF* remainFnt = CCLabelTTF::create(remainStr.c_str(), mySGD->getFont().c_str(), 12.f);
			remainFnt->setPosition(ccp(408, 219));
			this->addChild(remainFnt, kRP_Z_rankTable);
			CCLOG("step2 %s",GraphDogLib::JsonObjectToString(obj).c_str());


			//step1에서 받아온 카카오친구정보와 step2에서 받아온 점수정보를 scolrelist에 합침
			GraphDogLib::JsonToLog("friend1", appfriends);

			if(obj["result"]["code"].asInt() != GDSUCCESS){
				return;
			}
			Json::Value scorelist;

			for(unsigned int i=0;i<appfriends.size();i++){
				string mid = appfriends[i]["user_id"].asString();
				scorelist[mid]=appfriends[i];
			}

			for(unsigned int j=0;j<obj["list"].size();j++){
				string mid = obj["list"][j]["memberID"].asString();
				scorelist[mid]["scoreInfo"]=obj["list"][j];
			}
			GraphDogLib::JsonToLog("result", scorelist);
			Json::Value scorearray;
			for(auto iter = scorelist.begin(); iter != scorelist.end(); ++iter) {
				Json::Value temp = scorelist[iter.key().asString()];
				temp["user_id"] = iter.key().asString();
				scorearray.append(temp);
			}

			// 정렬 함
			// Selection Sort
			int N = scorearray.size();
			for (int i = 0; i < (N - 1); i++) {
				int minIndex = i;

				// Find the index of the minimum element
				for (int j = i + 1; j < N; j++) {
					if (scorearray[j]["scoreInfo"].get("score", -1).asInt() > scorearray[minIndex]["scoreInfo"].get("score", -1).asInt()) {
						minIndex = j;
					}
				}

				// Swap if i-th element not already smallest
				if (minIndex > i) {
					scorearray[i].swap(scorearray[minIndex]);
					//            swap(a[i], a[minIndex]);
				}
			}

			KS::KSLog("%", scorearray);
			//결과 돌려줌
			p1(scorearray);
		});
	}
}
void RankPopup::drawRank (Json::Value obj)
{
	m_scoreList = obj;
	//테이블 뷰 생성 시작 /////////////////////////////////////////////////////////////////////////////////////////
	
	//320x320 테이블 뷰 생성
	rankTableView = RankTableView::create(this, CCSizeMake(276, 190), NULL);
	//		CCScale9Sprite* bar = CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
	//																		1						 CCRectMake(10, 7, 53 - 10*2, 23 - 7*2));
	CCScale9Sprite* bar = CCScale9Sprite::create("card_scroll.png");
	m_scrollBar = ScrollBar::createScrollbar(rankTableView, -2, NULL, bar);
	m_scrollBar->setDynamicScrollSize(false);
	m_scrollBar->setVisible(false);
	rankTableView->setAnchorPoint(CCPointZero);
	
	//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
	rankTableView->setDirection(kCCScrollViewDirectionVertical);
	
	//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
	rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	
	//기준점 0,0
	rankTableView->setPosition(ccp(172, 20));
	
	//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
	rankTableView->setDelegate(this);
	this->addChild(rankTableView, kRP_Z_rankTable);
	rankTableView->setTouchPriority(-200);
	
	int myPosition = rankTableView->minContainerOffset().y;
	for(int i=0; i<m_scoreList.size(); i++)
	{
		if(m_scoreList[i]["user_id"].asString() == hspConnector::get()->getSocialID())
		{
			myPosition = m_scoreList.size() - i - 1;
			break;
		}
	}
	float yInitPosition = MAX(rankTableView->minContainerOffset().y, -cellSize.height*myPosition + rankTableView->getViewSize().height / 2.f);
	yInitPosition = MIN(rankTableView->maxContainerOffset().y, yInitPosition);
	if(rankTableView->minContainerOffset().y > rankTableView->maxContainerOffset().y)
	{
		yInitPosition = rankTableView->minContainerOffset().y;
	}
	rankTableView->setContentOffsetInDuration(
											  ccp(
												  0, yInitPosition)
											  , 0.3f);
	//테이블 뷰 생성 끝/////////////////////////////////////////////////////////////////////////////////////////
}
void RankPopup::drawTotalRank (Json::Value obj)
{
	m_scoreList = obj;
	KS::KSLog("%", obj);
	//테이블 뷰 생성 시작 /////////////////////////////////////////////////////////////////////////////////////////

	//320x320 테이블 뷰 생성
	rankTableView = RankTableView::create(this, CCSizeMake(276, 190), NULL);
	//		CCScale9Sprite* bar = CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
	//																		1						 CCRectMake(10, 7, 53 - 10*2, 23 - 7*2));
	CCScale9Sprite* bar = CCScale9Sprite::create("card_scroll.png");
	m_scrollBar = ScrollBar::createScrollbar(rankTableView, -2, NULL, bar);
	m_scrollBar->setDynamicScrollSize(false);
	m_scrollBar->setVisible(false);
	rankTableView->setAnchorPoint(CCPointZero);

	//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
	rankTableView->setDirection(kCCScrollViewDirectionVertical);

	//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
	rankTableView->setVerticalFillOrder(kCCTableViewFillTopDown);

	//기준점 0,0
	rankTableView->setPosition(ccp(172, 20));

	//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
	rankTableView->setDelegate(this);
	this->addChild(rankTableView, kRP_Z_rankTable);
	rankTableView->setTouchPriority(-200);

	int myPosition = rankTableView->minContainerOffset().y;
	for(int i=0; i<m_scoreList.size(); i++)
	{
		if(m_scoreList[i]["user_id"].asString() == hspConnector::get()->getSocialID())
		{
			myPosition = m_scoreList.size() - i - 1;
			break;
		}
	}
	float yInitPosition = MAX(rankTableView->minContainerOffset().y, -cellSize.height*myPosition + rankTableView->getViewSize().height / 2.f);
	yInitPosition = MIN(rankTableView->maxContainerOffset().y, yInitPosition);
	if(rankTableView->minContainerOffset().y > rankTableView->maxContainerOffset().y)
	{
		yInitPosition = rankTableView->minContainerOffset().y;
	}
	rankTableView->setContentOffsetInDuration(
			ccp(
					0, yInitPosition)
			, 0.3f);
	//테이블 뷰 생성 끝/////////////////////////////////////////////////////////////////////////////////////////
}
//void RankPopup::closePopup (CCControlButton * obj, CCControlEvent event)
//{
//	//		gray->runAction(CCSpawn::create(CCFadeOut::create(0.5),CCMoveBy::create(0.5,CCPoint(0,400)),NULL));
//	this->runAction(CCSequence::create(CCMoveBy::create(0.5, CCPoint(0,-400)),CCCallFunc::create(this, callfunc_selector(RankPopup::finishedClose)),NULL));
//}
CCTableViewCell * RankPopup::tableCellAtIndex (CCTableView * table, unsigned int idx)
{
	
	CCLabelTTF* userName;
	CCMenuItemLambda* sendBtn;
	CCLabelTTF* score;
	CCLabelTTF* rank;


	CCPoint sendBtnPosition = ccp(245, 20);
	Json::Value* member = &m_scoreList[idx];
	KS::KSLog("%", *member);
	
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	std::string cellBackFile;
	
	if((*member)["user_id"].asString()==hspConnector::get()->getSocialID())
	{
		cellBackFile = "rank_list_me.png";
	}
	else
	{
		int rank = idx + 1;
		if(rank == 1)
			cellBackFile = "rank_list_gold.png";
		else if(rank == 2)
			cellBackFile = "rank_list_silver.png";
		else if(rank == 3)
			cellBackFile = "rank_list_bronze.png";
		else
			cellBackFile = "rank_list_normal.png";
	}
	
	CCSprite* bg = CCSprite::create(cellBackFile.c_str());
	bg->setPosition(CCPointZero + ccp(2, -2));
	bg->setAnchorPoint(CCPointZero);
	cell->addChild(bg,0);
	
	if(m_rankCategory != RankCategory::kRealTotalFriend)
	{
		CCSprite* profileImg = GDWebSprite::create((*member)["profile_image_url"].asString(), "ending_noimg.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setTag(kRP_RT_profileImg);
		profileImg->setPosition(ccp(51, 20));
		profileImg->setScale(28.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kRP_Z_profileImg);
	}
	

	// 순위 표시함
	if(idx == 0)
	{
		CCSprite* rankSprite = CCSprite::create("puzzle_right_rank_gold.png");
		cell->addChild(rankSprite);
		rankSprite->setPosition(ccp(19, 19));
	}
	else if(idx == 1)
	{
		CCSprite* rankSprite = CCSprite::create("puzzle_right_rank_silver.png");
		cell->addChild(rankSprite);
		rankSprite->setPosition(ccp(19, 19));
	}
	else if(idx == 2)
	{
		CCSprite* rankSprite = CCSprite::create("puzzle_right_rank_bronze.png");
		cell->addChild(rankSprite);
		rankSprite->setPosition(ccp(19, 19));
	}
	else
	{
		CCLabelBMFont* rankFnt = CCLabelBMFont::create(boost::str(boost::format("%||") % (idx + 1)).c_str(), "allfont.fnt");
		cell->addChild(rankFnt);
		rankFnt->setPosition(ccp(19, 19));
	}


	// 카톡 마크 붙임.
	if (m_rankCategory != RankCategory::kRealTotalFriend)
	{
		/* code */
		if(KnownFriends::getInstance()->findById((*member)["user_id"].asString()) &&
			 (*member)["user_id"].asString() != hspConnector::get()->myKakaoInfo["user_id"].asString())
		{
			CCSprite* katokMark = CCSprite::create("puzzle_right_rank_kakao.png");
			cell->addChild(katokMark, kRP_Z_profileImg);
			katokMark->setPosition(ccp(45, 27));

		}
	}	
	
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setPosition(ccp(0, 0));
	_menu->setTouchPriority(-300);
	_menu->setTag(kRP_RT_menu);
	cell->addChild(_menu, kRP_Z_send);
	
	int remainTime = ::getHeartSendingRemainTime( m_scoreList[idx]["user_id"].asString(), mySGD->getHeartSendCoolTime() );
	if(remainTime <= 0)
	{
		sendBtn = CCMenuItemImageLambda::create
		("rank_cell_send.png", "rank_cell_send.png",
		 [=](CCObject* sender){

			 KHAlertView* av = KHAlertView::create(); 
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
			 auto ttf = CCLabelTTF::create("하트를 보내겠습니까?", mySGD->getFont().c_str(), 14.f); 
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
			 av->addButton(CommonButton::create("보내기", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
					 [=](CCObject* e) {
						 CCLOG("ok!!");
						 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
						 int idx = (int)obj->getUserData();
						 ////////////////////////////////
						 // 쪽지보내기 - HSP
						 ////////////////////////////////


						 Json::Value p;
						 Json::Value contentJson;

						 contentJson["msg"] = "하트받으쇼~";
						 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
						 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
						 p["content"] = GraphDogLib::JsonObjectToString(contentJson);
						 std::string recvId = m_scoreList[idx]["user_id"].asString();
						 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' 제거
						 p["receiverMemberID"] = recvId;
						 p["senderMemberID"]=hspConnector::get()->getSocialID();
						 p["type"]=kHeart;
						 p["nickname"] = hspConnector::get()->myKakaoInfo["nickname"].asString();

						 hspConnector::get()->command("sendMessage", p, this,[=](Json::Value r) {
							 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
							 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
							 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
							 GraphDogLib::JsonToLog("sendMessage", r);
							 if(r["result"]["code"].asInt() != GDSUCCESS){
								 return;
							 }
							 ::setHeartSendTime(m_scoreList[idx]["user_id"].asString());

//							 mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPSendHeart());
//							 myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v) {
//
//							 });

							 obj->removeFromParent();
							 std::string remainStr = ::getRemainTimeMsg( mySGD->getHeartSendCoolTime() );
							 CCLabelTTF* remainFnt = CCLabelTTF::create(remainStr.c_str(), mySGD->getFont().c_str(), 12.f);

							 CCMenuItemImageLambda* sendBtn1 = CCMenuItemImageLambda::create("rank_cell_notsend.png", "rank_cell_notsend.png",
								 [](CCObject*){});
							 sendBtn1->setPosition(sendBtnPosition);
							 _menu->addChild(sendBtn1,2);
							 sendBtn1->addChild(remainFnt);
							 remainFnt->setPosition(ccp(sendBtn1->getContentSize().width / 2.f, sendBtn1->getContentSize().height / 2.f));
							 ////////////////////////////////
							 // 쪽지보내기 - 카카오
							 ////////////////////////////////
//							 Json::Value p2;
//							 p2["receiver_id"] = m_scoreList[idx]["user_id"].asString();
//							 p2["message"] = "하트받으세용!";
//							 hspConnector::get()->kSendMessage(p2, [=](Json::Value r) {
//								 GraphDogLib::JsonToLog("kSendMessage", r);
//								 //																																						this->closePopup(0,0);
//							 });
						 });
					 });
			 addChild(av, 99999999);
			 av->show();

		 });
	}
	else
	{
		sendBtn = CCMenuItemImageLambda::create("rank_cell_notsend.png", "rank_cell_notsend.png",
												[](CCObject*){});
		std::string remainStr = ::getRemainTimeMsg( remainTime );
		CCLabelTTF* remainFnt = CCLabelTTF::create(remainStr.c_str(), mySGD->getFont().c_str(), 12.f);
		remainFnt->setPosition(ccp(sendBtn->getContentSize().width / 2.f, sendBtn->getContentSize().height / 2.f));
		sendBtn->addChild(remainFnt);
	}
	
	
	sendBtn->setPosition(sendBtnPosition);
	sendBtn->setTag(kRP_MT_send);
	_menu->addChild(sendBtn,2);
	
	CCSprite* decoInfo = CCSprite::create("rank_cardinfo.png");
	decoInfo->setVisible(false);  // 정보보기 버튼 없앰. 
	decoInfo->setPosition(ccp(190, 20));
	cell->addChild(decoInfo, kRP_Z_back + 1);

	userName = CCLabelTTF::create("",mySGD->getFont().c_str(),12);
	userName->setPosition(m_rankCategory == RankCategory::kRealTotalFriend ? ccp(45, 20) : ccp(75,20));
	userName->setAnchorPoint(CCPointZero);
	userName->setTag(kRP_RT_title);
	cell->addChild(userName,2);
	
	
	score = CCLabelTTF::create("",mySGD->getFont().c_str(),19);
	score->setPosition(m_rankCategory == RankCategory::kRealTotalFriend ? ccp(45, 1) : ccp(75,1));
	score->setAnchorPoint(CCPointZero);
	score->setTag(kRP_RT_score);
	cell->addChild(score,2);
	
	rank = CCLabelTTF::create("",mySGD->getFont().c_str(),25);
	rank->setPosition(ccp(10,10));
	rank->setAnchorPoint(CCPointZero);
	rank->setTag(kRP_RT_rank);
	cell->addChild(rank,2);
	
	
	
	
	
	sendBtn->setUserData((void *)idx);
	if((*member)["user_id"].asString() == hspConnector::get()->getSocialID()){
		if(1)
		{
			sendBtn->setVisible(false);
		}else{
			sendBtn->setVisible(true);
		}
	}
	std::string userNameStr;
	if(m_rankCategory == RankCategory::kRealTotalFriend)
	{
		userNameStr = (*member)["scoreInfo"]["nick"].asString();
	}
	else
	{
		userNameStr = (*member)["nickname"].asString();
	}
	userName->setString(userNameStr.c_str());
	score->setString((*member)["scoreInfo"].get("score","0").asString().c_str());
	//rank->setString((*member)["rankingGrade"].asString().c_str());
	
	return cell;
}
void RankPopup::scrollViewDidScroll (CCScrollView * view)
{
	if(m_scrollBar)
	{
		m_scrollBar->setBarRefresh();
	}
}
void RankPopup::scrollViewDidZoom (CCScrollView * view)
{
	
}
void RankPopup::resultLoadedCardInfo (Json::Value result_data)
{
	KS::KSLog("%", result_data);
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
				continue;
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			
//			Json::Value t_card_missile = t_card["missile"];
//			NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//			NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//			NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//			
//			NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//			
//			Json::Value t_ability = t_card["ability"];
//			NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//			for(int j=0;j<t_ability.size();j++)
//			{
//				Json::Value t_abil = t_ability[j];
//				NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//				
//				Json::Value t_option;
//				if(!t_abil["option"].isObject())                    t_option["key"]="value";
//				else												t_option =t_abil["option"];
//				
//				if(t_abil["type"].asInt() == kIC_attack)
//					NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_addTime)
//					NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_fast)
//					NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_silence)
//					NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_doubleItem)
//					NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_longTime)
//					NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//				else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//					NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//			}
			
			NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
			NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
			
			Json::Value t_imgInfo = t_card["imgInfo"];
			
//			bool is_add_cf = false;
			
			if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = t_imgInfo["size"].asInt();
				t_df.img = t_imgInfo["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
				df_list.push_back(t_df);
				// ================================
				
//				CopyFile t_cf;
//				t_cf.from_filename = t_df.filename.c_str();
//				t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//				cf_list.push_back(t_cf);
//				
//				is_add_cf = true;
			}
			
			Json::Value t_aniInfo = t_card["aniInfo"];
			NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
			if(t_aniInfo["isAni"].asBool())
			{
				Json::Value t_detail = t_aniInfo["detail"];
				NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
				
				Json::Value t_loopSeq = t_detail["loopSeq"];
				for(int j=0;j<t_loopSeq.size();j++)
					NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
				
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
				NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
				
				if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_detail["size"].asInt();
					t_df.img = t_detail["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
				
//				if(is_add_cf)
//				{
//					CopyFile t_cf = cf_list.back();
//					cf_list.pop_back();
//					t_cf.is_ani = true;
//					t_cf.cut_width = t_detail["cutWidth"].asInt();
//					t_cf.cut_height = t_detail["cutHeight"].asInt();
//					t_cf.position_x = t_detail["positionX"].asInt();
//					t_cf.position_y = t_detail["positionY"].asInt();
//					
//					t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//					
//					cf_list.push_back(t_cf);
//				}
			}
			
			NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
			NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
			NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
			NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
			NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
			NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
			
			int sound_cnt = t_card["sound"].size();
			NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
			for(int j=1;j<=sound_cnt;j++)
			{
				NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
			}
			
			NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
			
			Json::Value t_silImgInfo = t_card["silImgInfo"];
			NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
			if(t_silImgInfo["isSil"].asBool())
			{
				if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
				{
					// check, after download ----------
					DownloadFile t_df;
					t_df.size = t_silImgInfo["size"].asInt();
					t_df.img = t_silImgInfo["img"].asString().c_str();
					t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
					t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
					df_list.push_back(t_df);
					// ================================
				}
			}
			
			Json::Value t_faceInfo = t_card["faceInfo"];
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "")
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
				NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
				{
					DownloadFile t_df1;
					t_df1.size = t_faceInfo["size"].asInt();
					t_df1.img = t_faceInfo["ccbi"].asString().c_str();
					t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
					t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df1);
				}
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
				{
					DownloadFile t_df2;
					t_df2.size = t_faceInfo["size"].asInt();
					t_df2.img = t_faceInfo["plist"].asString().c_str();
					t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
					t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df2);
				}
				
				if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
				{
					DownloadFile t_df3;
					t_df3.size = t_faceInfo["size"].asInt();
					t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
					t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
					t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
					df_list.push_back(t_df3);
				}
				
//				if(!is_add_cf)
//				{
//					CopyFile t_cf;
//					t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//					t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//					cf_list.push_back(t_cf);
//					
//					is_add_cf = true;
//				}
//				
//				CopyFile t_cf = cf_list.back();
//				cf_list.pop_back();
//				t_cf.is_ccb = true;
//				t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//				
//				cf_list.push_back(t_cf);
			}
			else
			{
				NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
			}
			mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		}
		
		if(df_list.size() > 0) // need download
		{
			startDownloadCardImage();
		}
		else
		{
			if(last_selected_card_number == loading_card_number)
			{
				if(used_card_img)
				{
					used_card_img->removeFromParent();
					used_card_img = NULL;
				}
				addCardImg(loading_card_number, 0, "-1");
			}
			
			if(after_loading_card_number != 0)
			{
				loading_card_number = after_loading_card_number;
				after_loading_card_number = 0;
				
				Json::Value param;
				param["noList"][0] = loading_card_number;
				hspConnector::get()->command("getcardlist", param, this,json_selector(this, RankPopup::resultLoadedCardInfo));
			}
			else
				loading_card_number = 0;
		}
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		if(last_selected_card_number == loading_card_number)
		{
			if(used_card_img)
			{
				used_card_img->removeFromParent();
				used_card_img = NULL;
			}
			addCardImg(loading_card_number, 0, "-1");
		}
		
		if(after_loading_card_number != 0)
		{
			loading_card_number = after_loading_card_number;
			after_loading_card_number = 0;
			
			Json::Value param;
			param["noList"][0] = loading_card_number;
			hspConnector::get()->command("getcardlist", param, this,json_selector(this, RankPopup::resultLoadedCardInfo));
		}
		else
			loading_card_number = 0;
	}
	else
	{
		failAction();
	}
}
void RankPopup::successAction ()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img);
	
	if(ing_download_cnt >= df_list.size())
	{
//		for(int i=0;i<cf_list.size();i++)
//		{
//			mySIL->removeTextureCache(cf_list[i].from_filename);
//			mySIL->removeTextureCache(cf_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
////			if(cf_list[i].is_ani)
////			{
////				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
////				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
////				target_img->addChild(ani_img);
////			}
////			
////			if(cf_list[i].is_ccb)
////			{
////				CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + cf_list[i].ccb_filename.c_str()).first;
////				face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
////				target_img->addChild(face_img);
////			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->begin();
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(cf_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//                CCLOG("failed!!! card reduce : %s", cf_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
		
		df_list.clear();
//		cf_list.clear();
		
		
		if(last_selected_card_number == loading_card_number)
		{
			if(used_card_img)
			{
				used_card_img->removeFromParent();
				used_card_img = NULL;
			}
			addCardImg(loading_card_number, 0, "-1");
		}
		
		if(after_loading_card_number != 0)
		{
			loading_card_number = after_loading_card_number;
			after_loading_card_number = 0;
			
			Json::Value param;
			param["noList"][0] = loading_card_number;
			hspConnector::get()->command("getcardlist", param, this,json_selector(this, RankPopup::resultLoadedCardInfo));
		}
		else
			loading_card_number = 0;
	}
	else
	{
		ing_download_cnt++;
		startDownload();
	}
}
void RankPopup::failAction ()
{
	if(last_selected_card_number == loading_card_number)
	{
		if(used_card_img)
		{
			used_card_img->removeFromParent();
			used_card_img = NULL;
		}
		
		used_card_img = CCSprite::create("ending_take_card_back.png");
		used_card_img->setScale(0.34f);
		used_card_img->setPosition(ccp(95.f,144.f));
		addChild(used_card_img, kRP_Z_usedCardImg);
		
		CCLabelTTF* t_label = CCLabelTTF::create("정보 로드 실패", mySGD->getFont().c_str(), 20);
		t_label->setColor(ccBLACK);
		t_label->setPosition(ccp(160,215));
		used_card_img->addChild(t_label);
	}
	
	if(after_loading_card_number != 0)
	{
		loading_card_number = after_loading_card_number;
		after_loading_card_number = 0;
		
		Json::Value param;
		param["noList"][0] = loading_card_number;
		hspConnector::get()->command("getcardlist", param, this,json_selector(this, RankPopup::resultLoadedCardInfo));
	}
	else
		loading_card_number = 0;
}
void RankPopup::startDownloadCardImage ()
{
	CCLOG("start download card img");
	ing_download_cnt = 1;
	startDownload();
}
void RankPopup::startDownload ()
{
	CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename,
												  this, callfunc_selector(RankPopup::successAction), this, callfunc_selector(RankPopup::failAction));
}
void RankPopup::addCardImg (int t_card_number, int t_card_level, string t_passive)
{
	used_card_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", t_card_number)->getCString());
	used_card_img->setScale(0.34f);
	used_card_img->setPosition(ccp(95.f,144.f));
	addChild(used_card_img, kRP_Z_usedCardImg);
	
	if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, t_card_number))
	{
		CCSize ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card_number));
		CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("card%d_animation.png", t_card_number)->getCString(),
											  CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card_number)));
		used_card_img->addChild(t_ani);
	}
	
//	if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, t_card_number))
//	{
//		CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card_number)).first;
//		ccb_img->setPosition(ccp(160,215));
//		used_card_img->addChild(ccb_img);
//	}
	
	CardCase* t_case = CardCase::create(t_card_number, t_card_level, t_passive);
	t_case->setPosition(CCPointZero);
	used_card_img->addChild(t_case);
	
	CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
//	recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, t_card_number))->getCString());
//	recent_durability_label->setPosition(ccpAdd(recent_durability_label->getPosition(), ccp(6,-1)));
	recent_durability_label->setFontSize(recent_durability_label->getFontSize()+3);
	
//	t_case->getTotalDurabilityLabel()->removeFromParent();
}
void RankPopup::tableCellTouched (CCTableView * table, CCTableViewCell * cell)
{
	touchCellIndex(cell->getIdx());
}
CCSize RankPopup::cellSizeForTable (CCTableView * table)
{
	return cellSize;
}
unsigned int RankPopup::numberOfCellsInTableView (CCTableView * table)
{
	return m_scoreList.size();
}
void RankPopup::touchFriend (CCObject * a, CCControlEvent b)
{
	
	
}
CCPoint RankPopup::getContentPosition (int t_tag)
{
	CCPoint return_value;
	
	if(t_tag == kRP_MT_invite)					return_value = ccp(240,43);
	else if(t_tag == kRP_MT_close)				return_value = ccp(345,283);
	else if(t_tag == kRP_MT_send)				return_value = ccp(240,230);
	else if(t_tag == kRP_MT_send_close)			return_value = ccp(193,59);
	else if(t_tag == kRP_MT_invite_close)		return_value = ccp(345,283);
	else if(t_tag == kRP_MT_invite_rank)		return_value = ccp(240,43);
	else if(t_tag == kRP_MT_invite_send)		return_value = ccp(240,230);
	else if(t_tag == kRP_MT_invite_send_close)	return_value = ccp(193,59);
	
	return return_value;
}
void RankPopup::menuAction (CCObject * sender)
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
bool RankPopup::ccTouchBegan (CCTouch * pTouch, CCEvent * pEvent)
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
void RankPopup::ccTouchMoved (CCTouch * pTouch, CCEvent * pEvent)
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
void RankPopup::ccTouchEnded (CCTouch * pTouch, CCEvent * pEvent)
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
void RankPopup::ccTouchCancelled (CCTouch * pTouch, CCEvent * pEvent)
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
void RankPopup::registerWithTouchDispatcher ()
{
	CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
	pDispatcher->addTargetedDelegate(this, -170, true);
}

void RankPopup::touchCellIndex(int idx)
{
	
	int selectedCardIndex = 0;
	int highScore = 0;
	int highStage = 0;
	// 나를 클릭함.
	if(m_scoreList[idx]["user_id"].asString().c_str() == hspConnector::get()->getSocialID())
	{
		Json::Reader reader;
		Json::Value data;

		reader.parse(m_scoreList[idx]["scoreInfo"]["data"].asString(), data);
		highScore = data.get("allhighscore", 0).asInt();
		highStage = data.get("highstage", 0).asInt();
		selectedCardIndex = myDSH->getIntegerForKey(kDSH_Key_selectedCard); // 자기 카드 번호.
	}
	else
	{
		Json::Reader reader;
		Json::Value data;
		reader.parse(m_scoreList[idx]["scoreInfo"]["data"].asString(), data);
		//		Json::Value data = m_scoreList[cell->getIdx()]["scoreInfo"]["data"].asString()
		KS::KSLog("%", data);
		selectedCardIndex = data.get("selectedcard", 0).asInt();
		highScore = data.get("allhighscore", 0).asInt();
		highStage = data.get("highstage", 0).asInt();
	}
	CCLOG("card Number %d", selectedCardIndex);
	auto retStr = NSDS_GS(kSDS_CI_int1_imgInfo_s, selectedCardIndex);
	KS::KSLog("retStr %", retStr);
	
	if(used_card_img)
	{
		used_card_img->removeFromParent();
		used_card_img = NULL;
	}
	
	last_selected_card_number = selectedCardIndex;
	if(selectedCardIndex != 0)
	{
		if(retStr == "") // 카드 정보 없음
		{
			used_card_img = CCSprite::create("ending_take_card_back.png");
			used_card_img->setScale(0.34f);
			used_card_img->setPosition(ccp(95.f,144.f));
			addChild(used_card_img, kRP_Z_usedCardImg);
			
			CCLabelTTF* t_label = CCLabelTTF::create("카드 정보 로딩", mySGD->getFont().c_str(), 20);
			t_label->setColor(ccBLACK);
			t_label->setScale(2);
			t_label->setPosition(ccp(160,215));
			used_card_img->addChild(t_label);
			
			if(loading_card_number == 0)
			{
				loading_card_number = selectedCardIndex;
				
				Json::Value param;
				param["noList"][0] = loading_card_number;
				hspConnector::get()->command("getcardlist", param, this,json_selector(this, RankPopup::resultLoadedCardInfo));
			}
			else
				after_loading_card_number = selectedCardIndex;
		}
		else // 카드 정보 있음
		{
			int t_card_level;
			string t_card_passive;
			
			// 자기 자신을 찍음
			if(m_scoreList[idx]["user_id"].asString().c_str() == hspConnector::get()->getSocialID())
			{
//				t_card_level = myDSH->getIntegerForKey(kDSH_Key_cardLevel_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
//				t_card_passive = myDSH->getStringForKey(kDSH_Key_cardPassive_int1, myDSH->getIntegerForKey(kDSH_Key_selectedCard));
			}
			else
			{
				bool is_found = false;
				for(auto i : UnknownFriends::getInstance()->getFriends())
				{
					if(i.userId == m_scoreList[idx]["user_id"].asString())
					{
						is_found = true;
						Json::Value t_user_data = i.userData;
//						t_card_level = t_user_data.get(myDSH->getKey(kDSH_Key_selectedCardLevel), 1).asInt();
//						t_card_passive = t_user_data.get(myDSH->getKey(kDSH_Key_selectedCardPassive), "").asString();
						break;
					}
				}
				for(auto i : KnownFriends::getInstance()->getFriends())
				{
					if(i.userId == m_scoreList[idx]["user_id"].asString())
					{
						Json::Value t_user_data = i.userData;
//						t_card_level = t_user_data.get(myDSH->getKey(kDSH_Key_selectedCardLevel), 1).asInt();
//						t_card_passive = t_user_data.get(myDSH->getKey(kDSH_Key_selectedCardPassive), "").asString();
						break;
					}
				}
			}
			
//			m_scoreList[cell->getIdx()]["user_id"]
			addCardImg(selectedCardIndex, t_card_level, t_card_passive);
		}
	}
	else
	{
		used_card_img = CCSprite::create("ending_take_card_back.png");
		used_card_img->setScale(0.34f);
		used_card_img->setPosition(ccp(95.f,144.f));
		addChild(used_card_img, kRP_Z_usedCardImg);
	}
//	[a imageNamed:]
	
	if(m_highScore)
	{
		m_highScore->removeFromParent();
		m_highScore = NULL;
	}
	if(m_highStage)
	{
		m_highStage->removeFromParent();
		m_highStage = NULL;
	}
	std::string scoreStr = CCString::createWithFormat("%d", highScore)->getCString();
	std::string stageStr = CCString::createWithFormat("%d", highStage)->getCString();
	scoreStr = KS::insert_separator(scoreStr, ',', 3); // 3자리 마다 콤마찍기
	stageStr = KS::insert_separator(stageStr, ',', 3); // 3자리 마다 콤마찍기
	m_highScore =
	CCLabelBMFont::create(
						  scoreStr.c_str(), "allfont.fnt");
	m_highScore->setScale(0.5f);
	m_highScore->setPosition(ccp(125, 27));
	addChild(m_highScore, 3);

	m_highStage =
		CCLabelBMFont::create(
				stageStr.c_str(), "allfont.fnt");
	m_highStage->setScale(0.5f);
	m_highStage->setPosition(ccp(125, 43));
	addChild(m_highStage, 3);
	if(m_currentSelectSprite)
	{
		m_currentSelectSprite->removeFromParent();
		m_currentSelectSprite = NULL;
	}
	
	
	//		if((*member)["user_id"].asString() == hspConnector::get()->getSocialID())
	{
		m_currentSelectSprite = CCSprite::create("rank_cell_select.png");
		m_currentSelectSprite->setPosition(CCPointZero - ccp(3, 1));
		m_currentSelectSprite->setAnchorPoint(CCPointZero);
		if(rankTableView->cellAtIndex(idx))
		{
			rankTableView->cellAtIndex(idx)->addChild(m_currentSelectSprite, 2);
		}
	}
	
}
#undef LZZ_INLINE
