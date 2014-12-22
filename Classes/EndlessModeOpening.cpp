//
//  EndlessModeOpening.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 19..
//
//

#include "EndlessModeOpening.h"
#include "DataStorageHub.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "CommonButton.h"
#include "TouchSuctionLayer.h"
#include "FlagSelector.h"
#include "FormSetter.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "DownloadFile.h"
#include "StageImgLoader.h"
#include "StartSettingPopup.h"
#include "MainFlowScene.h"
#include "StyledLabelTTF.h"
#include "MyLocalization.h"
#include "RivalSelectPopup.h"
#include "CommonAnimation.h"
#include "LabelTTFMarquee.h"
#include "CCMenuLambda.h"
#include "TypingBox.h"
#include "PuzzleScene.h"
#include "HeartTime.h"

enum EndlessModeOpeningZorder
{
	kEndlessModeOpeningZorder_gray = 0,
	kEndlessModeOpeningZorder_back,
	kEndlessModeOpeningZorder_content
};
const static int kCellCase = 0x45451;
bool EndlessModeOpening::init()
{
	if(!CCLayer::init())
	{
		return false;
	}
	current_rank_percent = 100.f;
	animation_node1 = animation_node2 = nullptr;
	startFormSetter(this);
//	FormSetter::get()->start();
	
	refresh_ing_win_func = nullptr;
	CCLog("null kind_tutorial_pvp");
	kind_tutorial_pvp = nullptr;
	
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("flags.plist");
	
	touch_priority = -300;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kEndlessModeOpeningZorder_gray);
	
	gray->runAction(CCFadeTo::create(0.5f, 255));
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	suction->setTouchEnabled(true);
	addChild(suction);
	
	is_menu_enable = false;
	
	setMain();
	
	Json::Value param;
	param["memberID"] = myHSP->getMemberID();
	
	myHSP->command("getendlessrank", param, this,json_selector(this, EndlessModeOpening::resultGetEndlessRank));
	
	return true;
}

void EndlessModeOpening::setHideFinalAction(CCObject *t_final, SEL_CallFunc d_final)
{
	target_final = t_final;
	delegate_final = d_final;
}

void EndlessModeOpening::setMain()
{
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kEndlessModeOpeningZorder_back);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-10));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessMode), mySGD->getFont().c_str(), 14);
	title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,1.5f));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);

	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(main_case->getContentSize().width-20, main_case->getContentSize().height-12));
	close_button->setFunction([=](CCObject* sender)
							  {
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  myHSP->removeTarget(this);
								  myDSH->setPuzzleMapSceneShowType(kPuzzleMapSceneShowType_stage);
								  
								  addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
																		 {
																			 gray->setOpacity(255*t);
																		 }, [=](float t)
																		 {
																			 gray->setOpacity(0);
																			 if(target_final && delegate_final)
																				 (target_final->*delegate_final)();
																			 removeFromParent();
																		 }));
								  
									CommonAnimation::closePopup(this, main_case, gray, [=](){
										
									}, [=](){
//										end_func(); removeFromParent();
									});
//								  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
//									  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
//								  
//								  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t)
//								  {
//									  KS::setOpacity(main_case, t);
//									  if(t > 100)
//										{
//											this->n_ready_label2->setOpacity(100);
//											this->s_ready_label2->setOpacity(100);
//											
//											
//										}
//								  }, [=](int t)
//								  {
//									  KS::setOpacity(main_case, 0);
//									  this->n_ready_label2->setOpacity(0);
//									  this->s_ready_label2->setOpacity(0);
//										
//								  }));
							  });
	main_case->addChild(close_button, 2);
	
	
	CCScale9Sprite* tip_marquee_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	tip_marquee_back->setContentSize(CCSizeMake(278, 26));
	tip_marquee_back->setPosition(ccp(main_case->getContentSize().width*0.655f, main_case->getContentSize().height+2-23.5f));
	main_case->addChild(tip_marquee_back);
	
	LabelTTFMarquee* tipMaquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 278, 22, "");
	tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_endlessOpeningMarquee1));
	tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_endlessOpeningMarquee2));
	tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_endlessOpeningMarquee3));
	tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_endlessOpeningMarquee4));
	tipMaquee->addText(myLoc->getLocalForKey(LK::kMyLocalKey_endlessOpeningMarquee5));
	tipMaquee->setPosition(ccpFromSize(tip_marquee_back->getContentSize()/2.f));
	tipMaquee->startMarquee();
	tipMaquee->setAnchorPoint(ccp(0.5f,0.5f));
	tip_marquee_back->addChild(tipMaquee);
	
	CCSprite* tipBack = CCSprite::create("tabbutton_up.png");
	tipBack->setPosition(ccp(main_case->getContentSize().width*0.417f, main_case->getContentSize().height+2-25));
	main_case->addChild(tipBack);
	KSLabelTTF* tipLbl = KSLabelTTF::create("TIP", mySGD->getFont().c_str(), 14.f);
	tipLbl->enableOuterStroke(ccBLACK, 0.5f, 150, true);
//	tipLbl->disableOuterStroke();
	tipLbl->setPosition(ccpFromSize(tipBack->getContentSize()) / 2.f + ccp(0,1));
	tipBack->addChild(tipLbl);
	
	left_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	left_back->setContentSize(CCSizeMake(270, 212));
	left_back->setPosition(ccp(25+left_back->getContentSize().width/2.f, main_case->getContentSize().height*0.44f));
	main_case->addChild(left_back);
	
	
	KSLabelTTF* left_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessWeeklyranking), mySGD->getFont().c_str(), 12);
	left_title->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	left_title->setAnchorPoint(ccp(0.5f,0.5f));
	left_title->setPosition(ccp(40,left_back->getContentSize().height-15));
	left_back->addChild(left_title);
	
	loading_circle = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_circle->setPosition(ccp(left_back->getContentSize().width/2.f, left_back->getContentSize().height/2.f));
	left_back->addChild(loading_circle);
	
	rest_back = CCScale9Sprite::create("common_shadowgray.png", CCRectMake(0, 0, 34, 34), CCRectMake(8, 8, 18, 18));
	left_back->addChild(rest_back);
	
	rest_time_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_restTime), mySGD->getFont().c_str(), 11);
	rest_time_title->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	rest_time_title->setAnchorPoint(ccp(0.5f,0.5f));
	rest_back->addChild(rest_time_title);
	
	rest_time_value = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeHour), 0), mySGD->getFont().c_str(), 11);
	rest_time_value->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	rest_time_value->setVisible(false);
	rest_time_value->setAnchorPoint(ccp(0.5f,0.5f));
	rest_back->addChild(rest_time_value);
	
	rest_back->setContentSize(CCSizeMake(rest_time_title->getContentSize().width + rest_time_value->getContentSize().width + 20, 20));
	rest_back->setPosition(ccp(left_back->getContentSize().width-10-rest_back->getContentSize().width/2.f,left_back->getContentSize().height-15));
	
	rest_time_title->setPosition(ccp(10 + rest_time_title->getContentSize().width/2.f, rest_back->getContentSize().height/2.f));
	rest_time_value->setPosition(ccp(rest_back->getContentSize().width-10 - rest_time_value->getContentSize().width/2.f, rest_back->getContentSize().height/2.f));
	
	right_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	right_back->setContentSize(CCSizeMake(150, 160));
	right_back->setPosition(ccp(main_case->getContentSize().width-25-right_back->getContentSize().width/2.f, 149));
	main_case->addChild(right_back);
	
	right_info_node = CCNode::create();
	right_back->addChild(right_info_node);
	
	right_reward_node = CCNode::create();
	right_back->addChild(right_reward_node);
	right_reward_node->setVisible(false);
	
	right_top_menu = CCMenuLambda::create();
	right_top_menu->setPosition(ccp(0,0));
	right_back->addChild(right_top_menu);
	right_top_menu->setTouchPriority(touch_priority);
	
	CCSprite* n_info = CCSprite::create("endless_bt_down.png");
	KSLabelTTF* n_info_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_pvpInfo), mySGD->getFont().c_str(), 12);
	n_info_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	n_info_label->setPosition(ccpFromSize(n_info->getContentSize()/2.f));
	n_info->addChild(n_info_label);
	CCSprite* s_info = CCSprite::create("endless_bt_down.png");
	s_info->setColor(ccGRAY);
	KSLabelTTF* s_info_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_pvpInfo), mySGD->getFont().c_str(), 12);
	s_info_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	s_info_label->setPosition(ccpFromSize(s_info->getContentSize()/2.f));
	s_info->addChild(s_info_label);
	CCSprite* d_info = CCSprite::create("endless_bt_up.png");
	KSLabelTTF* d_info_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_pvpInfo), mySGD->getFont().c_str(), 12);
	d_info_label->setColor(ccc3(20, 50, 70));
	d_info_label->disableOuterStroke();
	d_info_label->setPosition(ccpFromSize(d_info->getContentSize()/2.f));
	d_info->addChild(d_info_label);
	
	info_item = CCMenuItemSpriteLambda::create(n_info, s_info, d_info, [=](CCObject* sender)
											   {
												   if(!is_menu_enable)
													   return;
												   
												   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
												   
												   right_info_node->setVisible(true);
												   right_reward_node->setVisible(false);
												   this->info_item->setEnabled(false);
												   this->reward_item->setEnabled(true);
											   });
	info_item->setPosition(ccp(right_back->getContentSize().width/2.f - 37, 145));
	right_top_menu->addChild(info_item);
	
	CCSprite* n_reward = CCSprite::create("endless_bt_down.png");
	KSLabelTTF* n_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_pvpReward), mySGD->getFont().c_str(), 12);
	n_reward_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	n_reward_label->setPosition(ccpFromSize(n_reward->getContentSize()/2.f));
	n_reward->addChild(n_reward_label);
	CCSprite* s_reward = CCSprite::create("endless_bt_down.png");
	s_reward->setColor(ccGRAY);
	KSLabelTTF* s_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_pvpReward), mySGD->getFont().c_str(), 12);
	s_reward_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	s_reward_label->setPosition(ccpFromSize(s_reward->getContentSize()/2.f));
	s_reward->addChild(s_reward_label);
	CCSprite* d_reward = CCSprite::create("endless_bt_up.png");
	KSLabelTTF* d_reward_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_pvpReward), mySGD->getFont().c_str(), 12);
	d_reward_label->setColor(ccc3(20, 50, 70));
	d_reward_label->disableOuterStroke();
	d_reward_label->setPosition(ccpFromSize(d_reward->getContentSize()/2.f));
	d_reward->addChild(d_reward_label);
	
	reward_item = CCMenuItemSpriteLambda::create(n_reward, s_reward, d_reward, [=](CCObject* sender)
											   {
												   if(!is_menu_enable)
													   return;
												   
												   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
												   
												   right_reward_node->setVisible(true);
												   right_info_node->setVisible(false);
												   this->reward_item->setEnabled(false);
												   this->info_item->setEnabled(true);
											   });
	reward_item->setPosition(ccp(right_back->getContentSize().width/2.f + 37, 145));
	right_top_menu->addChild(reward_item);
	
	info_item->setEnabled(false);
	
	CCSprite* graph_img = CCSprite::create("endless_graph.png");
	graph_img->setPosition(ccp(right_back->getContentSize().width/2.f, 100));
	right_info_node->addChild(graph_img);
	
	rank_percent_case = CCSprite::create("gameresult_rank_percent.png");
	rank_percent_case->setAnchorPoint(ccp(0.5f,1));
	rank_percent_case->setPosition(ccp(graph_img->getContentSize().width,7));
	graph_img->addChild(rank_percent_case);
	
	percent_label = KSLabelTTF::create("", mySGD->getFont().c_str(), 12);
	percent_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	percent_label->setOpacity(0);
	percent_label->setPosition(ccp(rank_percent_case->getContentSize().width/2.f+1,
																 rank_percent_case->getContentSize().height/2.f-3));
	rank_percent_case->addChild(percent_label);
	
	
	record_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	record_back->setContentSize(CCSizeMake(right_back->getContentSize().width-5, 20));
	record_back->setPosition(ccp(right_back->getContentSize().width/2.f, 60.f));
	right_info_node->addChild(record_back);
	
	KSLabelTTF* record_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessInfoScore), mySGD->getFont().c_str(), 11);
	record_title->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	record_title->setAnchorPoint(ccp(0,0.5f));
	record_title->setPosition(ccp(5, record_back->getContentSize().height/2.f));
	record_back->addChild(record_title);
	
	
	CCScale9Sprite* highscore_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	highscore_back->setContentSize(CCSizeMake(right_back->getContentSize().width-5, 20));
	highscore_back->setPosition(ccp(right_back->getContentSize().width/2.f, 37.f));
	right_info_node->addChild(highscore_back);
	
	KSLabelTTF* highscore_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessHighScore), mySGD->getFont().c_str(), 11);
	highscore_title->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	highscore_title->setAnchorPoint(ccp(0,0.5f));
	highscore_title->setPosition(ccp(5, highscore_back->getContentSize().height/2.f));
	highscore_back->addChild(highscore_title);
	
	highscore_content = KSLabelTTF::create("", mySGD->getFont().c_str(), 11);
	highscore_content->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	highscore_content->setAnchorPoint(ccp(1,0.5f));
	highscore_content->setPosition(ccp(highscore_back->getContentSize().width-5,
																		 highscore_back->getContentSize().height/2.f));
	highscore_back->addChild(highscore_content);
	
	CCScale9Sprite* straight_back = CCScale9Sprite::create("common_deepgray.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	straight_back->setContentSize(CCSizeMake(right_back->getContentSize().width-5, 20));
	straight_back->setPosition(ccp(right_back->getContentSize().width/2.f, 14.f));
	right_info_node->addChild(straight_back);
	
	KSLabelTTF* straight_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessHighStraight), mySGD->getFont().c_str(), 11);
	straight_title->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	straight_title->setAnchorPoint(ccp(0,0.5f));
	straight_title->setPosition(ccp(5, straight_back->getContentSize().height/2.f));
	straight_back->addChild(straight_title);
	
	straight_content = KSLabelTTF::create("", mySGD->getFont().c_str(), 11);
	straight_content->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	straight_content->setAnchorPoint(ccp(1,0.5f));
	straight_content->setPosition(ccp(straight_back->getContentSize().width-5,
																		straight_back->getContentSize().height/2.f));
	straight_back->addChild(straight_content);
	
	ready_button = CommonButton::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessReady), 18.5f, CCSizeMake(156, 50), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), touch_priority);
	ready_button->setPosition(ccp(right_back->getContentSize().width/2.f,-28));
	right_back->addChild(ready_button);
	ready_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  if(kind_tutorial_pvp != nullptr)
								  {
									  CCLog("before kind_tutorial_pvp()");
									  kind_tutorial_pvp();
									  CCLog("after_kind_tutorial_pvp()");
								  }
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  ready_loading = LoadingLayer::create(-99999);
								  addChild(ready_loading, 99999);
								  
								  transaction_list.clear();
								  
								  Json::Value transaction_param;
								  transaction_param["memberID"] = myHSP->getSocialID();
								  transaction_list.push_back(CommandParam("starttransaction", transaction_param, [=](Json::Value result_data)
																		  {
//																			  CCLog("ready result : \n%s", GraphDogLib::JsonObjectToString(result_data).c_str());
																			  if(result_data["result"]["code"].asInt() != GDSUCCESS)
																			{
																				ready_loading->removeFromParent();
																				ready_loading = NULL;
																				
																				is_menu_enable = true;
																				
																				Json::Value getheart_result = result_data["list"]["001"];
																				if(getheart_result.get("isMinus", false).asBool())
																				{
																					mySGD->heartRefreshSuccess(getheart_result);
																					((MainFlowScene*)getParent())->heart_time->refreshHeartTime();
																					
																					addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-500, kGoodsType_money, [=]()
																																		{
																																			ShopPopup* t_shop = ShopPopup::create();
																																			t_shop->setHideFinalAction(this, callfunc_selector(EndlessModeOpening::failTransaction));
																																			t_shop->targetHeartTime(((MainFlowScene*)getParent())->heart_time);
																																			t_shop->setShopCode(kSC_heart);
																																			t_shop->setShopBeforeCode(kShopBeforeCode_puzzle);
																																			addChild(t_shop, kEndlessModeOpeningZorder_content+9);
																																		}, [=]()
																																		{
																																			if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
																																			{
																																				CCLog("before tutorial_fail_func()");
																																				tutorial_fail_func();
																																				CCLog("after tutorial_fail_func()");
																																			}
																																		}), 9999);
																				}
																				else
																				{
																					addChild(ASPopupView::getCommonNoti(-999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_endlessServerError)), 999);
																			  
																				  if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
																				  {
																					  CCLog("before tutorial_fail_func()");
																					  tutorial_fail_func();
																					  CCLog("after tutorial_fail_func()");
																				  }
																				}
																			}
																		  }));
								  
									Json::Value heart_param;
									heart_param["memberID"] = myHSP->getMemberID();
								  if(/*!(mySGD->endless_my_victory.getV() > 0 || */myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) != 1 && !mySGD->isTimeEvent(TimeEventType::kTimeEventType_heart))
										heart_param["use"] = true;
									transaction_list.push_back(CommandParam("getheart", heart_param, [=](Json::Value result_data)
																		  {
																			  if(result_data["result"]["code"].asInt() == GDSUCCESS)
																			  {
																				  mySGD->heartRefreshSuccess(result_data);
																				  ((MainFlowScene*)getParent())->heart_time->refreshHeartTime();
																			  }
																		  }));
								  
								  Json::Value param;
								  param["memberID"] = myHSP->getMemberID();
								  //																	  param["no"] = 34;
								  param["autoLevel"] = mySGD->getUserdataAutoLevel();
								  //								  param["highPiece"] = mySGD->getUserdataHighPiece();
								  param["win"] = mySGD->getUserdataEndlessIngWin();
									
									param["level"] = mySGD->getUserdataCharLevel();
								  transaction_list.push_back(CommandParam("getendlessplayriver", param, json_selector(this, EndlessModeOpening::resultGetEndlessPlayData)));
								  myHSP->command(transaction_list);
