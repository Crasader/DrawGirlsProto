// MailPopup.cp180

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
#include "HatGacha.h"
#include "ASPopupView.h"

#include "UnknownFriends.h"
#include "KnownFriend.h"

#include "StarGoldData.h"
#include "GachaPurchase.h"
#include "StartSettingScene.h"
#include "KHAlertView.h"
#define LZZ_INLINE inline

using namespace std;
namespace
{
  CCSize mailCellSize = CCSizeMake(188, 39);
}
MailPopup * MailPopup::create (CCObject * t_close, SEL_CallFunc d_close, std::function<void(void)> heartRefresh)
{
	MailPopup* t_rp = new MailPopup();
	t_rp->myInit(t_close, d_close, heartRefresh);
	t_rp->autorelease();
	return t_rp;
}
//void MailPopup::finishedOpen ()
//{
//	loadMail();
//}
//void MailPopup::finishedClose ()
//{
//	
//	(target_close->*delegate_close)();
//	removeFromParent();
//}
void MailPopup::myInit (CCObject * t_close, SEL_CallFunc d_close, std::function<void(void)> heartRefresh)
{
	DimmedPopup::init();
	this->setHideFinalAction(t_close, d_close);
	this->setBackground("postbox_back.png");
	
//	target_close = t_close;
//	delegate_close = d_close;
	m_heartRefresh = heartRefresh;
	m_mailFilter = MailFilter::kTotal;	
	
	
	
#if 0 // 싹 지우는 루틴..
	Json::Value p;
	p["memberID"] = hspConnector::get()->getSocialID();
	p["type"] = 0;
	hspConnector::get()->command("removeallmessage",p,this,[](Json::Value r){});
#endif
	
	m_popupState = PostBoxState::kNoMenu;
	setTouchEnabled(true);
	isLoaded = false;
	
	CCMenuLambda* _menu = CCMenuLambda::create();
	_menu->setTouchPriority(-200);
//	_menu->setTouchEnabled(false); // 임시...
//	CCSprite* back = CCSprite::create("postbox_back.png");
//	back->setPosition(ccp(240,160));
//	addChild(back, kMP_Z_back);
//	back->addChild(_menu);
//	_menu->setPosition(ccp(0, 0));
	
//	CCSprite* totalFilterOn = CCSprite::create("postbox_all.png");
//	back->addChild(totalFilterOn);
//	totalFilterOn->setPosition(ccp(53, 258));	
//
//	CCSprite* coinFilterOn = CCSprite::create("postbox_coin.png");
//	back->addChild(coinFilterOn);
//	coinFilterOn->setPosition(ccp(118, 258));	
//
//	CCSprite* challengeFilterOn = CCSprite::create("postbox_challenge.png");
//	back->addChild(challengeFilterOn);
//	challengeFilterOn->setPosition(ccp(182, 258));	
//
//	CCSprite* ticketFilterOn = CCSprite::create("postbox_ticket.png");
//	back->addChild(ticketFilterOn);
//	ticketFilterOn->setPosition(ccp(247, 258));	
//
//	CCSprite* helpFilterOn = CCSprite::create("postbox_help.png");
//	back->addChild(helpFilterOn);
//	helpFilterOn->setPosition(ccp(312, 258));	
//
//	CCSprite* giftFilterOn = CCSprite::create("postbox_gift.png");
//	back->addChild(giftFilterOn);
//	giftFilterOn->setPosition(ccp(377, 258));	

	
	CommonButton* allReceive = CommonButton::create("모두받기", 12, CCSizeMake(100,40), CommonButtonGreen, -200);
	allReceive->setTitleColor(ccc3(20, 0, 0));
	allReceive->setBackgroundTypeForDisabled(CommonButtonGray);
	allReceive->setFunction([=](CCObject*){
			if(m_mailFilter == MailFilter::kHeart) {
				std::vector<int> mailNumbers;
				for(int i=0; i<m_mailList.size(); i++)
				{
					if(m_mailList[i]["type"].asInt() == MessageRecvType::kHeart)
					{
						mailNumbers.push_back(m_mailList[i]["no"].asInt());
					}
				}
				this->removeMessageByList(mailNumbers, hspConnector::get()->myKakaoInfo["user_id"].asInt64(), [=](Json::Value){
					//코인올리기
					myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+mailNumbers.size());
					m_heartRefresh();
				});
			}
			else if(m_mailFilter == MailFilter::kTicket){
				std::vector<int> mailNumbers;
				KS::KSLog("%", m_mailList);
				std::vector<int> ticketIndexs;
				for(int i=0; i<m_mailList.size(); i++)
				{
					if(m_mailList[i]["type"].asInt() == MessageRecvType::kTicketResult)
					{
						mailNumbers.push_back(m_mailList[i]["no"].asInt());
						ticketIndexs.push_back(i);
					}
				}
				Json::Value puzzleTicket;
				for(auto i : ticketIndexs)
				{
					Json::Value one;
					Json::Reader reader;
					Json::Value contentObj;
					reader.parse(m_mailList[i]["content"].asString(), contentObj);
					one["puzzlenumber"] = contentObj["puzzlenumber"].asInt();
					one["friendID"] = m_mailList[i]["friendID"].asInt64();
					puzzleTicket.append(one);
				}
				this->removeMessageByList(mailNumbers, hspConnector::get()->myKakaoInfo["user_id"].asInt64(), [=](Json::Value r){
					// to do
					if(r["result"]["code"].asInt() != GDSUCCESS){
						return;
					}
					KS::KSLog("%", puzzleTicket);
					// 영호 puzzleTicket 쓰면 됨. 이런 구조임.
					/*
					 [
					 
					 {
					 "friendID" : 90280374354071376,
					 "puzzlenumber" : 4
					 },
					 
					 {
					 "friendID" : 90280374354071376,
					 "puzzlenumber" : 4
					 },
					 
					 {
					 "friendID" : 90280374354071376,
					 "puzzlenumber" : 4
					 }
					 ]
					 */
					
					for(int t_i = 0;t_i<puzzleTicket.size();t_i++)
					{
						string t_friend_id = puzzleTicket[t_i]["friendID"].asString();
						int t_puzzle_number = puzzleTicket[t_i]["puzzlenumber"].asInt();
						if(mySGD->getPuzzleHistory(t_puzzle_number-1).is_clear.getV() && !mySGD->getPuzzleHistory(t_puzzle_number).is_open.getV()) {
							bool good_ticket = true;
							int have_ticket_cnt = myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt);
							for(int i=1;i<=have_ticket_cnt && good_ticket;i++) {
								string ticket_user_id = myDSH->getStringForKey(kDSH_Key_ticketUserId_int1, i);
								if(ticket_user_id == t_friend_id){
									good_ticket = false;
								}
							}
							
							if(good_ticket && have_ticket_cnt < NSDS_GI(t_puzzle_number, kSDS_PZ_ticket_i)) {
								int have_ticket_cnt = myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt) + 1;
								myDSH->setIntegerForKey(kDSH_Key_haveTicketCnt, have_ticket_cnt);
								myDSH->setStringForKey(kDSH_Key_ticketUserId_int1, have_ticket_cnt, t_friend_id);
								
								int need_ticket_cnt = NSDS_GI(t_puzzle_number, kSDS_PZ_ticket_i);
								
								CCLabelTTF* ticket_cnt_label = (CCLabelTTF*)((PuzzleMapScene*)getTarget())->getChildByTag(kPMS_MT_ticketCnt);
								if(ticket_cnt_label){
									ticket_cnt_label->setString(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt),
																																				 NSDS_GI(t_puzzle_number, kSDS_PZ_ticket_i))->getCString());
								}
								if(need_ticket_cnt <= have_ticket_cnt) {
									
									((PuzzleMapScene*)getTarget())->removeChildByTag(kPMS_MT_buyPuzzle);
									((PuzzleMapScene*)getTarget())->removeChildByTag(kPMS_MT_callTicket);
									((PuzzleMapScene*)getTarget())->removeChildByTag(kPMS_MT_ticketCnt);
									((PuzzleMapScene*)getTarget())->removeChildByTag(kPMS_MT_puzzleOpenTitle);
									
									((PuzzleMapScene*)getTarget())->openPuzzleAction(t_puzzle_number);
									
									for(int i=1;i<=have_ticket_cnt;i++){
										myDSH->setStringForKey(kDSH_Key_ticketUserId_int1, i, "");
										myDSH->setIntegerForKey(kDSH_Key_haveTicketCnt, 0);
										
										ASPopupView* t_popup = ASPopupView::create(-200);
										
										CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
										float screen_scale_x = screen_size.width/screen_size.height/1.5f;
										if(screen_scale_x < 1.f)
											screen_scale_x = 1.f;
										
										float height_value = 320.f;
										if(myDSH->screen_convert_rate < 1.f)
											height_value = 320.f/myDSH->screen_convert_rate;
										
										if(height_value < myDSH->ui_top)
											height_value = myDSH->ui_top;
										
										t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));
										
										CCNode* open_puzzle_container = CCNode::create();
										t_popup->setContainerNode(open_puzzle_container);
										
										CCScale9Sprite* open_puzzle_case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
										open_puzzle_case_back->setPosition(CCPointZero);
										open_puzzle_container->addChild(open_puzzle_case_back);
										
										open_puzzle_case_back->setContentSize(CCSizeMake(230, 250));
										
										CCScale9Sprite* open_puzzle_content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
										open_puzzle_content_back->setPosition(ccp(0,2));
										open_puzzle_container->addChild(open_puzzle_content_back);
										
										open_puzzle_content_back->setContentSize(CCSizeMake(202, 146));
										
										CCLabelTTF* open_puzzle_title_label = CCLabelTTF::create("퍼즐 오픈", mySGD->getFont().c_str(), 20);
										open_puzzle_title_label->setPosition(ccp(0, 102));
										open_puzzle_container->addChild(open_puzzle_title_label);
										
										CCLabelTTF* open_puzzle_content_label = CCLabelTTF::create("새로운 퍼즐이\n오픈 되었습니다.", mySGD->getFont().c_str(), 18);
										open_puzzle_content_label->setPosition(CCPointZero);
										open_puzzle_container->addChild(open_puzzle_content_label);
										
										CCLabelTTF* loading_puzzle_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 12);
										loading_puzzle_label->setPosition(ccp(0,-95));
										open_puzzle_container->addChild(loading_puzzle_label);
										
										CCSprite* n_op_ok = CCSprite::create("popup2_ok.png");
										CCSprite* s_op_ok = CCSprite::create("popup2_ok.png");
										s_op_ok->setColor(ccGRAY);
										
										CCMenuItemSpriteLambda* op_ok_item = CCMenuItemSpriteLambda::create(n_op_ok, s_op_ok, [=](CCObject* sender){
											t_popup->removeFromParent();
										});
										
										CCMenuLambda* op_ok_menu = CCMenuLambda::createWithItem(op_ok_item);
										op_ok_menu->setTouchPriority(t_popup->getTouchPriority()-1);
										op_ok_menu->setVisible(false);
										op_ok_menu->setPosition(ccp(0,-95));
										open_puzzle_container->addChild(op_ok_menu);
									}
									break;
								}
								else {
									// 가지고 있는 티켓
								}
								
							}
							else {
								// 소용없는 티켓
							}
						}
					}
					
				});
			}
	});
	allReceive->setPosition(ccp(240, 32));
	allReceive->setEnabled(false);
	this->addChild(allReceive, 1);
	

	
	
