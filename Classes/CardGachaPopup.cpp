//
//  CardGachaPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 12. 9..
//
//

#include "CardGachaPopup.h"
#include "TouchSuctionLayer.h"
#include "DataStorageHub.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "AudioEngine.h"
#include "CardSettingPopup.h"
#include "StageImgLoader.h"
#include "KSLabelTTF.h"
#include "KsLocal.h"
#include "StyledLabelTTF.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"
#include "FiveRocksCpp.h"

CardGachaPopup* CardGachaPopup::create(int t_touch_priority)
{
	CardGachaPopup* t_popup = new CardGachaPopup();
	if(t_popup && t_popup->init())
	{
		t_popup->myInit(t_touch_priority);
		t_popup->autorelease();
		return t_popup;
	}
	else
	{
		CC_SAFE_DELETE(t_popup);
		return NULL;
	}
}

void CardGachaPopup::setHideFinalAction(CCObject* t_target, SEL_CallFunc t_delegate)
{
	target_final = t_target;
	delegate_final = t_delegate;
}

enum CardGachaPopupZorder
{
	kCardGachaPopupZorder_gray = 0,
	kCardGachaPopupZorder_main_case
};

enum CardGachaAnimationStep
{
	kCardGachaAnimationStep_ready = 0,
	kCardGachaAnimationStep_fasting,
	kCardGachaAnimationStep_slowing,
	kCardGachaAnimationStep_takeCardCreate,
	kCardGachaAnimationStep_positioning
};

void CardGachaPopup::myInit(int t_touch_priority)
{
	is_menu_enable = false;
	touch_priority = t_touch_priority;
	target_final = NULL;
	delegate_final = NULL;
	
	TouchSuctionLayer* suction = TouchSuctionLayer::create(touch_priority);
	suction->setTouchEnabled(true);
	addChild(suction);
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray, kCardGachaPopupZorder_gray);
	
	
	left_character = CCSprite::create("cardgacha_cha1.png");
	left_character->setAnchorPoint(ccp(0,0));
	left_character->setPosition(ccp(-(480*screen_scale_x - 480)/2.f - left_character->getContentSize().width, -(320*gray->getScaleY() - 320)/2.f));
	addChild(left_character, kCardGachaPopupZorder_gray);
	
	if(mySGD->getCardGachaMsgLeft() != "")
	{
		CCSprite* left_talk = CCSprite::create("gacha_talkbox.png");
		left_talk->setAnchorPoint(ccp(0,0));
		left_talk->setPosition(ccp(left_character->getContentSize().width*0.2f, left_character->getContentSize().height*0.8f));
		left_character->addChild(left_talk);
		
		StyledLabelTTF* left_talk_label = StyledLabelTTF::create(mySGD->getCardGachaMsgLeft().c_str(), mySGD->getFont().c_str(), 10, 0, StyledAlignment::kCenterAlignment);
		left_talk_label->setAnchorPoint(ccp(0.5f,0.5f));
		left_talk_label->setPosition(left_talk->getPosition() + ccpFromSize(left_talk->getContentSize()/2.f) + ccp(0,5));
		left_character->addChild(left_talk_label);
	}
	
	right_character = CCSprite::create("cardgacha_cha2.png");
	right_character->setAnchorPoint(ccp(1,0));
	right_character->setPosition(ccp(480+(480*screen_scale_x - 480)/2.f + right_character->getContentSize().width, -(320*gray->getScaleY() - 320)/2.f));
	addChild(right_character, kCardGachaPopupZorder_gray);
	
	if(mySGD->getCardGachaMsgRight() != "")
	{
		CCSprite* right_talk = CCSprite::create("gacha_talkbox.png");
		right_talk->setFlipX(true);
		right_talk->setAnchorPoint(ccp(1,0));
		right_talk->setPosition(ccp(right_character->getContentSize().width*0.8f, right_character->getContentSize().height*0.8f));
		right_character->addChild(right_talk);
		
		StyledLabelTTF* right_talk_label = StyledLabelTTF::create(mySGD->getCardGachaMsgRight().c_str(), mySGD->getFont().c_str(), 10, 0, StyledAlignment::kCenterAlignment);
		right_talk_label->setAnchorPoint(ccp(0.5f,0.5f));
		right_talk_label->setPosition(right_talk->getPosition() + ccp(-right_talk->getContentSize().width/2.f, right_talk->getContentSize().height/2.f + 5.f));
		right_character->addChild(right_talk_label);
	}
	