//								  myHSP->command("getendlessplayriver", param, this,json_selector(this, EndlessModeOpening::resultGetEndlessPlayData));
							  });
	
	if(mySGD->isTimeEvent(kTimeEventType_heart))
	{
		KSLabelTTF* t_label = ready_button->getTitleLabel();
		
		CCSprite* time_event_back = CCSprite::create("startsetting_event.png");
		time_event_back->setPosition(ccp(95.5f, 29.f));
		t_label->addChild(time_event_back);
		time_event_back->setScale(0.7f);
		KSLabelTTF* time_event_back_lbl = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_heartFree), mySGD->getFont().c_str(), 10.f);
		time_event_back_lbl->disableOuterStroke();
		time_event_back->addChild(time_event_back_lbl);
		time_event_back_lbl->setPosition(ccpFromSize(time_event_back->getContentSize()) / 2.f + ccp(3, -5.f));
	}
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){
		
	}, [=](){
		
		if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
		{
			CCNode* scenario_node = CCNode::create();
			addChild(scenario_node, 9999);
			
			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
			if(screen_scale_x < 1.f)
				screen_scale_x = 1.f;
			
			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
			
			
			CCNode* t_stencil_node = CCNode::create();
			
			
			CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
			t_clipping->setAlphaThreshold(0.1f);
			
			float change_scale = 1.f;
			CCPoint change_origin = ccp(0,0);
			if(screen_scale_x > 1.f)
			{
				change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
				change_scale = screen_scale_x;
			}
			if(screen_scale_y > 1.f)
				change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
			CCSize win_size = CCDirector::sharedDirector()->getWinSize();
			t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
			
			
			CCSprite* t_gray = CCSprite::create("back_gray.png");
			t_gray->setScaleX(screen_scale_x);
			t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
			t_gray->setOpacity(0);
			t_gray->setPosition(ccp(240,160));
			t_clipping->addChild(t_gray);
			
			t_clipping->setInverted(true);
			scenario_node->addChild(t_clipping, 0);
			
			
			CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
			ikaruga->setAnchorPoint(ccp(0,0));
			ikaruga->setPosition(ccp(240-240*screen_scale_x+ikaruga->getContentSize().width, 160-160*screen_scale_y));
			scenario_node->addChild(ikaruga, 1);
			
			TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
			typing_box->setHide();
			scenario_node->addChild(typing_box, 2);
			
			CCSprite* n_skip = CCSprite::create("kt_skip.png");
			CCSprite* s_skip = CCSprite::create("kt_skip.png");
			s_skip->setColor(ccGRAY);
			
			CCMenuLambda* skip_menu = CCMenuLambda::create();
			skip_menu->setPosition(ccp(240-240*screen_scale_x + 35, 160+160*screen_scale_y - 25 + 150));
			scenario_node->addChild(skip_menu, 3);
			skip_menu->setTouchPriority(-19999);
			skip_menu->setEnabled(false);
			
			CCMenuItemLambda* skip_item = CCMenuItemSpriteLambda::create(n_skip, s_skip, [=](CCObject* sender)
																		 {
																			 skip_menu->setEnabled(false);
																			 
																			 is_menu_enable = true;
																			 
																			 CCLog("init tutorial_success_func");
																			 this->tutorial_success_func = [=]()
																			 {
																				 CCNode* scenario_node = CCNode::create();
																				 addChild(scenario_node, 9999);
																				 
																				 CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
																				 float screen_scale_x = screen_size.width/screen_size.height/1.5f;
																				 if(screen_scale_x < 1.f)
																					 screen_scale_x = 1.f;
																				 
																				 float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
																				 
																				 
																				 CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
																				 ikaruga->setAnchorPoint(ccp(0,0));
																				 ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width+ikaruga->getContentSize().width*2.f/3.f, 160-160*screen_scale_y));
																				 scenario_node->addChild(ikaruga, 1);
																				 
																				 TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
																				 scenario_node->addChild(typing_box, 2);
																				 
																				 function<void()> end_func3 = [=]()
																				 {
																					 addChild(KSTimer::create(0.1f, [=]()
																											  {
																												  scenario_node->removeFromParent();
																											  }));
																				 };
																				 
																				 typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent56), end_func3);
																			 };
																			 
																			 CCLog("init tutorial_fail_func");
																			 this->tutorial_fail_func = [=]()
																			 {
																				 
																			 };
																			 
																			 addChild(KSTimer::create(0.1f, [=]()
																									  {
																											scenario_node->removeFromParent();
																									  }));
																		 });
			skip_menu->addChild(skip_item);
			
			typing_box->showAnimation(0.3f);
			
			
			function<void()> end_func2 = [=]()
			{
				t_stencil_node->removeChildByTag(2);
				t_clipping->removeChildByTag(2);
				
				typing_box->setTouchOffScrollAndButton();
				typing_box->setTouchSuction(false);
				typing_box->setVisible(false);
				
				CCScale9Sprite* t_stencil3 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
				t_stencil3->setContentSize(ready_button->getContentSize() + CCSizeMake(10, 10));
				t_stencil3->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + right_back->getPosition() - ccpFromSize(right_back->getContentSize()/2.f) + ready_button->getPosition());
				t_stencil_node->addChild(t_stencil3, 0, 3);
				
				CCSprite* t_arrow3 = CCSprite::create("kt_arrow_big.png");
				t_arrow3->setRotation(-90);
				t_arrow3->setPosition(t_stencil3->getPosition() + ccp(0, t_stencil3->getContentSize().height/2.f + 50));
				t_clipping->addChild(t_arrow3, 0, 3);
				
				CCMoveTo* t_up = CCMoveTo::create(0.4f, t_arrow3->getPosition() + ccp(0,30));
				CCMoveTo* t_down = CCMoveTo::create(0.4f, t_arrow3->getPosition());
				CCSequence* t_seq = CCSequence::create(t_up, t_down, NULL);
				
				CCRepeatForever* t_repeat = CCRepeatForever::create(t_seq);
				
				t_arrow3->runAction(t_repeat);
				
				is_menu_enable = true;
				
				CCLog("init tutorial_success_func");
				this->tutorial_success_func = [=]()
				{
					CCNode* scenario_node = CCNode::create();
					addChild(scenario_node, 9999);
					
					CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
					float screen_scale_x = screen_size.width/screen_size.height/1.5f;
					if(screen_scale_x < 1.f)
						screen_scale_x = 1.f;
					
					float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
					
					
					CCSprite* ikaruga = CCSprite::create("kt_cha_ikaruga_1.png");
					ikaruga->setAnchorPoint(ccp(0,0));
					ikaruga->setPosition(ccp(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f, 160-160*screen_scale_y));
					scenario_node->addChild(ikaruga, 1);
					
					TypingBox* typing_box = TypingBox::create(-9999, "kt_talkbox_purple_right.png", CCRectMake(0, 0, 85, 115), CCRectMake(40, 76, 23, 14), CCRectMake(40, 26, 23, 64), CCSizeMake(210, 60), ccp(225, 50));
					scenario_node->addChild(typing_box, 2);
					
					function<void()> end_func3 = [=]()
					{
						addChild(KSTimer::create(0.1f, [=]()
												 {
													 scenario_node->removeFromParent();
												 }));
					};
					
					typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent56), end_func3);
				};
			
				CCLog("init tutorial_fail_func");
				this->tutorial_fail_func = [=]()
				{
					
				};

				CCLog("init kind_tutorial_pvp");
				kind_tutorial_pvp = [=]()
				{
					skip_menu->setEnabled(false);
					scenario_node->removeFromParent();
					CCLog("inner null kind_tutorial_pvp");
					kind_tutorial_pvp = nullptr;
				};
				
				TouchSuctionLayer* t_suction = TouchSuctionLayer::create(-9999);
				scenario_node->addChild(t_suction);
				t_suction->setTouchEnabled(true);
				
				t_suction->setNotSwallowRect(CCRectMake(t_stencil3->getPositionX()-t_stencil3->getContentSize().width/2.f, t_stencil3->getPositionY()-t_stencil3->getContentSize().height/2.f, t_stencil3->getContentSize().width, t_stencil3->getContentSize().height));
			};
			
			function<void()> end_func1 = [=]()
			{
				t_stencil_node->removeChildByTag(1);
				t_clipping->removeChildByTag(1);
				
				CCScale9Sprite* t_stencil2 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
				t_stencil2->setContentSize(left_back->getContentSize() + CCSizeMake(10, 10));
				t_stencil2->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + left_back->getPosition());
				t_stencil_node->addChild(t_stencil2, 0, 2);
				
				CCSprite* t_arrow2 = CCSprite::create("kt_arrow_big.png");
				t_arrow2->setScale(0.6f);
				t_arrow2->setRotation(0);
				t_arrow2->setPosition(t_stencil2->getPosition() + ccp(t_stencil2->getContentSize().width/2.f + 30, 0));
				t_clipping->addChild(t_arrow2, 0, 2);

				
				typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent55), end_func2);
			};
			
			scenario_node->addChild(KSGradualValue<float>::create(0.f, 1.f, 0.3f, [=](float t)
																  {
																	  t_gray->setOpacity(t*255);
																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																	  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
																  }, [=](float t)
																  {
																	  t_gray->setOpacity(255);
																	  ikaruga->setPositionX(240-240*screen_scale_x-ikaruga->getContentSize().width + ikaruga->getContentSize().width*2.f/3.f*t);
																	  skip_menu->setPositionY(160+160*screen_scale_y - 25 + 150 - 150*t);
																	  skip_menu->setEnabled(true);

																	  
																	  CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal1.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
																	  t_stencil1->setContentSize(right_back->getContentSize() + CCSizeMake(10, 10));
																	  t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + right_back->getPosition());
																	  t_stencil_node->addChild(t_stencil1, 0, 1);
																	  
																	  CCSprite* t_arrow1 = CCSprite::create("kt_arrow_big.png");
																	  t_arrow1->setScale(0.6f);
																	  t_arrow1->setRotation(-90);
																	  t_arrow1->setPosition(t_stencil1->getPosition() + ccp(0,t_stencil1->getContentSize().height/2.f + 30));
																	  t_clipping->addChild(t_arrow1, 0, 1);

																	  
																	  typing_box->startTyping(myLoc->getLocalForKey(LK::kMyLocalKey_scenarioMent54), end_func1);
																  }));
			
			
			
			
			
			
			
			
			
			
			
			
			
