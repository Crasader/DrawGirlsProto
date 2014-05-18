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

//void ContinueContent::menuAction(CCObject* sender)
//{
//	if(!is_menu_enable)
//		return;
//	
//	is_menu_enable = false;
//	
//	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
//
//	
//	int tag = ((CCNode*)sender)->getTag();
//	
//	if(tag == kContinueContentMenuTag_end)
//	{
//		end_selector();
//	}
//	else if(tag == kContinueContentMenuTag_continue)
//	{
//		if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getPlayContinueFee())
//		{
//			LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
//			t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
//			addChild(t_popup, 9999);
//			
//			mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getPlayContinueFee(), "이어하기");
//			mySGD->changeGoods([=](Json::Value result_data)
//			{
//				t_popup->removeFromParent();
//				if(result_data["result"]["code"].asInt() == GDSUCCESS)
//				{
//					is_continue = true;
//					
//					CCSprite* price_type = CCSprite::create("price_ruby_img.png");
//					price_type->setOpacity(0);
//					price_type->setPosition(ccpAdd(ccp(52,-48), ccp(-15, 0)));
//					addChild(price_type);
//					
//					CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("-%d", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 16);
//					price_label->setOpacity(0);
//					price_label->setAnchorPoint(ccp(0,0.5f));
//					price_label->setPosition(ccp(price_type->getContentSize().width/2.f+15,price_type->getContentSize().height/2.f));
//					price_type->addChild(price_label);
//					
//					CCFadeTo* t_fade1 = CCFadeTo::create(0.2f, 255);
//					CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
//					CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 0);
//					CCSequence* t_seq = CCSequence::create(t_fade1, t_delay1, t_fade2, NULL);
//					
//					CCMoveBy* t_move1 = CCMoveBy::create(0.9f, ccp(0,50));
//					
//					CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_seq, t_move1);
//					CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::startHide));
//					CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
//					
//					price_type->runAction(t_seq2);
//					
//					CCFadeTo* t_fade3 = CCFadeTo::create(0.2f, 255);
//					CCDelayTime* t_delay3 = CCDelayTime::create(0.2f);
//					CCFadeTo* t_fade4 = CCFadeTo::create(0.5f, 0);
//					CCSequence* t_seq3 = CCSequence::create(t_fade3, t_delay3, t_fade4, NULL);
//					price_label->runAction(t_seq3);
//				}
//				else
//				{
//					mySGD->clearChangeGoods();
//					addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
//					
//					is_menu_enable = true;
//				}
//			});
//		}
//		else
//		{
//			CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//			case_back->setPosition(CCPointZero);
//			addChild(case_back);
//			
//			case_back->setContentSize(CCSizeMake(250, 150));
//			
//			CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6));
//			content_back->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(0,20)));
//			case_back->addChild(content_back);
//			
//			content_back->setContentSize(CCSizeMake(230,90));
//			
//			
//			CCSprite* price_type_img1 = CCSprite::create("price_ruby_img.png");
//			price_type_img1->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-55,35)));
//			case_back->addChild(price_type_img1);
//			
//			CCSprite* price_type_img2 = CCSprite::create("price_ruby_img.png");
//			price_type_img2->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-88,17)));
//			case_back->addChild(price_type_img2);
//			
//			CCLabelTTF* price_value_label = CCLabelTTF::create(" 가 부족합니다.\n   10개를 구입하시겠습니까?", mySGD->getFont().c_str(), 16);
//			price_value_label->setAnchorPoint(ccp(0,0.5f));
//			price_value_label->setPosition(ccp(price_type_img1->getContentSize().width/2.f-35, price_type_img1->getContentSize().height/2.f-8));
//			price_type_img1->addChild(price_value_label);
//			
//			
//			CCScale9Sprite* n_cancel = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
//			n_cancel->setContentSize(CCSizeMake(95,45));
//			CCLabelTTF* n_cancel_label = CCLabelTTF::create("취소", mySGD->getFont().c_str(), 14);
//			n_cancel_label->setPosition(ccp(n_cancel->getContentSize().width/2.f, n_cancel->getContentSize().height/2.f));
//			n_cancel->addChild(n_cancel_label);
//			
//			CCScale9Sprite* s_cancel = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
//			s_cancel->setContentSize(CCSizeMake(95,45));
//			s_cancel->setColor(ccGRAY);
//			CCLabelTTF* s_cancel_label = CCLabelTTF::create("취소", mySGD->getFont().c_str(), 14);
//			s_cancel_label->setPosition(ccp(s_cancel->getContentSize().width/2.f, s_cancel->getContentSize().height/2.f));
//			s_cancel->addChild(s_cancel_label);
//			
//			CCMenuItemLambda* cancel_item = CCMenuItemSpriteLambda::create(n_cancel, s_cancel, [=](CCObject* sender){case_back->removeFromParent();is_menu_enable = true;});
//			
//			CCMenuLambda * cancel_menu = CCMenuLambda::createWithItem(cancel_item);
//			cancel_menu->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-52,-48)));
//			case_back->addChild(cancel_menu);
//			cancel_menu->setTouchPriority(touch_priority-2);
//			
//			
//			CCScale9Sprite* n_buy = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
//			n_buy->setContentSize(CCSizeMake(95,45));
//			CCLabelTTF* n_buy_label = CCLabelTTF::create("임시 결제", mySGD->getFont().c_str(), 14);
//			n_buy_label->setPosition(ccp(n_buy->getContentSize().width/2.f, n_buy->getContentSize().height/2.f));
//			n_buy->addChild(n_buy_label);
//			
//			CCScale9Sprite* s_buy = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
//			s_buy->setContentSize(CCSizeMake(95,45));
//			s_buy->setColor(ccGRAY);
//			CCLabelTTF* s_buy_label = CCLabelTTF::create("임시 결제", mySGD->getFont().c_str(), 14);
//			s_buy_label->setPosition(ccp(s_buy->getContentSize().width/2.f, s_buy->getContentSize().height/2.f));
//			s_buy->addChild(s_buy_label);
//			
//			CCMenuItemLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender)
//																		{
//																			inapp_loading = LoadingLayer::create(-9999, true);
//																			getParent()->addChild(inapp_loading, 9999);
//																			
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//																			mySGD->addChangeGoods(kGoodsType_ruby, NSDS_GI(kSDS_GI_shopRuby_int1_count_i, 0), "이어하기(IOS-인앱결제)", "", "", true);
//																			mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getPlayContinueFee(), "이어하기(IOS-소모)");
//																			
//																			mySGD->changeGoods([=](Json::Value result_data){
//																				inapp_loading->removeFromParent();
//																				if(result_data["result"]["code"].asInt() == GDSUCCESS)
//																				{
//																					is_continue = true;
//																					case_back->removeFromParent();
//																					
//																					CCSprite* price_type = CCSprite::create("price_ruby_img.png");
//																					price_type->setOpacity(0);
//																					price_type->setPosition(ccpAdd(ccp(52,-48), ccp(-15, 0)));
//																					addChild(price_type);
//																					
//																					CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("-%d", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 16);
//																					price_label->setOpacity(0);
//																					price_label->setAnchorPoint(ccp(0,0.5f));
//																					price_label->setPosition(ccp(price_type->getContentSize().width/2.f+15,price_type->getContentSize().height/2.f));
//																					price_type->addChild(price_label);
//																					
//																					CCFadeTo* t_fade1 = CCFadeTo::create(0.2f, 255);
//																					CCDelayTime* t_delay1 = CCDelayTime::create(0.2f);
//																					CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 0);
//																					CCSequence* t_seq = CCSequence::create(t_fade1, t_delay1, t_fade2, NULL);
//																					
//																					CCMoveBy* t_move1 = CCMoveBy::create(0.9f, ccp(0,50));
//																					
//																					CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_seq, t_move1);
//																					CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::startHide));
//																					CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
//																					
//																					price_type->runAction(t_seq2);
//																					
//																					CCFadeTo* t_fade3 = CCFadeTo::create(0.2f, 255);
//																					CCDelayTime* t_delay3 = CCDelayTime::create(0.2f);
//																					CCFadeTo* t_fade4 = CCFadeTo::create(0.5f, 0);
//																					CCSequence* t_seq3 = CCSequence::create(t_fade3, t_delay3, t_fade4, NULL);
//																					price_label->runAction(t_seq3);
//																				}
//																				else
//																				{
//																					mySGD->clearChangeGoods();
//																					
//																					getParent()->addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
//																					
//																					is_menu_enable = true;
//																				}
//																			});
//																			
//																			
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//																			Json::Value param;
//																			param["productid"] = mySGD->getInappProduct(0);
//																			hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
////																				KS::KSLog("in-app test \n%", v);
//																				if(v["issuccess"].asInt())
//																				{
//																					mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getPlayContinueFee(), "이어하기");
//																					requestItemDelivery();
//																				}
//																				else
//																				{
//																					inapp_loading->removeFromParent();
//																					
//																					getParent()->addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
//																					
//																					is_menu_enable = true;
//																				}
//																			});
//#endif
//																		});
//			
//			CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
//			buy_menu->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(52,-48)));
//			case_back->addChild(buy_menu);
//			buy_menu->setTouchPriority(touch_priority-2);
//			
//			case_back->setScale(0);
//			CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
//			case_back->runAction(t_scale);
//		}
//	}
//}

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
	else if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getPlayContinueFee())
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
		
		
//		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
//		case_back->setPosition(CCPointZero);
//		addChild(case_back);
//		
//		case_back->setContentSize(CCSizeMake(250, 150));
//		
//		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6));
//		content_back->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(0,20)));
//		case_back->addChild(content_back);
//		
//		content_back->setContentSize(CCSizeMake(230,90));
//		
//		
//		CCSprite* price_type_img1 = CCSprite::create("price_ruby_img.png");
//		price_type_img1->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-55,35)));
//		case_back->addChild(price_type_img1);
//		
//		CCSprite* price_type_img2 = CCSprite::create("price_ruby_img.png");
//		price_type_img2->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-88,17)));
//		case_back->addChild(price_type_img2);
//		
//		CCLabelTTF* price_value_label = CCLabelTTF::create(" 가 부족합니다.\n   10개를 구입하시겠습니까?", mySGD->getFont().c_str(), 16);
//		price_value_label->setAnchorPoint(ccp(0,0.5f));
//		price_value_label->setPosition(ccp(price_type_img1->getContentSize().width/2.f-35, price_type_img1->getContentSize().height/2.f-8));
//		price_type_img1->addChild(price_value_label);
//		
//		
//		CCScale9Sprite* n_cancel = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
//		n_cancel->setContentSize(CCSizeMake(95,45));
//		CCLabelTTF* n_cancel_label = CCLabelTTF::create("취소", mySGD->getFont().c_str(), 14);
//		n_cancel_label->setPosition(ccp(n_cancel->getContentSize().width/2.f, n_cancel->getContentSize().height/2.f));
//		n_cancel->addChild(n_cancel_label);
//		
//		CCScale9Sprite* s_cancel = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
//		s_cancel->setContentSize(CCSizeMake(95,45));
//		s_cancel->setColor(ccGRAY);
//		CCLabelTTF* s_cancel_label = CCLabelTTF::create("취소", mySGD->getFont().c_str(), 14);
//		s_cancel_label->setPosition(ccp(s_cancel->getContentSize().width/2.f, s_cancel->getContentSize().height/2.f));
//		s_cancel->addChild(s_cancel_label);
//		
//		CCMenuItemLambda* cancel_item = CCMenuItemSpriteLambda::create(n_cancel, s_cancel, [=](CCObject* sender){case_back->removeFromParent();is_menu_enable = true;});
//		
//		CCMenuLambda * cancel_menu = CCMenuLambda::createWithItem(cancel_item);
//		cancel_menu->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(-52,-48)));
//		case_back->addChild(cancel_menu);
//		cancel_menu->setTouchPriority(touch_priority-2);
//		
//		
//		CCScale9Sprite* n_buy = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
//		n_buy->setContentSize(CCSizeMake(95,45));
//		CCLabelTTF* n_buy_label = CCLabelTTF::create("임시 결제", mySGD->getFont().c_str(), 14);
//		n_buy_label->setPosition(ccp(n_buy->getContentSize().width/2.f, n_buy->getContentSize().height/2.f));
//		n_buy->addChild(n_buy_label);
//		
//		CCScale9Sprite* s_buy = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
//		s_buy->setContentSize(CCSizeMake(95,45));
//		s_buy->setColor(ccGRAY);
//		CCLabelTTF* s_buy_label = CCLabelTTF::create("임시 결제", mySGD->getFont().c_str(), 14);
//		s_buy_label->setPosition(ccp(s_buy->getContentSize().width/2.f, s_buy->getContentSize().height/2.f));
//		s_buy->addChild(s_buy_label);
//		
//		CCMenuItemLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender)
//																	{
//																		inapp_loading = LoadingLayer::create(-9999, true);
//																		getParent()->addChild(inapp_loading, 9999);
//																		
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//																		mySGD->addChangeGoods(kGoodsType_ruby, NSDS_GI(kSDS_GI_shopRuby_int1_count_i, 0), "이어하기(IOS-인앱결제)", "", "", true);
//																		mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getPlayContinueFee(), "이어하기(IOS-소모)");
//																		
//																		mySGD->changeGoods([=](Json::Value result_data){
//																			inapp_loading->removeFromParent();
//																			if(result_data["result"]["code"].asInt() == GDSUCCESS)
//																			{
//																				is_continue = true;
//																				case_back->removeFromParent();
//																				
//																				CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
//																				CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
//																				CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
//																				CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
//																				CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
//																				
//																				price_type->runAction(t_seq2);
//																				
//																				CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
//																				CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
//																				CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
//																				price_label->runAction(t_spawn2);
//																			}
//																			else
//																			{
//																				mySGD->clearChangeGoods();
//																				schedule(schedule_selector(ContinueContent::countingSchedule), 1.f);
//																				
//																				getParent()->addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
//																				
//																				is_menu_enable = true;
//																			}
//																		});
//																		
//																		
//#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//																		Json::Value param;
//																		param["productid"] = mySGD->getInappProduct(0);
//																		hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
//																			//																				KS::KSLog("in-app test \n%", v);
//																			if(v["issuccess"].asInt())
//																			{
//																				mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getPlayContinueFee(), "이어하기");
//																				requestItemDelivery();
//																			}
//																			else
//																			{
//																				inapp_loading->removeFromParent();
//																				schedule(schedule_selector(ContinueContent::countingSchedule), 1.f);
//																				getParent()->addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
//																				
//																				is_menu_enable = true;
//																			}
//																		});
//#endif
//																	});
//		
//		CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
//		buy_menu->setPosition(ccpAdd(ccp(case_back->getContentSize().width/2.f, case_back->getContentSize().height/2.f), ccp(52,-48)));
//		case_back->addChild(buy_menu);
//		buy_menu->setTouchPriority(touch_priority-2);
//		
//		case_back->setScale(0);
//		CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
//		case_back->runAction(t_scale);
	}
}

