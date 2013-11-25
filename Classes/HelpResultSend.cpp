#include "HelpResultSend.h"
#include "KSAlertView.h"
#include "CCMenuLambda.h"
#include "StarGoldData.h"
#include "hspConnector.h"
HelpResultSend::HelpResultSend()
{
	
}
HelpResultSend::~HelpResultSend()
{
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

bool HelpResultSend::init(const std::string& corp_id, bool isSuccess)
{
	CCLayer::init();
	if(isSuccess)
	{
		
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
		contentJson["sender"] = hspConnector::get()->getKakaoID();
		p["receiverMemberID"] = corp_id;
		p["senderMemberID"] = hspConnector::get()->getKakaoID();
		p["type"] = kHelpResult;
		p["content"] = GraphDogLib::JsonObjectToString(contentJson);
		hspConnector::get()->command
		("sendMessage", p, [=](Json::Value r)
		 {
			 //		NSString* receiverID =  [NSString stringWithUTF8String:param["receiver_id"].asString().c_str()];
			 //		NSString* message =  [NSString stringWithUTF8String:param["message"].asString().c_str()];
			 //		NSString* executeURLString = [NSString stringWithUTF8String:param["executeurl"].asString().c_str()];
			 
			 //																		setHelpSendTime(recvId);
			 GraphDogLib::JsonToLog("sendMessage", r);
			 KSAlertView* av = KSAlertView::create();
			 av->setCenterY(150);
			 auto ttf = CCLabelTTF::create((mySGD->getAcceptHelpNick() + "님에게 도움을 주었습니다. 모자돌리기!").c_str(), "", 12.f);
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
			 p2["receiver_id"] = mySGD->getAcceptHelpId();
			 p2["message"] = "저에게 도움을 받으셨네요 축하합니다. 우편함으로 와서 도움결과를 받으세요!";
			 hspConnector::get()->kSendMessage
			 (p2, [=](Json::Value r)
				{
					GraphDogLib::JsonToLog("kSendMessage", r);
				});
			 
		 }
		 );
		
		//		mySGD->getAcceptHelpId(), mySGD->getAcceptHelpNick();
		
	}
	else
	{
		KSAlertView* av = KSAlertView::create();
		av->setCenterY(150);
		auto ttf = CCLabelTTF::create((mySGD->getAcceptHelpNick() + "님에게 도움을 주지 못했습니다 OTL").c_str(), "", 12.f);
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
		p2["receiver_id"] = mySGD->getAcceptHelpId();
		p2["message"] = "저에게 도움을 못 받으셨네요. 좀 잘하는 사람한테 도움요청하세요 ㅋㅋㅋ";
		hspConnector::get()->kSendMessage
		(p2, [=](Json::Value r)
		 {
			 GraphDogLib::JsonToLog("kSendMessage", r);
		 });
	}
	
	return true;
}


