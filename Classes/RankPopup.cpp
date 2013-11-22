// RankPopup.cpp
//

#include "RankPopup.h"
#define LZZ_INLINE inline
using namespace cocos2d::extension;
using namespace std;
namespace
{
  CCSize cellSize = CCSizeMake(238, 47);
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
void RankPopup::finishedOpen ()
                           {
		loadRank();
	}
void RankPopup::finishedClose ()
                            {
		
		(target_close->*delegate_close)();
		
		removeFromParent();
	}
int RankPopup::getHeartIsSendable (std::string userId, int base_s)
        {
		return true;
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
void RankPopup::setHeartSendTime (string userId)
        {
		auto end = chrono::system_clock::now();
		auto currentSecond = chrono::system_clock::to_time_t(end);
		myDSH->setUserIntForStr("heart_" + userId, currentSecond);
//		saveData->setKeyValue(fbid, GameSystem::getCurrentTime_s());
	}
void RankPopup::myInit (CCObject * t_close, SEL_CallFunc d_close)
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
		CCMenuItemLambda* inviteEventBtn = CCMenuItemImageLambda::create(
																															 "rank_default_invite.png", "rank_default_invite.png",
																															 [=](CCObject*){
//																																 (target_close->*delegate_close)();
																																 
																																 InviteEventPopup* t_rp = InviteEventPopup::create(t_close, d_close);
																																 getParent()->addChild(t_rp, this->getZOrder());
																																 removeFromParent();
																																 
																															 });
		inviteEventBtn->setPosition(ccp(380, 290));
		
		_menu->addChild(inviteEventBtn);
		
		loadRank();
	}
void RankPopup::loadRank ()
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
void RankPopup::drawRank (Json::Value obj)
                                      {
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
void RankPopup::closePopup (CCControlButton * obj, CCControlEvent event)
                                                                   {
		
//		gray->runAction(CCSpawn::create(CCFadeOut::create(0.5),CCMoveBy::create(0.5,CCPoint(0,400)),NULL));
		this->runAction(CCSequence::create(CCMoveBy::create(0.5, CCPoint(0,-400)),CCCallFunc::create(this, callfunc_selector(RankPopup::finishedClose)),NULL));
	}
CCTableViewCell * RankPopup::tableCellAtIndex (CCTableView * table, unsigned int idx)
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
		
		
		
		
		std::string cellBackFile = "rank_cell.png";
			
		
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
			 [=](CCObject* sender){
				 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
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
				 std::string recvId = m_scoreList[idx]["user_id"].asString();
				 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' 제거
				 p["receiverMemberID"] = recvId;
				 p["senderMemberID"]=hspConnector::get()->getKakaoID();
				 p["type"]=kHeart;

				 hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																			{
																				//		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																				//		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																				//		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																				GraphDogLib::JsonToLog("sendMessage", r);
																				this->setHeartSendTime(m_scoreList[idx]["user_id"].asString());
																				obj->removeFromParent();
																				
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
//																																						this->closePopup(0,0);
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
				if(last_selected_card_number == loading_card_number)
				{
					if(used_card_img)
					{
						used_card_img->removeFromParent();
						used_card_img = NULL;
					}
					addCardImg(loading_card_number);
				}
				
				if(after_loading_card_number != 0)
				{
					loading_card_number = after_loading_card_number;
					after_loading_card_number = 0;
					
					Json::Value param;
					param["noList"][0] = loading_card_number;
					hspConnector::get()->command("getcardlist", param, json_selector(this, RankPopup::resultLoadedCardInfo));
				}
				else
					loading_card_number = 0;
			}
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
			
			
			if(last_selected_card_number == loading_card_number)
			{
				if(used_card_img)
				{
					used_card_img->removeFromParent();
					used_card_img = NULL;
				}
				addCardImg(loading_card_number);
			}
			
			if(after_loading_card_number != 0)
			{
				loading_card_number = after_loading_card_number;
				after_loading_card_number = 0;
				
				Json::Value param;
				param["noList"][0] = loading_card_number;
				hspConnector::get()->command("getcardlist", param, json_selector(this, RankPopup::resultLoadedCardInfo));
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
			used_card_img->setPosition(ccp(99.f,156.f));
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
			hspConnector::get()->command("getcardlist", param, json_selector(this, RankPopup::resultLoadedCardInfo));
		}
		else
			loading_card_number = 0;
	}
void RankPopup::startDownloadCardImage ()
        {
		CCLog("start download card img");
		ing_download_cnt = 1;
		startDownload();
	}
void RankPopup::startDownload ()
        {
		CCLog("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
		
		StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename,
													  this, callfunc_selector(RankPopup::successAction), this, callfunc_selector(RankPopup::failAction));
	}
void RankPopup::addCardImg (int t_card_number)
        {
		int card_stage = NSDS_GI(kSDS_CI_int1_stage_i, t_card_number);
		int card_grade = NSDS_GI(kSDS_CI_int1_grade_i, t_card_number);
		
		used_card_img = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_visible.png", card_stage, card_grade)->getCString());
		used_card_img->setScale(0.34f);
		used_card_img->setPosition(ccp(99.f,156.f));
		addChild(used_card_img, kRP_Z_usedCardImg);
		
		if(card_grade == 3 && mySD->isAnimationStage(card_stage))
		{
			CCSize ani_size = mySD->getAnimationCutSize(card_stage);
			CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("stage%d_level%d_animation.png", card_stage, card_grade)->getCString(),
												  CCRectMake(0, 0, ani_size.width, ani_size.height));
			t_ani->setPosition(mySD->getAnimationPosition(card_stage));
			used_card_img->addChild(t_ani);
		}
		
		CardCase* t_case = CardCase::create(card_stage, card_grade);
		t_case->setPosition(CCPointZero);
		used_card_img->addChild(t_case);
		
		CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
		recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, t_card_number))->getCString());
		recent_durability_label->setPosition(ccpAdd(recent_durability_label->getPosition(), ccp(6,-1)));
		recent_durability_label->setFontSize(recent_durability_label->getFontSize()+3);
		
		t_case->getTotalDurabilityLabel()->removeFromParent();
	}
