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
#include "CommonAnimation.h"

BuyMorphingPopup* BuyMorphingPopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func)
{
	BuyMorphingPopup* t_mup = new BuyMorphingPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_buy_func);
	t_mup->autorelease();
	return t_mup;
}

void BuyMorphingPopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func)
{
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
	
	back_case = CCSprite::create("popup_large_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251,128));
	back_in->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(0,15));
	back_case->addChild(back_in);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_buyMorphingTitle), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(-85, back_case->getContentSize().height/2.f-35));
	back_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
								  
									CommonAnimation::closePopup(this, m_container, gray, [=](){
										
									}, [=](){
										end_func();
										removeFromParent();
										
									});
							  });
	back_case->addChild(close_button);
	
	StyledLabelTTF* content_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_buyMorphingContent), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
	content_label->setPosition(ccp(back_in->getContentSize().width / 2.f,back_in->getContentSize().height-20));
	back_in->addChild(content_label);
	
	
	CCScale9Sprite* take_case = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	take_case->setContentSize(CCSizeMake(160, 34));
	take_case->setPosition(ccp(back_in->getContentSize().width/2.f, 25));
	back_in->addChild(take_case);
	
	CCSprite* pass_img = CCSprite::create("pass_ticket6.png");
	pass_img->setPosition(ccp(20,18));
	take_case->addChild(pass_img);
	
	KSLabelTTF* take_ment = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_buyMorphingTake), mySGD->getFont().c_str(), 13);
	take_ment->setColor(ccc3(255, 170, 20));
	take_ment->disableOuterStroke();
	take_ment->setAnchorPoint(ccp(0,0.5f));
	take_ment->setPosition(pass_img->getPosition() + ccp(pass_img->getContentSize().width/2.f+3, 0));
	take_case->addChild(take_ment);
	
	KSLabelTTF* take_value_label = KSLabelTTF::create((KS::insert_separator(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass6))) + myLoc->getLocalForKey(LK::kMyLocalKey_buyMorphingTakeValue)).c_str(), mySGD->getFont().c_str(), 13);
	take_value_label->setColor(ccc3(255,170,20));
	take_value_label->disableOuterStroke();
	take_value_label->setAnchorPoint(ccp(1,0.5f));
	take_value_label->setPosition(ccp(take_case->getContentSize().width-10, take_case->getContentSize().height/2.f));
	take_case->addChild(take_value_label);
	
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	KSLabelTTF* buy_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_buyMorphingBuy), mySGD->getFont().c_str(), 13);
	buy_label->disableOuterStroke();
	buy_label->setAnchorPoint(ccp(0.0f, 0.5f));
	r_label->addChild(buy_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("gray_ellipse.png", CCRectMake(0,0,82,26), CCRectMake(40,12,2,2));
	price_back->setContentSize(CCSizeMake(82, 26));
	price_back->setPosition(ccp(buy_label->getContentSize().width + price_back->getContentSize().width/2.f + 5, buy_label->getContentSize().height/2.f));
	buy_label->addChild(price_back);
	
	KSProtectVar<int> need_pass;//, need_ruby;
	need_pass = NSDS_GI(kSDS_CI_int1_mPrice_pass_i, mySGD->selected_collectionbook);
//	need_ruby = NSDS_GI(kSDS_CI_int1_mPrice_ruby_i, mySGD->selected_collectionbook);
	
//	if(mySGD->getGoodsValue(kGoodsType_pass6) >= need_pass.getV())
//	{
		CCSprite* price_type = CCSprite::create("pass_ticket6.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		CCLabelTTF* price_label = CCLabelTTF::create((string(CCString::createWithFormat("%d", need_pass.getV())->getCString()) + myLoc->getLocalForKey(LK::kMyLocalKey_buyMorphingTakeValue)).c_str(), mySGD->getFont().c_str(), 14);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
//	}
//	else
//	{
//		CCSprite* price_type = CCSprite::create("price_ruby_img.png");
//		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
//		price_back->addChild(price_type);
//		CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", need_ruby.getV())->getCString(), mySGD->getFont().c_str(), 15);
//		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
//		price_back->addChild(price_label);
//	}
	
	
	CCScale9Sprite* buy_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	buy_button = CCControlButton::create(r_label, buy_back);
	buy_button->addTargetWithActionForControlEvents(this, cccontrol_selector(BuyMorphingPopup::buyAction), CCControlEventTouchUpInside);
	buy_button->setPreferredSize(CCSizeMake(160,50));
	buy_button->setPosition(ccp(0,-80));
	m_container->addChild(buy_button);
	
	buy_label->setPositionX(-buy_button->getPreferredSize().width/2.f+15);
	
	buy_button->setTouchPriority(touch_priority);
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
	}, [=](){
											 is_menu_enable = true;
	});
}

void BuyMorphingPopup::cancelAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
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
//	else if(mySGD->getGoodsValue(kGoodsType_ruby) >= need_ruby.getV())
//	{
//		buy_loading = LoadingLayer::create(-9999);
//		buy_loading->setPositions(ccp(240,myDSH->ui_center_y));
//		addChild(buy_loading, 9999);
//		
//		mySGD->addChangeGoods("morphing_r", kGoodsType_ruby, -need_ruby.getV(), "", CCString::createWithFormat("%d", mySGD->selected_collectionbook)->getCString());
//		
//		Json::Value param;
//		param["memberID"] = myHSP->getSocialID();
//		param["cardNo"] = mySGD->selected_collectionbook;
//		param["isMorphing"] = true;
//		
//		vector<CommandParam> command_list;
//		command_list.clear();
//		
//		command_list.push_back(CommandParam("updateCardHistory", param, nullptr));
//		
//		mySGD->changeGoodsTransaction(command_list, json_selector(this, BuyMorphingPopup::resultBuyMorphing));
//	}
//	else
//	{
//		ShopPopup* t_popup = ShopPopup::create();
//		t_popup->setScale(myDSH->screen_convert_rate);
//		t_popup->setShopCode(kSC_ruby);
//		t_popup->setCloseFunc([=]()
//							  {
//								  is_menu_enable = true;
//							  });
//		addChild(t_popup);
//	}
	else
	{
		StyledLabelTTF* t_content = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_notEnoughtMorphingContent), mySGD->getFont().c_str(), 14, 999, StyledAlignment::kCenterAlignment);
		t_content->setAnchorPoint(ccp(0.5f,0.5f));
		
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_notEnoughtMorphingTitle), t_content,
							   [=]()
							   {
								   is_menu_enable = true;
							   }, 10.f, ccp(240, myDSH->ui_center_y), true), 9999);
	}
}

void BuyMorphingPopup::resultBuyMorphing(Json::Value result_data)
{
	GraphDogLib::JsonToLog("result buy morphing", result_data);
	
	buy_loading->removeFromParent();
	
	if(result_data["result"]["code"] == GDSUCCESS)
	{
		mySGD->onCardMorphing(mySGD->selected_collectionbook);
		
		CommonAnimation::closePopup(this, m_container, gray, [=](){
			
		}, [=](){
			buy_func(); removeFromParent();
		});
	}
	else
	{
		mySGD->clearChangeGoods();
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase), [](){}, ccp(240,myDSH->ui_center_y)), 9999);
		is_menu_enable = true;
	}
}