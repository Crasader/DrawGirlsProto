//
//  EventShopPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 30..
//
//

#include "EventShopPopup.h"
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
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "StyledLabelTTF.h"
#include "LabelTTFMarquee.h"
#include "FiveRocksCpp.h"

enum EventShopProductCode
{
	kEventShopProductCode_begin = 0,
	kEventShopProductCode_1,
	kEventShopProductCode_2,
	kEventShopProductCode_3,
	kEventShopProductCode_4,
	kEventShopProductCode_5,
	kEventShopProductCode_6,
	kEventShopProductCode_end
};

EventShopPopup* EventShopPopup::create(int t_touch_priority, function<void()> t_end_func)
{
	EventShopPopup* t_mup = new EventShopPopup();
	t_mup->myInit(t_touch_priority, t_end_func);
	t_mup->autorelease();
	return t_mup;
}

void EventShopPopup::myInit(int t_touch_priority, function<void()> t_end_func)
{
	mySGD->showPurchasePopup(kPurchaseGuideType_eventRubyShop);
	
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160-14.f));
	addChild(m_container);
	
	back_case = CCSprite::create("mainpopup2_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* title_back = CCScale9Sprite::create("title_tab.png", CCRectMake(0,0,90,35), CCRectMake(44, 17, 2, 1));
	title_back->setContentSize(CCSizeMake(150, 35));
	title_back->setPosition(ccp(90,back_case->getContentSize().height-13));
	back_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_eventShopTitle), mySGD->getFont().c_str(), 14);
	title_label->disableOuterStroke();
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);

	
	
	CCScale9Sprite* tip_marquee_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	tip_marquee_back->setContentSize(CCSizeMake(278, 26));
	tip_marquee_back->setPosition(ccp(back_case->getContentSize().width*0.655f, back_case->getContentSize().height+2-23.5f));
	back_case->addChild(tip_marquee_back);
	
	string sale_percent_string = NSDS_GS(kSDS_GI_shopEventRuby_int1_sale_s, 0);
	
	LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 278, 22, "");
	tipMaquee->addText((string("<font size=12>") + myLoc->getLocalForKey(LK::kMyLocalKey_eventShopMent1) + "</font>").c_str());
	tipMaquee->addText((string("<font size=12>") + ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_eventShopMent2), sale_percent_string.c_str()) + "</font>").c_str());
	tipMaquee->addText((string("<font size=12>") + ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_eventShopMent3), sale_percent_string.c_str()) + "</font>").c_str());
	tipMaquee->setPosition(ccpFromSize(tip_marquee_back->getContentSize()/2.f));
	tipMaquee->startMarquee();
	tipMaquee->setAnchorPoint(ccp(0.5f,0.5f));
	tip_marquee_back->addChild(tipMaquee);
	
	CCSprite* tipBack = CCSprite::create("tabbutton_up.png");
	tipBack->setPosition(ccp(back_case->getContentSize().width*0.417f, back_case->getContentSize().height+2-25));
	back_case->addChild(tipBack);
	KSLabelTTF* tipLbl = KSLabelTTF::create("TIP", mySGD->getFont().c_str(), 14.f);
	//	tipLbl->disableOuterStroke();
	tipLbl->setPosition(ccpFromSize(tipBack->getContentSize()) / 2.f + ccp(0,1));
	tipBack->addChild(tipLbl);
	
	
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 204));
	main_inner->setPosition(back_case->getContentSize().width/2.f, back_case->getContentSize().height*0.45f);
	back_case->addChild(main_inner);
	
	CommonButton* close_menu = CommonButton::createCloseButton(touch_priority);
	close_menu->setPosition(ccpFromSize(back_case->getContentSize()) + ccp(-20,-12));
	close_menu->setFunction([=](CCObject* sender)
							{
								if(!this->is_menu_enable)
									return;
								
								this->is_menu_enable = false;
								
								CommonAnimation::closePopup(this, m_container, nullptr, [=](){
									
								}, [=](){
									end_func(); removeFromParent();
								});
							});
	back_case->addChild(close_menu);
	