//	CCMenuItemLambda* allReceive = CCMenuItemImageLambda::create("postbox_allok.png", "postbox_allok.png",
//			[=](CCObject*){
//				if(m_mailFilter == MailFilter::kHeart) {
//					std::vector<int> mailNumbers;
//					for(int i=0; i<m_mailList.size(); i++)
//					{
//						if(m_mailList[i]["type"].asInt() == MessageRecvType::kHeart)
//						{
//							mailNumbers.push_back(m_mailList[i]["no"].asInt());
//						}
//					}
//					this->removeMessageByList(mailNumbers, hspConnector::get()->myKakaoInfo["user_id"].asInt64(), [=](Json::Value){
//						//코인올리기
//						myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+mailNumbers.size());
//						m_heartRefresh();
//					});
//				}
//				else if(m_mailFilter == MailFilter::kTicket){
//					std::vector<int> mailNumbers;
//					KS::KSLog("%", m_mailList);
//					std::vector<int> ticketIndexs;
//					for(int i=0; i<m_mailList.size(); i++)
//					{
//						if(m_mailList[i]["type"].asInt() == MessageRecvType::kTicketResult)
//						{
//							mailNumbers.push_back(m_mailList[i]["no"].asInt());
//							ticketIndexs.push_back(i);
//						}
//					}
//					Json::Value puzzleTicket;
//					for(auto i : ticketIndexs)
//					{
//						Json::Value one;
//						Json::Reader reader;
//						Json::Value contentObj;
//						reader.parse(m_mailList[i]["content"].asString(), contentObj);
//						one["puzzlenumber"] = contentObj["puzzlenumber"].asInt();
//						one["friendID"] = m_mailList[i]["friendID"].asInt64();
//						puzzleTicket.append(one);
//					}
//					this->removeMessageByList(mailNumbers, hspConnector::get()->myKakaoInfo["user_id"].asInt64(), [=](Json::Value r){
//						// to do
//						if(r["result"]["code"].asInt() != GDSUCCESS){
//							return;
//						}
//						KS::KSLog("%", puzzleTicket);
//						// 영호 puzzleTicket 쓰면 됨. 이런 구조임.
//						/*
//						 [
//						 
//						 {
//						 "friendID" : 90280374354071376,
//						 "puzzlenumber" : 4
//						 },
//						 
//						 {
//						 "friendID" : 90280374354071376,
//						 "puzzlenumber" : 4
//						 },
//						 
//						 {
//						 "friendID" : 90280374354071376,
//						 "puzzlenumber" : 4
//						 }
//						 ]
//						 */
//						
//						for(int t_i = 0;t_i<puzzleTicket.size();t_i++)
//						{
//							string t_friend_id = puzzleTicket[t_i]["friendID"].asString();
//							int t_puzzle_number = puzzleTicket[t_i]["puzzlenumber"].asInt();
//							if(myDSH->getBoolForKey(kDSH_Key_isClearedPuzzle_int1, t_puzzle_number-1) && myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+2 == t_puzzle_number) {
//								bool good_ticket = true;
//								int have_ticket_cnt = myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt);
//								for(int i=1;i<=have_ticket_cnt && good_ticket;i++) {
//									string ticket_user_id = myDSH->getStringForKey(kDSH_Key_ticketUserId_int1, i);
//									if(ticket_user_id == t_friend_id){
//										good_ticket = false;
//									}
//								}
//								
//								if(good_ticket && have_ticket_cnt < NSDS_GI(t_puzzle_number, kSDS_PZ_ticket_i)) {
//									int have_ticket_cnt = myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt) + 1;
//									myDSH->setIntegerForKey(kDSH_Key_haveTicketCnt, have_ticket_cnt);
//									myDSH->setStringForKey(kDSH_Key_ticketUserId_int1, have_ticket_cnt, t_friend_id);
//									
//									int need_ticket_cnt = NSDS_GI(t_puzzle_number, kSDS_PZ_ticket_i);
//									
//									CCLabelTTF* ticket_cnt_label = (CCLabelTTF*)((PuzzleMapScene*)m_target_close)->getChildByTag(kPMS_MT_ticketCnt);
//									if(ticket_cnt_label){
//										ticket_cnt_label->setString(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt),
//																							   NSDS_GI(t_puzzle_number, kSDS_PZ_ticket_i))->getCString());
//									}
//									if(need_ticket_cnt <= have_ticket_cnt) {
//										// open 퍼즐
//										myDSH->setIntegerForKey(kDSH_Key_openPuzzleCnt, myDSH->getIntegerForKey(kDSH_Key_openPuzzleCnt)+1);
//										vector<SaveUserData_Key> save_userdata_list;
//										save_userdata_list.push_back(kSaveUserData_Key_openPuzzle);
//										myDSH->saveUserData(save_userdata_list, nullptr);
//										
//										((PuzzleMapScene*)m_target_close)->removeChildByTag(kPMS_MT_buyPuzzle);
//										((PuzzleMapScene*)m_target_close)->removeChildByTag(kPMS_MT_callTicket);
//										((PuzzleMapScene*)m_target_close)->removeChildByTag(kPMS_MT_ticketCnt);
//										((PuzzleMapScene*)m_target_close)->removeChildByTag(kPMS_MT_puzzleOpenTitle);
//										
//										((PuzzleMapScene*)m_target_close)->openPuzzleAction(t_puzzle_number);
//										
//										for(int i=1;i<=have_ticket_cnt;i++){
//											myDSH->setStringForKey(kDSH_Key_ticketUserId_int1, i, "");
//											myDSH->setIntegerForKey(kDSH_Key_haveTicketCnt, 0);
//											
//											ASPopupView* t_popup = ASPopupView::create(-200);
//											
//											CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//											float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//											if(screen_scale_x < 1.f)
//												screen_scale_x = 1.f;
//											
//											t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top/myDSH->screen_convert_rate));
//											
//											CCNode* open_puzzle_container = CCNode::create();
//											t_popup->setContainerNode(open_puzzle_container);
//											
//											CCScale9Sprite* open_puzzle_case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
//											open_puzzle_case_back->setPosition(CCPointZero);
//											open_puzzle_container->addChild(open_puzzle_case_back);
//											
//											open_puzzle_case_back->setContentSize(CCSizeMake(230, 250));
//											
//											CCScale9Sprite* open_puzzle_content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//											open_puzzle_content_back->setPosition(ccp(0,2));
//											open_puzzle_container->addChild(open_puzzle_content_back);
//											
//											open_puzzle_content_back->setContentSize(CCSizeMake(202, 146));
//											
//											CCLabelTTF* open_puzzle_title_label = CCLabelTTF::create("퍼즐 오픈", mySGD->getFont().c_str(), 20);
//											open_puzzle_title_label->setPosition(ccp(0, 102));
//											open_puzzle_container->addChild(open_puzzle_title_label);
//											
//											CCLabelTTF* open_puzzle_content_label = CCLabelTTF::create("새로운 퍼즐이\n오픈 되었습니다.", mySGD->getFont().c_str(), 18);
//											open_puzzle_content_label->setPosition(CCPointZero);
//											open_puzzle_container->addChild(open_puzzle_content_label);
//											
//											CCLabelTTF* loading_puzzle_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 12);
//											loading_puzzle_label->setPosition(ccp(0,-95));
//											open_puzzle_container->addChild(loading_puzzle_label);
//											
//											CCSprite* n_op_ok = CCSprite::create("popup2_ok.png");
//											CCSprite* s_op_ok = CCSprite::create("popup2_ok.png");
//											s_op_ok->setColor(ccGRAY);
//											
//											CCMenuItemSpriteLambda* op_ok_item = CCMenuItemSpriteLambda::create(n_op_ok, s_op_ok, [=](CCObject* sender){
//												t_popup->removeFromParent();
//											});
//											
//											CCMenuLambda* op_ok_menu = CCMenuLambda::createWithItem(op_ok_item);
//											op_ok_menu->setTouchPriority(t_popup->getTouchPriority()-1);
//											op_ok_menu->setVisible(false);
//											op_ok_menu->setPosition(ccp(0,-95));
//											open_puzzle_container->addChild(op_ok_menu);
//										}
//										break;
//									}
//									else {
//										// 가지고 있는 티켓
//									}
//									
//								}
//								else {
//									// 소용없는 티켓
//								}
//							}
//						}
//						
//					});
//				}
//			}
//			);
//	allReceive->setPosition(ccp(375, 25));
//	allReceive->setColor(ccc3(100, 100, 100));
//	allReceive->setEnabled(false);
//	_menu->addChild(allReceive, kMP_Z_close);
	
	CommonButton* giftFilter = CommonButton::create("티켓함", 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* helpFilter = CommonButton::create("도움함", 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* ticketFilter = CommonButton::create("티켓함", 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* challengeFilter = CommonButton::create("도전함", 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* coinFilter = CommonButton::create("하트함", 12, CCSizeMake(65,38), CommonButtonGray, -200);
	CommonButton* totalFilter = CommonButton::create("전체보기", 12, CCSizeMake(65,38), CommonButtonGray, -200);
	
	
	
	loadMail();
	
	auto allInvisible = [=]()
	{
		giftFilter->setEnabled(true);
		helpFilter->setEnabled(true);
		ticketFilter->setEnabled(true);
		challengeFilter->setEnabled(true);
		coinFilter->setEnabled(true);
		totalFilter->setEnabled(true);
//		totalFilterOn->setVisible(false);
//		coinFilterOn->setVisible(false);
//		challengeFilterOn->setVisible(false);
//		ticketFilterOn->setVisible(false);
//		helpFilterOn->setVisible(false);
//		giftFilterOn->setVisible(false);
	};
	//allInvisible();
	//totalFilterOn->setVisible(true);
	
	
//	CCMenuItemLambda* totalFilter = CCMenuItemImageLambda::create("postbox_all_off.png", "postbox_all_off.png",
//			[=](CCObject*)
//			{
//				m_mailFilter = MailFilter::kTotal;
//				filterWithMailFilter();
//				this->mailTableView->reloadData();
//				allInvisible();
//				totalFilterOn->setVisible(true);
//				allReceive->setColor(ccc3(100, 100, 100));
//			});
//	totalFilter->setPosition(ccp(53, 258));
//	_menu->addChild(totalFilter, kMP_Z_close);
	
	totalFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	totalFilter->setTitleColor(ccc3(200, 200, 200));
	totalFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	totalFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = MailFilter::kTotal;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		totalFilter->setEnabled(false);
		allReceive->setEnabled(false);
	});
	totalFilter->setPosition(ccp(53, 255));
	this->addChild(totalFilter, 1);
	
	
	
	
	
//	CCMenuItemLambda* coinFilter = CCMenuItemImageLambda::create("postbox_coin_off.png", "postbox_coin_off.png",
//			[=](CCObject*)
//			{
//				m_mailFilter = MailFilter::kHeart;
//				filterWithMailFilter();
//				this->mailTableView->reloadData();
//				allInvisible();
//				coinFilterOn->setVisible(true);
//				allReceive->setColor(ccc3(255, 255, 255));
//				allReceive->setEnabled(true);
//			});
//	coinFilter->setPosition(ccp(118, 258));
//	_menu->addChild(coinFilter, kMP_Z_close);

	coinFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	coinFilter->setTitleColor(ccc3(200, 200, 200));
	coinFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	coinFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = MailFilter::kHeart;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		coinFilter->setEnabled(false);
		allReceive->setEnabled(true);

	});
	coinFilter->setPosition(ccp(118, 255));
	this->addChild(coinFilter, 1);
	
	
	
	
	
