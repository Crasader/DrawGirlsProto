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
#include "CommonButton.h"

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

void ContinueContent::endShow()
{
	counting_label->setOpacity(0);
	counting_label->setVisible(true);
	
	is_menu_enable = true;
	schedule(schedule_selector(ContinueContent::countingSchedule));
}

void ContinueContent::continueAction2(cocos2d::CCObject *sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	unschedule(schedule_selector(ContinueContent::countingSchedule));
	
	addChild(ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_realContinue),[=]()
										{
											/*if(mySGD->is_endless_mode)
											{
												if(mySGD->getGoodsValue(kGoodsType_gold) >= mySGD->getPlayContinueFeeEndless())
												{
													LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
													t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
													addChild(t_popup, 9999);
													
													mySGD->addChangeGoods("rp_g", kGoodsType_gold, 0, "", CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "이어하기(PvP)");
													mySGD->changeGoods([=](Json::Value result_data)
																	   {
																		   t_popup->removeFromParent();
																		   if(result_data["result"]["code"].asInt() == GDSUCCESS)
																		   {
																			   mySGD->pvp_continue_cnt++;
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
																			   schedule(schedule_selector(ContinueContent::countingSchedule));
																			   
																			   mySGD->clearChangeGoods();
																			   getParent()->addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
																			   
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
																			  schedule(schedule_selector(ContinueContent::countingSchedule));
																		  });
													getParent()->addChild(t_popup);
												}
											}
											else */if(mySGD->getGoodsValue(kGoodsType_pass1) > 0)
											{
												LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
												t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
												addChild(t_popup, 9999);
												
                                                
												mySGD->addChangeGoods("rp_p", kGoodsType_pass1, 0, "", CCString::createWithFormat("%d", mySD->getSilType())->getCString(), "이어하기(패스권)");
                                                
                                                vector<CommandParam> t_command_list;
                                                t_command_list.clear();
                                                t_command_list.push_back(mySGD->getChangeUserdataParam([=](Json::Value result_data)
                                                                                                       {
                                                                                                           if(result_data["result"]["code"].asInt() == GDSUCCESS)
                                                                                                               myGD->communication("Main_changeMonsterAutoLevel");
                                                                                                       }));
                                                
                                                mySGD->changeGoodsTransaction(t_command_list,[=](Json::Value result_data)
                                                                              {
                                                                                  t_popup->removeFromParent();
                                                                                  if(result_data["result"]["code"].asInt() == GDSUCCESS)
                                                                                  {
                                                                                      mySGD->ingame_continue_cnt++;
                                                                                      //																		   giveup_button->setEnabled(false);
                                                                                      continue_button2->setEnabled(false);
                                                                                      continue_button->setEnabled(false);
                                                                                      
                                                                                      is_continue = true;
                                                                                      
                                                                                      CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
                                                                                      CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
                                                                                      CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
                                                                                      CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
                                                                                      CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
                                                                                      
                                                                                      price_type2->runAction(t_seq2);
                                                                                      
                                                                                      CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
                                                                                      CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
                                                                                      CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
                                                                                      price_label2->runAction(t_spawn2);
                                                                                  }
                                                                                  else
                                                                                  {
                                                                                      mySGD->clearChangeGoods();
                                                                                      mySGD->clearChangeUserdata();
                                                                                      addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase), [=]()
                                                                                                                                                    {
                                                                                                                                                        schedule(schedule_selector(ContinueContent::countingSchedule));
                                                                                                                                                    },ccp(0.01f,0.01f)), 9999999);
                                                                                      
                                                                                      is_menu_enable = true;
                                                                                  }
                                                                              }, false);
                                                
