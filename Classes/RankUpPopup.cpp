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
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "StyledLabelTTF.h"
#include "FiveRocksCpp.h"

RankUpPopup* RankUpPopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_rankup_func, function<void()> t_success_func, function<void()> t_fail_func, bool t_is_time_event)
{
	RankUpPopup* t_mup = new RankUpPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_rankup_func, t_success_func, t_fail_func, t_is_time_event);
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
		
		bool is_ok = rand()%1000 <= (mySGD->getRankUpBaseRate() + mySGD->getRankUpAddRate())*1000.f || is_time_event;
		
		if(!is_time_event)
		{
			string fiverocks_param2;
			if(is_ok)
				fiverocks_param2 = "success";
			else
				fiverocks_param2 = "fail";
			fiverocks::FiveRocksBridge::trackEvent("UseGem", "StrengthenCard", ccsf("Display %02d", myDSH->getIntegerForKey(kDSH_Key_showedCardRankUp)), fiverocks_param2.c_str());
			myDSH->setIntegerForKey(kDSH_Key_showedCardRankUp, 0);
		}
		
		if(!is_ok)
			mySGD->setRankUpAddRate(mySGD->getRankUpAddRate() + mySGD->getRankUpRateDistance());
		else
		{
			mySGD->setRankUpAddRate(0.f);
			
			if(!is_time_event)
			{
				int take_grade;
				take_grade = mySGD->getStageGrade();
//				if(mySGD->is_showtime && mySGD->is_exchanged)
//					take_grade = 4;
//				else if(mySGD->is_showtime)
//					take_grade = 3;
//				else if(mySGD->is_exchanged)
//					take_grade = 2;
//				else
//					take_grade = 1;
				
				if(take_grade == 2)
				{
//					mySGD->is_showtime = true;
//					mySGD->is_exchanged = false;
//					mySGD->setPercentage(1.f);
					mySGD->setStageGrade(3);
				}
				else if(take_grade == 3)
				{
//					mySGD->is_showtime = true;
//					mySGD->is_exchanged = true;
//					mySGD->setPercentage(1.f);
					mySGD->setStageGrade(4);
				}
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
				gray->setVisible(false);
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

void RankUpPopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_rankup_func, function<void()> t_success_func, function<void()> t_fail_func, bool t_is_time_event)
{
	is_menu_enable = false;
	is_time_event = t_is_time_event;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	rankup_func = t_rankup_func;
	success_func = t_success_func;
	fail_func = t_fail_func;
	
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
	back_in->setContentSize(CCSizeMake(251, 125));
	back_in->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(0,0));
	back_case->addChild(back_in);
	
	string title_str;
	if(is_time_event)
		title_str = myLoc->getLocalForKey(LK::kMyLocalKey_rankUpEventTitle);
	else
	{
		myDSH->setIntegerForKey(kDSH_Key_showedCardRankUp, myDSH->getIntegerForKey(kDSH_Key_showedCardRankUp)+1);
		title_str = myLoc->getLocalForKey(LK::kMyLocalKey_rankUpTitle);
	}
	
	KSLabelTTF* title_label = KSLabelTTF::create(title_str.c_str(), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(-85,back_case->getContentSize().height/2.f-35));
	back_case->addChild(title_label);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width-25,back_case->getContentSize().height-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  giveupAction(sender, CCControlEventTouchUpInside);
							  });
	back_case->addChild(close_button);
	
	
	string content_str;
	if(is_time_event)
		content_str = myLoc->getLocalForKey(LK::kMyLocalKey_rankUpEventContent);
	else
		content_str = myLoc->getLocalForKey(LK::kMyLocalKey_rankUpSubTitle);
	
	KSLabelTTF* sub_title_label = KSLabelTTF::create(content_str.c_str(), mySGD->getFont().c_str(), 12);
	sub_title_label->setPosition(ccp(back_case->getContentSize().width/2.f,back_case->getContentSize().height-57));
	back_case->addChild(sub_title_label);
	
	int take_grade;
	take_grade = mySGD->getStageGrade();
