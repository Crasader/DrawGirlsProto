//
//  CharacterStrengthPopup.cpp
//  DGproto
//
//  Created by 사원3 on 2014. 12. 5..
//
//

#include "CharacterStrengthPopup.h"
#include "DataStorageHub.h"
#include "TouchSuctionLayer.h"
#include "CommonButton.h"
#include "CommonAnimation.h"
#include "KSLabelTTF.h"
#include "KsLocal.h"
#include "KSUtil.h"
#include "StageImgLoader.h"
#include "ServerDataSave.h"
#include "StarGoldData.h"
#include "ConvexGraph.h"
#include "AsyncImage.h"
#include "ScrollMenu.h"
#include "CCMenuLambda.h"
#include "LoadingLayer.h"
#include "ASPopupView.h"

CharacterStrengthPopup* CharacterStrengthPopup::create(int t_touch_priority, int t_character_idx, function<void()> t_end_func)
{
	CharacterStrengthPopup* t_popup = new CharacterStrengthPopup();
	if (t_popup && t_popup->init())
	{
		t_popup->myInit(t_touch_priority, t_character_idx, t_end_func);
		t_popup->autorelease();
		return t_popup;
	}
	else
	{
		CC_SAFE_DELETE(t_popup);
		return NULL;
	}
}

enum CharacterStrengthPopupZorder
{
	kCharacterStrengthPopupZorder_gray = 0,
	kCharacterStrengthPopupZorder_main_case
};

CharacterStrengthPopup::~CharacterStrengthPopup()
{
	while(!card_data_list.empty())
	{
		CardSortInfo* t_info = card_data_list.back();
		card_data_list.pop_back();
		delete t_info;
	}
}

