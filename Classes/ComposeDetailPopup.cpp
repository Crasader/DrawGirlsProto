//
//  ComposeDetailPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 12. 12..
//
//

#include "ComposeDetailPopup.h"
#include "StarGoldData.h"
#include "DataStorageHub.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "KSLabelTTF.h"
#include "KsLocal.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
#include "ServerDataSave.h"
#include "AsyncImage.h"
#include "ScrollMenu.h"
#include "CCMenuLambda.h"
#include "ASPopupView.h"
#include "CardComposePopup.h"
#include "LoadingLayer.h"

ComposeDetailPopup* ComposeDetailPopup::create(int t_touch_priority, int t_compose_idx)
{
	ComposeDetailPopup* t_popup = new ComposeDetailPopup();
	if (t_popup && t_popup->init())
	{
		t_popup->myInit(t_touch_priority, t_compose_idx);
		t_popup->autorelease();
		return t_popup;
	}
	else
	{
		CC_SAFE_DELETE(t_popup);
		return NULL;
	}
}

void ComposeDetailPopup::setHideFinalAction(CCObject* t_target, SEL_CallFunc t_delegate)
{
	target_final = t_target;
	delegate_final = t_delegate;
}

enum ComposeDetailPopupZorder
{
	kComposeDetailPopupZorder_gray = 0,
	kComposeDetailPopupZorder_main_case
};

ComposeDetailPopup::~ComposeDetailPopup()
{
	while(!card_data_list.empty())
	{
		CardSortInfo* t_info = card_data_list.back();
		card_data_list.pop_back();
		delete t_info;
	}
}

