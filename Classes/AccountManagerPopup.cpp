#include "AccountManagerPopup.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "ASPopupView.h"
#include "DataStorageHub.h"
#include "FormSetter.h"
#include "KSLabelTTF.h"
#include "StyledLabelTTF.h"
#include "TitleRenewal.h"
#include "LoadingLayer.h"
#include <boost/lexical_cast.hpp>
#include "KsLocal.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#import "HSPCore.h"
#endif
AccountManagerPopup::AccountManagerPopup()
{
	
}
AccountManagerPopup::~AccountManagerPopup()
{
}
//void AccountManagerPopup::registerWithTouchDispatcher()
//{
//CCTouchDispatcher* pDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
//pDispatcher->addTargetedDelegate(this, INT_MIN, true);
//}

//bool AccountManagerPopup::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool AccountManagerPopup::init(int touchP)
{
	CCLayer::init();

	std::map<HSPLoginTypeX, std::string> descMapper = {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		{HSPLoginTypeGOOGLE, "Google ID"},
#else
		{HSPLoginTypeGOOGLE, "GameCenter ID"},
#endif
		{HSPLoginTypeGUEST, "Guest ID"},
		{HSPLoginTypeFACEBOOK, "Facebook ID"},
	};
	m_touchP = touchP;
	startFormSetter(this);
	ASPopupView* managerPopup = ASPopupView::createDimmed(touchP);
	managerPopup->getDimmedSprite()->setVisible(false);
	addChild(managerPopup);
	m_managerPopup = managerPopup;
	
//	auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	auto back = CCSprite::create("popup_large_back.png");

	auto front = CCScale9Sprite::create("common_grayblue.png",
																			CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	CommonAnimation::openPopup(managerPopup, back, managerPopup->getDimmedSprite(), nullptr, nullptr);
//	front->setVisible(false);
	managerPopup->setContainerNode(back);
//	back->setContentSize(CCSizeMake(550 / 2.f, 506 / 2.f));
//	back->setContentSize(CCSizeMake(200, 200));
	front->setContentSize(CCSizeMake(251, 370 / 2.f));
	

	back->addChild(front);
	
	front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0,15 - 26));

	KSLabelTTF* titleLbl = KSLabelTTF::create(getLocal(LK::kAccountConf), mySGD->getFont().c_str(), 12.f);
//	titleLbl->setColor(ccc3(255, 170, 20));
	titleLbl->setAnchorPoint(ccp(0.5f,0.5f));
	titleLbl->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(-85, back->getContentSize().height/2.f-35));
	back->addChild(titleLbl);

	
	CommonButton* closeButton = CommonButton::createCloseButton(touchP - 1);
	closeButton->setFunction([=](CCObject*)
													 {
														 AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
														 CommonAnimation::closePopup(this, back, managerPopup->getDimmedSprite(), nullptr,
																												 [=]()
																												 {
																													 managerPopup->removeFromParent();
																												 });
	});
	back->addChild(closeButton);
	closeButton->setPosition(ccp(back->getContentSize().width-25, back->getContentSize().height-22));


	
	auto connectionBox = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	connectionBox->setContentSize(CCSizeMake(474 / 2.f, 92 / 2.f));
	connectionBox->setPosition(ccp(back->getContentSize().width/2.f, 182.f));

	back->addChild(connectionBox);
