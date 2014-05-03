//
//  MissileUpgradePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 4. 20..
//
//

#include "MissileUpgradePopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "StoneMissile.h"
#include "ASPopupView.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "PuzzleScene.h"

MissileUpgradePopup* MissileUpgradePopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_upgrade_func)
{
	MissileUpgradePopup* t_mup = new MissileUpgradePopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_upgrade_func);
	t_mup->autorelease();
	return t_mup;
}

void MissileUpgradePopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_upgrade_func)
{
	is_menu_enable = false;
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	upgrade_func = t_upgrade_func;
	
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
	
	CCSprite* n_cancel = CCSprite::create("subpop_cancel.png");
	CCSprite* s_cancel = CCSprite::create("subpop_cancel.png");
	s_cancel->setColor(ccGRAY);
	
	CCMenuItemLambda* cancel_item = CCMenuItemSpriteLambda::create(n_cancel, s_cancel, [=](CCObject* sender)
																   {
																	   if(!is_menu_enable)
																		   return;
																	   
																	   is_menu_enable = false;
																	   
																	   addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
																		   addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
																	   
																	   addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); end_func(); removeFromParent();}));
																	   
																   });
	
	CCMenuLambda* cancel_menu = CCMenuLambda::createWithItem(cancel_item);
	cancel_menu->setPosition(ccp(140, 105));
	m_container->addChild(cancel_menu);
	cancel_menu->setTouchPriority(touch_priority);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_missileUpgrade), mySGD->getFont().c_str(), 21);
	title_label->setColor(ccc3(50, 250, 255));
	title_label->setPosition(ccp(0,100));
	m_container->addChild(title_label);
	
	KSLabelTTF* sub_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_upgradeSubMent), mySGD->getFont().c_str(), 12);
	sub_label->enableOuterStroke(ccBLACK, 1.f);
	sub_label->setPosition(ccp(0,80));
	m_container->addChild(sub_label);
	
	upgrade_action_node = CCNode::create();
	m_container->addChild(upgrade_action_node);
	
	CCSprite* level_case = CCSprite::create("startsetting_levelbox.png");
	level_case->setPosition(ccp(0,60));
	upgrade_action_node->addChild(level_case);
	
	StoneType missile_type_code = StoneType(mySGD->getSelectedCharacterHistory().characterNo.getV()-1);
	
	int missile_level = mySGD->getSelectedCharacterHistory().level.getV();
	
	missile_img = NULL;
	
	if(missile_type_code == kStoneType_guided)
	{
		int grade = (missile_level-1)/5+1;
		bool rotation = false;
		if(grade == 1 || grade == 4)
			rotation = true;
		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("jack_missile_%d.png", missile_level)->getCString(),
																														 rotation);
		t_gm->setPosition(ccp(0,0));
		t_gm->beautifier((missile_level-1)/5+1, (missile_level-1)%5+1);
		upgrade_action_node->addChild(t_gm);
		
		
		t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
		
		missile_img = t_gm;
	}
	
	missile_data_level = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_levelValue), missile_level)->getCString(), mySGD->getFont().c_str(), 12);
	missile_data_level->setColor(ccc3(255, 222, 0));
	missile_data_level->enableOuterStroke(ccBLACK, 1.f);
	missile_data_level->setAnchorPoint(ccp(0,0.5f));
	missile_data_level->setPosition(ccp(-53,60));
	upgrade_action_node->addChild(missile_data_level);
	
	missile_data_power = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_powerValue), StoneAttack::getPower((missile_level-1)/5+1, (missile_level-1)%5+1))->getCString(), mySGD->getFont().c_str(), 12);
	missile_data_power->setColor(ccc3(255, 222, 0));
	missile_data_power->enableOuterStroke(ccBLACK, 1.f);
	missile_data_power->setAnchorPoint(ccp(0,0.5f));
	missile_data_power->setPosition(ccp(1,60));
	upgrade_action_node->addChild(missile_data_power);
	
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	upgrade_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_upgradeLevelValue), missile_level+1)->getCString(), mySGD->getFont().c_str(), 13);
	upgrade_label->setPosition(ccp(0,10));
	CCScale9Sprite* price_back = CCScale9Sprite::create("subpop_darkred.png", CCRectMake(0,0,30,30), CCRectMake(14,14,2,2));
	price_back->setContentSize(CCSizeMake(80, 30));
	price_back->setPosition(ccp(upgrade_label->getContentSize().width/2.f, upgrade_label->getContentSize().height/2.f-20));
	upgrade_label->addChild(price_back);
	
	CCSprite* price_type = CCSprite::create("price_gold_img.png");
	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-25,price_back->getContentSize().height/2.f));
	price_back->addChild(price_type);
	price_label = CCLabelTTF::create(CCString::createWithFormat("%d", mySGD->getSelectedCharacterHistory().nextPrice.getV())->getCString(), mySGD->getFont().c_str(), 12);
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);
	
	t_label->addChild(upgrade_label);
	
	
	
	CCScale9Sprite* upgrade_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	upgrade_button = CCControlButton::create(t_label, upgrade_back);
	upgrade_button->addTargetWithActionForControlEvents(this, cccontrol_selector(MissileUpgradePopup::upgradeAction), CCControlEventTouchUpInside);
	upgrade_button->setPreferredSize(CCSizeMake(150,65));
	upgrade_button->setPosition(ccp(0,-85));
	upgrade_action_node->addChild(upgrade_button);
	
	upgrade_button->setTouchPriority(touch_priority);
	
	
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 255);is_menu_enable = true;}));
}

