//
//  ItemGachaPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 21..
//
//

#include "ItemGachaPopup.h"
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
#include "CommonAnimation.h"
#include "FiveRocksCpp.h"

ItemGachaPopup* ItemGachaPopup::create(int t_touch_priority, function<void()> t_end_func, function<void(int)> t_gacha_on_func, int t_item_type)
{
	ItemGachaPopup* t_mup = new ItemGachaPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_gacha_on_func, t_item_type);
	t_mup->autorelease();
	return t_mup;
}

void ItemGachaPopup::completedAnimationSequenceNamed (char const * name)
{
	string t_name = name;
	
	if(t_name == "Default Timeline")
	{
		AudioEngine::sharedInstance()->playEffect("se_itemgacha.mp3", false);
		m_container->addChild(KSGradualValue<float>::create(-0.5f, 1.f, 0.6f, [=](float t){
			if(t >= 0)
			{
				item_title->setOpacity(255-t*255);
				item_ment->setOpacity(255-t*255);
				KS::setOpacity(question_img, 255-t*255);
			}
		}, [=](float t){
			item_title->setOpacity(0);
			item_ment->setOpacity(0);
			KS::setOpacity(question_img, 0);
		}));
		
		m_container->addChild(KSTimer::create(0.3f, [=](){
			CCSprite* beam_img = CCSprite::create("puzzle_unlock_beam.png");
			beam_img->setPosition(ccp(0,33));
			beam_img->setScale(0.3f);
			beam_img->setOpacity(0);
			m_container->addChild(beam_img);
			
			m_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 7.f/30.f, [=](float t){
				beam_img->setOpacity(255*t);
			}, [=](float t){
				beam_img->setOpacity(255);
				
				m_container->addChild(KSTimer::create(18.f/30.f, [=](){
					m_container->addChild(KSGradualValue<float>::create(1.f, 0.f, 8.f/30.f, [=](float t){
						beam_img->setOpacity(255*t);
					}, [=](float t){
						beam_img->setOpacity(0);
						
						m_container->addChild(KSTimer::create(0.1f, [=](){beam_img->removeFromParent();}));
					}));
				}));
				
				CCSprite* upgrade_4 = CCSprite::create("missile_upgrade_4.png");
				upgrade_4->setPosition(ccp(0,33));
				upgrade_4->setScale(0.3f);
				upgrade_4->setRotation(0);
				upgrade_4->setOpacity(0);
				m_container->addChild(upgrade_4);
				
				m_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 13.f/30.f, [=](float t){
					upgrade_4->setScale(0.3f+t*3.7f);
					upgrade_4->setOpacity(t*255);
				}, [=](float t){
					upgrade_4->setScale(4.7f);
					upgrade_4->setOpacity(255);
					
					m_container->addChild(KSTimer::create(5.f/30.f, [=](){
						m_container->addChild(KSGradualValue<float>::create(1.f, 0.f, 10.f/30.f, [=](float t){
							upgrade_4->setOpacity(t*255);
						}, [=](float t){
							upgrade_4->setOpacity(0);
							
							m_container->addChild(KSTimer::create(0.1f, [=](){upgrade_4->removeFromParent();}));
						}));
						
						string title_str, ment_str;
						if(item_type == kIC_fast) // 200
						{
							title_str = myLoc->getLocalForKey(LK::kMyLocalKey_item4title);
							ment_str = myLoc->getLocalForKey(LK::kMyLocalKey_item4ment);
						}
						else if(item_type == kIC_subOneDie) // 200
						{
							title_str = myLoc->getLocalForKey(LK::kMyLocalKey_item5title);
							ment_str = myLoc->getLocalForKey(LK::kMyLocalKey_item5ment);
						}
						else if(item_type == kIC_silence) // 250
						{
							title_str = myLoc->getLocalForKey(LK::kMyLocalKey_item7title);
							ment_str = myLoc->getLocalForKey(LK::kMyLocalKey_item7ment);
						}
						else if(item_type == kIC_longTime) // 250
						{
							title_str = myLoc->getLocalForKey(LK::kMyLocalKey_item8title);
							ment_str = myLoc->getLocalForKey(LK::kMyLocalKey_item8ment);
						}
						else // 100 item_type == kIC_heartUp
						{
							title_str = myLoc->getLocalForKey(LK::kMyLocalKey_item10title);
							ment_str = myLoc->getLocalForKey(LK::kMyLocalKey_item10ment);
						}
						
						item_img = CCSprite::create(CCString::createWithFormat("item%d.png", item_type)->getCString());
						item_img->setPosition(ccp(0,33));
						item_img->setOpacity(0);
						item_img->setScale(0.6f);
						m_container->addChild(item_img);
						
						item_title->setColor(ccc3(255, 170, 20));
						item_title->setString(title_str.c_str());
						item_title->setOpacity(0);
						
						item_ment->setColor(ccWHITE);
						item_ment->setString(ment_str.c_str());
						item_ment->setOpacity(0);
						
						m_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t){
							item_img->setScale(0.6f + t*0.4f);
							item_img->setOpacity(t*255);
							item_title->setOpacity(t*255);
							item_ment->setOpacity(t*255);
						}, [=](float t){
							item_img->setScale(1.f);
							item_img->setOpacity(255);
							item_title->setOpacity(255);
							item_ment->setOpacity(255);
							gacha_on_func(item_type);
						}));
					}));
				}));
				
				m_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 28.f/30.f, [=](float t){
					upgrade_4->setRotation(t*30.f);
				}, [=](float t){
					upgrade_4->setRotation(30.f);
				}));
			}));
			
			m_container->addChild(KSGradualValue<float>::create(0.f, 0.9f, 17.f/30.f, [=](float t){
				beam_img->setScale(0.3f + t);
			}, [=](float t){
				beam_img->setScale(1.2f);
				m_container->addChild(KSGradualValue<float>::create(0.f, 0.2f, 16.f/30.f, [=](float t){
					beam_img->setScale(1.2f + t);
				}, [=](float t){
					beam_img->setScale(1.4f);
				}));
			}));
			
			m_container->addChild(KSGradualValue<float>::create(0.f, -20.f, 33.f/30.f, [=](float t){
				beam_img->setRotation(t);
			}, [=](float t){
				beam_img->setRotation(-20.f);
			}));
		}));
		
		m_container->addChild(KSTimer::create(0.3f+29.f/30.f, [=](){
			KS::setOpacity(regacha_button, 0);
			regacha_button->setVisible(true);
			m_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 6.f/30.f, [=](float t){
				KS::setOpacity(regacha_button, 255*t);
				regacha_button->setPosition(ccp(-65,-80-98+98*t));
			}, [=](float t){
				KS::setOpacity(regacha_button, 255);
				regacha_button->setPosition(ccp(-65,-80));
			}));
		}));
		
		m_container->addChild(KSTimer::create(0.3f+34.f/30.f, [=](){
			KS::setOpacity(use_button, 0);
			use_button->setVisible(true);
			m_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 6.f/30.f, [=](float t){
				KS::setOpacity(use_button, 255*t);
				use_button->setPosition(ccp(65,-80-98+98*t));
			}, [=](float t){
				KS::setOpacity(use_button, 255);
				use_button->setPosition(ccp(65,-80));
				is_menu_enable = true;
			}));
		}));
		
	}
}

void ItemGachaPopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void(int)> t_gacha_on_func, int t_item_type)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	gacha_on_func = t_gacha_on_func;
	
	item_type = t_item_type;
	
	CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
	float screen_scale_x = screen_size.width/screen_size.height/1.5f;
	if(screen_scale_x < 1.f)
		screen_scale_x = 1.f;
	
	gray = CCSprite::create("back_gray.png");
	gray->setOpacity(0);
	gray->setPosition(ccp(240,160));
	gray->setScaleX(screen_scale_x);
	gray->setScaleY(myDSH->ui_top/320.f/myDSH->screen_convert_rate);
	addChild(gray);
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCSprite::create("popup_large_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_itemGachaTitle), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251, 128));
	back_in->setPosition(ccpFromSize(back_case->getContentSize()/2.f) + ccp(0,18));
	back_case->addChild(back_in);
	
	
	auto t_ccb = KS::loadCCBI<CCSprite*>(this, "startsetting_question.ccbi");
	
	question_img = t_ccb.first;
	question_img->setPosition(ccp(0, 33));
	m_container->addChild(question_img);
	
	question_manager = t_ccb.second;
	question_manager->setDelegate(this);
	
	CCSprite* question_box = KS::loadCCBI<CCSprite*>(this, "startsetting_randombox.ccbi").first;
	question_box->setPosition(ccp(0,33));
	m_container->addChild(question_box);
	
	
	item_title = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_itemGachaDefaultContent1), mySGD->getFont().c_str(), 15);
	item_title->disableOuterStroke();
	item_title->setPosition(ccp(0,-17));
	m_container->addChild(item_title);
	
	item_ment = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_itemGachaDefaultContent2), mySGD->getFont().c_str(), 12);
	item_ment->disableOuterStroke();
	item_ment->setColor(ccc3(255, 170, 20));
	item_ment->setPosition(ccp(0,-32));
	m_container->addChild(item_ment);
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	
	regacha_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_itemRegacha), mySGD->getFont().c_str(), 13);
	regacha_label->disableOuterStroke();
	regacha_label->setPosition(ccp(0,10));
	r_label->addChild(regacha_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("common_in.png", CCRectMake(0,0,20,20), CCRectMake(9,9,2,2));
	price_back->setContentSize(CCSizeMake(82, 20));
	price_back->setPosition(ccp(regacha_label->getContentSize().width/2.f, regacha_label->getContentSize().height/2.f-20));
	regacha_label->addChild(price_back);
	
//	if(mySGD->getGoodsValue(kGoodsType_pass4) <= 0)
//	{
		is_stamp = true;
		
		CCSize stamp_size = CCSizeMake(50,20);
		
		CCScale9Sprite* stamp_back = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0,0,20,20), CCRectMake(9,9,2,2));
		stamp_back->setContentSize(stamp_size);
		stamp_back->setPosition(ccp(0, regacha_label->getContentSize().height/2.f+15));
		stamp_back->setRotation(-27);
		regacha_label->addChild(stamp_back);
		
		float discount_value = ((0.f + mySGD->getItemGachaGoldFee()) - mySGD->getItemGachaReplayGoldFee()) / mySGD->getItemGachaGoldFee() * 100.f;
		
		KSLabelTTF* stamp_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_itemRegachaDiscountValue), discount_value)->getCString(), mySGD->getFont().c_str(), 9);
		stamp_label->disableOuterStroke();
		stamp_label->setPosition(ccp(stamp_size.width/2.f, stamp_size.height/2.f));
		stamp_label->setColor(ccc3(255,222,0));
		stamp_back->addChild(stamp_label);
		
		CCSprite* price_type = CCSprite::create("price_gold_img.png");
		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
		price_back->addChild(price_type);
		CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getItemGachaReplayGoldFee())->getCString(), mySGD->getFont().c_str(), 12);
		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
		price_back->addChild(price_label);