void CharacterStrengthPopup::myInit(int t_touch_priority, int t_character_idx, function<void()> t_end_func)
{
	t_after_history = NULL;
	
	is_max = false;
	is_full = false;
	is_menu_enable = false;
	touch_priority = t_touch_priority;
	character_idx = t_character_idx;
	end_func = t_end_func;
	
	up_exp_value = 0;
	
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
	addChild(gray, kCharacterStrengthPopupZorder_gray);
	
	main_case = CCSprite::create("mainpopup2_back.png");
	main_case->setPosition(ccp(240,160-14.f));
	addChild(main_case, kCharacterStrengthPopupZorder_main_case);
	
	card_data_list.clear();
	int card_data_count = mySGD->getHasGottenCardsSize();
	for(int i=0;i<card_data_count;i++)
	{
		CardSortInfo t_info = mySGD->getHasGottenCardData(i);
		if(t_info.count.getV() >= 2)
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

//	CCSprite* list_back_img = CCSprite::create("whitePaper.png", CCRectMake(0, 0, 275, 175));
//	list_back_img->setOpacity(100);
//	list_back_img->setPosition(ccpFromSize(card_list_back->getContentSize()/2.f) + ccp(0,29));
//	card_list_back->addChild(list_back_img);

	
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
	
	CCScale9Sprite* character_back = CCScale9Sprite::create("common_grayblue.png", CCRectMake(0, 0, 26, 26), CCRectMake(12, 12, 2, 2));
	character_back->setContentSize(CCSizeMake(139, 177));
	character_back->setPosition(ccpFromSize(main_case->getContentSize()/2.f) + ccp(143,30.5f));
	main_case->addChild(character_back);
	
	CCSprite* character_title_back = CCSprite::create("cha_strength_box.png");
	character_title_back->setPosition(ccpFromSize(character_back->getContentSize()/2.f));
	character_back->addChild(character_title_back);
	
	
	KSLabelTTF* character_title = KSLabelTTF::create(NSDS_GS(kSDS_GI_characterInfo_int1_name_s, character_idx).c_str(), mySGD->getFont().c_str(), 16);
	character_title->setGradientColor(ccc4(255, 230, 10, 255), ccc4(255, 130, 7, 255), ccp(0,-1));
	character_title->setPosition(ccp(character_title_back->getContentSize().width/2.f, character_title_back->getContentSize().height - 25));
	character_title_back->addChild(character_title);
	
	CCSprite* light_back = KS::loadCCBI<CCSprite*>(this, "hell_cha_back.ccbi").first;
	light_back->setPosition(ccp(character_title_back->getContentSize().width/2.f, character_title_back->getContentSize().height/2.f-10));
	character_title_back->addChild(light_back);
	
	character_img = KS::loadCCBIForFullPath<CCSprite*>(this, mySIL->getDocumentPath() + NSDS_GS(kSDS_GI_characterInfo_int1_resourceInfo_ccbiID_s, character_idx) + ".ccbi").first;
	character_img->setPosition(ccp(character_title_back->getContentSize().width/2.f, character_title_back->getContentSize().height/2.f-1));
	character_title_back->addChild(character_img);
	
	int history_count = mySGD->getCharacterHistorySize();
	CharacterHistory t_history;
	for(int i=0;i<history_count;i++)
	{
		if(mySGD->getCharacterHistory(i).characterIndex.getV() == character_idx)
		{
			t_history = mySGD->getCharacterHistory(i);
			break;
		}
	}
	
	level_value = KSLabelTTF::create(ccsf(getLocal(LK::kMyLocalKey_levelValue), t_history.characterLevel.getV()), mySGD->getFont().c_str(), 13);
	level_value->setPosition(ccp(110,65));
	character_title_back->addChild(level_value);
	
	exp_graph = ConvexGraph::create("cha_graph_center.png", CCRectMake(0, 0, 15, 15), CCRectMake(7, 7, 1, 1), CCSizeMake(120, 15), ConvexGraphType::width);
	exp_graph->setPosition(ccp(character_title_back->getContentSize().width/2.f, 50));
	character_title_back->addChild(exp_graph);
	
	exp_graph->setCover("cha_graph_front.png", "cha_graph_center.png");
	exp_graph->setBack("cha_graph_back.png");
	
	int sub_base_value = t_history.characterNextLevelExp.getV() - t_history.characterCurrentLevelExp.getV();
	int sub_value = t_history.characterExp.getV() - t_history.characterCurrentLevelExp.getV();
	
	float recent_percentage = 100.f*sub_value/sub_base_value;
	if(t_history.characterLevel.getV() == NSDS_GI(kSDS_GI_characterInfo_int1_maxLevel_i, character_idx))
	{
		exp_graph->setPercentage(100.f);
		before_percentage = 100.f;
	}
	else
	{
		exp_graph->setPercentage(recent_percentage);
		before_percentage = recent_percentage;
	}
	
	exp_value = KSLabelTTF::create(ccsf("EXP %s/%s", KS::insert_separator(t_history.characterExp.getV()).c_str(), KS::insert_separator(t_history.characterNextLevelExp.getV()).c_str()), mySGD->getFont().c_str(), 11);
	exp_value->enableOuterStroke(ccBLACK, 1.f, int(255*0.6f), true);
	exp_value->setPosition(ccp(character_title_back->getContentSize().width/2.f, 49));
	character_title_back->addChild(exp_value);
	
	
	KSLabelTTF* maybe_exp_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_maybeStrengthenExp), mySGD->getFont().c_str(), 11);
	maybe_exp_label->setPosition(ccp(37,20));
	character_title_back->addChild(maybe_exp_label);
	
	maybe_exp_value = KSLabelTTF::create(KS::insert_separator(up_exp_value.getV()).c_str(), mySGD->getFont().c_str(), 25);
	maybe_exp_value->setGradientColor(ccc4(255, 230, 10, 255), ccc4(255, 130, 7, 255), ccp(0,-1));
	maybe_exp_value->setPosition(ccp(95,19));
	character_title_back->addChild(maybe_exp_value);
	
	
	CCScale9Sprite* character_tab = CCScale9Sprite::create("tabbutton_up.png", CCRectMake(0, 0, 74, 33), CCRectMake(36, 16, 2, 1));
	character_tab->setContentSize(CCSizeMake(85, 33));
	character_tab->setPosition(ccp(character_back->getContentSize().width/2.f, character_back->getContentSize().height-2.5f));
	character_back->addChild(character_tab);
	
	KSLabelTTF* character_tab_title = KSLabelTTF::create(getLocal(LK::kMyLocalKey_strengthenCharacter), mySGD->getFont().c_str(), 13);
	character_tab_title->setPosition(ccpFromSize(character_tab->getContentSize()/2.f));
	character_tab->addChild(character_tab_title);
	
	
	CCSprite* selected_card_back = CCSprite::create("cha_selectbox.png");
	selected_card_back->setPosition(ccp(card_list_back->getContentSize().width/2.f, selected_card_back->getContentSize().height/2.f+1));
	card_list_back->addChild(selected_card_back);
	
	KSLabelTTF* selected_card_list_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_selectedCardList), mySGD->getFont().c_str(), 10);
	selected_card_list_ment->enableOuterStroke(ccBLACK, 1, 127, true);
	selected_card_list_ment->setAnchorPoint(ccp(0,0));
	selected_card_list_ment->setPosition(ccp(6, selected_card_back->getContentSize().height - selected_card_list_ment->getContentSize().height-3.5f));
	selected_card_back->addChild(selected_card_list_ment);
	
	KSLabelTTF* selected_card_list_sub_ment = KSLabelTTF::create(getLocal(LK::kMyLocalKey_10cardsStrengthenPossibleAtOne), mySGD->getFont().c_str(), 8);
	selected_card_list_sub_ment->setColor(ccc3(30, 25, 70));
	selected_card_list_sub_ment->setAnchorPoint(ccp(0,0));
	selected_card_list_sub_ment->setPosition(ccp(selected_card_list_ment->getPositionX() + selected_card_list_ment->getContentSize().width + 5, selected_card_list_ment->getPositionY()));
	selected_card_back->addChild(selected_card_list_sub_ment);
	
	target_item_list.clear();
	target_card_number_list.clear();
	
	target_menu = CCMenuLambda::create();
	target_menu->setPosition(CCPointZero);
	selected_card_back->addChild(target_menu);
	target_menu->setTouchPriority(touch_priority-2);
	
	CCLabelTTF* t_button_label = CCLabelTTF::create();
	KSLabelTTF* strength_label = KSLabelTTF::create(getLocal(LK::kMyLocalKey_doStrengthen), mySGD->getFont().c_str(), 16);
	strength_label->setPosition(ccp(0,9.5f));
	t_button_label->addChild(strength_label);
	
	price_back = CCScale9Sprite::create("common_in.png", CCRectMake(0, 0, 20, 20), CCRectMake(9, 9, 2, 2));
	price_back->setContentSize(CCSizeMake(90, 20));
	price_back->setPosition(ccp(0,-9.5f));
	t_button_label->addChild(price_back);
	
	if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass10) > 0)
	{
		price_icon = CCSprite::create("icon_p10.png");
		price_icon->setScale(0.8f);
		price_value = KSLabelTTF::create(KS::insert_separator(mySGD->getGoodsValue(GoodsType::kGoodsType_pass10)).c_str(), mySGD->getFont().c_str(), 14);
		price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
		price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
		price_back->addChild(price_icon);
		price_back->addChild(price_value);
	}
	else
	{
		price_icon = CCSprite::create("price_gold_img.png");
		price_icon->setScale(0.8f);
		price_value = KSLabelTTF::create(KS::insert_separator(up_exp_value.getV()*mySGD->getGoldPerExp()).c_str(), mySGD->getFont().c_str(), 14);
		price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
		price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
		price_back->addChild(price_icon);
		price_back->addChild(price_value);
	}
	
	strength_button = CCControlButton::create(t_button_label, CCScale9Sprite::create("subbutton_purple4.png", CCRectMake(0, 0, 92, 45), CCRectMake(45, 22, 2, 1)));
	strength_button->setPreferredSize(CCSizeMake(147, 63));
	strength_button->setPosition(ccp(character_back->getPositionX(), 50));
	strength_button->addTargetWithActionForControlEvents(this, cccontrol_selector(CharacterStrengthPopup::strengthAction), CCControlEventTouchUpInside);
	main_case->addChild(strength_button);
	strength_button->setTouchPriority(touch_priority-2);
	
	strength_button->setEnabled(false);
	strength_button->setColor(ccGRAY);
	
	CommonButton* close_button = CommonButton::createCloseButton(touch_priority-1);
	close_button->setPosition(ccpFromSize(main_case->getContentSize()) + ccp(-20,-12));
	close_button->setFunction([=](CCObject* sender)
							  {
								  if(!is_menu_enable)
									  return;
								  
								  is_menu_enable = false;
								  
								  AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
								  
								  CommonAnimation::closePopup(this, main_case, gray, [=](){}, [=]()
								  {
									  end_func();
									  removeFromParent();
								  });
							  });
	main_case->addChild(close_button);
	
	CommonAnimation::openPopup(this, main_case, gray, [=](){}, [=]()
							   {
								   is_menu_enable = true;
							   });
}