void ComposeDetailPopup::myInit(int t_touch_priority, int t_compose_idx)
{
	compose_idx = t_compose_idx;
	is_menu_enable = false;
	touch_priority = t_touch_priority;
	
	sum_exp_value = 0;
	
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
	addChild(gray, kComposeDetailPopupZorder_gray);
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kComposeDetailPopupZorder_main_case);
	
	is_full = false;
	
	vector<KSProtectVar<int>> t_material_list = mySGD->card_compose_list[compose_idx].material_card_list;
	int material_count = t_material_list.size();
	
	card_data_list.clear();
	int card_data_count = mySGD->getHasGottenCardsSize();
	for(int i=0;i<card_data_count;i++)
	{
		CardSortInfo t_info = mySGD->getHasGottenCardData(i);
		
		bool is_found = false;
		int t_card_number = t_info.card_number.getV();
		
		for(int j=0;!is_found && j<material_count;j++)
		{
			if(t_card_number == t_material_list[j].getV())
				is_found = true;
		}
		
		if(!is_found && t_info.count.getV() >= 2)
		{
			CardSortInfo* r_info = new CardSortInfo();
			r_info->card_number = t_info.card_number.getV();
			r_info->take_number = t_info.take_number.getV();
			r_info->grade = t_info.grade.getV();
			r_info->rank = t_info.rank.getV();
			r_info->user_ment = t_info.user_ment.getV();
			r_info->is_morphing = t_info.is_morphing.getV();
			r_info->count = t_info.count.getV();
			
			card_data_list.push_back(r_info);
		}
	}
	
	CCScale9Sprite* card_list_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	card_list_back->setContentSize(CCSizeMake(279, 238));
	card_list_back->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(-75,0));
	main_case->addChild(card_list_back, 1);
	
	card_table = CCTableView::create(this, CCSizeMake(275, 175));
	card_table->setDirection(CCScrollViewDirection::kCCScrollViewDirectionVertical);
	card_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	card_table->setPosition(ccpFromSize(card_list_back->getContentSize()/2.f) + ccp(0,29) + ccp(-137.5f,-87.5f));
	card_table->setDelegate(this);
	card_list_back->addChild(card_table);
	card_table->setTouchPriority(touch_priority-1);
	
	CCScale9Sprite* card_list_tab = CCScale9Sprite::create("tabbutton_up.png", CCRectMake(0, 0, 74, 33), CCRectMake(36, 16, 2, 1));
	card_list_tab->setContentSize(CCSizeMake(95, 33));
	card_list_tab->setPosition(ccp(card_list_back->getContentSize().width/2.f, card_list_back->getContentSize().height-2.5f));
	card_list_back->addChild(card_list_tab);
	
	KSLabelTTF* card_list_title = KSLabelTTF::create(getLocal(LK::kMyLocalKey_materialCard), mySGD->getFont().c_str(), 13);
	card_list_title->setPosition(ccpFromSize(card_list_tab->getContentSize()/2.f));
	card_list_tab->addChild(card_list_title);
	
	CCScale9Sprite* necessary_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	necessary_back->setContentSize(CCSizeMake(139, 177));
	necessary_back->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(143,30.5f));
	main_case->addChild(necessary_back);
	
	CCScale9Sprite* necessary_tab = CCScale9Sprite::create("tabbutton_up.png", CCRectMake(0, 0, 74, 33), CCRectMake(36, 16, 2, 1));
	necessary_tab->setContentSize(CCSizeMake(85, 33));
	necessary_tab->setPosition(ccp(necessary_back->getContentSize().width/2.f, necessary_back->getContentSize().height-2.5f));
	necessary_back->addChild(necessary_tab);
	
	KSLabelTTF* necessary_tab_title = KSLabelTTF::create(getLocal(LK::kMyLocalKey_necessaryCard), mySGD->getFont().c_str(), 13);
	necessary_tab_title->setPosition(ccpFromSize(necessary_tab->getContentSize()/2.f));
	necessary_tab->addChild(necessary_tab_title);
	
	is_necessary_on = true;
	
	for(int i=material_count-1;i>=0;i--)
	{
		CardSortInfo t_info = mySGD->getHasGottenCardDataForCardNumber(t_material_list[i].getV());
		CCPoint t_position = ccp(0,0);
		if(i == 0)
			t_position = ccp(37,117);
		else if(i == 1)
			t_position = ccp(60,48);
		else if(i == 2)
			t_position = ccp(102,105);
		if(t_info.count.getV() >= 1)
		{
			CCClippingNode* clipping_node = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			clipping_node->setAlphaThreshold(0.1f);
			clipping_node->setPosition(t_position);
			
			GraySprite* t_card = GraySprite::createWithTexture(mySIL->addImage(ccsf("card%d_visible.png", t_info.card_number.getV())));
			t_card->setScale(0.2f);
			t_card->setGray(t_info.count.getV() < 2);
			clipping_node->addChild(t_card);
			
			CCSprite* frame_img = CCSprite::create("cha_cardcase.png");
			frame_img->setPosition(ccp(0,0));
			clipping_node->addChild(frame_img);
			
			KSLabelTTF* no_label = KSLabelTTF::create(ccsf("No.%d", t_info.card_number.getV()), mySGD->getFont().c_str(), 9);
			no_label->setPosition(ccp(frame_img->getContentSize().width-27, 12));
			frame_img->addChild(no_label);
			
			KSLabelTTF* count_label = KSLabelTTF::create(ccsf("%d", t_info.count.getV()), mySGD->getFont().c_str(), 9);
			count_label->setPosition(ccp(frame_img->getContentSize().width-16, frame_img->getContentSize().height-12));
			frame_img->addChild(count_label);
			
			necessary_back->addChild(clipping_node);
			
			if(t_info.count.getV() < 2)
				is_necessary_on = false;
		}
		else
		{
			is_necessary_on = false;
			CCSprite* not_have_back = CCSprite::create("cardsetting_off.png");
			not_have_back->setPosition(t_position);
			necessary_back->addChild(not_have_back);
			
			KSLabelTTF* need_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_necessaryPlease), mySGD->getFont().c_str(), 11);
			need_ment->enableOuterStroke(ccBLACK, 0.5f, int(255*0.6f), true);
			need_ment->setPosition(ccpFromSize(not_have_back->getContentSize()/2.f) + ccp(0,4));
			not_have_back->addChild(need_ment);
			
			KSLabelTTF* no_label = KSLabelTTF::create(ccsf("No.%d", t_material_list[i].getV()), mySGD->getFont().c_str(), 12);
			no_label->setColor(ccc3(255, 220, 150));
			no_label->enableOuterStroke(ccBLACK, 0.5f, int(255*0.6f), true);
			no_label->setPosition(ccpFromSize(not_have_back->getContentSize()/2.f) + ccp(0,-8));
			not_have_back->addChild(no_label);
		}
	}
	
	CCSprite* selected_card_back = CCSprite::create("cha_selectbox.png");
	selected_card_back->setPosition(ccp(card_list_back->getContentSize().width/2.f, selected_card_back->getContentSize().height/2.f+1));
	card_list_back->addChild(selected_card_back);
	
	KSLabelTTF* selected_card_list_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_selectedCardList), mySGD->getFont().c_str(), 10);
	selected_card_list_ment->enableOuterStroke(ccBLACK, 1, 127, true);
	selected_card_list_ment->setAnchorPoint(ccp(0,0));
	selected_card_list_ment->setPosition(ccp(6, selected_card_back->getContentSize().height - selected_card_list_ment->getContentSize().height-3.5f));
	selected_card_back->addChild(selected_card_list_ment);
	
	sum_exp_label = KSLabelTTF::create(ccsf("EXP %s/%s", KS::insert_separator(sum_exp_value.getV()).c_str(), KS::insert_separator(mySGD->card_compose_list[compose_idx].need_exp.getV()).c_str()), mySGD->getFont().c_str(), 12);
	sum_exp_label->setGradientColor(ccc4(255, 255, 40, 255), ccc4(255, 160, 20, 255), ccp(0,-1));
	sum_exp_label->enableOuterStroke(ccBLACK, 1.f, int(255*0.7f), true);
	sum_exp_label->setAnchorPoint(ccp(1,0.5f));
	sum_exp_label->setPosition(ccpFromSize(selected_card_back->getContentSize()) + ccp(-7,-9));
	selected_card_back->addChild(sum_exp_label);
	
	KSLabelTTF* selected_card_list_sub_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_10cardsStrengthenPossibleAtOne), mySGD->getFont().c_str(), 8);
	selected_card_list_sub_ment->setColor(ccc3(30, 25, 70));
	selected_card_list_sub_ment->setAnchorPoint(ccp(0,0));
	selected_card_list_sub_ment->setPosition(ccp(selected_card_list_ment->getPositionX() + selected_card_list_ment->getContentSize().width + 5, selected_card_list_ment->getPositionY()));
	selected_card_back->addChild(selected_card_list_sub_ment);
	
	target_card_number_list.clear();
	
