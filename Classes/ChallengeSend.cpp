#include "ChallengeSend.h"
#include "KSAlertView.h"
#include "jsoncpp/json.h"
#include "SilhouetteData.h"
#include "GDLib.h"
#include "hspConnector.h"

ChallengeSend::ChallengeSend()
{
	
}
ChallengeSend::~ChallengeSend()
{
	CCLog("ChallengeSend Destroy");
}
//void ChallengeSend::registerWithTouchDispatcher()
//{
//	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
//}

//bool ChallengeSend::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent)
//{
//	CCTouch* touch = pTouch;
//
//	CCPoint location(ccp(0, 0));
//	location = CCDirector::sharedDirector()->convertToGL(touch->locationInView());
//
//	return true;
//}


bool ChallengeSend::init(const std::string& user_id, const std::string& nickname, int score)
{
	CCLayer::init();
	{
		Json::Value p;
		Json::Value contentJson;
		
		contentJson["msg"] = (nickname + "님에게 도전!");
		contentJson["challengestage"] = mySD->getSilType();
		//										 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
		//				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
		p["content"] = GraphDogLib::JsonObjectToString(contentJson);
		std::string recvId = user_id;
		p["receiverMemberID"] = recvId;
		
		p["senderMemberID"] = hspConnector::get()->getKakaoID();
		p["type"] = kChallengeRequest;
		hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																 {
																	 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																	 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																	 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																	 
																	 //																		setHelpSendTime(recvId);
																	 GraphDogLib::JsonToLog("sendMessage", r);
																	 
																	 //																		obj->removeFromParent();
																	 KSAlertView* av = KSAlertView::create();
																	 av->setCenterY(150);
																	 auto ttf = CCLabelTTF::create((nickname + "님에게 도전을 하였습니다. 결과를 기다리세요!").c_str(), "", 12.f);
																	 av->setContentNode(
																											ttf
																											);
																	 av->setContentSize(ttf->getDimensions());
																	 
																	 //	av->setVScroll(CCScale9Sprite::create("popup_bar_v.png", CCRectMake(0, 0, 23, 53),
																	 //																				CCRectMake(7, 7, 23 - 7*2, 53 - 7*2 - 4)));
																	 //	av->setHScroll(CCScale9Sprite::create("popup_bar_h.png", CCRectMake(0, 0, 53, 23),
																	 //																				CCRectMake(10, 7, 53 - 10*2, 23 - 7*2)));
																	 //	auto m1 = CCMenuItemImageLambda::create("ui_common_ok.png", "ui_common_ok.png",
																	 //																					[](CCObject* e){
																	 //																						CCLog("press!!");
																	 //																					});
																	 //	av->addButton(m1);
																	 av->addButton(CCMenuItemImageLambda::create
																								 (
																									"ui_common_ok.png",
																									"ui_common_ok.png",
																									[=](CCObject* e){
//																										removeFromParent();
																									}
																									));
																	 addChild(av, 1);
																	 av->show();

																	 Json::Value p2;
																	 p2["receiver_id"] = recvId;
																	 p2["message"] = "도전을 신청한다!!";
																	 hspConnector::get()->kSendMessage
																	 (p2, [=](Json::Value r)
																		{
																			GraphDogLib::JsonToLog("kSendMessage", r);
																		});
																 });
	}
	{ // 자기한테 보내기.
		Json::Value p;
		Json::Value contentJson;
		contentJson["msg"] = ("도전한다!");
		contentJson["challengestage"] = mySD->getSilType();
		//										 KS::KSLog("%", hspConnector::get()->myKakaoInfo);
		//				 contentJson["nick"] = hspConnector::get()->myKakaoInfo["nickname"].asString();
		p["content"] = GraphDogLib::JsonObjectToString(contentJson);
		std::string recvId = user_id;
		//										 recvId.erase(std::remove(recvId.begin(), recvId.end(), '-'), recvId.end()); // '-' ¡¶∞≈
		p["receiverMemberID"] = hspConnector::get()->getKakaoID();
		p["senderMemberID"] = recvId;
		p["type"] = kChallengeRequest;
		hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																 {
																	 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																	 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																	 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																	 
																	 //																			setHelpSendTime(recvId);
																	 GraphDogLib::JsonToLog("sendMessage", r);
																	 
																	 //																		obj->removeFromParent();
																	 
																 });
	}
		
	
	return true;
}