CCTableViewCell* CharacterStrengthPopup::tableCellAtIndex( CCTableView *table, unsigned int idx )
{
	CCTableViewCell* cell = table->dequeueCell();
	
	cell = new CCTableViewCell();
	cell->init();
	cell->autorelease();
	
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
		
		CCMenuItem* t_card_item = CCMenuItemSprite::create(n_node, s_node, this, menu_selector(CharacterStrengthPopup::cardAction));
		t_card_item->setTag(card_number);
		
		ScrollMenu* t_card_menu = ScrollMenu::create(t_card_item, NULL);
		t_card_menu->setPosition(card_position);
		cell->addChild(t_card_menu);
		t_card_menu->setTouchPriority(touch_priority-2);
		
		if(is_max || is_full || t_info->count.getV() - target_count <= 1)
		{
			n_card->setGray(true);
			s_card->setGray(true);
			t_card_item->setEnabled(false);
		}
	}
	
	return cell;
}

void CharacterStrengthPopup::scrollViewDidScroll( CCScrollView* view )
{
	
}

void CharacterStrengthPopup::scrollViewDidZoom( CCScrollView* view )
{
	
}

void CharacterStrengthPopup::tableCellTouched( CCTableView* table, CCTableViewCell* cell )
{
	
}

CCSize CharacterStrengthPopup::cellSizeForTable( CCTableView *table )
{
	return CCSizeMake(275, 90);
}

