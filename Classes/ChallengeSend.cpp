//#include "ChallengeSend.h"
//#include "KSAlertView.h"
//#include "jsoncpp/json.h"
//#include "SilhouetteData.h"
//#include "GDLib.h"
//#include "hspConnector.h"
//#include "StarGoldData.h"
//#include "HatGacha.h"
//#include "SendMessageUtil.h"
//#include "GachaPurchase.h"
//#include "KHAlertView.h"
//#include "CommonButton.h"
//ChallengeSend::ChallengeSend()
//{
//	
//}
//ChallengeSend::~ChallengeSend()
//{
//	CCLog("ChallengeSend Destroy");
//}
////void ChallengeSend::registerWithTouchDispatcher()
////{
////	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
////}
//
////bool ChallengeSend::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
////{
////	CCTouch* touch = pTouch;
////
////	CCPoint location(ccp(0, 0));
////	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
////
////	return true;
////}
//
//
//bool ChallengeSend::init(const std::string& user_id, const std::string& nickname, int score, ChallengeCategory cc, std::function<void(void)> endFunction)
//{
//	CCLayer::init();
//	
//	
//	
//	
//	
//	if(cc == ChallengeCategory::kRequest)
//	{
//		Json::Value p;
//		Json::Value contentJson;
//		contentJson["msg"] = (nickname + "님에게 도전!");
//		contentJson["challengestage"] = mySD->getSilType();
//		contentJson["score"] = score;
//		contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//		p["content"] = GraphDogLib::JsonObjectToString(contentJson);
//		std::string recvId = user_id;
//		p["receiverMemberID"] = recvId;
//		
//		p["senderMemberID"] = hspConnector::get()->getSocialID();
//		
//		p["type"] = kChallengeRequest;
//		hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
//																 {
//																	 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//																	 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//																	 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//																	 
//																	 //																		setHelpSendTime(recvId);
//																	 if(r["result"]["code"].asInt() != GDSUCCESS)
//																	 {
//																		 // 에러.
//																		 endFunction();
//																		 return;
//																	 }
//																	 
//																	 setChallengeSendTime(user_id);
////																	 friend_list.erase(friend_list.begin() + tag);
//																	 GraphDogLib::JsonToLog("sendMessage", r);
//																	 
//																	 //																		obj->removeFromParent();
//																	
//																	 KHAlertView* av = KHAlertView::create(); 
//																	 // av->setTitleFileName("msg_challenge.png");
//																	 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
//																	 av->setWidth(240);
//																	 av->setHeight(240);
//																	 av->setTitleHeight(10);
//																	 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
//																	 av->setCenterY(150);
//
//																	 CCNode* emptyNode = CCNode::create();
//																	 auto ttf = CCLabelTTF::create((nickname + "님에게 도전을 하였습니다. 결과를 기다리세요!").c_str(), mySGD->getFont().c_str(), 12.f); 
//																	 ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
//																	 //	con->setAnchorPoint(ccp(0, 0));
//																	 //ttf->setAnchorPoint(ccp(0.5f, 0.5f));
//																	 ttf->setColor(ccc3(255, 255, 255));
//																	 ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, ttf->getPositionY() - 15));
//																	 emptyNode->addChild(ttf);
//																	 av->setContentNode(
//																			 emptyNode
//																			 );
//																	 av->setContentSize(ttf->getDimensions());
//																	 av->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
//																								 [=](CCObject* e) {
//																									 CCLog("ok!!");
//																									 endFunction();
//																								 });
//
//																	 addChild(av, 1);
//																	 av->show();
//
//																	 ////////////////////////////////////
//																 });
//	}
//	else if(cc == ChallengeCategory::kRequestReply)
//	{
//		Json::Value p;
//		Json::Value contentJson;
////		contentJson["msg"] = (nickname + "님에게 도전!");
//		contentJson["challengestage"] = mySD->getSilType();
//		contentJson["score"] = score;
//		contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
//		p["receiverMemberID"] = user_id;
//		p["senderMemberID"] = hspConnector::get()->getSocialID();
//		p["type"] = kChallengeResult;
//		if(mySGD->getAcceptChallengeScore() <= score)
//		{
//			// 이겼을 때,
//			contentJson["result"] = "lose"; // 상대방을 lose 으로 세링~
//			p["content"] = GraphDogLib::JsonObjectToString(contentJson);
//			hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
//																	 {
//																		 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//																		 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//																		 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//																		 //																		setHelpSendTime(recvId);
//																		 
//																		 if(r["result"]["code"].asInt() != GDSUCCESS)
//																		 {
//																			 endFunction();
//																			 return;
//																		 }
//																		 
//																		 GraphDogLib::JsonToLog("sendMessage", r);
//																		 mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPFinishedChallenge());
//																		 myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v)
//																												 {
//																													 
//																												 });
//																		 
//																		 //																		obj->removeFromParent();
//																		 addChild(GachaPurchase::create(kGachaPurchaseStartMode_reward,
//																																		[=](){
//																																			CCLog("hat close");
//																																			endFunction();
//																																		}
//																																		), 1);
//
//																		 
////																		 Json::Value p2;
////																		 p2["receiver_id"] = user_id;
////																		 // 여기서 당신은 지금 배틀한 상대방을 지칭
////																		 p2["message"] = "당신이 패배하였습니다.";
////																		 hspConnector::get()->kSendMessage
////																		 (p2, [=](Json::Value r)
////																			{
////																				GraphDogLib::JsonToLog("kSendMessage", r);
////																			});
//																	 });
//		
//		}
//		else
//		{
//			// 졌당... ㅜㅜ
//			contentJson["result"] = "win"; // 상대방을 win 으로 세링~
//			p["content"] = GraphDogLib::JsonObjectToString(contentJson);
//			hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
//																	 {
//																		 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
//																		 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
//																		 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
//																		 
//																		 //																		setHelpSendTime(recvId);
//																		 GraphDogLib::JsonToLog("sendMessage", r);
//																		 if(r["result"]["code"].asInt() != GDSUCCESS)
//																		 {
//																			 endFunction();
//																			 return;
//																		 }
//																		 
//																		 mySGD->setFriendPoint(mySGD->getFriendPoint() + mySGD->getSPFinishedChallenge());
//																		 myDSH->saveUserData({kSaveUserData_Key_friendPoint}, [=](Json::Value v)
//																												 {
//																													 
//																												 });
//																		 //												 						obj->removeFromParent();
//																		 KHAlertView* av = KHAlertView::create(); 
//																		 // av->setTitleFileName("msg_challenge.png");
//																		 av->setBack9(CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6)));
//																		 av->setWidth(240);
//																		 av->setHeight(240);
//																		 av->setTitleHeight(10);
//																		 av->setContentBorder(CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6)));
//																		 av->setCenterY(150);
//
//																		 CCNode* emptyNode = CCNode::create();
//																		 auto ttf = CCLabelTTF::create((nickname + "에게 졌습니다...").c_str(), mySGD->getFont().c_str(), 12.f); 
//																		 ttf->setHorizontalAlignment(kCCTextAlignmentCenter);
//																		 //	con->setAnchorPoint(ccp(0, 0));
//																		 //ttf->setAnchorPoint(ccp(0.5f, 0.5f));
//																		 ttf->setColor(ccc3(255, 255, 255));
//																		 ttf->setPosition(ccp(av->getContentRect().size.width / 2.f, ttf->getPositionY() - 15));
//																		 emptyNode->addChild(ttf);
//																		 av->setContentNode(
//																				 emptyNode
//																				 );
//																		 av->setContentSize(ttf->getDimensions());
//																		 av->addButton(CommonButton::create("ok", 14.f, CCSizeMake(90, 54), CommonButtonType::CommonButtonBlue, INT_MIN),
//																									 [=](CCObject* e) {
//																										 CCLog("ok!!");
//																										 endFunction();
//																									 });
//
//																		 addChild(av, 1);
//																		 av->show();
//
//
//																		 Json::Value p2;
//																		 p2["receiver_id"] = user_id;
//																		 // 여기서 당신은 지금 배틀한 상대방을 지칭
//																		 p2["message"] = "당신이 승리하였습니다. 보상을 받으세요 ^_^";
////																		 hspConnector::get()->kSendMessage
////																		 (p2, [=](Json::Value r)
////																			{
////																				GraphDogLib::JsonToLog("kSendMessage", r);
////																			});
//																	 });
//		}
//		
//	}
//	
//	
//	
//	
//	return true;
//}