//												mySGD->changeGoods([=](Json::Value result_data)
//																   {
//																	   t_popup->removeFromParent();
//																	   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//																	   {
//																		   mySGD->ingame_continue_cnt++;
////																		   giveup_button->setEnabled(false);
//																		   continue_button2->setEnabled(false);
//																		   continue_button->setEnabled(false);
//																		   
//																		   is_continue = true;
//																		   
//																		   CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
//																		   CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
//																		   CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
//																		   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
//																		   CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
//																		   
//																		   price_type2->runAction(t_seq2);
//																		   
//																		   CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
//																		   CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
//																		   CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
//																		   price_label2->runAction(t_spawn2);
//																	   }
//																	   else
//																	   {
//																		   schedule(schedule_selector(ContinueContent::countingSchedule));
//																		   
//																		   mySGD->clearChangeGoods();
//																		   getParent()->addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//																		   
//																		   is_menu_enable = true;
//																	   }
//																   });
											}
											else
											{
//												if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getPlayContinueFee())
//												{
//													LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
//													t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
//													addChild(t_popup, 9999);
//													
//													mySGD->addChangeGoods("rp_r", kGoodsType_ruby, 0, "", CCString::createWithFormat("%d", mySD->getSilType())->getCString(), "이어하기(루비)");
//													mySGD->changeGoods([=](Json::Value result_data)
//																	   {
//																		   t_popup->removeFromParent();
//																		   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//																		   {
//																			   giveup_button->setEnabled(false);
//																			   continue_button->setEnabled(false);
//																			   
//																			   is_continue = true;
//																			   
//																			   CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
//																			   CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
//																			   CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
//																			   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
//																			   CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
//																			   
//																			   price_type->runAction(t_seq2);
//																			   
//																			   CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
//																			   CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
//																			   CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
//																			   price_label->runAction(t_spawn2);
//																		   }
//																		   else if(result_data["result"]["code"] == GDPROPERTYISMINUS)
//																		   {
//																			   getParent()->addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought), [=]()
//																																{
//																																	schedule(schedule_selector(ContinueContent::countingSchedule));
//																																	mySGD->clearChangeGoods();
//																																	is_menu_enable = true;
//																																}), 9999);
//																		   }
//																		   else
//																		   {
//																			   schedule(schedule_selector(ContinueContent::countingSchedule));
//																			   
//																			   mySGD->clearChangeGoods();
//																			   getParent()->addChild(ASPopupView::getCommonNoti(touch_priority-200,
//																																myLoc->getLocalForKey(LK::kMyLocalKey_noti),
//																																myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//																			   
//																			   is_menu_enable = true;
//																		   }
//																	   });
//												}
//												else
//												{
													ShopPopup* t_popup = ShopPopup::create();
													t_popup->setScale(myDSH->screen_convert_rate);
													t_popup->setShopCode(kSC_p1);
													t_popup->setCloseFunc([=]()
																		  {
																			  if(take_count)
																			  {
																				  take_count->setString(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_ruby)));
																				  float wide_value = take_count->getContentSize().width - 40;
																				  if(wide_value < 0)
																					  wide_value = 0;
																				  take_back->setContentSize(CCSizeMake(60+wide_value, 20));
																				  take_count->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
																			  }
																			  
																			  if(take_count2)
																			  {
																				  take_count2->setString(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_pass1)));
																				  float wide_value = take_count2->getContentSize().width - 40;
																				  if(wide_value < 0)
																					  wide_value = 0;
																				  take_back2->setContentSize(CCSizeMake(60+wide_value, 20));
																				  take_count2->setPosition(ccpFromSize(take_back2->getContentSize()/2.f));
																			  }
																			  
																			  is_menu_enable = true;
																			  schedule(schedule_selector(ContinueContent::countingSchedule));
																		  });
													t_popup->is_continue = true;
													t_popup->continue_end = [=]()
													{
														continueAction(NULL, CCControlEventTouchUpInside);
													};
													getParent()->addChild(t_popup);
//												}
											}
											
										},12.f,ccp(0.01f,0.01f),true,true,[=](){
											
											schedule(schedule_selector(ContinueContent::countingSchedule));
											
											is_menu_enable = true;
										}), 9999);
}