CCSize CharacterStrengthPopup::tableCellSizeForIndex(CCTableView *table, unsigned int idx)
{
	return cellSizeForTable(table);
}

unsigned int CharacterStrengthPopup::numberOfCellsInTableView( CCTableView *table )
{
	int has_count = card_data_list.size();
	if(has_count == 0)
		return 0;
	else
		return (has_count-1)/4+1;
}

void CharacterStrengthPopup::strengthAction(CCObject* t_sender, CCControlEvent t_event)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	loading_layer = LoadingLayer::create(touch_priority-3);
	addChild(loading_layer);
	loading_layer->startLoading();
	
	Json::Value param;
	param["memberID"] = myHSP->getSocialID();
	param["characterNo"] = NSDS_GI(kSDS_GI_characterInfo_int1_no_i, character_idx);
	param["exp"] = up_exp_value.getV();
	bool is_use_pass = mySGD->getGoodsValue(GoodsType::kGoodsType_pass10) > 0;
	param["usePass"] = is_use_pass;
	if(is_use_pass)
		param["gold"] = 0;
	else
		param["gold"] = up_exp_value.getV() * mySGD->getGoldPerExp();
	for(int i=0;i<target_card_number_list.size();i++)
	{
		param["cards"][i] = target_card_number_list[i];
	}
	
	myHSP->command("strengthCharacter", param, json_selector(this, CharacterStrengthPopup::resultStrength));
}