//	KSLabelTTF* sub_ment1 = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_eventShopMent1), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 11);
//	sub_ment1->setAnchorPoint(ccp(0,0.5f));
//	sub_ment1->setPosition(ccpAdd(title_label->getPosition(), ccp(title_label->getContentSize().width + 20,0)));
//	back_case->addChild(sub_ment1);
	
	string filename = "shop_ruby%d.png";
	
	for(int i=4;i<=6;i++)
	{
		CCNode* content_node = CCNode::create();
		content_node->setPosition(ccp(100,125) + ccp((i-4)*140, 0));
		back_case->addChild(content_node, 1, kEventShopProductCode_begin + i);
		
		CCSprite* content_back = CCSprite::create("onesale_back.png");
		content_back->setPosition(ccp(0, 0));
		content_node->addChild(content_back);
		
		CCLabelTTF* t_label = CCLabelTTF::create();
		KSLabelTTF* price_label = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopEventRuby_int1_priceName_s, i-1).c_str(), mySGD->getFont().c_str(), 15);
		t_label->addChild(price_label);
		
		CCControlButton* price_button = CCControlButton::create(t_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
		price_button->addTargetWithActionForControlEvents(this, cccontrol_selector(EventShopPopup::controlAction), CCControlEventTouchUpInside);
		price_button->setPreferredSize(CCSizeMake(110, 50));
		price_button->setTag(kEventShopProductCode_begin + i);
		price_button->setPosition(ccp(0,-67));
		content_node->addChild(price_button);
		
		price_button->setTouchPriority(touch_priority);
		
		
		CCSprite* inner = CCSprite::create(CCString::createWithFormat(filename.c_str(), i)->getCString());
		
		StyledLabelTTF* ruby_label = StyledLabelTTF::create(NSDS_GS(kSDS_GI_shopEventRuby_int1_countName_s, i-1).c_str(), mySGD->getFont().c_str(), 20, 999, StyledAlignment::kCenterAlignment);
		ruby_label->setAnchorPoint(ccp(0.5f,0.5f));
		ruby_label->setPosition(ccpFromSize(inner->getContentSize()/2.f) + ccp(0,-32));
		inner->setPosition(ccp(0,10));
		inner->addChild(ruby_label, 3);
		
		content_node->addChild(inner, 2);
		
		string sale_str = NSDS_GS(kSDS_GI_shopEventRuby_int1_sale_s, i-1);
		string price_key = "wonToRuby%d";
		
		if(!sale_str.empty())
		{
//			CCSprite* tab = CCSprite::create("shop_tab.png");
//			tab->setPosition(ccp(-34,38.f));
//			content_node->addChild(tab, 4);
			
			KSLabelTTF* sale_label = KSLabelTTF::create(sale_str.c_str(), mySGD->getFont().c_str(), 15);
			sale_label->enableOuterStroke(ccBLACK, 1, 153, true);
			sale_label->setPosition(ccp(0,content_back->getContentSize().height/2.f-15));//ccp(tab->getContentSize().width/2.f, tab->getContentSize().height/2.f));
			content_node->addChild(sale_label, 4);
		}
		
//		if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_money))
//		{
//			CCLabelTTF* won_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_shopEventRuby_int1_priceName_s, i-1).c_str(), mySGD->getFont().c_str(), 14);
//			won_label->setPosition(ccp(0, -77));
//			content_node->addChild(won_label, 3);
//		}
//		else if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_ruby))
//		{
//			CCSprite* ruby_img = CCSprite::create("price_ruby_img.png");
//			ruby_img->setPosition(ccp(-30, -33));
//			content_node->addChild(ruby_img, 3);
//			
//			CCLabelTTF* ruby_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat(price_key.c_str(), i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
//			ruby_label->setPosition(ccp(10, -33));
//			content_node->addChild(ruby_label, 3);
//		}
//		else if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_gold))
//		{
//			CCSprite* gold_img = CCSprite::create("price_gold_img.png");
//			gold_img->setPosition(ccp(-30, -33));
//			content_node->addChild(gold_img, 3);
//			
//			CCLabelTTF* gold_label = CCLabelTTF::create(getPriceData(CCString::createWithFormat(price_key.c_str(), i)->getCString()).c_str(), mySGD->getFont().c_str(), 12);
//			gold_label->setPosition(ccp(10, -33));
//			content_node->addChild(gold_label, 3);
//		}
	}
	
	
	
//	KSLabelTTF* sub_ment2 = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_eventShopMent2), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 9);
//	sub_ment2->setAnchorPoint(ccp(1.f,0.5f));
//	sub_ment2->setPosition(ccp(back_case->getContentSize().width-20,15));
//	back_case->addChild(sub_ment2);
	
	
	
	CommonAnimation::openPopup(this, m_container, nullptr, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void EventShopPopup::controlAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int tag = ((CCNode*)sender)->getTag();
	int t_index = tag - kEventShopProductCode_1;
	
	createCheckBuyPopup([=]()
						{
							loading_layer = LoadingLayer::create();
							addChild(loading_layer, 999);
							
							Json::Value param;
							param["productid"] = mySGD->getEventInappProduct(t_index);
							hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
								KS::KSLog("in-app test \n%", v);
								if(v["issuccess"].asInt())
								{
									Json::Value t_info = mySGD->getProductInfo(mySGD->getEventInappProduct(t_index));
									if(!t_info.empty())
									{
										myHSP->getAdXConnectEventInstance("Sale", t_info["price"].asString().c_str(), t_info["currency"].asString().c_str());
										fiverocks::FiveRocksBridge::trackPurchase(ccsf("ShopPurchaseEventGemCode%d", t_index+1), t_info["currency"].asString().c_str(), t_info["price"].asDouble(), "");
										myHSP->IgawAdbrixBuy(ccsf("ShopPurchaseEventGemCode%d", t_index+1));
									}
									requestItemDelivery();
								}
								else
								{
									loading_layer->removeFromParent();
									
//									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
									
									is_menu_enable = true;
								}
							});
						});
}

