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
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(480,280));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_eventShopTitle), mySGD->getFont().c_str(), 16);
	title_label->setColor(ccc3(255, 150, 100));
	title_label->setAnchorPoint(ccp(0,0.5));
	title_label->setPosition(ccp(20,256));
	back_case->addChild(title_label);
	
	CCScale9Sprite* main_inner = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	main_inner->setContentSize(CCSizeMake(445, 228));
	main_inner->setPosition(back_case->getContentSize().width/2.f, back_case->getContentSize().height*0.45f);
	back_case->addChild(main_inner);
	
	CommonButton* close_menu = CommonButton::createCloseButton(touch_priority);
	close_menu->setPosition(ccp(453,257));
	close_menu->setFunction([=](CCObject* sender)
							{
								if(!this->is_menu_enable)
									return;
								
								this->is_menu_enable = false;
								
								addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
									addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
								
								addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
							});
	back_case->addChild(close_menu);
	
	string sale_percent_string = NSDS_GS(kSDS_GI_shopEventRuby_int1_sale_s, 0);
	
	KSLabelTTF* sub_ment1 = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_eventShopMent1), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 11);
	sub_ment1->setAnchorPoint(ccp(0,0.5f));
	sub_ment1->setPosition(ccpAdd(title_label->getPosition(), ccp(title_label->getContentSize().width + 20,0)));
	back_case->addChild(sub_ment1);
	
	string filename = "shop_ruby%d.png";
	
	for(int i=1;i<=6;i++)
	{
		CCNode* content_node = CCNode::create();
		content_node->setPosition(ccp(100,177) + ccp(((i-1)%3)*140, ((i-1)/3)*(-105)));
		back_case->addChild(content_node, 1, kEventShopProductCode_begin + i);
		
		CCSprite* n_content = CCSprite::create("shop_button_back.png");
		CCSprite* s_content = CCSprite::create("shop_button_back.png");
		s_content->setColor(ccGRAY);
		
		CCSprite* inner = CCSprite::create(CCString::createWithFormat(filename.c_str(), i)->getCString());
		
		CCLabelTTF* ruby_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_shopEventRuby_int1_countName_s, i-1).c_str(), mySGD->getFont().c_str(), 12);
		ruby_label->setColor(ccORANGE);
		ruby_label->setPosition(ccp(inner->getContentSize().width/2.f, inner->getContentSize().height/2.f-5));
		inner->addChild(ruby_label);
		
		content_node->addChild(inner, 2);
		
		string sale_str = NSDS_GS(kSDS_GI_shopEventRuby_int1_sale_s, i-1);
		string price_type = NSDS_GS(kSDS_GI_shopEventRuby_int1_priceType_s, i-1);
		string price_key = "wonToRuby%d";
		
		if(!sale_str.empty())
		{
			CCSprite* tab = CCSprite::create("shop_tab.png");
			tab->setPosition(ccp(32,33));
			content_node->addChild(tab, 4);
			
			CCLabelTTF* sale_label = CCLabelTTF::create(sale_str.c_str(), mySGD->getFont().c_str(), 10);
			sale_label->setPosition(ccp(tab->getContentSize().width/2.f, tab->getContentSize().height/2.f+3));
			tab->addChild(sale_label);
		}
		
		if(price_type == mySGD->getGoodsTypeToKey(kGoodsType_money))
		{
			CCLabelTTF* won_label = CCLabelTTF::create(NSDS_GS(kSDS_GI_shopEventRuby_int1_priceName_s, i-1).c_str(), mySGD->getFont().c_str(), 12);
			won_label->setPosition(ccp(0, -33));
			content_node->addChild(won_label, 3);
		}
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
		
		CCMenuItem* content_item = CCMenuItemSprite::create(n_content, s_content, this, menu_selector(EventShopPopup::menuAction));
		content_item->setTag(kEventShopProductCode_begin + i);
		
		CCMenu* content_menu = CCMenu::createWithItem(content_item);
		content_menu->setPosition(CCPointZero);
		content_node->addChild(content_menu, 1);
		
		content_menu->setTouchPriority(touch_priority);
	}
	
	
	
	KSLabelTTF* sub_ment2 = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_eventShopMent2), sale_percent_string.c_str())->getCString(), mySGD->getFont().c_str(), 9);
	sub_ment2->setAnchorPoint(ccp(1.f,0.5f));
	sub_ment2->setPosition(ccp(back_case->getContentSize().width-20,15));
	back_case->addChild(sub_ment2);
	
	
	
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