//	main_case = CCSprite::create("mainpopup2_back.png");
//	main_case->setPosition(ccp(240,160-14.f));
//	addChild(main_case, kCardGachaPopupZorder_main_case);
	
	main_case = CCNode::create();
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kCardGachaPopupZorder_main_case);
	
	CCSprite* title_back = CCSprite::create("cardgacha_title.png");
	title_back->setPosition(ccp(0,116));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_main = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardGacha), mySGD->getFont().c_str(), 25);
	title_main->enableOuterStroke(ccBLACK, 1.5f, 255, true);
	title_main->setGradientColor(ccc4(255, 255, 170, 255), ccc4(255, 140, 0, 255), ccp(0,-1));
	title_main->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,10));
	title_back->addChild(title_main);
	
	StyledLabelTTF* title_msg = StyledLabelTTF::create(mySGD->card_gacha_msg.getV().c_str(), mySGD->getFont().c_str(), 12, 999, StyledAlignment::kCenterAlignment);
	title_msg->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,-6));
	title_back->addChild(title_msg);
	
	moving_speed = 2.f;
	
	card_que.clear();
	
	card_type_size = mySGD->card_gacha_list.size();
	
	CCSprite* gacha_case_back = CCSprite::create("cardgacha_case_back.png");
	gacha_case_back->setPosition(ccp(0,3));
	main_case->addChild(gacha_case_back);
	
	CCSprite* gacha_case_center = CCSprite::create("cardgacha_case_center.png");
	gacha_case_center->setPosition(ccp(0,3));
	main_case->addChild(gacha_case_center);
	
	CCSprite* t_card_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", mySGD->card_gacha_list[rand()%card_type_size].getV()));
	t_card_img->setPosition(ccp(0,0));
	t_card_img->setScale(0.25f);
	
	card_que.push_back(t_card_img);
	
	float screen_scale_y = myDSH->ui_top/320.f/myDSH->screen_convert_rate;
	
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
	
	clipping_node = CCClippingNode::create(CCSprite::create("whitePaper.png", CCRectMake(0, 0, t_card_img->getContentSize().width*t_card_img->getScale() + 10, 320*gray->getScaleY() + 6.5f)));
	clipping_node->setRectYH(CCRectMake(change_origin.x, change_origin.y, win_size.width*change_scale, win_size.height*change_scale));
	clipping_node->setAlphaThreshold(0.1f);
	clipping_node->setPosition(ccp(0,-6.5f));
	main_case->addChild(clipping_node);
	
	clipping_node->addChild(t_card_img);
	
	
	mask_rect = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, t_card_img->getContentSize().width*t_card_img->getScale() + 10, t_card_img->getContentSize().height*t_card_img->getScale() + 10));
	mask_rect->setPosition(ccp(0,-6.5f));
	main_case->addChild(mask_rect);
	
	mask_bounding_rect = mask_rect->boundingBox();
	mask_bounding_rect.origin.x -= 5.f;
	mask_bounding_rect.size.width += 100.f;
	
	
	CCSprite* gacha_case_front = CCSprite::create("cardgacha_case_front.png");
	gacha_case_front->setPosition(ccp(0,3));
	main_case->addChild(gacha_case_front);
	
	CCSprite* light_img = KS::loadCCBI<CCSprite*>(this, "cardgacha_light.ccbi").first;
	light_img->setPosition(ccpFromSize(gacha_case_front->getContentSize()/2.f) + ccp(0,15));
	gacha_case_front->addChild(light_img);
	
	
	CCSprite* t_card_img2 = mySIL->getLoadedImg(ccsf("card%d_visible.png", mySGD->card_gacha_list[rand()%card_type_size].getV()));
	t_card_img2->setPosition(t_card_img->getPosition() + ccp(mask_rect->getContentSize().width, 0));
	t_card_img2->setScale(0.25f);
	clipping_node->addChild(t_card_img2);
	
	card_que.push_back(t_card_img2);
	
	CCSprite* t_card_img3 = mySIL->getLoadedImg(ccsf("card%d_visible.png", mySGD->card_gacha_list[rand()%card_type_size].getV()));
	t_card_img3->setPosition(t_card_img2->getPosition() + ccp(mask_rect->getContentSize().width, 0));
	t_card_img3->setScale(0.25f);
	clipping_node->addChild(t_card_img3);
	
	card_que.push_back(t_card_img3);
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	KSLabelTTF* button_label = KSLabelTTF::create("STOP", mySGD->getFont().c_str(), 15);
	button_label->enableOuterStroke(ccBLACK, 1.f, 255, true);
	button_label->setPosition(ccp(0,0));
	t_label->addChild(button_label);
	
	stop_button = CCControlButton::create(t_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	stop_button->setPreferredSize(CCSizeMake(92, 45));
	stop_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CardGachaPopup::myStopAction), CCControlEventTouchUpInside);
	stop_button->setPosition(ccp(0, -113));
	main_case->addChild(stop_button);
	
	stop_button->setTouchPriority(touch_priority-1);
	stop_button->setVisible(false);
	
	CCLabelTTF* t4_label = CCLabelTTF::create();
	KSLabelTTF* ok_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_onePercentGame10), mySGD->getFont().c_str(), 15);
	ok_label->enableOuterStroke(ccBLACK, 1.f, 255, true);
	ok_label->setPosition(ccp(0,0));
	t4_label->addChild(ok_label);
	
	ok_button = CCControlButton::create(t4_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	ok_button->setPreferredSize(CCSizeMake(92, 45));
	ok_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CardGachaPopup::okAction), CCControlEventTouchUpInside);
	ok_button->setPosition(ccp(0, -113));
	main_case->addChild(ok_button, 1);
	
	ok_button->setTouchPriority(touch_priority-1);
	ok_button->setVisible(false);
	
	CCLabelTTF* t2_label = CCLabelTTF::create();
	KSLabelTTF* one_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_oneGacha), mySGD->getFont().c_str(), 18);
	one_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	one_label->enableOuterStroke(ccBLACK, 1.f, 255, true);
	one_label->setPosition(ccp(-38,5));
	t2_label->addChild(one_label);
	
	price_back = CCScale9Sprite::create("common_in.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	price_back->setContentSize(CCSizeMake(70, 30));
	price_back->setPosition(ccp(32,0));
	t2_label->addChild(price_back);
	if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9) > 0)
	{
		CCSprite* price_type = CCSprite::create("icon_p9.png");
		price_type->setScale(0.8f);
		KSLabelTTF* price_value = KSLabelTTF::create(KS::insert_separator(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9)).c_str(), mySGD->getFont().c_str(), 14);
		price_type->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_value->getContentSize().width/2.f-4,0));
		price_value->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_type->getContentSize().width*price_type->getScale()/2.f-4, 0));
		price_back->addChild(price_type);
		price_back->addChild(price_value);
		
		sub_ment1 = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopGachaCardPass_countName_s).c_str(), mySGD->getFont().c_str(), 10);
		sub_ment1->enableOuterStroke(ccBLACK, 0.5f, 255, true);
		sub_ment1->setPosition(ccp(-38, -10));
		price_back->getParent()->addChild(sub_ment1);
	}
	else
	{
		string t_type = "icon_r.png";
		
		Json::Reader t_reader;
		Json::Value t_reward;
		t_reader.parse(NSDS_GS(kSDS_GI_shopGachaCardOnce_reward_s), t_reward);
		
		GoodsType goods_type = mySGD->getGoodsKeyToType(t_reward[0]["type"].asString());
		if(goods_type == kGoodsType_ruby)
			t_type = "icon_r.png";
		else if(goods_type == kGoodsType_gold)
			t_type = "icon_g.png";
		
		CCSprite* price_type = CCSprite::create(t_type.c_str());
		price_type->setScale(0.8f);
		KSLabelTTF* price_value = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopGachaCardOnce_priceName_s).c_str(), mySGD->getFont().c_str(), 14);
		price_type->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_value->getContentSize().width/2.f-4,0));
		price_value->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_type->getContentSize().width*price_type->getScale()/2.f-4, 0));
		price_back->addChild(price_type);
		price_back->addChild(price_value);
		
		sub_ment1 = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopGachaCardOnce_countName_s).c_str(), mySGD->getFont().c_str(), 10);
		sub_ment1->enableOuterStroke(ccBLACK, 0.5f, 255, true);
		sub_ment1->setPosition(ccp(-38, -10));
		price_back->getParent()->addChild(sub_ment1);
	}
	
	one_button = CCControlButton::create(t2_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	one_button->setPreferredSize(CCSizeMake(165, 55));
	one_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CardGachaPopup::oneAction), CCControlEventTouchUpInside);
	one_button->setPosition(ccp(-80, -113));
	main_case->addChild(one_button);
	
	one_button->setTouchPriority(touch_priority-1);
	
	CCLabelTTF* t3_label = CCLabelTTF::create();
	KSLabelTTF* five_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_fiveGacha), mySGD->getFont().c_str(), 18);
	five_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	five_label->enableOuterStroke(ccBLACK, 1.f, 255, true);
	five_label->setPosition(ccp(-38,5));
	t3_label->addChild(five_label);
	
	CCScale9Sprite* price_back2 = CCScale9Sprite::create("common_in.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	price_back2->setContentSize(CCSizeMake(72, 30));
	price_back2->setPosition(ccp(30,0));
	t3_label->addChild(price_back2);
	
	string t_type = "icon_r.png";
	
	Json::Reader t_reader;
	Json::Value t_reward;
	t_reader.parse(NSDS_GS(kSDS_GI_shopGachaCardDozen_reward_s), t_reward);
	
	GoodsType goods_type = mySGD->getGoodsKeyToType(t_reward[0]["type"].asString());
	if(goods_type == kGoodsType_ruby)
		t_type = "icon_r.png";
	else if(goods_type == kGoodsType_gold)
		t_type = "icon_g.png";
	
	CCSprite* price_type2 = CCSprite::create(t_type.c_str());
	price_type2->setScale(0.8f);
	KSLabelTTF* price_value2 = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopGachaCardDozen_priceName_s).c_str(), mySGD->getFont().c_str(), 14);
	price_type2->setPosition(ccpFromSize(price_back2->getContentSize()/2.f) + ccp(-price_value2->getContentSize().width/2.f-4,0));
	price_value2->setPosition(ccpFromSize(price_back2->getContentSize()/2.f) + ccp(price_type2->getContentSize().width*price_type2->getScale()/2.f-4, 0));
	price_back2->addChild(price_type2);
	price_back2->addChild(price_value2);
	
	KSLabelTTF* sub_ment = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopGachaCardDozen_countName_s).c_str(), mySGD->getFont().c_str(), 10);
	sub_ment->enableOuterStroke(ccBLACK, 0.5f, 255, true);
	sub_ment->setPosition(ccp(-38, -10));
	price_back2->getParent()->addChild(sub_ment);
	
	five_button = CCControlButton::create(t3_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	five_button->setPreferredSize(CCSizeMake(165, 55));
	five_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CardGachaPopup::fiveAction), CCControlEventTouchUpInside);
	five_button->setPosition(ccp(80, -113));
	main_case->addChild(five_button);
	
	five_button->setTouchPriority(touch_priority-1);
	
	
	step_cnt = kCardGachaAnimationStep_ready;
	
	close_button = CommonButton::createCloseButton(touch_priority-1);
	close_button->setPosition(ccp(185,-113));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