void CharacterStrengthPopup::expUpAction()
{
	if(before_level == after_level)
	{
		after_exp = t_after_history->characterExp.getV();
		
		float after_percentage;
		after_percentage = 100.f * (t_after_history->characterExp.getV() - t_after_history->characterCurrentLevelExp.getV()) / (t_after_history->characterNextLevelExp.getV() - t_after_history->characterCurrentLevelExp.getV());
		
		addChild(KSGradualValue<float>::create(before_percentage, after_percentage, 0.3f, [=](float t_f)
											   {
												   exp_graph->setPercentage(t_f);
											   }, [=](float t_f)
											   {
												   exp_graph->setPercentage(t_f);
												   before_percentage = after_percentage;
											   }));
		
		addChild(KSGradualValue<int>::create(before_exp, after_exp, 0.3f, [=](int t_i)
											 {
												 exp_value->setString(ccsf("EXP %s/%s", KS::insert_separator(t_i).c_str(), KS::insert_separator(t_after_history->characterNextLevelExp.getV()).c_str()));
											 }, [=](int t_i)
											 {
												 exp_value->setString(ccsf("EXP %s/%s", KS::insert_separator(t_i).c_str(), KS::insert_separator(t_after_history->characterNextLevelExp.getV()).c_str()));
												 
												 if(after_level == NSDS_GI(kSDS_GI_characterInfo_int1_maxLevel_i, t_after_history->characterIndex.getV()))
												 {
													 is_max = true;
													 strength_button->setVisible(false);
													 
													 exp_value->setString("MAX");
												 }
												 
												 delete t_after_history;
												 t_after_history = NULL;
												 is_full = false;
												 
												 up_exp_value = 0;
												 maybe_exp_value->setString("0");
												 
												 price_icon->removeFromParent();
												 price_value->removeFromParent();
												 if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass10) > 0)
												 {
													 price_icon = CCSprite::create("icon_p10.png");
													 price_icon->setScale(0.8f);
													 price_value = KSLabelTTF::create(KS::insert_separator(mySGD->getGoodsValue(GoodsType::kGoodsType_pass10)).c_str(), mySGD->getFont().c_str(), 14);
													 price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
													 price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
													 price_back->addChild(price_icon);
													 price_back->addChild(price_value);
												 }
												 else
												 {
													 price_icon = CCSprite::create("price_gold_img.png");
													 price_icon->setScale(0.8f);
													 price_value = KSLabelTTF::create(KS::insert_separator(up_exp_value.getV()*mySGD->getGoldPerExp()).c_str(), mySGD->getFont().c_str(), 14);
													 price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
													 price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
													 price_back->addChild(price_icon);
													 price_back->addChild(price_value);
												 }
												 
												 strength_button->setColor(ccGRAY);
												 strength_button->setEnabled(false);
												 
												 CCPoint t_offset = card_table->getContentOffset();
												 card_table->reloadData();
												 card_table->setContentOffset(t_offset);
												 
												 loading_layer->removeFromParent();
												 is_menu_enable = true;
											 }));
	}
	else
	{
		exp_value->setString("LEVEL UP");
		addChild(KSGradualValue<float>::create(0, 100.f, 0.3f, [=](float t_f)
											   {
												   exp_graph->setPercentage(t_f);
											   }, [=](float t_f)
											   {
												   exp_graph->setPercentage(t_f);
												   before_level++;
												   before_percentage = 0;
												   level_value->setString(ccsf(getLocal(LK::kMyLocalKey_levelValue), before_level));
												   expUpAction();
											   }));
	}
}

