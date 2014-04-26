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

ItemGachaPopup* ItemGachaPopup::create(int t_touch_priority, function<void()> t_end_func, function<void(int)> t_gacha_on_func)
{
	ItemGachaPopup* t_mup = new ItemGachaPopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_gacha_on_func);
	t_mup->autorelease();
	return t_mup;
}

void ItemGachaPopup::completedAnimationSequenceNamed (char const * name)
{
	string t_name = name;
	
	if(t_name == "Default Timeline")
	{
		m_container->addChild(KSGradualValue<float>::create(-0.5f, 1.f, 0.6f, [=](float t){
			if(t >= 0)
			{
				KS::setOpacity(question_img, 255-t*255);
			}
		}, [=](float t){
			KS::setOpacity(question_img, 0);
		}));
		
		m_container->addChild(KSTimer::create(0.3f, [=](){
			CCSprite* beam_img = CCSprite::create("puzzle_unlock_beam.png");
			beam_img->setPosition(ccp(0,10));
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
				upgrade_4->setPosition(ccp(0,10));
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
						
						int random_value = rand()%1000;
						if(random_value < 250)
							item_type = kIC_fast;
						else if(random_value < 500)
							item_type = kIC_subOneDie;
						else if(random_value < 800)
							item_type = kIC_silence;
						else
							item_type = kIC_heartUp;
						
						item_img = CCSprite::create(CCString::createWithFormat("item%d.png", item_type)->getCString());
						item_img->setPosition(ccp(0,10));
						item_img->setOpacity(0);
						item_img->setScale(0.6f);
						m_container->addChild(item_img);
						
						m_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 8.f/30.f, [=](float t){
							item_img->setScale(0.6f + t*0.4f);
							item_img->setOpacity(t*255);
						}, [=](float t){
							item_img->setScale(1.f);
							item_img->setOpacity(255);
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
				regacha_button->setPosition(ccp(-70,-85-100+100*t));
			}, [=](float t){
				KS::setOpacity(regacha_button, 255);
				regacha_button->setPosition(ccp(-70,-85));
			}));
		}));
		
		m_container->addChild(KSTimer::create(0.3f+34.f/30.f, [=](){
			KS::setOpacity(use_button, 0);
			use_button->setVisible(true);
			m_container->addChild(KSGradualValue<float>::create(0.f, 1.f, 6.f/30.f, [=](float t){
				KS::setOpacity(use_button, 255*t);
				use_button->setPosition(ccp(70,-85-100+100*t));
			}, [=](float t){
				KS::setOpacity(use_button, 255);
				use_button->setPosition(ccp(70,-85));
				is_menu_enable = true;
			}));
		}));
		
	}
}

void ItemGachaPopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void(int)> t_gacha_on_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	gacha_on_func = t_gacha_on_func;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(350,280));
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	
	CCSprite* title_label = CCSprite::create("item_gacha_title.png");
	title_label->setPosition(ccp(0,90));
	m_container->addChild(title_label);
	
	auto t_ccb = KS::loadCCBI<CCSprite*>(this, "startsetting_question.ccbi");
	
	question_img = t_ccb.first;
	question_img->setPosition(ccp(0, 10));
	m_container->addChild(question_img);
	
	question_manager = t_ccb.second;
	question_manager->setDelegate(this);
	
	CCSprite* question_box = KS::loadCCBI<CCSprite*>(this, "startsetting_randombox.ccbi").first;
	question_box->setPosition(ccp(0,10));
	m_container->addChild(question_box);
	
	
	CCLabelTTF* r_label = CCLabelTTF::create();
	
	KSLabelTTF* regacha_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_itemRegacha), mySGD->getFont().c_str(), 13);
	regacha_label->setPosition(ccp(20,10));
	CCScale9Sprite* price_back = CCScale9Sprite::create("subpop_darkred.png", CCRectMake(0,0,30,30), CCRectMake(14,14,2,2));
	price_back->setContentSize(CCSizeMake(80, 30));
	price_back->setPosition(ccp(regacha_label->getContentSize().width/2.f-20, regacha_label->getContentSize().height/2.f-20));
	regacha_label->addChild(price_back);
	
	CCSize stamp_size = CCSizeMake(50,20);
	
	CCScale9Sprite* stamp_back = CCScale9Sprite::create("subpop_stamp.png", CCRectMake(0,0,20,20), CCRectMake(9,9,2,2));
	stamp_back->setContentSize(stamp_size);
	stamp_back->setPosition(ccp(-25, regacha_label->getContentSize().height/2.f));
	stamp_back->setRotation(-27);
	regacha_label->addChild(stamp_back);
	
	float discount_value = ((0.f + mySGD->getItemGachaGoldFee()) - mySGD->getItemGachaReplayGoldFee()) / mySGD->getItemGachaGoldFee() * 100.f;
	
	KSLabelTTF* stamp_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_itemRegachaDiscountValue), discount_value)->getCString(), mySGD->getFont().c_str(), 9);
	stamp_label->setPosition(ccp(stamp_size.width/2.f, stamp_size.height/2.f));
	stamp_label->setColor(ccc3(255,222,0));
	stamp_back->addChild(stamp_label);
	
	
	CCSprite* price_type = CCSprite::create("common_button_gold.png");
	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
	price_back->addChild(price_type);
	CCLabelTTF* price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getItemGachaReplayGoldFee())->getCString(), mySGD->getFont().c_str(), 12);
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	r_label->addChild(regacha_label);
	
	
	CCScale9Sprite* regacha_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	regacha_button = CCControlButton::create(r_label, regacha_back);
	regacha_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ItemGachaPopup::regachaAction), CCControlEventTouchUpInside);
	regacha_button->setPreferredSize(CCSizeMake(120,65));
	regacha_button->setPosition(ccp(-70,-85-100));
	m_container->addChild(regacha_button);
	
	regacha_button->setVisible(false);
	
	regacha_button->setTouchPriority(touch_priority);
	
	
	KSLabelTTF* use_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_thisItemUse), mySGD->getFont().c_str(), 13);
	CCScale9Sprite* use_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	use_button = CCControlButton::create(use_label, use_back);
	use_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ItemGachaPopup::useAction), CCControlEventTouchUpInside);
	use_button->setPreferredSize(CCSizeMake(120,65));
	use_button->setPosition(ccp(70,-85-100));
	m_container->addChild(use_button);
	
	use_button->setVisible(false);
	
	use_button->setTouchPriority(touch_priority);
	
	
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t)
										 {
											 KS::setOpacity(m_container, 255);
											 question_manager->runAnimationsForSequenceNamed("Default Timeline");
											 AudioEngine::sharedInstance()->playEffect("se_itemgacha.mp3", false);
										 }));
}

void ItemGachaPopup::useAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
	
	addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
}

void ItemGachaPopup::regachaAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	if(mySGD->getGold() < mySGD->getItemGachaReplayGoldFee())
	{
		addChild(ASPopupView::getCommonNoti(touch_priority-100, myLoc->getLocalForKey(kMyLocalKey_goldNotEnought)));
		is_menu_enable = true;
		return;
	}
	
	loading_layer = LoadingLayer::create(touch_priority-100);
	addChild(loading_layer);
	
	mySGD->setGold(mySGD->getGold()-mySGD->getItemGachaReplayGoldFee());
	
	myDSH->saveUserData({kSaveUserData_Key_gold}, json_selector(this, ItemGachaPopup::resultSaveUserData));
}

void ItemGachaPopup::resultSaveUserData(Json::Value result_data)
{
	CCLog("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLog("save userdata success!!");
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
		KS::setOpacity(question_img, 255);
		item_img->removeFromParent();
		regacha_button->setPosition(ccp(-70,-85-100));
		regacha_button->setVisible(false);
		use_button->setPosition(ccp(70,-85-100));
		use_button->setVisible(false);
		
		question_manager->runAnimationsForSequenceNamed("Default Timeline");
		AudioEngine::sharedInstance()->playEffect("se_itemgacha.mp3", false);
	}
	else
	{
		CCLog("missile upgrade fail!!");
		
		mySGD->setGold(mySGD->getGold() + mySGD->getItemGachaReplayGoldFee());
		is_menu_enable = true;
	}
	loading_layer->removeFromParent();
}