//								  CardSettingPopup* t_popup = CardSettingPopup::create();
//								  t_popup->setHideFinalAction(target_final, delegate_final);
//								  getParent()->addChild(t_popup, getZOrder());
//								  
//								  target_final = NULL;
								  
								  addChild(KSGradualValue<float>::create(-(480*screen_scale_x - 480)/2.f - left_character->getContentSize().width*0.2f, -(480*screen_scale_x - 480)/2.f - left_character->getContentSize().width, 0.25f, [=](float t_f)
																		 {
																			 left_character->setPositionX(t_f);
																		 }, [=](float t_f)
																		 {
																			 left_character->setPositionX(t_f);
																		 }));
								  
								  addChild(KSGradualValue<float>::create(480+(480*screen_scale_x - 480)/2.f + right_character->getContentSize().width*0.2f, 480+(480*screen_scale_x - 480)/2.f + right_character->getContentSize().width, 0.25f, [=](float t_f)
																		 {
																			 right_character->setPositionX(t_f);
																		 }, [=](float t_f)
																		 {
																			 right_character->setPositionX(t_f);
																		 }));
								  
								  CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
															  {
																  if(target_final && delegate_final)
																	  (target_final->*delegate_final)();
																  removeFromParent();
															  });
							  });
	main_case->addChild(close_button);
	
	addChild(KSGradualValue<float>::create(-(480*screen_scale_x - 480)/2.f - left_character->getContentSize().width, -(480*screen_scale_x - 480)/2.f - left_character->getContentSize().width*0.2f, 0.25f, [=](float t_f)
										   {
											   left_character->setPositionX(t_f);
										   }, [=](float t_f)
										   {
											   left_character->setPositionX(t_f);
										   }));
	
	addChild(KSGradualValue<float>::create(480+(480*screen_scale_x - 480)/2.f + right_character->getContentSize().width, 480+(480*screen_scale_x - 480)/2.f + right_character->getContentSize().width*0.2f, 0.25f, [=](float t_f)
										   {
											   right_character->setPositionX(t_f);
										   }, [=](float t_f)
										   {
											   right_character->setPositionX(t_f);
										   }));
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){}, [=]()
							   {
								   schedule(schedule_selector(CardGachaPopup::cardMoving));
								   is_menu_enable = true;
							   });
}