void RankPopup::tableCellTouched (CCTableView * table, CCTableViewCell * cell)
                                                                                {
		
		int selectedCardIndex = 0;
		int highScore = 0;
		// 나를 클릭함.
		if(m_scoreList[cell->getIdx()]["user_id"].asString().c_str() == hspConnector::get()->getKakaoID())
		{
			Json::Reader reader;
			Json::Value data;
			reader.parse(m_scoreList[cell->getIdx()]["scoreInfo"]["data"].asString(), data);
			highScore = data.get("allhighscore", 0).asInt();
			
			selectedCardIndex = myDSH->getIntegerForKey(kDSH_Key_selectedCard); // 자기 카드 번호.
		}
		else
		{
			Json::Reader reader;
			Json::Value data;
			reader.parse(m_scoreList[cell->getIdx()]["scoreInfo"]["data"].asString(), data);
			//		Json::Value data = m_scoreList[cell->getIdx()]["scoreInfo"]["data"].asString()
			KS::KSLog("%", data);
			selectedCardIndex = data.get("selectedcard", 0).asInt();
			highScore = data.get("allhighscore", 0).asInt();
		}
		CCLog("card Number %d", selectedCardIndex); // 영호
		auto retStr = NSDS_GS(kSDS_CI_int1_imgInfo_s, selectedCardIndex);
		KS::KSLog("retStr %", retStr); // 영호.
		
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
				used_card_img->setPosition(ccp(99.f,156.f));
				addChild(used_card_img, kRP_Z_usedCardImg);
				
				CCLabelTTF* t_label = CCLabelTTF::create("카드 정보 로딩", mySGD->getFont().c_str(), 20);
				t_label->setColor(ccBLACK);
				t_label->setPosition(ccp(160,215));
				used_card_img->addChild(t_label);
				
				if(loading_card_number == 0)
				{
					loading_card_number = selectedCardIndex;
					
					Json::Value param;
					param["noList"][0] = loading_card_number;
					hspConnector::get()->command("getcardlist", param, json_selector(this, RankPopup::resultLoadedCardInfo));
				}
				else
					after_loading_card_number = selectedCardIndex;
			}
			else // 카드 정보 있음
			{
				addCardImg(selectedCardIndex);
			}
		}
		else
		{
			used_card_img = CCSprite::create("ending_take_card_back.png");
			used_card_img->setScale(0.34f);
			used_card_img->setPosition(ccp(99.f,156.f));
			addChild(used_card_img, kRP_Z_usedCardImg);
		}
		
		
		
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
#undef LZZ_INLINE