//	connectionBox->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 62.5));

	HSPLoginTypeX loginType = (HSPLoginTypeX)myHSP->getLoginType();
	std::string mappedAccountMsg = "";
	if(loginType == HSPLoginTypeGOOGLE) // 구글 로긴 타입임.
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		mappedAccountMsg = getLocal(LK::kLinkingGoogle);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		mappedAccountMsg = getLocal(LK::kLinkingGameCenter);
#endif
	}
	else
	if(loginType == HSPLoginTypeFACEBOOK)
	{
		mappedAccountMsg = getLocal(LK::kLinkingFacebook);
		
	}
	else if(loginType == HSPLoginTypeGUEST)
	{
		mappedAccountMsg = getLocal(LK::kLinkingX);
	}
	else
	{
		mappedAccountMsg = getLocal(LK::kLinkingUnknown);
		CCLog("logintype = %d", loginType);
	}
	
	KSLabelTTF* mappedMsgTitle = KSLabelTTF::create(getLocal(LK::kLinkingAccount), mySGD->getFont().c_str(), 12.f);
	connectionBox->addChild(mappedMsgTitle);
	mappedMsgTitle->setPosition(ccp(32.0, 35.0)); 		// dt (47.0, 180.0)
	
	KSLabelTTF* memberID = KSLabelTTF::create(getLocal(LK::kMemberNo), mySGD->getFont().c_str(), 12.f);
	connectionBox->addChild(memberID);
	memberID->setPosition(ccp(32.0, 13.0)); 	// dt (47.0, 155.0)
	setFormSetter(mappedMsgTitle);
	setFormSetter(memberID);
	KSLabelTTF* mappedMsgLbl = KSLabelTTF::create(mappedAccountMsg.c_str(), mySGD->getFont().c_str(), 12.f);
	mappedMsgLbl->setAnchorPoint(ccp(0.f, 0.5f));
	mappedMsgLbl->setPosition(ccp(69.5, 35.0)); 				// dt (27.0, -53.5)
	mappedMsgLbl->setColor(ccc3(255, 170, 20));
//	mappedMsgLbl->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 129));
	connectionBox->addChild(mappedMsgLbl);
	
	KSLabelTTF* hspIDLbl = KSLabelTTF::create(ccsf("%llu", myHSP->getMemberID()), mySGD->getFont().c_str(), 12.f);
//	hspIDLbl->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 100));
	hspIDLbl->setAnchorPoint(ccp(0.f, 0.5f));
	connectionBox->addChild(hspIDLbl);
	hspIDLbl->setPosition(ccp(69.5f, 13.0)); 				// dt (24.5, -49.5)
	hspIDLbl->setColor(ccc3(255, 170, 20));
	setFormSetter(mappedMsgLbl);
	setFormSetter(hspIDLbl);
	
	CCSprite* seper0 = CCSprite::create("cardsetting_line.png");
	seper0->setScaleX(235.f/seper0->getContentSize().width);
	front->addChild(seper0);
	seper0->setPosition(ccp(125.5, 153.5));
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	KSLabelTTF* googleGuide = KSLabelTTF::create(ccsf(getLocal(LK::kSaveDesc1), "GameCenter"),
																							 mySGD->getFont().c_str(), 10.f);

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	KSLabelTTF* googleGuide = KSLabelTTF::create(ccsf(getLocal(LK::kSaveDesc1), "Google"),
																							 mySGD->getFont().c_str(), 10.f);
	