void CardGachaPopup::oneAction(CCObject *t_sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	loading_layer = LoadingLayer::create(touch_priority-100);
	addChild(loading_layer, 999);
	loading_layer->startLoading();
	
	Json::Value param;
	param["memberID"] = myHSP->getSocialID();
	param["gachaNo"] = mySGD->card_gacha_no.getV();
	param["number"] = myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV());
	param["count"] = 1;
	if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9) > 0)
		param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardPass_exchangeID_s);
	else
		param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardOnce_exchangeID_s);
	
	myHSP->command("getcardbygacha", param, json_selector(this, CardGachaPopup::resultOne));
}

void CardGachaPopup::resultOne(Json::Value result_data)
{
//	CCLog("resultOne : \n%s", result_data.toStyledString().c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		loading_layer->removeFromParent();
		
		myDSH->setIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV(), myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV()) + 1, true);
		
		close_button->setVisible(false);
		
		take_card_number_list.clear();
		
		Json::Value cardlist = result_data["cardlist"];
		for(int i=0;i<cardlist.size();i++)
		{
			if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9) <= 0)
				fiverocks::FiveRocksBridge::trackEvent("UseGem", "CardGacha", ccsf("%d", myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV())), ccsf("%d", cardlist[i]["data"]["cardNo"].asInt()));
			
			mySGD->refreshCardData(cardlist[i]["data"]);
			KSProtectVar<int> t_prt;
			t_prt = cardlist[i]["data"]["cardNo"].asInt();
			take_card_number_list.push_back(t_prt);
		}
		
		Json::Value exchangeResult = result_data["exchangeResult"]["list"];
		for(int i=0;i<exchangeResult.size();i++)
		{
			mySGD->refreshGoodsData(exchangeResult[i]["type"].asString(), exchangeResult[i]["count"].asInt());
		}
		
//		AudioEngine::sharedInstance()->playEffect("se_mapgacha.mp3", true);
		step_cnt = kCardGachaAnimationStep_fasting;
		one_button->setVisible(false);
		five_button->setVisible(false);
		addChild(KSGradualValue<float>::create(2.f, 10.f, 0.5f, [=](float t_f)
											   {
												   moving_speed = t_f;
											   }, [=](float t_f)
											   {
												   moving_speed = t_f;
												   addChild(KSTimer::create(0.2f, [=]()
																			{
																				stop_button->setVisible(true);
																				is_menu_enable = true;
																			}));
											   }));
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		mySGD->network_check_cnt = 0;
		
		mySGD->refreshGoodsData(result_data["minusType"].asString(), result_data["minusCount"].asInt());
		
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought)), 9999);
		
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
				Json::Value param;
				param["memberID"] = myHSP->getSocialID();
				param["gachaNo"] = mySGD->card_gacha_no.getV();
				param["number"] = myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV());
				param["count"] = 1;
				if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9) > 0)
					param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardPass_exchangeID_s);
				else
					param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardOnce_exchangeID_s);
				
				myHSP->command("getcardbygacha", param, json_selector(this, CardGachaPopup::resultOne));
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 Json::Value param;
										 param["memberID"] = myHSP->getSocialID();
										 param["gachaNo"] = mySGD->card_gacha_no.getV();
										 param["number"] = myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV());
										 param["count"] = 1;
										 if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9) > 0)
											 param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardPass_exchangeID_s);
										 else
											 param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardOnce_exchangeID_s);
										 
										 myHSP->command("getcardbygacha", param, json_selector(this, CardGachaPopup::resultOne));
									 }));
		}
	}
}

void CardGachaPopup::fiveAction(CCObject *t_sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	loading_layer = LoadingLayer::create(touch_priority-100);
	addChild(loading_layer, 999);
	loading_layer->startLoading();
	
	Json::Value param;
	param["memberID"] = myHSP->getSocialID();
	param["gachaNo"] = mySGD->card_gacha_no.getV();
	param["number"] = myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV());
	param["count"] = 5;
	param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardDozen_exchangeID_s);
	
	myHSP->command("getcardbygacha", param, json_selector(this, CardGachaPopup::resultFive));
}

void CardGachaPopup::resultFive(Json::Value result_data)
{
//	CCLog("resultFive : \n%s", result_data.toStyledString().c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		loading_layer->removeFromParent();
		
		myDSH->setIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV(), myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV()) + 5, true);
		close_button->setVisible(false);
		
		take_card_number_list.clear();
		
		Json::Value cardlist = result_data["cardlist"];
		for(int i=0;i<cardlist.size();i++)
		{
			if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9) <= 0)
				fiverocks::FiveRocksBridge::trackEvent("UseGem", "CardGacha", ccsf("%d", myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV()-5+i+1)), ccsf("%d", cardlist[i]["data"]["cardNo"].asInt()));
			
			mySGD->refreshCardData(cardlist[i]["data"]);
			KSProtectVar<int> t_prt;
			t_prt = cardlist[i]["data"]["cardNo"].asInt();
			take_card_number_list.push_back(t_prt);
		}
		
		Json::Value exchangeResult = result_data["exchangeResult"]["list"];
		for(int i=0;i<exchangeResult.size();i++)
		{
			mySGD->refreshGoodsData(exchangeResult[i]["type"].asString(), exchangeResult[i]["count"].asInt());
		}
		