//			CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
//			float screen_scale_x = screen_size.width/screen_size.height/1.5f;
//			if(screen_scale_x < 1.f)
//				screen_scale_x = 1.f;
//			
//			CCNode* t_stencil_node = CCNode::create();
//			CCScale9Sprite* t_stencil1 = CCScale9Sprite::create("rank_normal.png", CCRectMake(0, 0, 40, 40), CCRectMake(19, 19, 2, 2));
//			t_stencil1->setContentSize(right_back->getContentSize() + CCSizeMake(10, 10));
//			t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + right_back->getPosition());
//			t_stencil_node->addChild(t_stencil1);
//
//			CCClippingNode* t_clipping = CCClippingNode::create(t_stencil_node);
//			t_clipping->setAlphaThreshold(0.1f);
//			
//			float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
//			
//			float change_scale = 1.f;
//			CCPoint change_origin = ccp(0,0);
//			if(screen_scale_x > 1.f)
//			{
//				change_origin.x = -(screen_scale_x-1.f)*480.f/2.f;
//				change_scale = screen_scale_x;
//			}
//			if(screen_scale_y > 1.f)
//				change_origin.y = -(screen_scale_y-1.f)*320.f/2.f;
//			CCSize win_size = CCDirector::sharedDirector()->getWinSize();
//			t_clipping->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
//			
//			
//			CCSprite* t_gray = CCSprite::create("back_gray.png");
//			t_gray->setScaleX(screen_scale_x);
//			t_gray->setScaleY(myDSH->ui_top/myDSH->screen_convert_rate/320.f);
//			t_gray->setOpacity(0);
//			t_gray->setPosition(ccp(240,160));
//			t_clipping->addChild(t_gray);
//			
//			CCSprite* t_arrow1 = CCSprite::create("main_tutorial_arrow1.png");
//			t_arrow1->setRotation(180);
//			t_arrow1->setPosition(t_stencil1->getPosition() + ccp(0,t_stencil1->getContentSize().height/2.f + 15));
//			t_clipping->addChild(t_arrow1);
//
//			StyledLabelTTF* t_ment1 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent2), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
//			t_ment1->setAnchorPoint(ccp(0.5f,0.f));
//			t_ment1->setPosition(t_arrow1->getPosition() + ccp(0, t_arrow1->getContentSize().height/2.f + 3));
//			t_clipping->addChild(t_ment1);
//			
//			t_suction = TouchSuctionLayer::create(touch_priority-99);
//			addChild(t_suction);
//			t_suction->setTouchEnabled(true);
//			
//			t_clipping->setInverted(true);
//			addChild(t_clipping, 9999);
//			
//			addChild(KSGradualValue<float>::create(0.f, 1.f, 1.f, [=](float t)
//												   {
//													   t_gray->setOpacity(t*255);
//												   }, [=](float t)
//												   {
//													   t_gray->setOpacity(255);
//													   this->is_menu_enable = true;
//													   
//													   this->t_suction->is_on_touch_began_func = true;
//													   this->t_suction->touch_began_func = [=]()
//													   {
//														   this->t_suction->is_on_touch_began_func = false;
//														   if(!this->is_menu_enable)
//															   return;
//														   
//														   this->is_menu_enable = false;
//														   
//														   t_arrow1->removeFromParent();
//														   t_ment1->removeFromParent();
//														   
//														   t_stencil1->setContentSize(left_back->getContentSize() + CCSizeMake(10, 10));
//														   t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + left_back->getPosition());
//														   
//														   CCSprite* t_arrow2 = CCSprite::create("main_tutorial_arrow1.png");
//														   t_arrow2->setRotation(-90);
//														   t_arrow2->setPosition(t_stencil1->getPosition() + ccp(t_stencil1->getContentSize().width/2.f + 15, 0));
//														   t_clipping->addChild(t_arrow2);
//
//														   StyledLabelTTF* t_ment2 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent3), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kLeftAlignment);
//														   t_ment2->setAnchorPoint(ccp(0.f,0.5f));
//														   t_ment2->setPosition(t_arrow2->getPosition() + ccp(t_arrow2->getContentSize().width/2.f + 3, 0));
//														   t_clipping->addChild(t_ment2);
//														   
//														   this->is_menu_enable = true;
//														   this->t_suction->touchFuncOn();
//														   this->t_suction->touch_began_func = [=]()
//														   {
//															   this->t_suction->is_on_touch_began_func = false;
//															   if(!this->is_menu_enable)
//																   return;
//															   
//															   
//															   this->is_menu_enable = false;
//															   
//															   t_arrow2->removeFromParent();
//															   t_ment2->removeFromParent();
//															   
//															   t_stencil1->setContentSize(ready_button->getContentSize() + CCSizeMake(10, 10));
//															   t_stencil1->setPosition(main_case->getPosition() - ccpFromSize(main_case->getContentSize()/2.f) + right_back->getPosition() - ccpFromSize(right_back->getContentSize()/2.f) + ready_button->getPosition());
//															   
//															   CCSprite* t_arrow3 = CCSprite::create("main_tutorial_arrow1.png");
//															   t_arrow3->setRotation(180);
//															   t_arrow3->setPosition(t_stencil1->getPosition() + ccp(0, t_stencil1->getContentSize().height/2.f + 15));
//															   t_clipping->addChild(t_arrow3);
//															   
//															   StyledLabelTTF* t_ment3 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent4), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
//															   t_ment3->setAnchorPoint(ccp(0.5f,0.f));
//															   t_ment3->setPosition(t_arrow3->getPosition() + ccp(0, t_arrow3->getContentSize().height/2.f + 3));
//															   t_clipping->addChild(t_ment3);
//															   
//															   this->is_menu_enable = true;
//															   this->t_suction->is_on_touch_began_func = true;
//															   this->t_suction->touch_began_func = [=]()
//															   {
//																   this->t_suction->is_on_touch_began_func = false;
//																   if(!this->is_menu_enable)
//																	   return;
//																   
//																   this->is_menu_enable = false;
//																   
//																   this->tutorial_success_func = [=]()
//																   {
//																	   t_arrow3->removeFromParent();
//																	   t_ment3->removeFromParent();
//																	   
//																	   t_stencil1->setContentSize(CCSizeMake(300, 130));
//																	   t_stencil1->setPosition(ccp(240,143));
//																	   
//																	   StyledLabelTTF* t_ment4 = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_endlessTutorialMent18), mySGD->getFont().c_str(), 15, 999, StyledAlignment::kCenterAlignment);
//																	   t_ment4->setAnchorPoint(ccp(0.5f,0.5f));
//																	   t_ment4->setPosition(ccp(240,143) + ccp(0, -80));
//																	   t_clipping->addChild(t_ment4);
//																	   
//																	   this->tutorial_success_func = [=]()
//																	   {
//																		   addChild(KSGradualValue<float>::create(1.f, 0.f, 1.f, [=](float t)
//																												  {
//																													  t_gray->setOpacity(255*t);
//																												  }, [=](float t)
//																												  {
//																													  t_gray->setOpacity(255*t);
//																													  
//																													  this->t_suction->removeFromParent();
//																													  
//																													  t_clipping->removeFromParent();
//																												  }));
//																	   };
//																   };
//																   
//																   this->tutorial_fail_func = [=]()
//																   {
//																	   this->t_suction->is_on_touch_began_func = true;
//																	   this->is_menu_enable = true;
//																   };
//
//																   ready_loading = LoadingLayer::create(-99999);
//																   addChild(ready_loading, 99999);
//																   
//																   Json::Value param;
//																   param["memberID"] = myHSP->getMemberID();
//																   //																	  param["no"] = 34;
//																   param["win"] = mySGD->getUserdataEndlessIngWin(); //mySGD->getUserdataAutoLevel();
//																   param["highPiece"] = mySGD->getUserdataHighPiece();
//																	 
//																   myHSP->command("getendlessplayriver", param, this,json_selector(this, EndlessModeOpening::resultGetEndlessPlayData));
//															   };
//														   };
//													   };
//												   }));
		}
		else
			is_menu_enable = true;
	});
}

void EndlessModeOpening::failTransaction()
{
	if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
	{
		CCLog("before tutorial_fail_func()");
		tutorial_fail_func();
		CCLog("after tutorial_fail_func()");
	}
}