//	if(mySGD->is_showtime && mySGD->is_exchanged)
//		take_grade = 4-1;
//	else if(mySGD->is_showtime)
//		take_grade = 3;
//	else if(mySGD->is_exchanged)
//		take_grade = 2;
//	else
//		take_grade = 1;
	
	int t_card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_grade);
	string t_card_type = NSDS_GS(kSDS_CI_int1_category_s, t_card_number);
	if(t_card_type == "")
		t_card_type = "normal";
	
	CCSprite* recent_take_card = mySIL->getLoadedImg(ccsf("card%d_visible.png", t_card_number));/* mySIL->getUnsafeLoadedImg(CCString::createWithFormat("card%d_thumbnail.png",
																				t_card_number)->getCString());*/
	recent_take_card->setScale(0.8f*0.4f*0.5f);
	CCSprite* card_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", t_card_type.c_str())->getCString());
	card_case->setPosition(ccp(recent_take_card->getContentSize().width/2.f, recent_take_card->getContentSize().height/2.f));
	card_case->setScale(2.f/0.4f);
	recent_take_card->addChild(card_case);
	recent_take_card->setPosition(ccp(-55,10));
	m_container->addChild(recent_take_card);
	
	KSLabelTTF* recent_card_ment = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_recentTakeCard), mySGD->getFont().c_str(), 12);
	recent_card_ment->setColor(ccc3(255, 170, 0));
	recent_card_ment->enableOuterStroke(ccBLACK, 1.f);
	recent_card_ment->setPosition(ccp(-55,48));
	m_container->addChild(recent_card_ment);
	
	float distance = 15;
	CCPoint base_position = ccp(-55-(take_grade-1)/2.f*distance, -23);
	for(int i=0;i<take_grade;i++)
	{
		CCSprite* t_star = CCSprite::create("gage_star_gold.png");
		t_star->setPosition(base_position + ccp(i*distance, 0));
		m_container->addChild(t_star);
	}
	
	int after_grade;
	if(is_time_event)
	{
		after_grade = take_grade + mySGD->getTimeEventIntValue(kTimeEventType_card);
		if(after_grade > 4)
			after_grade = 4;
	}
	else
	{
		after_grade = take_grade+1;
	}
	
	CCSprite* next_take_card = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 86, 64));
	next_take_card->setColor(ccc3(100, 100, 100));
	next_take_card->setRotation(90);
	
	CCSprite* next_card_case = CCSprite::create(CCString::createWithFormat("cardsetting_minicase_%s.png", t_card_type.c_str())->getCString());
	next_card_case->setPosition(ccp(43, 32));
	next_take_card->addChild(next_card_case);
	next_take_card->setPosition(ccp(55,10));
	next_card_case->setRotation(-90);
	next_take_card->setScale(0.9f);
	m_container->addChild(next_take_card);
	
	base_position = ccp(55-(after_grade-1)/2.f*distance, -25);
	for(int i=0;i<after_grade;i++)
	{
		CCSprite* t_star = CCSprite::create("gage_star_gold.png");
		t_star->setPosition(base_position + ccp(i*distance, 0));
		m_container->addChild(t_star);
	}
	
	
	auto t_ccb = KS::loadCCBI<CCSprite*>(this, "startsetting_question.ccbi");
	
	question_img = t_ccb.first;
	question_img->setPosition(ccp(55, 10));
	m_container->addChild(question_img);
	
	question_manager = t_ccb.second;
	question_manager->setDelegate(this);
	
	
	rankup_rate_back = CCSprite::create("rankup_ratearrow.png");
	rankup_rate_back->setPosition(ccp(0,15));
	m_container->addChild(rankup_rate_back);
	
	KSLabelTTF* rate_ment = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankUpRate), mySGD->getFont().c_str(), 8);
	rate_ment->setColor(ccBLACK);
	rate_ment->disableOuterStroke();
	rate_ment->setPosition(ccp(rankup_rate_back->getContentSize().width/2.f-5, rankup_rate_back->getContentSize().height/2.f+12));
	rankup_rate_back->addChild(rate_ment);
	
	float rate_value = (mySGD->getRankUpBaseRate()+mySGD->getRankUpAddRate())*100.f;
	
	if(is_time_event)
		rate_value = 100.f;
	
	KSLabelTTF* rate_value_label = KSLabelTTF::create(CCString::createWithFormat("%.0f%%", rate_value)->getCString(), mySGD->getFont().c_str(), 18);
	rate_value_label->setColor(ccBLACK);
	rate_value_label->disableOuterStroke();
	rate_value_label->setPosition(ccp(rankup_rate_back->getContentSize().width/2.f-2, rankup_rate_back->getContentSize().height/2.f-5));
	rankup_rate_back->addChild(rate_value_label);
	
	
	
