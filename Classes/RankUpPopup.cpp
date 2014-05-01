//
//  RankUpPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 28..
//
//

#include "RankUpPopup.h"
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

RankUpPopup* RankUpPopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_rankup_func, function<void()> t_success_func, function<void()> t_fail_func)
{
	RankUpPopup* t_mup = new RankUpPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_rankup_func, t_success_func, t_fail_func);
	t_mup->autorelease();
	return t_mup;
}

void RankUpPopup::completedAnimationSequenceNamed (char const * name)
{
	string t_name = name;
	
	if(t_name == "Default Timeline")
	{
		if(is_menu_enable)
			return;
		
		bool is_ok = rand()%1000 <= (mySGD->getRankUpBaseRate() + mySGD->getRankUpAddRate())*1000.f;
		
		if(!is_ok)
			mySGD->setRankUpAddRate(mySGD->getRankUpAddRate() + mySGD->getRankUpRateDistance());
		else
		{
			mySGD->setRankUpAddRate(0.f);
			int take_grade = 1;
			if(mySGD->is_showtime && mySGD->is_exchanged)
				take_grade = 4;
			else if(mySGD->is_showtime)
				take_grade = 3;
			else if(mySGD->is_exchanged)
				take_grade = 2;
			else
				take_grade = 1;
			
			if(take_grade == 2)
			{
				mySGD->is_showtime = true;
				mySGD->is_exchanged = false;
				mySGD->setPercentage(1.f);
				mySGD->setStageGrade(3);
			}
			else if(take_grade == 3)
			{
				mySGD->is_showtime = true;
				mySGD->is_exchanged = true;
				mySGD->setPercentage(1.f);
				mySGD->setStageGrade(4);
			}
		}
		
		CCSprite* showtime_spin_light = CCSprite::create("showtime_spin_light.png");
		showtime_spin_light->setPosition(ccp(240,myDSH->ui_center_y));
		addChild(showtime_spin_light);
		
		showtime_spin_light->setScale(0.5f);
		showtime_spin_light->setOpacity(0);
		
		addChild(KSGradualValue<float>::create(0.f, 1.f, 4.f/30.f, [=](float t){
			showtime_spin_light->setOpacity(t*255);
		}, [=](float t){
			showtime_spin_light->setOpacity(255);
			addChild(KSTimer::create(47.f/30.f, [=](){
				addChild(KSGradualValue<float>::create(255, 0, 11.f/30.f, [=](float t){
					showtime_spin_light->setOpacity(t);
				}, [=](float t){
					showtime_spin_light->setOpacity(0);
				}));
			}));
		}));
		
		addChild(KSGradualValue<float>::create(0.5f, 5.f, 27.f/30.f, [=](float t){
			showtime_spin_light->setScale(t);
		}, [=](float t){
			showtime_spin_light->setScale(5.f);
		}));
		
		addChild(KSGradualValue<float>::create(0.f, 360.f, 62.f/30.f, [=](float t){
			showtime_spin_light->setRotation(t);
		}, [=](float t){
			showtime_spin_light->setRotation(360.f);
		}));
		
		CCSprite* back_light = CCSprite::create("whitePaper.png");
		back_light->setPosition(ccp(240,myDSH->ui_center_y));
		back_light->setScale(1.5f);
		back_light->setOpacity(0);
		addChild(back_light);
		
		addChild(KSTimer::create(18.f/30.f, [=](){
			addChild(KSGradualValue<float>::create(0.f, 255.f, 9.f/30.f, [=](float t){
				back_light->setOpacity(t);
			}, [=](float t){
				back_light->setOpacity(255);
				m_container->setVisible(false);
				
				if(is_ok)
					rankup_func();
				
				addChild(KSTimer::create(24.f/30.f, [=](){
					addChild(KSGradualValue<float>::create(255.f, 0.f, 11.f/30.f, [=](float t){
						back_light->setOpacity(t);
					}, [=](float t){
						back_light->setOpacity(0);
						
						addChild(KSTimer::create(0.1f, [=](){
							if(is_ok)
								success_func();
							else
								fail_func();
							removeFromParent();
						}));
					}));
				}));
			}));
		}));
		
		string result_title_filename;
		if(is_ok)
			result_title_filename = "rankup_result_success.png";
		else
			result_title_filename = "rankup_result_fail.png";
		
		CCSprite* result_title = CCSprite::create(result_title_filename.c_str());
		result_title->setPosition(ccp(240,myDSH->ui_center_y));
		result_title->setScale(1.3f);
		result_title->setOpacity(0);
		addChild(result_title);
		
		addChild(KSTimer::create(25.f/30.f, [=](){
			addChild(KSGradualValue<float>::create(0.f, 1.f, 5.f/30.f, [=](float t){
				result_title->setScale(1.3f-0.3f*t);
				result_title->setOpacity(t*255);
			}, [=](float t){
				result_title->setScale(1.f);
				result_title->setOpacity(255);
				
				addChild(KSTimer::create(9.f/30.f, [=](){
					addChild(KSGradualValue<float>::create(1.f, 1.5f, 5.f/30.f, [=](float t){
						result_title->setScale(t);
					}, [=](float t){
						result_title->setScale(1.5f);
					}));
				}));
				
				addChild(KSTimer::create(11.f/30.f, [=](){
					addChild(KSGradualValue<float>::create(255.f, 0.f, 3.f/30.f, [=](float t){
						result_title->setOpacity(t);
					}, [=](float t){
						result_title->setOpacity(0);
					}));
				}));
			}));
		}));
		
		
	}
}

void RankUpPopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_rankup_func, function<void()> t_success_func, function<void()> t_fail_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	rankup_func = t_rankup_func;
	success_func = t_success_func;
	fail_func = t_fail_func;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,myDSH->ui_center_y));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(350,280));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	CCSprite* title_label = CCSprite::create("rankup_title.png");
	title_label->setPosition(ccp(0,95));
	m_container->addChild(title_label);
	
	KSLabelTTF* sub_title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankUpSubTitle), mySGD->getFont().c_str(), 12);
	sub_title_label->setPosition(ccp(0,70));
	m_container->addChild(sub_title_label);
	
	int take_grade = 1;
	if(mySGD->is_showtime && mySGD->is_exchanged)
		take_grade = 4;
	else if(mySGD->is_showtime)
		take_grade = 3;
	else if(mySGD->is_exchanged)
		take_grade = 2;
	else
		take_grade = 1;
	
	
	CCSprite* recent_take_card = mySIL->getLoadedImg(CCString::createWithFormat("card%d_thumbnail.png",
																				NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_grade))->getCString());
	
	CCSprite* card_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase%d.png", take_grade)->getCString());
	card_case->setPosition(ccp(recent_take_card->getContentSize().width/2.f, recent_take_card->getContentSize().height/2.f));
	recent_take_card->addChild(card_case);
	recent_take_card->setPosition(ccp(-55,0));
	m_container->addChild(recent_take_card);
	
	KSLabelTTF* recent_card_ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_recentTakeCard), mySGD->getFont().c_str(), 12);
	recent_card_ment->setColor(ccc3(255, 170, 0));
	recent_card_ment->enableOuterStroke(ccBLACK, 1.f);
	recent_card_ment->setPosition(ccp(-55,38));
	m_container->addChild(recent_card_ment);
	
	
	
	CCSprite* next_take_card = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 86, 64));
	next_take_card->setColor(ccc3(100, 100, 100));
	next_take_card->setRotation(90);
	
	CCSprite* next_card_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase%d.png", take_grade+1)->getCString());
	next_card_case->setPosition(ccp(43, 32));
	next_take_card->addChild(next_card_case);
	next_take_card->setPosition(ccp(55,0));
	next_card_case->setRotation(-90);
	
	m_container->addChild(next_take_card);
	
	
	auto t_ccb = KS::loadCCBI<CCSprite*>(this, "startsetting_question.ccbi");
	
	question_img = t_ccb.first;
	question_img->setPosition(ccp(55, 0));
	m_container->addChild(question_img);
	
	question_manager = t_ccb.second;
	question_manager->setDelegate(this);
	
	
	rankup_rate_back = CCSprite::create("rankup_ratearrow.png");
	rankup_rate_back->setPosition(ccp(0,5));
	m_container->addChild(rankup_rate_back);
	
	KSLabelTTF* rate_ment = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankUpRate), mySGD->getFont().c_str(), 8);
	rate_ment->setColor(ccBLACK);
	rate_ment->setPosition(ccp(rankup_rate_back->getContentSize().width/2.f-5, rankup_rate_back->getContentSize().height/2.f+12));
	rankup_rate_back->addChild(rate_ment);
	
	KSLabelTTF* rate_value_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", (mySGD->getRankUpBaseRate()+mySGD->getRankUpAddRate())*100.f)->getCString(), mySGD->getFont().c_str(), 18);
	rate_value_label->setColor(ccBLACK);
	rate_value_label->setPosition(ccp(rankup_rate_back->getContentSize().width/2.f-2, rankup_rate_back->getContentSize().height/2.f-5));
	rankup_rate_back->addChild(rate_value_label);
	
	
	
	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_giveup), mySGD->getFont().c_str(), 13);
	CCScale9Sprite* giveup_back = CCScale9Sprite::create("subpop_gray.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	giveup_button = CCControlButton::create(giveup_label, giveup_back);
	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(RankUpPopup::giveupAction), CCControlEventTouchUpInside);
	giveup_button->setPreferredSize(CCSizeMake(90,60));
	giveup_button->setPosition(ccp(-55,-85));
	m_container->addChild(giveup_button);
	
	giveup_button->setTouchPriority(touch_priority);
	
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	
	KSLabelTTF* rankup_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_rankup), mySGD->getFont().c_str(), 13);
	rankup_label->setPosition(ccp(0,10));
	CCScale9Sprite* price_back = CCScale9Sprite::create("subpop_darkred.png", CCRectMake(0,0,30,30), CCRectMake(14,14,2,2));
	price_back->setContentSize(CCSizeMake(60, 30));
	price_back->setPosition(ccp(rankup_label->getContentSize().width/2.f, rankup_label->getContentSize().height/2.f-20));
	rankup_label->addChild(price_back);
	
	CCSprite* price_type = CCSprite::create("common_button_ruby.png");
	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-15,price_back->getContentSize().height/2.f));
	price_back->addChild(price_type);
	CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getRankUpRubyFee())->getCString(), mySGD->getFont().c_str(), 12);
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	r_label->addChild(rankup_label);
	
	
	CCScale9Sprite* rankup_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	rankup_button = CCControlButton::create(r_label, rankup_back);
	rankup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(RankUpPopup::rankupAction), CCControlEventTouchUpInside);
	rankup_button->setPreferredSize(CCSizeMake(90,60));
	rankup_button->setPosition(ccp(55,-85));
	m_container->addChild(rankup_button);
	
	rankup_button->setTouchPriority(touch_priority);
	
	
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

