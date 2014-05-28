//
//  BuyMorphingPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 22..
//
//

#include "BuyMorphingPopup.h"
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
#include "StarGoldData.h"
#include "StyledLabelTTF.h"

BuyMorphingPopup* BuyMorphingPopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func)
{
	BuyMorphingPopup* t_mup = new BuyMorphingPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_buy_func);
	t_mup->autorelease();
	return t_mup;
}

void BuyMorphingPopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func)
{
	mySGD->showPurchasePopup(kPurchaseGuideType_levelupGuide);
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	buy_func = t_buy_func;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,myDSH->ui_center_y));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0,0,50,50), CCRectMake(24,24,2,2));
	back_case->setContentSize(CCSizeMake(260,210));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_front.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_in->setContentSize(CCSizeMake(back_case->getContentSize().width-10, back_case->getContentSize().height-46));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f-17));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_buyMorphingTitle), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setAnchorPoint(ccp(0,0.5f));
	title_label->setPosition(ccp(17, back_case->getContentSize().height-25));
	back_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-25));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
								  
								  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
									  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
								  
								  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
																	   {
																		   gray->setOpacity(t);
																		   KS::setOpacity(m_container, t);
																	   }, [=](int t)
																	   {
																		   gray->setOpacity(0);
																		   KS::setOpacity(m_container, 0);
																		   end_func();
																		   removeFromParent();
																	   }));
							  });
	back_case->addChild(close_button);
	
	StyledLabelTTF* content_label = StyledLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_buyMorphingContent), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kLeftAlignment);
	content_label->setPosition(ccp(15,back_in->getContentSize().height-20));
	back_in->addChild(content_label);
	
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	KSLabelTTF* buy_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_buyMorphingBuy), mySGD->getFont().c_str(), 13);
	buy_label->setAnchorPoint(ccp(0,0.5f));
	r_label->addChild(buy_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("gray_ellipse.png", CCRectMake(0,0,82,26), CCRectMake(40,12,2,2));
	price_back->setContentSize(CCSizeMake(82, 26));
	price_back->setPosition(ccp(buy_label->getContentSize().width + price_back->getContentSize().width/2.f + 5, buy_label->getContentSize().height/2.f));
	buy_label->addChild(price_back);
	
	KSProtectVar<int> need_pass, need_ruby;
	need_pass = NSDS_GI(kSDS_CI_int1_mPrice_pass_i, mySGD->selected_collectionbook);
	need_ruby = NSDS_GI(kSDS_CI_int1_mPrice_ruby_i, mySGD->selected_collectionbook);
	
	if(mySGD->getGoodsValue(kGoodsType_pass6) >= need_pass.getV())
	{
		CCSprite* price_type = CCSprite::create("pass_ticket6.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", need_pass.getV())->getCString(), mySGD->getFont().c_str(), 15);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	else
	{
		CCSprite* price_type = CCSprite::create("price_ruby_img.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", need_ruby.getV())->getCString(), mySGD->getFont().c_str(), 15);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	
	
	CCScale9Sprite* buy_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	buy_button = CCControlButton::create(r_label, buy_back);
	buy_button->addTargetWithActionForControlEvents(this, cccontrol_selector(BuyMorphingPopup::buyAction), CCControlEventTouchUpInside);
	buy_button->setPreferredSize(CCSizeMake(200,50));
	buy_button->setPosition(ccp(0,-63));
	m_container->addChild(buy_button);
	
	buy_label->setPositionX(-buy_button->getPreferredSize().width/2.f+15);
	
	buy_button->setTouchPriority(touch_priority);
	
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t)
										 {
											 gray->setOpacity(t);
											 KS::setOpacity(m_container, t);
										 }, [=](int t)
										 {
											 gray->setOpacity(255);
											 KS::setOpacity(m_container, 255);
											 is_menu_enable = true;
										 }));
}

void BuyMorphingPopup::cancelAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}

void BuyMorphingPopup::buyAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	KSProtectVar<int> need_pass, need_ruby;
	need_pass = NSDS_GI(kSDS_CI_int1_mPrice_pass_i, mySGD->selected_collectionbook);
	need_ruby = NSDS_GI(kSDS_CI_int1_mPrice_ruby_i, mySGD->selected_collectionbook);
	
	if(mySGD->getGoodsValue(kGoodsType_pass6) >= need_pass.getV())
	{
		buy_loading = LoadingLayer::create(-9999);
		buy_loading->setPositions(ccp(240,myDSH->ui_center_y));
		addChild(buy_loading, 9999);
		
		mySGD->addChangeGoods("morphing_p", kGoodsType_pass6, -need_pass.getV(), "", CCString::createWithFormat("%d", mySGD->selected_collectionbook)->getCString());
		
		Json::Value param;
		param["memberID"] = myHSP->getSocialID();
		param["cardNo"] = mySGD->selected_collectionbook;
		param["isMorphing"] = true;
		
		vector<CommandParam> command_list;
		command_list.clear();
		
		command_list.push_back(CommandParam("updateCardHistory", param, nullptr));
		
		mySGD->changeGoodsTransaction(command_list, json_selector(this, BuyMorphingPopup::resultBuyMorphing));
	}
	else if(mySGD->getGoodsValue(kGoodsType_ruby) >= need_ruby.getV())
	{
		buy_loading = LoadingLayer::create(-9999);
		buy_loading->setPositions(ccp(240,myDSH->ui_center_y));
		addChild(buy_loading, 9999);
		
		mySGD->addChangeGoods("morphing_r", kGoodsType_ruby, -need_ruby.getV(), "", CCString::createWithFormat("%d", mySGD->selected_collectionbook)->getCString());
		
		Json::Value param;
		param["memberID"] = myHSP->getSocialID();
		param["cardNo"] = mySGD->selected_collectionbook;
		param["isMorphing"] = true;
		
		vector<CommandParam> command_list;
		command_list.clear();
		
		command_list.push_back(CommandParam("updateCardHistory", param, nullptr));
		
		mySGD->changeGoodsTransaction(command_list, json_selector(this, BuyMorphingPopup::resultBuyMorphing));
	}
	else
	{
		ShopPopup* t_popup = ShopPopup::create();
		t_popup->setScale(myDSH->screen_convert_rate);
		t_popup->setShopCode(kSC_ruby);
		t_popup->setCloseFunc([=]()
							  {
								  is_menu_enable = true;
							  });
		addChild(t_popup);
	}
}

void BuyMorphingPopup::resultBuyMorphing(Json::Value result_data)
{
	GraphDogLib::JsonToLog("result buy morphing", result_data);
	
	buy_loading->removeFromParent();
	
	if(result_data["result"]["code"] == GDSUCCESS)
	{
		mySGD->onCardMorphing(mySGD->selected_collectionbook);
		
		addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
			addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
		
		addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); buy_func(); removeFromParent();}));
		
	}
	else
	{
		mySGD->clearChangeGoods();
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase), [](){}, ccp(240,myDSH->ui_center_y)), 9999);
		is_menu_enable = true;
	}
}