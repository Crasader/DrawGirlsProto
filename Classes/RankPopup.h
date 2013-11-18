//
//  RankPopup.h
//  DGproto
//
//  Created by 사원3 on 13. 9. 30..
//
//

#ifndef __DGproto__RankPopup__
#define __DGproto__RankPopup__

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
USING_NS_CC;

using namespace cocos2d::extension;
using namespace std;

enum RankPopupZorder{
	kRP_Z_gray = 1,
	kRP_Z_back,
	kRP_Z_close,
	kRP_Z_content,
	kRP_Z_rankTable,
	kRP_Z_send,
	kRP_Z_profileImg
};

enum RankTableViewTag{
	kRP_RT_title = 0x1000,
	kRP_RT_score,
	kRP_RT_rank,
	kRP_RT_send,
	kRP_RT_menu,
	kRP_RT_profileImg,
};

enum RankPopupMenuTag{
	kRP_MT_close = 0x2000,
	kRP_MT_invite,
	kRP_MT_send,
	kRP_MT_send_close,
	kRP_MT_invite_close,
	kRP_MT_invite_rank,
	kRP_MT_invite_send,
	kRP_MT_invite_send_close
};

enum RankPopupState{
	kRP_State_rank = 1,
	kRP_State_send,
	kRP_State_invite,
	kRP_State_invite_send
};

namespace{
	CCSize cellSize = CCSizeMake(238, 47);
}
#define SAFE_REMOVECHILD(X) do{if(X){ X->removeFromParentAndCleanup(true); X = 0;}}while(0);

class CC_DLL KSEaseBackOut : public CCActionEase
{
public:
	virtual void update(float time)
	{
//    float overshoot = 1.70158f;
    float overshoot = 0.1f;
		
    time = time - 1;
    m_pInner->update(time * time * ((overshoot + 1) * time + overshoot) + 1);
	}
	virtual CCActionInterval* reverse(void)
	{
    return CCEaseBackIn::create(m_pInner->reverse());
	}
	virtual CCObject* copyWithZone(CCZone* pZone)
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
	
public:
	
	/** creates the action */
	static CCEaseBackOut* create(CCActionInterval* pAction)
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
};

class RankTableView : public CCTableView
{
public:
	static RankTableView* create(CCTableViewDataSource* dataSource, CCSize size, CCNode *container)
	{
    RankTableView *table = new RankTableView();
    table->initWithViewSize(size, container);
    table->autorelease();
    table->setDataSource(dataSource);
    table->_updateCellPositions();
    table->_updateContentSize();
		
    return table;
	}

