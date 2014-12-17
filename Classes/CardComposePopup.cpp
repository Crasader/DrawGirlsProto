//
//  CardComposePopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 12. 11..
//
//

#include "CardComposePopup.h"
#include "TouchSuctionLayer.h"
#include "DataStorageHub.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "CardSettingPopup.h"
#include "KSLabelTTF.h"
#include "KsLocal.h"
#include "LabelTTFMarquee.h"
#include "EffectSprite.h"
#include "ScrollMenu.h"
#include "ComposeDetailPopup.h"

CardComposePopup* CardComposePopup::create(int t_touch_priority)
{
	CardComposePopup* t_popup = new CardComposePopup();
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

void CardComposePopup::setHideFinalAction(CCObject* t_target, SEL_CallFunc t_delegate)
{
	target_final = t_target;
	delegate_final = t_delegate;
}

enum CardComposePopupZorder
{
	kCardComposePopupZorder_gray = 0,
	kCardComposePopupZorder_main_case
};

void CardComposePopup::myInit(int t_touch_priority)
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
	addChild(gray, kCardComposePopupZorder_gray);
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kCardComposePopupZorder_main_case);
	
	CCSprite* title_back = CCSprite::create("title_tab.png");
	title_back->setPosition(ccp(60,main_case->getContentSize().height-13));
	main_case->addChild(title_back);
	
	KSLabelTTF* title_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_cardCompose), mySGD->getFont().c_str(), 15);
	title_label->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	title_label->setPosition(ccpFromSize(title_back->getContentSize()/2.f) + ccp(0,2));
	title_back->addChild(title_label);
	
	CommonAnimation::applyShadow(title_label);
	
	
	CCScale9Sprite* tip_marquee_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	tip_marquee_back->setContentSize(CCSizeMake(278, 26));
	tip_marquee_back->setPosition(ccp(main_case->getContentSize().width*0.655f, main_case->getContentSize().height-21.5f));
	main_case->addChild(tip_marquee_back);
	
	LabelTTFMarquee* tipMarquee = LabelTTFMarquee::create(ccc4(0, 0, 0, 0), 278, 22, "");
	tipMarquee->addText(getLocal(LK::kMyLocalKey_detailContextShow));
	tipMarquee->setPosition(ccpFromSize(tip_marquee_back->getContentSize()/2.f));
	tipMarquee->startMarquee();
	tipMarquee->setAnchorPoint(ccp(0.5f,0.5f));
	tip_marquee_back->addChild(tipMarquee);
	
	CCSprite* tipBack = CCSprite::create("tabbutton_up.png");
	tipBack->setPosition(ccp(main_case->getContentSize().width*0.417f, main_case->getContentSize().height-23));
	main_case->addChild(tipBack);
	
	KSLabelTTF* tipLbl = KSLabelTTF::create("TIP", mySGD->getFont().c_str(), 14.f);
	tipLbl->enableOuterStroke(ccBLACK, 0.5f, 150, true);
	tipLbl->setPosition(ccpFromSize(tipBack->getContentSize()) / 2.f + ccp(0,1));
	tipBack->addChild(tipLbl);
	
	main_inner = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	main_inner->setContentSize(CCSizeMake(424, 218));
	main_inner->setPosition(ccp(main_case->getContentSize().width/2.f, main_case->getContentSize().height*0.45f + 3));
	main_case->addChild(main_inner);
	
//	CCSprite* table_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 423, 212));
//	table_back->setOpacity(100);
//	table_back->setAnchorPoint(ccp(0,0));
//	table_back->setPosition(ccp(0.5f,3));
//	main_inner->addChild(table_back);
	
	compose_table = CCTableView::create(this, CCSizeMake(423, 212));
	compose_table->setDirection(CCScrollViewDirection::kCCScrollViewDirectionHorizontal);
	compose_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	compose_table->setPosition(ccp(0.5f,3));
	compose_table->setDelegate(this);
	main_inner->addChild(compose_table);
	compose_table->setTouchPriority(touch_priority-1);
	
	if(compose_table->minContainerOffset().x > 0.f)
	{
		compose_table->setContentOffset(ccp(0, compose_table->getContentOffset().y));
		myDSH->setIntegerForKey(kDSH_Key_composeListOffsetX, compose_table->getContentOffset().x);
	}
	else
	{
		float offset_x = myDSH->getIntegerForKey(kDSH_Key_composeListOffsetX);
		if(offset_x < compose_table->minContainerOffset().x) // compose_table->maxContainerOffset().x
			compose_table->setContentOffset(ccp(compose_table->minContainerOffset().x, compose_table->getContentOffset().y));
		else if(offset_x > compose_table->maxContainerOffset().x)
			compose_table->setContentOffset(ccp(compose_table->maxContainerOffset().x, compose_table->getContentOffset().y));
		else
			compose_table->setContentOffset(ccp(offset_x, compose_table->getContentOffset().y));
		myDSH->setIntegerForKey(kDSH_Key_composeListOffsetX, compose_table->getContentOffset().x);
	}
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority-1);
	close_button->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
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
								  
								  myDSH->setIntegerForKey(kDSH_Key_composeListOffsetX, compose_table->getContentOffset().x);
								  
								  CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
															  {
																  if(target_final && delegate_final)
																	  (target_final->*delegate_final)();
																  removeFromParent();
															  });
							  });
	main_case->addChild(close_button);
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){}, [=]()
							   {
								   is_menu_enable = true;
							   });
}

