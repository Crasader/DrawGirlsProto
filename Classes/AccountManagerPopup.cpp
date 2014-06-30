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
	
	
	auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	auto front = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	
	managerPopup->setContainerNode(back);
	back->setContentSize(CCSizeMake(550 / 2.f, 506 / 2.f));
	front->setContentSize(CCSizeMake(528 / 2.f, 418 / 2.f));
	

	back->addChild(front);
	
	front->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -16.5));

	KSLabelTTF* titleLbl = KSLabelTTF::create("계정 설정", mySGD->getFont().c_str(), 15.f);
	titleLbl->setColor(ccc3(255, 170, 20));
	titleLbl->setPosition(ccpFromSize(back->getContentSize() / 2.f) + ccp(0, 99.5f + 2.5f));
	back->addChild(titleLbl);

	
	CommonButton* closeButton = CommonButton::createCloseButton(touchP - 1);
	closeButton->setFunction([=](CCObject*){
		CommonAnimation::closePopup(this, back, managerPopup->getDimmedSprite(), nullptr,
																[=](){
																	managerPopup->removeFromParent();
																});
	});
	back->addChild(closeButton);
	closeButton->setPosition(ccp(251, 229.5f));
	
	auto connectionBox = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	connectionBox->setContentSize(CCSizeMake(240, 60));
	front->addChild(connectionBox);
	connectionBox->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 62.5));

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
	front->addChild(mappedMsgTitle);
	mappedMsgTitle->setPosition(ccp(47.0, 180.0)); 			// dt (47.0, 180.0)
	
	KSLabelTTF* memberID = KSLabelTTF::create("회원번호", mySGD->getFont().c_str(), 12.f);
	front->addChild(memberID);
	memberID->setPosition(ccp(47.0, 155.0)); 			// dt (47.0, 155.0)
	setFormSetter(mappedMsgTitle);
	setFormSetter(memberID);
	KSLabelTTF* mappedMsgLbl = KSLabelTTF::create(mappedAccountMsg.c_str(), mySGD->getFont().c_str(), 12.f);
	mappedMsgLbl->setPosition(ccp(159.0, 180.0)); 			// dt (27.0, -53.5)
//	mappedMsgLbl->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 129));
	front->addChild(mappedMsgLbl);
	
	KSLabelTTF* hspIDLbl = KSLabelTTF::create(ccsf("%llu", myHSP->getMemberID()), mySGD->getFont().c_str(), 12.f);