#endif
	googleGuide->setPosition(ccp(67, 98.f));
	front->addChild(googleGuide);
	
	std::string gButtonLbl;
	if(loginType == HSPLoginTypeGOOGLE) // 구글 로긴 타입임.
	{
		gButtonLbl = getLocal(LK::kLinked);
	}
	else
	{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		gButtonLbl = ccsf(getLocal(LK::kGoogleButton), "GameCenter");
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		gButtonLbl = ccsf(getLocal(LK::kGoogleButton), "GOOGLE+");
#endif
		
	}
	CommonButton* googleLogin = CommonButton::create(gButtonLbl.c_str(), 14.f, CCSizeMake(101, 60),
																										 CommonButtonAchievement, touchP - 1);
	googleLogin->setZoomOnTouchDown(false);
	
	googleLogin->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(69, -7.5f + 6 - 4.5f + 10.5f));
	
	CCSprite* seper = CCSprite::create("cardsetting_line.png");
	seper->setScaleX(235.f/seper->getContentSize().width);
	front->addChild(seper);
	seper->setPosition(ccpFromSize(front->getContentSize() / 2.f) + ccp(0, -32 + 4.5));
	
	front->addChild(googleLogin);
	
	KSLabelTTF* facebookGuide = KSLabelTTF::create(getLocal(LK::kSaveDesc2),
																							 mySGD->getFont().c_str(), 10.f);
	facebookGuide->setPosition(ccp(67, 33));
	front->addChild(facebookGuide);
	
	std::string fButtonLbl;
	if(loginType == HSPLoginTypeFACEBOOK)
	{
		fButtonLbl = getLocal(LK::kLinked);
	}
	else
	{
		fButtonLbl = getLocal(LK::kFacebookButton);
	}
	CommonButton* facebookLogin = CommonButton::create(fButtonLbl.c_str(), 14.f, CCSizeMake(101, 60),
																										 CommonButtonAchievement, touchP - 1);
	
	facebookLogin->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(69, -79 + 4.5f + 14.5f));
	facebookLogin->setZoomOnTouchDown(false);
	front->addChild(facebookLogin);
	
	
	auto anotherAccountFunctor = [=](int mm, HSPLogin mm2, long long prevMemberNo,
																	 std::string tryName){
		CCLog("another!!!");
		Json::Value param;
		param["memberID"] = prevMemberNo;
		param["retry"] = true;
		param["passCode"] = GDDONTFINDUSER;
//#if __ANDROID__
//#else
////		param["memberID"] = myHSP->getMemberID();
//#endif
		CCLog("-----------------------------------------------------------------------");
		TRACE();

		LoadingLayer* ll = LoadingLayer::create(touchP - 100);
		addChild(ll, INT_MAX);
		myHSP->command("getUserData", param, [=](Json::Value t){
			ll->removeFromParent();
			TRACE();
			KS::KSLog("%", t);
			if(t["result"]["code"].asInt() == GDSUCCESS)
			{
				ASPopupView* toAnotherAccount = ASPopupView::createDimmed(touchP - 2);
				
				toAnotherAccount->getDimmedSprite()->setVisible(false);
				//					toAnotherAccount->getDimmedSprite()->removeFromParent();
				managerPopup->addChild(toAnotherAccount);
				
				
				auto back = CCSprite::create("popup_large_back.png");
				auto front = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));

				CommonAnimation::openPopup(toAnotherAccount, back, toAnotherAccount->getDimmedSprite(), nullptr, nullptr);
				
				toAnotherAccount->setContainerNode(back);
//				back->setContentSize(CCSizeMake(550 / 2.f, 432 / 2.f));
				front->setContentSize(CCSizeMake(251, 128));
				
				
				back->addChild(front);
				front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0, 15));

				//					toAnotherAccount->addChild(back);
				//					CommonAnimation::openPopup(this, back, nullptr);
				
				
				KSLabelTTF* titleLbl = KSLabelTTF::create(getLocal(LK::kWarningDesc), mySGD->getFont().c_str(), 12.f);
