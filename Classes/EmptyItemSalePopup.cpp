//
//  EmptyItemSalePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 29..
//
//

#include "EmptyItemSalePopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "DataStorageHub.h"
#include "ASPopupView.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "PuzzleScene.h"
#include "FormSetter.h"

EmptyItemSalePopup* EmptyItemSalePopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_purchase_func, PurchaseGuideType t_type)
{
	EmptyItemSalePopup* t_mup = new EmptyItemSalePopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_purchase_func, t_type);
	t_mup->autorelease();
	return t_mup;
}

void EmptyItemSalePopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_purchase_func, PurchaseGuideType t_type)
{
	m_type = t_type;
	
	mySGD->showPurchasePopup(m_type);
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	purchase_func = t_purchase_func;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(320,280));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	if(m_type == kPurchaseGuideType_emptyItem)
	{
		KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_emptyItemSaleTitle), mySGD->getFont().c_str(), 21);
		title_label->setColor(ccc3(50, 250, 255));
		title_label->setPosition(ccp(0,95));
		m_container->addChild(title_label);
	}
	else if(m_type == kPurchaseGuideType_stupidNpuHelp)
	{
		KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_itemPackageChance), mySGD->getFont().c_str(), 21);
		title_label->setColor(ccc3(50, 250, 255));
		title_label->setPosition(ccp(0,95));
		m_container->addChild(title_label);
	}
	
	string ment1_str;
	if(m_type == kPurchaseGuideType_emptyItem)
		ment1_str = myLoc->getLocalForKey(kMyLocalKey_emptyItemSaleMent1);
	else if(m_type == kPurchaseGuideType_stupidNpuHelp)
		ment1_str = myLoc->getLocalForKey(kMyLocalKey_stupidNpuHelpMent1);
	
	KSLabelTTF* sub_ment1 = KSLabelTTF::create(ment1_str.c_str(), mySGD->getFont().c_str(), 12);
	sub_ment1->setPosition(ccp(0,60));
	m_container->addChild(sub_ment1);
	
	string ment2_str;
	if(m_type == kPurchaseGuideType_emptyItem)
		ment2_str = myLoc->getLocalForKey(kMyLocalKey_emptyItemSaleMent2);
	else if(m_type == kPurchaseGuideType_stupidNpuHelp)
		ment2_str = myLoc->getLocalForKey(kMyLocalKey_stupidNpuHelpMent2);
	
	string sale_percent_string = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_sale_s, m_type-1);
	
	KSLabelTTF* sub_ment2 = KSLabelTTF::create(CCString::createWithFormat(ment2_str.c_str(), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 12);
	sub_ment2->setColor(ccYELLOW);
	sub_ment2->setPosition(ccp(0,43));
	m_container->addChild(sub_ment2);
	
	
	CCSprite* item9 = CCSprite::create("item9.png");
	item9->setPosition(ccp(-70,0));
	m_container->addChild(item9);
	
	KSLabelTTF* plus1 = KSLabelTTF::create("+", mySGD->getFont().c_str(), 20);
	plus1->setPosition(ccp(-35,0));
	m_container->addChild(plus1);
	
	CCSprite* item6 = CCSprite::create("item6.png");
	item6->setPosition(ccp(0,0));
	m_container->addChild(item6);
	
	KSLabelTTF* plus2 = KSLabelTTF::create("+", mySGD->getFont().c_str(), 20);
	plus2->setPosition(ccp(35,0));
	m_container->addChild(plus2);
	
	CCSprite* item8 = CCSprite::create("item8.png");
	item8->setPosition(ccp(70,0));
	m_container->addChild(item8);
	
	
	string before_value = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_data_s, m_type-1);
	Json::Reader reader;
	Json::Value before_data;
	reader.parse(before_value, before_data);
	
	KSLabelTTF* before_price = KSLabelTTF::create(KS::insert_separator(before_data["beforeDiscount"].asString()).c_str(), mySGD->getFont().c_str(), 14);
	before_price->enableOuterStroke(ccBLACK, 1.f);
	before_price->setPosition(ccp(-45,-30));
	m_container->addChild(before_price);
	
	
	CCSprite* sale_arrow = CCSprite::create("sale_arrow.png");
	sale_arrow->setPosition(ccp(-45,-30));
	m_container->addChild(sale_arrow);
	
	
	CCSprite* after_price_type = CCSprite::create("common_button_gold.png");
	after_price_type->setPosition(ccp(10,-30));
	m_container->addChild(after_price_type);
	
	KSLabelTTF* after_price = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_price_i, m_type-1))->getCString()).c_str(), mySGD->getFont().c_str(), 20);
	after_price->setColor(ccYELLOW);
	after_price->enableOuterStroke(ccBLACK, 1.f);
	after_price->setPosition(ccp(50,-30));
	m_container->addChild(after_price);
	
	
	CCScale9Sprite* stamp_case = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	stamp_case->setContentSize(CCSizeMake(75, 45));
	stamp_case->setPosition(ccp(90,30));
	stamp_case->setRotation(-15);
	m_container->addChild(stamp_case);
	
	
	KSLabelTTF* package_sale_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_packageItemSale), mySGD->getFont().c_str(), 10);
	package_sale_label->setPosition(ccp(37.5f,33));
	stamp_case->addChild(package_sale_label);
	
	KSLabelTTF* sale_value_label = KSLabelTTF::create(CCString::createWithFormat("-%s", sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 17);
	sale_value_label->setColor(ccYELLOW);
	sale_value_label->setPosition(ccp(37.5f,15));
	stamp_case->addChild(sale_value_label);
	
	
	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 13);
	CCScale9Sprite* giveup_back = CCScale9Sprite::create("subpop_gray.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	giveup_button = CCControlButton::create(giveup_label, giveup_back);
	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(EmptyItemSalePopup::giveupAction), CCControlEventTouchUpInside);
	giveup_button->setPreferredSize(CCSizeMake(90,60));
	giveup_button->setPosition(ccp(-55,-85));
	m_container->addChild(giveup_button);
	
	giveup_button->setTouchPriority(touch_priority);
	
	
	
	CCLabelTTF* p_label = CCLabelTTF::create();
	
	KSLabelTTF* purchase_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_purchase), mySGD->getFont().c_str(), 13);
	purchase_label->setPosition(ccp(0,10));
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("subpop_darkred.png", CCRectMake(0,0,30,30), CCRectMake(14,14,2,2));
	price_back->setContentSize(CCSizeMake(70, 30));
	price_back->setPosition(ccp(purchase_label->getContentSize().width/2.f, purchase_label->getContentSize().height/2.f-20));
	purchase_label->addChild(price_back);
	
	CCSprite* price_type = CCSprite::create("common_button_gold.png");
	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
	price_type->setScale(0.7f);
	price_back->addChild(price_type);
	CCLabelTTF* price_label = CCLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_price_i, m_type-1))->getCString()).c_str(), mySGD->getFont().c_str(), 12);
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f+10,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	p_label->addChild(purchase_label);
	
	
	CCScale9Sprite* purchase_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	purchase_button = CCControlButton::create(p_label, purchase_back);
	purchase_button->addTargetWithActionForControlEvents(this, cccontrol_selector(EmptyItemSalePopup::purchaseAction), CCControlEventTouchUpInside);
	purchase_button->setPreferredSize(CCSizeMake(90,60));
	purchase_button->setPosition(ccp(55,-85));
	m_container->addChild(purchase_button);
	
	purchase_button->setTouchPriority(touch_priority);
	
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t)
										 {
											 KS::setOpacity(m_container, 255);
											 is_menu_enable = true;
										 }));
}