CCTableViewCell* CardComposePopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	CardComposeInfo t_info = mySGD->card_compose_list[idx];
	
	CCNode* center_node = CCNode::create();
	center_node->setPosition(ccp(62.5f, 106));
	cell->addChild(center_node);
	
	CCSprite* card_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", t_info.compose_card_number.getV()));
	card_img->setScale(0.32f);
	card_img->setPosition(ccp(0,10));
	center_node->addChild(card_img);
	
	string back_filename;
	bool is_have = mySGD->isHasGottenCards(t_info.compose_card_number.getV());
	if(is_have)
		back_filename = "cardcomposition_cell_open.png";
	else
		back_filename = "cardcomposition_cell.png";
	
	CCSprite* n_case = CCSprite::create(back_filename.c_str());
	CCSprite* s_case = CCSprite::create(back_filename.c_str());
	s_case->setColor(ccGRAY);
	
	CCMenuItemSprite* t_item = CCMenuItemSprite::create(n_case, s_case, this, menu_selector(CardComposePopup::menuAction));
	t_item->setTag(idx);
	
	ScrollMenu* scroll_menu = ScrollMenu::create(t_item, NULL);
	scroll_menu->setPosition(ccp(0,0));
	center_node->addChild(scroll_menu);
	scroll_menu->setTouchPriority(touch_priority-2);
	
	if(!is_have)
	{
		CCClippingNode* sil_clipping = CCClippingNode::create(CCSprite::create("cardcomposition_cell_mask.png"));
		sil_clipping->setAlphaThreshold(0.1f);
		sil_clipping->setPosition(ccp(0,0));
		center_node->addChild(sil_clipping);
		
		EffectSprite* sil_img = EffectSprite::createWithTexture(mySIL->addImage(ccsf("card%d_invisible.png", t_info.compose_card_number.getV())));
		sil_img->setColorOpacitySilhouette(0, 0, 0, int(255*0.3f));
		sil_img->setScale(0.32f);
		sil_img->setPosition(ccp(0,10));
		sil_clipping->addChild(sil_img);
		
		CCSprite* cover_img = CCSprite::create("cardcomposition_cell_front.png");
		cover_img->setPosition(ccp(0,0));
		center_node->addChild(cover_img);
	}
	
	KSLabelTTF* title_label = KSLabelTTF::create(NSDS_GS(kSDS_CI_int1_name_s, t_info.compose_card_number.getV()).c_str(), mySGD->getFont().c_str(), 13);
	title_label->enableOuterStroke(ccBLACK, 1.f, int(255*0.6f), true);
	title_label->setPosition(ccp(0, 106-18));
	center_node->addChild(title_label);
	
	StyledLabelTTF* bottom_label = StyledLabelTTF::create(t_info.msg.getV().c_str(), mySGD->getFont().c_str(), 10, 999, StyledAlignment::kCenterAlignment);
	bottom_label->setAnchorPoint(ccp(0.5f,0.5f));
	bottom_label->setPosition(ccp(0, 27-106));
	center_node->addChild(bottom_label);
	
//	KSLabelTTF* bottom_label = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_necessaryCardAndNeedExp), t_info.material_card_list.size(), t_info.need_exp.getV()), mySGD->getFont().c_str(), 10);
//	bottom_label->enableOuterStroke(ccBLACK, 1.f, int(255*0.6f), true);
//	bottom_label->setPosition(ccp(0, 27-106));
//	center_node->addChild(bottom_label);
	
	return cell;
}

void CardComposePopup::menuAction(CCObject* t_sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int t_tag = ((CCNode*)t_sender)->getTag();
	
	ComposeDetailPopup* t_popup = ComposeDetailPopup::create(touch_priority, t_tag);
	t_popup->setHideFinalAction(target_final, delegate_final);
	getParent()->addChild(t_popup, getZOrder());
	
	target_final = NULL;
	
	CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
								{
									if(target_final && delegate_final)
										(target_final->*delegate_final)();
									removeFromParent();
								});
}

void CardComposePopup::scrollViewDidScroll( CCScrollView* view )
{
	
}

void CardComposePopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void CardComposePopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	
}

CCSize CardComposePopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(125, 212);
}

CCSize CardComposePopup::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return cellSizeForTable(table);
}

unsigned int CardComposePopup::numberOfCellsInTableView( CCTableView *table )
{
	return mySGD->card_compose_list.size();
}