//	hspIDLbl->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 100));
	front->addChild(hspIDLbl);
	hspIDLbl->setPosition(ccp(156.5, 155.0)); 			// dt (24.5, -49.5)
	setFormSetter(mappedMsgLbl);
	setFormSetter(hspIDLbl);
	
	CCSprite* seper0 = CCSprite::create("cardsetting_line.png");
	seper0->setScaleX(235.f/seper0->getContentSize().width);
	front->addChild(seper0);
	seper0->setPosition(ccpFromSize(front->getContentSize() / 2.f) + ccp(0, 63));
	
	KSLabelTTF* googleGuide = KSLabelTTF::create("게임을 저장하시려면 Google+ ID를 연결해 주세요.",
																							 mySGD->getFont().c_str(), 10.f);
	googleGuide->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 16 + 6));
	front->addChild(googleGuide);
	CommonButton* googleLogin = CommonButton::create("GOOGLE+ ID 연결", 14.f, CCSizeMake(160, 50),
																										 CommonButtonLightPupple, touchP - 1);
	
	googleLogin->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, -7.5f + 6 - 4.5f));
	
	CCSprite* seper = CCSprite::create("cardsetting_line.png");
	seper->setScaleX(235.f/seper->getContentSize().width);
	front->addChild(seper);
	seper->setPosition(ccpFromSize(front->getContentSize() / 2.f) + ccp(0, -32));
	
	front->addChild(googleLogin);
	
	KSLabelTTF* facebookGuide = KSLabelTTF::create("여러 기기에서 게임을 즐기시려면 facebook ID를 연결해 주세요.",
																							 mySGD->getFont().c_str(), 10.f);
	facebookGuide->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, -54 + 8));
	front->addChild(facebookGuide);
	
	CommonButton* facebookLogin = CommonButton::create("FACEBOOK ID 연결", 14.f, CCSizeMake(160, 50),
																										 CommonButtonLightPupple, touchP - 1);
	
	facebookLogin->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, -79 + 4.5f));

	front->addChild(facebookLogin);
	
	
	auto anotherAccountFunctor = [=](enum HSPMapping mm, HSPLogin mm2){
		CCLog("another!!!");
		ASPopupView* toAnotherAccount = ASPopupView::createDimmed(touchP - 2);
		//					toAnotherAccount->getDimmedSprite()->removeFromParent();
		managerPopup->addChild(toAnotherAccount);
		
		
		auto back = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		auto front = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
		
		toAnotherAccount->setContainerNode(back);
		back->setContentSize(CCSizeMake(550 / 2.f, 432 / 2.f));
		front->setContentSize(CCSizeMake(528 / 2.f, 346 / 2.f));
		
		
		back->addChild(front);
		front->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, -17));
		//					toAnotherAccount->addChild(back);
		//					CommonAnimation::openPopup(this, back, nullptr);
		
		KSLabelTTF* titleLbl = KSLabelTTF::create("유의사항", mySGD->getFont().c_str(), 15.f);
		titleLbl->setColor(ccc3(255, 170, 20));
		back->addChild(titleLbl);
		titleLbl->setPosition(ccpFromSize(back->getContentSize()) / 2.f + ccp(0, 40 + 43.5));
		
		StyledLabelTTF* content = StyledLabelTTF::create(
																										 "<font color=#FFFFFF newline=12>이 ID에 연결된</font>"
																										 "<font color=#FFFFFF newline=12>다른 게임 기록이 있습니다.</font>"
																										 "<font color=#FFFFFF newline=24>(닉네임~, 레벨~~)</font>"
																										 "<font color=#FFAA14 newline=24>이전 기록을 불러오시겠습니까?</font>"
																										 "<font color=#FFFFFF newline=12>이전 게임 기록을 불러오면</font>"
																										 "<font color=#FFFFFF newline=12>현재 게임 기록은 삭제됩니다.</font>" ,
																										 mySGD->getFont().c_str(), 12.f, 999, StyledAlignment::kCenterAlignment);
		front->addChild(content);
		content->setPosition(ccpFromSize(front->getContentSize()) / 2.f + ccp(0, 72.5f));
		
		CommonButton* previousLoad = CommonButton::create("이전 기록 불러오기", 13.f, CCSizeMake(110, 50),
																											CommonButtonLightPupple, touchP - 3);
		previousLoad->setPosition(ccp(69.5, 37.0)); 			// dt (-1.0, 9.0)
		front->addChild(previousLoad);
		CommonButton* keepLoad = CommonButton::create("현재 기록을 저장", 13.f, CCSizeMake(110, 50),
																									CommonButtonLightPupple, touchP - 3);
		keepLoad->setPosition(ccp(193.0, 37.0)); 			// dt (-57.0, 9.0)
		front->addChild(keepLoad);
		CommonButton* closeButton = CommonButton::createCloseButton(touchP - 3);
		closeButton->setPosition(ccp(244.5, 187.0)); 			// dt (244.5, 187.0)
		front->addChild(closeButton);
		closeButton->setFunction([=](CCObject*){
			CommonAnimation::closePopup(this, back, nullptr, nullptr,
																	[=](){
																		toAnotherAccount->removeFromParent();
																	});
			
		});
		previousLoad->setFunction([=](CCObject*){
			hspConnector::get()->logout([=](Json::Value result_data){
				CCLOG("resultLogout data : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
//				if(result_data["error"]["isSuccess"].asBool())
				{
					hspConnector::get()->mappingToAccount(mm, true, [=](Json::Value t){
						
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
//					myDSH->clear();
//					myDSH->resetDSH();
//					CCDirector::sharedDirector()->replaceScene(TitleRenewalScene::scene());
				}
//				else
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
			hspConnector::get()->mappingToAccount(mm, true, [=](Json::Value t){
				
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
	};
	
	facebookLogin->setFunction([=](CCObject*){
		hspConnector::get()->mappingToAccount(HSPMapping::kFACEBOOK, false, [=](Json::Value t){
			KS::KSLog("hhh %", t);
			KS::KSLog("%", t);
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
					anotherAccountFunctor(HSPMapping::kGOOGLE, HSPLogin::FACEBOOK);
					CCLog("%s %s %d", __FILE__, __FUNCTION__, __LINE__);
				}
				else
				{
					
				}
			}
		});
		
	});
	googleLogin->setFunction([=](CCObject*){
		hspConnector::get()->mappingToAccount(HSPMapping::kGOOGLE, false, [=](Json::Value t){
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
					anotherAccountFunctor(HSPMapping::kGOOGLE, HSPLogin::GOOGLE);
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