//	CCSprite* selected_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 264, 35));
//	selected_back->setOpacity(100);
//	selected_back->setAnchorPoint(ccp(0,0));
//	selected_back->setPosition(ccp(7,6));
//	selected_card_back->addChild(selected_back);
	
	selected_table = CCTableView::create(this, CCSizeMake(264, 35));
	selected_table->setDirection(CCScrollViewDirection::kCCScrollViewDirectionHorizontal);
	selected_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	selected_table->setPosition(ccp(7,6));
	selected_table->setDelegate(this);
	selected_card_back->addChild(selected_table);
//	selected_table->setTouchPriority(touch_priority-1);
	
	card_table->reloadData();
	selected_table->reloadData();
	
	CCLabelTTF* t_button_label = CCLabelTTF::create();
	KSLabelTTF* compose_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_doCompose), mySGD->getFont().c_str(), 16);
	compose_label->setPosition(ccp(0,9.5f));
	t_button_label->addChild(compose_label);
	
	price_back = CCScale9Sprite::create("common_in.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	price_back->setContentSize(CCSizeMake(90, 20));
	price_back->setPosition(ccp(0,-9.5f));
	t_button_label->addChild(price_back);
	
	if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass11) > 0)
	{
		price_icon = CCSprite::create("icon_p11.png");
		price_icon->setScale(0.8f);
		price_value = KSLabelTTF::create(KS::insert_separator(mySGD->getGoodsValue(GoodsType::kGoodsType_pass11)).c_str(), mySGD->getFont().c_str(), 14);
		price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
		price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
		price_back->addChild(price_icon);
		price_back->addChild(price_value);
		
		KSLabelTTF* take_count = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(GoodsType::kGoodsType_pass11)), mySGD->getFont().c_str(), 11);
		take_count->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		
		float wide_value = take_count->getContentSize().width - 40;
		if(wide_value < 0)
			wide_value = 0;
		
		take_back = CCScale9Sprite::create("common_count.png", CCRectMake(0, 0, 60, 20), CCRectMake(29, 9, 2, 2));
		take_back->setContentSize(CCSizeMake(60+wide_value, 20));
		take_back->setAnchorPoint(ccp(1.f,0.5f));
		take_back->setPosition(ccp(68,27));
		t_button_label->addChild(take_back);
		
		take_count->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
		take_back->addChild(take_count);
	}
	else
	{
		price_icon = CCSprite::create("icon_p1.png");
		price_icon->setScale(0.8f);
		price_value = KSLabelTTF::create(KS::insert_separator(mySGD->card_compose_list[compose_idx].need_stone.getV()).c_str(), mySGD->getFont().c_str(), 14);
		price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
		price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
		price_back->addChild(price_icon);
		price_back->addChild(price_value);
		
		KSLabelTTF* take_count = KSLabelTTF::create(ccsf(myLoc->getLocalForKey(LK::kMyLocalKey_takeCount), mySGD->getGoodsValue(GoodsType::kGoodsType_pass1)), mySGD->getFont().c_str(), 11);
		take_count->enableOuterStroke(ccBLACK, 0.3f, 50, true);
		
		float wide_value = take_count->getContentSize().width - 40;
		if(wide_value < 0)
			wide_value = 0;
		
		take_back = CCScale9Sprite::create("common_count.png", CCRectMake(0, 0, 60, 20), CCRectMake(29, 9, 2, 2));
		take_back->setContentSize(CCSizeMake(60+wide_value, 20));
		take_back->setAnchorPoint(ccp(1.f,0.5f));
		take_back->setPosition(ccp(68,27));
		t_button_label->addChild(take_back);
		
		take_count->setPosition(ccpFromSize(take_back->getContentSize()/2.f));
		take_back->addChild(take_count);
	}
	
	compose_button = CCControlButton::create(t_button_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	compose_button->setPreferredSize(CCSizeMake(147, 63));
	compose_button->setPosition(ccp(necessary_back->getPositionX(), 50));
	compose_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ComposeDetailPopup::composeAction), CCControlEventTouchUpInside);
	main_case->addChild(compose_button);
	compose_button->setTouchPriority(touch_priority-2);
	
//	compose_button->setEnabled(false);
//	compose_button->setColor(ccGRAY);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority-1);
	close_button->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  CardComposePopup* t_popup = CardComposePopup::create(touch_priority);
								  t_popup->setHideFinalAction(target_final, delegate_final);
								  getParent()->addChild(t_popup, getZOrder());
								  
								  target_final = NULL;
								  
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