//	}
//	else
//	{
//		is_stamp = false;
//		
//		CCSprite* price_type = CCSprite::create("pass_ticket4.png");
//		price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
//		price_back->addChild(price_type);
//		
//		CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
//		pass_count_case->setPosition(ccpFromSize(price_type->getContentSize()/2.f) + ccp(9,6));
//		price_type->addChild(pass_count_case);
//		
//		CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass4)), mySGD->getFont().c_str(), 8);
//		pass_count_label->setColor(ccc3(255, 255, 255));
//		float min_width = 10+pass_count_label->getContentSize().width;
//		if(min_width < 20)
//			min_width = 20;
//		pass_count_case->setContentSize(CCSizeMake(min_width, 20));
//		pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
//		pass_count_case->addChild(pass_count_label);
//		
//		CCLabelTTF* price_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 12);
//		price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
//		price_back->addChild(price_label);
//	}
	
	
	CCScale9Sprite* regacha_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	regacha_button = CCControlButton::create(r_label, regacha_back);
	regacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ItemGachaPopup::regachaAction), CCControlEventTouchUpInside);
	regacha_button->setPreferredSize(CCSizeMake(117,60));
	regacha_button->setPosition(ccp(-65,-80-98));
	m_container->addChild(regacha_button);
	
	regacha_button->setVisible(false);
	
	regacha_button->setTouchPriority(touch_priority);
	
	
	KSLabelTTF* use_label = KSLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_thisItemUse), mySGD->getFont().c_str(), 13);
	use_label->disableOuterStroke();
	CCScale9Sprite* use_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	use_button = CCControlButton::create(use_label, use_back);
	use_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ItemGachaPopup::useAction), CCControlEventTouchUpInside);
	use_button->setPreferredSize(CCSizeMake(117,60));//120,65
	use_button->setPosition(ccp(65,-80-98));
	m_container->addChild(use_button);
	
	use_button->setVisible(false);
	
	use_button->setTouchPriority(touch_priority);
	
	gacha_cnt = 1;
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		question_manager->runAnimationsForSequenceNamed("Default Timeline");
//		AudioEngine::sharedInstance()->playEffect("se_itemgacha.mp3", false);
	});
}