void EndlessModeOpening::resultGetEndlessPlayData(Json::Value result_data)
{
	GraphDogLib::JsonToLog("getendlessplaydata : %s", result_data);
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->is_pvp_event = result_data["isEvent"].asBool();
		mySGD->pvp_event_title = result_data.get("eventMsg", "").asString();
		mySGD->resetReplayPlayingInfo();
		
		if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
		{
			string easy_play_data = R"({"as":0,"cg":0,"cs":0,"ds":0,"gt":150,"icm":false,"ics":false,"lbc":null,"lose":37,"md":[[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,196608,196608,196608,196608,196608,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]],"mt":[0],"os":0,"sd":[0],"st":[0],"ta":0.01499999966472387,"tg":0,"ts":[{"mi":0,"si":0},{"boss":[{"bpx":188.7735595703125,"bpy":328.9600524902344}],"cpx":161,"cpy":217,"mob":[{"x":67.68907928466797,"y":298.0100708007812}]},{"boss":[{"bpx":154.3245086669922,"bpy":321.8156127929688}],"cpx":161,"cpy":217,"mob":[{"x":54.15271759033203,"y":317.3423461914062}]},{"boss":[{"bpx":121.0160217285156,"bpy":310.5301513671875}],"cpx":161,"cpy":217,"mob":[{"x":40.00989151000977,"y":334.8093872070312}]},{"boss":[{"bpx":95.23295593261719,"bpy":295.490966796875}],"cpx":161,"cpy":217,"mob":[{"x":20.6778450012207,"y":321.2723999023438}]},{"boss":[{"bpx":106.7893981933594,"bpy":262.2425842285156}],"cpx":143,"cpy":219,"mob":[{"x":6.386994361877441,"y":306.707763671875}]},{"boss":[{"bpx":113.7460403442383,"bpy":230.3540344238281}],"cpx":121,"cpy":257,"id":true,"mob":[{"x":19.92340850830078,"y":287.37548828125}]},{"boss":[{"bpx":129.7234344482422,"bpy":221.6747436523438}],"cpx":137,"cpy":219,"mob":[{"x":33.45981216430664,"y":268.043212890625}]},{"boss":[{"bpx":112.8137741088867,"bpy":252.497314453125}],"cpx":137,"cpy":219,"mob":[{"x":46.99611282348633,"y":248.7109375}]},{"boss":[{"bpx":95.82832336425781,"bpy":283.3076477050781}],"cpx":137,"cpy":219,"mob":[{"x":57.15585708618164,"y":230.096435546875}]},{"boss":[{"bpx":78.37179565429688,"bpy":313.8701782226562}],"cpx":137,"cpy":219,"mob":[{"x":36.92659378051758,"y":217.94189453125}]},{"boss":[{"bpx":57.63263320922852,"bpy":329.7658081054688}],"cpx":113,"cpy":247,"mob":[{"x":16.69741821289062,"y":205.787353515625}]},{"boss":[{"bpx":29.94655227661133,"bpy":308.0441284179688}],"cpx":127,"cpy":293,"mob":[{"x":8.981722831726074,"y":190.3717041015625}]},{"boss":[{"bpx":5.879875659942627,"bpy":287.7563171386719}],"cpx":61,"cpy":293,"mob":[{"x":21.1366081237793,"y":170.1422119140625}]},{"boss":[{"bpx":13.40987110137939,"bpy":266.0831298828125}],"cpx":11,"cpy":277,"gi":-1,"id":true,"mob":[{"x":33.29150390625,"y":149.9127197265625}]}],"tts":0,"uic":0,"win":5})";
			
			Json::Value read_data;
			Json::Reader reader;
			reader.parse(easy_play_data, read_data);
			mySGD->setReplayPlayingInfo(read_data);
		}
		else if(!result_data["rival"]["playData"].isNull())
		{
			Json::Value read_data;
			Json::Reader reader;
			reader.parse(result_data["rival"]["playData"].asString(), read_data);
			mySGD->setReplayPlayingInfo(read_data);
		}
		
		mySGD->dummy_list = result_data["dummy"];
		
		mySGD->endless_memberID = result_data["rival"]["memberID"].asInt64();
		
		mySGD->endless_score = result_data["rival"]["score"].asInt();
		mySGD->endless_regDate = result_data["rival"]["regDate"].asInt64();
		
		mySGD->replay_playing_info[mySGD->getReplayKey(kReplayKey_stageNo)] = result_data["stageInfo"]["realNo"].asInt();
		
		saveStageInfo(result_data["stageInfo"]);
	}
}