//	CCMenuItemLambda* challengeFilter = CCMenuItemImageLambda::create("postbox_challenge_off.png", "postbox_challenge_off.png",
//			[=](CCObject*)
//			{
//				m_mailFilter = MailFilter::kChallenge;	
//				filterWithMailFilter();
//				this->mailTableView->reloadData();
//				allInvisible();
//				challengeFilterOn->setVisible(true);
//				allReceive->setColor(ccc3(100, 100, 100));
//				allReceive->setEnabled(false);
//			});
//	challengeFilter->setPosition(ccp(182, 258));
//	_menu->addChild(challengeFilter, kMP_Z_close);

	challengeFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	challengeFilter->setTitleColor(ccc3(200, 200, 200));
	challengeFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	challengeFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = MailFilter::kChallenge;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		challengeFilter->setEnabled(false);
		allReceive->setEnabled(false);

	});
	challengeFilter->setPosition(ccp(182, 255));
	this->addChild(challengeFilter, 1);
	
	
	
//	CCMenuItemLambda* ticketFilter = CCMenuItemImageLambda::create("postbox_ticket_off.png", "postbox_ticket_off.png",
//			[=](CCObject*)
//			{
//
//				m_mailFilter = MailFilter::kTicket;	
//				filterWithMailFilter();
//				this->mailTableView->reloadData();
//				allInvisible();
//				ticketFilterOn->setVisible(true);
//				allReceive->setColor(ccc3(255, 255, 255));
//				allReceive->setEnabled(true);
//			});
//	ticketFilter->setPosition(ccp(247, 258));
//	_menu->addChild(ticketFilter, kMP_Z_close);
	ticketFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	ticketFilter->setTitleColor(ccc3(200, 200, 200));
	ticketFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	ticketFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = MailFilter::kTicket;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		ticketFilter->setEnabled(false);
		allReceive->setEnabled(true);

	});
	ticketFilter->setPosition(ccp(247, 255));
	this->addChild(ticketFilter, 1);
	
	
	
	
//	CCMenuItemLambda* helpFilter = CCMenuItemImageLambda::create("postbox_help_off.png", "postbox_help_off.png",
//			[=](CCObject*)
//			{
//
//				m_mailFilter = MailFilter::kHelp;	
//				filterWithMailFilter();
//				this->mailTableView->reloadData();
//				allInvisible();
//				helpFilterOn->setVisible(true);
//				allReceive->setColor(ccc3(100, 100, 100));
//				allReceive->setEnabled(false);
//			});
//	helpFilter->setPosition(ccp(312, 258));
//	_menu->addChild(helpFilter, kMP_Z_close);
	helpFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	helpFilter->setTitleColor(ccc3(200, 200, 200));
	helpFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	helpFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = MailFilter::kHelp;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		helpFilter->setEnabled(false);
	});
	helpFilter->setPosition(ccp(312, 255));
	this->addChild(helpFilter, 1);
	
	
	
	