//				titleLbl->setColor(ccc3(255, 170, 20));
				back->addChild(titleLbl);
				titleLbl->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(-85, back->getContentSize().height/2.f-35));

				auto nickName = t["nick"].asString();
				if(nickName == "")
				{
					nickName = "옛날유저";
				}
				auto highPiece = t["highPiece"].asInt();
				if(highPiece == 0)
				{
					highPiece = 1;
				}
				std::string guidanceMsg = ccsf( getLocal(LK::kAnotherHistory),
																			 tryName.c_str(),
//																			 descMapper.at((HSPLoginTypeX)myHSP->getLoginType()).c_str(),
																			 nickName.c_str(), highPiece,
																			 myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getUserdataHighPiece());
				StyledLabelTTF* content = StyledLabelTTF::create(
																												 guidanceMsg.c_str() ,
																												 mySGD->getFont().c_str(), 12.f, 999, StyledAlignment::kCenterAlignment);
				front->addChild(content);
				content->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 72.5f - 20.f));
				
				CommonButton* previousLoad = CommonButton::create(getLocal(LK::kLoadPreviousData), 13.f, CCSizeMake(130, 50),
																													CommonButtonAchievement, touchP - 3);
				previousLoad->setPosition(ccp(69.5, -32.f)); 			// dt (-1.0, 9.0)
				front->addChild(previousLoad);
				CommonButton* keepLoad = CommonButton::create(getLocal(LK::kSaveCurrentData), 13.f, CCSizeMake(110, 50),
																											CommonButtonAchievement, touchP - 3);
				keepLoad->setPosition(ccp(193.0, -32.f)); 			// dt (-57.0, 9.0)
				front->addChild(keepLoad);
				CommonButton* closeButton = CommonButton::createCloseButton(touchP - 3);
				closeButton->setPosition(ccp(back->getContentSize().width-25,back->getContentSize().height-22));
				back->addChild(closeButton);
				closeButton->setFunction([=](CCObject*){
					CommonAnimation::closePopup(this, back, nullptr, nullptr,
																			[=](){
																				toAnotherAccount->removeFromParent();
																			});
					
				});
				previousLoad->setFunction([=](CCObject*){
					
					
					if(loginType != HSPLoginTypeX::HSPLoginTypeGUEST)
					{
//						mentString = ccsf("<font color=#FF00000 newline=14>%s 와의 연동을 해제합니다.</font>", descMapper.at(loginType).c_str());
					}
					auto loadFunction = [=]()
					{
						{
							CCLog("-----------------------------------------------------------------------");
							TRACE();
							LoadingLayer* ll = LoadingLayer::create(touchP - 100);
							addChild(ll, INT_MAX);
							hspConnector::get()->logout([=](Json::Value result_data){
								TRACE();
								ll->removeFromParent();
								CCLOG("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
								if(result_data["error"]["isSuccess"].asBool())
								{
									// 매핑 없이 로그인 시도. 이전기록 불러오기
									
									mySGD->resetLabels();
									SaveData::sharedObject()->resetAllData();
									myDSH->removeCache();
									mySDS->removeCache();
									myDSH->setStringForKey(kDSH_Key_savedMemberID, boost::lexical_cast<std::string>(prevMemberNo));
									TRACE();
									CCLOG("save accountType %d", mm2);
									myDSH->setIntegerForKey(kDSH_Key_accountType, (int)mm2);
									myDSH->setBoolForKey(kDSH_Key_isCheckTerms, true); // 약관 동의~~~
									myDSH->setIntegerForKey(kDSH_Key_clientVersion, mySGD->client_version);
									CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
									
									Json::Value param;
									param["loginType"] = -1*myDSH->getIntegerForKeyDefault(kDSH_Key_accountType, (int)HSPLogin::GUEST);;
									
									myHSP->command("updateuserdata", param, nullptr);
#if 0             ///////////////////// 아래 코드는 동작안함
									// 게스트인 경우 드롭아웃시키고 완료됐다고 띄우고
									// 게스트가 아닌 경우 바로 매핑 완료 시킴.
									if(loginType == HSPLoginTypeX::HSPLoginTypeGUEST)
									{
										Json::Value param;
										param["memberID"] = hspConnector::get()->getMemberID();
										// 성공하든 실패하든 걍 무조건 일단 날림. 트랜잭션이 안되기 때문에 어쩔 수 없음.
										myHSP->command("dropoutuser", param, [=](Json::Value v)
																	 {
																		CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																	 });
									}
									else
									{
									}
#endif
								}
								else
								{
									//					loading_progress_img->removeFromParent();
									//					cancel_button->setEnabled(true);
									//					ok_button->setEnabled(true);
									//
									//					cancel_button->setVisible(true);
									//					ok_button->setVisible(true);
									
									CCLOG("fail logout");
								}
							});
							
							
						}
					};
					
					if(loginType == HSPLoginTypeX::HSPLoginTypeGUEST)
					{
						std::string mentString;
						mentString += ccsf(getLocal(LK::kForeverDeleteAccount1),
															 tryName.c_str());
						StyledLabelTTF* ment = StyledLabelTTF::create(mentString.c_str(),
																													mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
						ment->setAnchorPoint(ccp(0.5f, 0.5f));
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
						this->showWarning("", HSPMapping::kGOOGLE, HSPLogin::GOOGLE, ment, loadFunction);
#else
						this->showWarning("", HSPMapping::kGAMECENTER, HSPLogin::GOOGLE, ment, loadFunction);
#endif
					}
					else
					{
						std::string mentString;
						if(loginType != HSPLoginTypeX::HSPLoginTypeGUEST)
						{
							mentString = ccsf(getLocal(LK::kUnlinkAccount1),
																descMapper.at(loginType).c_str());
						}

//						mentString += ccsf(getLocal(LK::kForeverDeleteAccount1),
//															 tryName.c_str());
						StyledLabelTTF* ment = StyledLabelTTF::create(mentString.c_str(),
																													mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
						ment->setAnchorPoint(ccp(0.5f, 0.5f));
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
						this->showWarning("", HSPMapping::kGOOGLE, HSPLogin::GOOGLE, ment, loadFunction);
#else
						this->showWarning("", HSPMapping::kGAMECENTER, HSPLogin::GOOGLE, ment, loadFunction);
#endif
//						loadFunction();
					}
				});
				
				keepLoad->setFunction([=](CCObject*){
					// 강제 매핑
					std::string mentString;
					
					if(loginType != HSPLoginTypeX::HSPLoginTypeGUEST)
					{
						mentString = ccsf(getLocal(LK::kUnlinkAccount1),
															descMapper.at(loginType).c_str());
					}
					mentString += ccsf(getLocal(LK::kForeverDeleteAccount2),
														 tryName.c_str());
					StyledLabelTTF* ment = StyledLabelTTF::create(mentString.c_str(),
																												mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
					ment->setAnchorPoint(ccp(0.5f, 0.5f));
					this->showWarning("",
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
														HSPMapping::kGOOGLE,
#else
														HSPMapping::kGAMECENTER,
#endif
														HSPLogin::GOOGLE, ment, [=]()
														{
															CCLog("-----------------------------------------------------------------------");
															TRACE();

															LoadingLayer* ll = LoadingLayer::create(touchP - 100);
															addChild(ll, INT_MAX);
															
															hspConnector::get()->mappingToAccount(mm, true, [=](Json::Value t){
																TRACE();
																ll->removeFromParent();
																KS::KSLog("keepLoad %", t);
																if(t["error"]["isSuccess"].asInt())
																{
																	m_logoutRepeater1 = [=](Json::Value result_data){
																		TRACE();
																		//																		ll->removeFromParent();
																		CCLOG("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
																		if(result_data["error"]["isSuccess"].asBool())
																		{
																			mySGD->resetLabels();
																			myDSH->setIntegerForKey(kDSH_Key_accountType, (int)mm2);
																			CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																		}
																		else
																		{
																			hspConnector::get()->logout(m_logoutRepeater1);
																		}
																	};
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
																	hspConnector::get()->logout(m_logoutRepeater1);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
																	mySGD->resetLabels();
																	myDSH->setIntegerForKey(kDSH_Key_accountType, (int)mm2);
																	CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
#endif
																	
																}
																else
																{
																	
																}
															});
														});
					
				});
			}
			else
			{
				// 원래는 에러가 떠야 하지만 원래 없었던 데이터 처럼 로그인 시킴.
				CCLog("-----------------------------------------------------------------------");
				TRACE();
				LoadingLayer* ll = LoadingLayer::create(touchP - 100);
				addChild(ll, INT_MAX);
				hspConnector::get()->mappingToAccount(mm, true, [=](Json::Value t){
					TRACE();
					ll->removeFromParent();
					KS::KSLog("force %", t);
					if(t["error"]["isSuccess"].asInt())
					{
						mySGD->resetLabels();
						myDSH->setIntegerForKey(kDSH_Key_accountType, (int)mm2);
						std::string msg;
						
						if(loginType != HSPLoginTypeX::HSPLoginTypeGUEST)
						{
							msg = ccsf(getLocal(LK::kUnlinkAccount1),
												 descMapper.at(loginType).c_str());
						}
						msg += ccsf(getLocal(LK::kLinkAccount1),
												tryName.c_str()
												);
						auto content = StyledLabelTTF::create(msg.c_str(),
																									mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
						content->setAnchorPoint(ccp(0.5f, 0.5f));
						
						ASPopupView* alert = ASPopupView::getCommonNoti2(touchP - 2, getLocal(LK::kWarningDesc),
																														 content,
																														 [=]()
																														 {
																															 CCLOG("ttTT");
																															 TRACE();
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
																															 m_logoutRepeater2 = [=](Json::Value result_data){
																																 CCLOG("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
																																 if(result_data["error"]["isSuccess"].asBool())
																																 {
																																	 CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																																 }
																																 else
																																 {
																																	 hspConnector::get()->logout(m_logoutRepeater2);
																																 }
																															 };
																															 hspConnector::get()->logout(m_logoutRepeater2);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
																															 CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
#endif
																															 
																														 });
						addChild(alert);

					}
					else
					{
						
					}
				});
				
				CCLog("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
//				mySGD->resetLabels();
//				myDSH->setIntegerForKey(kDSH_Key_accountType, (int)mm2);
//				std::string msg;
//				
//				if(loginType != HSPLoginTypeX::HSPLoginTypeGUEST)
//				{
//					msg = ccsf(getLocal(LK::kUnlinkAccount1),
//										 descMapper.at(loginType).c_str());
//				}
//				msg += ccsf(getLocal(LK::kLinkAccount1),
//										tryName.c_str()
//										);
//				auto content = StyledLabelTTF::create(msg.c_str(),
//																							mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
//				content->setAnchorPoint(ccp(0.5f, 0.5f));
//				
//				ASPopupView* alert = ASPopupView::getCommonNoti2(touchP - 2, getLocal(LK::kWarningDesc),
//																												 content,
//																												 [=]()
//																												 {
//																													 CCLOG("ttTT");
//																													 CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
//																												 });
//				addChild(alert);

//				auto st = StyledLabelTTF::create(getLocal(LK::kNetworkError),
//															 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
//				
//				st->setAnchorPoint(ccp(0.5f, 0.5f));
//				ASPopupView* alert = ASPopupView::getCommonNoti2(touchP - 2, "LQError",
//																												 st, nullptr);
//				addChild(alert);
			}
		});
	};
	auto doLogin = [=](int hspmap, const std::string& tryName, HSPLogin willSaveLogin){
		CCLog("-----------------------------------------------------------------------");
		TRACE();
	
		LoadingLayer* ll = LoadingLayer::create(touchP - 100);
		addChild(ll, INT_MAX);
		hspConnector::get()->mappingToAccount(hspmap, false, [=](Json::Value t){
			CCLog("-----------------------------------------------------------------------");
			TRACE();

			KS::KSLog("hhh %", t);
			KS::KSLog("%", t);
			ll->removeFromParent();
			if(t["error"]["isSuccess"].asBool()) {
				CCLog("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
				mySGD->resetLabels();
				myDSH->setIntegerForKey(kDSH_Key_accountType, (int)willSaveLogin);
				std::string msg;
				
				if(loginType != HSPLoginTypeX::HSPLoginTypeGUEST)
				{
					msg = ccsf(getLocal(LK::kUnlinkAccount1),
										 descMapper.at(loginType).c_str());
				}
				msg += ccsf(getLocal(LK::kLinkAccount1),
															 tryName.c_str()
															 );
				auto content = StyledLabelTTF::create(msg.c_str(),
																							mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
				content->setAnchorPoint(ccp(0.5f, 0.5f));
				
				ASPopupView* alert = ASPopupView::getCommonNoti2(touchP - 2, getLocal(LK::kWarningDesc),
																												 content,
																												 [=]()
																												 {
																													 CCLOG("ttTT");
																													 m_logoutRepeater3 = [=](Json::Value result_data){
																														 CCLOG("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
																														 if(result_data["error"]["isSuccess"].asBool())
																														 {
																															 CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
																														 }
																														 else
																														 {
																															 hspConnector::get()->logout(m_logoutRepeater3);
																														 }
																														};
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
																													 hspConnector::get()->logout(m_logoutRepeater3);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
																													 CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
#endif
																													 
																												 });
				addChild(alert);
				
				
			}
			else {
				if(t["error"]["code"].asInt() == 0x0014006D) {
					anotherAccountFunctor(hspmap, willSaveLogin, t["prevMemberNo"].asUInt64(), tryName);
					CCLog("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
				}
				else {
					auto ment	= StyledLabelTTF::create(getLocal(LK::kCantLinking),
																						 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
					ment->setAnchorPoint(ccp(0.5f, 0.5f));
					ASPopupView* alert = ASPopupView::getCommonNoti2(touchP - 2, getLocal(LK::kWarningDesc),
																													 ment, nullptr);
					addChild(alert);
				}
			}
		});
	};
	
	auto tryLogin = [=](int hspmap, const std::string& tryName, HSPLogin willSaveLogin){
		HSPLoginTypeX loginType = (HSPLoginTypeX)myHSP->getLoginType();
		doLogin(hspmap, tryName, willSaveLogin);
//		if(loginType == HSPLoginTypeGUEST) { // 뭔가에 연결 되어 있다면...
//			showWarning(tryName, hspmap, willSaveLogin);
//		}
//		else {
//			doLogin(hspmap, tryName, willSaveLogin);
//		}
	};
	
	if(loginType != HSPLoginTypeFACEBOOK) // 페이스북이 아닌 경우에만~
	{
		facebookLogin->setFunction([=](CCObject*){
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
			tryLogin((int)HSPMapping::kFACEBOOK, "Facebook ID", HSPLogin::FACEBOOK);
		});
	}
	
	if(loginType != HSPLoginTypeGOOGLE) // 구글로그인이 안되어있는 경우에만...
	{
		googleLogin->setFunction([=](CCObject*){
			AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
			tryLogin((int)HSPMapping::kGOOGLE, "Google ID", HSPLogin::GOOGLE);
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
			tryLogin((int)HSPMapping::kGAMECENTER, "GameCenter ID", HSPLogin::GAMECENTER);
#endif

//			tryLogin((int)HSPMapping::kGOOGLE, "Google ID", HSPLogin::GOOGLE);
		});

	}
	
	
	setFormSetter(front);
	setFormSetter(back);
	setFormSetter(seper);
	setFormSetter(seper0);
	
	setFormSetter(googleLogin);
	setFormSetter(connectionBox);
	
	setFormSetter(googleGuide);
	setFormSetter(facebookLogin);
	setFormSetter(facebookGuide);
	setFormSetter(closeButton);
	setFormSetter(titleLbl);
	return true;
}
void AccountManagerPopup::showWarning(const std::string& desc, HSPMapping hspmap, HSPLogin willSaveLogin,
											 StyledLabelTTF* announce,
											 std::function<void(void)> loginProcess){
	ASPopupView* warningPopup = ASPopupView::createDimmed(m_touchP - 4);
	
	warningPopup->getDimmedSprite()->setVisible(false);
	m_managerPopup->addChild(warningPopup);
	
	auto back = CCSprite::create("popup_large_back.png");
	
	CommonAnimation::openPopup(warningPopup, back, warningPopup->getDimmedSprite(), nullptr, nullptr);
	auto front = CCScale9Sprite::create("common_grayblue.png",
																			CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	
	//	front->setVisible(false);
	warningPopup->setContainerNode(back);
	//	back->setContentSize(CCSizeMake(550 / 2.f, 506 / 2.f));
	//	back->setContentSize(CCSizeMake(200, 200));
	front->setContentSize(CCSizeMake(251, 141));
	
	
	back->addChild(front);
	
	front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0, 15));
	
	KSLabelTTF* titleLbl = KSLabelTTF::create(getLocal(LK::kWarningDesc), mySGD->getFont().c_str(), 12.f);
	//	titleLbl->setColor(ccc3(255, 170, 20));
	titleLbl->setAnchorPoint(ccp(0.5f,0.5f));
	titleLbl->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(-85, back->getContentSize().height/2.f-35));
	back->addChild(titleLbl);
	
	
	CommonButton* closeButton = CommonButton::createCloseButton(m_touchP - 4);
	closeButton->setFunction([=](CCObject*)
													 {
														 CommonAnimation::closePopup(this, back, warningPopup->getDimmedSprite(), nullptr,
																												 [=]()
																												 {
																													 warningPopup->removeFromParent();
																												 });
													 });
	back->addChild(closeButton);
	closeButton->setPosition(ccp(back->getContentSize().width-25, back->getContentSize().height-22));
	
	
	front->addChild(announce);
	announce->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 0));
	
	
	CommonButton* connect = CommonButton::create(getLocal(LK::kLinkingCommand), 13.f, CCSizeMake(110, 50),
																							 CommonButtonAchievement, m_touchP - 4);
	connect->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, -99.5f));
	front->addChild(connect);
	connect->setFunction([=](CCObject*){
		loginProcess();
		//			doLogin(hspmap, desc, willSaveLogin);
	});
	startFormSetter(this);
	auto front2 = front;
	auto content2 = announce;
	setFormSetter(front2);
	setFormSetter(back);
	setFormSetter(closeButton);
	setFormSetter(content2);
	setFormSetter(titleLbl);
	setFormSetter(connect);
	
};