//		AudioEngine::sharedInstance()->playEffect("se_mapgacha.mp3", true);
		step_cnt = kCardGachaAnimationStep_fasting;
		one_button->setVisible(false);
		five_button->setVisible(false);
		addChild(KSGradualValue<float>::create(2.f, 10.f, 0.5f, [=](float t_f)
											   {
												   moving_speed = t_f;
											   }, [=](float t_f)
											   {
												   moving_speed = t_f;
												   addChild(KSTimer::create(0.2f, [=]()
																			{
																				stop_button->setVisible(true);
																				is_menu_enable = true;
																			}));
											   }));
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		mySGD->network_check_cnt = 0;
		
		mySGD->refreshGoodsData(result_data["minusType"].asString(), result_data["minusCount"].asInt());
		
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_rubyNotEnought)), 9999);
		
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
				Json::Value param;
				param["memberID"] = myHSP->getSocialID();
				param["gachaNo"] = mySGD->card_gacha_no.getV();
				param["number"] = myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV());
				param["count"] = 5;
				param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardDozen_exchangeID_s);
				
				myHSP->command("getcardbygacha", param, json_selector(this, CardGachaPopup::resultFive));
			}, 1);
			if(alert)
				((CCNode*)CCDirector::sharedDirector()->getRunningScene()->getChildren()->objectAtIndex(0))->addChild(alert,999999);
		}
		else
		{
			addChild(KSTimer::create(0.5f, [=]()
									 {
										 Json::Value param;
										 param["memberID"] = myHSP->getSocialID();
										 param["gachaNo"] = mySGD->card_gacha_no.getV();
										 param["number"] = myDSH->getIntegerForKey(kDSH_Key_cardGachaTryCnt_int1, mySGD->card_gacha_no.getV());
										 param["count"] = 5;
										 param["exchangeID"] = NSDS_GS(kSDS_GI_shopGachaCardDozen_exchangeID_s);
										 
										 myHSP->command("getcardbygacha", param, json_selector(this, CardGachaPopup::resultFive));
									 }));
		}
	}
}

void CardGachaPopup::myStopAction(CCObject* t_sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
//	AudioEngine::sharedInstance()->stopEffect("se_mapgacha.mp3");
	
	step_cnt = kCardGachaAnimationStep_slowing;
	stop_button->setVisible(false);
	addChild(KSGradualValue<float>::create(10.f, 4.f, 0.5f, [=](float t_f)
										   {
											   moving_speed = t_f;
										   }, [=](float t_f)
										   {
											   moving_speed = t_f;
											   step_cnt = kCardGachaAnimationStep_takeCardCreate;
										   }));
}

void CardGachaPopup::okAction(CCObject* t_sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	ok_button->setVisible(false);
	
	take_card_number_list.pop_front();
	
	function<void()> final_func;
	
	moving_speed = 2.f;
	
	if(take_card_number_list.empty())
	{
		step_cnt = kCardGachaAnimationStep_ready;
		schedule(schedule_selector(CardGachaPopup::cardMoving));
		
		final_func = [=]()
		{
			// 각 버튼의 재화 확인
			CCNode* t_parent = price_back->getParent();
			
			price_back->removeFromParent();
			
			price_back = CCScale9Sprite::create("common_in.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
			price_back->setContentSize(CCSizeMake(70, 30));
			price_back->setPosition(ccp(32,0));
			t_parent->addChild(price_back);
			if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9) > 0)
			{
				CCSprite* price_type = CCSprite::create("icon_p9.png");
				price_type->setScale(0.8f);
				KSLabelTTF* price_value = KSLabelTTF::create(KS::insert_separator(mySGD->getGoodsValue(GoodsType::kGoodsType_pass9)).c_str(), mySGD->getFont().c_str(), 14);
				price_type->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_value->getContentSize().width/2.f-4,0));
				price_value->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_type->getContentSize().width*price_type->getScale()/2.f-4, 0));
				price_back->addChild(price_type);
				price_back->addChild(price_value);
				
				sub_ment1->setString(NSDS_GS(kSDS_GI_shopGachaCardPass_countName_s).c_str());
			}
			else
			{
				string t_type = "icon_r.png";
				
				Json::Reader t_reader;
				Json::Value t_reward;
				t_reader.parse(NSDS_GS(kSDS_GI_shopGachaCardOnce_reward_s), t_reward);
				
				GoodsType goods_type = mySGD->getGoodsKeyToType(t_reward[0]["type"].asString());
				if(goods_type == kGoodsType_ruby)
					t_type = "icon_r.png";
				else if(goods_type == kGoodsType_gold)
					t_type = "icon_g.png";
				
				CCSprite* price_type = CCSprite::create(t_type.c_str());
				price_type->setScale(0.8f);
				KSLabelTTF* price_value = KSLabelTTF::create(NSDS_GS(kSDS_GI_shopGachaCardOnce_priceName_s).c_str(), mySGD->getFont().c_str(), 14);
				price_type->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(-price_value->getContentSize().width/2.f-4,0));
				price_value->setPosition(ccpFromSize(price_back->getContentSize()/2.f) + ccp(price_type->getContentSize().width*price_type->getScale()/2.f-4, 0));
				price_back->addChild(price_type);
				price_back->addChild(price_value);
				
				sub_ment1->setString(NSDS_GS(kSDS_GI_shopGachaCardOnce_countName_s).c_str());
			}
			
			one_button->setVisible(true);
			five_button->setVisible(true);
			close_button->setVisible(true);
			
			is_menu_enable = true;
		};
	}
	else
	{
//		AudioEngine::sharedInstance()->playEffect("se_mapgacha.mp3", true);
		step_cnt = kCardGachaAnimationStep_fasting;
		schedule(schedule_selector(CardGachaPopup::cardMoving));
		
		final_func = [=]()
		{
			addChild(KSGradualValue<float>::create(2.f, 10.f, 0.5f, [=](float t_f)
												   {
													   moving_speed = t_f;
												   }, [=](float t_f)
												   {
													   moving_speed = t_f;
													   addChild(KSTimer::create(0.2f, [=]()
																				{
																					stop_button->setVisible(true);
																					is_menu_enable = true;
																				}));
												   }));
		};
	}
	
	t_gray->addChild(KSGradualValue<int>::create(255, 0, 0.3f, [=](int t_i)
										 {
											 t_gray->setOpacity(t_i);
										 }, [=](int t_i)
										 {
											 t_gray->setOpacity(t_i);
											 t_gray->removeFromParent();
										 }));
	
	big_card_img->addChild(KSGradualValue<float>::create(0.5f, 0.f, 0.3f, [=](float t_f)
										   {
											   big_card_img->setScale(t_f);
										   }, [=](float t_f)
										   {
											   big_card_img->setScale(t_f);
											   big_card_img->removeFromParent();
											   final_func();
										   }));
	
	grade_card_take->addChild(KSGradualValue<float>::create(1.f, 1.5f, 0.3f, [=](float t_f)
										   {
											   grade_card_take->setScale(t_f);
										   }, [=](float t_f)
										   {
											   grade_card_take->setScale(t_f);
										   }));
	grade_card_take->addChild(KSGradualValue<int>::create(255, 0, 0.3f, [=](int t_i)
														  {
															  grade_card_take->setOpacity(t_i);
														  }, [=](int t_i)
														  {
															  grade_card_take->setOpacity(t_i);
														  }));
	grade_card_take->addChild(KSGradualValue<CCPoint>::create(ccp(0,20), ccp(0,24), 0.3f, [=](CCPoint t_p)
															  {
																  grade_card_take->setPosition(t_p);
															  }, [=](CCPoint t_p)
															  {
																  grade_card_take->setPosition(t_p);
																  grade_card_take->removeFromParent();
															  }));
}