//	KSLabelTTF* giveup_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_giveup), mySGD->getFont().c_str(), 15);
//	CCScale9Sprite* giveup_back = CCScale9Sprite::create("achievement_cellback_normal.png", CCRectMake(0,0,47,47), CCRectMake(23, 23, 1, 1));
//	giveup_button = CCControlButton::create(giveup_label, giveup_back);
//	giveup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(RankUpPopup::giveupAction), CCControlEventTouchUpInside);
//	giveup_button->setPreferredSize(CCSizeMake(80,40));
//	giveup_button->setPosition(ccp(-87,-80));
//	m_container->addChild(giveup_button);
//	
//	giveup_button->setTouchPriority(touch_priority);
	
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	
	KSLabelTTF* rankup_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankup), mySGD->getFont().c_str(), 15);
	rankup_label->disableOuterStroke();
	rankup_label->setAnchorPoint(ccp(0,0.5));
	rankup_label->setPosition(ccp(0,0));
	CCScale9Sprite* price_back = CCScale9Sprite::create("gray_ellipse.png", CCRectMake(0,0,82,26), CCRectMake(40,12,2,2));
	price_back->setContentSize(CCSizeMake(82, 26));
	price_back->setPosition(ccp(rankup_label->getContentSize().width + price_back->getContentSize().width/2.f + 5, rankup_label->getContentSize().height/2.f));
	rankup_label->addChild(price_back);
	
	CCSprite* price_type = CCSprite::create("price_ruby_img.png");
	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-15,price_back->getContentSize().height/2.f));
	price_back->addChild(price_type);
	KSLabelTTF* price_label = KSLabelTTF::create(CCString::createWithFormat("%d", mySGD->getRankUpRubyFee())->getCString(), mySGD->getFont().c_str(), 15);
	price_label->disableOuterStroke();
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f+10,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	if(is_time_event)
	{
		price_label->setString(myLoc->getLocalForKey(LK::kMyLocalKey_free));
		CCSprite* event_img = CCSprite::create("puzzle_event.png");
		event_img->setPosition(price_label->getPosition() + ccp(15,16));
		price_back->addChild(event_img);
	}
	
	r_label->addChild(rankup_label);
	
	
	CCScale9Sprite* rankup_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	rankup_button = CCControlButton::create(r_label, rankup_back);
	rankup_button->addTargetWithActionForControlEvents(this, cccontrol_selector(RankUpPopup::rankupAction), CCControlEventTouchUpInside);
	rankup_button->setPreferredSize(CCSizeMake(170,44));
	rankup_button->setPosition(ccp(0,-88));
	m_container->addChild(rankup_button);
	
	rankup_label->setPositionX(-(rankup_label->getContentSize().width + 5 + price_back->getContentSize().width)/2.f);
	
	rankup_button->setTouchPriority(touch_priority);
	
	if(!is_time_event)
	{
		StyledLabelTTF* fail_rate_up_label = StyledLabelTTF::create(/*ccsf(*/myLoc->getLocalForKey(LK::kMyLocalKey_rankUpFailRateUp)/*, mySGD->getRankUpRateDistance()*100.f)*/, mySGD->getFont().c_str(), 10, 999, StyledAlignment::kCenterAlignment);
		fail_rate_up_label->setAnchorPoint(ccp(0.5f,0.5f));
		fail_rate_up_label->setPosition(rankup_button->getPosition() + ccp(0,40));
		m_container->addChild(fail_rate_up_label);
	}
	else
	{
		StyledLabelTTF* rank_up_event_sub_label = StyledLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_rankUpEventSubMent), mySGD->getFont().c_str(), 10, 999, StyledAlignment::kCenterAlignment);
		rank_up_event_sub_label->setAnchorPoint(ccp(0.5f,0.5f));
		rank_up_event_sub_label->setPosition(rankup_button->getPosition() + ccp(0,40));
		m_container->addChild(rank_up_event_sub_label);
	}
	
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
}

void RankUpPopup::giveupAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		end_func(); removeFromParent();
	});
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
	
	if(is_time_event)
	{
		rankupAnimation();
	}
	else if(mySGD->getGoodsValue(kGoodsType_ruby) >= mySGD->getRankUpRubyFee())
	{
		LoadingLayer* t_loading = LoadingLayer::create(-9999, true);
		addChild(t_loading, 9999);
		
		int take_grade;
		take_grade = mySGD->getStageGrade();
		
		int t_card_number = NSDS_GI(mySD->getSilType(), kSDS_SI_level_int1_card_i, take_grade);
		
		mySGD->addChangeGoods("c_up", kGoodsType_begin, 0, "", ccsf("%d", t_card_number), "카드승급");
		
		mySGD->changeGoods([=](Json::Value result_data){
			t_loading->removeFromParent();
			if(result_data["result"]["code"] == GDSUCCESS)
			{
				rankupAnimation();
			}
			else if(result_data["result"]["code"] == GDPROPERTYISMINUS)
			{
				mySGD->clearChangeGoods();
				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought)), 9999);
				is_menu_enable = true;
			}
			else
			{
				mySGD->clearChangeGoods();
				addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
				is_menu_enable = true;
			}
		});
	}
	else
	{
		is_menu_enable = true;
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought)), 9999);
		
//		addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(-9999, GoodsType::kGoodsType_ruby, [=]()
//															{
//																ShopPopup* t_popup = ShopPopup::create();
//																t_popup->setScale(myDSH->screen_convert_rate);
//																t_popup->setShopCode(kSC_ruby);
//																t_popup->setCloseFunc([=]()
//																					  {
//																						  is_menu_enable = true;
//																					  });
//																addChild(t_popup);
//															}, [=]()
//															{
//																is_menu_enable = true;
//															}), 9999);
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
	{
		vector<CommandParam> t_list = mySGD->getChangeGoodsParam(json_selector(mySGD, StarGoldData::saveChangeGoodsTransaction));
		for(int i=0;i<t_list.size();i++)
		{
			command_list.push_back(t_list[i]);
		}
	}
	
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
		addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
		
		is_menu_enable = true;
	}
}