//	CCMenuItemLambda* giftFilter = CCMenuItemImageLambda::create("postbox_gift_off.png", "postbox_gift_off.png",
//			[=](CCObject*)
//			{
//				m_mailFilter = MailFilter::kNews;	
//				filterWithMailFilter();
//				this->mailTableView->reloadData();
//				allInvisible();
//				giftFilterOn->setVisible(true);
//				allReceive->setColor(ccc3(100, 100, 100));
//				allReceive->setEnabled(false);
//			});
//	giftFilter->setPosition(ccp(377, 258));	
//	_menu->addChild(giftFilter, kMP_Z_close);

	
	giftFilter->setBackgroundTypeForDisabled(CommonButtonYellow);
	giftFilter->setTitleColor(ccc3(200, 200, 200));
	giftFilter->setTitleColorForDisable(ccc3(20, 0, 0));
	giftFilter->setFunction([=](CCObject*){
		if(!this->isLoaded)return;
		m_mailFilter = MailFilter::kHelp;
		filterWithMailFilter();
		this->mailTableView->reloadData();
		allInvisible();
		giftFilter->setEnabled(false);
	});
	giftFilter->setPosition(ccp(377, 255));
	this->addChild(giftFilter, 1);
	
	
//	CCMenuItemLambda* closeBtn = CCMenuItemImageLambda::create(
//			"cardchange_cancel.png", "cardchange_cancel.png",
//			[=](CCObject*){
//				(target_close->*delegate_close)();
//				removeFromParent();
//			});
//
//	closeBtn->setPosition(ccp(451, 257));

	CommonButton* closeBtn = CommonButton::createCloseButton(-200);
	closeBtn->setFunction([=](CCObject*){
		hspConnector::get()->removeTarget(this);
		this->hidePopup();
	});
	closeBtn->setPosition(ccp(450, 255));
	this->addChild(closeBtn);
	
	
	allInvisible();
	totalFilter->setEnabled(false);
	
	//_menu->addChild(closeBtn, kMP_Z_close);
	
	
}
void MailPopup::loadMail ()
{
	Json::Value p;
	p["memberID"]=hspConnector::get()->getSocialID();
	p["type"]=0; // 모든 타입의 메시지를 받겠다는 뜻.
	p["limitDay"] = mySGD->getMsgRemoveDay();
	// 0 이 아니면 해당하는 타입의 메시지가 들어옴.
	
	hspConnector::get()->command("getmessagelist",p,this,[this](Json::Value r)
															 {
																 GraphDogLib::JsonToLog("getmessagelist", r);
																 if(r["result"]["code"].asInt() != GDSUCCESS)
																	 return;
																 this->drawMail(r);
															 });
}
void MailPopup::drawMail (Json::Value obj)
{
	m_mailList = obj["list"];
//	auto app_friends = fInfo["app_friends_info"];
	std::map<std::string, FriendData> userIdKeyValue;
	// m_mailList 와 app_friends 를 합쳐야됨.
	//
	
	
	for(auto it : KnownFriends::getInstance()->getFriends())
	{
		userIdKeyValue[it.userId] = it;
	}
	
	for(int i=0; i<m_mailList.size(); i++)
	{
		std::string user_id = m_mailList[i]["friendID"].asString();
		m_mailList[i]["nickname"] = userIdKeyValue[user_id].nick;
		m_mailList[i]["profile_image_url"] = userIdKeyValue[user_id].profileUrl;
	}
	
	filterWithMailFilter();
	//테이블 뷰 생성 시작 /////////////////////////////////////////////////////////////////////////////////////////
	
	//320x320 테이블 뷰 생성
	mailTableView = CCTableView::create(this, CCSizeMake(422.5f, 174.f));
		
	CCScale9Sprite* bar = CCScale9Sprite::create("postbox_bar.png");
	m_scrollBar = ScrollBar::createScrollbar(mailTableView, -2 - 10, NULL, bar);
	m_scrollBar->setDynamicScrollSize(false);
	m_scrollBar->setVisible(false);	
	mailTableView->setAnchorPoint(CCPointZero);
	
	//kCCScrollViewDirectionVertical : 세로 스크롤, kCCScrollViewDirectionHorizontal : 가로 스크롤
	mailTableView->setDirection(kCCScrollViewDirectionVertical);
	
	//추가시 정렬 기준 설정 kCCTableViewFillTopDown : 아래부분으로 추가됨, kCCTableViewFillBottomUp : 위에서 부터 추가됨.
	mailTableView->setVerticalFillOrder(kCCTableViewFillTopDown);
	
	//기준점 0,0
	// 좌표 수동으로 잡느라 이리 됨
	mailTableView->setPosition(ccp(37, 56));
	
	//데이터를 가져오고나 터치 이벤트를 반환해줄 대리자를 이 클래스로 설정.
	mailTableView->setDelegate(this);
	this->addChild(mailTableView, kMP_Z_mailTable);
	mailTableView->setTouchPriority(-200);

	isLoaded=true;
	
	//테이블 뷰 생성 끝/////////////////////////////////////////////////////////////////////////////////////////
	
	// 필터한 json 을 생성함. m_mailFilter 에 따라 필터함.
	// atIndex 에서 그릴 때, m_filteredMailList 에 따라 돌아가야 될 듯.
	//
	//if(m_mailFilter == MailFilter::kTotal)
	//{
	//}
	//else 
		//if(m_mailFilter == MailFilter::kTotal)
}
//void MailPopup::closePopup (CCControlButton * obj, CCControlEvent event)
//{
//	this->runAction(CCSequence::create(CCMoveBy::create(0.5, CCPoint(0,-400)),CCCallFunc::create(this, callfunc_selector(MailPopup::finishedClose)),NULL));
//}
void MailPopup::removeMail (CCObject * _obj)
{
	
	
}
CCTableViewCell * MailPopup::tableCellAtIndex (CCTableView * table, unsigned int idx)
{
	CCTableViewCell* realCell = table->dequeueCell();
	
	realCell = new CCTableViewCell();
	realCell->init();
	realCell->autorelease();
	auto createCCNodeFromIdx = [=](int idx)->CCNode*
	{

		CCLabelTTF* title;
		CCMenuItemLambda* sendBtn;
		CCLabelTTF* score;
		Json::Reader reader;
		Json::Value contentObj;

		const Json::Value& mail = m_filteredMailList[idx]; //hspConnector::get()->getMailByIndex(idx);
		reader.parse(mail["content"].asString(), contentObj);


		KS::KSLog("%", mail);
		
		KS::KSLog("%", contentObj);
		//	KS::KSLog("%", contentObj["puzzlenumber"].asInt());

		CCNode* cell = CCNode::create();

		std::string cellBackFile = "ui_common_cell.png";


		CCSprite* bg = CCSprite::create(cellBackFile.c_str());
		bg->setPosition(CCPointZero);
		bg->setAnchorPoint(CCPointZero);
		cell->addChild(bg,0);

		CCSprite* profileImg = GDWebSprite::create((mail)["profile_image_url"].asString(), "ending_noimg.png");
		profileImg->setAnchorPoint(ccp(0.5, 0.5));
		profileImg->setTag(kMP_MT_profileImg);
		profileImg->setPosition(ccp(20, 20));
		profileImg->setScale(30.f / profileImg->getContentSize().width);
		cell->addChild(profileImg, kMP_Z_profileImg);


		CCMenuLambda* _menu = CCMenuLambda::create();
		_menu->setPosition(ccp(0, 0));
		_menu->setTouchPriority(-200);
		_menu->setTag(kMP_MT_send);
		cell->addChild(_menu, kMP_MT_getheart);



		title = CCLabelTTF::create((contentObj["nick"].asString() + "님의").c_str(), mySGD->getFont().c_str(),12);
		title->setPosition(ccp(38,20));
		title->setColor(ccc3(20, 0, 0));
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
//						 CCMenuItemLambda* obj = dynamic_cast<CCMenuItemLambda*>(sender);
//						 int idx = (int)obj->getUserData();

						 Json::Value p;
						 int mailNo = mail["no"].asInt();

						 p["no"] = mailNo;
						 p["memberID"] = mail["memberID"].asInt64();


						 //삭제요청
						 this->removeMessage (mailNo, mail["memberID"].asInt64(),
						 [=](Json::Value r)
						 {
							 if(r["error"]["code"].asInt() != GDSUCCESS) {
								 return;
							 }
							 //코인올리기

							 myDSH->setIntegerForKey(kDSH_Key_heartCnt, myDSH->getIntegerForKey(kDSH_Key_heartCnt)+1);
							 m_heartRefresh();

						 });
					 }
				);
				sendBtn->setPosition(ccp(155, 22));

				_menu->addChild(sendBtn,2);

				break;
			case kChallengeRequest:
				comment = "도전장이 도착했어요.";
				sendBtn = CCMenuItemImageLambda::create
					("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject* sender) {
						 KHAlertView* av = KHAlertView::create(); 
						 av->setCloseOnPress(false);
						 // av->setTitleFileName("msg_challenge.png");
						 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
						 av->setWidth(240);
						 av->setHeight(240);
						 av->setTitleHeight(10);
						 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
						 av->setCenterY(150);

						 CCNode* emptyNode = CCNode::create();
						 auto ttf = CCLabelTTF::create("도전을 받아들이시겠습니까?", mySGD->getFont().c_str(), 14.f); 
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
						 av->addButton(CommonButton::create("도망가기", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
													 [=](CCObject* e) {
														 CCLOG("ok!!");
//														 CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
														 // 도망에 대한 처리
														 Json::Value p;
														 int mailNo = mail["no"].asInt();;
														 p["no"] = mailNo;
														 p["memberID"] = mail["memberID"].asInt64();
														 // 도전장 삭제요청
														 this->removeMessage (mailNo, mail["memberID"].asInt64(),
																									[=](Json::Value r) {
																										av->removeFromParent();
																										Json::Value p;
																										Json::Value contentJson;
																										//		contentJson["msg"] = (nickname + "님에게 도전!");
																										contentJson["challengestage"] = contentObj["challengestage"].asInt();
																										contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
																										p["receiverMemberID"] = mail["friendID"].asString();
																										p["senderMemberID"] = hspConnector::get()->getSocialID();
																										p["type"] = kChallengeResult;
																										contentJson["result"] = "win"; // 상대방을 win 으로 세링~
																										p["content"] = GraphDogLib::JsonObjectToString(contentJson);
																										hspConnector::get()->command("sendMessage", p,this, [=](Json::Value r) {
																											//		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																											//		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																											//		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];

																											//																		setHelpSendTime(recvId);
																											GraphDogLib::JsonToLog("sendMessage", r);
																											if(r["result"]["code"].asInt() != GDSUCCESS){
																												return;
																											}
																											//												 						obj->removeFromParent();
																											///////////////////////////////////////////
																											KHAlertView* av = KHAlertView::create(); 
																											av->setTitleFileName("msg_challenge_result.png");
																											av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
																											av->setWidth(240);
																											av->setHeight(240);
																											av->setTitleHeight(10);
																											av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
																											av->setCenterY(150);

																											CCNode* emptyNode = CCNode::create();
																											auto ttf = CCLabelTTF::create("도전에 임하는 자세를 가집시다!!\n 도망으로간주, 패하셨습니다.", mySGD->getFont().c_str(), 14.f); 
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
																											addChild(av, kMP_Z_helpAccept);
																											av->show();

																											
																											// 카톡 메세지 전송																					
																											// Json::Value p2;
																											//p2["receiver_id"] = mail["friendID"].asString();
																											//// 여기서 당신은 지금 배틀한 상대방을 지칭
																											//p2["message"] = "당신이 승리하였습니다. 보상을 받으세요 ^_^";
																											//hspConnector::get()->kSendMessage
																												//(p2, [=](Json::Value r) {
																													//GraphDogLib::JsonToLog("kSendMessage", r);
																												//});
																										});
																									});
													 });
						 av->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
													 [=](CCObject* e) {
//														 CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
														 int mailNo = mail["no"].asInt();
														 
														 mySGD->setRemoveMessageMailNo(mailNo);
														 mySGD->setRemoveMessageMemberId(mail["memberID"].asInt64());
//														 mySGD->setAcceptChallengeTarget(mail["friendID"].asString(), mail["nickname"].asString(),
//																														 contentObj["score"].asFloat(), contentObj["replaydata"], contentObj["profile"].asString());
														 mySD->setSilType(contentObj["challengestage"].asInt());
//														 mySGD->setIsAcceptChallenge(true);
														 // ST 받고 성공시 창 띄움.. & sender->removeFromParent();
														 addChild(StageInfoDown::create
																			(this,
																			 callfunc_selector(ThisClassType::onReceiveStageSuccess),
																			 this, callfunc_selector(ThisClassType::onReceiveStageFail)), kMP_Z_popup);
														 CCLOG("ok!!");
													 });

						 addChild(av, kMP_Z_helpAccept);
						 av->show();
						 ///////////////////////////////////////
					 }
				);
				sendBtn->setPosition(ccp(180, 22));

				_menu->addChild(sendBtn,2);
				break;
			case kChallengeResult:
				comment = "도전결과!!";
				sendBtn = CCMenuItemImageLambda::create
					("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject*) {
						 if(contentObj["result"].asString() == "win") {
							 // 메세지 삭제후 모자가차.
							 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
														 [=](Json::Value r) {
//															 mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPFinishedChallenge());
//															 myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v) {
//
//															 });

															 addChild(GachaPurchase::create(kGachaPurchaseStartMode_reward,
																															[=](){
																																CCLOG("hat close");
																															}
																														 ), kMP_Z_helpAccept);
														 });

						 }
						 else if(contentObj["result"].asString() == "lose") {
							 // 메세지 삭제후 졌다는거 띄움.
							 Json::Value p;
							 int mailNo = mail["no"].asInt();
							 p["no"] = mailNo;
							 p["memberID"] = mail["memberID"].asInt64();
							 // 도전장 삭제요청
							 hspConnector::get()->command ( "removemessage",p, [=](Json::Value r) {
								 if(r["result"]["code"].asInt() != GDSUCCESS){
									 return;
								 }
//								 mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPFinishedChallenge());
//								 myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v) {
//
//								 });
								 KHAlertView* av = KHAlertView::create();
								 KS::KSLog("%", contentObj);
								 av->setTitleFileName("msg_challenge_result.png");
								 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
								 av->setWidth(240);
								 av->setHeight(240);
								 av->setTitleHeight(10);
								 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
								 av->setCenterY(150);

								 CCNode* emptyNode = CCNode::create();
								 auto ttf = CCLabelTTF::create("졌습니다. 브레이크 포인트 필요함. 위에 contentObj", mySGD->getFont().c_str(), 14.f); 
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
																 CCLOG("ok");
															 });

								 addChild(av, kMP_Z_helpAccept);
								 av->show();
								 ////////////////////////////////////////

							 }
							 );
						 }
					 }
				);
				sendBtn->setPosition(ccp(180, 22));

				_menu->addChild(sendBtn,2);
				break;

			case kHelpRequest:
				comment = "도움요청이 도착했어요.";

				sendBtn = CCMenuItemImageLambda::create
					("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject*) {
						 ////////////////////////////////////////////
						  KHAlertView* av = KHAlertView::create(); 
							av->setCloseOnPress(false);
						 av->setTitleFileName("msg_help_request.png");
						 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
						 av->setWidth(240);
						 av->setHeight(240);
						 av->setTitleHeight(10);
						 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
						 av->setCenterY(150);

						 CCNode* emptyNode = CCNode::create();
						 auto ttf = CCLabelTTF::create("상대방을 도와줍니다.", mySGD->getFont().c_str(), 14.f); 
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
						 av->addButton(CommonButton::create("거절", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
																			 [=](CCObject* e) {
																				 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
																											 [=](Json::Value r)
																											 {
																												 av->removeFromParent();
																											 });
																				 CCLOG("ok!!");
																			 });
						av->addButton(CommonButton::create("수락", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
																															 [=](CCObject* e) {
//																																 CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
																																 int mailNo = mail["no"].asInt();

																																 mySGD->setRemoveMessageMailNo(mailNo);
																																 mySGD->setRemoveMessageMemberId(mail["memberID"].asInt64());
//																																 mySGD->setAcceptHelpTarget(mail["friendID"].asString(), mail["nickname"].asString());
																																 mySD->setSilType(contentObj["helpstage"].asInt());
//																																 mySGD->setIsAcceptHelp(true);
																																 // ST 받고 성공시 창 띄움.. & sender->removeFromParent();
																																 addChild(StageInfoDown::create
																																					(this,
																																					 callfunc_selector(ThisClassType::onReceiveStageSuccess),
																																					 this, callfunc_selector(ThisClassType::onReceiveStageFail)), kMP_Z_popup);
																																 //																									 Json::Value p;
																																 //																									 int mailNo = mail["no"].asInt();
																																 //																									 p["no"] = mailNo;
																																 //																									 p["memberID"] = mail["memberID"].asInt64();
																																 //
																																 //																									 iHelpYou(contentObj.get("helpstage", 0).asInt(),
																																 //																														mail["friendID"].asInt64(), mail["nickname"].asString(),
																																 //																														p);
																																 CCLOG("ok!!");
																															 });

						 addChild(av, kMP_Z_helpAccept);
						 av->show();
/////////////////////////////////////////
					 });
				sendBtn->setPosition(ccp(180, 22));

				_menu->addChild(sendBtn,2);
				break;
			case kHelpResult:
				comment = "상대방의 도움이 왔어요!";
				sendBtn = CCMenuItemImageLambda::create
					("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject*) {
						 Json::Value p;
						 int mailNo = mail["no"].asInt();

						 p["no"] = mailNo;
						 p["memberID"] = mail["memberID"].asInt64();
						 //삭제요청
						 removeMessage (mailNo, mail["memberID"].asInt64(),
														[=](Json::Value r) {
//															mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPFinishedChallenge());
//															myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v) {
//
//															});
															////////////////////////////////////////////////////

															KHAlertView* av = KHAlertView::create();
															av->setTitleFileName("msg_help_result.png");
															auto retStr = NSDS_GS(kSDS_CI_int1_imgInfo_s, contentObj["cardnumber"].asInt());
															// 카드 정보 없음
															if(retStr == "") {
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
																hspConnector::get()->command("getcardlist", param, this,json_selector(this, MailPopup::resultLoadedCardInfo));
																av->setContentNode(card_img);
															}
															// 카드 정보 있음 
															else {
																if(mySGD->isHasGottenCards(contentObj["cardnumber"].asInt()) == 0) {
																	mySGD->addHasGottenCardNumber(contentObj["cardnumber"].asInt());
																}

																av->setContentNode(addCardImg(contentObj["cardnumber"].asInt(), -1, "-1"));
																//							av->addChild();
															}

															// av->setTitleFileName("msg_challenge.png");
															av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
															av->setWidth(240);
															av->setHeight(240);
															av->setTitleHeight(10);
															av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
															av->setCenterY(150);

															CCNode* emptyNode = CCNode::create();
															auto ttf = CCLabelTTF::create("도움을 받았습니다!!", mySGD->getFont().c_str(), 14.f); 
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

															addChild(av, kMP_Z_helpAccept);
															av->show();
														});
					 }
				);
				sendBtn->setPosition(ccp(180, 22));

				_menu->addChild(sendBtn,2);
				break;
			case kTicketRequest:
				comment = "티켓요청이 도착했어요.";
				sendBtn = CCMenuItemImageLambda::create
					("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject*) {
						 KHAlertView* av = KHAlertView::create(); 
						 av->setCloseOnPress(false);
						 av->setTitleFileName("msg_ticket_req.png");
						 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
						 av->setWidth(240);
						 av->setHeight(240);
						 av->setTitleHeight(10);
						 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
						 av->setCenterY(150);

						 CCNode* emptyNode = CCNode::create();
						 auto ttf = CCLabelTTF::create("티켓요청이 도착. 티켓은 퍼즐을 열 때 필요합니다. 친구를 도와주세요!!", mySGD->getFont().c_str(), 14.f); 
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
						 av->addButton(CommonButton::create("거절", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
													 [=](CCObject* e) {
														 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
																					 [=](Json::Value r) {
																						 av->removeFromParent();
																					 });
														 CCLOG("ok!!");
													 });
						 av->addButton(CommonButton::create("보내기", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
													 [=](CCObject* e) {
														 CCLOG("ok!!");
//														 CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
														 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
																					 [=](Json::Value r) {
																						 Json::Value p;
																						 Json::Value contentJson;
																						 //		contentJson["msg"] = (nickname + "님에게 도전!");
																						 contentJson["puzzlenumber"] = contentObj["puzzlenumber"].asInt(); // 받은거 그대로 넣음. puzzlenumber 들어감.
																						 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
																						 p["receiverMemberID"] = mail["friendID"].asString();
																						 p["senderMemberID"] = hspConnector::get()->getSocialID();
																						 p["type"] = kTicketResult;
																						 p["content"] = GraphDogLib::JsonObjectToString(contentJson);
																						 hspConnector::get()->command
															 ("sendMessage", p, [=](Json::Value r) {
																 if(r["result"]["code"].asInt() != GDSUCCESS)
															 {
																 av->removeFromParent();
																 return;
															 }
//															 mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPSendTicket());
//															 myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v) {
//
//															 });
															 av->removeFromParent();
															 });
																					 });
													 });

						 addChild(av, kMP_Z_helpAccept);
						 av->show();
						 av->getContainerScrollView()->setTouchEnabled(false);
						 /////////////////////////////////////

					 }
				);
				sendBtn->setPosition(ccp(180, 22));
				_menu->addChild(sendBtn,2);
				break;
			case kTicketResult:
				comment = "티켓이 왔네요 어서 받으세요.";
				sendBtn = CCMenuItemImageLambda::create
					("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject*) {
						 KHAlertView* av = KHAlertView::create(); 
						 av->setCloseOnPress(false);
						 av->setTitleFileName("msg_ticket_req.png");
						 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
						 av->setWidth(240);
						 av->setHeight(240);
						 av->setTitleHeight(10);
						 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
						 av->setCenterY(150);

						 CCNode* emptyNode = CCNode::create();
						 auto ttf = CCLabelTTF::create("티켓이 도착했습니다. 티켓을 받으세요.", mySGD->getFont().c_str(), 12.f); 
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
						 av->addButton(CommonButton::create("티켓 받기", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
													 [=](CCObject* e) {
														 CCLOG("ok!!");
														 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
																					 [=](Json::Value r) {
																						 av->removeFromParent();
																						 if(mySGD->getPuzzleHistory(contentObj["puzzlenumber"].asInt()-1).is_clear.getV() && !mySGD->getPuzzleHistory(contentObj["puzzlenumber"].asInt()).is_open.getV()) {
																							 bool good_ticket = true;
																							 int have_ticket_cnt = myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt);
																							 for(int i=1;i<=have_ticket_cnt && good_ticket;i++) {
																								 string ticket_user_id = myDSH->getStringForKey(kDSH_Key_ticketUserId_int1, i);
																								 if(ticket_user_id == mail["friendID"].asString()){
																									 good_ticket = false;
																								 }
																							 }

																							 if(good_ticket) {
																								 int have_ticket_cnt = myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt) + 1;
																								 myDSH->setIntegerForKey(kDSH_Key_haveTicketCnt, have_ticket_cnt);
																								 myDSH->setStringForKey(kDSH_Key_ticketUserId_int1, have_ticket_cnt, mail["friendID"].asString());

																								 int need_ticket_cnt = NSDS_GI(contentObj["puzzlenumber"].asInt(), kSDS_PZ_ticket_i);

																								 CCLabelTTF* ticket_cnt_label = (CCLabelTTF*)((PuzzleMapScene*)getTarget())->getChildByTag(kPMS_MT_ticketCnt);
																								 if(ticket_cnt_label){
																									 ticket_cnt_label->setString(CCString::createWithFormat("%d/%d", myDSH->getIntegerForKey(kDSH_Key_haveTicketCnt),
																																																					NSDS_GI(contentObj["puzzlenumber"].asInt(), kSDS_PZ_ticket_i))->getCString());
																								 }
																								 if(need_ticket_cnt <= have_ticket_cnt) {
																									 
																									 ((PuzzleMapScene*)getTarget())->removeChildByTag(kPMS_MT_buyPuzzle);
																									 ((PuzzleMapScene*)getTarget())->removeChildByTag(kPMS_MT_callTicket);
																									 ((PuzzleMapScene*)getTarget())->removeChildByTag(kPMS_MT_ticketCnt);
																									 ((PuzzleMapScene*)getTarget())->removeChildByTag(kPMS_MT_puzzleOpenTitle);

																									 ((PuzzleMapScene*)getTarget())->openPuzzleAction(contentObj["puzzlenumber"].asInt());

																									 for(int i=1;i<=have_ticket_cnt;i++){
																										 myDSH->setStringForKey(kDSH_Key_ticketUserId_int1, i, "");
																										 myDSH->setIntegerForKey(kDSH_Key_haveTicketCnt, 0);

																										 ASPopupView* t_popup = ASPopupView::create(-200);

																										 CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
																										 float screen_scale_x = screen_size.width/screen_size.height/1.5f;
																										 if(screen_scale_x < 1.f)
																											 screen_scale_x = 1.f;
																										 
																										 float height_value = 320.f;
																										 if(myDSH->screen_convert_rate < 1.f)
																											 height_value = 320.f/myDSH->screen_convert_rate;
																										 
																										 if(height_value < myDSH->ui_top)
																											 height_value = myDSH->ui_top;

																										 t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));

																										 CCNode* open_puzzle_container = CCNode::create();
																										 t_popup->setContainerNode(open_puzzle_container);

																										 CCScale9Sprite* open_puzzle_case_back = CCScale9Sprite::create("popup2_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(13, 45, 135-13, 105-13));
																										 open_puzzle_case_back->setPosition(CCPointZero);
																										 open_puzzle_container->addChild(open_puzzle_case_back);

																										 open_puzzle_case_back->setContentSize(CCSizeMake(230, 250));

																										 CCScale9Sprite* open_puzzle_content_back = CCScale9Sprite::create("popup2_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
																										 open_puzzle_content_back->setPosition(ccp(0,2));
																										 open_puzzle_container->addChild(open_puzzle_content_back);

																										 open_puzzle_content_back->setContentSize(CCSizeMake(202, 146));

																										 CCLabelTTF* open_puzzle_title_label = CCLabelTTF::create("퍼즐 오픈", mySGD->getFont().c_str(), 20);
																										 open_puzzle_title_label->setPosition(ccp(0, 102));
																										 open_puzzle_container->addChild(open_puzzle_title_label);

																										 CCLabelTTF* open_puzzle_content_label = CCLabelTTF::create("새로운 퍼즐이\n오픈 되었습니다.", mySGD->getFont().c_str(), 18);
																										 open_puzzle_content_label->setPosition(CCPointZero);
																										 open_puzzle_container->addChild(open_puzzle_content_label);

																										 CCLabelTTF* loading_puzzle_label = CCLabelTTF::create("Loading...", mySGD->getFont().c_str(), 12);
																										 loading_puzzle_label->setPosition(ccp(0,-95));
																										 open_puzzle_container->addChild(loading_puzzle_label);

																										 CCSprite* n_op_ok = CCSprite::create("popup2_ok.png");
																										 CCSprite* s_op_ok = CCSprite::create("popup2_ok.png");
																										 s_op_ok->setColor(ccGRAY);

																										 CCMenuItemSpriteLambda* op_ok_item = CCMenuItemSpriteLambda::create(n_op_ok, s_op_ok, [=](CCObject* sender){
																											 t_popup->removeFromParent();
																										 });

																										 CCMenuLambda* op_ok_menu = CCMenuLambda::createWithItem(op_ok_item);
																										 op_ok_menu->setTouchPriority(t_popup->getTouchPriority()-1);
																										 op_ok_menu->setVisible(false);
																										 op_ok_menu->setPosition(ccp(0,-95));
																										 open_puzzle_container->addChild(op_ok_menu);
																									 }
																								 }
																								 else {
																									 // 가지고 있는 티켓
																								 }
																							 }
																							 else {
																								 // 소용없는 티켓
																							 }
																						 }
																					 });
													 });
						 addChild(av, kMP_Z_helpAccept);
						 av->show();
						 /////////////////////////////////
					 }
				);
				sendBtn->setPosition(ccp(180, 22));

				_menu->addChild(sendBtn,2);
				break;
			case kUnknownFriendRequest:
				comment = "친구추가 요청이 왔습니다.";
				sendBtn = CCMenuItemImageLambda::create
					("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject*)
					 {
						 KHAlertView* av = KHAlertView::create(); 
						 av->setCloseOnPress(false);
						 // av->setTitleFileName("msg_challenge.png");
						 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
						 av->setWidth(240);
						 av->setHeight(240);
						 av->setTitleHeight(10);
						 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
						 av->setCenterY(150);

						 CCNode* emptyNode = CCNode::create();
						 auto ttf = CCLabelTTF::create(comment.c_str(), mySGD->getFont().c_str(), 14);
						 KS::KSLog("%", contentObj);
						 
						 ttf->setColor(ccc3(20, 0, 0));
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
						 av->addButton(CommonButton::create("거절", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
													 [=](CCObject* e) {
														 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
																					 [=](Json::Value r)
																					 {
																						 av->removeFromParent();
																					 });
														 CCLOG("ok!!");
													 });
						 av->addButton(CommonButton::create("수락", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
													 [=](CCObject* e) {
														 CCLOG("ok!!");
//														 CCMenuLambda* sender = dynamic_cast<CCMenuLambda*>(e);
														 Json::Value param;
														 //						memberID : string or number, 내카카오아이디
														 //						-> friendID : string or number, 추가할 게임친구 카카오아이디
														 //						-> friendMax :
														 param["memberID"] = hspConnector::get()->getSocialID();
														 param["friendID"] = mail["friendID"].asString();
														 param["friendMax"] = mySGD->getGameFriendMax(); // magic number
														 hspConnector::get()->command ("addfriendeach", param,
																													 [=](Json::Value v) {
																														 KS::KSLog("%", v);
																														 std::string errorMessage;
																														 /*
																																errorCode 필드에 10030 값이 넘어오면 내친구인원이 초과
																																errorCode 필드에 10031값이 넘어오면 상대방 친구인원이 초과
																																*/
																														 if(v["result"]["code"].asInt() != GDSUCCESS){
																															 return;
																														 }

																														 // 편.삭.
																														 removeMessage(mail["no"].asInt(), mail["memberID"].asInt64(),
																																					 [=](Json::Value r) {
																																						 if(r["result"]["code"].asInt() != GDSUCCESS) {
																																							 av->removeFromParent();
																																							 KHAlertView* exceptionPopup = KHAlertView::create(); 
																																							 av->setTitleFileName("msg_error.png");
																																							 exceptionPopup->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
																																							 exceptionPopup->setWidth(240);
																																							 exceptionPopup->setHeight(240);
																																							 exceptionPopup->setTitleHeight(10);
																																							 exceptionPopup->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
																																							 exceptionPopup->setCenterY(150);

																																							 CCNode* emptyNode = CCNode::create();
																																							 auto ttf = CCLabelTTF::create("오류가 발생했습니다.", mySGD->getFont().c_str(), 14.f); 
																																							 ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
																																							 //	con->setAnchorPoint(ccp(0, 0));
																																							 //ttf->setAnchorPoint(ccp(0.5f, 0.5f));
																																							 ttf->setColor(ccc3(255, 255, 255));
																																							 ttf->setPosition(ccp(exceptionPopup->getContentRect().size.width / 2.f, -77));
																																							 emptyNode->addChild(ttf);
																																							 exceptionPopup->setContentNode(
																																									 emptyNode
																																									 );
																																							 exceptionPopup->setContentSize(ttf->getDimensions());
																																							 exceptionPopup->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
																																														 [=](CCObject* e) {
																																															 CCLOG("ok!!");
																																														 });

																																							 addChild(exceptionPopup, kMP_Z_helpAccept);
																																							 exceptionPopup->show();
																																							 return;
																																						 }
																																						 FriendData ufd;
																																						 ufd.userId = v["friendInfo"]["memberID"].asString();
																																						 ufd.joinDate = v["friendInfo"]["joinDate"].asUInt64();
																																						 ufd.lastDate = v["friendInfo"]["lastDate"].asUInt64();
																																						 ufd.lastTime = v["friendInfo"]["lastTime"].asUInt64();
																																						 ufd.nick = v["friendInfo"]["nick"].asString();
																																						 UnknownFriends::getInstance()->add(ufd);
																																						 av->removeFromParent();
																																					 });

																													 });
													 });

						 addChild(av, kMP_Z_helpAccept);
						 av->setContentNode(NULL);
						 av->show();
					 });

				sendBtn->setPosition(ccp(180, 22));

				_menu->addChild(sendBtn,2);
				break;
			default:
				comment = "알 수 없는 요청이 도착했어요.";
				sendBtn = CCMenuItemImageLambda::create
					("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject*)
					 {
					 });

				sendBtn->setPosition(ccp(180, 22));

				_menu->addChild(sendBtn,2);
				break;
				///
				/*
					 comment = "티켓이 왔네요 어서 받으세요.";
					 sendBtn = CCMenuItemImageLambda::create
					 ("postbox_challenge_ok.png", "postbox_challenge_ok.png",
					 [=](CCObject*)
					 {
					 });

					 sendBtn->setPosition(ccp(190, 22));

					 _menu->addChild(sendBtn,2);
					 break;

*/
				///
		}

		score = CCLabelTTF::create(comment.c_str(),mySGD->getFont().c_str(), 10);
		score->setColor(ccc3(20, 0, 0));
		score->setPosition(ccp(38,5));
		score->setAnchorPoint(CCPointZero);
		score->setTag(kMP_MT_score);
		cell->addChild(score,2);

		sendBtn->setUserData((void *)idx);
		sendBtn->setPosition(ccp(157, 22));

		return cell;
	};
	bool odd = false;
	if(numberOfCellsInTableView(mailTableView) - 1 == idx) // last 조건
	{
		if(m_filteredMailList.size() % 2 == 1)
			odd = true;
	}

	if(odd)
	{
		CCNode* cell1 = createCCNodeFromIdx(idx * 2);	
		realCell->addChild(cell1);	
	}
	else
	{
		CCNode* cell1 = createCCNodeFromIdx(idx * 2);	
		realCell->addChild(cell1);	
		CCNode* cell2= createCCNodeFromIdx(idx * 2 + 1);	
		realCell->addChild(cell2);	
		cell2->setPosition(ccp(200, 0));
	}
		
	return realCell;
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
	
	CCLOG("touch!!");
	
}
CCSize MailPopup::cellSizeForTable (CCTableView * table)
{
	return mailCellSize;
}
unsigned int MailPopup::numberOfCellsInTableView (CCTableView * table)
{
	return ceilf(m_filteredMailList.size() / 2.f);
}
void MailPopup::touchFriend (CCObject * a, CCControlEvent b)
{
	
	
}
//CCPoint MailPopup::getContentPosition (int t_tag)
//{
	//CCPoint return_value;
	
	//if(t_tag == kMP_MT_invite)					return_value = ccp(240,43);
	//else if(t_tag == kMP_MT_close)				return_value = ccp(345,283);
	//else if(t_tag == kMP_MT_send)				return_value = ccp(240,230);
	//else if(t_tag == kMP_MT_send_close)			return_value = ccp(193,59);
	//else if(t_tag == kMP_MT_invite_close)		return_value = ccp(345,283);
	//else if(t_tag == kMP_MT_invite_rank)		return_value = ccp(240,43);
	//else if(t_tag == kMP_MT_invite_send)		return_value = ccp(240,230);
	//else if(t_tag == kMP_MT_invite_send_close)	return_value = ccp(193,59);
	
	//return return_value;
