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
	
//	vector<KSProtectVar<int>> t_material_list
	for(int i=0;i<material_count;i++)
	{
		CardSortInfo t_info = mySGD->getHasGottenCardDataForCardNumber(t_material_list[i].getV());
		CCPoint t_position = ccp(0,0);
		if(i == 0)
			t_position = ccp(42,130);
		else if(i == 1)
			t_position = ccp(62,50);
		else if(i == 2)
			t_position = ccp(100,100);
		if(t_info.count.getV() >= 2)
		{
			
		}
		else
		{
			CCSprite* not_have_back = CCSprite::create("cardsetting_off.png");
			not_have_back->setPosition(t_position);
			necessary_back->addChild(not_have_back);
			
			KSLabelTTF* need_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_necessaryPlease), mySGD->getFont().c_str(), 11);
			need_ment->setPosition(ccpFromSize(not_have_back->getContentSize()/2.f) + ccp(0,10));
			not_have_back->addChild(need_ment);
			
			KSLabelTTF* no_label = KSLabelTTF::create(ccsf("No.%d", t_info.card_number.getV()), mySGD->getFont().c_str(), 12);
			no_label->setColor(ccc3(255, 220, 150));
			no_label->setPosition(ccpFromSize(not_have_back->getContentSize()/2.f) + ccp(0,-10));
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
	sum_exp_label->setPosition(ccpFromSize(selected_card_back->getContentSize()) + ccp(-5,-10));
	selected_card_back->addChild(sum_exp_label);
	
//	KSLabelTTF* selected_card_list_sub_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_10cardsStrengthenPossibleAtOne), mySGD->getFont().c_str(), 8);
//	selected_card_list_sub_ment->setColor(ccc3(30, 25, 70));
//	selected_card_list_sub_ment->setAnchorPoint(ccp(0,0));
//	selected_card_list_sub_ment->setPosition(ccp(selected_card_list_ment->getPositionX() + selected_card_list_ment->getContentSize().width + 5, selected_card_list_ment->getPositionY()));
//	selected_card_back->addChild(selected_card_list_sub_ment);
	
	target_card_number_list.clear();
	
	CCSprite* selected_back = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 264, 35));
	selected_back->setOpacity(100);
	selected_back->setAnchorPoint(ccp(0,0));
	selected_back->setPosition(ccp(7,6));
	selected_card_back->addChild(selected_back);
	
	selected_table = CCTableView::create(this, CCSizeMake(264, 35));
	selected_table->setDirection(CCScrollViewDirection::kCCScrollViewDirectionHorizontal);
	selected_table->setVerticalFillOrder(kCCTableViewFillTopDown);
	selected_table->setPosition(ccp(7,6));
	selected_table->setDelegate(this);
	selected_card_back->addChild(selected_table);
	selected_table->setTouchPriority(touch_priority-1);
	
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
	}
	else
	{
		price_icon = CCSprite::create("price_gold_img.png");
		price_icon->setScale(0.8f);
		price_value = KSLabelTTF::create(KS::insert_separator(mySGD->card_compose_list[compose_idx].need_exp.getV()*mySGD->getGoldPerExp()).c_str(), mySGD->getFont().c_str(), 14);
		price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
		price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
		price_back->addChild(price_icon);
		price_back->addChild(price_value);
	}
	
	compose_button = CCControlButton::create(t_button_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	compose_button->setPreferredSize(CCSizeMake(147, 63));
	compose_button->setPosition(ccp(necessary_back->getPositionX(), 50));
	compose_button->addTargetWithActionForControlEvents(this, cccontrol_selector(ComposeDetailPopup::composeAction), CCControlEventTouchUpInside);
	main_case->addChild(compose_button);
	compose_button->setTouchPriority(touch_priority-2);
	
	compose_button->setEnabled(false);
	compose_button->setColor(ccGRAY);
	
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
			
			if(t_info->count.getV() - target_count <= 1)
			{
				n_card->setGray(true);
				s_card->setGray(true);
				t_card_item->setEnabled(false);
			}
		}
	}
	else if(table == selected_table)
	{
		
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
		return CCSizeMake(275, 90);
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
		return 0;
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
	
}

void ComposeDetailPopup::cardAction(CCObject *t_sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	
	
}