void EventShopPopup::menuAction(CCObject* sender)
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
							
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
							mySGD->addChangeGoods(kGoodsType_ruby, NSDS_GI(kSDS_GI_shopEventRuby_int1_count_i, t_index), "40%더루비구매(IOS-인앱결재)", "", "", true);
							
							mySGD->changeGoods([=](Json::Value result_data){
								loading_layer->removeFromParent();
								
								if(result_data["result"]["code"].asInt() == GDSUCCESS)
								{
									
								}
								else
								{
									mySGD->clearChangeGoods();
									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
								}
								is_menu_enable = true;
							});
							
							
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
							Json::Value param;
							param["productid"] = mySGD->getEventInappProduct(t_index);
							hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
								KS::KSLog("in-app test \n%", v);
								if(v["issuccess"].asInt())
								{
									requestItemDelivery();
								}
								else
								{
									loading_layer->removeFromParent();
									
									addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
									
									is_menu_enable = true;
								}
							});
#endif
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
			
			mySGD->initProperties(t["list"]);
			//			mySGD->refreshGoodsData(t["list"]["type"].asString(), t["list"]["count"].asInt());
			
			loading_layer->removeFromParent();
			is_menu_enable = true;
		}
		else if(t["result"]["code"].asInt() == 2016) // GDNOTINGWORK
		{
			addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
		}
		else
		{
			addChild(KSTimer::create(3.f, [=](){requestItemDelivery();}));
		}
	});
}

void EventShopPopup::createCheckBuyPopup(function<void()> buy_action)
{
	ASPopupView* t_popup = ASPopupView::create(touch_priority-5);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	float height_value = 320.f;
	if(myDSH->screen_convert_rate < 1.f)
		height_value = 320.f/myDSH->screen_convert_rate;
	
	if(height_value < myDSH->ui_top)
		height_value = myDSH->ui_top;
	
	t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
	t_popup->setDimmedPosition(ccp(240, 160));
	t_popup->setBasePosition(ccp(240, 160));
	
	CCNode* t_container = CCNode::create();
	t_popup->setContainerNode(t_container);
	addChild(t_popup, 900);
	
	CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	case_back->setPosition(ccp(0,0));
	t_container->addChild(case_back);
	
	case_back->setContentSize(CCSizeMake(220, 190));
	
	CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
	content_back->setPosition(ccp(0,25));
	t_container->addChild(content_back);
	
	content_back->setContentSize(CCSizeMake(200, 120));
	
	CCLabelTTF* ment_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_realBuy),	mySGD->getFont().c_str(), 15);
	ment_label->setPosition(ccp(0,25));
	t_container->addChild(ment_label);
	
	
	
	CommonButton* cancel_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
	cancel_button->setPosition(ccp(100,85));
	cancel_button->setFunction([=](CCObject* sender)
							   {
								   is_menu_enable = true;
								   t_popup->removeFromParent();
							   });
	t_container->addChild(cancel_button);
	
	
	CommonButton* ok_button = CommonButton::create(myLoc->getLocalForKey(kMyLocalKey_ok), 15, CCSizeMake(110, 50), CommonButtonOrange, t_popup->getTouchPriority()-5);
	ok_button->setPosition(ccp(0,-65));
	ok_button->setFunction([=](CCObject* sender)
						   {
							   buy_action();
							   t_popup->removeFromParent();
						   });
	t_container->addChild(ok_button);
}