void EventShopPopup::menuAction(CCObject* sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	
//	addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_afterOpenCBT), [=](){is_menu_enable = true;}), 9999);;
//	
//	
//	return;
	
	int tag = ((CCNode*)sender)->getTag();
	int t_index = tag - kEventShopProductCode_1;
	
	createCheckBuyPopup([=]()
						{
							loading_layer = LoadingLayer::create();
							addChild(loading_layer, 999);
							
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//							mySGD->addChangeGoods(NSDS_GS(kSDS_GI_shopEventRuby_int1_exchangeID_s, t_index), kGoodsType_begin, 0, "", ccsf("%d", mySGD->getUserdataHighPiece()), "상점");
//							
//							mySGD->changeGoods([=](Json::Value result_data){
//								loading_layer->removeFromParent();
//								
//								if(result_data["result"]["code"].asInt() == GDSUCCESS)
//								{
//									mySGD->clearChangeGoods();
//									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_successPurchase)), 9999);
//								}
//								else
//								{
//									mySGD->clearChangeGoods();
//									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//								}
//								is_menu_enable = true;
//							});
//							
//							
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
							Json::Value param;
							param["productid"] = mySGD->getEventInappProduct(t_index);
							hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
								KS::KSLog("in-app test \n%", v);
								if(v["issuccess"].asInt())
								{
                                    Json::Value t_info = mySGD->getProductInfo(mySGD->getEventInappProduct(t_index));
                                    if(!t_info.empty())
									{
                                        myHSP->getAdXConnectEventInstance("Sale", t_info["price"].asString().c_str(), t_info["currency"].asString().c_str());
										fiverocks::FiveRocksBridge::trackPurchase(ccsf("ShopPurchaseEventGemCode%d", t_index+1), t_info["currency"].asString().c_str(), t_info["price"].asDouble(), "");
										myHSP->IgawAdbrixBuy(ccsf("ShopPurchaseEventGemCode%d", t_index+1));
									}
									requestItemDelivery();
								}
								else
								{
									loading_layer->removeFromParent();
									
									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
									
									is_menu_enable = true;
								}
							});
//#endif
						});
}

void EventShopPopup::requestItemDelivery()
{
	Json::Value param;
	param["memberID"] = hspConnector::get()->getMemberID();
	GraphDog::get()->command("requestItemDelivery", param, [=](Json::Value t){
		if(t["result"]["code"].asInt() == GDSUCCESS)
		{
			CCLOG("inapp success!! refresh!!!");
			
            mySGD->network_check_cnt = 0;
            
			addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_successPurchase)), 9999);
			
			mySGD->initProperties(t["list"]);
			//			mySGD->refreshGoodsData(t["list"]["type"].asString(), t["list"]["count"].asInt());
			
			loading_layer->removeFromParent();
			is_menu_enable = true;
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
	});
}

void EventShopPopup::createCheckBuyPopup(function<void()> buy_action)
{
//	ASPopupView* t_popup = ASPopupView::create(touch_priority-5);
//	
//	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//	if(screen_scale_x < 1.f)
//		screen_scale_x = 1.f;
//	
//	float height_value = 320.f;
//	if(myDSH->screen_convert_rate < 1.f)
//		height_value = 320.f/myDSH->screen_convert_rate;
//	
//	if(height_value < myDSH->ui_top)
//		height_value = myDSH->ui_top;
//	
//	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
//	t_popup->setDimmedPosition(ccp(240, 160));
//	t_popup->setBasePosition(ccp(240, 160));
//	
//	CCNode* t_container = CCNode::create();
//	t_popup->setContainerNode(t_container);
//	addChild(t_popup, 900);
//	
//	CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//	case_back->setPosition(ccp(0,0));
//	t_container->addChild(case_back);
//	
//	case_back->setContentSize(CCSizeMake(220, 190));
//	
//	CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//	content_back->setPosition(ccp(0,25));
//	t_container->addChild(content_back);
//	
//	content_back->setContentSize(CCSizeMake(200, 120));
//	
//	CCLabelTTF* ment_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_realBuy),	mySGD->getFont().c_str(), 15);
//	ment_label->setPosition(ccp(0,25));
//	t_container->addChild(ment_label);
//	
//	
//	
//	CommonButton* cancel_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
//	cancel_button->setPosition(ccp(100,85));
//	cancel_button->setFunction([=](CCObject* sender)
//							   {
//								   is_menu_enable = true;
//								   t_popup->removeFromParent();
//							   });
//	t_container->addChild(cancel_button);
//	
//	
//	CommonButton* ok_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_ok), 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
//	ok_button->setPosition(ccp(0,-65));
//	ok_button->setFunction([=](CCObject* sender)
//						   {
//							   buy_action();
//							   t_popup->removeFromParent();
//						   });
//	t_container->addChild(ok_button);
//	
//	
//	
//	
	
	
	addChild(ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_realBuy),[=](){
		
		buy_action();
		
	},12.f,CCPointZero,true,true,[=](){
		is_menu_enable = true;
	}));
	
	
}






