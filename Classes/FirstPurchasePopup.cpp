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
#include "CommonAnimation.h"
#include "StyledLabelTTF.h"
#include "FiveRocksCpp.h"

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
	back_in->setContentSize(CCSizeMake(251, 128));
	back_in->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(0, 15));
	back_case->addChild(back_in);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_firstPurchaseSaleTitle), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25, back_case->getContentSize().height/2.f-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  giveupAction(NULL, CCControlEventTouchUpInside);
							  });
	m_container->addChild(close_button);
	
	CCSprite* ruby_img = CCSprite::create("shop_ruby6.png");
	ruby_img->setPosition(ccp(0,18));
	m_container->addChild(ruby_img);
	
	KSLabelTTF* ruby100_img = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_countName_s, kPurchaseGuideType_firstPurchase-1).c_str(), mySGD->getFont().c_str(), 20);
	ruby100_img->setColor(ccc3(40, 215, 255));
	ruby100_img->enableOuterStroke(ccBLACK, 1);
	ruby100_img->setPosition(ccp(45,43));
	m_container->addChild(ruby100_img);
	
	
	string before_value = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_data_s, kPurchaseGuideType_firstPurchase-1);
	Json::Reader reader;
	Json::Value before_data;
	reader.parse(before_value, before_data);
	
	KSLabelTTF* before_price = KSLabelTTF::create(KS::insert_separator(before_data["beforeDiscount"].asString()).c_str(), mySGD->getFont().c_str(), 14);
	before_price->enableOuterStroke(ccBLACK, 0.5f);
	before_price->setPosition(ccp(0,18));
	m_container->addChild(before_price);
	
	
	CCSprite* sale_arrow = CCSprite::create("sale_arrow.png");
	sale_arrow->setPosition(ccp(0,18));
	m_container->addChild(sale_arrow);
	
	
	KSLabelTTF* after_price = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, kPurchaseGuideType_firstPurchase-1).c_str(), mySGD->getFont().c_str(), 20);
	after_price->setColor(ccYELLOW);
	after_price->enableOuterStroke(ccBLACK, 0.5f);
	after_price->setPosition(ccp(65,18));
	m_container->addChild(after_price);
	
	
	CCScale9Sprite* stamp_case = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	stamp_case->setContentSize(CCSizeMake(70, 45));
	stamp_case->setPosition(ccp(-70,38));
	stamp_case->setRotation(-15);
	m_container->addChild(stamp_case);
	
	
	KSLabelTTF* first_sale_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_firstPurchaseSale), mySGD->getFont().c_str(), 10);
	first_sale_label->disableOuterStroke();
	first_sale_label->setPosition(ccp(35,33));
	stamp_case->addChild(first_sale_label);
	
	string sale_percent_string = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_sale_s, kPurchaseGuideType_firstPurchase-1);
	
	KSLabelTTF* sale_value_label = KSLabelTTF::create(CCString::createWithFormat("-%s", sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 19);
	sale_value_label->setColor(ccYELLOW);
	sale_value_label->enableOuterStroke(ccBLACK, 0.5f);
	sale_value_label->setPosition(ccp(35,17));
	stamp_case->addChild(sale_value_label);
	
	
	StyledLabelTTF* sub_ment = StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_firstPurchaseMent1), sale_percent_string.c_str()), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	sub_ment->setAnchorPoint(ccp(0.5f,0.5f));
	sub_ment->setPosition(ccp(0,-23.5f));
	m_container->addChild(sub_ment);
	
//	KSLabelTTF* sub_ment1 = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_firstPurchaseMent1), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 12);
//	sub_ment1->setColor(ccc3(255,170,20));
//	sub_ment1->disableOuterStroke();
//	sub_ment1->setAnchorPoint(ccp(1.f,0.5f));
//	sub_ment1->setPositionY(-15);
//	m_container->addChild(sub_ment1);
//	
//	
//	KSLabelTTF* sub_ment2 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_firstPurchaseMent2), mySGD->getFont().c_str(), 12);
//	sub_ment2->disableOuterStroke();
//	sub_ment2->setAnchorPoint(ccp(0.f, 0.5f));
//	sub_ment2->setPositionY(-15);
//	m_container->addChild(sub_ment2);
//	
//	
//	float w1 = sub_ment1->getContentSize().width;
//	float w2 = sub_ment2->getContentSize().width;
//	
//	sub_ment1->setPositionX(w1 - (w1+w2)/2.f);
//	sub_ment2->setPositionX(w1 - (w1+w2)/2.f);
//	
//	KSLabelTTF* sub_ment3 = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_firstPurchaseMent3), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 12);
//	sub_ment3->disableOuterStroke();
//	sub_ment3->setPosition(ccp(0,-32));
//	m_container->addChild(sub_ment3);
	
	
//	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_giveup), mySGD->getFont().c_str(), 15);
//	giveup_label->setColor(ccBLACK);
//	CCScale9Sprite* giveup_back = CCScale9Sprite::create("achievement_cellback_normal.png", CCRectMake(0,0,47,47), CCRectMake(23, 23, 1, 1));
//	giveup_button = CCControlButton::create(giveup_label, giveup_back);
//	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(FirstPurchasePopup::giveupAction), CCControlEventTouchUpInside);
//	giveup_button->setPreferredSize(CCSizeMake(80,40));
//	giveup_button->setPosition(ccp(-87,-85));
//	m_container->addChild(giveup_button);
//	
//	giveup_button->setTouchPriority(touch_priority);
	
	
	
	CCLabelTTF* p_label = CCLabelTTF::create();
	
	KSLabelTTF* purchase_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_purchase), mySGD->getFont().c_str(), 15);
	purchase_label->disableOuterStroke();
	purchase_label->setAnchorPoint(ccp(0,0.5f));
	purchase_label->setPosition(ccp(0,0));
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("gray_ellipse.png", CCRectMake(0,0,82,26), CCRectMake(40,12,2,2));
	price_back->setContentSize(CCSizeMake(82, 26));
	price_back->setPosition(ccp(purchase_label->getContentSize().width + price_back->getContentSize().width/2.f + 5, purchase_label->getContentSize().height/2.f));
	purchase_label->addChild(price_back);
	