void EndlessModeOpening::saveStageInfo(Json::Value result_data)
{
	df_list.clear();
//	cf_list.clear();
	
	stage_number = result_data["no"].asInt();
	
	NSDS_SI(stage_number, kSDS_SI_puzzle_i, result_data["puzzle"].asInt(), false);
	NSDS_SI(stage_number, kSDS_SI_playtime_i, result_data["playtime"].asInt(), false);
	NSDS_SD(stage_number, kSDS_SI_scoreRate_d, result_data["scoreRate"].asDouble(), false);
	NSDS_SD(stage_number, kSDS_SI_scale_d, result_data["scale"].asDouble(), false);
//	NSDS_SB(stage_number, kSDS_SI_minigame_b, result_data["minigame"].asBool(), false);
	NSDS_SS(stage_number, kSDS_SI_type_s, result_data["type"].asString(), false);
	
	Json::Value t_mission = result_data["mission"];
	NSDS_SI(stage_number, kSDS_SI_missionType_i, t_mission["type"].asInt(), false);
	
	Json::Value t_option;
	if(!t_mission["option"].isObject())			t_option["key"]="value";
	else										t_option =t_mission["option"];
	
	if(t_mission["type"].asInt() == kCLEAR_bossLifeZero)
		NSDS_SI(stage_number, kSDS_SI_missionOptionEnergy_i, t_option["energy"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_subCumberCatch)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_bigArea)
	{
		NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
	}
	else if(t_mission["type"].asInt() == kCLEAR_itemCollect)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_perfect)
		NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_timeLimit)
		NSDS_SI(stage_number, kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_hellMode)
		NSDS_SI(stage_number, kSDS_SI_missionOptionSec_i, t_option["sec"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_percentage)
		NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_score)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["score"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_combo)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["combo"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_gold)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["gold"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_turns)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["turns"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_casting)
		NSDS_SI(stage_number, kSDS_SI_missionOptionCount_i, t_option["count"].asInt(), false);
	else if(t_mission["type"].asInt() == kCLEAR_littlePercent)
		NSDS_SI(stage_number, kSDS_SI_missionOptionPercent_i, t_option["percent"].asInt(), false);
	
	
	Json::Value shopItems = result_data["shopItems"];
	NSDS_SI(stage_number, kSDS_SI_shopItemsCnt_i, shopItems.size(), false);
	for(int i=0;i<shopItems.size();i++)
	{
		Json::Value t_item = shopItems[i];
		NSDS_SS(stage_number, kSDS_SI_shopItems_int1_currency_s, i, t_item["currency"].asString(), false);
		NSDS_SI(stage_number, kSDS_SI_shopItems_int1_type_i, i, t_item["type"].asInt(), false);
		NSDS_SI(stage_number, kSDS_SI_shopItems_int1_price_i, i, t_item["price"].asInt(), false);
		
		
		Json::Value t_option;
		if(!t_item["option"].isObject())				t_option["key"]="value";
		else											t_option =t_item["option"];
		
		if(t_item["type"].asInt() == kIC_attack)
			NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option.get("power",0).asInt(), false);
		else if(t_item["type"].asInt() == kIC_addTime)
			NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option.get("sec",0).asInt(), false);
		else if(t_item["type"].asInt() == kIC_fast)
			NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option.get("sec",0).asInt(), false);
		else if(t_item["type"].asInt() == kIC_silence)
			NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option.get("sec",0).asInt(), false);
		else if(t_item["type"].asInt() == kIC_doubleItem)
			NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_longTime)
			NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_baseSpeedUp)
			NSDS_SI(stage_number, kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
	}
	
	Json::Value defItems = result_data["defItems"];
	NSDS_SI(stage_number, kSDS_SI_defItemsCnt_i, defItems.size(), false);
	for(int i=0;i<defItems.size();i++)
	{
		Json::Value t_item = defItems[i];
		NSDS_SI(stage_number, kSDS_SI_defItems_int1_type_i, i, t_item["type"].asInt(), false);
		
		Json::Value t_option;
		if(!t_item["option"].isObject())				t_option["key"]="value";
		else											t_option =t_item["option"];
		
		if(t_item["type"].asInt() == kIC_attack)
			NSDS_SI(stage_number, kSDS_SI_itemOptionAttackPower_i, t_option["power"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_addTime)
			NSDS_SI(stage_number, kSDS_SI_itemOptionAddTimeSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_fast)
			NSDS_SI(stage_number, kSDS_SI_itemOptionFastSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_silence)
			NSDS_SI(stage_number, kSDS_SI_itemOptionSilenceSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_doubleItem)
			NSDS_SI(stage_number, kSDS_SI_itemOptionDoubleItemPercent_i, t_option["percent"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_longTime)
			NSDS_SI(stage_number, kSDS_SI_itemOptionLongTimeSec_i, t_option["sec"].asInt(), false);
		else if(t_item["type"].asInt() == kIC_baseSpeedUp)
			NSDS_SI(stage_number, kSDS_SI_itemOptionBaseSpeedUpUnit_i, t_option["unit"].asInt(), false);
	}
	
	Json::Value cards = result_data["cards"];
	NSDS_SI(stage_number, kSDS_SI_cardCount_i, cards.size(), false);
	for(int i=0;i<cards.size();i++)
	{
		Json::Value t_card = cards[i];
		NSDS_SI(t_card["piece"].asInt(), kSDS_SI_level_int1_card_i, t_card["grade"].asInt(), t_card["no"].asInt());
		NSDS_SI(kSDS_GI_serial_int1_cardNumber_i, t_card["serial"].asInt(), t_card["no"].asInt());
		if(NSDS_GI(kSDS_CI_int1_version_i, t_card["no"].asInt()) >= t_card["version"].asInt())
			continue;
		NSDS_SI(kSDS_CI_int1_serial_i, t_card["no"].asInt(), t_card["serial"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_version_i, t_card["no"].asInt(), t_card["version"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_rank_i, t_card["no"].asInt(), t_card["rank"].asInt(), false);
		NSDS_SI(kSDS_CI_int1_grade_i, t_card["no"].asInt(), t_card["grade"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_durability_i, t_card["no"].asInt(), t_card["durability"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_reward_i, t_card["no"].asInt(), t_card["reward"].asInt(), false);
		
//		NSDS_SI(kSDS_CI_int1_theme_i, t_card["no"].asInt(), 1, false);
//		NSDS_SI(kSDS_CI_int1_stage_i, t_card["no"].asInt(), t_card["piece"].asInt(), false);
		
//		Json::Value t_card_missile = t_card["missile"];
//		NSDS_SS(kSDS_CI_int1_missile_type_s, t_card["no"].asInt(), t_card_missile["type"].asString().c_str(), false);
//		NSDS_SI(kSDS_CI_int1_missile_power_i, t_card["no"].asInt(), t_card_missile["power"].asInt(), false);
//		NSDS_SI(kSDS_CI_int1_missile_dex_i, t_card["no"].asInt(), t_card_missile["dex"].asInt(), false);
//		NSDS_SD(kSDS_CI_int1_missile_speed_d, t_card["no"].asInt(), t_card_missile["speed"].asDouble(), false);
//		
//		NSDS_SS(kSDS_CI_int1_passive_s, t_card["no"].asInt(), t_card["passive"].asString().c_str(), false);
//		
//		Json::Value t_ability = t_card["ability"];
//		NSDS_SI(kSDS_CI_int1_abilityCnt_i, t_card["no"].asInt(), int(t_ability.size()), false);
//		for(int j=0;j<t_ability.size();j++)
//		{
//			Json::Value t_abil = t_ability[j];
//			NSDS_SI(kSDS_CI_int1_ability_int2_type_i, t_card["no"].asInt(), t_abil["type"].asInt(), t_abil["type"].asInt(), false);
//			
//			Json::Value t_option;
//			if(!t_abil["option"].isObject())                    t_option["key"]="value";
//			else												t_option =t_abil["option"];
//			
//			if(t_abil["type"].asInt() == kIC_attack)
//				NSDS_SI(kSDS_CI_int1_abilityAttackOptionPower_i, t_card["no"].asInt(), t_option["power"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_addTime)
//				NSDS_SI(kSDS_CI_int1_abilityAddTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_fast)
//				NSDS_SI(kSDS_CI_int1_abilityFastOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_silence)
//				NSDS_SI(kSDS_CI_int1_abilitySilenceOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_doubleItem)
//				NSDS_SI(kSDS_CI_int1_abilityDoubleItemOptionPercent_i, t_card["no"].asInt(), t_option["percent"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_longTime)
//				NSDS_SI(kSDS_CI_int1_abilityLongTimeOptionSec_i, t_card["no"].asInt(), t_option["sec"].asInt(), false);
//			else if(t_abil["type"].asInt() == kIC_baseSpeedUp)
//				NSDS_SI(kSDS_CI_int1_abilityBaseSpeedUpOptionUnit_i, t_card["no"].asInt(), t_option["unit"].asInt(), false);
//		}
		
		NSDS_SB(kSDS_CI_int1_haveAdult_b, t_card["no"].asInt(), t_card["haveAdult"].asBool(), false);
		NSDS_SI(kSDS_CI_int1_exp_i, t_card["no"].asInt(), t_card["exp"].asInt(), false);
		
		Json::Value t_imgInfo = t_card["imgInfo"];
		
//		bool is_add_cf = false;
		
		if(NSDS_GS(kSDS_CI_int1_imgInfo_s, t_card["no"].asInt()) != t_imgInfo["img"].asString())
		{
			// check, after download ----------
			DownloadFile t_df;
			t_df.size = t_imgInfo["size"].asInt();
			t_df.img = t_imgInfo["img"].asString().c_str();
			t_df.filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
			t_df.key = CCSTR_CWF("%d_imgInfo", t_card["no"].asInt())->getCString();
			df_list.push_back(t_df);
			// ================================
			
//			CopyFile t_cf;
//			t_cf.from_filename = t_df.filename.c_str();
//			t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//			cf_list.push_back(t_cf);
//			
//			is_add_cf = true;
		}
		
		Json::Value t_aniInfo = t_card["aniInfo"];
		NSDS_SB(kSDS_CI_int1_aniInfoIsAni_b, t_card["no"].asInt(), t_aniInfo["isAni"].asBool(), false);
		if(t_aniInfo["isAni"].asBool())
		{
			Json::Value t_detail = t_aniInfo["detail"];
			NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopLength_i, t_card["no"].asInt(), t_detail["loopLength"].asInt(), false);
			
			Json::Value t_loopSeq = t_detail["loopSeq"];
			for(int j=0;j<t_loopSeq.size();j++)
				NSDS_SI(kSDS_CI_int1_aniInfoDetailLoopSeq_int2_i, t_card["no"].asInt(), j, t_loopSeq[j].asInt(), false);
			
			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutWidth_i, t_card["no"].asInt(), t_detail["cutWidth"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutHeight_i, t_card["no"].asInt(), t_detail["cutHeight"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_aniInfoDetailCutLength_i, t_card["no"].asInt(), t_detail["cutLength"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionX_i, t_card["no"].asInt(), t_detail["positionX"].asInt(), false);
			NSDS_SI(kSDS_CI_int1_aniInfoDetailPositionY_i, t_card["no"].asInt(), t_detail["positionY"].asInt(), false);
			
			if(NSDS_GS(kSDS_CI_int1_aniInfoDetailImg_s, t_card["no"].asInt()) != t_detail["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = t_detail["size"].asInt();
				t_df.img = t_detail["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_aniInfo_detail_img", t_card["no"].asInt())->getCString();
				df_list.push_back(t_df);
				// ================================
			}
			
//			if(is_add_cf)
//			{
//				CopyFile t_cf = cf_list.back();
//				cf_list.pop_back();
//				t_cf.is_ani = true;
//				t_cf.cut_width = t_detail["cutWidth"].asInt();
//				t_cf.cut_height = t_detail["cutHeight"].asInt();
//				t_cf.position_x = t_detail["positionX"].asInt();
//				t_cf.position_y = t_detail["positionY"].asInt();
//				
//				t_cf.ani_filename = CCSTR_CWF("card%d_animation.png", t_card["no"].asInt())->getCString();
//				
//				cf_list.push_back(t_cf);
//			}
		}
		
		NSDS_SS(kSDS_CI_int1_script_s, t_card["no"].asInt(), t_card["script"].asString(), false);
		NSDS_SS(kSDS_CI_int1_profile_s, t_card["no"].asInt(), t_card["profile"].asString(), false);
		NSDS_SS(kSDS_CI_int1_name_s, t_card["no"].asInt(), t_card["name"].asString(), false);
		NSDS_SI(kSDS_CI_int1_mPrice_ruby_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_ruby)].asInt(), false);
		NSDS_SI(kSDS_CI_int1_mPrice_pass_i, t_card["no"].asInt(), t_card["mPrice"][mySGD->getGoodsTypeToKey(kGoodsType_pass6)].asInt(), false);
		
		NSDS_SI(kSDS_CI_int1_type_i, t_card["no"].asInt(), t_card["type"].asInt(), false);
		NSDS_SS(kSDS_CI_int1_category_s, t_card["no"].asInt(), t_card["category"].asString(), false);
		NSDS_SI(kSDS_CI_int1_level_i, t_card["no"].asInt(), t_card["level"].asInt(), false);
		
		int sound_cnt = t_card["sound"].size();
		NSDS_SI(kSDS_CI_int1_soundCnt_i, t_card["no"].asInt(), sound_cnt, false);
		for(int j=1;j<=sound_cnt;j++)
		{
			NSDS_SS(kSDS_CI_int1_soundType_int1_s, t_card["no"].asInt(), j, t_card["sound"][j-1].asString(), false);
		}
		
		NSDS_SI(kSDS_CI_int1_characterNo_i, t_card["no"].asInt(), t_card["characterNo"].asInt(), false);
		
		Json::Value t_silImgInfo = t_card["silImgInfo"];
		NSDS_SB(kSDS_CI_int1_silImgInfoIsSil_b, t_card["no"].asInt(), t_silImgInfo["isSil"].asBool(), false);
		if(t_silImgInfo["isSil"].asBool())
		{
			if(NSDS_GS(kSDS_CI_int1_silImgInfoImg_s, t_card["no"].asInt()) != t_silImgInfo["img"].asString())
			{
				// check, after download ----------
				DownloadFile t_df;
				t_df.size = t_silImgInfo["size"].asInt();
				t_df.img = t_silImgInfo["img"].asString().c_str();
				t_df.filename = CCSTR_CWF("card%d_invisible.png", t_card["no"].asInt())->getCString();
				t_df.key = CCSTR_CWF("%d_silImgInfo_img", t_card["no"].asInt())->getCString();
				df_list.push_back(t_df);
				// ================================
			}
		}
		
		Json::Value t_faceInfo = t_card["faceInfo"];
		if(!t_faceInfo.isNull() && t_faceInfo.asString() != "" && t_faceInfo.asString() != " ")
		{
			NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), true, false);
			NSDS_SS(kSDS_CI_int1_faceInfo_s, t_card["no"].asInt(), t_faceInfo["ccbiID"].asString() + ".ccbi", false);
			
			if(NSDS_GS(kSDS_CI_int1_faceInfoCcbi_s, t_card["no"].asInt()) != (t_faceInfo["ccbiID"].asString() + ".ccbi"))
			{
				DownloadFile t_df1;
				t_df1.size = t_faceInfo["size"].asInt();
				t_df1.img = t_faceInfo["ccbi"].asString().c_str();
				t_df1.filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
				t_df1.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoCcbi_s).c_str(), t_card["no"].asInt());
				df_list.push_back(t_df1);
			}
			
			if(NSDS_GS(kSDS_CI_int1_faceInfoPlist_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".plist"))
			{
				DownloadFile t_df2;
				t_df2.size = t_faceInfo["size"].asInt();
				t_df2.img = t_faceInfo["plist"].asString().c_str();
				t_df2.filename = t_faceInfo["imageID"].asString() + ".plist";
				t_df2.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPlist_s).c_str(), t_card["no"].asInt());
				df_list.push_back(t_df2);
			}
			
			if(NSDS_GS(kSDS_CI_int1_faceInfoPvrccz_s, t_card["no"].asInt()) != (t_faceInfo["imageID"].asString() + ".pvr.ccz"))
			{
				DownloadFile t_df3;
				t_df3.size = t_faceInfo["size"].asInt();
				t_df3.img = t_faceInfo["pvrccz"].asString().c_str();
				t_df3.filename = t_faceInfo["imageID"].asString() + ".pvr.ccz";
				t_df3.key = ccsf(mySDS->getRKey(kSDS_CI_int1_faceInfoPvrccz_s).c_str(), t_card["no"].asInt());
				df_list.push_back(t_df3);
			}
			
//			if(!is_add_cf)
//			{
//				CopyFile t_cf;
//				t_cf.from_filename = CCSTR_CWF("card%d_visible.png", t_card["no"].asInt())->getCString();
//				t_cf.to_filename = CCSTR_CWF("card%d_thumbnail.png", t_card["no"].asInt())->getCString();
//				cf_list.push_back(t_cf);
//				
//				is_add_cf = true;
//			}
//			
//			CopyFile t_cf = cf_list.back();
//			cf_list.pop_back();
//			t_cf.is_ccb = true;
//			t_cf.ccb_filename = t_faceInfo["ccbiID"].asString() + ".ccbi";
//			
//			cf_list.push_back(t_cf);
		}
		else
		{
			NSDS_SB(kSDS_CI_int1_haveFaceInfo_b, t_card["no"].asInt(), false, false);
		}
	}
	
//	NSDS_SI(stage_number, kSDS_SI_level_i, result_data["level"].asInt(), false);
	
	NSDS_SS(stage_number, kSDS_SI_boss_s, result_data["boss"].asString(), false);
	NSDS_SS(stage_number, kSDS_SI_junior_s, result_data["junior"].asString(), false);
	NSDS_SI(stage_number, kSDS_SI_autoBalanceTry_i, result_data["autoBalanceTry"].asInt(), false);
	
	if(df_list.size() > 0) // need download
	{
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		download_version = result_data["version"].asInt();
		ing_download_cnt = 1;
		ing_download_per = 0;
//		download_state = CCLabelBMFont::create(CCSTR_CWF("%.0f\t%d/%d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString(), "allfont.fnt");
//		download_state->setPosition(ccp(240,130));
//		addChild(download_state, kSID_Z_content);
		startDownload();
	}
	else
	{
		SDS_SI(kSDF_stageInfo, stage_number, "version", result_data["version"].asInt(), false);
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
		
		successGetStageInfo();
	}
}

void EndlessModeOpening::startDownload()
{
	CCLOG("%d : %s", ing_download_cnt, df_list[ing_download_cnt-1].filename.c_str());
	
	mySIL->downloadImg(df_list[ing_download_cnt-1].img, df_list[ing_download_cnt-1].size, df_list[ing_download_cnt-1].filename, this, callfunc_selector(EndlessModeOpening::successAction), this, callfunc_selector(EndlessModeOpening::failAction));
	
//	schedule(schedule_selector(StageInfoDown::downloadingAction));
}

void EndlessModeOpening::successAction()
{
	SDS_SS(kSDF_cardInfo, df_list[ing_download_cnt-1].key, df_list[ing_download_cnt-1].img, false);
	
	if(ing_download_cnt >= df_list.size())
	{
//		for(int i=0;i<cf_list.size();i++)
//		{
//			mySIL->removeTextureCache(cf_list[i].from_filename);
//			mySIL->removeTextureCache(cf_list[i].to_filename);
//			
//			CCSprite* target_img = new CCSprite();
//			target_img->initWithTexture(mySIL->addImage(cf_list[i].from_filename.c_str()));
//			target_img->setAnchorPoint(ccp(0,0));
//			
////			if(cf_list[i].is_ani)
////			{
////				CCSprite* ani_img = CCSprite::createWithTexture(mySIL->addImage(cf_list[i].ani_filename.c_str()), CCRectMake(0, 0, cf_list[i].cut_width, cf_list[i].cut_height));
////				ani_img->setPosition(ccp(cf_list[i].position_x, cf_list[i].position_y));
////				target_img->addChild(ani_img);
////			}
////			
////			if(cf_list[i].is_ccb)
////			{
////				CCSprite* face_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + cf_list[i].ccb_filename.c_str()).first;
////				face_img->setPosition(ccpFromSize(target_img->getContentSize()/2.f));
////				target_img->addChild(face_img);
////			}
//			
//			target_img->setScale(0.4f);
//			
//			CCRenderTexture* t_texture = new CCRenderTexture();
//			t_texture->initWithWidthAndHeight(320.f*target_img->getScaleX(), 430.f*target_img->getScaleY(), kCCTexture2DPixelFormat_RGBA8888, 0);
//			t_texture->setSprite(target_img);
//			t_texture->begin();
//			t_texture->getSprite()->visit();
//			t_texture->end();
//			
//			if(!(t_texture->saveToFileNoAlpha(cf_list[i].to_filename.c_str(), kCCImageFormatPNG)))
//               CCLOG("failed!!! card reduce : %s", cf_list[i].to_filename.c_str());
//			
//			t_texture->release();
//			target_img->release();
//			
//			if(i % 3 == 0)
//			{
//				CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//			}
//		}
		
		NSDS_SI(stage_number, kSDS_SI_version_i, download_version, false);
		mySDS->fFlush(stage_number, kSDS_SI_scale_d);
		mySDS->fFlush(kSDS_CI_int1_ability_int2_type_i);
//		download_state->setString(CCSTR_CWF("%.0f        %d  %d", 1.f*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		
		successGetStageInfo();
	}
	else
	{
		ing_download_cnt++;
		ing_download_per = 0.f;
//		download_state->setString(CCSTR_CWF("%.0f        %d  %d", ing_download_per*100.f, ing_download_cnt, int(df_list.size()))->getCString());
		startDownload();
	}
}

