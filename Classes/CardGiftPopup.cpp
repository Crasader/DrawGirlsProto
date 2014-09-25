//
//  CardGiftPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 7. 7..
//
//

#include "CardGiftPopup.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "TouchSuctionLayer.h"
#include "KSLabelTTF.h"
#include "MyLocalization.h"
#include "CommonAnimation.h"
#include "CommonButton.h"
#include "StyledLabelTTF.h"
#include "KSUtil.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "KsLocal.h"
#include "FormSetter.h"
#include "FlagSelector.h"
#include "SendMessageUtil.h"


CardGiftPopup* CardGiftPopup::create(int t_touch_priority, int t_gift_card, function<void()> t_end_func, function<void()> t_close_func)
{
	CardGiftPopup* t_cgp = new CardGiftPopup();
	t_cgp->myInit(t_touch_priority, t_gift_card, t_end_func, t_close_func);
	t_cgp->autorelease();
	return t_cgp;
}

void CardGiftPopup::myInit(int t_touch_priority, int t_gift_card, function<void()> t_end_func, function<void()> t_close_func)
{
	startFormSetter(this);
	m_touchPriority = t_touch_priority;
	end_func = t_end_func;
	close_func = t_close_func;
	gift_card_number = t_gift_card;
	
	is_menu_enable = false;
	
	TouchSuctionLayer* t_suction = TouchSuctionLayer::create(m_touchPriority+1);
	t_suction->setTouchEnabled(true);
	addChild(t_suction);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	main_case = CCSprite::create("popup_large_back.png");
	main_case->setPosition(ccp(240,160));
	addChild(main_case);
	
	KSLabelTTF* title_label = KSLabelTTF::create(getLocal(LK::kFriendGiftTitle), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(main_case->getContentSize().width/2.f-85,main_case->getContentSize().height-35));
	main_case->addChild(title_label);
	
	main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(250, 161.5f));
	main_inner->setPosition(main_case->getContentSize().width/2.f, 109.5f);
	main_case->addChild(main_inner);
	
	setFormSetter(main_inner);
	CommonButton* close_menu = CommonButton::createCloseButton(m_touchPriority);
	close_menu->setPosition(ccp(main_case->getContentSize().width-25,main_case->getContentSize().height-22));
	close_menu->setFunction([=](CCObject* sender)
							{
								if(!is_menu_enable)
									return;
								
								AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								is_menu_enable = false;
								
								
								CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
								{
									end_func();
									removeFromParent();
								});
							});
	main_case->addChild(close_menu);
	
		
	CommonAnimation::openPopup(this, main_case, gray,
							   [=](){}, [=]()
	{
		is_menu_enable = true;
	});
	KSLabelTTF* ment = KSLabelTTF::create(getLocal(LK::kFriendGiftDesc), mySGD->getFont().c_str(), 13.f);
	main_case->addChild(ment);
	setFormSetter(ment);
	ment->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, 73.0f));
	
	
	Json::Value param;
	param["memberID"] = myHSP->getMemberID();
	CCSprite* loadingCCBI = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loadingCCBI->setPosition(ccpFromSize(main_inner->getContentSize()) / 2.f + ccp(0, 0));
	main_inner->addChild(loadingCCBI, 100);
	myHSP->command("getfriendlist", param, this, [=](Json::Value v){
		loadingCCBI->removeFromParent();
		/*
		 {
		 "result":{
		 "code":1
		 },
		 "list":[
		 {
		 "memberID":88899626759652560,
		 "nick":"니코짱",
		 "lastDate":20140911202136,
		 "flag":"tw",
		 "highPiece":1,
		 "highScore":0
		 }
		 ],
		 "log":[
		 ]
		 }
		 */
		
		KS::KSLog("%", v);
		if(v["result"]["code"] != GDSUCCESS)
		{
			addChild(ASPopupView::getCommonNoti(m_touchPriority, getLocal(LK::kFriendNoti),
																					getLocal(LK::kFriendError), [=]()
																					{
																						
																					}));
			return;
		}
		
		m_friends = v["list"];
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
		m_friendTable = CCTableView::create(this, CCSizeMake(250, 148));
		m_friendTable->setAnchorPoint(CCPointZero);
		m_friendTable->setDirection(kCCScrollViewDirectionVertical);
		m_friendTable->setVerticalFillOrder(kCCTableViewFillTopDown);
		m_friendTable->setPosition(ccp(26.0, 34));

		
		m_friendTable->setDelegate(this);
		main_case->addChild(m_friendTable);
		m_friendTable->setTouchPriority(m_touchPriority);
		
		m_friendTable->reloadData();
		
		if(m_friends.empty())
		{
			KSLabelTTF* nothing = KSLabelTTF::create(getLocal(LK::kFriendNothingExist), mySGD->getFont().c_str(), 13.f);
			main_case->addChild(nothing, 100);
			nothing->setPosition(ccpFromSize(main_case->getContentSize()) / 2.f + ccp(0, -19.5f));
			setFormSetter(nothing);
		}
		setFormSetter(m_friendTable);
		
	});

	
	
}