void ContinueContent::continueAction(cocos2d::CCObject *sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
	unschedule(schedule_selector(ContinueContent::countingSchedule));
	
	addChild(ASPopupView::getCommonNoti(-99999,myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_realContinue),[=]()
	{
		if(mySGD->is_endless_mode)
		{
			if(mySGD->getGoodsValue(kGoodsType_gold) >= mySGD->getPlayContinueFeeEndless()*myGD->getCommunication("Jack_getContinueOnCount"))
			{
				LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
				t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
				addChild(t_popup, 9999);
				
				mySGD->addChangeGoods("rp_g", kGoodsType_gold, -mySGD->getPlayContinueFeeEndless()*myGD->getCommunication("Jack_getContinueOnCount"), "", CCString::createWithFormat("%d", mySGD->endless_my_victory.getV())->getCString(), "이어하기(PvP)");
				mySGD->changeGoods([=](Json::Value result_data)
								   {
									   t_popup->removeFromParent();
									   if(result_data["result"]["code"].asInt() == GDSUCCESS)
									   {
										   mySGD->pvp_continue_cnt++;
//										   giveup_button->setEnabled(false);
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
										   mySGD->clearChangeGoods();
										   addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase), [=]()
                                                                                            {
                                                                                                schedule(schedule_selector(ContinueContent::countingSchedule));
                                                                                            },ccp(0.01f,0.01f)), 9999999);
										   
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
										  if(take_count)
										  {
											  take_count->setString(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_ruby)));
											  float wide_value = take_count->getContentSize().width - 40;
											  if(wide_value < 0)
												  wide_value = 0;
											  take_back->setContentSize(CCSizeMake(60+wide_value, 20));
											  take_count->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
										  }
										  
										  if(take_count2)
										  {
											  take_count2->setString(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_pass1)));
											  float wide_value = take_count2->getContentSize().width - 40;
											  if(wide_value < 0)
												  wide_value = 0;
											  take_back2->setContentSize(CCSizeMake(60+wide_value, 20));
											  take_count2->setPosition(ccpFromSize(take_back2->getContentSize()/2.f));
										  }
										  
										  is_menu_enable = true;
										  schedule(schedule_selector(ContinueContent::countingSchedule));
									  });
				getParent()->addChild(t_popup);
			}
		}
//		else if(mySGD->getGoodsValue(kGoodsType_pass1) > 0)
//		{
//			LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
//			t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
//			addChild(t_popup, 9999);
//			
//			mySGD->addChangeGoods("rp_p", kGoodsType_pass1, 0, "", CCString::createWithFormat("%d", mySD->getSilType())->getCString(), "이어하기(패스권)");
//			mySGD->changeGoods([=](Json::Value result_data)
//							   {
//								   t_popup->removeFromParent();
//								   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//								   {
//									   mySGD->ingame_continue_cnt++;
//									   giveup_button->setEnabled(false);
//									   continue_button->setEnabled(false);
//									   
//									   is_continue = true;
//									   
//									   CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
//									   CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
//									   CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
//									   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
//									   CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
//									   
//									   price_type->runAction(t_seq2);
//									   
//									   CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
//									   CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
//									   CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
//									   price_label->runAction(t_spawn2);
//								   }
//								   else
//								   {
//									   schedule(schedule_selector(ContinueContent::countingSchedule));
//									   
//									   mySGD->clearChangeGoods();
//									   getParent()->addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//									   
//									   is_menu_enable = true;
//								   }
//							   });
//		}
		else
		{
			if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getPlayContinueFee() + myGD->getCommunication("Jack_getContinueOnCount")-1)
			{
				LoadingLayer* t_popup = LoadingLayer::create(touch_priority-200, true);
				t_popup->setPosition(ccp(-240, -myDSH->ui_center_y));
				addChild(t_popup, 9999);
				
        
                
                
                
				mySGD->addChangeGoods("rp_r", kGoodsType_ruby, -(mySGD->getPlayContinueFee() + myGD->getCommunication("Jack_getContinueOnCount")-1), "", CCString::createWithFormat("%d", mySD->getSilType())->getCString(), "이어하기(루비)");
                
                vector<CommandParam> t_command_list;
                t_command_list.clear();
                t_command_list.push_back(mySGD->getChangeUserdataParam([=](Json::Value result_data)
                                                                       {
                                                                           if(result_data["result"]["code"].asInt() == GDSUCCESS)
                                                                               myGD->communication("Main_changeMonsterAutoLevel");
                                                                       }));
                
                mySGD->changeGoodsTransaction(t_command_list, [=](Json::Value result_data)
                                              {
                                                  t_popup->removeFromParent();
                                                  if(result_data["result"]["code"].asInt() == GDSUCCESS)
                                                  {
													  mySGD->ingame_continue_cnt++;
                                                      //										   giveup_button->setEnabled(false);
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
                                                  else if(result_data["result"]["code"] == GDPROPERTYISMINUS)
                                                  {
                                                      mySGD->clearChangeGoods();
                                                      mySGD->clearChangeUserdata();
                                                      is_menu_enable = true;
                                                      
                                                      addChild(ASPopupView::getCommonNoti(-9999999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought), [=]()
                                                                                                       {
                                                                                                           schedule(schedule_selector(ContinueContent::countingSchedule));
                                                                                                       },ccp(0.01f,0.01f)), 9999999);
                                                  }
                                                  else
                                                  {
                                                      mySGD->clearChangeGoods();
                                                      mySGD->clearChangeUserdata();
                                                      addChild(ASPopupView::getCommonNoti(touch_priority-200,
                                                                                                       myLoc->getLocalForKey(LK::kMyLocalKey_noti),
                                                                                                       myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase), [=]()
                                                                                                       {
                                                                                                           schedule(schedule_selector(ContinueContent::countingSchedule));
                                                                                                       },ccp(0.01f,0.01f)), 9999999);
                                                      
                                                      is_menu_enable = true;
                                                  }
											  }, false);
                