CCTableViewCell* ComposeDetailPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
	if(table == card_table)
	{
		for(int i=idx*4;i<idx*4+4 && i<card_data_list.size();i++)
		{
			CardSortInfo* t_info = card_data_list[i];
			
			int card_number = t_info->card_number.getV();
			
			int target_count = 0;
			for(int i=0;i<target_card_number_list.size();i++)
			{
				int t_n = target_card_number_list[i];
				if(card_number == t_n)
				{
					target_count++;
				}
			}
			
			int exp_value = NSDS_GI(kSDS_CI_int1_exp_i, card_number);
			CCPoint card_position = ccp(35.f + (i-idx*4)*(68.f), cellSizeForTable(table).height/2.f);
			
			CCClippingNode* n_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			n_clipping->setAlphaThreshold(0.1f);
			
			AsyncImage* n_card = AsyncImage::create(mySIL->getDocumentPath() + ccsf("card%d_visible.png", card_number), "loading_card.png");
			n_clipping->addChild(n_card);
			n_card->setScale(0.2f);
			
			CCSprite* n_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
			n_clipping->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
			n_node->addChild(n_clipping);
			
			CCSprite* n_frame = CCSprite::create("cha_cardcase.png");
			n_frame->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
			n_node->addChild(n_frame);
			
			KSLabelTTF* n_label = KSLabelTTF::create(ccsf("EXP %d", exp_value), mySGD->getFont().c_str(), 9);
			n_label->setPosition(ccp(n_node->getContentSize().width-16, 12));
			n_frame->addChild(n_label);
			
			KSLabelTTF* n_count_label = KSLabelTTF::create(ccsf("%d", t_info->count.getV() - target_count), mySGD->getFont().c_str(), 9);
			n_count_label->setPosition(ccp(n_node->getContentSize().width-4, n_node->getContentSize().height-1));
			n_frame->addChild(n_count_label);
			
			CCClippingNode* s_clipping = CCClippingNode::create(CCSprite::create("cardsetting_mask.png"));
			s_clipping->setAlphaThreshold(0.1f);
			
			AsyncImage* s_card = AsyncImage::create(mySIL->getDocumentPath() + ccsf("card%d_visible.png", card_number), "loading_card.png");
			s_clipping->addChild(s_card);
			s_card->setScale(0.2f);
			s_card->setColor(ccGRAY);
			
			CCSprite* s_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
			s_clipping->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
			s_node->addChild(s_clipping);
			
			CCSprite* s_frame = CCSprite::create("cha_cardcase.png");
			s_frame->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
			s_node->addChild(s_frame);
			
			KSLabelTTF* s_label = KSLabelTTF::create(ccsf("EXP %d", exp_value), mySGD->getFont().c_str(), 9);
			s_label->setPosition(ccp(s_node->getContentSize().width-16, 12));
			s_frame->addChild(s_label);
			
			KSLabelTTF* s_count_label = KSLabelTTF::create(ccsf("%d", t_info->count.getV() - target_count), mySGD->getFont().c_str(), 9);
			s_count_label->setPosition(ccp(s_node->getContentSize().width-4, s_node->getContentSize().height-1));
			s_frame->addChild(s_count_label);
			
			CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(ComposeDetailPopup::cardAction));
			t_card_item->setTag(card_number);
			
			ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
			t_card_menu->setPosition(card_position);
			cell->addChild(t_card_menu);
			t_card_menu->setTouchPriority(touch_priority-2);
			
			if(is_full || t_info->count.getV() - target_count <= 1)
			{
				n_card->setGray(true);
				s_card->setGray(true);
				t_card_item->setEnabled(false);
			}
		}
	}
	else if(table == selected_table)
	{
		int card_number = target_card_number_list[idx];
		
		CCSprite* n_mask = CCSprite::create("cardsetting_mask.png");
		n_mask->setScale(1.f/0.2f*0.076f);
		CCClippingNode* n_clipping = CCClippingNode::create(n_mask);
		n_clipping->setAlphaThreshold(0.1f);
		
		CCSprite* n_card = mySIL->getLoadedImg(ccsf("card%d_visible.png", card_number));
		n_clipping->addChild(n_card);
		n_card->setScale(0.076f);
		
		CCSprite* n_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, n_card->getContentSize().width*n_card->getScale(), n_card->getContentSize().height*n_card->getScale()));
		n_clipping->setPosition(ccp(n_node->getContentSize().width/2.f, n_node->getContentSize().height/2.f));
		n_node->addChild(n_clipping);
		
		int t_exp_value = NSDS_GI(kSDS_CI_int1_exp_i, card_number);
		
		KSLabelTTF* n_exp = KSLabelTTF::create(KS::insert_separator(t_exp_value).c_str(), mySGD->getFont().c_str(), 12);
		n_exp->enableOuterStroke(ccBLACK, 1.f, 255, true);
		n_exp->setPosition(ccpFromSize(n_node->getContentSize()/2.f));
		n_node->addChild(n_exp);
		
		CCSprite* s_mask = CCSprite::create("cardsetting_mask.png");
		s_mask->setScale(1.f/0.2f*0.076f);
		CCClippingNode* s_clipping = CCClippingNode::create(s_mask);
		s_clipping->setAlphaThreshold(0.1f);
		
		CCSprite* s_card = mySIL->getLoadedImg(ccsf("card%d_visible.png", card_number));
		s_card->setColor(ccGRAY);
		s_clipping->addChild(s_card);
		s_card->setScale(0.076f);
		
		CCSprite* s_node = CCSprite::create("whitepaper2.png", CCRectMake(0, 0, s_card->getContentSize().width*s_card->getScale(), s_card->getContentSize().height*s_card->getScale()));
		s_clipping->setPosition(ccp(s_node->getContentSize().width/2.f, s_node->getContentSize().height/2.f));
		s_node->addChild(s_clipping);
		
		KSLabelTTF* s_exp = KSLabelTTF::create(KS::insert_separator(t_exp_value).c_str(), mySGD->getFont().c_str(), 12);
		s_exp->enableOuterStroke(ccBLACK, 1.f, 255, true);
		s_exp->setPosition(ccpFromSize(n_node->getContentSize()/2.f));
		s_node->addChild(s_exp);
		
		CCMenuItemSprite* t_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(ComposeDetailPopup::unselectedAction));
		t_item->setTag(idx);
		t_item->setPosition(ccp(13.2f, 17.5f));
		
		ScrollMenu* selected_menu = ScrollMenu::create(t_item, NULL);
		selected_menu->setPosition(ccp(0,0));
		cell->addChild(selected_menu);
		selected_menu->setTouchPriority(touch_priority-2-idx);
	}
	
	
	return cell;
}