CCTableViewCell* CardGiftPopup::tableCellAtIndex(CCTableView *table, unsigned int idx)
{

	CCTableViewCell* cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	Json::Value memberInfo = m_friends[idx];
	

	std::string	cell_back_filename = "rank_normal2.png";
	
	CCScale9Sprite* cell_back;
	cell_back = CCScale9Sprite::create(cell_back_filename.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
	cell_back->setContentSize(CCSizeMake(238, 34));
	
	cell_back->setAnchorPoint(CCPointZero);
	cell_back->setPosition(ccp(7.0, 0.0)); 			// dt (4.0, -2.0)
	cell->addChild(cell_back);
	setFormSetter(cell_back);
	
	KSLabelTTF* nick = KSLabelTTF::create(memberInfo["nick"].asString().c_str(), mySGD->getFont().c_str(), 12.5f);
	cell_back->addChild(nick, 1);
	nick->setAnchorPoint(ccp(0.f, 0.5f));
	nick->setPosition(ccp(51.0, 17.f));
	
	nick->enableOuterStroke(ccc3(0, 0, 0), 1.f, 200, true);
	setFormSetter(nick);
	
	
	
	
	CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(memberInfo["flag"].asString()).c_str());
	setFormSetter(selectedFlagSpr);
	cell_back->addChild(selectedFlagSpr, 1);
	selectedFlagSpr->setPosition(ccp(25.5, 17.0)); 			// dt (25.5, 17.0)

	
	CommonButton* cardSendButton = CommonButton::create(CCSprite::create("friend_bt.png"), m_touchPriority);
	cardSendButton->setTitle(getLocal(LK::kFriendGiftSend));
	cardSendButton->setTitleSize(13.f);
	cell_back->addChild(cardSendButton);
	cardSendButton->setPosition(ccp(188.5, 17.0));
	setFormSetter(cardSendButton);
	cardSendButton->setFunction([=](CCObject*){
		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
		ASPopupView* warningPopup = ASPopupView::createDimmed(m_touchPriority - 1);
		
		warningPopup->getDimmedSprite()->setVisible(false);
		addChild(warningPopup);
		
		auto back = CCSprite::create("popup_small_back.png");
		
		CommonAnimation::openPopup(warningPopup, back, warningPopup->getDimmedSprite(), nullptr, nullptr);
		
		auto front = CCScale9Sprite::create("common_grayblue.png",
																				CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		
		//	front->setVisible(false);
		warningPopup->setContainerNode(back);
		//	back->setContentSize(CCSizeMake(550 / 2.f, 506 / 2.f));
		//	back->setContentSize(CCSizeMake(200, 200));
		front->setContentSize(CCSizeMake(251, 68));
		front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0,9));
		
		
		back->addChild(front);
		
		//				back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-12));
		//				content_back->setPosition(ccp(0.0,-12)); 			// dt (0.0,-4.5)
		
		//				front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0, 15));
		setFormSetter(front);
		
		KSLabelTTF* titleLbl = KSLabelTTF::create(getLocal(LK::kFriendNoti), mySGD->getFont().c_str(), 12.f);
		//	titleLbl->setColor(ccc3(255, 170, 20));
		titleLbl->setAnchorPoint(ccp(0.5f,0.5f));
		titleLbl->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(-85, back->getContentSize().height/2.f-35));
		back->addChild(titleLbl);
		
		
		CommonButton* closeButton = CommonButton::createCloseButton(m_touchPriority - 2);
		closeButton->setFunction([=](CCObject*)
														 {
															 AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
															 CommonAnimation::closePopup(this, back, warningPopup->getDimmedSprite(), nullptr,
																													 [=]()
																													 {
																														 warningPopup->removeFromParent();
																													 });
														 });
		back->addChild(closeButton);
		closeButton->setPosition(ccp(back->getContentSize().width-25, back->getContentSize().height-22));
		
		StyledLabelTTF* content_label = StyledLabelTTF::create(ccsf(getLocal(LK::kFriendGiftQ),
																																memberInfo["nick"].asString().c_str()), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
		content_label->setAnchorPoint(ccp(0.5f,0.5f));
		content_label->setPosition(ccpFromSize(back->getContentSize() / 2.f) + ccp(0, 10));
		setFormSetter(content_label);
		back->addChild(content_label);
		
		CommonButton* confirm = CommonButton::create("확인", 12.f, CCSizeMake(101, 44), CommonButtonAchievement, m_touchPriority - 2);
		back->addChild(confirm);
		confirm->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -50));
		setFormSetter(confirm);
		confirm->setFunction([=](CCObject*){
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			CommonAnimation::closePopup(this, back, warningPopup->getDimmedSprite(), nullptr,
																	[=]()
																	{
																		warningPopup->removeFromParent();
																	});
			
			LoadingLayer* ll = LoadingLayer::create(m_touchPriority - 100);
			addChild(ll, 100);
			t_loading = ll;
			Json::Value transaction_param;
			transaction_param["memberID"] = hspConnector::get()->getMemberID();
//			command_list.push_back(CommandParam("starttransaction", transaction_param, nullptr));
			
			Json::Value send_card_param;
			send_card_param["memberID"] = myHSP->getMemberID();
			send_card_param["toMemberID"] = memberInfo["memberID"].asInt64();
			send_card_param["cardNo"] = gift_card_number.getV();
			command_list.clear();
			command_list.push_back(CommandParam("sendcard", send_card_param, [=](Json::Value v){
				KS::KSLog("%", v);
				
				if(v["result"]["code"].asInt() == GDSUCCESS)
				{
					m_failed = false;
				}
				else if(v["result"]["code"].asInt() == GDALREADY)
				{
					m_failed = true;
					int h, m, s;
					::timeSpliter(v["leftTime"].asInt64(), nullptr, nullptr, nullptr, &h, &m, &s);
					
					string msg = ccsf(getLocal(LK::kFriendCardGiftErrorMessage), v["hourLimit"].asInt(), h, m);
					StyledLabelTTF* slt = StyledLabelTTF::create(msg.c_str(), mySGD->getFont().c_str(), 13.f, 0, StyledAlignment::kCenterAlignment);
					slt->setAnchorPoint(ccp(0.5f, 0.5f));
					setFormSetter(slt);
					addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																							slt, [=]()
																							{
																								
																							}));
					
				}
				else
				{
					m_failed = true;
//					string msg = ccsf(getLocal(LK::kFriendCardGiftErrorMessage), v["hourLimit"].asInt(), h, m);
					StyledLabelTTF* slt = StyledLabelTTF::create(getLocal(LK::kFriendError), mySGD->getFont().c_str(), 13.f, 0, StyledAlignment::kCenterAlignment);
					slt->setAnchorPoint(ccp(0.5f, 0.5f));
					setFormSetter(slt);
					addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
																							slt, [=]()
																							{
																								
																							}));
				}
					
			}));
			
			Json::Value card_history_param;
			card_history_param["memberID"] = myHSP->getMemberID();
			command_list.push_back(CommandParam("getCardHistory", card_history_param, json_selector(this, CardGiftPopup::resultSendAction)));
			
			myHSP->command(command_list);
			
			
			//			Json::Value param;
			//			Json::Value param2;
			//			param["memberID"] = myHSP->getMemberID();
			//			param["friendID"] = memberInfo["memberID"].asString();
			//			myHSP->command("removefriend", param, [=](Json::Value v){
			//				CommonAnimation::closePopup(this, back, warningPopup->getDimmedSprite(), nullptr,
			//																		[=]()
			//																		{
			//																			warningPopup->removeFromParent();
			//																		});
			//
			//
			//				if(v["result"]["code"] != GDSUCCESS)
			//				{
			//					addChild(ASPopupView::getCommonNoti(m_touchPriority - 1, getLocal(LK::kFriendNoti),
			//																							getLocal(LK::kFriendError), [=]()
			//																							{
			//
			//																							}));
			//					return;
			//				}
			//
			//				// 성공 적으로 삭제했다.
			//				m_afterActionFunc(0);
			//				//							m_manageButtonCallback(0);
			//			});
		});
		
		
	});
	return cell;
}

