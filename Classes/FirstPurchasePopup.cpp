//
//  FirstPurchasePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 29..
//
//

#include "FirstPurchasePopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "ASPopupView.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "AudioEngine.h"
#include "PuzzleScene.h"
#include "FormSetter.h"

FirstPurchasePopup* FirstPurchasePopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_purchase_func)
{
	FirstPurchasePopup* t_mup = new FirstPurchasePopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_purchase_func);
	t_mup->autorelease();
	return t_mup;
}

void FirstPurchasePopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_purchase_func)
{
	mySGD->showPurchasePopup(kPurchaseGuideType_firstPurchase);
	
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
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_firstPurchaseSaleTitle), mySGD->getFont().c_str(), 21);
	title_label->setColor(ccc3(50, 250, 255));
	title_label->setPosition(ccp(0,95));
	m_container->addChild(title_label);
	
	
	CCSprite* ruby_img = CCSprite::create("shop_ruby6.png");
	ruby_img->setPosition(ccp(-70,20));
	m_container->addChild(ruby_img);
	
	
	CCSprite* ruby100_img = CCSprite::create("firstpurchase_ruby100.png");
	ruby100_img->setPosition(ccp(-25,45));
	m_container->addChild(ruby100_img);
	
	
	string before_value = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_data_s, kPurchaseGuideType_firstPurchase-1);
	Json::Reader reader;
	Json::Value before_data;
	reader.parse(before_value, before_data);
	
	KSLabelTTF* before_price = KSLabelTTF::create(KS::insert_separator(before_data["beforeDiscount"].asString()).c_str(), mySGD->getFont().c_str(), 14);
	before_price->enableOuterStroke(ccBLACK, 1.f);
	before_price->setPosition(ccp(-70,20));
	m_container->addChild(before_price);
	
	
	CCSprite* sale_arrow = CCSprite::create("sale_arrow.png");
	sale_arrow->setPosition(ccp(-70,20));
	m_container->addChild(sale_arrow);
	
	
	KSLabelTTF* after_price = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_price_i, kPurchaseGuideType_firstPurchase-1))->getCString()).c_str(), mySGD->getFont().c_str(), 20);
	after_price->setColor(ccYELLOW);
	after_price->enableOuterStroke(ccBLACK, 1.f);
	after_price->setPosition(ccp(-5,20));
	m_container->addChild(after_price);
	
	
	CCScale9Sprite* stamp_case = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	stamp_case->setContentSize(CCSizeMake(70, 45));
	stamp_case->setPosition(ccp(75,40));
	stamp_case->setRotation(-15);
	m_container->addChild(stamp_case);
	
	
	KSLabelTTF* first_sale_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_firstPurchaseSale), mySGD->getFont().c_str(), 10);
	first_sale_label->setPosition(ccp(35,33));
	stamp_case->addChild(first_sale_label);
	
	string sale_percent_string = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_sale_s, kPurchaseGuideType_firstPurchase-1);
	
	KSLabelTTF* sale_value_label = KSLabelTTF::create(CCString::createWithFormat("-%s", sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 17);
	sale_value_label->setColor(ccYELLOW);
	sale_value_label->setPosition(ccp(35,15));
	stamp_case->addChild(sale_value_label);
	
	
	KSLabelTTF* sub_ment1 = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_firstPurchaseMent1), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 12);
	sub_ment1->setColor(ccYELLOW);
	sub_ment1->setAnchorPoint(ccp(1.f,0.5f));
	sub_ment1->setPositionY(-18);
	m_container->addChild(sub_ment1);
	
	
	KSLabelTTF* sub_ment2 = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_firstPurchaseMent2), mySGD->getFont().c_str(), 12);
	sub_ment2->setAnchorPoint(ccp(0.f, 0.5f));
	sub_ment2->setPositionY(-18);
	m_container->addChild(sub_ment2);
	
	
	float w1 = sub_ment1->getContentSize().width;
	float w2 = sub_ment2->getContentSize().width;
	
	sub_ment1->setPositionX(w1 - (w1+w2)/2.f);
	sub_ment2->setPositionX(w1 - (w1+w2)/2.f);
	
	KSLabelTTF* sub_ment3 = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_firstPurchaseMent3), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 12);
	sub_ment3->setPosition(ccp(0,-35));
	m_container->addChild(sub_ment3);
	
	
	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 13);
	CCScale9Sprite* giveup_back = CCScale9Sprite::create("subpop_gray.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	giveup_button = CCControlButton::create(giveup_label, giveup_back);
	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(FirstPurchasePopup::giveupAction), CCControlEventTouchUpInside);
	giveup_button->setPreferredSize(CCSizeMake(90,60));
	giveup_button->setPosition(ccp(-55,-85));
	m_container->addChild(giveup_button);
	
	giveup_button->setTouchPriority(touch_priority);
	
	
	
	CCLabelTTF* p_label = CCLabelTTF::create();
	
	KSLabelTTF* purchase_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_purchase), mySGD->getFont().c_str(), 13);
	purchase_label->setPosition(ccp(0,10));
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("subpop_darkred.png", CCRectMake(0,0,30,30), CCRectMake(14,14,2,2));
	price_back->setContentSize(CCSizeMake(60, 30));
	price_back->setPosition(ccp(purchase_label->getContentSize().width/2.f, purchase_label->getContentSize().height/2.f-20));
	purchase_label->addChild(price_back);
	