//				mySGD->changeGoods([=](Json::Value result_data)
//								   {
//									   t_popup->removeFromParent();
//									   if(result_data["result"]["code"].asInt() == GDSUCCESS)
//									   {
////										   giveup_button->setEnabled(false);
//										   continue_button->setEnabled(false);
//										   
//										   is_continue = true;
//										   
//										   CCFadeTo* t_fade1 = CCFadeTo::create(1.f, 0);
//										   CCMoveBy* t_move1 = CCMoveBy::create(1.f, ccp(0,50));
//										   CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_fade1, t_move1);
//										   CCCallFunc* t_call = CCCallFunc::create(this, callfunc_selector(ContinueContent::closeAction));
//										   CCSequence* t_seq2 = CCSequence::create(t_spawn, t_call, NULL);
//										   
//										   price_type->runAction(t_seq2);
//										   
//										   CCFadeTo* t_fade3 = CCFadeTo::create(1.f, 0);
//										   CCMoveBy* t_move2 = CCMoveBy::create(1.f, ccp(0,50));
//										   CCSpawn* t_spawn2 = CCSpawn::createWithTwoActions(t_fade3, t_move2);
//										   price_label->runAction(t_spawn2);
//									   }
//									   else if(result_data["result"]["code"] == GDPROPERTYISMINUS)
//									   {
//										   getParent()->addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought), [=]()
//																							{
//																								schedule(schedule_selector(ContinueContent::countingSchedule));
//																								mySGD->clearChangeGoods();
//																								is_menu_enable = true;
//																							}), 9999);
//									   }
//									   else
//									   {
//										   schedule(schedule_selector(ContinueContent::countingSchedule));
//										   
//										   mySGD->clearChangeGoods();
//										   getParent()->addChild(ASPopupView::getCommonNoti(touch_priority-200,
//																							myLoc->getLocalForKey(LK::kMyLocalKey_noti),
//																							myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
//										   
//										   is_menu_enable = true;
//									   }
//								   });
			}
			else
			{
//				getParent()->addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought), [=]()
//																 {
//																	 schedule(schedule_selector(ContinueContent::countingSchedule));
//																	 is_menu_enable = true;
//																 }), 9999);
				
				ShopPopup* t_popup = ShopPopup::create();
				t_popup->setScale(myDSH->screen_convert_rate);
				t_popup->setShopCode(kSC_ruby);
				t_popup->setCloseFunc([=]()
									  {
										  if(take_count)
										  {
											  take_count->setString(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_ruby)));
											  float wide_value = take_count->getContentSize().width - 40;
											  if(wide_value < 0)
												  wide_value = 0;
											  take_back->setContentSize(CCSizeMake(60+wide_value, 20));
											  take_count->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
										  }
										  
										  if(take_count2)
										  {
											  take_count2->setString(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_pass1)));
											  float wide_value = take_count2->getContentSize().width - 40;
											  if(wide_value < 0)
												  wide_value = 0;
											  take_back2->setContentSize(CCSizeMake(60+wide_value, 20));
											  take_count2->setPosition(ccpFromSize(take_back2->getContentSize()/2.f));
										  }
										  
										  is_menu_enable = true;
										  schedule(schedule_selector(ContinueContent::countingSchedule));
									  });
				t_popup->is_continue = true;
				t_popup->continue_end = [=]()
				{
					continueAction(NULL, CCControlEventTouchUpInside);
				};
				getParent()->addChild(t_popup);
			}
		}
		
	},12.f,ccp(0.01f,0.01f),true,true,[=](){
		
		schedule(schedule_selector(ContinueContent::countingSchedule));
		
		is_menu_enable = true;
	}), 9999);
}