//}
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


void MailPopup::onReceiveStageSuccess()
{
	// 성공시 게임창...
	CCLOG("sec");
//	PuzzleMapScene* pms = dynamic_cast<PuzzleMapScene*>(target_close);
//	CCAssert(pms, "!!");
//	(target_close->*delegate_close)();
//	removeFromParent();
//	pms->showAcceptStageSettingPopup();
	
	CCDirector::sharedDirector()->replaceScene(StartSettingScene::scene());
	
//	PuzzleMapScene::showAcceptStageSettingPopup();
	
}
void MailPopup::onReceiveStageFail()
{
	mySGD->setRemoveMessageMemberId(0);
	mySGD->setRemoveMessageMailNo(0);
//	removeFromParent();
	CCLOG("fail");
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

CCNode* MailPopup::addCardImg (int t_card_number, int t_card_level, string t_passive)
{
	CCSprite* card_img = mySIL->getLoadedImg(CCString::createWithFormat("card%d_visible.png", t_card_number)->getCString());
	card_img->setScale(0.34f);
	
	if(NSDS_GB(kSDS_CI_int1_aniInfoIsAni_b, t_card_number))
	{
		CCSize ani_size = CCSizeMake(NSDS_GI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card_number));
		CCSprite* t_ani = mySIL->getLoadedImg(CCString::createWithFormat("card%d_animation.png", t_card_number)->getCString(),
											  CCRectMake(0, 0, ani_size.width, ani_size.height));
		t_ani->setPosition(ccp(NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card_number), NSDS_GI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card_number)));
		card_img->addChild(t_ani);
	}
	