void CardGachaPopup::cardMoving()
{
	deque<deque<CCSprite*>::iterator> remove_list;
	remove_list.clear();
	
	for(auto t_iter = card_que.begin();t_iter != card_que.end();++t_iter)
	{
		CCSprite* t_card_img = *t_iter;
		t_card_img->setPositionX(t_card_img->getPositionX() - moving_speed);
		
		CCRect card_bounding = t_card_img->boundingBox();
		card_bounding.origin = card_bounding.origin + clipping_node->getPosition();
		
		if(!mask_bounding_rect.intersectsRect(card_bounding))
		{
			remove_list.push_back(t_iter);
		}
	}
	
	while(!remove_list.empty())
	{
		auto t_iter = remove_list.front();
		(*t_iter)->removeFromParent();
		card_que.erase(t_iter);
		remove_list.pop_front();
		
		AudioEngine::sharedInstance()->playEffect("se_flipcard.mp3");
		
		if(remove_list.empty() && step_cnt == kCardGachaAnimationStep_takeCardCreate)
		{
			CCSprite* last_card = card_que.back();
			
			small_cover = CCSprite::create("cardgacha_card.png");//mySIL->getLoadedImg(ccsf("card%d_visible.png", take_card_number_list.front().getV()));
			small_cover->setPosition(last_card->getPosition() + ccp(mask_rect->getContentSize().width, 0));
			small_cover->setScale((320*0.25f)/small_cover->getContentSize().width);
			clipping_node->addChild(small_cover);
			
			card_que.push_back(small_cover);
			
//			cover_clipping = CCClippingNode::create(CCSprite::create("whitePaper.png", CCRectMake(0, 0, t_card_img->getContentSize().width, t_card_img->getContentSize().height)));
//			cover_clipping->setAlphaThreshold(0.1f);
//			cover_clipping->setPosition(ccpFromSize(t_card_img->getContentSize()/2.f));
//			t_card_img->addChild(cover_clipping);
//			
//			cover_img = CCSprite::create("cardgacha_card.png");
//			cover_img->setPosition(ccp(0,0));
//			cover_clipping->addChild(cover_img);
			
			step_cnt = kCardGachaAnimationStep_positioning;
			unschedule(schedule_selector(CardGachaPopup::cardMoving));
			positioning_value = small_cover->getPositionX();
			schedule(schedule_selector(CardGachaPopup::cardPositioning));
		}
		else
		{
			CCSprite* last_card = card_que.back();
			
			CCSprite* t_card_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", mySGD->card_gacha_list[rand()%card_type_size].getV()));
			t_card_img->setPosition(last_card->getPosition() + ccp(mask_rect->getContentSize().width, 0));
			t_card_img->setScale(0.25f);
			clipping_node->addChild(t_card_img);
			
			card_que.push_back(t_card_img);
		}
	}
}