	void setContentOffsetInDuration(CCPoint offset, float dt)
	{
    CCFiniteTimeAction *scroll, *expire;
    
    scroll = KSEaseBackOut::create( CCMoveTo::create(dt, offset) );
    expire = CCCallFuncN::create(this, (SEL_CallFuncN)(&CCScrollView::stoppedAnimatedScroll));
    m_pContainer->runAction(CCSequence::create(scroll, expire, NULL));
    this->schedule(schedule_selector(CCScrollView::performedAnimatedScroll));
	}
};
class RankPopup : public CCLayer, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	static RankPopup* create(CCObject* t_close, SEL_CallFunc d_close)
	{
		RankPopup* t_rp = new RankPopup();
		t_rp->myInit(t_close, d_close);
		t_rp->autorelease();
		return t_rp;
	}
	
	void finishedOpen(){
		loadRank();
	}
	
	void finishedClose(){
		
		(target_close->*delegate_close)();
		removeFromParent();
	}
	
	int getHeartIsSendable(std::string userId, int base_s = 60 * 60 * 24)
	{
		auto end = chrono::system_clock::now();
		auto currentSecond = chrono::system_clock::to_time_t(end);
		int ii = myDSH->getUserIntForStr("heart_" + userId, 0);
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
	void setHeartSendTime(string userId)
	{
		auto end = chrono::system_clock::now();
		auto currentSecond = chrono::system_clock::to_time_t(end);
		myDSH->setUserIntForStr("heart_" + userId, currentSecond);
//		saveData->setKeyValue(fbid, GameSystem::getCurrentTime_s());
	}

	
	void myInit(CCObject* t_close, SEL_CallFunc d_close)
	{
		setTouchEnabled(true);
		target_close = t_close;
		delegate_close = d_close;
		m_currentSelectSprite = NULL;
		
//		gray = CCSprite::create("back_gray.png");
//		gray->setPosition(ccp(240,160));
//		gray->setContentSize(CCSizeMake(600, 400));
//		addChild(gray, kRP_Z_gray);
		
		CCSprite* back = CCSprite::create("ranking_back.png");
		back->setPosition(ccp(240,160));
		addChild(back, kRP_Z_back);
		
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
		
		loadRank();
	}
	
	
	
	void loadRank()
	{
		

		std::function<void(Json::Value e)> p1 = bind(&RankPopup::drawRank, this, std::placeholders::_1);
		//step1 카카오친구목록 로드
		hspConnector::get()->kLoadFriends(Json::Value(),[p1](Json::Value fInfo)
		{
			CCLog("step1 %s",GraphDogLib::JsonObjectToString(fInfo).c_str());

			  
				
			Json::Value appfriends = fInfo["app_friends_info"];
			appfriends.append(hspConnector::get()->myKakaoInfo);
			Json::Value p;
			  
			for(int i=0; i<appfriends.size();i++){
				p["memberIDList"].append(appfriends[i]["user_id"].asString());
				
			}
		  
			p["memberIDList"].append(hspConnector::get()->getKakaoID());
			
			//step2 위클리스코어 목록 읽어옴
			hspConnector::get()->command("getweeklyscorelist",p,[p1,appfriends](Json::Value obj)
			{										   
				CCLog("step2 %s",GraphDogLib::JsonObjectToString(obj).c_str());
				
				//step1에서 받아온 카카오친구정보와 step2에서 받아온 점수정보를 scolrelist에 합침
				GraphDogLib::JsonToLog("friend1", appfriends);
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
				for(auto iter = scorelist.begin(); iter != scorelist.end(); ++iter)
				{
					Json::Value temp = scorelist[iter.key().asString()];
					temp["user_id"] = iter.key().asString();
					scorearray.append(temp);
				}
				
				// 정렬 함
				// Selection Sort
				int N = scorearray.size();
				for (int i = 0; i < (N - 1); i++)
				{
					int minIndex = i;
					
					// Find the index of the minimum element
					for (int j = i + 1; j < N; j++)
					{
            if (scorearray[j]["scoreInfo"].get("score", -1).asInt() > scorearray[minIndex]["scoreInfo"].get("score", -1).asInt())
            {
							minIndex = j;
            }
					}
					
					// Swap if i-th element not already smallest
					if (minIndex > i)
					{
						scorearray[i].swap(scorearray[minIndex]);
//            swap(a[i], a[minIndex]);
					}
				}
				
				
				//결과 돌려줌
				p1(scorearray);
				
//			   Json::Value p;
//			   Json::Value idMap = obj["memberNoMap"];
//				idMap[hspConnector::get()->getKakaoID()]=hspConnector::get()->getHSPMemberNo();
//			   p["past"]=false;
//			   p["rankingKey"]="rankingFactor-3,rankingPeriod-77";
//			   p["requesterMemberNo"]=hspConnector::get()->getHSPMemberNo();
//			   p["gameNo"]=hspConnector::get()->hspNo;
//			   Json::Value::Members m = idMap.getMemberNames();
//			   for (auto iter = m.begin(); iter!=m.end(); ++iter) {
//				   string key = (string)*iter;
//				   p["memberNoList"].append(idMap[key].asInt64());
//			   }
//			   
//			   //step3 HSP ID목록으로 게임랭킹 뽑기
//			   hspConnector::get()->command("GetRankingListByMemberNoList2",p,[delekey,idMap](Json::Value obj)
//				{
//					
//					Json::Value friendInfo;
//					Json::Value::Members m = idMap.getMemberNames();
//					for (auto iter = m.begin(); iter!=m.end(); ++iter) {
//						string kakaoID = (string)*iter;
//						for(int i=0; i<obj["rankingList"].size();i++){
//							if(obj["rankingList"][i]["profile"]["memberNo"].asInt64() == idMap[kakaoID].asInt64()){
//								obj["rankingList"][i]["profile"]["kakaoID"] = kakaoID;
//								friendInfo[kakaoID]=obj["rankingList"][i];
//								//hspConnector::get()->frineds[i]["hspNo"]=obj["memberNoMap"][key].asInt64();
//								break;
//							}
//						}
//					}
//					
//					
//					hspConnector::get()->idMap = idMap;
//					hspConnector::get()->appFriends = friendInfo;
//					
//					//CCLog("%s",GraphDogLib::JsonObjectToString(obj).c_str());
//					jsonDelegator::DeleSel delsel = jsonDelegator::get()->load(delekey);
//					
//					delsel.func(obj);
//					
//					jsonDelegator::get()->remove(delekey);
//				});
										   
		});
		  
	  });
		
		
	}
	
	
	void drawRank(Json::Value obj){
		m_scoreList = obj;
		//테이블 뷰 생성 시작 /////////////////////////////////////////////////////////////////////////////////////////
		
		//320x320 테이블 뷰 생성
		rankTableView = RankTableView::create(this, CCSizeMake(270, 233), NULL);
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
		rankTableView->setPosition(ccp(187, 28));
		
		//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
		rankTableView->setDelegate(this);
		this->addChild(rankTableView, kRP_Z_rankTable);
		rankTableView->setTouchPriority(-200);
		
		int myPosition = rankTableView->minContainerOffset().y;
		for(int i=0; i<m_scoreList.size(); i++)
		{
			if(m_scoreList[i]["user_id"].asString() == hspConnector::get()->getKakaoID())
			{
				myPosition = m_scoreList.size() - i - 1;
				break;
			}
		}
		float yInitPosition = MAX(rankTableView->minContainerOffset().y, -cellSize.height*myPosition + rankTableView->getViewSize().height / 2.f);
		yInitPosition = MIN(0, yInitPosition);
		rankTableView->setContentOffsetInDuration(
						ccp(
							0, yInitPosition)
						, 0.7f);
		//테이블 뷰 생성 끝/////////////////////////////////////////////////////////////////////////////////////////
	}

	
	////////////////////////////////////////////////////////
	// touch button ////////////////////////////////////////
	////////////////////////////////////////////////////////
	
   	void closePopup(CCControlButton *obj, CCControlEvent event){
		
//		gray->runAction(CCSpawn::create(CCFadeOut::create(0.5),CCMoveBy::create(0.5,CCPoint(0,400)),NULL));
		this->runAction(CCSequence::create(CCMoveBy::create(0.5, CCPoint(0,-400)),CCCallFunc::create(this, callfunc_selector(RankPopup::finishedClose)),NULL));
	}
	

	
	
	
	////////////////////////////////////////////////////////
	// tableview	////////////////////////////////////////
	////////////////////////////////////////////////////////
	
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx){
		
		CCLabelTTF* title;
		CCMenuItemLambda* sendBtn;
		CCLabelTTF* score;
		CCLabelTTF* rank;
		Json::Value* member = &m_scoreList[idx];
		KS::KSLog("%", *member);
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		
		
		
		
		std::string cellBackFile = "ui_rank_cell_back.png";
			
		
		CCSprite* bg = CCSprite::create(cellBackFile.c_str());
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,0);
		
		CCSprite* profileImg = GDWebSprite::create((*member)["profile_image_url"].asString(), "ending_take_particle.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setTag(kRP_RT_profileImg);
		profileImg->setPosition(ccp(62, 22));
		profileImg->setScale(45.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kRP_Z_profileImg);
			

		
		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setPosition(ccp(0, 0));
		_menu->setTouchPriority(-300);
		_menu->setTag(kRP_RT_menu);
		cell->addChild(_menu, kRP_Z_send);
		
		if(getHeartIsSendable( m_scoreList[idx]["user_id"].asString() ))
		{
			sendBtn = CCMenuItemImageLambda::create
			("rank_cell_send.png", "rank_cell_send.png",
			 [=](CCObject* _obj){
				 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(_obj);
				 int idx = (int)obj->getUserData();
				 ////////////////////////////////
				 // 쪽지보내기 - HSP
				 ////////////////////////////////
				 
				 
				 Json::Value p;
				 Json::Value contentJson;

				 contentJson["msg"] = "하트받으쇼~";
				 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
//				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
				 p["content"] = GraphDogLib::JsonObjectToString(contentJson);
				 p["receiverMemberID"]=m_scoreList[idx]["user_id"].asString();
				 p["senderMemberID"]=hspConnector::get()->getKakaoID();
				 p["type"]=1;

				 hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																			{
																				
																				//		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																				//		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																				//		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																				
																				
																				GraphDogLib::JsonToLog("sendMessage", r);
																				this->setHeartSendTime(m_scoreList[idx]["user_id"].asString());
																				sendBtn->removeFromParent();
																				
																				CCMenuItemImageLambda* sendBtn1 = CCMenuItemImageLambda::create("rank_cell_notsend.png", "rank_cell_notsend.png",
																																								[](CCObject*){});
																				sendBtn1->setPosition(ccp(205,22));
																				_menu->addChild(sendBtn1,2);
																				////////////////////////////////
																				// 쪽지보내기 - 카카오
																				////////////////////////////////
																				Json::Value p2;
																				p2["receiver_id"] = m_scoreList[idx]["user_id"].asString();
																				p2["message"] = "하트받으세용!";
																				hspConnector::get()->kSendMessage(p2, [=](Json::Value r)
																																					{
																																						GraphDogLib::JsonToLog("kSendMessage", r);
																																						this->closePopup(0,0);
																																					});
																			});
			 });
		}
		else
		{
			sendBtn = CCMenuItemImageLambda::create("rank_cell_notsend.png", "rank_cell_notsend.png",
																							[](CCObject*){});
		}
		
		
		sendBtn->setPosition(ccp(205,22));
		sendBtn->setTag(kRP_MT_send);
		_menu->addChild(sendBtn,2);
		
		title = CCLabelTTF::create("","Helvetica",12);
		title->setPosition(ccp(90,28));
		title->setAnchorPoint(CCPointZero);
		title->setTag(kRP_RT_title);
		cell->addChild(title,2);
		
		
		score = CCLabelTTF::create("","Helvetica",20);
		score->setPosition(ccp(90,5));
		score->setAnchorPoint(CCPointZero);
		score->setTag(kRP_RT_score);
		cell->addChild(score,2);
		
		rank = CCLabelTTF::create("","Helvetica",25);
		rank->setPosition(ccp(10,10));
		rank->setAnchorPoint(CCPointZero);
		rank->setTag(kRP_RT_rank);
		cell->addChild(rank,2);
		
		
		
		
		
		sendBtn->setUserData((void *)idx);
		//sendBtn->setUserData((void *)&member);
		if((*member)["user_id"].asString()==hspConnector::get()->getKakaoID()){
			sendBtn->setVisible(false);
		}else{
			sendBtn->setVisible(true);
		}
		title->setString((*member)["nickname"].asString().c_str());
		score->setString((*member)["scoreInfo"].get("score","0").asString().c_str());
		//rank->setString((*member)["rankingGrade"].asString().c_str());
		
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
		
		int selectedCardIndex = 0;
		int highScore = 71234;
		// 나를 클릭함.
		if(m_scoreList[cell->getIdx()]["user_id"].asString().c_str() == hspConnector::get()->getKakaoID())
		{
			selectedCardIndex = myDSH->getIntegerForKey(kDSH_Key_selectedCard); // 자기 카드 번호.
		}
		else
		{
			Json::Reader reader;
			Json::Value data;
			reader.parse(m_scoreList[cell->getIdx()]["scoreInfo"]["data"].asString(), data);
			//		Json::Value data = m_scoreList[cell->getIdx()]["scoreInfo"]["data"].asString()
			selectedCardIndex = data.get("selectedcard", 0).asInt();
			highScore = data.get("highscore", 71234).asInt();
		}
		CCLog("card Number %d", selectedCardIndex); // 영호
		auto retStr = NSDS_GS(kSDS_CI_int1_imgInfo_s, selectedCardIndex);
		KS::KSLog("retStr %", retStr); // 영호.
		
		std::string scoreStr = CCString::createWithFormat("%d", highScore)->getCString();
		scoreStr = KS::insert_separator(scoreStr, ',', 3); // 3자리 마다 콤마찍기
		CCLabelBMFont* highScoreFnt =
			CCLabelBMFont::create(
						scoreStr.c_str(), "mb_white_font.fnt");
		highScoreFnt->setPosition(ccp(216 / 2.f, 86 / 2.f));
		addChild(highScoreFnt, 3);
		if(m_currentSelectSprite)
		{
			m_currentSelectSprite->removeFromParent();
			m_currentSelectSprite = NULL;
		}
		
		
