#include "ChallengeSend.h"
#include "KSAlertView.h"
#include "jsoncpp/json.h"
#include "SilhouetteData.h"
#include "GDLib.h"
#include "hspConnector.h"
#include "StarGoldData.h"
#include "HatGacha.h"


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


bool ChallengeSend::init(const std::string& user_id, const std::string& nickname, int score, ChallengeCategory cc)
{
	CCLayer::init();
	
	
	
	
	
	if(cc == ChallengeCategory::kRequest)
	{
		Json::Value p;
		Json::Value contentJson;
		contentJson["msg"] = (nickname + "님에게 도전!");
		contentJson["challengestage"] = mySD->getSilType();
		contentJson["score"] = score;
		contentJson["sender"] = hspConnector::get()->getKakaoID();
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
	else if(cc == ChallengeCategory::kRequestReply)
	{
		Json::Value p;
		Json::Value contentJson;
//		contentJson["msg"] = (nickname + "님에게 도전!");
		contentJson["challengestage"] = mySD->getSilType();
		contentJson["score"] = score;
		contentJson["sender"] = hspConnector::get()->getKakaoID();
		p["receiverMemberID"] = user_id;
		p["senderMemberID"] = hspConnector::get()->getKakaoID();
		p["type"] = kChallengeResult;
		
		if(mySGD->getAcceptChallengeScore() <= score)
		{
			// 이겼을 때,
			contentJson["result"] = "lose"; // 상대방을 lose 으로 세링~
			p["content"] = GraphDogLib::JsonObjectToString(contentJson);
			hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																	 {
																		 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																		 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																		 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																		 
																		 //																		setHelpSendTime(recvId);
																		 GraphDogLib::JsonToLog("sendMessage", r);
																		 
																		 //																		obj->removeFromParent();
																		 addChild(HatGacha::create([=](){
																			 CCLog("hat close");
																		 }), 1);
																		 
																		 Json::Value p2;
																		 p2["receiver_id"] = user_id;
																		 // 여기서 당신은 지금 배틀한 상대방을 지칭
																		 p2["message"] = "당신이 패배하였습니다.";
																		 hspConnector::get()->kSendMessage
																		 (p2, [=](Json::Value r)
																			{
																				GraphDogLib::JsonToLog("kSendMessage", r);
																			});
																	 });
		
		}
		else
		{
			// 졌당... ㅜㅜ
			contentJson["result"] = "win"; // 상대방을 win 으로 세링~
			p["content"] = GraphDogLib::JsonObjectToString(contentJson);
			hspConnector::get()->command("sendMessage", p, [=](Json::Value r)
																	 {
																		 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
																		 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
																		 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
																		 
																		 //																		setHelpSendTime(recvId);
																		 GraphDogLib::JsonToLog("sendMessage", r);
																		 //												 						obj->removeFromParent();
																		 KSAlertView* av = KSAlertView::create();
																		 av->setCenterY(150);
																		 auto ttf = CCLabelTTF::create((nickname + "에게 졌습니다...").c_str(), "", 12.f);
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
																		 p2["receiver_id"] = user_id;
																		 // 여기서 당신은 지금 배틀한 상대방을 지칭
																		 p2["message"] = "당신이 승리하였습니다. 보상을 받으세요 ^_^";
																		 hspConnector::get()->kSendMessage
																		 (p2, [=](Json::Value r)
																			{
																				GraphDogLib::JsonToLog("kSendMessage", r);
																			});
																	 });
		}
		
	}
	
	
	
	
	return true;
}