void ContinueContent::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
	
//	giveup_button->setEnabled(false);
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
		counting_label->setOpacity(200+54*cvt_value);
	}
	else if(ani_value >= 60-6-10-32)
	{
		counting_label->setScale(1.f);
		counting_label->setOpacity(254);
	}
	else // 11 ~ 0
	{
		float cvt_value = (11.f-ani_value)/11.f;
		counting_label->setScaleX(1.f-cvt_value);
		counting_label->setOpacity(254-254*cvt_value);
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
	
	CCSprite* back_case = CCSprite::create("ingame_back.png");
	back_case->setPosition(ccp(0,0));
	addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(235,75));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f,back_case->getContentSize().height/2.f+25.f));
	back_case->addChild(back_in,3);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_doYouWantToContinue), mySGD->getFont().c_str(), 16);
	title_label->enableOuterStroke(ccBLACK, 1, int(255*0.6f), true);// disableOuterStroke();
	title_label->setPosition(ccp(back_case->getContentSize().width/2.f,back_case->getContentSize().height-28));
	back_case->addChild(title_label);
	
	counting_value = 10*60;
	
	counting_label = KSLabelTTF::create(CCString::createWithFormat("%d", counting_value/60)->getCString(), mySGD->getFont().c_str(), 80);
	counting_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	counting_label->enableOuterStroke(ccc3(60, 20, 0), 2.5f, 255, true);
	counting_label->setScale(5.f);
	counting_label->setOpacity(0);
	counting_label->setAnchorPoint(ccp(0.5f,0.5f));
	counting_label->setPosition(ccp(back_in->getContentSize().width/2.f,back_in->getContentSize().height/2.f-8));
	back_in->addChild(counting_label);
	counting_label->setVisible(false);
	
	
	CommonButton* cancel_button = CommonButton::createCloseButton(touch_priority-1);
	cancel_button->setFunction([=](CCObject* sender)
							   {
								   if(!is_menu_enable)
									   return;
								   
								   is_menu_enable = false;
								   
								   AudioEngine::sharedInstance()->playEffect("se_button1.mp3");
								   
//								   giveup_button->setEnabled(false);
								   cancel_button->setEnabled(false);
								   continue_button->setEnabled(false);
								   
								   unschedule(schedule_selector(ContinueContent::countingSchedule));
								   closeAction();
							   });
	cancel_button->setPosition(ccpFromSize(back_case->getContentSize()) + ccp(-20,-12));
	back_case->addChild(cancel_button);
	