void ComposeDetailPopup::scrollViewDidScroll( CCScrollView* view ){}
void ComposeDetailPopup::scrollViewDidZoom( CCScrollView* view ){}
void ComposeDetailPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell ){}

CCSize ComposeDetailPopup::cellSizeForTable( CCTableView *table )
{
	if(table == card_table)
		return CCSizeMake(275, 90);
	else if(table == selected_table)
		return CCSizeMake(26.4f, 35);
	else
		return CCSizeMake(0, 0);
}

CCSize ComposeDetailPopup::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return cellSizeForTable(table);
}

unsigned int ComposeDetailPopup::numberOfCellsInTableView( CCTableView *table )
{
	if(table == card_table)
	{
		int has_count = card_data_list.size();
		if(has_count == 0)
			return 0;
		else
			return (has_count-1)/4+1;
	}
	else if(table == selected_table)
	{
		return target_card_number_list.size();
	}
	else
		return 0;
}

void ComposeDetailPopup::composeAction(CCObject* t_sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	if(!is_necessary_on)
	{
		addChild(ASPopupView::getCommonNoti(touch_priority-100, getLocal(LK::kMyLocalKey_noti), getLocal(LK::kMyLocalKey_shortageMaterialCard)), 9999);
		is_menu_enable = true;
	}
	else if(sum_exp_value.getV() < mySGD->card_compose_list[compose_idx].need_exp.getV())
	{
		addChild(ASPopupView::getCommonNoti(touch_priority-100, getLocal(LK::kMyLocalKey_noti), getLocal(LK::kMyLocalKey_notEnoughtEXP)), 9999);
		is_menu_enable = true;
	}
	else if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass11) <= 0 && mySGD->getGoodsValue(GoodsType::kGoodsType_pass1) < mySGD->card_compose_list[compose_idx].need_stone.getV())
	{
		addChild(ASPopupView::getCommonNoti(touch_priority-100, getLocal(LK::kMyLocalKey_noti), getLocal(LK::kMyLocalKey_notEnoughtP1)), 9999);
		is_menu_enable = true;
	}
	else
	{
		ASPopupView* t_popup = ASPopupView::create(touch_priority - 100);
		CCSize screen_size = CCEGLView::sharedOpenGLView()->getFrameSize();
		float screen_scale_x = screen_size.width/screen_size.height/1.5f;
		if(screen_scale_x < 1.f)
			screen_scale_x = 1.f;
		
		float height_value = 320.f;
		if(myDSH->screen_convert_rate < 1.f)
			height_value = 320.f/myDSH->screen_convert_rate;
		
		if(height_value < myDSH->ui_top)
			height_value = myDSH->ui_top;
		
		t_popup->setDimmedSize(CCSizeMake(screen_scale_x*480.f, height_value));// /myDSH->screen_convert_rate));
		
		t_popup->setDimmedPosition(ccp(240, 160));
		t_popup->setBasePosition(ccp(240, 160));
		
		
		CCNode* t_container = CCNode::create();
		t_popup->setContainerNode(t_container);
		
		CCSprite* case_back = CCSprite::create("popup_small_back.png");
		case_back->setPosition(CCPointZero);
		t_container->addChild(case_back);
		
		CCScale9Sprite* content_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
		content_back->setContentSize(CCSizeMake(251, 113));
		content_back->setPosition(ccp(0.0,-12)); 			// dt (0.0,-4.5)
		t_container->addChild(content_back);
		
		KSLabelTTF* title_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_noti), mySGD->getFont().c_str(), 12);
		title_label->disableOuterStroke();
		title_label->setPosition(ccp(-85,case_back->getContentSize().height/2.f-35));
		t_container->addChild(title_label);
		
		
		KSLabelTTF* ment_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_composeUsedCardIsRemove), mySGD->getFont().c_str(), 12);
		ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
		content_back->addChild(ment_label);
		
		CommonButton* close_button = CommonButton::createCloseButton(t_popup->getTouchPriority()-5);
		close_button->setPosition(ccp(case_back->getContentSize().width/2.f-20,case_back->getContentSize().height/2.f-18));
		close_button->setFunction([=](CCObject* sender)
								  {
									  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
									  
									  CommonAnimation::closePopup(t_popup, t_container,
																  t_popup->getDimmedSprite(), nullptr,
																  [=]()
																  {
																	  is_menu_enable = true;
																	  t_popup->removeFromParent();
																  });
								  });
		t_container->addChild(close_button);
		
		
		CommonButton* continue_button = CommonButton::create(getLocal(LK::kMyLocalKey_doCompose), 12, CCSizeMake(101, 44), CCScale9Sprite::create("achievement_button_success.png", CCRectMake(0, 0, 101, 44), CCRectMake(50, 21, 1, 2)), t_popup->getTouchPriority()-5);
		continue_button->setPosition(ccp(0,case_back->getContentSize().height/2.f*-1+45));
		continue_button->setFunction([=](CCObject* sender)
									 {
										 AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
										 
										 CommonAnimation::closePopup(t_popup, t_container,
																	 t_popup->getDimmedSprite(), nullptr,
																	 [=]()
																	 {
																		 composeOn();
																		 
																		 t_popup->removeFromParent();
																	 });
									 });
		t_container->addChild(continue_button);
		
		
		content_back->setContentSize(content_back->getContentSize() + CCSizeMake(0, -53));
		content_back->setPositionY(content_back->getPositionY() + 22.5f);
		
		ment_label->setPosition(ccpFromSize(content_back->getContentSize()/2.f));
		
		t_popup->getDimmedSprite()->setOpacity(0);
		CommonAnimation::openPopup(t_popup, t_container, t_popup->getDimmedSprite());
		
		addChild(t_popup, 9999);
	}
	
}