void ContinueContent::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	unschedule(schedule_selector(ContinueContent::countingSchedule));
	closeAction();
}

void ContinueContent::closeAction()
{
	is_menu_enable = false;
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){setScaleY(t);}, [=](float t){setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(this, t);}, [=](int t)
	{
		KS::setOpacity(this, 0);
		if(is_continue)
			continue_selector();
		else
			end_selector();
	}));
}

void ContinueContent::countingSchedule()
{
	counting_value--;
	
	if(counting_value >= 0)
		counting_label->setString(CCString::createWithFormat("%d", counting_value)->getCString());
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
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_doYouWantToContinue), mySGD->getFont().c_str(), 30);
	title_label->setPosition(ccp(0,100));
	addChild(title_label);
	
	counting_value = 10;
	
	counting_label = CCLabelBMFont::create(CCString::createWithFormat("%d", counting_value)->getCString(), "continue.fnt");
	counting_label->setPosition(ccp(0,0));
	addChild(counting_label);
	
	
	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_offContinue), mySGD->getFont().c_str(), 13);
	CCScale9Sprite* giveup_back = CCScale9Sprite::create("subpop_gray.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	CCControlButton* giveup_button = CCControlButton::create(giveup_label, giveup_back);
	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ContinueContent::giveupAction), CCControlEventTouchUpInside);
	giveup_button->setPreferredSize(CCSizeMake(110,60));
	giveup_button->setPosition(ccp(-65,-90));
	addChild(giveup_button);
	
	giveup_button->setTouchPriority(touch_priority-1);
	
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	
	KSLabelTTF* c_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_onContinue), mySGD->getFont().c_str(), 13);
	c_label->setPosition(ccp(0,10));
	CCScale9Sprite* price_back = CCScale9Sprite::create("subpop_darkred.png", CCRectMake(0,0,30,30), CCRectMake(14,14,2,2));
	price_back->setContentSize(CCSizeMake(80, 30));
	price_back->setPosition(ccp(c_label->getContentSize().width/2.f, c_label->getContentSize().height/2.f-20));
	c_label->addChild(price_back);
	
	if(mySGD->getGoodsValue(kGoodsType_pass1) > 0)
	{
		price_type = CCSprite::create("pass_ticket1.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		price_label = CCLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_free), mySGD->getFont().c_str(), 12);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+15,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	else
	{
		price_type = CCSprite::create("price_ruby_img.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-20,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getPlayContinueFee())->getCString(), mySGD->getFont().c_str(), 12);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+15,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
	}
	
	r_label->addChild(c_label);
	
	
	CCScale9Sprite* c_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	CCControlButton* continue_button = CCControlButton::create(r_label, c_back);
	continue_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ContinueContent::continueAction), CCControlEventTouchUpInside);
	continue_button->setPreferredSize(CCSizeMake(110,60));
	continue_button->setPosition(ccp(65,-90));
	addChild(continue_button);
	
	continue_button->setTouchPriority(touch_priority-1);
}