//	CCSprite* price_type = CCSprite::create("price_ruby_img.png");
//	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-15,price_back->getContentSize().height/2.f));
//	price_back->addChild(price_type);
	CCLabelTTF* price_label = CCLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_price_i, kPurchaseGuideType_firstPurchase-1))->getCString()).c_str(), mySGD->getFont().c_str(), 12);
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	p_label->addChild(purchase_label);
	
	
	CCScale9Sprite* purchase_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	purchase_button = CCControlButton::create(p_label, purchase_back);
	purchase_button->addTargetWithActionForControlEvents(this, cccontrol_selector(FirstPurchasePopup::purchaseAction), CCControlEventTouchUpInside);
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

void FirstPurchasePopup::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}

void FirstPurchasePopup::purchaseAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	inapp_loading = LoadingLayer::create(-9999, true);
	addChild(inapp_loading);
	
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	mySGD->addChangeGoods(kGoodsType_ruby, NSDS_GI(kSDS_GI_shopPurchaseGuide_int1_count_i, kPurchaseGuideType_firstPurchase-1), "첫구매팝업(IOS-인앱결재)", "", "", true);
	
	mySGD->setUserdataIsFirstBuy(1); // true
	vector<CommandParam> command_list;
	command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	mySGD->changeGoodsTransaction(command_list, [=](Json::Value result_data){
		inapp_loading->removeFromParent();
		if(result_data["result"]["code"].asInt() == GDSUCCESS)
		{
			is_menu_enable = true;
			giveupAction(sender, t_event);
		}
		else
		{
			mySGD->clearChangeGoods();
			mySGD->clearChangeUserdata();
			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
			is_menu_enable = true;
		}
	});
	
	
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	Json::Value param;
	param["productid"] = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_pID_s, kPurchaseGuideType_firstPurchase-1); //
	hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
		//																				KS::KSLog("in-app test \n%", v);
		if(v["issuccess"].asInt())
		{
//			mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getRankUpRubyFee(), "승급");
			mySGD->setUserdataIsFirstBuy(1); // true
			requestItemDelivery();
		}
		else
		{
			inapp_loading->removeFromParent();
			mySGD->clearChangeUserdata();
			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
			is_menu_enable = true;
		}
	});
#endif
	
}

void FirstPurchasePopup::requestItemDelivery()
{
	vector<CommandParam> command_list;
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getMemberID();
	command_list.push_back(CommandParam("starttransaction", transaction_param, [=](Json::Value result_data)
										{
											if(result_data["result"]["code"].asInt() == GDSUCCESS)
											{
												inapp_loading->removeFromParent();
												
												is_menu_enable = true;
												giveupAction(NULL, CCControlEventTouchUpInside);
											}
											else
											{
												addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
											}
										}));
	
	Json::Value request_param;
	request_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("requestItemDelivery", request_param, nullptr));
	if(mySGD->isChangedGoods())
		command_list.push_back(mySGD->getChangeGoodsParam(json_selector(mySGD, StarGoldData::saveChangeGoodsTransaction)));
	command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	hspConnector::get()->command(command_list);
}

void FirstPurchasePopup::resultSaveUserData(Json::Value result_data)
{
	CCLOG("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("save userdata success!!");
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
//		rankup_button->setVisible(false);
//		giveup_button->setVisible(false);
//		
//		question_manager->runAnimationsForSequenceNamed("Default Timeline");
	}
	else
	{
		CCLOG("missile upgrade fail!!");
		
		mySGD->clearChangeGoods();
		addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
		
		is_menu_enable = true;
	}
}