void EndlessModeOpening::failAction()
{
	addChild(ASPopupView::getCommonNoti(-99999, myLoc->getLocalForKey(LK::kMyLocalKey_endlessDownloadFail), [=]()
							   {
								   startDownload();
							   }, CCPointZero));
}

void EndlessModeOpening::successGetStageInfo()
{
	ready_loading->removeFromParent();
	ready_loading = NULL;
	
	if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
	{
		CCLog("before tutorial_success_func()");
		tutorial_success_func();
		CCLog("after tutorial_success_func()");
	}
	
	RivalSelectPopup* t_popup = RivalSelectPopup::create(touch_priority-100, [=](){is_menu_enable = true;}, [=]()
														 {
															 
//															 if(myDSH->getIntegerForKey(kDSH_Key_isShowEndlessModeTutorial) == 1)
//															 {
//																 tutorial_success_func();
//															 }
															 
															 addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
																									{
																										gray->setOpacity(255*t);
																									}, [=](float t)
																									{
																										gray->setOpacity(0);
																										mySGD->is_endless_mode = true;
//																										mySGD->endless_my_victory = 0;
																										mySD->setSilType(stage_number);
																										
																										StartSettingPopup* t_popup = StartSettingPopup::create();
																										t_popup->setHideFinalAction(getParent(), callfunc_selector(MainFlowScene::showEndlessOpening));
																										getParent()->addChild(t_popup, kMainFlowZorder_popup);
																										removeFromParent();
																									}));
															 
															 CommonAnimation::closePopup(this, main_case, nullptr, [=](){
																 
															 }, [=](){
//																 end_func(); removeFromParent();
															 });
//															 addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
//																 addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
//															 
//															 addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){
//																 KS::setOpacity(main_case, t);
//																 //		mySelection->setOpacity(0);
//															 }, [=](int t){
//																 KS::setOpacity(main_case, 0);
//																 //		mySelection->setOpacity(0);
//															 }));
														 });
	addChild(t_popup, 999);
//	
//	addChild(KSGradualValue<float>::create(1.f, 0.f, 0.2f, [=](float t)
//										   {
//											   gray->setOpacity(255*t);
//										   }, [=](float t)
//										   {
//											   gray->setOpacity(0);
//											   mySGD->is_endless_mode = true;
//											   mySGD->endless_my_victory = 0;
//											   mySD->setSilType(stage_number);
//											   
//											   StartSettingPopup* t_popup = StartSettingPopup::create();
//											   t_popup->setHideFinalAction(getParent(), callfunc_selector(MainFlowScene::showEndlessOpening));
//											   getParent()->addChild(t_popup, kMainFlowZorder_popup);
//											   removeFromParent();
//										   }));
//	
//	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(1.2f);
//		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){main_case->setScaleY(t);}, [=](float t){main_case->setScaleY(0.f);}));}));
//	
//	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){
//		KS::setOpacity(main_case, t);
////		mySelection->setOpacity(0);
//	}, [=](int t){
//		KS::setOpacity(main_case, 0);
////		mySelection->setOpacity(0);
//	}));
}

class RewardTableDelegator : public CCNode, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	Json::Value m_rewardData;
//	virtual CCSize tableCellSizeForIndex(CCTableView *table, unsigned int idx)
//	{
//		
//	}
	virtual CCSize cellSizeForTable(CCTableView *table)
	{
		return CCSizeMake(150, 38);
	}
	virtual unsigned int numberOfCellsInTableView(CCTableView *table)
	{
		return m_rewardData.size();
	}
	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell)
	{
		
	}
	virtual void scrollViewDidScroll(CCScrollView* view)
	{
		
	}
	virtual void scrollViewDidZoom(CCScrollView* view)
	{
		
	}

	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx)
	{
		CCTableViewCell* cell = new CCTableViewCell();
		cell->init();
		cell->autorelease();
		CCLog("idx = %d", idx);
		CCNode* A, *B, *C;
		A = B = C = nullptr;
		
		CCScale9Sprite* rewardBack2 = CCScale9Sprite::create("mainpopup_pupple2.png", CCRectMake(0, 0, 35, 35), CCRectMake(17, 17, 1, 1));
		rewardBack2->setPosition(ccp(0.f, 0.f));
		rewardBack2->setAnchorPoint(ccp(0.f, 0.0f));
		rewardBack2->setContentSize(CCSizeMake(148, 42));
		//			rewardBack2->setPosition(105,175);
		setFormSetter(rewardBack2);
		cell->addChild(rewardBack2);
		
		auto titleHanger = [=, &A](){
			std::string left1 = m_rewardData[idx]["left1"].asString();
			std::string left2 = m_rewardData[idx]["left2"].asString();
			//		left1 = KS::replaceAll(left1, "%", "%");
			//		left2 = KS::replaceAll(left2, "%", "%");
			StyledLabelTTF* reward2Title = StyledLabelTTF::create(
																														ccsf("<font color=#FFAA14 newline=13>%s</font><font color=#FFAA14>%s</font>",
																																 left1.c_str(), left2.c_str()),
																														mySGD->getFont().c_str(), 8,
																														999, StyledAlignment::kCenterAlignment);
			reward2Title->setAnchorPoint(ccp(0.5f, 0.5f));
			A = reward2Title;
			rewardBack2->addChild(reward2Title);
		};
		if(m_rewardData[idx]["left1"].asString() == "TOP")
		{
			int r = m_rewardData[idx]["left2"].asInt();
			if( r == 1 )
			{
				CCSprite* goldImg = CCSprite::create("rank_gold.png");
				A = goldImg;
				rewardBack2->addChild(goldImg);
			}
			else if(r == 2)
			{
				
				CCSprite* goldImg = CCSprite::create("rank_silver.png");
				A = goldImg;
				rewardBack2->addChild(goldImg);
			}
			else if(r == 3)
			{
				CCSprite* goldImg = CCSprite::create("rank_bronze.png");
				A = goldImg;
				rewardBack2->addChild(goldImg);
			}
			
			else
			{
				titleHanger();
			}
		}
		else
		{
			titleHanger();
		}
		
		std::string titleStr = m_rewardData[idx]["title"].asString();
		std::string contentStr = m_rewardData[idx]["content"].asString();
		
		//	contentStr = KS::replaceAll(contentStr, "%", "%");
		StyledLabelTTF* reward2Lbl = StyledLabelTTF::create(
																												ccsf("<font color=#FFAA14 size=11 newline=13>%s</font>"
																														 "<font color=999 size=9 strokesize=0.5 strokeopacity=150>%s</font>",
																														 titleStr.c_str(), contentStr.c_str()),
																												mySGD->getFont().c_str(),
																												15.f, 999, StyledAlignment::kLeftAlignment);
		rewardBack2->addChild(reward2Lbl);
		
		int rubyCount = m_rewardData[idx]["reward"][0]["count"].asInt();
		std::string rType = m_rewardData[idx]["reward"][0]["type"].asString();
		std::string rCnt =ccsf("x %d", rubyCount);
		std::string rubyImgFileName;
		float rScale= 1.f;
		if(rType=="r" || rType=="fr" || rType=="pr"){
			rScale= 0.5f;
			if(rubyCount >= 300)rubyImgFileName = "shop_ruby6.png";
			else if(rubyCount >= 200)rubyImgFileName = "shop_ruby5.png";
			else if(rubyCount >= 100)rubyImgFileName = "shop_ruby4.png";
			else if(rubyCount >= 30)rubyImgFileName = "shop_ruby3.png";
			else if(rubyCount >= 5)rubyImgFileName = "shop_ruby2.png";
			else if(rubyCount >= 1)rubyImgFileName = "shop_ruby1.png";
		}else if(rType=="g"){
			rScale= 0.5f;
//			if(rubyCount >= 10000)rubyImgFileName = "shop_gold6.png";
//			else if(rubyCount >= 5000)rubyImgFileName = "shop_gold5.png";
//			else if(rubyCount >= 1000)rubyImgFileName = "shop_gold4.png";
//			else if(rubyCount >= 500)rubyImgFileName = "shop_gold3.png";
//			else if(rubyCount >= 200)rubyImgFileName = "shop_gold2.png";
//			else if(rubyCount >= 1)rubyImgFileName = "shop_gold1.png";
			rubyImgFileName = "shop_gold2.png";
		}else if(rType=="p1"){
			rScale= 0.5f;
			if(rubyCount >= 100)rubyImgFileName = "shop_p1_6.png";
			else if(rubyCount >= 50)rubyImgFileName = "shop_p1_5.png";
			else if(rubyCount >= 30)rubyImgFileName = "shop_p1_4.png";
			else if(rubyCount >= 10)rubyImgFileName = "shop_p1_3.png";
			else if(rubyCount >= 5)rubyImgFileName = "shop_p1_2.png";
			else if(rubyCount >= 1)rubyImgFileName = "shop_p1_1.png";
		}else if(rType=="h"){
			rScale= 0.5f;
			if(rubyCount >= 300)rubyImgFileName = "shop_coin6.png";
			else if(rubyCount >= 200)rubyImgFileName = "shop_coin5.png";
			else if(rubyCount >= 100)rubyImgFileName = "shop_coin4.png";
			else if(rubyCount >= 30)rubyImgFileName = "shop_coin3.png";
			else if(rubyCount >= 5)rubyImgFileName = "shop_coin2.png";
			else if(rubyCount >= 1)rubyImgFileName = "shop_coin1.png";
		}else{
			if(rType=="cd")rCnt=ccsf("No.%d", rubyCount);
			rScale= 1.f;
			rubyImgFileName = CCString::createWithFormat("icon_%s.png",rType.c_str())->getCString();
		}
		
		CCSprite* reward2Img = CCSprite::create(rubyImgFileName.c_str());
		reward2Img->setScale(rScale);
		B = reward2Lbl;
		rewardBack2->addChild(reward2Img);
		KSLabelTTF* reward2Detail = KSLabelTTF::create(ccsf("x %d", rubyCount), mySGD->getFont().c_str(),
																									 10.f);
		reward2Detail->setScale(1/rScale);
		reward2Detail->setPosition(ccpFromSize(reward2Img->getContentSize()) / 2.f + ccp(0, -20 + 6));
		C = reward2Img;
		reward2Detail->setColor(ccc3(255, 255, 255));
		reward2Detail->enableOuterStroke(ccc3(0, 0, 0), 1.f);
		reward2Img->addChild(reward2Detail);
		
		if(A)
		{
			A->setPosition(ccp(20.f,19.0 + 2)); 			// dt (26.0,19.0)
		}
		if(B)
		{
			B->setPosition(ccp(36.5,32.5)); 			// dt (9.5,-6.0)
		}
		if(C)
		{
			C->setPosition(ccp(122.0,27.5 - 10.5)); 			// dt (-0.5,0.5)
		}
		return cell;
	}
	
};