//	if(NSDS_GB(kSDS_CI_int1_haveFaceInfo_b, t_card_number))
//	{
//		CCSprite* ccb_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_CI_int1_faceInfo_s, t_card_number)).first;
//		ccb_img->setPosition(ccp(160,215));
//		card_img->addChild(ccb_img);
//	}
	
	CardCase* t_case = CardCase::create(t_card_number, t_card_level, t_passive);
	t_case->setPosition(CCPointZero);
	card_img->addChild(t_case);
	
	CCLabelTTF* recent_durability_label = t_case->getRecentDurabilityLabel();
//	recent_durability_label->setString(CCString::createWithFormat("%d", NSDS_GI(kSDS_CI_int1_durability_i, t_card_number))->getCString());
//	recent_durability_label->setPosition(ccpAdd(recent_durability_label->getPosition(), ccp(6,-1)));
	recent_durability_label->setFontSize(recent_durability_label->getFontSize()+3);
	
//	t_case->getTotalDurabilityLabel()->removeFromParent();
	
	return card_img;
}

void MailPopup::resultLoadedCardInfo (Json::Value result_data)
{
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		Json::Value cards = result_data["list"];
		for(int i=0;i<cards.size();i++)
		{
			Json::Value t_card = cards[i];
			NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
			NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
			if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
				continue;
			NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//			NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
			
//			NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
			NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
			
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
			if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && t_faceInfo.asString() != " ")
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
		
		if(mySGD->isHasGottenCards(download_card_number) == 0)
		{
			mySGD->addHasGottenCardNumber(download_card_number);
		}
		
		(getTarget()->*callfunc_selector(PuzzleMapScene::resetPuzzle))();
		
		if(df_list.size() > 0) // need download
		{
			startDownloadCardImage();
		}
		else
		{
			CCNode* loading_parent = loading_card_img->getParent();
			CCPoint loading_position = loading_card_img->getPosition();
			
			loading_card_img->removeFromParent();
			
			loading_parent->addChild(addCardImg(download_card_number, -1, "-1"));
		}
	}
	else if(result_data["result"]["code"].asInt() == GDSAMEVERSION)
	{
		CCNode* loading_parent = loading_card_img->getParent();
		CCPoint loading_position = loading_card_img->getPosition();
		
		loading_card_img->removeFromParent();
		
		loading_parent->addChild(addCardImg(download_card_number, -1, "-1"));
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
		
		CCNode* loading_parent = loading_card_img->getParent();
		CCPoint loading_position = loading_card_img->getPosition();
		
		loading_card_img->removeFromParent();
		
		loading_parent->addChild(addCardImg(download_card_number, -1, "-1"));
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
	CCLOG("start download card img");
	ing_download_cnt = 1;
	startDownload();
}
void MailPopup::startDownload ()
{
	CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	StageImgLoader::sharedInstance()->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename,
												  this, callfunc_selector(MailPopup::successAction), this, callfunc_selector(MailPopup::failAction));
}