void CardGiftPopup::resultSendAction(Json::Value result_data)
{
	KS::KSLog("%", result_data);
	if(t_loading)
	{
		t_loading->removeFromParent();
		t_loading = nullptr; 
	}
	if(m_failed) // 이전에 sendcard 에서 실패했다면 이것도 그냥 무시.
		return;
	
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->total_card_cnt = result_data["lastCardNo"].asInt();
		
		mySGD->network_check_cnt = 0;
		
		vector<int> card_data_load_list;
		card_data_load_list.clear();
		
		mySGD->initTakeCardInfo(result_data["list"], card_data_load_list);
	
		mySGD->resetHasGottenCards();
		
		
		CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
																{
																	end_func();
																	removeFromParent();
																});
		is_menu_enable = true;
	
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(kMyLocalKey_reConnect), myLoc->getLocalForKey(kMyLocalKey_reConnectAlert4),[=](){
				myHSP->command(command_list);
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
															 {
																 myHSP->command(command_list);
															 }));
		}
	}
}
void CardGiftPopup::scrollViewDidScroll(CCScrollView* view)
{

}
void CardGiftPopup::scrollViewDidZoom(CCScrollView* view)
{

}

void CardGiftPopup::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{

}
CCSize CardGiftPopup::cellSizeForTable(CCTableView *table)
{
	return CCSizeMake(238, 34);
}

unsigned int CardGiftPopup::numberOfCellsInTableView(CCTableView *table)
{
	return m_friends.size();
	
}