void EndlessModeOpening::resultGetEndlessRank(Json::Value result_data)
{
	loading_circle->removeFromParent();
	loading_circle = NULL;
	
	GraphDogLib::JsonToLog("getendlessrank", result_data);

	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		RewardTableDelegator* rtd = new RewardTableDelegator();
		rtd->init();
		rtd->autorelease();
		right_reward_node->addChild(rtd);
		rtd->m_rewardData = result_data["rewardInfo"];
		
		CCRect t_table_rect = CCRectMake(1.f, 1.f, 150, 127);
		
//		CCSprite* t_table_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, t_table_rect.size.width, t_table_rect.size.height));
//		t_table_back->setOpacity(100);
//		t_table_back->setAnchorPoint(ccp(0,0));
//		t_table_back->setPosition(t_table_rect.origin);
//		right_reward_node->addChild(t_table_back);
		
		reward_table = CCTableView::create(rtd, t_table_rect.size);
		reward_table->setPosition(t_table_rect.origin); 			// dt (7.5, 9.0)
		
		reward_table->setTouchPriority(touch_priority - 2);
		right_reward_node->addChild(reward_table);
		reward_table->setDirection(kCCScrollViewDirectionVertical);
		
		reward_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		reward_table->setDelegate(rtd);


		rank_list.clear();
		
		Json::Value t_list = result_data["list"];
		int list_size = t_list.size();
		for(int i=0;i<list_size;i++)
		{
			EndlessRankInfo t_rank;
			t_rank.memberID = t_list[i]["memberID"].asInt64();
			t_rank.regWeek = t_list[i]["regWeek"].asInt();
			t_rank.score = t_list[i]["score"].asInt();
			t_rank.nick = t_list[i]["nick"].asString();
			t_rank.level = t_list[i]["level"].asInt();
			t_rank.flag = t_list[i]["flag"].asString();
			t_rank.victory = t_list[i]["victory"].asInt();
			t_rank.regDate = t_list[i]["regDate"].asInt64();
			t_rank.data = t_list[i].get("data", "").asString();
			
			rank_list.push_back(t_rank);
		}
		
		myscore = result_data["myscore"].asInt();
		victory = result_data["victory"].asInt();
		alluser = result_data["alluser"].asInt();
		myrank = result_data["myrank"].asInt();
		remainTime = result_data["remainTime"].asInt();
		mySGD->endless_my_level = result_data["level"].asInt();
		
		if(!result_data["myInfo"].isNull())
		{
			mySGD->endless_my_high_victory = result_data["myInfo"].get("victory", 0).asInt();
			mySGD->endless_my_lose = result_data["myInfo"].get("lose", 0).asInt();
			mySGD->endless_my_high_score = result_data["myInfo"].get("score", 0).asInt();
			mySGD->endless_my_win = result_data["myInfo"].get("win", 0).asInt();
			
			mySGD->endless_my_ing_win = result_data["myInfo"].get("ing_win", 0).asInt();
			mySGD->endless_my_victory = result_data["myInfo"].get("ing_win", 0).asInt();
			
			mySGD->setUserdataEndlessIngWin(mySGD->endless_my_victory.getV());
			
			if(refresh_ing_win_func != nullptr)
			{
				refresh_ing_win_func();
				refresh_ing_win_func = nullptr;
			}
			
			mySGD->endless_my_ing_score = result_data["myInfo"].get("ing_score", 0).asInt();
			
			if(mySGD->endless_my_ing_win.getV() > 0)
			{
				ready_button->setTitle(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_endlessReadyIngWin), mySGD->endless_my_ing_win.getV()+1)->getCString());
			}
		}
		else
		{
			mySGD->endless_my_high_victory = 0;
			mySGD->endless_my_lose = 0;
			mySGD->endless_my_high_score = 0;
			mySGD->endless_my_win = 0;
		}
		
		int remain_time = remainTime.getV();
		if(remain_time < 60)
			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeMinute), 0)->getCString());
		else if(remain_time < 60*60)
			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeMinute), remain_time/60)->getCString());
		else if(remain_time < 60*60*24)
			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeHour), remain_time/60/60)->getCString());
		else
			rest_time_value->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_restTimeDay), remain_time/60/60/24)->getCString());
		
		rest_time_value->setVisible(true);
		
		rest_back->setContentSize(CCSizeMake(rest_time_title->getContentSize().width + rest_time_value->getContentSize().width + 20, 20));
		rest_back->setPosition(ccp(left_back->getContentSize().width-10-rest_back->getContentSize().width/2.f,left_back->getContentSize().height-15));
		
		rest_time_title->setPosition(ccp(10 + rest_time_title->getContentSize().width/2.f, rest_back->getContentSize().height/2.f));
		rest_time_value->setPosition(ccp(rest_back->getContentSize().width-10 - rest_time_value->getContentSize().width/2.f, rest_back->getContentSize().height/2.f));
		
		
		
		CCRect table_rect = CCRectMake(6, left_back->getContentSize().height/2.f - 150/2.f - 30, 263, 150);
		
//		CCSprite* table_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, table_rect.size.width, table_rect.size.height));
//		table_back->setOpacity(100);
//		table_back->setAnchorPoint(ccp(0,0));
//		table_back->setPosition(table_rect.origin);
//		left_back->addChild(table_back);
		
		rank_table = CCTableView::create(this, table_rect.size);
		rank_table->setVerticalFillOrder(kCCTableViewFillTopDown);
		rank_table->setPosition(table_rect.origin);
		left_back->addChild(rank_table);
		rank_table->setTouchPriority(touch_priority);
		rank_table->setDelegate(this);
		
		
		CCScale9Sprite* list_cell_case = CCScale9Sprite::create("mainpopup_purple.png", CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
		list_cell_case->setContentSize(CCSizeMake(259, 31));
		list_cell_case->setPosition(ccp(5+list_cell_case->getContentSize().width/2.f,167));//list_cell_case->getContentSize().height/2.f+5));
		list_cell_case->setTag(kCellCase);
		left_back->addChild(list_cell_case);
		
		mySelection = CommonButton::create(CCSprite::create("whitepaper2.png", CCRectMake(0, 0, 249, 31)), touch_priority);
		mySelection->setPosition(ccp(10+list_cell_case->getContentSize().width/2.f,160));//list_cell_case->getContentSize().height/2.f+5));
		mySelection->setFunction([=](CCObject* obj){
			Json::Value param;
			param["endlessData"] = Json::Value();
			param["endlessData"]["victory"] = result_data["myInfo"].get("victory", 0).asInt();
			param["endlessData"]["lose"] = result_data["myInfo"].get("lose", 0).asInt();
			param["endlessData"]["win"] = result_data["myInfo"].get("win", 0).asInt();
			param["endlessData"]["score"] = result_data["myInfo"].get("score", 0).asInt();
			param["rank"] = result_data["myrank"].asInt();
			param["alluser"] = result_data["alluser"].asInt();
			param["level"] = result_data["level"].asInt();
			param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
			param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);
			putInformation(param);
			if(currentSelectedCell)
			{
				currentSelectedCell->removeFromParent();
//				currentSelectedCell = nullptr;
				currentSelectedIdx = -1;
			}
			currentSelectedCell = CellScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
			currentSelectedCell->setPosition(ccpFromSize(list_cell_case->getContentSize()/2.f));
			currentSelectedCell->setContentSize(list_cell_case->getContentSize() + CCSizeMake(7, 7));
			currentSelectedCell->destroy_func = [=]()
			{
				currentSelectedCell = NULL;
			};
			list_cell_case->addChild(currentSelectedCell);
			
			right_info_node->setVisible(true);
			right_reward_node->setVisible(false);
			this->info_item->setEnabled(false);
			this->reward_item->setEnabled(true);
		});
		left_back->addChild(mySelection);
		CCPoint rank_position = ccp(20,list_cell_case->getContentSize().height/2.f);
		int my_rank = myrank.getV();
		if(my_rank == 1)
		{
			CCSprite* gold_medal = CCSprite::create("rank_gold.png");
			//gold_medal->setScale(0.75f);
			gold_medal->setPosition(rank_position);
			list_cell_case->addChild(gold_medal);
			
		}
		else if(my_rank == 2)
		{
			CCSprite* silver_medal = CCSprite::create("rank_silver.png");
			//silver_medal->setScale(0.75f);
			silver_medal->setPosition(rank_position);
			list_cell_case->addChild(silver_medal);
		}
		else if(my_rank == 3)
		{
			CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
			//bronze_medal->setScale(0.75f);
			bronze_medal->setPosition(rank_position);
			list_cell_case->addChild(bronze_medal);
		}
		else
		{
			KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", my_rank)->getCString(), mySGD->getFont().c_str(), 10);
			rank_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
			rank_label->setPosition(ccp(35,rank_position.y+8));
			list_cell_case->addChild(rank_label);
		}
		
		string flag = myDSH->getStringForKey(kDSH_Key_flag);
		
		CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
		if(my_rank > 3)
			selectedFlagSpr->setPosition(ccp(35, rank_position.y-5));
		else
			selectedFlagSpr->setPosition(ccp(50, list_cell_case->getContentSize().height/2.f));
		selectedFlagSpr->setScale(0.8f);
		list_cell_case->addChild(selectedFlagSpr);
		
		
//		CCLabelTTF* t_nick_size = CCLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12);
//		if(t_nick_size->getContentSize().width > 85)
//		{
//			LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 85, 15, "");
//			nick_marquee->setSpace(30);
//			nick_marquee->addText(("<font color=#352990>"+myDSH->getStringForKey(kDSH_Key_nick)+"</font>").c_str());
//			nick_marquee->startMarquee();
//			nick_marquee->setFontSize(12.f);
//			nick_marquee->setAnchorPoint(ccp(0,0.5f));
//			nick_marquee->setPosition(ccp(70 - 3.5,list_cell_case->getContentSize().height/2.f));
//			list_cell_case->addChild(nick_marquee);
//		}
//		else
//		{
			KSLabelTTF* nick_label = KSLabelTTF::create(myDSH->getStringForKey(kDSH_Key_nick).c_str(), mySGD->getFont().c_str(), 12, CCSizeMake(85, 15), CCTextAlignment::kCCTextAlignmentLeft);
//			nick_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
			nick_label->setColor(ccc3(53, 41, 144));
			nick_label->setAnchorPoint(ccp(0,0.5f));
			nick_label->setPosition(ccp(70 - 3.5,list_cell_case->getContentSize().height/2.f));
			list_cell_case->addChild(nick_label);
//		}
		
		KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d", myscore.getV())->getCString()).c_str(), mySGD->getFont().c_str(), 9);
		score_label->setColor(ccc3(53, 41, 144));
		score_label->setAnchorPoint(ccp(1,0.5f));
		score_label->disableOuterStroke();
		score_label->setPosition(ccp(list_cell_case->getContentSize().width-10,list_cell_case->getContentSize().height/2.f-7));
		list_cell_case->addChild(score_label);
		
		StyledLabelTTF* victory_label =
				StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessHighStraightValue1), victory.getV()),
															 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kRightAlignment);
		victory_label->setAnchorPoint(ccp(1,0.5f));
		victory_label->setPosition(ccp(list_cell_case->getContentSize().width-10, list_cell_case->getContentSize().height/2.f+4));
		list_cell_case->addChild(victory_label);
		///////////////////////////////////
		
		Json::Value param;
		
		param["endlessData"] = Json::Value();
		param["endlessData"]["victory"] = result_data["myInfo"].get("victory", 0).asInt();
		param["endlessData"]["lose"] = result_data["myInfo"].get("lose", 0).asInt();
		param["endlessData"]["win"] = result_data["myInfo"].get("win", 0).asInt();
		param["endlessData"]["score"] = result_data["myInfo"].get("score", 0).asInt();
		param["rank"] = result_data["myrank"].asInt();
		param["alluser"] = result_data["alluser"].asInt();
		param["level"] = result_data["level"].asInt();
		param["flag"] = myDSH->getStringForKey(kDSH_Key_flag);
		param["nick"] = myDSH->getStringForKey(kDSH_Key_nick);

		putInformation(param);
		currentSelectedCell = CellScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
		currentSelectedCell->setPosition(ccpFromSize(list_cell_case->getContentSize()/2.f));
		currentSelectedCell->setContentSize(list_cell_case->getContentSize() + CCSizeMake(7, 7));
		currentSelectedCell->destroy_func = [=]()
		{
			currentSelectedCell = NULL;
		};
		list_cell_case->addChild(currentSelectedCell);
		currentSelectedIdx = -1;
	}
	else
	{
		
	}
	
	
}

unsigned int EndlessModeOpening::numberOfCellsInTableView(CCTableView *table)
{
	return rank_list.size();
}