void MissileUpgradePopup::upgradeAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int upgrade_price = mySGD->getSelectedCharacterHistory().nextPrice.getV();

	if(mySGD->getGoodsValue(kGoodsType_gold) < upgrade_price)// + use_item_price_gold.getV())
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
	
	int missile_level = mySGD->getSelectedCharacterHistory().level.getV();
	before_gold = mySGD->getGoodsValue(kGoodsType_gold);
	before_level = missile_level;
	before_damage = StoneAttack::getPower((before_level-1)/5+1, (before_level-1)%5+1);
	mySGD->addChangeGoods(kGoodsType_gold, -mySGD->getSelectedCharacterHistory().nextPrice.getV(), "미사일업그레이드", CCString::createWithFormat("%d", missile_level)->getCString());
	
	CharacterHistory t_history = mySGD->getSelectedCharacterHistory();
	t_history.level = t_history.level.getV() + 1;
	
	vector<CommandParam> command_list;
	command_list.clear();
	command_list.push_back(mySGD->getUpdateCharacterHistoryParam(t_history, nullptr));
	
	
	mySGD->changeGoodsTransaction(command_list, json_selector(this, MissileUpgradePopup::resultSaveUserData));
}

void MissileUpgradePopup::resultSaveUserData(Json::Value result_data)
{
	CCLOG("resultSaveUserData : %s", GraphDogLib::JsonObjectToString(result_data).c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		CCLOG("missile upgrade success!!");
		
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		AudioEngine::sharedInstance()->playEffect("se_upgrade.mp3", false);
		
		addChild(KSGradualValue<float>::create(1.f, 0.f, 0.3f, [=](float t){
			upgrade_action_node->setScaleX(t/2.f + 0.5f);
			upgrade_action_node->setScaleY(t);
		}, [=](float t){
			upgrade_action_node->setScaleX(0.5f);
			upgrade_action_node->setScaleY(0.f);
		}));
		
		CCSprite* upgrade_effect_3 = CCSprite::create("missile_upgrade_3.png");
		upgrade_effect_3->setOpacity(0);
		upgrade_effect_3->setPosition(ccp(0,0));
		m_container->addChild(upgrade_effect_3);
		
		CCSprite* upgrade_effect_4 = CCSprite::create("missile_upgrade_4.png");
		upgrade_effect_4->setScale(0);
		upgrade_effect_4->setOpacity(0);
		upgrade_effect_4->setPosition(ccp(-57,0));
		m_container->addChild(upgrade_effect_4);
		
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.25f, [=](float t){
			upgrade_effect_3->setOpacity(t*255);
			upgrade_effect_4->setOpacity(t*255);
			upgrade_effect_4->setScale(t);
		}, [=](float t){
			upgrade_effect_3->setOpacity(255);
			upgrade_effect_4->setOpacity(255);
			upgrade_effect_4->setScale(1.f);
			
			addChild(KSGradualValue<float>::create(0.f, 1.f, 0.45f, [=](float t){
				upgrade_effect_3->setScaleY(1.f - 0.8f*t);
				upgrade_effect_4->setPositionX(-57+120*t);
				upgrade_effect_4->setScale(1.f-t);
				upgrade_effect_4->setOpacity(255-t*255);
			}, [=](float t){
				upgrade_effect_3->setScaleY(0.2f);
				upgrade_effect_4->setPositionX(0);
				upgrade_effect_4->setScale(0.f);
				upgrade_effect_4->setOpacity(0);
				
				addChild(KSGradualValue<float>::create(0.f, 1.f, 0.45f, [=](float t){
					upgrade_effect_3->setScaleX(1.f+4.f*t);
					upgrade_effect_3->setScaleY(0.2f+4.8f*t);
					upgrade_effect_4->setScale(10.f*t);
				}, [=](float t){
					addChild(KSTimer::create(0.1f, [=](){
						upgrade_effect_3->removeFromParent();
						upgrade_effect_4->removeFromParent();
					}));
				}));
				
				addChild(KSGradualValue<float>::create(0.f, 1.f, 0.25f, [=](float t){
					upgrade_effect_4->setOpacity(t*255);
				}, [=](float t){
					upgrade_effect_4->setOpacity(255);
					
					addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t){
						upgrade_effect_3->setOpacity(255-t*255);
						upgrade_effect_4->setOpacity(255-t*255);
					}, [=](float t){
						upgrade_effect_3->setOpacity(0);
						upgrade_effect_4->setOpacity(0);
					}));
					
					addChild(KSGradualValue<float>::create(-0.2f, 1.f, 0.2f, [=](float t){
						if(t >= 0)
						{
							upgrade_action_node->setScaleX(0.5f + t*0.5f);
							upgrade_action_node->setScaleY(t);
						}
					}, [=](float t){
						upgrade_action_node->setScaleX(1.f);
						upgrade_action_node->setScaleY(1.f);
					}));
					
					setAfterUpgrade();
				}));
			}));
		}));
	}
	else
	{
		CCLOG("missile upgrade fail!!");
		
		mySGD->clearChangeGoods();
		
		addChild(ASPopupView::getCommonNoti(touch_priority-200, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
	}
	loading_layer->removeFromParent();
	is_menu_enable = true;
}

void MissileUpgradePopup::setAfterUpgrade()
{
	int missile_level = mySGD->getSelectedCharacterHistory().level.getV();
	
	int after_damage = StoneAttack::getPower((missile_level-1)/5+1, (missile_level-1)%5+1);
	
	missile_data_level->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_levelValue), missile_level)->getCString());
	missile_data_power->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_powerValue), after_damage)->getCString());
	
	CCPoint missile_position;
	if(missile_img)
	{
		missile_position = missile_img->getPosition();
		missile_img->removeFromParent();
	}
	
	StoneType missile_type_code = StoneType(mySGD->getSelectedCharacterHistory().characterNo.getV()-1);
	
	if(missile_type_code == kStoneType_guided)
	{

		int grade = (missile_level-1)/5+1;
		bool rotation = false;
		if(grade == 1 || grade == 4)
			rotation = true;

		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("jack_missile_%d.png", missile_level)->getCString(),
																															rotation);