void ItemGachaPopup::useAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	string fiverocks_param1;
	if(item_type == kIC_fast)
		fiverocks_param1 = "dash";
	else if(item_type == kIC_subOneDie)
		fiverocks_param1 = "onekill";
	else if(item_type == kIC_silence)
		fiverocks_param1 = "silence";
	else if(item_type == kIC_longTime)
		fiverocks_param1 = "time";
	else if(item_type == kIC_heartUp)
		fiverocks_param1 = "continue";
	
	fiverocks::FiveRocksBridge::trackEvent("UseGold", "BuyRandomItem", fiverocks_param1.c_str(), ccsf("gacha %03d", gacha_cnt));
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	CommonAnimation::closePopup(this, m_container, gray, [=](){
		
	}, [=](){
		question_manager->setDelegate(NULL);
		end_func(); removeFromParent();
	});
}

void ItemGachaPopup::regachaAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
//	if(mySGD->getGoodsValue(kGoodsType_pass4) > 0)
//	{
//		loading_layer = LoadingLayer::create(touch_priority-100);
//		addChild(loading_layer);
//		
//		mySGD->addChangeGoods("g_i_p");
//		mySGD->changeGoods(json_selector(this, ItemGachaPopup::resultSaveUserData));
//	}
//	else
//	{
		if(mySGD->getGoodsValue(kGoodsType_gold) < mySGD->getItemGachaReplayGoldFee())
		{
			addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-100, kGoodsType_gold, [=]()
																{
																	((PuzzleScene*)getParent()->getParent())->showShopPopup(kSC_gold);
																}), 9999);
			is_menu_enable = true;
			return;
		}
		
		loading_layer = LoadingLayer::create(touch_priority-100);
		addChild(loading_layer);
		
	int random_value = rand()%1000;
	
	if(random_value < 200) // 200
		item_type = kIC_fast; // 4
	else if(random_value < 400) // 200
		item_type = kIC_subOneDie; // 5
	else if(random_value < 650) // 250
		item_type = kIC_silence; // 7
	else if(random_value < 900) // 250
		item_type = kIC_longTime; // 8
	else // 100
		item_type = kIC_heartUp; // 10
	
		mySGD->addChangeGoods("g_i_gr", kGoodsType_begin, 0, "", ccsf("%d", item_type), "아이템가챠(다시뽑기)");
		mySGD->changeGoods(json_selector(this, ItemGachaPopup::resultSaveUserData));
//	}
}

