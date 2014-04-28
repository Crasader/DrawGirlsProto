#include "HelpResultSend.h"
#include "KSAlertView.h"
#include "CCMenuLambda.h"
#include "StarGoldData.h"
#include "hspConnector.h"
#include "HatGacha.h"
#include "GachaPurchase.h"
#include "KHAlertView.h"
#include "CommonButton.h"
#include "DataStorageHub.h"
HelpResultSend::HelpResultSend()
{
	
}
HelpResultSend::~HelpResultSend()
{
	CCLOG("help end!!");
}
//void HelpResult::registerWithTouchDispatcher()
//{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
//}

//bool HelpResult::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}

bool HelpResultSend::init(const std::string& corp_id, bool isSuccess, std::function<void(void)> endFunction)
{
	CCLayer::init();
	if(isSuccess)
	{
		// 도움이 성공되었다.
		
		addChild(GachaPurchase::create(kGachaPurchaseStartMode_reward,
																	 [=](){
																		 endFunction();
																		 removeFromParent();
																	 }
																	 ), 1);
		
		
		Json::Value p;
		Json::Value contentJson;
		//		contentJson["msg"] = (nickname + "님에게 도전!");
		contentJson["helpstage"] = mySD->getSilType();
		
		int stage_number = mySD->getSilType();
		int take_level;
		if(mySGD->is_exchanged && mySGD->is_showtime)		take_level = 3;
		else if(mySGD->is_exchanged || mySGD->is_showtime)	take_level = 2;
		else												take_level = 1;
		
		contentJson["cardnumber"] = NSDS_GI(stage_number, kSDS_SI_level_int1_card_i, take_level);
		contentJson["result"] = "success";
		contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
		p["receiverMemberID"] = corp_id;
		p["senderMemberID"] = hspConnector::get()->getSocialID();
		p["type"] = kHelpResult;
		p["content"] = GraphDogLib::JsonObjectToString(contentJson);
		hspConnector::get()->command
		("sendMessage", p, [=](Json::Value r)
		 {
			 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
			 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
			 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
			 
			 //																		setHelpSendTime(recvId);
//			 addChild(HatGacha::create([=](){}), 1);
			 if(r["result"]["code"].asInt() != GDSUCCESS)
				 return;
			 
//			 mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPFinishedChallenge());
//			 myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v)
//													 {
//														 
//													 });
			 GraphDogLib::JsonToLog("sendMessage", r);
			 
			 
//			 Json::Value p2;
//			 p2["receiver_id"] = mySGD->getAcceptHelpId();
//			 p2["message"] = "저에게 도움을 받으셨네요 축하합니다. 우편함으로 와서 도움결과를 받으세요!";
//			 hspConnector::get()->kSendMessage
//			 (p2, [=](Json::Value r)
//				{
//					GraphDogLib::JsonToLog("kSendMessage", r);
//				});
			 
		 }
		 );
		
		//		mySGD->getAcceptHelpId(), mySGD->getAcceptHelpNick();
		
	}
	else
	{
		KHAlertView* av = KHAlertView::create(); 
		av->setCloseOnPress(false);
		av->setTitleFileName("msg_help_result.png");
		av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
		av->setWidth(240);
		av->setHeight(240);
		av->setTitleHeight(10);
		av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
		av->setCenterY(150);

		CCNode* emptyNode = CCNode::create();
		auto ttf = CCLabelTTF::create("님에게 도움을 주지 못했습니다 OTL", mySGD->getFont().c_str(), 14.f);
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


		addChild(av, 1);
		av->show();
		////////////////////////////
		av->getContainerScrollView()->setTouchEnabled(false);

//		Json::Value p2;
//		p2["receiver_id"] = mySGD->getAcceptHelpId();
//		p2["message"] = "저에게 도움을 못 받으셨네요. 좀 잘하는 사람한테 도움요청하세요 ㅋㅋㅋ";
//		hspConnector::get()->kSendMessage
//		(p2, [=](Json::Value r)
//		 {
//			 GraphDogLib::JsonToLog("kSendMessage", r);
//		 });
	}
	
	return true;
}