void CharacterStrengthPopup::resultStrength(Json::Value result_data)
{
//	CCLog("result strength : \n%s", result_data.toStyledString().c_str());
	if(result_data["result"]["code"].asInt() == GDSUCCESS)
	{
		mySGD->network_check_cnt = 0;
		
		// 카드정보 갱신
		Json::Value cardData = result_data["cardData"];
		int card_data_size = cardData.size();
		for(int i=0;i<card_data_size;i++)
		{
			mySGD->refreshCardData(cardData[i]);
		}
		
		while(!card_data_list.empty())
		{
			CardSortInfo* t_info = card_data_list.back();
			card_data_list.pop_back();
			delete t_info;
		}
		card_data_list.clear();
		int card_data_count = mySGD->getHasGottenCardsSize();
		for(int i=0;i<card_data_count;i++)
		{
			CardSortInfo t_info = mySGD->getHasGottenCardData(i);
			if(t_info.count.getV() >= 2)
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
		
		
		CharacterHistory before_history;
		int character_number = result_data["characterData"]["characterNo"].asInt();
		int t_history_size = mySGD->getCharacterHistorySize();
		bool t_found = false;
		for(int i=0;!t_found && i<t_history_size;i++)
		{
			if(mySGD->getCharacterHistory(i).characterNo.getV() == character_number)
			{
				t_found = true;
				before_history = mySGD->getCharacterHistory(i);
			}
		}
		
		if(!t_found)
		{
			CCLOG("not found before strength target character history!!!!");
		}
		
		// 캐릭터정보 갱신
		mySGD->refreshCharacterHistory(result_data["characterData"]);
		
		CharacterHistory after_history;
		t_history_size = mySGD->getCharacterHistorySize();
		t_found = false;
		for(int i=0;!t_found && i<t_history_size;i++)
		{
			if(mySGD->getCharacterHistory(i).characterNo.getV() == character_number)
			{
				t_found = true;
				after_history = mySGD->getCharacterHistory(i);
			}
		}
		
		if(!t_found)
		{
			CCLOG("not found after strength target character history!!!!");
			t_after_history = new CharacterHistory();
		}
		else
		{
			t_after_history = new CharacterHistory();
			t_after_history->characterCurrentLevelExp = after_history.characterCurrentLevelExp.getV();
			t_after_history->characterExp = after_history.characterExp.getV();
			t_after_history->characterIndex = after_history.characterIndex.getV();
			t_after_history->characterLevel = after_history.characterLevel.getV();
			t_after_history->characterNextLevelExp = after_history.characterNextLevelExp.getV();
			t_after_history->characterNextLevelUpExp = after_history.characterNextLevelUpExp.getV();
			t_after_history->characterNo = after_history.characterNo.getV();
		}
		
		before_level = before_history.characterLevel.getV();
		after_level = after_history.characterLevel.getV();
		before_exp = before_history.characterExp.getV();
		
		function<void()> exp_up_func = [=]()
		{
			addChild(KSTimer::create(0.3f, [=]()
									 {
										 target_item_list.clear();
										 target_card_number_list.clear();
										 target_menu->removeAllChildren();
										 
										 expUpAction();
									 }));
		};
		
		
		for(int i=target_item_list.size()-1;i>=0;--i)
		{
			CCPoint b_world = target_menu->convertToWorldSpace(target_item_list[i]->getPosition());
			CCPoint b_main = main_case->convertToNodeSpace(b_world);
			
			CCPoint a_world = (character_img->getParent())->convertToWorldSpace(character_img->getPosition());
			CCPoint a_main = main_case->convertToNodeSpace(a_world);
			
			CCPoint sub_position = a_main - b_main;
			
			addChild(KSTimer::create(0.07f*(target_item_list.size()-1-i), [=]()
									 {
										 addChild(KSGradualValue<CCPoint>::create(target_item_list[i]->getPosition(), target_item_list[i]->getPosition() + sub_position, 0.25f, [=](CCPoint t_p)
																				  {
																					  target_item_list[i]->setPosition(t_p);
																				  }, [=](CCPoint t_p)
																				  {
																					  target_item_list[i]->setPosition(t_p);
//																					  target_item_list[i]->setVisible(false);
																					  if(i == 0)
																						{
																							exp_up_func();
																						}
																				  }));
									 }));
		}
	}
	else if(result_data["result"]["code"].asInt() == GDPROPERTYISMINUS)
	{
		mySGD->network_check_cnt = 0;
		
		mySGD->refreshGoodsData(result_data["minusType"].asString(), result_data["minusCount"].asInt());
		
		addChild(ASPopupView::getCommonNoti(-9999, myLoc->getLocalForKey(LK::kMyLocalKey_noti), myLoc->getLocalForKey(LK::kMyLocalKey_goldNotEnought)), 9999);
		
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
				param["characterNo"] = NSDS_GI(kSDS_GI_characterInfo_int1_no_i, character_idx);
				param["exp"] = up_exp_value.getV();
				bool is_use_pass = mySGD->getGoodsValue(GoodsType::kGoodsType_pass10) > 0;
				param["usePass"] = is_use_pass;
				if(is_use_pass)
					param["gold"] = 0;
				else
					param["gold"] = up_exp_value.getV() * mySGD->getGoldPerExp();
				for(int i=0;i<target_card_number_list.size();i++)
				{
					param["cards"][i] = target_card_number_list[i];
				}
				
				myHSP->command("strengthCharacter", param, json_selector(this, CharacterStrengthPopup::resultStrength));
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
										 param["characterNo"] = NSDS_GI(kSDS_GI_characterInfo_int1_no_i, character_idx);
										 param["exp"] = up_exp_value.getV();
										 bool is_use_pass = mySGD->getGoodsValue(GoodsType::kGoodsType_pass10) > 0;
										 param["usePass"] = is_use_pass;
										 if(is_use_pass)
											 param["gold"] = 0;
										 else
											 param["gold"] = up_exp_value.getV() * mySGD->getGoldPerExp();
										 for(int i=0;i<target_card_number_list.size();i++)
										 {
											 param["cards"][i] = target_card_number_list[i];
										 }
										 
										 myHSP->command("strengthCharacter", param, json_selector(this, CharacterStrengthPopup::resultStrength));
									 }));
		}
	}
}