CCTableViewCell* EndlessModeOpening::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	int my_rank = myrank.getV();
	
	string case_name;
	if(my_rank == idx+1)
	{
		case_name = "mainpopup_purple.png";
	}
	else
	{
		case_name = "rank_normal2.png";
	}
	
	CCScale9Sprite* list_cell_case = CCScale9Sprite::create(case_name.c_str(), CCRectMake(0, 0, 31, 31), CCRectMake(15, 15, 1, 1));
	list_cell_case->setContentSize(CCSizeMake(259, 31));
	list_cell_case->setPosition(ccp(list_cell_case->getContentSize().width/2.f,list_cell_case->getContentSize().height/2.f));
	cell->addChild(list_cell_case);
	
	CCPoint rank_position = ccp(20,list_cell_case->getContentSize().height/2.f);
	if(idx == 0)
	{
		CCSprite* gold_medal = CCSprite::create("rank_gold.png");
		//gold_medal->setScale(0.75f);
		gold_medal->setPosition(rank_position);
		list_cell_case->addChild(gold_medal);
		setFormSetter(gold_medal);
	}
	else if(idx == 1)
	{
		CCSprite* silver_medal = CCSprite::create("rank_silver.png");
		//silver_medal->setScale(0.75f);
		silver_medal->setPosition(rank_position);
		list_cell_case->addChild(silver_medal);
	}
	else if(idx == 2)
	{
		CCSprite* bronze_medal = CCSprite::create("rank_bronze.png");
		//bronze_medal->setScale(0.75f);
		bronze_medal->setPosition(rank_position);
		list_cell_case->addChild(bronze_medal);
	}
	else
	{
		KSLabelTTF* rank_label = KSLabelTTF::create(CCString::createWithFormat("%d", idx+1)->getCString(), mySGD->getFont().c_str(), 14);
		rank_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
		rank_label->setPosition(rank_position);//ccp(35,rank_position.y+8));
		list_cell_case->addChild(rank_label);
	}
	
	Json::Reader reader;
	Json::Value read_data;
	reader.parse(rank_list[idx].data.getV(), read_data);
	
	if(my_rank != idx+1)
	{
		int character_number = read_data.get("character", 1).asInt();
		if(character_number==0)character_number=1;
		
		int character_count = NSDS_GI(kSDS_GI_characterCount_i);
		int found_index = -1;
		for(int i=0;found_index == -1 && i<character_count;i++)
		{
			if(NSDS_GI(kSDS_GI_characterInfo_int1_no_i, i+1) == character_number)
			{
				found_index = i+1;
			}
		}
		
		if(found_index != -1)
		{
			CCNode* character_node = CCNode::create();
			character_node->setScale(0.8f);
			character_node->setPosition(ccp(50, 6));
			list_cell_case->addChild(character_node);
			
			auto character_ccb = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, found_index) + ".ccbi");
			CCSprite* character_img = character_ccb.first;
			character_img->setPosition(ccp(0,0));
			character_node->addChild(character_img);
			
			character_ccb.second->runAnimationsForSequenceNamed("move_down");
		}
	}
	
	string flag = rank_list[idx].flag.getV();
	
	CCSprite* selectedFlagSpr = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(flag).c_str());
	if(my_rank == idx+1)
	{
		selectedFlagSpr->setPosition(ccp(50,list_cell_case->getContentSize().height/2.f));
		selectedFlagSpr->setScale(0.8);
	}
	else
	{
		selectedFlagSpr->setPosition(ccp(50,7));
		selectedFlagSpr->setScale(0.5);
	}
	list_cell_case->addChild(selectedFlagSpr);
	
//	CCLabelTTF* t_nick_size = CCLabelTTF::create(rank_list[idx].nick.getV().c_str(), mySGD->getFont().c_str(), 12);
//	if(t_nick_size->getContentSize().width > 85)
//	{
//		LabelTTFMarquee* nick_marquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 85, 15, "");
//		nick_marquee->setSpace(30);
//		nick_marquee->addText(("<font color=#352990>"+rank_list[idx].nick.getV()+"</font>").c_str());
//		nick_marquee->startMarquee();
//		nick_marquee->setFontSize(12.f);
//		nick_marquee->setAnchorPoint(ccp(0,0.5f));
//		nick_marquee->setPosition(ccp(70 - 3.5,list_cell_case->getContentSize().height/2.f));
//		list_cell_case->addChild(nick_marquee);
//	}
//	else
//	{
		KSLabelTTF* nick_label = KSLabelTTF::create(rank_list[idx].nick.getV().c_str(), mySGD->getFont().c_str(), 12, CCSizeMake(85, 15), CCTextAlignment::kCCTextAlignmentLeft);
		nick_label->setColor(ccc3(53, 41, 144));
		nick_label->setAnchorPoint(ccp(0,0.5f));
		nick_label->disableOuterStroke();
		nick_label->setPosition(ccp(70 - 3.5,list_cell_case->getContentSize().height/2.f));
		list_cell_case->addChild(nick_label);
//	}
	
	KSLabelTTF* score_label = KSLabelTTF::create(KS::insert_separator(CCString::createWithFormat("%d",rank_list[idx].score.getV())->getCString()).c_str(), mySGD->getFont().c_str(), 9);
	score_label->setColor(ccc3(53, 41, 144));
	score_label->disableOuterStroke();
	score_label->setAnchorPoint(ccp(1,0.5f));
	score_label->setPosition(ccp(list_cell_case->getContentSize().width-10,list_cell_case->getContentSize().height/2.f-7));
	list_cell_case->addChild(score_label);
	
	StyledLabelTTF* victory_label =
	StyledLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_endlessHighStraightValue1), rank_list[idx].victory.getV()),
												 mySGD->getFont().c_str(), 12, 999, StyledAlignment::kRightAlignment);
	victory_label->setAnchorPoint(ccp(1,0.5f));
	victory_label->setPosition(ccp(list_cell_case->getContentSize().width-10,list_cell_case->getContentSize().height/2.f+4));
	list_cell_case->addChild(victory_label);
	
	if(idx == currentSelectedIdx)
	{
		if(currentSelectedCell)
		{
			currentSelectedCell->removeFromParent();
		}
		currentSelectedCell = CellScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
		currentSelectedCell->setPosition(ccpFromSize(list_cell_case->getContentSize()/2.f));
		currentSelectedCell->setContentSize(list_cell_case->getContentSize() + CCSizeMake(7,7));
		currentSelectedCell->destroy_func = [=]()
		{
			currentSelectedCell = NULL;
		};
		cell->addChild(currentSelectedCell);
	}
	return cell;
}

void EndlessModeOpening::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	Json::Value param;
	int tempIndex = clampf(cell->getIdx(), 0, rank_list.size() - 1);
	param["memberID"] = rank_list[tempIndex].memberID.getV();
	loading_right_circle = KS::loadCCBI<CCSprite*>(this, "loading.ccbi").first;
	loading_right_circle->setPosition(ccp(right_back->getContentSize().width/2.f, right_back->getContentSize().height/2.f));
	right_info_node->addChild(loading_right_circle);
	myHSP->command("getendlessrankinfo", param, this, [=](Json::Value obj){
		if(obj["result"]["code"].asInt() == GDSUCCESS)
		{
			if(loading_right_circle){
				loading_right_circle->removeFromParent();
				loading_right_circle = nullptr;
			}
			putInformation(obj);
		}
		else
		{
			
		}
	});
	currentSelectedIdx = tempIndex;
	if(currentSelectedCell)
	{
		currentSelectedCell->removeFromParent();
	}
	currentSelectedCell = CellScale9Sprite::create("common_select.png", CCRectMake(0, 0, 34, 34), CCRectMake(16, 16, 2, 2));
	currentSelectedCell->setPosition(ccpFromSize(CCSizeMake(259, 31)/2.f));
	currentSelectedCell->setContentSize(CCSizeMake(259, 31) + CCSizeMake(7,7)); 			// dt (6.0,9.5)
	currentSelectedCell->destroy_func = [=]()
	{
		currentSelectedCell = NULL;
	};
//	list_cell_case->setPosition(ccp(10+list_cell_case->getContentSize().width/2.f,180));//list_cell_case->getContentSize().height/2.f+5));
	cell->addChild(currentSelectedCell);
	
	right_info_node->setVisible(true);
	right_reward_node->setVisible(false);
	this->info_item->setEnabled(false);
	this->reward_item->setEnabled(true);
}
void EndlessModeOpening::putInformation(Json::Value info)
{
	KS::KSLog("%", info);

	int win_count = info["endlessData"]["win"].asInt();
	int lose_count = info["endlessData"]["lose"].asInt();
	int win_rate = 0;
	if(win_count + lose_count != 0)
	{
		win_rate = 100.f*win_count/(win_count + lose_count);
	}
	
	if(record_content)
	{
		record_content->removeFromParent();
		record_content = NULL;
	}
	record_content = StyledLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_endlessInfoScoreValue2), win_count, lose_count, win_rate)->getCString(),
																					mySGD->getFont().c_str(),
																					11.f, 0, StyledAlignment::kRightAlignment);
	record_content->setAnchorPoint(ccp(1,0.5f));
	record_content->setPosition(ccp(record_back->getContentSize().width-5, record_back->getContentSize().height/2.f));
	record_back->addChild(record_content);
	
	highscore_content->setString(KS::insert_separator(CCString::createWithFormat("%d", info["endlessData"]["score"].asInt())->getCString()).c_str());
	straight_content->setString(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_endlessHighStraightValue2), info["endlessData"]["victory"].asInt())->getCString());
	
	int	alluser = info["alluser"].asInt();
	int	myrank = info["rank"].asInt();
	float rank_percent = alluser == 0 ? 1.f : 1.f * myrank/alluser;
	
	if(animation_node1)
	{
		animation_node1->removeFromParent();
		animation_node1 = nullptr;
	}
	if(animation_node2)
	{
		animation_node2->removeFromParent();
		animation_node2 = nullptr;
	}
	
	if(myrank <= 50)
	{
		percent_label->setString(ccsf(getLocal(LK::kMyLocalKey_rankRewardRankValue), myrank));
	}
	else
	{
		addChild(animation_node1 = KSGradualValue<float>::create(current_rank_percent, rank_percent*100.f,
																 1.f, [=](float t){
																	 percent_label->setString(ccsf("%.0f%%", t));
																 }, [=](float t){
																	 percent_label->setString(ccsf("%.0f%%", t));
																	 animation_node1 = nullptr;
																 }));
	}
	
	addChild(animation_node2 = KSGradualValue<float>::create(rank_percent_case->getPositionX(),
																														 rank_percent_case->getParent()->getContentSize().width*rank_percent,
																														 1.f,
																														 [=](float t){
																															 rank_percent_case->setPositionX(t);
																														 }, [=](float t){
																															 rank_percent_case->setPositionX(t);
																															 animation_node2 = nullptr;
																														 }));
	
	CCDelayTime* t_delay2 = CCDelayTime::create(1.f);
	CCFadeTo* t_fade2 = CCFadeTo::create(0.5f, 255);
	CCSequence* t_seq2 = CCSequence::create(t_delay2, t_fade2, NULL);
	percent_label->runAction(t_seq2);
	current_rank_percent = rank_percent * 100.f;
	CCNode* flagnick = CCNode::create();
	if(right_flag)
	{
		right_flag->removeFromParent();
		right_flag = NULL;
	}
	
	right_flag = CCSprite::createWithSpriteFrameName(FlagSelector::getFlagString(info.get("flag", "kr").asString()).c_str());
	right_flag->setScale(0.75f);
	right_flag->setAnchorPoint(ccp(0.5f, 0.5f));
	flagnick->addChild(right_flag);
	
	if(right_nick)
	{
		right_nick->removeFromParent();
		right_nick = NULL;
	}
	right_nick = KSLabelTTF::create(info.get("nick", "").asString().c_str(),
																	mySGD->getFont().c_str(), 12.f);
	right_nick->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	right_nick->setAnchorPoint(ccp(0.5f, 0.5f));
	right_nick->setPosition(ccp(right_flag->getContentSize().width/2.f+2, 0));
	flagnick->addChild(right_nick);
	
	right_flag->setPosition(ccp(-right_nick->getContentSize().width/2.f-2, 0));
	
	float flagNickWidth = right_flag->getContentSize().width + 4.f + right_nick->getContentSize().width;
	
	
	flagnick->setPosition(ccpFromSize(right_back->getContentSize() / 2.f) + ccp(0,40));
	right_info_node->addChild(flagnick);
}