//	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_offContinue), mySGD->getFont().c_str(), 15);
//	giveup_label->disableOuterStroke();
//	CCScale9Sprite* giveup_back = CCScale9Sprite::create("achievement_button_ing.png");
//	giveup_button = CCControlButton::create(giveup_label, giveup_back);
//	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ContinueContent::giveupAction), CCControlEventTouchUpInside);
//	giveup_button->setPreferredSize(CCSizeMake(101,44));
//	giveup_button->setPosition(ccp(back_case->getContentSize().width/2.f-77,back_case->getContentSize().height/2.f-65));
//	back_case->addChild(giveup_button);
//	
//	giveup_button->setTouchPriority(touch_priority-1);
	
	continue_button2 = NULL;
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	
	KSLabelTTF* c_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_onContinueGem), mySGD->getFont().c_str(), 13);
	c_label->disableOuterStroke();
	c_label->setAnchorPoint(ccp(0.5f,0.5f));
	c_label->setPosition(ccp(0,15));
	
	CCNode* price_node = CCNode::create();
	price_node->setPosition(ccp(0, -10));
	
	
	take_count = NULL;
	take_count2 = NULL;
	
//	CCScale9Sprite* price_back = CCScale9Sprite::create("gray_ellipse.png", CCRectMake(0,0,82,26), CCRectMake(40,12,2,2));
//	price_back->setContentSize(CCSizeMake(82, 26));
//	price_back->setPosition(ccp(c_label->getContentSize().width + price_back->getContentSize().width/2.f + 5, c_label->getContentSize().height/2.f));
//	c_label->addChild(price_back);
	
	if(mySGD->is_endless_mode)
	{
		title_label->setPosition(title_label->getPosition() + ccp(0,-2));
		
		back_in->setContentSize(CCSizeMake(225, 90));
		back_in->setPosition(ccp(back_case->getContentSize().width/2.f,back_case->getContentSize().height/2.f+11.f));
		
		counting_label->setPosition(ccpFromSize(back_in->getContentSize()/2.f) + ccp(0,-6));
		
		c_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_onContinue));
		c_label->setAnchorPoint(ccp(0,0.5f));
		c_label->setPosition(ccp(0,0));
		
		price_node->setPosition(ccp(c_label->getContentSize().width + 1, c_label->getContentSize().height/2.f));
		c_label->addChild(price_node);
		r_label->addChild(c_label);
		
		price_type = CCSprite::create("price_gold_img.png");
		price_type->setPosition(ccp(price_type->getContentSize().width/2.f,0));
		price_node->addChild(price_type);
		price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getPlayContinueFeeEndless()*myGD->getCommunication("Jack_getContinueOnCount"))->getCString(), mySGD->getFont().c_str(), 15);
		price_label->setAnchorPoint(ccp(0,0.5f));
		price_label->setPosition(ccp(price_type->getContentSize().width+2,0));
		price_node->addChild(price_label);
		
		CCScale9Sprite* c_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
		
		continue_button = CCControlButton::create(r_label, c_back);
		continue_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ContinueContent::continueAction), CCControlEventTouchUpInside);
		continue_button->setPreferredSize(CCSizeMake(150,44));
		continue_button->setPosition(ccp(back_case->getContentSize().width/2.f,back_case->getContentSize().height/2.f-62));
		back_case->addChild(continue_button, 2);
		
		continue_button->setTouchPriority(touch_priority-1);
		
		c_label->setPositionX(-(c_label->getContentSize().width + 1 + price_type->getContentSize().width + 2 + price_label->getContentSize().width)/2.f);
		
		
		return;
	}
	else if(myGD->getCommunication("Jack_getContinueOnCount") == 1)
	{
		// 버튼 두개(부활석, 젬 소모)
		
        take_count = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_ruby)), mySGD->getFont().c_str(), 11);
        take_count->enableOuterStroke(ccBLACK, 0.3f, 50, true);
        
        float wide_value = take_count->getContentSize().width - 40;
        if(wide_value < 0)
            wide_value = 0;
        
        take_back = CCScale9Sprite::create("common_count.png", CCRectMake(0, 0, 60, 20), CCRectMake(29, 9, 2, 2));
        take_back->setContentSize(CCSizeMake(60+wide_value, 20));
        take_back->setAnchorPoint(ccp(1.f,0.5f));
        take_back->setPosition(ccp(58,31));
        r_label->addChild(take_back);
        
        take_count->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
        take_back->addChild(take_count);
        
        
        
		CCScale9Sprite* price_back1 = CCScale9Sprite::create("common_in.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		price_back1->setContentSize(CCSizeMake(80, 28));
		price_back1->setPosition(ccp(0,0));
		price_node->addChild(price_back1);
		
		CCLabelTTF* r_label2 = CCLabelTTF::create();
		
		KSLabelTTF* c_label2 = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_onContinueP1), mySGD->getFont().c_str(), 13);
		c_label2->disableOuterStroke();
		c_label2->setAnchorPoint(ccp(0.5f,0.5f));
		c_label2->setPosition(ccp(0,15));
		r_label2->addChild(c_label2);
		
        take_count2 = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_pass1)), mySGD->getFont().c_str(), 11);
        take_count2->enableOuterStroke(ccBLACK, 0.3f, 50, true);
        
        wide_value = take_count2->getContentSize().width - 40;
        if(wide_value < 0)
            wide_value = 0;
        
        take_back2 = CCScale9Sprite::create("common_count.png", CCRectMake(0, 0, 60, 20), CCRectMake(29, 9, 2, 2));
        take_back2->setContentSize(CCSizeMake(60+wide_value, 20));
        take_back2->setAnchorPoint(ccp(1.f,0.5f));
        take_back2->setPosition(ccp(58,31));
        r_label2->addChild(take_back2);
        
        take_count2->setPosition(ccpFromSize(take_back2->getContentSize()/2.f));
        take_back2->addChild(take_count2);
        
        
		CCNode* price_node2 = CCNode::create();
		price_node2->setPosition(ccp(0,-10));
		r_label2->addChild(price_node2);
		
		CCScale9Sprite* price_back2 = CCScale9Sprite::create("common_in.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		price_back2->setContentSize(CCSizeMake(80, 28));
		price_back2->setPosition(ccp(0,0));
		price_node2->addChild(price_back2);
		
		price_type2 = CCSprite::create("pass_ticket1.png");
		price_type2->setPosition(ccp(price_type2->getContentSize().width/2.f,0));
		price_node2->addChild(price_type2);
		
//		CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
//		pass_count_case->setPosition(ccpFromSize(price_type2->getContentSize()/2.f) + ccp(9,6));
//		price_type2->addChild(pass_count_case);
//		
//		CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass1)), mySGD->getFont().c_str(), 8);
//		pass_count_label->setColor(ccc3(255, 255, 255));
//		float min_width = 10+pass_count_label->getContentSize().width;
//		if(min_width < 20)
//			min_width = 20;
//		pass_count_case->setContentSize(CCSizeMake(min_width, 20));
//		pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
//		pass_count_case->addChild(pass_count_label);
		
		price_label2 = CCLabelTTF::create(CCString::createWithFormat("%d", 1)->getCString(), mySGD->getFont().c_str(), 15);
		price_label2->setAnchorPoint(ccp(0.5f,0.5f));
		price_label2->setPosition(ccp(price_type2->getContentSize().width/2.f,0));
		price_node2->addChild(price_label2);
		
		price_type2->setPositionX(-price_label2->getContentSize().width/2.f);
		
		
		CCScale9Sprite* c_back2 = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
		
		continue_button2 = CCControlButton::create(r_label2, c_back2);
		continue_button2->addTargetWithActionForControlEvents(this, cccontrol_selector(ContinueContent::continueAction2), CCControlEventTouchUpInside);
		continue_button2->setPreferredSize(CCSizeMake(120,70));
		continue_button2->setPosition(ccp(back_case->getContentSize().width/2.f-61,back_case->getContentSize().height/2.f-53));
		back_case->addChild(continue_button2, 2);
		
		continue_button2->setTouchPriority(touch_priority-1);
		
//		c_label2->setPositionX(-(c_label2->getContentSize().width + 1 + price_type2->getContentSize().width + 2 + price_label2->getContentSize().width)/2.f);
		
		
		
		
		price_type = CCSprite::create("price_ruby_img.png");
		price_type->setPosition(ccp(price_type->getContentSize().width/2.f,0));
		price_node->addChild(price_type);
		price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getPlayContinueFee() + myGD->getCommunication("Jack_getContinueOnCount")-1)->getCString(), mySGD->getFont().c_str(), 15);
		price_label->setAnchorPoint(ccp(0.5f,0.5f));
		price_label->setPosition(ccp(price_type->getContentSize().width/2.f,0));
		price_node->addChild(price_label);
		price_type->setPositionX(-price_label->getContentSize().width/2.f);
	}