void CharacterStrengthPopup::cardAction(CCObject *t_sender)
{
	if(!is_menu_enable)
		return;
	
	is_menu_enable = false;
	
	AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
	
	if(target_card_number_list.size() >= 10)
	{
		is_menu_enable = true;
		return;
	}
	
	int card_number = ((CCNode*)t_sender)->getTag();
	
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
	
	up_exp_value = up_exp_value.getV() + t_exp_value;
	maybe_exp_value->setString(KS::insert_separator(up_exp_value.getV()).c_str());
	if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass10) <= 0)
	{
		price_value->setString(KS::insert_separator(up_exp_value.getV() * mySGD->getGoldPerExp()).c_str());
		price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
		price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
		
		if(mySGD->getGoodsValue(GoodsType::kGoodsType_gold) < up_exp_value.getV() * mySGD->getGoldPerExp())
		{
			strength_button->setColor(ccGRAY);
			strength_button->setEnabled(false);
		}
		else
		{
			strength_button->setColor(ccWHITE);
			strength_button->setEnabled(true);
		}
	}
	else
	{
		strength_button->setColor(ccWHITE);
		strength_button->setEnabled(true);
	}
	
	CCMenuItemSpriteLambda* t_item = CCMenuItemSpriteLambda::create(n_node, s_node, [=](CCObject* sender)
																	{
																		if(!is_menu_enable)
																			return;
																		
																		is_menu_enable = false;
																		
																		AudioEngine::sharedInstance()->playEffect("se_button1.mp3", false);
																		
																		up_exp_value = up_exp_value.getV() - t_exp_value;
																		maybe_exp_value->setString(KS::insert_separator(up_exp_value.getV()).c_str());
																		if(mySGD->getGoodsValue(GoodsType::kGoodsType_pass10) <= 0)
																		{
																			price_value->setString(KS::insert_separator(up_exp_value.getV() * mySGD->getGoldPerExp()).c_str());
																			price_value->setPosition(ccp(price_icon->getContentSize().width*price_icon->getScale()/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f-1));
																			price_icon->setPosition(ccp(-price_value->getContentSize().width/2.f + price_back->getContentSize().width/2.f - 4, price_back->getContentSize().height/2.f));
																		}
																		
																		int idx = ((CCNode*)sender)->getTag();
																		target_card_number_list.erase(target_card_number_list.begin() + idx);
																		CCMenuItemSpriteLambda* tt_item = target_item_list[idx];
																		target_item_list.erase(target_item_list.begin() + idx);
																		
																		is_full = false;
																		
																		if(target_item_list.size() <= 0)
																		{
																			strength_button->setColor(ccGRAY);
																			strength_button->setEnabled(false);
																		}
																		else
																		{
																			if(mySGD->getGoodsValue(GoodsType::kGoodsType_gold) >= up_exp_value.getV() * mySGD->getGoldPerExp())
																			{
																				strength_button->setColor(ccWHITE);
																				strength_button->setEnabled(true);
																			}
																		}
																		
																		for(int i=0;i<target_item_list.size();i++)
																		{
																			target_item_list[i]->setTag(i);
																			target_item_list[i]->setPosition(ccp(23 + i*28, 23.5f));
																		}
																		
																		addChild(KSTimer::create(0.1f, [=]()
																								 {
																									 CCPoint t_offset = card_table->getContentOffset();
																									 card_table->reloadData();
																									 card_table->setContentOffset(t_offset);
																									 
																									 is_menu_enable = true;
																									 
																									 tt_item->removeFromParent();
																								 }));
																	});
	t_item->setTag(target_item_list.size());
	t_item->setPosition(ccp(20 + target_item_list.size()*26.4f, 23.5f));
	target_menu->addChild(t_item);
	
	
	target_item_list.push_back(t_item);
	target_card_number_list.push_back(card_number);
	
	if(target_card_number_list.size() >= 10)
		is_full = true;
	
	CCPoint t_offset = card_table->getContentOffset();
	card_table->reloadData();
	card_table->setContentOffset(t_offset);
	
	is_menu_enable = true;
}