//	CCSprite* price_type = CCSprite::create("price_ruby_img.png");
//	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-15,price_back->getContentSize().height/2.f));
//	price_back->addChild(price_type);
	KSLabelTTF* price_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_priceName_s, kPurchaseGuideType_firstPurchase-1).c_str(), mySGD->getFont().c_str(), 15);
	price_label->enableOuterStroke(ccBLACK, 0.5f);
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	p_label->addChild(purchase_label);
	
	
	CCScale9Sprite* purchase_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	purchase_button = CCControlButton::create(p_label, purchase_back);
	purchase_button->addTargetWithActionForControlEvents(this, cccontrol_selector(FirstPurchasePopup::purchaseAction), CCControlEventTouchUpInside);
	purchase_button->setPreferredSize(CCSizeMake(170,50));
	purchase_button->setPosition(ccp(0,-80));
	m_container->addChild(purchase_button);
	
	purchase_label->setPositionX(-(purchase_label->getContentSize().width + 5 + price_back->getContentSize().width)/2.f);
	
	purchase_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void FirstPurchasePopup::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
}

void FirstPurchasePopup::purchaseAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	inapp_loading = LoadingLayer::create(-9999, true);
	addChild(inapp_loading);
	
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//	mySGD->addChangeGoods("pg_fp");
//	
//	mySGD->setUserdataIsFirstBuy(1); // true
//	vector<CommandParam> command_list;
//	command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
//	
//	mySGD->changeGoodsTransaction(command_list, [=](Json::Value result_data){
//		inapp_loading->removeFromParent();
//		if(result_data["result"]["code"].asInt() == GDSUCCESS)
//		{
//			is_menu_enable = true;
//			giveupAction(sender, t_event);
//		}
//		else
//		{
//			mySGD->clearChangeGoods();
//			mySGD->clearChangeUserdata();
//			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//			is_menu_enable = true;
//		}
//	});
//	
//	
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	Json::Value param;
	param["productid"] = NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_pID_s, kPurchaseGuideType_firstPurchase-1); //
	CCLOG("productid : %s", param["productid"].asString().c_str());
	hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
		//																				KS::KSLog("in-app test \n%", v);
		if(v["issuccess"].asInt())
		{
			CCLOG("FirstPurchase purchaseProduct success");
//			mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getRankUpRubyFee(), "승급");
			mySGD->setUserdataIsFirstBuy(1); // true
            
            Json::Value t_info = mySGD->getProductInfo(NSDS_GS(kSDS_GI_shopPurchaseGuide_int1_pID_s, kPurchaseGuideType_firstPurchase-1));
            if(!t_info.empty())
			{
                myHSP->getAdXConnectEventInstance("Sale", t_info["price"].asString().c_str(), t_info["currency"].asString().c_str());
				fiverocks::FiveRocksBridge::trackPurchase("Sale", t_info["currency"].asString().c_str(), t_info["price"].asDouble(), "");
			}
            
			requestItemDelivery();
		}
		else
		{
			CCLOG("FirstPurchase purchaseProduct fail");
			inapp_loading->removeFromParent();
//			mySGD->clearChangeUserdata();
			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
			is_menu_enable = true;
		}
	});
//#endif
	
}

void FirstPurchasePopup::requestItemDelivery()
{
	CCLOG("FirstPurchase requestItemDelivery try");
	
	vector<CommandParam> command_list;
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getMemberID();
	command_list.push_back(CommandParam("starttransaction", transaction_param, json_selector(this, FirstPurchasePopup::resultSaveUserData)));
	
	Json::Value request_param;
	request_param["memberID"] = hspConnector::get()->getSocialID();
	command_list.push_back(CommandParam("requestItemDelivery", request_param, nullptr));
	if(mySGD->isChangedGoods())
	{
		vector<CommandParam> t_list = mySGD->getChangeGoodsParam(json_selector(mySGD, StarGoldData::saveChangeGoodsTransaction));
		for(int i=0;i<t_list.size();i++)
		{
			command_list.push_back(t_list[i]);
		}
	}
	command_list.push_back(mySGD->getChangeUserdataParam(nullptr));
	
	hspConnector::get()->command(command_list);
}

void FirstPurchasePopup::resultSaveUserData(Json::Value result_data)
{
	CCLOG("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("FirstPurchase requestItemDelivery success");
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
		mySGD->network_check_cnt = 0;
		
		mySGD->initProperties(result_data["list"]);
		
		is_menu_enable = true;
		giveupAction(NULL, CCControlEventTouchUpInside);
		inapp_loading->removeFromParent();
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_reConnect), myLoc->getLocalForKey(LK::kMyLocalKey_reConnectAlert4),[=](){
				requestItemDelivery();
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 requestItemDelivery();
									 }));
		}
	}
}