//		GuidedMissile* t_gm = GuidedMissile::createForShowWindow(CCString::createWithFormat("me_guide%d.ccbi", (missile_level-1)%5 + 1)->getCString());
		t_gm->setPosition(missile_position);
		t_gm->beautifier((missile_level-1)/5+1, (missile_level-1)%5+1);
		upgrade_action_node->addChild(t_gm);
		
		
		t_gm->setShowWindowVelocityRad(M_PI / (60.f - (grade-1)*6));
		
		missile_img = t_gm;
	}
	
	
	if(missile_level >= 25)
	{
		upgrade_button->removeFromParent();
	}
	else
	{
		upgrade_label->setString(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_upgradeLevelValue), missile_level+1)->getCString());
		price_label->setString(CCString::createWithFormat("%d", mySGD->getSelectedCharacterHistory().nextPrice.getV())->getCString());
	}
	
	upgrade_func();
	
	addChild(KSTimer::create(0.1f, [=](){
		CCSprite* upgrade_effect_2 = CCSprite::create("missile_upgrade_2.png");
		upgrade_effect_2->setScaleX(0.f);
		upgrade_effect_2->setPosition(ccp(99,-14));
		m_container->addChild(upgrade_effect_2);
		
		KSLabelTTF* effect_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_power), mySGD->getFont2().c_str(), 13);
		effect_label->enableOuterStroke(ccBLACK, 1.f);
		effect_label->setPosition(ccp(38,47));
		upgrade_effect_2->addChild(effect_label);
		
		KSLabelTTF* upper_damage = KSLabelTTF::create(CCString::createWithFormat("+%d", after_damage-before_damage)->getCString(), mySGD->getFont().c_str(), 13);
		upper_damage->setColor(ccc3(255,246,0));
		upper_damage->enableOuterStroke(ccBLACK, 1.f);
		upper_damage->setPosition(ccp(upgrade_effect_2->getContentSize().width/2.f, upgrade_effect_2->getContentSize().height/2.f-8));
		upgrade_effect_2->addChild(upper_damage);
		
		addChild(KSGradualValue<float>::create(0.f, 1.f, 0.2f, [=](float t){
			upgrade_effect_2->setScaleX(t);
		}, [=](float t){
			upgrade_effect_2->setScaleX(1.f);
			
			CCMoveTo* t_move1 = CCMoveTo::create(0.3f, ccp(99, 12));
			CCMoveTo* t_move2 = CCMoveTo::create(0.15f, ccp(99, -8));
			CCSequence* t_seq1 = CCSequence::createWithTwoActions(t_move1, t_move2);
			CCRepeat* t_repeat = CCRepeat::create(t_seq1, 3);
			
			CCMoveTo* t_move = CCMoveTo::create(0.3f, ccp(99, 12));
			CCScaleTo* t_scale = CCScaleTo::create(0.3f, 0.f, 1.f);
			CCSpawn* t_spawn = CCSpawn::createWithTwoActions(t_move, t_scale);
			
			CCCallFunc* t_call = CCCallFunc::create(upgrade_effect_2, callfunc_selector(CCSprite::removeFromParent));
			
			CCSequence* t_seq = CCSequence::create(t_repeat, t_spawn, t_call, NULL);
			
			upgrade_effect_2->runAction(t_seq);
		}));
	}));
}
