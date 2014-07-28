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
#include "CommonAnimation.h"

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
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCSprite::create("popup_large_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251,128));
	back_in->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(0,15));
	back_case->addChild(back_in);
	
	
	if(m_type == kPurchaseGuideType_emptyItem)
	{
		KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_emptyItemSaleTitle), mySGD->getFont().c_str(), 12);
		title_label->disableOuterStroke();
		title_label->setAnchorPoint(ccp(0.5f,0.5f));
		title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
		m_container->addChild(title_label);
	}
	else if(m_type == kPurchaseGuideType_stupidNpuHelp)
	{
		KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_itemPackageChance), mySGD->getFont().c_str(), 12);
		title_label->disableOuterStroke();
		title_label->setAnchorPoint(ccp(0.5f,0.5f));
		title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
		m_container->addChild(title_label);
	}
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-22, back_case->getContentSize().height-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  giveupAction(NULL, CCControlEventTouchUpInside);
							  });
	back_case->addChild(close_button);
	
	string ment1_str;
	if(m_type == kPurchaseGuideType_emptyItem)
		ment1_str = myLoc->getLocalForKey(kMyLocalKey_emptyItemSaleMent1);
	else if(m_type == kPurchaseGuideType_stupidNpuHelp)
		ment1_str = myLoc->getLocalForKey(kMyLocalKey_stupidNpuHelpMent1);
	
	KSLabelTTF* sub_ment1 = KSLabelTTF::create(ment1_str.c_str(), mySGD->getFont().c_str(), 12);
	sub_ment1->disableOuterStroke();
	sub_ment1->setPosition(ccp(0,-20));
	m_container->addChild(sub_ment1);
	
	string ment2_str;
	if(m_type == kPurchaseGuideType_emptyItem)
		ment2_str = myLoc->getLocalForKey(kMyLocalKey_emptyItemSaleMent2);
	else if(m_type == kPurchaseGuideType_stupidNpuHelp)
		ment2_str = myLoc->getLocalForKey(kMyLocalKey_stupidNpuHelpMent2);
	
	string sale_percent_string = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_sale_s, m_type-1);
	
	KSLabelTTF* sub_ment2 = KSLabelTTF::create(CCString::createWithFormat(ment2_str.c_str(), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 12);
	sub_ment2->setColor(ccc3(255, 170, 20));
	sub_ment2->disableOuterStroke();
	sub_ment2->setPosition(ccp(0,-37));
	m_container->addChild(sub_ment2);
	
	
	CCSprite* item9 = CCSprite::create("item9.png");
	item9->setPosition(ccp(-70,50));
	m_container->addChild(item9);
	
	KSLabelTTF* plus1 = KSLabelTTF::create("+", mySGD->getFont().c_str(), 20);
	plus1->setPosition(ccp(-35,50));
	m_container->addChild(plus1);
	
	CCSprite* item6 = CCSprite::create("item6.png");
	item6->setPosition(ccp(0,50));
	m_container->addChild(item6);
	
	KSLabelTTF* plus2 = KSLabelTTF::create("+", mySGD->getFont().c_str(), 20);
	plus2->setPosition(ccp(35,50));
	m_container->addChild(plus2);
	
	CCSprite* item11 = CCSprite::create("item11.png");
	item11->setPosition(ccp(70,50));
	m_container->addChild(item11);
	
	CCSprite* before_price_type = CCSprite::create("price_gold_img.png");
	before_price_type->setPosition(ccp(-70,10));
	before_price_type->setScale(0.7f);
	m_container->addChild(before_price_type);
	
	string before_value = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_data_s, m_type-1);
	Json::Reader reader;
	Json::Value before_data;
	reader.parse(before_value, before_data);
	
	KSLabelTTF* before_price = KSLabelTTF::create(KS::insert_separator(before_data["beforeDiscount"].asString()).c_str(), mySGD->getFont().c_str(), 14);
	before_price->enableOuterStroke(ccBLACK, 0.5f);
	before_price->setPosition(ccp(-45,10));
	m_container->addChild(before_price);
	
	
	CCSprite* sale_arrow = CCSprite::create("sale_arrow.png");
	sale_arrow->setPosition(ccp(-45,10));
	m_container->addChild(sale_arrow);
	
	
	CCSprite* after_price_type = CCSprite::create("price_gold_img.png");
	after_price_type->setPosition(ccp(10,10));
	m_container->addChild(after_price_type);
	
	KSLabelTTF* after_price = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, m_type-1).c_str(), mySGD->getFont().c_str(), 20);
	after_price->setColor(ccc3(255, 170, 20));
	after_price->enableOuterStroke(ccBLACK, 0.5f);
	after_price->setPosition(ccp(50,10));
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
	
	
//	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 15);
//	giveup_label->setColor(ccBLACK);
//	CCScale9Sprite* giveup_back = CCScale9Sprite::create("achievement_cellback_normal.png", CCRectMake(0,0,47,47), CCRectMake(23, 23, 1, 1));
//	giveup_button = CCControlButton::create(giveup_label, giveup_back);
//	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(EmptyItemSalePopup::giveupAction), CCControlEventTouchUpInside);
//	giveup_button->setPreferredSize(CCSizeMake(80,40));
//	giveup_button->setPosition(ccp(-87,-85));
//	m_container->addChild(giveup_button);
//	
//	giveup_button->setTouchPriority(touch_priority);
	
	
	
	CCLabelTTF* p_label = CCLabelTTF::create();
	
	KSLabelTTF* purchase_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_purchase), mySGD->getFont().c_str(), 15);
	purchase_label->disableOuterStroke();
	purchase_label->setAnchorPoint(ccp(0,0.5f));
	purchase_label->setPosition(ccp(0,0));
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("gray_ellipse.png", CCRectMake(0,0,82,26), CCRectMake(40,12,2,2));
	price_back->setContentSize(CCSizeMake(82, 26));
	price_back->setPosition(ccp(purchase_label->getContentSize().width + price_back->getContentSize().width/2.f + 5, purchase_label->getContentSize().height/2.f));
	purchase_label->addChild(price_back);
	
	CCSprite* price_type = CCSprite::create("price_gold_img.png");
	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
//	price_type->setScale(0.7f);
	price_back->addChild(price_type);
	CCLabelTTF* price_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, m_type-1).c_str(), mySGD->getFont().c_str(), 15);
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f+7,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	p_label->addChild(purchase_label);
	
	
	CCScale9Sprite* purchase_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	purchase_button = CCControlButton::create(p_label, purchase_back);
	purchase_button->addTargetWithActionForControlEvents(this, cccontrol_selector(EmptyItemSalePopup::purchaseAction), CCControlEventTouchUpInside);
	purchase_button->setPreferredSize(CCSizeMake(180,48));
	purchase_button->setPosition(ccp(0,-83));
	m_container->addChild(purchase_button);
	
	purchase_label->setPositionX(-(purchase_label->getContentSize().width + 5 + price_back->getContentSize().width)/2.f);
	
	purchase_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void EmptyItemSalePopup::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
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
	
	mySGD->addChangeGoods("pg_ei");
	
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
			addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-100, kGoodsType_gold, [=]()
																{
																	((PuzzleScene*)getParent()->getParent())->showShopPopup(kSC_gold);
																}), 9999);
//			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
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