//		if((*member)["user_id"].asString() == hspConnector::get()->getKakaoID())
		{
			m_currentSelectSprite = CCSprite::create("rank_cell_select.png");
			m_currentSelectSprite->setPosition(CCPointZero - ccp(6, 0));
			m_currentSelectSprite->setAnchorPoint(CCPointZero);
			cell->addChild(m_currentSelectSprite, 2);
		}
		
		
		
		
	}
    virtual CCSize cellSizeForTable(CCTableView *table){
			return cellSize;
	}
	
    virtual unsigned int numberOfCellsInTableView(CCTableView *table){
		return m_scoreList.size();
	}
	
	
	void touchFriend(CCObject*, CCControlEvent){
		
		
	}
		
	CCPoint getContentPosition(int t_tag)
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
	
	void menuAction(CCObject* sender)
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
	
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
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
	
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
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
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
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
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
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
	
	virtual void registerWithTouchDispatcher()
	{
		CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
		pDispatcher->addTargetedDelegate(this, -170, true);
	}
	
protected:

	Json::Value m_scoreList;

	RankTableView* rankTableView;
	
	bool is_menu_enable;
	
	int touched_number;
	CCObject* target_close;
	SEL_CallFunc delegate_close;
	RankPopupState my_state;
	CCControlButton *closeBtn;
	
	CCSprite* m_currentSelectSprite;
	ScrollBar* m_scrollBar;
};

#endif /* defined(__DGproto__RankPopup__) */
