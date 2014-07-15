#include "AccountManagerPopup.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "ASPopupView.h"
#include "DataStorageHub.h"
#include "FormSetter.h"
#include "KSLabelTTF.h"
#include "StyledLabelTTF.h"
#include "TitleRenewal.h"
#include "HSPEnums.h"
#include "LoadingLayer.h"
#include <boost/lexical_cast.hpp>
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

	startFormSetter(this);
	ASPopupView* managerPopup = ASPopupView::createDimmed(touchP);
	addChild(managerPopup);
	
	
//	auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	auto back = CCSprite::create("popup_large_back.png");

	auto front = CCScale9Sprite::create("common_grayblue.png",
																			CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));

//	front->setVisible(false);
	managerPopup->setContainerNode(back);
//	back->setContentSize(CCSizeMake(550 / 2.f, 506 / 2.f));
//	back->setContentSize(CCSizeMake(200, 200));
	front->setContentSize(CCSizeMake(251, 370 / 2.f));
	

	back->addChild(front);
	
	front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0,15 - 26));

	KSLabelTTF* titleLbl = KSLabelTTF::create("계정 설정", mySGD->getFont().c_str(), 12.f);
//	titleLbl->setColor(ccc3(255, 170, 20));
	titleLbl->setAnchorPoint(ccp(0.5f,0.5f));
	titleLbl->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(-85, back->getContentSize().height/2.f-35));
	back->addChild(titleLbl);

	
	CommonButton* closeButton = CommonButton::createCloseButton(touchP - 1);
	closeButton->setFunction([=](CCObject*)
													 {
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
		mappedAccountMsg = "구글과 연결되어 있습니다.";
	}
	else
	if(loginType == HSPLoginTypeFACEBOOK)
	{
		mappedAccountMsg = "페이스북과 연결되어 있습니다.";
		
	}
	else if(loginType == HSPLoginTypeGUEST)
	{
		mappedAccountMsg = "연결이 안되어 있습니다.";
	}
	else
	{
		mappedAccountMsg = "알 수 없습니다.";
		CCLog("logintype = %d", loginType);
	}
	
	KSLabelTTF* mappedMsgTitle = KSLabelTTF::create("연결계정", mySGD->getFont().c_str(), 12.f);
	connectionBox->addChild(mappedMsgTitle);
	mappedMsgTitle->setPosition(ccp(32.0, 35.0)); 		// dt (47.0, 180.0)
	
	KSLabelTTF* memberID = KSLabelTTF::create("회원번호", mySGD->getFont().c_str(), 12.f);
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
	
	KSLabelTTF* googleGuide = KSLabelTTF::create("게임을 저장하시려면\n Google+ ID를\n 연결해 주세요.",
																							 mySGD->getFont().c_str(), 10.f);
	googleGuide->setPosition(ccp(67, 98.f));
	front->addChild(googleGuide);
	CommonButton* googleLogin = CommonButton::create("GOOGLE+\nID 연결", 14.f, CCSizeMake(101, 60),
																										 CommonButtonAchievement, touchP - 1);
	
	googleLogin->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(69, -7.5f + 6 - 4.5f + 10.5f));
	
	CCSprite* seper = CCSprite::create("cardsetting_line.png");
	seper->setScaleX(235.f/seper->getContentSize().width);
	front->addChild(seper);
	seper->setPosition(ccpFromSize(front->getContentSize() / 2.f) + ccp(0, -32 + 4.5));
	
	front->addChild(googleLogin);
	
	KSLabelTTF* facebookGuide = KSLabelTTF::create("여러 기기에서 게임을\n 즐기시려면 facebook ID를\n 연결해 주세요.",
																							 mySGD->getFont().c_str(), 10.f);
	facebookGuide->setPosition(ccp(67, 33));
	front->addChild(facebookGuide);
	
	CommonButton* facebookLogin = CommonButton::create("FACEBOOK\n ID 연결", 14.f, CCSizeMake(101, 60),
																										 CommonButtonAchievement, touchP - 1);
	
	facebookLogin->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(69, -79 + 4.5f + 14.5f));

	front->addChild(facebookLogin);
	
	
	auto anotherAccountFunctor = [=](enum HSPMapping mm, HSPLogin mm2, long long prevMemberNo){
		CCLog("another!!!");
		Json::Value param;
		param["memberID"] = myHSP->getMemberID();
		LoadingLayer* ll = LoadingLayer::create(touchP - 100);
		addChild(ll, INT_MAX);
		myHSP->command("getUserData", param, [=](Json::Value t){
			ll->removeFromParent();
			KS::KSLog("%", t);
			if(t["result"]["code"].asInt() == GDSUCCESS)
			{
				ASPopupView* toAnotherAccount = ASPopupView::createDimmed(touchP - 2);
				//					toAnotherAccount->getDimmedSprite()->removeFromParent();
				managerPopup->addChild(toAnotherAccount);
				
				
				auto back = CCSprite::create("popup_large_back.png");
				auto front = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));

				
				toAnotherAccount->setContainerNode(back);
//				back->setContentSize(CCSizeMake(550 / 2.f, 432 / 2.f));
				front->setContentSize(CCSizeMake(251,128));
				
				
				back->addChild(front);
				front->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(0,15));

				//					toAnotherAccount->addChild(back);
				//					CommonAnimation::openPopup(this, back, nullptr);
				
				KSLabelTTF* titleLbl = KSLabelTTF::create("유의사항", mySGD->getFont().c_str(), 12.f);
//				titleLbl->setColor(ccc3(255, 170, 20));
				back->addChild(titleLbl);
				titleLbl->setPosition(ccpFromSize(back->getContentSize()/2.f) + ccp(-85, back->getContentSize().height/2.f-35));

				std::string guidanceMsg = ccsf( "<font color=#FFFFFF newline=12>이 ID에 연결된</font>"
																											 "<font color=#FFFFFF newline=12>다른 게임 기록이 있습니다.</font>"
																											 "<font color=#FFFFFF newline=24>(%s, %d)</font>"
																											 "<font color=#FFAA14 newline=24>이전 기록을 불러오시겠습니까?</font>"
																											 "<font color=#FFFFFF newline=12>이전 게임 기록을 불러오면</font>"
																											 "<font color=#FFFFFF newline=12>현재 게임 기록(%s, %d)은 삭제됩니다.</font>",
																			 t["data"]["nick"].asString().c_str(), t["highPiece"].asInt(),
																			 myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getUserdataHighPiece());
				StyledLabelTTF* content = StyledLabelTTF::create(
																												 guidanceMsg.c_str() ,
																												 mySGD->getFont().c_str(), 12.f, 999, StyledAlignment::kCenterAlignment);
				front->addChild(content);
				content->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 72.5f - 20.f));
				
				CommonButton* previousLoad = CommonButton::create("이전 기록 불러오기", 13.f, CCSizeMake(110, 50),
																													CommonButtonAchievement, touchP - 3);
				previousLoad->setPosition(ccp(69.5, -32.f)); 			// dt (-1.0, 9.0)
				front->addChild(previousLoad);
				CommonButton* keepLoad = CommonButton::create("현재 기록을 저장", 13.f, CCSizeMake(110, 50),
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
					LoadingLayer* ll = LoadingLayer::create(touchP - 100);
					addChild(ll, INT_MAX);
					hspConnector::get()->logout([=](Json::Value result_data){
						ll->removeFromParent();
						CCLOG("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
						if(result_data["error"]["isSuccess"].asBool())
						{
							// 매핑 없이 로그인 시도. 이전기록 불러오기
							myDSH->setStringForKey(kDSH_Key_savedMemberID, boost::lexical_cast<std::string>(prevMemberNo));
							myDSH->setIntegerForKey(kDSH_Key_accountType, (int)mm2);
							mySGD->resetLabels();
							CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
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
					
				});
				
				keepLoad->setFunction([=](CCObject*){
					// 강제 매핑
					LoadingLayer* ll = LoadingLayer::create(touchP - 100);
					addChild(ll, INT_MAX);
					hspConnector::get()->mappingToAccount(mm, true, [=](Json::Value t){
						ll->removeFromParent();
						KS::KSLog("force %", t);
						if(t["error"]["isSuccess"].asInt())
						{
							myDSH->setIntegerForKey(kDSH_Key_accountType, (int)mm2);
							mySGD->resetLabels();
							CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
						}
						else
						{
							
						}
					});
				});
				setFormSetter(front);
				setFormSetter(back);
				setFormSetter(closeButton);
				setFormSetter(content);
				setFormSetter(titleLbl);
				setFormSetter(previousLoad);
				setFormSetter(keepLoad);
			}
			else
			{
				ASPopupView* alert = ASPopupView::getCommonNoti2(touchP - 2, "LQError",
																												 StyledLabelTTF::create("<font color=#FFFFFF newline=12>통신 에러</font>"
																																								"<font color=#FFAA14>다시 시도해 주세요.</font>",
																																								mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment), nullptr);
				addChild(alert);
			}
		});
	};
	
	facebookLogin->setFunction([=](CCObject*){
		LoadingLayer* ll = LoadingLayer::create(touchP - 100);
		addChild(ll, INT_MAX);
		hspConnector::get()->mappingToAccount(HSPMapping::kFACEBOOK, false, [=](Json::Value t){
			KS::KSLog("hhh %", t);
			KS::KSLog("%", t);
			ll->removeFromParent();
			if(t["error"]["isSuccess"].asBool())
			{
					CCLog("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
					mySGD->resetLabels();
					CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
					myDSH->setIntegerForKey(kDSH_Key_accountType, (int)HSPLogin::FACEBOOK);
			}
			else
			{
				if(t["error"]["code"].asInt() == 0x0014006D)
				{
					anotherAccountFunctor(HSPMapping::kFACEBOOK, HSPLogin::FACEBOOK, t["prevMemberNo"].asUInt64());
					CCLog("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
				}
				else
				{
					ASPopupView* alert = ASPopupView::getCommonNoti2(touchP - 2, "유의사항",
													StyledLabelTTF::create("<font color=#FFFFFF newline=12>연결 할 수 없습니다.</font>"
																								 "<font color=#FFAA14>다시 시도해 주세요.</font>",
																								 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment), nullptr);
					addChild(alert);
				}
			}
		});
		
	});
	googleLogin->setFunction([=](CCObject*){
		LoadingLayer* ll = LoadingLayer::create(touchP - 100);
		addChild(ll, INT_MAX);
		hspConnector::get()->mappingToAccount(HSPMapping::kGOOGLE, false, [=](Json::Value t){
			ll->removeFromParent();
			KS::KSLog("%", t);
			if(t["error"]["isSuccess"].asBool())
			{
					CCLog("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
					mySGD->resetLabels();
					CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
					myDSH->setIntegerForKey(kDSH_Key_accountType, (int)HSPLogin::GOOGLE);
			}
			else
			{
				if(t["error"]["code"].asInt() == 0x0014006D)
				{
					anotherAccountFunctor(HSPMapping::kGOOGLE, HSPLogin::GOOGLE, t["prevMemberNo"].asUInt64());
					CCLog("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
				}
				else
				{
					
				}
			}
		});
	});
	
	
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