//	else if(mySGD->getGoodsValue(kGoodsType_pass1) > 0)
//	{
//		price_type = CCSprite::create("pass_ticket1.png");
//		price_type->setPosition(ccp(price_type->getContentSize().width/2.f,0));
//		price_node->addChild(price_type);
//		
//		CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
//		pass_count_case->setPosition(ccpFromSize(price_type->getContentSize()/2.f) + ccp(9,6));
//		price_type->addChild(pass_count_case);
//		
//		CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass1)), mySGD->getFont().c_str(), 8);
//		pass_count_label->setColor(ccc3(255, 255, 255));
//		float min_width = 10+pass_count_label->getContentSize().width;
//		if(min_width < 20)
//			min_width = 20;
//		pass_count_case->setContentSize(CCSizeMake(min_width, 20));
//		pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
//		pass_count_case->addChild(pass_count_label);
//		
//		price_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 15);
//		price_label->setAnchorPoint(ccp(0,0.5f));
//		price_label->setPosition(ccp(price_type->getContentSize().width+2,0));
//		price_node->addChild(price_label);
//	}
	else
	{
        take_count = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(kGoodsType_ruby)), mySGD->getFont().c_str(), 11);
        take_count->enableOuterStroke(ccBLACK, 0.3f, 50, true);
        
        float wide_value = take_count->getContentSize().width - 40;
        if(wide_value < 0)
            wide_value = 0;
        
        take_back = CCScale9Sprite::create("common_count.png", CCRectMake(0, 0, 60, 20), CCRectMake(29, 9, 2, 2));
        take_back->setContentSize(CCSizeMake(60+wide_value, 20));
        take_back->setAnchorPoint(ccp(1.f,0.5f));
        take_back->setPosition(ccp(58,31));
        r_label->addChild(take_back);
        
        take_count->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
        take_back->addChild(take_count);
        
		CCScale9Sprite* price_back1 = CCScale9Sprite::create("common_in.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
		price_back1->setContentSize(CCSizeMake(80, 28));
		price_back1->setPosition(ccp(0,0));
		price_node->addChild(price_back1);
		
		price_type = CCSprite::create("price_ruby_img.png");
		price_type->setPosition(ccp(price_type->getContentSize().width/2.f,0));
		price_node->addChild(price_type);
		price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getPlayContinueFee() + myGD->getCommunication("Jack_getContinueOnCount")-1)->getCString(), mySGD->getFont().c_str(), 15);
		price_label->setAnchorPoint(ccp(0.5f,0.5f));
		price_label->setPosition(ccp(price_type->getContentSize().width/2.f,0));
		price_node->addChild(price_label);
		price_type->setPositionX(-price_label->getContentSize().width/2.f);
	}
	
	r_label->addChild(c_label);
	r_label->addChild(price_node);
	
	
	CCScale9Sprite* c_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	continue_button = CCControlButton::create(r_label, c_back);
	continue_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ContinueContent::continueAction), CCControlEventTouchUpInside);
	continue_button->setPreferredSize(CCSizeMake(120,70));
	continue_button->setPosition(ccp(back_case->getContentSize().width/2.f,back_case->getContentSize().height/2.f-53));
	back_case->addChild(continue_button, 2);
	
	continue_button->setTouchPriority(touch_priority-1);
	
//	c_label->setPositionX(-(c_label->getContentSize().width + 1 + price_type->getContentSize().width + 2 + price_label->getContentSize().width)/2.f);
	
	if(continue_button2)
	{
		continue_button->setPosition(continue_button->getPosition() + ccp(61,0));
	}
}