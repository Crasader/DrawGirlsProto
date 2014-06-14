//
//  ContinueContent.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 2. 3..
//
//

#include "ContinueContent.h"
#include "CCMenuLambda.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "MyLocalization.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "ShopPopup.h"

void ContinueContent::requestItemDelivery()
{
	vector<CommandParam> command_list;
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getMemberID();
	command_list.push_back(CommandParam("starttransaction", transaction_param, [=](Json::Value result_data)
										{
											if(result_data["result"]["code"].asInt() == GDSUCCESS)
											{
												inapp_loading->removeFromParent();
												
												is_continue = true;
												case_back->removeFromParent();
												
												CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
												CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
												CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
												CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
												CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
												
												price_type->runAction(t_seq2);
												
												CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
												CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
												CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
												price_label->runAction(t_spawn2);
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
	{
		vector<CommandParam> t_list = mySGD->getChangeGoodsParam(json_selector(mySGD, StarGoldData::saveChangeGoodsTransaction));
		for(int i=0;i<t_list.size();i++)
		{
			command_list.push_back(t_list[i]);
		}
	}
	
	hspConnector::get()->command(command_list);
}

void ContinueContent::continueAction(cocos2d::CCObject *sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	unschedule(schedule_selector(ContinueContent::countingSchedule));
	
	if(mySGD->getGoodsValue(kGoodsType_pass1) > 0)
	{
		LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
		t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
		addChild(t_popup, 9999);
		
		mySGD->addChangeGoods("rp_p", kGoodsType_pass1, 0, "", CCString::createWithFormat("%d", mySD->getSilType())->getCString());
		mySGD->changeGoods([=](Json::Value result_data)
						   {
							   t_popup->removeFromParent();
							   if(result_data["result"]["code"].asInt() == GDSUCCESS)
							   {
								   giveup_button->setEnabled(false);
								   continue_button->setEnabled(false);
								   
								   is_continue = true;
								   
								   CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
								   CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
								   CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
								   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
								   CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
								   
								   price_type->runAction(t_seq2);
								   
								   CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
								   CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
								   CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
								   price_label->runAction(t_spawn2);
							   }
							   else
							   {
								   schedule(schedule_selector(ContinueContent::countingSchedule), 1.f);
								   
								   mySGD->clearChangeGoods();
								   getParent()->addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
								   
								   is_menu_enable = true;
							   }
						   });
	}
	else if(mySGD->is_endless_mode)
	{
		if(mySGD->getGoodsValue(kGoodsType_gold) >= mySGD->getPlayContinueFeeEndless())
		{
			LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
			t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
			addChild(t_popup, 9999);
			
			mySGD->addChangeGoods("rp_g", kGoodsType_gold, 0, "", CCString::createWithFormat("%d", mySD->getSilType())->getCString());
			mySGD->changeGoods([=](Json::Value result_data)
							   {
								   t_popup->removeFromParent();
								   if(result_data["result"]["code"].asInt() == GDSUCCESS)
								   {
									   giveup_button->setEnabled(false);
									   continue_button->setEnabled(false);
									   
									   is_continue = true;
									   
									   CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
									   CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
									   CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
									   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
									   CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
									   
									   price_type->runAction(t_seq2);
									   
									   CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
									   CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
									   CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
									   price_label->runAction(t_spawn2);
								   }
								   else
								   {
									   schedule(schedule_selector(ContinueContent::countingSchedule), 1.f);
									   
									   mySGD->clearChangeGoods();
									   getParent()->addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
									   
									   is_menu_enable = true;
								   }
							   });
		}
		else
		{
			ShopPopup* t_popup = ShopPopup::create();
			t_popup->setScale(myDSH->screen_convert_rate);
			t_popup->setShopCode(kSC_gold);
			t_popup->setCloseFunc([=]()
								  {
									  is_menu_enable = true;
									  schedule(schedule_selector(ContinueContent::countingSchedule), 1.f);
								  });
			getParent()->addChild(t_popup);
		}
	}
	else
	{
		if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getPlayContinueFee())
		{
			LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
			t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
			addChild(t_popup, 9999);
			
			mySGD->addChangeGoods("rp_r", kGoodsType_ruby, 0, "", CCString::createWithFormat("%d", mySD->getSilType())->getCString());
			mySGD->changeGoods([=](Json::Value result_data)
							   {
								   t_popup->removeFromParent();
								   if(result_data["result"]["code"].asInt() == GDSUCCESS)
								   {
									   giveup_button->setEnabled(false);
									   continue_button->setEnabled(false);
									   
									   is_continue = true;
									   
									   CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
									   CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
									   CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
									   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
									   CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
									   
									   price_type->runAction(t_seq2);
									   
									   CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
									   CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
									   CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
									   price_label->runAction(t_spawn2);
								   }
								   else
								   {
									   schedule(schedule_selector(ContinueContent::countingSchedule), 1.f);
									   
									   mySGD->clearChangeGoods();
									   getParent()->addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
									   
									   is_menu_enable = true;
								   }
							   });
		}
		else
		{
			ShopPopup* t_popup = ShopPopup::create();
			t_popup->setScale(myDSH->screen_convert_rate);
			t_popup->setShopCode(kSC_ruby);
			t_popup->setCloseFunc([=]()
								  {
									  is_menu_enable = true;
									  schedule(schedule_selector(ContinueContent::countingSchedule), 1.f);
								  });
			getParent()->addChild(t_popup);
		}
	}
}

void ContinueContent::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	giveup_button->setEnabled(false);
	continue_button->setEnabled(false);
	
	unschedule(schedule_selector(ContinueContent::countingSchedule));
	closeAction();
}

void ContinueContent::closeAction()
{
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->stopEffect("se_clock.mp3");
	
	CommonAnimation::closePopup(this, this, nullptr, [=](){
		
	}, [=](){
		if(is_continue)
			continue_selector();
		else
			end_selector();
		
	});
}

void ContinueContent::countingSchedule()
{
	counting_value--;
	
	int ani_value = counting_value%60; // 59->start : 0->end
	if(ani_value >= 60-6) // 59 58 57 56 55 54
	{
		float cvt_value = (59.f-ani_value)/5.f;
		counting_label->setOpacity(cvt_value*200.f);
	}
	else if(ani_value >= 60-6-10) // 53 ~ 44
	{
		float cvt_value = (53.f-ani_value)/9.f;
		counting_label->setScale(5.f-4.f*cvt_value);
		counting_label->setOpacity(200+55*cvt_value);
	}
	else if(ani_value >= 60-6-10-32)
	{
		
	}
	else // 11 ~ 0
	{
		float cvt_value = (11.f-ani_value)/11.f;
		counting_label->setScaleX(1.f-cvt_value);
		counting_label->setOpacity(255-255*cvt_value);
	}
	
	
	if(counting_value >= 0)
	{
		counting_label->setString(CCString::createWithFormat("%d", counting_value/60)->getCString());
	}
	else
	{
		unschedule(schedule_selector(ContinueContent::countingSchedule));
		
		closeAction();
	}
}

void ContinueContent::myInit(int t_touch_priority, function<void(void)> t_end, function<void(void)> t_continue)
{
	is_continue = false;
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_selector = t_end;
	continue_selector = t_continue;
	
	CCScale9Sprite* back_case = CCScale9Sprite::create("mainpopup_back.png", CCRectMake(0, 0, 50, 50), CCRectMake(24, 24, 2, 2));
	back_case->setContentSize(CCSizeMake(290, 230));
	back_case->setPosition(ccp(0,0));
	addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("mainpopup_pupple5.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
	back_in->setContentSize(CCSizeMake(260,130));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f,back_case->getContentSize().height/2.f+5.f));
	back_case->addChild(back_in,2);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_doYouWantToContinue), mySGD->getFont().c_str(), 15);
	title_label->setColor(ccc3(255, 170, 20));
	title_label->setPosition(ccp(back_case->getContentSize().width/2.f,back_case->getContentSize().height-25));
	back_case->addChild(title_label);
	
	counting_value = 10*60;
	
	counting_label = CCLabelBMFont::create(CCString::createWithFormat("%d", counting_value/60)->getCString(), "continue.fnt");
	counting_label->setScale(5.f);
	counting_label->setOpacity(0);
	counting_label->setAnchorPoint(ccp(0.5f,0.5f));
	counting_label->setPosition(ccp(back_in->getContentSize().width/2.f,back_in->getContentSize().height/2.f));
	back_in->addChild(counting_label);
	counting_label->setVisible(false);
	
	
	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_offContinue), mySGD->getFont().c_str(), 15);
	giveup_label->setColor(ccBLACK);
	CCScale9Sprite* giveup_back = CCScale9Sprite::create("achievement_cellback_normal.png", CCRectMake(0,0,47,47), CCRectMake(23, 23, 1, 1));
	giveup_button = CCControlButton::create(giveup_label, giveup_back);
	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ContinueContent::giveupAction), CCControlEventTouchUpInside);
	giveup_button->setPreferredSize(CCSizeMake(80,40));
	giveup_button->setPosition(ccp(back_case->getContentSize().width/2.f-87,back_case->getContentSize().height/2.f-85));
	back_case->addChild(giveup_button);
	
	giveup_button->setTouchPriority(touch_priority-1);
	
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	
	KSLabelTTF* c_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_onContinue), mySGD->getFont().c_str(), 15);
	c_label->setAnchorPoint(ccp(0,0.5f));
	c_label->setPosition(ccp(0,0));
	CCScale9Sprite* price_back = CCScale9Sprite::create("gray_ellipse.png", CCRectMake(0,0,82,26), CCRectMake(40,12,2,2));
	price_back->setContentSize(CCSizeMake(82, 26));
	price_back->setPosition(ccp(c_label->getContentSize().width + price_back->getContentSize().width/2.f + 5, c_label->getContentSize().height/2.f));
	c_label->addChild(price_back);
	
	if(mySGD->getGoodsValue(kGoodsType_pass1) > 0)
	{
		price_type = CCSprite::create("pass_ticket1.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		price_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_free), mySGD->getFont().c_str(), 15);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+15,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	else if(mySGD->is_endless_mode)
	{
		price_type = CCSprite::create("price_gold_img.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getPlayContinueFeeEndless())->getCString(), mySGD->getFont().c_str(), 17);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+15,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	else
	{
		price_type = CCSprite::create("price_ruby_img.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 17);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+15,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	
	r_label->addChild(c_label);
	
	
	CCScale9Sprite* c_back = CCScale9Sprite::create("common_button_lightpupple.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	continue_button = CCControlButton::create(r_label, c_back);
	continue_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ContinueContent::continueAction), CCControlEventTouchUpInside);
	continue_button->setPreferredSize(CCSizeMake(170,44));
	continue_button->setPosition(ccp(back_case->getContentSize().width/2.f+43,back_case->getContentSize().height/2.f-85));
	back_case->addChild(continue_button);
	
	continue_button->setTouchPriority(touch_priority-1);
	
	c_label->setPositionX(-continue_button->getPreferredSize().width/2.f+15);
}