void ComposeDetailPopup::cardAction(CCObject *t_sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int card_number = ((CCNode*)t_sender)->getTag();
	
	target_card_number_list.push_back(card_number);
	
	sum_exp_value = sum_exp_value.getV() + NSDS_GI(kSDS_CI_int1_exp_i, card_number);
	sum_exp_label->setString(ccsf("EXP %s/%s", KS::insert_separator(sum_exp_value.getV()).c_str(), KS::insert_separator(mySGD->card_compose_list[compose_idx].need_exp.getV()).c_str()));
	
//	if(sum_exp_value.getV() >= mySGD->card_compose_list[compose_idx].need_exp.getV())
//	{
//		compose_button->setColor(ccWHITE);
//		compose_button->setEnabled(true);
//	}
	
	if(target_card_number_list.size() >= 10)
		is_full = true;
	
	selected_table->reloadData();
	selected_table->setContentOffset(ccp(MIN(selected_table->minContainerOffset().x, 0), selected_table->getContentOffset().y));
	
	CCPoint t_offset = card_table->getContentOffset();
	card_table->reloadData();
	card_table->setContentOffset(t_offset);
	
	is_menu_enable = true;
}

void ComposeDetailPopup::unselectedAction(CCObject* t_sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	int t_idx = ((CCNode*)t_sender)->getTag();
	int card_number = target_card_number_list[t_idx];
	
	sum_exp_value = sum_exp_value.getV() - NSDS_GI(kSDS_CI_int1_exp_i, card_number);
	sum_exp_label->setString(ccsf("EXP %s/%s", KS::insert_separator(sum_exp_value.getV()).c_str(), KS::insert_separator(mySGD->card_compose_list[compose_idx].need_exp.getV()).c_str()));
	
//	if(sum_exp_value.getV() < mySGD->card_compose_list[compose_idx].need_exp.getV())
//	{
//		compose_button->setColor(ccGRAY);
//		compose_button->setEnabled(false);
//	}
	
	target_card_number_list.erase(target_card_number_list.begin() + t_idx);
	
	is_full = false;
	
	CCPoint keep_offset = selected_table->getContentOffset();
	selected_table->reloadData();
	selected_table->setContentOffset(ccp(MIN(MAX(keep_offset.x, selected_table->minContainerOffset().x), 0), keep_offset.y));
	
	CCPoint t_offset = card_table->getContentOffset();
	card_table->reloadData();
	card_table->setContentOffset(t_offset);
	
	is_menu_enable = true;
}

void ComposeDetailPopup::composeOn()
{
	loading_layer = LoadingLayer::create(touch_priority-200);
	addChild(loading_layer, 999);
	loading_layer->startLoading();
	
	Json::Value param;
	param["memberID"] = myHSP->getSocialID();
	param["no"] = mySGD->card_compose_list[compose_idx].compose_no.getV();
	if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass11) > 0)
		param["exchangeID"] = NSDS_GS(kSDS_GI_shopComposeCardPass_exchangeID_s);
	else
		param["exchangeID"] = NSDS_GS(kSDS_GI_shopComposeCardStone_exchangeID_s);
	
	int material_count = target_card_number_list.size();
	for(int i=0;i<material_count;i++)
	{
		param["cards"][i] = target_card_number_list[i];
	}
	
	myHSP->command("composecard", param, json_selector(this, ComposeDetailPopup::resultCompose));
}

