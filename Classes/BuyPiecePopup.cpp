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
#include "CommonButton.h"
#include "CommonAnimation.h"

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
	
	back_case = CCSprite::create("popup_small_back.png");
	back_case->setPosition(ccp(0,0));
	m_container->addChild(back_case);
	
	CCScale9Sprite* back_in = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	back_in->setContentSize(CCSizeMake(251, 53));
	back_in->setPosition(ccp(back_case->getContentSize().width/2.f, back_case->getContentSize().height/2.f+10));
	back_case->addChild(back_in);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority);
	close_button->setPosition(ccp(back_case->getContentSize().width/2.f-25, back_case->getContentSize().height/2.f-22));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
									CommonAnimation::closePopup(this, m_container, gray, [=](){
										
									}, [=](){
										end_func(); removeFromParent();
									});
							  });
	m_container->addChild(close_button);
	
	
	KSLabelTTF* title_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_openStage), mySGD->getFont().c_str(), 12);
	title_label->disableOuterStroke();
	title_label->setAnchorPoint(ccp(0.5f,0.5f));
	title_label->setPosition(ccp(-85,back_case->getContentSize().height/2.f-35));
	m_container->addChild(title_label);
	
	KSLabelTTF* sub_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_openStageContent), mySGD->getFont().c_str(), 12);
	// CCString::createWithFormat(myLoc->getLocalForKey(kMyLocalKey_openGoldValue), NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, t_stage_number))->getCString()
	sub_label->setAnchorPoint(ccp(0.5f,0.5f));
	sub_label->setPosition(ccpFromSize(back_in->getContentSize()/2.f));
	back_in->addChild(sub_label);
	
	
	
	CCLabelTTF* t_label = CCLabelTTF::create();
	
	KSLabelTTF* buy_label = KSLabelTTF::create(myLoc->getLocalForKey(kMyLocalKey_doOpen), mySGD->getFont().c_str(), 12);
	buy_label->disableOuterStroke();
	buy_label->setPosition(ccp(-40,0));
	t_label->addChild(buy_label);
	
	CCScale9Sprite* price_back = CCScale9Sprite::create("gray_ellipse.png", CCRectMake(0,0,82,26), CCRectMake(40,12,2,2));
	price_back->setContentSize(CCSizeMake(82, 26));
	price_back->setPosition(ccp(30,0));
	t_label->addChild(price_back);
	
	CCSprite* price_type = CCSprite::create("price_gold_img.png");
	price_type->setPosition(ccp(price_back->getContentSize().width/2.f-27,price_back->getContentSize().height/2.f));
	price_back->addChild(price_type);
	KSLabelTTF* price_label = KSLabelTTF::create(CCString::createWithFormat("%d", NSDS_GI(puzzle_number, kSDS_PZ_stage_int1_condition_gold_i, t_stage_number))->getCString(), mySGD->getFont().c_str(), 15);
	price_label->disableOuterStroke();
	price_label->setPosition(ccp(price_back->getContentSize().width/2.f+8,price_back->getContentSize().height/2.f));
	price_back->addChild(price_label);

	
	CCScale9Sprite* buy_back = CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0,0,101,44), CCRectMake(50, 21, 1, 2));
	
	CCControlButton* buy_button = CCControlButton::create(t_label, buy_back);
	buy_button->addTargetWithActionForControlEvents(this, cccontrol_selector(BuyPiecePopup::buyAction), CCControlEventTouchUpInside);
	buy_button->setPreferredSize(CCSizeMake(180,50));
	buy_button->setPosition(ccp(0,-35));
	m_container->addChild(buy_button);
	
	buy_button->setTouchPriority(touch_priority);
	
	CommonAnimation::openPopup(this, m_container, gray, [=](){
		
	}, [=](){
		is_menu_enable = true;
	});
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
		
		mySGD->addChangeGoods(CCString::createWithFormat("p_s_%d", stage_number)->getCString());
		
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
												CommonAnimation::closePopup(this, m_container, gray, [=](){
													
												}, [=](){
													buy_func(); removeFromParent();
												});
										  }
									  });
	}
	else
	{
		addChild(ASPopupView::getNotEnoughtGoodsGoShopPopup(touch_priority-100, kGoodsType_gold, [=]()
															{
																((PuzzleScene*)getParent())->showShopPopup(kSC_gold);
															}), 9999);
		is_menu_enable = true;
		return;
	}
}