void RankUpPopup::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}

void RankUpPopup::rankupAnimation()
{
	addChild(KSGradualValue<float>::create(1.f, 0.f, 6.f/30.f, [=](float t){
		rankup_rate_back->setScaleY(t);
	}, [=](float t){
		rankup_rate_back->setScaleY(0.f);
		question_manager->runAnimationsForSequenceNamed("Default Timeline");
	}));
}

void RankUpPopup::rankupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
	
	if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getItemGachaReplayGoldFee())
	{
		LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
		addChild(t_loading, 9999);
		
		mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getRankUpRubyFee(), "승급");
		
		mySGD->changeGoods([=](Json::Value result_data){
			t_loading->removeFromParent();
			if(result_data["result"]["code"] == GDSUCCESS)
			{
				rankupAnimation();
			}
			else
			{
				mySGD->clearChangeGoods();
				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
				is_menu_enable = true;
			}
		});
	}
	else
	{
		t_popup = ASPopupView::create(touch_priority-200);
		
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, myDSH->ui_top));// /myDSH->screen_convert_rate));
		t_popup->setDimmedPosition(ccp(240, myDSH->ui_center_y));
		t_popup->setBasePosition(ccp(240, myDSH->ui_center_y));
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		
		getParent()->addChild(t_popup, getZOrder()+1);
		
		CCScale9Sprite* case_back = CCScale9Sprite::create("popup4_case_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6, 6, 144-6, 144-6));
		case_back->setPosition(CCPointZero);
		t_container->addChild(case_back);
		
		case_back->setContentSize(CCSizeMake(250, 150));
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("popup4_content_back.png", CCRectMake(0, 0, 150, 150), CCRectMake(6,6,144-6,144-6));
		content_back->setPosition(ccp(0,20));
		t_container->addChild(content_back);
		
		content_back->setContentSize(CCSizeMake(230,90));
		
		
		CCSprite* price_type_img1 = CCSprite::create("price_ruby_img.png");
		price_type_img1->setPosition(ccp(-55,35));
		t_container->addChild(price_type_img1);
		
		CCSprite* price_type_img2 = CCSprite::create("price_ruby_img.png");
		price_type_img2->setPosition(ccp(-88,17));
		t_container->addChild(price_type_img2);
		
		CCLabelTTF* price_value_label = CCLabelTTF::create(" 가 부족합니다.\n   10개를 구입하시겠습니까?", mySGD->getFont().c_str(), 16);
		price_value_label->setAnchorPoint(ccp(0,0.5f));
		price_value_label->setPosition(ccp(price_type_img1->getContentSize().width/2.f-35, price_type_img1->getContentSize().height/2.f-8));
		price_type_img1->addChild(price_value_label);
		
		
		CCScale9Sprite* n_cancel = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
		n_cancel->setContentSize(CCSizeMake(95,45));
		CCLabelTTF* n_cancel_label = CCLabelTTF::create("취소", mySGD->getFont().c_str(), 14);
		n_cancel_label->setPosition(ccp(n_cancel->getContentSize().width/2.f, n_cancel->getContentSize().height/2.f));
		n_cancel->addChild(n_cancel_label);
		
		CCScale9Sprite* s_cancel = CCScale9Sprite::create("popup4_orange_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
		s_cancel->setContentSize(CCSizeMake(95,45));
		s_cancel->setColor(ccGRAY);
		CCLabelTTF* s_cancel_label = CCLabelTTF::create("취소", mySGD->getFont().c_str(), 14);
		s_cancel_label->setPosition(ccp(s_cancel->getContentSize().width/2.f, s_cancel->getContentSize().height/2.f));
		s_cancel->addChild(s_cancel_label);
		
		CCMenuItemLambda* cancel_item = CCMenuItemSpriteLambda::create(n_cancel, s_cancel, [=](CCObject* sender)
																	   {
																		   AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		   
																		   t_popup->removeFromParent();
																		   is_menu_enable = true;
																	   });
		
		CCMenuLambda * cancel_menu = CCMenuLambda::createWithItem(cancel_item);
		cancel_menu->setPosition(ccp(-52,-48));
		t_container->addChild(cancel_menu);
		cancel_menu->setTouchPriority(t_popup->getTouchPriority()-2);
		
		
		CCScale9Sprite* n_buy = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
		n_buy->setContentSize(CCSizeMake(95,45));
		CCLabelTTF* n_buy_label = CCLabelTTF::create("임시 결재", mySGD->getFont().c_str(), 14);
		n_buy_label->setPosition(ccp(n_buy->getContentSize().width/2.f, n_buy->getContentSize().height/2.f));
		n_buy->addChild(n_buy_label);
		
		CCScale9Sprite* s_buy = CCScale9Sprite::create("popup4_green_button.png", CCRectMake(0,0,95,45), CCRectMake(6,6,89-6,39-6));
		s_buy->setContentSize(CCSizeMake(95,45));
		s_buy->setColor(ccGRAY);
		CCLabelTTF* s_buy_label = CCLabelTTF::create("임시 결재", mySGD->getFont().c_str(), 14);
		s_buy_label->setPosition(ccp(s_buy->getContentSize().width/2.f, s_buy->getContentSize().height/2.f));
		s_buy->addChild(s_buy_label);
		
		CCMenuItemLambda* buy_item = CCMenuItemSpriteLambda::create(n_buy, s_buy, [=](CCObject* sender)
																	{
																		CCLOG("buy!");
																		
																		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
																		
																		inapp_loading = LoadingLayer::create(-9999, true);
																		addChild(inapp_loading);
																		
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
																		mySGD->addChangeGoods(kGoodsType_ruby, NSDS_GI(kSDS_GI_shopRuby_int1_count_i, 0), "승급(IOS-인앱결재)", "", "", true);
																		mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getRankUpRubyFee(), "승급(IOS-소모)");
																		
																		mySGD->changeGoods([=](Json::Value result_data){
																			inapp_loading->removeFromParent();
																			if(result_data["result"]["code"].asInt() == GDSUCCESS)
																			{
																				t_popup->removeFromParent();
																				
																				rankupAnimation();
																			}
																			else
																			{
																				mySGD->clearChangeGoods();
																				
																				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
																				
																				is_menu_enable = true;
																			}
																		});
																		
																		
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
																		Json::Value param;
																		param["productid"] = mySGD->getInappProduct(0);
																		hspConnector::get()->purchaseProduct(param, Json::Value(), [=](Json::Value v){
																			//																				KS::KSLog("in-app test \n%", v);
																			if(v["issuccess"].asInt())
																			{
																				mySGD->addChangeGoods(kGoodsType_ruby, -mySGD->getRankUpRubyFee(), "승급");
																				requestItemDelivery();
																			}
																			else
																			{
																				inapp_loading->removeFromParent();
																				
																				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
																				
																				is_menu_enable = true;
																			}
																		});
#endif
																	});
		
		CCMenuLambda* buy_menu = CCMenuLambda::createWithItem(buy_item);
		buy_menu->setPosition(ccp(52,-48));
		t_container->addChild(buy_menu);
		buy_menu->setTouchPriority(t_popup->getTouchPriority()-2);
		
		t_container->setScale(0);
		CCScaleTo* t_scale = CCScaleTo::create(0.2f, 1.f);
		t_container->runAction(t_scale);
	}
	