void ItemGachaPopup::resultSaveUserData(Json::Value result_data)
{
	CCLOG("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("save userdata success!!");
		gacha_cnt++;
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
		KS::setOpacity(question_img, 255);
		item_img->removeFromParent();
		item_title->setString("");
		item_ment->setString("");
		regacha_button->setPosition(ccp(-65,-80-98));
		regacha_button->setVisible(false);
		
//		if(mySGD->getGoodsValue(kGoodsType_pass4) <= 0 && !is_stamp)
//		{
//			is_stamp = true;
//			
//			regacha_label->removeAllChildren();
//			
//			CCScale9Sprite* price_back = CCScale9Sprite::create("common_in.png", CCRectMake(0,0,20,20), CCRectMake(9,9,2,2));
//			price_back->setContentSize(CCSizeMake(82, 20));
//			price_back->setPosition(ccp(regacha_label->getContentSize().width/2.f, regacha_label->getContentSize().height/2.f-20));
//			regacha_label->addChild(price_back);
//			
//			CCSize stamp_size = CCSizeMake(50,20);
//			
//			CCScale9Sprite* stamp_back = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0,0,20,20), CCRectMake(9,9,2,2));
//			stamp_back->setContentSize(stamp_size);
//			stamp_back->setPosition(ccp(0, regacha_label->getContentSize().height/2.f+15));
//			stamp_back->setRotation(-27);
//			regacha_label->addChild(stamp_back);
//			
//			float discount_value = ((0.f + mySGD->getItemGachaGoldFee()) - mySGD->getItemGachaReplayGoldFee()) / mySGD->getItemGachaGoldFee() * 100.f;
//			
//			KSLabelTTF* stamp_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(LK::kMyLocalKey_itemRegachaDiscountValue), discount_value)->getCString(), mySGD->getFont().c_str(), 9);
//			stamp_label->setPosition(ccp(stamp_size.width/2.f, stamp_size.height/2.f));
//			stamp_label->setColor(ccc3(255,222,0));
//			stamp_back->addChild(stamp_label);
//			
//			CCSprite* price_type = CCSprite::create("price_gold_img.png");
//			price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
//			price_back->addChild(price_type);
//			CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getItemGachaReplayGoldFee())->getCString(), mySGD->getFont().c_str(), 12);
//			price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
//			price_back->addChild(price_label);
//		}
//		else
//		{
//			regacha_label->removeAllChildren();
//			
//			CCScale9Sprite* price_back = CCScale9Sprite::create("common_in.png", CCRectMake(0,0,20,20), CCRectMake(9,9,2,2));
//			price_back->setContentSize(CCSizeMake(82, 20));
//			price_back->setPosition(ccp(regacha_label->getContentSize().width/2.f, regacha_label->getContentSize().height/2.f-20));
//			regacha_label->addChild(price_back);
//			
//			CCSprite* price_type = CCSprite::create("pass_ticket4.png");
//			price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
//			price_back->addChild(price_type);
//			
//			CCScale9Sprite* pass_count_case = CCScale9Sprite::create("mainflow_new2.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
//			pass_count_case->setPosition(ccpFromSize(price_type->getContentSize()/2.f) + ccp(9,6));
//			price_type->addChild(pass_count_case);
//			
//			CCLabelTTF* pass_count_label = CCLabelTTF::create(ccsf("%d", mySGD->getGoodsValue(kGoodsType_pass4)), mySGD->getFont().c_str(), 8);
//			pass_count_label->setColor(ccc3(255, 255, 255));
//			float min_width = 10+pass_count_label->getContentSize().width;
//			if(min_width < 20)
//				min_width = 20;
//			pass_count_case->setContentSize(CCSizeMake(min_width, 20));
//			pass_count_label->setPosition(ccp(pass_count_case->getContentSize().width/2.f-0.5f, pass_count_case->getContentSize().height/2.f+0.5f));
//			pass_count_case->addChild(pass_count_label);
//			
//			CCLabelTTF* price_label = CCLabelTTF::create(myLoc->getLocalForKey(LK::kMyLocalKey_free), mySGD->getFont().c_str(), 12);
//			price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
//			price_back->addChild(price_label);
//			
//		}
		
		use_button->setPosition(ccp(65,-80-98));
		use_button->setVisible(false);
		
		question_manager->runAnimationsForSequenceNamed("Default Timeline");
//		AudioEngine::sharedInstance()->playEffect("se_itemgacha.mp3", false);
	}
	else
	{
		CCLOG("missile upgrade fail!!");
		
		mySGD->clearChangeGoods();
		addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_failPurchase)), 9999);
		
		is_menu_enable = true;
	}
	loading_layer->removeFromParent();
}