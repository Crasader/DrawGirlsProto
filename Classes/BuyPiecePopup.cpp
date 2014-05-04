//
//  BuyPiecePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 5. 4..
//
//

#include "BuyPiecePopup.h"
#include "CCMenuLambda.h"
#include "TouchSuctionLayer.h"
#include "KSUtil.h"
#include "KSLabelTTF.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "LoadingLayer.h"
#include "MyLocalization.h"
#include "PuzzleScene.h"
#include "ASPopupView.h"

BuyPiecePopup* BuyPiecePopup::create(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func, int t_stage_number)
{
	BuyPiecePopup* t_mup = new BuyPiecePopup();
	t_mup->myInit(t_touch_priority, t_end_func, t_buy_func, t_stage_number);
	t_mup->autorelease();
	return t_mup;
}

void BuyPiecePopup::myInit(int t_touch_priority, function<void()> t_end_func, function<void()> t_buy_func, int t_stage_number)
{
	stage_number = t_stage_number;
	is_menu_enable = false;
	
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	touch_priority = t_touch_priority;
	end_func = t_end_func;
	buy_func = t_buy_func;
	
	suction = TouchSuctionLayer::create(touch_priority+1);
	addChild(suction);
	
	suction->setTouchEnabled(true);
	
	m_container = CCNode::create();
	m_container->setPosition(ccp(240,160));
	addChild(m_container);
	
	back_case = CCScale9Sprite::create("subpop_back.png", CCRectMake(0,0,100,100), CCRectMake(49,49,2,2));
	back_case->setContentSize(CCSizeMake(280,220));
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
	cancel_menu->setPosition(ccp(100, 75));
	m_container->addChild(cancel_menu);
	cancel_menu->setTouchPriority(touch_priority);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_openStage), mySGD->getFont().c_str(), 21);
	title_label->setColor(ccc3(50, 250, 255));
	title_label->setPosition(ccp(0,70));
	m_container->addChild(title_label);
	
	KSLabelTTF* sub_label = KSLabelTTF::create(CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_openGoldValue), NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, t_stage_number))->getCString(), mySGD->getFont().c_str(), 18);
	sub_label->enableOuterStroke(ccBLACK, 1.f);
	sub_label->setPosition(ccp(0,10));
	m_container->addChild(sub_label);
	
	
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	KSLabelTTF* buy_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_purchase), mySGD->getFont().c_str(), 15);
	buy_label->setPosition(ccp(0,0));
	t_label->addChild(buy_label);
	
	CCScale9Sprite* buy_back = CCScale9Sprite::create("subpop_red.png", CCRectMake(0,0,34,34), CCRectMake(16, 16, 2, 2));
	
	CCControlButton* buy_button = CCControlButton::create(t_label, buy_back);
	buy_button->addTargetWithActionForControlEvents(this, cccontrol_selector(BuyPiecePopup::buyAction), CCControlEventTouchUpInside);
	buy_button->setPreferredSize(CCSizeMake(100,50));
	buy_button->setPosition(ccp(0,-60));
	m_container->addChild(buy_button);
	
	buy_button->setTouchPriority(touch_priority);
	
	m_container->setScaleY(0.f);
	
	addChild(KSGradualValue<float>::create(0.f, 1.2f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
		addChild(KSGradualValue<float>::create(1.2f, 0.8f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.8f);
			addChild(KSGradualValue<float>::create(0.8f, 1.f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.f);}));}));}));
	
	addChild(KSGradualValue<int>::create(0, 255, 0.25f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 255);is_menu_enable = true;}));
}

void BuyPiecePopup::buyAction(CCObject* sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	int puzzle_number = myDSH->getIntegerForKey(kDSH_Key_selectedPuzzleNumber);
	
	if(mySGD->getGoodsValue(kGoodsType_gold) >= NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number))
	{
		AudioEngine::sharedInstance()->playEffect("se_buy.mp3", false);
		
		loading_layer = LoadingLayer::create(-9999);
		addChild(loading_layer, 9999);
		
		mySGD->addChangeGoods(kGoodsType_gold, -NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, stage_number), "피스오픈", CCString::createWithFormat("%d", stage_number)->getCString());
		
		PieceHistory t_history = mySGD->getPieceHistory(stage_number);
		t_history.is_open = true;
		t_history.open_type = "골드오픈";
		
		mySGD->changeGoodsTransaction({mySGD->getUpdatePieceHistoryParam(t_history, [=](Json::Value result_data)
																		 {
																			 if(result_data["result"]["code"].asInt() != GDSUCCESS)
																			 {
																				 PieceHistory r_history = mySGD->getPieceHistory(stage_number);
																				 r_history.is_open = false;
																				 r_history.open_type = "";
																				 
																				 mySGD->setPieceHistoryForNotSave(r_history);
																			 }
																		 })}, [=](Json::Value result_data)
									  {
										  loading_layer->removeFromParent();
										  if(result_data["result"]["code"].asInt() != GDSUCCESS)
										  {
											  mySGD->clearChangeGoods();
											  addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(kMyLocalKey_failPurchase)), 9999);
										  }
										  else
										  {
											  addChild(KSGradualValue<float>::create(1.f, 1.2f, 0.05f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(1.2f);
												  addChild(KSGradualValue<float>::create(1.2f, 0.f, 0.1f, [=](float t){m_container->setScaleY(t);}, [=](float t){m_container->setScaleY(0.f);}));}));
											  
											  addChild(KSGradualValue<int>::create(255, 0, 0.15f, [=](int t){KS::setOpacity(m_container, t);}, [=](int t){KS::setOpacity(m_container, 0); buy_func(); removeFromParent();}));
										  }
									  });
	}
	else
	{
		addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-100, kGoodsType_gold, [=]()
															{
																((PuzzleScene*)getParent()->getParent())->showShopPopup(kSC_gold);
															}), 9999);
		is_menu_enable = true;
		return;
	}
}