void MailPopup::removeMessage(int mailNo, long long memberID, std::function<void(Json::Value)> userFunction)
{
	Json::Value p;
	p["no"] = mailNo;
	p["memberID"] = memberID;
	// 도전장 삭제요청
	hspConnector::get()->command
	(
	 "removemessage",p,
	 
	 [=](Json::Value r)
	 {
		 Json::Value newMailList;
		 if(r["result"]["code"].asInt() != GDSUCCESS)
		 {
			 if(userFunction != nullptr)
				 userFunction(r);
			 return;
		 }
		 

		 {
			 //테이블에서 없어진것 없애기
			 for(int i=0;i<m_mailList.size();i++){
				 if(m_mailList[i]["no"].asInt() != mailNo){
					 newMailList.append(m_mailList[i]);
				 }
			 }
			 //테이블 리로드
			 m_mailList=newMailList;
			 this->filterWithMailFilter();
			 this->mailTableView->reloadData();
			 
		 }
		 if(userFunction != nullptr)
			 userFunction(r);
	 }
	 );
}

void MailPopup::removeMessageByList(vector<int> mailNo, long long memberID, std::function<void(Json::Value)> userFunction)
{
	Json::Value p;
	for(auto no : mailNo)
	{
		p["noList"].append(no);
	}
	p["memberID"] = memberID;
	// 도전장 삭제요청
	hspConnector::get()->command
		(
		 "removemessagebylist",p,
		 [=](Json::Value r)
		 {
			 Json::Value newMailList;
			 if(r["result"]["code"].asInt() != GDSUCCESS){
				 if(userFunction != nullptr)
					 userFunction(r);
				 return;
			 }
			 //테이블에서 없어진것 없애기
			 for(int i=0;i<m_mailList.size();i++){
				 int tempNo = m_mailList[i]["no"].asInt();
				 if(std::find(mailNo.begin(), mailNo.end(), tempNo) == mailNo.end()){
					 newMailList.append(m_mailList[i]);
				 }
			 }
			 //테이블 리로드
			 m_mailList = newMailList;
			 this->filterWithMailFilter();
			 this->mailTableView->reloadData();


			 if(userFunction != nullptr){
				 userFunction(r);
			 }
		 }
	);
}
void MailPopup::filterWithMailFilter()
{
	KS::KSLog("%", m_mailList);
	m_filteredMailList.clear();
	if(m_mailFilter == MailFilter::kTotal)
	{
		m_filteredMailList = m_mailList;
	}
	else
	{
		for(int i=0; i<m_mailList.size(); i++)
		{
			int mailType = m_mailList[i]["type"].asInt();
			if(m_mailFilter == MailFilter::kHeart)
			{
				if(mailType == 1)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == MailFilter::kChallenge)
			{
				if(mailType == 2 || mailType == 3)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == MailFilter::kHelp)
			{
				if(mailType == 4 || mailType == 5)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == MailFilter::kTicket)
			{
				if(mailType == 6 || mailType == 7)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == MailFilter::kInvite)
			{
				if(mailType == 8)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == MailFilter::kNews)
			{
				if(mailType == 9)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
			else if(m_mailFilter == MailFilter::kUnknownFriendRequest)
			{
				if(mailType == 10)
				{
					m_filteredMailList.append(m_mailList[i]);
				}	
			}
		}
	}

	if(m_mailFilter == MailFilter::kTotal)
	{
		KS::KSLog("%", m_mailList);
	}
}
#undef LZZ_INLINE