void CardGachaPopup::cardPositioning()
{
	if(moving_speed >= 1.05f)
	{
		moving_speed -= 0.05f;
	}
	else
		moving_speed = 1.f;
	
	deque<deque<CCSprite*>::iterator> remove_list;
	remove_list.clear();
	
	float move_value;
	bool is_final = false;
	if(positioning_value <= moving_speed)
	{
		is_final = true;
		move_value = positioning_value;
		positioning_value = 0;
	}
	else
	{
		move_value = moving_speed;
		positioning_value -= moving_speed;
	}
	
	
	for(auto t_iter = card_que.begin();t_iter != card_que.end();++t_iter)
	{
		CCSprite* t_card_img = *t_iter;
		t_card_img->setPositionX(t_card_img->getPositionX() - move_value);
		
		CCRect card_bounding = t_card_img->boundingBox();
		card_bounding.origin = card_bounding.origin + clipping_node->getPosition();
		
		if(!mask_bounding_rect.intersectsRect(card_bounding))
		{
			remove_list.push_back(t_iter);
		}
	}
	
	while(!remove_list.empty())
	{
		auto t_iter = remove_list.front();
		(*t_iter)->removeFromParent();
		card_que.erase(t_iter);
		remove_list.pop_front();
		
		AudioEngine::sharedInstance()->playEffect("se_flipcard.mp3");
		
		CCSprite* last_card = card_que.back();
		
		CCSprite* t_card_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", mySGD->card_gacha_list[rand()%card_type_size].getV()));
		t_card_img->setPosition(last_card->getPosition() + ccp(mask_rect->getContentSize().width, 0));
		t_card_img->setScale(0.25f);
		clipping_node->addChild(t_card_img);
		
		card_que.push_back(t_card_img);
	}
	
	if(is_final)
	{
		unschedule(schedule_selector(CardGachaPopup::cardPositioning));
		
		addChild(KSTimer::create(0.3f, [=]()
		{
			CCSprite* take_card_img = small_cover;
			addChild(KSGradualValue<float>::create(0.f, 250.f, 0.2f, [=](float t_f)
			{
				take_card_img->setPositionY(t_f);
//				cover_clipping->setPositionY(take_card_img->getContentSize().height/2.f - t_f/take_card_img->getScale());
//				cover_img->setPositionY(t_f/take_card_img->getScale());
			}, [=](float t_f)
			{
				take_card_img->setPositionY(t_f);
//				cover_clipping->setPositionY(take_card_img->getContentSize().height/2.f - t_f/take_card_img->getScale());
//				cover_img->setPositionY(t_f/take_card_img->getScale());
				
				addChild(KSTimer::create(0.2f, [=]()
				 {
					 t_gray = CCSprite::create("back_gray.png");
					 t_gray->setOpacity(0);
					 t_gray->setPosition(ccp(0,14));
					 t_gray->setScaleX(gray->getScaleX());
					 t_gray->setScaleY(gray->getScaleY());
					 main_case->addChild(t_gray);
					 
					 big_card_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", take_card_number_list.front().getV()));
					 big_card_img->setScale(0.5f);
					 big_card_img->setPosition(ccp(0,400));
					 main_case->addChild(big_card_img);
					 
					 CCSprite* t_cover_img = CCSprite::create("cardgacha_card.png");
					 t_cover_img->setScale(320/t_cover_img->getContentSize().width);
					 t_cover_img->setPosition(ccpFromSize(big_card_img->getContentSize()/2.f));
					 big_card_img->addChild(t_cover_img);
					 
					 CCSprite* top_case = CCSprite::create("diary_frame_top.png");
					 top_case->setPosition(ccpFromSize(big_card_img->getContentSize()/2.f) + ccp(0,215));
					 big_card_img->addChild(top_case);
					 
					 CCSprite* bottom_case = CCSprite::create("diary_frame_bottom.png");
					 bottom_case->setPosition(ccpFromSize(big_card_img->getContentSize()/2.f) + ccp(0,-215));
					 big_card_img->addChild(bottom_case);
					 
					 CCSprite* left_case = CCSprite::create("diary_frame_left.png");
					 left_case->setPosition(ccpFromSize(big_card_img->getContentSize()/2.f) + ccp(-160,0));
					 big_card_img->addChild(left_case);
					 
					 CCSprite* right_case = CCSprite::create("diary_frame_right.png");
					 right_case->setPosition(ccpFromSize(big_card_img->getContentSize()/2.f) + ccp(160,0));
					 big_card_img->addChild(right_case);
					 
					 addChild(KSGradualValue<int>::create(0, 255, 0.3f, [=](int t_i)
														  {
															  t_gray->setOpacity(t_i);
														  }, [=](int t_i)
														  {
															  t_gray->setOpacity(t_i);
														  }));
					 
					 addChild(KSGradualValue<float>::create(400, 20, 0.3f, [=](float t_f)
					{
						big_card_img->setPositionY(t_f);
					}, [=](float t_f)
					{
						big_card_img->setPositionY(t_f);
						
						take_card_img->setPositionY(0);
//						cover_img->setPositionY(take_card_img->getContentSize().height/2.f);
						
						int t_grade = NSDS_GI(kSDS_CI_int1_grade_i, take_card_number_list.front().getV());
						
						CCSprite* zoom_3 = CCSprite::create("zoom_3.png");
						zoom_3->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
						if(t_grade == 4)
							zoom_3->setColor(ccc3(255,209,79));
						else
							zoom_3->setColor(ccc3(175,243,142));
						zoom_3->setScale(1.5f);
						zoom_3->setOpacity(0);
						zoom_3->setPosition(ccp(0,20));
						main_case->addChild(zoom_3);
						
						AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3");
						
						zoom_3->addChild(KSGradualValue<float>::create(1.5f, 1.3f, 11.f/30.f, [=](float t_f)
					   {
						   zoom_3->setScale(t_f);
					   }, [=](float t_f)
					   {
						   zoom_3->setScale(t_f);
						   
						   zoom_3->addChild(KSTimer::create(3.f/30.f, [=]()
							{
								zoom_3->addChild(KSGradualValue<float>::create(1.3f, 2.f, 10.f/30.f, [=](float t_f)
							   {
								   zoom_3->setScale(t_f);
							   }, [=](float t_f)
							   {
								   zoom_3->setScale(t_f);
							   }));
							}));
					   }));
						
						zoom_3->addChild(KSGradualValue<int>::create(0, 255, 4.f/30.f, [=](int t_i)
						 {
							 zoom_3->setOpacity(t_i);
						 }, [=](int t_i)
						 {
							 zoom_3->setOpacity(t_i);
							 zoom_3->addChild(KSTimer::create(17.f/30.f, [=]()
							  {
								  t_cover_img->removeFromParent();
								  zoom_3->addChild(KSGradualValue<int>::create(255, 0, 3.f/30.f, [=](int t_i)
								   {
									   zoom_3->setOpacity(t_i);
								   }, [=](int t_i)
								   {
									   zoom_3->setOpacity(t_i);
									   zoom_3->removeFromParent();
								   }));
							  }));
						 }));
						
						CCSprite* zoom_5 = CCSprite::create("zoom_5.png");
						zoom_5->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
						zoom_5->setScale(2.f);
						zoom_5->setOpacity(0);
						zoom_5->setPosition(ccp(0,20));
						main_case->addChild(zoom_5);
						
						zoom_5->addChild(KSGradualValue<float>::create(2.f, 0.7f, 6.f/30.f, [=](float t_f)
					   {
						   zoom_5->setScale(t_f);
					   }, [=](float t_f)
					   {
						   zoom_5->setScale(t_f);
						   
						   zoom_5->addChild(KSTimer::create(12.f/30.f, [=]()
							{
								zoom_5->addChild(KSGradualValue<float>::create(0.7f, 1.8f, 6.f/30.f, [=](float t_f)
							   {
								   zoom_5->setScale(t_f);
							   }, [=](float t_f)
							   {
								   zoom_5->setScale(t_f);
							   }));
							}));
					   }));
						
						zoom_5->addChild(KSGradualValue<int>::create(0, 150, 4.f/30.f, [=](int t_i)
						 {
							 zoom_5->setOpacity(t_i);
						 }, [=](int t_i)
						 {
							 zoom_5->setOpacity(t_i);
							 zoom_5->addChild(KSTimer::create(17.f/30.f, [=]()
							  {
								  zoom_5->addChild(KSGradualValue<int>::create(150, 0, 3.f/30.f, [=](int t_i)
								   {
									   zoom_5->setOpacity(t_i);
								   }, [=](int t_i)
								   {
									   zoom_5->setOpacity(t_i);
									   
									   ok_button->setVisible(true);
									   is_menu_enable = true;
									   
									   zoom_5->removeFromParent();
								   }));
							  }));
						 }));
						
						CCSprite* zoom_6 = CCSprite::create("zoom_5.png");
						zoom_6->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
						zoom_6->setScale(2.f);
						zoom_6->setOpacity(0);
						zoom_6->setPosition(ccp(0,20));
						main_case->addChild(zoom_6);
						
						zoom_6->addChild(KSTimer::create(4.f/30.f, [=]()
						 {
							 zoom_6->addChild(KSGradualValue<float>::create(2.f, 0.7f, 7.f/30.f, [=](float t_f)
							{
								zoom_6->setScale(t_f);
							}, [=](float t_f)
							{
								zoom_6->setScale(t_f);
								zoom_6->addChild(KSTimer::create(3.f/30.f, [=]()
								 {
									 zoom_6->addChild(KSGradualValue<float>::create(0.7f, 1.6f, 7.f/30.f, [=](float t_f)
									{
										zoom_6->setScale(t_f);
									}, [=](float t_f)
									{
										zoom_6->setScale(t_f);
									}));
								 }));
							}));
						 }));
						
						zoom_6->addChild(KSTimer::create(4.f/30.f, [=]()
						 {
							 zoom_6->addChild(KSGradualValue<int>::create(0, 150, 7.f/30.f, [=](int t_i)
							{
								zoom_6->setOpacity(t_i);
							}, [=](int t_i)
							{
								zoom_6->setOpacity(t_i);
								zoom_6->addChild(KSTimer::create(3.f/30.f, [=]()
								 {
									 zoom_6->addChild(KSGradualValue<int>::create(150, 0, 7.f/30.f, [=](int t_i)
									{
										zoom_6->setOpacity(t_i);
									}, [=](int t_i)
									{
										zoom_6->setOpacity(t_i);
										zoom_6->removeFromParent();
									}));
								 }));
							}));
						 }));
						
						
						grade_card_take = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_cardTake), t_grade), mySGD->getFont().c_str(), 40);
						if(t_grade == 1)
							grade_card_take->setGradientColor(ccc4(255, 255, 255, 255), ccc4(255, 255, 255, 255), ccp(0,-1));
						else if(t_grade == 2)
							grade_card_take->setGradientColor(ccc4(255, 255, 255, 255), ccc4(255, 255, 255, 255), ccp(0,-1));
						else if(t_grade == 3)
							grade_card_take->setGradientColor(ccc4(255, 255, 255, 255), ccc4(255, 255, 255, 255), ccp(0,-1));
						else if(t_grade == 4)
							grade_card_take->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
						grade_card_take->enableOuterStroke(ccBLACK, 2.f, 255, true);
						grade_card_take->setPosition(ccp(0,20));
						grade_card_take->setScale(3.f);
						grade_card_take->setOpacity(0);
						main_case->addChild(grade_card_take);
						
						grade_card_take->addChild(KSGradualValue<float>::create(3.f, 1.f, 8.f/30.f, [=](float t_f)
						   {
							   grade_card_take->setScale(t_f);
						   }, [=](float t_f)
						   {
							   grade_card_take->setScale(t_f);
						   }));
						grade_card_take->addChild(KSGradualValue<int>::create(0, 255, 8.f/30.f, [=](int t_i)
						  {
							  grade_card_take->setOpacity(t_i);
						  }, [=](int t_i)
						  {
							  grade_card_take->setOpacity(t_i);
						  }));
					}));
				 }));
			}));
		}));
	}
}