void ComposeDetailPopup::resultCompose(Json::Value result_data)
{
//	CCLog("resultCompose : \n%s", result_data.toStyledString().c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		loading_layer->removeFromParent();
		
		string fiverocks_goods_type;
		
		if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass11) <= 0)
			fiverocks_goods_type = "p11";
		else
			fiverocks_goods_type = "p1";
		fiverocks::FiveRocksBridge::trackEvent("Game", "Compose", ccsf("%d", mySGD->card_compose_list[compose_idx].compose_no.getV()), fiverocks_goods_type.c_str());
		
		mySGD->refreshCardData(result_data["card"]);
		
		Json::Value expCards = result_data["expCards"];
		for(int i=0;i<expCards.size();i++)
		{
			mySGD->refreshCardData(expCards[i]);
		}
		
		Json::Value materialCards = result_data["materialCards"];
		for(int i=0;i<materialCards.size();i++)
		{
			mySGD->refreshCardData(materialCards[i]);
		}
		
		Json::Value propertie_list = result_data["myItem"]["list"];
		for(int i=0;i<propertie_list.size();i++)
		{
			mySGD->refreshGoodsData(propertie_list[i]["type"].asString(), propertie_list[i]["count"].asInt());
		}
		
		CCSprite* zoom_3 = CCSprite::create("zoom_3.png");
		zoom_3->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
		zoom_3->setColor(ccc3(255, 209, 79));
		zoom_3->setScale(0.f);
		zoom_3->setPosition(ccp(240,172));
		addChild(zoom_3, 100);
		
		CCSprite* zoom_5 = CCSprite::create("zoom_5.png");
		zoom_5->setBlendFunc({GL_SRC_ALPHA, GL_ONE});
		zoom_5->setScale(0.f);
		zoom_5->setOpacity(150);
		zoom_5->setPosition(ccp(240,172));
		addChild(zoom_5, 101);
		
		CCSprite* black_img = CCSprite::create("whitePaper.png");
		black_img->setColor(ccBLACK);
		black_img->setOpacity(0);
		black_img->setScaleX(gray->getScaleX());
		black_img->setScaleY(gray->getScaleY());
		black_img->setPosition(gray->getPosition());
		addChild(black_img, 98);
		
		CCSprite* card_img = mySIL->getLoadedImg(ccsf("card%d_visible.png", mySGD->card_compose_list[compose_idx].compose_card_number.getV()));
		card_img->setScale(0.5f);
		card_img->setPosition(ccp(240,172));
		addChild(card_img, 99);
		
		CCSprite* top_case = CCSprite::create("diary_frame_top.png");
		top_case->setPosition(ccpFromSize(card_img->getContentSize()/2.f) + ccp(0,215));
		card_img->addChild(top_case);
		
		CCSprite* bottom_case = CCSprite::create("diary_frame_bottom.png");
		bottom_case->setPosition(ccpFromSize(card_img->getContentSize()/2.f) + ccp(0,-215));
		card_img->addChild(bottom_case);
		
		CCSprite* left_case = CCSprite::create("diary_frame_left.png");
		left_case->setPosition(ccpFromSize(card_img->getContentSize()/2.f) + ccp(-160,0));
		card_img->addChild(left_case);
		
		CCSprite* right_case = CCSprite::create("diary_frame_right.png");
		right_case->setPosition(ccpFromSize(card_img->getContentSize()/2.f) + ccp(160,0));
		card_img->addChild(right_case);
		
		KS::setOpacity(card_img, 0);
		
		AudioEngine::sharedInstance()->playEffect("se_clearreward.mp3");
		
		zoom_3->addChild(KSGradualValue<float>::create(0.f, 1.3f, 11.f/30.f, [=](float t_f)
													   {
														   zoom_3->setScale(t_f);
													   }, [=](float t_f)
													   {
														   zoom_3->setScale(t_f);
														   zoom_3->addChild(KSTimer::create(2.f/30.f, [=]()
														   {
															   zoom_3->addChild(KSGradualValue<float>::create(1.3f, 2.f, 11.f/30.f, [=](float t_f)
																											  {
																												  zoom_3->setScale(t_f);
																											  }, [=](float t_f)
																											  {
																												  zoom_3->setScale(t_f);
																											  }));
															   zoom_3->addChild(KSTimer::create(3.f/30.f, [=]()
																								{
																									KS::setOpacity(card_img, 255);
																									
																									zoom_3->addChild(KSGradualValue<int>::create(255, 0, 8.f/30.f, [=](int t_i)
																																				 {
																																					 zoom_3->setOpacity(t_i);
																																				 }, [=](int t_i)
																																				 {
																																					 zoom_3->setOpacity(t_i);
																																					 zoom_3->removeFromParent();
																																				 }));
																								}));
														   }));
													   }));
		
		zoom_5->addChild(KSGradualValue<float>::create(0.f, 0.75f, 11.f/30.f, [=](float t_f)
													   {
														   zoom_5->setScale(t_f);
													   }, [=](float t_f)
													   {
														   zoom_5->setScale(t_f);
														   zoom_5->addChild(KSTimer::create(2.f/30.f, [=]()
																							{
																								zoom_5->addChild(KSGradualValue<float>::create(0.75f, 1.5f, 11.f/30.f, [=](float t_f)
																																			   {
																																				   zoom_5->setScale(t_f);
																																			   }, [=](float t_f)
																																			   {
																																				   zoom_5->setScale(t_f);
																																			   }));
																								zoom_5->addChild(KSTimer::create(3.f/30.f, [=]()
																																 {
																																	 zoom_5->addChild(KSGradualValue<int>::create(150, 0, 8.f/30.f, [=](int t_i)
																																												  {
																																													  zoom_5->setOpacity(t_i);
																																												  }, [=](int t_i)
																																												  {
																																													  zoom_5->setOpacity(t_i);
																																													  zoom_5->removeFromParent();
																																												  }));
																																 }));
																							}));
													   }));
		
		black_img->addChild(KSTimer::create(2.f/30.f, [=]()
											{
												black_img->addChild(KSGradualValue<int>::create(150, 255, 5.f/30.f, [=](int t_i)
																								{
																									black_img->setOpacity(t_i);
																								}, [=](int t_i)
																								{
																									black_img->setOpacity(t_i);
																									black_img->addChild(KSTimer::create(4.f/30.f, [=]()
																																		{
																																			black_img->addChild(KSGradualValue<int>::create(255, 150, 5.f/30.f, [=](int t_i)
																																															{
																																																black_img->setOpacity(t_i);
																																															}, [=](int t_i)
																																															{
																																																black_img->setOpacity(t_i);
																																															}));
																																		}));
																								}));
											}));
		
		CommonButton* take_card_button = CommonButton::create(getLocal(LK::kMyLocalKey_onePercentGame10), 12, CCSizeMake(100, 50), CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)), touch_priority-100);
		take_card_button->setPosition(ccp(240,-100));
		take_card_button->setFunction([=](CCObject* t_sender)
									  {
										  take_card_button->setVisible(false);
										  take_card_button->setEnabled(false);
										  card_img->addChild(KSGradualValue<float>::create(0.5f, 0.5f*1.3f, 5.f/30.f, [=](float t_f)
																						   {
																							   card_img->setScale(t_f);
																						   }, [=](float t_f)
																						   {
																							   card_img->setScale(t_f);
																						   }));
										  card_img->addChild(KSGradualValue<int>::create(255, 0, 5.f/30.f, [=](int t_i)
																						 {
																							 KS::setOpacity(card_img, t_i);
																						 }, [=](int t_i)
																						 {
																							 KS::setOpacity(card_img, t_i);
																						 }));
										  black_img->addChild(KSGradualValue<int>::create(150, 0, 5.f/30.f, [=](int t_i)
																						  {
																							  black_img->setOpacity(t_i);
																						  }, [=](int t_i)
																						  {
																							  black_img->setOpacity(t_i);
																							  
																							  CardComposePopup* t_popup = CardComposePopup::create(touch_priority);
																							  t_popup->setHideFinalAction(target_final, delegate_final);
																							  getParent()->addChild(t_popup, getZOrder());
																							  
																							  target_final = NULL;
																							  
																							  CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
																														  {
																															  if(target_final && delegate_final)
																																  (target_final->*delegate_final)();
																															  removeFromParent();
																														  });
																							  
																							  take_card_button->removeFromParent();
																						  }));
									  });
		addChild(take_card_button, 999);
		take_card_button->setEnabled(false);
		
		addChild(KSTimer::create(24.f/30.f, [=]()
								 {
									 addChild(KSGradualValue<CCPoint>::create(ccp(240,-100), ccp(240,30), 6.f/30.f, [=](CCPoint t_p)
																			  {
																				  take_card_button->setPosition(t_p);
																			  }, [=](CCPoint t_p)
																			  {
																				  take_card_button->setPosition(t_p);
																				  take_card_button->setEnabled(true);
																			  }));
								 }));
		
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		mySGD->network_check_cnt = 0;
		
		mySGD->refreshGoodsData(result_data["minusType"].asString(), result_data["minusCount"].asInt());
		
		addChild(ASPopupView::getCommonNoti(-9999, getLocal(LK::kMyLocalKey_noti), getLocal(LK::kMyLocalKey_notEnoughtP1)), 9999);
		
		loading_layer->removeFromParent();
		is_menu_enable = true;
	}
	else
	{
		mySGD->network_check_cnt++;
		
		if(mySGD->network_check_cnt >= mySGD->max_network_check_cnt)
		{
			mySGD->network_check_cnt = 0;
			
			ASPopupView *alert = ASPopupView::getCommonNotiTag(-99999,getLocal(LK::kMyLocalKey_reConnect), getLocal(LK::kMyLocalKey_reConnectAlert4),[=](){
				Json::Value param;
				param["memberID"] = myHSP->getSocialID();
				param["no"] = mySGD->card_compose_list[compose_idx].compose_card_number.getV();
				if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass11) > 0)
					param["exchangeID"] = NSDS_GS(kSDS_GI_shopComposeCardPass_exchangeID_s);
				else
					param["exchangeID"] = NSDS_GS(kSDS_GI_shopComposeCardStone_exchangeID_s);
				
				int material_count = target_card_number_list.size();
				for(int i=0;i<material_count;i++)
				{
					param["cards"][i] = target_card_number_list[i];
				}
				
				myHSP->command("composecard", param, json_selector(this, ComposeDetailPopup::resultCompose));
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
										 param["no"] = mySGD->card_compose_list[compose_idx].compose_card_number.getV();
										 if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass11) > 0)
											 param["exchangeID"] = NSDS_GS(kSDS_GI_shopComposeCardPass_exchangeID_s);
										 else
											 param["exchangeID"] = NSDS_GS(kSDS_GI_shopComposeCardStone_exchangeID_s);
										 
										 int material_count = target_card_number_list.size();
										 for(int i=0;i<material_count;i++)
										 {
											 param["cards"][i] = target_card_number_list[i];
										 }
										 
										 myHSP->command("composecard", param, json_selector(this, ComposeDetailPopup::resultCompose));
									 }));
		}
	}
}