//	mySGD->addChangeGoods(kGoodsType_gold, -mySGD->getItemGachaReplayGoldFee(), "아이템뽑기");
//	mySGD->changeGoods(json_selector(this, ItemGachaPopup::resultSaveUserData));
}

void RankUpPopup::requestItemDelivery()
{
	vector<CommandParam> command_list;
	
	Json::Value transaction_param;
	transaction_param["memberID"] = hspConnector::get()->getMemberID();
	command_list.push_back(CommandParam("starttransaction", transaction_param, [=](Json::Value result_data)
										{
											if(result_data["result"]["code"].asInt() == GDSUCCESS)
											{
												inapp_loading->removeFromParent();
												
												t_popup->removeFromParent();
												
												rankupAnimation();
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
		command_list.push_back(mySGD->getChangeGoodsParam(json_selector(mySGD, StarGoldData::saveChangeGoodsTransaction)));
	
	hspConnector::get()->command(command_list);
}

void RankUpPopup::resultSaveUserData(Json::Value result_data)
{
	CCLOG("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("save userdata success!!");
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
		rankup_button->setVisible(false);
		giveup_button->setVisible(false);
		
		question_manager->runAnimationsForSequenceNamed("Default Timeline");
	}
	else
	{
		CCLOG("missile upgrade fail!!");
		
		mySGD->clearChangeGoods();
		addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
		
		is_menu_enable = true;
	}
}