void EmptyItemSalePopup::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}

void EmptyItemSalePopup::purchaseAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	inapp_loading = LoadingLayer::create(-9999, true);
	addChild(inapp_loading);
	
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	mySGD->addChangeGoods(kGoodsType_gold, -NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_price_i, m_type-1), "아이템없을때패키지판매팝업(IOS-인앱결재)");
	mySGD->addChangeGoods(kGoodsType_item9, NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_count_i, m_type-1), "아이템없을때패키지판매팝업");
	mySGD->addChangeGoods(kGoodsType_item6, NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_count_i, m_type-1), "아이템없을때패키지판매팝업");
	mySGD->addChangeGoods(kGoodsType_item8, NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_count_i, m_type-1), "아이템없을때패키지판매팝업");
	
	mySGD->changeGoods([=](Json::Value result_data){
		inapp_loading->removeFromParent();
		if(result_data["result"]["code"].asInt() == GDSUCCESS)
		{
			is_menu_enable = true;
			giveupAction(sender, t_event);
		}
		else
		{
			mySGD->clearChangeGoods();
			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
			is_menu_enable = true;
		}
	});
	
//	
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	Json::Value param;
//	param["productid"] = mySGD->getInappProduct(0); //
//	hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
//		//																				KS::KSLog("in-app test \n%", v);
//		if(v["issuccess"].asInt())
//		{
//			//			mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getRankUpRubyFee(), "승급");
//			requestItemDelivery();
//		}
//		else
//		{
//			inapp_loading->removeFromParent();
//			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
//			is_menu_enable = true;
//		}
//	});
//#endif
	
}

//void EmptyItemSalePopup::requestItemDelivery()
//{
//	vector<CommandParam> command_list;
//	
//	Json::Value transaction_param;
//	transaction_param["memberID"] = hspConnector::get()->getMemberID();
//	command_list.push_back(CommandParam("starttransaction", transaction_param, [=](Json::Value result_data)
//										{
//											if(result_data["result"]["code"].asInt() == GDSUCCESS)
//											{
//												inapp_loading->removeFromParent();
//												
//												is_menu_enable = true;
//												giveupAction(NULL, CCControlEventTouchUpInside);
//											}
//											else
//											{
//												addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
//											}
//										}));
//	
//	Json::Value request_param;
//	request_param["memberID"] = hspConnector::get()->getSocialID();
//	command_list.push_back(CommandParam("requestItemDelivery", request_param, nullptr));
//	
//	command_list.push_back(mySGD->getChangeGoodsParam(json_selector(mySGD, StarGoldData::saveChangeGoodsTransaction)));
//	
//	hspConnector::get()->command(command_list);
//}

//void EmptyItemSalePopup::resultSaveUserData(Json::Value result_data)
//{
//	CCLOG("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
//	if(result_data["result"]["code"].asInt() == GDSUCCESS)
//	{
//		CCLOG("save userdata success!!");
//		
//		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
//		
//		//		rankup_button->setVisible(false);
//		//		giveup_button->setVisible(false);
//		//
//		//		question_manager->runAnimationsForSequenceNamed("Default Timeline");
//	}
//	else
//	{
//		CCLOG("missile upgrade fail!!");
//		
//		mySGD->clearChangeGoods();
//		addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
//		
//		is_menu_enable = true;
//	}
//}